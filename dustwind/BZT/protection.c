

#include "protection.h"
#include "bianliang.h"
//#include "menu.h"
#include "i2c.H"
#include "yc.h"
#include "io.h"
#include "soe.h"

u16 ProtectionBit = 0x0;


float fsx = 0, kt = 7;
const float fsxc[20] = {1, 1.013959, 1.022215, 1.028114, 1.032712, 1.036485, 1.039685, 1.042466, 1.044924, 1.047129, 1.049126 , 1.050954, 1.052638, 1.054199, 1.055655, 1.057018};
const float fsx2[20] = {1, 4, 9, 14, 25, 36, 49, 64, 81, 100, 121 , 144, 169, 196, 225, 256};

void protection_Init()
{

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    //重新将Timer设置为缺省值
    TIM_DeInit(TIM2);
    //采用内部时钟给TIM2提供时钟源
    TIM_InternalClockConfig(TIM2);
    //预分频系数为36000-1，这样计数器时钟为72MHz/36000=2kHz
    TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1;
    //设置时钟分割
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    //设置计数器模式为向上计数模式
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //设置计数溢出大小，每计2000个数就产生一个更新事件
    TIM_TimeBaseStructure.TIM_Period = 0xffff;
    //将配置应用到TIM2中
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    //开启定时器2
    //TIM_Cmd(TIM2,ENABLE);

}
void Pro_StartTIM2(u8 n)
{
    if(TIM2->CR1 & 0x01)
        ActionTime[n] = TIM2->CNT;
    else
    {
        TIM2->CNT = 0;
        TIM_Cmd(TIM2, ENABLE);
        ActionTime[n] = 0;
    }
    ProtectionBit |= 0x01 << n;
}
void px(int16_t *max, int16_t *min, int16_t *data)
{
    int16_t t, a, b, c;
    a = *data;
    b = *(data + 1);
    c = *(data + 2);
    if(a > b)
    {
        t = a;
        a = b;
        b = t;
    }
    if(a > c)
    {
        t = a;
        a = c;
        c = t;
    }
    if(b > c)
    {
        t = b;
        b = c;
        c = t;
    }
    *max = c;
    *min = a;
}
void pxI(int16_t *max, int16_t *min, int16_t *p, int16_t *data)
{
    int16_t t, a, b, c, d;
    a = *data;
    b = *(data + 1);
    c = *(data + 2);
    if(a > b)
    {
        t = a;
        d = 0;
    }
    else
    {
        t = b;
        d = 1;
    }
    if(t < c)
    {
        t = c;
        d = 2;
    }


    *max = t;
    *min = 0;
    if(YC[0] < 500)
        a = d;
    if(PhaseAngle[0] > 300)
        c = d;
    /*
    switch(d)
    {
    case 0:
      if(d==0 && PhaseAngle[0]<300 && PhaseAngle[0]>-1500)
          *p=55;
        else
          *p=0;
        break;
    case 1:
       if(d==1 && ((PhaseAngle[1]<-900 && PhaseAngle[1]>-1800) || (PhaseAngle[1]>900 && PhaseAngle[1]<1800)))
          *p=55;
        else
          *p=0;
        break;
    case 2:
       if(d==2 && PhaseAngle[2]<1500 && PhaseAngle[2]>-300)
          *p=55;
        else
          *p=0;
        break;
    }
    */


}
void TZ_Action(void)
{
    T1_Relay_cl;
    T2_Relay_cl;
    T3_Relay_cl;
    SG_LED_ON;
    SG_Relay_cl;
}
void TZ_out(u8 n, int16_t data, u8 CO)
{
    u8 SG, GZ;
    if(T1_Bit) SG = 1;
    else SG = 0;
    if(GZ_Bit) GZ = 1;
    else GZ = 0;
    SOEdata[12] = (data & 0xff00) >> 8; //动作值
    SOEdata[13] = data & 0x00ff;

    if(CO == 0xaa && SG)
    {
        TZ_Action();
    }
    else if(CO == 0xCC && GZ)
    {
        GZ_Relay_cl;
        GZ_LED_ON;
    }
    else
        return;

    SOEdata[9] = n; //保护序号
    SOEdata[8] = 2; //保护/YX
    WSOE();
}

void TZ_Reset(void)
{
    T1_Relay_op;
    T2_Relay_op;
    T3_Relay_op;
}

void protection(void)
{
    YX[2] = ProtectionBit;
    if(ProtectionBit == 0 && TIM2->CR1 * 0x01)
    {
        TIM_Cmd(TIM2, DISABLE);
    }
    int16_t Imax, Imin, ULmax, ULmin, UPmax, UPmin, Pmax;
    pxI(&Imax, &Imin, &Pmax, &YC[YC_PIA]);
    px(&ULmax, &ULmin, &YC[YC_UAB]);
    px(&UPmax, &UPmin, &YC[YC_UA]);
    if(CON[0] == 0xAA) //速断 DZ[0]       CON0
    {
        if((Imax > DZ[0]) && ((ProtectionBit & 0x01) == 0) )
        {

            SOEdata[14] = Property_I; //动作属性
            TZ_out(0, Imax, 0xaa);
            ProtectionBit |= 0x01;

        }
        if(Imax < DZ[0] * 0.96 && (ProtectionBit & 0x01) > 0)
        {
            ProtectionBit &= ~0x01;
            TZ_Reset();
        }
    }
    if(CON[1] == 0xAA) //限时速断 DZ[1] DZ[2]    CON1
    {
        if(Imax > DZ[1] && ((ProtectionBit & 0x02) == 0))
        {
            Pro_StartTIM2(1);
        }
        if(Imax < DZ[1] * 0.96 && (ProtectionBit & 0x02) > 0)
        {
            ProtectionBit &= ~0x02;
            TZ_Reset();
        }

        if((ProtectionBit & 0x02) > 0 && TIM2->CNT - ActionTime[1] > DZ[2] * 20)
        {
            SOEdata[14] = Property_I; //动作属性
            TZ_out(1, Imax, 0xaa);
        }
    }
    if(CON[2] == 0xAA) //过流保护 DZ[3] DZ[4]    CON2
    {
        if(Imax > DZ[3] && ((ProtectionBit & 0x04) == 0))
        {
            Pro_StartTIM2(2);
        }
        if(Imax < DZ[3] * 0.96 && (ProtectionBit & 0x04) > 0)
        {
            ProtectionBit &= ~0x04;
            TZ_Reset();
        }

        if((ProtectionBit & 0x04) && TIM2->CNT - ActionTime[2] > DZ[4] * 20)
        {
            SOEdata[14] = Property_I; //动作属性
            TZ_out(2, Imax, 0xaa);
        }
    }
}

void IOprotection(void){
}
