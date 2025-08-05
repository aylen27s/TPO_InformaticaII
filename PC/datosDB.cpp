#include "datosDB.h"

// Establezco el tipo de conexión e instancio global
QSqlDatabase MY_DB = QSqlDatabase::addDatabase("QSQLITE");

QString DB_TABLE = "data";

QString DB_PATH = "/home/aylen/Escritorio/test_data_base";
