/*
 * GPIOFO.cpp
 *
 *  Created on: 20 ago. 2025
 *  Author: aylen
 */



#include "MyIncludes.h"

GpioFO::GpioFO(uint32_t port, uint32_t pin, uint32_t estadoInicial, uint32_t tiempoActivacion):Gpio(port,pin,OUTPUT,estadoInicial) {
	m_contador = 0;
	m_tiempoActivacion = tiempoActivacion;
	m_estado = estadoInicial;
}


void GpioFO::Callback(){
	if(m_contador < m_tiempoActivacion){
		m_contador++;
	}else{
		Gpio::Set(m_estado);
		m_contador = 0;
	}
}

void GpioFO::Set(uint32_t estado){
	m_estado = estado;
	Gpio::Set(m_estado);
}


GpioFO::~GpioFO() {
	// TODO Auto-generated destructor stub
}

