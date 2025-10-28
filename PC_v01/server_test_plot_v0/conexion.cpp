#include "conexion.h"
#include "ui_conexion.h"

Conexion::Conexion(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Conexion)
{
    ui->setupUi(this);
}

Conexion::~Conexion()
{
    qDebug()<< "Ventana emergente destruida";
    delete ui;

}


void Conexion::on_pb_conectar_clicked()
{
    qDebug()<<"click botoncito conectar, mandando data al main"<< ui->textEdit_psw->toPlainText() << ui->textEdit_ssid->toPlainText();
    emit sendDataToMain(ui->textEdit_psw->toPlainText());
}


void Conexion::on_pb_cerrar_clicked()
{
    this->close();
}

