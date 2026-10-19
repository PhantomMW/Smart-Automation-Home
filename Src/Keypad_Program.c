#include "../Include/Lib/STD_Types.h"
#include "../Include/Lib/BIT_Math.h"

#define F_CPU 8000000UL
#include <util/delay.h>

#include "../Include/MCAL/DIO/DIO_Interface.h"
#include "../Include/HAL/KEYPAD/Keypad_Interface.h"
#include "../Include/HAL/KEYPAD/Keypad_Cfg.h"


/* ========================================= */
/* Keypad Map                                */
/* ========================================= */

static u8 KPD_Au8Keys[4][4] =
{
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};


/* ========================================= */
/* Init                                       */
/* ========================================= */

void KEYPAD_voidInit(void)
{
    /* Rows OUTPUT */

    DIO_u8SetPinDirection(KEYPAD_u8_R1_PORT, KEYPAD_u8_R1_PIN, DIO_u8_OUTPUT);
    DIO_u8SetPinDirection(KEYPAD_u8_R2_PORT, KEYPAD_u8_R2_PIN, DIO_u8_OUTPUT);
    DIO_u8SetPinDirection(KEYPAD_u8_R3_PORT, KEYPAD_u8_R3_PIN, DIO_u8_OUTPUT);
    DIO_u8SetPinDirection(KEYPAD_u8_R4_PORT, KEYPAD_u8_R4_PIN, DIO_u8_OUTPUT);


    /* Columns INPUT */

    DIO_u8SetPinDirection(KEYPAD_u8_C1_PORT, KEYPAD_u8_C1_PIN, DIO_u8_INPUT);
    DIO_u8SetPinDirection(KEYPAD_u8_C2_PORT, KEYPAD_u8_C2_PIN, DIO_u8_INPUT);
    DIO_u8SetPinDirection(KEYPAD_u8_C3_PORT, KEYPAD_u8_C3_PIN, DIO_u8_INPUT);
    DIO_u8SetPinDirection(KEYPAD_u8_C4_PORT, KEYPAD_u8_C4_PIN, DIO_u8_INPUT);


    /* Pull-Up */

    DIO_u8SetPinValue(KEYPAD_u8_C1_PORT, KEYPAD_u8_C1_PIN, DIO_u8_HIGH);
    DIO_u8SetPinValue(KEYPAD_u8_C2_PORT, KEYPAD_u8_C2_PIN, DIO_u8_HIGH);
    DIO_u8SetPinValue(KEYPAD_u8_C3_PORT, KEYPAD_u8_C3_PIN, DIO_u8_HIGH);
    DIO_u8SetPinValue(KEYPAD_u8_C4_PORT, KEYPAD_u8_C4_PIN, DIO_u8_HIGH);


    /* Rows HIGH */

    DIO_u8SetPinValue(KEYPAD_u8_R1_PORT, KEYPAD_u8_R1_PIN, DIO_u8_HIGH);
    DIO_u8SetPinValue(KEYPAD_u8_R2_PORT, KEYPAD_u8_R2_PIN, DIO_u8_HIGH);
    DIO_u8SetPinValue(KEYPAD_u8_R3_PORT, KEYPAD_u8_R3_PIN, DIO_u8_HIGH);
    DIO_u8SetPinValue(KEYPAD_u8_R4_PORT, KEYPAD_u8_R4_PIN, DIO_u8_HIGH);
}


/* ========================================= */
/* Get Key                                   */
/* ========================================= */

u8 KEYPAD_u8GetKey(void)
{
    u8 Local_u8Row;
    u8 Local_u8Col;
    u8 Local_u8Value;


    for (Local_u8Row = 0; Local_u8Row < 4; Local_u8Row++)
    {
        /* All rows HIGH */

        DIO_u8SetPinValue(KEYPAD_u8_R1_PORT, KEYPAD_u8_R1_PIN, DIO_u8_HIGH);
        DIO_u8SetPinValue(KEYPAD_u8_R2_PORT, KEYPAD_u8_R2_PIN, DIO_u8_HIGH);
        DIO_u8SetPinValue(KEYPAD_u8_R3_PORT, KEYPAD_u8_R3_PIN, DIO_u8_HIGH);
        DIO_u8SetPinValue(KEYPAD_u8_R4_PORT, KEYPAD_u8_R4_PIN, DIO_u8_HIGH);


        /* Current Row LOW */

        if (Local_u8Row == 0)
        {
            DIO_u8SetPinValue(KEYPAD_u8_R1_PORT, KEYPAD_u8_R1_PIN, DIO_u8_LOW);
        }
        else if (Local_u8Row == 1)
        {
            DIO_u8SetPinValue(KEYPAD_u8_R2_PORT, KEYPAD_u8_R2_PIN, DIO_u8_LOW);
        }
        else if (Local_u8Row == 2)
        {
            DIO_u8SetPinValue(KEYPAD_u8_R3_PORT, KEYPAD_u8_R3_PIN, DIO_u8_LOW);
        }
        else
        {
            DIO_u8SetPinValue(KEYPAD_u8_R4_PORT, KEYPAD_u8_R4_PIN, DIO_u8_LOW);
        }


        /* Check Columns */

        for (Local_u8Col = 0; Local_u8Col < 4; Local_u8Col++)
        {
            Local_u8Value = DIO_u8_HIGH;


            if (Local_u8Col == 0)
            {
                DIO_u8GetPinValue(
                    KEYPAD_u8_C1_PORT,
                    KEYPAD_u8_C1_PIN,
                    &Local_u8Value
                );
            }
            else if (Local_u8Col == 1)
            {
                DIO_u8GetPinValue(
                    KEYPAD_u8_C2_PORT,
                    KEYPAD_u8_C2_PIN,
                    &Local_u8Value
                );
            }
            else if (Local_u8Col == 2)
            {
                DIO_u8GetPinValue(
                    KEYPAD_u8_C3_PORT,
                    KEYPAD_u8_C3_PIN,
                    &Local_u8Value
                );
            }
            else
            {
                DIO_u8GetPinValue(
                    KEYPAD_u8_C4_PORT,
                    KEYPAD_u8_C4_PIN,
                    &Local_u8Value
                );
            }


            if (Local_u8Value == DIO_u8_LOW)
            {
                _delay_ms(30);

                /* Wait release */

                do
                {
                    if (Local_u8Col == 0)
                    {
                        DIO_u8GetPinValue(
                            KEYPAD_u8_C1_PORT,
                            KEYPAD_u8_C1_PIN,
                            &Local_u8Value
                        );
                    }
                    else if (Local_u8Col == 1)
                    {
                        DIO_u8GetPinValue(
                            KEYPAD_u8_C2_PORT,
                            KEYPAD_u8_C2_PIN,
                            &Local_u8Value
                        );
                    }
                    else if (Local_u8Col == 2)
                    {
                        DIO_u8GetPinValue(
                            KEYPAD_u8_C3_PORT,
                            KEYPAD_u8_C3_PIN,
                            &Local_u8Value
                        );
                    }
                    else
                    {
                        DIO_u8GetPinValue(
                            KEYPAD_u8_C4_PORT,
                            KEYPAD_u8_C4_PIN,
                            &Local_u8Value
                        );
                    }

                } while (Local_u8Value == DIO_u8_LOW);


                return KPD_Au8Keys[Local_u8Row][Local_u8Col];
            }
        }
    }


    return KEYPAD_NO_KEY;
}
