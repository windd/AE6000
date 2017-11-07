#ifndef _DW_ADC_H_
#define _DW_ADC_H_

#include "stm32f10x.h"

//====AD开启通道个数======================
#define		ADC_ENB_NUM		14
#define   Num_Adc_Chanel 14


extern uint16_t AD_Value[ADC_ENB_NUM] ; //DMA存储数据的区域

#define ADC1_DR_Address    ((uint32_t)0x4001244C)	 //ADC1数据寄存器的基地址
#define ADC1_CYtime ADC_SampleTime_71Cycles5

//*************************************
// 函数名称：ADC1_Config
// 函数功能：adc1 初始化配置
// 入口参数：无
// 出口参数：无
// 返 回 值：无
//***************************************/
void ADC1_Config(void);

void F_PB7_Init(void);//HZ没用

#endif /* __CMSIS_IAR_H__ */
