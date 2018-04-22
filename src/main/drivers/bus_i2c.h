/*
 * bus_i2c.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_BUS_I2C_H_
#define SRC_MAIN_DRIVERS_BUS_I2C_H_

#pragma once

#include "platform.h"

#include "drivers/io_types.h"
#include "drivers/rcc_types.h"

#ifndef I2C_DEVICE
#define I2C_DEVICE I2CINVALID
#endif

typedef enum I2CDevice {
    I2CINVALID = -1,
    I2CDEV_1   = 0,
    I2CDEV_2,
    I2CDEV_3,
    I2CDEV_4,
} I2CDevice;

#if defined(STM32F1) || defined(STM32F3)
#define I2CDEV_COUNT 2
#elif defined(STM32F4)
#define I2CDEV_COUNT 3
#elif defined(STM32F7)
#define I2CDEV_COUNT 4
#else
#define I2CDEV_COUNT 4
#endif

// Macros to convert between CLI bus number and I2CDevice.
#define I2C_CFG_TO_DEV(x)   ((x) - 1)
#define I2C_DEV_TO_CFG(x)   ((x) + 1)

// I2C device address range in 7-bit address mode
#define I2C_ADDR7_MIN       8
#define I2C_ADDR7_MAX       119

struct i2cConfig_s;
void i2cHardwareConfigure(const struct i2cConfig_s *i2cConfig);
void i2cInit(I2CDevice device);
bool i2cWriteBuffer(I2CDevice device, uint8_t addr_, uint8_t reg_, uint8_t len_, uint8_t *data);
bool i2cWrite(I2CDevice device, uint8_t addr_, uint8_t reg, uint8_t data);
bool i2cRead(I2CDevice device, uint8_t addr_, uint8_t reg, uint8_t len, uint8_t* buf);

uint16_t i2cGetErrorCounter(void);



#endif /* SRC_MAIN_DRIVERS_BUS_I2C_H_ */