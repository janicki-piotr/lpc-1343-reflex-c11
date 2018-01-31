#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "type.h"
#define PORT0		0
#define PORT1		1
#define PORT2		2
#define PORT3		3

void GPIO_init(void);
void GPIO_SetDir(uint32_t portNum, uint32_t bitPosi, uint32_t dir);
void GPIO_SetValue(uint32_t portNum, uint32_t bitPosi, uint32_t bitVal);
uint8_t GPIO_GetValue(uint32_t portNum, uint32_t bitPosi);

#endif
