#ifndef MCAL_DIO_DIO_INTERFACE_H_
#define MCAL_DIO_DIO_INTERFACE_H_

#include "../../Lib/STD_Types.h"

/* ========================================= */
/* PORTS                                     */
/* ========================================= */

#define DIO_u8_PORTA        0
#define DIO_u8_PORTB        1
#define DIO_u8_PORTC        2
#define DIO_u8_PORTD        3


/* ========================================= */
/* PINS                                      */
/* ========================================= */

#define DIO_u8_PIN0         0
#define DIO_u8_PIN1         1
#define DIO_u8_PIN2         2
#define DIO_u8_PIN3         3
#define DIO_u8_PIN4         4
#define DIO_u8_PIN5         5
#define DIO_u8_PIN6         6
#define DIO_u8_PIN7         7


/* ========================================= */
/* DIRECTION                                 */
/* ========================================= */

#define DIO_u8_INPUT        0
#define DIO_u8_OUTPUT       1


/* ========================================= */
/* VALUE                                     */
/* ========================================= */

#define DIO_u8_LOW          0
#define DIO_u8_HIGH         1


/* ========================================= */
/* INITIAL CONFIGURATION VALUES              */
/* ========================================= */

#define DIO_u8_INTIAL_INPUT     DIO_u8_INPUT
#define DIO_u8_INTIAL_OUTPUT    DIO_u8_OUTPUT

#define DIO_u8_INPUT_FLOATING   DIO_u8_LOW
#define DIO_u8_INPUT_PULLUP     DIO_u8_HIGH

#define DIO_u8_OUTPUT_LOW       DIO_u8_LOW
#define DIO_u8_OUTPUT_HIGH      DIO_u8_HIGH


/* ========================================= */
/* ERROR STATUS                              */
/* ========================================= */

#define DIO_u8_OK               0
#define DIO_u8_NOK              1


/* ========================================= */
/* FUNCTIONS                                 */
/* ========================================= */

void DIO_voidInit(void);

u8 DIO_u8SetPinDirection(
        u8 Copy_u8PortId,
        u8 Copy_u8PinId,
        u8 Copy_u8PinDirection
);

u8 DIO_u8SetPinValue(
        u8 Copy_u8PortId,
        u8 Copy_u8PinId,
        u8 Copy_u8PinValue
);

u8 DIO_u8GetPinValue(
        u8 Copy_u8PortId,
        u8 Copy_u8PinId,
        u8 *Copy_pu8ReturnedPinValue
);

u8 DIO_u8SetPortDirection(
        u8 Copy_u8PortId,
        u8 Copy_u8PortDirection
);

u8 DIO_u8SetPortValue(
        u8 Copy_u8PortId,
        u8 Copy_u8PortValue
);

u8 DIO_u8GetPortValue(
        u8 Copy_u8PortId,
        u8 *Copy_pu8ReturnedPortValue
);

#endif /* MCAL_DIO_DIO_INTERFACE_H_ */
