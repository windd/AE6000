

#include "modbus.h"
#include "modbusTCP.h"
#include "w5500s.h"

u16 TCP_Get_TXdata(void)
{
    if(Rx_Buffer[6] != 1)return 0; //地址

    Modbus_Get_TXdata(Rx_Buffer + 7, Tx_Buffer + 7);

    Tx_Buffer[0] = Rx_Buffer[0];
    Tx_Buffer[1] = Rx_Buffer[1];
    Tx_Buffer[2] = Rx_Buffer[2];
    Tx_Buffer[3] = Rx_Buffer[3];
    Tx_Buffer[4] = Rx_Buffer[4];
    Tx_Buffer[5] = Tx_Buffer[8] + 3; //Modbus长度
    Tx_Buffer[6] = 0x1; //地址
    //	Tx_Buffer[7]=0x3;Tx_Buffer[8]=0x2;Tx_Buffer[9]=0x12;Tx_Buffer[10]++;// 功能\长度\数据
    return Tx_Buffer[5] + 6;
}


