#include "stm32f10x.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

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

void Send_data(uint8_t Byte){
    USART_SendData(USART1,Byte);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}


int main(void)
{
   
    while (1)
    {
    }
}

