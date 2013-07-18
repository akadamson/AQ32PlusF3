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

// Cycle counter stuff - these should be defined by CMSIS, but they aren't
#define DWT_CTRL    (*(volatile uint32_t *)0xE0001000)
#define DWT_CYCCNT  ((volatile uint32_t *)0xE0001004)
#define CYCCNTENA   (1 << 0)

///////////////////////////////////////////////////////////////////////////////

// Cycles per microsecond
static volatile uint32_t usTicks = 0;

///////////////////////////////////////////////////////////////////////////////

// Current uptime for 1kHz systick timer. will rollover after 49 days.
// Hopefully we won't care.
static volatile uint32_t sysTickUptime = 0;
static volatile uint32_t sysTickCycleCounter = 0;

///////////////////////////////////////////////////////////////////////////////
// Cycle Counter
///////////////////////////////////////////////////////////////////////////////

static void cycleCounterInit(void)
{
    RCC_ClocksTypeDef clocks;
    RCC_GetClocksFreq(&clocks);
    usTicks = clocks.SYSCLK_Frequency / 1000000;

    // enable DWT access
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    // enable the CPU cycle counter
    DWT_CTRL |= CYCCNTENA;
}

///////////////////////////////////////
// Frame Timing Variables
///////////////////////////////////////

uint16_t frameCounter = 0;

uint8_t frame_500Hz = false;
uint8_t frame_100Hz = false;
uint8_t frame_50Hz  = false;
uint8_t frame_10Hz  = false;
uint8_t frame_5Hz   = false;
uint8_t frame_1Hz   = false;

uint32_t deltaTime1000Hz, executionTime1000Hz, previous1000HzTime;
uint32_t deltaTime500Hz,  executionTime500Hz,  previous500HzTime;
uint32_t deltaTime100Hz,  executionTime100Hz,  previous100HzTime;
uint32_t deltaTime50Hz,   executionTime50Hz,   previous50HzTime;
uint32_t deltaTime10Hz,   executionTime10Hz,   previous10HzTime;
uint32_t deltaTime5Hz,    executionTime5Hz,    previous5HzTime;
uint32_t deltaTime1Hz,    executionTime1Hz,    previous1HzTime;

float dt500Hz, dt100Hz;

uint8_t systemReady = false;

uint8_t execUp = false;

///////////////////////////////////////////////////////////////////////////////
// SysTick
///////////////////////////////////////////////////////////////////////////////

void SysTick_Handler(void)
{
    uint8_t index;
    uint32_t currentTime;

    sysTickCycleCounter = *DWT_CYCCNT;
    sysTickUptime++;

    if ((systemReady       == true)  &&
        (cliBusy           == false) &&
        (accelCalibrating  == false) &&
        (escCalibrating    == false) &&
        (gyroCalibrating   == false) &&
        (magCalibrating    == false))

    {
    	frameCounter++;
        if (frameCounter > FRAME_COUNT)
            frameCounter = 1;

        ///////////////////////////////

        currentTime = micros();
        deltaTime1000Hz = currentTime - previous1000HzTime;
        previous1000HzTime = currentTime;

        readAccel();
        readGyro();

        accelSum500Hz[XAXIS] += rawAccel[XAXIS].value;
        accelSum500Hz[YAXIS] += rawAccel[YAXIS].value;
        accelSum500Hz[ZAXIS] += rawAccel[ZAXIS].value;

        accelSum100Hz[XAXIS] += rawAccel[XAXIS].value;
        accelSum100Hz[YAXIS] += rawAccel[YAXIS].value;
        accelSum100Hz[ZAXIS] += rawAccel[ZAXIS].value;

        gyroSum500Hz[ROLL ] += rawGyro[ROLL ].value;
        gyroSum500Hz[PITCH] += rawGyro[PITCH].value;
        gyroSum500Hz[YAW  ] += rawGyro[YAW  ].value;

        ///////////////////////////////

        if ((frameCounter % COUNT_500HZ) == 0)
        {
        	frame_500Hz = true;

            for (index = 0; index < 3; index++)
            {
            	accelSummedSamples500Hz[index] = accelSum500Hz[index];
            	accelSum500Hz[index] = 0.0f;

            	gyroSummedSamples500Hz[index] = gyroSum500Hz[index];
                gyroSum500Hz[index] = 0.0f;
            }
        }
        ///////////////////////////////

        if ((frameCounter % COUNT_100HZ) == 0)
        {
        	frame_100Hz = true;

            for (index = 0; index < 3; index++)
            {
                accelSummedSamples100Hz[index] = accelSum100Hz[index];
                accelSum100Hz[index] = 0.0f;
            }
        }

        ///////////////////////////////

        if ((frameCounter % COUNT_50HZ) == 0)
        {
        	frame_50Hz = true;
        }

        ///////////////////////////////

        if (((frameCounter + 1) % COUNT_10HZ) == 0)
            newMagData = readMag();

        if ((frameCounter % COUNT_10HZ) == 0)
        {
        	frame_10Hz = true;
        }

        ///////////////////////////////

        if ((frameCounter % COUNT_5HZ) == 0)
        {
        	frame_5Hz = true;
        }

        ///////////////////////////////

        if ((frameCounter % COUNT_1HZ) == 0)
        {
        	frame_1Hz = true;
        }

        ///////////////////////////////////

        executionTime1000Hz = micros() - currentTime;

        ///////////////////////////////
    }
}

///////////////////////////////////////////////////////////////////////////////
// System Time in Microseconds
//
// Note: This can be called from within IRQ Handlers, so uses LDREX/STREX.
// If a higher priority IRQ or DMA or anything happens the STREX will fail
// and restart the loop. Otherwise the same number that was read is harmlessly
// written back.
///////////////////////////////////////////////////////////////////////////////

uint32_t micros(void)
{
    register uint32_t oldCycle, cycle, timeMs;

    do
    {
        timeMs = __LDREXW(&sysTickUptime);
        cycle = *DWT_CYCCNT;
        oldCycle = sysTickCycleCounter;
    }
    while ( __STREXW( timeMs , &sysTickUptime ) );

    return (timeMs * 1000) + (cycle - oldCycle) / usTicks;
}

///////////////////////////////////////////////////////////////////////////////
// System Time in Milliseconds
///////////////////////////////////////////////////////////////////////////////

uint32_t millis(void)
{
    return sysTickUptime;
}

///////////////////////////////////////////////////////////////////////////////
// System Initialization
///////////////////////////////////////////////////////////////////////////////

void systemInit(void)
{
	// Init cycle counter
    cycleCounterInit();

    // SysTick
    SysTick_Config(SystemCoreClock / 1000);

    checkFirstTime(false);
	readEEPROM();

	if (eepromConfig.receiverType == SPEKTRUM)
		checkSpektrumBind();

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 2 bits for pre-emption priority, 2 bits for subpriority

	initMixer();

    ledInit();

    adcInit();
    cliInit();
    gpsInit();
    i2cInit(I2C1);
    pwmEscInit(eepromConfig.escPwmRate);
    pwmServoInit(eepromConfig.servoPwmRate);
    pwmServoInit(eepromConfig.servoPwmRate);
    rxInit();
    spiInit(SPI1);
    spiInit(SPI2);
    spiInit(SPI3);
    telemetryInit();
    timingFunctionsInit();

    initPID();
    initFirstOrderFilter();

    RED_N_LED_ON;
    delay(2500);
    ORANGE_NE_LED_ON;
    delay(2500);
    GREEN_E_LED_ON;
    delay(2500);

    initGPS();

    BLUE_SE_LED_ON;
    delay(2500);
    RED_S_LED_ON;
    delay(2500);
    ORANGE_SW_LED_ON;
    delay(2500);
    GREEN_W_LED_ON;
    delay(2500);
    BLUE_NW_LED_ON;

    initAccel();
    initGyro();
    initMag();
    initPressure();

    initMax7456();
}

///////////////////////////////////////////////////////////////////////////////
// Delay Microseconds
///////////////////////////////////////////////////////////////////////////////

void delayMicroseconds(uint32_t us)
{
    uint32_t elapsed = 0;
    uint32_t lastCount = *DWT_CYCCNT;

    for (;;) {
        register uint32_t current_count = *DWT_CYCCNT;
        uint32_t elapsed_us;

        // measure the time elapsed since the last time we checked
        elapsed += current_count - lastCount;
        lastCount = current_count;

        // convert to microseconds
        elapsed_us = elapsed / usTicks;
        if (elapsed_us >= us)
            break;

        // reduce the delay by the elapsed time
        us -= elapsed_us;

        // keep fractional microseconds for the next iteration
        elapsed %= usTicks;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Delay Milliseconds
///////////////////////////////////////////////////////////////////////////////

void delay(uint32_t ms)
{
    while (ms--)
        delayMicroseconds(1000);
}

///////////////////////////////////////////////////////////////////////////////
// System Reset
///////////////////////////////////////////////////////////////////////////////

#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)

void systemReset(bool toBootloader)
{
    if (toBootloader)
    {
        // 1FFFF000 -> 20000200 -> SP
        // 1FFFF004 -> 1FFFF021 -> PC
        *((uint32_t *)0x20009FFC) = 0xDEADBEEF; // 40KB SRAM STM32F30X
    }

    // Generate system reset
    SCB->AIRCR = AIRCR_VECTKEY_MASK | (uint32_t) 0x04;
}

///////////////////////////////////////////////////////////////////////////////
