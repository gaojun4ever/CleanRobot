#include "stm32f4_rcc.h"

void RCC_SystemCoreClockUpdate(PLL_PARAMS params)
{
	//reset RCC
	RCC_DeInit();
	
	//start HSI, wait until up
	RCC_HSICmd(ENABLE);
  while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	//switch the system clock to HSI
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	while(RCC_GetSYSCLKSource() != 0x00);

	//stop PLL, wait until down
	RCC_PLLCmd(DISABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == SET);

	//start HSE, wait until up
	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

	//change PLL params;
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div4);
	RCC_PCLK2Config(RCC_HCLK_Div2);
	RCC_PLLConfig(RCC_PLLSource_HSE, params.PLLM,
		params.PLLN, params.PLLP, params.PLLQ);

	//start PLL, wait until up
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

	//switch system clock to PLL, wait until switched
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);

	//update
	SystemCoreClockUpdate();
}
