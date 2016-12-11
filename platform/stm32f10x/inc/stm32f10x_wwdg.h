#ifndef _LIB_WWDG_STM32_H_
  #define _LIB_WWDG_STM32_H_

#include "sysdef.h"




#define WWDG_TIMEBASE_DIV1       (0x00)
#define WWDG_TIMEBASE_DIV2       (0x01 << 7)   
#define WWDG_TIMEBASE_DIV4       (0x02 << 7)  
#define WWDG_TIMEBASE_DIV8       (0x03 << 7)   


#define IS_WWDG_TIMEBASE_PARAM(param)  ((param == WWDG_TIMEBASE_DIV1) || (param == WWDG_TIMEBASE_DIV2) || \
                                        (param == WWDG_TIMEBASE_DIV4) || (param == WWDG_TIMEBASE_DIV8))




/***********************************************************
*  WWDG½Ó¿Úº¯Êý
************************************************************/
void WWDG_StartupWDT(void);
void WWDG_SetCounter(__in uint8 cnt);
void WWDG_SetTimerBase(__in uint8 base);
void WWDG_SetWindowCounter(__in uint8 cnt);
void WWDG_EnableEWI(void);
void WWDG_ClrEWIFlag(void);
FlagStatus WWDG_GetEWIFFlag(void);























#endif



