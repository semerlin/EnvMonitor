#ifndef _STM32F10X_CRC_H_
  #define _STM32F10X_CRC_H_

#include "sysdef.h"

/* interface */
void CRC_ResetDR(void);
uint32 CRC_Cal(__in uint32 data);
uint32 CRC_CalBlock(__in uint32 *buf, __in uint32 len);
uint32 CRC_GetDR(void);
void CRC_SetIDR(__in uint8 data);
uint8 CRC_GetIDR(void);

#endif

