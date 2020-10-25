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

#define bufferSize 3

volatile int pressed = 0;
volatile char ar = 'n';

/* volatile uint8_t txByte = 0x00; */
/* volatile uint8_t rxSerNum = 0x0C; */
/* volatile uint8_t syncByte = 0xAA; */

volatile int ser_bool = 0;
volatile uint8_t rxSerNum = 0x1F;
volatile uint8_t syncByte = 0xAA;
volatile uint8_t num1 = 0x1F;
volatile uint8_t num2 = 0x0C;
volatile uint8_t txByte = 0x00;

volatile uint8_t txBuffer[bufferSize];
volatile int  txWritePos = 0;
volatile int  txReadPos = 0;

void nullByteIfEmpty(){
    if (UCSR0A & (1 << UDRE0)){
        UDR0 = 0x00;
    }
}


void txPacket(uint8_t rxbyte, uint8_t command){
    txBuffer[0] = syncByte;
    txBuffer[1] = rxbyte;
    txBuffer[2] = command;
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
        txPacket(0x1F, 0xDA);
        PORTB |= (1 << PB0);
    }
    else if (but2){
        txPacket(0x1F, 0xCB);
        PORTB |= (1 << PB0);
    }
    else if (but1){
        txPacket(0x1F, 0x62);
        PORTB |= (1 << PB0);
    }
    else if (but0){
        txPacket(0x1F, 0xFA);
        PORTB |= (1 << PB0);
    }
    else if (butTog){
        ser_bool = !ser_bool;
        PORTB |= (1 << PB0);
    }
    else {
        txPacket(0x1F, 0x05);
        PORTB &=~ (1 << PB0);
    }
}

ISR(USART_TX_vect) // once tx buffer is clear, set clear bit to accept new data
{
    if (txReadPos != bufferSize){ //now we are reading the info from the buffer index by index to be transmitted
        UDR0 = txBuffer[txReadPos]; //tx read pos just used to index the buffer data to be transmitted
        txReadPos ++;
        if (txReadPos >= bufferSize){
            txReadPos = 0;
        }
    }
}
