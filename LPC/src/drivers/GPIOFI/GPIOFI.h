/*
 * GPIOF.h
 *
 *  Created on: 24 jun. 2025
 *  Author: aylen
 *  GPIO con antirebote de entradas GPIO.
 */


#define CANT_ESTADOS_VALIDOS 20

class GpioFI : public CALLBACK, public Gpio{
private:
	uint8_t m_contador;
	uint8_t m_estAnterior;
	uint8_t m_estEstable;

public:
	GpioFI(uint32_t port, uint32_t pin);
	virtual ~GpioFI();
	void Antirebote(void);
	void Callback (void);

};

