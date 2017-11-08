
#include "io.h"
#include "bianliang.h"
#include "soe.h"
#include "protection.h"

void RESET_KEY()
{
    //  GZ_OFF;SG_OFF;QFcloseRelay_op;QFopenRelay_op;GZ_LED_OFF;SG_LED_OFF;YX[3]&=0xFE;
}
void YXSOE(u8 bit, u8 num)
{
    if(num < 8) SOEdata[12] = (bit >> num) & 0x01;
    SOEdata[9] = num;
    SOEdata[8] = 1;
    SOEdata[11] = bit;
    WSOE();
}
void YX_init(u8 init)
{
    u8 a = 0, i;
    if(In1)
    {
        a &= ~GPIO_Pin_0;    //·Ö
        QF_F_LED_OFF;
    }
    else
    {
        a |= GPIO_Pin_0;
        QF_F_LED_ON;
    }
    if(In2)
    {
        a &= ~GPIO_Pin_1;    //ºÏ
        QF_H_LED_OFF;
    }
    else
    {
        a |= GPIO_Pin_1;
        QF_H_LED_ON;
    }

    if(In3)
    {
        a &= ~GPIO_Pin_2;
    }
    else
    {
        a |= GPIO_Pin_2;
    }
    if(In4)
    {
        a &= ~GPIO_Pin_3;
    }
    else
    {
        a |= GPIO_Pin_3;
    }
    if(In5)
    {
        a &= ~GPIO_Pin_4;
    }
    else
    {
        a |= GPIO_Pin_4;
    }
    if(In6)
    {
        a &= ~GPIO_Pin_5;
    }
    else
    {
        a |= GPIO_Pin_5;
    }
    if(In7)
    {
        a &= ~GPIO_Pin_6;
    }
    else
    {
        a |= GPIO_Pin_6;
    }
    if(In8)
    {
        a &= ~GPIO_Pin_7;
    }
    else
    {
        a |= GPIO_Pin_7;
    }

    if(init == 0)
    {
        if(((a >> 1 ^ a) & 0x01) == 0)
        {
            if((YX[3] & 0x01) == 0)
            {
                YXSOE(a, 15);
                YX[0] = a;
                YX[3] |= 0x01;
                GZ_LED_ON;
            }
        }
        else YX[3] &= 0xFE;

        for(i = 0; i < 8; i++)
        {
            if((YX[0] >> i) + (a >> i) & 0x01)
                YXSOE(a, i);
        }
    }
    YX[0] = a;
		IOprotection();
}
void SYSCLK_Init()
{
    RCC_ClocksTypeDef RCC_Clocks;
    // RCC_GetClocksFreq(&RCC_Clocks);
    FLASH->ACR = 0x32;
    ErrorStatus HSEStartUpStatus;
    RCC_HSEConfig(RCC_HSE_ON);
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    if(HSEStartUpStatus == SUCCESS)
    {
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
        RCC_ADCCLKConfig(RCC_PCLK2_Div6);
        RCC_PCLK1Config(RCC_HCLK_Div2);
        RCC_PLLCmd(ENABLE);
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    }
    RCC_GetClocksFreq(&RCC_Clocks);
}
void GPIO_Init_DW()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2; //LED
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //LED
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //LED
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //e7-rs e8-cs e9-a e10-/res e11-e e12-r/w .e13-cs2
    //f10-rs e6-cs e4-a e5-/res f8-e f9-r/w .e13-cs2
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10; //LCD
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6; //LCD
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    //f8-f15 d0-d7
    //f0-f7 d7-d0
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; //LCD DataBus
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//KEY
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //KEY
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //KEY
    GPIO_Init(GPIOD, &GPIO_InitStructure);



    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; //AI
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7; //AI
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //AI
    GPIO_Init(GPIOB, &GPIO_InitStructure);



    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//F
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; //relay
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //relay
    GPIO_Init(GPIOD, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8; //YX
    GPIO_Init(GPIOE, &GPIO_InitStructure);



    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//RS485EN
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//RS485 TX
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//RS485 RX
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //I2C
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    YH_Relay_op;

    YT_Relay_op;

    T1_Relay_op;

    T2_Relay_op;

    T3_Relay_op;

    SG_Relay_op;

    GZ_Relay_op;

    //   GZ_Relay_op;

    //   SG_Relay_op;

    ZZ_Relay_op;

    RUN_LED_OFF;
    COMM_LED_OFF;
    QF_F_LED_OFF;
    QF_H_LED_OFF;
    GZ_LED_OFF;
    SG_LED_OFF;

}

void iwdg_init()
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_8);
    IWDG_SetReload(0xfa0);
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);
    IWDG_Enable();
}
