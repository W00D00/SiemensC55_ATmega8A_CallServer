/*
 * gsm.h
 *
 * Created: 2015.01.25. 16:55:43
 *  Author: iszirtes
 */

#ifndef GSM_H_
#define GSM_H_

#include <util/delay.h>

#include "uart.h"
#include "port.h"

extern void gsm_put_at_command(const char *at_command, const char *terminate, const char *at_response[], const uint8_t response_size, uint16_t time_out_ms);
extern void gsm_init(void);
extern void gsm_event_handler(void);

#endif /* GSM_H_ */