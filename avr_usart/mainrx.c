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


#define bufferSize 128
uint8_t rxBuffer[bufferSize];
uint8_t rxWritePos = 0;
uint8_t rxReadPos = 0;

uint8_t  rxSerialNum1 = 0xCF;
uint8_t rxCmdRec = 0x00;
uint8_t rxSyncRec = 0x00;
uint8_t rxSerRec = 0x00;
uint8_t rxBlkRec = 0x00;

uint8_t syncByte = 0xAA;


uint8_t i = 0;
int enable = 1;
char c = '\0';

volatile uint8_t rxByte;




uint8_t recieveByte(){
    while( ((UCSR0A & (1<<RXC0)) == 0) && (UDR0 == 0x00) );
  return UDR0;
}

void appendRx(uint8_t data_byte) //UDR0 will be the input of the function
{
    rxBuffer[rxWritePos] = data_byte;
    rxWritePos++; //increment global write position as you are inputting things into the array

    if (rxWritePos >= bufferSize){
        rxWritePos  = 0;
    }
}

void readRxCmdFunc(){
    rxCmdRec = 0x00;
    if (rxReadPos != rxWritePos){
        rxCmdRec = rxBuffer[rxReadPos];
        rxReadPos ++;
        if (rxReadPos >= bufferSize){
            rxReadPos = 0;
        }
    }
}



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
        // readRxCmdFunc();
            if (rxCmdRec == 0x44)
            { 
                PORTB |= (1 << PB5);
            }
            else if (rxCmdRec == 0xCB)
            {
                PORTB |= (1 << PB4);
            }
            else if (rxCmdRec == 0x62)
            {
                PORTB |=  (1 << PB3);
            }
            else if (rxCmdRec == 0xFA)
            {
                PORTB |= (1 << PB2);
            }
            else if (rxCmdRec == 0x05){ 
                PORTB &=~ (1 << PB5);
                PORTB &=~ (1 << PB4);
                PORTB &=~ (1 << PB3);
                PORTB &=~ (1 << PB2);
            }
        }
}  

ISR(USART_RX_vect)
{
    // uint8_t rxByte;
    rxCmdRec = UDR0;
    // uint8_t blankAddr, syncAddr, serAddr, cmdAddr ;
    // blankAddr = recieveByte();
    // cmdAddr = recieveByte();
    // appendRx(blankAddr);
    // appendRx(cmdAddr);
}

