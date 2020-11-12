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

volatile  uint8_t ser = 0x00;
volatile uint8_t cmd = 0x00;
volatile uint8_t syn = 0xAA;

volatile int enable = 0;

volatile char keyboard_key = '0';

volatile int ser_bool = 0;
volatile uint8_t rxSerNum = 0x0C;
volatile uint8_t syncByte = 0xAA; 
volatile uint8_t num1 = 0x0C;
volatile uint8_t num2 = 0x09;
volatile uint8_t txByte = 0x00;
volatile int cmd_pcint = 1;

/* volatile uint8_t txBuffer[bufferSize] = {0x00, 0x00, 0x00}; */


volatile char txBuffer[bufferSize] = {'0', '0', '0'};
volatile int  txWritePos = 0;
volatile int  txReadPos = 0;


void txPacket(char rxByte, char command){
    txBuffer[0] = syncByte;
    txBuffer[1] = rxByte;
    txBuffer[2] = command;
}

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

/*
PB4 -> 0x44
PB3 -> 0xCB 
PB2 -> 0x62 
PB1 -> 0xFA 
else -> 0x05
*/


int main(void){
    DDRB |= (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1) | (1 << PB0);
    PORTB &= ~ (1 << PB4) | (1 << PB3) | (1 << PB2) | ( 1 << PB1 ) | (1 << PB0);

    
    DDRD |= (1 << PD7) | (1 << PD6);
    PORTD &=~ (1 << PD7)| ( 1 << PD6 );

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

    while (1){
        switch (keyboard_key){
        case ('f'):
            txPacket(rxSerNum, 0x44);
            PORTB |= (1 << PB1);
            PORTB &=~ (1 << PB0);
            break;
        case('b'):
            txPacket(rxSerNum, 0xCB);
            PORTB |= (1 << PB4);
            PORTB &=~ (1 << PB0);
            break;
        case('l'):
            txPacket(rxSerNum, 0x62);
            PORTB |=  (1 << PB3);
            PORTB &=~ (1 << PB0);
            break;
        case('r'):
            txPacket(rxSerNum, 0xFA);
            PORTB |=  (1 << PB2);
            PORTB &=~ (1 << PB0);
            break;
        case('n'):
            txPacket(rxSerNum, 0x05);
            PORTB |= (1 << PB0);
            PORTB &=~ (1 << PB1);
            PORTB &=~ (1 << PB4);
            PORTB &=~ (1 << PB3);
            PORTB &=~ (1 << PB2);
            break;
        }
    }

    return 0;
}  


ISR(USART_TX_vect) // once tx buffer is clear, set clear bit to accept new data
{
    if (txReadPos != bufferSize){ //now we are reading the info from the buffer index by index to be transmitted
        txBuffer[txReadPos] = UDR0; //tx read pos just used to index the buffer data to be transmitted
//        UDR0 = txBuffer[txReadPos]; //tx read pos just used to index the buffer data to be transmitted
        txReadPos ++;
        if (txReadPos >= bufferSize){
            txReadPos = 0;
        }
    }
}

ISR(USART_RX_vect)
{
    keyboard_key = UDR0;
    if (keyboard_key == '1'){
        rxSerNum = num2;
        PORTD |= (1 << PD7);
        PORTD &=~ (1 << PD6);
    }
    else if (keyboard_key == '2'){
        rxSerNum = num2;
        PORTD &=~ (1 << PD7);
        PORTD |= (1 << PD6);
        
    }
}
