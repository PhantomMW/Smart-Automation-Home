	#ifndef MCAL_UART_UART_INTERFACE_H_
	#define MCAL_UART_UART_INTERFACE_H_

	#include "../../Lib/STD_Types.h"


	/* UART Initialization */
	void UART_voidInit(void);


	/* Send one byte */
	u8 UART_u8SendByte(u8 Copy_u8Data);


	/* Receive one byte - Blocking */
	u8 UART_u8ReceiveByte(u8 *Copy_pu8Data);


	/* Receive one byte - Non Blocking */
	u8 UART_u8ReceiveByteNonBlocking(u8 *Copy_pu8Data);


	/* Send string */
	u8 UART_u8SendString(u8 *Copy_pu8String);


	#endif /* MCAL_UART_UART_INTERFACE_H_ */
