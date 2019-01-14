#define F_CPU 1000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>   

int main (void) { 
	init();
    	while (1)  
        { 	       
    	}  


     } 


void init(){  
	DDRB &=~ (1 << PB4);  //setting PB4 as input for the RX signal   
	DDRB |=  (1 << PB3) | (1 << PB2) | (1 << PB1) | (1 << PB0);  
	PORTB |=  (1 << PB3) | (1 << PB2) | (1 << PB1) | (1 << PB0);   //will indicate which state is on
	sei(); 

}  

void ADCinit(){ 
	
}
 
