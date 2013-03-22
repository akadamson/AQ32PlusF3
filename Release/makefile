################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := cs-rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/startup/subdir.mk
-include src/sensors/subdir.mk
-include src/max7456/subdir.mk
-include src/gps/subdir.mk
-include src/drv/subdir.mk
-include src/calibration/subdir.mk
-include src/subdir.mk
-include Libraries/STM32F30x_StdPeriph_Driver/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
aq32Plus_F3.hex \

SECONDARY_LIST += \
aq32Plus_F3.lst \

SECONDARY_SIZE += \
aq32Plus_F3.siz \


# All Target
all: aq32Plus_F3.elf secondary-outputs

# Tool invocations
aq32Plus_F3.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Sourcery Windows GCC C Linker'
	arm-none-eabi-gcc -T"D:\Documents and Settings\e722209\Desktop\workspace\aq32Plus_F3\stm32_flash.ld" -Xlinker --gc-sections -Wl,-Map,aq32Plus_F3.map -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -g3 -gdwarf-2 -o "aq32Plus_F3.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

aq32Plus_F3.hex: aq32Plus_F3.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex aq32Plus_F3.elf  "aq32Plus_F3.hex"
	@echo 'Finished building: $@'
	@echo ' '

aq32Plus_F3.lst: aq32Plus_F3.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Create Listing'
	arm-none-eabi-objdump -h -S aq32Plus_F3.elf > "aq32Plus_F3.lst"
	@echo 'Finished building: $@'
	@echo ' '

aq32Plus_F3.siz: aq32Plus_F3.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Print Size'
	arm-none-eabi-size  --format=berkeley aq32Plus_F3.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(EXECUTABLES)$(SECONDARY_LIST)$(S_UPPER_DEPS) aq32Plus_F3.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
