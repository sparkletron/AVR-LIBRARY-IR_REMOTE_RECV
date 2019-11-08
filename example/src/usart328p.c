#include "usart328p.h"

#ifdef __AVR_ATmega328P__

void usart_init(const uint32_t baud)
{
	USART_DDR |= _BV(USART_TX) | _BV(USART_RX);
	//Set baud rate
	uint16_t ubrr = (uint16_t)(F_CPU/16/baud-1);/* for normal mode */
	UBRR0H = (uint8_t)(ubrr>>8);
	UBRR0L = (uint8_t)ubrr;
	//select frame format (8 bit data, 1 stop bit, no parity bit)
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
	//enable Tx & Rx mode
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
}

//check if the buffer is full
uint8_t usart_available(void)
{
	//if there is unread data in the buffer
	if(UCSR0A & _BV(RXC0)) return 1;
	return 0;
}

/****to receive sthg via usart****/

char usart_getchar(void)
{
	loop_until_bit_is_set(UCSR0A, RXC0);
	//while(!(UCSR0A & _BV(RXC0))){;;}//wait for incoming data
	return (char)UDR0;
}

/****to send sthg via usart****/

//to send a char
void usart_putchar(const char c)
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	//while(!(UCSR0A & _BV(UDRE0))){;;}//wait for transmission to be ready
	UDR0 = (uint8_t)c;//load the character to transmit
}

//to send a string
void usart_puts(const char* s)
{
	while(*s != '\0'){
		usart_putchar(*s++);
		//s++;
	}
}

#endif