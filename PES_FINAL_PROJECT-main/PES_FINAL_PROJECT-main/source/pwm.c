/*
 * pwm.c - Define functions to utilize pulse width modulation for RGB LED
 *
 *  Created on: Apr 28, 2021
 *      Author: saloni shah
 */
#include "board.h"
#include "peripherals.h"
#include "MKL25Z4.h"

#include "pwm.h"

//SOPT2 -Systems Options register 2
//TPMSRC Timer/PWM source select
//MCG Multipurpose clock generator C1,C2 control registers
//SCGC clock gating register
//IREFS internal reference select
//IRCS internal reference clock select
void init_pwm() {
	MCG->C1 |= MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK;      //internal reference and clock select enabled in C1 register of clock generator
	MCG->C2 |= MCG_C2_IRCS_MASK;   							//fast internal reference clock set

	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;                        //TPM0 enabled
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;                        //TPM2 enabled

	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(3);      			  //set clock source for TPM

	TPM2->SC = TPM_SC_CMOD(1) | TPM_SC_PS(4);             //LPTPM clock selected with prescalar 16
	TPM2->MOD = 2500;                                     //load mod register set

	TPM0->SC = TPM_SC_CMOD(1) | TPM_SC_PS(4);            //LPTPM clock selected with prescalar 16
	TPM0->MOD = 2500;                                    //load mod register set

	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;                 //clock gating PORTB and PORTD for RGB LED
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

	PORTB->PCR[18] = PORTB->PCR[19] = PORT_PCR_MUX(3);   //multiplex PORTB for TPM2
	PORTD->PCR[1] = PORT_PCR_MUX(4);                     //multiplex PORTD for TPM0

	TPM0->CONF |= TPM_CONF_DBGMODE(3);					//configure TPM2 and TPM0 for debug mode
	TPM2->CONF |= TPM_CONF_DBGMODE(3);

	TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK
			| TPM_CnSC_ELSA_MASK;              //rising edge output generation set for TPM2 channel 0
	TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK
			| TPM_CnSC_ELSA_MASK;              //rising edge output generation set for TPM2 channel 1
	TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK
			| TPM_CnSC_ELSA_MASK;              //rising edge output generation set for TPM0 channel 1
}

void change_led(int R, int G, int B) {
	TPM2->CONTROLS[0].CnV  = (R*TPM2->MOD)/255;		//change led colour
	TPM2->CONTROLS[1].CnV  = (G*TPM2->MOD)/255;
	TPM0->CONTROLS[1].CnV  = (B*TPM0->MOD)/255;
}

