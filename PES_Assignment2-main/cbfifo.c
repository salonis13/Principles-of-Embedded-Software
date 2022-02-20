/* This file defines all the functions to 
    *implement a linked list FIFO(first in first out)
	 */ 


#include<stdio.h>
#include<stdint.h>
#include "cbfifo.h"
#define SIZE 128

// declaring all the global variables which are used in multiple functions

char temp[128];
uint8_t firs=0;
uint8_t las=0;
size_t capacity=SIZE;
size_t lengt=0;

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
 
size_t cbfifo_enqueue(void *buf, size_t nbyte)
{
    size_t newbyte;
	 lengt=cbfifo_length();
	 capacity=cbfifo_capacity();
    if(lengt==SIZE-1)
        return -1;
    if(SIZE>lengt+nbyte)
        newbyte=nbyte;
	 else
	     newbyte=SIZE-lengt;
	for(int i=0; i<newbyte;i++)
	{
        temp[firs]=*(char*)buf;
		  firs=(firs+1)&(SIZE-1);
		  buf++;
		  lengt++;
		  capacity--;
	}
    return newbyte;
}

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
size_t cbfifo_dequeue(void *buf, size_t nbyte)
{
    size_t newbyte=0;
	 char *pls;
	 pls=(char *)buf;
	 lengt=cbfifo_length();
	 capacity=cbfifo_capacity();
    if(lengt==0)
        return 0;
	 if(nbyte>lengt)
	   {newbyte=lengt;}
	 else
	   {newbyte=nbyte;}
		for(int i=0; i<newbyte; i++)
		{
			pls[i]=temp[las];
		las=(las+1)&(SIZE-1);
		lengt--;
		capacity++;  
	}   
	return newbyte;
}


/*
 * Returns the number of bytes currently on the FIFO. 
 *
 * Parameters:
 *   none
 * 
 * Returns:
 *   Number of bytes currently available to be dequeued from the FIFO
 */
size_t cbfifo_length()
{
	 return lengt;
}

/*
 * Returns the FIFO's capacity
 *
 * Parameters:
 *   none
 * 
 * Returns:
 *   The capacity, in bytes, for the FIFO
 */
size_t cbfifo_capacity()
{
	return capacity;
}
