/*
 * funkcje_ratatuj.h
 *
 *  Created on: 21 paź 2017
 *      Author: pcpio
 */

#ifndef INC_FUNKCJE_RATATUJ_H_
#define INC_FUNKCJE_RATATUJ_H_

#define ROTATE_WAIT  0
#define ROTATE_RIGHT 1
#define ROTATE_LEFT  2


void rotate_init (void);
uint8_t rotate_read(void);


#define JOYSTICK_CENTER 0x01
#define JOYSTICK_UP     0x02
#define JOYSTICK_DOWN   0x04
#define JOYSTICK_LEFT   0x08
#define JOYSTICK_RIGHT  0x10


void joystick_init (void);
uint8_t joystick_read(void);


#endif /* INC_FUNKCJE_RATATUJ_H_ */
