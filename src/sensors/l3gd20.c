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
// L3GD20 Defines and Variables
///////////////////////////////////////////////////////////////////////////////

/* Read/Write command */
#define READ_CMD               ((uint8_t)0x80)
#define MULTIPLEBYTE_CMD       ((uint8_t)0x40)
#define DUMMY_BYTE             ((uint8_t)0x00)

#define CTRL_REG1_ADDR         0x20
#define CTRL_REG4_ADDR         0x23
#define CTRL_REG5_ADDR         0x24
#define OUT_TEMP_ADDR          0x26
#define OUT_X_L_ADDR           0x28

///////////////////////////////////////

#define MODE_ACTIVE                   ((uint8_t)0x08)

#define OUTPUT_DATARATE_1             ((uint8_t)0x00)
#define OUTPUT_DATARATE_2             ((uint8_t)0x40)
#define OUTPUT_DATARATE_3             ((uint8_t)0x80)
#define OUTPUT_DATARATE_4             ((uint8_t)0xC0)

#define AXES_ENABLE                   ((uint8_t)0x07)

#define BANDWIDTH_1                   ((uint8_t)0x00)
#define BANDWIDTH_2                   ((uint8_t)0x10)
#define BANDWIDTH_3                   ((uint8_t)0x20)
#define BANDWIDTH_4                   ((uint8_t)0x30)

#define FULLSCALE_250                 ((uint8_t)0x00)
#define FULLSCALE_500                 ((uint8_t)0x10)
#define FULLSCALE_2000                ((uint8_t)0x20)

#define BLOCK_DATA_UPDATE_CONTINUOUS  ((uint8_t)0x00)

#define BLE_MSB	                      ((uint8_t)0x40)

#define BOOT                          ((uint8_t)0x80)

///////////////////////////////////////

float   gyroRTBias[3];

int32_t gyroSum500Hz[3] = { 0, 0, 0 };

int32_t gyroSummedSamples500Hz[3];

int16andUint8_t rawGyro[3];

///////////////////////////////////////

uint8_t gyroCalibrating = false;

///////////////////////////////////////////////////////////////////////////////
// Gyro Initialization
///////////////////////////////////////////////////////////////////////////////

void initGyro(void)
{
    ///////////////////////////////////

	GPIO_ResetBits(L3GD20_CS_GPIO, L3GD20_CS_PIN);

	spiTransfer(L3GD20_SPI, CTRL_REG5_ADDR);
	spiTransfer(L3GD20_SPI, BOOT);

	GPIO_SetBits(L3GD20_CS_GPIO, L3GD20_CS_PIN);

	delayMicroseconds(100);

	GPIO_ResetBits(L3GD20_CS_GPIO, L3GD20_CS_PIN);

    spiTransfer(L3GD20_SPI, CTRL_REG1_ADDR);
    spiTransfer(L3GD20_SPI, MODE_ACTIVE | OUTPUT_DATARATE_4 | AXES_ENABLE | BANDWIDTH_4);

    GPIO_SetBits(L3GD20_CS_GPIO, L3GD20_CS_PIN);

    delayMicroseconds(1);

    GPIO_ResetBits(L3GD20_CS_GPIO, L3GD20_CS_PIN);

    spiTransfer(L3GD20_SPI, CTRL_REG4_ADDR);
    spiTransfer(L3GD20_SPI, BLOCK_DATA_UPDATE_CONTINUOUS | BLE_MSB | FULLSCALE_500);

    GPIO_SetBits(L3GD20_CS_GPIO, L3GD20_CS_PIN);

    ///////////////////////////////////

    delay(100);

    computeGyroRTData();
}

///////////////////////////////////////////////////////////////////////////////
// Read Gyro
///////////////////////////////////////////////////////////////////////////////

void readGyro(void)
{
	GPIO_ResetBits(L3GD20_CS_GPIO, L3GD20_CS_PIN);

	                          //spiTransfer(L3GD20_SPI, OUT_TEMP_ADDR | READ_CMD);

	//rawGyroTemperature      = spiTransfer(L3GD20_SPI, DUMMY_BYTE);

	                          spiTransfer(L3GD20_SPI, OUT_X_L_ADDR | READ_CMD |MULTIPLEBYTE_CMD);

    rawGyro[PITCH].bytes[1] = spiTransfer(L3GD20_SPI, DUMMY_BYTE);
    rawGyro[PITCH].bytes[0] = spiTransfer(L3GD20_SPI, DUMMY_BYTE);
    rawGyro[ROLL ].bytes[1] = spiTransfer(L3GD20_SPI, DUMMY_BYTE);
    rawGyro[ROLL ].bytes[0] = spiTransfer(L3GD20_SPI, DUMMY_BYTE);
    rawGyro[YAW  ].bytes[1] = spiTransfer(L3GD20_SPI, DUMMY_BYTE);
    rawGyro[YAW  ].bytes[0] = spiTransfer(L3GD20_SPI, DUMMY_BYTE);

    GPIO_SetBits(L3GD20_CS_GPIO, L3GD20_CS_PIN);
}

///////////////////////////////////////////////////////////////////////////////
// Compute Gyro Runtime Data
///////////////////////////////////////////////////////////////////////////////

void computeGyroRTData(void)
{
    uint8_t  axis;
    uint16_t samples;

    float gyroSum[3]  = { 0.0f, 0.0f, 0.0f };

    gyroCalibrating = true;

    for (samples = 0; samples < 2000; samples++)
    {
        readGyro();

        gyroSum[ROLL ]  += (float)rawGyro[ROLL ].value;
        gyroSum[PITCH]  += (float)rawGyro[PITCH].value;
        gyroSum[YAW  ]  += (float)rawGyro[YAW  ].value;

        delayMicroseconds(1000);
    }

    for (axis = 0; axis < 3; axis++)
    {
        gyroRTBias[axis] = gyroSum[axis]  / 2000.0f;
    }

    gyroCalibrating = false;
}

///////////////////////////////////////////////////////////////////////////////

