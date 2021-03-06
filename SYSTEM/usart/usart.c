#include <string.h>
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

#include "../USER/main.h"


//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化（适合STM32F10x系列）		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2010/1/1
//版本：V1.7
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
//V1.6修改说明 20150109
//uart_init函数去掉了开启PE中断
//V1.7修改说明 20150322
//修改OS_CRITICAL_METHOD宏判断为：SYSTEM_SUPPORT_OS
////////////////////////////////////////////////////////////////////////////////// 	  
 
//////////////////////////////////////////////////////////////////

//重定向fputc函数
//printf的输出，指向fputc，由fputc输出到串口
//这里使用串口1(USART1)输出printf信息
void uartSendChar(u8 ch)
{      
	while((USART1->SR&0X40)==0);//等待上一次串口数据发送完成  
	USART1->DR = ch;      	//写DR,串口1将发送数据
}

void uartSendbuffer(u8* ch, int len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		uartSendChar(ch[i]);
	}
}


//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //如果使能了接收

u8 uart_to_main_com_sm;
#define DC_SYNC_BYTE					0xAABBCCDD
#define DC_SYNC_BYTE1					0xAA
#define DC_SYNC_BYTE2					0xBB
#define DC_SYNC_BYTE3					0xCC
#define DC_SYNC_BYTE4					0xDD

//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 uart_to_main_cmd_rcv_buff[UART_TO_MAIN_CMD_RCV_BUFF_LEN];     		//接收缓冲,最大USART_REC_LEN个字节.
u8 uart_to_main_cmd_parse_buff[UART_TO_MAIN_CMD_RCV_BUFF_LEN];     	//接收缓冲,最大USART_REC_LEN个字节.

u16 uart1_rcv_cnt = 0;       //接收状态标记	 
u16 total_len = 0;

u8  endflg[3] = {0};
u8  endflgcnt = 0;
void USART1_IRQHandler(void)
{
	u8 rcv_byte = 0;	
		
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	
	if(USART1->SR&(1<<5))	//接收到数据
	{	 
		rcv_byte=USART1->DR; 
			
		//存入缓冲区
		uart_to_main_cmd_rcv_buff[uart1_rcv_cnt] = rcv_byte;
		uart1_rcv_cnt++;
		if(uart1_rcv_cnt >= 3)
		{
			if((uart_to_main_cmd_rcv_buff[uart1_rcv_cnt - 3] == 0xff) && 
				(uart_to_main_cmd_rcv_buff[uart1_rcv_cnt - 2] == 0xff) &&
				(uart_to_main_cmd_rcv_buff[uart1_rcv_cnt - 1] == 0xff)) //收到一条完整报文
			{
				memcpy ( uart_to_main_cmd_parse_buff, uart_to_main_cmd_rcv_buff, uart1_rcv_cnt - 3 );
				uart_to_main_cmd_parse_buff[uart1_rcv_cnt - 3] = 0; //加入一个结束符	
				uart1_rcv_cnt = 0;				
				main_task_mask |= MAIN_TASK_MASK_UART_TO_MAIN_CMD_PARSE;
			}
			else  //检测少接到0xff的情况
			{
				if((uart_to_main_cmd_rcv_buff[uart1_rcv_cnt - 3] == 0xff))
				{
					if((uart_to_main_cmd_rcv_buff[uart1_rcv_cnt - 2] != 0xff) ||
						(uart_to_main_cmd_rcv_buff[uart1_rcv_cnt - 1] != 0xff))
					{
						uart_to_main_cmd_rcv_buff[0] = uart_to_main_cmd_rcv_buff[uart1_rcv_cnt - 2];
						uart_to_main_cmd_rcv_buff[1] = uart_to_main_cmd_rcv_buff[uart1_rcv_cnt - 1];
						uart1_rcv_cnt = 2;
					}
				}
			}
		}
	}
	
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
	
} 
#endif										 
//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	GPIOA->CRH&=0XFFFFF00F;//IO状态设置
	GPIOA->CRH|=0X000008B0;//IO状态设置 
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X200C;  //1位停止,无校验位.
#if EN_USART1_RX		  //如果使能了接收
	//使能接收中断 
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//组2，最低优先级 
#endif
}
