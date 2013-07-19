
7/18/2013
-Cleanup drv_pwmEsc.c to support 6 ESCs
-Corrected TIM4 typo in serial pwm rx init to TIM1
-Commited merge of VCP branch back onto Master branch
-Added setup folder with copy of .cproject
-Added .cproject to .gitignore

7/17/2013
-Added CMSIS 3.2
-VCP fully functional

6/7/2013
-Added DSP Library call for matrix math function in cordinateTransforms.c
-Deleted 4th order lowpass filter, added firstOrderFilter
-Updates to MS5611 sensor to match AQ32Plus procesing
-Move MS5611 calls from systick ISR to 100 Hz loop

5/22/2013
-Changed serial pwm timer 1 channel 4 to timer 1 channel 1 in drv_rx.c
-VCP transmit works, VCP receive still needs work

4/29/2013
-Fixed alternate function definition errors in drv_rx.c
-Fixed alternate function definition errors in drv_pwmEsc.c
-Reworked drv_adc to convert battery voltage and differential pressure inputs on ADC1
-Reworked cli.c to display battery voltage and differential pressure with 'z' command

AQ32PlusF3
==============

See AQ32Plus project for STM32 Tool Archieve, containing compiler and ST Tools

