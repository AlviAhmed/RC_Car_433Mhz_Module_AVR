#ifndef RX_HEADER_H
#define RX_HEADER_H
    

#define F_CPU 16000000UL

// Standard AVR libraries
#include <stdio.h>
#include <avr/io.h> 
#include <avr/fuse.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h> 

// Definitions for USART
#define BAUD 9600 //same rate as in the Makefile baud is in bits per second
#define ubbrn ((F_CPU/16/BAUD) - 1)
#define buffer_size 128

// For Receiver, buffer, writing and reading
#define bufferSize 3
volatile uint8_t rxBuffer[bufferSize] = {0x00, 0x00, 0x00};
volatile int  rxWritePos = 0;
volatile int  rxReadPos = 0;

// Initializations of serial number, reciever number, buffer and etc.
volatile uint8_t rxSerNum = 0x09;
volatile uint8_t num1 = 0x0C;
volatile uint8_t num2 = 0x09;
volatile  uint8_t ser = 0x00;
volatile uint8_t cmd = 0x00;
volatile uint8_t syn = 0x00;
volatile int enable = 0;


#endif
