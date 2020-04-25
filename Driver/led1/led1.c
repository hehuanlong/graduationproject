#include "led1.h"
#include "stm32f4xx.h"

void led1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;//����һ��GPIO��ṹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʱ�ӵ�ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʱ�ӵ�ʹ��
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;//9������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//����ٶ�
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;//������������
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14;//9������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//����ٶ�
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;//������������
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	GPIO_SetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);
	GPIO_SetBits(GPIOE,GPIO_Pin_13|GPIO_Pin_14);
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);
}

//void delay(unsigned int ms)
//{
//	int i,j;
//	for(i=0;i<ms;i++)
//		for(j=0;j<12000;j++);

//}
/*	n 1  2  3  4
    onoff  1   0     
           ��  ��

*/
//void led_onoff(int n,int onoff)
//{
//	int i;
//	if(n==1){
//		if(onoff){
//			GPIO_ResetBits(GPIOF,GPIO_Pin_9);
//		}
//		else 
//			GPIO_SetBits(GPIOF,GPIO_Pin_9);
//			
//	}else if(n==2){
//	
//		if(onoff){
//			GPIO_ResetBits(GPIOF,GPIO_Pin_10);
//			
//		}
//		
//		else 
//			GPIO_SetBits(GPIOF,GPIO_Pin_10);
//			
//	} else if(n==3){
//		if(onoff){
//			GPIO_ResetBits(GPIOE,GPIO_Pin_13);
//		}
//		else 
//			GPIO_SetBits(GPIOE,GPIO_Pin_13);		
//	} else if(n==4){
//		if(onoff){
//			GPIO_ResetBits(GPIOE,GPIO_Pin_14);		
//		}
//			
//		else 
//			GPIO_SetBits(GPIOE,GPIO_Pin_14);	
//	
//	}

//}


	