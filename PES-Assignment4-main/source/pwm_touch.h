/*
 * pwm_touch.h
 *
 *	This file initializes the pwm and touch slider functions
 *  Created on: Mar 7, 2021
 *      Author: Saloni Shah
 */

#ifndef PWM_TOUCH_H_
#define PWM_TOUCH_H_

#define OFFSET 600 			//offset to normalize touch slider value

/*
 *  Function to clock gate and initialize touch slider
 *  to detect any touches on capacitive slider
 *  Function return type: void
 *  Parameters passed: none
 */
void init_touch();

/*
 * Function to get the touch slider value
 * Function return type: normalised slider value (integer)
 * Parameters passed: none
 */
int touch_scan();

/*
 * Function to initialise PWM
 * Function return type: void
 * Parameters passed: none
 */
void init_pwm();

#endif /* PWM_TOUCH_H_ */
