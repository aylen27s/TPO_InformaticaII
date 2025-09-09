/*
 * SYSTICK.cpp
 *
 *  Created on: 17 jun. 2025
 *	Author: aylen
 */
#include "MyIncludes.h"


SYSTICK Systick(1000);

SYSTICK::SYSTICK(uint32_t frec){

	uint32_t ticks = FREQ_PPAL / frec;

	if(ticks >= MAX_TICKS)
		return;

	SysTick->CURR = 0;

	SysTick->RELOAD = ticks - 1;

	SysTick->CTRL = 0x07;
}

void SysTick_Handler(void){
	// 1ms o 1Khz

	CALLBACK::SysTickCall();

}
