/*
  October 2012

  aq32Plus Rev -

  Copyright (c) 2012 John Ihlein.  All rights reserved.

  Open Source STM32 Based Multicopter Controller Software

  Includes code and/or ideas from:

  1)AeroQuad
  2)BaseFlight
  3)CH Robotics
  4)MultiWii
  5)S.O.H. Madgwick
  6)UAVX

  Designed to run on the AQ32 Flight Control Board

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

uint8_t magCalibrating = false;

///////////////////////////////////////////////////////////////////////////////
// Mag Calibration
///////////////////////////////////////////////////////////////////////////////

void magCalibration()
{
	uint16_t calibrationCounter = 0;
	uint16_t population[2][3];

	float    d[3000][3];       // 3000 Samples = 60 seconds of data at 50 Hz
	float    sphereOrigin[3];
	float    sphereRadius;

	magCalibrating = true;

	uart1Print("\n\nMagnetometer Calibration:\n\n");

    uart1Print("Rotate magnetometer around all axes multiple times\n");
    uart1Print("Must complete within 60 seconds....\n\n");
    uart1Print("  Send a character when ready to begin and another when complete\n\n");

    while (uart1Available() == false);

    uart1Print("  Start rotations.....\n\n");

    uart1Read();

    while ((uart1Available() == false) && (calibrationCounter <= 3000))
	{
		if (readMag() == true)
		{
			d[calibrationCounter][XAXIS] = (float)rawMag[XAXIS].value * magScaleFactor[XAXIS];
			d[calibrationCounter][YAXIS] = (float)rawMag[YAXIS].value * magScaleFactor[YAXIS];
			d[calibrationCounter][ZAXIS] = (float)rawMag[ZAXIS].value * magScaleFactor[ZAXIS];

			calibrationCounter++;
		}

		delay(20);
	}

	itoa(calibrationCounter, numberString, 10);
	uart1Print("\r\nMagnetometer Bias Calculation ("); uart1Print(numberString); uart1Print(" samples collected out of 3000 max)\n\n");

	sphereFit(d, calibrationCounter, 100, 0.0f, population, sphereOrigin, &sphereRadius);

	eepromConfig.magBias[XAXIS] = sphereOrigin[XAXIS];
	eepromConfig.magBias[YAXIS] = sphereOrigin[YAXIS];
	eepromConfig.magBias[ZAXIS] = sphereOrigin[ZAXIS];

    uart1Print("Magnetometer Bias Values: ");
    ftoa(eepromConfig.magBias[XAXIS], numberString); uart1Print(numberString); uart1Print(", ");
    ftoa(eepromConfig.magBias[YAXIS], numberString); uart1Print(numberString); uart1Print(", ");
    ftoa(eepromConfig.magBias[ZAXIS], numberString); uart1Print(numberString); uart1Print("\n");

	uart1Print("\n\nMagnetometer Calibration Complete.\n\n");

	magCalibrating = false;
}
