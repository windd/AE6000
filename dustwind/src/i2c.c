

/*********文件名：i2c_ee.c**********/

#include "i2c.h"
#include "delay.h"



void I2C_EE_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // Configure I2C1 pins: SCL and SDA
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}



/**************************************************************************
延时
ms：延时的毫秒数
CYCLECOUNTER / 72000000
***************************************************************************/
void DelayMs(uint16_t ms)//for(iq1 = 11998; iq1 > 0; iq1--); // ( (6*iq1+9)*iq0+15 ) / 72000000
{
    while(ms)
    {
        Delay(11998);
        ms--;
    }
}

uint8_t TWI_Start(void)
{
    SDAH;
    SCLH;
    Delay(DE);
    if(!SDAread)return TWI_BUS_BUSY; //SDA线为低电平则总线忙,退出
    SDAL;
    Delay(DE);
    if(SDAread) return TWI_BUS_ERROR; //SDA线为高电平则总线出错,退出
    SCLL;
    Delay(DE);
    return TWI_READY;
}

/*void TWI_Stop(void)
{
 SCLL;
 TWI_delay();
 SDAL;
 TWI_delay();
 SCLH;
 TWI_delay();
 SDAH;
 TWI_delay();
}*/
void TWI_Stop(void)
{
    SDAL;
    SCLL;
    Delay(DE);
    SCLH;
    Delay(DE);
    SDAH;
    Delay(DE);
}

void TWI_Ack(void)
{
    SCLL;
    Delay(DE);
    SDAL;
    Delay(DE);
    SCLH;
    Delay(DE);
    SCLL;
    Delay(DE);
}

void TWI_NoAck(void)
{
    SCLL;
    Delay(DE);;
    SDAH;
    Delay(DE);
    SCLH;
    Delay(DE);
    SCLL;
    Delay(DE);
}

uint8_t TWI_WaitAck(void)   //返回为:=1有ACK,=0无ACK
{
    SCLL;
    Delay(DE);
    SDAH;
    Delay(DE);
    SCLH;
    Delay(DE);
    if(SDAread)
    {
        SCLL;
        return 0;
    }
    SCLL;
    return 1;
}

void TWI_SendByte(uint8_t SendByte) //数据从高位到低位//
{
    uint8_t i = 8;
    while(i--)
    {
        SCLL;
        Delay(DE);
        if(SendByte & 0x80)
            SDAH;
        else
            SDAL;
        SendByte <<= 1;
        Delay(DE);
        SCLH;
        Delay(DE);
    }
    SCLL;

}


uint8_t TWI_ReceiveByte(void)  //数据从高位到低位//
{
    uint8_t i = 8;
    uint8_t ReceiveByte = 0;

    SDAH;
    while(i--)
    {
        ReceiveByte <<= 1;
        SCLL;
        Delay(DE);
        SCLH;
        Delay(DE);
        if(SDAread)
        {
            ReceiveByte |= 0x01;
        }
    }
    SCLL;
    return ReceiveByte;
}
/***************************************************************************
向24c256中写多个字节
psrc_data：指向要写入数据数组的指针
adr：24c256中要写入数据的首地址
nbyte：写入的字节数
返回值:  0：执行完毕；1：执行出现错误
形参中：C02只有一个地址adr；C256中有高位地址hadr和低位地址ladr
***************************************************************************/
uint8_t I2C_EE_BufferWrite(uint8_t *psrc_data, uint8_t adr, uint8_t nbyte) //写时间
{
    uint8_t i;

    for(; nbyte != 0; nbyte--)
    {
        i = TWI_Start();
        if(i)
            return i;

        TWI_SendByte( ADDR_24CXX & 0xFE);//写器件地址

        if(!TWI_WaitAck())
        {
            TWI_Stop();
            return 0;
        }

        TWI_SendByte(adr);   //设置起始地址
        TWI_WaitAck();
        TWI_SendByte(*psrc_data);           //写数据
        TWI_WaitAck();
        psrc_data++;    //指向待写数据的指针加1
        adr++;    //对24C08的操作地址加1
        if(adr == 6)
        {
            adr++;
            psrc_data++;
        }
        TWI_Stop();
        //注意：因为这里要等待EEPROM写完，可以采用查询或延时方式(10ms)
        DelayMs(12); //写入延时 12ms  写周期大于10ms即可

    }
    return 0;
}


/***************************************************************************
从24c02读多个字节
pdin_data：指向要保存读出数据的数组的指针
adr：24c02中要读出数据的首地址
nbyte：读出的字节数
返回值:  0：执行完毕；1：执行出现错误
***************************************************************************/
uint8_t I2C_EE_BufferRead(uint8_t *pdin_data, uint8_t adr, uint8_t nbyte) //读时间
{
    uint8_t i;
    i = TWI_Start();
    if(i)
        return i;

    TWI_SendByte((ADDR_24CXX & 0xFE));//写器件地址，先执行一次伪写操作
    if(!TWI_WaitAck())
    {
        TWI_Stop();
        return 0;
    }

    TWI_SendByte(adr);
    TWI_WaitAck();
    TWI_Start();
    TWI_SendByte((ADDR_24CXX & 0xFE) | 0x01); //读器件地址    写入：地址最低位是0，读取：地址最低位是1
    TWI_WaitAck();

    while(nbyte != 1)               //读入前(nbyte-1)个字节
    {
        *pdin_data = TWI_ReceiveByte();
        Delay(DE);//循环从24C02中读数据，存入pdin_data所指的存储器中
        TWI_Ack();
        Delay(DE); //IIC应答
        pdin_data++;  //指向存储读入数据的存储器指针加1
        nbyte--;  //剩余要读入的字节减1
    };

    *pdin_data = TWI_ReceiveByte();  //读入最后一个字节
    TWI_NoAck();      //IIC无应答操作
    TWI_Stop();

    return 0;
}


uint8_t I2C_EE_BufferRead_AT24C512(uint8_t *pdin_data, unint adr, uint8_t nbyte) //读芯片
{
    uint8_t i;
    i = TWI_Start();
    if(i)
        return i;

    TWI_SendByte((AT24C512 & 0xFE));//写器件地址，先执行一次伪写操作
    if(!TWI_WaitAck())
    {
        TWI_Stop();
        return 0;
    }
    TWI_SendByte((unchar)((adr & 0xFF00) >> 8)); //设置起始地址
    TWI_WaitAck();
    TWI_SendByte((unchar)(adr & 0x00FF));   //设置起始地址

    TWI_WaitAck();
    TWI_Start();
    TWI_SendByte((AT24C512 & 0xFE) | 0x01);  //读器件地址    写入：地址最低位是0，读取：地址最低位是1
    TWI_WaitAck();

    while(nbyte != 1)               //读入前(nbyte-1)个字节
    {
        *pdin_data = TWI_ReceiveByte(); //循环从24C02中读数据，存入pdin_data所指的存储器中
        TWI_Ack();   //IIC应答
        pdin_data++;  //指向存储读入数据的存储器指针加1
        nbyte--;  //剩余要读入的字节减1
    };

    *pdin_data = TWI_ReceiveByte();  //读入最后一个字节
    TWI_NoAck();      //IIC无应答操作

    TWI_Stop();

    return 0;
}

u8 writeat24c(unint adr)
{
    uint8_t i;

    i = TWI_Start();
    if(i)return i;

    TWI_SendByte( 0xA0 & 0xFE);//写器件地址

    if(!TWI_WaitAck())
    {
        TWI_Stop();
        return 0;
    }
    TWI_SendByte((unchar)((adr & 0xFF00) >> 8)); //设置起始地址
    TWI_WaitAck();
    TWI_SendByte((unchar)(adr & 0x00FF));   //设置起始地址
    TWI_WaitAck();
    return 0;
}

uint8_t I2C_EE_BufferWrite_AT24C512(uint8_t *psrc_data, unint adr, uint8_t nbyte) //写芯片
{
    writeat24c(adr);
    for(; nbyte != 0; nbyte--)
    {
        if((adr % 64) == 0)
        {
            TWI_Stop();
            DelayMs(6);
            writeat24c(adr);
        }
        TWI_SendByte(*psrc_data);           //写数据
        TWI_WaitAck();
        psrc_data++;
        adr++;

    }
    TWI_Stop();
    DelayMs(6);
    return 0;
}

/*
uint8_t I2C_EE_BufferWrite_AT24C512(uint8_t *psrc_data,unint adr,uint8_t nbyte)
{
 uint8_t i;

for(;nbyte!=0;nbyte--)
{
i = TWI_Start();
      if(i)
   return i;

      TWI_SendByte( 0xA0 & 0xFE);//写器件地址

   if(!TWI_WaitAck())
   {
     TWI_Stop();
     return 0;
   }
 TWI_SendByte((unchar)((adr & 0xFF00)>>8));   //设置起始地址
 TWI_WaitAck();
TWI_SendByte((unchar)(adr & 0x00FF));   //设置起始地址

      TWI_WaitAck();
      TWI_SendByte(*psrc_data);           //写数据
      TWI_WaitAck();

      TWI_Stop();

              psrc_data++;    //指向待写数据的指针加1
adr++;    //对24C08的操作地址加1
//注意：因为这里要等待EEPROM写完，可以采用查询或延时方式(10ms)
      DelayMs(5); //写入延时 12ms  写周期大于10ms即可

}
return 0;
}
*/
/*
void TWI_24CXX_Write(uint8_t* pDat, uint8_t nAddr, uint8_t nLen)
{
  uint16_t i;
  for(i=0;i<nLen;i++)
  {
    TWI_WriteByte(*(pDat+i), nAddr+i);


  }
}

void TWI_24CXX_Read(uint8_t* pDat, uint8_t nAddr, uint8_t nLen)
{
    uint16_t i;
    for(i=0; i<nLen; i++)
      *(pDat+i) = TWI_ReadByte(nAddr+i);
}
*/


