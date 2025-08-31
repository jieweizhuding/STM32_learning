通信协议有：
![alt text](<屏幕截图 2025-08-18 151150.png>)

    双工：两设备之间可以同时发送与接受数据
    半双工：同一时间只能有一个方向的数据流
    单工：永远只能有一个固定方向的数据流

    同步：有数据线控制两设备发送与接受频率
    异步：接受与发送设备的频率需要遵守提前的规定

    单端：低电平由接地规定
    差分：不需要GND

    点对点：两设备间传输，无需寻址
    多设备：传输时需要寻址

#### 串口：
串口是一种十分简单的通信接口
#### USART：
USART是串口的一种，本身是一个外设，被集成在STM32内部
stm32有USART1、2、3

USART初始化流程：
![alt text](<屏幕截图 2025-08-18 155559.png>)

发送部分初始化代码为：
```
void USART_init(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

    GPIO_InitTypeDef GPIO_structure;
    GPIO_structure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_structure.GPIO_Pin=GPIO_Pin_9;
    GPIO_structure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_structure);

    USART_InitTypeDef USART_structure;
    USART_structure.USART_BaudRate=9600;
    USART_structure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_structure.USART_Mode=USART_Mode_Tx;
    USART_structure.USART_Parity=USART_Parity_No;
    USART_structure.USART_StopBits=USART_StopBits_1;
    USART_structure.USART_WordLength=USART_WordLength_8b;
    USART_Init(USART1,&USART_structure);

    USART_Cmd(USART1,ENABLE);
}
```
可以看到函数都比较基础无需特别注意，剩下没有用到的函数都无需了解。

发送数据代码：
```
void Send_data(uint8_t Byte){
    USART_SendData(USART1,Byte);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}


void Send_string(char* s){
  while(*s!=0){
    Send_data(*s);
    s++;
  }
}
```

在接收时有轮询与中断两种模式，轮询即在main函数中不断重复读取，所以一般使用中断较多。
初始化代码：
```
void USART_init(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

    GPIO_InitTypeDef GPIO_structure;
    GPIO_structure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_structure.GPIO_Pin=GPIO_Pin_9;
    GPIO_structure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_structure);
    GPIO_structure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_structure.GPIO_Pin=GPIO_Pin_10;
    GPIO_structure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_structure);


    USART_InitTypeDef USART_structure;
    USART_structure.USART_BaudRate=115200;
    USART_structure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_structure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
    USART_structure.USART_Parity=USART_Parity_No;
    USART_structure.USART_StopBits=USART_StopBits_1;
    USART_structure.USART_WordLength=USART_WordLength_8b;
    USART_Init(USART1,&USART_structure);

    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_structure;
    NVIC_structure.NVIC_IRQChannel=USART1_IRQn;
    NVIC_structure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_structure.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_structure.NVIC_IRQChannelSubPriority=0;
    NVIC_Init(&NVIC_structure);
    USART_Cmd(USART1,ENABLE);
}
```

接收代码：
```
void USART1_IRQHandler()
{
  static uint8_t RX_State = 0;
  static uint8_t RX_Num = 0;
  if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
  {
    char RX_DATA = USART_ReceiveData(USART1);
    if (RX_State == 0)
    {
      if (RX_DATA == '@')
      {
        RX_State = 1;
        RX_Num = 0;
      }
    }
    else if (RX_State == 1)
    {
      if (RX_DATA == '\r')
      {
        RX_State = 2;
      }
      else
      {
        Rec_Data[RX_Num] = RX_DATA;
        RX_Num++;
      }
    }
    else if (RX_State == 2)
    {
      if (RX_DATA == '\n')
      {
        RX_State = 0;
        Rec_Data[RX_Num] = '\0';
        Rec_Flag = 1;
      }
    }
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }
}
```