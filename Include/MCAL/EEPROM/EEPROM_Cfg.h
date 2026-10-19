#ifndef INCLUDE_MCAL_EEPROM_EEPROM_CFG_H_
#define INCLUDE_MCAL_EEPROM_EEPROM_CFG_H_

/* ========================================= */
/* SOFTWARE I2C PINS                         */
/* ========================================= */

/*
 * ATmega32
 *
 * PC4 = SCL
 * PC5 = SDA
 */

#define EEPROM_SCL_PORT    DIO_u8_PORTC
#define EEPROM_SCL_PIN     DIO_u8_PIN4

#define EEPROM_SDA_PORT    DIO_u8_PORTC
#define EEPROM_SDA_PIN     DIO_u8_PIN5

/* ========================================= */
/* I2C DELAY                                  */
/* ========================================= */

#define EEPROM_I2C_DELAY_US    5

#endif
