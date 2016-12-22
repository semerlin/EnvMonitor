#include "pinconfig.h"
#include "stm32f10x_cfg.h"
#include <string.h>

/* pin configure structure */
typedef struct 
{
    char name[16];
    GPIO_Group group;
    GPIO_Config config;
}PIN_CONFIG;


typedef enum
{
    AHB,
    APB1,
    APB2,
}PIN_BUS;

typedef struct
{
    PIN_BUS bus;
    uint32 resetReg;
    uint32 enableReg;
}PIN_CLOCK;


/* pin arrays */
PIN_CONFIG pins[] = 
{
    {"power", GPIOA, 8, GPIO_Speed_2MHz, GPIO_Mode_Out_PP},
    {"usart1_tx", GPIOA, 9, GPIO_Speed_50MHz, GPIO_Mode_AF_PP},
    {"usart1_rx", GPIOA, 10, GPIO_Speed_2MHz, GPIO_Mode_IN_FLOATING}
};

/* clock arrays */
PIN_CLOCK pinClocks[] = 
{
    {APB2, RCC_APB2_RESET_IOPA, RCC_APB2_ENABLE_IOPA},
    {APB2, RCC_APB2_RESET_USART1, RCC_APB2_ENABLE_USART1},
};

/**
 * @brief get pin configuration by name
 * @param pin name
 * @return pin configuration
 */
static const PIN_CONFIG *getPinConfig(const char *name)
{
    uint32 len = sizeof(pins) / sizeof(PIN_CONFIG);
    for(uint32 i = 0; i < len; ++i)
    {
        if(strcmp(name, pins[i].name) == 0)
            return &pins[i];
    }
    
    return NULL;
}

/**
 * @brief init pins
 */
void pinInit(void)
{
    //config pin clocks
    uint32 len = sizeof(pinClocks) / sizeof(PIN_CLOCK);
    for(uint32 i = 0; i < len; ++i)
    {
        switch(pinClocks[i].bus)
        {
        case AHB:
            RCC_AHBPeripClockEnable(pinClocks[i].enableReg, TRUE);
            break;
        case APB1:
            RCC_APB1PeriphReset(pinClocks[i].resetReg, TRUE);
            RCC_APB1PeriphReset(pinClocks[i].resetReg, FALSE);
            RCC_APB1PeripClockEnable(pinClocks[i].enableReg, TRUE);
            break;
        case APB2:
            RCC_APB2PeriphReset(pinClocks[i].resetReg, TRUE);
            RCC_APB2PeriphReset(pinClocks[i].resetReg, FALSE);
            RCC_APB2PeripClockEnable(pinClocks[i].enableReg, TRUE);
            break;
        default:
            break;
        }
    }
    
    //config pins
    len = sizeof(pins) / sizeof(PIN_CONFIG);
    for(uint32 i = 0; i < len; ++i)
    {
        GPIO_Setup(pins[i].group, &pins[i].config);
    }
}

/**
 * @brief enable or disable board 5V
 * @param enable flag
 */
void powerEnable(BOOL flag)
{
    const PIN_CONFIG *config = getPinConfig("power");
    if(config != NULL)
    {
        if(flag)
            GPIO_SetPin(config->group, config->config.pin);
        else
            GPIO_ResetPin(config->group, config->config.pin);
    }
}