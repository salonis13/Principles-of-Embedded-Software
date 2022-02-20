/*
 * mma8451.h - Define functions to get values from accelerometer
 *
 *  Created on: Apr 28, 2021
 *      Author: saloni shah
 */

#ifndef MMA8451_H_
#define MMA8451_H_

#include <stdint.h>

/**
 ******************************************************************************
 * @brief	function to initialize accelerometer.
 * @param	none.
 * @retval	return 1 on successful initialization.
 ******************************************************************************
 */
int init_mma(void);

/**
 ******************************************************************************
 * @brief	function to get acceleration values along X axis.
 * @param	none.
 * @retval	none.
 ******************************************************************************
 */
void read_full_xyz_1(void);

/**
 ******************************************************************************
 * @brief	function to get acceleration values along Y axis.
 * @param	none.
 * @retval	none.
 ******************************************************************************
 */
void read_full_xyz_2(void);

/**
 ******************************************************************************
 * @brief	function to convert acceleration values to rotation around front to back axis.
 * @param	none.
 * @retval	float value of roll.
 ******************************************************************************
 */
float convert_to_roll();

/**
 ******************************************************************************
 * @brief	function to convert acceleration values to rotation around side to side axis.
 * @param	none.
 * @retval	float value of pitch
 ******************************************************************************
 */
float convert_to_pitch();


#endif /* MMA8451_H_ */
