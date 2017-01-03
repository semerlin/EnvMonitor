#include "display.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tft.h"
#include "GUI.h"
#include "background.h"
#include "global.h"
#include <string.h>

/* static functions */
static void setPressureValue(__in uint8 value);
static void setVocValue(__in uint8 value);
static void setLightValue(__in uint8 value);
static void setSoundValue(__in uint8 value);
static void setPMValue(__in uint8 value);
static void setRHValue(__in uint8 value);

/**
 * @brief lcd show task
 */
static void vLcdShow(void *pvParameters)
{
    UNUSED(pvParameters);

    Sensor_Info sensorInfo;
    //WM_SetCreateFlags(WM_CF_MEMDEV);
    /* init gui module, stemwin use BGR(565)mode */
    GUI_Init();
    showBackground();
    
    //clear PM2.5 background
    GUI_SetColor(0);
    GUI_FillRect(130, 130, 140, 140);
    
    setPressureValue(0);
    setVocValue(0);
    setLightValue(0);
    setSoundValue(0);
    setPMValue(0);
    setRHValue(0);
    for(;;)
    {
        if(xQueueReceive(xSensorValues, &sensorInfo, portMAX_DELAY) == pdTRUE)
        {
            //get sensor data
            switch(sensorInfo.type)
            {
            case PMS5003S:
                break;
            case GP2Y1050:
                break;
            case Sound:
                setSoundValue(sensorInfo.value);
                break;
            case Voc:
                setVocValue(sensorInfo.value);
                break;
            default:
                break;
            }
        }
        
        //vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief setup lcd module
 */
void vDisplaySetup(void)
{
    xTaskCreate(vLcdShow, "lcdShow", LCD_STACK_SIZE, NULL, LCD_PRIORITY, NULL);
}

static void uintToString(__in uint8 val, __out int8 *str)
{
    if(val >= 100)
    {
        str[0] = val / 100 + 0x30;
        str[1] = val / 10 % 10 + 0x30;
        str[2] = val % 10 + 0x30;
        str[3] = '\0';
    }
    else if(val >= 10)
    {
        str[0] = val / 10 + 0x30;
        str[1] = val % 10 + 0x30;
        str[2] = '\0';
    }
    else
    {
        str[0] = val + 0x30;
        str[1] = '\0';
    }
}

static void setPressureValue(__in uint8 value)
{
    int8 val[4];
    GUI_SetColor(0x456be3);
    GUI_DrawGradientV(31, 44, 62, 65, 0xa2a09e, 0xb9b8b8);
    GUI_SetTextMode(GUI_TM_TRANS); 
    GUI_SetFont(GUI_FONT_20_ASCII);
    uintToString(value, val);
    GUI_DispStringHCenterAt((const char *)val, 47, 45);
}

static void setVocValue(__in uint8 value)
{
    int8 val[4];
    GUI_SetColor(0x2c1ebe);
    GUI_DrawGradientV(262, 46, 293, 67, 0xa5a4a3, 0xb9b9b9);
    GUI_SetTextMode(GUI_TM_TRANS); 
    GUI_SetFont(GUI_FONT_20_ASCII);
    uintToString(value, val);
    GUI_DispStringHCenterAt((const char *)(const char *)val, 278, 47);
}

static void setLightValue(__in uint8 value)
{
    int8 val[4];
    GUI_SetColor(0x0a0a0a);
    GUI_FillRect(73, 166, 97, 180);
    GUI_SetColor(0x315a00);
    GUI_SetTextMode(GUI_TM_NORMAL); 
    GUI_SetFont(&GUI_Font8x16);
    uintToString(value, val);
    GUI_DispStringHCenterAt((const char *)val, 85, 166);
}

static void setSoundValue(__in uint8 value)
{
    int8 val[4];
    GUI_SetColor(0x0a0a0a);
    GUI_FillRect(225, 166, 249, 180);
    GUI_SetColor(0xba751e);
    GUI_SetTextMode(GUI_TM_NORMAL); 
    GUI_SetFont(&GUI_Font8x16);
    uintToString(value, val);
    GUI_DispStringHCenterAt((const char *)val, 237, 166);
}

static void setPMValue(__in uint8 value)
{
    int8 val[4];
    GUI_SetColor(0x020202);
    GUI_FillRect(133, 130, 183, 164);
    GUI_SetColor(GUI_WHITE);
    GUI_SetTextMode(GUI_TM_NORMAL); 
    GUI_SetFont(GUI_FONT_32_ASCII);
    uintToString(value, val);
    GUI_DispStringHCenterAt((const char *)val, 160, 132);
}

static void setRHValue(__in uint8 value)
{
    int8 val[8];
    GUI_SetColor(0x020202);
    GUI_FillRect(140, 198, 180, 210);
    GUI_SetColor(GUI_WHITE);
    GUI_SetTextMode(GUI_TM_NORMAL); 
    GUI_SetFont(&GUI_Font6x8);
    uintToString(value, val);
    strcat(val, "%RH");
    GUI_DispStringHCenterAt((const char *)val, 160, 200);
}