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
    Copyright � 2011  Bill Nesbitt
*/

///////////////////////////////////////////////////////////////////////////////
// UART1 Defines and Variables
///////////////////////////////////////////////////////////////////////////////

#define UART1_TX_PIN        GPIO_Pin_4
#define UART1_RX_PIN        GPIO_Pin_5
#define UART1_GPIO          GPIOC
#define UART1_TX_PINSOURCE  GPIO_PinSource4
#define UART1_RX_PINSOURCE  GPIO_PinSource5

#define UART1_BUFFER_SIZE 2048

// Receive buffer, circular DMA
volatile uint8_t rx1Buffer[UART1_BUFFER_SIZE];
uint32_t rx1DMAPos = 0;

volatile uint8_t tx1Buffer[UART1_BUFFER_SIZE];
uint32_t tx1BufferTail = 0;
uint32_t tx1BufferHead = 0;

///////////////////////////////////////////////////////////////////////////////
// UART1 Transmit via DMA
///////////////////////////////////////////////////////////////////////////////

static void uart1TxDMA(void)
{
    DMA1_Channel4->CMAR = (uint32_t) & tx1Buffer[tx1BufferTail];
    if (tx1BufferHead > tx1BufferTail)
    {
        DMA1_Channel4->CNDTR = tx1BufferHead - tx1BufferTail;
        tx1BufferTail = tx1BufferHead;
    }
    else
    {
        DMA1_Channel4->CNDTR = UART1_BUFFER_SIZE - tx1BufferTail;
        tx1BufferTail = 0;
    }

    DMA_Cmd(DMA1_Channel4, ENABLE);
}

///////////////////////////////////////////////////////////////////////////////
// UART1 TX Complete Interrupt Handler
///////////////////////////////////////////////////////////////////////////////

void DMA1_Channel4_IRQHandler(void)
{
    DMA_ClearITPendingBit(DMA1_IT_TC4);
    DMA_Cmd(DMA1_Channel4, DISABLE);

    if (tx1BufferHead != tx1BufferTail)
        uart1TxDMA();
}

///////////////////////////////////////////////////////////////////////////////
// UART1 Initialization
///////////////////////////////////////////////////////////////////////////////

void uart1Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef   DMA_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,    ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,     ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = UART1_TX_PIN | UART1_RX_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

    GPIO_PinAFConfig(UART1_GPIO, UART1_TX_PINSOURCE, GPIO_AF_7);
    GPIO_PinAFConfig(UART1_GPIO, UART1_RX_PINSOURCE, GPIO_AF_7);

    GPIO_Init(UART1_GPIO, &GPIO_InitStructure);

    // DMA TX Interrupt
    NVIC_InitStructure.NVIC_IRQChannel                   = DMA1_Channel4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;

    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);

    // Receive DMA into a circular buffer

    DMA_DeInit(DMA1_Channel5);

    DMA_InitStructure.DMA_BufferSize         = UART1_BUFFER_SIZE;
	DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)rx1Buffer;
	DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->RDR;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;

    DMA_Init(DMA1_Channel5, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel5, ENABLE);

    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

    rx1DMAPos = DMA_GetCurrDataCounter(DMA1_Channel5);

    // Transmit DMA
    DMA_DeInit(DMA1_Channel4);

    DMA_InitStructure.DMA_BufferSize         = UART1_BUFFER_SIZE;
	DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)tx1Buffer;
	DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &USART1->TDR;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;

    DMA_Init(DMA1_Channel4, &DMA_InitStructure);

    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);

    DMA1_Channel4->CNDTR = 0;

    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

    USART_Cmd(USART1, ENABLE);
}

///////////////////////////////////////////////////////////////////////////////
// UART1 Avaialble
///////////////////////////////////////////////////////////////////////////////

uint16_t uart1Available(void)
{
    return (DMA_GetCurrDataCounter(DMA1_Channel5) != rx1DMAPos) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
// UART1 Read
///////////////////////////////////////////////////////////////////////////////

uint8_t uart1Read(void)
{
    uint8_t ch;

    ch = rx1Buffer[UART1_BUFFER_SIZE - rx1DMAPos];
    // go back around the buffer
    if (--rx1DMAPos == 0)
        rx1DMAPos = UART1_BUFFER_SIZE;

    return ch;
}

///////////////////////////////////////////////////////////////////////////////
// UART1 Read Poll
///////////////////////////////////////////////////////////////////////////////

uint8_t uart1ReadPoll(void)
{
    while (!uart1Available()); // wait for some bytes
    return uart1Read();
}

///////////////////////////////////////////////////////////////////////////////
// UART1 Write
///////////////////////////////////////////////////////////////////////////////

void uart1Write(uint8_t ch)
{
    tx1Buffer[tx1BufferHead] = ch;
    tx1BufferHead = (tx1BufferHead + 1) % UART1_BUFFER_SIZE;

    // if DMA wasn't enabled, fire it up
    if (!(DMA1_Channel4->CCR & 1))
        uart1TxDMA();
}

///////////////////////////////////////////////////////////////////////////////
// UART1 Print
///////////////////////////////////////////////////////////////////////////////

void uart1Print(char *str)
{
    while (*str)
        uart1Write(*(str++));
}

///////////////////////////////////////////////////////////////////////////////