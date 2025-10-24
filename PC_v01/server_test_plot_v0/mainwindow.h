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

//
#include "datadb.h"

//Defino intervalo para refrescar las muestras para el plot en tiempo real
#define TIME_TO_REFRESH 5000

//Defino enumeraciones para manejo del plot
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
    void insertDataBaseInfo(int _ps,int _pd, QString _fecha);

private slots:
    void plotData(); // slot para dibujar/actualizar los datos
    // void userInteracted();
    void on_liveViewButton_clicked();
    void on_calendarWidget_clicked(const QDate &date);
    void switchToLiveView();    // Slot para reanudar el modo en vivo
    void onReadyRead();         // Slot para accionar cuando el server manda información


    void on_pushButton_import_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;    // objeto para la conexión a la DB
    QTimer *dataTimer;  // timer para refrescar
    // bool m_isUserExploring = false; // flag para saber si se esta explorando el gráfico
    QDate m_selectedDate;
    double m_viewingWindowSeconds;
    // QTimer *m_inactivityTimer;  // Timer para detectar inactividad del usuario --inhabilio para que solo vuelva a muestreo en tiempo real por accion manual.
    QTcpSocket m_socket;        //Socket para comunicacion con ESP

    MyModePlot m_modePlot = LAST_SAMPLE; //flag para indicar al plot que mostrará un rango de fechas. Por defecto la UI muestra los datos del dia actual.
    QString m_dateMin;                  //atributo para guardar los rangos de fechas a filtrar en plot. Solo se utilizan cuando m_modePlot es PERIOD_SAMPLE
    QString m_dateMax;                  //idem m_dateMin
};
#endif // MAINWINDOW_H
