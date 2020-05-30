This repository is for the code and files I wrote regarding BLDC motor control using Sine PWM. 

The arduino code is used on an RCTIMER 1.0 BLDC gimbal board to drive up to two brushless motors. Approximations for one period of sine are calculated using the included python script.
The python script writes an array of sine values in the proper format for arduino to a text file for easy copy-pasting. The size of the array and length of each line is changed in 
a single variable near the top. 

ISR_counter_delay calculates the length of delay needed in the interrupt service routine to reach a desired linear velocity as outlined in the code comments. Make sure the 
setMotorPosition function is called at least once per loop, and don't put anything extraneous, especially Serial monitor activity, in the ISR. 

I tried to implement a function that would convert the desired RPM from the user into a delay for the ISR, but it doesn't seem to work right and for now
I have exhausted my interest in the project. 

Here is a link to the accompanying video of my results: https://bit.ly/3gx4dYF

 

