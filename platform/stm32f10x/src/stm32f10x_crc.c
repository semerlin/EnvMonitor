/**
* @file      stm32f10x_crc.c
* @brief     library of crc module
* @details   none
* @author    huangyang
* @date      2016-12-11
* @version   V0.0.0.1
* @par Copyright (c):
       HuangYang
* @par History:        
*   version: huangyang, 2016-12-11, create this module\n
*/
#include "stm32f10x_crc.h"
#include "stm32f10x_map.h"
#include "stm32f10x_cfg.h"


/* crc register structure */
typedef struct 
{
	volatile uint32 DR;
	volatile uint8 IDR;
	uint8 RESERVED0;
	uint16 RESERVED1;
	volatile uint32 CR;
}CRC_TypeDef;

/* init crc struct base address */
CRC_TypeDef *CRC = (CRC_TypeDef *)CRC_BASE;



/** 
 * @brief   reset DR value to 0xffffffff
 */
void CRC_ResetDR(void)
{
	CRC->CR = 0x01;	//CR×îµÍÎ»Ð´1
}


/**
 * @brief calculate one data crc value
 * *return 32-bit crc value
 */
uint32 CRC_Cal(__in uint32 data)
{
	CRC->DR = data;

	return CRC->DR;
}

/**
 * @brief calculate block data crc value
 * @param [in] inbuf: buffer hold data
 * @param [in] len: buffer length
 * @return 32-bit crc value
 */
uint32 CRC_CalBlock(__in uint32 *buf, __in uint32 len)
{
    assert_param(buf != NULL);

	while(len--)
	{
		CRC->DR = *buf++;	
	}

	return CRC->DR;
}


/**
 * @brief get previous crc value
 * @return previour crc value
 */
uint32 CRC_GetDR(void)
{
	return CRC->DR;
}


/**
 * @brief temporary store one byte
 * @param data: data need to hold
 */
void CRC_SetIDR(__in uint8 data)
{
	CRC->IDR = data;
}

/**
 * @brief get temporary store data
 * @return data value
 */
uint8 CRC_GetIDR(void)
{
	return CRC->IDR;
}




