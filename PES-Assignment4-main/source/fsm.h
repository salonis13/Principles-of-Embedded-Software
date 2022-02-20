/*
 * fsm.h
 *
 * This file gives an outline of the systick functions, finite state machine
 * as well as the rgb led display
 *  Created on: Mar 6, 2021
 *      Author: Saloni Shah
 */

#ifndef FSM_H_
#define FSM_H_

#define FREQ_TIMER 48000000L 	//systick internal clock has the frequency of 48MHz
#define TICKS 100				//number of ticks in 1 sec
#define TIME 48000				//normalise load value to get time in msecs

#define STOP_R 97			//red led portion decimal value of STOP sign
#define STOP_G 30			//green led portion decimal value of STOP sign
#define STOP_B 60			//blue led portion decimal value of STOP sign
#define GO_R 34				//red led portion decimal value of GO sign
#define GO_G 150			//green led portion decimal value of GO sign
#define GO_B 34				//blue led portion decimal value of GO sign
#define WAR_R 255			//red led portion decimal value of WARNING sign
#define WAR_G 178			//green led portion decimal value of WARNING sign
#define WAR_B 0				//blue led portion decimal value of WARNING sign
#define CROSS_R 0			//red led portion decimal value of CROSSWALK sign
#define CROSS_G 16			//green led portion decimal value of CROSSWALK sign
#define CROSS_B 48			//blue led portion decimal value of CROSSWALK sign
#define CROSS_ON 75			//CROSSWALK led blink on time 750 msec
#define CROSS_OFF 25		//CROSSWALK led blink off time 250 msec
#define TRANS_TIMEOUT 100	//led transition from one colour to another time of 1000 msec

//typedef unsigned integer of 32 bits as ticktime_t
typedef uint32_t ticktime_t;

/*
 *  Function to initialize the Systick timer in the system
 *  			and also calculate the time and load value
 *  Function return type: void
 *  Parameters passed: none
 */
void init_systick();

/*
 * Systick handler is an interrupt which is triggered
 * everytime the systick timer overflows after 10 msec
 * 		In this function we are appending the system time
 * 		and poll_time and trans variables used to monitor led status
 *  Function return type: void
 *  Parameters passed: none
 */
void SysTick_Handler();

/*
 * Function to get the current system time since startup
 * Function return type: total_time (unsigned integer)
 * Parameters passed: none
 */
ticktime_t now();

/*
 * Function to reset timer and set get_time to 0
 * Function return type: void
 * Parameters passed: none
 */
void reset_timer();

/*
 * Function to get the system time since reset
 * Function return type: get_time (unsigned integer)
 * Parameters passed: none
 */
ticktime_t get_timer();

/*
 * Function to define different led states
 * and its transition. For detailed picture of the FSM, please refer state_machine.pdf
 * Function return type: void
 * Parameters passed: none
 */
void state_machine();

/*
 * Function to perform led display and led transition
 * 		In this function, the current rgb values and desired rgb values are passed.
 * 		Also required led display or transition time is passed
 * 	Function return type: void
 * 	Parameters passed:
 * 		now_r, now_g, now_b    			RGB decimal values of current led color
 * 		next_r, next_g, next_b			RGB decimal values of desired led color
 * 		n								TIMEOUT for STOP and GO state
 * 										WAR_TIMEOUT for WARNING state
 * 										TRANS_TIMEOUT for led transition
 * 										CROSS_ON and CROSS_OFF for CROSSWALK state
 */
void main_loop(float now_r, float now_g, float now_b, float next_r, float next_g, float next_b, int n);

#endif /* FSM_H_ */
