#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/05
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//LED�˿ڶ���
#define LED0 PAout(8)	// PA8
#define LED1 PDout(2)	// PD2	

#define SET_TEST_PIN1()									PAout(2) = 1
#define CLR_TEST_PIN1()									PAout(2) = 0
#define SET_TEST_PIN2()									PAout(3) = 1
#define CLR_TEST_PIN2()									PAout(3) = 0


void LED_Init(void);	//��ʼ��		 				    
#endif
















