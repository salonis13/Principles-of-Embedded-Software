/*
 * pwm_touch.c
 *
 * This file defines functions to initialize pwm and touch slider and get slider values
 *  Created on: Mar 7, 2021
 *      Author: Saloni Shah
 */
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "core_cm0plus.h"

#include "pwm_touch.h"

int touch_value=0;			//touch slider value

void init_touch()
{
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK; 			  //CLOCK GATING TOUCH SENSOR

	//set register for touch sensor
	TSI0->GENCS |= TSI_GENCS_MODE(0u)	//set touch slider on non-noise mode
					| TSI_GENCS_REFCHRG(0u) 	//set oscillator charge and discharge value to 500nA
					| TSI_GENCS_DVOLT(0u)		//set oscillator voltage rails to default value
					| TSI_GENCS_EXTCHRG(0u) 	//set electrode oscillator charge and discharge value to 500nA
					| TSI_GENCS_PS(0u)			//set prescaler value to 1 for frequency division of clock
					| TSI_GENCS_NSCN(31u) 		//set electrode oscillator count for number of scans to 32
					| TSI_GENCS_TSIEN_MASK		//enable TSI module
					| TSI_GENCS_EOSF_MASK; 		//set end of scan flag to clear it
}

int touch_scan() {
	unsigned int scan=0;
	TSI0->DATA = TSI_DATA_TSICH(10u);	 		//take TSI channel 10
	TSI0->DATA |= TSI_DATA_SWTS_MASK;	 		//initialize software trigger to start scanning the slider
	while(!(TSI0->GENCS & TSI_GENCS_EOSF_MASK)) //wait for 32 scans i.e. until end of scan flag is not set
		__asm volatile("nop");
	scan = TSI0->DATA & TSI_DATA_TSICNT_MASK; 	//get slider value
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK; 		//reset end of scan flag
	touch_value=scan-OFFSET;
	return touch_value; 						//return normalized slider value
}

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
