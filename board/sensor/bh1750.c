#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stm32f10x_cfg.h"
#include "global.h"
#include "i2c.h"

/* bh1750 definition */
#define BH1750_ADDRESS     (0x23)
#define BH1750_SPEED       (100000)

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

/* interface */
static void bh1750Start(__in Handle i2c);
static uint32 bh1750GetLight(__in Handle i2c);

/**
 * @brief process bh1750 data
 */
uint32 light = 0;
static void vBH1750SProcess(void *pvParameters)
{
    UNUSED(pvParameters);
    const TickType_t xNotifyWait = 100 / portTICK_PERIOD_MS;
    const TickType_t xDelay = 1000 / portTICK_PERIOD_MS; 
    Sensor_Info sensorInfo = {BH1750 , 0};
    Handle i2c = I2c_Request(I2c2);
    I2c_SetSpeed(i2c, BH1750_SPEED);
    I2c_SetSlaveAddress(i2c, BH1750_ADDRESS);
    I2c_Open(i2c);
    uint32 prevValue = 0;
    for(;;)
    {
        xSemaphoreTake(xI2cMutex, portMAX_DELAY);
        bh1750Start(i2c);
        xSemaphoreGive(xI2cMutex);
        vTaskDelay(xDelay);
        xSemaphoreTake(xI2cMutex, portMAX_DELAY);
        light = bh1750GetLight(i2c);
        xSemaphoreGive(xI2cMutex);
        sensorInfo.value = light;
        if(sensorInfo.value != prevValue)
        {
            prevValue = sensorInfo.value;
            xQueueSend(xSensorValues, (const void *)&sensorInfo, 
                           xNotifyWait);
        }
        
        vTaskDelay(100 / portTICK_PERIOD_MS);
        
    }
}

/**
 * @brief setup bh1750 process
 */
void vBH1750Setup(void)
{
    xTaskCreate(vBH1750SProcess, "BH1750SProcess", BH1750_STACK_SIZE, 
                NULL, BH1750_PRIORITY, NULL);
}


/**
 * @brief start measure light
 * @param i2c handle
 */
static void bh1750Start(__in Handle i2c)
{
    uint8 data = BH1750_POWER_ON;
    I2c_Write(i2c, (const char *)&data, 1);
    data = BH1750_RESET;
    I2c_Write(i2c, (const char *)&data, 1);
    data = BH1750_ONE_HR1;
    I2c_Write(i2c, (const char *)&data, 1);
}

/**
 * @brief get light value
 * @param i2c module
 */
static uint32 bh1750GetLight(__in Handle i2c)
{
    uint16 result = 0;
    uint8 data[2] = {0, 0};
    I2c_Read(i2c, (char *)data, 2);
    result = data[0];
    result <<= 16;
    result |= data[1];
    return (uint32)(result / 1.2);
}