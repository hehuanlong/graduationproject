#ifndef _ADC_H
#define _ADC_H
#include "stm32f4xx.h"
void adc_init(void );
u16 get_adc1value(void);
u16 get_adc2value(void);
#endif
