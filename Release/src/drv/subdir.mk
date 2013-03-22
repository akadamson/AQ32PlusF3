################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/drv/drv_gps.c \
../src/drv/drv_i2c.c \
../src/drv/drv_led.c \
../src/drv/drv_max7456.c \
../src/drv/drv_pwmEsc.c \
../src/drv/drv_pwmServo.c \
../src/drv/drv_rx.c \
../src/drv/drv_spi.c \
../src/drv/drv_system.c \
../src/drv/drv_telemetry.c \
../src/drv/drv_timingFunctions.c \
../src/drv/drv_uart1.c 

OBJS += \
./src/drv/drv_gps.o \
./src/drv/drv_i2c.o \
./src/drv/drv_led.o \
./src/drv/drv_max7456.o \
./src/drv/drv_pwmEsc.o \
./src/drv/drv_pwmServo.o \
./src/drv/drv_rx.o \
./src/drv/drv_spi.o \
./src/drv/drv_system.o \
./src/drv/drv_telemetry.o \
./src/drv/drv_timingFunctions.o \
./src/drv/drv_uart1.o 

C_DEPS += \
./src/drv/drv_gps.d \
./src/drv/drv_i2c.d \
./src/drv/drv_led.d \
./src/drv/drv_max7456.d \
./src/drv/drv_pwmEsc.d \
./src/drv/drv_pwmServo.d \
./src/drv/drv_rx.d \
./src/drv/drv_spi.d \
./src/drv/drv_system.d \
./src/drv/drv_telemetry.d \
./src/drv/drv_timingFunctions.d \
./src/drv/drv_uart1.d 


# Each subdirectory must supply rules for building sources it contributes
src/drv/%.o: ../src/drv/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DSTM32F30X -DUSE_STDPERIPH_DRIVER -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\src" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\Libraries\CMSIS\Include" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\Libraries\Device\STM32F30x\Include" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\Libraries\STM32F30x_StdPeriph_Driver\inc" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\src\calibration" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\src\drv" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\src\gps" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\src\max7456" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\src\sensors" -O0 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -fomit-frame-pointer -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


