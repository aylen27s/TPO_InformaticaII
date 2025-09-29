#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer> // automáticamente se actualiza el gráfico
#include <QDate>


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

private slots:
     void plotData(); // slot para dibujar/actualizar los datos
    void userInteracted();
    void on_liveViewButton_clicked();
    void on_calendarWidget_clicked(const QDate &date);
    void switchToLiveView(); // Slot para reanudar el modo en vivo


private:
    Ui::MainWindow *ui;
    QSqlDatabase db;    // objeto para la conexión a la DB
    QTimer *dataTimer;  // timer para refrescar
    bool m_isUserExploring = false; // flag para saber si se esta explorando el gráfico
    QDate m_selectedDate;
    double m_viewingWindowSeconds;
    QTimer *m_inactivityTimer; // Timer para detectar inactividad del usuario
};
#endif // MAINWINDOW_H
