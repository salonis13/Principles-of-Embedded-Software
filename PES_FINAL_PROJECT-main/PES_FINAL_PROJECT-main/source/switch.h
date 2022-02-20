/*
 * switch.h - Define functions to use interrupt driven push-button
 *
 *  Created on: May 3, 2021
 *      Author: saloni shah
 */

#ifndef SWITCH_H_
#define SWITCH_H_

/**
 ******************************************************************************
 * @brief	Function to enable interrupt for push-button pin .
 * @param	none.
 * @retval none.
 ******************************************************************************
 */
void init_interrupt(void);

/**
 ******************************************************************************
 * @brief	Function to initialize push-button pin and port.
 * @param	none.
 * @retval none.
 ******************************************************************************
 */
void init_switch();

/**
 ******************************************************************************
 * @brief	PORTD interrupt handler.
 * @param	none.
 * @retval none.
 ******************************************************************************
 */
void PORTD_IRQHandler(void);

#endif /* SWITCH_H_ */
