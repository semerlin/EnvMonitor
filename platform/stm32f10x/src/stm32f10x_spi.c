#include "stm32f10x_spi.h"
#include "stm32f10x_map.h"
#include "stm32f10x_cfg.h"
#include "stm32f10x_rcc.h"

/* spi structure */
typedef struct 
{
    volatile uint16 CR1;
    uint16 RESERVED0;
    volatile uint16 CR2;
    uint16 RESERVED1;
    volatile uint16 SR;
    uint16 RESERVED2;
    volatile uint16 DR;
    uint16 RESERVED3;
    volatile uint16 CRCPR;
    uint16 RESERVED4;
    volatile uint16 RXCRCR;
    uint16 RESERVED5;
    volatile uint16 TXCRCR;
    uint16 RESERVED6;
    volatile uint16 I2SCFGR;
    uint16 RESERVED7;
    volatile uint16 I2SPR;
    uint16 RESERVED8;
}SPI_T;

/* SPI group array */
static SPI_T * const SPIx[] = {(SPI_T *)SPI1_BASE, 
                               (SPI_T *)SPI2_BASE,
                               (SPI_T *)SPI3_BASE};

/* SPI register bit definition */
#define CR1_MSTR   (1 << 2)
#define CR1_BR     (0x07 << 3)
#define CR1_BIDIMODE  (1 << 15)
#define CR1_BIDIOE (1 << 14)
#define CR1_CRCEN (1 << 13)
#define CR1_CRCNEXT (1 << 12)
#define CR1_DFF (1 << 11)
#define CR1_RXONLY (1 << 10)
#define CR1_SSM (1 << 9)
#define CR1_SSI (1 << 8)
#define CR1_LSBFIRST (1 << 7)
#define CR1_SPE (1 << 6)
#define CR1_CPOL  (1 << 1)
#define CR1_CPHA (0x01)

#define CR2_SSOE (1 << 2)
#define CR2_TXDMAEN (1 << 1)
#define CR2_RXDMAEN (0x01)

#define SR_RXNE   (0x01)
#define SR_TXE    (1 << 1)
#define SR_BSY    (1 << 7)

/**
 * @brief enable or disable spi
 * @param spt group
 * @param enable or disable flag
 */
void SPI_Enable(__in SPI_Group group, __in BOOL flag)
{
    assert_param(group < SPI_Count);
    SPI_T * const SpiX = SPIx[group];
    
    if(flag)
        SpiX->CR1 |= CR1_SPE;
    else
    {
        if(SpiX->CR1 & CR1_SPE)
        {
            if(((SpiX->CR1 & CR1_BIDIMODE) == 0) &&
               ((SpiX->CR1 & CR1_RXONLY) == 0))
            {
                while(SpiX->SR & SR_BSY);
            }
            else if(((SpiX->CR1 & CR1_BIDIMODE) == 1) &&
                    ((SpiX->CR1 & CR1_BIDIOE) == 1))
            {
                while(SpiX->SR & SR_BSY);
            }
            
            SpiX->CR1 &= ~CR1_SPE;
        }
    }
}

/**
 * @brief setup spi
 * @param spi group
 * @param configure parameters
 */
void SPI_Setup(__in SPI_Group group, __in const SPI_Config *config)
{
    assert_param(group < SPI_Count);
    assert_param(IS_SPI_MainMode_Param(config->mainMode));
    assert_param(IS_SPI_SubMode_Param(config->subMode));
    assert_param(IS_SPI_Clk_Param(config->clock));
    assert_param(IS_SPI_Polority_Param(config->polarity));
    assert_param(IS_SPI_Phase_Param(config->phase));
    assert_param(IS_SPI_DataLength_Param(config->length));
    assert_param(IS_SPI_DataFormat_Param(config->format));
    assert_param(IS_SPI_NSS_Param(config->nssMode));
    
    SPI_T * const SpiX = SPIx[group];
    
    //set spi master or slave mode
    SpiX->CR1 &= ~CR1_MSTR;
    SpiX->CR1 |= config->mainMode;
    
    //set detail mode
    SpiX->CR1 &= ~(CR1_BIDIMODE | CR1_BIDIOE | CR1_RXONLY);
    SpiX->CR1 |= config->subMode;
    
    //set clock
    uint32 pclk = RCC_GetPCLK2();
    switch(group)
    {
    case SPI1:
        pclk = RCC_GetPCLK2();
        break;
    case SPI2:
    case SPI3:
        pclk = RCC_GetPCLK1();
        break;
    default:
        break;
    }
    SpiX->CR1 &= ~CR1_BR;
    SpiX->CR1 |= config->clock;
    
    //set polarity
    SpiX->CR1 &= ~CR1_CPOL;
    SpiX->CR1 |= config->polarity;
    
    //set phase
    SpiX->CR1 &= ~CR1_CPHA;
    SpiX->CR1 |= config->phase;

    //set data length
    SpiX->CR1 &= ~CR1_DFF;
    SpiX->CR1 |= config->length;
    
    //set frame format
    SpiX->CR1 &= ~CR1_LSBFIRST;
    SpiX->CR1 |= config->format;
    
    //set nss mode
    if(config->nssMode == SPI_NSS_SOFTWARE)
    {
        SpiX->CR1 |= CR1_SSM;
        if(config->mainMode == SPI_MainMode_Master)
            SpiX->CR1 |= CR1_SSI;
        else
            SpiX->CR1 &= ~CR1_SSI;
    }
    else
    {
        SpiX->CR1 &= ~CR1_SSM;
        if(config->mainMode == SPI_MainMode_Master)
            SpiX->CR2 |= CR2_SSOE;
        else
            SpiX->CR2 &= ~CR2_SSOE;
    }
    
    //set crc
    if(config->useCrc)
        SpiX->CR1 |= CR1_CRCEN;
    else
        SpiX->CR1 &= ~CR1_CRCEN;
    
    
}

/**
 * @brief init spi config structure 
 * @param spi config structure
 */
void SPI_StructInit(__inout SPI_Config *config)
{
    assert_param(config != NULL);
    config->mainMode = SPI_MainMode_Master;
    config->subMode = SPI_SubMode_Full_Duplex;
    config->clock = SPI_Clk_Divided_2;
    config->polarity = SPI_Polarity_High;
    config->phase = SPI_Phase_FirstClk;
    config->format = SPI_DataFormat_MSB;
    config->length = SPI_DataLength_8Bits;
    config->nssMode = SPI_NSS_SOFTWARE;
    config->useCrc = FALSE;
}

/**
 * @brief enable or disable spi interrupt
 * @param spi group
 * @param interrupt name
 * @param enable or disable flag
 */
void SPI_EnableInt(__in SPI_Group group, __in uint8 intFlag, __in BOOL flag)
{
    assert_param(group < SPI_Count);
    assert_param(IS_SPI_IT_Param(intFlag));
    
    SPI_T * const SpiX = SPIx[group];
    if(flag)
        SpiX->CR2 |= intFlag;
    else
        SpiX->CR2 &= ~intFlag;
    
}

/**
 * @brief check if specified flag is on 
 * @param spi group
 * @param flag need to check
 * @return flag status
 */
BOOL SPI_IsFlagOn(__in SPI_Group group, __in uint8 flag)
{   
    assert_param(group < SPI_Count);
    assert_param(IS_SPI_Flag_Param(flag));
    
    SPI_T * const SpiX = SPIx[group];
    
    if((SpiX->SR & flag) != 0)
        return TRUE;
    else
        return FALSE;
}

/**
 * @brief set next character is crc 
 * @param spi group
 * @param next character is crr or not 
 */
void SPI_SetCRCNext(__in SPI_Group group,__in BOOL flag)
{
    assert_param(group < SPI_Count);
    
    SPI_T * const SpiX = SPIx[group];
    if(flag)
        SpiX->CR1 |= CR1_CRCNEXT;
    else
        SpiX->CR1 &= ~CR1_CRCNEXT;
}

/**
 * @brief enable tx transfer use dma or not 
 * @param spi group
 * @param enable or not
 */
void SPI_EnableTxDMA(__in SPI_Group group, __in BOOL flag)
{
    assert_param(group < SPI_Count);
    
    SPI_T * const SpiX = SPIx[group];
    if(flag)
        SpiX->CR2 |= CR2_TXDMAEN;
    else
        SpiX->CR2 &= ~CR2_TXDMAEN;
}

/**
 * @brief enable rx transfer use dma or not 
 * @param spi group
 * @param enable or not
 */
void SPI_EnableRxDMA(__in SPI_Group group, __in BOOL flag)
{
    assert_param(group < SPI_Count);
    
    SPI_T * const SpiX = SPIx[group];
    if(flag)
        SpiX->CR2 |= CR2_RXDMAEN;
    else
        SpiX->CR2 &= ~CR2_RXDMAEN;
}

/**
 * @brief write data to spi port 
 * @param spi group
 * @param data to write
 */
void SPI_WriteData(__in SPI_Group group, __in uint16 data)
{
    assert_param(group < SPI_Count);
    
    SPI_T * const SpiX = SPIx[group];
    SpiX->DR = data;
}


/**
 * @brief write and read data to spi port synchronization
 * @param spi group
 * @param data to write
 */
uint16 SPI_WriteReadDataSync(__in SPI_Group group, __in uint16 data)
{
    assert_param(group < SPI_Count);
    
    SPI_T * const SpiX = SPIx[group];
    //wait tx complete
    while(!(SpiX->SR & SPI_Flag_TXE));
    SpiX->DR = data;
    while(!(SpiX->SR & SPI_Flag_RXNE));
    return SpiX->DR;
}
/**
 * @brief read data from spi port 
 * @param spi group
 * @return data return 
 */
uint16 SPI_ReadData(__in SPI_Group group)
{
    assert_param(group < SPI_Count);
    
    SPI_T * const SpiX = SPIx[group];
    return SpiX->DR;
}

/**
 * @brief set crc polynomial 
 * @param spi group
 * @param crc polynomial
 */
void SPI_SetCRCPolynomial(__in SPI_Group group, __in uint16 data)
{
    assert_param(group < SPI_Count);
    
    SPI_T * const SpiX = SPIx[group];
    SpiX->CRCPR = data;
}

/**
 * @brief  get rx crc value
 * @param spi group
 * @return rx crc value
 */
uint16 SPI_GetRxCRC(__in SPI_Group group)
{
    assert_param(group < SPI_Count);
    
    SPI_T * const SpiX = SPIx[group];
    return SpiX->RXCRCR;
}

/**
 * @brief get tx crc value
 * @param spi group
 * @return tx crc value
 */
uint16 SPI_GetTxCRC(__in SPI_Group group)
{
    assert_param(group < SPI_Count);
    
    SPI_T * const SpiX = SPIx[group];
    return SpiX->RXCRCR;
}
