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
 *
 *      2019 modified by Hugo Schaaf v1.3
 *          - added getIRCode function to only get the received code
 *          - added the possibility to use GPIOs of PORTB or PORTC as signal entry
 *          - modified the decoding State Machine because it didn't work with my NEC remotes. don't know why...
 *          - added the possibility to switch on the LED 13 on Arduino
 *          - added signal input error detection to avoid to block the state machine
 */

#ifndef IRREMOTE_H_
#define IRREMOTE_H_

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                      CONFIGURATION                            */ 
/*                                                               */

/* Select the remote signal entry GPIO port
 * #define PORTTOUSE 'B' --> use GPIOs on PORTB as signal entry
 * #define PORTTOUSE 'C' --> use GPIOs on PORTC as signal entry
 * #define PORTTOUSE 'D' --> use GPIOs on PORTD as signal entry
 */
#define PORT_TO_USE     'B'

/* Switch on led on PB5 (led 13) while receiving on Arduino UNO and NANO boards
 * #define BLINK_LED 0 --> disable feature 
 * #define BLINK_LED 1 --> enable feature
 */
#define BLINK_LED   1


#ifndef COMMONTIMER_H_
    #warning "Make sure to use 100uS timer"
#endif

#include <inttypes.h>
#include "remotes.h"

/* takes a 16 bit address that corresponds to the remote to be used (can be 8 or 16 bit),
 * a uint8_t for pinNumber and a struct of the remote codes.
 * If you want to only read codes with getIRCode(), give commands as NULL and a dummy uint16_t value
 * as address. Then getIRCommandRec() will always return NO_ACTION and the parameter address will be ignored.
 */
void initIR(uint8_t pinNumber, uint16_t address, const CommandTemplate *commands);

/* returns the received code, REPEAT_CODE if repeat code
 * or NO_CODE if no code retreived
 */
#define NO_CODE     0x00000000UL
#define REPEAT_CODE 0x00000001UL

uint32_t getIRCode(void);

/* returns commandAction that corresponds
 * to the remote code.
 * returns NO_ACTION if the code hasn't been found in commands or if commands == NULL
 */
CommandAction getIRCommandRec(void);

#endif /* IRREMOTE_H_ */
