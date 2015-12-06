#ifndef _STM32F4_EXTI_H
#define _STM32F4_EXTI_H

#include "stm32f4xx.h"
#include "stm32f4_rcc.h"

typedef struct EXTI_DRIVER_T
{
	GPIO_TypeDef* Gpio;
	RCC_AXXPeriphClockCmd GPIO_CLK;
	uint32_t GPIO_Func;
	uint16_t GPIO_Pin;
	uint8_t EXTI_PortSourceGPIO;
	uint8_t EXTI_PinSource;

	EXTI_InitTypeDef EXIT_Init;
	NVIC_InitTypeDef NVIC_Init;
}EXTI_Driver;

void EXTIx_Init(EXTI_Driver* EXTIx);

#endif
