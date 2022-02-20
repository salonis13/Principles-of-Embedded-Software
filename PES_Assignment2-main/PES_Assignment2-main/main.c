/* The main function calls all the test case functions 
    * of linked list FIFO and circular buffer FIFO
	 * Return: value 1 in case it passes all the test cases.
	 */

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<assert.h>
#include"llfifo.h"

int test_llfifo1();
int test_llfifo2();
int test_llfifo3();
int test_llfifo4();
int test_llfifo5();
int test_cbfifo1();
int test_cbfifo2();
int test_cbfifo3();
int test_cbfifo4();
int test_cbfifo5();
int test_cbfifo6();


int main()
{
    assert(test_llfifo1()==0);
	 assert(test_llfifo2()==0);
	 assert(test_llfifo3()==0);
	 assert(test_llfifo4()==0);
	 assert(test_llfifo5()==0);
	 assert(test_cbfifo1()==0);
	 assert(test_cbfifo2()==0);
	 assert(test_cbfifo3()==0);
	 assert(test_cbfifo4()==0);
	 assert(test_cbfifo5()==0);
	 assert(test_cbfifo6()==0);
	   return 1;
}