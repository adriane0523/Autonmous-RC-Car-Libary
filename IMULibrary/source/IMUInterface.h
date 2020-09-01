/*
 * IMUInterface.h
 *
 *  Created on: Oct 30, 2019
 *      Author: kaika
 */

#ifndef IMUINTERFACE_H_
#define IMUINTERFACE_H_

	#ifndef IMU_ADDRESS
	#define IMU_ADDRESS 0x28u
	#endif

unsigned int DeviceInit();
void CalibrateIMUBlocking(uint8_t *Offsets);
unsigned int WriteOffsets(uint8_t* Offsets);
unsigned int GetEulerVectors(uint8_t* Data, uint16_t* HeadingDeg, uint16_t* HeadingMin);
unsigned int GetEulerHeadingDeg(uint8_t LSB, uint8_t MSB);
unsigned int GetEulerHeadingMin(uint8_t LSB, uint8_t MSB);

#endif /* IMUINTERFACE_H_ */
