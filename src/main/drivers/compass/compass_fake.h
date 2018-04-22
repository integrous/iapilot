/*
 * compass_fake.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_COMPASS_COMPASS_FAKE_H_
#define SRC_MAIN_DRIVERS_COMPASS_COMPASS_FAKE_H_

#pragma once

struct magDev_s;
bool fakeMagDetect(struct magDev_s *mag);
void fakeMagSet(int16_t x, int16_t y, int16_t z);



#endif /* SRC_MAIN_DRIVERS_COMPASS_COMPASS_FAKE_H_ */
