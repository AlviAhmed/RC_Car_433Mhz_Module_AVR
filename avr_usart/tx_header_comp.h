#ifndef TX_HEADER_COMP_H
#define TX_HEADER_COMP_H

#define F_CPU 16000000UL

// Standard AVR libraries
#include <stdio.h>
#include <avr/io.h> 
#include <avr/fuse.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h> 

//Definitions for USART
#define BAUD 9600 //same rate as in the Makefile baud is in bits per second
#define ubbrn ((F_CPU/16/BAUD) - 1)
#define buffer_size 128

// For Receiver, buffer, writing and reading
#define bufferSize 3
volatile uint8_t rxBuffer[bufferSize] = {0x00, 0x00, 0x00};
volatile int  rxWritePos = 0;
volatile int  rxReadPos = 0;

// For Transmitter, buffer, writing and reading
volatile uint8_t txBuffer[bufferSize] = {0x00, 0x00, 0x00};
volatile int  txWritePos = 0;
volatile int  txReadPos = 0;

// Initializations of serial number, reciever number, buffer and etc.
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


#endif
