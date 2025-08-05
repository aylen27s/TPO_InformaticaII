#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QtCharts>
#include <QDebug>
#include "datosDB.h"

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
    void setCalendar(bool active);
    void plotData(QString _xName, QString _yName);


private slots:
    void on_radioButton_periodo_clicked();

    void on_radioButton_dia_clicked();

    void on_calendarWidget_periodo_selectionChanged();

private:
    Ui::MainWindow *ui;
    QChart *myChart;
    QLineSeries *myData;
    void initWidgets();
};
#endif // MAINWINDOW_H
