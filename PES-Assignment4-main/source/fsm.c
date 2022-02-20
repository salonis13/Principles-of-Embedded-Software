/*
 * fsm.c
 *
 * This file initializes systick, defines the state machine and led display functions
 *  Created on: Mar 6, 2021
 *      Author: Saloni Shah
 */
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "core_cm0plus.h"

#include "log.h"
#include "fsm.h"
#include "pwm_touch.h"

float portionR=0;			//variable calculating red portion of led during transition
float portionG=0;			//variable calculating green portion of led during transition
float portionB=0;			//variable calculating blue portion of led during transition
int cross_loop=0;			//variable to count led blinking in CROSSWALK state
int n=0;					//variable for led timeout in different states
float phase=0;				//variable for led transition from one colour to another
float now_r=0,now_g=0,now_b=0;	//decimal values of current led
float next_r=0,next_g=0,next_b=0;	//decimal values of led colour to be transitioned into
int trans=0;					//variable used to monitor led timeout value
ticktime_t time=0;						//tick time
ticktime_t total_time=0;		//total time after system startup
ticktime_t get_time=0;			//system time after timer is reset
int poll_time=0;			//variable used to poll the touch slider

void init_systick()  {
	SysTick->LOAD = (FREQ_TIMER/TICKS)-1;		//load systick timer with value 479,999 which will be reloaded everytime systick overflows
	SysTick->VAL=0;								//set current systick value to 0
	time=((FREQ_TIMER/TICKS)/TIME);				//calculate tick time to be 10 msec
	NVIC_SetPriority(SysTick_IRQn,3);			//set systick interrupt priority as 3
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk 	//counting down the systick load value to 0 will enable systick exception
					| SysTick_CTRL_ENABLE_Msk 	//enable counter
					| SysTick_CTRL_CLKSOURCE_Msk;	//selects clock source to be processor clock
}

void SysTick_Handler()  {
	total_time += time;			//append total time after system startup by 10 msec
	get_time += time;			//append system time after timer is reset by 10 msec
	poll_time++;				//append variable used to poll the touch slider
	trans++;					//append variable used to monitor led timeout value
}

ticktime_t now() {
	return total_time;			//return total timer after system startup
}

void reset_timer()  {
	get_time=0;					//reset timer and set get time to 0
}

ticktime_t get_timer()  {
	return get_time;			//return the system time after timer is reset
}

/*
 * enumeration of all led states
 */
typedef enum {
	STOP,
	GO,
	WARNING,
	CROSSWALK
}state_t;

/*
 * structure to define the current led state
 */
static struct {
	 state_t cur_state;
}signal = {
	.cur_state = STOP,
};

void state_machine()  {
	switch(signal.cur_state)								//check the current led state
	{
	case STOP:												//check whether cur_state is STOP
		main_loop(STOP_R, STOP_G, STOP_B,0,0,0, TIMEOUT);	//stay in STOP state for 5 sec in debug mode and 20 sec in production mode
		if(signal.cur_state==CROSSWALK)						//check if cur_state has changed to CROSSWALK
			{break;}
		LOG("State change from STOP to GO - %d msec since startup\n\r",now());		//print statement for state change
		main_loop(STOP_R, STOP_G, STOP_B, GO_R, GO_G, GO_B, TRANS_TIMEOUT);		//transition from STOP sign to GO sign
		if(signal.cur_state==CROSSWALK)						//check if cur_state has changed to CROSSWALK
			{break;}
		signal.cur_state = GO;								//set cur_state as GO
	break;

	case GO:												//check whether cur_state is GO
		main_loop(GO_R, GO_G, GO_B,0,0,0, TIMEOUT);			//stay in GO state for 5 sec in debug mode and 20 sec in production mode
		if(signal.cur_state==CROSSWALK)						//check if cur_state has changed to CROSSWALK
			{break;}
		LOG("State change from GO to WARNING - %d msec since startup\n\r",now());	//print statement for state change
		main_loop(GO_R, GO_G, GO_B, WAR_R, WAR_G, WAR_B, TRANS_TIMEOUT);		//transition from GO sign to WARNING sign
		if(signal.cur_state==CROSSWALK)						//check if cur_state has changed to CROSSWALK
			{break;}
		signal.cur_state = WARNING;							//set cur_state as WARNING
	break;

	case WARNING:											//check whether cur_state is WARNING
		main_loop(WAR_R, WAR_G, WAR_B,0,0,0, WAR_TIMEOUT);	//stay in WARNING state for 3 sec in debug mode and 5 sec in production mode
		if(signal.cur_state==CROSSWALK)						//check if cur_state has changed to CROSSWALK
			{break;}
		LOG("State change from WARNING to STOP - %d msec since startup\n\r",now());	//print statement for state change
		main_loop(WAR_R, WAR_G, WAR_B, STOP_R, STOP_G, STOP_B, TRANS_TIMEOUT);	//transition from WARNING sign to STOP sign
		if(signal.cur_state==CROSSWALK)						//check if cur_state has changed to CROSSWALK
			{break;}
		signal.cur_state = STOP;							//set cur_state as STOP
	break;

	case CROSSWALK:											//check whether cur_state is CROSSWALK
		if(portionR==97 && portionG==30 && portionB==60)	//check if current led state was STOP
			LOG("State change from STOP to CROSSWALK - %d msec since startup\n\r",now());	//if so led transition will be from STOP to CROSSWALK
		else if(portionR==34 && portionG==150 && portionB==34)	//check if current led state was GO
			LOG("State change from GO to CROSSWALK - %d msec since startup\n\r",now());	//if so led transition will be from GO to CROSSWALK
		else if(portionR==255 && portionG==178 && portionB==0)	//check if current led state was WARNING
			LOG("State change from WARNING to CROSSWALK - %d msec since startup\n\r",now());	//if so led transition will be from WARNING to CROSSWALK
		else
			LOG("State change to CROSSWALK - %d msec from startup\n\r",now());		//if led was already in transition state, led will enter CROSSWALK state
		main_loop(portionR, portionG, portionB, CROSS_R, CROSS_G, CROSS_B, TRANS_TIMEOUT);	//transition from current led state to CROSSWALK state
		for(cross_loop=0; cross_loop<10; cross_loop++) {		//loop to blink led 10 times
				main_loop(0,0,0,0,0,0,CROSS_OFF);				//keep crosswalk led off for 250 msec
				main_loop(CROSS_R, CROSS_G, CROSS_B,0,0,0,CROSS_ON); }	//keep crosswalk led on for 750 msec
		cross_loop=0;											//reset loop variable
		LOG("State change from CROSSWALK to GO - %d msec since startup\n\r",now());	//print statement for state change
		main_loop(CROSS_R, CROSS_G, CROSS_B, GO_R, GO_G, GO_B, TRANS_TIMEOUT);			//transition from CROSSWALK sign to GO sign
			signal.cur_state = GO;								//set cur_state as GO
	break;
	}
}

void main_loop(float now_r, float now_g, float now_b, float next_r, float next_g, float next_b, int n) {
	if(next_r==0 && next_g==0 && next_b==0)			//check if led is to stay in same state
	{
	while(trans<n) {								//stay in same led state until timeout
		if(poll_time>=5 && signal.cur_state!=CROSSWALK) {	//poll touch slider every 50msec and check if led is already in CROSSWALK state
			//touch_value=touch_scan();				//get touch slider value
			poll_time=0;
			if(touch_scan() >=75) {					//check if the slider is touched
			LOG("Button press detected - %d msec since startup\n\r",now());	//print statement to show button press detected
			signal.cur_state=CROSSWALK;				//change cur_state to CROSSWALK and return to state machine
			n=0; trans=0;
			return; } }
		portionR = now_r;							//set current RGB value
	    portionG = now_g;
		portionB = now_b;
	    TPM2->CONTROLS[0].CnV  = (portionR*TPM2->MOD)/255;	//change led color to desired state
		TPM2->CONTROLS[1].CnV  = (portionG*TPM2->MOD)/255;
		TPM0->CONTROLS[1].CnV  = (portionB*TPM0->MOD)/255;
	}
	n=0; trans=0;
	}
	else  {										//if led is to be transitioned from one color to another
		while(trans<n && phase<=1)  {			//transition until timeout
			if(poll_time>=5 && signal.cur_state!=CROSSWALK) {	//poll touch slider every 50msec and check if led is already in CROSSWALK state
				//touch_value=touch_scan();		//get touch slider value
				poll_time=0;
				if(touch_scan() >=75) {			//check if the slider is touched
				LOG("Button press detected - %d msec since startup\n\r",now());	//print statement to show button press detected
				signal.cur_state=CROSSWALK;		//change cur_state to CROSSWALK and return to state machine
				n=0; trans=0;
				phase=0;
				return; } }
			portionR = ((next_r-now_r)*(phase))+now_r;	//formula to set RGB portions
			portionG = ((next_g-now_g)*(phase))+now_g;
			portionB = ((next_b-now_b)*(phase))+now_b;
		    TPM2->CONTROLS[0].CnV  = (portionR*TPM2->MOD)/255;	//change led colour
			TPM2->CONTROLS[1].CnV  = (portionG*TPM2->MOD)/255;
			TPM0->CONTROLS[1].CnV  = (portionB*TPM0->MOD)/255;
			phase += 0.00005;							//increment phase value to get next color state in transition
		}
		n=0; trans=0;
		phase=0;
	}
	return;
}
