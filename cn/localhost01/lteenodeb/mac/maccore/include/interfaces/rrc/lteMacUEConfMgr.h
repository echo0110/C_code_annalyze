/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacUEConfMgr.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacUEConfMgr.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.13  2009/07/13 04:06:48  gur19413
 * added new Macro for log
 *
 * Revision 1.12  2009/07/01 04:21:16  gur15697
 * review comments incorporated
 *
 * Revision 1.11  2009/06/24 14:04:53  gur19413
 * added DRX Node structure.
 *
 * Revision 1.10  2009/06/23 23:22:10  gur19413
 * added function prototype.
 *
 * Revision 1.9  2009/06/23 22:59:07  gur19413
 * initUEConf() added
 *
 * Revision 1.8  2009/06/18 06:19:28  gur15697
 * Redefination warning of  MAC_LOG_CH_EXISTS, MAC_LOG_CH_NOT_EXIST, MAC_MAX_ULLOG_CH_CONFIG
 * removed
 *
 * Revision 1.7  2009/06/15 06:31:16  gur19413
 * added function prototype updateDRXInUEContext
 *
 * Revision 1.6  2009/06/11 11:31:21  gur19413
 * function prototype change
 *
 * Revision 1.5  2009/06/05 14:03:28  gur18569
 * fixed bugs
 *
 * Revision 1.4  2009/06/03 14:41:41  gur19413
 * remove compilation error
 *
 * Revision 1.3  2009/05/28 06:17:02  gur19413
 * remove compilation error
 *
 * Revision 1.2  2009/05/13 16:18:05  gur19413
 * remove compilation error
 *
 * Revision 1.1  2009/04/30 12:12:54  gur19413
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_UE_CONF_MGR_H
#define LTE_MAC_UE_CONF_MGR_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacParseUtil.h"
#include "lteMacDLUEContext.h"
#include "lteMacULUEContext.h"
#include "lteMacPUCCH.h"
#include "lteMacRandomAccess.h"
#include "lteMacDLResourceAllocator.h"
#include "lteMacDLResourceInfoMgr.h"
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define DOWNLINK 0
#define UPLINK 1

/****************************************************************************
 * Exported Types
 ****************************************************************************/


typedef struct PendingDeleteQueueNodeT
{
    /*data type of anchor, stored in every List Node*/
    LTE_SNODE     nodeAnchor;
    UInt16        ueIndex;
    UInt16        transactionId;
    /* SPR 15909 fix start */
    tickType_t        tti;
    /* SPR 15909 fix end */
    RNTI          rntiFlag;
}PendingDeleteQueueNode;

typedef struct MacResetQueueNodeT
{
    /*data type of anchor, stored in every List Node*/
    LTE_SNODE     nodeAnchor;
    UInt16        ueIndex;
    UInt16        transactionId;
    /* SPR 15909 fix start */
    tickType_t        tti;
    /* SPR 15909 fix end */
}MacResetQueueNode;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/*SPR 16855 Fix Start*/
#define     UL_SHORT_SR_MAP_INDICATION 1
#define     UL_PUCCH_MAP_INDICATION    2
#define     UL_SHORT_SRS_MAP_INDICATION 4    
#define     UL_LONG_SRS_MAP_INDICATION 8
#define     DL_MEAS_GAP_80MS_MAP_INDICATION 1
#define     DL_MEAS_GAP_40MS_MAP_INDICATION 2
#define     DL_DRX_INDICATION  4
#ifdef ENDC_ENABLED
/*Meas_Gap_Changes_Start*/
#define		DL_MEAS_GAP_20MS_MAP_INDICATION 16
#define		DL_MEAS_GAP_160MS_MAP_INDICATION 32
/*Meas_Gap_Changes_End*/
#endif
/*SPR 16855 Fix End*/
/* TDD_Warning_Fix */
/*SPR 4503 CHANGES*/
#define     RECONFIG_RELEASE_TPC_REQ   1
#define     DELETE_UE_REQ              0
/*SPR 4503 CHANGES*/
/* TDD_Warning_Fix */
#define INVALID_TRANSACTION_ID      65537 /*Transaction ID Max  Value is 65536*/
/* +- SPR 16738 */
#define INVALID_PRECODING_INDEX      255
#define UE_CONTEXT_DEL_DELAY        2
#define MAC_DELETION_PENDING        2
#define INVALID_SIMULTANEOUS_ACK_NACK_VALUE 255
#define UE_CONF_MGR                "UEConfMgr"
#define MAX_NUM_PENDING_RESET_UES  50

/* CLPC_CHG */
#define MAX_TPC_INDEX_FOR_3A 31
#define MAX_TPC_INDEX_FOR_3 15
/* CLPC_CHG */
/*Soft Buffer Changes Start */
#define MAC_CODING_FACTOR 1
/*Soft Buffer Changes End */
#ifdef TDD_CONFIG
#define UL_CONTAINER_0      0
#define UL_CONTAINER_1      1   
#define UL_CONTAINER_2      2
#define UL_CONTAINER_3      3
#define UL_CONTAINER_4      4
/* TDD Config 0 Changes Start */
#define UL_CONTAINER_5      5
/* TDD Config 0 Changes End */
#endif

typedef enum SpsCrntiStatusT
{
    SPS_CRNTI_FREE = 0,
    SPS_CRNTI_BUSY = 1
} SpsCrntiStatus;

/****************************************************************************
 * Exported Variables
****************************************************************************/

/* Following structure defines the SPS-CRNTI array node and its status */
typedef struct SpsCrntiNodeT
{
    /* SPS CRNTI to be allocated */
    UInt16          spsCrnti;    
    /* Flag if SPS CRNTI Free/busy */
    SpsCrntiStatus  idxStatus;
} SpsCrntiNode, *LP_SpsCrntiNode;

/* Following structure defines the SPS CRNTI List */
typedef struct SpsCrntiContextT
{
    /* Last Index which is allocated to SPS UE */
    UInt16           lastCrntiAllocIndex;
    /* Gives total number of SPS CRNTIs that can be allocated */
    UInt16           totalSpsCrntiAllocated;
    /* Array containing SPS CRNTI and its status */
    LP_SpsCrntiNode  spsCrntiList_p;
} SpsCrntiContext, *LP_SpsCrntiContext;

extern SpsCrntiContext spsCrntiMap_g[MAX_NUM_CELL];

/*CA Changes start  */
extern LTE_SQUEUE* pendingUEDeleteQueue_gp[MAX_NUM_CELL];
/*CA Changes end  */
extern DRX_Q_TYPE dlDRXQueue_g;

/*Node structure of the DRX Queues */
typedef struct DRXQueueNodeT
{
    /*CAUTION: Required circular queue enqueue changes if Anchor node moved down in the structure */
    LTE_SNODE   dRXQueueNodeAnchor;
        /*UE ID of the UE*/
     UInt16      ueIdx;
}DRXQueueNode;

/* for UE Entity Reset Response */
typedef struct UEEntityResetResT
{
    UInt16 idx;
    UInt16 response;
    UInt16 transactionId;
}UEEntityResetRes;
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* SPR 19288 change start */
/* SPR 19288 change end */
/*CA Changes start  */
void initUEConfMgr(UInt8 numOfCells);
void cleanupUEConfMgr(InternalCellIndex internalCellIndex);
void deInitUEConfMgr(InternalCellIndex internalCellIndex);
MacRetType deleteUEContext( UInt32 ueIndex
                            ,InternalCellIndex internalCellIndex);
/* Cyclomatic_complexity_changes_start */
    /* CA changes Start */
void deleteUlUeScellContext(ULUEContext *);
    /* CA changes End */
void deleteDlLcStrategyNode(DLUEContext *);
void deleteUlLcStrategyNode(ULUEContext *);
void deleteUlLcStrategyNodeFromAmbrInfo(ULUEContext *);
void deleteFromNonZeroBsrList(ULUEContext *,InternalCellIndex);
void deleteMcsToPrbMap(ULUEContext *);
/* Cyclomatic_complexity_changes_end */

/*CA Changes end  */
/* For common API Downlink /Uplink UE Context create, reConfigure, delete */

/* Create UE context(Downlink/Uplink) */
MacRetType createMACUEEntity( UeContextForDownlink *dwlkUEInfo_p,
                              UeContextForUplink   *uplkUEInfo_p,
                              UInt32 uplkLCCount,
                              /* +- SPR 17777 */
                              InternalCellIndex internalCellIndex
                              /*SPR 16855 Fix Start*/
                              , UInt32 *createULUEBitMap_p,
                              UInt32 *createDLUEBitMap_p,
                              UInt32 currSFN, UInt32 currSF
                              /*SPR 16855 Fix End*/
                             );

/* Resetting UpLink UE Context */
/*CA Changes start  */
void resetMACULUEEntity(ULUEContext* ueULContext_p, InternalCellIndex internalCellIndex);

/* Resetting Downlink UE Context */
MacRetType resetMACDLUEEntity(DLUEContext* ueDLContext_p, InternalCellIndex internalCellIndex);
/*CA Changes end  */

/* CA_phase2_harq_code Start */  

MacRetType macDeleteScellMACDLHarqEntity (DLUEContext*  ueContext_p, 
        InternalCellIndex scellIndex, InternalCellIndex internalCellIndex);

/* CA_phase2_harq_code End */ 
/* pdcch changes start */
void setUeSearchSpaceInfoforScell(ULUEContext *ULUEContext_p,DLUEContext *DLUEContext_p ,
		RNTIInfo *rntiInfo_p,InternalCellIndex internalCellIndex);
/* pdcch changes end */
/* Delete UE Context (Downlink/Uplink) */
/*+COVERITY 5.3.0 - 32260*/
UInt32 deleteMACUEEntity( UInt32 ueIndex,
/*-COVERITY 5.3.0 - 32260*/
                              UInt32 transactionId,
                              InternalCellIndex internalCellIndex
                              );

/* ReConfigure the UE Context (Downlink/Uplink)*/
/*+COVERITY 5.3.0 - 32264*/
UInt32 reconfigureMACUEEntity( UeContextForDownlink *dwlkUEInfo_p,
                                   UeContextForUplink   *uplkUEInfo_p,
                                   UInt32 uplkLCCount
                                   /*CA Changes start */
                                   ,InternalCellIndex internalCellIndex
                                   /*CA Changes end */
                                   /*-COVERITY 5.3.0 - 32264*/
                                   /*SPR 16855 Fix Start*/
                                   ,ULReconfigUeReq *ulReconfigUeReq_p,
                                    DLReconfigUeReq *dlReconfigUeReq_p,
                                    UInt32 currSFN, UInt32 currSF,
                                    UInt8 gapPatternId,
                                    UInt8 *gapBitMask,
                                    UeContextForReconfig *reconfigInfo_p 
                                    /*SPR 16855 Fix End*/
                                 );
/*Reconfig Changes End */
/*CA Changes start  */
/* +- SPR 17777 */
MacRetType processPendingDeleteUE(
#if (defined(FAPI_1_1_COMPLIANCE) || defined (FAPI_4_0_COMPLIANCE)) && defined(TDD_CONFIG)
        tickType_t currentGlobalTick,
#endif
        InternalCellIndex internalCellIndex);        
/* +- SPR 17777 */

/*CA Changes end  */
MacRetType processPendingResetUE(tickType_t currentTTI, InternalCellIndex internalCellIndex);
/* SPR 15909 fix end */
MacRetType updateDRXInDLUEContext( UInt32 ueIndex,InternalCellIndex internalCellIndex );
           /* SPR 5051 Fix Start */
/*CA Changes start  */
/* + SPR_17858_633_CHANGES */
void deleteUECnf(UInt32 ueIndex, UInt32 transactionId,
                 UInt32 ueId, UInt16 tempContextIndex, UInt32 rrcUeIndex,InternalCellIndex internalCellIndex);
 /* - SPR_17858_633_CHANGES */
/*CA Changes end  */
           /* SPR 5051 Fix End */

void checkAndSetHighPriorityScheduling(UInt32 ueIndex);
/* + CQI_4.1 */
void initBandwidthPart(ULUEContext *newULUEContext_p,
    InternalCellIndex internalCellIndex);
UInt8 getCQIFromMCS(UInt8 mcsValue);
/* - CQI_4.1 */

extern void putEntryInPendingDeleteUEQueue(UInt32 ueIndex, 
                                           UInt32 transactionId, 
                                           /* SPR 15909 fix start */
                                           tickType_t        tti,
                                           /* SPR 15909 fix end */
                                           /*+COVERITY 5.3.0 - 52874*/
                                           RNTI rntiFlag
                                           /*-COVERITY 5.3.0 - 52874*/
                                           /*CA Changes start */
                                           ,InternalCellIndex internalCellIndex
                                           /*CA Changes end */
        );

extern void putEntryInMacResetQueue(UInt32 ueIndex,
        UInt32 transactionId,
        /* SPR 15909 fix start */
        tickType_t        tti
        /* SPR 15909 fix end */
                                   );
/* SPR 609 changes start */
/*SPR 16855 Fix Start*/
MacRetType configureDedicatedSRSConfig(UInt16 ueIndex,
                                       SRSConfigInfo *srsConfigInfo,
                                       SRSDedicatedSetupInfo   *dedicatedSetupInfo_p,
                                       UInt8   srsUEConfigType, 
                                       UInt32 currSFN, UInt32 currSF,
                                       InternalCellIndex internalCellIndex);
/*Reconfig Changes Start */
MacRetType reconfigureDedicatedSRSConfig(UInt16 ueIndex,SRSConfigInfo *srsConfigInfo,
                    SRSDedicatedSetupInfo   *dedicatedSetupInfo_p, UInt8 newConfigType,
                    UInt32 currSFN, UInt32 currSF, 
                    UeContextForReconfig *reconfigInfo_p,
                    InternalCellIndex internalCellIndex);
void deleteDedicatedSRSConfig(SRSConfigInfo *srsConfigInfo,UeContextForReconfig *reconfigInfo_p,
                              /*CA Changes start  */
                              InternalCellIndex internalCellIndex);
                              /*CA Changes end  */
/*Reconfig Changes End */
UInt32 deleteUEMeasgapContext  (UInt16 ueIndex, UInt32 *deleteDLUEBitMap,
                                        InternalCellIndex internalCellIndex);
/** SPR 12320 Fix : START **/
UInt32 deleteUEPucchSRSContext (UInt16 ueIndex, UInt32 *deleteULUEBitMap,UeCellInfo * ueCellInfo_p,
/** SPR 12320 Fix : END **/
                                InternalCellIndex internalCellIndex);
UInt32 deleteUEContextLPThd (UInt16 ueIndex,
                             /*CA Changes start */
                             InternalCellIndex internalCellIndex);
                             /*CA Changes end */

/* SPR 11215 Changes Start */
#if defined(FAPI_1_1_COMPLIANCE) || defined(FAPI_4_0_COMPLIANCE)
/* SPR 11215 Changes End */
/* SPR 15909 fix start */
/* +- SPR 17777 */
#ifdef FDD_CONFIG
UInt32 deleteUEContextUlHiPrioThd(UInt8 ulDelay, tickType_t currentGlobalTick,
                                  InternalCellIndex internalCellIndex);
#else
UInt32 deleteUEContextUlHiPrioThd(tickType_t currentGlobalTick,
                                  InternalCellIndex internalCellIndex);
#endif
/* +- SPR 17777 */
/* SPR 15909 fix end */
#else
UInt32 deleteUEContextUlHiPrioThd (InternalCellIndex internalCellIndex);
#endif

UInt32 deleteUEContextDlHiPrioThd (InternalCellIndex internalCellIndex);
/*BUG:9307:START*/
void updateTotalActiveUEs(UInt16 ueIndex,
        InternalCellIndex internalCellIndex);
/*BUG:9307:END*/
/*SPR 16855 Fix End*/

/* SPR 5599 changes start (ZIP ID 129059) */
 MacRetType createMACDLUEConnHandOverEntity(UInt8 preambleIndex,
/* SPR 5599 changes end (ZIP ID 129059) */
                                                         UInt16 ueIndex,
                                                         UInt16 *tcRNTI_p
                                                         /* CA changes Start */
                                                         , InternalCellIndex internalCellIndex
                                                         /* CA Changes end */
         );


 /* SPR 5599 changes start (ZIP ID 129059) */
/*CA Changes start  */
extern MacRetType deleteMACDLUEConnHandOverEntity(UInt16 ueIndex, InternalCellIndex internalCellIndex);
/*CA Changes end  */
/* SPR 5599 changes end (ZIP ID 129059) */

/*+COVERITY 5.3.0 - 32261*/
UInt32 resetMacUEEntity(UInt16 ueIndex, UInt16 transactionId
        /* CA changes Start */
        ,InternalCellIndex internalCellIndex
        /* CA Changes end */
        );
/*-COVERITY 5.3.0 - 32261*/
/* SPR 5599 changes start (ZIP ID 129059) */
/*CA Changes start  */
MacRetType updateCrntiToIndexTable(ChangeCrntiInfo changeCrntiInfo, InternalCellIndex internalCellIndex);
/*CA Changes end  */

extern MacRetType reconfigureTpcPdcchConfigInfoForPucch(
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt8 release,
        UInt16 rnti,
        UInt8 tpcBitMapIndex,
        UInt8 dciFormat,
        DLUEContext *ueContext_p,
        InternalCellIndex internalCellIndex);

/*SPR 4503 CHANGES*/
/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType deleteTpcPdcchConfigInfoForPucch(
/* SPR 5599 changes end (ZIP ID 129059) */
         UInt8 release,
         /* +- SPR 17777 */
         DLUEContext *ueContext_p,
         /*CA Changes start  */
         InternalCellIndex internalCellIndex);
         /*CA Changes end  */
/*SPR 4503 CHANGES*/

/* Delete uplink Logical channel */
/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType deleteULUELogicalChannel( ULUEContext *ueContext_p,
/* SPR 5599 changes end (ZIP ID 129059) */
                                             UInt8 ueLCId,
                                             /*CA Changes start  */
                                             InternalCellIndex internalCellIndex
                                             /*CA Changes end  */
                                          );

/* ReConfigure the Uplink Logical Channel*/
/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType reconfigULUELogicalChannel( ULUEContext *ueContext_p,
/* SPR 5599 changes end (ZIP ID 129059) */
                                              ulLogicalChannelInfo* logicalChInfo_p,
                                              /*CA Changes start  */
                                              InternalCellIndex internalCellIndex
                                              /*CA Changes end  */
                                              );


/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType addULUELogicalChannel( ULUEContext* newUEContext_p,
/* SPR 5599 changes end (ZIP ID 129059) */
                                                UInt32 logicalChCount,
                                                ulLogicalChannelInfo * logicalChInfo_p
                                                /* CA changes Start */
                                                ,InternalCellIndex internalCellIndex
                                                /* CA changes End */

        );

/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType reconfigureTpcPdcchConfigInfoForPusch(
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt8 release,
        UInt16 rnti,
        UInt8 tpcBitMapIndex,
        /* Coverity 54156 Fix Start */
        DCIFormat dciFormat,
        /* Coverity 54156 Fix End */
        ULUEContext *ueContext_p,
        InternalCellIndex internalCellIndex);

/*SPR 4503 CHANGES*/
/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType deleteTpcPdcchConfigInfoForPusch(
/* SPR 5599 changes end (ZIP ID 129059) */
         UInt8 release,
         /* +- SPR 17777 */
         ULUEContext *ueContext_p,
         /*CA Changes start  */
         InternalCellIndex internalCellIndex);
         /*CA Changes end  */
/*SPR 4503 CHANGES*/

/* Delete Downlink LC */
/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType deleteDLUELogicalChannel( DLUEContext *ueContext_p,
/* SPR 5599 changes end (ZIP ID 129059) */
                                            UInt8 ueLCIdi,
                                            InternalCellIndex internalCellIndex
                                          );

/* ReConfigure the Downlink Logical Channel*/
/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType reconfigureMACDLLogicalChannel( DLUEContext* newUEcontext_p,
/* SPR 5599 changes end (ZIP ID 129059) */
                                                         LogicalChannelConfigInfo* logicalChConfig_p,
                                                         InternalCellIndex internalCellIndex
                                                       );

/* ADD Downlink Logical Channel*/
/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType addMACDLLogicalChannel( DLUEContext* newUEContext_p,
/* SPR 5599 changes end (ZIP ID 129059) */
                                                UInt32 logicalChCount,
                                                 LogicalChannelConfigInfo* logicalChConfig_p,
                                                 InternalCellIndex internalCellIndex
                                               );
/* + DRX_CHG */
/*SPR 16855 Fix Start*/
extern void processDRXConfig(DRXConfigInfo *drxConfigTmp_p,/* SPR 2466 Changes */
            /* SPR 21043 Start-End */
        DLUEContext *dlUEContext_p, 
        ULUEContext *ulUEContext_p,
        /*CA Changes start */
        InternalCellIndex internalCellIndex);
        /*CA Changes end */
extern void processDRXReConfig(DRXConfigInfo *drxConfigTmp_p,/* SPR 2466 Changes */
            /* SPR 21043 Start-End */
        DLUEContext *dlUEContext_p,
        ULUEContext *ulUEContext_p,
        /*CA Changes start */
        InternalCellIndex internalCellIndex);
        /*CA Changes end */
/*SPR 16855 Fix End*/
extern void deleteDRXConfig(DLUEContext *dlUEContext_p, 
                            ULUEContext *ulUEContext_p,
                            /*CA Changes start  */
                            InternalCellIndex internalCellIndex);
                            /*CA Changes end  */
/* - DRX_CHG */
/* + MEAS_GAP_CHG */
/*SPR 16855 Fix Start*/
extern void processMeasGapConfig (UeContextForDownlink *dwlkUEInfo_p, 
                             MeasGapContextDL *dlMeasGapContext,
                             MeasGapContextUL *ulMeasGapContext,
                             UInt8 *tempDLmeasGapConfigType,
                             UInt8 *tempULmeasGapConfigType,
                             InternalCellIndex internalCellIndex);
extern void processMeasGapReConfig (UeContextForDownlink *dwlkUEInfo_p,
                             MeasGapContextDL *dlMeasGapContext,
                             MeasGapContextUL *ulMeasGapContext,
                             UInt8 *dlMeasGapConfigType,
                             UInt8 *ulMeasGapConfigType, 
                             UInt16 ueIndex,
                             UInt8 gapPatternId,
                             UInt8 *gapBitMask,
                             /*CA Changes start */
                             InternalCellIndex internalCellIndex);
                             /*CA Changes end */
extern void deleteUEMeasGapConfig (MeasGapContextDL *dlMeasGapContext_p,
                             MeasGapContextUL *ulMeasGapContext_p,
                             UInt8 *dlMeasGapConfigType,
                             UInt8 *ulMeasGapConfigType,
                             UInt16 ueIndex,
                             UInt8 gapPatternId,
                             /*CA Changes start  */
                             InternalCellIndex internalCellIndex);
                             /*CA Changes end  */

/*SPR 16855 Fix End*/

/* -  MEAS_GAP_CHG */

/* SPR 3271 changes start */
/*CA Changes start  */
extern void initUEULCSIContext( ULUEContext *ulUEContext_p, UInt8 minMcsIndex, InternalCellIndex internalCellIndex); 
            
extern void initPucchBlerContext(DLUEContext *newUEcontext_p, InternalCellIndex internalCellIndex);
/* SPR 5599 changes start (ZIP ID 129059) */
extern UInt8 getEffectiveMCS(UInt8 sinr, UInt8 ueCategory, 
/* SPR 5599 changes end (ZIP ID 129059) */
        /* +- SPR 18268 */
                               SInt32 sinrCorrectionFactor,UInt16 ueIndex, /*SPR_4001_CHANGES*/
                               InternalCellIndex internalCellIndex);
/*CA Changes end  */
/* SPR 5599 changes start (ZIP ID 129059) */
/* +- SPR 17777 */
extern void resetTpcPdcchConfigInfoForPucch(
/* SPR 5599 changes end (ZIP ID 129059) */
        DLUEContext *ueContext_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex);
/*CA Changes end  */
/* +- SPR 17777 */
/* SPR 5599 changes start (ZIP ID 129059) */
extern void resetTpcPdcchConfigInfoForPusch(
        /* SPR 5599 changes end (ZIP ID 129059) */
        ULUEContext *ueContext_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex);
/*CA Changes end  */
/* +- SPR 17777 */
/* SPR 3271 changes end */

/* SPR 609 changes end */

/* + CQI_5.0 */
extern void resetCQIInfo(DLUEContext *ueContext_p,
                         DLCQIInfo *dlCQIInfo_p,
                         InternalCellIndex internalCellIndex);

#ifdef FDD_CONFIG
MacRetType macFillScellDLCtxtCqiInfo_dummy(UInt32               ueIndex,
                            UeContextForDownlink *ueDlConfigInfo_p,
                            DLUEContext          *newUEcontext_p,
                            /* SPR 15909 fix start */
                            tickType_t currentTick,
                            /* SPR 15909 fix end */
                            /* +- SPR 17777 */
                            UInt8                cellId);
#endif
extern SInt16 dlBlerCorrectionFactor_g[DL_BLER_CORRECTION_FACTOR_TABLE_SIZE];
/* - CQI_5.0 */

/* SPR 9427 changes start */
extern void initNonSpatialRAT0RAT1Bitmaps(UInt8 cqiValue,
                                   CQIInfoNonSpatialMul *cqiInfoNonSpatialMul_p,
                                   InternalCellIndex internalCellIndex);

extern void initSpatialRAT0RAT1Bitmaps(UInt8 cqiIndex,
                                CQIInfoSpatialMul *cqiInfoSpatialMul_p,
                                InternalCellIndex internalCellIndex);
extern void initNumOfSBperBWpart(DLCQIInfo *dlCQIInfo_p,
        InternalCellIndex internalCellIndex);
/* SPR 9427 changes end */
/* SPR 16102 Changes Start */
/* SPR 16994 changes - START */
extern void setSPSMeasGapTTI( MeasGapContextDL *dlMeasGapContext_p,
                              MeasGapContextUL *ulMeasGapContext_p,
                              UInt16 ueIndex, InternalCellIndex internalCellIndex);
/* SPR 16994 changes - END */
/* SPR 16102 Changes Start */
/* SPR 11940 start*/
/** HD FDD Changes **/
#ifdef HD_FDD_CONFIG
 void deleteUeFromHdFddContext(DLUEContext* ueDLContext_p
        /* CA changes Start */
                ,InternalCellIndex internalCellIndex
                        /* CA Changes end */
                        );
 void hdFddCtxConfigReconfigUpdate(UInt16 ueIndex);
 void hdFddCtxReconfigUpdateReports(ULUEContext* ulUEContext_p,
                            InternalCellIndex internalCellIndex);
#endif
/** HD FDD Changes **/
/* SPR 11940 end*/
/* SPS_CHG */
extern UInt16 spsIntervalMap_g[SPS_INTERVAL_MAX];

/****************************************************************************
 * Function Name  : deAllocateSpsCrnti
 * Inputs         : spsCrnti - SPS Crnti that needs to be removed
 * Outputs        :
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function deallocates SPS CRNTI allocated to SPS UE.
 ****************************************************************************/
UInt16 deAllocateSpsCrnti( UInt16 spsCrnti ,InternalCellIndex internalCellIndex);

/****************************************************************************
 * Function Name  : initializeSpsCrntiMap
 * Inputs         : spsCrntiStartRange - SPS Crnti start range
 *                  spsCrntiEndRange -  SPS Crnti end range
 * Outputs        :
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function initializes global SPS CRNTI map.
 ****************************************************************************/
MacRetType initializeSpsCrntiMap(
        UInt16 spsCrntiStartRange,
        UInt16 spsCrntiEndRange 
        /* CA changes Start */
        , InternalCellIndex internalCellIndex
        /* CA Changes end */
        );

/*****************************************************************************
 * Function Name  : processUEReconfigCompInd
 * Inputs         : ueReconfigCompInd_p - Pointer to UEReconfigCompInd
 * Outputs        :
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This method will parse the passed buffer that is received
 *                  from RRC when SPS reconfiguration is happening in
 *                  RRCConnectionReconfiguration or RRCConnectionSetup message.
 *****************************************************************************/
/*CA Changes start  */
MacRetType processUEReconfigCompInd(UEReconfigCompInd *ueReconfigCompInd_p, InternalCellIndex internalCellIndex);
void macDeleteDLScellContext(DLUEContext * dlUeContext_P, UInt8 scellId, InternalCellIndex internalCellIndex);
void macInitBandwidthScellPart(ULUESCellContext *newULUEScellContext_p,
    InternalCellIndex internalCellIndex);
extern void macResetScellCQIInfo(DLUESCellContext *ueScellContext_p,
                         DLCQIInfo *dlCQIInfo_p
                         );
/* CA_phase2_harq_code Start */  
void deleteDLScellContext(DLUEContext * dlUeContext_P, UInt8 scellId);
/* CA_phase2_harq_code End */  
/*CA Changes end  */
/* SPS_CHG */
extern InternalCellIndex getServeCellIndexFromInternalCellId( DLUEContext* ueDLContext_p,
                                                              InternalCellIndex cellIndex);

/*Complexity Start*/
void updateCqiStats(ULCQIPeriodicReqInfo *cqiPeriodicReqInfo_p, PucchConfig *tempPucchConfigInfo_p,InternalCellIndex internalCellIndex,PeriodicFeedbackType periodicFeedBack,ULReconfigUeReq *ulReconfigUeReq_p);

void updateRiStats(ULCQIPeriodicReqInfo *cqiPeriodicReqInfo_p, PucchConfig *tempPucchConfigInfo_p,InternalCellIndex internalCellIndex,ULReconfigUeReq *ulReconfigUeReq_p);
/*Complexity End*/
/* + SPR 17733 */
#ifdef FDD_CONFIG
/****************************************************************************
 * Function Name  : createMACDLHarqEntity
 * Inputs         : newUEcontext_p - its point to DLUEContext
 *                  hScheme - its point to HarqSchme Structure  
 *                  numHarqProcess - number of harq processes.
 * Outputs        : None
 * Returns        : MacRetType [MAC_FAILURE OR MAC_SUCCESS]
 * Description    : This is called by createMACDLUEEntity while creating the
 *                  UE entities harq context.
 ****************************************************************************/
MacRetType createMACDLHarqEntity( DLHARQEntity **dlHharqContext_pp,
        UInt32 numHarqProcess);
#endif
/* - SPR 17733 */


#endif  /* LTE_MAC_UE_CONF_MGR_H */
