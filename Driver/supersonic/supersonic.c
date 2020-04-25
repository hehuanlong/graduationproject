
#include "supersonic.h"
//#include "systick_delay.h"
#include "delay.h"
#include "usart.h"
#include "sys.h"
#include "rtthread.h"
#include "led.h"
/*������������������,����cm*/
void supersonic_init(void )
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;//PC8
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;//PC6
	GPIO_Init(GPIOC,&GPIO_InitStruct);

}

int get_sro4_distance()
{
	
	int time=0;
	TRIG = 0;
	printf(" get_sro4_distance\n");
	DELAY_MS(10);//���ֵ͵�ƽ
	TRIG = 1;	
	DWT_Delay_us(15);
	TRIG = 0;
	
	while(ECHO==0)
		TIM_SetCounter(TIM2,0);//���ô�0��ʼ��߽���ʱ����ʼֵ��Ϊ0(TIM_SetCounter)
  	while(ECHO){
	time = TIM_GetCounter(TIM2);// ��Ͷ�ȡ��ʱ������ֵ(TIM_GetCounter)
	}  
   return time*10/58 ;  //��ʽ��uS/58=���׻��� uS/148=Ӣ�磻���ǣ�����= �ߵ�ƽʱ��*���٣�340M/S��/2 34000/1000
}

void Trigger_distance(u16 distance )
{
	u16 value;
	while(1)
	{
		value =get_sro4_distance();
			
		if(value<=distance)
		{
		 // printf("distance : %d\r\n", value);
			PFout(10)=0;
			DELAY_MS(value);
			PFout(10)=1;
		  DELAY_MS(value);
		
		}
		else 
		{
			PFout(10)=1;
			printf("distance : %d\r\n", value);
			break;
		}
		Delay_ms(20);
	
	}

}
