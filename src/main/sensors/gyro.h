/*
 * gyro.h
 *
 *  Created on: 2018. 4. 19.
 *      Author: kangju
 */

#ifndef SRC_MAIN_SENSORS_GYRO_H_
#define SRC_MAIN_SENSORS_GYRO_H_

#pragma once

#include "common/axis.h"
#include "common/time.h"
#include "pg/pg.h"
#include "drivers/bus.h"
#include "drivers/sensor.h"

typedef enum {
    GYRO_NONE = 0,
    GYRO_DEFAULT,
    GYRO_MPU6050,
    GYRO_FAKE
} gyroSensor_e;

typedef struct gyro_s {
    uint32_t targetLooptime;
    float gyroADCf[XYZ_AXIS_COUNT];
} gyro_t;

extern gyro_t gyro;

typedef enum {
    GYRO_OVERFLOW_CHECK_NONE = 0,
    GYRO_OVERFLOW_CHECK_YAW,
    GYRO_OVERFLOW_CHECK_ALL_AXES
} gyroOverflowCheck_e;

#define GYRO_CONFIG_USE_GYRO_1      0
#define GYRO_CONFIG_USE_GYRO_2      1
#define GYRO_CONFIG_USE_GYRO_BOTH   2

typedef struct gyroConfig_s {
    sensor_align_e gyro_align;              // gyro alignment
    uint8_t  gyroMovementCalibrationThreshold; // people keep forgetting that moving model while init results in wrong gyro offsets. and then they never reset gyro. so this is now on by default.
    uint8_t  gyro_sync_denom;                  // Gyro sample divider
    uint8_t  gyro_lpf;                         // gyro LPF setting - values are driver specific, in case of invalid number, a reasonable default ~30-40HZ is chosen.
    uint8_t  gyro_soft_lpf_type;
    uint8_t  gyro_soft_lpf_hz;
    bool     gyro_high_fsr;
    bool     gyro_use_32khz;
    uint8_t  gyro_to_use;
    uint16_t gyro_soft_lpf_hz_2;
    uint16_t gyro_soft_notch_hz_1;
    uint16_t gyro_soft_notch_cutoff_1;
    uint16_t gyro_soft_notch_hz_2;
    uint16_t gyro_soft_notch_cutoff_2;
    gyroOverflowCheck_e checkOverflow;
    uint16_t gyro_filter_q;
    uint16_t gyro_filter_r;
    uint16_t gyro_filter_p;
    int16_t  gyro_offset_yaw;
} gyroConfig_t;

PG_DECLARE(gyroConfig_t, gyroConfig);

bool gyroInit(void);

void gyroInitFilters(void);
void gyroUpdate(timeUs_t currentTimeUs);
bool gyroGetAccumulationAverage(float *accumulation);
const busDevice_t *gyroSensorBus(void);
struct mpuConfiguration_s;
const struct mpuConfiguration_s *gyroMpuConfiguration(void);
struct mpuDetectionResult_s;
const struct mpuDetectionResult_s *gyroMpuDetectionResult(void);
void gyroStartCalibration(bool isFirstArmingCalibration);
bool isFirstArmingGyroCalibrationRunning(void);
bool isGyroCalibrationComplete(void);
void gyroReadTemperature(void);
int16_t gyroGetTemperature(void);
int16_t gyroRateDps(int axis);
bool gyroOverflowDetected(void);
uint16_t gyroAbsRateDps(int axis);

#endif /* SRC_MAIN_SENSORS_GYRO_H_ */
