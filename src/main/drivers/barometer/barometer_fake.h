/*
 * barometer_fake.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_BAROMETER_BAROMETER_FAKE_H_
#define SRC_MAIN_DRIVERS_BAROMETER_BAROMETER_FAKE_H_

#pragma once

struct baroDev_s;
bool fakeBaroDetect(struct baroDev_s *baro);
void fakeBaroSet(int32_t pressure, int32_t temperature);



#endif /* SRC_MAIN_DRIVERS_BAROMETER_BAROMETER_FAKE_H_ */
