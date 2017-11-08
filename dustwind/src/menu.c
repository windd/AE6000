
#include "menu.h"
#include "menu_content.h"
#include "lcd.h"
#include "bianliang.h"
#include "dz.h"
#include "i2c.h"
#include "yc.h"
#include "charlib.h"
#include "io.h"
#include "soe.h"





void DisMainMenu0()
{
    currmenu = MENU[0];
    LCDWword(1, 2, 0, &mainmenu[0][0]);
    LCDWchar(2, 0, 0, &mainmenu[1][0]);
    LCDWchar(3, 0, 0, &mainmenu[2][0]);
    LCDWchar(4, 0, 0, &mainmenu[3][0]);
    currmenu.cur = 1;
}

void DisBBH()
{
	//LCDWword(1,0,0,menuID16[0]);
    
   // LCDWword(2,0,0,menuID16[1]);
   // LCDWchar(2,9,0,menuID16[2]);
    
   // LCDWword(3,0,0,menuID16[3]);
   // LCDWchar(3,9,0,menuID16[4]);
	
    LCDWword(1, 2, 0, menuID16[0]);
    LCDWchar(2, 6, 0, menuID16[1]);
    LCDWchar(3, 7, 0, menuID16[2]);
    LCDWword(4, 2, 0, menuID16[3]);
}

void Dis_num(int16_t y, int16_t x, short num, char xsd)
{
    unsigned char i = 2, data[9];

    data[0] = 7;
    if(num < 0)
    {
        data[1] = 32;
        num = 0 - num;
    }
    else data[1] = 31;

    data[i++] = num / 10000;
    data[i++] = (num % 10000) / 1000;
    if(xsd == 4)
    {
        data[i++] = 10;
    }
    data[i++] = (num % 1000) / 100;
    if(xsd == 3)
    {
        data[i++] = 10;
    }
    data[i++] = (num % 100) / 10;
    if(xsd == 2)
    {
        data[i++] = 10;
    }
    data[i++] = num % 10;
    data[i++] = 31;

    LCDWchar(y, x, 0, data); //YC
}

void Dis_YC_Temp(int16_t y, int16_t x, unsigned char   *xh, char len)
{
    unchar i, b;
    unchar dh[2] = {1, 33};
    for(i = 0; i < len; i++)
    {
        x = mainmenu[currmenu.cur + i][0];
        if(currmenu.cur == 26)
            b = 2;
        else
            b = 3;
        if(currmenu.cur + i == 24)
            b = 4;
        else
            b = 3;
        Dis_num(y + i, x, YC[*(xh + i)], b); //YC
        LCDWchar(y + i, x + 7, 0, &unit[YCunit[*(xh + i)]][0]); //UNIT
        if(len == 3)
        {
            Dis_num(y + i, x + 9, PhaseAngle[*(xh + i)], 2); //角度
            LCDWchar(y + i, x + 16, 0, dh); //。号
        }
    }
}

void DisYC()
{
    //  int32_t kao;
    //  int32_t YCDisData = 0;
    //  unchar YCbit[9], b, a;
    //  unchar unit[6];

    unchar Dis_YC_Content1[3] = {YC_MIA , YC_MIB , YC_MIC};
    unchar Dis_YC_Content2[3] = {YC_PIA , YC_PIB , YC_PIC};
    unchar Dis_YC_Content3[3] = {YC_UA , YC_UB , YC_UC};
    unchar Dis_YC_Content4[3] = {YC_UAB , YC_UBC , YC_UCA};
    unchar Dis_YC_Content5[2] = {YC_I01 , YC_U01};
    unchar Dis_YC_Content6[2] = {YC_I2 , YC_U2};
    unchar Dis_YC_Content7[2] = {YC_F , YC_COS};
    unchar Dis_YC_Content8[2] = {YC_P , YC_Q};


    switch(currmenu.cur)
    {
    case 1:
        Dis_YC_Temp(2, 0, Dis_YC_Content1, 3);
        break;
    case 5:
        Dis_YC_Temp(2, 0, Dis_YC_Content2, 3);
        break;
    case 9:
        Dis_YC_Temp(2, 0, Dis_YC_Content3, 3);
        break;
    case 13:
        Dis_YC_Temp(2, 0, Dis_YC_Content4, 3);
        break;
    case 17:
        Dis_YC_Temp(2, 0, Dis_YC_Content5, 2);
        break;
    case 20:
        Dis_YC_Temp(2, 0, Dis_YC_Content6, 2);
        break;
    case 23:
        Dis_YC_Temp(2, 0, Dis_YC_Content7, 2);
        break;
    case 26:
        Dis_YC_Temp(2, 0, Dis_YC_Content8, 2);
        break;
    default:
        break;
    }
    /*
    if(currmenu.cur > 16)
    {
    b = (currmenu.cur - 17) / 3 * 2 + 12;
    a = 2;
    }
    else
    {
    b = currmenu.cur - currmenu.cur / 4 - 1;
    a = 3;
    }
    for(i = 0; i < a; i++)
    {
    switch(i + b)
    {
    case 3:
    case 4:
    case 5:
        kao = YC[i + b];
        unit[0] = 1;
        unit[1] = 12;
        break;
    case 16:
        kao = YC[i + b];
        unit[0] = 2;
        unit[1] = 18;
        unit[2] = 19;
        break;
    case 17:
        kao = YC[i + b];
        unit[0] = 0;
        break;
    case 0:
    case 1:
    case 2:
    case 12:
    case 14:
        YCDisData = YC[i + b] * SYS[3];
        if(YCDisData > 99999)
        {
            kao = YCDisData / 1000;
            unit[0] = 2;
            unit[1] = 22;
            unit[2] = 12;
        }
        else
        {
            kao = YCDisData;
            unit[0] = 2;
            unit[1] = 12;
            unit[2] = 31;
        }
        break;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 13:
    case 15:
        YCDisData = YC[i + b] * SYS[2];
        if(YCDisData > 99999)
        {
            kao = YCDisData / 1000;
            unit[0] = 2;
            unit[1] = 22;
            unit[2] = 16;
        }
        else
        {
            kao = YCDisData;
            unit[0] = 2;
            unit[1] = 16;
            unit[2] = 31;
        }
        break;
    case 18:  //P
        YCDisData = YC[i + b] * SYS[2] * SYS[3];
        if(YCDisData > 9999 || YCDisData < -9999)
        {
            kao = YCDisData / 1000;
            unit[0] = 2;
            unit[1] = 22;
            unit[2] = 21;
        }
        else
        {
            kao = YCDisData;
            unit[0] = 2;
            unit[1] = 21;
            unit[2] = 31;
        }
        break;
    case 19:   //Q
        YCDisData = YC[i + b] * SYS[2] * SYS[3];
        if(YCDisData > 9999 || YCDisData < -9999)
        {
            kao = YCDisData / 1000;
            unit[0] = 4;
            unit[1] = 22;
            unit[2] = 16;
            unit[3] = 26;
            unit[4] = 27;
        }
        else
        {
            kao = YCDisData;
            unit[0] = 4;
            unit[1] = 16;
            unit[2] = 26;
            unit[3] = 27;
            unit[4] = 31;
        }
        break;
    }
    u8 yy = 0;
    if(YCDisData < 0)
    {
        yy = 1;
        YCbit[1] = 31;
    }
    YCbit[0] = 6 + yy;
    if(kao < 0)kao = 0 - kao;
    YCbit[1 + yy] = kao / 10000;
    YCbit[1 + yy] = YCbit[1 + yy] == 0 ? 31 : YCbit[1 + yy];
    YCbit[2 + yy] = (kao % 10000) / 1000;
    if(YCbit[1 + yy] == 31)
        YCbit[2 + yy] = YCbit[2 + yy] == 0 ? 31 : YCbit[2 + yy];
    YCbit[3 + yy] = (kao % 1000) / 100;
    if(i + b == 18 || i + b == 19)
    {
        YCbit[4 + yy] = (kao % 100) / 10;
        YCbit[5 + yy] = 10;
    }
    else
    {
        YCbit[4 + yy] = 10;
        YCbit[5 + yy] = (kao % 100) / 10;
    }
    YCbit[6 + yy] = kao % 10;
    if(YCDisData < 0)
    {
        if(YCbit[2 + yy] == 31)
            YCbit[2 + yy] = 32;
        else if(YCbit[1 + yy] == 31)
            YCbit[1 + yy] = 32;
        else
            YCbit[0 + yy] = 32;
    }
    if(i + b == 17) //COS
    {
        YCbit[0] = 6;
        YCbit[1] = 31;
        YCbit[2] = 0;
        YCbit[4] = YCbit[3];
        YCbit[3] = 10;
    }
    LCDWchar(2 + i, mainmenu[currmenu.cur + i][0], 0, YCbit); //YC
    LCDWchar(2 + i, mainmenu[currmenu.cur + i][0] + 7, 0, &unit[0]); //UNIT
    if(i + b < 16) //32-
    {
        YCbit[0] = 8;
        kao = PhaseAngle[i + b];
        if(kao < 0) kao = 0 - kao;
        YCbit[1] = 31;
        YCbit[2] = kao / 10000;
        YCbit[2] = YCbit[2] == 0 ? 31 : YCbit[2];
        YCbit[3] = (kao % 10000) / 1000;
        if(YCbit[2] == 31) YCbit[3] = YCbit[3] == 0 ? 31 : YCbit[3];
        YCbit[4] = (kao % 1000) / 100;
        YCbit[5] = (kao % 100) / 10;
        YCbit[6] = 10;
        YCbit[7] = kao % 10;
        if(PhaseAngle[i + b] < 0)
        {
            if(YCbit[3] == 31)
                YCbit[3] = 32;
            else if(YCbit[2] == 31)
                YCbit[2] = 32;
            else
                YCbit[1] = 32;
        }
        YCbit[8] = 33;
        LCDWchar(2 + i, mainmenu[currmenu.cur + i][0] + 9, 0, YCbit);	 //角度
    }
    }
    */
}


void menusub(unchar key)
{
    unchar key3dis;
    unchar key3cur, cur1;
    if(A_BIT > 0)
    {
        LIGHT_ON;
        return;
    }
		LightTime=0;
    if(DisSOEbit == 0)
    {
        DisSOEbit = 1;
        key = 5;
    }
    switch(key)
    {
    case 2://右
        if(OK == 1) //是否界面
        {
            OKcancel++;
            LCDWword(2, 0, 3 + (OKcancel & 0x01), &OKmenu[1][0]);
            break;
        }
        if((XG == 0) && (currmenu.ID == 12) && currmenu.ID == 15)
        {
            break;
        }
        if(currmenu.ID == 9 && key2lx == 0)
        {
            DisSOEI(currmenu.cur);
            key2lx = 2;
            break;
        }
        if(currmenu.ID == 9 && key2lx == 2)
        {
            DisSOEU(currmenu.cur);
            key2lx = 1;
            break;
        }
        if(currmenu.ID == 9 && key2lx == 1)
        {
            DisSOE(currmenu.cur);
            key2lx = 0;
            break;
        }
        key2lx++;
        if(currmenu.ID == 12 && XG == 1) //定值
        {
            key2lx = key2lx > 6 ? 1 : key2lx;
            Dis_DZ(currmenu.cur, XG, key2lx);
        }
        if(currmenu.ID == 15 && XG == 1) //参数
        {
					if(currmenu.cur>SYS_IP)
						key2lx = key2lx > 15 ? 1 : key2lx;
					else
            key2lx = key2lx > 5 ? 1 : key2lx;
          Dis_SYS(currmenu.cur, XG, key2lx);
        }
        if(currmenu.ID == 11)   //时间
        {
            key2lx = key2lx > (Data[0] + time[0]) ? 1 : key2lx;
            if(key2lx <= Data[0])
            {
                LCDWchar(2, 6, key2lx, Data);
                LCDWchar(3, 6, 0, time);
            }
            else
            {
                LCDWchar(2, 6, 0, Data);
                LCDWchar(3, 6, key2lx - Data[0], time);
            }
        }
        if(currmenu.ID == 2 || currmenu.ID == 14) //密码
        {
            key2lx = key2lx > 2 ? 1 : key2lx;
            LCDWchar(2, 2, key2lx, password);
        }
        break;
    case 3://上
        if(XG == 1)
        {
            if(currmenu.ID == 11)
            {
                switch(key2lx)
                {
                case 3:
                case 4:
                case 7:
                case 10:
                    Data[key2lx] = Data[key2lx] < 9 ? Data[key2lx] + 1 : 0;
                    break;
                case 18:
                case 15:
                case 12:
                    time[key2lx - 10] = time[key2lx - 10] < 9 ? time[key2lx - 10] + 1 : 0;
                    break;
                case 6:
                    Data[6] = Data[6] < 1 ? Data[6] + 1 : 0;
                    break;
                case 9:
                    Data[9] = Data[9] < 3 ? Data[9] + 1 : 0;
                    break;
                case 11:
                    time[1] = time[1] < 2 ? time[1] + 1 : 0;
                    break;
                case 14:
                case 17:
                    time[key2lx - 10] = time[key2lx - 10] < 5 ? time[key2lx - 10] + 1 : 0;
                    break;
                }
                if(key2lx <= Data[0])
                {
                    LCDWchar(2, 6, key2lx, Data);
                    LCDWchar(3, 6, 0, time);
                }
                else
                {
                    LCDWchar(2, 6, 0, Data);
                    LCDWchar(3, 6, key2lx - Data[0], time);
                }
            }
            if(currmenu.ID == 12)
            {
                if(key2lx == 4)break;
                pFdata[key2lx] = pFdata[key2lx] < 9 ? pFdata[key2lx] + 1 : 0;
                Dis_DZ(currmenu.cur, XG, key2lx);
            }
            if(currmenu.ID == 15)
            {
                if(currmenu.cur == 2)
                {
                    pFdata[6] = pFdata[6] < 3 ? pFdata[6] + 1 : 0;
                }
                else
                {
                    pFdata[key2lx] = pFdata[key2lx] < 9 ? pFdata[key2lx] + 1 : 0;
                }
                Dis_SYS(currmenu.cur, XG, key2lx);
            }
            if(currmenu.ID == 13)
            {
                if(con_sm[currmenu.cur] == 0)
                {
                    pCON[1] = pCON[1] == 92 ? 94 : 92;
                    pCON[2] = pCON[2] == 93 ? 33 : 93;
                }
                else
                {
                    pCON[1] = pCON[1] == 69 ? 70 : 69;
                    pCON[2] = pCON[2] == 13 ? 71 : 13;
                }
                Rcon(1);
            }
            if(currmenu.ID == 14)
            {
                password[key2lx] = password[key2lx] < 9 ? password[key2lx] + 1 : 0;
                LCDWchar(2, 2, key2lx, password);
            }
            break;
        }
        if(currmenu.ID == 6)
        {
            YKcheck[2] = YKcheck[2] < 8 ? YKcheck[2] + 1 : 1;
            if(YKcheck[2] > 9)
            {
                YKcheck[1] = 1;
                YKcheck[2] = 0;
            }
            else YKcheck[1] = 0;
            LCDWchar(1, 10, 0, YKcheck);
            break;
        }
        if(currmenu.ID == 2)
        {
            password[key2lx] = password[key2lx] < 9 ? password[key2lx] + 1 : 0;
            LCDWchar(2, 2, key2lx, password);
            break;
        }
        if(currmenu.cur == 0)break;
        LCDclear();
        if(currmenu.ID == 9)
        {
            currmenu.cur = currmenu.cur < SOEnum[0] ? currmenu.cur + 1 : 1;
            DisSOE(currmenu.cur);
            break;
        }
        if(currmenu.ID == 0)
        {
            if(currmenu.cur == 1)
                currmenu.cur = 26;
            else if(currmenu.cur > 19)
                currmenu.cur -= 3;
            else
                currmenu.cur -= 4;
            LCDWword(1, 2, 0, &mainmenu[currmenu.cur - 1][0]);
            LCDWchar(2, 0, 0, &mainmenu[currmenu.cur][0]);
            LCDWchar(3, 0, 0, &mainmenu[currmenu.cur + 1][0]);
            if(currmenu.cur < 17)
            {
                LCDWchar(4, 0, 0, &mainmenu[currmenu.cur + 2][0]);
            }
            DisYC();
        }
        else
        {
            currmenu.cur--;
            if(currmenu.cur == 0)
                currmenu.cur = currmenu.mdatax;

            if(currmenu.cur <= 4)
            {
                key3cur = currmenu.cur;
                cur1 = 0;
            }
            else
            {
                key3cur = 4;
                cur1 = currmenu.cur - 4;
            }

            for(key3dis = 1; key3dis <= (4 < currmenu.mdatax ? 4 : currmenu.mdatax); key3dis++)
            {
                if(key3cur == key3dis)
                    LCDWword(key3dis, currmenu.x, 1, &*(currmenu.mdata + currmenu.mdatay * (cur1 + key3dis - 1)));
                else
                    LCDWword(key3dis, currmenu.x, 0, &*(currmenu.mdata + currmenu.mdatay * (cur1 + key3dis - 1)));
            }
            if(currmenu.ID == 12)Dis_DZ(currmenu.cur, 0, 0);
            if(currmenu.ID == 15)Dis_SYS(currmenu.cur, 0, 0);
            if(currmenu.ID == 13) Rcon(0);
        }
        break;
    case 4://左
        if(OK == 1)
        {
            OKcancel++;
            LCDWword(2, 0, 3 + (OKcancel & 0x01), &OKmenu[1][0]);
            break;
        }
        if((XG == 0) && (currmenu.ID == 12) && currmenu.ID == 15)
        {
            break;
        }
        if(currmenu.ID == 9 && key2lx == 0)
        {
            DisSOEU(currmenu.cur);//U
            key2lx = 1;
            break;
        }
        if(currmenu.ID == 9 && key2lx == 1)
        {
            DisSOEI(currmenu.cur);
            key2lx = 2;
            break;
        }
        if(currmenu.ID == 9 && key2lx == 2)
        {
            DisSOE(currmenu.cur);
            key2lx = 0;
            break;
        }
        key2lx--;
        if(currmenu.ID == 12 && XG == 1)
        {
            key2lx = key2lx < 1 ? 6 : key2lx;
            Dis_DZ(currmenu.cur, XG, key2lx);
        }
        if(currmenu.ID == 15 && XG == 1)
        {
						if(currmenu.cur>SYS_IP)
						{
							key2lx = key2lx < 1 ? 15 : key2lx;
						}
						else
							key2lx = key2lx < 1 ? 5 : key2lx;
            Dis_SYS(currmenu.cur, XG, key2lx);
        }
        if(currmenu.ID == 11)
        {
            key2lx = key2lx < 1 ? (Data[0] + time[0]) : key2lx;
            if(key2lx <= Data[0])
            {
                LCDWchar(2, 6, key2lx, Data);
                LCDWchar(3, 6, 0, time);
            }
            else
            {
                LCDWchar(2, 6, 0, Data);
                LCDWchar(3, 6, key2lx - Data[0], time);
            }
        }
        if(currmenu.ID == 2 || currmenu.ID == 14)
        {
            key2lx = key2lx > 2 ? 1 : key2lx;
            LCDWchar(2, 2, key2lx, password);
        }
        break;
    case 5://取消
        LCDclear();
        XG = 0;
        OK = 0;
        if(currmenu.parentID == 0)
        {
            LCDWword(1, 2, 0, &mainmenu[0][0]);
            LCDWchar(2, 0, 0, &mainmenu[1][0]);
            LCDWchar(3, 0, 0, &mainmenu[2][0]);
            LCDWchar(4, 0, 0, &mainmenu[3][0]);
            currmenu = MENU[0];
            currmenu.cur = 1;
            break;
        }
        if(currmenu.parentID == 2)
            currmenu.parentID = 1;
        currmenu = MENU[currmenu.parentID];
        key3cur = currmenu.cur < 4 ? currmenu.cur : 4;
        for(key3dis = 1; key3dis <= (4 < currmenu.mdatax ? 4 : currmenu.mdatax); key3dis++)
        {
            if(key3cur == key3dis)
                LCDWword(key3dis, currmenu.x, 1, &*(currmenu.mdata + currmenu.mdatay * (key3dis - 1)));
            else
                LCDWword(key3dis, currmenu.x, 0, &*(currmenu.mdata + currmenu.mdatay * (key3dis - 1)));
        }
        break;
    case 6://确定
        if(currmenu.ID == 6)
        {
            switch(YKcheck[1] * 10 + YKcheck[2])
            {
            case 1:
                RelayAutoReset |= 0x01;
                YH_Relay_cl;
                break;
            case 2:
                RelayAutoReset |= 0x02;
                YT_Relay_cl;
                break;
            case 3:
                RelayAutoReset |= 0x04;
                T1_Relay_cl;
                break;
            case 4:
                RelayAutoReset |= 0x08;
                T2_Relay_cl;
                break;
            case 5:
                RelayAutoReset |= 0x010;
                T3_Relay_cl;
                break;
            case 6:
                RelayAutoReset |= 0x020;
                SG_Relay_cl;
                break;
            case 7:
                RelayAutoReset |= 0x040;
                GZ_Relay_cl;
                break;
            case 8:
                RelayAutoReset |= 0x080;
                ZZ_Relay_cl;
                break;
            default:
                break;
            }
            break;
        }
        if((currmenu.ID == 12) && (XG == 0) && (OK == 0))
        {
            Dis_DZ(currmenu.cur, 0, 1);
            XG = 1;
            key2lx = 1;
            break;
        }
        if((currmenu.ID == 15) && (XG == 0) && (OK == 0))
        {
            Dis_SYS(currmenu.cur, 0, 1);
            XG = 1;
            key2lx = 1;
            break;
        }
        if((currmenu.ID == 13) && (XG == 0) && (OK == 0))
        {
            Rcon(1);
            XG = 1;
            break;
        }
        if((XG == 1) && ((currmenu.ID == 11) || (currmenu.ID == 12) || (currmenu.ID == 14) || (currmenu.ID == 13) || (currmenu.ID == 15)))
        {
            LCDclear();
            OK = 1;
            LCDWword(1, 0, 0, &OKmenu[0][0]);
            LCDWword(2, 0, 3, &OKmenu[1][0]);
            XG = 0;
            break;
        }
        if(OK == 1)
        {
            if(OKcancel & 0x01)
            {
                if(currmenu.ID == 11)
                {
                    Wtime();
                    Rtime();
                    currmenu = MENU[currmenu.ID];
                }
                if(currmenu.ID == 12)
                {
                    WDZ();
                    RDZ();
                }
                if(currmenu.ID == 14)
                {
                    Wpw();
                    I2C_EE_BufferRead_AT24C512(&password[1], 0, 1);
                }
                if(currmenu.ID == 13)
                {
                    Wcon();
                    I2C_EE_BufferRead_AT24C512(CON, 90, CONnum);
                }
                if(currmenu.ID == 15)
                {
                    WSYS();
                    RSYS();
                }
            }
            OK = 0;
            OKcancel = 0;
        }
        else
        {
            if(currmenu.childnum == 0)
                break;
            if(currmenu.ID == 3 && currmenu.cur == 2)
            {
                SOEnum[0] = 0;
                SOEnum[1] = 0;
                I2C_EE_BufferWrite_AT24C512(SOEnum, 149, 2);
                break;
            }
            if(currmenu.ID == 0)
                currmenu.cur = 1;
            if(currmenu.ID == 2)
            {
                if(Rpw())
                    currmenu = MENU[8];
                else
                    currmenu = MENU[1];
            }
            else
                currmenu = MENU[currmenu.childstart + currmenu.cur - 1];
        }
        LCDclear();
        if(currmenu.ID == 7)
        {
            DisBBH();
            break;
        }
        if(currmenu.ID == 5)
        {
            LCDWword(1, currmenu.x, 0, currmenu.mdata);
            LCDWchar(2, currmenu.x, 0, &*(currmenu.mdata + currmenu.mdatay));
            break;
        }
        if(currmenu.ID == 9)
        {
            DisSOE(currmenu.cur);
            key2lx = 0;
            break;
        }
        if(currmenu.cur <= 4)
        {
            key3cur = currmenu.cur;
            cur1 = 0;
        }
        else
        {
            key3cur = 4;
            cur1 = currmenu.cur - 4;
        }
        for(key3dis = 1; key3dis <= (4 < currmenu.mdatax ? 4 : currmenu.mdatax); key3dis++)
        {
            if(key3cur == key3dis)
                LCDWword(key3dis, currmenu.x, 1, &*(currmenu.mdata + currmenu.mdatay * (key3dis + cur1 - 1)));
            else
                LCDWword(key3dis, currmenu.x, 0, &*(currmenu.mdata + currmenu.mdatay * (key3dis + cur1 - 1)));
        }
        if(currmenu.ID == 11)
        {
            Rtime();
            LCDWchar(2, 6, 1, Data);
            LCDWchar(3, 6, 0, time);
            XG = 1;
            key2lx = 1;
        }
        if(currmenu.ID == 12)
        {
            RDZ();
            Dis_DZ(currmenu.cur, 0, 0);
        }
        if(currmenu.ID == 15)
        {
            RSYS();
            Dis_SYS(currmenu.cur, 0, 0);
        }
        if(currmenu.ID == 2)
        {
            password[1] = 0;
            password[2] = 0;
            LCDWchar(2, 2, 1, password);
            key2lx = 1;
        }
        if(currmenu.ID == 14)
        {
            I2C_EE_BufferRead_AT24C512(&password[1], 0, 1); //Rpassword
            password[2] = password[1] & 0x0f;
            password[1] = password[1] >> 4;
            LCDWchar(2, 2, 1, password);
            key2lx = 1;
            XG = 1;
        }
        if(currmenu.ID == 13)
        {
            I2C_EE_BufferRead_AT24C512(CON, 90, CONnum);    //RCON //DisCON
            Rcon(0);
        }
        if(currmenu.ID == 6)
        {
            LCDWchar(1, 10, 0, YKcheck);
            LCDWword(1, 6, 1, &YKcheck[3]);
        }
        break;

    case 7://下
        if(XG == 1)
        {
            if(currmenu.ID == 11)
            {
                switch(key2lx)
                {
                case 3:
                case 4:
                case 7:
                case 10:
                    Data[key2lx] = Data[key2lx] > 0 ? Data[key2lx] - 1 : 9;
                    break;
                case 18:
                case 15:
                case 12:
                    time[key2lx - 10] = time[key2lx - 10] > 0 ? time[key2lx - 10] - 1 : 9;
                    break;
                case 6:
                    Data[6] = Data[6] > 0 ? Data[6] - 1 : 1;
                    break;
                case 9:
                    Data[9] = Data[9] > 0 ? Data[9] - 1 : 3;
                    break;
                case 11:
                    time[1] = time[1] > 0 ? time[1] - 1 : 2;
                    break;
                case 14:
                case 17:
                    time[key2lx - 10] = time[key2lx - 10] > 0 ? time[key2lx - 10] - 1 : 5;
                    break;
                }
                if(key2lx <= Data[0])
                {
                    LCDWchar(2, 6, key2lx, Data);
                    LCDWchar(3, 6, 0, time);
                }
                else
                {
                    LCDWchar(2, 6, 0, Data);
                    LCDWchar(3, 6, key2lx - Data[0], time);
                }
            }
            if(currmenu.ID == 12)
            {
                if(key2lx == 4)break;
                pFdata[key2lx] = pFdata[key2lx] > 0 ? pFdata[key2lx] - 1 : 9;
                Dis_DZ(currmenu.cur, XG, key2lx);
            }
            if(currmenu.ID == 15)
            {
                if(currmenu.cur == 2)
                {
                    pFdata[6] = pFdata[6] > 0 ? pFdata[6] - 1 : 3;
                }
                else
                {
                    pFdata[key2lx] = pFdata[key2lx] > 0 ? pFdata[key2lx] - 1 : 9;
                }
                Dis_SYS(currmenu.cur, XG, key2lx);
            }
            if(currmenu.ID == 13)
            {
                if(con_sm[currmenu.cur] == 0)
                {
                    pCON[1] = pCON[1] == 92 ? 94 : 92;
                    pCON[2] = pCON[2] == 93 ? 33 : 93;
                }
                else
                {
                    pCON[1] = pCON[1] == 69 ? 70 : 69;
                    pCON[2] = pCON[2] == 13 ? 71 : 13;
                }
                Rcon(1);
            }
            if(currmenu.ID == 14)
            {
                password[key2lx] = password[key2lx] > 0 ? password[key2lx] - 1 : 9;
                LCDWchar(2, 2, key2lx, password);
            }
            break;
        }
        if(currmenu.ID == 6)
        {
            YKcheck[2] = YKcheck[2] > 1 ? YKcheck[2] - 1 : 8;
            if(YKcheck[2] > 9)
            {
                YKcheck[1] = 1;
                YKcheck[2] = 0;
            }
            else YKcheck[1] = 0;
            LCDWchar(1, 10, 0, YKcheck);
            break;
        }
        if(currmenu.ID == 2)
        {
            password[key2lx] = password[key2lx] > 0 ? password[key2lx] - 1 : 9;
            LCDWchar(2, 2, key2lx, password);
            break;
        }
        if(currmenu.cur == 0) break;
        LCDclear();
        if(currmenu.ID == 9)
        {
            currmenu.cur = currmenu.cur > 1 ? currmenu.cur - 1 : SOEnum[0];
            DisSOE(currmenu.cur);
            break;
        }
        if(currmenu.ID == 0)
        {
            if(currmenu.cur == 26)
                currmenu.cur = 1;
            else if(currmenu.cur > 16)
                currmenu.cur += 3;
            else
                currmenu.cur += 4;
            LCDWword(1, 2, 0, &mainmenu[currmenu.cur - 1][0]);
            LCDWchar(2, 0, 0, &mainmenu[currmenu.cur][0]);
            LCDWchar(3, 0, 0, &mainmenu[currmenu.cur + 1][0]);
            if(currmenu.cur < 17)
            {
                LCDWchar(4, 0, 0, &mainmenu[currmenu.cur + 2][0]);
            }
            DisYC();
        }
        else
        {
            if(currmenu.cur >= currmenu.mdatax) currmenu.cur = 0;
            currmenu.cur++;
            if(currmenu.cur <= 4)
            {
                key3cur = currmenu.cur;
                cur1 = 0;
            }
            else
            {
                key3cur = 4;
                cur1 = currmenu.cur - 4;
            }

            for(key3dis = 1; key3dis <= (4 < currmenu.mdatax ? 4 : currmenu.mdatax); key3dis++)
            {
                if(key3cur == key3dis)
                    LCDWword(key3dis, currmenu.x, 1, &*(currmenu.mdata + currmenu.mdatay * (cur1 + key3dis - 1)));
                else
                    LCDWword(key3dis, currmenu.x, 0, &*(currmenu.mdata + currmenu.mdatay * (cur1 + key3dis - 1)));
            }
            if(currmenu.ID == 12)Dis_DZ(currmenu.cur, 0, 0);
            if(currmenu.ID == 15)
            {
                Dis_SYS(currmenu.cur, 0, 0);
            }
            if(currmenu.ID == 13)Rcon(0);
        }
        break;
    }
    if(currmenu.ID == 0)NOSOEbit = 0;
    else NOSOEbit = 1;
}



//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
/*	   填充负半波
float c=0,d=0;
unchar num_0=0,arr_0[20]={0};
for(i=0;i<20;i++)
{
	if(YCArray[5][i]>0){arr_0[num_0]=i;num_0++;}
	if(YCArray[5][i]==0)k=i;
}
num_0--;
if(num_0>=8)
{
if(arr_0[0]==10)  //1
{
	for(i=0;i<4;i++)
	{
		c=(float)YCArray[5][arr_0[i*2]]/YCArray[5][arr_0[i*2]+1];
		c=atan(0.309/(0.951-c));
		if(c<0)c=c+3.1415;
		d=d+YCArray[5][arr_0[i*2]+1]/sin(c);
	}
	c=(float)YCArray[5][arr_0[1]]/YCArray[5][arr_0[0]];
	c=atan(0.309/(c-0.951));
	if(c<0)c=c+3.1415;
	d=d/4;
	for(i=0;i<10;i++)
		YCArray[5][9-i]=d*sin(c-0.31415*(i+1));
	YC[11]=1;  //1
   }
if(arr_0[0]>0&&arr_0[0]<10)	 //2
{
	for(i=0;i<4;i++)
	{
		c=(float)YCArray[5][arr_0[i*2]]/YCArray[5][arr_0[i*2]+1];
		c=atan(0.309/(0.951-c));
		if(c<0)c=c+3.1415;
		d=d+YCArray[5][arr_0[i*2]+1]/sin(c);
	}
	d=d/4;
	c=(float)YCArray[5][arr_0[1]]/YCArray[5][arr_0[0]];
	c=atan(0.309/(c-0.951));
	if(c<0)c=c+3.1415;
	for(i=1;i<=arr_0[0];i++)
		YCArray[5][arr_0[0]-i]=d*sin(c-0.31415*i);
	c=(float)YCArray[5][arr_0[num_0-1]]/YCArray[5][arr_0[num_0]];
	c=atan(0.309/(0.951-c));
	if(c<0)c=c+3.1415;
	for(i=arr_0[num_0];i<19;i++)
		YCArray[5][i+1]=d*sin(c+0.31415*(i-arr_0[num_0]+1));	  //?
	YC[11]=2;  //2
}
if(arr_0[0]==0&&arr_0[num_0]<19) //3
{
	for(i=0;i<4;i++)
	{
		c=(float)YCArray[5][arr_0[i*2]]/YCArray[5][arr_0[i*2]+1];
		c=atan(0.309/(0.951-c));
		if(c<0)c=c+3.1415;
		d=d+YCArray[5][arr_0[i*2]+1]/sin(c);
	}
	d=d/4;
	c=(float)YCArray[5][arr_0[num_0-1]]/YCArray[5][arr_0[num_0]];
	c=atan(0.309/(0.951-c));
	if(c<0)c=c+3.1415;
	for(i=arr_0[num_0];i<19;i++)
		YCArray[5][i+1]=d*sin(c+0.31415*(i-arr_0[num_0]+1));	  //  ?
	YC[11]=3;   //3
}
if(arr_0[0]==0&&arr_0[num_0]==19) //4
{
	switch(k-9)
	{
	case 1:
	case 2:
		for(i=0;i<4;i++)
		{
			c=(float)YCArray[5][i*2+k+1]/YCArray[5][i*2+k+2];
			c=atan(0.309/(0.951-c));
			if(c<0)c=c+3.1415;
			d=d+YCArray[5][i*2+k+2]/sin(c);
		}
		d=d/4;
		c=(float)YCArray[5][k+2]/YCArray[5][k+1];
		c=atan(0.309/(c-0.951));
		if(c<0)c=c+3.1415;
		for(i=0;i<10;i++)
			YCArray[5][k-i]=d*sin(c-0.31415*(i+1));
		YC[11]=4;
		break;
	case 3:
	case 4:
		for(i=0;i<3;i++)
		{
			c=(float)YCArray[5][i*2+k+1]/YCArray[5][i*2+k+2] ;
			c=atan(0.309/(0.951-c));
			if(c<0)c=c+3.1415;
			d=d+YCArray[5][i*2+k+2]/sin(c);
		}
		c=(float)YCArray[5][0]/YCArray[5][1];
		c=atan(0.309/(0.951-c));
		if(c<0)c=c+3.1415;
		d=d+YCArray[5][1]/sin(c);
		d=d/4;
		c=(float)YCArray[5][k+2]/YCArray[5][k+1];
		c=atan(0.309/(c-0.951));
		if(c<0)c=c+3.1415;
		for(i=0;i<10;i++)
			YCArray[5][k-i]=d*sin(c-0.31415*(i+1));
		YC[11]=5;
		break;
	case 5:
		for(i=0;i<2;i++)
		{
			c=(float)YCArray[5][i*2]/YCArray[5][i*2+1];
			c=atan(0.309/(0.951-c));
			if(c<0)c=c+3.1415;
			d=d+YCArray[5][i*2+1]/sin(c);
		}
		for(i=0;i<2;i++)
		{
			c=(float)YCArray[5][i*2+k+1]/YCArray[5][arr_0[i*2+k+2]+1];
			c=atan(0.309/(0.951-c));
			if(c<0)c=c+3.1415;
			d=d+YCArray[5][i*2+k+2]/sin(c);
		}
		d=d/4;
		c=(float)YCArray[5][k+2]/YCArray[5][k+1];
		c=atan(0.309/(c-0.951));
		if(c<0)c=c+3.1415;
		for(i=0;i<10;i++)
			YCArray[5][k-i]=d*sin(c-0.31415*(i+1));
		YC[11]=6;
		break;
	case 6:
	case 7:
		for(i=0;i<3;i++)
		{
			c=(float)YCArray[5][i*2]/YCArray[5][i*2+1];
			c=atan(0.309/(0.951-c));
			if(c<0)c=c+3.1415;
			d=d+YCArray[5][i*2+1]/sin(c);
		}
		c=(float)YCArray[5][k+1]/YCArray[5][k+2];
		c=atan(0.309/(0.951-c));
		if(c<0)c=c+3.1415;
		d=d+YCArray[5][k+2]/sin(c);
		d=d/4;
		c=(float)YCArray[5][k+2]/YCArray[5][k+1];
		c=atan(0.309/(c-0.951));
		if(c<0)c=c+3.1415;
		for(i=0;i<10;i++)
			YCArray[5][k-i]=d*sin(c-0.31415*(i+1));
		YC[11]=7;
		break;
	case 8:
	case 9:
		for(i=0;i<4;i++)
		{
			c=(float)YCArray[5][i*2]/YCArray[5][i*2+1];
			c=atan(0.309/(0.951-c));
			if(c<0)c=c+3.1415;
			d=d+YCArray[5][i*2+1]/sin(c);
		}
		d=d/4;
		c=(float)YCArray[5][k-11]/YCArray[5][k-10];
		c=atan(0.309/(0.951-c));
		if(c<0)c=c+3.1415;
		for(i=0;i<10;i++)
			YCArray[5][k-9+i]=d*sin(c+0.31415*(i+1));
		YC[11]=8;
		break;
	}
}
//YC[11]=9;	//4
}


	switch(YC[11])
{
case 1:
i=0;		//59 ..
break;
case 2:
i=1;		// 52-59
break;
case 3:
i=2;	   //59
	break;
case 4:
i=4;		//58
	break;
case 5:
i=5;		//56/57
	break;
case 6:
i=6;	   //58
	break;
case 7:
i=7;		 //57
	break;
case 8:
i=8;	  //58 59
	break;
}

  	*/

/*		for(i=0;i<20;i++)
	{
		if(YCArray[5][i]>0){k=i;i=20;}
	}
	c=atan(0.309/(YCArray[5][k+1]/YCArray[5][k]-0.951));if(c<0)c=c+3.1415;			 // 后/前 前点角
	d=YCArray[5][k]/sin(c);
	for(i=1;i<=k;i++)
		YCArray[5][k-i]=d*sin(c-0.31415*i);

	k=k+10;
	c=atan(0.309/(0.951-YCArray[5][k-2]/YCArray[5][k-1]));if(c<0)c=c+3.1415;		// 前/后 后点角
	d=YCArray[5][k-1]/sin(c);
	for(i=1;i<=20-k;i++)
		YCArray[5][k+i-1]=d*sin(c+0.31415*i);
*/


/*  	if(YCArray[6][0]>0)		//取反填充
	{
		for(i=0;i<20;i++)
		{
			if(YCArray[6][i]==0){k=i;i=20;}
		}
		for(i=0;i<k;i++)
			YCArray[6][k+i]=-YCArray[6][k-i-1];
		for(i=0;i<10-k;i++)
			YCArray[6][k*2+i]=-YCArray[6][19-i];
    }

	if(YCArray[6][0]<=0)
	{
		for(i=0;i<20;i++)
		{
			if(YCArray[6][i]>0){k=i;i=20;}
		}
		for(i=0;i<k;i++)
			YCArray[6][k-i-1]=-YCArray[6][k+i];
		for(i=0;i<10-k;i++)
			YCArray[6][19-i]=-YCArray[6][k*2+i];
    }
	*/

/*
for(i=0;i<20;i++)
{
	a=a+YCArray[6][i]*YCsin[i];
	b=b+YCArray[6][i]*YCcos[i];
}
YC[10]=(unint) sqrt(a*a+b*b)*1.649;		//UB
a=atan(a/b);
//	b=atan(b/a);
if(a<0)a+=3.14;
YC[5]=(unint) (a*57.296);			//cosB
//	YC[6]=(unint) ((b+1.57)*100);
YC[7]=abs(YC[5]-YC[3]);
//	YC[8]=abs(YC[6]-YC[4]);				//A-B
for(i=1;i<20;i++)
{
//		YC[i]=YCArray[5][i];
}*/


