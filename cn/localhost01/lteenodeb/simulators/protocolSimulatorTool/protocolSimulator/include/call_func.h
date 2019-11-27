/************************************************************************************
*
*   FILE NAME  : call_func.h
*
*   DESCRIPTION: External scheduler command line interface definition.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

#ifndef __CALL_FUNC_H__
#define __CALL_FUNC_H__

/************************************************************************************
 * Includes
 ************************************************************************************/

#include "simTypes.h"

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
 * PARAMATERS       :   [IN] UChar8** string_list
 *                      [IN/OUT]UInt32 *p_line_no
 * RETURN VALUE     :   [OUT] SIM_FALSE=no exit,SIM_TRUE=exit
 ************************************************************************************/
UInt32 userCallCallback(UChar8** string_list,UInt32 *p_line_no);

#endif /*__CALL_FUNC_H__*/
