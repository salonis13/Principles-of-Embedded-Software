/**
 * @file    pes_final_proj.c for PES ECEN 5813 Final Project Spring 2021
 * @brief   Main file to execute all project functionalities
 * @author: Saloni Shah.
 * @references: Dean Book (Chap 2, Chap 3, Chap 8)
 * 				Previous assignment implementations for systick, PWM, TSI
 * 				NXP Application note 3397 for implementing positioning algorithm using accelerometer
 */
#include <stdio.h>
#include <stdint.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

/* TODO: insert other include files here. */
#include <math.h>
#include "i2c.h"
#include "mma8451.h"
#include "delay.h"
#include "pwm.h"
#include "systick.h"
#include "tsi.h"
#include "test.h"
#include "calc.h"
#include "switch.h"

#define TOUCH_SENSE		70
#define POS_CONST		10000
#define DELAY			50

int touch_value=0;			//variable to store touch sensor value
int tilt_flag=0;			//variable to check board tilt
int switch_flag=0;			//variable to check switch status

signed long pos_value=0;
int ref_value=0;

signed int acc_X=0, acc_Y=0, acc_Z=0;		//variable for acceleration along all 3 axis

/**
 ******************************************************************************
 * @brief	Application entry point.
 * @param	none.
 * @retval	integer
 ******************************************************************************
 */
int main(void) {

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif

	PRINTF("PES FINAL PROJECT TERMINAL\n\r");

	//initialize switch gpio and port interrupt
	init_switch();

	//initialize i2c communication protocol for accelerometer
	init_i2c();

	//initialize PWM peripheral
	init_pwm();

	//initialize systick timer
	init_systick();

	//accelerometer test sequence
	test_accel();

	//touch sensor test sequence
	test_tsi();

	PRINTF("Keep the board still on a flat surface\n\r");

	while(1) {
		PRINTF("Touch the slider to set a reference position of board\n\r");

		while(1) {
			touch_value=touch_scan();

			//check if touch sensor is touched
			if(touch_value>=TOUCH_SENSE) {
				break;
			}
			Delay(DELAY);
		}

		//turn off the LED
		change_led(0,0,0);
		Delay(DELAY);

		//reset acceleration, velocity and position variables
		reinitialize();

		PRINTF("Calibrating accelerometer\n\r");

		//calibrate accelerometer (set reference position)
		ref_value=Calibrate();

		PRINTF("Calibration Done\n\r"
				" Y=%d\n\r", ref_value);

		//reset flags
		tilt_flag=0;
		switch_flag=0;

		Delay(DELAY);

		while(1) {

			//get current position of board based on accelerated movement
			pos_value=get_position();

			//check if tilt flag is set
			if(tilt_flag==1) {
				PRINTF("Please keep the board straight and re-calibrate\n\r");
				break;
			}

			//check if switch is pressed
			if(switch_flag==1) {
				break;
			}

			//print the position of board along Y-axis
			PRINTF("Position Y=%d\n\r",pos_value/POS_CONST);
		}
	}

	//unreachable code
	PRINTF("Should not be here\n\r");

	return 0 ;
}
