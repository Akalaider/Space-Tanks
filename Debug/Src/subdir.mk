################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/30010_io.c \
../Src/ansi.c \
../Src/ball.c \
../Src/charset.c \
../Src/io.c \
../Src/lcd.c \
../Src/main.c \
../Src/sinus.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f30x.c \
../Src/timer.c 

OBJS += \
./Src/30010_io.o \
./Src/ansi.o \
./Src/ball.o \
./Src/charset.o \
./Src/io.o \
./Src/lcd.o \
./Src/main.o \
./Src/sinus.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f30x.o \
./Src/timer.o 

C_DEPS += \
./Src/30010_io.d \
./Src/ansi.d \
./Src/ball.d \
./Src/charset.d \
./Src/io.d \
./Src/lcd.d \
./Src/main.d \
./Src/sinus.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f30x.d \
./Src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F302R8Tx -DSTM32F3 -DNUCLEO_F302R8 -DDEBUG -DUSE_STDPERIPH_DRIVER=1 -c -I"/Users/adambenali/STM32CubeIDE/workspace_1.8.0/BaseProject/SPL/inc" -I"/Users/adambenali/STM32CubeIDE/workspace_1.8.0/BaseProject/Inc" -I"/Users/adambenali/STM32CubeIDE/workspace_1.8.0/BaseProject/cmsis" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/30010_io.d ./Src/30010_io.o ./Src/ansi.d ./Src/ansi.o ./Src/ball.d ./Src/ball.o ./Src/charset.d ./Src/charset.o ./Src/io.d ./Src/io.o ./Src/lcd.d ./Src/lcd.o ./Src/main.d ./Src/main.o ./Src/sinus.d ./Src/sinus.o ./Src/syscalls.d ./Src/syscalls.o ./Src/sysmem.d ./Src/sysmem.o ./Src/system_stm32f30x.d ./Src/system_stm32f30x.o ./Src/timer.d ./Src/timer.o

.PHONY: clean-Src

