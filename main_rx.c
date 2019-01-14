#define F_CPU 1000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>   
adcval = 0;
int main (void) { 
	init();
    	while (1)  
        { 	        
		if (adcval >= 141 && adcval <= 150){ 
			PORTB |= (1 << PB3); //will soon replace with functions and states 
			//have to turn of the rest of leds 
			//ex PORTB = (PORTB & 0x00)|3 <- this will turn on just 3 and turn of the rest
		} 
		else if (adcval >= 104 && adcval <= 114){  
			PORTB |= (1 << PB2); 
			//PORTB = (PORTB & 0x00)|2;
		} 
		else if (adcval >= 68  && adcval <= 76){  
			PORTB |= (1 << PB1); 
			//PORTB = (PORTB & 0x00)|1;

		}
		else if (adcval >= 31 && adcval <= 40){  
			PORTB |= (1 << PB0); 
			//PORTB = (PORTB & 0x00)|0;

		} 
		else { 
			PORTB &=~  (1<<PB3) | (1<<PB2) | (1<<PB1) | (1<<PB0);   
			//PORTB = 0x00;

		}
    	}  


     } 


void init(){  
	DDRB &=~ (1<<PB4);  //setting PB4(ADC2) as input for the RX signal   
	DDRB |=  (1<<PB3) | (1<<PB2) | (1<<PB1) | (1<<PB0);  //setting up leds
	PORTB &=~  (1<<PB3) | (1<<PB2) | (1<<PB1) | (1<<PB0);  
	ADCinit();
	sei();
}   

void startconvo(){ 
ADCSRA |= (1<<ADSC); //used to initialize convo
}

void ADCinit(){  
	ADMUX |= (1<<ADLAR)|(1<<MUX1); //left shifting ADC reg, enabling ADC2 (PB4) for input adc
	ADCSRA |= (1<<ADEN)|(1<<ADATE)|(1<<ADIE);//adc enable, auto trigger, enable interrupt
	//default for ADCSRB is free running mode 
	ADCSRA |= (1<<ADPS1)|(1<<ADPS0);//prescaler of 8, thus with 1Mhz clock -> 125 kHz clock
	startconvo();
} 

ISR(ADC_vect){ 
	adcval = ADCH;//updating value of adcval

}
 
