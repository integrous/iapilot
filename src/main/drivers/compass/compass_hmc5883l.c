/*
 * compass_hmc5883l.c
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#include <stdbool.h>
#include <stdint.h>

#include <math.h>

#include "platform.h"

#if defined(USE_MAG_HMC5883) || defined(USE_MAG_SPI_HMC5883)

#include "build/debug.h"

#include "common/axis.h"
#include "common/maths.h"

#include "drivers/bus.h"
#include "drivers/bus_i2c.h"
#include "drivers/bus_i2c_busdev.h"
#include "drivers/bus_spi.h"
#include "drivers/exti.h"
#include "drivers/io.h"
#include "drivers/light_led.h"
#include "drivers/nvic.h"
#include "drivers/sensor.h"
#include "drivers/time.h"

#include "compass.h"

#include "compass_hmc5883l.h"

//#define DEBUG_MAG_DATA_READY_INTERRUPT

#define HMC5883_MAG_I2C_ADDRESS     0x1E
#define HMC5883_DEVICE_ID           0x48

#define HMC58X3_REG_CONFA           0x00
#define HMC58X3_REG_CONFB           0x01
#define HMC58X3_REG_MODE            0x02
#define HMC58X3_REG_DATA            0x03
#define HMC58X3_REG_IDA             0x0A

#define HMC_CONFA_NORMAL            0x00
#define HMC_CONFA_POS_BIAS          0x01
#define HMC_CONFA_NEG_BIAS          0x02
#define HMC_CONFA_DOR_15HZ          0X10
#define HMC_CONFA_8_SAMLES          0X60
#define HMC_CONFB_GAIN_2_5GA        0X60
#define HMC_CONFB_GAIN_1_3GA        0X20
#define HMC_MODE_CONTINOUS          0X00
#define HMC_MODE_SINGLE             0X01

#define HMC58X3_X_SELF_TEST_GAUSS   (+1.16f)            // X axis level when bias current is applied.
#define HMC58X3_Y_SELF_TEST_GAUSS   (+1.16f)            // Y axis level when bias current is applied.
#define HMC58X3_Z_SELF_TEST_GAUSS   (+1.08f)            // Z axis level when bias current is applied.
#define SELF_TEST_LOW_LIMIT         (243.0f / 390.0f)   // Low limit when gain is 5.
#define SELF_TEST_HIGH_LIMIT        (575.0f / 390.0f)   // High limit when gain is 5.

#ifdef USE_MAG_DATA_READY_SIGNAL

static void hmc5883_extiHandler(extiCallbackRec_t* cb)
{
    UNUSED(cb);
#ifdef DEBUG_MAG_DATA_READY_INTERRUPT
    // Measure the delta between calls to the interrupt handler
    // currently should be around 65/66 milli seconds / 15hz output rate
    static uint32_t lastCalledAt = 0;
    static int32_t callDelta = 0;

    uint32_t now = millis();
    callDelta = now - lastCalledAt;

    //UNUSED(callDelta);
    debug[0] = callDelta;

    lastCalledAt = now;
#endif
}
#endif

static void hmc5883lConfigureDataReadyInterruptHandling(magDev_t* mag)
{
#ifdef USE_MAG_DATA_READY_SIGNAL
    if (mag->magIntExtiTag == IO_TAG_NONE) {
        return;
    }

    const IO_t magIntIO = IOGetByTag(mag->magIntExtiTag);

#ifdef ENSURE_MAG_DATA_READY_IS_HIGH
    uint8_t status = IORead(magIntIO);
    if (!status) {
        return;
    }
#endif

    IOInit(magIntIO, OWNER_COMPASS_EXTI, 0);
    IOConfigGPIO(magIntIO, IOCFG_IN_FLOATING);
    EXTIHandlerInit(&mag->exti, hmc5883_extiHandler);
    EXTIConfig(magIntIO, &mag->exti, NVIC_PRIO_MAG_INT_EXTI, EXTI_Trigger_Rising);
    EXTIEnable(magIntIO, true);
#else
    UNUSED(mag);
#endif
}



static bool hmc5883lRead(magDev_t *mag, int16_t *magData)
{
    uint8_t buf[6];

    busDevice_t *busdev = &mag->busdev;

    bool ack = busReadRegisterBuffer(busdev, HMC58X3_REG_DATA, buf, 6);

    if (!ack) {
        return false;
    }

    magData[X] = (int16_t)(buf[0] << 8 | buf[1]);
    magData[Z] = (int16_t)(buf[2] << 8 | buf[3]);
    magData[Y] = (int16_t)(buf[4] << 8 | buf[5]);

    return true;
}

static bool hmc5883lInit(magDev_t *mag)
{

    busDevice_t *busdev = &mag->busdev;


    // leave test mode
    busWriteRegister(busdev, HMC58X3_REG_CONFA, HMC_CONFA_8_SAMLES | HMC_CONFA_DOR_15HZ | HMC_CONFA_NORMAL);    // Configuration Register A  -- 0 11 100 00  num samples: 8 ; output rate: 15Hz ; normal measurement mode
    busWriteRegister(busdev, HMC58X3_REG_CONFB, HMC_CONFB_GAIN_1_3GA);                                          // Configuration Register B  -- 001 00000    configuration gain 1.3Ga
    busWriteRegister(busdev, HMC58X3_REG_MODE, HMC_MODE_CONTINOUS);                                             // Mode register             -- 000000 00    continuous Conversion Mode

    delay(100);

    hmc5883lConfigureDataReadyInterruptHandling(mag);
    return true;
}

bool hmc5883lDetect(magDev_t* mag)
{
    busDevice_t *busdev = &mag->busdev;

    uint8_t sig = 0;



#ifdef USE_MAG_HMC5883
    if (busdev->bustype == BUSTYPE_I2C && busdev->busdev_u.i2c.address == 0) {
        busdev->busdev_u.i2c.address = HMC5883_MAG_I2C_ADDRESS;
    }
#endif

    bool ack = busReadRegisterBuffer(&mag->busdev, HMC58X3_REG_IDA, &sig, 1);

    if (!ack || sig != HMC5883_DEVICE_ID) {
        return false;
    }

    mag->init = hmc5883lInit;
    mag->read = hmc5883lRead;

    return true;
}
#endif
