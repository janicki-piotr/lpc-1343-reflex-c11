/*
 * funkcje_GPIO.h
 *
 *  Created on: 21 paź 2017
 *      Author: pcpio
 */

#ifndef FUNKCJE_GPIO_H_
#define FUNKCJE_GPIO_H_

#define PORT0		0
#define PORT1		1
#define PORT2		2
#define PORT3		3

void GPIO_init( void );

void GPIO_SetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir );
void GPIO_SetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal );
uint8_t GPIO_GetValue( uint32_t portNum, uint32_t bitPosi);

#endif /* FUNKCJE_GPIO_H_ */
