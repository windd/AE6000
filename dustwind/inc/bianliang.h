
#ifndef __BIANLIANG_H
#define __BIANLIANG_H

#include "stm32f10x.h"
#include "typedef.h"
#include "dz.h"



extern int16_t YC[YCnum];
extern int16_t PhaseAngle[YCnum];
extern structmenu currmenu;
extern unchar YX[YXnum];
extern u16 DZ[DZnum];
extern unchar CON[CONnum], SOEnum[2];
extern u16 SYS[SYSnum];


extern u16 RelayAutoReset;
extern u8 DisSOEbit, CommSOEbit, CommYK_cl, CommYK_op, NOSOEbit;
extern unchar Data[11], time[9], password[3], pCON[3];
extern unchar YXcheck[16], YKcheck[7], YKcheckOK;
extern u32 ARDyc;
extern u8 HZbit, ARDaction;
extern unchar  pFdata[17];
extern u16 ActionTime[20];
extern unchar XG, key2lx, OK, OKcancel;
extern u8 SOEdata[27];


extern unsigned char unit[7][4];


#endif

