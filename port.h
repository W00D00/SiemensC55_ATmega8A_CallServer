/*
 * port.h
 *
 * Created: 2015.01.25. 16:55:57
 *  Author: iszirtes
 */

#ifndef PORT_H_
#define PORT_H_

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* manual switch button */
#define BUTTON_1_PIN			PD2
// feedback about the operation (keep alive LED) */
#define OK_LED_PIN				PB7
#define OK_LED_PORT				PORTB
#define OK_LED_STATE()			(OK_LED_PORT & (1 << OK_LED_PIN))
#define OK_LED_ON()				OK_LED_PORT |= (1 << OK_LED_PIN)
#define OK_LED_OFF()			OK_LED_PORT &= ~(1 << OK_LED_PIN)
#define OK_LED_TOGGLE()			OK_LED_PORT ^= (1 << OK_LED_PIN)
/* feedback about the errors */
#define ERROR_LED_PIN			PB6
#define ERROR_LED_PORT			PORTB
#define ERROR_LED_STATE()		(ERROR_LED_PORT & (1 << ERROR_LED_PIN))
#define ERROR_LED_ON()			ERROR_LED_PORT |= (1 << ERROR_LED_PIN)
#define ERROR_LED_OFF()			ERROR_LED_PORT &= ~(1 << ERROR_LED_PIN)
#define ERROR_LED_TOGGLE()		ERROR_LED_PORT ^= (1 << ERROR_LED_PIN)
/* output for call */
#define OUT_1_PIN				PC5
#define OUT_1_PORT				PORTC
#define OUT_1_STATE()			(OUT_1_PORT & (1 << OUT_1_PIN))
#define OUT_1_ON()				OUT_1_PORT |= (1 << OUT_1_PIN)
#define OUT_1_OFF()				OUT_1_PORT &= ~(1 << OUT_1_PIN)
#define OUT_1_TOGGLE()			OUT_1_PORT ^= (1 << OUT_1_PIN)
/* output for sms */
#define OUT_2_PIN				PC4
#define OUT_2_PORT				PORTC
#define OUT_2_STATE()			(OUT_2_PORT & (1 << OUT_2_PIN))
#define OUT_2_ON()				OUT_2_PORT |= (1 << OUT_2_PIN)
#define OUT_2_OFF()				OUT_2_PORT &= ~(1 << OUT_2_PIN)
#define OUT_2_TOGGLE()			OUT_2_PORT ^= (1 << OUT_2_PIN)

/* Prototypes */
extern void port_config(void);
extern void toggle_outputs(void);
extern void leds_blinking(uint8_t n);
extern void ok_led_blinking(uint8_t n);
extern void error_led_blinking(uint8_t n);

#endif /* PORT_H_ */