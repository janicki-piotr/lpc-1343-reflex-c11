#include <funkcje_uart.h>
#include "mcu_regs.h"

#define SystemFrequency SystemCoreClock


/*****************************************************************************
 ** Function name:		UARTInit
 **
 ** Descriptions:		Initialize UART0 port, setup pin select,
 **				clock, parity, stop bits, FIFO, etc.
 **
 ** parameters:			UART baudrate
 **
 **
 *****************************************************************************/
void UARTInit(uint32_t baudrate)
{
    uint32_t Fdiv;
    uint32_t regVal;

    LPC_IOCON->PIO1_6 &= ~0x07;    /*  UART I/O config */
    LPC_IOCON->PIO1_6 |= 0x01;     /* UART RXD */
    LPC_IOCON->PIO1_7 &= ~0x07;
    LPC_IOCON->PIO1_7 |= 0x01;     /* UART TXD */
    /* Enable UART clock */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);
    LPC_SYSCON->UARTCLKDIV = 0x1;     /* divided by 1 */

    LPC_UART->LCR = 0x83;             /* 8 bits, no Parity, 1 Stop bit */
    regVal = LPC_SYSCON->UARTCLKDIV;
    Fdiv = (((SystemFrequency/LPC_SYSCON->SYSAHBCLKDIV)/regVal)/16)/baudrate ;	/*baud rate */

    LPC_UART->DLM = Fdiv / 256;
    LPC_UART->DLL = Fdiv % 256;
    LPC_UART->LCR = 0x03;		/* DLAB = 0 */
    LPC_UART->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

    /* Read to clear the line status. */
    regVal = LPC_UART->LSR;

    /* Ensure a clean start, no data in either TX or RX FIFO. */
    // CodeRed - added parentheses around comparison in operand of &
    while (( LPC_UART->LSR & (LSR_THRE|LSR_TEMT)) != (LSR_THRE|LSR_TEMT) );
    while ( LPC_UART->LSR & LSR_RDR )
    {
        regVal = LPC_UART->RBR;	/* Dump data from RX FIFO */
    }

    /* Enable the UART Interrupt */


    return;
}

/*****************************************************************************
 ** Function name:       UARTSendString
 **
 ** Descriptions:        Send a null-terminated string
 **
 ** parameters:      buffer pointer, and data length
 ** Returned value:  None
 **
 *****************************************************************************/
void UARTSendString(uint8_t *string)
{
    if (!string) {
        /* error */
        return;
    }

    while ( *string != '\0' )
    {
        /* THRE status, contain valid data */
        while ( !(LPC_UART->LSR & LSR_THRE) );
        LPC_UART->THR = *string;

        string++;
    }

    return;
}
