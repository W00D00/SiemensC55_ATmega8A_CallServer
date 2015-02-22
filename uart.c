/*************************************************************************
	Title:    Interrupt UART library with receive/transmit circular buffers
	Author:   István Szirtes
	Software: AVR-GCC 4.1, AVR Libc 1.4.6 or higher
	Hardware: any AVR with built-in UART,
	License:  GNU General Public License

	Updated UART library (this one) by Andy Gock
	https://github.com/andygock/avr-uart

	Based on updated UART library (this one) by Tim Sharpe
	http://beaststwo.org/avr-uart/index.shtml

	Based on original library by Peter Fluery
	http://homepage.hispeed.ch/peterfleury/avr-software.html

*************************************************************************/

#include "uart.h"

volatile uint8_t BUTTON_INT0 = 0;

/* Set transmit buffer */
#ifndef UART_RX_BUFFER_SIZE
	#define UART_RX_BUFFER_SIZE 128 /**< Size of the circular receive buffer, must be power of 2 */
#endif

#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1)

#if ( UART_RX_BUFFER_SIZE & UART_RX_BUFFER_MASK )
	#error RX buffer size is not a power of 2
#endif

/* Check buffer sizes are not too large for 8-bit positioning */
#if (UART_RX_BUFFER_SIZE > 256 & !defined(USART_LARGE_BUFFER))
	#error "Buffer too large, please use -DUSART_LARGE_BUFFER switch in compiler options"
#endif

static volatile uint8_t UART_RxBuf[UART_RX_BUFFER_SIZE];

/* Set transmit buffer */
#ifndef UART_TX_BUFFER_SIZE
	#define UART_TX_BUFFER_SIZE 128 /**< Size of the circular transmit buffer, must be power of 2 */
#endif

#define UART_TX_BUFFER_MASK ( UART_TX_BUFFER_SIZE - 1)

#if ( UART_TX_BUFFER_SIZE & UART_TX_BUFFER_MASK )
	#error TX buffer size is not a power of 2
#endif

/* Check buffer sizes are not too large for *_LARGE_BUFFER operation (16-bit positioning) */
#if (UART_RX_BUFFER_SIZE > 65536)
	#error "Buffer too large, maximum allowed is 65536 bytes"
#endif

/* test if the size of the circular buffers fits into SRAM */
#if ( (UART_RX_BUFFER_SIZE+UART_TX_BUFFER_SIZE) >= (RAMEND-0x60 ) )
	#error "size of UART_RX_BUFFER_SIZE + UART_TX_BUFFER_SIZE larger than size of SRAM"
#endif

static volatile uint8_t UART_TxBuf[UART_TX_BUFFER_SIZE];

/* set size of the buffer vars */
#if defined( USART_LARGE_BUFFER )
	static volatile uint16_t UART_TxHead;
	static volatile uint16_t UART_TxTail;
	static volatile uint16_t UART_RxHead;
	static volatile uint16_t UART_RxTail;
	static volatile uint8_t	 UART_RxReady;
	static volatile uint8_t	 UART_LastRxError;
#else
	static volatile uint8_t UART_TxHead;
	static volatile uint8_t UART_TxTail;
	static volatile uint8_t UART_RxHead;
	static volatile uint8_t UART_RxTail;
	static volatile uint8_t	UART_RxReady;
	static volatile uint8_t UART_LastRxError;
#endif

/* UART error states */
#define UART_FRAME_ERROR      0x1000	/* Framing Error by UART       */
#define UART_OVERRUN_ERROR    0x0800	/* Overrun condition by UART   */
#define UART_PARITY_ERROR     0x0400	/* Parity Error by UART        */
#define UART_BUFFER_OVERFLOW  0x0200	/* receive ringbuffer overflow */
#define UART_NO_DATA          0x0100	/* no receive data available   */

/* termination chars */
#define NULL_TERMINATOR		'\0'
#define CR_TERMINATOR		'\r'
#define LF_TERMINATOR		'\n'
#define CR_LF_TERMINATOR	"\r\n"


ISR(USART_RXC_vect)
{
    uint16_t tmp_head;
    uint8_t data;
	static volatile uint8_t data_in_buffer = 0;
    uint8_t usr;
    uint8_t last_rx_error;

    /* read UART status register and UART data register */
    usr  = UCSRA;
    data = UDR;

    /* receive buffer had a error */
    last_rx_error = (usr & ((1<<FE)|(1<<DOR)));

    /* calculate buffer index */
    tmp_head = ( UART_RxHead + 1) & UART_RX_BUFFER_MASK;

    if ( tmp_head == UART_RxTail )
	{
        /* error: receive buffer overflow */
        last_rx_error = (UART_BUFFER_OVERFLOW>>8);
    }
	else
	{
		/* "\r\nRING\r\n\r\n+CLIP: "+362043269592,145,,,,0\r\n" */
		/* store received data in buffer */
		if ((data != CR_TERMINATOR) && (data != LF_TERMINATOR))
		{
			/* store received data in buffer */
			UART_RxBuf[UART_RxHead] = data;
			/* store new index */
			UART_RxHead = tmp_head;
			/* set that data is in the buffer */
			data_in_buffer = 1;
		}
		/* at termination char close the buffer data */
		else if (data_in_buffer && ((data == CR_TERMINATOR) || (data == LF_TERMINATOR)))
		{
			/* close the data with '\0' */
			UART_RxBuf[UART_RxHead] = NULL_TERMINATOR;
			/* store new index */
			UART_RxHead = tmp_head;
			/* set the flag to proceed the buffer */
			UART_RxReady++;
			/* reset the termination counter */
			data_in_buffer = 0;
		}
    }
	/* store the actual error in the global var*/
    UART_LastRxError = last_rx_error;
}

ISR(INT0_vect)
{
	/* button is pushed */
	BUTTON_INT0 = 1;
}

ISR(USART_UDRE_vect)
{
    uint16_t tmpTail;

    if ( UART_TxHead != UART_TxTail)
	{
        /* calculate and store new buffer index */
        tmpTail = (UART_TxTail + 1) & UART_TX_BUFFER_MASK;
        UART_TxTail = tmpTail;
        /* get one byte from buffer and write it to UART */
        UDR = UART_TxBuf[tmpTail];  /* start transmission */
    }
	else
	{
        /* tx buffer empty, disable UDRE interrupt */
        UCSRB &= ~(1<<UDRIE);
    }
}

void button_int0_handler(void)
{
	/* manual switch button is pushed */
	if (BUTTON_INT0)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			/* reset the flag */
			BUTTON_INT0 = 0;
		}
		/* switch ON or OFF the outputs */
		toggle_outputs();
	}
}

void uart_init(uint16_t baudrate)
{
	/* Set baud rate */
	if ( baudrate & 0x8000 )
	{
		UCSRA = (1<<U2X);  //Enable 2x speed
		baudrate &= ~0x8000;
	}
	/* Load the higher 8 bit of the valueUBRR into the UBRRH register */
	UBRRH = (uint8_t)(baudrate>>8);
	/* Load the lower 8 bit of the valueUBRR into the UBRRL register */
	UBRRL = (uint8_t) baudrate;
	/* Enable USART receiver and transmitter and receive complete interrupt */
	UCSRB |= ((1 << RXEN)|(1 << TXEN)|(1 << RXCIE));
	/* Set frame format: asynchronous, 8data, no parity, 1stop bit */
	UCSRC |= ((1 << URSEL)|(1 << UCSZ0)|(1 << UCSZ1));
}

uint16_t uart_get_char(void)
{
	uint16_t tmpTail;
	uint8_t data;

	/* no data available */
	if ( UART_RxHead == UART_RxTail )
	{
		return UART_NO_DATA;
	}

	/* calculate/store buffer index */
	tmpTail = (UART_RxTail + 1) & UART_RX_BUFFER_MASK;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		UART_RxTail = tmpTail;
		/* get data from receive buffer */
		data = UART_RxBuf[UART_RxTail];
	}

	/* TODO: last error should be in the atomic block */
	return (UART_LastRxError << 8) + data;
}

uint16_t uart_peek(void)
{
	uint16_t tmp_tail;
	uint8_t data;

	/* no data available */
	if ( UART_RxHead == UART_RxTail )
	{
		return UART_NO_DATA;
	}

	/* calculate/store buffer index */
	tmp_tail = (UART_RxTail + 1) & UART_RX_BUFFER_MASK;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		/* get data from receive buffer */
		data = UART_RxBuf[tmp_tail];
	}

	/* TODO: last error should be in the atomic block */
	return (UART_LastRxError << 8) + data;
}

void uart_put_char(uint8_t c)
{
	uint16_t tmpHead;

	tmpHead  = (UART_TxHead + 1) & UART_TX_BUFFER_MASK;

	while ( tmpHead == UART_TxTail )
	{
		;/* wait for free space in buffer */
	}

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		UART_TxBuf[tmpHead] = c;
		UART_TxHead = tmpHead;
	}

	/* enable UDRE interrupt */
	UCSRB |= (1<<UDRIE);
}

void uart_put_string(const char *s)
{
	while (*s)
	{
		uart_put_char(*s++);
	}
}

void uart_put_string_p(const char *s)
{
	register char c;

	while ( (c = pgm_read_byte(s++)) )
	{
		uart_put_char(c);
	}
}

uint16_t uart_rx_data_available(void)
{
	return (UART_RX_BUFFER_SIZE + UART_RxHead - UART_RxTail) & UART_RX_BUFFER_MASK;
}

void uart_flush_rx_buffer(void)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		UART_RxHead = UART_RxTail;
	}
}

void uart_reset_rx_buffer(void)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		UART_RxHead = 0;
		UART_RxTail = 0;
		UART_RxReady = 0;
		memset(&UART_RxBuf, NULL_TERMINATOR, sizeof(UART_RxBuf));
	}
}

void uart_reset_tx_buffer(void)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		UART_TxHead = 0;
		UART_TxTail = 0;
		memset(&UART_TxBuf, NULL_TERMINATOR, sizeof(UART_TxBuf));
	}
}

void uart_move_rx_tail(void)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		const char *ptr = strchr((const char *)UART_RxBuf+UART_RxTail, NULL_TERMINATOR);
		if (ptr)
		{
        	UART_RxTail = ptr-(const char *)UART_RxBuf+1;
			UART_RxReady--;
    	}
	}
}

uint8_t uart_is_data_in_rx_buffer(const char *data)
{
	uint8_t res;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		/* if (uart_data_available() && strstr((const char *)UART_RxBuf+UART_RxTail, data)) */
		if (UART_RxReady && strstr((const char *)UART_RxBuf+UART_RxTail, data))
		{
			res = 1;
		}
		else
		{
			res = 0;
		}
	}
	return res;
}

uint8_t uart_is_rx_buffer_ready(void)
{
	uint8_t s;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		s = UART_RxReady;
	}
	return s;
}
