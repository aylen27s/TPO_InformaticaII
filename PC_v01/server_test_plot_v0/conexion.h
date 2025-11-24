#ifndef CONEXION_H
#define CONEXION_H

#include <QDialog>

//Para enviar configuracion wifi por puerto serie a ESP
#include <QTimer>
#include <chrono>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QMessageBox>
#include <QStatusBar>

//Definiciones para configuracion de puerto serie
#define     PORTNAME  "ttyUSB0"


namespace Ui {
class Conexion;
}

class Conexion : public QDialog
{
    Q_OBJECT

public:
    explicit Conexion(QWidget *parent = nullptr);
    ~Conexion();
    void openSerialPort();
    void closeSerialPort();
    void readSerialPort();
    void writeSerialPort(const QByteArray &data);
    void handleBytesWritten(qint64 bytes);
    void handleWriteTimeout();

signals:
    void sendConexionToMain(QString ssid, QString psw);

private slots:
    void on_pb_conectar_clicked();
    void on_pb_cerrar_clicked();

private:
    Ui::Conexion *ui;
    QSerialPort * m_serialPort = nullptr;
    qint64 m_bytesToWrite = 0;  //Contador. Se incrementa c/ vez que un byte se escribe por el puerto serie.
    QTimer *m_timer = nullptr;  //
};

#endif // CONEXION_H
