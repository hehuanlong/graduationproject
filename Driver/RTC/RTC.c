#include "stm32f4xx.h"
#include "stdio.h"
#include "rtc.h"
#include "sys.h"
#include "flash.h"
void rtc_init(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	RTC_AlarmTypeDef RTC_AlarmStruct;
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	RTC_InitTypeDef RTC_InitStruct;
	
	//if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x8888)//�ж��Ƿ��Ѿ���ʼ����
	//{
	  //1ʹ��PWRʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		//2ʹ��RTC�Ĵ�������
		PWR_BackupAccessCmd(ENABLE);
		//3 ѡ��RTCʱ��Դ(LSE) LSE =32.768
		RCC_LSEConfig(RCC_LSE_ON);//ʹ��LSEʱ��
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)!=SET);//�ȴ�LSE����
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//ѡ��LSE��ΪRTCʱ��Դ
		RCC_RTCCLKCmd(ENABLE);//ʹ��RTCʱ��
		//��4���ȴ�����RTC�Ĵ�������
			RTC_WaitForSynchro();



		//5 ��ʼ��RTC(��Ƶϵ��/ʱ���ʽ)
		RTC_InitStruct.RTC_HourFormat=RTC_HourFormat_24;
		RTC_InitStruct.RTC_AsynchPrediv=0x7f;//�첽��Ƶϵ�� 127+1 128��Ƶ
		RTC_InitStruct.RTC_SynchPrediv=0xff;//ͬ����Ƶϵ��  255 +1 256 ��Ƶ
		RTC_Init(&RTC_InitStruct);
		//6�������ں�ʱ��
		RTC_TimeStruct.RTC_Hours =0x11;//ʱ
		RTC_TimeStruct.RTC_H12=RTC_H12_AM;//����/���� AM/PM
		RTC_TimeStruct.RTC_Minutes=0x36;//��
		RTC_TimeStruct.RTC_Seconds=0x30;//��
		RTC_SetTime(RTC_Format_BCD,&RTC_TimeStruct);
		
		RTC_DateStruct.RTC_Year=0x19;
		RTC_DateStruct.RTC_Date=0x19;
		RTC_DateStruct.RTC_Month=RTC_Month_September;
		RTC_DateStruct.RTC_WeekDay=RTC_Weekday_Thursday;
		RTC_SetDate(RTC_Format_BCD,&RTC_DateStruct);
		
		RTC_AlarmCmd(RTC_Alarm_A, DISABLE);//�ر����Ӳ�������
//		//1��������ʱ��
//		RTC_TimeStruct.RTC_Hours = 0x11;
//		RTC_TimeStruct.RTC_Minutes = 0x36;
//		RTC_TimeStruct.RTC_Seconds = 0x40;
//		RTC_AlarmStruct.RTC_AlarmMask =RTC_AlarmMask_None;//����ʱ����Ҫ���εĳ�Ա
//		RTC_AlarmStruct.RTC_AlarmTime =RTC_TimeStruct;//����ʱ��
//		RTC_AlarmStruct.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_WeekDay;//ѡ������ƥ��/����ƥ��
//		RTC_AlarmStruct.RTC_AlarmDateWeekDay=RTC_Weekday_Thursday;//���� ����/����
//		RTC_SetAlarm(RTC_Format_BCD,RTC_Alarm_A,&RTC_AlarmStruct);
//		//2 ��������ն˱�ʶ��ʹ�������ж�
//		RTC_ClearITPendingBit(RTC_IT_ALRA);
//		RTC_ITConfig(RTC_IT_ALRA,ENABLE);
//		//��ʼ���ⲿ�ж�
//		EXTI_InitStruct.EXTI_Line=EXTI_Line17;//�жϺ�
//	  EXTI_InitStruct.EXTI_LineCmd=ENABLE;//�ж�ʹ��
//		EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;//����Ϊ�ж�ģʽ
//		EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising;//�����ش���
//		EXTI_Init(&EXTI_InitStruct);
//		
//		NVIC_InitStruct.NVIC_IRQChannel=RTC_Alarm_IRQn;//�жϺ�
//		NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//�ж�ʹ��
//		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0x03;
//		NVIC_InitStruct.NVIC_IRQChannelSubPriority=0x03;
//		NVIC_Init(&NVIC_InitStruct);
		//ʹ������
		RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x8888);
 // }
}
void rtc_gettime()
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	RTC_GetTime(RTC_Format_BCD, &RTC_TimeStruct);
  RTC_GetDate(RTC_Format_BCD, &RTC_DateStruct);
	//2019-9-19 11:36:30
		printf("20%02x-%02x-%02x Week:%x %02x:%02x:%02x\r\n",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date,
			RTC_DateStruct.RTC_WeekDay,RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);

}
void rtc_send_gettime(RTC_TimeTypeDef *RTC_TimeStruct,RTC_DateTypeDef *RTC_DateStruct)
{
  RTC_GetTime(RTC_Format_BCD, RTC_TimeStruct);
  RTC_GetDate(RTC_Format_BCD, RTC_DateStruct);


}
void RTC_Alarm_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_ALRA)==SET)
	{
	//PFout(9) ^=1;
	RTC_ClearITPendingBit(RTC_IT_ALRA);
	}
	
	EXTI_ClearITPendingBit(EXTI_Line17);//����ⲿ�ж�17

}
