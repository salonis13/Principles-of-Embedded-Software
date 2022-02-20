/* This file defines all the functions to 
    *implement a linked list FIFO(first in first out)
	 */



#include<stdio.h>
#include<assert.h>
#include<stdint.h>
#include<stdlib.h>
#include "llfifo.h"

/* data structure new has 2 objects   
		*-void pointer data      to store data of the buf
		*  -addr                   which points to the address the next data byte
	  */

struct new
{
	void *data;
	struct new *addr;
};

/* data structure llfifo_s which is typedef-ed
  *  as llfifo_t in llfifo.h file. This data  
  *structure has two objects which point to the 
  *first and last element of the linked list  
  */

struct llfifo_s{
	struct new *first;
	int length;
	int capacity;
};


//declaring global variables which are used in multiple functions

/*
 * Creates and initializes the FIFO
 *
 * Parameters:
 *   capacity  the initial size of the fifo, in number of elements
 * 
 * Returns: A pointer to a newly-created llfifo_t instance, or NULL in
 *   case of an error.
 */


llfifo_t *llfifo_create(int capacity)
{
	llfifo_t *fifo = (llfifo_t *)malloc(sizeof(struct llfifo_s));
	if(fifo==NULL)
	   return NULL;
	fifo->first = NULL;
	fifo->length=0;
	fifo->capacity=0;
	return fifo;
}

/*
 * Enqueues an element onto the FIFO, growing the FIFO by adding
 * additional elements, if necessary
 *
 * Parameters:
 *   fifo    The fifo in question
 *   element The element to enqueue
 * 
 * Returns:
 *   The new length of the FIFO on success, -1 on failure
 */

int llfifo_enqueue(llfifo_t *fifo, void *element)
{
    if(fifo==NULL)
	   return -1;
	struct new* another= (struct new*)malloc(sizeof(struct new));
	if(another==NULL)
		return -1;
	another->data = element;
	another->addr = NULL;
	if(fifo->first==NULL)
	{
		fifo->first = another;
		}
	else
	{
		struct new *again=fifo->first;
		while(again->addr != NULL)
		{	again=again->addr;
	 }
		again->addr = another; 
 	}	
	   fifo->length++;
		fifo->capacity++;
		return fifo->length;
}

/*
 * Removes ("dequeues") an element from the FIFO, and returns it
 *
 * Parameters:
 *   fifo  The fifo in question
 * 
 * Returns:
 *   The dequeued element, or NULL if the FIFO was empty
 */

void *llfifo_dequeue(llfifo_t *fifo)
{
		void *ret;
    if(fifo==NULL || fifo->first==NULL)
			return NULL; 
	else
	{
	struct new *next = fifo->first;
	if(fifo->first->addr == NULL)
	{
		fifo->first=NULL;
		ret = next->data;
		free(next);
   }	
	else
	{
	fifo->first = next->addr;
	ret = next->data;
	free(next);
   }	
}
	fifo->length--;
	fifo->capacity--;
	return ret;
	
}

/*
 * Returns the number of elements currently on the FIFO. 
 *
 * Parameters:
 *   fifo  The fifo in question
 * 
 * Returns:
 *   The number of elements currently on the FIFO
 */

int llfifo_length(llfifo_t *fifo)
{
	int len=0;
	struct new* current = fifo->first;
	while(current!=NULL)
	{
		len++;
		current = current->addr;
	}
	return len;
}

/*
 * Returns the FIFO's current capacity
 *
 * Parameters:
 *   fifo  The fifo in question
 * 
 * Returns:
 *   The current capacity, in number of elements, for the FIFO
 */

int llfifo_capacity(llfifo_t *fifo)
{
    return fifo->capacity;
} 

/*
 * Teardown function: Frees all dynamically allocated
 * memory. After calling this function, the fifo should not be used
 * again!
 *
 * Parameters:
 *   fifo  The fifo in question
 * 
 * Returns: none
 */

void llfifo_destroy(llfifo_t *fifo)
{
    struct new *temp= fifo->first;
	 while(temp!=NULL)
	 {
	   temp=fifo->first;
		free(temp);
	}
	fifo->first=NULL;
}
