/*
 * remotes.h
 *
 *  Created on: Nov 5, 2014
 *      Author: John Convertino
 *      email: electrobs@gmail.com
 *
    Copyright (C) 2014 John Convertino/dev/ttyS1

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


    2019 modified by Hugo Schaaf
        - Added amazon remote mapping
        - Moved commandAction and commandTemplate from irremote.h to here

 */

#ifndef REMOTES_H_
#define REMOTES_H_

#include <inttypes.h>

/* List of possible actions
 * You can add your custom actions
 */
typedef enum
{
    NO_ACTION, CMD_REPEAT, ARROW_UP, ARROW_DOWN, ARROW_LEFT, ARROW_RIGHT, VOLUME_UP, VOLUME_DOWN, MUTE,
    INPUT1, INPUT2, INPUT3, INPUT4, PLAY_PAUSE, PREV, NEXT, CH_MINUS, CH_PLUS, STOP_MODE, SETUP, OK,
    ENTER_SAVE, ZERO, ZERO_TEN, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, HASH, TIMES, REPEAT
} CommandAction;


/* commandTemplate structure */
typedef struct
{
    uint8_t command;
    CommandAction action;
} CommandTemplate;

/* List of remotes
 * all remotes must end with {something, NO_ACTION}
 * You can add your custom remotes following the rule above and giving
 * the 8 bits non-inverted corresponding code.
 */

/* ADAFRUIT REMOTE */
extern CommandTemplate ADAFRUIT_REMOTE[];
#define ADAFRUIT_ADDRESS    0xBF00

/* AMAZON UNKNOWN REMOTE
 *  ___________
 *  | 1  2  3 |
 *  | 4  5  6 |
 *  | 7  8  9 |
 *  | *  0  # |
 *  |    ▲    |
 *  | ◄  OK ► |
 *  |    ▼    |
 *  |_________|
 */ 
extern CommandTemplate AMAZON_REMOTE[];
#define AMAZON_ADDRESS      0x0000


#endif /* REMOTES_H_ */

