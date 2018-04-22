/*
 * bus_i2c_busdev.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_BUS_I2C_BUSDEV_H_
#define SRC_MAIN_DRIVERS_BUS_I2C_BUSDEV_H_

#pragma once

bool i2cBusWriteRegister(const busDevice_t *busdev, uint8_t reg, uint8_t data);
bool i2cBusReadRegisterBuffer(const busDevice_t *busdev, uint8_t reg, uint8_t *data, uint8_t length);
uint8_t i2cBusReadRegister(const busDevice_t *bus, uint8_t reg);




#endif /* SRC_MAIN_DRIVERS_BUS_I2C_BUSDEV_H_ */
