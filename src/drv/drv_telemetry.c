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

/*
    DMA UART routines idea lifted from AutoQuad
    Copyright © 2011  Bill Nesbitt
*/

///////////////////////////////////////////////////////////////////////////////
// UART4 Defines and Variables
///////////////////////////////////////////////////////////////////////////////

#define UART4_TX_PIN        GPIO_Pin_10
#define UART4_RX_PIN        GPIO_Pin_11
#define UART4_GPIO          GPIOC
#define UART4_TX_PINSOURCE  GPIO_PinSource10
#define UART4_RX_PINSOURCE  GPIO_PinSource11

#define UART4_BUFFER_SIZE    2048

// Receive buffer, circular DMA
volatile uint8_t rx4Buffer[UART4_BUFFER_SIZE];
uint32_t rx4DMAPos = 0;

volatile uint8_t tx4Buffer[UART4_BUFFER_SIZE];
uint16_t tx4BufferTail = 0;
uint16_t tx4BufferHead = 0;

///////////////////////////////////////////////////////////////////////////////
// UART4 Transmit via DMA
///////////////////////////////////////////////////////////////////////////////

static void uart4TxDMA(void)
{
    DMA2_Channel5->CMAR = (uint32_t)&tx4Buffer[tx4BufferTail];
    if (tx4BufferHead > tx4BufferTail)
    {
    	DMA2_Channel5->CNDTR = tx4BufferHead - tx4BufferTail;
	    tx4BufferTail = tx4BufferHead;
    }
    else
    {
    	DMA2_Channel5->CNDTR = UART4_BUFFER_SIZE - tx4BufferTail;
	    tx4BufferTail = 0;
    }

    DMA_Cmd(DMA2_Channel5, ENABLE);
}

///////////////////////////////////////////////////////////////////////////////
// UART4 TX Complete Interrupt Handler
///////////////////////////////////////////////////////////////////////////////

void DMA2_Channel5_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA2_IT_TC5);
    DMA_Cmd(DMA2_Channel5, DISABLE);

    if (tx4BufferHead != tx4BufferTail)
	    uart4TxDMA();
}

///////////////////////////////////////////////////////////////////////////////
// Telemetry Initialization
///////////////////////////////////////////////////////////////////////////////

void telemetryInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef   DMA_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,    ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,     ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,  ENABLE);

    GPIO_InitStructure.GPIO_Pin   = UART4_TX_PIN | UART4_RX_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

    GPIO_PinAFConfig(UART4_GPIO, UART4_TX_PINSOURCE, GPIO_AF_5);
    GPIO_PinAFConfig(UART4_GPIO, UART4_RX_PINSOURCE, GPIO_AF_5);

    GPIO_Init(UART4_GPIO, &GPIO_InitStructure);

    // DMA TX Interrupt
    NVIC_InitStructure.NVIC_IRQChannel                   = DMA2_Channel5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;

    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    USART_Init(UART4, &USART_InitStructure);

    // Receive DMA into a circular buffer

    DMA_DeInit(DMA2_Channel3);

    DMA_InitStructure.DMA_BufferSize         = UART4_BUFFER_SIZE;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)rx4Buffer;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART4->RDR;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;

    DMA_Init(DMA2_Channel3, &DMA_InitStructure);

    DMA_Cmd(DMA2_Channel3, ENABLE);

    USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);

    rx4DMAPos = DMA_GetCurrDataCounter(DMA2_Channel3);

    // Transmit DMA
    DMA_DeInit(DMA2_Channel5);

    DMA_InitStructure.DMA_BufferSize         = UART4_BUFFER_SIZE;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)tx4Buffer;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &UART4->TDR;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;

    DMA_Init(DMA2_Channel5, &DMA_InitStructure);

    DMA_ITConfig(DMA2_Channel5, DMA_IT_TC, ENABLE);

    DMA2_Channel5->CNDTR = 0;

    USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);

    USART_Cmd(UART4, ENABLE);
}

///////////////////////////////////////////////////////////////////////////////
// Telemetry Available
///////////////////////////////////////////////////////////////////////////////

uint16_t telemetryAvailable(void)
{
    return (DMA_GetCurrDataCounter(DMA2_Channel3) != rx4DMAPos) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
// Telemetry Read
///////////////////////////////////////////////////////////////////////////////

uint8_t telemetryRead(void)
{
    uint8_t ch;

    ch = rx4Buffer[UART4_BUFFER_SIZE - rx4DMAPos];
    // go back around the buffer
    if (--rx4DMAPos == 0)
	    rx4DMAPos = UART4_BUFFER_SIZE;

    return ch;
}

///////////////////////////////////////////////////////////////////////////////
// Telemetry Read Poll
///////////////////////////////////////////////////////////////////////////////

uint8_t telemetryReadPoll(void)
{
    while (!telemetryAvailable()); // wait for some bytes
    return telemetryRead();
}

///////////////////////////////////////////////////////////////////////////////
// Telemetry Write
///////////////////////////////////////////////////////////////////////////////

void telemetryWrite(uint8_t ch)
{
    tx4Buffer[tx4BufferHead] = ch;
    tx4BufferHead = (tx4BufferHead + 1) % UART4_BUFFER_SIZE;

    // if DMA wasn't enabled, fire it up
    if (!(DMA2_Channel5->CCR & 1))
        uart4TxDMA();
}

///////////////////////////////////////////////////////////////////////////////
// Telemetry Print
///////////////////////////////////////////////////////////////////////////////

void telemetryPrint(char *str)
{
    while (*str)
	    telemetryWrite(*(str++));
}

///////////////////////////////////////////////////////////////////////////////
// Telemetry Print Formatted - Print formatted string to Telemetry Port
// From Ala42
///////////////////////////////////////////////////////////////////////////////

void telemetryPrintF(const char * fmt, ...)
{
	char buf[256];

	va_list  vlist;
	va_start (vlist, fmt);

	vsnprintf(buf, sizeof(buf), fmt, vlist);
	telemetryPrint(buf);
	va_end(vlist);
}

///////////////////////////////////////////////////////////////////////////////
