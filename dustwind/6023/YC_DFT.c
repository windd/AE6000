#include "YC_DFT.h"
#include "yc.h"

#define ADCa Num_Adc_Chanel
#define ADCb Num_Adc_Chanel+1

#define PIA_Chanel 0
#define PIB_Chanel 1
#define PIC_Chanel 2
#define MIA_Chanel 3
#define MIB_Chanel 4
#define MIC_Chanel 5
#define UA_Chanel 6
#define UB_Chanel 7
#define UC_Chanel 8
#define LI0_Chanel 9
#define U0_Chanel 10
#define HI0_Chanel 11

#define XS_MI 0.053
#define XS_PI 0.528
#define XS_U  0.555

//int16_t Chanel_Index[Num_Adc_Chanel]={YC_PIA,YC_PIB,YC_PIC,YC_MIA,YC_MIB,YC_MIC,YC_UA,YC_UB,YC_UC,YC_I01,YC_U01,YC_I02};



const float YCsin[20] = {0.0, 0.309, 0.588, 0.809, 0.951, 1.0, 0.951, 0.809, 0.588, 0.309, 0.0, -0.309, -0.588, -0.809, -0.951, -1.0, -0.951, -0.809, -0.588, -0.309};
const float YCcos[20] = {1.0, 0.951, 0.809, 0.588, 0.309, 0.0, -0.309, -0.588, -0.809, -0.951, -1.0, -0.951, -0.809, -0.588, -0.309, 0.0, 0.309, 0.588, 0.809, 0.951};

void YCDFT()
{
    float a = 0, b = 0, c = 0;
    float a2, a3, b2, b3;
    unchar i, ia;
    unchar k = 0;
    for(k = 0; k < Num_Adc_Chanel; k++)
    {
        a = 0;
        b = 0;
        for(i = 0; i < 20; i++)
        {
            a = a + YCArray[k][i] * YCsin[i];
            b = b + YCArray[k][i] * YCcos[i];
        }
        c = atan(b / a) * 572.9;
        if(a < 0)c += 1800;
        if(b < 0 && a > 0)c += 3600;
        PhaseAngle[k] = (int)c;
        switch(k)
        {
        case PIC_Chanel://PIC
            a = a * SYS[SYS_PIC] / 1000;
            b = b * SYS[SYS_PIC] / 1000;
            YC[YC_PIC] = (int)(sqrt(a * a + b * b) * XS_PI);
            if(YC[YC_PIC] < 5)
            {
                YC[YC_PIC] = 0;
                a = 1;
                b = 0;
            }

            break;
        case PIB_Chanel://PIB
            a = a * SYS[SYS_PIB] / 1000;
            b = b * SYS[SYS_PIB] / 1000;
            YC[YC_PIB] = (int)(sqrt(a * a + b * b) * XS_PI);
            if(YC[YC_PIB] < 5)
            {
                YC[YC_PIB] = 0;
                a = 1;
                b = 0;
            }

            break;
        case PIA_Chanel://PIA
            a = a * SYS[SYS_PIA] / 1000;
            b = b * SYS[SYS_PIA] / 1000;
            YC[YC_PIA] = (int)(sqrt(a * a + b * b) * XS_PI);
            if(YC[YC_PIA] < 5)
            {
                YC[YC_PIA] = 0;
                a = 1;
                b = 0;
            }

            break;
        case MIC_Chanel://MIC
            a = a * SYS[SYS_MIC] / 1000;
            b = b * SYS[SYS_MIC] / 1000;
            YC[YC_MIC] = (int)(sqrt(a * a + b * b) * XS_MI);
            if(YC[YC_MIC] < 5)
            {
                YC[YC_MIC] = 0;
                a = 1;
                b = 0;
            }
            break;
        case MIB_Chanel://MIB
            a = a * SYS[SYS_MIB] / 1000;
            b = b * SYS[SYS_MIB] / 1000;
            YC[YC_MIB] = (int)(sqrt(a * a + b * b) * XS_MI);
            if(YC[YC_MIB] < 5)
            {
                YC[YC_MIB] = 0;
                a = 1;
                b = 0;
            }
            break;
        case MIA_Chanel://MIA
            a = a * SYS[SYS_MIA] / 1000;
            b = b * SYS[SYS_MIA] / 1000;
            YC[YC_MIA] = (int)(sqrt(a * a + b * b) * XS_MI);
            if(YC[YC_MIA] < 5)
            {
                YC[YC_MIA] = 0;
                a = 1;
                b = 0;
            }
            break;
        case LI0_Chanel://LI0
            YC[YC_I01] = (int)(sqrt(a * a + b * b) * XS_MI * SYS[SYS_I01] / 1000);
            if(YC[YC_I01] < 5)
            {
                YC[YC_I01] = 0;
                a = 1;
                b = 0;
            }
            break;
        case HI0_Chanel://HI0
            YC[YC_I02] = (int)(sqrt(a * a + b * b) * XS_MI * SYS[SYS_I02] / 1000);
            if(YC[YC_I02] < 5)
            {
                YC[YC_I02] = 0;
                a = 1;
                b = 0;
            }
            break;
        case UB_Chanel://UB
            a = a * SYS[SYS_UB] / 1000;
            b = b * SYS[SYS_UB] / 1000;
            YC[YC_UB] = (int)(sqrt(a * a + b * b) * XS_U);
            if(YC[YC_UB] < 20)
            {
                YC[YC_UB] = 0;
                a = 1;
                b = 0;
            }
            break;
        case UC_Chanel://UC
            a = a * SYS[SYS_UC] / 1000;
            b = b * SYS[SYS_UC] / 1000;
            YC[YC_UC] = (int)(sqrt(a * a + b * b) * XS_U);
            if(YC[YC_UC] < 20)
            {
                YC[YC_UC] = 0;
                a = 1;
                b = 0;
            }
            break;
        case UA_Chanel://UA
            a = a * SYS[SYS_UA] / 1000;
            b = b * SYS[SYS_UA] / 1000;
            YC[YC_UA] = (int)(sqrt(a * a + b * b) * XS_U);
            if(YC[YC_UA] < 20)
            {
                YC[YC_UA] = 0;
                a = 1;
                b = 0;
            }
            break;
        case U0_Chanel://U0
            YC[YC_U01] = (int)(sqrt(a * a + b * b) * XS_U * SYS[SYS_U01] / 1000);
            if(YC[YC_U01] < 20)
            {
                YC[YC_U01] = 0;;
                a = 1;
                b = 0;
            }
            break;
        }
        YCArray[ADCa][k] = (int)a;
        YCArray[ADCb][k] = (int)b;
    }
    //   if(YC[YC_UA]<1600){YC[YC_F]=0;TIM1->ARR=36000-1;}
    //   else TIM1->ARR=(u16)(180000000/YC[YC_F]-1);

    YCArray[0][0] = PhaseAngle[UA_Chanel]; //UA
    YCArray[0][1] = PhaseAngle[0];
    PhaseAngle[0] = PhaseAngle[3];
    PhaseAngle[3] = YCArray[0][1];
    YCArray[0][1] = PhaseAngle[1];
    PhaseAngle[1] = PhaseAngle[4];
    PhaseAngle[4] = YCArray[0][1];
    YCArray[0][1] = PhaseAngle[2];
    PhaseAngle[2] = PhaseAngle[5];
    PhaseAngle[5] = YCArray[0][1];

    //三相四线
    a = YCArray[ADCa][UA_Chanel] - YCArray[ADCa][UB_Chanel];
    b = YCArray[ADCb][UA_Chanel] - YCArray[ADCb][UB_Chanel];
    c = sqrt(a * a + b * b) * XS_U;
    YC[YC_UAB] = (int)c;				 //9 Uab

    c = atan(b / a) * 572.9;
    if(a < 0)c += 1800;
    if(b < 0 && a > 0)c += 3600;
    PhaseAngle[YC_UAB] = (int)c;


    a = YCArray[ADCa][UB_Chanel] - YCArray[ADCa][UC_Chanel];
    b = YCArray[ADCb][UB_Chanel] - YCArray[ADCb][UC_Chanel];
    c = sqrt(a * a + b * b) * XS_U;
    YC[YC_UBC] = (int)c;				 //10 Ubc

    c = atan(b / a) * 572.9;
    if(a < 0)c += 1800;
    if(b < 0 && a > 0)c += 3600;
    PhaseAngle[YC_UBC] = (int)c;

    a = YCArray[ADCa][UC_Chanel] - YCArray[ADCa][UA_Chanel];
    b = YCArray[ADCb][UC_Chanel] - YCArray[ADCb][UA_Chanel];
    c = sqrt(a * a + b * b) * XS_U;
    YC[YC_UCA] = (unint)c;				 //11 Uca

    c = atan(b / a) * 572.9;
    if(a < 0)c += 1800;
    if(b < 0 && a > 0)c += 3600;
    PhaseAngle[YC_UCA] = (int)c;


    a = YCArray[ADCa][UB_Chanel] * (cosB) + YCArray[ADCb][UB_Chanel] * sinB; //U2
    b = YCArray[ADCb][UB_Chanel] * (cosB) - YCArray[ADCa][UB_Chanel] * sinB;
    a += YCArray[ADCa][UC_Chanel] * (cosB) - YCArray[ADCb][UC_Chanel] * sinB;
    b += YCArray[ADCa][UC_Chanel] * sinB + YCArray[ADCb][UC_Chanel] * (cosB);
    a += YCArray[ADCa][UA_Chanel];
    b += YCArray[ADCb][UA_Chanel];
    c = sqrt(a * a + b * b) * XS_U / 3;
    YC[YC_U2] = (int)c;



    for(ia = 0; ia < 12; ia++)
    {
        if(YC[ia] > 0)
        {
            PhaseAngle[ia] -= YCArray[0][0];
            if(PhaseAngle[ia] > 1800)PhaseAngle[ia] -= 3600;
            if(PhaseAngle[ia] < -1800)PhaseAngle[ia] += 3600;
        }
        else
            PhaseAngle[ia] = 0;
    }
    PhaseAngle[YC_UA] = 0;

    b = 0 - PhaseAngle[YC_MIA] * 3.1415 / 1800;
    b2 = (PhaseAngle[YC_UB] - PhaseAngle[YC_MIB]) * 3.1415 / 1800;
    b3 = (PhaseAngle[YC_UC] - PhaseAngle[YC_MIC]) * 3.1415 / 1800;
    a = YC[YC_UA] * YC[YC_MIA];
    a2 = YC[YC_UB] * YC[YC_MIB];
    a3 = YC[YC_UC] * YC[YC_MIC];
    YC[YC_Q] = (int)((a * sin(b) + a2 * sin(b2) + a3 * sin(b3)) / 1000); //Q
    YC[YC_P] = (int)((a * cos(b) + a2 * cos(b2) + a3 * cos(b3)) / 1000); //P
    if(a + a2 + a3 == 0)
        YC[YC_COS] = 0;
    else
    {
        a = ((float)YC[YC_P]) * 1000000 / (a + a2 + a3);
        YC[YC_COS] = (int)(a); //cos
    }

    a = YCArray[ADCa][UA_Chanel] + YCArray[ADCa][UB_Chanel] + YCArray[ADCa][UC_Chanel];
    b = YCArray[ADCb][UA_Chanel] + YCArray[ADCb][UB_Chanel] + YCArray[ADCb][UC_Chanel];
    c = sqrt(a * a + b * b) * 0.62;
    YC[YC_Custom1] = (int)c;

    a = YCArray[ADCa][MIB_Chanel] * (cosB) + YCArray[ADCb][MIB_Chanel] * sinB; //I2
    b = YCArray[ADCb][MIB_Chanel] * (cosB) - YCArray[ADCa][MIB_Chanel] * sinB;
    a += YCArray[ADCa][MIC_Chanel] * (cosB) - YCArray[ADCb][MIC_Chanel] * sinB;
    b += YCArray[ADCa][MIC_Chanel] * sinB + YCArray[ADCb][MIC_Chanel] * (cosB);
    a += YCArray[ADCa][MIA_Chanel];
    b += YCArray[ADCb][MIA_Chanel];
    c = sqrt(a * a + b * b) * XS_MI / 3;
    YC[YC_I2] = (int)c;

    if(YC[YC_I2] < 5)
    {
        YC[YC_I2] = 0;
    }

 for(i=1;i<30;i++){YC[i]=i;PhaseAngle[i]=i;}
		 
    //YC[YC_UAB]=1200;
    protection();

}
