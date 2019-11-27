/************************************************************************************
*
*   FILE NAME  : protocolSimulatorMain.c
*
*   DESCRIPTION: Application main file.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

/************************************************************************************
 * Includes
 ************************************************************************************/
#include <pthread.h>
#include "simLogger.h"

#include "call_func.h"
#include "callbacks.h"
#include "callbacksVars.h"
#include "sock_tools.h"
#include "simuScheduler.h"
#include "testStatistic.h"

/************************************************************************************
 * Local definitions
 ************************************************************************************/
#define SIM_SOCKET_CONFIG_FILE "../cfg/mme/sockets_simu.ini"


/************************************************************************************
 * FUNCTION NAME    : main
 * DESCRIPTION      : Application main function
 * PARAMATERS       :
 * RETURN VALUE     : status (SUCCESS(0)/FAIL(1))
 ************************************************************************************/
int main(int argc, char** argv)
{
    isTerminated = SIM_FALSE;

#ifdef WIN32
    pthread_win32_process_attach_np();
#endif

    /*Init log*/
    loggerInit(PNULL);
    loggerSetLogLevel(SIM_LOG_DUMP);

    SIM_LOG( SIM_LOG_INFO, "SIM\t: started");

    if( SOCKET_ERROR == InitSockets(SIM_SOCKET_CONFIG_FILE) )
    {
        SIM_LOG(SIM_LOG_DETAIL,"InitSockets returned FAIL");
        return 1;
    }

    InitVars();
    statisticInit();

    esStart();

    /* on behalf of Alex */
    if(1 < argc)
    {
        userRunScriptCallback(1, (const UChar8**)&argv[1]);
    }

    while ( userCmdlineParse() == 0 );

    CloseSockets();

    SIM_LOG( SIM_LOG_INFO, "SIM\t: Stopped");
    return 0;
}
