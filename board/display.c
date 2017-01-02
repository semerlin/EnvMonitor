#include "display.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tft.h"
#include "GUI.h"
#include "background.h"
#include "global.h"


static void vLcdShow(void *pvParameters)
{
    UNUSED(pvParameters);

    //WM_SetCreateFlags(WM_CF_MEMDEV);
    /* init gui module, stemwin use BGR(565)mode */
    GUI_Init();
    showBackground();
  
    for(;;)
    {
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}


void vDisplaySetup(void)
{
    xTaskCreate(vLcdShow, "lcdShow", LCD_STACK_SIZE, NULL, LCD_PRIORITY, NULL);
}