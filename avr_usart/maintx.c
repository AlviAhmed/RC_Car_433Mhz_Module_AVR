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

#define syncByte 0xAA


volatile int pressed = 0;
volatile char ar = 'n';
volatile uint8_t txByte = 0x00;
volatile uint8_t rxSerNum = 0x0C;
uint8_t i = 0;
int enable = 1;
int tx_bool = 0;
uint8_t num1 = 0x0C;
uint8_t num2 = 0xAA;


void txPacket(uint8_t rxbyte, uint8_t command){
    txByte = syncByte;
    txByte = rxbyte;
    txByte = command;
}


int main(void){

    DDRB |= (1 << PB1);
    PORTB &=~ (1 << PB1);

    //High and low bits
    UBRR0H = (ubbrn >> 8); 
    UBRR0L = ubbrn; 
    //////////////// 

//transimit and recieve enable
    UCSR0B = (1 << TXEN0) | (1 << TXCIE0); //enabling tranmission interrupt
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  //8 bit data format

    UDR0 = 0;
    
// Pin interrupt config
    PCMSK0 |= (1 << PCINT5) | (1 << PCINT4) | (1 << PCINT3) | (1 << PCINT2);
    PCICR |= ( 1 << PCIE0 );
    sei();
    
    while (1){
    }
}  


ISR(PCINT0_vect){
    _delay_ms(5);

    if (but3){
        // txPacket(rxSerialNum1,0x04);
        // txByte = syncByte;
        txByte = 0x04 + rxSerNum;
        // ar = '3';
        PORTB |= (1 << PB1);
    }
        
    else if (but2){    
        // txPacket(rxSerialNum1,0x03);
        // txByte = syncByte; 
        txByte = 0x03 + rxSerNum;
        // ar = '2';
        PORTB |= (1 << PB1);
    }
        
    else if (but1){
        // txPacket(rxSerialNum1,0x02);
        // txByte = syncByte; 
        txByte = 0x02 + rxSerNum;
       // ar = '1';
        PORTB |= (1 << PB1);
    }
        
    else if (but0){    
        // txPacket(rxSerialNum1,0x01);
        // txByte = syncByte; 
        txByte = 0x01 + rxSerNum;
        // ar = '0';
        PORTB |= (1 << PB1);
    }
        
    else {   
        // txPacket(rxSerialNum1,0x05);
        // txByte = syncByte; 
        txByte = 0x05;
        // ar = 'n';
        PORTB &=~ (1 << PB1);
    }

}

ISR(USART_TX_vect)
{
    // UDR0 = ar;
    UDR0 = txByte;
}

