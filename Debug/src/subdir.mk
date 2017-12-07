################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/core_cm3.c \
../src/cr_startup_lpc13.c \
../src/font5x7.c \
../src/funkcje_7segled.c \
../src/funkcje_GPIO.c \
../src/funkcje_OLED.c \
../src/funkcje_SSP.c \
../src/funkcje_diody.c \
../src/funkcje_i2c.c \
../src/funkcje_light.c \
../src/funkcje_ratatuj.c \
../src/funkcje_speaker_tone.c \
../src/funkcje_timer.c \
../src/funkcje_uart.c \
../src/main.c \
../src/system_LPC13xx.c 

OBJS += \
./src/core_cm3.o \
./src/cr_startup_lpc13.o \
./src/font5x7.o \
./src/funkcje_7segled.o \
./src/funkcje_GPIO.o \
./src/funkcje_OLED.o \
./src/funkcje_SSP.o \
./src/funkcje_diody.o \
./src/funkcje_i2c.o \
./src/funkcje_light.o \
./src/funkcje_ratatuj.o \
./src/funkcje_speaker_tone.o \
./src/funkcje_timer.o \
./src/funkcje_uart.o \
./src/main.o \
./src/system_LPC13xx.o 

C_DEPS += \
./src/core_cm3.d \
./src/cr_startup_lpc13.d \
./src/font5x7.d \
./src/funkcje_7segled.d \
./src/funkcje_GPIO.d \
./src/funkcje_OLED.d \
./src/funkcje_SSP.d \
./src/funkcje_diody.d \
./src/funkcje_i2c.d \
./src/funkcje_light.d \
./src/funkcje_ratatuj.d \
./src/funkcje_speaker_tone.d \
./src/funkcje_timer.d \
./src/funkcje_uart.d \
./src/main.d \
./src/system_LPC13xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv1p30_LPC13xx -D__CODE_RED -D_LPCXpresso_ -D__REDLIB__ -I"C:\Users\piotrpc\Desktop\olejej\project_REFLEKS_C11\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


