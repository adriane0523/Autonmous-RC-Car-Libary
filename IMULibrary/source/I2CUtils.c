/*
 * I2CUtils.c
 *
 *  Created on: Oct 30, 2019
 *      Author: kaika
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "I2CUtils.h"

//Would really have liked to declare these as macros but I always find a way to screw up macro syntax.

//Clear the STOPF, STARTF, ARBL (Arbitration Lost), and IICIF (Interrupt Flag) in the I2C1 Module's status registers
void I2C1clearStatusFlags(){
	I2C1->FLT |= (uint8_t)((1 << 6) | (1 << 4));
	I2C1->S |= (1 << 4) | (1 << 1);
}
//Waits for the TCF bit to be set. TCF is cleared by reading from the data register
void I2C1TCFWait(){
	while(!(I2C1->S & (1 << 7))){
	}
}
//Waits for the IICIF bit to be set, must be cleared by writing a 1 to its location
void I2C1IICIFWait(){
	while(!(I2C1->S & (1 << 1))){
	}
}
//Send Start Condition by writing a 1 to MST and TX.
void I2C1SendStart(){
	I2C1->C1 |= (1 << 5) | (1 << 4);
}

//Send Same as Send Start but with RSTA bit set as well to send a repeated start
void I2C1RepeatStart(){
	I2C1->C1 |= (1 << 5) | (1 << 4) | (1 << 2);
	//wait 6 cycles by stalling the processor.
	__asm("nop\n");
	__asm("nop\n");
	__asm("nop\n");
	__asm("nop\n");
	__asm("nop\n");
	__asm("nop\n");
}
//Send stop condition on the Bus.
//Clears all bits that were written by SendStart.
//Waits for the Busy Flag to go idle before exiting.
void I2C1SendStop(){
	I2C1->C1 &= ~((1 << 5) | (1 << 4) | (1 << 3));
	while(I2C1->S & (1 << 5)){
	}
}
//Clear the IICIF bit
void I2C1clearIICIF(){
	I2C1->S |= (1 << 1);
}

//Return ACK status on last transfer, 1 if ACK, 0 if NACK.
unsigned int I2C1RxAK(){
	return ~(I2C1->S) & 1;
}

//Writes one byte to the device specified by DevAddress (7-bit, unshifted) to the register specified by RegAddress (8-bit)
void I2C1WriteByte(uint8_t DevAddress, uint8_t RegAddress, uint8_t Data){
	//clear flags
	I2C1clearStatusFlags();
	//wait for any current transfers to complete.
	I2C1TCFWait();
	//Send start
	I2C1SendStart();
	//write device address on the bus, R\W = 0
	I2C1->D = (DevAddress << 1);
	//wait for the transfer to complete
	I2C1IICIFWait();
	//if NACK received, error
	if(!I2C1RxAK()){
		printf("NO RESPONSE FROM DEVICE ADDRESS %d\n", DevAddress);
		//end transmission
		I2C1SendStop();
		return;
	}
	//clear the IICIF bit
	I2C1clearIICIF();
	//Write register address on the bus (this sets the slave device's internal memory pointer to that address)
	I2C1->D = (RegAddress);
	//wait for transfer to complete
	I2C1IICIFWait();
	//if NACK, error.
	if(!I2C1RxAK()){
		printf("NO RESPONSE FROM REGISTER ADDRESS %d\n", RegAddress);
		//end transmission
		I2C1SendStop();
		return;
	}
	//Wait for transfer to complete
	I2C1TCFWait();
	//clear the IICIF bit
	I2C1clearIICIF();
	//Write the data byte onto the bus (will be written to target register)
	I2C1->D = (Data);
	//wait for transfer to complete
	I2C1IICIFWait();
	//if NACK, error.
	if(!I2C1RxAK()){
		printf("Incorrect ACK\n");
	}
	//clear the IICIF bit
	I2C1clearIICIF();
	//End transmission
	I2C1SendStop();
}

//Reads a block of data from the target device specified by DevAddress, starting from the address specified by RegAddress.
//Data is a pointer to a byte array of size Length.
//Read Data is return in the array pointed to by Data.
void I2C1ReadBlock(uint8_t DevAddress, uint8_t RegAddress, uint8_t* Data, uint16_t Length){
	//Dummy variable to ensure dummy data register read is not optimized out.
	uint8_t dummy = 0;
	//clear status flags
	I2C1clearStatusFlags();
	//Wait for any pending transfers to complete
	I2C1TCFWait();
	//Begin Transmission
	I2C1SendStart();
	//Write Device address on bus R/W = 0.
	I2C1->D = (DevAddress << 1);
	//wait for transfer to complete
	I2C1IICIFWait();
	//If NACK, error
	if(!I2C1RxAK()){
		printf("NO RESPONSE FROM DEVICE ADDRESS %d\n", DevAddress);
		//end transmission
		I2C1SendStop();
		return;
	}
	//clear the IICIF bit
	I2C1clearIICIF();
	//Write the target register address on the bus (this sets the slave device's internal memory pointer to that address)
	I2C1->D = (RegAddress);
	//wait for transfer to complete
	I2C1IICIFWait();
	//if NACK, error
	if(!I2C1RxAK()){
		printf("NO RESPONSE FROM REGISTER ADDRESS %d\n", RegAddress);
		//end transmission
		I2C1SendStop();
		return;
	}
	//clear the IICIF bit.
	I2C1clearIICIF();
	//Send a repeated start condition, required for data reads as specified by device datasheets
	I2C1RepeatStart();
	//write the device address on the bus, R\W = 1;
	I2C1->D = (DevAddress << 1) | 1;
	//wait for transfer to complete
	I2C1IICIFWait();
	//if NACK, error
	if(!I2C1RxAK()){
		printf("NO RESPONSE FROM DEVICE ADDRESS %d ON RESTART\n", DevAddress);
		//end transmission
		I2C1SendStop();
		return;
	}
	//wait for transfer to complete
	I2C1TCFWait();
	//clear the IICIF bit
	I2C1clearIICIF();
	//clear the Tx and TxAK bits to set the Master in receive mode and to transmit ACK on byte reception.
	I2C1->C1 &= ~((1 << 4) | (1 << 3));
	//if we are only reading one byte, set TxAK to signal to the slave device to stop transmitting.
	if(Length == 1){
		I2C1->C1 |= (1 << 3);
	}
	//dummy read of data register to initiate reception of data from slave device. This read must not be optimized out.
	dummy = I2C1->D;
	//read in the succeeding bytes
	for(int i = 0; i < Length; i++){
		//wait for transfer to complete
		I2C1IICIFWait();
		//clear the IICIF flag
		I2C1clearIICIF();
		//if this is the second to last byte, set TxAK to transmit a NACK on the next byte (which is the last byte of the transfer)
		if(i == Length - 2){
			I2C1->C1 |= (1 << 3);
		}
		//if this is the last byte of data, end transmission
		if(i == Length - 1){
			I2C1SendStop();
		}
		//copy the received data in the Data array.
		Data[i] = I2C1->D;
	}
	//prevent the compiler from optimizing out the dummy read.
	dummy++;
}

//Initializes the I2C1 module
void initI2C1(){
	//enable clock gating to the I2C1 module and PORTC
	SIM->SCGC4 |= (1 << 7);
	SIM->SCGC5 |= (1 << 11);
	//Set PTC1 and PTC2 to alternate function 2, I2C1.
	//PTC1 is SCL
	PORTC->PCR[1] &= ~0x700;
	PORTC->PCR[1] |= (2 << 8);
	//PTC2 is SDA
	PORTC->PCR[2] &= ~0x700;
	PORTC->PCR[2] |= (2 << 8);
	//Initialize all I2C1 registers to 0. This also disables slave mode.
	I2C1->C1 = 0;
	I2C1->C2 = 0;
	I2C1->D = 0;
	I2C1->SMB = 0;
	I2C1->RA = 0;
	//reset the STOPF and STARTF bits by writing a 1 to them
	I2C1->FLT = 0x50;
	//clear status flags
	I2C1clearStatusFlags();
	//set the SCL clock divider. From the reference manual, this is a divider of 240.
	//The I2C module derives its clock from the Bus Clock (24MHz in this case).
	//24MHz / 240 = 100kHz. I2C Standard Mode.
	//MAKE SURE YOUR PROJECT HAS THE CLOCKS CONFIGURED FOR A BUS CLOCK OF 24MHZ.
	I2C1->F = 0x1F;
	//Enable the I2C1 module
	I2C1->C1 |= (1 << 7);
}
