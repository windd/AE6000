#ifndef _DW_ADC_H_
#define _DW_ADC_H_

#include "stm32f10x.h"

#include "define.h"


extern uint16_t AD_Value[ADC_ENB_NUM] ; //DMA�洢���ݵ�����

#define ADC1_DR_Address    ((uint32_t)0x4001244C)	 //ADC1���ݼĴ����Ļ���ַ
#define ADC1_CYtime ADC_SampleTime_71Cycles5

//*************************************
// �������ƣ�ADC1_Config
// �������ܣ�adc1 ��ʼ������
// ��ڲ�������
// ���ڲ�������
// �� �� ֵ����
//***************************************/
void ADC1_Config(void);

void F_PB7_Init(void);//HZû��

#endif /* __CMSIS_IAR_H__ */
