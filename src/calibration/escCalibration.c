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

uint8_t escCalibrating = false;
char    temp;

///////////////////////////////////////////////////////////////////////////////
// ESC Calibration
///////////////////////////////////////////////////////////////////////////////

void escCalibration(void)
{
    escCalibrating = true;

    armed = false;

    uart1Print("\nESC Calibration:\n\n");
    uart1Print("!!!! CAUTION - Remove all propellers and disconnect !!!!\n");
    uart1Print("!!!! flight battery before proceeding any further   !!!!\n\n");
    uart1Print("Type 'Y' to continue, anything other character exits\n\n");

    while (uart1Available() == false);
    temp = uart1Read();
    if (temp != 'Y')
    {
    	uart1Print("ESC Calibration Canceled!!\n\n");
    	escCalibrating = false;
    	return;
    }

    ///////////////////////////////////

    uart1Print("Enter 'h' for Max Command....\n");
    uart1Print("Enter 'm' for Mid Command....\n");
    uart1Print("Enter 'l' for Min Command....\n");
    uart1Print("Enter 'x' to exit....\n\n");

    while(true)
    {
		while (uart1Available() == false);

		temp = uart1Read();

		switch (temp)
		{
			case 'h':
			    uart1Print("Applying Max Command....\n\n");
			    writeAllMotors(eepromConfig.maxThrottle);
			    break;

			case 'm':
			    uart1Print("Applying Mid Command....\n\n");
			    writeAllMotors(eepromConfig.midCommand);
			    break;

			case 'l':
			    uart1Print("Applying Min Command....\n\n");
			    writeAllMotors(MINCOMMAND);
			    break;

			case 'x':
			    uart1Print("Applying Min Command, Exiting Calibration....\n\n");
			    writeAllMotors(MINCOMMAND);
			    escCalibrating = false;
			    return;
			    break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
