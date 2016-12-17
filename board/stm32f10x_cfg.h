#ifndef _STM32F10x_CFG_H
#define _STM32F10x_CFG_H

#define __DEBUG
#include "assert.h"



/**********************************************************
* library module inclue configure
***********************************************************/
/*********************************************************/
#define _MODULE_RCC
#define _MODULE_FLASH
#define _MODULE_GPIO
#define _MODULE_USART


/**********************************************************/
#ifdef _MODULE_CRC
  #include "stm32f10x_crc.h" 
#endif

#ifdef _MODULE_FLASH
  #include "stm32f10x_flash.h" 
#endif

#ifdef _MODULE_RCC
  #include "stm32f10x_rcc.h" 
#endif

#ifdef _MODULE_GPIO
  #include "stm32f10x_gpio.h" 
#endif

#ifdef _MODULE_USART
  #include "stm32f10x_usart.h" 
#endif





#endif

