	/*
	 * LCD_Program.c
	 *
	 * Created on: Jul 12, 2026
	 * Author: hp
	 */

	#include "../Include/Lib/STD_Types.h"
	#include "../Include/Lib/BIT_Math.h"

	#define F_CPU 8000000UL
	#include <util/delay.h>

	#include "../Include/MCAL/DIO/DIO_Interface.h"

	#include "../Include/HAL/LCD/LCD_Interface.h"
	#include "../Include/HAL/LCD/LCD_Private.h"
	#include "../Include/HAL/LCD/LCD_Cfg.h"


	/* ========================================================= */
	/* LCD Initialization                                        */
	/* ========================================================= */

	void LCD_voidInit(void)
	{
		/* Power ON delay */
		_delay_ms(50);

		/* 8-bit mode, 2 lines, 5x8 font */
		LCD_voidSendCmnd(0x38);
		_delay_ms(5);

		/* Display ON, Cursor OFF, Blink OFF */
		LCD_voidSendCmnd(0x0C);
		_delay_ms(2);

		/* Clear display */
		LCD_voidSendCmnd(0x01);
		_delay_ms(3);

		/* Entry mode: cursor moves right */
		LCD_voidSendCmnd(0x06);
		_delay_ms(2);
	}


	/* ========================================================= */
	/* Send Command                                              */
	/* ========================================================= */

	void LCD_voidSendCmnd(u8 Copy_u8Cmnd)
	{
		/* RS = 0 -> Command */
		DIO_u8SetPinValue(
			LCD_u8_CONTROL_PORT,
			LCD_u8_RS_PIN,
			DIO_u8_LOW
		);

		/* RW = 0 -> Write */
		DIO_u8SetPinValue(
			LCD_u8_CONTROL_PORT,
			LCD_u8_RW_PIN,
			DIO_u8_LOW
		);

		/* Put command on D0-D7 */
		DIO_u8SetPortValue(
			LCD_u8_DATA_PORT,
			Copy_u8Cmnd
		);

		/* Enable pulse */
		DIO_u8SetPinValue(
			LCD_u8_CONTROL_PORT,
			LCD_u8_E_PIN,
			DIO_u8_HIGH
		);

		_delay_us(2);

		DIO_u8SetPinValue(
			LCD_u8_CONTROL_PORT,
			LCD_u8_E_PIN,
			DIO_u8_LOW
		);

		/* Give LCD time to execute command */
		_delay_us(50);
	}


	/* ========================================================= */
	/* Send Character                                            */
	/* ========================================================= */

	void LCD_voidSendChar(u8 Copy_u8Char)
	{
		/* RS = 1 -> Data */
		DIO_u8SetPinValue(
			LCD_u8_CONTROL_PORT,
			LCD_u8_RS_PIN,
			DIO_u8_HIGH
		);

		/* RW = 0 -> Write */
		DIO_u8SetPinValue(
			LCD_u8_CONTROL_PORT,
			LCD_u8_RW_PIN,
			DIO_u8_LOW
		);

		/* Put character on D0-D7 */
		DIO_u8SetPortValue(
			LCD_u8_DATA_PORT,
			Copy_u8Char
		);

		/* Enable pulse */
		DIO_u8SetPinValue(
			LCD_u8_CONTROL_PORT,
			LCD_u8_E_PIN,
			DIO_u8_HIGH
		);

		_delay_us(2);

		DIO_u8SetPinValue(
			LCD_u8_CONTROL_PORT,
			LCD_u8_E_PIN,
			DIO_u8_LOW
		);

		/* Character execution time */
		_delay_us(50);
	}


	/* ========================================================= */
	/* Go To XY                                                  */
	/* ========================================================= */

	u8 LCD_u8GoToXY(
		u8 Copy_u8LineNum,
		u8 Copy_u8Location
	)
	{
		u8 Local_u8ErrorState = STD_TYPES_OK;

		if (Copy_u8Location < 40)
		{
			switch (Copy_u8LineNum)
			{
				case LCD_u8_LINE1:

					LCD_voidSendCmnd(
						0x80 + Copy_u8Location
					);

					break;


				case LCD_u8_LINE2:

					LCD_voidSendCmnd(
						0xC0 + Copy_u8Location
					);

					break;


				default:

					Local_u8ErrorState = STD_TYPES_NOK;

					break;
			}
		}
		else
		{
			Local_u8ErrorState = STD_TYPES_NOK;
		}

		return Local_u8ErrorState;
	}


	/* ========================================================= */
	/* Clear Display                                             */
	/* ========================================================= */

	void LCD_voidClearDisplay(void)
	{
		LCD_voidSendCmnd(0x01);

		_delay_ms(2);
	}


	/* ========================================================= */
	/* Send String                                               */
	/* ========================================================= */

	void LCD_voidSendString(u8 *A_u8PtrToString)
	{
		if (A_u8PtrToString != NULL)
		{
			while (*A_u8PtrToString != '\0')
			{
				LCD_voidSendChar(*A_u8PtrToString);

				A_u8PtrToString++;
			}
		}
	}


	/* ========================================================= */
	/* Display Number                                            */
	/* ========================================================= */

	void LCD_voidDisplayNumber(s32 A_s32Number)
	{
		u32 Local_u32Number = 0;
		u8 Local_u8Digit;


		/* Zero */
		if (A_s32Number == 0)
		{
			LCD_voidSendChar('0');
			return;
		}


		/* Negative */
		if (A_s32Number < 0)
		{
			LCD_voidSendChar('-');

			A_s32Number = -A_s32Number;
		}


		/* Reverse number */
		while (A_s32Number != 0)
		{
			Local_u32Number =
				(Local_u32Number * 10)
				+ (A_s32Number % 10);

			A_s32Number /= 10;
		}


		/* Display digits */
		while (Local_u32Number != 0)
		{
			Local_u8Digit =
				Local_u32Number % 10;

			LCD_voidSendChar(
				Local_u8Digit + '0'
			);

			Local_u32Number /= 10;
		}
	}
