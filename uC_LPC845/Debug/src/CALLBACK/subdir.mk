################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CALLBACK/CALLBACK.cpp 

CPP_DEPS += \
./src/CALLBACK/CALLBACK.d 

OBJS += \
./src/CALLBACK/CALLBACK.o 


# Each subdirectory must supply rules for building sources it contributes
src/CALLBACK/%.o: ../src/CALLBACK/%.cpp src/CALLBACK/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -DCPP_USE_HEAP -D__LPC84X__ -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/TIMER_DRIVER" -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/MAX30102_DRIVER" -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/I2C_DRIVER" -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/I2C_DRIVER" -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/GPIOF_DRIVER" -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/UART_DRIVER" -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/CALLBACK" -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/inc" -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/SYSTICK_DRIVER" -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/GPIO_DRIVER" -Os -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-CALLBACK

clean-src-2f-CALLBACK:
	-$(RM) ./src/CALLBACK/CALLBACK.d ./src/CALLBACK/CALLBACK.o

.PHONY: clean-src-2f-CALLBACK

