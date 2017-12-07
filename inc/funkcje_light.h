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

void light_init (void);
void light_enable (void);
void light_setRange(light_range_t newRange);


#endif /* FUNKCJE_LIGHT_H_ */
