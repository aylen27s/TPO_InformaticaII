/*
 * TIMER.h
 *
 *  Created on: 9 sep. 2025
 *      Author: aylen
 */

#ifndef TIMER_TIMER_H_
#define TIMER_TIMER_H_

class TIMER : public CALLBACK{
private:
	uint32_t time;
	uint32_t timeReload;

	void (*func)(void);

public:
	TIMER(uint32_t timeStart, uint32_t timeReload, void (*func)(void));

	void Start(uint32_t timeStart, uint32_t timeReload, void (*func)(void));

	void Discount(void);

	void Stop(void);

	void Callback( void );
};

#endif /* TIMER_TIMER_H_ */
