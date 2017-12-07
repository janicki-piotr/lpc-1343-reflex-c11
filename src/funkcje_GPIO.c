#include "mcu_regs.h"
#include "funkcje_GPIO.h"

/* Shadow registers used to prevent chance of read-modify-write errors */
static uint32_t GPIOShadowPort0;
static uint32_t GPIOShadowPort1;
static uint32_t GPIOShadowPort2;
static uint32_t GPIOShadowPort3;

/*****************************************************************************
** Descriptions:		Initialize GPIO, install the
**						GPIO interrupt handler
*****************************************************************************/
void GPIO_init( void )
{
  /* Enable AHB clock to the GPIO domain.
   * Enable GPIO Clock (powers the GPIO peripheral*/


  //The SYSAHBCLKCTRL register enables the clocks to individual system and peripheral blocks.
  //SYSAHBCLKCTRL   /* Offset 0x80 */;  wskaźnik na blok System Control ma nazwę LPC_SYSCON
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6); //6 - GPIO - Enables clock for GPIO port registers and GPIO pin interrupt registers.


  /* Set up NVIC when I/O pins are configured as external interrupts. */
  NVIC_EnableIRQ(EINT0_IRQn);
  NVIC_EnableIRQ(EINT1_IRQn);
  NVIC_EnableIRQ(EINT2_IRQn);
  NVIC_EnableIRQ(EINT3_IRQn);


  /* It gives the current state of the GPIO pin when read and saves to GPIOShadowPortn */
  GPIOShadowPort0 = LPC_GPIO0->DATA;
  GPIOShadowPort1 = LPC_GPIO1->DATA;
  GPIOShadowPort2 = LPC_GPIO2->DATA;
  GPIOShadowPort3 = LPC_GPIO3->DATA;
  return;
}

/*****************************************************************************
** Function name:		GPIOSetDir
**
** Descriptions:		Set the direction in GPIO port
**
** parameters:			port num, bit position, direction (1 out, 0 input)
** Returned value:		None
**
*****************************************************************************/
void GPIO_SetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir )
{
  /* if DIR is OUT(1), but GPIOx_DIR is not set, set DIR
  to OUT(1); if DIR is IN(0), but GPIOx_DIR is set, clr
  DIR to IN(0). */
  switch ( portNum )
  {
	case PORT0:
	  if (dir)
		LPC_GPIO0->DIR |= (1 << bitPosi); //Set bitPosi on GPIO0 to OUT  |-OR
	  else
		LPC_GPIO0->DIR &= ~(1 << bitPosi);//Set bitPosi on GPIO0 to IN	 &-AND
	break;

 	case PORT1:
	  if (dir)
		LPC_GPIO1->DIR |= (1 << bitPosi);
	  else
		LPC_GPIO1->DIR &= ~(1 << bitPosi);
	break;

	case PORT2:
	  if (dir)
		LPC_GPIO2->DIR |= (1 << bitPosi);
	  else
		LPC_GPIO2->DIR &= ~(1 << bitPosi);
	break;

	case PORT3:
	  if (dir)
		LPC_GPIO3->DIR |= (1 << bitPosi);
	  else
		LPC_GPIO3->DIR &= ~(1 << bitPosi);
	break;
	default:
	  break;

  }
  return;
}

/*****************************************************************************
** Function name:		GPIOSetValue
**
** Descriptions:		Set/clear a bit value in a specific bit position
**						in GPIO portX(X is the port number.) Because of the
**						implementation of port shadowing to prevent GPIO errors
**						under external cap load it is important to only use
**						this function for GPIO or to remove it and use your own
**						GPIO routines.
*****************************************************************************/
void GPIO_SetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal )
{

  switch ( portNum )
  {
	case PORT0:
		 if(bitVal)
			 GPIOShadowPort0 |= (1 << bitPosi);
		 else
			 GPIOShadowPort0 &= ~(1 << bitPosi);

		 LPC_GPIO0->DATA = GPIOShadowPort0;
	break;

 	case PORT1:
		 if(bitVal)
			 GPIOShadowPort1 |= (1 << bitPosi);
		 else
			 GPIOShadowPort1 &= ~(1 << bitPosi);

		 LPC_GPIO1->DATA = GPIOShadowPort1;
	break;

	case PORT2:
		 if(bitVal)
			 GPIOShadowPort2 |= (1 << bitPosi);
		 else
			 GPIOShadowPort2 &= ~(1 << bitPosi);

		 LPC_GPIO2->DATA = GPIOShadowPort2;
	break;

	case PORT3:
		 if(bitVal)
			 GPIOShadowPort3 |= (1 << bitPosi);
		 else
			 GPIOShadowPort3 &= ~(1 << bitPosi);

		 LPC_GPIO3->DATA = GPIOShadowPort3;
	break;

	default:
	  break;
  }
  return;
}

/*****************************************************************************
** Function name:		GPIO_GetValue
**
** Descriptions:
**
**
**
*****************************************************************************/
uint8_t GPIO_GetValue( uint32_t portNum, uint32_t bitPosi)
{
  uint8_t ret = 0;

  switch ( portNum )
  {
    case PORT0:

        ret = ( (LPC_GPIO0->DATA & (1 << bitPosi) ) != 0);
    break;
    case PORT1:
        ret = ( (LPC_GPIO1->DATA & (1 << bitPosi) ) != 0);
    break;
    case PORT2:
        ret = ( (LPC_GPIO2->DATA & (1 << bitPosi) ) != 0);
    break;
    case PORT3:
        ret = ( (LPC_GPIO3->DATA & (1 << bitPosi) ) != 0);
    break;
    default:
      break;
  }
  return ret;
}
