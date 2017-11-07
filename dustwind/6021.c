

#include "stm32f10x.h"
#include "typedef.h"
#include "menu_content.h"
#include "LCD.h"
#include "i2c.h"
#include "adc.h"
#include "stm32f10x_it.h"
#include "yc.h"
#include "menu.h"
#include "protection.h"
#include "modbusRTU.h"
#include "dz.h"
#include "delay.h"
#include "bianliang.h"
#include "io.h"
#include "soe.h"
#include "w5500s.h"
 
int main(void)
{

    u16 key2up = 0, key3up = 0, key4up = 0, key5up = 0, key6up = 0, key7up = 0;
    u32 i = 0, m = 0, n, o, p = 0, z = 0;
    unchar j = 0, k;
    LightTime = 0;
    SYSCLK_Init();//时钟
    GPIO_Init_DW();//针脚
    LCD_Init();//液晶
    pFdata[0] = 6;
    pFdata[4] = 10; //定值显示用
    RDZ();//读定值
    RSYS();//读系统参数
    RCON();
    RSOEnum();
    ADC1_Config();//模拟量
    F_Init();//测频
    Modbus_init();//通讯
    protection_Init();//保护
    YX_init(1);//遥信

    DisBBH();
        
    w5500_init();

		Delay(20000000);

		LCDclear();
    //  iwdg_init();
    DisMainMenu0();
    DisYC();


    while(1)
    {
        if(RIGHT_KEY == 0 && key2up && i == 0)
        {
            menusub(2);    //right
            j = 1;
            z = 0;
        }
        key2up = RIGHT_KEY;
        if(UP_KEY == 0 && key3up && i == 0)
        {
            menusub(3);    //up
            j = 1;
            z = 0;
        }
        key3up = UP_KEY;
        if(LEFT_KEY == 0 && key4up && i == 0)
        {
            menusub(4);    //left
            j = 1;
            z = 0;
        }
        key4up = LEFT_KEY;
        if(CANCEL_KEY == 0 && key5up && i == 0)
        {
            menusub(5);    //cancel
            j = 1;
            z = 0;
        }
        key5up = CANCEL_KEY;
        if(OK_KEY == 0 && key6up && i == 0)
        {
            menusub(6);    //OK
            j = 1;
            z = 0;
        }
        key6up = OK_KEY;
        if(DOWN_KEY == 0 && key7up && i == 0)
        {
            menusub(7);    //down
            j = 1;
            z = 0;
        }
        key7up = DOWN_KEY;
        if(j == 1)i++;
        if(i > 25000)
        {
            i = 0;
            j = 0;
        }

        m++;
        n++;
        o++;
        LightTime++;
        z++;
        if(DisSOEbit) //自动返回
        {
            if(currmenu.ID > 0  && z == 6000000)
            {
                menusub(5);
                z = 5900000;
            }
            if(currmenu.ID == 0)z = 0;
        }

        if(LightTime > 4000000) //背光
        {
            LIGHT_OFF;
            LightTime = 0;
        }

        if(n > 150000) //运行灯
        {
            if(RUN_LED_BIT > 0)
            {
                RUN_LED_ON;
            }
            else
            {
                RUN_LED_OFF;
            }
            n = 0;
        }

        if(currmenu.ID == 0 && m > 150001 && DisSOEbit)
        {
            DisYC();    //主界面
            m = 1;
        }

        if(currmenu.ID == 10 && m > 150000) //时间
        {
            Rtime();
            LCDWchar(2, 6, 0, Data);
            LCDWchar(3, 6, 0, time);
            m = 1;
        }

        if(o > 100000) //YX
        {
            YX_init(0);
            o = 0;
        }
        if(W5500_Interrupt == 1)w5500_cycle();
        if(currmenu.ID == 5 && m > 150000) //开入测试********
        {
            for(k = 0; k < 8; k++)
            {
                YXcheck[2 * k + 1] = YX[0] >> k & 0x01;
            }
            LCDWchar(3, 1, 0, YXcheck);
            m = 1;
        }

        if(RelayAutoReset > 0 && p == 0)
        {
            p = 1;
        }
        if(p)p++;
        if(p > 300000)
        {
            if(RelayAutoReset & 0x01)
            {
                YH_Relay_op;
                RelayAutoReset &= ~0x01;
            }
            if(RelayAutoReset & 0x02)
            {
                YT_Relay_op;
                RelayAutoReset &= ~0x02;
            }
            if(RelayAutoReset & 0x04)
            {
                T1_Relay_op;
                RelayAutoReset &= ~0x04;
            }
            if(RelayAutoReset & 0x08)
            {
                T2_Relay_op;
                RelayAutoReset &= ~0x08;
            }
            if(RelayAutoReset & 0x10)
            {
                T3_Relay_op;
                RelayAutoReset &= ~0x10;
            }
            if(RelayAutoReset & 0x20)
            {
                SG_Relay_op;
                RelayAutoReset &= ~0x20;
            }
            if(RelayAutoReset & 0x40)
            {
                GZ_Relay_op;
                RelayAutoReset &= ~0x40;
            }
            if(RelayAutoReset & 0x80)
            {
                ZZ_Relay_op;
                RelayAutoReset &= ~0x80;
            }
            if( RelayAutoReset == 0)
                p = 0;
        }

        //if(ARDyc>0)ARDyc--;
        //if(ARDyc==1)ARDaction=0;
        IWDG_ReloadCounter();
    }

}

