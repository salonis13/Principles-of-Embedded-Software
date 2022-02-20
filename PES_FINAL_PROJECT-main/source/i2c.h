/*
 * i2c.h - Define functions for I2C communication protocol
 *
 *  Created on: Apr 28, 2021
 *      Author: saloni shah
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

/**
 ******************************************************************************
 * @brief	function to initialize I2C communication.
 * @param	none.
 * @retval	none.
 ******************************************************************************
 */
void init_i2c(void);

/**
 ******************************************************************************
 * @brief	function to send I2C start sequence.
 * @param	none.
 * @retval	none.
 ******************************************************************************
 */
void i2c_start(void);

/**
 ******************************************************************************
 * @brief	function to setup I2C read for required device.
 * @param	dev - device address (here mma8451).
 * 		address - register address
 * @retval	none.
 ******************************************************************************
 */
void i2c_read_setup(uint8_t dev, uint8_t address);

/**
 ******************************************************************************
 * @brief	function to read bytes from I2C.
 * @param	integer 0 if byte to be read is last in sequence and 1 if not.
 * @retval	integer data on that byte.
 ******************************************************************************
 */
uint8_t i2c_repeated_read(uint8_t);

/**
 ******************************************************************************
 * @brief	function to read data from single byte on I2C.
 * @param	dev - device address (here mma8451).
 * 		address - register address
 * @retval	integer data read from I2C
 ******************************************************************************
 */
uint8_t i2c_read_byte(uint8_t dev, uint8_t address);

/**
 ******************************************************************************
 * @brief	function to write data on a single byte on I2C.
 * @param	dev - device address (here mma8451).
 * 		address - register address
 * 		data - data to be written
 * @retval	none
 ******************************************************************************
 */
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data);

#endif /* I2C_H_ */
