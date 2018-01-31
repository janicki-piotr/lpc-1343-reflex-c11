#include "mcu_regs.h"
#include "funkcje_timer.h"

/******************************************************************************
** Descriptions:		Initialize timer 0, set timer interval, reset timer,
**						install timer interrupt handler
**
** parameters:			timer interval
******************************************************************************/

void timer0_32_init(uint32_t TimerInterval)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9); /* Enables clock for 32-bit counter/timer 0. */
    LPC_IOCON->PIO1_5 |= 0x02;	/* Timer0_32 CAP0 */
    LPC_IOCON->PIO1_6 |= 0x02;	/* Timer0_32 MAT0 */
    LPC_IOCON->PIO1_7 |= 0x02;	/* Timer0_32 MAT1 */
    LPC_TMR32B0->MR0 = TimerInterval; /* Match Register 0 (MR0). MR0 can be enabled through the MCR to reset the TC, stop both the TC and PC, and/or generate an interrupt every time MR0 matches the TC. */
    LPC_TMR32B0->MCR = 3;			/* Interrupt and Reset on MR0 */

  return;
}

/*****************************************************************************
** Function name:		delay32Ms_timer0
**
** Descriptions:		Start the timer0 delay in milliseconds
**						until elapsed
**
** Parameters:			Delay value in millisecond
*****************************************************************************/
void delay32Ms_timer0(uint32_t delayInMs)
{
    /* setup timer #0 for delay */
    LPC_TMR32B0->TCR = 0x02;		/* reset timer */
    LPC_TMR32B0->PR  = 0x00;		/* set prescaler to zero */
    LPC_TMR32B0->MR0 = delayInMs * ((SystemFrequency/LPC_SYSCON->SYSAHBCLKDIV) / 1000);
    LPC_TMR32B0->IR  = 0xff;		/* reset all interrupts */
    LPC_TMR32B0->MCR = 0x04;		/* stop timer on match */
    LPC_TMR32B0->TCR = 0x01;		/* start timer */

    while (LPC_TMR32B0->TCR & 0x01); /* wait until delay time has elapsed */

  return;
}

/*****************************************************************************
** Function name:		delay32Us_timer0
**
** Descriptions:		Start the timer0 delay in microseconds
**						until elapsed
**
** Parameters:			Delay value in microseconds
*****************************************************************************/
void delay32Us_timer0(uint32_t delayInUs)
{
    /* setup timer #0 for delay */
    LPC_TMR32B0->TCR = 0x02;        /* reset timer */
    LPC_TMR32B0->PR  = 0x00;        /* set prescaler to zero */
    LPC_TMR32B0->MR0 = delayInUs * ((SystemFrequency/LPC_SYSCON->SYSAHBCLKDIV) / 1000000);
    LPC_TMR32B0->IR  = 0xff;        /* reset all interrupts */
    LPC_TMR32B0->MCR = 0x04;        /* stop timer on match */
    LPC_TMR32B0->TCR = 0x01;        /* start timer */

    while (LPC_TMR32B0->TCR & 0x01); /* wait until delay time has elapsed */

  return;
}
