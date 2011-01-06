#include "temp.h"

#define CS BIT3
#define SO BIT4
#define SCK BIT5
#define PWR BIT6
#define GND BIT7

void temp_init(void)
{
	P1DIR |= CS | SCK | PWR | GND;
	P1OUT = PWR;
}

unsigned int get_temp(void)
{
    int i;
    uint16_t value;

    P1OUT ^= CS;
	//CS low to enable device

    P1OUT |= SCK;
    P1OUT ^= SCK;
	//strobe SCK to trash dummy bit

    for (i=11; i>=0; i--)
    {
        P1OUT |= SCK;
		//SCK goes high
        value += ((P1IN & SO) > 0) << i;
		//read value of SO into bit i stored in value
        P1OUT ^= SCK;
		//SCK goes low
    }
//strobe SCK to read bits 14 through 3

    P1OUT |= SCK;
    P1OUT ^= SCK;
//strobe SCK to grab thermocouple input bit

    P1OUT |= CS;
	//CS high to disable device

    value >>= 2;
	//use a bitshift to divide value by 4, truncating the LSBs
    return value;
}
