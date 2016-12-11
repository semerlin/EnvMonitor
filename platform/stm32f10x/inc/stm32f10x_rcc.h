#ifndef _STM32F10X_RCC_H_
  #define _STM32F10X_RCC_H_

#include "sysdef.h"



/************************************************************/
#define RCC_MCO_NONE    (0x00)
#define RCC_MCO_SYSCLK  (0x04 << 24)
#define RCC_MCO_HSI     (0x05 << 24)
#define RCC_MCO_HSE     (0x06 << 24)
#define RCC_MCO_PLL     (0x07 << 24)

#define IS_RCC_MCO_PARAM(param)  ((param == RCC_MCO_NONE) || (param == RCC_MCO_SYSCLK) || \
                                  (param == RCC_MCO_HSI) || (param == RCC_MCO_HSE) || \
                                  (param == RCC_MCO_PLL))



/**************************************************************/
#define RCC_USBPRE_1POINT5    (0x00)
#define RCC_USBPRE_DIRECT     (0x01)

#define IS_RCC_USBPRE_PARAM(param)  ((param == RCC_USBPRE_1POINT5) || (param == RCC_USBPRE_DIRECT))


/******************************************************/
#define RCC_SW_HSI      (0x00)
#define RCC_SW_HSE      (0x01)
#define RCC_SW_PLL      (0x02)
#define RCC_SW_INVALID  (0x03)

#define IS_RCC_SW_PARAM(param)  ((param == RCC_SW_HSI) || (param == RCC_SW_HSE) || \
                                  (param == RCC_SW_PLL) || (param == RCC_SW_INVALID))

/******************************************************/
#define RCC_HPRE_SYSCLK        (0x00)
#define RCC_HPRE_SYSCLK_DIV2       (0x08 << 4)
#define RCC_HPRE_SYSCLK_DIV4       (0x09 << 4)
#define RCC_HPRE_SYSCLK_DIV8       (0x0a << 4)
#define RCC_HPRE_SYSCLK_DIV16      (0x0b << 4)
#define RCC_HPRE_SYSCLK_DIV64      (0x0c << 4)
#define RCC_HPRE_SYSCLK_DIV128     (0x0d << 4)
#define RCC_HPRE_SYSCLK_DIV256     (0x0e << 4)
#define RCC_HPRE_SYSCLK_DIV512     (0x0f << 4)

#define IS_RCC_HPRE_PARAM(param) ((param == RCC_HPRE_SYSCLK) || (param == RCC_HPRE_SYSCLK_DIV2) || \
                                  (param == RCC_HPRE_SYSCLK_DIV4) || (param == RCC_HPRE_SYSCLK_DIV8) || \
                                  (param == RCC_HPRE_SYSCLK_DIV16) || (param == RCC_HPRE_SYSCLK_DIV64) || \
                                  (param == RCC_HPRE_SYSCLK_DIV128) || (param == RCC_HPRE_SYSCLK_DIV256) || \
                                  (param == RCC_HPRE_SYSCLK_DIV512))
                                  

/******************************************************/
#define RCC_PPRE1_HCLK        (0x00)
#define RCC_PPRE1_HCLK_DIV2       (0x04 << 8)
#define RCC_PPRE1_HCLK_DIV4       (0x05 << 8)
#define RCC_PPRE1_HCLK_DIV8       (0x06 << 8)
#define RCC_PPRE1_HCLK_DIV16      (0x07 << 8)


#define IS_RCC_PPRE1_PARAM(param) ((param == RCC_PPRE1_HCLK) || (param == RCC_PPRE1_HCLK_DIV2) || \
                                  (param == RCC_PPRE1_HCLK_DIV4) || (param == RCC_PPRE1_HCLK_DIV8) || \
                                  (param == RCC_PPRE1_HCLK_DIV16))


/******************************************************/
#define RCC_PPRE2_HCLK        (0x00)
#define RCC_PPRE2_HCLK_DIV2       (0x04 << 11)
#define RCC_PPRE2_HCLK_DIV4       (0x05 << 11)
#define RCC_PPRE2_HCLK_DIV8       (0x06 << 11)
#define RCC_PPRE2_HCLK_DIV16      (0x07 << 11)


#define IS_RCC_PPRE2_PARAM(param) ((param == RCC_PPRE2_HCLK) || (param == RCC_PPRE2_HCLK_DIV2) || \
                                  (param == RCC_PPRE2_HCLK_DIV4) || (param == RCC_PPRE2_HCLK_DIV8) || \
                                  (param == RCC_PPRE2_HCLK_DIV16))



/******************************************************/
#define RCC_ADC_PCLK_DIV2    (0x00)
#define RCC_ADC_PCLK_DIV4    (0x01 << 14)
#define RCC_ADC_PCLK_DIV6    (0x02 << 14)
#define RCC_ADC_PCLK_DIV8    (0x03 << 14)

#define IS_RCC_ADC_PARAM(param) ((param == RCC_ADC_PCLK_DIV2) || (param == RCC_ADC_PCLK_DIV4) || \
                                  (param == RCC_ADC_PCLK_DIV6) || (param == RCC_ADC_PCLK_DIV8))


/*******************************************************/
#define RCC_PLL_MUL2          (0x00)
#define RCC_PLL_MUL3          (0x01 << 18)
#define RCC_PLL_MUL4          (0x02 << 18)
#define RCC_PLL_MUL5          (0x03 << 18)
#define RCC_PLL_MUL6          (0x04 << 18)
#define RCC_PLL_MUL7          (0x05 << 18)
#define RCC_PLL_MUL8          (0x06 << 18)
#define RCC_PLL_MUL9          (0x07 << 18)
#define RCC_PLL_MUL10         (0x08 << 18)
#define RCC_PLL_MUL11         (0x09 << 18)
#define RCC_PLL_MUL12         (0x0a << 18)
#define RCC_PLL_MUL13         (0x0b << 18)
#define RCC_PLL_MUL14         (0x0c << 18)
#define RCC_PLL_MUL15         (0x0d << 18)
#define RCC_PLL_MUL16         (0x0e << 18)
#define RCC_PLL_MUL16_1       (0x0f << 18)

#define IS_RCC_PLLMUL_PARAM(param) ((param == RCC_PLL_MUL2) || (param == RCC_PLL_MUL3) || \
                                 (param == RCC_PLL_MUL4) || (param == RCC_PLL_MUL5)|| \
                                 (param == RCC_PLL_MUL6) || (param == RCC_PLL_MUL7)|| \
                                 (param == RCC_PLL_MUL8) || (param == RCC_PLL_MUL9)|| \
                                 (param == RCC_PLL_MUL10) || (param == RCC_PLL_MUL11)|| \
                                 (param == RCC_PLL_MUL12) || (param == RCC_PLL_MUL13)|| \
                                 (param == RCC_PLL_MUL14) || (param == RCC_PLL_MUL15)|| \
                                 (param == RCC_PLL_MUL16) || (param == RCC_PLL_MUL16_1))

/*******************************************************/
#define RCC_PLLXTPRE_HSE       (0x00)
#define RCC_PLLXTPRE_HSE_DIV2     (0x01)

#define IS_RCC_PLLXTPRE_PARAM(param) ((param == RCC_PLLXTPRE_HSE) || (param == RCC_PLLXTPRE_HSE_DIV2))


/*******************************************************/
#define RCC_PLLSRC_HSI       (0x00)
#define RCC_PLLSRC_HSE       (0x01)

#define IS_RCC_PLLSRC_PARAM(param) ((param == RCC_PLLSRC_HSI) || (param == RCC_PLLSRC_HSE))


/******************************************************/
#define RCC_INT_ClockSecuty      0x01
#define RCC_INT_PLLReady         0x02
#define RCC_INT_HSEReady         0x04
#define RCC_INT_HSIReady         0x08
#define RCC_INT_LSEReady         0x10
#define RCC_INT_LSIReady         0x20


/******************************************************/
#define RCC_AHB_DMA1           0x0001
#define RCC_AHB_DMA2           0x0002
#define RCC_AHB_SRAM           0x0004
#define RCC_AHB_FLITF          0x0010
#define RCC_AHB_CRC            0x0040
#define RCC_AHB_FSMC           0x0100
#define RCC_AHB_SDIO           0x0400

#define RCC_AHB_ALL            0x0557


/******************************************************/
#define RCC_APB2_AFIO         0x0001
#define RCC_APB2_IOPA         0x0004
#define RCC_APB2_IOPB         0x0008
#define RCC_APB2_IOPC         0x0010
#define RCC_APB2_IOPD         0x0020
#define RCC_APB2_IOPE         0x0040
#define RCC_APB2_IOPF         0x0080
#define RCC_APB2_IOPG         0x0100
#define RCC_APB2_ADC1         0x0200
#define RCC_APB2_ADC2         0x0400
#define RCC_APB2_TIM1         0x0800
#define RCC_APB2_SPI1         0x1000
#define RCC_APB2_TIM8         0x2000
#define RCC_APB2_USART1       0x4000
#define RCC_APB2_ADC3         0x8000

#define RCC_APB2_ALL          0xfffd



/******************************************************/
#define RCC_APB1_TIM2        0x00000001
#define RCC_APB1_TIM3        0x00000002
#define RCC_APB1_TIM4        0x00000004
#define RCC_APB1_TIM5        0x00000008
#define RCC_APB1_TIM6        0x00000010
#define RCC_APB1_TIM7        0x00000020
#define RCC_APB1_WWDG        0x00000800
#define RCC_APB1_SPI2        0x00004000
#define RCC_APB1_SPI3        0x00008000
#define RCC_APB1_UART2       0x00020000
#define RCC_RSAPB1_UART3       0x00040000
#define RCC_APB1_UART4       0x00080000
#define RCC_APB1_UART5       0x00100000
#define RCC_APB1_I2C1        0x00200000
#define RCC_APB1_I2C2        0x00400000
#define RCC_APB1_USB         0x00800000
#define RCC_APB1_CAN         0x02000000
#define RCC_APB1_BKP         0x08000000
#define RCC_APB1_PWR         0x10000000
#define RCC_APB1_DAC         0x20000000

#define RCC_APB1_ALL         0x3afec83f





/********************************************
* ½Ó¿Úº¯Êý
*********************************************/
void RCC_DeInit(void);
ErrorStatus RCC_StartupHSI(void);
ErrorStatus RCC_CloseHSI(void);
uint8 RCC_GetHSICalValue(void);
void RCC_SetHSITrimValue(uint8 value);
FlagStatus RCC_GetHSIONFlag(void);
FlagStatus RCC_GetHSIRDYFlag(void);
ErrorStatus RCC_StartupHSE(void);
ErrorStatus RCC_BypassHSE(__in FunctionStatus status);
ErrorStatus RCC_CloseHSE(void);
FlagStatus RCC_GetHSEBYPFlag(void);
FlagStatus RCC_GetHSERDYFlag(void);
FlagStatus RCC_GetHSEONFlag(void);
void RCC_ClockSecurityConfig(__in FunctionStatus status);
ErrorStatus RCC_StartupPLL(void);
ErrorStatus RCC_ClosePLL(void);
FlagStatus RCC_GetPLLRDYFlag(void);
FlagStatus RCC_GetPLLONFlag(void);
void RCC_MCOConfig(__in uint32 method);
void RCC_USBPrescalerConfig(__in uint8 config);
void RCC_SystemClockSwitch(__in uint8 clock);
uint8 RCC_GetSystemClock(void);
void RCC_AHBPrescalerConfig(__in uint8 config);
void RCC_APB1PrescalerConfig(__in uint32 config);
void RCC_APB2PrescalerConfig(__in uint32 config);
void RCC_ADCPrescalerConfig(__in uint32 config);
void RCC_PLLEntryClock(__in uint8 clock);
void RCC_HSEDividerForPLL(__in uint8 divider);
void RCC_SetPLLMultiFactor(__in uint32 factor);
void RCC_ClrClockIntFlag(__in uint8 intSrc);
void RCC_EnableClockInt(__in uint8 intSrc);
uint8 RCC_GetClockIntFlag(__in uint8 intSrc);
void RCC_APB2PeriphReset(__in uint16 reg);
void RCC_APB1PeriphReset(__in uint32 reg);
void RCC_AHBPeripEnable(__in uint16 reg);
void RCC_AHBPeripDisable(__in uint16 reg);
void RCC_APB2PeripEnable(__in uint16 reg);
void RCC_APB2PeripDisable(__in uint16 reg);
void RCC_APB1PeripEnable(__in uint32 reg);
void RCC_APB1PeripDisable(__in uint32 reg);
void RCC_BackUpRegisterReset(__in FunctionStatus status);
FlagStatus RCC_GetRTCStatus(void);
uint8 RCC_GetRTCClockSource(void);
ErrorStatus RCC_StartupLSE(void);
ErrorStatus RCC_BypassLSE(__in FunctionStatus status);
ErrorStatus RCC_CloseLSE(void);
ErrorStatus RCC_StartupLSI(void);
ErrorStatus RCC_CloseLSI(void);
uint8 RCC_GetResetFlag(void);
void RCC_ClrResetFlag(void);
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    



































#endif













