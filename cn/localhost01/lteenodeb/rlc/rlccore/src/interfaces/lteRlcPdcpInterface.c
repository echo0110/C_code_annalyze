/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcPdcpInterface.c,v 1.1.1.1.12.2.2.1 2010/09/21 15:50:20 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *  This file contains the implementation of interface APIs for PDCP.  
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcPdcpInterface.c,v $
 *
 *
****************************************************************************/


/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcPdcpInterface.h"
#include "logging.h"
#include "alarm.h"
#include "lteRlcStat.h"
/* SPR 4994 Fix Start */
#include "lteRlcErrorCode.h"
/* SPR 4994 Fix End */
/* SPR 5178 changes starts here */
/* SPR 5178 changes ends here */

#ifdef ENDC_ENABLED
#include "lteRlcRrcInterface.h"
#include "lteRlcRrcExtInterface.h"
#endif

/* SPR 5178 changes starts here */

/* +SPR 20962 */
/* +-SPR 19296 */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
extern void lteMacUpdateKpiStatsIPThpUlOnDataArrival( UInt16 ueIndex , UInt16 lcId, UInt32 size, tickType_t pktTimeStamp);
#endif
/* -SPR 20962 */
#if defined(ENDC_ENABLED) &&  defined (PDCP_GTPU_INTF)
extern SInt32 writeX2uSockFD_g;
extern SockAddr writeX2uAddr_g;
extern SockAddr writeX2uAddr_g;
#endif

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/* This queue will be data interface from PDCP layer to RLC layer. */
LTE_QUEUE rlcUeDataReqQ_g ;

/* This queue will be data interface from PDCP to RLC  */
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
LTE_CIRCQUEUE pdcpRlcUeDataReqQ_g;
/* SPR 4497 Changes Start */
LTE_CIRCQUEUE pdcpRlcUeDataDiscardReqQ_g;
/* SPR 4497 Changes End */
#endif

/* UL changes - 13Jan*/

extern UInt16 rlcLayerInitialized_g;

extern UInt32 rlcHandleDataFromPdcp(RlcUeDataReq* ueDataReq_p);

/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
#ifdef PDCP_THROUGHPUT_STATS
extern UInt32 rlcDataTx;
extern UInt32 rlcPktTx;
#endif
/* SPR 15909 fix end */
/* SPR 4994 Fix Start */
#if !defined (RLC_PDCP_IPC_INTF) || defined (UE_SIM_TESTING)
static UInt32 rlcGetPdcpReturnCode(UInt32 retCode);
#endif
/* SPR 4994 Fix End */
/***************************************************************************/

#ifndef UE_SIM_TESTING

/* SPR 5178 changes ends here */
/* SPR 5178 changes starts here */
/****************************************************************************
 * Function Name  : sendRlcFirstSubframeIndToPdcp 
 * Inputs         : UInt32 sysFrameNum, UInt32 subFrameNum
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function sends RlcFirstSubframeInd to PDCP 
 *****************************************************************************/
void sendRlcFirstSubframeIndToPdcp( UInt32 sysFrameNum, UInt32 subFrameNum, 
InternalCellIndex activeCell, InternalCellIndex internalCellIndex)
{
    rlcFirstSubFrameInd(sysFrameNum, subFrameNum, internalCellIndex);
}

/****************************************************************************
 * Function Name  : sendRlcSubframeIndToPdcp 
 * Inputs         : ueIndex - UE Id.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function sends RlcSubframeInd to PDCP 
*****************************************************************************/

void sendRlcSubframeIndToPdcp(InternalCellIndex internalCellIndex)
{
    rlcSubFrameInd(internalCellIndex);
}

/****************************************************************************
 * Function Name  : sendRlcDeliveryStatusIndToPdcp
 * Inputs         : ueIndex - UE Id.
 *                   lcId    - Logical channel Id.
 *                   transactionId  - will define the transaction uniquely.
 *                                    It will be same as received in data
 *                                    request for which delievery status need to
 *                                    be sent.
 *                   deliveryStatus - the delivery status.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function receives data delivery indication (PDCP PDU)
 *                   from the RLC layer and enqueues it in RLC Delivery Status
 *                   Ind Queue.
 *****************************************************************************/
void sendRlcDeliveryStatusIndToPdcp (
        UInt16  ueIndex,
        UInt16  lcId,
        UInt32  count,
        RlcStatusBuffer* deliveryStatus_p)
{
}
#endif /* UE_SIM_TESTING */

/****************************************************************************
 * Function Name  : sendRlcUeDataIndToPdcp 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  data_p  - pointer to PDCP PDU data in Zero-copy buffering
 *                      format.
 *                  size - size of data
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function receives data indication (PDCP PDU) from
 *                   the RLC layer and stores it into the PDCP RX queue.
 *                   It's asynchronous function.
 *****************************************************************************/
void sendRlcUeDataIndToPdcp ( UInt16  ueIndex,
                              UInt16  lcId,
                              UInt8 * data_p,
                              UInt16  size,
                              UInt8 packetForReEstablishment,
                              tickType_t headPktTS,
                              UInt8 macState
                              /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                              ,tickType_t packetTime
#endif
                              /* 32.425 Events code changes end*/
                            )
{
    rlcUeDataInd ( ueIndex,
            lcId,
            data_p,
            size,
            packetForReEstablishment,
            headPktTS,
            macState
            /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
            ,packetTime
#endif
            /* 32.425 Events code changes end*/
            );
}

/* SPR 5178 changes starts here */
/***************************************************************************/

/****************************************************************************
 * Function Name  : rlcUeDataReq
 * Inputs         : ueIndex - UE Identifier of a UE,
 *                  lcId - Logical Channel ID,
 *                  data_p - Pointer to the data received from PDCP,
 *                  size - Size of data received from PDCP,
 *                  transactionIdPresent,
 *                  transId - Transaction ID,
 *                  serviceRequested - Service Requested from PDCP,
 *                  pktArrTime - Arrival time,
 *                  ipPktType - IPv4 or Ipv6,
 *                  voiceSilenceType,
 *                  avgPduSize
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to receive data from PDCP.
 ****************************************************************************/
UInt32 rlcUeDataReq ( UInt16  ueIndex, 
                      UInt16  lcId,
                      UInt8 * data_p,
                      UInt16  size,
                      UInt8 transactionIdPresent,
                      /* SPR 2812 changes start */
                      void *  transId,
                      /* SPR 2812 changes end */
                      UInt16  serviceRequested,
                      tickType_t  pktArrTime,       		      
                      UInt8   ipPktType,
                      UInt8   voiceSilenceType,
                      UInt32  avgPduSize
                      )
{
#ifdef PDCP_THROUGHPUT_STATS
    rlcDataTx += size;
    rlcPktTx++;
#endif
	/* SPR 5178 changes starts here */
	/* SPR 5178 changes ends here */
    CLOCK_START_RLC_API(RLC_UE_DATA_REQ);
#if !defined (RLC_PDCP_IPC_INTF) || defined (UE_SIM_TESTING)
    UInt32 ret = RLC_SUCCESS;
#endif

    if((RLC_NULL == data_p) || (0 == size))
    {
        if(data_p) 
        {
            /* SPR 5178 changes starts here */
            msgFree(data_p);
            /* SPR 20985 fix start */
            data_p = RLC_NULL;
            /* SPR 20985 fix end */
            /* SPACC_OPTIMIZATION */
            /* SPACC_OPTIMIZATION */
            /* SPR 5178 changes ends here */
        }    
        return RLC_FAILURE;
    }
    if((RLC_FAILURE == rlcLayerInitialized_g) || (ueIndex >= MAX_UE_ID)
            || (lcId >= MAX_LC_ID) || (lcId == 0)) 
    {
        /* SPR 5178 changes starts here */
        msgFree(data_p);
        /* SPR 20985 fix start */
        data_p = RLC_NULL;
        /* SPR 20985 fix end */
        /* SPACC_OPTIMIZATION */
        /* SPACC_OPTIMIZATION */
        /* SPR 5178 changes ends here */
        CLOCK_END_RLC_PDCP_API(RLC_UE_DATA_REQ);
        return RLC_FAILURE;
    }
    RlcUeDataReq * pdcpRlcUeDataReq_p = RLC_NULL;

     pdcpRlcUeDataReq_p =
            (RlcUeDataReq *)getMemFromPool(sizeof(RlcUeDataReq) ,RLC_NULL);

    if(RLC_NULL == pdcpRlcUeDataReq_p)
    {
/* SPACC_OPTIMIZATION */
        msgFree(data_p);
/* SPACC_OPTIMIZATION */
        CLOCK_END_RLC_PDCP_API(RLC_UE_DATA_REQ);
        return RLC_FAILURE;
    }

    pdcpRlcUeDataReq_p->ueIndex = ueIndex ;
    pdcpRlcUeDataReq_p->lcId = lcId ;
    /* SPR 5178 changes starts here */
    pdcpRlcUeDataReq_p->data_p = data_p ;
    /* SPR 5178 changes ends here */
    pdcpRlcUeDataReq_p->size = size ;
    pdcpRlcUeDataReq_p->transId = transId ;
    pdcpRlcUeDataReq_p->serviceRequested = serviceRequested ;
    pdcpRlcUeDataReq_p->transactionIdPresent = transactionIdPresent;
    pdcpRlcUeDataReq_p->pktArrTime = pktArrTime;
    pdcpRlcUeDataReq_p->ipPktType = ipPktType;
    pdcpRlcUeDataReq_p->voiceSilenceType = voiceSilenceType;
    pdcpRlcUeDataReq_p->avgSduSize = avgPduSize;
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
    /* + SPR 9253 */
    if(!(ENQUEUE_PDCP_RLC_UEDATA_REQ_Q(pdcpRlcUeDataReq_p)))
    {
        freeMemPool(pdcpRlcUeDataReq_p);
        msgFree(data_p);
        /* SPR 20985 fix start */
        data_p = RLC_NULL;
        /* SPR 20985 fix end */
        return RLC_FAILURE;
    }
    /* - SPR 9253 */
    CLOCK_END_RLC_PDCP_API(RLC_UE_DATA_REQ);
    return RLC_SUCCESS;

#else
    ret = rlcHandleDataFromPdcp(pdcpRlcUeDataReq_p);
    CLOCK_END_RLC_PDCP_API(RLC_UE_DATA_REQ);
    /* SPR 4994 Fix Start */
    return rlcGetPdcpReturnCode(ret);
    /* SPR 4994 Fix Start */

#endif
}
/* SPR 4994 Fix Start */
/****************************************************************************
 * Function Name  : rlcGetPdcpReturnCode
 * Inputs         : retCode 
 * Outputs        : None
 * Returns        : RLC_PDCP_SUCCESS/RLC_PDCP_FAIL
 * Description    : This API is used to map return caode from RLC to PDCP.
 * *************************************************************************/
#if !defined (RLC_PDCP_IPC_INTF) || defined (UE_SIM_TESTING)
static UInt32 rlcGetPdcpReturnCode(UInt32 retCode)
{
    switch (retCode)
    {
        case RLC_SUCCESS:
             retCode = RLC_PDCP_SUCCESS;
        break;
        case RLC_ENTITY_NOT_ACTIVE_MODE:
             retCode = RLC_PDCP_ENTITY_NOT_ACTIVE;
        break;
        default:
             retCode = RLC_PDCP_FAIL; 
        break;
    }
    return retCode;
}
#endif
/* SPR 4994 Fix End */

#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
/****************************************************************************
 * Function Name  : rlcHandleUeDataReq
 * Inputs         : cQCount -no of nodes in PdcpRlcUeDataReqQ_g
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to receive data from PDCP.
 * *************************************************************************/
UInt32 rlcHandleUeDataReq(UInt32 cQCount)
{

    RlcUeDataReq * rlcUeDataReq_p = RLC_NULL;
    UInt32 retVal = RLC_SUCCESS;
    while(cQCount > 0)
    {
        /* +COVERITY 26454 */
        retVal = DEQUEUE_PDCP_RLC_UEDATA_REQ_Q(rlcUeDataReq_p);
        if( (rlcUeDataReq_p == RLC_NULL) || (RLC_SUCCESS != retVal))
        /* -COVERITY 26454 */
        {
            return RLC_FAILURE;
        }

       (void)rlcHandleDataFromPdcp(rlcUeDataReq_p);
       cQCount --;
    }
    return RLC_SUCCESS;
}
#endif
       
    
/****************************************************************************
 * Function Name  : rlcDataDiscardReq
 * Inputs         : ueIndex - UE Identifier of a UE,
 *                  lcId - Logical Channel ID,
 *                  transactionId - Transaction ID
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used by PDCP to discard any PDU already sent
 *                  to RLC Layer. RLC Layer will delete the referenced PDU only
 *                  if any portion of it is not transmitted to MAC yet.
 ****************************************************************************/
UInt32 rlcDataDiscardReq ( UInt16 ueIndex, 
                           UInt16 lcId,
                           /* SPR 2812 changes start */
                           void *transactionId
                           /* SPR 2812 changes end */
                         )
{
    CLOCK_END_RLC_PDCP_API(RLC_DATA_DISCARD_REQ);
#if !defined (RLC_PDCP_IPC_INTF) || defined (UE_SIM_TESTING)
    UInt32  ret = RLC_SUCCESS;
#endif    
    if((RLC_FAILURE == rlcLayerInitialized_g) || (ueIndex >= MAX_UE_ID) 
            || (lcId >= MAX_LC_ID)) 
    {
       CLOCK_END_RLC_PDCP_API(RLC_DATA_DISCARD_REQ);
        return RLC_FAILURE;
    }
    RlcUeDataReq * pdcpRlcUeDataReq_p = RLC_NULL;

     pdcpRlcUeDataReq_p =
            (RlcUeDataReq *)getMemFromPool(sizeof(RlcUeDataReq) ,RLC_NULL);

    if(RLC_NULL == pdcpRlcUeDataReq_p)
    {
        CLOCK_END_RLC_PDCP_API(RLC_DATA_DISCARD_REQ);
        return RLC_FAILURE;
    }

    pdcpRlcUeDataReq_p->ueIndex = ueIndex ;
    pdcpRlcUeDataReq_p->lcId = lcId ;
    pdcpRlcUeDataReq_p->data_p = RLC_NULL ;
    pdcpRlcUeDataReq_p->size = 0 ;
    pdcpRlcUeDataReq_p->transId = transactionId ;

    pdcpRlcUeDataReq_p->serviceRequested = 0 ;
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
    if(!(ENQUEUE_PDCP_RLC_UEDATA_DISCARD_REQ_Q(pdcpRlcUeDataReq_p)))
    {
        freeMemPool(pdcpRlcUeDataReq_p);
        return RLC_FAILURE;
    }
    
    CLOCK_END_RLC_PDCP_API(RLC_DATA_DISCARD_REQ);
    return RLC_SUCCESS;

#else
    ret = rlcHandleDataFromPdcp(pdcpRlcUeDataReq_p);
   CLOCK_END_RLC_PDCP_API(RLC_DATA_DISCARD_REQ);
    return ret;

#endif

}

/* SPR 4497 Changes Start */
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
/****************************************************************************
 * Function Name  : rlcHandleUeDataDiscardReq
 * Inputs         : cQCount -no of nodes in PdcpRlcUeDataDiscardReqQ_g
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to receive data discard req from PDCP.
 * *************************************************************************/
UInt32 rlcHandleUeDataDiscardReq(UInt32 cQCount)
{
    RlcUeDataReq * rlcUeDataReq_p = RLC_NULL;
    UInt32 retVal = RLC_SUCCESS;
    while(cQCount > 0)
    {
        /* +COVERITY 26453 */
        retVal = DEQUEUE_PDCP_RLC_UEDATA_DISCARD_REQ_Q(rlcUeDataReq_p);
        if( (rlcUeDataReq_p == RLC_NULL) || (RLC_SUCCESS != retVal))
        /* -COVERITY 26453 */
        {
            return RLC_FAILURE;
        }

        (void)rlcHandleDataFromPdcp(rlcUeDataReq_p);
        cQCount --;
    }
    return RLC_SUCCESS;
}
#endif
/* SPR 4497 Changes End */
       
    
/****************************************************************************
 * Function Name  : rlcInitPdcpInterface
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to initialise global data.
 ****************************************************************************/
/* + SPR 17439 */
void rlcInitPdcpInterface(void) 
/* -SPR 17439 */    
{
    QINIT_RLC_UEDATA_REQ_Q() ;
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
    QINIT_PDCP_RLC_UEDATA_REQ_Q() ;
/* SPR 4497 Changes Start */
    QINIT_PDCP_RLC_UEDATA_DISCARD_REQ_Q();
/* SPR 4497 Changes End */
#endif
}
/****************************************************************************
 * Function Name  : rlcDeInitPdcpInterface
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to de-initialise pdcp global data.
 ****************************************************************************/
/* + SPR 17439 */
void rlcDeInitPdcpInterface(void) 
/* - SPR 17439 */    
{
    QDEINIT_RLC_UEDATA_REQ_Q() ;
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
    QDEINIT_PDCP_RLC_UEDATA_REQ_Q() ;
/* SPR 4497 Changes Start */
    QDEINIT_PDCP_RLC_UEDATA_DISCARD_REQ_Q() ;
/* SPR 4497 Changes End */
#endif
/* T3300 UL changes - 13Jan*/
}

/*SPR 8544 FIX START*/
#ifndef RLC_PDCP_IPC_INTF
UInt8 rlcSduQFullInd( UInt16 ueIndex, UInt8 lcId)
{
    UInt8 ret = 0;
    if((RLC_FAILURE == rlcLayerInitialized_g) || (ueIndex >= MAX_UE_ID)
            || (lcId >= MAX_LC_ID))
    {
        return RLC_FAILURE;
    }
    RlcUeDataReq * pdcpRlcUeDataReq_p = RLC_NULL;

     pdcpRlcUeDataReq_p =
            (RlcUeDataReq *)getMemFromPool(sizeof(RlcUeDataReq) ,RLC_NULL);

    if(RLC_NULL == pdcpRlcUeDataReq_p)
    {
        return RLC_FAILURE;
    }

    pdcpRlcUeDataReq_p->ueIndex = ueIndex ;
    pdcpRlcUeDataReq_p->lcId = lcId ;
    pdcpRlcUeDataReq_p->data_p = RLC_NULL ;
    pdcpRlcUeDataReq_p->size = 0 ;
    pdcpRlcUeDataReq_p->transId = 0;
    pdcpRlcUeDataReq_p->serviceRequested = 0 ;
   
    ret = rlcHandleDataFromPdcp(pdcpRlcUeDataReq_p);
    return ret;
}
#endif
/*SPR 8544 FIX END*/


#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
#if  defined PDCP_GTPU_INTF
extern SInt32 readSockX2uRelayFD_g;
/****************************************************************************
 * Function Name  : handlePdcpDataFromX2u 
 * Description    : This function receives PDCP DL data from X2U Relay module
 * Returns        : None
 ****************************************************************************/
void handlePdcpDataFromX2u()
{
    UInt8   *rlcX2uRxData_p = PNULL;
    UInt8   *transId = PNULL;
    UInt8   *pdcpData_p = PNULL;
    UInt8   transIdPresent = 0;
    fd_Set  fdSock;
    UInt16  current = 0;
    SInt16  rlcRxByteRead = 0;
    UInt16  ueIndex = 0;
    UInt16  lcId = 0;
    UInt16  msgLen = 0;
    UInt16  pdcpDataLen = 0;
    UInt16  transIdLen = 0;
    UInt16  serviceRequest = 0;
    SInt32  selectReturn;
    UInt32  result = 0;
    timeVal    wTime;
    tickType_t pktArrTime = 0;
    

    while(1)
    {
        rlcRxByteRead = 0;
        wTime.tv_sec  = 0;
        wTime.tv_usec = 200;

        lteFD_ZERO( &fdSock );
        lteFD_SET(readSockX2uRelayFD_g,&fdSock);

        while( (selectReturn = select_wrapper( readSockX2uRelayFD_g + 1, &fdSock, 0, 0, &wTime )) < 0)
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
        if (lteFD_ISSET(readSockX2uRelayFD_g,&fdSock))
        {

            rlcX2uRxData_p = (UInt8 *)getMemFromPool(BUFFERSIZE, PNULL);
            if(PNULL == rlcX2uRxData_p)
            {
                LOG_PDCP_WARNING( PDCP_RX, " MEM ALLOC FAILED ");
                return;
            }
            rlcRxByteRead = recvfrom_wrapper(readSockX2uRelayFD_g, (rlcX2uRxData_p), BUFFERSIZE,0,PNULL,0);
            lteFD_CLR(readSockX2uRelayFD_g,&fdSock);

            if(rlcRxByteRead > 0)
            {
                if(RELAY_MODULE_ID != LTE_GET_U16BIT(rlcX2uRxData_p + 2))
                {
                    return;

                }
                msgLen = LTE_GET_U16BIT(rlcX2uRxData_p + 8);
                if(INTERFACE_API_HEADER_LEN > msgLen)
                {
                    return;
                }
                current = INTERFACE_API_HEADER_LEN;

                if((X2U_RLC_DL_DATA_REQ == LTE_GET_U16BIT(rlcX2uRxData_p + 6))) 
                {

                    ueIndex = LTE_GET_U16BIT(rlcX2uRxData_p + current);
                    current +=2;

                    lcId    = LTE_GET_U16BIT(rlcX2uRxData_p + current);
                    current +=2;

                    pdcpDataLen = LTE_GET_U16BIT(rlcX2uRxData_p + current);
                    current +=2;

                    transIdPresent = *(rlcX2uRxData_p + current);
                    current++;

                    transIdLen = LTE_GET_U16BIT(rlcX2uRxData_p + current);
                    current += 2;

                    pktArrTime = LTE_GET_U32BIT(rlcX2uRxData_p + current);
                    current += 4;

                    pktArrTime = (pktArrTime << 32) | (LTE_GET_U32BIT(rlcX2uRxData_p + current));
                    current += 4;

                    serviceRequest = LTE_GET_U16BIT(rlcX2uRxData_p + current);
                    current += 2;

                    pdcpData_p = (UInt8 *)msgAlloc(PNULL, 0, 0, 0);

                    msgExtract(rlcX2uRxData_p, current, pdcpData_p, pdcpDataLen);
                    current += pdcpDataLen;

                    transId = getMemFromPool(transIdLen, PNULL);

                    msgExtract(rlcX2uRxData_p, current, transId, transIdLen);

                    ueIndex = getUeIdxFromRrcUeIdx(ueIndex);
                    result = rlcUeDataReq(
                            ueIndex,
                            lcId,
                            pdcpData_p,
                            pdcpDataLen,
                            transIdPresent,
                            (void*)transId,
                            serviceRequest,
                            pktArrTime,
                            LTE_FALSE,
                            LTE_FALSE,
                            LTE_FALSE);
                    if(RLC_FAILURE == result)
                    {
                        LOG_PDCP_WARNING( PDCP_RX, " RLC UE DATA REQ processing failed ");
                    }
                }
                else if((X2U_RLC_DATA_DISCARD_REQ == LTE_GET_U16BIT(rlcX2uRxData_p + 6)))
                {
                    ueIndex = LTE_GET_U16BIT(rlcX2uRxData_p + current);
                    current +=2;

                    lcId    = LTE_GET_U16BIT(rlcX2uRxData_p + current);
                    current +=2;

                    transIdLen = LTE_GET_U16BIT(rlcX2uRxData_p + current);
                    current += 2;

                    transId = getMemFromPool(transIdLen, PNULL);

                    msgExtract(rlcX2uRxData_p, current, transId, transIdLen);

                    ueIndex = getUeIdxFromRrcUeIdx(ueIndex);
                    if(RLC_FAILURE == rlcDataDiscardReq(ueIndex, lcId, transId))
                    {
                        LOG_PDCP_WARNING( PDCP_RX, " RLC UE DATA DISCARD REQ processing failed ");
                    }
                }
            }
            freeMemPool(rlcX2uRxData_p);
        }
    }
}
/****************************************************************************
 * Function Name  : prepareRlcX2uHeader 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function prepare RLC header
 ****************************************************************************/
void prepareRlcX2uHeader(UInt8 *buffer_p,UInt32 msgLen, UInt16 apiId)
{
    UInt16 idx=0;

    LTE_SET_U16BIT((buffer_p + idx),0); /*transactionId*/
    idx += 2;
    LTE_SET_U16BIT((buffer_p + idx),RLC_MODULE_ID);
    idx += 2;
    LTE_SET_U16BIT((buffer_p + idx),RELAY_MODULE_ID);
    idx += 2;
    LTE_SET_U16BIT((buffer_p + idx),apiId);
    idx += 2;
    /* Set the length */
    LTE_SET_U32BIT((buffer_p + idx),msgLen);
    idx += 4;
}
#endif
/****************************************************************************
 * Function Name  : sendRlcDeliveryStatusToX2U 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function send delivery status to X2U Relay module
 ****************************************************************************/
void sendRlcDeliveryStatusToX2U(UInt16 ueIndex, UInt8 lcId, UInt32 rlcStatusBufferCount,
RlcStatusBuffer *rlcQStatusBuffer_p)
{
#if  defined (PDCP_GTPU_INTF)
    UInt8  *buffer_p = PNULL;
    UInt16 currentLen = 0;
    UInt32 deliveryStatusLen = 0;
    deliveryStatusLen = sizeof(RlcStatusBuffer)*rlcStatusBufferCount + 1;
    buffer_p = getMemFromPool(BUFFERSIZE, RLC_NULL);
    if(PNULL == buffer_p)
    {
        return;
    }

    currentLen = RLC_API_HEADER_LEN;
    /*Fill UeIndex and LcId*/
    ueIndex = getRrcUeIdxFromUeIdx(ueIndex);

    LTE_SET_U16BIT((buffer_p + currentLen),ueIndex);
    currentLen += 2;

    *(buffer_p + currentLen++) = lcId;

    msgExtract(rlcQStatusBuffer_p, 0, buffer_p+currentLen, deliveryStatusLen);
    currentLen += deliveryStatusLen;

    prepareRlcX2uHeader(buffer_p,currentLen,RLC_X2U_DL_DATA_DELIVERY_STATUS);

    if (currentLen != sendto_wrapper(writeX2uSockFD_g, (UInt8 *)buffer_p,currentLen, 0,
                &(writeX2uAddr_g), sizeof(writeX2uAddr_g)))
    {
       LOG_PDCP_WARNING( PDCP_RX, "sending delivery status to X2U Fail ");
    }
    freeMemPool(buffer_p);
#else
    rlcX2uDlDataDeliveryStatus(ueIndex,lcId,rlcStatusBufferCount,rlcQStatusBuffer_p);
#endif
    freeMemPool(rlcQStatusBuffer_p);

}
/****************************************************************************
 * Function Name  : sendUeDataIndToX2U 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function send delivery status to X2U Relay module
 ****************************************************************************/
void sendUeDataIndToX2U(UInt16  ueIndex,
                        UInt16  lcId,
                        UInt8 * data_p,
                        UInt16  size,
                        UInt8 packetForReEstablishment,
                        tickType_t headPktTS,
                        tickType_t pktTimeStamp)
{
#if  defined (PDCP_GTPU_INTF)
    UInt8 *buffer_p = PNULL;
    UInt8  currentLen = 0;
    buffer_p = getMemFromPool(BUFFERSIZE, RLC_NULL);
    if(PNULL == buffer_p)
    {
        return;
    }

    currentLen = RLC_API_HEADER_LEN;
    /*Fill UeIndex and LcId*/
    ueIndex = getRrcUeIdxFromUeIdx(ueIndex);
    LTE_SET_U16BIT((buffer_p + currentLen),ueIndex);
    currentLen += 2;

    *(buffer_p + currentLen++) = lcId;

    *(buffer_p + currentLen++) = packetForReEstablishment;

    LTE_SET_U16BIT((buffer_p + currentLen),size);
    currentLen += 2;

    LTE_SET_U64BIT((buffer_p + currentLen),headPktTS);
    currentLen += 8;

    LTE_SET_U64BIT((buffer_p + currentLen),pktTimeStamp);
    currentLen += 8;

    msgExtract(data_p, 0, buffer_p+currentLen,size);

    prepareRlcX2uHeader(buffer_p,currentLen,RLC_X2U_UL_DATA_IND);

    if (currentLen != sendto_wrapper(writeX2uSockFD_g, (UInt8 *)buffer_p, currentLen, 0,
                &(writeX2uAddr_g), sizeof(writeX2uAddr_g)))
    {
       LOG_PDCP_WARNING( PDCP_RX, "sending UL Data to X2U Fail ");
    }
    freeMemPool(buffer_p);
#else
rlcX2uUlDataInd( ueIndex,lcId,
	        	data_p,
		        size) ;

#endif

}
#endif

