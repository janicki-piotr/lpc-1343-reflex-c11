#include <funkcje_gpio.h>
#include "mcu_regs.h"
#include "type.h"
#include "funkcje_diody.h"

/*****************************************************************************
** Function name:		rgb_init
**
** Descriptions:		Initialize RGB driver
**
** Parameters:			None
*****************************************************************************/
void rgb_init (void)
{
    GPIO_SetDir( PORT1, 9, 1 );  /*R*/
    GPIO_SetDir( PORT1, 2, 1 );  /*B*/
    GPIO_SetDir( PORT1, 10, 1 ); /*G*/

    LPC_IOCON->JTAG_nTRST_PIO1_2 = (LPC_IOCON->JTAG_nTRST_PIO1_2) | 0x1;

    return;
}

/*****************************************************************************
** Functions name:		rgb_diodaB / rgb_diodaG / rgb_diodaR
**
** Descriptions:		set RGB diod color to Blue Green or Red
**
** Parameters:			None
*****************************************************************************/
void rgb_diodaB (void)
{
   GPIO_SetValue( PORT1, 9, 0 );
   GPIO_SetValue( PORT1, 2, 1 );
   GPIO_SetValue( PORT1, 10, 1 );

   return;
}

void rgb_diodaG (void)
{
   GPIO_SetValue( PORT1, 9, 0);
   GPIO_SetValue( PORT1, 2, 0);
   GPIO_SetValue( PORT1, 10, 1 );

   return;
}

void rgb_diodaR (void)
{
   GPIO_SetValue( PORT1, 9, 1 );
   GPIO_SetValue( PORT1, 2, 0 );
   GPIO_SetValue( PORT1, 10, 1 );

   return;
}
