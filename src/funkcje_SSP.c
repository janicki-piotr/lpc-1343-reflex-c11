#include <funkcje_gpio.h>
#include "mcu_regs.h"
#include "funkcje_SSP.h"
#include "funkcje_gpio.h"

/*****************************************************************************
** Function name:		SSP_Init
**
** Descriptions:		SSP port initialization routine
**
** Parameters:			None
*****************************************************************************/
void SSP_init( void )
{
  uint8_t i, Dummy=Dummy;

  LPC_SYSCON->PRESETCTRL |= (1<<0);		 /* SSP0_RST_N;  De-assert SSP0 reset */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<11);	 /* Enables clock for SSP */
  LPC_IOCON->PIO0_8 |= 0x01;			 /* SSP Selects function MISO0 */
  LPC_IOCON->PIO0_9 |= 0x01;			 /* SSP Selects function MOSI0 */

  LPC_IOCON->SCKLOC = 0x01;
  LPC_IOCON->PIO2_11 = 0x01;

  GPIO_SetDir( PORT0, 2, 1 );
  GPIO_SetValue( PORT0, 2, 1 );

  LPC_SSP->CR0 = 0x0707; /* ‭0111 0000 0111‬; 0111 - DSS 8-bit;  00 - FRF SPI; 0 - Clock Out Polarity */
  LPC_SSP->CPSR = 0x2;	 /* SSPCPSR clock prescale register, master mode, minimum divisor is 0x02 */

  for ( i = 0; i < FIFOSIZE; i++ )
  {
	Dummy = LPC_SSP->DR;		/* clear the RxFIFO */
  }


  /* Master mode */
  LPC_SSP->CR1 = SSPCR1_SSE;
  /* SSP Enable - The SSP controller will interact with other devices on the serial bus.
   * Software should write the appropriate control information to the other SSP registers
   * and interrupt controller registers, before setting this bit. */

  /* Set SSPINMS registers to enable interrupts
   *enable all error related interrupts */
  LPC_SSP->IMSC = SSPIMSC_RORIM | SSPIMSC_RTIM;
  return;
}

/*****************************************************************************
** Function name:		SSP_Send
**
** Descriptions:		Send a block of data to the SSP port, the
**						first parameter is the buffer pointer, the 2nd
**						parameter is the block length.
**
** Parameters:			buffer pointer, block length
*****************************************************************************/
void SSP_Send( uint8_t *buf, uint32_t Length )
{
  uint32_t i;
  uint8_t Dummy = Dummy;

  for ( i = 0; i < Length; i++ )
  {
    /* Move on only if NOT busy and TX FIFO not full. */
    while ( (LPC_SSP->SR & (SSPSR_TNF|SSPSR_BSY)) != SSPSR_TNF );
    {
        LPC_SSP->DR = *buf;
    }
    buf++;
    while ( (LPC_SSP->SR & (SSPSR_BSY|SSPSR_RNE)) != SSPSR_RNE );
    {
        /* Whenever a byte is written, MISO FIFO counter increments, Clear FIFO
        on MISO. Otherwise, when SSP0Receive() is called, previous data byte
        is left in the FIFO. */
        Dummy = LPC_SSP->DR;
    }

  }
  return;
}
