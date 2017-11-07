/*********文件名：i2c_ee.h**********/

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __I2C_EE_H
#define __I2C_EE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "typedef.h"


/* Exported macro ------------------------------------------------------------*/
#define ADDR_24CXX        0xA2

#define  AT24C512 0xA0

#define SCLH         GPIOB->BSRR = GPIO_Pin_10
#define SCLL         GPIOB->BRR  = GPIO_Pin_10

#define SDAH         GPIOB->BSRR = GPIO_Pin_11
#define SDAL         GPIOB->BRR  = GPIO_Pin_11

#define SCLread      GPIOB->IDR  & GPIO_Pin_10
#define SDAread      GPIOB->IDR  & GPIO_Pin_11


/* Exported functions ------------------------------------------------------- */
void I2C_EE_Init(void);
uint8_t I2C_EE_BufferWrite(uint8_t *psrc_data, uint8_t adr, uint8_t nbyte);
uint8_t I2C_EE_BufferRead(uint8_t *pdin_data, uint8_t adr, uint8_t nbyte);


enum ENUM_TWI_REPLY
{
    TWI_NACK = 0
    , TWI_ACK = 1
};

enum ENUM_TWI_BUS_STATE
{
    TWI_READY = 0
    , TWI_BUS_BUSY = 1
    , TWI_BUS_ERROR = 2
};


void I2C_EE_Init(void);



#define DE 100
/**************************************************************************
延时
ms：延时的毫秒数
CYCLECOUNTER / 72000000
***************************************************************************/
void DelayMs(uint16_t ms);//for(iq1 = 11998; iq1 > 0; iq1--); // ( (6*iq1+9)*iq0+15 ) / 72000000


uint8_t TWI_Start(void);



void TWI_Stop(void);


void TWI_Ack(void);


void TWI_NoAck(void);


uint8_t TWI_WaitAck(void);   //返回为:=1有ACK,=0无ACK


void TWI_SendByte(uint8_t SendByte); //数据从高位到低位//



uint8_t TWI_ReceiveByte(void);  //数据从高位到低位//


uint8_t I2C_EE_BufferWrite(uint8_t *psrc_data, uint8_t adr, uint8_t nbyte); //写时间


uint8_t I2C_EE_BufferRead(uint8_t *pdin_data, uint8_t adr, uint8_t nbyte); //读时间





uint8_t I2C_EE_BufferRead_AT24C512(uint8_t *pdin_data, unint adr, uint8_t nbyte); //读芯片


u8 writeat24c(unint adr);


uint8_t I2C_EE_BufferWrite_AT24C512(uint8_t *psrc_data, unint adr, uint8_t nbyte); //写芯片


#endif /* __I2C_EE_H */

