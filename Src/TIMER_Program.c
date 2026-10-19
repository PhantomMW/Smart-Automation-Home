#include "../Include/Lib/STD_Types.h"
#include "../Include/Lib/BIT_Math.h"

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

#include "../Include/MCAL/DIO/DIO_Interface.h"
#include "../Include/MCAL/TIMER/TIMER_Interface.h"
#include "../Include/MCAL/TIMER/TIMER_Private.h"
#include "../Include/MCAL/TIMER/TIMER_Cfg.h"


static volatile u8 TIMER0_u8PWMCounter = 0;
static volatile u8 TIMER0_u8DutyCycle = 0;
static volatile u8 TIMER0_u8PWMEnabled = 0;


void TIMER0_voidSoftwarePWMInit(void)
{
    /* PA0 = PWM output */

    DIO_u8SetPinDirection(
        TIMER0_PWM_PORT,
        TIMER0_PWM_PIN,
        DIO_u8_OUTPUT
    );

    DIO_u8SetPinValue(
        TIMER0_PWM_PORT,
        TIMER0_PWM_PIN,
        DIO_u8_LOW
    );


    /*
     * TIMER0
     * CTC MODE
     *
     * F_CPU = 8 MHz
     * Prescaler = 64
     * OCR0 = 124
     *
     * Interrupt every 1 ms
     */

    TIMER0_TCCR0_REG = 0;

    CLR_BIT(
        TIMER0_TCCR0_REG,
        TIMER0_WGM00
    );

    SET_BIT(
        TIMER0_TCCR0_REG,
        TIMER0_WGM01
    );


    TIMER0_OCR0_REG = 124;


    /*
     * Enable Timer0 Compare Match interrupt
     */

    SET_BIT(
        TIMER0_TIMSK_REG,
        TIMER0_OCIE0
    );


    /*
     * Prescaler = 64
     */

    CLR_BIT(
        TIMER0_TCCR0_REG,
        TIMER0_CS02
    );

    SET_BIT(
        TIMER0_TCCR0_REG,
        TIMER0_CS01
    );

    SET_BIT(
        TIMER0_TCCR0_REG,
        TIMER0_CS00
    );


    TIMER0_u8PWMCounter = 0;
    TIMER0_u8DutyCycle = 0;
    TIMER0_u8PWMEnabled = 0;


    sei();
}


/* ========================================= */
/* SET DUTY                                  */
/* ========================================= */

void TIMER0_voidSetDutyCycle(u8 Copy_u8Duty)
{
    if (Copy_u8Duty > 100)
    {
        Copy_u8Duty = 100;
    }


    TIMER0_u8DutyCycle =
        (u8)(
            ((u16)Copy_u8Duty * TIMER0_PWM_STEPS)
            / 100
        );


    TIMER0_u8PWMCounter = 0;
}


/* ========================================= */
/* START PWM                                 */
/* ========================================= */

void TIMER0_voidStartPWM(void)
{
    TIMER0_u8PWMEnabled = 1;
    TIMER0_u8PWMCounter = 0;
}


/* ========================================= */
/* STOP PWM                                  */
/* ========================================= */

void TIMER0_voidStopPWM(void)
{
    TIMER0_u8PWMEnabled = 0;
    TIMER0_u8PWMCounter = 0;

    DIO_u8SetPinValue(
        TIMER0_PWM_PORT,
        TIMER0_PWM_PIN,
        DIO_u8_LOW
    );
}


/* ========================================= */
/* TIMER0 COMPARE ISR                        */
/* ========================================= */

ISR(TIMER0_COMP_vect)
{
    if (TIMER0_u8PWMEnabled == 0)
    {
        DIO_u8SetPinValue(
            TIMER0_PWM_PORT,
            TIMER0_PWM_PIN,
            DIO_u8_LOW
        );

        TIMER0_u8PWMCounter = 0;

        return;
    }


    if (TIMER0_u8PWMCounter >= TIMER0_PWM_STEPS)
    {
        TIMER0_u8PWMCounter = 0;
    }


    if (
        TIMER0_u8PWMCounter <
        TIMER0_u8DutyCycle
    )
    {
        DIO_u8SetPinValue(
            TIMER0_PWM_PORT,
            TIMER0_PWM_PIN,
            DIO_u8_HIGH
        );
    }
    else
    {
        DIO_u8SetPinValue(
            TIMER0_PWM_PORT,
            TIMER0_PWM_PIN,
            DIO_u8_LOW
        );
    }


    TIMER0_u8PWMCounter++;
}
