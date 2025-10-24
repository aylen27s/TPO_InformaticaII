#include "mainwindow.h"
#include "ui_mainwindow.h"


//Defino utilidades para establecer conexión con el server
#define MY_IP_SERVER    "192.168.0.72"  //Del server ESP
#define MY_PORT_SERVER  10234           //Puerto en en el que escucha el server ESP

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_selectedDate = QDate().currentDate();                                                           // inicializa m_selectedDate con un QDate vacío
    qDebug()<<m_selectedDate;
    m_viewingWindowSeconds = 300;                                                         // ajusta la ventana de visualización en 300 [s] (últimas muestras almacenadas en los útlimos 5 minutos)

    // *** conexión a la base de datos de sqlite ***
    // db = QSqlDatabase::addDatabase("QSQLITE", "miConexion");                              // crea una conexión de base de datos con el driver QSQLITE y la etiqueta "miConexion"
    MY_DB.setDatabaseName(DB_PATH);                                                          // ruta del archivo sqlite que se va a abrir
    if (!MY_DB.open()) {                                                                     // intenta abrir conexión, si falla entra al if
        qDebug() << "Error: " << MY_DB.lastError().text();                                   // comentario por consola si falló la conexión
    } else {
        qDebug() << "Base de datos conectada exitosamente.";                              // conexión exitosa
    }

    /* ---- Configuración de socket cliente TCP ----*/
    m_socket.connectToHost(QHostAddress(MY_IP_SERVER), MY_PORT_SERVER);                 //Equivalente a hacer el bind entre socket y direccion
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));                 //readyRead() es signal de QT

    // *** configuración inicial de QCustomPlot ***
    ui->customPlot->xAxis->setLabel("Fecha y Hora");                                      // etiqueta del eje x
    ui->customPlot->yAxis->setLabel("Presión arterial");                                  // etiqueta del eje y
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);          // crea un QCPAxisTickerDateTime gestionado por QSharedPointer para formateo de fechas en el eje x (convertirá ticks a fecha/hora).
    dateTicker->setDateTimeFormat("hh:mm:ss\nyyyy-MM-dd");                                // define el formato de fecha/hora para el tick del eje x
    ui->customPlot->xAxis->setTicker(dateTicker);                                         // asigna el tick de fecha/hora al eje x del customPlot
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);                   // habilita interacciones de arrastre y zoom con la rueda del mouse para el gráfico
    // connect(ui->customPlot, &QCustomPlot::mousePress, this, &MainWindow::userInteracted); // deteca la interacción del usuario en el gráfico con el mouse y la conecta al slot userInteracted
    // connect(ui->customPlot, &QCustomPlot::mouseWheel, this, &MainWindow::userInteracted); // detecta la interacción del usuario en el gráfico con la rueda del mouse y la conecta al mismo slot

    // *** configuración del timer para actualización automática ***
    dataTimer = new QTimer(this);                                                         // se crea un QTImer para actualizaciones periódicas
    connect(dataTimer, &QTimer::timeout, this, &MainWindow::plotData);                    // conecta la señal timeout del timer a la función plotData() para actualizar el gráfico cada vez que el timer expire
    dataTimer->start(TIME_TO_REFRESH);                                                               // se llamará a plotData cada 1000 [ms]
    // *** configuración del timer para volver al modo muestro en tiempo real automáticamente ***
    // m_inactivityTimer = new QTimer(this);                                                           // timer para medir la inactividad del usuario
    // m_inactivityTimer->setSingleShot(true);                                                         // se dispara solo una vez
    // connect(m_inactivityTimer, &QTimer::timeout, this, &MainWindow::switchToLiveView);              // conecta su timeout al slot switchToLiveView para volver al modo de muestreo en tiempo real
                                                                                                    // comienza a graficar
    // ~ ACLARACIÓN: Si el usuario está viendo muestras pasadas en el gráfico y se detecta inactividad, volverá automáticamente despues del conteo establecido a las muestras del presente ~

    /* ----- Inicialización de elementos de UI -----*/
    ui->dateEdit_minDay->setDate(QDate::currentDate().addDays(-7));
    ui->dateEdit_maxDay->setDate(QDate::currentDate());

    ui->dateEdit_maxDay->setMaximumDate(QDate::currentDate());              // restrinjo valores maximos de rango de fechas para que la query no sea tan pesada
    // ui->dateEdit_minDay->setMinimumDate(QDate::currentDate().addDays(-28));
    ui->calendarWidget->setMaximumDate(QDate::currentDate());

    m_dateMin = ui->dateEdit_minDay->date().toString("yyyy-MM-dd");
    m_dateMax = ui->dateEdit_maxDay->date().toString("yyyy-MM-dd");

    plotData();
}

MainWindow::~MainWindow()
{
    MY_DB.close();                // cierra la conexión con la base de datos al cerrar la ventana
    if (!MY_DB.isOpen()) {    // verifica que la conexión esté abierta
        qDebug() << "Conexión con DB cerrada.";
        return;
    }
    delete ui;                 // libera la memoria del objeto ui creado en el constructor anteriormente
}

// *** DEFINICIONES DE LOS SLOTS ***
// void MainWindow::userInteracted()       // se llama cuando el usuario interactúa con el gŕafico
// {
//     m_isUserExploring = true;           // flag que indica que el usuario está interactuando con el gráfico
//     m_inactivityTimer->start(5000);     // si luego de 5000 [ms] (5 segundos) no se detectan interacciones se vuelve al modo muestro en tiempo real. Si se detectan, se reinicia el timer.
// }

void MainWindow::switchToLiveView()     // restaura la vista de muestro de datos en tiempo real
{
    //m_selectedDate = QDate();
    // m_isUserExploring = false;                                  // flag que indica que el usuario no está interactuando con el gráfico
    // ui->calendarWidget->setSelectedDate(QDate::currentDate());  // ajusta la fecha del calendario a la fecha que se está muestreando
    m_modePlot = LAST_SAMPLE;
    dataTimer->start(TIME_TO_REFRESH);
    plotData();                                                 // comienza a graficar
}

void MainWindow::plotData()                                     // consulta la base de datos y plotea los datos en un gráfico
{

    // if (m_isUserExploring) {                                    // si el usuario está explorando no se actualiza el gráfico
    //     return;
    // }

    QVector<double> timestamps;                                 // vector que almacena timestamps (segundos desde epoch) que irán al eje x
    QVector<double> values;                                     // vector que almacena valores sensados de presión arterial que irán al eje y

    /*QSqlDatabase db = QSqlDatabase::database("miConexion");     // obtiene la conexión a la base de datos previamente establecida
    if (!db.isOpen()) { */
    if (!MY_DB.isOpen()) {    // verifica que la conexión esté abierta
        qDebug() << "La base de datos no está abierta.";
        return;
    }

    QSqlQuery query(MY_DB);
    QString myQuery;

    switch(m_modePlot){
        case LAST_SAMPLE:
            myQuery = QString("SELECT fecha, ps FROM data ORDER BY fecha DESC LIMIT 50");
            // query.prepare("SELECT fecha, ps FROM data ORDER BY fecha DESC LIMIT 1000");
            break;

        case DAY_SAMPLE:
            myQuery = QString("SELECT fecha, ps FROM data WHERE fecha LIKE %1 ORDER BY fecha ASC").arg("\"" + m_selectedDate.toString("yyyy-MM-dd") + "%" + "\"");
            // query.prepare("SELECT fecha, ps FROM data WHERE fecha LIKE :dateFilter ORDER BY fecha ASC");    // busca los datos cuya columna fecha sea la que se seleccionó en el calendario
            // query.bindValue(":dateFilter", m_selectedDate.toString("yyyy-MM-dd") + "%");
            break;

        case PERIOD_SAMPLE:
            myQuery = QString("SELECT fecha, ps FROM data WHERE fecha BETWEEN %1 AND %2 ORDER BY fecha ASC").arg("\""+m_dateMin+"\"").arg("\""+m_dateMax+" 23:59:59"+"\"");
            // query.prepare("SELECT fecha, ps FROM data WHERE fecha BETWEEN :minDay AND :maxDay ORDER BY fecha ASC");
            // query.bindValue(":minDay", m_dateMin);
            // query.bindValue(":maxDay", m_dateMax);
            break;

        default:
            m_modePlot = LAST_SAMPLE;
    }


    qDebug() << myQuery;

    // interacción con la base de datos
    // QSqlQuery query("SELECT fecha, ps FROM data ORDER BY fecha DESC LIMIT 1000", MY_DB);               // se limita en 1000 muestras los datos que levanta

    query.prepare(myQuery);

    if (!query.exec()) {
        qDebug() << "Error en la consulta:" << query.lastError().text();
        return;
    }

    if(!query.size()){
        qDebug() << "La consulta no arrojó resultados.";
    } else {
        qDebug() << "Hay registros.";
    }

    while (query.next()) {
        // recorre los resultados de la consulta a la db fila por fila
        QDateTime dt = QDateTime::fromString(query.value(0).toString(), "yyyy-MM-dd HH:mm:ss");     // convierte el campo (fecha (string)) a QDateTime en ese formato
        timestamps.push_back(dt.toSecsSinceEpoch());                                                  // inserta al inicio del vector timestamps el valor en segundos desde epoch para mantener el orden ascendente cronológico (se usa prepend para eso ya que las levantó en orden descendente)
        values.push_back(query.value(1).toDouble());                                                  // inserta al inicio del vector values el valor númerico del valor sensado
        qDebug()<< "Record -->" << dt << query.value(1).toDouble() ;
    }// antes de push_back, estaba con "prepend" revisar diferencia
    // ~ ACLARACIÓN: dt.toSecsSinceEpoch() devuelve un entero muy grande que representa la cantidad de segundos transcurridos desde la época (epoch) [1 de enero de 1970 a las 00:00:00 UTC] ~
    // ~ este número entero sirve para graficar en el eje x, se evitan problemas de formato. QCPAxisTickerDateTime se encarga de asignarle a este número la fecha/hora linda en el eje       ~

    if (timestamps.isEmpty()) {
        qDebug()<<"Vector de tiempos vacio";
        return;                                                                                     // no hay datos para graficar
    }

    if (ui->customPlot->graphCount() == 0) {                                                        // verifica si el customPlot ya tiene algun gráfico añadido. Si no, crea uno
        ui->customPlot->addGraph();                                                                 // lo crea
    }

    ui->customPlot->graph(0)->setData(timestamps, values);                                          // asigna los vectores timestamps y values al primer gráfico

    //Esto es para cuando estamos en live_view
    // double lastTimestamp = timestamps.last();                                                       // guarda el ultimo valor del vector timestamps
    // ui->customPlot->xAxis->setRange(lastTimestamp - m_viewingWindowSeconds, lastTimestamp);         // ajusta el rango del eje x para ver desde el intervalo que configuramos al inicio hasta el último valor

    ui->customPlot->xAxis->setRange(timestamps.first() - m_viewingWindowSeconds, timestamps.last());

    ui->customPlot->yAxis->rescale();                                                               // reescala el eje y en base a los valores sensados
    QCPRange range = ui->customPlot->yAxis->range();                                                // obtiene el rango resultante del eje y en base a los valores sensados (mínimo y máximo)
    ui->customPlot->yAxis->setRange(range.lower - 5, range.upper + 5);                              // margen visual. 5 para arriba, 5 para abajo

    ui->customPlot->replot();                                                                       // grafica todo con los datos y rangos actualizados
}


void MainWindow::on_liveViewButton_clicked()                                  // restaura la vista de muestreo en tiempo real tocando el botón
{
    qDebug() << "Volviendo al modo de seguimiento en tiempo real (botón).";

    // m_inactivityTimer->stop();                                                  // cancela el retorno automático
    ui->calendarWidget->setSelectedDate(QDate::currentDate());                  // ajusta la fecha del calendario a la fecha que se está muestreando
    m_selectedDate = QDate().currentDate();
    // switchToLiveView();                                                         // reanuda el modo muestreo en tiempo real
    m_modePlot = LAST_SAMPLE;
    dataTimer->start(TIME_TO_REFRESH);
    // plotData();
}

void MainWindow::on_calendarWidget_clicked(const QDate &date)                       // slot que se ejecuta cuando se clickea una fecha del calendario (se conecta el calendario a la db)
{
    qDebug() << "Mostrando datos para la fecha:" << date.toString("yyyy-MM-dd");

    // m_isUserExploring = true;                                                       // flag que indica que el usuario está interactuando con el calendario
    m_selectedDate = date;                                                          // guarda la fecha seleccionada en m_selectedDate para referencia interna

    /*--------------------- V2 ------------------*/
    m_modePlot = DAY_SAMPLE;
    dataTimer->stop();//Freno el timer, para que deje de actualizar en tiempo real
    plotData(); //Llamo al plot

    /* -------------------- V1 ------------------*/
    // QVector<double> timestamps;                                                     // vector que almacena timestamps de la fecha seleccionada
    // QVector<double> values;                                                         // vector que almacena valores sensados de presión arterial de la fecha seleccionada

    // // QSqlDatabase db = QSqlDatabase::database("miConexion");                         // obtiene la conexión a la base de datos previamente establecida
    // // if(!db.isOpen()){
    // if(!MY_DB.isOpen()){
    //     qDebug() << "La base de datos no está abierta.";
    //     return;
    // }

    // QSqlQuery query(MY_DB);                                                                            // interacción con la base de datos
    // query.prepare("SELECT fecha, ps FROM data WHERE fecha LIKE :dateFilter ORDER BY fecha ASC");    // busca los datos cuya columna fecha sea la que se seleccionó en el calendario
    // query.bindValue(":dateFilter", date.toString("yyyy-MM-dd") + "%");                              // se filtran todas las entradas de esa fecha

    // if (!query.exec()) {
    //     qDebug() << "Error en la consulta de fecha:" << query.lastError().text();
    //     return;
    // }

    // while(query.next()){                                                                            // recorre las filas una por una
    //     QDateTime dt = QDateTime::fromString(query.value(0).toString(), "yyyy-MM-dd HH:mm:ss");     // convierte la fecha (string) a QDateTime
    //     timestamps.push_back(dt.toSecsSinceEpoch());                                                // añade al final del vector timestamps el valor en segundos desde epoch (orden ascendente de la consulta a la db)
    //     values.push_back(query.value(1).toDouble());                                                // añade al final del vector values el valor numérico del valor sensado
    // }

    // if(ui->customPlot->graphCount() == 0){
    //     ui->customPlot->addGraph();
    // }

    // ui->customPlot->graph(0)->setData(timestamps, values);                                          // asigna los vectores de tiempos y valores al gráfico para mostrar los datos de la fecha

    // // se ajustan los ejes para que se vean todos los datos de ese día
    // ui->customPlot->rescaleAxes();
    // QCPRange range = ui->customPlot->yAxis->range();
    // ui->customPlot->yAxis->setRange(range.lower - 5, range.upper + 5);
    // ui->customPlot->replot();

    // se inicial el timer por si el usuario deja la vista del calendario inactiva
    // m_inactivityTimer->start(15000); // 15 segundos
}

/* --- Lectura de trama TCP --- */
void MainWindow::onReadyRead(){

    QByteArray tcpData = m_socket.readAll();    //Lectura de info que llega por socket. Input esperado SSS,DDD,FFFFFFFFFFFFFFFFFFF
    QString fullStringResponse(tcpData);        //Parseo a string
    QStringList dataResponse = fullStringResponse.split(',');   //Separo cada dato que me interesa guardar

    int _ps = dataResponse.at(0).toFloat();
    int _pd = dataResponse.at(1).toFloat();
    QString _date = dataResponse.at(2);

    // qDebug() << "Trama recibida" <<tcpData << "--fin--";
    qDebug() << "Datos spliteados:" <<_ps<<_pd<< _date; //no me funcionaba el internet cuando quise probar esto :(

    insertDataBaseInfo(_ps,_pd,_date); //Escribo en DB local

}

/* --- Metodos custom de la app --- */

void MainWindow::insertDataBaseInfo(int _ps, int _pd, QString _fecha){
    QString myQuery = QString("INSERT INTO %1 (ps, pd, fecha) VALUES (:ps, :pd, :fecha)").arg(DB_TABLE);
    // qDebug()<< myQuery;

    QSqlQuery insertQuery(MY_DB);
    insertQuery.prepare(myQuery);

    insertQuery.bindValue(":ps", _ps);
    insertQuery.bindValue(":pd", _pd);
    insertQuery.bindValue(":fecha", _fecha);


    if (!insertQuery.exec()) {
        qDebug() << "Error al insertar datos:" << insertQuery.lastError().text();
    }else{
        qDebug() << "Datos insertados correctamente.";
    }

    // qDebug()<<"Saliendo por insertDataBaseInfo";

}

void MainWindow::on_pushButton_import_clicked()
{
    /* Metodo para llamar al plot por rango de fechas */
    m_modePlot = PERIOD_SAMPLE;
    m_dateMin = ui->dateEdit_minDay->date().toString("yyyy-MM-dd");
    m_dateMax = ui->dateEdit_maxDay->date().toString("yyyy-MM-dd");
    dataTimer->stop(); //freno timer para que el usuario pueda visualizar los datos el tiempo que quiera
    plotData();
    // qDebug()<< ui->dateEdit_minDay->date().toString("yyyy-MM-dd") << ui->dateEdit_maxDay->date().toString("yyyy-MM-dd");

}

