#ifndef _CM3_CORE_H
  #define _CM3_CORE_H

#include "sysdef.h"

void __NOP(void);
void __WFI(void);
void __WFE(void);
void __SEV(void);
void __ISB(void);
void __DSB(void);
void __DMB(void);
void __SVC(void);
uint32 __get_PSR(void);
uint32 __get_IPSR(void);
uint32 __get_CONTROL(void);
void __set_CONTROL(__in uint32 val);
uint32 __get_PSP(void);
void __set_PSP(__in uint32 val);
uint32 __get_MSP(void);
void __set_MSP(__in uint32 val);
void __set_PRIMASK();
uint32 __get_PRIMASK(void);
void __reset_PRIMASK(void);
void __set_FAULTMASK(void);
uint32 __get_FAULTMASK(void);
void __reset_FAULTMASK(void);
void __set_BASEPRI(__in uint32 val);
uint32 __get_BASEPRI(void);
uint32 __REV(__in uint32 value);
uint32 __REV16(__in uint16 value);
int32 __REVSH(__in int16 value);
uint32 __RBIT(__in uint32 value);
uint32 __CLZ(__in uint32 value);


#endif

