#include <funkcje_gpio.h>
#include "funkcje_przelaczniki.h"
#include "mcu_regs.h"
#include "type.h"

#define ROTATE_READ_STATE() ( (LPC_GPIO1->DATA) & 0x03)

/* ROTARY SWITCH DIRECTIONS */
#define R_W  0
#define R_L1 1
#define R_L2 2
#define R_L3 3
#define R_R1 4
#define R_R2 5
#define R_R3 6


/*****************************************************************************
** Function name:		rotate_init
**
** Descriptions:		Initialize Rotary switch driver
**
** Parameters:			None
*****************************************************************************/
void rotate_init (void)
{
    GPIO_SetDir( PORT1, 0, 0 );
    GPIO_SetDir( PORT1, 1, 0 );
}

/*****************************************************************************
** Function name:		rotate_read
**
** Descriptions:		Returns  rotary switch state
**
** Parameters:			None
*****************************************************************************/
uint8_t rotate_read (void)
{
    uint8_t state = ROTATE_READ_STATE();
    uint8_t event = ROTATE_WAIT;
    uint8_t e = R_W;

    if (state == 0x03)
    {
        return (ROTATE_WAIT);
    }

    while (state != 0x03)
    {

        switch (e)
        {

        case R_W:
            if (state == 0x02)
            {
                e = R_R1;
            }
            else if (state == 0x01)
            {
                e = R_L1;
            }
            break;

        case R_L1:
            if (state == 0x00)
            {
                e = R_R2;
            }
            break;

        case R_L2:
            if (state == 0x01)
            {
                e = R_R3;
                event = ROTATE_RIGHT;
            }
            break;

        case R_R1:
            if (state == 0x00)
            {
                e = R_L2;
            }
            break;

        case R_R2:
            if (state == 0x02)
            {
                e = R_L3;
                event = ROTATE_LEFT;
            }
            break;
        }

        state = ROTATE_READ_STATE();
    }

    return (event);
}

/*****************************************************************************
** Function name:		buttons_init
**
** Descriptions:		Buttons initialization
**
** Parameters:			None
*****************************************************************************/
void buttons_init (void)
{
    /* set the GPIOs as inputs */
    GPIO_SetDir( PORT1, 4, 0 );
    GPIO_SetDir( PORT0, 1, 0 );

}
