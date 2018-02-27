

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

void protection_Init() {

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
void Pro_StartTIM2(u8 n) {
    if(TIM2->CR1 & 0x01)
        ActionTime[n] = TIM2->CNT;
    else {
        TIM2->CNT = 0;
        TIM_Cmd(TIM2, ENABLE);
        ActionTime[n] = 0;
    }
    ProtectionBit |= 0x01 << n;
}
void px(int16_t *max, int16_t *min, int16_t *data) {
    int16_t t, a, b, c;
    a = *data;
    b = *(data + 1);
    c = *(data + 2);
    if(a > b) {
        t = a;
        a = b;
        b = t;
    }
    if(a > c) {
        t = a;
        a = c;
        c = t;
    }
    if(b > c) {
        t = b;
        b = c;
        c = t;
    }
    *max = c;
    *min = a;
}
void pxI(int16_t *max, int16_t *min, int16_t *p, int16_t *data) {
    int16_t t, a, b, c, d;
    a = *data;
    b = *(data + 1);
    c = *(data + 2);
    if(a > b) {
        t = a;
        d = 0;
    } else {
        t = b;
        d = 1;
    }
    if(t < c) {
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
void TZ_Action(void) {
    T1_Relay_cl;
    T2_Relay_cl;
    T3_Relay_cl;
    SG_LED_ON;
    SG_Relay_cl;
}
void TZ_out(u8 n, int16_t data, u8 CO) {
    u8 SG, GZ;
    if(T1_Bit) SG = 1;
    else SG = 0;
    if(GZ_Bit) GZ = 1;
    else GZ = 0;
    SOEdata[12] = (data & 0xff00) >> 8; //����ֵ
    SOEdata[13] = data & 0x00ff;

    if(CO == 0xaa && SG) {
        TZ_Action();
    } else if(CO == 0xCC && GZ) {
        GZ_Relay_cl;
        GZ_LED_ON;
    } else
        return;

    SOEdata[9] = n; //�������
    SOEdata[8] = 2; //����/YX
    WSOE();
}

void TZ_Reset(void) {
    T1_Relay_op;
    T2_Relay_op;
    T3_Relay_op;
}

#define DL3_C (YX[0]&0x01)
#define DL3_O (YX[0]&0x02)
#define DL1_C ((~YX[0])&0x04)
#define DL2_C ((~YX[0])&0x08)
#define BS_1  (YX[0]&0x10)
#define BS_2  (YX[0]&0x20)
#define BS_34  (YX[0]&0x40)

unchar fs1 = 0, fs2 = 0, fs3 = 0, fs4 = 0;
void protection(void) {
    YX[2] = ProtectionBit;
    if(ProtectionBit == 0 && TIM2->CR1 * 0x01) {
        TIM_Cmd(TIM2, DISABLE);
        TIM2->CNT = 0;
    }
    u16 dz1, yy, wy, num, Pbit;
    unchar con1, con2;
    _Bool fao1, fao2, fao3, fao4;

    yy = DZ[0];
    wy = DZ[1]; //��ѹ ��ѹ


    //fs1
    dz1 = DZ[2]; //�����ӳ�
    con1 = CON[0]; //Ͷ��
    con2 = CON[5]; //L2��·��ѹ���
    num = 0;Pbit=0x01<<num;

    fao1 = YC[YC_UAB1] > yy;
    fao2 = YC[YC_UAB2] > yy; //��ѹ�ж�
    fao3 = (con2 != 0xaa) || ((YC[YC_UL2] > yy) && (con2 == 0xAA)); //�������ж�
    fao4 = (DL3_C > 0) && (DL1_C > 0) && (DL2_C == 0); //λ���ж�
    if( fao1 && fao2 && fao3  && fao4 && ((ProtectionBit & Pbit) == 0) && (fs1 == 0)) { //�������
        Pro_StartTIM2(num);
    }
    if(TIM2->CNT - ActionTime[num] > 1500 * 20 && (ProtectionBit & Pbit ) > 0) { //15S������
        fs1 = 0x55;
        ProtectionBit &= ~ Pbit;
    }
    if((YC[YC_UL2] < yy && con2 == 0xAA) || (DL2_C > 0) || (BS_1 > 0) || (con1 != 0xAA)) { //������
        fs1 = 0;
        ProtectionBit &= ~ Pbit;
    }

		num=1;Pbit=0x01<<num;
    if(con1 == 0xAA) {
        if(fs1 == 0x55) {
            if(YC[YC_UAB1] < wy && YC[YC_UAB2] < wy && YC[YC_UL2] > yy && YC[YC_I1] < 10 && ((ProtectionBit & Pbit) == 0)) { //������Ͷ
                Pro_StartTIM2(num);
            }
        }
        if((ProtectionBit & Pbit ) > 0 && TIM2->CNT - ActionTime[num] > dz1 * 20) { //��Ͷ����
            DL1_T_cl;
            if(DL1_C == 0) {
                DL2_C_cl;
                DL1_T_op;
            }
            if(DL2_C > 0) { //��Ͷ�ɹ�
                ProtectionBit &= ~ Pbit;
                DL2_C_op;
                DL1_T_op;
                SOEdata[9] = 0; //�������
                SOEdata[8] = 2;
                SOEdata[14] = 1; //1 OK   2 NO
                WSOE();
                YX[2] |= 0x01;
            }
        }
        if((TIM2->CNT - ActionTime[num] > (dz1 + 500) * 20) && (ProtectionBit & Pbit ) > 0) { //��Ͷʧ��
            ProtectionBit &= ~ Pbit;
            fs1 = 0;
            DL2_C_op;
            DL1_T_op;
            GZ_Relay_cl;
            SOEdata[9] = 0;
            SOEdata[8] = 2;
            SOEdata[14] = 2;
            WSOE();
            YX[2] |= (0x01 << 1);
        }
    }//fs1

    //fs2
    dz1 = DZ[3]; //�����ӳ�
    con1 = CON[1]; //Ͷ��
    con2 = CON[4]; //L1��·��ѹ���
    num = 2;Pbit=0x01<<num;

    fao1 = YC[YC_UAB1] > yy;
    fao2 = YC[YC_UAB2] > yy; //��ѹ�ж�
    fao3 = (con2 != 0xaa) || ((YC[YC_UL1] > yy) && (con2 == 0xAA)); //�������ж�
    fao4 = (DL3_C > 0) && (DL2_C > 0) && (DL1_C == 0); //λ���ж�
    if( fao1 && fao2 && fao3  && fao4 && ((ProtectionBit & Pbit) == 0) && (fs2 == 0)) { //�������
        Pro_StartTIM2(num);
    }
    if(TIM2->CNT - ActionTime[num] > 1500 * 20 && (ProtectionBit & Pbit ) > 0) { //15S������
        fs2 = 0x55;
        ProtectionBit &= ~ Pbit;
    }
    if((YC[YC_UL1] < yy && con2 == 0xAA) || (DL1_C > 0) || (BS_2 > 0) || (con1 != 0xAA)) { //������
        fs2 = 0;
        ProtectionBit &= ~ Pbit;
    }

		num=3;Pbit=0x01<<num;
    if(con1 == 0xAA) {
        if(fs2 == 0x55) {
            if(YC[YC_UAB1] < wy && YC[YC_UAB2] < wy && YC[YC_UL1] > yy && YC[YC_I2] < 10 && ((ProtectionBit & Pbit) == 0)) { //������Ͷ
                Pro_StartTIM2(num);
            }
        }
        if((ProtectionBit & Pbit ) > 0 && TIM2->CNT - ActionTime[num] > dz1 * 20) { //��Ͷ����
            DL2_T_cl;
            if(DL2_C == 0) {
                DL1_C_cl;
                DL2_T_op;
            }
            if(DL1_C > 0) { //��Ͷ�ɹ�
                ProtectionBit &= ~ Pbit;
                DL1_C_op;
                DL2_T_op;
                SOEdata[9] = 1; //�������
                SOEdata[8] = 2;
                SOEdata[14] = 1; //1 OK   2 NO
                WSOE();
                YX[2] |= (0x01 << 2);
            }
        }
        if((TIM2->CNT - ActionTime[num] > (dz1 + 500) * 20) && (ProtectionBit & Pbit ) > 0) { //��Ͷʧ��
            ProtectionBit &= ~ Pbit;
            fs2 = 0;
            DL1_C_op;
            DL2_T_op;
            GZ_Relay_cl;
            SOEdata[9] = 1;
            SOEdata[8] = 2;
            SOEdata[14] = 2;
            WSOE();
            YX[2] |= (0x01 << 3);
        }
    }//fs2

    //fs3.fs4
    
    num = 4;Pbit=0x01<<num;
		
    fao1 = YC[YC_UAB1] > yy;
    fao2 = YC[YC_UAB2] > yy; //��ѹ�ж�
    
    fao4 = (DL1_C > 0) && (DL2_C > 0) && (DL3_C == 0); //λ���ж�
    if( fao1 && fao2 && fao4 && ((ProtectionBit & Pbit) == 0) && (fs3 == 0)) { //�������
        Pro_StartTIM2(num);
    }
    if(TIM2->CNT - ActionTime[num] > 1500 * 20 && (ProtectionBit & Pbit ) > 0) { //15S������
        fs3 = 0x55;fs4 = 0x55;
        ProtectionBit &= ~ Pbit;
    }
    if( (DL3_C > 0) || (BS_34 > 0) || ((YC[YC_UAB1] < wy) && (YC[YC_UAB2] < wy))) { //������
        fs3 = 0; fs4 = 0;
        ProtectionBit &= ~ Pbit;
    }

		//fs3
		dz1 = DZ[4]; //�����ӳ�
    con1 = CON[2]; //Ͷ��
    
		num=5;Pbit=0x01<<num;
		
    if(con1 == 0xAA) {
        if(fs3 == 0x55) {
            if(YC[YC_UAB1] < wy && YC[YC_UAB2] > yy &&  YC[YC_I1] < 10 && ((ProtectionBit & Pbit) == 0)) { //������Ͷ
                Pro_StartTIM2(num);
            }
        }
        if((ProtectionBit & Pbit ) > 0 && TIM2->CNT - ActionTime[num] > dz1 * 20) { //��Ͷ����
            DL1_T_cl;
            if(DL1_C == 0) {
                DL3_C_cl;
                DL1_T_op;
            }
            if(DL3_C > 0) { //��Ͷ�ɹ�
                ProtectionBit &= ~ Pbit;
                DL3_C_op;
                DL1_T_op;
                SOEdata[9] = 2; //�������
                SOEdata[8] = 2;
                SOEdata[14] = 1; //1 OK   2 NO
                WSOE();
                YX[2] |= (0x01 << 4);
            }
        }
        if((TIM2->CNT - ActionTime[num] > (dz1 + 500) * 20) && (ProtectionBit & Pbit ) > 0) { //��Ͷʧ��
            ProtectionBit &= ~ Pbit;
            fs3 = 0;
            DL3_C_op;
            DL1_T_op;
            GZ_Relay_cl;
            SOEdata[9] = 2;
            SOEdata[8] = 2;
            SOEdata[14] = 2;
            WSOE();
            YX[2] |= (0x01 << 5);
        }
    }//fs3  

    //fs4
		dz1 = DZ[5]; //�����ӳ�
    con1 = CON[3]; //Ͷ��
    
		num=6;Pbit=0x01<<num;
		
    if(con1 == 0xAA) {
        if(fs4 == 0x55) {
            if(YC[YC_UAB2] < wy && YC[YC_UAB1] > yy &&  YC[YC_I2] < 10 && ((ProtectionBit & Pbit) == 0)) { //������Ͷ
                Pro_StartTIM2(num);
            }
        }
        if((ProtectionBit & Pbit ) > 0 && TIM2->CNT - ActionTime[num] > dz1 * 20) { //��Ͷ����
            DL2_T_cl;
            if(DL2_C == 0) {
                DL3_C_cl;
                DL2_T_op;
            }
            if(DL3_C > 0) { //��Ͷ�ɹ�
                ProtectionBit &= ~ Pbit;
                DL3_C_op;
                DL2_T_op;
                SOEdata[9] = 3; //�������
                SOEdata[8] = 2;
                SOEdata[14] = 1; //1 OK   2 NO
                WSOE();
                YX[2] |= (0x01 << 6);
            }
        }
        if((TIM2->CNT - ActionTime[num] > (dz1 + 500) * 20) && (ProtectionBit & Pbit ) > 0) { //��Ͷʧ��
            ProtectionBit &= ~ Pbit;
            fs4 = 0;
            DL3_C_op;
            DL2_T_op;
            GZ_Relay_cl;
            SOEdata[9] = 3;
            SOEdata[8] = 2;
            SOEdata[14] = 2;
            WSOE();
            YX[2] |= (0x01 << 7);
        }
    }//fs4		

}

void IOprotection(void) {
}
