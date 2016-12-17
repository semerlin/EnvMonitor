#ifndef _STM32F10X_GPIO_H_
#define _STM32F10X_GPIO_H_

#include "sysdef.h"

/* gpio group definition */
typedef enum
{
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF,
    GPIOG,
    GPIO_Count
}GPIO_Group;

/* gpio configure definition */
typedef enum
{ 
  GPIO_Speed_10MHz = 1,
  GPIO_Speed_2MHz, 
  GPIO_Speed_50MHz
}GPIO_Speed;

typedef enum
{ GPIO_Mode_AIN = 0x0,
  GPIO_Mode_IN_FLOATING = 0x01,
  GPIO_Mode_IPD = 0x02,
  GPIO_Mode_IPU = 0x22,
  GPIO_Mode_Out_OD = 0x11,
  GPIO_Mode_Out_PP = 0x10,
  GPIO_Mode_AF_OD = 0x13,
  GPIO_Mode_AF_PP = 0x12
}GPIO_Mode;

     
typedef struct
{
  uint8 pin;
  GPIO_Speed speed;
  GPIO_Mode mode;  
}GPIO_Config;


/* interface */
void GPIO_Setup(__in GPIO_Group group, __in const GPIO_Config *config);
uint16 GPIO_ReadDataGroup(__in GPIO_Group group);
void GPIO_WriteDataGroup(__in GPIO_Group group, uint16 data);
uint8 GPIO_ReadPin(__in GPIO_Group group, __in uint8 pin);
void GPIO_SetPin(__in GPIO_Group group, __in uint8 pin);
void GPIO_ResetPin(__in GPIO_Group group, __in uint8 pin);
void GPIO_LockPin(__in GPIO_Group group, __in uint8 pin);




#endif

