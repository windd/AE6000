


#include "adc.h"

uint16_t AD_Value[ADC_ENB_NUM] ; //DMA存储数据的区域

//*************************************
// 函数名称：ADC1_Config
// 函数功能：adc1 初始化配置
// 入口参数：无
// 出口参数：无
// 返 回 值：无
//***************************************/
void ADC1_Config(void)
{

#ifdef  VECT_TAB_RAM
    /* Set the Vector Table base location at 0x20000000 */
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
    /* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure and enable ADC interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //   NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;

    //  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;

    //  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;

    //  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    //  NVIC_Init(&NVIC_InitStructure);

    //  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    // NVIC_Init(&NVIC_InitStructure);

    // GPIO_InitTypeDef GPIO_InitStructure;
    /* Configure TIM1_CH1 (PA8) as alternate function push-pull */
    //  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    //  GPIO_Init(GPIOA, &GPIO_InitStructure);

    // TIM1 configuration ------------------------------------------------------
    // Time Base configuration
    TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
    TIM_OCInitTypeDef         TIM_OCInitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_DeInit(TIM1);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 36000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    // TIM1 channel1 configuration in PWM mode
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0x1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable; // /*!< Specifies the TIM complementary Output Compare state.
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High; // !< Specifies the complementary output polarity.
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; // !< Specifies the TIM Output Compare pin state during Idle state.
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; // < Specifies the TIM Output Compare pin state during Idle state.
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    //TIM1->CR2=0x08;
    /*

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
     TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
     //重新将Timer设置为缺省值
     TIM_DeInit(TIM1);
     //采用内部时钟给TIM2提供时钟源
     TIM_InternalClockConfig(TIM1);
     //预分频系数为36000-1，这样计数器时钟为72MHz/36000?=?2kHz
     TIM_TimeBaseStructure.TIM_Prescaler=1;
     //设置时钟分割?
     TIM_TimeBaseStructure.TIM_ClockDivision=0x0;//TIM_CKD_DIV1;
     //设置计数器模式为向上计数模式
     TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
     //设置计数溢出大小，每计2000个数就产生一个更新事件
     TIM_TimeBaseStructure.TIM_Period=36000-1;
     //将配置应用到TIM2中
     TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
     //开启定时器2
     TIM_Cmd(TIM1,ENABLE);
        */
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		 //使能DMA时钟
    DMA_DeInit(DMA1_Channel1);		  //开启DMA1的第一通道
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address; //DMA对应的外设基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&AD_Value[0];//内存存储基地址 自己开僻的数组
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//DMA的转换模式为SRC模式，由外设搬移到内存
    DMA_InitStructure.DMA_BufferSize = Num_Adc_Chanel; //DMA缓存大小，N个
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//接收一次数据后，设备地址禁止后移
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//关闭接收一次数据后，目标内存地址后移
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //定义外设数据宽度为16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //DMA搬移数据尺寸，HalfWord就是为16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA_Mode_Circular;//DMA_Mode_Normal;   //转换模式，循环缓存模式。
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//DMA优先级高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  //M2M模式禁用
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, ENABLE);
    /* Enable DMA1 channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);


    ADC_InitTypeDef ADC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);	  //使能ADC
    /* DMA1 channel1 configuration ----------------------------------------------*/
    //RCC_ADCCLKConfig(RCC_PCLK2_Div6) ; //ADC时钟分频 72/6 = 12M
    /* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立的转换模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;		  //开启扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;   //开启连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1; //ADC_ExternalTrigConv_None;	//ADC外部开关，关闭状态
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //对齐方式,ADC为12位中，右对齐方式
    ADC_InitStructure.ADC_NbrOfChannel = ADC_ENB_NUM;	 //开启通道数，15个
    ADC_Init(ADC1, &ADC_InitStructure);
    /* ADC1 regular channel9 configuration */
    //ADC通道组,第9个通道 采样顺序1，转换时间
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC1_CYtime);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC1_CYtime);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC1_CYtime);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC1_CYtime);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 5, ADC1_CYtime);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 6, ADC1_CYtime);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 7, ADC1_CYtime);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 8, ADC1_CYtime);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 9, ADC1_CYtime);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 10, ADC1_CYtime);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 11, ADC1_CYtime);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 12, ADC1_CYtime);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 13, ADC1_CYtime);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 14, ADC1_CYtime);
    //  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 15, ADC1_CYtime);
    //	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 2, ADC_SampleTime_55Cycles5);
    ADC_ExternalTrigConvCmd(ADC1, ENABLE);
    // ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);	  //ADC命令，使能
    ADC_Cmd(ADC1, ENABLE);  //开启ADC1

    ADC_ResetCalibration(ADC1);	  //重新校准
    while(ADC_GetResetCalibrationStatus(ADC1));  //等待重新校准完成
    ADC_StartCalibration(ADC1);		//开始校准
    while(ADC_GetCalibrationStatus(ADC1));	   //等待校准完成

    //  ADC_TempSensorVrefintCmd(ENABLE);

    //TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
    /* TIM1 counter enable */
    TIM_Cmd(TIM1, ENABLE);
    /* TIM1 main Output Enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    /* Test on channel1 transfer complete flag */
    // while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));
    /* Clear channel1 transfer complete flag */
    DMA_ClearFlag(DMA1_FLAG_TC1);

    /* TIM1 counter disable */
    //TIM_Cmd(TIM1, DISABLE);

}

//*************************************
// 函数名称：DMA_Config_ADC1
// 函数功能：DMA 初始化配置
// 入口参数：无
// 出口参数：无
// 返 回 值：无
//***************************************/
/*
void DMA_Config_ADC1(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		 //使能DMA时钟
	DMA_DeInit(DMA1_Channel1);		  //开启DMA1的第一通道
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address; //DMA对应的外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&AD_Value[0];//内存存储基地址 自己开僻的数组
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//DMA的转换模式为SRC模式，由外设搬移到内存
	DMA_InitStructure.DMA_BufferSize = Num_Adc_Chanel; //DMA缓存大小，N个
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//接收一次数据后，设备地址禁止后移
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//关闭接收一次数据后，目标内存地址后移
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //定义外设数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //DMA搬移数据尺寸，HalfWord就是为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   //转换模式，循环缓存模式。
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//DMA优先级高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  //M2M模式禁用
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	// Enable DMA1 channel1
	DMA_Cmd(DMA1_Channel1, ENABLE);
}*/
//===ADC GPIO 初始化=======
/*void ADC1_GPIOInit(void)
{
//-----------PB---------------------------------
	//VR 端口 PB1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  //管脚1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;			 //输入模式
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

*/
//====实例应用=======

//ADC1初始化设置
//	ADC1_GPIOInit();
//	ADC1_Config();
//	DMA_Config_ADC1();	//dma设置

//--主程序-----
/*void mian(void)
{
	while(1)
	{
		while(!F_10MS);
		F_10MS = 0 ;

		Tmp = AD_Value[0] ; //在缓存区中 读取AD结果

	}

}*/



void F_PB7_Init()//HZ没用
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits*/
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    /* Set the Vector Table base location at 0x08000000 */
    // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    EXTI_InitTypeDef EXTI_InitStructure;
    /*connect PB11 to EXTI*/
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);
    /*Config EXTI11*/
    EXTI_InitStructure.EXTI_Line = EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

}
