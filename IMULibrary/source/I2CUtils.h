/*
 * I2CUtils.h
 *
 *  Created on: Oct 30, 2019
 *      Author: kaika
 */

#ifndef I2CUTILS_H_
#define I2CUTILS_H_



void I2C1clearStatusFlags();
void I2C1TCFWait();

void I2C1IICIFWait();
void I2C1SendStart();
void I2C1RepeatStart();

void I2C1SendStop();
void I2C1clearIICIF();
unsigned int I2C1RxAK();
void I2C1WriteByte(unsigned char DevAddress, unsigned char RegAddress, unsigned char Data);

void I2C1ReadBlock(unsigned char DevAddress, unsigned char RegAddress, unsigned char* Data, unsigned short Length);

void initI2C1();


#endif /* I2CUTILS_H_ */
