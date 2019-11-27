/************************************************************************************
*
*   FILE NAME  : callbacks.c
*
*   DESCRIPTION: External scheduler callbacks implementation.
                 "exit", "sleep", "runscript" command callbacks definitions.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc.
*
*************************************************************************************/

/************************************************************************************
 * Includes
 ************************************************************************************/

#include <assert.h>

#include "lteSimPdcpEsLteFramework.h"
#include "lteLog.h"
#include <ltePdcpLteDefs.h>
#include "lteSimPdcpCallbacks.h"
#include "lteSimPdcpUserUtils.h"
#include "lteSimPdcpCallFunc.h"
#include "lteSimPdcpTestStatistic.h"
 #include "lteSimPdcpSockTools.h"

/************************************************************************************
 * Definitions
 ************************************************************************************/

/************************************************************************************
 * Global variable
 ************************************************************************************/

Int isTerminated = 0;
FILE *throughputFD_g = NULL;
UChar8 throughputFile_g[256]={0};
UInt16 interval_g = 10;
UInt8 startCaptureThroughput_g = 0;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
extern void loadToSockCmd(const UChar8* cfg_fname);
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
    UInt32 i = 0;
    UChar8** strings = PNULL;
    if ( 1 != argc && PNULL == argv)
    {
       PDCP_LOG( LOG_WARNING, "Wrong Format :: Format Should be 'runscript <filename.scr>'");
       return LTE_FALSE;
    }
    PDCP_LOG( LOG_INFO, "COMMAND[ runscript ] %s", argv[0] ); 
    strings = userGetListStringsFromCfgFile(argv[0]);
    if(PNULL == strings)
    {
        PDCP_LOG( LOG_WARNING, "Wrong Script File.");
        return LTE_FALSE;
    }
    for(i=0; strings[i]; i++)
    {
        userCallCallback(strings[i]);
    }
    userFreeListStrings(strings);
    statisticUpdate( SIM_STAT_CMD_RUNSCRIPT_AMOUNT, 1 );
    return LTE_FALSE;
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
    if ( 1 != argc && PNULL == argv)
    {
        PDCP_LOG( LOG_WARNING, "Wrong Format :: Format Should be 'sleep <ms>'");
        return LTE_FALSE;
    }
    PDCP_LOG( LOG_INFO, "COMMAND[ sleep ] %s", argv[0] ); 
    userSleep(atoi(argv[0]));
    statisticUpdate( SIM_STAT_CMD_SLEEP_AMOUNT, 1 );
    statisticUpdate( SIM_STAT_SLEEP_TIME, atoi(argv[0]) );
    return LTE_FALSE;
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
    PDCP_LOG( LOG_INFO, "COMMAND[ exit ]" ); 
    isTerminated = LTE_TRUE;
    if(strlen(throughputFile_g)) {	
        unlink(throughputFile_g);	
    }	
    exit(0);	
    return LTE_TRUE; /*means stop parser*/
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
    if ( 1 != argc && PNULL == argv)
    {
        PDCP_LOG( LOG_WARNING, "Wrong Format :: Format Should be 'echo <\"text\">'");
        return LTE_FALSE;
    }
    PDCP_LOG(LOG_INFO, "%s", argv[0]);
    statisticUpdate( SIM_STAT_CMD_ECHO_AMOUNT, 1 );
    return LTE_FALSE;
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
    if ( 1 != argc && PNULL == argv)
    {
        PDCP_LOG( LOG_WARNING, "Wrong Format :: Format should be 'redirectlog < file name>'");
        return LTE_FALSE;
    }
    PDCP_LOG( LOG_INFO, "COMMAND[ redirectlog ] %s", argv[0] );
    statisticUpdate( SIM_STAT_CMD_REDIRECTLOG_AMOUNT, 1 );
    if(0!=strcmp(argv[0],"off"))
        redirectPdcpLog((void*)argv[0]);
    else
        redirectPdcpLog(PNULL);
    return LTE_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   userDisplayStatsPeriodicCallback
 * DESCRIPTION      :   start capturing stats
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/

UInt32 userDisplayStatsPeriodicCallback(UInt32 argc,const UChar8** argv)
{
    if(2!= argc)
    {
        PDCP_LOG( LOG_WARNING, "Wrong parameters. Should be 'displaystatsperiodic <timeInterval> <scriptFileName>'");
        return LTE_FALSE;
    }
    assert(argv);

    PDCP_LOG( LOG_INFO, "COMMAND[ displaystatsperiodic ] %s %s", argv[0],argv[1]);
    startCaptureThroughput_g = 1;

    /*  save this as global time interval)*/
    interval_g  = (UInt16)atoi(argv[0]);

    /* open file */
    if(throughputFD_g != NULL)
    {
        fclose(throughputFD_g);
        throughputFD_g = NULL;  
        throughputFile_g[0]='\0';
    }
    strcpy(throughputFile_g,argv[1]);

    throughputFD_g = fopen(throughputFile_g,"w");

    if(PNULL == throughputFD_g)
    {
        PDCP_LOG( LOG_WARNING, "Unable to open %s", throughputFile_g);
    }
    fprintf(throughputFD_g,"\t\tSNo\tDownlink Throughput\tUplink Throughput\n");
    fflush(throughputFD_g);
    return LTE_FALSE;
}
/************************************************************************************
 * FUNCTION NAME    :   userStopDisplayStats
 * DESCRIPTION      :   stop capturing stats
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userStopDisplayStats(UInt32 argc,const UChar8** argv)
{
    if ( 0 != argc )
    {
        PDCP_LOG( LOG_WARNING, "Wrong parameters. Should be 'stopdisplaystats'");
        return LTE_FALSE;
    }
    startCaptureThroughput_g = 0;
    PDCP_LOG( LOG_INFO, "COMMAND[ stopdisplaystats ] ");
    return LTE_FALSE;
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
    if ( 1 != argc && PNULL == argv)
    {
        PDCP_LOG( LOG_WARNING, "Wrong Format :: Format should be 'setloglevel < log level >'");
        return LTE_FALSE;
    }
    PDCP_LOG( LOG_INFO, "COMMAND[ setloglevel ] %s", argv[0] );
    lteLogLevel(atoi(argv[0]));
    pdcpLogLevel(atoi(argv[0]));
    return LTE_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   userPrintMemPoolState
 * DESCRIPTION      :   Prints out Memory pool state 
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userPrintMemPoolState(UInt32 argc, const UChar8** argv)
{
    if ( 0 != argc )
    {
        PDCP_LOG( LOG_WARNING, "Wrong Format :: Format should be 'printmempoolstate'");
        return LTE_FALSE;
    }
    PDCP_LOG( LOG_INFO, "COMMAND[ printmempoolstate ]" );
    getMemPoolStats();
    return LTE_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   userGetThroughput
 * DESCRIPTION      :   This function is used to get the throughput 
 * PARAMATERS       :   None
 * RETURN VALUE     :   None
 ************************************************************************************/
void userGetThroughput(UInt32 argc, const UChar8** argv)
{
    UChar8 *cfg_fname = "scripts/oam/mac/getThroughput.cfg";
    loadToSockCmd(cfg_fname); 
}
/************************************************************************************
 * FUNCTION NAME    :   userGetCPU
 * DESCRIPTION      :   This function is used to get the CPU Utilization 
 * PARAMATERS       :   None
 * RETURN VALUE     :   None
 ************************************************************************************/
void userGetCPU(UInt32 argc, const UChar8** argv)
{
   system("mpstat -P ALL"); 
}
