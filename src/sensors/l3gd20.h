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
// L3GD20 Defines and Variables
///////////////////////////////////////////////////////////////////////////////

#define L3GD20_SPI          SPI1

#define L3GD20_CS_GPIO      GPIOE
#define L3GD20_CS_PIN       GPIO_Pin_3
#define L3GD20_CS_GPIO_CLK  RCC_AHBPeriph_GPIOE

#define GYRO_SCALE_FACTOR  0.00030543f  // (17.5e-3) * pi/180  (17.5 mdps/bit)

///////////////////////////////////////

extern float   gyroRTBias[3];

extern int32_t gyroSum500Hz[3];

extern int32_t gyroSummedSamples500Hz[3];

extern int16andUint8_t rawGyro[3];

///////////////////////////////////////

extern uint8_t gyroCalibrating;

///////////////////////////////////////////////////////////////////////////////
// Gyro Initialization
///////////////////////////////////////////////////////////////////////////////

void initGyro(void);

///////////////////////////////////////////////////////////////////////////////
// Read Gyro
///////////////////////////////////////////////////////////////////////////////

void readGyro(void);

///////////////////////////////////////////////////////////////////////////////
// Compute Gyro Runtime Data
///////////////////////////////////////////////////////////////////////////////

void computeGyroRTData(void);

///////////////////////////////////////////////////////////////////////////////
