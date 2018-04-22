/*
 * pwm_output_counts.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_PWM_OUTPUT_COUNTS_H_
#define SRC_MAIN_DRIVERS_PWM_OUTPUT_COUNTS_H_

#pragma once

#include "platform.h"


#if defined(USE_QUAD_MIXER_ONLY)
#define MAX_SUPPORTED_MOTORS 4
#define MAX_SUPPORTED_SERVOS 1
#else
#ifndef MAX_SUPPORTED_MOTORS
#define MAX_SUPPORTED_MOTORS 8
#endif
#define MAX_SUPPORTED_SERVOS 8
#endif



#endif /* SRC_MAIN_DRIVERS_PWM_OUTPUT_COUNTS_H_ */
