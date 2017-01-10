#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stm32f10x_cfg.h"
#include "global.h"
#include "environment.h"
#include "i2c.h"
#include "bmp280.h"
#include "bh1750.h"



/**
 * @brief process iic sensor data
 */
static void vI2CSensorProcess(void *pvParameters)
{
    UNUSED(pvParameters);
    const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
    const TickType_t xNotifyWait = 100 / portTICK_PERIOD_MS;
    
    //init bmp280
    Sensor_Info bmp280Info = {BMP280 , 0};
    BMP280_T bmp280;
    bmp280.i2c = I2c_Request(I2c1);
    bmp280Init(&bmp280);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    double temperature = 0, pressure = 0;
    uint32 prevPressureValue = 0;
    
    //init bh1750
    Sensor_Info bh1750Info = {BH1750 , 0};
    bh1750Init(I2c_Request(I2c2));
    uint32 prevLight = 0;
    
    
    for(;;)
    {
        //start sample
        bmp280SetWorkMode(&bmp280, BMP280_FORCED);
        bh1750Start();
        vTaskDelay(xDelay);
        //get bmp280 data
        bmp280GetTemperatureAndPressure(&bmp280, &temperature, &pressure);
        bmp280Info.value = (uint32)pressure;
        if(bmp280Info.value != prevPressureValue)
        {
            prevPressureValue = bmp280Info.value;
            xQueueSend(xSensorValues, (const void *)&bmp280Info, 
                           xNotifyWait);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
        //get bh1750 data
        bh1750Info.value = bh1750GetLight();
        if(bh1750Info.value != prevLight)
        {
            prevLight = bh1750Info.value;
            xQueueSend(xSensorValues, (const void *)&bh1750Info, 
                           xNotifyWait);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}



void vIICSensorSetup(void)
{
    xTaskCreate(vI2CSensorProcess, "I2CSensorProcess", I2C_STACK_SIZE, 
                NULL, I2C_PRIORITY, NULL);
}