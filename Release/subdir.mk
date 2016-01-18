################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Common.cpp \
../Database.cpp \
../InapMessage.cpp \
../M3UAmessage.cpp \
../SccpMessage.cpp \
../SctpConnection.cpp \
../TcapMessage.cpp \
../protoVSCP.cpp 

OBJS += \
./Common.o \
./Database.o \
./InapMessage.o \
./M3UAmessage.o \
./SccpMessage.o \
./SctpConnection.o \
./TcapMessage.o \
./protoVSCP.o 

CPP_DEPS += \
./Common.d \
./Database.d \
./InapMessage.d \
./M3UAmessage.d \
./SccpMessage.d \
./SctpConnection.d \
./TcapMessage.d \
./protoVSCP.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/aalto/workspace/ProtoVSCP/asn" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


