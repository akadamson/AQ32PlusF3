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
// LSM303DLHC Defines and Variables
///////////////////////////////////////////////////////////////////////////////

// Address

#define LSM303DLHC_ACCEL_ADDRESS 0x32
#define LSM303DLHC_MAG_ADDRESS   0x3C

// Registers

#define CTRL_REG1_A      0x20
#define CTRL_REG4_A      0x23
#define CTRL_REG5_A      0x24
#define OUT_X_L_A        0x28
#define CRA_REG_M        0x00
#define CRB_REG_M        0x01
#define MR_REG_M         0x02
#define OUT_X_H_M        0x03

///////////////////////////////////////

#define ODR_1344_HZ             0x90
#define AXES_ENABLE             0x07

#define FULLSCALE_2G            0x00
#define FULLSCALE_4G            0x10
#define FULLSCALE_8G            0x20
#define FULLSCALE_16G           0x30

#define BOOT                    0x80

///////////////////////////////////////

#define ODR_75_HZ               0x18
#define ODR_15_HZ               0x10

#define FS_1P3_GA               0x20
#define FS_1P9_GA               0x40
#define FS_2P5_GA               0x60
#define FS_4P0_GA               0x80
#define FS_4P7_GA               0xA0
#define FS_5P6_GA               0xC0
#define FS_8P1_GA               0xE0

#define CONTINUOUS_CONVERSION   0x00

///////////////////////////////////////

uint8_t accelCalibrating = false;

float   accelOneG = 9.8065;

int32_t accelSum100Hz[3] = { 0, 0, 0 };

int32_t accelSum500Hz[3] = { 0, 0, 0 };

int32_t accelSummedSamples100Hz[3];

int32_t accelSummedSamples500Hz[3];

int16andUint8_t rawAccel[3];

///////////////////////////////////////

float magScaleFactor[3];

uint8_t magDataUpdate = false;

uint8_t newMagData = false;

int16andUint8_t rawMag[3];

///////////////////////////////////////////////////////////////////////////////
// Read Accel
///////////////////////////////////////////////////////////////////////////////

void readAccel(void)
{
    uint8_t buffer[6];

    i2cRead(I2C1, LSM303DLHC_ACCEL_ADDRESS, OUT_X_L_A, 6, buffer);

    rawAccel[XAXIS].bytes[0] = buffer[0];
    rawAccel[XAXIS].bytes[1] = buffer[1];
    rawAccel[YAXIS].bytes[0] = buffer[2];
    rawAccel[YAXIS].bytes[1] = buffer[3];
    rawAccel[ZAXIS].bytes[0] = buffer[4];
    rawAccel[ZAXIS].bytes[1] = buffer[5];
}

///////////////////////////////////////////////////////////////////////////////
// Accel Initialization
///////////////////////////////////////////////////////////////////////////////

void initAccel(void)
{
	i2cWrite(I2C1, LSM303DLHC_ACCEL_ADDRESS, CTRL_REG5_A, BOOT);

	delay(100);

	i2cWrite(I2C1, LSM303DLHC_ACCEL_ADDRESS, CTRL_REG1_A, ODR_1344_HZ | AXES_ENABLE);

    delay(10);

    i2cWrite(I2C1, LSM303DLHC_ACCEL_ADDRESS, CTRL_REG4_A, FULLSCALE_4G);

    delay(100);

    computeAccelRTData();
}

///////////////////////////////////////////////////////////////////////////////
// Compute Accel Runtime Data
///////////////////////////////////////////////////////////////////////////////

void computeAccelRTData(void)
{
    uint8_t  axis;
    uint16_t samples;

    float accelSum[3]  = { 0.0f, 0.0f, 0.0f };

    accelCalibrating = true;

    for (samples = 0; samples < 2000; samples++)
    {
        readAccel();

        accelSum[XAXIS]  += ((float)rawAccel[XAXIS].value - eepromConfig.accelBias[XAXIS]) * eepromConfig.accelScaleFactor[XAXIS];
        accelSum[YAXIS]  += ((float)rawAccel[YAXIS].value - eepromConfig.accelBias[YAXIS]) * eepromConfig.accelScaleFactor[YAXIS];
        accelSum[ZAXIS]  += ((float)rawAccel[ZAXIS].value - eepromConfig.accelBias[ZAXIS]) * eepromConfig.accelScaleFactor[ZAXIS];

        delayMicroseconds(1000);
    }

    for (axis = 0; axis < 3; axis++)
    {
        accelSum[axis] = accelSum[axis]  / 2000.0f;
    }

    accelOneG = sqrt(accelSum[XAXIS] * accelSum[XAXIS] +
    		         accelSum[YAXIS] * accelSum[YAXIS] +
    		         accelSum[ZAXIS] * accelSum[ZAXIS]);

    accelCalibrating = false;
}

///////////////////////////////////////////////////////////////////////////////
// Read Mag
///////////////////////////////////////////////////////////////////////////////

uint8_t readMag(void)
{
    uint8_t I2C_Buffer_Rx[6];

    i2cRead(I2C1, LSM303DLHC_MAG_ADDRESS, OUT_X_H_M, 6, I2C_Buffer_Rx);

    rawMag[XAXIS].bytes[1] = I2C_Buffer_Rx[0];
    rawMag[XAXIS].bytes[0] = I2C_Buffer_Rx[1];
    rawMag[ZAXIS].bytes[1] = I2C_Buffer_Rx[2];
    rawMag[ZAXIS].bytes[0] = I2C_Buffer_Rx[3];
    rawMag[YAXIS].bytes[1] = I2C_Buffer_Rx[4];
    rawMag[YAXIS].bytes[0] = I2C_Buffer_Rx[5];

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Mag Initialization
///////////////////////////////////////////////////////////////////////////////

void initMag(void)
{
    magScaleFactor[XAXIS] = 1.0f;
    magScaleFactor[YAXIS] = 1.0f;
    magScaleFactor[ZAXIS] = 1.0f;

    i2cWrite(I2C1, LSM303DLHC_MAG_ADDRESS, CRA_REG_M, ODR_15_HZ);
    delay(20);

    i2cWrite(I2C1, LSM303DLHC_MAG_ADDRESS, CRB_REG_M, FS_2P5_GA);
    delay(20);

    i2cWrite(I2C1, LSM303DLHC_MAG_ADDRESS, MR_REG_M, CONTINUOUS_CONVERSION);
    delay(20);

    readMag();

    delay(20);
}

///////////////////////////////////////////////////////////////////////////////
