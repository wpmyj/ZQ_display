/*****************************************************************************
 *
 * Timer 
 *
 *****************************************************************************
 * FileName:        timer.h
 * Dependencies:    
 * Processor:
 * Compiler:
 * Linker:  
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the "Company") is intended and supplied to you, the Company's
 * customer, for use solely and exclusively with products manufactured
 * by the Company. 
 *
 * The software is owned by the Company and/or its supplier, and is 
 * protected under applicable copyright laws. All rights are reserved. 
 * Any use in violation of the foregoing restrictions may subject the 
 * user to criminal sanctions under applicable laws, as well as to 
 * civil liability for the breach of the terms and conditions of this 
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES, 
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT, 
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR 
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 * This is a simple timer function used to provide quant for state mashines
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Ross Fosler			04/28/03	...	
 * 
 *****************************************************************************/
#ifndef LCD_COM_H
#define LCD_COM_H

#ifndef LCD_COM_GLOBAL
#define LCD_COM_EXT				extern
#else
#define LCD_COM_EXT
#endif



typedef struct
{
	u8 t_string[32][10];
	u8 n_val[32][10];
	u8 g_string[2][50];
	u8 j_percent[2][4];
	u8 x_str[4][50];
	u8 f_str[2][30];
}page_para;

typedef struct
{
	unsigned char start;
	unsigned char pageID;
	unsigned char touchStatus;
	unsigned char cmdEnd[3];
}CMD_PAGE_ID;

typedef struct
{
	unsigned char  cmdUpdate;
	CMD_PAGE_ID    cmdPage;
}CMD_PAGE_ID_INFO;


extern CMD_PAGE_ID_INFO  gIDInfo;

extern page_para gPagePara;

extern void uart_to_main_cmd_parse(void);


#endif //#ifndef LCD_COM_H


