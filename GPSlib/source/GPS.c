#include "GPS.h"


// reverses a string 'str' of length 'len'
void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

 // Converts a given integer x to string str[].  d is the number
 // of digits required in output. If d is more than the number
 // of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

/*
void convert_float_to_char(float n, char* res, int afterpoint)
{
	// Extract integer part
	    int ipart = (int)n;

	    // Extract floating part
	    float fpart = n - (float)ipart;

	    // convert integer part to string
	    int i = intToStr(ipart, res, 0);

	    // check for display option after point
	    if (afterpoint != 0)
	    {
	        res[i] = '.';  // add dot

	        // Get the value of fraction part upto given no.
	        // of points after dot. The third parameter is needed
	        // to handle cases like 233.007
	        fpart = fpart * pow(10, afterpoint);

	        intToStr((int)fpart, res + i + 1, afterpoint);
	    }
}*/

void convert_float_to_char(float n, char* res, int afterpoint)
{
		int neg = 0;
		if (n < 0)
		{
			n = n * -1.0;
			neg = 1;
		}
	// Extract integer part
	    int ipart = (int)n;

	    // Extract floating part
	    float fpart = n - (float)ipart;

	    // convert integer part to string
	    int i = intToStr(ipart, res, 0);

	    // check for display option after point
	    if (afterpoint != 0)
	    {
	        res[i] = '.';  // add dot

	        // Get the value of fraction part upto given no.
	        // of points after dot. The third parameter is needed
	        // to handle cases like 233.007
	        fpart = fpart * pow(10, afterpoint);

	        intToStr((int)fpart, res + i + 1, afterpoint);
	    }

	    if (neg)
	    {
	    	for (int i = strlen(res); i > 0; i--)
	    	{
	    		res[i +1] = res[i];
	    	}
	    	res[0]= '-';
	    }
}



void delay_ms(unsigned short delay_t)
{
	for(int i = 0; i < delay_t*1750; i++);
}

void sendByte(char data)
{
	int dummy = UART1->S1;
	dummy = dummy + 1;

	UART1->D = data; // send the data over UART

	// wait for the transmission complete flag
	while(!(UART1->S1 & ( 1 << 6 )));
}

void sendCommand(char* data)
{
	UART1->C2 |= (0 << 2); // turn off receiving over UART
	UART1->C2 |= (1 << 3); // turn on transmission over UART

	while(*data != '\0')
	{
		sendByte(*data);
		data++;
	}

	UART1->C2 |= (0 << 3); // turn off transmission
	UART1->C2 |= (1 << 2); // turn on receiving
}

// Location defined in startup/startup_MKL46Z4.c
void UART1_IRQHandler(void)
{
	if ((UART1->S1 & ( 1 << 5))){

		if ((char)(UART1->D) == '$')
		{
			free(current_read);
			current_read = (char *)( malloc(sizeof(char) * 100));
			for (int i = 0; i < size; i++)
			{
				current_read[i] = destination[i];
			}

			free(destination);
			destination = (char *)( malloc(sizeof(char) * 100));
			index = 0;
			size = 0;
		}

		destination[index] = (char)(UART1->D);
		index++;
		size++;

	}
	// read the receive flag, requirement to clear it
	int dummy = UART1->S1;
	dummy++;

    // save initial location

    // receive flag auto cleared when it is read and then UART1->D is also read
}

void setup_UART1_interrupt()
{
	UART1->C2 |= (1 << 5); // interrupts will be requested when RDRF flag == 1 (receive complete flag)

	// Leave as priority 0

	// Call Core API to Enable IRQ for UART1
    NVIC_EnableIRQ(13);
}

struct gps_point* parse_gps(char* sentence)
{
	char* token;
	struct gps_point* point = (struct gps_point*) malloc(sizeof(struct gps_point)); // the gps point that will be read
	token = strtok(sentence, ",");
	int part = 0; // indicates which string (in between ',') is being looked at
	//printf("\n");

	int updateFix = 0;

	while(token != NULL)
	{
	//	printf("%d: %s\n", part, token);
		char temp_deg[4];
		char temp_min[10];

		if(part == 3 && !fixGPS)
		{

			//parse degree
			temp_deg[0] = token[0];
			temp_deg[1] = token[1];
			temp_deg[2] = '\0';

			int index = 0;
			for ( int i = 2; i < strlen(token); i++)
			{
				temp_min[index] = token[i];
				index = index + 1;
			}

			temp_min[index + 1] = '\0';

			//convert to degree coordinates

			float result = (float)(atof(temp_deg) + (float)(atof(temp_min)/60));
			point->latitude = result;
		}

		if(part == 2 && fixGPS)
		{
			//parse degree
			temp_deg[0] = token[0];
			temp_deg[1] = token[1];
			temp_deg[2] = '\0';

			int index = 0;
			for ( int i = 2; i < strlen(token); i++)
			{
				temp_min[index] = token[i];
				index = index + 1;
			}

			temp_min[index + 1] = '\0';

			//convert to degree coordinates

			float result = (float)(atof(temp_deg) + (float)(atof(temp_min)/60));
			point->latitude = result;
		}

		// check if GPS is fixed
		if(!fixGPS && part == 4 && (token[0] == 'N' || token[0] == 'S'))
			updateFix = 1;

		// check if we have received a valid "destination" sentence
		if(!savePoint && part == 3 && (token[0] == 'N' || token[0] == 'S'))
			savePoint = 1;

		if(part == 5 && !fixGPS)
		{
			//parse degree
			temp_deg[0] = token[0];
			temp_deg[1] = token[1];
			temp_deg[2] = token[2];

			temp_deg[3] = '\0';

			//parse minutes
			int index = 0;
			for ( int i = 3; i < strlen(token); i++)
			{
				temp_min[index] = token[i];
				index = index + 1;
			}

			temp_min[index + 1] = '\0';

			//convert to degree coordinates

			float result = (float)(atof(temp_deg) + (float)(atof(temp_min)/60));
			point->longitude = -1.0*result;
		}

		if(part == 4 && fixGPS)
		{
			//parse degree
			temp_deg[0] = token[0];
			temp_deg[1] = token[1];
			temp_deg[2] = token[2];

			temp_deg[3] = '\0';

			//parse minutes
			int index = 0;
			for ( int i = 3; i < strlen(token); i++)
			{
				temp_min[index] = token[i];
				index = index + 1;
			}

			temp_min[index + 1] = '\0';

			//convert to degree coordinates

			float result = (float)(atof(temp_deg) + (float)(atof(temp_min)/60));
			point->longitude = -1.0*result;
		}

		part++;
		token = strtok(NULL, ",");
	}

	if(updateFix)
		fixGPS = 1;

	return point;
}

// calculate the north-oriented angle between two gps points, returns angle in degrees
float calculate_angle(struct gps_point* coordinate_1, struct gps_point* coordinate_2)
{
	float x_1 = coordinate_1->longitude; //lat
	float x_2 = coordinate_2->longitude;

	float y_1 = coordinate_1->latitude; //1->long
	float y_2 = coordinate_2->latitude;

	float y = y_2 - y_1;
	float x = x_2 - x_1;

	//float tan2x = atan2(y * (float)(M_PI/180.0) ,x * (float)(M_PI/180.0)) * (float)(180.0/M_PI ) ;

	float angle = (float)(90.0 - ( atan2(y * (float)(M_PI/180.0) ,x * (float)(M_PI/180.0 )) * (float)(180.0/M_PI )));

	return angle;
}

// calculate the distance between two gps points, returns distance in meters
float calculate_distance(struct gps_point* coordinate_1, struct gps_point* coordinate_2)
{
	print_float(coordinate_1->latitude);
	print_float(coordinate_2->latitude);

	float x_1 = coordinate_1->longitude;
	float x_2 = coordinate_2->longitude;

	float y_1 = coordinate_1->latitude;
	float y_2 = coordinate_2->latitude;

	float y = y_2 - y_1;
	float x = x_2 - x_1;

	x=x*92.3*1000.0;
	y=y*110.6*1000.0; // these are based on how far one degree is where we are (in phx/tempe)

	float tot = (float) sqrt((x*x)+(y*y)); // distance between the two points

	return tot;

	/*
	---------------------------------------
	haversine formula: another way to do it
	---------------------------------------

	float distance_degrees = sqrt( pow(x,2) + pow(y,2) );

	float R = 6378.137;//earth radius in km

	float dLat = ( x_2 * (float)(M_PI / 180.0) ) - ( x_1 * (float)(M_PI / 180.0) );
	float dLon = ( y_2 * (float)(M_PI / 180.0) ) - ( y_1 * (float)(M_PI / 180.0) );
	printf("%d\n", (int)(dLat));

	printf("%d\n", (int)(dLon));

	float a = sin(dLat/2.0) * sin(dLat/2.0) + cos(x_1 * (float)(M_PI / 180.0)) * cos(x_2 * (float)(M_PI / 180.0)) * sin(dLon/2.0) * sin(dLon/2.0);

	float c = 2.0 * atan2(sqrt(a), sqrt(1-a));
	printf("%d\n", (int)(c));
	float d = R * c;

	return d * 1000.0; // meters
*/
}

// saves the gps data of the current location the car is in. Saves in the global var destinationLocation
void save_current_gps()
{
	struct gps_point* parsedPoint;
	while(!savePoint)
		{
			int temp_size = size;

			char* temp = (char *) malloc(sizeof(char) * 100); // the sentence being read from the GPS module

			// build the sentence one char at a time
			for (int i = 0; i < temp_size ;i++)
			{

				temp[i] = current_read[i];
			}

			//printf("\n");

			parsedPoint = parse_gps(temp); // return the gps coordinates that the sentence indicates

			if(!savePoint)
			{
				free(parsedPoint); // free the memory because it is a garbage val
			}

			free(temp); // free the memory used to hold the sentence

			delay_ms(5000); // wait 1s before reading again
		}

		destinationLocation = parsedPoint; // point initialLocation to parsed point once a fix is achieved
		/*
		printf("\Saved Lat:");
		print_float(destinationLocation->latitude);
		printf("\nSaved Long:");
		print_float(destinationLocation->longitude);*/

		savePoint = 0; // initialize this to 0 for next time we are waiting for a fix at the new location
}

float get_bearing_angle_float()
{
	float angle = calculate_angle(initialLocation, destinationLocation);
	return angle;
}

float get_distance_float()
{
	float distance = calculate_distance(initialLocation,destinationLocation);
	return distance;
}


struct gps_point* get_current_coordinates()
{
	struct gps_point* parsedPoint;
	int temp_size = size;
	char* temp = (char *) malloc(sizeof(char) * 100); // the sentence being read from the GPS module

	// build the sentence one char at a time
	for (int i = 0; i < temp_size ;i++)
	{

		temp[i] = current_read[i];
	}

	parsedPoint = parse_gps(temp); // return the gps coordinates that the sentence indicates
	free(temp); // free the memory used to hold the sentence

	return parsedPoint;
}


float get_inital_latitude_float()
{
	return initialLocation->latitude;
}
float get_saved_latitude_float()
{
	return destinationLocation->latitude;
}
float get_inital_longitude_float()
{
	return initialLocation->longitude;
}
float get_saved_longitude_float()
{
	return destinationLocation->longitude;
}
struct gps_point* get_inital_coordinates()
{
	return initialLocation;
}


char* get_bearing_angle_char_arr()
{
	float angle = calculate_angle(initialLocation, destinationLocation);
	char result[20];

	convert_float_to_char(angle, result, 5);

	printf("\ANGLE: ");
	for (int i = 0; i < strlen(result); i++)
	{
		printf("%c", result[i]);
	}
	printf("\n");


	return result;

}

char* get_distance_char_arr()
{
	float distance = calculate_distance(initialLocation,destinationLocation);
	char result[20];
	convert_float_to_char(distance, result, 5);

	printf("\nDISTANCE: ");
	for (int i = 0; i < strlen(result); i++)
	{
		printf("%c", result[i]);
	}
	printf("\n");


	return result;
}

void setupGPS()
{
    SIM->SCGC5 |= (1 << 13); // clock gating for PORTE
    SIM->SCGC4 |= (1 << 11); // clock gating for the UART1

    PORTE->PCR[0] &= ~0xF0703; // Clear MUX First
    PORTE->PCR[1] &= ~0xF0703; // clear MUX

    PORTE->PCR[0] |= (1 << 8) | (1 << 9) | (0 << 10); // set mux for PTE0 to UART1_TX (ALT3) TX = transmit
    PORTE->PCR[1] |= (1 << 8) | (1 << 9) | (0 << 10); // set mux for PTE1 to UART1_RX (ALT3) RX = receive

    GPIOE->PDDR |= (1 << 0); // configure PTE0 for output (send GPS commands to GPS module)
    GPIOE->PDDR |= (0 << 1) ; // configure PTE1 for input (receive GPS sentences)

    // baudRateDIV = clksrc/(16 * baudRate) <----- we are using baudRate = 9600
    // baudRateDIV = 24MHz/(16 * 9600) = 156.25 =~ 156 (truncate decimal)

    // set baudRateDiv in UART1
    UART1->BDH = 0; // upper 5 bits, needs to be set first even though not used
    UART1->BDH |= (0 << 5); // configure for 1 stop bit (8-N-1)
    UART1->BDL = 156; // lower 8 bits


   // initialLocation = (char *)( malloc(sizeof(char) * 100));
    destination = (char *)( malloc(sizeof(char) * 100));
    current_read = (char *) (malloc(sizeof(char) * 100));

    setup_UART1_interrupt();

    sendCommand("$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"); // setting NMEA output
    delay_ms(5000); // wait 3 seconds
    sendCommand("$PMTK220,1000*1F\r\n"); // set update rate to 1hz (1 gps fix sent every second)

}


void save_destination()
{

    struct gps_point* parsedPoint;// = malloc(sizeof(struct gps_point*));

    // obtain initial location GPS fix data
    while(!fixGPS)
    {
    	int temp_size = size;
    	printf("%d\n", temp_size);

    	char* temp = (char *) malloc(sizeof(char) * 100); // the sentence being read from the GPS module

    	// build the sentence one char at a time
    	for (int i = 0; i < temp_size ;i++)
    	{
    		//printf("%c", current_read[i]);
    		temp[i] = current_read[i];
    	}

    //	printf("\n");

    	parsedPoint = parse_gps(temp); // return the gps coordinates that the sentence indicates

    	if(!fixGPS)
    	{
    		free(parsedPoint); // free the memory because it is a garbage val
    	}

    	free(temp); // free the memory used to hold the sentence

    	delay_ms(5000); // wait 1s before reading again
    }

    initialLocation = parsedPoint; // point initialLocation to parsed point once a fix is achieved

    /*
    printf("\nInitial Lat:");
    print_float(initialLocation->latitude);
    printf("\nInitial Long:");
    print_float(initialLocation->longitude);*/
}
void print_float(float num)
{

	char result[20];

	convert_float_to_char(num, result, 5);
	printf("\n\"%s\"\n", result);
}


/*
 * Project 5 methods
 */


int reached_destination()
{
	int result = 1;
	if ( calculate_distance(initialLocation,get_current_coordinates()) < 2)
	{
		result = 0;

	}
	return result;
}



