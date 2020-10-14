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

char rxbuffer[buffer_size];
uint8_t rxreadpos = 0;
uint8_t rxwritepos = 0;

char ar[128]= " hello ";
uint8_t i = 0;
int enable = 1;
char c = '\0';

char peekChar(void){
    char ret = '\0';
    if (rxreadpos != rxwritepos){
        ret = rxbuffer[rxreadpos];
    }
    return ret;
}

char getChar(void)
{
    char ret = '\0';
    if (rxreadpos != rxwritepos){
        ret = rxbuffer[rxreadpos];
        rxreadpos ++;

        if (rxreadpos >= buffer_size){
            rxreadpos = 0;
        }
    }
    return ret;
}



int main(void){
    DDRB |= (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1);
    PORTB &= ~ (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2);
    PORTB |= (1 << PB1);
    //High and low bits
    UBRR0H = (ubbrn >> 8); 
    UBRR0L = ubbrn; 
    //////////////// 
    //transimit and recieve enable
    UCSR0B =  (1 << RXEN0) | (1 << RXCIE0);
    // UCSR0B = (1 << RXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  //8 bit data format
    sei();
    ////////////////////////////////////////////////////////////////
    UDR0 = 0;
    while (1){
        
        if (c == '3')
        { 
            PORTB |= (1 << PB5);
        }
        else if (c == '2')
        {
            PORTB |= (1 << PB4);
        }
        else if (c == '1')
        {
            PORTB |=  (1 << PB3);
        }
        else if (c == '0')
        {
            PORTB |= (1 << PB2);
        }
        else if (c == 'n'){ 
            PORTB &=~ (1 << PB5);
            PORTB &=~ (1 << PB4);
            PORTB &=~ (1 << PB3);
            PORTB &=~ (1 << PB2);

        }
        
    }
}  



ISR(USART_RX_vect)
{
    c = UDR0;
    // rxbuffer[rxwritepos] = UDR0;
    // rxwritepos ++;
    // if (rxwritepos >= buffer_size){
    //     rxwritepos =  0;
    // }
}

/* 
   for (i = 0; i < strlen(ar) ; i++){ 
   UDR0 = ar[i]; 
   } 
   if(UCSR0A & (1 << UDRE0))
   {
   UDR0 = 0;
   }
*/