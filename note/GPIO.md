GPIO为通用输入、输出口，可配置为8种输出、输出模式，引脚电平为0-3.3V或0-5V。

8种模式为：
1. 开漏输入
2. 上拉输入
3. 下拉输入
4. 模拟输入
5. 推挽输出
    高低电平都有驱动能力，输出的一定为0或1信号。
6. 开漏输出
    只有低电平具有驱动能力，高电平为高阻态，可以认为是断路。作用是可以更改匹配输出的电压或者令多个IO口控制一个输入端。
7. 可复用推挽输出
8. 可复用开漏输出
    复用输出是输出信号由CPU以外的部分控制，比如OC

一个GPIO有16个引脚
寄存器有：
端口配置寄存器低电平 （GPIOx_CRL）（x=A..G） 32位
端口配置寄存器高电平 （GPIOx_CRH）（x=A..G） 32位
端口输入数据寄存器 （GPIOx_IDR） （x=A..G） 32位，其中高16位保留
端口输出数据寄存器 （GPIOx_ODR） （x=A..G） 32位，其中高16位保留
端口位设置/复位寄存器 （GPIOx_BSRR） （x=A..G） 32位
端口位复位寄存器 （GPIOx_BRR） （x=A..G） 32位，其中高16位保留
端口配置锁定寄存器 （GPIOx_LCKR） （x=A..G） 32位，其中高15位保留

则对于每个引脚有：4个配置位，1个输入数据位，1个输出数据位，2个设置位，1个复位位，1个锁位。

标准库GPIO：
1. 初始化：
使用RCC使能GPIOx的时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

配置GPIOx：
GPIO_InitTypeDef gpioInit;
gpioInit.GPIO_Pin = GPIO_Pin_13;
gpioInit.GPIO_Speed = GPIO_Speed_2MHz;
gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_Init(GPIOC, &gpioInit);

2. 输出
//设置单个引脚为高电平
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
//设置单个引脚为低电平
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
//设置单个引脚的电平
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);
//同时设置整个GPIOx的所有引脚
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);
//读取单个引脚的输出值，可以用于对输出值取反
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
//读取整个GPIOx的所有输出值
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);

3. 输入
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);

//因为会有抖动，所以一般需要延时一小会，如20ms左右，且由于检测速度远大于人手，所以需要while循环卡住代码

if(GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)==0){
    delay(20ms);
    while(GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin==0)){
        delay(20ms);
    }


    //到这里才认为按键操作结束
}