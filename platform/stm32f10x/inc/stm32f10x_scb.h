#ifndef _LIB_SBC_STM32_H_
  #define _LIB_SBC_STM32_H_

#include "sysdef.h"

typedef enum
{
    CODE, 
    SRAM,
}VectTablePlace;

typedef struct
{
    uint32 offsetAddr;
    VectTablePlace place;
}VectTable, *PVectTable;


typedef enum
{
    MemMangeFault = 0, BusFault, UsageFault, SVCall, PendSV, SysTick,
}SysIntHandle;


/* sleep mode */
#define SCB_Sleep_Sleep        (0x00)
#define SCB_Sleep_DeepSleep    (1 << 2)
#define IS_SCB_SLEEP_PARAM(SLEEP)   ((SLEEP == SCB_Sleep_Sleep) || \
                                     (SLEEP == SCB_Sleep_DeepSleep))

/* stack align */
#define SCB_Stack_Align_4Bytes    (0x00)
#define SCB_Stack_Align_8Bytes    (1 << 9)
#define IS_SCB_STACK_PARAM(STACK)   ((STACK == SCB_Stack_Align_4Bytes) || \
                                     (STACK == SCB_Stack_Align_8Bytes))

/* thread mode enter method */
#define SCB_ThreadMode_Enter_NoActiveException    (0x00)
#define SCB_ThreadMode_Enter_EXC_RETURN           (1 << 0)
#define IS_SCB_THREADMODE_PARAM(STACK)  \
                    ((STACK == SCB_ThreadMode_Enter_NoActiveException) || \
                    (STACK == SCB_ThreadMode_Enter_EXC_RETURN))

                        
/* system exception handlers */
#define SCB_Exception_MemMangeFault     (0x01)
#define SCB_Exception_BusFault          (0x02)
#define SCB_Exception_UsageFault        (0x04)
#define SCB_Exception_SVCall            (0x08)
#define SCB_Exception_PendSV            (0x10)
#define SCB_Exception_SysTick           (0x20)
#define SCB_Debug_Monitor               (0x40)
#define IS_SCB_EXCEPTION_PARAM(EXCEPTION) \
                        ((EXCEPTION == SCB_Exception_MemMangeFault) || \
                         (EXCEPTION == SCB_Exception_BusFault) || \
                         (EXCEPTION == SCB_Exception_UsageFault) || \
                         (EXCEPTION == SCB_Exception_SVCall) || \
                         (EXCEPTION == SCB_Exception_PendSV) || \
                         (EXCEPTION == SCB_Exception_SysTick))  
                            
#define IS_SCB_ACTIVE_PARAM(ACTIVE) \
                        ((ACTIVE == SCB_Exception_MemMangeFault) || \
                         (ACTIVE == SCB_Exception_BusFault) || \
                         (ACTIVE == SCB_Exception_UsageFault) || \
                         (ACTIVE == SCB_Exception_SVCall) || \
                         (ACTIVE == SCB_Exception_PendSV) || \
                         (ACTIVE == SCB_Exception_SysTick) || \
                         (ACTIVE == SCB_Debug_Monitor))  

                            
/* usage fault detail */
#define SCB_USAGE_FAULT_DIVBYZERO          (1 << 25)
#define SCB_USAGE_FAULT_UNALIGNED          (1 << 24)
#define SCB_USAGE_FAULT_NOCP               (1 << 19)
#define SCB_USAGE_FAULT_INVPC              (1 << 18)
#define SCB_USAGE_FAULT_INVSTATE           (1 << 17)
#define SCB_USAGE_FAULT_UNDEFINSTR         (1 << 16)
#define IS_SCB_USAGE_FAULT_PARAM(USAGE) \
                            ((USAGE == SCB_USAGE_FAULT_DIVBYZERO) || \
                             (USAGE == SCB_USAGE_FAULT_UNALIGNED) || \
                             (USAGE == SCB_USAGE_FAULT_NOCP) || \
                             (USAGE == SCB_USAGE_FAULT_INVPC) || \
                             (USAGE == SCB_USAGE_FAULT_INVSTATE) || \
                             (USAGE == SCB_USAGE_FAULT_UNDEFINSTR))
                                
                                
/* bus fault detail */
#define SCB_BUS_FAULT_BFARVALID            (1 << 15)
#define SCB_BUS_FAULT_STKERR               (1 << 12)
#define SCB_BUS_FAULT_UNSTKERR             (1 << 11)
#define SCB_BUS_FAULT_IMPRECISERR          (1 << 10)
#define SCB_BUS_FAULT_PRECISERR            (1 << 9)
#define SCB_BUS_FAULT_IBUSERR              (1 << 8)
#define IS_SCB_BUS_FAULT_PARAM(BUS) \
                            ((BUS == SCB_BUS_FAULT_BFARVALID) || \
                             (BUS == SCB_BUS_FAULT_STKERR) || \
                             (BUS == SCB_BUS_FAULT_UNSTKERR) || \
                             (BUS == SCB_BUS_FAULT_IMPRECISERR) || \
                             (BUS == SCB_BUS_FAULT_PRECISERR) || \
                             (BUS == SCB_BUS_FAULT_IBUSERR))
                                
                                
 /* memory management fault detail */
#define SCB_MEMFAULT_MMARVALID            (1 << 7)
#define SCB_MEMFAULT_MSTKERR              (1 << 4)
#define SCB_MEMFAULT_MUNSTKERR            (1 << 3)
#define SCB_MEMFAULT_DACCVIOL             (1 << 1)
#define SCB_MEMFAULT_IACCVIOL             (1 << 0)
#define IS_SCB_MEM_FAULT_PARAM(MEM) \
                            ((MEM == SCB_MEMFAULT_MMARVALID) || \
                             (MEM == SCB_MEMFAULT_MSTKERR) || \
                             (MEM == SCB_MEMFAULT_MUNSTKERR) || \
                             (MEM == SCB_MEMFAULT_DACCVIOL) || \
                             (MEM == SCB_MEMFAULT_IACCVIOL))
                                
                                
/* hard fault detail */
#define SCB_HARDFAULT_FORCED        (1 << 30)
#define SCB_HARDFAULT_VECTTBL       (1 << 1)
#define IS_SCB_HARD_FAULT_PARAM(HARD) \
                            ((HARD == SCB_HARDFAULT_FORCED) || \
                             (HARD == SCB_HARDFAULT_VECTTBL))



/* interface */
uint32 SCB_GetCPUID(void);
void SCB_PendNMI(void);
BOOL SCB_IsNMIPending(void);
void SCB_PendPendSV(BOOL flag);
BOOL SCB_IsPendSVPending(void);
void SCB_PendSysTick(BOOL flag);
BOOL SCB_IsIntPending(void);
uint32 SCB_GetPendIntVector(void);
BOOL SCB_IsIntPreempted(void);
uint32 SCB_GetActiveIntVector(void);
void SCB_SetVectTableConfig(__in VectTable table);
VectTable SCB_GetVectTableConfig(void);
void SCB_SetPriorityGrouping(__in uint32 group);
uint8 SCB_GetMinPreemptionPriority(void);
uint8 SCB_GetMinSubPriority(void);
uint8 SCB_GetPriorityGrouping(void);
void SCB_GenSystemReset(void);
void SCB_EnableAllIntWakeup(__in BOOL flag);         
void SCB_SetSleepMode(__in uint8 mode);
void SCB_EnableSleepOnExit(__in BOOL flag);                
void SCB_SetStackAlign(__in uint16 align);               
void SCB_BusFaultIgnore(__in BOOL flag);           
void SCB_EnableDiv0Trp(__in BOOL flag);
void SCB_EnableUnalignTrp(__in BOOL flag);          
void SCB_EnableUserAccessSTIR(__in BOOL flag);
void SCB_SetThreadModeEnterMethod(__in uint8 mode);     
void SCB_SetExceptionPriorty(__in uint8 exception, __in uint32 priority);
void SCB_EnableException(__in uint8 handle, __in BOOL flag);
uint8 SCB_GetPendingException(void);
void SCB_SetPendingStatus(__in uint8 exception, __in BOOL flag);
uint8 SCB_GetActiveException(void);
void SCB_SetActiveStatus(__in uint8 exception, __in BOOL flag);
uint32 SCB_GetUsageFaultDetail(void);
void SCB_ClrUsageFaultStatus(__in uint32 reg);
uint32 SCB_GetBusFaultDetail(void);
void SCB_ClrBusFaultStatus(__in uint32 status);
uint32 SCB_GetMemFaultDetail(void);
void SCB_ClrMemFaultStatus(__in uint32 reg);
uint32 SCB_GetHardFaultDetail(void);
void SCB_ClrHardFaultStatus(__in uint32 reg);
uint32 SCB_GetMemFaultAddress(void);
uint32 SCB_GetBusFaultAddress(void);


#endif
