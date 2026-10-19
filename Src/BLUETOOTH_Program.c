#include "../Include/Lib/STD_Types.h"

#include "../Include/MCAL/UART/UART_Interface.h"

#include "../Include/HAL/BLUETOOTH/BLUETOOTH_Interface.h"
#include "../Include/HAL/BLUETOOTH/BLUETOOTH_Private.h"
#include "../Include/HAL/BLUETOOTH/BLUETOOTH_Cfg.h"


void BLUETOOTH_voidInit(void)
{
    UART_voidInit();
}


u8 BLUETOOTH_u8SendChar(u8 Copy_u8Data)
{
    return UART_u8SendByte(Copy_u8Data);
}


u8 BLUETOOTH_u8SendString(u8 *Copy_pu8String)
{
    return UART_u8SendString(Copy_pu8String);
}


u8 BLUETOOTH_u8ReceiveChar(u8 *Copy_pu8Data)
{
    if (Copy_pu8Data == NULL)
    {
        return STD_TYPES_NOK;
    }

    return UART_u8ReceiveByte(Copy_pu8Data);
}
