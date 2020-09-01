/*
 * IMUInterface.c
 *
 *  Created on: Oct 30, 2019
 *      Author: kaika
 */


#include "I2CUtils.h"
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "IMUInterface.h"
//systick code, for delay timing
volatile uint32_t g_systickCounter;

void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}

void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while(g_systickCounter != 0U)
    {
    }
}

//initializes the IMU
//returns Device ID, 160 if success, 0 if error.
unsigned int DeviceInit(){
	uint8_t x;

	//make sure the systick is setup correctly
    if(SysTick_Config(SystemCoreClock / 1000U))
    {
        while(1)
        {
        }
    }

    //initialize the I2C1 Module
    initI2C1();
    //check to see if the IMU is responsive by reading the chip ID
    I2C1ReadBlock(IMU_ADDRESS, 0x0, &x, 1);
    if(x != 160){
    	return 0;
    }
    //printf("Chip ID: %d\n", x);
    //send system reset
    //I2C1WriteByte(IMU_ADDRESS, 0x3F, (1<<5));
    //SysTick_DelayTicks(10000);

    //change to config mode
    I2C1WriteByte(IMU_ADDRESS, 0x3D, 0);
    SysTick_DelayTicks(100);

    //change to register page 0
    I2C1WriteByte(IMU_ADDRESS, 0x07, 0);

    //set normal power mode
    I2C1WriteByte(IMU_ADDRESS, 0x3E, 0);
    SysTick_DelayTicks(100);

    //change mode to NDOF
    I2C1WriteByte(IMU_ADDRESS, 0x3D, 0x0C);
    SysTick_DelayTicks(100);

    //set to external clock source
    I2C1WriteByte(IMU_ADDRESS, 0x3F, (1<<7));
    SysTick_DelayTicks(100);

    //read chip ID and return the ID. 0 indicates failure
    x = 0;
    I2C1ReadBlock(IMU_ADDRESS, 0x0, &x, 1);
    return x;
}

//Returns the offset values from calibration in Offsets argument
//bytes 0-5 are for the accelerometer, 6-11 are for the magnometer, 12-17 are for the gyroscope.
//each offset value is two bytes wide, with the least significant byte first, i.e. byte zero is X offset LSB.
//order of offsets are X,Y,Z.
//INPUTS: Offsets, pointer byte array of size 18.
//OUTPUTS: Device ID, should be 160, 0 if error.
//Calibration Offset values written to Offsets array.
void CalibrateIMUBlocking(uint8_t *Offsets){
	int count = 0;
	uint8_t cal = 0,x = 0;
	//while the calibration status has not held steady at 255 for at least 50 samples
    while(count < 50){
    	//check the make the sure the device is responsive by reading the chip ID
        I2C1ReadBlock(IMU_ADDRESS, 0x0, &x, 1);
        if(x != 160){
        	printf("Device Communication Failure\n");
        	return;
        }
        //read the calibration status register
    	I2C1ReadBlock(IMU_ADDRESS, 0x35, &cal, 1);
    	printf("Calibration Values: %d\n", cal);
        SysTick_DelayTicks(100);
        //if the calibration status is 255, increment the counter
        if(cal == 255){
        	count++;
        }
        else{
        	count = 0;
        }
    }

    //change to config mode
    I2C1WriteByte(IMU_ADDRESS, 0x3D, 0);
    SysTick_DelayTicks(100);

    //read the offset values
    I2C1ReadBlock(IMU_ADDRESS, 0x55, Offsets, 18);

    //change to NDOF mode.
    I2C1WriteByte(IMU_ADDRESS, 0x3D, 0x0C);
    SysTick_DelayTicks(100);

}

//writes calibration offsets, the data in offsets must be in the same order the data appears in the IMU's registers
//bytes 0-5 are for the accelerometer, 6-11 are for the magnometer, 12-17 are for the gyroscope.
//each offset value is two bytes wide, with the least significant byte first, i.e. byte zero is X offset LSB.
//order of offsets are X,Y,Z.
//INPUTS: Offsets, pointer to byte array of size 18.
//OUTPUTS: Device ID, should be 160, 0 if error.
unsigned int WriteOffsets(uint8_t* Offsets){
	uint8_t status = 0, x = 0;

    //change to config mode
    I2C1WriteByte(IMU_ADDRESS, 0x3D, 0);
    SysTick_DelayTicks(100);

    //write each offset value in their respective register
	for(int i = 0; i < 18; i++){
		I2C1WriteByte(IMU_ADDRESS, 0x55 + i, Offsets[i]);
	}

    //change mode to NDOF by writing 0x0C to register at address 0x3D
    I2C1WriteByte(IMU_ADDRESS, 0x3D, 0x0C);
    SysTick_DelayTicks(100);

    //check to make sure system status is ready (SYS bits in calibration status register are 0b11)
   while(0){
    // while(status < 0xC0){
    	//check device responsiveness by reading chip ID
        I2C1ReadBlock(IMU_ADDRESS, 0x0, &x, 1);
        if(x != 160){
        	printf("Device Communication Failure\n");
        	return x;
        }
        printf("Calibration Status: %d\n", status);
        //read the chip status
    	I2C1ReadBlock(IMU_ADDRESS, 0x35, &status, 1);
        SysTick_DelayTicks(100);
    }

    //read chip ID and return.
    I2C1ReadBlock(IMU_ADDRESS, 0x0, &x, 1);
    return x;
}

//returns Euler Vector data from the IMU.
//Data is a pointer to a byte array of length 6.
//each vector is two bytes wide, LSB first.
//bytes 0-1 are the heading, 2-3, roll, 4-5 pitch.
//returns device ID, 0 if error. Euler Vector data return in modified data array.
//Also writes the euler heading degree and minute components in the associated parameters
unsigned int GetEulerVectors(uint8_t* Data, uint16_t* HeadingDeg, uint16_t* HeadingMin){
	uint8_t x = 0;
	//Read the data and put it into the array.
    I2C1ReadBlock(IMU_ADDRESS, 0x1A, Data, 6);
    //read chip ID and return, 0 means failure.
    I2C1ReadBlock(IMU_ADDRESS, 0x0, &x, 1);

    *HeadingDeg = GetEulerHeadingDeg(Data[0], Data[1]);
    *HeadingMin = GetEulerHeadingMin(Data[0], Data[1]);
    return x;
}

//Returns an integer representing the degrees portion of the Euler Heading
//LSB is the least significant byte read from the IMU.
//MSB is the most significant byte read from the IMU.

unsigned int GetEulerHeadingDeg(uint8_t LSB, uint8_t MSB){
	uint16_t Heading = LSB | (MSB << 8);
	return Heading >> 4;
}

//Returns an integer representing the minutes portion of the Euler Heading
//LSB is the least significant byte read from the IMU.
//MSB is the most significant byte read from the IMU.

unsigned int GetEulerHeadingMin(uint8_t LSB, uint8_t MSB){
	uint16_t Heading = LSB | (MSB << 8);
	Heading &= 0b1111;
	return (Heading * 60) >> 4;
}
