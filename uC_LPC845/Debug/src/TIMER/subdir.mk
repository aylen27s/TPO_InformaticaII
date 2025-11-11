################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/TIMER/TIMER.cpp 

CPP_DEPS += \
./src/TIMER/TIMER.d 

OBJS += \
./src/TIMER/TIMER.o 


# Each subdirectory must supply rules for building sources it contributes
src/TIMER/%.o: ../src/TIMER/%.cpp src/TIMER/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -DCPP_USE_HEAP -D__LPC84X__ -I"C:\Users\guido\Desktop\PROYECTO_LPC845BRK\uC_LPC845\inc" -I"C:\Users\guido\Desktop\PROYECTO_LPC845BRK\uC_LPC845\src\TIMER" -I"C:\Users\guido\Desktop\PROYECTO_LPC845BRK\uC_LPC845\src\CALLBACK" -I"C:\Users\guido\Desktop\PROYECTO_LPC845BRK\uC_LPC845\src\SYSTICK_DRIVER" -I"C:\Users\guido\Desktop\PROYECTO_LPC845BRK\uC_LPC845\src\GPIO_DRIVER" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-TIMER

clean-src-2f-TIMER:
	-$(RM) ./src/TIMER/TIMER.d ./src/TIMER/TIMER.o

.PHONY: clean-src-2f-TIMER

