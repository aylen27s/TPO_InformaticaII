#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Configuraciones de conexiones manuales de slots

    //Inicialización de elementos de UI
    initWidgets();

}

MainWindow::~MainWindow()
{
    MY_DB.close();
    qDebug()<< "Cierre de conexion.";

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

    }else{
        ui->calendarWidget_periodo->setEnabled(false);
        ui->calendarWidget_periodo->setSelectedDate(QDate::currentDate());
        ui->dateEdit_maxDay->setDisabled(true);
        ui->dateEdit_minDay->setDisabled(true);
    }
}

void MainWindow::on_calendarWidget_periodo_selectionChanged()
{
    qDebug()<< "jelou";
    // revisar para hacer dinámico esto
}

void MainWindow::initWidgets(){
    //Por defecto las mediciones del día
    ui->radioButton_dia->setChecked(true);
    setCalendar(false);

    //encapsular todo estooo
    QDate dMin = QDate::currentDate().addDays(-7);
    ui->dateEdit_maxDay->setDate(QDate::currentDate());
    ui->dateEdit_minDay->setDate(dMin);
    ui->dateEdit_maxDay->setDisabled(true);
    ui->dateEdit_minDay->setDisabled(true);


    // prueba de la grafica
    plotData("Hora","Presion");

}

void MainWindow::plotData(QString _xName, QString _yName){
    myData = new QLineSeries(); // instancia arreglo de puntos

    //añade puntos al arreglo lineal
    myData->append(0, 6);
    myData->append(2, 4);
    myData->append(3, 8);
    myData->append(7, 4);
    myData->append(10, 5);


    //Dispone el arreglo en un chart o tablero
    myChart = new QChart();
    myChart->legend()->hide(); //muestra etiquetas de la grafica
    myChart->addSeries(myData);
    myChart->createDefaultAxes();
    myChart->axisX()->setTitleText(_xName);
    myChart->axisY()->setTitleText(_yName);

    QChartView *chartView = new QChartView(myChart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // lo manda al layout puesto desde el GUI del form
    ui->horizontalLayout_chart->addWidget(chartView);
}
