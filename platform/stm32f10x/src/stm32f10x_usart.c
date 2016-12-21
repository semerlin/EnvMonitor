#include "stm32f10x_usart.h"
#include "stm32f10x_map.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_cfg.h"


/* flash register structure */
typedef struct 
{
    volatile uint16 SR;
    uint16 RESERVED0;
    volatile uint16 DR;
    uint16 RESERVED1;
    volatile uint16 BRR;
    uint16 RESERVED2;
    volatile uint16 CR1;
    uint16 RESERVED3;
    volatile uint16 CR2;
    uint16 RESERVED4;
    volatile uint16 CR3;
    uint16 RESERVED5;
    volatile uint16 GTPR;
    uint16 RESERVED6;
}USART_T;

/* usart definition */
#define UE (1 << 13)
#define TE (1 << 3)
#define RE (1 << 2)

#define ADD (0x0f)
#define PSC (0xff)

#define M  (1 << 12)
#define PARITY  (0x03 << 9)

#define STOP (0x03 << 12)
#define CLKEN (0x01 << 11)
#define CPOL (0x01 << 10)
#define CPHA (0x01 << 9)
#define LBCL (1 << 8)

#define CRTS (0x03 << 8)

#define WAKE (1 << 11)

#define RWU (1 << 1)


/* GPIO group array */
static USART_T * const USARTx[] = {(USART_T *)USART1_BASE, 
                                   (USART_T *)USART2_BASE,
                                   (USART_T *)USART3_BASE};


/**
 * @brief enable or disable usart
 * @param group: usart group
 * @param flag: TRUE: enable FALSE:disable
 */
void USART_Enable(__in USART_Group group, __in BOOL flag)
{
    assert_param(group < UASRT_Count);
    
    USART_T * const UsartX = USARTx[group]; 
    if(flag)
        UsartX->CR1 |= UE;
    else
        UsartX->CR1 &= ~UE;
}

/**
 * @brief enable or disable usart transfer
 * @param group: usart group
 * @param flag: TRUE: enable FALSE:disable
 */
void USART_TransEnable(__in USART_Group group, __in BOOL flag)
{
    assert_param(group < UASRT_Count);
    
    USART_T * const UsartX = USARTx[group];
    if(flag)
        UsartX->CR1 |= TE;
    else
        UsartX->CR1 &= ~TE;
}

/**
 * @brief enable or disable usart recive
 * @param group: usart group
 * @param flag: TRUE: enable FALSE:disable
 */
void USART_RecvEnable(__in USART_Group group, __in BOOL flag)
{
    assert_param(group < UASRT_Count);
    
    USART_T * const UsartX = USARTx[group];
    if(flag)
        UsartX->CR1 |= RE;
    else
        UsartX->CR1 &= ~RE;
}

void USART_Setup(__in USART_Group group, __in const USART_Config *config)
{
    assert_param(group < UASRT_Count);
    assert_param(config != NULL);
    assert_param((config->baudRate <= 4500000) && (config->baudRate != 0));
    assert_param(IS_USART_WORD_LENGTH(config->wordLength));
    assert_param(IS_USART_STOPBITS(config->stopBits));
    assert_param(IS_USART_PARITY(config->parity));
    assert_param(IS_USART_HARDWARE_FLOW_CONTROL(config->hardwareFlowControl));
    assert_param(IS_USART_CPOL(config->clkPolarity));
    assert_param(IS_USART_CPHA(config->clkPhase));
   
    USART_T * const UsartX = USARTx[group];
    
    uint32 pclk = 0;
    if(group == USART1)
        pclk = RCC_GetPCLK2();
    else
        pclk = RCC_GetPCLK1();
    
    //config baudrate
    uint16 divFraction = 0;
    uint16 divMantissa = 0;
    float divVal = pclk / (float)(config->baudRate);
    divMantissa = (uint16)(divVal / 16);
    divFraction = (uint8)((divVal - divMantissa * 16));
    if(((divVal - divMantissa * 16) - divFraction) >= 0.5)
        divFraction += 1;
    
    UsartX->BRR = (divMantissa << 4) + divFraction;
    
    //config word lenght, tx/rx enable, parity
    UsartX->CR1 &= ~(M | TE | RE | PARITY);
    UsartX->CR1 |= (config->wordLength | config->parity);
    if(config->txEnable)
        UsartX->CR1 |= TE;
    if(config->rxEnable)
        UsartX->CR1 |= RE;
    
    //config stop bits, CK
    UsartX->CR2 &= ~(CLKEN | CPOL | CPHA | STOP | LBCL);
    UsartX->CR2 |= (config->stopBits | config->clkPolarity | config->clkPhase);
    if(config->clkEnable)
        UsartX->CR2 |= CLKEN;
    if(config->lastBitClkEnable)
        UsartX->CR2 |= LBCL;
    
    UsartX->CR3 &= ~CRTS;
    UsartX->CR3 |= config->hardwareFlowControl;

}

void USART_StructInit(__inout USART_Config *config)
{   
    config->baudRate = 9600;
    config->wordLength = USART_WordLength_8;
    config->parity = USART_Parity_None;
    config->stopBits = USART_StopBits_1;
    config->hardwareFlowControl = USART_HardwareFlowControl_None;
    config->txEnable = TRUE;
    config->rxEnable = TRUE;
    config->clkEnable = FALSE;
    config->clkPolarity = USART_CPOL_Low;
    config->clkPhase = USART_CPHA_1Edge;
    config->lastBitClkEnable = FALSE;
}

/**
 * @brief set usart address
 * @param group: usart group
 * @param address
 */
void USART_SetAddress(__in USART_Group group, __in uint8 address)
{
    assert_param(group < UASRT_Count);
    assert_param(address < 16);
    
    USART_T * const UsartX = USARTx[group];
    UsartX->CR2 &= ~ADD;
    UsartX->CR2 |= address;
}

/**
 * @brief get usart address
 * @param group: usart group
 * @return address
 */
uint8 USART_GetAddress(__in USART_Group group)
{
    assert_param(group < UASRT_Count);
    
    USART_T * const UsartX = USARTx[group];
    return ((uint8)(UsartX->CR2 & ADD));
}

/**
 * @param set usart clock prescaler
 * @param group: usart group
 * @param prescaler
 */
void USART_SetPrescaler(__in USART_Group group, __in uint8 prescaler)
{
    assert_param(group < UASRT_Count);
    
    USART_T * const UsartX = USARTx[group];
    UsartX->GTPR &= ~PSC;
    UsartX->GTPR |= prescaler;
}

/**
 * @param check uart flag status
 * @param group: usart group
 * @param flag: flag position
 * @return TRUE: flag is set FALSE: flag is not set
 */
BOOL USART_IsFlagOn(__in USART_Group group, __in uint16 flag)
{
    assert_param(group < UASRT_Count);
    assert_param(IS_USART_FLAG(flag));
    
    USART_T * const UsartX = USARTx[group];
    if(UsartX->SR & flag)
        return TRUE;
    else
        return FALSE;
}

/**
 * @param clear uart flag status
 * @param group: usart group
 * @param flag: flag position
 */
void USART_ClearFlag(__in USART_Group group, __in uint16 flag)
{
    assert_param(group < UASRT_Count);
    assert_param(IS_USART_FLAG(flag));
    
    USART_T * const UsartX = USARTx[group];
    
    UsartX->SR &= ~flag;
}

/**
 * @param write data
 * @param group: usart group
 * @param data
 */
void USART_WriteData(__in USART_Group group, __in uint8 data)
{
    assert_param(group < UASRT_Count);
    
    USART_T * const UsartX = USARTx[group];
    
    UsartX->DR = data;
}

/**
 * @param get data
 * @param group: usart group
 * @return data value
 */
uint8 USART_ReadData(__in USART_Group group)
{
    assert_param(group < UASRT_Count);
    
    USART_T * const UsartX = USARTx[group];
    
    return UsartX->DR;
}

/**
 * @param set usart wakeup mode
 * @param group: usart group
 * @return wakeup mode
 */
void USART_SetWakeupMethod(__in USART_Group group, __in uint16 method)
{
    assert_param(group < UASRT_Count);
    assert_param(IS_USART_WAKEUP(method));
    
    USART_T * const UsartX = USARTx[group];
    
    UsartX->CR1 &= ~WAKE;
    UsartX->CR1 |= method;
}

/**
 * @param enable usart interrupt
 * @param interrupt flag
 * @param enable or disable flag
 */
uint8 val = 0;
void USART_EnableInt(__in USART_Group group, __in uint8 intFlag, __in BOOL flag)
{
    assert_param(group < UASRT_Count);
    assert_param(IS_USART_IT(intFlag));
    
    USART_T * const UsartX = USARTx[group];
    
    val = (intFlag >> 1);
    switch(intFlag & 0x01)
    {
    case 0x01:
        UsartX->CR1 &= ~(1 << (intFlag >> 4));
        UsartX->CR1 |= (1 << (intFlag >> 4));
        break;
    case 0x02:
        UsartX->CR2 &= ~(1 << (intFlag >> 4));
        UsartX->CR2 |= (1 << (intFlag >> 4));
        break;
    case 0x03:
        UsartX->CR3 &= ~(1 << (intFlag >> 4));
        UsartX->CR3 |= (1 << (intFlag >> 4));
        break;
    default:
        break;
    }
}


/**
 * @param set usart receive mode
 * @param group: usart group
 * @param mode
 */
void USART_SetRecvMode(__in USART_Group group, __in uint8 mode)
{
    assert_param(group < UASRT_Count);
    assert_param(IS_USART_RECVMODE(mode));
    
    USART_T * const UsartX = USARTx[group];
    
    UsartX->CR1 &= ~RWU;
    UsartX->CR1 |= mode;
}

/**
 * @param set usart break character
 * @param group: usart group
 */
void USART_SendBreakCharacter(__in USART_Group group)
{
    assert_param(group < UASRT_Count);
    
    USART_T * const UsartX = USARTx[group];
    
    UsartX->CR1 |= 0x01;
}

/**
 * @param enable or disable usart LIN mode
 * @param group: usart group
 */
void USART_EnableLINMode(__in USART_Group group, __in BOOL flag)
{
    assert_param(group < UASRT_Count);
    
    USART_T * const UsartX = USARTx[group];
    
    if(flag)
        UsartX->CR2 |= (1 << 14);
    else
        UsartX->CR2 &= ~(1 << 14);
}

/**
 * @param set line break detect length
 * @param group: usart group
 * @param delect length
 */
void USART_SetLineBreakDetectLength(__in USART_Group group, __in uint8 length)
{
    assert_param(group < UASRT_Count);
    assert_param(IS_USART_LIN_BREAK_DETECT_LENGTH(length));
    
    USART_T * const UsartX = USARTx[group];
    UsartX->CR2 &= ~(1 << 1);
    UsartX->CR2 |= length;
}

/**
 * @param enable or disable dma tx
 * @param group: usart group
 * @param enable or disable flag 
 */
void USART_EnableDMATX(__in USART_Group group, __in BOOL flag)
{
    assert_param(group < UASRT_Count);
    
    USART_T * const UsartX = USARTx[group];
    
    if(flag)
        UsartX->CR3 &= ~(1 << 7);
    else
        UsartX->CR3 |= (1 << 7);
}

/**
 * @param enable or disable dma rx
 * @param group: usart group
 * @param enable or disable flag 
 */
void USART_EnableDMARX(__in USART_Group group, __in BOOL flag)
{
    assert_param(group < UASRT_Count);
    
    USART_T * const UsartX = USARTx[group];
    
    if(flag)
        UsartX->CR3 &= ~(1 << 6);
    else
        UsartX->CR3 |= (1 << 6);
}

/**
 * @param enable or disable smartcard
 * @param group: usart group
 * @param enable or disable flag 
 */
void USART_SmartcardEnable(__in USART_Group group, __in BOOL flag)
{
    assert_param(group < UASRT_Count);
    
    USART_T * const UsartX = USARTx[group];
    if(flag)
        UsartX->CR3 &= ~(1 << 5);
    else
        UsartX->CR3 |= (1 << 5);
}

/**
 * @param enable or disable smartcard NACK
 * @param group: usart group
 * @param enable or disable flag 
 */
void USART_SmartcardNACKEnable(__in USART_Group group, __in BOOL flag)
{
    assert_param(group < UASRT_Count);
    
    USART_T * const UsartX = USARTx[group];
    if(flag)
        UsartX->CR3 &= ~(1 << 4);
    else
        UsartX->CR3 |= (1 << 5);
}

/**
 * @param enable or disable half duplex selection
 * @param group: usart group
 * @param enable or disable flag 
 */
void USART_HalfDuplexSelection(__in USART_Group group, __in BOOL flag)
{
    assert_param(group < UASRT_Count);
    
    USART_T * const UsartX = USARTx[group];
    if(flag)
        UsartX->CR3 &= ~(1 << 3);
    else
        UsartX->CR3 |= (1 << 5);
}

/**
 * @param enable or disable irda
 * @param group: usart group
 * @param enable or disable flag 
 */
void USART_IrdaEnable(__in USART_Group group, __in BOOL flag)
{
    assert_param(group < UASRT_Count);
    
    USART_T * const UsartX = USARTx[group];
    if(flag)
        UsartX->CR3 &= ~(1 << 1);
    else
        UsartX->CR3 |= (1 << 1);
}

/**
 * @param set irda mode
 * @param group: usart group
 * @param work mode 
 */
void USART_SetIrdaMode(__in USART_Group group, __in uint8 mode)
{
    assert_param(group < UASRT_Count);
    assert_param(IS_USART_IRDA_MODE(mode));
    
    USART_T * const UsartX = USARTx[group];
    UsartX->CR3 &= ~(1 << 2);
    UsartX->CR3 |= mode;
}

void USART_SetGuardTime(__in USART_Group group, __in uint16 time)
{
    assert_param(group < UASRT_Count);
    
    USART_T * const UsartX = USARTx[group];
    UsartX->GTPR &= ~(0x0ff << 8);
    UsartX->GTPR |= (time << 8);
}
