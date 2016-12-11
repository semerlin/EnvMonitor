#ifndef _CM3_CORE_H
  #define _CM3_CORE_H

#include "sysdef.h"

void __WFI(void);
void __WFE(void);
void __SEV(void);
void __ISB(void);
void __DSB(void);
void __DMB(void);
void __SVC(void);
uint32 __MRS_CONTROL(void);
void __MSR_CONTROL(uint32 Control);
void __SETPRIMASK(void);
void __RESETPRIMASK(void);
void __SETFAULTMASK(void);
void __RESETFAULTMASK(void);
void __BASEPRICONFIG(uint32 NewPriority);
uint32 __GetBASEPRI(void);
uint16 __REV_HalfWord(uint16 Data);
uint32 __REV_Word(uint32 Data);

#endif

