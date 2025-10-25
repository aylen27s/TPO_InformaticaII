#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include <QString>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QTimer> // automáticamente se actualiza el gráfico
#include <QDate>

//Para manejo de las solicitudes TCP
#include <QTcpServer>
#include <QTcpSocket>

//Para exportar datos
#include <QFile>
#include <QTextStream>

//Archivos propios de la app
#include "datadb.h"

//Definiciones propias de la app
#define TIME_TO_REFRESH 1000
#define LIMIT_RECORDS 150

//Definicion de enumeraciones para manejo del plot
enum MyModePlot {LAST_SAMPLE,DAY_SAMPLE,PERIOD_SAMPLE};

//Definicion de estructura para almacenar las muestras
struct MySample {
    double ps;
// int pd;
    QDateTime fecha;
}typedef MySample;


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void insertDataBaseInfo(int _ps,int _pd, QString _fecha);

private slots:
    void plotData(); // slot para dibujar/actualizar los datos
    void on_liveViewButton_clicked();
    void on_calendarWidget_clicked(const QDate &date);
    void switchToLiveView();    // Slot para reanudar el modo en vivo
    void onReadyRead();         // Slot para accionar cuando el server manda información


    void on_pushButton_import_clicked();

    void on_pushButton_export_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *dataTimer;                  // timer para refrescar
    QDate m_selectedDate;               //Atributo para guardar el dia seleccionado
    double m_viewingWindowSeconds;
    QTcpSocket m_socket;                //Socket para comunicacion con ESP

    MyModePlot m_modePlot;              //flag para indicar al plot que mostrará un rango de fechas. Por defecto la UI muestra los datos del dia actual.
    QString m_dateMin;                  //atributo para guardar los rangos de fechas a filtrar en plot. Solo se utilizan cuando m_modePlot es PERIOD_SAMPLE
    QString m_dateMax;                  //idem m_dateMin
    // QVector<MySample> m_queryResults;
    QVector<double> timestamps;
    QVector<double> values;
};
#endif // MAINWINDOW_H
