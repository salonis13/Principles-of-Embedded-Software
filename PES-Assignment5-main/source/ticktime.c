/**
 * @file    ticktime.c
 * @brief   Code to map SysTick timer into msec counter
 * @author  Howdy Pierce, howdy.pierce@colorado.edu
 */

#include "MKL25Z4.h"
#include "core_cm0plus.h"
#include "ticktime.h"

static volatile ticktime_t g_now = 0;
static ticktime_t g_timer = 0;

void init_ticktime(void)
{
  // set control & status register to use 48 MHz/16 = 3 MHz
  // clock. Then interrupt 10000 times per second
  SysTick->LOAD = (48000000/16)/10000;
  NVIC_SetPriority(SysTick_IRQn, 3);
	NVIC_ClearPendingIRQ(SysTick_IRQn); 
	NVIC_EnableIRQ(SysTick_IRQn);	
  SysTick->VAL = 0;
  SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk);
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

  g_now = 0;
  g_timer = 0;
}

void SysTick_Handler(void)
{
  g_now++;
}


ticktime_t now(void)
{
  return g_now;
}


void reset_timer(void)
{
  g_timer = g_now;
}


ticktime_t get_timer(void)
{
  return g_now - g_timer;
}

