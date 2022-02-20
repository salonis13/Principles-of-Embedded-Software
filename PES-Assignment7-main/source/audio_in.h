/*
 * audio_in.h - Initialize ADC, calibrate ADC, take audio input and analyse
 * 						the audio input
 *
 *  Created on: Apr 18, 2021
 *      Author: saloni
 */

#ifndef AUDIO_IN_H_
#define AUDIO_IN_H_

//function to calculate minimum, maximum and
//	average values in audio input buffer
//Parameters: uint16_t *a - buffer containing audio input information
//			  int n - number of samples in the buffer
uint16_t min_max(uint16_t *a,int n);

//function to initialize analog to digital conversion module
//Parameters: none
//Return type: void
void Init_ADC(void);

//function to calibrate ADC
//Parameters: none
//Return type: void
void calibrate();

//function to store audio input in buffer
//Parameters: none
//Return type: void
void audio_input();

//function to carry out analysis on audio input buffer
//Parameters: none
//Return type: void
void audio_analysis();


void delay(int n);
#endif /* AUDIO_IN_H_ */
