#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stm32f10x_cfg.h"
#include "global.h"
#include "i2c.h"
#include "environment.h"

static Handle i2cHandle = NULL;

/* bh1750 definition */
#define BH1750_ADDRESS      (0x23)
//#ifdef __DEBUG
#define BH1750_SPEED        (100000)
//#else
//#define BH1750_SPEED       (50000)
//#endif

/* register */
#define BH1750_POWER_DOWN      (0x00)
#define BH1750_POWER_ON        (0x01)
#define BH1750_RESET           (0x07)
#define BH1750_CON_HR1         (0x10)
#define BH1750_CON_HR2         (0x11)
#define BH1750_CON_LR          (0x13)
#define BH1750_ONE_HR1         (0x20)
#define BH1750_ONE_HR2         (0x21)
#define BH1750_ONE_LR          (0x23)


/**
* @brief init bh1750
* @param i2c handle
*/
void bh1750Init(__in Handle i2c)
{
    i2cHandle = i2c;
    I2c_SetSpeed(i2c, BH1750_SPEED);
    I2c_SetSlaveAddress(i2c, BH1750_ADDRESS);
    I2c_Open(i2c);
}

/**
 * @brief start measure light
 * @param i2c handle
 */
void bh1750Start(void)
{
    uint8 data = BH1750_POWER_ON;
    I2c_Write(i2cHandle, (const char *)&data, 1);
    data = BH1750_RESET;
    I2c_Write(i2cHandle, (const char *)&data, 1);
    data = BH1750_ONE_HR1;
    I2c_Write(i2cHandle, (const char *)&data, 1);
}

/**
 * @brief get light value
 * @param i2c module
 */
uint32 bh1750GetLight(void)
{
    uint16 result = 0;
    uint8 data[2] = {0, 0};
    I2c_Read(i2cHandle, (char *)data, 2);
    result = data[0];
    result <<= 16;
    result |= data[1];
    return (uint32)(result / 1.2);
}