/*
 * Gpio.h
 *
 *  Created on: 14 ago. 2025
 *      Author: guido
 */

#ifndef GPIO_DRIVER_GPIO_H_
#define GPIO_DRIVER_GPIO_H_

class Gpio{
private:
	uint32_t port;
	uint32_t pin;
	uint32_t dir;
public:
	Gpio(uint32_t port, uint32_t pin, uint32_t dir);
	void Set(uint32_t state);
	void Dir(uint32_t dir);
	uint32_t Get(uint32_t port, uint32_t pin);
	uint32_t Get(void);

};

#endif /* GPIO_DRIVER_GPIO_H_ */
