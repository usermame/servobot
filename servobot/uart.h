/*
 * uart.h
 *
 * Created: 19.3.2013 9:14:11
 *  Author: Radim
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>



void USART_Transmit(unsigned char data);
void USART_Init(unsigned int ubrr);
unsigned char USART_Receive( void );


#endif /* UART_H_ */