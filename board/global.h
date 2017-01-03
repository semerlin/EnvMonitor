#ifndef _GLOBAL_H_
#define _GLOBAL_H_


#define __DEBUG
#include "assert.h"

#include "sysdef.h"
#include "FreeRTOS.h"
#include "semphr.h"

typedef enum
{
    PMS5003S,
    GP2Y1050,
    Sound,
    Voc,
}Sensor_Type;

typedef struct
{
    Sensor_Type type;
    uint32 value;
}Sensor_Info;

extern xQueueHandle xSensorValues;
extern xSemaphoreHandle xAdcMutex;

/* task priority definition */
#define LCD_PRIORITY             (tskIDLE_PRIORITY + 3)
#define VOC_PRIORITY             (tskIDLE_PRIORITY + 2)
#define SOUND_PRIORITY           (tskIDLE_PRIORITY + 1)
#define PM2_5_PRIORITY           (tskIDLE_PRIORITY + 1)

/* task stack definition */
#define LCD_STACK_SIZE           (configMINIMAL_STACK_SIZE * 2)
#define VOC_STACK_SIZE           (configMINIMAL_STACK_SIZE)
#define SOUND_STACK_SIZE         (configMINIMAL_STACK_SIZE)
#define PM2_5_STACK_SIZE         (configMINIMAL_STACK_SIZE)

/* sensor control */
#define SOUND_FACTOR (20)




#endif