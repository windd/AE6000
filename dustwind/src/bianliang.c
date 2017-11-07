
#include "bianliang.h"


int16_t YC[YCnum] = {0};
int16_t PhaseAngle[YCnum] = {0};
structmenu currmenu;
unchar YX[YXnum] = {0, 0, 0, 0};
u16 DZ[DZnum] = {0, 0, 0, 0, 0, 0};
unchar CON[CONnum], SOEnum[2];
u16 SYS[SYSnum] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


u16 RelayAutoReset = 0;
u8 DisSOEbit = 1, CommSOEbit = 0, CommYK_cl = 0, CommYK_op = 0, NOSOEbit = 0;
unchar Data[11] = {10, 0, 0, 0, 0, 10, 0, 0, 10, 0, 0}, time[9] = {8, 0, 0, 10, 0, 0, 10, 0, 0}, password[3] = {2, 0, 0}, pCON[3];
unchar YXcheck[16] = {15, 7, 31, 6, 31, 5, 31, 4, 31, 3, 31, 2, 31, 1, 31, 0}, YKcheck[7] = {2, 0, 1, 1, 35, 68, 35}, YKcheckOK = 0;
u32 ARDyc;
u8 HZbit, ARDaction;
unchar  pFdata[17];
u16 ActionTime[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
unchar XG = 0, key2lx, OK, OKcancel;
u8 SOEdata[27] = {0};

unsigned char unit[7][4] = //A.0 /V.1 /W.2 /VAR.3 /Пе.4 /HZ.5 /S.6
{
    {1, 12},
    {1, 16},
    {1, 21}, {3, 16, 26, 27}, {0}, {2, 18, 19}, {1, 25},
};

u8 YCunit[YCnum] =
{
    UNIT_I, UNIT_I, UNIT_I, UNIT_I, UNIT_I, UNIT_I,
    UNIT_U, UNIT_U, UNIT_U, UNIT_U, UNIT_U, UNIT_U,
    UNIT_I, UNIT_U, UNIT_I, UNIT_U, UNIT_I, UNIT_U,
    UNIT_F, UNIT_P, UNIT_Q, UNIT__
};
//Password 0 SYS 1 DZ 10 CON 90  SOE 150

