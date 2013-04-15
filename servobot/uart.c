/*
 * CProgram1.c
 *
 * Created: 5.3.2013 20:46:40
 *  Author: Radim
 */ 

#include "uart.h"




void USART_Transmit(unsigned char data)
{
while (!( UCSR0A & (1<<UDRE0)));
UDR0 = data;
}

void USART_Init(unsigned int ubrr)
{
UBRR0H = (unsigned char)(ubrr>>8);
UBRR0L = (unsigned char)ubrr;
UCSR0B = _BV(RXEN0)|_BV(TXEN0)|_BV(RXCIE0);
UCSR0C = _BV(USBS0)|_BV(UCSZ00)|_BV(UCSZ01);
}

unsigned char USART_Receive( void )
{
while ( !(UCSR0A & (1<<RXC0)));
return UDR0;
}