/*
 * systick.h - Define functions for systick timer
 *
 *  Created on: Apr 28, 2021
 *      Author: saloni shah
 */
#include<stdint.h>
#include"MKL25Z4.h"

//typedef unsigned integer of 32 bits as ticktime_t
typedef uint32_t ticktime_t;

/**
 ******************************************************************************
 * @brief	Function to initialize the Systick timer in the system
 *  			and also calculate the time and load value.
 * @param	none.
 * @retval none.
 ******************************************************************************
 */
void init_systick();

/**
 ******************************************************************************
 * @brief	Systick handler is an interrupt which is triggered
 * 		everytime the systick timer overflows after 10 msec
 * 		In this function we are appending the system time
 * 		and poll_time and trans variables used to monitor led status
 * @param	none.
 * @retval none.
 ******************************************************************************
 */
void SysTick_Handler();

/**
 ******************************************************************************
 * @brief	Function to get the current system time since startup
 * @param	none.
 * @retval total_time (unsigned integer).
 ******************************************************************************
 */
ticktime_t now();

/**
 ******************************************************************************
 * @brief	Function to reset timer and set get_time to 0
 * @param	none.
 * @retval none.
 ******************************************************************************
 */
void reset_timer();

/**
 ******************************************************************************
 * @brief	Function to get the system time since reset
 * @param	none.
 * @retval get_time (unsigned integer).
 ******************************************************************************
 */
ticktime_t get_timer();

