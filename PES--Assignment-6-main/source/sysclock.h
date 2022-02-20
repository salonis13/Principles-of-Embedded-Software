/* 
 * sysclock.h - configuration routines for KL25Z system clock
 * 
 * Author Howdy Pierce, howdy.pierce@colorado.edu
 */

#ifndef _SYSCLOCK_H_
#define _SYSCLOCK_H_

#include "MKL25Z4.h"
#include<string.h>
#include<stdio.h>
//#include "startup_mkl25z4.c"
#define SYS_CLOCK (24000000U)

/*
 * Initializes the system clock. You should call this first in your
 * program.
 */
void sysclock_init();

#endif  // _SYSCLOCK_H_
