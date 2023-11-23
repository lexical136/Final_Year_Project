#ifndef TIMER_H_
#define TIMER_H_

#include <stdbool.h>

// set default CPU clock frequency to 16 MHz
#ifndef F_CPU
#define F_CPU 16000000
#endif /* F_CPU */

unsigned long timer_cpuf();
void timer_init(void);
void timer_start(unsigned short value);
bool timer_overflow();
unsigned short timer_now();
unsigned long timer_now_us();
void timer_wait_micro(unsigned short delay);
void timer_wait_milli(unsigned short delay);

#endif /* TIMER_H_ */
