#include "stm32f10x.h"

void delay_ms(uint32_t ms);

int main(void)
{
    // 1. 使能 GPIOC 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // 2. 配置 PC13 为推挽输出
    GPIO_InitTypeDef gpioInit;
    gpioInit.GPIO_Pin = GPIO_Pin_13;
    gpioInit.GPIO_Speed = GPIO_Speed_2MHz;
    gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &gpioInit);
    GPIO_SetBits(GPIOC, GPIO_Pin_13);   // 熄灭LED（高电平）
    // GPIO_ResetBits(GPIOC, GPIO_Pin_13); // 点亮LED（低电平）

    // 3. 主循环：LED 闪烁
    while (1)
    {
        // delay_ms(500);
        // delay_ms(500);
    }
}

// 简单的毫秒级延迟函数（阻塞式）
void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms * 8000; i++)
    {
        __NOP();
    }
}