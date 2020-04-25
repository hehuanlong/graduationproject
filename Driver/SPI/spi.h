#ifndef _SPI_H
#define _SPI_H
#include "sys.h"
#include "stm32f4xx.h"
#define W25Q128_CS PBout(14)
void spi_init(void);
u8 SPI_readwrite_byte(u8 data);
u16 W25Q128_readid(void);
//写使能 1 使能  0 禁止
void w25q128_write_enable(u32 enable);
u8 w25q128_read_status1(void);
void erase_sectors(u32 addr);
void w25q128_write_page(u32 addr,u8 *pdata,u32 len);
void w25q128_read_data(u32 addr,u8 *pdata,u32 len);
#endif 
