/*********************************************************************************    
**********************************************************************************
**    FILE NAME:
**        logger_typ.h
**
**    DESCRIPTION:	Contains all logger types
**
**    DATE              AUTHOR              REF         REASON
**    ----              ------              ---         ------
**    06Feb03           svaid           ---         Original
**
**     Copyright (C) 2006 Aricent Inc . All Rights Reserved
*******************************************************************************/
#ifndef __LOGGER_TYPE_H__
#define __LOGGER_TYPE_H__


/** Types *******************************************************************/
typedef unsigned int   U32;
typedef unsigned short  U16;
typedef unsigned char   U8;
typedef signed int     S32;
typedef signed short    S16;
typedef signed char     S8;

typedef enum
{
    LOGGER_FAILURE = 0,
    LOGGER_SUCCESS = 1
} logger_return_t;

typedef enum
{
    LOGGER_FALSE = 0,
    LOGGER_TRUE = 1
} logger_boolean_t;

#endif
