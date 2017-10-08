/*
 * irremote.c
 *
 *  Created on: Nov 1, 2014
 *      Author: John Convertino
 *      email: electrobs@gmail.com
 *
 *       LIBRARY for atmega328P microcontroller for NEC IR remotes.
 *       -Decodes via timing the pulses with a pin change interrupt, must
 *        be used on a dedicated microcontroller, or with minimal other functions.
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
 *
 *        Version:  v0.3
 *        11/5/14	v0.3	Added common timer with extern to keep time constant
 *        11/5/14	v0.2	Comments and added changeable remote capability.
 *        11/4/14	v0.1	First working version
 */

#include "irremote.h"
#include "commonTimer.h"
#include "commonDefines.h"
//avr gcc
#include <avr/io.h>
#include <avr/common.h>
#include <avr/interrupt.h>

//stores data from incoming IR signal and related housekeeping data
struct
{
	enum states{ START, SKIP, EVAL } decodingStates;
	uint8_t pinNumber;
	uint64_t timePassed;
	uint64_t timeStart;
	uint32_t dataBuffer;
	uint16_t receiverAddress;
	uint8_t shiftCount;
	uint8_t receivingData:1;
	uint8_t repeatCMD:1;
	commandTemplate *remoteCommands;
} ir;

//will setup counters, and pin interrupt
void initIR(uint8_t pinNumber, uint16_t address, commandTemplate *commands)
{
	uint8_t tmpSREG = 0;

	tmpSREG = SREG;
	cli();

	//setup initial values for IR_DATA
	ir.receiverAddress = address;
	ir.decodingStates = START;
	ir.dataBuffer = 0;
	ir.timePassed = 0;
	ir.timeStart = 0;
	ir.shiftCount = 0;
	ir.receivingData = FLAG_OFF;
	ir.repeatCMD = FLAG_OFF;
	ir.pinNumber = pinNumber;
	ir.remoteCommands = commands;
	//setup change of pin state interrupt
	DDRC &= ~(1 << ir.pinNumber);
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << ir.pinNumber);

	SREG = tmpSREG;

	sei();
}

//checks for valid address and a valid command, if found returns command
commandAction getIRCommandRec()
{
	uint8_t buffSREG = SREG;
	cli();

	if (ir.receivingData == FLAG_ON)
	{
		SREG = buffSREG;
		return NO_ACTION;
	}
	if (ir.repeatCMD == FLAG_ON)
	{
		ir.repeatCMD = FLAG_OFF;
		SREG = buffSREG;
		return CMD_REPEAT;
	}

	//save start point of pointer
	commandTemplate *startPointer = ir.remoteCommands;
	uint8_t address8bit = ir.dataBuffer & MASK_16BIT_FF;
	uint8_t address8bitINV = ~(ir.dataBuffer >> 8) & MASK_16BIT_FF;
	uint16_t address16bit = ir.dataBuffer & MASK_16BIT_FFFF;

	if((address16bit == ir.receiverAddress) || ((address8bit == address8bitINV) && (address8bit == ir.receiverAddress)))
	{

		uint8_t command = (ir.dataBuffer >> 16) & MASK_16BIT_FF;
		uint8_t commandINV = ~(ir.dataBuffer >> 24) & MASK_16BIT_FF;
		if(command == commandINV)
		{
			//loop till last command which is NO_ACTION
			while(ir.remoteCommands->action != NO_ACTION)
			{
				if(command == ir.remoteCommands->command)
				{
					ir.dataBuffer = 0;
					commandAction tempAction = ir.remoteCommands->action;
					ir.remoteCommands = startPointer;
					SREG = buffSREG;
					return tempAction;
				}
				ir.remoteCommands++;
			}
		}
	}
	ir.remoteCommands = startPointer;
	SREG = buffSREG;
	return NO_ACTION;
}

//attached to IR decoder and activated on change of state
ISR(PCINT1_vect)
{
	uint8_t pinState = (PINC & (1 << ir.pinNumber)) >> ir.pinNumber;

	//tests for low state, meaning a high to low transition has occurred
	if (pinState == LOW)
	{
		ir.timePassed = e_100microseconds - ir.timeStart;

		if(ir.timePassed > PACKET_TIME)
		{
			ir.shiftCount = 0;
			ir.receivingData = FLAG_OFF;
			ir.repeatCMD = FLAG_OFF;
			ir.decodingStates = START;
		}

		switch(ir.decodingStates)
		{
		case START:
			ir.decodingStates = SKIP;
		case EVAL:
			ir.timeStart = e_100microseconds;
		default:
			break;
		}
	}
	else
	{	//since pin must be high, check state machine as to what action to perform
		switch(ir.decodingStates)
		{
		case SKIP:
			ir.decodingStates = EVAL;
			break;
		case EVAL:
			if(ir.timePassed > AGC_BURST)
			{
				ir.receivingData = FLAG_ON;
				ir.repeatCMD = FLAG_OFF;
			}
			else if (ir.timePassed > REPEAT_TIME)
			{
				ir.repeatCMD = FLAG_ON;
				ir.receivingData = FLAG_OFF;
				ir.decodingStates = START;
			}
			else if (ir.timePassed > ONES_TIME)
			{
				//source of major bug here, must cast one for the size need, defaults to only 16 bit
				ir.dataBuffer |= ((uint32_t)1 << ir.shiftCount);
				ir.shiftCount++;
			}
			else if (ir.timePassed > ZEROS_TIME)
			{
				//same as above this is done more of a just in case, the shift could be removed.
				ir.dataBuffer &= ~((uint32_t)1 << ir.shiftCount);
				ir.shiftCount++;
			}
			if(ir.shiftCount >= 32)
			{
				ir.shiftCount = 0;
				ir.receivingData = FLAG_OFF;
				ir.decodingStates = START;
			}
			break;
		default:
			break;
		}
	}
}
