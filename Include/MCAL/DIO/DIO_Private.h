#ifndef MCAL_DIO_DIO_PRIVATE_H_
#define MCAL_DIO_DIO_PRIVATE_H_

#include <avr/io.h>

/* ========================================= */
/* PORT A REGISTERS                          */
/* ========================================= */

#define DIO_u8_DDRA_REG       DDRA
#define DIO_u8_PORTA_REG      PORTA
#define DIO_u8_PINA_REG       PINA


/* ========================================= */
/* PORT B REGISTERS                          */
/* ========================================= */

#define DIO_u8_DDRB_REG       DDRB
#define DIO_u8_PORTB_REG      PORTB
#define DIO_u8_PINB_REG       PINB


/* ========================================= */
/* PORT C REGISTERS                          */
/* ========================================= */

#define DIO_u8_DDRC_REG       DDRC
#define DIO_u8_PORTC_REG      PORTC
#define DIO_u8_PINC_REG       PINC


/* ========================================= */
/* PORT D REGISTERS                          */
/* ========================================= */

#define DIO_u8_DDRD_REG       DDRD
#define DIO_u8_PORTD_REG      PORTD
#define DIO_u8_PIND_REG       PIND


#endif /* MCAL_DIO_DIO_PRIVATE_H_ */
