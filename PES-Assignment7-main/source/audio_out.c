/*
 * audio_out.c - Initialize DAC, DMA, store samples in buffer
 * 					 and play tones
 *
 *  Created on: Apr 18, 2021
 *      Author: salon
 */
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

#include"audio_out.h"
#include"sin.h"
#include"systick.h"
#include"audio_in.h"

//pin number of DAC output
#define DAC_POS 30

//number of samples to be taken for each tone
#define SAMPLE_A4 109
#define SAMPLE_D5 82
#define SAMPLE_E5 73
#define SAMPLE_A5 54

//frequency of each tone
#define TONE_A4 440
#define TONE_D5 585
#define TONE_E5 659
#define TONE_A5 880

//buffers for all tones
uint16_t sin_table_A4[SAMPLE_A4];
uint16_t sin_table_D5[SAMPLE_D5];
uint16_t sin_table_E5[SAMPLE_E5];
uint16_t sin_table_A5[SAMPLE_A5];

uint16_t *sin_table;
uint16_t SAMPLE_SIZE;
uint16_t TONE_SIZE;

//reload values in DMA
uint16_t *Reload_DMA_Source;
uint32_t Reload_DMA_Byte_Count;

int d=0;

void Init_DAC() {

	//clock gating DAC module and PORTE
	SIM->SCGC6 |= SIM_SCGC6_DAC0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	// Select analog for pin
	PORTE->PCR[DAC_POS] &= ~(PORT_PCR_MUX(7));

	// Disable buffer mode
	DAC0->C1 = 0;
	DAC0->C2 = 0;

	// Enable DAC, select VDDA as reference voltage
	DAC0->C0 = DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK;
}

void tone_to_samples() {
	int32_t n=0;
	int32_t sample=0;
	int i=0;

	//store samples for tone A4 in buffer
	for(n=0; n<=TWO_PI; n+=(TWO_PI/SAMPLE_A4)) {
		sample=fp_sin(n);							//calculate sin value of the sample
		sample+=TRIG_SCALE_FACTOR;					//scale the sample value to get positive number
		sin_table_A4[i]=sample;						//store scaled sample value in buffer
		i++;
	}
	i=0;

	//store samples for tone D5 in buffer
	for(n=0; n<=TWO_PI; n+=(TWO_PI/SAMPLE_D5)) {
		sample=fp_sin(n);
		sample+=TRIG_SCALE_FACTOR;
		sin_table_D5[i]=sample;
		i++;
	}
	i=0;

	//store samples for tone E5 in buffer
	for(n=0; n<=TWO_PI; n+=(TWO_PI/SAMPLE_E5)) {
		sample=fp_sin(n);
		sample+=TRIG_SCALE_FACTOR;
		sin_table_E5[i]=sample;
		i++;
	}
	i=0;

	//store samples for tone A5 in buffer
	for(n=0; n<=TWO_PI; n+=(TWO_PI/SAMPLE_A5)) {
		sample=fp_sin(n);
		sample+=TRIG_SCALE_FACTOR;
		sin_table_A5[i]=sample;
		i++;
	}
}

void check_tone(int i) {

	//update required buffer and sample size as per the value of i
	if(i==1) {
		sin_table = sin_table_A4;
		SAMPLE_SIZE = SAMPLE_A4;
		TONE_SIZE = TONE_A4;
	}
	else if(i==2) {
		sin_table = sin_table_D5;
		SAMPLE_SIZE = SAMPLE_D5;
		TONE_SIZE = TONE_D5;
	}
	else if(i==3) {
		sin_table = sin_table_E5;
		SAMPLE_SIZE = SAMPLE_E5;
		TONE_SIZE = TONE_E5;
	}
	else if(i==4) {
		sin_table = sin_table_A5;
		SAMPLE_SIZE = SAMPLE_A5;
		TONE_SIZE = TONE_A5;
		d=0;							//set d=0 to start playback from first tone
	}
}

void audio_output() {

	//increment tone number after every tone plays for 1 second and update tone values
	d++;
	check_tone(d);

	//reset systick timer
	reset_timer();

	//statement to show computations for a tone
	PRINTF("\n\rGenerated %d samples at %d Hz computed period=%d samples\n\r"
			,    SAMPLE_SIZE,    TONE_SIZE,            SAMPLE_SIZE);

	//initialize DMA and play the tone
	init_dma_for_playback(sin_table, SAMPLE_SIZE);
	start_dma_playback();
}

void init_dma_for_playback(uint16_t *source, uint32_t count) {

	//save reload information
	Reload_DMA_Source = source;
	Reload_DMA_Byte_Count = count*2;

	//gate clocks to DMA and DMAMUX
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	//disable DMA channel
	DMAMUX0->CHCFG[0] = 0;

	//generate DMA interrupt when done
	//increment source, transfer 16 bits
	//enable peripheral request
	DMA0->DMA[0].DCR = DMA_DCR_EINT_MASK
						| DMA_DCR_SINC_MASK
						| DMA_DCR_SSIZE(2)
						| DMA_DCR_DSIZE(2)
						| DMA_DCR_ERQ_MASK
						| DMA_DCR_CS_MASK;

	//configure NVIC for DMA ISR
	NVIC_SetPriority(DMA0_IRQn, 2);
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	NVIC_EnableIRQ(DMA0_IRQn);

	//set DMA MUX channel to use TPM0 overflow as trigger
	DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_SOURCE(54);
}

void start_dma_playback() {

	//initialize source and destination pointers
	DMA0->DMA[0].SAR = DMA_SAR_SAR((uint32_t) Reload_DMA_Source);
	DMA0->DMA[0].DAR = DMA_DAR_DAR((uint32_t) (&(DAC0->DAT[0])));

	//byte count
	DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(Reload_DMA_Byte_Count);

	//clear done flag
	DMA0->DMA[0].DSR_BCR &= ~DMA_DSR_BCR_DONE_MASK;

	//set enable flag
	DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK;
}

void DMA0_IRQHandler() {

	//clear done flag
	DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;
	start_dma_playback();
}
