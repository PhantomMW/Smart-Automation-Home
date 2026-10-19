#ifndef HAL_KEYPAD_KEYPAD_INTERFACE_H_
#define HAL_KEYPAD_KEYPAD_INTERFACE_H_

#include "../../Lib/STD_Types.h"


#define KEYPAD_NO_KEY  0xFF


void KEYPAD_voidInit(void);

u8 KEYPAD_u8GetKey(void);


#endif /* HAL_KEYPAD_KEYPAD_INTERFACE_H_ */
