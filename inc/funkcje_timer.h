/*
 * funkcje_timer.h
 *
 *  Created on: 21 paź 2017
 *      Author: pcpio
 */



#ifndef INC_FUNKCJE_TIMER_H_
#define INC_FUNKCJE_TIMER_H_

#define SystemFrequency SystemCoreClock

void timer0_32_init(uint32_t timerInterval);

void delay32Ms_timer0(uint32_t delayInUs);
void delay32Us_timer0(uint32_t delayInUs);

#endif /* INC_FUNKCJE_TIMER_H_ */
