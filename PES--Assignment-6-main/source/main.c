/*PES - Assignment 6 - UART communication
 *
 * main.c - application entry point
 *
 * This program calls all the initialization functions
 * for system clock and UART. It also executes all the commands
 * passed on terminal.
 *
 * Author: Saloni Shah
 *
 * Reference: 1. Lecture notes
 * 			  2. Dean book for UART initialization
 */
#include "sysclock.h"
#include "cbfifo.h"
#include "hexdump.h"
#include "uart.h"


/*
 * Function to execute commands received on UART
 * Parameters: none
 * Return type: integer
 */
int command_execution() {

	//initialization of the buffers and variables
	char buffer[100], dump_buf[10], start[10];
	int ret=1, ret1=1, ret2=1, flag=0, i=0, j=0, length=0;
	char len[10];

	//enter infinite loop
	while(1) {

		printf("? ");

		//clear the buffer after every command entry
		for(j=0; buffer[j]!='\0'; j++) {
			buffer[j]='\0';
		}

		//store every character entered in the buffer
here:	while((buffer[i]=getchar())!='\r') {

			// Condition to remove heading space
			if(buffer[0] == ' ') {
				buffer[0]='\0';
				goto here;
			}

			//condition to check backspace entry
			if(buffer[i]==8) {
				flag=2;				//set flag value if backspace entered
			}

			// Condition to remove trailing space
			if(buffer[i]==' ' && flag==1) {
				buffer[i]='\0';
				goto here;
			}

			//echo back the character entered in terminal
			printf("%c",buffer[i]);

			//check if backspace is entered
			if(flag==2) {
				flag=0;
				buffer[i]='\0';			//delete the previously entered character from buffer
				i--;
				goto here;
			}

			//check if a capital character is entered
			if(buffer[i]>=65&&buffer[i]<=90) {
				buffer[i]=buffer[i]+32;				//modify the capital character into lower case
			}

			i++;

			// Condition to check trailing space
			if(!(strcmp(buffer,"author"))) {
				flag=1;
				goto here;
			}

		}

		//resetting flag value
		flag=0;
		//remove enter character from the end of the buffer
		buffer[i]='\0';

		//Compare the input && to add another command in the terminal add another strcmp as shown in the commented example
		ret=strcmp(buffer,"author");
		ret1=strncmp(buffer,"dump ",5);
		ret2=strcmp(buffer,"help");
		//retx = strcmp(buffer,"XXX"); //where XXX is the command to match the input with.

		//output if author command is entered
		if(ret==0){
			printf("\n\rSaloni Shah\n\r");
			ret = 1;
		}

		//check if dump cpmmand is entered
		else if(ret1==0) {
			strncpy(dump_buf,buffer,4);						//copy string "dump" in dump_buf
			sscanf(buffer,"%s %x %s",dump_buf,start,len);	//scan the dump command inputs: dump <start address> <data length>
			printf("\n\r");
			if(len[1]=='x' || len[1]=='X') {				//check if length is entered in hexadecimal value
				sscanf(len,"%x",&length);					//convert hexadecimal length value in integer
			}
			else {
				sscanf(len,"%d",&length);					//take length value in integer
			}
			hexdump(buffer, strlen(buffer), (int*)start, length);	//call hexdump function for data dump
			ret1=0;
		}

		//check if help command is entered
		else if(ret2==0) {
			//print list of all supported commands in UART
			printf("\n\rThe commands supported by UART implementation are:\n\r");
			printf("1. author - To display the name of the author of the project\n\r");
			printf("2. dump <start> <len> - To dump the data on specified memory addresses where\n\r\t"
					"'start' is the absolute start address and 'len' is desired length of dump\n\r");
			printf("3. help - To display supported commands\n\r");
			ret2=1;
		}

		//output if any other command than the supported commands entered
		else {
			printf("\n\rUnknown Command: %s\n\r",buffer);
		}

		i=0;
		}
}

int main(void)
{
	//initialize system clock
	sysclock_init();

	//initialize UART communication
	init_UART0();

	//set interrupt priority for UART, clear all pending interrupts and enable interrupt request
	NVIC_SetPriority(UART0_IRQn, 2);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	//welcome message for user at the start of the program
	printf("Welcome to BreakfastSerial!\n\r");

	//call function to execute commands
	command_execution();

	return 0 ;
}
