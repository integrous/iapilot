/*
 * pinio.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_PINIO_H_
#define SRC_MAIN_DRIVERS_PINIO_H_

#pragma once

#include <stdbool.h>

#ifndef PINIO_COUNT
#define PINIO_COUNT 4
#endif

#define PINIO_CONFIG_OUT_INVERTED 0x80
#define PINIO_CONFIG_MODE_MASK    0x7F
#define PINIO_CONFIG_MODE_OUT_PP  0x01

struct pinioConfig_s;

void pinioInit(const struct pinioConfig_s *pinioConfig);
void pinioSet(int index, bool on);



#endif /* SRC_MAIN_DRIVERS_PINIO_H_ */
