#ifndef SOE_DUSTWIND_H
#define SOE_DUSTWIND_H

#include "stm32f10x.h"
#include "typedef.h"

void WSOE(void);
void DisSOE(u8 num);

void DisSOEU(u8 num);

void DisSOEI(u8 num);

extern u32 LightTime;
#endif

