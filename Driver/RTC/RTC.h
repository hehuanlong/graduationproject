#ifndef _RTC_H
#define _RTC_H
#include "stm32f4xx.h"
void rtc_init(void);
void rtc_gettime( void );
void rtc_send_gettime(RTC_TimeTypeDef *RTC_TimeStruct,RTC_DateTypeDef *RTC_DateStruct);

#endif 
