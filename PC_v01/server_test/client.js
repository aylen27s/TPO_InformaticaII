/*
Cliente que manda datos de pulso cardíaco simulado cada 1 segundo
*/
const axios = require("axios");

const SERVER_URL = "http://localhost:3000/api/data";
const SEGS_TO_SEND = 1000 * 3;
// contador de tiempo en segundos
let t = 0;

// Función que simula un pulso cardíaco más realista
function simulateHeartbeat(time) {
  // Frecuencia cardíaca base (latidos por minuto)
  const heartRate = 72; // BPM
  const beatDuration = 60 / heartRate; // duración de un latido en segundos
  
  // Posición dentro del ciclo del latido actual
  const phase = (time % beatDuration) / beatDuration;
  
  // Simulación de la forma de onda del pulso cardíaco
  let pulseValue;
  
  if (phase < 0.1) {
    // Pico sistólico agudo
    pulseValue = Math.exp(-Math.pow((phase - 0.05) * 100, 2)) * 25;
  } else if (phase < 0.3) {
    // Caída rápida después del pico
    pulseValue = 8 + Math.sin((phase - 0.1) * Math.PI * 5) * 2;
  } else if (phase < 0.7) {
    // Onda dicrótica (segundo pico más pequeño)
    pulseValue = 6 + Math.exp(-Math.pow((phase - 0.5) * 10, 2)) * 8;
  } else {
    // Retorno a la línea base
    pulseValue = 5 + (phase - 0.7) * 5;
  }
  
  return Math.max(2, pulseValue); // Valor mínimo de 2
}

function sendData() {
  // Simular variaciones naturales en la presión arterial
  const basePs = 120; // Presión sistólica base
  const basePd = 80;  // Presión diastólica base
  
  // Obtener el valor del pulso simulado
  const pulse = simulateHeartbeat(t);
  
  // Calcular presiones basadas en el pulso con variaciones naturales
  const ps = basePs + 
             Math.sin(2 * Math.PI * t / 45) * 8 + // Variación lenta
             (pulse - 10) * 0.8 + // Componente del pulso
             Math.random() * 3;   // Ruido aleatorio pequeño
  
  const pd = basePd + 
             Math.sin(2 * Math.PI * t / 45 + Math.PI/6) * 5 + // Variación lenta
             (pulse - 10) * 0.4 + // Componente del pulso
             Math.random() * 2;   // Ruido aleatorio pequeño
  
 // const fecha = new Date().toISOString().replace("T", " ").split(".")[0];
	const fechaAjustada = new Date(new Date().getTime() - 3 * 60 * 60 * 1000);
	const fecha = fechaAjustada.toISOString().replace("T", " ").split(".")[0];

  axios.post(SERVER_URL, { ps: Math.round(ps), pd: Math.round(pd), fecha })
    .then(res => {
      console.log("Dato enviado:", res.data);
    })
    .catch(err => {
      console.error("Error al enviar:", err.message);
    });

  t++; // incrementar el tiempo
}

// Enviar cada 1 segundo
setInterval(sendData, SEGS_TO_SEND);
console.log("Simulación de pulso cardíaco iniciado...");
