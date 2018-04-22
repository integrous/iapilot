/*
 * pwm_esc_detect.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_PWM_ESC_DETECT_H_
#define SRC_MAIN_DRIVERS_PWM_ESC_DETECT_H_

#pragma once

#ifdef USE_BRUSHED_ESC_AUTODETECT
typedef enum {
    MOTOR_UNKNOWN = 0,
    MOTOR_BRUSHED,
    MOTOR_BRUSHLESS
} HardwareMotorTypes_e;

extern uint8_t hardwareMotorType;

void detectBrushedESC(void);
#endif



#endif /* SRC_MAIN_DRIVERS_PWM_ESC_DETECT_H_ */
