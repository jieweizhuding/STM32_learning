TIM编码器可以通过软件在TIM中断中实现，但十分消耗系统资源，并且进行的操作仅仅是自增、自减这样简单的操作。所以TIM中用硬件实现了编码器。

TIM中的编码器是正交编码器，两个引脚接收正交方波信号，可以选择一个引脚计数或两个引脚计数。
这两个输入引脚必须是TIM的通道1与通道2。一个TIM中只有一个Encoder，在Encoding时，TIM就无法完成其他功能了。stm32f103C一共只有4个TIM，所以编码器硬件还是很珍贵的。如果硬件资源实在不足，则只能用软件实现。

初始化流程为：
![alt text](<截图 2025-08-06 15-48-39.png>)

初始化代码为：
```
TIM_Encoderinit(){
    RCC_AHBPeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef GPIO_structure;
    GPIO_structure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_structure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_structure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_structure);

    //不需要选择时钟源，因为没有使用内部时钟
    TIM_TimeBaseInitTypeDef TIM_BASEstructure;
    TIM_BASEstructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_BASEstructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_BASEstructure.TIM_Period=65536-1;
    TIM_BASEstructure.TIM_Prescaler=0;
    TIM_BASEstructure.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM3,&TIM_BASEstructure);

    TIM_ICInitTypeDef TIM_ICstructure;
    TIM_ICStructInit(&TIM_ICstructure);
    TIM_ICstructure.TIM_Channel=TIM_Channel_1;
    TIM_ICstructure.TIM_ICFilter=0xF;
    TIM_ICInit(TIM3,&TIM_ICstructure);
    TIM_ICstructure.TIM_Channel=TIM_Channel_2;
    TIM_ICstructure.TIM_ICFilter=0xF;
    TIM_ICInit(TIM3,&TIM_ICstructure);

    TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);

    TIM_Cmd(TIM3,ENABLE);
}
```

编码器读取速度需要配合另一个定时器一起使用，另一个定时器负责规定周期，如40Hz的定时器，则读取时：
```
int16_t Get_value(){
    int16_t temp=TIM_GetCounter(TIM3);
    TIM_SetCounter(TIM3,0);
    return temp;
}
```
还要注意的是使用的另一个定时器的周期太短会导致精度较低，过长会导致数据更新过慢，需要自己权衡。
