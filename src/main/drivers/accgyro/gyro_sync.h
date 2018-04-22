/*
 * gyro_sync.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_ACCGYRO_GYRO_SYNC_H_
#define SRC_MAIN_DRIVERS_ACCGYRO_GYRO_SYNC_H_

#pragma once

#include "drivers/accgyro/accgyro.h"

bool gyroSyncCheckUpdate(gyroDev_t *gyro);
uint32_t gyroSetSampleRate(gyroDev_t *gyro, uint8_t lpf, uint8_t gyroSyncDenominator, bool gyro_use_32khz);



#endif /* SRC_MAIN_DRIVERS_ACCGYRO_GYRO_SYNC_H_ */
