/************************************************************************************
*
*   FILE NAME  : callbacksVars.h
*
*   DESCRIPTION: Variable-related callbacks definitions
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

#ifndef __CALLBACKS_VARS_H__
#define __CALLBACKS_VARS_H__

/************************************************************************************
 * Includes
 ************************************************************************************/

#include "simTypes.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

#define SIM_VAR_NAME_LEN 32
#define SIM_VAR_MAX_SIZE /*(1024*10)*/0xFFFE

/* Maximum number of variables */
#define SIM_VARS_NUM 64

#define INVALID_VAR_IDX (-1)

/****************************************************************************
 * Types
 ****************************************************************************/

typedef struct VarInfoT {
    UChar8  name[SIM_VAR_NAME_LEN+1];
    UInt16  size;
    UInt8   *val;
    UInt8	is_static;
} VarInfo;

/****************************************************************************
 * Functions declaration
 ****************************************************************************/

/************************************************************************************
 * FUNCTION NAME    :   InitVars
 * DESCRIPTION      :   Init variables unit
 * PARAMATERS       :
 * RETURN VALUE     :
 ************************************************************************************/
void InitVars(void);

/************************************************************************************
 * FUNCTION NAME    :   userSetVarCallback
 * DESCRIPTION      :   "setvar" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userSetVarCallback(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userSetVarStaticCallback
 * DESCRIPTION      :   "setvarstatic" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userSetVarStaticCallback(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userPrintVarCallback
 * DESCRIPTION      :   "printVar" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userPrintVarCallback(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userClearVarCallback
 * DESCRIPTION      :   "clearVar" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userClearVarCallback(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userClearVarsStaticCallback
 * DESCRIPTION      :   "clearVarsStatics" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userClearVarsStaticCallback(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   varFindIdx
 * DESCRIPTION      :   Returns index of existing variable with given 'name'
 * PARAMATERS       :   [IN] const UChar8 *name
 * RETURN VALUE     :   Returns index of existing variable with given 'name',
 *                      INVALID_VAR_IDX if not found
 ************************************************************************************/
Int varFindIdx(const UChar8 *name);

/************************************************************************************
 * FUNCTION NAME    :   varAllocIdx
 * DESCRIPTION      :   Returns index of new allocated variable with given 'name'
 * PARAMATERS       :   [IN] const UChar8 *name
 * RETURN VALUE     :   Returns index of new allocated variable with given 'name',
 *                      INVALID_VAR_IDX if no free index
 ************************************************************************************/
Int varAllocIdx(const UChar8 *name);

/************************************************************************************
 * FUNCTION NAME    :   varIdxIsEmpty
 * DESCRIPTION      :   Checks if entry for given var_idx is empty
 * PARAMATERS       :   [IN] Int var_idx
 * RETURN VALUE     :   SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 varIdxIsEmpty(Int var_idx);

/************************************************************************************
 * FUNCTION NAME    :   varIsStatic
 * DESCRIPTION      :   Checks if variable for given var_idx is static
 * PARAMATERS       :   [IN] Int var_idx
 * RETURN VALUE     :   SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 varIsStatic(Int var_idx);

/************************************************************************************
 * FUNCTION NAME    :   varGetSize
 * DESCRIPTION      :   returns var size (in bytes)
 * PARAMATERS       :   [IN] Int var_idx
 * RETURN VALUE     :   var size / 0 if var not exists
 ************************************************************************************/
UInt16 varGetSize(Int var_idx);

/************************************************************************************
 * FUNCTION NAME    :   varExpandVars
 * DESCRIPTION      :   expands variable values from src_p to dest_p
 * PARAMATERS       :   [IN] const UChar8 *src_p
 *                      [OUT] UChar8* dest_p
 * RETURN VALUE     :   SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 varExpandVars(UChar8* dest_p,const UChar8 *src_p);

#endif /*__CALLBACKS_VARS_H__*/
