#include "board.h"
#include "application.h"
#include "stm32f10x_cfg.h"



uint16 val = 0;
int main(int argc, char **argv)
{
    board_init();
    RCC_ADCPrescalerFromPCLK2(RCC_ADC_PCLK_DIV8);

    ApplicationStartup();
    
    //should never reached here
    return 0;
}

