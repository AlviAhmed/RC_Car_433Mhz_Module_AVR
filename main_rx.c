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
int const_state = 4;
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
		_delay_ms(5);
		if (~butpress){  
			if ( ~(tick_count > (const_state - 4) && tick_count <= (const_state + 4)) ){ /*if the value is NOT 
				within range, means tick count changed, update value and engage new state */
				const_state = tick_count;	
			}
			//State 4 FWD
			if (const_state > 90  && const_state <= 104){  //when tick_counter = 96.3764,gone by 
			//	PORTB = PORTB & (0x00) | (0x10); //turning off all leds other than PB4  
				PORTB = PORTB & (0x00) | (0x12);//just PB4 & PB1 
			}   
			//State 3 RIGHT
			else if (const_state > 79 && const_state <= 87 ){ //tick counter = 12.04 
				//PORTB = PORTB & (0x00) | (0x08); // //   ///               //     PB3 
				  PORTB = PORTB & (0x00) | (0x11);//PB3 & PB0
			}  
			//State 2 LEFT
			else if (const_state > 60 && const_state <= 68){ //tick counter = 8.0314
				//PORTB = PORTB & (0x00) | (0x02); // //   ///               //     PB1 
				PORTB = PORTB & (0x00) | (0x0A);//PB3 & PB1
			} 
			//State 1 REV
			else if (const_state > 28 && const_state <= 36){ //tick counter = 4.0
				//PORTB = PORTB & (0x00) | (0x01); // //   ///               //     PB0 
				PORTB = PORTB & (0x00) | (0x09);//PB0 & PB4
			} 
			//State 0 OFF
			else  { //tick counter = 0.8 (default signal)
				PORTB &=~ (0x1B);  //turn of all bits
			}     
		}
    	}  


     } 


void init(){   
	// ******* button init 
	DDRB &=~ (1<<PB2);  //setting PB4(ADC2) as input for the RX signal     

	// ******* led init
	DDRB |= (0x1B);  //setting up leds expect for PB2
	PORTB &=~ (0x1B); //initially turning all leds off expect for PB2

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
		tick_count = 0;
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

