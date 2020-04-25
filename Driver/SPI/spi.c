#include "stm32f4xx.h"
#include "systick_delay.h"
#include "spi.h"
void spi_init(void)
{
	SPI_InitTypeDef SPI_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;//����һ��GPIO��ṹ��
	//����SPI��GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;//����ģʽ
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_3|GPIO_Pin_4;//3 4 5������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;//������������
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//����Ƭѡ��Ϊ���
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;//����ģʽ
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;//������������
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//��IO��ӳ��ΪSPI
	 GPIO_PinAFConfig(GPIOB, GPIO_PinSource3,GPIO_AF_SPI1);
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource4,GPIO_AF_SPI1);    
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource5,GPIO_AF_SPI1);
	// Ƭѡ ��ѡ��
	W25Q128_CS =1;
	 //��ʼ��SPI
  SPI_InitStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_16;//16��Ƶ
	SPI_InitStruct.SPI_CPHA=SPI_CPHA_1Edge;//��һ������ ��λ
	SPI_InitStruct.SPI_CPOL=SPI_CPOL_Low;//����
	//SPI_InitStruct.SPI_CRCPolynomial= //У��
	SPI_InitStruct.SPI_DataSize=SPI_DataSize_8b;//���ݳ���8λ
	SPI_InitStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//ȫ˫��
	SPI_InitStruct.SPI_FirstBit=SPI_FirstBit_MSB;//��λ����
	SPI_InitStruct.SPI_Mode=SPI_Mode_Master;//��ģʽ
	SPI_InitStruct.SPI_NSS=SPI_NSS_Soft;//Ƭѡ ���������
	SPI_Init(SPI1,&SPI_InitStruct);
	
	//ʹ��SPI
	SPI_Cmd(SPI1,ENABLE);
	

}
//���ͺͽ�������
u8 SPI_readwrite_byte(u8 data)
{
	//�ȴ����ͻ�����Ϊ��
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
  //��������
	  SPI_I2S_SendData( SPI1,data);
	//ͬ�����ܵȴ� 
	   while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
	//��������
	
   return SPI_I2S_ReceiveData(SPI1);

}
//��ȡid
u16 W25Q128_readid(void)
{
	u16 id=0;
	//Ƭѡ
	W25Q128_CS=0;
	//��������
	SPI_readwrite_byte(0x90);
	//����0x0000000��ַ
	SPI_readwrite_byte(0x00);
	SPI_readwrite_byte(0x00);
	SPI_readwrite_byte(0x00);
	
	//��ȡ����id
	id  = SPI_readwrite_byte(0xff)<< 8 ;
	//��ȡ�豸id
	id |= SPI_readwrite_byte(0xff) ;
	//��ѡ��
	W25Q128_CS = 1;
	
	return id;
}
//дʹ�� 1 ʹ��  0 ��ֹ
void w25q128_write_enable(u32 enable)
{
	//Ƭѡ ����
	W25Q128_CS=0;
	if(enable)
		SPI_readwrite_byte(0x06);
	else
		SPI_readwrite_byte(0x04);
	//��ѡ��
	W25Q128_CS = 1;

}
 //��ȡ�Ĵ���1
u8 w25q128_read_status1(void)
{
	u8 status=0;
	//Ƭѡ ����
	W25Q128_CS=0;
	SPI_readwrite_byte(0x05);
	
	status=SPI_readwrite_byte(0xff);
	//��ѡ��
	W25Q128_CS = 1;
	return status;

}
//��������
void erase_sectors(u32 addr)
{
	u8 status=0;
	//дʹ��
	w25q128_write_enable(1);
	//��ʱ�ȴ�CS�ű����
	delay_us(10);
	W25Q128_CS=0;
	SPI_readwrite_byte(0x20);
	//���͵�ַ
	SPI_readwrite_byte((addr >> 16) &0xff);
	SPI_readwrite_byte((addr >> 8 ) &0xff);
	SPI_readwrite_byte((addr >> 0 ) &0xff);
	//��ѡ��
	W25Q128_CS = 1;
	//��ʱ�ȴ�CS�ű����
	delay_us(10);
	//�ȴ��������
	while(1)
	{
		status=w25q128_read_status1();
		if((0x1& status)==0)//������� �Ĵ���1 �����λΪ0
					break;
				delay_ms(1);
	
	}
 //�ر�дʹ��
	w25q128_write_enable(0);
}
//дһҳ  <=256�ֽ�
void w25q128_write_page(u32 addr,u8 *pdata,u32 len)
{
	u8 status=0;
	//дʹ��
	w25q128_write_enable(1);
	//��ʱ�ȴ�CS�ű����
	delay_us(10);
	W25Q128_CS=0;
	
	SPI_readwrite_byte(0x02);
	//���͵�ַ
	SPI_readwrite_byte((addr >> 16) &0xff);
	SPI_readwrite_byte((addr >> 8 ) &0xff);
	SPI_readwrite_byte((addr >> 0 ) &0xff);
	while(len--)
	{
		SPI_readwrite_byte(*pdata++);
	
	}
	
	//��ѡ��
	W25Q128_CS = 1;
	//��ʱ�ȴ�CS�ű����
	delay_us(10);
	//�ȴ�д�����
	while(1)
	{
		status=w25q128_read_status1();
		if((0x1& status)==0)//������� �Ĵ���1 �����λΪ0
					break;
				delay_ms(1);
	
	}
 //�ر�дʹ��
	w25q128_write_enable(0);
	

}
//��flash����
void w25q128_read_data(u32 addr,u8 *pdata,u32 len)
{
	//Ƭѡ����
	W25Q128_CS = 0;
	
	//����03H
	SPI_readwrite_byte(0x03);
	
	//����24λ��ַ ��λ�ȷ�
	SPI_readwrite_byte((addr>>16)&0xff);
	SPI_readwrite_byte((addr>>8)&0xff);
	SPI_readwrite_byte(addr&0xff);
	
	//��ȡ����
	while(len--){
		*pdata++ = SPI_readwrite_byte(0xff);
	}
	
	//Ƭѡ����
	W25Q128_CS = 1;
}