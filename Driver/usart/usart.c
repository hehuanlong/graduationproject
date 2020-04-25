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

//����_sys_exit��������ʹ�ð�����ģʽ
void _sys_exit(int x)
{
    x = x;
}

//printf�ĺ���
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
	//����ʱ��ʹ�ܺ�GPIOA ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	//��ʼ��GPIO
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
		//��PA9 PA10 ӳ�䵽����1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	//c��ʼ������
	USART_InitStruct.USART_BaudRate=115200;//������
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//��Ӳ��������
	USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//����/����       ���䷽ʽ
	USART_InitStruct.USART_Parity=USART_Parity_No;//��У��λ
	USART_InitStruct.USART_StopBits=USART_StopBits_1;//1��ֹͣλ
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;//8λ����
	USART_Init(USART1,&USART_InitStruct);
	
	//��ʼ���ж�Դ��NVIC
	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;//�жϺ�
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//�ж�ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0x01;
	NVIC_Init(&NVIC_InitStruct);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//ʹ�ܴ���  ��������
	USART_Cmd(USART1,ENABLE);
}


void usart3_init(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	//����ʱ��ʹ�ܺ�GPIOA ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//��ʼ��GPIO
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
		//��PB10 PB110 ӳ�䵽����1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	//c��ʼ������
	USART_InitStruct.USART_BaudRate=115200;//������
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//��Ӳ��������
	USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//����/����       ���䷽ʽ
	USART_InitStruct.USART_Parity=USART_Parity_No;//��У��λ
	USART_InitStruct.USART_StopBits=USART_StopBits_1;//1��ֹͣλ
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;//8λ����
	USART_Init(USART3,&USART_InitStruct);
	
	//��ʼ���ж�Դ��NVIC
	NVIC_InitStruct.NVIC_IRQChannel=USART3_IRQn;//�жϺ�
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//�ж�ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0x01;
	NVIC_Init(&NVIC_InitStruct);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	//ʹ�ܴ���  ��������
	USART_Cmd(USART3,ENABLE);
		
}
void usart3_putc(u8 data)
{
		//�ȴ����ݷ������
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
		//��������
	 USART_SendData(USART3,data);


}
//����һ���ַ���
void usart3_puts(char *pdata)
{
	while(*pdata!='\0'){
		usart3_putc(*pdata++);
	}
}

volatile u8 usart1_buf[64] = {0};//�洢����1�յ�������
volatile u32 usart1_cnt = 0;//��¼�յ������ݳ���
volatile u32 usart1_flag = 0;//��¼����1�������ʶ
//u8 testbuf[5]={0xa,0xb1,0x8b,0x15,0x25};//����Ŀ���



//����һ���ַ�
void usart1_putc(u8 data)
{
		//�ȴ����ݷ������
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
		//��������
	 USART_SendData(USART1,data);
}
//����һ���ַ���
void usart1_puts(u8 *pdata)
{
	while(*pdata){
		usart1_putc(*pdata++);
	}
}


//�����жϴ�����
void USART1_IRQHandler(void)
{	
	u8 temp = 0;
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)//�����˴��ڽ����ж�
	{	
		temp = USART_ReceiveData(USART1);
		usart3_putc(temp);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);//��������ж�
	}
}

//�����жϴ�����
void USART3_IRQHandler(void)
{	

	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)//�����˴��ڽ����ж�
		{
			USART3_RX_BUFF[USART3_RX_CNT++] = USART_ReceiveData(USART3);
		//����յ��˽�����ʶ���߻��������ˣ����ô����ʶ
		if(USART3_RX_BUFF[USART3_RX_CNT-1]=='\n'||USART3_RX_CNT>=sizeof(usart1_buf)-1)
			USART3_RX_FLAG = 1;	
		
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);//��������ж�
	}
}

