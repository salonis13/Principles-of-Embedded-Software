# PES-Assignment7
```
In this assignment, we are playing different tones using DMA and DAC and analyzing the tone using ADC.
The code cycles through four musical notes: 440 Hz (an A4), 587 Hz (a D5), 659 Hz (an E5) and   
880 Hz (an A5). Each note is played out for one second. When playback reaches the end of this list,
it restarts at the beginning. 
	Once a note is playing, we use the ADC to read in 1024 samples and then analyze the readings.

Here, we are developing the following modules:
1.	A function to accurately calculate sin(x) using only integer math and lookup table. 
	We also write a test function for sin to calculate maximum error and sum of squares
2.	A tone to samples function to compute the samples representing a pure tone of a specified frequency 
	based on our sin(x) function and fill a buffer using these sample values.
3.	An audio output module in which we pass the computed buffer of samples. 
	We use DAC, DMA and TPM to play our tones continuously.
4.	To monitor playing of tones in 1 sec cycles, a systick module to check time.
5.	A audio input module to capture some input samples from DAC and store them in a buffer
6.	An audio analysis module to make computations on the audio input buffer like taking minimum,
	 maximum, average, samples and frequency of the audio. In ADC initialization, also calibrate the ADC module. 
	 For triggering ADC, we are setting a separate hardware trigger using TPM1.

Each time we pass through these modules a message will print on UART like;

Generated 109 samples at 440 Hz computed period=109 samples
min=74 max=65192 avg=34329 period=219 samples frequency=438 Hz
min=76 max=65186 avg=30570 period=220 samples frequency=436 Hz
min=69 max=65191 avg=30628 period=220 samples frequency=436 Hz

where the first line is printed after computing the output buffer of samples
The second line is printed after the tone starts playing and input values are stored in a buffer. 
After receiving every 1024 input values, analysis is carried out on that buffer.

Modules for code modularity:
 "autocorrelate.h" - Detect the period of the fundamental frequency in a buffer full of samples
 "tpm.h" - Initialize and start Timer/PWM modules for DAC and ADC
 "systick.h" - for tone timing analysis 
 "sin.h" - Calculate sin wave using integer math and testing sin function
 "audio_out.h" - Initialize DAC, DMA, store samples in buffer and play tones
 "audio_in.h" - Initialize DAC, DMA, store samples in buffer and play tones
