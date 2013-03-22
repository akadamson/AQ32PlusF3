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

extern uint8_t cliBusy;

extern uint8_t highSpeedTelem1Enabled;
extern uint8_t highSpeedTelem2Enabled;
extern uint8_t highSpeedTelem3Enabled;
extern uint8_t highSpeedTelem4Enabled;
extern uint8_t highSpeedTelem5Enabled;
extern uint8_t highSpeedTelem6Enabled;
extern uint8_t highSpeedTelem7Enabled;
extern uint8_t highSpeedTelem8Enabled;
extern uint8_t highSpeedTelem9Enabled;

///////////////////////////////////////////////////////////////////////////////
// High Speed Telem Disable
///////////////////////////////////////////////////////////////////////////////

void highSpeedTelemDisable(void);

///////////////////////////////////////////////////////////////////////////////
// Read Float from Uart1 Comm
///////////////////////////////////////////////////////////////////////////////

float readFloatUart1(void);

///////////////////////////////////////////////////////////////////////////////
// Read Character String from Uart1 Comm
///////////////////////////////////////////////////////////////////////////////

char *readStringUart1(char *data, uint8_t length);

///////////////////////////////////////////////////////////////////////////////
// CLI Communication
///////////////////////////////////////////////////////////////////////////////

void cliCom(void);

///////////////////////////////////////////////////////////////////////////////
