################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Driver/gpio.c \
../Core/Src/Driver/platform.c \
../Core/Src/Driver/uart.c 

OBJS += \
./Core/Src/Driver/gpio.o \
./Core/Src/Driver/platform.o \
./Core/Src/Driver/uart.o 

C_DEPS += \
./Core/Src/Driver/gpio.d \
./Core/Src/Driver/platform.d \
./Core/Src/Driver/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Driver/%.o Core/Src/Driver/%.su Core/Src/Driver/%.cyclo: ../Core/Src/Driver/%.c Core/Src/Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DF_CPU=16000000 -DSTM32F411xE -DUSE_FULL_LL_DRIVER -DHSE_VALUE=25000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DEXTERNAL_CLOCK_VALUE=12288000 -DHSI_VALUE=16000000 -DLSI_VALUE=32000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=1 -DDATA_CACHE_ENABLE=1 -c -I"D:/ST/CubeWorkspace/RookProject/Core/CMSIS/Device/ST/STM32F4xx/Include" -I"D:/ST/CubeWorkspace/RookProject/Core/CMSIS/Include" -I"D:/ST/CubeWorkspace/RookProject/Core/Inc" -I"D:/ST/CubeWorkspace/RookProject/Core/Inc/Driver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Driver

clean-Core-2f-Src-2f-Driver:
	-$(RM) ./Core/Src/Driver/gpio.cyclo ./Core/Src/Driver/gpio.d ./Core/Src/Driver/gpio.o ./Core/Src/Driver/gpio.su ./Core/Src/Driver/platform.cyclo ./Core/Src/Driver/platform.d ./Core/Src/Driver/platform.o ./Core/Src/Driver/platform.su ./Core/Src/Driver/uart.cyclo ./Core/Src/Driver/uart.d ./Core/Src/Driver/uart.o ./Core/Src/Driver/uart.su

.PHONY: clean-Core-2f-Src-2f-Driver

