/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpDataTransferRx.c,v 1.1.1.1.14.2.2.2 2010/10/25 08:32:31 gur21010 Exp $
 *
 ******************************************************************************
 *
 *  File Description : Data transfer RX routines implementation.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpDataTransferRx.c,v $
 * Revision 1.1.1.1.14.2.2.2  2010/10/25 08:32:31  gur21010
 * Warnings Removed
 *
 * Revision 1.1.1.1.14.2.2.1  2010/09/21 15:49:14  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.11.2.6  2009/07/20 08:21:55  gur19479
 * Changes done for SRB handling
 *
 * Revision 1.11.2.5  2009/07/16 06:47:45  gur19140
 * downlink fixes
 *
 * Revision 1.11.2.4  2009/07/13 11:00:11  gur20435
 *
 * PDCP Optimizations
 *
 * Revision 1.11.2.3  2009/07/10 09:07:18  gur19140
 * optimization changes for zcb
 *
 * Revision 1.11.2.2  2009/07/07 11:37:15  gur19140
 * *** empty log message ***
 *
 * Revision 1.11.2.1  2009/06/09 11:33:51  gur19140
 * *** empty log message ***
 *
 * Revision 1.11  2009/05/28 16:26:52  gur19140
 * *** empty log message ***
 *
 * Revision 1.10  2009/05/26 14:57:17  gur19140
 * *** empty log message ***
 *
 * Revision 1.9  2009/05/22 09:20:52  gur19836
 * ROHC Changes merged
 *
 * Revision 1.8  2009/05/21 07:10:46  gur19140
 * memory leak fixes
 *
 * Revision 1.7  2009/05/20 13:40:56  gur20548
 * header file name changed
 *
 * Revision 1.6  2009/05/15 18:43:24  gur19140
 * fixes
 *
 * Revision 1.5  2009/05/15 11:01:41  gur19836
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
#include "ltePdcpLteFramework.h"
#include "rohc_ex.h"
#include "rohc_typ.h"
#include "ltePdcpLteDefs.h"
#include "ltePdcpTSDefs.h"
#include "ltePdcpErrors.h"
#include "ltePdcpDataTransferRx.h"
#include "ltePdcpRlcSap.h"
#include "ltePdcpRrcSap.h"
#include "ltePdcpPdcpUserSap.h"
/*ECN changes start*/
#include "ltePdcpRRMUtils.h"
#include "ltePdcpTlvUtils.h"
/*ECN chnages end*/
#include "integrity.h"
#include "ltePdcpStatistic.h"
#include "rohc_main_ex.h"
#include "confidentiality.h"
#include "logging.h"
#include "alarm.h"
#include "ltePdcpExecutionLegs.h"
#include "ltePdcpMacSap.h"
#ifdef PDCP_ASYNC_INTERFACE
#include "ltePdcpEncSap.h"
#endif
#include "lteCommonStatsManager.h"
/* + KPI_CA */
#ifdef KPI_STATS
#include "ltePdcpContext.h"
#endif
/* - KPI_CA */

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
#ifndef PDCP_ASYNC_INTERFACE
/* SPR_1349_FIX_START*/
/* SPR 2498 changes start */
static UInt32 pdcpToDecipherRx(LP_PdcpDataInd dataInd_p,LP_PdcpEnityContext ctx_p);
/* SPR 2498 changes end */
/* SPR_1349_FIX_END*/
static inline UInt32 pdcpDecipherMultipleSegRxPdu( UInt16 algoId, UInt32 size,
       UInt32 cipherSize, LP_PdcpDataInd dataInd_p, 
       LP_PdcpEnityContext ctx_p);
#endif
/* SPR 5303 changes start */
/* + KPI_CA */
static UInt32 pdcpProcessRx_DRB_AM(LP_PdcpDataInd dataInd_p,
        LP_PdcpEnityContext ctx_p,UInt16 sn
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
		, InternalCellIndex internalCellIndex
#endif
		);
/* - KPI_CA */
/* SPR 5303 changes end */
static void pdcpTransferUlPacketFromRSN(LP_PdcpEnityContext ctx_p, 
        LP_PdcpDataInd currDataInd_p );
static void pdcpTransferUlPacketTillRSN( LP_PdcpEnityContext ctx_p, 
        LP_PdcpDataInd currDataInd_p );

#ifndef UE_SIM_TESTING
static UInt32 pdcpSpsConfigRecvd(UInt16 ueIndex, UInt8 lcId,
        UInt32 state, UInt32 event, LP_PdcpUeContext ueCtx_p, 
        LP_PdcpSpsIntervalProfile spsIntProf_p, InternalCellIndex intCellIdx );
static UInt32 pdcpSpsReleaseRecvd(UInt16 ueIndex, UInt8 lcId,
        UInt32 state, UInt32 event, LP_PdcpUeContext ueCtx_p, 
        LP_PdcpSpsIntervalProfile spsIntProf_p, InternalCellIndex intCellIdx );
static UInt32 pdcpSpsVoiceInActivated(UInt16 ueIndex, UInt8 lcId,
        UInt32 state, UInt32 event, LP_PdcpUeContext ueCtx_p, 
        LP_PdcpSpsIntervalProfile spsIntProf_p, InternalCellIndex intCellIdx );
static UInt32 pdcpSpsSilenceInActivated(UInt16 ueIndex, UInt8 lcId,
        UInt32 state, UInt32 event, LP_PdcpUeContext ueCtx_p, 
        LP_PdcpSpsIntervalProfile spsIntProf_p, InternalCellIndex intCellIdx );
static UInt32 pdcpSpsVoiceInDeactivated(UInt16 ueIndex, UInt8 lcId,
        UInt32 state, UInt32 event, LP_PdcpUeContext ueCtx_p, 
        LP_PdcpSpsIntervalProfile spsIntProf_p, InternalCellIndex intCellIdx );
static UInt32 pdcpSpsSilenceInDeactivated(UInt16 ueIndex, UInt8 lcId,
        UInt32 state, UInt32 event, LP_PdcpUeContext ueCtx_p, 
        LP_PdcpSpsIntervalProfile spsIntProf_p, InternalCellIndex intCellIdx );
static UInt32 pdcpSpsVoiceSilenceInDeactivated( UInt16 ueIndex, UInt8 lcId,
        UInt32 state, UInt32 event, LP_PdcpUeContext ueCtx_p, 
        LP_PdcpSpsIntervalProfile spsIntProf_p, InternalCellIndex intCellIdx );
static inline UInt32 pdcpProcessSpsRxPdu( UInt32 size, 
        LP_PdcpDataInd dataInd_p, LP_PdcpEnityContext ctx_p);
#endif
static inline UInt32 pdcpProcessEcnDrbReq( LP_PdcpDataInd dataInd_p, 
        LP_PdcpEnityContext ctx_p );

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* SPR 15909 fix start */
extern tickType_t pdcpCurrentTime_g;
/* SPR 15909 fix end */
#ifdef PDCP_THROUGHPUT_STATS 
extern UInt32 pdcpDataRx;
extern UInt32 pdcpDataRxPkt;
#endif
#ifdef PDCP_ASYNC_INTERFACE
extern MSGPOOL rxOutMsgPool_g;
#endif
#ifdef L2_FAST_PATH
extern PdcpContext pdcpContext;
#endif
/* + SPR 19808 */
#if defined (LOG_PRINT_ENABLED) && !(defined UE_SIM_TESTING)
extern UInt8 *integrityCheckMACIbaseptr_p;
#endif
/* - SPR 19808 */

/* SPR 3607 Changes Start */
extern UInt32 snhfnCountThreshold_g;
/* SPR 3607 Changes End */
extern UInt8 *pdcpFpProcessDrbRxBuf_p;
extern UInt8 *pdcpFpProcessSrbRxBuf_p;
extern UInt8 *ipHdrWoOptBytes_p;

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
/* array size is 2 for lcid type SRB/DRB */
pdcpFpLcidHandlerFuncT pdcpFpProcessPacket[PDCP_ENTITY_DRB + 1] =
{   pdcpFpProcessSrbRx, /*PDCP_ENTITY_SRB */
    pdcpFpProcessDrbRx  /*PDCP_ENTITY_DRB */
};

pdcpFpDrbHandlerFuncT pdcpFpProcessDrbPacket[PDCP_TS_PDU_D_C_DATA_PDU + 1] =
{   pdcpFpProcessDrbControl,/*PDCP_TS_PDU_D_C_CONTROL_PDU */
    pdcpFpProcessDrbData    /*PDCP_TS_PDU_D_C_DATA_PDU */
};

#ifndef UE_SIM_TESTING
/* State machine to handle SPS states at  RX side */
pdcpSpsHandlerFuncT pdcpSpsRxStateMachine[PDCP_MAX_SPS_RX_STATE]
                             [PDCP_MAX_SPS_RX_EVENT][PDCP_MAX_SPS_PKT_TYPE] =
{
    {   /* PDCP_SPS_IDLE State */
        {   /* PDCP_SPS_RRC_CONFIG Event*/        
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
            pdcpSpsConfigRecvd,
        },
        {   /* PDCP_SPS_CONFIGURED Event */
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
        },
        {   /* PDCP_SPS_ACTIVATED Event */
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
        },
        {   /* PDCP_SPS_DEACTIVATED Event */
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
        },
    },
    {   /* PDCP_SPS_CONFIGURED State */
        {   /* PDCP_SPS_RRC_CONFIG Event*/        
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
            pdcpSpsReleaseRecvd,
        },
        {   /* PDCP_SPS_CONFIGURED Event */
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
        },
        {   /* PDCP_SPS_ACTIVATED Event */
            pdcpSpsVoiceInActivated,
            pdcpSpsSilenceInActivated,
            pdcpSpsLogandIgnore,
        },
        {   /* PDCP_SPS_DEACTIVATED Event */
            pdcpSpsVoiceInDeactivated,
            pdcpSpsSilenceInDeactivated,
            pdcpSpsLogandIgnore,
        },
    },
    {   /* PDCP_SPS_ACTIVATED_SILENCE_NOT_DETECTED State */
        {   /* PDCP_SPS_RRC_CONFIG Event*/        
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
            pdcpSpsReleaseRecvd,
        },
        {   /* PDCP_SPS_CONFIGURED Event */
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
        },
        {   /* PDCP_SPS_ACTIVATED Event */
            pdcpSpsLogandIgnore,
            pdcpSpsSilenceInActivated,
            pdcpSpsLogandIgnore,
        },
        {   /* PDCP_SPS_DEACTIVATED Event */
            pdcpSpsVoiceInDeactivated,
            pdcpSpsSilenceInDeactivated,
            pdcpSpsLogandIgnore,
        },
    },
    {   /* PDCP_SPS_ACTIVATED_SILENCE_DETECTED State */
        {   /* PDCP_SPS_RRC_CONFIG Event*/        
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
            pdcpSpsReleaseRecvd,
        },
        {   /* PDCP_SPS_CONFIGURED Event */
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
        },
        {   /* PDCP_SPS_ACTIVATED Event */
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
        },
        {   /* PDCP_SPS_DEACTIVATED Event */
            pdcpSpsVoiceInDeactivated,
            pdcpSpsSilenceInDeactivated,
            pdcpSpsLogandIgnore,
        },
    },
    {   /* PDCP_SPS_DEACTIVATED_VOICE_NOT_DETECTED State */
        {   /* PDCP_SPS_RRC_CONFIG Event*/        
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
            pdcpSpsReleaseRecvd,
        },
        {   /* PDCP_SPS_CONFIGURED Event */
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
        },
        {   /* PDCP_SPS_ACTIVATED Event */
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
        },
        {   /* PDCP_SPS_DEACTIVATED Event */
            pdcpSpsVoiceInDeactivated,
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
        },
    },
    {   /* PDCP_SPS_DEACTIVATED_VOICE_DETECTED State */
        {   /* PDCP_SPS_RRC_CONFIG Event*/        
            pdcpSpsReleaseRecvd,
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
        },
        {   /* PDCP_SPS_CONFIGURED Event */
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
            pdcpSpsLogandIgnore,
        },
        {   /* PDCP_SPS_ACTIVATED Event */
            pdcpSpsVoiceInActivated,
            pdcpSpsSilenceInActivated,
            pdcpSpsLogandIgnore,
        },
        {   /* PDCP_SPS_DEACTIVATED Event */
            pdcpSpsVoiceSilenceInDeactivated,
            pdcpSpsVoiceSilenceInDeactivated,
            pdcpSpsLogandIgnore,
        },
    }
};
#endif


/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************** RX PROCEDURES *******************************/
/****************************************************************************
 * Function Name  : pdcpSrbFinishPduRx
 * Inputs         : LP_PdcpDataInd dataInd_p,LP_PdcpEnityContext ctx_p
 * Outputs        : None.
 * Returns        : None.
 * Description    : Finish Rx of SRB PDU
 ****************************************************************************/
void pdcpSrbFinishPduRx( LP_PdcpDataInd dataInd_p,
        LP_PdcpEnityContext ctx_p)
{
    UInt32  size = 0;
#ifndef PDCP_ASYNC_INTERFACE
    UInt16  integrityResult = PDCP_FAIL;
    void    *header_p = PNULL;
#endif   
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
	/* + KPI_CA */
	InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(dataInd_p->ueIndex);
/*spr 19296 fix start*/
	/*code deleted*/
/*spr 19296 fix end*/
/* KPI changes start */
    UInt32 activeIndexForPdcpStats = (gMacStats.pStats->
                            lteCommonKPIStats[internalCellIndex].activeIndexForPdcpStats);
    LtePdcpKPIStats * pLtePdcpKPIStats = 
        &(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].ltePdcpKPIStats[
        activeIndexForPdcpStats]);
/* KPI changes end */
	/* - KPI_CA */
#endif
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING) 
    LteUePdcpULPerfStats * lteUePdcpULPerfStats_p = (LteUePdcpULPerfStats*)\
            &(gPdcpUePerfStats_p[dataInd_p->ueIndex].lteUePdcpULPerfStats);
#endif
    CHECK_PNULL(ctx_p);

#ifndef PDCP_ASYNC_INTERFACE
    /*
     ** Integrity protection (for SRB only) 36323-860 [Section 5.7]. The
     ** integrity protection function includes both integrity protection and
     ** integrity verification and is performed in PDCP for PDCP entities 
     ** associated with SRBs. The data unit that is integrity protected is 
     ** the PDU header and the data part of the PDU before ciphering.
     */
    if ( ALGO_EIA0 != ctx_p->integrity.algorithmId ) 
    {
        /* Add SRB PDU Header byte (that was removed when sending to 
           ciphering) before calculating MACI */
        header_p = pdcpMakePDUHeaderSRB( dataInd_p->count, ctx_p->snSize, 
                dataInd_p->data_p);
        if(PNULL == header_p)
        {
            LOG_PDCP_MSG( PDCP_NULL_HEADER, LOGFATAL, PDCP_RX, pdcpCurrentTime_g,
                    __LINE__, dataInd_p->ueIndex, dataInd_p->lcId, 
                    dataInd_p->count, ctx_p->snSize, 0,0, __func__,"");
            pdcpFreeRxDataInd(dataInd_p);
            ltePanic("%s:header_p is NULL",__FUNCTION__);
            return;
        }

        /*check integrity*/
        integrityResult = integrityCheckMACI(
                ctx_p->integrity.algorithmId,
                /*SPR 1936 fix start*/
                /* SPR 3276 changes start */
                (ctx_p->radioBearer - 1),
                /* SPR 3276 changes end */
                /*SPR 1936 fix end*/
                dataInd_p->count,
                PDCP_ENTITY_RB_TX,
                ctx_p->integrity.key,
                dataInd_p->data_p
#ifndef UE_SIM_TESTING
				/* SPR 8174 Changes Start */
				,&(ctx_p->reconfigUeFlag)
				/* SPR 8174 Changes End */
#endif
                );

        /*we don't need SRB header any more*/
        if(0 == msgRemove(dataInd_p->data_p,0,1))
        {
            LOG_PDCP_MSG( PDCP_MSG_REMOVE_FAILED, LOGFATAL, PDCP_RX, 
                    pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                    dataInd_p->lcId, 1, dataInd_p->count,0,0, __func__,
                    "PDCP Header cannot be removed");
        }

        /*check integrity result*/
        if ( PDCP_FAIL == integrityResult )
        {
            LOG_PDCP_MSG( PDCP_INTEGRITY_VERIFICATION_FAILED, LOGWARNING, PDCP_RX,
                    pdcpCurrentTime_g, __LINE__,dataInd_p->ueIndex, 
                    dataInd_p->lcId, dataInd_p->count,0, 0,0,__func__,"");

            /* Update statistic data */
            pdcpStats.statsIntegrity.numberOfPacketsRejectedByIntegrityVerification += 1;
            /* SPR 3175 Changes Start */
            pdcpSendNotifyIntegrityFailureInd(dataInd_p->ueIndex, dataInd_p->lcId);
            /* SPR 3175 Changes Start */

            /*discard*/
            pdcpFreeRxDataInd(dataInd_p);
            return;
        }
    }
    else
    {
        /* + SPR 19808 */
#if defined (LOG_PRINT_ENABLED) && !(defined UE_SIM_TESTING)
	UInt8 temp = 0;      
	if( 0 != msgExtract( dataInd_p->data_p, -1,                  
				integrityCheckMACIbaseptr_p, MACI_LEN ) )  /*CID 110672*/
    	{
		if ( 0 != memCmp(integrityCheckMACIbaseptr_p, &temp, MACI_LEN) )
		{
			LOG_PDCP_WARNING( PDCP_RX, "Non-Zero MAC-I received for NULL "
					"integrity for UE[%u]", dataInd_p->ueIndex );
		}
	}
	else
	{
		LOG_PDCP_FATAL( PDCP_RX, "Unable to extract MAC-I value for UE[%u]",
				dataInd_p->ueIndex );
	}
#endif
        /* - SPR 19808 */
        /* Update statistic data for null integrity */
        pdcpStats.statsIntegrity.numberOfPacketsIntegrityVerifiedUsingEIA0
                += 1;
    }
    
    /*remove MACI from data_p*/
    if( 0 == msgRemove(dataInd_p->data_p,-1,MACI_LEN) )
    {
        LOG_PDCP_MSG( PDCP_MSG_REMOVE_FAILED, LOGFATAL, PDCP_RX, pdcpCurrentTime_g,
                __LINE__, dataInd_p->ueIndex, dataInd_p->lcId, MACI_LEN,
                dataInd_p->count, 0,0,__func__,"MACI cannot be removed");
    }
#endif
    /*SRB not discarded, processing*/
    if(dataInd_p->sn < ctx_p->next_SN)
    {
        ctx_p->hfn++;
    }
    ctx_p->next_SN = dataInd_p->sn;
    PDCP_INCREMENT_SN(&ctx_p->next_SN,&ctx_p->hfn,ctx_p->snSize);
    
    /* SPR_1211_FIX_START -> 8.6.0 compliance */
    size = msgSize(dataInd_p->data_p, PNULL);
    if ( PDCP_TS_SDU_MAX_SIZE >= size )
    {
        pdcpStats.statsUEs[dataInd_p->ueIndex].srbs[dataInd_p->lcId-1].\
            numberOfSrbUlSduTransmitted += 1;
#if defined (PERF_STATS) && !defined(UE_SIM_TESTING)
        /* Update KPI Counter for total SRB Bytes Transmitted  in UL */
        lteUePdcpULPerfStats_p->totalUlSrbBytesRcvd +=\
                                                      msgSize(dataInd_p->data_p, PNULL) ;
#endif
        LOG_PDCP_MSG( PDCP_SDU_SENT_UPPER_LAYER, LOGDEBUG, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                dataInd_p->lcId, dataInd_p->count, 0, 0,0,__func__,"");
/* KPI changes start */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    pLtePdcpKPIStats->ulCtrlPdcpSduBitRate += msgSize(dataInd_p->data_p, PNULL);
/* KPI changes end  */
	 /* 100MS_SUPPORT : start */
/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/
        /* 100MS_SUPPORT : end */
#endif               
        /* deliver the resulting PDCP SDU to upper layer PDCP_SRB_DATA_IND */
        pdcpRrcDataInd( dataInd_p->ueIndex, dataInd_p->lcId, dataInd_p->data_p );
    }
    else
    {
        pdcpStats.statsUEs[dataInd_p->ueIndex].srbs[dataInd_p->lcId -1].\
            numberOfUlSrbSduDropped += 1;
        LOG_PDCP_MSG( PDCP_SDU_SIZE_GREATER_THAN_LIMIT, LOGWARNING, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                dataInd_p->lcId, size, dataInd_p->count, 0,0, __func__, "");
    }
    /* SPR_1211_FIX_END */

    pdcpFreeRxDataInd(dataInd_p);
}

/****************************************************************************
 * Function Name  : pdcpDrbFinishPduRx
 * Inputs         : LP_PdcpDataInd dataInd_p,LP_PdcpEnityContext ctx_p
 * Outputs        : None.
 * Returns        : None.
 * Description    : Finish Rx of DRB PDU
 ****************************************************************************/
void pdcpDrbFinishPduRx( LP_PdcpDataInd dataInd_p,
        LP_PdcpEnityContext ctx_p)
{
#ifndef PDCP_ASYNC_INTERFACE
    UInt8  *out_buff_p = PNULL;
    UInt32 eCode = PDCP_SUCCESS;
    rohc_api_info_t  rohc_api_data;
#endif
    LP_PdcpDataInd node = PNULL;
    UInt32 size = 0;
    
#ifndef UE_SIM_TESTING
    LP_PdcpUeContext    ueCtx_p = PNULL;
    InternalCellIndex cellIndex  = INVALID_CELL_INDEX;
#else
    InternalCellIndex cellIndex  = 0;
#endif
#ifndef UE_SIM_TESTING
#ifdef KPI_STATS 
	cellIndex = PDCP_GET_CELL_INDEX(dataInd_p->ueIndex); 
    UInt32 activeIndexForPdcpStats = (gMacStats.
            pStats->lteCommonKPIStats[cellIndex].activeIndexForPdcpStats);
    LtePdcpKPIStats * pLtePdcpKPIStats = &(gMacStats.pStats->lteCommonKPIStats[cellIndex].
            ltePdcpKPIStats[activeIndexForPdcpStats]);
#endif
#ifdef PERF_STATS
    LteUePdcpULPerfStats * lteUePdcpULPerfStats_p = (LteUePdcpULPerfStats*)\
                                                    &(gPdcpUePerfStats_p[dataInd_p->ueIndex].lteUePdcpULPerfStats);
#endif
#endif
    CHECK_PNULL(ctx_p);
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    UInt8 qci = ctx_p->qci;
#endif

#ifndef UE_SIM_TESTING
    /*Check UE context*/
    ueCtx_p = PDCP_GET_UE_CONTEXT( dataInd_p->ueIndex );
    if ( PNULL == ueCtx_p )
    {
        LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex,
                pdcpContext.isInitialized, 0,0, 0,0, __func__,"");
        pdcpFreeRxDataInd(dataInd_p);
        return;
    }
    cellIndex = ueCtx_p->cellIndex ;

    /* SPR 3607, 5034 , 5102 changes start */
    if(dataInd_p->count == pdcpContext.sCellInfo[cellIndex].thresholdValue)
    {
        pdcpSendCountWraparoundInd(dataInd_p->ueIndex);
    }
    /* SPR 3607, 5034 , 5102 changes end */
#endif   

#ifndef PDCP_ASYNC_INTERFACE
    /*ROHC decompression (for DRB only)*/    
    if(PNULL != ctx_p->rohc.context)
    {
        rohc_api_data.api_id = ROHC_RLC_DATA_IND;
        rohc_api_data.entity_id.ueIndex = dataInd_p->ueIndex;
        rohc_api_data.entity_id.lcId = dataInd_p->lcId;
#if defined (PERF_STATS) && !defined(UE_SIM_TESTING)
        rohc_entity_t 	*p_rohc_entity = PNULL;
        p_rohc_entity = (rohc_entity_t*)(ctx_p->rohc.context); 
        p_rohc_entity->rohc_entity_id.qci = ctx_p->qci;
#endif
        rohc_api_data.in_buff_len = msgSize(dataInd_p->data_p,PNULL);
        LOG_PDCP_MSG( PDCP_DATA_TO_ROHC_ENGINE, LOGDEBUG, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex,
                dataInd_p->lcId, dataInd_p->count,0,0,0,__func__,"");

        if( RO_SUCCESS != rfc3095_process_msg(ctx_p->rohc.context, 
                    dataInd_p->data_p,&out_buff_p,&rohc_api_data,&eCode) )
        {
            /* + SPR_17858_389_CHANGES */
            if(dataInd_p->ueIndex < LTE_MAX_UE_ID)
            {
                /* - SPR_17858_389_CHANGES */

            /*error during ROHC decompression, discarding*/
            LOG_PDCP_MSG( PDCP_ROHC_DECOMPRESSION_FAILED, LOGWARNING, PDCP_RX,
                    pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                    dataInd_p->lcId,dataInd_p->count, 0,0,0,__func__,"");
            pdcpStats.statsUEs[dataInd_p->ueIndex].drbs[dataInd_p->lcId - 3].\
                numberOfDrbUlPduDropDueToRoHcFailure += 1;
            /* Update KPI counters for UL PDU Drop due to ROHC Failure*/
               /* + SPR_17858_389_CHANGES */
            }
             /* - SPR_17858_389_CHANGES */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
            if(qci != 0)
                pLtePdcpKPIStats->totalUlLossRatePerQci[qci-1]++; 
#endif
            pdcpFreeRxDataInd(dataInd_p);
            /*SPR 2498 changes start*/
            dataInd_p = PNULL;
            /*SPR 2498 changes end*/
            return;
        }
    }
#endif

    /* SPR_1211_FIX_START -> 8.6.0 compliance */
    size = msgSize(dataInd_p->data_p, PNULL);
    if ( PDCP_TS_SDU_MAX_SIZE < size)
    {
        LOG_PDCP_MSG( PDCP_SDU_SIZE_GREATER_THAN_LIMIT, LOGWARNING, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                dataInd_p->lcId, size, dataInd_p->count, 0,0, __func__, "");
        if(dataInd_p->ueIndex < LTE_MAX_UE_ID)
        {
            pdcpStats.statsUEs[dataInd_p->ueIndex].drbs[dataInd_p->lcId - 3].\
                numberOfDrbUlPduDropAsSizeOORange += 1;
        }
        /* Update KPI counters for UL PDU Drop due to size out of range*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        if(qci != 0)
            pLtePdcpKPIStats->totalUlLossRatePerQci[qci-1]++; 
#endif
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;
        return;
    }

#ifdef PDCP_THROUGHPUT_STATS 
    pdcpDataRx += dataInd_p->size;
    pdcpDataRxPkt++;
#endif

#ifndef UE_SIM_TESTING
    if ( LTE_TRUE == ctx_p->isSpsLc )
    {
        if (PDCP_SUCCESS != pdcpProcessSpsRxPdu( size, dataInd_p, ctx_p ) ) 
        {
            return;
        }
    }
#endif

    /* This flag is enabled when RRM sends the PDCP to PDCP_UE_DRB_ECN_CE_REQ */
    if( ( (pdcpContext.pdcpEcnConfigParam[cellIndex].ecnFeatureStatus
                    == ECN_APPLIED_IN_UL ) ||
          (pdcpContext.pdcpEcnConfigParam[cellIndex].ecnFeatureStatus 
                    == ECN_APPLIED_IN_BOTH_UL_DL )
        )  && 
        (ctx_p->congestionStatus))  
    {
        if(PDCP_FAIL == pdcpProcessEcnDrbReq (dataInd_p, ctx_p))
        {
            return;
        }
    } /* end of if congestionNotification enabled */

#ifdef L2_FAST_PATH
    if (LTE_PDCP_SLOW_PATH == dataInd_p->pktProcessing)
#endif
    {
        if(PDCP_ENTITY_RLC_AM == ctx_p->rlcMode)
        {
            /*Search for node in the storage*/
            node = pdcpRxStorageQFindNode(ctx_p, dataInd_p->count);
            if (PNULL == node)
            {
                /* Store the PDCP SDU */
                ENQUEUE_PDCP_RX_STORAGE_Q(ctx_p, dataInd_p);
            }
            else
            {
                if (dataInd_p->count == node->count)
                {
                    /*we already have node with same count in the storage*/
                    LOG_PDCP_MSG( PDCP_DUPLICATE_PACKET_RECEIVED_FROM_RLC, 
                            LOGWARNING, PDCP_RX, pdcpCurrentTime_g, __LINE__, 
                            dataInd_p->ueIndex, dataInd_p->lcId, 
                            dataInd_p->count, 0, 0,0, __func__, "");

                    /* If a PDCP SDU with the same PDCP SN is stored - discard
                       this PDCP SDU*/
                    pdcpFreeRxDataInd(dataInd_p);
                    /*SPR 2498 changes start*/
                    dataInd_p = PNULL;
                    /*SPR 2498 changes end*/
                    return;
                }
                else
                {
                    /* Store the PDU */
                    INSERT_BEFORE_PDCP_RX_STORAGE_Q(ctx_p, node, dataInd_p);
                }
            }

            /* 36323-8.6.0 Section 5.1.2.1.2 */
            if ( NORMAL_PACKET == dataInd_p->packetForReEstablishment )
            {
                /* Delivers all stored SDUs with value less than count,
                 * including cavity if any. */
                pdcpTransferUlPacketTillRSN( ctx_p, dataInd_p );
                /* Delivers all stored SDUs from value equal to count till some
                 * cavity */
                pdcpTransferUlPacketFromRSN( ctx_p, dataInd_p ); 
            }
            else
            {
                if ( dataInd_p->sn == ( (ctx_p->last_Submitted_SN + 1 ) % 
                            (PDCP_TS_MAX_SN_AM_MODE + 1) ) )
                {
                    /* Delivers all stored SDUs from value equal to count till
                     * some cavity */
                    pdcpTransferUlPacketFromRSN( ctx_p, dataInd_p );
                }
            }
        }
        else /*PDCP_ENTITY_RLC_UM==ctx_p->rlcMode*/
        {
            if(dataInd_p->ueIndex < MAX_UE_SUPPORTED)
            {
                pdcpStats.statsUEs[dataInd_p->ueIndex].drbs[dataInd_p->lcId -3].\
		            numberOfDrbUlPduTransmitted += 1;
#if defined (PERF_STATS) && !defined(UE_SIM_TESTING)
                /* Update KPI Counter for total DRB Bytes transmitted per LC in UL for PDCP SDUs:
                 * This does not inculde PDCP header */
                lteUePdcpULPerfStats_p->totalUlDrbBytesSent
                    [dataInd_p->lcId- LTE_DRB_LC_ID_LOW_BOUND] += 
                    msgSize(dataInd_p->data_p, PNULL);
#endif  
                /*36323-840: 5.1.2.1.3*/
                LOG_UT( PDCP_SDU_SENT_UPPER_LAYER, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
                        __LINE__,dataInd_p->ueIndex, dataInd_p->lcId, dataInd_p->count,
                        0, 0,0,__func__,"");
#ifndef UE_SIM_TESTING
                /* + SPR 19066 */
                /* update last scheduled tick in UL */
                ueCtx_p->lastSchedTickUl = pdcpCurrentTime_g;
                /* Reset lapsedInactiveTime as data is received */
                ueCtx_p->lapsedInactiveTime = 0;
                /* - SPR 19066 */ 
#endif
                /*deliver the resulting PDCP SDU to upper layer*/
                pdcpDataInd( dataInd_p->ueIndex, dataInd_p->lcId,
                        dataInd_p->data_p, size);
            }
            dataInd_p->data_p = PNULL;
            freeMemPool(dataInd_p);
            dataInd_p = PNULL;	
        }
    }
#ifdef L2_FAST_PATH
    else
    {
        if(dataInd_p->ueIndex < MAX_UE_SUPPORTED)
        {
            pdcpStats.statsUEs[dataInd_p->ueIndex].drbs[dataInd_p->lcId -3].\
                numberOfDrbUlPduTransmitted += 1;

            /*36323-840: 5.1.2.1.3*/
            LOG_UT( PDCP_SDU_SENT_UPPER_LAYER, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
                    __LINE__,dataInd_p->ueIndex, dataInd_p->lcId, dataInd_p->count,
                    0, 0,0,__func__,"");
#if defined (PERF_STATS) && !defined(UE_SIM_TESTING)
            /* Update KPI Counter for total DRB Bytes transmitted per LC in UL for PDCP SDUs:
             * This does not inculde PDCP header */
            lteUePdcpULPerfStats_p->totalUlDrbBytesSent\
                [dataInd_p->lcId- LTE_DRB_LC_ID_LOW_BOUND] += msgSize(dataInd_p->data_p, PNULL);
#endif
            /* + SPR 19066 */
            /* SPR 22304 Fix Start */
#ifndef UE_SIM_TESTING
            /* update last scheduled tick in UL */
            ueCtx_p->lastSchedTickUl = pdcpCurrentTime_g;
            /* Reset lapsedInactiveTime as data is received */ 
            ueCtx_p->lapsedInactiveTime = 0;
#endif
            /* SPR 22304 Fix End */
             /* + SPR 19066 */
            /*deliver the resulting PDCP SDU to upper layer*/
            pdcpDataInd( dataInd_p->ueIndex, dataInd_p->lcId,
                    dataInd_p->data_p, size);
        }

        /* Set Last_Submitted_PDCP_RX_SN to the PDCP SN of the last PDCP
         * SDU delivered to upper layers */
        ctx_p->last_Submitted_SN = dataInd_p->sn;
        dataInd_p->data_p = PNULL;
        freeMemPool(dataInd_p);
    }
#endif
}

/****************************************************************************
 * Function Name  : pdcpProcessEcnDrbReq
 * Inputs         : LP_PdcpDataInd dataInd_p,LP_PdcpEnityContext ctx_p
 * Outputs        : None.
 * Returns        : PDCP_FAIL/PDCP_SUCCESS
 * Description    : Process ECN DRB reqeuest received from RRM while finish 
 *                  Rx of DRB pdu
 ***************************************************************************/
static inline UInt32 pdcpProcessEcnDrbReq( LP_PdcpDataInd dataInd_p,
        LP_PdcpEnityContext ctx_p)
{
    UInt8 version = IPV4_VER;
    UInt8 ipHeaderLength = 0;
    EcnCeBit set_ecn_ip_hdr = SET_ECN_CE_BIT_IPV4_HDR;

    UInt8 *buff_p = ipHdrWoOptBytes_p; 
    RrmPdcpDrbEcnCeInd ecnCeInd;

    /* Extract first 20 bytes of data of IP header */
    if ( 0 == msgExtract(dataInd_p->data_p, 0, ipHdrWoOptBytes_p, 
                IP_HEADER_LENGTH_WO_OPT_BYTES))
    {
        LOG_PDCP_MSG( PDCP_MSG_EXTRACT_FAILED, LOGFATAL, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, 
                IP_HEADER_LENGTH_WO_OPT_BYTES, 
                dataInd_p->ueIndex, dataInd_p->lcId, 
                dataInd_p->count, 0,0, __func__,
                "ZCB data cannot be extracted");
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;	
        ltePanic("%s: msgExtract failed while extracting bufer",__func__);
        /* Coverity_55802 Fix Start */
        return PDCP_FAIL;
        /* Coverity_55802 Fix End */
    }
    if ( IPV6_VER == ( (*(ipHdrWoOptBytes_p) >> 4) & 0xF ) )
    {
        set_ecn_ip_hdr = SET_ECN_CE_BIT_IPV6_HDR;
        version = IPV6_VER;
        /*Currently ext headers not supported*/
        ipHeaderLength = IPV6_HEADER_LENGTH_TILL_ECN;
    }
    /* comparison with last 2 bits of DSCP value to know ECN Transport Capability */
    if( set_ecn_ip_hdr & (*(buff_p + 1)))
    { 
        if(ctx_p->ecnIndtobeSent)
        {
            /* send the indication */
            ecnCeInd.ueIndex          = dataInd_p->ueIndex;
            ecnCeInd.lcId             = dataInd_p->lcId;
            ecnCeInd.direction        = PDCP_ENTITY_RB_RX; 
            ecnCeInd.congestionStatus = CONGESTION_SET_TO_RRM ; 
            /* Start Coverity 25518 Fix*/
            ecnCeInd.pad[0]                = 0;
            ecnCeInd.pad[1]                = 0;
            ecnCeInd.pad[2]                = 0;
            /* End Coverity 25518 Fix*/
            sendEcnDrbUeIndicationToRRM(ecnCeInd );
            ctx_p->ecnIndtobeSent = LTE_FALSE;
        }

        /* If already set congestion enable ECN bit "11" then no need to
         * change the bit and checksum value in IP header*/
        if ( ( set_ecn_ip_hdr & (*(buff_p + 1))) != set_ecn_ip_hdr)
        { 
            *(buff_p + 1) |= set_ecn_ip_hdr;
            UInt16 *ptr = PNULL;

            if(IPV4_VER == version)
            {
                /* Need to mask first 4 bit containing version to get the ip header length
                 * in last 4 bits of 1st byte */
                ipHeaderLength = MULTIPLY_BY_FOUR((*buff_p) & 0x0F); 

                /* If optional bytes are included in IP header then its header 
                 * length is more then 20 bytes*/
                if (ipHeaderLength > IP_HEADER_LENGTH_WO_OPT_BYTES)
                {
                    if ( 0 == msgExtract(dataInd_p->data_p, IP_HEADER_LENGTH_WO_OPT_BYTES, 
                                &ipHdrWoOptBytes_p[IP_HEADER_LENGTH_WO_OPT_BYTES], 
                                ipHeaderLength - IP_HEADER_LENGTH_WO_OPT_BYTES))
                    { 
                        LOG_PDCP_MSG( PDCP_MSG_EXTRACT_FAILED, LOGFATAL, PDCP_RX, 
                                pdcpCurrentTime_g, __LINE__, 
                                ipHeaderLength - IP_HEADER_LENGTH_WO_OPT_BYTES, 
                                dataInd_p->ueIndex, dataInd_p->lcId, 
                                dataInd_p->count, 0,0, __func__,
                                "ZCB data cannot be extracted");
                        pdcpFreeRxDataInd(dataInd_p);
                        dataInd_p = PNULL;	
                        ltePanic("%s: msgExtract failed while extracting bufer",__func__);
                        /* Coverity_55802 Fix Start */
                        return PDCP_FAIL;
                        /* Coverity_55802 Fix End */
                    }
                }
                ptr = (UInt16 *)(buff_p + CHECKSUM_BYTE);
                /* set the checksum byte to ZERO */
                *ptr = 0; 

                /* set the checksum byte to the value got from getCheckSum */
                *ptr = pdcpGetIPHdrChecksum(ipHeaderLength, buff_p);
                CONVERT_INT16_HOST_TO_NETWORK(*ptr);
            }
            if( 0 == msgRemove(dataInd_p->data_p, 0, ipHeaderLength))
            {
                LOG_PDCP_MSG( PDCP_MSG_REMOVE_FAILED, LOGFATAL, PDCP_RX, 
                        pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                        dataInd_p->lcId, ipHeaderLength, 
                        dataInd_p->count, 0,0, __func__,
                        "ipHeaderLength cannot be removed");
                pdcpFreeRxDataInd(dataInd_p);
                dataInd_p = PNULL;	
                ltePanic("%s: msgRemove failed",__func__);
                /* Coverity_55802 Fix Start */
                return PDCP_FAIL;
                /* Coverity_55802 Fix End */
            }

            if(PNULL == msgInsert(dataInd_p->data_p, 0, buff_p , ipHeaderLength))
            {
                LOG_PDCP_MSG( PDCP_MSG_INSERT_FAILED, LOGFATAL, PDCP_RX, 
                        pdcpCurrentTime_g, __LINE__, ipHeaderLength, 
                        dataInd_p->ueIndex, dataInd_p->lcId, 
                        dataInd_p->count, 0,0, __func__,
                        "Failed while making congestion buffer");
                ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                        CRITICAL_ALARM);
                pdcpFreeRxDataInd(dataInd_p);
                dataInd_p = PNULL;	
                ltePanic("%s: msgInsert failed while making congestion "
                        "buffer for PDCP", __func__);
                /* Coverity_55802 Fix Start */
                return PDCP_FAIL;
                /* Coverity_55802 Fix End */
            }
        }
    }
    else
    {
        if(ctx_p->ecnIndtobeSent)
        {
            /* send the indication */
            ecnCeInd.ueIndex          = dataInd_p->ueIndex;
            ecnCeInd.lcId             = dataInd_p->lcId; 
            ecnCeInd.direction        = PDCP_ENTITY_RB_RX; 
            ecnCeInd.congestionStatus = CONGESTION_CANT_SET_TO_RRM ; 
            /* Start Coverity 25517 Fix*/
            ecnCeInd.pad[0]                = 0;
            ecnCeInd.pad[1]                = 0;
            ecnCeInd.pad[2]                = 0;
            /* End Coverity 25517 Fix*/
            sendEcnDrbUeIndicationToRRM(ecnCeInd );
            ctx_p->ecnIndtobeSent = LTE_FALSE;
        }    
    }
    /* Coverity_55802 Fix */
    return PDCP_SUCCESS;
}

#ifndef PDCP_ASYNC_INTERFACE
/****************************************************************************
 * Function Name  : pdcpToDecipherRx
 * Inputs         : LP_PdcpDataInd dataInd_p,LP_PdcpEnityContext ctx_p
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_FAIL
 * Description    : Decipher in case of need or perform Rx of PDU
 ****************************************************************************/
static UInt32 pdcpToDecipherRx(LP_PdcpDataInd dataInd_p,LP_PdcpEnityContext ctx_p)
{
    UInt32 retVal = PDCP_SUCCESS;
    UInt32 size = 0;
    UInt32 cipherSize = 0;
    UInt32 msgSegCnt = 0;
    UInt8 *msg = PNULL;
    QSEGMENT segment;
    UInt16 algoId = 0;

    CHECK_PNULL(ctx_p);

    /*Remove header*/
    if( 0 == msgRemove(dataInd_p->data_p, 0, 
                       GET_BYTESIZE_FROM_BITSIZE(ctx_p->snSize) ) )
    {
        LOG_PDCP_MSG( PDCP_MSG_REMOVE_FAILED, LOGFATAL, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                dataInd_p->lcId, 1, dataInd_p->count, 0,0, __func__,
                "PDCP Header cannot be removed");
        pdcpFreeRxDataInd(dataInd_p);
        /*SPR 2498 changes start*/
        dataInd_p = PNULL;
        /*SPR 2498 changes end*/
        return PDCP_FAIL;
    }

    /* SPR 2739 changes start */
    if ( NORMAL_PACKET == dataInd_p->packetForReEstablishment)
    {
       algoId = ctx_p->ciphering.newAlgorithmId;
    }
    else
    {
       algoId = ctx_p->ciphering.algorithmId;
    }
    if ( ALGO_EEA0 != algoId )
    /* SPR 2739 changes end */
    {
        /*8.6.0 Specs compliance - Packet should not be discarded */

        msgSegCnt = msgSegCount(dataInd_p->data_p);
        size = msgSize(dataInd_p->data_p,PNULL);

        /* SPR_1205_FIX_START */
        /* 
        ** Ciphering (EEA1) compliance requires data to be in multiple 
        ** of 4, i.e size (bytes) should be in multiple of 4 
        */
        cipherSize = size;

        /* Single segment in the message  */
        /* SES-525 Changes Start */
        /*if ( ( ( ALGO_EEA1 == algoId ) && 
                    ( SINGLE_SEGMENT == msgSegCnt) &&
                    ( 0 == size % CIPHER_MULTIPLE ) ) || 
                ( ALGO_EEA1 != algoId) )*/
        if ( ( SINGLE_SEGMENT == msgSegCnt) && ( 0 == size % CIPHER_MULTIPLE ) )				
        /* SES-525 Changes End */
        {
            /* Extracts the data from ZCB format data */
            if (PNULL == msgSegNext(dataInd_p->data_p, 0, &segment))
            {
                LOG_PDCP_MSG (PDCP_MSG_SEG_NEXT_FAILED, LOGFATAL, PDCP_RX, 
                         pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex,
                         dataInd_p->lcId,0,0,0,0,__func__,"");
                pdcpFreeRxDataInd(dataInd_p);
                /*SPR 2498 changes start*/
                dataInd_p = PNULL;
                return PDCP_FAIL;
                /*SPR 2498 changes end*/
            }
            msg  = segment.base;
            
            LOG_PDCP_MSG( PDCP_DATA_TO_CIPHERING_ENGINE, LOGDEBUG, PDCP_RX, 
                     pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                     dataInd_p->lcId, dataInd_p->count,0, 0,0,__func__,"");

            /* Calls the deciphering function */
            /* SPR 2739 changes start */                            
            if ( NORMAL_PACKET == dataInd_p->packetForReEstablishment)
            {
                retVal = sendToDeciphering( algoId,
                               /*SPR 1936 fix start*/
                               /* SPR 3276 changes start */
                               (ctx_p->radioBearer - 1),
                               /* SPR 3276 changes end */
                               /*SPR 1936 fix end*/
                               dataInd_p->count, PDCP_ENTITY_RB_TX,
                               ctx_p->ciphering.newKey,
                        msg, PNULL, msg, 
                        cipherSize, 0);
            }
            else
            {
                retVal = sendToDeciphering( algoId,
                               /*SPR 1936 fix start*/
                               /* SPR 3276 changes start */
                               (ctx_p->radioBearer - 1),
                               /* SPR 3276 changes end */
                               /*SPR 1936 fix end*/
                               dataInd_p->count, PDCP_ENTITY_RB_TX,
                               ctx_p->ciphering.key,
                        msg, PNULL, msg,
                        cipherSize, 0);
            }
            /* SPR 2739 changes end */  
            /* SPR_1210_FIX_START*/
            if (PDCP_SUCCESS != retVal)
            {
                LOG_PDCP_MSG (PDCP_DECIPHERING_FAILED, LOGFATAL, PDCP_RX, 
                         pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex,
                         dataInd_p->lcId, dataInd_p->count, cipherSize, 0,0,
                         __func__,"");
                ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
                pdcpFreeRxDataInd(dataInd_p);
                ltePanic("ciphering function fails %s",__FUNCTION__);
                /* SPR_1349_FIX_START*/
                /*SPR 2498 changes start*/
                return PDCP_FAIL;
                /*SPR 2498 changes end*/
                /* SPR_1349_FIX_END*/
            }
            /* SPR_1210_FIX_END*/
        }
        else /* Multiple segments in the message */
        {
            /* Cyclomatic Complexity changes - starts here */
           retVal = pdcpDecipherMultipleSegRxPdu( algoId, size, cipherSize, 
                       dataInd_p, ctx_p);
            /* Cyclomatic Complexity changes - ends here */
        }
        /* SPR_1205_FIX_END */
    }
    else
    {
        pdcpStats.statsCiphering.numberOfPacketsDecipheredUsingEIA0 += 1;
    }
    return retVal;
}

/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : pdcpDecipherMultipleSegRxPdu
 * Inputs         : algoId - Algorithm Id
 *                  size - PDU size
 *                  cipherSize - Ciphering Size
 *                  dataInd_p - Pointer to Data Ind
 *                  ctx_p - Pointer to PDCP Entity Context
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_FAIL
 * Description    : Decipher Rx of PDU in case of Multiple segments
 ****************************************************************************/
static inline UInt32 pdcpDecipherMultipleSegRxPdu( UInt16 algoId, UInt32 size,
        UInt32 cipherSize, LP_PdcpDataInd dataInd_p,
        LP_PdcpEnityContext ctx_p)
{
    UInt32 retVal = PDCP_SUCCESS;
    UInt8 *msg = PNULL;
    if ( 0 != size % CIPHER_MULTIPLE )
    {
        cipherSize += ( CIPHER_MULTIPLE - (size % CIPHER_MULTIPLE) );
    }
    msg = (UInt8 *)getMemFromPool(cipherSize, PNULL);
    if ( PNULL == msg )
    {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, cipherSize, 
                dataInd_p->ueIndex, dataInd_p->lcId, dataInd_p->count,
                0,0, __func__,"Cannot allocate memory for deciphering");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        pdcpFreeRxDataInd(dataInd_p);
        /*SPR 2498 changes start*/
        dataInd_p = PNULL;
        /*SPR 2498 changes end*/
        ltePanic ("getMemPool error in %s", __func__);
        /* SPR_1349_FIX_START*/
        /*SPR 2498 changes start*/
        return PDCP_FAIL;
        /*SPR 2498 changes end*/
        /* SPR_1349_FIX_END*/
    }
    if ( 0 == msgExtract(dataInd_p->data_p, 0, msg, size ) )
    {
        LOG_PDCP_MSG( PDCP_MSG_EXTRACT_FAILED, LOGFATAL, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, size, dataInd_p->ueIndex,
                dataInd_p->lcId, dataInd_p->count, 0,0, __func__,
                "ZCB data cannot be extracted for deciphering");
        pdcpFreeRxDataInd(dataInd_p);
        /*SPR 2498 changes start*/
        dataInd_p = PNULL;
        /*SPR 2498 changes end*/
        freeMemPool(msg);
        msg = PNULL;	
        ltePanic ("msgExtract error in %s", __func__);
        /* SPR_1349_FIX_START*/
        /*SPR 2498 changes start*/
        return PDCP_FAIL;
        /*SPR 2498 changes end*/
        /* SPR_1349_FIX_END*/
    }
    LOG_PDCP_MSG( PDCP_DATA_TO_CIPHERING_ENGINE, LOGDEBUG, PDCP_RX, 
            pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
            dataInd_p->lcId, dataInd_p->count,0, 0,0,__func__,"");

    /* Calls the deciphering function */
    /* SPR 2739 changes start */                            
    if ( NORMAL_PACKET == dataInd_p->packetForReEstablishment)
    {
        retVal = sendToDeciphering( algoId, 
                /*SPR 1936 fix start*/
                /* SPR 3276 changes start */
                (ctx_p->radioBearer - 1),
                /* SPR 3276 changes end */
                /*SPR 1936 fix end*/
                dataInd_p->count, PDCP_ENTITY_RB_TX,
                ctx_p->ciphering.newKey,
                msg, PNULL, msg, cipherSize, 0
                );
                
    }
    else
    {
        retVal = sendToDeciphering( algoId, 
                /*SPR 1936 fix start*/
                /* SPR 3276 changes start */
                (ctx_p->radioBearer - 1),
                /* SPR 3276 changes end */
                /*SPR 1936 fix end*/
                dataInd_p->count, PDCP_ENTITY_RB_TX,
                ctx_p->ciphering.key,
                msg, PNULL, msg, cipherSize, 0
                );
    }
    /* SPR 2739 changes end */  
    /* SPR_1210_FIX_START*/
    if (PDCP_SUCCESS != retVal)
    {
        LOG_PDCP_MSG (PDCP_DECIPHERING_FAILED, LOGFATAL, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex,
                dataInd_p->lcId, dataInd_p->count, cipherSize, 
                0,0,__func__,"");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        freeMemPool(msg);
        msg = PNULL;	
        pdcpFreeRxDataInd(dataInd_p);
        /*SPR 2498 changes start*/
        dataInd_p = PNULL;
        /*SPR 2498 changes end*/
        ltePanic("ciphering function fails %s",__FUNCTION__);
        /*SPR 2498 changes start*/
        return PDCP_FAIL;
        /*SPR 2498 changes end*/
    }
    /* SPR_1210_FIX_END*/
    if ( 0 == msgRemove(dataInd_p->data_p, 0, size ) )
    {
        LOG_PDCP_MSG( PDCP_MSG_REMOVE_FAILED, LOGFATAL, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                dataInd_p->lcId, size, dataInd_p->count, 0,0, 
                __func__,"Extra bytes needed for deciphering to be removed");
        pdcpFreeRxDataInd(dataInd_p);
        /*SPR 2498 changes start*/
        dataInd_p = PNULL;
        /*SPR 2498 changes end*/
        freeMemPool(msg);
        msg = PNULL;	
        ltePanic("%s: MsgRemove failed while extra bytes needed for deciphering",__func__);
        /*SPR 2498 changes start*/
        return PDCP_FAIL;
        /*SPR 2498 changes end*/
    }
    if (PNULL == msgInsert(dataInd_p->data_p, 0, msg, size) )
    {
        LOG_PDCP_MSG( PDCP_MSG_INSERT_FAILED, LOGFATAL, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, size, dataInd_p->ueIndex,
                dataInd_p->lcId, dataInd_p->count, 0,0, __func__,
                "data cannot be converted bach to ZCB after deciphering ");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        pdcpFreeRxDataInd(dataInd_p);
        /*SPR 2498 changes start*/
        dataInd_p = PNULL;
        /*SPR 2498 changes end*/
        freeMemPool(msg);
        msg = PNULL;	
        ltePanic("msgInsert failed in %s",__FUNCTION__);
        /*SPR 2498 changes start*/
        return PDCP_FAIL;
        /*SPR 2498 changes end*/
    }
    /* Free the message */
    freeMemPool(msg);
    msg = PNULL;
    return retVal;
}
#endif

/****************************************************************************
 * Function Name  : pdcpProcessRx_DRB_AM
 * Inputs         : LP_PdcpDataInd dataInd_p,LP_PdcpEnityContext ctx_p,UInt16 sn,
 8                  internalCellIndex
 * Outputs        : None.
 * Returns        : PDCP_SUECCESS/PDCP_FAIL
 * Description    : for RX DRB AM processing (36323-840: 5.1.2.1.2)
 ****************************************************************************/
/* SPR 5303 changes start */
/* + KPI_CA */
static UInt32 pdcpProcessRx_DRB_AM(LP_PdcpDataInd dataInd_p,
        LP_PdcpEnityContext ctx_p,UInt16 sn
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
		, InternalCellIndex internalCellIndex
#endif		
		)
/* SPR 5303 changes End */
{
    Int snDelta = 0;
#ifndef PDCP_ASYNC_INTERFACE
    rohc_api_info_t  rohc_api_data;
    UInt8        *out_buff_p = PNULL;
    UInt32       eCode = PDCP_SUCCESS;
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    UInt8 qci = ctx_p->qci; 
    UInt32 activeIndexForPdcpStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForPdcpStats);
    LtePdcpKPIStats *pLtePdcpKPIStats = &(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].
            ltePdcpKPIStats[activeIndexForPdcpStats]);
/* - KPI_CA */
#endif
#if defined (PERF_STATS) && !defined(UE_SIM_TESTING)
    rohc_entity_t 	*p_rohc_entity = PNULL;
#endif
#else
    UInt8      *outData_p = PNULL;
    UInt16     result = 0;
#endif

    CHECK_PNULL(dataInd_p);
    CHECK_PNULL(ctx_p);

    snDelta = ctx_p->last_Submitted_SN - sn; 
    if( (snDelta < -PDCP_TS_REORDERING_WINDOW /*sn-ctx_p->last_Submitted_SN < PDCP_TS_REORDERING_WINDOW*/) ||
            ((0 <= snDelta) && (snDelta < PDCP_TS_REORDERING_WINDOW)) )
        /*in window*/
    {
        if(sn > ctx_p->next_SN)
        {

            dataInd_p->count = PDCP_MAKE_COUNT(sn,ctx_p->hfn-1,ctx_p->snSize);
            LOG_PDCP_MSG( PDCP_SDU_SN_HFN_RX, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
                    __LINE__,dataInd_p->ueIndex, dataInd_p->lcId, sn,
                    ctx_p->hfn, 0, 0,__func__,"");
        }
        else
        {

            dataInd_p->count = PDCP_MAKE_COUNT(sn,ctx_p->hfn,ctx_p->snSize);
            LOG_PDCP_MSG( PDCP_SDU_SN_HFN_RX, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
                    __LINE__,dataInd_p->ueIndex, dataInd_p->lcId, sn,
                    ctx_p->hfn, 0, 0,__func__,"");
        }

        /*we have to do following (36323-840: 5.1.2.1.2):
         *)decipher the PDCP PDU
         *)perform header decompression (if configured) - THIS IS TO UPDATE ROHC STATE
         *)discard this PDCP SDU
         */
        if(PNULL != ctx_p->rohc.context)
        {
#ifdef PDCP_ASYNC_INTERFACE
            /* Set the flag to true as packet needs to be discarded */
            dataInd_p->discardAfterDecompression = LTE_TRUE;
            /* Allocate memory for output buffer and pass the details to 
             * security engine. */
            outData_p = (UInt8 *)msgAlloc(rxOutMsgPool_g,0,0,0);
            if( PNULL == outData_p )          
            {
                LOG_PDCP_MSG( PDCP_MSG_ALLOC_FAILED, LOGFATAL, PDCP_RX, 
                        pdcpCurrentTime_g, __LINE__,0,0,0, dataInd_p->count, 0,
                        0, __func__,"Allocation for deciphered output");
                ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                        CRITICAL_ALARM);
                pdcpFreeRxDataInd(dataInd_p);
                dataInd_p = PNULL;
                ltePanic("msgAlloc failed in %s", __FUNCTION__);
                return PDCP_FAIL;
            }
            /* Pass the shared context, input/output data and meta-data to 
             * security engine */
            result = pdcpSecurityProcessPacket( ctx_p->pdcpSecurityContext_p, 
                    dataInd_p->data_p, outData_p, 
                    (uaContextHandler)dataInd_p );
            if ( PDCP_SUCCESS != result )
            {
                /* SPR 16766 fix start */
                LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE, LOGWARNING, PDCP_RX, 
                        pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                        dataInd_p->lcId, dataInd_p->count, 0,
                        0, 0, __func__, "SecFail");
                /* SPR 16766 fix end */

                msgFree( outData_p );
                outData_p = PNULL;
                msgFree(dataInd_p->data_p);
                dataInd_p->data_p = PNULL;
                pdcpFreeRxDataInd( dataInd_p );
                dataInd_p = PNULL;
                /*SPR 5303 changes start*/
                return PDCP_FAIL;
                /*SPR 5303 changes end*/
            }
#else
            /* SPR 1207 changes start */
            /* No need to decipher if RoHC is NULL, as we need to discard the
             * packet */            
            /*SPR 2498 changes start*/
            if (PDCP_SUCCESS != pdcpToDecipherRx(dataInd_p,ctx_p) )
            {
                /*SPR 5303 changes start*/
                return PDCP_FAIL;
                /*SPR 5303 changes end*/
            }
            /*SPR 2498 changes end*/
            /* SPR 1207 changes end */

            rohc_api_data.api_id = ROHC_RLC_DATA_IND;
            rohc_api_data.entity_id.ueIndex = dataInd_p->ueIndex;

#if defined (PERF_STATS) && !defined(UE_SIM_TESTING) 
            p_rohc_entity = (rohc_entity_t*)(ctx_p->rohc.context); 
            p_rohc_entity->rohc_entity_id.qci = ctx_p->qci;
#endif

            rohc_api_data.entity_id.lcId = dataInd_p->lcId;
            rohc_api_data.in_buff_len = msgSize(dataInd_p->data_p,PNULL);

            LOG_PDCP_MSG( PDCP_DATA_TO_ROHC_ENGINE, LOGINFO, PDCP_RX, 
                    pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex,
                    dataInd_p->lcId, dataInd_p->count,0,0,0,__func__,"");
            if(RO_SUCCESS != rfc3095_process_msg(ctx_p->rohc.context, 
                        dataInd_p->data_p,&out_buff_p,&rohc_api_data,&eCode))
            {
                /*error during ROHC decompression, discarding*/
                LOG_PDCP_MSG( PDCP_ROHC_DECOMPRESSION_FAILED, LOGWARNING, PDCP_RX,
                        pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                        dataInd_p->lcId,dataInd_p->count, 0,0,0,__func__,"");
                pdcpStats.statsUEs[dataInd_p->ueIndex].drbs[dataInd_p->lcId- \
                    3].numberOfDrbUlPduDropDueToRoHcFailure += 1;

                /* Update KPI counters for UL PDU Drop due to ROHC Failure for
                 * AM Entity*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                if(qci != 0)
                    pLtePdcpKPIStats->totalUlLossRatePerQci[qci-1]++; 
#endif

                pdcpFreeRxDataInd(dataInd_p);
                /*SPR 2498 changes start*/
                dataInd_p = PNULL;
                /*SPR 2498 changes end*/
                /*SPR 5303 changes start*/
                return PDCP_FAIL;
                /*SPR 5303 changes end*/
            }
            /* Discard the packet */
            LOG_PDCP_MSG( PDCP_OUT_WINDOW_DISCARD, LOGWARNING, PDCP_RX, 
                    pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                    dataInd_p->lcId, dataInd_p->count, 0,0,0,__func__,"");

            pdcpFreeRxDataInd(dataInd_p);
            dataInd_p = PNULL;	
            /*SPR 5303 changes start*/
            return PDCP_FAIL;
            /*SPR 5303 changes End*/
#endif
        }
        else
        {
            /* Discard the packet */
            LOG_PDCP_MSG( PDCP_OUT_WINDOW_DISCARD, LOGWARNING, PDCP_RX, 
                    pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                    dataInd_p->lcId, dataInd_p->count, 0,0,0,__func__,"");

            pdcpFreeRxDataInd(dataInd_p);
            /*SPR 2498 changes start*/
            dataInd_p = PNULL;
            /*SPR 2498 changes end*/
            /*SPR 5303 changes start*/
            return PDCP_FAIL;
            /*SPR 5303 changes End*/
        }
    }
    else if ( (ctx_p->next_SN > sn) &&
            ((ctx_p->next_SN - sn) > PDCP_TS_REORDERING_WINDOW))
        /*out of window. too old SN*/
    {
        ctx_p->hfn++;

        dataInd_p->count = PDCP_MAKE_COUNT(sn,ctx_p->hfn,ctx_p->snSize);
        ctx_p->next_SN = (sn + 1) & PDCP_SN_MASK_BIT;
        LOG_PDCP_MSG( PDCP_SDU_SN_HFN_RX, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
                __LINE__,dataInd_p->ueIndex, dataInd_p->lcId, ctx_p->next_SN,
                ctx_p->hfn, 0, 0,__func__,"");
    }
    else if( (sn > ctx_p->next_SN) &&
            ((sn - ctx_p->next_SN) >= PDCP_TS_REORDERING_WINDOW) )    
        /*out of window. too new SN*/
    {

        dataInd_p->count = PDCP_MAKE_COUNT(sn,ctx_p->hfn-1,ctx_p->snSize);
        LOG_PDCP_MSG( PDCP_SDU_SN_HFN_RX, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
                __LINE__,dataInd_p->ueIndex, dataInd_p->lcId, sn,
                ctx_p->hfn, 0, 0,__func__,"");
    }
    else if(sn >= ctx_p->next_SN)
        /*new SN*/
    {

        dataInd_p->count = PDCP_MAKE_COUNT(sn,ctx_p->hfn,ctx_p->snSize);
        LOG_PDCP_MSG( PDCP_SDU_SN_HFN_RX, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
                __LINE__,dataInd_p->ueIndex, dataInd_p->lcId, sn,
                ctx_p->hfn, 0, 0,__func__,"");
        ctx_p->next_SN = sn;
        PDCP_INCREMENT_SN(&ctx_p->next_SN,&ctx_p->hfn,ctx_p->snSize);
    }
    else if(sn < ctx_p->next_SN)
        /*old SN*/
    {

        dataInd_p->count = PDCP_MAKE_COUNT(sn,ctx_p->hfn,ctx_p->snSize);
        LOG_PDCP_MSG( PDCP_SDU_SN_HFN_RX, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
                __LINE__,dataInd_p->ueIndex, dataInd_p->lcId, sn,
                ctx_p->hfn, 0, 0,__func__,"");
    }

    /*if the PDCP PDU has not been discarded in the above*/
    /*Send to Decipher or Finish processing*/
    LOG_PDCP_MSG( PDCP_SDU_COUNT, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g, __LINE__,
            dataInd_p->ueIndex, dataInd_p->lcId, dataInd_p->count, 0, 
            0,0,__func__,"");
    /*SPR 5303 changes Start*/
    return PDCP_SUCCESS;
    /*SPR 5303 changes End*/
}

/****************************************************************************
 * Function Name  : pdcpFpProcessSrbRx
 * Inputs         : ctx_p - PDCP Entity context,
 *                  dataInd_p - current packet
 * Outputs        : None
 * Returns        : UInt32 - Success/Fail
 * Description    : Performs fast path processing for Rx packets on SRB
 ****************************************************************************/
UInt32 pdcpFpProcessSrbRx(LP_PdcpEnityContext ctx_p,
    LP_PdcpDataInd dataInd_p)
{   
#ifdef PDCP_ASYNC_INTERFACE
    UInt8        *outData_p = PNULL;
    UInt16       result = 0;
#endif

    if( 0 == msgExtract(dataInd_p->data_p, 0, pdcpFpProcessSrbRxBuf_p, 
                PDCP_MAX_HDR_SIZE) )
    {
        LOG_PDCP_MSG( PDCP_MSG_EXTRACT_FAILED, LOGFATAL, PDCP_RX, pdcpCurrentTime_g,
                __LINE__, PDCP_MAX_HDR_SIZE, dataInd_p->ueIndex, 
                dataInd_p->lcId, 0, 0,0, __func__, 
                "Unable to extract PDCP header");
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;	
        return PDCP_FAIL;
    }

    LOG_UT( PDCP_SIGNALING_PDU_FROM_RLC, LOGDEBUG, PDCP_RX, 
            pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
            dataInd_p->lcId, 0,0, 0,0, __func__,"");

    /* Update statistic data */
    pdcpStats.statsUEs[dataInd_p->ueIndex].srbs[dataInd_p->lcId \
        -1].numberOfSrbUlPduReceived += 1;

    /*calc SN*/
    dataInd_p->sn = pdcpFpProcessSrbRxBuf_p[0] & PDCP_TS_PDU_SRB_SN_MASK;

    /*36323-860: 5.1.2.2*/
    if ( dataInd_p->sn < ctx_p->next_SN )
    {

        dataInd_p->count = PDCP_MAKE_COUNT(dataInd_p->sn, 
                ctx_p->hfn+1, ctx_p->snSize);
    }
    else
    {

        dataInd_p->count = PDCP_MAKE_COUNT( dataInd_p->sn, 
                ctx_p->hfn, ctx_p->snSize);
    }
    LOG_PDCP_MSG( PDCP_SDU_SN_HFN_RX, LOGINFO, PDCP_RX, pdcpCurrentTime_g,
            __LINE__,dataInd_p->ueIndex, dataInd_p->lcId, dataInd_p->sn,
            ctx_p->hfn, 0, 0, __func__,"");

    LOG_UT( PDCP_SDU_COUNT, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
            __LINE__, dataInd_p->ueIndex, dataInd_p->lcId, 
            dataInd_p->count, 0, 0,0, __func__,"");

#ifdef PDCP_ASYNC_INTERFACE
    /* Allocate memory for output buffer and pass the details to security 
     * engine. */
    outData_p = (UInt8 *)msgAlloc(rxOutMsgPool_g,0,0,0);
    if( PNULL == outData_p )          
    {
        LOG_PDCP_MSG( PDCP_MSG_ALLOC_FAILED, LOGFATAL, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__,0,0,0, dataInd_p->count,
                0, 0, __func__,"Allocation for deciphered output");
        ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;
        ltePanic("msgAlloc failed in %s", __FUNCTION__);
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }
    /* Pass the shared context, input/output data and meta-data to
     * security engine */
    result = pdcpSecurityProcessPacket( ctx_p->pdcpSecurityContext_p, 
            dataInd_p->data_p, outData_p, 
            (uaContextHandler)dataInd_p );
    if ( PDCP_SUCCESS != result )
    {
        /* SPR 16766 fix start */
        LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE, LOGWARNING, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                dataInd_p->lcId, dataInd_p->count, 0,
                0, 0, __func__, "SecFail");
        /* SPR 16766 fix end */
        msgFree( outData_p );
        outData_p = PNULL;

        msgFree(dataInd_p->data_p);
        dataInd_p->data_p = PNULL;
        pdcpFreeRxDataInd( dataInd_p );
        dataInd_p = PNULL;
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }        
#else
    /*Send to Decipher or Finish processing*/
    /*SPR 2498 changes start*/
    if (PDCP_SUCCESS != pdcpToDecipherRx(dataInd_p,ctx_p) )
    {
        return PDCP_FAIL;
    }
    /*SPR 2498 changes end*/
    pdcpSrbFinishPduRx(dataInd_p,ctx_p);
#endif

    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpProcessSrbRx 
 * Inputs         : ueIndex, lcId, state, event, context_p
 * Outputs        : None.
 * Returns        : PDCP_SUECCESS/PDCP_FAIL
 * Description    : for RX processing
 ****************************************************************************/
UInt32 pdcpProcessSrbRx(UInt16 ueIndex, UInt8 lcId, 
        UInt32 state, UInt32 event, void *context_p) 
{
    LP_PdcpEnityContext ctx_p = (LP_PdcpEnityContext)context_p;
    LP_PdcpDataInd dataInd_p = PNULL;

    LOG_UT( PDCP_EVENT_RCVD, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g, 
            __LINE__, dataInd_p->ueIndex, dataInd_p->lcId, PDCP_RX_STATE_CONNECTED, 
            PDCP_SRB_RX, 0,0, __func__, "");

    /*Get Data Ind (PDU) from PDCP RX queue*/
    DEQUEUE_PDCP_RX_DATA_IND_Q(ctx_p, dataInd_p);
    if ( PNULL == dataInd_p )
    {
        /*Nothing to process*/
        LOG_PDCP_MSG (PDCP_NULL_NODE_Q, LOGWARNING, PDCP_RX, pdcpCurrentTime_g,
                __LINE__, ueIndex, lcId,event,state, 0,0, __func__, "pdcpRxDataIndQ");
        return PDCP_FAIL;
    }
    if(PNULL == dataInd_p->data_p)
    {
        LOG_PDCP_MSG( PDCP_DATA_POINTER_NULL, LOGWARNING, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, ueIndex, lcId,event,state, 0,0, 
                __func__, "pdcpRxDataIndQ has null data");
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;	
        return PDCP_FAIL;
    }

    return (pdcpFpProcessSrbRx(ctx_p, dataInd_p));
}

/* SPR 3608 changes end */
/****************************************************************************
 * Function Name  : pdcpFpProcessDrbControl
 * Inputs         : ctx_p - PDCP Entity context,
 *                  dataInd_p - current packet
 *                  hdr_p - 
 * Outputs        : None
 * Returns        : None
 * Description    : enqueues data packets to Control PDU Tx Q
 ****************************************************************************/
void pdcpFpProcessDrbControl(LP_PdcpEnityContext ctx_p,
    LP_PdcpDataInd dataInd_p, UInt8* hdr_p)
{
    UInt8 ctrl_type = 0;
    LOG_UT( PDCP_CONTROL_PDU_FROM_RLC, LOGDEBUG, PDCP_RX, 
           pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
           dataInd_p->lcId, 0,0, 0,0, __func__,"");

    /*
    ** Store received PDCP Control PDU indication in PDCP Control PDU
    ** TX queue for future processing in pdcpElTxProcessMessage->
    ** pdcpElTxProcessControlPduInd
    */
    ctrl_type = (*hdr_p & 0xff)>>4;
    if ( PNULL == pdcpStoreControlPduInd(
                            dataInd_p->ueIndex, 
                            dataInd_p->lcId, 
                            PDCP_RX_CONTROL_PDU_IND, 
                            dataInd_p->data_p,
                            ctrl_type) )
    {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, sizeof(PdcpControlPduInd),
                dataInd_p->ueIndex, dataInd_p->lcId,ctx_p->last_Submitted_SN, 0,0, 
                __func__, "Control PDU Ind is NULL");
        ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        /* Coverity 25198 Fix Start*/
        dataInd_p->data_p = PNULL;
        /* Coverity 25198 Fix End*/
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;	
        ltePanic("MemAlloc fails for PdcpControlPduInd\n");
        return ;
    } 

    /* don't delete message */
    dataInd_p->data_p = PNULL;
    pdcpFreeRxDataInd(dataInd_p);
    dataInd_p = PNULL;	

    return;
}

/****************************************************************************
 * Function Name  : pdcpFpProcessDrbRx
 * Inputs         : ctx_p - PDCP Entity context,
 *                  dataInd_p - current packet
 * Outputs        : None
 * Returns        : UInt32 - Success/Fail
 * Description    : Performs fast path processing for Rx packets on DRB
 ****************************************************************************/
extern UInt64 uplinkIngressDataSize;
extern UInt64 uplinkIngressDataNum;
UInt32 pdcpFpProcessDrbRx(LP_PdcpEnityContext ctx_p,
    LP_PdcpDataInd dataInd_p)
{
    uplinkIngressDataSize += dataInd_p->size;
	uplinkIngressDataNum++;	
 #if defined (PERF_STATS) && !defined(UE_SIM_TESTING)
    LteUePdcpULPerfStats * lteUePdcpULPerfStats_p = (LteUePdcpULPerfStats*)
            &(gPdcpUePerfStats_p[dataInd_p->ueIndex].lteUePdcpULPerfStats);
#endif

    LOG_UT( PDCP_DATA_PDU_FROM_RLC, LOGDEBUG, PDCP_RX, 
            pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                dataInd_p->lcId, 0, 0, 0,0, __func__,"");
    if( 0 == msgExtract(dataInd_p->data_p, 0, pdcpFpProcessDrbRxBuf_p, 
                        PDCP_MAX_HDR_SIZE) )
    {
        LOG_PDCP_MSG( PDCP_MSG_EXTRACT_FAILED, LOGFATAL, PDCP_RX, pdcpCurrentTime_g,
                __LINE__, PDCP_MAX_HDR_SIZE, dataInd_p->ueIndex, 
                dataInd_p->lcId, 0, 0,0, __func__, 
                "Unable to extract PDCP header");
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;	
        return PDCP_FAIL;
    }

    /* Update statistic data */
    pdcpStats.statsUEs[dataInd_p->ueIndex].drbs[dataInd_p->lcId-3]. \
               numberOfDrbUlSduReceived += 1;
    /* Update KPI Counters for total UL pkts rcvd*/
#if defined (PERF_STATS) && !defined(UE_SIM_TESTING)
    /* Update KPI Counters for total UL SDU rcvd per LC and per UE*/
    lteUePdcpULPerfStats_p->numberOfUlDrbSduReceived\
	    [dataInd_p->lcId - LTE_DRB_LC_ID_LOW_BOUND]++;
#endif
    pdcpFpProcessDrbPacket[PDCP_TS_PDU_GET_D_C(
                               pdcpFpProcessDrbRxBuf_p[0])](ctx_p, 
            dataInd_p, pdcpFpProcessDrbRxBuf_p);

    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpProcessDrbRx  
 * Inputs         : ueIndex, LcId
 *                  state - state of PDCP for RX entity,
 *                  event - event received to process packet.
 *                  context_p - PDCP Entity context,
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_FAIL
 * Description    : for RX processing
 ****************************************************************************/
UInt32 pdcpProcessDrbRx( UInt16 ueIndex, UInt8 lcId, 
        UInt32 state, UInt32 event, void *context_p )
{
    LP_PdcpEnityContext ctx_p = (LP_PdcpEnityContext)context_p;
    LP_PdcpDataInd dataInd_p = PNULL;

    LOG_UT( PDCP_EVENT_RCVD, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g, 
            __LINE__, dataInd_p->ueIndex, dataInd_p->lcId, 
            PDCP_RX_STATE_CONNECTED, PDCP_DRB_RX, 0,0, __func__, "");

    /*Get Data Ind (PDU) from PDCP RX queue*/
    DEQUEUE_PDCP_RX_DATA_IND_Q(ctx_p, dataInd_p);
    
    if ( PNULL == dataInd_p )
    {
        /*Nothing to process*/
        LOG_PDCP_MSG (PDCP_NULL_NODE_Q, LOGWARNING, PDCP_RX, pdcpCurrentTime_g,
                __LINE__, ueIndex, lcId,event,state, 0,0, __func__, "pdcpRxDataIndQ");
        return PDCP_FAIL;
    }
    if(PNULL == dataInd_p->data_p)
    {
        LOG_PDCP_MSG( PDCP_DATA_POINTER_NULL, LOGWARNING, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, ueIndex, lcId,event,state,0,0, 
                __func__, "pdcpRxDataIndQ has null data");
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;	
        return PDCP_FAIL;
    }

    return(pdcpFpProcessDrbRx( ctx_p, dataInd_p));
}

/****************************************************************************
 * Function Name  : pdcpTransferUlPacketTillRSN
 * Inputs         : ctx_p - PDCP Entity context,
 *                  currDataInd_p - current packet
 * Outputs        : None.
 * Returns        : None.
 * Description    : Transfers uplink packets till count associated with RSN 
 *                  (including cavities, if any) to Packet Relay
 ****************************************************************************/
void pdcpTransferUlPacketTillRSN( LP_PdcpEnityContext ctx_p, 
         LP_PdcpDataInd currDataInd_p )
{
    LP_PdcpDataInd  dataInd_p = PNULL;
    UInt32 qCount = QCOUNT_PDCP_RX_STORAGE_Q( ctx_p );
#if defined (PERF_STATS) && !defined(UE_SIM_TESTING)
    LteUePdcpULPerfStats * lteUePdcpULPerfStats_p = PNULL; 
#endif

    while ( qCount-- )
    {
        GET_FIRST_PDCP_RX_STORAGE_Q(ctx_p, dataInd_p);
        if ( PNULL == dataInd_p)
        {
            LOG_PDCP_MSG (PDCP_NULL_NODE_Q, LOGWARNING, PDCP_RX, pdcpCurrentTime_g,
                    __LINE__, currDataInd_p->ueIndex, currDataInd_p->lcId, 
                    0,0, 0,0, __func__, "pdcpUlReorderingQ");
            break;            
        }

        if ( dataInd_p->count != currDataInd_p->count )
        {
            LOG_PDCP_MSG( PDCP_SDU_SENT_UPPER_LAYER, LOGDEBUG, PDCP_RX, 
                    pdcpCurrentTime_g, __LINE__,dataInd_p->ueIndex, 
                    dataInd_p->lcId, dataInd_p->sn, 0, 0,0,__func__,"");

            pdcpStats.statsUEs[dataInd_p->ueIndex].drbs[dataInd_p->lcId - 3].\
                numberOfDrbUlPduTransmitted += 1;
#if defined (PERF_STATS) && !defined(UE_SIM_TESTING)
            lteUePdcpULPerfStats_p =  (LteUePdcpULPerfStats*)
                &(gPdcpUePerfStats_p[dataInd_p->ueIndex].lteUePdcpULPerfStats);
            /* Update KPI Counter for total DRB Bytes transmitted per LC in UL for PDCP SDUs:
             * This does not inculde PDCP header */
            lteUePdcpULPerfStats_p->totalUlDrbBytesSent
                [dataInd_p->lcId- LTE_DRB_LC_ID_LOW_BOUND] += 
                msgSize(dataInd_p->data_p, PNULL);
#endif
           /* + SPR 19066 */
            LP_PdcpUeContext ueCtx_p = PDCP_GET_VALID_UE_CONTEXT( 
                    dataInd_p->ueIndex );  
            /* update last scheduled tick in UL */
            ueCtx_p->lastSchedTickUl = pdcpCurrentTime_g;
            /* Reset lapsedInactiveTime as data is received */ 
            ueCtx_p->lapsedInactiveTime = 0;
            /* - SPR 19066 */
            /* Deliver in-sequence PDCP SDU to upper layer */
            pdcpDataInd( dataInd_p->ueIndex, dataInd_p->lcId, 
                    dataInd_p->data_p, 
                    msgSize(dataInd_p->data_p, PNULL) );

            /* Set Last_Submitted_PDCP_RX_SN to the PDCP SN of the last PDCP
               SDU delivered to upper layers */
            ctx_p->last_Submitted_SN = dataInd_p->sn;
            DEQUEUE_PDCP_RX_STORAGE_Q( ctx_p, dataInd_p );
            if ( PNULL == dataInd_p )
            {
                LOG_PDCP_MSG (PDCP_NULL_NODE_Q, LOGWARNING, PDCP_RX, 
                        pdcpCurrentTime_g, __LINE__, currDataInd_p->ueIndex, 0,
                        0,0, 0,0, __func__, "pdcpUlReorderingQ");
                continue;
            }
            dataInd_p->data_p = PNULL;
            pdcpFreeRxDataInd(dataInd_p);          
            dataInd_p = PNULL;
        }
        else
        {
            if ( dataInd_p->sn != (ctx_p->last_Submitted_SN + 1)  %
                    (PDCP_TS_MAX_SN_AM_MODE + 1) )
            {
                LOG_UT( PDCP_SDU_NOT_SENT_UPPER_LAYER, LOGWARNING, PDCP_RX, 
                        pdcpCurrentTime_g, __LINE__,dataInd_p->ueIndex, 
                        dataInd_p->lcId, dataInd_p->sn, 
                        ctx_p->last_Submitted_SN, 0,0, __func__,"");
            }
            break;
        }
    }
}

/****************************************************************************
 * Function Name  : pdcpTransferUlPacketFromRSN
 * Inputs         : ctx_p - PDCP Entity context,
 *                  currDataInd_p - current packet
 * Outputs        : None.
 * Returns        : None.
 * Description    : Transfers uplink packets from current count associated with
 *                  RSN till cavity to Packet Relay
 *****************************************************************************/
void pdcpTransferUlPacketFromRSN( LP_PdcpEnityContext ctx_p, 
       LP_PdcpDataInd currDataInd_p)
{
    LP_PdcpDataInd  dataInd_p = PNULL;
    UInt32 qCount = QCOUNT_PDCP_RX_STORAGE_Q( ctx_p );
    UInt32 count =  currDataInd_p->count;
#if defined (PERF_STATS) && !defined(UE_SIM_TESTING)
    LteUePdcpULPerfStats *lteUePdcpULPerfStats_p = PNULL;
#endif

    while ( qCount-- )
    {
        GET_FIRST_PDCP_RX_STORAGE_Q(ctx_p, dataInd_p);
        if ( PNULL == dataInd_p)
        {
            LOG_PDCP_MSG (PDCP_NULL_NODE_Q, LOGWARNING, PDCP_RX, pdcpCurrentTime_g,
                     __LINE__, currDataInd_p->ueIndex, currDataInd_p->lcId, 
                     0,0, 0,0, __func__, "pdcpUlReorderingQ");
            break;
        }
        if ( ( dataInd_p->count == count ) ||
             ( dataInd_p->sn == ( (ctx_p->last_Submitted_SN + 1 ) % 
                                (PDCP_TS_MAX_SN_AM_MODE + 1) ) ) )
        {
            LOG_PDCP_MSG( PDCP_SDU_SENT_UPPER_LAYER, LOGDEBUG, PDCP_RX, 
                    pdcpCurrentTime_g, __LINE__,dataInd_p->ueIndex, 
                    dataInd_p->lcId, dataInd_p->sn, 0, 0,0,__func__,"");

            pdcpStats.statsUEs[dataInd_p->ueIndex].drbs[dataInd_p->lcId - 3].\
                numberOfDrbUlPduTransmitted += 1;
#if defined (PERF_STATS) && !defined(UE_SIM_TESTING)
            lteUePdcpULPerfStats_p =  (LteUePdcpULPerfStats*)
                &(gPdcpUePerfStats_p[dataInd_p->ueIndex].lteUePdcpULPerfStats);
            /* Update KPI Counter for total DRB Bytes transmitted per LC in UL
             * for PDCP SDUs: This does not inculde PDCP header */
            lteUePdcpULPerfStats_p->totalUlDrbBytesSent
                [dataInd_p->lcId- LTE_DRB_LC_ID_LOW_BOUND] += 
                msgSize(dataInd_p->data_p, PNULL);
#endif
            /* + SPR 19066 */ 
            LP_PdcpUeContext ueCtx_p = PDCP_GET_VALID_UE_CONTEXT( 
                    dataInd_p->ueIndex );
            /* update last scheduled tick in UL */
            ueCtx_p->lastSchedTickUl = pdcpCurrentTime_g; 
            /* Reset lapsedInactiveTime as data is received */
            ueCtx_p->lapsedInactiveTime = 0;
             /* - SPR 19066 */
            /* Deliver in-sequence PDCP SDU to upper layer */
            pdcpDataInd( dataInd_p->ueIndex, dataInd_p->lcId, 
                    dataInd_p->data_p, 
                    msgSize(dataInd_p->data_p, PNULL) );

            /* Set Last_Submitted_PDCP_RX_SN to the PDCP SN of the last PDCP
               SDU delivered to upper layers */
            ctx_p->last_Submitted_SN = dataInd_p->sn;
            DEQUEUE_PDCP_RX_STORAGE_Q( ctx_p, dataInd_p );
            if ( PNULL == dataInd_p )
            {
                LOG_PDCP_MSG (PDCP_NULL_NODE_Q, LOGWARNING, PDCP_RX, 
                        pdcpCurrentTime_g, __LINE__, currDataInd_p->ueIndex,
                        currDataInd_p->lcId, 0,0, 0,0, __func__, 
                        "pdcpUlReorderingQ");
                continue;
            }
            dataInd_p->data_p = PNULL;
            pdcpFreeRxDataInd(dataInd_p);          
            dataInd_p = PNULL;
        }
        else
        {
            LOG_PDCP_MSG( PDCP_SDU_NOT_SENT_UPPER_LAYER, LOGWARNING, PDCP_RX, 
                    pdcpCurrentTime_g, __LINE__,dataInd_p->ueIndex, 
                    dataInd_p->lcId, dataInd_p->sn, ctx_p->last_Submitted_SN,
                    0,0, __func__,"");
            break;
        }
    }
}

#ifdef PDCP_ASYNC_INTERFACE
/****************************************************************************
 * Function Name  : pdcpRxProcessRoHC
 * Inputs         : dataInd_p -
 * Outputs        : None
 * Returns        : None
 * Description    : Performs Decompression of deciphered PDUs received from 
 *                  security engine.
 ****************************************************************************/
void pdcpRxProcessRoHC( LP_PdcpDataInd dataInd_p )
{
    LP_PdcpEnityContext ctx_p = PNULL;
    UInt8  *out_buff_p = PNULL;
    UInt32 eCode = PDCP_SUCCESS;
    rohc_api_info_t  rohc_api_data;

#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(dataInd_p->ueIndex);
    UInt32 activeIndexForPdcpStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForPdcpStats);
    LtePdcpKPIStats * pLtePdcpKPIStats = &(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].
            ltePdcpKPIStats[activeIndexForPdcpStats]);
    UInt8 qci = 0;
#endif
#if defined (PERF_STATS) && !defined(UE_SIM_TESTING)
    rohc_entity_t  *p_rohc_entity = PNULL;
#endif

    if ( PNULL == dataInd_p->data_p )
    {
        LOG_PDCP_MSG( PDCP_DATA_POINTER_NULL, LOGWARNING, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                dataInd_p->lcId, 0,0, 0,0,  __func__, 
                "pdcpRxDecipheredQ_g has null data");
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;
        return;
    }

    ctx_p = pdcpGetEntityContext( dataInd_p->ueIndex,
                                  dataInd_p->lcId, PDCP_ENTITY_RB_RX);
    if ( PNULL == ctx_p )
    {
        LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_RX, LOGWARNING, PDCP_RX,
                pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                dataInd_p->lcId, 0,0, 0,0, __func__,"");
        return;
    }
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    qci = ctx_p->qci;
#endif

    /*ROHC decompression (for DRB only)*/    
    if(PNULL != ctx_p->rohc.context)
    {
        rohc_api_data.api_id = ROHC_RLC_DATA_IND;
        rohc_api_data.entity_id.ueIndex = dataInd_p->ueIndex;
        rohc_api_data.entity_id.lcId = dataInd_p->lcId;
#if defined (PERF_STATS) && !defined(UE_SIM_TESTING)
        p_rohc_entity = (rohc_entity_t*)(ctx_p->rohc.context); 
        p_rohc_entity->rohc_entity_id.qci = ctx_p->qci;
#endif

/* SPR 20149 Start */
        rohc_api_data.in_buff_len = msgSize(dataInd_p->data_p,PNULL);
/* SPR 20149 End   */
        LOG_PDCP_MSG( PDCP_DATA_TO_ROHC_ENGINE, LOGDEBUG, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex,
                dataInd_p->lcId, dataInd_p->count,0,0,0,__func__,"");

        if( RO_SUCCESS != rfc3095_process_msg(ctx_p->rohc.context, 
                    dataInd_p->data_p,&out_buff_p,&rohc_api_data,&eCode) )
        {
            /*error during ROHC decompression, discarding*/
            LOG_PDCP_MSG( PDCP_ROHC_DECOMPRESSION_FAILED, LOGWARNING, PDCP_RX,
                    pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                    dataInd_p->lcId,dataInd_p->count, 0,0,0,__func__,"");
            pdcpStats.statsUEs[dataInd_p->ueIndex].drbs[dataInd_p->lcId - 3].\
                numberOfDrbUlPduDropDueToRoHcFailure += 1;
            /* Update KPI counters for UL PDU Drop due to ROHC Failure*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
            if(qci != 0)
                pLtePdcpKPIStats->totalUlLossRatePerQci[qci-1]++;
#endif

            pdcpFreeRxDataInd(dataInd_p);
            /*SPR 2498 changes start*/
            dataInd_p = PNULL;
            /*SPR 2498 changes end*/
            return;
        }
    }
    if ( LTE_TRUE == dataInd_p->discardAfterDecompression )
    {
        /* Discard the packet */
        LOG_PDCP_MSG( PDCP_OUT_WINDOW_DISCARD, LOGWARNING, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                dataInd_p->lcId, dataInd_p->count, 0,0,0,__func__,""); 
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;
    }
    else
    {
        /* No Decompression required, directly send the data to upper layer */
        pdcpDrbFinishPduRx( dataInd_p, ctx_p);
    }
}
#endif

/****************************************************************************
 * Function Name  : pdcpFpProcessDrbData
 * Inputs         : ctx_p - PDCP Entity context,
 *                  dataInd_p - current packet
 *                  hdr_p -
 * Outputs        : None
 * Returns        : None
 * Description    : enqueues data packets to Control PDU Tx Q
 ****************************************************************************/
void pdcpFpProcessDrbData(LP_PdcpEnityContext ctx_p,
        LP_PdcpDataInd dataInd_p, UInt8* hdr_p)
{
#ifdef PDCP_ASYNC_INTERFACE
    UInt8        *outData_p = PNULL;
    UInt16       result = 0;
#endif

#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(dataInd_p->ueIndex);
    UInt32 activeIndexForPdcpStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForPdcpStats);
    LtePdcpKPIStats * pLtePdcpKPIStats = &(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].
            ltePdcpKPIStats[activeIndexForPdcpStats]);
    UInt8 qci = ctx_p->qci;
#endif

    /*calc SN*/
    if(1 == GET_BYTESIZE_FROM_BITSIZE(ctx_p->snSize))
    {
        dataInd_p->sn = hdr_p[0] & PDCP_TS_PDU_DRB_SN_SMALL_MASK;
    }
    else
    {
        dataInd_p->sn = ( ( (UInt16) hdr_p[0] & 
                    PDCP_TS_PDU_DRB_SN_BIG_MASK_HI)<<8) | hdr_p[1];
    }

#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /*If rcvd SN is > next expected SN, there is a cavity
     * if rcvd SN is < next expected SN, it is a wrap around condition,
     * Increment the KPI counters accordingly*/
    if(qci != 0)
    {
       /* To calculate UL uu loss rate, totalUlDrbSduRcvd 
        * should also include num of lost packets. If packet is 
        * lost then incrementing totalUlDrbSduRcvd counter by num 
        * of lost packets else by 1.
        */
        if(dataInd_p->sn > ctx_p->next_SN)
        {
            pLtePdcpKPIStats->totalUlLossRatePerQci[qci-1] += 
                dataInd_p->sn - ctx_p->next_SN;

            pLtePdcpKPIStats->totalUlDrbSduRcvd[qci-1] += 
                  dataInd_p->sn - ctx_p->next_SN;
        }
        else if(dataInd_p->sn < ctx_p->next_SN)
        {
            UInt16 snRange = (1<<ctx_p->snSize) - 1;
            pLtePdcpKPIStats->totalUlLossRatePerQci[qci-1] +=
                snRange - ctx_p->next_SN + dataInd_p->sn + 1;
            
            pLtePdcpKPIStats->totalUlDrbSduRcvd[qci-1] +=
                snRange - ctx_p->next_SN + dataInd_p->sn + 1;
        }
        else 
        {
            pLtePdcpKPIStats->totalUlDrbSduRcvd[qci-1]++;
        }
    }
#endif
    if ( PDCP_ENTITY_RLC_AM == ctx_p->rlcMode)
    {
        if (PDCP_SUCCESS != pdcpProcessRx_DRB_AM( 
                    dataInd_p, ctx_p, dataInd_p->sn
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
		   , internalCellIndex
#endif
       ) )
        {
            return;
        }
    }
    else
    {
        if (dataInd_p->sn < ctx_p->next_SN)
        {
            ctx_p->hfn++;
        }

        dataInd_p->count = PDCP_MAKE_COUNT(dataInd_p->sn,ctx_p->hfn,ctx_p->snSize);
        LOG_PDCP_MSG( PDCP_SDU_SN_HFN_RX, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
                __LINE__,dataInd_p->ueIndex, dataInd_p->lcId, dataInd_p->sn,
                ctx_p->hfn, 0, 0,__func__,"");

        ctx_p->next_SN = dataInd_p->sn;
        PDCP_INCREMENT_SN(&ctx_p->next_SN,&ctx_p->hfn,ctx_p->snSize);
    }

    LOG_UT( PDCP_SDU_COUNT, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
            __LINE__, dataInd_p->ueIndex, dataInd_p->lcId, 
            dataInd_p->count, 0, 0,0, __func__,"");

#ifdef PDCP_ASYNC_INTERFACE
    /* Allocate memory for output buffer and pass the details to 
     * security engine. */
    outData_p = (UInt8 *)msgAlloc(rxOutMsgPool_g,0,0,0);
    if( PNULL == outData_p )          
    {
        LOG_PDCP_MSG( PDCP_MSG_ALLOC_FAILED, LOGFATAL, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__,0,0,0, dataInd_p->count,
                0, 0, __func__,"Allocation for deciphered output");
        ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;
        ltePanic("msgAlloc failed in %s", __FUNCTION__);
        return ;
    }
    /* Pass the shared context, input/output data and meta-data to 
     * security engine */
    result = pdcpSecurityProcessPacket( ctx_p->pdcpSecurityContext_p, 
            dataInd_p->data_p, outData_p, 
            (uaContextHandler)dataInd_p );
    if ( PDCP_SUCCESS != result )
    {
          /* SPR 16766 fix start */
          LOG_PDCP_WARNING( PDCP_RX, "Unable to provide the packet to SEC "
                  "engine, ue[%u] lcId[%u] count[%u]", dataInd_p->ueIndex, 
                  dataInd_p->lcId, dataInd_p->count );
          /* SPR 16766 fix end */
        msgFree( outData_p );
        outData_p = PNULL;
        msgFree(dataInd_p->data_p);
        dataInd_p->data_p = PNULL;
        pdcpFreeRxDataInd( dataInd_p );
        dataInd_p = PNULL;
        return ;
    }
#else
    /*Send to Decipher or Finish processing*/
    if (PDCP_SUCCESS != pdcpToDecipherRx(dataInd_p,ctx_p) )
    {
        return ;
    }

    pdcpDrbFinishPduRx(dataInd_p, ctx_p);
#endif
}

#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : pdcpProcessSpsRxPdu
 * Inputs         : UInt32 size, LP_PdcpDataInd dataInd_p,
 *                  LP_PdcpEnityContext ctx_p
 * Outputs        : None.
 * Returns        : PDCP_FAIL/PDCP_SUCCESS
 * Description    : Process SPS Rx pdu while finish Rx of DRB pdu
 ****************************************************************************/
static inline UInt32 pdcpProcessSpsRxPdu( UInt32 size, 
        LP_PdcpDataInd  dataInd_p, LP_PdcpEnityContext ctx_p)
{
    QSEGMENT segment = {0};
    UInt8 ulSpsInterval = 0;
    UInt8 voiceSilenceType = PDCP_VOICE_PKT;
    LP_PdcpUeContext ueCtx_p = PNULL;
    LP_PdcpSpsIntervalProfile spsIntProf_p = PNULL;
    UInt32 configuredSilPktSize = 0;  
    InternalCellIndex  cellIndex = INVALID_CELL_INDEX;
#ifdef LOG_PRINT_ENABLED 
    UInt32 state = 0;
#endif

    /* Get the UE context */ 
    ueCtx_p = PDCP_GET_VALID_UE_CONTEXT( dataInd_p->ueIndex);
    /* Get the SPS Interval and SPS profile param pointers */
    ulSpsInterval = ueCtx_p->semiPersistentSchedIntervalUL;
    
    cellIndex = ueCtx_p->cellIndex ;

    spsIntProf_p = pdcpContext.sCellInfo[cellIndex].
        spsProfileParams_p[ulSpsInterval];

    if( PNULL != spsIntProf_p )
    {
        /* Checks if LC is an SPS LC and enable Silence Detection is ON */
        if ( spsIntProf_p->enableSilenceDetection )
        {
            /* Following piece of code determines IP packet type only if 
             * SPS is enabled and will compute only once */
            if( NOT_DETERMINED == ctx_p->ipPktType )
            {
                UInt8 *buf_p = PNULL;
                if (PNULL == msgSegNext(dataInd_p->data_p, 0, &segment) )
                {
                    LOG_PDCP_MSG( PDCP_MSG_SEG_NEXT_FAILED, LOGWARNING, PDCP_RX,
                            pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                            dataInd_p->lcId, 0,0, 0,0, __func__, "");
                    pdcpFreeRxDataInd(dataInd_p);
                    dataInd_p = PNULL;	
                    return PDCP_FAIL;  
                } 
                buf_p = segment.base;
                /* As per below byte shown for IP header first 4 bits 
                 * contains packet type where 0x4 indicates IPv4 and 0x6 
                 * reperesents IPv6
                 *   0 1 2 3 	4 5 6 7 	 	
                 *   Version 	IHL      */
                if( 0x40 == (buf_p[0] & 0xF0) )
                {
                    ctx_p->ipPktType = IP_TYPE_IPV4;
                }
                else       
                {
                    ctx_p->ipPktType = IP_TYPE_IPV6;
                }
            }
            /* Following piece of code determines packet type information,
             * i.e, if it is SILENCE or VOICE */
            if( IP_TYPE_IPV4 == ctx_p->ipPktType )
            {
                configuredSilPktSize = spsIntProf_p->silencePdcpSduSizeIpv4;
            }
            else
            {
                configuredSilPktSize = spsIntProf_p->silencePdcpSduSizeIpv6;
            }
            if( configuredSilPktSize >= size )
            {
                voiceSilenceType = PDCP_SILENCE_PKT;
            }
        }
#ifdef LOG_PRINT_ENABLED 
        state = ueCtx_p->pdcpRxSpsState;
#endif
        /* Call SPS state machine for appropriate handling */
        pdcpSpsRxStateMachine[ueCtx_p->pdcpRxSpsState]
            [dataInd_p->macState][voiceSilenceType]( 
                    dataInd_p->ueIndex, dataInd_p->lcId, 
                    ueCtx_p->pdcpRxSpsState, dataInd_p->macState, ueCtx_p,
                    spsIntProf_p, cellIndex );
        LOG_PDCP_MSG( PDCP_UE_SPS_STATE, LOGINFO, PDCP_RX, pdcpCurrentTime_g,
                __LINE__, dataInd_p->ueIndex, state, dataInd_p->macState, ueCtx_p->pdcpRxSpsState, 
                dataInd_p->lcId,0, __func__, "PdcpSpsRxStateMachine");
    }
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpSpsConfigRecvd
 * Inputs         : ueIndex, lcId, state, event, ueCtx_p, spsIntProf_p, intCellIdx
 * Outputs        : None.
 * Returns        : Success
 * Description    : Changes the state from Idle to Configured
 ****************************************************************************/
UInt32 pdcpSpsConfigRecvd(UInt16 ueIndex, UInt8 lcId,
        UInt32 state, UInt32 event, LP_PdcpUeContext ueCtx_p, 
        LP_PdcpSpsIntervalProfile spsIntProf_p, InternalCellIndex intCellIdx )
{
    LOG_UT( PDCP_EVENT_RCVD, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g, 
            __LINE__, ueIndex, lcId, state, event, 
            0,0, __func__, "");

    ueCtx_p->pdcpRxSpsState = PDCP_SPS_CONFIGURED;
    ueCtx_p->numDeactivatedReceived = 0;

    LTE_GCC_UNUSED_PARAM(spsIntProf_p);
    LOG_PDCP_MSG( PDCP_UE_SPS_STATE, LOGINFO, PDCP_RX, pdcpCurrentTime_g,
            __LINE__, ueIndex, ueCtx_p->pdcpRxSpsState,intCellIdx,event, 
            state,lcId, __func__, "");

    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpSpsReleaseRecvd
 * Inputs         : ueIndex, lcId, state, event, ueCtx_p, spsIntProf_p, intCellIdx
 * Outputs        : None.
 * Returns        : Success
 * Description    : Changes the state to Idle from any state
 ****************************************************************************/
UInt32 pdcpSpsReleaseRecvd(UInt16 ueIndex, UInt8 lcId,
        UInt32 state, UInt32 event, LP_PdcpUeContext ueCtx_p, 
        LP_PdcpSpsIntervalProfile spsIntProf_p, InternalCellIndex intCellIdx )
{
    LOG_UT( PDCP_EVENT_RCVD, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g, 
            __LINE__, ueIndex, lcId, state, event, 
            0,0, __func__, "");

    ueCtx_p->pdcpRxSpsState = PDCP_SPS_IDLE;
    
  LTE_GCC_UNUSED_PARAM(spsIntProf_p)
    LOG_PDCP_MSG( PDCP_UE_SPS_STATE, LOGINFO, PDCP_RX, pdcpCurrentTime_g,
           __LINE__, ueIndex, ueCtx_p->pdcpRxSpsState,intCellIdx,event, 
           state,lcId, __func__, "");

    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpSpsVoiceInActivated
 * Inputs         : ueIndex, lcId, state, event, ueCtx_p, spsIntProf_p, intCellIdx
 * Outputs        : None.
 * Returns        : Success
 * Description    : The function handles use cases when voice packet received
 *                  with activated state from MAC.
 ****************************************************************************/
UInt32 pdcpSpsVoiceInActivated(UInt16 ueIndex, UInt8 lcId,
        UInt32 state, UInt32 event, LP_PdcpUeContext ueCtx_p, 
        LP_PdcpSpsIntervalProfile spsIntProf_p, InternalCellIndex intCellIdx )
{
    LOG_UT( PDCP_EVENT_RCVD, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g, 
            __LINE__, ueIndex, lcId, state, event, 
            0,0, __func__, "");

    ueCtx_p->pdcpRxSpsState = PDCP_SPS_ACTIVATED_SILENCE_NOT_DETECTED;
    ueCtx_p->numDeactivatedReceived = 0;
 
  LTE_GCC_UNUSED_PARAM(spsIntProf_p)
    LOG_PDCP_MSG( PDCP_UE_SPS_STATE, LOGINFO, PDCP_RX, pdcpCurrentTime_g,
           __LINE__, ueIndex, ueCtx_p->pdcpRxSpsState,intCellIdx,event, 
           state,lcId, __func__, "");

    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpSpsSilenceInActivated
 * Inputs         : ueIndex, lcId, state, event, ueCtx_p, spsIntProf_p, intCellIdx
 * Outputs        : None.
 * Returns        : Success
 * Description    : The function handles use cases when silence packet 
 *                  received with activated state from MAC.
 ****************************************************************************/
UInt32 pdcpSpsSilenceInActivated(UInt16 ueIndex, UInt8 lcId,
        UInt32 state, UInt32 event, LP_PdcpUeContext ueCtx_p, 
        LP_PdcpSpsIntervalProfile spsIntProf_p, InternalCellIndex intCellIdx )
{
    LOG_UT( PDCP_EVENT_RCVD, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g, 
            __LINE__, ueIndex, lcId, state, event, 
            0,0, __func__, "");

    ueCtx_p->pdcpRxSpsState = PDCP_SPS_ACTIVATED_SILENCE_DETECTED;
    
    LOG_PDCP_MSG( PDCP_UE_SPS_STATE, LOGINFO, PDCP_RX, pdcpCurrentTime_g,
           __LINE__, ueIndex, ueCtx_p->pdcpRxSpsState,state,lcId,
           0,0, __func__, "");

    if ( spsIntProf_p->enableSilenceDetection )
    {
        /* Notify MAC that silence is detected when it is in Activated 
         * State */
        macUeSpsVoiceSilenceDetection( ueIndex,PDCP_SILENCE_PKT, 
                                       event, intCellIdx );
    }
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpSpsVoiceInDeactivated
 * Inputs         : ueIndex, lcId, state, event, ueCtx_p, spsIntProf_p, intCellIdx
 * Outputs        : None.
 * Returns        : Success
 * Description    : The function handles use cases when silence packet 
 *                  received with deactivated state from MAC.
 ****************************************************************************/
UInt32 pdcpSpsVoiceInDeactivated(UInt16 ueIndex, UInt8 lcId,
        UInt32 state, UInt32 event, LP_PdcpUeContext ueCtx_p, 
        LP_PdcpSpsIntervalProfile spsIntProf_p, InternalCellIndex intCellIdx )
{
    LOG_UT( PDCP_EVENT_RCVD, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g, 
            __LINE__, ueIndex, lcId, state, event, 
            0,0, __func__, "");

    ueCtx_p->pdcpRxSpsState = PDCP_SPS_DEACTIVATED_VOICE_DETECTED;

    LOG_PDCP_MSG( PDCP_UE_SPS_STATE, LOGINFO, PDCP_RX, pdcpCurrentTime_g,
           __LINE__, ueIndex, ueCtx_p->pdcpRxSpsState,state,lcId, 
           spsIntProf_p->enableSilenceDetection,0, __func__, "");
    
    /* Notify MAC that voice is detected when it is in Deactivated State */
    macUeSpsVoiceSilenceDetection( ueIndex,PDCP_VOICE_PKT, event, 
            intCellIdx );
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpSpsSilenceInDeactivated
 * Inputs         : ueIndex, lcId, state, event, ueCtx_p, spsIntProf_p, intCellIdx
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS
 * Description    : The function handles use cases when silence packet 
 *                  received with deactivated state from MAC.
 ****************************************************************************/
UInt32 pdcpSpsSilenceInDeactivated(UInt16 ueIndex, UInt8 lcId,
        UInt32 state, UInt32 event, LP_PdcpUeContext ueCtx_p, 
        LP_PdcpSpsIntervalProfile spsIntProf_p, InternalCellIndex intCellIdx )
{
    LOG_UT( PDCP_EVENT_RCVD, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g, 
            __LINE__, ueIndex, lcId, state, event, 
            0,0, __func__, "");

    ueCtx_p->pdcpRxSpsState = PDCP_SPS_DEACTIVATED_VOICE_NOT_DETECTED;

  LTE_GCC_UNUSED_PARAM(spsIntProf_p)
    LOG_PDCP_MSG( PDCP_UE_SPS_STATE, LOGINFO, PDCP_RX, pdcpCurrentTime_g,
           __LINE__, ueIndex, ueCtx_p->pdcpRxSpsState,intCellIdx,event, 
           state,lcId, __func__, "");

    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpSpsVoiceSilenceInDeactivated
 * Inputs         : ueIndex, lcId, state, event, ueCtx_p, spsIntProf_p, intCellIdx
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS
 * Description    : The function handles use cases when voice/silence packet 
 *                  received with deactivated state from MAC.
 ****************************************************************************/
UInt32 pdcpSpsVoiceSilenceInDeactivated( UInt16 ueIndex, UInt8 lcId,
        UInt32 state, UInt32 event, LP_PdcpUeContext ueCtx_p, 
        LP_PdcpSpsIntervalProfile spsIntProf_p, InternalCellIndex intCellIdx )
{
    LOG_UT( PDCP_EVENT_RCVD, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g, 
            __LINE__, ueIndex, lcId, state, event, 
            0,0, __func__, "");

    if( MAX_DEACTIVATED_RECEIVED_IN_UL == ueCtx_p->numDeactivatedReceived )
    {
        ueCtx_p->pdcpRxSpsState = PDCP_SPS_DEACTIVATED_VOICE_NOT_DETECTED;
        ueCtx_p->numDeactivatedReceived = 0;
    }
    else
    {
        (ueCtx_p->numDeactivatedReceived)++;
    }
  LTE_GCC_UNUSED_PARAM(spsIntProf_p)
    LOG_PDCP_MSG( PDCP_UE_SPS_STATE, LOGINFO, PDCP_RX, pdcpCurrentTime_g,
           __LINE__, ueIndex, ueCtx_p->pdcpRxSpsState,intCellIdx,event, 
           state,lcId, __func__, "");

    return PDCP_SUCCESS;
}
#endif
