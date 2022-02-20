/*
 * cbfifo.h
 *
 *  Created on: Mar 29, 2021
 *      Author: saloni
 */

#ifndef CBFIFO_H_
#define CBFIFO_H_

#include <stdio.h>
#include <stdint.h>  // for size_t

#define SIZE 256

//data structure to define circular fifo components
typedef struct {
	uint8_t data[SIZE];
	unsigned int first;
	unsigned int last;
	unsigned int length;
} volatile cbfifo_t;

/*
 * Function to initialize circular buffer
 * Parameters- buf of type cbfifo_t
 * Return type- void
 */
void cbfifo_init(cbfifo_t *buf);

/*
 * Function to check if fifo is empty
 * Parameters- buf of type cbfifo_t
 * Return type- integer
 */
int fifo_empty(cbfifo_t *buf);

/*
 * Function to check if fifo is full
 * Parameters- buf of type cbfifo_t
 * Return type- integer
 */
int fifo_full(cbfifo_t *buf);

/*
 * Enqueues data onto the FIFO, up to the limit of the available FIFO
 * capacity.
 *
 * Parameters:
 *   buf      Pointer to the data
 *   nbyte    Max number of bytes to enqueue
 *
 * Returns:
 *   The number of bytes actually enqueued, which could be 0. In case
 * of an error, returns -1.
 */
size_t cbfifo_enqueue(cbfifo_t *buf, size_t nbyte);


/*
 * Attempts to remove ("dequeue") up to nbyte bytes of data from the
 * FIFO. Removed data will be copied into the buffer pointed to by buf.
 *
 * Parameters:
 *   buf      Destination for the dequeued data
 *   nbyte    Bytes of data requested
 *
 * Returns:
 *   The number of bytes actually copied, which will be between 0 and
 * nbyte.
 *
 * To further explain the behavior: If the FIFO's current length is 24
 * bytes, and the caller requests 30 bytes, cbfifo_dequeue should
 * return the 24 bytes it has, and the new FIFO length will be 0. If
 * the FIFO is empty (current length is 0 bytes), a request to dequeue
 * any number of bytes will result in a return of 0 from
 * cbfifo_dequeue.
 */
size_t cbfifo_dequeue(cbfifo_t *buf);


/*
 * Returns the number of bytes currently on the FIFO.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   Number of bytes currently available to be dequeued from the FIFO
 */
size_t cbfifo_length(cbfifo_t *buf);

#endif /* CBFIFO_H_ */
