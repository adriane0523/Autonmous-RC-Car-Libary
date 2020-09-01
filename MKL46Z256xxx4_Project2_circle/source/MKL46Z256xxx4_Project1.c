
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"
#include "lcd.h"
#include "IMUInterface.h"
#include "GPS.h"

void motor(int speed, int angle);
void setup_motor();


void setup_motor()
{
	//LED to check for power
		SIM->SCGC5 |= 1<<12;
		PORTD->PCR[5] &= ~0x700;
		PORTD -> PCR[5] |= 0x700 &(1<<8);
		GPIOD ->PDDR |= (1<<5);
		//delay_ms(1000);
		GPIOD ->PDOR|= (1<<5);


        /* Write your code here */
    	// Setup PWM
    	SIM->SCGC6 |= (1 << 26); // Clock Enable TPM2
    	//SIM->SOPT2 |= (0x2 << 24); // Set TPMSRC to OSCERCLK

    	// Enable portb2 as pwm (servo control)
    	SIM->SCGC5 |= (1 << 10); // enable clock gating for PORTB
    	PORTB->PCR[2] &= ~(0x700); // Clear MUX
    	PORTB->PCR[2] |= 0x300; // Drive PTB2 with TPM2 (ALT3)

    	PORTB->PCR[3] &= ~(0x700);
    	PORTB->PCR[3] |= 0x300;


    	// Setup Channel 5

    	TPM2->SC &= ~(0b11 << 3);
    	TPM2->CNT = 0;
    	TPM2->SC |= 0b111;
    	TPM2->MOD = 1250;  // set the mod for 1 second using prescaler divide by 128 //1990 after

    	TPM2->CONTROLS[0].CnSC |= 0b101000;  //Left Edge-Aligned Active HIGH PWM.
		TPM2->CONTROLS[1].CnSC |= 0b101000;
		//TPM2->CONTROLS[0].CnV = 98;
		//TPM2->CONTROLS[1].CnV = 125;



    	//TPM2->SC |= 3; //sets prescaler
    	//TPM2->SC |= (1<<3); //start the clock


	//TPM2->SC |= 3; //sets prescaler
	TPM2->SC |= (1<<3); //start the clock
}


void motor(int speed, int angle)
{
	TPM2->CONTROLS[0].CnV = angle;
	TPM2->CONTROLS[1].CnV = speed;

}

int main(void) {
  	/* Init board hardware. */

	BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    unsigned short result = 0; //We need a 16-bit variable
    unsigned int Deg;
    unsigned int Min;
    unsigned char Data[6];
    char str[16];

    setup_motor();



      lcd_init();
      lcd_displayOn();
      lcd_print("WAITING FOR FIX");
   //  setupGPS();

      lcd_clear();
      lcd_print("READY           SELECT DEST");
      DeviceInit();


    int flag = 1;



	while(1) {
		ADC0->SC1[0] = 0x08;
		while(!(ADC0->SC1[0] & 0x80)){	}
		for(int i = 0; i < (1200000); i++){} // 1/4 second
		result = ADC0->R[0]; // Resets COCO

	    GetEulerVectors(Data, &Deg, &Min);
		//snprintf(str, 16, "CurH:%uD", Deg);
	//	printf("%d\n",Deg);
		//motor(125, 100);

		/*Button Press*/
		if(result < 240)
			{
			motor(125,98);
/*
			save_current_gps();
			lcd_clear();
			lcd_print("DEST SAVED");
			delay_ms(3000);
			lcd_clear();
			lcd_printFloat(get_distance_float());
			lcd_print(" meters");
			delay_ms(6000);

			*/
			//GetEulerVectors(Data, &Deg, &Min);
			//snprintf(str, 16, "CurH:%uD", Deg);
			//lcd_clear();
			//lcd_print(str);


/*
			    delay_ms(6000);
			    lcd_clear();
			    lcd_print("DesireH: ");
			   	lcd_printFloat(get_bearing_angle_float());*/





			   	if (flag == 1)
				{
					lcd_clear();
					lcd_print("WAITING FOR FIX");
					save_destination();
					lcd_clear();
					lcd_print("DEST SAVED");
					delay_ms(3000);
					lcd_clear();
					/*
					lcd_print("Saved Lat: ");
					lcd_printFloat(get_inital_latitude_float());
					lcd_print("Saved long: ");
					lcd_printFloat(get_inital_longitude_float());*/

					flag = 0;
				}
				else
				{
					save_current_gps();
					lcd_clear();
					/*
					lcd_print("Saved Lat: ");
					lcd_printFloat(get_saved_latitude_float());
					lcd_print("\nSaved Lat: ");
					lcd_printFloat(get_saved_longitude_float());

					 */
					delay_ms(6000);
					lcd_clear();
					lcd_print("dist: ");
					lcd_printFloat(get_distance_float());
					lcd_print(" meters");
					delay_ms(6000);
					GetEulerVectors(Data, &Deg, &Min);

					//snprintf(str, 16, "CurH:%uD", Deg);
					lcd_clear();
					lcd_print(str);

					delay_ms(6000);
					lcd_clear();
					lcd_print("DesireH: ");
					float result_bearing_angle = get_bearing_angle_float();
					lcd_printFloat(result_bearing_angle);


					delay_ms(6000);
					lcd_clear();
					lcd_print("Start Driving...");
					delay_ms(6000);

					/*if ( result_bearing_angle < 180 )
					{
						result_bearing_angle = 180 - result_bearing_angle;
					}
					else if (result_bearing_angle > 180)
					{
						result_bearing_angle = result_bearing_angle - 180;
					}*/




				//	printf("Adjusting\n");
					//printf("%d", Deg);
					while(Deg != (int)(result_bearing_angle) )
					{
						GetEulerVectors(Data, &Deg, &Min);
						motor(125, 200);


					}

					motor(0,98);
					delay_ms(6000);

					motor(125,98);
					delay_ms(10000);

			//		printf("Going to destination\n");
					while(reached_destination())
					{
						// motor(125, 95);

						GetEulerVectors(Data, &Deg, &Min);
						float desired_angle = calculate_angle(get_inital_coordinates(), get_current_coordinates());

						/*if ( result_bearing_angle < 180 )
						{
							result_bearing_angle = 180 - result_bearing_angle;
						}
						else if (result_bearing_angle > 180)
						{
							result_bearing_angle = result_bearing_angle - 180;

						}*/
						while(Deg != (int)(result_bearing_angle))
						{
							GetEulerVectors(Data, &Deg, &Min);
							motor(125, 200);
							//delay_ms(500);

						}
						motor(0,98);
						delay_ms(6000);

						motor(125,98);
						delay_ms(10000);


					}
			   	}
			}
		}
	return 0;
}
