#ifndef _USART_H
#define _USART_H
#include "stm32f4xx.h"
void usart1_init(void);
void usart1_putc(u8 data);
void usart1_puts(u8 *pdata);

void usart3_init(void);
void usart3_putc(u8 data);
void usart3_puts(char *pdata);

void parse_cmd(void);
void  show_mq2_dht11(void);
#endif
