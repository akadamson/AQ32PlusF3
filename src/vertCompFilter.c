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
// Vertical Complementary Filter Defines and Variables
///////////////////////////////////////////////////////////////////////////////

float   accelZ;
float   estimationError = 0.0f;
float   hDotEstimate    = 0.0f;
float   hEstimate;
uint8_t previousExecUp  = false;

///////////////////////////////////////////////////////////////////////////////
// Vertical Complementary Filter
///////////////////////////////////////////////////////////////////////////////

void vertCompFilter(float dt)
{
    if ((execUp == true) && (previousExecUp == false))
    	hEstimate = sensors.pressureAlt10Hz;

    previousExecUp = execUp;

	if (execUp == true)
    {
    	accelZ = -earthAxisAccels[ZAXIS] + eepromConfig.compFilterB * estimationError;

        hDotEstimate += accelZ * dt;

        hEstimate += (hDotEstimate + eepromConfig.compFilterA * estimationError) * dt;

        estimationError = sensors.pressureAlt10Hz - hEstimate;
    }
}

///////////////////////////////////////////////////////////////////////////////




