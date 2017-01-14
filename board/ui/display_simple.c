#include "display_simple.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tft.h"
#include "GUI.h"
#include "global.h"
#include <string.h>
#include <math.h>
#include "font.h"



/* static functions */
static void setPressureValue(__in uint8 value);
static void setVocValue(__in uint8 value);
static void setLightValue(__in uint8 value);
static void setSoundValue(__in uint8 value);
static void setPMValue(__in uint16 value);
static void setRHValue(__in uint8 value);
static void setTemperValue(__in int8 value);



/**
 * @brief lcd show task
 */
static void vLcdSimpleShow(void *pvParameters)
{
    UNUSED(pvParameters);
    
#ifndef __DEMO
    Sensor_Info sensorInfo;
#endif
    /* init gui module, stemwin use BGR(565)mode */
    GUI_Init();
    
    //set background
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    
    setPressureValue(0);
    setVocValue(0);
    setLightValue(0);
    setSoundValue(0);
    setPMValue(0);
    setRHValue(0);
    setTemperValue(-10);
#ifdef __DEMO
    uint8 normalValue = 0;
    int8 temperValue = -10;
    uint8 rhValue = 0;
#endif
    for(;;)
    {
#ifdef __DEMO
        setPressureValue(normalValue);
        setVocValue(normalValue);
        setLightValue(normalValue);
        setSoundValue(normalValue);
        setPMValue(normalValue);
        setRHValue(rhValue++);
        setTemperValue(temperValue++);
        normalValue++;
        if(rhValue > 100)
            rhValue = 0;
        if(temperValue > 50)
            temperValue = -10;
        vTaskDelay(200 / portTICK_PERIOD_MS);
#else
        if(xQueueReceive(xSensorValues, &sensorInfo, portMAX_DELAY) == pdTRUE)
        {
            //get sensor data
            switch(sensorInfo.type)
            {
            case PMS5003S:
                setPMValue(sensorInfo.value);
                break;
            case GP2Y1050:
                setPMValue(sensorInfo.value);
                break;
            case Sound:
                setSoundValue(sensorInfo.value);
                break;
            case Voc:
                setVocValue(sensorInfo.value);
                break;
            case AM2302:
                {
                    int8 temper;
                    setRHValue(sensorInfo.value & 0xff);
                    if(sensorInfo.value & 0x80000000)
                        temper = -((sensorInfo.value & 0xff00) >> 8);
                    else
                        temper = ((sensorInfo.value & 0xff00) >> 8);
                    setTemperValue(temper);
                }
                break;
            case BMP280:
                sensorInfo.value /= 1000;
                setPressureValue(sensorInfo.value);
                break;
            case BH1750:
                setLightValue(sensorInfo.value);
                break;
            default:
                break;
            }
        }
#endif
    }
}

/**
 * @brief setup lcd module
 */
void vDisplaySimpleSetup(void)
{
    xTaskCreate(vLcdSimpleShow, "lcdSimpleShow", LCD_STACK_SIZE, 
                NULL, LCD_PRIORITY, NULL);
}

/**
 * @brief change uint value to string value
 * @param value need to convert
 * @param converted string
 */
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

/**
 * @brief set pressure value
 * @param pressure value
 */
static void setPressureValue(__in uint8 value)
{

}

/**
 * @brief set voc value
 * @param voc value
 */
static void setVocValue(__in uint8 value)
{

}

/**
 * @brief set light value
 * @param light value
 */
static void setLightValue(__in uint8 value)
{

}

/**
 * @brief set sound value
 * @param sound value
 */
static void setSoundValue(__in uint8 value)
{

}

/**
 * @brief set pm2.5 value
 * @param pm2.5 value
 */
static void setPMValue(__in uint16 value)
{
    GUI_SetPenSize(10);
    GUI_SetColor(0xcd7e11);
    GUI_DrawArc(80, 120, 65, 65, 0, 360);
    
    int8 val[16];
    if(value <= 50)
        GUI_SetColor(0x18cf43);
    else if(value <= 100)
        GUI_SetColor(0x30dcee);
    else if(value <= 150)
        GUI_SetColor(0x01aaff);
    else if(value <= 200)
        GUI_SetColor(0x1b40ff);
    else
        GUI_SetColor(0x4100d2);
    GUI_SetFont(GUI_FONT_24_ASCII);
    uintToString(value, val);
    strcat((char *)val, "ug/m3");
    GUI_DispStringHCenterAt((const char *)val, 80, 108);
    
    
}

/**
 * @brief set humidity value
 * @param humidity value
 */
static void setRHValue(__in uint8 value)
{
    GUI_SetPenSize(10);
    GUI_SetColor(0x5cce68);
    GUI_DrawArc(240, 180, 45, 45, 0, 360);
    
    GUI_SetFont(GUI_FONT_20_1);
    GUI_SetColor(0x315a00);
    int8 val[8];
    uintToString(value, val);
    strcat((char *)val, "%RH");
    GUI_DispStringHCenterAt((const char *)val, 240, 172);
}

/**
 * @brief set temperature value
 * @param temperature value
 */
static void setTemperValue(__in int8 value)
{
    GUI_SetPenSize(10);
    GUI_SetColor(0x0d7bf1);
    GUI_DrawArc(240, 60, 45, 45, 0, 360);
    
    GUI_SetFont(GUI_FONT_20_1);
    GUI_SetColor(0xba751e);
    int8 val[12];
    if(value < 0)
    {
        if(value < -10)
            value = -10;
        uintToString(-value, val + 1);
        val[0] = '-';
    }
    else
    {
        if(value > 50)
            value = 50;
        uintToString(value, val);
    }
    
    strcat((char *)val, " 'C");
    GUI_DispStringHCenterAt((const char *)val, 240, 52);
}