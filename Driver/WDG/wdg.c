#include "stm32f4xx.h"
#include "wdg.h"
void wdg_init(void)
{
	//�ر�дʹ��
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);   
	//����Ԥ��Ƶ�ͳ�ʼ����ֵ  LSIRC 32k   1s
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(1000);
	//װ�ؼ���ֵ��ι����
	 IWDG_ReloadCounter();
	//�������Ź�
	IWDG_Enable();

}