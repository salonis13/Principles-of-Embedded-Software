/*
 * systick.c - for tone timing analysis
 *
 *  Created on: Apr 18, 2021
 *      Author: saloni
 */
#include "MKL25Z4.h"

#include "systick.h"
#include "tpm.h"

#define TICKS 100				//number of ticks in 1 sec
#define TIME 48000				//normalise load value to get time in msecs

int trans=0;					//variable used to monitor led timeout value
ticktime_t time=0;						//tick time
ticktime_t total_time=0;		//total time after system startup
ticktime_t get_time=0;			//system time after timer is reset
int poll_time=0;			//variable used to poll the touch slider

void init_systick()  {
	SysTick->LOAD = (CLOCK/TICKS)-1;		//load systick timer with value 479,999 which will be reloaded everytime systick overflows
	SysTick->VAL=0;								//set current systick value to 0
	time=((CLOCK/TICKS)/TIME);				//calculate tick time to be 10 msec
	NVIC_SetPriority(SysTick_IRQn,3);			//set systick interrupt priority as 3
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk 	//counting down the systick load value to 0 will enable systick exception
					| SysTick_CTRL_ENABLE_Msk 	//enable counter
					| SysTick_CTRL_CLKSOURCE_Msk;	//selects clock source to be processor clock
}

void SysTick_Handler()  {
	get_time += time;			//append system time after timer is reset by 10 msec
}

void reset_timer()  {
	get_time=0;					//reset timer and set get time to 0
}

ticktime_t get_timer()  {
	return get_time;			//return the system time after timer is reset
}

