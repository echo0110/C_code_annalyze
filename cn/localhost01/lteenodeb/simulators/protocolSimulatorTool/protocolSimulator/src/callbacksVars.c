/************************************************************************************
*
*   FILE NAME  : callbacksVars.c
*
*   DESCRIPTION: Variable-related callbacks definitions
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

/************************************************************************************
 * Includes
 ************************************************************************************/

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "simLogger.h"

#include "callbacksVars.h"
#include "userUtils.h"
#include "call_func.h"
#include "testStatistic.h"

/************************************************************************************
 * Definitions
 ************************************************************************************/

/************************************************************************************
 * Global variable
 ************************************************************************************/

VarInfo vars_db[SIM_VARS_NUM];

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static void setVarCmd(const UChar8 *name,const UChar8 *str_val,UInt16 size,UInt8 is_static);
static void varClear(Int var_idx);
static void userPrintVarCmd(Int var_idx);

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/****************************************************************************
 * Functions implementation
 ****************************************************************************/

/************************************************************************************
 * FUNCTION NAME    :   InitVars
 * DESCRIPTION      :   Init variables unit
 * PARAMATERS       :
 * RETURN VALUE     :
 ************************************************************************************/
void InitVars(void)
{
    memset(vars_db,0,sizeof(vars_db));
}

/************************************************************************************
 * FUNCTION NAME    :   userSetVarCallback
 * DESCRIPTION      :   "setvar" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userSetVarCallback(UInt32 argc, const UChar8** argv)
{
    if ((argc < 2) || (argc > 3))
    {
        SIM_LOG(SIM_LOG_WARNING, "Wrong parameters. Should be 'setvar varname,value[,size (in bytes) to check]");
        return SIM_FALSE;
    }
    assert(argv);

    if(3==argc)
    {
        SIM_LOG(SIM_LOG_INFO, "COMMAND[ setvar ] %s,%s,%s", argv[0], argv[1], argv[2]);
        setVarCmd(argv[0],argv[1],atoi(argv[2]),SIM_FALSE);
    }
    else
    {
        SIM_LOG(SIM_LOG_INFO, "COMMAND[ setvar ] %s,%s", argv[0], argv[1]);
        setVarCmd(argv[0],argv[1],0,SIM_FALSE);
    }

    return SIM_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   userSetVarStaticCallback
 * DESCRIPTION      :   "setvarstatic" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userSetVarStaticCallback(UInt32 argc, const UChar8** argv)
{
    if ((argc < 2) || (argc > 3))
    {
        SIM_LOG(SIM_LOG_WARNING, "Wrong parameters. Should be 'setvarstatic varname,value[,size (in bytes) to check]");
        return SIM_FALSE;
    }
    assert(argv);

    if(3==argc)
    {
        SIM_LOG(SIM_LOG_INFO, "COMMAND[ setvarstatic ] %s,%s,%s", argv[0], argv[1], argv[2]);
        setVarCmd(argv[0],argv[1],atoi(argv[2]),SIM_TRUE);
    }
    else
    {
        SIM_LOG(SIM_LOG_INFO, "COMMAND[ setvarstatic ] %s,%s", argv[0], argv[1]);
        setVarCmd(argv[0],argv[1],0,SIM_TRUE);
    }

    return SIM_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   setVarCmd
 * DESCRIPTION      :   action to set variable value
 * PARAMATERS       :   [IN] UChar8 *name - variable name
 *                      [IN] UChar8 *str_val - string with variable value (hex string)
 *                      [IN] UInt16 size - variable size to check (0=no check)
 *                      [IN] UInt8 is_static - SIM_TRUE/SIM_FALSE - if var is static
 * RETURN VALUE     :
 ************************************************************************************/
static void setVarCmd(const UChar8 *name,const UChar8 *str_val,UInt16 size,UInt8 is_static)
{
    Int var_idx=0;
    UInt8 buf_p[SIM_VAR_MAX_SIZE];
    UInt16 buf_size=0;

    if(size>SIM_VAR_MAX_SIZE)
    {
        SIM_LOG(SIM_LOG_WARNING,"Wrong var size %i",size);
        return;
    }

    if(SIM_TRUE!=userParseMsgStr(str_val,SIM_VAR_MAX_SIZE,buf_p,&buf_size,PNULL))
    {
        SIM_LOG(SIM_LOG_WARNING, "%s:Error parsing variable '%s' value '%s'",__FUNCTION__,name,str_val);
        return;
    }

    var_idx=varFindIdx(name);
    if(INVALID_VAR_IDX!=var_idx)
    {
        varClear(var_idx);
    }

    var_idx=varAllocIdx(name);
    if(INVALID_VAR_IDX==var_idx)
    {
        SIM_LOG(SIM_LOG_CRITICAL,"Can't allocate index for variable '%s'",name);
        return;
    }

    if((size>0)&&(size!=buf_size))
    {
        SIM_LOG(SIM_LOG_WARNING, "%s:Error length variable '%s' value '%s', expected len(%i) != actual len(%i)",__FUNCTION__,name,str_val,size,buf_size);
        return;
    }
    size=buf_size;
    if(size<1)
    {
        SIM_LOG(SIM_LOG_WARNING,"Wrong var size %i",size);
        return;
    }

    vars_db[var_idx].val=(UInt8*)malloc(size);
    if(PNULL==vars_db[var_idx].val)
    {
        SIM_LOG(SIM_LOG_CRITICAL, "%s:can't allocate memory for variable, len=%i",__FUNCTION__,size);
        return;
    }

    memcpy(vars_db[var_idx].val,buf_p,size);
    strcpy(vars_db[var_idx].name,name);
    vars_db[var_idx].size=size;
    vars_db[var_idx].is_static=is_static;
}

/************************************************************************************
 * FUNCTION NAME    :   userPrintVarCallback
 * DESCRIPTION      :   "printVar" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userPrintVarCallback(UInt32 argc, const UChar8** argv)
{
    int var_idx=0;

    if (argc > 1)
    {
        SIM_LOG(SIM_LOG_WARNING, "Wrong parameters. Should be 'printvar [varname]'. Default = print all variables");
        return SIM_FALSE;
    }

    if(1==argc)
    {
        assert(argv);

        SIM_LOG(SIM_LOG_INFO, "COMMAND[ printvar ] %s", argv[0]);

        var_idx=varFindIdx(argv[0]);
        if(INVALID_VAR_IDX!=var_idx)
        {
            userPrintVarCmd(var_idx);
        }
        else
        {
            SIM_LOG(SIM_LOG_WARNING, "Variable '%s' not found",argv[0]);
        }
    }
    else
    {
        SIM_LOG(SIM_LOG_INFO, "COMMAND[ printvar ]");

        for(var_idx=0;var_idx<SIM_VARS_NUM;var_idx++)
        {
            if(SIM_FALSE==varIdxIsEmpty(var_idx))
            {
                userPrintVarCmd(var_idx);
            }
        }
    }

    return SIM_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   userPrintVarCmd
 * DESCRIPTION      :   prints variable information (for given var_idx)
 * PARAMATERS       :   [IN] Int var_idx
 * RETURN VALUE     :
 ************************************************************************************/
static void userPrintVarCmd(Int var_idx)
{
    if((var_idx>=0)&&(var_idx<SIM_VARS_NUM)&&(SIM_FALSE==varIdxIsEmpty(var_idx)))
    {
    	if(SIM_FALSE==varIsStatic(var_idx))
    	{
    		SIM_LOG(SIM_LOG_INFO, "Variable '%s' ",vars_db[var_idx].name);
    	}
    	else
    	{
    		SIM_LOG(SIM_LOG_INFO, "Variable '%s' (STATIC)",vars_db[var_idx].name);
    	}

        userPrintRawBuffer("Value",vars_db[var_idx].val,vars_db[var_idx].size,0);
    }
    else
    {
        SIM_LOG(SIM_LOG_INFO, "Invalid var_idx (%i)",var_idx);
    }
}

/************************************************************************************
 * FUNCTION NAME    :   userClearVarCallback
 * DESCRIPTION      :   "clearVar" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userClearVarCallback(UInt32 argc, const UChar8** argv)
{
    int var_idx=0;

    if (argc > 1)
    {
        SIM_LOG(SIM_LOG_WARNING, "Wrong parameters. Should be 'clearvar [varname]'. Default = clear all non-static variables");
        return SIM_FALSE;
    }

    if(1==argc)
    {
        assert(argv);

        SIM_LOG(SIM_LOG_INFO, "COMMAND[ clearvar ] %s", argv[0]);

        var_idx=varFindIdx(argv[0]);
        if(INVALID_VAR_IDX!=var_idx)
        {
            varClear(var_idx);
        }
        else
        {
            SIM_LOG(SIM_LOG_WARNING, "Variable '%s' not found",argv[0]);
        }
    }
    else
    {
        SIM_LOG(SIM_LOG_INFO, "COMMAND[ clearvar ]");

        /*clear all non-static vars*/
        for(var_idx=0;var_idx<SIM_VARS_NUM;var_idx++)
        {
            if( (SIM_FALSE==varIdxIsEmpty(var_idx)) && (SIM_FALSE==varIsStatic(var_idx)) )
            {
                varClear(var_idx);
            }
        }
    }

    return SIM_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   userClearVarsStaticCallback
 * DESCRIPTION      :   "clearVarsStatics" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userClearVarsStaticCallback(UInt32 argc, const UChar8** argv)
{
    int var_idx=0;

    if (argc != 0)
    {
        SIM_LOG(SIM_LOG_WARNING, "Wrong parameters. Should be 'clearvarsstatic'. clear all statics variables");
        return SIM_FALSE;
    }

	SIM_LOG(SIM_LOG_INFO, "COMMAND[ clearvarsstatic ]");

	/*clear all statics vars*/
	for(var_idx=0;var_idx<SIM_VARS_NUM;var_idx++)
	{
		if( (SIM_FALSE==varIdxIsEmpty(var_idx)) && (SIM_TRUE==varIsStatic(var_idx)) )
		{
			varClear(var_idx);
		}
	}

    return SIM_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   varFindIdx
 * DESCRIPTION      :   Returns index of existing variable with given 'name'
 * PARAMATERS       :   [IN] const UChar8 *name
 * RETURN VALUE     :   Returns index of existing variable with given 'name',
 *                      INVALID_VAR_IDX if not found
 ************************************************************************************/
Int varFindIdx(const UChar8 *name)
{
    Int idx;
    assert(name);
    for(idx=0;idx<SIM_VARS_NUM;idx++)
    {
		userTolower((UChar8*)name);
        if(0==strcmp(name,vars_db[idx].name))
        {
            return idx;
        }
    }
    return INVALID_VAR_IDX;
}

/************************************************************************************
 * FUNCTION NAME    :   varAllocIdx
 * DESCRIPTION      :   Returns index of new allocated variable with given 'name'
 * PARAMATERS       :   [IN] const UChar8 *name
 * RETURN VALUE     :   Returns index of new allocated variable with given 'name',
 *                      INVALID_VAR_IDX if no free index
 ************************************************************************************/
Int varAllocIdx(const UChar8 *name)
{
    Int idx;
    Int name_len=(Int)strlen(name);

	assert(name);

	if((name_len>SIM_VAR_NAME_LEN) || (name_len<1))
    {
        SIM_LOG(SIM_LOG_WARNING, "variable name '%s' length is incorrect (%i)",name,name_len);
        return INVALID_VAR_IDX;
    }
    for(idx=0; idx<SIM_VARS_NUM; idx++)
    {
        if(SIM_TRUE==varIdxIsEmpty(idx))
        {
            vars_db[idx].size=0;
            return idx;
        }
    }
    return INVALID_VAR_IDX;
}

/************************************************************************************
 * FUNCTION NAME    :   varIdxIsEmpty
 * DESCRIPTION      :   Checks if entry for given var_idx is empty
 * PARAMATERS       :   [IN] Int var_idx
 * RETURN VALUE     :   SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 varIdxIsEmpty(Int var_idx)
{
    assert((var_idx>=0)&&(var_idx<=SIM_VARS_NUM));

    if((0==vars_db[var_idx].name[0])&&(PNULL==vars_db[var_idx].val))
    {
        return SIM_TRUE;
    }
    else
    {
        return SIM_FALSE;
    }
}

/************************************************************************************
 * FUNCTION NAME    :   varIsStatic
 * DESCRIPTION      :   Checks if variable for given var_idx is static
 * PARAMATERS       :   [IN] Int var_idx
 * RETURN VALUE     :   SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 varIsStatic(Int var_idx)
{
    assert((var_idx>=0)&&(var_idx<=SIM_VARS_NUM));
    assert(SIM_FALSE==varIdxIsEmpty(var_idx));

    return vars_db[var_idx].is_static;
}

/************************************************************************************
 * FUNCTION NAME    :   varGetSize
 * DESCRIPTION      :   returns var size (in bytes)
 * PARAMATERS       :   [IN] Int var_idx
 * RETURN VALUE     :   var size / 0 if var not exists
 ************************************************************************************/
UInt16 varGetSize(Int var_idx)
{
    if( (var_idx>=0) && (var_idx<=SIM_VARS_NUM) && (SIM_FALSE==varIdxIsEmpty(var_idx)) )
	{
    	return vars_db[var_idx].size;
	}
    else
    {
    	return 0;
    }
}

/************************************************************************************
 * FUNCTION NAME    :   varClear
 * DESCRIPTION      :   Clears variable with given 'var_idx'
 * PARAMATERS       :   [IN] Int var_idx
 * RETURN VALUE     :
 ************************************************************************************/
static void varClear(Int var_idx)
{
    assert((var_idx>=0)&&(var_idx<SIM_VARS_NUM));

    if(PNULL!=vars_db[var_idx].val)
    {
        free(vars_db[var_idx].val);
    }
    memset(&vars_db[var_idx],0,sizeof(vars_db[var_idx]));
}

/************************************************************************************
 * FUNCTION NAME    :   varExpandVars
 * DESCRIPTION      :   expands variable values from src_p to dest_p
 * PARAMATERS       :   [IN] const UChar8 *src_p
 *                      [OUT] UChar8* dest_p
 * RETURN VALUE     :   SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 varExpandVars(UChar8* dest_p,const UChar8 *src_p)
{
    Int i=0;
    Int di=0;
    Int vi=0;
    Int var_idx=0;
    UInt16 n=0;
    UChar8 var_name[SIM_VAR_NAME_LEN+1];
    while(0!=src_p[i])
    {
        if('}'==src_p[i])
        {
            SIM_LOG(SIM_LOG_WARNING, "'{' out of var_block in '%s'",src_p);
            return SIM_FALSE;
        }
        if('{'==src_p[i])
        {
            vi=0;
            i++;
            while('}'!=src_p[i])
            {
                if(0==src_p[i])
                {
                    SIM_LOG(SIM_LOG_WARNING, "EOL in var_block in '%s'",src_p);
                    return SIM_FALSE;
                }
                var_name[vi++]=src_p[i++];
                if(vi>SIM_VAR_NAME_LEN)
                {
                    var_name[vi]=0;
                    SIM_LOG(SIM_LOG_WARNING, "Var name %s too long in '%s'",var_name,src_p);
                    return SIM_FALSE;
                }
            }
            i++;
            var_name[vi]=0;
            var_idx=varFindIdx(var_name);
            if(INVALID_VAR_IDX!=var_idx)
            {
                for(n=0;n<vars_db[var_idx].size;n++)
                {
                    sprintf(dest_p+di,"%02X ",vars_db[var_idx].val[n]);
                    di+=3;
                }
            }
            else
            {
                SIM_LOG(SIM_LOG_WARNING, "UNKNOWN Var name %s in '%s'",var_name,src_p);
                return SIM_FALSE;
            }
            continue;
        }
        dest_p[di++]=src_p[i++];
    }
    dest_p[di]=0;
    return SIM_TRUE;
}
