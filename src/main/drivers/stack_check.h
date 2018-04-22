/*
 * stack_check.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_STACK_CHECK_H_
#define SRC_MAIN_DRIVERS_STACK_CHECK_H_

#pragma once

#include "common/time.h"

void taskStackCheck(timeUs_t currentTimeUs);
uint32_t stackUsedSize(void);
uint32_t stackTotalSize(void);
uint32_t stackHighMem(void);



#endif /* SRC_MAIN_DRIVERS_STACK_CHECK_H_ */
