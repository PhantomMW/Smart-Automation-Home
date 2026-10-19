#ifndef MCAL_TIMER_TIMER_PRIVATE_H_
#define MCAL_TIMER_TIMER_PRIVATE_H_

#include <avr/io.h>


/* ========================================= */
/* TIMER0 REGISTERS                          */
/* ========================================= */

#define TIMER0_TCCR0_REG    TCCR0
#define TIMER0_TCNT0_REG    TCNT0
#define TIMER0_OCR0_REG     OCR0
#define TIMER0_TIMSK_REG    TIMSK


/* ========================================= */
/* TIMER0 BIT NUMBERS                        */
/* ========================================= */

#define TIMER0_WGM00        6
#define TIMER0_WGM01        3

#define TIMER0_COM00        4
#define TIMER0_COM01        5

#define TIMER0_CS00         0
#define TIMER0_CS01         1
#define TIMER0_CS02         2

#define TIMER0_OCIE0        1


#endif
