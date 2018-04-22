/*
 * barometer_ms5611.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_BAROMETER_BAROMETER_MS5611_H_
#define SRC_MAIN_DRIVERS_BAROMETER_BAROMETER_MS5611_H_

#pragma once

// MS5611, Standard address 0x77
#define MS5611_I2C_ADDR                 0x77

bool ms5611Detect(baroDev_t *baro);



#endif /* SRC_MAIN_DRIVERS_BAROMETER_BAROMETER_MS5611_H_ */
