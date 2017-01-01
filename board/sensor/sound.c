#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stm32f10x_cfg.h"
#include "application.h"

/**
 * @brief process sound data
 */
static void vSoundProcess(void *pvParameters)
{
    UNUSED(pvParameters);
    const TickType_t xNotifyWait = 100 / portTICK_PERIOD_MS;
    const TickType_t xDelay = 100 / portTICK_PERIOD_MS;
    const TickType_t xAdcWait = 50 / portTICK_PERIOD_MS; 
    uint32 value = 0;
    Sensor_Info sensorInfo = {Sound , 0};
    uint8 count = 0;
    for(;;)
    {
        if(xSemaphoreTake(xAdcSemphr, xAdcWait) == pdTRUE)
        {
            ADC_SetRegularChannel(ADC1, 0, ADC_CHANNEL11);
            ADC_InternalTriggerConversion(ADC1, ADC_CHANNEL_GROUP_REGULAR);
            while(!ADC_IsFlagOn(ADC1, ADC_FLAG_EOC));
            value += ADC_GetRegularValue(ADC1);
            xSemaphoreGive(xAdcSemphr);
            count ++;
            if(count >= 8)
            {
                count = 0;
                value = 0;
                value >>= 3;
                sensorInfo.value = value / 25;
                xQueueSend(xSensorValues, (const void *)&sensorInfo, xNotifyWait);
            }
        }
        
        vTaskDelay(xDelay);
    }
}

/**
 * @brief ssetup sound process
 */
void vSoundSetup(void)
{
    xTaskCreate(vSoundProcess, "SoundProcess", configMINIMAL_STACK_SIZE, 
                NULL, 1, NULL);
}