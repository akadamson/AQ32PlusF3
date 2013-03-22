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
// Accelerometer Calibration
///////////////////////////////////////////////////////////////////////////////

void accelCalibration(void)
{
    float noseUp        = 0.0f;
    float noseDown      = 0.0f;
    float leftWingDown  = 0.0f;
    float rightWingDown = 0.0f;
    float upSideDown    = 0.0f;
    float rightSideUp   = 0.0f;

    int16_t index;

    accelCalibrating = true;

    uart1Print("\nAccelerometer Calibration:\n\n");

    ///////////////////////////////////

    uart1Print("Place accelerometer right side up\n");
    uart1Print("  Send a character when ready to proceed\n\n");

    while (uart1Available() == false);
    uart1Read();

    uart1Print("  Gathering Data...\n\n");

    for (index = 0; index < 5000; index++) {
        readAccel();
        rightSideUp += (float) rawAccel[ZAXIS].value;
        delayMicroseconds(1000);
    }

    rightSideUp /= 5000.0f;

    ftoa(rightSideUp, numberString);
    uart1Print(numberString);
    uart1Print("\n\n");

    uart1Print("Place accelerometer up side down\n");
    uart1Print("  Send a character when ready to proceed\n\n");

    while (uart1Available() == false);
    uart1Read();

    uart1Print("  Gathering Data...\n\n");

    for (index = 0; index < 5000; index++) {
        readAccel();
        upSideDown += (float) rawAccel[ZAXIS].value;
        delayMicroseconds(1000);
    }

    upSideDown /= 5000.0f;

    ftoa(upSideDown, numberString);
    uart1Print(numberString);
    uart1Print("\n\n");

    eepromConfig.accelBias[ZAXIS] = (rightSideUp + upSideDown) / 2.0f;

    eepromConfig.accelScaleFactor[ZAXIS] = (2.0f * 9.8065f) / (fabs(rightSideUp) + fabs(upSideDown));

    ///////////////////////////////////

    uart1Print("Place accelerometer left edge down\n");
    uart1Print("  Send a character when ready to proceed\n\n");

    while (uart1Available() == false);
    uart1Read();

    uart1Print("  Gathering Data...\n\n");

    for (index = 0; index < 5000; index++) {
        readAccel();
        leftWingDown += (float) rawAccel[YAXIS].value;
        delayMicroseconds(1000);
    }

    leftWingDown /= 5000.0f;

    ftoa(leftWingDown, numberString);
    uart1Print(numberString);
    uart1Print("\n\n");

    uart1Print("Place accelerometer right edge down\n");
    uart1Print("  Send a character when ready to proceed\n\n");

    while (uart1Available() == false);
    uart1Read();

    uart1Print("  Gathering Data...\n\n");

    for (index = 0; index < 5000; index++) {
        readAccel();
        rightWingDown += (float) rawAccel[YAXIS].value;
        delayMicroseconds(1000);
    }

    rightWingDown /= 5000.0f;

    ftoa(rightWingDown, numberString);
    uart1Print(numberString);
    uart1Print("\n\n");

    eepromConfig.accelBias[YAXIS] = (leftWingDown + rightWingDown) / 2.0f;

    eepromConfig.accelScaleFactor[YAXIS] = (2.0f * 9.8065f) / (fabs(leftWingDown) + fabs(rightWingDown));

    ///////////////////////////////////

    uart1Print("Place accelerometer rear edge down\n");
    uart1Print("  Send a character when ready to proceed\n\n");

    while (uart1Available() == false);
    uart1Read();

    uart1Print("  Gathering Data...\n\n");

    for (index = 0; index < 5000; index++) {
        readAccel();
        noseUp += (float) rawAccel[XAXIS].value;
        delayMicroseconds(1000);
    }

    noseUp /= 5000.0f;

    ftoa(noseUp, numberString);
    uart1Print(numberString);
    uart1Print("\n\n");

    uart1Print("Place accelerometer front edge down\n");
    uart1Print("  Send a character when ready to proceed\n\n");

    while (uart1Available() == false);
    uart1Read();

    uart1Print("  Gathering Data...\n\n");

    for (index = 0; index < 5000; index++) {
        readAccel();
        noseDown += (float) rawAccel[XAXIS].value;
        delayMicroseconds(1000);
    }

    noseDown /= 5000.0f;

    ftoa(noseDown, numberString);
    uart1Print(numberString);
    uart1Print("\n\n");

    eepromConfig.accelBias[XAXIS] = (noseUp + noseDown) / 2.0f;

    eepromConfig.accelScaleFactor[XAXIS] = (2.0f * 9.8065f) / (fabs(noseUp) + fabs(noseDown));

    ///////////////////////////////////

    ftoa(noseUp, numberString);
    uart1Print(numberString);
    uart1Print(", ");
    ftoa(noseDown, numberString);
    uart1Print(numberString);
    uart1Print(", ");
    ftoa(eepromConfig.accelScaleFactor[XAXIS], numberString);
    uart1Print(numberString);
    uart1Print(", ");
    ftoa(eepromConfig.accelBias[XAXIS], numberString);
    uart1Print(numberString);
    uart1Print(", ");
    ftoa((noseUp - eepromConfig.accelBias[XAXIS]) * eepromConfig.accelScaleFactor[XAXIS], numberString);
    uart1Print(numberString);
    uart1Print(", ");
    ftoa((noseDown - eepromConfig.accelBias[XAXIS]) * eepromConfig.accelScaleFactor[XAXIS], numberString);
    uart1Print(numberString);
    uart1Print("\n\n");

    ftoa(leftWingDown, numberString);
    uart1Print(numberString);
    uart1Print(", ");
    ftoa(rightWingDown, numberString);
    uart1Print(numberString);
    uart1Print(", ");
    ftoa(eepromConfig.accelScaleFactor[YAXIS], numberString);
    uart1Print(numberString);
    uart1Print(", ");
    ftoa(eepromConfig.accelBias[YAXIS], numberString);
    uart1Print(numberString);
    uart1Print(", ");
    ftoa((leftWingDown - eepromConfig.accelBias[YAXIS]) * eepromConfig.accelScaleFactor[YAXIS], numberString);
    uart1Print(numberString);
    uart1Print(", ");
    ftoa((rightWingDown - eepromConfig.accelBias[YAXIS]) * eepromConfig.accelScaleFactor[YAXIS], numberString);
    uart1Print(numberString);
    uart1Print("\n\n");

    ftoa(upSideDown, numberString);
    uart1Print(numberString);
    uart1Print(", ");
    ftoa(rightSideUp, numberString);
    uart1Print(numberString);
    uart1Print(", ");
    ftoa(eepromConfig.accelScaleFactor[ZAXIS], numberString);
    uart1Print(numberString);
    uart1Print(", ");
    ftoa(eepromConfig.accelBias[ZAXIS], numberString);
    uart1Print(numberString);
    uart1Print(", ");
    ftoa((upSideDown - eepromConfig.accelBias[ZAXIS]) * eepromConfig.accelScaleFactor[ZAXIS], numberString);
    uart1Print(numberString);
    uart1Print(", ");
    ftoa((rightSideUp - eepromConfig.accelBias[ZAXIS]) * eepromConfig.accelScaleFactor[ZAXIS], numberString);
    uart1Print(numberString);
    uart1Print("\n");

    uart1Print("\nAccel Calibration Complete.\n");

    accelCalibrating = false;
}

///////////////////////////////////////////////////////////////////////////////
