
/************************************************************************************
*
*   FILE NAME  : simu_table_func.h
*
*   DESCRIPTION: Table with callbacks definitions.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc.
*
*************************************************************************************/

#ifndef __SIMU_TABLE_FUNC_H__
#define __SIMU_TABLE_FUNC_H__

/************************************************************************************
 * Includes
 ************************************************************************************/

#include "lteTypes.h"

/************************************************************************************
 * STRUCT NAME  :   FuncId
 * DESCRIPTION  :   Used for making connection between identifier of callback
 *                  function and pointer to this callback function
 * FIELDS       :   const UChar8* str_id - identifier of callback function
 *                  UInt32 (*pfunc)(UInt32 argc, const UChar8** argv) - pointer to 
 *                  callback function
 ************************************************************************************/
typedef struct FuncIdT
{
        const UChar8* str_id; 
        UInt32 (*pfunc)(UInt32 argc, const UChar8** argv);
} FuncId;

extern FuncId table_func[];

#endif /*__SIMU_TABLE_FUNC_H__*/
