#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include"stm32f10x_rcc.h"
#include"stm32f10x_gpio.h"
#include"stm32f10x_exti.h"
#include"misc.h"
#include"stm32f10x.h"
void EXTI_INIT();
void EXTI15_10_IRQHandler();

#endif