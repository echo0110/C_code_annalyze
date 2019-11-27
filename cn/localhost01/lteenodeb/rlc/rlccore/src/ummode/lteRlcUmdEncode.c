/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcUmdEncode.c,v 1.1.1.1.12.2.2.5 2010/10/25 10:58:36 gur11835 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file describes encoding of SDU(s) to form UMD PDU.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcUmdEncode.c,v $
 *
 *
 *
****************************************************************************/


/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcUmArithmetic.h"
#include "lteRlcUmdEncode.h"
#include "lteRlcPdcpInterface.h"
#include "logging.h"
#include "lteRlcBuffer.h"
#include "lteRlcEntityDef.h"
#include "lteRlcErrorCode.h"
#include "alarm.h"
#include "lteRlcOamInterface.h"
#ifndef UE_SIM_TESTING
#include "lteCommonStatsManager.h"
#endif
#include "lteRlcUeContext.h"

/****************************************************************************
 * Varibale Definition
 ****************************************************************************/
/****************************************************************************
 * External Variable Includes
 ****************************************************************************/
    /* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
    /* SPR 15909 fix end */
extern void *zeroPool_g;

//inline static UInt32 rlcSetFixedHdrUmdRawPdu(UInt8 *base_data_p,UInt8 fi,UInt8 e,UInt16 sn,UInt32 snLen);

/****************************************************************************
 * Function Name  : rlcAddDfeToPdu
 * Inputs         : encodedPdu_p - a pointer to UMD PDU buffer,
 *                  pDfe - pointer to Sdu to be added,
 *                  size - size of  SDU,
 *                  pduHdrInfo_p - pointer to structure to maintain info for
 *                                 header formation,
 *                  pAvaliableSize - pointer to remaining requested size of PDU,
 *                  incompleteSdu_p - pointer to UmdRawSduIn,
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE with error
 * Description    : 1. Adds SDU( complete) to PDU if avaliable size
 *                     is greater than SDu size
 *                  2. Splits and then add SDU segment to pDU if avaliable size
 *                     is less than SDU size
 *                  3. Store Incomplete SDU info if any
 *                  4. Updates header size
 *                  5. updates numof DFEs.
 ****************************************************************************/
 UInt32 rlcAddDfeToPdu(UInt8 *encodedPdu_p,
                                    UInt8 *pDfe,
                                    UInt16 size,
                                    RlcPduHeaderInfo *pduHdrInfo_p,
                                    UInt32 *pAvailableSize,
                                    UmdRawSduIn **incompleteSdu_p)
{
    UInt32  availableSize = *pAvailableSize;
    UInt8   *pUnsentSeg   = PNULL;
    UInt32 unsentSegSz    = 0;
    UInt16  numDfe = pduHdrInfo_p->numDfe + 1;

    /* If available size is less than 2bytes, do not encode */
    /** SPR 3823 Changes Start **/
    if ((availableSize <= 2) && (pduHdrInfo_p->numDfe))
    /** SPR 3823 Changes End **/
    {
        LOG_RLC_MSG( LTE_RLC_ADD_DFE_TO_PDU_ERR, LOGWARNING, 
        (RLC_TX|RLC_UM), globalTTITickCount_g, __LINE__,
        availableSize, size, numDfe, 0, 0, 0, __func__, "");

        return LTE_RLC_INSUFF_SIZE_FOR_ENCODING;
    }

    /* Calculate available size for data */
    if(pduHdrInfo_p->numDfe > 0)
    {
        availableSize -= (((numDfe +1) & 1) + 1);
    }    

    /* If available size < SDU size, split the SDU */
    if (availableSize < size)
    {
        if ( (pUnsentSeg = msgAlloc(zeroPool_g,1,0,0) ) == RLC_NULL)
        {
            LOG_RLC_MSG( LTE_RLC_ADD_DFE_TO_PDU_ERR_1, LOGFATAL, 
                    (RLC_TX|RLC_UM), globalTTITickCount_g, __LINE__,
                    availableSize, size, numDfe, 0, 0, 0, __func__, "");

            return LTE_RLC_MSG_ALLOC_FAIL;
        }

        /* Split the segment at available size */
        if (msgSplit(pDfe, availableSize, pUnsentSeg) == RLC_NULL)
        {
            LOG_RLC_MSG( LTE_RLC_ADD_DFE_TO_PDU_ERR_2, LOGWARNING, 
                    (RLC_TX|RLC_UM), globalTTITickCount_g, __LINE__,
                    availableSize, size, numDfe, 0, 0, 0, __func__, "");
                    
            msgFree(pUnsentSeg);
            return LTE_RLC_MSG_SPLIT_FAIL;
        }
       /* If there is any unsent Segment still left, save its size in Incomplete SDU */
        unsentSegSz = (size - availableSize)  ;
        /* Update length of Dfe */
        pduHdrInfo_p->dfeLength[numDfe - 1] = availableSize;
        availableSize = 0;
    }
    else
    {
        pduHdrInfo_p->dfeLength[numDfe - 1] = size;
        availableSize -= size;
    }

    /* Join SDU with PDU data */
    msgJoinAndFree(encodedPdu_p, pDfe);

    /* If there is any unsent Segment still left, save it pointer and size in Incomplete SDU */
    (*incompleteSdu_p)->pRawSdu.data_p = pUnsentSeg;
    (*incompleteSdu_p)->pRawSdu.size   = unsentSegSz;

    /* Update header info */
    pduHdrInfo_p->numDfe++;
    if(pduHdrInfo_p->numDfe > 1)	
        pduHdrInfo_p->hdrSize +=(((pduHdrInfo_p->numDfe +1) & 1) + 1);

    /* update available size */
    *pAvailableSize = availableSize;

    return RLC_SUCCESS;
}
/********************************************************************************
 * Function Name  : rlcAddSduSegmentToPdu
 * Inputs         : encodedPdu_p - a pointer to UMD PDU buffer,
 *                  pduHdrInfo_p - pointer to structure to maintain info for
 *                                 header formation,
 *                  pAvaliableSize - pointer to remaining requested size of PDU,
 *                  incompleteSdu_p - pointer to UmdRawSduIn
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : 1. API to add Incomplete SDU segment to PDU 
 *                  2. updates fi accordingly.
 ****************************************************************************/

 UInt32 rlcAddSduSegmentToPdu(UInt8 *encodedPdu_p, 
                                           RlcPduHeaderInfo *pduHdrInfo_p,
                                           UInt32 *pAvailableSize,
                                           UmdRawSduIn **incompleteSdu_p)
{
    UInt32 retVal = RLC_SUCCESS;
        /*   Incomplet SDu check added--confirm */
    if ((*incompleteSdu_p)->pRawSdu.data_p != RLC_NULL)
    {
        retVal = rlcAddDfeToPdu(encodedPdu_p, (*incompleteSdu_p)->pRawSdu.data_p,
                (*incompleteSdu_p)->pRawSdu.size, pduHdrInfo_p, pAvailableSize,
                incompleteSdu_p);
        /* Since we are adding a segmented SDU, this has to be the first sdu
           in the PDU, hence set fi bit for it */
        pduHdrInfo_p->fi |= RLC_FIRST_SDU_SEGMENTED;

        /* If there is an incomplete SDU, then that is the last SDU, and is
         *        segmented, hence set the fi bit for it */
        if (RLC_NULL != (*incompleteSdu_p)->pRawSdu.data_p)
        {
            pduHdrInfo_p->fi |= RLC_LAST_SDU_SEGMENTED;
        }
    }
    return retVal;
}
/*********************************************************************************
 * Function Name  : rlcAddSduToPdu
 * Inputs         : encodedPdu_p - a pointer to UMD PDU buffer,
 *                  sdu_p - pointer to RlcUeDataReq,
 *                  pduHdrInfo_p - pointer to structure to maintain info for
 *                                 header formation,
 *                  pAvaliableSize - pointer to remaining requested size of PDU,
 *                  incompleteSdu_p - pointer to UmdRawSduIn
 * Outputs        : None
 * Returns        : retVal
 * Description    : 1.API to add SDU to PDU
 *                  2.updates fi accordingly.
 ********************************************************************************/

static inline UInt32 rlcAddSduToPdu(UInt8 *encodedPdu_p, 
                                    RlcUeDataReq *sdu_p,
                                    RlcPduHeaderInfo *pduHdrInfo_p,
                                    UInt32 *pAvailableSize,
                                    UmdRawSduIn **incompleteSdu_p)
{
    UInt32 retVal = rlcAddDfeToPdu(encodedPdu_p, sdu_p->data_p, sdu_p->size,
            pduHdrInfo_p, pAvailableSize, incompleteSdu_p);

    /* If there is an incomplete SDU, then that is the last SDU, and is
       segmented, hence set the fi bit for it */
    if (RLC_NULL != (*incompleteSdu_p)->pRawSdu.data_p)
    {
        pduHdrInfo_p->fi |= RLC_LAST_SDU_SEGMENTED;
        /* SPR 4497 Changes Start */
        (*incompleteSdu_p)->transId = sdu_p->transId;
        /* SPR 4497 Changes End */
/* KPI Changes Start */
#if defined(KPI_STATS) || defined(PERF_STATS)
        (*incompleteSdu_p)->unsegSduSize = sdu_p->size;
#endif
    }
    return retVal;
}
/****************************************************************************
 * Function Name  : rlcEncodePduHeader
 * Inputs         : encodedPdu_p - a pointer to UMD PDU buffer,
 *                  pduHdrInfo_p - pointer to structure to maintain info for,
 * Outputs        : pSzRawPdu - data size of PDU formed
 * Returns        : RLC_SUCCESS/RLC_FAILURE with error
 * Description    : 1.API to encode variable header of PDU formed 
 *                  2.calculates PDU data size.
 ****************************************************************************/

 UInt32 rlcEncodePduHeader(UInt8 *encodedPdu_p,
                                        RlcPduHeaderInfo *pduHdrInfo_p,
                                        UInt32 *pSzRawPdu)
{
    UInt8 *pPduHeader;
    UInt16 Idx = 0;
    UInt32 szSdu = 0;
    UInt8 eToggle = 0;
    UInt8 e = 0 ;
    UInt8 value = 0; 

    if (pduHdrInfo_p->numDfe == 0)
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_PDU_HEADER_ERR, LOGWARNING, 
                (RLC_TX|RLC_UM), globalTTITickCount_g, __LINE__,
                0, 0, 0, 0, 0, 0, __func__, "");

        return LTE_RLC_NO_DATA_TO_ENCODE;
    }
    /* Reserve memory for header */
    if ( (pPduHeader = msgReserve(encodedPdu_p, 0, pduHdrInfo_p->hdrSize) )
            == RLC_NULL)
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_PDU_HEADER_1, LOGWARNING, 
                (RLC_TX|RLC_UM), globalTTITickCount_g, __LINE__,
                0, 0, 0, 0, 0, 0, __func__, "");

        return LTE_RLC_MSG_RESERVE_FAIL;
    }
    /* forming variable header */
    if ( pduHdrInfo_p->numDfe > 1)
    {
        pPduHeader += ( (pduHdrInfo_p->snLen + 7) >> 3);
        for ( Idx = 0 ; Idx < (pduHdrInfo_p->numDfe -1); Idx ++)
        {
            e           = ((pduHdrInfo_p->numDfe - Idx) > 2 );
            szSdu       = pduHdrInfo_p->dfeLength[Idx];
            *pSzRawPdu += szSdu;

            if(eToggle ^= 1)
            {
                value         = (e & 0x01U) << 7;
                value        |= ((szSdu >> 4)  & 0x7FU);
                *pPduHeader++ = value;
                value         = (szSdu  & 0xFU) << 4;
                *pPduHeader   = value; 
            }
            else
            {
                value        |= (e & 0x01U) << 3 ;
                value        |= ((szSdu >> 8 ) & 0x07U);
                *pPduHeader++ = value;
                value         = (szSdu & 0xFFU);
                *pPduHeader++ = value;               
            }
        }
    }
    *pSzRawPdu += pduHdrInfo_p->dfeLength[pduHdrInfo_p->numDfe - 1];

    return RLC_SUCCESS;

}
/* Cyclomatic Complexity changes - starts here */
#if defined(KPI_STATS) || defined(PERF_STATS)
/****************************************************************************
 * Function Name  : rlcGetPdcpHdrSize
 * Inputs         : pE - a pointer to TxUMEntity
 * Outputs        : None
 * Returns        : pdcpHdrSize
 * Description    : This function gets PDCP HDR Size.
 ****************************************************************************/
static inline UInt32 rlcGetPdcpHdrSize(TxUMEntity *pE)
{
    UInt32 pdcpHdrSize = 0;
    if( pE->snFieldLength == 5 )
    {
        pdcpHdrSize = 1;
    }
    else if( pE->snFieldLength == 10 )
    {
        pdcpHdrSize = 2;
    }
    return pdcpHdrSize;
}
#endif
/* Cyclomatic Complexity changes - ends here */

/****************************************************************************
 * Function Name  : rlcEncodeUmdRawPdu
 * Inputs         : pE - Pointer to TxUMEntity,
 *                  pAvailableSize - a pointer to available size,
 *                  txQueueSize_p - a pointer to tx queue size,
 *                  dfeCount - Count of DFE's,
 *                  pktArrTime - Packet arrival time
 * Outputs        : pOutPdu - pointer to UmdRawPdu
 *                  pEncodedDataSize - pointer to size of formed pdu
 * Returns        : RLC_SUCCESS if successful else RLC_FAILURE
 * Description    : 1.encodes (including segmetation and concatenation of UMD 
 *                    SDU(s)) SDU(s) from @q into the @pOutPdu
 *                  2. returns PDU size in pAvaliableSize 
 *                  3. updtaes txQueueSize_p (tx queue load ).
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 rlcEncodeUmdRawPdu ( TxUMEntity *pE,
/* SPR 5599_129119 Fix End */
                                    UInt32 *pAvailableSize,
                                    UInt32 *txQueueSize_p,
                                    UmdRawPdu *pOutPdu,
                                    UInt32 *dfeCount, 
                                    /* SPR 15909 fix start */
                                    tickType_t *pktArrTime
                                    /* SPR 15909 fix end */
                                  )
{
    RlcPduHeaderInfo *pduHdrInfo_p;
    UInt8           *encodedPdu_p;
    RlcUeDataReq    *sdu_p           = PNULL;
    UmdRawSduQ      *sduQ_p          = &pE->txSDUQueue;
    UmdRawSduIn     *incompleteSdu_p = &pE->txSduIn;
    UInt32          retVal           = RLC_SUCCESS;
    UInt32          szRawPdu         = 0;
    /* SPR 4497 Changes Start */    
    UInt32 rlcStatusBufferCount = 0;
    RlcStatusBuffer *rlcQStatusBuffer_p = PNULL;
    /* SPR 4497 Changes End */    
#ifndef UE_SIM_TESTING
    UInt8 sduCount                   = 0;
#endif
    /* KPI Changes Start */
#if defined(KPI_STATS) || defined(PERF_STATS)
    UInt32 pdcpHdrSize = 0;

    /* PDCP Header would be either 1 byte or 2 byte */
    /* Cyclomatic Complexity changes - starts here */
    pdcpHdrSize = rlcGetPdcpHdrSize(pE);
    /* Cyclomatic Complexity changes - ends here */
#endif
    /* KPI Changes End */
	/* + PERF_CA GBL */
    InternalCellIndex cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(pE->ueIndex);    
	/* - PERF_CA GBL */
	
    /* SPR 13888 fix start */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    UInt8 commonIndex = 0;
    UInt16 lcId = 0;
    UInt16 ueIndex = 0;
#endif

    /* Use local variable for available size to avoid restoring original
       value in case of failure */
    UInt32 availableSize = *pAvailableSize;
    UInt32 snLen         =  pE->snFieldLength;

    UInt32 sQCount = QUEUE_COUNT_UMD_RAW_SDU_Q(*sduQ_p); 
    if( !sQCount && pE->txSduIn.pRawSdu.size == 0)
    {
         pE->txQueueSize = 0; 
        return RLC_FAILURE;
    }

    /* Allocate Structure for storing header information */
    if ( (pduHdrInfo_p = getMemFromPool(sizeof(RlcPduHeaderInfo), RLC_NULL ) )
            == RLC_NULL)
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_UMD_RAW_PDU_ERR, LOGFATAL, 
                (RLC_TX|RLC_UM), globalTTITickCount_g, __LINE__,
                pE->ueIndex, pE->lcId, availableSize, 0, 0, 0, __func__, "");

        return LTE_RLC_MEM_ALLOC_FAIL;
    }
    pduHdrInfo_p->hdrSize = ( (snLen + 7) >> 3); /* Fixed header size */
    pduHdrInfo_p->fi      = 0;
    pduHdrInfo_p->numDfe  = 0;
    pduHdrInfo_p->snLen   = snLen;
    availableSize        -= pduHdrInfo_p->hdrSize; /* size available for data */

    /* Allocate memory for out pdu */
    if( (encodedPdu_p = msgAlloc(zeroPool_g,0,0,0) ) == RLC_NULL)
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_UMD_RAW_PDU_ERR_1, LOGFATAL, 
                (RLC_TX|RLC_UM), globalTTITickCount_g, __LINE__,
                pE->ueIndex,  pE->lcId, availableSize, 0, 0, 0, __func__, "");

        freeMemPool(pduHdrInfo_p);
        return LTE_RLC_MSG_ALLOC_FAILURE;
    }

    /* SPR 4497 Changes Start */    
    rlcQStatusBuffer_p =   getMemFromPool((sizeof(RlcStatusBuffer) * RLC_MAX_DFE_IN_PDU ) ,RLC_NULL);
    if( RLC_NULL == rlcQStatusBuffer_p)
    {
        /* + Coverity 25513 */
        msgFree( encodedPdu_p );
        freeMemPool(pduHdrInfo_p);
        /* - Coverity 25513 */
        ltePanic("%s => Memory Allocation Failure",__FUNCTION__);
        return RLC_FAILURE;
    }
    /* SPR 4497 Changes End */    

    /* Encode the segmented SDU first */
    if (RLC_SUCCESS != (retVal = rlcAddSduSegmentToPdu(encodedPdu_p,
                    pduHdrInfo_p, &availableSize, &incompleteSdu_p) ) )
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_UMD_RAW_PDU_ERR_2, LOGWARNING, 
                (RLC_TX|RLC_UM), globalTTITickCount_g, __LINE__,
                pE->ueIndex, pE->lcId, availableSize, 0, 0, 0, __func__, "");

        freeMemPool(pduHdrInfo_p);
        freeMemPool(rlcQStatusBuffer_p);
        msgFree(encodedPdu_p);
        return LTE_RLC_ENCODING_SDU_SEG_FAIL;
    }

    /* Encode all enqueued SDUs */

    /* SPR 4497 Changes Start */    
    if( pduHdrInfo_p->fi == 0x02 )
    {
        /* Cyclomatic Complexity changes - starts here */
        rlcQStatusBuffer_p[rlcStatusBufferCount].transId = incompleteSdu_p->transId;
        /* Cyclomatic Complexity changes - ends here */
        rlcQStatusBuffer_p[rlcStatusBufferCount].deliveryStatus = RLC_SUCCESS;
        rlcStatusBufferCount++;
#if defined(KPI_STATS) || defined(PERF_STATS)
        pOutPdu->pRawPdu.pdcpSduSize[pOutPdu->pRawPdu.completeEncodedSduCount] =
            (incompleteSdu_p->unsegSduSize - pdcpHdrSize );
        pOutPdu->pRawPdu.completeEncodedSduCount++;
#endif
    }
    /* SPR 4497 Changes End */    

    /** SPR 3823 Changes Start **/
    /* + SPR 12349 Changes */
    while (((availableSize > 2) || (!pduHdrInfo_p->numDfe && availableSize) ) && ((SInt32)*txQueueSize_p > 0 ))
    /* - SPR 12349 Changes */
        /** SPR 3823 Changes End **/
    {
        sdu_p = PNULL;
        DE_QUEUE_TX_UMD_SDU_Q(sdu_p, sduQ_p);
        /* Break out of loop if SDU dequeue fails */
        if(PNULL == sdu_p)
        {
            LOG_RLC_MSG( LTE_RLC_ENCODE_UMD_RAW_PDU_ERR_3, LOGWARNING, 
                    (RLC_TX|RLC_UM), globalTTITickCount_g, __LINE__,
                    pE->ueIndex,  pE->lcId, availableSize,
                    0, 0, 0, __func__, "");
            break;
        }
        /* SPR 13888 fix start */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        lcId = sdu_p->lcId;
        ueIndex = sdu_p->ueIndex;
        /* Updating pdcpTick for a packet with its pktArrTime, 
         * ie - the time when this packet is received at pdcp */
        if(lcId >2)
        {
            commonIndex = scheduledIPThroughputDL_g[ueIndex][lcId - 3].commonIndex;
            if(0 == scheduledIPThroughputDL_g[ueIndex][lcId - 3].pdcpTick[commonIndex])
            {
                scheduledIPThroughputDL_g[ueIndex][lcId - 3].pdcpTick[commonIndex] =  sdu_p->pktArrTime;
            }
        }
#endif
        /* SPR 13888 fix end */
        /* + PERF_CA GBL */
        lteRlcStatInfo_g[cellIndex].lteUMStat.umdSduTransmittedToMac++;
		/* - PERF_CA GBL */


        /* Join SDU to PDU */
        if (RLC_SUCCESS != (retVal = rlcAddSduToPdu(encodedPdu_p, sdu_p,
                        pduHdrInfo_p, &availableSize, &incompleteSdu_p) ) )
        {
            LOG_RLC_MSG( LTE_RLC_ENCODE_UMD_RAW_PDU_ERR_4, LOGWARNING, 
                    (RLC_TX|RLC_UM), globalTTITickCount_g, __LINE__,
                    pE->ueIndex,  pE->lcId, availableSize, 0, 0, 0, __func__, "");
            /* Even if encoding fails, we will try to encode next SDU, so
               do not return here, continue to next SDU */
            /* SPR 3952 Fix Start */
            msgFree(sdu_p->data_p);
            sdu_p->data_p = RLC_NULL;
            /* SPR 3952 Fix End */
        }
        else
        {
#ifndef UE_SIM_TESTING
            if(sduCount < MAX_RLC_SDU_IN_PDU_FOR_KPI)
            {
                pktArrTime[sduCount] = sdu_p->pktArrTime;
                sduCount++;
            }
#endif
        }
        /* SPR 4497 Changes Start */           
        if(pduHdrInfo_p->fi == 0x00 || pduHdrInfo_p->fi == 0x02)
        {
            rlcQStatusBuffer_p[rlcStatusBufferCount].transId = sdu_p->transId;
            rlcQStatusBuffer_p[rlcStatusBufferCount].deliveryStatus = RLC_SUCCESS;
            rlcStatusBufferCount++;
#if defined(KPI_STATS) || defined(PERF_STATS)
            pOutPdu->pRawPdu.pdcpSduSize[pOutPdu->pRawPdu.completeEncodedSduCount] =
                (sdu_p->size - pdcpHdrSize );
            pOutPdu->pRawPdu.completeEncodedSduCount++;
#endif
        }
        /* SPR 4497 Changes End */            
        freeMemPool(sdu_p);
        /* A maximum of RLC_MAX_DFE_IN_PDU can be encoded in 1 PDU */
        if (pduHdrInfo_p->numDfe >= RLC_MAX_DFE_IN_PDU)
        {
            LOG_RLC_MSG( LTE_RLC_ENCODE_UMD_RAW_PDU_ERR_5, LOGWARNING, 
                    (RLC_TX|RLC_UM), globalTTITickCount_g, __LINE__,
                    pE->ueIndex,  pE->lcId, availableSize,
                    pduHdrInfo_p->numDfe, 0, 0, __func__, "");

            break;
        }
        /* Cyclomatic Complexity changes - starts here */
        if((pduHdrInfo_p->numDfe > 0)&& 
                (pduHdrInfo_p->dfeLength[pduHdrInfo_p->numDfe - 1] >
                 MAX_DATA_FIELD_SIZE))
        {
            LOG_RLC_MSG( LTE_RLC_ENCODE_UMD_RAW_PDU_ERR_6, LOGWARNING, 
                    (RLC_TX|RLC_UM), globalTTITickCount_g, __LINE__,
                    pE->ueIndex, pE->lcId, availableSize, pduHdrInfo_p->numDfe,
                    pduHdrInfo_p->dfeLength[pduHdrInfo_p->numDfe - 1],
                    0, __func__, "");
            /* last dfe added was greater than 2047, LI field cannot contain
               this value, hence it should be the last DFE in PDU */
            break;
        }
    }
    /* SPR 4497 Changes Start */    
    if( rlcStatusBufferCount )
    {
        CLOCK_START_RLC_API(RLC_DLVRY_STATUS_IND);

#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
		/*Dual Connectivity SeNB changes +*/
		if((SN_TERMINATED_SCG_SPLIT_BEARER ==  pE->dcBearerType) && (pE->lcId >= MIN_DRB_LC_ID))
		    sendRlcDeliveryStatusToX2U(
		            pE->ueIndex,
		            pE->lcId,
		            rlcStatusBufferCount,
		            rlcQStatusBuffer_p);
		else
		    /*Dual Connectivity SeNB changes -*/
#endif

        rlcDeliveryStatusInd(pE->ueIndex, pE->lcId, rlcStatusBufferCount,
                rlcQStatusBuffer_p);
        CLOCK_END_RLC_PDCP_API(RLC_DLVRY_STATUS_IND);
        gRlcStats.pRlcStats->lteRlcUeStat[pE->ueIndex].
            lcStats[pE->lcId].numDlvryInd += rlcStatusBufferCount;
    }
    else
    {
        freeMemPool(rlcQStatusBuffer_p);
    }

    /* Encode header */
    if (RLC_SUCCESS != (retVal = rlcEncodePduHeader(encodedPdu_p, pduHdrInfo_p, &szRawPdu) ))
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_UMD_RAW_PDU_ERR_7, LOGWARNING, 
                (RLC_TX|RLC_UM), globalTTITickCount_g, __LINE__,
                pE->ueIndex, pE->lcId, availableSize, 0, 0, 0, __func__, "");

        freeMemPool(pduHdrInfo_p);
        msgFree(encodedPdu_p);
        return retVal;
    }
    /** E Bit will be 1 only if RLC UM PDU have more than 1 SDUs  **/
    rlcSetFixedHdrUmdRawPdu(encodedPdu_p,(pduHdrInfo_p->fi),(pduHdrInfo_p->numDfe >1), pE->US, snLen);
    /* return formed PDU */
    pOutPdu->pRawPdu.copyType     = RLC_ZERO_COPY_BUFFER ;
    pOutPdu->pRawPdu.size         = 0 ;
    pOutPdu->pRawPdu.szUsed       = 0 ;
    pOutPdu->pRawPdu.data_p       = encodedPdu_p;
    pOutPdu->pRawPdu.payloadSize  = szRawPdu ;
    *txQueueSize_p               -= szRawPdu;
    *pAvailableSize               = szRawPdu + pduHdrInfo_p->hdrSize;

    /*updating for QOS */
    /** SPR 12498 Changes Start **/
    sQCount = QUEUE_COUNT_UMD_RAW_SDU_Q(*sduQ_p); 
    /*SPR 16936 fix*/
    if(sQCount > 0)
    {
        pE->headPktTS   =  ((RlcUeDataReq *)(sduQ_p->pQ->ring[sduQ_p->pQ->head].data))->pktArrTime;
    }
    pOutPdu->pRawPdu.headPktTS = pE->headPktTS;
    /*SPR 16936 fix*/
    /** SPR 12498 Changes End **/
#ifndef UE_SIM_TESTING
    *dfeCount = sduCount;
#endif

    LOG_RLC_MSG( LTE_RLC_ENCODE_UMD_RAW_PDU, LOGINFO, 
            (RLC_TX|RLC_UM), globalTTITickCount_g, __LINE__,
            pE->ueIndex,  pE->lcId, 
            *pAvailableSize, szRawPdu,0,0, __func__, "");

    freeMemPool(pduHdrInfo_p);

    return RLC_SUCCESS;
}
/****************************************************************************
 * Function Name  : rlcSetFixedHdrUmdRawPdu
 * Inputs         : base_data_p - a pointer to UMD PDU buffer,
 *                  fi - integer having Framing info,
 *                  e - integer having Extention field in fixed header,
 *                  sn - SN field,
 *                  snLen - number of bits in SN field 
 * Outputs        : None
 * Returns        : hdrLen/RLC_FAILURE
 * Description    : set Framing info(@fi) and E field (@e) value to UMD PDU
 *                  buffer.
 ****************************************************************************/
 UInt32 rlcSetFixedHdrUmdRawPdu( UInt8 *base_data_p,
                                              UInt8 fi,
                                              UInt8 e,
                                              UInt16 sn,
                                              UInt32 snLen
                                            )
{
    UInt32 hdrLen = 1;
    QSEGMENT segment = {0} ;
   if(RLC_NULL !=  msgSegNext(base_data_p, 0, &segment))
   {
       UInt8 *data_p  = segment.base;
       /*Coverity 94773 Fix Start*/
       if(data_p == RLC_NULL)
       {
           return RLC_FAILURE;
       }
       /*Coverity 94773 Fix End*/

       data_p[0] = 0;

       if(snLen == size5)
       {
           data_p[0] |= (fi << 6);
           data_p[0] |= (e << 5);
           data_p[0] |= sn ;
       }
       else
       {
           data_p[1] = 0;
           SET_BYTE_FIELD(data_p[0],fi,3,2) ;
           SET_BYTE_FIELD(data_p[0],e,2,1) ;
           SET_BYTE_FIELD(data_p[0],sn>>8,0,2) ;
           SET_BYTE_FIELD(data_p[1],sn,0,8) ;
           hdrLen++;
       }
       return hdrLen;	
   }
   else
       return RLC_FAILURE;
}


