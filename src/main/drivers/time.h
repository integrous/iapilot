/*
 * time.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_TIME_H_
#define SRC_MAIN_DRIVERS_TIME_H_

#pragma once

#include <stdint.h>

#include "common/time.h"

void delayMicroseconds(timeUs_t us);
void delay(timeMs_t ms);

timeUs_t micros(void);
timeUs_t microsISR(void);
timeMs_t millis(void);

uint32_t ticks(void);
timeDelta_t ticks_diff_us(uint32_t begin, uint32_t end);



#endif /* SRC_MAIN_DRIVERS_TIME_H_ */
