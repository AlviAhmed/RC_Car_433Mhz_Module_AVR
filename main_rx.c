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
int state = 0;

int main (void) {  
	init();  
	OCR0A = 100;  //ctc mode will trigger at 0.1ms
    	while (1)  
        { 	     
		//PORTB = PB7 PB6 PB5 PB4 PB3 PB2 PB1 PB0  
		//PB2 => input for signal 
		//led ports => PB4, PB3, PB1, PB0  

		if (~butpress){ 
			//State 4 FWD
			if (tick_count > 14 && tick_count <= 18){  //when tick_counter = 16.1, means 1.601 ms gone by 
			//	PORTB = PORTB & (0x00) | (0x10); //turning off all leds other than PB4  
				PORTB = PORTB & (0x00) | (0x12);//just PB4 & PB1 
			}   
			//State 3 REV	
			else if (tick_count > 10 && tick_count <= 13){ //tick counter = 12.04 
				//PORTB = PORTB & (0x00) | (0x08); // //   ///               //     PB3 
				  PORTB = PORTB & (0x00) | (0x09);//PB3 & PB0
			}  
			//State 2 LEFT
			else if (tick_count > 6 && tick_count <= 10){ //tick counter = 8.0314
				//PORTB = PORTB & (0x00) | (0x02); // //   ///               //     PB1 
				PORTB = PORTB & (0x00) | (0x0A);//PB3 & PB1
			} 
			//State 1 RIGHT
			else if (tick_count > 2 && tick_count <= 6){ //tick counter = 4.0
				//PORTB = PORTB & (0x00) | (0x01); // //   ///               //     PB0 
				PORTB = PORTB & (0x00) | (0x11);//PB0 & PB4
			} 
			//State 0 OFF
			else if (tick_count <= 1) { //tick counter = 0.8 (default signal)
				PORTB &=~ (0xFF);  //turn of all bits
			}     
			tick_count = 0; //setting tick count back to zero after pulse hits low 
		}
    	}  


     } 


void init(){   
	// ******* button init 
	DDRB &=~ (1<<PB2);  //setting PB4(ADC2) as input for the RX signal     

	// ******* led init
	DDRB |= (0x1B);  //setting up leds
	PORTB &=~ (0x1B); //initially turning all leds off  

	//*********INT0 init	
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
	} 
	else { 
		TCCR0B &=~ (1 << CS00);  //turning off ctc mode when pulse reaches low, since want to start and stop timer 
		// in relation to pulse 
	}
}  

ISR ( TIMER0_COMPA_vect){  
	tick_count ++; //amount of times 0.1ms has passed
}

