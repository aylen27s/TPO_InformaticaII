/*
 * GPIO.cpp
 *
 *  Created on: 17 jun. 2025
 *      Author: aylen
 */

#include "MyIncludes.h"

Gpio::Gpio(uint32_t port, uint32_t pin, uint32_t mode, uint32_t state){
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);								// 7 = SWM
	SYSCON->SYSAHBCLKCTRL0 |= (1 << 6) | (1 << 20) | (1 << 18);		// 6 = GPIO0	20 = GPIO1	18 = IOCON

	m_port = port;
	m_pin = pin;
	m_currState = state;

	SetMode(mode);

	if(m_mode == OUTPUT)
		Set(m_currState);
}

//Encendido o apagado
void Gpio::Set(uint32_t state){

	if(state==ON && m_mode==OUTPUT)
		GPIO->SET[m_port] |= (1 << m_pin);
	else
		GPIO->CLR[m_port] |= (1 << m_pin);
}


//GPIO en modo entrada o salida
void Gpio::SetMode(uint32_t mode){
	m_mode = mode;

	if(m_mode == OUTPUT)
		GPIO->DIR[m_port] |= (1 << m_pin);
	else if (m_mode == INPUT)
		GPIO->DIR[m_port] &= ~(1 << m_pin);
}

void Gpio::TogglePin(void){
	if(m_mode == OUTPUT){
		Set(!m_currState);
		m_currState = !m_currState;

	}
}

//Retornar el valor de la GPIO
uint32_t Gpio::Get(void){

	if( GPIO->PIN[m_port] & (1<<m_pin) )
	        return 1;
	else
	        return 0;
}


Gpio::~Gpio(){

}
