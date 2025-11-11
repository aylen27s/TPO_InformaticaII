/*
 * SYSTICK.cpp
 *
 *  Created on: 14 ago. 2025
 *      Author: guido
 */

#include "Defines.h"

SYSTICK Systick(freq); // El systick se configurará para interrumpir 'frec' veces por segundo, es decir cada 1/frec [segundos]. Lo recomendado freq = 1 [kHz] = 1000 [Hz]
volatile static uint32_t get_milisegundos = 0;


SYSTICK::SYSTICK(uint32_t frec){
	uint32_t ticks = FREQ_PRINCIPAL / frec; 	// *1*
	if(ticks >= MAX_TICKS){				 		// Se asegura de no superar el límite de 2^24.
		return;
	}

	SysTick->CURR 	= 0; 						// Registro SYST_CVR ≡ CURR:    Borra el contador para iniciar de cero.
	SysTick->RELOAD = ticks - 1;				// Registro SYST_RVR ≡ RELOAD:  Se pone "ticks - 1" porque el conteo incluye al cero. Ej.: ticks = 1000 entonces va de 999 a 0. IMPORTANTE: el conteo es decreciente.
	SysTick->CTRL 	= 0x07;						// Registro SYST_CSR ≡ CTRL: 	*2*
}

// Función de interrupción						
void SysTick_Handler(void){ 					// *3*
	CALLBACK::SysTickCall();
	get_milisegundos++;
}

uint32_t SYSTICK::Get_milis(void)
{
	return get_milisegundos;
}
// *** 1: ******************************************************************************************************
//
//	El Systick contará 'ticks' ciclos de CPU antes de ejecutar la interrupción. Cada tick es un pulso de la señal
//  de reloj que alimenta al SysTick. Esto tarda exactamente 1/frec [segundos], es decir el tiempo deseado.
//
//	En este caso FREQ_PRINCIPAL = 12000000 [Hz] y frec = 1000 [Hz] -> ticks = 12000. Entonces, el Systick contará
//  12000 ciclos del CPU antes de ejecutar la interrupción. Esto ocurrirá exactamente cada 1/freq = 1/1000 [s] = 1 [ms].
//
// *************************************************************************************************************


// *** 2: ******************************************************************************************************
//
//	SysTick->CTRL =
// 	  	  	  	  	0x01;  → Habilita el contador (SysTick).
//
//	  	  	  	  	0x02;  → Habilita la interrupción al llegar a cero.
//
//      	  	  	0x04;  → Selecciona el clock del core como fuente (12 MHz en este caso).
//
//      	  	  	0x07;  → Todos los bits anteriores activados (equivale en binario a 0111).
//
//	Dicho de otra forma: SysTick->CTRL = (1 << 0) | (1 << 1) | (1 << 2);
// 	Resumiendo, el registro de control CTRL (SYST_CSR) habilita las funciones esenciales del SysTick.
//
// *************************************************************************************************************


// *** 3: ******************************************************************************************************
// El microcontrolador tiene un índice en una posición de memoria fija. Este íncice es la TABLA DE VECTORES DE
// INTERRUPCIÓN. Cada entrada del íncide corresponde a una fuente de interrupción distinta (SYSTICK, GPIO, UART, etc.).
//
// 1. Contenido del índice: Cada entrada corresponde a la dirección de memoria de la función que se debe ejecutar
// cuando ocurre la interrupción en particular.
//
// 2. Cuando el temporizador SysTick llega a cero y genera la interrupción, el procesador detiene lo que está haciendo,
// guarda la posición de memoria en la que se encuentra y va a atender la interrupción. En el momento que la atiende, lo
// que hace es consultar la entrada en la tabla de vectores mencionada, y salta a la dirección de memoria que ahí
// se encuentra. Esto es posible gracias a lo que hicimos en el comentario *2* al habilitar los bits 0, 1 y 2 en el registro CTRL (SYST_CSR).
//
// 3. El procesador empieza a ejecutar el código que reside en dicha dirección de memoria. En este caso, el contenido de la
// función SysTick_Handler.
//
// *************************************************************************************************************

