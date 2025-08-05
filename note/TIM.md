STM32的定时器可以分为基本定时器、通用定时器、高级定时器三类。
![alt text](<截图 2025-07-30 15-24-21.png>)
STM32F103C8T6共有TIM1-4,四个定时器
其中TIM1、8为高级定时器，在APB2上，因为APB2较快。TIM2-7为低级定时器，在APB1上。

其中定时器的默认频率为72MHz，
计数器技术频率为：$\frac{72MHz}{PSC+1}$,PSC为预分频器的值。
计数器溢出频率为：$\frac{72MHz}{(PSC+1)(ARR+1)}$,ARR为自动重装器，即达到ARR，计数器从零重新计数。

预分频器与计数器为了防止运行中寄存器的值被更改导致的错误，设置了缓冲寄存器，当两者的寄存器的值要被更改时，新来的值会放到缓冲寄存器中，当本次中断操作发生时，再真正改变两者寄存器中的值。当然，也可以自己选择是否启用缓冲寄存器。

因此，为了在初始化时将输入的值立刻放入ARR、PSC寄存器中，库函数会在初始化时手动调用生成一个中断，所以在初始化时会立刻生成一次中断。中断数从1开始。

要想不受该中断的影响，即中断数正常从0开始，需要在时基部分初始化后清除中断标志位即可。即调用TIM_ClearFlag(TIMx,TIM_FLAG_Update)函数。

TIM初始化过程图为：
![alt text](<截图 2025-08-02 17-31-20.png>)

TIM初始化涉及的函数：

void TIM_init(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    TIM_InternalClockConfig(TIM2);

    TIM_TimeBaseInitTypeDef TIM_BASEInitstructure;
    TIM_BASEInitstructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_BASEInitstructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_BASEInitstructure.TIM_Period=7200-1;
    TIM_BASEInitstructure.TIM_Prescaler=10000-1;
    TIM_BASEInitstructure.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM2,&TIM_BASEInitstructure);

    // TIM_ClearFlag(TIM2,TIM_FLAG_Update);

    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);


    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_Initstrucure;
    NVIC_Initstrucure.NVIC_IRQChannel=TIM2_IRQn;
    NVIC_Initstrucure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Initstrucure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_Initstrucure.NVIC_IRQChannelSubPriority=1;
    NVIC_Init(&NVIC_Initstrucure);

    TIM_Cmd(TIM2,ENABLE);

}

//初始化时基模块
void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);
//启动计数器
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState);
//初始化TIM中断信号模块
void TIM_ITConfig(TIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState);
//使用内部时钟源
void TIM_InternalClockConfig(TIM_TypeDef* TIMx);



//配置预分频器
void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint16_t Prescaler, uint16_t TIM_PSCReloadMode);
//配置计数器
void TIM_CounterModeConfig(TIM_TypeDef* TIMx, uint16_t TIM_CounterMode);
//配置是否启动自动重装器的预装功能
void TIM_ARRPreloadConfig(TIM_TypeDef* TIMx, FunctionalState NewState);
//设置计时器的值
void TIM_SetCounter(TIM_TypeDef* TIMx, uint16_t Counter);
//设置自动重装器的值
void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint16_t Autoreload);

//设置、读取中断位寄存器
FlagStatus TIM_GetFlagStatus(TIM_TypeDef* TIMx, uint16_t TIM_FLAG);
void TIM_ClearFlag(TIM_TypeDef* TIMx, uint16_t TIM_FLAG);
ITStatus TIM_GetITStatus(TIM_TypeDef* TIMx, uint16_t TIM_IT);
void TIM_ClearITPendingBit(TIM_TypeDef* TIMx, uint16_t TIM_IT);