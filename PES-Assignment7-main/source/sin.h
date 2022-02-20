/*
 * sin.h - Calculate sin wave using integer math and testing sin function
 *
 *  Created on: Apr 18, 2021
 *      Author: saloni
 */

#ifndef SIN_H_
#define SIN_H_

#define TRIG_SCALE_FACTOR 	2037				//scaling factor for sine
#define HALF_PI 			3200				//value of pi/2
#define PI 					6399				//value of pi
#define TWO_PI 				12799				//value of 2*pi

//function to calculate sine value using integer math
//Parameters: int32_t i - value for which sin output is to be calculated
//Return type: int32_t - returns the resulting value of sin
int32_t fp_sin(int32_t i);

//function to carry out interpolation for a lookup-table
// This function calculates interpolated value using
//	linear interpolation equation y = mx + c
//	Here, m is slope = (y2-y1)/(x2-x1) and c is a constant
//	which is y1 in our case and x is (x-x1)
//Parameters - x (value of i)
//			   x1 (calculated value from lookup table)
//			   y1 (index of x1 on lookup table)
//			   x2 (calculated value of index+1 on lookup table)
//			   y2 (index of x2 on lookup table)
//Return type: int32_t - returns the resultant interpolation value
int32_t interpolate(int32_t x, int32_t x1, int32_t y1, int32_t x2, int32_t y2);

//function to test fp_sin function
//Parameters: none
//Return type: void
void test_sin();


#endif /* SIN_H_ */
