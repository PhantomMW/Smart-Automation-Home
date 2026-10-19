#include "../Include/Lib/STD_Types.h"
#include "../Include/Lib/BIT_Math.h"

#include "../Include/MCAL/DIO/DIO_Interface.h"
#include "../Include/MCAL/DIO/DIO_Private.h"
#include "../Include/MCAL/DIO/DIO_Cfg.h"


/* ========================================= */
/* DIO INITIALIZATION                        */
/* ========================================= */

void DIO_voidInit(void)
{
    /* ===================================== */
    /* PORT A                                */
    /* ===================================== */

    DIO_u8_DDRA_REG =
          (DIO_u8_PA0_INTIAL_DIRECTION << 0)
        | (DIO_u8_PA1_INTIAL_DIRECTION << 1)
        | (DIO_u8_PA2_INTIAL_DIRECTION << 2)
        | (DIO_u8_PA3_INTIAL_DIRECTION << 3)
        | (DIO_u8_PA4_INTIAL_DIRECTION << 4)
        | (DIO_u8_PA5_INTIAL_DIRECTION << 5)
        | (DIO_u8_PA6_INTIAL_DIRECTION << 6)
        | (DIO_u8_PA7_INTIAL_DIRECTION << 7);

    DIO_u8_PORTA_REG =
          (DIO_u8_PA0_INTIAL_VALUE << 0)
        | (DIO_u8_PA1_INTIAL_VALUE << 1)
        | (DIO_u8_PA2_INTIAL_VALUE << 2)
        | (DIO_u8_PA3_INTIAL_VALUE << 3)
        | (DIO_u8_PA4_INTIAL_VALUE << 4)
        | (DIO_u8_PA5_INTIAL_VALUE << 5)
        | (DIO_u8_PA6_INTIAL_VALUE << 6)
        | (DIO_u8_PA7_INTIAL_VALUE << 7);


    /* ===================================== */
    /* PORT B                                */
    /* ===================================== */

    DIO_u8_DDRB_REG =
          (DIO_u8_PB0_INTIAL_DIRECTION << 0)
        | (DIO_u8_PB1_INTIAL_DIRECTION << 1)
        | (DIO_u8_PB2_INTIAL_DIRECTION << 2)
        | (DIO_u8_PB3_INTIAL_DIRECTION << 3)
        | (DIO_u8_PB4_INTIAL_DIRECTION << 4)
        | (DIO_u8_PB5_INTIAL_DIRECTION << 5)
        | (DIO_u8_PB6_INTIAL_DIRECTION << 6)
        | (DIO_u8_PB7_INTIAL_DIRECTION << 7);

    DIO_u8_PORTB_REG =
          (DIO_u8_PB0_INTIAL_VALUE << 0)
        | (DIO_u8_PB1_INTIAL_VALUE << 1)
        | (DIO_u8_PB2_INTIAL_VALUE << 2)
        | (DIO_u8_PB3_INTIAL_VALUE << 3)
        | (DIO_u8_PB4_INTIAL_VALUE << 4)
        | (DIO_u8_PB5_INTIAL_VALUE << 5)
        | (DIO_u8_PB6_INTIAL_VALUE << 6)
        | (DIO_u8_PB7_INTIAL_VALUE << 7);


    /* ===================================== */
    /* PORT C                                */
    /* ===================================== */

    DIO_u8_DDRC_REG =
          (DIO_u8_PC0_INTIAL_DIRECTION << 0)
        | (DIO_u8_PC1_INTIAL_DIRECTION << 1)
        | (DIO_u8_PC2_INTIAL_DIRECTION << 2)
        | (DIO_u8_PC3_INTIAL_DIRECTION << 3)
        | (DIO_u8_PC4_INTIAL_DIRECTION << 4)
        | (DIO_u8_PC5_INTIAL_DIRECTION << 5)
        | (DIO_u8_PC6_INTIAL_DIRECTION << 6)
        | (DIO_u8_PC7_INTIAL_DIRECTION << 7);

    DIO_u8_PORTC_REG =
          (DIO_u8_PC0_INTIAL_VALUE << 0)
        | (DIO_u8_PC1_INTIAL_VALUE << 1)
        | (DIO_u8_PC2_INTIAL_VALUE << 2)
        | (DIO_u8_PC3_INTIAL_VALUE << 3)
        | (DIO_u8_PC4_INTIAL_VALUE << 4)
        | (DIO_u8_PC5_INTIAL_VALUE << 5)
        | (DIO_u8_PC6_INTIAL_VALUE << 6)
        | (DIO_u8_PC7_INTIAL_VALUE << 7);


    /* ===================================== */
    /* PORT D                                */
    /* ===================================== */

    DIO_u8_DDRD_REG =
          (DIO_u8_PD0_INTIAL_DIRECTION << 0)
        | (DIO_u8_PD1_INTIAL_DIRECTION << 1)
        | (DIO_u8_PD2_INTIAL_DIRECTION << 2)
        | (DIO_u8_PD3_INTIAL_DIRECTION << 3)
        | (DIO_u8_PD4_INTIAL_DIRECTION << 4)
        | (DIO_u8_PD5_INTIAL_DIRECTION << 5)
        | (DIO_u8_PD6_INTIAL_DIRECTION << 6)
        | (DIO_u8_PD7_INTIAL_DIRECTION << 7);

    DIO_u8_PORTD_REG =
          (DIO_u8_PD0_INTIAL_VALUE << 0)
        | (DIO_u8_PD1_INTIAL_VALUE << 1)
        | (DIO_u8_PD2_INTIAL_VALUE << 2)
        | (DIO_u8_PD3_INTIAL_VALUE << 3)
        | (DIO_u8_PD4_INTIAL_VALUE << 4)
        | (DIO_u8_PD5_INTIAL_VALUE << 5)
        | (DIO_u8_PD6_INTIAL_VALUE << 6)
        | (DIO_u8_PD7_INTIAL_VALUE << 7);
}


/* ========================================= */
/* SET PIN DIRECTION                         */
/* ========================================= */

u8 DIO_u8SetPinDirection(
        u8 Copy_u8PortId,
        u8 Copy_u8PinId,
        u8 Copy_u8PinDirection)
{
    u8 Local_u8ErrorState = DIO_u8_OK;

    if (Copy_u8PinId > 7)
    {
        Local_u8ErrorState = DIO_u8_NOK;
    }
    else
    {
        switch (Copy_u8PortId)
        {
            case DIO_u8_PORTA:

                if (Copy_u8PinDirection == DIO_u8_OUTPUT)
                {
                    SET_BIT(DIO_u8_DDRA_REG, Copy_u8PinId);
                }
                else if (Copy_u8PinDirection == DIO_u8_INPUT)
                {
                    CLR_BIT(DIO_u8_DDRA_REG, Copy_u8PinId);
                }
                else
                {
                    Local_u8ErrorState = DIO_u8_NOK;
                }

                break;


            case DIO_u8_PORTB:

                if (Copy_u8PinDirection == DIO_u8_OUTPUT)
                {
                    SET_BIT(DIO_u8_DDRB_REG, Copy_u8PinId);
                }
                else if (Copy_u8PinDirection == DIO_u8_INPUT)
                {
                    CLR_BIT(DIO_u8_DDRB_REG, Copy_u8PinId);
                }
                else
                {
                    Local_u8ErrorState = DIO_u8_NOK;
                }

                break;


            case DIO_u8_PORTC:

                if (Copy_u8PinDirection == DIO_u8_OUTPUT)
                {
                    SET_BIT(DIO_u8_DDRC_REG, Copy_u8PinId);
                }
                else if (Copy_u8PinDirection == DIO_u8_INPUT)
                {
                    CLR_BIT(DIO_u8_DDRC_REG, Copy_u8PinId);
                }
                else
                {
                    Local_u8ErrorState = DIO_u8_NOK;
                }

                break;


            case DIO_u8_PORTD:

                if (Copy_u8PinDirection == DIO_u8_OUTPUT)
                {
                    SET_BIT(DIO_u8_DDRD_REG, Copy_u8PinId);
                }
                else if (Copy_u8PinDirection == DIO_u8_INPUT)
                {
                    CLR_BIT(DIO_u8_DDRD_REG, Copy_u8PinId);
                }
                else
                {
                    Local_u8ErrorState = DIO_u8_NOK;
                }

                break;


            default:

                Local_u8ErrorState = DIO_u8_NOK;

                break;
        }
    }

    return Local_u8ErrorState;
}


/* ========================================= */
/* SET PIN VALUE                             */
/* ========================================= */

u8 DIO_u8SetPinValue(
        u8 Copy_u8PortId,
        u8 Copy_u8PinId,
        u8 Copy_u8PinValue)
{
    u8 Local_u8ErrorState = DIO_u8_OK;

    if (Copy_u8PinId > 7)
    {
        Local_u8ErrorState = DIO_u8_NOK;
    }
    else
    {
        switch (Copy_u8PortId)
        {
            case DIO_u8_PORTA:

                if (Copy_u8PinValue == DIO_u8_HIGH)
                {
                    SET_BIT(DIO_u8_PORTA_REG, Copy_u8PinId);
                }
                else if (Copy_u8PinValue == DIO_u8_LOW)
                {
                    CLR_BIT(DIO_u8_PORTA_REG, Copy_u8PinId);
                }
                else
                {
                    Local_u8ErrorState = DIO_u8_NOK;
                }

                break;


            case DIO_u8_PORTB:

                if (Copy_u8PinValue == DIO_u8_HIGH)
                {
                    SET_BIT(DIO_u8_PORTB_REG, Copy_u8PinId);
                }
                else if (Copy_u8PinValue == DIO_u8_LOW)
                {
                    CLR_BIT(DIO_u8_PORTB_REG, Copy_u8PinId);
                }
                else
                {
                    Local_u8ErrorState = DIO_u8_NOK;
                }

                break;


            case DIO_u8_PORTC:

                if (Copy_u8PinValue == DIO_u8_HIGH)
                {
                    SET_BIT(DIO_u8_PORTC_REG, Copy_u8PinId);
                }
                else if (Copy_u8PinValue == DIO_u8_LOW)
                {
                    CLR_BIT(DIO_u8_PORTC_REG, Copy_u8PinId);
                }
                else
                {
                    Local_u8ErrorState = DIO_u8_NOK;
                }

                break;


            case DIO_u8_PORTD:

                if (Copy_u8PinValue == DIO_u8_HIGH)
                {
                    SET_BIT(DIO_u8_PORTD_REG, Copy_u8PinId);
                }
                else if (Copy_u8PinValue == DIO_u8_LOW)
                {
                    CLR_BIT(DIO_u8_PORTD_REG, Copy_u8PinId);
                }
                else
                {
                    Local_u8ErrorState = DIO_u8_NOK;
                }

                break;


            default:

                Local_u8ErrorState = DIO_u8_NOK;

                break;
        }
    }

    return Local_u8ErrorState;
}


/* ========================================= */
/* GET PIN VALUE                             */
/* ========================================= */

u8 DIO_u8GetPinValue(
        u8 Copy_u8PortId,
        u8 Copy_u8PinId,
        u8 *Copy_pu8ReturnedPinValue)
{
    u8 Local_u8ErrorState = DIO_u8_OK;

    if ((Copy_u8PinId > 7) || (Copy_pu8ReturnedPinValue == 0))
    {
        Local_u8ErrorState = DIO_u8_NOK;
    }
    else
    {
        switch (Copy_u8PortId)
        {
            case DIO_u8_PORTA:
                *Copy_pu8ReturnedPinValue =
                        GET_BIT(DIO_u8_PINA_REG, Copy_u8PinId);
                break;

            case DIO_u8_PORTB:
                *Copy_pu8ReturnedPinValue =
                        GET_BIT(DIO_u8_PINB_REG, Copy_u8PinId);
                break;

            case DIO_u8_PORTC:
                *Copy_pu8ReturnedPinValue =
                        GET_BIT(DIO_u8_PINC_REG, Copy_u8PinId);
                break;

            case DIO_u8_PORTD:
                *Copy_pu8ReturnedPinValue =
                        GET_BIT(DIO_u8_PIND_REG, Copy_u8PinId);
                break;

            default:
                Local_u8ErrorState = DIO_u8_NOK;
                break;
        }
    }

    return Local_u8ErrorState;
}


/* ========================================= */
/* SET PORT DIRECTION                        */
/* ========================================= */

u8 DIO_u8SetPortDirection(
        u8 Copy_u8PortId,
        u8 Copy_u8PortDirection)
{
    u8 Local_u8ErrorState = DIO_u8_OK;

    switch (Copy_u8PortId)
    {
        case DIO_u8_PORTA:

            if (Copy_u8PortDirection == DIO_u8_OUTPUT)
            {
                DIO_u8_DDRA_REG = 0xFF;
            }
            else if (Copy_u8PortDirection == DIO_u8_INPUT)
            {
                DIO_u8_DDRA_REG = 0x00;
            }
            else
            {
                Local_u8ErrorState = DIO_u8_NOK;
            }

            break;


        case DIO_u8_PORTB:

            if (Copy_u8PortDirection == DIO_u8_OUTPUT)
            {
                DIO_u8_DDRB_REG = 0xFF;
            }
            else if (Copy_u8PortDirection == DIO_u8_INPUT)
            {
                DIO_u8_DDRB_REG = 0x00;
            }
            else
            {
                Local_u8ErrorState = DIO_u8_NOK;
            }

            break;


        case DIO_u8_PORTC:

            if (Copy_u8PortDirection == DIO_u8_OUTPUT)
            {
                DIO_u8_DDRC_REG = 0xFF;
            }
            else if (Copy_u8PortDirection == DIO_u8_INPUT)
            {
                DIO_u8_DDRC_REG = 0x00;
            }
            else
            {
                Local_u8ErrorState = DIO_u8_NOK;
            }

            break;


        case DIO_u8_PORTD:

            if (Copy_u8PortDirection == DIO_u8_OUTPUT)
            {
                DIO_u8_DDRD_REG = 0xFF;
            }
            else if (Copy_u8PortDirection == DIO_u8_INPUT)
            {
                DIO_u8_DDRD_REG = 0x00;
            }
            else
            {
                Local_u8ErrorState = DIO_u8_NOK;
            }

            break;


        default:

            Local_u8ErrorState = DIO_u8_NOK;

            break;
    }

    return Local_u8ErrorState;
}


/* ========================================= */
/* SET PORT VALUE                            */
/* ========================================= */

u8 DIO_u8SetPortValue(
        u8 Copy_u8PortId,
        u8 Copy_u8PortValue)
{
    u8 Local_u8ErrorState = DIO_u8_OK;

    switch (Copy_u8PortId)
    {
        case DIO_u8_PORTA:

            DIO_u8_PORTA_REG = Copy_u8PortValue;
            break;

        case DIO_u8_PORTB:

            DIO_u8_PORTB_REG = Copy_u8PortValue;
            break;

        case DIO_u8_PORTC:

            DIO_u8_PORTC_REG = Copy_u8PortValue;
            break;

        case DIO_u8_PORTD:

            DIO_u8_PORTD_REG = Copy_u8PortValue;
            break;

        default:

            Local_u8ErrorState = DIO_u8_NOK;
            break;
    }

    return Local_u8ErrorState;
}


/* ========================================= */
/* GET PORT VALUE                            */
/* ========================================= */

u8 DIO_u8GetPortValue(
        u8 Copy_u8PortId,
        u8 *Copy_pu8ReturnedPortValue)
{
    u8 Local_u8ErrorState = DIO_u8_OK;

    if (Copy_pu8ReturnedPortValue == 0)
    {
        Local_u8ErrorState = DIO_u8_NOK;
    }
    else
    {
        switch (Copy_u8PortId)
        {
            case DIO_u8_PORTA:

                *Copy_pu8ReturnedPortValue = DIO_u8_PINA_REG;
                break;

            case DIO_u8_PORTB:

                *Copy_pu8ReturnedPortValue = DIO_u8_PINB_REG;
                break;

            case DIO_u8_PORTC:

                *Copy_pu8ReturnedPortValue = DIO_u8_PINC_REG;
                break;

            case DIO_u8_PORTD:

                *Copy_pu8ReturnedPortValue = DIO_u8_PIND_REG;
                break;

            default:

                Local_u8ErrorState = DIO_u8_NOK;
                break;
        }
    }

    return Local_u8ErrorState;
}
