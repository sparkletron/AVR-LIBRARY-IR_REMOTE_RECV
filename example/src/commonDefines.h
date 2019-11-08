/*
 * commonDefines.h
 *
 *  Created on: Nov 7, 2014
 *      Author: John Convertino
 *      email: electrobs@gmail.com
 *		
 *		Just common defines for libraries and projects, for consistencies sake.
 *		These defines with time have been depreciated, especially for masking and selection.
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
 *		Version: v0.PI
 *		Nov 7, 2014 v0.PI	Initial file, just add other defines as needed, no version required
 *
 *		TODO
 */

#ifndef COMMONDEFINES_H_
#define COMMONDEFINES_H_

//masks, not a great way of masking, just here for sake of being
#define MASK_8BIT_FF 0xFF
#define MASK_16BIT_FF 0x000000FF
#define MASK_16BIT_FFFF 0x0000FFFF
//flag states
#define FLAG_ON 1
#define FLAG_OFF 0
#define	HIGH 1
#define LOW 0
//selection defines
#define SELECT_B 'B'
#define SELECT_C 'C'
#define SELECT_D 'D'
//bases
#define HEX 16
#define DEC 10
#define OCT 8
#define BIN 2

#ifndef NULL
#define NULL (void*)0x0
#endif

#endif /* COMMONDEFINES_H_ */
