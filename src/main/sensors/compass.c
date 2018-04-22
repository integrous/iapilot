/*
 * compass.c
 *
 *  Created on: 2018. 4. 19.
 *      Author: kangju
 */

#include <stdbool.h>
#include <stdint.h>

#include "platform.h"

#include "common/axis.h"

#include "pg/pg.h"
#include "pg/pg_ids.h"

#include "drivers/bus_i2c.h"
#include "drivers/bus_spi.h"
#include "drivers/bus.h"
#include "drivers/accgyro/accgyro_mpu.h"
#include "drivers/compass/compass.h"
#include "drivers/compass/compass_fake.h"
#include "drivers/compass/compass_hmc5883l.h"
#include "drivers/io.h"
#include "drivers/light_led.h"

#include "fc/config.h"
#include "fc/runtime_config.h"

#include "sensors/boardalignment.h"
#include "sensors/compass.h"
#include "sensors/gyro.h"
#include "sensors/sensors.h"

#ifdef USE_HARDWARE_REVISION_DETECTION
#include "hardware_revision.h"
#endif

magDev_t magDev;
mag_t mag;                   // mag access functions

#ifdef MAG_INT_EXTI
#define COMPASS_INTERRUPT_TAG   IO_TAG(MAG_INT_EXTI)
#else
#define COMPASS_INTERRUPT_TAG   IO_TAG_NONE
#endif

PG_REGISTER_WITH_RESET_FN(compassConfig_t, compassConfig, PG_COMPASS_CONFIG, 1);

void pgResetFn_compassConfig(compassConfig_t *compassConfig)
{
    compassConfig->mag_align = ALIGN_DEFAULT;
    compassConfig->mag_declination = 0;
    compassConfig->mag_hardware = MAG_DEFAULT;

// Generate a reasonable default for backward compatibility
// Strategy is
// 1. If SPI device is defined, it will take precedence, assuming it's onboard.
// 2. I2C devices are will be handled by address = 0 (per device default).
// 3. Slave I2C device on SPI gyro

if !(defined(USE_GYRO_SPI_MPU6500))
    compassConfig->mag_bustype = BUSTYPE_I2C;
    compassConfig->mag_i2c_device = I2C_DEV_TO_CFG(MAG_I2C_INSTANCE);
    compassConfig->mag_i2c_address = 0;
    compassConfig->mag_spi_device = SPI_DEV_TO_CFG(SPIINVALID);
    compassConfig->mag_spi_csn = IO_TAG_NONE;
#elif (defined(USE_GYRO_SPI_MPU6500))
    compassConfig->mag_bustype = BUSTYPE_MPU_SLAVE;
    compassConfig->mag_i2c_device = I2C_DEV_TO_CFG(I2CINVALID);
    compassConfig->mag_i2c_address = 0;
    compassConfig->mag_spi_device = SPI_DEV_TO_CFG(SPIINVALID);
    compassConfig->mag_spi_csn = IO_TAG_NONE;
#else
    compassConfig->mag_hardware = MAG_NONE;
    compassConfig->mag_bustype = BUSTYPE_NONE;
    compassConfig->mag_i2c_device = I2C_DEV_TO_CFG(I2CINVALID);
    compassConfig->mag_i2c_address = 0;
    compassConfig->mag_spi_device = SPI_DEV_TO_CFG(SPIINVALID);
    compassConfig->mag_spi_csn = IO_TAG_NONE;
#endif
    compassConfig->interruptTag = COMPASS_INTERRUPT_TAG;
}

#if defined(USE_MAG)

static int16_t magADCRaw[XYZ_AXIS_COUNT];
static uint8_t magInit = 0;

#if !defined(SITL)

bool compassDetect(magDev_t *dev)
{
    magSensor_e magHardware = MAG_NONE;

    busDevice_t *busdev = &dev->busdev;

#ifdef USE_MAG_DATA_READY_SIGNAL
    dev->magIntExtiTag = compassConfig()->interruptTag;
#endif

    switch (compassConfig()->mag_bustype) {
#ifdef USE_I2C
    case BUSTYPE_I2C:
        busdev->bustype = BUSTYPE_I2C;
        busdev->busdev_u.i2c.device = I2C_CFG_TO_DEV(compassConfig()->mag_i2c_device);
        busdev->busdev_u.i2c.address = compassConfig()->mag_i2c_address;
#endif
        break;



#if defined(USE_GYRO_SPI_MPU6500)
    case BUSTYPE_MPU_SLAVE:
        {
            if (gyroMpuDetectionResult()->sensor == MPU_9250_SPI) {
                busdev->bustype = BUSTYPE_MPU_SLAVE;
                busdev->busdev_u.mpuSlave.master = gyroSensorBus();
                busdev->busdev_u.mpuSlave.address = compassConfig()->mag_i2c_address;
            } else {
                return false;
            }
        }
#endif
        break;

    default:
        return false;
    }

    dev->magAlign = ALIGN_DEFAULT;

    switch (compassConfig()->mag_hardware) {
    case MAG_DEFAULT:
        FALLTHROUGH;


    case MAG_NONE:
        magHardware = MAG_NONE;
        break;
    }

    if (magHardware == MAG_NONE) {
        return false;
    }

    detectedSensors[SENSOR_INDEX_MAG] = magHardware;
    sensorsSet(SENSOR_MAG);
    return true;
}
#else
bool compassDetect(magDev_t *dev)
{
    UNUSED(dev);

    return false;
}
#endif // !SITL

bool compassInit(void)
{
    // initialize and calibration. turn on led during mag calibration (calibration routine blinks it)
    // calculate magnetic declination
    mag.magneticDeclination = 0.0f; // TODO investigate if this is actually needed if there is no mag sensor or if the value stored in the config should be used.

    if (!compassDetect(&magDev)) {
        return false;
    }

    const int16_t deg = compassConfig()->mag_declination / 100;
    const int16_t min = compassConfig()->mag_declination % 100;
    mag.magneticDeclination = (deg + ((float)min * (1.0f / 60.0f))) * 10; // heading is in 0.1deg units
    LED1_ON;
    magDev.init(&magDev);
    LED1_OFF;
    magInit = 1;
    if (compassConfig()->mag_align != ALIGN_DEFAULT) {
        magDev.magAlign = compassConfig()->mag_align;
    }
    return true;
}

bool compassIsHealthy(void)
{
    return (mag.magADC[X] != 0) && (mag.magADC[Y] != 0) && (mag.magADC[Z] != 0);
}

void compassUpdate(timeUs_t currentTimeUs)
{
    static timeUs_t tCal = 0;
    static flightDynamicsTrims_t magZeroTempMin;
    static flightDynamicsTrims_t magZeroTempMax;

    magDev.read(&magDev, magADCRaw);
    for (int axis = 0; axis < XYZ_AXIS_COUNT; axis++) {
        mag.magADC[axis] = magADCRaw[axis];
    }
    alignSensors(mag.magADC, magDev.magAlign);

    flightDynamicsTrims_t *magZero = &compassConfigMutable()->magZero;
    if (STATE(CALIBRATE_MAG)) {
        tCal = currentTimeUs;
        for (int axis = 0; axis < 3; axis++) {
            magZero->raw[axis] = 0;
            magZeroTempMin.raw[axis] = mag.magADC[axis];
            magZeroTempMax.raw[axis] = mag.magADC[axis];
        }
        DISABLE_STATE(CALIBRATE_MAG);
    }

    if (magInit) {              // we apply offset only once mag calibration is done
        mag.magADC[X] -= magZero->raw[X];
        mag.magADC[Y] -= magZero->raw[Y];
        mag.magADC[Z] -= magZero->raw[Z];
    }

    if (tCal != 0) {
        if ((currentTimeUs - tCal) < 30000000) {    // 30s: you have 30s to turn the multi in all directions
            LED0_TOGGLE;
            for (int axis = 0; axis < 3; axis++) {
                if (mag.magADC[axis] < magZeroTempMin.raw[axis])
                    magZeroTempMin.raw[axis] = mag.magADC[axis];
                if (mag.magADC[axis] > magZeroTempMax.raw[axis])
                    magZeroTempMax.raw[axis] = mag.magADC[axis];
            }
        } else {
            tCal = 0;
            for (int axis = 0; axis < 3; axis++) {
                magZero->raw[axis] = (magZeroTempMin.raw[axis] + magZeroTempMax.raw[axis]) / 2; // Calculate offsets
            }

            saveConfigAndNotify();
        }
    }
}
#endif // USE_MAG
