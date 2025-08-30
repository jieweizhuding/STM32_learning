输入捕获可以测量输入信号的频率，
PWMI可以同时测量频率与占空比。

测量频率方法有两种为：
当f_x < f_m时，测频法更好，反之，测周法更好。
![alt text](<截图 2025-08-06 08-51-36.png>)

与OC类似，一个TIM也有四个IC通道，简化电路如图：
![alt text](<截图 2025-08-06 08-57-39.png>)
该电路实现的是测周法。
注意到电路中使用了主从模式。主从模式是主模式、从模式、主从触发源选择三个部分的简称。

主模式可以将定时器内部的信号映射到TRGO引脚，用于触发别的外设。
从模式是接受其他外设或自身外设的信号，用于控制定时器自身的运行。

![alt text](<截图 2025-08-06 09-06-09.png>)
通过使用主从模式可以实现自动清零CNT，否则需要手动捕获中断信号，在处理函数中清零CNT，这样会导致TIM频繁处于中断中，严重影响效率。

IC初始化流程为：
![alt text](<截图 2025-08-06 09-19-44.png>)


PWMI是使用了两个输入通道，与IC原理类似。
PWMI初始化流程为：
![alt text](<截图 2025-08-06 09-21-40.png>)


IC使用的函数有：
```
//与OCinit不同的是，IC的四个通道共用一个配置函数，便于后续通道选择
void TIM_ICInit(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct);

//选择从模式触发源TRGI
void TIM_SelectInputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource);

//选择主模式的输出触发源TRGO
void TIM_SelectOutputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_TRGOSource);

//选择从模式输出触发源
void TIM_SelectSlaveMode(TIM_TypeDef* TIMx, uint16_t TIM_SlaveMode);

//设置四个IC通道的预分频器的值
void TIM_SetIC1Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC);
void TIM_SetIC2Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC);
void TIM_SetIC3Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC);
void TIM_SetIC4Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC);

//读取四个IC通道的CCR
uint16_t TIM_GetCapture1(TIM_TypeDef* TIMx);
uint16_t TIM_GetCapture2(TIM_TypeDef* TIMx);
uint16_t TIM_GetCapture3(TIM_TypeDef* TIMx);
uint16_t TIM_GetCapture4(TIM_TypeDef* TIMx);
```

IC初始化代码为：
```
void TIM_ICinit(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);    //PA6输入
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);     //TIM3 1通道

    GPIO_InitTypeDef GPIO_structure;
    GPIO_structure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_structure.GPIO_Pin=GPIO_Pin_6;
    GPIO_structure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_structure);

    TIM_InternalClockConfig(TIM3);
    TIM_TimeBaseInitTypeDef TIM_Basestructure;
    TIM_Basestructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_Basestructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_Basestructure.TIM_Period=65536-1;   //ARR
    TIM_Basestructure.TIM_Prescaler=72-1;        //PSC
    TIM_Basestructure.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM3,&TIM_Basestructure);

    TIM_ICInitTypeDef TIM_ICstructure;
    TIM_ICstructure.TIM_Channel=TIM_Channel_1;
    TIM_ICstructure.TIM_ICFilter=0xF;
    TIM_ICstructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
    TIM_ICstructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
    TIM_ICstructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
    TIM_ICInit(TIM3,&TIM_ICstructure);

    TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);

    TIM_Cmd(TIM3,ENABLE);
}
```

当我们要获取频率时，需要知道

$$
f=\frac{72M}{PSC*CCR}
$$

直接计算即可得到

对PWMI需要注意的是，通道1与通道2配置一般相反对称，所以使用TIM_PWMIConfig函数默认封装了这样配置，但通道3和4没有这样的定义，所以需要该函数不能用于初始化3,4通道
PWMI使用的函数：
```
void TIM_PWMIConfig(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct);
```

PWMI初始化代码：
与IC初始化类似，只需要修改IC初始化部分为PWMI初始化即可。
```
void TIM_PWMIinit(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef GPIO_structure;
    GPIO_structure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_structure.GPIO_Pin=GPIO_Pin_6;
    GPIO_structure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_structure);

    TIM_InternalClockConfig(TIM3);
    TIM_TimeBaseInitTypeDef TIM_BASEstructure;
    TIM_BASEstructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_BASEstructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_BASEstructure.TIM_Period=65536-1;
    TIM_BASEstructure.TIM_Prescaler=72-1;
    TIM_BASEstructure.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM3,&TIM_BASEstructure);

    TIM_ICInitTypeDef TIM_ICstrucure;
    TIM_ICstrucure.TIM_Channel=TIM_Channel_1;
    TIM_ICstrucure.TIM_ICFilter=0xF;
    TIM_ICstrucure.TIM_ICPolarity=TIM_ICPolarity_Rising;
    TIM_ICstrucure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
    TIM_ICstrucure.TIM_ICSelection=TIM_ICSelection_DirectTI;
    TIM_PWMIConfig(TIM3,&TIM_ICstrucure);

    TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);

    TIM_Cmd(TIM3,ENABLE);
}
```
因为使用的是通道一与通道二，且TI1FP1作为从模式触发源，则CCR1为整个周期的CNT，用于计算频率，CCR2中为高电平的CNT，故占空比为：

$$
P=\frac{CCR2}{CCR1}
$$
