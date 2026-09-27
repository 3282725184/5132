#include "reg52.h"
#include "delay.h"
#include "uart.h"
#include "main.h"
#include <string.h>

unsigned char recv;
char esp_recv_buf[15] = {0};
char esp_flag = 1;

void main()
{
	
	Delay_Xms(1000);
	UART_Init();
	
	do
	{
		UART_Send_Str("AT+CWMODE=3\r\n");
		Delay_Xms(500);
	}while(esp_flag);
	
	esp_flag = 1;
	
	do
	{
		UART_Send_Str("AT+CWJAP=\"HONOR 200\",\"514381curry\"\r\n");
		Delay_Xms(1000);
	}while(esp_flag);	
	
	esp_flag = 1;
	
	do
	{
		UART_Send_Str("AT+CIPSTART=\"TCP\",\"10.219.230.146\",5132\r\n");
		Delay_Xms(1000);
	}while(esp_flag);	
	
	esp_flag = 1;	

	do
	{
		UART_Send_Str("AT+CIPSEND=5\r\n");
		Delay_Xms(1000);
	}while(esp_flag);	
	

	UART_Send_Str("CURRY\r\n");
	
	while(1)
	{	
		Delay_Xms(1000);
	
	}
}

void UART_Routine(void) interrupt 4
{
	static char recv_count = 0;
	
	if(RI == 1)
	{
		RI = 0;
		recv = SBUF;
		
		if(recv == 'O' || recv == '+')
		{
			memset(esp_recv_buf,'\0',sizeof(esp_recv_buf));
			recv_count = 0;
			esp_recv_buf[recv_count] = recv;
		}
		else
		{
			recv_count++;
			esp_recv_buf[recv_count] = recv;
		}
		
		if(esp_recv_buf[0] == 'O' && esp_recv_buf[1] == 'K')
		{
			esp_flag = 0;
			memset(esp_recv_buf,'\0',sizeof(esp_recv_buf));
		}
		
		if(esp_recv_buf[0] == '+' && esp_recv_buf[3] == 'D')
		{
			if(esp_recv_buf[7]=='L' && esp_recv_buf[10]=='1'&& esp_recv_buf[11]=='0')
			{
				LED1 = 0;	
			}
			if(esp_recv_buf[7]=='L' && esp_recv_buf[10]=='1'&& esp_recv_buf[11]=='1')
			{
				LED1 = 1;	
			}			
			if(esp_recv_buf[7]=='L' && esp_recv_buf[10]=='2'&& esp_recv_buf[11]=='0')
			{
				LED2 = 0;	
			}
			if(esp_recv_buf[7]=='L' && esp_recv_buf[10]=='2'&& esp_recv_buf[11]=='1')
			{
				LED2 = 1;	
			}			
			if(esp_recv_buf[7]=='B' && esp_recv_buf[10]=='P'&& esp_recv_buf[11]=='0')
			{
				BEEP = 0;	
			}
			if(esp_recv_buf[7]=='B' && esp_recv_buf[10]=='P'&& esp_recv_buf[11]=='1')
			{
				BEEP = 1;	
			}
		}		
		recv_count = recv_count % 14;
	}

}
