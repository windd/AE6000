//#define GPIOG_CRL   (*((unsigned int *)(0x40012000)))
//#define GPIOG_BSRR  (*((unsigned int *)(0x40012010)))
//#define GPIOG_BRR   (*((unsigned int *)(0x40012014)))
//#define RCC_APB2ENR (*((unsigned int *)(0x40021018)))
/* Configure all the GPIOA in Input Floating mode */

#include "dz.h"
#include "lcd.h"
#include "bianliang.h"
#include "i2c.h"
#include "io.h"
#include "menu_content.h"
#include "protection.h"
#include "math.h"
#include "w5500s.h"


//SDA PB11 SCL PB10

void RSOEnum()
{
    I2C_EE_BufferRead_AT24C512(SOEnum, SoeStAddr, 2); //读SOE个数
}



void Rtime()
{
    unchar DT[9];
    I2C_EE_BufferRead(&DT[0], 0x00, 0x09);

    time[8] = DT[2] & 0x0f;
    time[7] = (DT[2] & 0x70) >> 4;
    time[5] = DT[3] & 0x0f;
    time[4] = (DT[3] & 0x70) >> 4;
    time[2] = DT[4] & 0x0f;
    time[1] = (DT[4] & 0x30) >> 4;
    Data[10] = DT[5] & 0x0f;
    Data[9] = (DT[5] & 0x30) >> 4;
    Data[7] = DT[7] & 0x0f;
    Data[6] = (DT[7] & 0x10) >> 4;
    Data[4] = DT[8] & 0x0f;
    Data[3] = (DT[8] & 0xf0) >> 4;
    Data[2] = 0x00;
    Data[1] = 0x02;
}
void Wtime()
{
    unchar DT[9];
    //  DT[0] = 0;
    //  DT[1] = 0;
    DT[2] = time[7] * 0x10 + time[8];
    DT[3] = time[4] * 0x10 + time[5];
    DT[4] = time[1] * 0x10 + time[2];
    DT[5] = Data[9] * 0x10 + Data[10];
    DT[7] = Data[6] * 0x10 + Data[7];
    DT[8] = Data[3] * 0x10 + Data[4];
    I2C_EE_BufferWrite(&DT[2], 0x02, 0x07);
}

void RCON()
{
    I2C_EE_BufferRead_AT24C512(CON, 90, DZnum); //读控制字
}
void Wcon()
{
    unchar i = 0xFF;
    if(pCON[1] == 69 || pCON[1] == 92)
        i = 0xAA;
    else
        i = 0xCC;
    I2C_EE_BufferWrite_AT24C512(&i, currmenu.cur + 89, 1);
}
void Rcon(unchar XGbit)
{
    unchar CONchar[3], i, cur;

    CONchar[0] = 2;
    pCON[0] = 2;
    if(XGbit)
    {
        if(currmenu.cur > 4)
            LCDWword(4, 7, 1, pCON);
        else
            LCDWword(currmenu.cur, 7, 1, pCON);
        return;
    }
    if(currmenu.cur > 4)
    {
        XGbit = 3;
        cur = currmenu.cur - 4;
    }
    else
    {
        XGbit = currmenu.cur - 1;
        cur = 0;
    }

    for(i = 0; i < (4 < currmenu.mdatax ? 4 : currmenu.mdatax); i++)
    {
        if(con_sm[cur + i+1] == 0)
        {
            switch(CON[cur + i])
            {
            case 0xAA:
                CONchar[1] = 92;
                CONchar[2] = 93;
                break;   //信号跳闸
            case 0xCC:
                CONchar[1] = 94;
                CONchar[2] = 33;
                break;
            default:
                CONchar[1] = 72;
                CONchar[2] = 73;
            }
        }
        else
        {
            switch(CON[cur + i])
            {
            case 0xAA:
                CONchar[1] = 69;
                CONchar[2] = 13;
                break;   //投入退出
            case 0xCC:
                CONchar[1] = 70;
                CONchar[2] = 71;
                break;
            default:
                CONchar[1] = 72;
                CONchar[2] = 73;
            }
        }
        if(i == XGbit)
        {
            LCDWword(i + 1, 7, 0, CONchar);
            pCON[2] = CONchar[2];
            pCON[1] = CONchar[1];
        }
        else
            LCDWword(i + 1, 7, 0, CONchar);
    }
}
unchar Rpw()
{
    unchar i = 0x45;
    I2C_EE_BufferRead_AT24C512(&i, 0, 1);
    if(((0x0f & i >> 4) == password[1] && (i & 0x0f) == password[2]) || (password[1] == 8 && password[2] == 8))
        return 1;
    else
        return 0;
}

void Wpw()
{
    unchar i;
    i = password[1] * 0x10 + password[2];
    I2C_EE_BufferWrite_AT24C512(&i, 0, 1);
}



void WSYS()//WSYS
{
    USART_InitTypeDef USART_InitStructure;
    unint i;
    unchar j[4];
	
		if(currmenu.cur==SYS_IP+1)
		{
			IP_Addr[0]=(pFdata[1]*100+pFdata[2]*10+pFdata[3])& 0xff;
			IP_Addr[1]=(pFdata[5]*100+pFdata[6]*10+pFdata[7])& 0xff;
			IP_Addr[2]=(pFdata[9]*100+pFdata[10]*10+pFdata[11])& 0xff;
			IP_Addr[3]=(pFdata[13]*100+pFdata[14]*10+pFdata[15])& 0xff;
			I2C_EE_BufferWrite_AT24C512 (IP_Addr, currmenu.cur * 2 - 1, 4);
			RSYS();
			Change_IP();
			return;
		}
		if(currmenu.cur==SYS_YM+1)
		{
			Sub_Mask[0]=(pFdata[1]*100+pFdata[2]*10+pFdata[3])& 0xff;
			Sub_Mask[1]=(pFdata[5]*100+pFdata[6]*10+pFdata[7])& 0xff;
			Sub_Mask[2]=(pFdata[9]*100+pFdata[10]*10+pFdata[11])& 0xff;
			Sub_Mask[3]=(pFdata[13]*100+pFdata[14]*10+pFdata[15])& 0xff;
			I2C_EE_BufferWrite_AT24C512 (Sub_Mask, (currmenu.cur+1) * 2 - 1, 4);
			RSYS();
			Change_IP();
			return;
		}
    i = pFdata[1] * 10000 + pFdata[2] * 1000 + pFdata[3] * 100 + pFdata[4] * 10 + pFdata[5];
    j[0] = (i & 0xff00) >> 8;
    j[1] = i & 0xff;
    I2C_EE_BufferWrite_AT24C512 (j, currmenu.cur * 2 - 1, 2);
    if(currmenu.cur == SYS_BOND+1)
    {
        USART_Cmd(USART1, DISABLE);
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        USART_InitStructure.USART_BaudRate = i;
        USART_Init(USART1, &USART_InitStructure);
        USART_Cmd(USART1, ENABLE);
    }
}

void RSYS()
{
    unchar  SYSchar[SYSnum * 2], c;
    I2C_EE_BufferRead_AT24C512(SYSchar, 1, SYSnum * 2);
    for(c = 0; c < SYSnum; c++)
    {
        SYS[c] = SYSchar[c * 2] * 0x100 + SYSchar[c * 2 + 1];
    }
}

void Dis_IP(unchar cur,unchar Fx,unchar Fy)
{
	unchar i;
	pFdata[0]=15;
	if(Fx!=1)
	{
		for(i=0;i<4;i++)
		{
			pFdata[1+i*4]=IP_Addr[i]/100;
			pFdata[2+i*4]=(IP_Addr[i]%100)/10;
			pFdata[3+i*4]=IP_Addr[i]%10;
		}
	}
	pFdata[4]=10;pFdata[8]=10;pFdata[12]=10;
	LCDWchar(cur, 6, Fy, pFdata);
}

void Dis_YM(unchar cur,unchar Fx,unchar Fy)
{
	unchar i;
	pFdata[0]=15;
	if(Fx!=1)
	{
		for(i=0;i<4;i++)
		{
			pFdata[1+i*4]=Sub_Mask[i]/100;
			pFdata[2+i*4]=(Sub_Mask[i]%100)/10;
			pFdata[3+i*4]=Sub_Mask[i]%10;
		}
	}
	pFdata[4]=10;pFdata[8]=10;pFdata[12]=10;
	LCDWchar(cur, 6, Fy, pFdata);	
}

void Dis_SYS(unchar cur, unchar Fx, unchar Fy) //dis SYS Fx是否是在修改 Fy修改的是哪一位
{
    unchar Fdata[6], i;
    u16 bond = 0;
    Fdata[0] = 5; //pFdat[0]=6;
    if(Fx)
    {
        if(cur == 2)
        {
            bond = (u16)pow(2, pFdata[6]);
            bond = 2400 * bond;
            pFdata[1] = bond / 10000;
            pFdata[2] = (bond % 10000) / 1000;
            pFdata[3] = (bond % 1000) / 100;
            pFdata[4] = (bond % 100) / 10;
            pFdata[5] = bond % 10;
            LCDWchar(2, 12, 111, pFdata);
					return;
        }
				if(currmenu.cur==SYS_IP+1)
				{
					Dis_IP(4,1,Fy);
					return;
				}
				if(currmenu.cur==SYS_YM+1)
				{
					Dis_YM(4,1,Fy);
					return;
				}
        if(cur > 4)
				{
            LCDWchar(4, 12, Fy, pFdata);
					return;
				}
        LCDWchar(cur, 12, Fy, pFdata);
        return;
    }
    if(cur > 4)
    {
        Fx = 3;
        cur -= 3;
    }
    else
    {
        Fx = cur - 1;
        cur = 1;
    }
    for(i = 0; i < 4; i++)
    {
				if(cur+i==SYS_IP+1) 
				{
					if(i == Fx)Dis_IP(i+1,0,Fy);else Dis_IP(i+1,0,0);
					continue;
				}
				if(cur+i==SYS_YM+1) 
				{
					if(i == Fx)Dis_YM(i+1,0,Fy);else Dis_YM(i+1,0,0);
					continue;
				}
        Fdata[1] = SYS[cur + i - 1] / 10000;
        Fdata[2] = (SYS[cur + i - 1] % 10000) / 1000;
        Fdata[3] = (SYS[cur + i - 1] % 1000) / 100;
        Fdata[4] = (SYS[cur + i - 1] % 100) / 10;
        Fdata[5] = SYS[cur + i - 1] % 10;
        if(i == Fx)
        {
            LCDWchar(i + 1, 12, Fy, Fdata);
            pFdata[0] = Fdata[0];
            pFdata[1] = Fdata[1];
            pFdata[2] = Fdata[2];
            pFdata[3] = Fdata[3];
            pFdata[4] = Fdata[4];
            pFdata[5] = Fdata[5];
        }
        else
            LCDWchar(i + 1, 12, 0, Fdata);
    }
}

void RDZ()
{
    unchar DZchar[DZnum * 2], i;
    I2C_EE_BufferRead_AT24C512(DZchar, 30, DZnum * 2);
    for(i = 0; i < DZnum; i++)
    {
        DZ[i] = DZchar[i * 2] * 0x100 + DZchar[i * 2 + 1];
    }
}
void WDZ()
{
    unint i;
    unchar j[2];
    i = pFdata[1] * 10000 + pFdata[2] * 1000 + pFdata[3] * 100 + pFdata[5] * 10 + pFdata[6];
    j[0] = (i & 0xff00) >> 8;
    j[1] = i & 0xff;
    I2C_EE_BufferWrite_AT24C512 (j, currmenu.cur * 2 + 28, 2);
}

void Dis_DZ(unchar cur, unchar Fx, unchar Fy) //dis DZ
{
    unchar Fdata[8], i;
    Fdata[0] = 6;
    pFdata[4] = 10;
    Fdata[4] = 10;
    pFdata[0] = 6;
    if(Fx)
    {
        if(cur > 4)
            LCDWchar(4, 13, Fy, pFdata);
        else
        {
            LCDWchar(cur, 13, Fy, pFdata);
        }
        return;
    }
    if(cur > 4)
    {
        Fx = 3;
        cur -= 3;
    }
    else
    {
        Fx = cur - 1;
        cur = 1;
    }

    for(i = 0; i < 4; i++)
    {
        Fdata[1] = DZ[cur + i - 1] / 10000;
        Fdata[2] = (DZ[cur + i - 1] % 10000) / 1000;
        Fdata[3] = (DZ[cur + i - 1] % 1000) / 100;
        Fdata[5] = (DZ[cur + i - 1] % 100) / 10;
        Fdata[6] = DZ[cur + i - 1] % 10;
        if(i == Fx)
        {
            pFdata[1] = Fdata[1];
            pFdata[2] = Fdata[2];
            pFdata[3] = Fdata[3];
            pFdata[5] = Fdata[5];
            pFdata[6] = Fdata[6];
            LCDWchar(i + 1, 13, Fy, Fdata);
        }
        else
        {
            LCDWchar(i + 1, 13, 0, Fdata);
        }
        LCDWchar(i + 1, 20, 0, DZunit[cur + i - 1]);
    }
}


