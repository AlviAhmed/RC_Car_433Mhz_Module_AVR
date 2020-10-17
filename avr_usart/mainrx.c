// *******************************************************
//AVR_USART_ATMEGA328p_RX
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

#include <util/delay.h>
#include <avr/interrupt.h> 
#define prt PORTB
#define ddr DDRB
#define pin PB0
#define BAUD 9600 //same rate as in the Makefile baud is in bits per second
#define ubbrn ((F_CPU/16/BAUD) - 1)
#define buffer_size 128

#define syncByte 0xAA

char rxbuffer[buffer_size];
uint8_t rxreadpos = 0;
uint8_t rxwritepos = 0;
uint8_t rxSerialNum1 = 0x1F;
uint8_t serialByte = 0x1F;
char ar[128]= " hello ";
uint8_t i = 0;
int enable = 1;
char c = '\0';

uint8_t recieveByte(){
    while( ((UCSR0A & (1<<RXC0)) == 0) && (UDR0 == 0x00) );
  return UDR0;
}
volatile uint8_t rxByte = 0x00;

int main(void){
    DDRB |= (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1);
    PORTB &= ~ (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2);
    PORTB |= (1 << PB1);
    //High and low bits
    UBRR0H = (ubbrn >> 8); 
    UBRR0L = ubbrn; 
    ////////////////
    uint8_t addr = 0x00;
    //transimit and recieve enable
    UCSR0B =  (1 << RXEN0) | (1 << RXCIE0);
    // UCSR0B = (1 << RXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  //8 bit data format
    sei();
    ////////////////////////////////////////////////////////////////
    UDR0 = 0;
    while (1){
            addr = rxByte;
            if (addr == (0x04 + rxSerialNum1))
            { 
                PORTB |= (1 << PB5);
            }
            else if (addr == (0x03 + rxSerialNum1))
            {
                PORTB |= (1 << PB4);
            }
            else if (addr == (0x02 + rxSerialNum1))
            {
                PORTB |=  (1 << PB3);
            }
            else if (addr == (0x01 + rxSerialNum1))
            {
                PORTB |= (1 << PB2);
            }
            else if (addr == 0x05){ 
                PORTB &=~ (1 << PB5);
                PORTB &=~ (1 << PB4);
                PORTB &=~ (1 << PB3);
                PORTB &=~ (1 << PB2);
            }
        }
}  

ISR(USART_RX_vect)
{
    rxByte = UDR0;
}

