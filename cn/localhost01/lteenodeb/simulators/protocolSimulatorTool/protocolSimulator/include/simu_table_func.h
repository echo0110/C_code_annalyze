/************************************************************************************
*
*   FILE NAME  : simu_table_func.h
*
*   DESCRIPTION: Table with callbacks definitions.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

#ifndef __SIMU_TABLE_FUNC_H__
#define __SIMU_TABLE_FUNC_H__

/************************************************************************************
 * Includes
 ************************************************************************************/

#include "simTypes.h"

typedef struct
{
        const UChar8* str_id;
        UInt32 (*pfunc)(UInt32 argc, const UChar8** argv);
} CallbackFuncId;


typedef struct
{
        const UChar8* str_id;
        UInt32 (*pfunc)(UInt32 argc, const UChar8** argv,
                UChar8** string_list,
                UInt32 *p_line_no);
} FlowCallbackFuncId;

typedef struct
{
        const UChar8* str_id;
        UInt32 (*pfunc)(UInt32 argc, const UChar8** argv,
        		UInt16 maxSize,
        		UInt8* msgBuffer_p,
        		UInt16* size,
        		UInt8* bit_pos,
        		UInt8* msgMask_p);
} OperationFuncId;

extern CallbackFuncId table_callbacks_func[];
extern FlowCallbackFuncId table_flow_callbacks_func[];
extern OperationFuncId table_operation_func[];

#endif /*__SIMU_TABLE_FUNC_H__*/
