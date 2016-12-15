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
}GPIO_TypeDef;

/* GPIO group array */
static GPIO_TypeDef * const GPIOx[] = {(GPIO_TypeDef *)GPIOA_BASE, 
                                      (GPIO_TypeDef *)GPIOB_BASE,
                                      (GPIO_TypeDef *)GPIOC_BASE,
                                      (GPIO_TypeDef *)GPIOD_BASE,
                                      (GPIO_TypeDef *)GPIOE_BASE,
                                      (GPIO_TypeDef *)GPIOF_BASE,
                                      (GPIO_TypeDef *)GPIOG_BASE};



/**
 * @beirf configure one pin function
 * @param group: group name
 * @param config: pin configuration
 */
void GPIO_Setup(__in GPIO_Group group, __in const GPIO_Config *config)
{
    assert_param(config->pin < 16);
    assert_param(group < count);
    
    GPIO_TypeDef * const GpioX = GPIOx[group];
 
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
    assert_param(group < count);
    GPIO_TypeDef * const GpioX = GPIOx[group];
    
    return GpioX->IDR;
}

/**
 * @brief write a group all pin data
 * @param group: port group
 * @param data: write value
 * @return data
 */
void GPIO_WriteDataGroup(__in GPIO_Group group, uint16 data)
{
    assert_param(group < count);
    GPIO_TypeDef * const GpioX = GPIOx[group];
    GpioX->ODR = data;
}


/**
 * @brief read pin data
 * @param group: port group
 * @param pin: pin position
 * @return pin data
 */
uint8 GPIO_ReadPin(__in GPIO_Group group, __in uint8 pin)
{
    assert_param(group < count);
    assert_param(pin < 16);
    GPIO_TypeDef * const GpioX = GPIOx[group];
    
    return GpioX->IDR >> pin;
}

/**
 * @brief set pin data
 * @param group: port group
 * @param pin: pin position
 */
void GPIO_SetPin(__in GPIO_Group group, __in uint8 pin)
{
    assert_param(group < count);
    assert_param(pin < 16);
    GPIO_TypeDef * const GpioX = GPIOx[group];
    GpioX->BSRR = (1 << pin);
}

/**
 * @brief reset pin data
 * @param group: port group
 * @param pin: pin position
 */
void GPIO_ResetPin(__in GPIO_Group group, __in uint8 pin)
{
    assert_param(group < count);
    assert_param(pin < 16);
    GPIO_TypeDef * const GpioX = GPIOx[group];
    GpioX->BRR = (1 << pin);
}
 
/**
 * @brief lock pin
 * @param group: port group
 * @param pin: pin position
 */
void GPIO_LockPin(__in GPIO_Group group, __in uint8 pin)
{
    assert_param(group < count);
    assert_param(pin < 16);
    
    GPIO_TypeDef * const GpioX = GPIOx[group];
    uint32 tmp = 0x00010000;
    tmp |= pin;
    
    /* lock sequence */
    GpioX->LCKR = tmp;
    GpioX->LCKR =  pin;
    GpioX->LCKR = tmp;
    tmp = GpioX->LCKR;
    tmp = GpioX->LCKR;
}


















