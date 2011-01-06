#include "common.h"
#include "uart.h"
#include "cpu.h"
#include "temp.h"

static void brief_pause(register unsigned int n){
	__asm__ __volatile__ (
		"1: \n"
		" dec      %[n] \n"
		" jne      1b \n"
		: [n] "+r"(n));
}

void main (void) {
	uint16_t value;
	uint8_t temp;
	messageBuf[5] = '\r';
	messageBuf[6] = '\n';
	cpu_init();
	uart_init();
	temp_init();
	eint();
	while (1) {
		value = get_temp();
		utoa(value, messageBuf, 10);
		for (temp = 0; temp < 10; temp++) {
			uart_putc(messageBuf[temp]);
		}
		brief_pause(5000);
	}
}
