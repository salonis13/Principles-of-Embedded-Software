/*
 * tpm.c - Initialize and start Timer/PWM modules for DAC and ADC
 *
 *  Created on: Apr 18, 2021
 *      Author: saloni
 */
#include "MKL25Z4.h"

//function description in tpm.h
#include "tpm.h"

void Init_TPM0(uint32_t period_us) {
		//turn on clock to TPM
		SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

		//set clock source for tpm
		SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);

		// disable TPM
		TPM0->SC = 0;

		//load the modulo
		TPM0->MOD = TPM_MOD_MOD(period_us);

		//set TPM to enable DMA transfer,  and divide by 2 prescaler
		TPM0->SC = (TPM_SC_DMA_MASK | TPM_SC_PS(1));
}

void Start_TPM0(void) {
	// Enable counter
	TPM0->SC |= TPM_SC_CMOD(1);
}

void Init_TPM1() {
		//turn on clock to TPM
		SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;

		//set clock source for tpm
		SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1)
						| SIM_SOPT2_PLLFLLSEL_MASK);

		// disable TPM
		TPM1->SC = 0;

		//load the modulo
		TPM1->MOD = (CLOCK/TPM_OVERFLOW)-1;

		//set TPM to no prescaler
		TPM1->SC |= TPM_SC_PS(0);
}

void Start_TPM1(void) {
	// Enable counter
	TPM1->SC |= TPM_SC_CMOD(1);
}
