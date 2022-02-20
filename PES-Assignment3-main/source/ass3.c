#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

/* TODO: insert other definitions and declarations here. */
volatile uint32_t touch_value=0; //value of touch slider
#define RED_LED 18	 //PORT B PIN 18 IS RED LED
#define GREEN_LED 19 	// PORT B PIN 19
#define BLUE_LED 1  	//PORT D PIN 1
#define DELAY_COUNT 1600 //iterations per millisecond
#define OFFSET 600 	//offset to normalize touch slider value
#define POLLING_INTERVAL 100 //polling interval of 100 ms for touch slider
volatile uint32_t i;
int msec;		//given delay time
unsigned int red_on;	//state of red led
unsigned int green_on;	//state of green led
unsigned int blue_on;	//state of blue led

#ifdef DEBUG
#define LOG PRINTF //take LOG as printf in debug mode
#else
#define LOG(...)
#endif

//Delay loop - give delay for desired amount of time as well as poll touch slider every 100ms.

void delay(int msec)
{
    i = DELAY_COUNT*POLLING_INTERVAL; 				//initialize i for 100ms delay
    LOG("START TIMER OF %d\n",msec);
    for(int counter=msec/POLLING_INTERVAL;counter>0;counter--) { //set counter value according to required delay period
    while(i--!=0) {						 //delay for 100ms
    	__asm volatile("NOP");					 //do nothing
    }
    i = DELAY_COUNT*POLLING_INTERVAL; 				//update value of i every 100ms
    if(touch_scan()>40) { 					//go in if statement if value returned by touch_scan function greater than 40
		LOG("SLIDER VALUE %d\n",touch_scan());
    	slider(); } 						//go to slider function if the if statement is satisfied i.e. slider is touched
    }
}

//data structure for value of touch slider and initialize red, green and blue leds

typedef struct {
	uint32_t max_touch_value;	//touch slider value
	uint8_t R, G, B;
}touch_rgb_entry;

//RGB_table of type touch_rgb_entry to indicate value of LEDs at certain slider values
//touch slider value = 75 - 300 suggests left position and turns on red led
//touch slider value = 400 - 600 suggests center position and turns on green led
//touch slider value = 700 or higher suggests right position and turns on blue led

touch_rgb_entry RGB_table[] = { {75,0,0,0},
			       {200,1,0,0},
			       {300,1,0,0},
			       {400,0,1,0},
			       {500,0,1,0}, 
			       {600,0,1,0},
			       {700,0,0,1},
			       {850,0,0,1}, 
			       {1000,0,0,1}, 
			       {0,0,0}  };

//touch_scan function to get slider values

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

//function to turn on or turn off leds; based on values passed in the function

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

//slider function to take the slider value and change the led colour accordingly

void slider() {
while(1) {
	touch_value = touch_scan();			//get touch slider value
    for(int j=0;RGB_table[j].max_touch_value>0;j++) {
        if(touch_value>75 && touch_value<300) 		//check the value of slider
        	{red_on=1; green_on=0; blue_on=0;
        		LOG("LED COLOUR CHANGED TO RED\n");} //change led colour to red
        else if(touch_value>=400 && touch_value<600) 	//check the value of slider
        	{red_on=0; green_on=1; blue_on=0;
        		LOG("LED COLOUR CHANGED TO GREEN\n");} //change led colour to green
        else if(touch_value>=700) 			//check the value of slider
        	{red_on=0; green_on=0; blue_on=1;
        		LOG("LED COLOUR CHANGED TO BLUE\n");} //change led colour to blue
        flashing(); 					//go to flashing sequence
     }
}
}

//test_led function run during start of code

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
    rgb_led(0,0,0); //turn off white led
    delay(100);
}

//flashing function for infinite loop of led blinking

void flashing()
{
while(1) {
	rgb_led(red_on,green_on,blue_on); //turn on led for 500ms
	    delay(500);
	rgb_led(0,0,0); 		//turn off led for 500ms
	    delay(500);
	rgb_led(red_on,green_on,blue_on); //turn on led for 1000ms
	    delay(1000);
	rgb_led(0,0,0);			 //turn off led for 500ms
	    delay(500);
	rgb_led(red_on,green_on,blue_on); //turn on led for 2000ms
	    delay(2000);
	rgb_led(0,0,0); 		//turn off led for 500ms
	    delay(500);
	rgb_led(red_on,green_on,blue_on); //turn on led for 3000ms
	    delay(3000);
	rgb_led(0,0,0); 		//turn off led for 500ms
	    delay(500);
}
}

//main function

int main(void) {
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK; //CLOCK GATING LED PORTS
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK; 			  //CLOCK GATING TOUCH SENSOR

	//set register for touch sensor
	TSI0->GENCS |= TSI_GENCS_MODE(0u)	//set touch slider on non-noise mode
		| TSI_GENCS_REFCHRG(0u) 	//set oscillator charge and discharge value to 500nA
		| TSI_GENCS_DVOLT(0u)		//set oscillator voltage rails to default value
		| TSI_GENCS_EXTCHRG(0u) 	//set electrode oscillator charge and discharge value to 500nA
		| TSI_GENCS_PS(0u)		//set prescaler value to 1 for frequency division of clock
		| TSI_GENCS_NSCN(31u) 		//set electrode oscillator count for number of scans to 32
		|TSI_GENCS_TSIEN_MASK		//enable TSI module
		| TSI_GENCS_EOSF_MASK; 		//set end of scan flag to clear it

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
	rgb_led(0,0,0); //turn off all leds

	TSI0->DATA = TSI_DATA_TSICH(10u);	//take TSI channel 10
	TSI0->DATA |= TSI_DATA_SWTS_MASK;	//initialize software trigger to start scanning the slider

	test_led(); 			//LED SEQUENCE ON STARTUP
	red_on=1;green_on=1;blue_on=1; //set led values for white light
	flashing();		      //WHITE LED SEQUENCE IN INFINITE LOOP
	return 0;
}

