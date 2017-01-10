#ifndef _BH1750_H_
#define _BH1750_H_

#include "sysdef.h"

void bh1750Init(__in Handle i2c);
void bh1750Start(void);
uint32 bh1750GetLight(void);

#endif

