################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/lis3mdl/subdir.mk
-include Drivers/STM32L0xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := STM32_L053C8_MCU_LIS3MDL_Uzduotis
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
STM32_L053C8_MCU_LIS3MDL_Uzduotis.elf \

MAP_FILES += \
STM32_L053C8_MCU_LIS3MDL_Uzduotis.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
STM32_L053C8_MCU_LIS3MDL_Uzduotis.list \


# All Target
all: main-build

# Main-build Target
main-build: STM32_L053C8_MCU_LIS3MDL_Uzduotis.elf secondary-outputs

# Tool invocations
STM32_L053C8_MCU_LIS3MDL_Uzduotis.elf STM32_L053C8_MCU_LIS3MDL_Uzduotis.map: $(OBJS) $(USER_OBJS) C:\Users\arvyd\STM32CubeIDE\workspace_1.17.0\STM32_L053C8_MCU_LIS3MDL_Uzduotis\STM32L053C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "STM32_L053C8_MCU_LIS3MDL_Uzduotis.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0plus -T"C:\Users\arvyd\STM32CubeIDE\workspace_1.17.0\STM32_L053C8_MCU_LIS3MDL_Uzduotis\STM32L053C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="STM32_L053C8_MCU_LIS3MDL_Uzduotis.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

STM32_L053C8_MCU_LIS3MDL_Uzduotis.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "STM32_L053C8_MCU_LIS3MDL_Uzduotis.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) STM32_L053C8_MCU_LIS3MDL_Uzduotis.elf STM32_L053C8_MCU_LIS3MDL_Uzduotis.list STM32_L053C8_MCU_LIS3MDL_Uzduotis.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
