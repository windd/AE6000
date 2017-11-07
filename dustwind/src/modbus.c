
#include "modbus.h"

#include "bianliang.h"
#include "dz.h"
#include "io.h"
#include "i2c.h"
void Modbus_Get_TXdata(unsigned char *RXdata, unsigned char *TXdata)
{
	TXdata[1]=0;
    switch(*RXdata) //03 00 00 00 01   //03 02 12 34
    {
    case 0x02://YX
        TXdata[1] = YX_data(RXdata, TXdata);
        break;
    case 0x03://YC
        TXdata[1] = YC_data(RXdata, TXdata);
        break;
    case 0x05://YK
        TXdata[1] = YK_data(RXdata, TXdata);
        break;
    case 0x06://RESET
        TXdata[1] = RESET_data(RXdata, TXdata);
        break;
    case 0x0c://SOE
        TXdata[1] = SOE_data(RXdata, TXdata);
        break;
    case 0x10://YT及校时
        TXdata[1] = YT_data(RXdata, TXdata);
        break;
    default:
        break;
    }
    TXdata[0] = RXdata[0];
}
u8 YC_data(unsigned char *RXdata, unsigned char *TXdata)
{
    u16 Saddr, num, i;
    Saddr = RXdata[1] * 0x100 + RXdata[2];
    num = RXdata[3] * 0x100 + RXdata[4];
    if(Saddr > 0x12B)      //控制字
    {
        Saddr -= 0x12c;
        //  if(Saddr + num > CONnum) return 0;
        for(i = 0; i < num; i++)
        {
            TXdata[2 + i * 2] = 0x00;
            TXdata[3 + i * 2] = CON[Saddr + i];
        }
    }
    else if(Saddr > 0xc7) //定值
    {
        Saddr -= 0xc8;
        //  if(Saddr + num > DZnum) return 0;
        for(i = 0; i < num; i++)
        {
            TXdata[2 + i * 2] = DZ[Saddr + i] >> 8;
            TXdata[3 + i * 2] = DZ[Saddr + i] & 0x00FF;
        }
    }
    else if(Saddr > 0x63) //系统参数
    {
        Saddr -= 0x64;
        //  if(Saddr + num > 12) return 0;
        for(i = 0; i < num; i++)
        {
            TXdata[2 + i * 2] = SYS[Saddr + i] >> 8;
            TXdata[3 + i * 2] = SYS[Saddr + i] & 0x00FF;
        }
    }
    else
    {
        //  if(Saddr + num > 21) return 0;
        for(i = 0; i < num; i++)
        {
            TXdata[2 + i * 2] = YC[Saddr + i] >> 8;
            TXdata[3 + i * 2] = YC[Saddr + i] & 0x00FF;
        }
    }
    return num * 2;
}
u8 YX_data(unsigned char *RXdata, unsigned char *TXdata)
{
    u16 Saddr, num, i;

    Saddr = RXdata[1] * 0x100 + RXdata[2];
    num = (RXdata[3] * 0x100 + RXdata[4]) / 8;
    if((RXdata[3] * 0x100 + RXdata[4]) % 8 != 0) num++;
    // if(Saddr + num > 4)return 0;


    for(i = 0; i < num; i++)
    {
        TXdata[2 + i] = YX[Saddr + i];
    }
    return num;
}
u8 SOE_data(unsigned char *RXdata, unsigned char *TXdata)
{
    u8 i;
    if(CommSOEbit == 1)
    {
        for(i = 0; i < 15; i++)
            TXdata[2 + i] = SOEdata[i];
        //        CommSOEbit = 0;
        //        YX[3] &= 0x7F;
        return 0x0f;
    }
    return 0;
}
u8 RESET_data(unsigned char *RXdata, unsigned char *TXdata)
{
    u8 i;
    RESET_KEY();
    for(i = 0; i < 5; i++)
        TXdata[i] = TXdata[i];
    return 5;
}
u8 YT_data(unsigned char *RXdata, unsigned char *TXdata)
{
    u16 Saddr, num, i;
    unchar DT[9];
    Saddr = RXdata[2] * 0x100 + RXdata[3];
    num = RXdata[4] * 0x100 + RXdata[5];

    if(Saddr > 0x12B) //控制字
    {
        Saddr -= 0x12c;
        if(Saddr + num > CONnum) return 0;
        for(i = 0; i < num; i++)
        {
            if(RXdata[8 + 2 * i] == 0xAA || RXdata[8 + 2 * i] == 0xCC )
                CON[Saddr + i] = RXdata[8 + 2 * i];
        }
        I2C_EE_BufferWrite_AT24C512(&CON[Saddr], Saddr + 90, num);
    }
    else if(Saddr > 0xc7) //定值
    {
        Saddr -= 0xc8;
        if(Saddr + num > DZnum) return 0;

        for(i = 0; i < num; i++)
        {
            DZ[Saddr + i] = RXdata[7 + i * 2] * 0x100 + RXdata[8 + i * 2];
        }
        I2C_EE_BufferWrite_AT24C512(&RXdata[7], (Saddr) * 2 + 30, RXdata[6]);
    }
    else if(Saddr > 0x63) //系统参数
    {
        Saddr -= 0x64;
        if(Saddr + num > SYSnum) return 0;
        for(i = 2; i < num; i++)
        {
            SYS[Saddr + i] = RXdata[7 + i * 2] * 0x100 + RXdata[8 + i * 2];
        }
        I2C_EE_BufferWrite_AT24C512(&RXdata[7], (Saddr + 2) * 2 + 1, RXdata[6]);
    }
    else
    {
        if(Saddr != 0 && num != 6) return 0; //时间
        DT[2] = RXdata[8];
        DT[3] = RXdata[10];
        DT[4] = RXdata[12];
        DT[5] = RXdata[14];
        //DT[6]=0;
        DT[7] = RXdata[16];
        DT[8] = RXdata[18];
        I2C_EE_BufferWrite(&DT[2], 0x02, 0x06);
    }
    for(i = 0; i < 6; i++)//???
        TXdata[i] = RXdata[i];
    return 0;
}
u8 YK_data(unsigned char *RXdata, unsigned char *TXdata)
{
    u8 i;
    if(RXdata[5] == 0xFF && RXdata[3] == 0x01)
    {
        CommYK_cl = 1;
        YH_Relay_cl;
        RelayAutoReset |= 0x01;
    }
    if(RXdata[5] == 0xFF && RXdata[3] == 0x02)
    {
        CommYK_cl = 1;
        YT_Relay_cl;
        RelayAutoReset |= 0x02;
    }
    if(RXdata[5] == 0xFF && RXdata[3] == 0x03)
    {
        CommYK_cl = 1;
        //       GZ_Relay_cl;
        RelayAutoReset |= 0x04;
    }
    if(RXdata[5] == 0xFF && RXdata[3] == 0x04)
    {
        CommYK_cl = 1;
        //        SG_Relay_cl;
        RelayAutoReset |= 0x08;
    }
    if(CommYK_cl)
    {
        for(i = 0; i < 5; i++)
            TXdata[i] = RXdata[i];
        CommYK_cl = 0;
        return 5;
    }
    return 0;
}

