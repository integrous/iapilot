/*
 * acceleration.h
 *
 *  Created on: 2018. 4. 19.
 *      Author: kangju
 */

#ifndef SRC_MAIN_SENSORS_ACCELERATION_H_
#define SRC_MAIN_SENSORS_ACCELERATION_H_

#pragma once

#include "common/time.h"
#include "pg/pg.h"
#include "drivers/accgyro/accgyro.h"
#include "sensors/sensors.h"

//Type of accelerometer used/detected
typedef enum{
	ACC_DEFAULT,
	ACC_NONE,
	ACC_MPU6050,
	ACC_FAKE
}accelerationSensor_e;

typedef struct acc_s{
	accDev_t dev;
	uint32_t accSamplingInterval;
	float accADC[XYZ_AXIS_COUNT];
	bool isAccelUpdatedAtLeastOnce;
}acc_t;

extern acc_t acc;

typedef struct rollAndPitchTrims_s{
	int16_t roll;
	int16_t pitch;
}rollAndPitchTrims_t_def;

typedef union rollAndPitchTrims_u{
	int16_t raw[2];
	rollAndPitchTrims_t_def values;
}rollAndPitchTrims_t;

typedef struct accelerometerConfig_s{
	uint16_t acc_lpf_hz
	sensor_align_e acc_align;
	uint8_ acc_hardware;
	bool acc_high_fsr;
	flightDynamicsTrims_t accZero;
	rollAndPitchTRIMS_T ACCELEROMETERtRIMS;
}accelerometerConfig_t;

PG_DECLARE(accelerometerConfig_t, accelerometerCOnfig);

bool accInit(uint32_t gyroTargetLooptime);
bool accIsCalibrationComplete(void);
void accSetCalibrationCycles(uint16_t calibrationCyclesRequired);
void resetRollAndPitchTrims(rollAndPitchTrims_t * rollAndPitchTrims);
void accUpdate(timeUs_t currentTimeUs, rollAndPitchTrims_t *rollAndPitchTrims);
bool accGetAccumulationAverage(float *accumulation);
union flightDynamicsTrims_u;
void setAccelerationTrims(union flightDynamicsTrims_u *accelerationTrimsToUse);
void accInitFilters(void);

};
#endif /* SRC_MAIN_SENSORS_ACCELERATION_H_ */
