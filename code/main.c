#include "stm32f10x.h"
#include"stm32f10x_rcc.h"
#include"stm32f10x_adc.h"
#include"stm32f10x_gpio.h"

void ADC_init(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

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

uint16_t get_ADC_result(){
    ADC_SoftwareStartConvCmd(ADC1,ENABLE);
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
    return ADC_GetConversionValue(ADC1);
}

int main(void)
{
    
}


