//#define GPIOG_CRL   (*((unsigned int *)(0x40012000)))
//#define GPIOG_BSRR  (*((unsigned int *)(0x40012010)))
//#define GPIOG_BRR   (*((unsigned int *)(0x40012014)))
//#define RCC_APB2ENR (*((unsigned int *)(0x40021018)))
/* Configure all the GPIOA in Input Floating mode */

#ifndef _DW_HH_H_
#define _DW_HH_H_

#include "stm32f10x.h"
#include "typedef.h"
#include "define.h"

//spi:����1 0,����2 1����ֵ10-99��������100-149������150-199��ѹ��200-249��ϵͳ����250-299��ͨѶ����300-349��SOE350
#define PW1ADDR 0
#define PW2ADDR 1
#define DZADDR 10
#define CONADDR 100
#define OUTADDR 150
#define SWADDR 200
#define SYSADDR 250
#define COMMADDR 300
#define SOEnumADDR 350
#define SOEdataADDR 360



//Password 0 SYS 1 DZ 10 CON 90  SOE 150
#define SoeStAddr 3000////

void Rtime(void);//
void Wtime(void);//
void RSYS(void);//
void Dis_DZ(unchar cur, unchar Fx, unchar Fy); //dis DZ
void Dis_SYS(unchar cur, unchar Fx, unchar Fy); //dis SYS
void RDZ(void);//
void WDZ(void);//
void WSYS(void);//change comm parameter
unchar Rpw(void);//
void Wpw(void);//
void Rcon(unchar XGbit);//
void Wcon(void);//
void RSOEnum(void);

void RCON(void);




#endif

