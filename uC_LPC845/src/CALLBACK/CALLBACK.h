/*
 * CALLBACK.h
 *
 *  Created on: 20 ago. 2025
 *      Author: guido
 */

#ifndef CALLBACK_CALLBACK_H_
#define CALLBACK_CALLBACK_H_

class CALLBACK{						// *1*
public:
	CALLBACK();						// *2*
	virtual void Callback(void);	// *3*
	static void SysTickCall(void);	// *4*
};

#endif /* CALLBACK_CALLBACK_H_ */



// *** 1: ******************************************************************************************************
//
//	En C, un callback es un puntero a función que se pasa como argumento a otra función.
//	En C++, el callback está "embebido" en el objeto mediante herencia y polimorfismo.
//
// *************************************************************************************************************

// *** 2: ******************************************************************************************************
//
// Constructor por defecto de la clase. Se llamará cada vez que se cree un objeto de tipo CALLBACK o de una clase
// que herede de ella, como por ejemplo, la clase TIMER
//
// *************************************************************************************************************

// *** 3: ******************************************************************************************************
//
// Se declara una función virtual. La palabra "virtual" indica que las clases que hereden de CALLBACK pueden
// sobreescribir este método con su propia implementación.
// Cuando se llame a Callback() a través de un puntero a CALLBACK, ejecutará la versión correcta del método
// (la del TIMER, por ejemplo) y no la de la clase base. Esto es la base del polimorfismo en C++.
// Cada clase derivada (como un temporizador, un gestor de ADC, etc.) definirá que hacer cuando ocurra el "tick"
// del sistema, es decir lo que vimos cuando implementamos el SysTick.
//
// *************************************************************************************************************

// *** 4: ******************************************************************************************************
//
// Se declara una función estática. La palabra 'static' indica que este método pertenece a la clase CALLBACK en sí,
// no a un objeto individual.
// Se puede llamar sin necesidad de crear un objeto, usando la sintaxis CALLBACK::SysTickCall();.
// Su propósito es actuar como el "invocador" central que llamará a todos los callbacks registrados.
//
// *************************************************************************************************************

// *************************************************************************************************************
//
// Es decir, el evento (interrupción periódica "tick"), y el callback (el método Callback()) de cada objeto que se
// ejecuta en respuesta a ese tick.
// El callback sirve para desacoplar quién genera el evento (el SysTick) de qué se hace cuando pasa (los timers u
// otros módulos que se registren).
//
// *************************************************************************************************************

