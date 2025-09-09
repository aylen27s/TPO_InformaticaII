/*
 * CALLBACK.cpp
 *
 *  Created on: 17 jun. 2025
 *      Author: aylen
 */

#include "MyIncludes.h"

std::vector<CALLBACK*> vCallback;

CALLBACK::CALLBACK() {
	//Cada vez que se construye un objeto que hereda de esta clase, el objeto se encola en el vector vCallback.
	vCallback.push_back(this);
}


void CALLBACK::Callback(void){
	//Se deja vacío para que cada clase hija pueda hacer su propia implementación
}


void CALLBACK::SysTickCall( void ){
	//Metodo llamado desde el Handler de interrupción del Systick
	for (CALLBACK* q: vCallback)
		q->Callback();
}
