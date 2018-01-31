#include <funkcje_gpio.h>
#include "mcu_regs.h"


/* Shadow registers used to prevent chance of read-modify-write errors */
static uint32_t GPIOShadowPort0;
static uint32_t GPIOShadowPort1;
static uint32_t GPIOShadowPort2;
static uint32_t GPIOShadowPort3;


/*****************************************************************************
** Function name:		GPIO_init
**
** Descriptions:		Initialize GPIO
**
** Parameters:			None
*****************************************************************************/
void GPIO_init (void)
{
  LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6); /*
  /* It gives the current state of the GPIO pin when read and saves to GPIOShadowPortn */
  GPIOShadowPort0 = LPC_GPIO0->DATA;
  GPIOShadowPort1 = LPC_GPIO1->DATA;
  GPIOShadowPort2 = LPC_GPIO2->DATA;
  GPIOShadowPort3 = LPC_GPIO3->DATA;
  return;
}

/*****************************************************************************
** Function name:		GPIO_SetDir
**
** Descriptions:		Set the direction in GPIO port
**
** Parameters:			port number, bit position, direction (1 out, 0 input)
*****************************************************************************/
void GPIO_SetDir(uint32_t portNum, uint32_t bitPosi, uint32_t dir)
{
  switch (portNum)
  {
    case PORT0:
      if (dir)
      {
          LPC_GPIO0->DIR |= (1 << bitPosi); /*Set bitPosi on GPIO0 to OUT*/
      }
      else
      {
          LPC_GPIO0->DIR &= ~(1 << bitPosi); /*Set bitPosi on GPIO0 to IN*/
      }
      break;

    case PORT1:
      if (dir)
      {
          LPC_GPIO1->DIR |= (1 << bitPosi);
      }
      else
      {
          LPC_GPIO1->DIR &= ~(1 << bitPosi);
      }
      break;

    case PORT2:
      if (dir)
      {
          LPC_GPIO2->DIR |= (1 << bitPosi);
      }
      else
      {
          LPC_GPIO2->DIR &= ~(1 << bitPosi);
      }
      break;

    case PORT3:
      if (dir)
      {
          LPC_GPIO3->DIR |= (1 << bitPosi);
      }
      else
      {
          LPC_GPIO3->DIR &= ~(1 << bitPosi);
      }
      break;

    default:
      break;
  }

  return;
}

/*****************************************************************************
** Function name:		GPIO_SetValue
**
** Descriptions:		Set/clear a bit value in a specific bit position
**						in GPIO portX.
**
** Parameters:			port number, bit position, bit value
*****************************************************************************/
void GPIO_SetValue(uint32_t portNum, uint32_t bitPosi, uint32_t bitVal)
{

  switch (portNum)
  {
    case PORT0:
      if (bitVal)
      {
          GPIOShadowPort0 |= (1 << bitPosi);
      }
      else
      {
         GPIOShadowPort0 &= ~(1 << bitPosi);
      }

      LPC_GPIO0->DATA = GPIOShadowPort0;
      break;

    case PORT1:
      if (bitVal)
      {
          GPIOShadowPort1 |= (1 << bitPosi);
      }
      else
      {
          GPIOShadowPort1 &= ~(1 << bitPosi);
      }

      LPC_GPIO1->DATA = GPIOShadowPort1;
      break;

    case PORT2:
      if(bitVal)
      {
          GPIOShadowPort2 |= (1 << bitPosi);
      }
      else
      {
          GPIOShadowPort2 &= ~(1 << bitPosi);
      }

      LPC_GPIO2->DATA = GPIOShadowPort2;
      break;

    case PORT3:
      if(bitVal)
      {
          GPIOShadowPort3 |= (1 << bitPosi);
      }
      else
      {
         GPIOShadowPort3 &= ~(1 << bitPosi);
      }

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
** Descriptions:		Returns a bit value.
**
** Parameters:			port number, bit position
*****************************************************************************/
uint8_t GPIO_GetValue(uint32_t portNum, uint32_t bitPosi)
{
  uint8_t ret = 0;

  switch (portNum)
  {
    case PORT0:
      ret = (( LPC_GPIO0->DATA & (1 << bitPosi) ) != 0);
      break;

    case PORT1:
      ret = (( LPC_GPIO1->DATA & (1 << bitPosi) ) != 0);
      break;

    case PORT2:
      ret = (( LPC_GPIO2->DATA & (1 << bitPosi) ) != 0);
      break;

    case PORT3:
      ret = (( LPC_GPIO3->DATA & (1 << bitPosi) ) != 0);
      break;

    default:
      break;
  }
  return (ret);
}
