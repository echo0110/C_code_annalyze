/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpContext.c,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:13 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : PDCP context implementation 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpContext.c,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:13  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/31 05:20:33  gur19836
 * Fix for SPR 638, DRB Entity configuration in AM Mode
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.11.2.4  2009/07/14 05:44:59  gur20435
 * PDCP Optimizations
 *
 * Revision 1.11.2.3  2009/07/13 10:26:17  gur20435
 *
 * PDCP Optimizations
 *
 * Revision 1.11.2.2  2009/06/19 14:28:20  gur19836
 * Changes Done for Execution Scheduler
 *
 * Revision 1.11.2.1  2009/06/09 11:33:51  gur19140
 * *** empty log message ***
 *
 * Revision 1.11  2009/05/23 03:22:36  gur19479
 * modified for ROHC Tag processing
 *
 * Revision 1.10  2009/05/22 09:20:52  gur19836
 * ROHC Changes merged
 *
 * Revision 1.9  2009/05/22 06:04:24  gur20548
 * bugs fixed
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
 * Revision 1.5  2009/05/15 09:10:14  gur19140
 * bug fixes
 *
 * Revision 1.4  2009/05/15 05:37:56  gur19836
 * Incorporated CR Comments
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
#include "rohc_def.h"
#include "ltePdcpLteDefs.h"
#include "ltePdcpContext.h"
#include "ltePdcpErrors.h"
#include "ltePdcpProtocolTools.h"
#include "ltePdcpStatistic.h"
#include "rohc_main_ex.h"
#include "ltePdcpTSDefs.h"
#include "ltePdcpTxQueues.h"
#include "ltePdcpRxQueues.h"
#include "logging.h"
#include "alarm.h"
#include "ltePdcpExecutionLegs.h"
/* + SPR_16046 */
#include "ltePdcpRrcSap.h"
/* - SPR_16046 */
#ifdef PDCP_ASYNC_INTERFACE
#include "ltePdcpEncSap.h"
#endif

/******************************************************************************
  Private Definitions
 *****************************************************************************/
#define NUM_POOL_PDCP_SRB_AND_DRB_FOR_PDCP_HDR 2
#define NUM_POOL_PDCP_ASYNC_FOR_PDCP_HDR 2
#define NUM_POOL_PDCP_SRB_TX_FOR_MACI 1
#define NUM_POOL_PDCP_INTGERITY_CHECK_FOR_MACI 1

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
STATIC UInt16 pdcpEntityContextCreate( LP_PdcpEnityContext* entity_p, 
        UInt16 type, UInt16 rlcMode, UInt16 rbDirection, UInt8 radioBearer );
STATIC UInt16 pdcpEntitySetDiscardTimer( UInt16 ueIndex, UInt16 lcId, 
        UInt16 rbDirection, UInt16 discardTimer );
STATIC UInt16 pdcpEntitySetSnSize( UInt16 ueIndex, UInt16 lcId, 
        UInt16 rbDirection, UInt16 snSize );
#ifndef UE_SIM_TESTING
STATIC UInt16 pdcpEntitySetQci( UInt16 ueIndex, UInt16 lcId, 
        UInt16 rbDirection, UInt8 qci );
#endif
STATIC UInt16 pdcpEntitySetStatusReportRequired( UInt16 ueIndex, UInt16 lcId,
        UInt16 rbDirection, UInt8 StatusReportRequired );
/* SPR 3569 changes start */
STATIC UInt16 pdcpEntityResume( UInt16 ueIndex, UInt8 lcId, 
        UInt16 resumeDirection);
/* SPR 3569 changes end */
/* SRP 3608 changes start */
STATIC UInt16 pdcpEntitySetUeStatusReportRequired( UInt16 ueIndex, UInt8 lcId,
        UInt16 rbDirection, UInt8 StatusReportRequired );
/* SPR 3608 changes end */

/* + SPR 19066 */
tickType_t pdcpConvertTime(UInt32 inactiveTime);
/* - SPR 19066 */

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING 
/* SPR 15909 fix start */
extern tickType_t pdcpGlobalTTITickCount_g[MAX_NUM_CELL];
/* SPR 15909 fix start */
#endif                    
/* - Layer2 NON CA Changes */
/* SPR 15909 fix start */
extern tickType_t pdcpCurrentTime_g;
/* SPR 15909 fix end */


/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
UInt8 *pdcpFpProcessDrbRxBuf_p = PNULL;
UInt8 *pdcpFpProcessSrbRxBuf_p = PNULL;
UInt8 *pdcpProcessSrbTxMacI_p = PNULL;
UInt8 *integrityCheckMACIbaseptr_p = PNULL;
UInt8 *ipHdrWoOptBytes_p = PNULL;

#ifdef PDCP_ASYNC_INTERFACE
UInt8 *pdcpAsyncTxHeader_p = PNULL;
UInt8 *pdcpAsyncRxHeader_p = PNULL;
#endif
#ifdef L2_FAST_PATH
extern UInt8             pdcpIsDrbDataReceivedInTxQ_g;
#endif
PdcpContext    pdcpContext;
/* SRP 3608 changes start */
pdcpHandlerRrcStateT pdcpRrcTxStateMachine[PDCP_TX_MAX_STATE]
                                          [PDCP_MAX_RRC_TX_EVENT] =
{
    {   /* Idle State */
        pdcpSetConnectedState,
        pdcpInvalidState,
        pdcpInvalidState,
        pdcpSetHoState,
        pdcpInvalidState,
    },
    {   /* Connected State */
        pdcpInvalidState,
        pdcpSetSuspendedState,
        pdcpInvalidState,
        pdcpSetHoState,
        pdcpInvalidState,
    },
    {   /* Suspended State */
        pdcpSetConnectedState,
        pdcpInvalidState,
        pdcpSetResumeState, 
        /*SPR 23127 +*/
        pdcpInvalidState,
        /*SPR 23127 -*/
        pdcpSetAwaitedState,
    },
    {   /* Resume State */
        pdcpInvalidState,
        /* SPR 10805 FIX START*/
        pdcpSetSuspendedState,
        /* SPR 10805 FIX END*/
        pdcpSetConnectedState,
        /* SPR 10805 FIX START*/
        pdcpSetHoState, 
        /* SPR 10805 FIX END*/
        pdcpSetAwaitedState,
    },
    {   /* HO State */
        pdcpSetConnectedState,
        pdcpSetSuspendedState,
        pdcpInvalidState,
        pdcpInvalidState,
        pdcpSetAwaitedState,
    },
    {
        /*Awaited State */
        pdcpSetConnectedState,
        /* SPR 10805 FIX START*/
        pdcpSetSuspendedState,
        /* SPR 10805 FIX END*/
        pdcpSetResumeState, 
        /* SPR 10805 FIX START*/
        pdcpSetHoState, 
        /* SPR 10805 FIX END*/
        pdcpInvalidState,
    }

};
/* SRP 3608 changes end */

/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : pdcpContextCreateMemPool 
 * Inputs         : maxUeNumber - Maximum number of supported UEs
 *                  txReservationCoef - Number of blocks for TX purposes
 *                  rxReservationCoef - Number of blocks for RX purposes
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code.
 * Description    : Reserve memory in pools for PDCP Context purposes.
 ****************************************************************************/
/* +- SPR 17777*/
UInt16 pdcpContextCreateMemPool(UInt16 maxUeNumber, UInt16 txReservationCoef)
{
    /* Start 128 UE: Memory changes */
    /*
     ** 128 UE: Memory Changes: Replaced Macros which indicates maximum
     ** number of LC can be configured with the global variable.
     */

    /*Reserve memory for UE context */
    createMemPool( sizeof(PdcpUeContext), (maxUeNumber) );

    /*Reserve memory for PDCP Entity context */
    createMemPool( sizeof(PdcpEnityContext), 
            (pdcpNumOfSupportedLC_g * PDCP_ENTITY_RB_BOTH * maxUeNumber) );
    /*
     ** Moved createMemPool for MACI_LEN and GET_BYTESIZE_FROM_BITSIZE(PDCP_TS_DRB_SN_SIZE_BIG)
     ** (which is same as PDCP_MAX_HDR_SIZE) from pdcpStorageCreateMemPool(),
     ** It is being used in this function and was being used before creation.
     */
    /* 
     ** Reserve memory for for PDCP header alloactions
     **
     ** Message pool does take care for further allocation of this.
     */
    createMemPool((sizeof(UInt8) * PDCP_MAX_HDR_SIZE), NUM_POOL_PDCP_SRB_AND_DRB_FOR_PDCP_HDR);

#ifdef PDCP_ASYNC_INTERFACE
    createMemPool((sizeof(UInt8) * PDCP_MAX_HDR_SIZE), NUM_POOL_PDCP_ASYNC_FOR_PDCP_HDR);
#endif

    /*
     ** Reserve memory for for PDCP MAC-I field alloactions
     **
     ** Message pool does take care for further allocation of this.
     */
    createMemPool( MACI_LEN, NUM_POOL_PDCP_SRB_TX_FOR_MACI + NUM_POOL_PDCP_INTGERITY_CHECK_FOR_MACI);
    /* End 128 UE: Memory changes */

    /* Memory handling changes start */
#ifdef KPI_STATS
    createMemPool( sizeof(PdcpKpiStatsIndParams), 1);
    createMemPool(L2_RRM_API_HEADER_LEN + sizeof(PdcpKpiThpStatsIndParams), 1);
    createMemPool( sizeof(PdcpConfigureKpiStatsResp), 1);
    /* SPR 9068 Fix Start */
    createMemPool( sizeof(PdcpRrmConfigureKpiStatsResp), 1);
    /* SPR 9068 Fix End */
#endif
#ifdef PERF_STATS
    createMemPool(sizeof(TlvHeader) + sizeof(PdcpCellPerfStatsIndParams), 1);
    createMemPool(sizeof(TlvHeader) + sizeof(PdcpGetCellPerfStatsCnfParams), 1);
    createMemPool(sizeof(TlvHeader) + sizeof(LP_PdcpConfigurePerfStatsCnfParams), 1);
#endif
    createMemPool(sizeof(TlvHeader) + sizeof(PdcpOamCnf), 1);
    createMemPool(sizeof(TlvHeader) + sizeof(PdcpOamGetLogLevelResp), 1);
    createMemPool(sizeof(TlvHeader) + sizeof(PdcpOamGetLogCategoryResp), 1);
    /* Memory handling changes end */

    /*Reserve memory for ZCB allocations */
    pdcpContext.msgPool = 
        msgPoolCreate(0, (txReservationCoef + txReservationCoef)*maxUeNumber);
    if ( PNULL == pdcpContext.msgPool )
    {
        ltePanic("%s:msgPoolCreate fails.",__FUNCTION__);
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }

    /* Reserving memory for global pointers for PDCP for CSPL 
     * operations */
    pdcpFpProcessSrbRxBuf_p = getMemFromPool(
            (sizeof(UInt8)*PDCP_MAX_HDR_SIZE), PNULL); 
    if ( PNULL == pdcpFpProcessSrbRxBuf_p )
    {
        /*CRITICAL! Memory error*/
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, 
                sizeof(UInt8) * PDCP_MAX_HDR_SIZE, 0, 0,0, 0,0, 
                __func__, "Allocating pdcpFpProcessSrbRxBuf_p");
        ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
        ltePanic("MemAlloc fails for pdcpFpProcessSrbRxBuf_p" );
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }

    pdcpFpProcessDrbRxBuf_p = getMemFromPool(
            (sizeof(UInt8) * PDCP_MAX_HDR_SIZE), PNULL); 
    if ( PNULL == pdcpFpProcessDrbRxBuf_p )
    {
        /*CRITICAL! Memory error*/
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, 
                sizeof(UInt8) * PDCP_MAX_HDR_SIZE, 0, 0,0, 0,0, 
                __func__, "Allocating pdcpFpProcessDrbRxBuf_p");
        ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
        ltePanic("MemAlloc fails for pdcpFpProcessDrbRxBuf_p" );
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }

    pdcpProcessSrbTxMacI_p  = getMemFromPool(
            (sizeof(UInt8) * MACI_LEN), PNULL); 
    if ( PNULL == pdcpProcessSrbTxMacI_p )
    {
        /*CRITICAL! Memory error*/
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, 
                sizeof(UInt8) * MACI_LEN, 0, 0,0, 0,0, 
                __func__, "Allocating pdcpProcessSrbTxMacI_p");
        ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
        ltePanic("MemAlloc fails for pdcpProcessSrbTxMacI_p" );
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }

    integrityCheckMACIbaseptr_p = getMemFromPool(
            (sizeof(UInt8) * MACI_LEN), PNULL); 
    if ( PNULL == integrityCheckMACIbaseptr_p )
    {
        /*CRITICAL! Memory error*/
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, 
                sizeof(UInt8) * MACI_LEN, 0, 0,0, 0,0, 
                __func__, "Allocating integrityCheckMACIbaseptr_p");
        ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
        ltePanic("MemAlloc fails for integrityCheckMACIbaseptr_p" );
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }

    ipHdrWoOptBytes_p = getMemFromPool( 
            (sizeof(UInt8) * IP_HEADER_LENGTH_WO_OPT_BYTES) , PNULL);
    if( PNULL == ipHdrWoOptBytes_p )
    {
        /*CRITICAL! Memory error*/
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, 
                sizeof(UInt8) * IP_HEADER_LENGTH_WO_OPT_BYTES, 0, 0,0, 
                0,0, __func__, "Allocating ipHdrWoOptBytes_p");
        ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
        ltePanic("MemAlloc fails for ipHdrWoOptBytes_p" );
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }
#ifdef PDCP_ASYNC_INTERFACE
    pdcpAsyncTxHeader_p = getMemFromPool(
            (sizeof(UInt8) * PDCP_MAX_HDR_SIZE), PNULL); 
    if ( PNULL == pdcpAsyncTxHeader_p )
    {
        /*CRITICAL! Memory error*/
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, 
                sizeof(UInt8) * PDCP_MAX_HDR_SIZE, 0, 0,0, 0,0, 
                __func__, "Allocating pdcpAsyncTxHeader_p");
        ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
        ltePanic("MemAlloc fails for pdcpAsyncTxHeader_p" );
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }
    pdcpAsyncRxHeader_p = getMemFromPool(
            (sizeof(UInt8) * PDCP_MAX_HDR_SIZE), PNULL); 
    if ( PNULL == pdcpAsyncRxHeader_p )
    {
        /*CRITICAL! Memory error*/
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, 
                sizeof(UInt8) * PDCP_MAX_HDR_SIZE, 0, 0,0, 0,0, 
                __func__, "Allocating pdcpAsyncRxHeader_p");
        ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
        ltePanic("MemAlloc fails for pdcpAsyncRxHeader_p" );
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }
#endif
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpContextGetMsgPool 
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : MSGPOOL or PNULL if pool isn't allocated.
 * Description    : Returns memory pools for using with msgAlloc.
 ****************************************************************************/
MSGPOOL pdcpContextGetMsgPool( void )
{
    return pdcpContext.msgPool;
}

/****************************************************************************
 * Function Name  : pdcpContextInit 
 * Inputs         : LP_PdcpOamInitLayerReq initParams_p
 *                  cellId -
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code.
 * Description    : It inits PDCP Context data.
 ****************************************************************************/
UInt16 pdcpContextInit(LP_PdcpOamInitLayerReq initParams_p
#ifndef UE_SIM_TESTING
                       ,RrcCellIndex cellId
#endif
        )
{
    void        *rohc_entity_p = PNULL;
    UInt8       *in_buff_p = PNULL;
    UInt8       *out_buff_p = PNULL;
    UInt32      eCode = PDCP_SUCCESS;
    rohc_api_info_t  rohc_api_data;
#ifndef UE_SIM_TESTING
    /* SPR 20874 start*/
    UInt8 qciIndex = 0;
    /* SPR 20874 end  */
    LP_PdcpSpsIntervalProfile pdcpSpsProfCtx_p = PNULL;
    UInt8       idx = 0;
    UInt8       cellCount = 0;
    InternalCellIndex       cellIndex = INVALID_CELL_INDEX ;
#endif

    if ( LTE_TRUE == pdcpContext.isInitialized )
    {
        LOG_PDCP_MSG (PDCP_CONTEXT_ALREADY_INITIALIZED, LOGWARNING, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__,0,0,0,0, 0,0,__func__,"" );
        return PDCP_ERR_CONTEXT_ALREADY_INITIALIZED;
    }

    memSet( pdcpContext.ueContexts, 0, 
            /*CSR 40799 chg start*/
            MAX_UE_SUPPORTED* sizeof(LP_PdcpUeContext) );
            /*CSR 40799 chg end*/

    /* Store the parameters in global context */

#ifndef UE_SIM_TESTING
    pdcpContext.sCellCount = initParams_p->sCellCount + 1;
    cellIndex = PDCP_GET_INTERNAL_CELL_INDEX( cellId );

    /* coverity_60264  Fix Start */
    /* coverity_60836  Fix Start */
    if(INVALID_CELL_INDEX == cellIndex)
    {
      return PDCP_FAIL;
    }

    /* Store the parameters in global context */

    pdcpContext.isInitialized = LTE_TRUE;

    /* coverity_60836 Fix End */
    /* coverity_60264 Fix End */

    pdcpContext.sCellInfo[cellIndex].maxUeNumber = initParams_p->maxUeNumber;
    pdcpContext.sCellInfo[cellIndex].isSpsEnable = initParams_p->enableSps;
    if (LTE_TRUE == initParams_p->enableSps)
    {
         /* SPR 20874 start */
         /* memCpy( pdcpContext.sCellInfo[cellIndex].qciSpslist, 
                initParams_p->qciSpslist, MAX_QCI); */
         for (qciIndex = 0; qciIndex < MAX_QCI; qciIndex++)
         {
             if(qciIndex >= SPS_QCI)
             {
                 pdcpContext.sCellInfo[cellIndex].qciSpslist[qciIndex] = LTE_FALSE;
                 LOG_PDCP_INFO(PDCP_RRCOAM,"PDCP_INVALID_SPS_QCI_MAP");         
                 continue;
             }
             pdcpContext.sCellInfo[cellIndex].qciSpslist[qciIndex] = initParams_p->qciSpslist[qciIndex];
         }
         /* SPR 20874 end   */

        memCpy( pdcpContext.sCellInfo[cellIndex].supportedSpsDlIntervalList, 
                initParams_p->supportedSpsDlIntervalList, SPS_INTERVAL_LIST);
        memCpy( pdcpContext.sCellInfo[cellIndex].supportedSpsUlIntervalList, 
                initParams_p->supportedSpsUlIntervalList, SPS_INTERVAL_LIST);

        /* SPR_9578_Fix Start */
        createMemPool(sizeof (PdcpSpsIntervalProfile), SPS_INTERVAL_LIST);
        /* SPR_9578_Fix End */

        for( idx = 0; idx < SPS_INTERVAL_LIST; idx++ )
        {
            if( LTE_TRUE == initParams_p->supportedSpsDlIntervalList[idx] || 
                LTE_TRUE == initParams_p->supportedSpsUlIntervalList[idx] )
        {
            /*Allocate memory for SPS Interval context */
            pdcpSpsProfCtx_p = (LP_PdcpSpsIntervalProfile)getMemFromPool(
                    sizeof(PdcpSpsIntervalProfile), PNULL);
            if ( PNULL == pdcpSpsProfCtx_p )
            {
                /*CRITICAL! Memory error*/
                    LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, 
                             PDCP_RRCOAM, pdcpCurrentTime_g, __LINE__, 
                             sizeof(PdcpSpsIntervalProfile), 0, 0,0, 0,0,
                             __func__, "Allocating pdcpSpsProfCtx_p");
                ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                        CRITICAL_ALARM);
                return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
            }
            pdcpSpsProfCtx_p->enableSilenceDetection =
                    initParams_p->spsProfileParams[idx].enableSilenceDetection;
                pdcpSpsProfCtx_p->interPktDelayForSilenceDetection =
                    initParams_p->spsProfileParams[idx].\
                    interPktDelayForSilenceDetection;
                pdcpSpsProfCtx_p->silencePdcpSduSizeIpv4 =
                    initParams_p->spsProfileParams[idx].silencePdcpSduSizeIpv4;
                pdcpSpsProfCtx_p->silencePdcpSduSizeIpv6 =
                    initParams_p->spsProfileParams[idx].silencePdcpSduSizeIpv6;
                /* Store in context */
                pdcpContext.sCellInfo[cellIndex].spsProfileParams_p[idx] = 
                                                pdcpSpsProfCtx_p;
            }
        }
    }

    /* ECN start changes*/
    pdcpContext.pdcpEcnConfigParam[cellIndex].ecnFeatureStatus = 
        DEFAULT_ECN_FEATURE_STATUS;

    pdcpContext.pdcpEcnConfigParam[cellIndex].dlWaterMarkEnabled = 
        LTE_FALSE;
    /* ECN end changes*/

    if(initParams_p->sCellCount)
    {
        for(cellCount = 0;cellCount < initParams_p->sCellCount;cellCount++)
        {
            cellIndex = PDCP_GET_INTERNAL_CELL_INDEX(initParams_p->sCellInfo\
                                                            [cellCount].cellId);

            /* coverity_60836 Fix Start */
            if(INVALID_CELL_INDEX == cellIndex)
            {
                continue;
            }
            /* coverity_60836 Fix End */

            pdcpContext.sCellInfo[cellIndex].maxUeNumber = 
                            initParams_p->sCellInfo[cellCount].maxUeNumber;

            pdcpContext.sCellInfo[cellIndex].isSpsEnable = 
                            initParams_p->sCellInfo[cellCount].enableSps;
            
            if (LTE_TRUE == initParams_p->sCellInfo[cellCount].enableSps)
            {
        
            /* SPR 20874 start */
            
            /*   memCpy( pdcpContext.sCellInfo[cellIndex].qciSpslist,
                        initParams_p->sCellInfo[cellCount].qciSpslist, MAX_QCI);*/
            for (qciIndex = 0; qciIndex < MAX_QCI; qciIndex++)
            {
               if(qciIndex >= SPS_QCI)
               {
                   pdcpContext.sCellInfo[cellIndex].qciSpslist[qciIndex] = LTE_FALSE;
                   LOG_PDCP_INFO(PDCP_RRCOAM,"PDCP_INVALID_SPS_QCI_MAP");
                   continue;
               }
               pdcpContext.sCellInfo[cellIndex].qciSpslist[qciIndex] = initParams_p->sCellInfo[cellCount].qciSpslist[qciIndex];
            }

            /* SPR 20874 end  */ 
             
                memCpy( pdcpContext.sCellInfo[cellIndex].\
                        supportedSpsDlIntervalList, 
                        initParams_p->sCellInfo[cellCount].\
                        supportedSpsDlIntervalList, SPS_INTERVAL_LIST);

                memCpy( pdcpContext.sCellInfo[cellIndex].\
                        supportedSpsUlIntervalList, 
                        initParams_p->sCellInfo[cellCount].\
                        supportedSpsUlIntervalList, SPS_INTERVAL_LIST);

                /* SPR_9578_Fix Start */
                createMemPool(sizeof (PdcpSpsIntervalProfile), SPS_INTERVAL_LIST);
                /* SPR_9578_Fix End */

                for( idx = 0; idx < SPS_INTERVAL_LIST; idx++ )
                {
                    if( (LTE_TRUE == initParams_p->sCellInfo[cellCount].\
                                supportedSpsDlIntervalList[idx] )|| 
                            (LTE_TRUE == initParams_p->sCellInfo[cellCount].\
                             supportedSpsUlIntervalList[idx] ))
                    {
                        /*Allocate memory for SPS Interval context */
                        pdcpSpsProfCtx_p = (LP_PdcpSpsIntervalProfile)\
                         getMemFromPool(sizeof(PdcpSpsIntervalProfile), PNULL);

                        if ( PNULL == pdcpSpsProfCtx_p )
                        {
                            /*CRITICAL! Memory error*/
                            LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, 
                                    PDCP_RRCOAM, pdcpCurrentTime_g, __LINE__, 
                                    sizeof(PdcpSpsIntervalProfile), 0, 0,0, 0, 
                                    0, __func__,"Allocating pdcpSpsProfCtx_p");
                            ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID
                                    ,CRITICAL_ALARM);
                            return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
                        }
                        pdcpSpsProfCtx_p->enableSilenceDetection =
                            initParams_p->sCellInfo[cellCount].\
                spsProfileParams[idx].enableSilenceDetection;

                        pdcpSpsProfCtx_p->interPktDelayForSilenceDetection =
                            initParams_p->sCellInfo[cellCount].\
                            spsProfileParams[idx].\
                            interPktDelayForSilenceDetection;

                        pdcpSpsProfCtx_p->silencePdcpSduSizeIpv4 =
                            initParams_p->sCellInfo[cellCount].\
                spsProfileParams[idx].silencePdcpSduSizeIpv4;

                        pdcpSpsProfCtx_p->silencePdcpSduSizeIpv6 =
                            initParams_p->sCellInfo[cellCount].\
                spsProfileParams[idx].silencePdcpSduSizeIpv6;
            /* Store in context */
                        pdcpContext.sCellInfo[cellIndex].\
                            spsProfileParams_p[idx] = pdcpSpsProfCtx_p;
                    }
                }
            }

            /* ECN start changes*/
            pdcpContext.pdcpEcnConfigParam[cellIndex].\
                ecnFeatureStatus = DEFAULT_ECN_FEATURE_STATUS;

            pdcpContext.pdcpEcnConfigParam[cellIndex].\
                dlWaterMarkEnabled = LTE_FALSE;
            /* ECN end changes*/
        }
    }

#else
    pdcpContext.isInitialized = LTE_TRUE;
#endif

    pdcpContext.numOfActiveUe = 0;

    rohc_main_init();
    rohc_api_data.api_id = ROHC_RO_INITDB_REQ; 
    rohc_api_data.rohc_api_union.initdb_req.max_size_entity_pool 
        = PDCP_ROHC_SIZE_ENTITY_POOL;
    rohc_api_data.rohc_api_union.initdb_req.max_size_csc_pool 
        =  PDCP_ROHC_SIZE_CSC_POOL;
    rohc_api_data.rohc_api_union.initdb_req.max_size_dsc_pool 
        =  PDCP_ROHC_SIZE_DSC_POOL;
    if(RO_SUCCESS != rfc3095_process_msg(rohc_entity_p,in_buff_p,
                &out_buff_p, &rohc_api_data, &eCode))
    {
        return PDCP_FAIL;
    }
    /* Initialize all the stats at the time of init*/
    pdcpInitStats();

    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpContextCleanup 
 * Inputs         :     
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code.
 * Description    : It cleans PDCP Context data.
 ****************************************************************************/
UInt16 pdcpContextCleanup(void)
{
#ifndef UE_SIM_TESTING
    UInt8   idx     = 0;
    UInt8   cellCount = 0;
#endif
    UInt16  ueIndex = 0;
    UInt16  result = PDCP_SUCCESS;

    if ( LTE_FALSE == pdcpContext.isInitialized )
    {
        LOG_PDCP_MSG (PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__,0,0,0,0, 0,0,__func__,"" );
        return PDCP_ERR_CONTEXT_NOT_INITIALIZED;
    }

    pdcpContext.isInitialized = LTE_FALSE;
    /*Cleanup all UE contexts*/
    for (ueIndex = 0; ueIndex < pdcpContext.maxUeNumber; ueIndex++)
    {
        /* Check ueContex to avoid warnings */
        if ( PNULL != pdcpContext.ueContexts[ueIndex] )
        {
            /*cleanup UE context*/
            result = pdcpUeContextDelete( ueIndex );
            if ( PDCP_SUCCESS != result )
            {
                LOG_PDCP_MSG( PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, 
                        pdcpCurrentTime_g, __LINE__, result, ueIndex, 0,0, 
                        0,0,__func__,"Delete UE fail" );
            }
        }
    }
    pdcpContext.maxUeNumber = 0;

    /*Deallocate memory for ueContexts array*/
    /*CSR 40799 chg start*/
    memSet( pdcpContext.ueContexts, 0, MAX_UE_SUPPORTED* sizeof(LP_PdcpUeContext) );
    /*CSR 40799 chg end*/
    pdcpContext.numOfActiveUe = 0;

#ifndef UE_SIM_TESTING
    for(cellCount = 0;cellCount < pdcpContext.sCellCount;cellCount++)
    { 
        for( idx = 0; idx < SPS_INTERVAL_LIST; idx++ )
        {
            if ( PNULL != pdcpContext.sCellInfo[cellCount].
                    spsProfileParams_p[idx] )
            {
                freeMemPool( pdcpContext.sCellInfo[cellCount].
                        spsProfileParams_p[idx] );

                pdcpContext.sCellInfo[cellCount].spsProfileParams_p[idx] = 
                    PNULL;
            }
        }
    }
#endif
    /* Initialize all the stats at the time of context delete*/
    pdcpInitStats();
    /* Coverity 54622/54623*/
    /* Doing Clean for ROHC Memory */
    rohcFreeFromPool();
    /* Coverity 54622/54623*/
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpContextIsInitialized
 * Inputs         :     
 * Outputs        : None.
 * Returns        : LTE_TRUE / LTE_FALSE
 * Description    : .
 ****************************************************************************/
UInt16 pdcpContextIsInitialized(void)
{
    return pdcpContext.isInitialized;
}

/****************************************************************************
 * Function Name  : setPdcpContextInitialized
 * Inputs         : flag
 * Outputs        : Number of Active UEs
 * Returns        : None.
 * Description    : The function gives the number of active UEs
 ****************************************************************************/
void setPdcpContextInitialized(UInt16 flag)
{
    pdcpContext.isInitialized = flag;
}

/****************************************************************************
 * Function Name  : pdcpNumActiveUe
 * Inputs         : None   
 * Outputs        : None.
 * Returns        : Number of Active UEs
 * Description    : The function gives the number of active UEs
 ****************************************************************************/
UInt16 pdcpNumActiveUe (void)
{
    return pdcpContext.numOfActiveUe;
}

/****************************************************************************
 * Function Name  : pdcpContextGetMaxUeNumber
 * Inputs         :     
 * Outputs        : None.
 * Returns        : Maximum number of supported UEs.
 * Description    : .
 ****************************************************************************/
UInt16 pdcpContextGetMaxUeNumber(void)
{
    return pdcpContext.maxUeNumber;
}

/****************************************************************************
 * Function Name  : pdcpUeContextCreate
 * Inputs         : ueIndex - UE Id.
 *                  crnti - unique identification of UE.
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It creates PDCP UE context.
 ****************************************************************************/
UInt16 pdcpUeContextCreate( UInt16 ueIndex, UInt16 crnti)
{
    LP_PdcpUeContext pdcpUeContext_p = PNULL;    
#ifdef LOG_PRINT_ENABLED 
    UInt32 state = 0;
#endif
    /* Check ueIndex */
    if ( ueIndex >= pdcpContext.maxUeNumber )
    {
        LOG_PDCP_MSG (PDCP_INVALID_UE, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, ueIndex, pdcpContext.isInitialized,0,0, 0,0,
                __func__,"");
        return PDCP_ERR_TLV_PARSING_INVALID_UE_ID;
    }

    /* check UE context */
    if ( PNULL != pdcpContext.ueContexts[ueIndex] )
    {
        LOG_PDCP_MSG( PDCP_UE_CTX_ALREADY_CREATED, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, ueIndex, 0, 0, 0, 0,0, 
                __func__,"");
        return PDCP_ERR_UE_CONTEXT_ALREADY_CREATED;
    }

    /*Allocate memory for UE context */
    pdcpUeContext_p = (LP_PdcpUeContext)
        getMemFromPool( sizeof(PdcpUeContext), PNULL);
    if ( PNULL == pdcpUeContext_p )
    {
        /*CRITICAL! Memory error*/
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, sizeof(PdcpUeContext), ueIndex,
                0,0, 0,0,__func__,"Allocating pdcpUeContext_p");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("MemAlloc fails for ueContext allocation ueIndex[%u].",
                ueIndex);
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }

    /*reset UE Context*/
    memSet( pdcpUeContext_p, 0, sizeof(PdcpUeContext) );

    pdcpUeContext_p->crnti = crnti;

    pdcpContext.ueContexts[ueIndex] = pdcpUeContext_p;

    /* Change thr UE RX state to connected state */
    pdcpContext.ueContexts[ueIndex]->pdcpRxState = PDCP_RX_STATE_CONNECTED;

#ifdef LOG_PRINT_ENABLED 
    state = pdcpUeContext_p->pdcpTxSrb1State;
#endif
    /* Change thr UE TX state to connected state */
    (void)pdcpRrcTxStateMachine[pdcpUeContext_p->pdcpTxSrb1State]
        [PDCP_CONNECTED] ( ueIndex, LTE_SRB_LC_ID_LOW_BOUND, 
                pdcpUeContext_p->pdcpTxSrb1State, PDCP_CONNECTED, 
                pdcpUeContext_p );

    LOG_PDCP_MSG( PDCP_UE_CONTEXT_STATE_TX, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
              __LINE__, ueIndex, state, PDCP_CONNECTED,
              pdcpUeContext_p->pdcpTxSrb1State, LTE_SRB_LC_ID_LOW_BOUND, 0, __func__,
              "PdcpRrcTxStateMachine");

#ifdef LOG_PRINT_ENABLED 
    state = pdcpUeContext_p->pdcpTxSrb2DrbState;
#endif
    pdcpUeContext_p->event = PDCP_CONNECTED;
    (void)pdcpRrcTxStateMachine[pdcpUeContext_p->pdcpTxSrb2DrbState]
        [pdcpUeContext_p->event] ( ueIndex, LTE_SRB_LC_ID_HIGH_BOUND,
                pdcpUeContext_p->pdcpTxSrb2DrbState, 
                pdcpUeContext_p->event, pdcpUeContext_p );

    LOG_PDCP_MSG( PDCP_UE_CONTEXT_STATE_TX, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
              __LINE__, ueIndex, state, pdcpUeContext_p->event,
              pdcpUeContext_p->pdcpTxSrb2DrbState, LTE_SRB_LC_ID_HIGH_BOUND, 0, __func__,
              "PdcpRrcTxStateMachine");

    /* Initialize SPS state to IDLE */
    pdcpUeContext_p->pdcpRxSpsState = PDCP_SPS_IDLE;   
    /* SPR 6758 changes start */
    /* Initialize for hoTriggerRecvd to FALSE */
    pdcpUeContext_p->hoTriggerRecvd = LTE_FALSE;
    /* SPR 6758 changes end */
    /*SPR 51542 Start*/
    pdcpUeContext_p->dlFwdingQInitilized = LTE_FALSE;
    /*SPR 51542 End*/
    /* SPR 10894 fix start*/
    pdcpUeContext_p->newUeIndex = INVALID_UE_INDEX;
    /* SPR 10894 fix end*/
    /* + SPR 19066 */
    /* Store the current tick in for UE inactivity */
    pdcpUeContext_p->lastSchedTickDl = pdcpCurrentTime_g;
    pdcpUeContext_p->lastSchedTickUl = pdcpCurrentTime_g;
   /* + SPR 19066 */

    pdcpContext.numOfActiveUe += 1;

    /* Updates the list of active UEs*/
    updateActiveUeInfo(ueIndex, 
            pdcpContext.activeUeInfo,
            pdcpContext.activeUeIndex 
            );

    /* reset statistic for UE */
    pdcpStats.statNumActiveUe += 1;
    pdcpStats.statsUEs[ueIndex].crnti = crnti;

    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpUeContextDelete
 * Inputs         : ueIndex - UE Id.
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It deletes PDCP UE context and all PDCP Entities within
 *                  this UE.
 ****************************************************************************/
UInt16 pdcpUeContextDelete( UInt16 ueIndex )
{
    UInt16 lcId = 0;
    UInt16 result = 0;
    UInt16 numOfActiveLc = 0;

    /* Check ueIndex */
    if ( ueIndex >= pdcpContext.maxUeNumber )
    {
        LOG_PDCP_MSG (PDCP_INVALID_UE, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, ueIndex, pdcpContext.isInitialized, 0,0, 0,0, 
                __func__,"");
        return PDCP_ERR_TLV_PARSING_INVALID_UE_ID;
    }

    if ( PNULL == pdcpContext.ueContexts[ueIndex] )
    {
        LOG_PDCP_MSG( PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, ueIndex, pdcpContext.isInitialized,
                0,0, 0,0, __func__,"");
        return PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
    }

    /* Associate current tick with UE for deletion to take place */
    pdcpContext.ueContexts[ueIndex]->tickAss = pdcpCurrentTime_g;

    /* SPR 2717 changes start */
    numOfActiveLc = pdcpContext.ueContexts[ueIndex]->numOfActiveSrbLc;
    /* Cleanup all PDCP Entites (range 1..LTE_MAX_LC_ID) */
    for (lcId = 0; lcId < numOfActiveLc; lcId++)
    {
        result = pdcpSrbEntityFree( ueIndex, lcId + 1 );
        if ( PDCP_SUCCESS != result)
        {
            LOG_PDCP_MSG( PDCP_LC_DELETE_FAILURE, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, ueIndex, lcId + 1,
                    result, 0, 0,0, __func__,"");
            return result;
        }
    }
    /* SPR 2717 changes end */
    numOfActiveLc = pdcpContext.ueContexts[ueIndex]->numOfActiveDrbLc;

    /* SPR 2933 changes start */
    for ( lcId = numOfActiveLc; lcId > 0; lcId-- )
        /* SPR 2933 changes end */
    {
        result = pdcpDrbEntityFree( ueIndex, 
                pdcpContext.ueContexts[ueIndex]->activeLcInfo[lcId -1] );
        if ( PDCP_SUCCESS != result)
        {
            LOG_PDCP_MSG( PDCP_LC_DELETE_FAILURE, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, ueIndex, 
                    pdcpContext.ueContexts[ueIndex]->activeLcInfo[lcId],
                    result, 0, 0,0, __func__,"");
            break;
        }
    }
    if( PDCP_SUCCESS == result )
    {
        /*SPR 51542 Start*/
        /* Destroy the semaphore for the queue initialized */
        LTE_QUEUE * queue_p = 
            &(pdcpContext.ueContexts[ueIndex]->pdcpDataForwardingDlQ);
        semDestroy((LTE_SEM *)&queue_p->sem_lock);
        /*SPR 51542 End*/

        /*Deallocate memory for UE Context*/
        freeMemPool( pdcpContext.ueContexts[ueIndex] );
        pdcpContext.ueContexts[ueIndex] = PNULL;

        /* Updates the list of active UEs*/
        updateActiveUeInfo(ueIndex, 
                pdcpContext.activeUeInfo, 
                pdcpContext.activeUeIndex 
                );

        /*reset UE Context*/
        pdcpContext.numOfActiveUe -= 1;
        pdcpStats.statNumActiveUe -= 1;

    }
    return PDCP_SUCCESS;
}

/* SPR_1235_FIX_START*/
/****************************************************************************
 * Function Name  : pdcpEntitySetDirectionFlag
 * Inputs         : ueIndex - UE Id.
 *                  lcId - logical channel id.
 *                  rbDirection - Radio bearer link type (TX, RX, Both).
 *                  type - entity type (PDCP_ENTITY_SRB, PDCP_ENTITY_DRB).
 * Outputs        : None.
 * Returns        : void
 * Description    : It sets isDirection field set for SRB reconfiguration 
 *                  request.
 ****************************************************************************/
void pdcpEntitySetDirectionFlag( 
        UInt16 ueIndex, 
        UInt16 lcId, 
        UInt16 rbDirection,
        UInt16 type )
{
    LP_PdcpEnityContext  entity_p = PNULL;

    do 
    {
        /* Find TX context for UE Index and LC Id */
        entity_p = pdcpGetEntityContext( ueIndex, lcId, PDCP_ENTITY_RB_TX );
        if ( PNULL == entity_p )
        {
            break;
        }
        if ( PDCP_ENTITY_RB_RX == rbDirection )
        {
            if (PDCP_ENTITY_SRB == type)
            {
                entity_p->isDirectionSetSrb = PDCP_DIRECTION_NOT_SET;
            }
            else
            {
                entity_p->isDirectionSetDrb = PDCP_DIRECTION_NOT_SET;
            }
        }
        else 
        {
            if ( PDCP_ENTITY_SRB == type )
            {
                entity_p->isDirectionSetSrb = PDCP_DIRECTION_SET;
            }
            else
            {
                entity_p->isDirectionSetDrb = PDCP_DIRECTION_SET;
            }
        }
    }while (0);

    entity_p = PNULL;
    do
    {
        /* Find RX context for UE Index and LC Id */
        entity_p = pdcpGetEntityContext( ueIndex, lcId, PDCP_ENTITY_RB_RX );
        if ( PNULL == entity_p )
        {
            break;
        }
        if ( PDCP_ENTITY_RB_TX == rbDirection )
        {
            if ( PDCP_ENTITY_SRB == type )
            {
                entity_p->isDirectionSetSrb = PDCP_DIRECTION_NOT_SET;
            }
            else
            {
                entity_p->isDirectionSetDrb = PDCP_DIRECTION_NOT_SET;
            }
        }
        else 
        {
            if ( PDCP_ENTITY_SRB == type )
            {
                entity_p->isDirectionSetSrb = PDCP_DIRECTION_SET;
            }
            else
            {
                entity_p->isDirectionSetDrb = PDCP_DIRECTION_SET;
            }
        }
    }while(0);  
}
/* SPR_1235_FIX_END*/

/****************************************************************************
 * Function Name  : pdcpEntityCreate
 * Inputs         : ueIndex - UE Id.
 *                  lcId - logical channel id.
 *                  type - entity type (PDCP_ENTITY_SRB, PDCP_ENTITY_DRB).
 *                  rlcMode - RLC link mode (UM, AM).
 *                  rbDirection - Radio bearer link type (TX, RX, Both).
 *                  radioBearer - Radio Bearer Id
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It creates TX and/or RX PDCP Entities for RB which is
 *                  identified ueIndex, lcId.
 ****************************************************************************/
UInt16 pdcpEntityCreate( UInt16 ueIndex, UInt16 lcId, UInt16 type, 
        UInt16 rlcMode, UInt16 rbDirection, 
        /* SPR 3276 changes start */
        UInt8 radioBearer )
/* SPR 3276 changes end */
{
    UInt16 result = PDCP_SUCCESS;

    /* Check ueIndex */
    if ( ueIndex >= pdcpContext.maxUeNumber )
    {
        LOG_PDCP_MSG (PDCP_INVALID_UE, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, ueIndex, pdcpContext.isInitialized, 0,0, 0,0, 
                __func__,"");
        return PDCP_ERR_TLV_PARSING_INVALID_UE_ID;
    }

    /*Check UE context*/
    if ( PNULL == pdcpContext.ueContexts[ueIndex] )
    {
        LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, ueIndex, pdcpContext.isInitialized,
                0,0, 0,0, __func__,"");
        return PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
    }

    /* input range 1..LTE_MAX_LC_ID - storage range 0..LTE_MAX_LC_ID-1 */
    lcId--; 

    /* Check lcId */
    if ( LTE_MAX_LC_ID <= lcId )
    {
        LOG_PDCP_MSG (PDCP_INVALID_LC, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, lcId+1, ueIndex, 0,0, 0,0, __func__,"");
        return PDCP_ERR_TLV_PARSING_INVALID_LC_ID;
    }

    if ( PDCP_ENTITY_RB_BOTH == rbDirection )
    {
        /* dirrection BOTH - create TX entity */
        result = pdcpEntityContextCreate( 
                &pdcpContext.ueContexts[ueIndex]->pdcpEntities[lcId][PDCP_ENTITY_RB_TX],
                type,
                rlcMode,
                PDCP_ENTITY_RB_TX, 
                /* SPR 3276 changes start */
                radioBearer );
        /* SPR 3276 changes end */

#ifdef PDCP_ASYNC_INTERFACE
        pdcpCreateSecContext(
                /* SPR 15236 fix start */
                ueIndex, lcId, 
                /* SPR 15236 fix start */
                pdcpContext.ueContexts[ueIndex]->pdcpEntities[lcId]
                [PDCP_ENTITY_RB_TX],
                /* SPR 3276 changes start */
                radioBearer );
        /* SPR 3276 changes end */
#endif
        /* set dirrection to RX */
        rbDirection = PDCP_ENTITY_RB_RX;   
    }

    if ( PDCP_SUCCESS == result )
    {
        /* dirrection is one or RX (for BOTH) - create it */
        result = pdcpEntityContextCreate( 
                &pdcpContext.ueContexts[ueIndex]->pdcpEntities[lcId][rbDirection],
                type,
                rlcMode,
                rbDirection,
                /* SPR 3276 changes start */
                radioBearer );
        /* SPR 3276 changes end */
#ifdef PDCP_ASYNC_INTERFACE
        pdcpCreateSecContext(
                /* SPR 15236 fix start */
                ueIndex, lcId, 
                /* SPR 15236 fix start */
                pdcpContext.ueContexts[ueIndex]->pdcpEntities[lcId][rbDirection],
                /* SPR 3276 changes start */
                radioBearer );
        /* SPR 3276 changes end */
#endif
    }

    /* SPR 3189 changes start */
    if ( PDCP_SUCCESS == result )
        /* SPR 3189 changes end */
    {
        if ( (lcId+1) <= LTE_SRB_LC_ID_HIGH_BOUND )
        {
            pdcpContext.ueContexts[ueIndex]->numOfActiveSrbLc += 1;
            pdcpStats.statsUEs[ueIndex].statNumActiveSrbLc += 1;

            /* SPR 3189 changes start */
            pdcpStats.statsUEs[ueIndex].srbs[lcId].srbId = (lcId+1);
            /* SPR 3189 changes end */
        }
        else
        {
            /* Updates the list of active DRBs*/
            pdcpContext.ueContexts[ueIndex]->numOfActiveDrbLc += 1;
            pdcpStats.statsUEs[ueIndex].statNumActiveDrbLc += 1;
            updateActiveLcInfo(ueIndex, (lcId + 1), 
                    pdcpContext.ueContexts[ueIndex]->activeLcInfo,
                    pdcpContext.ueContexts[ueIndex]->activeLcIndex 
                    );
            /* SPR 3189 changes start */
            pdcpStats.statsUEs[ueIndex].drbs[lcId - 2].lcId = (lcId+1);
            /* SPR 3189 changes end */
        }
    }
    return result;
}

/****************************************************************************
 * Function Name  : pdcpSrbEntityFree
 * Inputs         : ueIndex - UE Id.
 *                  lcId - logical channel id.
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : To free TX/RX contexts for given ueIndex and lcId(SRB)
 ****************************************************************************/
UInt16 pdcpSrbEntityFree( UInt16 ueIndex, UInt16 lcId )
{
    UInt16 result = PDCP_SUCCESS;
    UInt16 dir = 0;
    LP_PdcpEnityContext entity_p = PNULL;

    /*Check UE context*/
    if ( PNULL == pdcpContext.ueContexts[ueIndex] )
    {
        LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, ueIndex, pdcpContext.isInitialized,
                0,0, 0,0, __func__,"");
        return PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
    }

    /*Coverity 10193, 10208 Fix Start*/ 
    /* Check lcId */
    if ( (lcId < LTE_SRB_LC_ID_LOW_BOUND) || (lcId > LTE_SRB_LC_ID_HIGH_BOUND))
    {
        LOG_PDCP_MSG (PDCP_INVALID_LC, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, lcId + 1, 0,0,0, 0,0, __func__,"");
        return PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
    }
    /*Coverity 10193, 10208 Fix End*/ 

    /* input range 1..2 - storage range 0..1 */
    lcId--; 

    result = PDCP_ERR_ENTITY_NOT_FOUND;

#if  defined(PERF_STATS) && !defined(UE_SIM_TESTING)
    LteUePdcpRadioBearerInfoStats *ueRadioBearerInfoStats_p =
        &(gPerfStats.perfStats[0].lteUePerfStats[ueIndex].
                lteUePdcpRadioBearerInfoStats[lcId + 1]);

    LteUePdcpRadioBearerInfoStats *ueRadioBearerInfoStatsAtFirstIndex_p =
        &(gPerfStats.perfStats[1].lteUePerfStats[ueIndex].
                lteUePdcpRadioBearerInfoStats[lcId + 1]);
    ueRadioBearerInfoStats_p->bearerId =  INVALID_VALUE;
    ueRadioBearerInfoStats_p->bearerType = INVALID_VALUE;
    ueRadioBearerInfoStats_p->qci = INVALID_VALUE;
    ueRadioBearerInfoStats_p->rlcMode = INVALID_VALUE;

    ueRadioBearerInfoStatsAtFirstIndex_p->bearerId =  INVALID_VALUE;
    ueRadioBearerInfoStatsAtFirstIndex_p->bearerType = INVALID_VALUE;
    ueRadioBearerInfoStatsAtFirstIndex_p->qci = INVALID_VALUE;
    ueRadioBearerInfoStatsAtFirstIndex_p->rlcMode = INVALID_VALUE;
#endif
    for(dir=PDCP_ENTITY_RB_TX; dir<=PDCP_ENTITY_RB_RX; dir++)
    {
        entity_p = pdcpContext.ueContexts[ueIndex]->pdcpEntities[lcId][dir];
        if ( PNULL != entity_p )
        {
#ifdef PDCP_ASYNC_INTERFACE
            pdcpDeleteSecurityContext( entity_p->pdcpSecurityContext_p );
            entity_p->pdcpSecurityContext_p = PNULL;

#endif
            /* SPR 3159 changes start */
            /* SPR 4183 changes start */
            pdcpEntityContextFree(entity_p, ueIndex, lcId + 1, dir);
            /* SPR 4183 changes end */
            /* SPR 3159 changes end */
            /*SPR 51542 Start*/
            if(PDCP_ENTITY_RB_TX == dir)
            {
                semDestroy((LTE_SEM *)&(entity_p->pdcpTxDataReqQ).sem_lock);
                semDestroy((LTE_SEM *)&(entity_p->pdcpStorageQ).sem_lock);
            }
            else if (dir == PDCP_ENTITY_RB_RX)
            {
                /* + SPR 19525 */
                DEINIT_PDCP_RX_DATA_IND_Q( entity_p );
                /* - SPR 19525 */
                semDestroy((LTE_SEM *)&(entity_p->pdcpUlReorderingQ).sem_lock);
            }
            /*SPR 51542 Start*/
            freeMemPool(entity_p);
            entity_p = PNULL;
            pdcpContext.ueContexts[ueIndex]->pdcpEntities[lcId][dir] = PNULL;
            result = PDCP_SUCCESS;
        }
    }
    pdcpContext.ueContexts[ueIndex]->numOfActiveSrbLc -= 1;
    pdcpStats.statsUEs[ueIndex].statNumActiveSrbLc -= 1;

    /* SPR 3189 changes start */
    pdcpStats.statsUEs[ueIndex].srbs[lcId].srbId = 0;
    /* SPR 3189 changes end */
    return result;
}

/****************************************************************************
 * Function Name  : pdcpDrbEntityFree
 * Inputs         : ueIndex - UE Id.
 *                  lcId - logical channel id.
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : To free TX/RX contexts for given ueIndex and lcId(DRB)
 ****************************************************************************/
UInt16 pdcpDrbEntityFree( UInt16 ueIndex, UInt16 lcId )
{
    UInt16 result = PDCP_SUCCESS;
    LP_PdcpEnityContext entity_p = PNULL;
    LP_PdcpUeContext    ueCtx_p = PNULL;
    /* SPR 5219 changes start */
    UInt8 localIsEmiFwded = LTE_FALSE;
    /* SPR 5219 changes end */
    /* SPR 16770 fix start */
    UInt8 localIsFwdingEnabled = LTE_FALSE;
    /* SPR 16770 fix end */

    /*Check UE context*/
    ueCtx_p = PDCP_GET_UE_CONTEXT( ueIndex );
    if ( PNULL == ueCtx_p )
    {
        LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, ueIndex, pdcpContext.isInitialized,
                0,0, 0,0, __func__,"");
        return PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
    }

    /* input range 1..LTE_MAX_LC_ID - storage range 0..LTE_MAX_LC_ID-1 */
    lcId--; 

    /* Check lcId */
    if ( LTE_MAX_LC_ID <= lcId )
    {
        LOG_PDCP_MSG (PDCP_INVALID_LC, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, lcId + 1, 0,0,0, 0,0, __func__,"");
        return PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
    }

    result = PDCP_ERR_ENTITY_NOT_FOUND;


#if  defined(PERF_STATS) && !defined(UE_SIM_TESTING)
    LteUePdcpRadioBearerInfoStats *ueRadioBearerInfoStats_p =
        &(gPerfStats.perfStats[0].lteUePerfStats[ueIndex].
                lteUePdcpRadioBearerInfoStats[lcId + 1]);

    LteUePdcpRadioBearerInfoStats *ueRadioBearerInfoStatsAtFirstIndex_p =
        &(gPerfStats.perfStats[1].lteUePerfStats[ueIndex].
                lteUePdcpRadioBearerInfoStats[lcId + 1]);

    ueRadioBearerInfoStats_p->bearerId =  INVALID_VALUE;
    ueRadioBearerInfoStats_p->bearerType = INVALID_VALUE;
    ueRadioBearerInfoStats_p->qci = INVALID_VALUE;
    ueRadioBearerInfoStats_p->rlcMode = INVALID_VALUE;

    ueRadioBearerInfoStatsAtFirstIndex_p->bearerId =  INVALID_VALUE;
    ueRadioBearerInfoStatsAtFirstIndex_p->bearerType = INVALID_VALUE;
    ueRadioBearerInfoStatsAtFirstIndex_p->qci = INVALID_VALUE;
    ueRadioBearerInfoStatsAtFirstIndex_p->rlcMode = INVALID_VALUE;
#endif

    do
    {
        entity_p = ueCtx_p->pdcpEntities[lcId][PDCP_ENTITY_RB_TX];
        if ( PNULL != entity_p )
        {
            /* SPR 5219 changes start */
            localIsEmiFwded = entity_p->isEMIFwded ;
            /* SPR 16770 fix start */
            localIsFwdingEnabled = entity_p->isFwdingEnabled;
            /* SPR 16770 fix end */
            if ( (LTE_TRUE == localIsEmiFwded ) ||
                    /* SPR 16770 fix start */
                    ( LTE_FALSE == localIsFwdingEnabled ) ||
                    /* SPR 16770 fix end */
                    /* SPR 5219 changes end */
                    ( PDCP_TX_STATE_HO != ueCtx_p->pdcpTxSrb2DrbState) || 
                    /* SPR 6758 changes start */
                    ( LTE_TRUE == ueCtx_p->hoTriggerRecvd ) 
                    )
                /* SPR 6758 changes end */
            {
#ifdef PDCP_ASYNC_INTERFACE
                pdcpDeleteSecurityContext( entity_p->pdcpSecurityContext_p );
                /* +- SPR 15236 */
                entity_p->pdcpSecurityContext_p = PNULL;

#endif
                /* SPR 3159 changes start */
                pdcpEntityContextFree(entity_p, ueIndex, lcId + 1, 
                        PDCP_ENTITY_RB_TX);
                /* SPR 3159 changes end */
                /*SPR 51542 Start*/
                semDestroy((LTE_SEM *)&(entity_p->pdcpTxDataReqQ).sem_lock);
                semDestroy((LTE_SEM *)&(entity_p->pdcpStorageQ).sem_lock);
                /*SPR 51542 Start*/
                freeMemPool(entity_p);
                entity_p = PNULL;
                ueCtx_p->pdcpEntities[lcId][PDCP_ENTITY_RB_TX] = PNULL;
                result = PDCP_SUCCESS;
            }
            else
            {
                /* Do not delete the entity as in the process of data 
                   forwarding */
                LOG_PDCP_MSG( PDCP_UE_CANNOT_DELETE, LOGWARNING, PDCP_RRCOAM, 
                        pdcpCurrentTime_g, __LINE__, ueIndex, lcId + 1, 0,0, 
                        0,0, __func__,"");
                result =  PDCP_ERR_UE_DELETE_DATA_FWDING;
                break;
            }
        }

        entity_p = ueCtx_p->pdcpEntities[lcId][PDCP_ENTITY_RB_RX];
        if ( PNULL != entity_p )
        {
            /* SPR 5219 changes start */
            if ( (LTE_TRUE == localIsEmiFwded) ||
                    /* SPR 5219 changes end */
                    /* SPR 16770 fix start */
                    ( LTE_FALSE == localIsFwdingEnabled ) ||
                    /* SPR 16770 fix end */
                    ( PDCP_RX_STATE_HO != ueCtx_p->pdcpRxState ) 
                    /* SPR 2898 chages start */
                    || ( ( PDCP_RX_STATE_HO == ueCtx_p->pdcpRxState ) && 
                        ( PDCP_SUSPENDED == ueCtx_p->event ) )                  
                    /* SPR 2898 changes end */ 
                    /* SPR 6758 changes start */
                    || ( LTE_TRUE == ueCtx_p->hoTriggerRecvd ) 
                    )
                /* SPR 6758 changes end */
            {
#ifdef PDCP_ASYNC_INTERFACE
                pdcpDeleteSecurityContext( entity_p->pdcpSecurityContext_p );
                /* +- SPR 15236 */
                entity_p->pdcpSecurityContext_p = PNULL;

#endif
                pdcpEntityContextFree(entity_p, ueIndex, lcId + 1, 
                        PDCP_ENTITY_RB_RX);
                /* + SPR 19525 */
                DEINIT_PDCP_RX_DATA_IND_Q( entity_p );
                /* - SPR 19525 */
                semDestroy((LTE_SEM *)&(entity_p->pdcpUlReorderingQ).sem_lock);
                freeMemPool(entity_p);
                ueCtx_p->pdcpEntities[lcId][PDCP_ENTITY_RB_RX] = PNULL;
                result = PDCP_SUCCESS;
            }
            else
            {
                /* Do not delete the entity as in the process of data 
                   forwarding */
                LOG_PDCP_MSG( PDCP_UE_CANNOT_DELETE, LOGWARNING, PDCP_RRCOAM, 
                        pdcpCurrentTime_g, __LINE__, ueIndex, lcId + 1, 0,0, 
                        0,0, __func__,"");
                result =  PDCP_ERR_UE_DELETE_DATA_FWDING;
                break;
            }
        }
        /* Updates the list of active DRBs*/
        updateActiveLcInfo(ueIndex, (lcId + 1), 
                pdcpContext.ueContexts[ueIndex]->activeLcInfo,
                pdcpContext.ueContexts[ueIndex]->activeLcIndex 
                );
        pdcpStats.statsUEs[ueIndex].statNumActiveDrbLc -= 1;
        pdcpContext.ueContexts[ueIndex]->numOfActiveDrbLc -= 1;
        /* SPR 3189 changes start */
        pdcpStats.statsUEs[ueIndex].drbs[lcId - 2].lcId = 0;
        /* SPR 3189 changes end */
    }while (0);

    return result;
}

/****************************************************************************
 * Function Name  : pdcpEntityContextCreate
 * Inputs         : type - .
 *                  rlcMode - .
 *                  rbDirection - .
 *                  radioBearer - . 
 * Outputs        : entity_p - pointer on created entity.
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It creates PDCP Entity context for RB which is 
 *                  identified ueIndex, lcId, direction.
 ****************************************************************************/
UInt16 pdcpEntityContextCreate( LP_PdcpEnityContext *entity_p, UInt16 type,
        UInt16 rlcMode, UInt16 rbDirection,
        /* SPR 3276 changes start */
        UInt8 radioBearer )
/* SPR 3276 changes end */
{
    /*Check PDCP entity*/
    if ( PNULL != *entity_p )
    {
        return PDCP_ERR_ENTITY_ALREADY_CREATED;
    }

    /*Check direction*/
    if((PDCP_ENTITY_RB_TX!=rbDirection) && (PDCP_ENTITY_RB_RX!=rbDirection))
    {
        LOG_PDCP_MSG (PDCP_INVALID_DIR, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, rbDirection, 0,0,0, 0,0, __func__,"");
        return PDCP_ERR_ENTITY_WRONG_DIRECTION;
    }

    *entity_p = 
        (LP_PdcpEnityContext)getMemFromPool( sizeof(PdcpEnityContext), PNULL);
    if ( PNULL == *entity_p )
    {
        /*CRITICAL! Memory error*/
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, sizeof(PdcpEnityContext), 0, 0,
                0, 0,0, __func__,"Allocating entity_p (PDCP Context)");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);

        ltePanic("%s:MemAlloc fails for entityContext allocation.",__FUNCTION__);
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    } 

    /*reset entity*/
    memSet(*entity_p, 0, sizeof(PdcpEnityContext));

    /*set header fields*/
    (*entity_p)->type = type;
    (*entity_p)->rlcMode = rlcMode;
    (*entity_p)->rbDirection = rbDirection;
    /* SPR 3276 changes start */
    (*entity_p)->radioBearer = radioBearer;
    /* SPR 3276 changes end */

    /*Setting default snSize*/
    if(PDCP_ENTITY_SRB == type)
    {
        (*entity_p)->snSize = PDCP_TS_SRB_SN_SIZE;
    }
    else
        /*PDCP_ENTITY_DRB==type*/
    {
        (*entity_p)->snSize = PDCP_TS_DRB_SN_SIZE_BIG;
        if ( PDCP_ENTITY_RLC_AM == (*entity_p)->rlcMode )
        { /* 3GPP TS 36.323 V8.5.0 (2009-03): 7.1 e) */
            (*entity_p)->last_Submitted_SN = 4095;
        }
    }

    /*Initializing storage*/
    if(PDCP_ENTITY_RB_TX == rbDirection)
    {
        /* Initialize TX queue for an UE and LC */
        queueInit( &((*entity_p)->pdcpTxDataReqQ) );
        /* Initialize storage TX queue for an UE and LC */
        queueInit( &((*entity_p)->pdcpStorageQ) );
    }
    else /*PDCP_ENTITY_RB_RX==rbDirection*/
    {
        /*Initialize the queues for an UE*/
        queueInit( &((*entity_p)->pdcpRxDataIndQ) );        
        queueInit( &((*entity_p)->pdcpUlReorderingQ) );
    }

    /*SPR_1235_FIX_START*/
    (*entity_p)->isDirectionSetSrb = PDCP_DIRECTION_NOT_SET;
    (*entity_p)->isDirectionSetDrb = PDCP_DIRECTION_NOT_SET;
    /*SPR_1235_FIX_END*/

    (*entity_p)->isEndMarkerIndRecvd = LTE_FALSE;
    /* SPR 3225 changes start */
    (*entity_p)->isEMIFwded = LTE_FALSE;
    /* SPR 3225 changes end */
    /* SPR 3607 Fix Start */
    (*entity_p)->securityOffset = PDCP_SECURITY_OFFSET_DISABLED;
    /* Initialize isPsrRecvd with FALSE at creating context */
    (*entity_p)->isPsrRecvd = LTE_FALSE;
    /* SPR 3607 Fix End */
#ifndef UE_SIM_TESTING
    (*entity_p)->isSpsLc = LTE_FALSE;
    (*entity_p)->numValidSdu = 0;
    (*entity_p)->arrIndex = NUM_RLC_SDU_FOR_RUNNING_AVG - 1;
    (*entity_p)->numPktsAvgSduProvided = 0;
    (*entity_p)->sumSduSize = 0;
    memSet( (*entity_p)->sduArray, 0, NUM_RLC_SDU_FOR_RUNNING_AVG);
#endif
    (*entity_p)->lossyHoRab = LTE_FALSE;
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
       (*entity_p)->isTrafficAllowed = LTE_TRUE;
#endif
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpEntityContextFree
 * Inputs         : LP_PdcpEnityContext* entity_p
 *                  ueIndex
 *                  lcId
 *                  rbDirection
 * Outputs        :
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : To free context memory
 ****************************************************************************/
UInt16 pdcpEntityContextFree( LP_PdcpEnityContext entity_p, 
        UInt16 ueIndex,
        UInt16 lcId,
        /* SPR 3159 changes start */
        UInt16 rbDirection
        /* SPR 3159 changes end */ )
{
    void            *rohc_entity_p = PNULL;
    UInt8           *out_buff_p = PNULL;
    UInt8           *in_buff_p  = PNULL;
    UInt32          eCode   =  PDCP_SUCCESS;
    rohc_api_info_t rohc_api_data;
    LP_PdcpDataReq  txDataReq_p = PNULL;
    LP_PdcpSrbDataReq  txSrbDataReq_p = PNULL;
    LP_PdcpDataInd  rxDataInd_p = PNULL;
#if  defined(PERF_STATS) && !defined(UE_SIM_TESTING)
    rohc_entity_t               *p_rohc_entity = PNULL;
#endif

    if(PNULL == entity_p)
    {
        LOG_PDCP_MSG (PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__,0,0,0,0, 0,0,__func__,"");
        return PDCP_ERR_CONTEXT_NOT_INITIALIZED;
    }

    /* SPR 3159 changes start */
    if ( PDCP_ENTITY_RB_TX == rbDirection )
    {
        /* SPR 3159 changes end */
        /* SPR 15692 fix start */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        if( PDCP_ENTITY_DRB == entity_p->type )
        {
            /* Since the KPIs are only valid for DRBs, hence call them 
             * in case of DRBs only */
            /* Update the KPI stats for RAB which is going to be deleted */
            pdcpUpdateKPIBeforeLcDelete( ueIndex, lcId, entity_p->qci - 1 );
        }
#endif   
        /* SPR 15692 fix end */
        /* Free data request queue for TX*/
        do 
        {
            DEQUEUE_PDCP_TX_DATA_REQ_Q(entity_p, txDataReq_p);
            if ( PNULL != txDataReq_p )
            {
                /* SPR 16766 fix start */
                LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE , LOGWARNING, PDCP_TX, 
                        pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 
                        txDataReq_p->commDataReq.count, 
                        txDataReq_p->tickAssociated,
                        0,0,__func__, 
                        "FreeContext");
                /* SPR 16766 fix end */
                pdcpFreeTxDataReq(txDataReq_p);
            }
        } while ( PNULL != txDataReq_p );

        /* SPR 10111 Fix Start */
        /* Reset the data forwarding variables store into the context */
        entity_p->dataNode = PNULL;
        entity_p->numberOfNodesFwded = 0;
        /* SPR 10111 Fix End */
    

        /* Free data request storage queue for TX*/
        do
        {
            DEQUEUE_PDCP_TX_STORAGE_Q(entity_p, txDataReq_p);
            if ( PNULL != txDataReq_p )
            {
                /* SPR 16766 fix start */
                LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE , LOGWARNING, PDCP_TX, 
                        pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 
                        txDataReq_p->commDataReq.count, 
                        txDataReq_p->tickAssociated
                        , 0, 0, __func__,
                        "FreeContext");
                /* SPR 16766 fix end */
                pdcpFreeTxDataReq(txDataReq_p);
            }
        } while ( PNULL != txDataReq_p );

        /* Free SRB data request queue for TX*/
        do 
        {
            DEQUEUE_PDCP_SRB_TX_DATA_REQ_Q(entity_p, txSrbDataReq_p);
            if ( PNULL != txSrbDataReq_p )
            {
                /* SPR 16766 fix start */
                LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE , LOGWARNING, PDCP_TX, 
                        pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 
                        txSrbDataReq_p->commDataReq.count, 
                        txSrbDataReq_p->tickAssociated
                        , 0, 0, __func__, 
                        "FreeContext");
                /* SPR 16766 fix end */
                pdcpFreeSrbTxDataReq(txSrbDataReq_p);
            }
        } while ( PNULL != txSrbDataReq_p );

        /* Free SRB data request storage queue for TX*/
        do
        {
            DEQUEUE_PDCP_SRB_TX_STORAGE_Q(entity_p, txSrbDataReq_p);
            if ( PNULL != txSrbDataReq_p )
            {
                /* SPR 16766 fix start */
                LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE , LOGWARNING, PDCP_TX, 
                        pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 
                        txSrbDataReq_p->commDataReq.count, 
                        txSrbDataReq_p->tickAssociated
                        , 0, 0, __func__, 
                        "FreeContext");
                /* SPR 16766 fix end */
                pdcpFreeSrbTxDataReq(txSrbDataReq_p);
            }
        } while ( PNULL != txSrbDataReq_p );
        /* SPR 3159 changes start */
    }
    else 
    {
        /* SPR 3159 changes end */
        /* Free data indication queue for RX*/
        do 
        {
            DEQUEUE_PDCP_RX_DATA_IND_Q(entity_p, rxDataInd_p);
            if (PNULL != rxDataInd_p)
            {
                pdcpFreeRxDataInd(rxDataInd_p);
            }
        } while ( PNULL != rxDataInd_p );

        /* Free data indication storage for RX*/
        do 
        {
            DEQUEUE_PDCP_RX_STORAGE_Q(entity_p, rxDataInd_p);
            if (PNULL != rxDataInd_p)
            {
                pdcpFreeRxDataInd(rxDataInd_p);
            }
        } while ( PNULL != rxDataInd_p );
    }

    if(PNULL != entity_p->rohc.context)
    {
        rohc_api_data.api_id = ROHC_RELEASE_REQ;
        rohc_api_data.entity_id.ueIndex = ueIndex;
        rohc_api_data.entity_id.lcId = lcId;
        rohc_entity_p = entity_p->rohc.context;
#if  defined(PERF_STATS) && !defined(UE_SIM_TESTING)
        p_rohc_entity = (rohc_entity_t*)(rohc_entity_p);
        p_rohc_entity->rohc_entity_id.qci = 0;
#endif
        if (RO_SUCCESS != rfc3095_process_msg(rohc_entity_p,
                    in_buff_p, &out_buff_p , &rohc_api_data, &eCode))
        {
            return PDCP_FAIL;
        }

        entity_p->rohc.context = PNULL;
    }

    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpGetEntityContext
 * Inputs         : ueIndex, lcId, rbDirection
 * Outputs        : 
 * Returns        : LP_PdcpEnityContext, PNULL if does not exist.
 * Description    : returns context entity for given ueIndex, lcId, rbDirection.
 ****************************************************************************/
LP_PdcpEnityContext pdcpGetEntityContext( UInt16 ueIndex, UInt16 lcId, 
        UInt16 rbDirection )
{
    /* Check ueIndex */
    if(ueIndex >= MAX_UE_SUPPORTED) {
        LOG_PDCP_MSG (PDCP_INVALID_UE, LOGWARNING, (PDCP_RRCOAM|PDCP_TX|PDCP_RX),
                pdcpCurrentTime_g, __LINE__, ueIndex, 
                pdcpContext.isInitialized, 0,0, 0,0, __func__,"");
        return PNULL;
    }

    /*Check UE context*/
    if ( PNULL == pdcpContext.ueContexts[ueIndex]) 
    {
        LOG_PDCP_MSG ( PDCP_INVALID_UE_CONTEXT, LOGWARNING,
                (PDCP_RRCOAM|PDCP_TX|PDCP_RX), pdcpCurrentTime_g, 
                __LINE__, ueIndex, pdcpContext.isInitialized, 0,0, 
                0,0, __func__,"");
        return PNULL;
    }

    /* input range 1..LTE_MAX_LC_ID - storage range 0..LTE_MAX_LC_ID-1 */
    lcId--; 

    /* Check lcId */
    if ( LTE_MAX_LC_ID <= lcId ) {
        LOG_PDCP_MSG (PDCP_INVALID_LC, LOGWARNING, (PDCP_RRCOAM|PDCP_TX|PDCP_RX),
                pdcpCurrentTime_g, __LINE__, lcId, ueIndex, 0,0, 
                0,0, __func__,"");
        return PNULL;
    }

    /*Check direction*/
    if((PDCP_ENTITY_RB_TX!=rbDirection) && (PDCP_ENTITY_RB_RX!=rbDirection)) {
        LOG_PDCP_MSG (PDCP_INVALID_DIR, LOGWARNING, (PDCP_RRCOAM|PDCP_TX|PDCP_RX),
                pdcpCurrentTime_g, __LINE__, rbDirection, ueIndex, lcId, 0,
                0,0, __func__,"");
        return PNULL;
    }
    return pdcpContext.ueContexts[ueIndex]->pdcpEntities[lcId][rbDirection];
}

/****************************************************************************
 * Function Name  : pdcpEntityConfigureIntegrity
 * Inputs         : ueIndex - . 
 *                  lcId - .
 *                  algorithmId - .
 *                  key - .
 *                  securityOffset - .
 * Outputs        : 
 * Returns        : void
 * Description    : It configures Integrity protection parameters for 
 *                  Entities in any direction.
 ****************************************************************************/
void pdcpEntityConfigureIntegrity( UInt16 ueIndex, UInt16 lcId,
        /*SPR 10446 fix start */
        UInt16 algorithmId, UInt8* key, UInt8 securityOffset )
        /*SPR 10446 fix end */
{
    /* SPR_1235_FIX_START*/
    LP_PdcpEnityContext  entity_p = PNULL;

    do {
        /* get entity context for TX direction */
        entity_p = pdcpGetEntityContext( ueIndex, lcId, PDCP_ENTITY_RB_TX );
        if ( PNULL == entity_p )
        {
            break;
        }
        /*SPR 10446 fix start */
        if( PDCP_SECURITY_OFFSET_ENABLED == securityOffset )
        {
            /* Called at the time of configuration, hence use the value
             * as stored in context */
            securityOffset = entity_p->securityOffset;
        }
        /* SPR 3607 Fix Start */
        if( PDCP_SECURITY_OFFSET_ENABLED != securityOffset)
        {
            /*SPR 10446 fix end */
            if ( PDCP_DIRECTION_SET == entity_p->isDirectionSetSrb )
            {
                /* configure TX */
                entity_p->integrity.algorithmId = algorithmId;
                memCpy( entity_p->integrity.key, key, LTE_SECURITY_KEY_LENGTH );
#ifdef PDCP_ASYNC_INTERFACE
                entity_p->pdcpSecurityContext_p->integrityAlgoId = 
                    (UInt8)algorithmId;
                /* SPR 15236 fix start */
                memCpy( entity_p->pdcpSecurityContext_p->integrityKey_p,
                    key, LTE_SECURITY_KEY_LENGTH );
                /* SPR 15236 fix end */
                pdcpCreateSecurityContext( (UInt8 *)(&pdcpSecInputTxQ_g), 
                        entity_p->pdcpSecurityContext_p );
#endif
            }
            /* SPR 2854 changes start*/
            entity_p->isDirectionSetSrb = PDCP_DIRECTION_SET;
            /* SPR 2854 changes end*/
        }
        else
        {
            /* configure TX */
            entity_p->new_integrity.algorithmId = algorithmId;
            memCpy( entity_p->new_integrity.key, key, LTE_SECURITY_KEY_LENGTH );
        }
        /* SPR 3607 Fix End */

    } while (0);
    entity_p = PNULL;

    do {
        /* get entity context for RX direction */
        entity_p = pdcpGetEntityContext( ueIndex, lcId, PDCP_ENTITY_RB_RX );
        if ( PNULL == entity_p )
        {
            break;
        }
        if ( PDCP_DIRECTION_SET == entity_p->isDirectionSetSrb )
        {
            /* configure TX */
#ifndef UE_SIM_TESTING
            /* SPR 8174 Changes Start */
            /*COVERITY 69283 START*/
            if(ALGO_EIA0 == entity_p->integrity.algorithmId)
            /*COVERITY 69283 END*/
            {
                entity_p->reconfigUeFlag = LTE_TRUE;
            } 
            /* SPR 8174 Changes End */
#endif
            entity_p->integrity.algorithmId = algorithmId;
            memCpy( entity_p->integrity.key, key, LTE_SECURITY_KEY_LENGTH );
#ifdef PDCP_ASYNC_INTERFACE 

            entity_p->pdcpSecurityContext_p->integrityAlgoId = 
                (UInt8)algorithmId;
            /* SPR 15236 fix start */
            memCpy( entity_p->pdcpSecurityContext_p->integrityKey_p,
                key, LTE_SECURITY_KEY_LENGTH );
            /* SPR 15236 fix end */

#ifndef UE_SIM_TESTING
            /* SPR 8174 Changes Start */
            if(ALGO_EIA0 == entity_p->pdcpSecurityContext_p->integrityAlgoId)
            {
                entity_p->pdcpSecurityContext_p->reconfigUeFlag = LTE_TRUE;
            } 
            /* SPR 8174 Changes End */
#endif
            pdcpCreateSecurityContext( (UInt8 *)(&pdcpSecInputRxQ_g), 
                    entity_p->pdcpSecurityContext_p );
#endif
        }
        /* SPR 2854 changes start*/
        entity_p->isDirectionSetSrb = PDCP_DIRECTION_SET;
        /* SPR 2854 changes end*/
    } while (0);
    /* SPR_1235_FIX_END*/
}

/****************************************************************************
 * Function Name  : pdcpEntitySetDiscardTimer
 * Inputs         : ueIndex - . 
 *                  lcId - .
 *                  rbDirection - .
 *                  discardTimer - .
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures DiscardTimer feature for 1 direction Entity.
 ****************************************************************************/
UInt16 pdcpEntitySetDiscardTimer( UInt16 ueIndex, UInt16 lcId, 
        UInt16 rbDirection, UInt16 discardTimer )
{
    LP_PdcpEnityContext  entity_p = PNULL;

    if ( PDCP_ENTITY_RB_BOTH == rbDirection )
    {
        LOG_PDCP_MSG (PDCP_INVALID_DIR, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, rbDirection, ueIndex, lcId,0, 0,0, __func__,"");
        return PDCP_ERR_ENTITY_WRONG_DIRECTION;
    }

    /* get entity context */
    entity_p = pdcpGetEntityContext( ueIndex, lcId, rbDirection );
    if ( PNULL == entity_p )
    {
        LOG_PDCP_MSG( PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, 0,0,0,0,0,0,__func__,"");
        return PDCP_ERR_ENTITY_NOT_FOUND;
    }

    entity_p->discardTimer = discardTimer;

    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpEntityConfigureDiscardTimer
 * Inputs         : ueIndex - . 
 *                  lcId - .
 *                  rbDirection - .
 *                  discardTimer - .
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures DiscardTimer feature for Entities in 
 *                  any direction.
 ****************************************************************************/
UInt16 pdcpEntityConfigureDiscardTimer( UInt16 ueIndex, UInt16 lcId, 
        UInt16 rbDirection, UInt16 discardTimer )
{
    UInt16  result = PDCP_SUCCESS;

    if ( PDCP_ENTITY_RB_BOTH == rbDirection )
    {
        /* dirrection BOTH - configure TX */
        result = pdcpEntitySetDiscardTimer( ueIndex, lcId, 
                PDCP_ENTITY_RB_TX, discardTimer );

        /* set dirrection to RX */
        rbDirection = PDCP_ENTITY_RB_RX;        
    }

    if ( PDCP_SUCCESS == result )
    {
        /* dirrection is one or RX (for BOTH) - configure it */
        result = pdcpEntitySetDiscardTimer( ueIndex, lcId, rbDirection, 
                discardTimer );
    }

    return result;
}

/****************************************************************************
 * Function Name  : pdcpEntityConfigureSnSize
 * Inputs         : ueIndex - . 
 *                  lcId - .
 *                  rbDirection - .
 *                  snSize - .
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It set snSize value for one direction Entity.
 ****************************************************************************/
UInt16 pdcpEntitySetSnSize( UInt16 ueIndex, UInt16 lcId, UInt16 rbDirection,
        UInt16 snSize )
{
    UInt16 result = PDCP_SUCCESS;
    LP_PdcpEnityContext  entity_p = PNULL;
  

    if ( PDCP_ENTITY_RB_BOTH == rbDirection )
    {
        LOG_PDCP_MSG (PDCP_INVALID_DIR, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, rbDirection, ueIndex, lcId, 0,
                0,0, __func__,"");
        return PDCP_ERR_ENTITY_WRONG_DIRECTION;
    }

    /* get entity context */
    entity_p = pdcpGetEntityContext( ueIndex, lcId, rbDirection );
    if ( PNULL == entity_p )
    {
        LOG_PDCP_MSG( PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, 0,0,0,0,0,0,__func__,"");
        result = PDCP_ERR_ENTITY_NOT_FOUND;
    } else {
        /* FIX for __SPR_638__ START */    
        /* It's supported only for DRB which is mapped on RLC UM/AM */
        if ( ( PDCP_ENTITY_DRB == entity_p->type ) && 
                ( PDCP_ENTITY_RLC_UM == entity_p->rlcMode || 
                  ( PDCP_ENTITY_RLC_AM == entity_p->rlcMode && 
                    /*SPR 3492 changes start */
                    PDCP_TS_DRB_SN_SIZE_BIG	== snSize )
                  /*SPR 3492 changes ends*/ ) )
            /* FIX for __SPR_638__ END */    
        {
            entity_p->snSize = snSize;
#ifdef PDCP_ASYNC_INTERFACE

            entity_p->pdcpSecurityContext_p->snSize = (UInt8)snSize;
            /*SPR 3490 changes start */

            if ( PDCP_ENTITY_RB_TX== rbDirection )
            {	    
                pdcpCreateSecurityContext( (UInt8 *)(&pdcpSecInputTxQ_g), 
                        entity_p->pdcpSecurityContext_p );

            }
            else
            {
                pdcpCreateSecurityContext( (UInt8 *)(&pdcpSecInputRxQ_g), 
                        entity_p->pdcpSecurityContext_p );       
            }
            /* SPR 3490 changes ends */
#endif
        }
        else
        {
            result = PDCP_ERR_ENTITY_SET_SN_SIZE;
        }
    }
    return result;
}

/****************************************************************************
 * Function Name  : pdcpEntityConfigureSnSize
 * Inputs         : ueIndex - . 
 *                  lcId - .
 *                  rbDirection - .
 *                  snSize - .
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures snSize value for Entities in any direction.
 ****************************************************************************/
UInt16 pdcpEntityConfigureSnSize( UInt16 ueIndex, UInt16 lcId, 
        UInt16 rbDirection, UInt16 snSize )
{
    UInt16 result = PDCP_SUCCESS;

    if ( PDCP_ENTITY_RB_BOTH == rbDirection )
    {
        /* dirrection BOTH - configure TX */
        result = pdcpEntitySetSnSize(ueIndex, lcId, PDCP_ENTITY_RB_TX, snSize);

        /* set dirrection to RX */
        rbDirection = PDCP_ENTITY_RB_RX;        
    }

    if ( PDCP_SUCCESS == result )
    {
        /* dirrection is one or RX (for BOTH) - configure it */
        result = pdcpEntitySetSnSize( ueIndex, lcId, rbDirection, snSize );
    }

    return result;
}

#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : pdcpEntitySetQci
 * Inputs         : ueIndex - .
 *                  lcId - .
 *                  rbDirection - .
 *                  qci - .
 * Outputs        :
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It set snSize value for one direction Entity.
 ****************************************************************************/
UInt16 pdcpEntitySetQci( UInt16 ueIndex, UInt16 lcId, UInt16 rbDirection,
        UInt8 qci )
{
    UInt16               result   = PDCP_SUCCESS;
    LP_PdcpEnityContext  entity_p = PNULL;
    LP_PdcpUeContext     ueCtx_p  = PNULL;
    InternalCellIndex      cellIndex = INVALID_CELL_INDEX;

#ifdef LOG_PRINT_ENABLED 
    UInt32 state = 0;
#endif

    do
    {
        if ( PDCP_ENTITY_RB_BOTH == rbDirection )
        {
            LOG_PDCP_MSG (PDCP_INVALID_DIR, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, rbDirection, ueIndex, lcId, 0,
                    0,0, __func__,"");
            result =  PDCP_ERR_ENTITY_WRONG_DIRECTION;
            break;
        }

        /* get entity context */
        ueCtx_p = PDCP_GET_UE_CONTEXT( ueIndex );
        if( PNULL == ueCtx_p )
        {
            /* Invalid UE Context */
            LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, ueIndex,
                    pdcpContext.isInitialized, 0,0, 0,0, __func__,"");
            result = PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
            break;
        }

        entity_p = pdcpGetEntityContext( ueIndex, lcId, rbDirection);
        if ( PNULL == entity_p )
        {
            LOG_PDCP_MSG( PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, 0,0,0,0,0,0,__func__,"");
            result = PDCP_ERR_ENTITY_NOT_FOUND;
            break;
        }

        /* Return Failure for SRB */
        if ( PDCP_ENTITY_SRB == entity_p->type )
        {            
            result = PDCP_FAIL;
            break;
        }

        /* Sets QCI for DRB */
        entity_p->qci = qci;
        cellIndex = ueCtx_p->cellIndex ;

        /* Sets if its SPS LC if SPS is enabled by OAM and QCI to SPS 
         * mapping list has enabled value for SPS for teh QCI */
        if( LTE_TRUE == pdcpContext.sCellInfo[cellIndex].isSpsEnable )
        {
            if ( PDCP_ENTITY_RB_TX == rbDirection )
            {
                if ( (LTE_TRUE == ueCtx_p->dlSpsReqType) && 
                     (LTE_TRUE == pdcpContext.sCellInfo[cellIndex].
                      qciSpslist[qci - 1] ) )
                {
                    entity_p->isSpsLc = LTE_TRUE;
                }
                else
                {
                    entity_p->isSpsLc = LTE_FALSE;
                }
            }
            else
            {
                if ( (LTE_TRUE == ueCtx_p->ulSpsReqType)  &&
                     (LTE_TRUE == pdcpContext.sCellInfo[cellIndex].
                                qciSpslist[qci - 1] ) )
                {
                    entity_p->isSpsLc = LTE_TRUE;

#ifdef LOG_PRINT_ENABLED 
                    state = ueCtx_p->pdcpRxSpsState;
#endif
                    /* Set the RX state for SPS */
                    pdcpSpsRxStateMachine[ueCtx_p->pdcpRxSpsState]
                        [PDCP_SPS_RRC_CONFIG][PDCP_NO_PACKET ](
                                ueIndex, lcId, ueCtx_p->pdcpRxSpsState,
                                PDCP_SPS_RRC_CONFIG, ueCtx_p, PNULL, 
                                cellIndex);
                    LOG_PDCP_MSG( PDCP_UE_SPS_STATE, LOGINFO, PDCP_RX, pdcpCurrentTime_g,
                             __LINE__, ueIndex, state, PDCP_SPS_RRC_CONFIG, ueCtx_p->pdcpRxSpsState,
                             lcId,0, __func__, "PdcpSpsRxStateMachine");
                }
                else
                {
                    entity_p->isSpsLc = LTE_FALSE;
                }
            }
        }
    } while( 0 );

    return result;
}

/****************************************************************************
 * Function Name  : pdcpEntityQci
 * Inputs         : ueIndex - .
 *                  lcId - .
 *                  rbDirection - .
 *                  qci - .
 * Outputs        :
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures snSize value for Entities in any direction.
 ****************************************************************************/
UInt16 pdcpEntityQci( UInt16 ueIndex, UInt16 lcId,
        UInt16 rbDirection, UInt8 qci )
{
    UInt16 result = PDCP_SUCCESS;

    if ( PDCP_ENTITY_RB_BOTH == rbDirection )
    {
        /* dirrection BOTH - configure TX */
        result = pdcpEntitySetQci(ueIndex, lcId, PDCP_ENTITY_RB_TX, qci);

        /* set dirrection to RX */
        rbDirection = PDCP_ENTITY_RB_RX;
    }

    if ( PDCP_SUCCESS == result )
    {
        /* dirrection is one or RX (for BOTH) - configure it */
        result = pdcpEntitySetQci( ueIndex, lcId, rbDirection, qci );
    }

    return result;
}

/****************************************************************************
 * Function Name  : pdcpEntitySetsSpsLc
 * Inputs         : ueIndex - UE ID
 *                  ueCtx_p - Pointer to UE context.
 *                  rbDirection - Direction.
 * Outputs        :
 * Returns        : None
 * Description    : It sets or resets isSpsLc.
 ****************************************************************************/
void pdcpEntitySetsSpsLc( UInt16 ueIndex, LP_PdcpUeContext ueCtx_p,
        UInt16 rbDirection )
{
    UInt8 lcId  = 0;
    LP_PdcpEnityContext ctx_p = PNULL;
    InternalCellIndex cellIndex = INVALID_CELL_INDEX;
#ifdef LOG_PRINT_ENABLED
    UInt32 state = 0;
#endif

    cellIndex = ueCtx_p->cellIndex ;

    /* Return if SPS is not enabled by OAM */
    if( LTE_FALSE == pdcpContext.sCellInfo[cellIndex].isSpsEnable )
    {
        return;
    }

    while ( lcId < ueCtx_p->numOfActiveDrbLc)
    {
        /*
         ** activeLcInfo consists only of active DRBs in an UE, hence
         ** valid PDCP context will always be available. Therefore, NULL
         ** check not required
         */
        ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p,
                   ueCtx_p->activeLcInfo[lcId],
                   rbDirection);
        /* Sets if its SPS LC if SPS is enabled by OAM and QCI to SPS 
         * mapping list has enabled value for SPS for teh QCI */
        if( PDCP_ENTITY_RB_TX == rbDirection )
        {
            if ( (LTE_TRUE == ueCtx_p->dlSpsReqType) && 
                    (LTE_TRUE == pdcpContext.sCellInfo[cellIndex].
                     qciSpslist[ctx_p->qci - 1] ) )
            {
                ctx_p->isSpsLc = LTE_TRUE;
            }
            else
            {
                ctx_p->isSpsLc = LTE_FALSE;
            }
        }
        else
        {
            if ( LTE_TRUE == pdcpContext.sCellInfo[cellIndex].
                    qciSpslist[ctx_p->qci - 1] )
            {
                if( LTE_TRUE == ueCtx_p->ulSpsReqType) 
                {
                    ctx_p->isSpsLc = LTE_TRUE;
                }
                else
                {
                    ctx_p->isSpsLc = LTE_FALSE;
                }
#ifdef LOG_PRINT_ENABLED
                state = ueCtx_p->pdcpRxSpsState;
#endif
                /* Set the RX state for SPS */
                pdcpSpsRxStateMachine[ueCtx_p->pdcpRxSpsState]
                    [PDCP_SPS_RRC_CONFIG][PDCP_NO_PACKET ](
                            ueIndex, ueCtx_p->activeLcInfo[lcId], 
                            ueCtx_p->pdcpRxSpsState,
                            PDCP_SPS_RRC_CONFIG, ueCtx_p, PNULL, cellIndex );

                LOG_PDCP_MSG( PDCP_UE_SPS_STATE, LOGINFO, PDCP_RX, pdcpCurrentTime_g,
                        __LINE__, ueIndex, state, PDCP_SPS_RRC_CONFIG, ueCtx_p->pdcpRxSpsState,
                        ueCtx_p->activeLcInfo[lcId], 0, __func__, "PdcpSpsRxStateMachine");
            }
        }
        lcId++;
    }
}
#endif

/****************************************************************************
 * Function Name  : pdcpEntitySetStatusReportRequired
 * Inputs         : ueIndex - . 
 *                  lcId - .
 *                  rbDirection - .
 *                  StatusReportRequired - .
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures statusReportRequired value for one 
 *                  direction Entity.
 ****************************************************************************/
UInt16 pdcpEntitySetStatusReportRequired( UInt16 ueIndex, UInt16 lcId, 
        UInt16 rbDirection, UInt8 StatusReportRequired )
{
    UInt16 result = PDCP_SUCCESS;
    LP_PdcpEnityContext  entity_p = PNULL;

    /*SPR 4865 Fix*/

    if ( PDCP_ENTITY_RB_BOTH == rbDirection )
    {
        LOG_PDCP_MSG (PDCP_INVALID_DIR, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, rbDirection, ueIndex, lcId, 0,
                0,0, __func__,"");
        return PDCP_ERR_ENTITY_WRONG_DIRECTION;
    }

    /* get entity context */
    entity_p = pdcpGetEntityContext( ueIndex, lcId, rbDirection );
    if ( PNULL == entity_p )
    {
        LOG_PDCP_MSG( PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, 0,0,0,0,0,0,__func__,"");
        result = PDCP_ERR_ENTITY_NOT_FOUND;
    }
    else
    {
        /* It's supported only for DRB which is mapped on RLC AM */
        if ( ( PDCP_ENTITY_DRB == entity_p->type ) && 
                ( PDCP_ENTITY_RLC_AM == entity_p->rlcMode ) )
        {
            entity_p->statusReportRequired = StatusReportRequired;
        }
        else
        {
            result = PDCP_ERR_ENTITY_SET_STATUS_REPORT_REQUIRED;
        }
    }

    return result;
}

/****************************************************************************
 * Function Name  : pdcpEntityConfigureStatusReportRequired
 * Inputs         : ueIndex - . 
 *                  lcId - .
 *                  rbDirection - .
 *                  StatusReportRequired - .
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures statusReportRequired value for Entities 
 *                  in any direction.
 ****************************************************************************/
UInt16 pdcpEntityConfigureStatusReportRequired( UInt16 ueIndex, UInt16 lcId, 
        UInt16 rbDirection, UInt8 StatusReportRequired )
{
    UInt16 result = PDCP_SUCCESS;

    if ( PDCP_ENTITY_RB_BOTH == rbDirection )
    {
        /* dirrection BOTH - configure TX */
        result = pdcpEntitySetStatusReportRequired( ueIndex, lcId, 
                PDCP_ENTITY_RB_TX, StatusReportRequired );

        /* set dirrection to RX */
        rbDirection = PDCP_ENTITY_RB_RX;        
    }

    if ( PDCP_SUCCESS == result )
    {
        /* dirrection is one or RX (for BOTH) - configure it */
        result = pdcpEntitySetStatusReportRequired( ueIndex, lcId,
                rbDirection, StatusReportRequired );
    }

    return result;
}

/* SPR 3608 changes start */
/****************************************************************************
 * Function Name  : pdcpEntityConfigureUeStatusReportReq
 * Inputs         : ueIndex - .
 *                  lcId - .
 *                  rbDirection - .
 *                  StatusReportRequired - .
 * Outputs        :
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures UestatusReportRequired value for UE
 *                  in any direction.
 ****************************************************************************/
UInt16 pdcpEntityConfigureUeStatusReportReq( UInt16 ueIndex, UInt8 lcId,
        UInt16 rbDirection, UInt8 StatusReportRequired )
{
    UInt16 result = PDCP_SUCCESS;

    if ( PDCP_ENTITY_RB_BOTH == rbDirection )
    {
        /* dirrection BOTH - configure TX */
        result = pdcpEntitySetUeStatusReportRequired( ueIndex, lcId, 
                PDCP_ENTITY_RB_TX, StatusReportRequired );

        /* set dirrection to RX */
        rbDirection = PDCP_ENTITY_RB_RX;        
    }

    if ( PDCP_SUCCESS == result )
    {
        /* dirrection is one or RX (for BOTH) - configure it */
        result = pdcpEntitySetUeStatusReportRequired( ueIndex, lcId,
                rbDirection, StatusReportRequired );
    }

    return result;
}

/****************************************************************************
 * Function Name  : pdcpEntitySetUeStatusReportRequired
 * Inputs         : ueIndex - .
 *                  lcId - .
 *                  rbDirection - .
 *                  StatusReportRequired - .
 * Outputs        :
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures UestatusReportRequired value for UE
 *                  in any direction.
 ****************************************************************************/
UInt16 pdcpEntitySetUeStatusReportRequired( UInt16 ueIndex, UInt8 lcId,
        UInt16 rbDirection, UInt8 StatusReportRequired )
{
    UInt16  result = PDCP_SUCCESS;
    LP_PdcpEnityContext ctx_p = PNULL;

    /* Fetch LC Context */
    ctx_p = pdcpGetEntityContext( ueIndex, lcId, rbDirection );
    if ( PNULL == ctx_p)
    {
        LOG_PDCP_MSG( PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, ueIndex, pdcpContext.isInitialized,
                0,0, 0,0, __func__,"");
        return PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
    }

    if ( ( PDCP_ENTITY_DRB == ctx_p->type ) && 
            ( PDCP_ENTITY_RLC_AM == ctx_p->rlcMode ) )
    {
        ctx_p->ueStatusReportReq = StatusReportRequired;
    }
    else
    {
        result = PDCP_ERR_ENTITY_SET_STATUS_REPORT_REQUIRED;
    }

    return result;
}
/* SPR 3608 changes end */

/* SPR 3607 Fix start */
/****************************************************************************
 * Function Name  : pdcpEntityConfigureSecurityOffset
 * Inputs         : ueIndex - .
 *                  lcId - .
 * Outputs        :
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures Security Offset value for UE
 ****************************************************************************/
UInt16 pdcpEntityConfigureSecurityOffset( UInt16 ueIndex, UInt16 lcId)
{
    UInt16 result = PDCP_SUCCESS;
    LP_PdcpEnityContext ctx_p = PNULL;

    /* get ue context */
    ctx_p = pdcpGetEntityContext( ueIndex, lcId, PDCP_ENTITY_RB_TX );

    if ( PNULL == ctx_p)
    {
        LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, ueIndex, pdcpContext.isInitialized,
                0,0, 0,0, __func__,"");
        return PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
    }
    else
    {
        ctx_p->securityOffset = PDCP_SECURITY_OFFSET_ENABLED;
    }
    return result;
}

/* SPR 3607 Fix end */

/****************************************************************************
 * Function Name  : pdcpEntityConfigureSrbCiphering
 * Inputs         : ueIndex - . 
 *                  lcId - .
 *                  algorithmId - .
 *                  key - .
 *                  securityOffset - .
 * Outputs        : 
 * Returns        : void
 * Description    : It configures Ciphering parameters for Entities in 
 *                  any direction.
 ****************************************************************************/
void pdcpEntityConfigureSrbCiphering( UInt16 ueIndex, UInt16 lcId, 
        /*SPR 10446 fix start */
        UInt16 algorithmId, UInt8* key, UInt8 securityOffset )
        /*SPR 10446 fix end */
{
    /* SPR_1235_FIX_START*/
    LP_PdcpEnityContext  entity_p = PNULL;

    do {
        /* get entity context for TX direction */
        entity_p = pdcpGetEntityContext( ueIndex, lcId, PDCP_ENTITY_RB_TX );
        if ( PNULL == entity_p )
        {
            break;
        }
        /*SPR 10446 fix start */
        if( PDCP_SECURITY_OFFSET_ENABLED == securityOffset )
        {
            /* Called at the time of configuration, hence use the value
             * as stored in context */
            securityOffset = entity_p->securityOffset;
        }
        /* SPR 3607 Fix Start */
        if( PDCP_SECURITY_OFFSET_ENABLED != securityOffset)
        {
            /*SPR 10446 fix end */
            if ( PDCP_DIRECTION_SET == entity_p->isDirectionSetSrb )
            {
                /* configure TX */
                entity_p->ciphering.algorithmId = algorithmId;
                memCpy( entity_p->ciphering.key, key, LTE_SECURITY_KEY_LENGTH );
                /* SPR 2739 changes start */
                entity_p->ciphering.newAlgorithmId = algorithmId;
                memCpy( entity_p->ciphering.newKey, key, LTE_SECURITY_KEY_LENGTH );
                /* SPR 2739 changes end */
#ifdef PDCP_ASYNC_INTERFACE
                entity_p->pdcpSecurityContext_p->oldCiphAlgoId = 
                    (UInt8)algorithmId;            
                entity_p->pdcpSecurityContext_p->newCiphAlgoId = 
                    (UInt8)algorithmId;
                /* SPR 15236 fix start */
                memCpy( entity_p->pdcpSecurityContext_p->oldCiphKey_p,
                    key, LTE_SECURITY_KEY_LENGTH );
                memCpy( entity_p->pdcpSecurityContext_p->newCiphKey_p, 
                    key, LTE_SECURITY_KEY_LENGTH );
                /* SPR 15236 fix end */
                pdcpCreateSecurityContext( (UInt8 *)(&pdcpSecInputTxQ_g), 
                        entity_p->pdcpSecurityContext_p );

#endif
            }
            /* SPR 2854 changes start*/
            entity_p->isDirectionSetSrb = PDCP_DIRECTION_SET;
            /* SPR 2854 changes end*/
            /*SPR 10446 fix start */
            /* Reset securtity offset parameter */
            entity_p->securityOffset = PDCP_SECURITY_OFFSET_DISABLED;
            /*SPR 10446 fix end */
        }
        else
        {
            /* configure TX */
            entity_p->new_ciphering.algorithmId = algorithmId;
            memCpy( entity_p->new_ciphering.key, key, LTE_SECURITY_KEY_LENGTH );
        }
        /* SPR 3607 Fix End */
    } while (0);
    entity_p = PNULL;

    do {
        /* get entity context for RX direction */
        entity_p = pdcpGetEntityContext( ueIndex, lcId, PDCP_ENTITY_RB_RX );
        if ( PNULL == entity_p )
        {
            break;
        }
        if ( PDCP_DIRECTION_SET == entity_p->isDirectionSetSrb )
        {
            /* configure RX */
            entity_p->ciphering.algorithmId = algorithmId;
            memCpy( entity_p->ciphering.key, key, LTE_SECURITY_KEY_LENGTH );
            /* SPR 2739 changes start */
            entity_p->ciphering.newAlgorithmId = algorithmId;
            memCpy( entity_p->ciphering.newKey, key, LTE_SECURITY_KEY_LENGTH );
            /* SPR 2739 changes end */
#ifdef PDCP_ASYNC_INTERFACE
            entity_p->pdcpSecurityContext_p->oldCiphAlgoId = 
                (UInt8)algorithmId;            
            entity_p->pdcpSecurityContext_p->newCiphAlgoId = 
                (UInt8)algorithmId;
            /* SPR 15236 fix start */
            memCpy( entity_p->pdcpSecurityContext_p->oldCiphKey_p,
                key, LTE_SECURITY_KEY_LENGTH );
            memCpy( entity_p->pdcpSecurityContext_p->newCiphKey_p,
                key, LTE_SECURITY_KEY_LENGTH );
            /* SPR 15236 fix end */
            pdcpCreateSecurityContext( (UInt8 *)(&pdcpSecInputRxQ_g),
                    entity_p->pdcpSecurityContext_p );
#endif
        }
        /* SPR 2854 changes start*/
        entity_p->isDirectionSetSrb = PDCP_DIRECTION_SET;
        /* SPR 2854 changes end*/
    } while (0);
    /* SPR_1235_FIX_END*/
}

/* SPR_1235_FIX_START*/
/****************************************************************************
 * Function Name  : pdcpEntityConfigureDrbCiphering
 * Inputs         : ueIndex - . 
 *                  lcId - .
 *                  algorithmId - .
 *                  key - .
 * Outputs        : None
 * Returns        : void
 * Description    : It configures Ciphering parameters for Entities in 
 *                  any direction.
 ****************************************************************************/
void pdcpEntityConfigureDrbCiphering( UInt16 ueIndex, UInt16 lcId, 
        UInt16 algorithmId, UInt8* key )
{
    LP_PdcpEnityContext  entity_p = PNULL;
    /* SPR 2739 changes start */
    LP_PdcpUeContext ueCtx_p   = PNULL;    
    /* SPR 2739 changes end */

    do {
        /* get entity context for TX direction */
        entity_p = pdcpGetEntityContext( ueIndex, lcId, PDCP_ENTITY_RB_TX );
        if ( PNULL == entity_p )
        {
            break;
        }
        if ( PDCP_DIRECTION_SET == entity_p->isDirectionSetDrb )
        {
            /* configure TX */
            entity_p->ciphering.algorithmId = algorithmId;
            memCpy( entity_p->ciphering.key, key, LTE_SECURITY_KEY_LENGTH );
            /* SPR 2739 changes start */
            entity_p->ciphering.newAlgorithmId = algorithmId;
            memCpy( entity_p->ciphering.newKey, key, LTE_SECURITY_KEY_LENGTH );
            /* SPR 2739 changes end */
#ifdef PDCP_ASYNC_INTERFACE
            entity_p->pdcpSecurityContext_p->oldCiphAlgoId = 
                (UInt8)algorithmId;            
            entity_p->pdcpSecurityContext_p->newCiphAlgoId = 
                (UInt8)algorithmId;
            /* SPR 15236 fix start */
            memCpy( entity_p->pdcpSecurityContext_p->oldCiphKey_p,
                key, LTE_SECURITY_KEY_LENGTH );
            memCpy( entity_p->pdcpSecurityContext_p->newCiphKey_p,
                key, LTE_SECURITY_KEY_LENGTH );
            /* SPR 15236 fix end */
            pdcpCreateSecurityContext( (UInt8 *)(&pdcpSecInputTxQ_g), 
                    entity_p->pdcpSecurityContext_p );
#endif
        }
        /* SPR 2854 changes start*/
        entity_p->isDirectionSetDrb = PDCP_DIRECTION_SET;
        /* SPR 2854 changes end*/
    } while (0);
    entity_p = PNULL;

    do 
    {
        /* +Coverity 10084 */
        ueCtx_p = PDCP_GET_UE_CONTEXT(ueIndex);
        if ( PNULL == ueCtx_p )
        {
            break;
        }
        /* +Coverity 10084 */
        /* get entity context for RX direction */
        entity_p = pdcpGetEntityContext( ueIndex, lcId, PDCP_ENTITY_RB_RX );
        if ( PNULL == entity_p )
        {
            break;
        }
        if ( PDCP_DIRECTION_SET == entity_p->isDirectionSetDrb )
        {
            /* configure TX */
            /* SPR 2739 changes start */
            entity_p->ciphering.newAlgorithmId = algorithmId;
            memCpy( entity_p->ciphering.newKey, key, LTE_SECURITY_KEY_LENGTH );
#ifdef PDCP_ASYNC_INTERFACE
            entity_p->pdcpSecurityContext_p->newCiphAlgoId = 
                (UInt8)algorithmId;
            /* SPR 15236 fix start */
            memCpy( entity_p->pdcpSecurityContext_p->newCiphKey_p,
                key, LTE_SECURITY_KEY_LENGTH );;
            /* SPR 15236 fix end */
            pdcpCreateSecurityContext( (UInt8 *)(&pdcpSecInputRxQ_g), 
                    entity_p->pdcpSecurityContext_p );

#endif
            if ( PDCP_RX_STATE_CONNECTED == ueCtx_p->pdcpRxState )
            {
                entity_p->ciphering.algorithmId = algorithmId;
                memCpy( entity_p->ciphering.key, key, LTE_SECURITY_KEY_LENGTH );
#ifdef PDCP_ASYNC_INTERFACE
                entity_p->pdcpSecurityContext_p->oldCiphAlgoId = 
                    (UInt8)algorithmId;            
                /* SPR 15236 fix start */
                memCpy( entity_p->pdcpSecurityContext_p->oldCiphKey_p,
                    key, LTE_SECURITY_KEY_LENGTH );
                /* SPR 15236 fix end */
                pdcpCreateSecurityContext( (UInt8 *)(&pdcpSecInputRxQ_g),
                        entity_p->pdcpSecurityContext_p );
#endif

            }
            /* SPR 2739 changes end */
        }
        /* SPR 2854 changes start*/
        entity_p->isDirectionSetDrb = PDCP_DIRECTION_SET;
        /* SPR 2854 changes end*/
    } while (0);
}
/* SPR_1235_FIX_END*/

/****************************************************************************
 * Function Name  : pdcpEntitySetRohc
 * Inputs         : ueIndex - . 
 *                  lcId - .
 *                  rbDirection - .
 *                  profileIDsMask - .
 *                  maxCid - .
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures ROHC for Entity in one direction.
 ****************************************************************************/
UInt16 pdcpEntitySetRohc( UInt16 ueIndex, UInt16 lcId, UInt16 rbDirection,
        UInt16 profileIDsMask, UInt16 maxCid )
{
    LP_PdcpEnityContext  ctx_p = PNULL;
    void *rohc_entity_p = PNULL;
    rohc_api_info_t  rohc_api_data;
    UInt8 *out_buff_p = PNULL;
    UInt8 *in_buff_p  = PNULL;
    UInt32 eCode = PDCP_SUCCESS;

    if ( PDCP_ENTITY_RB_BOTH == rbDirection )
    {
        LOG_PDCP_MSG (PDCP_INVALID_DIR, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, rbDirection, ueIndex, lcId, 0,
                0,0, __func__,"");
        return PDCP_ERR_ENTITY_WRONG_DIRECTION;
    }

    /* get entity context */
    ctx_p = pdcpGetEntityContext( ueIndex, lcId, rbDirection );
    if ( PNULL == ctx_p )
    {
        LOG_PDCP_MSG( PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, 0,0,0,0,0,0,__func__,"");
        return PDCP_ERR_ENTITY_NOT_FOUND;
    }

    if(PDCP_ENTITY_DRB != ctx_p->type)
    {
        return PDCP_ERR_ENTITY_WRONG_TYPE;
    }

    if(PNULL != ctx_p->rohc.context)
    {
        /*free old ROHC context*/
        LOG_PDCP_MSG( PDCP_ROHC_CONTEXT_ALREADY_INITIALIZED, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, ueIndex, lcId, rbDirection,
                profileIDsMask, maxCid, 0,0, __func__,"");
        return PDCP_FAIL;
    }
    rohc_api_data.entity_id.ueIndex = ueIndex;
    rohc_api_data.entity_id.lcId = lcId;
    rohc_api_data.api_id = ROHC_CONFIG_REQ;
    rohc_api_data.rohc_api_union.config_req.max_cid = maxCid;
    rohc_api_data.rohc_api_union.config_req.profiles = profileIDsMask;
    rohc_api_data.rohc_api_union.config_req.direction = rbDirection;
    if (RO_SUCCESS != rfc3095_process_msg(rohc_entity_p,
                in_buff_p, &out_buff_p , &rohc_api_data, &eCode))
    {
        return PDCP_FAIL;
    }

    ctx_p->rohc.context = out_buff_p;
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpEntityReSetRohc
 * Inputs         : ueIndex - . 
 *                  lcId - .
 *                  rbDirection - .
 *                  profileIDsMask - .
 *                  maxCid - .
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures ROHC for Entity in one direction.
 ****************************************************************************/
UInt16 pdcpEntityReSetRohc( UInt16 ueIndex, UInt16 lcId, UInt16 rbDirection, 
        UInt16 profileIDsMask, UInt16 maxCid )
{
    LP_PdcpEnityContext  ctx_p = PNULL;
    void *rohc_entity_p = PNULL;
    rohc_api_info_t  rohc_api_data;
    UInt8 *out_buff_p = PNULL;
    UInt8 *in_buff_p  = PNULL;
    UInt32 eCode = PDCP_SUCCESS;
#if  defined(PERF_STATS) && !defined(UE_SIM_TESTING)
    rohc_entity_t               *p_rohc_entity = PNULL;
#endif

    if ( PDCP_ENTITY_RB_BOTH == rbDirection )
    {
        LOG_PDCP_MSG (PDCP_INVALID_DIR, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, rbDirection, ueIndex, lcId, 0,
                0,0, __func__,"");
        return PDCP_ERR_ENTITY_WRONG_DIRECTION;
    }

    /* get entity context */
    ctx_p = pdcpGetEntityContext( ueIndex, lcId, rbDirection );
    if ( PNULL == ctx_p )
    {
        LOG_PDCP_MSG( PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, 0,0,0,0,0,0,__func__,"");
        return PDCP_ERR_ENTITY_NOT_FOUND;
    }

    if(PDCP_ENTITY_DRB != ctx_p->type)
    {
        return PDCP_ERR_ENTITY_WRONG_TYPE;
    }

    rohc_entity_p = ctx_p->rohc.context;
    if(PNULL == rohc_entity_p)
    {
        rohc_api_data.api_id = ROHC_CONFIG_REQ;
    }
    else
    {
#if  defined(PERF_STATS) && !defined(UE_SIM_TESTING)
        p_rohc_entity = (rohc_entity_t*)(rohc_entity_p);
        p_rohc_entity->rohc_entity_id.qci = ctx_p->qci;
#endif
        rohc_api_data.api_id = ROHC_RECONFIG_REQ;
    }
    rohc_api_data.entity_id.ueIndex = ueIndex;
    rohc_api_data.entity_id.lcId = lcId;
    rohc_api_data.rohc_api_union.config_req.max_cid = maxCid;
    rohc_api_data.rohc_api_union.config_req.profiles = profileIDsMask;
    rohc_api_data.rohc_api_union.config_req.direction = rbDirection;
    if (RO_SUCCESS != rfc3095_process_msg(rohc_entity_p,
                in_buff_p, &out_buff_p , &rohc_api_data, &eCode))
    {
        return PDCP_FAIL;
    }
    /* +Coverity 54618 */
    /* In case of Reset out_buff_p is always NULL, hence no need to assign in
     * PDCP context */
    if( ROHC_CONFIG_REQ == rohc_api_data.api_id )
    {
        ctx_p->rohc.context = out_buff_p;
    }
    /* -Coverity 54618 */
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpEntityReConfigureRohc
 * Inputs         : ueIndex - . 
 *                  lcId - .
 *                  rbDirection - .
 *                  profileIDsMask - .
 *                  maxCid - .
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures ROHC for Entities in any direction.
 ****************************************************************************/
UInt16 pdcpEntityReConfigureRohc( UInt16 ueIndex, UInt16 lcId, 
        UInt16 rbDirection, UInt16 profileIDsMask, UInt16 maxCid )
{
    UInt16  result = PDCP_SUCCESS;

    if ( PDCP_ENTITY_RB_BOTH == rbDirection )
    {
        /* dirrection BOTH - configure TX */
        /* configure TX - create ROHC compressor context */
        result = pdcpEntityReSetRohc( ueIndex, lcId, PDCP_ENTITY_RB_TX, 
                profileIDsMask, maxCid);

        /* set dirrection to RX */
        rbDirection = PDCP_ENTITY_RB_RX;
    }

    if ( PDCP_SUCCESS == result )
    {
        /* dirrection is one or RX (for BOTH) - configure it */
        /* if BOTH  - create ROHC decompressor context for RX */
        /* else create ROHC context according to rbDirection value */
        result = pdcpEntityReSetRohc( ueIndex, lcId, rbDirection, 
                profileIDsMask, maxCid);
    }

    return result;
}

/****************************************************************************
 * Function Name  : pdcpEntityConfigureRohc
 * Inputs         : ueIndex - . 
 *                  lcId - .
 *                  rbDirection - .
 *                  profileIDsMask - .
 *                  maxCid - .
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures ROHC for Entities in any direction.
 ****************************************************************************/
UInt16 pdcpEntityConfigureRohc(UInt16 ueIndex, UInt16 lcId, UInt16 rbDirection,
        UInt16 profileIDsMask, UInt16 maxCid )
{
    UInt16  result = PDCP_SUCCESS;

    if ( PDCP_ENTITY_RB_BOTH == rbDirection )
    {
        /* dirrection BOTH - configure TX */
        /* configure TX - create ROHC compressor context */
        result = pdcpEntitySetRohc( ueIndex, lcId, PDCP_ENTITY_RB_TX, 
                profileIDsMask, maxCid);

        /* set dirrection to RX */
        rbDirection = PDCP_ENTITY_RB_RX;
    }

    if ( PDCP_SUCCESS == result )
    {
        /* dirrection is one or RX (for BOTH) - configure it */
        /* if BOTH  - create ROHC decompressor context for RX */
        /* else create ROHC context according to rbDirection value */
        result = pdcpEntitySetRohc( ueIndex, lcId, rbDirection, 
                profileIDsMask, maxCid);
    }

    return result;
}

/****************************************************************************
 * Function Name  : updateActiveUeInfo
 * Inputs         : ueIndex,
 *                  activeUeInfo - list of Active UEs
 *                  activeUeIndex - indices for active UEs in activeUeInfo
 *                  ueIndex - UE Id. 
 * Outputs        : 
 * Returns        : None.
 * Description    : updates list of Active UEs in the system.
 ****************************************************************************/
void updateActiveUeInfo(
        UInt16 ueIndex, UInt16 *activeUeInfo_p, UInt16 *activeUeIndex_p)
{
    UInt16 uePos = 0;
    UInt16 lastUePos = 0;

    if ( 0 == activeUeIndex_p[ueIndex] )
    { 
        /*UE is not present in system. Creation request*/
        activeUeInfo_p[pdcpContext.numOfActiveUe - 1] = ueIndex;
        activeUeIndex_p[ueIndex] = pdcpContext.numOfActiveUe;
    }
    else
    {
        /* Deletion request */
        uePos = activeUeIndex_p[ueIndex];
        if (uePos == pdcpContext.numOfActiveUe)
        {
            /* Last UE in the array is deleted */
            activeUeInfo_p[uePos - 1] = 0;
            activeUeIndex_p[ueIndex]  = 0;
        }
        else
        {
            lastUePos = activeUeInfo_p[pdcpContext.numOfActiveUe - 1];

            activeUeInfo_p[uePos - 1] = activeUeInfo_p[pdcpContext.numOfActiveUe - 1];
            activeUeInfo_p[pdcpContext.numOfActiveUe - 1] = 0;
            activeUeIndex_p[ueIndex] = 0;
            activeUeIndex_p[lastUePos] = uePos;
        }
       /* SPR 12889 fix start */
#ifndef UE_SIM_TESTING
       if (0 == setUeIdxInvalid(ueIndex, UE_IDX_MAPPING_FOR_PDCP))
       {
           freeUeIdx(ueIndex);
       }
#endif
       /* SPR 12889 fix end */
    }
}

/****************************************************************************
 * Function Name  : updateActiveLcInfo
 * Inputs         : ueIndex,
 *                  lcId,
 *                  activeLcInfo - list of Active LCs
 *                  activeLcIndex - indices for active LCs in activeLcInfo
 *                  lcId - LC Id. 
 * Outputs        : 
 * Returns        : None.
 * Description    : updates list of Active DRBs in the UE.
 ****************************************************************************/
void updateActiveLcInfo(
        UInt16 ueIndex, UInt8 lcId, UInt8 *activeLcInfo_p, UInt8 *activeLcIndex_p)
{
    UInt8 lcPos = 0;
    UInt8 lastLcPos = 0;

    if ( 0 == activeLcIndex_p[lcId - LTE_DRB_LC_ID_LOW_BOUND])
    { 
        /*LCId is not present in UE. Creation request*/
        activeLcInfo_p[pdcpContext.ueContexts[ueIndex]->numOfActiveDrbLc - 1] =
            lcId;
        activeLcIndex_p[lcId - LTE_DRB_LC_ID_LOW_BOUND] = 
            pdcpContext.ueContexts[ueIndex]->numOfActiveDrbLc;
    }
    else
    {
        /* Deletion request */
        lcPos = activeLcIndex_p[lcId - LTE_DRB_LC_ID_LOW_BOUND];
        if (lcPos == pdcpContext.ueContexts[ueIndex]->numOfActiveDrbLc )
        {
            /* Last LC in the array is deleted */
            activeLcInfo_p[lcPos - 1] = 0;
            activeLcIndex_p[lcId - LTE_DRB_LC_ID_LOW_BOUND] = 0;
        }
        else
        {
            lastLcPos = activeLcInfo_p[pdcpContext.ueContexts[ueIndex]->numOfActiveDrbLc - 1];

            activeLcInfo_p[lcPos - 1] = 
                activeLcInfo_p[pdcpContext.ueContexts[ueIndex]->\
                numOfActiveDrbLc - 1];
            activeLcInfo_p[pdcpContext.ueContexts[ueIndex]->\
                numOfActiveDrbLc - 1] = 0;
            activeLcIndex_p[lcId - LTE_DRB_LC_ID_LOW_BOUND] = 0;
            /* SPR 2620 changes start */
            activeLcIndex_p[lastLcPos - LTE_DRB_LC_ID_LOW_BOUND] = lcPos;
            /* SPR 2620 changes end */
        }
    }
}

/****************************************************************************
 * Function Name  : pdcpSuspendAllRbEntities
 * Inputs         : ueIndex - UE Id. 
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE.
 * Description    : Suspends all the entities configured for an UE.
 ****************************************************************************/
UInt16 pdcpSuspendAllRbEntities( UInt16 ueIndex )
{
    LP_PdcpUeContext ueContext_p = PNULL;
    /* SPR 2739 changes start */
    LP_PdcpEnityContext  entity_p = PNULL;
    UInt16 lcId = 0;
    /* SPR 2739 changes end */
#ifdef LOG_PRINT_ENABLED
    UInt32 state = 0;
#endif

    ueContext_p = PDCP_GET_UE_CONTEXT( ueIndex );
    if ( PNULL == ueContext_p)
    {
        LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, ueIndex, pdcpContext.isInitialized,
                0,0, 0,0, __func__,"");
        return PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED; 
    }

    /* Reset PSR Sent/Receive variables */
    ueContext_p->pdcpPSRTimer      = 0;
    ueContext_p->totNumOfPSRtoSend = 0;
    ueContext_p->numOfPSRSent      = 0;
    ueContext_p->totNumPsrRecvd    = 0;
    ueContext_p->numOfPsrRecvd     = 0;    
    /* SPR 16770 fix start */
    /* Incase at target before data_buffer_stop_ind, suspend is received,
     * reset the flag so that HO legs are not hit. */
    ueContext_p->hoTriggerRecvd = LTE_FALSE;
    /* SPR 16770 fix start */

#ifdef LOG_PRINT_ENABLED
    state = ueContext_p->pdcpTxSrb1State;
#endif
    (void)pdcpRrcTxStateMachine[ueContext_p->pdcpTxSrb1State]
        [PDCP_SUSPENDED] ( ueIndex, LTE_SRB_LC_ID_LOW_BOUND, 
                ueContext_p->pdcpTxSrb1State, PDCP_SUSPENDED, 
                ueContext_p );
    LOG_PDCP_MSG( PDCP_UE_CONTEXT_STATE_TX, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
              __LINE__, ueIndex, state, PDCP_SUSPENDED,
              ueContext_p->pdcpTxSrb1State, LTE_SRB_LC_ID_LOW_BOUND, 0, __func__,
              "PdcpRrcTxStateMachine");

#ifdef LOG_PRINT_ENABLED
    state = ueContext_p->pdcpTxSrb2DrbState;
#endif
    ueContext_p->event = PDCP_SUSPENDED;
    (void)pdcpRrcTxStateMachine[ueContext_p->pdcpTxSrb2DrbState]
        [ueContext_p->event] ( ueIndex, LTE_SRB_LC_ID_HIGH_BOUND,
                ueContext_p->pdcpTxSrb2DrbState, 
                ueContext_p->event, ueContext_p );
    LOG_PDCP_MSG( PDCP_UE_CONTEXT_STATE_TX, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
              __LINE__, ueIndex, state, ueContext_p->event,
              ueContext_p->pdcpTxSrb2DrbState, LTE_SRB_LC_ID_HIGH_BOUND, 0, __func__,
              "PdcpRrcTxStateMachine");

    /* SPR 2739 changes start */
    /* Change RX state to HO when suspend request received */
    ueContext_p->pdcpRxState = PDCP_RX_STATE_HO;
    /* Copy new keys and algo Id to old keys as its a trigger point for
     * re-establishment procedure */
    for (lcId = 0; lcId < ueContext_p->numOfActiveDrbLc; lcId++)
    {
        /* SPR 5485 changes start */
        entity_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueContext_p,
                ueContext_p->activeLcInfo[lcId], 
                PDCP_ENTITY_RB_RX );
        /* SPR 5485 changes end */

        entity_p->ciphering.algorithmId = entity_p->ciphering.newAlgorithmId;
        memCpy( entity_p->ciphering.key, entity_p->ciphering.newKey, 
                LTE_SECURITY_KEY_LENGTH );
        /* SPR 5034 FIX START */
        entity_p->fms = (entity_p->last_Submitted_SN + 1 ) % 
            (PDCP_TS_MAX_SN_AM_MODE + 1);
        /* SPR 5034 FIX END */
#ifdef PDCP_ASYNC_INTERFACE
        entity_p->pdcpSecurityContext_p->oldCiphAlgoId = 
            (UInt8)entity_p->ciphering.newAlgorithmId; 
        /* SPR 15236 fix start */
        memCpy( entity_p->pdcpSecurityContext_p->oldCiphKey_p, 
            entity_p->ciphering.key, LTE_SECURITY_KEY_LENGTH );
        /* SPR 15236 fix end */
        pdcpCreateSecurityContext( (UInt8 *)(&pdcpSecInputRxQ_g), 
                entity_p->pdcpSecurityContext_p );
#endif
        /* Reset isPsrRecvd flag */
        entity_p->isPsrRecvd = LTE_FALSE;
        /* SPR 16770 fix start */
        /* SPR 16973 fix start */
        entity_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueContext_p,
                ueContext_p->activeLcInfo[lcId], 
                PDCP_ENTITY_RB_TX );
        /* SPR 16973 fix end */
        /* Reset HO (forwarding) related flags in TX context  */
        entity_p->isFwdingEnabled = LTE_FALSE;
        entity_p->isEndMarkerIndRecvd = LTE_FALSE;
        entity_p->isEMIFwded = LTE_FALSE;
        entity_p->numberOfNodesFwded = 0;
        entity_p->dataNode = PNULL;
        /* SPR 16770 fix end */

    }
    /* SPR 2739 changes end */
    return PDCP_SUCCESS;
}

/* SPR 3569 changes start */
/****************************************************************************
 * Function Name  : pdcpResumeRbEntity
 * Inputs         : ueIndex - UE Id, lcId - LC ID, resumeDirection
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE.
 * Description    : Resumes RB entity configured for an UE.
 ****************************************************************************/
UInt16 pdcpResumeRbEntity( UInt16 ueIndex, UInt8 lcId,
        UInt16 resumeDirection)
{
    UInt16  result = PDCP_SUCCESS;

    if ( PDCP_ENTITY_RB_BOTH == resumeDirection )
    {
        /* direction BOTH - Resume TX direction */
        result = pdcpEntityResume( ueIndex, lcId, PDCP_ENTITY_RB_TX );

        /* set dirrection to RX */
        resumeDirection = PDCP_ENTITY_RB_RX;
    }

    if ( PDCP_SUCCESS == result )
    {
        /* Resume for RX direction if direction received is both */
        /* If direction received is TX or RX, resume TX or RX, respectively */
        result = pdcpEntityResume( ueIndex, lcId, resumeDirection );
    }
    return result;
}

/****************************************************************************
 * Function Name  : pdcpEntityResume
 * Inputs         : ueIndex - UE Id, lcId - LC ID, resumeDirection
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE.
 * Description    : Resumes RB entity configured for an UE.
 ****************************************************************************/
UInt16 pdcpEntityResume( UInt16 ueIndex, UInt8 lcId,
        UInt16 resumeDirection)
{
    LP_PdcpUeContext ueContext_p = PNULL;
    LP_PdcpEnityContext  ctx_p   = PNULL;
#ifndef UE_SIM_TESTING        
    InternalCellIndex  cellIndex   =  0;
#endif    
#ifdef LOG_PRINT_ENABLED
    UInt32 state = 0;
#endif
    ueContext_p = PDCP_GET_UE_CONTEXT( ueIndex );
    if ( PNULL == ueContext_p)
    {
        LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, ueIndex, pdcpContext.isInitialized,
                0,0, 0,0, __func__,"");
        return PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED; 
    }
#ifndef UE_SIM_TESTING    
    cellIndex   =  ueContext_p->cellIndex;
#endif    

    if ( PDCP_ENTITY_RB_TX == resumeDirection )
    {
        if (LTE_SRB_LC_ID_LOW_BOUND == lcId)  
        {
#ifdef LOG_PRINT_ENABLED
            state = ueContext_p->pdcpTxSrb1State;
#endif
            (void)pdcpRrcTxStateMachine[ueContext_p->pdcpTxSrb1State]
                [PDCP_CONNECTED] ( ueIndex, lcId, 
                        ueContext_p->pdcpTxSrb1State, PDCP_CONNECTED, 
                        ueContext_p );
            LOG_PDCP_MSG( PDCP_UE_CONTEXT_STATE_TX, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                     __LINE__, ueIndex, state, PDCP_CONNECTED,
                    ueContext_p->pdcpTxSrb1State, lcId, 0, __func__,
                    "PdcpRrcTxStateMachine");
        }
        else
        {
            if( PDCP_AWAITED != ueContext_p->event ) 
            {
                ueContext_p->event = PDCP_RESUME;
            }
            ctx_p = pdcpGetEntityContext( ueIndex, lcId, PDCP_ENTITY_RB_RX );
            if( PNULL == ctx_p )
            {
                LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_RRCOAM,
                        pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 0,0, 0,0, 
                        __func__,"");
                return PDCP_ERR_ENTITY_NOT_FOUND; 
            }
#ifdef UE_SIM_TESTING                    
            if ( LTE_TRUE == ctx_p->ueStatusReportReq ) 
#else                            
            if ( LTE_TRUE == ctx_p->statusReportRequired ) 
#endif
            {
                /* Encode PSR and put it in CtrlPDU Q to be sent to RLC */
                pdcpEncodePSR( ueIndex, lcId, ctx_p );
                ueContext_p->event = PDCP_AWAITED;
                (ueContext_p->totNumOfPSRtoSend)++;
            }
#ifndef UE_SIM_TESTING 
            if( LTE_TRUE == ctx_p->ueStatusReportReq ) 
#else
            if( LTE_TRUE == ctx_p->statusReportRequired )
#endif
            {
                /* PSR is expected from peer so start the PSR expiry timer and
                 * set the state to PDCP_AWAITED */
#ifndef UE_SIM_TESTING 
                ueContext_p->pdcpPSRTimer = pdcpGlobalTTITickCount_g[cellIndex];
#else
                ueContext_p->pdcpPSRTimer = pdcpCurrentTime_g;
#endif
                ueContext_p->event = PDCP_AWAITED;
                (ueContext_p->totNumPsrRecvd)++;
            } 
            /* SPR 10802 FIX START */
            if(( PDCP_TX_STATE_SUSPENDED == ueContext_p->pdcpTxSrb2DrbState )||
                ( PDCP_TX_STATE_RESUME == ueContext_p->pdcpTxSrb2DrbState && 
                 PDCP_AWAITED == ueContext_p->event ) )
            {
                /* SPR 10802 FIX END */
#ifdef LOG_PRINT_ENABLED
                state = ueContext_p->pdcpTxSrb2DrbState;
#endif
                (void)pdcpRrcTxStateMachine[ueContext_p->pdcpTxSrb2DrbState]
                    [ueContext_p->event] ( ueIndex, lcId, 
                            ueContext_p->pdcpTxSrb2DrbState, 
                            ueContext_p->event, ueContext_p );
                LOG_PDCP_MSG( PDCP_UE_CONTEXT_STATE_TX, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                         __LINE__, ueIndex, state, ueContext_p->event,
                        ueContext_p->pdcpTxSrb2DrbState, lcId, 0, __func__,
                        "PdcpRrcTxStateMachine");
            }
        }
    }
    else /* RX direction */
    {
        if (LTE_SRB_LC_ID_LOW_BOUND != lcId) 
        {
            /* SPR 2739 changes start */
            /* Change RX state to CONNECTED when RESUME request received */
            ueContext_p->pdcpRxState = PDCP_RX_STATE_CONNECTED;
            /* SPR 2739 changes end */
            LOG_PDCP_MSG( PDCP_UE_CONTEXT_STATE_RX, LOGINFO, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, ueIndex, ueContext_p->pdcpRxState,
                    0, 0, 0,0, __func__,"");
        }
    }
    return PDCP_SUCCESS;
}
/* SPR 3569 changes end */

/****************************************************************************
 * Function Name  : pdcpReEstablishDl
 * Inputs         : ueIndex - UE Id, lcId - LC ID
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE.
 * Description    : Re-establishes RB entity configured for an UE for DL.
 ****************************************************************************/
UInt16 pdcpReEstablishDl( UInt16 ueIndex, UInt8 lcId )
{
    LP_PdcpEnityContext ctx_p = PNULL;
    LP_PdcpSrbDataReq dataReq_p  = PNULL;
    rohc_api_info_t  rohc_api_data = {0};
    UInt8 *out_buff_p = PNULL;
    UInt32 eCode = PDCP_SUCCESS;
#if  defined(PERF_STATS) && !defined(UE_SIM_TESTING)
    rohc_entity_t               *p_rohc_entity = PNULL;
#endif 

    ctx_p = pdcpGetEntityContext(ueIndex, lcId, PDCP_ENTITY_RB_TX);
    if ( PNULL == ctx_p ) 
    {
        LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 0, 0, 
                0,0, __func__,"");
        return PDCP_ERR_ENTITY_NOT_FOUND;
    }

    if ( LTE_SRB_LC_ID_HIGH_BOUND >= lcId ) /*SRB*/
    {
        /* Set Next_PDCP_TX_SN and TX_HFN to 0 */
        ctx_p->next_SN = 0;
        ctx_p->hfn     = 0;

        LOG_PDCP_MSG( PDCP_STATE_VARS_RESET_TX, LOGDEBUG, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, ueIndex, lcId, ctx_p->next_SN,
                ctx_p->hfn, 0, 0, __func__, "" );

        /* Discard fresh SDUs from RRC */
        do
        {
            DEQUEUE_PDCP_SRB_TX_DATA_REQ_Q(ctx_p, dataReq_p);
            if ( PNULL != dataReq_p )
            {
                /* + SPR_16046 */
                if ( LTE_TRUE == dataReq_p->srbServiceReqested )
                {
                    pdcpRrcSrbDataStatusInd( 0, ueIndex, lcId,
                            dataReq_p->transactionId, PDCP_FAIL,
                            PDCP_ERR_DT_RLC_NACK, dataReq_p->data_p,
                            LTE_TRUE,  msgSize(dataReq_p->data_p, PNULL));
                }
                /* - SPR_16046 */
                pdcpFreeSrbTxDataReq(dataReq_p);
            }          
        } while ( PNULL != dataReq_p );

        LOG_PDCP_MSG( PDCP_Q_EMPTY, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, ueIndex, lcId, 0,0, 0,0, __func__, "pdcpTxDataReqQ" );

        /* Discard PDUs whose delivery indication is not yet recvd from RLC */
        do
        {
            DEQUEUE_PDCP_SRB_TX_STORAGE_Q(ctx_p, dataReq_p);
            if ( PNULL != dataReq_p )
            {
                /* + SPR_16046 */
                if ( LTE_TRUE == dataReq_p->srbServiceReqested )
                {
                    pdcpRrcSrbDataStatusInd( 0, ueIndex, lcId,
                            dataReq_p->transactionId, PDCP_FAIL,
                            PDCP_ERR_DT_RLC_NACK, dataReq_p->data_p,
                            LTE_TRUE,  msgSize(dataReq_p->data_p, PNULL));
                }
                /* - SPR_16046 */

                pdcpFreeSrbTxDataReq(dataReq_p);
            }
        } while ( PNULL != dataReq_p );
        
        LOG_PDCP_MSG( PDCP_Q_EMPTY, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, ueIndex, lcId, 0,0, 0,0, __func__, "pdcpStorageQ" );
    }
    else /* DRB */
    {
        if ( PDCP_ENTITY_RLC_UM == ctx_p->rlcMode )
        {
            /* Reset the header compression protocol for DL */
            if(PNULL != ctx_p->rohc.context) 
            {
                rohc_api_data.api_id = ROHC_RESET_REQ;
                rohc_api_data.entity_id.ueIndex = ueIndex;
                rohc_api_data.entity_id.lcId = lcId;
#if  defined(PERF_STATS) && !defined(UE_SIM_TESTING)
                p_rohc_entity = (rohc_entity_t*)(ctx_p->rohc.context);
                p_rohc_entity->rohc_entity_id.qci = ctx_p->qci;
#endif
                rohc_api_data.rohc_api_union.rohc_data_req.feedback_flag = 
                    LTE_FALSE;
                rohc_api_data.rohc_api_union.rohc_data_req.in_buff_len = 0;

                if(RO_SUCCESS != rfc3095_process_msg(ctx_p->rohc.context,
                            PNULL, &out_buff_p, &rohc_api_data, &eCode))
                {
                    return PDCP_FAIL;
                }
            }
            /* Set Next_PDCP_TX_SN and TX_HFN to 0 */
            ctx_p->next_SN = 0;
            ctx_p->hfn     = 0;

            LOG_PDCP_MSG( PDCP_STATE_VARS_RESET_TX, LOGDEBUG, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 
                    ctx_p->next_SN, ctx_p->hfn, 0,0, __func__, "" );
        }
        else /* AM mode */
        {
            /* Reset the header compression protocol for DL */
            if(PNULL != ctx_p->rohc.context) 
            {
                rohc_api_data.api_id = ROHC_RESET_REQ;
                rohc_api_data.entity_id.ueIndex = ueIndex;
                rohc_api_data.entity_id.lcId = lcId;
#if  defined(PERF_STATS) && !defined(UE_SIM_TESTING)
                p_rohc_entity = (rohc_entity_t*)(ctx_p->rohc.context);
                p_rohc_entity->rohc_entity_id.qci = ctx_p->qci;
#endif
                rohc_api_data.rohc_api_union.rohc_data_req.feedback_flag = 
                    LTE_FALSE;
                rohc_api_data.rohc_api_union.rohc_data_req.in_buff_len = 0;

                if(RO_SUCCESS != rfc3095_process_msg(ctx_p->rohc.context,
                            PNULL, &out_buff_p, &rohc_api_data, &eCode))
                {
                    return PDCP_FAIL;
                }
            }
            LOG_PDCP_MSG( PDCP_STATE_VARS_RESET_TX, LOGDEBUG, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, ueIndex, lcId, ctx_p->next_SN,
                    ctx_p->hfn, 0, 0, __func__, "" );
        }
    }
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpReEstablishUl
 * Inputs         : ueIndex - UE Id, lcId - LC ID
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE.
 * Description    : Re-establishes RB entity configured for an UE for UL.
 ****************************************************************************/
UInt16 pdcpReEstablishUl( UInt16 ueIndex, UInt8 lcId )
{
    LP_PdcpEnityContext ctx_p = PNULL;
    LP_PdcpDataInd dataInd_p  = PNULL;
    rohc_api_info_t  rohc_api_data = {0};
    UInt8 *out_buff_p = PNULL;
    UInt32 eCode = PDCP_SUCCESS;
#if  defined(PERF_STATS) && !defined(UE_SIM_TESTING)
    rohc_entity_t               *p_rohc_entity = PNULL;
#endif 

    ctx_p = pdcpGetEntityContext(ueIndex, lcId, PDCP_ENTITY_RB_RX);
    if ( PNULL == ctx_p ) 
    {
        LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 0, 0, 
                0,0, __func__,"");
        return PDCP_ERR_ENTITY_NOT_FOUND;
    }

    if ( LTE_SRB_LC_ID_HIGH_BOUND >= lcId ) /*SRB*/
    {
        /*
         ** Discard the PDCP Data PDUs that are received from lower layers due 
         ** to the re-establishment of the lower layers.
         */
        do
        {
            DEQUEUE_PDCP_RX_DATA_IND_Q(ctx_p, dataInd_p);
            if ( PNULL != dataInd_p )
            {
                pdcpFreeRxDataInd(dataInd_p);
            }             
        } while ( PNULL != dataInd_p );
        LOG_PDCP_MSG( PDCP_Q_EMPTY, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, ueIndex, lcId, 0,0, 0,0, __func__, "pdcpRxDataIndQ" );

        /* Set Next_PDCP_RX_SN and RX_HFN to 0 */
        ctx_p->next_SN = 0;
        ctx_p->hfn     = 0;
        /* Set last_Submitted_RX_SN to 4095 */
        ctx_p->last_Submitted_SN = PDCP_TS_MAX_SN_AM_MODE;

        LOG_PDCP_MSG( PDCP_STATE_VARS_RESET_RX, LOGDEBUG, PDCP_RRCOAM, 
                pdcpCurrentTime_g, ueIndex, lcId, ctx_p->next_SN,
                ctx_p->hfn, ctx_p->last_Submitted_SN, 0,0, 
                __func__, "AM mode" );
        /* Discard all stored SDUs recvd from RLC */
        do
        {
            DEQUEUE_PDCP_RX_STORAGE_Q(ctx_p, dataInd_p);
            if ( PNULL != dataInd_p )
            {
                pdcpFreeRxDataInd(dataInd_p);
            }             
        } while ( PNULL != dataInd_p );
        LOG_PDCP_MSG( PDCP_Q_EMPTY, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, ueIndex, lcId, 0,0, 0,0, __func__, 
                "pdcpUlReorderingQ" );
    }
    else /* DRB */
    {
        if ( PDCP_ENTITY_RLC_UM == ctx_p->rlcMode )
        {
            /* Reset the header compression protocol for UL */
            if(PNULL != ctx_p->rohc.context) 
            {
                rohc_api_data.api_id = ROHC_RESET_REQ;
                rohc_api_data.entity_id.ueIndex = ueIndex;
                rohc_api_data.entity_id.lcId = lcId;
#if  defined(PERF_STATS) && !defined(UE_SIM_TESTING)
                p_rohc_entity = (rohc_entity_t*)(ctx_p->rohc.context);
                p_rohc_entity->rohc_entity_id.qci = ctx_p->qci;
#endif
                rohc_api_data.rohc_api_union.rohc_data_req.feedback_flag = 
                    LTE_FALSE;
                rohc_api_data.rohc_api_union.rohc_data_req.in_buff_len = 0;

                if(RO_SUCCESS != rfc3095_process_msg(ctx_p->rohc.context,
                            PNULL, &out_buff_p, &rohc_api_data, &eCode))
                {
                    return PDCP_FAIL;
                }
            }
            /* Set Next_PDCP_RX_SN and RX_HFN to 0 */
            ctx_p->next_SN = 0;
            ctx_p->hfn     = 0;

            LOG_PDCP_MSG( PDCP_STATE_VARS_RESET_RX, LOGDEBUG, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, ueIndex, lcId, ctx_p->next_SN,
                    ctx_p->hfn, 0, 0,0, __func__, "UM mode" );
        }
        else /* AM mode */
        {
            /*3608*/
#ifndef UE_SIM_TESTING
            if (LTE_TRUE == ctx_p->statusReportRequired)
#else
            if (LTE_TRUE == ctx_p->ueStatusReportReq)
#endif
            {
                pdcpEncodeUlBitmap(ueIndex, lcId, ctx_p );
            }
            ctx_p->fms = (ctx_p->last_Submitted_SN + 1 ) % 
                (PDCP_TS_MAX_SN_AM_MODE + 1);
            /*3608*/

            /* Reset the header compression protocol for UL */
            if(PNULL != ctx_p->rohc.context) 
            {
                rohc_api_data.api_id = ROHC_RESET_REQ;
                rohc_api_data.entity_id.ueIndex = ueIndex;
                rohc_api_data.entity_id.lcId = lcId;
                rohc_api_data.rohc_api_union.rohc_data_req.feedback_flag = 
                    LTE_FALSE;

#if  defined(PERF_STATS) && !defined(UE_SIM_TESTING)
                p_rohc_entity = (rohc_entity_t*)(ctx_p->rohc.context);
                p_rohc_entity->rohc_entity_id.qci = ctx_p->qci;
#endif
                rohc_api_data.rohc_api_union.rohc_data_req.in_buff_len = 0;

                if(RO_SUCCESS != rfc3095_process_msg(ctx_p->rohc.context,
                            PNULL, &out_buff_p, &rohc_api_data, &eCode))
                {
                    return PDCP_FAIL;
                }
            }
            LOG_PDCP_MSG( PDCP_STATE_VARS_RESET_RX, LOGDEBUG, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, ueIndex, lcId, ctx_p->next_SN,
                    ctx_p->hfn, ctx_p->last_Submitted_SN, 0,0, __func__, 
                    "AM mode" );
        }
    }
    return PDCP_SUCCESS;
}

/******************************************************************************
 * Function Name  : pdcpEncodeUlBitmap
 * Inputs         : ueIndex, lcId, ctx_p - UE + LC context
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE.
 * Description    : Encodes bitmap for UL packets received for a LC. This
 *                  function is called only when StatusReportRequired field set
 *                  true by RRC.
 *****************************************************************************/
UInt16 pdcpEncodeUlBitmap( UInt16 ueIndex, UInt8 lcId,
        LP_PdcpEnityContext ctx_p )
{
#ifndef PDCP_ASYNC_INTERFACE
    LP_PdcpUeContext ueCtx_p   = PNULL;    
#endif
    LP_PdcpDataInd   dataInd_p = PNULL; 
    UInt16           editByte  = 0;
    UInt16           diff      = 0;
    UInt16     lastSubmittedSn = 0;
    /* SPR 8703 fix start */
	UInt32     lastSubmittedCount = 0;
	UInt32     lastSubmittedCountPlus2 = 0;
    /* SPR 8703 fix end */
    UInt8            editBit   = 0;
#ifndef PDCP_ASYNC_INTERFACE
#ifdef LOG_PRINT_ENABLED
    UInt32           state     = 0;
#endif
    /* Make sure that all OOO packets are processed which are recvd from RLC */
    ueCtx_p = PDCP_GET_VALID_UE_CONTEXT(ueIndex);
#endif
    UInt64 count = 0;

    /* SPR 10721 fix start */ 
#ifndef PDCP_ASYNC_INTERFACE
    /* SPR 10721 fix end */ 
    count = QCOUNT_PDCP_RX_DATA_IND_Q(ctx_p);
    while ( count-- )
    {
        /* Process PDU*/
#ifdef LOG_PRINT_ENABLED
        state = ueCtx_p->pdcpRxState;
#endif
        (void)pdcpRxStateMachine[ueCtx_p->pdcpRxState][PDCP_DRB_RX](
                ueIndex, lcId, ueCtx_p->pdcpRxState,
                PDCP_DRB_RX, ctx_p);
        LOG_PDCP_MSG( PDCP_RX_EVENT_RCVD, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
                 __LINE__, ueIndex, state, PDCP_DRB_RX, ueCtx_p->pdcpRxState, lcId,0, __func__,
                 "PdcpRxStateMachine");
    }
#endif
    /* Update lastSubmittedSn, with updated value */
    lastSubmittedSn = ctx_p->last_Submitted_SN;
    /* SPR 8703 fix start */
    /* SPR 9008 changes start */
	if( (ctx_p->next_SN > lastSubmittedSn) || (0 == ctx_p->hfn) )
        /* SPR 9008 changes end */
    {
        lastSubmittedCount = PDCP_MAKE_COUNT( lastSubmittedSn, ctx_p->hfn, 
                ctx_p->snSize);
    }
	else
    {
        lastSubmittedCount = PDCP_MAKE_COUNT( lastSubmittedSn, ctx_p->hfn -1, 
                ctx_p->snSize);
    }
    memSet(ctx_p->pdcpUlStatusInfo.bitmap, 0, BITMAP_SIZE);
    /* SPR 8703 fix end */

    /* Get nodes from PDCP re-ordering queue if there is any packet that has 
     * not been transmitted to Packet Relay */
    count = QCOUNT_PDCP_RX_STORAGE_Q(ctx_p);

    /* SPR 5112 changes start */
    /* Initialize numOfBytes for PSR */
    ctx_p->pdcpUlStatusInfo.noOfBytes = 0;
    /* SPR 5112 changes end */

    if ( 0 < count )
    {
        GET_FIRST_PDCP_RX_STORAGE_Q( ctx_p, dataInd_p );
        if ( PNULL == dataInd_p )
        {
            LOG_PDCP_MSG( PDCP_NULL_NODE_Q, LOGWARNING, (PDCP_RX | PDCP_RRCOAM), 
                    pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 0,0, 0,0,
                    __func__, "pdcpUlReorderingQ");
            return PDCP_FAIL;
        }
        else
        {
            /* SPR 8703 fix start */
            if( MAX_UINT32_VALUE == lastSubmittedCount )
            {
                lastSubmittedCountPlus2 = 1;
            }
            else if( (MAX_UINT32_VALUE - 1) == lastSubmittedCount )
            {
                lastSubmittedCountPlus2 = 0;
            }
            else
            {
                lastSubmittedCountPlus2 = lastSubmittedCount + 2;
            }
            /* SPR 8703 fix end */
            do 
            {
                /* SPR 8703 fix start */
				if(dataInd_p->count >= lastSubmittedCountPlus2)
				{
					diff = dataInd_p->count - lastSubmittedCountPlus2;
				}
				else
                {
                    diff = MAX_UINT32_VALUE - lastSubmittedCountPlus2 +
                        dataInd_p->count;
                }

                /* Following check is to remove coverity warning */
                if( PDCP_TS_MAX_SN_AM_MODE < diff )
                {
                    /* This condition will never occur as number of packets 
                     * cannot be more than 4096 in queue as per 36.323 */
                    LOG_PDCP_MSG( PDCP_CANNOT_ADD_UL_BITMAP, LOGWARNING, 
                            (PDCP_RX | PDCP_RRCOAM), pdcpCurrentTime_g, 
                            __LINE__, ueIndex, lcId, lastSubmittedCount, 
                            dataInd_p->count, 0, 0, __func__, "");
                    /* No packet after this packet will be eligible for bitmap
                       as this queue has nodes in sorted order */
                    break;
                }

                editByte = diff / NUM_BITS_IN_BYTES;
                editBit = diff % NUM_BITS_IN_BYTES;
                /* SPR 8703 fix end */
                /* Sets for packets that need not be re-transmitted as per
                 * 36.323 V8.6.0 [Section 6.3.10] */
                switch ( editBit)
                {
                    case 0:
                    {
                        ctx_p->pdcpUlStatusInfo.bitmap[editByte] |= SET_BIT_1;
                        break;
                    }
                    case 1:
                    {
                        ctx_p->pdcpUlStatusInfo.bitmap[editByte] |= SET_BIT_2;
                        break;
                    }
                    case 2:
                    {
                        ctx_p->pdcpUlStatusInfo.bitmap[editByte] |= SET_BIT_3;
                        break;
                    }
                    case 3:
                    {
                        ctx_p->pdcpUlStatusInfo.bitmap[editByte] |= SET_BIT_4;
                        break;
                    }
                    case 4:
                    {
                        ctx_p->pdcpUlStatusInfo.bitmap[editByte] |= SET_BIT_5;
                        break;
                    }
                    case 5:
                    {
                        ctx_p->pdcpUlStatusInfo.bitmap[editByte] |= SET_BIT_6;
                        break;
                    }
                    case 6:
                    {
                        ctx_p->pdcpUlStatusInfo.bitmap[editByte] |= SET_BIT_7;
                        break;
                    }
                    case 7:
                    {
                        ctx_p->pdcpUlStatusInfo.bitmap[editByte] |= SET_BIT_8;
                        break;
                    }
                }
                GET_NEXT_PDCP_RX_STORAGE_Q(ctx_p, dataInd_p, dataInd_p);
            }while ( PNULL != dataInd_p );
            /* SRP 3608, 5112 changes start */
            ctx_p->pdcpUlStatusInfo.noOfBytes = editByte + 1;
            /* SRP 3608, 5112 changes end */
        }
        }
    else
    {
        /* No missing SDUs which PDCP is not able to transmit to Packet 
         * Relay */
        LOG_PDCP_MSG( PDCP_NO_UL_BITMAP, LOGINFO, (PDCP_RX | PDCP_RRCOAM), 
                pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 0,0, 0,0,
                __func__, ""); 
    }
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpSetConnectedState
 * Inputs         : ueIndex, lcId, state, event, ue Context
 * Outputs        : 
 * Returns        : PDCP_SUCCESS
 * Description    : sets PDCP TX state to connected.
 ****************************************************************************/
UInt32 pdcpSetConnectedState( UInt16 ueIndex, UInt8 lcId, UInt32 state, 
        UInt32 event, LP_PdcpUeContext ueCtx_p )
{
    if ( LTE_SRB_LC_ID_LOW_BOUND == lcId )
    {
        ueCtx_p->pdcpTxSrb1State = PDCP_TX_STATE_CONNECTED;
    }
    else
    {
        ueCtx_p->pdcpTxSrb2DrbState = PDCP_TX_STATE_CONNECTED;
    }
    /* SPR +- 17777 */
    LOG_PDCP_MSG( PDCP_RX_EVENT_RCVD, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
            __LINE__, ueIndex, state,event, lcId,0,0, __func__,
            "PdcpRxStateMachine");
    /* SPR +- 17777 */

    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpInvalidState
 * Inputs         : ueIndex, lcId, state, event, ue Context
 * Outputs        : 
 * Returns        : PDCP_SUCCESS
 * Description    : Logs an error for invalid state.
 ****************************************************************************/
UInt32 pdcpInvalidState( UInt16 ueIndex, UInt8 lcId, UInt32 state, 
        UInt32 event, LP_PdcpUeContext ueCtx_p )
{
    /* SPR +- 17777 */
    LOG_PDCP_MSG( PDCP_INVALID_STATE, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
            ueIndex, lcId, ueCtx_p->pdcpTxSrb1State, 
            ueCtx_p->pdcpTxSrb2DrbState, event,state, 0, __func__, "");
    /* SPR +- 17777 */
    
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpSetHoState
 * Inputs         : ueIndex, lcId, state, event, ue Context
 * Outputs        : 
 * Returns        : PDCP_SUCCESS
 * Description    : sets PDCP TX state to HO.
 ****************************************************************************/
UInt32 pdcpSetHoState( UInt16 ueIndex, UInt8 lcId, UInt32 state, 
        UInt32 event, LP_PdcpUeContext ueCtx_p )
{
    if ( LTE_SRB_LC_ID_LOW_BOUND == lcId )
    {
        ueCtx_p->pdcpTxSrb1State = PDCP_TX_STATE_HO;
    }
    else
    {
        ueCtx_p->pdcpTxSrb2DrbState = PDCP_TX_STATE_HO;
        /* Reset PSR Sent/Receive variables */
        ueCtx_p->pdcpPSRTimer      = 0;
        ueCtx_p->totNumOfPSRtoSend = 0;
        ueCtx_p->numOfPSRSent      = 0;
        ueCtx_p->totNumPsrRecvd    = 0;
        ueCtx_p->numOfPsrRecvd     = 0;    
    }
    /* SPR +- 17777 */
    LOG_PDCP_MSG( PDCP_RX_EVENT_RCVD, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
            __LINE__, ueIndex, state,event, lcId,0,0, __func__,
            "PdcpRxStateMachine");
    /* SPR +- 17777 */

    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpSetSuspendedState
 * Inputs         : ueIndex, lcId, state, event, ue Context
 * Outputs        : 
 * Returns        : PDCP_SUCCESS
 * Description    : sets PDCP TX state to suspended.
 ****************************************************************************/
UInt32 pdcpSetSuspendedState( UInt16 ueIndex, UInt8 lcId, UInt32 state, 
        UInt32 event, LP_PdcpUeContext ueCtx_p )
{
    if ( LTE_SRB_LC_ID_LOW_BOUND == lcId )
    {
        ueCtx_p->pdcpTxSrb1State = PDCP_TX_STATE_SUSPENDED;
    }
    else
    {
        ueCtx_p->pdcpTxSrb2DrbState = PDCP_TX_STATE_SUSPENDED;
    }

    /* Reset the forwarding variables */
    ueCtx_p->isFwdingApiRecvd = LTE_FALSE ;
    /* SPR 10894 fix start*/
    ueCtx_p->newUeIndex = INVALID_UE_INDEX;
    /* Set the type of HO as NORMAL */
    ueCtx_p->hoType = NORMAL_HO;
    /* SPR +- 17777 */
    LOG_PDCP_MSG( PDCP_RX_EVENT_RCVD, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
            __LINE__, ueIndex, state,event, lcId,0,0, __func__,
            "PdcpRxStateMachine");
    /* SPR +- 17777 */

    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpSetResumeState
 * Inputs         : ueIndex, lcId, state, event, ue Context
 * Outputs        : 
 * Returns        : PDCP_SUCCESS
 * Description    : sets PDCP TX state to resume.
 ****************************************************************************/
UInt32 pdcpSetResumeState( UInt16 ueIndex, UInt8 lcId, UInt32 state, 
        UInt32 event, LP_PdcpUeContext ueCtx_p )
{
    if ( LTE_SRB_LC_ID_LOW_BOUND == lcId )
    {
        ueCtx_p->pdcpTxSrb1State = PDCP_TX_STATE_RESUME;
    }
    else
    {
        ueCtx_p->pdcpTxSrb2DrbState = PDCP_TX_STATE_RESUME;
        /* SPR 10805 FIX START*/
#ifdef L2_FAST_PATH
        /* Set the flag to true so that storage queue can be processed from
         * pdcpElTxProcessDrbMessage */
        pdcpIsDrbDataReceivedInTxQ_g = LTE_TRUE;
#endif
        /* SPR 10805 FIX END*/
        if( LTE_SRB_LC_ID_HIGH_BOUND < lcId )
        {
            /* If resume request is received for DRB, fill the storageCount for
             * each entity with number of nodes present in storage queue */
            UInt8 lcCtr = 0;
            while ( lcCtr < ueCtx_p->numOfActiveDrbLc )
            {
                LP_PdcpEnityContext ctx_p = 
                    PDCP_GET_VALID_ENTITY_CONTEXT( 
                            ueCtx_p, ueCtx_p->activeLcInfo[lcCtr], 
                            PDCP_ENTITY_RB_TX );
                ctx_p->storageCount = QCOUNT_PDCP_TX_STORAGE_Q( ctx_p );
                lcCtr++;
            }
        }
        /* +- SPR 20801 */
   }
    /* SPR +- 17777 */
    LOG_PDCP_MSG( PDCP_RX_EVENT_RCVD, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
            __LINE__, ueIndex, state,event, lcId,0,0, __func__,
            "PdcpRxStateMachine");
    /* SPR +- 17777 */

    return PDCP_SUCCESS;
}

/* SPR 3608 changes start */
/****************************************************************************
 * Function Name  : pdcpSetAwaitedState
 * Inputs         : ueIndex, lcId, state, event, ue Context
 * Outputs        :
 * Returns        : UInt32
 * Description    : sets PDCP TX state to Awaited and event to RESUME.
 ****************************************************************************/
UInt32 pdcpSetAwaitedState( UInt16 ueIndex, UInt8 lcId, UInt32 state,
        UInt32 event, LP_PdcpUeContext ueCtx_p )
{
    if (LTE_SRB_LC_ID_LOW_BOUND == lcId)
    {
        /* Not a valid event in case of SRB1 */
    }
    else
    {
        ueCtx_p->pdcpTxSrb2DrbState = PDCP_TX_STATE_AWAITED;
        /* SPR 10721 fix start */
        /* In awaited state, storage queue count is set to 0, so that no node
         * is processed for re-transmission for any LC for PSR processing. */
        UInt8 lcCtr = 0;
        while ( lcCtr < ueCtx_p->numOfActiveDrbLc)
        {
            LP_PdcpEnityContext ctx_p = 
                PDCP_GET_VALID_ENTITY_CONTEXT( 
                        ueCtx_p, ueCtx_p->activeLcInfo[lcCtr], 
                        PDCP_ENTITY_RB_TX );
            ctx_p->storageCount = 0;
            lcCtr++;
        }
        /* SPR 10721 fix end */
    }
    /* SPR +- 17777 */
    LOG_PDCP_MSG( PDCP_RX_EVENT_RCVD, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
            __LINE__, ueIndex, state,event, lcId,0,0, __func__,
            "PdcpRxStateMachine");
    /* SPR +- 17777 */

    return PDCP_SUCCESS;
}
/* SPR 3608 changes end */

/****************************************************************************
 * Function Name  : pdcpGetDrbCountMsb
 * Inputs         : ueIndex - . 
 *                  lcId - .
 *                  rbDirection - .
 *                  count_p - out parameter contains 25bit count value
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It get the COUNT MSB from the Entities in any direction.
 ****************************************************************************/
UInt16 pdcpGetDrbCountMsb( UInt16 ueIndex, UInt16 lcId, UInt16 rbDirection,
    UInt32 *count_p)
{
    UInt16 result = PDCP_SUCCESS;
    UInt16 sn = 0;
    UInt32 hfn = 0;
    LP_PdcpEnityContext  entity_p = PNULL;

    if ( PDCP_ENTITY_RB_BOTH == rbDirection )
    {
        LOG_PDCP_MSG (PDCP_INVALID_DIR, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, rbDirection, ueIndex, lcId, 0,
                0,0, __func__,"");
        return PDCP_ERR_ENTITY_WRONG_DIRECTION;
    }

    /* get entity context */
    entity_p = pdcpGetEntityContext( ueIndex, lcId, rbDirection );
    if ( PNULL == entity_p )
    {
        LOG_PDCP_MSG( PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, 0,0,0,0,0,0,__func__,"");
        result = PDCP_ERR_ENTITY_NOT_FOUND;
    } else {
        /* For SRB COUNT = 27 bit of HFN and 5 bit of SN */
        /* For DRB-UM COUNT = 25 bit of HFN and 7 bit of SN */
        /* For DRB-AM COUNT = 20 bit of HFN and 12 bit of SN */
        sn = entity_p->next_SN;
        hfn = entity_p->hfn;
        if (0 != sn)
        {
            sn -= 1;
        }
        else if ((0 == sn) && (0 != hfn))
        {
            sn = ((1<<(entity_p->snSize))-1);
            hfn -= 1;
        }
        *count_p = PDCP_MAKE_COUNT(sn, hfn, entity_p->snSize);
        *count_p >>= 7;
    }
    return result;
}

/****************************************************************************
 * Function Name  : pdcpHandleCongestionNotificationStatus
 * Inputs         : rrmPdcpDrbEcnCeReq_p
 * Outputs        : rrmPdcpDrbEcnCeCnf_p
 * Returns        : PDCP_STATUS --> SUCCESS/FAILURE/
 * Description    : set/reset the congestion notification status to RX/TX entity.
 ****************************************************************************/
UInt16 pdcpHandleCongestionNotificationStatus( 
     RrmPdcpDrbEcnCeReq *rrmPdcpDrbEcnCeReq_p, 
     RrmPdcpDrbEcnCeCnf *rrmPdcpDrbEcnCeCnf_p )
{
    LP_PdcpUeContext ueContext_p = PNULL;
    LP_PdcpEnityContext ctx_p = PNULL;
    UInt8 lcIdIndex = 0;
    UInt16 result = PDCP_SUCCESS;
    UInt8 ctx_exist = 0;
    /* CA Changes start */
    InternalCellIndex cellIndex = 0;
    /* CA Changes end */

    ueContext_p = PDCP_GET_UE_CONTEXT( rrmPdcpDrbEcnCeReq_p->ueIndex );
    if ( PNULL == ueContext_p)
    {
        LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, rrmPdcpDrbEcnCeReq_p->ueIndex, 
                pdcpContext.isInitialized,
                0,0, 0,0, __func__,"");
        return PDCP_RRM_ERR_UE_CONTEXT_NOT_INITIALIZED;
    }

/* CA Changes start */
#ifndef UE_SIM_TESTING
    cellIndex   =  ueContext_p->cellIndex;
#endif    
    if( (pdcpContext.pdcpEcnConfigParam[cellIndex].ecnFeatureStatus 
                == ECN_NOT_APPLIED) ||
        (pdcpContext.pdcpEcnConfigParam[cellIndex].ecnTrigger 
                == ECN_T1_BASED_ON_WATERMARKS) )
    {
        return PDCP_RRM_ERR_RRM_ECN_DETECTION_NOT_ENABLED;
    }
/* CA Changes end */

    for ( lcIdIndex = 0; lcIdIndex < rrmPdcpDrbEcnCeReq_p->numOfLc; lcIdIndex ++)
    {    
        /* congestion notification status is set for corresponding direction ( UL/DL or Both)
         * received in ECN ce request*/ 
        if ((PDCP_ENTITY_RB_TX == rrmPdcpDrbEcnCeReq_p->ceLc[lcIdIndex].direction) ||
                (PDCP_ENTITY_RB_BOTH == rrmPdcpDrbEcnCeReq_p->ceLc[lcIdIndex].direction))
        {
            if (pdcpContext.pdcpEcnConfigParam[cellIndex].\
                    ecnFeatureStatus != ECN_APPLIED_IN_UL)
            {
                ctx_p = pdcpGetEntityContext(rrmPdcpDrbEcnCeReq_p->ueIndex, 
                        rrmPdcpDrbEcnCeReq_p->ceLc[lcIdIndex].lcId, 
                        PDCP_ENTITY_RB_TX);
                if( PNULL != ctx_p )
                {
                    if (rrmPdcpDrbEcnCeReq_p->ceLc[lcIdIndex].congestionStatus)
                    {
                        /* Congestion notification is enable for Trigger 2 RRM detection*/
                        ctx_p->congestionStatus |= ECN_T2_BASED_ON_RRM_DETECTION; 
                        ctx_p->ecnIndtobeSent = LTE_TRUE;
                    }
                    else
                    {   
                        /* Congestion notification is disable for Trigger 2 RRM detection*/
                        ctx_p->congestionStatus &= (~ECN_T2_BASED_ON_RRM_DETECTION); 
                        ctx_p->ecnIndtobeSent = LTE_FALSE;
                    }    
                    rrmPdcpDrbEcnCeCnf_p->ceLc[lcIdIndex].result = PDCP_SUCCESS;
                    /* This flag shall determine whether congestion notification status is 
                     * succesfully applied to all the LC ID's received in ECN ce req or not*/
                    ctx_exist = LTE_TRUE;
                } 
                else
                {
                    rrmPdcpDrbEcnCeCnf_p->ceLc[lcIdIndex].result = PDCP_FAIL;
                    result = PDCP_PARTIAL_SUCCESS;
                }
            } 
            else
            {
                rrmPdcpDrbEcnCeCnf_p->ceLc[lcIdIndex].result = PDCP_FAIL;
                result = PDCP_PARTIAL_SUCCESS;
            }
        }

        if ( (PDCP_ENTITY_RB_RX == rrmPdcpDrbEcnCeReq_p->ceLc[lcIdIndex].direction) ||
                (PDCP_ENTITY_RB_BOTH == rrmPdcpDrbEcnCeReq_p->ceLc[lcIdIndex].direction))
        {
            if (pdcpContext.pdcpEcnConfigParam[cellIndex].\
                    ecnFeatureStatus != ECN_APPLIED_IN_DL)
            {
                ctx_p = pdcpGetEntityContext(rrmPdcpDrbEcnCeReq_p->ueIndex, 
                        rrmPdcpDrbEcnCeReq_p->ceLc[lcIdIndex].lcId, 
                        PDCP_ENTITY_RB_RX);
                if( PNULL != ctx_p )
                {
                    if (rrmPdcpDrbEcnCeReq_p->ceLc[lcIdIndex].congestionStatus)
                    {
                        ctx_p->congestionStatus |= ECN_T2_BASED_ON_RRM_DETECTION; // congestion is enabled
                        ctx_p->ecnIndtobeSent = LTE_TRUE;
                    }
                    else
                    {
                        /* As currently we are supporting Water Mark Enabled ECN for DL Only 
                         * So reseting the congetion status to 0 for UL*/
                        ctx_p->congestionStatus = LTE_FALSE;
                        ctx_p->ecnIndtobeSent = LTE_FALSE;
                    }
                    rrmPdcpDrbEcnCeCnf_p->ceLc[lcIdIndex].result = PDCP_SUCCESS;
                    ctx_exist = LTE_TRUE;
                } 
                else
                {
                    rrmPdcpDrbEcnCeCnf_p->ceLc[lcIdIndex].result = PDCP_FAIL;
                    result = PDCP_PARTIAL_SUCCESS;
                }   
            } 
            else
            {
                rrmPdcpDrbEcnCeCnf_p->ceLc[lcIdIndex].result = PDCP_FAIL;
                result = PDCP_PARTIAL_SUCCESS;
            }   
        }
    }

    if( !ctx_exist) // to check if context exist for any of the lc Id given by RRM 
    {
        result = PDCP_FAIL;
    }
    return result;
}

/****************************************************************************
 * Function Name  : pdcpSetEcnStatus
 * Inputs         : rrmPdcpEcnConfigParam_p - Input ECN parameters from RRM
 *                  cellIndex: cell for which the parameters are received
 * Outputs        :
 * Returns        : None
 * Description    : sets the configration for ECN feature.
 ****************************************************************************/
void pdcpSetEcnStatus(RrmPdcpEcnConfigParamReq *rrmPdcpEcnConfigParam_p, 
        InternalCellIndex cellIndex )
{
    PdcpRrmEcnConfigParam* ecnParams_p = &(pdcpContext.\
                                    pdcpEcnConfigParam[cellIndex]);
    /* + Coverity 32369 */
    ecnParams_p->ecnFeatureStatus  = 
        (EcnFeatureStatus)rrmPdcpEcnConfigParam_p->ecnFeatureStatus ; 
    /* - Coverity 32369 */
 
    if (ECN_NOT_APPLIED != ecnParams_p->ecnFeatureStatus)
    {
        /* + Coverity 32368 */
        ecnParams_p->ecnTrigger = 
            (EcnTriggerInfo)rrmPdcpEcnConfigParam_p->ecnTrigger;
        /* - Coverity 32368 */
        ecnParams_p->lowWaterMarkSduCount = 
            ((rrmPdcpEcnConfigParam_p->lowWaterMark * MAX_PDCP_SDU_QUEUE_SIZE )/ 100);
        ecnParams_p->highWaterMarkSduCount = 
            ((rrmPdcpEcnConfigParam_p->highWaterMark * MAX_PDCP_SDU_QUEUE_SIZE ) / 100);

        if( (ecnParams_p->ecnTrigger != ECN_T2_BASED_ON_RRM_DETECTION) &&
                ((ecnParams_p->ecnFeatureStatus == ECN_APPLIED_IN_DL) ||
                 (ecnParams_p->ecnFeatureStatus == ECN_APPLIED_IN_BOTH_UL_DL)))
        {
            ecnParams_p->dlWaterMarkEnabled = LTE_TRUE;
        }
        else
        {
            ecnParams_p->dlWaterMarkEnabled = LTE_FALSE;
        }
    }
    else
    {
        ecnParams_p->dlWaterMarkEnabled = LTE_FALSE;
    }
} 

/****************************************************************************
 * Function Name  : pdcpGetIPHdrChecksum
 * Inputs         : count, addr_c
 * Outputs        :
 * Returns        : UInt16 
 * Description    : Gets the IP Header CheckSum
 ****************************************************************************/
UInt16 pdcpGetIPHdrChecksum(UInt8 count, UInt8 *addr_c)
{
    UInt16 addr_s;
    UInt32 sum = 0;
    UInt8 indx;

    /* SPR 18122 Changes Start*/
    indx = DIVIDE_BY_TWO(count);
    /* SPR 18122 Changes End*/
    do {
        addr_s = (*addr_c++ << ONE_OCTET);
        addr_s |= (*addr_c++);

        sum += addr_s;
        count -= 2;
    } while(--indx);

    /*  Fold 32-bit sum to 16 bits */
    while (sum >> TWO_OCTETS)
        sum = (sum & 0xffff) + (sum >> TWO_OCTETS);

    return ~sum;
}

/* + SPR 19066 */
/****************************************************************************
 * Function Name  : pdcpGetInactiveTimeVal
 * Inputs         : ueIndex
 * Outputs        : inactTimeVal_p - Inactive time value for the UE
 * Returns        : Success/Failure
 * Description    : It computes the inactive time value of the UE.
 ****************************************************************************/
tickType_t pdcpGetInactiveTimeVal( UInt16 ueIndex, tickType_t *inactTimeVal_p )
{
    LP_PdcpUeContext ueCtx_p = PNULL;
    tickType_t inactTimeVal   = 0;
    tickType_t inactTimeValDl = 0;
    tickType_t inactTimeValUl = 0;

    /* Get the UE context */
    ueCtx_p = PDCP_GET_UE_CONTEXT( ueIndex );
    if( PNULL == ueCtx_p )
    {
        LOG_PDCP_WARNING(PDCP_RRCOAM,
                "PDCP_UE context is null[%u]",ueIndex);
                return PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
    }


    /* calculate inactivity time value for DL */
    if( ueCtx_p->lastSchedTickDl < pdcpCurrentTime_g )
    {
        inactTimeValDl = ueCtx_p->lapsedInactiveTime + 
            ( pdcpCurrentTime_g - ueCtx_p->lastSchedTickDl);
    }
    else
    {
        /* Wrap around condition */
        inactTimeValDl = ueCtx_p->lapsedInactiveTime + 
            ( pdcpCurrentTime_g + 
              (MAX_UINT64_VALUE - ueCtx_p->lastSchedTickDl) + 1 );
    }
    /* calculate inactivity time value for DL */
    if( ueCtx_p->lastSchedTickUl < pdcpCurrentTime_g )
    {
        inactTimeValUl = ueCtx_p->lapsedInactiveTime + 
            ( pdcpCurrentTime_g - ueCtx_p->lastSchedTickUl );
    }
    else
    {
        /* Wrap around condition */
        inactTimeValUl = ueCtx_p->lapsedInactiveTime +
            ( pdcpCurrentTime_g + 
              (MAX_UINT64_VALUE - ueCtx_p->lastSchedTickUl) + 1 );
    }


    /* Find mimimum inactivity time value in ms */
    inactTimeVal = (inactTimeValDl < inactTimeValUl) ? 
        inactTimeValDl : inactTimeValUl;

    /* Convert the inactive time in ms to hrs/days format and get
     * corresponding enum value */
    *inactTimeVal_p = pdcpConvertTime( inactTimeVal );

    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpConvertTime 
 * Inputs         : inactiveTime
 * Output         : None
 * Returns        : UInt32
 * Description    : It converts the time in miliseconds into day, hour, min,
 *                  sec and returns the enum value.  
 *****************************************************************************/
tickType_t pdcpConvertTime(UInt32 inactiveTime)
{
    tickType_t retVal = UE_INACTIVITY_TIME_S1;

    PdcpUeInactiveTimeMs inactiveTimeArray[MAX_UE_INACTIVITY_TIME] = {
        UE_INACTIVITY_TIME_S1_MS,
        UE_INACTIVITY_TIME_S2_MS,
        UE_INACTIVITY_TIME_S3_MS,
        UE_INACTIVITY_TIME_S5_MS,
        UE_INACTIVITY_TIME_S7_MS,
        UE_INACTIVITY_TIME_S10_MS,
        UE_INACTIVITY_TIME_S15_MS,
        UE_INACTIVITY_TIME_S20_MS,
        UE_INACTIVITY_TIME_S25_MS,
        UE_INACTIVITY_TIME_S30_MS,
        UE_INACTIVITY_TIME_S40_MS,
        UE_INACTIVITY_TIME_S50_MS,
        UE_INACTIVITY_TIME_MIN1_MS,
        UE_INACTIVITY_TIME_MIN1S20_MS,
        UE_INACTIVITY_TIME_MIN1S40_MS,
        UE_INACTIVITY_TIME_MIN2_MS,
        UE_INACTIVITY_TIME_MIN2S30_MS,
        UE_INACTIVITY_TIME_MIN3_MS,
        UE_INACTIVITY_TIME_MIN3S30_MS,
        UE_INACTIVITY_TIME_MIN4_MS,
        UE_INACTIVITY_TIME_MIN5_MS,
        UE_INACTIVITY_TIME_MIN6_MS,
        UE_INACTIVITY_TIME_MIN7_MS,
        UE_INACTIVITY_TIME_MIN8_MS,
        UE_INACTIVITY_TIME_MIN9_MS,
        UE_INACTIVITY_TIME_MIN10_MS,
        UE_INACTIVITY_TIME_MIN12_MS,
        UE_INACTIVITY_TIME_MIN14_MS,
        UE_INACTIVITY_TIME_MIN17_MS,
        UE_INACTIVITY_TIME_MIN20_MS,
        UE_INACTIVITY_TIME_MIN24_MS,
        UE_INACTIVITY_TIME_MIN28_MS,
        UE_INACTIVITY_TIME_MIN33_MS,
        UE_INACTIVITY_TIME_MIN38_MS,
        UE_INACTIVITY_TIME_MIN44_MS,
        UE_INACTIVITY_TIME_MIN50_MS,
        UE_INACTIVITY_TIME_HR1_MS,
        UE_INACTIVITY_TIME_HR1MIN30_MS,
        UE_INACTIVITY_TIME_HR2_MS,
        UE_INACTIVITY_TIME_HR2MIN30_MS,
        UE_INACTIVITY_TIME_HR3_MS,
        UE_INACTIVITY_TIME_HR3MIN30_MS,
        UE_INACTIVITY_TIME_HR4_MS,
        UE_INACTIVITY_TIME_HR5_MS,
        UE_INACTIVITY_TIME_HR6_MS,
        UE_INACTIVITY_TIME_HR8_MS,
        UE_INACTIVITY_TIME_HR10_MS,
        UE_INACTIVITY_TIME_HR13_MS,
        UE_INACTIVITY_TIME_HR16_MS,
        UE_INACTIVITY_TIME_HR20_MS,
        UE_INACTIVITY_TIME_DAY1_MS,
        UE_INACTIVITY_TIME_DAY1HR12_MS,
        UE_INACTIVITY_TIME_DAY2_MS,
        UE_INACTIVITY_TIME_DAY2HR12_MS,
        UE_INACTIVITY_TIME_DAY3_MS,
        UE_INACTIVITY_TIME_DAY4_MS,
        UE_INACTIVITY_TIME_DAY5_MS,
        UE_INACTIVITY_TIME_DAY7_MS,
        UE_INACTIVITY_TIME_DAY10_MS,
        UE_INACTIVITY_TIME_DAY14_MS,
        UE_INACTIVITY_TIME_DAY19_MS,
        UE_INACTIVITY_TIME_DAY24_MS,
        UE_INACTIVITY_TIME_DAY30_MS,
        UE_INACTIVITY_TIME_DAYMORETHAN30_MS };

    UInt8 Ub =(UInt8)( MAX_UE_INACTIVITY_TIME-1);
    UInt8 Lb = 0;
    UInt8 Mid = (Ub+Lb)/2;
    inactiveTime = inactiveTime/ONE_THOUSAND;
    if( 0 == inactiveTime)
    {
        return UE_INACTIVITY_TIME_S1;
    }
    if( UE_INACTIVITY_TIME_DAYMORETHAN30_MS <= inactiveTime )
    {
        return UE_INACTIVITY_TIME_DAYMORETHAN30;
    }
    while (Lb < Ub)
    {
        if (inactiveTime > inactiveTimeArray[Mid])
        {
            Lb = Mid +1 ;
        }
        else if(inactiveTime < inactiveTimeArray[Mid])
        {
            Ub = Mid -1;
        }
        else if (inactiveTime == inactiveTimeArray[Mid])
        {
            return Mid;
        }
        Mid = (Lb+Ub)/2;
    }
    if ((inactiveTime < inactiveTimeArray[Mid +1]) &&
            (inactiveTime>inactiveTimeArray[Mid]))
    {
        if ((inactiveTimeArray[Mid+1]-inactiveTime)>(inactiveTime -
                    inactiveTimeArray[Mid]))
        {
            retVal = Mid;
        }
        else
        {
            retVal =  (Mid+1);
        }
    }
    else if ((inactiveTime > inactiveTimeArray[Mid -1])&& 
            (inactiveTime <inactiveTimeArray[Mid]))
    {
        if ((inactiveTimeArray[Mid]- inactiveTime)>  
                (inactiveTime- inactiveTimeArray[Mid-1]))
        {
            retVal = (Mid -1);
        }
        else
        {
            retVal = Mid;
        }
    }
    return retVal;
}

/****************************************************************************
 * Function Name  : pdcpConvertTimetoMs
 * Inputs         : inactiveTimeVal
 * Output         : None
 * Returns        : Time in ms
 * Description    : It converts the time to miliseconds.
 *****************************************************************************/
tickType_t pdcpConvertTimetoMs(tickType_t inactiveTimeVal)
{
    PdcpUeInactiveTimeMs inactiveTimeArray[MAX_UE_INACTIVITY_TIME] = {
        UE_INACTIVITY_TIME_S1_MS,
        UE_INACTIVITY_TIME_S2_MS,
        UE_INACTIVITY_TIME_S3_MS,
        UE_INACTIVITY_TIME_S5_MS,
        UE_INACTIVITY_TIME_S7_MS,
        UE_INACTIVITY_TIME_S10_MS,
        UE_INACTIVITY_TIME_S15_MS,
        UE_INACTIVITY_TIME_S20_MS,
        UE_INACTIVITY_TIME_S25_MS,
        UE_INACTIVITY_TIME_S30_MS,
        UE_INACTIVITY_TIME_S40_MS,
        UE_INACTIVITY_TIME_S50_MS,
        UE_INACTIVITY_TIME_MIN1_MS,
        UE_INACTIVITY_TIME_MIN1S20_MS,
        UE_INACTIVITY_TIME_MIN1S40_MS,
        UE_INACTIVITY_TIME_MIN2_MS,
        UE_INACTIVITY_TIME_MIN2S30_MS,
        UE_INACTIVITY_TIME_MIN3_MS,
        UE_INACTIVITY_TIME_MIN3S30_MS,
        UE_INACTIVITY_TIME_MIN4_MS,
        UE_INACTIVITY_TIME_MIN5_MS,
        UE_INACTIVITY_TIME_MIN6_MS,
        UE_INACTIVITY_TIME_MIN7_MS,
        UE_INACTIVITY_TIME_MIN8_MS,
        UE_INACTIVITY_TIME_MIN9_MS,
        UE_INACTIVITY_TIME_MIN10_MS,
        UE_INACTIVITY_TIME_MIN12_MS,
        UE_INACTIVITY_TIME_MIN14_MS,
        UE_INACTIVITY_TIME_MIN17_MS,
        UE_INACTIVITY_TIME_MIN20_MS,
        UE_INACTIVITY_TIME_MIN24_MS,
        UE_INACTIVITY_TIME_MIN28_MS,
        UE_INACTIVITY_TIME_MIN33_MS,
        UE_INACTIVITY_TIME_MIN38_MS,
        UE_INACTIVITY_TIME_MIN44_MS,
        UE_INACTIVITY_TIME_MIN50_MS,
        UE_INACTIVITY_TIME_HR1_MS,
        UE_INACTIVITY_TIME_HR1MIN30_MS,
        UE_INACTIVITY_TIME_HR2_MS,
        UE_INACTIVITY_TIME_HR2MIN30_MS,
        UE_INACTIVITY_TIME_HR3_MS,
        UE_INACTIVITY_TIME_HR3MIN30_MS,
        UE_INACTIVITY_TIME_HR4_MS,
        UE_INACTIVITY_TIME_HR5_MS,
        UE_INACTIVITY_TIME_HR6_MS,
        UE_INACTIVITY_TIME_HR8_MS,
        UE_INACTIVITY_TIME_HR10_MS,
        UE_INACTIVITY_TIME_HR13_MS,
        UE_INACTIVITY_TIME_HR16_MS,
        UE_INACTIVITY_TIME_HR20_MS,
        UE_INACTIVITY_TIME_DAY1_MS,
        UE_INACTIVITY_TIME_DAY1HR12_MS,
        UE_INACTIVITY_TIME_DAY2_MS,
        UE_INACTIVITY_TIME_DAY2HR12_MS,
        UE_INACTIVITY_TIME_DAY3_MS,
        UE_INACTIVITY_TIME_DAY4_MS,
        UE_INACTIVITY_TIME_DAY5_MS,
        UE_INACTIVITY_TIME_DAY7_MS,
        UE_INACTIVITY_TIME_DAY10_MS,
        UE_INACTIVITY_TIME_DAY14_MS,
        UE_INACTIVITY_TIME_DAY19_MS,
        UE_INACTIVITY_TIME_DAY24_MS,
        UE_INACTIVITY_TIME_DAY30_MS,
        UE_INACTIVITY_TIME_DAYMORETHAN30_MS };

    return( inactiveTimeArray[inactiveTimeVal] );
}
/* - SPR 19066 */
