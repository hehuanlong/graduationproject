#include "ble.h"
#include "systick_delay.h"
#include "usart.h"

void ble_config(void)
{
	usart3_puts("AT+ROLE0\r\n");
	delay_ms(100);
	//���������豸������
	usart3_puts("AT+HELP\r\n");
	delay_ms(100);
	usart3_puts("AT+NAMEHHL123\r\n");
  usart3_puts("AT+INQ\r\n");
	delay_ms(200);
	//��ѯ��ַ
	usart3_puts("AT+LADDR\r\n");
	delay_ms(200);



}