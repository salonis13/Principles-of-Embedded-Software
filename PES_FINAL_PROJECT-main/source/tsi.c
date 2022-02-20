/*
 * tsi.c - Define functions for capacitive touch sensor
 *
 *  Created on: Apr 29, 2021
 *      Author: saloni shah
 */
#include "MKL25Z4.h"
#include "tsi.h"

#define OFFSET 600 	//offset to normalize touch slider value

void init_touch() {
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

int touch_scan(void) {
	unsigned int scan=0;
	TSI0->DATA = TSI_DATA_TSICH(10u);				 //take TSI channel 10
	TSI0->DATA |= TSI_DATA_SWTS_MASK;	 			//initialize software trigger to start scanning the slider
	while(!(TSI0->GENCS & TSI_GENCS_EOSF_MASK)) 	//wait for 32 scans i.e. until end of scan flag is not set
		__asm volatile("NOP");
	scan = TSI0->DATA & TSI_DATA_TSICNT_MASK; 		//get slider value
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK; 			//reset end of scan flag
	return scan-OFFSET; 							//return normalized slider value
}

