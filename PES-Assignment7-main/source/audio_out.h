/*
 * audio_out.h - Initialize DAC, DMA, store samples in buffer
 * 					 and play tones
 *
 *  Created on: Apr 18, 2021
 *      Author: saloni
 */

#ifndef AUDIO_OUT_H_
#define AUDIO_OUT_H_

//function to initialize digital to analog conversion module
//Parameters: none
//Return type: void
void Init_DAC();

//function to store tone samples in buffers
//Parameters: none
//Return type: void
void tone_to_samples();

//function to check which tone to play next
//Parameters: i - number of tone to be played next
//Return type: void
void check_tone(int i);

//function to compute required tone and play it
//Parameters: none
//Return type: void
void audio_output();

//function to initialize DMA module
//Parameters: uint16_t *source - tone buffer
//			  uint32_t count - number of samples of the tone
//Return type: void
void init_dma_for_playback(uint16_t *source, uint32_t count);

//function to enable DMA to start playing the tone
//Parameters: none
//Return type: void
void start_dma_playback();

//DMA interrupt handler
//Parameters: none
//Return type: void
void DMA0_IRQHandler();

#endif /* AUDIO_OUT_H_ */
