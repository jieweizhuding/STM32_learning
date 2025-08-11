STM32有多种中断，如内核中断、GPIO中断等，这些中断都由NVIC统一控制。
其中GPIO中断、PVD输出、RTC闹钟、USB唤醒、以太网唤醒五个部分又统一交由EXTI(External interrupt)管理。
结构为：
![alt text](<截图 2025-07-29 16-13-51.png>)

我们这里主要使用EXTI中的GPIO中断部分。
EXTI关于GPIO中断的输入端共有16个引脚，而每个GPIO都有16个引脚，所以需要AFIO对GPIO进行复用。
AFIO的作用为中断引脚选择与复用功能引脚重映射。这里我们使用第一个功能。即AFIO类似于数据选择器。多个GPIO的相同的引脚同时只能有一个工作。

而EXTI中输出端EXTI5-EXTI9共用同一引脚输出，EXTI10-EXTI15共用同一引脚输出。使用时需要注意共用的引脚同时只能有一个真正的输出端工作。

而EXTI本身又具有中断响应与事件响应两种方式，中断响应是将中断信号交给NVIC管理，事件响应是将中断信号直接交给对应外设，而不通知CPU。所以EXTI输出部分还有一些引脚直接链接外设。

![alt text](<截图 2025-07-29 16-36-38.png>)

有了上面两个结构图，我们就可以直观地初始化GPIO中断了。

初始化过程代码为：
```
void EXTI_INIT(){
    //为GPIO与AFIO使能时钟
    //定义在rcc头文件
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    //初始化GPIO
    //定义在gpio头文件
    GPIO_InitTypeDef GPIO_INIT_structure;
    GPIO_INIT_structure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_INIT_structure.GPIO_Pin=GPIO_Pin_14;
    GPIO_INIT_structure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOB,&GPIO_INIT_structure);

    //初始化AFIO
    //定义在gpio头文件
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_Pin_14);

    //初始化EXIT
    //定义在exti头文件
    EXTI_InitTypeDef EXTI_INIT_structure;
    EXTI_INIT_structure.EXTI_Line=EXTI_Line14;
    EXTI_INIT_structure.EXTI_LineCmd=ENABLE;
    EXTI_INIT_structure.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_INIT_structure.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_INIT_structure);

    //初始化NVIC
    //定义在misc头文件
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_INIT_structure;
    NVIC_INIT_structure.NVIC_IRQChannel=EXTI15_10_IRQn;
    NVIC_INIT_structure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_INIT_structure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_INIT_structure.NVIC_IRQChannelSubPriority=1;
    NVIC_Init(&NVIC_INIT_structure);
}
```

过程中出现的部分涉及的函数有：
//AFIO事件中断配置
void GPIO_EventOutputConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);
//AFIO复用功能引脚重映射
void GPIO_PinRemapConfig(uint32_t GPIO_Remap, FunctionalState NewState);
//AFIO外部中断配置
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);
//网卡配置
void GPIO_ETH_MediaInterfaceConfig(uint32_t GPIO_ETH_MediaInterface);


//EXTI软件中断
void EXTI_GenerateSWInterrupt(uint32_t EXTI_Line);
//EXTI在main函数中读取EXTI寄存器的值
FlagStatus EXTI_GetFlagStatus(uint32_t EXTI_Line);
//EXTI在main函数中清除EXTI寄存器的值
void EXTI_ClearFlag(uint32_t EXTI_Line);
//EXTI在中断处理函数中读取EXTI寄存器的值
ITStatus EXTI_GetITStatus(uint32_t EXTI_Line);
//EXTI在中断处理函数中清除EXTI寄存器的值
void EXTI_ClearITPendingBit(uint32_t EXTI_Line);



中断处理程序：
```
//中断处理函数放在启动文件中
void EXTI15_10_IRQHandler(){
    if(EXTI_GetFlagStatus(EXTI_Line14)==SET){
        //处理函数

        //在处理后必须清除，否则会一直重复中断处理函数
        EXTI_ClearFlag(EXTI_Line14);
    }
}
```
