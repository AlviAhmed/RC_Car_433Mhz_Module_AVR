#define F_CPU 1000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>  
#define but3 (~PINB) & (1 << PB3) 
#define but2 (~PINB) & (1 << PB2) 
#define but1 (~PINB) & (1 << PB1) 
#define but0 (~PINB) & (1 << PB0) 
int duty = 10;
int main (void) { 
	init();
    	while (1)  
        { 	       


     	} 

}
void init(){  
	DDRB |= (1 << PB4);     
	DDRB &= ~ (1 << PB3) | (1 << PB2) | (1 << PB1) | (1 << PB0);  
	PORTB |= (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1) | (1 << PB0);    
	GIMSK |= (1 << PCIE); 
	PCMSK |= (1 << PCINT3) | (1 << PCINT2) |(1 << PCINT1) | (1 << PCINT0);
	GTCCR |= ((1 << COM1B1) | (1 << PWM1B));  //PB4 enabled for output of pwm
	TIMSK |= (1 << TOIE1); 
	sei(); 
	TCCR1 |= ((1 << CS12) | (1 << CS11) | (1 << CS10)); //prescaler of 64, 61.0352 Hz
} 


ISR(TIMER1_OVF_vect){ 
	OCR1B = duty; 

}  
 

ISR(PCINT0_vect){ 

	_delay_ms(10); 
//button number corresponds with pin	
	if (but3){    
		duty = 200; //2.924 on rx
	}
	
	else if (but2){    
		duty = 150;//2.183 on rx
	}

	else if (but1){    
		duty = 100;//1.438 on rx
	}

	else if (but0){    
		duty = 50;//0.714 on rx
	}

	else {   
		duty = 10;//0.141 on rx
	} 
	
	
} 

