/*
 * commonTimer.h
 *
 *  Created on: Nov 6, 2014
 *      Author: John Convertino
 *
 *      Collection of ways of initializing timer 0, 1 or 2 for counting @10kHz or 1kHz, 
 *      pass F_CPU for f_cpu setting based on compile time define for f_cpu.
 *      This scales the counters for 8 MHz to 20 Mhz. Designed for Atmega328P.
 *      You may have to check the registers description to use it on another device.
 *
 *  Copyright (C) 2014 John Convertino modified by Hugo Schaaf 2019
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
 *      v1.3
 *
 *      2019 modified by Hugo Schaaf
 *          - added the possibility to use or not the timers
 *          - added the possibility to define which timer to use for each functionnality
 *
 */

#ifndef COMMONTIMER_H_
#define COMMONTIMER_H_

/* select which timer to use. example :
 * #define USE100USTIMER 1 --> allow the use of the 100us timer
 * #define USE100USTIMER 0 --> cancel the use of the 100us timer
 *
 * define which timer to use (timer ID)
 * timer 0 --> 0
 * timer 1 --> 1
 * timer 2 --> 2
 *
 * #define __100USTIMER 0 
 * this will set timer 0 to be used for 100us timer
 * the 1ms and the 100us timers ID must be different !
 */
#define USE100USTIMER   1
#define USE1MSTIMER     0
#define __100USTIMER    0
#define __1MSTIMER      0   

#include <inttypes.h>
#include <avr/interrupt.h>


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Timers in CTC mode
 * OCRxn = F_CPU/(F_CTC*N) - 1
 * N is the prescaling factor
 * F_CTC is the frequency of the compare match
 *
 */


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 * set up 100us interval timer ie. 10kHz counter
 */
#if USE100USTIMER == 1

extern volatile uint64_t e_100microseconds;  /* global counter variable */

static inline
void init100usTimer(uint32_t f_cpu)
{
    uint8_t bufSREG = SREG;
    cli();

#if __100USTIMER == 0      /* use timer 0 */
    OCR0A = (uint8_t)(((f_cpu/10000UL) >> 3) - 1);
    TIMSK0 = (uint8_t)(1 << OCIE0A);
    TCCR0A = (uint8_t)(1 << WGM01);
    TCCR0B = (uint8_t)(1 << CS01); /* 8 prescaling */
#elif __100USTIMER == 1    /* use timer 1 */
    OCR1A = (uint16_t)(((f_cpu/10000UL) >> 3) - 1);
    TIMSK1 = (uint8_t)(1 << OCIE1A);
    TCCR1A = 0x00;
    TCCR1B = (uint8_t)((1 << WGM12) | (1 << CS11)); /* 8 prescaling */
#else                       /* use timer 2 and default this case */
    OCR2A = (uint8_t)((f_cpu/10000UL >> 3) - 1);
    TIMSK2 = (uint8_t)(1 << OCIE2A);
    TCCR2A = (uint8_t)(1 << WGM21);
    TCCR2B = (uint8_t)(1 << CS21); /* 8 prescaling */  
#endif

    SREG = bufSREG;
}
#endif /* USE100USTIMER */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 * set up 1ms interval timer ie. 1kHz counter
 */
#if USE1MSTIMER == 1

extern volatile uint64_t e_1millisecond;

static inline
void init1msTimer(uint32_t f_cpu)
{
    uint8_t bufSREG = SREG;
    cli();

#if __1MSTIMER == 1      /* use timer 1 */
    OCR1A = (uint8_t)(((f_cpu/1000UL) >> 6) - 1);
    TIMSK1 = (uint8_t)(1 << OCIE1A);
    TCCR1A = (uint8_t)(1 << WGM12);
    TCCR1B = (uint8_t)((1 << CS11) | (1<< CS10)); /* 64 prescaling */
#elif __1MSTIMER == 2    /* use timer 2 */
    OCR2A = (uint8_t)(((f_cpu/1000UL) >> 6) - 1);
    TIMSK2 = (uint8_t)(1 << OCIE2A);
    TCCR2A = (uint8_t)(1 << WGM21);
    TCCR2B = (uint8_t)((1 << CS22)); /* 64 prescaling */
#else                    /* use timer 0 and default this case */
    OCR0A = (uint8_t)(((f_cpu/1000UL) >> 6) - 1);
    TIMSK0 = (uint8_t)(1 << OCIE0A);
    TCCR0A = (uint8_t)(1 << WGM01);
    TCCR0B = (uint8_t)((1 << CS01) | (1 << CS00)); /* 64 prescaling */
#endif

    SREG = bufSREG;
}
#endif /* USE1MSTIMER */

#endif /* COMMONTIMER_H_ */
