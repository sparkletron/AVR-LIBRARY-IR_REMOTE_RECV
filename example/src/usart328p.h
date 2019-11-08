/*
 * usart328p.h
 *
 * Created: 20/02/2017 17:42:32
 *  Author: SCHAAF Hugo
 */ 
#ifndef USART328P_H_
#define USART328P_H_

#ifdef __AVR_ATmega328P__


#include <avr/io.h>

/* Port definitions */
#define USART_DDR		DDRD
#define USART_RX		PD0
#define USART_TX		PD1

void usart_init(const uint32_t baud);
//check if the buffer is full
uint8_t usart_available(void);

/****to receive sthg via usart****/

char usart_getchar(void);

/****to send sthg via usart****/

//to send a char
void usart_putchar(const char c);
//to send a string
void usart_puts(const char* s);

#endif
#endif