#include "MKL46Z4.h"
#include "lcd.h"

//All function sets and clears are based on Table 6 from LCD reference manual

/*Completes LCD and Button startup functions*/
void lcd_init()
{
	unsigned short result = 0;

	SIM->SCGC5 |= (1 << 11); //PORTC
	SIM->SCGC6 |= (1 << 27); //ADC

	PORTC->PCR[0] &= ~(0x700); // Clear MUX -- RS
	PORTC->PCR[4] &= ~(0x700); // Clear MUX -- Enable
	PORTC->PCR[6] &= ~(0x700); // Clear MUX -- D4
	PORTC->PCR[7] &= ~(0x700); // Clear MUX -- D5
	PORTC->PCR[10] &= ~(0x700); // Clear MUX -- D6
	PORTC->PCR[11] &= ~(0x700); // Clear MUX -- D7
	PORTC->PCR[13] &= ~(0xF0703); // Clear MUX -- A0

	PORTC->PCR[0] |= (1 << 8); //Set MUX to 1 for GPIO -- RS
	PORTC->PCR[4] |= (1 << 8); //Set MUX to 1 for GPIO -- Enable
	PORTC->PCR[6] |= (1 << 8); //Set MUX to 1 for GPIO -- D4
	PORTC->PCR[7] |= (1 << 8); //Set MUX to 1 for GPIO -- D5
	PORTC->PCR[10] |= (1 << 8); //Set MUX to 1 for GPIO -- D6
	PORTC->PCR[11] |= (1 << 8); //Set MUX to 1 for GPIO -- D7
	PORTC->PCR[13] |= (1 << 8); // Set MUX to 1 for GPIO -- A0

    GPIOC->PDDR |= (0x2CD1); // Setup Pin 0,4,6,7,10,11,13 Port C as output

    /*Start LCD Initiate*/
	// Wait for power
	for(int i = 0; i < (37150); i++){} // ~55ms

	// Function Set 1
	GPIOC->PSOR |= (1 << 4);

	GPIOC->PCOR |= (1 << 0);
	GPIOC->PCOR |= (1 << 11);
	GPIOC->PCOR |= (1 << 10);
	GPIOC->PSOR |= (1 << 7);
	GPIOC->PSOR |= (1 << 6);

	GPIOC->PCOR |= (1 << 4);

	for(int i = 0; i < (28000); i++){} // ~4.1ms

	// Function Set 2
	GPIOC->PSOR |= (1 << 4);

	GPIOC->PCOR |= (1 << 0);
	GPIOC->PCOR |= (1 << 11);
	GPIOC->PCOR |= (1 << 10);
	GPIOC->PSOR |= (1 << 7);
	GPIOC->PSOR |= (1 << 6);

	GPIOC->PCOR |= (1 << 4);

	for(int i = 0; i < (1800); i++){} // ~100 microseconds (162)

	// Function Set 3
	GPIOC->PSOR |= (1 << 4);

	GPIOC->PCOR |= (1 << 0);
	GPIOC->PCOR |= (1 << 11);
	GPIOC->PCOR |= (1 << 10);
	GPIOC->PSOR |= (1 << 7);
	GPIOC->PSOR |= (1 << 6);

	GPIOC->PCOR |= (1 << 4);

	for(int i = 0; i < (700); i++){} // ~37 microseconds (59)

	// Set 4 bit
	GPIOC->PSOR |= (1 << 4);

	GPIOC->PCOR |= (1 << 0);
	GPIOC->PCOR |= (1 << 11);
	GPIOC->PCOR |= (1 << 10);
	GPIOC->PSOR |= (1 << 7);
	GPIOC->PCOR |= (1 << 6);

	GPIOC->PCOR |= (1 << 4);

	for(int i = 0; i < (700); i++){} // ~37 microseconds

	// Set Character and Font Bit 7-4
	GPIOC->PSOR |= (1 << 4);

	GPIOC->PCOR |= (1 << 0);
	GPIOC->PCOR |= (1 << 11);
	GPIOC->PCOR |= (1 << 10);
	GPIOC->PSOR |= (1 << 7);
	GPIOC->PCOR |= (1 << 6);

	GPIOC->PCOR |= (1 << 4);

	// Set Character and Font Bit 3-0
	GPIOC->PSOR |= (1 << 4);

	GPIOC->PCOR |= (1 << 0);
	GPIOC->PSOR |= (1 << 11);
	GPIOC->PCOR |= (1 << 10);
	GPIOC->PCOR |= (1 << 7);
	GPIOC->PCOR |= (1 << 6);

	GPIOC->PCOR |= (1 << 4);

	for(int i = 0; i < (700); i++){} //~37 microseconds

	// Display Off Bit 7-4
	GPIOC->PSOR |= (1 << 4);

	GPIOC->PCOR |= (1 << 0);
	GPIOC->PCOR |= (1 << 11);
	GPIOC->PCOR |= (1 << 10);
	GPIOC->PCOR |= (1 << 7);
	GPIOC->PCOR |= (1 << 6);

	GPIOC->PCOR |= (1 << 4);

	// Display Off Bit 3-0
	GPIOC->PSOR |= (1 << 4);

	GPIOC->PCOR |= (1 << 0);
	GPIOC->PSOR |= (1 << 11);
	GPIOC->PCOR |= (1 << 10);
	GPIOC->PCOR |= (1 << 7);
	GPIOC->PCOR |= (1 << 6);

	GPIOC->PCOR |= (1 << 4);

	for(int i = 0; i < (700); i++){} //~37 microseconds

	// Display Clear Bit 7-4
	GPIOC->PSOR |= (1 << 4);

	GPIOC->PCOR |= (1 << 0);
	GPIOC->PCOR |= (1 << 11);
	GPIOC->PCOR |= (1 << 10);
	GPIOC->PCOR |= (1 << 7);
	GPIOC->PCOR |= (1 << 6);

	GPIOC->PCOR |= (1 << 4);

	// Display Clear 3-0
	GPIOC->PSOR |= (1 << 4);

	GPIOC->PCOR |= (1 << 0);
	GPIOC->PCOR |= (1 << 11);
	GPIOC->PCOR |= (1 << 10);
	GPIOC->PCOR |= (1 << 7);
	GPIOC->PSOR |= (1 << 6);

	GPIOC->PCOR |= (1 << 4);

	for(int i = 0; i < (10000); i++){} //~1.52ms (2462)

	// Entry Mode 7-4
	GPIOC->PSOR |= (1 << 4);

	GPIOC->PCOR |= (1 << 0);
	GPIOC->PCOR |= (1 << 11);
	GPIOC->PCOR |= (1 << 10);
	GPIOC->PCOR |= (1 << 7);
	GPIOC->PCOR |= (1 << 6);

	GPIOC->PCOR |= (1 << 4);

	// Entry Mode  3-0
	GPIOC->PSOR |= (1 << 4);

	GPIOC->PCOR |= (1 << 0);
	GPIOC->PCOR |= (1 << 11);
	GPIOC->PSOR |= (1 << 10);
	GPIOC->PSOR |= (1 << 7);
	GPIOC->PCOR |= (1 << 6);

	GPIOC->PCOR |= (1 << 4);

	for(int i = 0; i < (700); i++){} //~37 microseconds
	/*END LCD Initiate*/

	/*Start Button Initiate*/
    ADC0->SC3 |= (0x7); //Set hardware averaging to 32 samples

    ADC0->CFG1 |= 0; //Default everything

	ADC0->SC1[0] = 0x08; // Set for ADC_SE8 PTB0;

    // Wait for Calibration to Complete (either COCO or CALF)
    while(!(ADC0->SC1[0] & 0x80)){	}

    //Complete Calibration
    result = ADC0->CLP0 + ADC0->CLP1 + ADC0->CLP2 + ADC0->CLP3 + ADC0->CLP4 + ADC0->CLPS; //Add calibration results
    result = result >> 1 | 0x8000; //Shift one to the right to divide by 2 and set the MSB
    ADC0->PG = result; //Save into PG register

    result = 0;
    result = ADC0->CLP0 + ADC0->CLP1 + ADC0->CLP2 + ADC0->CLP3 + ADC0->CLP4 + ADC0->CLPS; //Add calibration results
    result = result >> 1 | 0x8000; //Shift one to the right to divide by 2 and set the MSB
    ADC0->MG = result; //Save into MG register

    ADC0->SC3 = 0; //Set hardware averaging off
    /*END LCD Initiate*/
}

/*Turns on LCD display and enables cursor*/
void lcd_displayOn()
{
// Display and Cursor On Bit 7-4
	GPIOC->PSOR |= (1 << 4);

	GPIOC->PCOR |= (1 << 0);
	GPIOC->PCOR |= (1 << 11);
	GPIOC->PCOR |= (1 << 10);
	GPIOC->PCOR |= (1 << 7);
	GPIOC->PCOR |= (1 << 6);

	GPIOC->PCOR |= (1 << 4);

	// Display and Cursor On Bit 3-0
	GPIOC->PSOR |= (1 << 4);

	GPIOC->PCOR |= (1 << 0);
	GPIOC->PSOR |= (1 << 11);
	GPIOC->PSOR |= (1 << 10);
	GPIOC->PSOR |= (1 << 7);
	GPIOC->PCOR |= (1 << 6);

	GPIOC->PCOR |= (1 << 4);

	for(int i = 0; i < (700); i++){} // ~37 microseconds
}

void lcd_displayUpper(int upperBits) //Sets and clears the correct registers depending on int sent for first half of message
{
	GPIOC->PSOR |= (1 << 4);

	GPIOC->PSOR |= (1 << 0);
	if(upperBits & (1 << 3)){
	GPIOC->PSOR |= (1 << 11);
	}
	else{
		GPIOC->PCOR |= (1 << 11);
	}

	if(upperBits & (1 << 2)){
		GPIOC->PSOR |= (1 << 10);
	}
	else{
		GPIOC->PCOR |= (1 << 10);
	}

	if(upperBits & (1 << 1)){
		GPIOC->PSOR |= (1 << 7);
	}

	else{
		GPIOC->PCOR |= (1 << 7);
	}

	if(upperBits & (1 << 0)){
		GPIOC->PSOR |= (1 << 6);
	}
	else{
		GPIOC->PCOR |= (1 << 6);
	}
	GPIOC->PCOR |= (1 << 4);
}

void lcd_displayLower(int lowerBits) //Sets and clears the correct registers depending on int sent for second half of message
{
	GPIOC->PSOR |= (1 << 4);

	GPIOC->PSOR |= (1 << 0);
	if(lowerBits & (1 << 3)){
	GPIOC->PSOR |= (1 << 11);
	}
	else{
		GPIOC->PCOR |= (1 << 11);
	}

	if(lowerBits & (1 << 2)){
		GPIOC->PSOR |= (1 << 10);
	}
	else{
		GPIOC->PCOR |= (1 << 10);
	}

	if(lowerBits & (1 << 1)){
		GPIOC->PSOR |= (1 << 7);
	}
	else{
		GPIOC->PCOR |= (1 << 7);
	}

	if(lowerBits & (1 << 0)){
		GPIOC->PSOR |= (1 << 6);
	}
	else{
		GPIOC->PCOR |= (1 << 6);
	}
	GPIOC->PCOR |= (1 << 4);
}

/*Displays message onto LCD screen*/
void lcd_print(char message[])
{
	int i = 0;
	char c = message[i];

	while (c != '\0')
	{
		if(i == 16)
		{
			// Set Cursor to Next Line Bit 7-4
			GPIOC->PSOR |= (1 << 4);

			GPIOC->PCOR |= (1 << 0);
			GPIOC->PSOR |= (1 << 11);
			GPIOC->PSOR |= (1 << 10);
			GPIOC->PCOR |= (1 << 7);
			GPIOC->PCOR |= (1 << 6);

			GPIOC->PCOR |= (1 << 4);

			// Set Cursor to Next Line 3-0
			GPIOC->PSOR |= (1 << 4);

			GPIOC->PCOR |= (1 << 0);
			GPIOC->PCOR |= (1 << 11);
			GPIOC->PCOR |= (1 << 10);
			GPIOC->PCOR |= (1 << 7);
			GPIOC->PCOR |= (1 << 6);

			GPIOC->PCOR |= (1 << 4);

			for(int i = 0; i < (700); i++){} // 37microseconds
		}

		int upperBits = c >> 4;
		int lowerBits = c & 15;

		lcd_displayUpper(upperBits);
		lcd_displayLower(lowerBits);

		i++;
		c = message[i];
		for(int i = 0; i < (1400); i++){} // ~74 microseconds
		if(i == 32)
		{
			c = '\0';
		}
	}
}

/*Converts the integer value to a char array in order to display on LCD*/
void lcd_printInt(int i)
{
	char message[12];
	itoa(i, message, 10);
	lcd_print(message);
}

/*Converts the float value to a char array in order to display on LCD*/
void lcd_printFloat(float f)
{
	char message[100];
	sprintf(message, "%.3f", f);
	lcd_print(message);
}

/*Clear content currently displayed on LCD*/
void lcd_clear()
{
	// Display Clear Bit 7-4
	GPIOC->PSOR |= (1 << 4);

	GPIOC->PCOR |= (1 << 0);
	GPIOC->PCOR |= (1 << 11);
	GPIOC->PCOR |= (1 << 10);
	GPIOC->PCOR |= (1 << 7);
	GPIOC->PCOR |= (1 << 6);

	GPIOC->PCOR |= (1 << 4);

	// Display Clear 3-0
	GPIOC->PSOR |= (1 << 4);

	GPIOC->PCOR |= (1 << 0);
	GPIOC->PCOR |= (1 << 11);
	GPIOC->PCOR |= (1 << 10);
	GPIOC->PCOR |= (1 << 7);
	GPIOC->PSOR |= (1 << 6);

	GPIOC->PCOR |= (1 << 4);

	for(int i = 0; i < (10000); i++){} // 1.52ms
}
