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

eepromConfig_t eepromConfig;

uint8_t        execUpCount = 0;

sensors_t      sensors;

heading_t      heading;

uint16_t       timerValue;

///////////////////////////////////////////////////////////////////////////////

int main(void)
{
	///////////////////////////////////////////////////////////////////////////

	uint32_t currentTime;

    systemReady = false;

    systemInit();

    systemReady = true;

    while (1)
    {
    	///////////////////////////////

        if (frame_50Hz)
        {
        	frame_50Hz = false;

        	currentTime      = micros();
			deltaTime50Hz    = currentTime - previous50HzTime;
			previous50HzTime = currentTime;

			processFlightCommands();

			if (eepromConfig.osdEnabled)
			{
				if (eepromConfig.osdDisplayAlt)
				    displayAltitude(sensors.pressureAlt50Hz, 0.0f, DISENGAGED);

				if (eepromConfig.osdDisplayAH)
				    displayArtificialHorizon(sensors.attitude500Hz[ROLL], sensors.attitude500Hz[PITCH], flightMode);

				if (eepromConfig.osdDisplayAtt)
				    displayAttitude(sensors.attitude500Hz[ROLL], sensors.attitude500Hz[PITCH], flightMode);

				if (eepromConfig.osdDisplayHdg)
				    displayHeading(sensors.attitude500Hz[YAW]);
			}

			executionTime50Hz = micros() - currentTime;
        }

        ///////////////////////////////

        if (frame_10Hz)
        {
        	frame_10Hz = false;

        	currentTime      = micros();
			deltaTime10Hz    = currentTime - previous10HzTime;
			previous10HzTime = currentTime;

			if (newMagData == true)
			{
				sensors.mag10Hz[XAXIS] = -((float)rawMag[XAXIS].value * magScaleFactor[XAXIS] - eepromConfig.magBias[XAXIS]);
			    sensors.mag10Hz[YAXIS] =   (float)rawMag[YAXIS].value * magScaleFactor[YAXIS] - eepromConfig.magBias[YAXIS];
			    sensors.mag10Hz[ZAXIS] = -((float)rawMag[ZAXIS].value * magScaleFactor[ZAXIS] - eepromConfig.magBias[ZAXIS]);

			    newMagData = false;
			    magDataUpdate = true;
            }

        	switch (eepromConfig.gpsType)
			{
			    ///////////////////////

			    case NO_GPS:                // No GPS installed
			        break;

			    ///////////////////////

			    case MEDIATEK_3329_BINARY:  // MediaTek 3329 in binary mode
			    	decodeMediaTek3329BinaryMsg();
			    	break;

				///////////////////////

				case MEDIATEK_3329_NMEA:    // MediaTek 3329 in NMEA mode
				    decodeNMEAsentence();
	        	    break;

			    ///////////////////////

			    case UBLOX:                 // UBLOX in binary mode
			    	decodeUbloxMsg();
			    	break;

			    ///////////////////////
			}

        	cliCom();

        	rfCom();

            executionTime10Hz = micros() - currentTime;
        }

        ///////////////////////////////

        if (frame_500Hz)
        {
			frame_500Hz = false;

       	    currentTime       = micros();
       	    deltaTime500Hz    = currentTime - previous500HzTime;
       	    previous500HzTime = currentTime;

       	    TIM_Cmd(TIM6, DISABLE);
       	 	timerValue = TIM_GetCounter(TIM6);
       	 	TIM_SetCounter(TIM6, 0);
       	 	TIM_Cmd(TIM6, ENABLE);

       	 	dt500Hz = (float)timerValue * 0.0000005f;  // For integrations in 500 Hz loop

            sensors.accel500Hz[XAXIS] = -((float)accelSummedSamples500Hz[XAXIS] / 2.0f - eepromConfig.accelBias[XAXIS]) * eepromConfig.accelScaleFactor[XAXIS];
			sensors.accel500Hz[YAXIS] =  ((float)accelSummedSamples500Hz[YAXIS] / 2.0f - eepromConfig.accelBias[YAXIS]) * eepromConfig.accelScaleFactor[YAXIS];
			sensors.accel500Hz[ZAXIS] = -((float)accelSummedSamples500Hz[ZAXIS] / 2.0f - eepromConfig.accelBias[ZAXIS]) * eepromConfig.accelScaleFactor[ZAXIS];

			sensors.accel500Hz[XAXIS] = firstOrderFilter(sensors.accel500Hz[XAXIS], &firstOrderFilters[ACCEL500HZ_X_LOWPASS]);
			sensors.accel500Hz[YAXIS] = firstOrderFilter(sensors.accel500Hz[YAXIS], &firstOrderFilters[ACCEL500HZ_Y_LOWPASS]);
			sensors.accel500Hz[ZAXIS] = firstOrderFilter(sensors.accel500Hz[ZAXIS], &firstOrderFilters[ACCEL500HZ_Z_LOWPASS]);

			sensors.gyro500Hz[ROLL ] =  ((float)gyroSummedSamples500Hz[ROLL]  / 2.0f - gyroRTBias[ROLL ]) * GYRO_SCALE_FACTOR;
			sensors.gyro500Hz[PITCH] =  ((float)gyroSummedSamples500Hz[PITCH] / 2.0f - gyroRTBias[PITCH]) * GYRO_SCALE_FACTOR;
            sensors.gyro500Hz[YAW  ] = -((float)gyroSummedSamples500Hz[YAW]   / 2.0f - gyroRTBias[YAW  ]) * GYRO_SCALE_FACTOR;

            MargAHRSupdate( sensors.gyro500Hz[ROLL],   sensors.gyro500Hz[PITCH],  sensors.gyro500Hz[YAW],
                            sensors.accel500Hz[XAXIS], sensors.accel500Hz[YAXIS], sensors.accel500Hz[ZAXIS],
                            sensors.mag10Hz[XAXIS],    sensors.mag10Hz[YAXIS],    sensors.mag10Hz[ZAXIS],
                            eepromConfig.accelCutoff,
                            magDataUpdate,
                            dt500Hz );

            magDataUpdate = false;

            computeAxisCommands(dt500Hz);
            mixTable();
            writeServos();
            writeMotors();

       	    executionTime500Hz = micros() - currentTime;
		}

        ///////////////////////////////

        if (frame_100Hz)
        {
        	frame_100Hz = false;

        	currentTime       = micros();
			deltaTime100Hz    = currentTime - previous100HzTime;
			previous100HzTime = currentTime;

			TIM_Cmd(TIM7, DISABLE);
			timerValue = TIM_GetCounter(TIM7);
			TIM_SetCounter(TIM7, 0);
			TIM_Cmd(TIM7, ENABLE);

			dt100Hz = (float)timerValue * 0.0000005f;  // For integrations in 100 Hz loop

			sensors.accel100Hz[XAXIS] = -((float)accelSummedSamples100Hz[XAXIS] / 10.0f - eepromConfig.accelBias[XAXIS]) * eepromConfig.accelScaleFactor[XAXIS];
			sensors.accel100Hz[YAXIS] =  ((float)accelSummedSamples100Hz[YAXIS] / 10.0f - eepromConfig.accelBias[YAXIS]) * eepromConfig.accelScaleFactor[YAXIS];
			sensors.accel100Hz[ZAXIS] = -((float)accelSummedSamples100Hz[ZAXIS] / 10.0f - eepromConfig.accelBias[ZAXIS]) * eepromConfig.accelScaleFactor[ZAXIS];

			sensors.accel100Hz[XAXIS] = firstOrderFilter(sensors.accel100Hz[XAXIS], &firstOrderFilters[ACCEL100HZ_X_LOWPASS]);
			sensors.accel100Hz[YAXIS] = firstOrderFilter(sensors.accel100Hz[YAXIS], &firstOrderFilters[ACCEL100HZ_Y_LOWPASS]);
			sensors.accel100Hz[ZAXIS] = firstOrderFilter(sensors.accel100Hz[ZAXIS], &firstOrderFilters[ACCEL100HZ_Z_LOWPASS]);

            if (!newTemperatureReading)
			{
				readTemperatureRequestPressure();
			    newTemperatureReading = true;
			}
			else
			{
			    readPressureRequestTemperature();
			    newPressureReading = true;
			}

            if (newTemperatureReading && newPressureReading)
            {
                d1Value = d1.value;
                d2Value = d2.value;

                calculateTemperature();
                calculatePressureAltitude();

                newTemperatureReading = false;
                newPressureReading    = false;
            }

            createRotationMatrix();
            bodyAccelToEarthAccel();
            vertCompFilter(dt100Hz);

            if ( highSpeedTelem1Enabled == true )
            {
            	// 500 Hz Accels
            	telemetryPrintF("%9.4f, %9.4f, %9.4f\n", sensors.accel500Hz[XAXIS],
            	        			                     sensors.accel500Hz[YAXIS],
            	        			                     sensors.accel500Hz[ZAXIS]);
            }

            if ( highSpeedTelem2Enabled == true )
            {
            	// 500 Hz Gyros
            	telemetryPrintF("%9.4f, %9.4f, %9.4f\n", sensors.gyro500Hz[ROLL ],
            	        			                     sensors.gyro500Hz[PITCH],
            	        					             sensors.gyro500Hz[YAW  ]);
            }

            if ( highSpeedTelem3Enabled == true )
            {
            	// Roll Rate, Roll Rate Command
            	telemetryPrintF("%9.4f, %9.4f\n", sensors.gyro500Hz[ROLL],
            			                          rxCommand[ROLL]);
            }

            if ( highSpeedTelem4Enabled == true )
            {
            	// Pitch Rate, Pitch Rate Command
            	telemetryPrintF("%9.4f, %9.4f\n", sensors.gyro500Hz[PITCH],
            	            			          rxCommand[PITCH]);
            }

            if ( highSpeedTelem5Enabled == true )
            {
            	// Yaw Rate, Yaw Rate Command
            	telemetryPrintF("%9.4f, %9.4f\n", sensors.gyro500Hz[YAW],
            	            	                  rxCommand[YAW]);
            }

            if ( highSpeedTelem6Enabled == true )
            {
            	// 500 Hz Attitudes
            	telemetryPrintF("%9.4f, %9.4f, %9.4f\n", sensors.attitude500Hz[ROLL ],
            	        			                     sensors.attitude500Hz[PITCH],
            	        			                     sensors.attitude500Hz[YAW  ]);
            }

            if ( highSpeedTelem7Enabled == true )
            {
               	// Vertical Variables
            	telemetryPrintF("%9.4f, %9.4f, %9.4f, %9.4f\n", earthAxisAccels[ZAXIS],
            			                                        sensors.pressureAlt50Hz,
            			                                        hDotEstimate,
            			                                        hEstimate);
            }

            executionTime100Hz = micros() - currentTime;
        }

        ///////////////////////////////

        if (frame_5Hz)
        {
        	frame_5Hz = false;

        	currentTime     = micros();
			deltaTime5Hz    = currentTime - previous5HzTime;
			previous5HzTime = currentTime;

			if (execUp == true)
			    GREEN_W_LED_TOGGLE;

        	executionTime5Hz = micros() - currentTime;
        }

        ///////////////////////////////

        if (frame_1Hz)
        {
        	frame_1Hz = false;

        	currentTime     = micros();
			deltaTime1Hz    = currentTime - previous1HzTime;
			previous1HzTime = currentTime;

			if (execUp == true)
				GREEN_E_LED_TOGGLE;

			if (execUp == false)
			    execUpCount++;

			if ((execUpCount == 5) && (execUp == false))
			{
			    execUp = true;
			    RED_N_LED_OFF;
			    ORANGE_NE_LED_OFF;
			    GREEN_E_LED_OFF;
			    BLUE_SE_LED_OFF;
			    RED_S_LED_OFF;
			    ORANGE_SW_LED_OFF;
			    GREEN_W_LED_OFF;
			    BLUE_NW_LED_OFF;
			}

			executionTime1Hz = micros() - currentTime;
        }

        ////////////////////////////////
    }

    ///////////////////////////////////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////////////////
