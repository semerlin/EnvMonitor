#include "stm32f10x_cfg.h"
#include "global.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "serial.h"
#include <string.h>
#include "pms5003s.h"
#include "gp2y1050.h"
#include "sound.h"
#include "voc.h"
#include "display.h"
#include "am2302.h"
#include "bmp280.h"
#include "bh1750.h"
#include "environment.h"

xQueueHandle xSensorValues = NULL;
xSemaphoreHandle xAdcMutex = NULL;
xSemaphoreHandle xI2cMutex = NULL;

void ApplicationStartup()
{
    xSensorValues = xQueueCreate(10, 
                         (UBaseType_t)(sizeof(Sensor_Info) / sizeof(char)));
    xAdcMutex = xSemaphoreCreateMutex();
    xI2cMutex = xSemaphoreCreateMutex();
    
    vDisplaySetup();
#ifndef __DEMO
    vBH1750Setup();
    vBMP280Setup();
#ifdef __PMS5003S
    vPMS5003Setup();
#else
    vGP2Y10150Setup();
#endif
    vSoundSetup();
    vVocSetup();
    vAM2302Setup();
#endif
    
	/* Start the scheduler. */
	vTaskStartScheduler();
}


#ifdef __DEBUG
//Handle serial1;
void assert_failed(const char *file, const char *line, const char *exp)
{
    //Serial_PutString(serial1, "Assert Failed: ", 15);
    //Serial_PutString(serial1, file, strlen(file));
    //Serial_PutChar(serial1, ':', 0);
    //Serial_PutString(serial1, line, strlen(line));
    //Serial_PutChar(serial1, '(', 0);
    //Serial_PutString(serial1, exp, strlen(exp));
    //Serial_PutString(serial1, ")\n", 2);
    while(1);
}
#endif


#ifdef __ENABLE_TRACE
void Log(unsigned char level, const char *msg)
{
}
#endif