/*
 * CALLBACK.h
 *
 *  Created on: 17 jun. 2025
 *      Author: aylen
 */

#ifndef CALLBACK_CALLBACK_H_
#define CALLBACK_CALLBACK_H_

class CALLBACK {
public:
	CALLBACK();
	virtual void Callback(); // acá se ejecuta la tarea. Es virtual para que las clases hijas(las que heredan) puedan poner su propia implementacion dependiendo del tipo de periferico que sea
	static void SysTickCall(); //acá se recorre el vector de callbacks. Tiene que ser unica para que el systick handler recorra la lista de tareas una sola vez

};

#endif /* CALLBACK_CALLBACK_H_ */
