#include "stm32f10x_rcc.h"
#include "stm32f10x_map.h"
#include "stm32f10x_cfg.h"



typedef struct
{
    volatile uint32 CR;
    volatile uint32 CFGR;
    volatile uint32 CIR;
    uint16 RESERVE0;
    volatile uint16 APB2RSTR;
    volatile uint32 APB1RSTR;
    uint16 RESERVE1;
    volatile uint16 AHBENR;
    uint16 RESERVE2;
    volatile uint16 APB2ENR;
    volatile uint32 APB1ENR;
    volatile uint32 BDCR;
    volatile uint32 CSR;  
}RCC_TypeDef;

/* RCC寄存器结构体定义*/
RCC_TypeDef *RCC = (RCC_TypeDef *)RCC_BASE;

/******************************************************
* 晶振起振稳定周期
*******************************************************/
#define OSC_StableCycle    20

//CR寄存器
#define CR_HSICAL         (0xff << 8)
#define CR_HSITRIM        (0x1f << 3)


//CFGR寄存器
#define CFGR_MCO     (0x07 << 24)
#define CFGR_SW      (0x03)
#define CFGR_SWS     (0x03 << 2)
#define CFGR_HPRE    (0x0f << 4)
#define CFGR_PPRE1   (0x07 << 8)
#define CFGR_PPRE2   (0x07 << 11)
#define CFGR_ADCPRE  (0x03 << 14)
#define CFGR_PLLMUL  (0x0f << 18)

//BDCR寄存器
#define BDCR_RTCSEL  (0x03 << 8)

//CSR寄存器
#define CSR_RESET    (0x3f << 26)


/**************************************************
*  RCC寄存器位带别名区 
***************************************************/
#define RCC_OFFSET (RCC_BASE - PERIPH_BASE)
/*  CR寄存器位带别名区定义 */
#define CR_OFFSET (RCC_OFFSET + 0x00)
#define CR_HSION (PERIPH_BB_BASE + CR_OFFSET * 32 + 0x00 * 4)
#define CR_HSIRDY (PERIPH_BB_BASE + CR_OFFSET * 32 + 0x01 * 4)
#define CR_HSEON  (PERIPH_BB_BASE + CR_OFFSET * 32 + 16 * 4)
#define CR_HSERDY  (PERIPH_BB_BASE + CR_OFFSET * 32 + 17 * 4)
#define CR_HSEBYP  (PERIPH_BB_BASE + CR_OFFSET * 32 + 18 * 4)
#define CR_CSSON  (PERIPH_BB_BASE + CR_OFFSET * 32 + 19 * 4)
#define CR_PLLRDY  (PERIPH_BB_BASE + CR_OFFSET * 32 + 25 * 4)
#define CR_PLLON (PERIPH_BB_BASE + CR_OFFSET * 32 + 24 * 4)


/*  CFGRR寄存器位带别名区定义 */
#define CFGR_OFFSET (RCC_OFFSET + 0x04)
#define CFGR_USBPRE (PERIPH_BB_BASE + CFGR_OFFSET * 32 + 22 * 4)
#define CFGR_PLLSRC (PERIPH_BB_BASE + CFGR_OFFSET * 32 + 16 * 4)
#define CFGR_PLLXTPRE (PERIPH_BB_BASE + CFGR_OFFSET * 32 + 17 * 4)


/*  CIR寄存器位带别名区定义 */
#define CIR_OFFSET (RCC_OFFSET + 0x08)
#define CIR_CSSC (PERIPH_BB_BASE + CIR_OFFSET * 32 + 23 * 4)
#define CIR_PLLRDYC (PERIPH_BB_BASE + CIR_OFFSET * 32 + 20 * 4)
#define CIR_HSERDYC (PERIPH_BB_BASE + CIR_OFFSET * 32 + 19 * 4)
#define CIR_HSIRDYC (PERIPH_BB_BASE + CIR_OFFSET * 32 + 18 * 4)
#define CIR_LSERDYC (PERIPH_BB_BASE + CIR_OFFSET * 32 + 17 * 4)
#define CIR_LSIRDYC (PERIPH_BB_BASE + CIR_OFFSET * 32 + 16 * 4)
#define CIR_PLLRDYIE (PERIPH_BB_BASE + CIR_OFFSET * 32 + 12 * 4)
#define CIR_HSERDYIE (PERIPH_BB_BASE + CIR_OFFSET * 32 + 11 * 4)
#define CIR_HSIRDYIE (PERIPH_BB_BASE + CIR_OFFSET * 32 + 10 * 4)
#define CIR_LSERDYIE (PERIPH_BB_BASE + CIR_OFFSET * 32 + 9 * 4)
#define CIR_LSIRDYIE (PERIPH_BB_BASE + CIR_OFFSET * 32 + 8 * 4)
#define CIR_CSSF (PERIPH_BB_BASE + CIR_OFFSET * 32 + 7 * 4)
#define CIR_PLLRDYF (PERIPH_BB_BASE + CIR_OFFSET * 32 + 4 * 4)
#define CIR_HSERDYF (PERIPH_BB_BASE + CIR_OFFSET * 32 + 3 * 4)
#define CIR_HSIRDYF (PERIPH_BB_BASE + CIR_OFFSET * 32 + 2 * 4)
#define CIR_LSERDYF (PERIPH_BB_BASE + CIR_OFFSET * 32 + 1 * 4)
#define CIR_LSIRDYF (PERIPH_BB_BASE + CIR_OFFSET * 32 + 0 * 4)

/*  BDCR寄存器位带别名区定义 */
#define BDCR_OFFSET (RCC_OFFSET + 0x20)
#define BDCR_BDRST (PERIPH_BB_BASE + BDCR_OFFSET * 32 + 16 * 4)
#define BDCR_RTCEN (PERIPH_BB_BASE + BDCR_OFFSET * 32 + 15 * 4)
#define BDCR_LSEBYP (PERIPH_BB_BASE + BDCR_OFFSET * 32 + 2 * 4)
#define BDCR_LSERDY (PERIPH_BB_BASE + BDCR_OFFSET * 32 + 1 * 4)
#define BDCR_LSEON (PERIPH_BB_BASE + BDCR_OFFSET * 32 + 0 * 4)


/*  CSR寄存器位带别名区定义 */
#define CSR_OFFSET (RCC_OFFSET + 0x24)
#define CSR_RMVF (PERIPH_BB_BASE + CSR_OFFSET * 32 + 24 * 4)
#define CSR_LSIRDY (PERIPH_BB_BASE + CSR_OFFSET * 32 + 1 * 4)
#define CSR_LSION (PERIPH_BB_BASE + CSR_OFFSET * 32 + 0 * 4)








/************************************************************************
Function: void RCC_DeInit(void)
Description: 重新初始化RCC状态
Input: none
Output: none
Return: none
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_DeInit(void)
{
    RCC->CR = 0x000000f9;
    RCC->CFGR = 0x00000000;
    RCC->CIR = 0x009f0000;
    RCC->APB2RSTR = RCC_APB2_ALL;
    RCC->APB1RSTR = RCC_APB1_ALL;
    RCC->AHBENR = 0x00;
    RCC->APB2ENR = 0x00;
    RCC->APB1ENR = 0x00;
    RCC->BDCR = 0x00;
    RCC->CSR = (1 << 24);
}



/************************************************************************
Function: void RCC_EnableHSI(__in FunctionStatus status)
Description: 使能内部高速时钟
Input: none
Output: none
Return: ERROR: 关闭失败
           SUCCESS: 关闭成功
Author: hy
Version: V1.0
Others: none
*************************************************************************/
ErrorStatus RCC_StartupHSI(void)
{
    uint32 waitCount = 0;
	volatile uint32 i = 0;
	
    if((*(volatile uint32*)CR_HSION == 0x01) && (*(volatile uint32*)CR_HSIRDY == 0x01))  //已经被使能
    {
        return SUCCESS;
    }
    
    *(volatile uint32*)CR_HSION = 0x01;
    while((!(*(volatile uint32*)CR_HSIRDY)) && (waitCount < OSC_StableCycle))  //时钟没有就绪
    {
        for(i = 0; i < 256; i++);  //等待一段时间
        waitCount ++;
    }

    if(waitCount >= OSC_StableCycle)  //起振失败
    {
        return ERROR;
    }


    return SUCCESS;  
}

/************************************************************************
Function: void RCC_EnableHSI(__in FunctionStatus status)
Description: 关闭内部高速时钟
Input: none
Output: none
Return: ERROR: 关闭失败
           SUCCESS: 关闭成功
Author: hy
Version: V1.0
Others: 当内部8MHz振荡器被直接或间接地作或被选择将要作为系统时钟时，该位不能被清零
*************************************************************************/
ErrorStatus RCC_CloseHSI(void)
{
    uint32 waitCount = 0;
	volatile uint32 i = 0;
	
    if((*(volatile uint32*)CR_HSION == 0x00) && (*(volatile uint32*)CR_HSIRDY == 0x00))
    {
        return SUCCESS;
    }
    
    *(volatile uint32*)CR_HSION = 0x00;

    while((*(volatile uint32*)CR_HSIRDY) && (waitCount < OSC_StableCycle))  //时钟没有就绪
    {
        for(i = 0; i < 256; i++);  //等待一段时间
        waitCount ++;
    }

    if(waitCount >= OSC_StableCycle)  //关闭失败
    {
        return ERROR;
    }

    return SUCCESS;
}



/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
uint8 RCC_GetHSICalValue(void)
{
    return ((RCC->CR & CR_HSICAL) >> 8);
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_SetHSITrimValue(uint8 value)
{
    uint16 tempVal = 0;
    tempVal = (value & 0x1f);
    tempVal <<= 3;

    RCC->CR &= ~CR_HSITRIM;
    RCC->CR |= tempVal;
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
FlagStatus RCC_GetHSIONFlag(void)
{
    if(*(volatile uint32*)CR_HSION)
    {
        return SET;
    }

    return RESET;
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
FlagStatus RCC_GetHSIRDYFlag(void)
{
    if(*(volatile uint32*)CR_HSIRDY)
    {
        return SET;
    }

    return RESET;
}


/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
ErrorStatus RCC_StartupHSE(void)
{
    uint32 waitCount = 0;
	volatile uint16 i = 0;
	
    if((*(volatile uint32*)CR_HSERDY == 0x01) && (*(volatile uint32*)CR_HSEON== 0x01))  //已经被使能
    {
        return SUCCESS;
    }
    
    *(volatile uint32*)CR_HSEON = 0x01;
    while((!(*(volatile uint32*)CR_HSERDY)) && (waitCount < OSC_StableCycle))  //时钟没有就绪
    {
        for(i = 0; i < 256; i++);  //等待一段时间
        waitCount ++;
    }

    if(waitCount >= OSC_StableCycle)  //起振失败
    {
        return ERROR;
    }

    return SUCCESS;  

}

/************************************************************************
Function: void RCC_EnableHSI(__in FunctionStatus status)
Description: 关闭内部高速时钟
Input: none
Output: none
Return: ERROR: 关闭失败
           SUCCESS: 关闭成功
Author: hy
Version: V1.0
Others: 当内部8MHz振荡器被直接或间接地作或被选择将要作为系统时钟时，该位不能被清零
*************************************************************************/
ErrorStatus RCC_BypassHSE(__in FunctionStatus status)
{
    *(volatile uint32*)CR_HSEON = 0x00;
    *(volatile uint32*)CR_HSEBYP = 0x00;
    
    if(status)  //旁路HSE
    {
        *(volatile uint32*)CR_HSEBYP = 0x01;
        return RCC_StartupHSE();
    }

    return RCC_StartupHSE();
}


/************************************************************************
Function: void RCC_EnableHSI(__in FunctionStatus status)
Description: 关闭内部高速时钟
Input: none
Output: none
Return: ERROR: 关闭失败
           SUCCESS: 关闭成功
Author: hy
Version: V1.0
Others: 当内部8MHz振荡器被直接或间接地作或被选择将要作为系统时钟时，该位不能被清零
*************************************************************************/
ErrorStatus RCC_CloseHSE(void)
{
    uint32 waitCount = 0;
	volatile uint16 i = 0;
    if((*(volatile uint32*)CR_HSEON == 0x00) && (*(volatile uint32*)CR_HSERDY == 0x00))
    {
        return SUCCESS;
    }
    
    *(volatile uint32*)CR_HSEON = 0x00;

    while((*(volatile uint32*)CR_HSERDY) && (waitCount < OSC_StableCycle))  //时钟没有就绪
    {
        for(i = 0; i < 256; i++);  //等待一段时间
        waitCount ++;
    }

    if(waitCount >= OSC_StableCycle)  //关闭失败
    {
        return ERROR;
    }

    return SUCCESS;
}

/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
FlagStatus RCC_GetHSEBYPFlag(void)
{
    if(*(volatile uint32*)CR_HSEBYP)
    {
        return SET;
    }

    return RESET;
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
FlagStatus RCC_GetHSERDYFlag(void)
{
    if(*(volatile uint32*)CR_HSERDY)
    {
        return SET;
    }

    return RESET;
}

/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
FlagStatus RCC_GetHSEONFlag(void)
{
    if(*(volatile uint32*)CR_HSEON)
    {
        return SET;
    }

    return RESET;
}

/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_ClockSecurityConfig(__in FunctionStatus status)
{
    if(status)
    {
        *(volatile uint32*)CR_CSSON = 0x01;
    }
    else
    {
        *(volatile uint32*)CR_CSSON = 0x00;
    }
}

/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
ErrorStatus RCC_StartupPLL(void)
{
    uint32 waitCount = 0;
	volatile uint16 i = 0;
    if((*(volatile uint32*)CR_PLLRDY == 0x01) && (*(volatile uint32*)CR_PLLON == 0x01)) //PLL已经启动
    {
        return SUCCESS;
    }

    *(volatile uint32*)CR_PLLON = 0x01;

    while((!*(volatile uint32*)CR_PLLRDY) && (waitCount < OSC_StableCycle))  //时钟没有就绪
    {
        for(i = 0; i < 256; i++);  //等待一段时间
        waitCount ++;
    }

    if(waitCount >= OSC_StableCycle)  //启动失败
    {
        return ERROR;
    }

    return SUCCESS;
    
}

/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
ErrorStatus RCC_ClosePLL(void)
{
    uint32 waitCount = 0;
	volatile uint16 i = 0; 
    if((*(volatile uint32*)CR_PLLRDY == 0x00) && (*(volatile uint32*)CR_PLLON == 0x00)) //PLL已经关闭
    {
        return SUCCESS;
    }

    *(volatile uint32*)CR_PLLON = 0x00;

    while((*(volatile uint32*)CR_PLLRDY) && (waitCount < OSC_StableCycle))  //时钟没有就绪
    {
        for(i = 0; i < 256; i++);  //等待一段时间
        waitCount ++;
    }

    if(waitCount >= OSC_StableCycle)  //启动失败
    {
        return ERROR;
    }

    return SUCCESS;
    
}


/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
FlagStatus RCC_GetPLLRDYFlag(void)
{
    if(*(volatile uint32*)CR_PLLRDY)
    {
        return SET;
    }

    return RESET;
}

/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
FlagStatus RCC_GetPLLONFlag(void)
{
    if(*(volatile uint32*)CR_PLLON)
    {
        return SET;
    }

    return RESET;
}


/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_MCOConfig(__in uint32 method)
{
    assert_param(IS_RCC_MCO_PARAM(method));

    RCC->CSR &= ~CFGR_MCO;

    RCC->CSR |= method;
    
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_USBPrescalerConfig(__in uint8 config)
{
    assert_param(IS_RCC_USBPRE_PARAM(config));

    *(volatile uint32*)CFGR_USBPRE = config;
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_SystemClockSwitch(__in uint8 clock)
{
    assert_param(IS_RCC_SW_PARAM(clock));

    RCC->CSR &= ~CFGR_SW;
    RCC->CSR |= clock;
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
uint8 RCC_GetSystemClock(void)
{
    return ((RCC->CSR & CFGR_SWS)>>2);
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_AHBPrescalerConfig(__in uint8 config)
{
    assert_param(IS_RCC_HPRE_PARAM(config));

    RCC->CSR &= ~CFGR_HPRE;
    RCC->CSR |= config;
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_APB1PrescalerConfig(__in uint32 config)
{
    assert_param(IS_RCC_PPRE1_PARAM(config));

    RCC->CSR &= ~CFGR_PPRE1;
    RCC->CSR |= config;
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_APB2PrescalerConfig(__in uint32 config)
{
    assert_param(IS_RCC_PPRE2_PARAM(config));

    RCC->CSR &= ~CFGR_PPRE2;
    RCC->CSR |= config;
}

/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_ADCPrescalerConfig(__in uint32 config)
{
    assert_param(IS_RCC_ADC_PARAM(config));
    
    RCC->CSR &= ~CFGR_ADCPRE;
    RCC->CSR |= config;
}

/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_PLLEntryClock(__in uint8 clock)
{
    assert_param(IS_RCC_PLLSRC_PARAM(clock));

    *(volatile uint32*)CFGR_PLLSRC = clock;
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_HSEDividerForPLL(__in uint8 divider)
{
    assert_param(IS_RCC_PLLXTPRE_PARAM(divider));

    *(volatile uint32*)CFGR_PLLXTPRE = divider;
}

/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_SetPLLMultiFactor(__in uint32 factor)
{
    assert_param(IS_RCC_PLLMUL_PARAM(factor));

    RCC->CSR &= ~CFGR_PLLMUL;
    RCC->CSR |= factor;
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_ClrClockIntFlag(__in uint8 intSrc)
{
    if(intSrc == RCC_INT_ClockSecuty)
    {
        *(volatile uint32*)CIR_CSSC = 0x01;
    }

    if(intSrc == RCC_INT_PLLReady)
    {
        *(volatile uint32*)CIR_PLLRDYC = 0x01;
    }

    if(intSrc == RCC_INT_HSEReady)
    {
        *(volatile uint32*)CIR_HSERDYC = 0x01;
    }

    if(intSrc == RCC_INT_HSIReady)
    {
        *(volatile uint32*)CIR_HSIRDYC = 0x01;
    }

    if(intSrc == RCC_INT_LSEReady)
    {
        *(volatile uint32*)CIR_LSERDYC = 0x01;
    }

    if(intSrc == RCC_INT_LSIReady)
    {
        *(volatile uint32*)CIR_LSIRDYC = 0x01;
    }
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_EnableClockInt(__in uint8 intSrc)
{
    if(intSrc == RCC_INT_PLLReady)
    {
        *(volatile uint32*)CIR_PLLRDYIE = 0x01;
    }

    if(intSrc == RCC_INT_HSEReady)
    {
        *(volatile uint32*)CIR_HSERDYIE = 0x01;
    }

    if(intSrc == RCC_INT_HSIReady)
    {
        *(volatile uint32*)CIR_HSIRDYIE = 0x01;
    }

    if(intSrc == RCC_INT_LSEReady)
    {
        *(volatile uint32*)CIR_LSERDYIE = 0x01;
    }

    if(intSrc == RCC_INT_LSIReady)
    {
        *(volatile uint32*)CIR_LSIRDYIE = 0x01;
    }

}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
uint8 RCC_GetClockIntFlag(__in uint8 intSrc)
{
    uint8 flag = 0;
    
    if(*(volatile uint32*)CIR_CSSF)
    {
        flag |= RCC_INT_ClockSecuty;
    }

    if(*(volatile uint32*)CIR_PLLRDYF)
    {
        flag |= RCC_INT_PLLReady;
    }

    if(*(volatile uint32*)CIR_HSERDYF)
    {
        flag |= RCC_INT_HSEReady;
    }

    if(*(volatile uint32*)CIR_HSIRDYF)
    {
        flag |= RCC_INT_HSIReady;
    }

    if(*(volatile uint32*)CIR_LSERDYF)
    {
        flag |= RCC_INT_LSEReady;
    }

    if(*(volatile uint32*)CIR_LSIRDYF)
    {
        flag |= RCC_INT_LSIReady;
    }
	
	return flag;
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_APB2PeriphReset(__in uint16 reg)
{
    RCC->APB2RSTR |= reg;
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_APB1PeriphReset(__in uint32 reg)
{
    RCC->APB1RSTR |= reg;
}



/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_AHBPeripEnable(__in uint16 reg)
{
    RCC->AHBENR |= reg;
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_AHBPeripDisable(__in uint16 reg)
{
    RCC->AHBENR &= ~reg;
}

/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_APB2PeripEnable(__in uint16 reg)
{
    RCC->APB2ENR |= reg;
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_APB2PeripDisable(__in uint16 reg)
{
    RCC->APB2ENR &= ~reg;
}


/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_APB1PeripEnable(__in uint32 reg)
{
    RCC->APB1ENR |= reg;
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_APB1PeripDisable(__in uint32 reg)
{
    RCC->APB1ENR &= ~reg;
}

/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
void RCC_BackUpRegisterReset(__in FunctionStatus status)
{
    if(status)
    {
        *(volatile uint32*)BDCR_BDRST = 0x01;
    }
    else
    {
        *(volatile uint32*)BDCR_BDRST = 0x00;  
    }
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
FlagStatus RCC_GetRTCStatus(void)
{
    if(*(volatile uint32*)BDCR_RTCEN)
    {
        return SET;
    }

    return RESET;
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
uint8 RCC_GetRTCClockSource(void)
{
    return ((RCC->BDCR & BDCR_RTCSEL) >> 8);
}
/************************************************************************
Function: 
Description: 
Input:  none
Output: none
Return: 
Author: hy
Version: V1.0
Others: none
*************************************************************************/
ErrorStatus RCC_StartupLSE(void)
{
    uint32 waitCount = 0;
	volatile uint32 i = 0;
	
    if((*(volatile uint32*)BDCR_LSERDY == 0x01) && (*(volatile uint32*)BDCR_LSEON== 0x01))  //已经被使能
    {
        return SUCCESS;
    }
    
    *(volatile uint32*)BDCR_LSEON = 0x01;
    while((!(*(volatile uint32*)BDCR_LSERDY)) && (waitCount < OSC_StableCycle))  //时钟没有就绪
    {
        for(i = 0; i < 256; i++);  //等待一段时间
        waitCount ++;
    }

    if(waitCount >= OSC_StableCycle)  //起振失败
    {
        return ERROR;
    }

    return SUCCESS;  

}

/************************************************************************
Function: void RCC_EnableHSI(__in FunctionStatus status)
Description: 关闭内部高速时钟
Input: none
Output: none
Return: ERROR: 关闭失败
           SUCCESS: 关闭成功
Author: hy
Version: V1.0
Others: 当内部8MHz振荡器被直接或间接地作或被选择将要作为系统时钟时，该位不能被清零
*************************************************************************/
ErrorStatus RCC_BypassLSE(__in FunctionStatus status)
{
    *(volatile uint32*)BDCR_LSEON = 0x00;
    *(volatile uint32*)BDCR_LSEBYP = 0x00;
    
    if(status)  //旁路HSE
    {
        *(volatile uint32*)BDCR_LSEBYP = 0x01;
        return RCC_StartupHSE();
    }

    return RCC_StartupHSE();
}


/************************************************************************
Function: void RCC_EnableHSI(__in FunctionStatus status)
Description: 关闭内部高速时钟
Input: none
Output: none
Return: ERROR: 关闭失败
           SUCCESS: 关闭成功
Author: hy
Version: V1.0
Others: 当内部8MHz振荡器被直接或间接地作或被选择将要作为系统时钟时，该位不能被清零
*************************************************************************/
ErrorStatus RCC_CloseLSE(void)
{
    uint32 waitCount = 0;
	volatile uint32 i = 0;

    if((*(volatile uint32*)BDCR_LSEON == 0x00) && (*(volatile uint32*)BDCR_LSERDY == 0x00))
    {
        return SUCCESS;
    }
    
    *(volatile uint32*)BDCR_LSEON = 0x00;

    while((*(volatile uint32*)BDCR_LSERDY) && (waitCount < OSC_StableCycle))  //时钟没有就绪
    {
        for(i = 0; i < 256; i++);  //等待一段时间
        waitCount ++;
    }

    if(waitCount >= OSC_StableCycle)  //关闭失败
    {
        return ERROR;
    }

    return SUCCESS;
}


/************************************************************************
Function: void RCC_EnableHSI(__in FunctionStatus status)
Description: 关闭内部高速时钟
Input: none
Output: none
Return: ERROR: 关闭失败
           SUCCESS: 关闭成功
Author: hy
Version: V1.0
Others: 当内部8MHz振荡器被直接或间接地作或被选择将要作为系统时钟时，该位不能被清零
*************************************************************************/
ErrorStatus RCC_StartupLSI(void)
{
    uint32 waitCount = 0;
	volatile uint32 i = 0;
	
    if((*(volatile uint32*)CSR_LSIRDY == 0x01) && (*(volatile uint32*)CSR_LSION== 0x01))  //已经被使能
    {
        return SUCCESS;
    }
    
    *(volatile uint32*)CSR_LSION = 0x01;
    while((!(*(volatile uint32*)CSR_LSIRDY)) && (waitCount < OSC_StableCycle))  //时钟没有就绪
    {
        for(i = 0; i < 256; i++);  //等待一段时间
        waitCount ++;
    }

    if(waitCount >= OSC_StableCycle)  //起振失败
    {
        return ERROR;
    }

    return SUCCESS;  

}
    
  
    
/************************************************************************
Function: void RCC_EnableHSI(__in FunctionStatus status)
Description: 关闭内部高速时钟
Input: none
Output: none
Return: ERROR: 关闭失败
           SUCCESS: 关闭成功
Author: hy
Version: V1.0
Others: 当内部8MHz振荡器被直接或间接地作或被选择将要作为系统时钟时，该位不能被清零
*************************************************************************/
ErrorStatus RCC_CloseLSI(void)
{
    uint32 waitCount = 0;
	volatile uint32 i = 0;
	
    if((*(volatile uint32*)CSR_LSION == 0x00) && (*(volatile uint32*)CSR_LSIRDY == 0x00))
    {
        return SUCCESS;
    }
    
    *(volatile uint32*)CSR_LSION = 0x00;

    while((*(volatile uint32*)CSR_LSIRDY) && (waitCount < OSC_StableCycle))  //时钟没有就绪
    {
        for(i = 0; i < 256; i++);  //等待一段时间
        waitCount ++;
    }

    if(waitCount >= OSC_StableCycle)  //关闭失败
    {
        return ERROR;
    }

    return SUCCESS;
}


/************************************************************************
Function: void RCC_EnableHSI(__in FunctionStatus status)
Description: 关闭内部高速时钟
Input: none
Output: none
Return: ERROR: 关闭失败
           SUCCESS: 关闭成功
Author: hy
Version: V1.0
Others: 当内部8MHz振荡器被直接或间接地作或被选择将要作为系统时钟时，
        该位不能被清零
*************************************************************************/
uint8 RCC_GetResetFlag(void)
{
    return ((RCC->CSR & CSR_RESET) >> 26);
}


/************************************************************************
Function: void RCC_EnableHSI(__in FunctionStatus status)
Description: 关闭内部高速时钟
Input: none
Output: none
Return: ERROR: 关闭失败
           SUCCESS: 关闭成功
Author: hy
Version: V1.0
Others: 当内部8MHz振荡器被直接或间接地作或被选择将要作为系统时钟时，该位不能被清零
*************************************************************************/
void RCC_ClrResetFlag(void)
{
    *(volatile uint32*)CSR_RMVF = 0x01;
}


















































