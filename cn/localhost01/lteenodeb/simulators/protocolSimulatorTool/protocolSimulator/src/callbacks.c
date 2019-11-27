/************************************************************************************
*
*   FILE NAME  : callbacks.c
*
*   DESCRIPTION: External scheduler callbacks implementation.
                 "exit", "sleep", "runscript" command callbacks definitions.
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

#include "callbacks.h"
#include "userUtils.h"
#include "call_func.h"
#include "testStatistic.h"

/************************************************************************************
 * Definitions
 ************************************************************************************/

/************************************************************************************
 * Global variable
 ************************************************************************************/

Int isTerminated = 0;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static UInt32 userGotoCmd(
    const UChar8* label,
    UChar8** string_list,
    UInt32 *p_line_no
);

static void userTestCmd(const UChar8 *expr,UInt8 expected_res);

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/****************************************************************************
 * Functions implementation
 ****************************************************************************/

/************************************************************************************
 * FUNCTION NAME    :   userRunScriptCallback
 * DESCRIPTION      :   Run script callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userRunScriptCallback(UInt32 argc, const UChar8** argv)
{
    UInt32 line_no=0;
    UChar8** strings = PNULL;
    UInt32 result=SIM_FALSE;

    if ( 1 != argc )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be 'runscript <filename.scr>'");
        return SIM_FALSE;
    }
    assert(argv);

    SIM_LOG( SIM_LOG_INFO, "COMMAND[ runscript ] %s", argv[0] );

    strings = userGetListStringsFromCfgFile(argv[0]);
    if(PNULL == strings)
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong script file.");
        return SIM_FALSE;
    }
    while(PNULL!=strings[line_no])
    {
        result=userCallCallback(strings,&line_no);
        if(SIM_TRUE==result)
        {
        	break;
        }
    }
    userFreeListStrings(strings);

    statisticUpdate( SIM_STAT_CMD_RUNSCRIPT_AMOUNT, 1 );
    return result;
}

/************************************************************************************
 * FUNCTION NAME    :   userSleepCallback
 * DESCRIPTION      :   "sleep" callback
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userSleepCallback(UInt32 argc, const UChar8** argv)
{
    if ( 1 != argc )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be 'sleep <mseconds>'");
        return SIM_FALSE;
    }

    assert(argv);

    SIM_LOG( SIM_LOG_INFO, "COMMAND[ sleep ] %s", argv[0] );
    userSleep(atoi(argv[0]));

    statisticUpdate( SIM_STAT_CMD_SLEEP_AMOUNT, 1 );
    statisticUpdate( SIM_STAT_SLEEP_TIME, atoi(argv[0]) );
    return SIM_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   userExitCallback
 * DESCRIPTION      :   Exit ext scheduler
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userExitCallback(UInt32 argc, const UChar8** argv)
{
    SIM_LOG( SIM_LOG_INFO, "COMMAND[ exit ]" );

    isTerminated = SIM_TRUE;
    return SIM_TRUE; /*means stop parser*/
}

/************************************************************************************
 * FUNCTION NAME    :   userExitCallback
 * DESCRIPTION      :   Echo user message
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userEchoCallback(UInt32 argc, const UChar8** argv)
{
    if ( 1 != argc )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be 'echo <\"text\">'");
        return SIM_FALSE;
    }

    assert(argv);

    SIM_LOG(SIM_LOG_INFO, "%s", argv[0]);

    statisticUpdate( SIM_STAT_CMD_ECHO_AMOUNT, 1 );
    return SIM_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   userRedirectLogCallback
 * DESCRIPTION      :   Redirects log output from stderr to file
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userRedirectLogCallback(UInt32 argc, const UChar8** argv)
{
    if ( 1 != argc )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be 'redirectlog <logFileName>'");
        return SIM_FALSE;
    }

    assert(argv);

    SIM_LOG( SIM_LOG_INFO, "COMMAND[ redirectlog ] %s", argv[0] );

    statisticUpdate( SIM_STAT_CMD_REDIRECTLOG_AMOUNT, 1 );

    if(0!=strcmp(argv[0],"off"))
    {
        loggerRedirectLog((void*)argv[0]);
    }
    else
    {
        loggerRedirectLog(PNULL);
    }

    return SIM_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   userSetLogLevel
 * DESCRIPTION      :   Sets log level
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userSetLogLevel(UInt32 argc, const UChar8** argv)
{
    if ( 1 != argc )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be 'setloglevel <logLevel>'");
        return SIM_FALSE;
    }

    assert(argv);

    SIM_LOG( SIM_LOG_INFO, "COMMAND[ setloglevel ] %s", argv[0] );

    loggerSetLogLevel(atoi(argv[0]));

    return SIM_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   userTestCallback
 * DESCRIPTION      :   "test" callback
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userTestCallback(UInt32 argc, const UChar8** argv)
{
    if ( (1 != argc) && (2 != argc) )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be 'test <expression>[,false]'");
        return SIM_FALSE;
    }

    assert(argv);

    if (1==argc)
    {
    	SIM_LOG( SIM_LOG_INFO, "COMMAND[ test ] %s", argv[0] );
    	userTestCmd(argv[0],SIM_TRUE);
    }

	if (2==argc)
	{
		userTolower((UChar8*)argv[1]);
		if (0==strcmp("false",argv[1]))
		{
	    	SIM_LOG( SIM_LOG_INFO, "COMMAND[ test ] %s,false", argv[0] );
	    	userTestCmd(argv[0],SIM_FALSE);
		}
		else
		{
	        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be 'test <expression>[,false]'");
	        return SIM_FALSE;
		}
	}

    return SIM_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   userTestCmd
 * DESCRIPTION      :   action to set variable value
 * PARAMATERS       :   [IN] UChar8 *expr - string with expression
 * 						[IN] UInt8 expected_res -
 * 							 SIM_TRUE (expression should be != 0) or
 * 							 SIM_FALSE (expression should be == 0)
 * RETURN VALUE     :
 ************************************************************************************/
static void userTestCmd(const UChar8 *expr,UInt8 expected_res)
{
    if ((SIM_FALSE!=exprIsTrue(expr))==(SIM_FALSE!=expected_res))
    {
		SIM_LOG(SIM_LOG_INFO, "*********************** TEST \t\t[ PASSED ] ******");
		statisticUpdate( SIM_STAT_TESTS_SUCCEEDED, 1 );
    }
    else
    {
		SIM_LOG(SIM_LOG_INFO, "*********************** TEST \t\t[ FAILED ] ******");
		statisticUpdate( SIM_STAT_TESTS_FAILED, 1 );
    }
}

/************************************************************************************
 * FUNCTION NAME    :   userIfFlowCallback
 * DESCRIPTION      :   "goto" processor
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 *                      [IN] UChar8** string_list
 *                      [IN/OUT]UInt32 *p_line_n
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userIfFlowCallback(UInt32 argc, const UChar8** argv,
    UChar8** string_list,
    UInt32 *p_line_no
)
{
	UInt8 exp1_buf[MAX_EXPRESSION_BUF_SIZE];
	UInt16 exp1_buf_size=0;
    UInt8 is_true=SIM_FALSE;
	UInt16 k=0;

    if( (2 != argc) && (3 != argc) )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be 'if <expression>,<true_label>[,<false_label>]'");
        return SIM_FALSE;
    }

    assert(argv);

    if(argc<3)
    {
        SIM_LOG( SIM_LOG_INFO, "COMMAND[ if ] %s,%s", argv[0], argv[1] );
    }
    else
    {
        SIM_LOG( SIM_LOG_INFO, "COMMAND[ if ] %s,%s,%s", argv[0], argv[1], argv[2] );
    }

    if(SIM_TRUE!=userParseMsgStr(argv[0],MAX_EXPRESSION_BUF_SIZE,exp1_buf,&exp1_buf_size,PNULL))
    {
        SIM_LOG(SIM_LOG_WARNING, "%s:Error parsing expression '%s'",__FUNCTION__,argv[0]);
        return SIM_FALSE;
    }

    if( exp1_buf_size<1 )
    {
        SIM_LOG(SIM_LOG_WARNING, "%s:invalid exp1_buf_size (%i)",__FUNCTION__,exp1_buf_size);
        return SIM_FALSE;
    }

    for(k=0;k<exp1_buf_size;k++)
    {
        if( 0!=exp1_buf[k] )
        {
            is_true=SIM_TRUE;
            break;
        }
    }

    SIM_LOG( SIM_LOG_INFO, "Condition is %s",(SIM_TRUE==is_true)?("TRUE"):("FALSE") );

    if(SIM_TRUE==is_true)
    {
        return userGotoCmd(argv[1],string_list,p_line_no);
    }
    else
    {
        if(3 == argc)
        {
            return userGotoCmd(argv[2],string_list,p_line_no);
        }
    }

    (*p_line_no)++;

    return SIM_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   userGotoFlowCallback
 * DESCRIPTION      :   "if" processor
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 *                      [IN] UChar8** string_list
 *                      [IN/OUT]UInt32 *p_line_n
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userGotoFlowCallback(UInt32 argc, const UChar8** argv,
    UChar8** string_list,
    UInt32 *p_line_no
)
{
    if(1 != argc)
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be 'goto <label>'");
        return SIM_FALSE;
    }

    assert(argv);

    SIM_LOG( SIM_LOG_INFO, "COMMAND[ goto ] %s", argv[0] );

    return userGotoCmd(argv[0],string_list,p_line_no);
}


/************************************************************************************
 * FUNCTION NAME    :   userGotoCmd
 * DESCRIPTION      :   "goto" implementation
 * PARAMATERS       :   [IN] UChar8* label
 *                      [IN] UChar8** string_list
 *                      [IN/OUT]UInt32 *p_line_n
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
static UInt32 userGotoCmd(
    const UChar8* label,
    UChar8** string_list,
    UInt32 *p_line_no
)
{
    UInt32 i=0;
    UChar8 lbl_buf[MAX_LABEL_NAME_SIZE+2];

    if(strlen(label)>MAX_LABEL_NAME_SIZE)
    {
        SIM_LOG(SIM_LOG_CRITICAL, "%s:label too long '%s'",__FUNCTION__,label);
        return SIM_TRUE;
    }

    sprintf(lbl_buf,":%s",label);
    userTolower(lbl_buf);
    userRTrim(lbl_buf);

    for(i=0;string_list[i];i++)
    {
        if(':'==string_list[i][0])
        {
            userTolower(string_list[i]);
            userRTrim(string_list[i]);
            if(0==strcmp(string_list[i],lbl_buf))
            {
                *p_line_no = i+1;
                SIM_LOG( SIM_LOG_INFO, "Jumped to label %s",string_list[i]);
                return SIM_FALSE;
            }
        }
    }

    SIM_LOG(SIM_LOG_CRITICAL, "%s:label not found '%s'",__FUNCTION__,label);
    return SIM_TRUE;
}
