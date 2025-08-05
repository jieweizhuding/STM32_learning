#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "misc.h"


void TIM_init(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    TIM_InternalClockConfig(TIM2);

    TIM_TimeBaseInitTypeDef TIM_structure;
    TIM_structure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_structure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_structure.TIM_Period=7200-1;
    TIM_structure.TIM_Prescaler=10000-1;
    TIM_structure.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM2,&TIM_structure);

    // TIM_ClearFlag(TIM2,TIM_FLAG_Update);

    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_structure;
    NVIC_structure.NVIC_IRQChannel=TIM2_IRQn;
    NVIC_structure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_structure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_structure.NVIC_IRQChannelSubPriority=1;
    NVIC_Init(&NVIC_structure);

    TIM_Cmd(TIM2,ENABLE);
}

void TIM_OCinit(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef GPIO_structure;
    GPIO_structure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_structure.GPIO_Pin=GPIO_Pin_0;
    GPIO_structure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(GPIOA,&GPIO_structure);

    TIM_InternalClockConfig(TIM2);

    TIM_TimeBaseInitTypeDef TIM_BASEstructure;
    TIM_BASEstructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_BASEstructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_BASEstructure.TIM_Period=99;        //ARR
    TIM_BASEstructure.TIM_Prescaler=720-1;  //PSC
    TIM_BASEstructure.TIM_RepetitionCounter=0;

    TIM_TimeBaseInit(TIM2,&TIM_BASEstructure);

    TIM_OCInitTypeDef TIM_OCstructure;
    TIM_OCStructInit(&TIM_OCstructure);
    TIM_OCstructure.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCstructure.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCstructure.TIM_OutputState=TIM_OutputState_Enable;
    TIM_OCstructure.TIM_Pulse=50;           //CCR
    TIM_OC1Init(TIM2,&TIM_OCstructure);

    TIM_Cmd(TIM2,ENABLE);

}



int main(void)
{
    TIM_init();
    while (1)
    {

    }
}

