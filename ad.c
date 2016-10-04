//ad.c
//ref 4,096 LM35

#include "ad.h"
#include <avr/io.h>

void ADC_init(){

		ADMUX |= (0<<REFS0);	                            ///ADCref AREF T-bird3 ~3,8-4V capacitive
		ADCSRA |= ((1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2));	///Prescale for speed (128) from XTAL
		ADCSRA |= (1<<ADFR);                                ///ADC --> free running
		ADCSRA |= (1<<ADEN);                                ///ADC --> enable
}
void ADC_measure(uint8_t ch, uint16_t *p){

		ch = ch&0x07;                       ///channel mask 0-2bit
		ADMUX |= ch;                        ///channel enable on MUX
		ADCSRA |= (1<<ADSC);			    ///START a conversion
		while(!(ADCSRA & (1<<ADIF)));		///WAIT for done
		*p = ADC;                           ///READ 16bit
		ADMUX &= ~ch;                       ///CHANEL --> be free
}
