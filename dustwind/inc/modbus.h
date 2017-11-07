
#ifndef __DW_MODBUS_H_
#define __DW_MODBUS_H_
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "typedef.h"

void Modbus_Get_TXdata(unsigned char *RXdata, unsigned char *TXdata);

u8 YC_data(unsigned char *RXdata, unsigned char *TXdata);

u8 YX_data(unsigned char *RXdata, unsigned char *TXdata);

u8 SOE_data(unsigned char *RXdata, unsigned char *TXdata);

u8 RESET_data(unsigned char *RXdata, unsigned char *TXdata);

u8 YT_data(unsigned char *RXdata, unsigned char *TXdata);

u8 YK_data(unsigned char *RXdata, unsigned char *TXdata);


#endif
