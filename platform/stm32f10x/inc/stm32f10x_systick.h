#ifndef _STM32F10X_SYSTICK_H_
  #define _STM32F10X_SYSTICK_H_

#include "sysdef.h"

/* systick clock resource */
#define SYSTICK_CLOCK_AHB_DIV_EIGHT    (0x00)
#define SYSTICK_CLOCK_AHB              (1 << 2)
#define IS_SYSTICK_CLOCK_PARAM(CLOCK) \
                       ((CLOCK == SYSTICK_CLOCK_AHB_DIV_EIGHT) || \
                        (CLOCK == SYSTICK_CLOCK_AHB))





/* interface */
void SYSTICK_SetClockSource(__in uint8 source);
void SYSTICK_EnableInt(__in BOOL flag);
void SYSTICK_EnableCounter(__in BOOL flag);
BOOL SYSTICK_IsCountFlagSet(void);
void SYSTICK_ClrCountFlag(void);
void SYSTICK_SetTickInterval(__in uint32 time);






#endif