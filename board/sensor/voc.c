#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stm32f10x_cfg.h"
#include "application.h"
#include "pinconfig.h"

/**
 * @brief process voc data
 */
static void vVocProcess(void *pvParameters)
{
    UNUSED(pvParameters);
    const TickType_t xNotifyWait = 100 / portTICK_PERIOD_MS;
    const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
    uint8 value = 0;
    Sensor_Info sensorInfo = {Voc , 0};
    for(;;)
    {
        if(isPinSet("voca"))
            value = 0x02;
        else
            value = 0x00;
        
        if(isPinSet("vocb"))
            value += 0x01;
        
        sensorInfo.value = value;
        xQueueSend(xSensorValues, (const void *)&sensorInfo, xNotifyWait);
        
        vTaskDelay(xDelay);
    }
}

/**
 * @brief setup voc process
 */
void vVocSetup(void)
{
    xTaskCreate(vVocProcess, "VocProcess", configMINIMAL_STACK_SIZE, 
                NULL, 1, NULL);
}