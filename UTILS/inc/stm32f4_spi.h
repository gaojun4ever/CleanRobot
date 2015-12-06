#ifndef __STM32F4_SPI_H
#define __STM32F4_SPI_H

// Includes
#include "stm32f4xx.h"
#include "stm32f4_rcc.h"
//#define SPIx_USE_DMA
#define SPIx_BR_CLEAR_MASK ((uint16_t)(0xFFC7))

typedef struct SPI_DRIVER_T
{
	SPI_TypeDef* SPI;
	RCC_AXXPeriphClockCmd SPI_CLK;
	uint32_t SPI_Func;

	GPIO_TypeDef* Gpio;
	RCC_AXXPeriphClockCmd GPIO_CLK;
	uint32_t GPIO_Func;

	GPIO_TypeDef* Gpio_CS;
	RCC_AXXPeriphClockCmd GPIO_CS_CLK;
	uint32_t CS_Func;
	uint16_t CS_Pin;

	uint16_t SCK_Pin;
	uint16_t MISO_Pin;
	uint16_t MOSI_Pin;
	uint16_t SCK_Src;
	uint16_t MISO_Src;
	uint16_t MOSI_Src;

	SPI_InitTypeDef SPI_Init;

#ifdef SPIx_USE_DMA
	RCC_AXXPeriphClockCmd DMA_CLK;
	uint32_t DMA_Func;
	DMA_TypeDef* DMA_TX;
	DMA_Stream_TypeDef* DMA_TX_Stream;
	NVIC_InitTypeDef NVIC_DMA_TX;
	uint32_t DMA_TX_CH;
	uint32_t DMA_TX_Flag;
	DMA_TypeDef* DMA_RX;
	DMA_Stream_TypeDef* DMA_RX_Stream;
	NVIC_InitTypeDef NVIC_DMA_RX;
	uint32_t DMA_RX_CH;
	uint32_t DMA_RX_Flag;
#endif
	uint8_t GPIO_AF_SPI;

}SPI_Driver;

__inline void Chip_Select(SPI_Driver* SPIx)
{
	GPIO_ResetBits((SPIx)->Gpio_CS, (SPIx)->CS_Pin);
}

__inline void Chip_DeSelect(SPI_Driver* SPIx){
	GPIO_SetBits((SPIx)->Gpio_CS, (SPIx)->CS_Pin);
}

void SPIx_Init(SPI_Driver* SPIx);
void SPIx_DeInit(SPI_Driver* SPIx);
uint8_t SPIx_Read_Reg(SPI_Driver* SPIx, uint8_t reg);
void SPIx_Write_Reg(SPI_Driver* SPIx, uint8_t regAddr, uint8_t data);
void SPIx_Read_Regs(SPI_Driver* SPIx, uint8_t regAddr, uint8_t length, uint8_t* buffer);
#ifdef SPIx_USE_DMA
void SPIx_DMA_Read_Regs(SPI_Driver* SPIx, uint8_t regAddr, uint8_t length, uint8_t* buffer);
#endif
uint8_t SPIx_SendByte(SPI_Driver* SPIx, uint8_t byte);
uint16_t SPIx_SendWord(SPI_Driver* SPIx, uint16_t word);
void SPIx_ReadBytes(SPI_Driver* SPIx, uint8_t length, uint8_t* buffer);
void SPIx_SetDivisor(SPI_Driver* SPIx, uint16_t Prescaler);

#endif
