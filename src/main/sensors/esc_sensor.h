/*
 * esc_sensor.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_SENSORS_ESC_SENSOR_H_
#define SRC_MAIN_SENSORS_ESC_SENSOR_H_

#pragma once

#include "common/time.h"

typedef struct escSensorConfig_s {
    uint8_t halfDuplex;             // Set to false to listen on the TX pin for telemetry data
} escSensorConfig_t;

PG_DECLARE(escSensorConfig_t, escSensorConfig);

typedef struct {
    uint8_t dataAge;
    int8_t temperature;
    int16_t voltage;
    int16_t current;
    int16_t consumption;
    int16_t rpm;
} escSensorData_t;

#define ESC_DATA_INVALID 255

#define ESC_BATTERY_AGE_MAX 10

bool escSensorInit(void);
void escSensorProcess(timeUs_t currentTime);

#define ESC_SENSOR_COMBINED 255

escSensorData_t *getEscSensorData(uint8_t motorNumber);

void startEscDataRead(uint8_t *frameBuffer, uint8_t frameLength);
uint8_t getNumberEscBytesRead(void);

uint8_t calculateCrc8(const uint8_t *Buf, const uint8_t BufLen);



#endif /* SRC_MAIN_SENSORS_ESC_SENSOR_H_ */
