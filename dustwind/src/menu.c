
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

void Dis_YC_Temp(int16_t y, int16_t x,const unsigned char   *xh, char len)
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
void DisMain()
{
	int i,a=0;
	for(i=1;i<currmenu.cur;i++)
	{
		a+=mainmenunum[i-1];
	}
	LCDWword(1, 2, 0, &mainmenu[a][0]);
	for(i=1;i<mainmenunum[currmenu.cur-1];i++)
	{
		LCDWchar(i+1, 0, 0, &mainmenu[a+1][0]);
	}
}
void DisYC()
{
	int i,a=0;
	for(i=1;i<currmenu.cur;i++)
	{
		a+=mainmenunum[i-1]-1;
	}
   Dis_YC_Temp(2, 0, &Dis_YC_Content[a], mainmenunum[currmenu.cur]-1);
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
					currmenu.cur = currmenu.cur > 1 ? currmenu.cur-1 : currmenu.mdatax;

            DisMain();
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
            currmenu.cur = currmenu.cur < currmenu.mdatax ? currmenu.cur+1 : 1;

            DisMain();
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


