/*
 * heartRate.h
 *
 *  Created on: 21 nov. 2025
 *      Author: aylen
 */

#ifndef DRIVERS_MMA845X_HEARTRATE_H_
#define DRIVERS_MMA845X_HEARTRATE_H_


bool checkForBeat(int32_t sample);
int16_t averageDCEstimator(int32_t *p, uint16_t x);
int16_t lowPassFIRFilter(int16_t din);
int32_t mul16(int16_t x, int16_t y);


#endif /* DRIVERS_MMA845X_HEARTRATE_H_ */

