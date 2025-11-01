#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // *** conexión a la base de datos de sqlite ***
    MY_DB.setDatabaseName(DB_PATH);                                                          // ruta del archivo sqlite que se va a abrir
    if (!MY_DB.open()) {                                                                     // intenta abrir conexión, si falla entra al if
        qDebug() << "Error: " << MY_DB.lastError().text();                                   // comentario por consola si falló la conexión
    } else {
        qDebug() << "Base de datos conectada exitosamente.";
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

    /* ----- Inicialización de elementos de UI -----*/
    ui->dateEdit_minDay->setDate(QDate::currentDate().addDays(-7));
    ui->dateEdit_maxDay->setDate(QDate::currentDate());

    ui->dateEdit_maxDay->setMaximumDate(QDate::currentDate());              // restrinjo valores maximos de rango de fechas para que la query no sea tan pesada
    // ui->dateEdit_minDay->setMinimumDate(QDate::currentDate().addDays(-28));
    ui->calendarWidget->setMaximumDate(QDate::currentDate());

    /* --- Inicializacion de atributos de la clase --- */
    m_dateMin = ui->dateEdit_minDay->date().toString("yyyy-MM-dd");
    m_dateMax = ui->dateEdit_maxDay->date().toString("yyyy-MM-dd");
    m_selectedDate = QDate().currentDate();     // inicializa m_selectedDate con un QDate vacío
    m_viewingWindowSeconds = 300;               // ajusta la ventana de visualización en 300 [s] (últimas muestras almacenadas en los útlimos 5 minutos)
    m_modePlot = LAST_SAMPLE;                   //Por defecto, plotea en tiempo real las últimas 1000 muestras [los datos de la última hora, aproximadamente]
    // m_configPreferences = {PS_MAX_DEF,PS_MIN_DEF,PD_MAX_DEF,PD_MIN_DEF,TIME_TO_REFRESH};                   //Setea los valores limite de presion por defecto

    // *** configuración del timer para actualización automática ***
    dataTimer = new QTimer(this);                                       // se crea un QTImer para actualizaciones periódicas
    connect(dataTimer, &QTimer::timeout, this, &MainWindow::plotData);  // conecta la señal timeout del timer a la función plotData() para actualizar el gráfico cada vez que el timer expire

    /* --- Acciones iniciales de la app --- */
    plotData();                         //Primer llamado al plot al iniciar la app.
    dataTimer->start(m_configPreferences.tSample);  // se llamará a plotData cada 1000 [ms]

    // QDateTime testDate = QDateTime::currentDateTime();
    // double testDateSec= testDate.toSecsSinceEpoch();
    // qDebug()<< testDateSec;
    // qDebug()<< QDateTime::fromSecsSinceEpoch((qint64)testDateSec);
}

MainWindow::~MainWindow()
{
    MY_DB.close();
    if (!MY_DB.isOpen()) {
        qDebug() << "Conexión con DB finalizada.";
        return;
    }
    delete ui;                 // libera la memoria del objeto ui creado en el constructor anteriormente
}

// *** DEFINICIONES DE LOS SLOTS DEFAULTS de UI ***


void MainWindow::on_liveViewButton_clicked()                                  // restaura la vista de muestreo en tiempo real tocando el botón
{
    qDebug() << "Volviendo al modo de seguimiento en tiempo real (botón).";
    switchToLiveView();                                                         // reanuda el modo muestreo en tiempo real
}


void MainWindow::on_calendarWidget_clicked(const QDate &date)                       // slot que se ejecuta cuando se clickea una fecha del calendario (se conecta el calendario a la db)
{
    qDebug() << "Mostrando datos para la fecha:" << date.toString("yyyy-MM-dd");
    m_selectedDate = date;                                                          // guarda la fecha seleccionada en m_selectedDate para referencia interna
    m_modePlot = DAY_SAMPLE;
    dataTimer->stop();//Freno timer para que el usuario pueda visualizar los datos el tiempo que quiera
    plotData(); //Llamo al plot

}

void MainWindow::on_pushButton_import_clicked()
{
    /* Metodo para llamar al plot por rango de fechas */
    m_modePlot = PERIOD_SAMPLE;
    m_dateMin = ui->dateEdit_minDay->date().toString("yyyy-MM-dd");
    m_dateMax = ui->dateEdit_maxDay->date().toString("yyyy-MM-dd");
    dataTimer->stop(); //Freno timer para que el usuario pueda visualizar los datos el tiempo que quiera
    plotData();
    // qDebug()<< ui->dateEdit_minDay->date().toString("yyyy-MM-dd") << ui->dateEdit_maxDay->date().toString("yyyy-MM-dd");

}

/* ------------------ Metodos custom de la app ------------------ */

/* --- Plot de datos --- */
void MainWindow::plotData()                                     // consulta la base de datos y plotea los datos en un gráfico
{
    // QVector<double> timestamps;                                 // vector que almacena timestamps (segundos desde epoch) que irán al eje x
    // QVector<double> values;                                     // vector que almacena valores sensados de presión arterial que irán al eje y

    //Resetear los vectores para plot
    timestamps.clear();
    values.clear();

    if (!MY_DB.isOpen()) {
        qDebug() << "La base de datos no está abierta.";
        return;
    }

    QSqlQuery query(MY_DB);
    QString myQuery;

    switch(m_modePlot){ //Cambia la Query segun la acción del usuario. Por defecto LAST_SAMPLE.
    case LAST_SAMPLE:
        myQuery = QString("SELECT fecha, ps FROM data ORDER BY fecha DESC LIMIT %1").arg(LIMIT_RECORDS);
        break;

    case DAY_SAMPLE:
        myQuery = QString("SELECT fecha, ps FROM data WHERE fecha LIKE %1 ORDER BY fecha ASC").arg("\"" + m_selectedDate.toString("yyyy-MM-dd") + "%" + "\"");
        break;

    case PERIOD_SAMPLE:
        myQuery = QString("SELECT fecha, ps FROM data WHERE fecha BETWEEN %1 AND %2 ORDER BY fecha ASC").arg("\""+m_dateMin+"\"").arg("\""+m_dateMax+" 23:59:59"+"\"");
        break;

    default:
        m_modePlot = LAST_SAMPLE;
    }


    // qDebug() << myQuery;

    query.prepare(myQuery);

    if (!query.exec()) {
        qDebug() << "Error en la consulta:" << query.lastError().text();
        return;
    }

    if(!query.size()){
        qDebug() << "Error en la consulta";

    } else {
        // qDebug() << "La consulta arrojó resultados.";

        int totalRecords = 0;
        while (query.next()) {
            // recorre los resultados de la consulta a la db fila por fila
            QDateTime dt = QDateTime::fromString(query.value(0).toString(), "yyyy-MM-dd HH:mm:ss");     // convierte el campo (fecha (string)) a QDateTime en ese formato

            timestamps.push_back(dt.toSecsSinceEpoch());                                                  // inserta al inicio del vector timestamps el valor en segundos desde epoch para mantener el orden ascendente cronológico (se usa prepend para eso ya que las levantó en orden descendente)

            values.push_back(query.value(1).toDouble());                                                  // inserta al inicio del vector values el valor númerico del valor sensado
            totalRecords++;

        }// antes de push_back, estaba con "prepend" revisar diferencia
        // ~ ACLARACIÓN: dt.toSecsSinceEpoch() devuelve un entero muy grande que representa la cantidad de segundos transcurridos desde la época (epoch) [1 de enero de 1970 a las 00:00:00 UTC] ~
        // ~ este número entero sirve para graficar en el eje x, se evitan problemas de formato. QCPAxisTickerDateTime se encarga de asignarle a este número la fecha/hora linda en el eje       ~

        // qDebug()<< "Cantidad de registros seleccionados:" << totalRecords ;

        if (0 >= totalRecords) {
            QMessageBox msgBox;
            msgBox.setText("No se registran datos para la selección actual. Intente nuevamente con otra fecha.");
            msgBox.setInformativeText("Se volverá al modo Muestreo en tiempo real.");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
            switchToLiveView();
            return;                                                                                     // no hay datos para graficar
        } else {
            if (ui->customPlot->graphCount() == 0) {                                                        // verifica si el customPlot ya tiene algun gráfico añadido. Si no, crea uno
                ui->customPlot->addGraph();                                                                 // lo crea
            }

            ui->customPlot->graph(0)->setData(timestamps, values);                                          // asigna los vectores timestamps y values al primer gráfico
            ui->customPlot->xAxis->setRange(timestamps.first() - m_viewingWindowSeconds, timestamps.last());
            ui->customPlot->yAxis->rescale();                                                               // reescala el eje y en base a los valores sensados
            QCPRange range = ui->customPlot->yAxis->range();                                                // obtiene el rango resultante del eje y en base a los valores sensados (mínimo y máximo)
            ui->customPlot->yAxis->setRange(range.lower - 5, range.upper + 5);                              // margen visual. 5 para arriba, 5 para abajo
            ui->customPlot->replot();                                                                       // grafica todo con los datos y rangos actualizados
        }

    }
}

/* --- Configuracion para volver a modo de plot en tiempo real --- */
void MainWindow::switchToLiveView(){
    ui->calendarWidget->setSelectedDate(QDate::currentDate());                  // ajusta la fecha del calendario a la fecha que se está muestreando
    m_selectedDate = QDate().currentDate();
    m_modePlot = LAST_SAMPLE;
    dataTimer->start(m_configPreferences.tSample);
    plotData();
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

/* --- Insercion de datos  que llegan por TCP a DB --- */
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
}



/* --- Metodo para exportar los datos de la seleccion actual --- */
void MainWindow::on_pushButton_export_clicked()
{
    //Defino la ruta y el nombre del archivo. Por defecto fecha y hora para que no haya duplicidad
    QString myNameFile = QString("/home/aylen/Escritorio/test-export/%1.csv").arg(QDateTime::currentDateTime().toString(Qt::ISODateWithMs));

    //Intento abrir el archivo, o en su defecto se crea.
    QFile archivo(myNameFile);

    if (archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        //Declara donde se van a escribir los datos
        QTextStream newLineFile(&archivo);

        //Recorre las muestras filtradas actualmente y las concatena en un string para escribirlo en el archivo
        for (int i = 0; i < timestamps.size(); i++) {
            QString lineData = QString("%1,%2\n").arg(QDateTime::fromSecsSinceEpoch(timestamps[i]).toString("yyyy-MM-dd HH:mm:ss")).arg(QString::number((int)values[i]));
            newLineFile <<lineData;
        }
        qDebug() << "Archivo creado v2";
    } else {
        qDebug() << "Error al crear o abrir el archivo:" << archivo.errorString();
    }

    archivo.close();
}

/* --- Metodo para abrir ventana de conexion de red wifi del dispositivo ---- */
void MainWindow::on_actionConexi_n_triggered()
{
    //Instanciar nueva ventana emergente de tipo Conexion
    m_uiConexion = new Conexion(this);

    //Conectar la accion de la ventana emergente con el slot correspondiente
    connect(m_uiConexion, &Conexion::sendConexionToMain, this, &MainWindow::handleConexion);

    //Abrir la ventana emergente de manera bloqueante.
    m_uiConexion->exec();

}


void MainWindow::on_actionPreferencias_triggered()
{
    //Instanciar nueva ventana emergente de tipo Conexion
    m_uiPreferences = new Preferences(m_configPreferences,this);

    //Conectar la accion de la ventana emergente con el slot correspondiente
    connect(m_uiPreferences, &Preferences::sendPreferencesToMain, this, &MainWindow::handlePreferences);
    connect(this, &MainWindow::updateStatus, m_uiPreferences, &Preferences::readSatus);

    //Abrir la ventana emergente de manera bloqueante.
    m_uiPreferences->exec();
}

void MainWindow::handleConexion(QString ssid, QString psw){
    //Logica para pasar datos a ESP via UART para configurar el WiFi
    qDebug()<<"Recibiendo datos de Conexion: "<< ssid << psw;
}

void MainWindow::handlePreferences(MConfigData data){
    qDebug()<<"Recibiendo datos de Preferencias... "<< data.psMax;
    //Chequear si hay diferencias y hacer replot/alarmas
    if(data == m_configPreferences){
        emit updateStatus(false);
    }else{
        emit updateStatus(true);
        m_configPreferences = data;
        qDebug()<< "Se actualizó la configuración psMax nuevo: "<< m_configPreferences.psMax;
    }
}

