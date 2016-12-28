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
    {"usart1_rx", GPIOA, 10, GPIO_Speed_2MHz, GPIO_Mode_IN_FLOATING},
    {"pms_reset", GPIOA, 11, GPIO_Speed_2MHz, GPIO_Mode_Out_PP},
    {"pms_set", GPIOA, 12, GPIO_Speed_2MHz, GPIO_Mode_Out_PP},
    {"sound", GPIOC, 1, GPIO_Speed_2MHz, GPIO_Mode_AIN},
    {"gp2y1050", GPIOC, 3, GPIO_Speed_2MHz, GPIO_Mode_AIN},
    {"voca", GPIOC, 2, GPIO_Speed_2MHz, GPIO_Mode_IN_FLOATING},
    {"vocb", GPIOC, 6, GPIO_Speed_2MHz, GPIO_Mode_IN_FLOATING},
};

/* clock arrays */
PIN_CLOCK pinClocks[] = 
{
    {APB2, RCC_APB2_RESET_IOPA, RCC_APB2_ENABLE_IOPA},
    {APB2, RCC_APB2_RESET_IOPC, RCC_APB2_ENABLE_IOPC},
    {APB2, RCC_APB2_RESET_USART1, RCC_APB2_ENABLE_USART1},
    {APB2, RCC_APB2_RESET_ADC1, RCC_APB2_ENABLE_ADC1},
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
 * @brief set pin
 * @param pin name
 */
void pinSet(__in const char *name)
{
    const PIN_CONFIG *config = getPinConfig(name);
    assert_param(config != NULL);
    GPIO_SetPin(config->group, config->config.pin);

}

/**
 * @brief reset pin
 * @param pin name
 */
void pinReset(__in const char *name)
{
    const PIN_CONFIG *config = getPinConfig(name);
    assert_param(config != NULL);
    GPIO_ResetPin(config->group, config->config.pin);
}

/**
 * @brief check if pin is set
 * @param pin name
 */
BOOL isPinSet(__in const char *name)
{
    const PIN_CONFIG *config = getPinConfig(name);
    assert_param(config != NULL);
    if(GPIO_ReadPin(config->group, config->config.pin) != 0)
        return TRUE;
    else
        return FALSE;
}

