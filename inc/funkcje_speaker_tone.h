/*
 * funkcje_speaker_tone.h
 *
 *  Created on: 08 lis 2017
 *      Author: pcpio
 */

#ifndef INC_FUNKCJE_SPEAKER_TONE_H_
#define INC_FUNKCJE_SPEAKER_TONE_H_

void speaker_init( void );

void playNote(uint32_t note, uint32_t durationMs);
uint32_t getNote(uint8_t ch);
uint32_t getDuration(uint8_t ch);
uint32_t getPause(uint8_t ch);
void playSong(uint8_t *song);





#endif /* INC_FUNKCJE_SPEAKER_TONE_H_ */
