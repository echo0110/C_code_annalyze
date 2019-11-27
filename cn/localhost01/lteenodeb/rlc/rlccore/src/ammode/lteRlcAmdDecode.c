/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcAmdDecode.c,v 1.1.6.2.2.2 2010/10/07 13:34:06 gur23054 Exp $
 *
 ****************************************************************************
 *
 *  File Description : this file describes the decoding of AMD PDU.
 *
 *
 * 1. rlcDecodeAmdHeader
 * 2. rlcCheckStatusInReptBuffer 
 * 3. rlcInsertAmdSegmentInReptBuffer
 * 4. rlcDecodeAmdSegmentHeader 
 * 5. rlcCheckForDuplicateSegment
 * 6. rlcProcessAmdSegment
 * 7. rlcDecodeAmdRawPduSegment
 * 8. rlcDecodeAmdRawPdu
 * 9. decodeAndEnqueueAMSegment
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcAmdDecode.c,v $
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcAmArithmetic.h"
#include "lteRlcAmdDecode.h"
#include "lteQueue.h"
#include "lteRlcAmdArqOperation.h"
#include "logging.h"
#include "alarm.h"
#include "lteRlcErrorCode.h"
#include "lteRlcBuffer.h"
#include "lteRlcRxProcessor.h"
/****************************************************************************
 * External Variable
 ****************************************************************************/
extern void *zeroPool_g;
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */


static inline UInt32 decodeAndEnqueueAMSegment(
                                                AmdDFEQ *amdDfeQ_p,
                                                AmdDFE *dfesSegment_p,
                                                RlcPduHeaderInfo *pduHdrInfo_p,
                                                UInt8 fi
                                              );
static inline UInt32 insertAllDfeAndSegmentPdu( AmdDfeArrInfo *dfeInfo_p,
                                  AmdH * amdHdr_p,
                                  AmdPdu *amdPdu_p,
                                  AmdPduSegment *amdPduSegmentBase_p,
                                  UInt8 posToInsert
                                 );


/* SPR 4547 Changes Start */
RlcPduHeaderInfo rlcPduHeaderInfo_g = {0};
/* SPR 4547 Changes End */

/* Cyclomatic Complexity changes - starts here */

/****************************************************************************
 * Function Name  : rlcFreeAmdPduSegment
 * Inputs         : amdPduSeg_p - a pointer to AmdRawPdu
 * Outputs        : None
 * Returns        : None
 * Description    : Free DFEs received in Amd Psu Segment.
 ****************************************************************************/
inline void rlcFreeAmdPduSegment(AmdPduSegment *amdPduSeg_p)
{
    AmdDFE *amdDfe_p = RLC_NULL ;
    DEQUEUE_AMD_PDU_DFE_Q(amdPduSeg_p->dfeQ,amdDfe_p);
    while(amdDfe_p != RLC_NULL )
    {
        msgFree(amdDfe_p->pDFE.data_p) ;
        amdDfe_p->pDFE.data_p = RLC_NULL;
        freeMemPool(amdDfe_p);
        /* + SPR 5584 (COVERITY CID 25542) */
        amdDfe_p = PNULL;
        /* + SPR 5584 (COVERITY CID 25542) */
        DEQUEUE_AMD_PDU_DFE_Q(amdPduSeg_p->dfeQ,amdDfe_p);
    }
    /* + SPR 9646 Fix */
    semDestroy((LTE_SEM *)&(amdPduSeg_p->dfeQ.pQ->sem_lock));
    /* - SPR 9646 Fix */
    freeMemPool(amdPduSeg_p->dfeQ.pQ);
    amdPduSeg_p->dfeQ.pQ = RLC_NULL;
    freeMemPool(amdPduSeg_p);
}
/* Cyclomatic Complexity changes - ends here */

/****************************************************************************
 * Function Name  : rlcDecodeAmdHeader
 * Inputs         : rawPdu_p - a pointer to AmdRawPdu,
 *                  amdHdr_p - a pointer to  AmdH
 * Outputs        : None
 * Returns        : RLC_SUCCESS if successfull else RLC_FAILURE
 * Description    : decodes header of rawPdu_p and fill amdHdr_p accordingly.
 ****************************************************************************/

UInt32 rlcDecodeAmdHeader(AmdRawPdu *rawPdu_p,AmdH *amdHdr_p) 
{
    UInt8* data_p = RLC_NULL, *base_data_p = RLC_NULL;
    QSEGMENT segment = {0};
    UInt32 idx = 0;
    //,bitPos=RLC_BYTE;
    /************************************************************/
    if(( data_p = rawPdu_p->pRawPdu.data_p) == RLC_NULL )
    {
        LOG_RLC_MSG(LTE_RLC_DECODE_AMD_HEADER_ERR, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                RLC_FAILURE,0, 0,0, 0,0, __func__, "");

        return RLC_FAILURE;
    }
    /*************************************************************/    
    /*Get base pointer from ZCB pointer*/
    /** SPR 4913 Changes Start **/
    if(RLC_NULL == (msgSegNext(data_p, 0, &segment)))
    {
        LOG_RLC_MSG(LTE_RLC_DECODE_AMD_HEADER_ERR, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                RLC_FAILURE,*rawPdu_p->pRawPdu.data_p, segment.size,0, 0,0, __func__, "");
        return RLC_FAILURE;
    }
    base_data_p  = segment.base;
    if(base_data_p == RLC_NULL)
    {
        LOG_RLC_MSG(LTE_RLC_DECODE_AMD_HEADER_ERR, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                RLC_FAILURE,*rawPdu_p->pRawPdu.data_p,segment.size,0, 0,0, __func__, "");
        msgFree(rawPdu_p->pRawPdu.data_p);
        rawPdu_p->pRawPdu.data_p = RLC_NULL;
        return RLC_FAILURE;
    }
    /** SPR 4913 Changes End **/
    /* Get DC field */
    amdHdr_p->dc = ( base_data_p[idx] >> 7) & 0x1;

    if (  amdHdr_p->dc )
    {
        /* Get Re-segmentation Flag (RF) field */
        amdHdr_p->rf =  (base_data_p[idx] >> 6 ) & 0x01U;
        /* Get Polling bit (P) field */
        amdHdr_p->p  =  (base_data_p[idx] >> 5 ) & 0x01U;
        /* Get Framing Info (FI) field */
        amdHdr_p->fi =  (base_data_p[idx] >> 3 ) & 0x03U;
        /* Get Extension bit (E) field */
        amdHdr_p->e  =  (base_data_p[idx] >> 2 ) & 0x01U;
        /* Get Sequence Number (SN) field */
        amdHdr_p->sn = ( ( (base_data_p[idx] & 0x03U) << 8 )| (base_data_p[idx + 1] & 0xFFU) );

        /*Decode AMD PDU Segment*/    
        if ( amdHdr_p->rf )
        {
            /* Get Last Segment Flag (LSF) field */
            amdHdr_p->lsf =  (base_data_p[idx + 2] >> 7 ) & 0x01U;
            /* Get Segment Offset (SO) field */
           /* amdHdr_p->so  = ( ((base_data_p[idx + 2] << 8) & 0x3FU ) 
                    | (base_data_p[idx + 3] & 0xFFU ));*/
            amdHdr_p->so  = ( ((UInt16)(base_data_p[idx + 2] & 0x7FU ) << 8)
                               | (base_data_p[idx + 3] & 0xFFU )); 
        }
    }

    LOG_RLC_MSG(LTE_RLC_DECODE_AMD_HEADER_ERR_1, LOGINFO, 
            (RLC_RX), globalTTITickCount_g,  __LINE__ ,
            amdHdr_p->dc, amdHdr_p->rf, amdHdr_p->p, amdHdr_p->fi, 0,0, __func__, "");
    LOG_RLC_MSG(LTE_RLC_DECODE_AMD_HEADER_ERR_2, LOGINFO, 
            (RLC_RX), globalTTITickCount_g, __LINE__ ,
            amdHdr_p->e , amdHdr_p->sn ,amdHdr_p->lsf, amdHdr_p->so, 0,0, __func__, "");

    return RLC_SUCCESS;
}

/*SPR 11458 Changes Start*/
/****************************************************************************
 * Function Name  : checkAndUpdateFIStatusInfo
 * Inputs         : prevAmdPduSegment_p - a pointer to AmdPduSegment,
 *                  currentAmdPduSegment_p - a pointer to AmdPduSegment
 * Outputs        : None  
 * Returns        : None
 * Description    : Check the status of the AMD PDU SEGEMENT ( DFE), 
 *                  if it is not proper updating with proper value. 
 ****************************************************************************/
void checkAndUpdateFIStatusInfo(AmdPduSegment * prevAmdPduSegment_p,
                               AmdPduSegment * currentAmdPduSegment_p )
{

	AmdDFE *firstAmdDfeQ_p = RLC_NULL;
	AmdDFE *lastAmdDfeQ_p = RLC_NULL;

	LTE_QUEUE_NODE *pN = RLC_NULL;

	pN = getLastQueueNode(prevAmdPduSegment_p->dfeQ.pQ);
	lastAmdDfeQ_p = (AmdDFE *)MEMBEROF(AmdDFE,qN,pN);

	pN = getFirstQueueNode(currentAmdPduSegment_p->dfeQ.pQ);
	firstAmdDfeQ_p = (AmdDFE *) MEMBEROF(AmdDFE,qN,pN);

        if ( !firstAmdDfeQ_p || !lastAmdDfeQ_p)
	{
            return;
	}	

	if ( ( AM_PDU_MAP_SDU_FULL == lastAmdDfeQ_p->status) || (AM_PDU_MAP_SDU_END ==  lastAmdDfeQ_p->status))
	{
		if ( AM_PDU_MAP_SDU_MID == firstAmdDfeQ_p->status)
		{
			firstAmdDfeQ_p->status = AM_PDU_MAP_SDU_START;
		} 
		else if ( AM_PDU_MAP_SDU_END == firstAmdDfeQ_p->status)
		{
			firstAmdDfeQ_p->status = AM_PDU_MAP_SDU_FULL;
		}

	}

	if ( (AM_PDU_MAP_SDU_START == lastAmdDfeQ_p->status) || (AM_PDU_MAP_SDU_MID ==  lastAmdDfeQ_p->status))
	{
		if ( AM_PDU_MAP_SDU_START == firstAmdDfeQ_p->status)
		{
			firstAmdDfeQ_p->status = AM_PDU_MAP_SDU_MID;
		} 
		else if ( AM_PDU_MAP_SDU_FULL == firstAmdDfeQ_p->status)
		{
			firstAmdDfeQ_p->status = AM_PDU_MAP_SDU_END;
		}

	}

}
/*SPR 11458 Changes End*/

/****************************************************************************
 * Function Name  : rlcCheckStatusInReptBuffer
 * Inputs         : amdPdu_p - AMD PDU extracted from Ring,
 *                : amdHdr_p - AMD Segment Header
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Variables      : None
 * Description    : check status for updation of status in rept buffer. 
 ****************************************************************************/

UInt32 rlcCheckStatusInReptBuffer
(
    AmdPdu *amdPdu_p
    /* +- SPR 17777 */
)
{
    UInt32 lastSoEnd = 0;
    AmdPduSegment * amdPduSegment_p = RLC_NULL;

    /*SPR 11458 Changes Start*/
    AmdPduSegment * prevAmdPduSegment_p = RLC_NULL; 
    /*SPR 11458 Changes End*/
    /*TI_TICKOVERSHOOT Start*/
    //AmdDFEQ *amdPduSegmentQ_p = &amdPdu_p->pduSegmentQ;
    /* Find Number of nodes in Segment Queue */
    //UInt16 queueCntV = queueCount(amdPdu_p->pduSegmentQ.pQ);
    UInt16 queueCntV = queueCount(&(amdPdu_p->LQ));
    /*TI_TICKOVERSHOOT End*/
    while(queueCntV--)
    {
       /* get stored segment from Segment Queue */
        if (amdPduSegment_p)
        {
            /*TI_TICKOVERSHOOT Start*/
            amdPduSegment_p = (AmdPduSegment*)getNextQueueNode(&(amdPdu_p->LQ),&(amdPduSegment_p->qN));
            /* + coverity 24596 */
            if (RLC_NULL == amdPduSegment_p) 
            {
                LOG_RLC_MSG(LTE_RLC_CHECK_STATUS_REP_BUFF_ERR_1, LOGWARNING, 
                        (RLC_RX), globalTTITickCount_g, __LINE__,
                        RLC_FAILURE, 0, 0, 0, 0, 0, __func__, "");
                return RLC_FAILURE;
            }
            /* - coverity 24596 */
            /*TI_TICKOVERSHOOT End*/
            if (lastSoEnd != (amdPduSegment_p->soStart-1)) 
            {
             LOG_RLC_MSG(LTE_RLC_CHECK_STATUS_REP_BUFF_ERR, LOGWARNING, 
                     (RLC_RX), globalTTITickCount_g, __LINE__,
                      RLC_FAILURE, 0, 0, 0, 0, 0, __func__, "");
                return RLC_FAILURE; 
            }
            /*SPR 11458 Changes Start*/
            else
            {
                checkAndUpdateFIStatusInfo(  prevAmdPduSegment_p  ,   amdPduSegment_p);
            }
	        prevAmdPduSegment_p = amdPduSegment_p ;	   
            /*SPR 11458 Changes End*/
        }
        else
        {
            /*TI_TICKOVERSHOOT Start*/
            amdPduSegment_p = (AmdPduSegment*)getFirstQueueNode(&(amdPdu_p->LQ));
            /*TI_TICKOVERSHOOT End*/
           /*As per SPR 698 ,1203  change start*/
            if (RLC_NULL == amdPduSegment_p) 
            {
             LOG_RLC_MSG(LTE_RLC_CHECK_STATUS_REP_BUFF_ERR_1, LOGWARNING, 
                     (RLC_RX), globalTTITickCount_g, __LINE__,
                      RLC_FAILURE, 0, 0, 0, 0, 0, __func__, "");
                return RLC_FAILURE;
            }
            /*As per SPR 698 , 1203 change end*/
            if (amdPduSegment_p->soStart) 
            {
             LOG_RLC_MSG(LTE_RLC_CHECK_STATUS_REP_BUFF_ERR_2, LOGWARNING, 
                     (RLC_RX), globalTTITickCount_g, __LINE__,
                      RLC_FAILURE, 0, 0, 0, 0, 0, __func__, "");
                return RLC_FAILURE;
            }
            /*SPR 11458 Changes Start*/
	        prevAmdPduSegment_p = amdPduSegment_p ;	   
            /*SPR 11458 Changes End*/
        }
        lastSoEnd = amdPduSegment_p->soEnd;
       
    }
    if(amdPduSegment_p != RLC_NULL)
    {
        if(amdPduSegment_p->lsf)
        {
            return RLC_SUCCESS;
        }
    }
    return RLC_FAILURE;
}

/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : rlcFillDfesInfo
 * Inputs         : dfeInfo_p - Pointer to AmdDfeArrInfo,
 *                  soStart,
 *                  pduHdrInfo_p - Pointer to RlcPduHeaderInfo,
 *                  data_p -  - Pointer to data,
 *                  dfeCount - DFE Count,
 *                  amdHdr_p - Pointer to AmdH
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Variables      : None
 * Description    : check status for updation of status in rept buffer. 
 ****************************************************************************/
static inline UInt32 rlcFillDfesInfo(
    AmdDfeArrInfo *dfeInfo_p,
    UInt32 soStart,
    RlcPduHeaderInfo *pduHdrInfo_p,
    UInt8 **data_p,
    UInt32 dfeCount,
    AmdH * amdHdr_p)

{
    UInt32 i        = 0;
    /** SPR-6235 Changes Start **/
    UInt32 index = 0;
    /** SPR-6235 Changes End **/
    UInt8 *payload_p = *data_p;
    UInt8* tmpBuff_p = PNULL;       
        

    for ( i = 0 ; i < dfeCount - 1; i++) 
    {

        dfeInfo_p->numOfDfe++;
        /* + SPR 5498 */
        dfeInfo_p->dfeArr[i].lsf         =  0;
        /* - SPR 5498 */
        dfeInfo_p->dfeArr[i].nodeSoStart =  soStart;
        dfeInfo_p->dfeArr[i].li          =  pduHdrInfo_p->dfeLength[i];
        dfeInfo_p->dfeArr[i].nodeSoEnd   =  soStart + pduHdrInfo_p->dfeLength[i] - 1;

        if ( 0 != i ) /* Value of FI For First DFE already set */
        {
            dfeInfo_p->dfeArr[i].fi = 0x00;
        }
        /* Update the value of New SO Start */
        soStart = dfeInfo_p->dfeArr[i].nodeSoEnd + 1;

        dfeInfo_p->dfeArr[i].payload_p =(UInt8*) msgAlloc(zeroPool_g,0,0,0);
        if(!(dfeInfo_p->dfeArr[i].payload_p))
        {
            LOG_RLC_MSG(LTE_RLC_DECODE_ENQUE_SEG_ERR_1, LOGWARNING,
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    RLC_FAILURE, 0, 0, 0, 0, 0, __func__, "");
            /** SPR-6234 Changes Start **/
            /*+ SPR_17858_439_CHANGES*/
            msgFree(payload_p);
            data_p = RLC_NULL; // [YNK] 25,Mar.,2013: double free. payload_p has splitted from data_p
            payload_p = RLC_NULL;
            freeMemPool(pduHdrInfo_p);
            pduHdrInfo_p = RLC_NULL;

            while(i > 0)
            {
                msgFree(dfeInfo_p->dfeArr[i -1].payload_p);
                dfeInfo_p->dfeArr[i-1].payload_p = RLC_NULL;

                i--;
            }
             /*- SPR_17858_439_CHANGES*/
            return RLC_FAILURE;

        }
        if(!msgSplit(payload_p, dfeInfo_p->dfeArr[i].li, dfeInfo_p->dfeArr[i].payload_p))
        {
            LOG_RLC_MSG(LTE_RLC_DECODE_SEG_HDR_ERR, LOGWARNING,
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    dfeInfo_p->dfeArr[i].li , RLC_FAILURE, 0, 0, 0, 0, __func__, "");
            /** SPR-6235 Changes Start **/
            for(index = 0; index <= i ;index++)
            {
                msgFree(dfeInfo_p->dfeArr[index].payload_p);
                /* SPR 20985 fix start */
                dfeInfo_p->dfeArr[index].payload_p = RLC_NULL;
                /* SPR 20985 fix end */
            }
            freeMemPool(pduHdrInfo_p);
            return RLC_FAILURE;
            /** SPR-6235 Changes Start **/
        }
        tmpBuff_p = dfeInfo_p->dfeArr[i].payload_p;
        dfeInfo_p->dfeArr[i].payload_p = payload_p;
        payload_p = tmpBuff_p;
    }

    if(msgSize(payload_p,0) > 8190)
    {
        msgFree(payload_p);
        data_p = RLC_NULL;
        payload_p = RLC_NULL;
        freeMemPool(pduHdrInfo_p);
        for(index = 0; index <= i ;index++)
        {
            msgFree(dfeInfo_p->dfeArr[index].payload_p);
            dfeInfo_p->dfeArr[index].payload_p = RLC_NULL;
        }
        return RLC_FAILURE;
    }
    /* For Last DFE */
    /* Validate the lastPdu length  if ( msgSize(payload_p,0) === )*/
    dfeInfo_p->numOfDfe++;
    /* + SPR 5498 */
    dfeInfo_p->dfeArr[i].lsf          = 0;
    /* - SPR 5498 */
    dfeInfo_p->dfeArr[i].payload_p    = payload_p;
    dfeInfo_p->dfeArr[i].nodeSoStart  = soStart;
    dfeInfo_p->dfeArr[i].li           = msgSize(payload_p,0);
    dfeInfo_p->dfeArr[i].nodeSoEnd    = soStart + msgSize(payload_p,0) - 1;

    /* Setting the FI bit for Last DFE */
    dfeInfo_p->dfeArr[i].fi = amdHdr_p->fi & 0x01;

    return RLC_SUCCESS;

}
/* Cyclomatic Complexity changes - ends here */

/****************************************************************************
 * Function Name  : decodeAmdPduSegmentAndFillDfes
 * Inputs         : dfeInfo_p - Pointer to AmdDfeArrInfo,
 *                  amdHdr_p - Pointer to AmdH
 *                  data_p - Pointer to AMD Segment PDU,
 *                  payload_p - Pointer to payload,
 *                  packetTime
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : split payload and header and return index.
 ****************************************************************************/
#define VAR_HEADER_OFFSET_IN_PDU_SEGMENT 4
UInt32 decodeAmdPduSegmentAndFillDfes ( AmdDfeArrInfo *dfeInfo_p,
                                         AmdH * amdHdr_p,
                                         UInt8 *data_p,
                                         UInt8 *payload_p
                                         /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                                         /* SPR 15909 fix start */
                                         ,tickType_t packetTime
                                         /* SPR 15909 fix end */
#endif
                                         /*32.425 Events code changes end*/
        )
{
    UInt32 dfeCount = 0;
    UInt32 i        = 0;
    UInt32 soStart  = amdHdr_p->so;
    UInt32 ret = RLC_SUCCESS; 

    RlcPduHeaderInfo *pduHdrInfo_p;
    
    /* Only Fix Header added in AMD PDU Segment */
    if ( 0 == amdHdr_p->e )
    {
        if( 0 == msgRemove( data_p, 0, 4) )
        {
            msgFree(data_p );
            /*+ SPR_17858_439_CHANGES*/
            data_p = RLC_NULL;
            msgFree(payload_p);
            payload_p = RLC_NULL;
            /*- SPR_17858_439_CHANGES*/

            return RLC_FAILURE;
        }
           
        if(msgSize( data_p,0) > 8190)
        {
            msgFree(data_p );
            data_p = RLC_NULL;
            msgFree(payload_p);
            payload_p = RLC_NULL;

            return RLC_FAILURE;
        }
        dfeInfo_p->numOfDfe++;
        dfeInfo_p->dfeArr[i].payload_p   = data_p;
        dfeInfo_p->dfeArr[i].nodeSoStart = soStart;
        dfeInfo_p->dfeArr[i].li          = msgSize( data_p,0);
        dfeInfo_p->dfeArr[i].nodeSoEnd   = soStart + msgSize( data_p, 0) - 1;
        dfeInfo_p->dfeArr[i].fi          = amdHdr_p->fi;
        /* + SPR 5498 */
        dfeInfo_p->dfeArr[i].lsf         = 0;
        /* - SPR 5498 */
        /* Updating the so End */
        amdHdr_p->soEnd = soStart + msgSize(data_p, 0) - 1;
        /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        dfeInfo_p->packetTime = packetTime;
#endif
        /*32.425 Events code changes end*/
        /** SPR 13131 Changes Start **/
        msgFree(payload_p);
        payload_p = RLC_NULL;
        /** SPR 13131 Changes End **/

    } /* Variable Header in AMD PDU Segment */
    else if ( 1 == amdHdr_p->e)/* 1 == amdHdr_p->e */
    {
		/* Allocating Structure for storing header information */
		if ( (pduHdrInfo_p = getMemFromPool(sizeof(RlcPduHeaderInfo), RLC_NULL ) )
					== RLC_NULL)
		{
            /*+ SPR_17858_439_CHANGES*/
            msgFree(data_p );
            data_p = RLC_NULL;
            msgFree(payload_p);
            payload_p = RLC_NULL;
            /*- SPR_17858_439_CHANGES*/

			LOG_MSG(LTE_RLC_PROCESS_AMD_SEGMENT_ERR_3, LOGWARNING,
					(RLC_RX), globalTTITickCount_g, __LINE__,
					RLC_FAILURE, 0, 0, 0, 0, 0, __func__, "");
			return RLC_FAILURE;
		}
          
        /* rlcGetHeaderInfo decode the variable header and calculate the total header Size */ 
        dfeCount = rlcGetHeaderInfo(  data_p, VAR_HEADER_OFFSET_IN_PDU_SEGMENT, pduHdrInfo_p);

		/* Remove the header part from Received RLC AMD PDU Segment*/
		/*******************************************************/
        payload_p = msgSplit( data_p, pduHdrInfo_p->hdrSize, payload_p);
        if (!dfeCount || !payload_p)
        {
            /* SPR 5144 Fix start */
            /*+ SPR_17858_439_CHANGES*/
            msgFree(data_p );
            data_p = RLC_NULL;
            msgFree(payload_p);
            payload_p = RLC_NULL;
            /*- SPR_17858_439_CHANGES*/

            freeMemPool(pduHdrInfo_p);
  /* SPR 5144 Fix End */
            lteWarning("Invalid Scenario 0 dfe Count or payload_p[%x]\n", payload_p);
            return RLC_FAILURE;
        }
		/* Free the header Part */
	    msgFree(data_p);
        /* SPR 20985 fix start */    
        data_p = RLC_NULL;
        /* SPR 20985 fix end */
		/***********************************************/
        /* Updating the so End */
        amdHdr_p->soEnd = soStart + msgSize(payload_p, 0) - 1;

        /* Setting the FI value for First DFE element */
        dfeInfo_p->dfeArr[i].fi = amdHdr_p->fi & 0x02;
        /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        dfeInfo_p->packetTime = packetTime;
#endif
        /*32.425 Events code changes end*/

        /* Filling DFE Array and its payload */ 
        /************************************************/      
        /* Cyclomatic Complexity changes - starts here */
        ret = rlcFillDfesInfo(dfeInfo_p,soStart,pduHdrInfo_p,&payload_p,
                              dfeCount,amdHdr_p);
        if( ret != RLC_SUCCESS)
        {
            return RLC_FAILURE;
        }
        /* Cyclomatic Complexity changes - ends here */
        freeMemPool(pduHdrInfo_p); 
    }  
    else
    {
        lteWarning("Invalid Value in amdHdr_p->e [%d]\n", amdHdr_p->e);
    }

    if (amdHdr_p->lsf)
    {
        dfeInfo_p->dfeArr[i].lsf = 1;
    }
    return RLC_SUCCESS; 
}   

/****************************************************************************
 * Function Name  : discardRxPduSegmentFromStart
 * Inputs         : dfeInfo_p - Pointer to AmdDfeArrInfo,
 *                  soStart,
 *                  amdHdrSoStart
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This function split the PDU and discard the 
 *                  RX PDU payload from start 
 ****************************************************************************/

UInt8 discardRxPduSegmentFromStart( AmdDfeArrInfo *dfeInfo_p,
                              UInt16 soStart,
                              UInt16 amdHdrSoStart
                            )
{

   UInt32 i = 0;
   UInt16 totalLen = amdHdrSoStart;
   UInt8 *data_p = 0;
   UInt32 idx  = 0;
   
   for ( i = 0; i < dfeInfo_p->numOfDfe; i++)
   {
       totalLen += dfeInfo_p->dfeArr[i].li;

	   if ( soStart < totalLen )
	   {
          idx = dfeInfo_p->dfeArr[i].li - ( totalLen - soStart);
		  data_p =(UInt8*) msgAlloc(zeroPool_g,0,0,0);
		  if ( !data_p)
		  {
		     lteWarning("Msg Alloc Failed");
			 return RLC_FAILURE;
		  }
		  msgSplit( dfeInfo_p->dfeArr[i].payload_p, idx, data_p);
		  msgFree( dfeInfo_p->dfeArr[i].payload_p);
		  dfeInfo_p->dfeArr[i].payload_p = data_p;	
          dfeInfo_p->dfeArr[i].li = totalLen - soStart;
		  break;
	   }
	   else
	   {
	      msgFree(dfeInfo_p->dfeArr[i].payload_p);
		  dfeInfo_p->dfeArr[i].payload_p = PNULL;
          dfeInfo_p->dfeArr[i].li = 0;
          if ( soStart == totalLen )
              break;
	   }
   }
   return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : discardRxPduSegmentFromEnd
 * Inputs         : dfeInfo_p - Pointer to AmdDfeArrInfo,
 *                  soEnd,
 *                  amdHdrSoEnd
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This function split the PDU and discard the 
 *                  RX PDU payload from end
 ****************************************************************************/

UInt8 discardRxPduSegmentFromEnd( AmdDfeArrInfo *dfeInfo_p,
                                              UInt16 soEnd,
                                              UInt16 amdHdrSoEnd
                                            )
{
    UInt32 i        = 0;
    /* SPR 3872 fix start */
    UInt32 totalLen = amdHdrSoEnd +1;
    /* SPR 3872 fix end */
    UInt32 remainLen = soEnd + 1;
    UInt8 *data_p   = 0;
    UInt32 idx      = 0;

    /* SPR 3872 fix start */
    for ( i = dfeInfo_p->numOfDfe; i > 0; i--)
    {
        totalLen -= dfeInfo_p->dfeArr[i - 1].li;
		  
        if ( remainLen > totalLen )
	{
	    idx = (remainLen - totalLen) ;
	    data_p =(UInt8*) msgAlloc(zeroPool_g,0,0,0);
	    if (!data_p)
	    {
	        lteWarning("Msg Alloc Failed");
                return RLC_FAILURE;
            }
            msgSplit( dfeInfo_p->dfeArr[i -1].payload_p, idx, data_p);
            msgFree( data_p);
           /* SPR 20985 fix start */
            data_p = RLC_NULL;
           /* SPR 20985 fix end */
            dfeInfo_p->dfeArr[i -1].li = idx;
            /*Coverity ID <65227> Fix Changes Start*/
            /* SPR 10722 changes starts*/
            dfeInfo_p->dfeArr[i-1].fi = (dfeInfo_p->dfeArr[i-1].fi | 0x01) ; 
            /* SPR 10722 changes ends*/
            break;
            /*Coverity ID <65227> Fix Changes End*/
       }
       else 
       {
            msgFree( dfeInfo_p->dfeArr[i -1].payload_p);
            dfeInfo_p->dfeArr[i -1].payload_p = PNULL;
            dfeInfo_p->dfeArr[i -1].li        = 0;
    /* SPR 3872 fix end */
            if ( remainLen == totalLen )
            break;
       }
    }
    return RLC_SUCCESS;
}

/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : rlcFreeAmPduSegDfes
 * Inputs         : dfeInfo_p - pointer to AmdDfeArrInfo
 * Outputs        : None
 * Returns        : None
 * Description    : This function will free all DFEs of received data segment.
 ****************************************************************************/
static inline void rlcFreeAmPduSegDfes(
        AmdDfeArrInfo *dfeInfo_p)
{
    UInt32 count = 0;
    for (;count < dfeInfo_p->numOfDfe; count++)
    {
        msgFree(dfeInfo_p->dfeArr[count].payload_p);
        dfeInfo_p->dfeArr[count].payload_p = PNULL;
        dfeInfo_p->dfeArr[count].li = 0;
        dfeInfo_p->dfeArr[count].lsf = 0;
        dfeInfo_p->dfeArr[count].nodeSoEnd = 0;
        dfeInfo_p->dfeArr[count].nodeSoStart = 0;
    }
}

/* Cyclomatic Complexity changes - ends here */

/****************************************************************************
 * Function Name  : handleDuplicateSegment
 * Inputs         : dfeInfo_p - Pointer to AmdDfeArrInfo,
 *                  amdHdr_pp - Pointer to AmdH,
 *                  amdPdu_p - Pointer to AmdPdu,
 *                  queueCntV
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This function hadle the duplicate segment PDU 
 ****************************************************************************/
UInt32 handleDuplicateSegment(  AmdDfeArrInfo *dfeInfo_p,
                                AmdH * amdHdr_p,
                                AmdPdu *amdPdu_p,
                                UInt32 queueCntV
                             )
{
    AmdPduSegment * amdPduSegment_p = RLC_NULL;
    AmdPduSegment * tmpAmdPduSegment_p = RLC_NULL;
    UInt16 currentPduSoStart = 0;
    UInt16 currentPduSoEnd   = 0;;
    UInt8 posToInsert        = 0;
    UInt8 preGetNextNode     = RLC_FALSE;
    UInt32 ret               = RLC_SUCCESS;
    /*TI_TICKOVERSHOOT Start*/
    //AmdDFEQ *amdPduSegmentQ_p       = &amdPdu_p->pduSegmentQ;
    /*TI_TICKOVERSHOOT End*/
    /* Loop until all nodes are processed */
  
	/* + Coverity 25542 */
      amdPduSegment_p = (AmdPduSegment*)
                getFirstQueueNode(&(amdPdu_p->LQ));
        /* + coverity 24595 */
      if (RLC_NULL == amdPduSegment_p) 
      {
          LOG_RLC_MSG(LTE_RLC_CHECK_STATUS_REP_BUFF_ERR_1, LOGWARNING, 
                  (RLC_RX), globalTTITickCount_g, __LINE__,
                  RLC_FAILURE, 0, 0, 0, 0, 0, __func__, "");
          return RLC_FAILURE;
      }
        /* - coverity 24595 */
      preGetNextNode = RLC_TRUE; //pre get next node for first queue node.

    do{
	/* - Coverity 25542 */
        currentPduSoStart = amdHdr_p->so;
		currentPduSoEnd   = amdHdr_p->soEnd;

        /* Recvd current RLC AM PDU Segment Start less than Previous Recieved SO start*/
        if ( currentPduSoStart < amdPduSegment_p->soStart )
        {
            if ( currentPduSoEnd < amdPduSegment_p->soStart )
            {
                /* No need to discard */
				posToInsert = 0;
				ret=insertAllDfeAndSegmentPdu( dfeInfo_p,
                                           amdHdr_p,
                                           amdPdu_p,
                                           amdPduSegment_p,
                                           posToInsert);
				break;
            }
            else if ( currentPduSoEnd >  amdPduSegment_p->soEnd )
            {
                // discard Middle part of current PDU  ( amdPduSegment_p->soEnd - amdPduSegment_p->soStart)
                /* Adding two segement 1. before 2. after the existing PDU segment*/
                 /*TI_TICKOVERSHOOT Start*/
/* SPR 10722 */
		/*fix bug:115667 by dbc*/
                if( RLC_TRUE == preGetNextNode )
                {
                    tmpAmdPduSegment_p = (AmdPduSegment*) getNextQueueNode(&(amdPdu_p->LQ), &(amdPduSegment_p->qN));
                }
                else
                {
                    tmpAmdPduSegment_p = (AmdPduSegment*) getPrevQueueNode(&(amdPduSegment_p->qN));
                }
                /*fix bug:115667 by dbc**/
/* SPR 10722 */
                 delQueueNode(&(amdPdu_p->LQ), &(amdPduSegment_p->qN));
                 //delQueueNode(amdPduSegmentQ_p->pQ, &(amdPduSegment_p->qN));
                 /*TI_TICKOVERSHOOT End*/
                 /* Cyclomatic Complexity changes - starts here */
                 rlcFreeAmdPduSegment(amdPduSegment_p);
/* SPR 10722 */ 
                 amdPduSegment_p = tmpAmdPduSegment_p;
                 tmpAmdPduSegment_p = RLC_NULL;
/* SPR 10722 */
                 /* Cyclomatic Complexity changes - ends here */
                 /* check that more segment present before calling this function*/
                /** SPR 12832 Changes Start **/
                if ( (queueCntV - 1) == 0)
                {
                    /* If q have only 1 node and that too discarded due to 
                     * above condition, new node will insert at head. */
                    posToInsert = 1;
		    ret=insertAllDfeAndSegmentPdu( dfeInfo_p,
				    amdHdr_p,
				    amdPdu_p,
				    /* SPR 20812 Start */
				    amdPduSegment_p,
				    /* SPR 20812 End */
				    posToInsert);
                    break;
                }
                else
                {
					if (amdPduSegment_p)
                    {
			/*fix bug:115667 by dbc*/
                        if( RLC_FALSE == preGetNextNode )
                        {
                        /*TI_TICKOVERSHOOT Start*/
                        amdPduSegment_p = (AmdPduSegment*)
                            getNextQueueNode(&(amdPdu_p->LQ),&(amdPduSegment_p->qN));
                        /*TI_TICKOVERSHOOT End*/
                        }
                        else
                        {
                            /*reset preGetNextNode*/
                            preGetNextNode = RLC_FALSE;
                        }
                        /*fix bug:115667 by dbc*/

                    }
                    /* Assuming that Segment SO Start is adding in ascending order. */   
                    /* + coverity 24595 */
                    if (RLC_NULL == amdPduSegment_p) 
                    {
                        LOG_RLC_MSG(LTE_RLC_CHECK_STATUS_REP_BUFF_ERR_1, LOGWARNING, 
                                (RLC_RX), globalTTITickCount_g, __LINE__,
                                RLC_FAILURE, 0, 0, 0, 0, 0, __func__, "");
                        break;
                    }
					/** SPR 12832 Changes End **/
					continue;
                }
            } 
            else if ( currentPduSoEnd <=  amdPduSegment_p->soEnd )//include condition discard only 1 byte 
            {
                // discard End part of current PDU (amdPduSegment_p->soStart + currentPduSoEnd)  
                posToInsert = 0;
				discardRxPduSegmentFromEnd( dfeInfo_p, amdPduSegment_p->soStart - 1,currentPduSoEnd  );
				amdHdr_p->soEnd = amdPduSegment_p->soStart - 1;

/* SPR 10722 */
                amdHdr_p->lsf = 0; 
/* SPR 10722 */
				ret=insertAllDfeAndSegmentPdu( dfeInfo_p,
                                           amdHdr_p,
                                           amdPdu_p,
                                           amdPduSegment_p,
                                           posToInsert);
				break;
            } 
            else 
            {
                lteWarning("Invalid Scenario 1 currentPduSoStart [%d] currentPduSoEnd[%d] amdPduSegSoStart[%d] amdPduSegSoEnd[%d]\n",
					currentPduSoStart, currentPduSoEnd, amdPduSegment_p->soStart, amdPduSegment_p->soEnd);
            }

        }


        // fixing end part of new PDU segment
        else if ( currentPduSoEnd > amdPduSegment_p->soEnd)
        {
            if (  currentPduSoStart > amdPduSegment_p->soEnd )
            {
                /* No need to Discard */
				/* check that more segment present before calling this function*/
    /* SPR 8081 fix start */
				if ( (queueCntV -1) == 0)
    /* SPR 8081 fix end */
				{
				    posToInsert = 1;
				    ret=insertAllDfeAndSegmentPdu( dfeInfo_p,
                                           amdHdr_p,
                                           amdPdu_p,
                                           amdPduSegment_p,
                                           posToInsert);
					break;
				}
				else 
				{
    /* SPR 8081 fix start */
                    /* get stored segment from Segment Queue */
                    if (amdPduSegment_p)
                    {
                        /*TI_TICKOVERSHOOT Start*/
                        amdPduSegment_p = (AmdPduSegment*)
                            getNextQueueNode(&(amdPdu_p->LQ),&(amdPduSegment_p->qN));
                        /*TI_TICKOVERSHOOT End*/
                    }
                    /* Assuming that Segment SO Start is adding in ascending order. */   
                    /* + coverity 24595 */
                    if (RLC_NULL == amdPduSegment_p) 
                    {
                        LOG_RLC_MSG(LTE_RLC_CHECK_STATUS_REP_BUFF_ERR_1, LOGWARNING, 
                                (RLC_RX), globalTTITickCount_g, __LINE__,
                                RLC_FAILURE, 0, 0, 0, 0, 0, __func__, "");
                        break;
                    }

    /* SPR 8081 fix end */
					   continue;
				}
            }
            else if ( currentPduSoStart <= amdPduSegment_p->soEnd) //include condition discard only 1 byte
            {
                //discard start part    ( currentPduSoStart   to  amdPduSegment_p->soEnd)
				discardRxPduSegmentFromStart( dfeInfo_p, /*New Pdu Start from here*/ amdPduSegment_p->soEnd + 1 , currentPduSoStart );
				amdHdr_p->so = amdPduSegment_p->soEnd + 1;

    /* SPR 8081 fix start */
				if ( (queueCntV -1) == 0)
    /* SPR 8081 fix end */
                {
                    posToInsert = 1;
				    ret=insertAllDfeAndSegmentPdu( dfeInfo_p,
                                           amdHdr_p,
                                           amdPdu_p,
                                           amdPduSegment_p,
                                           posToInsert);
				   break;
				}
				else
				{
    /* SPR 8081 fix start */
                    /* get stored segment from Segment Queue */
                    if (amdPduSegment_p)
                    {
                        /*TI_TICKOVERSHOOT Start*/
                        amdPduSegment_p = (AmdPduSegment*)
                            getNextQueueNode(&(amdPdu_p->LQ),&(amdPduSegment_p->qN));
                        /*TI_TICKOVERSHOOT End*/
                    }
                    /* + coverity 24595 */
                    if (RLC_NULL == amdPduSegment_p) 
                    {
                        LOG_RLC_MSG(LTE_RLC_CHECK_STATUS_REP_BUFF_ERR_1, LOGWARNING, 
                                (RLC_RX), globalTTITickCount_g, __LINE__,
                                RLC_FAILURE, 0, 0, 0, 0, 0, __func__, "");
                        break;
                    }


    /* SPR 8081 fix end */
                    continue;
				}				    	
            }
            else
            {
                lteWarning("Invalid Scenario 2 currentPduSoStart [%d] currentPduSoEnd[%d]  	amdPduSegSoStart[%d] amdPduSegSoEnd[%d]\n",
					currentPduSoStart, currentPduSoEnd, amdPduSegment_p->soStart, amdPduSegment_p->soEnd);            
            }

        } 
        else if ( (currentPduSoStart >= amdPduSegment_p->soStart)  &&  (currentPduSoEnd  <=  amdPduSegment_p->soEnd) )
        {
            /* SPR 3991 Fix Start */
            /* Cyclomatic Complexity changes - starts here */
            rlcFreeAmPduSegDfes(dfeInfo_p);
            /* Cyclomatic Complexity changes - ends here */
            break;
            /* SPR 3991 Fix End */
            
        }
        else 
        {
            lteWarning("Invalid Scenario 3 currentPduSoStart [%d] currentPduSoEnd[%d] amdPduSegSoStart[%d] amdPduSegSoEnd[%d]\n",
					currentPduSoStart, currentPduSoEnd, amdPduSegment_p->soStart, amdPduSegment_p->soEnd);
        }

	/* + Coverity 25542 */
        /* get stored segment from Segment Queue */
        if (amdPduSegment_p)
        {
            /*TI_TICKOVERSHOOT Start*/
            amdPduSegment_p = (AmdPduSegment*)
                getNextQueueNode(&(amdPdu_p->LQ),&(amdPduSegment_p->qN));
            /*TI_TICKOVERSHOOT End*/
        }
        /* Assuming that Segment SO Start is adding in ascending order. */   
        /* + coverity 24595 */
        if (RLC_NULL == amdPduSegment_p) 
        {
            LOG_RLC_MSG(LTE_RLC_CHECK_STATUS_REP_BUFF_ERR_1, LOGWARNING, 
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    RLC_FAILURE, 0, 0, 0, 0, 0, __func__, "");
            return RLC_FAILURE;
        }
        /* - coverity 24595 */
    }while(--queueCntV && amdPduSegment_p );
	/* - Coverity 25542 */
    return ret;
}
/****************************************************************************
 * Function Name  : insertAllDfeAndSegmentPdu 
 * Outputs        : dfeInfo_p - Pointer to AmdDfeArrInfo,
 *                  amdHdr_p - Pointer to AmdH,
 *                  amdPdu_p - Pointer to AmdPdu,
 *                  amdPduSegmentBase_p - Pointer to AmdPduSegment,
 *                  posToInsert - Position to Insert
 * Returns        : RLC_SUCCESS if successfull else RLC_FAILURE
 * Description    : This function append the DFE and segment to  RLC am pdu 
 ****************************************************************************/

static inline UInt32 insertAllDfeAndSegmentPdu( AmdDfeArrInfo *dfeInfo_p,
                                                AmdH * amdHdr_p,
                                                AmdPdu *amdPdu_p,
                                                AmdPduSegment *amdPduSegmentBase_p,
                                                UInt8 posToInsert
                                              )
{ 
    UInt32 i = 0;

    AmdPduSegment * amdPduSegment_p = RLC_NULL;
    AmdDFEQ *amdDfeQ_p = RLC_NULL;
    AmdDFE *dfe_p      = RLC_NULL;
    /*TI_TICKOVERSHOOT Start*/
    //AmdDFEQ *amdPduSegmentQ_p = &amdPdu_p->pduSegmentQ;
    /*TI_TICKOVERSHOOT End*/
    /* Create AMD PDU Segment */
    CREATE_AMD_PDU_SEGMENT(amdPduSegment_p);
    if(amdPduSegment_p == RLC_NULL)
    {
        return RLC_FAILURE;
    }
    amdDfeQ_p = &(amdPduSegment_p->dfeQ);

    for ( i = 0; i < dfeInfo_p->numOfDfe; i++)
    { 
        if ( PNULL == dfeInfo_p->dfeArr[i].payload_p ||
             0     == dfeInfo_p->dfeArr[i].li   
           )
        {
           /* These segment has been already recieved*/
           continue;
        }
        CREATE_AMD_DFE(dfe_p);

        /* Coverity 25479 Fix Start */
        if(RLC_NULL == dfe_p)
        {
            rlcFreeAmdPduSegment(amdPduSegment_p);
            return RLC_FAILURE;
        }
        /* Coverity 25479 Fix End */

        dfe_p->pDFE.size   = dfeInfo_p->dfeArr[i].li; 
        dfe_p->pDFE.data_p = dfeInfo_p->dfeArr[i].payload_p;
        /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        dfe_p->packetTime = dfeInfo_p->packetTime;
#endif
        /*32.425 Events code changes end*/

        switch ( dfeInfo_p->dfeArr[i].fi)
        {
            case 0x00:
                 dfe_p->status = AM_PDU_MAP_SDU_FULL;
				break;
            case  0x01:
                dfe_p->status = AM_PDU_MAP_SDU_START ;
                break;

            case 0x03:
                dfe_p->status = AM_PDU_MAP_SDU_MID ;
                break;

            case 0x02:
                dfe_p->status = AM_PDU_MAP_SDU_END ;
                break;

            default:

                break;
        }
        
        ENQUEUE_AMD_PDU_DFE_Q(amdDfeQ_p, dfe_p);
    }
	/* Updating the value in the AMD PDU Segment */
    amdPduSegment_p->soStart = amdHdr_p->so;
    amdPduSegment_p->soEnd   = amdHdr_p->soEnd;
    amdPduSegment_p->lsf = amdHdr_p->lsf;

    /*TI_TICKOVERSHOOT Start*/
	/* Check where to append the node in to list */
	 posToInsert?
			 queueInsertAfter( &(amdPdu_p->LQ), (void *)amdPduSegmentBase_p, (void *)amdPduSegment_p)
		   : queueInsertBefore ( &(amdPdu_p->LQ), (void *)amdPduSegmentBase_p, (void *)amdPduSegment_p);
	 /*posToInsert?
			 queueInsertAfter( amdPduSegmentQ_p->pQ, (void *)amdPduSegmentBase_p, (void *)amdPduSegment_p)
		   : queueInsertBefore ( amdPduSegmentQ_p->pQ, (void *)amdPduSegmentBase_p, (void *)amdPduSegment_p);*/
    /*TI_TICKOVERSHOOT End*/
			   
    /* Set the slot status */
     /* +- SPR 17777 */
     if ( RLC_SUCCESS != rlcCheckStatusInReptBuffer(amdPdu_p))
     /* +- SPR 17777 */
        {
            amdPdu_p->status  = PDU_AM_SEGMENT;
        }
        else
        {
            amdPdu_p->status   = PDU_AM_SEGMENT_COMPLETE;
        }
  
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : handleRlcAmdPduSegment 
 * Inputs         : rawPdu_p - a pointer to AmdRawPdu,
 *                  amdPdu_p - a pointer of AmdPdu,
 *                  amdHdr_p - a pointer of AmdH,
 *                  packetTime
 * Outputs        : amdPdu_p - a pointer to AmdPdu
 * Returns        : RLC_SUCCESS if successfull else RLC_FAILURE
 * Description    : Check Duplicate segment bytes and populate reception buff.
 ****************************************************************************/

UInt32 handleRlcAmdPduSegment ( 
                                 AmdRawPdu *rawPdu_p,
                                 AmdPdu *amdPdu_p,
                                 AmdH * amdHdr_p
                                 /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                                 /* SPR 15909 fix start */
                                 ,tickType_t packetTime
                                 /* SPR 15909 fix end */
#endif
                                 /*32.425 Events code changes end*/
        )   
{
    UInt32 errorCode = INVALID_ERR_CODE;
    /* + SPR 5498 */
    AmdDfeArrInfo *dfeInfo_p = PNULL;
    /* - SPR 5498 */
    AmdPduSegmentInfo segmentInfo = {0};
    /* Allocate ZCB Pointer */
    segmentInfo.payload_p =(UInt8*)msgAlloc(zeroPool_g,0,0,0);
    if(!segmentInfo.payload_p) 
    {
        /*+ SPR_17858_439_CHANGES*/
        msgFree(rawPdu_p->pRawPdu.data_p);
        rawPdu_p->pRawPdu.data_p = RLC_NULL;
        /*- SPR_17858_439_CHANGES*/

        LOG_MSG(LTE_RLC_DECODE_ENQUE_SEG_ERR_1, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                RLC_FAILURE, 0, 0, 0, 0, 0, __func__, "");
        return RLC_FAILURE;      

    }
    /* + SPR 5498 */
    if ( (dfeInfo_p = getMemFromPool(sizeof(AmdDfeArrInfo), RLC_NULL ) )
            == RLC_NULL)
    {
        msgFree(segmentInfo.payload_p);
        return RLC_FAILURE;
    }
    dfeInfo_p->numOfDfe = 0;
    /* - SPR 5498 */

    /* Decode Amd PDU Segment header and Fill the dfe arrary */
    /* + SPR 5498 */
    errorCode = decodeAmdPduSegmentAndFillDfes( dfeInfo_p, 
            amdHdr_p,
            rawPdu_p->pRawPdu.data_p,
            segmentInfo.payload_p
            /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
            ,packetTime
#endif
            /*32.425 Events code changes end*/
            );
    if ( errorCode != RLC_SUCCESS )
    {
        /* SPR 5144 Fix Start */
        msgFree(segmentInfo.payload_p);
        freeMemPool(dfeInfo_p);
        /* - SPR 5498 */
        /* Uplink ThroughPut Fix Start */
        /*
        ** Msg has been freed in decodeAmdPduSegmentAndFillDfes()
        ** for all failure scenarios.
        */
        //msgFree(rawPdu_p->pRawPdu.data_p);
        /* Uplink ThroughPut Fix End */
        rawPdu_p->pRawPdu.data_p = RLC_NULL;   
        /* SPR 5144 Fix End */
        lteWarning("decoding of AMD PDU Segment failed \n");
        return errorCode;
    } 

    /* Now Check for Duplicate Segement if Duplicate then discard the PDU
       otherwise  Insert the segement in Pdu Segment Q*/

    /*TI_TICKOVERSHOOT Start*/
    /* Find Number of nodes in Segment Queue (in Ring) */
    UInt16 queueCntV = queueCount(&(amdPdu_p->LQ));
    //UInt16 queueCntV = queueCount(amdPdu_p->pduSegmentQ.pQ);
    /*TI_TICKOVERSHOOT End*/

    /* SPR 3993 Fix Start */
    if (amdHdr_p->lsf)
    {
        amdPdu_p->lsf = amdHdr_p->lsf;
    }
    /* SPR 3993 Fix End */
    if ( !queueCntV ) /* No need to check for duplicate */
    { 
        /* + SPR 5498 */
        errorCode = insertAllDfeAndSegmentPdu(dfeInfo_p, amdHdr_p, amdPdu_p, PNULL, 0);  
    }
    else
    {
        errorCode = handleDuplicateSegment(dfeInfo_p, amdHdr_p, amdPdu_p, queueCntV ); 
    }
    freeMemPool(dfeInfo_p);
    if ( errorCode != RLC_SUCCESS )
    {
        msgFree(segmentInfo.payload_p);
        msgFree(rawPdu_p->pRawPdu.data_p);
        rawPdu_p->pRawPdu.data_p = RLC_NULL;
        lteWarning("Mem alloc Failed\n");
        return RLC_FAILURE;
    }
    /* - SPR 5498 */
   return RLC_SUCCESS;

}


 /* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : rlcSetAmdPduAndDfeStatus
 * Inputs         : amdHdr_p - a pointer of AmdH,
 *                  amdPdu_p - a pointer to AmdPdu,
 *                  dfe_p- a pointer to dfe
 * Outputs        : None
 * Returns        : None
 * Description    : Set status of received data pdu and dfes.
 ****************************************************************************/
static inline void rlcSetAmdPduAndDfeStatus
(
    AmdH   *amdHdr_p,
    AmdPdu *amdPdu_p,
    AmdDFE *dfe_p
    )
{
    if ( amdHdr_p->rf == 0x01 &&  amdHdr_p->lsf == 0x00) 
    {
         amdPdu_p->status = PDU_AM_SEGMENT; 
    }
    else if ( amdHdr_p->rf == 0x01 && amdHdr_p->lsf == 0x01)
    {
         amdPdu_p->status = PDU_AM_SEGMENT_COMPLETE;
    }

    switch ( amdHdr_p->fi)
    {
         case  0x01:
            dfe_p->status = AM_PDU_MAP_SDU_START ;
             break;

         case 0x03:
            dfe_p->status = AM_PDU_MAP_SDU_MID ;
             break;

         case 0x02:
            dfe_p->status = AM_PDU_MAP_SDU_END ;
            break;

         default:
            break;
    }
}
/* Cyclomatic Complexity changes - ends here */


/****************************************************************************
 * Function Name  : rlcDecodeAmdRawPdu
 * Inputs         : rawPdu_p - a pointer to AmdRawPdu,
 *                  amdPdu_p - a pointer of AmdPdu ,
 *                  amdHdr_p - a pointer of AmdH,
 * Outputs        : amdPdu_p - a pointer to AmdPdu
 * Returns        : RLC_SUCCESS if successfull else RLC_FAILURE
 * Description    : decodes rawPdu_p and fill amdPdu_p accordingly.
 ****************************************************************************/

inline UInt32 rlcDecodeAmdRawPdu
(
    AmdRawPdu *rawPdu_p,
    AmdPdu *amdPdu_p,
    AmdH * amdHdr_p
    /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* SPR 15909 fix start */
    ,tickType_t packetTime
    /* SPR 15909 fix end */
#endif
    /*32.425 Events code changes end*/
    )
{
    UInt32 ret    = RLC_SUCCESS ;
    UInt32 hIdx   = 2;
    UInt32 dfIdx  = 0 ;
    AmdDFE *dfe_p = RLC_NULL ;
    UInt32 szPdu  = 0;
    UInt32 size   = 0;

    UInt32 szRemainingDF = 0;
    UInt8* data_p      = RLC_NULL;
    AmdDFEQ *amdDfeQ_p = RLC_NULL;
    AmdPduSegment * amdPduSegment_p = RLC_NULL;
    
    /** SPR-6232 Changes Start **/
    UInt16 queueCntV = queueCount(&(amdPdu_p->LQ));
    AmdPduSegment * tempAmdPduSegment_p = RLC_NULL;
    /** SPR-6232 Changes End **/

    /*TI_TICKOVERSHOOT Start*/
    //AmdDFEQ *amdPduSegmentQ_p = &amdPdu_p->pduSegmentQ;
    /*TI_TICKOVERSHOOT End*/

    szPdu  = rawPdu_p->pRawPdu.size;
    data_p = rawPdu_p->pRawPdu.data_p;
    size   = szPdu - hIdx ;
    dfIdx  = hIdx ;

    /*SPR 20986 fix*/
    if(rawPdu_p->pRawPdu.size < hIdx )
    {
        msgFree(rawPdu_p->pRawPdu.data_p);
        rawPdu_p->pRawPdu.data_p = RLC_NULL;
        return RLC_FAILURE;
    }
    /*SPR 20986 fix*/

    CREATE_AMD_PDU_SEGMENT(amdPduSegment_p);
    if(amdPduSegment_p == RLC_NULL)
    {
        LOG_RLC_MSG(LTE_RLC_HDL_PDU_ERR_3, LOGWARNING,
                (RLC_RX), globalTTITickCount_g, __LINE__,
                0,0,0,0,0,0, __func__, "");
        msgFree(data_p);
        rawPdu_p->pRawPdu.data_p = RLC_NULL;
        /* SPR 20985 fix start */
        data_p = RLC_NULL;
            /* SPR 20985 fix end */
            return RLC_FAILURE;
    }

    amdDfeQ_p = &(amdPduSegment_p->dfeQ);


    if ( amdHdr_p->e == 0)
    {
        if(size > 8190)
        {
            rlcFreeAmdPduSegment(amdPduSegment_p);
            msgFree(data_p);
            rawPdu_p->pRawPdu.data_p = RLC_NULL;
            /* SPR 20985 fix start */
            data_p = RLC_NULL;
            /* SPR 20985 fix end */
            return RLC_FAILURE;
        }

        CREATE_AMD_DFE(dfe_p);
        /* coverity 25480 Fix start */
        if(RLC_NULL == dfe_p)
        {
            rlcFreeAmdPduSegment(amdPduSegment_p);
            msgFree(data_p);
            rawPdu_p->pRawPdu.data_p = RLC_NULL;
            /* SPR 20985 fix start */
            data_p = RLC_NULL;
                /* SPR 20985 fix end */
                return RLC_FAILURE;
        }
        /* coverity 25480 Fix End */
        dfe_p->pDFE.size   = size ;
        dfe_p->pDFE.data_p = data_p;

    /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        dfe_p->packetTime = packetTime;
#endif
    /*32.425 Events code changes end*/
        if( 0 == msgRemove(dfe_p->pDFE.data_p, 0, dfIdx) )
        {
            LOG_RLC_MSG(LTE_RLC_PROCESS_AMD_SEGMENT_ERR_1, LOGWARNING, 
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    0, 0, 0, 0, 0, 0, __func__, "");
            
            msgFree(dfe_p->pDFE.data_p );
            rawPdu_p->pRawPdu.data_p = RLC_NULL;
            /* Cyclomatic Complexity changes - starts here */
            rlcFreeAmdPduSegment(amdPduSegment_p);
            /* Cyclomatic Complexity changes - ends here */
            /* Coverity Fix 25504 End*/
            freeMemPool(dfe_p);
            return RLC_FAILURE;
        }
        /* Cyclomatic Complexity changes - starts here */
        rlcSetAmdPduAndDfeStatus(amdHdr_p,amdPdu_p,dfe_p);
        /* Cyclomatic Complexity changes - ends here */
        ENQUEUE_AMD_PDU_DFE_Q(amdDfeQ_p, dfe_p);

        LOG_RLC_MSG(LTE_RLC_PROCESS_AMD_SEGMENT_ERR_7, LOGINFO, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                amdHdr_p->e, dfe_p->status, amdHdr_p->lsf , amdHdr_p->rf, 0, 0, __func__, "");
    }
    else 
    {
        /* read (E,LI) pair and extract DFE accordingly */
        UInt32 dfeCount = 0;
        UInt32 szHeader = 0;
        RlcPduHeaderInfo *pduHdrInfo_p;
        /* Allocate Structure for storing header information */

/* SPR 4547 Changes Start */
        pduHdrInfo_p = (RlcPduHeaderInfo *)&rlcPduHeaderInfo_g;
/* SPR 4547 Changes End */

                dfeCount = rlcGetHeaderInfo( data_p,
                hIdx,
                pduHdrInfo_p
                );

        if (!dfeCount)
        {
            LOG_RLC_MSG(LTE_RLC_PROCESS_AMD_SEGMENT_ERR_4, LOGWARNING, 
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    dfeCount, RLC_FAILURE,  0, 0, 0, 0, __func__, "");
            //msgFree(data_p);
            /* Cyclomatic Complexity changes - starts here */
            rlcFreeAmdPduSegment(amdPduSegment_p);
            /* Cyclomatic Complexity changes - ends here */
            return RLC_FAILURE;
        }

        CREATE_AMD_DFE(dfe_p);
        /* coverity_25480 Fix Start */
        if(PNULL == dfe_p)
        {
            LOG_RLC_MSG(LTE_RLC_DECODE_ENQUE_SEG_ERR_1, LOGWARNING, 
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    RLC_FAILURE, 0, 0, 0, 0, 0, __func__, "");
            rlcFreeAmdPduSegment(amdPduSegment_p);
            return RLC_FAILURE;
        }
        /* coverity_25480 Fix End */

        dfe_p->pDFE.data_p =(UInt8*)msgAlloc(zeroPool_g,0,0,0);
        if ( dfe_p->pDFE.data_p == RLC_NULL)
        {
            LOG_RLC_MSG(LTE_RLC_DECODE_ENQUE_SEG_ERR_1, LOGWARNING, 
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    RLC_FAILURE, 0, 0, 0, 0, 0, __func__, "");
            /* Cyclomatic Complexity changes - starts here */
            rlcFreeAmdPduSegment(amdPduSegment_p);
            /* Cyclomatic Complexity changes - ends here */
            freeMemPool(dfe_p);
            //msgFree(data_p);
            return RLC_FAILURE;      

        }
        szHeader =  pduHdrInfo_p->hdrSize; 
        /*Split header and DFE field*/
        szRemainingDF = szPdu - szHeader;

        if(!msgSplit(data_p, szHeader, dfe_p->pDFE.data_p))
        {
            LOG_RLC_MSG(LTE_RLC_DECODE_SEG_HDR_ERR, LOGWARNING, 
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    szHeader , RLC_FAILURE, 0, 0, 0, 0, __func__, "");
            msgFree(dfe_p->pDFE.data_p);
            /* SPR 20985 fix start */
            dfe_p->pDFE.data_p = RLC_NULL;
            /* SPR 20985 fix end */
            //msgFree(data_p);
            /* Cyclomatic Complexity changes - starts here */
            rlcFreeAmdPduSegment(amdPduSegment_p);
            /* Cyclomatic Complexity changes - ends here */
            freeMemPool(dfe_p);
            return RLC_FAILURE;      
        }

        dfe_p->pDFE.size = szRemainingDF ;
        /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        dfe_p->packetTime = packetTime;
#endif
        /*32.425 Events code changes end*/

        if((amdHdr_p->fi & 0x02))
        {
            dfe_p->status = AM_PDU_MAP_SDU_END;
        }
        ret = decodeAndEnqueueAMSegment ( amdDfeQ_p, dfe_p, pduHdrInfo_p, amdHdr_p->fi );

        if ( RLC_SUCCESS != ret )
        {
            LOG_RLC_MSG(LTE_RLC_PROCESS_AMD_SEGMENT_ERR_5, LOGWARNING, 
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    ret, 0,  0, 0, 0, 0, __func__, "");
            //msgFree(data_p);
            //freeMemPool(dfe_p);   
            /* Cyclomatic Complexity changes - starts here */
            rlcFreeAmdPduSegment(amdPduSegment_p);
            /* Cyclomatic Complexity changes - ends here */
            return ret;

        }
        /*Free header part*/
        msgFree(data_p);
        /* SPR 20985 fix start */
        data_p = RLC_NULL;        
        /* SPR 20985 fix end */

        LOG_RLC_MSG(LTE_RLC_PROCESS_AMD_SEGMENT_ERR_6, LOGINFO, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                amdHdr_p->e, amdHdr_p->fi,  dfe_p->pDFE.size, szHeader, 0, 0, __func__, "");
    }

    /** SPR-6232 Changes Start **/
    if(queueCntV)
    {
        DEQUEUE_AMD_PDU_SEGMENT_Q(amdPdu_p,tempAmdPduSegment_p);
        while(tempAmdPduSegment_p && (queueCntV--))
        {
            rlcFreeAmdPduSegment(tempAmdPduSegment_p);
            DEQUEUE_AMD_PDU_SEGMENT_Q(amdPdu_p,tempAmdPduSegment_p);
        }
    }
    /** SPR-6232 Changes End **/

    /*TI_TICKOVERSHOOT Start*/
    ENQUEUE_AMD_PDU_SEGMENT_Q( amdPdu_p, amdPduSegment_p);
    //ENQUEUE_AMD_PDU_SEGMENT_Q( amdPduSegmentQ_p, amdPduSegment_p);
    /*TI_TICKOVERSHOOT End*/
    return ret ;
} 

/*******************************************************************************
 * Function Name  : decodeAndEnqueueAMSegment
 * Inputs         : amdDfeQ_p - a pointer to AmdDFEQ,
 *                  dfesSegment_p - Pointer to AmdDFE,
 *                  pduHdrInfo_p - Pointer to RlcPduHeaderInfo,
 *                  fi
 * Outputs        : None
 * Returns        : RLC_SUCCESS if successfull else RLC_FAILURE
 * Description    : Spilt next data field element (DFE). call this method
 *                  only when data field of AMD PDU contains DFE more than one.
 *******************************************************************************/
static inline UInt32 decodeAndEnqueueAMSegment( 
                                                AmdDFEQ *amdDfeQ_p,
                                                AmdDFE *dfesSegment_p,
                                                RlcPduHeaderInfo *pduHdrInfo_p,
                                                UInt8 fi
                                              )
{
    UInt32 li          = 0 ;
    AmdDFE *dfe_p      = RLC_NULL ;
    AmdDFE *nextDFE_p  = RLC_NULL ;
    UInt32 i           = 0;
    /*SPR 11812 Fix Start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* SPR 15909 fix start */
    tickType_t packetTime = dfesSegment_p->packetTime;
    /* SPR 15909 fix end */
#endif
    /*SPR 11812 Fix End*/
    do 
    {
        li =  pduHdrInfo_p->dfeLength[i];

        if ( li > dfesSegment_p->pDFE.size )
        {
             LOG_RLC_MSG(LTE_RLC_DECODE_ENQUE_SEG_ERR, LOGWARNING, 
                     (RLC_RX), globalTTITickCount_g, __LINE__,
                      li, dfesSegment_p->pDFE.size, RLC_FAILURE, 0, 0, 0, __func__, "");
             msgFree((dfesSegment_p)->pDFE.data_p);
            /* SPR 20985 fix start */
            (dfesSegment_p)->pDFE.data_p = RLC_NULL;        
            /* SPR 20985 fix end */
             freeMemPool(dfesSegment_p);   
            return RLC_FAILURE;
        }
        /*As per SPR 698 ,ltePanic takes care of returning value, no change done*/
        CREATE_AMD_DFE(nextDFE_p);

        /* coverity_24562 Fix Start */
        if(PNULL == nextDFE_p)
        {
             LOG_RLC_MSG(LTE_RLC_DECODE_ENQUE_SEG_ERR_1, LOGWARNING, 
                     (RLC_RX), globalTTITickCount_g, __LINE__,
                     RLC_FAILURE, 0, 0, 0, 0, 0, __func__, "");
            msgFree((dfesSegment_p)->pDFE.data_p);
            /* SPR 20985 fix start */
            (dfesSegment_p)->pDFE.data_p = RLC_NULL;        
            /* SPR 20985 fix end */
            freeMemPool(dfesSegment_p);   
            return RLC_FAILURE;
        }
        /* coverity_24562 Fix Start */

        nextDFE_p->pDFE.data_p = (UInt8*)msgAlloc(zeroPool_g,0,0,0);
        if(nextDFE_p->pDFE.data_p == RLC_NULL)
        {
             LOG_RLC_MSG(LTE_RLC_DECODE_ENQUE_SEG_ERR_1, LOGWARNING, 
                     (RLC_RX), globalTTITickCount_g, __LINE__,
                     RLC_FAILURE, 0, 0, 0, 0, 0, __func__, "");
             msgFree((dfesSegment_p)->pDFE.data_p);
            /* SPR 20985 fix start */
            (dfesSegment_p)->pDFE.data_p = RLC_NULL;        
            /* SPR 20985 fix end */
             freeMemPool(nextDFE_p);
             freeMemPool(dfesSegment_p);   
             return RLC_FAILURE;
        }

        /*Split DFE field*/
        if(!msgSplit((dfesSegment_p)->pDFE.data_p, li, nextDFE_p->pDFE.data_p))
        {
             LOG_RLC_MSG(LTE_RLC_DECODE_SEG_HDR_ERR, LOGWARNING, 
                     (RLC_RX), globalTTITickCount_g, __LINE__,
                     li , RLC_FAILURE, 0, 0, 0, 0, __func__, "");
             msgFree((dfesSegment_p)->pDFE.data_p);
             msgFree(nextDFE_p->pDFE.data_p);
            /* SPR 20985 fix start */
            (dfesSegment_p)->pDFE.data_p = RLC_NULL;
            nextDFE_p->pDFE.data_p = RLC_NULL;
            /* SPR 20985 fix end */
             freeMemPool(nextDFE_p);
             freeMemPool(dfesSegment_p);   
             return RLC_FAILURE;      
        }     

        nextDFE_p->pDFE.size = (dfesSegment_p)->pDFE.size - li;

        (dfesSegment_p)->pDFE.size = li ;
     /*SPR 11812 Fix Start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
	 dfesSegment_p->packetTime= packetTime;
#endif
    /*SPR 11812 Fix End*/
        /*Swap dfe pointer*/
        dfe_p = dfesSegment_p ;
        dfesSegment_p = nextDFE_p ;
        nextDFE_p = RLC_NULL ;

        ENQUEUE_AMD_PDU_DFE_Q(amdDfeQ_p,dfe_p);        

        i++;
    } while ( i < pduHdrInfo_p->numDfe );

    /* Rel 5.3: Coverity 32372 Fix Start */
    dfesSegment_p->status =  (RlcSduStatus)(fi & 0x01);
    /* Rel 5.3: Coverity 32372 Fix End */
     /*SPR 11812 Fix Start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    dfesSegment_p->packetTime = packetTime;
#endif
    /*SPR 11812 Fix End*/
    /*enqueue last dfe into DFE queue */
    if(dfesSegment_p->pDFE.size > 8190)
    {
        msgFree((dfesSegment_p)->pDFE.data_p);
        (dfesSegment_p)->pDFE.data_p = RLC_NULL;
        freeMemPool(dfesSegment_p);
        return RLC_FAILURE;
   }
    ENQUEUE_AMD_PDU_DFE_Q(amdDfeQ_p, dfesSegment_p);  

    return RLC_SUCCESS ;
}
