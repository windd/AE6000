

#ifndef __DW_LCD_H_
#define __DW_LCD_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "typedef.h"

#define RS_OFF  GPIO_ResetBits(GPIOF, GPIO_Pin_10)//RS AO
#define RS_ON  GPIO_SetBits(GPIOF,GPIO_Pin_10)//RS
#define CS_OFF  GPIO_ResetBits(GPIOE, GPIO_Pin_6)//CS1
#define CS_ON  GPIO_SetBits(GPIOE, GPIO_Pin_6)//CS1
#define LIGHT_ON  GPIO_ResetBits(GPIOE, GPIO_Pin_4)   //背光
#define LIGHT_OFF  GPIO_SetBits(GPIOE, GPIO_Pin_4)   //背光
#define A_BIT (GPIOE->ODR & GPIO_Pin_4)
#define RES_OFF  GPIO_ResetBits(GPIOE, GPIO_Pin_5)  //RES
#define RES_ON  GPIO_SetBits(GPIOE, GPIO_Pin_5)    //RES
#define E_ON  GPIO_SetBits(GPIOF, GPIO_Pin_8)//E
#define E_OFF  GPIO_ResetBits(GPIOF, GPIO_Pin_8)//E
#define RW_OFF  GPIO_ResetBits(GPIOF, GPIO_Pin_9)//R/W



void LCD_WCOM_L(unchar com);


void LCD_WData_L(unchar lcddata);

void LCDclear(void);

void LCD_Init(void);


void LCDWword(unchar x, unchar y, unchar len, const unchar *word); //X 行 Y 起始列


void LCDWchar(unchar x, unchar y, unchar len, const unchar *word); //len变色 len=111(波特率)

#endif

