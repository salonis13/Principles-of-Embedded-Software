# PES-Assignment4
```I have developed a new traffic light for export to the little-known tropical island of Buffahiti. 
The residents of this island are widely known for their fondness of bison. 
Due to a genetic mutation, they also exhibit certain color-blindness, 
and as a result their traffic lights have the following unusual behavior:
-	When the traffic light is in the STOP state, Buffahiti residents use the color #611E3C
-	When the traffic light is in the GO state, Buffahiti residents use the color #229622
-	When the traffic light is in the WARNING state, Buffahiti residents use the color #FFB200
-	When someone presses the button to cross the street (CROSSWALK state), 
   the traffic light is blinking with the color #001030. 
-	In the CROSSWALK state, the light blinks 250 msec off, 750 msec on.
While changing the color of the LED after TIMEOUT, the light should transition
from the current color to next color over a period of 1 second. 
In order to speed development, a special DEBUG mode is used. 
While in DEBUG mode, light timing is shorter, and various diagnostic printouts may be delivered 
from the device as needed over the UART. Timing for the traffic light is given below:
-DEBUG 			      
STOP			5 sec				       
One sec transition to GO state
GO				5 sec				      
One sec transition to WARNING state
WARNING		3 sec				       
One sec transition to STOP state
-PRODUCTION 			      
STOP			20 sec				       
One sec transition to GO state
GO				20 sec				      
One sec transition to WARNING state
WARNING		5 sec				       
One sec transition to STOP state
At any point in the above cycle, if a resident presses the button to cross the street 
(represented on our FRDM-KL25Z by a touch anywhere on the capacitive slider), 
the traffic light should do the following:
-	Transition, over a one-second period, from whatever the current color is to the CROSSWALK color (#001030).
-	Stay in the CROSSWALK mode for 10 seconds, blinking 250 msec off, 750 msec on.
   (This duration is the same in both DEBUG and PRODUCTION modes.)
-	Transition, over a one-second period, from the CROSSWALK state to the GO state.
Button presses are ignored if they arrive when the light is either transitioning into 
or is already in the CROSSWALK state. Otherwise, the transition into the CROSSWALK state must start 
within 100 msec of a button press occurring. When in DEBUG mode, print 
the following to the diagnostic output:
-	Main loop is starting
-	All state transitions, with the system time of the event(msec since startup), 
  and the names of the state being transitioned from and to
-	Button press detected, with the system time (msec since startup)```

PEER CODE REVIEW
- Partner - Michelle Christian
- Code reviewed on 7th March 2021 14:00
- No changes suggested in the code
