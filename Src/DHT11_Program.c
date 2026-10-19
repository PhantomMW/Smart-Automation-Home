#include "../Include/Lib/STD_Types.h"
#include "../Include/Lib/BIT_Math.h"

#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../Include/MCAL/DIO/DIO_Interface.h"

#include "../Include/HAL/DHT11/DHT11_Interface.h"
#include "../Include/HAL/DHT11/DHT11_Cfg.h"
#include "../Include/HAL/DHT11/DHT11_Private.h"


static void DHT11_SetOutput(void)
{
    DIO_u8SetPinDirection(
        DHT11_PORT,
        DHT11_PIN,
        DIO_u8_OUTPUT
    );
}


static void DHT11_SetInput(void)
{
    DIO_u8SetPinDirection(
        DHT11_PORT,
        DHT11_PIN,
        DIO_u8_INPUT
    );
}


static void DHT11_Write(u8 Value)
{
    DIO_u8SetPinValue(
        DHT11_PORT,
        DHT11_PIN,
        Value
    );
}


static u8 DHT11_ReadPin(void)
{
    u8 Value = DIO_u8_LOW;

    DIO_u8GetPinValue(
        DHT11_PORT,
        DHT11_PIN,
        &Value
    );

    return Value;
}


/* ================================================= */
/* INIT                                              */
/* ================================================= */

void DHT11_Init(void)
{
    DHT11_SetOutput();

    DHT11_Write(
        DIO_u8_HIGH
    );
}


/* ================================================= */
/* WAIT FOR LEVEL WITH TIMEOUT                       */
/* ================================================= */

static u8 DHT11_WaitForLevel(
    u8 Copy_u8Level,
    u16 Copy_u16Timeout
)
{
    u16 Local_u16Counter = 0;

    while (DHT11_ReadPin() != Copy_u8Level)
    {
        _delay_us(1);

        Local_u16Counter++;

        if (Local_u16Counter >= Copy_u16Timeout)
        {
            return DHT11_ERROR;
        }
    }

    return DHT11_OK;
}


/* ================================================= */
/* READ ONE BIT                                      */
/* ================================================= */

static u8 DHT11_ReadBit(
    u8 *Copy_pu8Bit
)
{
    u16 Local_u16Timeout = 0;

    /* Wait for LOW period to finish */
    while (DHT11_ReadPin() == DIO_u8_LOW)
    {
        _delay_us(1);

        Local_u16Timeout++;

        if (Local_u16Timeout > 100)
        {
            return DHT11_ERROR;
        }
    }

    /*
     * At this point DATA became HIGH.
     *
     * DHT11:
     * 26-28 us HIGH = 0
     * around 70 us HIGH = 1
     *
     * Sampling at 40 us is enough.
     */

    _delay_us(40);

    if (DHT11_ReadPin() == DIO_u8_HIGH)
    {
        *Copy_pu8Bit = 1;
    }
    else
    {
        *Copy_pu8Bit = 0;
    }

    /* Wait until HIGH period ends */
    Local_u16Timeout = 0;

    while (DHT11_ReadPin() == DIO_u8_HIGH)
    {
        _delay_us(1);

        Local_u16Timeout++;

        if (Local_u16Timeout > 100)
        {
            return DHT11_ERROR;
        }
    }

    return DHT11_OK;
}


/* ================================================= */
/* READ DHT11                                        */
/* ================================================= */

u8 DHT11_Read(
    u8 *temperature,
    u8 *humidity
)
{
    u8 Data[5] = {0};

    u8 i;
    u8 j;
    u8 Local_u8Bit;


    if (
        temperature == NULL ||
        humidity == NULL
    )
    {
        return DHT11_ERROR;
    }


    /* ============================================= */
    /* START SIGNAL                                  */
    /* ============================================= */

    DHT11_SetOutput();

    DHT11_Write(
        DIO_u8_LOW
    );

    /*
     * DHT11 requires at least 18 ms LOW
     */
    _delay_ms(20);

    DHT11_Write(
        DIO_u8_HIGH
    );

    _delay_us(30);

    /*
     * Release DATA line
     */
    DHT11_SetInput();


    /* ============================================= */
    /* SENSOR RESPONSE                               */
    /* ============================================= */

    /*
     * Sensor pulls LOW ~80 us
     */
    if (
        DHT11_WaitForLevel(
            DIO_u8_LOW,
            200
        ) == DHT11_ERROR
    )
    {
        return DHT11_ERROR;
    }


    /*
     * Sensor pulls HIGH ~80 us
     */
    if (
        DHT11_WaitForLevel(
            DIO_u8_HIGH,
            200
        ) == DHT11_ERROR
    )
    {
        return DHT11_ERROR;
    }


    /*
     * Sensor returns LOW
     */
    if (
        DHT11_WaitForLevel(
            DIO_u8_LOW,
            200
        ) == DHT11_ERROR
    )
    {
        return DHT11_ERROR;
    }


    /* ============================================= */
    /* READ 40 BITS                                  */
    /* ============================================= */

    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (
                DHT11_ReadBit(
                    &Local_u8Bit
                ) == DHT11_ERROR
            )
            {
                return DHT11_ERROR;
            }

            if (Local_u8Bit == 1)
            {
                Data[i] |=
                    (1 << (7 - j));
            }
        }
    }


    /* ============================================= */
    /* CHECKSUM                                      */
    /* ============================================= */

    if (
        (u8)(
            Data[0] +
            Data[1] +
            Data[2] +
            Data[3]
        )
        !=
        Data[4]
    )
    {
        return DHT11_ERROR;
    }


    *humidity =
        Data[0];

    *temperature =
        Data[2];


    return DHT11_OK;
}
