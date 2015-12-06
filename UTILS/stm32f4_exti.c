
#include "stm32f4_exti.h"

void EXTIx_Init(EXTI_Driver* EXTIx)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  //Enable GPIO clocks
	EXTIx->GPIO_CLK(EXTIx->GPIO_Func, ENABLE);
  //Enable SYSCFG clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  //Configure GPIO pin as input floating
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = EXTIx->GPIO_Pin;
  GPIO_Init(EXTIx->Gpio, &GPIO_InitStructure);

  //Connect EXTI Line to GPIO Pin
	SYSCFG_EXTILineConfig(EXTIx->EXTI_PortSourceGPIO, EXTIx->EXTI_PinSource);

  //Configure EXTI line
  EXTI_Init(&EXTIx->EXIT_Init);

	//Enable and set EXTI Interrupt priority
  NVIC_Init(&EXTIx->NVIC_Init);
}
