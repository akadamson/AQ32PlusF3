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
// MAX7456 CLI
///////////////////////////////////////////////////////////////////////////////

void max7456CLI()
{
    uint8_t  max7456query;
    uint8_t  validQuery = false;

    cliBusy = true;

    cliPrint("\nEntering MAX7456 CLI....\n\n");

   	resetMax7456();

    while(true)
    {
    	if (!validQuery) cliPrint("MAX7456 CLI -> ");

		while ((cliAvailable() == false) && (validQuery == false));

		if (validQuery == false)
		    max7456query = cliRead();

		if (!validQuery) cliPrint("\n");

		switch(max7456query)
		{
            ///////////////////////

            case 'a': // OSD Configuration
                cliPrint("\nMAX7456 OSD Status:             ");
                if (eepromConfig.osdEnabled)
                	cliPrint("Enabled\n");
                else
               	    cliPrint("Disabled\n");

                cliPrint("OSD Default Video Standard:     ");
                if (eepromConfig.defaultVideoStandard)
                    cliPrint("PAL\n");
                else
                    cliPrint("NTSC\n");

                cliPrint("OSD Display Units:              ");
                if (eepromConfig.metricUnits)
                    cliPrint("Metric\n");
                else
                    cliPrint("English\n");

                cliPrint("OSD Altitude Display:           ");
                if (eepromConfig.osdDisplayAlt)
                    cliPrint("On\n");
                else
                    cliPrint("Off\n");

                cliPrint("OSD Artifical Horizon Display:  ");
                if (eepromConfig.osdDisplayAH)
                    cliPrint("On\n");
                else
                    cliPrint("Off\n");

                cliPrint("OSD Attitude Display:           ");
                if (eepromConfig.osdDisplayAtt)
                    cliPrint("On\n");
                else
                    cliPrint("Off\n");

                cliPrint("OSD Heading Display:            ");
                if (eepromConfig.osdDisplayHdg)
                    cliPrint("On\n");
                else
                    cliPrint("Off\n");

                cliPrint("\n");
                validQuery = false;
                break;

            ///////////////////////

            case 'b': // Enable OSD Altitude Display
                eepromConfig.osdDisplayAlt  = true;

                max7456query = 'a';
                validQuery = true;
                break;

            ///////////////////////

            case 'c': // Enable OSD Artifical Horizon Display
                eepromConfig.osdDisplayAH  = true;
                eepromConfig.osdDisplayAtt = false;

                max7456query = 'a';
                validQuery = true;
                break;

            ///////////////////////

            case 'd': // Enable OSD Attitude Display
                eepromConfig.osdDisplayAtt = true;
                eepromConfig.osdDisplayAH  = false;

                max7456query = 'a';
                validQuery = true;
                break;

            ///////////////////////

            case 'e': // Enable OSD Heading Display
                eepromConfig.osdDisplayHdg = true;

                max7456query = 'a';
                validQuery = true;
                break;

            ///////////////////////

            case 'q': // Set English Display Units
                eepromConfig.metricUnits = false;

                max7456query = 'a';
                validQuery = true;
                break;

            ///////////////////////

            case 'r': // Reset MAX7456
                resetMax7456();
                cliPrint("\nMAX7456 Reset....\n\n");
                break;

            ///////////////////////

            case 's': // Show character set
                showMax7456Font();
                cliPrint("\nMAX7456 Character Set Displayed....\n\n");
                break;

            ///////////////////////

            case 't': // Download font
                downloadMax7456Font();
                break;

            ///////////////////////

            case 'u': // Toggle OSD enabled status
   			    if (eepromConfig.osdEnabled)                   // If  Enabled
   			        eepromConfig.osdEnabled = false;           // Set Disabled
   			    else
   			    {                                              // If  Disabled
   			        eepromConfig.osdEnabled = true;            // Set Enabled
                    initMax7456();                             // and call init procedure
				}

                max7456query = 'a';
                validQuery = true;
   				break;

			///////////////////////

   		    case 'v': // Toggle default video standard
   			    if (eepromConfig.defaultVideoStandard)         // If  PAL
   			        eepromConfig.defaultVideoStandard = NTSC;  // Set NTSC
   			    else                                           // If  NTSC
   			        eepromConfig.defaultVideoStandard = PAL;   // Set PAL

                max7456query = 'a';
                validQuery = true;
   				break;

   		    ///////////////////////

   			case 'x':
   			    cliPrint("\nExiting MAX7456 CLI....\n\n");
   			    cliBusy = false;
   			    return;
   			    break;

            ///////////////////////

            case 'B': // Disable OSD Altitude Display
                eepromConfig.osdDisplayAlt = false;

                max7456query = 'a';
                validQuery = true;
                break;

            ///////////////////////

            case 'C': // Disable OSD Artifical Horizon Display
                eepromConfig.osdDisplayAH = false;

                max7456query = 'a';
                validQuery = true;
                break;

            ///////////////////////

            case 'D': // Disable OSD Attitude Display
                eepromConfig.osdDisplayAtt = false;

                max7456query = 'a';
                validQuery = true;
                break;

            ///////////////////////

            case 'E': // Disable OSD Heading Display
                eepromConfig.osdDisplayHdg = false;

                max7456query = 'a';
                validQuery = true;
                break;

           ///////////////////////

           case 'Q': // Set Metric Display Units
                eepromConfig.metricUnits = true;

                max7456query = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'W': // Write EEPROM Parameters
                cliPrint("\nWriting EEPROM Parameters....\n\n");
                writeEEPROM();
                break;

    		///////////////////////

			case '?':
			   	cliPrint("\n");
			   	cliPrint("'a' OSD Configuration\n");
			    cliPrint("'b' Enable OSD Altitude Display            'B' Disable OSD Altitude Display\n");
			   	cliPrint("'c' Enable OSD Artificial Horizon Display  'C' Disable OSD Artificial Horizon Display\n");
			   	cliPrint("'d' Enable OSD Attitude Display            'D' Disable OSD Attitude Display\n");
			   	cliPrint("'e' Enable OSD Heading Display             'E' Disable OSD Heading Display\n");
			   	cliPrint("'q' Set English Display Units              'Q' Set Metric Display Units\n");
			    cliPrint("'r' Reset MAX7456\n");
			   	cliPrint("'s' Display MAX7456 Character Set\n");
			   	cliPrint("'t' Download Font to MAX7456\n");
			   	cliPrint("'u' Change OSD Installed State\n");
			   	cliPrint("'v' Change Default Video Standard          'W' Write EEPROM Parameters\n");
			   	cliPrint("'x' Exit Sensor CLI                        '?' Command Summary\n");
			   	cliPrint("\n");
	    	    break;

	    	///////////////////////
	    }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Mixer CLI
///////////////////////////////////////////////////////////////////////////////

void mixerCLI()
{
    float    tempFloat;

    uint8_t  index;
    uint8_t  rows, columns;

    uint8_t  mixerQuery;
    uint8_t  validQuery = false;

    cliBusy = true;

    cliPrint("\nEntering Mixer CLI....\n\n");

    while(true)
    {
        cliPrint("Mixer CLI -> ");

		while ((cliAvailable() == false) && (validQuery == false));

		if (validQuery == false)
		    mixerQuery = cliRead();

		cliPrint("\n");

		switch(mixerQuery)
		{
            ///////////////////////////

            case 'a': // Mixer Configuration
                cliPrint("\nMixer Configuration:            ");
                switch (eepromConfig.mixerConfiguration)
                {
                    case MIXERTYPE_GIMBAL:
                    	cliPrint("MIXERTYPE GIMBAL\n");
                    	break;

                    ///////////////////////

                    case MIXERTYPE_FLYING_WING:
                    	cliPrint("MIXERTYPE FLYING WING\n");
                    	break;

                    ///////////////////////

                    case MIXERTYPE_BI:
                        cliPrint("MIXERTYPE BICOPTER\n");
                        break;

                    ///////////////////////

                    case MIXERTYPE_TRI:
                        cliPrint("MIXERTYPE TRICOPTER\n");
                        break;

                    ///////////////////////

                    case MIXERTYPE_QUADP:
                        cliPrint("MIXERTYPE QUAD PLUS\n");
                        break;

                    case MIXERTYPE_QUADX:
                        cliPrint("MIXERTYPE QUAD X\n");
                        break;

                    case MIXERTYPE_VTAIL4_NO_COMP:
                    	cliPrint("MULTITYPE VTAIL NO COMP\n");
                    	break;

                    case MIXERTYPE_VTAIL4_Y_COMP:
                    	cliPrint("MULTITYPE VTAIL Y COMP\n");
                    	break;

                    case MIXERTYPE_VTAIL4_RY_COMP:
                    	cliPrint("MULTITYPE VTAIL RY COMP\n");
                    	break;

                    case MIXERTYPE_VTAIL4_PY_COMP:
                    	cliPrint("MULTITYPE VTAIL PY COMP\n");
                    	break;

                    case MIXERTYPE_VTAIL4_RP_COMP:
                    	cliPrint("MULTITYPE VTAIL RP COMP\n");
                    	break;

                    case MIXERTYPE_VTAIL4_RPY_COMP:
                    	cliPrint("MULTITYPE VTAIL RPY COMP\n");
                    	break;

                    case MIXERTYPE_Y4:
                    	cliPrint("MIXERTYPE Y4\n");
                    	break;

                    ///////////////////////

                    case MIXERTYPE_HEX6P:
                        cliPrint("MIXERTYPE HEX PLUS\n");
                        break;

                    case MIXERTYPE_HEX6X:
                        cliPrint("MIXERTYPE HEX X\n");
                        break;

                    case MIXERTYPE_Y6:
                        cliPrint("MIXERTYPE Y6\n");
                        break;

                    ///////////////////////

                    case MIXERTYPE_OCTOF8P:
                        cliPrint("MIXERTYPE FLAT OCTO PLUS\n");
                        break;

                    case MIXERTYPE_OCTOF8X:
                        cliPrint("MIXERTYPE FLAT OCTO X\n");
                        break;

                    case MIXERTYPE_OCTOX8P:
                        cliPrint("MIXERTYPE COAXIAL OCTO PLUS\n");
                        break;

                    case MIXERTYPE_OCTOX8X:
                        cliPrint("MIXERTYPE COAXIAL OCTO X\n");
                        break;

                    ///////////////////////

                    case MIXERTYPE_FREEMIX:
                    	cliPrint("MIXERTYPE FREE MIX\n");
                    	break;
                }

                cliPrint("Number of Motors:               ");
                itoa(numberMotor,                         numberString, 10); cliPrint(numberString); cliPrint("\n");

                cliPrint("ESC PWM Rate:                   ");
                itoa((uint16_t)eepromConfig.escPwmRate,   numberString, 10); cliPrint(numberString); cliPrint("\n");

                cliPrint("Servo PWM Rate:                 ");
                itoa((uint16_t)eepromConfig.servoPwmRate, numberString, 10); cliPrint(numberString); cliPrint("\n");

                if ( eepromConfig.mixerConfiguration == MIXERTYPE_BI )
                {
                    cliPrint("BiCopter Left Servo Min:        ");
                    itoa((uint16_t)eepromConfig.biLeftServoMin,  numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("BiCopter Left Servo Mid:        ");
                    itoa((uint16_t)eepromConfig.biLeftServoMid,  numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("BiCopter Left Servo Max:        ");
                    itoa((uint16_t)eepromConfig.biLeftServoMax,  numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("BiCopter Right Servo Min:       ");
                    itoa((uint16_t)eepromConfig.biRightServoMin, numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("BiCopter Right Servo Mid:       ");
                    itoa((uint16_t)eepromConfig.biRightServoMid, numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("BiCopter Right Servo Max:       ");
                    itoa((uint16_t)eepromConfig.biRightServoMax, numberString, 10); cliPrint(numberString); cliPrint("\n");
                }

                if ( eepromConfig.mixerConfiguration == MIXERTYPE_FLYING_WING )
                {
                    cliPrint("Roll Direction Left:            ");
                    itoa((uint16_t)eepromConfig.rollDirectionLeft,   numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("Roll Direction Right:           ");
                    itoa((uint16_t)eepromConfig.rollDirectionRight,  numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("Pitch Direction Left:           ");
                    itoa((uint16_t)eepromConfig.pitchDirectionLeft,  numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("Pitch Direction Right:          ");
                    itoa((uint16_t)eepromConfig.pitchDirectionRight, numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("Wing Left Minimum:              ");
                    itoa((uint16_t)eepromConfig.wingLeftMinimum,     numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("Wing Left Maximum:              ");
                    itoa((uint16_t)eepromConfig.wingLeftMaximum,     numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("Wing Right Minimum:             ");
                    itoa((uint16_t)eepromConfig.wingRightMinimum,    numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("Wing Right Maximum:             ");
                    itoa((uint16_t)eepromConfig.wingRightMaximum,    numberString, 10); cliPrint(numberString); cliPrint("\n");
                }

                if ( eepromConfig.mixerConfiguration == MIXERTYPE_GIMBAL )
                {
                    cliPrint("Gimbal Roll Servo Min:          ");
                    itoa((uint16_t)eepromConfig.gimbalRollServoMin,  numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("Gimbal Roll Servo Mid:          ");
                    itoa((uint16_t)eepromConfig.gimbalRollServoMid,  numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("Gimbal Roll Servo Max:          ");
                    itoa((uint16_t)eepromConfig.gimbalRollServoMax,  numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("Gimbal Roll Servo Gain:        ");
                    ftoa(eepromConfig.gimbalRollServoGain, numberString);               cliPrint(numberString); cliPrint("\n");
                    cliPrint("Gimbal Pitch Servo Min:         ");
                    itoa((uint16_t)eepromConfig.gimbalPitchServoMin, numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("Gimbal Pitch Servo Mid:         ");
                    itoa((uint16_t)eepromConfig.gimbalPitchServoMid, numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("Gimbal Pitch Servo Max:         ");
                    itoa((uint16_t)eepromConfig.gimbalPitchServoMax, numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("Gimbal Pitch Servo Gain:       ");
                    ftoa(eepromConfig.gimbalPitchServoGain, numberString);              cliPrint(numberString); cliPrint("\n");
                }

                if ( eepromConfig.mixerConfiguration == MIXERTYPE_TRI )
                {
                    cliPrint("TriCopter Yaw Servo Min:        ");
                    itoa((uint16_t)eepromConfig.triYawServoMin, numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("TriCopter Yaw Servo Mid:        ");
                    itoa((uint16_t)eepromConfig.triYawServoMid, numberString, 10); cliPrint(numberString); cliPrint("\n");
                    cliPrint("TriCopter Yaw Servo Max:        ");
                    itoa((uint16_t)eepromConfig.triYawServoMax, numberString, 10); cliPrint(numberString); cliPrint("\n");
                }

                if (eepromConfig.mixerConfiguration == MIXERTYPE_VTAIL4_Y_COMP  ||
                    eepromConfig.mixerConfiguration == MIXERTYPE_VTAIL4_RY_COMP ||
                    eepromConfig.mixerConfiguration == MIXERTYPE_VTAIL4_PY_COMP ||
                    eepromConfig.mixerConfiguration == MIXERTYPE_VTAIL4_RP_COMP ||
                    eepromConfig.mixerConfiguration == MIXERTYPE_VTAIL4_RPY_COMP)
                {
                    cliPrint("V Tail Angle                   ");
                    ftoa(eepromConfig.vTailAngle, numberString); cliPrint(numberString); cliPrint("\n");
    			}

                cliPrint("Yaw Direction:                  ");
                itoa((int8_t)eepromConfig.yawDirection,   numberString, 10); cliPrint(numberString); cliPrint("\n\n");

                validQuery = false;
                break;

            ///////////////////////////

            case 'b': // Free Mix Matrix
        	    cliPrint("\nNumber of Free Mixer Motors:  ");
        	    itoa( eepromConfig.freeMixMotors, numberString, 10 ); cliPrint( numberString ); cliPrint("\n\n");
                cliPrint("         Roll    Pitch   Yaw\n");

        	    for ( index = 0; index < eepromConfig.freeMixMotors; index++ )
        	    {
        	    	cliPrint("Motor"); itoa(index, numberString, 10);       cliPrint(numberString); cliPrint("  ");
        	    	ftoa(eepromConfig.freeMix[index][ROLL ], numberString); cliPrint(numberString); cliPrint("  ");
        	    	ftoa(eepromConfig.freeMix[index][PITCH], numberString); cliPrint(numberString); cliPrint("  ");
        	    	ftoa(eepromConfig.freeMix[index][YAW  ], numberString); cliPrint(numberString); cliPrint("\n");
        	    }

        	    cliPrint("\n");
        	    validQuery = false;
        	    break;

            ///////////////////////////

			case 'x':
			    cliPrint("\nExiting Mixer CLI....\n\n");
			    cliBusy = false;
			    return;
			    break;

            ///////////////////////////

            case 'A': // Read Mixer Configuration
                eepromConfig.mixerConfiguration = (uint8_t)readFloatCLI();
                initMixer();

        	    mixerQuery = 'a';
                validQuery = true;
		        break;

            ///////////////////////////

            case 'B': // Read ESC and Servo PWM Update Rates
                eepromConfig.escPwmRate   = (uint16_t)readFloatCLI();
                eepromConfig.servoPwmRate = (uint16_t)readFloatCLI();

                pwmEscInit(eepromConfig.escPwmRate);
                pwmServoInit(eepromConfig.servoPwmRate);

                mixerQuery = 'a';
                validQuery = true;
        	    break;

            ///////////////////////////

            case 'C': // Read BiCopter Left Servo Parameters
           	    eepromConfig.biLeftServoMin = readFloatCLI();
           	    eepromConfig.biLeftServoMid = readFloatCLI();
           	    eepromConfig.biLeftServoMax = readFloatCLI();

           	    mixerQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'D': // Read BiCopter Right Servo Parameters
           	    eepromConfig.biRightServoMin = readFloatCLI();
           	    eepromConfig.biRightServoMid = readFloatCLI();
           	    eepromConfig.biRightServoMax = readFloatCLI();

           	    mixerQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'E': // Read Flying Wing Servo Directions
                eepromConfig.rollDirectionLeft   = readFloatCLI();
                eepromConfig.rollDirectionRight  = readFloatCLI();
                eepromConfig.pitchDirectionLeft  = readFloatCLI();
                eepromConfig.pitchDirectionRight = readFloatCLI();

         	    mixerQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'F': // Read Flying Wing Servo Limits
           	    eepromConfig.wingLeftMinimum  = readFloatCLI();
           	    eepromConfig.wingLeftMaximum  = readFloatCLI();
           	    eepromConfig.wingRightMinimum = readFloatCLI();
           	    eepromConfig.wingRightMaximum = readFloatCLI();

                mixerQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'G': // Read Free Mix Motor Number
           	    eepromConfig.freeMixMotors = (uint8_t)readFloatCLI();
           	    initMixer();

           	    mixerQuery = 'b';
                validQuery = true;
                break;

            ///////////////////////////

            case 'H': // Read Free Mix Matrix Element
                rows    = (uint8_t)readFloatCLI();
                columns = (uint8_t)readFloatCLI();
                eepromConfig.freeMix[rows][columns] = readFloatCLI();

                mixerQuery = 'b';
                validQuery = true;
                break;

            ///////////////////////////

            case 'I': // Read Gimbal Roll Servo Parameters
         	    eepromConfig.gimbalRollServoMin  = readFloatCLI();
           	    eepromConfig.gimbalRollServoMid  = readFloatCLI();
           	    eepromConfig.gimbalRollServoMax  = readFloatCLI();
           	    eepromConfig.gimbalRollServoGain = readFloatCLI();

           	    mixerQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'J': // Read Gimbal Pitch Servo Parameters
           	    eepromConfig.gimbalPitchServoMin  = readFloatCLI();
           	    eepromConfig.gimbalPitchServoMid  = readFloatCLI();
           	    eepromConfig.gimbalPitchServoMax  = readFloatCLI();
           	    eepromConfig.gimbalPitchServoGain = readFloatCLI();

           	    mixerQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'K': // Read TriCopter YawServo Parameters
        	    eepromConfig.triYawServoMin = readFloatCLI();
           	    eepromConfig.triYawServoMid = readFloatCLI();
           	    eepromConfig.triYawServoMax = readFloatCLI();

           	    mixerQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'L': // Read V Tail Angle
        	    eepromConfig.vTailAngle = readFloatCLI();

        	    mixerQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'M': // Read yaw direction
                tempFloat = readFloatCLI();
                if (tempFloat >= 0.0)
                    tempFloat = 1.0;
                else
                	tempFloat = -1.0;

                eepromConfig.yawDirection = tempFloat;

                mixerQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'W': // Write EEPROM Parameters
                cliPrint("\nWriting EEPROM Parameters....\n\n");
                writeEEPROM();
                break;

			///////////////////////////

			case '?':
			   	cliPrint("\n");
			   	cliPrint("'a' Mixer Configuration Data               'A' Set Mixer Configuration              A1 thru 21, see aq32Plus.h\n");
   		        cliPrint("'b' Free Mixer Configuration               'B' Set PWM Rates                        BESC;Servo\n");
			   	cliPrint("                                           'C' Set BiCopter Left Servo Parameters   CMin;Mid;Max\n");
			   	cliPrint("                                           'D' Set BiCopter Right Servo Parameters  DMin;Mid;Max\n");
			   	cliPrint("                                           'E' Set Flying Wing Servo Directions     ERollLeft;RollRight;PitchLeft;PitchRight\n");
			   	cliPrint("                                           'F' Set Flying Wing Servo Limits         FLeftMin;LeftMax;RightMin;RightMax\n");
   		        cliPrint("                                           'G' Set Number of FreeMix Motors         GNumber\n");
   		        cliPrint("                                           'H' Set FreeMix Matrix Element           HRow;Column;Element\n");
   		        cliPrint("                                           'I' Set Gimbal Roll Servo Parameters     IMin;Mid;Max;Gain\n");
   		        cliPrint("                                           'J' Set Gimbal Pitch Servo Parameters    JMin;Mid;Max;Gain\n");
   		        cliPrint("                                           'K' Set TriCopter Servo Parameters       KMin;Mid;Max\n");
   		        cliPrint("                                           'L' Set V Tail Angle                     LAngle\n");
   		        cliPrint("                                           'M' Set Yaw Direction                    M1 or M-1\n");
   		        cliPrint("                                           'W' Write EEPROM Parameters\n");
   		        cliPrint("'x' Exit Sensor CLI                        '?' Command Summary\n");
   		        cliPrint("\n");
	    	    break;

	    	///////////////////////////
	    }
	}

}

///////////////////////////////////////////////////////////////////////////////
// Receiver CLI
///////////////////////////////////////////////////////////////////////////////

void receiverCLI()
{
    char     rcOrderString[9];
    float    tempFloat;
    uint8_t  index;
    uint8_t  receiverQuery;
    uint8_t  validQuery = false;

    cliBusy = true;

    cliPrint("\nEntering Receiver CLI....\n\n");

    while(true)
    {
        cliPrint("Receiver CLI -> ");

		while ((cliAvailable() == false) && (validQuery == false));

		if (validQuery == false)
		    receiverQuery = cliRead();

		cliPrint("\n");

		switch(receiverQuery)
		{
            ///////////////////////////

            case 'a': // Receiver Configuration
                cliPrint("\nReceiver Type:                  ");
                switch(eepromConfig.receiverType)
                {
                    case PARALLEL_PWM:
                        cliPrint("Parallel\n");
                        break;
                    case SERIAL_PWM:
                        cliPrint("Serial\n");
                        break;
                    case SPEKTRUM:
                        cliPrint("Spektrum\n");
                        break;
		        }

                cliPrint("Current RC Channel Assignment:  ");
                for (index = 0; index < 8; index++)
                    rcOrderString[eepromConfig.rcMap[index]] = rcChannelLetters[index];

                rcOrderString[index] = '\0';

                cliPrint(rcOrderString);  cliPrint("\n");

                cliPrint("Spektrum Resolution:            ");
                if (eepromConfig.spektrumHires)
				    cliPrint("11 Bit Mode\n");
				else
				    cliPrint("10 Bit Mode\n");

				cliPrint("Number of Spektrum Channels:    ");
				snprintf(numberString, 16, "%d\n", eepromConfig.spektrumChannels); cliPrint(numberString);

                cliPrint("Mid Command:                    ");
                snprintf(numberString, 16, "%d\n", (uint16_t)eepromConfig.midCommand); cliPrint(numberString);

				cliPrint("Min Check:                      ");
                snprintf(numberString, 16, "%d\n", (uint16_t)eepromConfig.minCheck); cliPrint(numberString);

				cliPrint("Max Check:                      ");
                snprintf(numberString, 16, "%d\n", (uint16_t)eepromConfig.maxCheck); cliPrint(numberString);

				cliPrint("Min Throttle:                   ");
                snprintf(numberString, 16, "%d\n", (uint16_t)eepromConfig.minThrottle); cliPrint(numberString);

				cliPrint("Max Thottle:                    ");
                snprintf(numberString, 16, "%d\n\n", (uint16_t)eepromConfig.maxThrottle); cliPrint(numberString);

				cliPrint("Max Rate Command:               ");
				tempFloat = eepromConfig.rateScaling * 180000.0 / PI;
				snprintf(numberString, 16, "%6.2f DPS\n", tempFloat); cliPrint(numberString);

				cliPrint("Max Attitude Command:           ");
				tempFloat = eepromConfig.attitudeScaling * 180000.0 / PI;
				snprintf(numberString, 18, "%6.2f Degrees\n\n", tempFloat); cliPrint(numberString);

				validQuery = false;
                break;

            ///////////////////////////

            case 'b': // Read Max Rate Value
                eepromConfig.rateScaling = readFloatCLI() / 180000 * PI;

                receiverQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'c': // Read Max Attitude Value
                eepromConfig.attitudeScaling = readFloatCLI() / 180000 * PI;

                receiverQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

			case 'x':
			    cliPrint("\nExiting Receiver CLI....\n\n");
			    cliBusy = false;
			    return;
			    break;

            ///////////////////////////

            case 'A': // Read RX Input Type
                eepromConfig.receiverType = (uint8_t)readFloatCLI();
			    cliPrint( "\nReceiver Type Changed....\n");

			    cliPrint("\nSystem Resetting....\n");
			    delay(100);
			    writeEEPROM();
			    systemReset(false);

		        break;

            ///////////////////////////

            case 'B': // Read RC Control Order
                readStringCLI( rcOrderString, 8 );
                parseRcChannels( rcOrderString );

          	    receiverQuery = 'a';
                validQuery = true;
        	    break;

            ///////////////////////////

            case 'C': // Read Spektrum Resolution
                eepromConfig.spektrumHires = (uint8_t)readFloatCLI();

                receiverQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'D': // Read Number of Spektrum Channels
                eepromConfig.spektrumChannels = (uint8_t)readFloatCLI();

                receiverQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'E': // Read RC Control Points
                eepromConfig.midCommand   = readFloatCLI();
    	        eepromConfig.minCheck     = readFloatCLI();
    		    eepromConfig.maxCheck     = readFloatCLI();
    		    eepromConfig.minThrottle  = readFloatCLI();
    		    eepromConfig.maxThrottle  = readFloatCLI();

                receiverQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'W': // Write EEPROM Parameters
                cliPrint("\nWriting EEPROM Parameters....\n\n");
                writeEEPROM();
                break;

			///////////////////////////

			case '?':
			   	cliPrint("\n");
			   	cliPrint("'a' Receiver Configuration Data            'A' Set RX Input Type                    AX, 1=Parallel, 2=Serial, 3=Spektrum\n");
   		        cliPrint("'b' Set Maximum Rate Command               'B' Set RC Control Order                 BTAER1234\n");
			   	cliPrint("'c' Set Maximum Attitude Command           'C' Set Spektrum Resolution              C0 or C1\n");
			   	cliPrint("                                           'D' Set Number of Spektrum Channels      D6 thru D12\n");
			   	cliPrint("                                           'E' Set RC Control Points                EmidCmd;minChk;maxChk;minThrot;maxThrot\n");
			   	cliPrint("                                           'W' Write EEPROM Parameters\n");
			   	cliPrint("'x' Exit Receiver CLI                      '?' Command Summary\n");
			   	cliPrint("\n");
	    	    break;

	    	///////////////////////////
	    }
	}

}

///////////////////////////////////////////////////////////////////////////////
// Sensor CLI
///////////////////////////////////////////////////////////////////////////////

void sensorCLI()
{
    uint8_t  sensorQuery;
    // HJI uint8_t  tempInt;
    uint8_t  validQuery = false;

    cliBusy = true;

    cliPrint("\nEntering Sensor CLI....\n\n");

    while(true)
    {
        cliPrint("Sensor CLI -> ");

		while ((cliAvailable() == false) && (validQuery == false));

		if (validQuery == false)
		    sensorQuery = cliRead();

		cliPrint("\n");

		switch(sensorQuery)
		{
            ///////////////////////////

            case 'a': // Sensor Data
                cliPrint("\n");

                cliPrint("Accel One G:               ");
                snprintf(numberString, 16, "%9.4f\n", accelOneG); cliPrint(numberString);

                cliPrint("Mag Bias:                  ");
                snprintf(numberString, 16, "%9.4f, ", eepromConfig.magBias[XAXIS]); cliPrint(numberString);
                snprintf(numberString, 16, "%9.4f, ", eepromConfig.magBias[YAXIS]); cliPrint(numberString);
                snprintf(numberString, 16, "%9.4f\n", eepromConfig.magBias[ZAXIS]); cliPrint(numberString);

                cliPrint("Accel Cutoff:              ");
                snprintf(numberString, 16, "%9.4f\n", eepromConfig.accelCutoff); cliPrint(numberString);

                cliPrint("KpAcc (MARG):              ");
                snprintf(numberString, 16, "%9.4f\n", eepromConfig.KpAcc); cliPrint(numberString);

                cliPrint("KiAcc (MARG):              ");
                snprintf(numberString, 16, "%9.4f\n", eepromConfig.KiAcc); cliPrint(numberString);

                cliPrint("KpMag (MARG):              ");
                snprintf(numberString, 16, "%9.4f\n", eepromConfig.KpMag); cliPrint(numberString);

                cliPrint("KiMag (MARG):              ");
                snprintf(numberString, 16, "%9.4f\n", eepromConfig.KiMag); cliPrint(numberString);

                cliPrint("hdot est/h est Comp Fil A: ");
                snprintf(numberString, 16, "%9.4f\n", eepromConfig.compFilterA); cliPrint(numberString);

                cliPrint("hdot est/h est Comp Fil B: ");
                snprintf(numberString, 16, "%9.4f\n\n", eepromConfig.compFilterB); cliPrint(numberString);

                validQuery = false;
                break;

            ///////////////////////////

            case 'b': // Accel Calibration
                accelCalibration();

                sensorQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'c': // Magnetometer Calibration
                magCalibration();

                sensorQuery = 'a';
                validQuery = true;
                break;

			///////////////////////////

			case 'x':
			    cliPrint("\nExiting Sensor CLI....\n\n");
			    cliBusy = false;
			    return;
			    break;

            ///////////////////////////

            case 'B': // Accel Cutoff
                eepromConfig.accelCutoff = readFloatCLI();

                sensorQuery = 'a';
                validQuery = true;
        	    break;

            ///////////////////////////

            case 'C': // kpAcc, kiAcc
                eepromConfig.KpAcc = readFloatCLI();
                eepromConfig.KiAcc = readFloatCLI();

                sensorQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'D': // kpMag, kiMag
                eepromConfig.KpMag = readFloatCLI();
                eepromConfig.KiMag = readFloatCLI();

                sensorQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'E': // h dot est/h est Comp Filter A/B
                eepromConfig.compFilterA = readFloatCLI();
                eepromConfig.compFilterB = readFloatCLI();

                sensorQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'M': // Magnetic Variation
                eepromConfig.magVar = readFloatCLI() * D2R;

                sensorQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'W': // Write EEPROM Parameters
                cliPrint("\nWriting EEPROM Parameters....\n\n");
                writeEEPROM();
                break;

			///////////////////////////

			case '?':
			   	cliPrint("\n");
			   	cliPrint("'a' Display Sensor Data\n");
			   	cliPrint("'b' Accel Calibration                      'B' Set Accel Cutoff                     BAccelCutoff\n");
			   	cliPrint("'c' Magnetometer Calibration               'C' Set kpAcc/kiAcc                      CKpAcc;KiAcc\n");
			   	cliPrint("                                           'D' Set kpMag/kiMag                      DKpMag;KiMag\n");
			   	cliPrint("                                           'E' Set h dot est/h est Comp Filter A/B  EA;B\n");
			   	cliPrint("                                           'W' Write EEPROM Parameters\n");
			   	cliPrint("'x' Exit Sensor CLI                        '?' Command Summary\n");
			    cliPrint("\n");
	    	    break;

	    	///////////////////////////
	    }
	}

}

///////////////////////////////////////////////////////////////////////////////
// GPS CLI
///////////////////////////////////////////////////////////////////////////////

void gpsCLI()
{
    uint8_t  gpsQuery;
    uint8_t  validQuery = false;

    cliBusy = true;

    cliPrint("\nEntering GPS CLI....\n\n");

    while(true)
    {
        cliPrint("GPS CLI -> ");

		while ((cliAvailable() == false) && (validQuery == false));

		if (validQuery == false)
		    gpsQuery = cliRead();

		cliPrint("\n");

		switch(gpsQuery)
		{
            ///////////////////////////

            case 'a': // GPS Installation Data
                cliPrint("\n");

				switch(eepromConfig.gpsType)
				{
					///////////////

					case NO_GPS:
					    cliPrint("No GPS Installed....\n\n");
					    break;

					///////////////

					case MEDIATEK_3329_BINARY:
					    cliPrint("MediaTek 3329 GPS installed, Binary Mode....\n\n");
					    break;

					///////////////

					case MEDIATEK_3329_NMEA:
					    cliPrint("MediaTek 3329 GPS Installed, NMEA Mode....\n\n");
					    break;

					///////////////

					case UBLOX:
					    cliPrint("UBLOX GPS Installed, Binary Mode....\n\n");
					    break;

					///////////////
				}

                validQuery = false;
                break;

            ///////////////////////////

			case 'x':
			    cliPrint("\nExiting GPS CLI....\n\n");
			    cliBusy = false;
			    return;
			    break;

            ///////////////////////////

            case 'A': // Set GPS Installed State to False
                eepromConfig.gpsType = NO_GPS;

                gpsQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'B': // Set GPS Type to MediaTek 3329 Binary
                eepromConfig.gpsType = MEDIATEK_3329_BINARY;

                initGPS();

                gpsQuery = 'a';
                validQuery = true;
        	    break;

            ///////////////////////////

            case 'C': // Set GPS Type to MediaTek 3329 NMEA
                eepromConfig.gpsType = MEDIATEK_3329_NMEA;

                initGPS();

                gpsQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'D': // Set GPS Type to UBLOX Binary
                eepromConfig.gpsType = UBLOX;

                initGPS();

                gpsQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'W': // Write EEPROM Parameters
                cliPrint("\nWriting EEPROM Parameters....\n\n");
                writeEEPROM();
                break;

			///////////////////////////

			case '?':
			   	cliPrint("\n");
			   	cliPrint("'a' Display GPS Installation Data          'A' Set GPS Type to No GPS\n");
			   	cliPrint("                                           'B' Set GPS Type to MediaTek 3329 Binary\n");
			   	cliPrint("                                           'C' Set GPS Type to MediaTek 3329 NMEA\n");
			   	cliPrint("                                           'D' Set GPS Type to UBLOX\n");
			   	cliPrint("                                           'W' Write EEPROM Parameters\n");
			   	cliPrint("'x' Exit GPS CLI                           '?' Command Summary\n");
			    cliPrint("\n");
	    	    break;

	    	///////////////////////////
	    }
	}

}

///////////////////////////////////////////////////////////////////////////////
