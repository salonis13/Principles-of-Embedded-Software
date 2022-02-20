/* This file contains all the test cases for llfifo.c.
  * I have attempted to cover all corner cases which 
  * might arise in the implementation. These test cases 
  * checks the implementation of all the functions in 
  * different scenarios and returns a value or error accordingly
  */

#include<stdio.h>
#include<assert.h>
#include<stdint.h>
#include<stdlib.h>
#include "llfifo.h"

int test_llfifo1()
{
    llfifo_t *my_fifo =llfifo_create(5);
	assert (llfifo_length(my_fifo) == 0);
	assert (llfifo_capacity(my_fifo) == 5);
	llfifo_enqueue(my_fifo, "Sleepy"); // does not result in a malloc (len = 1, cap = 5)
	llfifo_enqueue(my_fifo, "Grumpy"); // does not result in a malloc (len = 2, cap = 5)
	llfifo_enqueue(my_fifo, "Sneezy"); // does not result in a malloc (len = 3, cap = 5)
	llfifo_enqueue(my_fifo, "Happy"); // does not result in a malloc (len = 4, cap = 5)
	llfifo_enqueue(my_fifo, "Bashful"); // does not result in a malloc (len = 5, cap = 5)
	llfifo_dequeue(my_fifo); // removes "Sleepy"; len = 4, cap = 5
	llfifo_enqueue(my_fifo, "Dopey"); // does not result in a malloc (len = 5, cap = 5)
	llfifo_enqueue(my_fifo, "Doc"); // DOES result in a malloc (len = 6, cap = 6)
	return 0;
}

int test_llfifo2()
{
    llfifo_t *my_fifo1 =llfifo_create(3);
	assert (llfifo_length(my_fifo1) == 0);
	assert (llfifo_capacity(my_fifo1) == 3);
	llfifo_enqueue(my_fifo1, "Saloni"); // does not result in a malloc (len = 1, cap = 3)
	llfifo_enqueue(my_fifo1, "Shah"); // does not result in a malloc (len = 2, cap = 3)
	llfifo_enqueue(my_fifo1, "llfifo"); // does not result in a malloc (len = 3, cap = 3)
	llfifo_enqueue(my_fifo1, "pes"); // does result in a malloc (len = 4, cap = 4)
	llfifo_dequeue(my_fifo1); // removes "Saloni"; len = 3, cap = 4
	llfifo_enqueue(my_fifo1, "Dopey"); // does not result in a malloc (len = 4, cap = 4)
	llfifo_enqueue(my_fifo1, "Doc"); // DOES result in a malloc (len = 5, cap = 5)
	return 0;   
}


int test_llfifo3()
{
    llfifo_t *my_fifo2 =llfifo_create(2);
	assert (llfifo_length(my_fifo2) == 0);
	assert (llfifo_capacity(my_fifo2) == 2);
	llfifo_enqueue(my_fifo2, "Sleepy"); // does not result in a malloc (len = 1, cap = 2)
	llfifo_enqueue(my_fifo2, ""); // does not result in a malloc (len = 2, cap = 2)
	llfifo_enqueue(my_fifo2, "Sneezy"); //trying to enqueue after passing null element
	llfifo_dequeue(my_fifo2);
	llfifo_enqueue(my_fifo2, "Happy"); 
	return 0;
}

int test_llfifo4()
{
    llfifo_t *my_fifo3 =llfifo_create(3);
	assert (llfifo_length(my_fifo3) == 0);
	assert (llfifo_capacity(my_fifo3) == 3);
	llfifo_enqueue(my_fifo3, "fifo"); // does not result in a malloc (len = 1, cap = 3)
	llfifo_enqueue(my_fifo3, "linked"); // does not result in a malloc (len = 2, cap = 3)
	llfifo_enqueue(my_fifo3, "Sneezy"); // does not result in a malloc (len = 3, cap = 3)
	llfifo_destroy(my_fifo3);
	llfifo_enqueue(my_fifo3, "Happy"); //trying to enqueue after destroying linked list completely
	return 0;
}

int test_llfifo5()
{
    llfifo_t *my_fifo4 =llfifo_create(5);
	assert (llfifo_length(my_fifo4) == 0);
	assert (llfifo_capacity(my_fifo4) == 5);
	llfifo_enqueue(my_fifo4, "Howdy"); // does not result in a malloc (len = 1, cap = 5)
	llfifo_enqueue(my_fifo4, "Pierce"); // does not result in a malloc (len = 2, cap = 5)
	llfifo_enqueue(my_fifo4, "ecenese"); // does not result in a malloc (len = 3, cap = 5)
	llfifo_enqueue(my_fifo4, "stack"); // does result in a malloc (len = 4, cap = 5)
	llfifo_dequeue(my_fifo4); // removes "Howdy"; len = 3, cap = 5
	llfifo_dequeue(my_fifo4); // removes "Pierce"; len = 2, cap = 5
	llfifo_dequeue(my_fifo4); // removes "ecenese"; len = 1, cap = 5
	llfifo_dequeue(my_fifo4); // removes "stack"; len = 0, cap = 5
	llfifo_dequeue(my_fifo4);
	llfifo_enqueue(my_fifo4, "Dopey"); // trying to enqueue
	llfifo_enqueue(my_fifo4, "Doc"); 
	return 0;   
}