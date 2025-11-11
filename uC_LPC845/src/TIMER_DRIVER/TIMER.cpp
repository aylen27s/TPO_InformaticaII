/*
 * TIMER.cpp
 *
 *  Created on: 20 ago. 2025
 *      Author: guido
 */

#include "Defines.h"

void TIMER::Start(uint32_t time_Start, uint32_t time_Reload, void(*func)(void))
{
	this->time = time_Start;
	this->time_Reload = time_Reload;
	this->func = func;
}

TIMER::TIMER(uint32_t time_Start, uint32_t time_Reload, void(*func)(void))
{
	Start(time_Start, time_Reload, func);
}

void TIMER::Stop(void)
{
	this->time = 0;
}

void TIMER::Discount(void)
{
	if(this->time > 0){
		this->time--;

		if(0 == this->time){
			this->time = this->time_Reload;
			this->func();						// Se ejecuta la función a la que apunta el puntero func.
		}
	}
}

void TIMER::Callback(void){
	Discount();
}
