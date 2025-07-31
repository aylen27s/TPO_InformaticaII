/*

Script para simular la inserción de datos en la DB en tiempo real y poder probar la función de plot de la aplicación QT.

*/

//Importar biblioteca para manejar bases SQLite
const sqlite3 = require('sqlite3').verbose();

//Constantes a utilizar
const ruta ='/el_path_en_donde_esta_la_db'
const db_name = 'nombre de la db aqui'
const DB_PATH = `${ruta}/${db_name}`;
const TABLE = 'data'

const min_ps = 11;
const max_ps = 15;
const min_pd = 13;
const max_pd = 10;
const decimales = 2;

//Conexión a la base.
const myDb = new sqlite3.Database(DB_PATH, (error)=>{
    if(error){ 
        console.log(`Error de conexion: ${error}`)
    }
    else { console.log("Conexión exitosa con la DB.") }
});

//Crear la tabla si no existe.
let sqlQuery = `CREATE TABLE IF NOT EXISTS ${TABLE}(
    ps REAL NOT NULL, 
    pd REAL NOT NULL,
    fecha TEXT NOT NULL
)`

myDb.run(sqlQuery, (error)=>{
    if(error){
        console.log(`Error query: ${error}`)
    } else { console.log("Tabla verificada.") }
})


//Funciones para generar los datos de forma aleatoria
function newValue(min,max) {
  const precision = Math.pow(10, decimales);
  const num = Math.floor(Math.random() * (max * precision - min * precision + 1) + min * precision) / precision;
  return num.toFixed(decimales);
}

//Funcion de inserción de datos
function insertData(){
    console.log("Preparando inserción de datos...")
    let ps_value = newValue(min_ps,max_ps);
    let pd_value = newValue(min_pd,max_pd);
    let fecha_value = new Date().toLocaleTimeString('es-AR', { hour12: false });
    
    sqlQuery = `INSERT INTO ${TABLE} (ps,pd,fecha) VALUES (?,?,?)`

    myDb.run(sqlQuery, [ps_value,pd_value,fecha_value], (error)=>{
        if(error){
            console.log("Error en inserción:", error);
        } else {
            console.log("Inserción ok.")
        }
    })
}

//Funcion para ver los datos
// function getData(){
//     sqlQuery = `SELECT * FROM ${TABLE}`

//     myDb.all(sqlQuery,(err, dataSelect)=>{
//         if(err){
//             console.log("Error en query select:", err);
//         } else {
//             console.log(dataSelect)
//         }
//     })
// }


//Escribir en la DB el nuevo conjunto de datos cada X segundos
let intervalId = setInterval(insertData, 5*1000)

//Pasados N segundos dejar de escribir y cerrar la conexión
setTimeout( ()=>{
    clearInterval(intervalId)
    myDb.close()
    console.log("Conexión cerrada.")
}, 1000*30)

