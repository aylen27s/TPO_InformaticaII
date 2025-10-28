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

//Headers de forms de configuracion
#include "conexion.h"
#include "preferences.h"

//Definiciones propias de la app
#define TIME_TO_REFRESH     1000            //Tiempo para el muestro en tiempo real
#define LIMIT_RECORDS       150
#define MY_IP_SERVER        "192.168.0.72"  //Del server ESP
#define MY_PORT_SERVER      10234           //Puerto en en el que escucha el server ESP

//Valores por defecto
#define PS_MAX_DEF      150
#define PS_MIN_DEF      110
#define PD_MAX_DEF      70
#define PD_MIN_DEF      98

//Definicion de enumeraciones para manejo del plot
enum MyModePlot {LAST_SAMPLE,DAY_SAMPLE,PERIOD_SAMPLE};




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
    void insertDataBaseInfo(int _ps,int _pd, QString _fecha); //Metodo para insertar los datos que llegan por TCP del ESP
    void handleConexion(QString, QString);
    void handlePreferences(mConfig);

private slots:
    void plotData(); // slot para dibujar/actualizar los datos
    void switchToLiveView();    // Slot para reanudar el modo en vivo
    void onReadyRead();         // Slot para accionar cuando el server manda información

    void on_liveViewButton_clicked();
    void on_calendarWidget_clicked(const QDate &date);
    void on_pushButton_import_clicked();
    void on_pushButton_export_clicked();
    void on_actionConexi_n_triggered();

    void on_actionPreferencias_triggered();

private:
    Ui::MainWindow *ui;

    QTimer *dataTimer;                  // timer para refrescar
    QDate m_selectedDate;               //Atributo para guardar el dia seleccionado
    double m_viewingWindowSeconds;
    QTcpSocket m_socket;                //Socket para comunicacion con ESP

    MyModePlot m_modePlot;              //flag para indicar al plot que mostrará un rango de fechas. Por defecto la UI muestra los datos del dia actual.
    QString m_dateMin;                  //atributo para guardar los rangos de fechas a filtrar en plot. Solo se utilizan cuando m_modePlot es PERIOD_SAMPLE
    QString m_dateMax;                  //idem m_dateMin
    QVector<double> timestamps;
    QVector<double> values;
    mConfig m_configPreferences;    // vector de strings para
    //Ventana de configuracion de para asignarle red WiFi al dispositivo.
    Conexion *m_uiConexion;
    Preferences *m_uiPreferences;
};
#endif // MAINWINDOW_H
