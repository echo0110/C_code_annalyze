/************************************************************************************
*
*   FILE NAME  : call_func.c
*
*   DESCRIPTION: External scheduler command line interface implementation.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

/************************************************************************************
 * Includes
 ************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "simLogger.h"

#include "call_func.h"
#include "simu_table_func.h"
#include "userUtils.h"

/************************************************************************************
 * FUNCTION NAME    :   userCmdlineParse
 * DESCRIPTION      :   Getting parameters from command line and calling callbacks
 *                      from table. Parameters get line by line in forever loop
 * PARAMATERS       :   None
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userCmdlineParse()
{
    UChar8  input_string    [MAX_CALLBACK_NAME_SIZE+MAX_PARAMS_SIZE+2];
    UChar8 *string_list[2]={input_string,PNULL};
    UInt32 line_no=0;

    fgets(input_string, sizeof(input_string), stdin);

    return userCallCallback(string_list,&line_no);
}

/************************************************************************************
 * FUNCTION NAME    :   userCallCallback
 * DESCRIPTION      :   Scans table and call selected callback
 * PARAMATERS       :   [IN] UChar8** string_list
 *                      [IN/OUT]UInt32 *p_line_no
 * RETURN VALUE     :   [OUT] SIM_FALSE=no exit,SIM_TRUE=exit
 ************************************************************************************/
UInt32 userCallCallback(UChar8** string_list,UInt32 *p_line_no)
{
    UChar8      callback_name[MAX_CALLBACK_NAME_SIZE+1]={0};
    UChar8*     callback_params = PNULL;
    UChar8      tmp_param_str[0xFFF];
    UChar8      *tmp_args_arr[2];
    UChar8      fmt_buf[MAX_FMT_BUF_SIZE+1];
    UInt32      argc = 0;
    UChar8**    argv = PNULL;
    UInt32      result = SIM_FALSE;
    UInt32      i;
    UInt8       freeMem = SIM_FALSE;
    UChar8      *input_string=PNULL;
    UInt8       isCallbackFound=SIM_FALSE;
    UInt8       isFlowCallbackFound=SIM_FALSE;

    assert(PNULL!=string_list);
    assert(PNULL!=p_line_no);

    input_string=string_list[*p_line_no];

    if(PNULL==input_string)
    {
        SIM_LOG(SIM_LOG_WARNING, "%s:PNULL==input_string", __FUNCTION__);
        return SIM_TRUE;
    }

    if(':'==input_string[0])
    {
        /* label */
        (*p_line_no)++;
        return SIM_FALSE;
    }

    /*replace '\' symbol to '/' */
    userStrReplaceChar(input_string,'\\','/');

    while( (0!=*input_string) && (input_string[strlen(input_string)-1]<=' ') )
    {
        input_string[strlen(input_string)-1]=0;
    }

    /* extract command name */
    sprintf(fmt_buf, "%s%ds", "%", MAX_CALLBACK_NAME_SIZE);
    sscanf(input_string, fmt_buf, callback_name );

    if ( 0 == callback_name[0] )
    {
/*      SIM_LOG(SIM_LOG_WARNING, "%s:invalid command. cmdLine[%s]", __FUNCTION__, input_string );*/
        (*p_line_no)++;
        return SIM_FALSE;
    }

    /* extract command parameters */
    if ( strlen(input_string) > (strlen(callback_name) + 1) )
    {
        callback_params = input_string+strlen(callback_name)+1;
    }

    if ( (PNULL != callback_params) && (0 != callback_params[0]) )
    {
        /* prepare set of command parameters as array of strings */
        if ( '"' == callback_params[0] )
        {
            if( '"' == callback_params[strlen(callback_params) - 1] )
            {
                argc = 1;
                callback_params++;
                strcpy(tmp_param_str,callback_params);
                tmp_param_str[strlen(tmp_param_str) - 1] = 0;
                tmp_args_arr[0]=tmp_param_str;
                tmp_args_arr[1]=PNULL;
                argv = tmp_args_arr;
            }
            else
            {
                SIM_LOG(SIM_LOG_WARNING, "SIM\t:%s:\" not found in the end of command line. ",__FUNCTION__);
            }
        }
        else
        {
            argv = userGetListStrings(callback_params, ",", 0);
            argc = userGetListStringsCount((const UChar8**)argv);
            freeMem = SIM_TRUE;
        }
    }

    /* find and run command handler */
    for(i = 0; table_callbacks_func[i].str_id; i++)
    {
        userTolower(callback_name);
        if ( 0 == strcmp(callback_name, table_callbacks_func[i].str_id) )
        {
            result = (*table_callbacks_func[i].pfunc)(argc, (const UChar8**)argv);
            isCallbackFound=SIM_TRUE;
            break;
        }
    }

    /* find and run command handler */
    if(SIM_FALSE==isCallbackFound)
    {
        for(i = 0; table_flow_callbacks_func[i].str_id; i++)
        {
            userTolower(callback_name);
            if ( 0 == strcmp(callback_name, table_flow_callbacks_func[i].str_id) )
            {
                result = (*table_flow_callbacks_func[i].pfunc)(argc, (const UChar8**)argv,string_list,p_line_no);
                isFlowCallbackFound=SIM_TRUE;
                break;
            }
        }
    }

    if ( (SIM_TRUE == freeMem) && (PNULL != argv) )
    {
        userFreeListStrings(argv);
    }

    if(SIM_FALSE==isFlowCallbackFound)
    {
        (*p_line_no)++;
    }

    return result;
}
