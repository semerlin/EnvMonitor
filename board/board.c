#include "board.h"
#include "sysdef.h"
#include "stm32f10x_cfg.h"
#include "pinconfig.h"
#include "string.h"

static void clockInit(void);
static void usartInit(void);
static void miscInit(void);

//init function
typedef void (*initFuc)(void);

//init sequence
initFuc initSequence[] = 
{
    clockInit,
    pinInit,
    usartInit,
    miscInit,
};

/**
 * @brief init board
 */
void board_init(void)
{
    uint32 len = sizeof(initSequence) / sizeof(initFuc);
    for(int i = 0; i < len; ++i)
    {
        assert_param(initSequence[i] != NULL);
        initSequence[i]();
        //TODO put some log information here
    }

    return;
}

/**
 * @brief board clock init
 */
static void clockInit(void)
{
    //config rcc
    RCC_DeInit();
    BOOL flag = RCC_StartupHSE();
    
    //config flash latency
    FLASH_SetLatency(FLASH_LATENCY_TWO);
    Flash_EnablePrefetch(TRUE);
    
    //config HCLK(72MHz), PCLK1(36MHz), PCLK2(72MHz)
    RCC_HCLKPrescalerFromSYSCLK(RCC_HPRE_SYSCLK);
    RCC_PCLK1PrescalerHCLK(RCC_PPRE1_HCLK_DIV2);
    RCC_PCLK2PrescalerFromHCLK(RCC_PPRE2_HCLK);
    
    //config PLL(72MHz)
    uint32 retVal = RCC_SetSysclkUsePLL(72000000, TRUE, 8000000);
    RCC_SystemClockSwitch(RCC_SW_PLL);
    //Wait till PLL is used as system clock source
	while( RCC_GetSystemClock() != 0x02);
    
    //setup interrupt grouping, we only use group priority
    SCB_SetPriorityGrouping(3);
}

/**
 * @brief board usart init
 */
static void usartInit(void)
{
    USART_Config config;
        
    USART_StructInit(&config);
    USART_Setup(USART1, &config);
    USART_EnableInt(USART1, USART_IT_RXNE, TRUE);
    
    NVIC_Config nvicConfig = {USART1_IRQChannel, 15, 0, TRUE};
    NVIC_Init(&nvicConfig);
    
    USART_Enable(USART1, TRUE);
    USART_WriteData(USART1, 0x34);
}

/**
 * @brief board misc devices init
 */
static void miscInit(void)
{
    powerEnable(TRUE);
}




#ifdef __DEBUG
int len = 0;
void assert_failed(const char *file, const char *line, const char *exp)
{
    len = strlen(file);
    for(int i = 0; i < len; ++i)
        USART_WriteData(USART1, file[i]);
    USART_WriteData(USART1, ':');
    len = strlen(line);
    for(int i = 0; i < len; ++i)
        USART_WriteData(USART1, line[i]);
    USART_WriteData(USART1, ' ');
    len = strlen(exp);
    for(int i = 0; i < len; ++i)
        USART_WriteData(USART1, exp[i]);
    while(1);
}
#endif


#ifdef __ENABLE_TRACE
void Log(unsigned char level, const char *msg)
{
}
#endif