/*
 * tsi.h - Define functions for capacitive touch sensor
 *
 *  Created on: Apr 29, 2021
 *      Author: saloni shah
 */

#ifndef TSI_H_
#define TSI_H_

/**
 ******************************************************************************
 * @brief	Function to clock gate and initialize
 *			 the capacitive touch slider present on FRDM KL25Z development board
 * @param	none.
 * @retval none.
 ******************************************************************************
 */
void init_touch();

/**
 ******************************************************************************
 * @brief	Function to get values from capacitive touch sensor.
 * @param	none.
 * @retval integer value from touch sensor.
 ******************************************************************************
 */
int touch_scan(void);


#endif /* TSI_H_ */
