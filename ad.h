//ad.h

#ifndef __AD_H_
#define __AD_H_

#include <avr/io.h>
#include <inttypes.h>

extern void ADC_init();
extern void ADC_measure(uint8_t ch, uint16_t *p);

#endif
