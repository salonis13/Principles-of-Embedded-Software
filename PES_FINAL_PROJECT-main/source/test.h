/*
 * test.h - Define functions to test accelerometer and touch sensor
 *
 *  Created on: May 3, 2021
 *      Author: saloni shah
 */

#ifndef TEST_H_
#define TEST_H_

/**
 ******************************************************************************
 * @brief	Function to test if correct register is initialized in I2C.
 * @param	none.
 * @retval none.
 ******************************************************************************
 */
uint8_t test_mma_device(void);

/**
 ******************************************************************************
 * @brief	Function to test accelerometer.
 * @param	none.
 * @retval none.
 ******************************************************************************
 */
void test_accel();

/**
 ******************************************************************************
 * @brief	Function to test touch sensor.
 * @param	none.
 * @retval none.
 ******************************************************************************
 */
void test_tsi();

#endif /* TEST_H_ */
