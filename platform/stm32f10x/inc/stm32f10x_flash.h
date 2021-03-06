#ifndef _STM32F10X_FLASH_H_
  #define _STM32F10X_FLASH_H_

#include "sysdef.h"


/* flash latency definition */
#define FLASH_LATENCY_ZERO    0x00
#define FLASH_LATENCY_ONE     0x01
#define FLASH_LATENCY_TWO     0x02

#define IS_FLASH_LATENCY_PARAM(param) ((param == FLASH_LATENCY_ZERO) || \
                                       (param == FLASH_LATENCY_ONE) || \
                                       (param == FLASH_LATENCY_TWO))









/* interface */
void Flash_EnablePrefetch(__in BOOL flag);
BOOL FLASH_IsPrefetchEnabled(void);
void FLASH_EnableHalfCycleAccess(__in BOOL flag);
BOOL FLASH_IsHalfCycleAccessEnabled(void);
void FLASH_SetLatency(__in uint8 latency);



























#endif


