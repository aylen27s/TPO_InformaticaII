################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/cr_cpp_config.cpp \
../src/cr_startup_lpc84x.cpp \
../src/main.cpp 

C_SRCS += \
../src/crp.c \
../src/mtb.c 

S_SRCS += \
../src/aeabi_romdiv_patch.s 

CPP_DEPS += \
./src/cr_cpp_config.d \
./src/cr_startup_lpc84x.d \
./src/main.d 

C_DEPS += \
./src/crp.d \
./src/mtb.d 

OBJS += \
./src/aeabi_romdiv_patch.o \
./src/cr_cpp_config.o \
./src/cr_startup_lpc84x.o \
./src/crp.o \
./src/main.o \
./src/mtb.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.s src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -D__NEWLIB__ -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__USE_ROMDIVIDE -D__LPC84X__ -g3 -gdwarf-4 -mcpu=cortex-m0 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -DCPP_USE_HEAP -D__LPC84X__ -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/TIMER_DRIVER" -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/MAX30102_DRIVER" -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/I2C_DRIVER" -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/I2C_DRIVER" -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/GPIOF_DRIVER" -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/UART_DRIVER" -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/CALLBACK" -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/inc" -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/SYSTICK_DRIVER" -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/GPIO_DRIVER" -Os -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -DCPP_USE_HEAP -D__LPC84X__ -I"/home/guido/PROYECTO_LPC845BRK/uC_LPC845/src/I2C_DRIVER" -Os -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/aeabi_romdiv_patch.o ./src/cr_cpp_config.d ./src/cr_cpp_config.o ./src/cr_startup_lpc84x.d ./src/cr_startup_lpc84x.o ./src/crp.d ./src/crp.o ./src/main.d ./src/main.o ./src/mtb.d ./src/mtb.o

.PHONY: clean-src

