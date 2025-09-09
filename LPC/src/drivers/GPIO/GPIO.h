/*
 * GPIO.h
 *
 *  Created on: 17 jun. 2025
 *      Author: aylen
 */

class Gpio{

	private:
		uint32_t m_port;
		uint32_t m_pin;
		uint32_t m_mode; //Entrada o salida
		uint32_t m_currState;

	public:
		Gpio(uint32_t port, uint32_t pin, uint32_t mode, uint32_t state = OFF_LED);
		~Gpio();

		void Set(uint32_t state); //Encendido o apagado
		void SetMode(uint32_t mode); //Entrada o salida
		void TogglePin(void);
		uint32_t Get(void); //Lectura del pin

};
