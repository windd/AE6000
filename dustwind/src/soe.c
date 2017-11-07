
#include "soe.h"
#include "lcd.h"
#include "bianliang.h"
#include "menu_content.h"
#include "i2c.h"
#include "dz.h"

u32 LightTime;
void WSOE()
{
    u8 DT[9];
    SOEdata[15] = (YC[0] & 0xff00) >> 8;
    SOEdata[16] = YC[0] & 0x00ff;
    SOEdata[17] = (YC[1] & 0xff00) >> 8;
    SOEdata[18] = YC[1] & 0x00ff;
    SOEdata[19] = (YC[2] & 0xff00) >> 8;
    SOEdata[20] = YC[2] & 0x00ff;
    SOEdata[21] = (YC[9] & 0xff00) >> 8;
    SOEdata[22] = YC[9] & 0x00ff;
    SOEdata[23] = (YC[10] & 0xff00) >> 8;
    SOEdata[24] = YC[10] & 0x00ff;
    SOEdata[25] = (YC[11] & 0xff00) >> 8;
    SOEdata[26] = YC[11] & 0x00ff;
    I2C_EE_BufferRead(DT, 0x00, 0x09);
    SOEdata[0] = DT[8];
    SOEdata[1] = DT[7] & 0x1f;
    SOEdata[2] = DT[5] & 0x3f;
    SOEdata[3] = DT[4] & 0x3f;
    SOEdata[4] = DT[3] & 0x7f;
    SOEdata[5] = DT[2] & 0x7f;
    SOEnum[0] = SOEnum[0] < 64 ? SOEnum[0] + 1 : 64;
    SOEnum[1] = SOEnum[1] < 64 ? SOEnum[1] + 1 : 1;
    I2C_EE_BufferWrite_AT24C512(SOEnum, SoeStAddr, 2);
    I2C_EE_BufferWrite_AT24C512(SOEdata, SoeStAddr + SOEnum[1] * 27 - 25, 27);
    CommSOEbit = 1;
    YC[3] |= 0x80;
    if(NOSOEbit == 0)
    {
        DisSOEbit = 0;
        DisSOE(111);
    }

}

void DisSOE(u8 num)
{
    u8 i, Rsoe[27], disdata[20], *Rsoedata;
    int16_t yc, kao;
    LightTime = 0;
    LIGHT_ON;
    LCDclear();
    if(SOEnum[0] == 0) //无SOE
    {
        disdata[0] = 1;
        disdata[1] = 64;
        LCDWword(1, 0, 0, disdata);
        return;
    }

    if(num == 111)       //显示当前
    {
        disdata[1] = SOEnum[1] / 10;
        disdata[2] = SOEnum[1] % 10; //编号
        Rsoedata = SOEdata;
    }
    else               //显示历史
    {
        disdata[1] = num / 10;
        disdata[2] = num % 10; //编号
        if(SOEnum[1] >= num)
            i = SOEnum[1] - num;
        else
            i = 64 + SOEnum[1] - num;
        I2C_EE_BufferRead_AT24C512(Rsoe, SoeStAddr + i * 27 + 2, 27);
        Rsoedata = Rsoe;
    }
    for(i = 0; i < 8; i++)
    {
        disdata[i + 3] = 31;
    }
    for(i = 0; i < 3; i++)
    {
        disdata[11 + i * 3] = (Rsoedata[i] & 0xf0) >> 4;
        disdata[12 + i * 3] = Rsoedata[i] & 0xf;
        disdata[13 + i * 3] = 10;
    }
    disdata[0] = 18;
    LCDWchar(1, 0, 0, disdata); // 日期

    for(i = 0; i < 4; i++)
    {
        disdata[i * 3 + 1] = (Rsoedata[i + 3] & 0xf0) >> 4;
        disdata[i * 3 + 2] = Rsoedata[i + 3] & 0xf;
        disdata[i * 3 + 3] = 10;
    }
    disdata[0] = 11;
    LCDWchar(2, 10, 0, disdata); //时间


    if(Rsoedata[8] == 2) //保护
    {
        i = 0;
        switch(Rsoedata[14])
        {
        case Property_I:   //I U I0 U0 I2 U2  F
            disdata[0] = 10;
            disdata[1] = 11;
            disdata[10] = 12;
            i = 2;
            break;
        case Property_U:
            disdata[0] = 10;
            disdata[1] = 15;
            disdata[10] = 16;
            i = 2;
            break;
        case Property_I0:
            disdata[0] = 11;
            disdata[1] = 11;
            disdata[2] = 0;
            disdata[11] = 12;
            i = 3;
            break;
        case Property_U0:
            disdata[0] = 11;
            disdata[1] = 15;
            disdata[2] = 0;
            disdata[11] = 16;
            i = 3;
            break;
        case Property_I2:
            disdata[0] = 11;
            disdata[1] = 11;
            disdata[2] = 2;
            disdata[11] = 12;
            i = 3;
            break;
        case Property_U2:    //U2
            disdata[0] = 11;
            disdata[1] = 15;
            disdata[2] = 2;
            disdata[11] = 16;
            i = 3;
            break;
        }
        disdata[i] = 28;
        yc = Rsoedata[12] * 0x100 + Rsoedata[13];
        disdata[i + 1] = 31;
        kao = yc;
        if(kao < 0)kao = 0 - kao;
        disdata[i + 2] = kao / 10000;
        disdata[i + 2] = disdata[i + 2] == 0 ? 31 : disdata[i + 2];
        disdata[i + 3] = (kao % 10000) / 1000;
        if(disdata[i + 2] == 31)
            disdata[i + 3] = disdata[i + 3] == 0 ? 31 : disdata[i + 3];
        disdata[i + 4] = (kao % 1000) / 100;
        disdata[i + 5] = 10;
        disdata[i + 6] = (kao % 100) / 10;
        disdata[i + 7] = kao % 10;
        if(yc < 0)
        {
            if(disdata[i + 3] == 31)
                disdata[i + 3] = 32;
            else if(disdata[4] == 31)
                disdata[i + 1] = 32;
            else
                disdata[i + 1] = 32;
        }

        i = Rsoedata[9];
        LCDWword(3, 0, 0, &menuID1112[Prot_sm[i]][0]);
        // if(i==9 || i==10)return;
        LCDWchar(4, 5, 0, disdata);
        return;


        /*
        switch(Rsoedata[14])   //保护类型
        {
        case 0:
          disdata[1]=11;break;
        case 1:
          disdata[1]=15;break;
        case 2:
          disdata[1]=17;break;
        case 3:
          disdata[1]=30;break;
        }
        LCDWword(4,0,0,&menuID1111[Prot_sm[Rsoedata[9]]][0]);  //保护名字
        disdata[0]=9;
        LCDWchar(4,10,0,disdata);                  //动作数据
        LCDWchar(4,19,0,&DZunit[Prot_sm[Rsoedata[9]]][0]);   //单位
        return;
        */
    }
    if(Rsoedata[8] == 1) //YX
    {
        if(Rsoedata[9] == 15) //控制回路断线
        {
            disdata[5] = 82;
            disdata[6] = 57;
            disdata[0] = 6;
            disdata[1] = 19, disdata[2] = 36;
            disdata[3] = 98;
            disdata[4] = 99;
            LCDWword(4, 2, 0, disdata);
            return;
        }
        switch(Rsoedata[9])    //开入1-8
        {
        case 0:
            disdata[1] = 35, disdata[2] = 100;
            disdata[3] = 94;
            disdata[4] = 33;
            break;
        case 1:
            disdata[1] = 34, disdata[2] = 100;
            disdata[3] = 94;
            disdata[4] = 33;
            break;
        case 2:
            disdata[1] = 95, disdata[2] = 96;
            disdata[3] = 97;
            disdata[4] = 68;
            break;
        case 3:
            disdata[1] = 132, disdata[2] = 130;
            disdata[3] = 131;
            disdata[4] = 68;
            break;
        case 4:
            disdata[1] = 126, disdata[2] = 130;
            disdata[3] = 131;
            disdata[4] = 68;
            break;
        case 5:
            disdata[1] = 52, disdata[2] = 137;
            disdata[3] = 138;
            disdata[4] = 139;
            break;
        case 6:
            disdata[1] = 133, disdata[2] = 134;
            disdata[3] = 135;
            disdata[4] = 136;
            break;
        case 7:
            disdata[1] = 133, disdata[2] = 134;
            disdata[3] = 51;
            disdata[4] = 136;
            break;
        }
        disdata[5] = 68;
        disdata[6] = 68;
        disdata[8] = 101;
        if(Rsoedata[12])
        {
            disdata[7] = 34;
            disdata[9] = 35;
        }
        else
        {
            disdata[7] = 35;
            disdata[9] = 34;
        }
        disdata[0] = 9;
        LCDWword(4, 0, 0, disdata);
        //if(Rsoedata[9]>1)
        // {
        //  disdata[0]=1,disdata[1]=Rsoedata[9]+1;
        //  LCDWchar(4,5,0,disdata);
        //}
    }

}
void INTtoBIT(int16_t i, u8 *b)
{
    int16_t kao;
    kao = i;
    if(kao < 0)kao = 0 - kao;
    b[0] = kao / 10000;
    b[0] = b[0] == 0 ? 31 : b[0];
    b[1] = (kao % 10000) / 1000;
    if(b[0] == 31)
        b[1] = b[1] == 0 ? 31 : b[1];
    b[2] = (kao % 1000) / 100;
    b[3] = 10;
    b[4] = (kao % 100) / 10;
    b[5] = kao % 10;
}
void DisSOEU(u8 num)
{
    u8 i, Rsoe[27], disdata[20], *Rsoedata;
    LCDclear();
    int16_t yc;
    disdata[0] = 2;
    disdata[1] = num / 10;
    disdata[2] = num % 10; //编号
    LCDWchar(1, 0, 0, disdata);
    if(SOEnum[1] >= num)
        i = SOEnum[1] - num;
    else
        i = 30 + SOEnum[1] - num;
    I2C_EE_BufferRead_AT24C512(Rsoe, SoeStAddr + i * 27 + 2, 27);
    Rsoedata = Rsoe;


    disdata[1] = 15;
    disdata[2] = 12;
    disdata[3] = 13;
    disdata[4] = 28;
    disdata[11] = 16;
    disdata[0] = 11;

    yc = Rsoedata[21] * 0x100 + Rsoedata[22];
    INTtoBIT(yc, &disdata[5]);
    LCDWchar(2, 4, 0, disdata);
    disdata[2] = 13;
    disdata[3] = 14;
    yc = Rsoedata[23] * 0x100 + Rsoedata[24];
    INTtoBIT(yc, &disdata[5]);
    LCDWchar(3, 4, 0, disdata);
    disdata[2] = 14;
    disdata[3] = 12;
    yc = Rsoedata[25] * 0x100 + Rsoedata[26];
    INTtoBIT(yc, &disdata[5]);
    LCDWchar(4, 4, 0, disdata);

}
void DisSOEI(u8 num)
{
    u8 i, Rsoe[27], disdata[20], *Rsoedata;
    int16_t yc;
    LCDclear();

    disdata[0] = 2;
    disdata[1] = num / 10;
    disdata[2] = num % 10; //编号
    LCDWchar(1, 0, 0, disdata);
    if(SOEnum[1] >= num)
        i = SOEnum[1] - num;
    else
        i = 30 + SOEnum[1] - num;
    I2C_EE_BufferRead_AT24C512(Rsoe, SoeStAddr + i * 27 + 2, 27);
    Rsoedata = Rsoe;

    disdata[1] = 11;
    disdata[2] = 12;
    disdata[3] = 28;
    disdata[10] = 12;
    disdata[0] = 10;

    yc = Rsoedata[15] * 0x100 + Rsoedata[16];
    INTtoBIT(yc, &disdata[4]);
    LCDWchar(2, 4, 0, disdata);
    disdata[2] = 13;
    yc = Rsoedata[17] * 0x100 + Rsoedata[18];
    INTtoBIT(yc, &disdata[4]);
    LCDWchar(3, 4, 0, disdata);
    disdata[2] = 14;
    yc = Rsoedata[19] * 0x100 + Rsoedata[20];
    INTtoBIT(yc, &disdata[4]);
    LCDWchar(4, 4, 0, disdata);
}

