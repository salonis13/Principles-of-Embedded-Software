/*
 * sin.c - Calculate sin wave using integer math and testing sin function
 *
 *  Created on: Apr 18, 2021
 *      Author: salon
 */
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "fsl_debug_console.h"

//function description in sin.h
#include "sin.h"

#define TRIG_TABLE_STEPS 	32							//lookup table length
#define TRIG_TABLE_STEP_SIZE (HALF_PI/TRIG_TABLE_STEPS)	//step size in the lookup table

//lookup table array for sin function
static const int16_t sin_lookup[TRIG_TABLE_STEPS+1] =
	{ 0, 100, 199, 300, 397, 495, 591, 686, 779, 870, 960,
	  1047, 1131, 1213, 1292, 1367, 1440, 1509, 1574, 1635, 1693,
	  1747, 1796, 1841, 1881, 1917, 1949, 1976, 1998, 2015, 2027,
	  2034, 2037 };

int32_t fp_sin(int32_t i) {
	int32_t index;
	int sign=1;

	//add TWO_PI to input i if less than -PI to get -PI <= i <= PI
	while(i < -PI)
		i += TWO_PI;

	//subtract TWO_PI from input i if more than PI to get -PI <= i <= PI
	while(i > PI)
		i -= TWO_PI;

	//invert input i if negative to mirror the range i.e. [-PI, 0] to [0, PI]
	if(i < 0) {
		i = -i;
		sign = -1;
	}

	//mirror the range from [HALF_PI, PI] to [0, HALF_PI]
	if(i > HALF_PI)
		i = PI - i;

	//calculate index of i on lookup table
	index = (TRIG_TABLE_STEPS * i) / HALF_PI;
	//calculate value found on lookup table
	int32_t i1 = index * TRIG_TABLE_STEP_SIZE;

	//if they both match, return the output
	if(i1==i)
		return sign * sin_lookup[index];

	//if the values don't match, interpolation need
	//calculate the next value found on lookup table
	int32_t i2 = (index+1) * TRIG_TABLE_STEP_SIZE;
	//carry out interpolation
	int32_t interpolation = interpolate(i, i1, sin_lookup[index], i2, sin_lookup[index+1]);

	//return resultant interpolation value with appropriate sign
	return sign * interpolation;

}

int32_t interpolate(int32_t x, int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
	int32_t y;
	y = (((y2-y1)*(x-x1))/(x2-x1))+y1+1;		//y=mx+c
	return y;
}


void test_sin() {
	double act_sin;
	double exp_sin;
	double err;
	double sum_sq = 0;
	double max_err = 0;

	//calculate expected and actual sin values for -TWO_PI <= i <= TWO_PI
	for (int i=-TWO_PI; i <= TWO_PI; i++) {
		exp_sin = sin((double)i/TRIG_SCALE_FACTOR)*TRIG_SCALE_FACTOR;
		act_sin = fp_sin(i);

		//calculate error
		err = act_sin-exp_sin;

		//keep the maximum error updated
		if (err < 0)
			err = -err;
		if (err > max_err)
			max_err = err;

		//calculate sum of squares
		sum_sq += err*err;
	}

	//print resultant values on terminal
	PRINTF("max_err=%f  sum_sq=%f\n\r", max_err, sum_sq);
}

