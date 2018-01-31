#include <funkcje_gpio.h>
#include "mcu_regs.h"
#include "type.h"
#include "funkcje_SSP.h"
#include "funkcje_7segled.h"

#define LED7_CS_OFF() GPIO_SetValue(PORT1, 11, 1)
#define LED7_CS_ON()  GPIO_SetValue(PORT1, 11, 0)

static uint8_t chars[] = {

		0x7D, 0xE0, 0x70, 0x39, 0x32,
};

/*****************************************************************************
** Function name:		led7_init
**
** Descriptions:		7 segment display initialization
**
** Parameters:			None
*****************************************************************************/
void led7_init(void)
{
    GPIO_SetDir(PORT1, 11, 1);
    LED7_CS_OFF();

    return;
}

/*****************************************************************************
** Function name:		led7_setChar
**
** Descriptions:		Draw a level number on the 7 segment display
**
** Parameters:			ch - character interpreted as an ascii character.
** 						Not all ascii characters can be realized on the display.
** 						If a character can't be realized all segments are off.
*****************************************************************************/
void led7_setChar(uint8_t ch)
{
    uint8_t val = 0xff;

    LED7_CS_ON();

    if (ch >= '1' && ch <= '5')
    {
        val = chars[ch - 49];
    }

    SSP_Send( (uint8_t *)&val, 1 );

    LED7_CS_OFF();

    return;
}
