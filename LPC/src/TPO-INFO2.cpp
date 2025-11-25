/*
 * TPO INFORMATICA 2 | UTN FRBA
 * Curso	R2052
 * Año		2025
 */

#include "MyIncludes.h"

/*	Definiciones globales	*/
#define TIME_TOGGLE_ALERT	1*SEG
#define TIME_TO_SEND		2*SEG
#define PIN_MY_PULSADOR		0,26
//#define PIN_LED_ON 			0,27
#define PIN_LED_ON			PIN_LED_VERDE
#define PIN_LED_ALARMA		0,29
#define PIN_BUZZER			0,28	//0,30 no conmutaba? ni idea por qué, asi que moví todo un pin para arriba.
#define SIZE_TRAMA_HANDLER	6		// >,AAAC C es checksum

typedef enum {PROCESSING_SAMPLES,SEND_SAMPLE_PROCESSED,CONFIGURATION,ALERT} mEstados;
typedef enum {Active=ON, Inactive=OFF} mEstadosAlarma;

/* Variables Globales */
volatile uint32_t g_milisegundos = 0;

mEstadosAlarma EstadoAlarma = Inactive;
mEstados EstadoMdE = PROCESSING_SAMPLES;


/* Prototipos de funciones */
void App_SysTick_Callback(void);
void MdE(void);
void MdEAlarma(void);
void handleAlarma(void);

void handleTest(void);






/* Objetos globales */
Gpio LedOn(PIN_LED_ON, OUTPUT, OFF_LED);
Gpio LedAlarma(PIN_LED_ALARMA, OUTPUT, OFF_LED);
Gpio Buzzer(PIN_BUZZER, OUTPUT, OFF);
GpioFI PulsadorAlarma(PIN_PULSADOR);
Gpio LED_ROJO(PIN_LED_ROJO,1, OFF_LED); //1= output, off_led = 1, on_led = 0;


/*Hay que instanciar un objeto UART para que no rompa la implementación. Mover esto a la incialización de hardware*/
UART0	Uart0(9600);

TIMER Systick_Callback_Timer(1, 1, App_SysTick_Callback); //App_SysTick_Callback se ejecuta cada 1ms
TIMER T_Alarma;
TIMER T_Test;

MUESTRAS Muestras;



/* -------------- MAIN APP -------------- */
int main(void) {
	IIC_Inicializacion();
	LedOn.Set(ON_LED);
	LedAlarma.Set(OFF_LED);
//	T_Test.Start(TIME_TO_SEND, TIME_TO_SEND, handleTest);

    while(1) {
    	MAX30102();
    	MdE();
    	MdEAlarma();
    }
    return 0 ;
}

/*IMPLEMENTACION TEST*/
void handleTest(void){
	int32_t sample_rcv = MAX30102_GetLectura();
	char buffHandler[6];
	uint8_t checksum = 0;

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
	LedAlarma.TogglePin();
}


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


/* -------------- Implementaciones de Máquinas de Estados --------------*/
void MdE(){
	switch(EstadoMdE){
		case PROCESSING_SAMPLES:{
			/* El objetivo es promediar N muestras antes de enviarlas a la APP de QT*/
//				Uart0.Send((uint8_t*)"En P_S", 7);
				int32_t	sample_rcv = MAX30102_GetLectura(); 		//	Guardo temporalmente la lectura del sensor

				if(sample_rcv > 0){							//	Valido que la lectura no sea -1
					Muestras.PushBack(sample_rcv);			//	Pusheo la muestra  al buffer interno del objeto Muestras
					MAX30102_CleanBuffer();					//	Guardé dato entonces limpio buffer
//					Uart0.Send((uint8_t*)"s>0", 4);
				}
//				else{
//					Uart0.Send((uint8_t*)"s<0", 4);
//				}
				uint32_t tam = Muestras.Size();
				if( tam == SIZE_BUFF ){
					Muestras.Process();						//	Metodo de la clase para sacar el promedio y limpiar el buffer para las nuevas muestras.
															//	Tambien monitorea que las muestras estén en el rango normal.
					EstadoMdE = SEND_SAMPLE_PROCESSED;		//	Cambio de estado para mandar el dato
//					Uart0.Send((uint8_t*)"ssss", 4);
				}

				//Siempre hay que mandar muestras pero tambien hay que mirar si se habilitó la alarma para apagarla cuando es debido.
				if(EstadoAlarma == Active && PulsadorAlarma.Get() == PULSED){	//Si la alarma se encuentra activa y se apretó el pulsador, hay que apagarla.
					T_Alarma.Stop();
					EstadoAlarma = Inactive;
				}
			}

			break;

			case SEND_SAMPLE_PROCESSED:{
				int32_t sample_rcv = Muestras.GetSampleProcessed();	//Retorna la muestra promediada en un lapso T de tiempo configurado.
				uint8_t checksum = 0;
				char buffHandler[SIZE_TRAMA_SAMPLE]; // >ABCD --> ABC: datos de medicion, si solo son dos dígitos, A=0. C: byte de checksum.
				if( sample_rcv > 0 ){
					/* Carga de la muestra byte a byte junto con el checksum para la UART*/
					buffHandler[0]= '+';								checksum += buffHandler[0];
					buffHandler[1]= ',';								checksum += buffHandler[1];
					buffHandler[2] ='0' + (sample_rcv / 100 );			checksum += buffHandler[2];
					buffHandler[3] ='0' + ((sample_rcv % 100 ) / 10);	checksum += buffHandler[3];
					buffHandler[4] ='0' + (sample_rcv % 10); 			checksum += buffHandler[4];
					buffHandler[5] =checksum;
					Uart0.Send((uint8_t*)buffHandler, SIZE_TRAMA_SAMPLE);
				}else {
					buffHandler[0]='_';		checksum += buffHandler[0];
					buffHandler[1]=',';		checksum += buffHandler[1];
					buffHandler[2] = '9';	checksum += buffHandler[2];
					buffHandler[3] = '9';	checksum += buffHandler[3];
					buffHandler[4] = '9';	checksum += buffHandler[4];
					buffHandler[5] =checksum;
					Uart0.Send((uint8_t*)buffHandler, SIZE_TRAMA_SAMPLE);
				}
				if(	Muestras.StatusLimit()){		// Se ha sobrepasado el corte inferior o superior de presion normal, prender alarma.
					EstadoMdE = ALERT;
				} else {
					EstadoMdE = PROCESSING_SAMPLES; // Si las muestras son normales, vuelvo al procesamieto
				}
			}

			break;

		case ALERT:
			/*Disparo la maquina de estados para la alarma, y vuelvo a procesamiento de muestras*/
			T_Alarma.Start(TIME_TOGGLE_ALERT,TIME_TOGGLE_ALERT, handleAlarma);
			EstadoAlarma = Active;
			EstadoMdE = PROCESSING_SAMPLES;
			break;

		case CONFIGURATION:
			EstadoMdE = PROCESSING_SAMPLES;
			break;
		default:
			EstadoMdE = PROCESSING_SAMPLES;
			break;
	}

}

void MdEAlarma(void){
	switch(EstadoAlarma){
		case Active:
			LedAlarma.Set(ON);
			Buzzer.Set(ON); //ON=1
			break;
		case Inactive:
			LedAlarma.Set(OFF);
			Buzzer.Set(OFF); //OFF=0
			break;
		default:
			EstadoAlarma = Inactive;
			break;
	}
}

/*
 * Cada vez que venza el timer, permuta el estado de MdEAlarma para encender o apagar el led y el buzzer.
 * Estará en esta situación hasta que el usuario presione el pulsador.
 *
 */
void handleAlarma(){
	static int toggleFlag = 0;
	if(toggleFlag == 0){
		EstadoAlarma = Active;
		toggleFlag = 1;
	} else {
		EstadoAlarma = Inactive;
		toggleFlag = 0;
	}
}
