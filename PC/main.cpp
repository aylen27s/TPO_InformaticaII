#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    /* Apertura de la DB. Si algo falla, no inicializar la pantalla ppal */

    //2. Establezco la DB con la que voy a interacturar
    MY_DB.setDatabaseName(DB_PATH);

    if (!MY_DB.open()) {
        qDebug() << "Error al abrir la base de datos:" << MY_DB.lastError().text();
        return 1;
    } else {
        qDebug() << "Conexión ok.";
    }




    w.show();
    return a.exec();

}
