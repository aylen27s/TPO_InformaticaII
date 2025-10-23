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
    void userInteracted();
    void on_liveViewButton_clicked();
    void on_calendarWidget_clicked(const QDate &date);
    void switchToLiveView();    // Slot para reanudar el modo en vivo
    void onReadyRead();         // Slot para accionar cuando el server manda información


private:
    Ui::MainWindow *ui;
    QSqlDatabase db;    // objeto para la conexión a la DB
    QTimer *dataTimer;  // timer para refrescar
    bool m_isUserExploring = false; // flag para saber si se esta explorando el gráfico
    QDate m_selectedDate;
    double m_viewingWindowSeconds;
    QTimer *m_inactivityTimer;  // Timer para detectar inactividad del usuario
    QTcpSocket m_socket;        //Socket para comunicacion con ESP
};
#endif // MAINWINDOW_H
