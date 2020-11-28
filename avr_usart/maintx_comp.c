// *******************************************************
//AVR_USART_ATMEGA328p_RX
// *******************************************************
//Specs of the USART:
// 1 start bit
// 8 data bits 
// no parity
// 1 stop bit

#include "tx_header_comp.h"

void txPacket(uint8_t rxByte, uint8_t command){
    txBuffer[0] = syncByte;
    txBuffer[1] = rxByte;
    txBuffer[2] = command;
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
            break;
        case('b'):
            txPacket(rxSerNum, 0xCB);
            PORTB |= (1 << PB4);
            break;
        case('l'):
            txPacket(rxSerNum, 0x62);
            PORTB |=  (1 << PB3);
            break;
        case('r'):
            txPacket(rxSerNum, 0xFA);
            PORTB |=  (1 << PB2);
            break;
        case('n'):
            txPacket(rxSerNum, 0x05);
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
    if (txReadPos != bufferSize){ //now we are reading the info from the 
       UDR0 = txBuffer[txReadPos]; //tx read pos just used to index thebuffer data to be transmitted
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
        rxSerNum = num1;
        PORTD |= (1 << PD7);
        PORTD &=~ (1 << PD6);
    }
    else if (keyboard_key == '2'){
        rxSerNum = num2;
        PORTD &=~ (1 << PD7);
        PORTD |= (1 << PD6);
        
    }
}
