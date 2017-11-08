#ifndef DEFINE_DUSTWIND_H
#define DEFINE_DUSTWIND_H



#include "stm32f10x.h"
#include "typedef.h"

#ifdef DW_6023

	#define DZnum 19
	#define CONnum 19
	//====AD开启通道个数======================
	#define	ADC_ENB_NUM	 14
	#define	Num_Adc_Chanel 14

#endif

#ifdef DW_BZT

	#define DZnum 6
	#define CONnum 6
	//====AD开启通道个数======================
	#define	ADC_ENB_NUM	 11
	#define	Num_Adc_Chanel 11

#endif

#define YCnum 26
#define SYSnum 22
#define YXnum 8
#define OUTnum 20
#define SWnum 20
#define COMMnum 10
#define SysErrnum 4

extern const unchar  mainmenu[28][7];
extern structmenu  MENU[16];
extern const unchar con_sm[CONnum+1];
extern const unchar DZunit[DZnum][3];
extern const unchar  menuID1112[CONnum][9];
extern const unchar  menuID16[4][8];
extern const unchar Prot_sm[12];

#endif


