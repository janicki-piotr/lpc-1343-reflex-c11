/*
 * funkcje_diody.h
 *
 *  Created on: 22 paź 2017
 *      Author: piotrpc
 */

#ifndef INC_FUNKCJE_DIODY_H_
#define INC_FUNKCJE_DIODY_H_

#define RGB_RED   0x01
#define RGB_BLUE  0x02
#define RGB_GREEN 0x04


void rgb_init (void);
void rgb_setLeds (uint8_t ledMask);
void rgb_diodaR (void);
void rgb_diodaG (void);
void rgb_diodaB (void);


#endif /* INC_FUNKCJE_DIODY_H_ */
