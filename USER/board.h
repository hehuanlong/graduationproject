#ifndef __BOARD_H_
#define __BOARD_H_

#include "stm32f4xx.h"

#include "led.h"

void rt_hw_board_init(void);
void SysTick_Handler(void);

#endif
