/*
 * port.c
 *
 * Created: 2015.01.25. 20:37:21
 *  Author: iszirtes
 */

#include "port.h"

void port_config(void)
{
	/* set PINs to 0 as output (O is input, 1 is output) */
	DDRB |= ((1 << ERROR_LED_PIN) | (1 << OK_LED_PIN));
	/* set PINs to 0 as low, default OFF (0 as low, 1 as high) */
	PORTB &= ~((1 << ERROR_LED_PIN) | (1 << OK_LED_PIN));
	/* set PINs to 0 as output (O is input, 1 is output) */
	DDRC |= ((1 << OUT_1_PIN) | (1 << OUT_2_PIN));
	/* set PINs to 0 as low, default OFF (0 as low, 1 as high) */
	PORTC &= ~((1 << OUT_1_PIN) | (1 << OUT_2_PIN));
	/* set PINs to 0 as input (O is input, 1 is output) */
	DDRD  &= ~(1 << BUTTON_1_PIN);
	/* set PINs to high to turn On the Pull-up (0 as low, 1 as high) */
	PORTD |= (1 << BUTTON_1_PIN);
	/* MCUCR MCU Control Register Bit 1, 0 – ISC01, ISC00: Interrupt Sense Control 0 Bit 1 and Bit 0 (low, change, h->l, l->h) */
	MCUCR &= ~(1 << ISC00);	// ISC00 - 0: Trigger on falling edge of INT0
	MCUCR |= (1 << ISC01);	// ISC01 - 1: Trigger on falling edge of INT0
	/* GICR General Interrupt Control Register */
	GICR |= (1 << INT0);
}

void toggle_outputs(void)
{
	OUT_1_TOGGLE();
	OUT_2_TOGGLE();
}

void leds_blinking(uint8_t n)
{
	for(uint8_t i=0; i < n; i++)
	{
		OK_LED_TOGGLE();
		ERROR_LED_TOGGLE();
		_delay_ms(250);
		OK_LED_TOGGLE();
		ERROR_LED_TOGGLE();
		_delay_ms(250);
	}
}

void ok_led_blinking(uint8_t n)
{
	for (uint8_t i=0; i<n; i++)
	{
		OK_LED_TOGGLE();
		_delay_ms(250);
		OK_LED_TOGGLE();
		_delay_ms(250);
	}
}

void error_led_blinking(uint8_t n)
{
	for (uint8_t i=0; i<n; i++)
	{
		ERROR_LED_TOGGLE();
		_delay_ms(250);
		ERROR_LED_TOGGLE();
		_delay_ms(250);
	}
}
