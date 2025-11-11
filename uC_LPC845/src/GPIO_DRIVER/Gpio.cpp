/*
 * Gpio.cpp
 *
 *  Created on: 14 ago. 2025
 *      Author: guido
 */
#include "Defines.h"

void Gpio::Set(uint32_t state){

	if(state){
		GPIO->SET[port] |= (1 << pin); 			   // Enciendo el pin.
	}else{
		GPIO->CLR[port] |= (1 << pin); 	           // Apago el pin.
	}
}

void Gpio::Dir(uint32_t _dir){

	dir = _dir;

	if(dir){
		GPIO->DIRSET[port] |= (1 << pin);	       // Configuro el pin como Salida.
	}else{
		GPIO->DIRCLR[port] |= (1 << pin);		   // Configuro el pin como Entrada.
	}
}

uint32_t Gpio::Get(uint32_t _port, uint32_t _pin){ // Lectura puntual del estado de cualquier pin.

	port = _port;
	pin  = _pin;

	if(GPIO->PIN[port] & (1 << pin)){
		return 1;
	}else{
		return 0;
	}
}

uint32_t Gpio::Get(void){						   // Lectura del pin asociado al objeto.

	if(GPIO->PIN[port] & (1 << pin)){
		return 1;
	}else{
		return 0;
	}
}

Gpio::Gpio(uint32_t _port, uint32_t _pin, uint32_t _dir){  // *1*
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 6) | (1 << 20) | (1 << 18);

	port = _port;
	pin = _pin;
	Dir(_dir);
}

// *** 1: ******************************************************************************************************************
// En un LPC845, SYSAHBCLKCTRL0 controla qué bloques de hardware que reciben clock.
// Si un módulo no recibe clock, está “apagado” y no se puede usar. Esto se hace para ahorrar energía.
// Los módulos son:
//
// 				- Bit 7 (SWM) 		→	Switch Matrix: módulo que te deja reasignar funciones a los pines
// 										(por ejemplo, mover un UART TX a otro pin).
//
// 				- Bit 6 (GPIO0) 	→ 	Puerto de pines GPIO0.
//
// 				- Bit 20 (GPIO1) 	→ 	Puerto de pines GPIO1.
//
// 				- Bit 18 (IOCON) 	→	Módulo que configura las características eléctricas
//  	 								de cada pin (pull-up, pull-down, función alternativa, etc.).
//
// Las asignaciones finales son simplemente para inicializar los atributos de la clase con los valores que
// recibe el constructor:
//
// 	- port y pin	→ se guardan para que el resto de métodos de la clase sepan qué pin manejar.
//
// 	- Dir(_dir)		→ llama al método anterior para configurar la dirección del pin (entrada o salida) en cuanto se crea el objeto.
// *************************************************************************************************************************
