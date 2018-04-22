/*
 * boardalignment.h
 *
 *  Created on: 2018. 4. 19.
 *      Author: kangju
 */

#ifndef SRC_MAIN_SENSORS_BOARDALIGNMENT_H_
#define SRC_MAIN_SENSORS_BOARDALIGNMENT_H_

#pragma once

#include "pg/pg.h"

typedef struct boardAlignment_s {
    int32_t rollDegrees;
    int32_t pitchDegrees;
    int32_t yawDegrees;
} boardAlignment_t;

PG_DECLARE(boardAlignment_t, boardAlignment);

void alignSensors(float *dest, uint8_t rotation);
void initBoardAlignment(const boardAlignment_t *boardAlignment);



#endif /* SRC_MAIN_SENSORS_BOARDALIGNMENT_H_ */
