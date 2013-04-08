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

static volatile uint8_t cliQuery;
static volatile uint8_t validCliCommand = false;

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
// Read Character String from CLI
///////////////////////////////////////////////////////////////////////////////

char *readStringCLI(char *data, uint8_t length)
{
    uint8_t index    = 0;
    uint8_t timeout  = 0;

    do
    {
        if (cliAvailable() == false)
        {
            delay(10);
            timeout++;
        }
        else
        {
            data[index] = cliRead();
            timeout = 0;
            index++;
        }
    }
    while ((index == 0 || data[index-1] != ';') && (timeout < 5) && (index < length));

    data[index] = '\0';

    return data;
}

///////////////////////////////////////////////////////////////////////////////
// Read Float from CLI
///////////////////////////////////////////////////////////////////////////////

float readFloatCLI(void)
{
    uint8_t index    = 0;
    uint8_t timeout  = 0;
    char    data[13] = "";

    do
    {
        if (cliAvailable() == false)
        {
            delay(10);
            timeout++;
        }
        else
        {
            data[index] = cliRead();
            timeout = 0;
            index++;
        }
    }
    while ((index == 0 || data[index-1] != ';') && (timeout < 5) && (index < sizeof(data)-1));

    data[index] = '\0';

    return stringToFloat(data);
}

///////////////////////////////////////////////////////////////////////////////
// Read PID Values from CLI
///////////////////////////////////////////////////////////////////////////////

void readCliPID(unsigned char PIDid)
{
  struct PIDdata* pid = &eepromConfig.PID[PIDid];

  pid->B             = readFloatCLI();
  pid->P             = readFloatCLI();
  pid->I             = readFloatCLI();
  pid->D             = readFloatCLI();
  pid->windupGuard   = readFloatCLI();
  pid->iTerm          = 0.0f;
  pid->lastDcalcValue = 0.0f;
  pid->lastDterm      = 0.0f;
  pid->lastLastDterm  = 0.0f;
  pid->dErrorCalc     =(uint8_t)readFloatCLI();
}

///////////////////////////////////////////////////////////////////////////////
// CLI Communication
///////////////////////////////////////////////////////////////////////////////

void cliCom(void)
{
	uint8_t  index;

    if ((cliAvailable() && !validCliCommand))
    	cliQuery = cliRead();

    switch (cliQuery)
    {
        ///////////////////////////////

        case 'a': // Rate PIDs
            cliPrint("\n");

            cliPrint("Roll Rate PID:  ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_RATE_PID].B);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_RATE_PID].P);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_RATE_PID].I);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_RATE_PID].D);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_RATE_PID].windupGuard); cliPrint(numberString);
            if  (eepromConfig.PID[ROLL_RATE_PID].dErrorCalc)
                cliPrint("Error\n");
            else
                cliPrint("State\n");

            cliPrint("Pitch Rate PID: ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_RATE_PID].B);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_RATE_PID].P);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_RATE_PID].I);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_RATE_PID].D);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_RATE_PID].windupGuard); cliPrint(numberString);
            if  (eepromConfig.PID[PITCH_RATE_PID].dErrorCalc)
                cliPrint("Error\n");
            else
                cliPrint("State\n");

            cliPrint("Yaw Rate PID:   ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[YAW_RATE_PID].B);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[YAW_RATE_PID].P);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[YAW_RATE_PID].I);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[YAW_RATE_PID].D);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[YAW_RATE_PID].windupGuard); cliPrint(numberString);
            if  (eepromConfig.PID[YAW_RATE_PID].dErrorCalc)
                cliPrint("Error\n");
            else
                cliPrint("State\n");

            cliQuery = 'x';
            validCliCommand = false;
            break;

        ///////////////////////////////

        case 'b': // Attitude PIDs
            cliPrint("\n");

            cliPrint("Roll Attitude PID:  ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_ATT_PID].B);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_ATT_PID].P);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_ATT_PID].I);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_ATT_PID].D);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[ROLL_ATT_PID].windupGuard); cliPrint(numberString);
            if  (eepromConfig.PID[ROLL_ATT_PID].dErrorCalc)
                cliPrint("Error\n");
            else
                cliPrint("State\n");

            cliPrint("Pitch Attitude PID: ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_ATT_PID].B);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_ATT_PID].P);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_ATT_PID].I);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_ATT_PID].D);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[PITCH_ATT_PID].windupGuard); cliPrint(numberString);
            if  (eepromConfig.PID[PITCH_ATT_PID].dErrorCalc)
                cliPrint("Error\n");
            else
                cliPrint("State\n");

            cliPrint("Heading PID:        ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HEADING_PID].B);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HEADING_PID].P);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HEADING_PID].I);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HEADING_PID].D);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HEADING_PID].windupGuard); cliPrint(numberString);
            if  (eepromConfig.PID[HEADING_PID].dErrorCalc)
                cliPrint("Error\n");
            else
                cliPrint("State\n");

            cliQuery = 'x';
            validCliCommand = false;
            break;

        ///////////////////////////////

        case 'c': // Velocity PIDs
            cliPrint("\n");

            cliPrint("nDot PID:  ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[NDOT_PID].B);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[NDOT_PID].P);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[NDOT_PID].I);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[NDOT_PID].D);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[NDOT_PID].windupGuard); cliPrint(numberString);
            if  (eepromConfig.PID[NDOT_PID].dErrorCalc)
                cliPrint("Error\n");
            else
                cliPrint("State\n");

            cliPrint("eDot PID:  ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[EDOT_PID].B);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[EDOT_PID].P);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[EDOT_PID].I);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[EDOT_PID].D);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[EDOT_PID].windupGuard); cliPrint(numberString);
            if  (eepromConfig.PID[EDOT_PID].dErrorCalc)
                cliPrint("Error\n");
            else
                cliPrint("State\n");

            cliPrint("hDot PID:  ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HDOT_PID].B);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HDOT_PID].P);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HDOT_PID].I);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HDOT_PID].D);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[HDOT_PID].windupGuard); cliPrint(numberString);
            if  (eepromConfig.PID[HDOT_PID].dErrorCalc)
                cliPrint("Error\n");
            else
                cliPrint("State\n");

            cliQuery = 'x';
            validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'd': // Position PIDs
            cliPrint("\n");

            cliPrint("n PID:  ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[N_PID].B);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[N_PID].P);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[N_PID].I);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[N_PID].D);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[N_PID].windupGuard); cliPrint(numberString);
            if  (eepromConfig.PID[N_PID].dErrorCalc)
                cliPrint("Error\n");
            else
                cliPrint("State\n");

            cliPrint("e PID:  ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[E_PID].B);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[E_PID].P);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[E_PID].I);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[E_PID].D);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[E_PID].windupGuard); cliPrint(numberString);
            if  (eepromConfig.PID[E_PID].dErrorCalc)
                cliPrint("Error\n");
            else
                cliPrint("State\n");

            cliPrint("h PID:  ");
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[H_PID].B);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[H_PID].P);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[H_PID].I);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[H_PID].D);           cliPrint(numberString);
            snprintf(numberString, 16, "%8.4f, ", eepromConfig.PID[H_PID].windupGuard); cliPrint(numberString);
            if  (eepromConfig.PID[H_PID].dErrorCalc)
                cliPrint("Error\n");
            else
                cliPrint("State\n");

            cliQuery = 'x';
            validCliCommand = false;
        	break;

         ///////////////////////////////

        case 'e': // Loop Delta Times
        	snprintf(numberString, 16, "%7ld, ", deltaTime1000Hz); cliPrint(numberString);
        	snprintf(numberString, 16, "%7ld, ", deltaTime500Hz ); cliPrint(numberString);
        	snprintf(numberString, 16, "%7ld, ", deltaTime100Hz ); cliPrint(numberString);
        	snprintf(numberString, 16, "%7ld, ", deltaTime50Hz  ); cliPrint(numberString);
        	snprintf(numberString, 16, "%7ld, ", deltaTime10Hz  ); cliPrint(numberString);
        	snprintf(numberString, 16, "%7ld, ", deltaTime5Hz   ); cliPrint(numberString);
        	snprintf(numberString, 16, "%7ld\n", deltaTime1Hz   ); cliPrint(numberString);

        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'f': // Loop Execution Times
        	snprintf(numberString, 16, "%7ld, ", executionTime1000Hz); cliPrint(numberString);
        	snprintf(numberString, 16, "%7ld, ", executionTime500Hz ); cliPrint(numberString);
        	snprintf(numberString, 16, "%7ld, ", executionTime100Hz ); cliPrint(numberString);
        	snprintf(numberString, 16, "%7ld, ", executionTime50Hz  ); cliPrint(numberString);
        	snprintf(numberString, 16, "%7ld, ", executionTime10Hz  ); cliPrint(numberString);
        	snprintf(numberString, 16, "%7ld, ", executionTime5Hz   ); cliPrint(numberString);
        	snprintf(numberString, 16, "%7ld\n", executionTime1Hz   ); cliPrint(numberString);

        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'g': // 500 Hz Accels
        	ftoa(sensors.accel500Hz[XAXIS], numberString); cliPrint(numberString); cliPrint(", ");
        	ftoa(sensors.accel500Hz[YAXIS], numberString); cliPrint(numberString); cliPrint(", ");
        	ftoa(sensors.accel500Hz[ZAXIS], numberString); cliPrint(numberString); cliPrint("\n");

        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'h': // 100 hz Earth Axis Accels
        	ftoa(earthAxisAccels[XAXIS], numberString); cliPrint(numberString); cliPrint(", ");
        	ftoa(earthAxisAccels[YAXIS], numberString); cliPrint(numberString); cliPrint(", ");
        	ftoa(earthAxisAccels[ZAXIS], numberString); cliPrint(numberString); cliPrint("\n");

        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'i': // 500 Hz Gyros
        	ftoa(sensors.gyro500Hz[ROLL ] * R2D, numberString); cliPrint(numberString); cliPrint(", ");
        	ftoa(sensors.gyro500Hz[PITCH] * R2D, numberString); cliPrint(numberString); cliPrint(", ");
        	ftoa(sensors.gyro500Hz[YAW  ] * R2D, numberString); cliPrint(numberString); cliPrint(", ");
        	// HJI ftoa(mpu6000Temperature,             numberString); cliPrint(numberString); cliPrint("\n");

        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'j': // 10 Hz Mag Data
        	ftoa(sensors.mag10Hz[XAXIS], numberString); cliPrint(numberString); cliPrint(", ");
        	ftoa(sensors.mag10Hz[YAXIS], numberString); cliPrint(numberString); cliPrint(", ");
        	ftoa(sensors.mag10Hz[ZAXIS], numberString); cliPrint(numberString); cliPrint("\n");

        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'k': // Vertical Axis Variables
        	ftoa(earthAxisAccels[ZAXIS],  numberString); cliPrint(numberString); cliPrint(", ");
        	ftoa(sensors.pressureAlt10Hz, numberString); cliPrint(numberString); cliPrint(", ");
        	ftoa(hDotEstimate,            numberString); cliPrint(numberString); cliPrint(", ");
        	ftoa(hEstimate,               numberString); cliPrint(numberString); cliPrint("\n");

        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'l': // Attitudes
        	snprintf(numberString, 16, "%9.4f, ", sensors.attitude500Hz[ROLL ] * R2D); cliPrint(numberString);
        	snprintf(numberString, 16, "%9.4f, ", sensors.attitude500Hz[PITCH] * R2D); cliPrint(numberString);
        	snprintf(numberString, 16, "%9.4f, ", heading.mag * R2D);                  cliPrint(numberString);
        	snprintf(numberString, 16, "%9.4f\n", heading.tru * R2D);                  cliPrint(numberString);

        	validCliCommand = false;
        	break;

       ///////////////////////////////

        case 'm': // GPS Data
        	snprintf(numberString, 16, "%12.7f, ", sensors.gpsLatitude  * R2D); cliPrint(numberString);
        	snprintf(numberString, 16, "%12.7f, ", sensors.gpsLongitude * R2D); cliPrint(numberString);

        	ftoa(sensors.gpsAltitude,          numberString); cliPrint(numberString); cliPrint(", ");
        	ftoa(sensors.gpsGroundSpeed,       numberString); cliPrint(numberString); cliPrint(", ");
        	ftoa(sensors.gpsGroundTrack * R2D, numberString); cliPrint(numberString); cliPrint("\n");

            validCliCommand = false;
            break;

        ///////////////////////////////

        case 'n': // GPS Stats
            if (sensors.gpsFix == FIX_2D)
                cliPrint("2D Fix,  ");
            else if (sensors.gpsFix == FIX_3D)
                cliPrint("3D Fix,  ");
            else if (sensors.gpsFix == FIX_2D_SBAS)
            	cliPrint("2D SBAS, ");
            else if (sensors.gpsFix == FIX_3D_SBAS)
            	cliPrint("3D SBAS, ");
            else
                cliPrint("No Fix, ");

            itoa(sensors.gpsNumSats, numberString, 10); cliPrint(numberString); cliPrint(", ");
        	itoa(sensors.gpsDate,    numberString, 10); cliPrint(numberString); cliPrint(" ");
        	ftoa(sensors.gpsTime,    numberString);     cliPrint(numberString); cliPrint(", ");
            ftoa(sensors.gpsHdop,    numberString);     cliPrint(numberString); cliPrint("\n");
            validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'o': // Not Used
            cliQuery = 'x';
            validCliCommand = false;
            break;

        ///////////////////////////////

        case 'p': // Not Used
            cliQuery = 'x';
        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'q': // Not Used
            cliQuery = 'x';
           	validCliCommand = false;
           	break;

        ///////////////////////////////

        case 'r':
        	if (flightMode == RATE)
        		cliPrint("Flight Mode = RATE      ");
        	else if (flightMode == ATTITUDE)
        		cliPrint("Flight Mode = ATTITUDE  ");
        	else if (flightMode == GPS)
        		cliPrint("Flight Mode = GPS       ");

        	if (headingHoldEngaged == true)
        	    cliPrint("Heading Hold = ENGAGED     ");
        	else
        	    cliPrint("Heading Hold = DISENGAGED  ");

        	if (altitudeHoldState == DISENGAGED)
        		cliPrint("Altitude Hold = DISENAGED\n");
            else if (altitudeHoldState == ENGAGED)
            	cliPrint("Altitude Hold = ENGAGED\n");
            else if (altitudeHoldState == PANIC)
            	cliPrint("Altitude Hold = PANIC\n");

        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 's': // Raw Receiver Commands
            if (eepromConfig.receiverType == SPEKTRUM)
            {
				for (index = 0; index < eepromConfig.spektrumChannels - 1; index++)
                {
    		    	itoa(spektrumChannelData[index], numberString, 10);
    		    	cliPrint(numberString); cliPrint(", ");
    		    }

                itoa(spektrumChannelData[eepromConfig.spektrumChannels - 1], numberString, 10);
                cliPrint(numberString); cliPrint("\n");
		    }
		    else
		    {
				for (index = 0; index < 7; index++)
                {
    		    	itoa(rxRead(index), numberString, 10);
    		    	cliPrint(numberString); cliPrint(", ");
    		    }

                itoa(rxRead(7), numberString, 10);
                cliPrint(numberString); cliPrint("\n");
			}

        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 't': // Processed Receiver Commands
            for (index = 0; index < 7; index++)
            {
    			ftoa( rxCommand[index], numberString );
    			cliPrint( numberString ); cliPrint( ", " );
    		}

            ftoa( rxCommand[7], numberString );
            cliPrint( numberString ); cliPrint( "\n" );

            validCliCommand = false;
            break;

        ///////////////////////////////

        case 'u': // Command in Detent Discretes
            if ( commandInDetent[ROLL] == true )
                cliPrint( "true" );
            else
                cliPrint( "false" );
            cliPrint(", ");

            if ( commandInDetent[PITCH] == true )
                cliPrint( "true" );
            else
                cliPrint( "false" );
            cliPrint(", ");

            if ( commandInDetent[YAW] == true )
                cliPrint( "true" );
            else
                cliPrint( "false" );
            cliPrint("\n");

            validCliCommand = false;
            break;

        ///////////////////////////////

        case 'v': // ESC PWM Outputs
            itoa(TIM17->CCR1, numberString, 10); cliPrint(numberString); cliPrint(", ");
            itoa(TIM4->CCR1,  numberString, 10); cliPrint(numberString); cliPrint(", ");
            itoa(TIM4->CCR2,  numberString, 10); cliPrint(numberString); cliPrint(", ");
            itoa(TIM4->CCR3,  numberString, 10); cliPrint(numberString); cliPrint(", ");
            itoa(TIM4->CCR4,  numberString, 10); cliPrint(numberString); cliPrint(", ");
            itoa(TIM8->CCR1,  numberString, 10); cliPrint(numberString); cliPrint(", ");
            itoa(TIM8->CCR2,  numberString, 10); cliPrint(numberString); cliPrint(", ");
            itoa(TIM8->CCR3,  numberString, 10); cliPrint(numberString); cliPrint("\n");

            validCliCommand = false;
            break;

        ///////////////////////////////

        case 'w': // Servo PWM Outputs
            itoa(TIM15->CCR1, numberString, 10); cliPrint(numberString); cliPrint(", ");
            itoa(TIM15->CCR2, numberString, 10); cliPrint(numberString); cliPrint(", ");

            validCliCommand = false;
            break;

        ///////////////////////////////

        case 'x':
        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'y': // ESC Calibration
        	escCalibration();

        	cliQuery = 'x';
        	break;

        ///////////////////////////////

        case 'z':
            cliQuery = 'x';
            break;

        ///////////////////////////////

        case '1': // Turn high speed telemetry 1 on
        	highSpeedTelemDisable();
          	highSpeedTelem1Enabled = true;

        	cliQuery = 'x';
            break;

        ///////////////////////////////

        case '2': // Turn high speed telemetry 2 on
           	highSpeedTelemDisable();
           	highSpeedTelem2Enabled = true;

            cliQuery = 'x';
           	break;

        ///////////////////////////////

        case '3': // Turn high speed telemetry 3 on
           	highSpeedTelemDisable();
           	highSpeedTelem3Enabled = true;

            cliQuery = 'x';
           	break;

        ///////////////////////////////

        case '4': // Turn high speed telemetry 4 on
           	highSpeedTelemDisable();
           	highSpeedTelem4Enabled = true;

            cliQuery = 'x';
           	break;

        ///////////////////////////////

        case '5': // Turn high speed telemetry 5 on
           	highSpeedTelemDisable();
           	highSpeedTelem5Enabled = true;

            cliQuery = 'x';
           	break;

        ///////////////////////////////

        case '6': // Turn high speed telemetry 6 on
           	highSpeedTelemDisable();
           	highSpeedTelem6Enabled = true;

            cliQuery = 'x';
           	break;

        ///////////////////////////////

        case '7': // Turn high speed telemetry 7 on
           	highSpeedTelemDisable();
           	highSpeedTelem7Enabled = true;

            cliQuery = 'x';
           	break;

        ///////////////////////////////

        case '8': // Turn high speed telemetry 8 on
           	highSpeedTelemDisable();
           	highSpeedTelem8Enabled = true;

            cliQuery = 'x';
           	break;

        ///////////////////////////////

        case '9': // Turn high speed telemetry 9 on
           	highSpeedTelemDisable();
           	highSpeedTelem9Enabled = true;

            cliQuery = 'x';
           	break;

        ///////////////////////////////

        case '0': // Disable high speed telemetry
           	highSpeedTelemDisable();

            cliQuery = 'x';
           	break;

        ///////////////////////////////

        ///////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////

        ///////////////////////////////

        case 'A': // Read Roll Rate PID Values
            readCliPID(ROLL_RATE_PID);
            cliPrint( "\nRoll Rate PID Received....\n" );

        	cliQuery = 'a';
        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'B': // Read Pitch Rate PID Values
            readCliPID(PITCH_RATE_PID);
            cliPrint( "\nPitch Rate PID Received....\n" );

        	cliQuery = 'a';
        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'C': // Read Yaw Rate PID Values
            readCliPID(YAW_RATE_PID);
            cliPrint( "\nYaw Rate PID Received....\n" );

        	cliQuery = 'a';
        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'D': // Read Roll Attitude PID Values
            readCliPID(ROLL_ATT_PID);
            cliPrint( "\nRoll Attitude PID Received....\n" );

        	cliQuery = 'b';
        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'E': // Read Pitch Attitude PID Values
            readCliPID(PITCH_ATT_PID);
            cliPrint( "\nPitch Attitude PID Received....\n" );

        	cliQuery = 'b';
        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'F': // Read Heading Hold PID Values
            readCliPID(HEADING_PID);
            cliPrint( "\nHeading PID Received....\n" );

        	cliQuery = 'b';
        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'G': // Read nDot PID Values
            readCliPID(NDOT_PID);
            cliPrint( "\nnDot PID Received....\n" );

        	cliQuery = 'c';
        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'H': // Read eDot PID Values
            readCliPID(EDOT_PID);
            cliPrint( "\neDot PID Received....\n" );

            cliQuery = 'c';
          	validCliCommand = false;
          	break;

        ///////////////////////////////

        case 'I': // Read hDot PID Values
            readCliPID(HDOT_PID);
            cliPrint( "\nhDot PID Received....\n" );

          	cliQuery = 'c';
          	validCliCommand = false;
          	break;

       	///////////////////////////////

        case 'J': // Read n PID Values
            readCliPID(N_PID);
            cliPrint( "\nn PID Received....\n" );

            cliQuery = 'd';
            validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'K': // Read e PID Values
            readCliPID(E_PID);
            cliPrint( "\ne PID Received....\n" );

            cliQuery = 'd';
            validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'L': // Read h PID Values
            readCliPID(H_PID);
            cliPrint( "\nh PID Received....\n" );

            cliQuery = 'd';
        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'M': // MAX7456 CLI
           	max7456CLI();

           	cliQuery = 'x';
        	validCliCommand = false;
        	break;

        ///////////////////////////////

        case 'N': // Mixer CLI
            mixerCLI();

            cliQuery = 'x';
            validCliCommand = false;
            break;

        ///////////////////////////////

        case 'O': // Receiver CLI
            receiverCLI();

            cliQuery = 'x';
            validCliCommand = false;
            break;

        ///////////////////////////////

        case 'P': // Sensor CLI
           	sensorCLI();

           	cliQuery = 'x';
           	validCliCommand = false;
           	break;

        ///////////////////////////////

        case 'Q': // GPS CLI
            gpsCLI();

            cliQuery = 'x';
           	validCliCommand = false;
           	break;

        ///////////////////////////////

        case 'R': // Reset to Bootloader
        	cliPrint("Entering Bootloader....\n\n");
        	delay(100);
        	systemReset(true);
        	break;

        ///////////////////////////////

        case 'S': // Reset System
        	cliPrint("\nSystem Reseting....\n\n");
        	delay(100);
        	systemReset(false);
        	break;

        ///////////////////////////////

        case 'T': // Not Used
            cliQuery = 'x';
           	validCliCommand = false;
           	break;

        ///////////////////////////////

        case 'U': // Not Used
            cliQuery = 'x';
         	validCliCommand = false;
         	break;

        ///////////////////////////////

        case 'V': // Reset EEPROM Parameters
            cliPrint( "\nEEPROM Parameters Reset....\n" );
            checkFirstTime(true);
            cliPrint("\nSystem Resetting....\n\n");
            delay(100);
            systemReset(false);
            break;

        ///////////////////////////////

        case 'W': // Write EEPROM Parameters
            cliPrint("\nWriting EEPROM Parameters....\n");
            writeEEPROM();

            cliQuery = 'x';
         	validCliCommand = false;
         	break;

        ///////////////////////////////

        case 'X': // Not Used
            cliQuery = 'x';
            validCliCommand = false;
            break;

        ///////////////////////////////

        case 'Y': // Not Used
            cliQuery = 'x';
            break;

        ///////////////////////////////

        case 'Z': // Not Used
            cliQuery = 'x';
            break;

        ///////////////////////////////

        case '?': // Command Summary
        	cliBusy = true;

        	cliPrint("\n");
   		    cliPrint("'a' Rate PIDs                              'A' Set Roll Rate PID Data   AB;P;I;D;windupGuard;dErrorCalc\n");
   		    cliPrint("'b' Attitude PIDs                          'B' Set Pitch Rate PID Data  BB;P;I;D;windupGuard;dErrorCalc\n");
   		    cliPrint("'c' Velocity PIDs                          'C' Set Yaw Rate PID Data    CB;P;I;D;windupGuard;dErrorCalc\n");
   		    cliPrint("'d' Position PIDs                          'D' Set Roll Att PID Data    DB;P;I;D;windupGuard;dErrorCalc\n");
   		    cliPrint("'e' Loop Delta Times                       'E' Set Pitch Att PID Data   EB;P;I;D;windupGuard;dErrorCalc\n");
   		    cliPrint("'f' Loop Execution Times                   'F' Set Hdg Hold PID Data    FB;P;I;D;windupGuard;dErrorCalc\n");
   		    cliPrint("'g' 500 Hz Accels                          'G' Set nDot PID Data        GB;P;I;D;windupGuard;dErrorCalc\n");
   		    cliPrint("'h' 100 Hz Earth Axis Accels               'H' Set eDot PID Data        HB;P;I;D;windupGuard;dErrorCalc\n");
   		    cliPrint("'i' 500 Hz Gyros                           'I' Set hDot PID Data        IB;P;I;D;windupGuard;dErrorCalc\n");
   		    cliPrint("'j' 10 hz Mag Data                         'J' Set n PID Data           JB;P;I;D;windupGuard;dErrorCalc\n");
   		    cliPrint("'k' Vertical Axis Variable                 'K' Set e PID Data           KB;P;I;D;windupGuard;dErrorCalc\n");
   		    cliPrint("'l' Attitudes                              'L' Set h PID Data           LB;P;I;D;windupGuard;dErrorCalc\n");
   		    cliPrint("\n");

   		    cliPrint("Press space bar for more, or enter a command....\n");
   		    while (cliAvailable() == false);
   		    cliQuery = cliRead();
   		    if (cliQuery != ' ')
   		    {
   		        validCliCommand = true;
   		        cliBusy = false;
   		    	return;
   		    }

   		    cliPrint("\n");
   		    cliPrint("'m' GPS Data                               'M' MAX7456 CLI\n");
   		    cliPrint("'n' GPS Stats                              'N' Mixer CLI\n");
   		    cliPrint("'o' Not Used                               'O' Receiver CLI\n");
   		    cliPrint("'p' Not Used                               'P' Sensor CLI\n");
   		    cliPrint("'q' Not Used                               'Q' GPS CLI\n");
   		    cliPrint("'r' Mode States                            'R' Reset and Enter Bootloader\n");
   		    cliPrint("'s' Raw Receiver Commands                  'S' Reset\n");
   		    cliPrint("'t' Processed Receiver Commands            'T' Not Used\n");
   		    cliPrint("'u' Command In Detent Discretes            'U' Not Used\n");
   		    cliPrint("'v' Motor PWM Outputs                      'V' Reset EEPROM Parameters\n");
   		    cliPrint("'w' Servo PWM Outputs                      'W' Write EEPROM Parameters\n");
   		    cliPrint("'x' Terminate Serial Communication         'X' Not Used\n");
   		    cliPrint("\n");

   		    cliPrint("Press space bar for more, or enter a command....\n");
   		    while (cliAvailable() == false);
   		    cliQuery = cliRead();
   		    if (cliQuery != ' ')
   		    {
   		    	validCliCommand = true;
   		    	cliBusy = false;
   		    	return;
   		    }

   		    cliPrint("\n");
   		    cliPrint("'y' ESC Calibration                        'Y' Not Used\n");
   		    cliPrint("'z' Not Used                               'Z' Not Used\n");
   		    cliPrint("'1' High Speed Telemetry 1 Enable\n");
   		    cliPrint("'2' High Speed Telemetry 2 Enable\n");
   		    cliPrint("'3' High Speed Telemetry 3 Enable\n");
   		    cliPrint("'4' High Speed Telemetry 4 Enable\n");
   		    cliPrint("'5' High Speed Telemetry 5 Enable\n");
   		    cliPrint("'6' High Speed Telemetry 6 Enable\n");
   		    cliPrint("'7' High Speed Telemetry 7 Enable\n");
   		    cliPrint("'8' High Speed Telemetry 8 Enable\n");
   		    cliPrint("'9' High Speed Telemetry 9 Enable\n");
   		    cliPrint("'0' High Speed Telemetry Disable           '?' Command Summary\n");
   		    cliPrint("\n");

  		    cliQuery = 'x';
  		    cliBusy = false;
   		    break;

            ///////////////////////////////
    }
}

///////////////////////////////////////////////////////////////////////////////
