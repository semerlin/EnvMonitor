#include "display.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tft.h"
#include "GUI.h"
#include "background.h"
#include "global.h"
#include <string.h>
#include <math.h>



/* static functions */
static void setPressureValue(__in uint8 value);
static void setVocValue(__in uint8 value);
static void setLightValue(__in uint8 value);
static void setSoundValue(__in uint8 value);
static void setPMValue(__in uint16 value);
static void setRHValue(__in uint8 value);
static void setTemperValue(__in int8 value);

/* rh line position map */
typedef struct
{
    uint32 x0;
    uint32 y0;
    uint32 x1;
    uint32 y1;
}RHLINE_T;

static RHLINE_T rhLine[] = 
{
    {128, 196, 115, 201},
    {131, 201, 119, 208},
    {134, 206, 123, 215},
    {138, 210, 129, 221},
    {143, 213, 136, 225},
    {148, 216, 143, 229},
    {154, 217, 151, 231},
    {159, 217, 159, 231},
    {165, 217, 168, 231},
    {171, 216, 176, 229},
    {176, 213, 183, 225},
    {181, 210, 190, 221},
    {185, 206, 196, 215},
    {188, 201, 200, 208},
    {191, 196, 204, 201},
};


/**
 * @brief lcd show task
 */
static void vLcdShow(void *pvParameters)
{
    UNUSED(pvParameters);
    
#ifndef __DEMO
    Sensor_Info sensorInfo;
#endif
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
void vDisplaySetup(void)
{
    xTaskCreate(vLcdShow, "lcdShow", LCD_STACK_SIZE, NULL, LCD_PRIORITY, NULL);
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
    int8 val[4];
    GUI_SetColor(0x456be3);
    GUI_DrawGradientV(31, 44, 62, 65, 0xa2a09e, 0xb9b8b8);
    GUI_SetTextMode(GUI_TM_TRANS); 
    GUI_SetFont(GUI_FONT_20_ASCII);
    uintToString(value, val);
    GUI_DispStringHCenterAt((const char *)val, 47, 45);
}

/**
 * @brief set voc value
 * @param voc value
 */
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

/**
 * @brief set light value
 * @param light value
 */
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

/**
 * @brief set sound value
 * @param sound value
 */
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

/**
 * @brief set pm2.5 value
 * @param pm2.5 value
 */
static void setPMValue(__in uint16 value)
{
    int8 val[4];
    GUI_SetColor(0x020202);
    GUI_FillRect(133, 130, 183, 164);
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
    GUI_SetTextMode(GUI_TM_NORMAL); 
    GUI_SetFont(GUI_FONT_32_ASCII);
    uintToString(value, val);
    GUI_DispStringHCenterAt((const char *)val, 160, 132);
}

/**
 * @brief set humidity value
 * @param humidity value
 */
static void setRHValue(__in uint8 value)
{
    int8 val[8];
    GUI_SetColor(0x020202);
    GUI_FillRect(140, 198, 180, 210);
    GUI_SetColor(GUI_WHITE);
    GUI_SetTextMode(GUI_TM_NORMAL); 
    GUI_SetFont(&GUI_Font6x8);
    uintToString(value, val);
    strcat((char *)val, "%RH");
    GUI_DispStringHCenterAt((const char *)val, 160, 200);
    
    //draw rh arc
    if(value > 100)
        value = 100;
    uint8 angle = value * 160 / 100;
    GUI_SetPenSize(15);
    GUI_SetColor(0x2bcff7);
    //GUI_AA_DrawArc(160, 185, 40, 40, -170, -170 + angle);
    GUI_DrawArc(160, 185, 40, 40, -170, -170 + angle);
    GUI_SetColor(0xb5b5b5);
    GUI_DrawArc(160, 185, 40, 40, -170 + angle, -10);
    //GUI_AA_DrawArc(160, 185, 40, 40, -170 + angle, -10);
    
    //draw rh arc line
    GUI_SetPenSize(1);
    GUI_SetColor(0x747474);
    /*float a = 0;
    uint32 x0[15], x1[15], y0[15], y1[15];
    for(uint8 i = 0; i < 15; ++i)
    {
        a = (200 + i * 10) * 3.14159 / 180;
        x0[i] = (int32)(33 * cos(a) + 160);
        y0[i] = (int32)(-33 * sin(a) + 185);
        x1[i] = (int32)(47 * cos(a) + 160);
        y1[i] = (int32)(-47 * sin(a) + 185);
    }*/
    
    for(uint8 i = 0; i < 15; ++i)
    {
        //use map for fast drawing
        GUI_DrawLine(rhLine[i].x0, rhLine[i].y0, rhLine[i].x1, rhLine[i].y1);
    }
}

static void setTemperValue(__in int8 value)
{
    //temperature: -10-50
    static uint32 prevPointX = 30, prevPointY = 226;
    static uint32 prevTextX = 10, prevTextY = 217;
    static int8 prevVal[5] = "-10";
     
    int8 val[5];
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
    
    //long axis x = 132 short axis y = 126 
    //clear prev point
    GUI_SetColor(0x020202);
    GUI_AA_FillCircle(prevPointX, prevPointY, 5);
    
    //draw new point
    GUI_SetColor(0xc8c8c8);
    float a = 0, factor = 0, length = 0;
    uint32 x, y;
    a = (180 - (value + 10) * 180 / 60) * 3.14159 / 180;
    factor = cos(a) * cos(a) / (132 * 132) + sin(a) * sin(a) / (126 * 126);
    length = sqrt(1.0 / factor);   
    x = (int32)(length * cos(a) + 160);
    y = (int32)(-length * sin(a) + 226);

    GUI_AA_FillCircle(x, y, 5);
    //store point
    prevPointX = x;
    prevPointY = y;
    
    //clear prev text
    GUI_SetColor(0x020202);
    GUI_DispStringHCenterAt((const char *)prevVal, prevTextX, prevTextY);
    
    //draw text
    GUI_SetColor(GUI_WHITE);
    GUI_SetTextMode(GUI_TM_NORMAL); 
    GUI_SetFont(&GUI_Font6x8);
    a = (180 - (value + 10) * 180 / 60) * 3.14159 / 180;
    x = (int32)(150 * cos(a) + 160);
    y = (int32)(-150 * sin(a) + 217);
    
    GUI_DispStringHCenterAt((const char *)val, x, y);
    //store text
    prevTextX = x;
    prevTextY = y;
    strcpy((char *)prevVal, (const char *)val);
}