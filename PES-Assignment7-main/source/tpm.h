/*
 * tpm.h - Initialize and start Timer/PWM modules for DAC and ADC
 *
 *  Created on: Apr 18, 2021
 *      Author: saloni
 */

#ifndef TPM_H_
#define TPM_H_

#define CLOCK 			48000000
#define TPM_OVERFLOW 	96000

//function to initialize TPM0
//Parameters: uint32_t period_us - counter value for timer
//Return type: void
void Init_TPM0(uint32_t period_us);

//function to start TPM0
//Parameters: none
//Return type: void
void Start_TPM0(void);

//function to initialize TPM1
//Parameters: none
//Return type: void
void Init_TPM1();

//function to start TPM1
//Parameters: none
//Return type: void
void Start_TPM1(void);

#endif /* TPM_H_ */
