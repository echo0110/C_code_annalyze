
/************************************************************************************
*
*   FILE NAME  : call_func.c
*
*   DESCRIPTION: External scheduler command line interface implementation.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc.
*
*************************************************************************************/

/************************************************************************************
 * Includes
 ************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "lteSimPdcpEsLteFramework.h"

#include "lteSimPdcpCallFunc.h"
#include "lteSimPdcpSimuTableFunc.h"
#include "lteSimPdcpUserUtils.h"
#include <ltePdcpLteDefs.h>

/************************************************************************************
 * FUNCTION NAME    :   userCmdlineParse
 * DESCRIPTION      :   Getting parameters from command line and calling callbacks
 *                      from table. Parameters get line by line in forever loop
 * PARAMATERS       :   None
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userCmdlineParse()
{
    printf("LTE-OAM:#");
    fflush(stdout);
    UChar8  input_string    [MAX_CALLBACK_NAME_SIZE+MAX_PARAMS_SIZE+2];
    fgets(input_string, sizeof(input_string), stdin);
    return userCallCallback(input_string);
}

/************************************************************************************
 * FUNCTION NAME    :   userCallCallback
 * DESCRIPTION      :   Scans table and call selected callback
 * PARAMATERS       :   [IN] UChar8* input_string
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userCallCallback(UChar8* input_string)
{    
    UChar8      callback_name[MAX_CALLBACK_NAME_SIZE]={0};
    UChar8*     callback_params = PNULL; 
    UChar8      fmt_buf[MAX_FMT_BUF_SIZE];
    UInt32      argc = 0;
    UChar8**    argv = PNULL;
    UInt32      result = 0, i =0;
    UInt8       freeMem = LTE_FALSE;
    if(PNULL==input_string)
    {
        PDCP_LOG(LOG_WARNING, "%s:PNULL==input_string", __FUNCTION__);
        return LTE_FALSE;
    }
    /*replace '\' symbol to '/' */
    userStrReplaceChar(input_string,'\\','/');
    while( (0!=*input_string) && (input_string[strlen(input_string)-1]<=' ') )
        input_string[strlen(input_string)-1] = 0;
    /* extract command name */
    sprintf(fmt_buf, "%s%ds", "%", MAX_CALLBACK_NAME_SIZE);
    sscanf(input_string, fmt_buf, callback_name );
    if ( 0 == callback_name[0] )
        return result;

    /* extract command parameters */
    if ( strlen(input_string) > (strlen(callback_name) + 1) )
        callback_params = input_string+strlen(callback_name)+1;

    if ( (PNULL != callback_params) && (0 != callback_params[0]) )
    {
        /* prepare set of command parameters as array of strings */
        if ( '"' == callback_params[0] )
        {
            if( '"' == callback_params[strlen(callback_params) - 1] )
            {
                argc = 1;
                callback_params++;
                callback_params[strlen(callback_params) - 1] = 0;
                argv = &callback_params;
            }
            else
            {
                PDCP_LOG(LOG_WARNING, "%s:\" not found in the end of command line. ",__FUNCTION__);
            }
        }
        else
        {
            argv = userGetListStrings(callback_params, ",", 0);
            argc = userGetListStringsCount((const UChar8**)argv);
            freeMem = LTE_TRUE;
        }
    }
       
    /* find and run command handler */
    for(i = 0; table_func[i].str_id; i++)
    {
        userTolower(callback_name);
        if ( 0 == strcmp(callback_name, table_func[i].str_id) )
        {
            result = (*table_func[i].pfunc)(argc, (const UChar8**)argv);
            break;
        }
    }
    
    if ( (LTE_TRUE == freeMem) && (PNULL != argv) )
        userFreeListStrings(argv);
    return result;
}
