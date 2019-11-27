/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacFAPIApi.c,v 1.1.4.15.4.1 2010/11/08 06:06:22 gur10121 Exp $
 *
 ******************************************************************************
 *
 *  File Description : The file contains API implementations for MAC-PHY 
 *                     interace complied to Femto Forum
 *
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacFAPIApi.c,v $
 * Revision 1.1.4.15.4.1  2010/11/08 06:06:22  gur10121
 * Semi static changes merging
 *
 * Revision 1.1.4.15  2010/10/29 04:38:30  gur20056
 * deepa - warning removal
 *
 * Revision 1.1.4.14  2010/10/25 10:10:05  gur23971
 * warning removed + shashanks change for Dafault values at FAPI fix
 *
 * Revision 1.1.4.13  2010/10/25 06:45:50  gur19942
 * Set default values for the parameters not coming from RRC.
 *
 * Revision 1.1.4.12  2010/10/20 08:45:11  gur20491
 * compilation error removed
 *
 * Revision 1.1.4.11  2010/10/20 06:03:26  gur20491
 * Parsing of additional structure from RRC 2.0.1
 *
 * Revision 1.1.4.10  2010/10/15 07:51:53  gur20491
 * Applied uniform precompilation macro for RRC_2_0_1 integration changes
 *
 * Revision 1.1.4.9  2010/10/14 17:25:09  gur19413
 * RRC_2_0_1_INT
 *
 * Revision 1.1.4.8  2010/10/14 12:27:42  gur19942
 * Review comments incorporated.
 *
 * Revision 1.1.4.7.2.4  2010/10/21 09:16:09  gur10121
 * UE config put under MAC_SEMI_STATIC_INF flag
 *
 * Revision 1.1.4.7.2.3  2010/10/21 07:53:13  gur10121
 * Semi static UT fixes
 *
 * Revision 1.1.4.7.2.2  2010/10/19 06:46:34  gur10121
 * Compilation changes checked in
 *
 * Revision 1.1.4.7.2.1  2010/10/18 14:37:20  gur19942
 * semistatic for ulconfig.
 *
 * Revision 1.1.4.7  2010/10/12 04:48:47  gur10121
 * Kloc work warning
 *
 * Revision 1.1.4.6  2010/10/11 12:59:02  gur10121
 * SPR #706 fix
 *
 *
 * Revision 1.1.4.5  2010/10/08 12:59:26  gur19942
 * Changes related to putting UL SCH pdu in q and sending some tick later.
 * Fapi UT fixes.
 * 
 * Revision 1.1.4.4  2010/10/07 09:35:17  gur10694
 *
 * Revision 1.1.4.3  2010/09/27 16:49:15  gur10121
 * SPR 662 Fix
 *
 * Revision 1.1.4.2  2010/09/27 14:37:55  gur10694
 * UE config Fix and Review comments
 *
 * Revision 1.1.4.1  2010/09/21 15:46:07  gur20491
 * FAPI changes
 *
 * Revision 1.1.2.2  2010/09/08 14:17:00  gur20439
 * FAPI UL Changes merged
 *
 * Revision 1.1.2.1  2010/09/07 11:21:07  gur20439
 * new files added for FAPI changes
 *
 * Revision 1.1.2.3  2010/08/30 10:02:07  gur10694
 * Aligned varible names with coding guidelines
 *
 * Revision 1.1.2.2  2010/08/20 12:04:48  gur10694
 * UL config code ci
 *
 * Revision 1.1.2.1  2010/08/13 08:52:49  gur10694
 * initial creation
 *
 *****************************************************************************/
/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/


/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteLayer2CommanTypes.h"
#include "lteFapiMacPhyInterface.h"
#include "lteMacFAPIApi.h"
#include "lteMacPhyInterface.h"
#include "lteMacParseUtil.h"
#include "lteMacCellConfMgr.h"
#include "lteMacUtilityAndSharedStructs.h"
#include "lteMacPUSCH.h"
#include "lteMacParseOAMMsg.h"
#include "lteMacULSchedulerStrategy.h"
#include "lteMacDLHarqMgr.h"
#ifdef TDD_CONFIG
#include "lteMacTddMgr.h"
#ifdef OTA_DL_ADVANCE_2
#include "lteMacPUCCH.h" 
#endif
#endif

#ifdef FLEXRAN
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#ifdef IPC_SEMAPHORE
#include <fcntl.h>			 /* For O_* constants */
#include <sys/stat.h>		 /* For mode constants */
#include <semaphore.h>
#include <errno.h>
#endif

#define MAX_SEGMENTS 4
extern UInt32 g_flexran_channelId[MAX_NUM_CELL] ; 
UInt8 *g_pSharedMemAddrSegments[FLEXRAN_MAX_CELL_NUM][MAX_SEGMENTS] = {{PNULL}};
#endif

#include "lteMacPhyCellReconfig.h"
#include "ltePerfStats.h"
#include "lteMacStatsManager.h"
/* SPR 19288 change start */
#ifdef FLEXRAN
extern void sendMacInitLayerRes(UInt16 transactionId, UInt32 response,
        InternalCellIndex internalCellIndex);
extern void lteMacMacInitLayerPhyHandler(
        void * msgBuf, SInt32 byteRead,InternalCellIndex internalCellIndex );
extern void lteMacStartPhyHandler(
        void * msgBuf, SInt32 byteRead,InternalCellIndex internalCellIndex);
extern void sendConfigPhyCellRes(UInt16 transactionId, UInt32 response,
        InternalCellIndex internalCellIndex);
extern void lteMacConfigCellReqPhyHandler(
        void * msgBuf, SInt32 byteRead,InternalCellIndex internalCellIndex);
extern void initWiresharkLoggingInterface(InternalCellIndex internalCellIndex);
extern UInt32 transactionId_g[MAX_NUM_CELL] ;
#endif
/* SPR 19288 change end */
#ifdef TDD_CONFIG
#include "lteMacTddMgr.h"
#endif

extern TddPdschTxMapOfDLHarq *tddPdschTxMapOfDLHarq_p[MAX_NUM_CELL];

extern  UInt8 getHarqResource(UInt16 ulK0, UInt8 scIndex);
/****************************************************************************
  Private Definitions
 ****************************************************************************/
/* Start 128 UE: Memory Changes */
#define NUM_OF_BLK_FOR_UL_CONFIG_MSG 6
/* End 128 UE: Memory Changes */
/* SPR 22925 Start */
#define SON_PHY_INTERFACE_API_HEADER_SIZE 40
/* SPR 22925 End */
/* SPR 22925 Start */
#ifdef INTEL_ICC_MODE
#define MAX_NUM_ICC_BLOCKS (16)
#endif
/* SPR 22925 End */



/****************************************************************************
  Private Types
 ****************************************************************************/
#define L1_PARTIAL_API_MSG_SIZE ( sizeof(FAPI_l1ApiMsg_st) - 2)
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
 void getulChannelBW(UInt8 ulChBw,
        InternalCellIndex internalCellIndex);

 void getdlChannelBW( UInt8 dlChBw, 
        InternalCellIndex internalCellIndex);

 void fillPhyCellReconfigParams (
        InternalCellIndex internalCellIndex);

 UInt16 prepareUlConfigPdu(FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo *ulSchUciInfo_p,
        UInt16 pduSize,
        UInt8 isSRSPresent,
        UInt8 *countInvalid,
        InternalCellIndex internalCellIndex);


UInt32 handleStopPhyRes(UInt8 *msgBuf,SInt32 bytesRead);
/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
extern UInt16 getCombinatorialIndex(UInt8 s0,UInt8 s1,UInt8 s2,UInt8 s3);
#endif
/* SPR 21958 PUSCH RAT1 Support End */

#ifdef MSPD_WATCH_DOG
extern  UInt8 is_phy_running; 
#endif
/*SPR 20199 fix start*/
#ifdef WATCHDOG_THREAD
extern UInt8 watchdogflag_g;
#endif
/*SPR 20199 fix end*/
/*SPR 21347 fix start*/
#if defined(FLEXRAN)
eltaAstriParamThreshold *eltaAstriParamUpdate(void);
#endif
/*SPR 21347 fix end*/


/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/
/*
* This structure is used to hold global config info for PHY getting from RRC
* Must be defined extern in execution leg scheduler file also 
*/
/* CA Changes start */
CellConfigPhy *cellConfigPhy_gp[MAX_NUM_CELL] = {0};


/*Indicates the release supported by PHY*/
UInt8 phyReleaseCapability_g=RELEASE_8;
FAPI_dlConfigPDUInfo_st *(*addDciToDLConfigFunction)(FAPI_dlConfigPDUInfo_st *, DCIPduInfo *,DciCCEInfo *,InternalCellIndex)
    =addDciPduToDlConfig;
FAPI_dlConfigPDUInfo_st *(*addDlschToDLConfigFunction)(FAPI_dlConfigPDUInfo_st *, DCIPduInfo *,UInt32 , UInt8 
            , UInt8
            ,FAPI_VendorSpecificForDLConfig_st *,
            UInt16*,
            InternalCellIndex
            )=addDlschPduToDlConfig;
    void (*prepareUciHarqPduFunction)(
            FAPI_ulPDUConfigInfo_st *ulPduConf_p,
            ULSchUciInfo * ulSchUciInfo_p,
            InternalCellIndex
            )=prepareUciHarqPdu;

    void (*prepareUciSrHarqPduFunction) (
            FAPI_ulPDUConfigInfo_st *ulPduConf_p,
            ULSchUciInfo * ulSchUciInfo_p,
            InternalCellIndex
            )=prepareUciSrHarqPdu;
    void (*prepareUciCqiHarqPduFunction)(
            FAPI_ulPDUConfigInfo_st *ulPduConf_p,
            ULSchUciInfo * ulSchUciInfo_p,
            InternalCellIndex
            )=prepareUciCqiHarqPdu;
    void (*prepareUciCqiSrHarqPduFunction)(
            FAPI_ulPDUConfigInfo_st *ulPduConf_p,
            ULSchUciInfo * ulSchUciInfo_p,
            InternalCellIndex)=prepareUciCqiSrHarqPdu;


/*SPR 15909 Fix Start*/
void (*processULSchIndFunction)(void *,
        void *,
        void *,
        UInt32,
        tickType_t,
        UInt32,
        UInt32,
        UInt32,
        InternalCellIndex
/* SPR 19288 change start */
/* SPR 19288 change end */
        )=processULSchInd;
void (* processRACHIndicationFunction) (void *,
        tickType_t,FAPI_VendorSpecificForRACHInd_st*,InternalCellIndex)=processRACHIndication;

void (*processSRSIndicationFunction)(void *,
        void *,
        tickType_t,
        InternalCellIndex)=processSRSIndication;
/* specific to Broadcom  */


#ifdef FAPI_4_0_COMPLIANCE
void (* processCQIIndicationFunction)(void *,
        void *,
        tickType_t,
        /*SPR 5620 START*/
#ifdef TDD_CONFIG
        UInt16,
#endif
        /*SPR 5620 END*/
        UInt16,
        InternalCellIndex
        )=processCQIIndicationR8;
/*BCRM +*/
/*BCRM -*/
#else/*end of declarartion for  case FAPI_4_0_COMPLIANCE defined*/
void (* processCQIIndicationFunction)(void *,
        void *,
        tickType_t,
        /*SPR 5620 START*/
#ifdef TDD_CONFIG
        UInt16,
#endif
        /*SPR 5620 END*/
        UInt16,
        InternalCellIndex
        )=processCQIIndication;
#endif /*end of declarartion for case  FAPI_4_0_COMPLIANCE not defined*/
/* - TM7_8 Changes End */
/*FAPI2_1 changes start*/
#ifndef FAPI_4_0_COMPLIANCE
void (* processHARQIndicationFunction)(void *,tickType_t,UInt16,UInt8,InternalCellIndex)=processUlHarqInd;
#else/*end of declarartion for  case FAPI_4_0_COMPLIANCE not defined*/
void (* processHARQIndicationFunction)(void *,tickType_t,UInt16,UInt8,InternalCellIndex)=processUlHarqIndR8;
#endif /*end of declarartion for  case FAPI_4_0_COMPLIANCE defined*/
/*FAPI2_1 changes end*/
/*BCRM +*/
/*BCRM -*/
/* SPR 15909 fix end */
 void fillDciFormat0Pdu(
#ifdef TDD_CONFIG
        FAPI_dlDCIPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu,UInt32 dci0ForTempUEFlag, InternalCellIndex internalCellIndex);
#elif FDD_CONFIG
/* +- SPR 17777 */
FAPI_dlDCIPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu,
#if defined(UTFWK_SIMULATION) || defined(ULHARQ_TIMER_PROC) /*SPR 18979 +-*/
UInt32 subFrameNum,
#endif
UInt32 dci0ForTempUEFlag,MsgType msgType,InternalCellIndex);
/* +- SPR 17777 */
#endif

#ifdef TDD_CONFIG
void (*fillDciFormat0PduFunction)(FAPI_dlDCIPduInfo_st *, DCIPduInfo *, UInt32, InternalCellIndex)=fillDciFormat0Pdu;
#elif FDD_CONFIG
/* +- SPR 17777 */
#if defined(UTFWK_SIMULATION) || defined(ULHARQ_TIMER_PROC) /*SPR 18979 +-*/
void (*fillDciFormat0PduFunction)(FAPI_dlDCIPduInfo_st *, DCIPduInfo *, UInt32, UInt32, MsgType,InternalCellIndex)=fillDciFormat0Pdu;
#else
void (*fillDciFormat0PduFunction)(FAPI_dlDCIPduInfo_st *, DCIPduInfo *, UInt32, MsgType,InternalCellIndex)=fillDciFormat0Pdu;
#endif
/* +- SPR 17777 */
#endif

MacRetType (*prepareUlschPduFunction)(FAPI_ulSCHPduInfo_st *,ULSchUciInfo *,UInt8,InternalCellIndex)=prepareUlschPdu;
#ifdef FAPI_4_0_COMPLIANCE
MacRetType (*prepareUlschCqiRiPduFunction) (FAPI_ulPDUConfigInfo_st *,
        ULSchUciInfo *,UInt8,InternalCellIndex);
#else/*end of declarartion for  case FAPI_4_0_COMPLIANCE defined*/
MacRetType (*prepareUlschCqiRiPduFunction) (FAPI_ulPDUConfigInfo_st *,ULSchUciInfo *,UInt8,InternalCellIndex)=prepareUlschCqiRiPdu;
#endif/*end of declarartion for  case FAPI_4_0_COMPLIANCE not defined*/

/* FAPI2.4 Changes Start */
/* +- SPR 17777 */
void (*lteMacUpdateFapiCqiIndStatsFunction)(UInt8 *pBufferToSend,
       InternalCellIndex internalCellIndex)=lteMacUpdateFapiCqiIndStats;
void (*lteMacUpdateFapiRxUlschStatsFunction)(UInt8 *pBufferToSend,
       InternalCellIndex internalCellIndex)=lteMacUpdateFapiRxUlschStats;
void (*lteMacUpdateFapiRachIndStatsFunction)(UInt8 *pBufferToSend,
        InternalCellIndex internalCellIndex)=lteMacUpdateFapiRachIndStats;
/* +- SPR 17777 */
#ifdef FAPI_4_0_COMPLIANCE
void (*lteMacUpdateFapiHarqIndStatsFunction )(void *msg_p,
        InternalCellIndex internalCellIndex)= lteMacUpdateFapiHarqIndStatsR8;

#else/*end of declarartion for  case FAPI_4_0_COMPLIANCE defined*/
void (*lteMacUpdateFapiHarqIndStatsFunction )(void *msg_p,
        InternalCellIndex internalCellIndex)= lteMacUpdateFapiHarqIndStats;

#endif/*end of declarartion for  case FAPI_4_0_COMPLIANCE not defined*/
/* FAPI2.4 Changes End */
MacRetType (*prepareUlschCqiHarqRiPduFunction)(
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p
        /*SPR 1547 Start*/
        ,UInt8 srsPresent,
        /*SPR 1547 End*/
        InternalCellIndex
        )
=prepareUlschCqiHarqRiPdu;

/* This Structure is used to hold PHY Reconfig Parameter After Stop Process */
ReConfigPhyCellMajor reconfigCellMajor_g[MAX_NUM_CELL] = {{0}} ;
/* CA Changes end */
extern  ReconfigPhyCell reconfigPhyCell_g[MAX_NUM_CELL];
UDouble32 ulBandwidth_g[MAX_NUM_CELL];
UDouble32 dlBandwidth_g[MAX_NUM_CELL];
/* CA Changes start */
/* SPR 1308 changes start */
FAPI_paramRespParam_stT phyParamResp_g[MAX_NUM_CELL];
/* SPR 1308 changes end */
/* CA Changes end */

/* Container to store the UL_CONFIG Information for Error handling */
UlConfigMap ulConfigMap_g[MAX_NUM_CELL][MAX_UL_CONFIG_CONTAINER_MAP_SIZE];

/* FAPI L2 AS MASTER START */
#ifdef FAPI_L2_MASTER
/*SPR 13047 Fix*/
UInt8 startExecutionAfterPhyStart_g[MAX_NUM_CELL] = {FALSE};
#endif
/* FAPI L2 AS MASTER END */


extern SInt32 sendMacMsgToRRC(UInt8 *msgBuf, UInt16 msgLen, SInt32 flag,
        InternalCellIndex internalCellIndex);
/*4x4 DL MIMO CHG START*/
extern UInt32 determineTBSize(UInt32 MCSIndex, UInt32 numRb,UInt8 tbMappedToNumLayer);
/*4x4 DL MIMO CHG END*/
/*ul config is not a semistatic msg*/
/*Message queue for ulconfig msg */
static ulConfigQueueNode ulConfigReqQueue_g[MAX_NUM_CELL][MAX_SUB_FRAME];
void initUlConfigReqQueue(InternalCellIndex internalCellIndex);
/* SPR 609 changes start */    


void fillHarqInfo ( 
		FAPI_ulSCHHarqInfo_st *ulSchHarqInfo_p,
                      ULSchUciInfo * ulSchUciInfo_p 
/* ULSCH_HARQ_issue_fix + */
#ifdef FLEXRAN
                      , InternalCellIndex internalCellIndex
#endif
/* ULSCH_HARQ_issue_fix - */
                      );
/* SPR 609 changes end */    
MacRetType calcReconfigOffsets(UInt8 *msg_p, UInt16 bitMask,
				SInt16 * offSetArr);

extern UInt8 *pULConfigMsg_gp[MAX_NUM_CELL][MAX_SUB_FRAME] ;
/* Cyclomatic_complexity_changes_start */
void fillPhyReConfigRequestForDuplexingMode(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8* tlvIndex,
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForPbFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8* tlvIndex, 
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForDlCyclicPrefixTypeFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex, 
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForUlCyclicPrefixTypeFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex, 
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForDlChannelBWFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForUlChannelBWFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex, 
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForTxAntennaPortFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex, 
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForRxAntennaPortFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex, 
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForPhichResourceFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex, 
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForPhichDuration(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex, 
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForPrimarySyncSignalFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex, 
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForSecondarySyncSignalFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex, 
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForPhysicalCellIdFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex, 
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForConfigurationIndexFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex, 
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForRootSeqIndexFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForZeroCorelationZoneConfigFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForHighSpeedFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForFreqOffsetFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForHoppingModeFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForHoppingOffsetFlag(FAPI_phyCellConfigRequest_st * FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForNumOfSubBandFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForDeltaPUCCHShiftFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForNCQIRBFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForNAnCsFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForN1PucchAnFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForBandWidthConfigurationFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex);

void fillPhyReConfigRequestForPrsPhyConfigUpdateFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex);

MacRetType parseReconfigPhyCellParamsForTxAntenna(UInt16 subBitMask,
        UInt8 **msgBuf_p,
        UInt16 *remLen,
        UInt8 *numTLV,
        InternalCellIndex internalCellIndex);

MacRetType parseReconfigPhyCellParamsForUlBW(UInt16 subBitMask,
        UInt8 *ulChBandwidth,
        UInt8 **msgBuf_p,
        UInt16 *remLen,
        UInt8 *numTLV,
        InternalCellIndex internalCellIndex);

MacRetType parseReconfigPhyCellParamsForDlBW(UInt16 subBitMask,
        UInt8 *dlChBandwidth,
        UInt8 **msgBuf_p,
        UInt16 *remLen,
        UInt8 *numTLV,
        InternalCellIndex internalCellIndex);

MacRetType reconfigSyncSignalCellParams(UInt16 *remLen, 
        UInt8 *numTLV,
        UInt8 **msgBuf_p,
        InternalCellIndex internalCellIndex);

MacRetType reconfigPrachCellParams(UInt16 *remLen, 
        UInt8 *numTLV,
        UInt8 **msgBuf_p,
        InternalCellIndex internalCellIndex);

MacRetType reconfigPuschCellParams(UInt16 *remLen, 
        UInt8 *numTLV,
        UInt8 **msgBuf_p,
        InternalCellIndex internalCellIndex);

MacRetType reconfigPucchCellParams(UInt16 *remLen, 
        UInt8 *numTLV,
        UInt8 **msgBuf_p,
        InternalCellIndex internalCellIndex);

MacRetType reconfigPhichCellParams(UInt16 *remLen, 
        UInt8 *numTLV,
        UInt8 **msgBuf_p,
        InternalCellIndex internalCellIndex);

MacRetType reconfigPdschCellParams(UInt16 *remLen,
        UInt8 *numTLV,
        UInt8 **msgBuf_p,
        InternalCellIndex internalCellIndex);

MacRetType parseExplicitStartBitMask(UInt16 explicitStartBitMask,
        UInt8 **msgBuf_p,
        UInt16 *remLen_p);

MacRetType parseReconfigPhyCellParamsForCyclicPrefix(UInt16 subBitMask,
        UInt8 **msgBuf_p,
        UInt16 *remLen,
        UInt8 *numTLV,
        InternalCellIndex internalCellIndex);
/* Cyclomatic_complexity_changes_end */
/*Added for Power Control*/
MacRetType checkAndUpdatePDSCHConfigCommonInfo(UInt8 *msg_p,SInt16 *offSetArr, 
        UInt8 *numOfTLV,InternalCellIndex internalCellIndex);
MacRetType checkAndUpdatePcfichPowerOffset(UInt8 *msg_p,SInt16 *offSetArr,
        UInt8 *numOfTLV, InternalCellIndex internalCellIndex);
MacRetType checkAndUpdatePhichPowerOffset (UInt8 *msg_p,SInt16 *offSetArr, 
        UInt8 *numOfTLV, InternalCellIndex internalCellIndex);
MacRetType checkAndUpdateSynchSignals (UInt8 *msg_p,
        SInt16 *offSetArr, UInt8 *numOfTLV, InternalCellIndex internalCellIndex);
/*PHY_RECONF_CHG*/
MacRetType fillPhyReConfigRequest(FAPI_l1ApiMsg_st **reconfigReq_pp,
        UInt16  *msgLen_p, UInt8 numOfTlvi, InternalCellIndex internalCellIndex);
MacRetType fillPhyReConfigRequestAfterCellStop(FAPI_l1ApiMsg_st **reconfigReq_pp,
        UInt16  *msgLen_p, UInt8 numOfTlv, InternalCellIndex internalCellIndex);
void phyCellConfigGlobalFill(InternalCellIndex internalCellIndex);
MacRetType parseConfigPhyCellAfterCellStop(UInt8 *msg_p,
        UInt16 totalMsgLen,UInt8 *numTLV_p,InternalCellIndex internalCellIndex);
static UInt32 handle_g[MAX_NUM_CELL];

MacRetType fapiSendPhyParamReq( InternalCellIndex internalCellIndex );

/*coverity 10511,29849,10168,10167,10512,10513*/
static  FAPI_phyCellConfigRequest_st * getPhyCellConfigReqPtr
                                            (FAPI_l1ApiMsg_st *pConfigReq_p);
static  FAPI_phyCellConfigResp_st * getPhyCellConfigRespPtr
                                            (FAPI_l1ApiMsg_st *pL1ConfigRes_p);                                            
 UInt8* getBuffer(FAPI_ulConfigRequest_st * ulConfReq_p);
 FAPI_phyCellReConfigRequest_st * getPhyCellReConfigReqPtr
                                            (FAPI_l1ApiMsg_st *reconfigReq_p);


/*coverity 10511,29849,10168,10167,10512,10513*/


#ifdef PERF_STATS
extern ContentionFreeRACHPool*   contentionFreeRACHPool_gp[MAX_NUM_CELL]; 
#endif
/*SPR 1254 Start*/
/*SPR 1254 End*/
/** PHY_382 Change start */
/** PHY_382 Change start */
/*mandeep changes start*/
/*mandeep changes stop*/
/** PHY_382 Change stop */
/* Start 128 UE: Memory Changes */
/*****************************************************************************
 * Function Name  : initULConfigMsg 
 * Inputs         : numOfCells 
 * Outputs        : None
 * Returns        : None
 * Description    : This function creates memory pool for UL config message.
 ******************************************************************************/
void initULConfigMsg(UInt8 numOfCells)
{
    /*
    ** This memory pool is utilized by creating UL Config Msg (prepareULConfigMsg())
    ** and the required size of memory is calculated on the basis of
    ** message size.
    **
    ** NumOfBlock = In Tdd, 6 UL config Msg can be created at any point of time.
    */
    InternalCellIndex cellCount = 0;
    createMemPool(512, NUM_OF_BLK_FOR_UL_CONFIG_MSG);
    /* CA Changes start */
    createMemPool(sizeof (CellConfigPhy) , numOfCells);
    /* CA Changes end */
    /* Initializing the UL CONFIG Map used for Error Ind Handling */
     for (cellCount = 0; cellCount < numOfCells; cellCount++)
    {
    memSet(&ulConfigMap_g[cellCount], 0,
            (sizeof(UlConfigMap) * MAX_UL_CONFIG_CONTAINER_MAP_SIZE));
    }
}
/* End 128 UE: Memory Changes */
/*****************************************************************************
 * Function Name  : initUeConfigMsgQueue 
 * Inputs         : numOfCells
 * Outputs        : None
 * Returns        : None
 * Description    : This function initializes UE config queues.
 ******************************************************************************/
void initUeConfigMsgQueue (UInt8 numOfCells)
{
    /* This queue is used for sending UL Config Msg */
    UInt8 cellIndex = 0;
    for (cellIndex= 0; cellIndex < numOfCells; cellIndex++)   
    {
        initUlConfigReqQueue (cellIndex);
    }
}
/*****************************************************************************
 * Function Name  : resetHandleVal 
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function resets handle_g counter.
 ******************************************************************************/
void resetHandleVal (InternalCellIndex internalCellIndex)
{
    handle_g[internalCellIndex] = 0;
}
/*****************************************************************************
 * Function Name  : generateHandleVal 
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : UInt32
 * Description    : This function increments handle_g counter.
 ******************************************************************************/
UInt32 generateHandleVal (InternalCellIndex internalCellIndex)
{
    return ++(handle_g[internalCellIndex]);
}
/*coverity 10511,10513*/
/*****************************************************************************
 * Function Name  : getPhyCellConfigReqPtr 
 * Inputs         : pConfigReq_p
 * Outputs        : None
 * Returns        : Pointer to FAPI_phyCellConfigRequest_st
 * Description    : 
 ******************************************************************************/

static  FAPI_phyCellConfigRequest_st * getPhyCellConfigReqPtr(FAPI_l1ApiMsg_st *pConfigReq_p)
{
    return (FAPI_phyCellConfigRequest_st *)&pConfigReq_p->msgBody[0];
}
/*coverity 10511,10513*/
/*****************************************************************************
 * Function Name  : fillPhyConfigRequest 
 * Inputs         : ppConfigReq_pp - double pointer to structure FAPI_l1ApiMsg_st,
 *                  pmsgLen_p - to store msg lenth
 *                  internalCellIndex - cell id  
 * Outputs        : Complete message structure for PHy Cell config request on
 *                  MAC - PHY interface (FAPI_l1ApiMsg_st)
 * Returns        : MAC_FAILURE/ MAC_SUCCESS 
 * Description    : This function is used by MAC to fill Phy Cell Config request
 *                  (PHY_CELL_CONFIG_REQUEST) that needs to be sent to PHY
 *                  interface.
 *****************************************************************************/
/* CA Changes start */
MacRetType fillPhyConfigRequest(FAPI_l1ApiMsg_st**ppConfigReq_pp, 
        UInt16  *pmsgLen_p, InternalCellIndex internalCellIndex)
{
    UInt8 tlvIndex = 0;    
    FAPI_l1ApiMsg_st *pConfigReq_p = PNULL;
    FAPI_phyCellConfigRequest_st *pFapiCellConfigReq_p = PNULL;
    /* SPR 9383 Fix Start */
    UInt16 tempMsgLen = 0;
    /* SPR 9383 Fix End */
    /* Allocate memory for sending request */
    UInt16 configMsgLen = sizeof(FAPI_l1ApiMsg_st) +
        /* SPR 1493 changes start */
        /*SPR 21056 fix start*/
        sizeof(FAPI_phyCellConfigRequest_st) + ((FAPI_MAX_NUM_TLVS_CONFIG_PHY -1)*
                /* SPR 1493 changes end */
                sizeof(FAPI_cellConfig_st)) - 2; /* -2 is for 1-1 byte memory taken by
                                                    the stretchable arrays */ 
    /*SPR 21056 fix end*/ 
    GET_MEM_FROM_POOL(FAPI_l1ApiMsg_st,pConfigReq_p,configMsgLen, NULL); 
    /* coverity 530 CID 25165*/
    if(PNULL == pConfigReq_p)
    {
        return MAC_FAILURE;    
    }
    /* coverity 530 CID 25165*/

    memSet(pConfigReq_p, 0, configMsgLen);

    /* Start filling the request structure */
    pConfigReq_p->msgId = PHY_CELL_CONFIG_REQUEST;
    /*coverity 10511*/
    pFapiCellConfigReq_p = getPhyCellConfigReqPtr(pConfigReq_p);
    /*coverity 10511*/

    /* Index - 0 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_DUPLEXING_MODE;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.duplexingMode = 
        MAC_PHY_CONVERT_16(cellConfigPhy_gp[internalCellIndex]->duplexingMode);
    tlvIndex++; 

    /* Index - 1 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PCFICH_POWER_OFFSET;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.pcfichPowerOffset = 
        MAC_PHY_CONVERT_16(cellConfigPhy_gp[internalCellIndex]->pcfichPowerOffset);

    tlvIndex++;

    /* Index - 2 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_P_B;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.pb =
        MAC_PHY_CONVERT_16(cellConfigPhy_gp[internalCellIndex]->pb);
    tlvIndex++;

    /* Index - 3 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_DL_CYCLIC_PREFIX_TYPE;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.dlCyclicPrefixType = 
        MAC_PHY_CONVERT_16(cellConfigPhy_gp[internalCellIndex]->dlCyclicPrefixType);

        tlvIndex++;

    /* Index - 4 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_UL_CYCLIC_PREFIX_TYPE;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.ulCyclicPrefixType = 
        MAC_PHY_CONVERT_16(cellConfigPhy_gp[internalCellIndex]->ulCyclicPrefixType);

        tlvIndex++;

    /* RF Config Starts */
    /* Index - 5 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_DL_CHANNEL_BANDWIDTH;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.dlChannelBW = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->rfConfig.dlChannelBW);
    tlvIndex++;

    /* Index - 6 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_UL_CHANNEL_BANDWIDTH;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.ulChannelBW = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->rfConfig.ulChannelBW);
    tlvIndex++;

    /* Index - 7 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_REFERENCE_SIGNAL_POWER;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    /* SPR 3802 */
    /* + SPR 9603 Change Start*/
    /* SPR 19288 change start */
    /* SPR 21548 fix start */

    /* For more details refer SPR: 7753 */

    /*Calculation of RefSignalPower 
     * 1.. refSignalPower_inSIB2 = ((Power Rating of RF Card) - 10*log10(Number of Subcarriers))
     *          Now, Power Rating of RF Card = 13dBm(max Power) for femtocell and Number of Subcarriers for 10 MHz is 600
     *          Hence, refSignalPower_inSIB2 = 13 - 10*log10(600) = -14.78 = -15
     * 2.. X_VENDOR_MAX_RS_EPRE/maxEpre = EPRE_TRANSFORM_BASE(68) +(4*refSignalPower_inSIB2) = 68 - 60 = 8
     * 3.. refSignalPower_to_L1 = (X_VENDOR_MAX_RS_EPRE - EPRE_TRANSFORM_BASE) - (4 * refSignalPower_inSIB2)
     *                          = (8 - 68) - (4 * -15) = 0
     *
     * For Reduction of 1dBm from max power 
     * 1.. refSignalPower_inSIB2    = -16
     * 2.. X_VENDOR_MAX_RS_EPRE     = 8
     * 3.. refSignalPower_to_L1     = (8 - 68)-(4*-16) = 4(In 0.25 steps it is 1db)
     */

    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.refSignalPower =
        MAC_PHY_CONVERT_16((cellConfigPhy_gp[internalCellIndex]->maxEpre - EPRE_TRANSFORM_BASE)
                - ((cellConfigPhy_gp[internalCellIndex]->rfConfig.refSignalPower)*4));
    /* SPR 21548 fix end */

    /* SPR 19288 change end */
    /* - SPR 9603 Change End*/
    /* SPR 3802 */

    tlvIndex++;

    /* Index - 8 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_TX_ANTENNA_PORTS;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.txAntennaPort = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->rfConfig.txAntennaPort);
    tlvIndex++;

    /* Index - 9 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_RX_ANTENNA_PORTS;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.rxAntennaPort = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->rfConfig.rxAntennaPort);
    tlvIndex++;
    /* RF Config Ends */

    /* PHICH Config starts */
    /* Index - 10 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PHICH_RESOURCE;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.phichResource = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->phichConfig.phichResource);
    tlvIndex++;

    /* Index - 11 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PHICH_DURATION;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.phichDuration = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->phichConfig.phichDuration);
    tlvIndex++;

    /* Index - 12 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PHICH_POWER_OFFSET;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.phichPowOffset = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->phichConfig.phichPowOffset);
    tlvIndex++;
    /* PHICH Config ends */

    /* SCH Config Starts */
    /* Index - 13 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PRIMARY_SYNC_SIGNAL;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.primarySyncSignal = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->schConfig.primarySyncSignal);
    tlvIndex++;

    /* Index - 14 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_SECONDARY_SYNC_SIGNAL;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.secondarySyncSignal = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->schConfig.secondarySyncSignal);
    tlvIndex++;

    /* Index - 15 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PHYSICAL_CELL_ID;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.physicalCellId = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->schConfig.physicalCellId);
    tlvIndex++;
    /* SCH Config Ends */

    /* PRACH Config Starts */
    /* Index - 16 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_CONFIGURATION_INDEX;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.configurationIndex = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->prachConfig.configurationIndex);
    tlvIndex++;

    /* Index - 17 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_ROOT_SEQUENCE_INDEX;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.rootSeqIndex = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->prachConfig.rootSeqIndex);
    tlvIndex++;

    /* Index - 18 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = 
        FAPI_ZERO_CORRELATION_ZONE_CONFIGURATION;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.
        zeroCorelationZoneConfig = MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->prachConfig.
                zeroCorelationZoneConfig);
    tlvIndex++;

    /* Index - 19 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_HIGH_SPEED_FLAG;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.highSpeedFlag = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->prachConfig.highSpeedFlag);
    tlvIndex++;

    /* Index - 20 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_FREQUENCY_OFFSET;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.freqOffset = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->prachConfig.freqOffset);
    tlvIndex++;
    /* PRACH Config Ends */

    /* PUSCH Config Starts */
    /* Index - 21 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_HOPPING_MODE;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.hoppingMode = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->puschConfig.hoppingMode);
    tlvIndex++;

    /* Index - 22 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_HOPPIG_OFFSET;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.hoppingOffset = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->puschConfig.hoppingOffset);
    tlvIndex++;

    /* Index - 23 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_NUM_OF_SUB_BANDS;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.numOfSubBand = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->puschConfig.numOfSubBand);
    tlvIndex++;
    /* PUSCH Config Ends */

    /* PUCCH Config Starts */
    /* Index - 24 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_DELTA_PUCCH_SHIFT;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.deltaPUCCHShift = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->pucchConfig.deltaPUCCHShift);
    tlvIndex++;

    /* Index - 25 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_N_CQI_RB;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.nCQIRB = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->pucchConfig.nCQIRB);
    tlvIndex++;

    /* Index - 26 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_N_AN_CS;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.nAnCs = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->pucchConfig.nAnCs);
    tlvIndex++;

    /* Index - 27 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_N_1_PUCCH_AN;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.n1PucchAn = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->pucchConfig.n1PucchAn);
    tlvIndex++;
    /* PUCCH Config Ends */

    /* SRS Config Starts */
    /* Index - 28 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_BANDWIDTH_CONFIGURATION;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.bandWidthConfiguration = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->\
                srsConfig.bandWidthConfiguration);
    tlvIndex++;

    /* Index - 29 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_MAX_UP_PTS;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.macUpPTS =
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->srsConfig.macUpPTS);
    tlvIndex++;

    /* Index - 30 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = 
        FAPI_SRS_SUB_FRAME_CONFIGURATION;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    /* SPR 19288 change start */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.
        SRSSubframeConfiguration = 
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->srsConfig.
                SRSSubframeConfiguration);
    /* SPR 19288 change end */
    tlvIndex++;

    /* Index - 31 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = 
        FAPI_SRS_ACK_NACK_SRS_SIMULTANEOUS_TRANSMISSION;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 1;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.srsAckNackSimulTx = 
        cellConfigPhy_gp[internalCellIndex]->srsConfig.srsAckNackSimulTx;
    /* SPR 19288 change start */
    /* SPR 19288 change end */
    tlvIndex++;
    /* SRS Config Ends */
    /* Uplink reference signal Config Starts */

    /* SPR 5411 Changes Start */
    /* Index - 32 */
    {
        UInt16 uplinkRSHoping = FAPI_RS_NO_HOPPING;
        if(cellConfigPhy_gp[internalCellIndex]->puschConfig.groupHopEnable)
        {
            uplinkRSHoping = FAPI_RS_GROUP_HOPPING;
        }
        else if (cellConfigPhy_gp[internalCellIndex]->puschConfig.seqHopEnable)
        {
            uplinkRSHoping = FAPI_RS_SEQUENCE_HOPPING;
        }
        pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_UPLINK_RS_HOPPING;
        pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;

        pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.uplinkRSHoping =
            MAC_PHY_CONVERT_16(uplinkRSHoping);
        tlvIndex++;
    }
    /* Index - 33 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_GROUP_ASSIGNMENT;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.groupAssignment =
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->puschConfig.groupAssignPUSCH);
    tlvIndex++;

    /* Index - 34 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_CYCLIC_SHIFT_1_FOR_DMRS;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.cyclicShift1forDMRS =
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->puschConfig.cyclicShift);
    tlvIndex++;
    /* SPR 5411 Changes end */

    /* Index - 35 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_DATA_REPORT_MODE;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.dataReportingMode =
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->dataReportingMode);
    tlvIndex++;
#ifdef TDD_CONFIG


    /* Index - 36 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_SUB_FRAME_ASSIGNMENT;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.subFrameAssignment =
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->subFrameAssign);
    tlvIndex++;

    /* Index - 37 */
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_SPECIAL_SUB_FRAME_PATTERNS;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.specialSubFramePatterns =
        MAC_PHY_CONVERT_16( cellConfigPhy_gp[internalCellIndex]->splSubFramePatterns);
    tlvIndex++;
#endif
    /* + PRS_CHANGES */
    /* SPR_7936_FIX */
    /* SPR 19288 change start */
    /* SPR 19288 change end */
#ifndef FLEXRAN
    if(cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.updateFlag)
    {
        /* SPR_7936_FIX */
        /* Index - 38 */
        pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PRS_BANDWIDTH;
        pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
        /* SPR 19288 change start */
        pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.prsBandWidth =
            MAC_PHY_CONVERT_16(cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsBandWidth);
        /* SPR 19288 change end */
        tlvIndex++;
        /* Index - 39*/
        pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PRS_CYCLIC_PREFIX;
        pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
        /* SPR 19288 change start */
        pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.prsCyclicPrifix = 
            MAC_PHY_CONVERT_16(cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsCyclicPrifix);
        /* SPR 19288 change end */
        tlvIndex++;
    }
#endif
    /* - PRS_CHANGES */

    /* SPR 9383 Fix Start */
#if defined(FLEXRAN)
#ifndef FLEXRAN
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PHY_SYNC_MODE;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.phySyncMode = 2;
    tlvIndex++;
#endif

    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_RACH_AND_SR_REPORT_MODE;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.rachAndSrReportMode = 0;
    tlvIndex++;

    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_EXTRACTION_WINDOW_MARGIN;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
#ifdef FLEXRAN
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.extractionWindowMargin = 0;
#else
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.extractionWindowMargin = 10;
#endif
    tlvIndex++;

    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PUCCH_NOISE_ESTIMATION_GAMMA;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.pucchNoiseEstimationGamma = 8192;
    tlvIndex++;
    /*SPR 21347 fix start*/
#if defined(FLEXRAN)
    eltaAstriParamThreshold *paramThresholdVal = PNULL;
    paramThresholdVal = eltaAstriParamUpdate();
#endif

    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PRACH_FORMAT_4_PEAK_RATIO;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
#ifdef FLEXRAN
   pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.prachFormat4PeakRatio = 8908;
#endif
    tlvIndex++;

    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PRACH_FORMAT_0_PEAK_RATIO;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
#ifdef FLEXRAN
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.prachFormat0PeakRatio = 8806;
#endif
    tlvIndex++;
#if defined(FLEXRAN)
    free(paramThresholdVal);
#endif
    /*SPR 21347 fix end*/

    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_DOPPLER_ESTIMATION_COMPENSATION_FACTOR;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.dopplerEstimationCompensationFactor = 65535;
    tlvIndex++;

    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PROBABILITY_DTX_ACK_PUSCH;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.probabilityDtxAckPusch = 12;
    tlvIndex++;

    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PROBABILITY_DTX_ACK_PUCCH_FORMAT_1;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.probabilityDtxAckPucchFormat1 = 12;
    tlvIndex++;
#ifndef FLEXRAN
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_DCI_POWER_OFFSET;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.dciPowerOffset = 6000;
    tlvIndex++;

    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_UPLINK_PAYLOAD_ALLOCATION_METHOD;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.uplinkpayloadallocationMethod = 1;
    tlvIndex++;
    /*SPR 21056 fix start*/
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_EARFCN_FOR_PHY;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.earfcn = \
                 cellConfigPhy_gp[internalCellIndex]->rfConfig.dlEarfcn;
    tlvIndex++;
    /*SPR 21056 fix end*/
#else
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tag = FAPI_UPLINK_PAYLOAD_ALLOCATION_METHOD;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
    pFapiCellConfigReq_p->configtlvs[tlvIndex].configParam.uplinkpayloadallocationMethod = 0;
    tlvIndex++;
#endif
#endif


    pFapiCellConfigReq_p->numOfTlv = tlvIndex;
    /*SPR_8966_FIX_START*/
    tempMsgLen = offsetof(FAPI_phyCellConfigRequest_st, configtlvs) +
        (pFapiCellConfigReq_p->numOfTlv * sizeof(FAPI_cellConfig_st));
    /* SPR 19288 change start */
    pConfigReq_p->msgLen = MAC_PHY_CONVERT_16(tempMsgLen);
    /* SPR 19288 change end */
    /*SPR_8966_FIX_END*/
    *ppConfigReq_pp = pConfigReq_p;
    /* SPR 19288 change start */
    *pmsgLen_p = offsetof(FAPI_l1ApiMsg_st,msgBody) + tempMsgLen;
/* SPR 19288 change end */
    /* SPR 9383 Fix End */
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : getulBandwidth
 * Inputs         : ulChBw - Uplink Channel Bandwidth
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function gets the Uplink Channel Bandwidth.
 ******************************************************************************/
/* CA Changes start */
void getulBandwidth(UInt8 ulChBw, InternalCellIndex internalCellIndex)
{
    switch ( ulChBw )
    {
        case UL_TX_BANDWIDTH_6RB:
        ulBandwidth_g[internalCellIndex] = 1.4;
            break;
        case UL_TX_BANDWIDTH_15RB:
        ulBandwidth_g[internalCellIndex] = 3;
            break;
        case UL_TX_BANDWIDTH_25RB:
        ulBandwidth_g[internalCellIndex] = 5;
            break;
        case UL_TX_BANDWIDTH_50RB:
        ulBandwidth_g[internalCellIndex] = 10;
            break;
        case UL_TX_BANDWIDTH_75RB:
        ulBandwidth_g[internalCellIndex] = 15;
            break;
        case UL_TX_BANDWIDTH_100RB:
        ulBandwidth_g[internalCellIndex] = 20;
            break;
        default :
            LOG_MAC_WARNING( MAC_RRC_INF,"Wrong value recevied of ulChannel BandWidth [%d]\n",ulChBw);
            break;
    }

}

/*****************************************************************************
 * Function Name  : getdlBandwidth
 * Inputs         : dlChBw - Downlink Channel Bandwidth
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function gets the Downlink Channel Bandwidth.
 ******************************************************************************/
void getdlBandwidth(UInt8 dlChBw,InternalCellIndex internalCellIndex)
{
    switch ( dlChBw )
    {
        case DL_TX_BANDWIDTH_6RB:
        dlBandwidth_g[internalCellIndex] = 1.4;
            break;
        case DL_TX_BANDWIDTH_15RB:
        dlBandwidth_g[internalCellIndex] = 3;
            break;
        case DL_TX_BANDWIDTH_25RB:
        dlBandwidth_g[internalCellIndex] = 5;
            break;
        case DL_TX_BANDWIDTH_50RB:
        dlBandwidth_g[internalCellIndex] = 10;
            break;
        case DL_TX_BANDWIDTH_75RB:
        dlBandwidth_g[internalCellIndex] = 15;
            break;
        case DL_TX_BANDWIDTH_100RB:
        dlBandwidth_g[internalCellIndex] = 20;
            break;
        default :
            LOG_MAC_WARNING( MAC_RRC_INF,"Wrong value recevied of dlChannel BandWidth [%d]\n",dlChBw);
            break;    }

}
/* CA Changes end */
/* cyclomatic changes */
/*****************************************************************************
 * Function Name  : validatePucchAndPuschConfiguration 
 * Inputs         : internalCellIndex
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is used by MAC to parse the cell config
 *                  request meant for PHY for PUSCH/PUCCH, which is sent from RRC. 
 *****************************************************************************/
/* CA Changes start */
MacRetType validatePucchAndPuschConfiguration(InternalCellIndex internalCellIndex)
{
    /* SPR 1868 changes start */
    /* SPR 1699 changes start */ 
    /* SPR 3204 changes start */ 
    if(cellConfigPhy_gp[internalCellIndex]->puschConfig.hoppingOffset >
            PUSCH_HOPPING_OFFSET)
        /* SPR 1699 changes end */ 
        /* SPR 1868 changes end */
        /* SPR 3204 changes end */ 
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: hopping offset for PUSCH");
        return MAC_FAILURE;
    }
    if ((1 > cellConfigPhy_gp[internalCellIndex]->puschConfig.numOfSubBand) || 
            (MAX_VAL_NUM_OF_SUB_BANDS <
             cellConfigPhy_gp[internalCellIndex]->puschConfig.numOfSubBand))
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: Num Sub-band");
        return MAC_FAILURE;
    }
    if (1 < cellConfigPhy_gp[internalCellIndex]->puschConfig.hoppingMode)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: Hopping Mode");
        return MAC_FAILURE;
    }
    if ((1 > cellConfigPhy_gp[internalCellIndex]->pucchConfig.deltaPUCCHShift) || 
            (MAX_VAL_DELTA_PUCCH_SHIFT < cellConfigPhy_gp[internalCellIndex]->pucchConfig.
             deltaPUCCHShift))
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: Delta PUCCH shft");
        return MAC_FAILURE;
    }

    if (MAX_VAL_NCQIRB < 
            cellConfigPhy_gp[internalCellIndex]->pucchConfig.nCQIRB)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: nCQIRB");
        return MAC_FAILURE;
    }

    if (MAX_VAL_NANCS < 
            cellConfigPhy_gp[internalCellIndex]->pucchConfig.nAnCs)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: nAnCs");
        return MAC_FAILURE;
    }
    if (MAX_VAL_N1_PUCCH_AN <
            cellConfigPhy_gp[internalCellIndex]->pucchConfig.n1PucchAn)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: n1PucchAn ");
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}
/* CA Changes end */
/* cyclomatic changes */
/*****************************************************************************
 * Function Name  : parseCellConfigPhyReq 
 * Inputs         : msg_p,totalMsgLen and internalCellIndex
 * Outputs        : Cell config structure for PHY (CellConfigPhy) 
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is used by MAC to parse the cell config
 *                  request meant for PHY, which is sent from RRC. 
 *                  This function shall parse the  request and fill the 
 *                  CellConfigPhy_gp structure.
 *****************************************************************************/
/* CA Changes start */
MacRetType parseCellConfigPhyReq ( UInt8 *msg_p, UInt16 totalMsgLen,
        InternalCellIndex internalCellIndex )
{
    SInt8     refSig = 0;
    UInt16     refSigCal = 0;
    UInt16 remLength  = totalMsgLen;   
    /* SPR 3133 Chg */
    UInt16 explicitStartBitMask = 0;
    /* SPR 3133 Chg Ends */
    MacRetType response = MAC_SUCCESS;
    MacRetType ret_val = MAC_FAILURE;
    UInt8 dlChBw = 0;
    UInt8 ulChBw = 0;
    UInt16 temp_u16 = 0;
    /* SPR_7936_FIX */
    UInt16 bitMask = 0;
    /* reinitialize the PRS update flag before parse */
    cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.updateFlag = FALSE;
    /* SPR_7936_FIX */

    /* SPR 3133 Chg */
    explicitStartBitMask = LTE_GET_U16BIT(msg_p);
    msg_p += 2;
    remLength -= 2;
    /* SPR 3133 Chg */

    msg_p++; // present regardless of MULTI_SECTOR feature
    remLength--;

    bitMask = LTE_GET_U16BIT(msg_p);
    msg_p += 2; 
    remLength -= 2; 

    /*  2.a. 1 Byte Duplex Mode (TDD/FDD)
     *  2.b. 1 Byte Freq Band Indicator
     *  2.c. 2 Byte Ul Carrier Frequency
     *  2.d. 4 Byte Dl Carrier Frequency
     *  2.e. 1 BYte No of Tx Antennas
     *  2.f. 1 Byte Ul Tx Bandwidth
     *  2.g. 1 Byte Dl Tx Bandwidth
     *  2.h. 1 Byte Sub Carrier Spacing
     *  2.i. 1 Byte Ul Cyclic Prefix Length
     *  2.j. 1 Byte Dl Cyclic Prefix Length
     *  2.k. X Bytes SRSS UL Config Common
     *  2.l. 2 Bytes Phy Cell Id
     */

    cellConfigPhy_gp[internalCellIndex]->duplexingMode = *msg_p++;/*2.a*/
    remLength--; 

    LTE_MAC_UT_LOG(LOG_INFO, CELL_CONF, "\n OPERATING MODE [%s] \n",
            (cellConfigPhy_gp[internalCellIndex]->duplexingMode)?" FDD ":" TDD ");
    if (MAX_VAL_DUPLEX_MODE < cellConfigPhy_gp[internalCellIndex]->duplexingMode)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range:  Duplex mode");
        return MAC_FAILURE;
    }
    /*2.01 ref is 2.b It seems that Frequency Band Indication is not taken care in FAPI*/
    msg_p++;  /* EUTRA Band */
    remLength--; 

    msg_p += 2;         /* UL EARFCN */ 
    remLength -= 2;

    msg_p += 4;         /* DL EARFCN */ 
    remLength -= 4;

    /* Note:
     * No. of antennas(Aricent) is used to fill Tx/Rx Antenna ports for FF APIs 
     */
    cellConfigPhy_gp[internalCellIndex]->rfConfig.txAntennaPort = *msg_p++;/*2.e*/
    remLength--; 
    cellConfigPhy_gp[internalCellIndex]->rfConfig.rxAntennaPort = 
        cellConfigPhy_gp[internalCellIndex]->rfConfig.txAntennaPort;
        /* SPR 21021 fix start*/ 
            /* SPR 21021 fix end*/
    if ((1 != cellConfigPhy_gp[internalCellIndex]->rfConfig.txAntennaPort) &&
            (2 != cellConfigPhy_gp[internalCellIndex]->rfConfig.txAntennaPort) &&
            (4 != cellConfigPhy_gp[internalCellIndex]->rfConfig.txAntennaPort))
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: Rx/Tx Antenna "
                "ports type");
        return MAC_FAILURE;
    }
#if  !defined(FLEXRAN)
    /* SPR 1308 changes start */
    /* SPR 5165 changes start */
    /* SPR 21021 fix start*/ 
    if (  phyParamResp_g[internalCellIndex].phyAntennaCapability <
            cellConfigPhy_gp[internalCellIndex]->rfConfig.rxAntennaPort)
    /* SPR 5165 changes end */
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param mismatch with received in Param Resp:"
                "Rx/Tx Antenna ports type");
        return MAC_FAILURE;
    }
    /* SPR 1308 changes end*/
#endif
    /* SPR 21021 fix end*/ 

    ulChBw = *msg_p++;
    remLength--; 
#if  !defined(FLEXRAN)
    /* SPR 1308 changes start*/
/* SPR 19288 change start */
    if (! (phyParamResp_g[internalCellIndex].ulBandWidthSupport & (0x01<<ulChBw) ) )
/*SPR 21001 Changes end */
/* SPR 19288 change end */
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param mismatch with received in Param Resp:UL bandwidth");
        return MAC_FAILURE;	
    }
    /* SPR 1308 changes end*/
#endif

    /*Cyclomatic Complexity changes - start here */
    getulChannelBW(ulChBw,internalCellIndex);
    /*Cyclomatic Complexity changes - end here */

    dlChBw = *msg_p++;
    remLength--; 
#if  !defined(FLEXRAN)
    /* SPR 1308 changes start*/
/* SPR 19288 change start */
/*SPR 21001 Changes start */
    if ( ! (phyParamResp_g[internalCellIndex].dlBandWidthSupport & (0x01<<dlChBw) ) )
/*SPR 21001 Changes end */
/* SPR 19288 change end */
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param mismatch with received in Param Resp :DL bandwidth");

        return MAC_FAILURE;
	
    }
    /* SPR 1308 changes end*/
#endif
    /*Cyclomatic Complexity changes - start here */
    getdlChannelBW(dlChBw,internalCellIndex);
    /*Cyclomatic Complexity changes - end here */
    getulBandwidth(ulChBw,internalCellIndex); 
    getdlBandwidth(dlChBw,internalCellIndex);
    msg_p++; /* Sub carrier spacing - Not Supported in FF */
    remLength--; 

    cellConfigPhy_gp[internalCellIndex]->ulCyclicPrefixType = *msg_p++;
    remLength--; 
    if ((0 != cellConfigPhy_gp[internalCellIndex]->ulCyclicPrefixType) &&
            (1 != cellConfigPhy_gp[internalCellIndex]->ulCyclicPrefixType))
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: UL Cyclic Prefix "
                "type");
        return MAC_FAILURE;
    }

    cellConfigPhy_gp[internalCellIndex]->dlCyclicPrefixType = *msg_p++;
    remLength--; 
    if ((0 != cellConfigPhy_gp[internalCellIndex]->dlCyclicPrefixType) &&
            (1 != cellConfigPhy_gp[internalCellIndex]->dlCyclicPrefixType))

    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: DL Cyclic Prefix "
                "type");
        return MAC_FAILURE;
    }

    /* Modifications for 2.01 RRC Doc*/
    /*Refer to struct for parsing SRS details
     * 2.k sRsUlConfigCommon
     *   .1.   1 Byte Bitmask for 2.k.2 presence(soundingRsULConfigCommonSetup)
     *   .2.   OPTIONAL soundingRsULConfigCommonSetup
     *     .i.   mask(srsMaxUpPts presence)
     *     .ii.  srsBandwidthConfig
     *     .iii. srsSubframeConfig
     *     .iv.  ackNackSrsSimultaneousTransmission
     *     .v.   srsMaxUpPts OPTIONAL
     */
    temp_u16 = LTE_GET_U16BIT(msg_p);
    msg_p += 2;
    remLength -= 2;
    if(temp_u16 & 0x01)
    {
        temp_u16  = LTE_GET_U16BIT(msg_p); /*presence of srsMaxUpPts*/
        msg_p += 2;
        remLength -= 2;
        cellConfigPhy_gp[internalCellIndex]->srsConfig.bandWidthConfiguration   
            = *msg_p++;
        remLength--; 
        cellConfigPhy_gp[internalCellIndex]->srsConfig.SRSSubframeConfiguration 
            = *msg_p++;
        remLength--; 
        cellConfigPhy_gp[internalCellIndex]->srsConfig.srsAckNackSimulTx
            = *msg_p++;
        remLength--; 
        if(temp_u16 & 0x01)
        {
            cellConfigPhy_gp[internalCellIndex]->srsConfig.macUpPTS = *msg_p++;
            remLength--; 
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_INFO, CELL_CONF,
                    "macUpPTS not present taking default [%d]\n",DEFAULT_SRSCONFIG_MAX_UP_PTS);
            cellConfigPhy_gp[internalCellIndex]->srsConfig.macUpPTS = 
                DEFAULT_SRSCONFIG_MAX_UP_PTS;

        }
    } 
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO, CELL_CONF,"bit mask for SRS config not present\n");
        LTE_MAC_UT_LOG(LOG_INFO, CELL_CONF,"taking bandWidthConfiguration default [%d]\n",
                DEFAULT_SRSCONFIG_BANDWIDTH_CONFIG);
        cellConfigPhy_gp[internalCellIndex]->srsConfig.bandWidthConfiguration = 
            DEFAULT_SRSCONFIG_BANDWIDTH_CONFIG;

        LTE_MAC_UT_LOG(LOG_INFO, CELL_CONF,"taking SRSSubframeConfiguration default [%d]\n",
                DEFAULT_SRSCONFIG_SUBFRM_CONFIG);
        cellConfigPhy_gp[internalCellIndex]->srsConfig.SRSSubframeConfiguration  = 
            DEFAULT_SRSCONFIG_SUBFRM_CONFIG;

        LTE_MAC_UT_LOG(LOG_INFO, CELL_CONF,"taking srsAckNackSimulTx default [%d]\n", 
                DEFAULT_SRSCONFIG_ACKNACK_SIMUL);
        cellConfigPhy_gp[internalCellIndex]->srsConfig.srsAckNackSimulTx = 
            DEFAULT_SRSCONFIG_ACKNACK_SIMUL;

        LTE_MAC_UT_LOG(LOG_INFO, CELL_CONF,"taking macUpPTS default [%d]\n", 
                DEFAULT_SRSCONFIG_MAX_UP_PTS);
        cellConfigPhy_gp[internalCellIndex]->srsConfig.macUpPTS =
            DEFAULT_SRSCONFIG_MAX_UP_PTS;

    }
    cellConfigPhy_gp[internalCellIndex]->schConfig.physicalCellId =
        LTE_GET_U16BIT(msg_p);
    msg_p += 2;
    remLength -= 2;
    if(MAX_VAL_PHYSICAL_CELL_ID <
            cellConfigPhy_gp[internalCellIndex]->schConfig.physicalCellId)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: PHY Cell Id");
        return MAC_FAILURE;
    }

    /*CLPC_CHG*/
    cellConfigPhy_gp[internalCellIndex]->pcfichPowerOffset = LTE_GET_U16BIT(msg_p);
    msg_p += 2;
    remLength -= 2;
    if ( cellConfigPhy_gp[internalCellIndex]->pcfichPowerOffset >
            MAX_PCFICH_POWER_OFFSET)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: PCFICH POWER OFFSET");
        return MAC_FAILURE;
    }

    /*Added for CLPC*/
    cellConfigPhy_gp[internalCellIndex]->phichConfig.phichPowOffset =
        LTE_GET_U16BIT(msg_p);
    msg_p += 2;
    remLength -= 2;
    if (cellConfigPhy_gp[internalCellIndex]->phichConfig.phichPowOffset > 
            MAX_PHICH_POWER_OFFSET)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: PHICH Power Offset ");
        return MAC_FAILURE;
    }
    /*CLPC_CHG*/

    /* SPR 3802 */
    cellConfigPhy_gp[internalCellIndex]->maxEpre =  *msg_p++;
    remLength--;
    /* SPR 3802 */

#ifdef TDD_CONFIG
    /* + PRS_CHANGES */
    if(bitMask & BIT_FIRST)
    /* - PRS_CHANGES */
    {
        cellConfigPhy_gp[internalCellIndex]->subFrameAssign = LTE_GET_U16BIT(msg_p);
        msg_p += 2;
        remLength -= 2;
        /*
         * Range check for subFrameAssign
         */
        if ( cellConfigPhy_gp[internalCellIndex]->subFrameAssign > MAC_TDD_MAX_SUBFRAME_ASSIGN)
        {
            LOG_MAC_WARNING( MAC_RRC_INF,"Invalid subFrameAssign = %d", 
                    cellConfigPhy_gp[internalCellIndex]->subFrameAssign);
            return MAC_FAILURE;
        }

        cellConfigPhy_gp[internalCellIndex]->splSubFramePatterns = 
            LTE_GET_U16BIT(msg_p);
        msg_p += 2;
        remLength -= 2;
        /*
         * Range check for splSubFramePatterns
         */
        if ( cellConfigPhy_gp[internalCellIndex]->splSubFramePatterns >
                MAC_TDD_MAX_SPECIAL_SUBFRAME)
        {
            LOG_MAC_WARNING( MAC_RRC_INF,"Invalid splSubFramePatterns = %d",
                    cellConfigPhy_gp[internalCellIndex]->splSubFramePatterns);
            return MAC_FAILURE;
        }
    }
    else
    {
        LOG_MAC_MSG(PHY_CELL_CONFIG_TDD_PARAM_FAIL,LOGERROR,MAC_L1_INF,
                getCurrentTick(),bitMask, 
                DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                __func__,"Mandatory TDD Params not received in PHY Config Cell");
        return MAC_FAILURE;
    }
#endif
    if(MAC_SUCCESS != parsePRSConfig( bitMask,&msg_p,&remLength,internalCellIndex))
    {
        return MAC_FAILURE;
    }
    /**
     *  4.4.1.3	syncSignals_t
     **/
    refSig = *msg_p++;
    remLength--;
    refSigCal = (refSig + MIN_VALUE_OF_SYNC_SIGNAL) *
        MULTIPLY_COEFF_FOR_SYNC_SIGNAL;

   /* SPR 19329 change start*/
    if ((MAX_VAL_SYNC_SIGNAL + 1) == refSigCal)
    {
        refSigCal = MAX_VAL_SYNC_SIGNAL;
    }
   /* SPR 19329 change end*/
    cellConfigPhy_gp[internalCellIndex]->schConfig.primarySyncSignal = refSigCal;
   /* SPR 19329 change start*/
    if(MAX_VAL_SYNC_SIGNAL <
            cellConfigPhy_gp[internalCellIndex]->schConfig.primarySyncSignal)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: Primary Synch"
                "signal");
        return MAC_FAILURE;
    }
   /* SPR 19329 change end*/
    refSig = *msg_p++;
    remLength--; 
    refSigCal = (refSig + MIN_VALUE_OF_SYNC_SIGNAL) *
        MULTIPLY_COEFF_FOR_SYNC_SIGNAL;

   /* SPR 19329 change start*/
    if ((MAX_VAL_SYNC_SIGNAL + 1) == refSigCal)
    {
        refSigCal = MAX_VAL_SYNC_SIGNAL;
    }
   /* SPR 19329 change end*/
    cellConfigPhy_gp[internalCellIndex]->schConfig.secondarySyncSignal = refSigCal;
   /* SPR 19329 change start*/
    if(MAX_VAL_SYNC_SIGNAL < cellConfigPhy_gp[internalCellIndex]->schConfig.secondarySyncSignal)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: Secondory Synch"
                "signal");
        return MAC_FAILURE;
    }
   /* SPR 19329 change end*/
    msg_p++; /* SecSynSignalMSec1 - Not Supported */
    remLength--; 

    msg_p++; /* SecSynSignalMSec2 - Not Supported */
    remLength--; 

    /*4.4.1.3	prachConfiguration_t*/

    cellConfigPhy_gp[internalCellIndex]->prachConfig.configurationIndex = *msg_p++;
    remLength--; 
    if(MAX_VAL_CONFIG_INDEX < 
            cellConfigPhy_gp[internalCellIndex]->prachConfig.configurationIndex)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: Config indx");
        return MAC_FAILURE;
    }
/*SPR 19258 / 19480 fix +-*/

    cellConfigPhy_gp[internalCellIndex]->prachConfig.rootSeqIndex = 
        LTE_GET_U16BIT(msg_p);
    msg_p += 2;
    remLength -= 2;
    if(MAX_VAL_ROOT_SEQ_INDEX < 
            cellConfigPhy_gp[internalCellIndex]->prachConfig.rootSeqIndex)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: Root Seq indx");
        return MAC_FAILURE;
    }
/*SPR 19258 / 19480 fix +-*/

    cellConfigPhy_gp[internalCellIndex]->prachConfig.freqOffset = *msg_p++; 
    remLength--; 

/*SPR 19258 / 19480 fix start*/
if(
	 (cellConfigPhy_gp[internalCellIndex]->prachConfig.freqOffset > ( cellConfigPhy_gp[internalCellIndex]->rfConfig.ulChannelBW - 6)))
    {/* Check applicable for FDD only. currently TDD not supported */
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: FreqOffset ");
        return MAC_FAILURE;
    }
/*SPR 19258 / 19480 fix end*/

    cellConfigPhy_gp[internalCellIndex]->prachConfig.zeroCorelationZoneConfig 
        = *msg_p++;
    remLength--; 
    if(MAX_VAL_ZERO_COR_ZONE < cellConfigPhy_gp[internalCellIndex]->prachConfig.\
            zeroCorelationZoneConfig)
    {/* Check applicable for FDD only. currently TDD not supported */
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: Zero corelation "
                "Zone");

        return MAC_FAILURE;
    }
/*SPR 19258 / 19480 fix +-*/

    cellConfigPhy_gp[internalCellIndex]->prachConfig.highSpeedFlag = *msg_p++;
    remLength--; 
    if(1 < cellConfigPhy_gp[internalCellIndex]->prachConfig.highSpeedFlag)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: High speed flag");
        return MAC_FAILURE;
    }
/*SPR 19258 / 19480 fix +-*/

    /*4.4.1.5	puschConfiguration_t*/ 
    /*  puschHopingOffset*/
    cellConfigPhy_gp[internalCellIndex]->puschConfig.hoppingOffset = *msg_p++;
    remLength--; 
    
    cellConfigPhy_gp[internalCellIndex]->puschConfig.numOfSubBand = *msg_p++; 
    remLength--; 
    
    cellConfigPhy_gp[internalCellIndex]->puschConfig.hoppingMode = *msg_p++; 
    remLength--; 
    
    /*4.4.1.5	pucchConfiguration_t**/
    cellConfigPhy_gp[internalCellIndex]->puschConfig.groupHopEnable = *msg_p++;
    remLength--; 
    cellConfigPhy_gp[internalCellIndex]->puschConfig.groupAssignPUSCH = *msg_p++;
    remLength--; 
    cellConfigPhy_gp[internalCellIndex]->puschConfig.seqHopEnable = *msg_p++;
    remLength--; 
    cellConfigPhy_gp[internalCellIndex]->puschConfig.cyclicShift = *msg_p++;
    remLength--; 

    cellConfigPhy_gp[internalCellIndex]->pucchConfig.deltaPUCCHShift = *msg_p++; 
    remLength--; 

    cellConfigPhy_gp[internalCellIndex]->pucchConfig.nCQIRB = *msg_p++;
    remLength--; 

    cellConfigPhy_gp[internalCellIndex]->pucchConfig.nAnCs = *msg_p++;
    remLength--; 

    cellConfigPhy_gp[internalCellIndex]->pucchConfig.n1PucchAn = 
        LTE_GET_U16BIT(msg_p);
    msg_p += 2;
    remLength -= 2;
    /* cyclomatic changes */
    ret_val = validatePucchAndPuschConfiguration(internalCellIndex);
    if(MAC_FAILURE == ret_val)
    {
       return MAC_FAILURE;
    }
    /* cyclomatic changes */

    /*4.4.1.7	phichConfiguration_t*/
    cellConfigPhy_gp[internalCellIndex]->phichConfig.phichResource = *msg_p++;
    remLength--; 
    if (MAX_VAL_PHICH_RESOURCE < 
            cellConfigPhy_gp[internalCellIndex]->phichConfig.phichResource)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: PHICH resource");
        return MAC_FAILURE;
    }

    cellConfigPhy_gp[internalCellIndex]->phichConfig.phichDuration = *msg_p++;
    remLength--; 
    if (1 < cellConfigPhy_gp[internalCellIndex]->phichConfig.phichDuration)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: PHICH duration");
        return MAC_FAILURE;
    }
    /*4.6.1.18	pdschConfigCommon_t*/
    cellConfigPhy_gp[internalCellIndex]->rfConfig.refSignalPower = *msg_p++;
    remLength--; 
    cellConfigPhy_gp[internalCellIndex]->pb = *msg_p++;
    remLength--; 

    if (MAX_VAL_PB < cellConfigPhy_gp[internalCellIndex]->pb)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Param out of range: pB");
        return MAC_FAILURE;  
    }

    /* Filling parameters that are not coming from RRC currently */
    cellConfigPhy_gp[internalCellIndex]->dataReportingMode = DATA_REPORT_MODE; 

    /* Cell Start Stop Chg */

    /* SPR 3133 Chg */
    /* Cyclomatic_complexity_changes_start */
    response = parseExplicitStartBitMask(explicitStartBitMask,
            &msg_p,
            &remLength);
    /* Cyclomatic_complexity_changes_end */
    /* SPR 3133 Chg Ends */
    /* Cell Start Stop Chg */

    return response;
}
/*****************************************************************************
 * Function Name  : parsePRSConfig 
 * Inputs         : msg_pp, bitMask, remLength_p, internalCellIndex  
 * Outputs        : PRS Config in Cell config structure  
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is used by MAC to parse the PRS Configuration
 *                  cell config request meant for PHY, which is sent from RRC. 
 *                  This function shall parse the PRS Parameters and fill the 
 *                  cellConfigPhy_gp structure.
 *****************************************************************************/
MacRetType parsePRSConfig(UInt16 bitMask,UInt8 **msg_pp,UInt16 *remLength_p,
        InternalCellIndex internalCellIndex)
{

    if (bitMask & BIT_SECOND)
    {  
        /* SPR 22525 Fix + */
#if defined FAPI_4_0_COMPLIANCE
        /* SPR 22525 Fix - */
        if(phyReleaseCapability_g == RELEASE_8)
        {

            LOG_MAC_MSG(PHY_CELL_PRS_CONFIG_FAIL,LOGERROR,MAC_RRC_INF,
                    bitMask, phyReleaseCapability_g,
                    DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    __func__,"PRS Info Received for Release 8 in  PHY Config Cell");
            return MAC_FAILURE;

        }
#endif

        cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsBandWidth = **msg_pp;
        (*msg_pp)++;
        (*remLength_p)--;
        if((FAPI_CHANNEL_BW_6RB != cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsBandWidth) &&\
                (FAPI_CHANNEL_BW_15RB != cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsBandWidth) &&\
                (FAPI_CHANNEL_BW_25RB != cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsBandWidth) &&\
                (FAPI_CHANNEL_BW_50RB != cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsBandWidth) &&\
                (FAPI_CHANNEL_BW_75RB != cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsBandWidth) &&\
                (FAPI_CHANNEL_BW_100RB != cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsBandWidth))
        {
            LOG_MAC_WARNING( MAC_RRC_INF,"Invalid PRS Bandwidth = %d", cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsBandWidth);
            return MAC_FAILURE;
        }
        cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsCyclicPrifix = **msg_pp;
        (*msg_pp)++;
        (*remLength_p)--;
        if((0 != cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsCyclicPrifix) && (1 != cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsCyclicPrifix))
        {
            LOG_MAC_WARNING( MAC_RRC_INF,"Invalid PRS Cyclic Prefix = %d", cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsCyclicPrifix);
            return MAC_FAILURE;

        }
        /* SPR_7936_FIX */
        cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.updateFlag = TRUE;
        /* SPR_7936_FIX */
    }
    return MAC_SUCCESS;

}


/*****************************************************************************
 * Function Name  : sendStartReqToPhy
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : This function is used by MAC to start the PHY Layer. The msg
 *                  after PHY is configured (i.e Cell configuration is sent).
 *****************************************************************************/
/* CA Changes start */
#if   FLEXRAN
MacRetType sendStartReqToPhy(InternalCellIndex internalCellIndex)
{
    FAPI_l1ApiMsg_st *pStartReq_p = NULL;
    MacRetType resp               = MAC_FAILURE;
    UInt16 startMsgLen            = sizeof(FAPI_l1ApiMsg_st);
    void *msgBuf                  = NULL;
    SInt32 bytesRead                 = -1; 

    GET_MEM_FROM_POOL(FAPI_l1ApiMsg_st,pStartReq_p,startMsgLen, NULL); 
    if(PNULL == pStartReq_p)
    {
        return MAC_FAILURE;    
    }

    memSet(pStartReq_p, 0, startMsgLen);

    pStartReq_p->msgId = PHY_START_REQUEST;
    pStartReq_p->msgLen = 0;

    g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_START_REQ; 

    if( sendControlInfoToPhy((UInt8 *)pStartReq_p, startMsgLen,internalCellIndex) >=  0)
    {
        resp = MAC_SUCCESS;
    }
    freeMemPool(pStartReq_p);

    LOG_MAC_MSG(MAC_PHY_START_REQ,LOGDEBUG,MAC_L1_INF,
            PHY_CELL_START_REQ,
            MAC_MODULE_ID,PHY_MODULE_ID,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"SEND_START_PHY_REQ");

    LTE_MAC_UT_LOG(LOG_INFO, CELL_CONF,"Sent Start Request to PHY");
    printf("\n\nSent Start Request to PHY\n\n");

    /* Wait for first SFI from PHY and receives that */
    // << How long to wait ????>>
#if 0		
    timer.tv_nsec = 500000000;
    timer.tv_sec = 0;
    if (-1 == pselect_wrapper(1, PNULL, PNULL, PNULL, &timer, PNULL))
    {
        perror("select failed\n");
    }
#endif
    if (macParams_g.numCellsConfigured == 2 && internalCellIndex == 0)
    {
	    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
		    cellState = CELL_WAITING_FOR_FIRST_SFI;
    }
    else
    { 
	    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
		    cellState = CELL_WAITING_FOR_FIRST_SFI;
	    msgBuf = (void *)getMemFromPool(2000/*FAPI_CTRL_MSG_MAX_BUF_SIZE*/, PNULL);
	    if (PNULL == msgBuf)
	    {
		    return MAC_FAILURE;  
	    }

            internalCellIndex = 0;

	    while(1)
	    {
		    flexran_ipc_wait_phy_ready(internalCellIndex);

		    /* Check if PHY has sent ERROR IND in response to MAC's PHy PARAM REQ */
		    g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_ERROR_IND;

		    bytesRead = recvControlInfoFromPhysical((void *)msgBuf,internalCellIndex);

		    if( bytesRead > 0)
		    {
			    /* PHY sent ERROR IND for MAC's PHY PARAM REQ*/
			    fprintf(stderr,"\n\nERROR !!!!! PHY sent ERROR IND in response to MAC's PHY START REQ\n\n");
			    break;

		    }
		    else
		    {
			    /* All Good, Nothing received on ERROR IND channel */
			    g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_SUBFRAME_IND;



			    /* TBD : DO we need to retry sending PHY START REQ and 
			     * thus send FAIL to RRC if PHY does not sends First SFI*/

			    bytesRead = recvControlInfoFromPhysical((void *)msgBuf, internalCellIndex);
		    }

		    if (bytesRead > 0)
		    {
			    fprintf(stderr,"\n\nReceived FIRST SFI ,byteRead[%d], cellId [%d] starting processing \n\n",bytesRead, internalCellIndex);

			    lteMacStartPhyHandler((void *)msgBuf, bytesRead,internalCellIndex);

			    if (internalCellIndex == 0 && macParams_g.numCellsConfigured == 2)
                            {
                               internalCellIndex = 1;
                            }
                            else
                            {    
                                 break;
                            }
		    }
	    }

	    freeMemPool(msgBuf);
	    msgBuf = PNULL;
    }
    return resp;  
}
#else
MacRetType sendStartReqToPhy(InternalCellIndex internalCellIndex)
    /* CA Changes start */
{
    FAPI_l1ApiMsg_st *pStartReq_p = NULL;
    MacRetType resp = MAC_FAILURE;
/* SPR 19288 change start */
/* SPR 19288 change end */

    UInt16 startMsgLen = sizeof(FAPI_l1ApiMsg_st);
    GET_MEM_FROM_POOL(FAPI_l1ApiMsg_st,pStartReq_p,startMsgLen, NULL); 
    /* coverity 530 CID 25168*/
    if(PNULL == pStartReq_p)
    {
        return MAC_FAILURE;    
    }
    /* coverity 530 CID 25168*/

    memSet(pStartReq_p, 0, startMsgLen);

    pStartReq_p->msgId = PHY_START_REQUEST;
    /* PHY Start request has no payload */
    pStartReq_p->msgLen = 0;

    /* Send the Config Request to the PHY layer */
    /* CA Changes start */
    if( sendControlInfoToPhy((UInt8 *)pStartReq_p, startMsgLen,internalCellIndex) >=  0)
        /* CA Changes end */
    {
        resp = MAC_SUCCESS;

    }
    freeMemPool(pStartReq_p);
    /* SPR 8194 Fix Start */
    LOG_MAC_MSG(MAC_PHY_START_REQ,LOGDEBUG,MAC_L1_INF,
            PHY_CELL_START_REQ,
            MAC_MODULE_ID,PHY_MODULE_ID,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"SEND_START_PHY_REQ");
    /* SPR 8194 Fix End */

    LTE_MAC_UT_LOG(LOG_INFO, CELL_CONF,"Sent Start Request to PHY");
/* SPR 19288 change start */
/* SPR 19288 change end */

   return resp;  
}
#endif 


/*****************************************************************************
 * Function Name  : sendStopReqToPhy
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : This function is used by MAC to STOP/DELETE the PHY Layer. 
 *       		    The msg while PHY is in running state.
 *****************************************************************************/
/* CA Changes start */
MacRetType sendStopReqToPhy(InternalCellIndex internalCellIndex)
    /* CA Changes end */
{
    FAPI_l1ApiMsg_st *pStopReq_p = NULL;
    MacRetType resp = MAC_FAILURE;
    UInt16 stopMsgLen = L1_PARTIAL_API_MSG_SIZE;
    GET_MEM_FROM_POOL(FAPI_l1ApiMsg_st,pStopReq_p,stopMsgLen, NULL); 
    /* coverity 530 CID 25169*/
    if(PNULL == pStopReq_p)
    {
        return MAC_FAILURE;    
    }
    /* coverity 530 CID 25169*/

    memSet(pStopReq_p, 0, stopMsgLen);

    pStopReq_p->msgId = PHY_STOP_REQUEST;

    /* PHY Stop request has no payload */
    pStopReq_p->msgLen = 0;
/* SPR 19288 change start */
#ifdef FLEXRAN
    g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_STOP_REQ ;
#endif
/* SPR 19288 change end */

    /* Send the Stop Request to the PHY layer */
    /* CA Changes start */
    if (sendControlInfoToPhy((UInt8 *)pStopReq_p, stopMsgLen,internalCellIndex) > 0)
        /*SPR 21001 Changes end */
        /* CA Changes end */
    {
        resp = MAC_SUCCESS;

    }
    freeMemPool(pStopReq_p);
    LTE_MAC_UT_LOG(LOG_INFO, CELL_CONF,"Sent Stop Request to PHY");

    return resp;  
}




/*****************************************************************************
 * Function Name  : parseAndSendConfigPhyCell
 * Inputs         : zeroBuffer_p - pointer of void type,
 *                  msg_p - message buffer to store msg,
 *                  totalMsgLen - total msg length,
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is used by MAC to configure PHY Cell. It shall
 *                  parse and send Cell config request to PHY and also receive the response.
 *****************************************************************************/
/* CA Changes start */
#if   FLEXRAN
MacRetType parseAndSendConfigPhyCell(UInt8 *msg_p, 
        UInt16 totalMsgLen, InternalCellIndex internalCellIndex)
{
    UInt16           msgLen    = 0;
    FAPI_l1ApiMsg_st *pL1ConfigReq_p = PNULL;
    MacRetType       resp      = MAC_FAILURE;
    UInt8            reTry     = 0;
    void*            msgBuf    = PNULL;
    SInt32           bytesRead = 0;
    SInt32           byteSent = 0;
    UInt8             isPhyErrorRcvd = FALSE;
    struct timespec timer;

    /* Parse the recived RRC PHY cell config message and fill it in a structure */
    if (MAC_FAILURE == parseCellConfigPhyReq(msg_p, totalMsgLen,internalCellIndex))
    {
        LOG_MAC_WARNING( MAC_RRC_INF, "Error parsing Cell config request "
                "from RRC" );
        printf("\nError parsing Cell config request from RRC\n");
        return resp;
    }

    /* Create the PHY Cell configuration message as per FF MAC-PHY interface */
    if (MAC_FAILURE == fillPhyConfigRequest(&pL1ConfigReq_p, &msgLen, internalCellIndex))
    {
        LTE_MAC_UT_LOG(LOG_WARNING, CELL_CONF,"Error filling Cell config request "
                "for PHY");
        printf("\nError filling Cell config request for PHY\n");
        return resp;
    }


    while( reTry < FAPI_MAX_PARAM_REQUEST_RETRY )
    {

        g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_CFG_REQ; 

        byteSent = sendControlInfoToPhy((UInt8 *)pL1ConfigReq_p, msgLen,internalCellIndex);
        LOG_MAC_MSG(MAC_PHY_CELL_CONFIG_REQ,LOGDEBUG,MAC_L1_INF,
                getCurrentTick(),
                PHY_CONFIG_CELL_REQ,
                MAC_MODULE_ID,PHY_MODULE_ID,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"SEND_CONFIG_PHY_CELL_REQ");

        if(byteSent)
        {
            resp = MAC_SUCCESS; 
	    fprintf (stderr, "| <<--- PHY-CONFIG-REQUEST ----------- | |                                   |\n");

            LOG_MAC_MSG(MAC_PHY_PARAM_REQ, LOGDEBUG, MAC_L1_INF, getCurrentTick(),
                    PHY_PARAM_REQUEST, MAC_MODULE_ID, PHY_MODULE_ID,
                    DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"SEND_PHY_PARAM_REQ");

            printf("\n\nSent PHY_CELL_CONFIG_REQUEST,Retry Count[%d]\n\n",reTry);
        }
        /* Wait for PHY to respond with PHY_CELL_CONFIG_RESP */
        timer.tv_nsec = 500000000;
        timer.tv_sec = 0;
        if (-1 == pselect_wrapper(1, PNULL, PNULL, PNULL, &timer, PNULL))
        {
            perror("select failed\n");
        }

        /* Check if PHY is ready */
        flexran_ipc_wait_phy_ready(internalCellIndex);


        /* Check if PHY has sent ERROR IND in response to MAC's PHy PARAM REQ */
        g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_ERROR_IND;
        bytesRead = recvControlInfoFromPhysical((void *)msgBuf,internalCellIndex);
        if( bytesRead > 0)
        {
            /* PHY sent ERROR IND for MAC's PHY PARAM REQ*/
            printf("\n\nERROR !!!!! PHY sent ERROR IND in response to MAC's CELL CONFIG REQ\n\n");
            isPhyErrorRcvd = TRUE;
            break;

        }
        else
        {
            /* All Good, Nothing received on ERROR IND channel */

            g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_CFG_RSP;

            msgBuf = (void *)getMemFromPool(2000/*FAPI_CTRL_MSG_MAX_BUF_SIZE*/, PNULL);
            if (PNULL == msgBuf)
            {
                printf("\n\nERROR !!! Unable to get memory for PHy PARAM REQ .... \n\n");

                freeMemPool(pL1ConfigReq_p);
                pL1ConfigReq_p = NULL;
                return MAC_FAILURE;  
            }

            bytesRead = recvControlInfoFromPhysical((void *)msgBuf, internalCellIndex);

            if( bytesRead <=0 )
            {
                printf("\n\nNOT valid response rcvd for PHY_CELL_CONFIG_REQUEST,bytesRead[%d] .... \n\n",bytesRead);
            }
            else
            {
                printf("\n\nResponse rcvd for PHY_CELL_CONFIG_REQUEST,bytesRead[%d]. Now processing.... \n\n",bytesRead);

                lteMacConfigCellReqPhyHandler((void *)msgBuf, bytesRead,internalCellIndex);

                if(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                        cellState == CELL_PHY_CONFIG)
                {
                    break;
                }
            }
        }
        reTry++;
    }
    /** Check if no resp rcvd from PHY after max RETRY , Send FAIL to RRC */

    if( isPhyErrorRcvd == TRUE || bytesRead <=0 ) // PHY ERROR or No response rcvd from PHY even after max retry
    {
        UInt32 trans_id = 0xFFFFFFFF;

        //trans_id = transactionId_g[internalCellIndex];

        sendConfigPhyCellRes(trans_id,MAC_FAILURE,internalCellIndex);
        //transactionId_g[internalCellIndex] = INVALID_TRANS_ID;
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellState = CELL_PHY_INIT;
    }


    freeMemPool(pL1ConfigReq_p);
    pL1ConfigReq_p = PNULL;

    freeMemPool(msgBuf);
    msgBuf = PNULL;
    return MAC_SUCCESS;
}

#else
MacRetType parseAndSendConfigPhyCell(UInt8 *msg_p, 
        UInt16 totalMsgLen, InternalCellIndex internalCellIndex)
{
    UInt16  msgLen = 0;
    FAPI_l1ApiMsg_st *pL1ConfigReq_p = PNULL;
    /* SPR 1494 changes start */ 
    /* + coverity 32192 */
    MacRetType  resp = MAC_FAILURE;
    /* - coverity 32192 */
    /* SPR 1494 changes end */ 
    /* SPR 19288 change start */
    /* SPR 19288 change end */

    /* Parse the recived RRC PHY cell config message and fill it in a structure */
    if (MAC_FAILURE == parseCellConfigPhyReq(msg_p, totalMsgLen,internalCellIndex))
    {
        LOG_MAC_WARNING( MAC_RRC_INF, "Error parsing Cell config request "
                "from RRC" );
        return resp;
    }

    /* LAA parameters are stored but will be sent to PHY in LAA-INIT request 
     * and not PHY.Config Request. Please note this can be changed once FAPI
     * specifications are available. */

    /* Create the PHY Cell configuration message as per FF MAC-PHY interface */
    if (MAC_FAILURE == fillPhyConfigRequest(&pL1ConfigReq_p, &msgLen, internalCellIndex))
    {
        LTE_MAC_UT_LOG(LOG_WARNING, CELL_CONF,"Error filling Cell config request "
                "for PHY");
        return resp;
    }
    /* SPR 19288 change start */
            /* SPR 19288 change end */


            sendControlInfoToPhy((UInt8 *)pL1ConfigReq_p, msgLen,internalCellIndex);
            /* SPR 8194 Fix Start */
            LOG_MAC_MSG(MAC_PHY_CELL_CONFIG_REQ,LOGDEBUG,MAC_L1_INF,
                    getCurrentTick(),
                    PHY_CONFIG_CELL_REQ,
                    MAC_MODULE_ID,PHY_MODULE_ID,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"SEND_CONFIG_PHY_CELL_REQ");
            /* SPR 8194 Fix End */

            /* SPR 19288 change start */

    /* SPR 19288 change end */
    freeMemPool(pL1ConfigReq_p);
    pL1ConfigReq_p = NULL;
    return MAC_SUCCESS;
}
#endif


/*****************************************************************************
 * Function Name  : SendConfigPhyCell
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is used by MAC to configure PHY Cell. It shall
 *                  send Cell config request to PHY and also receive the response.
 *****************************************************************************/

MacRetType sendConfigPhyCell(InternalCellIndex internalCellIndex)
{
    FAPI_l1ApiMsg_st *pL1ConfigReq_p = PNULL;
    UInt16  msgLen = 0;
    /* SPR 1494 changes start */ 
    /* + coverity 32194 */
    MacRetType  resp = MAC_SUCCESS;
    /* - coverity 32194 */
    /* SPR 1494 changes end */ 

    /* Create the PHY Cell configuration message as per FF MAC-PHY interface */
    if (MAC_FAILURE == fillPhyConfigRequest(&pL1ConfigReq_p, &msgLen,internalCellIndex))
    {
        LTE_MAC_UT_LOG(LOG_WARNING, CELL_CONF,"Error filling Cell config request "
                "for PHY");
        return MAC_FAILURE;
    }

    sendControlInfoToPhy((UInt8 *)pL1ConfigReq_p, msgLen,internalCellIndex);

    freeMemPool(pL1ConfigReq_p);
    pL1ConfigReq_p = NULL;
    return resp;
}
/*coverity 29849,10168*/
/*****************************************************************************
 * Function Name  : getPhyCellConfigRespPtr
 * Inputs         : pL1ConfigRes_p 
 * Outputs        : None
 * Returns        : Pointer to FAPI_phyCellConfigResp_st
 * Description    : 
 ******************************************************************************/

static  FAPI_phyCellConfigResp_st * getPhyCellConfigRespPtr(FAPI_l1ApiMsg_st *pL1ConfigRes_p)
{
    return (FAPI_phyCellConfigResp_st *)&pL1ConfigRes_p->msgBody[0];
}
/*coverity 29849,10168*/

/*****************************************************************************
 * Function Name  : handleConfigPhyCellRes
 * Inputs         : msgBuf - for stoing msg in a bufer and
 *                  bytesRead - to track number of byts read
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is used by MAC to handle the response of PHY Cell.
 *****************************************************************************/
UInt32 handleConfigPhyCellRes( UInt8 * msgBuf, SInt32 bytesRead)
{

    UInt16  iterator = 0;
    FAPI_l1ApiMsg_st *pL1ConfigRes_p = PNULL;
    FAPI_phyCellConfigResp_st *pConfigRes_p = PNULL;
#ifdef LOG_PRINT_ENABLED
    FAPI_cellConfig_st   *listOfMissingTlvs_p = PNULL;
    FAPI_cellConfig_st   *listOfInvalidTlvs_p = PNULL;
#endif
    UInt32 resp = MAC_FAILURE;

    if ( (bytesRead > 0) &&
            (bytesRead <= (SInt32)sizeof(FAPI_l1ApiMsg_st)) )
    {
        LOG_MAC_MSG(PHY_MAC_BYTES_READ,LOGERROR,MAC_L1_INF,
                getCurrentTick(), 
                __LINE__,bytesRead,sizeof(FAPI_l1ApiMsg_st),0,0,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                __func__,"CONFIG Request");    
    }
    else if (bytesRead > 0) 
    {
        pL1ConfigRes_p = (FAPI_l1ApiMsg_st *)msgBuf;

        /* Check if the response is for Config request and whether it is success or
         * failure */
        if (PHY_CELL_CONFIG_RESPONSE == pL1ConfigRes_p->msgId)
        {
            /*coverity 29849*/
            pConfigRes_p = getPhyCellConfigRespPtr(pL1ConfigRes_p);
            /*coverity 29849*/

            if(FAPI_MSG_OK == pConfigRes_p->errorCode)  
            {
                LTE_MAC_UT_LOG(LOG_INFO, CELL_CONF,"PHY Cell Configuration "
                        "Success");
/* SPR 19288 change start */
/* SPR 19288 change end */
                resp = MAC_SUCCESS;
            }
            else
            {
                LOG_MAC_MSG(PHY_CELL_CONFIG_FAIL,LOGERROR,MAC_L1_INF,
                         getCurrentTick(),pConfigRes_p->errorCode, 
                        pConfigRes_p->numOfMissingTLV, pConfigRes_p->numOfInvalidOrUnsupportedTLV,
                        __LINE__,bytesRead,
                         DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        __func__,"CONFIG Request");
         
                /* Log the Missing TLVs*/
                if(pConfigRes_p->numOfMissingTLV)
                {
                    for(iterator = 0; iterator < pConfigRes_p->numOfMissingTLV; 
                            iterator++) 
                    {
#ifdef LOG_PRINT_ENABLED
                        if(pConfigRes_p->numOfInvalidOrUnsupportedTLV)
                        {
                        listOfMissingTlvs_p = (FAPI_cellConfig_st *)(((UInt8 *)pConfigRes_p)+4+(sizeof(FAPI_cellConfig_st)*(pConfigRes_p->numOfInvalidOrUnsupportedTLV+iterator)));
                        }
                        else
                        {    
                        listOfMissingTlvs_p = (FAPI_cellConfig_st *)(((UInt8 *)pConfigRes_p)+4+(sizeof(FAPI_cellConfig_st)*(1+iterator)));
                        }
#endif
#ifdef MAC_AUT_TEST
                        missingorInvalidTlv_g[0]=listOfMissingTlvs_p->tag;
#endif
                        /*SPR 13698 fix end */
                        LOG_MAC_MSG(PHY_MAC_TLV_TAG,LOGWARNING,MAC_L1_INF,
                                getCurrentTick(),__LINE__, 
                                listOfMissingTlvs_p->tag, 0,
                                0,0,
                                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                                __func__,"Missing");
                    }
                }

                /* Log the Unsupported TLVs*/
                if(pConfigRes_p->numOfInvalidOrUnsupportedTLV)
                {
                    for(iterator = 0; iterator < pConfigRes_p->
                            numOfInvalidOrUnsupportedTLV; iterator++) 
                    {
#ifdef LOG_PRINT_ENABLED
                        listOfInvalidTlvs_p = &pConfigRes_p->listOfTLV[iterator];
#endif                        
                        /*SPR 13698 fix start */
#ifdef MAC_AUT_TEST
                        missingorInvalidTlv_g[1]=listOfInvalidTlvs_p->tag;
#endif
                        /*SPR 13698 fix end */
                        LOG_MAC_MSG(PHY_MAC_TLV_TAG,LOGWARNING,MAC_L1_INF,
                                getCurrentTick(),__LINE__, 
                                listOfInvalidTlvs_p->tag, 0,
                                0,0,
                                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                                __func__,"Invalid");
                    }
                }
            }
        }
        else
        {
            /* fapi changes start */
            LOG_MAC_MSG(PHY_MAC_INVALID_MSG,LOGERROR,MAC_L1_INF,
                    getCurrentTick(), 
                    pL1ConfigRes_p->msgId,PHY_MODULE_ID,MAC_MODULE_ID,
                    __LINE__,bytesRead,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    __func__,"invalidmsgresp");
            /* fapi changes end */ 
        }
    }
    return resp;
    /* SPR 1494 changes end */ 
}

/*****************************************************************************
 * Function Name  : fapiParsePhyParamResp 
 * Inputs         : paramResp_p - pointer to structure FAPI_paramResponse_st,
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function parse PHY_PARAM_RESPONSE received from phy 
 *                  and check for phy state/ if PHY is idle then return 
 *                  MAC_SUCCESS else return MAC_FAILURE.
 *                  At present MAC is not considering any other TLV coming in 
 *                  PHY_PARAM_RESPONSE. 
 ******************************************************************************/
/* CA Changes start */
MacRetType fapiParsePhyParamResp(FAPI_paramResponse_st *paramResp_p, 
        InternalCellIndex internalCellIndex)
{
    UInt8 i = 0;
    UInt8 *ptr = PNULL; 
    FAPI_paramResponseTLV_st *paramRspTlv_p = PNULL;
#ifdef LTE_EMBMS_SUPPORTED
    /*SPR 18013 fix start */
    UInt16 mbsfnCapability = PHY_EMBMS_NOT_SUPPORTED;
    /*SPR 18013 fix end */
#endif
    if ( FAPI_MSG_OK != paramResp_p->errCode )
    {
        return MAC_FAILURE;
    }
    ptr = ( UInt8 *) &(paramResp_p->tlvs[0]); 
    while ( i < paramResp_p->numOfTlv )
    {
        paramRspTlv_p = ( FAPI_paramResponseTLV_st *) ptr;
        switch ( paramRspTlv_p->tag )
        {
            case FAPI_PHY_STATE:
                {
                if ( FAPI_IDLE ==  
                        MAC_PHY_CONVERT_16(paramRspTlv_p->configParam.phyState))                 
                    {
                        /* SPR 1308 changes start */
                        /*
                           LTE_MAC_UT_LOG(LOG_CRITICAL, CELL_CONF,"[%s]exit with"
                           "MAC_SUCCESS \n",__func__); 
                           return MAC_SUCCESS;
                         */
                        /* SPR 1308 changes end */
                    }
                    else
                    {
                        return MAC_FAILURE;
                    }
                }

                /* SPR 1308 changes start */
                break;


            case  FAPI_DL_BANDWIDTH_SUPPORT:
                {
                phyParamResp_g[internalCellIndex].dlBandWidthSupport = 
                    MAC_PHY_CONVERT_16( paramRspTlv_p->\
                            configParam.dlBandWidthSupport);
                }			
                break;

            case   FAPI_UL_BANDWIDTH_SUPPORT:
                {
                phyParamResp_g[internalCellIndex].ulBandWidthSupport  = 
                    MAC_PHY_CONVERT_16( paramRspTlv_p->\
                            configParam.ulBandWidthSupport);
                }
                break;

            case  FAPI_DL_MODULATION_SUPPORT:
                {
                phyParamResp_g[internalCellIndex].dlModulationSupport = 
                    MAC_PHY_CONVERT_16( paramRspTlv_p->\
                            configParam.dlModulationSupport);

                }
                break;

            case  FAPI_UL_MODULATION_SUPPORT:
                {
                phyParamResp_g[internalCellIndex].ulModulationSupport  = 
                    MAC_PHY_CONVERT_16( paramRspTlv_p->\
                            configParam.ulModulationSupport);	                    
                }
                break;

            case  FAPI_PHY_ANTENNA_CAPABILITY:
                {
                phyParamResp_g[internalCellIndex].phyAntennaCapability = 
                    MAC_PHY_CONVERT_16( paramRspTlv_p->\
                            configParam.phyAntennaCapability);	                    
            }
            break;
            /* SPR 1308 changes end */
#ifdef FAPI_4_0_COMPLIANCE
            /*FAPI2_1 changes start*/ 
            case  FAPI_PHY_RELEASE_CAPABILITY:
            {
                /* phy release fix */

                if( (MAC_PHY_CONVERT_16(paramRspTlv_p->configParam.releaseCapability) & RELEASE10_MASK )
                    )
                {
                    phyReleaseCapability_g=RELEASE_10;
                }
                else if(MAC_PHY_CONVERT_16(paramRspTlv_p->configParam.releaseCapability) & RELEASE9_MASK)
                {
                    phyReleaseCapability_g=RELEASE_9;
                }
                else if(MAC_PHY_CONVERT_16(paramRspTlv_p->configParam.releaseCapability) & RELEASE8_MASK)
                {
                    phyReleaseCapability_g=RELEASE_8;
                }
                /** SPR 13147 Fix : Start **/
                if (macParams_g.isCASupported && 
                        ((RELEASE_8 == phyReleaseCapability_g) ||(RELEASE_9 ==phyReleaseCapability_g)))
                {
                    /* Coverity 66973 Start */
                    return MAC_FAILURE;
                    /* Coverity 66973 End */
                }
#ifndef FLEXRAN
                /** SPR 13147 Fix : End **/
                switch (phyReleaseCapability_g)
                {
                    case RELEASE_8 :
                    {
                        addDlschToDLConfigFunction = addDlschPduToDlConfigR8;
                        /* + SPR_15163 */
                        addDciToDLConfigFunction =  addDciPduToDlConfigR10;
                        /* - SPR_15163 */
                        prepareUlschPduFunction = prepareUlschPdu;
                        prepareUlschCqiRiPduFunction = prepareUlschCqiRiPdu;
                        prepareUlschCqiHarqRiPduFunction = prepareUlschCqiHarqRiPdu;
                        fillDciFormat0PduFunction = fillDciFormat0Pdu;
                        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
                        /* SPR 20011 Fix Start */
                        /*SPR 13791 changes start */
                        processULSchIndFunction = processULSchIndR9;
                        /*SPR 13791 changes end */
                        /* SPR 20011 Fix End */
                        /* SPR 19679: TDD HARQ Multiplexing Changes End */
                        processCQIIndicationFunction = processCQIIndicationR8;
                        processRACHIndicationFunction = processRACHIndication;
                        processSRSIndicationFunction = processSRSIndication;
                        /*Adding HARQ.Indication handling*/
                        /*FAPI2_1 changes start*/
                        processHARQIndicationFunction=processUlHarqIndR8;
                        /* FAPI2.4 Changes Start */
                        lteMacUpdateFapiRachIndStatsFunction=lteMacUpdateFapiRachIndStats;
                        lteMacUpdateFapiRxUlschStatsFunction=lteMacUpdateFapiRxUlschStats;
                        lteMacUpdateFapiCqiIndStatsFunction=lteMacUpdateFapiCqiIndStats;
                        lteMacUpdateFapiHarqIndStatsFunction=lteMacUpdateFapiHarqIndStatsR8;
                        /* FAPI2.4 Changes End */

                        prepareUciHarqPduFunction=prepareUciHarqPdu;
                        prepareUciSrHarqPduFunction=prepareUciSrHarqPdu;
                        prepareUciCqiHarqPduFunction=prepareUciCqiHarqPdu;
                        prepareUciCqiSrHarqPduFunction=prepareUciCqiSrHarqPdu;

                        /*FAPI2_1 changes end*/
                        /*Adding HARQ.Indication handling*/
                        break;
                    }
                    case RELEASE_9:
                    {
                        addDlschToDLConfigFunction = addDlschPduToDlConfigR9;
                        addDciToDLConfigFunction = addDciPduToDlConfigR9;
                        prepareUlschPduFunction = prepareUlschPdu;
                        prepareUlschCqiRiPduFunction = prepareUlschCqiRiPduR9;
                        prepareUlschCqiHarqRiPduFunction = prepareUlschCqiHarqRiPduR9;
                        fillDciFormat0PduFunction = fillDciFormat0Pdu;
                        processULSchIndFunction = processULSchIndR9;
                        processCQIIndicationFunction = processCQIIndicationR9;
                        processRACHIndicationFunction = processRACHIndicationR9;
                        processSRSIndicationFunction = processSRSIndicationR9;
                        /*Adding HARQ.Indication handling*/
                        /*FAPI2_1 changes start*/
                        processHARQIndicationFunction=processUlHarqIndR9;
                        /* FAPI2.4 Changes Start */
                        lteMacUpdateFapiRachIndStatsFunction=lteMacUpdateFapiRachIndStatsR9;
                        lteMacUpdateFapiRxUlschStatsFunction=lteMacUpdateFapiRxUlschStatsR9;
                        lteMacUpdateFapiCqiIndStatsFunction=lteMacUpdateFapiCqiIndStatsR9;
                        lteMacUpdateFapiHarqIndStatsFunction=lteMacUpdateFapiHarqIndStatsR9;
                        /* FAPI2.4 Changes End */
                        prepareUciHarqPduFunction=prepareUciHarqPduR9;
                        prepareUciSrHarqPduFunction=prepareUciSrHarqPduR9;
                        prepareUciCqiHarqPduFunction=prepareUciCqiHarqPduR9;
                        prepareUciCqiSrHarqPduFunction=prepareUciCqiSrHarqPduR9; 

                        /*FAPI2_1 changes end*/
                        /*Adding HARQ.Indication handling*/
                        break;
                    }
                    case RELEASE_10:
                /* phy release fix */
                    {
                        addDlschToDLConfigFunction = addDlschPduToDlConfigR10;
                        addDciToDLConfigFunction = addDciPduToDlConfigR10;
                        prepareUlschPduFunction = prepareUlschPduR10;
                        prepareUlschCqiHarqRiPduFunction = prepareUlschCqiHarqRiPduR9;
                        prepareUlschCqiRiPduFunction = prepareUlschCqiRiPduR9;
                        fillDciFormat0PduFunction = fillDciFormat0PduR10;
                        processULSchIndFunction = processULSchIndR9;
                        processCQIIndicationFunction = processCQIIndicationR9;
                        processRACHIndicationFunction = processRACHIndicationR9;
                        processSRSIndicationFunction = processSRSIndicationR9;
                        /*Adding HARQ.Indication handling*/
                        /*FAPI2_1 changes start*/
                        processHARQIndicationFunction=processUlHarqIndR9;
                        /* FAPI2.4 Changes Start */
                        lteMacUpdateFapiRachIndStatsFunction=lteMacUpdateFapiRachIndStatsR9;
                        lteMacUpdateFapiRxUlschStatsFunction=lteMacUpdateFapiRxUlschStatsR9;
                        lteMacUpdateFapiCqiIndStatsFunction=lteMacUpdateFapiCqiIndStatsR9;
                        lteMacUpdateFapiHarqIndStatsFunction=lteMacUpdateFapiHarqIndStatsR9;
                        /* FAPI2.4 Changes End */
                        prepareUciHarqPduFunction=prepareUciHarqPduR9;
                        prepareUciSrHarqPduFunction=prepareUciSrHarqPduR9;
                        prepareUciCqiHarqPduFunction=prepareUciCqiHarqPduR9;
                        prepareUciCqiSrHarqPduFunction=prepareUciCqiSrHarqPduR9; 

                        /*FAPI2_1 changes end*/
                        /*Adding HARQ.Indication handling*/
                        break;
                    }
                }
#endif
            }
            break;
#endif
#ifdef LTE_EMBMS_SUPPORTED
            case  FAPI_PHY_MBSFN_CAPABILITY:
            {
                /*SPR 18013 fix start */
                mbsfnCapability = MAC_PHY_CONVERT_16( paramRspTlv_p->\
                        configParam.mbsfnCapability);
                if(PHY_EMBMS_SUPPORTED == mbsfnCapability)
                {
                    phyParamResp_g[internalCellIndex].phyMbsfnCapability = PHY_EMBMS_SUPPORTED;
                }
                else
                {
                    phyParamResp_g[internalCellIndex].phyMbsfnCapability = PHY_EMBMS_NOT_SUPPORTED;
                }
                /*SPR 18013 fix end */
            }     
            break;
#endif


            /*FAPI2_1 changes end*/ 

            default:    
                LTE_MAC_UT_LOG(LOG_CRITICAL, CELL_CONF,"Not supported Tag %d" 
                        "received in PHY_PARAM_RESP\n",paramRspTlv_p->tag);
                break;
        }
        /*SPR 7095 change start */      
        ptr += (paramRspTlv_p->tagLen + sizeof(paramRspTlv_p->tag) + 
                sizeof(paramRspTlv_p->tagLen));
        /*SPR 7095 change End */       
        i++;
    }

    /* SPR 1308 chnages start */
    /*
       LTE_MAC_UT_LOG(LOG_CRITICAL, CELL_CONF,"[%s]exit with" 
       "MAC_FAILURE \n",__func__); 
       return MAC_FAILURE;
     */
/* SPR 19288 change start */
/* SPR 19288 change end */
    return MAC_SUCCESS;
    /* SPR 1308 chnages End */
}
/* UL-SUBFRAME Code Changes Start */

/* UL-SUBFRAME Code Changes End */

/*****************************************************************************
 * Function Name  : fapiSendPhyParamReq
 * Inputs         : internalCellIndex
 * Outputs        : None 
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function sends PHY_PARAM_REQUEST to PHY.
 ******************************************************************************/
/* CA Changes start */
#if   FLEXRAN 
MacRetType fapiSendPhyParamReq(InternalCellIndex internalCellIndex )
{
    UInt8 startReqSent_t = 0;
    FAPI_l1ApiMsg_st *paramReq_p    = PNULL;
    UInt16           paramReqMsgLen = sizeof(FAPI_l1ApiMsg_st);
    MacRetType       resp           = MAC_FAILURE;
    UInt8            validResponse  = FALSE;

    void*   msgBuf = PNULL;


    GET_MEM_FROM_POOL(FAPI_l1ApiMsg_st,paramReq_p,paramReqMsgLen, PNULL);

    if ( PNULL == paramReq_p )
    {
        printf("\n\nERROR !!! Unable to get memory for PHY PARAM REQ .... \n\n");
        return MAC_FAILURE;    
    }

        SInt32 byteSent  = 0;
        SInt32 bytesRead = 0;

        validResponse = FALSE;

        memSet(paramReq_p, 0 ,paramReqMsgLen);
        paramReq_p->msgId = PHY_PARAM_REQUEST;
        paramReq_p->msgLen = 0;

        /* Set the Channel ID to send PHY PARAM REQUEST */
        g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_PARA_REQ; 

        byteSent =  sendControlInfoToPhy((UInt8 *)paramReq_p, 
                paramReqMsgLen,
                internalCellIndex);

	if(byteSent)
	{
		resp = MAC_SUCCESS; 
		fprintf (stderr, "| <<--- PHY-PARAM-REQUEST ----------- | |                                   |\n");

            LOG_MAC_MSG(MAC_PHY_PARAM_REQ, LOGDEBUG, MAC_L1_INF, getCurrentTick(),
                    PHY_PARAM_REQUEST, MAC_MODULE_ID, PHY_MODULE_ID,
                    DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"SEND_PHY_PARAM_REQ");
	    
            msgBuf = (void *)getMemFromPool(2000/*FAPI_CTRL_MSG_MAX_BUF_SIZE*/, PNULL);
	    if (PNULL == msgBuf)
	    {
		    printf("\n\nERROR !!! Unable to get memory for PHy PARAM REQ .... \n\n");

		    freeMemPool(paramReq_p);
		    return MAC_FAILURE;  

	    }
            startReqSent_t = 1;
        }
        else
        {
              startReqSent_t = 0;
        }
        flexran_ipc_notify_mac_ready(internalCellIndex);
        /* Wait for PHY to respond and then receive the PHY PARAM RESPONSE from PHY layer */
        /* Check if PHY is ready */
	while( startReqSent_t == 1)
	{
            flexran_ipc_wait_phy_ready(internalCellIndex);

        /* Check if PHY has sent ERROR IND in response to MAC's PHy PARAM REQ */
        g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_ERROR_IND;
        bytesRead = recvControlInfoFromPhysical((void *)msgBuf,internalCellIndex);
        if( bytesRead > 0)
        {
            /* PHY sent ERROR IND for MAC's PHY PARAM REQ*/
            printf("\n\nERROR !!!!! PHY sent ERROR IND in response to MAC's PHY PARAM REQ\n\n");
            break;

        }
        else
        {
            /* All Good, Nothing received on ERROR IND channel */


            /* Set the Channel ID to Receive PHY PARAM RESPONSE */
            g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_PARA_RSP;

            bytesRead = recvControlInfoFromPhysical((void *)msgBuf,internalCellIndex);

            if ( bytesRead > sizeof(FAPI_l1ApiMsg_st) )
            {
                /* Successfully received PHY PARAM RESPONSE, start processing it. */
                printf("\n\n Received PHY PARAM RESPONSE\n\n");
                validResponse = TRUE;
                lteMacMacInitLayerPhyHandler(msgBuf,bytesRead,internalCellIndex);
                break;
            }
            else if ( (bytesRead > 0) &&
                    (bytesRead <= sizeof(FAPI_l1ApiMsg_st)))
            {
                printf("\n\n ERROR !!!  bytesRead = %d is less than expected in param resp msgbody size%lu\n\n",
                        bytesRead,sizeof(FAPI_l1ApiMsg_st));

                LTE_MAC_UT_LOG(LOG_CRITICAL, CELL_CONF,"[%s] returned"
                        " MAC_FAILURE reason bytesRead = %d is less"
                        " than expected in param resp msgbody size%d \n",
                        __func__,bytesRead, sizeof(FAPI_l1ApiMsg_st));
                break;
            }
            else if(bytesRead <=0)
            {
                printf("\n\nNo or NOT valid response rcvd for PHY PARAM REQ,bytesRead[%d] .... \n\n",bytesRead);
                /* Seems nothing on channel, Continue till max retry */
            }
        }
    }

    /* Check if no/invalid resp rcvd from PHY after max RETRY , send FAIL to OAM */ 
    if( validResponse == FALSE)
    {
        resp = MAC_FAILURE;
        UInt32 trans_id = 0xFFFFFFFF;

        sendMacInitLayerRes(trans_id,MAC_FAILURE,internalCellIndex);
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellState = CELL_INIT;
    }
    else
    {
       resp = MAC_SUCCESS;
    }

    freeMemPool(paramReq_p);
    freeMemPool(msgBuf);
    paramReq_p = PNULL;	
    return resp;
}
#else
MacRetType fapiSendPhyParamReq(InternalCellIndex internalCellIndex )
    /* CA Changes end */
{
/* SPR 19288 change start */
    FAPI_l1ApiMsg_st *paramReq_p= PNULL;
    UInt16 paramReqMsgLen = sizeof(FAPI_l1ApiMsg_st);
    MacRetType resp = MAC_FAILURE; 
    SInt32 byteSent = 0;
    GET_MEM_FROM_POOL(FAPI_l1ApiMsg_st,paramReq_p,paramReqMsgLen, PNULL);
    /*+COVERITY 5.3.0 - 32100*/
    if ( PNULL == paramReq_p )
    {
        return MAC_FAILURE;    
    }
    /*-COVERITY 5.3.0 - 32100*/

    memSet(paramReq_p, 0 ,paramReqMsgLen);

    paramReq_p->msgId = PHY_PARAM_REQUEST;
    /* PHY PARAM request has no payload */
    paramReq_p->msgLen = 0;
/*SPR 21001 Changes end */
/* SPR 19288 change end */
    /* CA Changes start */
    byteSent =  sendControlInfoToPhy((UInt8 *)paramReq_p, paramReqMsgLen,
            internalCellIndex);
    /* CA Changes end */
    if(byteSent)
    {
        resp = MAC_SUCCESS; 
        fprintf (stderr, "| <<--- PHY-PARAM-REQUEST ----------- | |                                   |\n");

    }
/* SPR 19288 change start */
/* SPR 19288 change end */
    /* SPR 8194 Fix Start */
    LOG_MAC_MSG(MAC_PHY_PARAM_REQ, LOGDEBUG, MAC_L1_INF, getCurrentTick(),
            PHY_PARAM_REQUEST, MAC_MODULE_ID, PHY_MODULE_ID,
            DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"SEND_PHY_PARAM_REQ");
    /* SPR 8194 Fix End */

    freeMemPool(paramReq_p);
    paramReq_p = PNULL;	
    return resp;
}
#endif	
/*****************************************************************************
 * Function Name  : handlePhyParamReqResp
 * Inputs         : msgBuf - msg Buffer and
 *                  bytesRead - to store the number of bytes read,
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is used by MAC to handle the response of the request to PHYParam.
 *****************************************************************************/
UInt32 handlePhyParamReqResp(UInt8 *msgBuf,SInt32 bytesRead,
        InternalCellIndex internalCellIndex)
{
    /* SPR 1768 changes end */
    FAPI_l1ApiMsg_st *l1ApiMsg_p = PNULL;
    FAPI_paramResponse_st *paramResp_p = PNULL;
    UInt32 resp = MAC_FAILURE; 

    if ( bytesRead > (SInt32)sizeof(FAPI_l1ApiMsg_st) )
    {
        l1ApiMsg_p = ( FAPI_l1ApiMsg_st *) msgBuf;
        if ( PHY_PARAM_RESPONSE == l1ApiMsg_p->msgId )
        {
            paramResp_p = (FAPI_paramResponse_st * )
                &l1ApiMsg_p->msgBody[0];
            resp = fapiParsePhyParamResp(paramResp_p,internalCellIndex);
            /* SPR 8194 Fix Start */
            LOG_MAC_MSG(PHY_MAC_PARAM_RES,LOGDEBUG,MAC_L1_INF,
                    getCurrentTick(),PHY_PARAM_RESPONSE,
                    PHY_MODULE_ID,MAC_MODULE_ID,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"PHY_PARAM_RES_RECVD");
            /* SPR 8194 Fix End */
            fprintf (stderr, "| ----- PHY-PARAM-RESPONSE -------->> | |                                   |\n");

        }
        else
        {
            /* fapi changes start */
            LOG_MAC_MSG(PHY_MAC_INVALID_MSG,LOGWARNING,MAC_L1_INF,
                    getCurrentTick(),l1ApiMsg_p->msgId,
                    PHY_MODULE_ID,MAC_MODULE_ID,
                    __LINE__,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
           /* fapi changes end */
        }
    }
    else if ( (bytesRead > 0) && 
            (bytesRead <= (SInt32)sizeof(FAPI_l1ApiMsg_st)))
    {
        /* fapi changes start */
        LOG_MAC_MSG(PHY_MAC_BYTES_READ,LOGERROR,MAC_L1_INF,
                getCurrentTick(), 
                __LINE__,bytesRead,sizeof(FAPI_l1ApiMsg_st),0,0,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                __func__,"phyparamresp");    
        /* fapi changes end */ 
    }

    return resp;
}

/*****************************************************************************
 * Function Name  : handleStartPhyRes 
 * Inputs         : msgBuf, bytesRead,
 *                  internalCellIndex - cell id  
 * Outputs        : MAC_SUCCESS/MAC_FAILURE 
 * Returns        : None 
 * Description    : This function is used by MAC to start PHY Cell.
 *****************************************************************************/
/* CA Changes start */
UInt32 handleStartPhyRes(UInt8 *msgBuf,SInt32 bytesRead, 
        InternalCellIndex internalCellIndex)
/* CA Changes end */
{
    uint16_t          sfnSf = 0;
    uint16_t          sfn = 0;
    uint16_t          sf = 0;
    FAPI_l1ApiMsg_st *pL1StartRes_p = NULL;
    MacRetType ret = MAC_FAILURE; 
    /* Receive data from PHY layer. This data could be: 
     * 1. Eror indication for Start request i.e Start is failed
     * 2. Frame indication if start is successfull
     */

    if (bytesRead < (SInt32)sizeof(FAPI_l1ApiMsg_st))
    {
        /* fapi changes start */
        LOG_MAC_MSG(PHY_MAC_BYTES_READ,LOGERROR,MAC_L1_INF,
                getCurrentTick(), 
                __LINE__,bytesRead,sizeof(FAPI_l1ApiMsg_st),0,0,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                __func__,"StartPhyRes");    
        /* fapi changes end */ 

        return MAC_FAILURE;
    }
    else
    {
        /* Scenario-1; Start failed */
        if (PHY_ERROR_INDICATION == (uint8_t)*msgBuf)
        {
            /* SPR 609 changes start */
#ifdef LOG_PRINT_ENABLED
            FAPI_phyErrorIndication_st * ptr = 
                ((FAPI_phyErrorIndication_st *)msgBuf);
            LOG_MAC_MSG(PHY_START_FAILURE, LOGERROR, MAC_L1_INF,
                         getCurrentTick(),ptr->errorCode, 
                         DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                         DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                         DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    __func__,"PHYStartError");
#endif            

            /* SPR 609 changes end */
        }
        else if (PHY_UL_SUBFRAME_INDICATION == (uint8_t)*msgBuf)
        { /* Scenario-2; Start is successful */
            LTE_MAC_UT_LOG(LOG_INFO, CELL_CONF,"PHY Started Successfully. "
                    "Received Sub Frame indication");
#ifdef FLEXRAN
            printf("\n\n\n@@@@@@  PHY Started Successfully. Received First Sub Frame indication  @@@@@@@@@\n\n\n");
#endif

            pL1StartRes_p = (FAPI_l1ApiMsg_st *)msgBuf;
            /* SPR 609 changes start 
             *  This is not Endianness independent 
             *  */
            /* SPR 609 changes end */    

            /* SPR 1824 changes start */    
            /* SPR 1644 changes start */    
            /* [15:4] SFN, range 0 . 1023[3:0] SF, range 0 . 9 */
            FAPI_subFrameIndication_st * sfnsf_p;
            sfnsf_p = (FAPI_subFrameIndication_st *) &(pL1StartRes_p->msgBody[0]);
            sfnSf = MAC_PHY_CONVERT_16(sfnsf_p->sfnsf);
            /* SPR 1644 changes end */
            /* SPR 1824 changes end */

            sfn = GET_SYSFRAMENUM_FROM_SFNSF (sfnSf);
            sf = GET_SUBFRAME_FROM_SFNSF (sfnSf);
/* SPR 19288 change start */
/* SPR 19288 change end */
#ifdef OTA_DL_ADVANCE_2
            /* OTA_DL_ADVANCE changes start */
#ifdef TDD_CONFIG
            GET_REQUIRED_TICK(sfn,sf,1);
#endif
            /* OTA_DL_ADVANCE changes end */
#endif
            /* FAPI L2 AS MASTER START */
#ifdef FAPI_L2_MASTER
            /* Update the L2 tick */
            processTickReceivedOnL2(internalCellIndex);
            
            /*
            ** Process start PHY response. Also, this function
            ** updates MAC Tick, systemFrame and subFrame.
            **
            ** This needs to be updated here because postCellSetupInd()
            ** and other functions are being invoked which are using MAC
            ** tick and system number before executing executionflow legs.
            */
            initL2TickBeforeFirstELExecution(internalCellIndex);

            /* Validate the system frame and subframe received from L1 */
            validateSFIFrmL1(sfn, sf, internalCellIndex);

            /*
            ** As we have updated the MAC global tick with L2 tick here itself,
            ** This global is updated to indicate execution flow thread to start
            ** execution of other ELs.
            */
            startExecutionAfterPhyStart_g[internalCellIndex] = TRUE;
            /*SPR 13047 fix, adding return in the success case*/ 
            ret = MAC_SUCCESS;

#else

            /* SPR 1535 Changes Start*/
            /* CA Changes start */
            processFirstSubFrameInd(sfn, sf,internalCellIndex);  
            /* CA Changes end */
            /* SPR 1535 Changes Start*/
            ret = MAC_SUCCESS;
#endif
        }
        else 
        {
           LOG_MAC_WARNING( MAC_RRC_INF," Invalid Msg received\n");
/* SPR 19288 change start */
/* SPR 19288 change end */
        }
    }
    return ret;
}
//cell_state_changes
/*****************************************************************************
 * Function Name  : handleStopPhyRes
 * Inputs         : msgBuf and bytesRead
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : This function is used by MAC to handle PHY's STOP response. 
 *****************************************************************************/
UInt32 handleStopPhyRes(UInt8 *msgBuf,SInt32 bytesRead)
{
    SInt32  responseSize = L1_PARTIAL_API_MSG_SIZE;
    /*  FAPI_l1ApiMsg_st *pL1StopRes_p = NULL; TDD Warning Fix */
    MacRetType resp = MAC_FAILURE;

    if (bytesRead < responseSize)
    {
        /* fapi changes start */
        LOG_MAC_MSG(PHY_MAC_BYTES_READ,LOGERROR,MAC_L1_INF,
                getCurrentTick(), 
                __LINE__,bytesRead,sizeof(FAPI_l1ApiMsg_st),0,0,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                __func__,"StopPhyRes");    

        /* fapi changes end */ 
        return resp;
    }
    else
    {
        /* CA Changes start */
        /* Scenario-1; STOP failed */
        /* CID 111471 Fix Start */ 
        if (NULL != msgBuf)
        {
            /* CID 111471 Fix End */
            if (PHY_ERROR_INDICATION == (uint8_t)*msgBuf)
            {
#ifdef LOG_PRINT_ENABLED
                FAPI_phyErrorIndication_st * ptr = 
                    ((FAPI_phyErrorIndication_st *)msgBuf);
                LOG_MAC_MSG( MAC_RECV_ERR_IND_FRM_PHY, LOGWARNING, MAC_L1_INF,
                        getCurrentTick(), __LINE__, ptr->msgId, ptr->errorCode,
                        0,0, 0,0, __func__, "");
#endif
            }
            else if (PHY_STOP_INDICATION == (uint8_t)*msgBuf)
            { /* Scenario-2; STOP successful */
                LTE_MAC_UT_LOG(LOG_INFO, "Cell Stop", "PHY Stop Successfully."); 
                /*      pL1StopRes_p = (FAPI_l1ApiMsg_st *)msgBuf; TDD Warning Fix */
                /* SPR 19288 change start */
                /*SPR 20199 fix start*/
#ifdef WATCHDOG_THREAD
                watchdogflag_g = FALSE;
#endif
                /*SPR 20199 fix end*/


            }
            resp = MAC_SUCCESS;
            /* CA Changes end */
        }
    }
    return resp;
}

/****************************************************************************
 * Function Name  : preparePhyRRCHeader
 * Inputs         : msgBuf_p - Buffer to be filled,
 *                  msgId - Msg Type to be send to RRC and
 *                  destModuleId - Module ID of Destination
 *                  transactionId- transaction id received in API from RRC
 *                  msgLength- length of message to be sent to RRC
 *                  cellId- cell id received by MAC in API header 
 * Outputs        : None
 * Returns        : None
 * Description    : This API takes the msgId & buffer to be filled by msg
 *                  header as input.
 **************************************************************************/
void preparePhyRRCHeader(UInt8 *msgBuf_p, UInt16 msgId, UInt16 destModuleId,
    UInt16 transactionId ,UInt16 msgLength, RrcCellIndex cellId)
{
    UInt16 current = 0;
    LTE_SET_U16BIT(msgBuf_p, transactionId);
    current += U16BIT_LEN;
    LTE_SET_U16BIT(msgBuf_p + current, PHY_MODULE_ID);
    current += U16BIT_LEN;
    LTE_SET_U16BIT(msgBuf_p + current, destModuleId);
    current += U16BIT_LEN;
    LTE_SET_U16BIT(msgBuf_p + current, msgId);
    current += U16BIT_LEN;
    LTE_SET_U16BIT(msgBuf_p + current, msgLength);
    current += U16BIT_LEN;
    msgBuf_p[current] = cellId;
    return;
}
/* CA changes end */
/*****************************************************************************
 * Function Name  : sendPhyCellRespToRRC 
 * Inputs         : msgId - Message Id which to be send to RRC,
 *                  transId - Transaction id of request,
 *                  retVal - Success/Failure of RRC request and
 *                  cellIndex - Cell Id
 * Outputs        : None
 * Returns        : retvalue - SUCCESS/FAILURE 
 * Description    : This function is used by MAC to send config response to RRC.
 *****************************************************************************/
/* CA Changes start */
SInt32 sendPhyCellRespToRRC(UInt16 msgId, SInt32 transId, 
        MacRetType retVal, RrcCellIndex cellId)
{
    UInt8  msgBuf[RRC_API_HEADER_LEN + 4] = {0};  
    UInt8 *resp_p = msgBuf;
    UInt32 index = 0;
    SInt32 retvalue = 0;

    switch(msgId)
    {
        case PHY_CONFIG_CELL_CNF:
        {
            index = RRC_API_HEADER_LEN ;
            resp_p[index++] = cellId;
            resp_p[index++] = retVal;
            preparePhyRRCHeader( resp_p, PHY_CONFIG_CELL_CNF, RRC_MODULE_ID,
                    transId,index,cellId);
            LOG_MAC_MSG(RRC_PHY_CELL_RECONFIG_RES_ID,LOGWARNING,MAC_RRC_INF,
                    getCurrentTick(),
                    cellId,retVal,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"PHY_CELL_CONFIG_RES");
        }
        break;
        case PHY_RECONFIG_CELL_CNF:
        {
            index = RRC_API_HEADER_LEN;
            resp_p[index++] = cellId;
            resp_p[index++] = retVal;
            preparePhyRRCHeader( resp_p, PHY_RECONFIG_CELL_CNF, RRC_MODULE_ID,
                    transId,index,cellId);
            LOG_MAC_MSG(RRC_PHY_CELL_RECONFIG_RES_ID,LOGWARNING,MAC_RRC_INF,
                    getCurrentTick(),
                    cellId,retVal,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"PHY_CELL_RECONFIG_RES");
        } 
        break;
        case PHY_DELETE_CELL_CNF:
        {
            index = RRC_API_HEADER_LEN;
            resp_p[index++] = cellId;
            preparePhyRRCHeader( resp_p, PHY_DELETE_CELL_CNF, RRC_MODULE_ID,
                    transId,index,cellId);
            LOG_MAC_MSG(RRC_PHY_CELL_DELETE_RES_ID,LOGDEBUG,MAC_RRC_INF,
                    getCurrentTick(),
                    cellId,retVal,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"PHY_CELL_DELETE_RES");
        }
        break;
        /* Cell Start Stop Chg */
        case PHY_CELL_STOP_CNF:
        {
            index = RRC_API_HEADER_LEN;
            resp_p[index++] = cellId;
            preparePhyRRCHeader(resp_p, PHY_CELL_STOP_CNF, RRC_MODULE_ID,
                    transId,index,cellId);

            /* SPR 10393 Changes Start */
            LOG_MAC_MSG(RRC_PHY_CELL_STOP_RES_ID,LOGDEBUG,MAC_RRC_INF,
                    getCurrentTick(),
                    cellId,retVal,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"PHY_CELL_STOP_RES");
            /* SPR 10393 Changes End */
        }
        break;
        case PHY_CELL_START_CNF:
        {
            index = RRC_API_HEADER_LEN;
            resp_p[index++] = cellId;

            LTE_SET_U16BIT(resp_p + index, retVal);
            index += U16BIT_LEN;
            preparePhyRRCHeader(resp_p, PHY_CELL_START_CNF, RRC_MODULE_ID,
                    transId,index,cellId);

            /* SPR 10393 Changes Start */
            LOG_MAC_MSG(RRC_PHY_CELL_START_RES_ID_DEBUG,LOGDEBUG,MAC_RRC_INF,
                    getCurrentTick(),
                    RRC_MODULE_ID,msgId,cellId,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"PHY_CELL_START_RES");
            /* SPR 10393 Changes End */
        }
        break;
        
        default:
        LOG_MAC_WARNING( MAC_RRC_INF,"Invalid PHY to RRC response!!\n");
        return  retvalue;
    }

    retvalue = sendMacMsgToRRC(msgBuf,index, 0,
            macCommonGetInternalCellIndex(cellId));

    return retvalue;
}

/* CA changes end */
/*****************************************************************************
 * Function Name  : ueConfigHandler
 * Inputs         : transId - Transaction ID of msg,
 *                  msg_p - MSG from RRC,
 *                  internalCellIndex - cell id  
 * Outputs        : None 
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function handles PHY_CREATE_UE_ENTITY_REQ from RRC.
 *****************************************************************************/
MacRetType ueConfigHandler (
        UInt16 transId,
        UInt8 *msg_p, 
        /* +- SPR 17777 */
        InternalCellIndex  internalCellIndex
        )
{
    MacRetType retVal = MAC_SUCCESS;
    UInt16 ueIndex = 0;

    ueIndex = LTE_GET_U16BIT(msg_p);

    LOG_MAC_MSG(MAC_PHY_CREATE_UE_RES_ID,LOGDEBUG,MAC_RRC_INF,
            getCurrentTick(),
            ueIndex,MAC_SUCCESS,transId,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"RRC_PHY_CREATE_UE_ID");

    if(MAC_SEND_ERROR == sendUEConfigRespToRRC( transId,retVal, ueIndex,internalCellIndex))
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"UE PHY Configuration Failed for cell:%d\n", macCommonGetRRCCellIndex(internalCellIndex));

        ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }
    else
    { 

        LOG_MAC_MSG(MAC_PHY_CREATE_UE_RES_ID,LOGINFO,MAC_RRC_INF,
                getCurrentTick(),
                ueIndex,MAC_SUCCESS,transId,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"PHY_CREATE_UE_RespSenttoRRC");
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : sendUEConfigRespToRRC
 * Inputs         : transId - Transaction ID,
 *                  retVal - SUCCESS/FAILURE,
 *                  ueIndex - Index of UE,
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : retvalue - SUCCESS/FAILURE
 * Description    : This function prepares UE config RSP for RRC.
 ******************************************************************************/
/* CA changes start */
SInt32 sendUEConfigRespToRRC(UInt16 transId, MacRetType retVal, UInt16 ueIndex ,
        InternalCellIndex internalCellIndex)
{
    /* SPR#706 Changes Start */
    UInt8  msgBuf[RRC_API_HEADER_LEN + 4] = {0};
    /* SPR#706 Changes End */
    UInt8 *resp_p = &msgBuf[0];
    UInt32 index = RRC_API_HEADER_LEN;
    SInt32 retvalue = 0;   


    LTE_SET_U16BIT( resp_p + index, ueIndex); // fill UE index
    index += U16BIT_LEN;
    resp_p[index++] = retVal; // fill response
    {
    
    preparePhyRRCHeader( resp_p, PHY_CREATE_UE_ENTITY_CNF, RRC_MODULE_ID,
        transId, index ,macCommonGetRRCCellIndex(internalCellIndex));
    }

    retvalue = sendMacMsgToRRC(msgBuf,index,0,
    internalCellIndex);

    return retvalue;
}
/* CA changes end */


/*****************************************************************************
 * Function Name  : processControlInfoFromPhysical 
 * Inputs         : msgBuf - msg Buffer, 
 *                  bytesRead - to store the number of bytes read,
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function control info msg from phy.
 ******************************************************************************/
/* CA Changes start */
void processControlInfoFromPhysical (UInt8 * msgBuf, Int bytesRead,
        InternalCellIndex internalCellIndex)
{
    /* SPR 1768 changes start */
    /* + coverity 54492 */
    MacRetType   retval = MAC_SUCCESS;
    /* - coverity 54492 */
    UInt16  iterator;
    /* SPR 1768 changes end */
    FAPI_l1ApiMsg_st *L1ConfigRes_p = PNULL;
    FAPI_phyCellConfigResp_st *ConfigRes_p = PNULL;
#ifdef LOG_PRINT_ENABLED
    FAPI_cellConfig_st   *listOfInvalidTlvs_p = PNULL;
    FAPI_cellConfig_st   *listOfMissingTlvs_p = PNULL;
#endif
    if (bytesRead < (SInt32)sizeof(FAPI_l1ApiMsg_st))
    {
        LTE_MAC_UT_LOG(LOG_WARNING,UE_CONF_MGR,"No. of bytes received are less"
                "than FAPI_l1ApiMsg_st");
        return;
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,"Proper msg recd\n");
        L1ConfigRes_p = (FAPI_l1ApiMsg_st *)msgBuf;
        if (PHY_UE_CONFIG_RESPONSE == L1ConfigRes_p->msgId)
        {
            LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,
                    "PHY_UE_CONFIG_RESPONSE recd\n");
        }
        else if ( (PHY_CELL_CONFIG_RESPONSE == L1ConfigRes_p->msgId) && 
                (CELL_PHY_RUNNING_CONFIG_RECVD == cellSpecificParams_g.\
                 cellConfigAndInitParams_p[internalCellIndex]->cellState)
                )
        {
            /*coverity 10168*/
            ConfigRes_p = getPhyCellConfigRespPtr(L1ConfigRes_p);
            /*coverity 10168*/

            /* +SPR_11258 Changes */
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                cellState = CELL_PHY_RUNNING;
            if(FAPI_MSG_OK == ConfigRes_p->errorCode)  
            {
                reconfigPhyCell_g[internalCellIndex].reconfigUpdate = FAPI_TRUE;
                retval = MAC_SUCCESS;
            }
            /* -SPR_11258 Changes */
            else
            {
                retval = MAC_FAILURE;
                reconfigPhyCell_g[internalCellIndex].reconfigUpdate = FAPI_FALSE;

                LOG_MAC_WARNING( MAC_RRC_INF,"PHY Cell Configuration Failed. "
                        "Error Code: [%d]", ConfigRes_p->errorCode);

                /* Log the Missing TLVs */
                if(ConfigRes_p->numOfMissingTLV)
                {
                    LOG_MAC_WARNING( MAC_RRC_INF,"List of Missing TLVs. Count = %d",
                            ConfigRes_p->numOfMissingTLV);
                    for(iterator = 0; iterator < 
                            ConfigRes_p->numOfMissingTLV;
                            iterator++)
                    {
                        /*cov  65059_65058 +*/
#ifdef LOG_PRINT_ENABLED
                        listOfMissingTlvs_p =
                            &ConfigRes_p->listOfMissingTlv[iterator];
#endif
                        LOG_MAC_MSG(PHY_MAC_TLV_TAG,LOGWARNING,MAC_L1_INF,
                                getCurrentTick(),__LINE__, 
                                listOfMissingTlvs_p->tag, 0,
                                0,0,
                                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                                __func__,"Missing");
                    }
                }
                /* Log the Unsupported TLVs */
                if(ConfigRes_p->numOfInvalidOrUnsupportedTLV)
                {
                    LOG_MAC_WARNING( MAC_RRC_INF,"List of Invalid TLVs. Count = %d"
                            , ConfigRes_p->numOfInvalidOrUnsupportedTLV);
                    for(iterator = 0; iterator < ConfigRes_p->
                            numOfInvalidOrUnsupportedTLV; iterator++)
                    {
#ifdef LOG_PRINT_ENABLED
                        listOfInvalidTlvs_p = &ConfigRes_p->listOfTLV[iterator];
#endif
                        LOG_MAC_MSG( PHY_MAC_TLV_TAG, LOGWARNING, MAC_L1_INF,
                                getCurrentTick(), __LINE__, 
                                listOfInvalidTlvs_p->tag, 0, 0,0,
                                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                                __func__,"Invalid");
                        /*cov 65059_65058 -*/ 
                    }
                }
            }
            /* Send Response to L3 */
            if( MAC_SEND_ERROR == sendPhyCellRespToRRC(PHY_RECONFIG_CELL_CNF,
            /* + coverity 54492 */
                        (SInt32) reconfigPhyCell_g[internalCellIndex].transactionId,
                        retval,macCommonGetRRCCellIndex(internalCellIndex) ))
            /* - coverity 54492 */
            {
                ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID,
                        MAJOR_ALARM);
            }
            else
            {
                LOG_MAC_MSG(RRC_PHY_CELL_RECONFIG_REQ_ID,LOGWARNING,
                        MAC_RRC_INF,
                        getCurrentTick(),
                        RRC_MODULE_ID, PHY_RECONFIG_CELL_CNF, DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"SentRespToRRC");
            }
        } 
        else if (PHY_CELL_CONFIG_RESPONSE != L1ConfigRes_p->msgId) 
        {
            retval = MAC_FAILURE; 
            LOG_MAC_WARNING( MAC_RRC_INF,"Invalid msg ID received msgId = %d",L1ConfigRes_p->msgId);
            /* Send Response to L3 */
            if( MAC_SEND_ERROR == sendPhyCellRespToRRC(PHY_RECONFIG_CELL_CNF,
                        /* + coverity 54492 */
                        (SInt32) reconfigPhyCell_g[internalCellIndex].transactionId,
                        retval,macCommonGetRRCCellIndex(internalCellIndex) ))
                /* - coverity 54492 */
            {
                ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID,
                        MAJOR_ALARM);
            }
            else
            {
                LOG_MAC_MSG(RRC_PHY_CELL_RECONFIG_REQ_ID,LOGWARNING,
                        MAC_RRC_INF,
                        getCurrentTick(),
                        RRC_MODULE_ID, PHY_RECONFIG_CELL_CNF, DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"SentRespToRRC");
            }
        }
    }
}

/* CA Changes end */

/*****************************************************************************
 * Function Name  : initUlConfigReqQueue 
 * Inputs         : internalCellIndex 
 * Outputs        : None
 * Returns        : None
 * Description    : This function initializes UL config msg queue.
 ******************************************************************************/
void initUlConfigReqQueue (InternalCellIndex internalCellIndex)
{
    UInt8 i = 0;
    for (i=0; i<MAX_SUB_FRAME; i++)
    {
        ulConfigReqQueue_g[internalCellIndex][i].ulConfigReq_p = PNULL;
        ulConfigReqQueue_g[internalCellIndex][i].len = 0;
    }
}

/*****************************************************************************
 * Function Name  : getNodeFromUlConfigReqQueue
 * Inputs         : sfn - Sub Frame Number,
 *		            ulConfig_pp - Out buffer to store ulconfig msg.
 *                  len_p - length of ulConfig_p.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles gets UL config msg in q at sfn.
 ******************************************************************************/
void getNodeFromUlConfigReqQueue (
        UInt8 sfn, 
        FAPI_l1ApiMsg_st** ulConfig_pp,
        UInt16 * len_p,
        InternalCellIndex internalCellIndex
        )
{
    *ulConfig_pp = ulConfigReqQueue_g[internalCellIndex][sfn].ulConfigReq_p;
    *len_p = ulConfigReqQueue_g[internalCellIndex][sfn].len;

    ulConfigReqQueue_g[internalCellIndex][sfn].ulConfigReq_p = PNULL;
    ulConfigReqQueue_g[internalCellIndex][sfn].len = 0;
}

/*****************************************************************************
 * Function Name  : insertNodeInUlConfigReqQueue
 * Inputs         : sfn - Sub Frame Number,
 *		            ulConfig_p - ulconfig msg to be inserted in Q and
 *                  len - length of ulConfig_p,
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles inserts UL config msg in q at sfn.
 ******************************************************************************/
void insertNodeInUlConfigReqQueue (
        UInt8 sfn, 
        FAPI_l1ApiMsg_st * ulConfig_p, 
        UInt16 len,
        InternalCellIndex internalCellIndex
        )
{
    /* + SPR_11194 */
    if(PNULL != ulConfigReqQueue_g[internalCellIndex][sfn].ulConfigReq_p)
    {
        ulConfigReqQueue_g[internalCellIndex][sfn].len = 0;
    }
    /* - SPR_11194 */
    ulConfigReqQueue_g[internalCellIndex][sfn].ulConfigReq_p = ulConfig_p;
    ulConfigReqQueue_g[internalCellIndex][sfn].len = len;
}

/* SPR 4439 Chg */
/*****************************************************************************
 * Function Name  : cleanupUlSchUciRachContainer
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles cleanup of UCI containers created for
 *                  advance UL config creation.
 *******************************************************************************/
/*CA Changes start  */
void cleanupUlSchUciRachContainer(InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{
    UInt8 index = 0;
#ifdef FDD_CONFIG    
    for(index = 0; index < MAX_PDCCH_CONTAINER; index++)
    /* TDD Config 0 Changes Start */
#else
    for(index = 0; index < MAX_ULSCH_UCI_CONTAINER; index++)
#endif
    /* TDD Config 0 Changes End */
    {
        /*CA Changes start  */
        (ulSchUciRachContainer_gp[internalCellIndex] + index)->numberOfInfoPresent = 0;
        /*CA Changes end  */ 
    }
    for(index = 0; index < MAX_SUB_FRAME; index++)
    {
        if(ulConfigReqQueue_g[internalCellIndex][index].ulConfigReq_p)
        {
            /* SPR 20445 Fix Start */
/* SPR 11091 Changes Start */
            /* SPR 20445 Fix End */
/* SPR 11091 Changes End */
            ulConfigReqQueue_g[internalCellIndex][index].ulConfigReq_p = PNULL;
            ulConfigReqQueue_g[internalCellIndex][index].len = 0;
        }
    }
    return;
}
/* SPR 4439 Chg */

/* MAC_TTI_LOG Start */
#ifdef SHM_PRINT_ENABLED
/*****************************************************************************
 * Function Name  : fillttiULConfigLoggingInfo 
 * Inputs         : log_ptr - pointer to ttiULLoggingInfo,
 *                  ulSchPdu_p - pointer to structure FAPI_ulSCHPduInfo_st,
 *                  mcsIndex - mcs index, 
 *                  ueIndex - UE Index,
 *                  pduType - type of pdu,
 *                  internalCellIndex - cell id  
 * Outputs        : fills ttiULLoggingInfo structure
 * Returns        : None
 * Description    : This function will fill ttiDciLoggingInfo from DciCCEInfo.
 *****************************************************************************/
void fillttiULConfigLoggingInfo (ttiULLoggingInfo *log_ptr,FAPI_ulSCHPduInfo_st *ulSchPdu_p ,
    UInt8 mcsIndex,UInt16 ueIndex,UInt8 pduType,
        InternalCellIndex  internalCellIndex)
{
	log_ptr->logid = MAC_TTI_UL_CONF_LOG_INFO; 
    log_ptr->systemFrame =  getSysFrameNumer(internalCellIndex); 
    log_ptr->subframe= getSubFrameNumber(internalCellIndex);

    log_ptr->rnti = ulSchPdu_p->rnti;
    log_ptr->RBStart = ulSchPdu_p->rbStart;
    log_ptr->numOfRBs = ulSchPdu_p->numOfRB;
    log_ptr->rv = ulSchPdu_p->redundancyVersion;
    log_ptr->txIndicator = ulSchPdu_p->newDataIndication;
    log_ptr->tbSize = ulSchPdu_p->size;
    log_ptr->modType = ulSchPdu_p->modulationType;
    log_ptr->pduType = pduType;
    log_ptr->mcsIndex = mcsIndex;
    log_ptr->UEID = ueIndex;

}
#endif
/* MAC_TTI_LOG End */
#ifdef FAPI_4_0_COMPLIANCE
/*****************************************************************************
 * Function Name  : fillULSCHRelease10Param 
 * Inputs         : release10Param_p 
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Ulsch pdu rel10 param.
 ******************************************************************************/
void fillULSCHRelease10Param(FAPI_ulschRelease10Param_st *release10Param_p
/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
		,ULHarqInfo * ulHarq_p
#endif
		)
/* SPR 21958 PUSCH RAT1 Support End */
{
    /*FAPI2_1 changes start*/
	/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
	if(TRUE == ulHarq_p->RAT1Allocation)
	{
		release10Param_p->resourceAllocationType = FAPI_RES_ALLOC_TYPE_1;
		release10Param_p->resourceBlockCoding = getCombinatorialIndex(
				ulHarq_p->startRbgIndexforSet1,
				ulHarq_p->endRbgIndexforSet1,
				ulHarq_p->startRbgIndexforSet2,
				ulHarq_p->endRbgIndexforSet2);	

	}
	else
	{
#endif
		release10Param_p->resourceAllocationType = FAPI_RES_ALLOC_TYPE_0;
		release10Param_p->resourceBlockCoding = MAC_PHY_CONVERT_32(DEFAULT_ULSCH_RBCODING);
#ifdef FDD_CONFIG
	} 
#endif
	/* SPR 21958 PUSCH RAT1 Support End */
	release10Param_p->transportBlocks = DEFAULT_ULSCH_TRANSPORTBLOCK;
	release10Param_p->transmissionScheme = DEFAULT_ULSCH_TRANS_SCHEME;
	release10Param_p->numOfLayers = DEFAULT_ULSCH_NUM_LAYERS;
	release10Param_p->codebookIndex = DEFAULT_ULSCH_CODEBOOK_INDEX;
	release10Param_p->disableSequenceHopping = DEFAULT_DISABLE_SEQUENCE_HOPPING;
    /*FAPI2_1 changes end*/

    return;
}
/*****************************************************************************
 * Function Name  : prepareUlschPduR10
 * Inputs         : ulSchPdu_p
 * 		            ulSchUciInfo_p,
 *                  srsPresent,
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Ulsch pdu.This function will be used in 
 *                    case PHY Supports Release 10
 ******************************************************************************/
MacRetType prepareUlschPduR10 (
        FAPI_ulSCHPduInfo_st *ulSchPdu_p,
        ULSchUciInfo * ulSchUciInfo_p
        /*SPR 1547 Start*/
        ,UInt8 srsPresent,
        /*SPR 1547 End*/
        InternalCellIndex  internalCellIndex)

{
    ULHarqInfo * ulHarq_p = ulSchUciInfo_p->ulHarqInfo_p;


    /* SPR 3600 Start */
    RNTIInfo *rntiInfo_p = PNULL;
    UInt16 ueIndex = 0;
    /* SPR 3600 End */

    /* + SPR_15091 */
    rntiInfo_p = getUeIdxFromRNTIMap( MAC_PHY_CONVERT_16(ulSchUciInfo_p->rnti),internalCellIndex);

    /* + SPR 22770, ue Context null check removed*/
    if((PNULL == rntiInfo_p) ||  (FREERNTI == rntiInfo_p->rntiFlag)
    /* SES-530 Fix Start */
            || ((CRNTI == rntiInfo_p->rntiFlag) && (PNULL == ulUECtxInfoArr_g[rntiInfo_p->index].ulUEContext_p))
    /* SES-530 Fix End */
      )
    {
        return MAC_FAILURE;
    }  
    /* - SPR_15091 */

    if(PNULL != ulHarq_p)
    {
        /*SPR 21001 Changes start */
        ulSchPdu_p->handle                 = MAC_PHY_CONVERT_32(generateHandleVal(internalCellIndex));
        LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,
                "handle in UL sch %d\n", ulSchPdu_p->handle);
        /*SPR 21001 Changes end */

        /* SPR 926 Fix Start */
        /* Temp check for preventing crash will be removed after detailed analysis*/
        /* + CQI_5.0 */  
#ifdef FDD_CONFIG
        /* SPR 4815 changes start */   
        if ( (ulHarq_p->riLength == 0 || ulHarq_p->mcsIndex == INVALID_MCS_INDEX)
                || ((MCS_INDEX_CQI_WITHOUT_DATA == ulHarq_p->mcsIndex)&& 
                    (ulHarq_p->riLength <= 4)) ||
                /* CA_phase2_csi_code Start- FR16(CA Arch DOC) */
                ((MCS_INDEX_CQI_WITHOUT_DATA == ulHarq_p->mcsIndex)&&
                 (ulHarq_p->riLength <= 20))
                /* CA_phase2_csi_code End */
           )
        {
            /* SPR 1868 */        
            LTE_MAC_UT_LOG(LOG_WARNING, UE_CONF_MGR,
                    "[%s]determineUlTBSize --> Number Of Resource Blocks[%d]"
                    " or mcsIndex[%d] Invalid\n",__func__,
                    ulHarq_p->riLength, ulHarq_p->mcsIndex ); 
            /* SPR 1868 */        
            ulSchPdu_p->size = 0;
        }
        else
        {
            /* SPR 1868 */        
            if(ulHarq_p->mcsIndex < MAX_MCS_INDEX)
            {
                ulSchPdu_p->size = MAC_PHY_CONVERT_16(determineUlTBSize(
                            (UInt32) ulHarq_p->mcsIndex,
                            (UInt32) ulHarq_p->riLength
                            ));
            }
            /* SPR 1868 */        
        }
        /* SPR 4815 changes end */   
        /* - CQI_5.0 */  
#elif TDD_CONFIG
        /* Temp check for preventing crash will be removed after detailed analysis*/
        /* + Coverity 10579 */
        if( (ulHarq_p->riLength == 0 || ulHarq_p->mcsIndex >= MAX_MCS_INDEX )
                /* - Coverity 10579 */
                || ((MCS_INDEX_CQI_WITHOUT_DATA == ulHarq_p->mcsIndex)&&
                    (ulHarq_p->riLength <= 4)) )
        {
            /* SPR 1868 */        
            LTE_MAC_UT_LOG(LOG_WARNING, UE_CONF_MGR,
                    "[%s]determineUlTBSize --> Number Of Resource Blocks[%d]"
                    " or mcsIndex[%d] Invalid\n",__func__,
                    ulHarq_p->riLength, ulHarq_p->mcsIndex ); 
            /* SPR 1868 */        
            ulSchPdu_p->size = 0;
        }
        else
        {
            /* SPR 1868 */        
            ulSchPdu_p->size =  MAC_PHY_CONVERT_16(determineUlTBSize(
                        (UInt32) ulHarq_p->mcsIndex,
                        (UInt32) ulHarq_p->riLength
                        ));
            /* SPR 1868 */        
        }
#endif
        /* SPR 926 Fix End */
        LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,
                "Size in UL sch %d\n", ulSchPdu_p->size);

        ulSchPdu_p->rnti                   =  MAC_PHY_CONVERT_16(ulSchUciInfo_p->rnti);
        LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,
                "RNTI in UL sch %d\n", ulSchPdu_p->rnti);
        /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
        if(TRUE == ulHarq_p->RAT1Allocation)
        {
            ulSchPdu_p->rbStart  = INVALID_VALUE_FOR_UPLINK_RAT1;
            ulSchPdu_p->numOfRB  = INVALID_VALUE_FOR_UPLINK_RAT1;
        }
        else
        {
#endif
        ulSchPdu_p->rbStart  = ulHarq_p->riStart;
        ulSchPdu_p->numOfRB  = ulHarq_p->riLength;
#ifdef FDD_CONFIG
        }
#endif
        /* SPR 21958 PUSCH RAT1 Support End */
        LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,
                "NUM of RB in UL sch %d\n", 
                ulSchPdu_p->numOfRB
                );
        /*Mapping from MCS to modulation type*/

        ulSchPdu_p->cyclicShift2forDMRS    = ulHarq_p->nDmrs;
        ulSchPdu_p->freqHoppingenabledFlag = ulHarq_p->freqEnabledFlag;
        ulSchPdu_p->freqHoppingBits        = ulHarq_p->freqHoppingBits;
        /* SPR 1219 Start */
        ulSchPdu_p->newDataIndication      = ulHarq_p->txIndicator;
        /* SPR 1219 End */



        LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,
                "New Data ind %d\n", 
                ulSchPdu_p->newDataIndication);

        /* Taking default as 0 */
        /* SPR 1660 + SPR 1668 Fix starts */
        ulSchPdu_p->redundancyVersion      = ulHarq_p->currentIrv;
        /* SPR 1660 + SPR 1668 Fix ends */
        ulSchPdu_p->harqProcessNumber      = ulSchUciInfo_p->ulHarqProcessId;
        ulSchPdu_p->ulTxMode               = ulHarq_p->ulTxMode;
        ulSchPdu_p->currentTxNB            = ulHarq_p->currentRetransCount;
        /*SPR 1547 Start*/
        ulSchPdu_p->nSRS                   = srsPresent;
        /*SPR 1547 End*/
        /* SPR 3600 Start */

        if (rntiInfo_p && (TCRNTI != rntiInfo_p->rntiFlag))
        {
            ueIndex = rntiInfo_p->index;

            GET_MOD_TYPE_FROM_MCS_INDEX_UL(ulHarq_p->mcsIndex,
                    ulSchPdu_p->modulationType,
                    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                    cellParams_p->macEnable64QAM,
                    ulUECtxInfoArr_g[ueIndex].ulUEContext_p->ueCategory);
        }
        else
        {
            /* + Coverity 24392 */
            GET_MOD_TYPE_FROM_MCS_INDEX_UL_UE_CATEGORY_1(ulHarq_p->mcsIndex,
                    ulSchPdu_p->modulationType,
                    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                    cellParams_p->macEnable64QAM);
            /* - Coverity 24392 */
        }
        /* SPR 3600 End */
        /* SPR 1600 changes start */
        LOG_UT(MAC_ULCONFIG_ULSCH_ID,LOGDEBUG,MAC_L1_INF,
                getCurrentTick(),
                ulSchPdu_p->rnti,
                ulSchPdu_p->harqProcessNumber,
                ulSchPdu_p->rbStart,
                ulSchPdu_p->numOfRB,
                (getCurrentTick() + PHY_DL_DELAY) ,
                ((getCurrentTick() + PHY_DL_DELAY) % MAX_PDCCH_CONTAINER),
                DEFAULT_FLOAT_VALUE,
                __func__,"ULCONFIG_ULSCH");

        /* SPR 21958 PUSCH RAT1 Support Start */
        fillULSCHRelease10Param(&ulSchPdu_p->release10Param
#ifdef FDD_CONFIG
			,ulHarq_p
#endif
        /* SPR 21958 PUSCH RAT1 Support End */
			);


        /* MAC_TTI_LOG Start */
#ifdef SHM_PRINT_ENABLED
        ttiULLoggingInfo *log_ptr = LOG_TTI_MSG(MAC_TTI_UL_CONF_LOG_INFO);
        if(log_ptr) 
        {
            fillttiULConfigLoggingInfo (log_ptr ,ulSchPdu_p ,ulHarq_p->mcsIndex,ueIndex,ulSchUciInfo_p->pduType,internalCellIndex);
        }
#endif
        /* MAC_TTI_LOG End */


        /* SPR 1600 changes end */

        /* + SPR_5470 */
        /* SPR 4525 Changes start */
        /* Here,resetting the following parameters of ulHarq_p,
         * in case of Aperiodic cqi report without data*/
        /*if(MCS_INDEX_CQI_WITHOUT_DATA == ulHarq_p->mcsIndex)
          {
          ulHarq_p->riStart = 0;
          ulHarq_p->riLength = 0;
          ulHarq_p->mcsIndex = 0;
          } */
        /* SPR 4525 Changes end */
        /* - SPR_5470 */

        return MAC_SUCCESS;
    }
    else 
    {
        LTE_MAC_UT_LOG(LOG_WARNING, UE_CONF_MGR,
                "Null Harq entry in ulsch pdu\n");
        return MAC_FAILURE;
    }
}
#endif	


/*****************************************************************************
 * Function Name  : prepareUlschPdu
 * Inputs         : ulschHarq_p 
 *        		    ulSchUciInfo_p,
 *                  srsPresent,
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Ulsch pdu.
 ******************************************************************************/
MacRetType prepareUlschPdu (
        FAPI_ulSCHPduInfo_st *ulSchPdu_p,
        ULSchUciInfo * ulSchUciInfo_p
        /*SPR 1547 Start*/
        ,UInt8 srsPresent
        /*SPR 1547 End*/,
        InternalCellIndex internalCellIndex
        )
{
    ULHarqInfo * ulHarq_p = ulSchUciInfo_p->ulHarqInfo_p;
/* SPR 19288 change start */
/* SPR 19288 change end */

    /* SPR 3600 Start */
    RNTIInfo *rntiInfo_p = PNULL;
    UInt16 ueIndex = 0;
    /* SPR 3600 End */

    if((PNULL != ulHarq_p)/* && (ulHarq_p->riLength != 0)*/)
    {
        /*SPR 8648 Changes Start*/
        rntiInfo_p = getUeIdxFromRNTIMap(ulSchUciInfo_p->rnti,internalCellIndex);

        if((PNULL == rntiInfo_p)  || (FREERNTI == rntiInfo_p->rntiFlag))
        {
            fprintf(stderr,"rntiInfo is null , rnti %d\n",ulSchUciInfo_p->rnti);
            return MAC_FAILURE;
        }
        /*SPR 8648 Changes End*/
/*SPR 21001 Changes start */
        ulSchPdu_p->handle                 = MAC_PHY_CONVERT_32(generateHandleVal(internalCellIndex));
/*SPR 21001 Changes end */
/*SPR 21001 Changes start */
        LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,
                "handle in UL sch %d\n", ulSchPdu_p->handle);
/*SPR 21001 Changes end */

        /* SPR 926 Fix Start */
        /* Temp check for preventing crash will be removed after detailed analysis*/
        /* + CQI_5.0 */  
#ifdef FDD_CONFIG
        /* SPR 4815 changes start */   
        if ( (ulHarq_p->riLength == 0 || ulHarq_p->mcsIndex == INVALID_MCS_INDEX)
                || ((MCS_INDEX_CQI_WITHOUT_DATA == ulHarq_p->mcsIndex)&& 
                     (ulHarq_p->riLength <= 4)) )
        {
            /* SPR 1868 */        
            LTE_MAC_UT_LOG(LOG_WARNING, UE_CONF_MGR,
                    "[%s]determineUlTBSize --> Number Of Resource Blocks[%d]"
                    " or mcsIndex[%d] Invalid\n",__func__,
                    ulHarq_p->riLength, ulHarq_p->mcsIndex ); 
            /* SPR 1868 */        
            ulSchPdu_p->size = 0;
        }
        else
        {
            /* SPR 1868 */        
	    if(ulHarq_p->mcsIndex < MAX_MCS_INDEX)
	    {
                ulSchPdu_p->size = MAC_PHY_CONVERT_16(determineUlTBSize(
                    (UInt32) ulHarq_p->mcsIndex,
                    (UInt32) ulHarq_p->riLength
                    ));
	    }
            /* SPR 1868 */        
        }
        /* SPR 4815 changes end */   
        /* - CQI_5.0 */  
#elif TDD_CONFIG
        /* Temp check for preventing crash will be removed after detailed analysis*/
        /* + Coverity 10579 */
		if( (ulHarq_p->riLength == 0 || ulHarq_p->mcsIndex >= MAX_MCS_INDEX )
        /* - Coverity 10579 */
				|| ((MCS_INDEX_CQI_WITHOUT_DATA == ulHarq_p->mcsIndex)&&
					(ulHarq_p->riLength <= 4)) )
        {
            /* SPR 1868 */        
            LTE_MAC_UT_LOG(LOG_WARNING, UE_CONF_MGR,
                    "[%s]determineUlTBSize --> Number Of Resource Blocks[%d]"
                    " or mcsIndex[%d] Invalid\n",__func__,
                    ulHarq_p->riLength, ulHarq_p->mcsIndex ); 
            /* SPR 1868 */        
            ulSchPdu_p->size = 0;
        }
        else
        {
            /* SPR 1868 */   
            ulSchPdu_p->size =  MAC_PHY_CONVERT_16(determineUlTBSize(
                    (UInt32) ulHarq_p->mcsIndex,
                    (UInt32) ulHarq_p->riLength
                    ));
            /* SPR 1868 */        
        }
#endif
        /* SPR 926 Fix End */
        LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,
                "Size in UL sch %d\n", ulSchPdu_p->size);
        ulSchPdu_p->rnti                   =  MAC_PHY_CONVERT_16(ulSchUciInfo_p->rnti);
        LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,
                "RNTI in UL sch %d\n", ulSchPdu_p->rnti);
        ulSchPdu_p->rbStart  = ulHarq_p->riStart;
        ulSchPdu_p->numOfRB  = ulHarq_p->riLength;
        LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,
                "NUM of RB in UL sch %d\n", 
                ulSchPdu_p->numOfRB
                );
        /*Mapping from MCS to modulation type*/
        /*SPR 3600 End*/
        ulSchPdu_p->cyclicShift2forDMRS    = ulHarq_p->nDmrs;
        ulSchPdu_p->freqHoppingenabledFlag = ulHarq_p->freqEnabledFlag;
        ulSchPdu_p->freqHoppingBits        = ulHarq_p->freqHoppingBits;
        /* SPR 1219 Start */
        ulSchPdu_p->newDataIndication      = ulHarq_p->txIndicator;
        /* SPR 1219 End */


        LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,
                "New Data ind %d\n", 
                ulSchPdu_p->newDataIndication);
        /* Taking default as 0 */
        /* SPR 1660 + SPR 1668 Fix starts */
        ulSchPdu_p->redundancyVersion      = ulHarq_p->currentIrv;
        /* SPR 1660 + SPR 1668 Fix ends */
        ulSchPdu_p->harqProcessNumber      = ulSchUciInfo_p->ulHarqProcessId;
        ulSchPdu_p->ulTxMode               = ulHarq_p->ulTxMode;
        ulSchPdu_p->currentTxNB            = ulHarq_p->currentRetransCount;

        if((ulHarq_p->newDataIndicator==1)&&(ulSchPdu_p->redundancyVersion!=0))
        {
            fprintf(stderr,"error ndi [%d] rv[%d]\n", ulHarq_p->newDataIndicator,
				ulSchPdu_p->redundancyVersion);
			ulSchPdu_p->redundancyVersion=0;
			ulHarq_p->currentIrv=0;
        }
		
        /*SPR 1547 Start*/
        /*SPR 7086 Fix Start*/
        if(TRUE == srsPresent)
        {
            ulSchPdu_p->nSRS               = ulSchUciInfo_p->nSRS;
        }
        else
        {
            ulSchPdu_p->nSRS                = SRS_ULSCH_NO_OVERLAP;
        }
        /*SPR 7086 Fix End*/
        /*SPR 1547 End*/
/* SPR 19288 change start */
/* SPR 19288 change end */
        /* SPR 3600 Start */
        rntiInfo_p = getUeIdxFromRNTIMap(ulSchPdu_p->rnti,internalCellIndex);
        
        if (rntiInfo_p && (TCRNTI != rntiInfo_p->rntiFlag))
        {
            ueIndex = rntiInfo_p->index;

            GET_MOD_TYPE_FROM_MCS_INDEX_UL(ulHarq_p->mcsIndex,
                    ulSchPdu_p->modulationType,
                    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                    cellParams_p->macEnable64QAM,
                    ulUECtxInfoArr_g[ueIndex].ulUEContext_p->ueCategory);
        }
        else
        {
            /* + Coverity 24392 */
            GET_MOD_TYPE_FROM_MCS_INDEX_UL_UE_CATEGORY_1(ulHarq_p->mcsIndex,
                    ulSchPdu_p->modulationType,
                    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                    cellParams_p->macEnable64QAM);
            /* - Coverity 24392 */
        }
        /* SPR 3600 End */
		if(ulSchPdu_p->numOfRB>(100-ulSchPdu_p->rbStart))
		{
		    fprintf(stderr,"error numOfRB [%d] rbStart[%d]\n", ulSchPdu_p->numOfRB,ulSchPdu_p->rbStart);
			ulSchPdu_p->numOfRB=100-ulSchPdu_p->rbStart;
		}
        /* SPR 1600 changes start */
        LOG_UT(MAC_ULCONFIG_ULSCH_ID,LOGDEBUG,MAC_L1_INF,
                getCurrentTick(),
                ulSchPdu_p->rnti,
                ulSchPdu_p->harqProcessNumber,
                ulSchPdu_p->rbStart,
                ulSchPdu_p->numOfRB,
                (getCurrentTick() + PHY_DL_DELAY) ,
                ((getCurrentTick() + PHY_DL_DELAY) % MAX_PDCCH_CONTAINER),
                DEFAULT_FLOAT_VALUE,
                __func__,"ULCONFIG_ULSCH");
#ifdef FAPI_4_0_COMPLIANCE
        fillULSCHRelease10Param(&ulSchPdu_p->release10Param
        /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
			,ulHarq_p
#endif
        /* SPR 21958 PUSCH RAT1 Support End */
			);
#endif	




/* MAC_TTI_LOG Start */
#ifdef SHM_PRINT_ENABLED
	ttiULLoggingInfo *log_ptr = LOG_TTI_MSG(MAC_TTI_UL_CONF_LOG_INFO);
	if(log_ptr) 
	{
            fillttiULConfigLoggingInfo (log_ptr ,ulSchPdu_p ,ulHarq_p->mcsIndex,ueIndex,
                    ulSchUciInfo_p->pduType,internalCellIndex);
	}
#endif
/* MAC_TTI_LOG End */


        /* SPR 1600 changes end */

      /* + SPR_5470 */
        /* SPR 4525 Changes start */
        /* Here,resetting the following parameters of ulHarq_p,
         * in case of Aperiodic cqi report without data*/
      /*if(MCS_INDEX_CQI_WITHOUT_DATA == ulHarq_p->mcsIndex)
        {
             ulHarq_p->riStart = 0;
             ulHarq_p->riLength = 0;
             ulHarq_p->mcsIndex = 0;
        } */
        /* SPR 4525 Changes end */
      /* - SPR_5470 */

        return MAC_SUCCESS;
    }
    else 
    {
        LTE_MAC_UT_LOG(LOG_WARNING, UE_CONF_MGR,
                "Null Harq entry in ulsch pdu\n");
		//fprintf(stderr, "ulHarq_p == NULL\n");
        return MAC_FAILURE;
    }
}
MacRetType prepareUlschHarqPdu (
        FAPI_ulSCHHarqPduInfo_st * ulschHarq_p,
        ULSchUciInfo * ulSchUciInfo_p
        /*SPR 1547 Start*/
        ,UInt8 srsPresent
        /*SPR 1547 End*/
        ,InternalCellIndex internalCellIndex

        )
{   
    /* + SPR_5470 */
    RNTIInfo *rntiInfo_p = PNULL;  
    /* - SPR_5470 */

    if (MAC_SUCCESS != (*prepareUlschPduFunction)(
                &ulschHarq_p->ulSCHPduInfo, ulSchUciInfo_p
                /*SPR 1547 Start*/
                ,srsPresent
                /*SPR 1547 End*/
                ,internalCellIndex))
    {
    /** klockwork Changes Start  **/
         ulschHarq_p->initialTxParamInfo.nSRSInitial = SRS_ULSCH_NO_OVERLAP;
         /*SPR 7086 Fix End*/
         ulschHarq_p->initialTxParamInfo.initialNumOfRB = 1;
        
         rntiInfo_p = getUeIdxFromRNTIMap(ulschHarq_p->ulSCHPduInfo.rnti,internalCellIndex); 
         
         if (rntiInfo_p && (TCRNTI != rntiInfo_p->rntiFlag))
         {
                  LOG_MAC_MSG(HARQ_PONITER_NULL_LOG_ID, LOGWARNING,MAC_L1_INF,
                         getCurrentTick(),rntiInfo_p->index, 
                         DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                         DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                         DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                         FUNCTION_NAME,"RECV_NULL_HARQ_POINTER");
         }
        return MAC_FAILURE;
    }

    /* SPR 609 changes start */    
    /*
       ulschHarq_p->harqInfo.harqSize =ulSchUciInfo_p->dlHarqInfo.harqSize;
       ulschHarq_p->harqInfo.deltaOffsetHarq =ulSchUciInfo_p->dlHarqInfo.deltaOffsetHARQ;
#ifdef TDD_CONFIG
ulschHarq_p->harqInfo.ackNackMode =ulSchUciInfo_p->dlHarqInfo.acknackMode;
#endif
     */

    fillHarqInfo(&(ulschHarq_p->harqInfo), ulSchUciInfo_p
/* ULSCH_HARQ_issue_fix + */
#ifdef FLEXRAN
            , internalCellIndex
#endif
/* ULSCH_HARQ_issue_fix - */
            );
    /* SPR 609 changes end */    
    /* Commented as part of SRS feature. The requirement of these parameters
       will be verified from L1.*/
    /* SPR 1538 Start */
    /*ulschHarq_p->initialTxParamInfo.nSRSInitial = ulSchUciInfo_p->nSRSInitiali; */
    /* + SPR_5470 */
         ulschHarq_p->initialTxParamInfo.initialNumOfRB = 
             ulSchUciInfo_p->ulHarqInfo_p->riLength;
         /*SPR 7086 Fix Start*/
         ulschHarq_p->initialTxParamInfo.nSRSInitial = 
             ulSchUciInfo_p->ulHarqInfo_p->nSrsInitial; 
         /*SPR 7086 Fix End*/
    /** klockwork Changes End **/
    /* - SPR_5470 */
    /* SPR 1538 End */
    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  :fillCqiRiInfoR8 
 * Inputs         : ulschCqiRi_p, ulSchUciInfo_p
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Ulsch Cqi Ri pdu.
 ******************************************************************************/

void fillCqiRiInfoR8(FAPI_ulSCHCqiRiPduInfo_st * ulschCqiRi_p,
        ULSchUciInfo * ulSchUciInfo_p)
{
    /* + CQI_5.0 */
#ifndef FLEXRAN
    ulschCqiRi_p->cqiRiInfo.dlCqiPmiSizeRank_1 = ulSchUciInfo_p->
        cqiRiInfo.dlCqiPmiSizeRank_1;
    ulschCqiRi_p->cqiRiInfo.dlCqiPmiSizeRankGT_1 = ulSchUciInfo_p->
        cqiRiInfo.dlCqiPmiSizeRankGT_1;
    /* - CQI_5.0 */
    ulschCqiRi_p->cqiRiInfo.riSize = ulSchUciInfo_p->cqiRiInfo.riSize;
    ulschCqiRi_p->cqiRiInfo.deltaOffsetCQI = ulSchUciInfo_p->cqiRiInfo.
        deltaOffsetCQI;
    ulschCqiRi_p->cqiRiInfo.deltaOffsetRI = ulSchUciInfo_p->cqiRiInfo.
        deltaOffsetRI;
#else
    ulschCqiRi_p->cqiRiInfo.reportType=ulSchUciInfo_p->cqiRiInfo.reportType;
    ulschCqiRi_p->cqiRiInfo.deltaOffsetCQI=ulSchUciInfo_p->cqiRiInfo.deltaOffsetCQI;
    ulschCqiRi_p->cqiRiInfo.deltaOffsetRI=ulSchUciInfo_p->cqiRiInfo.deltaOffsetRI;
    if( ulschCqiRi_p->cqiRiInfo.reportType==CQI_RI_PERIODIC_MODE)
    {
        if(ulSchUciInfo_p->cqiRiInfo.rankIndicator == 1)
        {

            ulschCqiRi_p->cqiRiInfo.periodicReport[0].dlCqiPmiSize=

                ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1;
        }
        else
        {
            ulschCqiRi_p->cqiRiInfo.periodicReport[0].dlCqiPmiSize=

                ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRankGT_1;

        }
        if (ulschCqiRi_p->cqiRiInfo.periodicReport[0].dlCqiPmiSize > 1)
        {
            ulschCqiRi_p->cqiRiInfo.periodicReport[0].controlType = 0;
        }
        else
        {
            ulschCqiRi_p->cqiRiInfo.periodicReport[0].controlType = 1;
        }
    }
    else
    {
         fprintf(stderr,"Only periodic CSI report is being supported for now in CRAN\n\n");
    }
#endif
}
#ifdef FAPI_4_0_COMPLIANCE
/*****************************************************************************
 * Function Name  :fillCqiRiInfoR9AndLater 
 * Inputs         : ulschCqiRi_p, ulSchUciInfo_p
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Cqi Ri Information for Release 9
 *                  and Later.
 ******************************************************************************/
UInt8 fillCqiRiInfoR9AndLater(FAPI_cqiRiPduInfoRel9AndLater_stT * ulschCqiRi_p,
        ULSchUciInfo * ulSchUciInfo_p)
{
    
    UInt8	iterator=0,rivalues=0,counter = 0,num_rivalues=0;
    UInt16 size = 0;
    dlcqiPmiRISizeAperiodic_st *dlCqiPmiRISize_p=PNULL;
    UInt8 *dlCqiPmiSizeRank_p=PNULL;
    /* CA_phase2_csi_code Start */
    UInt8 cellIdxOfExpctedReport;
    UInt8 numOfCC = 0;
    UInt8 servCellIndex;
    UInt8 index;
    /* CA_phase2_csi_code End */
    /*FAPI2_1 changes start*/
    /*fill report type*/
    ulschCqiRi_p->reportType=ulSchUciInfo_p->cqiRiInfo.reportType;
    ulschCqiRi_p->deltaOffsetCQI=ulSchUciInfo_p->cqiRiInfo.deltaOffsetCQI;
    ulschCqiRi_p->deltaOffsetRI=ulSchUciInfo_p->cqiRiInfo.deltaOffsetRI;
    if( ulschCqiRi_p->reportType==CQI_RI_PERIODIC_MODE)
    {
        if(ulSchUciInfo_p->cqiRiInfo.rankIndicator == 1)
        {

            ulschCqiRi_p->pmiRIReport.periodicReport.dlCqiPmiSize=
                ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1;
        }
        else
        {
            ulschCqiRi_p->pmiRIReport.periodicReport.dlCqiPmiSize=

                ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRankGT_1;

        }

    }
    else
    {
         /* CA_phase2_csi_code Start */
        cellIdxOfExpctedReport = ulSchUciInfo_p->cqiRiInfo.bitMaskForAperiodicReport;
        if(cellIdxOfExpctedReport & servingCellBitMask_g[PCELL_SERV_CELL_IX])
        {
            numOfCC++;
        }
        for(index = START_SCELL_INDEX ;index <= MAX_NUM_SCELL; index++)
        {
            if(cellIdxOfExpctedReport & servingCellBitMask_g[index])
            {
                numOfCC++;
            }
        }

        ulschCqiRi_p->pmiRIReport.aperiodicReport.numberOfCC = numOfCC;
        dlCqiPmiRISize_p=&ulschCqiRi_p->pmiRIReport.aperiodicReport.dlCqiPmiRISize[0];
        for(iterator=0;iterator<ulschCqiRi_p->pmiRIReport.aperiodicReport.numberOfCC;iterator++)
        {
            if(cellIdxOfExpctedReport & servingCellBitMask_g[PCELL_SERV_CELL_IX])
            {
                /* Report for Pcell is expected */
                dlCqiPmiRISize_p->riSize=ulSchUciInfo_p->cqiRiInfo.riSize;
                servCellIndex = PCELL_SERV_CELL_IX;
            }
            else
            {
                /* Report for Scell */
                /* since currently only one Scell is there, so directly fetching
                 * the scell values from first index */
                dlCqiPmiRISize_p->riSize = ulSchUciInfo_p->cqiRiInfo.cqiRiScellInfo[1].
                    riSize;
                servCellIndex = START_SCELL_INDEX ;
            }

          
            rivalues=CALC_RIVALUES(dlCqiPmiRISize_p->riSize);
            num_rivalues+=rivalues;
            size = sizeof(dlcqiPmiRISizeAperiodic_st);

            for(counter=0;counter<rivalues;counter++)
            {

                dlCqiPmiSizeRank_p=&(dlCqiPmiRISize_p->dlCqiPmiSizeRankX[counter]);
                if(PCELL_SERV_CELL_IX == servCellIndex)
                {
                    /* Pcell */
                    /* SPR 10867 Fix Start */
                    /* if RI is 1 */
                    if(counter == 0)
                    /* SPR 10867 Fix End */
                    {
                        *dlCqiPmiSizeRank_p=
                            ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1;
                    }
                    /* if RI is 2 */
                    else
                    {
                        *dlCqiPmiSizeRank_p=
                            ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRankGT_1;
                    }
                }
                else
                {

                    /* Scell */
                    /* SPR 12013 Fix start : Note: this fix is part of 
                       SPR 10867 as fixed above for Pcell */
                    /* if RI is 1 */
                    if(counter == 0)
                    /* SPR 12013 Fix end */
                    {
                        *dlCqiPmiSizeRank_p=
                            ulSchUciInfo_p->cqiRiInfo.cqiRiScellInfo[servCellIndex].dlCqiPmiSizeRank_1;
                    }
                    /* if RI is 2 */
                    else
                    {
                        *dlCqiPmiSizeRank_p=
                            ulSchUciInfo_p->cqiRiInfo.cqiRiScellInfo[servCellIndex].dlCqiPmiSizeRankGT_1;
                    }

                }
                size += sizeof(UInt8);
            }
            cellIdxOfExpctedReport = cellIdxOfExpctedReport & 
                (~servingCellBitMask_g[servCellIndex]);
         /* CA_phase2_csi_code End */
            size-=sizeof(UInt8);
            dlCqiPmiRISize_p = (dlcqiPmiRISizeAperiodic_st *)((UInt8 *)dlCqiPmiRISize_p + size);
        }
         /* CA_phase2_csi_code Start */
        ulSchUciInfo_p->cqiRiInfo.bitMaskForAperiodicReport = 0;
         /* CA_phase2_csi_code End */

        /* ri size for max num cc*/
    }
    /*FAPI2_1 changes end*/
    return num_rivalues;
}
/*****************************************************************************
 * Function Name  : prepareUlschCqiRiPduR9
 * Inputs         : ulPduConf_p, ulSchUciInfo_p, srsPresent, internalCellIndex
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function fills Ulsch Cqi Ri pdu.This function wil be used 
 *                   in case PHY Supports Release 9 or Later
 ******************************************************************************/
MacRetType prepareUlschCqiRiPduR9 (
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p
        /*SPR 1547 Start*/
        ,UInt8 srsPresent
        /*SPR 1547 End*/
        ,InternalCellIndex internalCellIndex
        )
{   
    /* + SPR_5470 */
    ULHarqInfo *ulHarq_p = ulSchUciInfo_p->ulHarqInfo_p;
    UInt8 num_rivalues=0;
    /* - SPR_5470 */

    FAPI_ulSCHCqiRiPduInfoR9_st * ulschCqiRi_p=(FAPI_ulSCHCqiRiPduInfoR9_st*)ulPduConf_p->ulPduConfigInfo;
    if (MAC_SUCCESS != (*prepareUlschPduFunction)(
                &ulschCqiRi_p->ulSCHPduInfo, ulSchUciInfo_p
                /*SPR 1547 Start*/
                ,srsPresent,
                /*SPR 1547 End*/
                internalCellIndex))
    {
        return MAC_FAILURE;
    }
    num_rivalues=fillCqiRiInfoR9AndLater(&ulschCqiRi_p->cqiRiInfo, ulSchUciInfo_p);
    if (PNULL != ulHarq_p)
    {

        /* + SPR_5470 */
        ulschCqiRi_p->initialTxParamInfo.initialNumOfRB = ulHarq_p->riLength;

        /* SPR 4525 Changes start */
        /* Here,resetting the following parameters of ulHarq_p,
         * in case of Aperiodic cqi report without data*/
        if(MCS_INDEX_CQI_WITHOUT_DATA == ulHarq_p->mcsIndex)
        {
            ulHarq_p->riStart = 0;
            ulHarq_p->riLength = 0;
            ulHarq_p->mcsIndex = 0;
        }
        /* SPR 4525 Changes end */
        /* - SPR_5470 */
        ulschCqiRi_p->initialTxParamInfo.nSRSInitial = ulHarq_p->nSrsInitial;
    }
    if(ulSchUciInfo_p->cqiRiInfo.reportType==CQI_RI_PERIODIC_MODE)
    {

        ulPduConf_p->ulConfigPduSize = sizeof(FAPI_ulSCHCqiRiPduInfoR9_st)+
            sizeof(FAPI_ulPDUConfigInfo_st) -1 ;
    }
    else
    {
        ulPduConf_p->ulConfigPduSize = sizeof(FAPI_ulSCHCqiRiPduInfoR9_st)+
            sizeof(FAPI_ulPDUConfigInfo_st) -1  
            + (ulschCqiRi_p->cqiRiInfo.pmiRIReport.aperiodicReport.numberOfCC -1)*sizeof(dlcqiPmiRISizeAperiodic_st)
            - ulschCqiRi_p->cqiRiInfo.pmiRIReport.aperiodicReport.numberOfCC*sizeof(UInt8) + num_rivalues;
    }


    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : prepareUlschCqiHarqRiPduR9 
 * Inputs         : ulPduConf_p, srsPresent,
 *                  ulSchUciInfo_p
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function fills Ulsch Cqi Harq Ri pdu in case
 *                  Release capability is Release 9 or Release 10
 ******************************************************************************/
MacRetType prepareUlschCqiHarqRiPduR9(
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p
        /*SPR 1547 Start*/
        ,UInt8 srsPresent,
        /*SPR 1547 End*/
        InternalCellIndex internalCellIndex
        )
{
    UInt8 num_rivalues=0;
    /* + SPR_5470 */
    FAPI_ulSCHCqiHarqRIPduInfoR9_st * ulschCqiHarqRi_p=(FAPI_ulSCHCqiHarqRIPduInfoR9_st*)ulPduConf_p->ulPduConfigInfo;
    ULHarqInfo *ulHarq_p = ulSchUciInfo_p->ulHarqInfo_p; 
    /* - SPR_5470 */

    if (MAC_SUCCESS != (*prepareUlschPduFunction)(
                &ulschCqiHarqRi_p->ulSCHPduInfo, ulSchUciInfo_p
                /*SPR 1547 Start*/
                ,srsPresent,
                /*SPR 1547 End*/
                internalCellIndex
                ))
    {
        LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,"Error preparing ULSCH PDU\n");
        return MAC_FAILURE;
    }

    num_rivalues=fillCqiRiInfoR9AndLater(&ulschCqiHarqRi_p->cqiRiInfo, ulSchUciInfo_p);
    
    /* + SPR_5470 */
    if (PNULL != ulHarq_p)
    {
        ulschCqiHarqRi_p->initialTxParamInfo.initialNumOfRB = ulHarq_p->riLength;

        /* SPR 4525 Changes start */
        /* Here,resetting the following parameters of ulHarq_p,
         * in case of Aperiodic cqi report without data*/
        if(MCS_INDEX_CQI_WITHOUT_DATA == ulHarq_p->mcsIndex)
        {
            ulHarq_p->riStart = 0;
            ulHarq_p->riLength = 0;
            ulHarq_p->mcsIndex = 0;
        }
        /* SPR 4525 Changes end */
        ulschCqiHarqRi_p->initialTxParamInfo.nSRSInitial = ulHarq_p->nSrsInitial;
    }
    
    /* - SPR_5470 */
    /* SPR 609 changes start */    
    fillHarqInfo(&(ulschCqiHarqRi_p->harqInfo), ulSchUciInfo_p
/* ULSCH_HARQ_issue_fix + */
#ifdef FLEXRAN
            , internalCellIndex
#endif
/* ULSCH_HARQ_issue_fix  */
            );
    /* SPR 609 changes end */ 
    if(ulSchUciInfo_p->cqiRiInfo.reportType==CQI_RI_PERIODIC_MODE)
    {

        ulPduConf_p->ulConfigPduSize = sizeof(FAPI_ulSCHCqiHarqRIPduInfoR9_st)+
            sizeof(FAPI_ulPDUConfigInfo_st) -1 ;
    }
    else
    {
        ulPduConf_p->ulConfigPduSize = sizeof(FAPI_ulSCHCqiHarqRIPduInfoR9_st)+
            sizeof(FAPI_ulPDUConfigInfo_st) -1  
            + (ulschCqiHarqRi_p->cqiRiInfo.pmiRIReport.aperiodicReport.numberOfCC -1)*sizeof(dlcqiPmiRISizeAperiodic_st)
            - ulschCqiHarqRi_p->cqiRiInfo.pmiRIReport.aperiodicReport.numberOfCC*sizeof(UInt8) + num_rivalues;
    }
    return MAC_SUCCESS;

}
#endif	
/*****************************************************************************
 * Function Name  : prepareUlschCqiRiPdu
 * Inputs         : ulschCqiRi_p - pointer to structure FAPI_ulSCHCqiRiPduInfo_st,
 *		            ulSchUciInfo_p - pointer to ULSchUciInfo and
 *                  srsPresent
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function fills Ulsch Cqi Ri pdu.
 ******************************************************************************/
MacRetType prepareUlschCqiRiPdu (
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p
        /*SPR 1547 Start*/
        ,UInt8 srsPresent
        /*SPR 1547 End*/
        ,InternalCellIndex internalCellIndex
        )
{   
    /* + SPR_5470 */
    ULHarqInfo *ulHarq_p = ulSchUciInfo_p->ulHarqInfo_p; 
    RNTIInfo *rntiInfo_p = PNULL; 
    /* - SPR_5470 */
    FAPI_ulSCHCqiRiPduInfo_st * ulschCqiRi_p=(FAPI_ulSCHCqiRiPduInfo_st*)ulPduConf_p->ulPduConfigInfo;

    if (MAC_SUCCESS != (*prepareUlschPduFunction)(
                &ulschCqiRi_p->ulSCHPduInfo, ulSchUciInfo_p
                /*SPR 1547 Start*/
                ,srsPresent
                /*SPR 1547 End*/
                ,internalCellIndex))
    {
        /*KLOCKWORK_CHANGES_START_55*/ 
        /*SPR 7086 Fix Start*/
        if (ulHarq_p == PNULL)
        {
            ulschCqiRi_p->initialTxParamInfo.nSRSInitial = SRS_ULSCH_NO_OVERLAP;
            /*SPR 7086 Fix End*/
            ulschCqiRi_p->initialTxParamInfo.initialNumOfRB = 1;

            rntiInfo_p = getUeIdxFromRNTIMap(ulschCqiRi_p->ulSCHPduInfo.rnti,internalCellIndex);
            /* COVERITY 69272 START */

            if (rntiInfo_p && (TCRNTI != rntiInfo_p->rntiFlag))
            {
                LOG_MAC_MSG(HARQ_PONITER_NULL_LOG_ID, LOGWARNING,MAC_L1_INF,
                        getCurrentTick(),rntiInfo_p->index, 
                        DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"RECV_NULL_HARQ_POINTER");
            }
        }
        return MAC_FAILURE;
        /*KLOCKWORK_CHANGES_STOP_55*/ 
    }
            /* COVERITY 69272 END */
    fillCqiRiInfoR8(ulschCqiRi_p, ulSchUciInfo_p);

    /* + SPR_5470 */
    if (PNULL != ulHarq_p)
    {
        ulschCqiRi_p->initialTxParamInfo.initialNumOfRB = ulHarq_p->riLength;

        /* SPR 4525 Changes start */
        /* Here,resetting the following parameters of ulHarq_p,
         * in case of Aperiodic cqi report without data*/
        if(MCS_INDEX_CQI_WITHOUT_DATA == ulHarq_p->mcsIndex)
        {
            ulHarq_p->riStart = 0;
            ulHarq_p->riLength = 0;
            ulHarq_p->mcsIndex = 0;
			fprintf(stderr,"set ri to zero for mscIndex is 29\n");
        }
        /*SPR 7086 Fix Start*/
        ulschCqiRi_p->initialTxParamInfo.nSRSInitial = ulHarq_p->nSrsInitial;
        /*SPR 7086 Fix End*/
    }
    /* - SPR_5470 */
    ulPduConf_p->ulConfigPduSize = sizeof(FAPI_ulSCHCqiRiPduInfo_st)+
        sizeof(FAPI_ulPDUConfigInfo_st) -1;


    return MAC_SUCCESS;
}

/* SPR 609 changes start */    
/*****************************************************************************
 * Function Name  : fillHarqInfo
 * Inputs         : ulSchHarqInfo_p - pointer to structure FAPI_ulSCHHarqInfo_st
 *		            and ulSchUciInfo_p - pointer to ULSchUciInfo
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Harq Info in ULSCH pdu.
 ******************************************************************************/
void fillHarqInfo (
		FAPI_ulSCHHarqInfo_st *ulSchHarqInfo_p,
        ULSchUciInfo * ulSchUciInfo_p
/* ULSCH_HARQ_issue_fix + */
#ifdef FLEXRAN
        , InternalCellIndex internalCellIndex
#endif
/* ULSCH_HARQ_issue_fix - */
        )
{
    ulSchHarqInfo_p->harqSize =ulSchUciInfo_p->dlHarqInfo.harqSize;
    ulSchHarqInfo_p->deltaOffsetHarq =ulSchUciInfo_p->dlHarqInfo.deltaOffsetHARQ;
#ifdef FDD_CONFIG
/* SPR 12243 fix start */
#ifdef FAPI_4_0_COMPLIANCE
/* SPR 12243 fix end */
    ulSchHarqInfo_p->ackNackMode =ulSchUciInfo_p->dlHarqInfo.ackNackMode;
/* SPR 12243 fix start */
#endif
/* SPR 12243 fix end */
    /* SPR 1549 changes start */

#else
/** SPR 14149 Fix : Start **/
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
    ulSchHarqInfo_p->ackNackMode = ulSchUciInfo_p->dlHarqInfo.ackNackMode;
#else
    /* SPR 19679: TDD HARQ Multiplexing Changes Start */ 
    /* SPR 20016 Fix Start */
    ulSchHarqInfo_p->ackNackMode = ulSchUciInfo_p->dlHarqInfo.acknackMode;
    /* SPR 20016 Fix End */
    /* SPR 19679: TDD HARQ Multiplexing Changes End */ 
#endif
/** SPR 14149 Fix : End **/
    /* SPR 1549 changes end */

/* ULSCH_HARQ_issue_fix + */
#ifdef FLEXRAN
    if (ACK_NACK_BUNDLING == ulSchHarqInfo_p->ackNackMode)
    {
        UInt16 ueIndex = 0;
        RNTIInfo *rntiInfo_p = PNULL;
        UInt8 nBundle = 0;
        rntiInfo_p = getUeIdxFromRNTIMap(ulSchUciInfo_p->rnti, internalCellIndex);
        /*QOS_TCP_FIX */
        if (rntiInfo_p && (TCRNTI != rntiInfo_p->rntiFlag) &&
                dlUECtxInfoArr_g[rntiInfo_p->index].dlUEContext_p)
        {
            ueIndex = rntiInfo_p->index;
            nBundle = dlUECtxInfoArr_g[ueIndex].dlUEContext_p->nBundle[(systemFrameNum_g[internalCellIndex] + tddPdschTxMapOfDLHarq_p[internalCellIndex][subFrameNum_g[internalCellIndex]].sfnFactor) % 2][tddPdschTxMapOfDLHarq_p[internalCellIndex][subFrameNum_g[internalCellIndex]].dlAckNackSf];
				//fprintf(stderr," nBundle [%d] Tick [%llu]\n", ueContext_p->nBundle, getCurrentTick());

            ulSchHarqInfo_p->ackNackBundlingScrambSeqIndex = (nBundle-1)%4;
			if(nBundle==0)
                fprintf(stderr,"nBundle [%d] ueindex [%d] rnti [%d] crnti[%d] Tick [%llu]\n", nBundle, ueIndex,ulSchUciInfo_p->rnti,dlUECtxInfoArr_g[ueIndex].dlUEContext_p->crnti,getCurrentTick());
        }
    }
#endif
/* ULSCH_HARQ_issue_fix - */
#endif
}
/* SPR 609 changes end */    

/*****************************************************************************
 * Function Name  : prepareUlschCqiHarqRiPdu
 * Inputs         : ulschCqiHarqRi_p - pointer to structure FAPI_ulSCHCqiHarqRIPduInfo_st,
 *                  ulSchUciInfo_p - pointer to ULSchUciInfo and
 *                  srsPresent
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function fills Ulsch Cqi Harq Ri pdu.
 ******************************************************************************/
MacRetType prepareUlschCqiHarqRiPdu (
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p
        /*SPR 1547 Start*/
        ,UInt8 srsPresent,
        /*SPR 1547 End*/
        InternalCellIndex internalCellIndex
        )
{
    /* + SPR_5470 */
    FAPI_ulSCHCqiHarqRIPduInfo_st * ulschCqiHarqRi_p= (FAPI_ulSCHCqiHarqRIPduInfo_st *)ulPduConf_p->ulPduConfigInfo;
    ULHarqInfo *ulHarq_p = ulSchUciInfo_p->ulHarqInfo_p;
    RNTIInfo *rntiInfo_p = PNULL;  
    /* - SPR_5470 */

    if (MAC_SUCCESS != (*prepareUlschPduFunction)(
                &ulschCqiHarqRi_p->ulSCHPduInfo
				, ulSchUciInfo_p
                /*SPR 1547 Start*/
                ,srsPresent
                /*SPR 1547 End*/
                ,internalCellIndex
                ))
    {
        /*KLOCKWORK_CHANGES_START_56*/
        /*SPR 7086 Fix Start*/
        if ( ulHarq_p == PNULL)
        {
            ulschCqiHarqRi_p->initialTxParamInfo.nSRSInitial = SRS_ULSCH_NO_OVERLAP;
            /*SPR 7086 Fix End*/
            ulschCqiHarqRi_p->initialTxParamInfo.initialNumOfRB = 1;

            rntiInfo_p = getUeIdxFromRNTIMap(
			ulschCqiHarqRi_p->ulSCHPduInfo.rnti
			,internalCellIndex);

            if (rntiInfo_p && (TCRNTI != rntiInfo_p->rntiFlag))
            {
                LOG_MAC_MSG(HARQ_PONITER_NULL_LOG_ID, LOGWARNING,MAC_L1_INF,
                        getCurrentTick(),rntiInfo_p->index, 
                        DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"RECV_NULL_HARQ_POINTER");
            }
        }
        LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,"Error preparing ULSCH PDU\n");
        return MAC_FAILURE;
        /*KLOCKWORK_CHANGES_STOP_56*/
    }
#ifdef FLEXRAN
    ulschCqiHarqRi_p->cqiRiInfo.reportType=ulSchUciInfo_p->cqiRiInfo.reportType;
    ulschCqiHarqRi_p->cqiRiInfo.deltaOffsetCQI=ulSchUciInfo_p->cqiRiInfo.deltaOffsetCQI;
    ulschCqiHarqRi_p->cqiRiInfo.deltaOffsetRI=ulSchUciInfo_p->cqiRiInfo.deltaOffsetRI;
    if( ulschCqiHarqRi_p->cqiRiInfo.reportType==CQI_RI_PERIODIC_MODE)
    {
        if(ulSchUciInfo_p->cqiRiInfo.rankIndicator == 1)
        {

            ulschCqiHarqRi_p->cqiRiInfo.periodicReport[0].dlCqiPmiSize=

                ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1;
        }
        else
        {
            ulschCqiHarqRi_p->cqiRiInfo.periodicReport[0].dlCqiPmiSize=

                ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRankGT_1;

        }
        if (ulschCqiHarqRi_p->cqiRiInfo.periodicReport[0].dlCqiPmiSize > 1)
        {
            ulschCqiHarqRi_p->cqiRiInfo.periodicReport[0].controlType = 0;
        }
        else
        {
            ulschCqiHarqRi_p->cqiRiInfo.periodicReport[0].controlType = 1;
        }
    }
    else
    {
         fprintf(stderr,"Only periodic CSI report is being supported for now in CRAN\n\n");
    }
    ulschCqiHarqRi_p->initialTxParamInfo.initialNumOfRB = ulHarq_p->riLength;
#else
    /* + CQI_5.0 */
    ulschCqiHarqRi_p->cqiRiInfo.dlCqiPmiSizeRank_1 = ulSchUciInfo_p->cqiRiInfo.
        dlCqiPmiSizeRank_1;
    ulschCqiHarqRi_p->cqiRiInfo.dlCqiPmiSizeRankGT_1 = ulSchUciInfo_p->
        cqiRiInfo.dlCqiPmiSizeRankGT_1;
    /* - CQI_5.0 */
    ulschCqiHarqRi_p->cqiRiInfo.riSize = ulSchUciInfo_p->cqiRiInfo.riSize;
    ulschCqiHarqRi_p->cqiRiInfo.deltaOffsetCQI = ulSchUciInfo_p->cqiRiInfo.
        deltaOffsetCQI;
    ulschCqiHarqRi_p->cqiRiInfo.deltaOffsetRI = ulSchUciInfo_p->cqiRiInfo.
        deltaOffsetRI;
    
     /* + SPR_5470 */
         ulschCqiHarqRi_p->initialTxParamInfo.initialNumOfRB = ulHarq_p->riLength;
#endif
    
         /* SPR 4525 Changes start */
         /* Here,resetting the following parameters of ulHarq_p,
          * in case of Aperiodic cqi report without data*/
         if(MCS_INDEX_CQI_WITHOUT_DATA == ulHarq_p->mcsIndex)
         {
             ulHarq_p->riStart = 0;
             ulHarq_p->riLength = 0;
             ulHarq_p->mcsIndex = 0;
			 fprintf(stderr,"set ri to zero for mcsIndex is 19_01\n");
         }
         /* SPR 4525 Changes end */
         /*SPR 7086 Fix Start*/
         ulschCqiHarqRi_p->initialTxParamInfo.nSRSInitial = ulHarq_p->nSrsInitial;
         /*SPR 7086 Fix End*/

    /* - SPR_5470 */
    /* SPR 609 changes start */    
    fillHarqInfo(&(ulschCqiHarqRi_p->harqInfo), ulSchUciInfo_p, internalCellIndex);
    /* SPR 609 changes end */ 
    ulPduConf_p->ulConfigPduSize = sizeof(FAPI_ulSCHCqiHarqRIPduInfo_st)+
        sizeof(FAPI_ulPDUConfigInfo_st) -1;

    return MAC_SUCCESS;

}

/*****************************************************************************
 * Function Name  : prepareUciSrPdu
 * Inputs         : uciSrPdu_p - pointer to structure FAPI_uciSrPduInfo_st,
 *		            ulSchUciInfo_p - pointer to ULSchUciInfo
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Uci sr pdu.
 ******************************************************************************/
void prepareUciSrPdu (
        FAPI_uciSrPduInfo_st * uciSrPdu_p,
        ULSchUciInfo * ulSchUciInfo_p
        ,InternalCellIndex internalCellIndex
        )
{
    uciSrPdu_p->handle = MAC_PHY_CONVERT_32(generateHandleVal(internalCellIndex));
    uciSrPdu_p->rnti = MAC_PHY_CONVERT_16(ulSchUciInfo_p->rnti);
    uciSrPdu_p->srInfo.pucchIndex = MAC_PHY_CONVERT_16(ulSchUciInfo_p->srInfo.pucchIndex);
    /*FAPI_2_1 changes start*/
    fillSRRelease10Param(&(uciSrPdu_p->srInfo));
    /*FAPI_2_1 changes end*/

    return;
}
/*FAPI_2_1 changes start*/
/*****************************************************************************
 * Function Name  : fillSRRelease10Param 
 * Inputs         : srPdu_p 
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills SR release10 Parameters .
 ******************************************************************************/
void fillSRRelease10Param(
		FAPI_srPduInfo_st * srPdu_p
		)
{

#ifdef FAPI_4_0_COMPLIANCE
    srPdu_p->release10Param.numPucchRes = DEFAULT_NUM_PUCCH_RESOURCE;
    srPdu_p->release10Param.pucchIndexP1 =MAC_PHY_CONVERT_16(DEFAULT_PUCCHINDEX_P1);
#else 
    (void)srPdu_p;/*SPR 19565 warning fix*/
#endif	
    return;
}
/*FAPI2_1 changes end*/

#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : fapiFillTddHarqInfo
 * Inputs         : tddHarqInfo_p - pointer to structure FAPI_tddHarqPduInfo_st and
 *                  dlHarqInfo_p - pointer to MacSSTddHarqInfo
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills tdd specific harq pdu.
 ******************************************************************************/
void fapiFillTddHarqInfo (
        FAPI_tddHarqPduInfo_st     *tddHarqInfo_p,
        MacSSTddHarqInfo           *dlHarqInfo_p
        )
{
    /* For ACK_NACK Mode 0 (Bundling) and 1 (Multiplexing), 
       this is the size of the ACK/NACK in bits.
Value: 1 . 4
For Special Bundling this is the expected number of ACK/NACK 
responses (UDAI + NSPS).
Value: 0 . 9	 */
    tddHarqInfo_p->harqSize = dlHarqInfo_p->harqSize; 

    /* The format of the ACK/NACK response expected. For TDD only.
       0 = BUNDLING
       1 = MULTIPLEXING		*/
//#ifndef FAPI_4_0_COMPLIANCE
#if 0
    tddHarqInfo_p->acknackMode= dlHarqInfo_p->acknackMode; 
#else
    tddHarqInfo_p->acknackMode= dlHarqInfo_p->ackNackMode; 
#endif
    /* The number of ACK/NACK responses received in this subframe. 
       For TDD only. 
Value: 0 . 4
(Value 0 is only valid for Special Bundling.)	*/

    tddHarqInfo_p->numOfPUCCHResource = dlHarqInfo_p->numOfPUCCHResource;
    /** SPR 14147_14190 Fix : Start **/
    tddHarqInfo_p->n_PUCCH_1_0 = MAC_PHY_CONVERT_16(dlHarqInfo_p->n_PUCCH_1_0);
    tddHarqInfo_p->n_PUCCH_1_1 = MAC_PHY_CONVERT_16(dlHarqInfo_p->n_PUCCH_1_1);
    tddHarqInfo_p->n_PUCCH_1_2 = MAC_PHY_CONVERT_16(dlHarqInfo_p->n_PUCCH_1_2);
    tddHarqInfo_p->n_PUCCH_1_3 = MAC_PHY_CONVERT_16(dlHarqInfo_p->n_PUCCH_1_3);
    /** SPR 14147_14190 Fix : End **/
}
#endif	
#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : fillUCIHarqInfo 
 * Inputs         : uciHarq_p 
 *		    ulSchUciInfo_p
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Uci harq pdui for release8
 ******************************************************************************/

void fillUCIHarqInfo(
FAPI_fddHarqPduInfo_st *uciHarq_p, 
ULSchUciInfo * ulSchUciInfo_p)
{
    /*The PUCCH Index value for ACK/NACK
     *
     * Value: 0 . 2047 (ACK_NACK mode = 0 or 1)
     * Value: 0 . 549 (ACK_NACK mode = 2)
     *
     * */
#ifdef FAPI_4_0_COMPLIANCE
    uciHarq_p->pucchIndex = MAC_PHY_CONVERT_16(ulSchUciInfo_p->dlHarqInfo.n_PUCCH_1_0);
#else
    uciHarq_p->pucchIndex = MAC_PHY_CONVERT_16(ulSchUciInfo_p->dlHarqInfo.pucchIndex); 
#endif
#if ((!defined(FAPI_4_0_COMPLIANCE)) &&  (!defined(FAPI_1_1_COMPLIANCE)))
    uciHarq_p->pucchIndex1= MAC_PHY_CONVERT_16(ulSchUciInfo_p->dlHarqInfo.pucchIndex); 
#endif
    uciHarq_p->harqSize = ulSchUciInfo_p->dlHarqInfo.harqSize;
}
#ifdef FAPI_4_0_COMPLIANCE
/*****************************************************************************
 * Function Name  : fillUCIHarqInfoR9 
 * Inputs         : uciHarq_p 
 *		    ulSchUciInfo_p
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Uci harq information for release9 and 
 *                  Later
 ******************************************************************************/

void fillUCIHarqInfoR9(FAPI_fddHarqPduInfoR9AndLater_st *uciHarq_p, ULSchUciInfo * ulSchUciInfo_p)
{
    /*The PUCCH Index value for ACK/NACK
     *
     * Value: 0 . 2047 (ACK_NACK mode = 0 or 1)
     * Value: 0 . 549 (ACK_NACK mode = 2)
     *
     * */
    uciHarq_p->harqSize = ulSchUciInfo_p->dlHarqInfo.harqSize;
    uciHarq_p->ackNackMode =ulSchUciInfo_p->dlHarqInfo.ackNackMode;
    /*Assumption: valid values in dlHarqInfo will be received according to  numOfPUCCHResource*/
    uciHarq_p->numOfPUCCHResource = ulSchUciInfo_p->dlHarqInfo.numOfPUCCHResource;
    uciHarq_p->n_PUCCH_1_0 = MAC_PHY_CONVERT_16(ulSchUciInfo_p->dlHarqInfo.n_PUCCH_1_0);
    uciHarq_p->n_PUCCH_1_1 = MAC_PHY_CONVERT_16(ulSchUciInfo_p->dlHarqInfo.n_PUCCH_1_1);
    uciHarq_p->n_PUCCH_1_2 = MAC_PHY_CONVERT_16(ulSchUciInfo_p->dlHarqInfo.n_PUCCH_1_2);
    uciHarq_p->n_PUCCH_1_3 = MAC_PHY_CONVERT_16(ulSchUciInfo_p->dlHarqInfo.n_PUCCH_1_3);


}
#endif
#endif	


/*****************************************************************************
 * Function Name  : prepareUciHarqPdu
 * Inputs         : uciHarq_p - pointer to structure FAPI_uciHarqPduInfo_st and
 *      		    ulSchUciInfo_p - pointer to ULSchUciInfo
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Uci harq pdu.
 ******************************************************************************/
void prepareUciHarqPdu (
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p
        ,InternalCellIndex internalCellIndex
        )
{

    FAPI_uciHarqPduInfo_st *uciHarq_p=(FAPI_uciHarqPduInfo_st*)ulPduConf_p->ulPduConfigInfo;
    uciHarq_p->handle = MAC_PHY_CONVERT_32(generateHandleVal(internalCellIndex)); 
    uciHarq_p->rnti = MAC_PHY_CONVERT_16(ulSchUciInfo_p->rnti);
#ifdef FDD_CONFIG
    fillUCIHarqInfo(&uciHarq_p->harqInfo, ulSchUciInfo_p);

#elif TDD_CONFIG
    FAPI_tddHarqPduInfo_st     *tddHarqInfo_p  = PNULL;
    MacSSTddHarqInfo           *dlHarqInfo_p   = PNULL;

    tddHarqInfo_p = &(uciHarq_p->harqInfo);  
    dlHarqInfo_p  = &(ulSchUciInfo_p->dlHarqInfo);	
    fapiFillTddHarqInfo(tddHarqInfo_p, dlHarqInfo_p);

#endif
    ulPduConf_p->ulConfigPduSize = sizeof(FAPI_uciHarqPduInfo_st)+
        sizeof(FAPI_ulPDUConfigInfo_st) -1;
    return;
}
#ifdef FAPI_4_0_COMPLIANCE
/*****************************************************************************
 * Function Name  : prepareUciHarqPduR9
 * Inputs         : msg_p -void pointer pointing to FAPI_uciHarqPduInfoR9_st
 *		            ulSchUciInfo_p
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Uci harq pdu in case PHY Supports
 *                   Release 9 or Later
 ******************************************************************************/
void prepareUciHarqPduR9(
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p,
        InternalCellIndex internalCellIndex)
{

    FAPI_uciHarqPduInfoR9_st *uciHarq_p=(FAPI_uciHarqPduInfoR9_st*)ulPduConf_p->ulPduConfigInfo;
    uciHarq_p->handle = MAC_PHY_CONVERT_32(generateHandleVal(internalCellIndex)); 
    uciHarq_p->rnti = MAC_PHY_CONVERT_16(ulSchUciInfo_p->rnti);

#ifdef FDD_CONFIG
    fillUCIHarqInfoR9(&uciHarq_p->harqInfo, ulSchUciInfo_p);
#elif TDD_CONFIG
    FAPI_tddHarqPduInfo_st     *tddHarqInfo_p  = PNULL;
    MacSSTddHarqInfo           *dlHarqInfo_p   = PNULL;

    tddHarqInfo_p = &(uciHarq_p->harqInfo);  
    dlHarqInfo_p  = &(ulSchUciInfo_p->dlHarqInfo);	
    fapiFillTddHarqInfo(tddHarqInfo_p, dlHarqInfo_p);

#endif
    ulPduConf_p->ulConfigPduSize = sizeof(FAPI_uciHarqPduInfoR9_st)+
        sizeof(FAPI_ulPDUConfigInfo_st) -1;

    return;
}
#endif	

/*FAPI2_1 changes start*/
/*****************************************************************************
 * Function Name  : fillCQIRelease10Param 
 * Inputs         : cqiPdu_p
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills CQI release10  parameters.
 ******************************************************************************/
void fillCQIRelease10Param(
		FAPI_cqiPduInfo_st *cqiPdu_p 
		)
{

#ifdef FAPI_4_0_COMPLIANCE
    cqiPdu_p->release10Param.numPucchRes =DEFAULT_NUM_PUCCH_RESOURCE ;
    cqiPdu_p->release10Param.pucchIndexP1 = MAC_PHY_CONVERT_16(DEFAULT_PUCCHINDEX_P1);
#else 
    (void)cqiPdu_p;/*SPR 19565 warning fix*/
#endif	
    return;
}
/*****************************************************************************
 * Function Name  : prepareUciSrHarqPdu
 * Inputs         : msg_p - void pointer pointing to structure 
 *                  FAPI_uciSrHarqPduInfo_st and
 *      		    ulSchUciInfo_p - pointer to ULSchUciInfo
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Uci SR harq pdu.
 ******************************************************************************/
void prepareUciSrHarqPdu (

        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p,
        InternalCellIndex internalCellIndex
        )
{
    FAPI_uciSrHarqPduInfo_st *uciSrHarq=(FAPI_uciSrHarqPduInfo_st *)ulPduConf_p->ulPduConfigInfo;
    uciSrHarq->handle = MAC_PHY_CONVERT_32(generateHandleVal(internalCellIndex));
    uciSrHarq->rnti   = MAC_PHY_CONVERT_16(ulSchUciInfo_p->rnti);
    uciSrHarq->srInfo.pucchIndex = MAC_PHY_CONVERT_16(ulSchUciInfo_p->srInfo.pucchIndex);
#ifdef TDD_CONFIG
    FAPI_tddHarqPduInfo_st     *tddHarqInfo_p  = PNULL;
    MacSSTddHarqInfo           *dlHarqInfo_p   = PNULL;

    tddHarqInfo_p = &(uciSrHarq->harqInfo);  
    dlHarqInfo_p  = &(ulSchUciInfo_p->dlHarqInfo);	
    fapiFillTddHarqInfo(tddHarqInfo_p, dlHarqInfo_p);

#else
    fillUCIHarqInfo(
			&uciSrHarq->harqInfo,
            ulSchUciInfo_p
            );
#endif
    /* + SPR_15163 */
    fillSRRelease10Param(&(uciSrHarq->srInfo));
    /* - SPR_15163 */
    ulPduConf_p->ulConfigPduSize = sizeof(FAPI_uciSrHarqPduInfo_st) +
        sizeof(FAPI_ulPDUConfigInfo_st) -1;
    return;
}
#ifdef FAPI_4_0_COMPLIANCE
/*****************************************************************************
 * Function Name  : prepareUciSrHarqPduR9
 * Inputs         : uciSrHarq -void  pointer pointing to structure 
 *                  FAPI_uciSrHarqPduInfoR9_st and
 *      		    ulSchUciInfo_p - pointer to ULSchUciInfo
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Uci SR harq pdu in case PHY Supports
 *                  Release9 or Later
 ******************************************************************************/
void prepareUciSrHarqPduR9(
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p,
        InternalCellIndex internalCellIndex)
{

    FAPI_uciSrHarqPduInfoR9_st *uciSrHarq=(FAPI_uciSrHarqPduInfoR9_st *)ulPduConf_p->ulPduConfigInfo;
    uciSrHarq->handle = MAC_PHY_CONVERT_32(generateHandleVal(internalCellIndex ));
    uciSrHarq->rnti   = MAC_PHY_CONVERT_16(ulSchUciInfo_p->rnti);
    uciSrHarq->srInfo.pucchIndex = MAC_PHY_CONVERT_16(ulSchUciInfo_p->srInfo.pucchIndex);
#ifdef TDD_CONFIG
    FAPI_tddHarqPduInfo_st     *tddHarqInfo_p  = PNULL;
    MacSSTddHarqInfo           *dlHarqInfo_p   = PNULL;

    tddHarqInfo_p = &(uciSrHarq->harqInfo);  
    dlHarqInfo_p  = &(ulSchUciInfo_p->dlHarqInfo);	
    fapiFillTddHarqInfo(tddHarqInfo_p, dlHarqInfo_p);

#else
    fillUCIHarqInfoR9(&uciSrHarq->harqInfo,
            ulSchUciInfo_p
            );
#endif
    /* + SPR_15163 */
    fillSRRelease10Param(&(uciSrHarq->srInfo));
    /* - SPR_15163 */
    ulPduConf_p->ulConfigPduSize = sizeof(FAPI_uciSrHarqPduInfoR9_st) +
        sizeof(FAPI_ulPDUConfigInfo_st) -1;
    return;
}

#endif	
/*****************************************************************************
 * Function Name  : prepareUciCqiPdu
 * Inputs         : uciCqi - pointer to structure FAPI_uciCqiPduInfo_st and
 *                  ulSchUciInfo_p - pointer to ULSchUciInfo
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Uci Cqi Pdu.
 ******************************************************************************/
void prepareUciCqiPdu(
        FAPI_uciCqiPduInfo_st *uciCqi,
        ULSchUciInfo * ulSchUciInfo_p,
        InternalCellIndex internalCellIndex
        )
{
    uciCqi->handle = MAC_PHY_CONVERT_32(generateHandleVal(internalCellIndex));
    uciCqi->rnti = MAC_PHY_CONVERT_16(ulSchUciInfo_p->rnti);
    uciCqi->cqiInfo.pucchIndex = MAC_PHY_CONVERT_16(ulSchUciInfo_p->cqiRiInfo.pucchIndex);
    if (0 != ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1)
    {
        uciCqi->cqiInfo.dlCqiPmiSize = ulSchUciInfo_p->cqiRiInfo.
            dlCqiPmiSizeRank_1;
    }
    else if (0 != ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRankGT_1) 
    {
        uciCqi->cqiInfo.dlCqiPmiSize = ulSchUciInfo_p->cqiRiInfo.
            dlCqiPmiSizeRankGT_1;
    }
    else
    {
        uciCqi->cqiInfo.dlCqiPmiSize = ulSchUciInfo_p->cqiRiInfo.
            riSize;
    }
    /*FAPI2_1 changes start*/
    fillCQIRelease10Param(&uciCqi->cqiInfo);
    /*FAPI2_1 changes end*/


    return;
}    
/*****************************************************************************
 * Function Name  : prepareUciCqiSrPdu
 * Inputs         : uciCqiSr - pointer to structure FAPI_uciCqiSrPduInfo_st
 *                  ulSchUciInfo_p - pointer to ULSchUciInfo
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Uci Cqi Sr Pdu.
 ******************************************************************************/
void prepareUciCqiSrPdu(
        FAPI_uciCqiSrPduInfo_st *uciCqiSr,
        ULSchUciInfo * ulSchUciInfo_p
        ,InternalCellIndex internalCellIndex
        )
{
    uciCqiSr->handle = MAC_PHY_CONVERT_32(generateHandleVal(internalCellIndex));
    uciCqiSr->rnti =  MAC_PHY_CONVERT_16(ulSchUciInfo_p->rnti);
    uciCqiSr->cqiInfo.pucchIndex =  MAC_PHY_CONVERT_16(ulSchUciInfo_p->cqiRiInfo.pucchIndex);
    if (0 == ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1 && 
            0 == ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRankGT_1)
    {
        uciCqiSr->cqiInfo.dlCqiPmiSize = ulSchUciInfo_p->cqiRiInfo.
            riSize;
    }
    else
    {
        if ( 1 == ulSchUciInfo_p->cqiRiInfo.rankIndicator)
        {
            uciCqiSr->cqiInfo.dlCqiPmiSize = ulSchUciInfo_p->cqiRiInfo.
                dlCqiPmiSizeRank_1;
        }
        else if (1 < ulSchUciInfo_p->cqiRiInfo.rankIndicator)
        {
            uciCqiSr->cqiInfo.dlCqiPmiSize = ulSchUciInfo_p->cqiRiInfo.
                dlCqiPmiSizeRankGT_1;
        }
    }
    uciCqiSr->srInfo.pucchIndex =  MAC_PHY_CONVERT_16(ulSchUciInfo_p->srInfo.pucchIndex);
    /*FAPI2.1 changes start*/
    fillCQIRelease10Param(&uciCqiSr->cqiInfo);
    fillSRRelease10Param(&uciCqiSr->srInfo);
    /*FAPI2.1 changes end*/

    return;
}
/*****************************************************************************
 * Function Name  : prepareUciCqiHarqPdu
 * Inputs         : uciCqiHarq 
 *                  ulSchUciInfo_p
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Uci Cqi Harq Pdu.
 ******************************************************************************/
void prepareUciCqiHarqPdu(
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p
        ,InternalCellIndex internalCellIndex
        )
{
    FAPI_uciCqiHarqPduInfo_st *uciCqiHarq=(FAPI_uciCqiHarqPduInfo_st*)ulPduConf_p->ulPduConfigInfo;
    uciCqiHarq->handle = MAC_PHY_CONVERT_32(generateHandleVal(internalCellIndex));
    uciCqiHarq->cqiInfo.pucchIndex = MAC_PHY_CONVERT_16(ulSchUciInfo_p->cqiRiInfo.pucchIndex);
    if (0 != ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1)
    {
        uciCqiHarq->cqiInfo.dlCqiPmiSize = ulSchUciInfo_p->cqiRiInfo.
            dlCqiPmiSizeRank_1;
    }
    else if (0 != ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRankGT_1)
    {
        uciCqiHarq->cqiInfo.dlCqiPmiSize = ulSchUciInfo_p->cqiRiInfo.
            dlCqiPmiSizeRankGT_1;
    }
    else
    {
        uciCqiHarq->cqiInfo.dlCqiPmiSize = ulSchUciInfo_p->cqiRiInfo.
            riSize;
    }
    /* - CL-MIMO LJA*/    
    uciCqiHarq->rnti = MAC_PHY_CONVERT_16(ulSchUciInfo_p->rnti);
#ifdef FDD_CONFIG
    /*FAPI2.1 changes start*/
    fillUCIHarqInfo(&uciCqiHarq->harqInfo, ulSchUciInfo_p);
    /*FAPI2.1 changes end*/

#elif TDD_CONFIG
    FAPI_tddHarqPduInfo_st     *tddHarqInfo_p  = PNULL;
    MacSSTddHarqInfo           *dlHarqInfo_p   = PNULL;

    tddHarqInfo_p = &(uciCqiHarq->harqInfo);  
    dlHarqInfo_p  = &(ulSchUciInfo_p->dlHarqInfo);	
    fapiFillTddHarqInfo(tddHarqInfo_p, dlHarqInfo_p);
    /* SPR 19679: TDD HARQ Multiplexing Changes Start */ 
    /* SPR 20024 Fix Start */
#if defined FAPI_1_1_COMPLIANCE
    tddHarqInfo_p->numOfPUCCHResource = 0;

    /* SPR 20024 Fix End */
    /* SPR 19679: TDD HARQ Multiplexing Changes End */ 

    /* Fill release 10 parameters for TDD */
#endif

#endif
    /* + SPR_15163 */
    fillCQIRelease10Param(&(uciCqiHarq->cqiInfo));
    /* - SPR_15163 */
    ulPduConf_p->ulConfigPduSize = sizeof(FAPI_uciCqiHarqPduInfo_st)+
        sizeof(FAPI_ulPDUConfigInfo_st) -1;
    return;
}
#ifdef FAPI_4_0_COMPLIANCE
/*****************************************************************************
 * Function Name  : prepareUciCqiHarqPduR9
 * Inputs         : uciCqiHarq 
 *                  ulSchUciInfo_p
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Uci Cqi Harq Pdu in case PHY Suports
 *                  Release9 or Later
 ******************************************************************************/
void prepareUciCqiHarqPduR9(
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p
        ,InternalCellIndex internalCellIndex
        )
{

    FAPI_uciCqiHarqPduInfoR9_st *uciCqiHarq=(FAPI_uciCqiHarqPduInfoR9_st*)ulPduConf_p->ulPduConfigInfo;
    uciCqiHarq->handle = MAC_PHY_CONVERT_32(generateHandleVal(internalCellIndex));
    uciCqiHarq->cqiInfo.pucchIndex = MAC_PHY_CONVERT_16(ulSchUciInfo_p->cqiRiInfo.pucchIndex);
    /* + CL-MIMO LJA*/
    if (0 != ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1)
    {
        uciCqiHarq->cqiInfo.dlCqiPmiSize = ulSchUciInfo_p->cqiRiInfo.
            dlCqiPmiSizeRank_1;
    }
    else if (0 != ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRankGT_1)
    {
        uciCqiHarq->cqiInfo.dlCqiPmiSize = ulSchUciInfo_p->cqiRiInfo.
            dlCqiPmiSizeRankGT_1;
    }
    else
    {
        uciCqiHarq->cqiInfo.dlCqiPmiSize = ulSchUciInfo_p->cqiRiInfo.
            riSize;
    }
    /* - CL-MIMO LJA*/    
    uciCqiHarq->rnti = MAC_PHY_CONVERT_16(ulSchUciInfo_p->rnti);
#ifdef FDD_CONFIG
    /*FAPI2.1 changes start*/
    fillUCIHarqInfoR9(&uciCqiHarq->harqInfo,
            ulSchUciInfo_p
            );
    /*FAPI2.1 changes end*/

#elif TDD_CONFIG
    FAPI_tddHarqPduInfo_st     *tddHarqInfo_p  = PNULL;
    MacSSTddHarqInfo           *dlHarqInfo_p   = PNULL;

    tddHarqInfo_p = &(uciCqiHarq->harqInfo);  
    dlHarqInfo_p  = &(ulSchUciInfo_p->dlHarqInfo);	
    fapiFillTddHarqInfo(tddHarqInfo_p, dlHarqInfo_p);

#endif
    /* + SPR_15163 */
    fillCQIRelease10Param(&(uciCqiHarq->cqiInfo));
    /* - SPR_15163 */
    ulPduConf_p->ulConfigPduSize = sizeof(FAPI_uciCqiHarqPduInfoR9_st)+
        sizeof(FAPI_ulPDUConfigInfo_st) -1;

    return;
}
#endif	
/*****************************************************************************
 * Function Name  : prepareUciCqiSrHarqPdu
 * Inputs         : uciCqiSrHarq 
 *                  ulSchUciInfo_p
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Uci Cqi Sr Harq Pdu.
 ******************************************************************************/
void prepareUciCqiSrHarqPdu(
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p
        ,InternalCellIndex internalCellIndex
        )
{

    FAPI_uciCqiSrHarqPduInfo_st *uciCqiSrHarq=(FAPI_uciCqiSrHarqPduInfo_st *)ulPduConf_p->ulPduConfigInfo;
    uciCqiSrHarq->handle = MAC_PHY_CONVERT_32(generateHandleVal(internalCellIndex));
    uciCqiSrHarq->rnti = MAC_PHY_CONVERT_16(ulSchUciInfo_p->rnti);
    uciCqiSrHarq->cqiInfo.pucchIndex = MAC_PHY_CONVERT_16(ulSchUciInfo_p->cqiRiInfo.pucchIndex);

    if (0 == ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1 && 
            0 == ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRankGT_1)
    {
        uciCqiSrHarq->cqiInfo.dlCqiPmiSize = ulSchUciInfo_p->cqiRiInfo.
            riSize;
    }
    else
    {
        if ( 1 == ulSchUciInfo_p->cqiRiInfo.rankIndicator)
        {
            uciCqiSrHarq->cqiInfo.dlCqiPmiSize = ulSchUciInfo_p->cqiRiInfo.
                dlCqiPmiSizeRank_1;
        }
        else if (1 < ulSchUciInfo_p->cqiRiInfo.rankIndicator)
        {
            uciCqiSrHarq->cqiInfo.dlCqiPmiSize = ulSchUciInfo_p->cqiRiInfo.
                dlCqiPmiSizeRankGT_1;
        }
    }
    /*FAPI2_1 changes start*/
    fillCQIRelease10Param(&uciCqiSrHarq->cqiInfo);
    /*FAPI2_1 changes end*/
    uciCqiSrHarq->srInfo.pucchIndex = ulSchUciInfo_p->srInfo.pucchIndex;
    fillSRRelease10Param(&uciCqiSrHarq->srInfo);
#ifdef FDD_CONFIG
 fillUCIHarqInfo(
		    &uciCqiSrHarq->harqInfo,
		    ulSchUciInfo_p
            );

#elif TDD_CONFIG
    FAPI_tddHarqPduInfo_st     *tddHarqInfo_p  = PNULL;
    MacSSTddHarqInfo           *dlHarqInfo_p   = PNULL;

    tddHarqInfo_p = &(uciCqiSrHarq->harqInfo);  
    dlHarqInfo_p  = &(ulSchUciInfo_p->dlHarqInfo);	
    fapiFillTddHarqInfo(tddHarqInfo_p, dlHarqInfo_p);
    /* SPR 10808 FIX START */
    /* SPR 19679: TDD HARQ Multiplexing Changes Start */ 
    /* SPR 20024 Fix Start */
#if defined(FAPI_1_1_COMPLIANCE) 
    tddHarqInfo_p->numOfPUCCHResource = 0;
#endif
    /* SPR 20024 Fix End */
    /* SPR 19679: TDD HARQ Multiplexing Changes End */ 
    /* SPR 10808 FIX END */    

#endif
    ulPduConf_p->ulConfigPduSize = sizeof(FAPI_uciCqiSrHarqPduInfo_st)+
        sizeof(FAPI_ulPDUConfigInfo_st) -1;
    return;
}
#ifdef FAPI_4_0_COMPLIANCE
/*****************************************************************************
 * Function Name  : prepareUciCqiSrHarqPduR9
 * Inputs         : uciCqiSrHarq 
 *                  ulSchUciInfo_p
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills Uci Cqi Sr Harq Pdu in case PHY 
 *                   Supports Release 9 or Later
 ******************************************************************************/
void prepareUciCqiSrHarqPduR9(
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p
        ,InternalCellIndex internalCellIndex
        )
{
    FAPI_uciCqiSrHarqPduInfoR9_st *uciCqiSrHarq=(FAPI_uciCqiSrHarqPduInfoR9_st*)ulPduConf_p->ulPduConfigInfo;
    uciCqiSrHarq->handle = MAC_PHY_CONVERT_32(generateHandleVal(internalCellIndex));
    uciCqiSrHarq->rnti = MAC_PHY_CONVERT_16(ulSchUciInfo_p->rnti);
    uciCqiSrHarq->cqiInfo.pucchIndex = MAC_PHY_CONVERT_16(ulSchUciInfo_p->cqiRiInfo.pucchIndex);

    if (0 == ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1 && 
            0 == ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRankGT_1)
    {
        uciCqiSrHarq->cqiInfo.dlCqiPmiSize = ulSchUciInfo_p->cqiRiInfo.
            riSize;
    }
    else
    {
        if ( 1 == ulSchUciInfo_p->cqiRiInfo.rankIndicator)
        {
            uciCqiSrHarq->cqiInfo.dlCqiPmiSize = ulSchUciInfo_p->cqiRiInfo.
                dlCqiPmiSizeRank_1;
        }
        else if (1 < ulSchUciInfo_p->cqiRiInfo.rankIndicator)
        {
            uciCqiSrHarq->cqiInfo.dlCqiPmiSize = ulSchUciInfo_p->cqiRiInfo.
                dlCqiPmiSizeRankGT_1;
        }
    }
    /*FAPI2_1 changes start*/
    fillCQIRelease10Param(&uciCqiSrHarq->cqiInfo);
    /*FAPI2_1 changes end*/
    uciCqiSrHarq->srInfo.pucchIndex = ulSchUciInfo_p->srInfo.pucchIndex;
    fillSRRelease10Param(&uciCqiSrHarq->srInfo);
#ifdef FDD_CONFIG
    fillUCIHarqInfoR9(&uciCqiSrHarq->harqInfo, ulSchUciInfo_p);
#elif TDD_CONFIG
    FAPI_tddHarqPduInfo_st     *tddHarqInfo_p  = PNULL;
    MacSSTddHarqInfo           *dlHarqInfo_p   = PNULL;

    tddHarqInfo_p = &(uciCqiSrHarq->harqInfo);  
    dlHarqInfo_p  = &(ulSchUciInfo_p->dlHarqInfo);	
    fapiFillTddHarqInfo(tddHarqInfo_p, dlHarqInfo_p);
#endif
    ulPduConf_p->ulConfigPduSize = sizeof(FAPI_uciCqiSrHarqPduInfoR9_st)+
        sizeof(FAPI_ulPDUConfigInfo_st) -1;

    return;
}
#endif	
/*****************************************************************************
 * Function Name  : prepareSrsPdu
 * Inputs         : srsPdu_p - pointer to structure FAPI_srsPduInfo_st and
 *                  ulSrsInfo_p - pointer to ULSRSPduInfo
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills  SRS  pdu.
 ******************************************************************************/

void prepareSrsPdu(FAPI_srsPduInfo_st * srsPdu_p,ULSRSPduInfo *ulSrsInfo_p
        ,InternalCellIndex internalCellIndex)
{

    SRSDedicatedSetupInfo *srsConfigInfo_p = PNULL;

    srsConfigInfo_p = ulSrsInfo_p->srsDedicatedSetupInfo_p;

    srsPdu_p->handle = MAC_PHY_CONVERT_32(generateHandleVal(internalCellIndex)); 
    srsPdu_p->size   = MAC_PHY_CONVERT_16(sizeof(FAPI_srsPduInfo_st));
    srsPdu_p->rnti   = MAC_PHY_CONVERT_16(ulSrsInfo_p->rnti);
    srsPdu_p->srsBandWidth           = srsConfigInfo_p->srsBandwidth;
    srsPdu_p->freqDomainPosition     = srsConfigInfo_p->freqDomainPos;
    srsPdu_p->srsHoppingBandWidth    = srsConfigInfo_p->srsHoppingBandwidth;
    srsPdu_p->transmissionComb       = srsConfigInfo_p->transmissionComb;
    srsPdu_p->isrsSRSConfigIndex     = MAC_PHY_CONVERT_16(srsConfigInfo_p->srsConfigIndex);
    srsPdu_p->soundingRefCyclicShift = srsConfigInfo_p->cyclicShift;
#ifdef FAPI_4_0_COMPLIANCE
    srsPdu_p->release10Param.antennaPort = DEFAULT_ANTENNA_PORT;
#endif
    return;
}
#if   ((defined(FAPI_1_1_COMPLIANCE)) || (defined(FAPI_4_0_COMPLIANCE)))
/*****************************************************************************
 * Function Name  : prepareUlschHarqBufferPdu
 * Inputs         : ulschHarqBuffPdu_p - pointer to structure FAPI_ulSCHHarqBufferReleasePduInfo_st,
 *                  ulSchUciInfo_p - pointer to ULSchUciInfo,
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills  HARQ_BUFFER_RELEASE  pdu.
 ******************************************************************************/

void prepareUlschHarqBufferPdu (
        FAPI_ulSCHHarqBufferReleasePduInfo_st * ulschHarqBuffPdu_p,
        ULSchUciInfo * ulSchUciInfo_p,
        InternalCellIndex internalCellIndex
        )
{
    ulschHarqBuffPdu_p->handle = MAC_PHY_CONVERT_32(generateHandleVal(internalCellIndex));
    ulschHarqBuffPdu_p->rnti   = MAC_PHY_CONVERT_16(ulSchUciInfo_p->
                                   harqReleaseInfo.rnti);
    return;
}
#endif	
/* SPR 5346 changes start */   
/*****************************************************************************
 * Function Name  : dummyPrepareULConfigMsg
 * Inputs         :
 *                  systemFrameNum -system frame number 
 *		            subFrameNum - sub frame number
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles UL config msg.
 ******************************************************************************/
void dummyPrepareULConfigMsg (
        /* +- SPR 17777 */
        UInt32 systemFrameNum, 
        UInt8 subFrameNum,
        InternalCellIndex internalCellIndex
        )
{
    FAPI_l1ApiMsg_st *pL1ConfigReq_p = PNULL;
    UInt16 sendBuffLen = 0;
    
    /* + Coverity_Stack Fix */
    UInt8             *msgBuff = PNULL;
    GET_MEM_FROM_POOL(UInt8,msgBuff,MAX_ULCONF_BUFF_SIZE, PNULL);
    /* - Coverity_Stack Fix */
    /*+COVERITY 5.3.0 - 32099*/
    if ( PNULL == msgBuff )
    {
        return;    
    }
    /*-COVERITY 5.3.0 - 32099*/

    /* SPR 11109 16-JUN-2014 FIX start */
    memSet ( msgBuff, 0x00, MAX_ULCONF_BUFF_SIZE );
    /* SPR 11109 16-JUN-2014 FIX end */

    FAPI_ulConfigRequest_st * ulConfReq_p   = PNULL;
    UInt32  newSfn = systemFrameNum;
    FAPI_ulConfigRequest_st tempUlPdu;
    UInt8  newSf = 0;

    pL1ConfigReq_p = (FAPI_l1ApiMsg_st *)msgBuff;
    pL1ConfigReq_p->msgId = PHY_UL_CONFIG_REQUEST;
    /*Length will be updated after calculating it runtime */
    pL1ConfigReq_p->msgLen = 0;
    ulConfReq_p = (FAPI_ulConfigRequest_st *)pL1ConfigReq_p->msgBody;

    /*Length will be updated after calculating it runtime */
    ulConfReq_p->ulConfigLen = 0;

    sendBuffLen = ((uintptr_t)&(tempUlPdu.ulPduConfigInfo[0]) - (uintptr_t)&tempUlPdu);
    sendBuffLen += L1_API_HEADER_LEN;

    pL1ConfigReq_p = (FAPI_l1ApiMsg_st *)msgBuff;
    pL1ConfigReq_p->msgId = PHY_UL_CONFIG_REQUEST;
    pL1ConfigReq_p->msgLen = MAC_PHY_CONVERT_16(((uintptr_t)&(tempUlPdu.ulPduConfigInfo[0]) 
                                                            - (uintptr_t)&tempUlPdu));

    ulConfReq_p = (FAPI_ulConfigRequest_st *)pL1ConfigReq_p->msgBody;

    /* Calculating current SFN/SF for UL config  */
    /* CA Changes start */
    newSf = subFrameNum + macParams_g.ulConfigAdvanceSubFrames;
    /* CA Changes end */
    if (newSf >= MAX_SUB_FRAME)
    {
        newSfn = systemFrameNum + 1;

        newSf = newSf%MAX_SUB_FRAME;

        if ( newSfn == MAX_SFN_VALUE )
        {
            newSfn = 0;
        }
    }
    else
    {
        newSfn = systemFrameNum ;
    }
/* SPR 19288 change start */
#ifdef FLEXRAN
    g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_UL_CFG_REQ; 
#endif
    ulConfReq_p->ulConfigLen = MAC_PHY_CONVERT_16(pL1ConfigReq_p->msgLen);
    ulConfReq_p->sfnsf = GENERATE_SUBFRAME_SFNSF(newSfn,newSf);

/* SPR 19288 change end */
    ulConfReq_p->numOfPdu = 0;
    ulConfReq_p->rachFreqResources = 0;
    ulConfReq_p->srsPresent = 0;
    if(0 >= sendDataToPhysical (msgBuff, sendBuffLen, internalCellIndex))
    {
        LOG_MAC_MSG(L2_SOCKET_SENDTO_FAIL_ID,LOGERROR,L2_SYS_FAIL,\
                getCurrentTick(),\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);
    }
    /* ++coverity--530-25216*/
    freeMemPool(msgBuff);
    /* - Coverity_Stack Fix */
}
/* SPR 5346 changes end */ 
/*coverity 10167*/
/*****************************************************************************
 * Function Name  : getBuffer 
 * Inputs         : ulConfReq_p
 * Outputs        : None
 * Returns        : ulPduConfigInfo Pointer
 * Description    : This function is used to get ulPduConfigInfo from 
                    UlConfigReq.
 ******************************************************************************/

 UInt8* getBuffer(FAPI_ulConfigRequest_st * ulConfReq_p)
{
    return (UInt8 *)(ulConfReq_p->ulPduConfigInfo);
}

/*coverity 10167*/
/*****************************************************************************
 * Function Name  : prepareULConfigMsg
 * Inputs         : newsf - ulSubframe for which ulconfig needs to be prepared,
 *		            phyDelay - Delay in PHY
 *                  currentTick - curent tick
 *                  systemFrameNum -system frame number and
 *                  subFrameNum - sub frame number
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles UL config msg.
 ******************************************************************************/
void prepareULConfigMsg (
#ifdef TDD_CONFIG
        UInt8  newSf, 
#else
        UInt32 phyDelay,
#endif
        /* SPR 15909 fix start */
        tickType_t currentTick,
        /* SPR 15909 fix end */
        UInt32 systemFrameNum, 
        UInt8 subFrameNum,
        InternalCellIndex internalCellIndex
        )
{
    FAPI_l1ApiMsg_st *pL1ConfigReq_p = PNULL;
    UInt16            pduSize        = 0;
    UInt8             countInvalid = 0;
    UInt8 indexForQ = 0; 
	UInt16            prepduSize        = 0;
    /* SPR 609 changes start 
     * ulConfigSendBuff_p,sendBuffLen are used inside FDD_CONFIG
     * */
#ifdef FDD_CONFIG    
    FAPI_l1ApiMsg_st * ulConfigSendBuff_p = PNULL;
    
    /* SPR 609 changes end */   
    UInt8 isMemFreeValid = LTE_FALSE;
    UInt16 sendBuffLen = 0;
    UInt8  newSf = 0;
#endif

    FAPI_ulConfigRequest_st * ulConfReq_p   = PNULL;
    
    FAPI_ulPDUConfigInfo_st * ulPduConf_p   = PNULL;
    UInt8  *tmpBuff = NULL;
    UInt16  qInsertSize = 0;
	 /*SPR 1254 Start*/
    FAPI_ulConfigRequest_st tempUlPdu;
    /*SPR 1254 End*/
	UInt32  newSfn = systemFrameNum;
    UInt8  count = 0;
    ULSchUciRachPoolInfo * ulSchUciPoolInfo_p = NULL;
    UInt32 containerTick = 0;
    ULSchUciInfo   * ulSchUciInfo_p = NULL;
    /*SPR 1115 CHG*/
    ULSRSPduInfo *ulSrsInfo_p = PNULL;
/* SPR 19288 change start */
/* SPR 19288 change end */

	/* SPR 11457 03062014 */
	RNTIInfo * rntiInfo_p = NULL;
	DLUEContext *dlUECtx_p = NULL;  
	/* SPR 11457 03062014 */
    /* index to store the ulCofig info for Error Ind handling*/
#ifdef FDD_CONFIG
    UInt8 idx = ( currentTick + phyDelay + FDD_DATA_OFFSET ) %
            MAX_UL_CONFIG_CONTAINER_MAP_SIZE ;
#elif TDD_CONFIG
    UInt8 idx = ( currentTick - PHY_ERROR_IND_DELAY_UL ) % 
        MAX_UL_CONFIG_CONTAINER_MAP_SIZE ;
    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
    ULUEContext *ulUECtx_p = NULL;
    UInt8 i = 0,  M  = 0;
    UInt8  k =0;
	SInt32 signedOffset = 0; 
	UInt32 dlIndex = 0; 
    /* SPR 19679: TDD HARQ Multiplexing Changes End */
#endif

    UInt8 infoIdx = 0;


/* Review comment fix start LJ8 */
#ifndef TDD_CONFIG
    /* Search UlConfigReqQueue for current sfn and send to phy */
    getNodeFromUlConfigReqQueue (
            subFrameNum, 
            &ulConfigSendBuff_p, 
            &sendBuffLen,
            internalCellIndex
            );
    if (sendBuffLen)
    {
    }
    /* SPR 1070 Fix start*/
    else
    {
        /*SPR 1254 Start*/
        sendBuffLen = ((uintptr_t)&(tempUlPdu.ulPduConfigInfo[0]) - (uintptr_t)&tempUlPdu);
        sendBuffLen += L1_API_HEADER_LEN;

        /*SPR 1254 End*/
        GET_MEM_FROM_POOL(FAPI_l1ApiMsg_st,ulConfigSendBuff_p,sendBuffLen, NULL);
        
        /* + coverity 25167 */
        if (PNULL == ulConfigSendBuff_p)
        {
            return;
        }
        /* - coverity 25167 */
        /* Purify UMR Fix Start */
        memSet(ulConfigSendBuff_p, 0, sendBuffLen);
        /* Purify UMR Fix End*/

        pL1ConfigReq_p = (FAPI_l1ApiMsg_st *)ulConfigSendBuff_p;

        pL1ConfigReq_p->msgId = PHY_UL_CONFIG_REQUEST;

        /*SPR 1254 Start*/
        pL1ConfigReq_p->msgLen = MAC_PHY_CONVERT_16(((uintptr_t)&(tempUlPdu.ulPduConfigInfo[0]) - (uintptr_t)&tempUlPdu));
        /*SPR 1254 End*/
        ulConfReq_p = (FAPI_ulConfigRequest_st *)pL1ConfigReq_p->msgBody;
/* SPR 19288 change start */
/*SPR 21001 Changes start */
/*SPR 21001 Changes end */
/* SPR 19288 change end */

        /* Calculating current SFN/SF for UL config  */

        /* SPR 2175 Changes Begin */
        newSf = subFrameNum + macParams_g.ulConfigAdvanceSubFrames;
        /* SPR 2175 Changes End */
        if (newSf >= MAX_SUB_FRAME)
        {
            newSfn = systemFrameNum + 1;

            newSf = newSf%MAX_SUB_FRAME;

            if ( newSfn == MAX_SFN_VALUE )
            {
                newSfn = 0;
            }
        }
        else
        {
            newSfn = systemFrameNum ;
        }
/* SPR 19288 change start */
        ulConfReq_p->sfnsf = GENERATE_SUBFRAME_SFNSF(newSfn,newSf);
/* SPR 19288 change end */
        /*SPR 1254 Start*/
        ulConfReq_p->ulConfigLen = MAC_PHY_CONVERT_16(pL1ConfigReq_p->msgLen);
        /*SPR 1254 End*/
        ulConfReq_p->numOfPdu = 0;
        /*SPR 1115 CHG*/
        ulConfReq_p->rachFreqResources = 0;
        ulConfReq_p->srsPresent = 0;

        /* + Coverity 32681 */
        isMemFreeValid = LTE_TRUE;
        /* - Coverity 32681 */
        LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,
                "Empty UL config sent to PHY of size [%d]\n", sendBuffLen);
    }
    /* ++coverity--530-25158*/
/* SPR 19288 change start */
#ifdef FLEXRAN
    g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_UL_CFG_REQ; 
#endif
/* SPR 19288 change end */
    if(0 >= sendDataToPhysical ((UInt8 *)ulConfigSendBuff_p, sendBuffLen,internalCellIndex))
    {
        LOG_MAC_MSG(L2_SOCKET_SENDTO_FAIL_ID,LOGERROR,L2_SYS_FAIL,\
                getCurrentTick(),\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);
    }
    /* ++coverity--530-25158*/

    LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,
            "UL config sent to PHY of size [%d]\n", sendBuffLen);
#ifndef LAYER2_PHYSIM_THREAD
    if( LTE_TRUE == isMemFreeValid )
    {
        freeMemPool(ulConfigSendBuff_p);
        ulConfigSendBuff_p = PNULL;
    }
    /* - Coverity 32681 */
#endif
#endif	
/* Review comment fix end LJ8 */

    LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,"====UL Config MSG ====\n");
    /* msg3 ulconfig fix start */

#ifdef TDD_CONFIG
/* TDD Config 0 Changes Start */
    if (newSf < subFrameNum)
    {
        newSfn = systemFrameNum + 1;
        if ( newSfn == MAX_SFN_VALUE )
        {
            newSfn = 0;
        }
    }
    containerTick = newSf % MAX_ULSCH_UCI_CONTAINER ;
/* TDD Config 0 Changes End */

    /* Calling function to process all the nodes that are needed to add in
     * current UL CONFIG message from ulschUciInfoForULConfigForRach_g.
     */
/* TDD Config 0 Changes Start */
        processULConfigForRachQueue(
                /* TDD Config 0 Changes Start
                 * ulSubframe and ulSysFramenum for which 
                 * ulconfig needs to be prepared will be passed now
                 * container index will be calculated from 
                 * ulSubframeNum
                 */

                newSf,
                newSfn,
                internalCellIndex);
        /* TDD Config 0 Changes End */
    /* msg3 ulconfig fix end */
#elif FDD_CONFIG
    containerTick = (currentTick + phyDelay) % MAX_PDCCH_CONTAINER ;
#endif

    /*CA Changes start  */
    ulSchUciPoolInfo_p = (ulSchUciRachContainer_gp[internalCellIndex] + containerTick); 
    /*CA Changes end  */

#ifdef FDD_CONFIG
    /*PHY will listen to UL config after 4 ticks so update the sfn */
    newSf = subFrameNum + phyDelay + 4 ;
    if (newSf >= MAX_SUB_FRAME)
    {
        newSfn = systemFrameNum + 1;

        newSf = newSf%MAX_SUB_FRAME;

        if ( newSfn == MAX_SFN_VALUE )
        {
            newSfn = 0;
        }
    }
    else
    {
        newSfn = systemFrameNum ;
    }
#endif

#ifdef TDD_CONFIG
    /* SPR 5520 START*/
    /* CA Changes start */
    indexForQ = (newSf - macParams_g.ulConfigAdvanceSubFrames + MAX_SUB_FRAME) % MAX_SUB_FRAME;
    /* CA Changes end */
    /* SPR 5520 END*/
#else
    indexForQ = (subFrameNum + phyDelay + (4 - macParams_g.ulConfigAdvanceSubFrames)  ) % MAX_SUB_FRAME;
#endif

    pL1ConfigReq_p = (FAPI_l1ApiMsg_st *)pULConfigMsg_gp[internalCellIndex][indexForQ];
 
    pL1ConfigReq_p->msgId = PHY_UL_CONFIG_REQUEST;
    /*Length will be updated after calculating it runtime */
    pL1ConfigReq_p->msgLen = 0;
    ulConfReq_p = (FAPI_ulConfigRequest_st *)pL1ConfigReq_p->msgBody;
    ulConfReq_p->sfnsf = GENERATE_SUBFRAME_SFNSF(newSfn,newSf);
    LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,
            "systemFrameNum = [%u], subFrameNum = [%u] \n",
            newSfn, newSf);
    LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,"SFN/SF in UL config is [%u] \n", 
            ulConfReq_p->sfnsf);
    /*Length will be updated after calculating it runtime */
    ulConfReq_p->ulConfigLen = 0;
    /* CSR 152181 changes +-*/
	ulConfReq_p->numOfPdu = 0;
    /* CSR 152181 changes -+*/
    /*SPR 1115 CHG*/
#if defined(FLEXRAN)
    if ( 1 == ulSchUciPoolInfo_p->isPrachResourceReserved)
    {
        ulConfReq_p->rachFreqResources = 32; //ulSchUciPoolInfo_p->isPrachResourceReserved;
    }
    else {
        ulConfReq_p->rachFreqResources = 0;
    }
#else
    ulConfReq_p->rachFreqResources = ulSchUciPoolInfo_p->isPrachResourceReserved;
#endif  
    ulConfReq_p->srsPresent = ulSchUciPoolInfo_p->isSRSPresent;


#ifdef PERF_STATS
    /*EVENT: EVENT_PARAM_RA_UNASSIGNED_CFRA_SUM , The total number of unassigned CFRA preambles 
     * at each PRACH occasion during the reporting period.*/
    if (1 == ulSchUciPoolInfo_p->isPrachResourceReserved)
    {
		/* + PERF_CA */
        gMacCellPerfStats_p[internalCellIndex]->lteCellMacULPerfStats.totalUnassignedSumCFRA += 
            contentionFreeRACHPool_gp[internalCellIndex]->currNumOfFreePreambles;
		/* + PERF_CA */
    }
  #endif

    /* SPR 609 changes start */    
    /*coverity 10167*/
      tmpBuff = getBuffer(ulConfReq_p);
    /*coverity 10167*/
/* SPR 19288 change start */
/* SPR 19288 change end */
    for (; count < ulSchUciPoolInfo_p->numberOfInfoPresent; count++)
        /* SPR 609 changes end */    
    {
        ulSchUciInfo_p = &(ulSchUciPoolInfo_p->ulschUciInfo[count]);
        /* SPR 609 changes start */    
        ulPduConf_p = (FAPI_ulPDUConfigInfo_st *)(tmpBuff + pduSize);
        /* SPR 609 changes end */    
        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
#ifdef TDD_CONFIG
        rntiInfo_p = getUeIdxFromRNTIMap(ulSchUciInfo_p->rnti, internalCellIndex);
        if(rntiInfo_p && (rntiInfo_p->rntiFlag != FREERNTI))
        {     
            ulUECtx_p = ulUECtxInfoArr_g[rntiInfo_p->index].ulUEContext_p;
        }     
        if ((ulUECtx_p) && ((HARQ_MULTIPLEXING ==  ulUECtx_p->tddAckNackFeedbackMode)
             ) )
        {    
            M = globalDlAssociationSetForHarqMultiplexing_gp[internalCellIndex][newSf].count;
            if(PNULL != (ulSchUciInfo_p->ueScheduledInfo_p))
            {   
                for (i = M; i > 0; i--)
                {
                    k = globalDlAssociationSetForHarqMultiplexing_gp[internalCellIndex][newSf].kwithSmallM[i - 1].k;
                    signedOffset = (newSf - k);
                    signedOffset = signedOffset % MAX_SUB_FRAME;
                    dlIndex = (signedOffset) < 0?(signedOffset + MAX_SUB_FRAME):(signedOffset); 
                    ulSchUciInfo_p->ueScheduledInfo_p->dlCceIndexMuxPusch[dlIndex] = INVALID_CCE_INDEX;
                    ulSchUciInfo_p->ueScheduledInfo_p->harqSizeForMuxMOneInMIMO[dlIndex] = 0;
                }
            }
        }
#endif
        /* CSR 152181 changes +-*/
        if((CRNTI== rntiInfo_p->rntiFlag ) && (ulUECtxInfoArr_g[rntiInfo_p->index].pendingDeleteFlag || ulUECtx_p == PNULL))
        {
          fprintf(stderr,"Dropping msg  of rnti %d \n",rntiInfo_p->index);
          continue;
        }
        /* CSR 152181 changes -+*/

		prepduSize=pduSize;
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
        /*Cyclomatic Complexity changes - starts here */
        pduSize = prepareUlConfigPdu(ulPduConf_p,ulSchUciInfo_p,
                pduSize,ulConfReq_p->srsPresent,&countInvalid,
                internalCellIndex);
        /*Cyclomatic Complexity changes - ends here */
        /* + SPR_15091 */
        if (pduSize  == prepduSize)
        {
           /*CSR 152109 debug +-*/
		   //fprintf(stderr,"UL-Config dropped at %llu pduType %d rnti %d cell %d \n",getCurrentTick(),ulSchUciInfo_p->pduType,ulSchUciInfo_p->rnti,
		   //internalCellIndex);
		   /*CSR 152109 debug -+*/
           continue;
        } 
		/* CSR 152181 changes +-*/
		else
		{
		   ulConfReq_p->numOfPdu++;
		}
        /* CSR 152181 changes -+*/
        /* - SPR_15091 */
        /* store the ulConfig Information in map */
        if (ulConfigMap_g[internalCellIndex][idx].tick != currentTick)
        {
            ulConfigMap_g[internalCellIndex][idx].numUe = 0;
        }
        infoIdx = ulConfigMap_g[internalCellIndex][idx].numUe;
		/*CSR 152109 debug +-*/
		ulConfigMap_g[internalCellIndex][idx].totalCount = ulConfReq_p->numOfPdu;
		/*CSR 152109 debug -+*/
		
        ulConfigMap_g[internalCellIndex][idx].tick = currentTick;
        ulConfigMap_g[internalCellIndex][idx].ulConfigInfo[infoIdx].rnti =
            ulSchUciInfo_p->rnti;
        ulConfigMap_g[internalCellIndex][idx].ulConfigInfo[infoIdx].pduType =
            ulSchUciInfo_p->pduType;
        ulConfigMap_g[internalCellIndex][idx].ulConfigInfo[infoIdx].ulHarqProcessId =
            ulSchUciInfo_p->ulHarqProcessId;
        ulConfigMap_g[internalCellIndex][idx].numUe++;
    }


    // decrement the invalid PDU count
    /* CSR 152181 changes +-*/
    /*code removed*/
	/* CSR 152181 changes -+*/

	/* SPR 11457 03062014 */
	countInvalid = 0;
    for(count = 0 ; count < ulSchUciPoolInfo_p->numSRSPdus ; count++)
    {
        ulSrsInfo_p =&(ulSchUciPoolInfo_p->srsPduInfo[count]);
        /* +SPR 12564 */
        rntiInfo_p = getUeIdxFromRNTIMap(ulSrsInfo_p->rnti, internalCellIndex);
        /* -SPR 12564 */
        /*Coverity ID <65222> Fix Changes Start*/
        if(rntiInfo_p && (rntiInfo_p->rntiFlag != FREERNTI))
        {
            dlUECtx_p = dlUECtxInfoArr_g[rntiInfo_p->index].dlUEContext_p;
        }
        else
        {
            continue;
        }
        /*Coverity ID <65222> Fix Changes End*/
        if(1 == dlUECtx_p->tcrntiFlag)
        {
            countInvalid++;
        }
        else
        {
            /* SPR 609 changes start */    
            ulPduConf_p = (FAPI_ulPDUConfigInfo_st *)(tmpBuff + pduSize);
            /* SPR 609 changes end */    
            prepareSrsPdu(
                    (FAPI_srsPduInfo_st *)ulPduConf_p->ulPduConfigInfo,
                    ulSrsInfo_p, internalCellIndex
                    );
            ulPduConf_p->ulConfigPduSize = sizeof(FAPI_srsPduInfo_st)+
                sizeof(FAPI_ulPDUConfigInfo_st) -1;
            pduSize += ulPduConf_p->ulConfigPduSize;
            ulPduConf_p->ulConfigPduType = FAPI_SRS;
        }
        /* SPR 11457 03062014 */
    }
	/* SPR 11457 03062014 */
	ulSchUciPoolInfo_p->numSRSPdus -= countInvalid;
	
	/*SPR 14328 Fix Start*/
	/*
 	 *Code Removed.	
	*/	
	/*SPR 14328 Fix End*/
	
	/* SPR 11457 03062014 */
    ulConfReq_p->numOfPdu += ulSchUciPoolInfo_p->numSRSPdus;
    ulSchUciPoolInfo_p->numSRSPdus = 0; 
    /* Reset the number of info present */
    ulSchUciPoolInfo_p->numberOfInfoPresent = 0;
    /*SPR 1115 CHG*/
    ulSchUciPoolInfo_p->isPrachResourceReserved = 0;
    ulSchUciPoolInfo_p->prachFreqOffset = 0;
    /*SPR 1254 Fix Start*/

    pL1ConfigReq_p->msgLen =  MAC_PHY_CONVERT_16(((uintptr_t)&(tempUlPdu.ulPduConfigInfo[0]) - (uintptr_t)&tempUlPdu) + pduSize);
    ulConfReq_p->ulConfigLen = MAC_PHY_CONVERT_16(pL1ConfigReq_p->msgLen);

    qInsertSize = L1_API_HEADER_LEN + 
        ((uintptr_t)&(tempUlPdu.ulPduConfigInfo[0]) - (uintptr_t)&tempUlPdu) +
/* SPR 19288 change start */
/* SPR 19288 change end */

        pduSize;
    /* SPR 609 changes start */    
    insertNodeInUlConfigReqQueue (indexForQ ,pL1ConfigReq_p,qInsertSize,internalCellIndex);
    /* SPR 609 changes end */    

    memset(ulSchUciPoolInfo_p, 0, sizeof(ULSchUciRachPoolInfo));
}

#ifdef TDD_CONFIG
#if   INTEL_NOCA_L1
/* SPR 22569 Fix - */




void prepareDummyTxDlControlMsg(UInt8 subFrameNum,UInt16 sysFrameNum,InternalCellIndex internalCellIndex)
{
    /* SPR# 19970 start */
	UInt32  length = 0;

	PMAC2PHY_QUEUE_EL pMac2PhyQE;
	UInt8 numOfDlChnlDesc = 0;
	UInt8 numOfDciChnlDesc = 0;
	GENMSGDESC  *pHeader;
	DLSUBFRDESC *pDlSubfrDesc;
	DLCMNTXPWRCTL *pDlCmnTxPowerCtl;

	pMac2PhyQE = (PMAC2PHY_QUEUE_EL)MspdMsgAllocBuffer(internalCellIndex);
	if( pMac2PhyQE == PNULL)
	{
	   return;
	}

	pHeader = (PGENMSGDESC)MspdMsgAllocBuffer(internalCellIndex);
	if( pHeader == PNULL)
	{
	   MspdMsgFreeBuffer(pMac2PhyQE,internalCellIndex);
	   return;
	}

	length = MSPD_API_HEADER_LEN;

	/* Get the pointer to Fill in API parameters */
	pDlSubfrDesc = (DLSUBFRDESC *)((UInt8 *)pHeader + MSPD_API_HEADER_LEN);
	/*
	 ** Length has been incremented for Fixed Control message and
	 ** Common control structure to fill DL Chnl Descriptor and
	 ** these strcutres is being filled in the last
	 */

	length += MINDSPEED_FIXED_SIZE_OF_DL_CONTROL_MSG;  //gourav

	/*changes  for power control*/
	pDlSubfrDesc->offsetPowerCtrl = length - MSPD_API_HEADER_LEN;
	pDlCmnTxPowerCtl = (DLCMNTXPWRCTL *)((UInt8 *)pHeader + length);
	pDlCmnTxPowerCtl->pilotPower = 0;

	/* Taking the changes from PHY INIT */
	pDlCmnTxPowerCtl->psyncPower =0; //Suggested by Louis
	pDlCmnTxPowerCtl->ssyncPower = 0; //Suggested by Louis
	pDlCmnTxPowerCtl->ciPower = 0;

	/* Dont Care about PAPR params */
#ifdef PHY_172
	pDlCmnTxPowerCtl->paprControl = 0;
	pDlCmnTxPowerCtl->paprThreshold = 0;
#else
	// For PRS support
	pDlCmnTxPowerCtl->prsRa = 0;
	pDlCmnTxPowerCtl->reserved = 0;
#endif

	length+=MINDSPEED_SIZE_OF_DLCOMMON_TX_POWER_CTL;
	pHeader->msgType = PHY_TXSTART_REQ;
	pHeader->phyEntityId = internalCellIndex;
	pHeader->msgSpecific = (length - MSPD_API_HEADER_LEN);
	pDlSubfrDesc->frameNumber =  sysFrameNum;
	pDlSubfrDesc->subframeNumber = subFrameNum;
	pDlSubfrDesc->subframeType = DLTX;
	pDlSubfrDesc->antennaPortcount = cellSpecificParams_g.cellConfigAndInitParams_p\
					 [internalCellIndex]->cellParams_p->numOfTxAnteenas;
	pDlSubfrDesc->numberofChannelDescriptors = (numOfDciChnlDesc + numOfDlChnlDesc);
	pDlSubfrDesc->offsetCustomFeatures = 0;
	pDlSubfrDesc->offsetDCIChannels = 0;
	pDlSubfrDesc->numCtrlSymbols = 0;
	pDlSubfrDesc->phichResource = 0;
	pDlSubfrDesc->phichDuration = cellSpecificParams_g.cellConfigAndInitParams_p\
				      [internalCellIndex]->cellParams_p->phichDuration;
	pDlSubfrDesc->numberOfPhichChannels = 0;
	pDlSubfrDesc->numCtrlChannels = numOfDciChnlDesc;
	pMac2PhyQE->frameNumber = sysFrameNum;
	pMac2PhyQE->subframeNumber = subFrameNum;
	pMac2PhyQE->MessageType = PHY_TXSTART_REQ;
	pMac2PhyQE->MessageLen = length;
	pMac2PhyQE->MessagePtr = (UInt8 *)MspdIcpuGetPhys(pHeader,internalCellIndex);
    pMac2PhyQE->Next = PNULL;
    pTxVector[internalCellIndex] = pMac2PhyQE;
	pPrevIcpu[internalCellIndex] = (MAC2PHY_QUEUE_EL *)pMac2PhyQE;
	pMac2PhyQE->Next                = NULL;
    pPrevIcpu[internalCellIndex]->Next                = NULL;
	return ;
/* SPR# 19970 end */
}


/*****************************************************************************
 * Function Name  : prepareDummytxULConfigMsg
 * Inputs         : systemFrameNum
 *                  subFrameNum
 * Outputs        : None
 * Returns        : None
 * Description    : This function prepares dummy UL config msg specifically this 
 *                  func created for MSPD tdd implentation purpose.
 ******************************************************************************/
void prepareDummytxULConfigMsg (
        UInt32 currentTick,
        UInt32 systemFrameNum,
        UInt8 subFrameNum,InternalCellIndex internalCellIndex
        )
{

	GENMSGDESC      *pL1ConfigReq_p1 = PNULL;
	ULSUBFRDESC     *ulConfReq_p1   = PNULL;

	PTR pNew3 = (PTR)MspdMsgAllocBuffer(internalCellIndex);
    PTR pNew4 = (PTR)MspdMsgAllocBuffer(internalCellIndex);

   if (pNew3 == NULL || pNew4 == NULL)
   {
        	ltePanic("ICPU Null buffer received\n");
        	return;
   }
#ifdef INTEL_API_LOG_OFF   
    pRxVector[internalCellIndex] = (MAC2PHY_QUEUE_EL *)pNew3;
#else
    pPrevIcpu[internalCellIndex]->Next = (MAC2PHY_QUEUE_EL *)MspdIcpuGetPhys(pNew3,internalCellIndex);
#endif
   MAC2PHY_QUEUE_EL  *pMac2PhyQE1 = (MAC2PHY_QUEUE_EL *)pNew3;

	pMac2PhyQE1->frameNumber = systemFrameNum;
	pMac2PhyQE1->subframeNumber = subFrameNum;
	pMac2PhyQE1->MessageType = PHY_RXSTART_REQ;
	pMac2PhyQE1->MessageLen = MAX_RXVECTOR_BUFF_SIZE;
	pMac2PhyQE1->MessagePtr = (UInt8 *)MspdIcpuGetPhys(pNew4,internalCellIndex);
	pMac2PhyQE1->Next = NULL;

	/* Populating the Generic Information */
	pL1ConfigReq_p1 = (GENMSGDESC *)pNew4;
	pL1ConfigReq_p1->msgType = PHY_RXSTART_REQ;
	pL1ConfigReq_p1->phyEntityId = internalCellIndex;
	/*Length will be updated after calculating it runtime */
	pL1ConfigReq_p1->msgSpecific = MAX_RXVECTOR_STRUCT;

	ulConfReq_p1 = (ULSUBFRDESC *)((UInt8 *)pL1ConfigReq_p1 + MSPD_API_HEADER_LEN);

	/* Populating the RXVECTOR Information */
	ulConfReq_p1->frameNumber = systemFrameNum;
	ulConfReq_p1->subframeNumber = subFrameNum;
	ulConfReq_p1->subframeType = ULRX;
	ulConfReq_p1->antennaPortcount = 1;
	ulConfReq_p1->numberofChannelDescriptors = 0;
	ulConfReq_p1->numberOfCtrlChannelDescriptors = 0;
	ulConfReq_p1->numberSrsinSf = 0;
         ulConfReq_p1->offsetULCtrlChannels =
                  (MAX_RXVECTOR_STRUCT -
                   ((SIZE_OF_SRS_DED_STRUCT * MAXCHSUBFRAME_PUSCH) + SIZE_OF_RACH_CTRL_STRUCT + ((sizeof (ULCTRLCHDESC) * MAXCHSUBFRAME_PUCCH))));
          ulConfReq_p1->offsetsrsInfo =
                  (MAX_RXVECTOR_STRUCT -
                   (SIZE_OF_RACH_CTRL_STRUCT +
                    (SIZE_OF_SRS_DED_STRUCT * MAXCHSUBFRAME_PUSCH)));
	ulConfReq_p1->offsetRachCtrlStruct =
		MAX_RXVECTOR_STRUCT - SIZE_OF_RACH_CTRL_STRUCT;
	ulConfReq_p1->offsetCustomFeatures = 0;
	/* Disabling the RACH detection for Empty RXVectors(for startin 7) */
	ulConfReq_p1->rachCtrl.prachEnable = 0;
}

/*****************************************************************************
 * Function Name  : txULConfigMsg
 * Inputs         : systemFrameNum
 *                  subFrameNum
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles UL config msg.
 ******************************************************************************/
void txULConfigMsg (
        ExecutionLegFunctionArg * execLegFuncArg_p,
        UInt32 systemFrameNum,
        UInt8 subFrameNum,
	    InternalCellIndex internalCellIndex
        )
{
	UInt8 * ulConfigSendBuff_p = PNULL;
	UInt16 sendBuffLen = 0;

	
	

	UInt32  newSfn = systemFrameNum;
	UInt8  newSf = subFrameNum;
	newSf = subFrameNum + macParams_g.ulConfigAdvanceSubFrames;
	/* CA Changes end */
	if (newSf >= MAX_SUB_FRAME)
	{
		newSfn = systemFrameNum + 1;

		newSf = newSf%MAX_SUB_FRAME;

		if ( newSfn == MAX_SFN_VALUE )
		{
			newSfn = 0;
		}
	}
	else
	{
		newSfn = systemFrameNum ;
	}

	/* SPR 11082 Fix Ends */
	if(DL != getSubframeType(newSf, internalCellIndex))
	{      
                //fprintf(stderr, "RX : %llu %d newSf %d \n",getCurrentTick(),subFrameNum,newSf);
		getNodeFromUlConfigReqQueue (
				subFrameNum,
				&ulConfigSendBuff_p,
				&sendBuffLen,
				internalCellIndex
				);

       if (sendBuffLen)
       {
#ifdef INTEL_API_LOG_OFF   
	/* + SPR 20736 */
        pRxVector[internalCellIndex] = ((MAC2PHY_QUEUE_EL *)ulConfigSendBuff_p);
	/* - SPR 20736 */
#else
            /*add this node in to linkedlist*/
	    pPrevIcpu[internalCellIndex]->Next = (MAC2PHY_QUEUE_EL *)MspdIcpuGetPhys((PTR)ulConfigSendBuff_p,internalCellIndex);
        pPrevIcpu[internalCellIndex] = (MAC2PHY_QUEUE_EL *)ulConfigSendBuff_p;
        pPrevIcpu[internalCellIndex]->Next = PNULL;
#endif		
       }
	   else
	   	{
	   	   prepareDummytxULConfigMsg (execLegFuncArg_p->advanceDLGlobalTick,
		   	                           newSfn,
                                       newSf,
                                       internalCellIndex);
	   	}
    }
	else
	{
	    /*For DL Only subframe , add dummy UL Config */
		prepareDummytxULConfigMsg (execLegFuncArg_p->advanceDLGlobalTick,
									newSfn,
									newSf,
									internalCellIndex);
	}
 Mac2PhySendListTdd(internalCellIndex);
}

#else

void txULConfigMsg (
        /* +- SPR 17777 */
        UInt32 systemFrameNum, 
        UInt8 subFrameNum,
        InternalCellIndex internalCellIndex
        )
{
    /* SPR 22569 Fix + */
#ifndef INTEL_NOCA_L1
    /* SPR 22569 Fix - */
    FAPI_l1ApiMsg_st * ulConfigSendBuff_p = PNULL;
#else
    UInt8 * ulConfigSendBuff_p = PNULL;
#endif
    UInt16 sendBuffLen = 0;
    FAPI_l1ApiMsg_st *pL1ConfigReq_p = PNULL;
    FAPI_ulConfigRequest_st *ulConfReq_p = PNULL;
    /* Search UlConfigReqQueue for current sfn and send to phy */
    UInt32  newSfn = systemFrameNum;
    UInt8  newSf = subFrameNum; 
    /*SPR 1254 Start*/
    FAPI_ulConfigRequest_st tempUlPdu;
    /* + Coverity 32697 */
    UInt8 isMemFreeValid = LTE_FALSE;
    /* - Coverity 32697 */
    /*SPR 1254 End*/
    getNodeFromUlConfigReqQueue (
            subFrameNum, 
            &ulConfigSendBuff_p, 
            &sendBuffLen,
            internalCellIndex
            );
    if (sendBuffLen)
    {
    }
    /* SPR 1070 Fix start*/
    else
    {
        /*SPR 1254 Start*/
        sendBuffLen = ((uintptr_t)&(tempUlPdu.ulPduConfigInfo[0]) - (uintptr_t)&tempUlPdu);
        sendBuffLen += L1_API_HEADER_LEN;
/* SPR 19288 change start */
/* SPR 19288 change end */

        /*SPR 1254 End*/
        /* SPR 22569 Fix + */
#ifndef INTEL_NOCA_L1
        /* SPR 22569 Fix - */
        GET_MEM_FROM_POOL(FAPI_l1ApiMsg_st,ulConfigSendBuff_p,sendBuffLen, NULL);
#else
        GET_MEM_FROM_POOL(UInt8,ulConfigSendBuff_p,sendBuffLen, NULL);
#endif
        /* + Coverity 25308 */
        if( PNULL == ulConfigSendBuff_p )
        {
            return;
        }
        memSet(ulConfigSendBuff_p,0,sendBuffLen);
        /* - Coverity 25308 */ 
        pL1ConfigReq_p = (FAPI_l1ApiMsg_st *)ulConfigSendBuff_p;

        pL1ConfigReq_p->msgId = PHY_UL_CONFIG_REQUEST;

        /*SPR 1254 Start*/
        pL1ConfigReq_p->msgLen =  MAC_PHY_CONVERT_16(((uintptr_t)&(tempUlPdu.ulPduConfigInfo[0]) - (uintptr_t)&tempUlPdu));
        /*SPR 1254 End*/

        ulConfReq_p = (FAPI_ulConfigRequest_st *)pL1ConfigReq_p->msgBody;

        /* Calculating current SFN/SF for UL config  */
        /* CA Changes start */
        newSf = subFrameNum + macParams_g.ulConfigAdvanceSubFrames;
        /* CA Changes end */
        if (newSf >= MAX_SUB_FRAME)
        {
            newSfn = systemFrameNum + 1;

            newSf = newSf%MAX_SUB_FRAME;

            if ( newSfn == MAX_SFN_VALUE )
            {
                newSfn = 0;
            }
        }
        else
        {
            newSfn = systemFrameNum ;
        }

        ulConfReq_p->sfnsf = GENERATE_SUBFRAME_SFNSF(newSfn,newSf);

        /*SPR 1254 Start*/
        ulConfReq_p->ulConfigLen = MAC_PHY_CONVERT_16(pL1ConfigReq_p->msgLen);
        /*SPR 1254 End*/
        ulConfReq_p->numOfPdu = 0;
        /*SPR 1115 CHG*/
        ulConfReq_p->rachFreqResources = 0;
        ulConfReq_p->srsPresent = 0;
        /* + Coverity 32697 */
        isMemFreeValid = LTE_TRUE;
        /* - Coverity 32697 */
        LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,
                "Empty UL config sent to PHY of size [%d]\n", sendBuffLen);

    }
#ifdef FLEXRAN
    g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_UL_CFG_REQ; 
#endif
/* SPR 19288 change start */
/* SPR 19288 change end */
    /* ++coverity--530-25158*/
    if(0 >= sendDataToPhysical ((UInt8 *)ulConfigSendBuff_p, sendBuffLen,
                internalCellIndex))
    {
        LOG_MAC_MSG(L2_SOCKET_SENDTO_FAIL_ID,LOGERROR,L2_SYS_FAIL,\
                getCurrentTick(),\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);
    }
    /* ++coverity--530-25158*/

    LTE_MAC_UT_LOG(LOG_INFO, UE_CONF_MGR,
            "UL config sent to PHY of size [%d]\n", sendBuffLen);
    /* + Coverity 32697 */
    if( LTE_TRUE == isMemFreeValid )
    {
        freeMemPool(ulConfigSendBuff_p);
        ulConfigSendBuff_p = PNULL;
    }
    /* - Coverity 32697 */
}
#endif	
#endif /* TDD_CONFIG */

/*****************************************************************************
 * Function Name  : handleUeReconfigReq
 * Inputs         : msg_p and transaction Id
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function handles UE reconfig msg.
 ******************************************************************************/
/* CA changes start */
SInt32 handleUeReconfigReq (UInt8 *msg_p,
        UInt16 transactionId, InternalCellIndex internalCellIndex)
{
    UInt32 index = RRC_API_HEADER_LEN;
    UInt16 ueIndex = 0x00;
    UInt8 msgBuf[30] = {0};
    SInt32 retval = MAC_SUCCESS;
    UInt8 *resp_p = &msgBuf[0];

    ueIndex = LTE_GET_U16BIT(msg_p);

    resp_p[index++]= 0; // fill bitmask
    resp_p[index++]= 0; // fill bitmask

    LTE_SET_U16BIT( resp_p + index, ueIndex); // fill UE index
    index += 2;
    resp_p[index++] = retval; // fill response
    preparePhyRRCHeader( resp_p, PHY_RECONFIG_UE_ENTITY_CNF, RRC_MODULE_ID,
            transactionId,index,macCommonGetRRCCellIndex(internalCellIndex));
    retval = sendMacMsgToRRC(msgBuf,index,0,
            internalCellIndex);
    if(retval>=0)
    {
        LOG_MAC_MSG(MAC_PHY_RECONFIG_UE_RES_ID,LOGINFO,MAC_RRC_INF,
                getCurrentTick(),
                PHY_RECONFIG_UE_ENTITY_CNF,ueIndex,index,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"PHY_RECONFIG_UE_RES_SENT");
    }
    else
    {
        LOG_MAC_MSG(MAC_PHY_RECONFIG_UE_RES_ID,LOGERROR,MAC_RRC_INF,
                getCurrentTick(),
                PHY_RECONFIG_UE_ENTITY_CNF,ueIndex,index,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"PHY_RECONFIG_UE_RES_FAILS");
    }   
    return retval;
}
/* CA changes end */

/*****************************************************************************
 * Function Name  : handleUeDeleteReq
 * Inputs         : msg_p and transaction Id
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function handles UE delete msg.
 ******************************************************************************/
/* CA changes start */
SInt32 handleUeDeleteReq (UInt8 *msg_p,
        UInt16 transactionId, InternalCellIndex internalCellIndex)
{
    UInt32 index = RRC_API_HEADER_LEN;
    UInt16 ueIndex = 0x00;
    UInt8 msgBuf[30] = {0};
    SInt32 retval = 0;
    UInt8 response = MAC_SUCCESS;
    UInt8 *resp_p = &msgBuf[0];

    ueIndex = LTE_GET_U16BIT(msg_p);
    LTE_SET_U16BIT( resp_p + index, ueIndex);
    index += U16BIT_LEN;
    resp_p[index++] = response;
    {
    preparePhyRRCHeader( resp_p, PHY_DELETE_UE_ENTITY_CNF, RRC_MODULE_ID,
            transactionId,index ,macCommonGetRRCCellIndex(internalCellIndex));
    }
    retval = sendMacMsgToRRC(msgBuf,index,0,
            internalCellIndex);

    if(retval <0)
    {
        LOG_MAC_MSG(MAC_PHY_UE_DEL_ID,LOGERROR,MAC_RRC_INF,
                getCurrentTick(),
                PHY_DELETE_UE_ENTITY_CNF,ueIndex,index,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"MAC_PHY_UE_DEL_RES_FAILS");
    }
    else
    {
        LOG_MAC_MSG(MAC_PHY_UE_DEL_ID,LOGINFO,MAC_RRC_INF,
                getCurrentTick(),
                PHY_DELETE_UE_ENTITY_CNF,ueIndex,index,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"MAC_PHY_UE_DEL_RES_SENT");
    }
    return retval;
}

/* CA changes end */

/*****************************************************************************
 * Function Name  : handlePhyChangeCrntiReq
 * Inputs         : msg_p and
 *		            transaction Id
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function handles Change CRNTI Req msg for a UE.
 *                  Currently there is no FAPI Intf to support this API, thus 
 *                  always sending success. Need to relook later on when support
 *                  will be added at FAPI Interface.
 ******************************************************************************/
/* CA changes start */
SInt32 handlePhyChangeCrntiReq (UInt8 *msg_p,
        UInt16 transactionId, InternalCellIndex internalCellIndex)
{
    UInt32 index = RRC_API_HEADER_LEN;
    UInt16 ueIndex = 0x00;
    /* SPR 2446 Fix Begins */
    UInt8 msgBuf[30] = {0};
    /* SPR 2446 Fix Ends */
    SInt32 retval = 0;
    UInt16 response = MAC_SUCCESS;//MULTI_SECTOR
    UInt8 *resp_p = &msgBuf[0];

    ueIndex = LTE_GET_U16BIT(msg_p);

    LTE_SET_U16BIT( resp_p + index, ueIndex); // fill UE Index
    index += U16BIT_LEN;
    LTE_SET_U16BIT( resp_p + index, response); // fill response
    index += U16BIT_LEN;
    preparePhyRRCHeader( resp_p, PHY_CHANGE_CRNTI_CNF, RRC_MODULE_ID,
            transactionId,index,macCommonGetRRCCellIndex(internalCellIndex));
    LOG_MAC_MSG(MAC_PHY_UE_DEL_ID,LOGWARNING,MAC_RRC_INF,
            getCurrentTick(),
            PHY_DELETE_UE_ENTITY_CNF,ueIndex,index,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"MAC_PHY_CHANGE_CRNTI_ID");


    retval = sendMacMsgToRRC(msgBuf,index,0,
            internalCellIndex);

    if(retval <0)
    {
        LOG_MAC_MSG(MAC_PHY_UE_DEL_ID,LOGERROR,MAC_RRC_INF,
                getCurrentTick(),
                PHY_DELETE_UE_ENTITY_CNF,ueIndex,index,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"MAC_PHY_CHG_CRNTI_RES_FAILS");
    }
    else
    {
        LOG_MAC_MSG(MAC_PHY_UE_DEL_ID,LOGINFO,MAC_RRC_INF,
                getCurrentTick(),
                PHY_DELETE_UE_ENTITY_CNF,ueIndex,index,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"MAC_PHY_CHG_CRNTI_RES_SENT");
    }

    return retval;
}
/* CA changes end */

/*****************************************************************************
 * Function Name  : reConfigPhyCell
 * Inputs         : msg_p and totalMsgLen
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is used by MAC to reconfigure PHY Cell. 
 *                  It shall send CONFIG.request to PHY and also receive the 
 *                  response.
 *****************************************************************************/
/* CA Changes start  */
MacRetType reConfigPhyCell(UInt8 *msg_p, UInt16 totalMsgLen,
        InternalCellIndex internalCellIndex)
{
    /*SPR 16855 +-*/
    FAPI_l1ApiMsg_st *L1ConfigReq_p = PNULL;
    UInt16 phyReconfigMsgLen        = 0;
    /*SPR 16855 +-*/
    UInt16 remLen = totalMsgLen;
    /* + coverity 32194 */
    MacRetType  resp = MAC_SUCCESS;
    /* - coverity 32194 */
    UInt16 sfn = 0;
    UInt8 sf = 0;
#if ((defined(LOG_PRINT_ENABLED) || defined(LOG_UT_ENABLED)) && defined(FAPI_4_0_COMPLIANCE))
    UInt8 cellIndex = 0;
#endif
    UInt16 bitMask = 0;
    SInt16 offsetArray[RECONFIG_OFFSET_NUM] = {INVALID_RECONFIG_PARAM_OFFSET};
    /* Initailize all the offsets to -1 to indicate the invalid offset */
    memSet(offsetArray, INVALID_RECONFIG_PARAM_OFFSET, sizeof(offsetArray));
    /*PHY_RECONF_CHG*/
    /* Initialize it 1 to always take care of SFN/SF TLV */
    UInt8 numOfTLV = 1;
    /*PHY_RECONF_CHG*/
    /* + PRS_CHANGES */
    UInt8 *tempMsg_p = PNULL;
    UInt8 tempPrsBandWidth = 0;
    UInt8 tempPrsCyclicPrifix = 0;
    /* reinitialize the PRS update flag before parse */
    reconfigPhyCell_g[internalCellIndex].prsPhyConfig.updateFlag = FALSE;
    /* - PRS_CHANGES */

    /* PHY_RECONFIG MSG Length Check */
    if (totalMsgLen < PHY_RECONFIG_MIN_LEN)
    {
        return MAC_FAILURE;
    }
    bitMask = LTE_GET_U16BIT(msg_p);
    msg_p += 2;
    remLen -= 2;

    /* if bitmask is zero return failure from here */
    if(0 == bitMask)
    {
        return MAC_FAILURE;
    }
#if ((defined(LOG_PRINT_ENABLED) || defined(LOG_UT_ENABLED)) && defined(FAPI_4_0_COMPLIANCE))
    cellIndex = *msg_p;
#endif
    msg_p++;
    remLen --;

    sfn = LTE_GET_U16BIT(msg_p);
    msg_p += 2;
    remLen -= 2;
    sf = *msg_p++;
    remLen --;

    /* Calculate the offsets of structures received in PHY cell 
     *  reconfig message  
     */
    /* Adding target sfn/sf to global reconfigPhyCell */
    reconfigPhyCell_g[internalCellIndex].targetSfn = sfn;
    reconfigPhyCell_g[internalCellIndex].targetSf = sf;

    if(remLen > 0)
    {
        if(MAC_FAILURE == calcReconfigOffsets(msg_p, bitMask, offsetArray)) 
        {
            return MAC_FAILURE;  
        }
        /* CLPC Change Begin*/
        /* parse updated reconfigured field referenceSignalPower */
        /* Return values will be checked for partial success.
           Partial success will return only if the new and old values are same.
           If this is the case for all the optional parameters then 
           Partial_Success will be returned. If any of the optional
           Parameters will return MAC_SUCCESS then reconfiguration 
           will be applied. As phy reconfig will be done on the 
           bases of update flag of each of the optional parameters 
           so only those will be reconfigured whose updateflag is true */
        MacRetType retValPdschConfig = 
            checkAndUpdatePDSCHConfigCommonInfo(msg_p, offsetArray ,
                    &numOfTLV,internalCellIndex);
        /*PHY_RECONF_CHG*/
        MacRetType retValPcfich = 
            checkAndUpdatePcfichPowerOffset(msg_p, offsetArray, 
                    &numOfTLV, internalCellIndex);
        MacRetType retValPhich = 
            checkAndUpdatePhichPowerOffset(msg_p, offsetArray, 
                    &numOfTLV, internalCellIndex);
        MacRetType retValSynchSignals = 
            checkAndUpdateSynchSignals(msg_p, offsetArray,
                    &numOfTLV,internalCellIndex);
        /*PHY_RECONF_CHG*/

        /* 
         * - If all the return values return partial success, it means
         that new values and old values are same for all the 
         optional parameters 
         - If all the return values are failure, it means either none 
         of the optional parameter are present or none have correct offset
         */ 
        if ((MAC_PARTIAL_SUCCESS == retValPdschConfig)
                && (MAC_PARTIAL_SUCCESS == retValPcfich)
                && (MAC_PARTIAL_SUCCESS == retValPhich)
                &&(MAC_PARTIAL_SUCCESS == retValSynchSignals))
        {
            return MAC_PARTIAL_SUCCESS;
        }
        else if ((MAC_FAILURE == retValPdschConfig)
                && (MAC_FAILURE == retValPcfich)
                && (MAC_FAILURE == retValPhich)
                && ( MAC_FAILURE == retValSynchSignals)
                && (MAC_FAILURE == offsetArray[PRS_CONFIG]))
        {
#if ((defined(LOG_PRINT_ENABLED) || defined(LOG_UT_ENABLED)) && defined(FAPI_4_0_COMPLIANCE))
            LOG_MAC_WARNING( MAC_RRC_INF, "None of the optional parameter present or having incorrect offsets for cell:%d",
                    cellIndex);
#endif
            return MAC_FAILURE;
        }
        /* CLPC Change End*/
    }
    else
    {
        return MAC_FAILURE;
    }
    /* + PRS_CHANGES */
    if(offsetArray[PRS_CONFIG] != 0)
    {
#ifdef FAPI_4_0_COMPLIANCE
        if(phyReleaseCapability_g == RELEASE_8)
        {

            LOG_MAC_MSG(PHY_CELL_CONFIG_FAIL,LOGERROR,MAC_L1_INF,
                    getCurrentTick(),bitMask, 
                    cellIndex, phyReleaseCapability_g,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    __func__,"PRS Config is not allowed  with Release 8 PHY Capability");


            return MAC_FAILURE;  
        }
#endif

        tempMsg_p = msg_p;
        tempMsg_p += offsetArray[PRS_CONFIG];
        /* SPR_7936_FIX */
        tempPrsBandWidth = *tempMsg_p++;
        tempPrsCyclicPrifix = *tempMsg_p++;
        /* SPR_7936_FIX */
        if ((tempPrsBandWidth ==
                    cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsBandWidth) &&
                (tempPrsCyclicPrifix == 
                 cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsCyclicPrifix))
        {
            LOG_MAC_WARNING( MAC_RRC_INF,"PRS is already configured with same set of values BW:%d CP:%d",
                    tempPrsBandWidth, tempPrsCyclicPrifix);
        }
        /* Atleast one of the field is updated */
        else
        {
            reconfigPhyCell_g[internalCellIndex].prsPhyConfig.updateFlag = TRUE;
            reconfigPhyCell_g[internalCellIndex].prsPhyConfig.prsBandWidth = 
                tempPrsBandWidth;
            reconfigPhyCell_g[internalCellIndex].prsPhyConfig.prsCyclicPrifix =
                tempPrsCyclicPrifix;
            /* + SPR_11067 */ 
            numOfTLV += 2;
            /* - SPR_11067 */ 
        }
        /* SPR_7936_FIX */
    }
    /* - PRS_CHANGES */
    /* Create the PHY Cell Reconfiguration message as per FF MAC-PHY interface */
    /* SPR 3224 START */
    /* if num of TLV remain same with which it was initialized then no need to
     * send UPDATE to PHY just send success response */
    if (numOfTLV != 1)
    {

        /*SPR 16855 +-*/
        if (MAC_FAILURE == fillPhyReConfigRequest (&L1ConfigReq_p, &phyReconfigMsgLen, 
                    numOfTLV, internalCellIndex)
           )
        /*SPR 16855 +-*/
        {
            return MAC_FAILURE;
        }
        /*SPR 16855 +-*/
        sendControlInfoToPhy((UInt8 *)L1ConfigReq_p, phyReconfigMsgLen,
                internalCellIndex);
        freeMemPool(L1ConfigReq_p);
        L1ConfigReq_p = PNULL;
        /*SPR 16855 +-*/
    }
    else
    {
        /* Means numOfTLV is 1 and 1 or more(but not all) parameters returns failure or PARTIAL Success
         * so send MAC_SUCCESS in response */
        resp = MAC_PARTIAL_SUCCESS;
    }

    /* SPR 3224 END */
    return resp;
}

/* CA Changes end */
/* Cell Start Stop Chg */
/*****************************************************************************
 * Function Name  : reConfigPhyCellAfterCellStop 
 * Inputs         : msg_p and totalMsgLen
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is used by MAC to reconfigure PHY Cell 
 *                  after Cell Stop is done. 
 *                  It shall send CONFIG.request to PHY and also receive the 
 *                  response.
 *****************************************************************************/
/* CA Changes start */
MacRetType sendAndParseReConfigPhyCellAfterCellStop(UInt8 *msg_p, UInt16 totalMsgLen,
        InternalCellIndex internalCellIndex)
{
    /* FAPI Msg Structure */
    UInt8 numTLV = 0;
    FAPI_l1ApiMsg_st *L1ReConfigReq_p = PNULL;
    UInt16 msgLength = 0;

    /* PHY_RECONFIG MSG Length Check */
    if (totalMsgLen < PHY_RECONFIG_MIN_LEN)
    {
        return MAC_FAILURE;
    }

    if(MAC_SUCCESS == parseConfigPhyCellAfterCellStop(msg_p, totalMsgLen, 
                &numTLV,internalCellIndex))
    {
        /* Create the PHY Cell Reconfiguration message as per FF MAC-PHY interface */
        if (MAC_FAILURE == fillPhyReConfigRequestAfterCellStop (&L1ReConfigReq_p,
                    &msgLength, numTLV, internalCellIndex)
                /* CA Changes end */
           )
        {
            LTE_MAC_UT_LOG(LOG_WARNING, CELL_CONF,"Error filling Cell config request "
                    "for PHY");
            return MAC_FAILURE;
        }
/* SPR 19288 change start */
#ifdef FLEXRAN
        g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_CFG_REQ; 
#endif
        /* Sending FAPI PHY Cell Config Request message to PHY */
        sendControlInfoToPhy( (UInt8 *)L1ReConfigReq_p, msgLength, internalCellIndex);

        freeMemPool(L1ReConfigReq_p);
        L1ReConfigReq_p = NULL;
    }
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : sendReConfigPhyCellAfterCellStop
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is used by MAC to send reconfigure PHY Cell 
 *                  after Cell Stop is done. 
 *****************************************************************************/
MacRetType sendReConfigPhyCellAfterCellStop(InternalCellIndex internalCellIndex)
{
    UInt8 numTLV = 0;
    /* FAPI Msg Structure */
    FAPI_l1ApiMsg_st *L1ReConfigReq_p = PNULL;
    UInt16 msgLength = 0;

    if (MAC_FAILURE == fillPhyReConfigRequestAfterCellStop (&L1ReConfigReq_p, &msgLength, 
                numTLV,internalCellIndex)
       )
    {
        LTE_MAC_UT_LOG(LOG_WARNING, CELL_CONF,"Error filling Cell config request "
                "for PHY");
        return MAC_FAILURE;
    }

    sendControlInfoToPhy( (UInt8 *)L1ReConfigReq_p, msgLength,internalCellIndex);
    freeMemPool(L1ReConfigReq_p);
    L1ReConfigReq_p = NULL;
    return MAC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : handleReConfigPhyCellAfterCellStop
 * Inputs         : msgBuf and bytesRead
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is used by MAC to handle reconfigure PHY Cell 
 *                  after Cell Stop is done. 
 *****************************************************************************/
UInt32 handleReConfigPhyCellAfterCellStop(UInt8 *msgBuf, UInt16 bytesRead ,
        InternalCellIndex internalCellIndex)
{

    UInt32 resp = MAC_FAILURE;
    UInt16  iterator;
    /* FAPI Msg Structure */
    FAPI_l1ApiMsg_st *L1ReConfigRes_p = PNULL;
    FAPI_phyCellConfigResp_st *ReConfigRes_p = PNULL;
#ifdef LOG_PRINT_ENABLED
    FAPI_cellConfig_st   *listOfInvalidTlvs_p = PNULL;
    FAPI_cellConfig_st   *listOfMissingTlvs_p = PNULL;
#endif
    if( (bytesRead > 0) && 
            (bytesRead <= sizeof(FAPI_l1ApiMsg_st) )
      )
    {
        LTE_MAC_UT_LOG(LOG_WARNING, CELL_CONF,"No. of bytes received are less "
                "than FAPI_l1ApiMsg_st");
    }
    else if(bytesRead > 0)
    {
        L1ReConfigRes_p = (FAPI_l1ApiMsg_st *)msgBuf;

        /* Check for response is for Config request otherwise it is Failure */
        if(PHY_CELL_CONFIG_RESPONSE == L1ReConfigRes_p->msgId)
        {
            ReConfigRes_p = 
                (FAPI_phyCellConfigResp_st *) &L1ReConfigRes_p->msgBody[0];
            if( FAPI_MSG_OK == ReConfigRes_p->errorCode)
            {
                LTE_MAC_UT_LOG(LOG_INFO, CELL_CONF,"PHY Cell Re-Configuration "
                        "After Cell Stop Success");
                phyCellConfigGlobalFill(internalCellIndex);
                resp = MAC_SUCCESS;
            }
            else
            {
                LOG_MAC_MSG(PHY_CELL_CONFIG_FAIL,LOGERROR,MAC_L1_INF,
                        getCurrentTick(),ReConfigRes_p->errorCode, 
                        ReConfigRes_p->numOfMissingTLV, 
                        ReConfigRes_p->numOfInvalidOrUnsupportedTLV,
                        __LINE__,bytesRead,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        __func__,"RECONFIG Req");

                /* Log the Missing TLVs*/
                if(ReConfigRes_p->numOfMissingTLV)
                {
                    for(iterator = 0; iterator < ReConfigRes_p->numOfMissingTLV; 
                            iterator++) 
                    {
#ifdef LOG_PRINT_ENABLED
                        listOfMissingTlvs_p = &ReConfigRes_p->listOfMissingTlv[iterator];
#endif
                        LOG_MAC_MSG(PHY_MAC_TLV_TAG,LOGWARNING,MAC_L1_INF,
                                getCurrentTick(),__LINE__, 
                                listOfMissingTlvs_p->tag, 0,
                                0,0,
                                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                                __func__,"Missing");
                    }
                }
                /* Log the Unsupported TLVs*/
                if(ReConfigRes_p->numOfInvalidOrUnsupportedTLV)
                {
                    for(iterator = 0; iterator < ReConfigRes_p->
                            numOfInvalidOrUnsupportedTLV; iterator++)
                    {
#ifdef LOG_PRINT_ENABLED
                        listOfInvalidTlvs_p = &ReConfigRes_p->listOfTLV[iterator];
#endif                        
                        LOG_MAC_MSG( PHY_MAC_TLV_TAG, LOGWARNING, MAC_L1_INF,
                                getCurrentTick(), __LINE__, 
                                listOfInvalidTlvs_p->tag, 0, 0,0,
                                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                                __func__,"Invalid");
                    }
                }
            }
        }
        else
        {
            LOG_MAC_WARNING( MAC_RRC_INF,"Invalid Response Recevied \n");
        }
    }
    return resp;
}

/*Cyclomatic Complexity changes - starts here */
/*****************************************************************************
 * Function Name  : reconfigPhyCellParams
 * Inputs         : remLen_p
 *                  numTLV_p
 *                  msg_p_p
 *                  ulChBandwidth_p
 *                  dlChBandwidth_p
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function will reconfig Phy cell parameters.
 *****************************************************************************/
 MacRetType reconfigPhyCellParams(UInt16* remLen,UInt8* numTLV,
                                          /* SPR 12915 Changes Start */
                                          UInt8 **msgBuf_p,
                                          /* SPR 12915 Changes End */
                                          UInt8* ulChBandwidth,
        UInt8* dlChBandwidth,
        InternalCellIndex internalCellIndex)
{
    /* SPR 12915 Changes Start */
    UInt8 *msg_p = *msgBuf_p;
    /* SPR 12915 Changes End */

    MacRetType retVal = MAC_SUCCESS;
    UInt16 subBitMask = 0;

    /* + PRS_CHANGES */
    /* reinitialize the PRS update flag before parse */
    reconfigCellMajor_g[internalCellIndex].prsPhyConfig.updateFlag = FALSE;
    /* - PRS_CHANGES */

    subBitMask = LTE_GET_U16BIT(msg_p);
    msg_p += 2;
    (*remLen) -= 2;

    if(subBitMask & BIT_FIRST)/* UL Carrier Frequency Indiacator*/
    {
        msg_p += 2;
        (*remLen) -= 2;
    }
    if(subBitMask & BIT_SECOND)/* DL Carrier Frequency Indicator*/
    {
        msg_p += 2;
        (*remLen) -= 2;
    }

    /* Cyclomatic_complexity_changes_start */
    /* Total number of antennas */
    retVal = parseReconfigPhyCellParamsForTxAntenna(subBitMask,
            &msg_p,
            remLen,
            numTLV,
            internalCellIndex);
    if (MAC_SUCCESS != retVal)
    {
        /* SPR 12915 Changes Start */
        *msgBuf_p = msg_p;
        /* SPR 12915 Changes End */
        return retVal;
    }

    /* UL Bandwidth */
    retVal = parseReconfigPhyCellParamsForUlBW(subBitMask,
            ulChBandwidth,
            &msg_p,
            remLen,
            numTLV,
            internalCellIndex);
    if (MAC_SUCCESS != retVal)
    {
        /* SPR 12915 Changes Start */
        *msgBuf_p = msg_p;
        /* SPR 12915 Changes End */
        return retVal;
    }

    /* DL Bandwidth */
    retVal = parseReconfigPhyCellParamsForDlBW(subBitMask,
            dlChBandwidth,
            &msg_p,
            remLen,
            numTLV,
            internalCellIndex);
    if (MAC_SUCCESS != retVal)
    {
        /* SPR 12915 Changes Start */
        *msgBuf_p = msg_p;
        /* SPR 12915 Changes End */
        return retVal;
    }
    /* Cyclomatic_complexity_changes_end */

    /* SPR 12915 Changes Start */
    /*
     ** PRS and other TAGs were getting parsed only when dlBW TAG was present.
     ** PRS and other TAGs should be parsed if there respective TAGs are present
     ** in the cell reconfigure request.
     */
    /* SPR 12915 Changes End */

    retVal =  parseReconfigPhyCellParamsForCyclicPrefix(
            subBitMask, &msg_p, remLen, numTLV, internalCellIndex);
    
    if (MAC_SUCCESS != retVal)
    {
        /* SPR 12915 Changes Start */
        *msgBuf_p = msg_p;
        /* SPR 12915 Changes End */
        return retVal;
    }

    if(subBitMask & BIT_EIGTH)/* SRS Bandwidth configuration */
    {
        UInt16 srsBitMask = 0;
        /* Till Date no use of this bitmask */
        srsBitMask = LTE_GET_U16BIT(msg_p);
        msg_p += 2;
        (*remLen) -= 2;

        /* bitmask For SRS optional parameters */
        srsBitMask = LTE_GET_U16BIT(msg_p);
        msg_p += 2;
        (*remLen) -= 2;

        reconfigCellMajor_g[internalCellIndex].bandWidthConfiguration = *msg_p++;
        (*remLen)--;

        if (reconfigCellMajor_g[internalCellIndex].bandWidthConfiguration != 
                cellConfigPhy_gp[internalCellIndex]->srsConfig.bandWidthConfiguration
           )
        {
            /* SPR 12915 Changes Start */
            *msgBuf_p = msg_p;
            /* SPR 12915 Changes End */
            return MAC_FAILURE;
        }

        reconfigCellMajor_g[internalCellIndex].bandWidthConfiguration_flag = TRUE;
        (*numTLV)++;

        /* SRS Subframe config */
        msg_p++;
        (*remLen)--;
        /* acknackSrsSimultaneous Transmission */
        msg_p++;
        (*remLen)--;

        /* srsMaxUpPts */
        if(srsBitMask & BIT_FIRST)
        {
            msg_p++;
            (*remLen)--;
        }
    }
    /* SPR 5525 changes start */
    if(subBitMask & BIT_NINTH) /* duplexing mode */
    {
        reconfigCellMajor_g[internalCellIndex].duplexingMode = *msg_p++;
        (*remLen)--;
        if(reconfigCellMajor_g[internalCellIndex].duplexingMode
                != cellConfigPhy_gp[internalCellIndex]->duplexingMode)
        {
            /* SPR 12915 Changes Start */
            *msgBuf_p = msg_p;
            /* SPR 12915 Changes End */
            return MAC_FAILURE;
        }
        reconfigCellMajor_g[internalCellIndex].duplexingMode_flag = TRUE;
        (*numTLV)++;
    }
    /* SPR 5525 changes end */
    if (subBitMask & BIT_TENTH) /* cell Id */
    {
        reconfigCellMajor_g[internalCellIndex].physicalCellId =
            LTE_GET_U16BIT(msg_p);
        /* SPR 5500 Fix Start */

        if( MAX_VAL_PHYSICAL_CELL_ID <
                reconfigCellMajor_g[internalCellIndex].physicalCellId)
            /* || (cellConfigPhy_gp[internalCellIndex]->schConfig.physicalCellId
               != reconfigCellMajor_g[internalCellIndex].physicalCellId) */
            /* SPR 5500 Fix End */
        {
            /* SPR 12915 Changes Start */
            *msgBuf_p = msg_p;
            /* SPR 12915 Changes End */
            return MAC_FAILURE;
        }

        reconfigCellMajor_g[internalCellIndex].physicalCellId_flag = TRUE;
        msg_p += 2;
        /*(*remLen) -= 2; */
    }
    /* SPR 3802 */
    if (subBitMask & BIT_FIFTEENTH)
    {
        reconfigCellMajor_g[internalCellIndex].maxEpre = *msg_p++;
        (*remLen)--;
    }
    /* SPR 3802 */
    /* + PRS_CHANGES */
    if(subBitMask & BIT_SIXTEENTH) 
    {
        reconfigCellMajor_g[internalCellIndex].prsPhyConfig.prsBandWidth
            = *msg_p++;
        (*remLen)--;

        reconfigCellMajor_g[internalCellIndex].prsPhyConfig.prsCyclicPrifix 
            = *msg_p++;
        (*remLen)--;

        /* SPR_7936_FIX  */
        if((cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsBandWidth ==
                    reconfigCellMajor_g[internalCellIndex].prsPhyConfig.prsBandWidth) &&
                (cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsCyclicPrifix == 
                 reconfigCellMajor_g[internalCellIndex].prsPhyConfig.prsCyclicPrifix))
        {
            LOG_MAC_WARNING( MAC_RRC_INF,"PRS is already configured with same set of values BW:%d CP:%d",
                    cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsBandWidth,
                    cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsCyclicPrifix);
        }
        /* Atleast one of the field is updated */
        else
        {
            reconfigCellMajor_g[internalCellIndex].prsPhyConfig.updateFlag = TRUE;
            /* SPR_7936_FIX  */

            (*numTLV)++;
        }
    }
    /* - PRS_CHANGES */
    
    /* SPR 12915 Changes Start */
    *msgBuf_p = msg_p;
    /* SPR 12915 Changes End */
    return retVal;
}
/*Cyclomatic Complexity changes - ends here */

/*****************************************************************************
 * Function Name  : parseConfigPhyCellAfterCellStop
 * Inputs         : msg_p, totalMsgLen and numTLV_p
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/ MAC_FAILURE
 * Description    : This function is used by MAC to parse reconfigure PHY Cell
 *                  after Cell Stop is done.
 *****************************************************************************/
/* CA Changes start */
MacRetType parseConfigPhyCellAfterCellStop(UInt8 *msg_p, 
        UInt16 totalMsgLen,
        UInt8 *numTLV_p,
        InternalCellIndex internalCellIndex)
{
    UInt8 numTLV = 0;
    UInt16 bitMask = 0;
    UInt8 ulChBandwidth = 0;
    UInt8 dlChBandwidth = 0;
    UInt16 remLen = totalMsgLen;
    UInt32 retValue = MAC_FAILURE;


    /* Parsing manadatory parameters */
    bitMask = LTE_GET_U16BIT(msg_p);
    msg_p += 2;
    remLen -= 2;

    /* if bitmask is zero return failure from here */
    if(0 == bitMask)
    {
        return MAC_FAILURE;
    }

    remLen --;

    /* Ignore SFN */
    msg_p += 2;
    remLen -= 2;
    /* Ignore SF */
    msg_p++;
    remLen --;

    memSet(&reconfigCellMajor_g[internalCellIndex], 0, sizeof(ReConfigPhyCellMajor));
    if(remLen > 0)
    {
        if(bitMask & BIT_FIRST) /* PHY Reconfig Cell Parameters */
        {
            /*Cyclomatic Complexity changes - starts here */
            retValue = reconfigPhyCellParams(&remLen,&numTLV,
                    /* SPR 12915 Changes Start */
                    &msg_p,
                    /* SPR 12915 Changes End */
                    &ulChBandwidth,
                    &dlChBandwidth,
                    internalCellIndex);
            if(MAC_FAILURE == retValue)
            { 
                return MAC_FAILURE;
            }
            /*Cyclomatic Complexity changes - ends here */
        }
        /* Cyclomatic_complexity_changes_start */
        if(bitMask & BIT_SECOND)/* Sync Signals */
        {
            retValue = reconfigSyncSignalCellParams(&remLen,
                    &numTLV,
                    &msg_p,
                    internalCellIndex);
            if(MAC_FAILURE == retValue)
            { 
                return MAC_FAILURE;
            }
        }
        if(bitMask & BIT_THIRD) /* PRACH Structure */
        {
            retValue = reconfigPrachCellParams(&remLen,
                    &numTLV,
                    &msg_p,
                    internalCellIndex);
            if(MAC_FAILURE == retValue)
            { 
                return MAC_FAILURE;
            }
        }
        if(bitMask & BIT_FOURTH) /* PUSCH Config */
        {
            retValue = reconfigPuschCellParams(&remLen,
                    &numTLV,
                    &msg_p,
                    internalCellIndex);
            if(MAC_FAILURE == retValue)
            { 
                return MAC_FAILURE;
            }
        }
        if(bitMask & BIT_FIFTH) /* PUCCH Config */
        {
            retValue = reconfigPucchCellParams(&remLen,
                    &numTLV,
                    &msg_p,
                    internalCellIndex);
            if(MAC_FAILURE == retValue)
            { 
                return MAC_FAILURE;
            }
        }
        if(bitMask & BIT_SIXTH) /* PHICH Config */
        {
            retValue = reconfigPhichCellParams(&remLen,
                    &numTLV,
                    &msg_p,
                    internalCellIndex);
            if(MAC_FAILURE == retValue)
            { 
                return MAC_FAILURE;
            }
        }
        if(bitMask & BIT_SEVENTH)/* PDSCH Config */
        {
            retValue = reconfigPdschCellParams(&remLen,
                    &numTLV,
                    &msg_p,
                    internalCellIndex);
            if(MAC_FAILURE == retValue)
            { 
                return MAC_FAILURE;
            }
            /* Cyclomatic_complexity_changes_end */
        }
        if(remLen > 0)
        {
            LOG_MAC_WARNING( MAC_RRC_INF,"Invalid PHY Reconfig Message from L3 for cell:%d, Containing Invalid"
                    " TLVs \n", macCommonGetRRCCellIndex(internalCellIndex));
            return MAC_FAILURE; 
        }

    }
    else
    {
        return MAC_FAILURE;
    }

    *numTLV_p = numTLV;
    return MAC_SUCCESS;
}

/* CA Changes end */
/**********************************************************************
 * Function Name  : phyCellConfigGlobalFill 
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to fill PHY Reconfig global variables.
 **********************************************************************/
/* CA Changes start */
void phyCellConfigGlobalFill(InternalCellIndex internalCellIndex)
{
    /* Update PHY Cell global settings at MAC */
    if(TRUE == reconfigCellMajor_g[internalCellIndex].duplexingMode_flag)
    {
        cellConfigPhy_gp[internalCellIndex]->duplexingMode = 
            reconfigCellMajor_g[internalCellIndex].duplexingMode;
    }
    if(TRUE == reconfigCellMajor_g[internalCellIndex].rxAntennaPort_flag)
    {
        cellConfigPhy_gp[internalCellIndex]->rfConfig.rxAntennaPort =
            reconfigCellMajor_g[internalCellIndex].rxAntennaPort;
    }
    if(TRUE == reconfigCellMajor_g[internalCellIndex].txAntennaPort_flag)
    {
        cellConfigPhy_gp[internalCellIndex]->rfConfig.txAntennaPort =
            reconfigCellMajor_g[internalCellIndex].txAntennaPort;
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].ulChannelBW_flag)
    {
        cellConfigPhy_gp[internalCellIndex]->rfConfig.ulChannelBW = 
            reconfigCellMajor_g[internalCellIndex].ulChannelBW;
        getulBandwidth(reconfigCellMajor_g[internalCellIndex].ulChannelBW,
                internalCellIndex);   
    }
    if(TRUE == reconfigCellMajor_g[internalCellIndex].dlChannelBW_flag)
    {
        cellConfigPhy_gp[internalCellIndex]->rfConfig.dlChannelBW = 
            reconfigCellMajor_g[internalCellIndex].dlChannelBW;
        getdlBandwidth(reconfigCellMajor_g[internalCellIndex].dlChannelBW,
                internalCellIndex);   
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].ulCyclicPrefixType_flag)
    {
        cellConfigPhy_gp[internalCellIndex]->ulCyclicPrefixType = 
            reconfigCellMajor_g[internalCellIndex].ulCyclicPrefixType;
    }
    if(TRUE == reconfigCellMajor_g[internalCellIndex].dlCyclicPrefixType_flag)
    {
        cellConfigPhy_gp[internalCellIndex]->dlCyclicPrefixType = 
            reconfigCellMajor_g[internalCellIndex].dlCyclicPrefixType;
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].physicalCellId_flag)
    {
        cellConfigPhy_gp[internalCellIndex]->schConfig.physicalCellId = 
            reconfigCellMajor_g[internalCellIndex].physicalCellId;
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].primarySyncSignal_flag)
    {
        cellConfigPhy_gp[internalCellIndex]->schConfig.primarySyncSignal = 
            reconfigCellMajor_g[internalCellIndex].primarySyncSignal;
    }
    if(TRUE == reconfigCellMajor_g[internalCellIndex].secondarySyncSignal_flag)
    {
        cellConfigPhy_gp[internalCellIndex]->schConfig.secondarySyncSignal = 
            reconfigCellMajor_g[internalCellIndex].secondarySyncSignal;
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].configurationIndex_flag)            
    {
        cellConfigPhy_gp[internalCellIndex]->prachConfig.configurationIndex =
            reconfigCellMajor_g[internalCellIndex].configurationIndex;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].rootSeqIndex_flag)
    {            
        cellConfigPhy_gp[internalCellIndex]->prachConfig.rootSeqIndex =
            reconfigCellMajor_g[internalCellIndex].rootSeqIndex;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].freqOffset_flag)            
    {
        cellConfigPhy_gp[internalCellIndex]->prachConfig.freqOffset =
            reconfigCellMajor_g[internalCellIndex].freqOffset;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].zeroCorelationZoneConfig_flag)            
    {
        cellConfigPhy_gp[internalCellIndex]->prachConfig.zeroCorelationZoneConfig =
            reconfigCellMajor_g[internalCellIndex].zeroCorelationZoneConfig;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].highSpeedFlag_flag)
    {            
        cellConfigPhy_gp[internalCellIndex]->prachConfig.highSpeedFlag =
            reconfigCellMajor_g[internalCellIndex].highSpeedFlag;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].hoppingOffset_flag)
    {            
        cellConfigPhy_gp[internalCellIndex]->puschConfig.hoppingOffset =
            reconfigCellMajor_g[internalCellIndex].hoppingOffset;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].hoppingMode_flag)
    {            
        cellConfigPhy_gp[internalCellIndex]->puschConfig.hoppingMode =
            reconfigCellMajor_g[internalCellIndex].hoppingMode;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].numOfSubBand_flag)
    {            
        cellConfigPhy_gp[internalCellIndex]->puschConfig.numOfSubBand =
            reconfigCellMajor_g[internalCellIndex].numOfSubBand;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].groupHopEnable_flag)
    {            
        cellConfigPhy_gp[internalCellIndex]->puschConfig.groupHopEnable =
            reconfigCellMajor_g[internalCellIndex].groupHopEnable;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].groupAssignPUSCH_flag)
    {            
        cellConfigPhy_gp[internalCellIndex]->puschConfig.groupAssignPUSCH =
            reconfigCellMajor_g[internalCellIndex].groupAssignPUSCH;            
    }

    /*Cyclomatic Complexity changes - starts here */
    fillPhyCellReconfigParams (internalCellIndex);
    /*Cyclomatic Complexity changes - end here */
}
/* CA Changes end */
/* Cell Start Stop Chg */


/**********************************************************************
 * Function Name  : calcReconfigOffsets
 * Inputs         : ms_p, bitMask and
 *                  offSetArr - offset array container 
 * Outputs        : offSetArr
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is used by to calculate the offsets. 
 **********************************************************************/
MacRetType calcReconfigOffsets(UInt8 *msg_p, UInt16 bitMask, SInt16 * offSetArr)
{
    UInt16 phyReconfigCellParambitmask = 0; 
    SInt16  currentOffset = 0;
    UInt16 presenceBitMask = 0;
    UInt8 *temp_msg_p = msg_p;

    /* check the bitmask for phyReconfigCellParameters */
    if(bitMask & 0x01)
    {
        offSetArr[PHY_RECONF_PARAM] = currentOffset;
        phyReconfigCellParambitmask =  LTE_GET_U16BIT(temp_msg_p);
        /* Skipping the duplexingMode param */
        temp_msg_p += 2;
        currentOffset += 2;

        /* duplex mode */
        if(phyReconfigCellParambitmask & 0x100)
        {
            offSetArr[DUPLEX_MODE] = currentOffset;
            currentOffset += 1;
            temp_msg_p += 1;
        }
        /* ulEARFCN */    
        if(phyReconfigCellParambitmask & 0x01)
        {
            offSetArr[UL_EAR_FCN] = currentOffset;
            currentOffset += 2;
            temp_msg_p += 2;
        }
        /* dlEARFCN */
        if(phyReconfigCellParambitmask & 0x02)
        {
            offSetArr[DL_EAR_FCN] = currentOffset;
            currentOffset += 2;
            temp_msg_p += 2;
        }
        /* num Of Antennas */ 
        if(phyReconfigCellParambitmask & 0x04)
        {
            offSetArr[ANTENNAE] = currentOffset;
            currentOffset += 1;
            temp_msg_p += 1;
        }
        /* ul Tx Bandwidth */
        if(phyReconfigCellParambitmask & 0x08)
        {
            offSetArr[UL_TX_BW] = currentOffset;
            currentOffset += 1;
            temp_msg_p += 1;
        }
        /* dl Tx Bandwidth */
        if(phyReconfigCellParambitmask & 0x10)
        {
            offSetArr[DL_TX_BW] = currentOffset;
            currentOffset += 1;
            temp_msg_p += 1;
        }
        /* ul Cyclic Prefix */
        if(phyReconfigCellParambitmask & 0x20)
        {
            offSetArr[UL_CYC_PRE] = currentOffset;
            currentOffset += 1;
            temp_msg_p += 1;
        }
        /* dl Cyclic Prefix */
        if(phyReconfigCellParambitmask & 0x40)
        {
            offSetArr[DL_CYC_PRE] = currentOffset;
            currentOffset += 1;
            temp_msg_p += 1;
        }
        /* Srs Bandwidth Configuration */
        if(phyReconfigCellParambitmask & 0x80)
        {
            offSetArr[SRS_CONFIG] = currentOffset;
            presenceBitMask = LTE_GET_U16BIT(temp_msg_p);

            currentOffset += 2;
            temp_msg_p += 2;

            /* Structure for Sr Ul Config Common */ 
            if(presenceBitMask == 1)
            {
                presenceBitMask = LTE_GET_U16BIT(temp_msg_p);

                /* Increment the offset for the mandatory params */
                currentOffset += 5;
                temp_msg_p += 5;

                /* Structure for srs Max Up Pts */
                if(presenceBitMask == 1) 
                {
                    currentOffset += 1;
                    temp_msg_p += 1;
                }
            }
        }
        /* Physical Cell Id */
        if(phyReconfigCellParambitmask & 0x200)
        {
            offSetArr[PHY_CELL_ID] = currentOffset;
            currentOffset += 2;
            temp_msg_p += 2;
        }

        /*CLPC Change pcfichPowerOffset*/
        if(phyReconfigCellParambitmask & 0x400)
        {
            offSetArr[PCFICH_OFF] = currentOffset;
            currentOffset += 2;
            temp_msg_p += 2;
        }

        /*PhichPowerOffset*/
        if(phyReconfigCellParambitmask & 0x800)
        {
            offSetArr[PHICH_OFF] = currentOffset;
            currentOffset += 2;
            temp_msg_p += 2;
        }
        /* SPR 3802 */
        /* If reference signal is changed than MAX EPRE will be present */
        if((bitMask & 0x40) && (phyReconfigCellParambitmask & 0x4000))
        {
            offSetArr[MAX_EPRE] = currentOffset;
            currentOffset += 1;
            temp_msg_p += 1;
        }
        /* SPR 3802 */
        /* + PRS_CHANGES */
        if(phyReconfigCellParambitmask & BIT_SIXTEENTH)
        {
            offSetArr[PRS_CONFIG] = currentOffset;
            /* SPR_7936_FIX */
            currentOffset += 2;
            temp_msg_p += 2;
            /* SPR_7936_FIX */
        }
        /* - PRS_CHANGES */
    }
    /* check the bitmask for syncSignals */
    if(bitMask & 0x02)
    {
        offSetArr[SYNC_SIGNALS] =  currentOffset;
        currentOffset += 2;
    }
    /* check the bitmask for prachConfiguration */
    if(bitMask & 0x04)
    {
        offSetArr[PRACH_CONFIGURATION] = currentOffset;
        currentOffset += 6;
    }
    /* check the bitmask for puschConfiguration */
    if(bitMask & 0x08)
    {
        offSetArr[PUSCH_CONFIGURATION] = currentOffset;
        currentOffset += 7;
    }
    /* check the bitmask for pucchConfiguration */
    if(bitMask & 0x10)
    {
        offSetArr[PUCCH_CONFIGURATION] = currentOffset;
        currentOffset += 5;
    }
    /* check the bitmask for phichConfiguration */
    if(bitMask & 0x20)
    {
        offSetArr[PHICH_CONFIGURATION] = currentOffset;
        currentOffset += 2;
    }
    /* check the bitmask for pdschConfiguration */
    if(bitMask & 0x40)
    {
        offSetArr[PDSCH_CONFIGURATION] = currentOffset;
    }
    return MAC_SUCCESS;
}

/*Added For Power Control*/
/*****************************************************************************
 * Function Name  : checkAndUpdatePDSCHConfigCommonInfo 
 * Inputs         : msg_p, offSetArr -  offset array container and
 *                  numOfTLV
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCESS/MAC_FAILURE
 * Description    : This function is used by MAC to set reconfig fields in 
 *                  global reconfig struture.
 *****************************************************************************/
/* CA Changes start */
/*PHY_RECONF_CHG*/
MacRetType checkAndUpdatePDSCHConfigCommonInfo(UInt8 *msg_p, SInt16 *offSetArr,
        UInt8 *numOfTLV,InternalCellIndex internalCellIndex) 
/*PHY_RECONF_CHG*/
{
    UInt8 *tempMsg_p = msg_p;    
    SInt8 tempRefSigPower = 0;
    /*CLPC Change*/
    UInt8 temppB = 0;
    MacRetType retVal = MAC_PARTIAL_SUCCESS;
    if (offSetArr[PDSCH_CONFIGURATION] == INVALID_RECONFIG_PARAM_OFFSET)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Incorrect offset for PDSCH_CONFIGURATION [%d] or \
                Optional paramater absent ", offSetArr[PDSCH_CONFIGURATION]);
        return MAC_FAILURE;
    }
    tempMsg_p += offSetArr[PDSCH_CONFIGURATION]; 
    ReconfigPdschConfigCommonInfo *reconfigPdschConfigCommonInfo_p = 
        &(reconfigPhyCell_g[internalCellIndex].reconfigPdschConfigCommonInfo);

    /* The rsp to be converted from db to steps of 0 -255. Refer FAPI doc */
    tempRefSigPower =  *tempMsg_p++;
    temppB = *tempMsg_p;

    if((tempRefSigPower == 
                cellConfigPhy_gp[internalCellIndex]->rfConfig.refSignalPower)
            && (temppB == cellConfigPhy_gp[internalCellIndex]->pb))
        /* CA Changes end */
    {
        reconfigPdschConfigCommonInfo_p->updateFlag = FALSE;
    }
    else
    {
        /* SPR 3802 */  
        /* + SPR 9603 Change Start*/
	/* SPR 21548 fix start */
	reconfigPdschConfigCommonInfo_p->refSignalPower =
		((cellConfigPhy_gp[internalCellIndex]->maxEpre - EPRE_TRANSFORM_BASE) - ((tempRefSigPower)*4));
	/* SPR 21548 fix end */
        /* - SPR 9603 Change End*/
        /* SPR 3802 */
        reconfigPdschConfigCommonInfo_p->pB = temppB;
        reconfigPdschConfigCommonInfo_p->updateFlag = TRUE;
        /*PHY_RECONF_CHG*/
        *numOfTLV = *numOfTLV + 2;
        /*PHY_RECONF_CHG*/
        retVal = MAC_SUCCESS;
    }

    return retVal;
}
/* CA Changes end */

/*****************************************************************************
 * Function Name  : checkAndUpdatePcfichPowerOffset 
 * Inputs         : msg_p -  message buffer, offSetArr - offset array and
 *                  numOfTLV
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCESS/MAC_FAILURE
 * Description    : This function is used by MAC to set reconfig fields in 
 *                  global reconfig struture.
 *****************************************************************************/
/*PHY_RECONF_CHG*/
/* CA Changes start */
MacRetType checkAndUpdatePcfichPowerOffset(UInt8 *msg_p,
        SInt16 *offSetArr, UInt8 *numOfTLV, InternalCellIndex internalCellIndex)
/*PHY_RECONF_CHG*/
{
    UInt8 *tempMsg_p = msg_p;
    UInt16 tempPcfichPowerOffset = 0;
    MacRetType retVal = MAC_PARTIAL_SUCCESS;
    if (offSetArr[PCFICH_OFF] == INVALID_RECONFIG_PARAM_OFFSET)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Incorrect offset for PCFICH Power [%d] or \
                Optional paramater absent ", offSetArr[PCFICH_OFF]);
        return MAC_FAILURE;
    }

    tempMsg_p += offSetArr[PCFICH_OFF];
    ReconfigCommonDLPControlInfoPhy *reconfigCommonDLPControlInfoPhy_p = 
        &(reconfigPhyCell_g[internalCellIndex].reconfigCommonDLPowerControlInfo);

    tempPcfichPowerOffset = LTE_GET_U16BIT(tempMsg_p);
    tempMsg_p += 2;

    if(tempPcfichPowerOffset == 
            cellConfigPhy_gp[internalCellIndex]->pcfichPowerOffset)
        /* CA Changes end */
    {
        reconfigCommonDLPControlInfoPhy_p->updateFlagPcfich = FALSE;
    }
    else
    {
        reconfigCommonDLPControlInfoPhy_p->pcfichPowerOffset = tempPcfichPowerOffset;
        reconfigCommonDLPControlInfoPhy_p->updateFlagPcfich = TRUE;
        /*PHY_RECONF_CHG*/
        *numOfTLV = *numOfTLV + 1;
        /*PHY_RECONF_CHG*/
        retVal = MAC_SUCCESS;
    }
    return retVal;
}


/*****************************************************************************
 * Function Name  : checkAndUpdatePhichPowerOffset 
 * Inputs         : msg_p - message buffer offSetArr - offset array and
 *                  numOfTLV
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCESS/MAC_FAILURE
 * Description    : This function is used by MAC to set reconfig fields in 
 *                  global reconfig struture.
 *****************************************************************************/
/*PHY_RECONF_CHG*/
/* CA Changes start */
MacRetType checkAndUpdatePhichPowerOffset(UInt8 *msg_p,
        SInt16 *offSetArr, UInt8 *numOfTLV, InternalCellIndex internalCellIndex)
/*PHY_RECONF_CHG*/
{
    UInt8 *tempMsg_p = msg_p;
    UInt16 tempPhichPowerOffset = 0;
    MacRetType retVal = MAC_PARTIAL_SUCCESS;
    if (offSetArr[PHICH_OFF] == INVALID_RECONFIG_PARAM_OFFSET)
    {
        LOG_MAC_WARNING( MAC_RRC_INF,"Incorrect offset for PHICH Power [%d] or \
                Optional paramater absent ", offSetArr[PHICH_OFF]);
        return MAC_FAILURE;
    }
    tempMsg_p += offSetArr[PHICH_OFF];
    ReconfigCommonDLPControlInfoPhy *reconfigCommonDLPControlInfoPhy_p =
        &(reconfigPhyCell_g[internalCellIndex].reconfigCommonDLPowerControlInfo);

    tempPhichPowerOffset = LTE_GET_U16BIT(tempMsg_p);
    tempMsg_p += 2;

    if(tempPhichPowerOffset == 
            cellConfigPhy_gp[internalCellIndex]->phichConfig.phichPowOffset)
        /* CA Changes end */
    {
        reconfigCommonDLPControlInfoPhy_p->updateFlagPhich = FALSE;
    }
    else
    {
        reconfigCommonDLPControlInfoPhy_p->phichPowerOffset = tempPhichPowerOffset;
        reconfigCommonDLPControlInfoPhy_p->updateFlagPhich = TRUE;
        /*PHY_RECONF_CHG*/
        *numOfTLV = *numOfTLV + 1;
        /*PHY_RECONF_CHG*/
        retVal = MAC_SUCCESS;
    }
    return retVal;
}
/*****************************************************************************
 * Function Name  : checkAndUpdateSynchSignals 
 * Inputs         : msg_p - message buffer, offSetArr - offset array and
 *                  numOfTLV
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCESS/MAC_FAILURE
 * Description    : This function is used by MAC to set reconfig fields in 
 *                  global reconfig struture.
 *****************************************************************************/
/* CA Changes start */
/*PHY_RECONF_CHG*/
MacRetType checkAndUpdateSynchSignals (UInt8 *msg_p,
        SInt16 *offSetArr, UInt8 *numOfTLV, InternalCellIndex internalCellIndex)
/* CA Changes end */
/*PHY_RECONF_CHG*/
{
    UInt8 *tempMsg_p = msg_p;
    UInt16 tempPrimarySignal = 0;
    UInt16 tempSecondarySignal = 0;
    MacRetType retVal = MAC_PARTIAL_SUCCESS;

    if (offSetArr[SYNC_SIGNALS] == INVALID_RECONFIG_PARAM_OFFSET)
    {
        return MAC_FAILURE;
    }
    tempMsg_p += offSetArr[SYNC_SIGNALS];
    tempPrimarySignal = *tempMsg_p++;
    tempPrimarySignal = (tempPrimarySignal + MIN_VALUE_OF_SYNC_SIGNAL) *
                            MULTIPLY_COEFF_FOR_SYNC_SIGNAL;

    /* CA Changes start */
    SyncSignal *syncSignal_p =
        &(reconfigPhyCell_g[internalCellIndex].syncSignal);

    if(tempPrimarySignal ==
            cellConfigPhy_gp[internalCellIndex]->schConfig.primarySyncSignal)
    {
        syncSignal_p->updatePrimarySignalFlag = FALSE;
    }
    else
    {
        syncSignal_p->primarySignal = tempPrimarySignal;
        syncSignal_p->updatePrimarySignalFlag = TRUE;
        /*PHY_RECONF_CHG*/
        *numOfTLV = *numOfTLV + 1;
        /*PHY_RECONF_CHG*/
        retVal = MAC_SUCCESS;
    }

    /*For Seconadary Signal*/
    tempSecondarySignal = *tempMsg_p++;
    tempSecondarySignal = (tempSecondarySignal + MIN_VALUE_OF_SYNC_SIGNAL) *
                             MULTIPLY_COEFF_FOR_SYNC_SIGNAL;


    if(tempSecondarySignal ==
            cellConfigPhy_gp[internalCellIndex]->schConfig.secondarySyncSignal)
        /* CA Changes end */
    {
        syncSignal_p->updateSecondarySignalFlag = FALSE;
    }
    else
    {
        syncSignal_p->secondarySignal = tempSecondarySignal;
        syncSignal_p->updateSecondarySignalFlag = TRUE;
        /*PHY_RECONF_CHG*/
        *numOfTLV = *numOfTLV + 1;
        /*PHY_RECONF_CHG*/
        retVal = MAC_SUCCESS;
    }

    return retVal;

}

/*coverity 10512*/
/*****************************************************************************
 * Function Name  : getPhyCellReConfigReqPtr 
 * Inputs         : reconfigReq_p 
 * Outputs        : None
 * Returns        : FAPI_phyCellReConfigRequest_st pointer
 * Description    : 
 ******************************************************************************/

 FAPI_phyCellReConfigRequest_st * getPhyCellReConfigReqPtr(FAPI_l1ApiMsg_st *reconfigReq_p)
{
    return (FAPI_phyCellReConfigRequest_st *)&reconfigReq_p->msgBody[0];
}
/*coverity 10512*/
/*****************************************************************************
 * Function Name  : fillPhyReConfigRequest
 * Inputs         : reconfigReq_pp - Cell Reconfig structure that is filled after parsing
 *                  Phy cell reconfiguration request received from RRC for PHY,
 *                  msgLen_p and numOfTlv
 *                  internalCellIndex - cell id  
 * Outputs        : Message structure for Phy Cell reconfig request on
 *                  MAC - PHY interface (FAPI_l1ApiMsg_st)
 * Returns        : MAC_FAILURE/MAC_SUCCESS 
 * Description    : This function is used by MAC to fill Phy Cell 
 *                  Reconfig request(PHY_CELL_RECONFIG_REQ) that needs
 *                  to be sent to PHY interface.
 *****************************************************************************/
/* CA Changes start */
MacRetType fillPhyReConfigRequest(FAPI_l1ApiMsg_st **reconfigReq_pp,
        UInt16  *msgLen_p, UInt8 numOfTlv, InternalCellIndex internalCellIndex)
{
    UInt8 tlvIndex = 0;   
    UInt16 sfnsf = 0; 
    FAPI_l1ApiMsg_st *reconfigReq_p = PNULL;
    UInt16 reConfigMsgLen = 0;
    FAPI_phyCellReConfigRequest_st *FapiCellReConfigReq_p = PNULL;
    /* Allocate memory for sending request */
    reConfigMsgLen = sizeof(FAPI_l1ApiMsg_st) +
        sizeof(FAPI_phyCellReConfigRequest_st) + 
        ( (numOfTlv - 1) * sizeof(FAPI_cellReConfig_st) ) - 2; /* -2 is for 1-1 byte memory taken by
                                                                  the stretchable arrays */ 
    GET_MEM_FROM_POOL(FAPI_l1ApiMsg_st,reconfigReq_p, reConfigMsgLen, NULL);
    /* COVERITY CID 24428 FIX START */
    if ( !reconfigReq_p )
    {
        return MAC_FAILURE;
    }
    /* COVERITY CID 24428 FIX END */


    memSet(reconfigReq_p, 0, reConfigMsgLen);

    /* Start filling the request structure */
    reconfigReq_p->msgId = PHY_CELL_CONFIG_REQUEST;

  /*coverity 10512*/
    FapiCellReConfigReq_p = getPhyCellReConfigReqPtr(reconfigReq_p);
  /*coverity 10512*/

    /* Index - 0 */
    FapiCellReConfigReq_p->numOfTlv = numOfTlv; 
    /*coverity 57872*/
     /*SPR_8966_FIX_START*/
    reconfigReq_p->msgLen= MAC_PHY_CONVERT_16(offsetof(FAPI_phyCellReConfigRequest_st,configtlvs) +
            (FapiCellReConfigReq_p->numOfTlv * sizeof(FAPI_cellConfig_st)));
     /*SPR_8966_FIX_END*/
    /*coverity 57872*/
    if (reconfigPhyCell_g[internalCellIndex].reconfigPdschConfigCommonInfo.updateFlag == TRUE)
    {
        FapiCellReConfigReq_p->configtlvs[tlvIndex].tag = 
            FAPI_REFERENCE_SIGNAL_POWER;
        FapiCellReConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[tlvIndex].configParam.refSignalPower = 
            MAC_PHY_CONVERT_16(reconfigPhyCell_g[internalCellIndex].\
                    reconfigPdschConfigCommonInfo.refSignalPower);
        tlvIndex++; 

        /*CLPC Change*/
        FapiCellReConfigReq_p->configtlvs[tlvIndex].tag = FAPI_P_B;
        FapiCellReConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[tlvIndex].configParam.pB = 
            MAC_PHY_CONVERT_16(reconfigPhyCell_g[internalCellIndex].\
                    reconfigPdschConfigCommonInfo.pB);
        tlvIndex++;
    }

    if (reconfigPhyCell_g[internalCellIndex].\
            reconfigCommonDLPowerControlInfo.updateFlagPcfich == TRUE)
    {
        FapiCellReConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PCFICH_POWER_OFFSET;
        FapiCellReConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[tlvIndex].configParam.pcfichPowerOffset =
            MAC_PHY_CONVERT_16(reconfigPhyCell_g[internalCellIndex].\
                    reconfigCommonDLPowerControlInfo.pcfichPowerOffset);
        tlvIndex++;
    }

    if (reconfigPhyCell_g[internalCellIndex].\
            reconfigCommonDLPowerControlInfo.updateFlagPhich == TRUE)
    {
        FapiCellReConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PHICH_POWER_OFFSET;
        FapiCellReConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[tlvIndex].configParam.phichPowOffset =
            MAC_PHY_CONVERT_16(reconfigPhyCell_g[internalCellIndex].\
                    reconfigCommonDLPowerControlInfo.phichPowerOffset);
        tlvIndex++;
    }
    if(reconfigPhyCell_g[internalCellIndex].syncSignal.\
            updatePrimarySignalFlag == TRUE)
    {
        FapiCellReConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PRIMARY_SYNC_SIGNAL;
        FapiCellReConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[tlvIndex].configParam.primarySignal = 
            MAC_PHY_CONVERT_16(reconfigPhyCell_g[internalCellIndex].\
                    syncSignal.primarySignal);
        tlvIndex++;

    }
    if(reconfigPhyCell_g[internalCellIndex].syncSignal.\
            updateSecondarySignalFlag == TRUE)
    {
        FapiCellReConfigReq_p->configtlvs[tlvIndex].tag = FAPI_SECONDARY_SYNC_SIGNAL;
        FapiCellReConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[tlvIndex].configParam.secondarySignal = 
            MAC_PHY_CONVERT_16(reconfigPhyCell_g[internalCellIndex].\
                    syncSignal.secondarySignal);
        tlvIndex++;

    }

    /*CLPC Change End*/
    /* + PRS_CHANGES */
/* SPR 19288 change start */
    if(TRUE == reconfigPhyCell_g[internalCellIndex].prsPhyConfig.updateFlag)
    {
        FapiCellReConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PRS_BANDWIDTH;
        FapiCellReConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[tlvIndex].configParam.prsBandWidth = 
            MAC_PHY_CONVERT_16(reconfigPhyCell_g[internalCellIndex].\
                    prsPhyConfig.prsBandWidth);
        tlvIndex++;

        FapiCellReConfigReq_p->configtlvs[tlvIndex].tag = FAPI_PRS_CYCLIC_PREFIX;
        FapiCellReConfigReq_p->configtlvs[tlvIndex].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[tlvIndex].configParam.prsCyclicPrifix = 
            MAC_PHY_CONVERT_16(reconfigPhyCell_g[internalCellIndex].\
                    prsPhyConfig.prsCyclicPrifix);
/* SPR 19288 change end */
        tlvIndex++;
    }
    /* - PRS_CHANGES */
    /* Index - 1 */
    FapiCellReConfigReq_p->configtlvs[tlvIndex].tag = FAPI_SFN_SF;
    FapiCellReConfigReq_p->configtlvs[tlvIndex].tagLen = 2; 

    /* calculating value for sfn sf for UInt16 */
    sfnsf = GENERATE_SUBFRAME_SFNSF(reconfigPhyCell_g[internalCellIndex].targetSfn, 
            reconfigPhyCell_g[internalCellIndex].targetSf);

    FapiCellReConfigReq_p->configtlvs[tlvIndex].configParam.sfnsf = 
        MAC_PHY_CONVERT_16(sfnsf);
    tlvIndex++;

    *reconfigReq_pp = reconfigReq_p;
    *msgLen_p = reConfigMsgLen;
    return MAC_SUCCESS;
}
/* CA Changes end */


/* Cell Start Stop Chg*/
/*****************************************************************************
 * Function Name  : fillPhyReConfigRequestAfterCellStop 
 * Inputs         : reconfigReq_pp - Cell Reconfig structure that is filled after parsing 
 *                  Phy cell reconfiguration request received from RRC for PHY,
 *                  msgLen_p and numOfTlv
 *                  internalCellIndex - cell id  
 * Outputs        : Message structure for Phy Cell reconfig request on
 *                  MAC - PHY interface (FAPI_l1ApiMsg_st)
 * Returns        : MAC_FAILURE/MAC_SUCCESS 
 * Description    : This function is used by MAC to fill Phy Cell 
 *                  Reconfig request(PHY_CELL_RECONFIG_REQ which comes after 
 *                  Cell Stop procedure)that needs to be sent to PHY interface.
 *****************************************************************************/
MacRetType fillPhyReConfigRequestAfterCellStop(FAPI_l1ApiMsg_st **reconfigReq_pp,
        UInt16  *msgLen_p, UInt8 numOfTlv,
        InternalCellIndex internalCellIndex) 
{
    UInt8 tlvIndex = 0;
    UInt16 reConfigMsgLen = 0;
    FAPI_l1ApiMsg_st *reconfigReq_p = PNULL;
    FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p = PNULL;
    reConfigMsgLen = sizeof(FAPI_l1ApiMsg_st) +
        sizeof(FAPI_phyCellConfigRequest_st) + ( (numOfTlv - 1) *
                sizeof(FAPI_cellConfig_st) ) - 2; 
    /* -2 is for 1-1 byte memory taken by the stretchable arrays */
    GET_MEM_FROM_POOL(FAPI_l1ApiMsg_st,reconfigReq_p, reConfigMsgLen, NULL);
    /* COVERITY CID 24429 FIX START */
    if ( !reconfigReq_p )
    {
        return MAC_FAILURE;
    }
    /* COVERITY CID 24429 FIX END */


    memSet(reconfigReq_p, 0, reConfigMsgLen);

    /* Start filling the request structure */
    reconfigReq_p->msgId = PHY_CELL_CONFIG_REQUEST;
    reconfigReq_p->msgLen = MAC_PHY_CONVERT_16(sizeof(FAPI_phyCellConfigRequest_st) +
        (numOfTlv * sizeof(FAPI_cellConfig_st)));
    /*coverity 10513*/
    FapiCellReConfigReq_p = getPhyCellConfigReqPtr(reconfigReq_p);
    /*coverity 10513*/
    

    FapiCellReConfigReq_p->numOfTlv = numOfTlv;

    /* Cyclomatic_complexity_changes_start */

    fillPhyReConfigRequestForDuplexingMode(FapiCellReConfigReq_p,
		    &tlvIndex,
		    internalCellIndex);

    fillPhyReConfigRequestForPbFlag(FapiCellReConfigReq_p,
		    &tlvIndex,
		    internalCellIndex);

    fillPhyReConfigRequestForDlCyclicPrefixTypeFlag(FapiCellReConfigReq_p,
		    &tlvIndex,
		    internalCellIndex);

    fillPhyReConfigRequestForUlCyclicPrefixTypeFlag(FapiCellReConfigReq_p,
		    &tlvIndex,
		    internalCellIndex);

    fillPhyReConfigRequestForDlChannelBWFlag(FapiCellReConfigReq_p,
		    &tlvIndex,
		    internalCellIndex);

    fillPhyReConfigRequestForUlChannelBWFlag(FapiCellReConfigReq_p,
		    &tlvIndex,
		    internalCellIndex);

    fillPhyReConfigRequestForTxAntennaPortFlag(FapiCellReConfigReq_p,
		    &tlvIndex,
		    internalCellIndex);

    fillPhyReConfigRequestForRxAntennaPortFlag(FapiCellReConfigReq_p,
		    &tlvIndex,
		    internalCellIndex);

    fillPhyReConfigRequestForPhichResourceFlag(FapiCellReConfigReq_p,
		    &tlvIndex,
		    internalCellIndex);

    fillPhyReConfigRequestForPhichDuration(FapiCellReConfigReq_p,
		    &tlvIndex,
		    internalCellIndex);

    fillPhyReConfigRequestForPrimarySyncSignalFlag(FapiCellReConfigReq_p,
		    &tlvIndex,
		    internalCellIndex);

    fillPhyReConfigRequestForSecondarySyncSignalFlag(FapiCellReConfigReq_p,
		    &tlvIndex,
		    internalCellIndex);

    fillPhyReConfigRequestForPhysicalCellIdFlag(FapiCellReConfigReq_p,
		    &tlvIndex,
             internalCellIndex);

    fillPhyReConfigRequestForConfigurationIndexFlag(FapiCellReConfigReq_p,
             &tlvIndex,
             internalCellIndex);

    fillPhyReConfigRequestForRootSeqIndexFlag(FapiCellReConfigReq_p,
             &tlvIndex,
             internalCellIndex);

    fillPhyReConfigRequestForZeroCorelationZoneConfigFlag(FapiCellReConfigReq_p,
             &tlvIndex,
             internalCellIndex);

    fillPhyReConfigRequestForHighSpeedFlag(FapiCellReConfigReq_p,
             &tlvIndex,
             internalCellIndex);
 
    fillPhyReConfigRequestForFreqOffsetFlag(FapiCellReConfigReq_p,
             &tlvIndex,
             internalCellIndex);

    fillPhyReConfigRequestForHoppingModeFlag(FapiCellReConfigReq_p,
             &tlvIndex,
             internalCellIndex);

    fillPhyReConfigRequestForHoppingOffsetFlag(FapiCellReConfigReq_p,
             &tlvIndex,
             internalCellIndex);

    fillPhyReConfigRequestForNumOfSubBandFlag(FapiCellReConfigReq_p,
             &tlvIndex,
             internalCellIndex); 

    fillPhyReConfigRequestForDeltaPUCCHShiftFlag(FapiCellReConfigReq_p,
             &tlvIndex,
             internalCellIndex);

    fillPhyReConfigRequestForNCQIRBFlag(FapiCellReConfigReq_p,
            &tlvIndex,
            internalCellIndex);

    fillPhyReConfigRequestForNAnCsFlag(FapiCellReConfigReq_p,
		    &tlvIndex,
		    internalCellIndex);

    fillPhyReConfigRequestForN1PucchAnFlag(FapiCellReConfigReq_p,
		    &tlvIndex,
		    internalCellIndex);

    fillPhyReConfigRequestForBandWidthConfigurationFlag(FapiCellReConfigReq_p,
		    &tlvIndex,
		    internalCellIndex);

    fillPhyReConfigRequestForPrsPhyConfigUpdateFlag(FapiCellReConfigReq_p,
		    &tlvIndex,
		    internalCellIndex);
    /* Cyclomatic_complexity_changes_end */

    /* - PRS_CHANGES */
    *reconfigReq_pp = reconfigReq_p;
    *msgLen_p = reConfigMsgLen;
    return MAC_SUCCESS;
}
/* Cell Start Stop Chg */
/* CA Changes end */  



/*Cyclomatic Complexity changes - starts here */
/*****************************************************************************
 * Function Name  : prepareUlConfigPdu 
 * Inputs         : ulPduConf_p - pointer to structure FAPI_ulPDUConfigInfo_st,
 *                  ulSchUciInfo_p - pointer to ULSchUciInfo,
 *                  pduSize - size of PDU,
 *                  isSRSPresent - chck for SRS,
 *                  countInvalid_p
 *                  internalCellIndex
 * Outputs        : None
 * Returns        : pduSize
 * Description    : This function will prepare the ul Config depending upon the PDU.
 *****************************************************************************/

 UInt16 prepareUlConfigPdu(FAPI_ulPDUConfigInfo_st *ulPduConf_p,
                        ULSchUciInfo *ulSchUciInfo_p,
                        UInt16 pduSize,
                        UInt8 isSRSPresent,
                        UInt8 *countInvalid,
                        InternalCellIndex internalCellIndex)
{
	/* SPR 11457 03062014 */
	RNTIInfo * rntiInfo_p = NULL;
    DLUEContext *dlUECtx_p = NULL;  
    rntiInfo_p = getUeIdxFromRNTIMap(ulSchUciInfo_p->rnti, internalCellIndex);
    /* SPR 11457 03062014 */
    MacRetType  ret_val=MAC_FAILURE;

#if 0
	if((ulSchUciInfo_p->pduType==MAC_SS_PDU_ULSCH_HARQ)&&(ulSchUciInfo_p->ulHarqInfo_p==NULL))
	{
	    if(ulSchUciInfo_p->ueScheduledInfo_p==NULL)
	    {
	        ulSchUciInfo_p->pduType=MAC_SS_PDU_UCI_HARQ;
			fprintf(stderr,"fixed pdyType to UCI_HARQ\n");
	    }
		else
		{
		    fprintf(stderr,"ueScheduledInfo_p is NULL\n");
		}
	}
#endif	
    switch (ulSchUciInfo_p->pduType)
    {
        case MAC_SS_PDU_ULSCH:
            {
             /*coverity 62546 FIX START <Nesting Indnet mismatch>*/
                if (MAC_SUCCESS == (*prepareUlschPduFunction)(
                            (FAPI_ulSCHPduInfo_st *)ulPduConf_p->ulPduConfigInfo,
                            ulSchUciInfo_p
                            /*SPR 1547 Start*/
                            ,isSRSPresent
                            /*SPR 1547 End*/
                        ,internalCellIndex
                            ))
             /*coverity 62546 FIX END <Nesting Indnet mismatch>*/
                {

                    ulPduConf_p->ulConfigPduSize = sizeof(FAPI_ulSCHPduInfo_st) +
                        sizeof(FAPI_ulPDUConfigInfo_st) -1;
                    pduSize += ulPduConf_p->ulConfigPduSize;
                    ulPduConf_p->ulConfigPduType = FAPI_ULSCH;

#ifdef TDD_CONFIG
                    if(PNULL != (ulSchUciInfo_p->ueScheduledInfo_p))
                    {
                        (ulSchUciInfo_p->ueScheduledInfo_p)->ulschUciInfoIndex = 
                            ULSCH_UCI_INVALID_INDEX;
                        ulSchUciInfo_p->ueScheduledInfo_p= PNULL;            
                    }
#elif FDD_CONFIG
                    if(PNULL != (ulSchUciInfo_p->ueDirectIndexInfo_p))
                    {
                        (ulSchUciInfo_p->ueDirectIndexInfo_p)->ulschUciInfoIndex = 
                            ULSCH_UCI_INVALID_INDEX;
						/* SPR 14877 SRS Changes */
						ulSchUciInfo_p->ueDirectIndexInfo_p->ulschInfoIndex = ULSCH_UCI_INVALID_INDEX ;
						/* SPR 14877 SRS Changes */
                        ulSchUciInfo_p->ueDirectIndexInfo_p = PNULL;            
                    }
#endif

                }
                /* + SPR_15091 */
                else
                {
                   //pduSize = 0;
                    (*countInvalid)++;
                   //fprintf(stderr,"Info Msg %d dropped at %s %d \n",ulSchUciInfo_p->pduType,__func__,__LINE__);
                }    
                /* - SPR_15091 */
                break;
            }
        case MAC_SS_PDU_ULSCH_HARQ:
            {
                if (MAC_SUCCESS == prepareUlschHarqPdu(
                            (FAPI_ulSCHHarqPduInfo_st *)ulPduConf_p->ulPduConfigInfo,
                            ulSchUciInfo_p
                            /*SPR 1547 Start*/
                            ,isSRSPresent
                            /*SPR 1547 End*/
                        ,internalCellIndex
                            ))
                {
                    ulPduConf_p->ulConfigPduSize = sizeof(FAPI_ulSCHHarqPduInfo_st)+
                        sizeof(FAPI_ulPDUConfigInfo_st) -1;
                    pduSize += ulPduConf_p->ulConfigPduSize;
                    ulPduConf_p->ulConfigPduType = FAPI_ULSCH_HARQ;
#ifdef TDD_CONFIG
                    if(PNULL != (ulSchUciInfo_p->ueScheduledInfo_p))
                    {
                        (ulSchUciInfo_p->ueScheduledInfo_p)->ulschUciInfoIndex = 
                            ULSCH_UCI_INVALID_INDEX;
                        ulSchUciInfo_p->ueScheduledInfo_p= PNULL;            
                    }
#elif FDD_CONFIG
                    if(PNULL != (ulSchUciInfo_p->ueDirectIndexInfo_p))
                    {
                        (ulSchUciInfo_p->ueDirectIndexInfo_p)->ulschUciInfoIndex = 
                            ULSCH_UCI_INVALID_INDEX;
						/* SPR 14877 SRS Changes */
						ulSchUciInfo_p->ueDirectIndexInfo_p->ulschInfoIndex = ULSCH_UCI_INVALID_INDEX ;
						/* SPR 14877 SRS Changes */
                        ulSchUciInfo_p->ueDirectIndexInfo_p = PNULL;            
                    }
#endif

                }
                /* + SPR_15091 */
                else
                {
                   //pduSize = 0;
                   /* SPR 21514 fix start */
                   (*countInvalid)++;
                   /* SPR 21514 fix end */
                   //fprintf(stderr,"Info Msg %d dropped at %s %d \n",ulSchUciInfo_p->pduType,__func__,__LINE__);
                }    
                /* - SPR_15091 */
                break;
            }
        case MAC_SS_PDU_ULSCH_CQI_RI:
            {
            ret_val=(*prepareUlschCqiRiPduFunction)(
                    ulPduConf_p,
                            ulSchUciInfo_p
                            /*SPR 1547 changes Start*/
                    ,isSRSPresent,
                            /*SPR 1547 changes End*/            
                    internalCellIndex
                    );
                /* + Coverity_62546 Changes */
                if(ret_val == MAC_SUCCESS)
                {
                /* - Coverity_62546 Changes */
                    pduSize += ulPduConf_p->ulConfigPduSize;
                    ulPduConf_p->ulConfigPduType = FAPI_ULSCH_CQI_RI;
#ifdef TDD_CONFIG
                    if(PNULL != (ulSchUciInfo_p->ueScheduledInfo_p))
                    {
                        (ulSchUciInfo_p->ueScheduledInfo_p)->ulschUciInfoIndex = 
                            ULSCH_UCI_INVALID_INDEX;
                        ulSchUciInfo_p->ueScheduledInfo_p= PNULL;            
                    }
#elif FDD_CONFIG
                    if(PNULL != (ulSchUciInfo_p->ueDirectIndexInfo_p))
                    {
                        (ulSchUciInfo_p->ueDirectIndexInfo_p)->ulschUciInfoIndex = 
                            ULSCH_UCI_INVALID_INDEX;
						/* SPR 14877 SRS Changes */
						ulSchUciInfo_p->ueDirectIndexInfo_p->ulschInfoIndex = ULSCH_UCI_INVALID_INDEX ;
						/* SPR 14877 SRS Changes */
                        ulSchUciInfo_p->ueDirectIndexInfo_p = PNULL;            
                    }
#endif

                }
                /* + SPR_15091 */
                else
                {
                   //pduSize = 0;
                   /* SPR 21514 fix start */
                   (*countInvalid)++;
                   //fprintf(stderr,"Info Msg %d dropped at %s %d \n",ulSchUciInfo_p->pduType,__func__,__LINE__);
                   /* SPR 21514 fix end */

                }    
                /* - SPR_15091 */
                break;
            }

        case MAC_SS_PDU_ULSCH_CQI_HARQ_RI:
             /*coverity 62546 FIX START <Nesting Indnet mismatch>*/
            {
                if(MAC_SUCCESS == (*prepareUlschCqiHarqRiPduFunction)(
                            ulPduConf_p,
                            ulSchUciInfo_p
                            /*SPR 1547 Start*/
                            ,isSRSPresent
                            /*SPR 1547 End*/
                            ,internalCellIndex
                            ))
                {
                    pduSize += ulPduConf_p->ulConfigPduSize;
                    ulPduConf_p->ulConfigPduType = FAPI_ULSCH_CQI_HARQ_RI;
#ifdef TDD_CONFIG
                    if(PNULL != (ulSchUciInfo_p->ueScheduledInfo_p))
                    {
                        (ulSchUciInfo_p->ueScheduledInfo_p)->ulschUciInfoIndex = 
                            ULSCH_UCI_INVALID_INDEX;
                        ulSchUciInfo_p->ueScheduledInfo_p= PNULL;            
                    }
#elif FDD_CONFIG
                    if(PNULL != (ulSchUciInfo_p->ueDirectIndexInfo_p))
                    {
                        (ulSchUciInfo_p->ueDirectIndexInfo_p)->ulschUciInfoIndex = 
                            ULSCH_UCI_INVALID_INDEX;
						/* SPR 14877 SRS Changes */
						ulSchUciInfo_p->ueDirectIndexInfo_p->ulschInfoIndex = ULSCH_UCI_INVALID_INDEX ;
						/* SPR 14877 SRS Changes */
                        ulSchUciInfo_p->ueDirectIndexInfo_p = PNULL;            
                    }
#endif

                }
                /* + SPR_15091 */
                else
                {
                   //pduSize = 0;
                   /* SPR 21514 fix start */
                   (*countInvalid)++;
                   //fprintf(stderr,"Info Msg %d dropped at %s %d \n",ulSchUciInfo_p->pduType,__func__,__LINE__);
                   /* SPR 21514 fix end */
                }    
                /* - SPR_15091 */
                break;
            }
             /*coverity 62546 FIX END <Nesting Indnet mismatch>*/
            /*FDD_SRS*/
        case MAC_SS_PDU_UCI_SR:
            {
            		/* SPR 11457 03062014 */
                    /*Coverity ID <65220> Fix Changes Start*/
                    if(rntiInfo_p && (rntiInfo_p->rntiFlag != FREERNTI))
                    {
                        dlUECtx_p = dlUECtxInfoArr_g[rntiInfo_p->index].dlUEContext_p;
                    }
                    else
                    {
                        /* SPR 21514 fix start */
                        (*countInvalid)++;
                   //fprintf(stderr,"Info Msg %d dropped at %s %d \n",ulSchUciInfo_p->pduType,__func__,__LINE__);
                        /* SPR 21514 fix end */
                        return 0;
                    }
                    /*Coverity ID <65220> Fix Changes End*/
                    /* SPR 16988 Fix Start */
                    /*This is added as part of SRS feature. This is used by SRS
                      execution leg to check for PUCCH pdu types for collision
                      handling. This is added for all UCI type pdus*/
#ifdef TDD_CONFIG
                    if(PNULL != (ulSchUciInfo_p->ueScheduledInfo_p))
                    {
                        (ulSchUciInfo_p->ueScheduledInfo_p)->ulschUciInfoIndex = 
                            ULSCH_UCI_INVALID_INDEX;
                        ulSchUciInfo_p->ueScheduledInfo_p= PNULL;            
                    }
#elif FDD_CONFIG
                    if(PNULL != (ulSchUciInfo_p->ueDirectIndexInfo_p))
                    {
                        (ulSchUciInfo_p->ueDirectIndexInfo_p)->ulschUciInfoIndex = 
                            ULSCH_UCI_INVALID_INDEX;
                        /* SPR 14877 SRS Changes */
                        ulSchUciInfo_p->ueDirectIndexInfo_p->ulschInfoIndex
                            = ULSCH_UCI_INVALID_INDEX ;
                        /* SPR 14877 SRS Changes */
                        ulSchUciInfo_p->ueDirectIndexInfo_p = PNULL;            
                    }
#endif
                    if(1 == dlUECtx_p->tcrntiFlag)
                    {
                        (*countInvalid)++;
                   //fprintf(stderr,"Info Msg %d dropped at %s %d \n",ulSchUciInfo_p->pduType,__func__,__LINE__);
                    }
                    else
                    {
                        prepareUciSrPdu(
                                (FAPI_uciSrPduInfo_st *)ulPduConf_p->ulPduConfigInfo,
                                ulSchUciInfo_p,internalCellIndex
                                );
                        ulPduConf_p->ulConfigPduSize = sizeof(FAPI_uciSrPduInfo_st)+
                            sizeof(FAPI_ulPDUConfigInfo_st) -1;
                        pduSize += ulPduConf_p->ulConfigPduSize;
                        ulPduConf_p->ulConfigPduType = FAPI_UCI_SR;
                    }
                    /* SPR 11457 03062014 */
                    break;
            }
        case MAC_SS_PDU_UCI_HARQ:
            {
                /* SPR 16988 Fix Start */
#ifdef TDD_CONFIG
                if(PNULL != (ulSchUciInfo_p->ueScheduledInfo_p))
                {
                    (ulSchUciInfo_p->ueScheduledInfo_p)->ulschUciInfoIndex = 
                        ULSCH_UCI_INVALID_INDEX;
                    ulSchUciInfo_p->ueScheduledInfo_p= PNULL;            
                }
#elif FDD_CONFIG
                if(PNULL != (ulSchUciInfo_p->ueDirectIndexInfo_p))
                {
                    (ulSchUciInfo_p->ueDirectIndexInfo_p)->ulschUciInfoIndex = 
                        ULSCH_UCI_INVALID_INDEX;
						/* SPR 14877 SRS Changes */
                    ulSchUciInfo_p->ueDirectIndexInfo_p->ulschInfoIndex = ULSCH_UCI_INVALID_INDEX ;
						/* SPR 14877 SRS Changes */
                    ulSchUciInfo_p->ueDirectIndexInfo_p = PNULL;            
                }
#endif
                (*prepareUciHarqPduFunction  )(
                        ulPduConf_p,
                        ulSchUciInfo_p
                        ,internalCellIndex
                        );
                pduSize += ulPduConf_p->ulConfigPduSize;
                ulPduConf_p->ulConfigPduType = FAPI_UCI_HARQ;
                /* SPR 16988 Fix End */
                break;
            }
        case MAC_SS_PDU_UCI_SR_HARQ:
            {		
                /* SPR 11457 03062014 */
                /*Coverity ID <65220> Fix Changes Start*/
                if(rntiInfo_p && (rntiInfo_p->rntiFlag != FREERNTI))
                {
                    dlUECtx_p = dlUECtxInfoArr_g[rntiInfo_p->index].dlUEContext_p;
                }
                else
                {
                   /* SPR 21514 fix start */
                   (*countInvalid)++;
                   //fprintf(stderr,"Info Msg %d dropped at %s %d \n",ulSchUciInfo_p->pduType,__func__,__LINE__);
                   /* SPR 21514 fix end */
                    return 0;
                }
                /*Coverity ID <65220> Fix Changes End*/
                /* SPR 16988 Fix Start */
#ifdef TDD_CONFIG
                if(PNULL != (ulSchUciInfo_p->ueScheduledInfo_p))
                {
                    (ulSchUciInfo_p->ueScheduledInfo_p)->ulschUciInfoIndex = 
                        ULSCH_UCI_INVALID_INDEX;
                    ulSchUciInfo_p->ueScheduledInfo_p= PNULL;            
                }
#elif FDD_CONFIG
                if(PNULL != (ulSchUciInfo_p->ueDirectIndexInfo_p))
                {
                    (ulSchUciInfo_p->ueDirectIndexInfo_p)->ulschUciInfoIndex = 
                        ULSCH_UCI_INVALID_INDEX;
                    /* SPR 14877 SRS Changes */
                    ulSchUciInfo_p->ueDirectIndexInfo_p->ulschInfoIndex = ULSCH_UCI_INVALID_INDEX ;
                    /* SPR 14877 SRS Changes */
                    ulSchUciInfo_p->ueDirectIndexInfo_p = PNULL;            
                }
#endif
                if(1 == dlUECtx_p->tcrntiFlag)
                {
					(*countInvalid)++;
                   //fprintf(stderr,"Info Msg %d dropped at %s %d \n",ulSchUciInfo_p->pduType,__func__,__LINE__);
                }
                else
                {
                    (*prepareUciSrHarqPduFunction)(
                            ulPduConf_p,
                            ulSchUciInfo_p,
                            internalCellIndex
                            );
                    pduSize += ulPduConf_p->ulConfigPduSize;
                    ulPduConf_p->ulConfigPduType = FAPI_UCI_SR_HARQ;
                }
                /* SPR 11457 03062014 */
             /* SPR 16988 Fix End */
                break;
            }
        case MAC_SS_PDU_UCI_CQI:
            {
                /* SPR 11457 03062014 */
                /*Coverity ID <65220> Fix Changes Start*/
                if(rntiInfo_p && (rntiInfo_p->rntiFlag != FREERNTI))
                {
                    dlUECtx_p = dlUECtxInfoArr_g[rntiInfo_p->index].dlUEContext_p;
                }
                else
                {
                   /* SPR 21514 fix start */
                   (*countInvalid)++;
                   //fprintf(stderr,"Info Msg %d dropped at %s %d \n",ulSchUciInfo_p->pduType,__func__,__LINE__);
                   /* SPR 21514 fix end */
                    return 0;
                }
                /*Coverity ID <65220> Fix Changes End*/
                /* SPR 16988 Fix Start */
#ifdef TDD_CONFIG
                if(PNULL != (ulSchUciInfo_p->ueScheduledInfo_p))
                {
                    (ulSchUciInfo_p->ueScheduledInfo_p)->ulschUciInfoIndex = 
                        ULSCH_UCI_INVALID_INDEX;
                    ulSchUciInfo_p->ueScheduledInfo_p= PNULL;            
                }
#elif FDD_CONFIG
                if(PNULL != (ulSchUciInfo_p->ueDirectIndexInfo_p))
                {
                    (ulSchUciInfo_p->ueDirectIndexInfo_p)->ulschUciInfoIndex = 
                        ULSCH_UCI_INVALID_INDEX;
                    /* SPR 14877 SRS Changes */
                    ulSchUciInfo_p->ueDirectIndexInfo_p->ulschInfoIndex = ULSCH_UCI_INVALID_INDEX ;
                    /* SPR 14877 SRS Changes */
                    ulSchUciInfo_p->ueDirectIndexInfo_p = PNULL;            
                }
#endif
                if(1 == dlUECtx_p->tcrntiFlag)
                {
					(*countInvalid)++;
                }
                else
                {
                    prepareUciCqiPdu(
                            (FAPI_uciCqiPduInfo_st *)ulPduConf_p->ulPduConfigInfo,
                            ulSchUciInfo_p
                            ,internalCellIndex
                            );
                    ulPduConf_p->ulConfigPduSize = sizeof(FAPI_uciCqiPduInfo_st)+
                        sizeof(FAPI_ulPDUConfigInfo_st) -1;
                    pduSize += ulPduConf_p->ulConfigPduSize;
                    ulPduConf_p->ulConfigPduType = FAPI_UCI_CQI;
                }
                /* SPR 11457 03062014 */
                /* SPR 16988 Fix End */
                break;
            }
        case MAC_SS_PDU_UCI_CQI_HARQ:
            {	
                /* SPR 11457 03062014 */
                /*Coverity ID <65220> Fix Changes Start*/
                if(rntiInfo_p && (rntiInfo_p->rntiFlag != FREERNTI))
                {
                    dlUECtx_p = dlUECtxInfoArr_g[rntiInfo_p->index].dlUEContext_p;
                }
                else
                {
                   /* SPR 21514 fix start */
                   (*countInvalid)++;
                   //fprintf(stderr,"Info Msg %d dropped at %s %d \n",ulSchUciInfo_p->pduType,__func__,__LINE__);
                   /* SPR 21514 fix end */
                    return 0;
                }
                /*Coverity ID <65220> Fix Changes End*/
                /* SPR 16988 Fix Start */
#ifdef TDD_CONFIG
                if(PNULL != (ulSchUciInfo_p->ueScheduledInfo_p))
                {
                    (ulSchUciInfo_p->ueScheduledInfo_p)->ulschUciInfoIndex = 
                        ULSCH_UCI_INVALID_INDEX;
                    ulSchUciInfo_p->ueScheduledInfo_p= PNULL;            
                }
#elif FDD_CONFIG
                if(PNULL != (ulSchUciInfo_p->ueDirectIndexInfo_p))
                {
                    (ulSchUciInfo_p->ueDirectIndexInfo_p)->ulschUciInfoIndex = 
                        ULSCH_UCI_INVALID_INDEX;
                    /* SPR 14877 SRS Changes */
                    ulSchUciInfo_p->ueDirectIndexInfo_p->ulschInfoIndex = ULSCH_UCI_INVALID_INDEX ;
                    /* SPR 14877 SRS Changes */
                    ulSchUciInfo_p->ueDirectIndexInfo_p = PNULL;            
                }
#endif
                if(1 == dlUECtx_p->tcrntiFlag)
                {
					(*countInvalid)++;
                   //fprintf(stderr,"Info Msg %d dropped at %s %d \n",ulSchUciInfo_p->pduType,__func__,__LINE__);
                }
                else
                {
                    (* prepareUciCqiHarqPduFunction)(
                            ulPduConf_p,
                            ulSchUciInfo_p
                            ,internalCellIndex
                            );
                    /* + SPR 12782 Changes */
                    /*ulPduConf_p->ulConfigPduSize = sizeof(FAPI_uciCqiHarqPduInfo_st)+
                        sizeof(FAPI_ulPDUConfigInfo_st) -1;*/
                    /* - SPR 12782 Changes */
                    pduSize += ulPduConf_p->ulConfigPduSize;
                    ulPduConf_p->ulConfigPduType = FAPI_UCI_CQI_HARQ;
                }
                /* SPR 16988 Fix End */
                /* SPR 11457 03062014 */
                break;
            }
        case MAC_SS_PDU_UCI_CQI_SR:
            {	
                /* SPR 11457 03062014 */
                /*Coverity ID <65220> Fix Changes Start*/
                if(rntiInfo_p && (rntiInfo_p->rntiFlag != FREERNTI))
                {
                    dlUECtx_p = dlUECtxInfoArr_g[rntiInfo_p->index].dlUEContext_p;
                }
                else
                {
                   /* SPR 21514 fix start */
                   (*countInvalid)++;
                   //fprintf(stderr,"Info Msg %d dropped at %s %d \n",ulSchUciInfo_p->pduType,__func__,__LINE__);
                   /* SPR 21514 fix end */
                    return 0;
                }
                /*Coverity ID <65220> Fix Changes End*/
                /* SPR 16988 Fix Start */
#ifdef TDD_CONFIG
                if(PNULL != (ulSchUciInfo_p->ueScheduledInfo_p))
                {
                    (ulSchUciInfo_p->ueScheduledInfo_p)->ulschUciInfoIndex = 
                        ULSCH_UCI_INVALID_INDEX;
                    ulSchUciInfo_p->ueScheduledInfo_p= PNULL;            
                }
#elif FDD_CONFIG
                if(PNULL != (ulSchUciInfo_p->ueDirectIndexInfo_p))
                {
                    (ulSchUciInfo_p->ueDirectIndexInfo_p)->ulschUciInfoIndex = 
                        ULSCH_UCI_INVALID_INDEX;
                    /* SPR 14877 SRS Changes */
                    ulSchUciInfo_p->ueDirectIndexInfo_p->ulschInfoIndex = ULSCH_UCI_INVALID_INDEX ;
                    /* SPR 14877 SRS Changes */

                    ulSchUciInfo_p->ueDirectIndexInfo_p = PNULL;            
                }
#endif
                if(1 == dlUECtx_p->tcrntiFlag)
                {
					(*countInvalid)++;
                   //fprintf(stderr,"Info Msg %d dropped at %s %d \n",ulSchUciInfo_p->pduType,__func__,__LINE__);
                }
                else
                {
                    prepareUciCqiSrPdu(
                            (FAPI_uciCqiSrPduInfo_st *)ulPduConf_p->ulPduConfigInfo,
                            ulSchUciInfo_p
                            ,internalCellIndex
                            );
                    ulPduConf_p->ulConfigPduSize = sizeof(FAPI_uciCqiSrPduInfo_st)+
                        sizeof(FAPI_ulPDUConfigInfo_st) -1;
                    pduSize += ulPduConf_p->ulConfigPduSize;
                    ulPduConf_p->ulConfigPduType = FAPI_UCI_CQI_SR;
                }
                /* SPR 16988 Fix End */
                /* SPR 11457 03062014 */
                break;
            } 
        case MAC_SS_PDU_UCI_CQI_SR_HARQ:
            {
                /* SPR 11457 03062014 */
                /*Coverity ID <65220> Fix Changes Start*/
                if(rntiInfo_p && (rntiInfo_p->rntiFlag != FREERNTI))
                {
                    dlUECtx_p = dlUECtxInfoArr_g[rntiInfo_p->index].dlUEContext_p;
                }
                else
                {
                   /* SPR 21514 fix start */
                   (*countInvalid)++;
                   //fprintf(stderr,"Info Msg %d dropped at %s %d \n",ulSchUciInfo_p->pduType,__func__,__LINE__);
                   /* SPR 21514 fix end */
                    return 0;
                }
                /* SPR 16988 Fix Start */
                /*Coverity ID <65220> Fix Changes End*/
#ifdef TDD_CONFIG
                if(PNULL != (ulSchUciInfo_p->ueScheduledInfo_p))
                {
                    (ulSchUciInfo_p->ueScheduledInfo_p)->ulschUciInfoIndex = 
                        ULSCH_UCI_INVALID_INDEX;
                    ulSchUciInfo_p->ueScheduledInfo_p= PNULL;            
                }
#elif FDD_CONFIG
                if(PNULL != (ulSchUciInfo_p->ueDirectIndexInfo_p))
                {
                    (ulSchUciInfo_p->ueDirectIndexInfo_p)->ulschUciInfoIndex = 
                        ULSCH_UCI_INVALID_INDEX;
                    /* SPR 14877 SRS Changes */
                    ulSchUciInfo_p->ueDirectIndexInfo_p->ulschInfoIndex = ULSCH_UCI_INVALID_INDEX ;
                    /* SPR 14877 SRS Changes */

                    ulSchUciInfo_p->ueDirectIndexInfo_p = PNULL;            
                }
#endif
                if(1 == dlUECtx_p->tcrntiFlag)
                {
                    (*countInvalid)++;
                   //fprintf(stderr,"Info Msg %d dropped at %s %d \n",ulSchUciInfo_p->pduType,__func__,__LINE__);
                }
                else
                {
                    (* prepareUciCqiSrHarqPduFunction)(
                            ulPduConf_p,
                            ulSchUciInfo_p,
                            internalCellIndex
                            );
                    pduSize += ulPduConf_p->ulConfigPduSize;
                    ulPduConf_p->ulConfigPduType = FAPI_UCI_CQI_SR_HARQ;
                }
                /* SPR 16988 Fix End */
                /* SPR 11457 03062014 */
                break;
            }  
#if   defined(FAPI_1_1_COMPLIANCE) ||  defined(FAPI_4_0_COMPLIANCE)         
        case MAC_SS_PDU_HARQ_BUFFER_RELEASE:
            {
                prepareUlschHarqBufferPdu(
                        (FAPI_ulSCHHarqBufferReleasePduInfo_st *)ulPduConf_p->ulPduConfigInfo,
                        ulSchUciInfo_p
                    , internalCellIndex
                        );
                ulPduConf_p->ulConfigPduSize = sizeof(FAPI_ulSCHHarqBufferReleasePduInfo_st)+
                    sizeof(FAPI_ulPDUConfigInfo_st) -1;
                pduSize += ulPduConf_p->ulConfigPduSize;
                ulPduConf_p->ulConfigPduType = FAPI_HARQ_BUFFER;

                break;
            }
#endif
            /*FDD_SRS*/ 
            /* SPR 609 changes start */    
        case MAC_SS_PDU_INVALID:
        default:
            {
                LOG_MAC_WARNING( MAC_RRC_INF,"Invalid Value recieved\n" );
                (*countInvalid)++;
                 fprintf(stderr,"Info Msg %d dropped at %s %d \n",ulSchUciInfo_p->pduType,__func__,__LINE__);
                break;
            }         
            /* SPR 609 changes end */    
    }
    return pduSize;
}
/*Cyclomatic Complexity changes - starts here */
/*****************************************************************************
 * Function Name  : getulChannelBW 
 * Inputs         : ulChBw - The Uplink Channel Bandwidth
 *                  internalCellIndex
 * Outputs        : The value of Uplink Channel Bandwidth is stored in CellConfig_Phy 
 *                  (CellConfigPhy_g.rfConfig)   
 * Returns        : None 
 * Description    : This function is used by MAC to fill Uplink Channel Bandwidth 
 *                  in RF Config for PHY  in (PHY_CELL_CONFIG_REQ).
 *****************************************************************************/
/* CA Changes start */
 void getulChannelBW(UInt8 ulChBw,
        InternalCellIndex internalCellIndex)
{
    switch ( ulChBw )
    {
        case UL_TX_BANDWIDTH_6RB:
        cellConfigPhy_gp[internalCellIndex]->rfConfig.ulChannelBW = 
            FAPI_CHANNEL_BW_6RB;
        break;
        case UL_TX_BANDWIDTH_15RB:
        cellConfigPhy_gp[internalCellIndex]->rfConfig.ulChannelBW = 
            FAPI_CHANNEL_BW_15RB;
        break;
        case UL_TX_BANDWIDTH_25RB:
        cellConfigPhy_gp[internalCellIndex]->rfConfig.ulChannelBW = 
            FAPI_CHANNEL_BW_25RB;
        break;
        case UL_TX_BANDWIDTH_50RB:
        cellConfigPhy_gp[internalCellIndex]->rfConfig.ulChannelBW = 
            FAPI_CHANNEL_BW_50RB;
        break;
        case UL_TX_BANDWIDTH_75RB:
        cellConfigPhy_gp[internalCellIndex]->rfConfig.ulChannelBW = 
            FAPI_CHANNEL_BW_75RB;
        break;
        case UL_TX_BANDWIDTH_100RB:
        cellConfigPhy_gp[internalCellIndex]->rfConfig.ulChannelBW = 
            FAPI_CHANNEL_BW_100RB;
        break;
        default :
        LTE_MAC_UT_LOG(LOG_INFO, CELL_CONF,"Wrong value recevied of "
                "ulChannel BandWidth [%d]\n",ulChBw);
        break;
    }
}
/* CA Changes end */
/*****************************************************************************
 * Function Name  : getdlChannelBW
 * Inputs         : dlChBw - The Downlink Channel Bandwidth
 *                   internalCellIndex
 * Outputs        : The value of Downlink Channel Bandwidth is stored in CellConfig_Phy 
 *                  (CellConfigPhy_g.rfConfig)
 * Returns        : None 
 * Description    : This function is used by MAC to fill Downlink Channel Bandwidth 
 *                  in RF Config for PHY in (PHY_CELL_CONFIG_REQ).
 *****************************************************************************/
/* CA Changes start */
 void getdlChannelBW( UInt8 dlChBw, InternalCellIndex internalCellIndex)
{
    switch ( dlChBw )
    {
        case DL_TX_BANDWIDTH_6RB:
        cellConfigPhy_gp[internalCellIndex]->rfConfig.dlChannelBW =
            FAPI_CHANNEL_BW_6RB;
        break;
        case DL_TX_BANDWIDTH_15RB:
        cellConfigPhy_gp[internalCellIndex]->rfConfig.dlChannelBW = 
            FAPI_CHANNEL_BW_15RB;
        break;
        case DL_TX_BANDWIDTH_25RB:
        cellConfigPhy_gp[internalCellIndex]->rfConfig.dlChannelBW = 
            FAPI_CHANNEL_BW_25RB;
        break;
        case DL_TX_BANDWIDTH_50RB:
        cellConfigPhy_gp[internalCellIndex]->rfConfig.dlChannelBW = 
            FAPI_CHANNEL_BW_50RB;
        break;
        case DL_TX_BANDWIDTH_75RB:
        cellConfigPhy_gp[internalCellIndex]->rfConfig.dlChannelBW = 
            FAPI_CHANNEL_BW_75RB;
        break;
        case DL_TX_BANDWIDTH_100RB:
        cellConfigPhy_gp[internalCellIndex]->rfConfig.dlChannelBW = 
            FAPI_CHANNEL_BW_100RB;
        break;
        default :
        LTE_MAC_UT_LOG(LOG_INFO, CELL_CONF,"Wrong value recevied of "
                "dlChannel BandWidth [%d]\n",dlChBw);
        break;
    }

}
/* CA Changes end */
/**********************************************************************
 * Function Name  : fillPhyCellReconfigParams
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to fill PHY Reconfig global 
 *                  variable.
 **********************************************************************/
/* CA Changes start */
 void fillPhyCellReconfigParams (InternalCellIndex internalCellIndex)
{
    if(TRUE == reconfigCellMajor_g[internalCellIndex].seqHopEnable_flag)
    {            
        cellConfigPhy_gp[internalCellIndex]->puschConfig.seqHopEnable =
            reconfigCellMajor_g[internalCellIndex].seqHopEnable;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].cyclicShift_flag)
    {            
        cellConfigPhy_gp[internalCellIndex]->puschConfig.cyclicShift =
            reconfigCellMajor_g[internalCellIndex].cyclicShift;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].deltaPUCCHShift_flag)
    {            
        cellConfigPhy_gp[internalCellIndex]->pucchConfig.deltaPUCCHShift =
            reconfigCellMajor_g[internalCellIndex].deltaPUCCHShift;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].nCQIRB_flag)
    {            
        cellConfigPhy_gp[internalCellIndex]->pucchConfig.nCQIRB =
            reconfigCellMajor_g[internalCellIndex].nCQIRB;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].nAnCs_flag)
    {            
        cellConfigPhy_gp[internalCellIndex]->pucchConfig.nAnCs =
            reconfigCellMajor_g[internalCellIndex].nAnCs;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].n1PucchAn_flag)
    {            
        cellConfigPhy_gp[internalCellIndex]->pucchConfig.n1PucchAn =
            reconfigCellMajor_g[internalCellIndex].n1PucchAn;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].phichResource_flag)
    {            
        cellConfigPhy_gp[internalCellIndex]->phichConfig.phichResource =
            reconfigCellMajor_g[internalCellIndex].phichResource;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].phichDuration_flag)
    {            
        cellConfigPhy_gp[internalCellIndex]->phichConfig.phichDuration =
            reconfigCellMajor_g[internalCellIndex].phichDuration;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].refSignalPower_flag)
    {            
        cellConfigPhy_gp[internalCellIndex]->rfConfig.refSignalPower =
            reconfigCellMajor_g[internalCellIndex].refSignalPower;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].pb_flag)
    {            
        cellConfigPhy_gp[internalCellIndex]->pb =
            reconfigCellMajor_g[internalCellIndex].pb;            
    }

    if(TRUE == reconfigCellMajor_g[internalCellIndex].bandWidthConfiguration_flag)
    {            
        cellConfigPhy_gp[internalCellIndex]->srsConfig.bandWidthConfiguration =
            reconfigCellMajor_g[internalCellIndex].bandWidthConfiguration;            
    }

    /* + PRS_CHANGES */
    if(TRUE == reconfigCellMajor_g[internalCellIndex].prsPhyConfig.updateFlag)
    {            
        cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsBandWidth =
            reconfigCellMajor_g[internalCellIndex].prsPhyConfig.prsBandWidth;

        cellConfigPhy_gp[internalCellIndex]->prsPhyConfig.prsCyclicPrifix =
            reconfigCellMajor_g[internalCellIndex].prsPhyConfig.prsCyclicPrifix;
    }
    /* - PRS_CHANGES */

    return;
}
/* CA Changes end */
/*Cyclomatic Complexity changes - end here */
 
/* Cyclomatic_complexity_changes_start */ 
/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForDuplexingMode
 *Inputs         : internalCellIndex
                   tlvIndex
                   FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                 for duplexing mode.
 ***********************************************************************/
void fillPhyReConfigRequestForDuplexingMode(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex; 
     /* Index 0 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].duplexingMode_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = FAPI_DUPLEXING_MODE;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.duplexingMode =
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].duplexingMode);
        count++;
        *tlvIndex = count;
    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForPbFlag.
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                 for PbFlag.
 ********************************************************************/

void fillPhyReConfigRequestForPbFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
     UInt8 count = 0;
     count = *tlvIndex;
    /* Index 1 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].pb_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = FAPI_P_B;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.pb =
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].pb);
       count++;
       *tlvIndex = count;
    }

    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForDlCyclicPrefixTypeFlag.
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                 for DlCyclicPrefixTypeFlag.
 *********************************************************************/

void fillPhyReConfigRequestForDlCyclicPrefixTypeFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;
    /* Index 2 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].dlCyclicPrefixType_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_DL_CYCLIC_PREFIX_TYPE;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.\
            dlCyclicPrefixType = 
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].\
                    dlCyclicPrefixType);
        count++;
        *tlvIndex = count;
    }

    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForUlcyclicPrefixTypeFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                 for UlcyclicPrefixTypeFlag
 **********************************************************************/

void fillPhyReConfigRequestForUlCyclicPrefixTypeFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 3 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].ulCyclicPrefixType_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_UL_CYCLIC_PREFIX_TYPE;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.\
            ulCyclicPrefixType =
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].\
                    ulCyclicPrefixType);
        count++;
        *tlvIndex = count;
    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForDlChannelBWFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                 for DlChannelBWFlag
 ***********************************************************************/

void fillPhyReConfigRequestForDlChannelBWFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 4 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].dlChannelBW_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_DL_CHANNEL_BANDWIDTH;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.dlChannelBW =
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].dlChannelBW);
        count++;
       *tlvIndex = count;
    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForUlChannelBWFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                 for UlChannelBWFlag
 ************************************************************************/

void fillPhyReConfigRequestForUlChannelBWFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 5 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].ulChannelBW_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_UL_CHANNEL_BANDWIDTH;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.ulChannelBW =
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].ulChannelBW);
       count++;
        *tlvIndex = count;   
    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForTxAntennaPortFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                for TxAntennaPortFlag
 *************************************************************************/

void fillPhyReConfigRequestForTxAntennaPortFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 6 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].txAntennaPort_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_TX_ANTENNA_PORTS;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.txAntennaPort=
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].txAntennaPort);
        count++;
         *tlvIndex = count;
    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequesForRxAntennaPortFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                for RxAntennaPortFlag
 **************************************************************************/

void fillPhyReConfigRequestForRxAntennaPortFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;
    /* Index 7 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].rxAntennaPort_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_RX_ANTENNA_PORTS;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.rxAntennaPort=
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].rxAntennaPort);
        count++;
       *tlvIndex = count;
    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForPhichResourceFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                for PhichResourceFlag
 **************************************************************************/

void fillPhyReConfigRequestForPhichResourceFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 8 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].phichResource_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_PHICH_RESOURCE;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.phichResource=
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].phichResource);
       count++;
       *tlvIndex = count;
    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForPhichDuration
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                for PhichDuration
 ***************************************************************************/


void fillPhyReConfigRequestForPhichDuration(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 9 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].phichDuration)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_PHICH_DURATION;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.phichDuration=
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].phichDuration);
        count++;
       *tlvIndex = count;
    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForPrimarySyncSignalFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                for PrimarySyncSignalFlag
 ****************************************************************************/
void fillPhyReConfigRequestForPrimarySyncSignalFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 10 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].primarySyncSignal_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_PRIMARY_SYNC_SIGNAL;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.primarySyncSignal=
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].primarySyncSignal);
        count++;
       *tlvIndex = count;

    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForSecondarySyncSignalFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                for SecondarySyncSignalFlag
 ****************************************************************************/
void fillPhyReConfigRequestForSecondarySyncSignalFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 11 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].secondarySyncSignal_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_SECONDARY_SYNC_SIGNAL;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.\
            secondarySyncSignal=
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].\
                    secondarySyncSignal);
        count++;
       *tlvIndex = count;

    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForPhysicalCellIdFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                 for PhysicalCellIdFlag
 *****************************************************************************/
void fillPhyReConfigRequestForPhysicalCellIdFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 12 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].physicalCellId_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_PHYSICAL_CELL_ID;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.physicalCellId =
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].physicalCellId);
        count++;
        *tlvIndex = count;
    }
    return;
}
/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForConfigurationIndexFlag
 *Inputs         : internalCellIndex
  *                tlvIndex
   *               FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                 for ConfigurationIndexFlag
 ******************************************************************************/

void fillPhyReConfigRequestForConfigurationIndexFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 13 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].configurationIndex_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_CONFIGURATION_INDEX;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.\
            configurationIndex =
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].\
                    configurationIndex);
        count++;
        *tlvIndex = count;
    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForRootSeqIndexFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                 for RootSeqIndexFlag
 *******************************************************************************/

void fillPhyReConfigRequestForRootSeqIndexFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 14 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].rootSeqIndex_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_ROOT_SEQUENCE_INDEX;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.rootSeqIndex =
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].rootSeqIndex);
        count++;
        *tlvIndex = count;

    }
    return;
}
/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForZeroCorelationZoneConfigFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                 for ZeroCorelationZoneConfigFlag
 ********************************************************************************/

void fillPhyReConfigRequestForZeroCorelationZoneConfigFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 15 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].zeroCorelationZoneConfig_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_ZERO_CORRELATION_ZONE_CONFIGURATION;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.\
            zeroCorelationZoneConfig =
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].\
                    zeroCorelationZoneConfig);
        count++;
        *tlvIndex = count;

    }
    return;
}
/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForHighSpeedFlagFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                 for HighSpeedFlag
 *********************************************************************************/

void fillPhyReConfigRequestForHighSpeedFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 16 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].highSpeedFlag_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_HIGH_SPEED_FLAG;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.highSpeedFlag =
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].highSpeedFlag);
        count++;
        *tlvIndex = count;
    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForFreqOffsetFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                 for FreqOffsetFlag
 **********************************************************************************/

void fillPhyReConfigRequestForFreqOffsetFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 17 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].freqOffset_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_FREQUENCY_OFFSET;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.freqOffset=
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].freqOffset);
        count++;
        *tlvIndex = count;
    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForHoppingMode_flag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                 for HoppingMode_flag
 ***********************************************************************************/

void fillPhyReConfigRequestForHoppingModeFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 18 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].hoppingMode_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_HOPPING_MODE;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.hoppingMode=
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].hoppingMode);
        count++;
        *tlvIndex = count;

    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForHoppingOffsetFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                 for HoppingOffsetFlag
 ************************************************************************************/

void fillPhyReConfigRequestForHoppingOffsetFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 19 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].hoppingOffset_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_HOPPIG_OFFSET;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.hoppingOffset=
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].hoppingOffset);
        count++;
        *tlvIndex = count;

    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForNumOfSubBandFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                for NumOfSubBandFlag
 *************************************************************************************/

void fillPhyReConfigRequestForNumOfSubBandFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 20 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].numOfSubBand_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_NUM_OF_SUB_BANDS;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.numOfSubBand=
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].numOfSubBand);
        count++;
        *tlvIndex = count;
    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForDeltaPUCCHShiftFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                for DeltaPUCCHShiftFlag
 **************************************************************************************/

void fillPhyReConfigRequestForDeltaPUCCHShiftFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 21 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].deltaPUCCHShift_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_DELTA_PUCCH_SHIFT;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.deltaPUCCHShift=
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].\
                    deltaPUCCHShift);
        count++;
        *tlvIndex = count;
    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForNCQIRBFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                 for NCQIRBFlag
 ***************************************************************************************/

void fillPhyReConfigRequestForNCQIRBFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 22 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].nCQIRB_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_N_CQI_RB;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.nCQIRB =
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].nCQIRB);
        count++;
        *tlvIndex = count;
    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForNAnCsFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                 for NAnCsFlag
 ****************************************************************************************/

void fillPhyReConfigRequestForNAnCsFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 23 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].nAnCs_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_N_AN_CS;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.nAnCs =
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].nAnCs);
        count++;
        *tlvIndex = count;
    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForN1PucchAnFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                 for N1PucchAnFlag
 ******************************************************************************************/

void fillPhyReConfigRequestForN1PucchAnFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8 *tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 24 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].n1PucchAn_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_N_1_PUCCH_AN;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.n1PucchAn =
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].n1PucchAn);
        count++;
        *tlvIndex = count;

    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForBandWidthConfigurationFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                for BandWidthConfigurationFlag
 *******************************************************************************************/

void fillPhyReConfigRequestForBandWidthConfigurationFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8* tlvIndex,
		InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    count = *tlvIndex;

    /* Index 25 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].bandWidthConfiguration_flag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = 
            FAPI_BANDWIDTH_CONFIGURATION;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.\
            bandWidthConfiguration =
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].\
                    bandWidthConfiguration);
        count++;
        *tlvIndex = count;
 
    }
    return;
}

/**********************************************************************
 *Function Name  : fillPhyReConfigRequestForPrsPhyConfigUpdateFlag
 *Inputs         : internalCellIndex
 *                 tlvIndex
 *                 FapiCellReConfigReq_p
 *Outputs        : None
 *Returns        : None
 *Description    : This function is used to fill PHY config request
 *                 for PrsPhyConfig.updateFlag
 ********************************************************************************************/

void fillPhyReConfigRequestForPrsPhyConfigUpdateFlag(FAPI_phyCellConfigRequest_st *FapiCellReConfigReq_p,
		UInt8* tlvIndex,
		InternalCellIndex internalCellIndex)
{
#ifndef FLEXRAN
    UInt8 count = 0;
    count = *tlvIndex;

/* SPR 19288 change start */
    /* + PRS_CHANGES */
    /* Index 26 */
    if( TRUE == reconfigCellMajor_g[internalCellIndex].prsPhyConfig.updateFlag)
    {
        FapiCellReConfigReq_p->configtlvs[count].tag = FAPI_PRS_BANDWIDTH;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.prsBandWidth =
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].\
                    prsPhyConfig.prsBandWidth);
        count++;
        

        /* Index 27 */
        FapiCellReConfigReq_p->configtlvs[count].tag = FAPI_PRS_CYCLIC_PREFIX;
        FapiCellReConfigReq_p->configtlvs[count].tagLen = 2;
        FapiCellReConfigReq_p->configtlvs[count].configParam.prsCyclicPrifix =
            MAC_PHY_CONVERT_16(reconfigCellMajor_g[internalCellIndex].\
                    prsPhyConfig.prsCyclicPrifix);
/* SPR 19288 change end */
        count++;
        *tlvIndex = count;
    }
#endif
    return;
}

/**********************************************************************
 *Function Name  : parseReconfigPhyCellParamsForTxAntenna
 *Inputs         : subBitMask
 *                 msgBuf_p
 *                 remLen
 *                 tlvIndex
 *                 internalCellIndex
 *Outputs        : None
 *Returns        : MAC_SUCCESS/MAC_FAILURE
 *Description    : This function parses reconfig cell params for tx antenna
 ********************************************************************************************/
MacRetType parseReconfigPhyCellParamsForTxAntenna(UInt16 subBitMask,
        UInt8 **msgBuf_p,
        UInt16 *remLen,
        UInt8 *numTLV,
        InternalCellIndex internalCellIndex)
{
    UInt8 *msg_p = *msgBuf_p;

    if(subBitMask & BIT_THIRD)/* Total number of antennas */
    { 
        reconfigCellMajor_g[internalCellIndex].txAntennaPort = *msg_p++; 
        (*remLen)--;
        reconfigCellMajor_g[internalCellIndex].rxAntennaPort = 
            reconfigCellMajor_g[internalCellIndex].txAntennaPort; 
            /*SPR 21021 fix start*/ 
                /*SPR 21021 fix end*/

        if ( ( (1 != reconfigCellMajor_g[internalCellIndex].txAntennaPort) &&
                    (2 != reconfigCellMajor_g[internalCellIndex].txAntennaPort) &&
                    (4 != reconfigCellMajor_g[internalCellIndex].txAntennaPort)) 
                || (reconfigCellMajor_g[internalCellIndex].txAntennaPort 
                    != cellConfigPhy_gp[internalCellIndex]->rfConfig.txAntennaPort)
           )
        {
            *msgBuf_p = msg_p;
            return MAC_FAILURE;
        }

        reconfigCellMajor_g[internalCellIndex].txAntennaPort_flag = TRUE; 
        reconfigCellMajor_g[internalCellIndex].rxAntennaPort_flag = TRUE; 
        (*numTLV)++;
        *msgBuf_p = msg_p;
    }


    return MAC_SUCCESS;

}

/**********************************************************************
 *Function Name  : parseReconfigPhyCellParamsForUlBW
 *Inputs         : subBitMask
 *                 ulChBandwidth
 *                 msgBuf_p
 *                 remLen
 *                 tlvIndex
 *                 internalCellIndex
 *Outputs        : None
 *Returns        : MAC_SUCCESS/MAC_FAILURE
 *Description    : This function parses reconfig cell params for UL BW.
 ********************************************************************************************/
MacRetType parseReconfigPhyCellParamsForUlBW(UInt16 subBitMask,
        UInt8 *ulChBandwidth,
        UInt8 **msgBuf_p,
        UInt16 *remLen,
        UInt8 *numTLV,
        InternalCellIndex internalCellIndex)
{
    UInt8 *msg_p = *msgBuf_p;

    if(subBitMask & BIT_FOURTH)/* UL Bandwidth */
    {
        *ulChBandwidth = *msg_p++;
        (*remLen)--;

        if( ! (phyParamResp_g[internalCellIndex].ulBandWidthSupport &
                    (0x01 << (*ulChBandwidth)) ) )
        {
            *msgBuf_p = msg_p;
            LOG_MAC_WARNING( MAC_RRC_INF,"Param mismatch with received in Param Resp: UL Bandwidth\n");
            return MAC_FAILURE;
        }

        reconfigCellMajor_g[internalCellIndex].ulChannelBW_flag = TRUE;
        switch(*ulChBandwidth)
        {
            case UL_TX_BANDWIDTH_6RB:
            reconfigCellMajor_g[internalCellIndex].ulChannelBW = 
                FAPI_CHANNEL_BW_6RB;
            break;
            case UL_TX_BANDWIDTH_15RB:
            reconfigCellMajor_g[internalCellIndex].ulChannelBW = 
                FAPI_CHANNEL_BW_15RB;
            break;
            case UL_TX_BANDWIDTH_25RB:
            reconfigCellMajor_g[internalCellIndex].ulChannelBW =
                FAPI_CHANNEL_BW_25RB;
            break;
            case UL_TX_BANDWIDTH_50RB:
            reconfigCellMajor_g[internalCellIndex].ulChannelBW =
                FAPI_CHANNEL_BW_50RB;
            break;
            case UL_TX_BANDWIDTH_75RB:
            reconfigCellMajor_g[internalCellIndex].ulChannelBW =
                FAPI_CHANNEL_BW_75RB;
            break;
            case UL_TX_BANDWIDTH_100RB:
            reconfigCellMajor_g[internalCellIndex].ulChannelBW = 
                FAPI_CHANNEL_BW_100RB;
            break;
        }
        if (reconfigCellMajor_g[internalCellIndex].ulChannelBW != 
                cellConfigPhy_gp[internalCellIndex]->rfConfig.ulChannelBW
           )
        {
            *msgBuf_p = msg_p;
            return MAC_FAILURE;
        }
        (*numTLV)++;
        *msgBuf_p = msg_p;
    }
    
    return MAC_SUCCESS;
}

/**********************************************************************
 *Function Name  : parseReconfigPhyCellParamsForCyclicPrefix
 *Inputs         : subBitMask
 *                 dlChBandwidth
 *                 msgBuf_p
 *                 remLen
 *                 tlvIndex
 *                 internalCellIndex
 *Outputs        : None
 *Returns        : MAC_SUCCESS/MAC_FAILURE
 *Description    : This function parses reconfig cell params for cyclic 
 *                   prefix.
 *************************************************************************/
MacRetType parseReconfigPhyCellParamsForCyclicPrefix(UInt16 subBitMask,
        UInt8 **msgBuf_p,
        UInt16 *remLen,
        UInt8 *numTLV,
        InternalCellIndex internalCellIndex)
{
    UInt8 *msg_p = *msgBuf_p;

    if(subBitMask & BIT_SIXTH)/* UL Cyclic prefix */
    {
        reconfigCellMajor_g[internalCellIndex].ulCyclicPrefixType = *msg_p++;
        (*remLen)--;

        if (reconfigCellMajor_g[internalCellIndex].ulCyclicPrefixType != 
                cellConfigPhy_gp[internalCellIndex]->ulCyclicPrefixType
           )
        {
            /* SPR 12915 Changes Start */
            *msgBuf_p = msg_p;
            /* SPR 12915 Changes End */
            return MAC_FAILURE;
        }

        reconfigCellMajor_g[internalCellIndex].ulCyclicPrefixType_flag = TRUE;
        (*numTLV)++;
    }
    if(subBitMask & BIT_SEVENTH)/* DL Cyclic prefix */
    {
        reconfigCellMajor_g[internalCellIndex].dlCyclicPrefixType = *msg_p++;
        (*remLen)--;

        if (reconfigCellMajor_g[internalCellIndex].dlCyclicPrefixType != 
                cellConfigPhy_gp[internalCellIndex]->dlCyclicPrefixType
           )
        {
            /* SPR 12915 Changes Start */
            *msgBuf_p = msg_p;
            /* SPR 12915 Changes End */
            return MAC_FAILURE;
        }

        reconfigCellMajor_g[internalCellIndex].dlCyclicPrefixType_flag = TRUE;
        (*numTLV)++;
    }

    *msgBuf_p = msg_p;
    return MAC_SUCCESS;
}

/**********************************************************************
 *Function Name  : parseReconfigPhyCellParamsForDlBW
 *Inputs         : subBitMask
 *                 dlChBandwidth
 *                 msgBuf_p
 *                 remLen
 *                 tlvIndex
 *                 internalCellIndex
 *Outputs        : None
 *Returns        : MAC_SUCCESS/MAC_FAILURE
 *Description    : This function parses reconfig cell params for Dl bandwidth
 ********************************************************************************************/
MacRetType parseReconfigPhyCellParamsForDlBW(UInt16 subBitMask,
        UInt8 *dlChBandwidth,
        UInt8 **msgBuf_p,
        UInt16 *remLen,
        UInt8 *numTLV,
        InternalCellIndex internalCellIndex)
{
    UInt8 *msg_p = *msgBuf_p;
    
    if(subBitMask & BIT_FIFTH)/* DL Bandwidth */ 
    {
        *dlChBandwidth = *msg_p++;
        (*remLen)--;
        if( ! (phyParamResp_g[internalCellIndex].dlBandWidthSupport &
                    (0x01 << (*dlChBandwidth)) ) )
        {
            *msgBuf_p = msg_p;
            LOG_MAC_WARNING( MAC_RRC_INF,"Param mismatch with received in Param Resp: DL Bandwidth\n");
            return MAC_FAILURE;
        }

        switch(*dlChBandwidth)
        {
            case DL_TX_BANDWIDTH_6RB:
            reconfigCellMajor_g[internalCellIndex].dlChannelBW = 
                FAPI_CHANNEL_BW_6RB;
            break;
            case DL_TX_BANDWIDTH_15RB:
            reconfigCellMajor_g[internalCellIndex].dlChannelBW = 
                FAPI_CHANNEL_BW_15RB;
            break;
            case DL_TX_BANDWIDTH_25RB:
            reconfigCellMajor_g[internalCellIndex].dlChannelBW = 
                FAPI_CHANNEL_BW_25RB;
            break;
            case DL_TX_BANDWIDTH_50RB:
            reconfigCellMajor_g[internalCellIndex].dlChannelBW = 
                FAPI_CHANNEL_BW_50RB;
            break;
            case DL_TX_BANDWIDTH_75RB:
            reconfigCellMajor_g[internalCellIndex].dlChannelBW = 
                FAPI_CHANNEL_BW_75RB;
            break;
            case DL_TX_BANDWIDTH_100RB:
            reconfigCellMajor_g[internalCellIndex].dlChannelBW = 
                FAPI_CHANNEL_BW_100RB;
            break;
        }
        if (reconfigCellMajor_g[internalCellIndex].dlChannelBW != 
                cellConfigPhy_gp[internalCellIndex]->rfConfig.dlChannelBW
           )
        {
            *msgBuf_p = msg_p;
            return MAC_FAILURE;
        }

        reconfigCellMajor_g[internalCellIndex].dlChannelBW_flag = TRUE;
        (*numTLV)++;
        *msgBuf_p = msg_p;
    }


    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : reconfigSyncSignalCellParams
 * Inputs         : remlLen
 *                  numTLV
 *                  msg_p
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/ MAC_FAILURE
 * Description    : This function is used by MAC to parse reconfigure sync signal
 *                  cell parms.
 *****************************************************************************/
MacRetType reconfigSyncSignalCellParams(UInt16 *remLen, 
        UInt8 *numTLV,
        UInt8 **msgBuf_p,
        InternalCellIndex internalCellIndex)
{
    UInt8  refSig    = 0;
    UInt16 refSigCal = 0;
    UInt8 *msg_p = *msgBuf_p;

    refSig = *msg_p++;
    (*remLen) --;
    refSigCal = (refSig + MIN_VALUE_OF_SYNC_SIGNAL) *
        MULTIPLY_COEFF_FOR_SYNC_SIGNAL;

    if ((MAX_VAL_SYNC_SIGNAL + 1) == refSigCal)
    {
        refSigCal = MAX_VAL_SYNC_SIGNAL;
    }

    reconfigCellMajor_g[internalCellIndex].primarySyncSignal = refSigCal;

    if( (MAX_VAL_SYNC_SIGNAL < 
                reconfigCellMajor_g[internalCellIndex].primarySyncSignal)
            || (reconfigCellMajor_g[internalCellIndex].primarySyncSignal != 
                cellConfigPhy_gp[internalCellIndex]->schConfig.primarySyncSignal)
      )
    {
        *msgBuf_p = msg_p;
        return MAC_FAILURE;
    }

    reconfigCellMajor_g[internalCellIndex].primarySyncSignal_flag = TRUE;
    (*numTLV)++;

    refSig = *msg_p++;
    (*remLen) --;

    refSigCal = (refSig + MIN_VALUE_OF_SYNC_SIGNAL) *
        MULTIPLY_COEFF_FOR_SYNC_SIGNAL;

    if ((MAX_VAL_SYNC_SIGNAL + 1) == refSigCal)
    {
        refSigCal = MAX_VAL_SYNC_SIGNAL;
    }

    reconfigCellMajor_g[internalCellIndex].secondarySyncSignal = refSigCal;

    if( (MAX_VAL_SYNC_SIGNAL < 
                reconfigCellMajor_g[internalCellIndex].secondarySyncSignal)
            || (reconfigCellMajor_g[internalCellIndex].secondarySyncSignal 
                !=  cellConfigPhy_gp[internalCellIndex]->schConfig.secondarySyncSignal)
      )
    {
        *msgBuf_p = msg_p;
        return MAC_FAILURE;
    }
    reconfigCellMajor_g[internalCellIndex].secondarySyncSignal_flag = TRUE;
    (*numTLV)++;

    msg_p++; /* SecSynSignalMsec1 - Not supported */
    (*remLen) --;
    msg_p++;/* SecSynSignalMsec2 - Not supported */
    (*remLen) --;

    *msgBuf_p = msg_p;
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : reconfigPrachCellParams
 * Inputs         : remlLen
 *                  numTLV
 *                  msg_p
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/ MAC_FAILURE
 * Description    : This function is used by MAC to parse reconfigure prach
 *                  cell parms.
 *****************************************************************************/
MacRetType reconfigPrachCellParams(UInt16 *remLen, 
        UInt8 *numTLV,
        UInt8 **msgBuf_p,
        InternalCellIndex internalCellIndex)
{
    UInt8 *msg_p = *msgBuf_p;

    reconfigCellMajor_g[internalCellIndex].configurationIndex = *msg_p++;
    (*remLen) --;

    if( (MAX_VAL_CONFIG_INDEX < 
                reconfigCellMajor_g[internalCellIndex].configurationIndex) ||
            (reconfigCellMajor_g[internalCellIndex].configurationIndex != 
             cellConfigPhy_gp[internalCellIndex]->prachConfig.configurationIndex))
    {
        *msgBuf_p = msg_p;
        return MAC_FAILURE;
    }

    reconfigCellMajor_g[internalCellIndex].configurationIndex_flag = TRUE;
    (*numTLV)++;

    reconfigCellMajor_g[internalCellIndex].rootSeqIndex = LTE_GET_U16BIT(msg_p);
    (*remLen) -= 2;
    msg_p += 2;

    if( (MAX_VAL_ROOT_SEQ_INDEX < 
                reconfigCellMajor_g[internalCellIndex].rootSeqIndex) || 
            (reconfigCellMajor_g[internalCellIndex].rootSeqIndex != 
             cellConfigPhy_gp[internalCellIndex]->prachConfig.rootSeqIndex)
      )
    {
        *msgBuf_p = msg_p;
        return MAC_FAILURE;
    }

    reconfigCellMajor_g[internalCellIndex].rootSeqIndex_flag = TRUE;
    (*numTLV)++;

    reconfigCellMajor_g[internalCellIndex].freqOffset = *msg_p++;
    (*remLen) --;

    if (reconfigCellMajor_g[internalCellIndex].freqOffset != 
            cellConfigPhy_gp[internalCellIndex]->prachConfig.freqOffset
       )
    {
        *msgBuf_p = msg_p;
        return MAC_FAILURE;
    }

    reconfigCellMajor_g[internalCellIndex].freqOffset_flag = TRUE;
    (*numTLV)++;

    reconfigCellMajor_g[internalCellIndex].zeroCorelationZoneConfig =
        *msg_p++;
    (*remLen) --;

    if( (MAX_VAL_ZERO_COR_ZONE < 
                reconfigCellMajor_g[internalCellIndex].zeroCorelationZoneConfig) ||
            (reconfigCellMajor_g[internalCellIndex].zeroCorelationZoneConfig 
             != cellConfigPhy_gp[internalCellIndex]->\
             prachConfig.zeroCorelationZoneConfig) 
      )
    {
        *msgBuf_p = msg_p;
        return MAC_FAILURE;       
    }

    reconfigCellMajor_g[internalCellIndex].zeroCorelationZoneConfig_flag =
        TRUE;
    (*numTLV)++;

    reconfigCellMajor_g[internalCellIndex].highSpeedFlag = *msg_p++;
    (*remLen) --;

    if ( (1 < reconfigCellMajor_g[internalCellIndex].highSpeedFlag) 
            || (reconfigCellMajor_g[internalCellIndex].highSpeedFlag 
                !=  cellConfigPhy_gp[internalCellIndex]->prachConfig.highSpeedFlag)
       )
    {
        *msgBuf_p = msg_p;
        return MAC_FAILURE;
    }

    reconfigCellMajor_g[internalCellIndex].highSpeedFlag_flag = TRUE;
    (*numTLV)++;

    *msgBuf_p = msg_p;
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : reconfigPuschCellParams
 * Inputs         : remlLen
 *                  numTLV
 *                  msg_p
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/ MAC_FAILURE
 * Description    : This function is used by MAC to parse reconfigure pusch
 *                  cell parms.
 *****************************************************************************/
MacRetType reconfigPuschCellParams(UInt16 *remLen, 
        UInt8 *numTLV,
        UInt8 **msgBuf_p,
        InternalCellIndex internalCellIndex)
{
    UInt8 *msg_p = *msgBuf_p;

    reconfigCellMajor_g[internalCellIndex].hoppingOffset = *msg_p++;
    (*remLen) --;

    if( (!reconfigCellMajor_g[internalCellIndex].hoppingOffset)
            || (MAX_VAL_GRP_ASSIGN  
                <   reconfigCellMajor_g[internalCellIndex].hoppingOffset)
            || (reconfigCellMajor_g[internalCellIndex].hoppingOffset
                !=  cellConfigPhy_gp[internalCellIndex]->puschConfig.hoppingOffset))
    {
        *msgBuf_p = msg_p;
        return MAC_FAILURE;
    }

    reconfigCellMajor_g[internalCellIndex].hoppingOffset_flag = TRUE;
    (*numTLV)++;

    reconfigCellMajor_g[internalCellIndex].numOfSubBand = *msg_p++;
    (*remLen) --;

    if( (1 > reconfigCellMajor_g[internalCellIndex].numOfSubBand)
            || (MAX_VAL_NUM_OF_SUB_BANDS 
                <   reconfigCellMajor_g[internalCellIndex].numOfSubBand)
            || (reconfigCellMajor_g[internalCellIndex].numOfSubBand 
                !=  cellConfigPhy_gp[internalCellIndex]->puschConfig.numOfSubBand) 
      )
    {
        *msgBuf_p = msg_p;
        return MAC_FAILURE;
    }

    reconfigCellMajor_g[internalCellIndex].numOfSubBand_flag = TRUE;
    (*numTLV)++;

    reconfigCellMajor_g[internalCellIndex].hoppingMode = *msg_p++;
    (*remLen) --;

    if ( (1 < reconfigCellMajor_g[internalCellIndex].hoppingMode)
            || ( reconfigCellMajor_g[internalCellIndex].hoppingMode 
                != cellConfigPhy_gp[internalCellIndex]->puschConfig.hoppingMode) 
       )
    {
        *msgBuf_p = msg_p;
        return MAC_FAILURE;
    }

    reconfigCellMajor_g[internalCellIndex].hoppingMode_flag = TRUE;
    (*numTLV)++;

    reconfigCellMajor_g[internalCellIndex].groupHopEnable = *msg_p++;
    (*remLen) --;

    if( reconfigCellMajor_g[internalCellIndex].groupHopEnable 
            != cellConfigPhy_gp[internalCellIndex]->puschConfig.groupHopEnable)  
    {
        *msgBuf_p = msg_p;
        return MAC_FAILURE;
    }

    reconfigCellMajor_g[internalCellIndex].groupHopEnable_flag = TRUE;
    (*numTLV)++;

    reconfigCellMajor_g[internalCellIndex].groupAssignPUSCH = *msg_p++;
    (*remLen) --;

    if(reconfigCellMajor_g[internalCellIndex].groupAssignPUSCH 
            != cellConfigPhy_gp[internalCellIndex]->puschConfig.groupAssignPUSCH
      )
    {
        *msgBuf_p = msg_p;
        return MAC_FAILURE;
    }

    reconfigCellMajor_g[internalCellIndex].groupAssignPUSCH_flag = TRUE;
    (*numTLV)++;

    reconfigCellMajor_g[internalCellIndex].seqHopEnable = *msg_p++;
    (*remLen) --;

    if(reconfigCellMajor_g[internalCellIndex].seqHopEnable 
            != cellConfigPhy_gp[internalCellIndex]->puschConfig.seqHopEnable
      )
    {
        *msgBuf_p = msg_p;
        return MAC_FAILURE; 
    }

    reconfigCellMajor_g[internalCellIndex].seqHopEnable_flag = TRUE;
    (*numTLV)++;

    reconfigCellMajor_g[internalCellIndex].cyclicShift = *msg_p++;
    (*remLen) --;

    if(reconfigCellMajor_g[internalCellIndex].cyclicShift 
            != cellConfigPhy_gp[internalCellIndex]->puschConfig.cyclicShift
      )
    {
        *msgBuf_p = msg_p;
        return MAC_FAILURE;
    }

    reconfigCellMajor_g[internalCellIndex].cyclicShift_flag = TRUE;
    (*numTLV)++;

    *msgBuf_p = msg_p;
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : reconfigPucchCellParams
 * Inputs         : remlLen
 *                  numTLV
 *                  msg_p
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/ MAC_FAILURE
 * Description    : This function is used by MAC to parse reconfigure pucch
 *                  cell parms.
 *****************************************************************************/
MacRetType reconfigPucchCellParams(UInt16 *remLen, 
        UInt8 *numTLV,
        UInt8 **msgBuf_p,
        InternalCellIndex internalCellIndex)
{
    UInt8 *msg_p = *msgBuf_p;

    reconfigCellMajor_g[internalCellIndex].deltaPUCCHShift = *msg_p++;
    (*remLen) --;

    if( (1 > reconfigCellMajor_g[internalCellIndex].deltaPUCCHShift)
            || (MAX_VAL_DELTA_PUCCH_SHIFT 
                < reconfigCellMajor_g[internalCellIndex].deltaPUCCHShift)
            || (reconfigCellMajor_g[internalCellIndex].deltaPUCCHShift
                != cellConfigPhy_gp[internalCellIndex]->\
                pucchConfig.deltaPUCCHShift)
      )
    {
        return MAC_FAILURE;
    }

    reconfigCellMajor_g[internalCellIndex].deltaPUCCHShift_flag = TRUE;
    (*numTLV)++;

    reconfigCellMajor_g[internalCellIndex].nCQIRB = *msg_p++;
    (*remLen) --;

    if( (MAX_VAL_NCQIRB < reconfigCellMajor_g[internalCellIndex].nCQIRB)
            || (reconfigCellMajor_g[internalCellIndex].nCQIRB 
                != cellConfigPhy_gp[internalCellIndex]->pucchConfig.nCQIRB) 
      )
    {
        return MAC_FAILURE;
    }

    reconfigCellMajor_g[internalCellIndex].nCQIRB_flag = TRUE;
    (*numTLV)++;

    reconfigCellMajor_g[internalCellIndex].nAnCs = *msg_p++;
    (*remLen) --;

    if( (MAX_VAL_NANCS < reconfigCellMajor_g[internalCellIndex].nAnCs)
            ||  ( reconfigCellMajor_g[internalCellIndex].nAnCs 
                != cellConfigPhy_gp[internalCellIndex]->pucchConfig.nAnCs) 
      )
    {
        return MAC_FAILURE;
    }

    reconfigCellMajor_g[internalCellIndex].nAnCs_flag = TRUE;
    (*numTLV)++;

    reconfigCellMajor_g[internalCellIndex].n1PucchAn =  
        LTE_GET_U16BIT(msg_p);
    (*remLen) -= 2;
    msg_p += 2;

    if( (MAX_VAL_N1_PUCCH_AN < 
                reconfigCellMajor_g[internalCellIndex].n1PucchAn)
            || (reconfigCellMajor_g[internalCellIndex].n1PucchAn 
                != cellConfigPhy_gp[internalCellIndex]->pucchConfig.n1PucchAn) 
      )
    {
        return MAC_FAILURE;
    }

    reconfigCellMajor_g[internalCellIndex].n1PucchAn_flag = TRUE;
    (*numTLV)++;

    *msgBuf_p = msg_p;
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : reconfigPhichCellParams
 * Inputs         : remlLen
 *                  numTLV
 *                  msg_p
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/ MAC_FAILURE
 * Description    : This function is used by MAC to parse reconfigure phich
 *                  cell parms.
 *****************************************************************************/
MacRetType reconfigPhichCellParams(UInt16 *remLen, 
        UInt8 *numTLV,
        UInt8 **msgBuf_p,
        InternalCellIndex internalCellIndex)
{
    UInt8 *msg_p = *msgBuf_p;

    reconfigCellMajor_g[internalCellIndex].phichResource = *msg_p++;
    (*remLen) --;

    if( (MAX_VAL_PHICH_RESOURCE < 
                reconfigCellMajor_g[internalCellIndex].phichResource)
            || (reconfigCellMajor_g[internalCellIndex].phichResource
                !=  cellConfigPhy_gp[internalCellIndex]->phichConfig.phichResource) 
      )
    {
        *msgBuf_p = msg_p;
        return MAC_FAILURE;
    }

    reconfigCellMajor_g[internalCellIndex].phichResource_flag = TRUE;
    (*numTLV)++;

    reconfigCellMajor_g[internalCellIndex].phichDuration = *msg_p++;
    (*remLen) --;

    if( (1 < reconfigCellMajor_g[internalCellIndex].phichDuration)
            || (reconfigCellMajor_g[internalCellIndex].phichDuration
                != cellConfigPhy_gp[internalCellIndex]->phichConfig.phichDuration)
      )    
    {
        *msgBuf_p = msg_p;
        return MAC_FAILURE;
    }

    reconfigCellMajor_g[internalCellIndex].phichDuration_flag = TRUE;
    (*numTLV)++;

    *msgBuf_p = msg_p;
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : reconfigPdschCellParams
 * Inputs         : remlLen
 *                  numTLV
 *                  msg_p
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/ MAC_FAILURE
 * Description    : This function is used by MAC to parse reconfigure pdsch
 *                  cell parms.
 *****************************************************************************/
MacRetType reconfigPdschCellParams(UInt16 *remLen,
        UInt8 *numTLV,
        UInt8 **msgBuf_p,
        InternalCellIndex internalCellIndex)
{
    UInt8 *msg_p = *msgBuf_p;

    reconfigCellMajor_g[internalCellIndex].refSignalPower = *msg_p++;
    (*remLen) --;
    reconfigCellMajor_g[internalCellIndex].refSignalPower_flag = TRUE;
    (*numTLV)++;

    reconfigCellMajor_g[internalCellIndex].pb = *msg_p++;
    (*remLen)--;

    if( (MAX_VAL_PB < reconfigCellMajor_g[internalCellIndex].pb) 
            || (reconfigCellMajor_g[internalCellIndex].pb !=
                cellConfigPhy_gp[internalCellIndex]->pb)
      )
    {
        *msgBuf_p = msg_p;
        return MAC_FAILURE;
    }

    reconfigCellMajor_g[internalCellIndex].pb_flag = TRUE;
    (*numTLV)++;

    *msgBuf_p = msg_p;
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : parseExplicitStartBitMask
 * Inputs         : explicitStartBitMask
 *                  msgBuf_p - Pointer to msg buffer
 *                  remLen_p - Pointer to remaining length.
 * Outputs        : NULL
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This funtion parses explicit start bit.
 *****************************************************************************/
MacRetType parseExplicitStartBitMask(UInt16 explicitStartBitMask,
        UInt8 **msgBuf_p,
        UInt16 *remLen_p)
{
    MacRetType response = MAC_SUCCESS;
    UInt8  explicitStartRequired_tag = 0;
    UInt8 *msg_p = *msgBuf_p;
    
    if(explicitStartBitMask & BIT_FIRST)
    {
        if(*remLen_p > 0)
        {
            explicitStartRequired_tag = *msg_p++;
            (*remLen_p)--;
            if(0 == explicitStartRequired_tag) /* FALSE used for value = 0 */
            {
                response = MAC_SUCCESS;
            }
            else
            {
                LOG_MAC_WARNING( MAC_RRC_INF,"Explicit Start Invalid Tag Value");
                response = MAC_FAILURE;
            }
            if(*remLen_p > 0)
            {
                LOG_MAC_WARNING( MAC_RRC_INF,"Invalid PHY Config Request Length");
                response = MAC_FAILURE;
            }
        }
        else
        {
            LOG_MAC_WARNING( MAC_RRC_INF,"Invalid PHY Config Request without explicitStart Bit");
            response = MAC_FAILURE;
        }
    }
    else
    {
        /* No code required for this section */
    }

    *msgBuf_p = msg_p;
    return response;
}
/* Cyclomatic_complexity_changes_end */



#if defined(FLEXRAN)
/*SPR 21347 fix start*/
eltaAstriParamThreshold *eltaAstriParamUpdate(void)
{

    eltaAstriParamThreshold *paramThresholdVal = PNULL;
    paramThresholdVal = malloc(sizeof(eltaAstriParamThreshold));

    if (NULL == paramThresholdVal)
        return NULL;
#ifdef FLEXRAN
    paramThresholdVal->prachFormat4Threshold = 0x38cc;
    paramThresholdVal->prachFormat0Threshold = 0x38cc;
    return paramThresholdVal;
#else
    FILE* fp;
    char *readFile = NULL;
    char  buf[501] = {0,};

    memset(paramThresholdVal,0,sizeof(eltaAstriParamThreshold));
    readFile = "../cfg/fapiLogConfig.cfg";
    fp = fopen(readFile,"rb");
    if (NULL == fp)
    {
        printf("Unable to open cfg\n");
        free(paramThresholdVal);
        return PNULL;
    }
    while (fgets(buf,500,fp))
    {
        if (sscanf(buf,"FAPI_PRACH_FORMAT_4_PEAK_RATIO=%d",&(paramThresholdVal->prachFormat4Threshold)) > 0)
        {
            printf("FAPI_PRACH_FORMAT_4_PEAK_RATIO[%d]\r\n", paramThresholdVal->prachFormat4Threshold);
        }
        if (sscanf(buf,"FAPI_PRACH_FORMAT_0_PEAK_RATIO=%d",&(paramThresholdVal->prachFormat0Threshold)) > 0)
        {
            printf("FAPI_PRACH_FORMAT_0_PEAK_RATIO[%d]\r\n", paramThresholdVal->prachFormat0Threshold);
        }
        if (sscanf(buf,"PS_SCANNING_REQ_ADVANCE_SF=%d",&(paramThresholdVal->psScanningReqAdvanceSf)) > 0)
        {
            printf("PS_SCANNING_REQ_ADVANCE_SF[%d]\r\n",paramThresholdVal->psScanningReqAdvanceSf);
        }

        if (sscanf(buf,"FAPI_SCANNING_REQ_ADVANCE_SF=%d",&(paramThresholdVal->psScanningReqAdvanceSf)) > 0)
        {
            printf("FAPI_SCANNING_REQ_ADVANCE_SF[%d]\r\n", paramThresholdVal->psScanningReqAdvanceSf);
        }
        /* for meanwhile filling only for cellIndex 0*/
        psScanIndexInfo_g[0].advanceScanSf = paramThresholdVal->psScanningReqAdvanceSf;

    }
    fclose(fp);
    return paramThresholdVal;
#endif
    
}
/*SPR 21347 fix end*/
#endif
#ifdef FLEXRAN
static ipcStatisticStruct ipcStat;
static char queue_init_done = FALSE;

ipc_queue_s queue_config[IPC_QUEUE_NUMBER];

void *g_pSharedMemAddr[FLEXRAN_MAX_CELL_NUM][IPC_QUEUE_NUMBER];
//char g_txBBUBuf[MAX_BBU_PKG_SIZE];


UInt32 g_flexran_channelId[FLEXRAN_MAX_CELL_NUM] = {0xFFFFFFFF, 0xFFFFFFFF};

#ifdef IPC_SEMAPHORE
sem_t *g_sem_phy2mac[FLEXRAN_MAX_CELL_NUM];
sem_t *g_sem_mac2phy[FLEXRAN_MAX_CELL_NUM];
#endif

static void flexran_ipc_ready_check(unsigned int cell_num, unsigned char type, int mac_cell_id)
{
    int i=0;
    unsigned long magicSend=IPC_READY_MAGIC_NUMBER, magicRecv=0;
    int cell_id = 0;

    if(IPC_USED_BY_PHY == type)
    {
        for(cell_id = 0; cell_id < cell_num; cell_id++)
        {	
            flexran_ipc_send_msg(cell_id, FAPI_SHMA_CH_PARA_RSP, (char*)&magicSend, sizeof(magicSend));
        }

        for(cell_id = 0; cell_id < cell_num; cell_id++)
        {
            while(1)
            {
                if(0 < flexran_ipc_recv_msg(cell_id, FAPI_SHMA_CH_PARA_REQ, (char*)&magicRecv, sizeof(magicRecv)) && 
                        magicRecv == IPC_READY_MAGIC_NUMBER)
                {
                    break;
                }
                if((++i%10) == 0)
                {
                    printf("Cell %d: PHY process wait %ds, MAC process not ready!\n", cell_id, i);
                }
                sleep(1);
            }
        }
    }
    else
    {
        while(1)
        {
            if(0 < flexran_ipc_recv_msg(mac_cell_id, FAPI_SHMA_CH_PARA_RSP, (char*)&magicRecv, sizeof(magicRecv)) && 
                    magicRecv == IPC_READY_MAGIC_NUMBER)
            {
                break;
            }
            if((++i%10) == 0)
            {
                printf("Cell %d: MAC wait %ds, PHY process not ready!\n", mac_cell_id, i);
            }
            sleep(1);
        }
        flexran_ipc_send_msg(mac_cell_id, FAPI_SHMA_CH_PARA_REQ, (char*)&magicSend, sizeof(magicSend));

        printf("MAC process IPC ready!\n");
    }
}


int flexran_ipc_init(unsigned int cell_num, unsigned char type, int mac_cell_id)
{

    int ret=IPC_SUCCESS;
    //int ipcId = 0;
    int shmId = -1;
    int cell_id = 0, queue_id = 0, share_memory_size = 0;
    key_t key;

    ipc_queue_s t_queue_para[IPC_QUEUE_NUMBER] =
    {
        {"para_reg_queue", 	    FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"cfg_req_queue", 	    FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"start_req_queue",     FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"stop_req_queue", 	    FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"dl_cfg_reg_queue", 	FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"ul_cfg_req_queue", 	FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"hi_dci0_req_queue", 	FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"tx_req_queue", 	    FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"para_rsp_queue",    	FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"cfg_rsp_queue", 	    FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"stop_ind_queue",   	FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"error_ind_queue", 	FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"subframe_ind_queue",	FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"harq_ind_queue", 	    FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"crc_ind_queue", 	    FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"ulsch_ind_queue", 	FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"rach_ind_queue",    	FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"srs_ind_queue", 	    FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"sr_ind_queue", 	    FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"cqi_ind_queue", 	    FLEXRAN_MAX_QUEUE_SIZE, FLEXRAN_MAX_MSG_SIZE},
        {"dl_buffer", 		    1,  FLEXRAN_DL_BUFFER_SIZE},
        {"ul_buffer", 		    1,  FLEXRAN_UL_BUFFER_SIZE},
        {"bbu_pkg_in_queue",	64, MAX_BBU_PKG_SIZE},
        {"bbu_pkg_out_queue",	64, MAX_BBU_PKG_SIZE}
    };


    memcpy(queue_config, t_queue_para, sizeof(ipc_queue_s) * IPC_QUEUE_NUMBER);

    for(cell_id = 0; cell_id < cell_num; cell_id++)
    {
	if (mac_cell_id != cell_id)
	{
	 //  g_pSharedMemAddr[cell_id][queue_id] = NULL;
	    continue;
	}
        for(queue_id = 0; queue_id < IPC_QUEUE_NUMBER; queue_id++)
        {
            key = ftok("/tmp/", (int)queue_id + cell_id*IPC_QUEUE_NUMBER);
            if(key == -1)
            {
                printf("ftok error for cell:%d, queue:%d",cell_id, queue_id);
                return -1;
            }

            /* PHY created share memory, while MAC open share memory */
            if(IPC_USED_BY_PHY == type )
            {
                share_memory_size = sizeof(memory_header_s) + queue_config[queue_id].counter * (
                        queue_config[queue_id].size + sizeof(unsigned int));

                /*create shared memory*/
                shmId = shmget(key, share_memory_size, 0666|IPC_CREAT);
                if(shmId == -1)
                {
                    perror("shmget error");
                    printf("shmget error for queue:%d", queue_id);
                    return -1;
                }
            }
            else // IPC_USED_BY_MAC
            {
                if (mac_cell_id != cell_id)
                {
                    continue;
                }

                /*open existed shared memory*/
                shmId = shmget(key, 0, 0666|IPC_CREAT);
                if(shmId == -1)
                {
                    perror("shmget error");
                    printf("shmget error for queue:%d", queue_id);
                    return -1;
                }
            }
            /*mapped the shared memory to current process address space*/
            g_pSharedMemAddr[cell_id][queue_id] = shmat(shmId, NULL, 0);
            if(g_pSharedMemAddr[cell_id][queue_id] == (void *)-1)
            {
                perror("shmat error");
                printf("shmat error for queue:%d", queue_id);
                return -1;
            }

            if(IPC_USED_BY_PHY == type)
            {
                memset(g_pSharedMemAddr[cell_id][queue_id], 0, share_memory_size);
                printf("Shared memory for cell %d queue:%s created \n", cell_id, queue_config[queue_id].name);
            }
            else
            {
                printf("Shared memory for cell_id %d queue:%s found \n", cell_id, queue_config[queue_id].name);
            }
        }
    }

    /* create segments for DL data payload to be sent to L1 */

    UInt8 cellId = 0;
    UInt8 numSegments = 0;
    UInt64 segOffset = 0;
    for (cellId = 0; cellId < cell_num; cellId++)
    {
        segOffset = 0;
	if (mac_cell_id != cellId)
	{
		continue;
	}
        for (numSegments = 0; numSegments < 4; numSegments++)
        {
            g_pSharedMemAddrSegments[cellId][numSegments] = ((UInt8 *)g_pSharedMemAddr[cellId][FAPI_SHMA_DL_BUFFER] + segOffset);
            segOffset += 20000; /* each segment will be of size 20000. Max size required is of 18883 */
	}
    }

#ifdef IPC_SEMAPHORE
    // create semaphore
    if(type == IPC_USED_BY_PHY)
    {
        for(cell_id = 0; cell_id < cell_num; cell_id++)
        {	
            char name[256];
            sem_t *sem = NULL;

            sprintf(name, "cran_sem_phy2mac_cell%d", cell_id);
            sem_unlink(name); // remove old semaphore if exist
            sem = sem_open(name, O_CREAT | O_EXCL, 0600, 0);
            if (sem == SEM_FAILED)
            {
                printf("flexran_ipc_init ERROR: sem_open() failed, name = %s, err = %d:%s\n", name, errno, strerror(errno));
                return -1;			
            }
            else
            {
                g_sem_phy2mac[cell_id] = sem;
            }

            sprintf(name, "cran_sem_mac2phy_cell%d", cell_id);
            sem_unlink(name); // remove old semaphore if exist
            sem = sem_open(name, O_CREAT | O_EXCL, 0600, 0);
            if (sem == SEM_FAILED)
            {
                printf("\nflexran_ipc_init().. ERROR!!! sem_open() failed, name = %s, err = %d:%s\n", 
                        name, errno, strerror(errno));
                return -1;			
            }
            else
            {
                g_sem_mac2phy[cell_id] = sem;
            }
        }
    }
    else //IPC_USED_BY_MAC
    {
        char name[256];
        sem_t *sem = NULL;

        sprintf(name, "cran_sem_phy2mac_cell%d", mac_cell_id);
        sem = sem_open(name, O_EXCL, 0600, 0);
        if (sem == SEM_FAILED)
        {
            printf("flexran_ipc_init ERROR: sem_open() failed, name = %s, err = %d:%s\n", name, errno, strerror(errno));
            return -1;			
        }
        else
        {
            g_sem_phy2mac[mac_cell_id] = sem;
        }

        sprintf(name, "cran_sem_mac2phy_cell%d", mac_cell_id);
        sem = sem_open(name, O_EXCL, 0600, 0);
        if (sem == SEM_FAILED)
        {
            printf("flexran_ipc_init ERROR: sem_open() failed, name = %s, err = %d:%s\n", name, errno, strerror(errno));
            return -1;			
        }
        else
        {
            g_sem_mac2phy[mac_cell_id] = sem;
        }

    }

#endif

    queue_init_done = TRUE;
    initWiresharkLoggingInterface(mac_cell_id);
    flexran_ipc_ready_check(cell_num, type, mac_cell_id);

    return ret;
}

void flexran_ipc_exit()
{
    //int shmId = -1;
    unsigned int queue_id = 0;

    for(queue_id = 0; queue_id < IPC_QUEUE_NUMBER; queue_id++)
    {
        if(-1 == shmdt(g_pSharedMemAddr[queue_id]))
        {
            printf("Detach shared memory for queue:%s error!\n", queue_config[queue_id].name);
        }
    }
    printf("Detach all shared memory.\n");

#ifdef IPC_SEMAPHORE
    int cell_id = 0;
    for(cell_id = 0; cell_id < FLEXRAN_MAX_CELL_NUM; cell_id++)
    {
        if (g_sem_phy2mac[cell_id] != NULL)
        {
            sem_close(g_sem_phy2mac[cell_id]);
            g_sem_phy2mac[cell_id] = NULL;
        }
        if (g_sem_mac2phy[cell_id] != NULL)
        {
            sem_close(g_sem_mac2phy[cell_id]);
            g_sem_mac2phy[cell_id] = NULL;
        }
    }
#endif

    queue_init_done = FALSE;
    return;
}

int flexran_ipc_is_channel_ready(unsigned int cell_id, FAPI_SHMA_CH_T queue_id, char* buf, unsigned int len)
{
    //int msgOffset = 0;

    memory_header_s *psShMem = (memory_header_s *)g_pSharedMemAddr[cell_id][queue_id];

    if(queue_init_done == FALSE)
    {
        printf("flexran_ipc_is_channel_ready ERROR: queue_init_done is 0\n");
        return 0;
    }

    if( (NULL == buf) || (len > queue_config[queue_id].size) )
    {
        if(buf == NULL)
        {
            printf("flexran_ipc_is_channel_ready ERROR: buf == NULL, queue:%s!\n", queue_config[queue_id].name);
        }
        else
        {
            printf("flexran_ipc_is_channel_ready ERROR: len:%u, queue Len:%d,queue:%s!\n", 
                    len, queue_config[queue_id].size, queue_config[queue_id].name);
        }

        return 0;
    }

    if(psShMem->nWriteIdx >= queue_config[queue_id].counter)
    {
        printf("flexran_ipc_is_channel_ready ERROR nWriteIdx:%d of queue:%s.\n", psShMem->nWriteIdx, queue_config[queue_id].name);
        return 0;
    }

    if( ((psShMem->nWriteIdx + 1) % queue_config[queue_id].counter) == psShMem->nReadIdx )
    {
        //printf("ERROR Queue %d Full, write index:%d, read index:%d\n",queue_id, psShMem->nWriteIdx,
        //psShMem->nReadIdx);
        return 0;
    }

    return 1;
}


int flexran_ipc_send_msg(unsigned int cell_id, FAPI_SHMA_CH_T queue_id, char* buf, unsigned int len)
{
    int msgOffset = 0;

    memory_header_s *psShMem = (memory_header_s *)g_pSharedMemAddr[cell_id][queue_id];

    if(queue_init_done == FALSE)
        return IPC_NOT_INIT_ERR;

    if( (NULL == buf) || (len > queue_config[queue_id].size) )
    {
        if(buf == NULL)
        {
            fprintf(stderr,"ERROR in flexran_ipc_send_msg buf == NULL, queue:%s!\n", queue_config[queue_id].name);
        }
        else
        {
            fprintf(stderr,"ERROR in flexran_ipc_send_msg len:%u, queue Len:%d,queue:%s!\n", 
                    len, queue_config[queue_id].size, queue_config[queue_id].name);
        }

        return IPC_INPUT_PARA_ERR;
    }

    if(psShMem->nWriteIdx >= queue_config[queue_id].counter)
    {
        fprintf(stderr,"ERROR nWriteIdx:%d of queue:%s.\n", psShMem->nWriteIdx, queue_config[queue_id].name);
        return IPC_INPUT_PARA_ERR;
    }

    if( ((psShMem->nWriteIdx + 1) % queue_config[queue_id].counter) == psShMem->nReadIdx )
    {
        fprintf(stderr,"ERROR Queue %d Full, write index:%d, read index:%d\n",queue_id, psShMem->nWriteIdx,
                psShMem->nReadIdx);
        return IPC_SEND_QUEUE_FULL_ERR;
    }

    /*get message offset*/
    msgOffset = psShMem->nWriteIdx * (sizeof(unsigned int) + queue_config[queue_id].size);

    /*fill in the length*/
    memcpy((psShMem->pMsgBuf + msgOffset), (UInt8 *)&len, sizeof(int));//length of a message

    /*fill in the message payload*/
    memcpy((void *)(psShMem->pMsgBuf + msgOffset + sizeof(unsigned int)), buf, len);

    /*update nWriteIdx*/
    psShMem->nWriteIdx = (psShMem->nWriteIdx + 1) % queue_config[queue_id].counter;

    return len;
}

int flexran_ipc_recv_msg(unsigned int cell_id, FAPI_SHMA_CH_T queue_id, char* buf, unsigned int len)
{
    int data_len = 0, msgOffset = 0;

    memory_header_s *psShMem = (memory_header_s *)g_pSharedMemAddr[cell_id][queue_id];

    if(queue_init_done == FALSE)
        return IPC_NOT_INIT_ERR;

    if(NULL == buf)
    {
        fprintf(stderr, "ERROR in flexran_ipc_recv_msg buf == NULL.\n");
        return IPC_INPUT_PARA_ERR;
    }

    if(psShMem->nReadIdx == psShMem->nWriteIdx)//no message in queue
        return IPC_SUCCESS;

    if(psShMem->nReadIdx >= queue_config[queue_id].counter)
    {
        fprintf(stderr,"ERROR nReadIdx:%d of queue:%s.\n", psShMem->nReadIdx, queue_config[queue_id].name);
        return IPC_INPUT_PARA_ERR;
    }

    /*get message offset*/
    msgOffset = psShMem->nReadIdx * (sizeof(unsigned int) + queue_config[queue_id].size);

    /*extract the message length*/
    memcpy((UInt8 *)&data_len , (psShMem->pMsgBuf + msgOffset), sizeof(int)) ;
    if((len < data_len) || (data_len <= 0))
    {
        fprintf(stderr, "ERROR in flexran_ipc_recv_msg, len:%d, data_len:%d.\n", len, data_len);
        return IPC_RECV_BUF_ERR;
    }

    /*extract the message payload*/
    memcpy(buf, (void *)(psShMem->pMsgBuf + msgOffset + sizeof(int)), data_len);

    /*update nReadIdx*/
    psShMem->nReadIdx = (psShMem->nReadIdx + 1) % queue_config[queue_id].counter;

    return data_len;
}

int flexran_ipc_set_statistic(ipcStatisticStruct* data)
{
    if(queue_init_done == FALSE)
        return IPC_NOT_INIT_ERR;

    if(data == NULL)
        return IPC_INPUT_PARA_ERR;

    *data = ipcStat;
    return IPC_SUCCESS;
}

char* flexran_ipc_get_dl_databuffer(unsigned int cell_index)
{
    if (cell_index >= FLEXRAN_MAX_CELL_NUM)
        return NULL;

    if(queue_init_done == FALSE)
        return NULL;

    return (char*) g_pSharedMemAddr[cell_index][FAPI_SHMA_DL_BUFFER];

}

char* flexran_ipc_get_ul_databuffer(unsigned int cell_index)
{
    if (cell_index >= FLEXRAN_MAX_CELL_NUM)
        return NULL;

    if(queue_init_done == FALSE)
        return NULL;

    return (char*) g_pSharedMemAddr[cell_index][FAPI_SHMA_UL_BUFFER];

}

#ifdef IPC_SEMAPHORE
int flexran_ipc_notify_mac_ready(unsigned int cell_index)
{
    if (g_sem_phy2mac[cell_index] == NULL)
    {
        printf("ipc_notify_mac_ready ERROR: sem is not created, cell_index = %d\n", cell_index);
        return -1;
    }

    if (sem_post(g_sem_phy2mac[cell_index]) != 0)
    {
        printf("ipc_notify_mac_ready ERROR: sem_post failed, cell_index = %d\n", cell_index);
        return -1;
    }	

    return 0;
}


int flexran_ipc_notify_phy_ready(unsigned int cell_index)
{
    if (g_sem_mac2phy[cell_index] == NULL)
    {
        printf("ipc_notify_phy_ready ERROR: sem is not created, cell_index = %d\n", cell_index);
        return -1;
    }

    if (sem_post(g_sem_mac2phy[cell_index]) != 0)
    {
        printf("ipc_notify_phy_ready ERROR: sem_post failed, cell_index = %d\n", cell_index);
        return -1;
    }	

    return 0;
}

int flexran_ipc_timewait_phy_ready(unsigned int cell_index, const struct timespec *timeout)
{
    if (g_sem_phy2mac[cell_index] == NULL)
    {
        printf("ipc_mac_timewait_phy_ready ERROR: sem is not created, cell_index = %d\n", cell_index);
        return -1;
    }

    return sem_timedwait(g_sem_phy2mac[cell_index], timeout);
}

int flexran_ipc_timewait_mac_ready(unsigned int cell_index, const struct timespec *timeout)
{
    if (g_sem_mac2phy[cell_index] == NULL)
    {
        printf("ipc_phy_timewait_mac_ready ERROR: sem is not created, cell_index = %d\n", cell_index);
        return -1;
    }

    return sem_timedwait(g_sem_mac2phy[cell_index], timeout);
}

int flexran_ipc_wait_phy_ready(unsigned int cell_index)
{
    if (g_sem_phy2mac[cell_index] == NULL)
    {
        fprintf(stderr,"ipc_wait_phy_ready ERROR: sem is not created, cell_index = %d\n", cell_index);
        return -1;
    }

    int ret = sem_wait(g_sem_phy2mac[cell_index]);
    if (ret != 0)
    {
        fprintf(stderr,"ipc_wait_phy_ready ERROR: sem_wait() failed ret = %d, err = %d:%s\n", ret, errno, strerror(errno));
    }

    return ret;
}

int flexran_ipc_wait_mac_ready(unsigned int cell_index)
{
    if (g_sem_mac2phy[cell_index] == NULL)
    {
        printf("ipc_wait_mac_ready ERROR: sem is not created, cell_index = %d\n", cell_index);
        return -1;
    }

    int ret = sem_wait(g_sem_mac2phy[cell_index]);
    if (ret != 0)
    {
        printf("ipc_wait_mac_ready ERROR: sem_wait() failed ret = %d, err = %d:%s\n", ret, errno, strerror(errno));
    }	

    return ret;
}

int flexran_ipc_trywait_phy_ready(unsigned int cell_index)
{
    if (g_sem_phy2mac[cell_index] == NULL)
    {
        printf("ipc_trywait_phy_ready ERROR: sem is not created, cell_index = %d\n", cell_index);
        return -1;
    }

    return sem_trywait(g_sem_phy2mac[cell_index]);
}

int flexran_ipc_trywait_mac_ready(unsigned int cell_index)
{
    if (g_sem_mac2phy[cell_index] == NULL)
    {
        printf("ipc_trywait_mac_ready ERROR: sem is not created, cell_index = %d\n", cell_index);
        return -1;
    }

    return sem_trywait(g_sem_mac2phy[cell_index]);
}

#endif
#endif




