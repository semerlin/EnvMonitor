#ifndef _I2C_H_
  #define _I2C_H_

#include "sysdef.h"

/* serial parameter definition */
typedef enum
{ 
	I2c1,
    I2c2,
    Port_Count,
}Port;





/* interface */
Handle I2c_Request(__in Port port);
BOOL I2c_Open(__in Handle handle);
void I2c_Close(__in Handle handle);
void I2c_SetSpeed(__in Handle handle, __in uint32 speed);
void I2c_SetSlaveAddress(__in Handle handle, __in uint8 address);
void I2c_SetBufferLength(__in Handle handle, __in UBaseType_t rxLen, 
                            __in UBaseType_t txLen);
BOOL I2c_Write(__in Handle handle, __in const char *data, __in uint32 length);
BOOL I2c_Read(__in Handle handle, __out char *data, __in uint32 length);


#endif

