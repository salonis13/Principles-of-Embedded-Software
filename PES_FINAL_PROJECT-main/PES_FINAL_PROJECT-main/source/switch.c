/*
 * switch.c - Define functions to use interrupt driven push-button
 *
 *  Created on: May 3, 2021
 *      Author: saloni shah
 */
#include "MKL25Z4.h"
#include "switch.h"

#define SW1_POS (7)		//push-button on port D

#define MASK(x) (1UL << (x))

extern int switch_flag;

void init_interrupt(void) {

	/* Configure PORT peripheral. Select GPIO and enable pull-up
	resistors and interrupts on all edges for pin connected to switche */
	PORTD->PCR[SW1_POS] = PORT_PCR_MUX(1) | PORT_PCR_PS_MASK |
			PORT_PCR_PE_MASK | PORT_PCR_IRQC(11);

	/* Configure NVIC */
	NVIC_SetPriority(PORTD_IRQn, 128);
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	NVIC_EnableIRQ(PORTD_IRQn);

	/* Configure PRIMASK */
	__enable_irq();
}

void init_switch() {

	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

	// Select port D on pin mux, enable pull-up resistors
	PORTD->PCR[SW1_POS] = PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;

	// Clear switch bits to input
	PTD->PDDR &= ~MASK(SW1_POS);

	//initialize gpio interrupt
	init_interrupt();
}

void PORTD_IRQHandler(void) {

	// Read switch
	if ((PORTD->ISFR & MASK(SW1_POS))) {
		switch_flag=1;
	}
	// clear status flags
	PORTD->ISFR = 0xffffffff;
}
