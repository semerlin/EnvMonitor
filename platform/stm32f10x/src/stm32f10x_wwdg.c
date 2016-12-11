#include "stm32f10x_wwdg.h"
#include "stm32f10x_map.h"
#include "stm32f10x_cfg.h"


typedef struct 
{
	uint16 RESERVE0;
    uint8 RESERVED1;
	volatile uint8 CR;
	uint16 RESERVE2;
	volatile uint16 CFR;
	uint16 RESERVE3;
    uint8 RESERVED4;
	volatile uint8 SR;
}WWDG_TypeDef;

/* CRC结构体地址初始化 */
WWDG_TypeDef *WWDG = (WWDG_TypeDef *)WWDG_BASE;


/****************************************************/
#define CFR_WDGTB   (0x03 << 7)

/**************************************************
*  WWDG寄存器位带别名区 
***************************************************/
#define WWDG_OFFSET (WWDG_BASE - PERIPH_BASE)
/*  CR寄存器位带别名区定义 */
#define CR_OFFSET (WWDG_OFFSET + 0x00)
#define CR_WDGA (PERIPH_BB_BASE + CR_OFFSET * 32 + 0x07 * 4)

/*  CFR寄存器位带别名区定义 */
#define CFR_OFFSET (WWDG_OFFSET + 0x08)
#define CFR_EWI (PERIPH_BB_BASE + CFR_OFFSET * 32 + 0x09 * 4)


/*  SR寄存器位带别名区定义 */
#define SR_OFFSET (WWDG_OFFSET + 0x08)
#define SR_EWIF (PERIPH_BB_BASE + SR_OFFSET * 32 + 0x01 * 4)



/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: none
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void WWDG_StartupWDT(void)
{
    WWDG->CR = 0x007f;
    *(volatile uint32*)CR_WDGA = 0x01;
}

/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: none
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void WWDG_SetCounter(__in uint8 cnt)
{
    cnt &= 0x7f;
    WWDG->CR |= cnt;
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: none
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void WWDG_SetTimerBase(__in uint8 base)
{
    assert_param(IS_WWDG_TIMEBASE_PARAM(base));

    WWDG->CFR &= ~CFR_WDGTB;
    WWDG->CFR |= base;
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: none
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void WWDG_SetWindowCounter(__in uint8 cnt)
{
    cnt &= 0x007f;

    WWDG->CFR |= cnt;
}


/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: none
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void WWDG_EnableEWI(void)
{
    *(volatile uint32*)CFR_EWI = 0x01;
}


/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: none
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void WWDG_ClrEWIFlag(void)
{
    *(volatile uint32*)SR_EWIF = 0x00;
}


/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: none
Author: hy
Version: V1.0
Others: none
*************************************************************************/
FlagStatus WWDG_GetEWIFFlag(void)
{
    if(*(volatile uint32*)SR_EWIF)
    {
        return SET;
    }

    return RESET;
}


