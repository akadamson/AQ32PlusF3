################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/sensors/l3gd20.c \
../src/sensors/lsm303dlhc.c 

OBJS += \
./src/sensors/l3gd20.o \
./src/sensors/lsm303dlhc.o 

C_DEPS += \
./src/sensors/l3gd20.d \
./src/sensors/lsm303dlhc.d 


# Each subdirectory must supply rules for building sources it contributes
src/sensors/%.o: ../src/sensors/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DSTM32F30X -DUSE_STDPERIPH_DRIVER -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\src" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\Libraries\CMSIS\Include" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\Libraries\Device\STM32F30x\Include" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\Libraries\STM32F30x_StdPeriph_Driver\inc" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\src\calibration" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\src\drv" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\src\gps" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\src\max7456" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\src\sensors" -O0 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -fomit-frame-pointer -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


