#include "stm32f10x_i2c.h"
#include "stm32f10x_map.h"
#include "stm32f10x_cfg.h"
#include "stm32f10x_rcc.h"

/* i2c structure */
typedef struct 
{
    volatile uint16 CR1;
    uint16 RESERVED0;
    volatile uint16 CR2;
    uint16 RESERVED1;
    volatile uint16 OAR1;
    uint16 RESERVED2;
    volatile uint16 OAR2;
    uint16 RESERVED3;
    volatile uint16 DR;
    uint16 RESERVED4;
    volatile uint16 SR1;
    uint16 RESERVED5;
    volatile uint16 SR2;
    uint16 RESERVED6;
    volatile uint16 CCR;
    uint16 RESERVED7;
    volatile uint16 TRISE;
    uint16 RESERVED8;
}I2C_T;

/* i2c register definition */
#define CR1_SWRST        (1 << 15)
#define CR1_ACK          (1 << 10)
#define CR1_STOP         (1 << 9)
#define CR1_START        (1 << 8)
#define CR1_NOSTRETCH    (1 << 7)
#define CR1_ENGC         (1 << 6)
#define CR1_ENPEC        (1 << 5)
#define CR1_SMBUS        (1 << 1)
#define CR1_PE           (0x01)

#define CR2_FREQ         (0x3f)

#define OAR1_ADDMODE     (1 << 15)
#define OAR2_ENDUAL      (0x01)
#define CCR_F_S          (1 << 15)
#define CCR_DUTY         (1 << 14)
#define CCR_CCR          (0xfff)

#define SR2_BUSY     (1 << 1)



/* i2c group array */
static I2C_T * const I2Cx[] = {(I2C_T *)I2C1_BASE, 
                               (I2C_T *)I2C2_BASE};


/**
 * @brief reset i2c module
 * @param i2c group
 * @param enable or disable flag
 */
void I2C_Reset(__in I2C_Group group)
{
    assert_param(group < I2C_Count);
    I2C_T * const I2cX = I2Cx[group];
    
    //start reset
    I2cX->CR1 |= CR1_SWRST;
    //wait bus free
    while(I2cX->SR2 & SR2_BUSY);
    //stop reset
    I2cX->CR1 &= ~CR1_SWRST;
}

/**
 * @brief enable or disable i2c module
 * @param i2c group
 * @param enable or disable
 */
void I2C_Enable(__in I2C_Group group, __in BOOL flag)
{
    assert_param(group < I2C_Count);
    I2C_T * const I2cX = I2Cx[group];
    
    if(flag)
        I2cX->CR1 |= CR1_PE;
    else
        I2cX->CR1 &= ~CR1_PE;
}

/**
 * @brief setup i2c module parameter
 * @param i2c group
 * @param i2c parameter
 */
void I2C_Setup(__in I2C_Group group, __in I2C_Config *config)
{
    assert_param(group < I2C_Count);
    assert_param(config != NULL);
    assert_param(config->clock <= 400000);
    assert_param(IS_I2C_Address_Param(config->addressBits));
    I2C_T * const I2cX = I2Cx[group];
    
    //set strech
    if(config->enableStrech)
        I2cX->CR1 &= ~CR1_NOSTRETCH;
    else
        I2cX->CR1 |= CR1_NOSTRETCH;
    
    if(config->enableGeneralCall)
        I2cX->CR1 |= CR1_ENGC;
    else
        I2cX->CR1 &= ~CR1_ENGC;
    
    if(config->enablePEC)
        I2cX->CR1 |= CR1_ENPEC;
    else
        I2cX->CR1 &= ~CR1_ENPEC;
    
    I2cX->CR1 &= ~CR1_SMBUS;
    
    //set address
    if(config->addressBits == I2C_Address_7Bits)
    {
        I2cX->OAR1 &= ~OAR1_ADDMODE;
        I2cX->OAR1 &= ~0xfe;
        I2cX->OAR1 |= (config->selfAddress << 1);
    }
    else
    {
        I2cX->OAR1 |= OAR1_ADDMODE;
        I2cX->OAR1 &= ~0x3ff;
        I2cX->OAR1 |= (config->selfAddress & 0x3ff);
    }
    
    if(config->enableDualAddress)
    {
        I2cX->OAR2 |= OAR2_ENDUAL;
        I2cX->OAR2 &= ~0xfe;
        I2cX->OAR2 |= (config->selfAddressDual << 1);
    }
    else
        I2cX->OAR2 &= ~OAR2_ENDUAL;
    
    //config clock
    uint32 pclk = RCC_GetPCLK1();
    I2cX->CR2 &= ~CR2_FREQ;
    I2cX->CR2 |= (pclk / 1000000);

    //config i2c clock
    uint32 clock = config->clock;
    if(clock > 400000)
        clock = 400000;
    if(clock <= 100000)
    {
        //sm mode
        I2cX->CCR &= ~CCR_F_S;
        I2cX->TRISE = pclk / 1000000 + 1;
    }
    else
    {
        //fm mode
        I2cX->CCR |= CCR_F_S;
        I2cX->TRISE = 3 * pclk / 10000000 + 1;
    }
    
    I2cX->CCR &= ~CCR_DUTY;
    I2cX->CCR &= ~CCR_CCR;
    I2cX->CCR |= ((pclk / clock) >> 1);
    
    
    
}

/**
 * @brief init i2c parameter structure
 * @param i2c parameter structure
 */
void I2C_StructInit(__inout I2C_Config *config)
{
    assert_param(config != NULL);
    
    config->clock = 100000;
    config->addressBits = I2C_Address_7Bits;
    config->selfAddress = 0x00;
    config->selfAddressDual = 0x00;
    config->enableDualAddress = FALSE;
    config->enableStrech = TRUE;
    config->enableGeneralCall = FALSE;
    config->enablePEC = FALSE;
    config->enableDMA = FALSE;
}

/**
* @brief generate start
* @param i2c group
*/
void I2C_GenerateStart(__in I2C_Group group)
{
    assert_param(group < I2C_Count);
    
    I2C_T * const I2cX = I2Cx[group];
    //generate start
    I2cX->CR1 |= CR1_START;
}

/**
* @brief generate stop
* @param i2c group
*/
void I2C_GenerateStop(__in I2C_Group group)
{
    assert_param(group < I2C_Count);
    
    I2C_T * const I2cX = I2Cx[group];
    //generate stop
    I2cX->CR1 |= CR1_STOP;
}

/**
 * @brief enable or disable i2c interrupt
 * @param i2c group
 * @param interrupt flag
 * @param enable or disable flag
 */
void I2C_EnableInt(__in I2C_Group group, __in uint16 intFlag,
                         __in BOOL flag)
{
    assert_param(group < I2C_Count);
    assert_param(IS_I2C_IT_Param(intFlag));
    
    I2C_T * const I2cX = I2Cx[group];
    if(flag)
        I2cX->CR2 |= intFlag;
    else
        I2cX->CR2 &= ~intFlag;
}

/**
 * @brief write data to i2c bus
 * @param i2c group
 * @param data need to write
 */
void I2C_WriteData(__in I2C_Group group,  __in uint8 data)
{
    assert_param(group < I2C_Count);
    
    I2C_T * const I2cX = I2Cx[group];
    
    I2cX->DR = data;
}

/**
 * @brief read data from i2c bus
 * @param i2c group
 * @return data return
 */
uint8 I2C_ReadData(__in I2C_Group group)
{
    assert_param(group < I2C_Count);
    
    I2C_T * const I2cX = I2Cx[group];
    
    return I2cX->DR;
}


/**
 * @brief check if i2c flag is set
 * @param i2c group
 * @param i2c flag
 * @return set or not
 */
BOOL I2C_Is_FlagSet(__in I2C_Group group, __in uint32 flag)
{
    assert_param(group < I2C_Count);
    
    I2C_T * const I2cX = I2Cx[group];
    if(flag & 0x80000000)
    {
        flag &= ~0x80000000;
        //SR2 register
        if(I2cX->SR2 & flag)
            return TRUE;
        else
            return FALSE;
    }
    else
    {
        //SR1 register
        if(I2cX->SR1 & flag)
            return TRUE;
        else
            return FALSE;
    }
}

/**
 * @brief get current i2c event
 * @param i2c group
 * @return i2c event 
 */
uint32 I2C_GetEvent(__in I2C_Group group)
{
    //assert_param(group < I2C_Count);
    
    I2C_T * const I2cX = I2Cx[group];
    uint32 event = 0;
    uint32 temp = 0;
    event = (I2cX->SR1 & 0xcf);
    temp = (I2cX->SR2 & 0x07);
    temp <<= 16;
    event |= temp;
    
    return event;
    
}

/**
 * @brief get i2c error
 * @param i2c group
 * @return i2c error
 */
uint32 I2C_GetError(__in I2C_Group group)
{
    assert_param(group < I2C_Count);
    
    I2C_T * const I2cX = I2Cx[group];
    return (I2cX->SR1 & 0xff00);
}

/**
 * @brief clear i2c error
 * @param i2c group
 * @param i2c error
 */
void I2C_ClearError(__in I2C_Group group, __in uint32 error)
{
    assert_param(group < I2C_Count);
    
    I2C_T * const I2cX = I2Cx[group];
    I2cX->SR1 &= ~error;;
}

/**
 * @brief ack enable or disable
 * @param i2c group
 * @param enable or disable flag
 */
void I2C_AckEnable(__in I2C_Group group, __in BOOL flag)
{
    assert_param(group < I2C_Count);
    
    I2C_T * const I2cX = I2Cx[group];
    if(flag)
        I2cX->CR1 |= CR1_ACK;
    else
        I2cX->CR1 &= ~CR1_ACK;
}