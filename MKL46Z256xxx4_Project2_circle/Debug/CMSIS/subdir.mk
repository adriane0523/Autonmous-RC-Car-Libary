################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/system_MKL46Z4.c 

OBJS += \
./CMSIS/system_MKL46Z4.o 

C_DEPS += \
./CMSIS/system_MKL46Z4.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ../CMSIS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL46Z256VLL4 -DCPU_MKL46Z256VLL4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I../board -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I../startup -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\LCDLibrary\CMSIS" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\LCDLibrary\source" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\LCDLibrary\board" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\IMULibrary\CMSIS" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\IMULibrary\source" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\IMULibrary\utilities" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\IMULibrary\drivers" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\IMULibrary\board" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\GPSlib\CMSIS" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\GPSlib\source" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\GPSlib\utilities" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\GPSlib\drivers" -I"D:\Documents\MCUXpressoIDE_11.0.0_2516\workspace2\GPSlib\board" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


