/*
 * hexdump.c
 *
 * File to dump data in hexadecimal
 *
 *  Created on: Mar 29, 2021
 *      Author: salon
 */
#include "sysclock.h"
#include "hexdump.h"

char hexdump(char *str, size_t size, void *loc, size_t nbytes)
{
	str=*(int*)loc;							//go to start address in str
    int i=0;

    while(nbytes)							//execute until data length is valid
    {
        printf("0000_%.4X ", str);			//print the address of str
        for(i=0; i<16; i++)
        {
            printf("%.2X ", *str++);		//print the data value in str
            nbytes--;						//decrement data length
            if(nbytes==0)					//check if length is 0
            	break;
        }
        printf("\n\r");
     }
    return 0;
}

