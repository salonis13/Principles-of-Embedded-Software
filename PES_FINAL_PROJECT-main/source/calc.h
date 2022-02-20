/*
 * calc.h - Define functions to calculate board position
 *
 *  Created on: May 3, 2021
 *      Author: saloni shah
 */

#ifndef CALC_H_
#define CALC_H_

/**
 ******************************************************************************
 * @brief	calibrate acceleromemter (set reference point).
 * @param	none.
 * @retval	integer value of calibration
 ******************************************************************************
 */
int Calibrate(void);

/**
 ******************************************************************************
 * @brief	function to check movement end of board.
 * @param	none.
 * @retval	none.
 ******************************************************************************
 */
void movement_end_check(void);

/**
 ******************************************************************************
 * @brief	function to filter accelerometer data to eliminate noise.
 * @param	none.
 * @retval	none.
 ******************************************************************************
 */
void data_filter();

/**
 ******************************************************************************
 * @brief	function to double integrate accelerometer data to estimate position.
 * @param	none.
 * @retval	integer value of position on Y axis
 ******************************************************************************
 */
int get_position();

/**
 ******************************************************************************
 * @brief	function to reset all position parameters.
 * @param	none.
 * @retval	none.
 ******************************************************************************
 */
void reinitialize();

#endif /* CALC_H_ */
