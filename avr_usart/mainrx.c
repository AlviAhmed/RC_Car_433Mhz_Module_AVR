// *******************************************************
//AVR_USART_ATMEGA328p_RX
// *******************************************************
//Specs of the USART:
// 1 start bit
// 8 data bits 
// no parity
// 1 stop bit

#include "rx_header.h"

/*
  PB4 -> 0x44
  PB3 -> 0xCB 
  PB2 -> 0x62 
  PB1 -> 0xFA 
  else -> 0x05
*/

init(){
    DDRB |= (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | (1 << PB1);
    PORTB &= ~ (1 << PB5) | (1 << PB4) | (1 << PB3) | (1 << PB2) | ( 1 << PB1 );
    //High and low bits
    UBRR0H = (ubbrn >> 8); 
    UBRR0L = ubbrn; 
    UCSR0A = 0x00;
    //transimit and recieve enable
    UCSR0B =  (1 << RXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  //8 bit data format
    sei();
    UDR0 = 0;
}

int main(void){
    init();
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
    if (rxBuffer[0] == 0xAA){
        switch (rxWritePos){
        case(0):
            syn = rxBuffer[rxWritePos];
            break;
        case(1):
            ser = rxBuffer[rxWritePos];
            if (ser == num2){
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

