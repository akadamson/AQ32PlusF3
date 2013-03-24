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

    uart1Print("\nEntering MAX7456 CLI....\n\n");

   	resetMax7456();

    while(true)
    {
    	if (!validQuery) uart1Print("MAX7456 CLI -> ");

		while ((uart1Available() == false) && (validQuery == false));

		if (validQuery == false)
		    max7456query = uart1Read();

		if (!validQuery) uart1Print("\n");

		switch(max7456query)
		{
            ///////////////////////

            case 'a': // OSD Configuration
                uart1Print("\nMAX7456 OSD Status:             ");
                if (eepromConfig.osdEnabled)
                	uart1Print("Enabled\n");
                else
               	    uart1Print("Disabled\n");

                uart1Print("OSD Default Video Standard:     ");
                if (eepromConfig.defaultVideoStandard)
                    uart1Print("PAL\n");
                else
                    uart1Print("NTSC\n");

                uart1Print("OSD Display Units:              ");
                if (eepromConfig.metricUnits)
                    uart1Print("Metric\n");
                else
                    uart1Print("English\n");

                uart1Print("OSD Altitude Display:           ");
                if (eepromConfig.osdDisplayAlt)
                    uart1Print("On\n");
                else
                    uart1Print("Off\n");

                uart1Print("OSD Artifical Horizon Display:  ");
                if (eepromConfig.osdDisplayAH)
                    uart1Print("On\n");
                else
                    uart1Print("Off\n");

                uart1Print("OSD Attitude Display:           ");
                if (eepromConfig.osdDisplayAtt)
                    uart1Print("On\n");
                else
                    uart1Print("Off\n");

                uart1Print("OSD Heading Display:            ");
                if (eepromConfig.osdDisplayHdg)
                    uart1Print("On\n");
                else
                    uart1Print("Off\n");

                uart1Print("\n");
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
                uart1Print("\nMAX7456 Reset....\n\n");
                break;

            ///////////////////////

            case 's': // Show character set
                showMax7456Font();
                uart1Print("\nMAX7456 Character Set Displayed....\n\n");
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
   			    uart1Print("\nExiting MAX7456 CLI....\n\n");
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
                uart1Print("\nWriting EEPROM Parameters....\n\n");
                writeEEPROM();
                break;

    		///////////////////////

			case '?':
			   	uart1Print("\n");
			   	uart1Print("'a' OSD Configuration\n");
			    uart1Print("'b' Enable OSD Altitude Display            'B' Disable OSD Altitude Display\n");
			   	uart1Print("'c' Enable OSD Artificial Horizon Display  'C' Disable OSD Artificial Horizon Display\n");
			   	uart1Print("'d' Enable OSD Attitude Display            'D' Disable OSD Attitude Display\n");
			   	uart1Print("'e' Enable OSD Heading Display             'E' Disable OSD Heading Display\n");
			   	uart1Print("'q' Set English Display Units              'Q' Set Metric Display Units\n");
			    uart1Print("'r' Reset MAX7456\n");
			   	uart1Print("'s' Display MAX7456 Character Set\n");
			   	uart1Print("'t' Download Font to MAX7456\n");
			   	uart1Print("'u' Change OSD Installed State\n");
			   	uart1Print("'v' Change Default Video Standard          'W' Write EEPROM Parameters\n");
			   	uart1Print("'x' Exit Sensor CLI                        '?' Command Summary\n");
			   	uart1Print("\n");
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

    uart1Print("\nEntering Mixer CLI....\n\n");

    while(true)
    {
        uart1Print("Mixer CLI -> ");

		while ((uart1Available() == false) && (validQuery == false));

		if (validQuery == false)
		    mixerQuery = uart1Read();

		uart1Print("\n");

		switch(mixerQuery)
		{
            ///////////////////////////

            case 'a': // Mixer Configuration
                uart1Print("\nMixer Configuration:            ");
                switch (eepromConfig.mixerConfiguration)
                {
                    case MIXERTYPE_GIMBAL:
                    	uart1Print("MIXERTYPE GIMBAL\n");
                    	break;

                    ///////////////////////

                    case MIXERTYPE_FLYING_WING:
                    	uart1Print("MIXERTYPE FLYING WING\n");
                    	break;

                    ///////////////////////

                    case MIXERTYPE_BI:
                        uart1Print("MIXERTYPE BICOPTER\n");
                        break;

                    ///////////////////////

                    case MIXERTYPE_TRI:
                        uart1Print("MIXERTYPE TRICOPTER\n");
                        break;

                    ///////////////////////

                    case MIXERTYPE_QUADP:
                        uart1Print("MIXERTYPE QUAD PLUS\n");
                        break;

                    case MIXERTYPE_QUADX:
                        uart1Print("MIXERTYPE QUAD X\n");
                        break;

                    case MIXERTYPE_VTAIL4_NO_COMP:
                    	uart1Print("MULTITYPE VTAIL NO COMP\n");
                    	break;

                    case MIXERTYPE_VTAIL4_Y_COMP:
                    	uart1Print("MULTITYPE VTAIL Y COMP\n");
                    	break;

                    case MIXERTYPE_VTAIL4_RY_COMP:
                    	uart1Print("MULTITYPE VTAIL RY COMP\n");
                    	break;

                    case MIXERTYPE_VTAIL4_PY_COMP:
                    	uart1Print("MULTITYPE VTAIL PY COMP\n");
                    	break;

                    case MIXERTYPE_VTAIL4_RP_COMP:
                    	uart1Print("MULTITYPE VTAIL RP COMP\n");
                    	break;

                    case MIXERTYPE_VTAIL4_RPY_COMP:
                    	uart1Print("MULTITYPE VTAIL RPY COMP\n");
                    	break;

                    case MIXERTYPE_Y4:
                    	uart1Print("MIXERTYPE Y4\n");
                    	break;

                    ///////////////////////

                    case MIXERTYPE_HEX6P:
                        uart1Print("MIXERTYPE HEX PLUS\n");
                        break;

                    case MIXERTYPE_HEX6X:
                        uart1Print("MIXERTYPE HEX X\n");
                        break;

                    case MIXERTYPE_Y6:
                        uart1Print("MIXERTYPE Y6\n");
                        break;

                    ///////////////////////

                    case MIXERTYPE_OCTOF8P:
                        uart1Print("MIXERTYPE FLAT OCTO PLUS\n");
                        break;

                    case MIXERTYPE_OCTOF8X:
                        uart1Print("MIXERTYPE FLAT OCTO X\n");
                        break;

                    case MIXERTYPE_OCTOX8P:
                        uart1Print("MIXERTYPE COAXIAL OCTO PLUS\n");
                        break;

                    case MIXERTYPE_OCTOX8X:
                        uart1Print("MIXERTYPE COAXIAL OCTO X\n");
                        break;

                    ///////////////////////

                    case MIXERTYPE_FREEMIX:
                    	uart1Print("MIXERTYPE FREE MIX\n");
                    	break;
                }

                uart1Print("Number of Motors:               ");
                itoa(numberMotor,                         numberString, 10); uart1Print(numberString); uart1Print("\n");

                uart1Print("ESC PWM Rate:                   ");
                itoa((uint16_t)eepromConfig.escPwmRate,   numberString, 10); uart1Print(numberString); uart1Print("\n");

                uart1Print("Servo PWM Rate:                 ");
                itoa((uint16_t)eepromConfig.servoPwmRate, numberString, 10); uart1Print(numberString); uart1Print("\n");

                if ( eepromConfig.mixerConfiguration == MIXERTYPE_BI )
                {
                    uart1Print("BiCopter Left Servo Min:        ");
                    itoa((uint16_t)eepromConfig.biLeftServoMin,  numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("BiCopter Left Servo Mid:        ");
                    itoa((uint16_t)eepromConfig.biLeftServoMid,  numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("BiCopter Left Servo Max:        ");
                    itoa((uint16_t)eepromConfig.biLeftServoMax,  numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("BiCopter Right Servo Min:       ");
                    itoa((uint16_t)eepromConfig.biRightServoMin, numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("BiCopter Right Servo Mid:       ");
                    itoa((uint16_t)eepromConfig.biRightServoMid, numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("BiCopter Right Servo Max:       ");
                    itoa((uint16_t)eepromConfig.biRightServoMax, numberString, 10); uart1Print(numberString); uart1Print("\n");
                }

                if ( eepromConfig.mixerConfiguration == MIXERTYPE_FLYING_WING )
                {
                    uart1Print("Roll Direction Left:            ");
                    itoa((uint16_t)eepromConfig.rollDirectionLeft,   numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("Roll Direction Right:           ");
                    itoa((uint16_t)eepromConfig.rollDirectionRight,  numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("Pitch Direction Left:           ");
                    itoa((uint16_t)eepromConfig.pitchDirectionLeft,  numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("Pitch Direction Right:          ");
                    itoa((uint16_t)eepromConfig.pitchDirectionRight, numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("Wing Left Minimum:              ");
                    itoa((uint16_t)eepromConfig.wingLeftMinimum,     numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("Wing Left Maximum:              ");
                    itoa((uint16_t)eepromConfig.wingLeftMaximum,     numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("Wing Right Minimum:             ");
                    itoa((uint16_t)eepromConfig.wingRightMinimum,    numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("Wing Right Maximum:             ");
                    itoa((uint16_t)eepromConfig.wingRightMaximum,    numberString, 10); uart1Print(numberString); uart1Print("\n");
                }

                if ( eepromConfig.mixerConfiguration == MIXERTYPE_GIMBAL )
                {
                    uart1Print("Gimbal Roll Servo Min:          ");
                    itoa((uint16_t)eepromConfig.gimbalRollServoMin,  numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("Gimbal Roll Servo Mid:          ");
                    itoa((uint16_t)eepromConfig.gimbalRollServoMid,  numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("Gimbal Roll Servo Max:          ");
                    itoa((uint16_t)eepromConfig.gimbalRollServoMax,  numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("Gimbal Roll Servo Gain:        ");
                    ftoa(eepromConfig.gimbalRollServoGain, numberString);               uart1Print(numberString); uart1Print("\n");
                    uart1Print("Gimbal Pitch Servo Min:         ");
                    itoa((uint16_t)eepromConfig.gimbalPitchServoMin, numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("Gimbal Pitch Servo Mid:         ");
                    itoa((uint16_t)eepromConfig.gimbalPitchServoMid, numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("Gimbal Pitch Servo Max:         ");
                    itoa((uint16_t)eepromConfig.gimbalPitchServoMax, numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("Gimbal Pitch Servo Gain:       ");
                    ftoa(eepromConfig.gimbalPitchServoGain, numberString);              uart1Print(numberString); uart1Print("\n");
                }

                if ( eepromConfig.mixerConfiguration == MIXERTYPE_TRI )
                {
                    uart1Print("TriCopter Yaw Servo Min:        ");
                    itoa((uint16_t)eepromConfig.triYawServoMin, numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("TriCopter Yaw Servo Mid:        ");
                    itoa((uint16_t)eepromConfig.triYawServoMid, numberString, 10); uart1Print(numberString); uart1Print("\n");
                    uart1Print("TriCopter Yaw Servo Max:        ");
                    itoa((uint16_t)eepromConfig.triYawServoMax, numberString, 10); uart1Print(numberString); uart1Print("\n");
                }

                if (eepromConfig.mixerConfiguration == MIXERTYPE_VTAIL4_Y_COMP  ||
                    eepromConfig.mixerConfiguration == MIXERTYPE_VTAIL4_RY_COMP ||
                    eepromConfig.mixerConfiguration == MIXERTYPE_VTAIL4_PY_COMP ||
                    eepromConfig.mixerConfiguration == MIXERTYPE_VTAIL4_RP_COMP ||
                    eepromConfig.mixerConfiguration == MIXERTYPE_VTAIL4_RPY_COMP)
                {
                    uart1Print("V Tail Angle                   ");
                    ftoa(eepromConfig.vTailAngle, numberString); uart1Print(numberString); uart1Print("\n");
    			}

                uart1Print("Yaw Direction:                  ");
                itoa((int8_t)eepromConfig.yawDirection,   numberString, 10); uart1Print(numberString); uart1Print("\n\n");

                validQuery = false;
                break;

            ///////////////////////////

            case 'b': // Free Mix Matrix
        	    uart1Print("\nNumber of Free Mixer Motors:  ");
        	    itoa( eepromConfig.freeMixMotors, numberString, 10 ); uart1Print( numberString ); uart1Print("\n\n");
                uart1Print("         Roll    Pitch   Yaw\n");

        	    for ( index = 0; index < eepromConfig.freeMixMotors; index++ )
        	    {
        	    	uart1Print("Motor"); itoa(index, numberString, 10);       uart1Print(numberString); uart1Print("  ");
        	    	ftoa(eepromConfig.freeMix[index][ROLL ], numberString); uart1Print(numberString); uart1Print("  ");
        	    	ftoa(eepromConfig.freeMix[index][PITCH], numberString); uart1Print(numberString); uart1Print("  ");
        	    	ftoa(eepromConfig.freeMix[index][YAW  ], numberString); uart1Print(numberString); uart1Print("\n");
        	    }

        	    uart1Print("\n");
        	    validQuery = false;
        	    break;

            ///////////////////////////

			case 'x':
			    uart1Print("\nExiting Mixer CLI....\n\n");
			    cliBusy = false;
			    return;
			    break;

            ///////////////////////////

            case 'A': // Read Mixer Configuration
                eepromConfig.mixerConfiguration = (uint8_t)readFloatUart1();
                initMixer();

        	    mixerQuery = 'a';
                validQuery = true;
		        break;

            ///////////////////////////

            case 'B': // Read ESC and Servo PWM Update Rates
                eepromConfig.escPwmRate   = (uint16_t)readFloatUart1();
                eepromConfig.servoPwmRate = (uint16_t)readFloatUart1();

                pwmEscInit(eepromConfig.escPwmRate);
                pwmServoInit(eepromConfig.servoPwmRate);

                mixerQuery = 'a';
                validQuery = true;
        	    break;

            ///////////////////////////

            case 'C': // Read BiCopter Left Servo Parameters
           	    eepromConfig.biLeftServoMin = readFloatUart1();
           	    eepromConfig.biLeftServoMid = readFloatUart1();
           	    eepromConfig.biLeftServoMax = readFloatUart1();

           	    mixerQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'D': // Read BiCopter Right Servo Parameters
           	    eepromConfig.biRightServoMin = readFloatUart1();
           	    eepromConfig.biRightServoMid = readFloatUart1();
           	    eepromConfig.biRightServoMax = readFloatUart1();

           	    mixerQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'E': // Read Flying Wing Servo Directions
                eepromConfig.rollDirectionLeft   = readFloatUart1();
                eepromConfig.rollDirectionRight  = readFloatUart1();
                eepromConfig.pitchDirectionLeft  = readFloatUart1();
                eepromConfig.pitchDirectionRight = readFloatUart1();

         	    mixerQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'F': // Read Flying Wing Servo Limits
           	    eepromConfig.wingLeftMinimum  = readFloatUart1();
           	    eepromConfig.wingLeftMaximum  = readFloatUart1();
           	    eepromConfig.wingRightMinimum = readFloatUart1();
           	    eepromConfig.wingRightMaximum = readFloatUart1();

                mixerQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'G': // Read Free Mix Motor Number
           	    eepromConfig.freeMixMotors = (uint8_t)readFloatUart1();
           	    initMixer();

           	    mixerQuery = 'b';
                validQuery = true;
                break;

            ///////////////////////////

            case 'H': // Read Free Mix Matrix Element
                rows    = (uint8_t)readFloatUart1();
                columns = (uint8_t)readFloatUart1();
                eepromConfig.freeMix[rows][columns] = readFloatUart1();

                mixerQuery = 'b';
                validQuery = true;
                break;

            ///////////////////////////

            case 'I': // Read Gimbal Roll Servo Parameters
         	    eepromConfig.gimbalRollServoMin  = readFloatUart1();
           	    eepromConfig.gimbalRollServoMid  = readFloatUart1();
           	    eepromConfig.gimbalRollServoMax  = readFloatUart1();
           	    eepromConfig.gimbalRollServoGain = readFloatUart1();

           	    mixerQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'J': // Read Gimbal Pitch Servo Parameters
           	    eepromConfig.gimbalPitchServoMin  = readFloatUart1();
           	    eepromConfig.gimbalPitchServoMid  = readFloatUart1();
           	    eepromConfig.gimbalPitchServoMax  = readFloatUart1();
           	    eepromConfig.gimbalPitchServoGain = readFloatUart1();

           	    mixerQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'K': // Read TriCopter YawServo Parameters
        	    eepromConfig.triYawServoMin = readFloatUart1();
           	    eepromConfig.triYawServoMid = readFloatUart1();
           	    eepromConfig.triYawServoMax = readFloatUart1();

           	    mixerQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'L': // Read V Tail Angle
        	    eepromConfig.vTailAngle = readFloatUart1();

        	    mixerQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'M': // Read yaw direction
                tempFloat = readFloatUart1();
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
                uart1Print("\nWriting EEPROM Parameters....\n\n");
                writeEEPROM();
                break;

			///////////////////////////

			case '?':
			   	uart1Print("\n");
			   	uart1Print("'a' Mixer Configuration Data               'A' Set Mixer Configuration              A1 thru 21, see aq32Plus.h\n");
   		        uart1Print("'b' Free Mixer Configuration               'B' Set PWM Rates                        BESC;Servo\n");
			   	uart1Print("                                           'C' Set BiCopter Left Servo Parameters   CMin;Mid;Max\n");
			   	uart1Print("                                           'D' Set BiCopter Right Servo Parameters  DMin;Mid;Max\n");
			   	uart1Print("                                           'E' Set Flying Wing Servo Directions     ERollLeft;RollRight;PitchLeft;PitchRight\n");
			   	uart1Print("                                           'F' Set Flying Wing Servo Limits         FLeftMin;LeftMax;RightMin;RightMax\n");
   		        uart1Print("                                           'G' Set Number of FreeMix Motors         GNumber\n");
   		        uart1Print("                                           'H' Set FreeMix Matrix Element           HRow;Column;Element\n");
   		        uart1Print("                                           'I' Set Gimbal Roll Servo Parameters     IMin;Mid;Max;Gain\n");
   		        uart1Print("                                           'J' Set Gimbal Pitch Servo Parameters    JMin;Mid;Max;Gain\n");
   		        uart1Print("                                           'K' Set TriCopter Servo Parameters       KMin;Mid;Max\n");
   		        uart1Print("                                           'L' Set V Tail Angle                     LAngle\n");
   		        uart1Print("                                           'M' Set Yaw Direction                    M1 or M-1\n");
   		        uart1Print("                                           'W' Write EEPROM Parameters\n");
   		        uart1Print("'x' Exit Sensor CLI                        '?' Command Summary\n");
   		        uart1Print("\n");
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

    uart1Print("\nEntering Receiver CLI....\n\n");

    while(true)
    {
        uart1Print("Receiver CLI -> ");

		while ((uart1Available() == false) && (validQuery == false));

		if (validQuery == false)
		    receiverQuery = uart1Read();

		uart1Print("\n");

		switch(receiverQuery)
		{
            ///////////////////////////

            case 'a': // Receiver Configuration
                uart1Print("\nReceiver Type:                  ");
                switch(eepromConfig.receiverType)
                {
                    case PARALLEL_PWM:
                        uart1Print("Parallel\n");
                        break;
                    case SERIAL_PWM:
                        uart1Print("Serial\n");
                        break;
                    case SPEKTRUM:
                        uart1Print("Spektrum\n");
                        break;
		        }

                uart1Print("Current RC Channel Assignment:  ");
                for (index = 0; index < 8; index++)
                    rcOrderString[eepromConfig.rcMap[index]] = rcChannelLetters[index];

                rcOrderString[index] = '\0';

                uart1Print(rcOrderString);  uart1Print("\n");

                uart1Print("Spektrum Resolution:            ");
                if (eepromConfig.spektrumHires)
				    uart1Print("11 Bit Mode\n");
				else
				    uart1Print("10 Bit Mode\n");

				uart1Print("Number of Spektrum Channels:    ");
				snprintf(numberString, 16, "%d\n", eepromConfig.spektrumChannels); uart1Print(numberString);

                uart1Print("Mid Command:                    ");
                snprintf(numberString, 16, "%d\n", (uint16_t)eepromConfig.midCommand); uart1Print(numberString);

				uart1Print("Min Check:                      ");
                snprintf(numberString, 16, "%d\n", (uint16_t)eepromConfig.minCheck); uart1Print(numberString);

				uart1Print("Max Check:                      ");
                snprintf(numberString, 16, "%d\n", (uint16_t)eepromConfig.maxCheck); uart1Print(numberString);

				uart1Print("Min Throttle:                   ");
                snprintf(numberString, 16, "%d\n", (uint16_t)eepromConfig.minThrottle); uart1Print(numberString);

				uart1Print("Max Thottle:                    ");
                snprintf(numberString, 16, "%d\n\n", (uint16_t)eepromConfig.maxThrottle); uart1Print(numberString);

				uart1Print("Max Rate Command:               ");
				tempFloat = eepromConfig.rateScaling * 180000.0 / PI;
				snprintf(numberString, 16, "%6.2f DPS\n", tempFloat); uart1Print(numberString);

				uart1Print("Max Attitude Command:           ");
				tempFloat = eepromConfig.attitudeScaling * 180000.0 / PI;
				snprintf(numberString, 18, "%6.2f Degrees\n\n", tempFloat); uart1Print(numberString);

				validQuery = false;
                break;

            ///////////////////////////

            case 'b': // Read Max Rate Value
                eepromConfig.rateScaling = readFloatUart1() / 180000 * PI;

                receiverQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'c': // Read Max Attitude Value
                eepromConfig.attitudeScaling = readFloatUart1() / 180000 * PI;

                receiverQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

			case 'x':
			    uart1Print("\nExiting Receiver CLI....\n\n");
			    cliBusy = false;
			    return;
			    break;

            ///////////////////////////

            case 'A': // Read RX Input Type
                eepromConfig.receiverType = (uint8_t)readFloatUart1();
			    uart1Print( "\nReceiver Type Changed....\n");

			    uart1Print("\nSystem Resetting....\n");
			    delay(100);
			    writeEEPROM();
			    systemReset(false);

		        break;

            ///////////////////////////

            case 'B': // Read RC Control Order
                readStringUart1( rcOrderString, 8 );
                parseRcChannels( rcOrderString );

          	    receiverQuery = 'a';
                validQuery = true;
        	    break;

            ///////////////////////////

            case 'C': // Read Spektrum Resolution
                eepromConfig.spektrumHires = (uint8_t)readFloatUart1();

                receiverQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'D': // Read Number of Spektrum Channels
                eepromConfig.spektrumChannels = (uint8_t)readFloatUart1();

                receiverQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'E': // Read RC Control Points
                eepromConfig.midCommand   = readFloatUart1();
    	        eepromConfig.minCheck     = readFloatUart1();
    		    eepromConfig.maxCheck     = readFloatUart1();
    		    eepromConfig.minThrottle  = readFloatUart1();
    		    eepromConfig.maxThrottle  = readFloatUart1();

                receiverQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'W': // Write EEPROM Parameters
                uart1Print("\nWriting EEPROM Parameters....\n\n");
                writeEEPROM();
                break;

			///////////////////////////

			case '?':
			   	uart1Print("\n");
			   	uart1Print("'a' Receiver Configuration Data            'A' Set RX Input Type                    AX, 1=Parallel, 2=Serial, 3=Spektrum\n");
   		        uart1Print("'b' Set Maximum Rate Command               'B' Set RC Control Order                 BTAER1234\n");
			   	uart1Print("'c' Set Maximum Attitude Command           'C' Set Spektrum Resolution              C0 or C1\n");
			   	uart1Print("                                           'D' Set Number of Spektrum Channels      D6 thru D12\n");
			   	uart1Print("                                           'E' Set RC Control Points                EmidCmd;minChk;maxChk;minThrot;maxThrot\n");
			   	uart1Print("                                           'W' Write EEPROM Parameters\n");
			   	uart1Print("'x' Exit Receiver CLI                      '?' Command Summary\n");
			   	uart1Print("\n");
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

    uart1Print("\nEntering Sensor CLI....\n\n");

    while(true)
    {
        uart1Print("Sensor CLI -> ");

		while ((uart1Available() == false) && (validQuery == false));

		if (validQuery == false)
		    sensorQuery = uart1Read();

		uart1Print("\n");

		switch(sensorQuery)
		{
            ///////////////////////////

            case 'a': // Sensor Data
                uart1Print("\n");

                uart1Print("Accel One G:               ");
                snprintf(numberString, 16, "%9.4f\n", accelOneG); uart1Print(numberString);

                uart1Print("Mag Bias:                  ");
                snprintf(numberString, 16, "%9.4f, ", eepromConfig.magBias[XAXIS]); uart1Print(numberString);
                snprintf(numberString, 16, "%9.4f, ", eepromConfig.magBias[YAXIS]); uart1Print(numberString);
                snprintf(numberString, 16, "%9.4f\n", eepromConfig.magBias[ZAXIS]); uart1Print(numberString);

                uart1Print("Accel Cutoff:              ");
                snprintf(numberString, 16, "%9.4f\n", eepromConfig.accelCutoff); uart1Print(numberString);

                uart1Print("KpAcc (MARG):              ");
                snprintf(numberString, 16, "%9.4f\n", eepromConfig.KpAcc); uart1Print(numberString);

                uart1Print("KiAcc (MARG):              ");
                snprintf(numberString, 16, "%9.4f\n", eepromConfig.KiAcc); uart1Print(numberString);

                uart1Print("KpMag (MARG):              ");
                snprintf(numberString, 16, "%9.4f\n", eepromConfig.KpMag); uart1Print(numberString);

                uart1Print("KiMag (MARG):              ");
                snprintf(numberString, 16, "%9.4f\n", eepromConfig.KiMag); uart1Print(numberString);

                uart1Print("hdot est/h est Comp Fil A: ");
                snprintf(numberString, 16, "%9.4f\n", eepromConfig.compFilterA); uart1Print(numberString);

                uart1Print("hdot est/h est Comp Fil B: ");
                snprintf(numberString, 16, "%9.4f\n\n", eepromConfig.compFilterB); uart1Print(numberString);

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
			    uart1Print("\nExiting Sensor CLI....\n\n");
			    cliBusy = false;
			    return;
			    break;

            ///////////////////////////

            case 'B': // Accel Cutoff
                eepromConfig.accelCutoff = readFloatUart1();

                sensorQuery = 'a';
                validQuery = true;
        	    break;

            ///////////////////////////

            case 'C': // kpAcc, kiAcc
                eepromConfig.KpAcc = readFloatUart1();
                eepromConfig.KiAcc = readFloatUart1();

                sensorQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'D': // kpMag, kiMag
                eepromConfig.KpMag = readFloatUart1();
                eepromConfig.KiMag = readFloatUart1();

                sensorQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'E': // h dot est/h est Comp Filter A/B
                eepromConfig.compFilterA = readFloatUart1();
                eepromConfig.compFilterB = readFloatUart1();

                sensorQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'M': // Magnetic Variation
                eepromConfig.magVar = readFloatUart1() * D2R;

                sensorQuery = 'a';
                validQuery = true;
                break;

            ///////////////////////////

            case 'W': // Write EEPROM Parameters
                uart1Print("\nWriting EEPROM Parameters....\n\n");
                writeEEPROM();
                break;

			///////////////////////////

			case '?':
			   	uart1Print("\n");
			   	uart1Print("'a' Display Sensor Data\n");
			   	uart1Print("'b' Accel Calibration                      'B' Set Accel Cutoff                     BAccelCutoff\n");
			   	uart1Print("'c' Magnetometer Calibration               'C' Set kpAcc/kiAcc                      CKpAcc;KiAcc\n");
			   	uart1Print("                                           'D' Set kpMag/kiMag                      DKpMag;KiMag\n");
			   	uart1Print("                                           'E' Set h dot est/h est Comp Filter A/B  EA;B\n");
			   	uart1Print("                                           'W' Write EEPROM Parameters\n");
			   	uart1Print("'x' Exit Sensor CLI                        '?' Command Summary\n");
			    uart1Print("\n");
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

    uart1Print("\nEntering GPS CLI....\n\n");

    while(true)
    {
        uart1Print("GPS CLI -> ");

		while ((uart1Available() == false) && (validQuery == false));

		if (validQuery == false)
		    gpsQuery = uart1Read();

		uart1Print("\n");

		switch(gpsQuery)
		{
            ///////////////////////////

            case 'a': // GPS Installation Data
                uart1Print("\n");

				switch(eepromConfig.gpsType)
				{
					///////////////

					case NO_GPS:
					    uart1Print("No GPS Installed....\n\n");
					    break;

					///////////////

					case MEDIATEK_3329_BINARY:
					    uart1Print("MediaTek 3329 GPS installed, Binary Mode....\n\n");
					    break;

					///////////////

					case MEDIATEK_3329_NMEA:
					    uart1Print("MediaTek 3329 GPS Installed, NMEA Mode....\n\n");
					    break;

					///////////////

					case UBLOX:
					    uart1Print("UBLOX GPS Installed, Binary Mode....\n\n");
					    break;

					///////////////
				}

                validQuery = false;
                break;

            ///////////////////////////

			case 'x':
			    uart1Print("\nExiting GPS CLI....\n\n");
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
                uart1Print("\nWriting EEPROM Parameters....\n\n");
                writeEEPROM();
                break;

			///////////////////////////

			case '?':
			   	uart1Print("\n");
			   	uart1Print("'a' Display GPS Installation Data          'A' Set GPS Type to No GPS\n");
			   	uart1Print("                                           'B' Set GPS Type to MediaTek 3329 Binary\n");
			   	uart1Print("                                           'C' Set GPS Type to MediaTek 3329 NMEA\n");
			   	uart1Print("                                           'D' Set GPS Type to UBLOX\n");
			   	uart1Print("                                           'W' Write EEPROM Parameters\n");
			   	uart1Print("'x' Exit GPS CLI                           '?' Command Summary\n");
			    uart1Print("\n");
	    	    break;

	    	///////////////////////////
	    }
	}

}

///////////////////////////////////////////////////////////////////////////////
