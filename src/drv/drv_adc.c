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
//  ADC Defines and Variables
///////////////////////////////////////////////////////////////////////////////

#define VBATT_CONVERTED_VALUE          0
#define DIFF_PRESSURE_CONVERTED_VALUE  1

#define VBATT_PIN              GPIO_Pin_0
#define VBATT_GPIO             GPIOC
#define VBATT_CHANNEL          ADC_Channel_6

#define DIFF_PRESSURE_PIN      GPIO_Pin_3
#define DIFF_PRESSURE_GPIO     GPIOC
#define DIFF_PRESSURE_CHANNEL  ADC_Channel_9

///////////////////////////////////////

uint16_t adc1ConvertedValues[2] =  { 0, 0 };

///////////////////////////////////////

#define VOLTS_PER_BIT   (3.3f / 4096.0f)

///////////////////////////////////////////////////////////////////////////////
//  ADC Initialization
///////////////////////////////////////////////////////////////////////////////

void adcInit(void)
{
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef       ADC_InitStructure;
    DMA_InitTypeDef       DMA_InitStructure;
    GPIO_InitTypeDef      GPIO_InitStructure;

    ADC_CommonStructInit(&ADC_CommonInitStructure);
    ADC_StructInit(&ADC_InitStructure);
    DMA_StructInit(&DMA_InitStructure);
    GPIO_StructInit(&GPIO_InitStructure);

    ///////////////////////////////////

    RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div256);  // 72 MHz divided by 256 = 281.25 kHz

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,  ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);

    ///////////////////////////////////

    DMA_DeInit(DMA1_Channel1);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)adc1ConvertedValues;
  //DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize         = 2;
  //DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
  //DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;

    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel1, ENABLE);

    ///////////////////////////////////

    GPIO_InitStructure.GPIO_Pin   = VBATT_PIN | DIFF_PRESSURE_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  //GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL ;

    GPIO_Init(GPIOC, &GPIO_InitStructure);

    ///////////////////////////////////

    ADC_VoltageRegulatorCmd(ADC1, ENABLE);

    delay(10);

    ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
    ADC_StartCalibration(ADC1);

    while(ADC_GetCalibrationStatus(ADC1) != RESET );

    ///////////////////////////////////

  //ADC_CommonInitStructure.ADC_Mode             = ADC_Mode_Independent;
  //ADC_CommonInitStructure.ADC_Clock            = ADC_Clock_AsynClkMode;
  //ADC_CommonInitStructure.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_DMAMode          = ADC_DMAMode_Circular;
  //ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0;

    ADC_CommonInit(ADC1, &ADC_CommonInitStructure);

    ///////////////////////////////////

    ADC_InitStructure.ADC_ContinuousConvMode    = ADC_ContinuousConvMode_Enable;
  //ADC_InitStructure.ADC_Resolution            = ADC_Resolution_12b;
  //ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;
  //ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
  //ADC_InitStructure.ADC_DataAlign             = ADC_DataAlign_Right;
  //ADC_InitStructure.ADC_OverrunMode           = DISABLE;
  //ADC_InitStructure.ADC_AutoInjMode           = DISABLE;
    ADC_InitStructure.ADC_NbrOfRegChannel       = 2;

    ADC_Init(ADC1, &ADC_InitStructure);

    ///////////////////////////////////

    ADC_RegularChannelConfig(ADC1, VBATT_CHANNEL,         1, ADC_SampleTime_181Cycles5);
    ADC_RegularChannelConfig(ADC1, DIFF_PRESSURE_CHANNEL, 2, ADC_SampleTime_181Cycles5);

    ADC_Cmd(ADC1, ENABLE);

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));

    ADC_DMAConfig(ADC1, ADC_DMAMode_Circular);

    ADC_DMACmd(ADC1, ENABLE);

    ADC_StartConversion(ADC1);
}

///////////////////////////////////////////////////////////////////////////////
//  Compute and return battery voltage
///////////////////////////////////////////////////////////////////////////////

float batteryVoltage(void)
{
	return (float)adc1ConvertedValues[VBATT_CONVERTED_VALUE] * VOLTS_PER_BIT * eepromConfig.batteryVoltageDivider;
}

///////////////////////////////////////////////////////////////////////////////
//  Compute and return differential pressure
///////////////////////////////////////////////////////////////////////////////

float differentialPressure(void)
{
	return (float)adc1ConvertedValues[DIFF_PRESSURE_CONVERTED_VALUE] * VOLTS_PER_BIT;
}

///////////////////////////////////////////////////////////////////////////////

