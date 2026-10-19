#include "../Include/Lib/STD_Types.h"
#include "../Include/Lib/BIT_Math.h"

#define F_CPU 8000000UL

#include "../Include/MCAL/UART/UART_Interface.h"
#include "../Include/MCAL/UART/UART_Private.h"
#include "../Include/MCAL/UART/UART_Cfg.h"


void UART_voidInit(void)
{
    u16 Local_u16UBRR = 103;

    /* Disable UART */
    CLR_BIT(UCSRB, RXEN);
    CLR_BIT(UCSRB, TXEN);


    /* Double Speed Mode */
    SET_BIT(UCSRA, U2X);


    /*
     * UBRRH / UCSRC share address 0x40
     *
     * URSEL = 0 -> UBRRH
     */

    UBRRH = (u8)(Local_u16UBRR >> 8);

    UBRRL = (u8)(Local_u16UBRR);


    /*
     * UCSRC
     *
     * URSEL = 1
     */

    UCSRC =
        (1 << URSEL) |
        (0 << UMSEL) |
        (0 << UPM1)  |
        (0 << UPM0)  |
        (0 << USBS)  |
        (1 << UCSZ1) |
        (1 << UCSZ0) |
        (0 << UCPOL);


    /*
     * UCSZ2 = 0
     */

    CLR_BIT(UCSRB, UCSZ2);


    /* Enable Receiver and Transmitter */
    SET_BIT(UCSRB, RXEN);
    SET_BIT(UCSRB, TXEN);
}


/* ========================================= */
/* Send Byte                                */
/* ========================================= */

u8 UART_u8SendByte(u8 Copy_u8Data)
{
    while (GET_BIT(UCSRA, UDRE) == 0)
    {
    }

    UDR = Copy_u8Data;

    return STD_TYPES_OK;
}


/* ========================================= */
/* Receive Byte                             */
/* ========================================= */

u8 UART_u8ReceiveByte(u8 *Copy_pu8Data)
{
    if (Copy_pu8Data == NULL)
    {
        return STD_TYPES_NOK;
    }

    while (GET_BIT(UCSRA, RXC) == 0)
    {
    }

    *Copy_pu8Data = UDR;

    return STD_TYPES_OK;
}


/* ========================================= */
/* Receive Byte Non Blocking                */
/* ========================================= */

u8 UART_u8ReceiveByteNonBlocking(u8 *Copy_pu8Data)
{
    if (Copy_pu8Data == NULL)
    {
        return STD_TYPES_NOK;
    }

    if (GET_BIT(UCSRA, RXC) == 0)
    {
        return STD_TYPES_NOK;
    }

    *Copy_pu8Data = UDR;

    return STD_TYPES_OK;
}


/* ========================================= */
/* Send String                              */
/* ========================================= */

u8 UART_u8SendString(u8 *Copy_pu8String)
{
    u8 Local_u8Index = 0;

    if (Copy_pu8String == NULL)
    {
        return STD_TYPES_NOK;
    }

    while (Copy_pu8String[Local_u8Index] != '\0')
    {
        UART_u8SendByte(Copy_pu8String[Local_u8Index]);

        Local_u8Index++;
    }

    return STD_TYPES_OK;
}
