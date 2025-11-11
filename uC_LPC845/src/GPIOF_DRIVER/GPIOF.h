/*
 * GPIOF.h
 *
 *  Created on: 2 sep. 2025
 *      Author: guido
 */

#ifndef GPIOF_DRIVER_GPIOF_H_
#define GPIOF_DRIVER_GPIOF_H_
#define  CANTIDAD_ESTADOS_ESTABLES 20

class GPIOF : public Gpio, public CALLBACK{

private:
	uint8_t contador;
	uint8_t estAnterior;
	uint8_t estEstable;

public:
	GPIOF(uint32_t port, uint32_t pin);

	void Antirebote(void);

	void Callback( void );

	uint32_t Get(void);
};

#endif /* GPIOF_DRIVER_GPIOF_H_ */
