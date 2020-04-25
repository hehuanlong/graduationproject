#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"
void adc_init(void )
{
	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	 GPIO_InitTypeDef GPIO_InitStruct;//����һ��GPIO��ṹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//ʹ��adcʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//84M
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AN;//����ģʽ
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6;//0������
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;//������������
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//��λADC
	ADC_DeInit();
	//����ADC1�ķ�Ƶϵ��
	ADC_CommonInitStruct.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;//��ʹ��DMA
	ADC_CommonInitStruct.ADC_Mode=ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler=ADC_Prescaler_Div4;//4��Ƶ 84/4 =21 ������30M
	ADC_CommonInitStruct.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;//��������5ms
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	//��ʼ��ADC1(�ֱ���,��������,����ģʽ)
	//5.ADC��ʼ��(�ֱ��� �������� ����ģʽ)
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//������ת��
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//�����Ҷ���
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//���ⲿ���������������
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//��ʹ�ö�ͨ��
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;//12λ�ֱ���
	ADC_InitStruct.ADC_NbrOfConversion = 2;//��������Ŀ
	ADC_Init(ADC1, &ADC_InitStruct);
	
		//7.���ù���ͨ��(ADC1��ͨ��5,��һ������,����ʱ��Ϊ3��ʱ������)
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_3Cycles);
	
	//6.ʹ��ADC1
	ADC_Cmd(ADC1,ENABLE);
}
u16 get_adc1value(void)
{
	u16 value=0;
	//7.���ù���ͨ��(ADC1��ͨ��5,��һ������,����ʱ��Ϊ3��ʱ������)
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);
	//����ADCת��
	ADC_SoftwareStartConv(ADC1);
	//�ȴ�ת������
   while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)!=SET);
  //��ȡת�����
    value=ADC_GetConversionValue(ADC1);
	
	return value;

}

u16 get_adc2value(void)
{
	u16 value=0;
	uint8_t i=0;
	//7.���ù���ͨ��(ADC1��ͨ��5,��һ������,����ʱ��Ϊ3��ʱ������)
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_3Cycles);
	//����ADCת��
	ADC_SoftwareStartConv(ADC1);
	//�ȴ�ת������
   while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)!=SET)
	 {
			
		   i++;
			 DELAY_MS(100);
		   if(  i== 30 ) //��ʱ3S����
			 {
				 printf("ADC_GetFlagStatus  Timeout\r\n");
				 //�ϱ���8266				
				 break;
			 }
			 
	 }
  //��ȡת�����
    value=ADC_GetConversionValue(ADC1);
	
	return value;

}

