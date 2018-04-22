/*
 * flash_m25p16.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_FLASH_M25P16_H_
#define SRC_MAIN_DRIVERS_FLASH_M25P16_H_

#pragma once

#include <stdint.h>
#include "flash.h"

#define M25P16_PAGESIZE 256

struct flashConfig_s;
bool m25p16_init(const struct flashConfig_s *flashConfig);

void m25p16_eraseSector(uint32_t address);
void m25p16_eraseCompletely(void);

void m25p16_pageProgram(uint32_t address, const uint8_t *data, int length);

void m25p16_pageProgramBegin(uint32_t address);
void m25p16_pageProgramContinue(const uint8_t *data, int length);
void m25p16_pageProgramFinish(void);

int m25p16_readBytes(uint32_t address, uint8_t *buffer, int length);

bool m25p16_isReady(void);
bool m25p16_waitForReady(uint32_t timeoutMillis);

struct flashGeometry_s;
const struct flashGeometry_s* m25p16_getGeometry(void);



#endif /* SRC_MAIN_DRIVERS_FLASH_M25P16_H_ */
