#ifndef DEFINE_DUSTWIND_H
#define DEFINE_DUSTWIND_H



#include "stm32f10x.h"
#include "typedef.h"

#ifdef DW_6023

	#define DZnum 19
	#define CONnum 19
	//====AD开启通道个数======================
	#define	ADC_ENB_NUM	 14
	#define	Num_Adc_Chanel 14
	
	//YC内容//MI.0-2 PI.3-5 UP.6-8 UL.9-11 I01.12 U0.13 I2.14 U2.15 P.16 Q.17 COS.18 F.19 I02.20
#define YC_MIA 0
#define YC_MIB 1
#define YC_MIC 2
#define YC_PIA 3
#define YC_PIB 4
#define YC_PIC 5
#define YC_UA 6
#define YC_UB 7
#define YC_UC 8
#define YC_UAB 9
#define YC_UBC 10
#define YC_UCA 11
#define YC_I01 12
#define YC_U01 13
#define YC_I02 14
#define YC_U02 15
#define YC_I2 16
#define YC_U2 17
#define YC_F 18
#define YC_P 19
#define YC_Q 20
#define YC_COS 21

//SYS系统参数
#define SYS_ADDR 0
#define SYS_BOND 1
#define SYS_PT 2
#define SYS_CT 3
#define SYS_PIA 4
#define SYS_PIB 5
#define SYS_PIC 6
#define SYS_MIA 7
#define SYS_MIB 8
#define SYS_MIC 9
#define SYS_UA 10
#define SYS_UB 11
#define SYS_UC 12
#define SYS_I01 13
#define SYS_U01 14
#define SYS_I02 20
//#define SYS_U02 21
#define SYS_IP 15
#define SYS_YM 16

extern const unchar  mainmenu[28][7];


#endif

#ifdef DW_BZT

	#define DZnum 6
	#define CONnum 6
	//====AD开启通道个数======================
	#define	ADC_ENB_NUM	 11
	#define	Num_Adc_Chanel 11
	
	//YC内容//MI.0-2 PI.3-5 UP.6-8 UL.9-11 I01.12 U0.13 I2.14 U2.15 P.16 Q.17 COS.18 F.19 I02.20
#define YC_MIA 0
#define YC_MIB 1
#define YC_MIC 2
#define YC_I1 3
#define YC_I2 4
#define YC_UAB1 5
#define YC_UBC1 6
#define YC_UAB2 7
#define YC_UBC2 8
#define YC_UL1 9
#define YC_UL2 10

//SYS系统参数
#define SYS_ADDR 0
#define SYS_BOND 1
#define SYS_PT 2
#define SYS_CT 3
#define SYS_I1 4
#define SYS_I2 5
#define SYS_MIA 6
#define SYS_MIB 7
#define SYS_MIC 8
#define SYS_UAB1 9
#define SYS_UBC1 10
#define SYS_UAB2 11
#define SYS_UBC2 12
#define SYS_UL1 13
#define SYS_UL2 14
#define SYS_IP 15
#define SYS_YM 16

extern const unchar  mainmenu[16][7];

#endif


/*************COM********************/
#define YC_Custom1 30


#define YCnum 40
#define SYSnum 22
#define YXnum 8
#define OUTnum 20
#define SWnum 20
#define COMMnum 10
#define SysErrnum 4

#define  PI 3.1415

//SOE显示属性
#define Property_I 0
#define Property_U 1
#define Property_I0 2
#define Property_U0 3
#define Property_I2 4
#define Property_U2 5
#define Property_F 6
#define Property_P 7
#define Property_Q 8
#define Property_PT 11

//A.0 /V.1 /W.2 /VAR.3 /空.4 /HZ.5 /S.6 测量数据的单位
#define UNIT_I 0
#define UNIT_U 1
#define UNIT_P 2
#define UNIT_Q 3
#define UNIT__ 4
#define UNIT_F 5
#define UNIT_T 6



extern structmenu  MENU[16];
extern const unchar con_sm[CONnum+1];
extern const unchar DZunit[DZnum];
extern const unchar  menuID1112[CONnum][9];
extern const unchar  menuID16[4][8];
extern const unchar Prot_sm[12];

extern const unchar Dis_YC_Content[][7];

#endif


