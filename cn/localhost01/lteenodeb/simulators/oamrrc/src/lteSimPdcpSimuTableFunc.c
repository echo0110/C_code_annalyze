
/************************************************************************************
*
*   FILE NAME  : table_func.c
*
*   DESCRIPTION: Table with callbacks implementation.
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
#include "lteSimPdcpSimuTableFunc.h"
#include "lteSimPdcpCallbacks.h"

#include "lteSimPdcpCallBacksSockets.h"
#include "lteSimPdcpTestStatistic.h"

/************************************************************************************
 * ARRAY NAME   :   table_func
 * DESCRIPTION  :   Array of structures, callback functions called from here 
 *                  by pointer
 * FIELDS       :   FuncId - structure wich consists of identifier of callback
 *                  function and pointer to callback function
 ************************************************************************************/
#if 0
FuncId table_func[] = 
{
    {"runscript",       userRunScriptCallback			},
    {"sleep",           userSleepCallback				},
    {"exit",            userExitCallback				},
    {"echo",            userEchoCallback				},
    {"redirectlog",     userRedirectLogCallback			},
    {"setloglevel",         userSetLogLevel     			},
    {"printmempoolstate",   userPrintMemPoolState           },
    {"loadtosock",      userToSockCallback				},
    {"sendandcheck",	userSendAndCheckFileCallback	},
    {"sendcmd",         userSendCommand                 },
    {"statisticcmd",    userStatisticCommandCallback    },
    {"displaystatsperiodic", userDisplayStatsPeriodicCallback },
    {"stopdisplaystats", userStopDisplayStats },
    {PNULL, PNULL},
};
#endif
