

#ifndef __DW_PROTECTION_H_
#define __DW_PROTECTION_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "typedef.h"


extern u16 ProtectionBit;





void protection_Init(void);

void Pro_StartTIM2(u8 n);

void px(int16_t *max, int16_t *min, int16_t *data);

void pxI(int16_t *max, int16_t *min, int16_t *p, int16_t *data);


void IOprotection(void);

void TZ_out(u8 n, int16_t data, u8 CO);
void TZ_Action(void);
void TZ_Reset(void);

void protection(void);



#endif

