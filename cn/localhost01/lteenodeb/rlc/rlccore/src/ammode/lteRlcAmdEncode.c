/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcAmdEncode.c,v 1.1.6.2.2.2 2010/10/07 13:34:13 gur23054 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file describes encoding of SDU(s) to form AMD PDU.
 *
 ****************************************************************************
 *
 *  Revision Details
 * -----------------
 *
 *  $Log: lteRlcAmdEncode.c,v $
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcAmArithmetic.h"
#include "lteRlcAmdEncode.h"
#include "lteRlcPdcpInterface.h"
#include "lteRlcAmdArqOperation.h"
#include "lteRlcRrcInterface.h"
#include "logging.h"
//#include "socket_utils.h"
#include "alarm.h"
#include "lteRlcStat.h"
#include "atomic-ops.h"
#include "lteRlcErrorCode.h"
#include "ltePerfStats.h"
/* SPR 13888 fix start */
#include "lteCommonStatsManager.h"
/* SPR 13888 fix end */
/* + PERF_CA GBL */
#include "lteRlcUeContext.h"
/* - PERF_CA GBL */

/****************************************************************************
 * Varibale Definition
 ****************************************************************************/
/****************************************************************************
 * External Variable Includes
 ****************************************************************************/
extern void *zeroPool_g;
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */
/* SPR 4547 Changes Start */
RlcPduHeaderInfo  pduHdrInfo_g[MAX_NUM_CELL];
/* SPR 4547 Changes End */

#define RLC_MAX_FI_LENGTH 2048
/****************************************************************************
 * Function Name  : rlcAddDfeToAmdPdu
 * Inputs         : pduData_p - a pointer to AMD PDU buffer,
 *                  pDfe - pointer to Sdu to be added,
 *                  size - size of  SDU,
 *                  pPduHdrInfo - pointer to structure to maintain info for
 *                                header formation,
 *                  pAvaliableSize - pointer to remaining requested size of PDU,
 *                  txSduIn_p - pointer to AmdRawSduIn
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : 1. Adds SDU(segmented) to PDU if avaliable size
 *                     is greater than SDU
 *                  2. Split and then add SDU segment to PDU if avaliable size
 *                     is less than SDU  size
 *                  3. Store incomplete SDU info if any 
 *                  4. Update header size
 *                  5. Update num of DFE's.
 ****************************************************************************/
 
static UInt32 rlcAddDfeToAmdPdu(UInt8 *pduData_p,
                                       UInt8 *pDfe,
                                       UInt16 size,
                                       RlcPduHeaderInfo *pPduHdrInfo,
                                       UInt32 *pAvailableSize,
                                       AmdRawSduIn * txSduIn_p)
{
    UInt8   *pUnsentSeg   = NULL;
    UInt32  availableSize = *pAvailableSize;
    UInt32  unsentSegSz   = 0;
    UInt16  numDfe        = pPduHdrInfo->numDfe + 1;

    /* If available size is less than 2bytes, do not encode */
    /** SPR 3823 Changes Start **/
    if ((*pAvailableSize <= 2) && (pPduHdrInfo->numDfe))
    /** SPR 3823 Changes End **/
    {
        LOG_RLC_MSG( LTE_RLC_ADD_DFE_TO_AM_PDU, LOGWARNING,
                (RLC_TX|RLC_AM), globalTTITickCount_g, __LINE__,
                0, 0, 0, 0, 0, 0, __func__, "");	   
        return RLC_INSUFF_SIZE_FOR_ENCODING;
    }

    /* Calculate available size for data */
    if(pPduHdrInfo->numDfe > 0)
    {
        availableSize -= (((numDfe +1) & 1) + 1);
    }

    /* If available size < SDU size, split the SDU */
    if (availableSize < size)
    {
        if ( (pUnsentSeg = msgAlloc(zeroPool_g,1,0,0) ) == RLC_NULL)
        {
            LOG_RLC_MSG( LTE_RLC_ADD_DFE_TO_AM_PDU_1, LOGWARNING,
                    (RLC_TX|RLC_AM), globalTTITickCount_g, __LINE__,
                    0, 0, 0, 0, 0, 0, __func__, "");	   
            return RLC_MSG_ALLOC_FAIL;
        }

        /* Split the segment at available size */
        if (msgSplit(pDfe, availableSize, pUnsentSeg) == RLC_NULL)
        {
            LOG_RLC_MSG( LTE_RLC_ADD_DFE_TO_AM_PDU_2, LOGWARNING,
                    (RLC_TX|RLC_AM), globalTTITickCount_g, __LINE__,
                    0, 0, 0, 0, 0, 0, __func__, "");	   
            msgFree(pUnsentSeg);
            /* SPR 20985 fix start */
            pUnsentSeg = RLC_NULL;
            /* SPR 20985 fix end */
            return RLC_MSG_SPLIT_FAIL;
        }
        /*  If there is any unsent Segment still left, save its size in Incomplete SDU */
        unsentSegSz = (size - availableSize)  ;
        /* Update length of Dfe */
        pPduHdrInfo->dfeLength[numDfe - 1] = availableSize;
        availableSize = 0;
    }
    else
    {
        pPduHdrInfo->dfeLength[numDfe - 1] = size;
        if(size > RLC_MAX_FI_LENGTH)
        {
            availableSize = 0;
        }
        else
        {
            availableSize -= size;
        }
        txSduIn_p->rlcAmRawSdu.size = 0  ;
    }

    /* Join SDU with PDU data */
    msgJoinAndFree(pduData_p, pDfe);
    /*  If there is any unsent Segment still left, save its pointer and size in Incomplete SDU */
    txSduIn_p->rlcAmRawSdu.data_p = pUnsentSeg ;
    txSduIn_p->rlcAmRawSdu.size = unsentSegSz ;

    /* Update header info */
    pPduHdrInfo->numDfe++;
    if(pPduHdrInfo->numDfe > 1)
        pPduHdrInfo->hdrSize +=(((pPduHdrInfo->numDfe +1) & 1) + 1);

    /* update available size */
    *pAvailableSize = availableSize;
 

    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : rlcAddSduSegmentToAmdPdu
 * Inputs         : pduData_p - a pointer to UMD PDU buffer,
 *                  pPduHdrInfo - pointer to structure to maintain info for
 *                                header formation,
 *                  pAvaliableSize - pointer to remaining requested size of PDU,
 *                  txSduIn_p - pointer to AmdRawSduIn,
 *                  amRetxPdu_p - pointer to retransmission buffer
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : 1. API to add Incomplete SDU segment to PDU 
 *                  2. Update fi accordingly
 *                  3. Update tIdSnCount for the segmented SDU in tid queue.
 ****************************************************************************/

static UInt32 rlcAddSduSegmentToAmdPdu( UInt8 *pduData_p, 
                                               RlcPduHeaderInfo *pPduHdrInfo,
                                               UInt32 *pAvailableSize,
                                               AmdRawSduIn *txSduIn_p)
                                               /* +- SPR 17777 */
{
    UInt32 retVal = RLC_SUCCESS;
    if ( RLC_NULL != txSduIn_p->rlcAmRawSdu.data_p )
    {
        retVal = rlcAddDfeToAmdPdu(pduData_p, txSduIn_p->rlcAmRawSdu.data_p,
                txSduIn_p->rlcAmRawSdu.size, pPduHdrInfo, pAvailableSize,
                txSduIn_p);

        /* Since we are adding a segmented SDU, this has to be the first sdu
         * * *        in the PDU, hence set fi bit for it */
        pPduHdrInfo->fi |= RLC_FIRST_SDU_SEGMENTED;
        /* If there is an incomplete SDU, then that is the last SDU, and
         * * * is segmented, hence set the fi bit for it */
        if (RLC_NULL != txSduIn_p->rlcAmRawSdu.data_p)
        {
            pPduHdrInfo->fi |= RLC_LAST_SDU_SEGMENTED;
        }
        else
        {
            txSduIn_p->tIDSegSduNode_p = RLC_NULL;
        }

    }
    return retVal;
}
/****************************************************************************
 * Function Name  : rlcAmdCheckAndValidate
 * Inputs         : pAmTxE - a pointer to TXAmEntity,
 *                  requestedDataSize - max size of Pdu possible,                  
 * Outputs        : pollBit
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : 1.Validate if data avaliable to encode in txQueue or unsent
 *                    incomplete SDU
 *                  2.Validate if SN falls within the transmitting window
 *                  3.Check if requested size is greater than 2 or not.
 ****************************************************************************/


static UInt32 rlcAmdCheckAndValidate( TxAMEntity* pAmTxE,
                                             UInt32 requestedDataSize,
                                             UInt8 *pollBit)
{  
    AmdRawSduIn   *txSduIn_p = &pAmTxE->txSduIn;
    UInt32        szSduIn    = txSduIn_p->rlcAmRawSdu.size;
    RlcVr         vtMS       = pAmTxE->vtMS;
    RlcVr         vtS        = pAmTxE->vtS;
    if(requestedDataSize <= 2)
    {
        LOG_RLC_MSG(LTE_RLC_REQUESTED_SIZE_LESS_THAN_FIXED_HEADER, 
        LOGWARNING, (RLC_TX|RLC_AM), globalTTITickCount_g, __LINE__,
		 pAmTxE->ueIndex, pAmTxE->lcId,requestedDataSize,0, 0,0, __func__, "");	   
          return RLC_FAILURE;
    }
    if(0 == (pAmTxE->txQueueSize + szSduIn))
    {
        LOG_RLC_MSG(LTE_RLC_NO_DATA_FOR_AM_ENTITY, LOGWARNING, (RLC_TX|RLC_AM), globalTTITickCount_g, __LINE__,
		pAmTxE->ueIndex, pAmTxE->lcId,0,0, 0,0, __func__, "");	
        return RLC_FAILURE;
    }
    if ( ! (isAMLTE (  pAmTxE->vtA, vtS, pAmTxE->vtA ) && isAMLT(  vtS, vtMS, pAmTxE->vtA )) )
    {
        /*a SN falls outside of the transmitting window */
	    LOG_RLC_MSG(LTE_RLC_SN_OUTSIDE_TX_WINDOW_FOR_ENCODING, LOGWARNING, (RLC_TX|RLC_AM), globalTTITickCount_g,
		pAmTxE->ueIndex, pAmTxE->lcId, pAmTxE->vtA, pAmTxE->vtS, pAmTxE->vtMS,0,0, __func__, "");	   
        *pollBit = 0x0F;
        /* Start 128 UE: 25% NACK Changes */
        /*
        ** To send RLC_WAIT_FOR_STATUS_PDU in BO update.
        */
        return LTE_RLC_VALIDATION_FAILED_SN_OUTSIDE_TX_WINDOW;
        /* End 128 UE: 25% NACK Changes */
    }
    return RLC_SUCCESS;
}

/*******************************************************************************
 * Function Name  : rlcAddSduToAmdPdu
 * Inputs         : pduData_p - a pointer to AMD PDU buffer,
 *                  pSdu - pointer to RlcUeDataReq,
 *                  pPduHdrInfo - pointer to structure to maintain info for
 *                                header formation,
 *                  pAvaliableSize - pointer to remaining requested size of PDU,
 *                  pAmTxE - pointer to TX AM entity
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : 1.API to add SDU to PDU
 *                  2.Update fi accordingly
 *                  3.Create Tid Node and Insert it in Tod Queue.
 *******************************************************************************/
static UInt32  rlcAddSduToAmdPdu(UInt8 *pduData_p, 
                                        RlcUeDataReq *pSdu,
                                        RlcPduHeaderInfo *pPduHdrInfo,
                                        UInt32 *pAvailableSize,
                                        TxAMEntity* pAmTxE)
{
    SduTidInfo  *sduTidInfo_p = RLC_NULL;
    SduTidInfoQ *tidQueue_p   = &pAmTxE->tidQueue;
    AmdRawSduIn *txSduIn_p    = &pAmTxE->txSduIn;
    /*Creating Tid node and inserting it in Queue */   
    CREATE_AM_SDU_TID_INFO_NODE(sduTidInfo_p);

    if(PNULL == sduTidInfo_p)
    {
        return RLC_FAILURE;
    }

    sduTidInfo_p->tId         = pSdu->transId;
    sduTidInfo_p->seqNum      = pAmTxE->vtS;
    sduTidInfo_p->tIdSnCount  = 1 ;
    if (0 == ENQUEUE_AM_SDU_TID_Q(tidQueue_p, sduTidInfo_p))
    {
        freeMemPool(sduTidInfo_p);
        return RLC_FAILURE;
    }
                            
    UInt32 retVal = rlcAddDfeToAmdPdu(pduData_p, pSdu->data_p, pSdu->size,
            pPduHdrInfo, pAvailableSize, txSduIn_p );
    
    
    /* If there is an incomplete SDU, then that is the last SDU, and is
     * *        segmented, hence set the fi bit for it */
    if (RLC_NULL != txSduIn_p->rlcAmRawSdu.data_p)
    {
        pPduHdrInfo->fi |= RLC_LAST_SDU_SEGMENTED;
        txSduIn_p->tIDSegSduNode_p = sduTidInfo_p;
#if defined(KPI_STATS) || defined(PERF_STATS)
        txSduIn_p->unsegSduSize = pSdu->size;
#endif
#if (defined KPI_STATS) && !(defined UE_SIM_TESTING)
        txSduIn_p->pktArrivalTime = pSdu->pktArrTime;
#endif
    }
    
    return retVal;
}

/****************************************************************************
 * Function Name  : rlcEncodeAmdPduHeader
 * Inputs         : pAmTxE - pointer to TX AM entity,
 *                  pPduHeader - a pointer to AMD PDU buffer,
 *                  pPduHdrInfo - pointer to structure to maintain info,
 *                  amReTxPdu_p - pointer to re-Transmission buffer
 * Outputs        : poll Bit
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : 1.API to encode header(variable and fixed) of PDU formed
 *                  2.Update Retx PDU dfe size and reTX PDU size
 *                  3.Update ReTx PDU SN and FI.
 ****************************************************************************/
static UInt32 rlcEncodeAmdPduHeader(TxAMEntity* pAmTxE,
                                           UInt8 *pPduHeader,
                                           RlcPduHeaderInfo *pPduHdrInfo,
                                           UInt8 *pollBit,
                                           AmReTransmission *amReTxPdu_p )
{
    UInt8  *basePdu_p = RLC_NULL;
    UInt32 szFH       = 2;
    UInt16 Idx        = 0;
    UInt32 szSdu      = 0;
    UInt8  eToggle    = 0;
    UInt8  e          = 0 ;
    UInt8 value       = 0; 
    RlcAmdPduFixH amdPduFixHdr; 

    if (pPduHdrInfo->numDfe == 0)
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_AMD_PDU_HEADER, LOGERROR,
                (RLC_TX|RLC_AM), globalTTITickCount_g, __LINE__,
                0, 0, 0, 0, 0, 0, __func__, "");	   
       // return LTE_RLC_NO_DATA_TO_ENCODE;
        return RLC_FAILURE;
    }

    basePdu_p = pPduHeader ;
    amReTxPdu_p->data_size = 0;
    /* forming variable header */
    if ( pPduHdrInfo->numDfe > 1)
    {
        pPduHeader += szFH;
        for ( Idx = 0 ; Idx < (pPduHdrInfo->numDfe -1); Idx ++)
        {
            e = ((pPduHdrInfo->numDfe - Idx) > 2 );
            szSdu = pPduHdrInfo->dfeLength[Idx];
            /* updating Retx data size */
            amReTxPdu_p->data_size             += szSdu;
            amReTxPdu_p->dfeInfo.sizeOfDfe[Idx] = szSdu;
            /*---*/
           
            if(eToggle ^= 1)
            {
                value = (e & 0x01U) << 7;
                value |= ((szSdu >> 4)  & 0x7FU);
                *pPduHeader++ = value;
                value = (szSdu  & 0xFU) << 4;
                *pPduHeader = value; 
            }
            else
            {
                value |= (e & 0x01U) << 3 ;
                value |= ((szSdu >> 8 ) & 0x07U);
                *pPduHeader++ = value;
                value = (szSdu & 0xFFU);
                *pPduHeader++ = value;               
            }
        }
    }
    /* updating Retx data size */
    amReTxPdu_p->data_size += pPduHdrInfo->dfeLength[pPduHdrInfo->numDfe -1];          
    amReTxPdu_p->dfeInfo.sizeOfDfe[pPduHdrInfo->numDfe -1] = pPduHdrInfo->dfeLength[pPduHdrInfo->numDfe -1];

     /* forming fixed header */
    amdPduFixHdr.DC = 1;
    amdPduFixHdr.RF = 0;
    amdPduFixHdr.FI = pPduHdrInfo->fi;
    amdPduFixHdr.E  = (pPduHdrInfo->numDfe > 1);
    amdPduFixHdr.SN = pAmTxE->vtS;
    
    /*Get the Poll bit for new transmission */
    *pollBit = amdPduFixHdr.P = 
        (0x01 & rlcPollBitForNewAmdPdu(pAmTxE,amReTxPdu_p->data_size)); 
    
    rlcAssignRlcAmdPduFixHeader(basePdu_p,&amdPduFixHdr);
    
    /* Changes for Rel 3.1 Start */ 	
    LOG_RLC_MSG(LTE_RLC_ENCODING_COMPLETE_FOR_FRESH_PDU_1, LOGDEBUG, (RLC_TX|RLC_AM), globalTTITickCount_g, 
		pAmTxE->ueIndex, pAmTxE->lcId,amdPduFixHdr.DC, 
		amdPduFixHdr.RF,amdPduFixHdr.P, 0, 0,  __func__, "");	
    
    LOG_RLC_MSG(LTE_RLC_ENCODING_COMPLETE_FOR_FRESH_PDU_2, LOGDEBUG, (RLC_TX|RLC_AM),
    globalTTITickCount_g,__LINE__ ,amdPduFixHdr.FI,amdPduFixHdr.E,
    amdPduFixHdr.SN,amReTxPdu_p->data_size, 0, 0,  __func__, "");
    
     /* udating Retx Pdu fields  */
        amReTxPdu_p->seqNum = amdPduFixHdr.SN;
        amReTxPdu_p->dfeInfo.fi = amdPduFixHdr.FI;

    return RLC_SUCCESS;
}

/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : rlcStoreAmdPduForReTx
 * Inputs         : pAmTxE - a pointer to TxAMEntity,
 *                  amReTxPdu_p - pointer to reTxPdu,
 *                  pduFormed_p - a pointer to RlcpduBuffer,
 *                  pPduHdrInfo - a pointer to RlcPduHeaderInfo
 * Outputs        : None
 * Returns        : ret - RLC_SUCCESS if successfull else RLC_FAILURE
 * Description    : Store Amd Pdu for retransmission.
 ****************************************************************************/
static UInt32 rlcStoreAmdPduForReTx(
    TxAMEntity* pAmTxE,
    AmReTransmission  *amReTxPdu_p,
    RlcPduBuffer* pduFormed_p,
    RlcPduHeaderInfo  *pPduHdrInfo)
{
    RlcVr              reTxQueueIdx   = 0;
    if (amReTxPdu_p != NULL)
    {
#if (!defined(UE_SIM_TESTING))
        amReTxPdu_p->pdu_data_p = msgClone(pduFormed_p->data_p);
#else
        amReTxPdu_p->pdu_data_p = pduFormed_p->data_p;    
#endif
        /* SPR 4171 Fix Start */
        if (RLC_NULL == amReTxPdu_p->pdu_data_p)
        {
            msgFree(pduFormed_p->data_p);
            freeMemPool(amReTxPdu_p);
            pduFormed_p->data_p = RLC_NULL;
            pduFormed_p->size = 0;
            /* SPR 7587 Changes Start */
            return LTE_RLC_ENCODING_FAIL;
            /* SPR 7587 Changes Start */
        }
        /* SPR 4171 Fix End */
        amReTxPdu_p->pdu_size = pduFormed_p->size;
        amReTxPdu_p->reTxCount = 0xFFFF;
        /*-----*/
        amReTxPdu_p->dfeInfo.dfeCount = pPduHdrInfo->numDfe;
        amReTxPdu_p->snTidCount = pPduHdrInfo->numDfe;

#ifndef UE_SIM_TESTING
        amReTxPdu_p->sduCountForPdbKPI = pduFormed_p->sduCount;
#endif
        reTxQueueIdx  = pAmTxE->vtS & 511;
        pAmTxE->reTxQueue[reTxQueueIdx] = amReTxPdu_p;
    }
    return RLC_SUCCESS; 
}
/* Cyclomatic Complexity changes - ends here */

/****************************************************************************
 * Function Name  : rlcEncodeAmdDataRawPdu
 * Inputs         : pAmTxE - a pointer to TxAMEntity,
 *                  pduFormed_p - a pointer to RlcPduBuffer,
 *                  requestedDataSize - Data size requested for PDUs,
 *                  pktArrTime - Pointer to pakcet arrival time
 * Outputs        : pollBit set
 * Returns        : ret - RLC_SUCCESS if successfull else RLC_FAILURE 
 * Description    : 1.Encodes(including segmentation and conactenation of AMD
 *                  SDU's) Amd fresh PDU into the @pduFormed_p
 *                  2.returns PDU size in dataSize
 *                  3.Updtaes Tx queueLoad.
 ****************************************************************************/
UInt32 rlcEncodeAmdDataRawPdu( TxAMEntity* pAmTxE,
                                      RlcPduBuffer* pduFormed_p,
                                      UInt32 requestedDataSize,
                                      UInt8  *pollBit,
                                      /* SPR 15909 fix start */
                                      tickType_t *pktArrTime
                                      /* SPR 15909 fix end */
                                      )
{
    AmdRawSduQ        *amdRawSduQ_p   = &pAmTxE->txSDUQueue;       
    AmdRawSduIn       *txSduIn_p      = &pAmTxE->txSduIn;
    RlcAmBuffer       *rlcAmBuffer_p  = RLC_NULL;
    AmReTransmission  *amReTxPdu_p    = RLC_NULL; 
    RlcUeDataReq      *pSdu           = RLC_NULL;
    UInt8             *pPduHeader     = RLC_NULL;
    UInt32             retVal         = RLC_SUCCESS;
    UInt32             availableSize  = requestedDataSize;
    /* SPR 4547 Changes Start */
    RlcAmBuffer       rlcAmBuffer = {0};

    /* SPR 4547 Changes End */
	/* + PERF_CA GBL */
    InternalCellIndex cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(pAmTxE->ueIndex);    
	/* - PERF_CA GBL */
    /* Coverity 54505*/
    RlcPduHeaderInfo  *pPduHdrInfo    = &pduHdrInfo_g[cellIndex];
    /* Coverity 54505*/
#ifndef UE_SIM_TESTING
    UInt8              sduCount       = 0;
#endif
    /* KPI Changes Start */
#if defined(KPI_STATS) || defined(PERF_STATS)
    UInt32 pdcpHdrSize = 2;
#endif
    /* KPI Changes End */
    /* SPR 13888 fix start */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    UInt8 commonIndex = 0;
    UInt16 lcId = 0;
    UInt16 ueIndex = 0;
#endif
    /* SPR 13888 fix end */
    UInt32 sQCount = QUEUE_COUNT_AMD_RAW_SDU_Q(*amdRawSduQ_p);
    if( !sQCount && pAmTxE->txSduIn.rlcAmRawSdu.size == 0)
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_DATA_PDU_ERR, LOGWARNING, (RLC_TX|RLC_AM),
                globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                pAmTxE->lcId, pAmTxE->txSduIn.rlcAmRawSdu.size, RLC_FAILURE, 0, 0, __func__, "");
         pAmTxE->txQueueSize = 0; 
        /* SPR 7587 Changes Start */
        return LTE_RLC_NO_DATA_TO_ENCODE;
        /* SPR 7587 Changes End */

    }
    /* validates wether to continue or break */
    /* Start 128 UE: 25% NACK Changes */
    if( RLC_SUCCESS != (retVal = rlcAmdCheckAndValidate(pAmTxE, requestedDataSize, pollBit))) 
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_DATA_PDU_ERR_1, LOGWARNING, (RLC_TX|RLC_AM),
                globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                pAmTxE->lcId, RLC_FAILURE, 0, 0, 0, __func__, "");
        return retVal;
    }
    /* End 128 UE: 25% NACK Changes */
    /* Allocate Structure for storing header information */
    /* Coverity 54505*/
    //Initialize PduHeaderInfo
    pPduHdrInfo->hdrSize = 2; /* Fixed header size */
    pPduHdrInfo->fi      = 0;
    pPduHdrInfo->numDfe  = 0;
    availableSize       -= pPduHdrInfo->hdrSize;

    /* Coverity 54505*/
    /* +Coverity 11053*/
    rlcAmBuffer_p = (RlcAmBuffer *)(&rlcAmBuffer);
    /* -Coverity 11053*/
    if( (rlcAmBuffer_p->data_p = msgAlloc(zeroPool_g,0,0,0)) == RLC_NULL)
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_DATA_PDU_ERR_4, LOGWARNING, (RLC_TX|RLC_AM),
                globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                pAmTxE->lcId, LTE_RLC_MEM_ALLOC_FAIL, 0, 0, 0, __func__, "");
        return LTE_RLC_MEM_ALLOC_FAIL; 
    }

    /*---- Creating  ReTx PDU----- */
    if((amReTxPdu_p = (AmReTransmission *)
                getMemFromPool(sizeof(AmReTransmission),RLC_NULL)) == RLC_NULL)
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_DATA_PDU_ERR_5, LOGWARNING, (RLC_TX|RLC_AM),
                globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                pAmTxE->lcId, LTE_RLC_MEM_ALLOC_FAIL, 0, 0, 0, __func__, "");
        msgFree(rlcAmBuffer_p->data_p);
        /* SPR 20985 fix start */
        rlcAmBuffer_p->data_p = RLC_NULL;
        /* SPR 20985 fix end */
        return LTE_RLC_MEM_ALLOC_FAIL;
    }
    /*--- Encoding the SDU segment first --*/
    /* +- SPR 17777 */
    if (RLC_SUCCESS != (retVal = rlcAddSduSegmentToAmdPdu(rlcAmBuffer_p->data_p,
                    pPduHdrInfo, &availableSize, txSduIn_p ) ) )
    /* +- SPR 17777 */
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_DATA_PDU_ERR_6, LOGWARNING, (RLC_TX|RLC_AM),
                globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                pAmTxE->lcId, LTE_RLC_ENCODING_SDU_SEG_FAIL, 0, 0, 0, __func__, "");

        /* SPR 4547 Changes Start */
        msgFree(rlcAmBuffer_p->data_p);
        /* SPR 20985 fix start */
        rlcAmBuffer_p->data_p = RLC_NULL;
        /* SPR 20985 fix end */
        /* SPR 4547 Changes End */
        freeMemPool(amReTxPdu_p);
        return LTE_RLC_ENCODING_SDU_SEG_FAIL;
    }

    /* SPR 22517 Fix + */ 
    /* Code Removed */
    /* SPR 22517 Fix - */ 

#if defined(KPI_STATS) || defined(PERF_STATS)
    if( pPduHdrInfo->fi == 0x02 )
    {
        pduFormed_p->pdcpSduSize[pduFormed_p->completeEncodedSduCount] =\
                                                                        (txSduIn_p->unsegSduSize - pdcpHdrSize );
        pduFormed_p->completeEncodedSduCount++;
    }
#endif
    /* KPI Changes End */

    /* Encode all enqueued SDUs */
    /** SPR 3823 Changes Start **/
    /* + SPR 12349 Changes */
    while (((availableSize > 2) || (!pPduHdrInfo->numDfe && availableSize )) && ((SInt32)pAmTxE->txQueueSize > 0)) //if avaliable size less than 2 do not dequeue 
    /* - SPR 12349 Changes */
        /** SPR 3823 Changes End **/
    {
        pSdu = PNULL;
        
        /* SPR 17554 Start */
        if(0==amdRawSduQ_p->enqueueCount)
        {
           // fprintf(stderr,"\n******* Operation Ongoing on same Node. Leave for now!!********\n");
            break;
        } 
        DEQUEUE_TX_AMD_RAW_SDU_Q (pSdu ,amdRawSduQ_p);
        /* Break out of loop if SDU dequeue fails */
        if(PNULL == pSdu)
        {
            /* free reTx queue */
            break;
        }
        /* SPR 17554 End */

        /* SPR 13888 fix start */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        lcId = pSdu->lcId;
        ueIndex = pSdu->ueIndex;
        /* Updating pdcpTick for a packet with its pktArrTime, 
         * ie - the time when this packet is received at pdcp */
        if(lcId >2)
        {
            commonIndex = scheduledIPThroughputDL_g[ueIndex][lcId - 3].commonIndex;
            if(0 == scheduledIPThroughputDL_g[ueIndex][lcId - 3].pdcpTick[commonIndex])
            {
                scheduledIPThroughputDL_g[ueIndex][lcId - 3].pdcpTick[commonIndex] =  pSdu->pktArrTime;
            }
        }
#endif
        /* SPR 13888 fix end */

        /* updating queueload */
        //Qsize should be set so that poll cond is met while setting header
        atomicSubtractInt(&pAmTxE->txQueueSize,(pSdu->size));
		/* + PERF_CA GBL */
        lteRlcStatInfo_g[cellIndex].lteAMStat.amdSduTransmitedToMac++;
		/* - PERF_CA GBL */
        
        /* SPR 17554 Start */
        atomicSubtractInt(&amdRawSduQ_p->enqueueCount, 1);
        /* SPR 17554 End */
        
        /* Join SDU to PDU */
        if (RLC_SUCCESS != (retVal = rlcAddSduToAmdPdu(rlcAmBuffer_p->data_p, pSdu,
                        pPduHdrInfo, &availableSize, pAmTxE) ) )
        {
            /* Even if encoding fails, we will try to encode next SDU, so
               do not return here, continue to next SDU */
            /* delete from retx queue and Tid queue */
            LOG_RLC_MSG( LTE_RLC_ENCODE_DATA_PDU_ERR_7, LOGWARNING, (RLC_TX|RLC_AM),
                    globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                    pAmTxE->lcId, 0, 0, 0, 0, __func__, "");
            /* SPR 3952 Fix start */
            msgFree(pSdu->data_p);
            pSdu->data_p = RLC_NULL;
            /* SPR 3952 Fix End */
        }
        else
        {
#ifndef UE_SIM_TESTING
            if((sduCount < pPduHdrInfo->numDfe) && (sduCount < MAX_RLC_SDU_IN_PDU_FOR_KPI))
            {
                pktArrTime[sduCount] = pSdu->pktArrTime;
                amReTxPdu_p->pktArrTime[sduCount] = pSdu->pktArrTime;
                sduCount++;
            }
#endif
            /* KPI Changes Start */
#if defined(KPI_STATS) || defined(PERF_STATS)
            if(pPduHdrInfo->fi == 0x00 || pPduHdrInfo->fi == 0x02)
            {
                pduFormed_p->pdcpSduSize[pduFormed_p->completeEncodedSduCount] =\
                                                                                (pSdu->size - pdcpHdrSize );
                pduFormed_p->completeEncodedSduCount++;
            }
#endif
            /* KPI Changes End */
        }
        freeMemPool(pSdu);
        if (pPduHdrInfo->numDfe >= RLC_MAX_DFE_IN_PDU)//check lower bound(-1)?
        {
            LOG_RLC_MSG( LTE_RLC_ENCODE_DATA_PDU_ERR_8, LOGWARNING, (RLC_TX|RLC_AM),
                    globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                    pAmTxE->lcId, pPduHdrInfo->numDfe , 0, 0, 0, __func__, "");

            break;
        }
        /* Cyclomatic Complexity changes - starts here */
        if((pPduHdrInfo->numDfe > 0)&&
                (pPduHdrInfo->dfeLength[pPduHdrInfo->numDfe - 1] > 
                 MAX_DATA_FIELD_SIZE))
        {
            /* last dfe added was greater than 2047, LI field cannot contain
               this value, hence it should be the last DFE in PDU */
            break;
        }
    }
    /*----- Encode header----- */
    /* Reserve memory for header */
    if ( (pPduHeader = msgReserve(rlcAmBuffer_p->data_p, 0, pPduHdrInfo->hdrSize) )
            == RLC_NULL)
    {
        msgFree(rlcAmBuffer_p->data_p);
        /* SPR 20985 fix start */
        rlcAmBuffer_p->data_p = RLC_NULL;
        /* SPR 20985 fix end */
        freeMemPool(amReTxPdu_p);

        LOG_RLC_MSG(LTE_RLC_MEM_ALLOC_FAILURE, LOGFATAL, L2_SYS_FAIL, globalTTITickCount_g, __LINE__,
                0, 0,0,0, 0,0, __func__, "");
        return LTE_RLC_MSG_RESERVE_FAIL;
    }
    memSet(pPduHeader, 0,  pPduHdrInfo->hdrSize);
    if ( (retVal = rlcEncodeAmdPduHeader(pAmTxE, pPduHeader, pPduHdrInfo, pollBit, amReTxPdu_p) ) == RLC_FAILURE)
    {
        msgFree(rlcAmBuffer_p->data_p);
        /* SPR 20985 fix start */
        rlcAmBuffer_p->data_p = RLC_NULL;
        /* SPR 20985 fix end */
        freeMemPool(amReTxPdu_p);
        return retVal;
    }
    pduFormed_p->size = amReTxPdu_p->data_size + pPduHdrInfo->hdrSize;
    pduFormed_p->data_p = rlcAmBuffer_p->data_p;
    pduFormed_p->seqNum = pAmTxE->vtS;
    pduFormed_p->payLoadSize = amReTxPdu_p->data_size;

    rlcAmBuffer_p->size = amReTxPdu_p->data_size;
    /*updating for QOS */
    if( amdRawSduQ_p->pQ->count > 0)
    {
        pAmTxE->headPktTS   =  ((RlcUeDataReq *)(amdRawSduQ_p->pQ->ring[amdRawSduQ_p->pQ->head].data))->pktArrTime;
    }
    pduFormed_p->headPktTS = pAmTxE->headPktTS;
#ifndef UE_SIM_TESTING
    pduFormed_p->sduCount = sduCount ;
#endif

    /*  updating ReTx Pdu fields */
    if(RLC_SUCCESS != (rlcStoreAmdPduForReTx(pAmTxE,amReTxPdu_p,pduFormed_p,pPduHdrInfo)))
    {
        return RLC_FAILURE;
    }

    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : rlcAssignRlcAmdPduFixHeader
 * Inputs         : base_data_p - a pointer to Base data,
 *                  amdPduHdr_p - a pointer to RlcAmdPduFixHeader
 * Outputs        : None
 * Returns        : RLC_SUCCESS  after successfull assignments
 * Description    : assign the values to elements in AmdPduFixHeader.
 ****************************************************************************/
UInt32 rlcAssignRlcAmdPduFixHeader (UInt8 *base_data_p,RlcAmdPduFixH *amdPduHdr_p)
{
    SET_BYTE_FIELD(base_data_p[0],amdPduHdr_p->DC,7,1); 
    SET_BYTE_FIELD(base_data_p[0],amdPduHdr_p->RF,6,1); 
    SET_BYTE_FIELD(base_data_p[0],amdPduHdr_p->P,5,1); 
    SET_BYTE_FIELD(base_data_p[0],amdPduHdr_p->FI,3,2); 
    SET_BYTE_FIELD(base_data_p[0],amdPduHdr_p->E,2,1);
    SET_BYTE_FIELD(base_data_p[0],amdPduHdr_p->SN>>8,0,2);
    SET_BYTE_FIELD(base_data_p[1],amdPduHdr_p->SN,0,8);
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : rlcCheckAmPduReTxCount
 * Inputs         : pAmTxE - a pointer to TxAMEntity,
 *                  amReTxPdu_p - a pointer to AmReTransmission,
 *                  pduFormed_p - a pointer to RlcPduBuffer,
 *                  pktArrTime - Pointer to pakcet arrival time
 * Outputs        : None
 * Returns        : ret - RLC_SUCCESS if successfull else RLC_FAILURE 
 * Description    : This functin checks the Re transmissioin count and send
 *                  the threshold indication to RRC 
 ****************************************************************************/
static UInt32 rlcCheckAmPduReTxCount(
        TxAMEntity* pAmTxE,
        AmReTransmission * amReTxPdu_p,
        RlcPduBuffer* pduFormed_p)
{
    if(0xFFFF == amReTxPdu_p->reTxCount) 
    {
        amReTxPdu_p->reTxCount = 0;
    }
    else if(amReTxPdu_p->reTxCount < pAmTxE->maxRetxThreshold) 
    {
        amReTxPdu_p->reTxCount++;
        gRlcStats.pRlcStats->lteRlcUeStat[pAmTxE->ueIndex].
            lcStats[pAmTxE->lcId].numRetx ++;
    }
    /** SPR 3717 Changes Start **/
    if (amReTxPdu_p->reTxCount >= pAmTxE->maxRetxThreshold)
    {
        /*Coverity 96609_97070 fix start*/
        if(amReTxPdu_p->reTxCount == pAmTxE->maxRetxThreshold)
        {
#if  defined(PERF_STATS) && !defined(UE_SIM_TESTING)

            gRlcUePerfStats_p[pAmTxE->ueIndex].\
                lteUeRlcDLPerfStats.totalMaxRetxExceeded++;
#endif
            rlcSendMaxReTxThresholdIndToRrc(pAmTxE);
            /* SPR 16843 start */
#if  defined(KPI_STATS) && !defined(UE_SIM_TESTING)
/*spr 19296 fix start*/ 
                UInt8 qci = pAmTxE->qci;
                InternalCellIndex cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(pAmTxE->ueIndex);
                UInt32 activeIndexForMacStats = (gMacStats.
                        pStats->lteCommonKPIStats[cellIndex].activeIndexForMacStats);

                LteMacKPIStats * pLteMacKPIStats = &(gMacStats.
                        pStats->lteCommonKPIStats[cellIndex].lteMacKPIStats[activeIndexForMacStats]);
                if (0 < qci && MAX_QCI >= qci )
                {
                    pLteMacKPIStats->totalDlDrbUuLossRatePerQci[--qci] +=
                        amReTxPdu_p->sduCountForPdbKPI;
                }
/*spr 19296 fix end*/
#endif
            /* SPR 16843 end */
        }
        amReTxPdu_p->reTxCount++;
        /** SPR 3717 Changes End **/
        if(pAmTxE->txPduIn.pRawPdu.data_p)
        {
            msgFree(pAmTxE->txPduIn.pRawPdu.data_p);
            pAmTxE->txPduIn.pRawPdu.data_p = RLC_NULL;
        }
        /* SPR 5303 Start */
        /* SPR 5160 Start */
        if(!pduFormed_p->data_p )
        {
            LOG_RLC_MSG( LTE_RLC_ENCODE_RETX_PDU_INFO, LOGDEBUG, (RLC_TX|RLC_AM),
                    globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                    pAmTxE->lcId,amReTxPdu_p->seqNum,amReTxPdu_p->reTxCount, 0, 0, __func__, "");

            return RLC_FAILURE;
        }
        /* SPR 5160 End */
        /* SPR 5303 End */

        LOG_RLC_MSG(LTE_RLC_ENCODE_RETX_PDU_INFO, LOGDEBUG, (RLC_TX|RLC_AM),
                globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                pAmTxE->lcId,amReTxPdu_p->seqNum,amReTxPdu_p->reTxCount, 0, 0, __func__, "");
        /* + SPR_16046 */
        /* SPR 19300 fix ++ */
        if(((amReTxPdu_p->reTxCount - 1)==(UInt16)pAmTxE->maxRetxThreshold)
                &&(pAmTxE->lcId < MIN_DRB_LC_ID))
            /* SPR 19300 fix -- */
        {
             /* cov 109091 Fix + */
             if (RLC_FAILURE == rlcSendMaxReTxThresholdInd(pAmTxE,amReTxPdu_p))
             {
                 return RLC_FAILURE;
             }
             /* cov 109091 Fix - */
        }
        /* - SPR_16046 */
        /*Coverity 96609_97070 fix end*/
    }

    return RLC_SUCCESS;
}
/* Cyclomatic Complexity changes - ends here */

/*
Reference 3GPP T.S 36.322 Section 5.2.1 Retransmission

When receiving a negative acknowledgement for an AMD PDU 
or a portion of an AMD PDU by a STATUS PDU from its peer 
AM RLC entity, the transmitting side of the AM RLC entity shall:

-   if the SN of the corresponding AMD PDU falls within the range VT(A) <= SN < VT(S):
-   consider the AMD PDU or the portion of the AMD PDU for 
    which a negative acknowledgement was received for retransmission.
    
When an AMD PDU or a portion of an AMD PDU is considered for retransmission, 
      the transmitting side of the AM RLC entity shall:
-   if the AMD PDU is considered for retransmission for the first time: 

-   set the RETX_COUNT associated with the AMD PDU to zero;

-   else, if it (the AMD PDU or the portion of the 
    AMD PDU that is considered for retransmission) 
    is not pending for retransmission already,
    or a portion of it is not pending for retransmission already:

-   increment the RETX_COUNT;
-   if RETX_COUNT = maxRetxThreshold:
-   indicate to upper layers that max retransmission has been reached.

When retransmitting an AMD PDU, the transmitting side of an AM RLC entity shall:
-   if the AMD PDU can entirely fit within the total size of RLC PDU(s)
    indicated by lower layer at the particular transmission opportunity:
-   deliver the AMD PDU as it is except for the P field
    (the P field should be set according to sub clause 5.2.2) to lower layer;
-   otherwise:
-   segment the AMD PDU, form a new AMD PDU segment 
    which will fit within the total size of RLC PDU(s) 
    indicated by lower layer at the particular transmission opportunity 
    and deliver the new AMD PDU segment to lower layer.
    
When retransmitting a portion of an AMD PDU, the transmitting side of an AM RLC entity shall:

-   segment the portion of the AMD PDU as necessary, 
   form a new AMD PDU segment which will fit within the total size of RLC PDU(s) 
   indicated by lower layer at the particular transmission opportunity 
   and deliver the new AMD PDU segment to lower layer.
   
When forming a new AMD PDU segment, the transmitting side of an AM RLC entity shall:
-   only map the Data field of the original AMD PDU to the Data field of the new AMD PDU segment;
-   set the header of the new AMD PDU segment in accordance with the description in sub clause 6.;
-   set the P field according to sub clause 5.2.2.


*/
/****************************************************************************
 * Function Name  : rlcEncodeAmdReTxPdu
 * Inputs         : pAmTxE - a pointer to TxAMEntity,
 *                  pduFormed_p - a pointer to RlcAmBuffer,
 *                  requestedDataSize - Control Pdu size,
 *                  pollBit,
 *                  rlcHarqFailureInfo_p - a pointer to RLCHarqFailureInfo,
 *                  pktArrTime - Packet arrival time
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : Encode Amd Retransmission Pdu.
 ****************************************************************************/

UInt32 rlcEncodeAmdReTxPdu( TxAMEntity* pAmTxE, 
                            RlcPduBuffer* pduFormed_p,
                            UInt32 requestedDataSize,
                            UInt8 *pollBit, 
                            RLCHarqFailureInfo* rlcHarqFailureInfo_p,
                            /* SPR 15909 fix start */
                            tickType_t *pktArrTime
                            /* SPR 15909 fix end */
                          )
{
    AmNackListQ* nackListQ_p = &pAmTxE->nackList;
    AmNackListNode* amNackList_p = RLC_NULL;
    RlcSn seqNum = 0;
    AmReTransmission * amReTxPdu_p = RLC_NULL;
    UInt32 szFH = 2;
    UInt8 segmentedPduType = 0;
#ifndef UE_SIM_TESTING
    UInt8 sduCount = 0;
#endif
    UInt32 ret = RLC_SUCCESS; 

    /* requested Data Size must greater than fixed size*/
    if(requestedDataSize <= szFH)
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_RETX_PDU_ERR, LOGWARNING, (RLC_TX|RLC_AM),
                globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                pAmTxE->lcId, requestedDataSize, RLC_FAILURE, 0, 0, __func__, "");
        return RLC_FAILURE;
    }

    if(pAmTxE->txPduIn.pRawPdu.size)
    {

        /* When retransmitting a portion of an AMD PDU */
        /* segment the portion of the AMD PDU as necessary, form a new AMD
           PDU segment which will fit within the total size of RLC PDU(s)
           indicated by lower layer at the particular transmission 
           opportunity and deliver the new AMD PDU segment to lower layer.
           */

        /* When forming a new AMD PDU segment 
           -   only map the Data field of the original AMD PDU to the Data
           field of the new AMD PDU segment;
           -   set the header of the new AMD PDU segment in accordance with
           the description in sub clause 6.;
           -   set the P field according to sub clause 3.2.1.5.2
           */
        /* Setting value for Incomplete Pdu */
        segmentedPduType = 1;
        seqNum = GET_RETX_QUEUE_INDEX_FROM_SN(pAmTxE->txPduIn.sn);

        amReTxPdu_p = pAmTxE->reTxQueue[seqNum];
        /**SPR 1330 START **/
        if (!amReTxPdu_p)
        {
            LOG_RLC_MSG( LTE_RLC_ENCODE_RETX_PDU_ERR_1, LOGWARNING, (RLC_TX|RLC_AM),
                    globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                    pAmTxE->lcId, seqNum, RLC_FAILURE, 0, 0, __func__, "");

            pAmTxE->txPduIn.pRawPdu.size = 0;  
            return RLC_FAILURE;
        } 
        /**SPR 1330 END **/
        ret =  rlcEncodeAmdPduSegment(pAmTxE, pduFormed_p, requestedDataSize, 
                amReTxPdu_p, pollBit, amNackList_p, segmentedPduType,
                rlcHarqFailureInfo_p);
        if(ret != RLC_SUCCESS)
        {
            return ret;
        }
        pduFormed_p->seqNum = pAmTxE->txPduIn.sn; 

    }    
    else
    {
        /*NACK list is formed using STATUS PDU received from peer RLC.
          NACK list already having AMD pdu sn that falls with in range 
          VT(A) <= SN < VT(S)*/
        /*Remove the node from the NACK List*/
        UInt8 *pduData_p = NULL;
        UInt16 dataSize = 0;
        /* SPR 20708 Changes Start */
        /*
        ** requestedDataSize or txOpp can be equal or less than to 4 bytes if Complete PDU needs
        ** to be retransmitted (as AMD PDU fixed header size is 2 bytes). Also, SEGMENTED Header
        ** Size (4 bytes) check is already present in rlcEncodeAmdPduSegment() while encoding
        ** segmented PDU.
        */
        /* SPR 20708 Changes End */
        DEQUEUE_AMD_NACK_LIST_Q(amNackList_p,nackListQ_p);
        if(RLC_NULL == amNackList_p)
        {
            LOG_RLC_MSG( LTE_RLC_ENCODE_RETX_PDU_ERR_8, LOGWARNING, (RLC_TX|RLC_AM),
                    globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                    pAmTxE->lcId, RLC_FAILURE, 0, 0, 0, __func__, "");
            return RLC_FAILURE;
        }

        pduFormed_p->seqNum = amNackList_p->seqNum; 

        seqNum = GET_RETX_QUEUE_INDEX_FROM_SN(amNackList_p->seqNum);
        amReTxPdu_p = pAmTxE->reTxQueue[seqNum];
        if(RLC_NULL == amReTxPdu_p)
        {
            LOG_RLC_MSG( LTE_RLC_ENCODE_RETX_PDU_ERR_9, LOGWARNING, (RLC_TX|RLC_AM),
                    globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                    pAmTxE->lcId, seqNum ,RLC_FAILURE, 0, 0, __func__, "");
            /** SPR 4171 Fix start **/
            freeMemPool(amNackList_p);
            amNackList_p = RLC_NULL;
            /** SPR 4171 Fix End **/
            return RLC_FAILURE;
        }
        if((0 == amNackList_p->Sostart) && (0 == amNackList_p->Soend))
        {
            /*if the AMD PDU can entirely fit within the total size of RLC PDU(s) 
              indicated by lower layer at the particular transmission opportunity
              Deliver the AMD PDU as it is except for the P field to lower layer */
            if(requestedDataSize >= amReTxPdu_p->pdu_size)
            {
                QSEGMENT segment = {0};
                pduData_p = msgClone(amReTxPdu_p->pdu_data_p);
                /* SPR 4171 Fix start */
                if (RLC_NULL == pduData_p)
                {
                    if (amNackList_p)
                    {
                        /* Cyclomatic Complexity changes - starts here */
                        freeMemPool(amNackList_p);
                        amNackList_p = RLC_NULL;
                        /* Cyclomatic Complexity changes - ends here */
                    }
                    return RLC_FAILURE;
                }
                /* SPR 4171 Fix End */
                dataSize = amReTxPdu_p->pdu_size;
                amReTxPdu_p = pAmTxE->reTxQueue[seqNum];
                if(RLC_NULL == amReTxPdu_p)
                {
                    msgFree(pduData_p);
                    /* SPR 20985 fix start */
                    pduData_p = RLC_NULL;        
                    /* SPR 20985 fix end */
                    /* SPR 4171 Fix start */
                    if (amNackList_p)
                    {
                        freeMemPool(amNackList_p);
                        amNackList_p = RLC_NULL;
                    }
                    /* SPR 4171 Fix End */
                    return RLC_FAILURE;
                }
                msgSegNext(pduData_p, 0, &segment);
                UInt8* base_data_p  = segment.base;
                if (PNULL == base_data_p) 
                {
                    /* SPR 4171 Fix start */
                    //ltePanic("%s: can't allocate buffer",__FUNCTION__);
                    msgFree(pduData_p);
                    /* SPR 20985 fix start */
                    pduData_p = RLC_NULL;        
                    /* SPR 20985 fix end */
                    if (amNackList_p)
                    {
                        /* Cyclomatic Complexity changes - starts here */
                        freeMemPool(amNackList_p);
                        amNackList_p = RLC_NULL;
                        /* Cyclomatic Complexity changes - ends here */
                    }
                    /* SPR 4171 Fix End */
                    return RLC_FAILURE;
                }
                pduFormed_p->data_p    = pduData_p; 
                pduFormed_p->size      = dataSize;
                atomicSubtractInt(&pAmTxE->reTxQueueSize,(dataSize - AM_PDU_SEG_FIX_HDR_SIZE));
                *pollBit = (0x01 & rlcPollBitForReTxAmdPdu(pAmTxE));
                SET_BYTE_FIELD(base_data_p[0], *pollBit, 5, 1);
                /** SPR 3657 Changes Start **/ 
                rlcHarqFailureInfo_p->harqFailReTxInfo[rlcHarqFailureInfo_p->reTxPduCount].
                    retxPduSeqNum = pduFormed_p->seqNum;
                rlcHarqFailureInfo_p->harqFailReTxInfo[rlcHarqFailureInfo_p->reTxPduCount].
                    SOStart = 0;
                rlcHarqFailureInfo_p->harqFailReTxInfo[rlcHarqFailureInfo_p->reTxPduCount].
                    SOEnd   = 0;
                rlcHarqFailureInfo_p->reTxPduCount++;
                /** SPR 3657 Changes End **/
                rlcHarqFailureInfo_p->harqFailureReporting
                    = REPORTING_TRUE;

            }
            else
            {
                /* segment the AMD PDU, form a new AMD PDU segment which will fit
                   within the total size of RLC PDU(s) indicated by lower layer at the
                   particular transmission opportunity and deliver the new AMD PDU
                   segment to lower layer */
                ret = rlcEncodeAmdPduSegment(pAmTxE, pduFormed_p, requestedDataSize,
                        amReTxPdu_p, pollBit, amNackList_p, segmentedPduType,
                        rlcHarqFailureInfo_p);					    
                if(ret != RLC_SUCCESS)
                {
                    /** SPR 4171 Fix start **/
                    freeMemPool(amNackList_p);
                    amNackList_p = RLC_NULL;
                    /** SPR 4171 Fix End **/
                    return ret;
                }
            }
        }
        else 
        {
            ret = rlcEncodeAmdPduSegment(pAmTxE, pduFormed_p, requestedDataSize,
                    amReTxPdu_p, pollBit, amNackList_p, segmentedPduType,
                    rlcHarqFailureInfo_p);
            if(ret != RLC_SUCCESS)
            {
                /** SPR 4171 Fix start **/
                freeMemPool(amNackList_p);
                amNackList_p = RLC_NULL;
                /** SPR 4171 Fix End **/
                return ret;
            }

        }

        freeMemPool(amNackList_p);
        /* SPR 4171 Fix start */
        amNackList_p = RLC_NULL;
        /* SPR 4171 Fix End */
    }
    
    /* coverity 97034 fix ++ */
    /** Rel 4.0 changes start **/
#ifndef UE_SIM_TESTING
    pduFormed_p->sduCount =  amReTxPdu_p->sduCountForPdbKPI;
    for(sduCount = 0; sduCount < pduFormed_p->sduCount; sduCount++)
    {
        pktArrTime[sduCount]=amReTxPdu_p->pktArrTime[sduCount];
    }
#endif
    /** Rel 4.0 changes end **/
    /* coverity 97034 fix -- */
    /* Cyclomatic Complexity changes - starts here */
    if(!pAmTxE->txPduIn.pRawPdu.size)
    {
        /* +- SPR 17777 */
        ret = rlcCheckAmPduReTxCount(pAmTxE,amReTxPdu_p,pduFormed_p);
        /* +- SPR 17777 */
    }
    return ret;
    /* Cyclomatic Complexity changes - ends here */
}

/* Rlc Rel 2.0 change start */
/****************************************************************************
 * Function Name  : rlcInitAmdPduSegmentFixHeader
 * Inputs         : amdPduSegHdr_p - a pointer to RlcAmdPduSegmentFixH,
 *		            seqNum - sequence Number of AMD PDU
 * Outputs        : None
 * Returns        : 0 after successfull initialization
 * Description    : initialize the AmdPduFixHeader elements.
 ****************************************************************************/

UInt32 rlcInitAmdPduSegmentFixHeader(RlcAmdPduSegmentFixH *amdPduSegHdr_p, 
	              RlcSn seqNum)
{
    amdPduSegHdr_p->DC = 0x01;
    amdPduSegHdr_p->RF = 0x01;
    amdPduSegHdr_p->FI = 0x00;
    amdPduSegHdr_p->P = 0x00;
    amdPduSegHdr_p->E = 0x00;
    amdPduSegHdr_p->LSF = 0x00;
    amdPduSegHdr_p->SN = seqNum;
    amdPduSegHdr_p->SO = 0;
    return 0;
}



/****************************************************************************
 * Function Name  : rlcGetAmPduSegVarHdrInfo
 * Inputs         : pAmTxE - a pointer to TxAMEntity,
 *                  amReTxPdu_p - a pointer to AmReTransmission,
 *                  requestedDataSize - Pdu size
 * Outputs        : rlcAmdPduSegInfo_p - a pointer to RlcAmdPduSegInfo
 * Returns        : RLC_SUCCESS  after successfull assignments
 * Description    : This functio updates the RLC AM PDU segment information 
 ****************************************************************************/
UInt32 rlcGetAmPduSegVarHdrInfo( TxAMEntity* pAmTxE,
                                        AmReTransmission *amReTxPdu_p,
                                        RlcAmdPduSegInfo *rlcAmdPduSegInfo_p,
                                        UInt32 requestedDataSize)
{
    UInt32 ret  = RLC_SUCCESS;
    UInt32 Idx  = 0;
    UInt32 cntDFE = amReTxPdu_p->dfeInfo.dfeCount;
    UInt32 szSdu  = 0;
    UInt32 segmtLen = 0;
    UInt32 sduToBeSegmented = RLC_FALSE;
    UInt32 dfeEnd = 0;
    UInt32 sizeOfDfe = 0;
    UInt8 soStartValid = RLC_FALSE;

    /** setting MSB FI **/
    if (!(rlcAmdPduSegInfo_p->soStart))
    {
        rlcAmdPduSegInfo_p->fi = amReTxPdu_p->dfeInfo.fi & RLC_FIRST_SDU_SEGMENTED;
    }
    else
    {
        while((Idx < cntDFE) && (szSdu < rlcAmdPduSegInfo_p->soEnd))
        {
            szSdu += amReTxPdu_p->dfeInfo.sizeOfDfe[Idx];
            /** Get Size of First LI and MSB Fi **/
            if(rlcAmdPduSegInfo_p->soStart < szSdu)
            {
                /*Case when it is not start of SDU */
                 rlcAmdPduSegInfo_p->fi = RLC_FIRST_SDU_SEGMENTED;
                 rlcAmdPduSegInfo_p->numDfe += 1;
                 if(rlcAmdPduSegInfo_p->soEnd < (szSdu - 1)) 
                 {
                     rlcAmdPduSegInfo_p->fi |= RLC_LAST_SDU_SEGMENTED;
                     rlcAmdPduSegInfo_p->rawDataSize =
                         (rlcAmdPduSegInfo_p->soEnd - rlcAmdPduSegInfo_p->soStart +1);
                 }
                 else
                 {
                     /* SPR 4308 Fix Start */
                     //rlcAmdPduSegInfo_p->fi |= (amReTxPdu_p->dfeInfo.fi & RLC_LAST_SDU_SEGMENTED); 
                     /* SPR 4308 Fix End */
                     rlcAmdPduSegInfo_p->rawDataSize = szSdu - rlcAmdPduSegInfo_p->soStart;
                 }
                 if(requestedDataSize < (rlcAmdPduSegInfo_p->rawDataSize + 
                                         rlcAmdPduSegInfo_p->SegHdrSize))
                 /* SPR 4308 Fix Start */
                 {
                     rlcAmdPduSegInfo_p->fi |= RLC_LAST_SDU_SEGMENTED;
                     rlcAmdPduSegInfo_p->rawDataSize =
                     (requestedDataSize - rlcAmdPduSegInfo_p->SegHdrSize);
                 }
                 /* SPR 4308 Fix End */
                 rlcAmdPduSegInfo_p->dfeLength[rlcAmdPduSegInfo_p->numDfe - 1] =
                 rlcAmdPduSegInfo_p->rawDataSize;
                 Idx++;
                 soStartValid = RLC_TRUE;
                 break;
            }
            else if(rlcAmdPduSegInfo_p->soStart == szSdu)
            {
                /*Case when it is start of SDU */
                Idx++;
                soStartValid = RLC_TRUE;
                break;
            }
            Idx++;
        }
         /* Case when SoStart is beyond the total size of PDU*/
        if(RLC_FALSE == soStartValid)
        {
            LOG_RLC_MSG( LTE_RLC_ENCODE_RETX_PDU_ERR_4, LOGWARNING, (RLC_TX|RLC_AM),
                    globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                    pAmTxE->lcId, RLC_FAILURE, 0, 0, 0, __func__, "");
            return RLC_FAILURE;
        }
    }
    /* This Loop calculate the size of header(szVH) and data field(szRawPduSeg) */
    for(;((Idx < cntDFE) && (requestedDataSize > (rlcAmdPduSegInfo_p->rawDataSize 
          + rlcAmdPduSegInfo_p->SegHdrSize + 2))&& (szSdu < (rlcAmdPduSegInfo_p->soEnd + 1)));Idx++)
    {
        rlcAmdPduSegInfo_p->numDfe += 1;
        if(rlcAmdPduSegInfo_p->numDfe > 1)
        {
            rlcAmdPduSegInfo_p->SegHdrSize += (((rlcAmdPduSegInfo_p->numDfe -1) & 1) + 1) ;
        }
        szSdu += amReTxPdu_p->dfeInfo.sizeOfDfe[Idx];
        sizeOfDfe = amReTxPdu_p->dfeInfo.sizeOfDfe[Idx];
        if ((szSdu - 1) > rlcAmdPduSegInfo_p->soEnd)
        {
            sduToBeSegmented = RLC_TRUE;
            dfeEnd = sizeOfDfe - (szSdu - rlcAmdPduSegInfo_p->soEnd) + 1;
        }
        else
        {
            sduToBeSegmented = RLC_FALSE;
            dfeEnd = sizeOfDfe;
        }
        if (requestedDataSize < (rlcAmdPduSegInfo_p->rawDataSize + rlcAmdPduSegInfo_p->SegHdrSize + dfeEnd) )
        {
            sduToBeSegmented = RLC_TRUE;
            dfeEnd = requestedDataSize - (rlcAmdPduSegInfo_p->rawDataSize + rlcAmdPduSegInfo_p->SegHdrSize);
        }
        rlcAmdPduSegInfo_p->rawDataSize += dfeEnd;
        rlcAmdPduSegInfo_p->dfeLength[rlcAmdPduSegInfo_p->numDfe - 1] = dfeEnd;
        if (sduToBeSegmented == RLC_TRUE)
        {
            rlcAmdPduSegInfo_p->fi |= RLC_LAST_SDU_SEGMENTED;
        }
    }
    /** Calculate LSF **/
    if(pAmTxE->txPduIn.pRawPdu.size)
    {
       segmtLen = pAmTxE->txPduIn.pRawPdu.size - rlcAmdPduSegInfo_p->rawDataSize;
    }
    else
    {
       segmtLen = ((rlcAmdPduSegInfo_p->soEnd - rlcAmdPduSegInfo_p->soStart + 1) 
                   - rlcAmdPduSegInfo_p->rawDataSize);
    }
    if(!segmtLen && ((rlcAmdPduSegInfo_p->soEnd+ 1) == amReTxPdu_p->data_size))
    {
        /* SPR 4308 Fix Start */
        rlcAmdPduSegInfo_p->fi |= (amReTxPdu_p->dfeInfo.fi & RLC_LAST_SDU_SEGMENTED); 
        /* SPR 4308 Fix End */
        rlcAmdPduSegInfo_p->LSF = 0x01; 
    }
    else{
        rlcAmdPduSegInfo_p->LSF = 0x00;
    }
   
    return ret;
}


/****************************************************************************
 * Function Name  : rlcFillPduSegmentHeader
 * Inputs         : pAmTxE - a pointer to TxAMEntity,
 *                  amdPduSegHdr - Pdu Segment Fix Header,
 *                  pduFormed_p - a pointer to RlcAmBuffer,
 *                  rlcAmdPduSegInfo_p - a pointer to RlcAmdPduSegInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS  after successfull assignments
 * Description    : It is used to FILL PDU segment Header.
 ****************************************************************************/
UInt32 rlcFillPduSegmentHeader(TxAMEntity* pAmTxE,RlcAmdPduSegmentFixH amdPduSegHdr,
    RlcPduBuffer* pduFormed_p,RlcAmdPduSegInfo *rlcAmdPduSegInfo_p)
{
    UInt32 Idx = 0,bitPos = RLC_BYTE;
    UInt8 *base_data_p = RLC_NULL;
    /*Klockworks Warning Fix - reverted, incorrect fix */
    UInt8 e = 0;
    amdPduSegHdr.E = e = (rlcAmdPduSegInfo_p->numDfe > 1);
    UInt32 sduCountLoop = 0,liIndex = 0;
    /* Calculating Poll bit */
    amdPduSegHdr.P = (0x01 & rlcPollBitForReTxAmdPdu(pAmTxE));
    rlcAmdPduSegInfo_p->P = amdPduSegHdr.P;
    if(( base_data_p = msgReserve(pduFormed_p->data_p, 0 ,rlcAmdPduSegInfo_p->SegHdrSize))
            == RLC_NULL) 
    {
        LOG_RLC_MSG(LTE_RLC_MEM_ALLOC_FAILURE, LOGFATAL, L2_SYS_FAIL, globalTTITickCount_g, __LINE__,
                                0, 0,0,0, 0,0, __func__, "");

        return LTE_RLC_MSG_RESERVE_FAIL;
    }
    memSet(base_data_p, 0, rlcAmdPduSegInfo_p->SegHdrSize);
    /* Filling the Fix Header */
    /* Setting D/C value In header*/
    RLC_FILL_DC_AND_UPDATE_BIT_POS_IDX(base_data_p,RLC_TRUE,Idx,bitPos);
    /* Setting RF value In header*/
    RLC_FILL_RF_AND_UPDATE_BIT_POS_IDX(base_data_p,RLC_TRUE,Idx, bitPos);
    /* Setting P value In header*/
    RLC_FILL_P_AND_UPDATE_BIT_POS_IDX(base_data_p,amdPduSegHdr.P,Idx,bitPos);
    /** SPR 1203 END **/
    /* Setting FI value In header*/
    RLC_FILL_FI_AND_UPDATE_BIT_POS_IDX(base_data_p,rlcAmdPduSegInfo_p->fi,Idx,bitPos);
    
    /* Setting E value In header*/
    RLC_FILL_E_AND_UPDATE_BIT_POS_IDX(base_data_p,amdPduSegHdr.E,Idx,bitPos);
    
    /* Setting SN value In header*/
    RLC_FILL_SN_AND_UPDATE_BIT_POS_IDX(base_data_p,amdPduSegHdr.SN,Idx,bitPos);
        
    /* Setting LSF value In header*/
    RLC_FILL_LSF_AND_UPDATE_BIT_POS_IDX(base_data_p,rlcAmdPduSegInfo_p->LSF,Idx,bitPos);
        
    /* Setting SO value In header*/
    RLC_FILL_SO_AND_UPDATE_BIT_POS_IDX(base_data_p,rlcAmdPduSegInfo_p->soStart,Idx,bitPos);
    /** Filling variable header **/
    if(amdPduSegHdr.E)
    {
       Idx = AM_PDU_RESEG_FIX_HDR_SIZE;
       bitPos = RLC_BYTE;
       e = amdPduSegHdr.E;
       sduCountLoop = rlcAmdPduSegInfo_p->numDfe - 1;
       for(liIndex = 0; 0 < sduCountLoop ;liIndex++)
       {
           if( e == 1)
           {
               e = ((sduCountLoop--)  > 1);
               /* Setting E value in header */
               RLC_FILL_E_AND_UPDATE_BIT_POS_IDX(base_data_p, e, Idx, bitPos);
               /* Setting LI value in header */
               RLC_FILL_LI_AND_UPDATE_BIT_POS_IDX(base_data_p,
                   rlcAmdPduSegInfo_p->dfeLength[liIndex],Idx,bitPos);
           }
       }
    }

    return RLC_SUCCESS;
}
/****************************************************************************
 * Function Name  : rlcGetStartEndOffset
 * Inputs         : pAmTxE - a pointer to TxAMEntity,
 *                  amReTxPdu_p - a pointer to AmReTransmission,
 *                  segmentedPduType - flag to identify new or incomplete case,
 *                  rlcAmdPduSegInfo_p - a pointer to RlcAmdPduSegInfo
 * Outputs        : None
 * Returns        : None
 * Description    : To get soStart and soEnd of PDU segment.
 ****************************************************************************/
void rlcGetStartEndOffset( TxAMEntity* pAmTxE,
                                  AmReTransmission* amReTxPdu_p,
                                  AmNackListNode* amNackList_p,
                                  UInt8 segmentedPduType,
                                  RlcAmdPduSegInfo *rlcAmdPduSegInfo_p
                                )
{
    if(segmentedPduType)
    {
         rlcAmdPduSegInfo_p->soStart = pAmTxE->txPduIn.Sostart;
         rlcAmdPduSegInfo_p->soEnd = pAmTxE->txPduIn.Soend;
    }
    else
    {
        if(!amNackList_p->Sostart && !amNackList_p->Soend)
        {
            rlcAmdPduSegInfo_p->soStart = 0;
            rlcAmdPduSegInfo_p->soEnd = amReTxPdu_p->data_size - 1;
        }
        else 
        { 
           rlcAmdPduSegInfo_p->soStart = amNackList_p->Sostart;
           rlcAmdPduSegInfo_p->soEnd = amNackList_p->Soend;
        }
    }
}

/****************************************************************************
 * Function Name  : rlcCreateAmdPduSegmentHeader
 * Inputs         : pAmTxE - a pointer to TxAMEntity,
 *                  pduFormed_p - a pointer to RlcPduBuffer,
 *                  amReTxPdu_p - a pointer to AmReTransmission,
 *                  requestedDataSize - PDU Size,
 *                  rlcAmdPduSegInfo_p - a pointer to RlcAmdPduSegInfo,
 *                  amdPduSegHdr_p - a pointer to RlcAmdPduSegmentFixH
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : To encode AMD PDU segment.
 ****************************************************************************/
UInt32 rlcCreateAmdPduSegmentHeader( TxAMEntity* pAmTxE,
                                            RlcPduBuffer* pduFormed_p,
                                            AmReTransmission *amReTxPdu_p,
                                            UInt32 requestedDataSize,
                                            RlcAmdPduSegInfo *rlcAmdPduSegInfo_p,
                                            RlcAmdPduSegmentFixH *amdPduSegHdr_p
                                          )
{
    UInt32 ret = RLC_SUCCESS;
    /* Initializing AmdPduSegmentFixHeader */ 
    rlcInitAmdPduSegmentFixHeader(amdPduSegHdr_p, amReTxPdu_p->seqNum);
    /** Filling SO Field **/
    amdPduSegHdr_p->SO = rlcAmdPduSegInfo_p->soStart;
    rlcAmdPduSegInfo_p->SegHdrSize = AM_PDU_RESEG_FIX_HDR_SIZE;
    if(RLC_FAILURE == rlcGetAmPduSegVarHdrInfo(pAmTxE,amReTxPdu_p,rlcAmdPduSegInfo_p,
                      requestedDataSize))
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_RETX_PDU_ERR_5, LOGWARNING, (RLC_TX|RLC_AM),
                 globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                 pAmTxE->lcId, amdPduSegHdr_p->SO , RLC_FAILURE, 0, 0, __func__, "");
        return RLC_FAILURE;
    }
    if(rlcAmdPduSegInfo_p->rawDataSize  == 0)
    {
        return RLC_FAILURE;
    }

    /** Filling Pdu Segment Size and SN **/
    pduFormed_p->size = rlcAmdPduSegInfo_p->rawDataSize + rlcAmdPduSegInfo_p->SegHdrSize;
    pduFormed_p->seqNum = amReTxPdu_p->seqNum;


    return ret;
}

/****************************************************************************
 * Function Name  : rlcGetAmdDataSegment
 * Inputs         : data_p - Pointer to data,
 *                  amReTxPdu_p - a pointer to AmReTransmission
 *                  rlcAmdPduSegInfo_p - Pointer to RlcAmdPduSegInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : To get AMD PDU Data segment.
 ****************************************************************************/
UInt32 rlcGetAmdDataSegment( UInt8 **data_p,
                                    AmReTransmission* amReTxPdu_p,
                                    RlcAmdPduSegInfo *rlcAmdPduSegInfo_p
                                  )
{
    UInt16 splitPos = 0;
    UInt32 ret = RLC_SUCCESS;
    UInt8 *pdu_data_ptr = msgClone(amReTxPdu_p->pdu_data_p);
    *data_p = RLC_NULL; 
    if(RLC_NULL == pdu_data_ptr)
    {
        LOG_RLC_MSG( LTE_RLC_GET_DATA_SEG_ERR, LOGWARNING, (RLC_TX|RLC_AM),
                 globalTTITickCount_g, __LINE__, RLC_FAILURE,
                 0, 0, 0, 0, 0, __func__, "");
        return RLC_FAILURE;
    }
    UInt8 *data_ptr = (UInt8 *)msgAlloc(zeroPool_g,0,0,0);
    if(data_ptr == RLC_NULL)
    {
        LOG_RLC_MSG( LTE_RLC_GET_DATA_SEG_ERR_1, LOGWARNING, (RLC_TX|RLC_AM),
                 globalTTITickCount_g, __LINE__, RLC_FAILURE,
                 0, 0, 0, 0, 0, __func__, "");
        msgFree(pdu_data_ptr);
        /* SPR 20985 fix start */
        pdu_data_ptr = RLC_NULL;
        /* SPR 20985 fix end */
        return RLC_FAILURE;
    }
    splitPos = ((amReTxPdu_p->pdu_size - amReTxPdu_p->data_size) + 
               rlcAmdPduSegInfo_p->soStart);
    /** get AM PDU from soStart **/ 
    if(RLC_NULL == msgSplit(pdu_data_ptr,splitPos,data_ptr))
    {
        LOG_RLC_MSG( LTE_RLC_GET_DATA_SEG_ERR_2, LOGWARNING, (RLC_TX|RLC_AM),
                 globalTTITickCount_g, __LINE__, splitPos,RLC_FAILURE,
                 0, 0, 0, 0, __func__, "");
                 
        msgFree(pdu_data_ptr);
        msgFree(data_ptr);
        /* SPR 20985 fix start */
        pdu_data_ptr = RLC_NULL;
        data_ptr = RLC_NULL;
        /* SPR 20985 fix end */
        return RLC_FAILURE;
    }
    msgFree(pdu_data_ptr);
    pdu_data_ptr = RLC_NULL;
    /** Getting Actual Data PDU Pointer **/
    if((rlcAmdPduSegInfo_p->soEnd + 1) != amReTxPdu_p->data_size )
    {
       pdu_data_ptr = (UInt8 *)msgAlloc(zeroPool_g,0,0,0);
       if(pdu_data_ptr == RLC_NULL)
       {
        LOG_RLC_MSG( LTE_RLC_GET_DATA_SEG_ERR_3, LOGWARNING, (RLC_TX|RLC_AM),
                 globalTTITickCount_g, __LINE__, RLC_FAILURE,
                 0, 0, 0, 0, 0, __func__, "");
                 
            msgFree(data_ptr);
            /* SPR 20985 fix start */
            data_ptr = RLC_NULL;        
            /* SPR 20985 fix end */
            return RLC_FAILURE;
       }
       splitPos = rlcAmdPduSegInfo_p->rawDataSize;
       if(RLC_NULL == msgSplit(data_ptr,splitPos,pdu_data_ptr))
       {
        LOG_RLC_MSG( LTE_RLC_GET_DATA_SEG_ERR_4, LOGWARNING, (RLC_TX|RLC_AM),
                 globalTTITickCount_g, __LINE__, splitPos,RLC_FAILURE,
                 0, 0, 0, 0, __func__, "");
                 
            msgFree(pdu_data_ptr);
            msgFree(data_ptr);
            /* SPR 20985 fix start */
            pdu_data_ptr = RLC_NULL;
            data_ptr = RLC_NULL;
            /* SPR 20985 fix end */
            return RLC_FAILURE;
       }
       /* SPR 4190 Fix Start */
       /** Freeing Split part **/
       msgFree(pdu_data_ptr);
        /* SPR 20985 fix start */
        pdu_data_ptr = RLC_NULL;
        /* SPR 20985 fix end */
       /* SPR 4190 Fix End */
    }
    *data_p = data_ptr;
    return ret;
}

/****************************************************************************
 * Function Name  : rlcUpdateRemainSegmentAndLSFInfo
 * Inputs         : pAmTxE - a pointer to AM Entity,
 *                  rlcAmdPduSegInfo_p - a pointer to RlcAmdPduSegInfo,
 *                  seqNum - Sequence Number
 * Outputs        : None
 * Returns        : RLC_SUCCESS
 * Description    : This function updates the AM PDU information 
 ****************************************************************************/
UInt32 rlcUpdateRemainSegmentAndLSFInfo( TxAMEntity* pAmTxE,
                                                RlcAmdPduSegInfo *rlcAmdPduSegInfo_p,
                                                UInt16 seqNum
                                              )
{
    UInt32 ret = RLC_SUCCESS;
    UInt32 segmtLen = 0;
    pAmTxE->txPduIn.sn = seqNum; 
    if(pAmTxE->txPduIn.pRawPdu.size)
    {
        segmtLen = pAmTxE->txPduIn.pRawPdu.size - rlcAmdPduSegInfo_p->rawDataSize;
    }
    else
    {
        segmtLen = ((rlcAmdPduSegInfo_p->soEnd - rlcAmdPduSegInfo_p->soStart + 1) - 
                 rlcAmdPduSegInfo_p->rawDataSize);
    }
    if(!segmtLen)
    {
        pAmTxE->txPduIn.pRawPdu.size = 0;
        pAmTxE->txPduIn.Sostart = pAmTxE->txPduIn.Soend = 0;
    }
    else
    {
        pAmTxE->txPduIn.pRawPdu.size = segmtLen;
        pAmTxE->txPduIn.Sostart = (rlcAmdPduSegInfo_p->soEnd - pAmTxE->txPduIn.pRawPdu.size +1);
        pAmTxE->txPduIn.Soend = rlcAmdPduSegInfo_p->soEnd;
    }
    /* SPR 4311 Fix Start */
    //if (pAmTxE->txPduIn.Sostart >= pAmTxE->txPduIn.Soend)
    if (pAmTxE->txPduIn.Sostart > pAmTxE->txPduIn.Soend)
    /* SPR 4311 Fix End */
    {
       pAmTxE->txPduIn.pRawPdu.size = 0;
    }
    return ret;
}

/****************************************************************************
 * Function Name  : rlcEncodeAmdPduSegment
 * Inputs         : pAmTxE - a pointer to TxAMEntity,
 *                  pduFormed_p - a pointer to RlcAmBuffer,
 *                  requestedDataSize - Pdu size,
 *		            amReTxPdu_p - a pointer to AmReTransmission,
 *		            pollBit - poll bit pointer,
 *		            amNackList_p - a pointer to AmNackListNode,
 *		            segmentedPduType - flag to identify new or incomplete case,
 *                  rlcHarqFailureInfo_p - a pointer to RLCHarqFailureInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : To encode AMD PDU segment.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 rlcEncodeAmdPduSegment( TxAMEntity* pAmTxE, 
/* SPR 5599_129119 Fix End */
                                      RlcPduBuffer* pduFormed_p,
                                      UInt32 requestedDataSize,
                                      AmReTransmission* amReTxPdu_p,
                                      UInt8 *pollBit, 
                                      AmNackListNode* amNackList_p,
                                      UInt8 segmentedPduType,
                                      RLCHarqFailureInfo* rlcHarqFailureInfo_p
                                    )
{
    RlcAmdPduSegInfo rlcAmdPduSegInfo_p = {0};
     RlcAmdPduSegmentFixH amdPduSegHdr;
    UInt8 *data_ptr = RLC_NULL;
    if(requestedDataSize <= AM_PDU_RESEG_FIX_HDR_SIZE) 
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_RETX_PDU_ERR, LOGWARNING, (RLC_TX|RLC_AM),
                 globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,pAmTxE->lcId, 
                 requestedDataSize, RLC_FAILURE, 0, 0, __func__,"");
        return RLC_FAILURE;
    }

    /** Get SoStart and SoEnd **/
     rlcGetStartEndOffset(pAmTxE, amReTxPdu_p, amNackList_p,
        segmentedPduType,&rlcAmdPduSegInfo_p );
    /*Allocating memory for the Segmented PDU*/
    if((pduFormed_p->data_p
                = (UInt8*)msgAlloc(zeroPool_g,0,0,0))
            == RLC_NULL)
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_RETX_PDU_ERR_2, LOGWARNING, (RLC_TX|RLC_AM),
                 globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                 pAmTxE->lcId, RLC_FAILURE, 0, 0, 0, __func__, "");
        return RLC_FAILURE;
    }
    
    /* Calculating fi -- Setting MSB of FI field and get szVH and SzRawDataSeg*/
    if(RLC_FAILURE == rlcCreateAmdPduSegmentHeader(pAmTxE, pduFormed_p, amReTxPdu_p,
        requestedDataSize, &rlcAmdPduSegInfo_p, &amdPduSegHdr))
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_RETX_PDU_ERR_3, LOGWARNING, (RLC_TX|RLC_AM),
                 globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                 pAmTxE->lcId, RLC_FAILURE, 0, 0, 0, __func__, "");
         msgFree(pduFormed_p->data_p);
         pduFormed_p->data_p = RLC_NULL;
         return RLC_FAILURE;
    }
    /** get data pointer from Re-Tx PDU **/
    if(RLC_FAILURE == rlcGetAmdDataSegment(&data_ptr,amReTxPdu_p,&rlcAmdPduSegInfo_p))
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_RETX_PDU_ERR_6, LOGWARNING, (RLC_TX|RLC_AM),
                 globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                 pAmTxE->lcId, RLC_FAILURE, 0, 0, 0, __func__, "");
        /** SPR 3722 Changes Start **/
        if(pduFormed_p->data_p)
        {
            msgFree(pduFormed_p->data_p);
            pduFormed_p->data_p = RLC_NULL;
        }
        /** SPR 3722 Changes End **/
       return RLC_FAILURE;
    }
    /** Update Remaining Segment Info **/
    rlcUpdateRemainSegmentAndLSFInfo(pAmTxE,&rlcAmdPduSegInfo_p,amReTxPdu_p->seqNum);
    /** Updating Re-Tx Queue Size **/

    /* SPR 4949 Fix Start */
    atomicSubtractInt(&pAmTxE->reTxQueueSize ,(rlcAmdPduSegInfo_p.rawDataSize - AM_PDU_RESEG_FIX_HDR_SIZE));
    /* SPR 4949 Fix End */
 
    /**Filling Pdu Segment Header **/   
    rlcFillPduSegmentHeader(pAmTxE,amdPduSegHdr,pduFormed_p,&rlcAmdPduSegInfo_p);
         
                    
    /** Joining both header and data **/
    if(RLC_FAILURE == msgJoinAndFree(pduFormed_p->data_p,data_ptr))
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_RETX_PDU_ERR_7, LOGWARNING, (RLC_TX|RLC_AM),
                 globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                 pAmTxE->lcId, RLC_FAILURE, 0, 0, 0, __func__, "");
                 
        msgFree(pduFormed_p->data_p);
        pduFormed_p->data_p = RLC_NULL;
        msgFree(data_ptr);
        data_ptr= RLC_NULL;
        return RLC_FAILURE;
    }
    data_ptr= RLC_NULL;
    /** SPR 3657 Changes Start **/
    /* Filling Harq Information*/
    rlcHarqFailureInfo_p->harqFailReTxInfo[rlcHarqFailureInfo_p->reTxPduCount].
        retxPduSeqNum = pduFormed_p->seqNum;
    rlcHarqFailureInfo_p->harqFailReTxInfo[rlcHarqFailureInfo_p->reTxPduCount].
        SOStart       = rlcAmdPduSegInfo_p.soStart;
    rlcHarqFailureInfo_p->harqFailureReporting
        = REPORTING_TRUE;
    rlcHarqFailureInfo_p->harqFailReTxInfo[rlcHarqFailureInfo_p->reTxPduCount].
       SOEnd = (rlcAmdPduSegInfo_p.soStart + rlcAmdPduSegInfo_p.rawDataSize - 1);
    rlcHarqFailureInfo_p->reTxPduCount++;
    /** SPR 3657 Changes End **/
    *pollBit = rlcAmdPduSegInfo_p.P;    
    
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : rlcSendMaxReTxThresholdInd
 * Inputs         : pAmTxE - a pointer to TxAMEntity,
 *                  amReTxPdu_p - re-Transmission Node
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : Send Negative delivery status to upper layer.
 ****************************************************************************/
UInt32 rlcSendMaxReTxThresholdInd(TxAMEntity* pAmTxE,
          AmReTransmission * amReTxPdu_p)
{
    UInt16 ueIndex = pAmTxE->ueIndex;
    UInt16 deliveryStatus = RLC_FAILURE;
    SduTidInfo* sduTidInfo_p = RLC_NULL;
    RlcSn reTxQIdx = 0;
    UInt16 rlcStatusBufferCount = 0;
    UInt8 lcId = pAmTxE->lcId;
    SduTidInfoQ *sduTidInfoQ_p    =   &pAmTxE->tidQueue;
    RlcStatusBuffer *rlcStatusBuffer = PNULL;
    
    /* SPR 4440 Fix Start */
    rlcStatusBuffer =   getMemFromPool((sizeof(RlcStatusBuffer) * \
                amReTxPdu_p->dfeInfo.dfeCount) + 1,RLC_NULL);
    /* SPR 4440 Fix End */
    if( RLC_NULL == rlcStatusBuffer)
    {
        ltePanic("%s => Memory Allocation Failure",__FUNCTION__);
        return RLC_FAILURE;
    }

    LOG_RLC_MSG(LTE_RLC_MAX_RE_TX_IND, LOGDEBUG,(RLC_TX|RLC_AM), globalTTITickCount_g, __LINE__,
            pAmTxE->ueIndex, pAmTxE->lcId,amReTxPdu_p->seqNum, 0, 0,0, __func__, "");
    
    while (amReTxPdu_p->dfeInfo.dfeCount--)
    {
        DEQUEUE_AMD_SDU_TID_Q(sduTidInfo_p,sduTidInfoQ_p);
        if ( (sduTidInfo_p) &&((amReTxPdu_p->dfeInfo.fi == 0x00 || amReTxPdu_p->dfeInfo.fi == 0x02)))
        {
            rlcStatusBuffer[rlcStatusBufferCount].transId = sduTidInfo_p->tId;
            rlcStatusBuffer[rlcStatusBufferCount].deliveryStatus =deliveryStatus;
            rlcStatusBufferCount++;
        }
    }
    /* SPR 10574 changes start */
    if( rlcStatusBufferCount )
    {
        /* SPR 10574 changes end */
        LOG_RLC_MSG(LTE_RLC_DELIVERY_STATUS_IND, LOGDEBUG, RLC_AM, globalTTITickCount_g, __LINE__,
                ueIndex ,lcId ,rlcStatusBufferCount,0, 0,0, __func__, "");
        CLOCK_START_RLC_API(RLC_DLVRY_STATUS_IND);
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
		/*Dual Connectivity SeNB changes +*/
		if((SN_TERMINATED_SCG_SPLIT_BEARER ==  pAmTxE->dcBearerType) && (pAmTxE->lcId >= MIN_DRB_LC_ID))
		    sendRlcDeliveryStatusToX2U(
		            pAmTxE->ueIndex,
		            pAmTxE->lcId,
		            rlcStatusBufferCount,
		            rlcStatusBuffer);
		else
		    /*Dual Connectivity SeNB changes -*/
#endif

        rlcDeliveryStatusInd(ueIndex ,lcId ,rlcStatusBufferCount, rlcStatusBuffer);
        CLOCK_END_RLC_PDCP_API(RLC_DLVRY_STATUS_IND);

        gRlcStats.pRlcStats->lteRlcUeStat[ueIndex].
            lcStats[lcId].numDlvryInd += rlcStatusBufferCount;
        /* SPR 10574 changes start */
    }
    else
    {
        freeMemPool( rlcStatusBuffer );
    }
    /* SPR 10574 changes end */

    reTxQIdx = amReTxPdu_p->seqNum & 511;
    FREE_AM_RETX_PDU(amReTxPdu_p);
    pAmTxE->reTxQueue[reTxQIdx] = RLC_NULL;
    return RLC_SUCCESS;
}

