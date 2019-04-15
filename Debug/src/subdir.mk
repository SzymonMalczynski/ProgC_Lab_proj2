################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ProjektC2.c \
../src/ipv6.c \
../src/libs.c \
../src/udp.c 

OBJS += \
./src/ProjektC2.o \
./src/ipv6.o \
./src/libs.o \
./src/udp.o 

C_DEPS += \
./src/ProjektC2.d \
./src/ipv6.d \
./src/libs.d \
./src/udp.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


