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

uint8_t cliBusy = false;

static volatile uint8_t queryType;
static volatile uint8_t validCommand = false;

uint8_t highSpeedTelem1Enabled = false;
uint8_t highSpeedTelem2Enabled = false;
uint8_t highSpeedTelem3Enabled = false;
uint8_t highSpeedTelem4Enabled = false;
uint8_t highSpeedTelem5Enabled = false;
uint8_t highSpeedTelem6Enabled = false;
uint8_t highSpeedTelem7Enabled = false;
uint8_t highSpeedTelem8Enabled = false;
uint8_t highSpeedTelem9Enabled = false;

///////////////////////////////////////////////////////////////////////////////
// High Speed Telem Disable
///////////////////////////////////////////////////////////////////////////////

void highSpeedTelemDisable(void)
{
	highSpeedTelem1Enabled = false;
	highSpeedTelem2Enabled = false;
	highSpeedTelem3Enabled = false;
	highSpeedTelem4Enabled = false;
	highSpeedTelem5Enabled = false;
	highSpeedTelem6Enabled = false;
	highSpeedTelem7Enabled = false;
	highSpeedTelem8Enabled = false;
	highSpeedTelem9Enabled = false;
}

///////////////////////////////////////////////////////////////////////////////
// Read Character String from Uart1 Comm
///////////////////////////////////////////////////////////////////////////////

char *readStringUart1(char *data, uint8_t length)
{
    uint8_t index    = 0;
    uint8_t timeout  = 0;

    do
    {
        if (uart1Available() == false)
        {
            delay(10);
            timeout++;
        }
        else
        {
            data[index] = uart1Read();
            timeout = 0;
            index++;
        }
    }
    while ((index == 0 || data[index-1] != ';') && (timeout < 5) && (index < length));

    data[index] = '\0';

    return data;
}

///////////////////////////////////////////////////////////////////////////////
// Read Float from Uart1 Comm
///////////////////////////////////////////////////////////////////////////////

float readFloatUart1(void)
{
    uint8_t index    = 0;
    uint8_t timeout  = 0;
    char    data[13] = "";

    do
    {
        if (uart1Available() == false)
        {
            delay(10);
            timeout++;
        }
        else
        {
            data[index] = uart1Read();
            timeout = 0;
            index++;
        }
    }
    while ((index == 0 || data[index-1] != ';') && (timeout < 5) && (index < sizeof(data)-1));

    data[index] = '\0';

    return stringToFloat(data);
}

///////////////////////////////////////////////////////////////////////////////
// Read PID Values from Uart1 Comm
///////////////////////////////////////////////////////////////////////////////

void readUsbPID(unsigned char PIDid)
{
  struct PIDdata* pid = &eepromConfig.PID[PIDid];

  pid->B             = readFloatUart1();
  pid->P             = readFloatUart1();
  pid->I             = readFloatUart1();
  pid->D             = readFloatUart1();
  pid->windupGuard   = readFloatUart1();
  pid->iTerm          = 0.0f;
  pid->lastDcalcValue = 0.0f;
  pid->lastDterm      = 0.0f;
  pid->lastLastDterm  = 0.0f;
  pid->dErrorCalc     =(uint8_t)readFloatUart1();
}

///////////////////////////////////////////////////////////////////////////////
// CLI Communication
///////////////////////////////////////////////////////////////////////////////

void cliCom(void)
{
	uint8_t  index;

    if ((uart1Available() && !validCommand))
    	queryType = uart1Read();

    switch (queryType)
    {
        ///////////////////////////////

        case 'a': // Rate PIDs
            uart1Print("\n");

            uart1Print("Roll Rate PID:  ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_RATE_PID].B);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_RATE_PID].P);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_RATE_PID].I);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_RATE_PID].D);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_RATE_PID].windupGuard); uart1Print(numberString);
            if  (eepromConfig.PID[ROLL_RATE_PID].dErrorCalc)
                uart1Print("Error\n");
            else
                uart1Print("State\n");

            uart1Print("Pitch Rate PID: ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_RATE_PID].B);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_RATE_PID].P);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_RATE_PID].I);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_RATE_PID].D);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_RATE_PID].windupGuard); uart1Print(numberString);
            if  (eepromConfig.PID[PITCH_RATE_PID].dErrorCalc)
                uart1Print("Error\n");
            else
                uart1Print("State\n");

            uart1Print("Yaw Rate PID:   ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[YAW_RATE_PID].B);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[YAW_RATE_PID].P);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[YAW_RATE_PID].I);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[YAW_RATE_PID].D);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[YAW_RATE_PID].windupGuard); uart1Print(numberString);
            if  (eepromConfig.PID[YAW_RATE_PID].dErrorCalc)
                uart1Print("Error\n");
            else
                uart1Print("State\n");

            queryType = 'x';
            validCommand = false;
            break;

        ///////////////////////////////

        case 'b': // Attitude PIDs
            uart1Print("\n");

            uart1Print("Roll Attitude PID:  ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_ATT_PID].B);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_ATT_PID].P);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_ATT_PID].I);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_ATT_PID].D);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_ATT_PID].windupGuard); uart1Print(numberString);
            if  (eepromConfig.PID[ROLL_ATT_PID].dErrorCalc)
                uart1Print("Error\n");
            else
                uart1Print("State\n");

            uart1Print("Pitch Attitude PID: ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_ATT_PID].B);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_ATT_PID].P);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_ATT_PID].I);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_ATT_PID].D);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_ATT_PID].windupGuard); uart1Print(numberString);
            if  (eepromConfig.PID[PITCH_ATT_PID].dErrorCalc)
                uart1Print("Error\n");
            else
                uart1Print("State\n");

            uart1Print("Heading PID:        ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HEADING_PID].B);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HEADING_PID].P);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HEADING_PID].I);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HEADING_PID].D);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HEADING_PID].windupGuard); uart1Print(numberString);
            if  (eepromConfig.PID[HEADING_PID].dErrorCalc)
                uart1Print("Error\n");
            else
                uart1Print("State\n");

            queryType = 'x';
            validCommand = false;
            break;

        ///////////////////////////////

        case 'c': // Velocity PIDs
            uart1Print("\n");

            uart1Print("nDot PID:  ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[NDOT_PID].B);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[NDOT_PID].P);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[NDOT_PID].I);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[NDOT_PID].D);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[NDOT_PID].windupGuard); uart1Print(numberString);
            if  (eepromConfig.PID[NDOT_PID].dErrorCalc)
                uart1Print("Error\n");
            else
                uart1Print("State\n");

            uart1Print("eDot PID:  ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[EDOT_PID].B);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[EDOT_PID].P);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[EDOT_PID].I);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[EDOT_PID].D);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[EDOT_PID].windupGuard); uart1Print(numberString);
            if  (eepromConfig.PID[EDOT_PID].dErrorCalc)
                uart1Print("Error\n");
            else
                uart1Print("State\n");

            uart1Print("hDot PID:  ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HDOT_PID].B);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HDOT_PID].P);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HDOT_PID].I);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HDOT_PID].D);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HDOT_PID].windupGuard); uart1Print(numberString);
            if  (eepromConfig.PID[HDOT_PID].dErrorCalc)
                uart1Print("Error\n");
            else
                uart1Print("State\n");

            queryType = 'x';
            validCommand = false;
        	break;

        ///////////////////////////////

        case 'd': // Position PIDs
            uart1Print("\n");

            uart1Print("n PID:  ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[N_PID].B);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[N_PID].P);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[N_PID].I);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[N_PID].D);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[N_PID].windupGuard); uart1Print(numberString);
            if  (eepromConfig.PID[N_PID].dErrorCalc)
                uart1Print("Error\n");
            else
                uart1Print("State\n");

            uart1Print("e PID:  ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[E_PID].B);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[E_PID].P);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[E_PID].I);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[E_PID].D);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[E_PID].windupGuard); uart1Print(numberString);
            if  (eepromConfig.PID[E_PID].dErrorCalc)
                uart1Print("Error\n");
            else
                uart1Print("State\n");

            uart1Print("h PID:  ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[H_PID].B);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[H_PID].P);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[H_PID].I);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[H_PID].D);           uart1Print(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[H_PID].windupGuard); uart1Print(numberString);
            if  (eepromConfig.PID[H_PID].dErrorCalc)
                uart1Print("Error\n");
            else
                uart1Print("State\n");

            queryType = 'x';
            validCommand = false;
        	break;

         ///////////////////////////////

        case 'e': // Loop Delta Times
        	snprintf(numberString, 16, "%7ld, ", deltaTime1000Hz); uart1Print(numberString);
        	snprintf(numberString, 16, "%7ld, ", deltaTime500Hz ); uart1Print(numberString);
        	snprintf(numberString, 16, "%7ld, ", deltaTime100Hz ); uart1Print(numberString);
        	snprintf(numberString, 16, "%7ld, ", deltaTime50Hz  ); uart1Print(numberString);
        	snprintf(numberString, 16, "%7ld, ", deltaTime10Hz  ); uart1Print(numberString);
        	snprintf(numberString, 16, "%7ld, ", deltaTime5Hz   ); uart1Print(numberString);
        	snprintf(numberString, 16, "%7ld\n", deltaTime1Hz   ); uart1Print(numberString);

        	validCommand = false;
        	break;

        ///////////////////////////////

        case 'f': // Loop Execution Times
        	snprintf(numberString, 16, "%7ld, ", executionTime1000Hz); uart1Print(numberString);
        	snprintf(numberString, 16, "%7ld, ", executionTime500Hz ); uart1Print(numberString);
        	snprintf(numberString, 16, "%7ld, ", executionTime100Hz ); uart1Print(numberString);
        	snprintf(numberString, 16, "%7ld, ", executionTime50Hz  ); uart1Print(numberString);
        	snprintf(numberString, 16, "%7ld, ", executionTime10Hz  ); uart1Print(numberString);
        	snprintf(numberString, 16, "%7ld, ", executionTime5Hz   ); uart1Print(numberString);
        	snprintf(numberString, 16, "%7ld\n", executionTime1Hz   ); uart1Print(numberString);

        	validCommand = false;
        	break;

        ///////////////////////////////

        case 'g': // 500 Hz Accels
        	ftoa(sensors.accel500Hz[XAXIS], numberString); uart1Print(numberString); uart1Print(", ");
        	ftoa(sensors.accel500Hz[YAXIS], numberString); uart1Print(numberString); uart1Print(", ");
        	ftoa(sensors.accel500Hz[ZAXIS], numberString); uart1Print(numberString); uart1Print("\n");

        	validCommand = false;
        	break;

        ///////////////////////////////

        case 'h': // 100 hz Earth Axis Accels
        	ftoa(earthAxisAccels[XAXIS], numberString); uart1Print(numberString); uart1Print(", ");
        	ftoa(earthAxisAccels[YAXIS], numberString); uart1Print(numberString); uart1Print(", ");
        	ftoa(earthAxisAccels[ZAXIS], numberString); uart1Print(numberString); uart1Print("\n");

        	validCommand = false;
        	break;

        ///////////////////////////////

        case 'i': // 500 Hz Gyros
        	ftoa(sensors.gyro500Hz[ROLL ] * R2D, numberString); uart1Print(numberString); uart1Print(", ");
        	ftoa(sensors.gyro500Hz[PITCH] * R2D, numberString); uart1Print(numberString); uart1Print(", ");
        	ftoa(sensors.gyro500Hz[YAW  ] * R2D, numberString); uart1Print(numberString); uart1Print(", ");
        	// HJI ftoa(mpu6000Temperature,             numberString); uart1Print(numberString); uart1Print("\n");

        	validCommand = false;
        	break;

        ///////////////////////////////

        case 'j': // 10 Hz Mag Data
        	ftoa(sensors.mag10Hz[XAXIS], numberString); uart1Print(numberString); uart1Print(", ");
        	ftoa(sensors.mag10Hz[YAXIS], numberString); uart1Print(numberString); uart1Print(", ");
        	ftoa(sensors.mag10Hz[ZAXIS], numberString); uart1Print(numberString); uart1Print("\n");

        	validCommand = false;
        	break;

        ///////////////////////////////

        case 'k': // Vertical Axis Variables
        	ftoa(earthAxisAccels[ZAXIS],  numberString); uart1Print(numberString); uart1Print(", ");
        	ftoa(sensors.pressureAlt10Hz, numberString); uart1Print(numberString); uart1Print(", ");
        	ftoa(hDotEstimate,            numberString); uart1Print(numberString); uart1Print(", ");
        	ftoa(hEstimate,               numberString); uart1Print(numberString); uart1Print("\n");

        	validCommand = false;
        	break;

        ///////////////////////////////

        case 'l': // Attitudes
        	snprintf(numberString, 16, "%9.4f, ", sensors.attitude500Hz[ROLL ] * R2D); uart1Print(numberString);
        	snprintf(numberString, 16, "%9.4f, ", sensors.attitude500Hz[PITCH] * R2D); uart1Print(numberString);
        	snprintf(numberString, 16, "%9.4f\n", sensors.attitude500Hz[YAW  ] * R2D); uart1Print(numberString);

        	validCommand = false;
        	break;

       ///////////////////////////////

        case 'm': // GPS Data
        	snprintf(numberString, 16, "%12.7f, ", sensors.gpsLatitude  * R2D); uart1Print(numberString);
        	snprintf(numberString, 16, "%12.7f, ", sensors.gpsLongitude * R2D); uart1Print(numberString);

        	ftoa(sensors.gpsAltitude,          numberString); uart1Print(numberString); uart1Print(", ");
        	ftoa(sensors.gpsGroundSpeed,       numberString); uart1Print(numberString); uart1Print(", ");
        	ftoa(sensors.gpsGroundTrack * R2D, numberString); uart1Print(numberString); uart1Print("\n");

            validCommand = false;
            break;

        ///////////////////////////////

        case 'n': // GPS Stats
            if (sensors.gpsFix == FIX_2D)
                uart1Print("2D Fix,  ");
            else if (sensors.gpsFix == FIX_3D)
                uart1Print("3D Fix,  ");
            else if (sensors.gpsFix == FIX_2D_SBAS)
            	uart1Print("2D SBAS, ");
            else if (sensors.gpsFix == FIX_3D_SBAS)
            	uart1Print("3D SBAS, ");
            else
                uart1Print("No Fix, ");

            itoa(sensors.gpsNumSats, numberString, 10); uart1Print(numberString); uart1Print(", ");
        	itoa(sensors.gpsDate,    numberString, 10); uart1Print(numberString); uart1Print(" ");
        	ftoa(sensors.gpsTime,    numberString);     uart1Print(numberString); uart1Print(", ");
            ftoa(sensors.gpsHdop,    numberString);     uart1Print(numberString); uart1Print("\n");
            validCommand = false;
        	break;

        ///////////////////////////////

        case 'o': // Not Used
            queryType = 'x';
            validCommand = false;
            break;

        ///////////////////////////////

        case 'p': // Not Used
            queryType = 'x';
        	validCommand = false;
        	break;

        ///////////////////////////////

        case 'q': // Not Used
            queryType = 'x';
           	validCommand = false;
           	break;

        ///////////////////////////////

        case 'r':
        	if (flightMode == RATE)
        		uart1Print("Flight Mode = RATE      ");
        	else if (flightMode == ATTITUDE)
        		uart1Print("Flight Mode = ATTITUDE  ");
        	else if (flightMode == GPS)
        		uart1Print("Flight Mode = GPS       ");

        	if (headingHoldEngaged == true)
        	    uart1Print("Heading Hold = ENGAGED     ");
        	else
        	    uart1Print("Heading Hold = DISENGAGED  ");

        	if (altitudeHoldState == DISENGAGED)
        		uart1Print("Altitude Hold = DISENAGED\n");
            else if (altitudeHoldState == ENGAGED)
            	uart1Print("Altitude Hold = ENGAGED\n");
            else if (altitudeHoldState == PANIC)
            	uart1Print("Altitude Hold = PANIC\n");

        	validCommand = false;
        	break;

        ///////////////////////////////

        case 's': // Raw Receiver Commands
            if (eepromConfig.receiverType == SPEKTRUM)
            {
				for (index = 0; index < eepromConfig.spektrumChannels - 1; index++)
                {
    		    	itoa(spektrumChannelData[index], numberString, 10);
    		    	uart1Print(numberString); uart1Print(", ");
    		    }

                itoa(spektrumChannelData[eepromConfig.spektrumChannels - 1], numberString, 10);
                uart1Print(numberString); uart1Print("\n");
		    }
		    else
		    {
				for (index = 0; index < 7; index++)
                {
    		    	itoa(rxRead(index), numberString, 10);
    		    	uart1Print(numberString); uart1Print(", ");
    		    }

                itoa(rxRead(7), numberString, 10);
                uart1Print(numberString); uart1Print("\n");
			}

        	validCommand = false;
        	break;

        ///////////////////////////////

        case 't': // Processed Receiver Commands
            for (index = 0; index < 7; index++)
            {
    			ftoa( rxCommand[index], numberString );
    			uart1Print( numberString ); uart1Print( ", " );
    		}

            ftoa( rxCommand[7], numberString );
            uart1Print( numberString ); uart1Print( "\n" );

            validCommand = false;
            break;

        ///////////////////////////////

        case 'u': // Command in Detent Discretes
            if ( commandInDetent[ROLL] == true )
                uart1Print( "true" );
            else
                uart1Print( "false" );
            uart1Print(", ");

            if ( commandInDetent[PITCH] == true )
                uart1Print( "true" );
            else
                uart1Print( "false" );
            uart1Print(", ");

            if ( commandInDetent[YAW] == true )
                uart1Print( "true" );
            else
                uart1Print( "false" );
            uart1Print("\n");

            validCommand = false;
            break;

        ///////////////////////////////

        case 'v': // ESC PWM Outputs
            itoa(TIM17->CCR1, numberString, 10); uart1Print(numberString); uart1Print(", ");
            itoa(TIM4->CCR1,  numberString, 10); uart1Print(numberString); uart1Print(", ");
            itoa(TIM4->CCR2,  numberString, 10); uart1Print(numberString); uart1Print(", ");
            itoa(TIM4->CCR3,  numberString, 10); uart1Print(numberString); uart1Print(", ");
            itoa(TIM4->CCR4,  numberString, 10); uart1Print(numberString); uart1Print(", ");
            itoa(TIM8->CCR1,  numberString, 10); uart1Print(numberString); uart1Print(", ");
            itoa(TIM8->CCR2,  numberString, 10); uart1Print(numberString); uart1Print(", ");
            itoa(TIM8->CCR3,  numberString, 10); uart1Print(numberString); uart1Print("\n");

            validCommand = false;
            break;

        ///////////////////////////////

        case 'w': // Servo PWM Outputs
            itoa(TIM15->CCR1, numberString, 10); uart1Print(numberString); uart1Print(", ");
            itoa(TIM15->CCR2, numberString, 10); uart1Print(numberString); uart1Print(", ");

            validCommand = false;
            break;

        ///////////////////////////////

        case 'x':
        	validCommand = false;
        	break;

        ///////////////////////////////

        case 'y': // ESC Calibration
        	escCalibration();

        	queryType = 'x';
        	break;

        ///////////////////////////////

        case 'z':
            queryType = 'x';
            break;

        ///////////////////////////////

        case '1': // Turn high speed telemetry 1 on
        	highSpeedTelemDisable();
          	highSpeedTelem1Enabled = true;

        	queryType = 'x';
            break;

        ///////////////////////////////

        case '2': // Turn high speed telemetry 2 on
           	highSpeedTelemDisable();
           	highSpeedTelem2Enabled = true;

            queryType = 'x';
           	break;

        ///////////////////////////////

        case '3': // Turn high speed telemetry 3 on
           	highSpeedTelemDisable();
           	highSpeedTelem3Enabled = true;

            queryType = 'x';
           	break;

        ///////////////////////////////

        case '4': // Turn high speed telemetry 4 on
           	highSpeedTelemDisable();
           	highSpeedTelem4Enabled = true;

            queryType = 'x';
           	break;

        ///////////////////////////////

        case '5': // Turn high speed telemetry 5 on
           	highSpeedTelemDisable();
           	highSpeedTelem5Enabled = true;

            queryType = 'x';
           	break;

        ///////////////////////////////

        case '6': // Turn high speed telemetry 6 on
           	highSpeedTelemDisable();
           	highSpeedTelem6Enabled = true;

            queryType = 'x';
           	break;

        ///////////////////////////////

        case '7': // Turn high speed telemetry 7 on
           	highSpeedTelemDisable();
           	highSpeedTelem7Enabled = true;

            queryType = 'x';
           	break;

        ///////////////////////////////

        case '8': // Turn high speed telemetry 8 on
           	highSpeedTelemDisable();
           	highSpeedTelem8Enabled = true;

            queryType = 'x';
           	break;

        ///////////////////////////////

        case '9': // Turn high speed telemetry 9 on
           	highSpeedTelemDisable();
           	highSpeedTelem9Enabled = true;

            queryType = 'x';
           	break;

        ///////////////////////////////

        case '0': // Disable high speed telemetry
           	highSpeedTelemDisable();

            queryType = 'x';
           	break;

        ///////////////////////////////

        ///////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////

        ///////////////////////////////

        case 'A': // Read Roll Rate PID Values
            readUsbPID(ROLL_RATE_PID);
            uart1Print( "\nRoll Rate PID Received....\n" );

        	queryType = 'a';
        	validCommand = false;
        	break;

        ///////////////////////////////

        case 'B': // Read Pitch Rate PID Values
            readUsbPID(PITCH_RATE_PID);
            uart1Print( "\nPitch Rate PID Received....\n" );

        	queryType = 'a';
        	validCommand = false;
        	break;

        ///////////////////////////////

        case 'C': // Read Yaw Rate PID Values
            readUsbPID(YAW_RATE_PID);
            uart1Print( "\nYaw Rate PID Received....\n" );

        	queryType = 'a';
        	validCommand = false;
        	break;

        ///////////////////////////////

        case 'D': // Read Roll Attitude PID Values
            readUsbPID(ROLL_ATT_PID);
            uart1Print( "\nRoll Attitude PID Received....\n" );

        	queryType = 'b';
        	validCommand = false;
        	break;

        ///////////////////////////////

        case 'E': // Read Pitch Attitude PID Values
            readUsbPID(PITCH_ATT_PID);
            uart1Print( "\nPitch Attitude PID Received....\n" );

        	queryType = 'b';
        	validCommand = false;
        	break;

        ///////////////////////////////

        case 'F': // Read Heading Hold PID Values
            readUsbPID(HEADING_PID);
            uart1Print( "\nHeading PID Received....\n" );

        	queryType = 'b';
        	validCommand = false;
        	break;

        ///////////////////////////////

        case 'G': // Read nDot PID Values
            readUsbPID(NDOT_PID);
            uart1Print( "\nnDot PID Received....\n" );

        	queryType = 'c';
        	validCommand = false;
        	break;

        ///////////////////////////////

        case 'H': // Read eDot PID Values
            readUsbPID(EDOT_PID);
            uart1Print( "\neDot PID Received....\n" );

            queryType = 'c';
          	validCommand = false;
          	break;

        ///////////////////////////////

        case 'I': // Read hDot PID Values
            readUsbPID(HDOT_PID);
            uart1Print( "\nhDot PID Received....\n" );

          	queryType = 'c';
          	validCommand = false;
          	break;

       	///////////////////////////////

        case 'J': // Read n PID Values
            readUsbPID(N_PID);
            uart1Print( "\nn PID Received....\n" );

            queryType = 'd';
            validCommand = false;
        	break;

        ///////////////////////////////

        case 'K': // Read e PID Values
            readUsbPID(E_PID);
            uart1Print( "\ne PID Received....\n" );

            queryType = 'd';
            validCommand = false;
        	break;

        ///////////////////////////////

        case 'L': // Read h PID Values
            readUsbPID(H_PID);
            uart1Print( "\nh PID Received....\n" );

            queryType = 'd';
        	validCommand = false;
        	break;

        ///////////////////////////////

        case 'M': // MAX7456 CLI
           	max7456CLI();

           	queryType = 'x';
        	validCommand = false;
        	break;

        ///////////////////////////////

        case 'N': // Mixer CLI
            mixerCLI();

            queryType = 'x';
            validCommand = false;
            break;

        ///////////////////////////////

        case 'O': // Receiver CLI
            receiverCLI();

            queryType = 'x';
            validCommand = false;
            break;

        ///////////////////////////////

        case 'P': // Sensor CLI
           	sensorCLI();

           	queryType = 'x';
           	validCommand = false;
           	break;

        ///////////////////////////////

        case 'Q': // GPS CLI
            gpsCLI();

            queryType = 'x';
           	validCommand = false;
           	break;

        ///////////////////////////////

        case 'R': // Reset to Bootloader
        	uart1Print("Entering Bootloader....\n\n");
        	delay(100);
        	systemReset(true);
        	break;

        ///////////////////////////////

        case 'S': // Reset System
        	uart1Print("\nSystem Reseting....\n\n");
        	delay(100);
        	systemReset(false);
        	break;

        ///////////////////////////////

        case 'T': // Not Used
            queryType = 'x';
           	validCommand = false;
           	break;

        ///////////////////////////////

        case 'U': // Not Used
            queryType = 'x';
         	validCommand = false;
         	break;

        ///////////////////////////////

        case 'V': // Reset EEPROM Parameters
            uart1Print( "\nEEPROM Parameters Reset....\n" );
            checkFirstTime(true);
            uart1Print("\nSystem Resetting....\n\n");
            delay(100);
            systemReset(false);
            break;

        ///////////////////////////////

        case 'W': // Write EEPROM Parameters
            uart1Print("\nWriting EEPROM Parameters....\n");
            writeEEPROM();

            queryType = 'x';
         	validCommand = false;
         	break;

        ///////////////////////////////

        case 'X': // Not Used
            queryType = 'x';
            validCommand = false;
            break;

        ///////////////////////////////

        case 'Y': // Not Used
            queryType = 'x';
            break;

        ///////////////////////////////

        case 'Z': // Not Used
            queryType = 'x';
            break;

        ///////////////////////////////

        case '?': // Command Summary
        	cliBusy = true;

        	uart1Print("\n");
   		    uart1Print("'a' Rate PIDs                              'A' Set Roll Rate PID Data   AB;P;I;D;windupGuard;dErrorCalc\n");
   		    uart1Print("'b' Attitude PIDs                          'B' Set Pitch Rate PID Data  BB;P;I;D;windupGuard;dErrorCalc\n");
   		    uart1Print("'c' Velocity PIDs                          'C' Set Yaw Rate PID Data    CB;P;I;D;windupGuard;dErrorCalc\n");
   		    uart1Print("'d' Position PIDs                          'D' Set Roll Att PID Data    DB;P;I;D;windupGuard;dErrorCalc\n");
   		    uart1Print("'e' Loop Delta Times                       'E' Set Pitch Att PID Data   EB;P;I;D;windupGuard;dErrorCalc\n");
   		    uart1Print("'f' Loop Execution Times                   'F' Set Hdg Hold PID Data    FB;P;I;D;windupGuard;dErrorCalc\n");
   		    uart1Print("'g' 500 Hz Accels                          'G' Set nDot PID Data        GB;P;I;D;windupGuard;dErrorCalc\n");
   		    uart1Print("'h' 100 Hz Earth Axis Accels               'H' Set eDot PID Data        HB;P;I;D;windupGuard;dErrorCalc\n");
   		    uart1Print("'i' 500 Hz Gyros                           'I' Set hDot PID Data        IB;P;I;D;windupGuard;dErrorCalc\n");
   		    uart1Print("'j' 10 hz Mag Data                         'J' Set n PID Data           JB;P;I;D;windupGuard;dErrorCalc\n");
   		    uart1Print("'k' Vertical Axis Variable                 'K' Set e PID Data           KB;P;I;D;windupGuard;dErrorCalc\n");
   		    uart1Print("'l' Attitudes                              'L' Set h PID Data           LB;P;I;D;windupGuard;dErrorCalc\n");
   		    uart1Print("\n");

   		    uart1Print("Press space bar for more, or enter a command....\n");
   		    while (uart1Available() == false);
   		    queryType = uart1Read();
   		    if (queryType != ' ')
   		    {
   		        validCommand = true;
   		        cliBusy = false;
   		    	return;
   		    }

   		    uart1Print("\n");
   		    uart1Print("'m' GPS Data                               'M' MAX7456 CLI\n");
   		    uart1Print("'n' GPS Stats                              'N' Mixer CLI\n");
   		    uart1Print("'o' Not Used                               'O' Receiver CLI\n");
   		    uart1Print("'p' Not Used                               'P' Sensor CLI\n");
   		    uart1Print("'q' Not Used                               'Q' GPS CLI\n");
   		    uart1Print("'r' Mode States                            'R' Reset and Enter Bootloader\n");
   		    uart1Print("'s' Raw Receiver Commands                  'S' Reset\n");
   		    uart1Print("'t' Processed Receiver Commands            'T' Not Used\n");
   		    uart1Print("'u' Command In Detent Discretes            'U' Not Used\n");
   		    uart1Print("'v' Motor PWM Outputs                      'V' Reset EEPROM Parameters\n");
   		    uart1Print("'w' Servo PWM Outputs                      'W' Write EEPROM Parameters\n");
   		    uart1Print("'x' Terminate Serial Communication         'X' Not Used\n");
   		    uart1Print("\n");

   		    uart1Print("Press space bar for more, or enter a command....\n");
   		    while (uart1Available() == false);
   		    queryType = uart1Read();
   		    if (queryType != ' ')
   		    {
   		    	validCommand = true;
   		    	cliBusy = false;
   		    	return;
   		    }

   		    uart1Print("\n");
   		    uart1Print("'y' ESC Calibration                        'Y' Not Used\n");
   		    uart1Print("'z' Not Used                               'Z' Not Used\n");
   		    uart1Print("'1' High Speed Telemetry 1 Enable\n");
   		    uart1Print("'2' High Speed Telemetry 2 Enable\n");
   		    uart1Print("'3' High Speed Telemetry 3 Enable\n");
   		    uart1Print("'4' High Speed Telemetry 4 Enable\n");
   		    uart1Print("'5' High Speed Telemetry 5 Enable\n");
   		    uart1Print("'6' High Speed Telemetry 6 Enable\n");
   		    uart1Print("'7' High Speed Telemetry 7 Enable\n");
   		    uart1Print("'8' High Speed Telemetry 8 Enable\n");
   		    uart1Print("'9' High Speed Telemetry 9 Enable\n");
   		    uart1Print("'0' High Speed Telemetry Disable           '?' Command Summary\n");
   		    uart1Print("\n");

  		    queryType = 'x';
  		    cliBusy = false;
   		    break;

            ///////////////////////////////
    }
}

///////////////////////////////////////////////////////////////////////////////
