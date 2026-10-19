
#ifndef MCAL_TIMER_TIMER_INTERFACE_H_
#define MCAL_TIMER_TIMER_INTERFACE_H_

#include "../../Lib/STD_Types.h"

/* ========================================= */
/* SOFTWARE PWM                             */
/* ========================================= */

/*
 * PWM output:
 *
 * PA0
 */

void TIMER0_voidSoftwarePWMInit(void);

void TIMER0_voidSetDutyCycle(u8 Copy_u8Duty);

void TIMER0_voidStartPWM(void);

void TIMER0_voidStopPWM(void);

#endif
