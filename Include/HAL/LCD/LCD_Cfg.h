#ifndef LCD_CFG_H_
#define LCD_CFG_H_

#include "../../MCAL/DIO/DIO_Interface.h"

#define LCD_u8_DATA_PORT       DIO_u8_PORTB

#define LCD_u8_CONTROL_PORT    DIO_u8_PORTC

#define LCD_u8_RS_PIN          DIO_u8_PIN0
#define LCD_u8_RW_PIN          DIO_u8_PIN1
#define LCD_u8_E_PIN           DIO_u8_PIN2

#endif
