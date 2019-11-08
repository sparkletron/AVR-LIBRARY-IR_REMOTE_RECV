/*
 * irremote.c
 *
 *  Created on: Nov 1, 2014
 *      Author: John Convertino
 *      email: electrobs@gmail.com
 *
 *       LIBRARY for atmega328P microcontroller for NEC protocol IR remotes.
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
 *
 *		2019 modified by Hugo Schaaf v1.3
 *	
 */

#include <avr/io.h>
#include <avr/common.h>
#include <avr/interrupt.h>

#include "commonDefines.h"
#include "commonTimer.h"
#include "irremote.h"

/* each pulse length implies burst time + space time so
 * 
 * AGC      --> 9 + 4.5 = 13.5 ms
 * Repeat   --> 9 + 2.25 =  11.25 ms
 * Bit to 1 --> 2.25 ms
 * Bit to 0 --> 1.125 ms
 *
 * reference doc @ https://www.sbprojects.net/knowledge/ir/nec.php
 * 
 * if the protocol isn't NEC extended
 * the data packet time is 54 ms after the AGC initialization pulse
 * the total packet time between 2 repeats is 110 ms
 *
 * the time counter increments itself by 0.1 ms (100us) as a result the
 * following macros :
 * I downsized the lower limits by ~10% to let a little tolerance.
 */
#define AGC_PULSE_LOWER_LIMIT      		122
#define AGC_PULSE_UPPER_LIMIT      		AGC_PULSE_LOWER_LIMIT+17

#define REPEAT_PULSE_LOWER_LIMIT  		101
#define REPEAT_PULSE_UPPER_LIMIT  		REPEAT_PULSE_LOWER_LIMIT+15

#define ONES_PULSE_LOWER_LIMIT 		   	20
#define ONES_PULSE_UPPER_LIMIT			ONES_PULSE_LOWER_LIMIT+6

#define ZEROS_PULSE_LOWER_LIMIT    		10
#define ZEROS_PULSE_UPPER_LIMIT    		ZEROS_PULSE_LOWER_LIMIT+5

#define DATA_PACKET_TIME_LOWER_LIMIT    486
#define TOTAL_PACKET_TIME_LOWER_LIMIT	607 

#define DATA_BIT_LENGTH		32	/* 32 bit codes */

/* Led 13 related macros
 *
 */
#if BLINK_LED == 1
	#define SETUP_LED()		DDRB |= _BV(PB5)
	#define LED_ON()		PORTB |= _BV(PB5)
	#define LED_OFF()		PORTB &= ~_BV(PB5)
#else
	#define SETUP_LED()
	#define LED_ON()
	#define LED_OFF()	
#endif

/* Pin Change related macros
 *
 */
#if 	PORT_TO_USE == 'B'
	#define GPIO_SETUP(pin)		DDRB &= ~(uint8_t)(1 << pin);\
								PCICR |= (uint8_t)(1 << PCIE0)
	#define SET_PCI(pin) 		PCMSK0 |= (uint8_t)(1 << pin);
	#define CLEAR_PCI(pin)		PCMSK0 &= ~(uint8_t)(1 << pin);		

#elif	PORT_TO_USE == 'C'
	#define GPIO_SETUP(pin)		DDRC &= ~(uint8_t)(1 << pin);\
								PCICR |= (uint8_t)(1 << PCIE1)
	#define SET_PCI(pin) 		PCMSK1 |= (uint8_t)(1 << pin);
	#define CLEAR_PCI(pin)		PCMSK1 &= ~(uint8_t)(1 << pin);	

#elif	PORT_TO_USE == 'D'
	#define GPIO_SETUP(pin)		DDRD &= ~(uint8_t)(1 << pin);\
								PCICR |= (uint8_t)(1 << PCIE2)
	#define SET_PCI(pin) 		PCMSK2 |= (uint8_t)(1 << pin);
	#define CLEAR_PCI(pin)		PCMSK2 &= ~(uint8_t)(1 << pin);	
#endif


/* stores data from incoming IR signal 
 * and related housekeeping data
 */
struct
{
    volatile enum states{WAITING, READY, READING } decodingState; /* state machine steps */
    volatile uint8_t pinNumber;
    volatile uint64_t timeStart;
    volatile uint32_t dataBuffer;
    volatile uint8_t shiftCount;
    volatile uint8_t receivingData:1;
    volatile uint8_t dataAvailable:1;
    volatile uint8_t repeatCMD:1;
    volatile uint8_t error:1;
    uint16_t receiverAddress;
    CommandTemplate *remoteCommands;
} ir;


/* will setup the Remote ir,
 * the Counter and the needed Interrupt
 */
void initIR(uint8_t pinNumber, uint16_t address, const CommandTemplate* commands)
{
	uint8_t bufSREG = SREG;

	cli();

	/* setup the led 13 */
	SETUP_LED();

	/*setup initial values */
	ir.pinNumber = pinNumber;
	ir.timeStart = 0;
	ir.decodingState = WAITING;
	ir.dataBuffer = 0;
	ir.shiftCount = 0;
	ir.dataAvailable = FLAG_OFF;
	ir.repeatCMD = FLAG_OFF;
	ir.error = FLAG_OFF;
	ir.receiverAddress = address;
	ir.remoteCommands = commands;
	
	/* setup pin change interrupt on PORTB's pinNumber corresponding GPIO */
	GPIO_SETUP(ir.pinNumber);
	
	/* setup the 10kHz timer, used to measure the pulses length */
	init100usTimer(F_CPU);

	SREG = bufSREG;
}


uint32_t getIRCode(void)
{	
	uint32_t tmp = NO_CODE;

	/* disable Pin Change interrupt */
	CLEAR_PCI(ir.pinNumber);

	if(ir.repeatCMD == FLAG_ON)
	{
		ir.repeatCMD = FLAG_OFF;
		tmp = REPEAT_CODE;
	}
	else if(ir.dataAvailable == FLAG_ON)
	{
		tmp = ir.dataBuffer;
		ir.dataBuffer = 0;
		ir.dataAvailable = FLAG_OFF;
	}

	/* re-enable pin change interrupt */
	SET_PCI(ir.pinNumber);

	return tmp;
}

/* checks for valid address and a valid command,
 * if found in the supplied commandTemplate returns command
 */
CommandAction getIRCommandRec(void)
{
	if(ir.remoteCommands == NULL) return NO_ACTION;
		
	CLEAR_PCI(ir.pinNumber);

	if (ir.repeatCMD == FLAG_ON)
	{
		ir.repeatCMD = FLAG_OFF;
		SET_PCI(ir.pinNumber);
		return CMD_REPEAT;
	}
	if (ir.dataAvailable == FLAG_OFF)
	{
		SET_PCI(ir.pinNumber);
		return NO_ACTION;
	}

	/* save start point of pointer */
	CommandTemplate *startPointer = ir.remoteCommands;
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
					CommandAction tempAction = ir.remoteCommands->action;
					ir.remoteCommands = startPointer;
					SET_PCI(ir.pinNumber);
					return tempAction;
				}
				ir.remoteCommands++;
			}
		}
	}
	ir.remoteCommands = startPointer;
	SET_PCI(ir.pinNumber);
	return NO_ACTION;
}

/* attached to IR decoder and activated on change of state */
#if 	PORT_TO_USE == 'B'
ISR(PCINT0_vect)
{
	uint8_t pinState = (uint8_t)(PINB & (1 << ir.pinNumber));

#elif 	PORT_TO_USE == 'C'
ISR(PCINT1_vect)
{
	uint8_t pinState = (uint8_t)(PINC & (1 << ir.pinNumber));

#elif 	PORT_TO_USE == 'D'
ISR(PCINT2_vect)
{
	uint8_t pinState = (uint8_t)(PIND & (1 << ir.pinNumber));
#endif

	uint64_t timePassed;

	/* if pin Low -> High to Low transition so the IR remote emits a pulsed signal */
	if(pinState == LOW)
	{
		timePassed = e_100microseconds - ir.timeStart;
		ir.timeStart = e_100microseconds;

		/* wait for AGC pulse */
		if(ir.decodingState == WAITING)
		{
			ir.decodingState = READY;
			ir.dataAvailable = FLAG_OFF;
			ir.error = FLAG_OFF;
			LED_ON();
		}

		/* 1st error check */
		else if (timePassed >= TOTAL_PACKET_TIME_LOWER_LIMIT)
		{
			ir.error = FLAG_ON;
		}


		/* if AGC pulse came in start reading the command code */
		else if(ir.decodingState == READY)
		{
			if(timePassed >= AGC_PULSE_LOWER_LIMIT && timePassed <= AGC_PULSE_UPPER_LIMIT)
			{
				ir.decodingState = READING;
			}
			/* if Rpeat pulse came in signal it and reset the state machine */ 
			else if(timePassed >= REPEAT_PULSE_LOWER_LIMIT && timePassed <= REPEAT_PULSE_UPPER_LIMIT)
			{
				ir.decodingState = WAITING;
				ir.repeatCMD = FLAG_ON;
				LED_OFF();	
			}
			/* error case */
			else
			{
				ir.error = FLAG_ON;
			}
		}
		/* in Reading state, check whether pulse length is a 1 or a 0 bit */
		else if(ir.decodingState == READING)
		{
			if(timePassed >= ONES_PULSE_LOWER_LIMIT && timePassed <= ONES_PULSE_UPPER_LIMIT)
			{
				ir.dataBuffer |= (uint32_t)(1UL << ir.shiftCount++);
			}
			else if(timePassed >= ZEROS_PULSE_LOWER_LIMIT && timePassed <= ZEROS_PULSE_UPPER_LIMIT)
			{
				ir.dataBuffer &= ~(uint32_t)(1UL << ir.shiftCount++);
			}
			/* error case */
			else
			{
				ir.error = FLAG_ON;
			}
		}

		/* reset state machine once 32 bits have been received */
		if(ir.shiftCount == DATA_BIT_LENGTH || ir.error == FLAG_ON)
		{
			if(ir.shiftCount == DATA_BIT_LENGTH)
			{
				ir.dataAvailable = FLAG_ON;
				ir.error = FLAG_OFF;
			}
			ir.decodingState = WAITING;
			ir.shiftCount = 0;
			LED_OFF();
		}
	}
}
