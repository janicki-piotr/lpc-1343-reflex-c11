#include "mcu_regs.h"
#include "type.h"
#include "funkcje_GPIO.h"
#include "funkcje_SSP.h"
#include "funkcje_7segled.h"

#define LED7_CS_OFF() GPIO_SetValue( PORT1, 11, 1 )
#define LED7_CS_ON()  GPIO_SetValue( PORT1, 11, 0 )

static uint8_t chars[] = {
        /* '-', '.' */
        0xFB, 0xDF, 0xFF,
        /* digits 0 - 9 */
        0x24, 0xAF, 0xE0, 0xA2, 0x2B, 0x32, 0x30, 0xA7, 0x20, 0x22,
        /* ':' to '@' are invalid */
        0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
        /* A to J */
        0x21, 0x38, 0x74, 0xA8, 0x70, 0x71, 0x10, 0x29, 0x8F, 0xAC,
        /* K to T */
        0xFF, 0x7C,  0xFF, 0xB9, 0x04, 0x61, 0x03, 0xF9, 0x12, 0x78,
        /* U to Z */
        0x2C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /* '[' to '`' */
        0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF,
        /* a to j */
        0x21, 0x38, 0xF8, 0xA8, 0x70, 0x71, 0x02, 0x39, 0x8F, 0xAC,
        /* k to t */
        0xFF, 0x7C,  0xFF, 0xB9, 0xB8, 0x61, 0x03, 0xF9, 0x12, 0x78,
        /* u to z */
        0xBC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /* { to '|' */
        0xFF, 0x7D,
};

/******************************************************************************
 *
 * Description:
 *    7 segment display initialization
 *
 *****************************************************************************/
void led7_init (void)
{
    GPIO_SetDir( PORT1, 11, 1 );
    LED7_CS_OFF();
}

/******************************************************************************
 *
 * Description:
 *    Draw a character on the 7 segment display
 *
 * Params:
 *   [in] ch - character interpreted as an ascii character. Not all ascii
 *             characters can be realized on the display. If a character
 *             can't be realized all segments are off.
 *****************************************************************************/
void led7_setChar(uint8_t ch)
{
    uint8_t val = 0xff;
    LED7_CS_ON();

    if (ch >= '-' && ch <= '|') {
        val = chars[ch-'-'];
    }

    SSP_Send( (uint8_t *)&val, 1 );

    LED7_CS_OFF();
}
