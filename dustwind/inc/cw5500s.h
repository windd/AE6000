#ifndef	_W5500S_H_
#define	_W5500S_H_

/***************************************************************************************
 * 工程名  ：W5500模块－服务端模式例程
 * 描述    ：W5500的端口0工作在服务端模式,则等待《TCP&UDP测试工具》上创建的客户端主动与服务端连接,
 *			 连接成功后，服务端定时给客户端发送字符串"\r\nWelcome To ChuangweiElec!\r\n",
 *			 同时将接收到客户端发来的数据回发给客户端。
 * 实验平台：用户STM32开发板+W5500以太网(TCP/IP)模块
 * 硬件连接：  PC5 -> W5500_RST
 *             PC4 -> W5500_INT
 *             PA4 -> W5500_SCS
 *             PA5 -> W5500_SCK
 *             PA6 -> W5500_MISO
 *             PA7 -> W5500_MOSI
 * 库版本  ：ST_v3.5
***************************************************************************************/

/*例程网络参数*/
//网关：192.168.1.1
//掩码:	255.255.255.0
//物理地址：0C 29 AB 7C 00 01
//本机IP地址:192.168.1.199
//端口0的端口号：5000
//端口0的目的IP地址：192.168.1.190
//端口0的目的端口号：6000

#include "stm32f10x.h"
#include "W5500.h"
#include <string.h>

void RCC_Configuration(void);		//设置系统时钟为72MHZ(这个可以根据需要改)
void NVIC_Configuration(void);		//STM32中断向量表配配置
void Timer2_Init_Config(void);		//Timer2初始化配置
void System_Initialization(void);	//STM32系统初始化函数(初始化STM32时钟及外设)
void Delay55(unsigned int d);			//延时函数(ms)

extern unsigned int Timer2_Counter; //Timer2定时器计数变量(ms)
extern unsigned int W5500_Send_Delay_Counter; //W5500发送延时计数变量(ms)

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

