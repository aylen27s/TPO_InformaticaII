/********************************************************************************
** Form generated from reading UI file 'conexion.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONEXION_H
#define UI_CONEXION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Conexion
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label_ssid;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_psw;
    QTextEdit *textEdit_ssid;
    QTextEdit *textEdit_psw;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pb_conectar;
    QPushButton *pb_cerrar;

    void setupUi(QDialog *Conexion)
    {
        if (Conexion->objectName().isEmpty())
            Conexion->setObjectName("Conexion");
        Conexion->resize(525, 292);
        gridLayoutWidget = new QWidget(Conexion);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(40, 50, 431, 81));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(38, 13, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 1, 1, 1);

        label_ssid = new QLabel(gridLayoutWidget);
        label_ssid->setObjectName("label_ssid");

        gridLayout->addWidget(label_ssid, 0, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 1, 1, 1);

        label_psw = new QLabel(gridLayoutWidget);
        label_psw->setObjectName("label_psw");

        gridLayout->addWidget(label_psw, 1, 0, 1, 1);

        textEdit_ssid = new QTextEdit(gridLayoutWidget);
        textEdit_ssid->setObjectName("textEdit_ssid");

        gridLayout->addWidget(textEdit_ssid, 0, 2, 1, 1);

        textEdit_psw = new QTextEdit(gridLayoutWidget);
        textEdit_psw->setObjectName("textEdit_psw");

        gridLayout->addWidget(textEdit_psw, 1, 2, 1, 1);

        horizontalLayoutWidget = new QWidget(Conexion);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(40, 150, 431, 41));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        pb_conectar = new QPushButton(horizontalLayoutWidget);
        pb_conectar->setObjectName("pb_conectar");
        pb_conectar->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_2->addWidget(pb_conectar);

        pb_cerrar = new QPushButton(horizontalLayoutWidget);
        pb_cerrar->setObjectName("pb_cerrar");

        horizontalLayout_2->addWidget(pb_cerrar);


        retranslateUi(Conexion);

        QMetaObject::connectSlotsByName(Conexion);
    } // setupUi

    void retranslateUi(QDialog *Conexion)
    {
        Conexion->setWindowTitle(QCoreApplication::translate("Conexion", "Dialog", nullptr));
        label_ssid->setText(QCoreApplication::translate("Conexion", "SSID", nullptr));
        label_psw->setText(QCoreApplication::translate("Conexion", "Contrase\303\261a", nullptr));
        pb_conectar->setText(QCoreApplication::translate("Conexion", "Conectar", nullptr));
        pb_cerrar->setText(QCoreApplication::translate("Conexion", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Conexion: public Ui_Conexion {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONEXION_H
