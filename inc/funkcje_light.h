/*
 * funkcje_light.h
 *
 *  Created on: 20 lis 2017
 *      Author: A
 */

#ifndef FUNKCJE_LIGHT_H_
#define FUNKCJE_LIGHT_H_

typedef enum
{
    LIGHT_RANGE_1000,
    LIGHT_RANGE_4000,
    LIGHT_RANGE_16000,
    LIGHT_RANGE_64000
} light_range_t;

void light_enable (void);
uint32_t light_read (void);
void light_setRange(light_range_t newRange);
void intToString(int value, uint8_t* pBuf, uint32_t len, uint32_t base);

#endif /* FUNKCJE_LIGHT_H_ */
