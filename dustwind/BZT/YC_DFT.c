

#include "YC_DFT.h"
#include "yc.h"


#define ADCa Num_Adc_Chanel
#define ADCb Num_Adc_Chanel+1

#define I1_Chanel 0
#define I2_Chanel 1
#define MIA_Chanel 2
#define MIB_Chanel 3
#define MIC_Chanel 4
#define UAB1_Chanel 5
#define UBC1_Chanel 6
#define UAB2_Chanel 7
#define UBC2_Chanel 8
#define UL1_Chanel 9
#define UL2_Chanel 10

//ÏµÊý
#define XS_MI 0.053
#define XS_PI 0.528
#define XS_U  0.555


const float YCsin[20] = {0.0, 0.309, 0.588, 0.809, 0.951, 1.0, 0.951, 0.809, 0.588, 0.309, 0.0, -0.309, -0.588, -0.809, -0.951, -1.0, -0.951, -0.809, -0.588, -0.309};
const float YCcos[20] = {1.0, 0.951, 0.809, 0.588, 0.309, 0.0, -0.309, -0.588, -0.809, -0.951, -1.0, -0.951, -0.809, -0.588, -0.309, 0.0, 0.309, 0.588, 0.809, 0.951};

void YCDFT()
{
    float a = 0, b = 0, c = 0;
    //float a2, a3, b2, b3;
    unchar i; 
		//unchar ia;
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
        case I1_Chanel:
            a = a * SYS[SYS_I1] / 1000;
            b = b * SYS[SYS_I1] / 1000;
            YC[YC_I1] = (int)(sqrt(a * a + b * b) * XS_PI);
            if(YC[YC_I1] < 5)
            {
                YC[YC_I1] = 0;
                a = 1;
                b = 0;
            }

            break;
        case I2_Chanel:
            a = a * SYS[SYS_I2] / 1000;
            b = b * SYS[SYS_I2] / 1000;
            YC[YC_I2] = (int)(sqrt(a * a + b * b) * XS_PI);
            if(YC[YC_I2] < 5)
            {
                YC[YC_I2] = 0;
                a = 1;
                b = 0;
            }

            break;
        case MIA_Chanel:
            a = a * SYS[SYS_MIA] / 1000;
            b = b * SYS[SYS_MIA] / 1000;
            YC[YC_MIA] = (int)(sqrt(a * a + b * b) * XS_PI);
            if(YC[YC_MIA] < 5)
            {
                YC[YC_MIA] = 0;
                a = 1;
                b = 0;
            }

            break;
        case MIB_Chanel:
            a = a * SYS[SYS_MIB] / 1000;
            b = b * SYS[SYS_MIB] / 1000;
            YC[YC_MIB] = (int)(sqrt(a * a + b * b) * XS_PI);
            if(YC[YC_MIB] < 5)
            {
                YC[YC_MIB] = 0;
                a = 1;
                b = 0;
            }
            break;
        case MIC_Chanel:
            a = a * SYS[SYS_MIC] / 1000;
            b = b * SYS[SYS_MIC] / 1000;
            YC[YC_MIC] = (int)(sqrt(a * a + b * b) * XS_PI);
            if(YC[YC_MIC] < 5)
            {
                YC[YC_MIC] = 0;
                a = 1;
                b = 0;
            }
            break;
        case UAB1_Chanel:
            a = a * SYS[SYS_UAB1] / 1000;
            b = b * SYS[SYS_UAB1] / 1000;
            YC[YC_UAB1] = (int)(sqrt(a * a + b * b) * XS_U);
            if(YC[YC_UAB1] < 5)
            {
                YC[YC_UAB1] = 0;
                a = 1;
                b = 0;
            }
            break;
        case UBC1_Chanel:
            YC[YC_UBC1] = (int)(sqrt(a * a + b * b) * XS_U * SYS[SYS_UBC1] / 1000);
            if(YC[YC_UBC1] < 5)
            {
                YC[YC_UBC1] = 0;
                a = 1;
                b = 0;
            }
            break;
        case UAB2_Chanel:
            YC[YC_UAB2] = (int)(sqrt(a * a + b * b) * XS_U * SYS[SYS_UAB2] / 1000);
            if(YC[YC_UAB2] < 5)
            {
                YC[YC_UAB2] = 0;
                a = 1;
                b = 0;
            }
            break;
        case UBC2_Chanel:
            a = a * SYS[SYS_UBC2] / 1000;
            b = b * SYS[SYS_UBC2] / 1000;
            YC[YC_UBC2] = (int)(sqrt(a * a + b * b) * XS_U);
            if(YC[YC_UBC2] < 20)
            {
                YC[YC_UBC2] = 0;
                a = 1;
                b = 0;
            }
            break;
        case UL1_Chanel:
            a = a * SYS[SYS_UL1] / 1000;
            b = b * SYS[SYS_UL1] / 1000;
            YC[YC_UL1] = (int)(sqrt(a * a + b * b) * XS_U);
            if(YC[YC_UL1] < 20)
            {
                YC[YC_UL1] = 0;
                a = 1;
                b = 0;
            }
            break;
        case UL2_Chanel:
            a = a * SYS[SYS_UL2] / 1000;
            b = b * SYS[SYS_UL2] / 1000;
            YC[YC_UL2] = (int)(sqrt(a * a + b * b) * XS_U);
            if(YC[YC_UL2] < 20)
            {
                YC[YC_UL2] = 0;
                a = 1;
                b = 0;
            }
            break;
        }
        YCArray[ADCa][k] = (int)a;
        YCArray[ADCb][k] = (int)b;
    }
 // for(i=0;i<15;i++)YC[i]=i;

    //YC[YC_UAB]=1200;
    protection();
}

