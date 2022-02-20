/* PES - ECEN 5813 - Assignment 3 - Blinkenlights
 * Author - Saloni Shah
 * University of Colorado Boulder
 *
 * I am aware of the practice of making multiple .h and .c files
 * for different functionalities of the code. Although after the peer
 * review session, I realized that it was very tough for the person
 * reviewing the code to go back and forth between multiple files and
 * keep track of all the functions, parameters and variables.
 * Keeping that in mind, I have combined everything in one code only.
 * To check the code, you only need this one file
 *
 *REFERENCES AND CREDIT:
 * 1.	https://github.com/alexander-g-dean/ESF/tree/master/NXP/Misc/Touch%20Sense
 * 2.	https://github.com/peterfillmore/frdm-kl25z-exploit-practice-code/blob/master/touch.c
 * 3.   Class notes and references given by Howdy Pierce
 * 4.	TA Saket Penurkar helped me understand Debug and release mode accurately
 * 5.	My friend and peer Chaithra Suresh also helped me in understanding the polling concept.
 * 6.   My peer Nimish Bhide also gave me some valuable input in the code review session
 */

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

/* Global variables and macros */
volatile uint32_t touch_value=0; //value of touch slider
#define RED_LED 18	 //PORT B PIN 18 IS RED LED
#define GREEN_LED 19 	// PORT B PIN 19
#define BLUE_LED 1  	//PORT D PIN 1
#define DELAY_COUNT 2500 //iterations per millisecond
#define OFFSET 600 	//offset to normalize touch slider value
#define POLLING_INTERVAL 100 //polling interval of 100 ms for touch slider
int msec;		//given delay time
unsigned int red_on;	//state of red led
unsigned int green_on;	//state of green led
unsigned int blue_on;	//state of blue led
unsigned int is_on;		//state of led

//define minimum of two numbers
#define min(a,b) (((a) < (b)) ? (a) : (b))

//abstraction for printf in debug mode only
#ifdef DEBUG
#define LOG PRINTF //take LOG as printf in debug mode
#else
#define LOG(...)
#endif

/* Delay loop - give delay for desired amount of time
 * Parameter - msec (delay in milliseconds
 * Return value - none								*/

void delay(int msec)
{
    msec = DELAY_COUNT*msec;
    while(msec--) {						 //delay for msec
    	__asm volatile("NOP");					 //do nothing
    }
}

/* Touch_scan function to get slider values
 *  This function assigns a touch slider channel, initializes the slider
 *  by giving software trigger and returns the normalised value of touch slider
 *  Parameter - none
 *  Return value - normalised touch slider value								*/

int touch_scan(void) {
	unsigned int scan=0;
	TSI0->DATA = TSI_DATA_TSICH(10u);	 //take TSI channel 10
	TSI0->DATA |= TSI_DATA_SWTS_MASK;	 //initialize software trigger to start scanning the slider
	while(!(TSI0->GENCS & TSI_GENCS_EOSF_MASK)) //wait for 32 scans i.e. until end of scan flag is not set
		__asm volatile("NOP");
	scan = TSI0->DATA & TSI_DATA_TSICNT_MASK; //get slider value
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK; 	//reset end of scan flag
	return scan-OFFSET; 			//return normalized slider value
}

/* Function to turn on or turn off leds; based on values passed in the function
 * parameters - desired condition of all the three leds. 0 for off condition
 * 				and 1 for on condition
 * return value - none															*/

void rgb_led(unsigned int red_on, unsigned int green_on, unsigned int blue_on) {
	if(red_on)
		GPIOB->PCOR |= (1<<RED_LED); //turn on red led
	else
		GPIOB->PSOR |= (1<<RED_LED); //turn off red led
	if(green_on)
		GPIOB->PCOR |= (1<<GREEN_LED); //turn on green led
	else
		GPIOB->PSOR |= (1<<GREEN_LED); //turn off green led
	if(blue_on)
		GPIOD->PCOR |= (1<<BLUE_LED); //turn on blue led
	else
		GPIOD->PSOR |= (1<<BLUE_LED); //turn off blue led
}

//flashing function which is implemented in the code later on

void flashing();

/*	slider function to poll the slider every 100ms take the slider value
 * and change the led colour accordingly
 *
 * Function organisation -
 * 1. Poll slider every 100ms
 * 2. Take touch slider value
 * 3. Check if the led is on
 * 4. If led is on, change led colour according to touch location
 * 5. Check if led is off
 * 6. If led is off, change led value flags and execute the change after led turns on
 *
 * The led colour will change based on touch location on slider
 * touch slider value range     Location	  LED colour
 *     75 to 350                Left side       Red
 *    350 to 650                 Center        Green
 *   650 or higher             Right side      Blue
 * Parameters -
 *			remaining_delay - required delay time in milliseconds
 *			is_on - suggests the state of led. 0 for off and 1 for on
 * return value - none													*/


void slider(int remaining_delay, unsigned int is_on) {
	LOG("START TIMER %d\n",remaining_delay);		//prints the timer value
	while (remaining_delay > 0)
	{
		delay(min(remaining_delay, POLLING_INTERVAL));	//executes delay loop for 100ms
		remaining_delay -= POLLING_INTERVAL;			//decreases remaining delay value every 100ms
		touch_value = touch_scan();					//takes the touch slider value from touch_scan function
		if (touch_value > 75)
			LOG("SLIDER VALUE: %d\n", touch_value);  //prints the touch slider value whenever slider is touched

		if(is_on==1) {								//checks if led is on
        if(touch_value>75 && touch_value<350) 		//condition to turn led red
        	{
        		rgb_led(1,0,0);					//change led colour to red instantaneously
        		red_on=1; green_on=0; blue_on=0;	//sets led values for flashing sequence
        		LOG("LED COLOUR CHANGED TO RED\n");
        	}
        else if(touch_value>=350 && touch_value<650) 	//condition to turn led green
        	{
        		rgb_led(0,1,0);					//change led colour to green instantaneously
        		red_on=0; green_on=1; blue_on=0;	//set led values for flashing led sequence
        		LOG("LED COLOUR CHANGED TO GREEN\n");
        	}
        else if(touch_value>=650) 					//condition to turn led blue
        	{
        		rgb_led(0,0,1);					//change led colour to blue instantaneously
        		red_on=0; green_on=0; blue_on=1;	//set led values for flashing sequence
        		LOG("LED COLOUR CHANGED TO BLUE\n");
        	}
     }
		else if(is_on==0)						//check if led is off
		{
			if(touch_value>75 && touch_value<400) 		//condition to turn led red
			{	red_on=1; green_on=0; blue_on=0;	//set led values for flashing sequence
			    LOG("LED COLOUR CHANGED TO RED\n");
			}
			else if(touch_value>=400 && touch_value<650) 	//condition to turn led green
			{	red_on=0; green_on=1; blue_on=0;	//set led values for flashing sequence
			    LOG("LED COLOUR CHANGED TO GREEN\n");
			}
			else if(touch_value>=650) 					//condition to turn led blue
			{	red_on=0; green_on=0; blue_on=1;	//set led values for flashing sequence
			    LOG("LED COLOUR CHANGED TO BLUE\n");
			}
		}
	}
}

/* test_led function - run during start of code
 * This function is only executed once during system turn on
 * Led colour does not change if slider is touched
 * during the execution of this function                     */

void test_led()
{
    rgb_led(1,0,0); //turn on red led for 500ms
    delay(500);
    rgb_led(0,0,0); //turn off led for 100ms
    delay(100);
    rgb_led(0,1,0); //turn on green led for 500ms
    delay(500);
    rgb_led(0,0,0); //turn off led for 100ms
    delay(100);
    rgb_led(0,0,1); //turn on blue led for 500ms
    delay(500);
    rgb_led(0,0,0); //turn off led for 100ms
    delay(100);
    rgb_led(1,1,1); //turn on white led for 100ms
    delay(100);
    rgb_led(0,0,0); //turn off led for 100ms
    delay(100);
    rgb_led(1,1,1); //turn on white led for 100ms
    delay(100);
    rgb_led(0,0,0); //turn off white led for 100ms
    delay(100);
}

/* Flashing function for infinite loop of led blinking
 * This function blinks white led initially and
 * calls slider function to poll touch slider and
 * change led accordingly. There is no way of going back
 * to white colour led once the slider is touched
 * Parameter - none
 * Return value - none									 */

void flashing()
{
while(1) {
	rgb_led(red_on,green_on,blue_on); //turn on led for 500ms
	    slider(500,1);
	rgb_led(0,0,0); 		//turn off led for 500ms
	    slider(500,0);
	rgb_led(red_on,green_on,blue_on); //turn on led for 1000ms
	    slider(1000,1);
	rgb_led(0,0,0);			 //turn off led for 500ms
	    slider(500,0);
	rgb_led(red_on,green_on,blue_on); //turn on led for 2000ms
	    slider(2000,1);
	rgb_led(0,0,0); 		//turn off led for 500ms
	    slider(500,0);
	rgb_led(red_on,green_on,blue_on); //turn on led for 3000ms
	    slider(3000,1);
	rgb_led(0,0,0); 		//turn off led for 500ms
	    slider(500,0);
}
}

/* Function to clock gate led ports and
 * initialize all leds					 */

void led_initialize()
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK; //CLOCK GATING LED PORTS

	//SET RED LED PIN, GREEN LED PIN AND BLUE LED PIN TO GPIO
	PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);
	PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
	PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);

	//SET LEDs TO OUTPUT
	GPIOB->PDDR |= (1<<RED_LED) | (1<<GREEN_LED);
	GPIOD->PDDR |= (1<<BLUE_LED);
	rgb_led(0,0,0);								 //turn off all leds
}

/* Function to clock gate and initialize
 * the capacitive touch slider present on FRDM KL25Z development board
 * In this function I have set all the registers of the slider			 */

void touch_initialize()
{
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK; 			  //CLOCK GATING TOUCH SENSOR

	//set register for touch sensor
	TSI0->GENCS |= TSI_GENCS_MODE(0u)	//set touch slider on non-noise mode
					| TSI_GENCS_REFCHRG(0u) 	//set oscillator charge and discharge value to 500nA
					| TSI_GENCS_DVOLT(0u)		//set oscillator voltage rails to default value
					| TSI_GENCS_EXTCHRG(0u) 	//set electrode oscillator charge and discharge value to 500nA
					| TSI_GENCS_PS(0u)		//set prescaler value to 1 for frequency division of clock
					| TSI_GENCS_NSCN(31u) 		//set electrode oscillator count for number of scans to 32
					| TSI_GENCS_TSIEN_MASK		//enable TSI module
					| TSI_GENCS_EOSF_MASK; 		//set end of scan flag to clear it
}

//main function

int main(void)
{
	led_initialize();		//function to initialize RGB led

	touch_initialize();		//function to initialize capacitive touch slider

	test_led(); 			//LED SEQUENCE ON STARTUP

	red_on=1;green_on=1;blue_on=1; //set led values for white light
	flashing();		      //LED SEQUENCE IN INFINITE LOOP

	return 0;
}
