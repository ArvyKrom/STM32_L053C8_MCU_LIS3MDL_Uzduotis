################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lis3mdl/lis3mdl.c 

OBJS += \
./Drivers/lis3mdl/lis3mdl.o 

C_DEPS += \
./Drivers/lis3mdl/lis3mdl.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lis3mdl/%.o Drivers/lis3mdl/%.su Drivers/lis3mdl/%.cyclo: ../Drivers/lis3mdl/%.c Drivers/lis3mdl/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L053xx -c -I"C:/Users/arvyd/STM32CubeIDE/workspace_1.17.0/STM32_L053C8_MCU_LIS3MDL_Uzduotis/Drivers/lis3mdl" -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-lis3mdl

clean-Drivers-2f-lis3mdl:
	-$(RM) ./Drivers/lis3mdl/lis3mdl.cyclo ./Drivers/lis3mdl/lis3mdl.d ./Drivers/lis3mdl/lis3mdl.o ./Drivers/lis3mdl/lis3mdl.su

.PHONY: clean-Drivers-2f-lis3mdl

