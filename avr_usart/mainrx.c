// *******************************************************
//AVR_USART_ATMEGA328p_RX
// *******************************************************
//Specs of the USART:
// 1 start bit
// 8 data bits 
// no parity
// 1 stop bit
#define F_CPU 16000000UL

#include <stdio.h>
#include <avr/io.h> 
#include <avr/fuse.h>
#include <stdio.h>

#include <util/delay.h>
#include <avr/interrupt.h> 
#define BAUD 9600 //same rate as in the Makefile baud is in bits per second
#define ubbrn ((F_CPU/16/BAUD) - 1)
#define buffer_size 128

#define bufferSize 3


volatile char rxBuffer[bufferSize] = {'0', '0', '0'};
volatile int  rxWritePos = 0;
volatile int  rxReadPos = 0;

volatile char rxSerNum = '1';

volatile char num1 = '1';
volatile char num2 = '2';
volatile  char ser = '0';
volatile char cmd = '0';
volatile char syn = '0';


volatile int enable = 0;

/*
  PB4 -> 0x44
  PB3 -> 0xCB 
  PB2 -> 0x62 
  PB1 -> 0xFA 
  else -> 0x05
*/

void serial_debug(){
    _delay_ms(10);
    UDR0 = 'x';
    _delay_ms(10);
    UDR0 = syn;
    _delay_ms(10);
    UDR0 = 'y';
    _delay_ms(10);
    UDR0 = ser;
    _delay_ms(10);
    UDR0 = 'z';
    _delay_ms(10);
    UDR0 = cmd;
    _delay_ms(10);
}

int main(void){
    DDRB |= (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1);
    PORTB &= ~ (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | ( 1 << PB1 );
    //High and low bits
    UBRR0H = (ubbrn >> 8); 
    UBRR0L = ubbrn; 
    ////////////////
    uint8_t addr = 0x00;

    UCSR0A = 0x00;
    //transimit and recieve enable
    UCSR0B =  (1 << RXEN0) | (1 << RXCIE0) | (1 << TXEN0) | (1 << TXCIE0);
    // UCSR0B = (1 << RXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  //8 bit data format
    sei();
    ////////////////////////////////////////////////////////////////
    UDR0 = 0;
    int delay_num = 10;
    while (1){
        if ( (enable == 1) ){
            switch (cmd){
            case ('f'):
                PORTB |= (1 << PB3);
                PORTB |= (1 << PB2);
                break;
            case('b'):
                PORTB |= (1 << PB4);
                PORTB |= (1 << PB1);
                break;
            case('l'):
                PORTB |=  (1 << PB1);
                PORTB |=  (1 << PB3);
                break;
            case('r'):
                PORTB |=  (1 << PB4);
                PORTB |=  (1 << PB2);
                break;
            case('n'):
                PORTB &=~ (1 << PB4);
                PORTB &=~ (1 << PB3);
                PORTB &=~ (1 << PB2);
                PORTB &=~ (1 << PB1);
                break;
            }
        }
        else{
            PORTB &=~ (1 << PB4);
            PORTB &=~ (1 << PB3);
            PORTB &=~ (1 << PB2);
            PORTB &=~ (1 << PB1);
        }
    }

    return 0;
}  

ISR(USART_RX_vect)
{
    rxBuffer[rxWritePos] = UDR0;
    UDR0 = rxBuffer[rxWritePos];
    if (rxBuffer[0] == 's'){
        switch (rxWritePos){
        case(0):
            syn = rxBuffer[rxWritePos];
            break;
        case(1):
            ser = rxBuffer[rxWritePos];
            if (ser == num1){
                enable = 1;
            }
            else{
                enable = 0;
            }
            break;
        case(2):
            cmd = rxBuffer[rxWritePos];
            break;
        }
    }
    else{
        rxWritePos = -1;
        PORTB &=~ (1 << PB4);
        PORTB &=~ (1 << PB3);
        PORTB &=~ (1 << PB2);
        PORTB &=~ (1 << PB1);
    }
    rxWritePos++;
    if (rxWritePos >= bufferSize){
        rxWritePos = 0;
    }
}

