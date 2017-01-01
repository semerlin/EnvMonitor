#ifndef _TFT_H_
#define _TFT_H_

#include "sysdef.h"




void lcdInit(void);
void lcdBklOn(__in BOOL flag);
void lcdClearScreen(__in uint8 red, uint8 green, uint8 blue);


void showimage(uint16 x,uint16 y);
void showimage1(uint16 x,uint16 y);

#endif