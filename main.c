#define F_CPU (8000000UL)

#include <avr/io.h>
#include <avr/iom128.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#include "lcd.h"
#include "FIFO.h"
#include "USART.h"
#include "ad.h"

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#define I_SPEED 5

///GLOBAL VAR's
uint8_t buffer[16]  = {0};
uint8_t ADC_en      = 0;
uint8_t L_SHIFT_en  = 0;
uint8_t glob_en     = 0;
uint8_t tick        = 0;
uint8_t tmp;

///FGV_DECLAR
void LCD_Puts(char *s);
void menu();
void cmd();
void adc();
void shift();

int main()
{

    TCCR0 = (1<<CS02)| (1<<CS01) | (1<<CS00); ///~30Hz
    TIMSK = (1<<TOIE0);

    LCD_init();
    ADC_init();
    USART_Init(MYUBRR);
    menu();
    sei();
    LCD_Puts("LCD_is_ON");

    while(1)
    {

        if(has_sen())
        {
            USART_get_sen(buffer);
            cmd();
        }
        if(glob_en)
        {
            shift();
            adc();
            glob_en=0;
        }

    }
    return 0;
}

ISR(TIMER0_OVF_vect)
{
    if(!glob_en)
    {
        if(tick == I_SPEED)
        {
            glob_en = 1;
            tick    = 0;
        }
        tick++;
    }
}

void menu()
{

    USART_Transmit(12);
    USART_Transmit_Puts("****MENU****");
    USART_Transmit_Puts("");            ///only /enter
    USART_Transmit_Puts("1. first");
    USART_Transmit_Puts("2. second");
    USART_Transmit_Puts("3. third");
    USART_Transmit_Puts("4. fourth");
    USART_Transmit_Puts("5. help");
    USART_Transmit('>');
}

void cmd()
{
    uint8_t element=0;
    USART_Transmit(12);

    if(!(strcmp(buffer,"first"))    || (!(strcmp(buffer,"1")))) { element = 1;
    }
    if(!(strcmp(buffer,"second"))   || (!(strcmp(buffer,"2")))) { element = 2;
    }
    if(!(strcmp(buffer,"third"))    || (!(strcmp(buffer,"3")))) { element = 3;
    }
    if(!(strcmp(buffer,"fourth"))   || (!(strcmp(buffer,"4")))) { element = 4;
    }
    if(!(strcmp(buffer,"help"))     || (!(strcmp(buffer,"h")))) { element = 5;
    }

    switch(element)
    {
    case 1:
        USART_Transmit_Puts(" detected->LCD_shift_on/off");
        L_SHIFT_en=~L_SHIFT_en;
        LCD_cmd(0x02);                                          ///lcd home adress
        LCD_goto(0,0);
        LCD_Puts("Menu-shift   ");
        break;
    case 2:
        USART_Transmit_Puts(" detected->empty2");

        LCD_goto(0,0);
        LCD_Puts("Menu-second  ");
        break;
    case 3:
        USART_Transmit_Puts(" detected->empty3");

        LCD_goto(0,0);
        LCD_Puts("Menu-third   ");
        break;
    case 4:
        USART_Transmit_Puts(" detected->ADC_enable/disable");
        ADC_en=~ADC_en;
        LCD_goto(0,0);
        LCD_Puts("ADC_EN/DIS   ");
        break;
    case 5:
        USART_Transmit_Puts(" detected->help");
        menu();
        LCD_goto(0,0);
        LCD_Puts("Help_menu    ");
        break;
    default:
        USART_Transmit_Puts(" undetected->error");
        LCD_goto(0,0);
        LCD_Puts("Fail_cmd     ");
        break;
    }
    USART_Transmit('>');
}
void adc()
{

    uint16_t temp;
    char str[15]= {0};

    if(ADC_en)
    {
        ADC_measure(0,&temp);
        itoa(temp/4,str,10);
        LCD_goto(3,0);
        LCD_Puts("Temp: ...C :");
        LCD_Puts(str);
        /*USART_Transmit(12);
        USART_Transmit_Puts("Temp: ...C :");
        USART_Transmit_Puts(str);*/
    }
    else
    {
        LCD_goto(3,0);
        LCD_Puts("               ");
    }
}

void shift()
{

    if(L_SHIFT_en)
    {
        LCD_shift();
    }
}

