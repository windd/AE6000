
#ifndef __DW_YC_H_
#define __DW_YC_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "typedef.h"



#define sinB 0.866
#define cosB  -0.5

//extern int YCArray[16][20];
//extern unchar  YCx;
extern int16_t LowV;

void YCDFT(void);

void RYC(void);


void F_Init(void);

void F(void);
#endif
