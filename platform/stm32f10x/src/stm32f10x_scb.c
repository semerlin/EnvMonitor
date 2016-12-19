#include "stm32f10x_scb.h"
#include "stm32f10x_map.h"
#include "stm32f10x_cfg.h"

/* SCB register must operate in privileged mode */


typedef struct
{
    volatile uint32 CPUID;
    volatile uint32 ICSR;
    volatile uint32 VTOR;
    volatile uint32 AIRCR;
    volatile uint32 SCR;
    volatile uint32 CCR;
    volatile uint32 SHPR1;
    volatile uint32 SHPR2;
    volatile uint32 SHPR3;
    volatile uint32 SHCSR;
    volatile uint32 CFSR;
    volatile uint32 HFSR;
    volatile uint32 MMFAR;
    volatile uint32 BFAR;
}SCB_TypeDef;

/* SCB寄存器结构体定义*/
SCB_TypeDef *SCB = (SCB_TypeDef *)SCB_BASE;

//寄存器内部位置定义
#define NMIPENDSET    ((uint32)1 << 31)
#define PENDSVSET     ((uint32)1 << 28)
#define PENDSVCLR     ((uint32)1 << 27)
#define PENDSTSET     ((uint32)1 << 26)
#define PENDSTCLR     ((uint32)1 << 25)
#define ISRPENDING    ((uint32)1 << 22)
#define VECTPENDING   ((uint32)0x1f << 12)
#define RETOBASE      ((uint32)1 << 11)
#define VECTACTIVE    ((uint32)0x1f << 0)

#define TBLOFF        ((uint32)0x7ffff << 11)
#define TBLBASE       ((uint32)1 << 29)

#define VECTKEY       ((uint32)0x5FA << 16)

#define PRIGROUP      ((uint32)0x07 << 8)
#define SYSRESETREQ   ((uint32)0x01 << 2)

#define SEVEONPEND    ((uint32)0x01 << 4)
#define SLEEPDEEP     ((uint32)0x01 << 2)
#define SLEEPONEXIT   ((uint32)0x01 << 1)

//CCR寄存器
#define STKALIGN      ((uint32)0x01 << 9)
#define BFHFNMIGN     ((uint32)0x01 << 8)
#define DIV0TRP       ((uint32)0x01 << 4)
#define UNALIGNTRP    ((uint32)0x01 << 3)
#define USERSETMPEND  ((uint32)0x01 << 1)
#define NONBASETHRDENA ((uint32)0x01)

//SHPRx寄存器
#define MEMMANGEFAULT   ((uint32)0x0f << 4)  
#define BUSFAULT        ((uint32)0x0f << 12)
#define USAGEFAULT      ((uint32)0x0f << 20)
#define SVCALL          ((uint32)0x0f << 28)
#define PENDSV          ((uint32)0x0f << 20)
#define SYSTICK         ((uint32)0x0f << 28)


//SHCSR寄存器
#define USGFAULTENA    ((uint32)0x01 << 18)
#define BUSFAULTENA    ((uint32)0x01 << 17)
#define MEMFAULTENA    ((uint32)0x01 << 16)

#define SVCALLPENDED   ((uint32)0x01 << 15)
#define BUSFAULTPENDED ((uint32)0x01 << 14)
#define MEMFAULTPENDED ((uint32)0x01 << 13)
#define USGFAULTPENDED ((uint32)0x01 << 12)

#define SYSTICKACT     ((uint32)0x01 << 11)
#define PENDSVACT      ((uint32)0x01 << 10)
#define MONITORACT     ((uint32)0x01 << 8)
#define SVCALLACT      ((uint32)0x01 << 7)
#define USGFAULTACT    ((uint32)0x01 << 3)
#define BUSFAULTACT    ((uint32)0x01 << 1)
#define MEMFAULTACT    ((uint32)0x01)


/**
 * @brief get cpu id
 * @return cpu id
 */
uint32 SCB_GetCPUID(void)
{
    return (SCB->CPUID);   
}

/**
 * @brief change NMI excpetion state to pending
 */
void SCB_PendNMI(void)
{
    SCB->ICSR |= NMIPENDSET;
}

/**
 * @brief check if NMI is pending
 * @retunr TREU:yes FALSE:no
 */
BOOL SCB_IsNMIPending(void)
{
    if(((SCB->ICSR) & NMIPENDSET) == NMIPENDSET)
        return TRUE;
    else
        return FALSE;
}

/**
 * @brief change pendsv exception state
 * @param flag TRUE:pending FALSE: not pending
 */
void SCB_PendPendSV(BOOL flag)
{
    if(flag)
        SCB->ICSR |= PENDSVSET;
    else
        SCB->ICSR |= PENDSVCLR;
}


/**
 * @brief check if PendSV is pending
 * @retunr TREU:yes FALSE:no
 */
BOOL SCB_IsPendSVPending(void)
{
    if(((SCB->ICSR) & PENDSVSET) == PENDSVSET)
        return TRUE;
    else
        return FALSE;
}

/**
 * @brief change SysTick exception state
 * @param flag TRUE:pending FALSE: not pending
 */
void SCB_PendSysTick(BOOL flag)
{
    if(flag)
        SCB->ICSR |= PENDSTSET;
    else
        SCB->ICSR |= PENDSTCLR;
}

/**
 * @brief check if any interrupt is pending, excluding NMI and Faults
 * @return TRUE:yes FALSE:no
 */
BOOL SCB_IsIntPending(void)
{
    if(((SCB->ICSR) & ISRPENDING) == ISRPENDING)
        return TRUE;
    else
        return FALSE;
}

/**
 * @brief get pending interrupt vector
 * @return interrupt vector
 */
uint32 SCB_GetPendIntVector(void)
{
    return (((SCB->ICSR) & VECTPENDING) >> 12);
}


/**
 * @brief indicates whether there are preempted active exceptions
 * @return TRUE:yes FALSE:no
 */
BOOL SCB_IsIntPreempted(void)
{
    if(((SCB->ICSR) & RETOBASE) == RETOBASE)
        return FALSE;
    else
        return TRUE;
}

/**
 * @brief get active exception vector
 * @retunr active exception vector
 */
uint32 SCB_GetActiveIntVector(void)
{
    return ((SCB->ICSR) & VECTACTIVE);
}

/**
* @brief set vector table address
* @param vector table address description
*/
void SCB_SetVectTableConfig(__in VectTable table)
{
    if(table.place == CODE)
        SCB->VTOR &= ~TBLBASE;
    else
        SCB->VTOR |= TBLBASE;
    
    SCB->VTOR = (((table.offsetAddr) & 0x7ffff) << 9); 
}

/**
* @brief set vector table address
* @return vector table address description
*/
VectTable SCB_GetVectTableConfig(void)
{
    VectTable table;
    table.offsetAddr = (((SCB->VTOR) & TBLOFF) >> 9);
    if(((SCB->VTOR) & TBLBASE) == TBLBASE)
        table.place = SRAM;
    else
        table.place = CODE;
    
    return table;
}

/**
 * @brief set priority group
 * @param priority group
 */
void SCB_SetPriorityGroup(__in uint32 group)
{
    assert_param(group <= 7);
	SCB->AIRCR &= (VECTKEY | (~PRIGROUP));
	SCB->AIRCR |= (VECTKEY | ((group << 8) & PRIGROUP));
}

/**
 * @brief generate system reset
 */
void SCB_GenSystemReset(void)
{
	SCB->AIRCR |= (VECTKEY | SYSRESETREQ);
}



/**
 * @brief config only enabled interrupts or all interrupts can wakeup processor
 * @param TRUE: all FALSE: only enabled
 */
void SCB_EnableAllIntWakeup(__in BOOL flag)
{
	SCB->SCR &= ~SEVEONPEND;
    if(flag)
        SCB->SCR |= (1 << 4);
}

/**
 * @brief controls whether the processor uses sleep or deep sleep as its low
 *        power mode
 * @param sleep mode
 */                  
void SCB_SetSleepMode(__in uint8 mode)
{
    assert_param(IS_SCB_SLEEP_PARAM(mode));
    SCB->SCR &= ~SLEEPDEEP;
    SCB->SCR |= mode;
}

/**
 * @brief configures sleep-on-exit when returning from handler mode to thread 
 *        mode
 * @param TRUE: Enter sleep or deep sleep FALSE: Do not sleep
 */  
void SCB_EnableSleepOnExit(__in BOOL flag)
{
    SCB->SCR &= ~SLEEPONEXIT;

    if(flag)
        SCB->SCR |= SLEEPONEXIT;
}


/**
* @brief set stack align mode
* @param align mode
*/                   
void SCB_SetStackAlign(__in uint16 align)
{
    assert_param(IS_SCB_STACK_PARAM(align));
    SCB->CCR &= ~STKALIGN;
    SCB->CCR |= align;
}


/**
 * @brief whether process bus faults or not
 * @param TRUE: yes FALSE:no
 */                   
void SCB_BusFaultIgnore(__in BOOL flag)
{
    SCB->CCR &= ~BFHFNMIGN;
    if(flag)
        SCB->CCR |= BFHFNMIGN;
}



/**
 * @brief configure trap divide by 0 
 * @param TRUE: yes FALSE:no
 */                   
void SCB_EnableDiv0Trp(__in BOOL flag)
{
    SCB->CCR &= ~DIV0TRP;
    if(flag)
        SCB->CCR |= DIV0TRP;
}


/**
 * @brief enables unaligned access traps 
 * @param TRUE: yes FALSE:no
 */ 
void SCB_EnableUnalignTrp(__in BOOL flag)
{
    SCB->CCR &= ~UNALIGNTRP;
    if(flag)
        SCB->CCR |= UNALIGNTRP;
}

/**
 * @brief enables unprivileged software access to the STIR 
 * @param TRUE: yes FALSE:no
 */                   
void SCB_EnableUserAccessSTIR(__in BOOL flag)
{
    SCB->CCR &= ~USERSETMPEND;
    if(flag)
        SCB->CCR |= USERSETMPEND;
}

/**
 * @brief set thread mode enter method
 * @param enter method
 */
void SCB_SetThreadModeEnterMethod(__in uint8 mode)
{
    assert_param(IS_SCB_THREADMODE_PARAM(mode));
    SCB->CCR &= ~NONBASETHRDENA;
    SCB->CCR |= mode;
}


/**
 * @brief set exception handlers priority
 * @param exception name
 * @param exception priority, 0-15, 0 has the highest priority, 15 has the 
         lowest                   
 */                   
void SCB_SetExceptionPriorty(__in uint8 exception, __in uint32 priority)
{
    assert_param(IS_SCB_EXCEPTION_PARAM(exception));
    assert_param(priority <= 15);
   
    switch(exception)
    {
    case SCB_Exception_MemMangeFault:
        SCB->SHPR1 &= ~MEMMANGEFAULT;
        SCB->SHPR1 |= (priority << 4);
        break;
    case SCB_Exception_BusFault:
        SCB->SHPR1 &= ~BUSFAULT;
        SCB->SHPR1 |= (priority << 12);
        break;
    case SCB_Exception_UsageFault:
        SCB->SHPR1 &= ~USAGEFAULT;
        SCB->SHPR1 |= (priority << 20);
        break;
    case SCB_Exception_SVCall:
        SCB->SHPR2 &= ~SVCALL;
        SCB->SHPR2 |= (priority << 28);
        break;
    case SCB_Exception_PendSV:
        SCB->SHPR3 &= ~PENDSV;
        SCB->SHPR3 |= (priority << 20);
        break;
    case SCB_Exception_SysTick:
        SCB->SHPR3 &= ~SYSTICK;
        SCB->SHPR3 |= (priority << 28);
        break;
    default:
        break;
    }
}


/**
 * @brief enable exception handler
 * @param exception handler
 * @param TRUE: enable FALSE: disable
 */
void SCB_EnableException(__in uint8 handle, __in BOOL flag)
{
    assert_param(IS_SCB_EXCEPTION_PARAM(handle));
    switch(handle)
    {
    case SCB_Exception_MemMangeFault:
        if(flag)
            SCB->SHCSR |= MEMFAULTENA;
        else
            SCB->SHCSR &= ~MEMFAULTENA;
        break;
    case SCB_Exception_BusFault:
        if(flag)
            SCB->SHCSR |= BUSFAULTENA;
        else
            SCB->SHCSR &= ~BUSFAULTENA;
        break;
    case SCB_Exception_UsageFault:
        if(flag)
            SCB->SHCSR |= USGFAULTENA;
        else
            SCB->SHCSR &= ~USGFAULTENA;
        break;
    default:
         break;
    }
}


/**
 * @brief get pending exception handlers
 * @return pending exception handlers
 */
uint8 SCB_GetPendingException(void)
{
    uint8 pendException = (SCB->SHCSR >> 12) & 0x0f;
    uint8 ret = 0;
    
    if(pendException & 0x01)
        ret |= SCB_Exception_UsageFault;
    
    if(pendException & 0x02)
        ret |= SCB_Exception_MemMangeFault;
    
    if(pendException & 0x04)
        ret |= SCB_Exception_BusFault;
    
    if(pendException & 0x08)
        ret |= SCB_Exception_SVCall;
    
    return ret;
}


/**
 * @brief set exception pending status
 * @param TRUE:set FALSE:reset
 */
void SCB_SetPendingStatus(__in uint8 exception, __in BOOL flag)
{   
    assert_param(IS_SCB_EXCEPTION_PARAM(exception));
   
    switch(exception)
    {
    case SCB_Exception_MemMangeFault:
        if(flag)
            SCB->SHCSR |= MEMFAULTPENDED;
        else
            SCB->SHCSR &= ~MEMFAULTPENDED;
        break;
    case SCB_Exception_BusFault:
        if(flag)
            SCB->SHCSR |= BUSFAULTPENDED;
        else
            SCB->SHCSR &= ~BUSFAULTPENDED;
        break;
    case SCB_Exception_UsageFault:
        if(flag)
            SCB->SHCSR |= USGFAULTPENDED;
        else
            SCB->SHCSR &= ~USGFAULTPENDED;
        break;
    case SCB_Exception_SVCall:
        if(flag)
            SCB->SHCSR |= SVCALLPENDED;
        else
            SCB->SHCSR &= ~SVCALLPENDED;
        break;
    default:
        break;
    }
}

/**
 * @param get active exception handlers
 * @return active exception handlers
 */
uint8 SCB_GetActiveException(void)
{
    uint8 dwActiveStatus = 0;
    if(SCB->SHCSR & MEMFAULTACT)
    {
        dwActiveStatus |= SCB_Exception_MemMangeFault;
    }

    if(SCB->SHCSR & BUSFAULTACT)
    {
        dwActiveStatus |= SCB_Exception_BusFault;
    }
    
    if(SCB->SHCSR & USGFAULTACT)
    {
        dwActiveStatus |= SCB_Exception_UsageFault;
    }

    if(SCB->SHCSR & SVCALLACT)
    {
        dwActiveStatus |= SCB_Exception_SVCall;
    }

    if(SCB->SHCSR & MONITORACT)
    {
        dwActiveStatus |= SCB_Debug_Monitor;
    }

    if(SCB->SHCSR & PENDSVACT)
    {
        dwActiveStatus |= SCB_Exception_PendSV;
    }

    if(SCB->SHCSR & SYSTICKACT)
    {
        dwActiveStatus |= SCB_Exception_SysTick;
    }

    return dwActiveStatus;
}

/**
 * @brief set exception active status
 * @param exception handler
 * @param TRUE:set FALSE:reset
 */
void SCB_SetActiveStatus(__in uint8 exception, __in BOOL flag)
{   
    
    assert_param(IS_SCB_ACTIVE_PARAM(exception));
   
    switch(exception)
    {
    case SCB_Exception_MemMangeFault:
        if(flag)
            SCB->SHCSR |= MEMFAULTACT;
        else
            SCB->SHCSR &= ~MEMFAULTACT;
        break;
    case SCB_Exception_BusFault:
        if(flag)
            SCB->SHCSR |= BUSFAULTACT;
        else
            SCB->SHCSR &= ~BUSFAULTACT;
        break;
    case SCB_Exception_UsageFault:
        if(flag)
            SCB->SHCSR |= USGFAULTACT;
        else
            SCB->SHCSR &= ~USGFAULTACT;
        break;
    case SCB_Exception_SVCall:
        if(flag)
            SCB->SHCSR |= SVCALLACT;
        else
            SCB->SHCSR &= ~SVCALLACT;
        break;
    case SCB_Debug_Monitor:
        if(flag)
            SCB->SHCSR |= MONITORACT;
        else
            SCB->SHCSR &= ~MONITORACT;
        break;
    case SCB_Exception_PendSV:
        if(flag)
            SCB->SHCSR |= PENDSVACT;
        else
            SCB->SHCSR &= ~PENDSVACT;
        break;
    case SCB_Exception_SysTick:
        if(flag)
            SCB->SHCSR |= SYSTICKACT;
        else
            SCB->SHCSR &= ~SYSTICKACT;
        break;
    default:
        break;
    }
}

/**
 * @brief get usage fault reason
 * @return reason
 */
uint32 SCB_GetUsageFaultDetail(void)
{
    return (SCB->CFSR & 0xffff0000);
}

/**
 * @brief clear usage fault
 * @param position to clear
 */
void SCB_ClrUsageFaultStatus(__in uint32 reg)
{
    assert_param(IS_SCB_USAGE_FAULT_PARAM(reg));
    SCB->CFSR |= reg;
}

/**
 * @brief get bus fault reason
 * @return reason
 */
uint32 SCB_GetBusFaultDetail(void)
{
    return (SCB->CFSR & 0x0000ff00);
}

/**
 * @brief clear bus fault
 * @param position to clear
 */
void SCB_ClrBusFaultStatus(__in uint32 status)
{
    assert_param(IS_SCB_BUS_FAULT_PARAM(status));
    SCB->CFSR |= status;
}

/**
 * @brief get bus fault reason
 * @return reason
 */
uint32 SCB_GetMemFaultDetail(void)
{
  return (SCB->CFSR & 0x000000ff);
}


/**
 * @brief clear bus fault
 * @param position to clear
 */
void SCB_ClrMemFaultStatus(__in uint32 reg)
{
    assert_param(IS_SCB_MEM_FAULT_PARAM(reg));
    SCB->CFSR |= reg;
}


/**
 * @brief get hard fault reason
 * @return reason
 */
uint32 SCB_GetHardFaultDetail(void)
{
  return SCB->HFSR;
}


/**
 * @brief clear hard fault
 * @param position to clear
 */
void SCB_ClrHardFaultStatus(__in uint32 reg)
{
    assert_param(IS_SCB_HARD_FAULT_PARAM(reg));
    SCB->HFSR |= reg;
}

/**
 * @brief get memory management fault address
 * @return address
 */
uint32 SCB_GetMemFaultAddress(void)
{
    return SCB->MMFAR;
}

/**
 * @brief get memory management fault address
 * @return address
 */
uint32 SCB_GetBusFaultAddress(void)
{
    return SCB->BFAR;
}







