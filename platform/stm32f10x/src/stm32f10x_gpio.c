#include "stm32f10x_gpio.h"
#include "stm32f10x_map.h"
#include "stm32f10x_cfg.h"

/* flash register structure */
typedef struct 
{
	volatile uint32 CRL;
	volatile uint32 CRH;
	volatile uint32 IDR;
	volatile uint32 ODR;
	volatile uint32 BSRR;
	volatile uint32 BRR;
	volatile uint32 LCKR;
}GPIO_T;

/* GPIO group array */
static GPIO_T * const GPIOx[] = {(GPIO_T *)GPIOA_BASE, 
                                      (GPIO_T *)GPIOB_BASE,
                                      (GPIO_T *)GPIOC_BASE,
                                      (GPIO_T *)GPIOD_BASE,
                                      (GPIO_T *)GPIOE_BASE,
                                      (GPIO_T *)GPIOF_BASE,
                                      (GPIO_T *)GPIOG_BASE};



/**
 * @beirf configure one pin function
 * @param group: group name
 * @param config: pin configuration
 */
void GPIO_Setup(__in GPIO_Group group, __in const GPIO_Config *config)
{
    assert_param(config->pin < 16);
    assert_param(group < GPIO_Count);
    
    GPIO_T * const GpioX = GPIOx[group];
 
    //config pin mode
    if((config->mode & 0x10) == 0x10)
    {
        //output
        if(config->pin < 8)
        {
            GpioX->CRL &= ~(0x0f << (config->pin << 2));
            GpioX->CRL |= (config->speed << (config->pin << 2));
            GpioX->CRL |= ((config->mode & 0x0f) << ((config->pin << 2) + 2));
        }
        else
        {
            GpioX->CRH &= ~(0x0f << ((config->pin - 8) << 2));
            GpioX->CRH |= (config->speed << ((config->pin - 8) << 2));
            GpioX->CRH |= ((config->mode & 0x0f) << 
                           (((config->pin - 8) << 2) + 2));
        }
    }
    else
    {
        //input
        if(config->pin < 8)
        {
            GpioX->CRL &= ~(0x0f << (config->pin << 2));
            GpioX->CRL |= ((config->mode & 0x0f) << ((config->pin << 2) + 2));
        }
        else
        {
            GpioX->CRH &= ~(0x0f << ((config->pin - 8) << 2));
            GpioX->CRH |= ((config->mode & 0x0f) << 
                           (((config->pin - 8) << 2) + 2));
        }
    }
}


/**
 * @brief read a group all pin data
 * @param group: port group
 * @return data
 */
uint16 GPIO_ReadDataGroup(__in GPIO_Group group)
{
    assert_param(group < GPIO_Count);
    
    return GPIOx[group]->IDR;
}

/**
 * @brief write a group all pin data
 * @param group: port group
 * @param data: write value
 * @return data
 */
void GPIO_WriteDataGroup(__in GPIO_Group group, uint16 data)
{
    assert_param(group < GPIO_Count);

    GPIOx[group]->ODR = data;
}


/**
 * @brief read pin data
 * @param group: port group
 * @param pin: pin position
 * @return pin data
 */
uint8 GPIO_ReadPin(__in GPIO_Group group, __in uint8 pin)
{
    assert_param(group < GPIO_Count);
    assert_param(pin < 16);
    
    return GPIOx[group]->IDR >> pin;
}

/**
 * @brief set pin data
 * @param group: port group
 * @param pin: pin position
 */
void GPIO_SetPin(__in GPIO_Group group, __in uint8 pin)
{
    assert_param(group < GPIO_Count);
    assert_param(pin < 16);

    GPIOx[group]->BSRR = (1 << pin);
}

/**
 * @brief reset pin data
 * @param group: port group
 * @param pin: pin position
 */
void GPIO_ResetPin(__in GPIO_Group group, __in uint8 pin)
{
    assert_param(group < GPIO_Count);
    assert_param(pin < 16);

    GPIOx[group]->BRR = (1 << pin);
}
 
/**
 * @brief lock pin
 * @param group: port group
 * @param pin: pin position
 */
void GPIO_LockPin(__in GPIO_Group group, __in uint8 pin)
{
    assert_param(group < GPIO_Count);
    assert_param(pin < 16);
    
    GPIO_T * const GpioX = GPIOx[group];
    uint32 tmp = 0x00010000;
    tmp |= pin;
    
    /* lock sequence */
    GpioX->LCKR = tmp;
    GpioX->LCKR =  pin;
    GpioX->LCKR = tmp;
    tmp = GpioX->LCKR;
    tmp = GpioX->LCKR;
}


















