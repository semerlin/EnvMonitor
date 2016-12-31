#ifndef _PINCONFIG_H_
#define _PINCONFIG_H_

#include "sysdef.h"


void pinInit(void);
void pinSet(__in const char *name);
void pinReset(__in const char *name);
BOOL isPinSet(__in const char *name);
void getPinInfo(__in const char *name, __out uint8 *group, __out uint8 *num);

#endif