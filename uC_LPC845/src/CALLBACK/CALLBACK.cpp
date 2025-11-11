/*
 * CALLBACK.cpp
 *
 *  Created on: 20 ago. 2025
 *      Author: guido
 */

#include "Defines.h"

std::vector<CALLBACK*> vCallBack;		// *1*

void CALLBACK::Callback(void)			// *2*
{

}

CALLBACK::CALLBACK(){					// *3*
	vCallBack.push_back(this);
}

void CALLBACK::SysTickCall(void){		// *4*
	for(CALLBACK* q : vCallBack){
		q->Callback();
	}
}


// *** 1: ******************************************************************************************************
//
// std::vector es un contenedor de la Biblioteca Estándar de C++ (STL) que funciona como un arreglo dinámico
// (puede crecer en tamaño).
// <CALLBACK*> indica que este vector no almacenará objetos CALLBACK directamente, sino punteros a objetos de
// tipo CALLBACK.
// vCallBack, este vector actuará como una lista central de registro para todos los objetos que necesiten ser notificados por el SysTick.
//
// *************************************************************************************************************

// *** 2: ******************************************************************************************************
//
//  Sirve como una implementación por defecto. Cualquier clase derivada, como TIMER, debe proporcionar su propia
//  versión de este método, que será la que realmente se ejecute. Por eso se define sin contenido.
//
// *************************************************************************************************************

// *** 3: ******************************************************************************************************
//
// Cada vez que un objeto de una clase derivada (como TIMER) se crea, se llama a este constructor de la clase base.
// 		- this: es un puntero que apunta al propio objeto que se está creando en ese momento.
// 		- vCallBack.push_back(): añade ese puntero al final del vector global vCallBack
// En resumen, al crear un TIMER, por ejemplo, su dirección de memoria se añade automáticamente a la lista vCallBack.
//
// *************************************************************************************************************

// *** 4: ******************************************************************************************************
//
// Esta función se llama desde la rutina de interrupción del SysTick del microcontrolador.
// El ciclo for itera sobre cada uno de los punteros (q) almacenados en el vector vCallBack
// Para cada puntero q en la lista, llama a su método Callback()
// 		- Si q apunta a un objeto TIMER, se ejecutará TIMER::Callback(). Si apuntara a otro objeto, como un ADC_Manager, se
// 		ejecutaría ADC_Manager::Callback(). De esta forma, con una sola llamada a CALLBACK::SysTickCall(), se actualizan
// 		todos los módulos registrados (temporizadores, etc.) en el sistema.
//
// *************************************************************************************************************
