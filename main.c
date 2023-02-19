#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "motors.h"
#include "sensors.h"
#include "pushbutton.h"

#define LEFT 			0
#define RIGHT 			1
#define ROTATE_SPEED	100
#define MAXSPEED 		200

void read_from_EEMEM(void)
{
	for(uint8_t i=0; i<numberOfSensors; i++)
	{
		threshold[i] = eeprom_read_word((uint16_t*)(i*2));
		minValues[i] = eeprom_read_word((uint16_t*)((i*2)+16));
		maxValues[i] = eeprom_read_word((uint16_t*)((i*2)+32));
	}
}

void follow_line(uint8_t binary)
{
	uint8_t binaryCount = count_binary(binary);
	switch(binaryCount)
	{
		case 1 :	if(binary == 0b10000000)
						set_motors(-(0.75 * MAXSPEED), MAXSPEED);
					else if(binary == 0b00000001)
						set_motors(MAXSPEED, -(0.75 * MAXSPEED));
					break;
					
		case 2 :	if(binary == 0b00011000)
						set_motors(MAXSPEED, MAXSPEED);						
					else if(binary == 0b11000000)
						set_motors(-(0.50 * MAXSPEED), MAXSPEED);
					else if(binary == 0b00000011)
						set_motors(MAXSPEED, -(0.50 * MAXSPEED));
					else if(binary == 0b01100000)
						set_motors(0, MAXSPEED);
					else if(binary == 0b00000110)
						set_motors(MAXSPEED, 0);
					else if(binary == 0b00001100)
						set_motors(MAXSPEED, 0.50 * MAXSPEED);
					else if(binary == 0b00110000)
						set_motors(0.50 * MAXSPEED, MAXSPEED);
					break;
					
		case 3 :	if(binary == 0b11100000)
						set_motors(-(0.25 * MAXSPEED), MAXSPEED);
					else if(binary == 0b00000111)
						set_motors(MAXSPEED, -(0.25 * MAXSPEED));
					else if(binary == 0b01110000)
						set_motors(0.25 * MAXSPEED, MAXSPEED);
					else if(binary == 0b00001110)
						set_motors(MAXSPEED, 0.25 * MAXSPEED);
					else if(binary == 0b00111000)
						set_motors(0.50 * MAXSPEED, MAXSPEED);
					else if(binary == 0b00011100)
						set_motors(MAXSPEED, 0.50 * MAXSPEED);
					break;
	}
}

void rotate_acute(uint8_t side)
{
	uint8_t binary;
	do
	{
		binary = get_sensors_binary();
		print_binary(binary);
		
		if((side == LEFT && (binary & 0b00000001)) || (side == RIGHT && (binary & 0b10000000)))
			return;
		//Follow the line until both the middle sensors are out of the line.
		follow_line(binary);
	}while((binary & 0b00010000) || (binary & 0b00001000));
	if(side == RIGHT)
		set_motors(ROTATE_SPEED, -ROTATE_SPEED);
	else
		set_motors(-ROTATE_SPEED, ROTATE_SPEED);
	//Rotate until either of the two middle sensor is on the line
	while((binary & 0b00010000) && (binary & 0b00001000))
	{
		binary = get_sensors_binary();
		print_binary(binary);
	}
}

void rotate(uint8_t side)
{
	set_motors(MAXSPEED, MAXSPEED);
	if(get_front_sensor())
		return;
	else
		rotate_acute(side);
}

void intro(void)
{
	init_lcd();
	init_motors();
	init_sensors();
	init_pushbutton();
	
	page:
	clear_screen();
	print_string(7, 1, "Launch");
	print_string(1, 2, "Calibrate");
	print_string(13, 2, "Test");
	switch(wait_for_button_press())
	{
		case LEFT_BUTTON  :	calibrate_sensors();
							goto page;
		
		case TOP_BUTTON   :	read_from_EEMEM();
							break;
					
		case RIGHT_BUTTON :	read_from_EEMEM();
							while(1)
							{
								clear_screen();
								print_binary(get_sensors_binary());
								if(get_front_sensor())
									print_string(8, 1, "1");
								else
									print_string(8, 1, "0");
								_delay_ms(50);
								if(get_single_button_press() < 4)
									goto page;
							}
	}
}

int main(void)
{
	uint8_t binary, binaryCount;
	intro();
	clear_screen();
	while(1)
	{
		binary = get_sensors_binary();
		print_binary(binary);
		binaryCount = count_binary(binary);

		switch(binaryCount)
		{
			case 1 :	follow_line(binary);
						break;
							
			case 2 :	follow_line(binary);
						break;
							
			case 3 :	follow_line(binary);
						if(binary == 0b00011001)
							rotate_acute(RIGHT);
						else if(binary == 0b10011000)
							rotate_acute(LEFT);
						else if(binary == 0b10110000)
							rotate_acute(LEFT);
						else if(binary == 0b00001101)
							rotate_acute(RIGHT);
						else if(binary == 0b10001100)
							rotate_acute(LEFT);
						else if(binary == 0b00110001)
							rotate_acute(RIGHT);
						break;
							
			case 4 :	if(binary == 0b11110000 || binary == 0b01111000)
							rotate(LEFT);
						else if(binary == 0b00001111 || binary == 0b00011110)
							rotate(RIGHT);
						else if(binary == 0b00111100)
							rotate(LEFT);
						else if(binary == 0b00110011)
							rotate_acute(RIGHT);
						else if(binary == 0b11001100)
							rotate_acute(LEFT);
						else if(binary == 0b11011000)
							rotate_acute(LEFT);
						else if(binary == 0b00011011)
							rotate_acute(RIGHT);
						else if(binary == 0b11101000)
							rotate(LEFT);
						else if(binary == 0b00010111)
							rotate(RIGHT);
						else if(binary == 0b10111000)
							rotate(LEFT);
						else if(binary == 0b00011101)
							rotate(RIGHT);
						else if(binary == 0b10011100)
							rotate_acute(LEFT);
						else if(binary == 0b00111001)
							rotate_acute(RIGHT);
						break;
						
			case 5 :	if(binary == 0b11111000 || binary == 0b01111100)
							rotate(LEFT);
						else if(binary == 0b00011111 || binary == 0b00111110)
							rotate(RIGHT);
						else if(binary == 0b11101100)
							rotate(LEFT);							
						else if(binary == 0b00110111)
							rotate(RIGHT);							
						else if(binary == 0b11100110)
							rotate(LEFT);							
						else if(binary == 0b01100111)
							rotate(RIGHT);							
						else if(binary == 0b00111101)
							rotate(RIGHT);							
						else if(binary == 0b10111100)
							rotate(LEFT);							
						else if(binary == 0b00111011)
							rotate(RIGHT);							
						else if(binary == 0b11011100)
							rotate(LEFT);
						break;
							
			case 6 :	if(binary == 0b11111100)
							rotate(LEFT);							
						else if(binary == 0b00111111)
							rotate(RIGHT);							
						else if(binary == 0b01111110)
							rotate(LEFT);	
						break;
							
			case 7 :	
							
			case 8 :	rotate(LEFT);	
						break;
		}
	}
}