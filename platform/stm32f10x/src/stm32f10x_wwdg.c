#include "stm32f10x_wwdg.h"
#include "stm32f10x_map.h"
#include "stm32f10x_cfg.h"

/**
 * @brief wwdg: it's call wwdg because there is a high and low
 * limit range to feed the dog. you can feed dog between CFG and
 * 0x40, can't be early or later, otherwise an interrupt will occur
 */

/* wwdg structure */
typedef struct 
{
	uint16 RESERVE0;
	volatile uint16 CR;
	uint16 RESERVE2;
	volatile uint16 CFR;
	uint16 RESERVE3;
	volatile uint16 SR;
}WWDG_TypeDef;

WWDG_TypeDef *WWDG = (WWDG_TypeDef *)WWDG_BASE;


/* wwdg bit band */
#define WWDG_OFFSET (WWDG_BASE - PERIPH_BASE)
/* CR register bit band */
#define CR_OFFSET (WWDG_OFFSET + 0x00)
#define CR_WDGA (PERIPH_BB_BASE + CR_OFFSET * 32 + 0x07 * 4)

/* CFR register bit band */
#define CFR_OFFSET (WWDG_OFFSET + 0x04)
#define CFR_EWI (PERIPH_BB_BASE + CFR_OFFSET * 32 + 0x09 * 4)

/* SR register bit band */
#define SR_OFFSET (WWDG_OFFSET + 0x08)
#define SR_EWIF (PERIPH_BB_BASE + SR_OFFSET * 32 + 0x01 * 4)

/* register operation bits definition */
#define CFR_WDGTB   (0x03 << 7)

static uint8 sCRCnt = 0;

/**
 * @brief startup window watchdog
 */
void WWDG_Startup(void)
{
    *((volatile uint32*)CR_WDGA) = 0x01;
}

/**
 * @brief feed dog
 */
void WWDG_Feed(void)
{
	uint8 curCnt = (WWDG->CR & 0x7f);
	if((curCnt > 0x3f) && (curCnt < (WWDG->CFR & 0x7f)))
	  WWDG->CR = sCRCnt;
}

/**
 * @brief set wwdg count, bit6 need to be 1 to avoid an immediate reset
 * @param cnt: timeout count
 */
void WWDG_SetCounter(__in uint8 cnt)
{
	assert_param(cnt <= 127);
    WWDG->CR = cnt;
	sCRCnt = cnt;
}

/**
 * @brief set wwdg timer base
 * @param base: timer base value
 */
void WWDG_SetTimerBase(__in uint8 base)
{
    assert_param(IS_WWDG_TIMEBASE_PARAM(base));

    WWDG->CFR &= ~CFR_WDGTB;
    WWDG->CFR |= base;
}

/**
 * @brief set window counter value, feed dog when CR is
 *        lower than CFG value
 * @param cnt: window counter value, it should be lower than
 *        CR counter value to prevent a immediate reset
 */
void WWDG_SetWindowCounter(__in uint8 cnt)
{
	assert_param(cnt <= 127);

    WWDG->CFR |= cnt;
}

/**
 * @brief enable early wakeup interrupt
 */
void WWDG_EnableEWI(void)
{
    *((volatile uint32*)CFR_EWI) = 0x01;
}

/**
 * @brief clear ewi flag
 */
void WWDG_ClrEWIFlag(void)
{
    *((volatile uint32*)SR_EWIF) = 0x00;
}

/**
 * @brief get ewi flag
 * @return true means set, false means clear
 */
BOOL WWDG_IsEWIFFlagSet(void)
{
    if(*((volatile uint32*)SR_EWIF) == 0x01)
        return TRUE;

    return FALSE;
}


