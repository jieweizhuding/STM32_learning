模数转换(analog digital convert)是将模拟量转化为数字量，输出数字量为12位，即0-4095,输入电压为0-3.3V，两者线性转化。
在STM32F103C8T6中有两个ADC，ADC1与ADC2，每个ADC有10个输入通道，具体通道定义看引脚定义。
且STM32中的ADC是逐次逼近型ADC，通过二分法得到具体的输出值，所以需要转换时间。我们一般认为AD转换步骤分为：采样、保持、量化、编码四个部分。其中采样、保持事件可以自己定义，量化与编码一般为12.5个ADC周期。一般ADC周期为14MHz，最小采样、保持时间为1.5个ADC周期，所以一个总转化时间为14个ADC周期，为1us。
STM32的ADC还有规则组与注入组两个转化单元，即可以实现批处理与抢占处理。因此STM32定义了四种模式单次转换、非扫描模式，单次转换、扫描模式，多次转换、非扫描模式，多次转换、扫描模式。
ADC校准：STM32在每次转换完成后需要进行校准，具体步骤不用了解，调用代码实现即可。
ADC的触发源：
![alt text](<截图 2025-08-08 15-19-30.png>)

ADC初始化流程：
![alt text](<截图 2025-08-08 15-22-03.png>)

ADC初始化代码：
```
void ADC_init(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //定义在rcc头文件中

    GPIO_InitTypeDef GPIO_structure;
    GPIO_structure.GPIO_Mode=GPIO_Mode_AIN;
    GPIO_structure.GPIO_Pin=GPIO_Pin_0;
    GPIO_structure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_structure);

    ADC_InitTypeDef ADC_structure;
    ADC_structure.ADC_ContinuousConvMode=DISABLE;
    ADC_structure.ADC_DataAlign=ADC_DataAlign_Right;
    ADC_structure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
    ADC_structure.ADC_Mode=ADC_Mode_Independent;
    ADC_structure.ADC_NbrOfChannel=1;
    ADC_structure.ADC_ScanConvMode=DISABLE;
    ADC_Init(ADC1,&ADC_structure);

    ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);

    ADC_Cmd(ADC1,ENABLE);

    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}
```

读取ADC结果的函数：
```
uint16_t get_ADC_result(){
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
    return ADC_GetConversionValue(ADC1);
}
```

其中使用的函数有：
```
//ADC初始化
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct);
//启动ADC
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState);
//ADC中断配置，与看门狗结合使用
void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState);

//校准部分代码
void ADC_ResetCalibration(ADC_TypeDef* ADCx);
FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef* ADCx);
void ADC_StartCalibration(ADC_TypeDef* ADCx);
FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef* ADCx);
void ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);

//该函数一般不使用
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* ADCx);

//中断模式配置
void ADC_DiscModeChannelCountConfig(ADC_TypeDef* ADCx, uint8_t Number);
void ADC_DiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState);

//规则组配置
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
void ADC_ExternalTrigConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);

//获取转换值
uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx);
//双端ADC转换值
uint32_t ADC_GetDualModeConversionValue(void);

//注入组配置
void ADC_AutoInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_InjectedDiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef* ADCx, uint32_t ADC_ExternalTrigInjecConv);
void ADC_ExternalTrigInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_SoftwareStartInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef* ADCx);
void ADC_InjectedChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
void ADC_InjectedSequencerLengthConfig(ADC_TypeDef* ADCx, uint8_t Length);
void ADC_SetInjectedOffset(ADC_TypeDef* ADCx, uint8_t ADC_InjectedChannel, uint16_t Offset);
uint16_t ADC_GetInjectedConversionValue(ADC_TypeDef* ADCx, uint8_t ADC_InjectedChannel);

//看门狗配置
void ADC_AnalogWatchdogCmd(ADC_TypeDef* ADCx, uint32_t ADC_AnalogWatchdog);
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCx, uint16_t HighThreshold, uint16_t LowThreshold);
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel);

//芯片内部温度传感器基准值配置
void ADC_TempSensorVrefintCmd(FunctionalState NewState);

//读写标志位
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);
void ADC_ClearFlag(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);

//读写中断标志位
ITStatus ADC_GetITStatus(ADC_TypeDef* ADCx, uint16_t ADC_IT);
void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, uint16_t ADC_IT);
```