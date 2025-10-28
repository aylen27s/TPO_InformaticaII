#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(mConfig data, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Preferences)
{
    ui->setupUi(this);

    ui->te_pd_max->setPlainText(QString::number(data.pdMax)); //Cargo los valores actuales para que el usuario sepa la configuracion actual y pueda editarla
    ui->te_pd_min->setPlainText(QString::number(data.pdMin));
    ui->te_ps_max->setPlainText(QString::number(data.psMax));
    ui->te_ps_min->setPlainText(QString::number(data.psMin));
    ui->te_t_sample->setPlainText(QString::number(data.tSample));
}

Preferences::~Preferences()
{
    qDebug()<< "Ventana Preferencias destruida";
    delete ui;
}

void Preferences::on_pushButton_cerrar_clicked()
{
    this->close();
}


void Preferences::on_pushButton_aplicar_clicked()
{
    //Reconfigurar los plots y mandar datos al main para reconfigurar LPC
    mConfig reconfig = {
        ui->te_ps_max->toPlainText().toFloat(),
        ui->te_ps_min->toPlainText().toFloat(),
        ui->te_pd_max->toPlainText().toFloat(),
        ui->te_pd_min->toPlainText().toFloat(),
        ui->te_t_sample->toPlainText().toInt()
    };

    emit sendPreferencesToMain(reconfig);
}

