/*
 * gsm.c
 *
 * Created: 2015.01.25. 18:18:11
 *  Author: iszirtes
 */

#include "gsm.h"

/* authorized mobile numbers TODO: get from mobile memory 1st and 2nd */
#define AUTHORIZED_MOBILE_NUMBER_1	"204336999"
#define AUTHORIZED_MOBILE_NUMBER_2	"302077726"
/* AT commands */
/* for initialize the mobile */
#define AT_COMMAND_ATTEST			"AT"						// AT Test
const char *AT_RESPONSE_ATTEST[] = {"AT","OK"};					// AT command is received
#define AT_COMMAND_ECHO_OFF			"ATE0"						// Echo Disabled
const char *AT_RESPONSE_ECHO_OFF[] = {"ATE0","OK"};				// AT command is received
#define AT_COMMAND_ECHO_ON			"ATE1"						// Echo Enabled
const char *AT_RESPONSE_ECHO_ON[] = {"ATE1","OK"};				// AT command is received
#define AT_COMMAND_CLIP_ON			"AT+CLIP=1"					// Calling line identification presentation
const char *AT_RESPONSE_CLIP_ON[] = {"OK"};						// AT command is received
/* commands */
#define AT_COMMAND_TEST_CALL		"ATD+36204336999;"			// Dial a voice call
#define AT_COMMAND_HANGUP			"AT+CHUP"					// Hangup call
const char *AT_RESPONSE_HANGUP[] = {"OK"};						// AT command is received
#define AT_COMMAND_GET_CONTACT		"AT+CPBR=%d"				// Get the contact info by its index
#define AT_COMMAND_STATUS			"AT+CPAS"					// Query the telephone status
const char *AT_RESPONSE_STATUS[] = {"+CPAS"};					// 0 Ready, 3 Incoming call (phone is ringing), 4 Call is active
const char *AT_RESPONSE_STATUS_READY[] = {"+CPAS: 0"};			// 0 Ready
const char *AT_RESPONSE_STATUS_CALL[] = {"+CPAS: 3"};			// 3 Incoming call (phone is ringing)
const char *AT_RESPONSE_STATUS_ACTIVE[] = {"+CPAS: 4"};			// 4 Call is active
/* AT responses */
const char *AT_RESPONSE_OK[] = {"OK"};							// AT command is received
const char *AT_RESPONSE_ERROR[] = {"ERROR"};					// something is wrong with AT command
const char *AT_RESPONSE_CLIP[] = {"+CLIP"};						// ring with caller ID if CLIP is enabled
const char *AT_RESPONSE_RING[] = {"RING"};						// simple ring
/* number of ringings */
#define NUMBER_OF_RINGS_ON			4							// Switch ON
#define NUMBER_OF_RINGS_OFF			2							// Switch OFF

/* string terminators */
#define NULL_TERMINATOR		'\0'
#define CR_TERMINATOR		'\r'
#define LF_TERMINATOR		'\n'
#define CR_LF_TERMINATOR	"\r\n"

void gsm_put_at_command(const char *at_command, const char *terminate, const char *at_response[], const uint8_t response_size, uint16_t time_out_ms)
{
	/* reset the buffer */
	if (at_response)
	{
		uart_reset_rx_buffer();
	}
	/* send an at command */
	uart_put_string(at_command);
	/* if it is needed terminate the command */
	if (terminate)
	{
		uart_put_string(terminate);
	}
	/* If response to command is expected */
	if (at_response)
	{
		/*
		time_out_ms: 0 means default 1s: 1000ms / 100ms = 10x
		time_out_ms: 2000: 2000ms / 100ms = 20x
		*/
		uint8_t time_out_circles = 10;				// x10
		uint8_t time_out_one_circle_time = 100;		// 100ms
		// time out is given by caller
		if (time_out_ms > 0)
		{
			time_out_circles = time_out_ms/time_out_one_circle_time;
		}
		/* wait until the time out expires or response is arrived */
		for (int i=0; i<response_size; i++)
		{
			uint8_t response_received = 0;
			while (time_out_circles)
			{
				if (uart_is_data_in_rx_buffer(at_response[i]))
				{
					ok_led_blinking(1);
					uart_move_rx_tail();
					response_received = 1;
					break;
				}
				else
				{
					_delay_ms(time_out_one_circle_time);
				}
				/*  */
				time_out_circles--;
			}
			/* response received find next */
			if (response_received)
			{
				continue;
			}
			/* recursive call for the response */
			error_led_blinking(1);
			gsm_put_at_command(at_command, terminate, at_response, response_size, time_out_ms);
		}
		uart_reset_rx_buffer();
	}
}

void gsm_init(void)
{
	gsm_put_at_command(AT_COMMAND_ATTEST, CR_LF_TERMINATOR, AT_RESPONSE_ATTEST, 2, 10000);
	gsm_put_at_command(AT_COMMAND_ECHO_OFF, CR_LF_TERMINATOR, AT_RESPONSE_ECHO_OFF, 2, 0);
	gsm_put_at_command(AT_COMMAND_CLIP_ON, CR_LF_TERMINATOR, AT_RESPONSE_OK, 1, 0);
}

void gsm_event_handler(void)
{
	static volatile uint8_t ring_cnt = 0;

	/* call: RING */
	if (uart_is_data_in_rx_buffer(AT_RESPONSE_RING[0]))
	{
		uart_move_rx_tail();
		ok_led_blinking(1);
	}
	/* call: RING as detailed call with caller ID "CLIP" */
	if (uart_is_data_in_rx_buffer(AT_RESPONSE_CLIP[0]))
	{
		/* just the authorized callers can switch the output */
		if (uart_is_data_in_rx_buffer(AUTHORIZED_MOBILE_NUMBER_1) || uart_is_data_in_rx_buffer(AUTHORIZED_MOBILE_NUMBER_2))
		{
			ring_cnt++;
			ok_led_blinking(1);
			/* if OFF then switch ON at 4th rings */
			if (!OUT_1_STATE() && (ring_cnt >= NUMBER_OF_RINGS_ON))
			{
				ring_cnt = 0;
				OUT_1_ON();
				OUT_2_ON();
				gsm_put_at_command(AT_COMMAND_HANGUP, CR_LF_TERMINATOR, AT_RESPONSE_HANGUP, 1, 0);
			}
			/* if ON than swtich OFF at 2nd rings */
			else if (OUT_1_STATE() && (ring_cnt >= NUMBER_OF_RINGS_OFF))
			{
				ring_cnt = 0;
				OUT_1_OFF();
				OUT_2_OFF();
				gsm_put_at_command(AT_COMMAND_HANGUP, CR_LF_TERMINATOR, AT_RESPONSE_HANGUP, 1, 0);
			}
			else
			{
				uart_reset_rx_buffer();
			}
		}
		/* at not authorized callers hang up the call */
		else
		{
			ring_cnt = 0;
			error_led_blinking(2);
			gsm_put_at_command(AT_COMMAND_HANGUP, CR_LF_TERMINATOR, AT_RESPONSE_HANGUP, 1, 0);
		}
	}
}
