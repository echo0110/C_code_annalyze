
/************************************************************************************
*
*   FILE NAME  : call_func.h
*
*   DESCRIPTION: External scheduler command line interface definition.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc.
*
*************************************************************************************/

#ifndef __CALL_FUNC_H__
#define __CALL_FUNC_H__

/************************************************************************************
 * Includes
 ************************************************************************************/

#include "lteTypes.h"

/************************************************************************************
 * FUNCTION NAME    :   userCmdlineParse
 * DESCRIPTION      :   Getting parameters from command line and calling callbacks
 *                      from table. Parameters get line by line in forever loop
 * PARAMATERS       :   None
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userCmdlineParse();

/************************************************************************************
 * FUNCTION NAME    :   userCallCallback
 * DESCRIPTION      :   Scans table and call selected callback
 * PARAMATERS       :   [IN] UChar8* input_string
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userCallCallback(UChar8* input_string);

#endif /*__CALL_FUNC_H__*/
