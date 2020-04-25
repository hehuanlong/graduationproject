#include "stm32f4xx.h"
#include "wdg.h"
void wdg_init(void)
{
	//关闭写使能
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);   
	//设置预分频和初始计数值  LSIRC 32k   1s
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(1000);
	//装载计数值（喂狗）
	 IWDG_ReloadCounter();
	//启动看门狗
	IWDG_Enable();

}