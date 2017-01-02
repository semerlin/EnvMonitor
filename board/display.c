#include "display.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tft.h"
#include "GUI.h"
#include "GUIDEMO.h"

static void vLcdShow(void *pvParameters)
{
    UNUSED(pvParameters);

    //WM_SetCreateFlags(WM_CF_MEMDEV);
    /* init gui module */
    GUI_Init();
    GUI_DispStringAt("Hello emWin!", 30, 216);
    for(;;)
    {
        vTaskDelay(500 / portTICK_PERIOD_MS);
        
    }
}


void vDisplaySetup(void)
{
    xTaskCreate(vLcdShow, "lcdShow", configMINIMAL_STACK_SIZE * 2, NULL, 2, NULL);
}