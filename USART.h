#ifndef USART_H_INCLUDED
#define USART_H_INCLUDED

#define recive_buffer_size 128
#define transmit_buffer_size 128

#include "inttypes.h"
void USART_Init( unsigned int ubrr );
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );
uint8_t has_sen();
uint8_t USART_get_sen(uint8_t* buff);
void USART_Transmit_Puts(char *s);

#endif // USART_H_INCLUDED
