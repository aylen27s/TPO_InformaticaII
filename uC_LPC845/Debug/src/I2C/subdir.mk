################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/I2C/i2c.c \
../src/I2C/i2c_ver.c 

C_DEPS += \
./src/I2C/i2c.d \
./src/I2C/i2c_ver.d 

OBJS += \
./src/I2C/i2c.o \
./src/I2C/i2c_ver.o 


# Each subdirectory must supply rules for building sources it contributes
src/I2C/%.o: ../src/I2C/%.c src/I2C/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -DCPP_USE_HEAP -D__LPC84X__ -O0 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-I2C

clean-src-2f-I2C:
	-$(RM) ./src/I2C/i2c.d ./src/I2C/i2c.o ./src/I2C/i2c_ver.d ./src/I2C/i2c_ver.o

.PHONY: clean-src-2f-I2C

