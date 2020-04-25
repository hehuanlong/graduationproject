#ifndef _DHT11_H
#define _DHT11_H
#include "stm32f4xx.h"
#define DQ_OUT PGout(9)
#define DQ_IN PGin(9)
void Set_DQ_inout(GPIOMode_TypeDef mode);
u8 dht11_init(void);
u8 dht11_read_bit(void);
u8 dht11_read_byte(void);
u8 dht11_read_data(u8 *temp,u8 *humi);
void temp_humi_res(u8 temp,u8 humi);
#endif 

