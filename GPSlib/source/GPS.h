
#ifndef GPS_H
#define GPS_H

#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"
#include <string.h>
#include <math.h>

#define M_PI 3.1415926 // pi

// represents a gps point in lat/long coordinates
struct gps_point
{
	float latitude;
	float longitude;
};

/**
 * User methods
 */
float get_bearing_angle_float(); // return the bearing angle between two gps points, in degrees
float get_distance_float(); // return the distance between two gps points, in meters
char* get_distance_char_arr(); // return the distance between two gps points as a string
char* get_bearing_angle_char_arr(); // return the bearing angle between two points in degrees as a string
void setupGPS(); // setup all necessary clocks and interrupt handlers, and get a gps fix. Saves initial location.
void save_current_gps(); // saves destination location as a gps point. if called again, it overwrites the previous destination.
void save_destination();



float get_inital_latitude_float();
float get_saved_latitude_float();
float get_inital_longitude_float();
float get_saved_longitude_float();
struct gps_point* get_current_coordinates();
struct gps_point* get_inital_coordinates();

char* get_inital_latitude_charr_arr();
char* get_saved_latitude_char_arr();
char* get_inital_longitude_char_arr();
char* get_saved_longitude_char_arr();





/*
 * Project 5 methods
 */
int reached_destination();






/**
 * Helper methods (not needed outside of library)
 */
void sendByte(char data); // used in sendCommand(). sends a char to the gps module, called over and over it sends a string.
void sendCommand(char* data); // sends a string command to the gps module (to configure it upon startup)
void setup_UART1_interrupt(); // setups interrupts for the uart1 (talks to the gps module)
float* parse_GPS(char* sentence); // parses and extracts data from gps sentences passed as strings
float calculate_angle(struct gps_point* coordinate_1, struct gps_point* coordinate_2); // calculate bearing angle (north oriented) between coordinate_1 and coordinate_2
float calculate_distance(struct gps_point* coordinate_1, struct gps_point* coordinate_2 ); // calculate distance in meters betweeen coordinate_1 and coordinate_2
void delay_ms(unsigned short delay_t); // busy wait loop
int intToStr(int x, char str[], int d);
void convert_float_to_char(float num, char* result, int afterpoint); // converts float to string
void reverse(char *str, int len); // reverses a string
void print_float(float num); // prints a float to the console


/**
 * global instance vars
 */
static volatile unsigned int first = 1;
static volatile unsigned int size = 0;
static volatile unsigned int index = 0; // used in the function that parses GPS sentences. It indexes strings
static volatile unsigned int count_calls = 0;
static volatile unsigned int fixGPS = 0; // set to 1 when an initial GPS fix is acquired, and stays 1
static volatile unsigned int savePoint = 0; // set to 1 when any gps point is saved, and back to 0 afterwards
static volatile struct gps_point* initialLocation; // the initial fix position upon startup
static volatile struct gps_point* destinationLocation; // the initial fix position upon startup
static volatile struct gps_point* currentLocation;
static volatile char* destination;
static volatile char* current_read;
static volatile int ready = 1;

#endif
