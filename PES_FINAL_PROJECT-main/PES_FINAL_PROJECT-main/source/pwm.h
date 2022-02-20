/*
 * pwm.h - Define functions to utilize pulse width modulation for RGB LED
 *
 *  Created on: Apr 28, 2021
 *      Author: saloni shah
 */

#ifndef PWM_H_
#define PWM_H_

/**
 ******************************************************************************
 * @brief	Function to initialise PWM.
 * @param	none.
 * @retval none.
 ******************************************************************************
 */
void init_pwm();

/**
 ******************************************************************************
 * @brief	Function to change led colour using pwm.
 * @param	R, G, B values for red, green and blue led.
 * @retval none.
 ******************************************************************************
 */
void change_led(int R, int G, int B);

#endif /* PWM_H_ */
