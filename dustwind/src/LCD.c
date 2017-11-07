/*
void LCD_Init();
void LCD_WCOM_L(unchar com);
void LCD_WCOM_M(unchar com);
void LCD_WCOM_R(unchar com);
void LCD_WData_L(unchar lcddata);
void LCD_WData_M(unchar lcddata);
void LCD_WData_R(unchar lcddata);
void LCD_clear();
void LCDWword(unchar x,unchar y,unchar len,unchar *word)
void LCDWchar(unchar xx,unchar yy,unchar lenn,unchar *wordd)
*/
#include "lcd.h"
#include "delay.h"
#include "charlib.h"

//e7-rs e8-cs e9-a e10-/res e11-e e12-r/w .e13-cs2
//f10-rs e6-cs e4-a e5-/res f8-e f9-r/w .e13-cs2




void LCD_WCOM_L(unchar com)
{
    uint16_t data = 0;
    RS_OFF;//F10

    RW_OFF;//F9

    CS_OFF;
    E_ON;//F8

    //com = (((com & 0xaaaa) >> 1) | ((com & 0x5555) << 1));
    //com = (((com & 0xcccc) >> 2) | ((com & 0x3333) << 2));
    //com = ((com >> 4) | (com << 4));

    data = 0x100 | (com & 0xFF);

    GPIO_Write(GPIOF, data); //COM
    Delay(10);
    E_OFF;//F8
    CS_ON;
}


void LCD_WData_L(unchar lcddata)
{
    uint16_t data = 0;
    RS_ON;//F10
    RW_OFF;//F9
    CS_OFF;
    E_ON;//F8

    //lcddata = (((lcddata & 0xaaaa) >> 1) | ((lcddata & 0x5555) << 1));
    //lcddata = (((lcddata & 0xcccc) >> 2) | ((lcddata & 0x3333) << 2));
    //lcddata = ((lcddata >> 4) | (lcddata << 4));

    data = 0x500 | (lcddata & 0xFF);

    GPIO_Write(GPIOF, data); //data
    Delay(10);

    E_OFF;//F8
    CS_ON;
}

void LCDclear()
{

    unchar d, b, c;
    for(d = 0; d < 8; d++)
    {
        LCD_WCOM_L(0xb0 + d);
        Delay(1);//3 page adds��
        for(c = 0; c < 9; c++)
        {
            LCD_WCOM_L(0x10 + c);
            Delay(10);//4.1 column addsh��
            for(b = 0; b < 16; b++)
            {
                LCD_WCOM_L(b);
                Delay(10);//4.1 column addsl����ÿ����
                LCD_WData_L(0x00);
                Delay(10);//6 wdata
            }
        }
    }

}
void LCD_Init()
{
    LIGHT_ON;

    RES_OFF;
    Delay(100000);
    RES_ON;

    LCD_WCOM_L(0xe2);
    Delay(10); /*��λ*/
    LCD_WCOM_L(0x2c);
    Delay(10);
    LCD_WCOM_L(0x2e);
    Delay(10);
    LCD_WCOM_L(0x2f);
    Delay(10);

    LCD_WCOM_L(0x23);
    Delay(10);/*�ֵ��Աȶȣ������÷�Χ0x20��0x27*/
    LCD_WCOM_L(0x81); //Delay(10); /*΢���Աȶ�*/
    LCD_WCOM_L(0x2a);
    Delay(10);/*΢���Աȶȵ�ֵ�������÷�Χ0x00��0x3f*/  //20-2D

    LCD_WCOM_L(0xa2);
    Delay(10);/*1/9 ƫѹ�ȣ�bias��*/

    LCD_WCOM_L(0xc8);
    Delay(10);/*��ɨ��˳�򣺴��ϵ���*/   // c8 c0
    LCD_WCOM_L(0xa0);
    Delay(10);/*��ɨ��˳�򣺴�����*/  //  a0 a1

    LCD_WCOM_L(0x60);
    Delay(10);/*��ʼ�У���һ�п�ʼ*/
    LCD_WCOM_L(0xa6);//����
    LCD_WCOM_L(0xaF);
    Delay(10);//����ʾָ��
    LCD_WCOM_L(0xA4);
    Delay(10);
    LCDclear();
}

void LCDWword(unchar x, unchar y, unchar len, const unchar *word) //X �� Y ��ʼ��
{
    // return;
    unchar i, j, a, b;

    x -= 1;
    i = y * WordW;
    for(b = 0; b < 2; b++) //ÿ��
    {

        LCD_WCOM_L(0xb1 + x * 2 - b); //��
        for(j = 0; j < *word; j++) //����ÿ��
        {
            for(a = 0; a < WordW; a++) //ÿ������
            {
                LCD_WCOM_L(0x10 + (i + a + j * WordW + 1) / 0x10);
                LCD_WCOM_L(0x00 + (i + a + j * WordW + 1) & 0x0f);
                if((len == 1) || (len == 3 && j == 0) || (len == 4 && j == 3))
                    LCD_WData_L(~wordlib[*(word + j + 1)][a + b * WordW]);
                else
                    LCD_WData_L(wordlib[*(word + j + 1)][a + b * WordW]);
            }
        }
    }
}
void LCDWchar(unchar x, unchar y, unchar len, const unchar *word) //len��ɫ len=111(������)
{
    // return;
    unchar i, j, a, b;

    x -= 1;
    i = y * CharW;
    for(b = 0; b < 2; b++)
    {

        LCD_WCOM_L(0xb1 + x * 2 - b); //��
        for(j = 0; j < *word; j++)
        {
            for(a = 0; a < CharW; a++)
            {
                LCD_WCOM_L(0x10 + (i + a + j * CharW + 1) / 0x10);
                LCD_WCOM_L(0x00 + (i + a + j * CharW + 1) & 0x0f);
                if(len == 111)
                    LCD_WData_L(~charlib[*(word + j + 1)][a + b * CharW]);
                else if(len == (j + 1))
                    LCD_WData_L(~charlib[*(word + j + 1)][a + b * CharW]);
                else
                    LCD_WData_L(charlib[*(word + j + 1)][a + b * CharW]);


            }
        }
    }
}
