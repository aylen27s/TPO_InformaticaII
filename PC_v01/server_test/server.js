
/*
Servidor para recibir datos vía HTTP y guardarlos en SQLite
*/

// Importar librerías
const express = require("express");
const sqlite3 = require("sqlite3").verbose();
const bodyParser = require("body-parser");

// Configuración
const app = express();
const PORT = 3000;
const DB_PATH = "/home/guido/Documentos/TPO_InformaticaII/file_past.db";
const TABLE = "data";

// Middleware para interpretar JSON
app.use(bodyParser.json());

// Conexión a la base
const myDb = new sqlite3.Database(DB_PATH, (error) => {
  if (error) {
    console.error("Error de conexión a la DB:", error);
  } else {
    console.log("Conexión exitosa con la DB.");
  }
});

// Crear tabla si no existe
const sqlCreate = `
CREATE TABLE IF NOT EXISTS ${TABLE}(
  ps REAL NOT NULL,
  pd REAL NOT NULL,
  fecha TEXT NOT NULL
)`;
myDb.run(sqlCreate, (error) => {
  if (error) {
    console.error("Error al crear/verificar tabla:", error);
  } else {
    console.log("Tabla verificada.");
  }
});

// Endpoint para recibir datos
app.post("/api/data", (req, res) => {
  const { ps, pd, fecha } = req.body;

  if (ps === undefined || pd === undefined || fecha === undefined) {
    return res.status(400).json({ error: "Faltan campos en el JSON." });
  }

  const sqlInsert = `INSERT INTO ${TABLE} (ps, pd, fecha) VALUES (?,?,?)`;
  myDb.run(sqlInsert, [ps, pd, fecha], (error) => {
    if (error) {
      console.error("Error en inserción:", error);
      return res.status(500).json({ error: "Error en la DB" });
    }
    console.log(`Inserción ok: ps=${ps}, pd=${pd}, fecha=${fecha}`);
    res.json({ status: "ok", ps, pd, fecha });
  });
});

// Endpoint para consultar los últimos N datos
app.get("/api/data/:n", (req, res) => {
  const n = parseInt(req.params.n) || 10;
  const sqlSelect = `SELECT * FROM ${TABLE} ORDER BY ROWID DESC LIMIT ?`;
  myDb.all(sqlSelect, [n], (err, rows) => {
    if (err) {
      return res.status(500).json({ error: "Error al leer la DB" });
    }
    res.json(rows);
  });
});

// Iniciar servidor
app.listen(PORT, () => {
  console.log(`Servidor escuchando en http://localhost:${PORT}`);
});
