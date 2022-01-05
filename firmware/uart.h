#ifndef UART_H
#define UART_H

void uart_init();
void uart_transmit(unsigned char data);
void uart_transmit_string(char* str);
unsigned char uart_recieve();

void uart_enable();
void uart_disable();

#endif
