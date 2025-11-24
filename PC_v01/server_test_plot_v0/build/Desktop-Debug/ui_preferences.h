/********************************************************************************
** Form generated from reading UI file 'preferences.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCES_H
#define UI_PREFERENCES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Preferences
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QFrame *line;
    QPlainTextEdit *te_pd_max;
    QPlainTextEdit *te_pd_min;
    QLabel *label_t_sample;
    QPlainTextEdit *te_ps_max;
    QLabel *label_ps_min;
    QFrame *line_2;
    QLabel *label_ps_max;
    QLabel *label_pd_max;
    QPlainTextEdit *te_ps_min;
    QLabel *label_pd_min;
    QPlainTextEdit *te_t_sample;
    QPushButton *pushButton_aplicar;
    QPushButton *pushButton_cerrar;
    QLabel *label_changesUpdate;

    void setupUi(QDialog *Preferences)
    {
        if (Preferences->objectName().isEmpty())
            Preferences->setObjectName("Preferences");
        Preferences->resize(541, 483);
        gridLayoutWidget = new QWidget(Preferences);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(90, 60, 341, 280));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        line = new QFrame(gridLayoutWidget);
        line->setObjectName("line");
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 4, 0, 1, 1);

        te_pd_max = new QPlainTextEdit(gridLayoutWidget);
        te_pd_max->setObjectName("te_pd_max");
        te_pd_max->setMaximumSize(QSize(150, 50));

        gridLayout->addWidget(te_pd_max, 2, 1, 1, 1);

        te_pd_min = new QPlainTextEdit(gridLayoutWidget);
        te_pd_min->setObjectName("te_pd_min");
        te_pd_min->setMaximumSize(QSize(150, 50));

        gridLayout->addWidget(te_pd_min, 3, 1, 1, 1);

        label_t_sample = new QLabel(gridLayoutWidget);
        label_t_sample->setObjectName("label_t_sample");

        gridLayout->addWidget(label_t_sample, 6, 0, 1, 1);

        te_ps_max = new QPlainTextEdit(gridLayoutWidget);
        te_ps_max->setObjectName("te_ps_max");
        te_ps_max->setMaximumSize(QSize(150, 50));

        gridLayout->addWidget(te_ps_max, 0, 1, 1, 1);

        label_ps_min = new QLabel(gridLayoutWidget);
        label_ps_min->setObjectName("label_ps_min");

        gridLayout->addWidget(label_ps_min, 1, 0, 1, 1);

        line_2 = new QFrame(gridLayoutWidget);
        line_2->setObjectName("line_2");
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 4, 1, 1, 1);

        label_ps_max = new QLabel(gridLayoutWidget);
        label_ps_max->setObjectName("label_ps_max");

        gridLayout->addWidget(label_ps_max, 0, 0, 1, 1);

        label_pd_max = new QLabel(gridLayoutWidget);
        label_pd_max->setObjectName("label_pd_max");

        gridLayout->addWidget(label_pd_max, 2, 0, 1, 1);

        te_ps_min = new QPlainTextEdit(gridLayoutWidget);
        te_ps_min->setObjectName("te_ps_min");
        te_ps_min->setMaximumSize(QSize(150, 50));

        gridLayout->addWidget(te_ps_min, 1, 1, 1, 1);

        label_pd_min = new QLabel(gridLayoutWidget);
        label_pd_min->setObjectName("label_pd_min");

        gridLayout->addWidget(label_pd_min, 3, 0, 1, 1);

        te_t_sample = new QPlainTextEdit(gridLayoutWidget);
        te_t_sample->setObjectName("te_t_sample");
        te_t_sample->setMaximumSize(QSize(150, 50));

        gridLayout->addWidget(te_t_sample, 6, 1, 1, 1);

        pushButton_aplicar = new QPushButton(gridLayoutWidget);
        pushButton_aplicar->setObjectName("pushButton_aplicar");

        gridLayout->addWidget(pushButton_aplicar, 7, 0, 1, 1);

        pushButton_cerrar = new QPushButton(gridLayoutWidget);
        pushButton_cerrar->setObjectName("pushButton_cerrar");

        gridLayout->addWidget(pushButton_cerrar, 7, 1, 1, 1);

        gridLayout->setColumnStretch(0, 2);
        gridLayout->setColumnStretch(1, 1);
        label_changesUpdate = new QLabel(Preferences);
        label_changesUpdate->setObjectName("label_changesUpdate");
        label_changesUpdate->setGeometry(QRect(90, 360, 341, 16));

        retranslateUi(Preferences);

        QMetaObject::connectSlotsByName(Preferences);
    } // setupUi

    void retranslateUi(QDialog *Preferences)
    {
        Preferences->setWindowTitle(QCoreApplication::translate("Preferences", "Dialog", nullptr));
        label_t_sample->setText(QCoreApplication::translate("Preferences", "Tiempo de muestreo", nullptr));
        label_ps_min->setText(QCoreApplication::translate("Preferences", "Presi\303\263n sist\303\263lica m\303\255nima", nullptr));
        label_ps_max->setText(QCoreApplication::translate("Preferences", "Presion sist\303\263lica m\303\241xima", nullptr));
        label_pd_max->setText(QCoreApplication::translate("Preferences", "Presi\303\263n diast\303\263lica m\303\241xima ", nullptr));
        label_pd_min->setText(QCoreApplication::translate("Preferences", "Presi\303\263n diast\303\263lica m\303\255nima ", nullptr));
        pushButton_aplicar->setText(QCoreApplication::translate("Preferences", "Aplicar", nullptr));
        pushButton_cerrar->setText(QCoreApplication::translate("Preferences", "Cerrar", nullptr));
        label_changesUpdate->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Preferences: public Ui_Preferences {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCES_H
