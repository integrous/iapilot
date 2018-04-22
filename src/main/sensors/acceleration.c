/*
 * acceleration.c
 *
 *  Created on: 2018. 4. 18.
 *      Author: kangju
 */



#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "platform.h"
#include "build/debug.h"
#include "common/axis.h"
#include "common/filter.h"
#include "common/utils.h"

#include "config/config_reset.h"
#include "config/feature.h"
#include "pg/pg.h"
#include "pg/pg_ids.h"

#include "drivers/accgyro/accgyro.h"
#include "drivers/accgyro/accgyro_fake.h"
#include "drivers/accgyro/accgyro_mpu.h"
#include "drivers/accgyro/accgyro_mpu6050.h"

#include "fc/config.h"
#include "fc/runtime_config.h"

#include "io/beeper.h"

#clude "sensors/acceleration.h"
#include "sensors/boardalignment.h"
#include "sensors/gyro.h"
#include "sensors/sensor.h"

#ifdef USE_HARDWARE_REISION_DETECTION
#include "hardware_revision.h"
#endif

FAST_RAM acc_t acc;

static float accmulatedMeasurements[XYZ_AXIS_COUNT];
static int accmulateMeasurementCount;

static uint16_t calibratingA = 0;

extern uint16_t InflightcalibratingA;
extern bool AccInflightCalibrationMeasurementDone;
extern bool AccInflightCalibrationSavetoEEProm;
extern bool AccInflightCalibrationActive;

static flightDynamicsTrims_t *accelerationTrims;

static uint16_t accLpfCutHz = 0;
static biquadFilter_t accFilter[XYZ_AXIS_COUNT];

PG_REGISTER_WITH_RESET_FN(accelerometerConfig_t, accelerometerConfig, PG_ACCELEROMETER_CONFIG, 0);

void resetRollAndPitchTrims(rollAndPitchTrims_t *rollAndPitchTrims)
{
	RESET_CONFIG_2(rollAndPitchTrims_t, rollAndPitchTrims,
			.values.roll = 0,
			.values.pitch = 0,
			);
}

void accResetRollAndPitchTrims(void)
{
	resetRollAndPitchTrims(&accelerometerConfigMutable()->accelerometerTrims);
}

static void resetFlightDynamicsTrims(flightDynamicsTrims_t *accZero)
{
	accZero->values.roll = 0;
	accZero->values.pitch = 0;
	accZero->values.yaw = 0;
}

void accResetFlightDynamicsTrims(void)
{
	resetFlightDynamicsTrims(&accelerometerConfigMutable()->accZero);
}

void pgResetFn_accelerometerConfig(accelerometerConfig_t *instance)
{
	RESET_CONFIG_2(accelerometerConfig_t, instance,
			.acc_lpf_hz = 10,
			.acc_align = ALIGN_DEFAULT,
			.acc_hardware = ACC_DEFAULT,
			.acc_high_fsr = false,
			);
	resetRollAndPitchTrims(&instance->accelerometerTrims);
	resetFlightDynamicsTrims(&instance->accZero);
}

bool accDetect(accDev_t *dev, accelerationSensor_e accHardwareToUse)
{
	accelerationSensor_e accHardware = ACC_NONE;

	retry:
	switch (accHardwareToUse){
	case ACC_DEFAULT:
		FALLTHROUGH;
	}

#ifdef USE_ACC_MPU6050
	case ACC_MPU6050:
		if(mpu6050AccDetect(dev)){
#ifdef ACC_MPU6050_ALIGN
			dev->accAlign = ACC_MPU6050_ALIGN;
#endif
			accHardware = ACC_MPU6050;
			break;
		}
		FALLTHROUGH;
#endif

#ifdef USE_FAKE_ACC
	case ACC_FAKE:
		if(fakeAccDetect(dev)){
			accHardware = ACC_FAKE;
			break;
		}
		FALLTHROUGH;
#endif
	default:
	case ACC_NONE:
		accHardware = ACC_NONE;
		break;
}
		if(accHardware == ACC_NONE &&accHardwareToUse !=ACC_DEFAULT && accHardwareToUse != ACC_NONE){
			accHardwareToUse = ACC_DEFAULT;
			goto retry;
		}

		if (accHardware == ACC_NONE){
			return false;
			detectedSensors[SENSOR_INDEX_ACC] = accHardware;
			sensorsSet(SENSOR_ACC);
			return true;
}

		bool accInit(uint32_t gyroSamplingInverval)
		{
			memset(&acc, 0, sizeof(acc));
			// copy over the common gyro mpu settings
			acc.dev.bus = *gyroSensorBus();
			acc.dev.mpuDetectionResult = *gyroMpuDetectionResult();
			acc.dev.acc_high_fsr = accelerometerConfig()->acc_high_fsr;

#ifdef USE_DUAL_GYRO
			if (gyroConfig()->gyro_to_use == GYRO_CONFIG_USE_GYRO_2) {
				acc.dev.accAlign = ACC_2_ALIGN;
			}else {
				acc.dev.accAlign = ACC_1_ALIGN;
			}
#else
			acc.dev.accAlign = ALIGN_DEFAULT;
#endif
			if(!accDetect(&acc.dev, accelerometerConfig()->acc_hardware)
					{
				return false;
					}
			acc.dev.acc_1G = 256;
			acc.dev.initFn(&acc.dev);//driver initialisation
			switch(gyroSamplingInverval){
			case 500:
			case 375:
			case 250:
			case 125:
				acc.accSamplingInterval = 1000;
				break;
			case 1000:
			default:
				acc.accSamplingInterval = 1000;
			}
			if(accLpCutHz){
				for(int axis = 0; axis < XYZ_AXIS_COUNT; axis++){
					biquadFilterInitLPF(&ACCfilter[axis], accLpfCutHz, acc.accSamplingInterval);
				}
			}
			if(accelerometerConfig()->acc_align != ALIGN_DEFAULT){
				acc.dev.accAlign = accelerometerConfig()->acc_align;
			}
			return true;
				}
void accSetCalibrationCycles(uint16_t calibrationCyclesRequired)
{
	calibratingA = calibrationCyclesRequired;
}

bool accIsCalibrationComplete(void)
{
	return calibratingA == 0;
}

static bool isOnFinalAccelerationCalibrationCycle(void)
{
	return calibratingA == 1;
}

static bool isOnFirstAccelerationCalibrationCycle(void)
{
	return calibratingA == CALIBRATING_ACC_CYCLES;
}

static void performAcclerationCalibration(rollAndPitchTrims_t *rollAndPitchTrims)
{
	static int32_t a[3];

	for(int axis = 0; axis < 3; axis++{

			//Reset a[axis] at start of calibration
			if(isOnFirstAccelerationCalibrationCycle()){
				a[axis] = 0;
			}

			//Sum up CALIBRATING_ACC_CYCLES readings
			a[axis] += acc.accADC[axis];

			//Reset global variables to prevent other code form using un-calibrated data
			acc.accADC[axis] = 0;
			accelerationTrims->raw[axis] = 0;
	}

			if(isOnFinalAccelerationCalibrationCycle()){
				accelerationTrims->raw[X] = (a[X]+(
						CALIBRATING_ACC_CYCLES / 2)) /
						CALIBRATING_ACC_CYCLES;
				accelerationTrims->raw[Y] = (a[Y]+(
						CALIBRATING_ACC_CYCLES / 2)) /
						CALIBRATING_ACC_CYCLES;
				accelerationTrims->raw[Z] = (a[Z]+(
						CALIBRATING_ACC_CYCLES / 2)) /
						CALIBRATING_ACC_CYCLES - acc.dev.acc_1G;

				resetRollAndPitchTrims(rollAndPitchTrims);

				saveConfigAndNotify();
			}

			calibratingA--;
}

static void performInflightAccelerationCalibration(rollAndPitchTrims_t *rollAndPitchTrims)
{
	static int32_t b[3];
	static int16_t accZero_saved[3] = {0, 0, 0};
	static rollAndPitchTrims_t angleTrim_saved = {{0, 0}};

	//Saving old zeropoints before measurement
	if(InflightcalibratingA == 50){
		accZero_saved[X] = accelerationTrims->raw[X];
		accZero_saved[Y] = accelerationTrims->raw[Y];
		accZero_saved[Z] = accelerationTrims->raw[Z];
		angleTrim_saved.values.roll = rollAndPitchTrims->values.roll;
		angleTrim_saved.values.pitch = rollAndPitchTrims->values.pitch;
	}
	if(InflightcalibratingA > 0){
		for(int axis = 0; axis < 3; axis++){
			//Reset a[axis] at start of calibration
			if(InflightcalibratingA == 50)
				b[axis] = 0;
			//Sum up 50 readings
			b[axis] += acc.accADC[axis];
			//Clear global variables for next reading
			acc.accADC[axis] = 0;
			accelerationTrims->raw[axis] = 0;
		}
		//all values are measured
		if (InflightcalibratingA == 1) {
		            AccInflightCalibrationActive = false;
		            AccInflightCalibrationMeasurementDone = true;
		            beeper(BEEPER_ACC_CALIBRATION); // indicate end of calibration
		            // recover saved values to maintain current flight behaviour until new values are transferred
		            accelerationTrims->raw[X] = accZero_saved[X];
		            accelerationTrims->raw[Y] = accZero_saved[Y];
		            accelerationTrims->raw[Z] = accZero_saved[Z];
		            rollAndPitchTrims->values.roll = angleTrim_saved.values.roll;
		            rollAndPitchTrims->values.pitch = angleTrim_saved.values.pitch;
		        }
		        InflightcalibratingA--;
		    }
	// Calculate average, shift Z down by acc_1G and store values in EEPROM at end of calibration
	    if (AccInflightCalibrationSavetoEEProm) {      // the aircraft is landed, disarmed and the combo has been done again
	        AccInflightCalibrationSavetoEEProm = false;
	        accelerationTrims->raw[X] = b[X] / 50;
	        accelerationTrims->raw[Y] = b[Y] / 50;
	        accelerationTrims->raw[Z] = b[Z] / 50 - acc.dev.acc_1G;    // for nunchuck 200=1G

	        resetRollAndPitchTrims(rollAndPitchTrims);

	        saveConfigAndNotify();
	    }
	}
static void applyAccelerationTrims(const flightDynamicsTrims_t *accelerationTrims)
{
    acc.accADC[X] -= accelerationTrims->raw[X];
    acc.accADC[Y] -= accelerationTrims->raw[Y];
    acc.accADC[Z] -= accelerationTrims->raw[Z];
}

void accUpdate(timeUs_t currentTimeUs, rollAndPitchTrims_t *rollAndPitchTrims)
{
    UNUSED(currentTimeUs);

    if (!acc.dev.readFn(&acc.dev)) {
        return;
    }
    acc.isAccelUpdatedAtLeastOnce = true;

    for (int axis = 0; axis < XYZ_AXIS_COUNT; axis++) {
        DEBUG_SET(DEBUG_ACCELEROMETER, axis, acc.dev.ADCRaw[axis]);
        acc.accADC[axis] = acc.dev.ADCRaw[axis];
    }

    if (accLpfCutHz) {
        for (int axis = 0; axis < XYZ_AXIS_COUNT; axis++) {
            acc.accADC[axis] = lrintf(biquadFilterApply(&accFilter[axis], (float)acc.accADC[axis]));
        }
    }

    alignSensors(acc.accADC, acc.dev.accAlign);

    if (!accIsCalibrationComplete()) {
        performAcclerationCalibration(rollAndPitchTrims);
    }

    if (feature(FEATURE_INFLIGHT_ACC_CAL)) {
        performInflightAccelerationCalibration(rollAndPitchTrims);
    }

    applyAccelerationTrims(accelerationTrims);

    ++accumulatedMeasurementCount;
    for (int axis = 0; axis < XYZ_AXIS_COUNT; axis++) {
        accumulatedMeasurements[axis] += acc.accADC[axis];
    }
}
bool accGetAccumulationAverage(float *accumulationAverage)
{
    if (accumulatedMeasurementCount > 0) {
        // If we have gyro data accumulated, calculate average rate that will yield the same rotation
        for (int axis = 0; axis < XYZ_AXIS_COUNT; axis++) {
            accumulationAverage[axis] = accumulatedMeasurements[axis] / accumulatedMeasurementCount;
            accumulatedMeasurements[axis] = 0.0f;
        }
        accumulatedMeasurementCount = 0;
        return true;
    } else {
        for (int axis = 0; axis < XYZ_AXIS_COUNT; axis++) {
            accumulationAverage[axis] = 0.0f;
        }
        return false;
    }
}

void setAccelerationTrims(flightDynamicsTrims_t *accelerationTrimsToUse)
{
    accelerationTrims = accelerationTrimsToUse;
}

void accInitFilters(void)
{
    accLpfCutHz = accelerometerConfig()->acc_lpf_hz;
    if (acc.accSamplingInterval) {
        for (int axis = 0; axis < XYZ_AXIS_COUNT; axis++) {
            biquadFilterInitLPF(&accFilter[axis], accLpfCutHz, acc.accSamplingInterval);
        }
    }
}

