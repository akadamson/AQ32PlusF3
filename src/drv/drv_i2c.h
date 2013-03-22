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

#define I2C_LONG_TIMEOUT             ((uint32_t)(10 * I2C_FLAG_TIMEOUT))

#define I2C_OK                       ((uint32_t) 0)
#define I2C_FAIL                     ((uint32_t) 0)

///////////////////////////////////////////////////////////////////////////////
// I2C Initialize
///////////////////////////////////////////////////////////////////////////////

void i2cInit(I2C_TypeDef *I2Cx);

///////////////////////////////////////////////////////////////////////////////
// I2C Write
///////////////////////////////////////////////////////////////////////////////

uint16_t i2cWrite(I2C_TypeDef *I2Cx, uint8_t addr_, uint8_t reg, uint8_t data);

///////////////////////////////////////////////////////////////////////////////
// I2C Read
///////////////////////////////////////////////////////////////////////////////

uint16_t i2cRead(I2C_TypeDef *I2Cx, uint8_t addr_, uint8_t reg, uint16_t len, uint8_t* buf);

///////////////////////////////////////////////////////////////////////////////
// Get I2C Error Count
///////////////////////////////////////////////////////////////////////////////

uint16_t i2cGetErrorCounter(I2C_TypeDef *I2Cx);

///////////////////////////////////////////////////////////////////////////////
