/*
 * SiemensC55_ATmega8A_CallServer.c
 *
 * Created: 2015.02.22. 14:56:42
 *  Author: iszirtes
 */

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "uart.h"
#include "gsm.h"

#define UART_BAUD_RATE 9600

int main(void)
{
	port_config();
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	uart_reset_tx_buffer();
	uart_reset_rx_buffer();
	sei();
	leds_blinking(3);
	gsm_init();

	while(1)
	{
		button_int0_handler();
		gsm_event_handler();
	}
}