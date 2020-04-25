#ifndef _SYSTICK_DELAY_H
#define _SYSTICK_DELAY_H
#include "stm32f4xx.h"
void delay_init(void);
void delay_us (u32 us);
void delay_ms (u32 ms);
void delay_s (u32 s);
#endif 

