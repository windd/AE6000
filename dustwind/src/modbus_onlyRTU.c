
#include "modbus.h"

#include "bianliang.h"
#include "i2c.h"
#include "dz.h"
#include "io.h"


unchar ReceiveData[512];
unchar SendData[512];


void Modbus_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
    USART_InitTypeDef USART_InitStructure;
    if(SYS[1] == 2400 || SYS[1] == 4800 || SYS[1] == 9600 || SYS[1] == 19200)
        USART_InitStructure.USART_BaudRate = SYS[1];
    else
        USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    // USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    //USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
    USART_ITConfig(USART1, USART_IT_IDLE , ENABLE);//开启空闲,帧错,噪声,校验错中断

    USART_Cmd(USART1, ENABLE);

    USART_ClearFlag(USART1, USART_FLAG_TC);


    DMA_InitTypeDef DMA_InitStructure;
    /* DMA clock enable */
    // RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA1

    /* DMA1 Channel5 (triggered by USART1 Rx event) Config */
    DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40013804;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ReceiveData[0];
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 512;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);

    DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
    DMA_ITConfig(DMA1_Channel5, DMA_IT_TE, ENABLE);

    /* Enable USART2 DMA RX request */
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

    DMA_Cmd(DMA1_Channel5, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure one bit for preemption priority */
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //Enable DMA Channel4 Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



void DMA1_Channel5_IRQHandler(void)
{
    DMA_ClearITPendingBit(DMA1_IT_TC5);
    DMA_ClearITPendingBit(DMA1_IT_TE5);
    DMA_Cmd(DMA1_Channel5, DISABLE);//关闭DMA,防止处理其间有数据
    DMA1_Channel5->CNDTR = 512;//重装填
    DMA_Cmd(DMA1_Channel5, ENABLE);//处理完,重开DMA
}
void RS485_Send(u16 len)
{
    DIR485_Send;//485发送打开
    for(int i = 0; i < len; i++)
    {
        USART_SendData(USART1, SendData[i]);
        //Delay(10000);//此句非常重要，不知道是我这个简易模块的问题，还是485通信速度的问题，删除就出错。
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//USART_FLAG_TC
}
u16 YC_data()
{
    u16 Saddr, num, i;
    Saddr = ReceiveData[2] * 0x100 + ReceiveData[3];
    num = ReceiveData[4] * 0x100 + ReceiveData[5];
    SendData[0] = SYS[0];
    SendData[1] = 3;
    SendData[2] = num * 2;
    if(Saddr > 0x12B)      //控制字
    {
        Saddr -= 0x12c;
        //  if(Saddr + num > CONnum) return 0;
        for(i = 0; i < num; i++)
        {
            SendData[3 + i * 2] = 0x00;
            SendData[4 + i * 2] = CON[Saddr + i];
        }
    }
    else if(Saddr > 0xc7) //定值
    {
        Saddr -= 0xc8;
        //  if(Saddr + num > DZnum) return 0;
        for(i = 0; i < num; i++)
        {
            SendData[3 + i * 2] = DZ[Saddr + i] >> 8;
            SendData[4 + i * 2] = DZ[Saddr + i] & 0x00FF;
        }
    }
    else if(Saddr > 0x63) //系统参数
    {
        Saddr -= 0x64;
        //  if(Saddr + num > 12) return 0;
        for(i = 0; i < num; i++)
        {
            SendData[3 + i * 2] = SYS[Saddr + i] >> 8;
            SendData[4 + i * 2] = SYS[Saddr + i] & 0x00FF;
        }
    }
    else
    {
        //  if(Saddr + num > 21) return 0;
        for(i = 0; i < num; i++)
        {
            SendData[3 + i * 2] = YC[Saddr + i] >> 8;
            SendData[4 + i * 2] = YC[Saddr + i] & 0x00FF;
        }
    }

    CRC16(&SendData[0], 3 + i * 2, &SendData[3 + i * 2], &SendData[4 + i * 2]);
    return 5 + i * 2;
}
u16 YX_data()
{
    u16 Saddr, num, i;

    Saddr = ReceiveData[2] * 0x100 + ReceiveData[3];
    num = (ReceiveData[4] * 0x100 + ReceiveData[5]) / 8;
    if((ReceiveData[4] * 0x100 + ReceiveData[5]) % 8 != 0) num++;
    // if(Saddr + num > 4)return 0;
    SendData[0] = SYS[0];
    SendData[1] = 2;
    SendData[2] = num;
    for(i = 0; i < num; i++)
    {
        SendData[3 + i] = YX[Saddr + i];
    }
    CRC16(&SendData[0], 3 + i, &SendData[3 + i], &SendData[4 + i]);
    return 5 + i;
}
u16 SOE_data()
{
    u8 i;
    SendData[0] = SYS[0];
    SendData[1] = 0x0c;
    if(CommSOEbit == 1)
    {
        SendData[2] = 0x0f;
        for(i = 0; i < 15; i++)
            SendData[3 + i] = SOEdata[i];
        CRC16(SendData, 18, &SendData[18], &SendData[19]);
        CommSOEbit = 0;
        YX[3] &= 0x7F;
        return 20;
    }
    else
    {
        SendData[2] = 0x00;
        CRC16(&SendData[0], 3, &SendData[3], &SendData[4]);
        return 5;
    }
}
u16 RESET_data()
{
    u8 i;
    RESET_KEY();
    for(i = 0; i < 8; i++)
        SendData[i] = ReceiveData[i];
    return 8;
}
u16 YT_data()
{
    u16 Saddr, num, i;
    unchar DT[9];
    Saddr = ReceiveData[2] * 0x100 + ReceiveData[3];
    num = ReceiveData[4] * 0x100 + ReceiveData[5];

    if(Saddr > 0x12B) //控制字
    {
        Saddr -= 0x12c;
        if(Saddr + num > CONnum) return 0;
        for(i = 0; i < num; i++)
        {
            if(ReceiveData[8 + 2 * i] == 0xAA || ReceiveData[8 + 2 * i] == 0xCC )
                CON[Saddr + i] = ReceiveData[8 + 2 * i];
        }
        I2C_EE_BufferWrite_AT24C512(&CON[Saddr], Saddr + 90, num);
    }
    else if(Saddr > 0xc7) //定值
    {
        Saddr -= 0xc8;
        if(Saddr + num > DZnum) return 0;

        for(i = 0; i < num; i++)
        {
            DZ[Saddr + i] = ReceiveData[7 + i * 2] * 0x100 + ReceiveData[8 + i * 2];
        }
        I2C_EE_BufferWrite_AT24C512(&ReceiveData[7], (Saddr) * 2 + 30, ReceiveData[6]);
    }
    else if(Saddr > 0x63) //系统参数
    {
        Saddr -= 0x64;
        if(Saddr + num > SYSnum) return 0;
        for(i = 2; i < num; i++)
        {
            SYS[Saddr + i] = ReceiveData[7 + i * 2] * 0x100 + ReceiveData[8 + i * 2];
        }
        I2C_EE_BufferWrite_AT24C512(&ReceiveData[7], (Saddr + 2) * 2 + 1, ReceiveData[6]);
    }
    else
    {
        if(Saddr != 0 && num != 6) return 0; //时间
        DT[2] = ReceiveData[8];
        DT[3] = ReceiveData[10];
        DT[4] = ReceiveData[12];
        DT[5] = ReceiveData[14];
        //DT[6]=0;
        DT[7] = ReceiveData[16];
        DT[8] = ReceiveData[18];
        I2C_EE_BufferWrite(&DT[2], 0x02, 0x06);
    }
    for(i = 0; i < 6; i++)
        SendData[i] = ReceiveData[i];
    CRC16(&SendData[0], 6, &SendData[6], &SendData[7]);
    return 8;
}
u16 YK_data()
{
    u8 i;
    if(ReceiveData[5] == 0xFF && ReceiveData[3] == 0x01)
    {
        CommYK_cl = 1;
        YH_Relay_cl;
        RelayAutoReset |= 0x01;
    }
    if(ReceiveData[5] == 0xFF && ReceiveData[3] == 0x02)
    {
        CommYK_cl = 1;
        YT_Relay_cl;
        RelayAutoReset |= 0x02;
    }
    if(ReceiveData[5] == 0xFF && ReceiveData[3] == 0x03)
    {
        CommYK_cl = 1;
        //       GZ_Relay_cl;
        RelayAutoReset |= 0x04;
    }
    if(ReceiveData[5] == 0xFF && ReceiveData[3] == 0x04)
    {
        CommYK_cl = 1;
        //        SG_Relay_cl;
        RelayAutoReset |= 0x08;
    }
    if(CommYK_cl)
    {
        for(i = 0; i < 8; i++)
            SendData[i] = ReceiveData[i];
        CommYK_cl = 0;
        return 8;
    }
    else
        return 0;
}
unchar ReceiveCheck(uint16_t len)
{
    unchar CRCHi, CRCLo;
    if(ReceiveData[0] != SYS[0])return 0; //地址
    len = 512 - DMA_GetCurrDataCounter(DMA1_Channel5);
    CRC16(&ReceiveData[0], len - 2, &CRCHi, &CRCLo);
    if(CRCHi != ReceiveData[len - 2] || CRCLo != ReceiveData[len - 1]) return 0;
    return 1;
}
const unsigned char auchCRCHi[256] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};
/*CRC低位字节值表*/
const char auchCRCLo[256] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};
/*要进行CRC校验的消息*//*消息中字节数*/
void CRC16(unsigned char *Databuf, unsigned short DataLen, unchar *RCRHi, unchar *CRCLo)
{
    unsigned char uchCRCHi = 0xFF; /*高CRC字节初始化*/
    unsigned char uchCRCLo = 0xFF; /*低CRC字节初始化*/
    unsigned uIndex;/*CRC循环中的索引*/
    while(DataLen--)/*传输消息缓冲区*/
    {
        uIndex = uchCRCHi^*Databuf++; /*计算CRC*/
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
        uchCRCLo = auchCRCLo[uIndex];
    }
    *RCRHi = uchCRCHi;
    *CRCLo = uchCRCLo;
}

void USART1_IRQHandler(void)
{
    u16 DATA_LEN;
    COMM_LED_ON;
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        DMA_Cmd(DMA1_Channel5, DISABLE);//关闭DMA,防止处理其间有数据
        DATA_LEN = 512 - DMA_GetCurrDataCounter(DMA1_Channel5);
        if(ReceiveCheck(DATA_LEN))
        {
            switch(ReceiveData[1])
            {
            case 0x02://YX
                DATA_LEN = YX_data();
                break;
            case 0x03://YC
                DATA_LEN = YC_data();
                break;
            case 0x05://YK
                DATA_LEN = YK_data();
                break;
            case 0x06://RESET
                DATA_LEN = RESET_data();
                break;
            case 0x0c://SOE
                DATA_LEN = SOE_data();
                break;
            case 0x10://YT及校时
                DATA_LEN = YT_data();
                break;
            }
            if(DATA_LEN)RS485_Send(DATA_LEN);
        }
        DIR485_Receive;//485发送完，就必须打开        485接收
        DMA_ClearFlag(DMA1_FLAG_GL5 | DMA1_FLAG_TC5 | DMA1_FLAG_TE5 | DMA1_FLAG_HT5);//清标志
        DMA1_Channel5->CNDTR = 512;//重装填
        DMA_Cmd(DMA1_Channel5, ENABLE);//处理完,重开DMA
        //读SR后读DR清除Idle
        DATA_LEN = USART1->SR;
        DATA_LEN = USART1->DR;
    }
    if(USART_GetITStatus(USART1, USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET)//出错
    {
        USART_ClearITPendingBit(USART1, USART_IT_PE | USART_IT_FE | USART_IT_NE);
    }
    USART_ClearITPendingBit(USART1, USART_IT_IDLE);
    COMM_LED_OFF;
}

