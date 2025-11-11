/*
 * TIMER.h
 *
 *  Created on: 20 ago. 2025
 *      Author: guido
 */

#ifndef TIMER_DRIVER_TIMER_H_
#define TIMER_DRIVER_TIMER_H_

class TIMER : public CALLBACK{
private:
	uint32_t time;				//  Almacena el valor actual del contador del temporizador. Va disminuyendo en cada "tick".
	uint32_t time_Reload;		//	Guarda el valor con el que se debe recargar el contador time una vez que llega a cero. Esto permite que el temporizador sea periódico.
	void (*func)(void);			//	Puntero a función, guarda la dirección de memoria de una función. El temporizador usará este puntero para "llamar de vuelta" (callback) a la función específica cuando el tiempo se agote.

public:
	TIMER(uint32_t time_Start, uint32_t time_Reload, void(*func)(void));
	void Start(uint32_t time_Start, uint32_t time_Reload, void(*func)(void));
	void Discount(void);
	void Stop(void);
	void Callback(void);
};

#endif /* TIMER_DRIVER_TIMER_H_ */
