#include "board.h"
#include "sysdef.h"
#include "stm32f10x_cfg.h"

static void clockConfig(void)
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
}

void board_init(void)
{
    clockConfig();
    
    RCC_APB2PeriphReset(RCC_APB2_RESET_IOPB);
    RCC_APB2PeriphReset(RCC_APB2_RESET_IOPE);

    RCC_APB2PeripClockEnable(RCC_APB2_ENABLE_IOPB, TRUE);
    RCC_APB2PeripClockEnable(RCC_APB2_ENABLE_IOPE, TRUE);
    
    GPIO_Config test = {5, GPIO_Speed_2MHz, GPIO_Mode_Out_OD};
    
    GPIO_Setup(GPIOB, &test);
    GPIO_Setup(GPIOE, &test);
    
    GPIO_SetPin(GPIOB, test.pin);
    GPIO_SetPin(GPIOE, test.pin);
    
    return;
}






#ifdef __DEBUG
void assert_failed(const char *file, unsigned int line)
{
}
#endif


#ifdef __ENABLE_TRACE
void Log(unsigned char level, const char *msg)
{
}
#endif