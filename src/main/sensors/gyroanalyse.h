/*
 * gyroanalyse.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_SENSORS_GYROANALYSE_H_
#define SRC_MAIN_SENSORS_GYROANALYSE_H_

#pragma once

#include "common/time.h"
#include "common/filter.h"

#define GYRO_FFT_BIN_COUNT      16 // FFT_WINDOW_SIZE / 2
typedef struct gyroFftData_s {
    float maxVal;
    uint16_t centerFreq;
} gyroFftData_t;

void gyroDataAnalyseInit(uint32_t targetLooptime);
const gyroFftData_t *gyroFftData(int axis);
struct gyroDev_s;
void gyroDataAnalyse(const struct gyroDev_s *gyroDev, biquadFilter_t *notchFilterDyn);
void gyroDataAnalyseUpdate(biquadFilter_t *notchFilterDyn);




#endif /* SRC_MAIN_SENSORS_GYROANALYSE_H_ */
