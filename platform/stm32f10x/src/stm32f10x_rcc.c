#include "stm32f10x_rcc.h"
#include "stm32f10x_map.h"
#include "stm32f10x_cfg.h"


/* RCC structure definition */
typedef struct
{
    volatile uint32 CR;
    volatile uint32 CFGR;
    volatile uint32 CIR;
    volatile uint32 APB2RSTR;
    volatile uint32 APB1RSTR;
    uint16 RESERVED0;
    volatile uint16 AHBENR;
    volatile uint32 APB2ENR;
    volatile uint32 APB1ENR;
    volatile uint32 BDCR;
    volatile uint32 CSR;  
}RCC_TypeDef;

RCC_TypeDef *RCC = (RCC_TypeDef *)RCC_BASE;



/*  RCC register bit band */
#define RCC_OFFSET (RCC_BASE - PERIPH_BASE)
/*  CR register bit band */
#define CR_OFFSET (RCC_OFFSET + 0x00)
#define CR_HSION (PERIPH_BB_BASE + CR_OFFSET * 32 + 0x00 * 4)
#define CR_HSIRDY (PERIPH_BB_BASE + CR_OFFSET * 32 + 0x01 * 4)
#define CR_HSEON  (PERIPH_BB_BASE + CR_OFFSET * 32 + 16 * 4)
#define CR_HSERDY  (PERIPH_BB_BASE + CR_OFFSET * 32 + 17 * 4)
#define CR_HSEBYP  (PERIPH_BB_BASE + CR_OFFSET * 32 + 18 * 4)
#define CR_CSSON  (PERIPH_BB_BASE + CR_OFFSET * 32 + 19 * 4)
#define CR_PLLON (PERIPH_BB_BASE + CR_OFFSET * 32 + 24 * 4)
#define CR_PLLRDY  (PERIPH_BB_BASE + CR_OFFSET * 32 + 25 * 4)


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





/* osc ready cycle */
#define OSC_StableCycle    20

//CR register
#define CR_HSITRIM        (0x1f << 3)
#define CR_HSICAL         (0xff << 8)


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

#define HSI_CLOCK   (8000000)




/**
* @brief deinit rcc module
*/
void RCC_DeInit(void)
{
    RCC->CR = 0x000000f9;
    RCC->CFGR = 0x00000000;
    RCC->CIR = 0x009f0000;
    RCC->APB2RSTR = RCC_APB2_RESET_ALL;
    RCC->APB1RSTR = RCC_APB1_RESET_ALL;
    RCC->AHBENR = 0x00;
    RCC->APB2ENR = 0x00;
    RCC->APB1ENR = 0x00;
    RCC->BDCR = 0x00;
    RCC->CSR = (1 << 24);
}




/**
 * @brief startup internal high speed clock
 * @return TRUE: success FALSE: failed
 */
BOOL RCC_StartupHSI(void)
{
    uint32 waitCount = 0;
	volatile uint32 i = 0;
	
    //check if already enabled
    if((*((volatile uint32*)CR_HSION) == 0x01) && 
       (*((volatile uint32*)CR_HSIRDY) == 0x01))
    {
        return TRUE;
    }
    
    //start hsi
    *((volatile uint32*)CR_HSION) = 0x01;
    while((!(*((volatile uint32*)CR_HSIRDY))) && 
          (waitCount < OSC_StableCycle))
    {
        //wait for ready
        for(i = 0; i < 128; i++);
        waitCount ++;
    }

    if(waitCount >= OSC_StableCycle)
    {
        //start failed
        return FALSE;
    }


    return TRUE;  
}


/**
 * @brief stop hsi
 * @note this bit cannot be reset if the internal 8MHz RC is used directy or 
 *        indirecty as system clock or is selected to become the system clock
 */
void RCC_StopHSI(void)
{
    //stop hsi
    *((volatile uint32*)CR_HSION) = 0x00;
    
    //wait for stop
    volatile uint8 i = 0;
    for(i = 0; i < 128; i++);
}

/**
 * @brief get hsi calibration value
 * @return hsi calibration value
 */
uint8 RCC_GetHSICalValue(void)
{
    return ((RCC->CR & CR_HSICAL) >> 8);
}

/**
 * @brief set hsi trim value, this is a user-programmable trimming value that
 *        added to the CAL value. It can be programmed to adjust variations in 
 *        voltage and temperature that influence the frequency the HSI RC
 * @param value: trim value, this value cannot be bigger than 16
 */
void RCC_SetHSITrimValue(__in uint8 value)
{
    assert_param(value <= 16);
    uint16 tempVal = value;
    tempVal <<= 3;

    RCC->CR &= ~CR_HSITRIM;
    RCC->CR |= tempVal;
}

/**
 * @brief get HSI trim value
 * @return HSI trim value
 */
uint8 RCC_GetHSITrimValue(void)
{
    return ((RCC->CR >> 3) & 0x1f);
}


/**
 * @brief check if HSI is on
 * @return TRUE:on FALSE:off
 */
BOOL RCC_IsHSIOn(void)
{
    if(*((volatile uint32*)CR_HSION))
        return TRUE;

    return FALSE;
}


/**
 * @brief start up HSE
 * @return TRUE: success FALSE: failed
 */
BOOL RCC_StartupHSE(void)
{
    uint32 waitCount = 0;
	volatile uint16 i = 0;
	
    //check if already enabled
    if((*((volatile uint32*)CR_HSERDY) == 0x01) 
       && (*((volatile uint32*)CR_HSEON) == 0x01))
    {
        return TRUE;
    }
    
    //start up HSE
    *((volatile uint32*)CR_HSEON) = 0x01;
    while((!(*((volatile uint32*)CR_HSERDY))) && 
          (waitCount < OSC_StableCycle))
    {
        //wait for ready
        for(i = 0; i < 128; i++);
        waitCount ++;
    }

    if(waitCount >= OSC_StableCycle)
    {
        //startup failed
        return FALSE;
    }

    return TRUE;
}


/**
 * @brief stop HSE
 */
void RCC_StopHSE(void)
{
    //stop HSE
    *((volatile uint32*)CR_HSEON) = 0x00;
    
    //wait for stop
    volatile uint8 i = 0;
    for(i = 0; i < 128; i++);
}

/**
 * @brief bypass the oscillator with an external clock
 * @param flag TRUE:bypass FALSE: not bypass
 */
BOOL RCC_BypassHSE(__in BOOL flag)
{
    *((volatile uint32*)CR_HSEON) = 0x00;
    *((volatile uint32*)CR_HSEBYP) = 0x00;
    
    if(flag)
        *((volatile uint32*)CR_HSEBYP) = 0x01;

    //start HSE
    return RCC_StartupHSE();
}

/**
 * @brief check if HSE is on
 * @return TRUE:on FALSE:off
 */
BOOL RCC_IsHSEOn(void)
{
    if(*((volatile uint32*)CR_HSEON))
        return TRUE;

    return FALSE;
}

/**
 * @brief check if HSE is bypassed
 * @return TRUE: yes FALSE: no
 */
BOOL RCC_IsHSEBypassed(void)
{
    if(*((volatile uint32*)CR_HSEBYP))
        return TRUE;

    return FALSE;
}


/**
 * @brief enable or disable ccs. When CSSON is set, the clock detector is 
 *        enabled by hardware when the HSE oscillator is ready, and disabled by
 *        hardware if a HSE clock failure is detected
 * @param flag: TRUE:enable FALSE:disable
*/
void RCC_EnableClockSecurityConfig(__in BOOL flag)
{
    if(flag)
        *((volatile uint32*)CR_CSSON) = 0x01;
    else
        *((volatile uint32*)CR_CSSON) = 0x00;
}


/**
 * @brief startup pll
 * @return TRUE: success FALSE:failed
 */
BOOL RCC_StartupPLL(void)
{
    uint32 waitCount = 0;
	volatile uint16 i = 0;
    
    //check if pll has already been started
    if((*((volatile uint32*)CR_PLLRDY) == 0x01) && 
       (*(volatile uint32*)CR_PLLON == 0x01))
    {
        return TRUE;
    }

    //start pll
    *((volatile uint32*)CR_PLLON) = 0x01;

    while((!*((volatile uint32*)CR_PLLRDY)) && 
          (waitCount < OSC_StableCycle))
    {
        //wait for stable
        for(i = 0; i < 128; i++);
        waitCount ++;
    }

    if(waitCount >= OSC_StableCycle)
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief stop pll
 */
void RCC_StopPLL(void)
{
    *((volatile uint32*)CR_PLLON) = 0x00;
    
    //wait for stop
    volatile uint8 i = 0;
    for(i = 0; i < 128; i++); 
}


/**
 * @brief check if pll is on
 * @return TRUE: on FALSE: off
 */
BOOL RCC_GetPLLONFlag(void)
{
    if(*((volatile uint32*)CR_PLLON))
        return TRUE;

    return FALSE;
}



/**
 * @brief config mco output source
 * @param source
*/
void RCC_MCOConfig(__in uint32 method)
{
    assert_param(IS_RCC_MCO_PARAM(method));

    RCC->CFGR &= ~CFGR_MCO;

    RCC->CFGR |= method;
    
}

/**
 * @brief usb clock prescale config, usb clock is 48MHz
 * @param prescale value
 */
void RCC_USBPrescalerFromPLL(__in uint8 config)
{
    assert_param(IS_RCC_USBPRE_PARAM(config));

    *((volatile uint32*)CFGR_USBPRE) = config;
}

/**
 * @brief calculate pll factor
 * @param clockIn: input clock
 * @param clockOut: output clock
 * @param div: div factor
 * @param needDiv2: return whether need div by 2
 * @return acturally output clock 
 */
static uint32 calcPllFactor(__in uint32 clockIn, __in uint32 clockOut,
                            __out uint32 *div, __out uint8 *needDiv2)
{
    uint8 tempDiv = 0;
    if(clockOut % clockIn == 0)
    {
        tempDiv = clockOut / clockIn;
        *div = (tempDiv & 0xff);
        *needDiv2 = 0;
    }
    else
    {
        tempDiv = clockOut / clockIn;
        if(tempDiv > 16)
        {
            *div = (tempDiv & 0xff);
            *needDiv2 = 0;
        }
        else
        {
            uint32 delta1 = clockOut - tempDiv * clockIn;
            tempDiv = clockOut * 2 / clockIn;
            tempDiv &= 0xff;
            uint32 delta2 = clockOut - tempDiv * clockIn;
            if(delta1 > delta2)
            {
                *div = tempDiv;
                *needDiv2 = 1;
            }
            else
            {
                *div = clockOut / clockIn;
                *needDiv2 = 0;
            }
        }
    }
    
    return *div * clockIn / (*needDiv2 + 1);
}

/**
* @brief set sysclk use pll
* @param clock: output clock
* @param useHse: whether use hse or hsi
* @param hse clock
* @return acturally output clock 
*/
uint32 RCC_SetSysclkUsePLL(__in uint32 clock, __in BOOL useHSE, 
                              __in uint32 hseClock)
{
    assert_param(clock > hseClock);
    
    uint32 div = 0;
    uint8 needDiv2 = 0;
    uint32 ret;
    RCC->CFGR &= ~CFGR_PLLMUL;
    RCC->CFGR &= ~CFGR_SW;
    RCC->CFGR |= 0x02;
    if(useHSE)
    {
        *((volatile uint32 *)CFGR_PLLSRC) = 0x01;
        ret = calcPllFactor(hseClock, clock, &div, &needDiv2);
    }
    else
    {
        *((volatile uint32 *)CFGR_PLLSRC) = 0x00;
       ret = calcPllFactor(HSI_CLOCK / 2, clock, &div, &needDiv2); 
    }
    
    *((volatile uint32 *)CFGR_PLLXTPRE) = needDiv2;
    RCC->CFGR |= (div << 18);
    
    return ret;

}


/**
 * @brief set hclk prescaler
 * @param config: prescaler value 
 */
void RCC_HCLKPrescalerFromSYSCLK(__in uint8 config)
{
    assert_param(IS_RCC_HPRE_PARAM(config));

    RCC->CFGR &= ~CFGR_HPRE;
    RCC->CFGR |= config;
}

/**
 * @brief set pclk1 prescaler
 * @param config: prescaler value 
 */
void RCC_PCLK1PrescalerHCLK(__in uint32 config)
{
    assert_param(IS_RCC_PPRE1_PARAM(config));

    RCC->CFGR &= ~CFGR_PPRE1;
    RCC->CFGR |= config;
}

/**
 * @brief set pclk2 prescaler
 * @param config: prescaler value 
 */
void RCC_PCLK2PrescalerFromHCLK(__in uint32 config)
{
    assert_param(IS_RCC_PPRE2_PARAM(config));

    RCC->CFGR &= ~CFGR_PPRE2;
    RCC->CFGR |= config;
}


/**
 * @brief switch system clock source
 * @param source value
 */
void RCC_SystemClockSwitch(__in uint8 clock)
{
    assert_param(IS_RCC_SW_PARAM(clock));

    RCC->CFGR &= ~CFGR_SW;
    RCC->CFGR |= clock;
}
/**
 * @brief get system clock source prescaler
 * @return 0: HSI 1: HSE 2: PLL other: invalid 
 */
uint8 RCC_GetSystemClock(void)
{
    return ((RCC->CSR & CFGR_SWS)>>2);
}

/**
 * @brief set adc prescaler
 * @param config: prescaler value 
 */
void RCC_ADCPrescalerFromPCLK2(__in uint32 config)
{
    assert_param(IS_RCC_ADC_PARAM(config));
    
    RCC->CFGR &= ~CFGR_ADCPRE;
    RCC->CFGR |= config;
}


/**
 * @brief clear clock ready interrupt flag
 * @param clock source
 */
void RCC_ClrClockIntFlag(__in uint8 intSrc)
{
    if(intSrc == RCC_INT_ClockSecuty)
    {
        *(volatile uint32*)CIR_CSSC = 0x01;
    }
    else if(intSrc == RCC_INT_PLLReady)
    {
        *(volatile uint32*)CIR_PLLRDYC = 0x01;
    }
    else if(intSrc == RCC_INT_HSEReady)
    {
        *(volatile uint32*)CIR_HSERDYC = 0x01;
    }
    else if(intSrc == RCC_INT_HSIReady)
    {
        *(volatile uint32*)CIR_HSIRDYC = 0x01;
    }
    else if(intSrc == RCC_INT_LSEReady)
    {
        *(volatile uint32*)CIR_LSERDYC = 0x01;
    }
    else if(intSrc == RCC_INT_LSIReady)
    {
        *(volatile uint32*)CIR_LSIRDYC = 0x01;
    }
}

/**
* @brief enable or disable clock ready interrupt
* @param intSrc clock source
* @param flag enable flag
*/
void RCC_EnableClockInt(__in uint8 intSrc, BOOL flag)
{
    if(intSrc == RCC_INT_PLLReady)
    {
        *(volatile uint32*)CIR_PLLRDYIE = (uint8)flag;
    }
    else if(intSrc == RCC_INT_HSEReady)
    {
        *(volatile uint32*)CIR_HSERDYIE = (uint8)flag;
    }
    else if(intSrc == RCC_INT_HSIReady)
    {
        *(volatile uint32*)CIR_HSIRDYIE = (uint8)flag;
    }
    else if(intSrc == RCC_INT_LSEReady)
    {
        *(volatile uint32*)CIR_LSERDYIE = (uint8)flag;
    }
    else if(intSrc == RCC_INT_LSIReady)
    {
        *(volatile uint32*)CIR_LSIRDYIE = (uint8)flag;
    }

}

/**
 * @brief get clock ready interrupt flag
 * @return flag status
 */
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

/**
 * @brief reset APB2 periphearl clock
 * @param periphearl name
 */
void RCC_APB2PeriphReset(__in uint32 reg)
{
    RCC->APB2RSTR |= reg;
}

/**
 * @brief reset APB1 periphearl clock
 * @param periphearl name
 */
void RCC_APB1PeriphReset(__in uint32 reg)
{
    RCC->APB1RSTR |= reg;
}


/**
 * @brief enable AHB periphearl clock
 * @param reg: periphearl name
 * @param flag: enable flag
 */
void RCC_AHBPeripClockEnable(__in uint32 reg, __in BOOL flag)
{
    if(flag)
        RCC->AHBENR |= reg;
    else
        RCC->AHBENR &= ~reg;
}

/**
 * @brief enable APB2 periphearl clock
 * @param reg: periphearl name
 * @param flag: enable flag
 */
void RCC_APB2PeripClockEnable(__in uint16 reg, __in BOOL flag)
{
    if(flag)
        RCC->APB2ENR |= reg;
    else
        RCC->APB2ENR &= ~reg;
}


/**
 * @brief enable APB1 periphearl clock
 * @param reg: periphearl name
 * @param flag: enable flag
 */
void RCC_APB1PeripEnable(__in uint32 reg, __in BOOL flag)
{
    if(flag)
        RCC->APB1ENR |= reg;
    else
        RCC->APB1ENR &= ~reg; 
}


/**
 * @brief reset backup domain 
 * @param reset flag
 */
void RCC_BackUpRegisterReset(__in BOOL flag)
{
    if(flag)
        *(volatile uint32*)BDCR_BDRST = 0x01;
    else
        *(volatile uint32*)BDCR_BDRST = 0x00;  
}

/**
 * @brief check if rtc clock is enabled
 * @return enable status
 */
BOOL RCC_IsRTCEnabled(void)
{
    if(*(volatile uint32*)BDCR_RTCEN)
        return TRUE;

    return FALSE;
}

/**
 * @brief set rtc clock source
 * @param clock source
 */
void RCC_SetRTCClockSource(__in uint32 source)
{
    assert_param(IS_RTC_CLOCK_PARAM(source));
    RCC->BDCR &= ~BDCR_RTCSEL;
    RCC->BDCR |= source;
}

/**
 * @brief get rtc clock source
 * @return clock source
 */
uint8 RCC_GetRTCClockSource(void)
{
    return ((RCC->BDCR & BDCR_RTCSEL) >> 8);
}

/**
 * @brief startup rtc use lse
 * @return success flag
 */
BOOL RCC_StartupLSE(void)
{
    uint32 waitCount = 0;
	volatile uint32 i = 0;
	
    if((*(volatile uint32*)BDCR_LSERDY == 0x01) && 
       (*(volatile uint32*)BDCR_LSEON== 0x01))
    {
        return TRUE;
    }
    
    *(volatile uint32*)BDCR_LSEON = 0x01;
    while((!(*(volatile uint32*)BDCR_LSERDY)) && (waitCount < OSC_StableCycle))
    {
        for(i = 0; i < 128; i++);
        waitCount ++;
    }

    if(waitCount >= OSC_StableCycle)
    {
        return FALSE;
    }

    return TRUE;  

}

/**
 * @brief bypass lse
 * @param bypass flag
 */
BOOL RCC_BypassLSE(__in BOOL flag)
{
    *(volatile uint32*)BDCR_LSEON = 0x00;
    *(volatile uint32*)BDCR_LSEBYP = 0x00;
    
    if(flag)  //旁路HSE
        *(volatile uint32*)BDCR_LSEBYP = 0x01;

    return RCC_StartupHSE();
}


/**
 * @brief stop lse
 */
void RCC_StopLSE(void)
{
	volatile uint32 i = 0;
    
    *(volatile uint32*)BDCR_LSEON = 0x00;
    for(i = 0; i < 128; i++);
}


/**
 * @brief startup rtc use lsi
 * @return success flag
 */
BOOL RCC_StartupLSI(void)
{
    uint32 waitCount = 0;
	volatile uint32 i = 0;
	
    if((*(volatile uint32*)CSR_LSIRDY == 0x01) && 
       (*(volatile uint32*)CSR_LSION== 0x01))
    {
        return TRUE;
    }
    
    *(volatile uint32*)CSR_LSION = 0x01;
    while((!(*(volatile uint32*)CSR_LSIRDY)) && (waitCount < OSC_StableCycle))
    {
        for(i = 0; i < 128; i++);
        waitCount ++;
    }

    if(waitCount >= OSC_StableCycle)
    {
        return FALSE;
    }

    return TRUE;  

}
    
  
/**
 * @brief stop lsi
 */
void RCC_CloseLSI(void)
{
	volatile uint32 i = 0;
    
    *(volatile uint32*)CSR_LSION = 0x00;
    for(i = 0; i < 128; i++);
}


/**
 * @brief get reset flag
 * @return reset flag
 */
uint8 RCC_GetResetFlag(void)
{
    return ((RCC->CSR & CSR_RESET) >> 26) & 0xff;
}

/**
 * @brief clear reset flag
 */
void RCC_ClrResetFlag(void)
{
    *(volatile uint32*)CSR_RMVF = 0x01;
}


















































