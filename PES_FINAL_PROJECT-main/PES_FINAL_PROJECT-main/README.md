# PES_FINAL_PROJECT
'''
For the final project for this course, I worked on the in-built MMA8451Q accelerometer on KL25Z4. The basic idea of the project is to develop a level sensor using the accelerometer which will detect the movement of object across Y-axis in upward or downward direction and the total shift in unit distance from a reference position. 
Functionalities demonstrated in the project: 
- On board startup, it will test for correctness of I2C device initialization and a “bubble level” demo is implemented to test the working of the accelerometer. In this demo, when the board is flat Red LED will be on and when it will be tilted across X-axis or Y-axis the RGB colour will change from red to green or red to blue based on the tilt angle. This initialization sequence runs for 10 seconds. (This demo is similar to the one executing on the first-time startup of KL25Z board) 
- After this initialization sequence, I have also implemented a sequence to test the capacitive touch sensor on the board. Whenever the user touches the slider, a message will be echoed on the terminal indicating which part of slider was touched. This test sequence also runs for 10 seconds
- After testing both functionalities, the user can keep the board to a particular position and set it a as a reference level ‘0’ by the touch on the capacitive touch sensor provided on the board. 
- After setting the reference position, when the user moves the board in upward direction it will detect the upward motion and indicate the level shift of the board. If the user moves the board in downward direction, the level shift will change accordingly. 
- Furthermore, if during upward or downward movement the board is tilted then the led will turn red to indicate angle shift. 
- At any point of time when the system is running, if the user feels that the system output is inaccurate, they can press a push-button installed on board to recalibrate the level ‘0’ by touching the capacitive touch sensor again. 
- In debug mode, the system will print debug statements to indicate initialization of accelerometer, touch on capacitive touch sensor and setting of reference level, change in level and tilt angle.
