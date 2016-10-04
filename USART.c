#include "USART.h"
#include <avr/interrupt.h>
#include "FIFO.h"

static FIFO recieve, transmit;
static uint8_t r[recive_buffer_size],t[transmit_buffer_size];
volatile uint8_t sen = 0;

void USART_Init( unsigned int ubrr )
{
/* Set baud rate */
UBRR0H = (unsigned char)(ubrr>>8);
UBRR0L = (unsigned char)ubrr;
/* Enable receiver and transmitter */
UCSR0B = (1<<RXEN)|(1<<TXEN) | (1<< RXCIE0);
/* Set frame format: 8data, 2stop bit */
UCSR0C = (1<<USBS)|(3<<UCSZ0);

    fifo_init(&recieve,r,recive_buffer_size);
    fifo_init(&transmit,t,transmit_buffer_size);
}

void USART_Transmit( unsigned char data ){
    fifo_send(&transmit,data);
    UCSR0B |= (1 << UDRIE0);

}
void USART_Transmit_Puts(char *s){
    while (*s){
        USART_Transmit(*s);
        s++;
    }
    USART_Transmit('\n');
    USART_Transmit('\r');
}
unsigned char USART_Receive( void ){
   uint8_t tmp;
   if(tmp = fifo_receive(&recieve)) return tmp;
   else return 0;
}

uint8_t has_sen(){
    return sen;
}

uint8_t USART_get_sen(uint8_t* buff){
    uint8_t tmp,i = 0;
    while((tmp = USART_Receive()) != 13){
        buff[i] = tmp;
        i++;
    }
    buff[i] = 0;
    sen--;
    return i;
}

ISR(USART0_RX_vect){
    uint8_t tmp = UDR0;
    if(tmp == 13) sen++;
    fifo_send(&recieve,tmp);
    UDR0 = tmp;

}

ISR(USART0_UDRE_vect){
    if(!transmit.fifo_elements){ //tilt
        UCSR0B &= ~(1 << UDRIE0);
    }else{
        UDR0 = fifo_receive(&transmit);
    }
}
