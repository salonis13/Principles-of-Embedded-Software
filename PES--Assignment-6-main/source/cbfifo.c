/*
 * cbfifo.c
 *
 * File to initialize circular fifo, enqueue and dequeue data from fifo
 * and check fifo status and length
 *
 *  Created on: Mar 29, 2021
 *      Author: saloni
 */
#include<stdio.h>
#include "sysclock.h"
#include "cbfifo.h"
#include "uart.h"

void cbfifo_init(cbfifo_t *buf) {
	unsigned int i;
	for(i=0; i<SIZE; i++)
		buf->data[i] = 0;			//empty data in buffer if any
	buf->first=0;					//start at first position
	buf->last=0;					//keep the end at first position
	buf->length=0;					//reset buffer length
}

int fifo_empty(cbfifo_t *buf) {
	return buf->length == 0;
}

int fifo_full(cbfifo_t *buf) {
	return buf->length == SIZE;
}

size_t cbfifo_enqueue(cbfifo_t *buf, size_t nbyte) {

	uint32_t masking_state;

	if(!fifo_full(buf)) {							//check if fifo is full
		buf->data[buf->last++] = nbyte;				//store data in buffer
		buf->last %= SIZE;

		masking_state = __get_PRIMASK();			//save current masking state

		__disable_irq();							//disable interrupt

		buf->length++;								//increment length of buffer

		__set_PRIMASK(masking_state);				//restore interrupt masking state
		return 1;									//success
	}
	else
		return 0;									//failure
}

size_t cbfifo_dequeue(cbfifo_t *buf) {

	uint32_t masking_state;
    size_t newbyte=0;

    if(!fifo_empty(buf)) {							//check if fifo is empty
    	newbyte = buf->data[buf->first];			//store the first byte
    	buf->data[buf->first++] = '_';				//empty unused entries
    	buf->first %= SIZE;

    	masking_state = __get_PRIMASK();			//save current masking state

    	__disable_irq();							//disable interrupt

    	buf->length--;								//decrement length of buffer

    	__set_PRIMASK(masking_state);				//restore masking state
    }
	return newbyte;
}

size_t cbfifo_length(cbfifo_t *buf) {

	 return buf->length;
}


