
#ifndef __DW_MODBUS_H_
#define __DW_MODBUS_H_
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "typedef.h"


extern unchar ReceiveData[512];
extern unchar SendData[512];
#define  DIR485_Send GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define  DIR485_Receive  GPIO_ResetBits(GPIOA, GPIO_Pin_8)
void CRC16(unsigned char *Databuf, unsigned short DataLen, unchar *RCRHi, unchar *CRCLo);
void Modbus_init(void);

void DMA1_Channel5_IRQHandler(void);

void RS485_Send(u16 len);

u16 YC_data(void);


u16 YX_data(void);

u16 SOE_data(void);

u16 RESET_data(void);

u16 YT_data(void);

u16 YK_data(void);


unchar ReceiveCheck(uint16_t len);


void CRC16(unsigned char *Databuf, unsigned short DataLen, unchar *RCRHi, unchar *CRCLo);


void USART1_IRQHandler(void);

#endif
