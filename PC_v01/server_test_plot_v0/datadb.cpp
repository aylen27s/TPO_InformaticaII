#include "datadb.h"

QSqlDatabase MY_DB = QSqlDatabase::addDatabase("QSQLITE", "miConexion");
QString DB_PATH = "/home/aylen/Escritorio/Info2/TPO_InformaticaII/file_past.db";
QString DB_TABLE = "data";
