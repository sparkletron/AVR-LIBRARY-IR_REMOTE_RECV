/*******************************************************************************
 * @file    remotes.h
 * @author  Jay Convertino(electrobs@gmail.com)
 * @date    2014.11.06
 * @brief   NEC IR remote protocol remote defines
 * @version 0.1.0
 *
 * @TODO
 *  - Cleanup interface
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

#ifndef REMOTES_H_
#define REMOTES_H_

#include "irremote.h"

//all remotes must end with {something, NO_ACTION}
//{DATA, ACTION}

//ADAFRUIT_REMOTE
#define ADAFRUIT_ADDRESS 0xBF00
commandTemplate ADAFRUIT_REMOTE[] = {{0x02, VOLUME_UP}, {0x00, VOLUME_DOWN},
{0x01, PLAY_PAUSE}, {0x04, SETUP}, {0x05, PREV}, {0x06, STOP_MODE},
{0x08, CH_MINUS}, {0x09, ENTER_SAVE}, {0x0A, CH_PLUS}, {0x0C, ZERO_TEN},
{0x0D, NEXT}, {0x0E, REPEAT}, {0x10, ONE}, {0x11, TWO}, {0x12, THREE},
{0x14, FOUR}, {0x15, FIVE}, {0x16, SIX}, {0x18, SEVEN},
{0x19, EIGHT}, {0x1A, NINE}, {0xFF, NO_ACTION}};

#endif /* REMOTES_H_ */
