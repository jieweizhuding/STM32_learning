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

void TIM_ICinit(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);    //PA6输入
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);     //TIM3 1通道

    GPIO_InitTypeDef GPIO_structure;
    GPIO_structure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_structure.GPIO_Pin=GPIO_Pin_6;
    GPIO_structure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_structure);

    TIM_InternalClockConfig(TIM3);
    TIM_TimeBaseInitTypeDef TIM_Basestructure;
    TIM_Basestructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_Basestructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_Basestructure.TIM_Period=65536-1;   //ARR
    TIM_Basestructure.TIM_Prescaler=72-1;        //PSC
    TIM_Basestructure.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM3,&TIM_Basestructure);

    TIM_ICInitTypeDef TIM_ICstructure;
    TIM_ICstructure.TIM_Channel=TIM_Channel_1;
    TIM_ICstructure.TIM_ICFilter=0xF;
    TIM_ICstructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
    TIM_ICstructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
    TIM_ICstructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
    TIM_ICInit(TIM3,&TIM_ICstructure);

    TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);

    TIM_Cmd(TIM3,ENABLE);
}

void TIM_PWMIinit(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef GPIO_structure;
    GPIO_structure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_structure.GPIO_Pin=GPIO_Pin_6;
    GPIO_structure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_structure);

    TIM_InternalClockConfig(TIM3);
    TIM_TimeBaseInitTypeDef TIM_BASEstructure;
    TIM_BASEstructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_BASEstructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_BASEstructure.TIM_Period=65536-1;
    TIM_BASEstructure.TIM_Prescaler=72-1;
    TIM_BASEstructure.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM3,&TIM_BASEstructure);

    TIM_ICInitTypeDef TIM_ICstrucure;
    TIM_ICstrucure.TIM_Channel=TIM_Channel_1;
    TIM_ICstrucure.TIM_ICFilter=0xF;
    TIM_ICstrucure.TIM_ICPolarity=TIM_ICPolarity_Rising;
    TIM_ICstrucure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
    TIM_ICstrucure.TIM_ICSelection=TIM_ICSelection_DirectTI;
    TIM_PWMIConfig(TIM3,&TIM_ICstrucure);

    TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);

    TIM_Cmd(TIM3,ENABLE);
}

TIM_Encoderinit(){
    RCC_AHBPeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef GPIO_structure;
    GPIO_structure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_structure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_structure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_structure);

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

int main(void)
{
    TIM_init();
    while (1)
    {

    }
}

