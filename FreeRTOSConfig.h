#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <avr/io.h>


/* ================================================= */
/* CPU / TICK                                       */
/* ================================================= */

#define configUSE_PREEMPTION        1

#define configUSE_IDLE_HOOK         0
#define configUSE_TICK_HOOK         0

#define configCPU_CLOCK_HZ          ((unsigned long)8000000UL)

#define configTICK_RATE_HZ          ((TickType_t)1000)


/* ================================================= */
/* TASK CONFIGURATION                               */
/* ================================================= */

#define configMAX_PRIORITIES        3

#define configMINIMAL_STACK_SIZE    80

#define configTOTAL_HEAP_SIZE       900

#define configMAX_TASK_NAME_LEN     10

#define configUSE_16_BIT_TICKS      1


/* ================================================= */
/* SYNCHRONIZATION                                  */
/* ================================================= */

#define configUSE_MUTEXES             1

#define configUSE_RECURSIVE_MUTEXES   0

#define configUSE_COUNTING_SEMAPHORES 1


/* ================================================= */
/* SOFTWARE TIMERS                                 */
/* ================================================= */

#define configUSE_TIMERS             0


/* ================================================= */
/* ERROR HANDLING                                  */
/* ================================================= */

#define configUSE_MALLOC_FAILED_HOOK 0

#define configCHECK_FOR_STACK_OVERFLOW 2


/* ================================================= */
/* MEMORY                                          */
/* ================================================= */

#define configSUPPORT_DYNAMIC_ALLOCATION 1

#define configSUPPORT_STATIC_ALLOCATION  0


/* ================================================= */
/* API                                              */
/* ================================================= */

#define INCLUDE_vTaskDelay           1


#endif
