#include "stm32f4xx.h"
#include "flash.h"
#include "usart.h"
#include "rtc.h"
#include "stdio.h"
#include "systick_delay.h"
void flash_init(void)
{
	
	FLASH_Unlock();//����
	//��������־
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|
                   FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	//����
	if(FLASH_EraseSector(FLASH_Sector_4,VoltageRange_3)==FLASH_COMPLETE)
	{
		//д������
		//FLASH_ProgramWord(0x08010000, 0x55555555);
		//FLASH_ProgramWord(0x08010004, 0x88888888);
	}
	//����
	FLASH_Lock();
	//��ȡ����
	//printf("%#x\n",*(uint32_t *)(0x08010000));
	//printf("%#x\n",*(uint32_t *)(0x08010004));

}
void erase_flash(uint32_t FLASH_Sector)
{
	FLASH_Unlock();//����
	//��������־
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|
                   FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	//����
	if(FLASH_EraseSector(FLASH_Sector,VoltageRange_3)==FLASH_COMPLETE)
	{
			printf("erase sector failed\n");
			return ;
	}
		//����
	FLASH_Lock();


}
void write_flash(uint32_t addr,uint32_t data)
{
		FLASH_Unlock();//����
	//��������־
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|
                   FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	if(FLASH_ProgramWord(addr,data) != FLASH_COMPLETE)
	{
		printf("write flash failed \r\n");
		//д������
	}
	//����
	FLASH_Lock();
}
//��ȡĳ����ַ���ڵ�flash����
//addr:flash��ַ
//����ֵ:0~11,��addr���ڵ�����
uint16_t STMFLASH_GetFlashSector(u32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 
	return FLASH_Sector_11;	
}
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ر�ע��:��ΪSTM32F4������ʵ��̫��,û�취���ر�����������,���Ա�����
//         д��ַ�����0XFF,��ô���Ȳ������������Ҳ�������������.����
//         д��0XFF�ĵ�ַ,�����������������ݶ�ʧ.����д֮ǰȷ��������
//         û����Ҫ����,��������������Ȳ�����,Ȼ����������д. 
//�ú�����OTP����Ҳ��Ч!��������дOTP��!
//OTP�����ַ��Χ:0X1FFF7800~0X1FFF7A0F
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ4�ı���!!)
//pBuffer:����ָ��
//NumToWrite:��(32λ)��(����Ҫд���32λ���ݵĸ���.) 
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
{ 
  FLASH_Status status = FLASH_COMPLETE;
	u32 addrx=0;
	u32 endaddr=0;	
  if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//�Ƿ���ַ
	FLASH_Unlock();									//���� 
  FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
 		
	addrx=WriteAddr;				//д�����ʼ��ַ
	endaddr=WriteAddr+NumToWrite*4;	//д��Ľ�����ַ
	if(addrx<0X1FFF0000)			//ֻ�����洢��,����Ҫִ�в�������!!
	{
		while(addrx<endaddr)		//ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���)
		{
			if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//�з�0XFFFFFFFF�ĵط�,Ҫ�����������
			{   
				status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V֮��!!
				if(status!=FLASH_COMPLETE)break;	//����������
			}else addrx+=4;
		} 
	}
	if(status==FLASH_COMPLETE)
	{
		while(WriteAddr<endaddr)//д����
		{
			if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)//д������
			{ 
				break;	//д���쳣
			}
			WriteAddr+=4;
			pBuffer++;
		} 
	}
  FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();//����
}

//��ȡָ����ַ�İ���(16λ����) 
//faddr:����ַ 
//����ֵ:��Ӧ����.
u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr; 
} 

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToRead:��(4λ)��
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)   	
{
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//��ȡ4���ֽ�.
		ReadAddr+=4;//ƫ��4���ֽ�.	
	}
}


//��װһ��ѭ��д��ʱ��ĺ���
// addr Ҫд�������
void write_time_flash(uint32_t addr,RTC_TimeTypeDef *RTC_TimeStruct,RTC_DateTypeDef *RTC_DateStruct,u8 i,u8 *card_name,u32 crad_legal)
{
	  u8 j=0;
	  u8 buf[100]={0};
	  RTC_GetTime(RTC_Format_BCD, RTC_TimeStruct);
    RTC_GetDate(RTC_Format_BCD, RTC_DateStruct);
		addr=addr + 0x30*i;
		write_flash(addr,RTC_DateStruct->RTC_Year);
		write_flash(addr+0x04,RTC_DateStruct->RTC_Month);
		write_flash(addr+0x08,RTC_DateStruct->RTC_Date);
		write_flash(addr+0x0c,RTC_TimeStruct->RTC_Hours);
		write_flash(addr+0x10,RTC_TimeStruct->RTC_Minutes);
		write_flash(addr+0x14,RTC_TimeStruct->RTC_Seconds);
		write_flash(addr+0x18,card_name[0]);
		write_flash(addr+0x1c,card_name[1]);
		write_flash(addr+0x20,card_name[2]);
		write_flash(addr+0x24,card_name[3]);
		write_flash(addr+0x28,card_name[4]);
		write_flash(addr+0x2c,crad_legal);
	  sprintf((char *)buf,"20%02x-%02x-%02x  %02x:%02x:%02x %#x %#x %#x %#x %#x %d\r\n",RTC_DateStruct->RTC_Year,RTC_DateStruct->RTC_Month,RTC_DateStruct->RTC_Date,
		RTC_TimeStruct->RTC_Hours,RTC_TimeStruct->RTC_Minutes,RTC_TimeStruct->RTC_Seconds,card_name[0],card_name[1],card_name[2],card_name[3],card_name[4],crad_legal);
		printf("write flash succeed\r\n");
		printf("%s\n",(char *)buf);
		usart3_puts(buf);
		for(j=0;j<12;j++)
		printf(" addr: %#x data :%#x\r\n",addr+4*j,*(uint32_t *)(addr+4*j));

	if(addr>=0x0801ffff)
	{
		
		erase_flash(FLASH_Sector_4);
		addr=0x08010000;
		printf("ADDR_FLASH_SECTOR_4 full\r\n");
	}
	//delay_s(1);
	
}

