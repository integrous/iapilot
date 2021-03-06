/*
 * light_led.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_LIGHT_LED_H_
#define SRC_MAIN_DRIVERS_LIGHT_LED_H_

#pragma once

#include "pg/pg.h"
#include "drivers/io_types.h"
#include "common/utils.h"

#define STATUS_LED_NUMBER 3

typedef struct statusLedConfig_s {
    ioTag_t ioTags[STATUS_LED_NUMBER];
    uint8_t inversion;
} statusLedConfig_t;

PG_DECLARE(statusLedConfig_t, statusLedConfig);

// Helpful macros
#if defined(UNIT_TEST) || defined(USE_FAKE_LED)

#define LED0_TOGGLE              NOOP
#define LED0_OFF                 NOOP
#define LED0_ON                  NOOP

#define LED1_TOGGLE              NOOP
#define LED1_OFF                 NOOP
#define LED1_ON                  NOOP

#define LED2_TOGGLE              NOOP
#define LED2_OFF                 NOOP
#define LED2_ON                  NOOP

#else

#define LED0_TOGGLE              ledToggle(0)
#define LED0_OFF                 ledSet(0, false)
#define LED0_ON                  ledSet(0, true)

#define LED1_TOGGLE              ledToggle(1)
#define LED1_OFF                 ledSet(1, false)
#define LED1_ON                  ledSet(1, true)

#define LED2_TOGGLE              ledToggle(2)
#define LED2_OFF                 ledSet(2, false)
#define LED2_ON                  ledSet(2, true)

void ledInit(const statusLedConfig_t *statusLedConfig);
void ledToggle(int led);
void ledSet(int led, bool state);

#endif




#endif /* SRC_MAIN_DRIVERS_LIGHT_LED_H_ */
