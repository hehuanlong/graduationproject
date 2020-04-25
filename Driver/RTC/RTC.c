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
	
	//if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x8888)//判断是否已经初始化过
	//{
	  //1使能PWR时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		//2使能RTC寄存器访问
		PWR_BackupAccessCmd(ENABLE);
		//3 选择RTC时钟源(LSE) LSE =32.768
		RCC_LSEConfig(RCC_LSE_ON);//使能LSE时钟
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)!=SET);//等待LSE就绪
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//选择LSE作为RTC时钟源
		RCC_RTCCLKCmd(ENABLE);//使能RTC时钟
		//（4）等待所有RTC寄存器就绪
			RTC_WaitForSynchro();



		//5 初始化RTC(分频系数/时间格式)
		RTC_InitStruct.RTC_HourFormat=RTC_HourFormat_24;
		RTC_InitStruct.RTC_AsynchPrediv=0x7f;//异步分频系数 127+1 128分频
		RTC_InitStruct.RTC_SynchPrediv=0xff;//同步分频系数  255 +1 256 分频
		RTC_Init(&RTC_InitStruct);
		//6设置日期和时间
		RTC_TimeStruct.RTC_Hours =0x11;//时
		RTC_TimeStruct.RTC_H12=RTC_H12_AM;//上午/下午 AM/PM
		RTC_TimeStruct.RTC_Minutes=0x36;//分
		RTC_TimeStruct.RTC_Seconds=0x30;//秒
		RTC_SetTime(RTC_Format_BCD,&RTC_TimeStruct);
		
		RTC_DateStruct.RTC_Year=0x19;
		RTC_DateStruct.RTC_Date=0x19;
		RTC_DateStruct.RTC_Month=RTC_Month_September;
		RTC_DateStruct.RTC_WeekDay=RTC_Weekday_Thursday;
		RTC_SetDate(RTC_Format_BCD,&RTC_DateStruct);
		
		RTC_AlarmCmd(RTC_Alarm_A, DISABLE);//关闭闹钟才能设置
//		//1设置闹钟时间
//		RTC_TimeStruct.RTC_Hours = 0x11;
//		RTC_TimeStruct.RTC_Minutes = 0x36;
//		RTC_TimeStruct.RTC_Seconds = 0x40;
//		RTC_AlarmStruct.RTC_AlarmMask =RTC_AlarmMask_None;//闹钟时间中要屏蔽的成员
//		RTC_AlarmStruct.RTC_AlarmTime =RTC_TimeStruct;//闹钟时间
//		RTC_AlarmStruct.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_WeekDay;//选择星期匹配/日期匹配
//		RTC_AlarmStruct.RTC_AlarmDateWeekDay=RTC_Weekday_Thursday;//闹钟 星期/日期
//		RTC_SetAlarm(RTC_Format_BCD,RTC_Alarm_A,&RTC_AlarmStruct);
//		//2 清除闹钟终端标识，使能闹钟中断
//		RTC_ClearITPendingBit(RTC_IT_ALRA);
//		RTC_ITConfig(RTC_IT_ALRA,ENABLE);
//		//初始化外部中断
//		EXTI_InitStruct.EXTI_Line=EXTI_Line17;//中断号
//	  EXTI_InitStruct.EXTI_LineCmd=ENABLE;//中断使能
//		EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;//设置为中断模式
//		EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising;//上升沿触发
//		EXTI_Init(&EXTI_InitStruct);
//		
//		NVIC_InitStruct.NVIC_IRQChannel=RTC_Alarm_IRQn;//中断号
//		NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//中断使能
//		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0x03;
//		NVIC_InitStruct.NVIC_IRQChannelSubPriority=0x03;
//		NVIC_Init(&NVIC_InitStruct);
		//使能闹钟
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
	
	EXTI_ClearITPendingBit(EXTI_Line17);//清除外部中断17

}
