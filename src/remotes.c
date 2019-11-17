/*
 * remotes.c
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
#include "remotes.h"

CommandTemplate ADAFRUIT_REMOTE[] =
{
	{0x02, VOLUME_UP},	{0x00, VOLUME_DOWN},	{0x01, PLAY_PAUSE},
	{0x04, SETUP}, 		{0x05, PREV}, 			{0x06, STOP_MODE},
	{0x08, CH_MINUS}, 	{0x09, ENTER_SAVE}, 	{0x0A, CH_PLUS},
	{0x0C, ZERO_TEN},	{0x0D, NEXT}, 			{0x0E, REPEAT},
	{0x10, ONE}, 		{0x11, TWO}, 			{0x12, THREE},
	{0x14, FOUR},		{0x15, FIVE}, 			{0x16, SIX},
	{0x18, SEVEN}, 		{0x19, EIGHT}, 			{0x1A, NINE},
	{0xFF, NO_ACTION}
};

CommandTemplate AMAZON_REMOTE[] =
{ 
	{0x45, ONE}, 		{0x46, TWO}, 		{0x47, THREE},
	{0x44, FOUR},		{0x40, FIVE}, 		{0x43, SIX},
	{0x07, SEVEN}, 		{0x15, EIGHT}, 		{0x09, NINE},
    {0x16, TIMES}, 		{0x19, ZERO}, 		{0x0D, HASH},
    {0x18, ARROW_UP}, 	{0x08, ARROW_LEFT},	{0x1C, OK},
    {0x5A, ARROW_RIGHT},{0x52, ARROW_DOWN}, {0xFF, NO_ACTION}
};