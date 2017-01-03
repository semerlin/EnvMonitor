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
#include "display.h"

xQueueHandle xSensorValues = NULL;
xSemaphoreHandle xAdcMutex = NULL;

void ApplicationStartup()
{
    xSensorValues = xQueueCreate(10, 
                         (UBaseType_t)(sizeof(Sensor_Info) / sizeof(char)));
    xAdcMutex = xSemaphoreCreateMutex();
    
    vDisplaySetup();
    vPMS5003Setup();
    //vGP2Y10150Setup();
    vSoundSetup();

	/* Start the scheduler. */
	vTaskStartScheduler();
}


#ifdef __DEBUG
Handle serial1;
void assert_failed(const char *file, const char *line, const char *exp)
{
    Serial_PutString(serial1, "Assert Failed: ", 15);
    Serial_PutString(serial1, file, strlen(file));
    Serial_PutChar(serial1, ':', 0);
    Serial_PutString(serial1, line, strlen(line));
    Serial_PutChar(serial1, '(', 0);
    Serial_PutString(serial1, exp, strlen(exp));
    Serial_PutString(serial1, ")\n", 2);
    while(1);
}
#endif


#ifdef __ENABLE_TRACE
void Log(unsigned char level, const char *msg)
{
}
#endif