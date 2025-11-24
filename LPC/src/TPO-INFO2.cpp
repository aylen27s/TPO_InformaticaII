/*
 * TPO INFORMATICA 2 | UTN FRBA
 * Curso	R2052
 * Año		2025
 */

#include "MyIncludes.h"
//#include <stdio.h>
//#include <string.h>

/*	Definiciones globales	*/
//#define TIME_TOGGLE			2*SEG
#define TIME_TO_SEND		3*SEG
#define PIN_MY_PULSADOR		0,26
#define PIN_LED_ON 			0,27
#define PIN_LED_ALARMA		0,28
#define PIN_BUZZER			0,29	//0,30 no conmutaba? ni idea por qué, asi que moví todo un pin para arriba.
#define SIZE_TRAMA_HANDLER	6		// >,AAAC C es checksum
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



//enum mEstados {AWAIT,ALERT};
//enum mEstadosAlarma {Active, Inactive};
//
//mEstadosAlarma flagStateAlarma = Inactive;
//mEstados EstadoMdE = AWAIT;
//
//uint32_t EstadoAlarma = OFF;

//Gpio LED_ON(PIN_LED_ON, OUTPUT, OFF);
//Gpio LED_ALARMA(PIN_LED_VERDE, OUTPUT, OFF_LED);	//Despues cambiar al pin correspondiente.
////Gpio LED_ALARMA(PIN_LED_ALARMA, OUTPUT, ON);
//Gpio BUZZER(PIN_BUZZER, OUTPUT, OFF);
//GpioFI PULSADOR(PIN_PULSADOR);

//Gpio LED_VERDE( PIN_LED_VERDE, OUTPUT, OFF_LED);	//Led del systick para hacer pruebas


/*Hay que instanciar un objeto UART para que no rompa la implementación. Mover esto a la incialización de hardware
El driver para manejar el MAX30102 tiene implementada la UART1, hay que adaptar eso y cambiar por donde transmite.*/
UART0	Uart0(9600);

//TIMER T_Alarma;
TIMER T_Test;

Gpio LED_ROJO(PIN_LED_ROJO,1, OFF_LED); //1= output, off_led = 1, on_led = 0;

/* Prototipos de funciones */
void MdE(void);
void MdEAlarma(void);
void handleAlarma(void);

void handleTest(void);


/* -------------- MAIN APP -------------- */
int main(void) {
	IIC_Inicializacion();
//	LED_ROJO.Set(ON_LED);
	T_Test.Start(TIME_TO_SEND, TIME_TO_SEND, handleTest);

    while(1) {
    	MAX30102();



//    	MdE();
//    	MdEAlarma();
    }
    return 0 ;
}

/*IMPLEMENTACION TEST*/
void handleTest(void){
	int32_t sample_rcv = MAX30102_GetLectura();
	char buffHandler[6];
	uint8_t checksum = 0;
//	sprintf((char *)buffHandler, "_%u\n",sample_rcv);


	if(sample_rcv > 0 ){
		buffHandler[0]= '+';								checksum += buffHandler[0];
		buffHandler[1]= ',';								checksum += buffHandler[1];
		buffHandler[2] ='0' + (sample_rcv / 100 );			checksum += buffHandler[2];
		buffHandler[3] ='0' + ((sample_rcv % 100 ) / 10);	checksum += buffHandler[3];
		buffHandler[4] ='0' + (sample_rcv % 10); 			checksum += buffHandler[4];
		buffHandler[5] =checksum;
	} else {
		buffHandler[0]='_';		checksum += buffHandler[0];
		buffHandler[1]=',';		checksum += buffHandler[1];
		buffHandler[2] = '9';	checksum += buffHandler[2];
		buffHandler[3] = '9';	checksum += buffHandler[3];
		buffHandler[4] = '9';	checksum += buffHandler[4];
		buffHandler[5] =checksum;
	}

	Uart0.Send((uint8_t*)buffHandler, SIZE_TRAMA_HANDLER);
	MAX30102_CleanBuffer();
	LED_ROJO.TogglePin();
}



/* -------------- Implementaciones de funciones --------------*/
//void MdE(){
//	switch(EstadoMdE){
//		case PROCESSING_SAMPLES:
//			/* El objetivo es promediar N muestras antes de enviarlas a la APP de QT*/
//			int32_t = sample_rcv;
//
//			sample_rcv = MAX30102_GetLectura(); 		//	Guardo temporalmente la lectura del sensor
//
//			if(sample_rcv > 0){							//	Valido que la lectura no sea -1
//				Muestras.PushBack(sample_rcv);			//	Pusheo la muestra  al buffer interno del objeto Muestras
//			}
//
//			if( Muestras.Size() == CANTIDAD_MUESTRAS){
//				Muestras.Process();						//	Metodo de la clase para sacar el promedio y limpiar el buffer para las nuevas muestras.
//														//	Tambien monitorea que las muestras estén en el rango normal.
//
//				EstadoMdE = SEND_SAMPLE_PROCESSED;		//	Cambio de estado para mandar el dato
//			}
//
//			//Siempre hay que mandar muestras pero tambien hay que mirar si se habilitó la alarma para apagarla cuando es debido.
//			if(flagStateAlarma == Active && PulsadorAlarma.Get() == ON){	//Si la alarma se encuentra activa y se apretó el pulsador, hay que apagarla.
//				T_Alarma.Stop();
//				EstadoAlarma = OFF;
//				flagAlarma = Inactive;
//			}
//			break;
//
//			case SEND_SAMPLE_PROCESSED:
//				uint32_t sample = Muestras.GetSampleProcessed();	//Retorna la muestra promediada en un lapso T de tiempo configurado.
//
//				/* Carga de la muestra byte a byte junto con el checksum para la UART*/
//				uint8_t checksum = 0;
//				uint8_t buff[SIZE_TRAMA_SAMPLE] // >ABCD --> ABC: datos de medicion, si solo son dos dígitos, A=0. C: byte de checksum.
//
//				buff[0] = ">"
//				checksum+=buff[0];
//
//				buff[1] = sample / 100;
//				checksum+=buff[1];
//
//				buff[2] = (sample % 100) / 10;
//				checksum+=buff[2];
//
//				buff[3] = sample % 10;
//				checksum+=buff[3];
//
//				buff[4] = checksum;
//
//				Uart0.Send(buff, SIZE_TRAMA_SAMPLE);
//
//
//				if(	Muestras.isLimitExceeded){		// Se ha sobrepasado el corte inferior o superior de presion normal, prender alarma.
//					EstadoMdE = ALERT
//				} else {
//					EstadoMdE = PROCESSING_SAMPLES; // Si las muestras son normales, vuelvo al procesamieto
//				}
//
//
//			break;
//
//		case ALERT:
//			/*Disparo la maquina de estados para la alarma, y vuelvo a procesamiento de muestras*/
//			T_Alarma.Start(TIME_TOGGLE,TIME_TOGGLE, handleAlarma);
//			flagStateAlarma = Active;
//			EstadoMdE = PROCESSING_SAMPLES;
//			break;
//	// 	case AWAIT:						//Se espera al pulsador para encender la alarma. Esto lo controlaría el micro. Se deja externo para hacer pruebas.
//	// 		if( PULSADOR.Get() == PULSED ){
//	// 			T_Alarma.Start(TIME_TOGGLE,TIME_TOGGLE, handleAlarma);
//	// 			EstadoMdE = ALERT;
//
//	// 		}
//	// 		break;
//	// 	case ALERT:						//Si se pulsa el boton se desactiva la alarma.
//	// 		if( PULSADOR.Get() == PULSED ){
//	// 			T_Alarma.Stop();
//	// 			EstadoAlarma = OFF;
//	// 			EstadoMdE = AWAIT;
//	// 		}
//	// 		break;
//
//		default:
//			EstadoMdE = PROCESSING_SAMPLES;
//			break;
//	}
//
//}
//
//void MdEAlarma(void){
//	switch(EstadoAlarma){
//		case ON:
//			LED_ALARMA.Set(ON_LED);
//			BUZZER.Set(ON); //ON=1
//			break;
//		case OFF:
//			LED_ALARMA.Set(OFF_LED);
//			BUZZER.Set(OFF); //OFF = 0
//			break;
//		default:
//			EstadoAlarma = OFF;
//			break;
//	}
//}
//
///*
// * Cada vez que venza el timer, permuta el estado de MdEAlarma para encender o apagar el led y el buzzer.
// * Estará en esta situación hasta que el usuario presione el pulsador.
// *
// */
//void handleAlarma(){
//	static int toggleFlag = 0;
//	if(toggleFlag == 0){
//		EstadoAlarma = ON;
////		Uart0.Send((char*) "Ayuda", 6);
//		toggleFlag = 1;
//	} else {
//		EstadoAlarma = OFF;
//		toggleFlag = 0;
//	}
//}
