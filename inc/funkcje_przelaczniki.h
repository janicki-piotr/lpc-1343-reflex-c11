#ifndef INC_FUNKCJE_PRZELACZNIKI_H_
#define INC_FUNKCJE_PRZELACZNIKI_H_

#include "type.h"

/* ROTARY SWITCH */
#define ROTATE_WAIT  0x00
#define ROTATE_RIGHT 0x01
#define ROTATE_LEFT  0x02

void rotate_init (void);
uint8_t rotate_read (void);


void buttons_init (void);


#endif /* INC_FUNKCJE_PRZELACZNIKI_H_ */
