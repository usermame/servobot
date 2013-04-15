/*
 * servobot.c
 *
 * Created: 3.3.2013 22:55:59
 *  Author: Radim
 */ 

#include <stdlib.h>
#include <stdio.h> 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"

#define _PRUMER 1

volatile uint16_t counter = 1;
volatile uint16_t wheelspeed[2][_PRUMER];
volatile uint16_t wheelspeedaverage[2];
volatile uint16_t wheelcounter[2];
volatile uint8_t wheelinc[2];
volatile uint8_t increg=1;

#define _PERIODA 4999

#define _PI 3.14

int krok=1;

void regulace()
{/*
if(OCR1A == (_PERIODA / 20) * 1.5)
	{
		OCR1B = (_PERIODA / 20) * 1.5;
		return;
	}		
wheelspeedaverage[0] = 0;
wheelspeedaverage[1] = 0;
for(int i=0; i<_PRUMER; i++)
{
	wheelspeedaverage[0] += wheelspeed[0][i] / _PRUMER;
	wheelspeedaverage[1] += wheelspeed[1][i] / _PRUMER;
}
	increg=(OCR1A < (_PERIODA / 20) * 1.5)?-1:1;
		
		if((OCR1B > 0)  && (OCR1B < 500))
			if(wheelspeedaverage[1]	< wheelspeedaverage[0])
				OCR1B +=increg;
			else
				OCR1B -=increg;
*/
}	



void pinout(uint8_t *portnum, int num, int val)
{
if(val)
	*portnum |= _BV(num);
else
	*portnum &= ~_BV(num);		
}

ISR(USART_RX_vect) {
	
}

ISR(TIMER0_OVF_vect) {
	wheelcounter[0]++;
	wheelcounter[1]++;
/*	counter += krok;
	if (counter>64000 || counter<1)
		krok = krok * - 1;
	OCR1A= ((_PERIODA / 20) * 1) + counter/256;*/
}	 

ISR(INT0_vect)
{
wheelinc[0]++;
wheelinc[0] = wheelinc[0] % _PRUMER;
wheelspeed[0][wheelinc[0]] = wheelcounter[0];
wheelcounter[0] = 0;
regulace();
}	

ISR(INT1_vect) 
{
wheelinc[1]++;
wheelinc[1] = wheelinc[1] % _PRUMER;
wheelspeed[1][wheelinc[1]] = wheelcounter[1];
wheelcounter[1] = 0;
regulace();
}	

void init_timer()
{
	TCCR0B =  _BV(CS01);
	TIMSK0 =  _BV(TOIE0);
}

void init_pwm()
{
	TCCR1A = 0;
	ICR1 = _PERIODA;
	TCCR1A = _BV(WGM11);  
	TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11) | _BV(CS10);
	DDRB |= _BV(1) | _BV(2);
	TCCR1A |= _BV(COM1A1);
	TCCR1A |= _BV(COM1B1);
}	

void init_ext()
{
    //DDRD &= ~(1 << DDD2);
    //PORTD &= ~(1 << PORTD2);
    EICRA |= _BV(ISC00)| _BV(ISC10) | _BV(ISC11) | _BV(ISC01);
    EIMSK |= _BV(INT0)| _BV(INT1);
	DDRD |= (1 << DDD4);
    //PORTD &= ~(1 << PORTD2);
	wheelcounter[0] = wheelcounter[1] = 0;
}	


int main(){
	init_timer();
	init_pwm();
	init_ext();
	USART_Init(103);
	
	sei();
	
	OCR1A = (_PERIODA / 20) *	 1.5;
	OCR1B = (_PERIODA / 20) *    1.5;

	DDRD |= (1 << DDD4);
	
	int i=0;
	char texttouart[50];
	while(1)
	{
		sprintf(texttouart, "%d\t%d\t%d\t%d\r\n", wheelspeedaverage[0], wheelspeedaverage[1], wheelcounter[0], OCR1B);	
		i=0;
		while(texttouart[i] != '\0')
		{
			USART_Transmit(texttouart[i]);
			i++;
		}	
		
		switch(USART_Receive())
		{
			case 'f':
				OCR1A = (_PERIODA / 20) *	 1.8;
				OCR1B = (_PERIODA / 20) *    1.2;
			break;
			case 's':
				OCR1A = (_PERIODA / 20) *	 1.5;
				OCR1B = (_PERIODA / 20) *    1.5;
			break;
				case 'l':
				OCR1A = (_PERIODA / 20) *	 1.6;
				OCR1B = (_PERIODA / 20) *    1.6;
			break;
				case 'r':
				OCR1A = (_PERIODA / 20) *	 1.4;
				OCR1B = (_PERIODA / 20) *    1.4;
			break;
				case 'b':
				OCR1A = (_PERIODA / 20) *	 1.2;
				OCR1B = (_PERIODA / 20) *    1.8;
			break;
			default:
			break;
		}		
	
		_delay_ms(100);
	}	
	


	return 0;

}