#ifndef _SUPERSONIC_H
#define _SUPERSONIC_H
#include "stm32f4xx.h"
void supersonic_init(void );
int get_sro4_distance();
void Trigger_distance(u16 distance );
#define ECHO PCin(6)
#define TRIG PCout(8)
#endif 
