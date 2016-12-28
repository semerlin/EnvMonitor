#ifndef _APPLICATION_H_
  #define _APPLICATION_H_

#define __DEBUG
#include "assert.h"

#include "sysdef.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

typedef enum
{
    PMS5003S,
    GP2Y1050,
    Sound,
}Sensor_Type;

typedef struct
{
    Sensor_Type type;
    uint32 value;
}Sensor_Info;

extern xQueueHandle xSensorValues;
extern xSemaphoreHandle xAdcSemphr;

void ApplicationStartup();




#endif