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
// LSM303DLHC Variables
///////////////////////////////////////////////////////////////////////////////

extern uint8_t accelCalibrating;

extern float   accelOneG;

extern int32_t accelSum100Hz[3];

extern int32_t accelSum500Hz[3];

extern int32_t accelSummedSamples100Hz[3];

extern int32_t accelSummedSamples500Hz[3];

extern int16andUint8_t rawAccel[3];

///////////////////////////////////////

extern float magScaleFactor[3];

extern uint8_t magDataUpdate;

extern uint8_t newMagData;

extern int16andUint8_t rawMag[3];

///////////////////////////////////////////////////////////////////////////////
// Read Accel
///////////////////////////////////////////////////////////////////////////////

void readAccel(void);

///////////////////////////////////////////////////////////////////////////////
// Read Accel Temperature
///////////////////////////////////////////////////////////////////////////////

void readAccelTemperature(void);

///////////////////////////////////////////////////////////////////////////////
// Accel Initialization
///////////////////////////////////////////////////////////////////////////////

void initAccel(void);

///////////////////////////////////////////////////////////////////////////////
// Compute Accel Runtime Data
///////////////////////////////////////////////////////////////////////////////

void computeAccelRTData(void);

///////////////////////////////////////////////////////////////////////////////
// Read Magnetometer
///////////////////////////////////////////////////////////////////////////////

uint8_t readMag(void);

///////////////////////////////////////////////////////////////////////////////
// Magnetometer Initialization
///////////////////////////////////////////////////////////////////////////////

void initMag(void);

///////////////////////////////////////////////////////////////////////////////
