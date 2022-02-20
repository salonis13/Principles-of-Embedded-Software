# PES-ASSIGNMENT5
```README DOCUMENT
This document states the final results and changes of the PBKDF2 assignment
GITHUB URL - https://github.com/saloni1307/PES-ASSIGNMENT5
.text size – 20692
Time taken – 2965 msec
All tests passed? - Yes
Changed made in the original code – 

1.	ISHA PROCESS MESSAGE BLOCK – 
Optimizing for loop and elimination of W[t] variable for faster computation
Combining two loops

2.	ISHA PAD MESSAGE –
Replacing loops with memset function to set data in memory
Optimization of if…else loop

3.	ISHA LOOP – 
Merging ISHA RESET, ISHA RESULT and ISHA INPUT functions to eliminate
return time and stack space utilization in HMAC ISHA

4.	ISHA RESULT – 
Elimination of loop to compute digest out by using type casting and 
__builtin_bswap32 function
Elimination of if condition
Limitation: program will give incorrect output in case of the 
message being corrupted while entering the loop

5.	ISHA INPUT – 
Computation of Length_Low outside while loop
Elimination of length high computation
Elimination of if condition 
Limitation: program will return incorrect output if the input length is 0, 
the message is corrupted while entering the function or if the message 
length becomes too long



6.	HMAC ISHA – 
Replacing loops to compute ipad, opad and keypad with memcpy and memset functions
Optimization of for loop to compute ipad and opad
Elimination of multiple function calls by calling ISHA LOOP only twice
Elimination of if else loop to save space and time
Limitation: program will not execute if key_len is more than ISHA_BLOCKLEN

7.	F – 
Replacing loop with memcpy function to compute result
Unrolling loop to XOR result with temp
Reducing array size of saltplus to save space
Limitation: cannot use saltplus for more than minimum required size

8.	PBKDF2 HMAC ISHA – 
Reducing array size of accumulator to save space
Replacing loop with memcpy to get data in DK
Limitation: Cannot use accumulator for more than minimum required size

9.	Other changes – 
Use of ‘register’ keyword to store variables in register instead of stack memory
for faster access, computation and saving memory space
