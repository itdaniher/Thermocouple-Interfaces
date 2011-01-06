#include "globals.h"
#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h> 

unsigned char CS = 1;	// CS pin on MAX6675
unsigned char SO = 3;	// SO pin of MAX6675
unsigned char SCK = 4;	// SCK pin of MAX6675


unsigned int get_temp ( void )
{
	int i;
	uint16_t value;
	bool pinstate;

//CS low to enable device
	PORTB ^= (1 << CS);

//strobe SCK to trash dummy bit
	PORTB |= (1 << SCK);
	PORTB ^= (1 << SCK);

//strobe SCK to read bits 14 through 3
	for (i=11; i>=0; i--)
	{
//SCK goes high
		PORTB |= (1 << SCK);
//read value of SO into bit i stored in value
		value += ((PINB & 1<<SO) > 0) << i;
//SCK goes low
		PORTB ^= (1 << SCK);
	}

//strobe SCK to grab thermocouple input bit
	PORTB |= (1 << SCK);
	PORTB ^= (1 << SCK);

//CS high to disable device
	PORTB |= (1 << CS);

//use a bitshift to divide value by 4, truncating the LSBs
	value >>= 2;
	return value;
}
int main ( void )
{
	uartInit();
	messageBuf[5] = '\r';
	messageBuf[6] = '\n';
	//setup pinmodes - CS and SCK output, SO input
	DDRB |= ( 1 << CS ) | ( 1 << SCK );
	DDRB ^= ( 1 << SO );
	for (;;) {
		uint16_t value;
		value = get_temp();
		utoa(value, messageBuf, 10);
//iterate through the first ten items of messageBuf, sending values over the software UART
		for (temp = 0; temp < 10; temp++) {
			sendByte(messageBuf[temp]);
		}
	_delay_ms(500);
	}
	return 0;
}
