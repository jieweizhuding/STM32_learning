#include "stm32f10x.h"
#include "interrupt.h"
void delay_ms(uint32_t ms);

int main(void)
{
    EXTI_INIT();

    // 3. 主循环：LED 闪烁
    while (1)
    {
    }
}
