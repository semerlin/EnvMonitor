#include "stm32f10x_cfg.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "application.h"
#include "serial.h"
#include <string.h>
#include "sysdef.h"
#include "pms5003s.h"
#include "gp2y1050.h"
#include "sound.h"

xQueueHandle xSensorValues = NULL;
xSemaphoreHandle xAdcSemphr = NULL;

void ApplicationStartup()
{
    xSensorValues = xQueueCreate(10, 
                         (UBaseType_t)(sizeof(Sensor_Info) / sizeof(char)));
    xAdcSemphr = xSemaphoreCreateBinary();
    
    vPMS5003Setup();
    vGP2Y10150Setup();
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