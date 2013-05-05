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

#pragma once

///////////////////////////////////////////////////////////////////////////////

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

///////////////////////////////////////////////////////////////////////////////

#include "stm32f30x.h"
#include "stm32f30x_conf.h"

#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

///////////////////////////////////////////////////////////////////////////////

#include "pid.h"

#include "aq32PlusF3.h"

#include "drv_adc.h"
#include "drv_cli.h"
#include "drv_gps.h"
#include "drv_i2c.h"
#include "drv_led.h"
#include "drv_max7456.h"
#include "drv_pwmESC.h"
#include "drv_pwmServo.h"
#include "drv_rx.h"
#include "drv_spi.h"
#include "drv_system.h"
#include "drv_telemetry.h"
#include "drv_timingFunctions.h"

#include "l3gd20.h"
#include "lsm303dlhc.h"
#include "ms5611_SPI.h"

#include "accelCalibration.h"
#include "cli.h"
#include "cliSupport.h"
#include "computeAxisCommands.h"
#include "config.h"
#include "coordinateTransforms.h"
#include "escCalibration.h"
#include "flightCommand.h"
#include "gps.h"
#include "gpsMediaTek19.h"
#include "gpsNMEA.h"
#include "gpsUblox.h"
#include "linearAlgebra.h"
#include "lowPassFilter.h"
#include "MargAHRS.h"
#include "magCalibration.h"
#include "mixer.h"
#include "osdWidgets.h"
#include "rfTelem.h"
#include "utilities.h"
#include "vertCompFilter.h"

///////////////////////////////////////////////////////////////////////////////
