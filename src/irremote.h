/*******************************************************************************
 * @file    irremote.h
 * @author  Jay Convertino(electrobs@gmail.com)
 * @date    2014.11.06
 * @brief   NEC IR remote protocol
 * @version 0.3.0
 *
 *
 * @license mit
 *
 * Copyright 2024 Johnathan Convertino
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/
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
