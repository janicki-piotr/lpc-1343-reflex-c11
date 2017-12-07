/*
 * funkcje_ratatuj.c
 *
 *  Created on: 21 paź 2017
 *      Author: pcpio
 */

#include "mcu_regs.h"
#include "type.h"
#include "funkcje_GPIO.h"
#include "funkcje_ratatuj.h"

//definiujemy ROTARY_READ_STATE() odczytanie jako odczytanie stanu portu 1 z GPIO
#define ROTATE_READ_STATE() ( (LPC_GPIO1->DATA) & 0x03)

#define R_W  0
#define R_L1 1
#define R_L2 2
#define R_L3 3
#define R_R1 4
#define R_R2 5
#define R_R3 6

/******************************************************************************
 *
 * Description:
 *    Initialize Rotary switch driver
 *
 *****************************************************************************/
void rotate_init (void)
{
    GPIO_SetDir( PORT1, 0, 0 ); //PIO1_0
    GPIO_SetDir( PORT1, 1, 0 ); //PIO1_1
}

/******************************************************************************
 *
 * Description:
 *    Read rotary switch state
 *
 * Returns:
 *    rotary switch state
 *
 *****************************************************************************/
uint8_t rotate_read (void)
{
    uint8_t state = ROTATE_READ_STATE();
    uint8_t event = ROTATE_WAIT;
    uint8_t e = R_W;

    if (state == 0x03) {
        return ROTATE_WAIT;
    }

    while (state != 0x03) {

        switch (e) {
        case R_W:
            if (state == 0x02)
                e = R_R1;
            else if (state == 0x01)
                e = R_L1;
            break;

        case R_L1:
            if (state == 0x00)
                e = R_R2;
            break;
        case R_L2:
            if (state == 0x01) {
                e = R_R3;
                event = ROTATE_RIGHT;
            }
            break;
        case R_R1:
            if (state == 0x00)
                e = R_L2;
            break;
        case R_R2:
            if (state == 0x02) {
                e = R_L3;
                event = ROTATE_LEFT;
            }
            break;
        }

        state = ROTATE_READ_STATE();
    }

    return event;

}

void joystick_init (void)
{
    //GPIO_SetDir( PORT0, 1, 0 );

    /* set the GPIOs as inputs */
    GPIO_SetDir( PORT2, 0, 0 );
    GPIO_SetDir( PORT2, 1, 0 );
    GPIO_SetDir( PORT2, 2, 0 );
    GPIO_SetDir( PORT2, 3, 0 );
    GPIO_SetDir( PORT2, 4, 0 );
}

/******************************************************************************
 *
 * Description:
 *    Read the joystick status
 *
 * Returns:
 *   The joystick status. The returned value is a bit mask. More than one
 *   direction may be active at any given time (e.g. UP and RIGHT)
 *
 *****************************************************************************/
uint8_t joystick_read(void)
{
    uint8_t status = 0;

    if (!GPIO_GetValue( PORT2, 0)) {
        status |= JOYSTICK_CENTER;
    }

    if (!GPIO_GetValue( PORT2, 1)) {
        status |= JOYSTICK_DOWN;
    }

    if (!GPIO_GetValue( PORT2, 2)) {
        status |= JOYSTICK_RIGHT;
    }

    if (!GPIO_GetValue( PORT2, 3)) {
        status |= JOYSTICK_UP;
    }

    if (!GPIO_GetValue( PORT2, 4)) {
        status |= JOYSTICK_LEFT;
    }
    return status;
}
