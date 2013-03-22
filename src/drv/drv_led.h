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

#pragma once

///////////////////////////////////////////////////////////////////////////////
// LED Defines
////////////////////////////////////////////////////////////////////////////////

#define RED_N_LED_PIN      GPIO_Pin_9
#define RED_N_LED_GPIO     GPIOE

#define ORANGE_NE_LED_PIN  GPIO_Pin_10
#define ORANGE_NE_LED_GPIO GPIOE

#define GREEN_E_LED_PIN    GPIO_Pin_11
#define GREEN_E_LED_GPIO   GPIOE

#define BLUE_SE_LED_PIN    GPIO_Pin_12
#define BLUE_SE_LED_GPIO   GPIOE

#define RED_S_LED_PIN      GPIO_Pin_13
#define RED_S_LED_GPIO     GPIOE

#define ORANGE_SW_LED_PIN  GPIO_Pin_14
#define ORANGE_SW_LED_GPIO GPIOE

#define GREEN_W_LED_PIN    GPIO_Pin_15
#define GREEN_W_LED_GPIO   GPIOE

#define BLUE_NW_LED_PIN    GPIO_Pin_8
#define BLUE_NW_LED_GPIO   GPIOE

///////////////////////////////////////

#define RED_N_LED_OFF        GPIO_ResetBits(RED_N_LED_GPIO,      RED_N_LED_PIN)
#define RED_N_LED_ON         GPIO_SetBits(RED_N_LED_GPIO,        RED_N_LED_PIN)
#define RED_N_LED_TOGGLE     GPIO_ToggleBits(RED_N_LED_GPIO,     RED_N_LED_PIN)

#define ORANGE_NE_LED_OFF    GPIO_ResetBits(ORANGE_NE_LED_GPIO,  ORANGE_NE_LED_PIN)
#define ORANGE_NE_LED_ON     GPIO_SetBits(ORANGE_NE_LED_GPIO,    ORANGE_NE_LED_PIN)
#define ORANGE_NE_LED_TOGGLE GPIO_ToggleBits(ORANGE_NE_LED_GPIO, ORANGE_NE_LED_PIN)

#define GREEN_E_LED_OFF      GPIO_ResetBits(GREEN_E_LED_GPIO,    GREEN_E_LED_PIN)
#define GREEN_E_LED_ON       GPIO_SetBits(GREEN_E_LED_GPIO,      GREEN_E_LED_PIN)
#define GREEN_E_LED_TOGGLE   GPIO_ToggleBits(GREEN_E_LED_GPIO,   GREEN_E_LED_PIN)

#define BLUE_SE_LED_OFF      GPIO_ResetBits(BLUE_SE_LED_GPIO,    BLUE_SE_LED_PIN)
#define BLUE_SE_LED_ON       GPIO_SetBits(BLUE_SE_LED_GPIO,      BLUE_SE_LED_PIN)
#define BLUE_SE_LED_TOGGLE   GPIO_ToggleBits(BLUE_SE_LED_GPIO,   BLUE_SE_LED_PIN)

#define RED_S_LED_OFF        GPIO_ResetBits(RED_S_LED_GPIO,      RED_S_LED_PIN)
#define RED_S_LED_ON         GPIO_SetBits(RED_S_LED_GPIO,        RED_S_LED_PIN)
#define RED_S_LED_TOGGLE     GPIO_ToggleBits(RED_S_LED_GPIO,     RED_S_LED_PIN)

#define ORANGE_SW_LED_OFF    GPIO_ResetBits(ORANGE_SW_LED_GPIO,  ORANGE_SW_LED_PIN)
#define ORANGE_SW_LED_ON     GPIO_SetBits(ORANGE_SW_LED_GPIO,    ORANGE_SW_LED_PIN)
#define ORANGE_SW_LED_TOGGLE GPIO_ToggleBits(ORANGE_SW_LED_GPIO, ORANGE_SW_LED_PIN)

#define GREEN_W_LED_OFF      GPIO_ResetBits(GREEN_W_LED_GPIO,    GREEN_W_LED_PIN)
#define GREEN_W_LED_ON       GPIO_SetBits(GREEN_W_LED_GPIO,      GREEN_W_LED_PIN)
#define GREEN_W_LED_TOGGLE   GPIO_ToggleBits(GREEN_W_LED_GPIO,   GREEN_W_LED_PIN)

#define BLUE_NW_LED_OFF      GPIO_ResetBits(BLUE_NW_LED_GPIO,    BLUE_NW_LED_PIN)
#define BLUE_NW_LED_ON       GPIO_SetBits(BLUE_NW_LED_GPIO,      BLUE_NW_LED_PIN)
#define BLUE_NW_LED_TOGGLE   GPIO_ToggleBits(BLUE_NW_LED_GPIO,   BLUE_NW_LED_PIN)

///////////////////////////////////////////////////////////////////////////////
// LED Initialization
///////////////////////////////////////////////////////////////////////////////

void ledInit();

///////////////////////////////////////////////////////////////////////////////
