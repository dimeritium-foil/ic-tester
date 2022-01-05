#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"

#define BAUD 9600
#define MYUBRR (uint16_t) ((F_CPU / (16.0 * BAUD)) - 1)

void uart_init() {

    // set 8 bit frame
    UCSRC = (1 << UCSZ1 | 1 << UCSZ0);

    UBRRH = (uint8_t) (MYUBRR >> 8);
    UBRRL = (uint8_t) MYUBRR;

    // set double speed to 0
    UCSRA &= ~(1 << U2X);

    // enable transmission and reception
    UCSRB = (1 << TXEN | 1 << RXEN); 
}

void uart_transmit(unsigned char data) {

    // wait for empty transmit buffer
    while ( !( UCSRA & (1 << UDRE)) );

    // send data
    UDR = data;
}

void uart_transmit_string(char* str) {

    int i = 0;

    while (*(str + i) != '\0') {
        uart_transmit(*(str + i));
        i++;
    }
}

unsigned char uart_recieve() {

    // wait for data to be received
    while ( !(UCSRA & (1 << RXC)) );

    // Get and return received data from buffer
    return UDR;
}

void uart_enable() {

    // enable transmission and reception
    UCSRB |= (1 << TXEN | 1 << RXEN); 
}

void uart_disable() {

    // disable transmission and reception
    UCSRB &= ~(1 << TXEN | 1 << RXEN); 
}
