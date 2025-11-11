/*
 * SYSTICK.h
 *
 *  Created on: 14 ago. 2025
 *      Author: guido
 */

#ifndef SYSTICK_DRIVER_SYSTICK_H_
#define SYSTICK_DRIVER_SYSTICK_H_

#define MAX_TICKS		0xFFFFFF		// 2^24 [bits] = 16.777.215 [bits] -> número máximo de bits que puede cargar el SysTick en su registro RELOAD.
#define FREQ_PRINCIPAL	(12000000UL)	// 12   [MHz]  = 12.000.000 [Hz]   -> frecuencia del reloj principal de la CPU.
#define freq			1000			// 1	[kHz]  = 1000 		[Hz]   -> frecuencia de interrupción.

extern "C"{
	void SysTick_Handler(void);
}

class SYSTICK{
public:
	SYSTICK(uint32_t _freq);
	uint32_t Get_milis(void);

};

#endif /* SYSTICK_DRIVER_SYSTICK_H_ */




// *****************************************************************************************************
//
//  Se define una clase SYSTICK para encapsular la configuración/uso del temporizador SysTick.
//
//	El constructor recibe como parámetro la frecuencia de interrupción deseada (en Hz).
//	Internamente, se calcula el valor de ticks que se cargará en el registro RELOAD del SysTick.
//
//	El valor calculado de ticks no puede ser mayor a MAX_TICKS.
//
// *****************************************************************************************************

