
#ifndef __DW_MODBUS_TCP_H_
#define __DW_MODBUS_TCP_H_
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "typedef.h"


u16 TCP_Get_TXdata(void);

unchar TCP_ReceiveCheck(uint16_t len);


#endif
