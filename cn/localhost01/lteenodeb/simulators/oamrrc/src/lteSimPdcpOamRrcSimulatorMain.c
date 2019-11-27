/************************************************************************************
*
*   FILE NAME  : oamRrcSimulatorMain.c
*
*   DESCRIPTION: Application main file.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc.
*
*************************************************************************************/

/************************************************************************************
 * Includes
 ************************************************************************************/
#include <sched.h>
#include <sys/types.h>

#include "lteSimPdcpEsLteFramework.h"
#include <ltePdcpLteDefs.h>
#include "lteSimPdcpCallFunc.h"
#include "lteSimPdcpCallbacks.h"
#include "lteSimPdcpSockTools.h"
#include "lteSimPdcpSimuScheduler.h"
#include "lteSimPdcpSimuTableFunc.h"
#include "lteSimPdcpCallBacksSockets.h"
#include "lteSimPdcpTestStatistic.h"

int rohc_profile = 0;
int rlc_mode_drbs = 0;
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

void displaycommandchoice();
/************************************************************************************
 * ARRAY NAME   :   table_func
 * DESCRIPTION  :   Array of structures, callback functions called from here
 *                  by pointer
 * FIELDS       :   FuncId - structure wich consists of identifier of callback
 *                  function and pointer to callback function
 ************************************************************************************/
FuncId table_func[] =
{
    {"help",            displaycommandchoice              },
    {"runscript",       userRunScriptCallback                   },
    {"sleep",           userSleepCallback                               },
    {"exit",            userExitCallback                                },
    {"echo",            userEchoCallback                                },
    {"redirectlog",     userRedirectLogCallback                 },
    {"setloglevel",         userSetLogLevel                             },
    {"printmempoolstate",   userPrintMemPoolState           },
    {"loadtosock",      userToSockCallback                              },
    {"sendandcheck",    userSendAndCheckFileCallback    },
    {"sendcmd",         userSendCommand                 },
    {"statisticcmd",    userStatisticCommandCallback    },
    {"loadsrbdata",     sendSrbCallback    },
    {"displaystatsperiodic", userDisplayStatsPeriodicCallback },
    {"stopdisplaystats", userStopDisplayStats },
    {"getthroughput", userGetThroughput},
    {"getcpu", userGetCPU},
    {PNULL, PNULL},
};


/************************************************************************************
* FUNCTION NAME    :   displaycommandchoice()
* DESCRIPTION      :   function to display command choice
* PARAMATERS       :   void
*
* RETURN VALUE     :   void
*************************************************************************************/
void displaycommandchoice()
{
    fprintf(stderr," OAM - RRC Commands Available:\n");
    fprintf(stderr," runscript - To run the script\n");
    fprintf(stderr," sleep - To sleep for a ms\n");
    fprintf(stderr," exit - For Exit\n");
    fprintf(stderr," echo - To Display\n");
    fprintf(stderr," redirectlog - To redirect the log\n");
    fprintf(stderr," setloglevel - Set log level\n");
    fprintf(stderr," printmempoolstate - To print Mem Pool Stats\n");
    fprintf(stderr," loaduserdata - Send Drb data to socket\n");
    fprintf(stderr," loadsrbdata - Send Srb data to socket\n");
    fprintf(stderr," deleteoutfile  - To delete out files\n");
    fprintf(stderr," loadtosock -  Send the packets to socket\n");
    fprintf(stderr," sendandcheck - Send the request and check the response\n");
    fprintf(stderr," sendcmd - Send the request to socket\n");
    fprintf(stderr," displaystatsperiodic - start capturing stats in file\n");
    fprintf(stderr," stopdisplaystats - stop capturing stats in file\n");
    fprintf(stderr," getthroughput - To display DL and UL Throughput\n");
    fprintf(stderr," getcpu - To display CPU Utilization\n");
    fprintf(stderr,"\n\n");
}
#ifdef ZPPP_INTEGRATION
/****************************************************************************
 * Function Name  : initUnixSocket
 * Inputs         : None
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function is the initialization function for opening 
 *                : Unix sockets for PPP Protocol.
 ****************************************************************************/
#define PREFIXLEN ((int)(&(((struct sockaddr_un *)0)->sun_path)))
UChar8 *OAMRRC_PPP_ADDR = "/zppp-control";
SInt32 sockRx_g;
struct sockaddr_un sockAddrTxRx_g;
SInt32 sockTxRx_g;

#ifdef TWO_UE_WORK
UChar8 *OAMRRC_PPP_ADDR_1 = "/zppp-control_1";
SInt32 sockRx_1_g;
struct sockaddr_un sockAddrTxRx_1_g;
SInt32 sockTxRx_1_g;

#endif

void initUnixSocket(void)
{
    socklen_t len = 0;
    unlink(OAMRRC_PPP_ADDR);
    sockRx_g = socket(PF_UNIX,SOCK_SEQPACKET, 0);
    if (sockRx_g < 0)
    {
        LTE_LOG(LOG_INFO,PNULL,"Not able to create unix sockets");
        return;
    }
    /* Pdcp Adapter will receive the messages on ADPT_ADDR */
    sockAddrTxRx_g.sun_family = PF_UNIX;
    sockAddrTxRx_g.sun_path[0]='\0';
    strcpy(&sockAddrTxRx_g.sun_path[1],OAMRRC_PPP_ADDR);
    fprintf(stderr,"Length of unix socket name is %d\n",(PREFIXLEN + 1 + strlen(OAMRRC_PPP_ADDR)));
    if (bind(sockRx_g, (struct sockaddr *)&sockAddrTxRx_g,(PREFIXLEN + 1 + strlen(OAMRRC_PPP_ADDR))) < 0)
    {
        LTE_LOG(LOG_INFO,PNULL,"Not able to bind unix sockets");
        return;
    }
    if (listen(sockRx_g,1) < 0)
    {
        LTE_LOG(LOG_INFO,PNULL,"Not able to bind unix sockets");
        return;
    }
    len = sizeof(sockAddrTxRx_g);
    if ((sockTxRx_g = accept(sockRx_g,PNULL,PNULL)) == -1)
    {
        LTE_LOG(LOG_INFO,PNULL,"Not able to accept connection");
        return;
    }

#ifdef TWO_UE_WORK
    unlink(OAMRRC_PPP_ADDR_1);
    sockRx_1_g = socket(PF_UNIX,SOCK_SEQPACKET, 0);
    if (sockRx_1_g < 0)
    {
        LTE_LOG(LOG_INFO,PNULL,"Not able to create unix sockets");
        return;
    }
    /* Pdcp Adapter will receive the messages on ADPT_ADDR */
    sockAddrTxRx_1_g.sun_family = PF_UNIX;
    sockAddrTxRx_1_g.sun_path[0]='\0';
    strcpy(&sockAddrTxRx_1_g.sun_path[1],OAMRRC_PPP_ADDR_1);
    fprintf(stderr,"Length of unix socket name is %d\n",(PREFIXLEN + 1 + strlen(OAMRRC_PPP_ADDR_1)));
    if (bind(sockRx_1_g, (struct sockaddr *)&sockAddrTxRx_1_g,(PREFIXLEN + 1 + strlen(OAMRRC_PPP_ADDR_1))) < 0)
    {
        LTE_LOG(LOG_INFO,PNULL,"Not able to bind unix sockets");
        return;
    }
    if (listen(sockRx_1_g,1) < 0)
    {
        LTE_LOG(LOG_INFO,PNULL,"Not able to bind unix sockets");
        return;
    }
    len = sizeof(sockAddrTxRx_1_g);
    if ((sockTxRx_1_g = accept(sockRx_1_g,PNULL,PNULL)) == -1)
    {
        LTE_LOG(LOG_INFO,PNULL,"Not able to accept connection");
        return;
    }
    fprintf(stderr,"at OAMRRC Stub Connection with 2nd zppp is accepted\n");
#endif
}
#endif

/************************************************************************************
* FUNCTION NAME    : InitSocket
* DESCRIPTION      : Performs initialization of the sockets engine. Should be called 
                  on program startup before using other socket functions
* PARAMATERS       :
* RETURN VALUE     : status (SUCCESS/FAIL)
 ************************************************************************************/
UInt8 InitSockets(void)
{
    /* To Read The Configuration file */
    UChar8 * filename_p = "../cfg/UESIM_Configuration.cfg";
    ReadSocketsConfig(filename_p);

    /*RRC Socket Creation*/ 
    if(SOCKET_ERROR==InitTxSocket(RRC_MODULE_ID))
       return LTE_FALSE;

    /* Init OAM Rx Sockets */ 
    if(SOCKET_ERROR==InitRxSocket(OAM_MODULE_ID,LTE_FALSE))
        return LTE_FALSE;

    /* Init TX sockets */
    if(SOCKET_ERROR==InitTxSocket(PDCP_MODULE_ID))
        return LTE_FALSE;

    /* Init pdcp Module Cmd Socket */
    if(SOCKET_ERROR==InitTxSocket(PDCP_MODULE_CMD_ID))
        return LTE_FALSE;

    /*RLC Socket Creation */ 
    if(SOCKET_ERROR==InitTxSocket(RLC_MODULE_ID))
        return LTE_FALSE;
  
   /*MAC Socket Creation*/ 
   if(SOCKET_ERROR==InitTxSocket(MAC_MODULE_ID))
       return LTE_FALSE;
  
   /*Packet Relay Socket Creation*/ 
   if(SOCKET_ERROR==InitTxSocket(PACKET_RELAY_MODULE_ID))
       return LTE_FALSE;
#if 0
/* Rohit: Removed because all messages will be sent to RRC for S1AP */
   /*S1AP Socket Creation */
   if(SOCKET_ERROR==InitTxSocket(S1AP_MODULE_ID))
       return LTE_FALSE;
#endif

   /*Traffic Gen Creation*/
   if(SOCKET_ERROR==InitTxSocket(TRAFFIC_GEN_MODULE_ID))
       return LTE_FALSE;

   /*Traffic Gen Creation*/
   if(SOCKET_ERROR==InitTxSocket(RRM_MODULE_ID))
       return LTE_FALSE;

   return LTE_TRUE;
}

/************************************************************************************
 * FUNCTION NAME    : main
 * DESCRIPTION      : Application main function
 * PARAMATERS       :
 * RETURN VALUE     : status (SUCCESS(0)/FAIL(1))
 ************************************************************************************/
#ifdef BUILD_EXE 
Int main(int argc, char** argv)
#else
Int initOamRrcSim(int argc , char** argv )
#endif
{
/*SPR 21653 changes start*/

if (argc < 4)
   {
      fprintf(stderr,"Usage is ./oamSim <core num to bind> <script file> <RLC_MODE> <Rohc Profile(Optional)>\n"); 
      return 1;
   }
   if (argc < 5)
   {
       rohc_profile = -1;
       fprintf(stderr,"No ROHC profile selected \n");
   }
   if (argc >= 4)
   {
       rlc_mode_drbs = atoi(argv[3]);
 
	   if (rlc_mode_drbs == 0)
       fprintf(stderr,"The RLC MODE selected is RLC_UM \n");

       else if (rlc_mode_drbs == 1)
       fprintf(stderr,"The RLC MODE selected is RLC_AM \n");

       else
	   {
           fprintf(stderr,"The RLC MODE selected should be either 0 (UM) or 1 (AM) \n");
	       return 1;
	   }

       if (argc == 5)
       {
	       rohc_profile = atoi(argv[4]);
           fprintf(stderr,"The rohc profile selected is = %d \n",rohc_profile);
       }
    }

/*SPR 21653 changes end*/
    isTerminated = LTE_FALSE;
   /*
    *  For Process Binding
    */
    cpu_set_t cpuSetSch;
    unsigned int coreNum = 1;
    coreNum = atoi(argv[1]);
    CPU_ZERO(&cpuSetSch);
    CPU_SET(coreNum ,&cpuSetSch);
    pid_t p = 0;
    if (0 != sched_setaffinity(p, sizeof(cpu_set_t), &cpuSetSch))
    {
        fprintf(stderr, " sched_setaffinity Returned Error in S1AP");
    }
    cleanupMemPool();       /* clean up Mem Pools */
    initMemPool();          /* Init Mem Pools */
    initMsgPool();          /* Init Msg Pools */
    initPdcpLog(PNULL);     /* Init Pdcp logs */
    pdcpLogLevel(LOG_DUMP); /* Set Pdcp log level */
    enableLog_g = LTE_TRUE;
    PDCP_LOG( LOG_INFO, "********* OAM RRC Simulator Started *********");

#ifdef ZPPP_INTEGRATION
    /* to integrate with zppp open an unix socket 
     * to recevie control packet from zppp */
    initUnixSocket();
    /* after connection accepted make the socket non blocking */
    if(fcntl(sockTxRx_g, F_SETFL, O_NONBLOCK) < 0)
    {
        perror("fcntl()");
        exit(0);
    }
#ifdef TWO_UE_WORK
    /* after connection accepted make the socket non blocking */
    if(fcntl(sockTxRx_1_g, F_SETFL, O_NONBLOCK) < 0)
    {
        perror("fcntl()");
        exit(0);
    }
#endif

#endif
    if(!InitSockets())
    {
        PDCP_LOG(LOG_DETAIL,"InitSockets returned LTE_FAIL");
        return 1;
    }
    displaycommandchoice();   
    esStart();
    if(2 < argc)
        userRunScriptCallback(1, (const UChar8**)&argv[2]);
    while(1) 
        userCmdlineParse();
    CloseSockets();

    cleanupMemPool();
    PDCP_LOG( LOG_INFO, "********** OAM RRC Simulator Stopped **********");
    return 0;
}
