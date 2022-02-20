/* This file contains all the test cases for cbfifo.c.
  * I have attempted to cover all corner cases which 
  * might arise in the implementation. These test cases 
  * checks the implementation of all the functions in 
  * different scenarios and returns a value or error accordingly
  */

#include<stdio.h>
#include<assert.h>
#include<stdint.h>
#include<stdlib.h>
#include "cbfifo.h"

void *buf;

int test_cbfifo1()
{
   cbfifo_enqueue("Sleepy",6);
	cbfifo_enqueue("Sneezy",6);
	cbfifo_enqueue("Happy",5);
	cbfifo_dequeue(buf,10);
	return 0;
}

int test_cbfifo2()
{
   cbfifo_enqueue("Sleepyreresasasasadtgfvbhyjulimnhiojnvsewazcvgfdesdctgbhjunb",60);
	cbfifo_enqueue("Sneezy",60);
	cbfifo_enqueue("Happyim",7);
	cbfifo_enqueue("oohhhhh",7);
	cbfifo_dequeue(buf,10);
	return 0; 
}


int test_cbfifo3()
{
   cbfifo_enqueue("Sleepy",6);
	cbfifo_enqueue("Sneezy",6);
	cbfifo_dequeue(buf,10);
	cbfifo_enqueue("Happyim",7);
	cbfifo_dequeue(buf,9);
	cbfifo_dequeue(buf,10);
	return 0;
}

int test_cbfifo4()
{
   cbfifo_enqueue("Saloni",6);
	cbfifo_enqueue("Shah",4);
	cbfifo_dequeue(buf,10);
	cbfifo_enqueue("Happyim",7);
	cbfifo_enqueue("moreeee",3);
	cbfifo_dequeue(buf,9);
	return 0;
}

int test_cbfifo5()
{
   cbfifo_enqueue("Saloni",6);
	cbfifo_enqueue("Shah",4);
	cbfifo_dequeue(buf,10);
	cbfifo_enqueue("",7);
	return 0;   
}

int test_cbfifo6()
{
   cbfifo_enqueue("Saloni",6);
	cbfifo_enqueue("Shah",4);
	cbfifo_dequeue(buf,0);
	cbfifo_enqueue("enjoyus",7);
	cbfifo_dequeue(buf,10);
	return 0;   
}