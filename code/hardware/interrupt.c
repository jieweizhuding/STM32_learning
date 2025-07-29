#include "interrupt.h"

void EXTI_INIT(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    GPIO_InitTypeDef GPIO_INIT_structure;
    GPIO_INIT_structure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_INIT_structure.GPIO_Pin=GPIO_Pin_14;
    GPIO_INIT_structure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOB,&GPIO_INIT_structure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);

    EXTI_InitTypeDef EXTI_INIT_structure;
    EXTI_INIT_structure.EXTI_Line=EXTI_Line14;
    EXTI_INIT_structure.EXTI_LineCmd=ENABLE;
    EXTI_INIT_structure.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_INIT_structure.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_INIT_structure);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_INIT_structure;
    NVIC_INIT_structure.NVIC_IRQChannel=EXTI15_10_IRQn;
    NVIC_INIT_structure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_INIT_structure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_INIT_structure.NVIC_IRQChannelSubPriority=1;
    NVIC_Init(&NVIC_INIT_structure);
}

void EXTI15_10_IRQHandler(){
    if(EXTI_GetFlagStatus(EXTI_Line14)==SET){

        //处理函数


        EXTI_ClearFlag(EXTI_Line14);
    }
}