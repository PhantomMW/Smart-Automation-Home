#ifndef MCAL_EEPROM_EEPROM_PRIVATE_H_
#define MCAL_EEPROM_EEPROM_PRIVATE_H_

/* ========================================= */
/* EEPROM I2C PINS                           */
/* ========================================= */

/* PA4 = SDA */
#define EEPROM_SDA_PORT     DIO_u8_PORTA
#define EEPROM_SDA_PIN      DIO_u8_PIN4

/* PA5 = SCL */
#define EEPROM_SCL_PORT     DIO_u8_PORTA
#define EEPROM_SCL_PIN      DIO_u8_PIN5

/* Software I2C delay */
#define EEPROM_I2C_DELAY_US     5

#endif /* MCAL_EEPROM_EEPROM_PRIVATE_H_ */
