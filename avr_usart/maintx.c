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

#define rxSerialNum2 0x0C
#define bufferSize 128

/* NOTE: since I am sending 2 bytes of info, 1 per index, that means that per transmission  
2 positions in the array will be taken and transmitted. 

For now this works since 128 is a multiple of 2, therefore when the array (readpos / write pos) eventually 
overlaps / ends there will be no overlaping of data. 

HOWEVER, if the amount of commands being send per packet should change (i.e. 3 or 5 commands). Then the buffer size HAS to be 
a multiple to avoid overriding values in the array when resetting the index(readpos / write pos). 

 */
uint8_t syncByte = 0xAA;
uint8_t blankByte = 0x05;
uint8_t  rxSerialNum1 = 0xCF;

uint8_t txBuffer[bufferSize];
uint8_t txWritePos = 0;
uint8_t txReadPos = 0;





volatile int pressed = 0;
volatile uint8_t txByte = 0x00;
uint8_t i = 0;
int enable = 1;


void nullByteIfEmpty(){
    if (UCSR0A & (1 << UDRE0)) { //if the UDRE register is empty, then send a null byte, just to make sure no junk is put in to the register
        UDR0 = 0x00;
     }
}
void appendTx(uint8_t data_byte)
{
    txBuffer[txWritePos] = data_byte;
    txWritePos++; //increment global write position as you are inputting things into the array

    if (txWritePos >= bufferSize){
        txWritePos  = 0;
    }
}

void txPacket(uint8_t command){
    appendTx(blankByte);
    appendTx(command);
    nullByteIfEmpty(); //set null byte if UDRE is empty after sending all the commands
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
        txPacket(0x44);
        // txByte = syncByte;
        // txByte = 0x04;
        // ar = '3';
        PORTB |= (1 << PB1);
    }
        
    else if (but2){    
        txPacket(0xCB);
        // txByte = syncByte; 
        // txByte = 0x03;
        // ar = '2';
        PORTB |= (1 << PB1);
    }
        
    else if (but1){
        txPacket(0x62);
        // txByte = syncByte; 
        // txByte = 0x02;
       // ar = '1';
        PORTB |= (1 << PB1);
    }
        
    else if (but0){    
        txPacket(0xFA);
        // txByte = syncByte; 
        // txByte = 0x01;
        // ar = '0';
        PORTB |= (1 << PB1);
    }
        
    else {   
        txPacket(0x05);
        // txByte = syncByte; 
        // txByte = 0x05;
        // ar = 'n';
        PORTB &=~ (1 << PB1);
    }       

}

ISR(USART_TX_vect)
{
    // UDR0 = 0x00;
    if (txReadPos != txWritePos){ //now we are reading the info from the buffer index by index to be transmitted
        UDR0 = txBuffer[txReadPos]; //tx read pos just used to index the buffer data to be transmitted
        txReadPos ++;
        if (txReadPos >= bufferSize){
            txReadPos = 0;
        }
    }
}

