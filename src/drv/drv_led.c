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
// LED Initialization
///////////////////////////////////////////////////////////////////////////////

void ledInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin   = RED_N_LED_PIN | ORANGE_NE_LED_PIN | GREEN_E_LED_PIN | BLUE_SE_LED_PIN;
	GPIO_InitStructure.GPIO_Pin  |= RED_S_LED_PIN | ORANGE_SW_LED_PIN | GREEN_W_LED_PIN | BLUE_NW_LED_PIN;	                          ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOE, &GPIO_InitStructure);

	RED_N_LED_OFF;
	ORANGE_NE_LED_OFF;
	GREEN_E_LED_OFF;
	BLUE_SE_LED_OFF;
	RED_S_LED_OFF;
	ORANGE_SW_LED_OFF;
	GREEN_W_LED_OFF;
	BLUE_NW_LED_OFF;
}

///////////////////////////////////////////////////////////////////////////////
