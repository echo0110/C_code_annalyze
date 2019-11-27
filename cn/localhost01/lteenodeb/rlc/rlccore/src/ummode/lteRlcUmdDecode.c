/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcUmdDecode.c,v 1.1.1.1.12.2.2.2 2010/10/07 13:36:50 gur23054 Exp $
 *
 ****************************************************************************
 *
 *  File Description : this file describes the decoding of UMD PDU. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcUmdDecode.c,v $
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcUmArithmetic.h"
#include "lteRlcUmdDecode.h"
#include "logging.h"
#include "alarm.h"
#include "lteRlcRxProcessor.h"
#include "lteRlcErrorCode.h"

/****************************************************************************
 * Varibale Definition
 ****************************************************************************/
static inline UInt32 decodeAndEnqueueSegment( UmdDFEQ* umdDfeQ_p,
                                              UmdDFE *dfesSegment_p,
                                              RlcPduHeaderInfo *pduHdrInfo_p,
                                              UInt8 fi
#ifndef UE_SIM_TESTING
                                              /* SPR 15909 fix start */
                                              , tickType_t pktTimeStamp,
                                              /* SPR 15909 fix end */
                                              UInt8 spsState
#endif
                                            );  
/*
Value   Description
00      First byte of the Data field corresponds to the first byte of a RLC SDU.
        Last byte of the Data field corresponds to the last byte of a RLC SDU.
        
01      First byte of the Data field corresponds to the first byte of a RLC SDU.
        Last byte of the Data field does not correspond to the last byte of a RLC SDU.
        
10      First byte of the Data field does not correspond to the first byte of a RLC SDU.
        Last byte of the Data field corresponds to the last byte of a RLC SDU.
        
11      First byte of the Data field does not correspond to the first byte of a RLC SDU.
        Last byte of the Data field does not correspond to the last byte of a RLC SDU.
*/
 

#define FULL_RLC_SDU          0x00
#define FIRST_BYTE_OF_RLC_SDU 0x01
#define LAST_BYTE_OF_RLC_SDU  0x02
#define MID_BYTE_OF_RLC_SDU   0x03


/****************************************************************************
 * External Variable Includes
 ****************************************************************************/
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */

/****************************************************************************
 * Function Name  : rlcDecodeUmdH
 * Inputs         : rawPdu_p - a pointer to UmdRawPdu,
 *                  umdHdr_p - a pointer to  UmdH,
 *                  snLen - length(in bits) of SN Field
 * Outputs        : None
 * Returns        : RLC_SUCCESS if successfull else RLC_FAILURE
 * Description    : decodes header of rawPdu_p and fill UmdH accordingly.
 ****************************************************************************/
inline UInt32 rlcDecodeUmdH( UmdRawPdu *rawPdu_p, 
                             UmdH *umdHdr_p,
                             UInt8 snLen
                           ) 
{
    UInt32 ret         = RLC_SUCCESS;
    UInt8* data_p      = RLC_NULL; 
    UInt8 *base_data_p = RLC_NULL;
    /* + Coverity 32401 */
    umdHdr_p->snLen = (SnFieldLength) snLen; 
    /* - Coverity 32401 */
    umdHdr_p->sz = GET_SIZE_UMD_RAW_PDU(rawPdu_p);

    if(( data_p = rawPdu_p->pRawPdu.data_p) == RLC_NULL )
    {
        /* SPR 4010 Fix Start */
        LOG_RLC_MSG(LTE_RLC_DECODE_UMD_HEADER_ERR, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                snLen, umdHdr_p->sz, 0, 0, 0, 0, 
                __func__, "");
        /* SPR 4010 Fix End */
        return RLC_FAILURE;
    }

    QSEGMENT segment = {0};
    if ( PNULL == msgSegNext(data_p, 0, &segment) )
    {
        return RLC_FAILURE;
    }
    
    base_data_p  = segment.base;
    /*Coverity 94768 Fix Start*/
    if(base_data_p == RLC_NULL)
    {
        LOG_RLC_MSG(LTE_RLC_DECODE_UMD_HEADER_ERR, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                snLen, umdHdr_p->sz, 0, 0, 0, 0, 
                __func__, "");
        return RLC_FAILURE;
    }
    /*Coverity 94768 Fix End*/

    switch ( snLen)
    {
        case size5:
            /*
               Header Info  
               FI - 2 bytes
               Ext - 1 byte
               SN  - 5 byte

               0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
             *******************************
             FI   | E |          SN      | 
             *******************************

             Data ( Payload)
             *******************************
             */
            /* + Coverity 32400, 32399 */
            /*--coverity--530--54184*/
            umdHdr_p->fi = (RlcFi)((base_data_p[0] >> 6) & 0x3U) ;
            umdHdr_p->e  = (RlcExt)((base_data_p[0] >> 5) & 0x1U) ;
            umdHdr_p->sn = (RlcSn)((base_data_p[0]) & 0x1FU ); 
            /*--coverity--530--54184*/
            /* - Coverity 32400, 32399 */

            break; 

        case size10:

            /*
               Header Info
               R1 - 1 byte
               R2 - 1 byte
               R3 - 1 byte  
               FI - 2 bytes
               E  - 1 byte
               SN - 10 bytes

               0 | 1 | 2 | 3 | 4 | 5 | 6 | 7|
             *******************************
             R1| R2| R3|   FI  | E |   SN |
             *******************************
             |           SN                | 
             ****************************** 
             Data ( Payload)
             *******************************
             */


            /* + Coverity  32399, 55024*/
            umdHdr_p->fi = (RlcFi)((base_data_p[0] >> 3) & 0x3U) ;
            umdHdr_p->e =  (RlcExt)((base_data_p[0] >> 2) & 0x1U) ;
            umdHdr_p->sn = (RlcSn)((((base_data_p[0]) & 0x3U) << 8)|(base_data_p[1] & 0xFFU));
            /* - Coverity  32399, 55024 */

            break;

        default:

            LOG_RLC_MSG(LTE_RLC_DECODE_UMD_HEADER_ERR_1, LOGWARNING, 
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    snLen,  umdHdr_p->sz, 0, 0, 0, 0, 
                    __func__, "");

            ret = RLC_FAILURE;

            break;
    }    
    LOG_RLC_MSG(LTE_RLC_DECODE_UMD_HEADER, LOGINFO, 
            (RLC_RX), globalTTITickCount_g,
            snLen,  umdHdr_p->sz,umdHdr_p->fi, umdHdr_p->e, ret, 0, 0, 
            __func__, "");
    return ret ;
}

/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : rlcSetUmdDfeStausAndSegCount
 * Inputs         : umdHdr_p - a pointer to UmdH,
 *                  dfe_p - a pointer to UmdDFE,
 *                  rlcRxSegCounter - RLC RX Seg counter
 * Outputs        : None
 * Returns        : None
 * Description    : Set status of DFEs.
 ****************************************************************************/
static inline void rlcSetUmdDfeStausAndSegCount(
        UmdH * umdHdr_p,
        UmdDFE *dfe_p
#ifndef UE_SIM_TESTING
        ,UInt32 **rlcRxSegCounter
#endif
        )
{
#ifndef UE_SIM_TESTING
        if( umdHdr_p->fi == RLC_NO_SDU_SEGMENTED)    
        {
            **rlcRxSegCounter = 0;
        }
        else
        {
            (**rlcRxSegCounter)++;
        }
#endif
        switch(umdHdr_p->fi) 
        {
            case  RLC_DF_FB_NLB_MATCH:
                   dfe_p->status = PDU_MAP_SDU_START ;
            break;

            case RLC_DF_NFB_NLB_MATCH:
                   dfe_p->status = PDU_MAP_SDU_MID ;
            break;

            case  RLC_DF_NFB_LB_MATCH:
                   dfe_p->status = PDU_MAP_SDU_END ;
            break;
            default:
        
            LOG_RLC_MSG(LTE_RLC_DECODE_UMD_RAW_PDU, LOGDEBUG, 
                 (RLC_RX), globalTTITickCount_g, __LINE__,
                  umdHdr_p->sn, 0, 0, umdHdr_p->fi, 0, 0, 
                  __func__, "");
            break;
        }
}
/* Cyclomatic Complexity changes - ends here */

/****************************************************************************
 * Function Name  : rlcDecodeUmdRawPdu
 * Inputs         : rawPdu_p - a pointer to UmdRawPdu,
 *                  umdPdu_p - a pointer to UmdPdu
					umdHdr_p - a pointer to UmdH,
 *                  hIdx - a pointer to integer location having index to
 *                         next (E,LI) pair,
 *                  rlcRxSegCounter - RLC RX seg counter,
 *                  pktTimeStamp - Packet time stamp,
 *                  spsState - SPS State,
 *                  packetTime
 * Outputs        : umdPdu_p - a pointer to UmdPdu
 * Returns        : RLC_SUCCESS if successfull else RLC_FAILURE
 * Description    : decodes rawPdu_p and fill umdPdu_p accordingly.
 ****************************************************************************/
inline UInt32 rlcDecodeUmdRawPdu( UmdRawPdu *rawPdu_p,
                                  UmdPdu *umdPdu_p,
                                  UmdH * umdHdr_p,
                                  UInt8 hIdx
#ifndef UE_SIM_TESTING
                                  , UInt32 *rlcRxSegCounter
                                  /* SPR 15909 fix start */
                                  , tickType_t pktTimeStamp,
                                  /* SPR 15909 fix end */
                                  UInt8 spsState
#endif
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
    UInt32 dfIdx  = 0 ;
    UInt8* data_p = RLC_NULL;
    UmdDFE *dfe_p = RLC_NULL ;
    UInt32 szPdu  = 0;
    UInt32 szRemainingDF = 0 ;
    UmdH *umdH_p  =&(umdPdu_p->h);

    if(( data_p = rawPdu_p->pRawPdu.data_p) == RLC_NULL )
    {
        /* SPR 4010 Fix Start */
        LOG_RLC_MSG(LTE_RLC_DECODE_UMD_RAW_PDU_ERR, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                umdHdr_p->sn, hIdx, 0, 0, 0, 0, 
                __func__, "");
        /* SPR 4010 Fix End */
        return RLC_FAILURE;
    }

    /* Fill the header */
    umdH_p->sn = umdHdr_p->sn;


    UmdDFEQ *umdDfeQ_p = &umdPdu_p->df;

    szPdu = rawPdu_p->pRawPdu.size; 

    umdHdr_p->sz = szPdu - hIdx;

    if (umdHdr_p->sz == 0)
    {
        LOG_RLC_MSG(LTE_RLC_DECODE_UMD_RAW_PDU_ERR_1, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                umdHdr_p->sn, hIdx, szPdu, 0, 0, 0, 
                __func__, "");

        return RLC_FAILURE;
    }

    dfIdx = hIdx;

    /*SPR 20989 Fix*/
    if(rawPdu_p->pRawPdu.size < hIdx)
    {
        msgFree(rawPdu_p->pRawPdu.data_p);
        rawPdu_p->pRawPdu.data_p = RLC_NULL;
        return RLC_FAILURE;
    }
    /*SPR 20989 Fix*/


    if ( 0 == umdHdr_p->e )
    {
        /* only one DFE in Data Field */
        CREATE_UMD_DFE(dfe_p,0);
        /* coverity_24574 Fix Start */
        if(RLC_NULL == dfe_p)
        {
            return RLC_FAILURE;
        }
        /* coverity_24574 Fix End */
        msgFree(dfe_p->pDFE.data_p);
        /* SPR 20985 fix start */
        dfe_p->pDFE.data_p = RLC_NULL;
        /* SPR 20985 fix end */
        
        dfe_p->status      = PDU_MAP_SDU_FULL;
        dfe_p->pDFE.szUsed = umdHdr_p->sz ;
        dfe_p->pDFE.size   = umdHdr_p->sz ;
        dfe_p->pDFE.data_p = data_p;
#ifndef UE_SIM_TESTING        
        dfe_p->pktTimeStamp = pktTimeStamp;
        dfe_p->spsState = spsState;
#endif
        /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        dfe_p->packetTime = packetTime;
#endif
        /*32.425 Events code changes end*/
        /* Remove the Fixed Header */
        if( 0 == msgRemove(dfe_p->pDFE.data_p, 0, dfIdx) )
        {
            LOG_RLC_MSG(LTE_RLC_DECODE_UMD_RAW_PDU_ERR_2, LOGWARNING, 
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    umdHdr_p->sn, hIdx, szPdu, RLC_DECODE_MSG_REMOVE_ERR, 
                    0, 0, __func__, "");

            msgFree(dfe_p->pDFE.data_p);
            /* SPR 20985 fix start */
            dfe_p->pDFE.data_p = RLC_NULL;
            /* SPR 20985 fix end */
            freeMemPool(dfe_p);
/*SPR 17053 fix start*/
            rawPdu_p->pRawPdu.data_p = RLC_NULL;
/*SPR 17053 fix end*/
            return RLC_DECODE_MSG_REMOVE_ERR;
        }
        /* Cyclomatic Complexity changes - starts here */
        rlcSetUmdDfeStausAndSegCount(umdHdr_p,dfe_p
#ifndef UE_SIM_TESTING
                ,&rlcRxSegCounter
#endif
                );
        /* Cyclomatic Complexity changes - ends here */
        EN_QUEUE_UMDDFE_Q(umdDfeQ_p, dfe_p);
    }
    else
    {
        /* read (E,LI) pair and extract DFE accordingly */
        UInt32 dfeCount = 0 ;
        RlcPduHeaderInfo *pduHdrInfo_p = PNULL;

        /* Allocate Structure for storing header information */
        if ( (pduHdrInfo_p = getMemFromPool(sizeof(RlcPduHeaderInfo), RLC_NULL ) )
                == RLC_NULL)
        {
            LOG_RLC_MSG(LTE_RLC_DECODE_UMD_RAW_PDU_ERR_4, LOGFATAL, 
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    umdHdr_p->sn, hIdx, szPdu, RLC_DECODE_MEM_ALLOC_FAIL, 
                    0, 0, __func__, "");

            return RLC_DECODE_MEM_ALLOC_FAIL;
        }

        /* search Data Field index and count DFEs */
        dfeCount = rlcGetHeaderInfo( data_p,
                hIdx,
                pduHdrInfo_p
                );
        /* SPR 5144 Fix Start */
        if (RLC_FAILURE == dfeCount)
        {
            freeMemPool(pduHdrInfo_p);
            return RLC_FAILURE;
        }
        /* SPR 5144 Fix End */

        /*As per SPR 698, ltePanic takes care of the returning value, no change done*/				
        CREATE_UMD_DFE(dfe_p,0);
        /* coverity_24574 Fix Start */
        if(PNULL == dfe_p)
        {
            freeMemPool(pduHdrInfo_p);
            return RLC_FAILURE;
        }
        /* coverity_24574 Fix End */

        UInt32 szH = pduHdrInfo_p->hdrSize;
        /*Split header and DFE field*/
        msgSplit( data_p, szH, dfe_p->pDFE.data_p);
        /*Free header part*/

        /* Substract szH( Fixed+Variable header) from Total RLC PDU*/ 
        szRemainingDF = szPdu - szH;
        dfe_p->pDFE.szUsed = szRemainingDF ; 
        dfe_p->pDFE.size   = szRemainingDF ;
        /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        dfe_p->packetTime = packetTime;
#endif
        /*32.425 Events code changes end*/

        /* collecting all data filed elements from PDU */
        /*enqueue first dfe_p into DFE queue */
        if ((umdHdr_p->fi & LAST_BYTE_OF_RLC_SDU))
        {
            dfe_p->status = PDU_MAP_SDU_END ;
        }
        ret = decodeAndEnqueueSegment ( umdDfeQ_p, dfe_p, pduHdrInfo_p, umdHdr_p->fi
#ifndef UE_SIM_TESTING
                , pktTimeStamp,
                spsState
#endif
                );

        /* Cyclomatic Complexity changes - starts here */
        /* When ever this function returns failure, caller check and free the DFEs, so No
           need to free it twice */
        /* Cyclomatic Complexity changes - ends here */
        freeMemPool(pduHdrInfo_p);
/*SPR 17053 fix start*/
        if(ret != RLC_SUCCESS )
        {
           LOG_RLC_MSG(LTE_RLC_DECODE_UMD_RAW_PDU_ERR_5, LOGINFO, 
                     (RLC_RX), globalTTITickCount_g, __LINE__,
                   umdHdr_p->sn, hIdx, szPdu, ret, 
                    0, 0, __func__, "");

           return ret;
        }
        msgFree(data_p);
        /* SPR 20985 fix start */
        data_p = RLC_NULL;
        /* SPR 20985 fix end */
        /*SPR 17053 fix end*/
    }

    LOG_RLC_MSG(LTE_RLC_DECODE_UMD_RAW_PDU_ERR_5, LOGINFO, 
            (RLC_RX), globalTTITickCount_g, __LINE__,
            umdHdr_p->sn, hIdx, szPdu, ret, 
            0, 0, __func__, "");
    return ret ;
}

/*******************************************************************************
 * Function Name  : decodeAndEnqueueSegment
 * Inputs         : umdDfeQ_p - a pointer to UmdDFEQ,
 *                  dfesSegment_p - a pointer of DFEs Segment,
 *                  pduHdrInfo_p - a pointer to RlcPduHeaderInfo,
 *                  fi
 *                  pktTimeStamp - Packet time stamp,
 *                  spsState - State of SPS
 * Outputs        : None
 * Returns        : RLC_SUCCESS if successfull else RLC_FAILURE
 * Description    : Spilt next data field element (DFE). call this method
 *                  only when data field of UMD PDU contains DFE more than one.
 *******************************************************************************/

static inline UInt32 decodeAndEnqueueSegment( UmdDFEQ* umdDfeQ_p,
                                              UmdDFE *dfesSegment_p,
                                              RlcPduHeaderInfo *pduHdrInfo_p,
                                              UInt8 fi
#ifndef UE_SIM_TESTING
                                              /* SPR 15909 fix start */
                                              , tickType_t pktTimeStamp,
                                              /* SPR 15909 fix end */
                                              UInt8 spsState
#endif
                                            )
{
    UInt32 li          = 0 ;
    UmdDFE *dfe_p      = RLC_NULL ;
    UmdDFE *nextDFE_p  = RLC_NULL ;
    UInt32 dfeLoop     = 0;  
/* SPR 11864 Fix */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* SPR 15909 fix start */
    tickType_t packetTime = dfesSegment_p->packetTime;
    /* SPR 15909 fix end */
#endif
/* SPR 11864 Fix */

    do 
    {
        nextDFE_p = RLC_NULL ;
        li = pduHdrInfo_p->dfeLength[dfeLoop];

        if( li > dfesSegment_p->pDFE.szUsed )
        { 
            LOG_RLC_MSG(LTE_RLC_DECODE_AND_ENQ_SEG_ERR, LOGWARNING, 
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    dfeLoop, li, dfesSegment_p->pDFE.szUsed, 0, 0, 0, 
                    __func__, "");
           
            FREE_UMD_DFE(dfesSegment_p); 
            
            return RLC_DECODE_INVALID_SEG_LEN;
        }

        /*As per SPR 698, ltePanic takes care of the returning value, no change done*/
        CREATE_UMD_DFE(nextDFE_p, 0);
        /* coverity_24573 Fix Start */
        if(PNULL == nextDFE_p)
        {
            FREE_UMD_DFE(dfesSegment_p);
            return RLC_FAILURE;
        }
        /* coverity_24573 Fix End */

        msgSplit(dfesSegment_p->pDFE.data_p, li, nextDFE_p->pDFE.data_p);

        /* Remaining Payload */ 
        nextDFE_p->pDFE.szUsed = dfesSegment_p->pDFE.szUsed - li;
        nextDFE_p->pDFE.size   = nextDFE_p->pDFE.szUsed ;

        /* Split Part according to Length */
        dfesSegment_p->pDFE.szUsed = li ;
        dfesSegment_p->pDFE.size   = li ;

        /*Swap dfe pointer*/
        dfe_p = dfesSegment_p ;
        dfesSegment_p = nextDFE_p ;
#ifndef UE_SIM_TESTING        
        dfe_p->pktTimeStamp = pktTimeStamp;
        dfe_p->spsState = spsState;
#endif
/* SPR 11864 Fix */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        dfe_p->packetTime = packetTime;
#endif
/* SPR 11864 Fix */
        
        /* Enqueue the RLC SDU */
        EN_QUEUE_UMDDFE_Q(umdDfeQ_p, dfe_p);

        dfeLoop++;

        LOG_RLC_MSG(LTE_RLC_DECODE_AND_ENQ_SEG, LOGDEBUG, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                pduHdrInfo_p->numDfe, li, dfe_p->pDFE.size, nextDFE_p->pDFE.szUsed, 0, 0, 
                __func__, "");
        

    } while ( dfeLoop < pduHdrInfo_p->numDfe );

    if (( fi & FIRST_BYTE_OF_RLC_SDU))
    {
        dfesSegment_p->status = PDU_MAP_SDU_START ;
    }

    LOG_RLC_MSG(LTE_RLC_DECODE_AND_ENQ_LAST_SEGMENT, LOGDEBUG, 
            (RLC_RX), globalTTITickCount_g, __LINE__,
            dfeLoop, dfesSegment_p->pDFE.szUsed, fi, 0, 0, 0, 
            __func__, "");
	/* + Coverity 72823 */
    //nextDFE_p = RLC_NULL ;
	/* - Coverity 72823 */
#ifndef UE_SIM_TESTING        
        dfesSegment_p->pktTimeStamp = pktTimeStamp;
        dfesSegment_p->spsState = spsState;
#endif
/* SPR 11864 Fix */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        dfesSegment_p->packetTime = packetTime;
#endif
/* SPR 11864 Fix */

    /* For last part */
    EN_QUEUE_UMDDFE_Q(umdDfeQ_p, dfesSegment_p);

    return RLC_SUCCESS ;
}
