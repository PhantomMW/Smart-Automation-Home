#ifndef HAL_BLUETOOTH_BLUETOOTH_INTERFACE_H_
#define HAL_BLUETOOTH_BLUETOOTH_INTERFACE_H_

#include "../../Lib/STD_Types.h"


/* Initialize Bluetooth */
void BLUETOOTH_voidInit(void);


/* Send one character */
u8 BLUETOOTH_u8SendChar(u8 Copy_u8Data);


/* Send string */
u8 BLUETOOTH_u8SendString(u8 *Copy_pu8String);


/* Receive one character */
u8 BLUETOOTH_u8ReceiveChar(u8 *Copy_pu8Data);


#endif /* HAL_BLUETOOTH_BLUETOOTH_INTERFACE_H_ */
