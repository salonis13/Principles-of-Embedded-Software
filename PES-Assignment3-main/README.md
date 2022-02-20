# PES-Assignment3
```PES – ECEN 5813 – ASSIGNMENT 3 – BLINKENLIGHTS
SUBMITTED BY – SALONI SHAH
UNIVERSITY OF COLORADO BOULDER


In this assignment, we get to play with FRDM-KL25Z.  
Here, we are writing to the GPIO pins to control the tri-color LED on the Freedom board, 
as well as reading from the touch sensor register. 
This program is a bare-metal C executable that runs on the KL25Z. In this program:
1)	At program startup, we “test” the LED by blinking in the following pattern:
RED for 500 msec, OFF for 100msec, 
GREEN for 500 msec, OFF for 100 msec,
BLUE for 500 msec, OFF for 100 msec
WHITE (that is, RED, GREEN, and BLUE all on) for 100 msec, OFF for 100 msec
WHITE for 100 msec, OFF for 100 MSEC

2) Then we enter an infinite loop where the LED is flashed in the following pattern:
ON for 500 msec, OFF for 500 msec, 
ON for 1000 msec, OFF for 500 msec,
ON for 2000 msec, OFF for 500 msec,
ON for 3000 msec, OFF for 500 msec

3)During the infinite loop, the color when the LED is ON will initially be white. 
If the user touches the capacitive touch slider, the color will change as follows:
User touches Left side of slider – colour changes to Red
User touches Center of slider – colour changes to Green
User touches Right side of slider – colour changes to Blue

Here, the program is polling the slider every 100msec to detect any touch immediately
and change the led colour.

 4)The program is built in two modes, DEBUG and RUN. 
 The LED and touch behavior are the same under both targets; 
 however, the DEBUG target additionally causes the following debug output
 (via printf) to a serial console:
      §CHANGE LED TO RED, etc. for changes to the LEDcolor
      §SLIDER VALUE 87 for changes to the slider value including showing the value read
      §START TIMER 2000 for starting a 2000 mSec delay






EXTRA CREDIT

Address of main() function – 0x8a4

Size of delay() function – 40 bytes

```00000410 <delay>:
     410:	b580      	push	{r7, lr} ;push address value on stack pointer
     412:	b082      	sub	sp, #8    ;reserve 8 bytes in stack pointer for variables
     414:	af00      	add	r7, sp, #0 ;store stack pointer adrress in r7
     416:	6078      	str	r0, [r7, #4] ;store value of msec in r0
     418:	687b      	ldr	r3, [r7, #4] ;store value of msec in r3
     41a:	4a07      	ldr	r2, [pc, #28]	; (438 <delay+0x28>), store delay_count in r2
     41c:	4353      	muls	r3, r2     ;multiply value of msec*delay_count
     41e:	607b      	str	r3, [r7, #4]  ;store new value of msec in r3
     420:	e000      	b.n	424 <delay+0x14>  ;check if value of msec is 0
     422:	46c0      	nop			; (mov r8, r8)  ;no operation until value of msec equals 0
     424:	687b      	ldr	r3, [r7, #4]    ;store value of msec in r3
     426:	1e5a      	subs	r2, r3, #1    ;subtract one from msec everytime while loop is executed
     428:	607a      	str	r2, [r7, #4]  ;store that value in r2
     42a:	2b00      	cmp	r3, #0        ;compare the value of msec with 0
     42c:	d1f9      	bne.n	422 <delay+0x12> ;continue no operation if msec not equal to 0
     42e:	46c0      	nop			; (mov r8, r8) no operation for one instruction cycle
     430:	46bd      	mov	sp, r7  ;move return address to stack pointer
     432:	b002      	add	sp, #8  ;space for variables
     434:	bd80      	pop	{r7, pc}  ;pop address from stack pointer
     436:	46c0      	nop			; (mov r8, r8)
     438:	000009c4 	.word	0x000009c4```
