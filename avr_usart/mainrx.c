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

#define bufferSize 3


volatile uint8_t rxBuffer[bufferSize];
volatile int  rxWritePos = 0;
volatile int  rxReadPos = 0;

/* volatile uint8_t rxSerNum = 0x0C; */

void appendRx(uint8_t data_byte) //UDR0 will be the input of the function
{
    rxBuffer[rxWritePos] = data_byte;
    rxWritePos++; //increment global write position as you are inputting things into the array

    if (rxWritePos >= bufferSize){
        rxWritePos  = 0;
    }
}

int main(void){
    DDRB |= (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1);
    PORTB &= ~ (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | ( 1 << PB1 );
    /* PORTB |= (1 << PB1); */
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
        uint8_t syn = rxBuffer[0];
        uint8_t ser = rxBuffer[1];
        uint8_t cmd = rxBuffer[2];
        if ( (ser == 0x1F) ){
             switch (cmd){
                case (0xDA):
                    PORTB |= (1 << PB1);
                    break;
                case(0xCB):
                    PORTB |= (1 << PB4);
                    break;
                case(0x62):
                    PORTB |=  (1 << PB3);
                    break;
                case(0xFA):
                    PORTB |=  (1 << PB2);
                    break;
                case(0x05):
                    PORTB &=~ (1 << PB1);
                    PORTB &=~ (1 << PB4);
                    PORTB &=~ (1 << PB3);
                    PORTB &=~ (1 << PB2);
                    break;
            }
        }
        else{
                /* PORTB &=~ (1 << PB5); */
                /* PORTB &=~ (1 << PB4); */
                /* PORTB &=~ (1 << PB3); */
                /* PORTB &=~ (1 << PB2); */
        }
        }

    return 0;
}  

ISR(USART_RX_vect)
{
    if (rxReadPos != bufferSize){ //now we are reading the info from the buffer index by index to be transmitted
        rxBuffer[rxReadPos] = UDR0; //tx read pos just used to index the buffer data to be transmitted
        rxReadPos ++;
        if (rxReadPos >= bufferSize){
            rxReadPos = 0;
        }
    }


}
