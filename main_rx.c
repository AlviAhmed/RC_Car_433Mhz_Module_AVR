#define F_CPU 1000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>    
#define starttime 
#define endtime 
#define count
#define counter 
#define butpress (PINB) & _BV(2)
int tick_count = 0;
int enable = 0; 

int main (void) {  
	init();  
	OCR0A = 100;  //ctc mode will trigger at 0.1ms
    	while (1)  
        { 	     
		//_delay_us(2);
//		TCCR0B &=~ (1 << CS00); //initially turn off the timer  
		if (tick_count > 14  && tick_count <= 20){  //when tick_counter = 16.1, means 1.601 ms gone by
			PORTB |= (1 << PB4); 
//			 
//			TCCR0B &=~ (1 << CS00); 
			tick_count = 0;	
		} 
	       	else { 
			PORTB &=~ (1 << PB4); 
		} 
    	}  


     } 


void init(){   
	//button init
	DDRB &=~ (1<<PB2);  //setting PB4(ADC2) as input for the RX signal    
	//led init
	DDRB |= (1 << PB4);  //setting up leds
	PORTB &=~ (1 << PB4); //initially turning all leds off  

	//INT0 init	
	MCUCR |=/* (1 << ISC01) |*/ (1 << ISC00);//ISC01 & ISC00 both 1, triggers INT0 on rising edge 
	GIMSK |= (1 << INT0);  

	//CTC Mode init
	TCCR0A |= (1 << WGM01); //CTC Mode, top OCRA 
	TIMSK |= (1 << OCIE0A); //interrupt enable when TCNT == OCRA

	sei();
}   

ISR(INT0_vect){    
	if (butpress) { 
		TCCR0B |= (1 << CS00); //turning on the timer, when pulse is high       
//		PORTB |= (1 << PB4);
	} 
	else { 
		TCCR0B &=~ (1 << CS00); 
		tick_count = 0; 
//		PORTB &=~ (1 << PB4);
	}
}  

ISR ( TIMER0_COMPA_vect){  
//	PORTB ^= (1 << PB4); //toggling led to test CTC
	tick_count ++; //amount of times 0.1ms has passed
}

