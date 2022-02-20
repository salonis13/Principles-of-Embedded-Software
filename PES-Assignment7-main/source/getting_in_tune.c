/**
 * @file    getting_in_tune.c
 * @brief   Application entry point.
 *
 * Main code to initialize all peripherals and run the implementation
 *
 * Author: Saloni Shah
 * Reference: Dean book and class notes
 */
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "autocorrelate.h"
#include "tpm.h"
#include "systick.h"
#include "sin.h"
#include "audio_out.h"
#include "audio_in.h"
/* TODO: insert other definitions and declarations here. */
#define PLAYBACK_TIME 830

//Function to initialize peripherals
//Parameters: none
//Return type: void
void initialization() {
	Init_DAC();				//initialize digital to analog conversion peripheral
	tone_to_samples();		//store values of tone waveforms in buffer
	Init_ADC();				//initialize analog to digital conversion peripheral
	Init_TPM0(500);			//initialize TPM0 peripheral
	Start_TPM0();			//start TPM0
	Init_TPM1();			//initialize TPM1 peripheral
	Start_TPM1();			//start TPM1
	init_systick();			//initialize systick timer
}

/*
 * @brief   Application entry point.
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

    PRINTF("Hello Pete...Get in Tune!!\n\r");				//debug print statement on UART

    test_sin();								//function to test fp_sin function

    initialization();						//initialize all peripherals

here:	audio_output();				//start playing a tone

	while(1) {
		if(get_timer()>=PLAYBACK_TIME) {	//check systick timer value
				goto here;					//if time reaches specified value change the tone
		}
		audio_input();						//call function to convert received data and store it in buffer
		audio_analysis();						//analyse the received buffer
	}

    return 0;
}
