/*
 * TPO INFORMATICA 2 | UTN FRBA
 * Curso	R2052
 * Año		2025
 */

#include "MyIncludes.h"

void SendToESP(void);

TIMER T1(2000,1000,SendToESP);
Gpio LED_VERDE(PIN_LED_VERDE, OUTPUT);

UART0	Uart0(9600); //Hay que instanciar un objeto para que no rompa la implementación. Mover esto a la incialización de hardware





int main(void) {
	LED_VERDE.Set(ON_LED);

    while(1) {

    }
    return 0 ;
}

void SendToESP(void){
	LED_VERDE.TogglePin();
	Uart0.Send( (uint8_t*)"Test", IS_STRING );
}
