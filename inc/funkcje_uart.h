#ifndef __UART_H 
#define __UART_H

#include "type.h"

#define LSR_RDR		0x01
#define LSR_THRE	0x20
#define LSR_TEMT	0x40


void UART_init(uint32_t Baudrate);
void UARTSendString(uint8_t *string);

#endif /* end __UART_H */
