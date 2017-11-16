
#ifndef __DW_YC_H_
#define __DW_YC_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "typedef.h"

#include "bianliang.h"
#include "menu_content.h"
#include "adc.H"
#include "lcd.h"
#include "protection.h"
#include "math.h"
#include "YC_DFT.h"



#define sinB 0.866
#define cosB  -0.5

extern int YCArray[Num_Adc_Chanel+2][20];
//extern unchar  YCx;



void RYC(void);


void F_Init(void);

void F(void);
#endif
