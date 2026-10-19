#ifndef MCAL_EEPROM_EEPROM_INTERFACE_H_
#define MCAL_EEPROM_EEPROM_INTERFACE_H_

#include "../../Lib/STD_Types.h"

/* ========================================= */
/* EEPROM DEVICE ADDRESS                     */
/* ========================================= */

#define EEPROM_FIXED_ADDRESS    0x50

/* ========================================= */
/* EEPROM FUNCTIONS                          */
/* ========================================= */

void EEPROM_voidInit(void);

u8 EEPROM_u8WriteByte(
    u8 Copy_u8Address,
    u8 Copy_u8Data
);

u8 EEPROM_u8ReadByte(
    u8 Copy_u8Address,
    u8 *Copy_pu8Data
);

#endif /* MCAL_EEPROM_EEPROM_INTERFACE_H_ */
