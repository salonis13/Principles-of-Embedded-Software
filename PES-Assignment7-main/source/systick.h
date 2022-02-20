/*
 * systick.h - for tone timing analysis
 *
 *  Created on: Apr 18, 2021
 *      Author: saloni
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

//typedef unsigned integer of 32 bits as ticktime_t
typedef uint32_t ticktime_t;

//function to initialise systick 
//Parameters: none
//Return type:void
void init_systick();

//systick interrupt handler
//Parameters: none
//Return type:void
void SysTick_Handler();

//function to reset systick timer
//Parameters: none
//Return type:void
void reset_timer();

//function to get systick time
//Parameters: none
//Return type:void
ticktime_t get_timer();

#endif /* SYSTICK_H_ */
