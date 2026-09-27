#include "reg52.h"
#include <intrins.h>

typedef unsigned char uchar;
typedef unsigned int uint;

sbit LED1 = P1^0;
sbit LED2 = P1^1;
sbit LED3 = P1^2;
sbit LED4 = P1^3;
sbit BEEP = P1^6;
sbit JDQ1 = P2^0;

uchar recv;

void Delay_xms(uint xms)	//@11.0592MHz
{
	uchar data i, j;
	while(xms)
	{
		_nop_();
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
    xms--;		
	}
} 
//串口发送一个字节
void UART_Send_Byte(uchar send_byte)
{
	SBUF = send_byte;
	while(!TI);
	TI=0;
}

//串口发送字符串
void UART_Send_Str(uchar *send_str)
{	
	while(*send_str != '\0') //判断字符串结束符，如果识别到‘\0',则跳出while，结束发送
	{
		UART_Send_Byte(*send_str++);	//本质是一个字节一个字节的发送，
		//发送完一个字节，地址+1，等待发送下一个字节，直到检测到字符串结束符为止
	}	
} 
void main()
{
	SCON = 0x50; 	//串口配成工作方式1
	PCON &= 0x7F; //波特率不加倍
	TMOD &= 0x0f;	
	TMOD |= 0x20;	//定时器1，模式二，自动重装初值，串口按照一定速率发数据，需要用的定时器
	TH1 = 0xFD;	
	TL1 = 0xFD; //@11.0592MH时钟，9600比特率定时器初值
	TR1 = 1; //定时器开始运行   
	ES = 1;  //串口中断打开
	EA=1;					//总中断打开

	while(1)
  {
		UART_Send_Str("I am wfeng!\r\n");
		Delay_xms(1000);
  }
}

/*****************
串口中断处理函数，接收数据
*****************/
void UART_Routine(void) interrupt 4
{
	if(1 == RI) //判断接收标志位是否为1
	{
		RI = 0;  //RI接收标志位清0
		recv = SBUF;   //接收数据
		switch (recv)
		{
			case 0x1:
				LED1 = 0;
				break;
			case 0x2:
				LED2 = 0;				
				break;
			case 0x3:
				LED3 = 0;
				break;
			case 0x4:
				LED4 = 0;				
				break;	
			case 0x5:
				P1 |= 0x0F;
				break;
			case 0x6:
				BEEP = 0;				
				break;
			case 0x7:
				BEEP = 1;
				break;
			case 0x8:
				JDQ1 = 0;				
				break;
			case 0x9:
				JDQ1 = 1;				
				break;			
			default:
				break;
		}
	}

}
