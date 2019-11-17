/*
 * main.c
 *
 *  Created on: Sep 8, 2019
 *      Author: Hugo Schaaf
 *
 *      Use the Ir remote library.
 *		for arduino Nano :
 * 		- connect the signal line to PB3
 *		- set baudrate to 19200 bps
 *
 *  Copyright (C) 2019 Hugo Schaaf
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *
 */

#include <avr/io.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "stdio.h"

#include "irremote.h"
#include "usart.h"


#define GET_CODE
//#define GET_CMD


int main(void)
{
	char debugStr[50] = "";

	usart_init(19200);
	usart_puts("Initializing IR\n");

	initIR(PB3, AMAZON_ADDRESS, AMAZON_REMOTE);

	sei();

	while(1)
	{

#ifdef GET_CODE

		int32_t code = getIRCode();
		if(code != -1)
		{
			sprintf(debugStr, "code : %lx\n", code);
			usart_puts(debugStr);
		}
#endif
#ifdef GET_CMD
		CommandAction cmd = getIRCommandRec();
		if(cmd != NO_ACTION)
		{
			sprintf(debugStr, "command : %d\n", cmd);
			usart_puts(debugStr);
		}
#endif

	}

	return 0;
}