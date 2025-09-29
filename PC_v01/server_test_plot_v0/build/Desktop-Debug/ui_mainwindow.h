/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QCustomPlot *customPlot;
    QCalendarWidget *calendarWidget;
    QPushButton *liveViewButton;
    QRadioButton *radioButton_periodo;
    QLabel *label_visualizar;
    QDateEdit *dateEdit_maxDay;
    QDateEdit *dateEdit_minDay;
    QRadioButton *radioButton_dia;
    QPushButton *pushButton_export;
    QPushButton *pushButton_import;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1353, 614);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        customPlot = new QCustomPlot(centralwidget);
        customPlot->setObjectName("customPlot");
        customPlot->setGeometry(QRect(320, 20, 591, 361));
        calendarWidget = new QCalendarWidget(centralwidget);
        calendarWidget->setObjectName("calendarWidget");
        calendarWidget->setGeometry(QRect(10, 100, 296, 179));
        liveViewButton = new QPushButton(centralwidget);
        liveViewButton->setObjectName("liveViewButton");
        liveViewButton->setGeometry(QRect(480, 390, 231, 31));
        radioButton_periodo = new QRadioButton(centralwidget);
        radioButton_periodo->setObjectName("radioButton_periodo");
        radioButton_periodo->setGeometry(QRect(20, 70, 111, 23));
        label_visualizar = new QLabel(centralwidget);
        label_visualizar->setObjectName("label_visualizar");
        label_visualizar->setGeometry(QRect(20, 0, 101, 31));
        dateEdit_maxDay = new QDateEdit(centralwidget);
        dateEdit_maxDay->setObjectName("dateEdit_maxDay");
        dateEdit_maxDay->setGeometry(QRect(100, 70, 81, 26));
        dateEdit_minDay = new QDateEdit(centralwidget);
        dateEdit_minDay->setObjectName("dateEdit_minDay");
        dateEdit_minDay->setGeometry(QRect(190, 70, 81, 26));
        radioButton_dia = new QRadioButton(centralwidget);
        radioButton_dia->setObjectName("radioButton_dia");
        radioButton_dia->setGeometry(QRect(20, 30, 111, 23));
        pushButton_export = new QPushButton(centralwidget);
        pushButton_export->setObjectName("pushButton_export");
        pushButton_export->setGeometry(QRect(150, 290, 88, 25));
        pushButton_import = new QPushButton(centralwidget);
        pushButton_import->setObjectName("pushButton_import");
        pushButton_import->setGeometry(QRect(50, 290, 88, 25));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1353, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        liveViewButton->setText(QCoreApplication::translate("MainWindow", "Muestreo en Tiempo Real", nullptr));
        radioButton_periodo->setText(QCoreApplication::translate("MainWindow", "Periodo", nullptr));
        label_visualizar->setText(QCoreApplication::translate("MainWindow", "Visualizar por", nullptr));
        radioButton_dia->setText(QCoreApplication::translate("MainWindow", "D\303\255a actual", nullptr));
        pushButton_export->setText(QCoreApplication::translate("MainWindow", "Exportar", nullptr));
        pushButton_import->setText(QCoreApplication::translate("MainWindow", "Ver", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
