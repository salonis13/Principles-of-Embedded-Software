/*
 * hexdump.h
 *
 *  Created on: Mar 29, 2021
 *      Author: salon
 */

#ifndef HEXDUMP_H_
#define HEXDUMP_H_

/*
 * Function to dump the hexadecimal data
 * Parameters- str - buffer of which data values are to be printed
 * 			   size - size of str
 * 			   loc - start address of the required buffer
 * 			   nbytes - length of required data dump
 * Return type - char
 */
char hexdump(char *str, size_t size, void *loc, size_t nbytes);

#endif /* HEXDUMP_H_ */
