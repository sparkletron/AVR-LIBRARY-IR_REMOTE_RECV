/*
 * commonTimer.c
 *
 *  Created on: Nov 6, 2014
 *      Author: John Convertino
 *
 *      Collection of ways of initializing timer 2 for counting, pass F_CPU for speed setting
 *      based on compile time define for speed. This scales the counters for 8 MHz to 16 Mhz.
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
 *      Version Status: 0.3
 *      Feb, 11 2014	0.3	changed timers to be scalable by speed and added timer 0 1 ms
 *      11/5/14		0.1	setup first 100us counter
 */
#include "commonTimer.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * set up 100us interval timer interrupt
 */
#if USE100USTIMER == 1

volatile uint64_t e_100microseconds = 0;

/* increment by 100us step */
#if __100USTIMER == 0
ISR(TIMER0_COMPA_vect)
#elif __100USTIMER == 1
ISR(TIMER1_COMPA_vect)
#else
ISR(TIMER2_COMPA_vect)
#endif
{
	e_100microseconds++;
}
#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 *set up 1ms interval timer interrupt
 */
#if USE1MSTIMER == 1

volatile uint64_t e_1millisecond = 0;

/* increment by 1ms */
#if __1MSTIMER == 1
ISR(TIMER1_COMPA_vect)
#elif __1MSTIMER == 2
ISR(TIMER2_COMPA_vect)
#else
ISR(TIMER0_COMPA_vect)
#endif
{
  e_1millisecond++;
}

#endif