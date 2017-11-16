#include "yc.h"


#define XS_MI 0.053
#define XS_PI 0.528
#define XS_U  0.555

//int16_t Chanel_Index[Num_Adc_Chanel]={YC_PIA,YC_PIB,YC_PIC,YC_MIA,YC_MIB,YC_MIC,YC_UA,YC_UB,YC_UC,YC_I01,YC_U01,YC_I02};

int YCArray[Num_Adc_Chanel + 2][20];
unchar  YCx = 0;


void RYC()
{
    unchar i;
    // unint j;
    for(i = 0; i < Num_Adc_Chanel; i++)
    {
        YCArray[i][YCx] = AD_Value[i];
    }
    YCx++;
    if(YCx == 20)
    {
        YCx = 0;
        YCDFT();
    }
}

void F_Init()//TIM4
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    // TIM1 configuration ------------------------------------------------------
    /*       // Time Base configuration
           TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
           TIM_ICInitTypeDef         TIM_ICInitStructure;
           TIM_DeInit(TIM4);

           TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
           TIM_TimeBaseStructure.TIM_Period= 36000;
           TIM_TimeBaseStructure.TIM_Prescaler = 1;
           TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
           TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
           TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_ICStructInit(&TIM_ICInitStructure);
    //TIM_ICInitStructure.TIM_ICMode = TIM_ICMode_ICAP;//TIM使用输入捕获模式;
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x4;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
    */
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM4->CR1 = 0x200;
    TIM4->SMCR = 0xe4;//这个好像可以调整一下下
    TIM4->DIER = 0x10;
    TIM4->CCMR2 = 0x4100;
    TIM4->CCER = 0x1000;
    TIM4->PSC = 0x31;
    TIM4->ARR = 0xffff;
    TIM_Cmd(TIM4, ENABLE);
}
void F()
{
    float a;
    a = (float)1440000 / TIM4->CCR4;
    YC[16] = (unint)(a * 100);
}

