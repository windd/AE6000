#ifndef IO_DUSTWIND_H
#define IO_DUSTWIND_H



#include "stm32f10x.h"
#include "typedef.h"

//#define QF_Relay_cl        GPIO_ResetBits(GPIOB, GPIO_Pin_13);GPIO_SetBits(GPIOB, GPIO_Pin_14)
//#define QF_Relay_op        GPIO_SetBits(GPIOB, GPIO_Pin_13);GPIO_ResetBits(GPIOB, GPIO_Pin_14)

#define YH_Relay_cl GPIO_ResetBits(GPIOG, GPIO_Pin_5)
#define YH_Relay_op GPIO_SetBits(GPIOG, GPIO_Pin_5)

#define YT_Relay_cl GPIO_ResetBits(GPIOG, GPIO_Pin_4)
#define YT_Relay_op GPIO_SetBits(GPIOG, GPIO_Pin_4)

#define T1_Relay_cl GPIO_ResetBits(GPIOG, GPIO_Pin_3)
#define T1_Relay_op GPIO_SetBits(GPIOG, GPIO_Pin_3)

#define T2_Relay_cl GPIO_ResetBits(GPIOG, GPIO_Pin_2)
#define T2_Relay_op GPIO_SetBits(GPIOG, GPIO_Pin_2)

#define T3_Relay_cl GPIO_ResetBits(GPIOD, GPIO_Pin_15)
#define T3_Relay_op GPIO_SetBits(GPIOD, GPIO_Pin_15)

#define SG_Relay_cl GPIO_ResetBits(GPIOD, GPIO_Pin_14)
#define SG_Relay_op GPIO_SetBits(GPIOD, GPIO_Pin_14)

#define T1_Bit (GPIOD->ODR & GPIO_Pin_3)
#define GZ_Bit (GPIOD->ODR & GPIO_Pin_13)

#define GZ_Relay_cl GPIO_ResetBits(GPIOD, GPIO_Pin_13)
#define GZ_Relay_op GPIO_SetBits(GPIOD, GPIO_Pin_13)

#define ZZ_Relay_cl  GPIO_ResetBits(GPIOD, GPIO_Pin_12)
#define ZZ_Relay_op  GPIO_SetBits(GPIOD, GPIO_Pin_12)

#define RUN_LED_ON  GPIO_ResetBits(GPIOE,GPIO_Pin_2)//GPIOD->BRR = GPIO_Pin_5
#define RUN_LED_OFF GPIO_SetBits(GPIOE,GPIO_Pin_2)//GPIOD->BSRR = GPIO_Pin_5

#define RUN_LED_BIT  (GPIOE->ODR&GPIO_Pin_2)

#define COMM_LED_ON  GPIO_ResetBits(GPIOE,GPIO_Pin_1)
#define COMM_LED_OFF GPIO_SetBits(GPIOE,GPIO_Pin_1)

#define QF_H_LED_ON  GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define QF_H_LED_OFF GPIO_SetBits(GPIOB,GPIO_Pin_7)

#define QF_F_LED_ON  GPIO_ResetBits(GPIOE,GPIO_Pin_0)
#define QF_F_LED_OFF GPIO_SetBits(GPIOE,GPIO_Pin_0)

#define SG_LED_ON  GPIO_ResetBits(GPIOD,GPIO_Pin_5)
#define SG_LED_OFF GPIO_SetBits(GPIOD,GPIO_Pin_5)

#define GZ_LED_ON  GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define GZ_LED_OFF GPIO_SetBits(GPIOB,GPIO_Pin_6)

#define In1 (GPIOE->IDR & GPIO_Pin_15)
#define In2 (GPIOE->IDR & GPIO_Pin_14)
#define In3 (GPIOE->IDR & GPIO_Pin_13)
#define In4 (GPIOE->IDR & GPIO_Pin_12)
#define In5 (GPIOE->IDR & GPIO_Pin_11)
#define In6 (GPIOE->IDR & GPIO_Pin_10)
#define In7 (GPIOE->IDR & GPIO_Pin_9)
#define In8 (GPIOE->IDR & GPIO_Pin_8)


#define UP_KEY (GPIOB->IDR & GPIO_Pin_5)
#define LEFT_KEY (GPIOG->IDR & GPIO_Pin_15)
#define CANCEL_KEY (GPIOG->IDR & GPIO_Pin_14)
#define DOWN_KEY (GPIOG->IDR & GPIO_Pin_13)
#define OK_KEY (GPIOD->IDR & GPIO_Pin_7)
#define RIGHT_KEY (GPIOD->IDR & GPIO_Pin_6)

void SYSCLK_Init(void);
void GPIO_Init_DW(void);
void iwdg_init(void);
void RESET_KEY(void);

void YXSOE(u8 bit, u8 num); //

void YX_init(u8 init);//
#endif

