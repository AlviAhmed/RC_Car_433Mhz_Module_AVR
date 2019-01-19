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
		_delay_ms(15);
		if (adcval >= 180){ 
			PORTB = (PORTB & 0x0F) | 0x08;  //PB3, making sure only pin turned on and nothing else
		} 
		else if (adcval > 145&& adcval <= 165){  
			PORTB = (PORTB & 0x0F) | 0x04; //PB2
		} 
		else if (adcval > 95 && adcval <= 115){  
			PORTB = (PORTB & 0x0F) | 0x02; //PB1

		}
		else if (adcval > 50 && adcval <= 65){  
			PORTB = (PORTB & 0x0F) | 0x01; //PB0

		} 
		else {  

			PORTB &=~  0x0F; //turning of all leds PB3 - PB0

		}
    	}  


     } 


void init(){  
	DDRB &=~ (1<<PB4);  //setting PB4(ADC2) as input for the RX signal   
	DDRB |= 0x0F;  //setting up leds
	PORTB &=~ 0x0F; //initially turning all leds off
	ADCinit();
	sei();
}   


void ADCinit(){   
	ADMUX &= ~ (1 << REFS1) | (1 << REFS0); //Vcc as reference, remeber to put decoupling capacitor in the aref pin
	ADMUX |= (1<<ADLAR)|(1<<MUX1); //left shifting ADC reg, enabling ADC2 (PB4) for input adc
	ADCSRA |= (1<<ADEN)|(1<<ADATE)|(1<<ADIE)|(1<<ADSC)|(1<<ADPS1)|(1<<ADPS0);//adc enable, auto trigger, enable interrupt 
	ADCSRB &=~ (1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0);
	//prescaler of 8, thus with 1Mhz clock -> 125 kHz clock
	startconvo();
} 

void startconvo(){ 
ADCSRA |= (1<<ADSC); //used to initialize convo
}

ISR(ADC_vect){  
	adcval = ADCH;//updating value of adcval

}
 
