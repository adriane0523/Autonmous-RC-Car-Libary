################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_ftfx_cache.c \
../drivers/fsl_ftfx_controller.c \
../drivers/fsl_ftfx_flash.c \
../drivers/fsl_ftfx_flexnvm.c \
../drivers/fsl_gpio.c \
../drivers/fsl_i2c.c \
../drivers/fsl_lpsci.c \
../drivers/fsl_smc.c \
../drivers/fsl_uart.c 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_ftfx_cache.o \
./drivers/fsl_ftfx_controller.o \
./drivers/fsl_ftfx_flash.o \
./drivers/fsl_ftfx_flexnvm.o \
./drivers/fsl_gpio.o \
./drivers/fsl_i2c.o \
./drivers/fsl_lpsci.o \
./drivers/fsl_smc.o \
./drivers/fsl_uart.o 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_ftfx_cache.d \
./drivers/fsl_ftfx_controller.d \
./drivers/fsl_ftfx_flash.d \
./drivers/fsl_ftfx_flexnvm.d \
./drivers/fsl_gpio.d \
./drivers/fsl_i2c.d \
./drivers/fsl_lpsci.d \
./drivers/fsl_smc.d \
./drivers/fsl_uart.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL46Z256VLL4 -DCPU_MKL46Z256VLL4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I../board -I"C:\nxp\MCUXpressoIDE_11.0.0_2516\ide\plugins\com.nxp.mcuxpresso.tools.win32_11.0.0.201905131304\tools\redlib\include" -I"C:\nxp\MCUXpressoIDE_11.0.0_2516\ide\plugins\com.nxp.mcuxpresso.tools.win32_11.0.0.201905131304\tools\features\include" -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\GPSlib\CMSIS" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\GPSlib\source" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\GPSlib\utilities" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\GPSlib\drivers" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\GPSlib\board" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\LCDLibrary\CMSIS" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\LCDLibrary\source" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\LCDLibrary\board" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\IMULibrary\CMSIS" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\IMULibrary\source" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\IMULibrary\utilities" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\IMULibrary\drivers" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\IMULibrary\board" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


