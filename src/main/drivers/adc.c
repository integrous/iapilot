/*
 * adc.c
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#include <stdbool.h>
#include <stdint.h>

#include "platform.h"
#include "common/utils.h"

#ifdef USE_ADC

#include "build/build_config.h"
#include "build/debug.h"

#include "drivers/adc_impl.h"
#include "drivers/io.h"

#include "pg/adc.h"

#include "adc.h"

//#define DEBUG_ADC_CHANNELS

adcOperatingConfig_t adcOperatingConfig[ADC_CHANNEL_COUNT];
volatile uint16_t adcValues[ADC_CHANNEL_COUNT];

#ifdef USE_ADC_INTERNAL
uint16_t adcTSCAL1;
uint16_t adcTSCAL2;
uint16_t adcTSSlopeK;
uint16_t adcVREFINTCAL;
#endif

uint8_t adcChannelByTag(ioTag_t ioTag)
{
    for (uint8_t i = 0; i < ARRAYLEN(adcTagMap); i++) {
        if (ioTag == adcTagMap[i].tag)
            return adcTagMap[i].channel;
    }
    return 0;
}

ADCDevice adcDeviceByInstance(ADC_TypeDef *instance)
{
    if (instance == ADC1) {
        return ADCDEV_1;
    }

#if defined(STM32F3) || defined(STM32F4) || defined(STM32F7)
    if (instance == ADC2) {
        return ADCDEV_2;
    }

    if (instance == ADC3) {
        return ADCDEV_3;
    }
#endif

#ifdef STM32F3
    if (instance == ADC4) {
        return ADCDEV_4;
    }
#endif

    return ADCINVALID;
}

uint16_t adcGetChannel(uint8_t channel)
{
#ifdef DEBUG_ADC_CHANNELS
    if (adcOperatingConfig[0].enabled) {
        debug[0] = adcValues[adcOperatingConfig[0].dmaIndex];
    }
    if (adcOperatingConfig[1].enabled) {
        debug[1] = adcValues[adcOperatingConfig[1].dmaIndex];
    }
    if (adcOperatingConfig[2].enabled) {
        debug[2] = adcValues[adcOperatingConfig[2].dmaIndex];
    }
    if (adcOperatingConfig[3].enabled) {
        debug[3] = adcValues[adcOperatingConfig[3].dmaIndex];
    }
#endif
    return adcValues[adcOperatingConfig[channel].dmaIndex];
}

// Verify a pin designated by tag has connection to an ADC instance designated by device

bool adcVerifyPin(ioTag_t tag, ADCDevice device)
{
    if (!tag) {
        return false;
    }

    for (int map = 0 ; map < ADC_TAG_MAP_COUNT ; map++) {
#if defined(STM32F1)
        UNUSED(device);
        if ((adcTagMap[map].tag == tag)) {
            return true;
        }
#else
        if ((adcTagMap[map].tag == tag) && (adcTagMap[map].devices & (1 << device))) {
            return true;
        }
#endif
    }

    return false;
}

#else
uint16_t adcGetChannel(uint8_t channel)
{
    UNUSED(channel);
    return 0;
}
#endif
