/*
 * delay.c - Define delay function
 *
 *  Created on: Apr 28, 2021
 *      Author: saloni shah
 */

#include <MKL25Z4.H>

void Delay (uint32_t msecs) {
	volatile uint32_t t;

	//stay in delay loop for msecs milliseconds
	for (t=msecs*10000; t>0; t--)
		;
}
