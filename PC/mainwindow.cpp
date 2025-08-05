#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Configuraciones de conexiones manuales de slots, si los hubiera */

    /* Inicialización de elementos de UI */
    initWidgets();

}

MainWindow::~MainWindow()
{
    /*Cierra conexión con la DB*/
    MY_DB.close();
    qDebug()<< "Cierre de conexion DB.";

    /* Libera memoria */

    delete myChart;
    delete myData;
    delete ui;
}

void MainWindow::on_radioButton_periodo_clicked()
{
    setCalendar(true);
}

void MainWindow::on_radioButton_dia_clicked()
{
    setCalendar(false);
}

void MainWindow::setCalendar(bool active){
    if(active){
        /* Se activan los controles del calendario para seleccionar el rango de fechas*/
        ui->calendarWidget_periodo->setEnabled(true);
        ui->dateEdit_maxDay->setEnabled(true);
        ui->dateEdit_minDay->setEnabled(true);
        ui->calendarWidget_periodo->setMaximumDate(QDate::currentDate());


        QTextCharFormat styleDisabledDate;
        styleDisabledDate.setForeground(QColor('lightgray'));

        QDate currDay = QDate::currentDate();

        int counterDays =  currDay.daysInMonth() - currDay.day();

        qDebug()<<counterDays;

        for (int var= 0; var < counterDays; ++var) {
            // qDebug()<<"Cambio color a " << currDay; // funciona pero si aparecen otros dias del mes en el calendario, no los pinta
            ui->calendarWidget_periodo->setDateTextFormat(currDay, styleDisabledDate);
            currDay = currDay.addDays(1);
        }


        /* Traer los datos de la DB */

        /* Hacer el plot de los datos segun el rango de fechas */

    }else{
        /* Deshabilitar los controles del calendario y de rango de fechas*/
        ui->calendarWidget_periodo->setEnabled(false);
        ui->calendarWidget_periodo->setSelectedDate(QDate::currentDate());
        ui->dateEdit_maxDay->setDisabled(true);
        ui->dateEdit_minDay->setDisabled(true);

        /* Traer datos del dia actual */

        /* Plotear datos del dia actual */
    }
}

void MainWindow::on_calendarWidget_periodo_selectionChanged()
{
    qDebug()<< "jelou";
    // revisar para hacer dinámico esto
}

void MainWindow::initWidgets(){
    /* Por defecto al iniciar el programa, se cargan las mediciones del dia actual */
    ui->radioButton_dia->setChecked(true);
    setCalendar(false);

    //Rango de fechas para seleccionar por periodo
    setPeriod( QDate::currentDate() );


    // prueba de la grafica
    plotData("Hora","Presion");
    GetData();

}

void MainWindow::setPeriod(QDate date){
    QDate date_init = date.addDays(-7);
    ui->dateEdit_maxDay->setDate(date);
    ui->dateEdit_minDay->setDate(date_init);
    ui->dateEdit_maxDay->setDisabled(true);
    ui->dateEdit_minDay->setDisabled(true);
}

void MainWindow::plotData(QString _xName, QString _yName){

    myData = new QLineSeries(); // instancia arreglo de puntos

    //Añadi
    myData->append(0, 6);
    myData->append(2, 4);
    myData->append(3, 8);
    myData->append(7, 4);
    myData->append(10, 5);


    //Dispone el arreglo en un chart o tablero
    myChart = new QChart();
    myChart->legend()->hide(); //Muestra etiquetas de la grafica
    myChart->addSeries(myData);
    myChart->createDefaultAxes();
    myChart->axisX()->setTitleText(_xName);
    myChart->axisY()->setTitleText(_yName);

    QChartView *chartView = new QChartView(myChart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // lo manda al layout puesto desde el GUI del form
    ui->horizontalLayout_chart->addWidget(chartView);
}

void MainWindow::GetData(){
    QString myQuery = QString("SELECT * FROM %1").arg(DB_TABLE);
    qDebug()<< myQuery;

    QSqlQuery getQuery(MY_DB);
    getQuery.prepare(myQuery);

    if (!getQuery.exec()) {
        qDebug() << "Error al buscar los datos:" << getQuery.lastError().text();
    }else{
        while (getQuery.next())
            qDebug() << getQuery.value("fecha").toString() << getQuery.value("ps").toString(); // output all names
    }

}
