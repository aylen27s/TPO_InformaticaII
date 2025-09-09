/*
 * GPIOFI.cpp
 *
 *  Created on: 24 jun. 2025
 *      Author: aylen
 */

#include "MyIncludes.h"

GpioFI::~GpioFI() {
	// TODO Auto-generated destructor stub
}

//en ppio se pueden llamar a los constructores de las clases padres, habria que ver si sirve para llamar a otros metodos
GpioFI::GpioFI(uint32_t port, uint32_t pin) : Gpio(port, pin, INPUT){ // al momento de construir un objeto de la calse GPIOF llamamos al constructor de la clase Gpio para configurar la entrada
	m_contador = 0;
	m_estAnterior = 0;
	m_estEstable = 0;
}

void GpioFI::Antirebote(void){
	uint32_t estActual = Gpio::Get(); //PIN está declarado globalmente, es la entrada a chequear

	if(estActual == m_estAnterior){
		m_contador++;
		if(m_contador == CANT_ESTADOS_VALIDOS){ //CANT_ESTADOS_VALIDOS suele ser 20, para que se evalue la permanencia del estado cada 20ms
			m_estEstable = estActual;
		}

	}else{
		m_contador=0;
	}
	m_estAnterior = estActual;
}

void GpioFI::Callback (void){
	Antirebote();//No ponemos el codigo de antirebote directamente acá para poder discernir rapidamente qué es lo que va a ejecutar el callback cuando lo llame el systick
}
