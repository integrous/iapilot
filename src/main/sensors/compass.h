/*
 * compass.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_SENSORS_COMPASS_H_
#define SRC_MAIN_SENSORS_COMPASS_H_

#pragma once

#include "common/time.h"
#include "drivers/io_types.h"
#include "drivers/sensor.h"
#include "pg/pg.h"
#include "sensors/sensors.h"


// Type of magnetometer used/detected
typedef enum {
    MAG_DEFAULT = 0,
    MAG_NONE = 1,

} magSensor_e;

typedef struct mag_s {
    float magADC[XYZ_AXIS_COUNT];
    float magneticDeclination;
} mag_t;

extern mag_t mag;

typedef struct compassConfig_s {
    int16_t mag_declination;                // Get your magnetic decliniation from here : http://magnetic-declination.com/

    sensor_align_e mag_align;
    uint8_t mag_hardware;
    uint8_t mag_bustype;
    uint8_t mag_i2c_device;
    uint8_t mag_i2c_address;
    uint8_t mag_spi_device;
    ioTag_t mag_spi_csn;
    ioTag_t interruptTag;
    flightDynamicsTrims_t magZero;
} compassConfig_t;

PG_DECLARE(compassConfig_t, compassConfig);

bool compassIsHealthy(void);
void compassUpdate(timeUs_t currentTime);
bool compassInit(void);



#endif /* SRC_MAIN_SENSORS_COMPASS_H_ */
