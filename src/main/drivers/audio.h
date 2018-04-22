/*
 * audio.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_AUDIO_H_
#define SRC_MAIN_DRIVERS_AUDIO_H_

void audioSetupIO(void);
void audioGenerateWhiteNoise(void);

#define TONE_COUNT 64
#define TONE_MIN 0
#define TONE_MAX (TONE_COUNT - 1)
#define TONE_MID ((TONE_COUNT / 2) + TONE_MIN)
void audioPlayTone(uint8_t tone); // TONE_MIN to TONE_MAX

void audioSilence(void);



#endif /* SRC_MAIN_DRIVERS_AUDIO_H_ */