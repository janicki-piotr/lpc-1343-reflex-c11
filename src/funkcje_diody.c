#include "mcu_regs.h"
#include "type.h"
#include "funkcje_GPIO.h"
#include "funkcje_diody.h"

/******************************************************************************
 *
 * Description:
 *    Initialize RGB driver
 *
 *****************************************************************************/
void rgb_init (void)
{
    GPIO_SetDir( PORT1, 9, 1 );  //R
    GPIO_SetDir( PORT1, 2, 1 );  //G
    GPIO_SetDir( PORT1, 10, 1 ); //B

    LPC_IOCON->JTAG_nTRST_PIO1_2 = (LPC_IOCON->JTAG_nTRST_PIO1_2) | 0x1;
}

/******************************************************************************
 *
 * Description:
 *    set RGB diod color to B G or R
 *****************************************************************************/
void rgb_diodaB (void)
{

   GPIO_SetValue( PORT1, 9, 0 );
   GPIO_SetValue( PORT1, 2, 1 );
   GPIO_SetValue( PORT1, 10, 1 );

}

void rgb_diodaG (void)
{

   GPIO_SetValue( PORT1, 9, 0);
   GPIO_SetValue( PORT1, 2, 0);
   GPIO_SetValue( PORT1, 10, 1 );

}

void rgb_diodaR (void)
{

   GPIO_SetValue( PORT1, 9, 1 );
   GPIO_SetValue( PORT1, 2, 0 );
   GPIO_SetValue( PORT1, 10, 1 );

}
