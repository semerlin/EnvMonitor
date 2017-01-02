#ifndef _TFT_H_
#define _TFT_H_

#include "sysdef.h"




/* low level control */
void lcdBklOn(__in BOOL flag);
uint16 lcdWidth(void);
uint16 lcdHeight(void);

/* gui interface */
void lcdInit(void);
void lcdSetCursor(__in uint16 xPos, __in uint16 yPos);
void lcdWriteReg(__in uint8 reg);
void lcdWriteData(__in uint8 data);
void lcdWriteDataMultiple(uint8 * pData, int NumItems);
void lcdReadDataMultiple(uint8 * pData, int NumItems);


#endif