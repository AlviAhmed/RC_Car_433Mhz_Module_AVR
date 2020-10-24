// *******************************************************
//AVR_USART_ATMEGA328p_TX
// *******************************************************
//Specs of the USART:
// 1 start bit
// 8 data bits 
// no parity
// 1 stop bit
#include <stdio.h>
#include <avr/io.h> 
#include <avr/fuse.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h> 

#define prt PORTB
#define ddr DDRB

#define BAUD 9600 //same rate as in the Makefile baud is in bits per second
#define ubbrn ((F_CPU/16/BAUD) - 1)

#define but3 (~PINB) & (1 << PB5) // PCINT5
#define but2 (~PINB) & (1 << PB4) // PCINT4
#define but1 (~PINB) & (1 << PB3) // PCINT3
#define but0 (~PINB) & (1 << PB2) // PCINT2
#define butTog (~PINB) & (1 << PB1) // PCINT0

/* #define but3 (~PIND) & (1 << PD4) // PCINT20 */
/* #define but2 (~PIND) & (1 << PD5) // PCINT21 */
/* #define but1 (~PIND) & (1 << PD6) // PCINT22 */
/* #define but0 (~PIND) & (1 << PD3) // PCINT19 */
/* #define butTog (~PINB) & (1 << PB0) // PCINT0 */



#define syncByte 0xAA

volatile int pressed = 0;
volatile char ar = 'n';
volatile uint8_t txByte = 0x00;
volatile uint8_t rxSerNum = 0x0C;

uint8_t i = 0;
int enable = 1;
volatile int ser_bool = 0;
uint8_t num1 = 0x0C;
uint8_t num2 = 0x1F;



void txPacket(uint8_t rxbyte, uint8_t command){
    txByte = syncByte;
    txByte = rxbyte;
    txByte = command;
}


int main(void){

    DDRB |= (1 << PB0);
    PORTB &=~ (1 << PB0);
    DDRD |= (1 << PD2) | (1 << PD3);
    PORTB &=~ (1 << PD2) | (1 << PD3);

    DDRB &=~ (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1);
    PORTB |= (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1);

    //High and low bits
    UBRR0H = (ubbrn >> 8); 
    UBRR0L = ubbrn; 
    //////////////// 

//transimit and recieve enable
    UCSR0B = (1 << TXEN0) | (1 << TXCIE0); //enabling tranmission interrupt
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  //8 bit data format

    UDR0 = 0;
    
// Pin interrupt config
    PCMSK0 |= (1 << PCINT5) | (1 << PCINT4) | (1 << PCINT3) | (1 << PCINT2) | (1 << PCINT1);
    PCICR |= (1 << PCIE0);
    sei();
    
    while (1){
        if (ser_bool == 0){
            rxSerNum = num1;
            PORTD |= (1 << PD2);
            PORTD &=~ (1 << PD3);
        }
        else if (ser_bool == 1){
            rxSerNum = num2;
            PORTD &=~ (1 << PD2);
            PORTD |= (1 << PD3);
        }

    }
}  


ISR(PCINT0_vect){
    _delay_ms(5);
    if (but3){
        txByte = syncByte;
        txByte = 0x04;
        PORTB |= (1 << PB0);
    }
    else if (but2){
        txByte = syncByte;
        txByte = 0x03;
        /* ar = '2'; */
        PORTB |= (1 << PB0);
    }
        
    else if (but1){
        txByte = syncByte;
        txByte = 0x02 ;
       /* ar = '1'; */
        PORTB |= (1 << PB0);
    }
    else if (but0){
        txByte = syncByte;
        txByte = 0x01;
        /* ar = '0'; */
        PORTB |= (1 << PB0);
    }
    else if (butTog){
        ser_bool = !ser_bool;
        PORTB |= (1 << PB0);
    }
    else {
        txByte = syncByte;
        txByte = 0x05;
        PORTB &=~ (1 << PB0);
    }

}

ISR(USART_TX_vect)
{
    UDR0 = txByte;
}
