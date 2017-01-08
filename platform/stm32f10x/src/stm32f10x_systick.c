#include "stm32f10x_systick.h"
#include "stm32f10x_map.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_cfg.h"


/* systick register structure */
typedef struct
{
    volatile uint32 CTRL;
    volatile uint32 LOAD;
    volatile uint32 VAL;
    volatile uint32 CALIB;
}SYSTICK_T;

SYSTICK_T *SYSTICK = (SYSTICK_T *)SYSTICK_BASE;

/* systick regiter definition */
#define CTRL_COUNTFLAG    (1 << 16)
#define CTRL_CLKSOURCE    (1 << 2)
#define CTRL_TICKINT      (1 << 1)
#define CTRL_ENABLE       (0x01)



/**
 * @brief set systick clock source
 * @param clock source
 */
void SYSTICK_SetClockSource(__in uint8 source)
{
    assert_param(IS_SYSTICK_CLOCK_PARAM(source));
    SYSTICK->CTRL &= ~CTRL_CLKSOURCE;
    SYSTICK->CTRL |= source;
}

/**
 * @brief systick exception request enable
 * @param enable flag
 */
void SYSTICK_EnableInt(__in BOOL flag)
{
    SYSTICK->CTRL &= ~CTRL_TICKINT;
    if(flag)
        SYSTICK->CTRL |= CTRL_TICKINT;
}

/**
 * @brief start or stop systick counter
 * @param start flag
 */
void SYSTICK_EnableCounter(__in BOOL flag)
{
    SYSTICK->CTRL &= ~CTRL_ENABLE;
    if(flag)
        SYSTICK->CTRL |= CTRL_ENABLE;
}

/**
 * @brief check if count flag is set
 * @return TRUE:set FALSE:not set
 */
BOOL SYSTICK_IsCountFlagSet(void)
{
    if(SYSTICK->CTRL & CTRL_COUNTFLAG)
        return TRUE;
    
    return FALSE;
}

/**
 * @brief clear systick count flag
 */
void SYSTICK_ClrCountFlag(void)
{
    SYSTICK->CTRL &= ~CTRL_COUNTFLAG;
}

/**
 * @brief set systick tick interval
 * @param time intervel, unit is ms
 */
void SYSTICK_SetTickInterval(__in uint32 time)
{
    uint32 hclk = RCC_GetHCLK();
    uint32 tickClock = 0;
    if(SYSTICK->CTRL & CTRL_CLKSOURCE)
        tickClock = hclk;
    else
        tickClock = (hclk >> 3);
    
#ifdef __DEBUG
    uint32 maxInterval = (1 << 24) / (tickClock / 1000);
    assert_param(time <= maxInterval);
#endif
    
    SYSTICK->LOAD = ((tickClock / 1000 * time) & 0xffffff);
}