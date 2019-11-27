/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpExtScheduler.c,v 1.1.1.1.16.1 2010/10/25 08:24:20 gur21010 Exp $
 *
 ******************************************************************************
 *
 *  File Description : External scheduler implementation. 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpExtScheduler.c,v $
 * Revision 1.1.1.1.16.1  2010/10/25 08:24:20  gur21010
 * warning removed
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.13.2.10.2.3  2009/12/05 13:10:09  gur19479
 * updated for thread binding
 *
 * Revision 1.13.2.10.2.2  2009/10/09 06:08:08  gur11318
 * flag change to UInt16 from UInt8.
 *
 * Revision 1.13.2.10.2.1  2009/09/30 12:33:25  gur20439
 * oam and rrc IP seperated for mwc demo.
 *
 * Revision 1.13.2.10  2009/08/06 13:26:05  gur19836
 * Graceful Exit code added
 *
 * Revision 1.13.2.9  2009/07/20 08:21:49  gur19479
 * Changes done for SRB handling
 *
 * Revision 1.13.2.8  2009/07/16 05:04:01  gur19140
 * tick related problem resolve
 *
 * Revision 1.13.2.7  2009/07/14 05:43:58  gur20435
 * PDCP Optimizations
 *
 * Revision 1.13.2.6  2009/07/09 02:36:38  gur19479
 * updated for ip and port issues
 *
 * Revision 1.13.2.5  2009/07/03 14:07:27  gur19479
 * updated for pdcp adapter
 *
 * Revision 1.13.2.4  2009/06/24 06:49:41  gur19140
 * *** empty log message ***
 *
 * Revision 1.13.2.3  2009/06/23 10:49:12  gur19836
 * Changes Done for Execution Scheduler
 *
 * Revision 1.13.2.2  2009/06/19 14:28:26  gur19836
 * Changes Done for Execution Scheduler
 *
 * Revision 1.13.2.1  2009/06/09 11:27:28  gur19140
 * first wave of Integration
 *
 * Revision 1.12  2009/05/27 04:44:05  gur19479
 * updated for rohc timers
 *
 * Revision 1.11  2009/05/26 14:55:53  gur19140
 * *** empty log message ***
 *
 * Revision 1.10  2009/05/22 06:04:26  gur20548
 * bugs fixed
 *
 * Revision 1.9  2009/05/21 07:06:09  gur19140
 * memory leak fixes
 *
 * Revision 1.8  2009/05/20 13:15:13  gur20548
 * Changes header files name
 *
 * Revision 1.7  2009/05/15 18:43:21  gur19140
 * fixes
 *
 * Revision 1.6  2009/05/15 09:05:23  gur19140
 * bug fixes
 *
 * Revision 1.5  2009/05/14 11:04:58  gur19836
 * CR Comments Incorporated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "ltePdcpEsLteFramework.h"

#include <ltePdcpExecutionLegs.h>
#include <ltePdcpOamSap.h>
#include <ltePdcpRrcSap.h>

#include "ltePdcpExtScheduler.h"
#include "ltePdcpSockTools.h"
#include "ltePdcpTlvUtils.h"
#include "ltePdcpContext.h"
#include "rohcTimer.h"
#include "lteLayer2CommanTypes.h"
#include "logging.h"
#include "alarm.h"
 /*SPR 21188 +*/
#if  defined PDCP_GTPU_INTF
 /*SPR 21188 -*/
#include "ltePdcpPdcpUserSap.h"
#include "ltePdcpErrors.h"
#ifdef PDCP_ASYNC_INTERFACE
#include "ltePdcpSecEngine.h"
#endif
#endif

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/


/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* Execution Sheduler task for PDCP TX Path. It do the follwoing:
   - call to pdcpElTxProcessMessage execution leg;
 */
static void* esPdcpTxTask(void* lpParam);

/* Execution Sheduler task for PDCP RX Path. It do the follwoing:
   - call to pdcpElRxProcessMessage execution leg;
 */
static void* esPdcpRxTask(void* lpParam);

extern SInt32 sendL2InitIndToOAM(UInt8 *cnfBuff_p, UInt16 msgLen, SInt32 sendSockFD);
/*SPR 20863 FIX*/
static UInt8 InitPdcpSockets(void);
#if  defined PDCP_GTPU_INTF
STATIC void msgAndMemPoolUsage( UInt32 *msgPoolUsage, UInt32 *memPoolUsage,
        UInt8 *msgPoolOverFlow, UInt8 *memPoolOverFlow );
#ifdef PDCP_ASYNC_INTERFACE
#define MAX_PACKETS_TX_SEC_Q 500
#endif
#endif
/*SPR 20863 FIX*/
/*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
extern Int pdcpSocketRx;
extern Int pdcpGtpuRx;
/*SPR 21635 +*/
#define NUM_OF_PDUS_READ 50
/*SPR 21635 -*/
#endif
/*SPR 21188 -*/
/*SPR 20908 +*/
SInt32 pdcpRxSockFD_g = -1;
/*SPR 20908 +*/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
extern void CloseSockets(void);
extern UInt8  pdcpInitInd_g;

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

static ULong32   es_tTxId; /* Sheduler TX task Id */
static ULong32   es_tRxId; /* Sheduler RX task Id */
static LTE_SEM     es_sem_stop; /* Stop Sheduler task semaphore   */
static LTE_SEM     es_sem_block_rx; /* Sheduler semaphore for blocking RX 
                                       task while OAM/RRC commands processed*/

/****************************************************************************
 * Function Name  : initPdcpLayer 
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Startup routine for PDCP Layer which just creates RRC 
 *                  thread to receive messages from OAM/RRC on socket.
 ****************************************************************************/
/* + SPR 17439 */
UInt32 initPDCPLayer (void)
    /* - SPR 17439 */
{
    UInt32 ret = 0;
    initPdcpLog(PNULL);
    pdcpLogLevel(LOG_DUMP);
    PDCP_LOG(LOG_DUMP, "Start %s", __FUNCTION__);
    ret = InitPdcpSockets();
    if(!ret) {
        ltePanic("Socket Init Fail in PDCP");
    }

    return LTE_TRUE;
}

/****************************************************************************
 * Function Name  : cleanUpPdcpLayer 
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Runs RRC thread 
 ****************************************************************************/
/* + SPR 17439 */
UInt32 cleanUpPDCPLayer (void)
/* - SPR 17439 */    
{
    UInt32 ret = 0;
    CloseSockets();
    return ret ;
}

/****************************************************************************
 * Function Name  : esStart 
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Runs RLC layer simulator.
 ****************************************************************************/
/* + SPR 17439 */
void    esStart(void)
/* - SPR 17439 */    
{
    PDCP_LOG(LOG_DUMP, "Start %s", __FUNCTION__);

    semInit(&es_sem_stop, 0);
    semInit(&es_sem_block_rx, 1);
    /* create TX task */
    threadCreate(
            &es_tTxId, 
            THREAD_DEFAULT_STATE, 
            THREAD_SCHED_DEFAULT,
            0, 
            THREAD_INHERIT_SCHED_DEFAULT, 
            THREAD_SCOPE_DEFAULT,
            &esPdcpTxTask, PNULL);

    /* create RX task */
    threadCreate(
            &es_tRxId, 
            THREAD_DEFAULT_STATE, 
            THREAD_SCHED_DEFAULT,
            0, 
            THREAD_INHERIT_SCHED_DEFAULT, 
            THREAD_SCOPE_DEFAULT,
            &esPdcpRxTask, PNULL);
}

/****************************************************************************
 * Function Name  : esStop 
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Stops RLC layer simulator.
 ****************************************************************************/
/* + SPR 17439 */
void    esStop(void)
    /* - SPR 17439 */
{
    void *status;
    PDCP_LOG(LOG_DUMP, "Stop %s", __FUNCTION__);
    semPost(&es_sem_stop);
    threadJoin(es_tTxId, &status);
    threadJoin(es_tRxId, &status);
    semDestroy(&es_sem_block_rx);
    semDestroy(&es_sem_stop);
}

/****************************************************************************
 * Function Name  : setPdcpCommPort 
 * Inputs         : flag,
 *                  pdcpRxPort - PDCP Rx Port to be initialized
 *                  oamTxPort - OAM Tx Port
 *                  rrcTxPort - RRC Tx Port
 *                  rrcOamHostName - RRC OAM IP
 *                  l2HostName - L2 IP
 *                  pdcpCmdRxPort - PDCP CMD Rx Port
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function sets the pdcp comminication port
 ****************************************************************************/
/* SPR 21983 Fix + */
extern Char8 oamIPAddress_g[HOSTNAME_MAX_LEN];
extern UInt16 txPortMacOAM_g;
/* SPR 21983 Fix - */
/*SPR 20863 FIX*/
void setPdcpCommPort(UInt16 flag, UInt16 pdcpRxPort,
                     UInt16 oamTxPort, UInt16 rrcTxPort,
                     Char8 *oamIp_p, Char8 *rrcIp_p,
                     Char8 *pdcpIp_p, Char8 *rrmIp_p, UInt16 rrmTxPort
                     /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
                     ,Char8 *gtpuIp_p, UInt16 gtpuRxPort, UInt16 pdcpGtpuRxPort
#endif
                     /*SPR 21188 -*/
                     )
{
    if(flag){
        /* SPR 21983 Fix + */
        txPortMacOAM_g = oamTxPort;
        strNCpy(oamIPAddress_g,oamIp_p,sizeof(oamIPAddress_g));
        /* SPR 21983 Fix - */
        configPdcpCommPort(pdcpRxPort,oamTxPort,rrcTxPort,
                            oamIp_p,rrcIp_p,pdcpIp_p, rrmIp_p, rrmTxPort
                            /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
                            ,gtpuIp_p,gtpuRxPort,pdcpGtpuRxPort
#endif
                            /*SPR 21188 -*/
                            ); 
    }
}
/*SPR 20863 FIX*/

/* + SPR 17439 */
UInt32 processTxPendingDelQ(void)
/* - SPR 17439 */    
{
    LP_PdcpTxPendingDeletionReq delReq_p = PNULL;
    LP_PdcpUeContext ueContext_p = PNULL;
    LP_PdcpEnityContext entity_p = PNULL;    
    UInt8 lcId = 0;
    UInt16 ueIndex = 0;
    UInt32 qCount = 0;
    qCount = QCOUNT_PDCP_RX_PENDING_DELETION_Q();
    while(qCount) {
        qCount--;
        DEQUEUE_PDCP_TX_PENDING_DELETION_Q(delReq_p);
        if( PNULL == delReq_p )
        {
            /*Nothing to process*/
            return LTE_FALSE;
        }
        lcId = delReq_p->lcId ; 
        ueIndex = delReq_p->ueIndex;
        ueContext_p = delReq_p->ueContext_p;
        if(ueContext_p){
            entity_p = ueContext_p->pdcpEntities[lcId][PDCP_ENTITY_RB_TX];
            if(entity_p){
                /* SPR 3159 changes start */
                pdcpEntityContextFree(entity_p,ueIndex,lcId,PDCP_ENTITY_RB_TX);
                /* SPR 3159 changes end */
                
                /*SPR 21128 Start*/
                semDestroy((LTE_SEM *)&(entity_p->pdcpTxDataReqQ).sem_lock);
                semDestroy((LTE_SEM *)&(entity_p->pdcpStorageQ).sem_lock);
                /*SPR 21128 End*/
                
                freeMemPool(entity_p);
                entity_p = PNULL;
                ueContext_p->pdcpEntities[lcId][PDCP_ENTITY_RB_TX] = PNULL;
            }
        }
        freeMemPool(delReq_p);
        delReq_p = PNULL;
    }
    return LTE_TRUE;
}


#ifdef PDCP_UT

UInt32 processRxPendingDelQ()
{
    LP_PdcpRxPendingDeletionReq delReq_p = PNULL;
    LP_PdcpUeContext ueContext_p = PNULL;
    LP_PdcpEnityContext entity_p = PNULL;    
    UInt8 lcId = 0;
    UInt32 qCount = 0;
    UInt16 ueIndex = 0;
    qCount = QCOUNT_PDCP_RX_PENDING_DELETION_Q();
    while(qCount) {
        qCount--;
        DEQUEUE_PDCP_RX_PENDING_DELETION_Q(delReq_p);
        if ( PNULL == delReq_p ) {
            /*Nothing to process*/
            return LTE_FALSE;
        }
        PDCP_LOG(LOG_DUMP, "In %s", __FUNCTION__);
        lcId = delReq_p->lcId ; 
        ueContext_p = delReq_p->ueContext_p;
        ueIndex = delReq_p->ueIndex;
        if(ueContext_p){
            entity_p = ueContext_p->pdcpEntities[lcId][PDCP_ENTITY_RB_RX];
            if(entity_p){
                /* SPR 3159 changes start */
                pdcpEntityContextFree(entity_p,ueIndex,lcId,PDCP_ENTITY_RB_RX);
                /* SPR 3159 changes end */
                
                /*SPR 21128 Start*/
                DEINIT_PDCP_RX_DATA_IND_Q( entity_p );
                semDestroy((LTE_SEM *)&(entity_p->pdcpUlReorderingQ).sem_lock);
                /*SPR 21128 End*/
                
                freeMemPool(entity_p);
                entity_p = PNULL;
                ueContext_p->pdcpEntities[lcId][PDCP_ENTITY_RB_RX] = PNULL;
            }
            if(delReq_p->deleteContext) {
                entity_p = ueContext_p->pdcpEntities[lcId][PDCP_ENTITY_RB_TX];
                while(entity_p){
                    PDCP_LOG(LOG_WARNING, "Tx Entity still panding\n");
                }
                freeMemPool(ueContext_p);
                ueContext_p  = PNULL;
            }
        }
        freeMemPool(delReq_p);
        delReq_p = PNULL;
    }
    return LTE_TRUE;
}

#endif

/****************************************************************************
 * Function Name  : InitPdcpSockets 
 * Inputs         : None
 * Outputs        : None.
 * Returns        : SUCCESS or FAILURE.
 * Description    : This function performs initialization of the sockets 
 *                  engine. Should be called on program startup before using
 *                  other socket functions
 ****************************************************************************/
/* + SPR 17439 */
/*SPR 20863 +*/
UInt8 InitPdcpSockets(void)
/* - SPR 17439 */    
{
    /* Init RX sockets */
    /*SPR 20908 +*/
    if(SOCKET_ERROR == ( pdcpRxSockFD_g = InitRxSocket(PDCP_MODULE_ID,
                    LTE_TRUE)) )
    {
        return LTE_FALSE;
    }
    /*SPR 20908 -*/
    /* Init TX sockets */
    if(SOCKET_ERROR==InitTxSocket(RRC_MODULE_ID)) {
        /* SPR 3444 changes start */
        CloseSocket( PDCP_MODULE_ID );
        /* SPR 3444 changes end */
        return LTE_FALSE;
    }
    if(SOCKET_ERROR==InitTxSocket(OAM_MODULE_ID)) {
        /* SPR 3444 changes start */
        CloseSocket( PDCP_MODULE_ID );
        CloseSocket( RRC_MODULE_ID );
        /* SPR 3444 changes end */
        return LTE_FALSE;
    }
    
    if(-1==InitTxSocket(RRM_MODULE_ID)) {
        CloseSocket( PDCP_MODULE_ID );
        CloseSocket( RRC_MODULE_ID );
        CloseSocket( OAM_MODULE_ID );
        return LTE_FALSE;
    }
#ifdef PDCP_GTPU_INTF
    if(-1==InitTxSocket(PACKET_RELAY_MODULE_ID)) {
        CloseSocket( PDCP_MODULE_ID );
        CloseSocket( RRC_MODULE_ID );
        CloseSocket( OAM_MODULE_ID );
        CloseSocket( RRM_MODULE_ID );
        return LTE_FALSE;
    }
    
    if(-1==InitGtpuPdcpRxSocket(PDCP_MODULE_ID,LTE_TRUE)) {
      CloseSocket( PDCP_MODULE_ID );
      CloseSocket( RRC_MODULE_ID );
      CloseSocket( OAM_MODULE_ID );
      CloseSocket( RRM_MODULE_ID );
      CloseSocket( PACKET_RELAY_MODULE_ID );

      return LTE_FALSE;
    }
#endif
    return LTE_TRUE;
}
/*SPR 20863 -*/

/****************************************************************************
 * Function Name  : esPdcpTxTask 
 * Inputs         : lpParam - not used.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Execution Sheduler task for PDCP TX Path. 
 *                  It do the following:
 *                       - call to pdcpElTxProcessMessage execution leg;
 ****************************************************************************/
void* esPdcpTxTask(void* lpParam)
{
    /* SPR +- 17777 */
  LTE_GCC_UNUSED_PARAM(lpParam)
    SInt32  sem_state;  
    PDCP_LOG(LOG_DUMP, "Start %s", __FUNCTION__);
    	
    /************************thread bind code start***************************/
    unsigned long new_mask = 0x01;
    pid_t p = 0;
    THREAD_TO_BIND_GET_CORE_NO(p);
    THREAD_TO_BIND_SET_CORE_NO(p,new_mask);
    THREAD_TO_BIND_GET_CORE_NO(p);
    /************************thread bind code end*****************************/


    do {

        /*Process one RLC data delivery indication (PDCP PDU) message from PDCP delivery indication TX queue*/
        pdcpElTxProcessDeliveryInd();

        /*Process discard timers */
        pdcpElTxProcessDiscardTimers();
        rohcProcessTimers();
        /* get and check stop task condition */
        sem_state = semTryWait(&es_sem_stop);
    } while (sem_state);

    semPost(&es_sem_stop);
    PDCP_LOG(LOG_DUMP, "Stop %s", __FUNCTION__);
    return PNULL;
}

/****************************************************************************
 * Function Name  : pdcpElTxProcessSocket 
 * Inputs         : buf - a pointer to buffer,
 *                  length - length of buffer
 * Outputs        : None.
 * Returns        : None.
 * Description    : Process incoming UDP messages
 ****************************************************************************/
void pdcpElTxProcessSocket(UInt8 *buf,UInt32 length)
{
    Char8* str_p = PNULL;
    UInt16 sourceModuleId = 0;
    /* SPR_1349_FIX_START*/
    LP_TlvHeader    messageHeader_p = PNULL;
    /* SPR_1349_FIX_END*/
#ifdef PDCP_GTPU_INTF
    UInt32 msgPoolUsage = 0;
    UInt32 memPoolUsage = 0;
    static UInt8 msgPoolOverFlow = 1 ;
    static UInt8 memPoolOverFlow = 1;
#endif
    /* check packet */
    if ( 0 < length ) 
    {
        if ((sizeof(TlvHeader) <= length)) /*packet length is checked by TLV parser */
        {
            /* SPR_1349_FIX_START*/
            messageHeader_p = (TlvHeader *)buf;
            sourceModuleId = PDCP_NTOHS(messageHeader_p->sourceModuleId);
            /* SPR_1349_FIX_END*/
            if ( OAM_MODULE_ID == sourceModuleId )
            {
                str_p = "OAM-REQ RECV\t";
                LOG_PDCP_MSG( L2_SOCKET_RECVFROM_SUCCESS_ID, LOGINFO, PDCP_RRCOAM,
                          pdcpCurrentTime_g,
                          sourceModuleId,length , 0, 0, 0,
                          0,0,
                          __func__,"OAM_REQ_RECV" );
            }
            else if ( RRC_MODULE_ID == sourceModuleId )
            {
                str_p = "RRC-REQ RECV\t";
                LOG_PDCP_MSG( L2_SOCKET_RECVFROM_SUCCESS_ID, LOGINFO, PDCP_RRCOAM,
                          pdcpCurrentTime_g,
                          sourceModuleId,length, 0, 0, 0,
                          0,0,
                          __func__,"RRC-REQ RECV");
            }
            /*SPR 20863 +*/
            else if ( RRM_MODULE_ID == sourceModuleId )
            {
                str_p = "RRM-REQ RECV\t";
                LOG_PDCP_MSG( L2_SOCKET_RECVFROM_SUCCESS_ID, LOGINFO, PDCP_RRCOAM,
                          pdcpCurrentTime_g,
                          sourceModuleId,length, 0, 0, 0,
                          0,0,
                          __func__,"RLC-REQ RECV");
            }
            /*SPR 20863 -*/
            /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
            else if ( PACKET_RELAY_MODULE_ID == sourceModuleId )
            {
                str_p = "GTPU-REQ RECV\t";
                LOG_PDCP_DEBUG( PDCP_TX, "Receive data from Relay, "
                        "sourceModuleId[%d] length[%d]",
                          sourceModuleId,length );
            }
            /*SPR 21188 -*/
#endif
            else 
            {
                PDCP_LOG(LOG_WARNING, 
                        "OAM-RRC: Incorrect Source module id. sourceModuleId[%i]", 
                        sourceModuleId );
            }

            if ( PNULL != str_p )
            {
                /* make some action */
                if ( OAM_MODULE_ID == sourceModuleId )
                {
                    pdcpOamProcessMessage( buf, length );
#if defined PDCP_GTPU_INTF
                    sendFreetoGTP( PNULL, buf );
#endif
                }
                else if ( RRC_MODULE_ID == sourceModuleId )
                {
                    pdcpRrcProcessMessage( buf, length );
#if defined PDCP_GTPU_INTF
                    sendFreetoGTP( PNULL, buf );
#endif
                }
                /*SPR 20863 +*/
                else if ( RRM_MODULE_ID == sourceModuleId )
                {
                    processPdcpRRMMsg( buf, length );
#if defined PDCP_GTPU_INTF
                    sendFreetoGTP( PNULL, buf );
#endif
                }
                /*SPR 20863 -*/
                /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
                else if(PACKET_RELAY_MODULE_ID == sourceModuleId)
                {
                    msgAndMemPoolUsage( &msgPoolUsage, &memPoolUsage,
                            &msgPoolOverFlow, &memPoolOverFlow );
                    UInt8 *freeBuff_p = buf;
#ifdef PDCP_ASYNC_INTERFACE
                    UInt32 sQCount = QCOUNT_PDCP_SEC_INPUT_TX_Q();
                    if ( MAX_PACKETS_TX_SEC_Q < sQCount )
                    {
                        LOG_MSG(PR_SEC_Q_FULL, LOGWARNING, PR_GTPU_DL_DATA,
                                pdcpCurrentTime_g, __LINE__, sQCount,
                                0,0,0, 0,0, __func__,"Stopping traffic");
                        /* Send free notification to GTP */
                        sendFreetoGTP( PNULL, freeBuff_p );
                        return;
                    }
                        /* + SPR 19525 */
                    sQCount = QCOUNT_PDCP_SEC_OUTPUT_TX_Q( pdcpSecConfig_g );
                    if ( MAX_PACKETS_TX_SEC_Q < sQCount )
                    {
                        LOG_MSG(PR_SEC_Q_FULL, LOGWARNING, PR_GTPU_DL_DATA,
                                pdcpCurrentTime_g, __LINE__, sQCount,
                                0,0,0, 0,0, __func__,"Stopping traffic");
                        /* Send free notification to GTP */
                        sendFreetoGTP( PNULL, freeBuff_p );
                        return;
                    }
                        /* - SPR 19525 */
#endif
                    /*Memory usage has hit high water mark, drop the packet*/
                    if ((1 == msgPoolOverFlow) || (1 == memPoolOverFlow) )
                    {
                        lteLogDropedPkt("Dropping packets msgPoolUsage = %d  "
                                "memPoolUsage = %d\n", 
                                msgPoolUsage,memPoolUsage );
                        /* Send free notification to GTP */
                        sendFreetoGTP( PNULL, freeBuff_p );
                        return;
                    } 
                    pdcpPrProcessMessage(buf, length);
                }
#endif
                /*SPR 21188 -*/
            }
        }
        else /* incorrect packet - drop it */
        {
            PDCP_LOG(LOG_WARNING, 
                    "OAM-RRC: Incorrect packet. length[%i] TLVlength[%i]", 
                    length, PDCP_NTOHS(((TlvHeader *)buf)->length));
        }
    }
}

/****************************************************************************
 * Function Name  : esPdcpRxTask 
 * Inputs         : lpParam - not used.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Execution Sheduler task for PDCP RX Path. It do the 
 *                  following:
 *                      - call to pdcpElRxProcessMessage execution leg;
 ****************************************************************************/
void* esPdcpRxTask(void* lpParam)
{
    /* SPR +- 17777 */
    LTE_GCC_UNUSED_PARAM(lpParam)
        SInt32  sem_state;  

    /************************thread bind code start***************************/
    unsigned long new_mask = 0x01;
    pid_t p = 0;
    THREAD_TO_BIND_GET_CORE_NO(p);
    THREAD_TO_BIND_SET_CORE_NO(p,new_mask);
    THREAD_TO_BIND_GET_CORE_NO(p);
    /************************thread bind code end*****************************/


    PDCP_LOG(LOG_DUMP, "Start %s", __FUNCTION__);
    do {
        /* Process one SDU from PDCP TX queue */
        pdcpElRxProcessMessage();

        /* get and check stop task condition */
        sem_state = semTryWait(&es_sem_stop);
    } while (sem_state);

    semPost(&es_sem_stop);
    PDCP_LOG(LOG_DUMP, "Stop %s", __FUNCTION__);
    return PNULL;
}

#if  defined PDCP_GTPU_INTF
/****************************************************************************
 * Function Name  : sendFreetoGTP
 * Inputs         : q - Pointer to the first data.
 *                  buf_p - Pointer to data to be freed.
 * Outputs        : void
 * Returns        : None
 * Description    : This API sends indication to GTP process to free the
 *                  buffer
 ****************************************************************************/
void sendFreetoGTP ( void *q, void *buf_p )
{
    freeMemPool( buf_p );
}


/****************************************************************************
 * Function Name  : mbSendDataPktsToPr
 * Inputs         : buf_p - Pointer to mailBuffer
 *                  buf_len - Length of Buffer
 * Outputs        : void
 * Returns        : None
 * Description    : This API sends the packet to RLC over mailbox.
 ****************************************************************************/
extern UInt64 uplinkEgressDataSize;
extern UInt64 uplinkEgressDataNum;
void mbSendDataPktsToPr(void *buf_p, UInt16 buf_len)
{
    uplinkEgressDataSize += buf_len;
	uplinkEgressDataNum++;

    (void)WriteToSocket( buf_p, buf_len, PACKET_RELAY_MODULE_ID );
    freeMemPool( buf_p );
}

/*SPR 21188 -*/

/******************************************************************************
 * Function Name  : msgAndMemPoolUsage
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This Function will get Message and Memory Pool 
 *****************************************************************************/
#define MSG_POOL_HIGH_WATERMARK         70
#define MSG_POOL_LOW_WATERMARK          60
#define MEM_POOL_HIGH_WATERMARK         70
#define MEM_POOL_LOW_WATERMARK          60
void msgAndMemPoolUsage( UInt32 *msgPoolUsage, UInt32 *memPoolUsage,
        UInt8 *msgPoolOverFlow, UInt8 *memPoolOverFlow )
{
    *msgPoolUsage = qvMsgPoolGetUsagePercentage();
    if ((MSG_POOL_HIGH_WATERMARK < *msgPoolUsage) && (*msgPoolOverFlow == 0))
    {
        *msgPoolOverFlow = 1;
    }
    else if ((MSG_POOL_LOW_WATERMARK > *msgPoolUsage) && (*msgPoolOverFlow == 1))
    {
        *msgPoolOverFlow = 0;

    }

    *memPoolUsage = qvMemPoolGetUsagePercentage();
    if ((MEM_POOL_HIGH_WATERMARK < *memPoolUsage) && (*memPoolOverFlow == 0))
    {
        *memPoolOverFlow = 1;
    }
    else if ((MEM_POOL_LOW_WATERMARK > *memPoolUsage) && (*memPoolOverFlow == 1))
    {
        *memPoolOverFlow = 0;
    }
}

/****************************************************************************
 * Function Name  : handleUserData
 * Inputs         : None
 * Outputs        : void
 * Returns        : None
 * Description    : This API receives the packet from GTP/OAM/RRC/RRM over 
 *                  mailbox.
****************************************************************************/
void handleUserData( void )
{
    fd_Set          fdSock;
    SInt32          selectReturn;
    UInt8 *pdcpTxData = PNULL;
    /*SPR 21635 +*/
    SInt16 pdcpGtpuRxByteRead = 0;
    SInt16 pdcpRxByteRead = 0;
    UInt8 *pdcpGtpuRxData = PNULL;
    SInt32          highFd = 0;
    timeVal    wTime;
    UInt32 pdusRead = 0;
    while(1)
    {
        pdcpGtpuRxByteRead = 0;
        pdcpRxByteRead = 0;
        wTime.tv_sec  = 0;
        wTime.tv_usec = 400;
        lteFD_ZERO( &fdSock );
        lteFD_SET(pdcpSocketRx,&fdSock);
        if(pdcpSocketRx > highFd) highFd = pdcpSocketRx;

        lteFD_SET(pdcpGtpuRx,&fdSock);
        if (pdcpGtpuRx > highFd) highFd = pdcpGtpuRx;

        while( (selectReturn = select_wrapper( highFd + 1, &fdSock, 0, 0, &wTime )) < 0)
        {
            if (errno == EINTR)
                continue;
            pError("PDCP Select Failed!");
            return;
        }
        if(!selectReturn)
        {
            return;
        }
      /*21366 +*/
        if (lteFD_ISSET(pdcpGtpuRx,&fdSock))
        {

            pdcpGtpuRxData = (UInt8 *)getMemFromPool(SOCKET_PDCP_RECV_BUF_SIZE, PNULL);
            if(PNULL == pdcpGtpuRxData)
            {
                LOG_PDCP_WARNING( PDCP_RX, " MEM ALLOC FAILED ");
                return;
            }
            pdcpGtpuRxByteRead = PdcpReadFromSocket(pdcpGtpuRxData, PDCP_MODULE_ID,pdcpGtpuRx);
            lteFD_CLR(pdcpGtpuRx,&fdSock);

            if(pdcpGtpuRxByteRead > 0)
            {   
                pdcpElTxProcessSocket(pdcpGtpuRxData,pdcpGtpuRxByteRead);
                pdusRead++;
            }
            else
            {
                freeMemPool(pdcpGtpuRxData);
            }
        } 
      /*21366 -*/
        if (lteFD_ISSET(pdcpSocketRx,&fdSock))
        {
            pdcpTxData = (UInt8 *)getMemFromPool(SOCKET_BUFFER_SIZE, PNULL);
            if(PNULL == pdcpTxData)
            {
                lteWarning("In %s, MEMORY ALLOC FAIL\n",__func__);
                return;
            }
            pdcpRxByteRead = PdcpReadFromSocket(pdcpTxData, PDCP_MODULE_ID,pdcpSocketRx);
            lteFD_CLR(pdcpSocketRx,&fdSock);

            if(pdcpRxByteRead > 0)
            {   
                pdcpElTxProcessSocket(pdcpTxData,pdcpRxByteRead);
            }
            else
            {
                freeMemPool(pdcpTxData);
            }
        }
        if((pdcpGtpuRxByteRead == 0) || (pdusRead == NUM_OF_PDUS_READ))
        {
            break;
        }
    }
    /*SPR 21635 -*/
}
#endif

/*SPR 21188 -*/

#if !defined(UE_SIM_TESTING)
/****************************************************************************
 * Function Name  : pdcpTrigInitInd
 * Inputs         : instanceId - instance of the cell
 * Outputs        : void
 * Returns        : None
 * Description    : This function triggers init indication message to OAM.
****************************************************************************/
void pdcpTrigInitInd( UInt8 instanceId )
{
    /* Reserve the space for QMI header while preparing indication */
    UInt8 *initIndBuff = (UInt8 *)getMemFromPool( (
                INTERFACE_API_HEADER_LEN), PNULL );
    UInt16 length = 0;
    if( PNULL == initIndBuff )
    {
        LOG_PDCP_FATAL( PDCP_RRCOAM, "Unable to allocate memory!" );
        ltePanic(  "Unable to allocate memory!" );
    }
    else
    {
        if(LTE_FALSE == pdcpInitInd_g)
        {
            oamCmdPrepInitInd( initIndBuff
                    , &length, instanceId );
            /* Trigger PDCP Init ind to OAM  */
            if( PDCP_SEND_ERROR == sendL2InitIndToOAM( initIndBuff, length,
                        pdcpRxSockFD_g ) )
            {
                LOG_PDCP_ERROR( PDCP_RRCOAM, "Unable to send to OAM");
                ALARM_MSG( PDCP_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM );
            }
        }
        freeMemPool( initIndBuff );
    }
}
#endif
