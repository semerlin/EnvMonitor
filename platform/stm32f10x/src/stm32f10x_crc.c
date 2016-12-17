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
	volatile uint32 IDR;
	volatile uint32 CR;
}CRC_T;

/* init crc struct base address */
CRC_T *CRC = (CRC_T *)CRC_BASE;


/* flash register's bit band area */
#define CRC_OFFSET (CRC_BASE - PERIPH_BASE)
/*  CR bit band */
#define CR_OFFSET (CRC_OFFSET + 0x08)
#define CR_RESET (PERIPH_BB_BASE + CR_OFFSET * 32 + 0x00 * 4)


/** 
 * @brief   reset DR value to 0xffffffff
 */
void CRC_ResetDR(void)
{
    *((volatile uint32 *)CR_RESET) = 0x01;
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




