/*
 * accgyro_spi_cpu6500.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_ACCGYRO_ACCGYRO_SPI_CPU6500_H_
#define SRC_MAIN_DRIVERS_ACCGYRO_ACCGYRO_SPI_CPU6500_H_

#pragma once

#include "drivers/bus.h"

uint8_t mpu6500SpiDetect(const busDevice_t *bus);

bool mpu6500SpiAccDetect(accDev_t *acc);
bool mpu6500SpiGyroDetect(gyroDev_t *gyro);

void mpu6500SpiGyroInit(gyroDev_t *gyro);
void mpu6500SpiAccInit(accDev_t *acc);



#endif /* SRC_MAIN_DRIVERS_ACCGYRO_ACCGYRO_SPI_CPU6500_H_ */
