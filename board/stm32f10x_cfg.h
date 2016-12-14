#ifndef _STM32F10x_CFG_H
#define _STM32F10x_CFG_H

#include "assert.h"



/**********************************************************
* library module inclue configure
***********************************************************/
/*********************************************************/
#define _MODULE_GPIO
#define _MODULE_POWER
#define _MODULE_CRC



/**********************************************************/
#ifdef _MODULE_GPIO
  #include "stm32f10x_gpio.h" 
#endif






#endif

