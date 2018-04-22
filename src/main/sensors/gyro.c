/*
 * gyro.c
 *
 *  Created on: 2018. 4. 19.
 *      Author: kangju
 */


#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "platform.h"

#include "build/debug.h"

#include "common/axis.h"
#include "common/maths.h"
#include "common/filter.h"

#include "config/feature.h"

#include "pg/pg.h"
#include "pg/pg_ids.h"

#include "drivers/accgyro/accgyro.h"
#include "drivers/accgyro/accgyro_fake.h"
#include "drivers/accgyro/accgyro_mpu.h"
#include "drivers/accgyro/accgyro_mpu6050.h"
#include "drivers/accgyro/gyro_sync.h"
#include "drivers/bus_spi.h"
#include "drivers/io.h"

#include "fc/config.h"
#include "fc/runtime_config.h"

#include "io/beeper.h"
#include "io/statusindicator.h"

#include "scheduler/scheduler.h"

#include "sensors/boardalignment.h"
#include "sensors/gyro.h"
#include "sensors/gyroanalyse.h"
#include "sensors/sensors.h"

#ifdef USE_HARDWARE_REVISION_DETECTION
#include "hardware_revision.h"
#endif

#if ((FLASH_SIZE > 128) && (defined(USE_GYRO_SPI_ICM20601) || defined(USE_GYRO_SPI_ICM20689) || defined(USE_GYRO_SPI_MPU6500)))
#define USE_GYRO_SLEW_LIMITER
#endif

FAST_RAM gyro_t gyro;
static FAST_RAM uint8_t gyroDebugMode;

static uint8_t gyroToUse = 0;

#ifdef USE_GYRO_OVERFLOW_CHECK
static FAST_RAM uint8_t overflowAxisMask;
#endif
static FAST_RAM float accumulatedMeasurements[XYZ_AXIS_COUNT];
static FAST_RAM float gyroPrevious[XYZ_AXIS_COUNT];
static FAST_RAM timeUs_t accumulatedMeasurementTimeUs;
static FAST_RAM timeUs_t accumulationLastTimeSampledUs;

typedef struct gyroCalibration_s {
    int32_t sum[XYZ_AXIS_COUNT];
    stdev_t var[XYZ_AXIS_COUNT];
    uint16_t calibratingG;
} gyroCalibration_t;

bool firstArmingCalibrationWasStarted = false;

typedef union gyroSoftFilter_u {
    biquadFilter_t gyroFilterLpfState[XYZ_AXIS_COUNT];
    pt1Filter_t gyroFilterPt1State[XYZ_AXIS_COUNT];
    firFilterDenoise_t gyroDenoiseState[XYZ_AXIS_COUNT];
} gyroSoftLpfFilter_t;

typedef struct gyroSensor_s {
    gyroDev_t gyroDev;
    gyroCalibration_t calibration;
    // gyro soft filter
    filterApplyFnPtr softLpfFilterApplyFn;
    gyroSoftLpfFilter_t softLpfFilter;
    filter_t *softLpfFilterPtr[XYZ_AXIS_COUNT];
    // notch filters
    filterApplyFnPtr notchFilter1ApplyFn;
    biquadFilter_t notchFilter1[XYZ_AXIS_COUNT];
    filterApplyFnPtr notchFilter2ApplyFn;
    biquadFilter_t notchFilter2[XYZ_AXIS_COUNT];
    filterApplyFnPtr notchFilterDynApplyFn;
    biquadFilter_t notchFilterDyn[XYZ_AXIS_COUNT];
    timeUs_t overflowTimeUs;
    bool overflowDetected;
#if defined(USE_GYRO_FAST_KALMAN)
    // gyro kalman filter
    filterApplyFnPtr fastKalmanApplyFn;
    fastKalman_t fastKalman[XYZ_AXIS_COUNT];
#elif defined(USE_GYRO_BIQUAD_RC_FIR2)
    // gyro biquad RC FIR2 filter
    filterApplyFnPtr biquadRCFIR2ApplyFn;
    biquadFilter_t biquadRCFIR2[XYZ_AXIS_COUNT];
#endif
} gyroSensor_t;

STATIC_UNIT_TESTED FAST_RAM gyroSensor_t gyroSensor1;
#ifdef USE_DUAL_GYRO
STATIC_UNIT_TESTED FAST_RAM gyroSensor_t gyroSensor2;
#endif

#ifdef UNIT_TEST
STATIC_UNIT_TESTED gyroSensor_t * const gyroSensorPtr = &gyroSensor1;
STATIC_UNIT_TESTED gyroDev_t * const gyroDevPtr = &gyroSensor1.gyroDev;
#endif

#if defined(USE_GYRO_FAST_KALMAN)
static void gyroInitFilterKalman(gyroSensor_t *gyroSensor, uint16_t gyro_filter_q, uint16_t gyro_filter_r, uint16_t gyro_filter_p);
#elif defined (USE_GYRO_BIQUAD_RC_FIR2)
static void gyroInitFilterBiquadRCFIR2(gyroSensor_t *gyroSensor, uint16_t lpfHz);
#endif
static void gyroInitSensorFilters(gyroSensor_t *gyroSensor);

#define DEBUG_GYRO_CALIBRATION 3

#ifdef STM32F10X
#define GYRO_SYNC_DENOM_DEFAULT 8
#elif defined(USE_GYRO_SPI_MPU6000) || defined(USE_GYRO_SPI_MPU6500) || defined(USE_GYRO_SPI_ICM20601) || defined(USE_GYRO_SPI_ICM20649) \
   || defined(USE_GYRO_SPI_ICM20689)
#define GYRO_SYNC_DENOM_DEFAULT 1
#else
#define GYRO_SYNC_DENOM_DEFAULT 4
#endif

#define GYRO_OVERFLOW_TRIGGER_THRESHOLD 31980  // 97.5% full scale (1950dps for 2000dps gyro)
#define GYRO_OVERFLOW_RESET_THRESHOLD 30340    // 92.5% full scale (1850dps for 2000dps gyro)

PG_REGISTER_WITH_RESET_TEMPLATE(gyroConfig_t, gyroConfig, PG_GYRO_CONFIG, 1);

#ifndef GYRO_CONFIG_USE_GYRO_DEFAULT
#ifdef USE_DUAL_GYRO
#define GYRO_CONFIG_USE_GYRO_DEFAULT GYRO_CONFIG_USE_GYRO_BOTH
#else
#define GYRO_CONFIG_USE_GYRO_DEFAULT GYRO_CONFIG_USE_GYRO_1
#endif
#endif

PG_RESET_TEMPLATE(gyroConfig_t, gyroConfig,
    .gyro_align = ALIGN_DEFAULT,
    .gyroMovementCalibrationThreshold = 48,
    .gyro_sync_denom = GYRO_SYNC_DENOM_DEFAULT,
    .gyro_lpf = GYRO_LPF_256HZ,
    .gyro_soft_lpf_type = FILTER_PT1,
    .gyro_soft_lpf_hz = 90,
    .gyro_high_fsr = false,
    .gyro_use_32khz = false,
    .gyro_to_use = GYRO_CONFIG_USE_GYRO_DEFAULT,
    .gyro_soft_notch_hz_1 = 400,
    .gyro_soft_notch_cutoff_1 = 300,
    .gyro_soft_notch_hz_2 = 200,
    .gyro_soft_notch_cutoff_2 = 100,
    .checkOverflow = GYRO_OVERFLOW_CHECK_ALL_AXES,
    .gyro_soft_lpf_hz_2 = 0,
    .gyro_filter_q = 0,
    .gyro_filter_r = 0,
    .gyro_filter_p = 0,
    .gyro_offset_yaw = 0,
);


const busDevice_t *gyroSensorBus(void)
{
#ifdef USE_DUAL_GYRO
    if (gyroToUse == GYRO_CONFIG_USE_GYRO_2) {
        return &gyroSensor2.gyroDev.bus;
    } else {
        return &gyroSensor1.gyroDev.bus;
    }
#else
    return &gyroSensor1.gyroDev.bus;
#endif
}
