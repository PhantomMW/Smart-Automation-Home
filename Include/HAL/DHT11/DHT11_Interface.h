#ifndef DHT11_INTERFACE_H_
#define DHT11_INTERFACE_H_

#include "../../Lib/STD_Types.h"

#define DHT11_OK       1
#define DHT11_ERROR    0

void DHT11_Init(void);
u8 DHT11_Read(u8 *temperature, u8 *humidity);

#endif
