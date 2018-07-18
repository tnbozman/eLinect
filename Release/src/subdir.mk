################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/kinect_device.cpp \
../src/main.cpp \
../src/objectHunter.cpp \
../src/occupancyMap.cpp \
../src/stateMachine.cpp \
../src/udpNetworkInterface.cpp 

OBJS += \
./src/kinect_device.o \
./src/main.o \
./src/objectHunter.o \
./src/occupancyMap.o \
./src/stateMachine.o \
./src/udpNetworkInterface.o 

CPP_DEPS += \
./src/kinect_device.d \
./src/main.d \
./src/objectHunter.d \
./src/occupancyMap.d \
./src/stateMachine.d \
./src/udpNetworkInterface.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/opencv -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


