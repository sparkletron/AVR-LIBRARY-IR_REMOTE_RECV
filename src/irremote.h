/*
 * irremote.h
 *
 *  Created on: Nov 1, 2014
 *      Author: John Convertino
 *      email: electrobs@gmail.com
 *
 *      LIBRARY for atmega328P microcontroller for NEC IR remotes
 *
    Copyright (C) 2014 John Convertino

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *      Current Version: v0.3
 *      see implementation file for details
 */

#ifndef IRREMOTE_H_
#define IRREMOTE_H_

#ifndef COMMONTIMER_H_
#warning "Make sure to use 100uS timer"
#endif

#include <inttypes.h>

//times for pulse length check (100 uS * 130 = 13 mS)
#define AGC_BURST 130
#define REPEAT_TIME 110
#define ONES_TIME 20
#define ZEROS_TIME 9
#define PACKET_TIME 1150

//enum actions for all remotes
typedef enum {NO_ACTION, CMD_REPEAT, VOLUME_UP, VOLUME_DOWN, MUTE, INPUT1, INPUT2, INPUT3, INPUT4, PLAY_PAUSE, PREV, NEXT,
CH_MINUS, CH_PLUS, STOP_MODE, SETUP, ENTER_SAVE, ZERO_TEN, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, REPEAT} commandAction;

typedef struct
{
	uint8_t command;
	commandAction action;
} commandTemplate;

// takes a 16 bit address that corresponds to the remote to be used (can be 8 or 16 bit), a uint8_t for pinNumber (0 to 5 on port c) and a struct of the remote codes
void initIR(uint8_t pinNumber, uint16_t address, commandTemplate *commands);
//will return a enum commandAction that corresponds to the remote code.
commandAction getIRCommandRec();

#endif /* IRREMOTE_H_ */
