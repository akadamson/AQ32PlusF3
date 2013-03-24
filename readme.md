AQ32PlusF3
==============

See AQ32Plus project for STM32 Tool Archieve, containing compiler and ST Tools

Features of aq32Plus: (Needs updating for the F3 version)

1)Focused at the capabilities of the aq32/STM32F407 hardware
2)1 KHz sampling of gyro and accel, averaged to 500 Hz for use
3)Pressure data sampling at 100 Hz, averged to 10 Hz for use
4)Mag data sampling at 50 Hz
5)Interrupt driven system timing, 1 KHz max rate for sensor reads
6)500Hz, 200Hz, 100Hz, 50Hz, 10 Hz, 5 Hz, and 1 Hz task frames.  Not quite a RTOS.
7)1 to 8 ESC PWM outputs, adjustable output rate, default 450 Hz, 0.5 uSec resolution
8)1 to 3 Servo PWM outputs, adjustable output rate, default 50 Hz, 0.5 uSec resolution
9)Parallel PWM (default), serial PWM, and Spektrum RX support(binding with RX too) **
10)MARG attitude estimator with functioning accel cutoff
11)Attitude estimation and flight control inner loops running at 500 Hz
12)Delta times for integration functions computed via hardware timers at 0.5 uSec resolution
13)Wide range of mixers:
    Gimbal
    Flying Wing
    BiCopter
    TriCopter
    QuadP, QuadX, VTail(6 different schemes), Y4
    Hex6P, Hex6X, Y6
    OctoF8P, OctoF8X, OctoX8P, OctoX8X
    FreeMix (Up to 8 motors/Surfaces)
14)Drivers for I2C 1 and 2, SPI 1,2, and 3, uart1(RF telem), uart2(GPS)
15)Setup via command line interface (CLI), no configurator interface at this time
16)Limited high speed (100 Hz) RF telemetry for sensor evaluation and PID tuning
17)Built with the Eclipse/Code Sourcery/STM Standard Peripherial Library tools
18)Programmed thru USB port via STM DFUSE demo program
19)Not Arduino compatible (sorry)
20)Whatever I forgot to add here......

** PWM at 0.5 uSec resolution, both low res and hi res Spektrum data

Bench Tested Features of aq32Plus:

1)Parallel PWM Inputs
2)Spektrum Satellite Input and Binding, 8 channel low res mode
3)Quad X Calculations
4)CLI Setup
5)RF Telem
6)MediaTek GPS, binary and NMEA modes

Flight Tested Features of aq32Plus:

1)Spektrum Satellite Input and Binding, 8 channel low res mode
2)Quad X
3)CLI Setup
4)RF Telem

Planned Additions (in no particular order):

1)Vertical VelocityHold/Altitude Hold
2)Velocity Hold
3)Position Hold
4)Battery Monitor
5)SD Card Read/Write
6)AGL Sensor
7)Optical Flow velocity/position aiding
8)Futaba SBUS
9)Support more than 8 RX channels
10)OSD
11)Second Spektrum Satellite
12)PID autotuning

There's a lot of stuff I could on here with, but enough typing for now.  I tried to keep
the code as readable as possible.  I'm sure there is a lot of room for improvement.

As of this writing I've done hovering tests in my backyard.  I think it's the best handling 
code I flown in my limited testing.  Some of the guys I conferred with, who are far better 
pilots than I am, have been able to fly circles around themselves without seeing the leans
 problem.  

My test configuration is the AeroQuad Typhoon frame, with the same motors and ESC supplied
in the ARF kit.  I have flashed the HobbyWing ESCs with Revision 9 of the BLHeli multirotor
code.  I'm using a Spektrum satellite for the RX, and will install the xBee telem radio shortly.

This a major work in progress, looking at what I feel are the best parts of many of the 
open source projects out there, melding that together in the STM32 world, along with some
different ways of doing other things, and seeing where it takes me.

Special thanks to Greg Egan (from the UAVX program) down in Australia for his help with 
the accel cutoff.  Many others had their hand in this too, ala42 for helping me figure out 
how to get the FPU running, RobertB for sharing ideas about different PID structures, and 
TimeCop for introducing the Naze32, which enabled me to finally figure out this STM32 stuff.
Most of the low level drivers in aq32Plus are based on his Naze32/BaseFlight work.  I also
have a subset of this code that does run on the Naze32, but it's a little behind in some of
the attitude estimator and PID enhacements at the moment.

Still a lot to learn about the STM32, and what we can do with it.  More to come.....

v0.1 - Initial Release

