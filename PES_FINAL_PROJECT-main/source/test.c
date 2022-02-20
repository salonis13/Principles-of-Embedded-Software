/*
 * test.c - Define functions to test accelerometer and touch sensor
 *
 *  Created on: May 3, 2021
 *      Author: saloni shah
 */
#include <math.h>
#include "fsl_debug_console.h"
#include "test.h"
#include "mma8451.h"
#include "pwm.h"
#include "delay.h"
#include "systick.h"
#include "tsi.h"
#include "i2c.h"

#define MMA_ADDR 0x3A
#define REG_WHOAMI 0x0D

#define TEST_TIME 	10000
#define ROLL_CONST 	2.5
#define PITCH_CONST 10
#define RED_CONST 	100
#define RED_LO		70
#define RED_HI		250
#define GREEN_LO	300
#define GREEN_HI	550
#define BLUE_LO		580
#define LED			255
#define DELAY 		50

int next_r=0,next_g=0,next_b=0;			//decimal values of led colour to be transitioned into
int tsi_value=0;

uint8_t test_mma_device(void)
{
	PRINTF("i2c Test: ");

	//test i2c register and device initialization
	if(i2c_read_byte(MMA_ADDR , REG_WHOAMI)==0x1A)//Reads the device id
	{
		PRINTF("Success\n\r");
		return 1;
	}
	PRINTF("Failure\n\r");
	return 0;
}

void test_accel() {
	float roll=0.0, pitch=0.0;

	//test accelerometer initialization
	if (!init_mma()) {					// init mma peripheral
		change_led(LED, 0, 0);			// Light red error LED
		while (1)						// not able to initialize mma
			;
	}

	PRINTF("Accelerometer initialized...\n\r");
	PRINTF("Tilt the board to test accelerometer\n\r");

	//bubble level demo for accelerometer for 10 seconds
	while ((get_timer())<=TEST_TIME) {

		//get accelerometer values
		read_full_xyz_1();
		roll=convert_to_roll();
		pitch=convert_to_pitch();
		next_r=RED_CONST;
		next_g=fabs((roll)*(ROLL_CONST));
		next_b=fabs((pitch)*(PITCH_CONST));
		change_led(next_r,next_g,next_b);	//change led based on tilt angle
		Delay(DELAY);
	}
	PRINTF("Accelerometer test complete\n\r");
	change_led(0,0,0);						//turn off led
}

//touch sensor test for 10 seconds
void test_tsi() {

	//initialize touch sensor
	init_touch();

	PRINTF("Touch Sensor Initialized\n\r");
	PRINTF("Touch the Capacitive Touch Sensor to test\n\r");

	reset_timer();							//reset systick timer

	while ((get_timer())<=TEST_TIME) {

		//get value from capacitive touch
		tsi_value=touch_scan();

		//change LED colour based on sensor value
		//red light when user touches left side of the sensor
		if((tsi_value>=RED_LO) & (tsi_value<=RED_HI)) {
			PRINTF("You touched left side of TSI\n\r");
			change_led(LED,0,0);
		}

		//green light when user touches center of the sensor
		else if((tsi_value>=GREEN_LO) & (tsi_value<=GREEN_HI)) {
			PRINTF("You touched center side of TSI\n\r");
			change_led(0,LED,0);
		}

		//blue light when user touches right side of the sensor
		else if(tsi_value>=BLUE_LO) {
			PRINTF("You touched right side of TSI\n\r");
			change_led(0,0,LED);
		}
		Delay(DELAY);
	}
	change_led(0,0,0);					//turn off the led
	Delay(DELAY);
	PRINTF("Touch Sensor test complete\n\r");
}
