/*
 * adcinternal.h
 *
 *  Created on: 2018. 4. 19.
 *      Author: kangju
 */

#ifndef SRC_MAIN_SENSORS_ADCINTERNAL_H_
#define SRC_MAIN_SENSORS_ADCINTERNAL_H_

#pragma once

#include "drivers/time.h"

void adcInternalInit(void);
void adcInternalProcess(timeUs_t currentTimeUs);
int16_t getCoreTemperatureCelsius(void);
uint16_t getVrefMv(void);

#endif /* SRC_MAIN_SENSORS_ADCINTERNAL_H_ */
