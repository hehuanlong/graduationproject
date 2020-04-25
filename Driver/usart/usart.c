#include "stm32f4xx.h"
//#include "warterfall_led.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "sys.h"
#include "dht11.h"
#pragma import(__use_no_semihosting)
 uint8_t USART3_RX_BUFF[512];
 uint32_t USART3_RX_CNT;
 uint32_t USART3_RX_FLAG;
struct __FILE
{
    int handle;
};

FILE __stdout;

//定义_sys_exit函数避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}

//printf的后门
int fputc(int ch,FILE *f)
{
	   while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
    USART_SendData(USART1,ch);
	
	return ch;
}
void usart1_init(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	//串口时钟使能和GPIOA 使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	//初始化GPIO
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
		//将PA9 PA10 映射到串口1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	//c初始化串口
	USART_InitStruct.USART_BaudRate=115200;//波特率
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//无硬件流控制
	USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//接收/发送       传输方式
	USART_InitStruct.USART_Parity=USART_Parity_No;//无校验位
	USART_InitStruct.USART_StopBits=USART_StopBits_1;//1个停止位
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;//8位数据
	USART_Init(USART1,&USART_InitStruct);
	
	//初始化中断源和NVIC
	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;//中断号
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//中断使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0x01;
	NVIC_Init(&NVIC_InitStruct);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//使能串口  开启串口
	USART_Cmd(USART1,ENABLE);
}


void usart3_init(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	//串口时钟使能和GPIOA 使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//初始化GPIO
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
		//将PB10 PB110 映射到串口1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	//c初始化串口
	USART_InitStruct.USART_BaudRate=115200;//波特率
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//无硬件流控制
	USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//接收/发送       传输方式
	USART_InitStruct.USART_Parity=USART_Parity_No;//无校验位
	USART_InitStruct.USART_StopBits=USART_StopBits_1;//1个停止位
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;//8位数据
	USART_Init(USART3,&USART_InitStruct);
	
	//初始化中断源和NVIC
	NVIC_InitStruct.NVIC_IRQChannel=USART3_IRQn;//中断号
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//中断使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0x01;
	NVIC_Init(&NVIC_InitStruct);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	//使能串口  开启串口
	USART_Cmd(USART3,ENABLE);
		
}
void usart3_putc(u8 data)
{
		//等待数据发送完成
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
		//发送数据
	 USART_SendData(USART3,data);


}
//发送一个字符串
void usart3_puts(char *pdata)
{
	while(*pdata!='\0'){
		usart3_putc(*pdata++);
	}
}

volatile u8 usart1_buf[64] = {0};//存储串口1收到的数据
volatile u32 usart1_cnt = 0;//记录收到的数据长度
volatile u32 usart1_flag = 0;//记录串口1待处理标识
//u8 testbuf[5]={0xa,0xb1,0x8b,0x15,0x25};//保存的卡号



//发送一个字符
void usart1_putc(u8 data)
{
		//等待数据发送完成
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
		//发送数据
	 USART_SendData(USART1,data);
}
//发送一个字符串
void usart1_puts(u8 *pdata)
{
	while(*pdata){
		usart1_putc(*pdata++);
	}
}


//串口中断处理函数
void USART1_IRQHandler(void)
{	
	u8 temp = 0;
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)//发生了串口接收中断
	{	
		temp = USART_ReceiveData(USART1);
		usart3_putc(temp);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);//清除接收中断
	}
}

//串口中断处理函数
void USART3_IRQHandler(void)
{	

	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)//发生了串口接收中断
		{
			USART3_RX_BUFF[USART3_RX_CNT++] = USART_ReceiveData(USART3);
		//如果收到了结束标识或者缓冲区满了，设置处理标识
		if(USART3_RX_BUFF[USART3_RX_CNT-1]=='\n'||USART3_RX_CNT>=sizeof(usart1_buf)-1)
			USART3_RX_FLAG = 1;	
		
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);//清除接收中断
	}
}

