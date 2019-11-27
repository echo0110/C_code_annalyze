/************************************************************************************
*
*   FILE NAME  : table_func.c
*
*   DESCRIPTION: Table with callbacks implementation.
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
#include "simu_table_func.h"
#include "callbacks.h"
#include "operations.h"
#include "callbacksVars.h"
#include "callbacksSockets.h"
#include "testStatistic.h"

/************************************************************************************
 * ARRAY NAME   :   table_callbacks_func
 * DESCRIPTION  :   Array of structures, callback functions called from here
 *                  by pointer
 * FIELDS       :   CallbackFuncId - structure wich consists of identifier of callback
 *                  function and pointer to callback function
 ************************************************************************************/
CallbackFuncId table_callbacks_func[] =
{
    {"runscript",        userRunScriptCallback           	},
    {"sleep",            userSleepCallback               	},
    {"exit",             userExitCallback                	},
    {"echo",             userEchoCallback                	},
    {"redirectlog",      userRedirectLogCallback         	},
    {"setloglevel",      userSetLogLevel                 	},
    {"loadtosock",       userToSockCallback              	},
    {"loadtosock_sctp",  userToSockSctpCallback         	},
    {"sendandcheck",     userSendAndCheckFileCallback    	},
    {"sendandcheck_sctp",userSendAndCheckFileSctpCallback  	},
    {"listensock",       userListenSockCallback          	},
    {"statisticcmd",     userStatisticCommandCallback    	},
    {"setvar",           userSetVarCallback              	},
    {"setvarstatic",     userSetVarStaticCallback          	},
    {"printvar",         userPrintVarCallback            	},
    {"clearvar",         userClearVarCallback            	},
    {"clearvarsstatic",  userClearVarsStaticCallback       	},
    {"listenandaccept",  userListenAndAccept             	},
    {"waitreset_sctp",   userWaitResetSctp             		},
    {"test",             userTestCallback    				},
    {"emptysocks",       userEmptySocksCallback             },
    {PNULL, PNULL},
};

/************************************************************************************
 * ARRAY NAME   :   table_callbacks_func
 * DESCRIPTION  :   Array of structures, callback functions called from here
 *                  by pointer
 * FIELDS       :   FlowCallbackFuncId - structure wich consists of identifier of
 *                  flow callback function and pointer to flow callback function
 ************************************************************************************/
FlowCallbackFuncId table_flow_callbacks_func[] =
{
    {"if",           userIfFlowCallback               },
    {"goto",         userGotoFlowCallback             },
    {PNULL, PNULL},
};


/************************************************************************************
 * ARRAY NAME   :   table_operation_func
 * DESCRIPTION  :   Array of structures, operation functions called from here
 *                  by pointer
 * FIELDS       :   OperationFuncId - structure wich consists of identifier of operation
 *                  function and pointer to operation function
 ************************************************************************************/
OperationFuncId table_operation_func[] =
{
    {"inttohex",   userRunInttohexOperation       },
    {"varsize",    userRunVarSizeOperation        },
    {"bits",       userRunBitsOperation           },
    {"dig",        userRunDigOperation            },
    {"skip",       userRunSkipOperation           },
    {"skip",       userRunSkipOperation           },
    {"ntoh",       userRunNtohOperation           },
    {"hton",       userRunNtohOperation           },
    {"cmp",        userRunCmpOperation            },
    {"inc",        userRunIncOperation            },
    {"readfile",   userRunReadFileOperation       },
    {"readsock",   userRunReadSockOperation       },
    {"and",        userRunAndOperation            },
    {"or",         userRunOrOperation             },
    {PNULL, PNULL},
};
