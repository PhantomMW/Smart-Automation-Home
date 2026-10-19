
#include "../Include/Lib/STD_Types.h"
#include "../Include/Lib/BIT_Math.h"

#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#include "FreeRTOS.h"
#include "task.h"

#include "../Include/MCAL/DIO/DIO_Interface.h"
#include "../Include/MCAL/UART/UART_Interface.h"
#include "../Include/MCAL/TIMER/TIMER_Interface.h"
#include "../Include/MCAL/EEPROM/EEPROM_Interface.h"

#include "../Include/HAL/LCD/LCD_Interface.h"
#include "../Include/HAL/BLUETOOTH/BLUETOOTH_Interface.h"
#include "../Include/HAL/KEYPAD/KEYPAD_Interface.h"
#include "../Include/HAL/DHT11/DHT11_Interface.h"


/* ================================================= */
/* PWM                                               */
/* ================================================= */

#define TEST_DISABLE_TIMER0_PWM     0


/* ================================================= */
/* PIN CONFIGURATION                                 */
/* ================================================= */

#define MOTOR_PORT      DIO_u8_PORTA
#define MOTOR_PIN       DIO_u8_PIN0

#define BUZZER_PORT     DIO_u8_PORTA
#define BUZZER_PIN      DIO_u8_PIN2

#define LED_PORT        DIO_u8_PORTA
#define LED_PIN         DIO_u8_PIN6

#define SERVO_PORT      DIO_u8_PORTA
#define SERVO_PIN       DIO_u8_PIN7


/* ================================================= */
/* EEPROM PASSWORD                                   */
/* ================================================= */

#define EEPROM_PASSWORD_ADDRESS     0

#define FORCE_RESET_PASSWORD        0


/* ================================================= */
/* PASSWORD CHANGE                                  */
/* ================================================= */

#define CHANGE_PASSWORD_TRIGGER     3


/* ================================================= */
/* FAN SPEED                                         */
/* ================================================= */

#define FAN_SPEED_25                25
#define FAN_SPEED_50                50
#define FAN_SPEED_75                75
#define FAN_SPEED_100               100


/* ================================================= */
/* GLOBAL PASSWORD                                   */
/* ================================================= */

static u8 Password[4];
static u8 EnteredPassword[4];
static u8 NewPassword[4];


/* ================================================= */
/* GLOBAL DHT DATA                                   */
/* ================================================= */

volatile u8 g_Temperature = 0;
volatile u8 g_Humidity = 0;

volatile u8 g_DHT11_Status = DHT11_ERROR;


/*
 * 0 = automatic temperature control OFF
 * 1 = automatic temperature control ON
 */
volatile u8 g_TemperatureControlEnabled = 0;

volatile u8 g_FanSpeed = FAN_SPEED_25;

volatile u8 g_MotorState = 0;


/*
 * 0 = password screen
 * 1 = user logged in
 */
volatile u8 g_AccessGranted = 0;


/* ================================================= */
/* SERVO                                             */
/* ================================================= */

void Servo_voidSetPosition(u8 Copy_u8Position)
{
    u8 Local_u8Counter;

    for (
        Local_u8Counter = 0;
        Local_u8Counter < 50;
        Local_u8Counter++
    )
    {
        DIO_u8SetPinValue(
            SERVO_PORT,
            SERVO_PIN,
            DIO_u8_HIGH
        );

        if (Copy_u8Position == 0)
        {
            _delay_us(1000);
        }
        else
        {
            _delay_us(2000);
        }

        DIO_u8SetPinValue(
            SERVO_PORT,
            SERVO_PIN,
            DIO_u8_LOW
        );

        _delay_ms(18);
    }
}


/* ================================================= */
/* PASSWORD SCREEN                                   */
/* ================================================= */

void DisplayPasswordScreen(void)
{
    LCD_voidClearDisplay();

    LCD_u8GoToXY(
        LCD_u8_LINE1,
        0
    );

    LCD_voidSendString(
        (u8*)"ENTER PASSWORD"
    );

    LCD_u8GoToXY(
        LCD_u8_LINE2,
        0
    );
}


/* ================================================= */
/* CHANGE PASSWORD SCREEN                            */
/* ================================================= */

void DisplayChangePasswordScreen(void)
{
    LCD_voidClearDisplay();

    LCD_u8GoToXY(
        LCD_u8_LINE1,
        0
    );

    LCD_voidSendString(
        (u8*)"NEW PASSWORD:"
    );

    LCD_u8GoToXY(
        LCD_u8_LINE2,
        0
    );
}


/* ================================================= */
/* PASSWORD CHANGED SCREEN                           */
/* ================================================= */

void DisplayPasswordChangedScreen(void)
{
    LCD_voidClearDisplay();

    LCD_u8GoToXY(
        LCD_u8_LINE1,
        0
    );

    LCD_voidSendString(
        (u8*)"PASSWORD"
    );

    LCD_u8GoToXY(
        LCD_u8_LINE2,
        0
    );

    LCD_voidSendString(
        (u8*)"CHANGED"
    );

    _delay_ms(1200);
}


/* ================================================= */
/* HOME SCREEN                                       */
/* ================================================= */

void DisplayHomeScreen(void)
{
    LCD_voidClearDisplay();

    LCD_u8GoToXY(
        LCD_u8_LINE1,
        0
    );

    LCD_voidSendString(
        (u8*)"A:DOOR B:LIGHT"
    );

    LCD_u8GoToXY(
        LCD_u8_LINE2,
        0
    );

    LCD_voidSendString(
        (u8*)"C:MOTOR D:SPEED"
    );
}


/* ================================================= */
/* DHT11 DISPLAY                                     */
/* ================================================= */

void DisplayDHT11Values(
    u8 Copy_u8Temperature,
    u8 Copy_u8Humidity
)
{
    LCD_voidClearDisplay();

    LCD_u8GoToXY(
        LCD_u8_LINE1,
        0
    );

    LCD_voidSendString(
        (u8*)"TEMP:"
    );

    if (Copy_u8Temperature >= 100)
    {
        LCD_voidSendChar(
            (Copy_u8Temperature / 100) + '0'
        );

        LCD_voidSendChar(
            ((Copy_u8Temperature / 10) % 10) + '0'
        );

        LCD_voidSendChar(
            (Copy_u8Temperature % 10) + '0'
        );
    }
    else if (Copy_u8Temperature >= 10)
    {
        LCD_voidSendChar(
            (Copy_u8Temperature / 10) + '0'
        );

        LCD_voidSendChar(
            (Copy_u8Temperature % 10) + '0'
        );
    }
    else
    {
        LCD_voidSendChar(
            Copy_u8Temperature + '0'
        );
    }

    LCD_voidSendString(
        (u8*)" C"
    );


    LCD_u8GoToXY(
        LCD_u8_LINE2,
        0
    );

    LCD_voidSendString(
        (u8*)"HUM:"
    );

    if (Copy_u8Humidity >= 100)
    {
        LCD_voidSendChar(
            (Copy_u8Humidity / 100) + '0'
        );

        LCD_voidSendChar(
            ((Copy_u8Humidity / 10) % 10) + '0'
        );

        LCD_voidSendChar(
            (Copy_u8Humidity % 10) + '0'
        );
    }
    else if (Copy_u8Humidity >= 10)
    {
        LCD_voidSendChar(
            (Copy_u8Humidity / 10) + '0'
        );

        LCD_voidSendChar(
            (Copy_u8Humidity % 10) + '0'
        );
    }
    else
    {
        LCD_voidSendChar(
            Copy_u8Humidity + '0'
        );
    }

    LCD_voidSendString(
        (u8*)" %"
    );
}


/* ================================================= */
/* FAN SPEED DISPLAY                                 */
/* ================================================= */

void DisplayFanSpeed(u8 Copy_u8Speed)
{
    LCD_voidClearDisplay();

    LCD_u8GoToXY(
        LCD_u8_LINE1,
        0
    );

    LCD_voidSendString(
        (u8*)"FAN SPEED:"
    );

    if (Copy_u8Speed == FAN_SPEED_25)
    {
        LCD_voidSendString(
            (u8*)"25%"
        );
    }
    else if (Copy_u8Speed == FAN_SPEED_50)
    {
        LCD_voidSendString(
            (u8*)"50%"
        );
    }
    else if (Copy_u8Speed == FAN_SPEED_75)
    {
        LCD_voidSendString(
            (u8*)"75%"
        );
    }
    else
    {
        LCD_voidSendString(
            (u8*)"100%"
        );
    }

    _delay_ms(500);
}


/* ================================================= */
/* TEMPERATURE -> FAN SPEED                          */
/* ================================================= */

u8 GetFanSpeedFromTemperature(
    u8 Copy_u8Temperature
)
{
    if (Copy_u8Temperature < 30)
    {
        return 0;
    }
    else if (Copy_u8Temperature <= 31)
    {
        return FAN_SPEED_25;
    }
    else if (Copy_u8Temperature <= 33)
    {
        return FAN_SPEED_50;
    }
    else if (Copy_u8Temperature <= 35)
    {
        return FAN_SPEED_75;
    }
    else
    {
        return FAN_SPEED_100;
    }
}


/* ================================================= */
/* LOAD PASSWORD                                     */
/* ================================================= */

void LoadPasswordFromEEPROM(
    u8 *Copy_pu8Password
)
{
    u8 Local_u8Index;

#if FORCE_RESET_PASSWORD == 1

    Copy_pu8Password[0] = '1';
    Copy_pu8Password[1] = '2';
    Copy_pu8Password[2] = '3';
    Copy_pu8Password[3] = '4';

    for (
        Local_u8Index = 0;
        Local_u8Index < 4;
        Local_u8Index++
    )
    {
        EEPROM_u8WriteByte(
            EEPROM_PASSWORD_ADDRESS + Local_u8Index,
            Copy_pu8Password[Local_u8Index]
        );

        _delay_ms(15);
    }

#else

    {
        u8 Local_u8Data;
        u8 Local_u8Status;
        u8 Local_u8Valid = 1;

        for (
            Local_u8Index = 0;
            Local_u8Index < 4;
            Local_u8Index++
        )
        {
            Local_u8Data = 0xFF;

            Local_u8Status =
                EEPROM_u8ReadByte(
                    EEPROM_PASSWORD_ADDRESS + Local_u8Index,
                    &Local_u8Data
                );

            if (Local_u8Status != STD_TYPES_OK)
            {
                Local_u8Valid = 0;
            }

            if (
                Local_u8Data < '0' ||
                Local_u8Data > '9'
            )
            {
                Local_u8Valid = 0;
            }

            Copy_pu8Password[Local_u8Index] =
                Local_u8Data;
        }

        if (Local_u8Valid == 0)
        {
            Copy_pu8Password[0] = '1';
            Copy_pu8Password[1] = '2';
            Copy_pu8Password[2] = '3';
            Copy_pu8Password[3] = '4';

            for (
                Local_u8Index = 0;
                Local_u8Index < 4;
                Local_u8Index++
            )
            {
                EEPROM_u8WriteByte(
                    EEPROM_PASSWORD_ADDRESS + Local_u8Index,
                    Copy_pu8Password[Local_u8Index]
                );

                _delay_ms(15);
            }
        }
    }

#endif
}


/* ================================================= */
/* SAVE PASSWORD                                     */
/* ================================================= */

void SavePasswordToEEPROM(
    u8 *Copy_pu8Password
)
{
    u8 Local_u8Index;

    for (
        Local_u8Index = 0;
        Local_u8Index < 4;
        Local_u8Index++
    )
    {
        EEPROM_u8WriteByte(
            EEPROM_PASSWORD_ADDRESS + Local_u8Index,
            Copy_pu8Password[Local_u8Index]
        );

        _delay_ms(15);
    }
}


/* ================================================= */
/* MOTOR PWM                                         */
/* ================================================= */

void MotorOn(u8 Copy_u8Speed)
{
#if TEST_DISABLE_TIMER0_PWM == 0

    TIMER0_voidSetDutyCycle(
        Copy_u8Speed
    );

    TIMER0_voidStartPWM();

#else

    DIO_u8SetPinValue(
        MOTOR_PORT,
        MOTOR_PIN,
        DIO_u8_HIGH
    );

#endif
}


/* ================================================= */
/* MOTOR OFF                                         */
/* ================================================= */

void MotorOff(void)
{
#if TEST_DISABLE_TIMER0_PWM == 0

    TIMER0_voidStopPWM();

#else

    DIO_u8SetPinValue(
        MOTOR_PORT,
        MOTOR_PIN,
        DIO_u8_LOW
    );

#endif
}


/* ================================================= */
/* STACK OVERFLOW HOOK                               */
/* ================================================= */

void vApplicationStackOverflowHook(
    TaskHandle_t xTask,
    char *pcTaskName
)
{
    (void)xTask;
    (void)pcTaskName;

    LCD_voidClearDisplay();

    LCD_u8GoToXY(
        LCD_u8_LINE1,
        0
    );

    LCD_voidSendString(
        (u8*)"STACK OVERFLOW"
    );

    LCD_u8GoToXY(
        LCD_u8_LINE2,
        0
    );

    LCD_voidSendString(
        (u8*)"TASK"
    );

    while (1)
    {
    }
}


/* ================================================= */
/* DHT TASK                                          */
/* ================================================= */

void DHTTask(void *pvParameters)
{
    u8 Local_u8Temperature = 0;
    u8 Local_u8Humidity = 0;
    u8 Local_u8Status;
    u8 Local_u8FanSpeed;

    (void)pvParameters;

    while (1)
    {
        cli();

        Local_u8Status =
            DHT11_Read(
                &Local_u8Temperature,
                &Local_u8Humidity
            );

        sei();


        /* ========================================= */
        /* DHT SUCCESS                               */
        /* ========================================= */

        if (Local_u8Status == DHT11_OK)
        {
            g_Temperature =
                Local_u8Temperature;

            g_Humidity =
                Local_u8Humidity;

            g_DHT11_Status =
                DHT11_OK;


            /* ===================================== */
            /* DISPLAY TEMPERATURE                   */
            /* ===================================== */

            if (g_AccessGranted == 1)
            {
                DisplayDHT11Values(
                    Local_u8Temperature,
                    Local_u8Humidity
                );
            }


            /* ===================================== */
            /* AUTOMATIC FAN CONTROL                 */
            /* ===================================== */

            if (g_TemperatureControlEnabled == 1)
            {
                Local_u8FanSpeed =
                    GetFanSpeedFromTemperature(
                        Local_u8Temperature
                    );

                g_FanSpeed =
                    Local_u8FanSpeed;


                if (Local_u8FanSpeed == 0)
                {
                    g_MotorState = 0;

                    MotorOff();
                }
                else
                {
                    g_MotorState = 1;

                    MotorOn(
                        Local_u8FanSpeed
                    );
                }
            }
        }


        /* ========================================= */
        /* DHT ERROR                                 */
        /* ========================================= */

        else
        {
            g_DHT11_Status =
                DHT11_ERROR;

            if (
                g_AccessGranted == 1 &&
                g_TemperatureControlEnabled == 1
            )
            {
                g_MotorState = 0;

                MotorOff();

                LCD_voidClearDisplay();

                LCD_u8GoToXY(
                    LCD_u8_LINE1,
                    0
                );

                LCD_voidSendString(
                    (u8*)"DHT11 ERROR"
                );

                LCD_u8GoToXY(
                    LCD_u8_LINE2,
                    0
                );

                LCD_voidSendString(
                    (u8*)"CHECK SENSOR"
                );
            }
        }


        /* ========================================= */
        /* DHT11 READING INTERVAL                   */
        /* ========================================= */

        vTaskDelay(
            pdMS_TO_TICKS(2000)
        );
    }
}


/* ================================================= */
/* MAIN TASK                                         */
/* ================================================= */

void MainTask(void *pvParameters)
{
    u8 PasswordIndex = 0;
    u8 NewPasswordIndex = 0;
    u8 WrongAttempts = 0;

    u8 AccessGranted = 0;
    u8 ChangePasswordMode = 0;
    u8 ZeroCounter = 0;

    u8 Local_u8Match;
    u8 Local_u8Index;
    u8 Local_u8Key;
    u8 BluetoothData;

    u8 DoorOpen = 0;
    u8 LightState = 0;

    u8 FanSpeed = FAN_SPEED_25;

    (void)pvParameters;


    /* ============================================= */
    /* INITIAL SCREEN                                */
    /* ============================================= */

    DisplayPasswordScreen();


    /* ============================================= */
    /* MAIN LOOP                                     */
    /* ============================================= */

    while (1)
    {
        /* ========================================= */
        /* BLUETOOTH                                  */
        /* ========================================= */

        if (
            UART_u8ReceiveByteNonBlocking(
                &BluetoothData
            ) == STD_TYPES_OK
        )
        {
            /* ===================================== */
            /* CHANGE PASSWORD                       */
            /* ===================================== */

            if (ChangePasswordMode == 1)
            {
                if (
                    BluetoothData >= '0' &&
                    BluetoothData <= '9'
                )
                {
                    if (NewPasswordIndex < 4)
                    {
                        NewPassword[
                            NewPasswordIndex
                        ] = BluetoothData;

                        LCD_voidSendChar('*');

                        NewPasswordIndex++;
                    }

                    if (NewPasswordIndex == 4)
                    {
                        SavePasswordToEEPROM(
                            NewPassword
                        );

                        for (
                            Local_u8Index = 0;
                            Local_u8Index < 4;
                            Local_u8Index++
                        )
                        {
                            Password[
                                Local_u8Index
                            ] =
                                NewPassword[
                                    Local_u8Index
                                ];
                        }

                        ChangePasswordMode = 0;
                        NewPasswordIndex = 0;
                        ZeroCounter = 0;

                        DisplayPasswordChangedScreen();

                        DisplayHomeScreen();
                    }
                }
            }


            /* ===================================== */
            /* PASSWORD MODE                         */
            /* ===================================== */

            else if (AccessGranted == 0)
            {
                if (
                    BluetoothData >= '0' &&
                    BluetoothData <= '9'
                )
                {
                    if (PasswordIndex < 4)
                    {
                        EnteredPassword[
                            PasswordIndex
                        ] = BluetoothData;

                        LCD_voidSendChar('*');

                        PasswordIndex++;
                    }

                    if (PasswordIndex == 4)
                    {
                        Local_u8Match = 1;

                        for (
                            Local_u8Index = 0;
                            Local_u8Index < 4;
                            Local_u8Index++
                        )
                        {
                            if (
                                EnteredPassword[
                                    Local_u8Index
                                ] !=
                                Password[
                                    Local_u8Index
                                ]
                            )
                            {
                                Local_u8Match = 0;
                            }
                        }


                        /* ========================= */
                        /* CORRECT PASSWORD            */
                        /* ========================= */

                        if (Local_u8Match == 1)
                        {
                            WrongAttempts = 0;

                            AccessGranted = 1;

                            g_AccessGranted = 1;

                            g_TemperatureControlEnabled = 0;

                            g_MotorState = 0;

                            MotorOff();


                            LCD_voidClearDisplay();

                            LCD_u8GoToXY(
                                LCD_u8_LINE1,
                                0
                            );

                            LCD_voidSendString(
                                (u8*)"ACCESS GRANTED"
                            );

                            LCD_u8GoToXY(
                                LCD_u8_LINE2,
                                0
                            );

                            LCD_voidSendString(
                                (u8*)"WELCOME"
                            );

                            _delay_ms(1500);


                            DoorOpen = 1;

                            Servo_voidSetPosition(1);

                            PasswordIndex = 0;
                        }


                        /* ========================= */
                        /* WRONG PASSWORD              */
                        /* ========================= */

                        else
                        {
                            WrongAttempts++;


                            /* ===================== */
                            /* 3 WRONG ATTEMPTS       */
                            /* ===================== */

                            if (WrongAttempts >= 3)
                            {
                                LCD_voidClearDisplay();

                                LCD_u8GoToXY(
                                    LCD_u8_LINE1,
                                    0
                                );

                                LCD_voidSendString(
                                    (u8*)"ALARM!"
                                );

                                LCD_u8GoToXY(
                                    LCD_u8_LINE2,
                                    0
                                );

                                LCD_voidSendString(
                                    (u8*)"3 WRONG TRIES"
                                );


                                /*
                                 * BUZZER IS USED ONLY HERE
                                 */

                                DIO_u8SetPinValue(
                                    BUZZER_PORT,
                                    BUZZER_PIN,
                                    DIO_u8_HIGH
                                );

                                _delay_ms(3000);

                                DIO_u8SetPinValue(
                                    BUZZER_PORT,
                                    BUZZER_PIN,
                                    DIO_u8_LOW
                                );

                                WrongAttempts = 0;
                            }


                            /* ===================== */
                            /* NORMAL WRONG PASSWORD  */
                            /* ===================== */

                            else
                            {
                                /*
                                 * BUZZER IS USED ONLY
                                 * FOR WRONG PASSWORD
                                 */

                                DIO_u8SetPinValue(
                                    BUZZER_PORT,
                                    BUZZER_PIN,
                                    DIO_u8_HIGH
                                );

                                _delay_ms(200);

                                DIO_u8SetPinValue(
                                    BUZZER_PORT,
                                    BUZZER_PIN,
                                    DIO_u8_LOW
                                );


                                LCD_voidClearDisplay();

                                LCD_u8GoToXY(
                                    LCD_u8_LINE1,
                                    0
                                );

                                LCD_voidSendString(
                                    (u8*)"WRONG PASSWORD"
                                );

                                LCD_u8GoToXY(
                                    LCD_u8_LINE2,
                                    0
                                );

                                LCD_voidSendString(
                                    (u8*)"TRY AGAIN"
                                );

                                _delay_ms(1500);
                            }


                            PasswordIndex = 0;

                            DisplayPasswordScreen();
                        }
                    }
                }
            }


            /* ===================================== */
            /* HOME                                  */
            /* ===================================== */

            else
            {
                /* ================================= */
                /* * = TEMPERATURE CONTROL            */
                /* ================================= */

                if (BluetoothData == '*')
                {
                    if (g_TemperatureControlEnabled == 0)
                    {
                        g_TemperatureControlEnabled = 1;

                        LCD_voidClearDisplay();

                        LCD_u8GoToXY(
                            LCD_u8_LINE1,
                            0
                        );

                        LCD_voidSendString(
                            (u8*)"AUTO CONTROL ON"
                        );

                        LCD_u8GoToXY(
                            LCD_u8_LINE2,
                            0
                        );

                        LCD_voidSendString(
                            (u8*)"TEMP CONTROL"
                        );

                        _delay_ms(700);
                    }
                    else
                    {
                        g_TemperatureControlEnabled = 0;

                        g_MotorState = 0;

                        MotorOff();

                        LCD_voidClearDisplay();

                        LCD_u8GoToXY(
                            LCD_u8_LINE1,
                            0
                        );

                        LCD_voidSendString(
                            (u8*)"AUTO CONTROL"
                        );

                        LCD_u8GoToXY(
                            LCD_u8_LINE2,
                            0
                        );

                        LCD_voidSendString(
                            (u8*)"OFF"
                        );

                        _delay_ms(700);
                    }
                }


                /* ================================= */
                /* 1 = MOTOR ON                      */
                /* ================================= */

                else if (BluetoothData == '1')
                {
                    g_TemperatureControlEnabled = 0;

                    g_MotorState = 1;

                    MotorOn(FanSpeed);

                    LCD_voidClearDisplay();

                    LCD_u8GoToXY(
                        LCD_u8_LINE1,
                        0
                    );

                    LCD_voidSendString(
                        (u8*)"MOTOR: ON"
                    );
                }


                /* ================================= */
                /* 2 = MOTOR OFF                     */
                /* ================================= */

                else if (BluetoothData == '2')
                {
                    g_TemperatureControlEnabled = 0;

                    g_MotorState = 0;

                    MotorOff();

                    LCD_voidClearDisplay();

                    LCD_u8GoToXY(
                        LCD_u8_LINE1,
                        0
                    );

                    LCD_voidSendString(
                        (u8*)"MOTOR: OFF"
                    );
                }


                /* ================================= */
                /* 3 = LIGHT ON                      */
                /* ================================= */

                else if (BluetoothData == '3')
                {
                    LightState = 1;

                    DIO_u8SetPinValue(
                        LED_PORT,
                        LED_PIN,
                        DIO_u8_HIGH
                    );

                    LCD_voidClearDisplay();

                    LCD_u8GoToXY(
                        LCD_u8_LINE1,
                        0
                    );

                    LCD_voidSendString(
                        (u8*)"LIGHT: ON"
                    );
                }


                /* ================================= */
                /* 4 = LIGHT OFF                     */
                /* ================================= */

                else if (BluetoothData == '4')
                {
                    LightState = 0;

                    DIO_u8SetPinValue(
                        LED_PORT,
                        LED_PIN,
                        DIO_u8_LOW
                    );

                    LCD_voidClearDisplay();

                    LCD_u8GoToXY(
                        LCD_u8_LINE1,
                        0
                    );

                    LCD_voidSendString(
                        (u8*)"LIGHT: OFF"
                    );
                }


                /* ================================= */
                /* 5 = DOOR OPEN                     */
                /* ================================= */

                else if (BluetoothData == '5')
                {
                    DoorOpen = 1;

                    Servo_voidSetPosition(1);

                    LCD_voidClearDisplay();

                    LCD_u8GoToXY(
                        LCD_u8_LINE1,
                        0
                    );

                    LCD_voidSendString(
                        (u8*)"DOOR: OPEN"
                    );
                }


                /* ================================= */
                /* 6 = DOOR CLOSED                   */
                /* ================================= */

                else if (BluetoothData == '6')
                {
                    DoorOpen = 0;

                    Servo_voidSetPosition(0);

                    LCD_voidClearDisplay();

                    LCD_u8GoToXY(
                        LCD_u8_LINE1,
                        0
                    );

                    LCD_voidSendString(
                        (u8*)"DOOR: CLOSED"
                    );
                }


                /* ================================= */
                /* 9 = FAN SPEED                     */
                /* ================================= */

                else if (BluetoothData == '9')
                {
                    g_TemperatureControlEnabled = 0;

                    if (FanSpeed == FAN_SPEED_25)
                    {
                        FanSpeed = FAN_SPEED_50;
                    }
                    else if (FanSpeed == FAN_SPEED_50)
                    {
                        FanSpeed = FAN_SPEED_75;
                    }
                    else if (FanSpeed == FAN_SPEED_75)
                    {
                        FanSpeed = FAN_SPEED_100;
                    }
                    else
                    {
                        FanSpeed = FAN_SPEED_25;
                    }

                    g_FanSpeed = FanSpeed;

                    if (g_MotorState == 1)
                    {
                        MotorOn(FanSpeed);
                    }

                    DisplayFanSpeed(FanSpeed);
                }


                /* ================================= */
                /* # = LOCK                          */
                /* ================================= */

                else if (BluetoothData == '#')
                {
                    AccessGranted = 0;

                    g_AccessGranted = 0;

                    PasswordIndex = 0;

                    g_TemperatureControlEnabled = 0;

                    g_MotorState = 0;

                    MotorOff();

                    DisplayPasswordScreen();
                }
            }
        }


        /* ========================================= */
        /* KEYPAD                                    */
        /* ========================================= */

        Local_u8Key = KEYPAD_u8GetKey();

        if (Local_u8Key != KEYPAD_NO_KEY)
        {
            /* ===================================== */
            /* CHANGE PASSWORD                       */
            /* ===================================== */

            if (ChangePasswordMode == 1)
            {
                if (
                    Local_u8Key >= '0' &&
                    Local_u8Key <= '9'
                )
                {
                    if (NewPasswordIndex < 4)
                    {
                        NewPassword[
                            NewPasswordIndex
                        ] = Local_u8Key;

                        LCD_voidSendChar('*');

                        NewPasswordIndex++;
                    }

                    if (NewPasswordIndex == 4)
                    {
                        SavePasswordToEEPROM(
                            NewPassword
                        );

                        for (
                            Local_u8Index = 0;
                            Local_u8Index < 4;
                            Local_u8Index++
                        )
                        {
                            Password[
                                Local_u8Index
                            ] =
                                NewPassword[
                                    Local_u8Index
                                ];
                        }

                        ChangePasswordMode = 0;

                        NewPasswordIndex = 0;

                        ZeroCounter = 0;

                        DisplayPasswordChangedScreen();

                        DisplayHomeScreen();
                    }
                }
            }


            /* ===================================== */
            /* PASSWORD MODE                         */
            /* ===================================== */

            else if (AccessGranted == 0)
            {
                if (
                    Local_u8Key >= '0' &&
                    Local_u8Key <= '9'
                )
                {
                    if (PasswordIndex < 4)
                    {
                        EnteredPassword[
                            PasswordIndex
                        ] = Local_u8Key;

                        LCD_voidSendChar('*');

                        PasswordIndex++;
                    }

                    if (PasswordIndex == 4)
                    {
                        Local_u8Match = 1;

                        for (
                            Local_u8Index = 0;
                            Local_u8Index < 4;
                            Local_u8Index++
                        )
                        {
                            if (
                                EnteredPassword[
                                    Local_u8Index
                                ] !=
                                Password[
                                    Local_u8Index
                                ]
                            )
                            {
                                Local_u8Match = 0;
                            }
                        }


                        /* ========================= */
                        /* CORRECT PASSWORD            */
                        /* ========================= */

                        if (Local_u8Match == 1)
                        {
                            WrongAttempts = 0;

                            AccessGranted = 1;

                            g_AccessGranted = 1;

                            g_TemperatureControlEnabled = 0;

                            g_MotorState = 0;

                            MotorOff();

                            LCD_voidClearDisplay();

                            LCD_u8GoToXY(
                                LCD_u8_LINE1,
                                0
                            );

                            LCD_voidSendString(
                                (u8*)"ACCESS GRANTED"
                            );

                            LCD_u8GoToXY(
                                LCD_u8_LINE2,
                                0
                            );

                            LCD_voidSendString(
                                (u8*)"WELCOME"
                            );

                            _delay_ms(1500);

                            DoorOpen = 1;

                            Servo_voidSetPosition(1);

                            PasswordIndex = 0;
                        }


                        /* ========================= */
                        /* WRONG PASSWORD              */
                        /* ========================= */

                        else
                        {
                            WrongAttempts++;


                            /* ===================== */
                            /* 3 WRONG ATTEMPTS       */
                            /* ===================== */

                            if (WrongAttempts >= 3)
                            {
                                LCD_voidClearDisplay();

                                LCD_u8GoToXY(
                                    LCD_u8_LINE1,
                                    0
                                );

                                LCD_voidSendString(
                                    (u8*)"ALARM!"
                                );

                                LCD_u8GoToXY(
                                    LCD_u8_LINE2,
                                    0
                                );

                                LCD_voidSendString(
                                    (u8*)"3 WRONG TRIES"
                                );


                                /*
                                 * BUZZER ONLY FOR ALARM
                                 */

                                DIO_u8SetPinValue(
                                    BUZZER_PORT,
                                    BUZZER_PIN,
                                    DIO_u8_HIGH
                                );

                                _delay_ms(3000);

                                DIO_u8SetPinValue(
                                    BUZZER_PORT,
                                    BUZZER_PIN,
                                    DIO_u8_LOW
                                );

                                WrongAttempts = 0;
                            }


                            /* ===================== */
                            /* NORMAL WRONG PASSWORD  */
                            /* ===================== */

                            else
                            {
                                /*
                                 * BUZZER ONLY FOR
                                 * WRONG PASSWORD
                                 */

                                DIO_u8SetPinValue(
                                    BUZZER_PORT,
                                    BUZZER_PIN,
                                    DIO_u8_HIGH
                                );

                                _delay_ms(200);

                                DIO_u8SetPinValue(
                                    BUZZER_PORT,
                                    BUZZER_PIN,
                                    DIO_u8_LOW
                                );

                                LCD_voidClearDisplay();

                                LCD_u8GoToXY(
                                    LCD_u8_LINE1,
                                    0
                                );

                                LCD_voidSendString(
                                    (u8*)"WRONG PASSWORD"
                                );

                                LCD_u8GoToXY(
                                    LCD_u8_LINE2,
                                    0
                                );

                                LCD_voidSendString(
                                    (u8*)"TRY AGAIN"
                                );

                                _delay_ms(1500);
                            }

                            PasswordIndex = 0;

                            DisplayPasswordScreen();
                        }
                    }
                }
            }


            /* ===================================== */
            /* HOME                                  */
            /* ===================================== */

            else
            {
                /* ================================= */
                /* 0 = CHANGE PASSWORD                */
                /* ================================= */

                if (Local_u8Key == '0')
                {
                    ZeroCounter++;

                    if (
                        ZeroCounter >=
                        CHANGE_PASSWORD_TRIGGER
                    )
                    {
                        ChangePasswordMode = 1;

                        NewPasswordIndex = 0;

                        ZeroCounter = 0;

                        g_TemperatureControlEnabled = 0;

                        g_MotorState = 0;

                        MotorOff();

                        DisplayChangePasswordScreen();
                    }
                }

                else
                {
                    ZeroCounter = 0;


                    /* ============================= */
                    /* * = TEMPERATURE CONTROL       */
                    /* ============================= */

                    if (Local_u8Key == '*')
                    {
                        if (
                            g_TemperatureControlEnabled == 0
                        )
                        {
                            g_TemperatureControlEnabled = 1;

                            LCD_voidClearDisplay();

                            LCD_u8GoToXY(
                                LCD_u8_LINE1,
                                0
                            );

                            LCD_voidSendString(
                                (u8*)"AUTO CONTROL ON"
                            );

                            LCD_u8GoToXY(
                                LCD_u8_LINE2,
                                0
                            );

                            LCD_voidSendString(
                                (u8*)"TEMP CONTROL"
                            );

                            _delay_ms(700);
                        }
                        else
                        {
                            g_TemperatureControlEnabled = 0;

                            g_MotorState = 0;

                            MotorOff();

                            LCD_voidClearDisplay();

                            LCD_u8GoToXY(
                                LCD_u8_LINE1,
                                0
                            );

                            LCD_voidSendString(
                                (u8*)"AUTO CONTROL"
                            );

                            LCD_u8GoToXY(
                                LCD_u8_LINE2,
                                0
                            );

                            LCD_voidSendString(
                                (u8*)"OFF"
                            );

                            _delay_ms(700);
                        }
                    }


                    /* ============================= */
                    /* A = DOOR                      */
                    /* ============================= */

                    else if (Local_u8Key == 'A')
                    {
                        if (DoorOpen == 0)
                        {
                            DoorOpen = 1;

                            Servo_voidSetPosition(1);

                            LCD_voidClearDisplay();

                            LCD_u8GoToXY(
                                LCD_u8_LINE1,
                                0
                            );

                            LCD_voidSendString(
                                (u8*)"DOOR: OPEN"
                            );
                        }
                        else
                        {
                            DoorOpen = 0;

                            Servo_voidSetPosition(0);

                            LCD_voidClearDisplay();

                            LCD_u8GoToXY(
                                LCD_u8_LINE1,
                                0
                            );

                            LCD_voidSendString(
                                (u8*)"DOOR: CLOSED"
                            );
                        }

                        _delay_ms(700);
                    }


                    /* ============================= */
                    /* B = LIGHT                     */
                    /* ============================= */

                    else if (Local_u8Key == 'B')
                    {
                        if (LightState == 0)
                        {
                            LightState = 1;

                            DIO_u8SetPinValue(
                                LED_PORT,
                                LED_PIN,
                                DIO_u8_HIGH
                            );

                            LCD_voidClearDisplay();

                            LCD_u8GoToXY(
                                LCD_u8_LINE1,
                                0
                            );

                            LCD_voidSendString(
                                (u8*)"LIGHT: ON"
                            );
                        }
                        else
                        {
                            LightState = 0;

                            DIO_u8SetPinValue(
                                LED_PORT,
                                LED_PIN,
                                DIO_u8_LOW
                            );

                            LCD_voidClearDisplay();

                            LCD_u8GoToXY(
                                LCD_u8_LINE1,
                                0
                            );

                            LCD_voidSendString(
                                (u8*)"LIGHT: OFF"
                            );
                        }

                        _delay_ms(700);
                    }


                    /* ============================= */
                    /* C = MOTOR                     */
                    /* ============================= */

                    else if (Local_u8Key == 'C')
                    {
                        /*
                         * Manual motor control
                         * disables automatic control.
                         */

                        g_TemperatureControlEnabled = 0;

                        if (g_MotorState == 0)
                        {
                            g_MotorState = 1;

                            MotorOn(FanSpeed);

                            LCD_voidClearDisplay();

                            LCD_u8GoToXY(
                                LCD_u8_LINE1,
                                0
                            );

                            LCD_voidSendString(
                                (u8*)"MOTOR: ON"
                            );
                        }
                        else
                        {
                            g_MotorState = 0;

                            MotorOff();

                            LCD_voidClearDisplay();

                            LCD_u8GoToXY(
                                LCD_u8_LINE1,
                                0
                            );

                            LCD_voidSendString(
                                (u8*)"MOTOR: OFF"
                            );
                        }

                        _delay_ms(700);
                    }


                    /* ============================= */
                    /* D = FAN SPEED                  */
                    /* ============================= */

                    else if (Local_u8Key == 'D')
                    {
                        g_TemperatureControlEnabled = 0;

                        if (FanSpeed == FAN_SPEED_25)
                        {
                            FanSpeed = FAN_SPEED_50;
                        }
                        else if (FanSpeed == FAN_SPEED_50)
                        {
                            FanSpeed = FAN_SPEED_75;
                        }
                        else if (FanSpeed == FAN_SPEED_75)
                        {
                            FanSpeed = FAN_SPEED_100;
                        }
                        else
                        {
                            FanSpeed = FAN_SPEED_25;
                        }

                        g_FanSpeed = FanSpeed;

                        if (g_MotorState == 1)
                        {
                            MotorOn(FanSpeed);
                        }

                        DisplayFanSpeed(FanSpeed);
                    }


                    /* ============================= */
                    /* # = LOCK                       */
                    /* ============================= */

                    else if (Local_u8Key == '#')
                    {
                        AccessGranted = 0;

                        g_AccessGranted = 0;

                        PasswordIndex = 0;

                        g_TemperatureControlEnabled = 0;

                        g_MotorState = 0;

                        MotorOff();

                        DisplayPasswordScreen();
                    }
                }
            }
        }


        /* ========================================= */
        /* SMALL RTOS DELAY                          */
        /* ========================================= */

        vTaskDelay(
            pdMS_TO_TICKS(1)
        );
    }
}


/* ================================================= */
/* MAIN                                              */
/* ================================================= */

int main(void)
{
    BaseType_t Local_xTaskStatus;
    BaseType_t Local_xDHTTaskStatus;


    /* ============================================= */
    /* HARDWARE INITIALIZATION                       */
    /* ============================================= */

    DIO_voidInit();

    LCD_voidInit();


    /* ============================================= */
    /* STARTUP SCREEN                                */
    /* ============================================= */

    LCD_voidClearDisplay();

    LCD_u8GoToXY(
        LCD_u8_LINE1,
        0
    );

    LCD_voidSendString(
        (u8*)"STARTING..."
    );

    _delay_ms(500);


    /* ============================================= */
    /* KEYPAD                                        */
    /* ============================================= */

    KEYPAD_voidInit();


    /* ============================================= */
    /* BLUETOOTH                                     */
    /* ============================================= */

    BLUETOOTH_voidInit();


    /* ============================================= */
    /* DHT11                                         */
    /* ============================================= */

    DHT11_Init();


    /* ============================================= */
    /* EEPROM                                        */
    /* ============================================= */

    EEPROM_voidInit();


    /* ============================================= */
    /* PASSWORD                                      */
    /* ============================================= */

    LoadPasswordFromEEPROM(
        Password
    );


    /* ============================================= */
    /* MOTOR PIN                                     */
    /* ============================================= */

    DIO_u8SetPinDirection(
        MOTOR_PORT,
        MOTOR_PIN,
        DIO_u8_OUTPUT
    );

    DIO_u8SetPinValue(
        MOTOR_PORT,
        MOTOR_PIN,
        DIO_u8_LOW
    );


    /* ============================================= */
    /* TIMER0 SOFTWARE PWM                           */
    /* ============================================= */

#if TEST_DISABLE_TIMER0_PWM == 0

    TIMER0_voidSoftwarePWMInit();

    TIMER0_voidSetDutyCycle(
        FAN_SPEED_25
    );

    TIMER0_voidStopPWM();

#endif


    /* ============================================= */
    /* BUZZER                                        */
    /* ============================================= */

    DIO_u8SetPinDirection(
        BUZZER_PORT,
        BUZZER_PIN,
        DIO_u8_OUTPUT
    );

    DIO_u8SetPinValue(
        BUZZER_PORT,
        BUZZER_PIN,
        DIO_u8_LOW
    );


    /* ============================================= */
    /* LED                                           */
    /* ============================================= */

    DIO_u8SetPinDirection(
        LED_PORT,
        LED_PIN,
        DIO_u8_OUTPUT
    );

    DIO_u8SetPinValue(
        LED_PORT,
        LED_PIN,
        DIO_u8_LOW
    );


    /* ============================================= */
    /* SERVO                                         */
    /* ============================================= */

    DIO_u8SetPinDirection(
        SERVO_PORT,
        SERVO_PIN,
        DIO_u8_OUTPUT
    );

    Servo_voidSetPosition(0);


    /* ============================================= */
    /* INITIAL GLOBAL STATES                         */
    /* ============================================= */

    g_AccessGranted = 0;

    g_TemperatureControlEnabled = 0;

    g_MotorState = 0;

    MotorOff();


    /* ============================================= */
    /* MAIN TASK                                     */
    /* ============================================= */

    Local_xTaskStatus =
        xTaskCreate(
            MainTask,
            "MainTask",
            220,
            NULL,
            2,
            NULL
        );


    /* ============================================= */
    /* DHT TASK                                      */
    /* ============================================= */

    Local_xDHTTaskStatus =
        xTaskCreate(
            DHTTask,
            "DHTTask",
            180,
            NULL,
            1,
            NULL
        );


    /* ============================================= */
    /* TASK CREATION CHECK                           */
    /* ============================================= */

    if (
        Local_xTaskStatus != pdPASS ||
        Local_xDHTTaskStatus != pdPASS
    )
    {
        LCD_voidClearDisplay();

        LCD_u8GoToXY(
            LCD_u8_LINE1,
            0
        );

        LCD_voidSendString(
            (u8*)"TASK FAILED"
        );

        LCD_u8GoToXY(
            LCD_u8_LINE2,
            0
        );

        LCD_voidSendString(
            (u8*)"LOW MEMORY"
        );

        while (1)
        {
        }
    }


    /* ============================================= */
    /* START FREERTOS                                */
    /* ============================================= */

    vTaskStartScheduler();


    /* ============================================= */
    /* SHOULD NEVER REACH HERE                      */
    /* ============================================= */

    LCD_voidClearDisplay();

    LCD_u8GoToXY(
        LCD_u8_LINE1,
        0
    );

    LCD_voidSendString(
        (u8*)"SCHEDULER ERR"
    );

    while (1)
    {
    }

    return 0;
}

