

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
    //���½�Timer����Ϊȱʡֵ
    TIM_DeInit(TIM2);
    //�����ڲ�ʱ�Ӹ�TIM2�ṩʱ��Դ
    TIM_InternalClockConfig(TIM2);
    //Ԥ��Ƶϵ��Ϊ36000-1������������ʱ��Ϊ72MHz/36000=2kHz
    TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1;
    //����ʱ�ӷָ�
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    //���ü�����ģʽΪ���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //���ü��������С��ÿ��2000�����Ͳ���һ�������¼�
    TIM_TimeBaseStructure.TIM_Period = 0xffff;
    //������Ӧ�õ�TIM2��
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    //������ʱ��2
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
    SOEdata[12] = (data & 0xff00) >> 8; //����ֵ
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

    SOEdata[9] = n; //�������
    SOEdata[8] = 2; //����/YX
    WSOE();
}

void TZ_Reset(void)
{
    T1_Relay_op;
    T2_Relay_op;
    T3_Relay_op;
}

#define DL3_C (YX[0]&0x01)
#define DL3_O (YX[0]&0x02)
#define DL1_C (YX[0]&0x04)
#define DL2_C (YX[0]&0x08)
#define BS_1  (YX[0]&0x10)

unchar fs1=0;
void protection(void)
{
    YX[2] = ProtectionBit;
    if(ProtectionBit == 0 && TIM2->CR1 * 0x01)
    {
        TIM_Cmd(TIM2, DISABLE);TIM2->CNT=0;
    }
	u16 dz1,yy,wy;
  unchar con1,con2;
  
  yy=DZ[0];wy=DZ[1];
  
  dz1=DZ[3];
  con1=CON[0];con2=CON[5];

  if( (YC[YC_UAB1]>yy && YC[YC_UAB2]>yy) && (con2!=0xaa ||(YC[YC_UL2]>yy && con2==0xAA))  && DL3_C && DL1_C && (DL2_C==0)&& ((ProtectionBit & 0x01)==0) && (fs1==0)) 
    {
      Pro_StartTIM2(0);
    }
  if(TIM2->CNT-ActionTime[0]>1500*20 && (ProtectionBit & 0x01 )>0)
  {
    fs1=0x55;ProtectionBit &=~ 0x01;
  }
	if((YC[YC_UL2]<yy && con2==0xAA) || DL2_C || BS_1 || con1!=0xAA)
	{
		fs1=0;ProtectionBit &=~ 0x01;
	}
  
  if(con1==0xAA)//fs1
  {
		if(fs1==0x55)
		{
      if(YC[YC_UAB1]<wy && YC[YC_UAB2]<wy && YC[YC_UL2]<wy && YC[YC_I1]<10 && ((ProtectionBit & 0x02)==0))
      {
        Pro_StartTIM2(1);
      }
		}
    if((ProtectionBit & 0x02 )>0 && TIM2->CNT-ActionTime[1]>dz1*20)
    {
      DL1_T_cl;
      if(DL1_C==0)
      {
        DL2_C_cl;
        DL1_T_op;
      }
      if(DL2_C)
      {
        ProtectionBit &=~ 0x02; 
        DL2_C_op;DL1_T_op;
        SOEdata[9]=0;//�������
        SOEdata[8]=2;
        SOEdata[14]=1;//1OK 2NO
        WSOE();
        YX[2]|=0x01;
      }
    }
    if((TIM2->CNT-ActionTime[1]>(dz1+500)*20) && (ProtectionBit & 0x02 )>0)
    {
         ProtectionBit &=~ 0x02; 
         fs1=0;
         DL2_C_op;DL1_T_op; GZ_Relay_cl;  
         SOEdata[9]=0;
         SOEdata[8]=2;
         SOEdata[14]=2;
         WSOE(); 
         YX[2]|=0x02;
    }
  }
}

void IOprotection(void){
}
