/*
 * GPIOFO.h
 *
 *  Created on: 20 ago. 2025
 *      Author: aylen
 */


class GpioFO : private Gpio, public CALLBACK {
private:
	uint32_t m_contador;
	uint32_t m_tiempoActivacion;
	uint32_t m_estado;

public:
	GpioFO(uint32_t port, uint32_t pin, uint32_t estadoInicial, uint32_t _tiempo_de_activacion);
	void Callback();
	void Set(uint32_t estado);
	virtual ~GpioFO();
};

