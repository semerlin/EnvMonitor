#include "stm32f10x_nvic.h"
#include "stm32f10x_map.h"
#include "stm32f10x_cfg.h"

/* NVIC register must operate in privileged mode */

typedef struct
{
    volatile uint32 ISER[3];
    uint32 RESERVED0[29];
    volatile uint32 ICER[3];
    uint32 RESERVED1[29];
    volatile uint32 ISPR[3];
    uint32 RESERVED2[29];
    volatile uint32 ICPR[3];
    uint32 RESERVED3[29];
    volatile uint32 IABR[3];
    uint32 RESERVED4[61];
    volatile uint32 IPR[21];
    uint32 RESERVED5[683];
    volatile uint32 STIR;
}NVIC_T;
       
/* NVIC register map definition */
NVIC_T *NVIC = (NVIC_T *)NVIC_BASE;

/**
* @brief enable or disable irq channel
* @param irq channel
* @param enable or disable flag
*/
void NVIC_EnableIRQ(__in uint8 channel, __in BOOL flag)
{
    assert_param(IS_NVIC_IRQ_CHANNEL(channel));
    if(flag)
        NVIC->ISER[channel >> 5] |= (1 << (channel % 32));
    else
        NVIC->ICER[channel >> 5] |= (1 << (channel % 32));
}

/**
* @brief set or reset irq pending
* @param irq channel
* @param set or reset flag
*/
void NVIC_SetIRQPending (__in uint8 channel, __in BOOL flag)
{
    assert_param(IS_NVIC_IRQ_CHANNEL(channel));
    if(flag)
        NVIC->ISPR[channel >> 5] |= (1 << (channel % 32));
    else
        NVIC->ICPR[channel >> 5] |= (1 << (channel % 32));
}

/**
* @brief check whether irq is pending
* @param irq channel
* @return pending flag
*/
BOOL NVIC_IsIRQPending(__in uint8 channel)
{
    assert_param(IS_NVIC_IRQ_CHANNEL(channel));
    if((NVIC->ISPR[channel >> 5] & (1 << (channel % 32))) != 0)
        return TRUE;
    else
        return FALSE;
}

/**
* @brief check whether irq is active
* @param irq channel
* @return active flag
*/
BOOL NVIC_IsIRQActive(__in uint8 channel)
{
    assert_param(IS_NVIC_IRQ_CHANNEL(channel));
    if((NVIC->IABR[channel >> 5] & (1 << (channel % 32))) != 0)
        return TRUE;
    else
        return FALSE;
}

/**
* @brief set irq priority
* @param irq channel
* @param irq priority
*/
void NVIC_SetIRQPriority(__in uint8 channel, __in uint32 priority)
{
    assert_param(IS_NVIC_IRQ_CHANNEL(channel));
    assert_param(priority <= 15);
    
    NVIC->IPR[channel >> 2] |= (priority << ((channel % 4) * 4));
}

/**
* @brief cget irq priority
* @param irq channel
* @return irq priority
*/
uint8 NVIC_GetIRQPriority(__in uint8 channel)
{
    assert_param(IS_NVIC_IRQ_CHANNEL(channel));
    
    return (NVIC->IPR[channel >> 2] >> ((channel % 4) * 4)) & 0x0f;
}

/**
* @brief generate software irq
* @param irq num
*/
void NVIC_GenerateIRQ(__in uint8 num)
{
    NVIC->STIR = num;
}