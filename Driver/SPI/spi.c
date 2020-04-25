#include "stm32f4xx.h"
#include "systick_delay.h"
#include "spi.h"
void spi_init(void)
{
	SPI_InitTypeDef SPI_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;//定义一个GPIO大结构体
	//开启SPI和GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;//输入模式
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_3|GPIO_Pin_4;//3 4 5号引脚
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;//无上拉无下拉
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//设置片选脚为输出
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;//输入模式
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;//无上拉无下拉
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//将IO口映射为SPI
	 GPIO_PinAFConfig(GPIOB, GPIO_PinSource3,GPIO_AF_SPI1);
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource4,GPIO_AF_SPI1);    
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource5,GPIO_AF_SPI1);
	// 片选 不选中
	W25Q128_CS =1;
	 //初始化SPI
  SPI_InitStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_16;//16分频
	SPI_InitStruct.SPI_CPHA=SPI_CPHA_1Edge;//第一个边沿 相位
	SPI_InitStruct.SPI_CPOL=SPI_CPOL_Low;//极性
	//SPI_InitStruct.SPI_CRCPolynomial= //校验
	SPI_InitStruct.SPI_DataSize=SPI_DataSize_8b;//数据长度8位
	SPI_InitStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//全双工
	SPI_InitStruct.SPI_FirstBit=SPI_FirstBit_MSB;//高位优先
	SPI_InitStruct.SPI_Mode=SPI_Mode_Master;//主模式
	SPI_InitStruct.SPI_NSS=SPI_NSS_Soft;//片选 由软件控制
	SPI_Init(SPI1,&SPI_InitStruct);
	
	//使能SPI
	SPI_Cmd(SPI1,ENABLE);
	

}
//发送和接收数据
u8 SPI_readwrite_byte(u8 data)
{
	//等待发送缓冲区为空
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
  //发送数据
	  SPI_I2S_SendData( SPI1,data);
	//同步接受等待 
	   while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
	//接受数据
	
   return SPI_I2S_ReceiveData(SPI1);

}
//读取id
u16 W25Q128_readid(void)
{
	u16 id=0;
	//片选
	W25Q128_CS=0;
	//发送命令
	SPI_readwrite_byte(0x90);
	//发送0x0000000地址
	SPI_readwrite_byte(0x00);
	SPI_readwrite_byte(0x00);
	SPI_readwrite_byte(0x00);
	
	//读取厂家id
	id  = SPI_readwrite_byte(0xff)<< 8 ;
	//读取设备id
	id |= SPI_readwrite_byte(0xff) ;
	//不选中
	W25Q128_CS = 1;
	
	return id;
}
//写使能 1 使能  0 禁止
void w25q128_write_enable(u32 enable)
{
	//片选 拉低
	W25Q128_CS=0;
	if(enable)
		SPI_readwrite_byte(0x06);
	else
		SPI_readwrite_byte(0x04);
	//不选中
	W25Q128_CS = 1;

}
 //读取寄存器1
u8 w25q128_read_status1(void)
{
	u8 status=0;
	//片选 拉低
	W25Q128_CS=0;
	SPI_readwrite_byte(0x05);
	
	status=SPI_readwrite_byte(0xff);
	//不选中
	W25Q128_CS = 1;
	return status;

}
//擦除扇区
void erase_sectors(u32 addr)
{
	u8 status=0;
	//写使能
	w25q128_write_enable(1);
	//延时等待CS脚被检测
	delay_us(10);
	W25Q128_CS=0;
	SPI_readwrite_byte(0x20);
	//发送地址
	SPI_readwrite_byte((addr >> 16) &0xff);
	SPI_readwrite_byte((addr >> 8 ) &0xff);
	SPI_readwrite_byte((addr >> 0 ) &0xff);
	//不选中
	W25Q128_CS = 1;
	//延时等待CS脚被检测
	delay_us(10);
	//等待擦除完成
	while(1)
	{
		status=w25q128_read_status1();
		if((0x1& status)==0)//擦除完成 寄存器1 的最低位为0
					break;
				delay_ms(1);
	
	}
 //关闭写使能
	w25q128_write_enable(0);
}
//写一页  <=256字节
void w25q128_write_page(u32 addr,u8 *pdata,u32 len)
{
	u8 status=0;
	//写使能
	w25q128_write_enable(1);
	//延时等待CS脚被检测
	delay_us(10);
	W25Q128_CS=0;
	
	SPI_readwrite_byte(0x02);
	//发送地址
	SPI_readwrite_byte((addr >> 16) &0xff);
	SPI_readwrite_byte((addr >> 8 ) &0xff);
	SPI_readwrite_byte((addr >> 0 ) &0xff);
	while(len--)
	{
		SPI_readwrite_byte(*pdata++);
	
	}
	
	//不选中
	W25Q128_CS = 1;
	//延时等待CS脚被检测
	delay_us(10);
	//等待写入完成
	while(1)
	{
		status=w25q128_read_status1();
		if((0x1& status)==0)//擦除完成 寄存器1 的最低位为0
					break;
				delay_ms(1);
	
	}
 //关闭写使能
	w25q128_write_enable(0);
	

}
//读flash数据
void w25q128_read_data(u32 addr,u8 *pdata,u32 len)
{
	//片选拉低
	W25Q128_CS = 0;
	
	//发送03H
	SPI_readwrite_byte(0x03);
	
	//发送24位地址 高位先发
	SPI_readwrite_byte((addr>>16)&0xff);
	SPI_readwrite_byte((addr>>8)&0xff);
	SPI_readwrite_byte(addr&0xff);
	
	//读取数据
	while(len--){
		*pdata++ = SPI_readwrite_byte(0xff);
	}
	
	//片选拉高
	W25Q128_CS = 1;
}