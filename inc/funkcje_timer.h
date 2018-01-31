#ifndef INC_FUNKCJE_TIMER_H_
#define INC_FUNKCJE_TIMER_H_

#define SystemFrequency SystemCoreClock

void timer0_32_init(uint32_t timerInterval);

void delay32Ms_timer0(uint32_t delayInMs);
void delay32Us_timer0(uint32_t delayInUs);

#endif
