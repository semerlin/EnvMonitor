#include "stm32f10x_flash.h"
#include "stm32f10x_map.h"
#include "stm32f10x_cfg.h"

/* flash register structure */
typedef struct 
{
	uint16 RESERVED0;
    uint8 RESERVED1; 
	volatile uint8 ACR;
	volatile uint32 KEYR;
	volatile uint32 OPTKEYR;
	uint16 RESERVED2;
    uint8 RESERVED3;
	volatile uint8 SR;
	uint16 RESERVED4;
	volatile uint16 CR;
	volatile uint32 AR;
    uint32 RESERVED5;
	volatile uint32 OBR;
	volatile uint32 WRPR;
}FLASH_TypeDef;

FLASH_TypeDef *FLASH = (FLASH_TypeDef *)FLASH_BASE;



/* key values */
#define RDPRT       0x00A5
#define KEY1        0x45670123
#define KEY2        0xCDEF89AB

/******************************************************/
//ACR latency 
#define ACR_LATENCY   0x03



/**************************************************
*  flash register's bit band area
***************************************************/
#define FLASH_OFFSET (FLASH_BASE - PERIPH_BASE)
/*  ACR bit band */
#define ACR_OFFSET (FLASH_OFFSET + 0x00)
#define ACR_PRFTBS (PERIPH_BB_BASE + ACR_OFFSET * 32 + 0x05 * 4)
#define ACR_PRFTBE (PERIPH_BB_BASE + ACR_OFFSET * 32 + 0x04 * 4)
#define ACR_HLFCYA (PERIPH_BB_BASE + ACR_OFFSET * 32 + 0x03 * 4)





/**
 * @brief enable flash prefetch
 * @param flag: TRUE: enable, FALSE: disable
 */
void Flash_EnablePrefetch(__in BOOL flag)
{
    if(flag)
        *((volatile uint32 *)ACR_PRFTBE) = 0x01;
    else
        *((volatile uint32 *)ACR_PRFTBE) = 0x00;
}


/**
 * @brief get flash prefetch status
 * @return flash prefetch is enabled if true, otherwise false
 */
BOOL FLASH_IsPrefetchEnabled(void)
{
    if(*((volatile uint32*)ACR_PRFTBS))
    {
        return TRUE;
    }

    return FALSE;
}


/**
 * @brief enable flash half cycle access, only use this when sysclk is lower
 *        than 8MHz
 * @param flag: TRUE: enable, FALSE: disable
 */
void FLASH_EnableHalfCycleAccess(__in BOOL flag)
{
    if(flag)
        *((volatile uint32*)ACR_HLFCYA) = 0x01;
    else
        *((volatile uint32*)ACR_HLFCYA) = 0x00;
}

/**
 * @brief get flash half cycle access status
 * @return flash half cycle access is enabled if true, otherwise false
 */
BOOL FLASH_IsHalfCycleAccessEnabled(void)
{
    if(*((volatile uint32*)ACR_HLFCYA))
    {
        return TRUE;
    }

    return FALSE;
}


/**
 * @brief set flash read latency
 * @param latency: latency value
 */
void FLASH_SetLatency(__in uint8 latency)
{
    assert_param(IS_FLASH_LATENCY_PARAM(latency));

    FLASH->ACR &= ~ACR_LATENCY;

    FLASH->ACR |= latency;
}

