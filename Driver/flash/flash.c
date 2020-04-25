#include "stm32f4xx.h"
#include "flash.h"
#include "usart.h"
#include "rtc.h"
#include "stdio.h"
#include "systick_delay.h"
void flash_init(void)
{
	
	FLASH_Unlock();//解锁
	//清除错误标志
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|
                   FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	//擦除
	if(FLASH_EraseSector(FLASH_Sector_4,VoltageRange_3)==FLASH_COMPLETE)
	{
		//写入数据
		//FLASH_ProgramWord(0x08010000, 0x55555555);
		//FLASH_ProgramWord(0x08010004, 0x88888888);
	}
	//加锁
	FLASH_Lock();
	//读取数据
	//printf("%#x\n",*(uint32_t *)(0x08010000));
	//printf("%#x\n",*(uint32_t *)(0x08010004));

}
void erase_flash(uint32_t FLASH_Sector)
{
	FLASH_Unlock();//解锁
	//清除错误标志
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|
                   FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	//擦除
	if(FLASH_EraseSector(FLASH_Sector,VoltageRange_3)==FLASH_COMPLETE)
	{
			printf("erase sector failed\n");
			return ;
	}
		//加锁
	FLASH_Lock();


}
void write_flash(uint32_t addr,uint32_t data)
{
		FLASH_Unlock();//解锁
	//清除错误标志
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|
                   FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	if(FLASH_ProgramWord(addr,data) != FLASH_COMPLETE)
	{
		printf("write flash failed \r\n");
		//写入数据
	}
	//加锁
	FLASH_Lock();
}
//获取某个地址所在的flash扇区
//addr:flash地址
//返回值:0~11,即addr所在的扇区
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
//从指定地址开始写入指定长度的数据
//特别注意:因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数
//         写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以
//         写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
//         没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写. 
//该函数对OTP区域也有效!可以用来写OTP区!
//OTP区域地址范围:0X1FFF7800~0X1FFF7A0F
//WriteAddr:起始地址(此地址必须为4的倍数!!)
//pBuffer:数据指针
//NumToWrite:字(32位)数(就是要写入的32位数据的个数.) 
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
{ 
  FLASH_Status status = FLASH_COMPLETE;
	u32 addrx=0;
	u32 endaddr=0;	
  if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//非法地址
	FLASH_Unlock();									//解锁 
  FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存
 		
	addrx=WriteAddr;				//写入的起始地址
	endaddr=WriteAddr+NumToWrite*4;	//写入的结束地址
	if(addrx<0X1FFF0000)			//只有主存储区,才需要执行擦除操作!!
	{
		while(addrx<endaddr)		//扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
		{
			if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
			{   
				status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V之间!!
				if(status!=FLASH_COMPLETE)break;	//发生错误了
			}else addrx+=4;
		} 
	}
	if(status==FLASH_COMPLETE)
	{
		while(WriteAddr<endaddr)//写数据
		{
			if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)//写入数据
			{ 
				break;	//写入异常
			}
			WriteAddr+=4;
			pBuffer++;
		} 
	}
  FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
	FLASH_Lock();//上锁
}

//读取指定地址的半字(16位数据) 
//faddr:读地址 
//返回值:对应数据.
u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr; 
} 

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToRead:字(4位)数
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)   	
{
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取4个字节.
		ReadAddr+=4;//偏移4个字节.	
	}
}


//封装一个循环写入时间的函数
// addr 要写入的扇区
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

