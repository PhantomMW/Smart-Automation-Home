#include "../Include/Lib/STD_Types.h"
#include "../Include/Lib/BIT_Math.h"

#define F_CPU 8000000UL
#include <util/delay.h>

#include "../Include/MCAL/DIO/DIO_Interface.h"

#include "../Include/MCAL/EEPROM/EEPROM_Interface.h"
#include "../Include/MCAL/EEPROM/EEPROM_Private.h"


/* ========================================= */
/* SOFTWARE I2C LOW LEVEL FUNCTIONS          */
/* ========================================= */

static void EEPROM_voidSCLHigh(void)
{
    DIO_u8SetPinDirection(
        EEPROM_SCL_PORT,
        EEPROM_SCL_PIN,
        DIO_u8_OUTPUT
    );

    DIO_u8SetPinValue(
        EEPROM_SCL_PORT,
        EEPROM_SCL_PIN,
        DIO_u8_HIGH
    );

    _delay_us(EEPROM_I2C_DELAY_US);
}


static void EEPROM_voidSCLLow(void)
{
    DIO_u8SetPinValue(
        EEPROM_SCL_PORT,
        EEPROM_SCL_PIN,
        DIO_u8_LOW
    );

    _delay_us(EEPROM_I2C_DELAY_US);
}


static void EEPROM_voidSDALow(void)
{
    DIO_u8SetPinDirection(
        EEPROM_SDA_PORT,
        EEPROM_SDA_PIN,
        DIO_u8_OUTPUT
    );

    DIO_u8SetPinValue(
        EEPROM_SDA_PORT,
        EEPROM_SDA_PIN,
        DIO_u8_LOW
    );

    _delay_us(EEPROM_I2C_DELAY_US);
}


static void EEPROM_voidSDARelease(void)
{
    /*
     * Release SDA line.
     *
     * External 4.7K pull-up resistor
     * will make SDA HIGH.
     */

    DIO_u8SetPinDirection(
        EEPROM_SDA_PORT,
        EEPROM_SDA_PIN,
        DIO_u8_INPUT
    );

    _delay_us(EEPROM_I2C_DELAY_US);
}


static u8 EEPROM_u8ReadSDA(void)
{
    u8 Local_u8Value;

    DIO_u8GetPinValue(
        EEPROM_SDA_PORT,
        EEPROM_SDA_PIN,
        &Local_u8Value
    );

    return Local_u8Value;
}


/* ========================================= */
/* START CONDITION                           */
/* ========================================= */

static void EEPROM_voidStartCondition(void)
{
    EEPROM_voidSDARelease();

    EEPROM_voidSCLHigh();

    EEPROM_voidSDALow();

    EEPROM_voidSCLLow();
}


/* ========================================= */
/* STOP CONDITION                            */
/* ========================================= */

static void EEPROM_voidStopCondition(void)
{
    EEPROM_voidSDALow();

    EEPROM_voidSCLHigh();

    EEPROM_voidSDARelease();
}


/* ========================================= */
/* SEND BYTE                                  */
/* ========================================= */

static u8 EEPROM_u8SendByte(u8 Copy_u8Data)
{
    u8 Local_u8Counter;
    u8 Local_u8ACK;


    for (
        Local_u8Counter = 0;
        Local_u8Counter < 8;
        Local_u8Counter++
    )
    {
        if (Copy_u8Data & 0x80)
        {
            EEPROM_voidSDARelease();
        }
        else
        {
            EEPROM_voidSDALow();
        }

        EEPROM_voidSCLHigh();

        EEPROM_voidSCLLow();

        Copy_u8Data <<= 1;
    }


    /*
     * Release SDA to receive ACK
     */

    EEPROM_voidSDARelease();

    EEPROM_voidSCLHigh();

    Local_u8ACK = EEPROM_u8ReadSDA();

    EEPROM_voidSCLLow();


    /*
     * ACK = LOW
     */

    if (Local_u8ACK == DIO_u8_LOW)
    {
        return STD_TYPES_OK;
    }

    return STD_TYPES_NOK;
}


/* ========================================= */
/* RECEIVE BYTE                               */
/* ========================================= */

static u8 EEPROM_u8ReceiveByte(
    u8 *Copy_pu8Data
)
{
    u8 Local_u8Counter;
    u8 Local_u8Data = 0;


    EEPROM_voidSDARelease();


    for (
        Local_u8Counter = 0;
        Local_u8Counter < 8;
        Local_u8Counter++
    )
    {
        Local_u8Data <<= 1;

        EEPROM_voidSCLHigh();

        if (
            EEPROM_u8ReadSDA()
            ==
            DIO_u8_HIGH
        )
        {
            Local_u8Data |= 1;
        }

        EEPROM_voidSCLLow();
    }


    /*
     * Send NACK after last byte
     */

    EEPROM_voidSDARelease();

    EEPROM_voidSCLHigh();

    EEPROM_voidSCLLow();


    *Copy_pu8Data = Local_u8Data;

    return STD_TYPES_OK;
}


/* ========================================= */
/* EEPROM INIT                                */
/* ========================================= */

void EEPROM_voidInit(void)
{
    /*
     * SCL idle HIGH
     */

    DIO_u8SetPinDirection(
        EEPROM_SCL_PORT,
        EEPROM_SCL_PIN,
        DIO_u8_OUTPUT
    );

    DIO_u8SetPinValue(
        EEPROM_SCL_PORT,
        EEPROM_SCL_PIN,
        DIO_u8_HIGH
    );


    /*
     * SDA released
     */

    EEPROM_voidSDARelease();
}


/* ========================================= */
/* EEPROM WRITE BYTE                          */
/* ========================================= */

u8 EEPROM_u8WriteByte(
    u8 Copy_u8Address,
    u8 Copy_u8Data
)
{
    u8 Local_u8Status;


    EEPROM_voidStartCondition();


    /*
     * Device address
     *
     * 1010 + A2 A1 A0 + Write
     */

    Local_u8Status =
        EEPROM_u8SendByte(
            (EEPROM_FIXED_ADDRESS << 1)
            |
            0
        );


    if (Local_u8Status != STD_TYPES_OK)
    {
        EEPROM_voidStopCondition();

        return STD_TYPES_NOK;
    }


    /*
     * Memory address
     */

    Local_u8Status =
        EEPROM_u8SendByte(
            Copy_u8Address
        );


    if (Local_u8Status != STD_TYPES_OK)
    {
        EEPROM_voidStopCondition();

        return STD_TYPES_NOK;
    }


    /*
     * Data
     */

    Local_u8Status =
        EEPROM_u8SendByte(
            Copy_u8Data
        );


    EEPROM_voidStopCondition();


    /*
     * EEPROM write cycle
     */

    _delay_ms(10);


    return Local_u8Status;
}


/* ========================================= */
/* EEPROM READ BYTE                           */
/* ========================================= */

u8 EEPROM_u8ReadByte(
    u8 Copy_u8Address,
    u8 *Copy_pu8Data
)
{
    u8 Local_u8Status;


    /*
     * Start
     */

    EEPROM_voidStartCondition();


    /*
     * Device address + WRITE
     */

    Local_u8Status =
        EEPROM_u8SendByte(
            (EEPROM_FIXED_ADDRESS << 1)
            |
            0
        );


    if (Local_u8Status != STD_TYPES_OK)
    {
        EEPROM_voidStopCondition();

        return STD_TYPES_NOK;
    }


    /*
     * Memory address
     */

    Local_u8Status =
        EEPROM_u8SendByte(
            Copy_u8Address
        );


    if (Local_u8Status != STD_TYPES_OK)
    {
        EEPROM_voidStopCondition();

        return STD_TYPES_NOK;
    }


    /*
     * Repeated START
     */

    EEPROM_voidStartCondition();


    /*
     * Device address + READ
     */

    Local_u8Status =
        EEPROM_u8SendByte(
            (EEPROM_FIXED_ADDRESS << 1)
            |
            1
        );


    if (Local_u8Status != STD_TYPES_OK)
    {
        EEPROM_voidStopCondition();

        return STD_TYPES_NOK;
    }


    /*
     * Receive data
     */

    Local_u8Status =
        EEPROM_u8ReceiveByte(
            Copy_pu8Data
        );


    EEPROM_voidStopCondition();


    return Local_u8Status;
}
