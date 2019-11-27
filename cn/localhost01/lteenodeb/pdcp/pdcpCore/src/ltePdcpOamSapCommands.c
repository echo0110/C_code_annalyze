/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpOamSapCommands.c,v 1.1.1.1.16.2 2010/10/25 08:33:42 gur21010 Exp $
 *
 ******************************************************************************
 *
 *  File Description : OAM API message handlers implementation
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpOamSapCommands.c,v $
 * Revision 1.1.1.1.16.2  2010/10/25 08:33:42  gur21010
 * Warnings Removed
 *
 * Revision 1.1.1.1.16.1  2010/10/07 13:22:58  gur23054
 * Fix for SPR 713
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.17.2.3  2009/07/13 11:04:05  gur20435
 *
 * PDCP Optimizations
 *
 * Revision 1.17.2.2  2009/06/19 14:28:20  gur19836
 * Changes Done for Execution Scheduler
 *
 * Revision 1.17.2.1  2009/06/09 11:33:51  gur19140
 * *** empty log message ***
 *
 * Revision 1.17  2009/05/28 15:28:41  gur20548
 * stats done
 *
 * Revision 1.16  2009/05/28 13:48:17  gur19140
 * *** empty log message ***
 *
 * Revision 1.15  2009/05/27 04:42:00  gur19479
 * Modified for freeing Rohc memory pools
 *
 * Revision 1.14  2009/05/26 15:01:20  gur19140
 * *** empty log message ***
 *
 * Revision 1.13  2009/05/25 11:49:39  gur19836
 * esStart bug fixed
 *
 * Revision 1.12  2009/05/22 06:04:24  gur20548
 * bugs fixed
 *
 * Revision 1.9  2009/05/20 14:17:09  gur20548
 * name change
 *
 * Revision 1.8  2009/05/20 04:13:32  gur19836
 * fixed the get stats issue
 *
 * Revision 1.7  2009/05/15 18:43:24  gur19140
 * fixes
 *
 * Revision 1.6  2009/05/15 10:34:23  gur19836
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
#include "lteConfigFileHdlr.h"
#include "lteLayer2CommanTypes.h"
#include "ltePdcpOamSap.h"
#include "ltePdcpTlvUtils.h"
#include "ltePdcpLteDefs.h"
#include "ltePdcpCmnTools.h"
#include "ltePdcpTxQueues.h"
#include "ltePdcpRxQueues.h"
#include "ltePdcpContext.h"
#include "ltePdcpErrors.h"
#include "ltePdcpBuildInfo.h"
#include "ltePdcpDataTransferTx.h"
#include "ltePdcpStatistic.h"
#include "ltePdcpExtScheduler.h"
#include "ltePdcpRRMUtils.h"
#include "ltePdcpRRMInterface.h"
#include "logging.h"
#include "alarm.h"
#include "lteOamInterface.h"
#include "rohc_g_typ.h"
#include "confidentiality.h"
#ifdef PDCP_ASYNC_INTERFACE
#include "ltePdcpEncSap.h"
#endif
#include "ltePdcpOamSapCommands.h"
#ifdef PDCP_GTPU_INTF
#include "ltePdcpPdcpUserSap.h"
#endif


/******************************************************************************
  Private Definitions
 *****************************************************************************/
#define PDCP_TX_MEMORY_RESERVATION_COEF    100
#define PDCP_RX_MEMORY_RESERVATION_COEF    100
#define PDCP_BUILD_INFO_FILE        "pdcpBuildInfo"
#define FALSE 0
#define TRUE 1

#ifdef PDCP_ASYNC_INTERFACE
#define NO_OF_TX_CIPH_OUT_BUFFERS 10000
#define NO_OF_RX_CIPH_OUT_BUFFERS 10000

MSGPOOL txOutMsgPool_g;
MSGPOOL rxOutMsgPool_g;
#endif

/* SPR 3607 Changes Start */
#define MAX_THRESHOLD_VALUE_FOR_WRAPAROUND 0xFFFFFFA0
/* SPR 3607 Changes End */
/*SPR 20870 +*/
/*SPR 21320 +*/
#define NUM_UE_STATS_PROCESS  30
/*SPR 21320 -*/
/*SPR 20870 -*/
/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
STATIC void oamCmdInitLayer( UInt8* data_p, UInt16 size );
STATIC void oamCmdCleanupLayer( UInt8* data_p, UInt16 size );
STATIC void oamCmdGetBuildInfo( UInt8* data_p, UInt16 size );
STATIC void oamCmdResetStats( UInt8* data_p, UInt16 size );
STATIC void oamCmdGetStats( UInt8* data_p, UInt16 size );
STATIC void oamCmdGetStatus( UInt8* data_p, UInt16 size );
#ifndef UE_SIM_TESTING
STATIC void oamCmdSetLogLevel( UInt8* data_p, UInt16 size );
STATIC void oamCmdGetLogLevel( UInt8* data_p, UInt16 size );
STATIC void oamCmdEnableLogCategory( UInt8* data_p, UInt16 size );
STATIC void oamCmdDisableLogCategory( UInt8* data_p, UInt16 size );
STATIC void oamCmdGetLogCategory( UInt8* data_p, UInt16 size );
STATIC void oamCmdHealthMonitorStatusReq( UInt8* data_p, UInt16 size );
/*spr 22474 start*/
STATIC void oamCmdGetDebugInfoReq( UInt8* data_p, UInt16 size );
/*spr 22474 end*/
STATIC UInt16 pdcpValidateIntervalList(UInt8 intList[]);
STATIC UInt32 pdcpValidateSpsProfile( LP_PdcpSpsIntervalProfile intProfile_p);
STATIC inline UInt16 pdcpValidateInitSpsParams( LP_PdcpOamInitLayerReq initParams_p );
#ifdef KPI_STATS
STATIC void oamCmdConfigureKpiStatsReq( UInt8* data_p, UInt16 size );
STATIC void oamCmdGetKpiStatsReq( UInt8* data_p, UInt16 size );
#endif
#endif

STATIC UInt16 createPDCPMemPools( UInt16 maxUeNumber );

STATIC inline void pdcpGetSrbStatus( LP_PdcpUeContext ueContext_p, 
        PdcpStatus *pdcpUeStatus_p);
STATIC inline void pdcpGetDrbStatus( LP_PdcpUeContext ueContext_p, 
        PdcpStatus *pdcpUeStatus_p);
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* Layer2---->OAM Init Ind changes */
UInt8 pdcpInitInd_g;
/* SPR 5449 changes start */
#ifdef PERF_STATS
UInt8 ueResponseBuf_g[40000];
#endif
/* SPR 5449 changes end */
/* SPR 15909 fix start */
extern tickType_t pdcpCurrentTime_g;
/* SPR 15909 fix end */

extern PdcpStats pdcpStats;
extern PdcpContext pdcpContext;
#ifndef UE_SIM_TESTING
/* returns port number for given moduleId */
extern UInt16 GetModulePortNum(Int moduleId);
/* returns host name (string) for given moduleId */
extern Char8 *GetModuleHostName(Int moduleId);
#ifdef KPI_STATS
extern SInt32 pdcpOamMsgEventFd_g;
extern void pdcpGetKpiStats(LP_PdcpKpiStatsIndParams cnfBuff, 
         UInt8 kpiBitmap, LtePdcpKPIStats *pLtePdcpKPIStats);
#endif
UInt32 pdcpHealthMonitoringTimeInterval_g;
/*SPR 21366 +*/
#if defined PDCP_GTPU_INTF
extern void sendGtpuRegisterPdcpInd(UInt8 *cellIdList_p, UInt8 cellCount);
#endif
/*SPR 21366 -*/
#endif
extern UInt8 *pdcpFpProcessSrbRxBuf_p;
extern UInt8 *pdcpFpProcessDrbRxBuf_p;
extern UInt8 *pdcpProcessSrbTxMacI_p;
extern UInt8 *integrityCheckMACIbaseptr_p;
extern UInt8* ipHdrWoOptBytes_p;

#ifdef PDCP_ASYNC_INTERFACE
extern UInt8 *pdcpAsyncTxHeader_p;
extern UInt8 *pdcpAsyncRxHeader_p;
#endif
/*SPR 21683 +-*/
TlvCommandTable oamCmdTable_g[] =
{
    { { PDCP_INIT_LAYER_REQ           },      oamCmdInitLayer,         },
    { { PDCP_CLEANUP_LAYER_REQ        },      oamCmdCleanupLayer,      },
    { { PDCP_GET_BUILD_INFO_REQ       },      oamCmdGetBuildInfo,      }, 
    { { PDCP_RESET_STATS_REQ          },      oamCmdResetStats,        },
    { { PDCP_GET_STATS_REQ            },      oamCmdGetStats,          },
    { { PDCP_GET_STATUS_REQ           },      oamCmdGetStatus,         },
#ifndef UE_SIM_TESTING
    { { PDCP_SET_LOG_LEVEL_REQ        },      oamCmdSetLogLevel,       },
    { { PDCP_GET_LOG_LEVEL_REQ        },      oamCmdGetLogLevel,       },
    { { PDCP_ENABLE_LOG_CATEGORY_REQ  },      oamCmdEnableLogCategory, },
    { { PDCP_DISABLE_LOG_CATEGORY_REQ },      oamCmdDisableLogCategory,},
    { { PDCP_GET_LOG_CATEGORY_REQ     },      oamCmdGetLogCategory,    },
#ifdef KPI_STATS
    { { PDCP_CONFIGURE_KPI_STATS_REQ  },      oamCmdConfigureKpiStatsReq, },
    { { PDCP_GET_KPI_STATS_REQ        },      oamCmdGetKpiStatsReq,    },
#endif
#ifdef PERF_STATS
    { {PDCP_CONFIGURE_PERF_STATS_REQ  },     oamCmdConfigurePerfStatsReq, },
    { {PDCP_GET_PERF_STATS_REQ        },     oamCmdGetPerfstatsReq,    },
#endif
    { { PDCP_PROC_SUP_REQ             },     oamCmdHealthMonitorStatusReq, },
	/*spr 22474 start*/
    { { OAM_PDCP_GET_DEBUG_INFO_REQ    },     oamCmdGetDebugInfoReq, },
    /*spr 22474 end*/
#endif
    { { TLV_LAST_ID                   },     PNULL,                   },
};

/* Start 128 UE: Memory Changes */
UInt16 pdcpNumOfSupportedUsers_g = 0;
UInt8  pdcpDlMemFac_g            = 0;
UInt8  pdcpUlMemFac_g            = 0;
/* End 128 UE: Memory Changes */

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
/* SPR 11939 fix start */
/* rohcFreeFromPool() is already being called from pdcpContextCleanup(), hence removed */
/* SPR 11939 fix end */
void cleanupPDCP(void);


/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : oamCmdInitLayer 
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size       
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes OAM PDCP_INIT_LAYER_REQ API.
 *                  It does all layer initializtion routines.
 *                  As response It sends PDCP_INIT_LAYER_CNF message to OAM.
 ****************************************************************************/
void oamCmdInitLayer( UInt8* data_p, UInt16 size )
{
    UInt8           *responseBuf;   /* Memory buffer for response */
    LP_TlvMessage	message_p = (LP_TlvMessage)data_p;
    LP_PdcpOamInitLayerReq	initParams_p = PNULL;
    LP_TlvMessage   responseMsg_p = PNULL;
    LP_PdcpOamCnf	initResponse_p = PNULL;
    /* + Layer2 NON CA Changes */
    UInt32          maxSupportedUE = 0; 
    UInt32          logLevel = 0;
    /*SPR 21366 +*/
#if defined PDCP_GTPU_INTF
    UInt8 cellIdList[MAX_NUM_CELL] = {0};
#endif
    /*SPR 21366 -*/

#ifndef UE_SIM_TESTING

    InternalCellIndex    cellCount = 0;
    RrcCellIndex         cellId = message_p->header.cellIndex;
    InternalCellIndex    cellIndex = PDCP_GET_INTERNAL_CELL_INDEX( cellId );

    /* coverity fix 60264 starts */
    if( INVALID_CELL_INDEX == cellIndex )
    {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, (sizeof(TlvHeader) + \
                sizeof(PdcpOamCnf) ), 0,0,0,0,0, __func__,
                "PDCP_INVALID_CELL_INDEX" );
        return ;
    }
    /* coverity fix 60264 ends */
    /*SPR 21366 +*/
#if defined PDCP_GTPU_INTF
    cellIdList[cellCount] = message_p->header.cellIndex;
#endif
    /*SPR 21366 -*/
#endif
    /* - Layer2 NON CA Changes */

    /*Check API id */
    CHECK_EXP( PDCP_INIT_LAYER_REQ != message_p->header.apiId );

    responseBuf = (UInt8 *)getMemFromPool( (
                        sizeof(TlvHeader) + sizeof(PdcpOamCnf) ), PNULL);
    if ( PNULL == responseBuf ) 
    {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, (sizeof(TlvHeader) + \
                sizeof(PdcpOamCnf) ), 0,0,0,0,0, __func__,
                "PDCP_INIT_LAYER_REQ response" );
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        return;
    }
    /* SPR 11109 Fix start */
    memSet (responseBuf, 0, (
                        sizeof(TlvHeader) + sizeof(PdcpOamCnf)));
    /* SPR 11109 Fix end */
    responseMsg_p = (LP_TlvMessage)&responseBuf[0];

    /* + Layer2 NON CA Changes */
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            message_p->header.transactionId,
            OAM_MODULE_ID,
            PDCP_INIT_LAYER_CNF,
            sizeof(PdcpOamCnf)
#ifndef UE_SIM_TESTING
            ,cellId
#endif
            );
    /* - Layer2 NON CA Changes */

    /*Prepare pointer to response message data */
    initResponse_p = (LP_PdcpOamCnf)responseMsg_p->data;

    /* Set default values in response message */
    initResponse_p->responseCode = PDCP_SUCCESS;
    do {
        /* --- check message length */
        /* + Layer2 NON CA Changes */
        if ( message_p->header.length != size )
            /* - Layer2 NON CA Changes */
        {   /* invalid length -- reject it */
	    LOG_PDCP_MSG( PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
		 pdcpCurrentTime_g, __LINE__, message_p->header.length,
                 PDCP_OAM_INIT_LAYER_REQ_MIN_SIZE, 0,0, 0,0,
                 __func__,"PDCP_INIT_LAYER_REQ");

            initResponse_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }

        /*Check data size */
        /*LTE_REG_FIX*/
        if ( message_p->header.length < PDCP_OAM_INIT_LAYER_REQ_MIN_SIZE )
        /*LTE_REG_FIX*/
        {
	    LOG_PDCP_MSG( PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
		 pdcpCurrentTime_g, __LINE__, message_p->header.length,
                 PDCP_OAM_INIT_LAYER_REQ_MIN_SIZE, 0,0, 0,0,
                 __func__, "PDCP_INIT_LAYER_REQ");
            initResponse_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }

        /*Prepare pointer to message data */
        initParams_p = (LP_PdcpOamInitLayerReq)message_p->data;

        /* + Layer2 NON CA Changes */
        maxSupportedUE = initParams_p->maxUeNumber;         

        logLevel = initParams_p->logLevel;
        l2_map_ext_loglevel_to_intern_loglevel((UInt8 *)&logLevel);
    /* SPR 20899 Changes Start*/
        pdcploglevelEnabled_g = MULTIPLY_BY_TWO(logLevel)-1;
    /* SPR 20899 Changes End*/

#ifdef UE_SIM_TESTING
        /*Check params */
        if ( (MAX_UE_SUPPORTED < maxSupportedUE) || 
                (0 == maxSupportedUE) )
        {/*CSR 40799 chg end*/
            initResponse_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_UE_ID;
            LOG_PDCP_MSG( PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, initResponse_p->responseCode,
                    maxSupportedUE,0,0, 0,0, __func__, "PDCP_INIT_LAYER_REQ" );
            break;
        }
#else
        /* coverity Fix 59924 starts */
        if( MAX_NUM_CELL <= initParams_p->sCellCount )
        {
            /* coverity Fix 59924 starts */
            initResponse_p->responseCode = 
                PDCP_ERR_TLV_PARSING_INVALID_UE_ID;
            LOG_PDCP_MSG( PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__,
                    initResponse_p->responseCode,
                    0,0,0, 0,0, __func__, "PDCP_INIT_LAYER_REQ" );

            break; 
        }

        for( ; cellCount < (initParams_p->sCellCount); cellCount++)
        {
            maxSupportedUE += initParams_p->sCellInfo[cellCount].maxUeNumber;
        }

        if ((MAX_UE_SUPPORTED < maxSupportedUE) || 
                (0 == maxSupportedUE))
        {
            initResponse_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_UE_ID;

            LOG_PDCP_MSG( PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, initResponse_p->responseCode,
                    maxSupportedUE,0,0, 0,0, __func__, "PDCP_INIT_LAYER_REQ" );
            break;
        }
        /* - Layer2 NON CA Changes */
#endif
        pdcpNumOfSupportedUsers_g = 
            ( maxSupportedUE * LTE_CREATE_MEMPOOL_EXTRA_BUF_FACTOR );

            LTE_DL_MULTIPLY_FACTOR(pdcpNumOfSupportedUsers_g, pdcpDlMemFac_g);
            LTE_UL_MULTIPLY_FACTOR(pdcpNumOfSupportedUsers_g, pdcpUlMemFac_g);

#ifndef UE_SIM_TESTING
        /* + Layer2 NON CA Changes */

        /* SPR 3607 Changes Start */
        if ( initParams_p->bitMask & 0x01)
        {
            pdcpContext.sCellInfo[cellIndex].thresholdValue = 
                (UInt32)initParams_p->thresholdValue;
        }
        else
        {
            pdcpContext.sCellInfo[cellIndex].thresholdValue = 
                MAX_THRESHOLD_VALUE_FOR_WRAPAROUND;
        }

        if (initParams_p->sCellCount)
        {
            for ( cellCount = 0 ;cellCount < initParams_p->sCellCount;
                    cellCount++)
            {   
                cellIndex = PDCP_GET_INTERNAL_CELL_INDEX(initParams_p->\
                        sCellInfo[cellCount].cellId);

                /* coverity fix 60264 starts */
                if( INVALID_CELL_INDEX == cellIndex )
                {
                    initResponse_p->responseCode = 
                        PDCP_ERR_TLV_PARSING_INVALID_CELL_INDEX ;

                    LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM,
                            pdcpCurrentTime_g, __LINE__, (sizeof(TlvHeader) + \
                                sizeof(PdcpOamCnf) ), 0,0,0,0,0, __func__,
                            "PDCP_INVALID_CELL_INDEX" );
                    break ;
                }
                /* coverity fix 60264 ends */
                /*SPR 21366 +*/
#if defined PDCP_GTPU_INTF
                if( (MAX_NUM_CELL-1) > cellCount)
                {
                    cellIdList[cellCount+1] = initParams_p->sCellInfo[cellCount].cellId;
                }
#endif
                /*SPR 21366 -*/

                if ( initParams_p->sCellInfo[cellCount].bitMask & 0x01)
                {
                    pdcpContext.sCellInfo[cellIndex].thresholdValue = 
                        (UInt32)initParams_p->sCellInfo[cellCount].thresholdValue;
                }
                else
                {
                    pdcpContext.sCellInfo[cellIndex].thresholdValue =
                        MAX_THRESHOLD_VALUE_FOR_WRAPAROUND;
                }
            }
        }
        /* SPR 3607 Changes End */

        /* Cyclomatic Complexity changes - starts here */
        initResponse_p->responseCode = pdcpValidateInitSpsParams(initParams_p);

        if ( PDCP_SUCCESS != initResponse_p->responseCode )
        {
            LOG_PDCP_MSG( PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, initResponse_p->responseCode,
                    maxSupportedUE,0,0, 0,0, __func__, "PDCP_INIT_LAYER_REQ" );

            break;
        }
        /* - Layer2 NON CA Changes */
        /* Cyclomatic Complexity changes - ends here */

        pdcpHealthMonitoringTimeInterval_g = (initParams_p->healthMonitoringTimeInterval * 1000);
        if(0 == pdcpHealthMonitoringTimeInterval_g)
        {
             LOG_PDCP_MSG(HSM_OAM_PDCP_INIT_FAILURE, LOGFATAL, L2_SYS_FAIL,
                pdcpCurrentTime_g,pdcpHealthMonitoringTimeInterval_g,
                DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                __func__,"PDCP OAM Health Status Init Failure");
             break;
        }
#endif

        /****** Do initialization */
        /* Init memory pools */
        if ( !pdcpContextIsInitialized() )
        {
            /* Start 128 UE: Memory Changes */
            initResponse_p->responseCode = createPDCPMemPools( 
                                           pdcpNumOfSupportedUsers_g );
            /* End 128 UE: Memory Changes */
            if ( PDCP_SUCCESS != initResponse_p->responseCode )
            {
                LOG_PDCP_MSG(PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, 
                        pdcpCurrentTime_g, __LINE__, 
                        initResponse_p->responseCode, 0,0,0, 0,0, __func__,
                        "PDCP_INIT_LAYER_REQ" );
                break;
            }
        }

        /*Init PDCP context */
        initResponse_p->responseCode = pdcpContextInit(initParams_p
#ifndef UE_SIM_TESTING
                ,cellId 
#endif
                );

        if ( PDCP_SUCCESS != initResponse_p->responseCode )
        {
            LOG_PDCP_MSG(PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__, initResponse_p->responseCode, 0,0,0, 0,0, 
                    __func__, "PDCP_INIT_LAYER_REQ" );
            break;
        }
        pdcpContext.maxUeNumber = maxSupportedUE ;

#ifndef UE_SIM_TESTING
        /* +- SPR 17777 */
        alarm_init (OAM_MODULE_ID,
                    GetModuleHostName(OAM_MODULE_ID),
                    GetModulePortNum(OAM_MODULE_ID)
                    );
        /* +- SPR 17777 */
        initPdcpRRMIpcInterface();
#endif

        /*Init TX queues */
        txQueuesInit();

        /*Init RX queues */
        rxQueuesInit();

#ifdef PDCP_ASYNC_INTERFACE
#ifdef PDCP_ASYNC_INTR

        initResponse_p->responseCode = pdcpSecInit (INTERRUPT_MODE);
#else
        initResponse_p->responseCode = pdcpSecInit (POLLING_MODE);
#endif
        if ( PDCP_SUCCESS != initResponse_p->responseCode )
        {
            LOG_PDCP_MSG( PDCP_SEC_INIT_FAILS, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, initResponse_p->responseCode,
                    0,0,0, 0,0, __func__, "PDCP_INIT_LAYER_REQ" );
            break;
        }
#endif
        /* SPR 14909 fix start */
        /* SPR 14909 fix end */
    } while (0);

    if ( PDCP_SUCCESS != initResponse_p->responseCode )
    {
        LOG_PDCP_MSG(PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, initResponse_p->responseCode, 0,0,0,
                0,0, __func__, "PDCP_INIT_LAYER_REQ" );
    }

    /*Convert Header fields to network byte order*/
    tlvPrepareHeaderForSending(responseMsg_p);

    /* Init Indication Changes */
    if( FALSE == pdcpInitInd_g)
    {
        pdcpInitInd_g = TRUE;
    }

    /*send response */
    pdcpOamSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    freeMemPool ( responseBuf );

#if defined PDCP_GTPU_INTF
    /*send GTP_REGISTER_PDCP_IND to GTP-U process*/
    /*SPR 21366 +*/
    sendGtpuRegisterPdcpInd(&cellIdList[0],initParams_p->sCellCount);
    /*SPR 21366 -*/
#endif

/* + Layer2 NON CA Changes */
}

#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : pdcpValidateInitSpsParams
 * Inputs         : initParams_p - Pointer to PDCP Oam Init Layer Req
 * Outputs        : None.
 * Returns        : Error code/ PDCP_SUCCESS
 * Description    : This function validates SPS parameters in Init request.
 ****************************************************************************/
STATIC inline UInt16 pdcpValidateInitSpsParams( 
        LP_PdcpOamInitLayerReq initParams_p )
{
    UInt8  idx          = 0;
    UInt16 responseCode = PDCP_SUCCESS;
    /* + Layer2 NON CA Changes */
    InternalCellIndex  cellCount = 0;
    /* + Layer2 NON CA Changes */

    do
    {
        if( LTE_TRUE < initParams_p->enableSps )
        {
            responseCode = PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM;
            LOG_PDCP_MSG( PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, responseCode,
                    0,0,0, 0,0, __func__, "PDCP_INIT_LAYER_REQ" );
            break;
        }
       
        if( LTE_TRUE == initParams_p->enableSps )
        {
        /* Validate QCI to SPS list */
        for(idx = 0; idx < MAX_QCI; idx++)
        {
            if ( LTE_TRUE < initParams_p->qciSpslist[idx] )
            {
                responseCode = PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM;
                LOG_PDCP_MSG( PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, 
                        pdcpCurrentTime_g, __LINE__, 
                        responseCode, 0,0,0, 0,0, __func__,
                        "PDCP_INIT_LAYER_REQ" );
                break;
            }
        }
        if( PDCP_FAIL == pdcpValidateIntervalList(
                    initParams_p->supportedSpsDlIntervalList) )
        {
            responseCode = PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM;
            LOG_PDCP_MSG( PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, responseCode,
                    0,0,0, 0,0, __func__, "PDCP_INIT_LAYER_REQ" );
            break;
        }
        if( PDCP_FAIL == pdcpValidateIntervalList(
                    initParams_p->supportedSpsUlIntervalList) )
        {
            responseCode = PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM;
            LOG_PDCP_MSG( PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, responseCode,
                    0,0,0, 0,0, __func__, "PDCP_INIT_LAYER_REQ" );
            break;
        }
        if( PDCP_FAIL == pdcpValidateSpsProfile(
                    initParams_p->spsProfileParams) )
        {
            responseCode = PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM;
            LOG_PDCP_MSG( PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, responseCode,
                    0,0,0, 0,0, __func__, "PDCP_INIT_LAYER_REQ" );
            break;
        }
        }
    }while (0);

    if(PDCP_SUCCESS != responseCode)
        return responseCode;
    
    if(initParams_p->sCellCount)
    {
        for( ;cellCount < initParams_p->sCellCount; cellCount++)
        {
            do
            {
                if( LTE_TRUE < initParams_p->sCellInfo[cellCount].\
                                            enableSps )
                {
                    responseCode = PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM;
                    LOG_PDCP_MSG( PDCP_OAM_MESSAGE_FAIL, LOGWARNING,
                             PDCP_RRCOAM, pdcpCurrentTime_g, __LINE__,
                             responseCode,
                            0,0,0, 0,0, __func__, "PDCP_INIT_LAYER_REQ" );
                    break;
                }
                
                if( LTE_TRUE == initParams_p->sCellInfo[cellCount].\
                                            enableSps )
                {
                    /* Validate QCI to SPS list */
                    for(idx = 0; idx < MAX_QCI; idx++)
                    {
                        if ( LTE_TRUE < initParams_p->sCellInfo[cellCount].\
                                        qciSpslist[idx] )
                        {
                            responseCode = 
                                        PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM;
                            LOG_PDCP_MSG( PDCP_OAM_MESSAGE_FAIL, LOGWARNING,
                                     PDCP_RRCOAM, 
                                     pdcpCurrentTime_g, __LINE__, 
                                     responseCode, 0,0,0, 0,0, __func__,
                                     "PDCP_INIT_LAYER_REQ" );
                            break;
                        }   
                    }
                    if( PDCP_FAIL == pdcpValidateIntervalList(
                                        initParams_p->sCellInfo[cellCount].supportedSpsDlIntervalList) )
                    {
                        responseCode = PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM;
                        LOG_PDCP_MSG( PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM,
                                pdcpCurrentTime_g, __LINE__, responseCode,
                                0,0,0, 0,0, __func__, "PDCP_INIT_LAYER_REQ" );
                        break;
                    }
                    if( PDCP_FAIL == pdcpValidateIntervalList(
                                initParams_p->sCellInfo[cellCount].supportedSpsUlIntervalList) )
                    {
                        responseCode = PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM;
                        LOG_PDCP_MSG( PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM,
                                pdcpCurrentTime_g, __LINE__, responseCode,
                                0,0,0, 0,0, __func__, "PDCP_INIT_LAYER_REQ" );
                        break;
                    }
                    if( PDCP_FAIL == pdcpValidateSpsProfile(
                                initParams_p->sCellInfo[cellCount].spsProfileParams) )
                    {
                        responseCode = PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM;
                        LOG_PDCP_MSG( PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM,
                                pdcpCurrentTime_g, __LINE__, responseCode,
                                0,0,0, 0,0, __func__, "PDCP_INIT_LAYER_REQ" );
                        break;
                    }
                }
            }while (0);
        }
    }
    return responseCode;
}
#endif
/* - Layer2 NON CA Changes */

/****************************************************************************
 * Function Name  : oamCmdCleanupLayer 
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size       
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes OAM PDCP_CLEANUP_LAYER_REQ API.
 *                  It does all layer deinitializtion routines.
 *                  As response It sends PDCP_CLEANUP_LAYER_CNF message to OAM.
 ****************************************************************************/
void oamCmdCleanupLayer( UInt8* data_p, UInt16 size )
{
    UInt8           responseBuf[PDCP_OAM_CNF_MAX_SIZE_SMALL];   /* Memory buffer for response */
    LP_TlvMessage   message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    LP_PdcpOamCnf       cleanupResponse_p = PNULL;

    /*Check API id */
    CHECK_EXP( PDCP_CLEANUP_LAYER_REQ != message_p->header.apiId );

    /* + Layer2 NON CA Changes */
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            message_p->header.transactionId,
            OAM_MODULE_ID,
            PDCP_CLEANUP_LAYER_CNF,
            sizeof(PdcpOamCnf)
#ifndef UE_SIM_TESTING
            ,message_p->header.cellIndex
#endif
            );
    /* - Layer2 NON CA Changes */

    /*Prepare pointer to response message data */
    cleanupResponse_p = (LP_PdcpOamCnf)responseMsg_p->data;

    /* Set default values in response message */
    cleanupResponse_p->responseCode = PDCP_SUCCESS;

    do {
        /* --- check message length */
        if ( message_p->header.length != size )
        {   /* invalid length -- reject it */
            cleanupResponse_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            LOG_PDCP_MSG(PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__, cleanupResponse_p->responseCode, 0,0,0, 0,0, 
                    __func__, "PDCP_CLEANUP_LAYER_REQ" );
            break;
        }

        if ( !pdcpContextIsInitialized() )
        {   /* PDCP isn't initialized */
            LOG_PDCP_MSG (PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__,0,0,0,0, 0,0,__func__,"" );
            cleanupResponse_p->responseCode = PDCP_ERR_CONTEXT_NOT_INITIALIZED;
            break;
        }
        
       /* This code is commented as this API is not supported in 6.0 */
        /*Cleanup PDCP context */
       /*  cleanupResponse_p->responseCode = pdcpContextCleanup();
        if ( PDCP_SUCCESS != cleanupResponse_p->responseCode)
        {
            LOG_PDCP_MSG(PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__, cleanupResponse_p->responseCode, 0,0,0, 0,0, 
                    __func__, "PDCP_CLEANUP_LAYER_REQ" );
            break;
        }

        cleanupPDCP();
       */
        
        break;
    } while (0);

    /* Returning failure as this API is not supported in Rel 6.0 */
    cleanupResponse_p->responseCode = PDCP_FAIL; 

    if ( PDCP_SUCCESS != cleanupResponse_p->responseCode )
    {
        LOG_PDCP_MSG(PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
               __LINE__, cleanupResponse_p->responseCode, 0,0,0, 0,0, 
               __func__, "PDCP_CLEANUP_LAYER_REQ" );
    }

    /*Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK( cleanupResponse_p->responseCode );

    tlvPrepareHeaderForSending(responseMsg_p);

    /*send response */
    pdcpOamSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
}

/****************************************************************************
 * Function Name  : cleanupPDCP 
 * Inputs         : None
 * Outputs        : None.
 * Returns        : None.
 * Description    : It cleansup all memory allocated in queues and timers.
 ****************************************************************************/
void cleanupPDCP(void)
{
    /*Cleanup TX queues */
    txQueuesCleanup();

    /*Cleanup RX queues */
    rxQueuesCleanup();

    /* SPR 11939 fix start */
    /* rohcFreeFromPool() is already being called from pdcpContextCleanup(), hence removed */
    /* SPR 11939 fix end */
    /* Cleanup the global PDCP pointers used for CSPL operations */
    freeMemPool( pdcpFpProcessSrbRxBuf_p );
    freeMemPool( pdcpFpProcessDrbRxBuf_p );
    freeMemPool( pdcpProcessSrbTxMacI_p );
    freeMemPool( integrityCheckMACIbaseptr_p );
    freeMemPool( ipHdrWoOptBytes_p );
#ifdef PDCP_ASYNC_INTERFACE
    freeMemPool( pdcpAsyncTxHeader_p );
    freeMemPool( pdcpAsyncRxHeader_p );
    /* SPR 15236 fix start */
    freeAllMemory( MAX_UE_SUPPORTED, 0, 0 ); 
    /* SPR 15236 fix end */
#endif
}

/****************************************************************************
 * Function Name  : oamCmdGetBuildInfo 
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size       
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes OAM PDCP_GET_BUILD_INFO_REQ API.
 *                  It returns information about current build version.
 *                  As response It sends PDCP_GET_BUILD_INFO_CNF message to OAM.
 ****************************************************************************/
STATIC void oamCmdGetBuildInfo( UInt8* data_p, UInt16 size )
{
    UInt8           responseBuf[PDCP_OAM_CNF_MAX_SIZE_SMALL];   /* Memory buffer for response */
    LP_TlvMessage   message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage   responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    Char8*         buildInfoResponse_p = PNULL;
    UInt16          buildInfoSize = 0;

    /*Check API id */
    CHECK_EXP( PDCP_GET_BUILD_INFO_REQ != message_p->header.apiId );

    /* + Layer2 NON CA Changes */
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            message_p->header.transactionId,
            OAM_MODULE_ID,
            PDCP_GET_BUILD_INFO_CNF,0 
#ifndef UE_SIM_TESTING
            ,message_p->header.cellIndex
#endif
            );

    /* - Layer2 NON CA Changes */

    /*Prepare pointer to response message data */
    buildInfoResponse_p = (Char8*)responseMsg_p->data;

    do {
        /* --- check message length */
        if ( message_p->header.length != size )
        {   /* invalid length -- reject it */
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, message_p->header.length, 
                    size, 0,0, 0,0, __func__,"PDCP_GET_BUILD_INFO_REQ");
            break;
        }

        if ( !pdcpContextIsInitialized() )
        {   /* PDCP isn't initialized */
            LOG_PDCP_MSG (PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__,0,0,0,0, 0,0,__func__,"" );
            break;
        }

        /****** Get buld information */

        buildInfoSize = (UInt16)(PDCP_OAM_CNF_MAX_SIZE_SMALL - sizeof(TlvHeader));    
        buildInfoSize = getBuildInfo( buildInfoResponse_p,buildInfoSize);

        /* update massage length */
        responseMsg_p->header.length += buildInfoSize;

    } while (0);

    /*Convert response fields to network byte order */
    tlvPrepareHeaderForSending(responseMsg_p);

    /*send response */
    pdcpOamSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
}

/****************************************************************************
 * Function Name  : oamCmdResetStats 
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size       
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes OAM PDCP_RESET_STATS_REQ API.
 *                  It resets PDCP statistic data.
 *                  As response It sends PDCP_RESET_STATS_CNF message to OAM.
 ****************************************************************************/
void oamCmdResetStats( UInt8* data_p, UInt16 size )
{
    UInt8           *responseBuf = PNULL;   /* Memory buffer for response */
    LP_TlvMessage   message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage   responseMsg_p = PNULL;
    LP_PdcpOamCnf   resetStatsResponse_p = PNULL;

    /*Check API id */
    CHECK_EXP( PDCP_RESET_STATS_REQ != message_p->header.apiId );

    responseBuf = (UInt8 *)getMemFromPool( (
            sizeof(TlvHeader) + sizeof(PdcpOamCnf) ), PNULL);
    if ( PNULL == responseBuf ) 
    {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, (sizeof(TlvHeader) + \
                 sizeof(PdcpOamCnf) ),size,0,0,0,0, __func__, 
                 "PDCP_RESET_STATS_REQ response");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic ("getMemPool error in %s", __func__);
        return;
    }
    responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    /* + Layer2 NON CA Changes */
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            message_p->header.transactionId,
            OAM_MODULE_ID,
            PDCP_RESET_STATS_CNF,
            sizeof(PdcpOamCnf)
#ifndef UE_SIM_TESTING
            ,message_p->header.cellIndex
#endif
            );
    /* - Layer2 NON CA Changes */

    /*Prepare pointer to response message data */
    resetStatsResponse_p = (LP_PdcpOamCnf)responseMsg_p->data;

    /* Set default values in response message */
    resetStatsResponse_p->responseCode = PDCP_SUCCESS;

    do {
        if ( !pdcpContextIsInitialized() )
        {   /* PDCP isn't initialized */
	    LOG_PDCP_MSG( PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM, 
		 pdcpCurrentTime_g, __LINE__,0,0,0,0, 0,0,__func__,"");
            resetStatsResponse_p->responseCode = PDCP_ERR_CONTEXT_NOT_INITIALIZED;
            break;
        }

        /****** Reset PDCP statistic */
        statsReset();

    } while (0);

    /*Convert header fields to network byte order */
    tlvPrepareHeaderForSending(responseMsg_p);

    /*send response */
    pdcpOamSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    freeMemPool ( responseBuf );
}

/****************************************************************************
 * Function Name  : oamCmdGetStats 
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size       
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes OAM PDCP_GET_STATS_REQ API.
 *                  It gets PDCP statistic data.
 *                  As response It sends PDCP_GET_STATS_CNF message to OAM.
 ****************************************************************************/
void oamCmdGetStats( UInt8* data_p, UInt16 size )
{
    UInt8           *responseBuf ;   /* Memory buffer for response */
    LP_TlvMessage   message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage   responseMsg_p = PNULL;
    LP_PdcpStatsCnf pdcpStatsParams_p = PNULL;
    StatsUe         *pdcpStatsUe_p = PNULL;
    LP_PdcpUeContext    ueContext_p = PNULL;
    UInt16          bufSize = 0;

    /*SPR 20870 +*/
    UInt16          remaintingUEs = 0;
    static UInt16   startUeId = 0;
    UInt8           NoOfBatches = 0;
    UInt8           processPdpcStats=LTE_TRUE;
    if(pdcpStats.statNumActiveUe)
    {
        remaintingUEs  = (pdcpStats.statNumActiveUe%NUM_UE_STATS_PROCESS);
        NoOfBatches =  (pdcpStats.statNumActiveUe/NUM_UE_STATS_PROCESS);
    }
    while(processPdpcStats)
    {
        /*SPR 21320 +*/
        if ( 0 < pdcpStats.statNumActiveUe  && (0 == NoOfBatches))
            /*SPR 21320 -*/
        {
            bufSize = ( sizeof(PdcpStatsCnf) + \
                    (remaintingUEs - 1) * sizeof(StatsUe) );
            processPdpcStats = 0;
        }
        else if (NoOfBatches)
        {
            bufSize = ( sizeof(PdcpStatsCnf) + \
                    (NUM_UE_STATS_PROCESS - 1) * sizeof(StatsUe) );
        }
        else
        {
            bufSize = sizeof(PdcpStatsCnf);
        }
        responseBuf = (UInt8 *)getMemFromPool(sizeof(TlvHeader) + bufSize, PNULL);
        if ( PNULL == responseBuf ) 
        {
            /* +- SPR 17777 */
            LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, sizeof(TlvHeader) + bufSize,size,0,
                    0,0,0, __func__,"PDCP_GET_STATS_REQ response");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            ltePanic ("getMemPool error in %s", __func__);
            return;
        }
        responseMsg_p = (LP_TlvMessage)&responseBuf[0];
        /* + Layer2 NON CA Changes */
        /* Prepare response buffer */
        tlvPrepareMessage( responseMsg_p,
                message_p->header.transactionId,
                OAM_MODULE_ID,
                PDCP_GET_STATS_RESP,
                bufSize
#ifndef UE_SIM_TESTING
                ,message_p->header.cellIndex
#endif
                );
        /* - Layer2 NON CA Changes */

        /*Prepare pointer to response message data */
        pdcpStatsParams_p = (LP_PdcpStatsCnf)responseMsg_p->data;

        /* Set default values in response message */
        pdcpStatsParams_p->responseCode = PDCP_SUCCESS;
        /*SPR 21320 +*/
        pdcpStatsParams_p->UesLeft = (0 == NoOfBatches)?LTE_FALSE:LTE_TRUE;
        /*SPR 21320 -*/

        do {
            if ( !pdcpContextIsInitialized() )
            {   /* PDCP isn't initialized */
                LOG_PDCP_MSG( PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM, 
                        pdcpCurrentTime_g, __LINE__,0,0,0,0, 0,0,__func__,"");
                pdcpStatsParams_p->responseCode = PDCP_ERR_CONTEXT_NOT_INITIALIZED;
                break;
            }

            /****** Get PDCP statistic ******/
            /* Append Integrity Stats */
            memCpy (&(pdcpStatsParams_p->statsIntegrity), 
                    &(pdcpStats.statsIntegrity), sizeof(PdcpIntegrityStats) );

            /* Append Ciphering Stats */
            memCpy (&(pdcpStatsParams_p->statsCiphering), 
                    &(pdcpStats.statsCiphering), sizeof(PdcpCipheringStats) );

            /* Append ROHC Stats */
            memCpy (&(pdcpStatsParams_p->statsRohc),
                    &(pdcpStats.statsRohc), sizeof(PdcpRohcStats) );

            /* Append UE Stats */
            pdcpStatsParams_p->statNumActiveUe = pdcpStats.statNumActiveUe;
            UInt16 index = 0, indCtr = 0;
            pdcpStatsParams_p->startUeId = startUeId;
            pdcpStatsParams_p->numOfUe = (NoOfBatches && (pdcpStats.statNumActiveUe > NUM_UE_STATS_PROCESS))? NUM_UE_STATS_PROCESS : remaintingUEs ;
            while (index < pdcpStatsParams_p->numOfUe)
            {
                ueContext_p = PDCP_GET_UE_CONTEXT( startUeId );
                if ( PNULL != ueContext_p )
                {
                    /*Prepare pointer to UE Stats data */
                    pdcpStatsUe_p = 
                        (StatsUe *)&(pdcpStatsParams_p->statsUEs_p[index] );
                    pdcpStatsUe_p->crnti = pdcpStats.statsUEs[startUeId].crnti;
                    pdcpStatsUe_p->statNumActiveSrbLc = 
                        pdcpStats.statsUEs[startUeId].statNumActiveSrbLc;
                    pdcpStatsUe_p->statNumActiveDrbLc = 
                        pdcpStats.statsUEs[startUeId].statNumActiveDrbLc;
                    for (indCtr = 0; indCtr < LTE_SRB_LC_ID_HIGH_BOUND; indCtr++)
                    {
                        memCpy (&(pdcpStatsUe_p->srbs[indCtr]), 
                                &(pdcpStats.statsUEs[startUeId].srbs[indCtr]),
                                sizeof(StatsSrbData));
                    }
                    for (indCtr = 0; indCtr < LTE_DRB_LC_ID_HIGH_BOUND -2; indCtr++)
                    {
                        memCpy (&(pdcpStatsUe_p->drbs[indCtr]), 
                                &(pdcpStats.statsUEs[startUeId].drbs[indCtr]), 
                                sizeof(StatsDrbData) );
                    }
                    index++;
                }
                startUeId++;
            }
            NoOfBatches--;

        } while (0);
        if(startUeId == pdcpStats.statNumActiveUe)
        {
            pdcpStatsParams_p->UesLeft = LTE_FALSE;
            startUeId = 0;
            /*SPR 21320 +*/
            processPdpcStats = 0;
            /*SPR 21320 -*/
        }
        /*Convert header fields to network byte order */
        tlvPrepareHeaderForSending(responseMsg_p);
        /*send response */
        pdcpOamSendMessage((UInt8*)responseMsg_p, 
                PDCP_NTOHS(responseMsg_p->header.length) 
                );
        freeMemPool ( responseBuf );
    }
    /*SPR 20870 +*/
}

/****************************************************************************
 * Function Name  : oamCmdGetStatus 
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size       
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes OAM PDCP_GET_STATUS_REQ API.
 *                  It gets PDCP layer status data.
 *                  As response It sends PDCP_GET_STATUS_CNF message to OAM.
 ****************************************************************************/
void oamCmdGetStatus( UInt8* data_p, UInt16 size )
{
#ifndef UE_SIM_TESTING
    UInt8               *responseBuf;   /* Memory buffer for response */
    LP_TlvMessage       responseMsg_p = PNULL;
    LP_PdcpUeStatusCnf  getStatusResponse_p = PNULL;
    LP_TlvMessage       message_p = (LP_TlvMessage)data_p;
    LP_PdcpStatusReq 	getStatusParams_p = PNULL;
    PdcpStatus          *pdcpUeStatus_p = PNULL;
    LP_PdcpUeContext    ueContext_p = PNULL;
    UInt16              bufSize = 0;
    UInt16              responseCode = PDCP_SUCCESS;
    UInt16              numActiveUE = 0;
    UInt16              ueIndex = 0;
    UInt16              rrcUeIdx = 0;
    UInt16              index = 0;

    InternalCellIndex cellIndex = PDCP_GET_INTERNAL_CELL_INDEX(message_p->header.cellIndex);

    do 
    {
        if ( !pdcpContextIsInitialized() )
        {   /* PDCP isn't initialized */
            /* +- SPR 17777 */
            LOG_PDCP_MSG( PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__,size,0,0,0,
                    0,0, __func__, "" );
            responseCode = PDCP_ERR_CONTEXT_NOT_INITIALIZED;
            break;
        }

        /****** Get PDCP layer status data */
        /*Prepare pointer to message data */
        getStatusParams_p = (LP_PdcpStatusReq)message_p->data;

        /* check range */
        if (getStatusParams_p->ueStartRange > getStatusParams_p->ueEndRange)
        {
            responseCode = PDCP_ERR_TLV_PARSING_INVALID_RNTI_RANGE;
            LOG_PDCP_MSG(PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__, responseCode, 0,0,0, 0,0, __func__, "" );
            break;
        }
        /* + stats_review_comments_ipc */
        for (rrcUeIdx = getStatusParams_p->ueStartRange; 
                rrcUeIdx <= getStatusParams_p->ueEndRange; rrcUeIdx++)
        {
            ueIndex = getUeIdxFromRrcUeIdx(rrcUeIdx);
            ueContext_p = PDCP_GET_UE_CONTEXT( ueIndex );
            if ((PNULL != ueContext_p ) &&
                /* if the UE belongs to requested cell */
                (cellIndex == ueContext_p->cellIndex))
            {
                numActiveUE++;
            }
        }
    }while (0);

    if ( 1 < numActiveUE )
    {
        bufSize = (sizeof(PdcpUeStatusCnf) + (numActiveUE - 1) * sizeof(PdcpStatus));
    }
    else
    {
        bufSize = sizeof(PdcpUeStatusCnf);
    }

    responseBuf = (UInt8 *)getMemFromPool (sizeof(TlvHeader) + bufSize, PNULL);
    if ( PNULL == responseBuf ) 
    {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, sizeof(TlvHeader) + bufSize, 0,0,
                0,0,0, __func__,"PDCP_GET_STATUS_REQ response");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic ("getMemPool error in %s", __func__);
        return;
    }
    memSet (responseBuf, 0, sizeof(TlvHeader) + bufSize);
    responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    /* + Layer2 NON CA Changes */
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            message_p->header.transactionId,
            OAM_MODULE_ID,
            PDCP_GET_STATUS_RESP,
            bufSize, message_p->header.cellIndex );
    /* - Layer2 NON CA Changes */

    /*Prepare pointer to response message data */
    getStatusResponse_p = (LP_PdcpUeStatusCnf)responseMsg_p->data;

    /* Set default values in response message */
    getStatusResponse_p->responseCode = responseCode;

    do 
    {
        if ( PDCP_SUCCESS != responseCode )
        {
            break;
        }
        /* get status */
        if (0 == numActiveUE )
        {
            getStatusResponse_p->responseCode = 
                PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
            LOG_PDCP_MSG(PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__, getStatusResponse_p->responseCode, 0,0,0, 
                    0,0, __func__, "" );
            break;
        }

        /* + stats_review_comments_ipc */
        for ( rrcUeIdx = getStatusParams_p->ueStartRange; 
                rrcUeIdx <= getStatusParams_p->ueEndRange; rrcUeIdx++)
        {
            ueIndex = getUeIdxFromRrcUeIdx(rrcUeIdx); 
            /* + stats_review_comments_ipc */
            ueContext_p = PDCP_GET_UE_CONTEXT( ueIndex );
            if ((PNULL != ueContext_p ) &&
                    /* if the UE belongs to requested cell */
                    (cellIndex == ueContext_p->cellIndex))
            {
                /*Prepare pointer to UE Status data */
                pdcpUeStatus_p = (PdcpStatus *)&(getStatusResponse_p->data_p[index]);

                /* + stats_review_comments_ipc */
                pdcpUeStatus_p->ueIndex = rrcUeIdx;
                /* - stats_review_comments_ipc */
                pdcpUeStatus_p->crnti = ueContext_p->crnti;

                /* Cyclomatic Complexity changes - starts here */
                pdcpGetSrbStatus (ueContext_p, pdcpUeStatus_p);
                pdcpGetDrbStatus (ueContext_p, pdcpUeStatus_p);
                /* Cyclomatic Complexity changes - ends here */
                index++;
            }
        }
    } while (0);

    /* Update the number of UE */
    getStatusResponse_p->numActiveUE = index;

    /*Convert header fields to network byte order */
    tlvPrepareHeaderForSending(responseMsg_p);

    /*send response */
    pdcpOamSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    freeMemPool ( responseBuf );
#endif
}

/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : pdcpGetSrbStatus 
 * Inputs         : ueContext_p    - Pointer to UE context
 *                  pdcpUeStatus_p - Pointer to PDCP UE status        
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function is to get the PDCP SRB status data.
 ****************************************************************************/
STATIC inline void pdcpGetSrbStatus( LP_PdcpUeContext ueContext_p, 
        PdcpStatus *pdcpUeStatus_p)
{
    UInt8   lcId = 0;
    pdcpUeStatus_p->numActiveSrbEntities = 0;

    /* Beginning of SRB status */
    for ( lcId = LTE_SRB_LC_ID_LOW_BOUND; lcId <= LTE_SRB_LC_ID_HIGH_BOUND; lcId++ )
    {
        /* SPR 3189 changes start */
        pdcpUeStatus_p->srbs[lcId - 1].srbId = lcId;
        /* SPR 3189 changes end */
        if ((PNULL != ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_TX]) ||
                (PNULL != ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_RX]))
        {
            pdcpUeStatus_p->numActiveSrbEntities++;
        }
        if (PNULL != ueContext_p->pdcpEntities[lcId - 1][PDCP_ENTITY_RB_TX])
        {
            pdcpUeStatus_p->integrityAlgoUsed = 
                ueContext_p->pdcpEntities[lcId - 1][PDCP_ENTITY_RB_TX]->
                integrity.algorithmId;
            pdcpUeStatus_p->cipheringSrbAlgoUsed = 
                ueContext_p->pdcpEntities[lcId - 1][PDCP_ENTITY_RB_TX]->
                ciphering.algorithmId;
            pdcpUeStatus_p->srbs[lcId - 1].direction = PDCP_ENTITY_RB_TX;
            pdcpUeStatus_p->srbs[lcId - 1].rlcMode = 
                ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_TX]->
                rlcMode;
        }
        /* SPR 3189 changes start */
        else
        {
            pdcpUeStatus_p->srbs[lcId - 1].srbId = 0;
        }
        /* SPR 3189 changes end */
        if (PNULL != ueContext_p->pdcpEntities[lcId - 1][PDCP_ENTITY_RB_RX])
        {
            pdcpUeStatus_p->integrityAlgoUsed = 
                ueContext_p->pdcpEntities[lcId - 1][PDCP_ENTITY_RB_RX]->
                integrity.algorithmId;
            pdcpUeStatus_p->cipheringSrbAlgoUsed = 
                ueContext_p->pdcpEntities[lcId - 1][PDCP_ENTITY_RB_RX]->
                ciphering.algorithmId;
            pdcpUeStatus_p->srbs[lcId - 1].direction = PDCP_ENTITY_RB_RX;
            pdcpUeStatus_p->srbs[lcId - 1].rlcMode = 
                ueContext_p->pdcpEntities[lcId-1][PDCP_ENTITY_RB_RX]->
                rlcMode;
        }
        /* SPR 3189 changes start */
        else
        {
            pdcpUeStatus_p->srbs[lcId - 1].srbId = 0;
        }
        /* SPR 3189 changes end */
        if ((PNULL != ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_TX]) &&
                (PNULL != ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_RX]) )
        {
            pdcpUeStatus_p->srbs[lcId - 1].direction = PDCP_ENTITY_RB_BOTH;
        }
    } /* end of for loop for SRB */
}

/****************************************************************************
 * Function Name  : pdcpGetDrbStatus 
 * Inputs         : ueContext_p    - Pointer to UE context
 *                  pdcpUeStatus_p - Pointer to PDCP UE status        
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function is to get the PDCP DRB status data.
 ****************************************************************************/
STATIC inline void pdcpGetDrbStatus( LP_PdcpUeContext ueContext_p, 
        PdcpStatus    *pdcpUeStatus_p)
{
    UInt8   lcId = 0;
    pdcpUeStatus_p->numActiveDrbEntities = 0;

    /* Beginning of DRB status */
    for ( lcId = LTE_DRB_LC_ID_LOW_BOUND; lcId <= LTE_DRB_LC_ID_HIGH_BOUND; lcId++ )
    {
        /* SPR 3189 changes start */
        pdcpUeStatus_p->drbs[lcId - 3].lcId = lcId;
        /* SPR 3189 changes end */
        if ((PNULL != ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_TX]) ||
                (PNULL != ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_RX]))
        {
            pdcpUeStatus_p->numActiveDrbEntities++;
        }
        if ( PNULL != ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_TX] )
        {
            pdcpUeStatus_p->cipheringDrbAlgoUsed = 
                ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_TX]->
                ciphering.algorithmId;
            pdcpUeStatus_p->drbs[lcId - 3].rlcMode = 
                ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_TX]->
                rlcMode;
            pdcpUeStatus_p->drbs[lcId - 3].direction = PDCP_ENTITY_RB_TX;
            pdcpUeStatus_p->drbs[lcId - 3].discardTimerValue = 
                ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_TX]->
                discardTimer;
            pdcpUeStatus_p->drbs[lcId - 3].statusReprtField = 
                ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_TX]->
                statusReportRequired;
            pdcpUeStatus_p->drbs[lcId - 3].snFieldLength = 
                ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_TX]->
                snSize;
            if (PNULL != (ueContext_p->
                        pdcpEntities[lcId -1][PDCP_ENTITY_RB_TX]->rohc.context) )
            {
                pdcpUeStatus_p->drbs[lcId - 3].rohcProfileUsed =
                    ((rohc_entity_t *)ueContext_p->
                     pdcpEntities[lcId -1][PDCP_ENTITY_RB_TX]->rohc.context)->
                    profilesSupported;
            }
            else
            {
                pdcpUeStatus_p->drbs[lcId - 3].rohcProfileUsed = 0xFFFF;
            }
        }
        /* SPR 3189 changes start */
        else
        {
            pdcpUeStatus_p->drbs[lcId - 3].lcId = 0;
        }
        /* SPR 3189 changes end */
        if ( PNULL != ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_RX] )	           
        {
            pdcpUeStatus_p->cipheringDrbAlgoUsed = 
                ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_RX]->
                ciphering.algorithmId;
            pdcpUeStatus_p->drbs[lcId - 3].rlcMode = 
                ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_RX]->
                rlcMode;
            pdcpUeStatus_p->drbs[lcId - 3].direction = PDCP_ENTITY_RB_RX;
            pdcpUeStatus_p->drbs[lcId - 3].discardTimerValue = 
                ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_RX]->
                discardTimer;
            pdcpUeStatus_p->drbs[lcId - 3].statusReprtField = 
                ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_RX]->
                statusReportRequired;
            pdcpUeStatus_p->drbs[lcId - 3].snFieldLength = 
                ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_RX]->
                snSize;
            if (PNULL != (ueContext_p->
                        pdcpEntities[lcId -1][PDCP_ENTITY_RB_RX]->rohc.context) )
            {
                pdcpUeStatus_p->drbs[lcId - 3].rohcProfileUsed =
                    ((rohc_entity_t *)ueContext_p->
                     pdcpEntities[lcId -1][PDCP_ENTITY_RB_RX]->rohc.context)->
                    profilesSupported;
            }
            else
            {
                pdcpUeStatus_p->drbs[lcId - 3].rohcProfileUsed = 0xFFFF;
            }

        }
        /* SPR 3189 changes start */
        else
        {
            pdcpUeStatus_p->drbs[lcId - 3].lcId = 0;
        }
        /* SPR 3189 changes end */
        if ((PNULL != ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_TX]) &&
                (PNULL != ueContext_p->pdcpEntities[lcId -1][PDCP_ENTITY_RB_RX]))
        {
            pdcpUeStatus_p->drbs[lcId - 3].direction = PDCP_ENTITY_RB_BOTH;
        }
    } /* end of for loop for DRB */
}
/* Cyclomatic Complexity changes - ends here */

/****************************************************************************
 * Function Name  : createPDCPMemPools 
 * Inputs         : maxUeNumber - Max numbar of UEs
 * Outputs        : None.
 * Returns        : UInt16
 * Description    : It reserves memory in pools for PDCP purposes.
 ****************************************************************************/
UInt16 createPDCPMemPools( UInt16 maxUeNumber )
{
    UInt16 result = PDCP_SUCCESS;
    /* Start 128 UE: Memory Changes */
    /* This is being used in pdcpContextCreateMemPool */
    createMemPool(sizeof(IP_HEADER_LENGTH_WO_OPT_BYTES), 1);
    /* End 128 UE: Memory Changes */
    /* +- SPR 17777 */
    result = pdcpContextCreateMemPool( maxUeNumber, 
            PDCP_TX_MEMORY_RESERVATION_COEF);
    if ( PDCP_SUCCESS == result )
    {
        /* 
         ** Removed pdcpStorageCreateMemPool(), because it's 
         ** doing nothing just returning Success.
         */
        /* End 128 UE: Memory Changes */
        /*  +- SPR 17777 */
        txQueuesCreateMemPool(
                );
        /*  +- SPR 17777 */
        rxQueuesCreateMemPool();
#ifdef PDCP_ASYNC_INTERFACE
        txOutMsgPool_g = msgPoolCreate( 0, NO_OF_TX_CIPH_OUT_BUFFERS ); 
        rxOutMsgPool_g = msgPoolCreate( 0, NO_OF_RX_CIPH_OUT_BUFFERS ); 
#endif
    }

    return result;
}

/****************************************************************************
 * Function Name  : oamCmdDefaultHandler
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size       
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes unknown OAM API.
 ****************************************************************************/
void oamCmdDefaultHandler( UInt8* data_p, UInt16 size )
{
    UInt8           *responseBuf;   /* Memory buffer for response */
    LP_TlvMessage   message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage   responseMsg_p = PNULL ;
    LP_PdcpOamCnf   oamResponse_p = PNULL;

    responseBuf = (UInt8 *)getMemFromPool( ( 
                sizeof(TlvHeader) + sizeof(PdcpOamCnf) ), PNULL);
    if ( PNULL == responseBuf ) 
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, sizeof(TlvHeader) + sizeof(PdcpOamCnf),
                 size,0,0, 0,0,__func__,"Default handler response");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic ("getMemPool error in %s", __func__);
        return;
    }
    responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    /* + Layer2 NON CA Changes */
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            message_p->header.transactionId,
            OAM_MODULE_ID,
            message_p->header.apiId,
            sizeof(PdcpOamCnf)
#ifndef UE_SIM_TESTING
            ,message_p->header.cellIndex
#endif
            );
    /* - Layer2 NON CA Changes */

    /*Prepare pointer to response message data */
    oamResponse_p = (LP_PdcpOamCnf)responseMsg_p->data;

    /* Set default values in response message */
    oamResponse_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_API_ID;

    LOG_PDCP_MSG( PDCP_OAM_API_NS, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
             __LINE__, message_p->header.apiId,0,0,0, 0,0,__func__,"");

    /*Convert header fields to network byte order */
    tlvPrepareHeaderForSending(responseMsg_p);

    /*send response */
    pdcpOamSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    freeMemPool ( responseBuf );
}

/****************************************************************************
 * Function Name  : getBuildInfo 
 * Inputs         : buildInfo_p - Pointer to buffer for storing Build information
 *                  maxSize - Maximum size of build info
 * Outputs        : None.
 * Returns        : Size of BuldInfo or 0 on case of error code.
 * Description    : It returns information about current build version 
 from file which name is defined in PDCP_BUILD_INFO_FILE.
 ****************************************************************************/
UInt16 getBuildInfo(Char8* buildInfo_p, UInt16 maxSize)
{
    return getPDCPBuildInfo(buildInfo_p,maxSize,PDCP_BUILD_INFO_FILE);
}

/****************************************************************************
 * Function Name  : pdcpOamProcessMessage 
 * Inputs         : data_p  - pointer to TLV API buffer.
 *                  size  -  TLV API buffer size.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will be used by the PDCP layer application for
 *                  delivering OAM API to the PDCP layer. The OAM API message
 *                  message will be processed immediately.
 ****************************************************************************/
void    pdcpOamProcessMessage( UInt8 * data_p, UInt16 size )
{
    /* process message */
    tlvProcessCommand( data_p, size );
}

#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : oamCmdSetLogLevel
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes OAM PDCP_SET_LOG_REQ API.
 *                  It set for entire L2 logging currently and not just PDCP 
 *                  As response It sends PDCP_SET_LOG_CNF message to OAM.
 ****************************************************************************/
void oamCmdSetLogLevel( UInt8* data_p, UInt16 size )
{
    LP_TlvMessage       message_p = (LP_TlvMessage)data_p;
    LP_PdcpOamSetLogLevelReq   setLogParams_p = PNULL;
    UInt8 loglevel = 0;
    /*Check API id */
    CHECK_EXP( PDCP_SET_LOG_LEVEL_REQ != message_p->header.apiId );

    /*Prepare pointer to message data */
    setLogParams_p = (LP_PdcpOamSetLogLevelReq)message_p->data;
    loglevel = setLogParams_p->logLevel;
    do
    {
        /* Check parameters */
        /* SPR 3444 changes start */
        if ( LOGTTIDEBUG < setLogParams_p->logLevel )
        /* SPR 3444 changes  end */
        {
            /* +- SPR 17777 */
            LOG_PDCP_MSG( PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, 
                 PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE,
                 size,0,0, 0,0, __func__, "" );
            break;
        }
        /* Set the loglevel for entire L2 */
        if ( INVALID_LOG == setLogParams_p->logLevel )
        {
            pdcploglevelEnabled_g = INVALID_LOG;
        }
        else
        {
            pdcploglevelEnabled_g = ( ( 2 * loglevel ) - 1);
        }
    }while (0);
}

/****************************************************************************
 * Function Name  : oamCmdGetLogLevel
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes OAM PDCP_GET_LOG_REQ API.
 *                  It gets entire L2 logging level enabled earlier 
 *                  not for just PDCP.
 *                  As response It sends PDCP_GET_LOG_RESP message to OAM.
 ****************************************************************************/
void oamCmdGetLogLevel( UInt8* data_p, UInt16 size )
{
    UInt8        *responseBuf;   /* Memory buffer for response */
    LP_TlvMessage       message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage       responseMsg_p = PNULL;
    LP_PdcpOamGetLogLevelResp  getLogResponse_p = PNULL;

    /*Check API id */
    CHECK_EXP( PDCP_GET_LOG_LEVEL_REQ != message_p->header.apiId );

    responseBuf = (UInt8 *)getMemFromPool( (
                sizeof(TlvHeader) + sizeof(PdcpOamGetLogLevelResp) ), PNULL);
    if ( PNULL == responseBuf ) 
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, sizeof(TlvHeader) + \
                 sizeof(PdcpOamGetLogLevelResp),size,0,0, 0,0, __func__,
                 "PDCP_GET_LOG_LEVEL_REQ response");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic ("getMemPool error in %s", __func__);
        return;
    }
    responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    /* + Layer2 NON CA Changes */
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            message_p->header.transactionId,
            OAM_MODULE_ID,
            PDCP_GET_LOG_LEVEL_RESP,
            sizeof(PdcpOamGetLogLevelResp)
#ifndef UE_SIM_TESTING
            ,message_p->header.cellIndex
#endif
                );
    /* - Layer2 NON CA Changes */

    /*Prepare pointer to response message data */
    getLogResponse_p = (LP_PdcpOamGetLogLevelResp)responseMsg_p->data;

    /* Fill response with entire L2 logging enabled earlier*/
    getLogResponse_p->logLevel = pdcploglevelEnabled_g;

    /*Convert response fields to network byte order */
    tlvPrepareHeaderForSending(responseMsg_p);

    /*send response */
    pdcpOamSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    freeMemPool ( responseBuf );
}

/****************************************************************************
 * Function Name  : oamCmdEnableLogCategory
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes OAM PDCP_GET_LOG_REQ API.
 *                  It gets L2 logging parameters.
 *                  As response It sends PDCP_GET_LOG_RESP message to OAM.
 ****************************************************************************/
void oamCmdEnableLogCategory( UInt8* data_p, UInt16 size )
{
    LP_TlvMessage       message_p = (LP_TlvMessage)data_p;
    LP_PdcpOamEnableLogCategoryReq  enableLogCategory_p = PNULL;
  LTE_GCC_UNUSED_PARAM(size)

    /*Check API id */
    CHECK_EXP( PDCP_ENABLE_LOG_CATEGORY_REQ != message_p->header.apiId );

    enableLogCategory_p = (LP_PdcpOamEnableLogCategoryReq)message_p->data;

    pdcplogCategoryEnabled_g |= enableLogCategory_p->logCategory;
}

/****************************************************************************
 * Function Name  : oamCmdDisableLogCategory
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes OAM PDCP_GET_LOG_REQ API.
 *                  It gets L2 logging parameters.
 *                  As response It sends PDCP_GET_LOG_RESP message to OAM.
 ****************************************************************************/
void oamCmdDisableLogCategory( UInt8* data_p, UInt16 size )
{
    LP_TlvMessage       message_p = (LP_TlvMessage)data_p;
    LP_PdcpOamDisableLogCategoryReq  disableLogCategory_p = PNULL;
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(size)

    /*Check API id */
    CHECK_EXP( PDCP_DISABLE_LOG_CATEGORY_REQ != message_p->header.apiId );


    disableLogCategory_p = (LP_PdcpOamDisableLogCategoryReq)message_p->data;

    pdcplogCategoryEnabled_g &= ~(disableLogCategory_p->logCategory);
}

/****************************************************************************
 * Function Name  : oamCmdGetLogCategory
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes OAM PDCP_GET_LOG_REQ API.
 *                  It gets L2 logging parameters.
 *                  As response It sends PDCP_GET_LOG_RESP message to OAM.
 ****************************************************************************/
void oamCmdGetLogCategory( UInt8* data_p, UInt16 size )
{
    UInt8        *responseBuf;   /* Memory buffer for response */
    LP_TlvMessage       message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage       responseMsg_p = PNULL;
    LP_PdcpOamGetLogCategoryResp  getLogCatResponse_p = PNULL;

    /*Check API id */
    CHECK_EXP( PDCP_GET_LOG_CATEGORY_REQ != message_p->header.apiId );

    responseBuf = (UInt8 *)getMemFromPool((
                sizeof(TlvHeader) + sizeof(PdcpOamGetLogCategoryResp) ), PNULL);
    if ( PNULL == responseBuf ) 
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, sizeof(TlvHeader) + \
                sizeof(PdcpOamGetLogCategoryResp),size,0,0, 0,0, __func__,
                "PDCP_GET_LOG_CATEGORY_REQ response");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic ("getMemPool error in %s", __func__);
        return;
    }
    responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    /* + Layer2 NON CA Changes */
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            message_p->header.transactionId,
            OAM_MODULE_ID,
            PDCP_GET_LOG_CATEGORY_RESP,
            sizeof(PdcpOamGetLogCategoryResp)
#ifndef UE_SIM_TESTING
            ,message_p->header.cellIndex
#endif
            );
    /* - Layer2 NON CA Changes */

    /*Prepare pointer to response message data */
    getLogCatResponse_p = (LP_PdcpOamGetLogCategoryResp)responseMsg_p->data;

    /****** Get PDCP logging */
    getLogCatResponse_p->logCategory = pdcplogCategoryEnabled_g;

    /*Convert response fields to network byte order */
    tlvPrepareHeaderForSending(responseMsg_p);

    /*send response */
    pdcpOamSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    freeMemPool ( responseBuf );
}

/****************************************************************************
 * Function Name  : pdcpValidateIntervalList
 * Inputs         : intList - interval list provided by OAM 
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : Validates interval List array as provided by OAM
 ****************************************************************************/
STATIC UInt16 pdcpValidateIntervalList(UInt8 intList[])
{
    UInt8  idx    = 0;
    UInt16 retVal = PDCP_SUCCESS;
    /* Compare every index provided by OAM */
    for( idx = 0; idx < SPS_INTERVAL_LIST; idx++)
    {
        /* Compare with interval list should be either enabled or disabled,
         * else return failure */
        if ( LTE_TRUE < intList[idx] )
        {
            retVal = PDCP_FAIL;
            break;
        }
    }
    return retVal;
}

/****************************************************************************
 * Function Name  : pdcpValidateSpsProfile
 * Inputs         : intProfile - interval list provided by OAM for each
 *                               profile 
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : Validates interval List profiles array as provided by OAM.
 ****************************************************************************/
STATIC UInt32 pdcpValidateSpsProfile( PdcpSpsIntervalProfile intProfile[] )
{
    UInt8  idx    = 0;
    UInt16 retVal = PDCP_SUCCESS;
    /* Compare every index provided by OAM */
    for( idx = 0; idx < SPS_INTERVAL_LIST; idx++)
    {
        if( LTE_TRUE < intProfile[idx].enableSilenceDetection)
        {
            retVal = PDCP_FAIL;
            break;
        }
        if ( MIN_INTER_PKT_DELAY_FOR_SILENCE_DETECTION > 
                intProfile[idx].interPktDelayForSilenceDetection || 
             MAX_INTER_PKT_DELAY_FOR_SILENCE_DETECTION < 
                intProfile[idx].interPktDelayForSilenceDetection )
        {
            retVal = PDCP_FAIL;
            break;
        }
    }
    return retVal;
}

#ifdef KPI_STATS
/* SPR 10579 changes start*/
/****************************************************************************
 * Function Name  : updatePdcpKpiContainer
 * Inputs         : passiveIndexForKpiStats, moduleId and Internal cell Id
 * Outputs        : None
 * Returns        : None
 * Description    : This function will update KPI Container for OAM / RRM.
 ****************************************************************************/
void updatePdcpKpiContainer
(
    UInt32 passiveIndexForKpiStats, 
    UInt32 moduleId, /* The container for respective modules */
    InternalCellIndex internalCellIndex
)
{
    UInt8 qci;
    /* SPR 10579 changes start*/
    LtePdcpKPIStats *pPdcpOamKPIStats = PNULL;
    LtePdcpKPIStats *pLtePdcpKPIStats = PNULL;

    LtePdcpKPIConfig *pPdcpOamKpiConfig = 
        &(g_kpiScannerObj.kpiStatsContainer[moduleId].config.ltePdcpKPIConfig); 
    /* SPR 10579 changes end*/

    pPdcpOamKPIStats = 
        &(g_kpiScannerObj.kpiStatsContainer[moduleId].stats.ltePdcpKPIStats[internalCellIndex]);

    pLtePdcpKPIStats =
        &(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].ltePdcpKPIStats[passiveIndexForKpiStats]);

    pPdcpOamKPIStats->lastCalculatedTTI += pLtePdcpKPIStats->lastCalculatedTTI;

    if (!(pPdcpOamKpiConfig->kpiBitmap))
    {
        return;
    }

    /* Get the data from shared memory to KPI container object */
    for (qci = 0; qci < MAX_QCI ; qci++)
    {
        if(IS_KPI_PDCP_BITSET(pPdcpOamKpiConfig->kpiBitmap, KPI_TOTAL_DL_DRB_DISCARD_RATE))
        {
            pPdcpOamKPIStats->totalDlDrbDiscardRatePerQci[qci] +=
                pLtePdcpKPIStats->totalDlDrbDiscardRatePerQci[qci];
            pPdcpOamKPIStats->totalDlDrbSduRcvd[qci] +=
                pLtePdcpKPIStats->totalDlDrbSduRcvd[qci];
        }

        /*spr 19296 fix start*/
        /*code deleted*/
        /*spr 19296 fix end*/

        if(IS_KPI_PDCP_BITSET(pPdcpOamKpiConfig->kpiBitmap, KPI_TOTAL_UL_DRB_LOSS_RATE))
        {
            pPdcpOamKPIStats->totalUlLossRatePerQci[qci] +=
                pLtePdcpKPIStats->totalUlLossRatePerQci[qci];
            pPdcpOamKPIStats->totalUlDrbSduRcvd[qci] +=
                pLtePdcpKPIStats->totalUlDrbSduRcvd[qci];
        }
    }

    if(IS_KPI_PDCP_BITSET(pPdcpOamKpiConfig->kpiBitmap, KPI_PDCP_DL_CTR_SDU_BITRATE))
    {
        /* SPR 12314 changes start */
        pPdcpOamKPIStats->dlCtrlPdcpSduBitRate +=
            pLtePdcpKPIStats->dlCtrlPdcpSduBitRate;
        /* SPR 12314 changes end */
    }

    if(IS_KPI_PDCP_BITSET(pPdcpOamKpiConfig->kpiBitmap, KPI_PDCP_UL_CTR_SDU_BITRATE))
    {
        /* SPR 12314 changes start */
        pPdcpOamKPIStats->ulCtrlPdcpSduBitRate +=
            pLtePdcpKPIStats->ulCtrlPdcpSduBitRate;
        /* SPR 12314 changes end */
    }
    return;
}

/****************************************************************************
 * Function Name  : updatePdcpKpiThpContainer
 * Inputs         : passiveIndexForKpiThpStats and moduleId
 * Outputs        : None
 * Returns        : None
 * Description    : This function will update KPI Container for OAM.
 ****************************************************************************/
/*SPR 15692 fix start*/
void updatePdcpKpiThpContainer(UInt32 passiveIndexForKpiThpStats,
        UInt32 moduleId,
        InternalCellIndex internalCellIndex)
/*SPR 15692 fix end*/
{
    LtePdcpKPIConfig *pPdcpOamKpiConfig     = 
        &(g_kpiScannerObj.kpiStatsContainer[moduleId].config.ltePdcpKPIConfig);

    LtePdcpKPIThpStats *pPdcpOamKPIThpStats = 
        &(g_kpiScannerObj.kpiStatsContainer[moduleId].stats.ltePdcpKPIThpStats);

    LtePdcpKPIThpStats *pLtePdcpKPIThpStats =
		&(gMacStats.pStats->lteCommonUeKPIStats.ltePdcpKPIThpStats[passiveIndexForKpiThpStats]);

    LteKPIThpStatus *pLtePdcpKPIThpStatus   =
        &(g_kpiScannerObj.kpiStatsContainer[moduleId].status.ltePdcpKPIThpStatus);

    /* +- SPR 18268 */
    UInt16  ueId =0;
    /* +- SPR 18268 */
    UInt8 lcId  =0;
    UInt16 activeUeIndex              = 0;
    UInt16 activeLcIndex              = 0;
    LP_PdcpUeContext     ueCtx_p      = PNULL;
    LP_PdcpEnityContext  ctx_p        = PNULL;

    if (!(pPdcpOamKpiConfig->kpiBitmap))
    {
        return;
    }

    pPdcpOamKPIThpStats->durationTTI += pLtePdcpKPIThpStats->durationTTI;

    for(activeUeIndex=0;activeUeIndex < pdcpContext.numOfActiveUe; activeUeIndex++)
    {
        ueCtx_p                    = PNULL;
        ctx_p                      = PNULL;

        ueId = pdcpContext.activeUeInfo[activeUeIndex];
        ueCtx_p = PDCP_GET_UE_CONTEXT(ueId);
        if(PNULL == ueCtx_p || ueId < pLtePdcpKPIThpStatus->startUeIdNxtBatch)
        {
            continue;
        }

        for(activeLcIndex=0;
                activeLcIndex < ueCtx_p->numOfActiveDrbLc && activeLcIndex < PDCP_MAX_DRB ; activeLcIndex++)
        {
            lcId  = ueCtx_p->activeLcInfo[activeLcIndex] - LTE_DRB_LC_ID_LOW_BOUND;
            ctx_p = pdcpGetEntityContext(ueId, ueCtx_p->activeLcInfo[activeLcIndex], PDCP_ENTITY_RB_TX);
            if(!ctx_p)
            {
                ctx_p = pdcpGetEntityContext(ueId, ueCtx_p->activeLcInfo[activeLcIndex], PDCP_ENTITY_RB_RX);
            }

            if(ctx_p)
            {

                /*spr 19296 fix start*/
                /*code deleted*/
                /*spr 19296 fix end*/

                if(pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[ueId][lcId]!= 0)
                {
                        pPdcpOamKPIThpStats->SharedMemoryDLPDCPSDU[ueId][lcId] += pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[ueId][lcId];
                }
                if(pLtePdcpKPIThpStats->SharedMemoryULPDCPSDU[ueId][lcId]!= 0)
                {
                        pPdcpOamKPIThpStats->SharedMemoryULPDCPSDU[ueId][lcId] += pLtePdcpKPIThpStats->SharedMemoryULPDCPSDU[ueId][lcId];
                }
        	/* SPR 10579 changes end */
            }

        } /* end of for (lcId) */

    }/* end of for (ueId) */
    /* SPR 15692 fix start */
    /*Cov 94787 +*/
    /* Update the THP Stats for the UE for which DELETE UE has received */
    pdcpUpdateKpiThpContainerForDeleteUEs(passiveIndexForKpiThpStats,moduleId,internalCellIndex);
    /*Cov 94787 - */
    /* SPR 15692 fix end */
}
/* SPR 10579 changes end*/


/****************************************************************************
 * Function Name  : pdcpResetKpiCounters
 * Inputs         : passiveIndexForPdcpStats -
 *                  internalCellIndex -
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API will reset all the KPI cumulative counters 
 *                  maintained at PDCP.
 ****************************************************************************/
void  pdcpResetKpiCounters(UInt32 passiveIndexForPdcpStats,InternalCellIndex internalCellIndex)
{
    if ( gMacStats.pStats )
    {
        memSet( &(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].
             ltePdcpKPIStats[passiveIndexForPdcpStats]), 0, 
             sizeof(LtePdcpKPIStats));
    }
}

/****************************************************************************
 * Function Name  : modCmdConfigureKpiStatsReq
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 *                  moduleId - KPI_CONTAINER_OAM_PDCP/KPI_CONTAINER_RRM_PDCP/
 *                             KPI_CONTAINER_SON_PDCP
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes OAM PDCP_CONFIGURE_KPI_STATS_REQ API.
 *                  It gets KPI configuration parameters, stores them &.
 *                  PDCP_CONFIGURE_KPI_STATS_RESP message to OAM in resp.
 ****************************************************************************/
void modCmdConfigureKpiStatsReq( UInt8* data_p, UInt16 size, UInt8 moduleId )
{
    UInt8                                *responseBuf          = PNULL;   
    LP_TlvMessage                         message_p            = (LP_TlvMessage)data_p;
    LP_TlvMessage                         responseMsg_p        = PNULL;
    LP_PdcpConfigureKpiStatsReqParams     cfgKpiStatsReq_p     = PNULL;
    LP_PdcpConfigureKpiStatsResp          cfgKpiStatsCnf_p     = PNULL;
    /* SPR 9068 Fix Start */
    LP_PdcpRrmConfigureKpiStatsResp       cfgRrmKpiStatsCnf_p     = PNULL;
    /* SPR 9068 Fix End */
    LtePdcpKPIConfig                     *pLtePdcpKPIConfig    = PNULL;
    UInt32                                modCounter           = 0;
    /* SPR 15909 fix start */
    tickType_t                            curr_tick            = 0;
    tickType_t                            diff                 = 0;
    /* SPR 15909 fix end */
    /* SPR 5443 */
    UInt16                                msgSize              = 0;
    /* SPR 5443 */
    LtePdcpKPIConfig *pKpiTHPConfig = PNULL;
    UInt8 moduleTHPId = 0;
    /* SPR 10579 changes start*/
    LteKPIThpStatus 	*pLtePdcpKPIThpStatus = PNULL;
    /* SPR 10579 changes end*/
    UInt8 numCellConfigured = 0;
    InternalCellIndex cellIndex = 0 ;
    pLtePdcpKPIConfig = &(g_kpiScannerObj.kpiStatsContainer[moduleId].config.ltePdcpKPIConfig);
    if(KPI_CONTAINER_RRM_PDCP == moduleId) 
    {
        cfgKpiStatsReq_p = (LP_PdcpConfigureKpiStatsReqParams)(data_p);
        moduleTHPId = KPI_CONTAINER_THP_RRM_PDCP;
    }

    else if(KPI_CONTAINER_OAM_PDCP == moduleId)
    {
        cfgKpiStatsReq_p = (LP_PdcpConfigureKpiStatsReqParams)(data_p + g_kpiModuleInfo[moduleId].hdrLen);
        moduleTHPId = KPI_CONTAINER_THP_OAM_PDCP;
    }
    else if(KPI_CONTAINER_SON_PDCP == moduleId)
    {
        cfgKpiStatsReq_p = (LP_PdcpConfigureKpiStatsReqParams)(data_p + g_kpiModuleInfo[moduleId].hdrLen);
        moduleTHPId = KPI_CONTAINER_THP_SON_PDCP;
    }
    /*coverity 52375*/
    if(PNULL == cfgKpiStatsReq_p)
    {
        return;
    }
    /*coverity 52375*/
    pKpiTHPConfig = &(g_kpiScannerObj.kpiStatsContainer[moduleTHPId].config.ltePdcpKPIConfig);
/* SPR 9068 Fix Start */
    if(KPI_CONTAINER_RRM_PDCP == moduleId) 
    {
        responseBuf = (UInt8 *)getMemFromPool( (g_kpiModuleInfo[moduleId].hdrLen + 
                    sizeof(PdcpRrmConfigureKpiStatsResp) ), PNULL);
        if ( PNULL == responseBuf ) 
        {
        /* +- SPR 17777 */
            LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, sizeof(TlvHeader) + \
                    sizeof(PdcpOamGetLogCategoryResp),size,0,0, 0,0, __func__,
                    "PDCP_CONFIGURE_KPI_STATS_REQ response");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            ltePanic ("getMemPool error in %s", __func__);
            return;
        }
        /* SPR 11109 Fix start */ 
        memSet(responseBuf, 0, (g_kpiModuleInfo[moduleId].hdrLen +
                    sizeof(PdcpRrmConfigureKpiStatsResp)));
        /* SPR 11109 Fix end */ 

        responseMsg_p = (LP_TlvMessage)&responseBuf[0];
        /* Prepare response buffer */
        tlvPrepareMessage( responseMsg_p,
                message_p->header.transactionId,
                g_kpiModuleInfo[moduleId].dstModuleId,
                g_kpiModuleInfo[moduleId].configCnfMsgType,
                sizeof(PdcpRrmConfigureKpiStatsResp),
                /* + Layer2 NON CA Changes */
                message_p->header.cellIndex);
                /* - Layer2 NON CA Changes */

        /*Prepare pointer to response message data */
        cfgRrmKpiStatsCnf_p = (LP_PdcpRrmConfigureKpiStatsResp)(responseBuf + g_kpiModuleInfo[moduleId].hdrLen);
    }
    else
    {
        responseBuf = (UInt8 *)getMemFromPool( (g_kpiModuleInfo[moduleId].hdrLen + 
                    sizeof(PdcpConfigureKpiStatsResp) ), PNULL);
        if ( PNULL == responseBuf ) 
        {
            LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, sizeof(TlvHeader) + \
                    sizeof(PdcpOamGetLogCategoryResp), 0,0,0, 0,0, __func__,
                    "PDCP_CONFIGURE_KPI_STATS_REQ response");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            ltePanic ("getMemPool error in %s", __func__);
            return;
        }
        /* SPR 11109 Fix start */ 
        memSet(responseBuf, 0, g_kpiModuleInfo[moduleId].hdrLen + 
                sizeof(PdcpConfigureKpiStatsResp));
        /* SPR 11109 Fix end */ 

        responseMsg_p = (LP_TlvMessage)&responseBuf[0];
        /* Prepare response buffer */
        tlvPrepareMessage( responseMsg_p,
                message_p->header.transactionId,
                g_kpiModuleInfo[moduleId].dstModuleId,
                g_kpiModuleInfo[moduleId].configCnfMsgType,
                sizeof(PdcpConfigureKpiStatsResp),
                /* + Layer2 NON CA Changes */
                message_p->header.cellIndex);
                /* - Layer2 NON CA Changes */

        /*Prepare pointer to response message data */
        cfgKpiStatsCnf_p = (LP_PdcpConfigureKpiStatsResp)(responseBuf + g_kpiModuleInfo[moduleId].hdrLen);
    }
    /* SPR 9068 Fix End */
    /* + SPR_10579 Merge Fix */
    pLtePdcpKPIThpStatus = &(g_kpiScannerObj.kpiStatsContainer[moduleTHPId].status.ltePdcpKPIThpStatus);
    /* - SPR_10579 Merge Fix */

    if(!(g_kpiScannerObj.module_bitmask))
    {
        g_kpiScannerObj.prevkpi_tick = pdcpCurrentTime_g;
    }

    /* Set default values in response message */
/* SPR 9068 Fix Start */
    if(KPI_CONTAINER_RRM_PDCP == moduleId) 
    {
        cfgRrmKpiStatsCnf_p->responseCode = PDCP_SUCCESS;
    }
    else
    {
        cfgKpiStatsCnf_p->responseCode = PDCP_SUCCESS;
    }
/* SPR 9068 Fix End */

    if (cfgKpiStatsReq_p->kpiToReportBitMap)
    {
        /*SPR_9013_FIX*/
        if((cfgKpiStatsReq_p->durationT <= 3600 && cfgKpiStatsReq_p->durationT >= 1) || \
                (cfgKpiStatsReq_p->durationT == 10100) || (cfgKpiStatsReq_p->durationT == 10200) ||\
                (cfgKpiStatsReq_p->durationT == 10300) || (cfgKpiStatsReq_p->durationT == 10400) ||\
                (cfgKpiStatsReq_p->durationT == 10500) || (cfgKpiStatsReq_p->durationT == 10600) ||\
                (cfgKpiStatsReq_p->durationT == 10700) || (cfgKpiStatsReq_p->durationT == 10800)||\
                (cfgKpiStatsReq_p->durationT == 10900))
        {
            /*SPR_9013_FIX*/
            /****Reseting timer list : start****/
            curr_tick  = pdcpCurrentTime_g;
            diff       = curr_tick - g_kpiScannerObj.prevkpi_tick;
            numCellConfigured = pdcpContext.sCellCount;
            for(cellIndex = 0; cellIndex < numCellConfigured; cellIndex++)
            {
                gMacStats.pStats->lteCommonKPIStats[cellIndex]. \
                    lteMacKPIStats[gMacStats.pStats->lteCommonKPIStats[cellIndex].activeIndexForMacStats].lastCalculatedTTI   += diff;
                gMacStats.pStats->lteCommonKPIStats[cellIndex]. \
                    ltePdcpKPIStats[gMacStats.pStats->lteCommonKPIStats[cellIndex].activeIndexForPdcpStats].lastCalculatedTTI += diff;
            }
            gMacStats.pStats->lteCommonUeKPIStats. \
                lteMacKPIThpStats[gMacStats.pStats->lteCommonUeKPIStats.activeIndexForMacThpStats].durationTTI += diff;
            gMacStats.pStats->lteCommonUeKPIStats. \
                ltePdcpKPIThpStats[gMacStats.pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats].durationTTI += diff;

            for(modCounter=0;modCounter<MAX_KPI_CONTAINER;modCounter++)
            {
                if(g_kpiScannerObj.module_bitmask & (1<<modCounter))
                {
                    g_kpiScannerObj.timeout_tti[modCounter]-=diff;
                }
            }
	      
            g_kpiScannerObj.prevkpi_tick = curr_tick;    
            /*******Reseting timer list : end ****/
            for(cellIndex=0; cellIndex < numCellConfigured; cellIndex++)
            {
                processStatsBuffBeforeConfig(0, cfgKpiStatsReq_p->kpiToReportBitMap, cellIndex);
            }
            if (cfgKpiStatsReq_p->kpiToReportBitMap & CELL_SPECIFIC_PDCP_COUNTERS_BITMAP)
            {
                g_kpiScannerObj.module_bitmask      |= (0x01 << moduleId);
                pLtePdcpKPIConfig->kpiConfigured     = 1;  
                pLtePdcpKPIConfig->kpiBitmap         = (cfgKpiStatsReq_p->kpiToReportBitMap & CELL_SPECIFIC_PDCP_COUNTERS_BITMAP);

                /* 100MS_SUPPORT : start */
                if(cfgKpiStatsReq_p->durationT <= 3600)  
                {
                    pLtePdcpKPIConfig->durationTTI       = (cfgKpiStatsReq_p->durationT) * 1000;
                }
                else 
                {
                    pLtePdcpKPIConfig->durationTTI       = (cfgKpiStatsReq_p->durationT) -KPI_CONFIG_ENUM_BASE;
                }
                /* 100MS_SUPPORT : end */
                pLtePdcpKPIConfig->periodicReporting = cfgKpiStatsReq_p->periodicReporting;
                pLtePdcpKPIConfig->transactionId     = message_p->header.transactionId;
                g_kpiScannerObj.kpiStatsContainer[moduleId].macOrPdcpStats = PDCP_KPI_STATS;

                macResetModuleKpiCounters((void *)(&g_kpiScannerObj.kpiStatsContainer[moduleId].stats.ltePdcpKPIStats), sizeof(LtePdcpKPIStats));
                g_kpiScannerObj.timeout_tti[moduleId] = pLtePdcpKPIConfig->durationTTI;
            }
            else
            {
                g_kpiScannerObj.module_bitmask &= ~(0x01 << moduleId);
                pLtePdcpKPIConfig->kpiConfigured = 0;
            } 
            /* Updating throughput stats container */  
            if ((IS_KPI_PDCP_BITSET(cfgKpiStatsReq_p->kpiToReportBitMap, KPI_PDCP_DL_LC_THROUGHPUT) || 
                        IS_KPI_PDCP_BITSET(cfgKpiStatsReq_p->kpiToReportBitMap, KPI_PDCP_UL_LC_THROUGHPUT)))
            {
                if(cfgKpiStatsReq_p->durationT <= 3600)  
                {
                    pKpiTHPConfig->durationTTI = (cfgKpiStatsReq_p->durationT) * 1000;
                } 
                else 
                {
                    pKpiTHPConfig->durationTTI  = (cfgKpiStatsReq_p->durationT) - KPI_CONFIG_ENUM_BASE;
                }

                /* COVERITY FIX 69271 START*/
                memSet((void *)((pLtePdcpKPIThpStatus)), 0x00, sizeof(LteKPIThpStatus));
                /* COVERITY FIX 69271 END*/
                for(cellIndex = 0 ; cellIndex < pdcpContext.sCellCount; cellIndex++)
                {
                    resetKpiContainer(moduleId, cellIndex);
                }
                (&g_kpiScannerObj.kpiStatsContainer[moduleTHPId].stats.ltePdcpKPIThpStats)->durationTTI = 0;
                /*Code Removed*/
                /* SPR 10579 changes end*/

                pKpiTHPConfig->kpiBitmap = (cfgKpiStatsReq_p->kpiToReportBitMap &\
                        ( (1 << KPI_PDCP_DL_LC_THROUGHPUT) | (1 << KPI_PDCP_UL_LC_THROUGHPUT)));
                pKpiTHPConfig->kpiConfigured = 1;
                g_kpiScannerObj.module_bitmask  |= (0x01 << moduleTHPId);
                g_kpiScannerObj.timeout_tti[moduleTHPId] = pKpiTHPConfig->durationTTI;
                pKpiTHPConfig->periodicReporting = cfgKpiStatsReq_p->periodicReporting;
                pKpiTHPConfig->transactionId     = message_p->header.transactionId;
                g_kpiScannerObj.kpiStatsContainer[moduleTHPId].macOrPdcpStats = PDCP_KPI_THP_STATS;
                /* SPR11759 Fix Start*/
                resetKpiContainer(moduleTHPId, 0); /* cell index not used in THP stats, so sending it as 0 */
                /* SPR11759 Fix End*/
            }
            else
            {
                g_kpiScannerObj.module_bitmask &= ~(0x01 << moduleTHPId);
                pKpiTHPConfig->kpiConfigured = 0;
            }

            eventfd_write_wrapper(pdcpOamMsgEventFd_g,1);
        /*SPR_9013_FIX*/
        }
        else
        {
            /* SPR 9068 Fix Start */
            if(KPI_CONTAINER_RRM_PDCP == moduleId) 
            {
                cfgRrmKpiStatsCnf_p->responseCode = PDCP_FAIL;
            }
            else
            {
                cfgKpiStatsCnf_p->responseCode = PDCP_FAIL;
            }
            /* SPR 9068 Fix End */
        }
        /*SPR_9013_FIX*/

    }
    else
    {
        g_kpiScannerObj.module_bitmask      &= ~(0x01 << moduleId);
        g_kpiScannerObj.module_bitmask      &= ~(0x01 << moduleTHPId);
        pKpiTHPConfig->kpiConfigured = 0;
        pLtePdcpKPIConfig->kpiConfigured     = 0;
        pLtePdcpKPIConfig->kpiBitmap         = cfgKpiStatsReq_p->kpiToReportBitMap;
        pKpiTHPConfig->kpiBitmap             = cfgKpiStatsReq_p->kpiToReportBitMap;
    }

    /* 100MS_SUPPORT : end */
    /* SPR 9068 Fix Start */
    if(KPI_CONTAINER_RRM_PDCP == moduleId) 
    {
        if ( PDCP_SUCCESS != cfgRrmKpiStatsCnf_p->responseCode )
        {
            LOG_PDCP_MSG(PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__, cfgRrmKpiStatsCnf_p->responseCode, 0,0,0,
                            /* SPR 9680 changes start */
                    0,0, __func__, "PDCP_CONFIGURE_KPI_STATS_RESP" );
                            /* SPR 9680 changes end */
        }
    }
    else
    {
        if ( PDCP_SUCCESS != cfgKpiStatsCnf_p->responseCode )
        {
            LOG_PDCP_MSG(PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__, cfgKpiStatsCnf_p->responseCode, 0,0,0,
                            /* SPR 9680 changes start */
                    0,0, __func__, "PDCP_CONFIGURE_KPI_STATS_RESP" );
                            /* SPR 9680 changes end */
        }
    }
    /* SPR 9068 Fix End */

    /*Convert response fields to network byte order */
    /* SPR 5443 */
    /* Intel-CA Fix Start */
    if(RRM_MODULE_ID != g_kpiModuleInfo[moduleId].dstModuleId)
    {
    /* Intel-CA Fix End */
        tlvPrepareHeaderForSending(responseMsg_p);
        msgSize = PDCP_NTOHS(responseMsg_p->header.length);
    /* Intel-CA Fix Start */
    }
    else
    {
        msgSize = responseMsg_p->header.length;
    }
    /* SPR 5443 */
    /* Intel-CA Fix End */

    /*send response */
    if(g_kpiModuleInfo[moduleId].sendToModule)
    {
        /* SPR 5443 */
        g_kpiModuleInfo[moduleId].sendToModule( responseBuf, msgSize
                ); 
        /* SPR 5443 */
    }
    freeMemPool ( responseBuf );
}

/****************************************************************************
 * Function Name  : oamCmdConfigureKpiStatsReq
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes OAM PDCP_CONFIGURE_KPI_STATS_REQ API.
 *                  It gets KPI configuration parameters, stores them &.
 *                  PDCP_CONFIGURE_KPI_STATS_RESP message to OAM in resp.
 ****************************************************************************/
void oamCmdConfigureKpiStatsReq( UInt8* data_p, UInt16 size )
{
    modCmdConfigureKpiStatsReq(data_p, size, KPI_CONTAINER_OAM_PDCP);
}

/****************************************************************************
 * Function Name  : modCmdGetKpiStatsReq
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 *                  moduleId - KPI_CONTAINER_OAM_PDCP/KPI_CONTAINER_RRM_PDCP/KPI_CONTAINER_SON_PDCP
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes OAM PDCP_GET_KPI_STATS_REQ API.
 *                  It API parses GET_KPI req and responds back to oam with
 *                  requested KPI indicators via PDCP_GET_KPI_STATS_RESP .
 ****************************************************************************/
/* + stats_review_comments_ipc */
void modCmdGetKpiStatsReq( UInt8* data_p, UInt16 size, UInt8 moduleId)
{
    UInt8             *responseBuf              = PNULL; /* Memory buffer for response */
    LP_TlvMessage     message_p                  = (LP_TlvMessage)data_p;
    LP_TlvMessage     responseMsg_p              = PNULL;
    LP_PdcpGetKpiStatsReqParams getKpiStatsReq_p = PNULL;
    LP_PdcpKpiStatsIndParams getKpiStatsCnf_p    = PNULL;
    LtePdcpKPIConfig    *pLtePdcpKPIConfig       = PNULL;
    LtePdcpKPIStats     *pLtePdcpKPIStats        = PNULL; 

    UInt8 kpiToGetBitMap            = 0;
    UInt8 resetStats                = 0;
    UInt32 passiveIndexForPdcpStats = 0;
    /* SPR 5443 */
    UInt16 msgSize                  = 0;
    /* SPR 5443 */
	/* + stats_review_comments_ipc */
	InternalCellIndex cellIndex = 0;
#ifndef UE_SIM_TESTING
	RrcCellIndex  cellId = message_p->header.cellIndex;
	cellIndex = PDCP_GET_INTERNAL_CELL_INDEX( cellId );

    if( INVALID_CELL_INDEX == cellIndex )
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG( PDCP_RRC_CELL_INDEX_ERR, LOGFATAL, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, (sizeof(TlvHeader) + \
                    sizeof(PdcpOamCnf) ),size,0,0,0,0, __func__,
                "PDCP_INVALID_CELL_INDEX" );
        return ;
    }
#endif
	/* - stats_review_comments_ipc */
	/* + KPI_CA */
    passiveIndexForPdcpStats = 
        gMacStats.pStats->lteCommonKPIStats[cellIndex].activeIndexForPdcpStats;
    pLtePdcpKPIConfig =  
        &(g_kpiScannerObj.kpiStatsContainer[moduleId].config.ltePdcpKPIConfig);
    pLtePdcpKPIStats = 
        &(g_kpiScannerObj.kpiStatsContainer[moduleId].stats.ltePdcpKPIStats[cellIndex]);

    /*Check API id */
    /* SPR 5443 */
    if(RRM_MODULE_ID != g_kpiModuleInfo[moduleId].dstModuleId)
    {
        getKpiStatsReq_p = (LP_PdcpGetKpiStatsReqParams)(data_p + g_kpiModuleInfo[moduleId].hdrLen);
    }
    else
    {
        getKpiStatsReq_p = (LP_PdcpGetKpiStatsReqParams)(data_p);
    }
    /* SPR 5443 */
    /* +- SPR 10577 */

	kpiToGetBitMap = 
		(pLtePdcpKPIConfig->kpiBitmap & getKpiStatsReq_p->kpiToGetBitMap);

    if(kpiToGetBitMap)
	{
		responseBuf = (UInt8 *)getMemFromPool((g_kpiModuleInfo[moduleId].hdrLen + 
                    sizeof(PdcpKpiStatsIndParams)), PNULL);
		if ( PNULL == responseBuf ) 
		{
			LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
					pdcpCurrentTime_g, __LINE__, g_kpiModuleInfo[moduleId].hdrLen + \
					sizeof(PdcpKpiStatsIndParams), 0,0,0, 0,0, __func__,
					"PDCP_GET_KPI_STATS_REQ response");
			ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
			ltePanic ("getMemPool error in %s", __func__);
			return;
		}
		responseMsg_p = (LP_TlvMessage)&responseBuf[0];
		if(pLtePdcpKPIConfig->kpiConfigured)
		{
            /* Prepare response buffer */
            tlvPrepareMessage( responseMsg_p,
                    message_p->header.transactionId,
                    g_kpiModuleInfo[moduleId].dstModuleId,
                    g_kpiModuleInfo[moduleId].aperiodicCnfMsgType,
                    sizeof(PdcpKpiStatsIndParams),
                    /* + Layer2 NON CA Changes */
                    cellId); /* + stats_review_comments_ipc - */
                    /* - Layer2 NON CA Changes */
        }
        else
        {
            tlvPrepareMessage( responseMsg_p,
                    message_p->header.transactionId,
                    g_kpiModuleInfo[moduleId].dstModuleId,
                    g_kpiModuleInfo[moduleId].aperiodicCnfMsgType,
                    sizeof(UInt32),
                    /* + Layer2 NON CA Changes */
                    cellId); /* + stats_review_comments_ipc - */
                    /* - Layer2 NON CA Changes */
		}
		/*Prepare pointer to response message data */
		getKpiStatsCnf_p = (LP_PdcpKpiStatsIndParams)
			(responseBuf + g_kpiModuleInfo[moduleId].hdrLen);

        /* SPR 10409 fix start  */
        memSet(getKpiStatsCnf_p, 0, sizeof(PdcpKpiStatsIndParams));
        /* SPR 10409 fix end */
		/* Update KPI Containers */
        /* + KPI_CA */
        updateStatsForAllModules(cellIndex);
		pdcpGetKpiStats(getKpiStatsCnf_p, kpiToGetBitMap, pLtePdcpKPIStats);
        /* 10409 spr fix start */
    }
    resetStats = getKpiStatsReq_p->resetStats;
		/* Reset the KPI Container */
		if(resetStats)
		{
        resetKpiContainer(moduleId, cellIndex);
		}
    if(kpiToGetBitMap)
    {
        /* 10409 spr fix end */
		/* Reset Cumulative counters */
        pdcpResetKpiCounters(passiveIndexForPdcpStats, cellIndex);
        /* - KPI_CA */
        /* Intel-CA Fix Start */
		if(RRM_MODULE_ID != g_kpiModuleInfo[moduleId].dstModuleId)
		{
			tlvPrepareHeaderForSending(responseMsg_p);
			msgSize = PDCP_NTOHS(responseMsg_p->header.length);
		}
		else
		{
			msgSize = responseMsg_p->header.length;
		}
		/* SPR 5443 */
        /* Intel-CA Fix End */
		/*send response */
		if(g_kpiModuleInfo[moduleId].sendToModule)
        {
			/* SPR 5443 */
            g_kpiModuleInfo[moduleId].sendToModule( responseBuf, msgSize
                    ); 
			/* SPR 5443 */
        }
		freeMemPool ( responseBuf );
    }
    /* +- SPR 10577 */
}

/* + SPR 10577 */
/****************************************************************************
 * Function Name  : sendPdcpGetKpiThpStatsCnf
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  moduleThpId - module Id to which THP stats to be sent
 * Outputs        : None.
 * Returns        : None.
 * Description    : It updates ue specific Kpi Thp Stats count needs to send
 *                  to OAM.
 ****************************************************************************/
void sendPdcpGetKpiThpStatsCnf( UInt8 *data_p, UInt8 moduleThpId )
{
    UInt8 kpiToGetThpBitMap = 0;
    /* + SPR 17637 */
    UInt8 resetStats        = 0;
    /* - SPR 17637 */
    UInt8 *responseBuf = PNULL; /* Memory buffer for response */
    LP_TlvMessage message_p = (LP_TlvMessage)data_p;
    LP_PdcpGetKpiStatsReqParams getKpiStatsReq_p = PNULL;
    LtePdcpKPIConfig *pLtePdcpThpKPIConfig = &(g_kpiScannerObj.
            kpiStatsContainer[moduleThpId].config.ltePdcpKPIConfig);
    InternalCellIndex cellIndex   = 0;
#ifndef UE_SIM_TESTING
    RrcCellIndex  cellId = message_p->header.cellIndex;
    cellIndex = PDCP_GET_INTERNAL_CELL_INDEX( cellId );

    if( INVALID_CELL_INDEX == cellIndex )
    {
        LOG_PDCP_MSG( PDCP_RRC_CELL_INDEX_ERR, LOGFATAL, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, (sizeof(TlvHeader) + \
                    sizeof(PdcpOamCnf) ), 0,0,0,0,0, __func__,
                "PDCP_INVALID_CELL_INDEX" );
        return ;
    }	
#endif

    if(RRM_MODULE_ID != g_kpiModuleInfo[moduleThpId].dstModuleId)
    {
        getKpiStatsReq_p = (LP_PdcpGetKpiStatsReqParams)(data_p + g_kpiModuleInfo[moduleThpId].hdrLen);
    }
    else
    {
        getKpiStatsReq_p = (LP_PdcpGetKpiStatsReqParams)(data_p);
    }

    kpiToGetThpBitMap = pLtePdcpThpKPIConfig->kpiBitmap & 
        getKpiStatsReq_p->kpiToGetBitMap;

    /* + SPR 17637 */
    if ( pLtePdcpThpKPIConfig->kpiConfigured  && kpiToGetThpBitMap &&
            (IS_KPI_PDCP_BITSET(kpiToGetThpBitMap, KPI_PDCP_DL_LC_THROUGHPUT) ||
             IS_KPI_PDCP_BITSET(kpiToGetThpBitMap, KPI_PDCP_UL_LC_THROUGHPUT)))
    {
    	/* - SPR 17637 */
        responseBuf = (UInt8 *)getMemFromPool((g_kpiModuleInfo[moduleThpId].hdrLen + 
                    sizeof(PdcpKpiThpStatsIndParams)), PNULL);
        if ( PNULL == responseBuf )
        {
            LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, g_kpiModuleInfo[moduleThpId].hdrLen + \
                    sizeof(PdcpKpiStatsIndParams), 0,0,0, 0,0, __func__,
                    "PDCP_GET_KPI_THP_STATS_RESP");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            ltePanic ("getMemPool error in %s", __func__);
            return;
        }
        pdcpGetAndSendKpiThpStats( responseBuf, 
                message_p->header.transactionId, kpiToGetThpBitMap, 
                moduleThpId, 
		/* SPR 22296 Fix Start */
                g_kpiModuleInfo[moduleThpId].aperiodicThpCnfMsgType, cellIndex ,LTE_TRUE);
	/* SPR 22296 Fix End */
        /* + SPR 17637 */
        resetStats = getKpiStatsReq_p->resetStats;
        /* Reset the KPI Container */
        if(resetStats)
        {
            resetKpiContainer(moduleThpId,cellIndex);
        }
        /* - SPR 17637 */
        freeMemPool ( responseBuf );
    }
}

/****************************************************************************
 * Function Name  : pdcpGetAndSendKpiThpStats
 * Inputs         : responseBuf - Pointer to confirmation buffer
 *                  transactionId - to be sent in API Header
 *                  kpiBitmap - Bitmap indicating KPI's to be pegged.
 *                  moduleTHPId - THP container Id
 *                  respMsgId - Response API ID to be filled in API Header
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will fill the Throughput stats in the 
 *                  confirmation buffer for Aperiodic KPI THP reports.
 ****************************************************************************/
    /* SPR 22296 Fix Start */
void pdcpGetAndSendKpiThpStats( UInt8 *responseBuf, UInt16 transactionId,
        UInt8 kpiBitmap, UInt8 moduleThpId, UInt16 respMsgId ,UInt8 internalCellIndex,UInt8 resetKpi)
	/* SPR 22296 Fix End */
{
    UInt8 isAllUEInfoSent = LTE_FALSE;
    UInt8 lcId = 0;
    UInt8 entry = 0;
    UInt16 msgSize = 0;
    UInt16 ueId = 0;
    UInt16 activeUeIndex = 0;
    UInt16 activeLcIndex = 0;
    LP_PdcpUeContext     ueCtx_p      = PNULL;
    LP_PdcpEnityContext  ctx_p        = PNULL;

    LP_TlvMessage responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    LteKPIThpStatus *pLtePdcpKPIThpStatus = &(g_kpiScannerObj.
            kpiStatsContainer[moduleThpId].status.ltePdcpKPIThpStatus);
    LtePdcpKPIThpStats *pLtePdcpKPIThpStats = &(g_kpiScannerObj.
            kpiStatsContainer[moduleThpId].stats.ltePdcpKPIThpStats);
    /*Prepare pointer to response message data */
    LP_PdcpKpiThpStatsIndParams cnfBuff = 
        (LP_PdcpKpiThpStatsIndParams)(responseBuf + 
                g_kpiModuleInfo[moduleThpId].hdrLen);

    cnfBuff->avgDurationT =  pLtePdcpKPIThpStats->durationTTI;
    cnfBuff->endIndicator = LTE_TRUE;

    if(!(IS_KPI_PDCP_BITSET(kpiBitmap, KPI_PDCP_DL_LC_THROUGHPUT) ||
                IS_KPI_PDCP_BITSET(kpiBitmap, KPI_PDCP_UL_LC_THROUGHPUT)))
    {
        return;
    }

    do
    {
        entry = 0;
        for(; activeUeIndex < pdcpContext.numOfActiveUe ; activeUeIndex++)
        {
            ueCtx_p = PNULL;
            ctx_p   = PNULL;

            /* Although activeUeInfo and activeLcInfo will have valid entries, 
             * but NULL check is added as this function can be called from LP 
             * thread in case of PDCP_KPI_THP_STATS_IND */
            ueId = pdcpContext.activeUeInfo[activeUeIndex];
            ueCtx_p = PDCP_GET_UE_CONTEXT(ueId);
            if ( PNULL == ueCtx_p )
            {
                /* UE Context does not exist */
                continue;
            }
	    /* SPR 22296 Fix Start */
            if(ueCtx_p->cellIndex != internalCellIndex)
            {
                continue;
            }
	    /* SPR 22296 Fix End */

            /* if this message is full of entries then break */
            if( ( entry >= MAX_KPI_INDEX ) ||
                    ( MAX_KPI_INDEX - entry) < ueCtx_p->numOfActiveDrbLc )
            {
                /* As entry is greater than MAX_KPI_INDEX, it means
                 * more data is pending for the message.
                 * Hence, setting endIndicator as FALSE */
                cnfBuff->endIndicator = LTE_FALSE;
                break;
            }

            for( activeLcIndex = 0;
                    activeLcIndex < ueCtx_p->numOfActiveDrbLc;
                    activeLcIndex++ )
            {
                lcId  = ueCtx_p->activeLcInfo[activeLcIndex] - 
                    LTE_DRB_LC_ID_LOW_BOUND;
                ctx_p = pdcpGetEntityContext( ueId, 
                        ueCtx_p->activeLcInfo[activeLcIndex], 
                        PDCP_ENTITY_RB_TX);
                if( PNULL == ctx_p )
                {
                    /* Valid LC context does not exist */
                    continue;
                }
                /* fill the data for UE/LC in buffer */
                pdcpUpdateKpiThpStatsForLc( ueId, lcId, entry, ctx_p->qci,
                        pLtePdcpKPIThpStats, cnfBuff);
                entry++;
            }
        }

        /* No need to send the deleted UE stats to RRM hence check for moduleId
         * is KPI_CONTAINER_THP_OAM_PDCP */
        if ( KPI_CONTAINER_THP_OAM_PDCP == moduleThpId )
        {
            /* Fill the stats for Deleted LC which cannot be captured in above
             * loop as the LC context is not available and numberOfActiveDrb 
             * count has decremented only if All the Active UE information is 
             * complete */
            if ( activeUeIndex >= pdcpContext.numOfActiveUe ) 
            {
                UInt8 numDelEntry = pdcpNumQciEntries( moduleThpId );
                if( 0 != numDelEntry )
                {
                    /* Since entry value is already incremented so check if
                     * (numDelEntry - 1) entries can be admitted */
                    if( MAX_KPI_INDEX >= ( entry + numDelEntry ) )
                    {
                        pdcpFillStatsOfDeletedLc(moduleThpId, cnfBuff, &entry);
                        isAllUEInfoSent = LTE_TRUE;
                    }
                    else
                    {
                        /* As entry is greater than MAX_KPI_INDEX and deleted
                         * UE stats are still not sent, set endIndicator to
                         * FALSE */
                        cnfBuff->endIndicator = LTE_FALSE;
                    }
                }
                else
                {
                    /* No deleted RAB entry and all UE information is sent */
                    isAllUEInfoSent = LTE_TRUE;
                }
            }
        }
        else
        {
            /* Incase the THP stats are for RRM, and for all UEs data is sent,
             * set isAllUEInfoSent */
            if( activeUeIndex >= pdcpContext.numOfActiveUe )
            {
                isAllUEInfoSent = LTE_TRUE;
            }
        }
    
        /* Set the last entry as INVALID to demarkate the end */
        if( MAX_KPI_INDEX > entry )
        {
            memSet( (void *)(&cnfBuff->ueIndex[entry]), 0xff,
                    ( sizeof(UInt16) * (MAX_KPI_INDEX-entry) ) );
            memSet( (void *)(&cnfBuff->qci[entry]), 0xff,
                    ( sizeof(UInt8) * (MAX_KPI_INDEX-entry) ) );
            /*spr 19711 fix start*/
            /*code deleted*/
            /*spr 19711 fix end*/

            /*spr 19296 fix start*/
            /*code deleted*/
            /*spr 19296 fix end*/

            memSet( (void *)(&cnfBuff->pdcpSduBitDl[entry]), 0xff,
                    ( sizeof(UInt64) * (MAX_KPI_INDEX-entry) ) );
            memSet( (void *)(&cnfBuff->pdcpSduBitUl[entry]), 0xff,
                    ( sizeof(UInt64) * (MAX_KPI_INDEX-entry) ) );
        }
    
        /* Reset the KPI Container when all the information is sent for all 
         * UEs */
        /* SPR 22296 Fix Start */
        /* SPR 22882 Start */
        if (LTE_TRUE == isAllUEInfoSent)
        {
            /* The end indicator in cnfBuff is set TRUE here which means that 
             * this is the end of the message  */
            cnfBuff->endIndicator = LTE_TRUE;
        }
        /* SPR 22882 End */
        if ( (LTE_TRUE == isAllUEInfoSent) && (LTE_TRUE == resetKpi))
        /* SPR 22296 Fix End */
        {
            pLtePdcpKPIThpStatus->servingRespId     = 0;
            pLtePdcpKPIThpStatus->moduleBitmap &= ~(0x01 << moduleThpId);
            /* Reset THP stats does not use cell index, so pass any value */
            /* +- SPR 17637 */
        /* SPR 22882 +- */
            /* code removed */
        }

        /* Prepare the TLV API header */
        tlvPrepareMessage( responseMsg_p, transactionId,
                g_kpiModuleInfo[moduleThpId].dstModuleId,
                respMsgId, sizeof(PdcpKpiThpStatsIndParams),
                /* SPR 22296 Fix Start */
                PDCP_GET_RRC_CELL_INDEX(internalCellIndex));
        /* SPR 22296 Fix End */

        /*Convert response fields to network byte order */
        if(RRM_MODULE_ID != g_kpiModuleInfo[moduleThpId].dstModuleId)
        {
            tlvPrepareHeaderForSending(responseMsg_p);
            msgSize = PDCP_NTOHS(responseMsg_p->header.length);
        }
        else
        {
            msgSize = responseMsg_p->header.length;
            /* Intel-CA Fix */
        }

        /*send response */
        if(g_kpiModuleInfo[moduleThpId].sendToModule)
        {
            g_kpiModuleInfo[moduleThpId].sendToModule( responseBuf, msgSize
                    );
        }
    } while( LTE_FALSE == cnfBuff->endIndicator );
}
/* - SPR 10577 */

/****************************************************************************
 * Function Name  : oamCmdGetKpiStatsReq
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes OAM PDCP_GET_KPI_STATS_REQ API.
 *                  It API parses GET_KPI req and responds back to oam with
 *                  requested KPI indicators via PDCP_GET_KPI_STATS_RESP .
 ****************************************************************************/
void oamCmdGetKpiStatsReq( UInt8* data_p, UInt16 size )
{
    /* Send the PDCP_KPI_STATS_CNF to OAM */
   	modCmdGetKpiStatsReq( data_p, size, KPI_CONTAINER_OAM_PDCP);

    /* + SPR 10577 */
    /* Send the PDCP_KPI_THP_STATS_CNF to OAM */
	sendPdcpGetKpiThpStatsCnf(data_p, KPI_CONTAINER_THP_OAM_PDCP);
    /* - SPR 10577 */
}
#endif

#ifdef PERF_STATS
/****************************************************************************
 * Function Name  : resetPdcpPerfContainer
 * Inputs         : resetId
 * Outputs        : none
 * Returns        :
 * Variables      :
 * Description    :
 *******************************************************************************/
void resetPdcpPerfContainer(UInt8 resetId)
{  
    switch(resetId)
    {
        case PERF_STATS_CONTAINER_OAM_PDCP_RESET_CELL_CONTAINER:
        {
            memSet(&(g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].
                        perfStats.ltePdcpPerfStats.ltePdcpCellPerfStats), 0 ,
                    sizeof(PdcpCellPerfStatsResp));
            break;
        }
        case PERF_STATS_CONTAINER_OAM_PDCP_RESET_UE_CONTAINER:
        {
            memSet(&(g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].
                        perfStats.ltePdcpPerfStats.ltePdcpUePerfStats), 0 ,
                    PERF_MAX_UE_SUPPORTED * sizeof(PdcpUePerfStatsResp));
            break;
        }
        default:
        break;
    }
}

/****************************************************************************
 * Function Name  : pdcpResetUePerfCounters
 * Inputs         : pLteUePerfStats
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API will reset all the KPI cumulative counters 
 *                  maintained at PDCP.
 ****************************************************************************/
void  pdcpResetUePerfCounters (LteUePerfStats *pLteUePerfStats)
{
    /* SPR 5449 changes start */
        UInt32 index = 0;
        LP_PdcpUeContext ueCtx_p = PNULL;

	if ( gPerfStats.perfStats )
	{
		for (index = 0; index < PERF_MAX_UE_SUPPORTED; index++)
		{
            ueCtx_p = PDCP_GET_UE_CONTEXT(index);
			if ( PNULL != ueCtx_p)
			{ 
				memSet (&pLteUePerfStats[index].lteUePdcpULPerfStats, 0,\
						(sizeof(LteUePdcpULPerfStats)));
				memSet (&pLteUePerfStats[index].lteUePdcpDLPerfStats, 0,\
						(sizeof(LteUePdcpDLPerfStats)));
			}
		}
    /* SPR 5449 changes end */
	}
}

/****************************************************************************
 * Function Name  : pdcpResetCellPerfCounters
 * Inputs         : pLteCellPerfStats
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API will reset all the KPI cumulative counters 
 *                  maintained at PDCP.
 ****************************************************************************/
void  pdcpResetCellPerfCounters (LteCellPerfStats *pLteCellPerfStats)
{
    if ( gPerfStats.perfStats )
    {
	    memSet(&(pLteCellPerfStats->lteCellPdcpULPerfStats), 0, 
			    sizeof(LteCellPdcpULPerfStats));
	    memSet(&(pLteCellPerfStats->lteCellPdcpDLPerfStats), 0,
			    sizeof(LteCellPdcpDLPerfStats));

    }
}

/****************************************************************************
 * Function Name  : oamCmdConfigurePerfStatsReq
 * Inputs         : data_p, size
 * Outputs        : None.
 * Returns        : None.
 * Description    : 
 ****************************************************************************/
void oamCmdConfigurePerfStatsReq( UInt8* data_p, UInt16 size )
{
    UInt8        *responseBuf;   /* Memory buffer for response */
    UInt8 passiveIndexForPdcpStats = gPerfStats.perfStatsControlInfo->pdcpActiveIndex;
    UInt32 modCounter = 0;
    /* SPR 15909 fix start */
    tickType_t curr_tick = pdcpCurrentTime_g;
    tickType_t diff = curr_tick - g_perfStatsScannerObj.prevPerfStats_tick;
    /* SPR 15909 fix end */
    LP_TlvMessage       message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage       responseMsg_p = PNULL;
    LP_PdcpConfigurePerfStatsReqParams cfgPerfStatsReq_p = PNULL;
    LP_PdcpConfigurePerfStatsCnfParams cfgPerfStatsCnf_p = PNULL;
    LtePerfStatsConfig *pOamPdcpPerfStatsConfig = &(g_perfStatsScannerObj.\
	    perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].
	    perfConfig.ltePdcpPerfStatsConfig);
	/* + PERF_CA */
	InternalCellIndex cellIndex = 0;
	/* - PERF_CA */
    /*Check API id */
    CHECK_EXP( PDCP_CONFIGURE_PERF_STATS_REQ != message_p->header.apiId );

    cfgPerfStatsReq_p = (LP_PdcpConfigurePerfStatsReqParams)message_p->data;

    responseBuf = (UInt8 *)getMemFromPool( (sizeof(TlvHeader) + 
                sizeof(LP_PdcpConfigurePerfStatsCnfParams) ), PNULL);
    if ( PNULL == responseBuf ) 
    {
	LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
		pdcpCurrentTime_g, __LINE__, sizeof(TlvHeader) + \
		sizeof(PdcpOamGetLogCategoryResp), 0,0,0, 0,0, __func__,
		"PDCP_CONFIGURE_PERF_STATS_REQ response");
	ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
	ltePanic ("getMemPool error in %s", __func__);
	return;
    }

    /* SPR 11109 Fix start */
    memSet(responseBuf, 0, (sizeof(TlvHeader)+
                sizeof(LP_PdcpConfigurePerfStatsCnfParams)));
    /* SPR 11109 Fix end */

    responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    /* SPR 5449 Fix Start*/
    /* SPR 5397 Fix Start*/
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
	    message_p->header.transactionId,
	    OAM_MODULE_ID,
	    PDCP_CONFIGURE_PERF_STATS_CNF,
	    sizeof(PdcpConfigurePerfStatsCnfParams)
#ifndef UE_SIM_TESTING
                ,message_p->header.cellIndex
#endif
        );
    /* SPR 5397 Fix End*/

        /* SPR 5449 Fix End*/

    if(!(g_perfStatsScannerObj.module_bitmask))
    {
        g_perfStatsScannerObj.prevPerfStats_tick = pdcpCurrentTime_g;
    }
    /*Prepare pointer to response message data */
    cfgPerfStatsCnf_p = (LP_PdcpConfigurePerfStatsCnfParams)responseMsg_p->data;
    /* Set default values in response message */
    cfgPerfStatsCnf_p->responseCode = PDCP_SUCCESS;

    if ((cfgPerfStatsReq_p->duration > PDCP_MAX_VAL_PERF_TIMER_T ) ||
	    (cfgPerfStatsReq_p->duration < PDCP_MIN_VAL_PERF_TIMER_T ))
    {
	cfgPerfStatsCnf_p->responseCode = 
	    PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
    }
    else
    {
	if ((cfgPerfStatsReq_p->uePerfStatsToReportBitMap) || 
		(cfgPerfStatsReq_p->cellPerfStatsToReportBitMap))
	{

	    gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->macActiveIndex].macLastCalculatedTTI += diff;
	    gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->pdcpActiveIndex].pdcpLastCalculatedTTI += diff;
	    gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->rlcDLActiveIndex].rlcDLLastCalculatedTTI += diff;

	    for (modCounter = 0; modCounter < MAX_PERF_STATS_CONTAINER; modCounter++)
	    {
		if (g_perfStatsScannerObj.module_bitmask & (1 << modCounter)) 
		{
		    g_perfStatsScannerObj.timeoutTTI[modCounter] -= diff; 

		}
	    }
	    g_perfStatsScannerObj.prevPerfStats_tick = curr_tick;
	    g_perfStatsScannerObj.module_bitmask |= PERF_STATS_CONTAINER_OAM_PDCP_BITMASK;
	    pOamPdcpPerfStatsConfig->periodicReporting = cfgPerfStatsReq_p->periodicReporting;
	    pOamPdcpPerfStatsConfig->duration = (cfgPerfStatsReq_p->duration) * 1000; 
	    pOamPdcpPerfStatsConfig->uePerfStatsToReportBitMap = 
			cfgPerfStatsReq_p->uePerfStatsToReportBitMap;
	    pOamPdcpPerfStatsConfig->cellPerfStatsToReportBitMap = 
		cfgPerfStatsReq_p->cellPerfStatsToReportBitMap;
	    g_perfStatsScannerObj.timeoutTTI[PERF_STATS_CONTAINER_OAM_PDCP] = 
		pOamPdcpPerfStatsConfig->duration;
            
            /* Perf stats RRM-MAC start */
            g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].macOrPdcpOrRlcStats = PDCP_PERF_STATS;
            /* Perf stats RRM-MAC end */


	    /*Switching the buffer, Toggle */
	    (gPerfStats.perfStatsControlInfo->pdcpActiveIndex) = passiveIndexForPdcpStats ^0x01;

	    /* Switching counter position from passive to active */
	/* + PERF_CA */
		for (cellIndex = 0; cellIndex < pdcpContext.sCellCount; cellIndex++)
        {
	    	gPdcpCellPerfStats_p[cellIndex] = 
				&gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->pdcpActiveIndex].lteCellPerfStats[cellIndex];
		}
	    gPdcpUePerfStats_p = 
		gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->pdcpActiveIndex].lteUePerfStats;

	    /* Reset the pdcp shm stats at active index */
		for (cellIndex = 0; cellIndex < pdcpContext.sCellCount; cellIndex++)
        {
	    	pdcpResetCellPerfCounters(&(gPerfStats.perfStats[passiveIndexForPdcpStats].lteCellPerfStats[cellIndex]));
		}
	/* - PERF_CA */
	    pdcpResetUePerfCounters(gPerfStats.perfStats[passiveIndexForPdcpStats].lteUePerfStats);
	    gPerfStats.perfStats[passiveIndexForPdcpStats].pdcpLastCalculatedTTI = 0;
	    /* Reset container stats */ 
	    resetPdcpPerfContainer(PERF_STATS_CONTAINER_OAM_PDCP_RESET_UE_CONTAINER);
	    resetPdcpPerfContainer(PERF_STATS_CONTAINER_OAM_PDCP_RESET_CELL_CONTAINER);
	    g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].
		perfStats.ltePdcpPerfStats.lastPDCPCalculatedTTI = 0;

	    eventfd_write_wrapper(pdcpOamMsgEventFd_g,1);
	}
	else
	{
	    g_perfStatsScannerObj.module_bitmask &= ~(PERF_STATS_CONTAINER_OAM_PDCP_BITMASK);
	}
    }
    if ( PDCP_SUCCESS != cfgPerfStatsCnf_p->responseCode )
    {
        /* +- SPR 17777 */
	LOG_PDCP_MSG(PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, cfgPerfStatsCnf_p->responseCode,size,0,0,
		0,0, __func__, "PDCP_CONFIGURE_PERF_STATS_CNF" );
    }

    /*Convert response fields to network byte order */
    tlvPrepareHeaderForSending(responseMsg_p);

    /*send response */
    pdcpOamSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
        );
    freeMemPool ( responseBuf );

}

/****************************************************************************
 * Function Name  : oamCmdGetPerfstatsreq
 * Inputs         : data_p, size
 * Outputs        : None.
 * Returns        : None.
 * Description    : 
 *                  
 *****************************************************************************/
void oamCmdGetPerfstatsReq( UInt8* data_p, UInt16 size )
{
    UInt8 *cellResponseBuf;   /* Memory buffer for response */
    LP_TlvMessage       message_p                      = (LP_TlvMessage)data_p;
    LP_TlvMessage ueResponseMsg_p                      = PNULL;
    LP_TlvMessage cellResponseMsg_p                    = PNULL;
    LP_PdcpGetPerfStatsReqParams getPerfStatsReq_p     = PNULL;
    LP_PdcpGetCellPerfStatsCnfParams getCellPerfConf_p = PNULL;
    LP_PdcpGetUePerfStatsCnfParams getUePerfConf_p     = PNULL;
    UInt32 uePerfStatsToGetBitMap    = 0;
    UInt32 cellPerfStatsToGetBitMap  = 0;
    UInt32 resetStats 		     = 0;
    UInt32 ueRespSize                = 0;
    UInt32 cellRespSize              = 0;
    UInt32 ueIndex                   = 0;
    UInt32 modCounter = 0;
    /* SPR 15909 fix start */
    tickType_t curr_tick = pdcpCurrentTime_g;
    tickType_t diff = curr_tick - g_perfStatsScannerObj.prevPerfStats_tick;
    /* SPR 15909 fix end */
	/* + stats_review_comments_ipc */
	InternalCellIndex cellIndex = 0;
#ifndef UE_SIM_TESTING
	RrcCellIndex  cellId = message_p->header.cellIndex;
	cellIndex = PDCP_GET_INTERNAL_CELL_INDEX( cellId );

    if( INVALID_CELL_INDEX == cellIndex )
    {
        LOG_PDCP_MSG( PDCP_RRC_CELL_INDEX_ERR, LOGFATAL, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, (sizeof(TlvHeader) + \
                    sizeof(PdcpOamCnf) ), 0,0,0,0,0, __func__,
                "PDCP_INVALID_CELL_INDEX" );
        return ;
    }	
#endif

    /*Check API id */
    CHECK_EXP( PDCP_GET_PERF_STATS_REQ != message_p->header.apiId );
    getPerfStatsReq_p = (LP_PdcpGetPerfStatsReqParams)message_p->data;

    cellResponseBuf = (UInt8 *)getMemFromPool( (sizeof(TlvHeader) + 
                sizeof(PdcpGetCellPerfStatsCnfParams)), PNULL);
    if ( PNULL == cellResponseBuf ) 
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, sizeof(TlvHeader) + \
                sizeof(PdcpGetCellPerfStatsCnfParams),size,0,0, 0,0, __func__,
                "PDCP_GET_PERF_STATS_REQ response");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic ("getMemPool error in %s", __func__);
        return;
    }
    /* SPR 5449 changes start */
    ueResponseMsg_p = (LP_TlvMessage)&(ueResponseBuf_g[0]);
    /* SPR 5449 changes end */
    cellResponseMsg_p = (LP_TlvMessage)&cellResponseBuf[0];
    /* SPR 5449 changes end */
    gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->macActiveIndex].macLastCalculatedTTI += diff;
    gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->pdcpActiveIndex].pdcpLastCalculatedTTI += diff;
    gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->rlcDLActiveIndex].rlcDLLastCalculatedTTI += diff;
    for (modCounter = 0; modCounter < MAX_PERF_STATS_CONTAINER; modCounter++)
    {
        if (g_perfStatsScannerObj.module_bitmask & (1 << modCounter)) 
        {
            g_perfStatsScannerObj.timeoutTTI[modCounter] -= diff; 

        }
    }
    g_perfStatsScannerObj.prevPerfStats_tick = curr_tick;

    uePerfStatsToGetBitMap = getPerfStatsReq_p->uePerfStatsToGetBitMap & 
        g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].\
        perfConfig.ltePdcpPerfStatsConfig.uePerfStatsToReportBitMap;

    cellPerfStatsToGetBitMap = getPerfStatsReq_p->cellPerfStatsToGetBitMap &
        g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].\
        perfConfig.ltePdcpPerfStatsConfig.cellPerfStatsToReportBitMap;

    resetStats = getPerfStatsReq_p->resetStats;


    if ( uePerfStatsToGetBitMap || cellPerfStatsToGetBitMap)
    {

        /* Perf stats RRM-MAC start */
        /*SPR 15909 fix start*/
        tickType_t pdcpLastCalculatedTTI = 0;
        /*SPR 15909 fix end*/
        UInt32 activeIndexForPdcpStats    =  gPerfStats.perfStatsControlInfo->pdcpActiveIndex;
        UInt32 passiveIndexForPdcpStats   = 0;
        passiveIndexForPdcpStats    = activeIndexForPdcpStats;
        pdcpLastCalculatedTTI =
            gPerfStats.perfStats[passiveIndexForPdcpStats].pdcpLastCalculatedTTI;
        if (pdcpLastCalculatedTTI)
        {
            /*Switching the buffer, Toggle */
            (gPerfStats.perfStatsControlInfo->pdcpActiveIndex) =
                passiveIndexForPdcpStats ^0x01;

            /* Switching counter position from passive to active */
			/* - stats_review_comments_ipc */
           	gPdcpCellPerfStats_p[cellIndex] =
               	&gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->pdcpActiveIndex].lteCellPerfStats[cellIndex];
            gPdcpUePerfStats_p =
                gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->pdcpActiveIndex].lteUePerfStats;
        }
		/* + PERF_CA */
        updatePdcpPerfContainer();
		pdcpResetCellPerfCounters(&(gPerfStats.perfStats[passiveIndexForPdcpStats].lteCellPerfStats[cellIndex])); 
			/* - stats_review_comments_ipc */
        pdcpResetUePerfCounters(gPerfStats.perfStats[passiveIndexForPdcpStats].lteUePerfStats);
        gPerfStats.perfStats[passiveIndexForPdcpStats].pdcpLastCalculatedTTI = 0;
        /* Perf stats RRM-MAC end */
    }

    if(!uePerfStatsToGetBitMap || !cellPerfStatsToGetBitMap)
    { 
        ueRespSize = cellRespSize = sizeof(UInt32);
    }

    /* UE specific message */
    if(uePerfStatsToGetBitMap)
    {
        ueRespSize = sizeof(PdcpGetUePerfStatsCnfParams);
    }
    if(ueRespSize)
    {
        do
        {
            getUePerfConf_p = (LP_PdcpGetUePerfStatsCnfParams)ueResponseMsg_p->data;
            memSet(getUePerfConf_p,0,sizeof(PdcpGetUePerfStatsCnfParams));

			/* + stats_review_comments_ipc */
            ueIndex = pdcpGetUePerfStats(getUePerfConf_p, uePerfStatsToGetBitMap, 
                    ueIndex,UNSET_PERIODIC_FLAG,cellIndex);
			/* - stats_review_comments_ipc */
       	    /* SPR 10577 changes start */
            getUePerfConf_p->endIndicator = LTE_FALSE;
            if((ueIndex >= PERF_MAX_UE_SUPPORTED) || 
                (ueIndex >= pdcpContext.numOfActiveUe)
              )
	    {
                /* endIndicator is set TRUE, as this is the end of the message */
	    	getUePerfConf_p->endIndicator = LTE_TRUE;
	    }
            /* sizeof(UInt8) has been added for endIndicator and padding */ 
            ueRespSize = (((getUePerfConf_p->numOfUePresent) * sizeof(PdcpUePerfStatsResp)) + 3* sizeof(UInt32)+ 4* sizeof(UInt8));
	    /* SPR 10577 changes end */

            tlvPrepareMessage( ueResponseMsg_p,
                    message_p->header.transactionId,
                    OAM_MODULE_ID,
                    PDCP_GET_UE_PERF_STATS_CNF,
                    ueRespSize
#ifndef UE_SIM_TESTING
                ,message_p->header.cellIndex /* + stats_review_comments_ipc */
#endif
                );
        
            tlvPrepareHeaderForSending(ueResponseMsg_p);
            /*send response */
            pdcpOamSendMessage( ueResponseBuf_g, 
                    PDCP_NTOHS(ueResponseMsg_p->header.length)
                    );
        }while(ueIndex < PERF_MAX_UE_SUPPORTED);
    }
    /* Cell specific */
	/* + stats_review_comments_ipc */
    if(cellPerfStatsToGetBitMap)
    {
        cellRespSize = sizeof(PdcpGetCellPerfStatsCnfParams);

    }
    if(cellRespSize)
    {
        tlvPrepareMessage( cellResponseMsg_p,
                message_p->header.transactionId,
                OAM_MODULE_ID,
                PDCP_GET_CELL_PERF_STATS_CNF,
               	cellRespSize
#ifndef UE_SIM_TESTING
                ,message_p->header.cellIndex 
#endif
               	);

        getCellPerfConf_p = (LP_PdcpGetCellPerfStatsCnfParams)cellResponseMsg_p->data;
        memSet(getCellPerfConf_p,0,sizeof(PdcpGetCellPerfStatsCnfParams));
       	pdcpGetCellPerfStats(getCellPerfConf_p,UNSET_PERIODIC_FLAG,cellPerfStatsToGetBitMap,cellIndex);

        /*Convert response fields to network byte order */
        tlvPrepareHeaderForSending(cellResponseMsg_p);

        pdcpOamSendMessage( cellResponseBuf, 
                PDCP_NTOHS(cellResponseMsg_p->header.length)
                );
    }
	/* - stats_review_comments_ipc */
	/* - PERF_CA */
    if ( resetStats && ( cellPerfStatsToGetBitMap || uePerfStatsToGetBitMap ))
    {
        /* Reset container stats */
        resetPdcpPerfContainer(PERF_STATS_CONTAINER_OAM_PDCP_RESET_UE_CONTAINER);
        resetPdcpPerfContainer(PERF_STATS_CONTAINER_OAM_PDCP_RESET_CELL_CONTAINER);
        g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].
            perfStats.ltePdcpPerfStats.lastPDCPCalculatedTTI = 0;

    }

    eventfd_write_wrapper(pdcpOamMsgEventFd_g,1);
    /* SPR 5449 changes start */
    freeMemPool ( cellResponseBuf );
    /* SPR 5449 changes end */
}
#endif

/****************************************************************************
 * Function Name  : oamCmdHealthMonitorStatusReq
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    :
 ****************************************************************************/
void oamCmdHealthMonitorStatusReq( UInt8* data_p, UInt16 size )
{
    UInt8           responseBuf[PDCP_OAM_CNF_MAX_SIZE_SMALL];   /* Memory buffer for response */
    LP_TlvMessage   message_p          = (LP_TlvMessage)data_p;
    UInt16          resp               = PDCP_FAIL;
    UInt8           threadIndx         = 0;
    /* SPR 15909 fix start */
    tickType_t      curr_tick          =  pdcpCurrentTime_g;
    /* SPR 15909 fix end */
    LP_TlvMessage responseMsg_p = (LP_TlvMessage)&responseBuf[0];

    /*Check API id */
    CHECK_EXP( PDCP_PROC_SUP_REQ != message_p->header.apiId );
    /*spr 8785 chg start*/
    /* CA changes start */
    /* Check the state of cells, if not any cell active then always return success */
    if(INVALID_CELL_INDEX != layer2CommonGetActiveCellTick())
    /* CA changes end */
    {
        for (threadIndx = 0; threadIndx < PDCP_MAX_THREAD_ID; threadIndx++)
        {
            /* CA changes start */
            if (curr_tick - l2ThreadHealthStoreTick_g.pdcpThread[threadIndx] <
                    pdcpHealthMonitoringTimeInterval_g)
            /* CA changes end */
            {
                resp = PDCP_SUCCESS;
            }
            else
            {
                resp = PDCP_FAIL;
                /* Break from loop if any thread is in fail state */
                break;
            }
        }
    }
    else
    {
        resp = PDCP_SUCCESS;
    }
    /*spr 8785 chg end*/
    if ( PDCP_SUCCESS == resp )
    {
        /* + Layer2 NON CA Changes */
        /* Prepare response buffer */
        tlvPrepareMessage( responseMsg_p,
                message_p->header.transactionId,
                OAM_MODULE_ID,
                PDCP_PROC_SUP_RESP,
                0
#ifndef UE_SIM_TESTING
                /* SPR 21683+- */
                ,message_p->header.cellIndex
#endif
                );
        /* - Layer2 NON CA Changes */
        /* SPR 21683+- */
        /*Convert Header fields to network byte order*/
        tlvPrepareHeaderForSending( responseMsg_p );

        /*send response */
        pdcpOamSendMessage( responseBuf,
                PDCP_NTOHS(responseMsg_p->header.length)
                );
    }
    else
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG(HSM_PDCP_OAM_REQ_FAILURE, LOGFATAL, L2_SYS_FAIL,
                pdcpCurrentTime_g,__LINE__,
                size, DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                __func__,"PDCP OAM Health Status Failure");

    }
}

/*spr 22474 start*/
/****************************************************************************
 * Function Name  : oamCmdGetDebugInfoReq
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    :
 ****************************************************************************/
void oamCmdGetDebugInfoReq( UInt8* data_p, UInt16 size )
{
    UInt8           *responseBuf = PNULL;   /* Memory buffer for response */
    LP_TlvMessage   message_p          = (LP_TlvMessage)data_p;
    UInt16          resp               = PDCP_FAIL;
    UInt8           threadIndx         = 0;
    tickType_t      curr_tick          =  pdcpCurrentTime_g;
    LP_TlvMessage responseMsg_p = PNULL;
    LP_PdcpGetDebugInfoReqParams pdcpGetDebugInfoReqParams_p = PNULL;
    LP_PdcpGetDebugInfoRespParams pdcpGetDebugInfoRespParams_p = PNULL;
    /*Check API id */
    CHECK_EXP( OAM_PDCP_GET_DEBUG_INFO_REQ != message_p->header.apiId );
    pdcpGetDebugInfoReqParams_p = (LP_PdcpGetDebugInfoReqParams)message_p->data;

    responseBuf = (UInt8 *)getMemFromPool( (
                sizeof(TlvHeader) + sizeof(PdcpGetDebugInfoRespParams)), PNULL);
    if ( PNULL == responseBuf ) 
    {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, sizeof(TlvHeader), 0,0,0, 
                0,0, __func__, "PDCP_NOTIFY_OAM_DEVICE_FAILURE");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic ("getMemPool error in %s", __func__);
        return;
    }
    responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    /* Check the state of cells, if not any cell active then always return success */
    if(INVALID_CELL_INDEX != layer2CommonGetActiveCellTick())
    {
        for (threadIndx = 0; threadIndx < PDCP_MAX_THREAD_ID; threadIndx++)
        {
            if (curr_tick - l2ThreadHealthStoreTick_g.pdcpThread[threadIndx] <
                    pdcpHealthMonitoringTimeInterval_g)
            {
                resp = PDCP_SUCCESS;
            }
            else
            {
                resp = PDCP_FAIL;
                /* Break from loop if any thread is in fail state */
                break;
            }
        }
    }
    else
    {
        resp = PDCP_SUCCESS;
    }
    if ( PDCP_SUCCESS == resp )
    {
        /* Prepare response buffer */
        tlvPrepareMessage( responseMsg_p,
                message_p->header.transactionId,
                OAM_MODULE_ID,
                PDCP_OAM_GET_DEBUG_INFO_RESP,
                sizeof(PdcpGetDebugInfoRespParams)
#ifndef UE_SIM_TESTING
                ,message_p->header.cellIndex
#endif
                );
        pdcpGetDebugInfoRespParams_p = (LP_PdcpGetDebugInfoRespParams )responseMsg_p->data;
        switch(pdcpGetDebugInfoReqParams_p->debugType)
        {
            case GET_DEBUG_INFO_MEM_STATS:
            {
                L2PrepareMemPoolStats (&(pdcpGetDebugInfoRespParams_p->u.csplMemPoolStats));
                pdcpGetDebugInfoRespParams_p->responseCode = PDCP_SUCCESS;
                pdcpGetDebugInfoRespParams_p->debugType = GET_DEBUG_INFO_MEM_STATS;
                break;
            }
            case GET_DEBUG_INFO_MSG_STATS:
            {
                L2PrepareMsgPoolStats (&(pdcpGetDebugInfoRespParams_p->u.csplMsgPoolStats));
                pdcpGetDebugInfoRespParams_p->responseCode = PDCP_SUCCESS;
                pdcpGetDebugInfoRespParams_p->debugType = GET_DEBUG_INFO_MSG_STATS;
                break;
            }
            default:
            {
                LOG_PDCP_ERROR( PDCP_RRCOAM, "Invalid debugType" );
                pdcpGetDebugInfoRespParams_p->responseCode = PDCP_FAIL;
                pdcpGetDebugInfoRespParams_p->debugType = pdcpGetDebugInfoReqParams_p->debugType;
                break;
            }

        }
        /*Convert Header fields to network byte order*/
        tlvPrepareHeaderForSending( responseMsg_p );

        /*send response */
        pdcpOamSendMessage( responseBuf,
                PDCP_NTOHS(responseMsg_p->header.length));
    }
    else
    {
            LOG_PDCP_ERROR( PDCP_RRCOAM, "PDCP OAM Debug Info Failure" );
    }
    freeMemPool ( responseBuf );
}
/*spr 22474 end*/

#endif

#ifdef PDCP_ASYNC_INTERFACE
/****************************************************************************
 * Function Name  : oamCmdNotifyDeviceFailure
 * Inputs         : None 
 * Outputs        : None.
 * Returns        : None.
 * Description    : It sends notification to OAM when device (security engine)
 *                  failure occurs in case of asynchronous interface.
 ****************************************************************************/
void oamCmdNotifyDeviceFailure( void )
{
    UInt8          *responseBuf;   /* Memory buffer for response */
    LP_TlvMessage  responseMsg_p = PNULL;

    responseBuf = (UInt8 *)getMemFromPool( (
                sizeof(TlvHeader) ), PNULL);
    if ( PNULL == responseBuf ) 
    {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, sizeof(TlvHeader), 0,0,0, 
                0,0, __func__, "PDCP_NOTIFY_OAM_DEVICE_FAILURE");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic ("getMemPool error in %s", __func__);
        return;
    }

    responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            OAM_TRANS_ID,
            OAM_MODULE_ID,
            PDCP_NOTIFY_OAM_DEVICE_FAILURE,
            0,
            0); /* Compile Fix */
    /*Convert response fields to network byte order */
    tlvPrepareHeaderForSending(responseMsg_p);

    /*send response */
    pdcpOamSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    freeMemPool ( responseBuf );
}
#endif

/****************************************************************************
 * Function Name  : oamCmdPrepInitInd
 * Inputs         : responseBuf, length, intCellId 
 * Outputs        : None.
 * Returns        : None.
 * Description    : It prepares the PDCP Init Ind message for OAM.
 ****************************************************************************/
void oamCmdPrepInitInd( UInt8 *responseBuf, UInt16 *length,
        InternalCellIndex intCellId )
{
    LP_TlvMessage  responseMsg_p = (LP_TlvMessage)&responseBuf[0];

    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            0,
            OAM_MODULE_ID,
            PDCP_INIT_LAYER_IND,
            0
#ifndef UE_SIM_TESTING
            , intCellId
#endif
            );

    /* Store thr length to return */
    *length = responseMsg_p->header.length;

    /*Convert response fields to network byte order */
    tlvPrepareHeaderForSending(responseMsg_p);
    
    LOG_PDCP_INFO( PDCP_RRCOAM, "Sending PDCP Init Indication to OAM" );
}

