# RC_tiny85

This is an attempt to create an RC car using some 433Mhz modules, 2 attiny 85 microcontrollers, a couple of H-bridges and motors. 

I am planning on making this a tank like car, therefore will turn and move like a tank.  

My first hurdle is establishing communication between 2 attiny 85, at first I used ADC to interpret the PWM signal from another attiny85, that didn't work so I moved on to using hardwire interrupts and timing the high signal of the PWM. 

Using the hardware interrupt method, I had my first success when I was able to make the attiny differentiate between a duty cycle of 200 among other duty cycles and be able to light up a corresponding LED.  

After tweaking with duty cycle and other values in the code, I got the RC car to work relaibly while it is connected to the PWM pin of the attiny85 on the control board. 

However when I use the 433mHz modules and try to go wireless, the motors jitter a lot and the overall performance goes down.  

After doing some measurements on the input of the reciever attiny85, I found that the signal is very unstable. Therefore my possible 
solutions are to use an amplify and/or a type of unity gain buffer on both the transmitter and reciever. 

If the code side is the issue then my plan will be to find a different mode of communication between the two attiny's. I feel that simply varying the duty of a pwm signal may not be the best solution for sending data through a signal.  

Instead it would be better to have some other communication protocols/encoding systems like SPI, USART, UART, ICP,  
Manchester Encoding, etc and see how they fair in wireless communication. 

Hackaday: https://hackaday.io/project/163719-rc-car

All files in the repository are licensed under CC BY-NC-SA 4.0