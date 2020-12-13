################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../MSP_EXP432P401R_FREERTOS.cmd 

SYSCFG_SRCS += \
../uartecho.syscfg 

LIB_SRCS += \
../msp432p4xx_driverlib.lib 

C_SRCS += \
../Commands.c \
../HeartBeat.c \
../Threads.c \
../Utils.c \
../main_freertos.c \
../uartecho.c \
./syscfg/ti_drivers_config.c 

GEN_FILES += \
./syscfg/ti_drivers_config.c 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./Commands.d \
./HeartBeat.d \
./Threads.d \
./Utils.d \
./main_freertos.d \
./uartecho.d \
./syscfg/ti_drivers_config.d 

OBJS += \
./Commands.obj \
./HeartBeat.obj \
./Threads.obj \
./Utils.obj \
./main_freertos.obj \
./uartecho.obj \
./syscfg/ti_drivers_config.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/syscfg_c.rov.xs 

GEN_MISC_DIRS__QUOTED += \
"syscfg\" 

OBJS__QUOTED += \
"Commands.obj" \
"HeartBeat.obj" \
"Threads.obj" \
"Utils.obj" \
"main_freertos.obj" \
"uartecho.obj" \
"syscfg\ti_drivers_config.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_drivers_config.h" \
"syscfg\syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"Commands.d" \
"HeartBeat.d" \
"Threads.d" \
"Utils.d" \
"main_freertos.d" \
"uartecho.d" \
"syscfg\ti_drivers_config.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_drivers_config.c" 

C_SRCS__QUOTED += \
"../Commands.c" \
"../HeartBeat.c" \
"../Threads.c" \
"../Utils.c" \
"../main_freertos.c" \
"../uartecho.c" \
"./syscfg/ti_drivers_config.c" 

SYSCFG_SRCS__QUOTED += \
"../uartecho.syscfg" 


