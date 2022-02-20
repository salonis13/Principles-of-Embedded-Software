/*
 * calc.c - Define functions to calculate board position
 *
 *  Created on: May 3, 2021
 *      Author: saloni shah
 */
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

#include "calc.h"
#include "mma8451.h"
#include "pwm.h"
#include "delay.h"

#define CALIBRATE_SAMPLES 	1024
#define END_SAMPLES			5
#define AVG_SAMPLES			64
#define MECH_FILTER			200
#define TILT_ERR			15
#define DELAY				50
#define RED					255

extern int tilt_flag;		//variable to check tilt

int sstatey=0;				//variable for calibrated accelerometer value

signed int  accelerationy[2];	//array of accelerometer data
signed long velocityy[2];		//array of integrated velocity data
signed long positionY[2];		//array of integrated position data

unsigned char county;

extern int acc_Y;

float tilt=0.0;

//take average value of 1024 accelerometer data samples to get a reference point
int Calibrate(void) {
	acc_Y=0;
	unsigned int count1;
	count1 = 0;

	//accumulate 1024 samples of accelerometer
	while(count1<CALIBRATE_SAMPLES) {
		read_full_xyz_2();
		sstatey = sstatey + acc_Y;
		count1++;
	}

	//average received samples
	sstatey=(sstatey/CALIBRATE_SAMPLES);

	return sstatey;
}

//check movement end of board
void movement_end_check(void) {

	//count number of acceleration samples which equal to zero
	if (accelerationy[1]==0) {
		county++;
	}
	else {
		county =0;
	}

	//if more than 5 samples are zero, assume that velocity is also zero
	if(county>=END_SAMPLES) {
		velocityy[1]=0;
		velocityy[0]=0;
	}
}

//data filter and mechanical filter to attenuate noise in accelerometer data
void data_filter() {
	unsigned char count ;
	count=0;

	//filter routine for noise attenuation
	//takes 64 samples of acceleration data for one instant
	while(count<AVG_SAMPLES) {
		read_full_xyz_2();
		tilt=convert_to_pitch();
		accelerationy[1]=accelerationy[1] + acc_Y;
		count++;
	}

	//average the 64 samples to get acceleration value
	accelerationy[1]= accelerationy[1]/AVG_SAMPLES;

	//eliminating zero reference offset of the acceleration data
	accelerationy[1] = accelerationy[1] - sstatey;

	//Mechanical filter applied to the acceleration variable
	if ((accelerationy[1] <=MECH_FILTER)&&(accelerationy[1] >= -(MECH_FILTER))) {
		accelerationy[1] = 0;
	}
}

//double integrate acceleration value to estimate position
int get_position() {

	//filter accelerometer data
	data_filter();

	//check tilt of board
	if((tilt>=TILT_ERR) | (tilt<=(-TILT_ERR))) {
		change_led(RED, 0, 0);
		Delay(DELAY);
		tilt_flag=1;
		return 0;
	}

	//first Y integration:
	velocityy[1] = velocityy[0]+(accelerationy[0]) + (((accelerationy[1] -accelerationy[0]))/2);
	//second Y integration:
	positionY[1] = positionY[0]+(velocityy[0]) + (((velocityy[1] - velocityy[0]))/2);

	//The current acceleration value must be sent to the
	//previous acceleration value
	accelerationy[0] = accelerationy[1];

	//Same done for the velocity variable
	velocityy[0] = velocityy[1];

	//check for movement end of board
	movement_end_check();

	//actual position data must be sent to the previous position
	positionY[0] = positionY[1];

	//return position value
	return positionY[1];
}

//reset all variables
void reinitialize() {
	positionY[0]=0;
	positionY[1]=0;
	velocityy[0]=0;
	velocityy[1]=0;
	accelerationy[0]=0;
	accelerationy[1]=0;
}
