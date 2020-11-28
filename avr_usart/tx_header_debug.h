#ifndef TX_HEADER_DEBUG_H
#define TX_HEADER_DEBUG_H

#define F_CPU 16000000UL

// Standard AVR libraries
#include <stdio.h>
#include <avr/io.h> 
#include <avr/fuse.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h> 

//Definitions for USART
#define BAUD 9600 //same rate as in the Makefile baud is in bits per second
#define ubbrn ((F_CPU/16/BAUD) - 1)
#define bufferSize 3


// Button Pin Definitions
#define but3 (~PINB) & (1 << PB4) // PCINT5
#define but2 (~PINB) & (1 << PB3) // PCINT4
#define but1 (~PINB) & (1 << PB2) // PCINT3
#define but0 (~PINB) & (1 << PB1) // PCINT2
#define butTog (~PINB) & (1 << PB0) // PCINT0

// For Transmitter, buffer, writing and reading
volatile char txBuffer[bufferSize] = {'0', '0', '0'};
volatile int  txWritePos = 0;
volatile int  txReadPos = 0;



// Initializations of serial number, reciever number, buffer and etc.
volatile int ser_bool = 0;
volatile char rxSerNum = '1';
volatile char syncByte = 's'; 
volatile char num1 = '1';
volatile char num2 = '2';
volatile char txByte = '0';
volatile int cmd_pcint = 1;



#endif
