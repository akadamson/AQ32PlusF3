################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/MargAHRS.c \
../src/cli.c \
../src/cliSupport.c \
../src/computeAxisCommands.c \
../src/config.c \
../src/coordinateTransforms.c \
../src/flightCommand.c \
../src/linearAlgebra.c \
../src/lowPassFilter.c \
../src/main.c \
../src/mixer.c \
../src/pid.c \
../src/rfTelem.c \
../src/stm32f30x_it.c \
../src/system_stm32f30x.c \
../src/utilities.c \
../src/vertCompFilter.c 

OBJS += \
./src/MargAHRS.o \
./src/cli.o \
./src/cliSupport.o \
./src/computeAxisCommands.o \
./src/config.o \
./src/coordinateTransforms.o \
./src/flightCommand.o \
./src/linearAlgebra.o \
./src/lowPassFilter.o \
./src/main.o \
./src/mixer.o \
./src/pid.o \
./src/rfTelem.o \
./src/stm32f30x_it.o \
./src/system_stm32f30x.o \
./src/utilities.o \
./src/vertCompFilter.o 

C_DEPS += \
./src/MargAHRS.d \
./src/cli.d \
./src/cliSupport.d \
./src/computeAxisCommands.d \
./src/config.d \
./src/coordinateTransforms.d \
./src/flightCommand.d \
./src/linearAlgebra.d \
./src/lowPassFilter.d \
./src/main.d \
./src/mixer.d \
./src/pid.d \
./src/rfTelem.d \
./src/stm32f30x_it.d \
./src/system_stm32f30x.d \
./src/utilities.d \
./src/vertCompFilter.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DSTM32F30X -DUSE_STDPERIPH_DRIVER -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\src" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\Libraries\CMSIS\Include" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\Libraries\Device\STM32F30x\Include" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\Libraries\STM32F30x_StdPeriph_Driver\inc" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\src\calibration" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\src\drv" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\src\gps" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\src\max7456" -I"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\src\sensors" -O0 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -fomit-frame-pointer -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


