/*
  December 2012

  aq32Plus_F3 Rev -

  Copyright (c) 2012 John Ihlein.  All rights reserved.

  Open Source STM32 Based Multicopter Controller Software

  Includes code and/or ideas from:

  1)AeroQuad
  2)BaseFlight
  3)CH Robotics
  4)MultiWii
  5)S.O.H. Madgwick
  6)UAVX
  7)STM DiscoveryF3 demonstration software

  Designed to run on the DiscoveryF3 board

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

///////////////////////////////////////////////////////////////////////////////

#include "board.h"

///////////////////////////////////////////////////////////////////////////////

// SPI1
// SCK  PA5
// MISO PA6
// MOSI PA7

// SPI2
// SCK  PB13
// MISO PB14
// MOSI PB15

///////////////////////////////////////////////////////////////////////////////
// SPI Defines and Variables
///////////////////////////////////////////////////////////////////////////////

#define SPI1_GPIO             GPIOA
#define SPI1_SCK_PIN          GPIO_Pin_5
#define SPI1_SCK_PIN_SOURCE   GPIO_PinSource5
#define SPI1_SCK_CLK          RCC_AHBPeriph_GPIOA
#define SPI1_MISO_PIN         GPIO_Pin_6
#define SPI1_MISO_PIN_SOURCE  GPIO_PinSource6
#define SPI1_MISO_CLK         RCC_AHBPeriph_GPIOA
#define SPI1_MOSI_PIN         GPIO_Pin_7
#define SPI1_MOSI_PIN_SOURCE  GPIO_PinSource7
#define SPI1_MOSI_CLK         RCC_AHBPeriph_GPIOA

#define SPI2_GPIO             GPIOB
#define SPI2_SCK_PIN          GPIO_Pin_13
#define SPI2_SCK_PIN_SOURCE   GPIO_PinSource13
#define SPI2_SCK_CLK          RCC_AHBPeriph_GPIOB
#define SPI2_MISO_PIN         GPIO_Pin_14
#define SPI2_MISO_PIN_SOURCE  GPIO_PinSource14
#define SPI2_MISO_CLK         RCC_AHBPeriph_GPIOB
#define SPI2_MOSI_PIN         GPIO_Pin_15
#define SPI2_MOSI_PIN_SOURCE  GPIO_PinSource15
#define SPI2_MOSI_CLK         RCC_AHBPeriph_GPIOB

static volatile uint16_t spi1ErrorCount = 0;
static volatile uint16_t spi2ErrorCount = 0;

///////////////////////////////////////////////////////////////////////////////
// SPI Initialize
///////////////////////////////////////////////////////////////////////////////

void spiInit(SPI_TypeDef *SPIx)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    ///////////////////////////////////

    if (SPIx == SPI1)
    {
    	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    	RCC_AHBPeriphClockCmd(SPI1_SCK_CLK | SPI1_MISO_CLK | SPI1_MOSI_CLK, ENABLE);

    	GPIO_PinAFConfig(SPI1_GPIO, SPI1_SCK_PIN_SOURCE,  GPIO_AF_5);
	    GPIO_PinAFConfig(SPI1_GPIO, SPI1_MISO_PIN_SOURCE, GPIO_AF_5);
	    GPIO_PinAFConfig(SPI1_GPIO, SPI1_MOSI_PIN_SOURCE, GPIO_AF_5);

	    // Init pins
        GPIO_InitStructure.GPIO_Pin   = SPI1_SCK_PIN | SPI1_MISO_PIN | SPI1_MOSI_PIN;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

        GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);

	    RCC_AHBPeriphClockCmd(L3GD20_CS_GPIO_CLK, ENABLE);

	    GPIO_InitStructure.GPIO_Pin   = L3GD20_CS_PIN;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

		GPIO_Init(L3GD20_CS_GPIO, &GPIO_InitStructure);

		GPIO_SetBits(L3GD20_CS_GPIO, L3GD20_CS_PIN);

		SPI_I2S_DeInit(SPI1);

        SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
        SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
        SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
        SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
        SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
        SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
        SPI_InitStructure.SPI_CRCPolynomial     = 7;

        SPI_Init(SPI1, &SPI_InitStructure);

        SPI_RxFIFOThresholdConfig(L3GD20_SPI, SPI_RxFIFOThreshold_QF);

        SPI_Cmd(SPI1, ENABLE);
    }

    ///////////////////////////////////

    if (SPIx == SPI2)
    {
    	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    	RCC_AHBPeriphClockCmd(SPI2_SCK_CLK | SPI2_MISO_CLK | SPI2_MOSI_CLK, ENABLE);

        GPIO_PinAFConfig(SPI2_GPIO, SPI2_SCK_PIN_SOURCE,  GPIO_AF_6);
	    GPIO_PinAFConfig(SPI2_GPIO, SPI2_MISO_PIN_SOURCE, GPIO_AF_6);
	    GPIO_PinAFConfig(SPI2_GPIO, SPI2_MOSI_PIN_SOURCE, GPIO_AF_6);

	    // Init pins
        GPIO_InitStructure.GPIO_Pin   = SPI2_SCK_PIN | SPI2_MISO_PIN | SPI2_MOSI_PIN;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

        GPIO_Init(SPI2_GPIO, &GPIO_InitStructure);
    }

    ///////////////////////////////////
}

///////////////////////////////////////////////////////////////////////////////
// SPI Timeout Callback
///////////////////////////////////////////////////////////////////////////////

uint32_t spiTimeoutUserCallback(SPI_TypeDef *SPIx)
{
	GPIO_SetBits(RED_N_LED_GPIO, RED_N_LED_PIN);

	if (SPIx == SPI1)
	{
		spi1ErrorCount++;
		return spi1ErrorCount;
	}
	else
	{
		spi2ErrorCount++;
		return spi2ErrorCount;
	}
}

///////////////////////////////////////////////////////////////////////////////
// SPI Transfer
///////////////////////////////////////////////////////////////////////////////

uint8_t spiTransfer(SPI_TypeDef *SPIx, uint8_t data)
{
    uint16_t spiTimeout;

    spiTimeout = 0x1000;
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
      if ((spiTimeout--) == 0) return spiTimeoutUserCallback(SPIx);

    SPI_SendData8(SPIx, data);

    spiTimeout = 0x1000;
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
      if ((spiTimeout--) == 0) return spiTimeoutUserCallback(SPIx);

    return((uint8_t)SPI_ReceiveData8(SPIx));
}

///////////////////////////////////////////////////////////////////////////////
// Set SPI Divisor
///////////////////////////////////////////////////////////////////////////////

void setSPIdivisor(SPI_TypeDef *SPIx, uint16_t data)
{
    #define BR_CLEAR_MASK 0xFFC7

	uint16_t tempRegister;

    SPI_Cmd(SPIx, DISABLE);

	tempRegister = SPIx->CR1;

	switch (data)
	{
	case 2:
		tempRegister &= BR_CLEAR_MASK;
	    tempRegister |= SPI_BaudRatePrescaler_2;
	    break;

	case 4:
		tempRegister &= BR_CLEAR_MASK;
	    tempRegister |= SPI_BaudRatePrescaler_4;
	    break;

	case 8:
		tempRegister &= BR_CLEAR_MASK;
	    tempRegister |= SPI_BaudRatePrescaler_8;
	    break;

	case 16:
		tempRegister &= BR_CLEAR_MASK;
	    tempRegister |= SPI_BaudRatePrescaler_16;
	    break;

	case 32:
		tempRegister &= BR_CLEAR_MASK;
	    tempRegister |= SPI_BaudRatePrescaler_32;
	    break;

	case 64:
		tempRegister &= BR_CLEAR_MASK;
	    tempRegister |= SPI_BaudRatePrescaler_64;
	    break;

	case 128:
		tempRegister &= BR_CLEAR_MASK;
	    tempRegister |= SPI_BaudRatePrescaler_128;
	    break;

	case 256:
		tempRegister &= BR_CLEAR_MASK;
	    tempRegister |= SPI_BaudRatePrescaler_256;
	    break;
	}

	SPIx->CR1 = tempRegister;

	SPI_Cmd(SPIx, ENABLE);
}

///////////////////////////////////////////////////////////////////////////////
// Get SPI Error Count
///////////////////////////////////////////////////////////////////////////////

uint16_t spiGetErrorCounter(SPI_TypeDef *SPIx)
{
    if (SPIx == SPI1)
    	return spi1ErrorCount;
    else
        return spi2ErrorCount;
}

///////////////////////////////////////////////////////////////////////////////