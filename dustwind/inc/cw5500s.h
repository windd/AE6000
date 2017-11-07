#ifndef	_W5500S_H_
#define	_W5500S_H_

/***************************************************************************************
 * ������  ��W5500ģ�飭�����ģʽ����
 * ����    ��W5500�Ķ˿�0�����ڷ����ģʽ,��ȴ���TCP&UDP���Թ��ߡ��ϴ����Ŀͻ�����������������,
 *			 ���ӳɹ��󣬷���˶�ʱ���ͻ��˷����ַ���"\r\nWelcome To ChuangweiElec!\r\n",
 *			 ͬʱ�����յ��ͻ��˷��������ݻط����ͻ��ˡ�
 * ʵ��ƽ̨���û�STM32������+W5500��̫��(TCP/IP)ģ��
 * Ӳ�����ӣ�  PC5 -> W5500_RST
 *             PC4 -> W5500_INT
 *             PA4 -> W5500_SCS
 *             PA5 -> W5500_SCK
 *             PA6 -> W5500_MISO
 *             PA7 -> W5500_MOSI
 * ��汾  ��ST_v3.5
***************************************************************************************/

/*�����������*/
//���أ�192.168.1.1
//����:	255.255.255.0
//�����ַ��0C 29 AB 7C 00 01
//����IP��ַ:192.168.1.199
//�˿�0�Ķ˿ںţ�5000
//�˿�0��Ŀ��IP��ַ��192.168.1.190
//�˿�0��Ŀ�Ķ˿ںţ�6000

#include "stm32f10x.h"
#include "W5500.h"
#include <string.h>

void RCC_Configuration(void);		//����ϵͳʱ��Ϊ72MHZ(������Ը�����Ҫ��)
void NVIC_Configuration(void);		//STM32�ж�������������
void Timer2_Init_Config(void);		//Timer2��ʼ������
void System_Initialization(void);	//STM32ϵͳ��ʼ������(��ʼ��STM32ʱ�Ӽ�����)
void Delay55(unsigned int d);			//��ʱ����(ms)

extern unsigned int Timer2_Counter; //Timer2��ʱ����������(ms)
extern unsigned int W5500_Send_Delay_Counter; //W5500������ʱ��������(ms)

void W5500_Initialization(void);
void Load_Net_Parameters(void);

void W5500_Socket_Set(void);
void Process_Socket_Data(SOCKET s);
void w5500_init(void);
void w5500_cycle(void);
void RCC_Configuration(void);
void NVIC_Configuration(void);
void Timer2_Init_Config(void);
void TIM2_IRQHandler(void);
void System_Initialization(void);
void Delay55(unsigned int d);

#endif

