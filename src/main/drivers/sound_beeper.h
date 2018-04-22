/*
 * sound_beeper.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_SOUND_BEEPER_H_
#define SRC_MAIN_DRIVERS_SOUND_BEEPER_H_

#pragma once

#ifdef BEEPER
#define BEEP_TOGGLE              systemBeepToggle()
#define BEEP_OFF                 systemBeep(false)
#define BEEP_ON                  systemBeep(true)
#else
#define BEEP_TOGGLE do {} while (0)
#define BEEP_OFF    do {} while (0)
#define BEEP_ON     do {} while (0)
#endif

void systemBeep(bool on);
void systemBeepToggle(void);
struct beeperDevConfig_s;
void beeperInit(const struct beeperDevConfig_s *beeperDevConfig);



#endif /* SRC_MAIN_DRIVERS_SOUND_BEEPER_H_ */
