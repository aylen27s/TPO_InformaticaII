################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/MAX30102/max30102.c 

C_DEPS += \
./src/MAX30102/max30102.d 

OBJS += \
./src/MAX30102/max30102.o 


# Each subdirectory must supply rules for building sources it contributes
src/MAX30102/%.o: ../src/MAX30102/%.c src/MAX30102/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -DCPP_USE_HEAP -D__LPC84X__ -O0 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-MAX30102

clean-src-2f-MAX30102:
	-$(RM) ./src/MAX30102/max30102.d ./src/MAX30102/max30102.o

.PHONY: clean-src-2f-MAX30102

