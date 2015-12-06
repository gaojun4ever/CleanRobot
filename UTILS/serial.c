#include "serial.h"
#include "stm32f4_usart.h"

static USART_Driver Serial=
{
  USART3, RCC_APB1PeriphClockCmd, RCC_APB1Periph_USART3, DEFAULT_BAUDRATE,
	GPIOB, RCC_AHB1PeriphClockCmd , RCC_AHB1Periph_GPIOB, GPIO_Pin_10, GPIO_PinSource10,
	GPIOB, RCC_AHB1PeriphClockCmd , RCC_AHB1Periph_GPIOB, GPIO_Pin_11, GPIO_PinSource11,

	GPIO_AF_USART3
};


void Serial_Init(void)
{
  USARTx_Init(&Serial);
}

int fputc(int ch, FILE *f)
{
    USART_SendData(Serial.USART, (u8) ch);  

    /* Loop until the end of transmission */
    while(USART_GetFlagStatus(Serial.USART, USART_FLAG_TXE) == RESET);


    return ch;
}
