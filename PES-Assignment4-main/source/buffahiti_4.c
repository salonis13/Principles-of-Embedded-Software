/*
* buffahiti_4.c
*
*	Main function
*
*  Created on: Mar 2, 2021
*      Author: Saloni Shah
*/
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "core_cm0plus.h"

#include "log.h"
#include "fsm.h"
#include "pwm_touch.h"

int main()
{
  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

  	LOG("Main Loop is Starting\n\r");                                                                                                           //TPMSRC is set to 3 i.e. MCGIRCLK clock selected for TPM

    init_systick();			//initialise systick

    init_touch();			//initialise touch slider

    init_pwm();				//initialise pwm

    while(1)
    {
    	state_machine();	//execute state machine
    }
    return 0;
}
