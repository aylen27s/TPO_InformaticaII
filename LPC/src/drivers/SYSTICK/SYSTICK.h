/*
 * SYSTICK.h
 *
 *  Created on: 17 jun. 2025
 *      Author: aylen
 */

#ifndef SYSTICK_SYSTICK_H_
#define SYSTICK_SYSTICK_H_

//#define MAX_TICKS		0xFFFFFF
//#define FREQ_PRINCIPAL	(12000000UL)

extern "C" {
void SysTick_Handler(void);
}

class SYSTICK {
public:
	SYSTICK(uint32_t ticks);
};


#endif /* SYSTICK_SYSTICK_H_ */
