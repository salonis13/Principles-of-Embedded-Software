/*
 * audio_in.c - Initialize ADC, calibrate ADC, take audio input and analyse
 * 						the audio input
 *
 *  Created on: Apr 18, 2021
 *      Author: saloni
 */
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

#include"audio_out.h"
#include"autocorrelate.h"
#include"tpm.h"

//define ADC module pin and channel
#define ADC_POS 20
#define ADC_INPUT_CHANNEL 23

uint16_t buffer[1024];
int s=0;

int min=0,max=0,avg=0;
int correlate;

void delay(int n) {
	for(int i=0; i<n; i++) {
		__asm("nop");
		for(int k=0; k<=2;k++);
	}
}

uint16_t min_max(uint16_t *a,int n) {
 	int i,sum=0;

 	min=max=a[0];

    //update minimum and maximum value in buffer
    for(i=1; i<=n; i++) {
    	if(min>a[i])
    		min=a[i];
        if(max<a[i])
        	max=a[i];

        //take sum of all values in buffer
        sum = sum + a[i];
    }

    //calculate average value in buffer
    avg = sum/i;
    return 0;
 }

void calibrate() {
	uint16_t calibrate=0;

    //set ADC calibration flag to start calibration
    ADC0->SC3 |= ADC_SC3_CAL_MASK;

    //check if calibration failed
    if((ADC0->SC3 & 0x0040))
    	PRINTF("Calibration failed\n\r");

    else {

    	//Add the plus-side calibration results CLP0, CLP1, CLP2, CLP3, CLP4, and CLPS
    	calibrate = ADC0->CLP0 + ADC0->CLP1 + ADC0->CLP2 + ADC0->CLP3 + ADC0->CLP4 + ADC0->CLPS;

    	//Divide by two.
    	calibrate /= 2;

    	//Set the MSB
    	calibrate+=0x8000;

    	//Store the value in the plus-side gain calibration register PG.
    	ADC0->PG = calibrate;

    	PRINTF("Plus calibration-%d\n\r",ADC0->PG);

    	calibrate=0;
    	//Repeat the procedure for the minus-side gain calibration value
    	calibrate = ADC0->CLM0 + ADC0->CLM1 + ADC0->CLM2 + ADC0->CLM3 + ADC0->CLM4 + ADC0->CLMS;
        calibrate = (calibrate>>1);
        calibrate+=0x8000;
        ADC0->MG = calibrate;

        PRINTF("Minus calibration-%d\n\r",ADC0->MG);
    }
}

void Init_ADC(void) {

	//gate clock of ADC0 module
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;

	// Select analog for pin
	PORTE->PCR[ADC_POS] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[ADC_POS] |= PORT_PCR_MUX(0);

	//16 bit single-ended conversion, bus clock input, divide bus by 4
	ADC0->CFG1 = ADC_CFG1_MODE(3)
						| ADC_CFG1_ADICLK(0)
						| ADC_CFG1_ADIV(2);

	//set ADC channel input as DAC output
	ADC0->SC1[0] |= ADC_SC1_ADCH(ADC_INPUT_CHANNEL);

	//calibrate ADC
	calibrate();

	//select hardware trigger of ADC
	//set trigger to TPM1 overflow
	SIM->SOPT7 |= SIM_SOPT7_ADC0ALTTRGEN(1)
						| SIM_SOPT7_ADC0TRGSEL(9)
						| SIM_SOPT7_ADC0PRETRGSEL(0);
}

void audio_input() {

	//store 1024 samples in buffer
	for(s=0; s<1024; s++) {

		//start conversion
		ADC0->SC1[0] = ADC_INPUT_CHANNEL;

		//poll conversion complete flag
		while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
			;
		delay(3);

		//store converted input in buffer
		buffer[s] = ADC0->R[0];
	}
}

void audio_analysis() {

	//calculate max, min and avg value in buffer
	min_max(buffer,s);

	//compute autocorrelated period
	correlate=autocorrelate_detect_period(buffer, s, kAC_16bps_unsigned);

	//print out analysis of audio signal on terminal
	PRINTF("min=%d max=%d avg=%d period=%d samples frequency=%d Hz\n\r",
			   min,   max,    avg,     correlate,         (TPM_OVERFLOW/correlate));
}
