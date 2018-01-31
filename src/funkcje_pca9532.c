/******************************************************************************
 * Includes
 *****************************************************************************/

#include "funkcje_pca9532.h"
#include "mcu_regs.h"
#include "type.h"
#include "funkcje_i2c.h"


#define LS_MODE_ON     0x01
static uint16_t ledStateShadow = 0;

/******************************************************************************
 * Local Functions
 *****************************************************************************/

static void setLsStates(uint16_t states, uint8_t* ls, uint8_t mode)
{
#define IS_LED_SET(bit, x) ( ( ((x) & (bit)) != 0 ) ? 1 : 0 )

    int i = 0;

    for (i = 0; i < 4; i++) {

        ls[i] |= ( (IS_LED_SET(LED4, states)*mode << 0)
                | (IS_LED_SET(LED5, states)*mode << 2)
                | (IS_LED_SET(LED6, states)*mode << 4)
                | (IS_LED_SET(LED7, states)*mode << 6) );

        states >>= 4;
    }
}

static void setLeds(void)
{
    uint8_t buf[5];
    uint8_t ls[4] = {0,0,0,0};
    uint16_t states = ledStateShadow;

    /* LEDs in On/Off state */
    setLsStates(states, ls, LS_MODE_ON);


    buf[0] = PCA9532_LS0 | PCA9532_AUTO_INC;
    buf[1] = ls[0];
    buf[2] = ls[1];
    buf[3] = ls[2];
    buf[4] = ls[3];
    I2CWrite(PCA9532_I2C_ADDR, buf, 5);
}

/******************************************************************************
 * Public Functions
 *****************************************************************************/

/******************************************************************************
 *
 * Description:
 *    Initialize the PCA9532 Device
 *
 *****************************************************************************/
void pca9532_init (void)
{
    /* nothing to initialize */
}


/******************************************************************************
 *
 * Description:
 *    Set LED states (on or off).
 *
 * Params:
 *    [in]  ledOnMask  - The LEDs that should be turned on. This mask has
 *                       priority over ledOffMask
 *    [in]  ledOffMask - The LEDs that should be turned off.
 *
 *****************************************************************************/
void pca9532_setLeds (uint16_t ledOnMask, uint16_t ledOffMask)
{
    /* turn off leds */
    ledStateShadow &= (~(ledOffMask) & 0xffff);

    /* ledOnMask has priority over ledOffMask */
    ledStateShadow |= ledOnMask;

    setLeds();
}
