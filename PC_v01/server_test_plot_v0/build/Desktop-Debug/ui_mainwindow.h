/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionConexi_n;
    QAction *actionRangos;
    QWidget *centralwidget;
    QCustomPlot *customPlot;
    QCalendarWidget *calendarWidget;
    QPushButton *liveViewButton;
    QLabel *label_visualizar;
    QDateEdit *dateEdit_maxDay;
    QDateEdit *dateEdit_minDay;
    QPushButton *pushButton_export;
    QPushButton *pushButton_import;
    QLabel *label_visualizar_2;
    QMenuBar *menubar;
    QMenu *menuConfiguraci_n;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1028, 508);
        actionConexi_n = new QAction(MainWindow);
        actionConexi_n->setObjectName("actionConexi_n");
        actionRangos = new QAction(MainWindow);
        actionRangos->setObjectName("actionRangos");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        customPlot = new QCustomPlot(centralwidget);
        customPlot->setObjectName("customPlot");
        customPlot->setGeometry(QRect(390, 30, 591, 361));
        calendarWidget = new QCalendarWidget(centralwidget);
        calendarWidget->setObjectName("calendarWidget");
        calendarWidget->setGeometry(QRect(50, 70, 296, 179));
        liveViewButton = new QPushButton(centralwidget);
        liveViewButton->setObjectName("liveViewButton");
        liveViewButton->setGeometry(QRect(570, 410, 231, 31));
        label_visualizar = new QLabel(centralwidget);
        label_visualizar->setObjectName("label_visualizar");
        label_visualizar->setGeometry(QRect(50, 30, 141, 31));
        dateEdit_maxDay = new QDateEdit(centralwidget);
        dateEdit_maxDay->setObjectName("dateEdit_maxDay");
        dateEdit_maxDay->setGeometry(QRect(140, 300, 91, 26));
        dateEdit_minDay = new QDateEdit(centralwidget);
        dateEdit_minDay->setObjectName("dateEdit_minDay");
        dateEdit_minDay->setGeometry(QRect(50, 300, 91, 26));
        pushButton_export = new QPushButton(centralwidget);
        pushButton_export->setObjectName("pushButton_export");
        pushButton_export->setGeometry(QRect(50, 360, 141, 25));
        pushButton_import = new QPushButton(centralwidget);
        pushButton_import->setObjectName("pushButton_import");
        pushButton_import->setGeometry(QRect(250, 300, 88, 25));
        label_visualizar_2 = new QLabel(centralwidget);
        label_visualizar_2->setObjectName("label_visualizar_2");
        label_visualizar_2->setGeometry(QRect(50, 270, 161, 31));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1028, 22));
        menuConfiguraci_n = new QMenu(menubar);
        menuConfiguraci_n->setObjectName("menuConfiguraci_n");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuConfiguraci_n->menuAction());
        menuConfiguraci_n->addAction(actionConexi_n);
        menuConfiguraci_n->addAction(actionRangos);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionConexi_n->setText(QCoreApplication::translate("MainWindow", "Conexi\303\263n", nullptr));
        actionRangos->setText(QCoreApplication::translate("MainWindow", "Rangos", nullptr));
        liveViewButton->setText(QCoreApplication::translate("MainWindow", "Muestreo en Tiempo Real", nullptr));
        label_visualizar->setText(QCoreApplication::translate("MainWindow", "Visualizar por d\303\255a", nullptr));
        pushButton_export->setText(QCoreApplication::translate("MainWindow", "Exportar datos", nullptr));
        pushButton_import->setText(QCoreApplication::translate("MainWindow", "Ver", nullptr));
        label_visualizar_2->setText(QCoreApplication::translate("MainWindow", "Visualizar por per\303\255odo", nullptr));
        menuConfiguraci_n->setTitle(QCoreApplication::translate("MainWindow", "Configuraci\303\263n", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
