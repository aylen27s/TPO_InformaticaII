#include "conexion.h"
#include "ui_conexion.h"



static constexpr std::chrono::seconds secsForTimeout = std::chrono::seconds{5}; //Tiempo de espera por escritura de byte.

Conexion::Conexion(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Conexion)
    , m_serialPort( new QSerialPort(this)) // Cuando se abre esta ventana, se crea un objeto para manipular el puerto serial.
    , m_timer(new QTimer(this))
{
    ui->setupUi(this);


    openSerialPort();

    /*Conecto señal para poder mandar SSID y PSW al ESP.
     * Cada vez que se llama al metodo write para escribir en el puerto serie, se emite bytesWritten y se ejecuta el handler para resetear el contador*/
    connect(m_serialPort, &QSerialPort::bytesWritten, this, &Conexion::handleBytesWritten);

    //Conecto señal para poder leer el status de confirmación desde el ESP
    connect(m_serialPort, &QSerialPort::readyRead   , this, &Conexion::readSerialPort);

    /*
     * Si pasó demasiado tiempo intentando escribir sin exito, ejecuta handleWriteTimeout
     * Se ejecuta solo una vez. Para reinciarlo hay que darle Start en cada ocasión
    */
    connect(m_timer, &QTimer::timeout, this, &Conexion::handleWriteTimeout);
    m_timer->setSingleShot(true);

    // const auto serialPortInfos = QSerialPortInfo::availablePorts();
    // for (const QSerialPortInfo &portInfo : serialPortInfos) {
    //     qDebug() << "\n"
    //              << "Port:" << portInfo.portName() << "\n"
    //              << "Location:" << portInfo.systemLocation() << "\n"
    //              << "Description:" << portInfo.description() << "\n"
    //              << "Manufacturer:" << portInfo.manufacturer() << "\n"
    //              << "Serial number:" << portInfo.serialNumber() << "\n"
    //              << "Vendor Identifier:"
    //              << (portInfo.hasVendorIdentifier()
    //                      ? QByteArray::number(portInfo.vendorIdentifier(), 16)
    //                      : QByteArray()) << "\n"
    //              << "Product Identifier:"
    //              << (portInfo.hasProductIdentifier()
    //                      ? QByteArray::number(portInfo.productIdentifier(), 16)
    //                      : QByteArray());
    // }

}

Conexion::~Conexion()
{
    qDebug()<< "Ventana Conexion destruida";
    closeSerialPort();  //El metodo ya evalua internamente si se pudo configurar el puerto serie o no.
    delete ui;

}


void Conexion::on_pb_conectar_clicked()
{
    //Logica anterior para mandar los datos desde la ventana emergente hacia el main.
    // qDebug()<<"click botoncito conectar, mandando data al main"<< ui->textEdit_psw->toPlainText() << ui->textEdit_ssid->toPlainText();
    // emit sendConexionToMain(ui->textEdit_ssid->toPlainText(), ui->textEdit_psw->toPlainText());

    //Logica para mandar los datos de conexion directamente desde la ventana emergente al ESP via puerto serie.
    if(m_serialPort->isOpen()){
        QByteArray config = QString("?%1?%2")
            .arg(ui->textEdit_ssid->toPlainText())
            .arg(ui->textEdit_psw->toPlainText())
            .toUtf8();

        writeSerialPort(config);
    }

}


void Conexion::on_pb_cerrar_clicked()
{
    closeSerialPort();
    this->close();
}

void Conexion::openSerialPort()
{
    m_serialPort->setPortName   (PORTNAME                  );
    m_serialPort->setBaudRate   (QSerialPort::Baud115200   );
    m_serialPort->setDataBits   (QSerialPort::Data8        );
    m_serialPort->setParity     (QSerialPort::NoParity     );
    m_serialPort->setStopBits   (QSerialPort::OneStop      );
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if ( m_serialPort->open(QIODevice::ReadWrite) ) {
        qDebug()<<"Puerto serie configurado correctamente.";
    } else {
        //Notificar error si no se abre ok
        QMessageBox::critical(this, tr("Error"), m_serialPort->errorString());
    }
}

void Conexion::handleBytesWritten(qint64 bytes)
{
    /*
    Decrementa el contador añadiendo la nueva cant. de bytes a enviar
    cuando m_bytesToWrite es 0 ya se han enviado todos los bytes que se cargaron en el buffer.
    */

    m_bytesToWrite -= bytes;
    if (m_bytesToWrite == 0)
        m_timer->stop();
}

void Conexion::readSerialPort(){

    const QByteArray response = m_serialPort->readAll();
    qDebug()<<QString(response);
    // if(QString(response) == "?#$&")
    //     qDebug()<<"Configuración de red exitosa.";

}

void Conexion::writeSerialPort(const QByteArray &data){
    /*
     * El metodo write() retorna la cantidad de bytes que realmente se escribieron en el puerto serie
     * Al mandar todos los bytes al puerto serie cargamos el contador m_bytesToWrite
     * y esperamos que estos no tarden mas de 5 segundos en enviarse.
     * Si el envió es exitoso, m_timer se detendrá en handleBytesWritten
    */
    const qint64 written = m_serialPort->write(data);
    if (written == data.size()) {
        qDebug()<< "Escribiendo en puerto serie ...";
        m_bytesToWrite += written;
        m_timer->start(secsForTimeout);
    } else {
        qDebug()<<QString(
            "Error al escribir en %1.\n"
            "Error: %2")
            .arg(m_serialPort->portName(),m_serialPort->errorString());
    }
}

void Conexion::closeSerialPort(){
    qDebug()<< "Cerrando puerto serie.";
    if (m_serialPort->isOpen())
        m_serialPort->close();
}

void Conexion::handleWriteTimeout(){
    qDebug()<<"ERROR: Se agotó el tiempo de escritura para el byte por puerto serie.";
}
