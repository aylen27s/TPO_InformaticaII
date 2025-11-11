#include "Defines.h"
#include <string.h>
// Variables globales

//Gpio LED_ROJO	(PP_LED_ROJO, OUTPUT);
//Gpio LED_AZUL	(PP_LED_AZUL, OUTPUT);

//void toggle_led_rgb(void);
//void led(void);
//TIMER temporizador(2000,1000, led);

//GPIOF PULSADOR(PIN_PULSADOR);

Gpio LED_VERDE	(PP_LED_VERDE,OUTPUT);
UART0 Uart0(9600); 						// baudios velocidad de transmisión

volatile uint32_t g_milisegundos = 0;

/*Esta función será llamada por nuestro objeto TIMER por el Systick y la clase CALLBACK.*/
void App_SysTick_Callback(void)
{
	g_milisegundos++;
    // 1. Decrementa el contador de demora del I2C
    // Esta función es necesaria para que las pausas en la máquina de estados del MAX30102 funcionen.
    uint32_t demora = GetDemora_IIC();
    if (demora > 0)
    {
        SetDemora_IIC(demora - 1);
    }

    // 2. Establece el flag de lectura del sensor (cada 10ms)
        // Usamos un contador estático para no hacerlo en CADA tick de 1ms.
        // Lo hacemos cada 10ms, que coincide con el IIC_DELAY_ms del sensor.
        static uint8_t sensor_tick_ms = 0;
        sensor_tick_ms++;

        if (sensor_tick_ms >= 10) 		// Se cumple cada 10ms
        {
            sensor_tick_ms = 0; 		// Resetea el contador
            SetFlagLecturaMAX30102(1); 	// Levanta el flag para el sensor
        }
    }

// --- Creación del Objeto TIMER ---

// Creamos un temporizador global.
// time_Start = 1, time_Reload = 1.
// Esto hace que App_SysTick_Callback() se ejecute CADA 1ms.
TIMER Systick_Callback_Timer(1, 1, App_SysTick_Callback);

int main(void){

	IIC_Inicializacion();
	LED_VERDE.Set(ON);

	while(1){
		MAX30102();

    }
}































//void led(void){
//	static uint8_t stateLed = 0;
//	if(stateLed == 1){
//		stateLed = 0;
//	}else{
//		stateLed = 1;
//	}
//	LED_ROJO.Set(stateLed);
//
//
//	const char* mensaje = "hola";
//	Uart0.Send((uint8_t*)mensaje, strlen(mensaje));
//}



//void toggle_led_rojo(void)
//{
//	LED_VERDE.Set(OFF);
//	LED_AZUL.Set(OFF);
//	LED_ROJO.Set(!LED_ROJO.Get());
//}



//typedef enum {
//    ESTADO_ROJO,
//    ESTADO_VERDE,
//    ESTADO_AZUL,
//} estado_led_t;



//static estado_led_t estado = ESTADO_ROJO;


//
//void toggle_led_rgb(void)
//{
//    switch(estado) {
//    case ESTADO_ROJO:
//        LED_ROJO.Set(ON);
//        LED_VERDE.Set(OFF);
//        LED_AZUL.Set(OFF);
//        estado = ESTADO_VERDE;
//        break;
//
//    case ESTADO_VERDE:
//        LED_ROJO.Set(OFF);
//        LED_VERDE.Set(ON);
//        LED_AZUL.Set(OFF);
//        estado = ESTADO_AZUL;
//        break;
//
//    case ESTADO_AZUL:
//        LED_ROJO.Set(OFF);
//        LED_VERDE.Set(OFF);
//        LED_AZUL.Set(ON);
//        estado = ESTADO_ROJO;
//        break;
//    }
//}




