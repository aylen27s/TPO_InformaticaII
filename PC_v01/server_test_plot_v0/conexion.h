#ifndef CONEXION_H
#define CONEXION_H

#include <QDialog>

namespace Ui {
class Conexion;
}

class Conexion : public QDialog
{
    Q_OBJECT

public:
    explicit Conexion(QWidget *parent = nullptr);
    ~Conexion();

signals:
    void sendConexionToMain(QString ssid, QString psw);

private slots:
    void on_pb_conectar_clicked();
    void on_pb_cerrar_clicked();

private:
    Ui::Conexion *ui;
};

#endif // CONEXION_H
