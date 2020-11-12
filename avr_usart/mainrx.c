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


volatile uint8_t rxBuffer[bufferSize] = {0x00, 0x00, 0x00};
volatile int  rxWritePos = 0;
volatile int  rxReadPos = 0;

volatile uint8_t rxSerNum = 0x0C;

volatile uint8_t num1 = 0x0C;
volatile uint8_t num2 = 0x09;
volatile  uint8_t ser = 0xAA;
volatile uint8_t cmd = 0x00;
volatile uint8_t syn = 0x00;

volatile int enable = 0;

char appendRx(void)
{
    char ret = '\0';
    if (rxReadPos != rxWritePos){
        ret = rxBuffer[rxReadPos];
        rxReadPos++;
        if(rxReadPos >= bufferSize){
            rxReadPos = 0;
        }
    }
    return ret;
}



/*
  PB4 -> 0x44
  PB3 -> 0xCB 
  PB2 -> 0x62 
  PB1 -> 0xFA 
  else -> 0x05
*/


void allOff(){
    PORTB &=~ (1 << PB4);
    PORTB &=~ (1 << PB3);
    PORTB &=~ (1 << PB2);
    PORTB &=~ (1 << PB1);
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
    UCSR0B =  (1 << RXEN0) | (1 << RXCIE0);
    //| (1 << TXEN0) | (1 << TXCIE0);
    // UCSR0B = (1 << RXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  //8 bit data format
    sei();
    ////////////////////////////////////////////////////////////////
    UDR0 = 0;
    int delay_num = 10;
    while (1){
        if ( (enable == 1) ){
            switch (cmd){
            case (0x44):
                PORTB |= (1 << PB3);
                PORTB |= (1 << PB2);
                break;
            case(0xCB):
                PORTB |= (1 << PB4);
                PORTB |= (1 << PB1);
                break;
            case(0x62):
                PORTB |=  (1 << PB1);
                PORTB |=  (1 << PB3);
                break;
            case(0xFA):
                PORTB |=  (1 << PB4);
                PORTB |=  (1 << PB2);
                break;
            case(0x05):
                PORTB |=  (1 << PB0);
                allOff();
                break;
            }
        }
        else{
            allOff();
        }
    }

    return 0;
}  

ISR(USART_RX_vect)
{
    rxBuffer[rxWritePos] = UDR0;
    if (rxBuffer[0] == 0xAA){
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
        allOff();
    }
    rxWritePos++;
    if (rxWritePos >= bufferSize){
        rxWritePos = 0;
    }
}

