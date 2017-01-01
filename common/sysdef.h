#ifndef _SYSDEFINE_H_
  #define _SYSDEFINE_H_


/*****************************************
* global type define
******************************************/

/*****************************************
* int definition
******************************************/
typedef unsigned char uint8; 
typedef signed char int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned int uint32;
typedef signed int int32;
typedef unsigned long long uint64;
typedef signed long long int64;


/*****************************************
* pointer definition
******************************************/
typedef unsigned char* puint8;
typedef signed char* pint8;
typedef unsigned short* puint16;
typedef signed short* pint16;
typedef unsigned int* puint32;
typedef signed int* pint32;
typedef unsigned long* puint64;
typedef signed long* pint64;

/* device handler */
typedef void *Handle;


/********************************************
* inline definition
*********************************************/
#ifndef __INLINE
  #define __INLINE inline
#endif

/* asm definition */
#ifndef __ASM
  #define __ASM __asm
#endif
/********************************************
* input and output flag
*********************************************/
#define __in
#define __out
#define __inout


//get array length
#define GET_ARRAYLEN(x) (sizeof(x)/sizeof(x[0]))

#ifndef NULL
  #define NULL ((void *)0)
#endif


/* bool definition */
typedef enum 
{
    FALSE = 0, TRUE = !FALSE,
}BOOL;

typedef enum
{
	DISABLE = 0, ENABLE = !DISABLE,
}FunctionStatus;

typedef enum
{
	RESET = 0, SET = !RESET,
}FlagStatus;

typedef enum
{
	ERROR = 0, SUCCESS = !ERROR,
}ErrorStatus;

typedef enum
{
    WRITE = 0, READ,
}OptionStatus;

/* unused parameter declare */
#ifndef UNUSED
 #define UNUSED(x) ((void)x)
#endif
















#endif

