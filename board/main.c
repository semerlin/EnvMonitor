#include "stm32f10x_cfg.h"
#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f10x_usart.h"

/* Task priorities. */
#define mainQUEUE_POLL_PRIORITY				( tskIDLE_PRIORITY + 2 )
#define mainCHECK_TASK_PRIORITY				( tskIDLE_PRIORITY + 3 )
#define mainSEM_TEST_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define mainBLOCK_Q_PRIORITY				( tskIDLE_PRIORITY + 2 )
#define mainCREATOR_TASK_PRIORITY           ( tskIDLE_PRIORITY + 3 )
#define mainFLASH_TASK_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define mainCOM_TEST_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define mainINTEGER_TASK_PRIORITY           ( tskIDLE_PRIORITY )

#define mainCHECK_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 50 )

static void vCheckTask( void *pvParameters )
{
    const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
    for(;;)
    {
        configASSERT(0);
        USART_WriteData(USART1, 0xdd);
        vTaskDelay(xDelay);
    }
}

static void vCheckTask1( void *pvParameters )
{
    const TickType_t xDelay = 800 / portTICK_PERIOD_MS;
    for(;;)
    {
        USART_WriteData(USART1, 0xcc);
        vTaskDelay(xDelay);
    }
}

int main(int argc, char **argv)
{
    board_init();
    xTaskCreate( vCheckTask, "Check", mainCHECK_TASK_STACK_SIZE, NULL, 1, NULL );	
	xTaskCreate( vCheckTask1, "Check1", mainCHECK_TASK_STACK_SIZE, NULL, 1, NULL );	

	/* Start the scheduler. */
	vTaskStartScheduler();
    while(1);
}