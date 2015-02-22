#ifndef UART_H
#define UART_H

/************************************************************************
Title:    Interrupt UART library with receive/transmit circular buffers
Author:   Andy Gock
Software: AVR-GCC 4.1, AVR Libc 1.4
Hardware: any AVR with built-in UART, tested on AT90S8515 & ATmega8 at 4 Mhz
License:  GNU General Public License
Usage:    see Doxygen manual

Based on original library by Peter Fluery, Tim Sharpe, Nicholas Zambetti.

https://github.com/andygock/avr-uart

************************************************************************/

#include <string.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>

#include "port.h"

#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
	#error "This library requires AVR-GCC 3.4 or later, update to newer AVR-GCC compiler !"
#endif

#define UART_BAUD_SELECT(baudRate,xtalCpu) (((xtalCpu)+8UL*(baudRate))/(16UL*(baudRate))-1UL)
#define UART_BAUD_SELECT_DOUBLE_SPEED(baudRate,xtalCpu) (((((xtalCpu)+4UL*(baudRate))/(8UL*(baudRate))-1UL))|0x8000)

extern void button_int0_handler(void);
extern void uart_init(uint16_t baudrate);
extern uint16_t uart_get_char(void);
extern uint16_t uart_peek(void);
extern void uart_put_char(uint8_t c);
extern void uart_put_string(const char *s);
extern void uart_put_string_p(const char *s);
extern uint16_t uart_rx_data_available(void);
extern void uart_flush_rx_buffer(void);
extern void uart_reset_rx_buffer(void);
extern void uart_reset_tx_buffer(void);
extern void uart_move_rx_tail(void);
extern uint8_t uart_is_data_in_rx_buffer(const char *data);
extern uint8_t uart_is_rx_buffer_ready(void);

#endif // UART_H

