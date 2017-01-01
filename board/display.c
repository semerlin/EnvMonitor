#include "display.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tft.h"


static void vLcdShow(void *pvParameters)
{
    UNUSED(pvParameters);
    lcdInit();
    showimage(0, 0);
    for(;;)
    {
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}


void vDisplaySetup(void)
{
    xTaskCreate(vLcdShow, "lcdShow", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
}