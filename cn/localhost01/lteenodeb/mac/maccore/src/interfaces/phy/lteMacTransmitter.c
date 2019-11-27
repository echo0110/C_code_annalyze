/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacTransmitter.c,v 1.2.6.1.4.2.2.11.4.1 2010/11/08 06:06:25 gur10121 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This File will provide all the functions needed to transmit 
 *                     the control and data buffer to the phy layer, the functions
 *                     provided in this file may be called from any other module also.
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacTransmitter.c,v $
 * Revision 1.2.6.1.4.2.2.11.4.1  2010/11/08 06:06:25  gur10121
 * Semi static changes merging
 *
 * Revision 1.2.6.1.4.2.2.11  2010/10/27 08:43:35  gur20056
 * removed a temp fix
 *
 * Revision 1.2.6.1.4.2.2.10  2010/10/25 10:15:07  gur23971
 * warning removed
 *
 * Revision 1.2.6.1.4.2.2.9.2.3  2010/10/29 10:25:47  gur10121
 *  Semi Static UT fixes
 *
 * Revision 1.2.6.1.4.2.2.9.2.2  2010/10/27 17:13:54  gur10121
 *  UT fixes + review comments incorporation
 * 
 * 
 * Revision 1.2.6.1.4.2.2.8.2.1  2010/10/21 07:57:17  gur10121
 * Semi static UT fixes
 *
 * Revision 1.2.6.1.4.2.2.9  2010/10/14 17:18:12  gur19413
 * DMRS FIXES
 *
 * Revision 1.2.6.1.4.2.2.8  2010/10/12 12:27:13  gur20439
 * temporariy fix for taglen field set in DL DATA TX Request and BCH Pdu
 *
 * Revision 1.2.6.1.4.2.2.7  2010/10/11 11:28:58  gur10121
 * Purify UMR Fix
 *
 * Revision 1.2.6.1.4.2.2.6  2010/10/08 13:00:02  gur19942
 * As UL sch pdu is being prepared after DCI 0, dont reset CCE.
 *
 * Revision 1.2.6.1.4.2.2.5  2010/10/07 12:27:59  gur10694
 * FAPI PCH UT fix
 *
 * Revision 1.2.6.1.4.2.2.4  2010/09/29 08:41:03  gur04640
 * Data buffer size increased to take care of MIMO case
 *
 * Revision 1.2.6.1.4.2.2.3  2010/09/27 14:38:28  gur10694
 * UE config Fix and Review comments
 *
 * Revision 1.2.6.1.4.2.2.2  2010/09/23 13:04:32  gur20491
 * segmentation fault removed
 *
 * Revision 1.2.6.1.4.2.2.1  2010/09/21 15:46:11  gur20491
 * FAPI changes
 *
 * Revision 1.67.26.5  2010/08/30 16:28:31  gur24420
 * removed unwanted code
 *
 * Revision 1.67.26.4  2010/08/12 10:18:25  gur24420
 * Changes made to encode MAC-PHY Interfaces messages in little endian format
*
* Revision 1.67.26.3  2010/08/09 15:05:44  gur23951
* ADDED SUPPORT FOR CORRECT REDUNDANCY VERSION VALUE
*
* Revision 1.67.26.2  2010/08/03 10:53:08  gur24420
* Converted transport notation to Network byte order
*
* Revision 1.67.26.1  2010/07/26 09:29:25  gur23951
* Changes done to toggle NDI for new transmission
*
* Revision 1.67  2009/11/20 06:12:47  gur11912
* passed system Frame and Sub Frame in txDLControl, data and common channel legs
*
* Revision 1.66  2009/11/02 14:15:09  gur11912
* MACRO used instead of magic numbers
*
* Revision 1.65  2009/10/27 07:12:39  gur19413
* reverted RAW Socket changes
*
* Revision 1.64  2009/10/21 05:10:42  gur19413
* Socket Functionality Modified
*
* Revision 1.63  2009/10/05 08:07:03  gur11912
* optimization changes
*
* Revision 1.62  2009/09/23 13:31:13  gur18550
* Added More Logs
*
* Revision 1.61  2009/09/23 04:42:35  gur11912
* optimized for harqSubFrameNum
*
* Revision 1.60  2009/09/02 19:45:41  gur20052
* dded warning/panic , when MAC is sending crnti=0
*
* Revision 1.59  2009/08/27 14:29:10  gur11912
* txDLControl and txDLDataMsg work on only valid cceArray
*
* Revision 1.58  2009/08/27 07:03:21  gur20439
* TA DRX fix found in SIT.
*
* Revision 1.57  2009/08/26 07:28:43  gur12140
* Bug Solved
*
* Revision 1.55  2009/08/18 16:25:48  gur11912
* modified for thread synchronization
*
* Revision 1.54  2009/08/03 14:06:39  gur11912
* HI PDU are formed before DCI PDUs
*
* Revision 1.53  2009/07/29 09:11:47  gur15697
* UT bugfix
*
* Revision 1.52  2009/07/29 07:00:20  gur15697
* message identifier is set to a valid value
*
* Revision 1.51  2009/07/28 14:35:47  gur11912
* HARQ Timer start moved to DL Data Message
*
* Revision 1.50  2009/07/23 07:13:06  gur18569
* changes regarding LTE_MAC_UT_LOG
*
* Revision 1.49  2009/07/21 08:18:00  gur15697
* INVALID_HARQ_ID == tempDciPduInfo_p->harqProcessId added
*
* Revision 1.48  2009/07/21 05:32:30  gur20052
* Removed the redundant switches related to MsgType
*
* Revision 1.47  2009/07/17 11:20:35  gur11912
* marked CCE as Free in createAndSendDataBufferReq
*
* Revision 1.46  2009/07/12 08:05:55  gur15697
* UL Flow bugs
*
* Revision 1.45  2009/07/12 05:17:12  gur15697
* tempHIPduNode_p->macHIPduInfo[tempHIPduNode_p->count]
* changed to tempHIPduNode_p->macHIPduInfo[tempHIPduNode_p->count - 1]
*
* Revision 1.44  2009/07/11 08:41:19  gur18550
* Removed redundant functions of bit shifting
*
* Revision 1.43  2009/07/10 06:54:08  gur15697
* UT Bug fix
*
* Revision 1.42  2009/07/08 13:50:59  gur11912
* corrected DLSCH TAG index in case of MIMO
*
* Revision 1.41  2009/07/07 12:13:59  gur19413
* Corrected length to be sent to PHY
*
* Revision 1.40  2009/07/06 16:05:12  gur11912
* Fixed for correct API Length
*
* Revision 1.39  2009/07/03 10:47:50  gur18569
* fixed UT bug
*
* Revision 1.38  2009/07/03 10:33:51  gur11912
* TB Size populated in Bits
*
* Revision 1.37  2009/07/03 08:46:16  gur11912
* replaced tab with spaces
*
* Revision 1.36  2009/07/03 05:19:38  gur19413
* UT Bugs Fixed
*
* Revision 1.35  2009/07/02 16:23:56  gur19413
* fixed UT bug
*
* Revision 1.34  2009/07/01 08:00:35  gur18550
* Function Updated
*
* Revision 1.33  2009/06/25 13:29:37  gur18550
* merging error removed
*
* Revision 1.32  2009/06/25 11:27:27  gur18569
* freeing CCE frm txDLControl itselt in case of DCI_FORMAT_0
*
* Revision 1.31  2009/06/24 16:18:56  gur18550
* File Updated
*
* Revision 1.30  2009/06/24 16:12:34  gur18550
* Final review Comments Inc
*
* Revision 1.29  2009/06/23 15:12:12  gur18569
* removed debug code
*
* Revision 1.28  2009/06/23 15:10:52  gur18569
* fixed bug of resetting CCE free
*
* Revision 1.27  2009/06/23 08:20:53  gur18569
* fixed bugs
*
* Revision 1.26  2009/06/22 08:16:56  gur18550
* Common Channels
*
* Revision 1.25  2009/06/19 13:56:37  gur18569
* replaced the loop of cceNumber in txDLDataMsgReq max index from MAX_CCE to maxCCE which is set equal to maxCCEAvailable_g
*
* Revision 1.24  2009/06/19 12:48:46  gur18569
* added mesg type cases ,starting buffer index if SHM_INTF at 8 in txDLDataMsg
*
* Revision 1.23  2009/06/19 09:22:05  gur18550
* Common Channels related Changes
*
* Revision 1.22  2009/06/19 09:06:51  gur18550
* txData.....
*
* Revision 1.21  2009/06/19 08:52:41  gur18550
* Changes due to = CCE_FREE
*
* Revision 1.20  2009/06/19 06:40:56  gur18550
* harqSubFrameNum
*
* Revision 1.19  2009/06/19 04:47:24  gur18550
* ScheduledExpiry Tick
*
* Revision 1.18  2009/06/18 12:43:14  gur18550
* HARQ TIMER
*
* Revision 1.17  2009/06/18 07:44:16  gur18569
* added check not to sent ctrl msg to phy whn ther's no msg at all
*
* Revision 1.16  2009/06/17 12:25:16  gur18550
* isCCEFree used
*
* Revision 1.15  2009/06/12 05:01:12  gur18550
* MIB &  Shared Memory Related Changes
*
* Revision 1.14  2009/06/11 08:31:27  gur18550
* Function Updated
*
* Revision 1.13  2009/06/11 07:56:22  gur18550
* Function Updated
*
* Revision 1.12  2009/06/11 05:24:08  gur18550
* Function Updated
*
* Revision 1.11  2009/06/11 05:19:18  gur18550
* MAC_REGISTER_REQ Added on Revant's Request
*
* Revision 1.10  2009/06/08 15:46:37  gur12140
* Removed Linking Errors
*
* Revision 1.9  2009/05/28 12:43:22  gur18550
* Review Comments indorporated
*
* Revision 1.8  2009/05/23 12:40:09  gur18550
* lteMacTransmitter.c
*
* Revision 1.7  2009/05/20 07:00:54  gur18550
* PCCH related changes
*
* Revision 1.6  2009/05/20 06:46:38  gur18550
* DCI FORMAT 2/2A related changes
*
* Revision 1.5  2009/05/20 04:39:36  gur18550
* MIB related changes
*
* Revision 1.4  2009/05/19 15:22:15  gur18550
* Changes in Memory allocation for Data
*
* Revision 1.3  2009/05/19 06:22:49  gur18550
* Function Updated
*
* Revision 1.2  2009/05/14 16:52:11  gur18550
* Function Updated
*
* Revision 1.1  2009/04/30 05:05:40  gur18550
* INITIAL VERSION
*
*****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacCellConfMgr.h"
#include "lteMacTransmitter.h"
#include "lteMacUtilityAndSharedStructs.h"
#include "lteFapiMacPhyInterface.h"
#include "lteMacFAPIApi.h"



#ifdef TDD_CONFIG
#include "lteMacTddMgr.h"
#endif

#include "lteMacPCCH.h"
/* SPR 3267 START */
#include "lteMacDLPowerControlData.h"
/* SPR 3267 END */
#include "lteMacPUSCH.h"
#include "ltePerfStats.h"


#ifdef LTE_EMBMS_SUPPORTED
#include "lteMacEmbmsTypes.h"
#endif
/*CA Changes start*/ 
/*OPT_SPR_4119_4120_4122_4123_CHANGES_START*/
UInt8            *pHIDCICfgMsgBuff_gp[MAX_NUM_CELL] ;
UInt8            *pDLCfgMsgBuff_gp[MAX_NUM_CELL]    ;
UInt8            *pTxReqMsgBuff_gp[MAX_NUM_CELL]    ;
/*OPT_SPR_4119_4120_4122_4123_CHANGES_END*/
/* SPR 19288 change start */
/* SPR 19288 change end */
UInt8 *pULConfigMsg_gp[MAX_NUM_CELL][MAX_SUB_FRAME] ;
extern MacRetType getNumRepetitions(UInt8 val, UInt8 *schedulingInfoSIB1_NB_p);
/******************************************************************************
  Private Definitions
 *****************************************************************************/
#define MAC_MOD_TRANSMITTER "TRANSMITTER"
/*3053 */

#ifdef TDD_CONFIG
extern FAPI_dlDCIPduInfo_st *(*fillDciFormat0PduFunction)(FAPI_dlDCIPduInfo_st *, DCIPduInfo *, UInt32, InternalCellIndex);
#elif FDD_CONFIG
/* +- SPR 17777 */

#if defined(UTFWK_SIMULATION) || defined(ULHARQ_TIMER_PROC) /*SPR 18979 +-*/
extern FAPI_dlDCIPduInfo_st *(*fillDciFormat0PduFunction)(FAPI_dlDCIPduInfo_st *, DCIPduInfo *, UInt32, UInt32, MsgType,InternalCellIndex);
#else
extern FAPI_dlDCIPduInfo_st *(*fillDciFormat0PduFunction)(FAPI_dlDCIPduInfo_st *, DCIPduInfo *,  UInt32, MsgType,InternalCellIndex);
#endif
/* +- SPR 17777 */
/*SPR 21001 Changes end */
#endif	
#ifdef UTFWK_SIMULATION
extern void processGrant(UInt32 ueIndex,UInt32 tempHarqProcessId);
#endif
extern UInt16 getPHICHPowerOffset(UInt8 widebandCqi, InternalCellIndex internalCellIndex);
/*START_DYN_PDCCH_TDD_CHANGE*/
/* SPR 19288 change start */
/* SPR 19288 change end */

/*END_DYN_PDCCH_TDD_CHANGE*/
/*START : DYN_PDCCH*/
extern UInt8 currentCFIValue_g[MAX_NUM_CELL];/*DYN_PDCCH*/
/*END   : DYN_PDCCH*/
/* Start 128 UE: Memory Changes */
/* 
 ** To transmit packet, the memory consumes and free per tti but it
 ** can take one extra TTI to free the consume memory. Therefore,
 ** 2 extra pool has been created.
 */
#define NUM_BLK_FOR_DL_MSG_BUF 3
/* End 128 UE: Memory Changes */
/*EICIC +*/
extern UInt8 usableAbsPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN];
extern  UInt8 usableAbsPatternIndex_g[MAX_NUM_CELL];
/*EICIC -*/
/******************************************************************************
  Private Types
 *****************************************************************************/
#ifdef LTE_EMBMS_SUPPORTED
/***KPI stats****/
UInt64 kpiMchPduTBSize_g = 0;
/* SPR 18296 fix start*/
extern UInt8 dci1CAreaBitMap_g[MAX_NUM_CELL];
extern UInt8 isDci1cOcassion_g[MAX_NUM_CELL];
/* SPR 18296 fix start*/
#endif

#ifdef FLEXRAN
#ifdef FLEXRAN_FAPI_LOGGING
extern SInt32 flexranFapiLoggingEnable_g;
#endif
UInt8 subFrameNum_flexran[2];
UInt16 sysFrameNum_flexran[2];
extern void do_tap_fun(void* data, uint32_t len, InternalCellIndex cellId);

extern UInt32 g_flexran_channelId[2] ; 
UInt64 dlDataOffset[FLEXRAN_MAX_CELL_NUM];
extern UInt8 *g_pSharedMemAddrSegments[FLEXRAN_MAX_CELL_NUM][4] ;
extern void *g_pSharedMemAddr[FLEXRAN_MAX_CELL_NUM][IPC_QUEUE_NUMBER];
#endif

/* SPR 19288 change start */
/* SPR 19288 change end */


/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
extern UInt8 phyReleaseCapability_g;
STATIC  FAPI_dlDCIPduInfo_st *addDciPduToHiDci0(
        FAPI_dlDCIPduInfo_st *pDlDciPdu, DCIPduInfo *pDciPdu,
#ifdef TDD_CONFIG
        DciCCEInfo *pDciCceInfo,
#elif FDD_CONFIG
        DciCCEInfo *pDciCceInfo,UInt32 subFrameNum, 
#endif
        InternalCellIndex internalCellIndex
        );
#ifdef DL_UL_SPLIT
STATIC void convertULConfigPduTypeRBAllocFail (ULSchUciInfo* ulSchUciInfo_p);
#endif
 void fillDciFormat0Pdu(
/* SPR Fix 17928 Start */
#ifdef TDD_CONFIG
        FAPI_dlDCIPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu,UInt32 dci0ForTempUEFlag,
#elif FDD_CONFIG
        /* +- SPR 17777 */
        FAPI_dlDCIPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu,
#if defined(UTFWK_SIMULATION) || defined(ULHARQ_TIMER_PROC)
        UInt32 subFrameNum,
#endif
        UInt32 dci0ForTempUEFlag,MsgType msgType,
        /* +- SPR 17777 */
#endif
        InternalCellIndex internalCellIndex);
/* SPR Fix 17928 End */

/* MAC_TTI_LOG Start */
#ifdef SHM_PRINT_ENABLED
STATIC  void fillttiDciLoggingInfo (ttiDciLoggingInfo *log_ptr,
		DciCCEInfo *tempDciCCEInfo_p, InternalCellIndex internalCellIndex);
#endif
/* MAC_TTI_LOG End */
/* + PRS_CHANGES */
static  FAPI_dlConfigPDUInfo_st *addPrsPduToDlConfig(
		FAPI_dlConfigPDUInfo_st *pDlConfigPdu, 
		InternalCellIndex internalCellIndex);

/* - PRS_CHANGES */
/*Cyclomatic Complexity changes - starts here */
STATIC  void  handleCommChControlMsg(MsgType msgType,
        FAPI_dlConfigPDUInfo_st **pPrevPdu,
        FAPI_dlConfigPDUInfo_st **pNextPdu,
        DCIPduInfo *pDciPduInfo,
        DciCCEInfo *pDciCceInfo,
        UInt32 *pduIndex,
        UInt32 *numPdu,
        UInt32 *length,
        UInt32 *numPdschRnti,
        UInt32 *numDci,
        /* + TM7_EPRE */
        FAPI_VendorSpecificForDLConfig_st *tempVendorSpecificForDLConfig_p, 
        UInt16 *numOfUEs, 
        InternalCellIndex internalCellIndex);

/* - TM7_EPRE */
STATIC  void addDlschPduForInvalidHarqId(DCIPduInfo *pDciPduInfo,
        FAPI_dlConfigPDUInfo_st **pPrevPdu,
        FAPI_dlConfigPDUInfo_st **pNextPdu,
        UInt32 *pduIndex,
        UInt32 *numPdu,
        UInt32 *length,
        UInt32 *numPdschRnti,
        /* + TM7_EPRE */
        FAPI_VendorSpecificForDLConfig_st *tempVendorSpecificForDLConfig_p, 
        UInt16 *numOfUEs, 
	InternalCellIndex internalCellIndex);
/* - TM7_EPRE */
STATIC  void addDlschPduForValidHarqId(DCIPduInfo *pDciPduInfo,
        FAPI_dlConfigPDUInfo_st **pPrevPdu,
        FAPI_dlConfigPDUInfo_st **pNextPdu,
        UInt32 *pduIndex,
        UInt32 *numPdu,
        UInt32 *length,
        UInt32 *numPdschRnti,
        /* +- SPR 17777 */
#ifdef UTFWK_SIMULATION
        UInt32 subFrame,
#endif
        /* +- SPR 17777 */
        UInt8 isTBOneValid,
        UInt8 isTBTwoValid,
        /* + TM7_EPRE */
        FAPI_VendorSpecificForDLConfig_st *tempVendorSpecificForDLConfig_p, 
        UInt16 *numOfUEs,
        InternalCellIndex internalCellIndex);
/* - TM7_EPRE */
STATIC  void addDlschDciPduForCCCHMsg(DCIPduInfo *pDciPduInfo,
		FAPI_dlPduInfo_st **pPrevPdu,
		FAPI_dlPduInfo_st **pNextPdu,
		UInt32 *pduIndex,
		UInt32 *numPdu,
		UInt32 *length
/* SPR 19288 change start */
#if defined(FLEXRAN)
                ,InternalCellIndex internalCellIndex
#endif
/* SPR 19288 change end */
                );
STATIC  void handleCommChDataMsg(MsgType msgType,
		DCIPduInfo *pDciPduInfo,
		FAPI_dlPduInfo_st **pPrevPdu,
		FAPI_dlPduInfo_st **pNextPdu,
		UInt32 *pduIndex,
		UInt32 *numPdu,
		UInt32 *length,
		UInt32 sysFrameNum,
		UInt32 subFrameNum,
		InternalCellIndex internalCellIndex
/* SPR 19288 change start */
/* SPR 19288 change end */
                 );
/*Cyclomatic Complexity changes - ends here*/

/* Cyclomatic_complexity_changes_start */
void processAllocatedCCEArray(
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
		DciCCEContainerInfo *dciCCEContainerInfo_p,
		FAPI_dlPduInfo_st **pNextPdu,
		FAPI_dlPduInfo_st **pPrevPdu,
        UInt32 *pduIndex,
        UInt32 *numPdu,
        UInt32 *length,
		InternalCellIndex internalCellIndex
/* SPR 19288 change start */
/* SPR 19288 change end */
		);

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
UInt32 totalMacDllinkBytesSend_g;
#ifdef UTFWK_SIMULATION
extern void addEntryInHarqList(UInt32 ueIndex,UInt32  subframe, UInt32 rntiType);
extern void addEntryInCrcIndList(UInt32 ueIndex,UInt32  subframe, UInt32 harqId, UInt32 rntiType);
#endif
/* + SPR 17733 */
#ifdef FDD_CONFIG
extern RedundancyVerVal redundancyVerCalc(UInt32 inputValK);
#endif
/* - SPR 17733 */
extern FAPI_dlConfigPDUInfo_st *(*addDlschToDLConfigFunction)(FAPI_dlConfigPDUInfo_st *, DCIPduInfo *,UInt32 , UInt8 
		, UInt8
		,FAPI_VendorSpecificForDLConfig_st *,UInt16 *,InternalCellIndex);
extern FAPI_dlConfigPDUInfo_st *(*addDciToDLConfigFunction)(FAPI_dlConfigPDUInfo_st *, DCIPduInfo *,DciCCEInfo *,InternalCellIndex);

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/****************************************************************************
 * Function Name  :  getTransactionId
 * Inputs         :  None
 * Outputs        :  None
 * Returns        :  transactionId - The value of transaction ID
 * Description    :  This function returms the value of transaction ID, this
 *                   number is generated from a range of 0 to 65535
 ****************************************************************************/
/* + SPR 17439 */
 UInt16 getTransactionId(void)
/* - SPR 17439 */
{
	static UInt32 transactionId = 0;
	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Entry",__func__);
	transactionId++;
	transactionId &= 0xFF;
	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Exit",__func__);
	return (UInt16)transactionId;
}

/*****************************************************************************
 * Function Name  :  createAndSendCmnChannelData 
 * Inputs         :  sysFrameNum- The value of system frame number on which
 *                                the buffer would be sent on PHY,
 *                   subFrameNum - The value sub frame number on which
 *                                 the buffer would be sent on PHY,
 *                   currentGlobalTick - The present Global Tick,
 *                   phyDelay - The delay of the execution leg in terms of the
 *                              number of sub-frames w.r.t. PHY and the current 
 *                              processing being done at MAC(is intended for PHY)
 *                   and dciCCEContainerInfo_p - a pointer to DciCCEContainerInfo
 *                   internalCellIndex - cell id  
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will create and send the data message for the
 *                   message for which the encoder has not been called.
 *****************************************************************************/

 MacRetType createAndSendCmnChannelData(
		UInt16 sysFrameNum,
		UInt8 subFrameNum,
        /* +- SPR 17777 */
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
		UInt8 phyDelay,
#endif
        /* +- SPR 17777 */
		DciCCEContainerInfo* dciCCEContainerInfo_p,
		InternalCellIndex internalCellIndex)
{
	UInt32 dataMesgBytesSent = 0;
	UInt8 *dataBufferForPhy = PNULL;
	UInt32 mesgLenIndexForData = 0;
	UInt32 bufferIndexForData = 0;
	DciCCEInfo* tempDciCCEInfo_p = PNULL;
	DCICCEInfoCmnChannel* tempDCICCEInfoCmnChannel_p = PNULL;    
	UInt8 localCount = dciCCEContainerInfo_p->countCmnChannelMsg ;
	UInt8 cceNumber = 0;
	UInt16 transactionId = 0;
	/* UInt8 localAggregationLvl = 0; TDD Warning Fix */
	DCIPduInfo* tempDciPduInfo_p = PNULL;
	UInt32 bufferIndexStart = 0;
	UInt16 apiLength = 0;
	/* SPR  3444 changes start */
	QSEGMENT segment = {0};
	/* SPR  3444 changes end */
	/*
	   This function is exclusively used to cater the case of common
	   channels, in other words for those cceIndex which were not handled by
	   the Encoder. The loop runs on the dciCCEContainerInfo_p->countCmnChannelMsg
	   and creates the data pdu buffer for all such cceIndex which are in the
	   dciCCEContainerInfo_p->dciCCEInfoCmnChannel[ ] Finally it will dispatch
	   the data Buffer to the PHY.
	 */ 

	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Entry",__func__);
	/*
	   In case of shared memory we need to leave the first 8 bytes.
	 */   
	bufferIndexStart = bufferIndexForData;

	if (dciCCEContainerInfo_p->mibData_p)
	{
		/* 
		   The memory in case of MIB is avaliable at a different pointer of
		   mibData_p placed at a different location for all other the else case '
		   is valid.
		 */   
		dataBufferForPhy = dciCCEContainerInfo_p->mibData_p;
		/*
		   We now update the buffer with the API header segments in the 
		   following lines.
		 */ 
		transactionId = getTransactionId();
		LTE_PHY_INT_SET_U16BIT( (&dataBufferForPhy[bufferIndexForData]),transactionId);
		bufferIndexForData +=SIZEOF_UINT16;
		LTE_PHY_INT_SET_U16BIT( (&dataBufferForPhy[bufferIndexForData]),MAC_MODULE_ID);
		bufferIndexForData +=SIZEOF_UINT16;
		LTE_PHY_INT_SET_U16BIT( (&dataBufferForPhy[bufferIndexForData]),PHY_MODULE_ID);
		bufferIndexForData +=SIZEOF_UINT16;
		/* 
		   In the next two lines we fill the API Identifier for Data message
		 */
		LTE_PHY_INT_SET_U16BIT( (&dataBufferForPhy[bufferIndexForData]),MAC_DL_DATA_MSG_REQ);
		bufferIndexForData +=SIZEOF_UINT16;
		mesgLenIndexForData = bufferIndexForData++;
		/*
		   This value has to be increase by one for the next byte. 
		 */
		bufferIndexForData++;
		LTE_PHY_INT_SET_U16BIT( (&dataBufferForPhy[bufferIndexForData]),sysFrameNum);
		bufferIndexForData +=SIZEOF_UINT16;	
		dataBufferForPhy[bufferIndexForData++] = subFrameNum; 
		dataBufferForPhy[bufferIndexForData++] = INVALID_MSG_IDENTIFIER;

		createDLDataMsgReqForMIB(dataBufferForPhy,
				dciCCEContainerInfo_p,
				&bufferIndexForData);

		apiLength = bufferIndexForData - bufferIndexStart; 
		LTE_PHY_INT_SET_U16BIT( (&dataBufferForPhy[mesgLenIndexForData]),apiLength);
		mesgLenIndexForData+=SIZEOF_UINT16;	
		dataMesgBytesSent = sendDataToPhysical(dataBufferForPhy, 
				bufferIndexForData,
				internalCellIndex);
		if (!dataMesgBytesSent) 
		{
			LTE_MAC_UT_LOG(LOG_INFO, MAC_MOD_TRANSMITTER, "[%s]Unable " \
					"to send Data Msg to PHY in the function " \
					"createAndSendCmnChannelData \n",__func__);
		}
		dciCCEContainerInfo_p->mibData_p = PNULL;
	}

	while (localCount)
	{
		bufferIndexForData = 0;
		bufferIndexStart = bufferIndexForData;

		tempDCICCEInfoCmnChannel_p = \
					     &(dciCCEContainerInfo_p->dciCCEInfoCmnChannel[localCount - 1]);
		cceNumber = tempDCICCEInfoCmnChannel_p->cceIndex;
		tempDciCCEInfo_p = &(dciCCEContainerInfo_p->dciCCEInfo[cceNumber]);
		tempDciPduInfo_p = &tempDciCCEInfo_p->dciPduInfo;
		/*
		   The memory corresponding to this buffer has been already allocated 
		   the corresponding module and we just access the same memory location
		 */
		dataBufferForPhy = msgSegNext(tempDciCCEInfo_p->dciPduInfo.dataTB_p,0,&segment);
		dataBufferForPhy = segment.base;
		if (PNULL == dataBufferForPhy)
		{
			LTE_MAC_UT_LOG(LOG_INFO, MAC_MOD_TRANSMITTER, \
					"[%s]Unable to access memory of Common Channel Data\n",__func__);
			localCount--;
			mac_reset_cce(tempDciPduInfo_p->aggregationLvl, tempDciCCEInfo_p, internalCellIndex);
			continue;
		}
		/*
		   We now update the buffer with the API header segments in the 
		   following lines.
		 */ 
		transactionId = getTransactionId();
		LTE_PHY_INT_SET_U16BIT( (&dataBufferForPhy[bufferIndexForData]),transactionId);
		bufferIndexForData +=SIZEOF_UINT16;	
		LTE_PHY_INT_SET_U16BIT( (&dataBufferForPhy[bufferIndexForData]),MAC_MODULE_ID);
		bufferIndexForData +=SIZEOF_UINT16;	
		LTE_PHY_INT_SET_U16BIT( (&dataBufferForPhy[bufferIndexForData]),PHY_MODULE_ID);
		bufferIndexForData +=SIZEOF_UINT16;	
		/* 
		   In the next two lines we fill the API Identifier for Data message
		 */
		LTE_PHY_INT_SET_U16BIT( (&dataBufferForPhy[bufferIndexForData]),MAC_DL_DATA_MSG_REQ);
		bufferIndexForData +=SIZEOF_UINT16;	
		mesgLenIndexForData = bufferIndexForData++;
		/*
		   This value has to be increase by one for the next byte. 
		 */
		bufferIndexForData++;

		LTE_PHY_INT_SET_U16BIT( (&dataBufferForPhy[bufferIndexForData]),sysFrameNum);
		bufferIndexForData +=SIZEOF_UINT16;	
		dataBufferForPhy[bufferIndexForData++] = subFrameNum; 
		dataBufferForPhy[bufferIndexForData++] = tempDciCCEInfo_p->msgIdentifier;

		createDLDataMsgReqForCmnChannel(
				dataBufferForPhy, 
				tempDciCCEInfo_p,
				tempDCICCEInfoCmnChannel_p, 
                /* +- SPR 17777 */
				&bufferIndexForData);

		apiLength = bufferIndexForData - bufferIndexStart; 

		LTE_PHY_INT_SET_U16BIT( (&dataBufferForPhy[mesgLenIndexForData]),apiLength);
		mesgLenIndexForData ++;	
		/*
		   Now the entire data pdu buffer has been created so we can sent
		   the same to the PHY by calling the function sendDataToPhysical
		 */
		dataMesgBytesSent = sendDataToPhysical(dataBufferForPhy, 
				bufferIndexForData,
				internalCellIndex);
		if (!dataMesgBytesSent) 
		{
			LTE_MAC_UT_LOG(LOG_INFO, MAC_MOD_TRANSMITTER, "[%s]Unable " \
					"to send Data Msg to PHY in the function " \
					"createAndSendCmnChannelData \n",__func__);
		}
		/* _HARQ_PROC_NUM_ */
		if((!((BCCH_MSG==tempDciCCEInfo_p->msgType)
						|| (PCCH_MSG==tempDciCCEInfo_p->msgType)
						|| (RAR_MSG==tempDciCCEInfo_p->msgType))) 
				&& (INVALID_HARQ_ID != tempDciPduInfo_p->harqProcessId))
		{
#ifdef TDD_CONFIG
			tddDlHarqTimerStart( tempDciPduInfo_p->ueIndex,
					tempDciPduInfo_p->harqProcessId,
                    subFrameNum,
                    (currentGlobalTick + phyDelay), /*dlHarqRttExpiryTick*/
                    sysFrameNum,
                    /* + SPS_TDD_Changes */
                    DRA_SCHEDULING,
                    /* - SPS_TDD_Changes */	
                    internalCellIndex
					);
#endif
		}

		/* _HARQ_PROC_NUM_ */ /* need revision for contention resolution message */        

		if ((( tempDCICCEInfoCmnChannel_p->msgType != BCCH_MSG) 
					&& (tempDCICCEInfoCmnChannel_p->msgType != CCCH_MSG))
				||(INVALID_HARQ_ID == tempDciPduInfo_p->harqProcessId))
		{
			msgFree(tempDciCCEInfo_p->dciPduInfo.dataTB_p);
		}
		/*
		   We now free the CCE reserved by this common channel PDU.
		 */ 
		/* localAggregationLvl = tempDciPduInfo_p->aggregationLvl;TDD Warning Fix */

		mac_reset_cce(tempDciPduInfo_p->aggregationLvl, tempDciCCEInfo_p, internalCellIndex);
		localCount--;
	} /* End of while */
	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Exit",__func__);
	return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  txCmnChannelData 
 *                :  phyDelay - The delay of the execution leg in terms of the 
 *                              number of sub-frames w.r.t. PHY and the current
 *                              processing being done at MAC(is intended for PHY),
 *                   currentGlobalTick - The present Global Tick,
 *                   sysFrame - The value of system frame number and
 *                   subFrame - The value of sub frame number
 *                   internalCellIndex - cell id  
 * Outputs        :  None 
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will call createAndSendCmnChannelData
 *                   for communicaion channel data to be sent, this is the
 *                   main invoking function.
 *****************************************************************************/
 MacRetType txCmnChannelData(
		UInt8 phyDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
		UInt32 sysFrame,
		UInt32 subFrame,
		InternalCellIndex internalCellIndex)
{
	UInt8 dciCCEContainerTick = 0;
	DciCCEContainerInfo* dciCCEContainerInfo_p = PNULL;
	dciCCEContainerTick = (currentGlobalTick + phyDelay) % MAX_PDCCH_CONTAINER;
	dciCCEContainerInfo_p = (dciCCEContainerInfo_gp[internalCellIndex] + dciCCEContainerTick);
	UInt16 sysFrameNum = 0;
	UInt8 subFrameNum = 0;
	/*
	   The function getSFAndSFN is called to receive the SFN and SF of the
	   currrent tick hence we would increase it by 1 while inserting the 
	   value of these entities in the control buffer as it should represent
	   that when it is sent to the PHY interface.
	 */
	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Entry",__func__);

	sysFrameNum = (UInt16)sysFrame;
	subFrameNum = (UInt8)subFrame; 
	subFrameNum = subFrameNum + phyDelay;
	if(subFrameNum >= MAX_SUB_FRAME)
	{
		subFrameNum -= MAX_SUB_FRAME;
		sysFrameNum++;
		if (sysFrameNum == MAX_SFN_VALUE)
		{
			sysFrameNum = 0;
		}    
	}

	if(MAC_SUCCESS != createAndSendCmnChannelData(
				sysFrameNum,
				subFrameNum,
                /* +- SPR 17777 */
#ifdef TDD_CONFIG
				currentGlobalTick,
				phyDelay,
#endif
                /* +- SPR 17777 */
				dciCCEContainerInfo_p, 
				internalCellIndex))
	{
		LTE_MAC_UT_LOG(LOG_INFO, MAC_MOD_TRANSMITTER, "[%s]Unable to create" \
				"and send Data Msg in the function txCmnChannelData \n",__func__);
		LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Entry",__func__);
		return MAC_FAILURE;         
	}
	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Entry",__func__);
	return MAC_SUCCESS;
}



/*****************************************************************************
 * Function Name  :  createDLDataMsgReqForCmnChannel
 * Inputs         :  dataBufferForPhy - The pointer to the data buffer that has to 
 *                                      be sent to the PHY,
 *                   tempDciCCEInfo_p - A pointer to DciCCEInfo from which the
 *                                      relevant information is retrived to be inserted 
 *                                      in the buffer.
 *                   tempDCICCEInfoCmnChannel_p - This is the pointer to the
 *                                                DCICCEInfoCmnChannel from this 
 *                                                we access the msgType element,
 *                   dciCCEContainerInfo_p - The pointer to the DciCCEContainerInfo
 *                                           which contains the structure containing 
 *                                           the mibData_p and
 *                   index - The pointer to the index of dataBufferForPhy.
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function creates the entire MAC_DL_DATA_MSG_REQ the 
 *                   TLV format pdu that has to be sent to the PHY.
 *****************************************************************************/
 void createDLDataMsgReqForCmnChannel(
		UInt8 *dataBufferForPhy,
		DciCCEInfo* tempDciCCEInfo_p,
		DCICCEInfoCmnChannel* tempDCICCEInfoCmnChannel_p,
        /* +- SPR 17777 */
		UInt32 *index)
{
	UInt32 macPduEntryIndex = 0;
	UInt32 macPduLengthIndex = 0;
	UInt32 localIndex = *index;
	DCIPduInfo* tempDciPduInfo_p = &tempDciCCEInfo_p->dciPduInfo; 
	UInt32 tempIndex = 0;  
	UInt8* localDataBufferForPhy = PNULL;
	localDataBufferForPhy = dataBufferForPhy;

	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Entry",__func__);

	switch(tempDCICCEInfoCmnChannel_p->msgType)
	{
		/*
		   Here we handle the case of common control channel message
		   which is sent as a DLSCH PDU to the phy
		 */
		case CCCH_MSG:
		case BCCH_MSG:
		case RAR_MSG:
		case CONTENTION_RESL_MSG:
			macPduEntryIndex = localIndex;
			LTE_PHY_INT_SET_U16BIT( (&localDataBufferForPhy[localIndex]),MAC_DLSCH_PDU_INFO);
			localIndex +=SIZEOF_UINT16;	
			macPduLengthIndex = localIndex++; 
			/* We need to increase the value of localIndex to store the next Three BYTE */
			localIndex++;   

			/*
			   We donot need to copy the buffer with the data out here
			   as this was already done by the calling module of this 
			   function, we only need to increase the length field 
			   presuming that the data already exists
			 */  
			localIndex += tempDciPduInfo_p->tb1Info.tbSize;
			tempIndex = localIndex - macPduEntryIndex;
			LTE_PHY_INT_SET_U16BIT( (&localDataBufferForPhy[macPduLengthIndex]),tempIndex);
			macPduLengthIndex +=SIZEOF_UINT16;	
			break; 
		case PCCH_MSG:
			macPduEntryIndex = localIndex;
			LTE_PHY_INT_SET_U16BIT( (&localDataBufferForPhy[localIndex]),MAC_PCH_PDU_INFO);
			localIndex +=SIZEOF_UINT16;	
			macPduLengthIndex = localIndex++; 
			/* We need to increase the value of localIndex to store the next Three BYTE */
			localIndex++;   
			/*
			   We donot need to copy the buffer with the data out here
			   as this was already done by the calling module of this 
			   function, we only need to increase the length field 
			   presuming that the data already exists
			 */  
			localIndex += tempDciPduInfo_p->tb1Info.tbSize;
			tempIndex = localIndex - macPduEntryIndex;
			LTE_PHY_INT_SET_U16BIT( (&localDataBufferForPhy[macPduLengthIndex]),tempIndex);
			macPduLengthIndex +=SIZEOF_UINT16;	
			break; 
		default:
			LTE_MAC_UT_LOG(LOG_INFO, MAC_MOD_TRANSMITTER, "[%s]Unable " \
					"to branch to proper case statement in the function " \
					"createDLDataMsgReqForCmnChannel \n",__func__);
	}/* End of Switch case */  
	*index = localIndex;
	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Exit",__func__);
}

/*****************************************************************************
 * Function Name  :  createDLDataMsgReq 
 * Inputs         :  dataBufferForPhy - The pointer to the data buffer that has to
 *                                      be sent to the PHY,
 *                   tempDciCCEInfo_p - A pointer to DciCCEInfo from which the 
 *                                      relevant information is retrived to be 
 *                                      inserted in the buffer,
 *                   index - The pointer to the index of dataBufferForPhy and
 *                   multipleTB - This field is set in case of TWO TB (MIMO mode).
 *                                otherwise remains unset. 
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function creates the entire MAC_DL_DATA_MSG_REQ the 
 *                   TLV format pdu that has to be sent to the PHY.
 *****************************************************************************/


 void createDLDataMsgReq(UInt8 *dataBufferForPhy,
		DciCCEInfo* tempDciCCEInfo_p,
		UInt32 *index,
		UInt8 multipleTB)
{
	UInt32 macPduEntryIndex = 0;
	UInt32 macPduLengthIndex = 0;
	UInt32 localIndex = *index;
	DCIPduInfo* tempDciPduInfo_p = &tempDciCCEInfo_p->dciPduInfo; 
	UInt16 tempIndex = 0;
	UInt8* localDataBufferForPhy = PNULL;

	localDataBufferForPhy = dataBufferForPhy;
	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Entry",__func__);

	/*
	   We now branch as according to the msg_type field and create the
	   data pdu buffer as according. 
	 */ 

	switch(tempDciCCEInfo_p->msgType)
	{
		case TA_SPATIAL_MUL:
		case DRX_SPATIAL_MUL:
		case TA_DRX_SPATIAL_MUL:
		case TA_NON_SPATIAL_MUL:
		case DRX_NON_SPATIAL_MUL:
		case TA_DRX_NON_SPATIAL_MUL:
		case SPATIAL_MUL:
		case NON_SPATIAL_MUL:
		case TA_WITH_DATA_SPATIAL_MUL:
		case DRX_WITH_DATA_SPATIAL_MUL:
		case TA_DRX_WITH_DATA_SPATIAL_MUL:
		case TA_WITH_DATA_NON_SPATIAL_MUL:
		case DRX_WITH_DATA_NON_SPATIAL_MUL:
		case TA_DRX_WITH_DATA_NON_SPATIAL_MUL:

			/*
			   The following section of code will hit in those cases  
			   when only one TB is being sent.
			 */ 
			if (multipleTB != 1)
			{
				macPduEntryIndex = localIndex;
				LTE_PHY_INT_SET_U16BIT( (&localDataBufferForPhy[localIndex]),MAC_DLSCH_PDU_INFO);
				localIndex +=SIZEOF_UINT16;	
				macPduLengthIndex = localIndex++;
				/* We need to increase the value of localIndex to store the next three BYTE */
				localIndex++;   
				/*
				   We donot need to copy the buffer with the data out here
				   as this was already done by the calling module of this 
				   function, we only need to increase the length field 
				   presuming that the data already exists
				 */  
				localIndex += tempDciPduInfo_p->tb1Info.tbSize;
				tempIndex = localIndex - macPduEntryIndex;
				LTE_PHY_INT_SET_U16BIT( (&localDataBufferForPhy[macPduLengthIndex]),tempIndex);
				macPduLengthIndex+= SIZEOF_UINT16;	
			}
			/*
			   The following section of code will hit in those cases  
			   when both the TBs are to be sent in MIMO case.
			 */ 
			else if (multipleTB == 1)
			{
				/* Here we handle the MIMO case When Two TB's Has to Be sent */
				macPduEntryIndex = localIndex;
				LTE_PHY_INT_SET_U16BIT( (&localDataBufferForPhy[localIndex]),MAC_DLSCH_PDU_INFO);
				localIndex +=SIZEOF_UINT16;	
				macPduLengthIndex = localIndex++; 
				/* We need to increase the value of localIndex to store the next Three BYTE */
				localIndex++;   
				/*
				   We donot need to copy the buffer with the data out here
				   as this was already done by the calling module of this 
				   function, we only need to increase the length field 
				   presuming that the data for both the TBs already exists
				   one after another in the buffer (taken care by the calling 
				   module)
				 */  
				localIndex += tempDciPduInfo_p->tb1Info.tbSize;
				localIndex += tempDciPduInfo_p->tb2Info.tbSize;
				tempIndex = localIndex - macPduEntryIndex;
				LTE_PHY_INT_SET_U16BIT( (&localDataBufferForPhy[macPduLengthIndex]),tempIndex);
				macPduLengthIndex+=SIZEOF_UINT16;
			}
			break;
		default:
			LTE_MAC_UT_LOG(LOG_INFO, MAC_MOD_TRANSMITTER, "[%s]Unable " \
					"to branch to proper case statement in the function " \
					"createDLDataMsgReqForCmnChannel \n",__func__); 
	}
	(*index) = localIndex;
	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Exit",__func__);
}



/*****************************************************************************
 * Function Name  :  createDLDataMsgReqForMIB 
 * Inputs         :  dataBufferForPhy - The pointer to the data buffer that has to
 *                                      be sent to the PHY,
 *                   dciCCEContainerInfo_p - The pointer to the DciCCEContainerInfo
 *                                           which contains the structure containing
 *                                           the mibData_p and
 *                   index - The pointer to the index of dataBufferForPhy.
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function creates the entire MAC_DL_DATA_MSG_REQ in the 
 *                   TLV format pdu for MIB that has to be sent to the PHY.
 *****************************************************************************/

 void createDLDataMsgReqForMIB(
		UInt8 *dataBufferForPhy,
		DciCCEContainerInfo* dciCCEContainerInfo_p,
		UInt32 *index)
{ 

	UInt32 macPduEntryIndex = *index;
	UInt8* localDataBufferForPhy = PNULL;
	/*Rel_523_Coverity_10017 Fix start*/
	UInt16 mibDataLen = dciCCEContainerInfo_p->mibDataLen;
	/*Rel_523_Coverity_10017 Fix end*/

	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Entry",__func__);

	/*
	   We now handle the case of MIB type message here
	 */
	dataBufferForPhy = dataBufferForPhy + macPduEntryIndex;
	localDataBufferForPhy = dataBufferForPhy;
	LTE_PHY_INT_SET_U16BIT( dataBufferForPhy,MAC_BCH_PDU_INFO);
	dataBufferForPhy +=SIZEOF_UINT16;	
	/*Rel_523_Coverity_10017 Fix start*/
	LTE_PHY_INT_SET_U16BIT( dataBufferForPhy,(mibDataLen + LEN_BCH_PDU));
	/*Rel_523_Coverity_10017 Fix end*/
	dataBufferForPhy +=SIZEOF_UINT16;	
	/*Rel_523_Coverity_10017 Fix start*/
	LTE_PHY_INT_SET_U16BIT( dataBufferForPhy,mibDataLen);
	/*Rel_523_Coverity_10017 Fix end*/
	dataBufferForPhy +=SIZEOF_UINT16;	

	/*Rel_523_Coverity_10017 Fix start*/
	*index = macPduEntryIndex + (dataBufferForPhy  - localDataBufferForPhy) + mibDataLen;    
	/*Rel_523_Coverity_10017 Fix end*/
	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Exit",__func__);

}
/*****************************************************************************
 * Function Name  :  txMacRegisterReq
 * Inputs         :  internalCellIndex
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function will be used to send the MAC Register Request
 *                   to the PHY.
 *****************************************************************************/

 void txMacRegisterReq(InternalCellIndex internalCellIndex)
{
	UInt16 transactionId = 0;
	UInt8 registerReqBufferForPhy[10] = {0};   
	UInt32 bufferIndexForCntl = 0;
	UInt8* bufferStart = PNULL; 

	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Entry",__func__);
	/*
	   We now update the buffer with the API header segments in the 
	   following lines.
	 */
	bufferStart = &registerReqBufferForPhy[0]; 
	transactionId = getTransactionId(); 
	LTE_PHY_INT_SET_U16BIT( (&registerReqBufferForPhy[bufferIndexForCntl]),transactionId);
	bufferIndexForCntl +=SIZEOF_UINT16;	
	LTE_PHY_INT_SET_U16BIT( (&registerReqBufferForPhy[bufferIndexForCntl]),MAC_MODULE_ID);
	bufferIndexForCntl +=SIZEOF_UINT16;	
	LTE_PHY_INT_SET_U16BIT( (&registerReqBufferForPhy[bufferIndexForCntl]),PHY_MODULE_ID);
	bufferIndexForCntl +=SIZEOF_UINT16;	
	LTE_PHY_INT_SET_U16BIT( (&registerReqBufferForPhy[bufferIndexForCntl]),MAC_REGISTER_REQ);
	bufferIndexForCntl +=SIZEOF_UINT16;	
	LTE_PHY_INT_SET_U16BIT( (&registerReqBufferForPhy[bufferIndexForCntl]),10);
	bufferIndexForCntl +=SIZEOF_UINT16;	
	(void)sendDataToPhysical(bufferStart, 10, internalCellIndex);
	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Exit",__func__);
}

//#endif

/******************************************************************************/



#include "lteFapiMacPhyInterface.h"

/***************************************************************/
/* This section contains default values of various parameters  */
/***************************************************************/
#define DEFAULT_POWER_FOR_PCFICH    1
#define DEFAULT_TRANSMISSION_SCHEME 0
#define DEFAULT_TX_POWER            6000
/* SPR 1486 changes start */
#define DEFAULT_PRACH_FLAG          0
/* SPR 1486 changes end */
#define DEFAULT_PREAMBLE_INDEX      0
#define DEFAULT_PRACH_MASK_INDEX    1
#define DEFAULT_DELTA_POWER_OFFSET_INDEX    0
#define DEFAULT_POWER_FOR_PCFICH    1
#define PAGING_RNTI                 0xFFFE
#define BCH_PDU_OFFSET              (DATA_MSG_API_HEADER_LEN + 2)
#define DLSCH_PDU_OFFSET            (DATA_MSG_API_HEADER_LEN)
#define DCI_PDU_OFFSET              (MAC_API_HEADER_LEN)
#define PCH_PDU_OFFSET              (DATA_MSG_API_HEADER_LEN)
/***************************************************************/

/*****************************************************************************
 * Function Name  :  init_ul_msgbuff
 * Inputs         :  numOfCells
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function is used to initialize the memory pool for uplink.
 *****************************************************************************/
void init_ul_msgbuff(UInt8 numOfCells)
{
	UInt8 loopCount = 0;
	UInt8 cellCount = 0;


	for (cellCount = 0; cellCount < numOfCells; cellCount++)
	{
		createMemPool(MAX_ULCONF_BUFF_SIZE, MAX_SUB_FRAME);
		for (loopCount = 0; loopCount < MAX_SUB_FRAME; loopCount++)
		{
			GET_MEM_FROM_POOL (UInt8, pULConfigMsg_gp[cellCount][loopCount],
					MAX_ULCONF_BUFF_SIZE, PNULL);
			memSet (pULConfigMsg_gp[cellCount][loopCount], 0, MAX_ULCONF_BUFF_SIZE);
		}
	}
}

/*OPT_SPR_4119_4120_4122_4123_CHANGES_START*/

/*****************************************************************************
 * Function Name  :  init_dl_msgbuff
 * Inputs         :  numOfCells
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function is used to initialize the memory pool for downlink.
 *****************************************************************************/
void init_dl_msgbuff(UInt8 numOfCells)
{
	InternalCellIndex cellIndex = 0;
	/* + SPR 13990 Changes */
	CellConfigParams *cellParams_p = PNULL;
	/* - SPR 13990 Changes */
#ifndef MEM_CHECK_DOUBLE_FREE
	/* Start 128 UE: Memory Changes */
	/* For HI_DCI message */
	createMemPool(CONTROL_BUFFER_SIZE, NUM_BLK_FOR_DL_MSG_BUF);
	/* For DL_CONFIG message */
	createMemPool(CONTROL_BUFFER_SIZE, NUM_BLK_FOR_DL_MSG_BUF);
	/* For TX Req message */
	createMemPool(DATA_BUFFER_SIZE, NUM_BLK_FOR_DL_MSG_BUF);
	/* End 128 UE: Memory Changes */

	/** SPR 14505 Changes **/
	/* For TX Req message in Case of 4*4 */
	createMemPool(2* DATA_BUFFER_SIZE, NUM_BLK_FOR_DL_MSG_BUF);
	/** SPR 14505 Changes **/

#endif    
	for (cellIndex = 0;cellIndex < numOfCells; cellIndex++)
	{    
		cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]->cellParams_p;
		/* - SPR 13990 Changes */
		/* Allocating memory for HI_DCI message */
		GET_MEM_FROM_POOL(UInt8, pHIDCICfgMsgBuff_gp[cellIndex], CONTROL_BUFFER_SIZE, PNULL);
		/* Coverity 55856 */
		if ( NULL !=  pHIDCICfgMsgBuff_gp[cellIndex] ) {
			memSet(pHIDCICfgMsgBuff_gp[cellIndex],0,CONTROL_BUFFER_SIZE);
		}
		/* Allocating memory for DL_CONFIG message */
		GET_MEM_FROM_POOL(UInt8, pDLCfgMsgBuff_gp[cellIndex], CONTROL_BUFFER_SIZE, PNULL);
		/* Coverity 55855 */
		if ( NULL != pDLCfgMsgBuff_gp[cellIndex] )
		{
			memSet(pDLCfgMsgBuff_gp[cellIndex],0,CONTROL_BUFFER_SIZE);
		}
		/* + SPR 13990 Changes */
		if(cellParams_p)
		{
			if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4 && cellParams_p->dlAvailableRBs == 100)
			{
				/* Allocating memory for TX Req message */
				GET_MEM_FROM_POOL(UInt8, pTxReqMsgBuff_gp[cellIndex], 2 * DATA_BUFFER_SIZE, PNULL);
				/* Coverity 55857*/

				if ( NULL !=  pTxReqMsgBuff_gp[cellIndex] )
				{
					memSet(pTxReqMsgBuff_gp[cellIndex],0, 2 * DATA_BUFFER_SIZE);
				}
			}
			else
			{
				/* Allocating memory for TX Req message */
				GET_MEM_FROM_POOL(UInt8, pTxReqMsgBuff_gp[cellIndex], DATA_BUFFER_SIZE, PNULL);
				/* Coverity 55857*/

				if ( NULL !=  pTxReqMsgBuff_gp[cellIndex] )
				{
					memSet(pTxReqMsgBuff_gp[cellIndex],0,DATA_BUFFER_SIZE);
				}
			}
		}
		/* - SPR 13990 Changes */
	}
}
/*OPT_SPR_4119_4120_4122_4123_CHANGES_END*/

/***************************************************************/
/* This section contains macros to convert values from MAC
 * endianness to PHY endianness before assigning to pkt buffer */
/***************************************************************/

/************************* Section End *************************/
/*****************************************************************************
 * Function Name  :  fillMacApiBufferHeader
 * Inputs         :  pMsgBuffer - buffer to store message,
 *                   srcModId - source module to fill in header,
 *                   apiId - API Id to fill in header and
 *                   length - length to fill in header
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This fucntion fills in the message MAC API header info.
 *****************************************************************************/
/*OPT_SPR_4119_4120_4122_4123_CHANGES_START*/
/* +- SPR 17777 */
STATIC  void fillMacApiBufferHeader(UInt8 *pMsgBuffer,
		UInt16 apiId,
		UInt16 length)
/* +- SPR 17777 */
/*OPT_SPR_4119_4120_4122_4123_CHANGES_END*/
{
	FAPI_l1ApiMsg_st    *pL1Api = (FAPI_l1ApiMsg_st *)pMsgBuffer;
	pL1Api->msgId = apiId;
	pL1Api->lenVendorSpecific = 0;
	pL1Api->msgLen = MAC_PHY_CONVERT_16(length);
}
/* + TM7_EPRE */
/*****************************************************************************
 * Function Name  :  fillVendorSpecificInfo
 * Inputs         :  pMsgBuffer - message buffer,
 *                   tempVendorSpecificForDLConfig_p - pointer to structure 
 *                                          FAPI_VendorSpecificForDLConfig_st,
 *                   vendorSpLen - length to fill in header.
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This fucntion fills in the Vendor specific fields.
 *****************************************************************************/

static  void fillVendorSpecificInfo(UInt8 *pMsgBuffer,
		FAPI_VendorSpecificForDLConfig_st *tempVendorSpecificForDLConfig_p,
		UInt8 *vendorSpLen)
{
	UInt8 index = 0;
	FAPI_VendorSpecificForDLConfig_st *vendorSpecificForDlConfig_p = PNULL;
	FAPI_PdschEpreToUeRsRatio_st  *pdschEpreToUeRsRatioInfo_p = PNULL;

	FAPI_l1ApiMsg_st    *pL1Api = (FAPI_l1ApiMsg_st *)pMsgBuffer;

	vendorSpecificForDlConfig_p = (FAPI_VendorSpecificForDLConfig_st *) (&pL1Api->msgBody[0] + pL1Api->msgLen);
	vendorSpecificForDlConfig_p->numOfUEs = MAC_PHY_CONVERT_16(tempVendorSpecificForDLConfig_p->numOfUEs);

	for(index = 0;index < tempVendorSpecificForDLConfig_p->numOfUEs ; index++)
	{
		pdschEpreToUeRsRatioInfo_p = &vendorSpecificForDlConfig_p->pdschEpreToUeRsRatioInfo[index];
		pdschEpreToUeRsRatioInfo_p->rnti = 
			tempVendorSpecificForDLConfig_p->pdschEpreToUeRsRatioInfo[index].rnti;
		pdschEpreToUeRsRatioInfo_p->pdschEpreToUeRsRatio = 
			tempVendorSpecificForDLConfig_p->pdschEpreToUeRsRatioInfo[index].pdschEpreToUeRsRatio;

		*vendorSpLen = ((offsetof(FAPI_VendorSpecificForDLConfig_st,pdschEpreToUeRsRatioInfo))+ 
				((sizeof(FAPI_PdschEpreToUeRsRatio_st))*(index+1)));

	}
	pL1Api->lenVendorSpecific = MAC_PHY_CONVERT_16(*vendorSpLen);

}

/* - TM7_EPRE */

/*****************************************************************************
 * Function Name  :  getRntiTypeForRnti
 * Inputs         :  rnti - RNTI whose type is to required.
 *                :  internalCellIndex - cell id  
 * Outputs        :  None
 * Returns        :  rntiType.
 * Description    :  This fucntion returns the rnti type.
 *****************************************************************************/
/* + SPS_TDD */
 UInt8 getRntiTypeForRnti(UInt16 rnti, InternalCellIndex internalCellIndex)
/* - SPS_TDD */
{
	UInt8   rntiType = FAPI_OTHER_CRNTI;
	/* 0001-003C : RA-RNTI, C-RNTI, Semi-Persistent Scheduling C-RNTI,
	   Temporary C-RNTI, TPC-PUCCH-RNTI and TPC-PUSCH-RNTI  */
	/* SPR Start 911 */ 
	if ((rnti >=cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->startRARNTI) 
			&& (rnti <= cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->endRARNTI))
	{
		rntiType = FAPI_RA_RNTI_P_RNTI_SI_RNTI;
	}
	/* 003D-FFF3 : C-RNTI, Semi-Persistent Scheduling C-RNTI,
	   Temporary C-RNTI, TPC-PUCCH-RNTI and TPC-PUSCH-RNTI */
	else if ((rnti > cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->endRARNTI 
				&& rnti <= 0xFFF3) || (
					rnti > 0 && rnti < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->startRARNTI) )
	{
		/*!
		 * \code
		 * Function Modification
		 * static  UInt8 getRntiTypeForRnti(UInt16 rnti)
		 * {
		 *     ...
		 *     if(rnti >=  cellConfig_g.startSpsCRnti && rnti <= cellConfig_g.endSpsCRnti)
		 *     {
		 *         rntiType = FAPI_SPS_CRNTI;
		 *     }
		 *     else
		 *     {
		 *         rntiType = FAPI_C_RNTI;
		 *     }
		 * }
		 * \endcode
		 */
		/* SPS_CHG_START */
		if( rnti >=  cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->spsCommonCellInfo.startSpsCRnti && 
				rnti <= cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->spsCommonCellInfo.endSpsCRnti)
		{
			rntiType = FAPI_SPS_CRNTI;
		}
		else
			/* SPS_CHG_END */
		{
			rntiType = FAPI_C_RNTI;
		}
	}
	/* FFF4-FFFC : Reserved for future use */
	/* FFFD : M-RNTI */
	else if ((rnti >= 0xFFF4) && (rnti <= 0xFFFD))
	{
		rntiType = FAPI_OTHER_CRNTI;
	}
	/* FFFE : P-RNTI */
	/* FFFF : SI-RNTI */
	else if ((rnti == 0xFFFE) || (rnti == 0xFFFF))
	{
		rntiType = FAPI_RA_RNTI_P_RNTI_SI_RNTI;
	}
	/* SPR 911 end */ 
	return rntiType;
}

/*****************************************************************************
 * Function Name  :  setTxIndFromHarq
 * Inputs         :  pDciPdu - Pointer to PDU using which TxInd is set 
 *                   msgType - Type of message 
 *                :  internalCellIndex - cell id  
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This fucntion sets TxInd.
 *****************************************************************************/
/* SPR 6967 Fix Start */
STATIC  void setTxIndFromHarq(DCIPduInfo *pDciPdu,InternalCellIndex internalCellIndex
                                /* + SPR 17733 */
#ifdef FDD_CONFIG
                                 ,UInt8  msgType
#endif
                                /* - SPR 17733 */              )
	/* SPR 6967 Fix End */
{
	UInt32 ueIndex = 0;
	UInt8  tempToggleVal=0;
	DLUEContextInfo *ueContextInfo_p = PNULL;
	DLHARQProcess *harqProcess_p = PNULL;
	UInt8 rntiType = 0;
#ifdef FDD_CONFIG
    UInt8 isMsgType = 0;
#endif


	if(INVALID_HARQ_ID == pDciPdu->harqProcessId)
	{
		return;
	}
	ueIndex = pDciPdu->ueIndex;
	if(MAX_UE_SUPPORTED > ueIndex)
	{
		/* SPR 6967 Fix Start */
		/* +- SPR 16962 */	
		/* + SPR 17733 */
#ifdef FDD_CONFIG
            isMsgType =  (CONTENTION_RESL_MSG == msgType );
        if(isMsgType)
		{
			TempCRNTICtx *tempUECtx_p = 
				tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].tempCRNTICtx_p;
			if( PNULL == tempUECtx_p )
			{
				LOG_MSG(MAC_ULSCH_FREQ_SEL_SCH_ID,LOGWARNING,MAC_PUSCH,
						getCurrentTick(), ueIndex, 0, 0,0,0, 0,0,
						__func__,"TempCtxNumm");
				return;
			}
			harqProcess_p = &tempUECtx_p->dlHarqCtx_p->harqProcess[
				pDciPdu->harqProcessId];
		}
		else  
		{
#endif
        /* - SPR 17733 */

			ueContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
            /* SPR 20567 fix start */
            if((PNULL!=ueContextInfo_p)&&(!ueContextInfo_p->pendingDeleteFlag))
            {
                /* + SPR 11469 Changes */
                if(IS_PCELL_OF_UE(ueContextInfo_p->dlUEContext_p,internalCellIndex))
                {
                    harqProcess_p = &ueContextInfo_p->dlUEContext_p->dlHarqContext_p->
                        harqProcess[pDciPdu->harqProcessId];
                }
                else
                {
                    harqProcess_p = &ueContextInfo_p->dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->\
                                    ScelldlHarqContext_p->harqProcess[pDciPdu->harqProcessId];
                }
            }
            /* SPR 20567 fix end */
			/* SPR 6967 Fix End */
 /* + SPR 17733 */
#ifdef FDD_CONFIG		
        }
#endif
/* - SPR 17733 */
        /* Coverity 113187 Fix Start */
        if( (PNULL != harqProcess_p) && (pDciPdu->harqProcessId==harqProcess_p->harqProcessId))
            /* Coverity 113187 Fix End */
		{
			if(TRUE==harqProcess_p->isTBOneValid)
			{
				/* SPS_CHG */
				rntiType =  getRntiTypeForRnti(pDciPdu->rnti, internalCellIndex);
				if( FAPI_SPS_CRNTI == rntiType)
					/* SPS_CHG */
				{
					if(RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator) 
						pDciPdu->tb1Info.txIndicator =\
									      harqProcess_p->dlHARQTBOneInfo.newDataIndicator = 1;

				}
				/* SPS_CHG */
				else if((harqProcess_p->dlHARQTBOneInfo.txIndicator 
							== pDciPdu->tb1Info.txIndicator)
						&& ((NEW_TX
								== harqProcess_p->dlHARQTBOneInfo.newDataIndicator)
							|| (RE_TX
								== harqProcess_p->dlHARQTBOneInfo.newDataIndicator)))
				{
					if(NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator)
					{
						tempToggleVal = harqProcess_p->dlHARQTBOneInfo.
							newDataIndicator;
						harqProcess_p->dlHARQTBOneInfo.newDataIndicator
							= tempToggleVal ^ NEW_TX;

						/* SPR 1388 changes start */ 
						if(pDciPdu->tb1Info.redundancyVer > 0)
						{
							lteWarning("Redundancy Version for TB1 NEW TX is not zero = %d\n",
									pDciPdu->tb1Info.redundancyVer);    
						}     
						/* SPR 1388 changes end */ 
					}
					/* + SPR 17733 */
#ifdef FDD_CONFIG
					else if (CONTENTION_RESL_MSG == msgType)
					{
						harqProcess_p->dlHARQTBOneInfo.current_TX_NB++; 
						UInt32 inputRedundanVerCal  = 0;
						inputRedundanVerCal = MODULO_FOUR(
								harqProcess_p->dlHARQTBOneInfo.current_TX_NB);
						harqProcess_p->dlHARQTBOneInfo.irVersion =
							redundancyVerCalc(inputRedundanVerCal);
						pDciPdu->tb1Info.redundancyVer = 
							harqProcess_p->dlHARQTBOneInfo.irVersion; 
					}
#endif
					/* - SPR 17733 */
					/* no need to toggle for retransmission */
					pDciPdu->tb1Info.txIndicator
						= harqProcess_p->dlHARQTBOneInfo.newDataIndicator;

					/* + TM7_8 Changes Start */
					/* txIndicator field of the disabled transport block TB2 shall be set as 0,
					 * to indicate the antenna port to be used*/

					if((DCI_FORMAT_2B == pDciPdu->dciFormat) && (FALSE == harqProcess_p->isTBTwoValid))
					{
						pDciPdu->tb2Info.txIndicator = 0;

					}
					/* - TM7_8 Changes End */

				}
				else
				{
					if(harqProcess_p->dlHARQTBOneInfo.txIndicator
							!= pDciPdu->tb1Info.txIndicator)
					{    
						/*LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"TB ONE TX IND MISMATCH\tHARQ\t%d\tPDU\t%d\n",
						  harqProcess_p->dlHARQTBOneInfo.txIndicator,
						  tempDciPduInfo_p->tb1Info.txIndicator); */
					}
					else
					{
						/*LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"IMPROPER TB ONE NEWDATAINDICATOR VAL\t%d\n",
						  harqProcess_p->dlHARQTBOneInfo.
						  newDataIndicator);*/
					}
				}
			}
			if(TRUE == harqProcess_p->isTBTwoValid)
			{
				if((harqProcess_p->dlHARQTBTwoInfo.txIndicator
							== pDciPdu->tb2Info.txIndicator)
						&&((NEW_TX
								==harqProcess_p->dlHARQTBTwoInfo.newDataIndicator)
							||(RE_TX == harqProcess_p->dlHARQTBTwoInfo.
								newDataIndicator)))
				{
					if(NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)
					{
						tempToggleVal
							= harqProcess_p->dlHARQTBTwoInfo.newDataIndicator;
						harqProcess_p->dlHARQTBTwoInfo.newDataIndicator
							= tempToggleVal ^ NEW_TX;
						/* SPR 1388 changes start */ 
						if(pDciPdu->tb2Info.redundancyVer > 0)
						{
							lteWarning("Redundancy Version for TB2 NEW TX is not zero = %d\n",
									pDciPdu->tb2Info.redundancyVer);    
						}     
						/* SPR 1388 changes end */ 

					}
					/* no need to toggle for retransmission */
					pDciPdu->tb2Info.txIndicator
						= harqProcess_p->dlHARQTBTwoInfo.newDataIndicator;

					/* + TM7_8 Changes Start */
					/* txIndicator field of the disabled transport block TB1 shall be set as 1,
					 * to indicate the antenna port to be used*/

					if((DCI_FORMAT_2B == pDciPdu->dciFormat) && (FALSE == harqProcess_p->isTBOneValid))
					{
						pDciPdu->tb1Info.txIndicator = 1;

					}
					/* - TM7_8 Changes End */

				}
				else
				{
					if(harqProcess_p->dlHARQTBTwoInfo.txIndicator
							!= pDciPdu->tb2Info.txIndicator)
					{
						/*LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"TB TWO TX IND MISMATCH\tHARQ\t%d\tPDU\t%d\n",
						  harqProcess_p->dlHARQTBTwoInfo.txIndicator,
						  tempDciPduInfo_p->tb2Info.txIndicator);*/
					}
					else
					{
						/*LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"IMPROPER TB TWO NEWDATAINDICATOR VAL\t%d\n",
						  harqProcess_p->dlHARQTBTwoInfo.
						  newDataIndicator);*/
					}
				}
			}
		}
		else
        {
            /* Coverity 113187 Fix Start */
            LOG_MAC_ERROR(MAC_DL_HARQ,"Null value received for harqProcess_p\n"); 
            /* Coverity 113187 Fix End */
			/*LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"HARQ PROCESS ID MISMATCH\tHARQ\t%d\tPDU\t%d\n",
			  harqProcess_p->harqProcessId,tempDciPduInfo_p->harqProcessId);*/
		}
	}
	else
	{
		/*LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"INCORRECT UE INDEX\t%d\n",ueIndex); */
	}
}
/* Rel 5.3: Coverity 54146 Fix Start */
/*SPR 19433 +*/
#if defined(MAC_AUT_TEST) 
/*SPR 19433 -*/
/*****************************************************************************
 * Function Name  :  isPMIInCodebookSubsetRestriction
 * Inputs         :  pmiUE, RI - Rank Indicator 
 *                   cbsrValue - Codebook Subset Restriction Value
 * Outputs        :  None
 * Returns        :  result - PMI is present in CBSR or not
 * Description    :  This fucntion checks whether the PMI is in CBSR or not.
 *****************************************************************************/
STATIC UInt8 isPMIInCodebookSubsetRestriction(UInt8 pmiUE, UInt8 RI, UInt8 *cbsrValue)
{
	UInt8 result = 0;
	if (2 == RI)
	{/*Read bit 7 or 6 in cbsr*/
		result = ((0x01 << (6+pmiUE))&cbsrValue[0]);
	}
	else
	{/*Read bit 5, 4, 3 or 2 in cbsrValue*/
		result = ((0x01 << (2+pmiUE))&cbsrValue[0]);
	}
	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"isPMIInCodebookSubsetRestriction returned: %d\n",result);

	return result;
}
#endif //MAC_AUT_TEST
/* Rel 5.3: Coverity 54146 Fix End */
/* +  SPR 1476*/

/*****************************************************************************
 * Function Name  :  fillCodebookInfoInDlschPdu
 * Inputs         :  pDlConfigPdu - Pointer to FAPI_dlConfigPDUInfo_st 
 *                   pDciPdu - pointer to DCIPduInfo
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This fucntion fills the Codebook info in DLSCHPDU.
 *****************************************************************************/

STATIC  void fillCodebookInfoInDlschPdu (
		FAPI_dlConfigPDUInfo_st *pDlConfigPdu, 
		DCIPduInfo *pDciPdu,
		/*SPR 13698 changes start */
		InternalCellIndex internalCellIndex) 
/*SPR 13698 changes end*/
{
	/* 4x4 DL MIMO CHG START */
	UInt8 subbandCount = pDciPdu->numberOfSubbands;
	/*SPR 13698 changes start */
#ifndef FLEXRAN
	FAPI_beamFormingVectorInfo_st *bfVector;
#endif
	UInt8 *subBandInfo_p;
/* SPR 19288 change start */
#if !defined(FLEXRAN)
/*SPR 21001 Changes start */
	if(pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector)
	{
		bfVector=&pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.bfVector[0];
		UInt16 size = 0;
		UInt8 count;
		UInt8 counter;
		for(count =0; count < pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector; count++)
		{
			size = sizeof(FAPI_beamFormingVectorInfo_st);
			for(counter = 0; counter < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas; counter++)
			{
				size += sizeof(UInt16);
			}

			size -= sizeof(UInt16);
			bfVector = (FAPI_beamFormingVectorInfo_st *)((UInt8 *)bfVector + size);
		}
		subBandInfo_p=(UInt8 *)bfVector;
	}
	else
	{
		subBandInfo_p=((UInt8 *)(&pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.bfVector[0])+sizeof(FAPI_beamFormingVectorInfo_st) * 1);
	}    
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
	/*SPR 13698 changes End */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfSubBand = pDciPdu->numberOfSubbands;
/* SPR 20184 change start */
/* SPR 19288 change start */
/*SPR 21001 Changes start */
	if (subbandCount)
	{
		/*cov fix ca-tdd start*/ 
		subbandCount--;
        /* SPR 18122 Changes Start*/
		while(subbandCount < (DIVIDE_BY_TWO(MAX_SUBBANDS)))
            /* SPR 18122 Changes End*/
		{    
			/*SPR 13698 changes start */
			*subBandInfo_p = pDciPdu->subbandtoPMIInfo[subbandCount];
			subBandInfo_p = subBandInfo_p + sizeof(UInt8);

			/*SPR 13698 changes end */
			subbandCount--;
			/*cov fix ca-tdd end*/      
		}
	}
/* SPR 20184 change end */
/*SPR 21001 Changes end */
/* SPR 19288 change end */
	/* 4x4 DL MIMO CHG END */
	return;
}
/* -  SPR 1476*/


/* + TM6_5.2 */

/*****************************************************************************
 * Function Name  :  fillCodebookInfoInDlschPduForTM6
 * Inputs         :  pDlConfigPdu - pointer to FAPI_dlConfigPDUInfo_st 
 *                   pDciPdu - Pointer to DCIPduInfo
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This fucntion fills the Codebook info in DLSCHPDU for TMode 6.
 *****************************************************************************/
STATIC  void fillCodebookInfoInDlschPduForTM6(
		FAPI_dlConfigPDUInfo_st *pDlConfigPdu, 
		DCIPduInfo *pDciPdu,
		/*SPR 13698 changes start */
		InternalCellIndex internalCellIndex)
/*SPR 13698 changes end */
{
    /* CID 113394  Coverity fix start */
    if (!pDciPdu)
    {
        return;
    }

    /* CID 113394 Fix Start */
	/*SPR 13698 changes start */
    /* CID 111500 Fix Start */
	UInt8 *subBandInfo_p = PNULL;
    /* CID 111500 Fix End */
/* SPR 19288 change start */
#ifndef FLEXRAN
    FAPI_beamFormingVectorInfo_st *bfVector;
/*SPR 21001 Changes start */
	if(pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector)
	{
		bfVector=&pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.bfVector[0];
		UInt16 size = 0;
		UInt8 count;
		UInt8 counter;
		for(count =0; count < pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector; count++)
		{
			size = sizeof(FAPI_beamFormingVectorInfo_st);
			for(counter = 0; counter < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas; counter++)
			{
				size += sizeof(UInt16);
			}

			size -= sizeof(UInt16);
			bfVector = (FAPI_beamFormingVectorInfo_st *)((UInt8 *)bfVector + size);
		}
		subBandInfo_p=(UInt8 *)bfVector;

	}
	else
	{
		subBandInfo_p=((UInt8 *)(&pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.bfVector[0])+sizeof(FAPI_beamFormingVectorInfo_st) * 1);
	}    
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
	/*SPR 13698 changes End */
	if(1 == pDciPdu->pmiConfirmation)
	{   
		UInt8 subbandCount = pDciPdu->numberOfSubbands;
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfSubBand = pDciPdu->numberOfSubbands;
		/*cov fix ca-tdd start*/ 
		subbandCount--;
        /* SPR 18122 Changes Start*/
		while(subbandCount < (DIVIDE_BY_TWO(MAX_SUBBANDS)))
            /* SPR 18122 Changes End*/
		{ 
			/*SPR 13698 changes start */
			*subBandInfo_p = pDciPdu->subbandtoPMIInfo[subbandCount];
			subBandInfo_p = subBandInfo_p + sizeof(UInt8);
			/*SPR 13698 changes end */
			/*cov fix ca-tdd end*/
			subbandCount--;
		}
	}
	else
    {
        /*SPR 13698 changes start */
        if(PNULL != subBandInfo_p)
        {
            *subBandInfo_p = pDciPdu->transmittedPMI;
        }
        /*SPR 13698 changes End */
    }
    /* CID 113394  Coverity fix end */
	return;

}
/* - TM6_5.2 */

/* + coverity 42177 */
#ifdef MAC_AUT_TEST

/*****************************************************************************
 * Function Name  :  fillpreCodingInfoForDCIFormat2Pdu
 * Inputs         :  pDciMsg - pointer to structure FAPI_dciFormat2_st and
 *                   pDciPdu - pointer to DCIPduInfo
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This fucntion fills Pre coding info for DCI Format2 PDU.
 *****************************************************************************/
STATIC  void fillpreCodingInfoForDCIFormat2Pdu (\
		FAPI_dciFormat2_st *pDciMsg, \
		DCIPduInfo *pDciPdu)
{
	DLUEContext *ueDLContext_p     = dlUECtxInfoArr_g[pDciPdu->ueIndex].dlUEContext_p;
	/* SPR 10346 changes start */
	UInt8 RI = ueDLContext_p->dlMIMOInfo.dlRi;
	/* SPR 10346 changes end */
	UInt8 pmiUE = ueDLContext_p->dlCQIInfo.wideBandPMIIndex;
	UInt8 cw = 0;
	/* +  SPR 1476*/
	UInt8 DCI2 = 1;
	/* -  SPR 1476*/

	if ( pDciPdu->isTBOneValid )        
	{
		cw++;
	}
	if ( pDciPdu->isTBTwoValid )        
	{
		cw++;
	}

	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER, "RI : %d, pmiUE : %d, cw : %d\n",RI, pmiUE, cw);

	if ((CQI_MODE_1_2 == ueDLContext_p->dlCQIInfo.cqiMode) || !(isPMIInCodebookSubsetRestriction (pmiUE,RI, ueDLContext_p->dlMIMOInfo.cbsrInfo.cbsrValue)))
	{/*For CQI Mode 1_2 or if reported PMI is not according to CBSR, eNodeB MAC sends the value as configured by RRC*/
		pDciMsg->preCodingInfo = ueDLContext_p->dlMIMOInfo.precodingIndex;
	}
	else
	{
		/* +  SPR 1476*/
		if ((RI ==1) &&(cw ==1))
		{
			DCI2 = pmiUE+1;
		}
		else if((RI == 1) && (cw ==2))
		{
			if((0==pmiUE) || (1==pmiUE))
			{
				DCI2 = 0;
			}
			else
			{
				DCI2 = 1;
			}
		}
		else if((RI == 2) && (cw ==2))
		{
			if(pmiUE == 0)
			{
				DCI2 = 0;
			}
			else
			{
				DCI2 = 1;
			}
		}
		else 
		{
			DCI2 = 0;
		}

		pDciMsg->preCodingInfo = DCI2;
		/* -  SPR 1476*/
	}
	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"eNB precoding info : %d\n",pDciMsg->preCodingInfo);
}
#endif
/* - coverity 42177 */

#if defined (FLEXRAN)

STATIC  void fillDciPdu(FAPI_dlConfigPDUInfo_st *pDlConfigPdu,
                FAPI_dciDlPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu,
                InternalCellIndex internalCellIndex, UInt8 type)
{
#ifndef FLEXRAN
    CellConfigParams *cellParams_p = PNULL;
    cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;  
#endif
    pDciMsg->aggregationLevel= 0 ;
    pDciMsg->resAllocationType= 0 ;
    pDciMsg->vRBAssignmentFlag= 0 ;
    pDciMsg->rbCoding= 0 ;
    pDciMsg->mcs_1= 0 ;
    pDciMsg->redundancyVersion_1= 0 ;
    pDciMsg->newDataIndicator_1= 0 ;
    pDciMsg->tbToCodeWordSwapFlag= 0 ;
    pDciMsg->mcs_2= 0 ;
    pDciMsg->redundancyVersion_2= 0 ;
    pDciMsg->newDataIndicator_2= 0 ;
    pDciMsg->harqProcessNum= 0 ;
    pDciMsg->tPMI= 0 ;
    pDciMsg->pmi= 0 ;
    pDciMsg->preCodingInfo= 0 ;
    pDciMsg->tpc= 0 ;
    pDciMsg->dlAssignmentIndex= 0 ;
    pDciMsg->nGAP= 0 ;
    pDciMsg->dlPowerOffset= 0 ;
    pDciMsg->allocatePRACHFlag= 0 ; 
    pDciMsg->preambleIndex= 0 ;
    pDciMsg->pRACHMaskIndex= 0 ;
    pDciMsg->rntiType= 0 ;
    pDciMsg->txPower= 0 ;
#if defined (FAPI_4_0_COMPLIANCE) && !defined (FLEXRAN)
    /*Release 9 parameters*/
    pDciMsg->mcchFlag=DEFAULT_MCCHFLAG;
    pDciMsg->mcchChangeNotification=DEFAULT_MCCH_CHANGE_NOTIF;
    pDciMsg->scramblingIndentity=pDciPdu->scramblingId;
    
    /*Release 10 parameters*/
    pDciMsg->crossCarrierSchedulingFlag=DEFAULT_CROSSCARRIER_SCH_FLAG;
    pDciMsg->carrierIndicator=DEFAULT_CARRIER_INDICATOR;
    pDciMsg->srsFlag=DEFAULT_SRS;
    pDciMsg->srsRequest=DEFAULT_SRS_REQUEST;
    pDciMsg->antennaPortScramblingAndLayers=DEFAULT_ANTENNA_SCRAMBLING_NUM_LAYERS;
	/*Release 11 Parameters*/
	pDciMsg->qcl = 0;
#endif
    /*calculate size of DCI*/
#ifndef FLEXRAN
    pDciMsg->payload_length = getFapiDciSize (cellParams_p->duplexingMode,cellParams_p->dlAvailableRBs, pDciPdu->dciFormat, 0, 0, 0,0,0,0);
#endif
    pDlConfigPdu->pduSize = (sizeof(FAPI_dciDlPduInfo_st)  +  2 /*size of PDU type and PDU size */);
   // fprintf(stderr," DL dci pdu size [%d]\n",  pDlConfigPdu->pduSize );
}
#endif    
/* - CL-MIMO LJA*/  
/*****************************************************************************
 * Function Name  :  fillDciFormat1Pdu
 * Inputs         :  pDciMsg - Pointer to message buffer to fill in the PDU and
 *                   pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message.
 *                :  internalCellIndex - cell id  
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This fucntion fills in the message with DCI Format 1 PDU.
 *****************************************************************************/
#if defined(FLEXRAN)
/*SPR 21001 Changes start */
STATIC  void fillDciFormat1Pdu(
                FAPI_dciDlPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu,
                InternalCellIndex internalCellIndex)

#else
STATIC  void fillDciFormat1Pdu(
		FAPI_dciFormat1_st *pDciMsg, DCIPduInfo *pDciPdu,
		InternalCellIndex internalCellIndex)
/*SPR 21001 Changes end */
#endif

{
	pDciMsg->aggregationLevel = pDciPdu->aggregationLvl;
	pDciMsg->resAllocationType = pDciPdu->resrcAllocType;
	pDciMsg->rbCoding = MAC_PHY_CONVERT_32(pDciPdu->resrcAssgmntBitMap);
	pDciMsg->mcs_1 = pDciPdu->tb1Info.modulationType;
	pDciMsg->redundancyVersion_1 = pDciPdu->tb1Info.redundancyVer;
	pDciMsg->newDataIndicator_1 = pDciPdu->tb1Info.txIndicator;

	/* SPR# 891 Fix start */
	/* SPR 1483 changes start */
#ifdef TDD_CONFIG
	if ( pDciPdu->harqProcessId < getMaxDLHarq(internalCellIndex))
#else    
		if( (MAX_HARQ_PROCESS_NB - 1) >= pDciPdu->harqProcessId )
#endif    
			/* SPR 1483 changes end */
		{
			pDciMsg->harqProcessNum = pDciPdu->harqProcessId;
		}
		else
		{
			pDciMsg->harqProcessNum  = MIN_DCI_HARQ_PROCESS_NUM;
		}
	/* SPR# 891 Fix End */
	pDciMsg->tpc = pDciPdu->tpc;
#ifdef TDD_CONFIG	 
	pDciMsg->dlAssignmentIndex = pDciPdu->dai;
#endif
	pDciMsg->rntiType = getRntiTypeForRnti(pDciPdu->rnti,internalCellIndex);
	/* CLPC_REWORK CHG */
/*SPR 21001 Changes start */
    pDciMsg->txPower = MAC_PHY_CONVERT_16(pDciPdu->txPower);
	/* CLPC_REWORK_CHG END */
}

/*****************************************************************************
 * Function Name  :  fillDciFormat1APdu
 * Inputs         :  pDciMsg - Pointer to message buffer to fill in the PDU,
 *                   pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message 
 *                :  msgType
 *                :  internalCellIndex - cell id  
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function fills in the message with DCI Format 1A data.
 *****************************************************************************/
#if defined(FLEXRAN)
/*SPR 21001 Changes start */
STATIC  void fillDciFormat1APdu(
                FAPI_dciDlPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu,MsgType msgType
                ,InternalCellIndex internalCellIndex)
#else
STATIC  void fillDciFormat1APdu(
		FAPI_dciFormat1A_st *pDciMsg, DCIPduInfo *pDciPdu,MsgType msgType
		,InternalCellIndex internalCellIndex)
/*SPR 21001 Changes end */
#endif
{
	/* SPR_4286_FIX Start */ 
	UInt32 numberOfBitsForRbCoding = 0;
	UInt32 dlRBs = 0;
	/* SPR_4286_FIX End */ 

	pDciMsg->aggregationLevel = pDciPdu->aggregationLvl;
	/*If preambleIdx is valid that means PDCCH Order needs to be sent to UE
	 * using DCI 1A PDU, which is encoded in else part*/

	if (PDCCH_ORDER_MSG != msgType)
	{
		/* SPS_CHG_START */
		UInt8 rntiType =  getRntiTypeForRnti(pDciPdu->rnti,internalCellIndex);
		if (FAPI_SPS_CRNTI == rntiType)
		{
			pDciMsg->rntiType = FAPI_SPS_CRNTI;
			pDciMsg->rbCoding = MAC_PHY_CONVERT_32(pDciPdu->resrcAssgmntBitMap);

			pDciMsg->harqProcessNum  = MIN_DCI_HARQ_PROCESS_NUM;
			pDciMsg->redundancyVersion_1 = 0;
			pDciMsg->vRBAssignmentFlag = 0; /* Localized */
			pDciMsg->tpc = pDciPdu->tpc;
			pDciMsg->mcs_1 = pDciPdu->tb1Info.modulationType;

			switch(msgType)
			{
				case SPS_ACTIVATION:
					{
						pDciMsg->mcs_1 = 0x0F & pDciPdu->tb1Info.modulationType;
						pDciMsg->newDataIndicator_1 = 0;
					}
					break;
				case SPS_DEACTIVATION:
					{
						pDciMsg->mcs_1 = 0x1F;
						pDciMsg->newDataIndicator_1 = 0;
						dlRBs = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->dlAvailableRBs;
						numberOfBitsForRbCoding = ceil_wrapper(log_wrapper(
									(dlRBs * (dlRBs + 1 ) / 2)) / log_wrapper(2));
						pDciMsg->rbCoding = MAC_PHY_CONVERT_32(
								(0xFFFFFFFF >> (32 - numberOfBitsForRbCoding)));
					}
					break;
				case SPS_REACTIVATION:
					{
						pDciMsg->mcs_1 = 0x0F & pDciPdu->tb1Info.modulationType;
						pDciMsg->newDataIndicator_1 = 0;
					}
					break;
				case SPS_RETX: 
					{
						pDciMsg->newDataIndicator_1 = pDciPdu->tb1Info.txIndicator;    
						pDciMsg->redundancyVersion_1 =  pDciPdu->tb1Info.redundancyVer;
						if( (MAX_HARQ_PROCESS_NB - 1) >= pDciPdu->harqProcessId )
						{
							pDciMsg->harqProcessNum = pDciPdu->harqProcessId;
						}
						else
						{
							pDciMsg->harqProcessNum  = MIN_DCI_HARQ_PROCESS_NUM;
						}
					}
					break;
				default:
					lteWarning("Invalid msgType value in DCIPduInfo \n");
					return ;

					break;
			}
			pDciMsg->allocatePRACHFlag = DEFAULT_PRACH_FLAG;
			pDciMsg->preambleIndex = DEFAULT_PREAMBLE_INDEX;
			pDciMsg->pRACHMaskIndex = DEFAULT_PRACH_MASK_INDEX;
			pDciMsg->txPower = MAC_PHY_CONVERT_16(pDciPdu->txPower);
		}
		else
		{
			/* SPS_CHG_END */
			pDciMsg->vRBAssignmentFlag = pDciPdu->virtulResrcBlkType;
#if   FLEXRAN
                        /* RAT2 Distributed partial Fix start*/
                        pDciMsg->nGAP = pDciPdu->nGAP;
                       /* RAT2 Distributed partial Fix end */
#endif
			pDciMsg->rbCoding = MAC_PHY_CONVERT_32(pDciPdu->resrcAssgmntBitMap);
			pDciMsg->mcs_1 = pDciPdu->tb1Info.modulationType;
			pDciMsg->redundancyVersion_1 = pDciPdu->tb1Info.redundancyVer;
			pDciMsg->rntiType = getRntiTypeForRnti(pDciPdu->rnti,internalCellIndex);
			/* SPR 1543 changes start */
			if (FAPI_RA_RNTI_P_RNTI_SI_RNTI != pDciMsg->rntiType)
			{    
				pDciMsg->tpc = pDciPdu->tpc;
				pDciMsg->newDataIndicator_1 = pDciPdu->tb1Info.txIndicator;
#ifdef TDD_CONFIG	 
				pDciMsg->dlAssignmentIndex = pDciPdu->dai;
#endif
				/* SPR# 891 Fix start */
				/* SPR 1483 changes start */
				if( (MAX_HARQ_PROCESS_NB - 1) >= pDciPdu->harqProcessId )
					/* SPR 1483 changes end */
				{
					pDciMsg->harqProcessNum = pDciPdu->harqProcessId;
				}
				else
				{
					pDciMsg->harqProcessNum  = MIN_DCI_HARQ_PROCESS_NUM;
				}
				/* SPR# 891 Fix End */
			}    
			else
			{
				/*SPR 1488 + SPR 1589 + SPR 1686 Starts */
				pDciMsg->tpc = pDciPdu->tpc;   
				/*SPR 1488 + SPR 1589 + SPR 1686 Ends */

				pDciMsg->harqProcessNum  = MIN_DCI_HARQ_PROCESS_NUM;
#ifdef TDD_CONFIG	 
				pDciMsg->dlAssignmentIndex = DAI_VALUE_ONE;
#endif
				/*SPR 1488 + SPR 1589 + SPR 1686 Starts */
				pDciMsg->newDataIndicator_1 = pDciPdu->tb1Info.txIndicator;
				/*SPR 1488 + SPR 1589 + SPR 1686 Ends */
			}
			/* SPR 1543 changes end */
			pDciMsg->allocatePRACHFlag = DEFAULT_PRACH_FLAG;
			pDciMsg->preambleIndex = DEFAULT_PREAMBLE_INDEX;
			pDciMsg->pRACHMaskIndex = DEFAULT_PRACH_MASK_INDEX;
			/* CLPC_REWORK CHG */
           pDciMsg->txPower = MAC_PHY_CONVERT_16(pDciPdu->txPower);
/*SPR 21001 Changes end */
			/* CLPC_REWORK_CHG END */
		}
	}
	else
	{
		pDciMsg->rntiType = getRntiTypeForRnti(pDciPdu->rnti,internalCellIndex);

		/*The Localized/Distributed VRB assignment flag and Resource block
		 * assignment values are reserved bits and set to all 1s. The prach mask
		 * index will be set to 0.*/
		pDciMsg->vRBAssignmentFlag = 0;

		/* SPR_4286_FIX Start */ 
		/*Error Removing Start*/
		dlRBs = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
			cellParams_p->dlAvailableRBs;
		/*Error Removing End*/
		numberOfBitsForRbCoding = ceil_wrapper(log_wrapper((dlRBs * (dlRBs + 1 ) / 2)) / log_wrapper(2));
		pDciMsg->rbCoding = MAC_PHY_CONVERT_32((0xFFFFFFFF >> (32 - numberOfBitsForRbCoding)));

		/* SPR_4286_FIX End */ 

		pDciMsg->mcs_1 = 0;
		pDciMsg->redundancyVersion_1 = 0;
		pDciMsg->tpc = 0;
		pDciMsg->harqProcessNum  = MIN_DCI_HARQ_PROCESS_NUM;
#ifdef TDD_CONFIG
		/* SPR 10806 changes start */
		/* As per FAPI spec, valid range of downlink assignment index is 1-4,
		 * hence, setting dlAssignmentIndex as 1 which corresponds to '00' which 
		 * 3GPP specs mentioned to be set for all other fields in DCI1A for PDCCH order */
		//pDciMsg->dlAssignmentIndex = 0;
		pDciMsg->dlAssignmentIndex = DAI_VALUE_ONE;
		/* SPR 10806 changes end */
#endif
		pDciMsg->newDataIndicator_1 = 0;
		pDciMsg->allocatePRACHFlag = TRUE;
		pDciMsg->preambleIndex = pDciPdu->preambleIdx;
		//pDciMsg->pRACHMaskIndex = DEFAULT_PRACH_MASK_INDEX;
		/* SPR_4264_FIX Start */ 
		pDciMsg->pRACHMaskIndex = 0;
		/* SPR_4264_FIX End */ 
		/* CLPC_REWORK CHG */
		pDciMsg->txPower = MAC_PHY_CONVERT_16(pDciPdu->txPower);
/*SPR 21001 Changes end */
		/* CLPC_REWORK_CHG END */

		LOG_MAC_MSG(MAC_TRANSMITTER_DCI_1A, LOGDEBUG, MAC_L1_INF,
				getCurrentTick(), pDciPdu->preambleIdx,DEFAULT_PRACH_MASK_INDEX,
				pDciMsg->rntiType, pDciMsg->allocatePRACHFlag,
				pDciMsg->harqProcessNum, DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE, __func__, "DCI_1A_PDU_PDCCH_ORDER"); 
	}

}
/*****************************************************************************
 * Function Name  :  fillDciFormat1BPdu
 * Inputs         :  pDciMsg - Pointer to message buffer to fill in the PDU 
 *                   pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message.
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function fills in the message with DCI Format 1B data
 *****************************************************************************/
#if defined(FLEXRAN)
/*SPR 21001 Changes start */
STATIC  void fillDciFormat1BPdu(
                FAPI_dciDlPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu
/*SPR 21001 Changes end */
#else
STATIC  void fillDciFormat1BPdu(
		FAPI_dciFormat1B_st *pDciMsg, DCIPduInfo *pDciPdu
#endif
)
{
	pDciMsg->aggregationLevel = pDciPdu->aggregationLvl;
	pDciMsg->vRBAssignmentFlag = pDciPdu->virtulResrcBlkType;
	pDciMsg->rbCoding = MAC_PHY_CONVERT_32(pDciPdu->resrcAssgmntBitMap);
	pDciMsg->mcs_1 = pDciPdu->tb1Info.modulationType;
	pDciMsg->redundancyVersion_1 = pDciPdu->tb1Info.redundancyVer;
	pDciMsg->newDataIndicator_1 = pDciPdu->tb1Info.txIndicator;

	/* SPR# 891 Fix start */
	/* SPR 1483 changes start */
	if( (MAX_HARQ_PROCESS_NB - 1) >= pDciPdu->harqProcessId )
		/* SPR 1483 changes end */
	{
		pDciMsg->harqProcessNum = pDciPdu->harqProcessId;
	}
	else
	{
		pDciMsg->harqProcessNum  = MIN_DCI_HARQ_PROCESS_NUM;
	}
	/* SPR# 891 Fix End */
	pDciMsg->tPMI = pDciPdu->transmittedPMI;
	pDciMsg->pmi = pDciPdu->pmiConfirmation;
	pDciMsg->tpc = pDciPdu->tpc;
#ifdef TDD_CONFIG	 
	pDciMsg->dlAssignmentIndex = pDciPdu->dai;
#endif
	pDciMsg->nGAP = 0;
	/* CLPC_REWORK CHG */
	pDciMsg->txPower = MAC_PHY_CONVERT_16(pDciPdu->txPower);
/*SPR 21001 Changes END */

	/* CLPC_REWORK_CHG END */
}
/*****************************************************************************
 * Function Name  :  fillDciFormat1CPdu
 * Inputs         :  pDciMsg - Pointer to message buffer to fill in the PDU 
 *                   pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message.
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function fills in the message with DCI Format 1C data.
 *****************************************************************************/
#if defined(FLEXRAN)
/*SPR 21001 Changes start */
STATIC  void fillDciFormat1CPdu(
                FAPI_dciDlPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu)
/*SPR 21001 Changes end */
#else
STATIC  void fillDciFormat1CPdu(
		FAPI_dciFormat1C_st *pDciMsg, DCIPduInfo *pDciPdu
)
#endif
{
	pDciMsg->aggregationLevel = pDciPdu->aggregationLvl;
	pDciMsg->rbCoding = MAC_PHY_CONVERT_32(pDciPdu->resrcAssgmntBitMap);
	/* SPR 3912 Start*/
	pDciMsg->mcs_1 = pDciMsg->tbSizeIndex = pDciPdu->tb1Info.modulationType;
	/* SPR 3912 End*/
	pDciMsg->redundancyVersion_1 = pDciPdu->tb1Info.redundancyVer;
	pDciMsg->newDataIndicator_1 = pDciPdu->tb1Info.txIndicator;
	pDciMsg->nGAP = pDciPdu->nGAP;
	/* SPR 3912 Start*/
	//pDciMsg->tbSizeIndex = pDciPdu->tb1Info.tbSize << 3;
	/* SPR 3912 End*/
	/* CLPC_REWORK CHG */
	pDciMsg->txPower = MAC_PHY_CONVERT_16(pDciPdu->txPower);
	/* CLPC_REWORK_CHG END */

}
/*****************************************************************************
 * Function Name  :  fillDciFormat1DPdu
 * Inputs         :  pDciMsg - Pointer to message buffer to fill in the PDU 
 *                   pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message.
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function fills in the message with DCI Format 1D data.
 *****************************************************************************/
#if defined(FLEXRAN)
/*SPR 21001 Changes start */
STATIC  void fillDciFormat1DPdu(
                FAPI_dciDlPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu)
/*SPR 21001 Changes end */
#else
STATIC  void fillDciFormat1DPdu(
		FAPI_dciFormat1D_st *pDciMsg, DCIPduInfo *pDciPdu
)
#endif
{
	pDciMsg->aggregationLevel = pDciPdu->aggregationLvl;
	pDciMsg->vRBAssignmentFlag = pDciPdu->virtulResrcBlkType;
	pDciMsg->rbCoding = MAC_PHY_CONVERT_32(pDciPdu->resrcAssgmntBitMap);
	pDciMsg->mcs_1 = pDciPdu->tb1Info.modulationType;
	pDciMsg->redundancyVersion_1 = pDciPdu->tb1Info.redundancyVer;
	pDciMsg->newDataIndicator_1 = pDciPdu->tb1Info.txIndicator;

	/* SPR# 891 Fix start */
	/* SPR 1483 changes start */
	if( (MAX_HARQ_PROCESS_NB - 1) >= pDciPdu->harqProcessId )
		/* SPR 1483 changes end */
	{
		pDciMsg->harqProcessNum = pDciPdu->harqProcessId;
	}
	else
	{
		pDciMsg->harqProcessNum  = MIN_DCI_HARQ_PROCESS_NUM;
	}
	/* SPR# 891 Fix End */
	pDciMsg->tPMI = pDciPdu->transmittedPMI;
	pDciMsg->tpc = pDciPdu->tpc;
#ifdef TDD_CONFIG	 
	pDciMsg->dlAssignmentIndex = pDciPdu->dai;
#endif
	pDciMsg->nGAP = 0;
	pDciMsg->dlPowerOffset = 0;
	/* CLPC_REWORK CHG */
	pDciMsg->txPower = MAC_PHY_CONVERT_16(pDciPdu->txPower);
	/* CLPC_REWORK_CHG END */
}
/*****************************************************************************
 * Function Name  :  fillDciFormat2Pdu
 * Inputs         :  pDciMsg - Pointer to message buffer to fill in the PDU
 *                   pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message.
 *                :  internalCellIndex - cell id  
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function fills in the message with DCI Format 2 data.
 *****************************************************************************/
#if defined(FLEXRAN)
/*SPR 21001 Changes start */
STATIC  void fillDciFormat2Pdu(
                FAPI_dciDlPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu
                ,InternalCellIndex internalCellIndex)
#else
STATIC  void fillDciFormat2Pdu(
		FAPI_dciFormat2_st *pDciMsg, DCIPduInfo *pDciPdu
		,InternalCellIndex internalCellIndex)
/*SPR 21001 Changes end */
#endif
{
#ifdef FDD_CONFIG    
	UInt8 containerTick = (getCurrentTick() + 1)% MAX_PDCCH_CONTAINER;
	/*CA Changes start  */
	PdcchULOutputInfo* tempPdcchULOutput_p = (pdcchULOutputInfo_gp[internalCellIndex]+containerTick);
	/*CA Changes end  */
	ULSchUciRachPoolInfo * ulSchUciPoolInfo_p = NULL;
	ULSchUciInfo   * ulSchUciInfo_p = NULL;
	UInt8 harqSizeChange = 0; 
#endif
	pDciMsg->aggregationLevel = pDciPdu->aggregationLvl;
	pDciMsg->resAllocationType = pDciPdu->resrcAllocType;
	pDciMsg->rbCoding = MAC_PHY_CONVERT_32(pDciPdu->resrcAssgmntBitMap);
	pDciMsg->mcs_1 = pDciPdu->tb1Info.modulationType;
	pDciMsg->redundancyVersion_1 = pDciPdu->tb1Info.redundancyVer;
	pDciMsg->newDataIndicator_1 = pDciPdu->tb1Info.txIndicator;
	pDciMsg->tbToCodeWordSwapFlag = pDciPdu->tbSwapFlag;
	pDciMsg->mcs_2 = pDciPdu->tb2Info.modulationType;
	pDciMsg->redundancyVersion_2 = pDciPdu->tb2Info.redundancyVer;
	pDciMsg->newDataIndicator_2 = pDciPdu->tb2Info.txIndicator;

	/* SPR 3053 */

	UInt32 ueIndex = 0;
	DLUEContextInfo *ueContextInfo_p = PNULL;
	DLHARQProcess *harqProcess_p = PNULL;
	ueIndex=pDciPdu->ueIndex;
	ueContextInfo_p = &dlUECtxInfoArr_g[ueIndex];

	if(INVALID_HARQ_ID != pDciPdu->harqProcessId)
	{

		if(MAX_UE_SUPPORTED > ueIndex)
		{
			/* + SPR 11469 Changes */
			if(IS_PCELL_OF_UE(ueContextInfo_p->dlUEContext_p,internalCellIndex))
			{
				harqProcess_p =
					&ueContextInfo_p->dlUEContext_p->dlHarqContext_p
					->harqProcess[pDciPdu->harqProcessId];
			}
			else
			{
				harqProcess_p =
					&ueContextInfo_p->dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->
					ScelldlHarqContext_p->harqProcess[pDciPdu->harqProcessId];
			}
			/* + SPR 11469 Changes */
			if ((FALSE == harqProcess_p->isTBOneValid ) || (FALSE == pDciPdu->isTBOneValid))
			{
				pDciMsg->mcs_1 = 0;
				pDciMsg->redundancyVersion_1 = 1;
#ifdef FDD_CONFIG
				harqSizeChange = 1;
#endif
			}
			if ((FALSE == harqProcess_p->isTBTwoValid ) || (FALSE == pDciPdu->isTBTwoValid))
			{

				pDciMsg->mcs_2 = 0;
				pDciMsg->redundancyVersion_2 = 1;
#ifdef FDD_CONFIG    
				harqSizeChange = 1;
#endif
			}
		}
	}
	if (FALSE == pDciPdu->isTBTwoValid)
	{
		pDciMsg->mcs_2 = 0;
		pDciMsg->redundancyVersion_2 = 1;
#ifdef FDD_CONFIG    
		harqSizeChange = 1;
#endif
	}

#ifdef FDD_CONFIG    
	/* SPR_12004_fix Start */
	if ( (!ueContextInfo_p->dlUEContext_p->scellCount) &&  harqSizeChange)
		/* SPR_12004_fix End */
	{
		/*Coverity 10171 Fix Start*/
		if ((MAX_UE_SUPPORTED > pDciPdu->ueIndex) && (ULSCH_UCI_INVALID_INDEX !=
					tempPdcchULOutput_p->directIndexingUEInfo[pDciPdu->ueIndex].ulschUciInfoIndex))
		{
			/*CA Changes start  */
			ulSchUciPoolInfo_p = ulSchUciRachContainer_gp[internalCellIndex] + containerTick;
			/*CA Changes end  */
			ulSchUciInfo_p =
				&ulSchUciPoolInfo_p->ulschUciInfo[tempPdcchULOutput_p->directIndexingUEInfo[pDciPdu->ueIndex].ulschUciInfoIndex];
			ulSchUciInfo_p->dlHarqInfo.harqSize = 1;
		}
	}
    /* coverity fix 73032 start */
	//  harqSizeChange = 0;
	/* coverity fix 73032 start */
#endif

	/* SPR 3053 */

	/* SPR# 891 Fix start */
	/* SPR 1483 changes start */
	if( (MAX_HARQ_PROCESS_NB - 1) >= pDciPdu->harqProcessId )
		/* SPR 1483 changes end */
	{
		pDciMsg->harqProcessNum = pDciPdu->harqProcessId;
	}
	else
	{
		pDciMsg->harqProcessNum  = MIN_DCI_HARQ_PROCESS_NUM;
	}
	/* SPR# 891 Fix End */
	/* + CL-MIMO LJA*/
	/*pDciMsg->preCodingInfo = pDciPdu->codeBookIndex;*/
	/* - CL-MIMO LJA*/
	/* + CQI_5.0 */
	pDciMsg->preCodingInfo = pDciPdu->codeBookIndex;
	/* - CQI_5.0 */
	pDciMsg->tpc = pDciPdu->tpc;

#ifdef TDD_CONFIG	 
	pDciMsg->dlAssignmentIndex = pDciPdu->dai;
#endif
	pDciMsg->rntiType = getRntiTypeForRnti(pDciPdu->rnti, internalCellIndex);
	/* CLPC_REWORK CHG */
	pDciMsg->txPower = MAC_PHY_CONVERT_16(pDciPdu->txPower);
/*SPR 21001 Changes end */
	/* CLPC_REWORK_CHG END */

}
/*****************************************************************************
 * Function Name  :  fillDciFormat2APdu
 * Inputs         :  pDciMsg - Pointer to message buffer to fill in the PDU 
 *                   pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message.
 *                :  internalCellIndex - cell id  
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function fills in the message with DCI Format 2A data.
 *****************************************************************************/
#if defined(FLEXRAN)
/*SPR 21001 Changes start */
STATIC  void fillDciFormat2APdu(
                FAPI_dciDlPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu,
                InternalCellIndex internalCellIndex)
#else

STATIC  void fillDciFormat2APdu(
		FAPI_dciFormat2A_st *pDciMsg, DCIPduInfo *pDciPdu,
		InternalCellIndex internalCellIndex)
/*SPR 21001 Changes end */
#endif
{
#ifdef FDD_CONFIG    
	UInt8 containerTick = (getCurrentTick() + 1)% MAX_PDCCH_CONTAINER;
	/*CA Changes start  */
	PdcchULOutputInfo* tempPdcchULOutput_p = (pdcchULOutputInfo_gp[internalCellIndex] + containerTick);
	/*CA Changes end  */
	ULSchUciRachPoolInfo * ulSchUciPoolInfo_p = NULL;
	ULSchUciInfo   * ulSchUciInfo_p = NULL;
	/* SPR 5509 START */
	UInt8 harqSizeChange = 0; 
#endif
	/* SPR 5509 END */
	pDciMsg->aggregationLevel = pDciPdu->aggregationLvl;
	pDciMsg->resAllocationType = pDciPdu->resrcAllocType;
	pDciMsg->rbCoding = MAC_PHY_CONVERT_32(pDciPdu->resrcAssgmntBitMap);
	pDciMsg->mcs_1 = pDciPdu->tb1Info.modulationType;
	pDciMsg->redundancyVersion_1 = pDciPdu->tb1Info.redundancyVer;
	pDciMsg->newDataIndicator_1 = pDciPdu->tb1Info.txIndicator;
	pDciMsg->tbToCodeWordSwapFlag = pDciPdu->tbSwapFlag;
	pDciMsg->mcs_2 = pDciPdu->tb2Info.modulationType;
	pDciMsg->redundancyVersion_2 = pDciPdu->tb2Info.redundancyVer;

	/* SPR 3053 */

	UInt32 ueIndex = 0;
	DLUEContextInfo *ueContextInfo_p = PNULL;
	DLHARQProcess *harqProcess_p = PNULL;
	ueIndex=pDciPdu->ueIndex;
	ueContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
	if(INVALID_HARQ_ID != pDciPdu->harqProcessId)
	{

		if(MAX_UE_SUPPORTED > ueIndex)
		{
			/* + SPR 11469 Changes */
			if(IS_PCELL_OF_UE(ueContextInfo_p->dlUEContext_p,internalCellIndex))
			{
				harqProcess_p =
					&ueContextInfo_p->dlUEContext_p->dlHarqContext_p
					->harqProcess[pDciPdu->harqProcessId];
			}
			else
			{
				harqProcess_p =
					&ueContextInfo_p->dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->
					ScelldlHarqContext_p->harqProcess[pDciPdu->harqProcessId];
			}
			/* - SPR 11469 Changes */
			if ((FALSE == harqProcess_p->isTBOneValid ) || (FALSE == pDciPdu->isTBOneValid))
			{
				pDciMsg->mcs_1 = 0;
				pDciMsg->redundancyVersion_1 = 1;
#ifdef FDD_CONFIG    
				harqSizeChange = 1;
#endif
			}
			if ((FALSE == harqProcess_p->isTBTwoValid ) || (FALSE == pDciPdu->isTBTwoValid))
			{
				pDciMsg->mcs_2 = 0;
				pDciMsg->redundancyVersion_2 = 1;
#ifdef FDD_CONFIG    
				harqSizeChange = 1;
#endif
			}
		}
	}
	if (FALSE == pDciPdu->isTBTwoValid)
	{
		pDciMsg->mcs_2 = 0;
		pDciMsg->redundancyVersion_2 = 1;
#ifdef FDD_CONFIG    
		harqSizeChange = 1;
#endif
	}

	/* SPR 5509 START */
#ifdef FDD_CONFIG
	/* SPR 5509 END */
	/* SPR_12004_fix Start */
	if ( (!ueContextInfo_p->dlUEContext_p->scellCount) &&  harqSizeChange)
		/* SPR_12004_fix End */
	{
		/*Coverity 10169 Start Fix*/
		if ((MAX_UE_SUPPORTED > pDciPdu->ueIndex) && (ULSCH_UCI_INVALID_INDEX !=
					tempPdcchULOutput_p->directIndexingUEInfo[pDciPdu->ueIndex].ulschUciInfoIndex))
		{
			/*CA Changes start  */
			ulSchUciPoolInfo_p = ulSchUciRachContainer_gp[internalCellIndex] + containerTick;
			/*CA Changes end  */
			ulSchUciInfo_p =
				&ulSchUciPoolInfo_p->ulschUciInfo[tempPdcchULOutput_p->directIndexingUEInfo[pDciPdu->ueIndex].ulschUciInfoIndex];
			ulSchUciInfo_p->dlHarqInfo.harqSize = 1;
		}
	}
	/* SPR 3053 */
#endif
	pDciMsg->newDataIndicator_2 = pDciPdu->tb2Info.txIndicator;
	/* SPR# 891 Fix start */
	/* SPR 1483 changes start */
	if( (MAX_HARQ_PROCESS_NB - 1) >= pDciPdu->harqProcessId )
		/* SPR 1483 changes end */
	{
		pDciMsg->harqProcessNum = pDciPdu->harqProcessId;
	}
	else
	{
		pDciMsg->harqProcessNum  = MIN_DCI_HARQ_PROCESS_NUM;
	}
	/* SPR# 891 Fix End */
	pDciMsg->preCodingInfo = pDciPdu->codeBookIndex;
	pDciMsg->tpc = pDciPdu->tpc;

#ifdef TDD_CONFIG	 
	pDciMsg->dlAssignmentIndex = pDciPdu->dai;
#endif
	pDciMsg->rntiType = getRntiTypeForRnti(pDciPdu->rnti,internalCellIndex);
	/* CLPC_REWORK CHG */
	pDciMsg->txPower = MAC_PHY_CONVERT_16(pDciPdu->txPower);
/*SPR 21001 Changes end */
	/* CLPC_REWORK_CHG END */

}
/* + TM7_8 Changes Start */
/*****************************************************************************
 *Function Name  :  fillDciFormat2BPdu
 * Inputs         :  pDciMsg - Pointer to message buffer to fill in the PDU 
 *                   pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message.
 *                :  internalCellIndex - cell id  
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function fills in the message with DCI Format 2B data.
 *****************************************************************************/
#if defined(FLEXRAN)
/*SPR 21001 Changes start */
STATIC  void fillDciFormat2BPdu(
                FAPI_dciDlPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu,
                InternalCellIndex internalCellIndex)
#else
STATIC  void fillDciFormat2BPdu(
		FAPI_dciFormat2B_st *pDciMsg, DCIPduInfo *pDciPdu,
		InternalCellIndex internalCellIndex)
/*SPR 21001 Changes end */
#endif
{
#ifdef FDD_CONFIG    
	UInt8 containerTick = (getCurrentTick() + 1)% MAX_PDCCH_CONTAINER;
	/*CA Changes start  */
	PdcchULOutputInfo* tempPdcchULOutput_p = (pdcchULOutputInfo_gp[internalCellIndex] + containerTick);
	/*CA Changes end  */
	ULSchUciRachPoolInfo * ulSchUciPoolInfo_p = NULL;
	ULSchUciInfo   * ulSchUciInfo_p = NULL;
	UInt8 harqSizeChange = 0; 
#endif
	pDciMsg->aggregationLevel = pDciPdu->aggregationLvl;
	pDciMsg->resAllocationType = pDciPdu->resrcAllocType;
	pDciMsg->rbCoding = MAC_PHY_CONVERT_32(pDciPdu->resrcAssgmntBitMap);
	pDciMsg->mcs_1 = pDciPdu->tb1Info.modulationType;
	pDciMsg->redundancyVersion_1 = pDciPdu->tb1Info.redundancyVer;
	pDciMsg->newDataIndicator_1 = pDciPdu->tb1Info.txIndicator;
	pDciMsg->mcs_2 = pDciPdu->tb2Info.modulationType;
	pDciMsg->redundancyVersion_2 = pDciPdu->tb2Info.redundancyVer;


	UInt32 ueIndex = 0;
	DLUEContextInfo *ueContextInfo_p = PNULL;
	DLHARQProcess *harqProcess_p = PNULL;
	ueIndex=pDciPdu->ueIndex;
	ueContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
	if(INVALID_HARQ_ID != pDciPdu->harqProcessId)
	{
		if(MAX_UE_SUPPORTED > ueIndex)
		{
			/* + SPR 11469 Changes */
			if(IS_PCELL_OF_UE(ueContextInfo_p->dlUEContext_p,internalCellIndex))
			{
				harqProcess_p =
					&ueContextInfo_p->dlUEContext_p->dlHarqContext_p
					->harqProcess[pDciPdu->harqProcessId];
			}
			else
			{
				harqProcess_p =
					&ueContextInfo_p->dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->
					ScelldlHarqContext_p->harqProcess[pDciPdu->harqProcessId];
			}
			/* - SPR 11469 Changes */
			if ((FALSE == harqProcess_p->isTBOneValid ) || (FALSE == pDciPdu->isTBOneValid))
			{
				pDciMsg->mcs_1 = 0;
				pDciMsg->redundancyVersion_1 = 1;
#ifdef FDD_CONFIG    
				harqSizeChange = 1;
#endif
			}
			if ((FALSE == harqProcess_p->isTBTwoValid ) || (FALSE == pDciPdu->isTBTwoValid))
			{
				pDciMsg->mcs_2 = 0;
				pDciMsg->redundancyVersion_2 = 1;
#ifdef FDD_CONFIG    
				harqSizeChange = 1;
#endif
			}
		}
	}
	if (FALSE == pDciPdu->isTBTwoValid)
	{
		pDciMsg->mcs_2 = 0;
		pDciMsg->redundancyVersion_2 = 1;
#ifdef FDD_CONFIG    
		harqSizeChange = 1;
#endif
	}

#ifdef FDD_CONFIG
	/* SPR_12004_fix Start */
	if ( (!ueContextInfo_p->dlUEContext_p->scellCount) &&  harqSizeChange)
		/* SPR_12004_fix End */
	{
		if ((MAX_UE_SUPPORTED > pDciPdu->ueIndex) && (ULSCH_UCI_INVALID_INDEX !=
					tempPdcchULOutput_p->directIndexingUEInfo[pDciPdu->ueIndex].ulschUciInfoIndex))
		{
			/*CA Changes start  */
			ulSchUciPoolInfo_p = ulSchUciRachContainer_gp[internalCellIndex] + containerTick;
			/*CA Changes end  */
			ulSchUciInfo_p =
				&ulSchUciPoolInfo_p->ulschUciInfo[tempPdcchULOutput_p->directIndexingUEInfo[pDciPdu->ueIndex].ulschUciInfoIndex];
			ulSchUciInfo_p->dlHarqInfo.harqSize = 1;
		}
	}
    /* coverity fix 73016 start */
	//harqSizeChange = 0;
	/* coverity fix 73016 start */
#endif
	pDciMsg->newDataIndicator_2 = pDciPdu->tb2Info.txIndicator;
	if( (MAX_HARQ_PROCESS_NB - 1) >= pDciPdu->harqProcessId )
	{
		pDciMsg->harqProcessNum = pDciPdu->harqProcessId;
	}
	else
	{
		pDciMsg->harqProcessNum  = MIN_DCI_HARQ_PROCESS_NUM;
	}
	pDciMsg->tpc = pDciPdu->tpc;

#ifdef TDD_CONFIG	 
	pDciMsg->dlAssignmentIndex = pDciPdu->dai;
#endif
	pDciMsg->rntiType = getRntiTypeForRnti(pDciPdu->rnti,internalCellIndex);
/*SPR 21001 Changes start */
    pDciMsg->txPower = MAC_PHY_CONVERT_16(pDciPdu->txPower);
#ifdef FLEXRAN
    pDciMsg->scramblingId = pDciPdu->scramblingId;
#else
	pDciMsg->scramblingId = 0;
#endif

}
/* - TM7_8 Changes End */
#ifdef FAPI_4_0_COMPLIANCE
/*****************************************************************************
 * Function Name  : fillDCIDLR9Params 
 * Inputs         :  dciRelease9_p - Pointer to Release9 structure of DCI 
 *                                  PDU.
 *                :  pDciPdu - Pointer to DCI PDU containing the info to fill 
 *                    the Release9 Parameters 
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This fucntion fills in the  
 *                    Release 9 Prameters in DCI PDU
 *****************************************************************************/
void fillDCIDLR9Params(FAPI_dciRelease9Param_st *dciRelease9_p,DCIPduInfo *pDciPdu)
{
	/*FAPI2_1 changes start*/
	dciRelease9_p->mcchFlag=DEFAULT_MCCHFLAG;
	dciRelease9_p->mcchChangeNotification=DEFAULT_MCCH_CHANGE_NOTIF;
	dciRelease9_p->scramblingIndentity=pDciPdu->scramblingId;
	/*FAPI2_1 changes end*/

	return;
}
#endif
/* SPR 19288 change start */
/*****************************************************************************
 * Function Name  : fillDCIDLR10Params 
 * Inputs         :  dciRelease10_p - Pointer to Release10 structure of DCI 
 *                                  PDU.
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function fills in the  
 *                    Release 10 Prameters in DCI PDU
 *****************************************************************************/
/*SPR 19565 +*/
#ifdef FAPI_4_0_COMPLIANCE
void fillDCIDLR10Params(FAPI_dciRelease10Param_st *dciRelease10_p)
{

	/*FAPI2_1 changes start*/
	dciRelease10_p->crossCarrierSchedulingFlag=DEFAULT_CROSSCARRIER_SCH_FLAG;
	dciRelease10_p->carrierIndicator=DEFAULT_CARRIER_INDICATOR;
	dciRelease10_p->srsFlag=DEFAULT_SRS;
	dciRelease10_p->srsRequest=DEFAULT_SRS_REQUEST;
	dciRelease10_p->antennaPortScramblingAndLayers=DEFAULT_ANTENNA_SCRAMBLING_NUM_LAYERS;
	/*FAPI2_1 changes end*/
	return;
}
#endif
/*SPR 19565 -*/
/* SPR 19288 change end */
/* SPR 19288 change start */
#ifndef FLEXRAN

/*****************************************************************************
 * Function Name  :  addDciPduToDlConfigR9
 * Inputs         :  pDlConfigPdu - Pointer to message buffer to fill in the
 *                                  PDU.
 *                :  pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message.
 *                   pDciCceInfo- Pointer to DCI CCE Info
 *                :  internalCellIndex - cell id  
 * Outputs        :
 * Returns        :  Pointer to the buffer which can be used to fill in the
 *                   next PDU
 * Description    :  This fucntion fills in the message with the DCI PDU data
 *                   and returns the pointer to the buffer which can be used to
 *                   fill in the next PDU.This Function will be used in case PHY 
 *                   Supports Release9.
 *****************************************************************************/
FAPI_dlConfigPDUInfo_st *addDciPduToDlConfigR9(
		FAPI_dlConfigPDUInfo_st *pDlConfigPdu, DCIPduInfo *pDciPdu,
		DciCCEInfo *pDciCceInfo,InternalCellIndex internalCellIndex)
{
	FAPI_dlConfigPDUInfo_st     *pNextPdu = PNULL;
	pDlConfigPdu->pduType = FAPI_DCI_DL_PDU;
	pDlConfigPdu->dlConfigpduInfo.DCIPdu.cceIndex = pDciPdu->cceIndex;
	pDlConfigPdu->dlConfigpduInfo.DCIPdu.rnti = MAC_PHY_CONVERT_16(pDciPdu->rnti);

/*SPR 19565 +*/
#ifdef FAPI_4_0_COMPLIANCE
	FAPI_dciRelease9Param_st *dciRelease9_p = PNULL;
	FAPI_dciRelease10Param_st *dciRelease10_p = PNULL;
	FAPI_dciFormat1_st *tempPtr1_p = PNULL;
	FAPI_dciFormat1A_st *tempPtr1A_p = PNULL;
	FAPI_dciFormat1B_st *tempPtr1B_p = PNULL;
	FAPI_dciFormat1C_st *tempPtr1C_p = PNULL;
	FAPI_dciFormat1D_st *tempPtr1D_p = PNULL;
	FAPI_dciFormat2_st *tempPtr2_p = PNULL;
	FAPI_dciFormat2A_st *tempPtr2A_p = PNULL;
	FAPI_dciFormat2B_st *tempPtr2B_p = PNULL;
#endif
/*SPR 19565 -*/
	switch (pDciPdu->dciFormat)
	{
		case DCI_FORMAT_1:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_1;
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat1_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat1Pdu(((FAPI_dciFormat1_st *)
							&pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu,internalCellIndex);
/*SPR 21001 Changes end */
/*SPR 19565 +*/
#if defined FAPI_4_0_COMPLIANCE
				tempPtr1_p = (FAPI_dciFormat1_st *)&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]);
				dciRelease9_p = &tempPtr1_p->release9Param;
				dciRelease10_p =  &tempPtr1_p->release10Param;
#endif
/*SPR 19565 -*/
				break;
			}
		case DCI_FORMAT_1A:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_1A;
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat1A_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat1APdu((FAPI_dciFormat1A_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu,pDciCceInfo->msgType,internalCellIndex);
/*SPR 21001 Changes end */
				/*In case DCI1A PDU is encodeed for PDCCH Order, explicitly
				 * reset CCE because DLSCH PDU will not be formed in this case.
				 * Also reset the msgType flag and premable Index*/
				if(PDCCH_ORDER_MSG == pDciCceInfo->msgType)
				{
					/*  RESET_CCE(pDciPdu->aggregationLvl, pDciCceInfo);*/
					pDciCceInfo->msgType = BCCH_MSG;
					pDciPdu->preambleIdx = INVALID_PREAMBLE_IDX;
				}
				else if(SPS_DEACTIVATION ==  pDciCceInfo->msgType)
				{
					/*  RESET_CCE(pDciPdu->aggregationLvl, pDciCceInfo);*/
				}
/*SPR 19565 +*/
#if defined FAPI_4_0_COMPLIANCE
				tempPtr1A_p = (FAPI_dciFormat1A_st *)&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]);
				dciRelease9_p = &tempPtr1A_p->release9Param;
				dciRelease10_p =  &tempPtr1A_p->release10Param;
#endif
/*SPR 19565 -*/
				break;
			}
		case DCI_FORMAT_1B:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_1B;
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat1B_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat1BPdu((FAPI_dciFormat1B_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu);
/*SPR 21001 Changes end */
/*SPR 19565 +*/
#if defined (FAPI_4_0_COMPLIANCE) 
				tempPtr1B_p = (FAPI_dciFormat1B_st *)&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]);

				dciRelease9_p = &tempPtr1B_p->release9Param;
				dciRelease10_p =  &tempPtr1B_p->release10Param;
#endif
/*SPR 19565 -*/
				break;
			}
		case DCI_FORMAT_1C:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_1C;
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat1C_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat1CPdu((FAPI_dciFormat1C_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu);
/*SPR 21001 Changes end */
/*SPR 19565 +*/
 #if defined (FAPI_4_0_COMPLIANCE) 
				tempPtr1C_p = (FAPI_dciFormat1C_st *)&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]);
				dciRelease9_p = &tempPtr1C_p->release9Param;
				dciRelease10_p =  &tempPtr1C_p->release10Param;
#endif
#ifdef LTE_EMBMS_SUPPORTED
                dciRelease9_p->mcchFlag = 1;
                dciRelease9_p->mcchChangeNotification = dci1CAreaBitMap_g[internalCellIndex];
                /* SPR 18296 fix start*/
                dci1CAreaBitMap_g[internalCellIndex] = 0;
                isDci1cOcassion_g[internalCellIndex] = FALSE;
                /* SPR 18296 fix end*/
#endif

/*SPR 19565 -*/
				break;
			}
		case DCI_FORMAT_1D:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_1D;
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat1D_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat1DPdu((FAPI_dciFormat1D_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu);
/*SPR 21001 Changes end */
/*SPR 19565 +*/
 #if defined (FAPI_4_0_COMPLIANCE) 
				tempPtr1D_p = (FAPI_dciFormat1D_st *)&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]);
				dciRelease9_p = &tempPtr1D_p->release9Param;
				dciRelease10_p =  &tempPtr1D_p->release10Param;
#endif
/*SPR 19565 -*/
				break;
			}
		case DCI_FORMAT_2:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_2;
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat2_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat2Pdu((FAPI_dciFormat2_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu,internalCellIndex);
/*SPR 21001 Changes end */
/*SPR 19565 +*/
 #if defined (FAPI_4_0_COMPLIANCE) 
				tempPtr2_p = (FAPI_dciFormat2_st *)&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]);
				dciRelease9_p = &tempPtr2_p->release9Param;
				dciRelease10_p =  &tempPtr2_p->release10Param;
#endif
/*SPR 19565 -*/
				break;
			}
		case DCI_FORMAT_2A:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_2A;
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat2A_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat2APdu((FAPI_dciFormat2A_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu,internalCellIndex);
/*SPR 21001 Changes end */
/*SPR 19565 +*/
 #if defined (FAPI_4_0_COMPLIANCE) 
				tempPtr2A_p = (FAPI_dciFormat2A_st *)&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]);
				dciRelease9_p = &tempPtr2A_p->release9Param;
				dciRelease10_p =  &tempPtr2A_p->release10Param;
#endif
/*SPR 19565 -*/
				break;
			}
			/* + TM7_8 Changes Start */
		case DCI_FORMAT_2B:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_2B;
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat2B_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat2BPdu((FAPI_dciFormat2B_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu,internalCellIndex);
/*SPR 21001 Changes end */
/*SPR 19565 +*/
 #if defined (FAPI_4_0_COMPLIANCE) 
				tempPtr2B_p = (FAPI_dciFormat2B_st *)&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]);
				dciRelease9_p = &tempPtr2B_p->release9Param;
				dciRelease10_p =  &tempPtr2B_p->release10Param;
#endif
/*SPR 19565 -*/
				break;
			}
			/* - TM7_8 Changes End */

		case DCI_FORMAT_0:
		case DCI_FORMAT_0_MSG3:
		case DCI_FORMAT_3:
		case DCI_FORMAT_3A:
		default:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat = 0;
				LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,
						"[%s] Exit Wrong DCI format %d, PDU dropped",
						__func__, pDciPdu->dciFormat);
				/* This PDU will be dropped, returning the current pointer */
				return pDlConfigPdu;
			}
	}
/*SPR 19565 +*/
#if defined (FAPI_4_0_COMPLIANCE) 
	fillDCIDLR9Params(dciRelease9_p,pDciPdu);
	fillDCIDLR10Params(dciRelease10_p);
#endif
/*SPR 19565 -*/
	/* pdu size should be a multiple of 4 bytes */
	/* SPR 1132 starts*/ 
	if ( pDlConfigPdu->pduSize % sizeof(UInt32) )
	{
		pDlConfigPdu->pduSize += sizeof(UInt32) - (pDlConfigPdu->pduSize % sizeof(UInt32));
	}
	/* SPR 1132 end */ 
	/*pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8 *)pDlConfigPdu) + (
	  sizeof(FAPI_dciDLPduInfo_st) - 1) + pDlConfigPdu->pduSize);*/
	pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8 *)pDlConfigPdu)
			+ pDlConfigPdu->pduSize);
/*SPR 21001 Changes end */
	return pNextPdu;
}

/*****************************************************************************
 * Function Name  :  addDciPduToDlConfigR10
 * Inputs         :  pDlConfigPdu - Pointer to message buffer to fill in the
 *                                  PDU.
 *                :  pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message.
 *                   pDciCceInfo- Pointer to DCI CCE Info
 *                :  internalCellIndex - cell id  
 * Outputs        :
 * Returns        :  Pointer to the buffer which can be used to fill in the
 *                   next PDU
 * Description    :  This fucntion fills in the message with the DCI PDU data
 *                   and returns the pointer to the buffer which can be used to
 *                   fill in the next PDU.This Function will be used in case PHY
 *                    Supports Release10.
 *****************************************************************************/
FAPI_dlConfigPDUInfo_st *addDciPduToDlConfigR10(
		FAPI_dlConfigPDUInfo_st *pDlConfigPdu, DCIPduInfo *pDciPdu,
		DciCCEInfo *pDciCceInfo,InternalCellIndex internalCellIndex)
{
	FAPI_dlConfigPDUInfo_st     *pNextPdu = PNULL;
	pDlConfigPdu->pduType = FAPI_DCI_DL_PDU;
	pDlConfigPdu->dlConfigpduInfo.DCIPdu.cceIndex = pDciPdu->cceIndex;
	pDlConfigPdu->dlConfigpduInfo.DCIPdu.rnti = MAC_PHY_CONVERT_16(pDciPdu->rnti);
/*SPR 19565 +*/
#ifdef FAPI_4_0_COMPLIANCE
	FAPI_dciRelease9Param_st *dciRelease9_p = PNULL;
	FAPI_dciRelease10Param_st *dciRelease10_p = PNULL;
	FAPI_dciFormat1_st *tempPtr1_p = PNULL;
	FAPI_dciFormat1A_st *tempPtr1A_p = PNULL;
	FAPI_dciFormat1B_st *tempPtr1B_p = PNULL;
	FAPI_dciFormat1C_st *tempPtr1C_p = PNULL;
	FAPI_dciFormat1D_st *tempPtr1D_p = PNULL;
	FAPI_dciFormat2_st *tempPtr2_p = PNULL;
	FAPI_dciFormat2A_st *tempPtr2A_p = PNULL;
	FAPI_dciFormat2B_st *tempPtr2B_p = PNULL;
#endif
/*SPR 19565 -*/

	switch (pDciPdu->dciFormat)
	{
		case DCI_FORMAT_1:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_1;
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat1_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat1Pdu(((FAPI_dciFormat1_st *)
							&pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu,internalCellIndex);
/*SPR 19565 +*/
#ifdef FAPI_4_0_COMPLIANCE
				tempPtr1_p = (FAPI_dciFormat1_st *)&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]);
				dciRelease9_p = &tempPtr1_p->release9Param;
				dciRelease10_p =  &tempPtr1_p->release10Param;
#endif
/*SPR 19565 -*/
				break;
			}
		case DCI_FORMAT_1A:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_1A;
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat1A_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat1APdu((FAPI_dciFormat1A_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu,pDciCceInfo->msgType,internalCellIndex);
				/*In case DCI1A PDU is encodeed for PDCCH Order, explicitly
				 * reset CCE because DLSCH PDU will not be formed in this case.
				 * Also reset the msgType flag and premable Index*/
				if(PDCCH_ORDER_MSG == pDciCceInfo->msgType)
				{
					/*  RESET_CCE(pDciPdu->aggregationLvl, pDciCceInfo);*/
					pDciCceInfo->msgType = BCCH_MSG;
					pDciPdu->preambleIdx = INVALID_PREAMBLE_IDX;
				}
				else if(SPS_DEACTIVATION ==  pDciCceInfo->msgType)
				{
					/*  RESET_CCE(pDciPdu->aggregationLvl, pDciCceInfo);*/
				}
/*SPR 19565 +*/
#if defined (FAPI_4_0_COMPLIANCE) 
				tempPtr1A_p = (FAPI_dciFormat1A_st *)&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]);
				dciRelease9_p = &tempPtr1A_p->release9Param;
				dciRelease10_p =  &tempPtr1A_p->release10Param;
#endif
/*SPR 19565 -*/
				break;
			}
		case DCI_FORMAT_1B:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_1B;
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat1B_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat1BPdu((FAPI_dciFormat1B_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu);
/*SPR 19565 +*/
#ifdef FAPI_4_0_COMPLIANCE
				tempPtr1B_p = (FAPI_dciFormat1B_st *)&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]);
				dciRelease9_p = &tempPtr1B_p->release9Param;
				dciRelease10_p =  &tempPtr1B_p->release10Param;
#endif
/*SPR 19565 -*/
				break;
			}
		case DCI_FORMAT_1C:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_1C;
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat1C_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat1CPdu((FAPI_dciFormat1C_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu);
/*SPR 19565 +*/
#ifdef FAPI_4_0_COMPLIANCE
				tempPtr1C_p = (FAPI_dciFormat1C_st *)&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]);
				dciRelease9_p = &tempPtr1C_p->release9Param;
				dciRelease10_p =  &tempPtr1C_p->release10Param;
#endif
/*SPR 19565 -*/
#ifdef LTE_EMBMS_SUPPORTED
                dciRelease9_p->mcchFlag = 1;
                dciRelease9_p->mcchChangeNotification = dci1CAreaBitMap_g[internalCellIndex];
                /* SPR 18296 fix start*/
                dci1CAreaBitMap_g[internalCellIndex] = 0;
                isDci1cOcassion_g[internalCellIndex] = FALSE;
                /* SPR 18296 fix end*/
#endif

				break;
			}
		case DCI_FORMAT_1D:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_1D;
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat1D_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat1DPdu((FAPI_dciFormat1D_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu);
/*SPR 19565 +*/
#ifdef FAPI_4_0_COMPLIANCE
				tempPtr1D_p = (FAPI_dciFormat1D_st *)&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]);
				dciRelease9_p = &tempPtr1D_p->release9Param;
				dciRelease10_p =  &tempPtr1D_p->release10Param;
#endif
/*SPR 19565 -*/
				break;
			}
		case DCI_FORMAT_2:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_2;
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat2_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat2Pdu((FAPI_dciFormat2_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu,internalCellIndex);
/*SPR 19565 +*/
#ifdef FAPI_4_0_COMPLIANCE
				tempPtr2_p = (FAPI_dciFormat2_st *)&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]);
				dciRelease9_p = &tempPtr2_p->release9Param;
				dciRelease10_p =  &tempPtr2_p->release10Param;
#endif
/*SPR 19565 -*/
				break;
			}
		case DCI_FORMAT_2A:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_2A;
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat2A_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat2APdu((FAPI_dciFormat2A_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu,internalCellIndex);
/*SPR 19565 +*/
#ifdef FAPI_4_0_COMPLIANCE
				tempPtr2A_p = (FAPI_dciFormat2A_st *)&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]);
				dciRelease9_p = &tempPtr2A_p->release9Param;
				dciRelease10_p =  &tempPtr2A_p->release10Param;
#endif
/*SPR 19565 -*/
				break;
			}
			/* + TM7_8 Changes Start */
		case DCI_FORMAT_2B:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_2B;
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat2B_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat2BPdu((FAPI_dciFormat2B_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu,internalCellIndex);
/*SPR 19565 +*/
#ifdef FAPI_4_0_COMPLIANCE
				tempPtr2B_p = (FAPI_dciFormat2B_st *)&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]);
				dciRelease9_p = &tempPtr2B_p->release9Param;
				dciRelease10_p =  &tempPtr2B_p->release10Param;
#endif
/*SPR 19565 -*/
				break;
			}
			/* - TM7_8 Changes End */

		case DCI_FORMAT_0:
		case DCI_FORMAT_0_MSG3:
		case DCI_FORMAT_3:
		case DCI_FORMAT_3A:
		default:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat = 0;
				LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,
						"[%s] Exit Wrong DCI format %d, PDU dropped",
						__func__, pDciPdu->dciFormat);
				/* This PDU will be dropped, returning the current pointer */
				return pDlConfigPdu;
			}
	}
/*SPR 19565 +*/
#ifdef FAPI_4_0_COMPLIANCE
	fillDCIDLR9Params(dciRelease9_p,pDciPdu);
	fillDCIDLR10Params(dciRelease10_p);
#endif
/*SPR 19565 -*/
	/* pdu size should be a multiple of 4 bytes */
	/* SPR 1132 starts*/ 
	if ( pDlConfigPdu->pduSize % sizeof(UInt32) )
	{
		pDlConfigPdu->pduSize += sizeof(UInt32) - (pDlConfigPdu->pduSize % sizeof(UInt32));
	}
	/* SPR 1132 end */ 
	/*pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8 *)pDlConfigPdu) + (
	  sizeof(FAPI_dciDLPduInfo_st) - 1) + pDlConfigPdu->pduSize);*/
	pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8 *)pDlConfigPdu)
			+ pDlConfigPdu->pduSize);
	return pNextPdu;
}
#endif
/* SPR 19288 change end */
/*****************************************************************************
 * Function Name  :  addDciPduToDlConfig
 * Inputs         :  pDlConfigPdu - Pointer to message buffer to fill in the
 *                                  PDU,
 *                   pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message 
 *                   pDciCceInfo - Pointer to DciCCEInfo
 *                   internalCellIndex - cell id  
 * Outputs        :  None
 * Returns        :  Pointer to the buffer which can be used to fill in the
 *                   next PDU.
 * Description    :  This fucntion fills in the message with the DCI PDU data
 *                   and returns the pointer to the buffer which can be used to
 *                   fill in the next PDU.
 *****************************************************************************/
 FAPI_dlConfigPDUInfo_st *addDciPduToDlConfig(
		FAPI_dlConfigPDUInfo_st *pDlConfigPdu, DCIPduInfo *pDciPdu,
		DciCCEInfo *pDciCceInfo,InternalCellIndex internalCellIndex)
{
	FAPI_dlConfigPDUInfo_st     *pNextPdu = PNULL;
	pDlConfigPdu->pduType = FAPI_DCI_DL_PDU;
	pDlConfigPdu->dlConfigpduInfo.DCIPdu.cceIndex = pDciPdu->cceIndex;
	pDlConfigPdu->dlConfigpduInfo.DCIPdu.rnti = MAC_PHY_CONVERT_16(pDciPdu->rnti);
#if defined(FLEXRAN)
    fillDciPdu(pDlConfigPdu,(FAPI_dciDlPduInfo_st *) &pDlConfigPdu->dlConfigpduInfo.DCIPdu,pDciPdu, internalCellIndex,3);
#endif    
	switch (pDciPdu->dciFormat)
	{
		case DCI_FORMAT_1:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_1;
#if !defined(FLEXRAN)
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat1_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat1Pdu(((FAPI_dciFormat1_st *)
							&pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu,internalCellIndex);
#else
				fillDciFormat1Pdu((FAPI_dciDlPduInfo_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu),
						pDciPdu,internalCellIndex);
#endif
				break;
			}
		case DCI_FORMAT_1A:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_1A;
#if !defined(FLEXRAN)
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat1A_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat1APdu((FAPI_dciFormat1A_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu,pDciCceInfo->msgType,internalCellIndex);
#else
				fillDciFormat1APdu((FAPI_dciDlPduInfo_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu),
						pDciPdu,pDciCceInfo->msgType,internalCellIndex);

#endif
				/*In case DCI1A PDU is encodeed for PDCCH Order, explicitly
				 * reset CCE because DLSCH PDU will not be formed in this case.
				 * Also reset the msgType flag and premable Index*/
				if(PDCCH_ORDER_MSG == pDciCceInfo->msgType)
				{
					/* DL DATA SEP Changes */
					//mac_reset_cce(pDciPdu->aggregationLvl, pDciCceInfo);
					pDciCceInfo->msgType = BCCH_MSG;
					pDciPdu->preambleIdx = INVALID_PREAMBLE_IDX;
				}
				else if(SPS_DEACTIVATION ==  pDciCceInfo->msgType)
				{
					/* DL DATA SEP Changes */
					//mac_reset_cce(pDciPdu->aggregationLvl, pDciCceInfo);
				}
				break;
			}
		case DCI_FORMAT_1B:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_1B;
#if !defined(FLEXRAN)
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat1B_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat1BPdu((FAPI_dciFormat1B_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu
);
#else
				fillDciFormat1BPdu((FAPI_dciDlPduInfo_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu),
						pDciPdu);
#endif
				break;
			}
		case DCI_FORMAT_1C:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_1C;
#if !defined(FLEXRAN)
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat1C_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat1CPdu((FAPI_dciFormat1C_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu
);
#else
				fillDciFormat1CPdu((FAPI_dciDlPduInfo_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu),
						pDciPdu);
#endif
				break;
			}
		case DCI_FORMAT_1D:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_1D;
#if !defined(FLEXRAN)
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat1D_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat1DPdu((FAPI_dciFormat1D_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu
);
#else
				fillDciFormat1DPdu((FAPI_dciDlPduInfo_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu),
						pDciPdu);
#endif
				break;
			}
		case DCI_FORMAT_2:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_2;
#if !defined(FLEXRAN)
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat2_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat2Pdu((FAPI_dciFormat2_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu,internalCellIndex);
#else
				fillDciFormat2Pdu((FAPI_dciDlPduInfo_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu),
						pDciPdu,internalCellIndex);
#endif
				break;
			}
		case DCI_FORMAT_2A:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_2A;
#if !defined(FLEXRAN)
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat2A_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat2APdu((FAPI_dciFormat2A_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu, internalCellIndex);
#else
				fillDciFormat2APdu((FAPI_dciDlPduInfo_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu),
						pDciPdu,internalCellIndex);
#endif
				break;
			}
			/* + TM7_8 Changes Start */
		case DCI_FORMAT_2B:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat
					= FAPI_DL_DCI_FORMAT_2B;
#if !defined(FLEXRAN)
				pDlConfigPdu->pduSize = sizeof(FAPI_dciFormat2B_st) +
					(sizeof(FAPI_dciDLPduInfo_st) - 1*sizeof(uint8_t) );
				fillDciFormat2BPdu((FAPI_dciFormat2B_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciPdu[0]),
						pDciPdu,internalCellIndex);
#else
				fillDciFormat2BPdu((FAPI_dciDlPduInfo_st *)
						&(pDlConfigPdu->dlConfigpduInfo.DCIPdu),
						pDciPdu,internalCellIndex);
#endif
				break;
			}
			/* - TM7_8 Changes End */

		case DCI_FORMAT_0:
		case DCI_FORMAT_0_MSG3:
		case DCI_FORMAT_3:
		case DCI_FORMAT_3A:
		default:
			{
				pDlConfigPdu->dlConfigpduInfo.DCIPdu.dciFormat = 0;
				LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,
						"[%s] Exit Wrong DCI format %d, PDU dropped",
						__func__, pDciPdu->dciFormat);
				/* This PDU will be dropped, returning the current pointer */
				return pDlConfigPdu;
			}
	}
	/* pdu size should be a multiple of 4 bytes */
	/* SPR 1132 starts*/ 
#ifdef FLEXRAN
	pDlConfigPdu->pduSize = 38;
#else
	if ( pDlConfigPdu->pduSize % sizeof(UInt32) )
	{
		pDlConfigPdu->pduSize += sizeof(UInt32) - (pDlConfigPdu->pduSize % sizeof(UInt32));
	}
#endif
	/* SPR 1132 end */ 
	/*pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8 *)pDlConfigPdu) + (
	  sizeof(FAPI_dciDLPduInfo_st) - 1) + pDlConfigPdu->pduSize);*/
	pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8 *)pDlConfigPdu)
			+ pDlConfigPdu->pduSize);
	return pNextPdu;
}
/*****************************************************************************
 * Function Name  :  addBchPduToDlConfig
 * Inputs         :  pDlConfigPdu - Pointer to message buffer to fill in the
 *                                  PDU,
 *                   pDciCce - Pointer to DCI CCE containing the info to create
 *                             the PDU message 
 *                   pduIndex - Index of the Data
 *                   internalCellIndex - cell id  
 * Outputs        :  None
 * Returns        :  Pointer to the buffer which can be used to fill in the
 *                   next PDU
 * Description    :  This fucntion fills in the message with the BCH PDU data
 *                   and returns the pointer to the buffer which can be used to
 *                   fill in the next PDU.
 *****************************************************************************/
STATIC  FAPI_dlConfigPDUInfo_st *addBchPduToDlConfig(
		FAPI_dlConfigPDUInfo_st *pDlConfigPdu, DciCCEContainerInfo *pDciCce,
		UInt32  pduIndex,InternalCellIndex internalCellIndex)
{
	FAPI_dlConfigPDUInfo_st     *pNextPdu = PNULL;

	pDlConfigPdu->pduType = FAPI_BCH_PDU;

	pDlConfigPdu->pduSize
		= /*MAC_PHY_CONVERT_16( */(
					offsetof(FAPI_dlConfigPDUInfo_st, dlConfigpduInfo) +
					sizeof(FAPI_bchConfigPDUInfo_st) ) /*)*/;
	pDlConfigPdu->dlConfigpduInfo.BCHPdu.bchPduLen
		= MAC_PHY_CONVERT_16(pDciCce->mibDataLen);
	pDlConfigPdu->dlConfigpduInfo.BCHPdu.pduIndex
		= MAC_PHY_CONVERT_16(pduIndex);
	/* CLPC_CHG */
	pDlConfigPdu->dlConfigpduInfo.BCHPdu.txPower = 
		MAC_PHY_CONVERT_16(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p-> \
				commonDLPowerControlInfo.pbchTransmissionPower);
	/* CLPC_CHG END */
	/* pdu size should be a multiple of 4 bytes */
	/* SPR 1132 starts*/ 
	if ( pDlConfigPdu->pduSize % 4 )
	{
		pDlConfigPdu->pduSize += sizeof(UInt32) - \
					 (pDlConfigPdu->pduSize % sizeof(UInt32));
	}
	/* SPR 1132 end*/ 
	/*pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8 *)pDlConfigPdu)
	  + sizeof(FAPI_bchConfigPDUInfo_st) );*/
	pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8 *)pDlConfigPdu)
			+ pDlConfigPdu->pduSize);
	return pNextPdu;
}
#if defined (FAPI_4_0_COMPLIANCE) && !defined (FLEXRAN)
/*****************************************************************************
 * Function Name  : fillTransmissionMode 
 * Inputs         : transmissionMode_p , pDciPdu
 * Outputs        :None
 * Returns        :None 
 * Description    : This function is used to fill TRansmission mode in dlsch Pdu 
 *                  
 *                  
 *****************************************************************************/
void fillTransmissionMode(UInt8 *transmissionMode_p,DCIPduInfo *pDciPdu)
{
	*transmissionMode_p=pDciPdu->transmissionMode;
	return;
}
/*****************************************************************************
 * Function Name  : fillDLSCHPDUR9Params 
 * Inputs         : release9Param_p,pDciPdu  
 * Outputs        :None
 * Returns        :None
 * Description    :This function is used to fill Release9 Parameters in DLSCH PDU  
 *****************************************************************************/
void fillDLSCHPDUR9Params( FAPI_dlschRelease9Param_st * release9Param_p,DCIPduInfo *pDciPdu)
{
	/*FAPI changes start*/
	release9Param_p->nSCID=pDciPdu->scramblingId;
	/*FAPI changes end*/
	return;
}
/*****************************************************************************
 * Function Name  : fillDLSCHPDUR10Params 
 * Inputs         : release10Param_p  
 * Outputs        :None
 * Returns        :None  
 *                   
 * Description    : This fucntion fills Release10 Params in DLSCH PDU 
 *****************************************************************************/
void fillDLSCHPDUR10Params(FAPI_dlschRelease10Param_st *release10Param_p)
{
	/*FAPI2_1 changes start*/
	release10Param_p->csi_rs_flag=DEFAULT_CSIRS_FLAG;
	release10Param_p->csi_rs_ResourceConfigR10=DEFAULT_CSIRS_RES_CONFIG_REL10;
	release10Param_p->csi_rs_ZeroTxPowerResConfigBitmapR10=MAC_PHY_CONVERT_16(DEFAULT_CSIRS_ZEROTXPOWER_BITMAP);
	/*FAPI2_1 changes end*/
	return;
}
/*****************************************************************************
 * Function Name  :  addDlschPduToDlConfigR8
 * Inputs         :  pDlConfigPdu - Pointer to message buffer to fill in the
 *                                  PDU.
 *                :  pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message.
 *                   pduIndex, tbIndex, isOnlyOneTb, *tempVendorSpecificForDLConfig_p,
 *                   internalCellIndex - cell id  
 * Outputs        :
 * Returns        :  Pointer to the buffer which can be used to fill in the
 *                   next PDU
 * Description    :  This fucntion fills in the message with the DLSCH PDU data
 *                   and returns the pointer to the buffer which can be used to
 *                   fill in the next PDU.This Function will be used in case PHY 
 *                   Supports Release8.
 *****************************************************************************/
 FAPI_dlConfigPDUInfo_st *addDlschPduToDlConfigR8(
		FAPI_dlConfigPDUInfo_st *pDlConfigPdu, DCIPduInfo *pDciPdu,
		UInt32 pduIndex, UInt8 tbIndex
		/* SPR 5807 Fix */       
		//#ifdef TDD_CONFIG
		,UInt8 isOnlyOneTb 
		//#endif 
		/* SPR 5807 Fix */       
		,FAPI_VendorSpecificForDLConfig_st *tempVendorSpecificForDLConfig_p, 
		UInt16 *numOfUEs,InternalCellIndex internalCellIndex)

{
	FAPI_dlConfigPDUInfo_st     *pNextPdu = PNULL;
	/* + TM7_8 Changes Start */
	UInt8 count = 0;
	UInt8 counter = 0;
	FAPI_beamFormingVectorInfo_st     *bfVector = PNULL;
	UInt16 *bfValue_per_antenna = PNULL;
	FAPI_PdschEpreToUeRsRatio_st  *tempPdschEpreToUeRsRatio_p = PNULL;
	/* - TM7_8 Changes End */


	pDlConfigPdu->pduType = FAPI_DLSCH_PDU;

	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.pduIndex = MAC_PHY_CONVERT_16((UInt16)pduIndex);
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.rnti = MAC_PHY_CONVERT_16(pDciPdu->rnti);
	/* SPR# 892 Fix start */
	if ( RA_POLICY_2_LOCALIZED <= pDciPdu->resrcAllocType )
	{
            /* SPR 21993 Fix Start */
#ifdef FAPI_4_0_COMPLIANCE
            if(pDciPdu->dciFormat == DCI_FORMAT_1C)
            {
	        /* RAT policy RAT Type2 value is 3  for 1C */
                pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.resAllocationType
                    =  FAPI_RA_POLICY_2_1C;
            }
            else
            {
                /* RAT policy RAT Type2 value is 2  for 1A,1B,1D */
                pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.resAllocationType
                    = FAPI_RA_POLICY_2 ;
            }
#else
            pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.resAllocationType
	        = FAPI_RA_POLICY_2 ;
#endif
            /* SPR 21993 Fix End */
	}
	else
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.resAllocationType
			= pDciPdu->resrcAllocType;
	}

	/* SPR# 892 Fix End */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.vRBAssignmentFlag
		= pDciPdu->virtulResrcBlkType;
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.rbCoding
		= MAC_PHY_CONVERT_32(pDciPdu->resrcAssgmntBitMap);
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transportBlocks
		= tbIndex;
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.tbToCodeWordSwapFlag
		= pDciPdu->tbSwapFlag;
	if ( TB_2_INDEX == tbIndex)
	{
		/* SPR 2422 Changes Start */
		/* In FAPI DLSch pdu mcs parameter is actually the modulation scheme.
		 * So filling the modulation scheme in the same */

		/* Cyclomatic_complexity_changes_start */
		getModSchemeFromMcsIndex(pDciPdu->tb2Info.modulationType,
				&(pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.mcs));
		/* Cyclomatic_complexity_changes_end */

		/* SPR 2422 Changes End */
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.redundancyVersion
			= pDciPdu->tb2Info.redundancyVer;
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.dlschPduLen
			= MAC_PHY_CONVERT_16(pDciPdu->tb2Info.tbSize);
	}
	else
	{
		/* SPR 2422 Changes Start */
		/* In FAPI DLSch pdu mcs parameter is actually the modulation scheme.
		 * So filling the modulation scheme in the same */

		/* Cyclomatic_complexity_changes_start */
		getModSchemeFromMcsIndex(pDciPdu->tb1Info.modulationType,
				&(pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.mcs));
		/* Cyclomatic_complexity_changes_end */

		/* SPR 2422 Changes End */
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.redundancyVersion
			= pDciPdu->tb1Info.redundancyVer;
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.dlschPduLen
			=MAC_PHY_CONVERT_16(pDciPdu->tb1Info.tbSize);
	}

	/** SI_MSG_MCS FIX START **/

	/* CMAS_CODE START */
	if(FAPI_RA_RNTI_P_RNTI_SI_RNTI == getRntiTypeForRnti(pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.rnti,internalCellIndex))
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.mcs= QPSK;
	}
	/* CMAS_CODE END */

	/** SI_MSG_MCS FIX END **/

	/* + CQI_5.0 */

	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transmissionScheme = 
		pDciPdu->transmissionScheme; 
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfLayers = pDciPdu->numLayer;


	if ( TX_MODE_3 == pDciPdu->transmissionMode )  /* 3053 */
	{
		/* SPR 5807 Fix start */
		//#ifdef TDD_CONFIG
		if (!isOnlyOneTb ) 
		{
			pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transmissionScheme =
				FAPI_TX_DIVERSITY;
			pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfLayers = 
				cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas;
		}
		//#endif
		/* SPR 5807 Fix end */
	} /* 3053 */

	/* SPR 11403 fix start */ 
	/* code snippet is commented, as in case of TM4, when 
	   only 1 TB is valid, tx_scheme is being changed into TX_DIVERSITY, while precoding index
	   is not set accordingly. */
	/* SPR 11403 fix end */ 
	/* +  SPR 1476 , 1487*/
#ifdef PERF_STATS

	UPDATE_UE_TX_MODE_RI_STATS(pDciPdu->ueIndex,
			pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transmissionScheme);
#endif
	/*numOfSubBand needs to be filled only for TX_MODE_3,4,5. Otherwise 
	 *it should be 0
	 */
	fillTransmissionMode(&pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transmissionMode,pDciPdu);
	/*SPR 13698 changes start*/ 
	if(((pDciPdu->transmissionMode == TX_MODE_7) && (DCI_FORMAT_1 == pDciPdu->dciFormat)) ||
			((pDciPdu->transmissionMode == TX_MODE_8) && (DCI_FORMAT_2B == pDciPdu->dciFormat)))
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector = 1;
	}
	else
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector = 0;
	}
	/*SPR 13698 changes end*/ 
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfSubBand = 0;


	/* 4x4 DL MIMO CHG START */  
	if ( (TX_MODE_4 == pDciPdu->transmissionMode) || 
			(TX_MODE_3 == pDciPdu->transmissionMode))  
	{
		/*SPR 13698 changes start*/ 
		fillCodebookInfoInDlschPdu(pDlConfigPdu, pDciPdu, internalCellIndex);
		/*SPR 13698 changes end*/ 

	}
	/* + TM6_5.2 */
	else if(TX_MODE_6 == pDciPdu->transmissionMode)
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfSubBand
			= 1; /* only wideband supported - for now hard coded 1 */
		/*SPR 13698 changes start*/ 
		fillCodebookInfoInDlschPduForTM6(pDlConfigPdu,pDciPdu,internalCellIndex);
		/*SPR 13698 changes end*/ 
	}
	/* - TM6_5.2 */
	/* - CQI_5.0 */
	/* -  SPR 1476 , 1487*/
	/* SPR# 903 Fix start */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.ueCatagoryCapacity
		=  pDciPdu->ueCategory;
	/* SPR 3377 */
	if (0 == pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.ueCatagoryCapacity)
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.ueCatagoryCapacity = 5;
	}
/*SPR 21001 Changes end */
	/* SPR 3377 */
	/* SPR# 903 Fix end */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.pA = pDciPdu->pA;
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.deltaPowerOffsetAIndex
		= DEFAULT_DELTA_POWER_OFFSET_INDEX;
	/* CMAS_CODE START */
	//pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nGap = 0;
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nGap = pDciPdu->nGAP;
	/* CMAS_CODE END */
	/* SPR 1492 changes start */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nPRB = pDciPdu->nPRB;
	/* SPR 1492 changes end */
	/* + TM7_8 Changes Start */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numRbPerSubBand =  
		cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->dlAvailableRBs;
#ifdef FAPI_DEVIATIONS    
#ifndef FAPI_4_0_COMPLIANCE
	if( DCI_FORMAT_2B == pDciPdu->dciFormat)
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nScId = pDciPdu->scramblingId;
	}
	else
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nScId = 0;
	}
#endif
#endif
	/* + TM7_EPRE */
	if((TX_MODE_7 == pDciPdu->transmissionMode)&&
			(DCI_FORMAT_1 == pDciPdu->dciFormat)&&
			(QPSK == pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.mcs))
	{
		tempPdschEpreToUeRsRatio_p = 
			&tempVendorSpecificForDLConfig_p->pdschEpreToUeRsRatioInfo[*numOfUEs]; 
		tempPdschEpreToUeRsRatio_p->rnti = pDciPdu->rnti; 
		tempPdschEpreToUeRsRatio_p->pdschEpreToUeRsRatio = pDciPdu->pdschEpreToUeRsRatio;
		(*numOfUEs)++;
		tempVendorSpecificForDLConfig_p->numOfUEs = *numOfUEs;
	}
	/* - TM7_EPRE */
/*SPR 21001 Changes start */
	bfVector = &pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.bfVector[0];
	UInt16 size = 0;
	for(count =0; count < pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector; count++)
	{
		bfVector->subBandIndex = (pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector -1);
		bfVector->numOfAntenna = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas;
		size = sizeof(FAPI_beamFormingVectorInfo_st);

		for(counter = 0; counter < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas; counter++)
		{
			size += sizeof(UInt16);
			bfValue_per_antenna = (UInt16 *)(&bfVector->bfValue_per_antenna[counter]);
			*bfValue_per_antenna = pDciPdu->bfValue[counter];
		}
		size -= sizeof(UInt16);
		bfVector = (FAPI_beamFormingVectorInfo_st *)((UInt8 *)bfVector + size);
	}
/*SPR 21001 Changes end */

	/* - TM7_8 Changes End */

	fillDLSCHPDUR9Params(&pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.release9Param,pDciPdu); 
	fillDLSCHPDUR10Params(&pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.release10Param);



	/* Now fill in the PDU size */
/*SPR 21001 Changes start */
	/* + TM7_8 Changes Start */
	pDlConfigPdu->pduSize = MAC_PHY_CONVERT_16( (
				offsetof(FAPI_dlConfigPDUInfo_st, dlConfigpduInfo) + 
				(offsetof(FAPI_dlSCHConfigPDUInfo_st, subBandInfo) )
				+ (sizeof(uint8_t) * pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfSubBand)
				+ (sizeof(FAPI_beamFormingVectorInfo_st) * 
					(pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector)) +
				((pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector) *
				 (sizeof(uint16_t)*(cellSpecificParams_g.\
						    cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
						    numOfTxAnteenas - 1)))));
/*SPR 21001 Changes end */

	/* - TM7_8 Changes End */
	/* pdu size should be a multiple of 4 bytes */
	/* SPR 1132 starts */ 
	if ( pDlConfigPdu->pduSize % sizeof(UInt32) )
	{
		pDlConfigPdu->pduSize += (sizeof(UInt32)) - (pDlConfigPdu->pduSize % sizeof(UInt32));
	}
	/* SPR 1132 ends */ 
	/* No data in pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.bfVector */
	/*pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8 *)pDlConfigPdu)
	  + pDlConfigPdu->pduSize);*/
	/* Logging for PDSCH Power */
	LOG_MAC_MSG(MAC_PDSCH_POWER_ID,LOGDEBUG,MAC_DL_PC,\
			getCurrentTick(),\
			pDciPdu->ueIndex,pDciPdu->pA,pDciPdu->ueCategory,0,0,\
			0.0,0.0,__func__,"DL Total Transmission Power for PDSCH");
	pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8*)pDlConfigPdu) 
			+ pDlConfigPdu->pduSize);
	return pNextPdu;
}
/*****************************************************************************
 * Function Name  :  addDlschPduToDlConfigR9
 * Inputs         :  pDlConfigPdu - Pointer to message buffer to fill in the
 *                                  PDU.
 *                :  pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message.
 *                   pduIndex, tbIndex, isOnlyOneTb, *tempVendorSpecificForDLConfig_p,
 *                   internalCellIndex - cell id  
 * Outputs        :
 * Returns        :  Pointer to the buffer which can be used to fill in the
 *                   next PDU
 * Description    :  This fucntion fills in the message with the DLSCH PDU data
 *                   and returns the pointer to the buffer which can be used to
 *                   fill in the next PDU.This Function will be used in case PHY 
 *                   Supports Release9.
 *****************************************************************************/
 FAPI_dlConfigPDUInfo_st *addDlschPduToDlConfigR9(
		FAPI_dlConfigPDUInfo_st *pDlConfigPdu, DCIPduInfo *pDciPdu,
		UInt32 pduIndex, UInt8 tbIndex
		/* SPR 5807 Fix */       
		//#ifdef TDD_CONFIG
		,UInt8 isOnlyOneTb 
		//#endif 
		/* SPR 5807 Fix */       
		,FAPI_VendorSpecificForDLConfig_st *tempVendorSpecificForDLConfig_p, 
		UInt16 *numOfUEs,InternalCellIndex internalCellIndex)
{
	FAPI_dlConfigPDUInfo_st     *pNextPdu = PNULL;
	/* + TM7_8 Changes Start */
	UInt8 count = 0;
	UInt8 counter = 0;
	FAPI_beamFormingVectorInfo_st     *bfVector = PNULL;
	UInt16 *bfValue_per_antenna = PNULL;
	FAPI_PdschEpreToUeRsRatio_st  *tempPdschEpreToUeRsRatio_p = PNULL;
	/* - TM7_8 Changes End */


	pDlConfigPdu->pduType = FAPI_DLSCH_PDU;

	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.pduIndex = MAC_PHY_CONVERT_16((UInt16)pduIndex);
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.rnti = MAC_PHY_CONVERT_16(pDciPdu->rnti);
	/* SPR# 892 Fix start */
	if ( RA_POLICY_2_LOCALIZED <= pDciPdu->resrcAllocType )
	{
	
            /* SPR 21993 Fix Start */
            if(pDciPdu->dciFormat == DCI_FORMAT_1C)
            {
                /* RAT policy RAT Type2 value is 3  for 1C */
                pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.resAllocationType
                    =  FAPI_RA_POLICY_2_1C;
            }
            else
            {
		/* RAT policy RAT Type2 value is 2  for 1A,1B,1D */
                pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.resAllocationType
                    = FAPI_RA_POLICY_2 ;
            }
            /* SPR 21993 Fix End */
	}
	else
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.resAllocationType
			= pDciPdu->resrcAllocType;
	}
	/* SPR# 892 Fix End */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.vRBAssignmentFlag
		= pDciPdu->virtulResrcBlkType;
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.rbCoding
		= MAC_PHY_CONVERT_32(pDciPdu->resrcAssgmntBitMap);
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transportBlocks
		= tbIndex;
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.tbToCodeWordSwapFlag
		= pDciPdu->tbSwapFlag;
	if ( TB_2_INDEX == tbIndex)
	{
		/* SPR 2422 Changes Start */
		/* In FAPI DLSch pdu mcs parameter is actually the modulation scheme.
		 * So filling the modulation scheme in the same */

		/* Cyclomatic_complexity_changes_start */
		getModSchemeFromMcsIndex(pDciPdu->tb2Info.modulationType,
				&(pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.mcs));
		/* Cyclomatic_complexity_changes_end */

		/* SPR 2422 Changes End */
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.redundancyVersion
			= pDciPdu->tb2Info.redundancyVer;
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.dlschPduLen
			= MAC_PHY_CONVERT_16(pDciPdu->tb2Info.tbSize);
	}
	else
	{
		/* SPR 2422 Changes Start */
		/* In FAPI DLSch pdu mcs parameter is actually the modulation scheme.
		 * So filling the modulation scheme in the same */

		/* Cyclomatic_complexity_changes_start */
		getModSchemeFromMcsIndex(pDciPdu->tb1Info.modulationType,
				&(pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.mcs));
		/* Cyclomatic_complexity_changes_end */

		/* SPR 2422 Changes End */
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.redundancyVersion
			= pDciPdu->tb1Info.redundancyVer;
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.dlschPduLen
			=MAC_PHY_CONVERT_16(pDciPdu->tb1Info.tbSize);
	}

	/** SI_MSG_MCS FIX START **/

	/* CMAS_CODE START */
	if(FAPI_RA_RNTI_P_RNTI_SI_RNTI == getRntiTypeForRnti(pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.rnti,internalCellIndex))
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.mcs= QPSK;
	}
	/* CMAS_CODE END */

	/** SI_MSG_MCS FIX END **/

	/* + CQI_5.0 */

	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transmissionScheme = 
		pDciPdu->transmissionScheme; 
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfLayers = pDciPdu->numLayer;

	if ( TX_MODE_3 == pDciPdu->transmissionMode )  /* 3053 */
	{
		/* SPR 5807 Fix start */
		//#ifdef TDD_CONFIG
		if (!isOnlyOneTb ) 
		{
			pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transmissionScheme =
				FAPI_TX_DIVERSITY;
			pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfLayers = 
				cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas;
		}
		//#endif
		/* SPR 5807 Fix end */
	} /* 3053 */

	/* SPR 11403 fix start */ 
	/* code snippet is commented, as in case of TM4, when 
	   only 1 TB is valid, tx_scheme is being changed into TX_DIVERSITY, while precoding index
	   is not set accordingly. */
	/* SPR 11403 fix end */ 
	/* +  SPR 1476 , 1487*/
#ifdef PERF_STATS

	UPDATE_UE_TX_MODE_RI_STATS(pDciPdu->ueIndex,
			pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transmissionScheme);
#endif
	/*numOfSubBand needs to be filled only for TX_MODE_3,4,5. Otherwise 
	 *it should be 0
	 */
	fillTransmissionMode(&pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transmissionMode,pDciPdu);
	/*SPR 13698 changes start*/ 
	if(((pDciPdu->transmissionMode == TX_MODE_7) && (DCI_FORMAT_1 == pDciPdu->dciFormat)) ||
			((pDciPdu->transmissionMode == TX_MODE_8) && (DCI_FORMAT_2B == pDciPdu->dciFormat)))
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector = 1;
	}
	else
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector = 0;
	}
	/*SPR 13698 changes end*/ 
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfSubBand = 0;


	/* 4x4 DL MIMO CHG START */  
	if ( (TX_MODE_4 == pDciPdu->transmissionMode) || 
			(TX_MODE_3 == pDciPdu->transmissionMode))  
	{
		/*SPR 13698 changes start*/ 
		fillCodebookInfoInDlschPdu(pDlConfigPdu, pDciPdu,internalCellIndex);        
		/*SPR 13698 changes end*/ 
	}
	/* + TM6_5.2 */
	else if(TX_MODE_6 == pDciPdu->transmissionMode)
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfSubBand
			= 1; /* only wideband supported - for now hard coded 1 */
		/*SPR 13698 changes start*/ 
		fillCodebookInfoInDlschPduForTM6(pDlConfigPdu,pDciPdu,internalCellIndex);
		/*SPR 13698 changes end*/ 
	}
	/* - TM6_5.2 */
	/* - CQI_5.0 */
	/* -  SPR 1476 , 1487*/
	/* SPR# 903 Fix start */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.ueCatagoryCapacity
		=  pDciPdu->ueCategory;
	/* SPR 3377 */
	if (0 == pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.ueCatagoryCapacity)
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.ueCatagoryCapacity = 5;
	}
	/* SPR 3377 */
	/* SPR# 903 Fix end */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.pA = pDciPdu->pA;
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.deltaPowerOffsetAIndex
		= DEFAULT_DELTA_POWER_OFFSET_INDEX;
	/* CMAS_CODE START */
	//pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nGap = 0;
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nGap = pDciPdu->nGAP;
	/* CMAS_CODE END */
	/* SPR 1492 changes start */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nPRB = pDciPdu->nPRB;
	/* SPR 1492 changes end */
	/* + TM7_8 Changes Start */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numRbPerSubBand =  
		cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->dlAvailableRBs;
#ifdef FAPI_DEVIATIONS    
#ifndef FAPI_4_0_COMPLIANCE
	if( DCI_FORMAT_2B == pDciPdu->dciFormat)
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nScId = pDciPdu->scramblingId;
	}
	else
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nScId = 0;
	}
#endif
#endif
	/* + TM7_EPRE */
	if((TX_MODE_7 == pDciPdu->transmissionMode)&&
			(DCI_FORMAT_1 == pDciPdu->dciFormat)&&
			(QPSK == pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.mcs))
	{
		tempPdschEpreToUeRsRatio_p = 
			&tempVendorSpecificForDLConfig_p->pdschEpreToUeRsRatioInfo[*numOfUEs]; 
		tempPdschEpreToUeRsRatio_p->rnti = pDciPdu->rnti; 
		tempPdschEpreToUeRsRatio_p->pdschEpreToUeRsRatio = pDciPdu->pdschEpreToUeRsRatio;
		(*numOfUEs)++;
		tempVendorSpecificForDLConfig_p->numOfUEs = *numOfUEs;
	}
	/* - TM7_EPRE */

	bfVector = &pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.bfVector[0];
	UInt16 size = 0;
	for(count =0; count < pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector; count++)
	{
		bfVector->subBandIndex = (pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector -1);
		bfVector->numOfAntenna = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas;
		size = sizeof(FAPI_beamFormingVectorInfo_st);

		for(counter = 0; counter < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas; counter++)
		{
			size += sizeof(UInt16);
			bfValue_per_antenna = (UInt16 *)(&bfVector->bfValue_per_antenna[counter]);
			*bfValue_per_antenna = pDciPdu->bfValue[counter];
		}
		size -= sizeof(UInt16);
		bfVector = (FAPI_beamFormingVectorInfo_st *)((UInt8 *)bfVector + size);
	}

	/* - TM7_8 Changes End */

	fillDLSCHPDUR9Params(&pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.release9Param,pDciPdu); 
	fillDLSCHPDUR10Params(&pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.release10Param);



	/* Now fill in the PDU size */


	/* + TM7_8 Changes Start */
	pDlConfigPdu->pduSize = MAC_PHY_CONVERT_16( (
				offsetof(FAPI_dlConfigPDUInfo_st, dlConfigpduInfo) + 
				(offsetof(FAPI_dlSCHConfigPDUInfo_st, subBandInfo) )
				+ (sizeof(uint8_t) * pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfSubBand)
				+ (sizeof(FAPI_beamFormingVectorInfo_st) * 
					(pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector)) +
				((pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector) *
				 (sizeof(uint16_t)*(cellSpecificParams_g.\
						    cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
						    numOfTxAnteenas - 1)))));

	/* - TM7_8 Changes End */
	/* pdu size should be a multiple of 4 bytes */
	/* SPR 1132 starts */ 
	if ( pDlConfigPdu->pduSize % sizeof(UInt32) )
	{
		pDlConfigPdu->pduSize += (sizeof(UInt32)) - (pDlConfigPdu->pduSize % sizeof(UInt32));
	}
	/* SPR 1132 ends */ 
	/* No data in pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.bfVector */
	/*pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8 *)pDlConfigPdu)
	  + pDlConfigPdu->pduSize);*/
	/* Logging for PDSCH Power */
	LOG_MAC_MSG(MAC_PDSCH_POWER_ID,LOGDEBUG,MAC_DL_PC,\
			getCurrentTick(),\
			pDciPdu->ueIndex,pDciPdu->pA,pDciPdu->ueCategory,0,0,\
			0.0,0.0,__func__,"DL Total Transmission Power for PDSCH");

	pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8*)pDlConfigPdu) 
			+ pDlConfigPdu->pduSize);
	return pNextPdu;
}
/*****************************************************************************
 * Function Name  :  addDlschPduToDlConfigR10
 * Inputs         :  pDlConfigPdu - Pointer to message buffer to fill in the
 *                                  PDU.
 *                :  pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message.
 *                   pduIndex,tbIndex,isOnlyOneTb,*tempVendorSpecificForDLConfig_p
 *                   internalCellIndex - cell id  
 * Outputs        :
 * Returns        :  Pointer to the buffer which can be used to fill in the
 *                   next PDU
 * Description    :  This fucntion fills in the message with the DLSCH PDU data
 *                   and returns the pointer to the buffer which can be used to
 *                   fill in the next PDU.This Function will be used in case PHY 
 *                   Supports Release10.
 *****************************************************************************/
 FAPI_dlConfigPDUInfo_st *addDlschPduToDlConfigR10(
		FAPI_dlConfigPDUInfo_st *pDlConfigPdu, DCIPduInfo *pDciPdu,
		UInt32 pduIndex, UInt8 tbIndex
		/* SPR 5807 Fix */       
		//#ifdef TDD_CONFIG
		,UInt8 isOnlyOneTb 
		//#endif 
		/* SPR 5807 Fix */       
		,FAPI_VendorSpecificForDLConfig_st *tempVendorSpecificForDLConfig_p, 
		UInt16 *numOfUEs,InternalCellIndex internalCellIndex)
{
	FAPI_dlConfigPDUInfo_st     *pNextPdu = PNULL;
	/* + TM7_8 Changes Start */
	UInt8 count = 0;
	UInt8 counter = 0;
	FAPI_beamFormingVectorInfo_st     *bfVector = PNULL;
	UInt16 *bfValue_per_antenna = PNULL;
	FAPI_PdschEpreToUeRsRatio_st  *tempPdschEpreToUeRsRatio_p = PNULL;
	/* - TM7_8 Changes End */


	pDlConfigPdu->pduType = FAPI_DLSCH_PDU;

	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.pduIndex = MAC_PHY_CONVERT_16((UInt16)pduIndex);
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.rnti = MAC_PHY_CONVERT_16(pDciPdu->rnti);
	/* SPR# 892 Fix start */
	if ( RA_POLICY_2_LOCALIZED <= pDciPdu->resrcAllocType )
	{
            /* SPR 21993 Fix Start */
            if(pDciPdu->dciFormat == DCI_FORMAT_1C)
            {
	        /* RAT policy RAT Type2 value is 3  for 1C */
                pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.resAllocationType
                    =  FAPI_RA_POLICY_2_1C;
            }
            else
            {
	        /* RAT policy RAT Type2 value is 2  for 1A,1B,1D */
                pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.resAllocationType
                    = FAPI_RA_POLICY_2 ;
            }
            /* SPR 21993 Fix End */	
	}
	else
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.resAllocationType
			= pDciPdu->resrcAllocType;
	}
	/* SPR# 892 Fix End */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.vRBAssignmentFlag
		= pDciPdu->virtulResrcBlkType;
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.rbCoding
		= MAC_PHY_CONVERT_32(pDciPdu->resrcAssgmntBitMap);
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transportBlocks
		= tbIndex;
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.tbToCodeWordSwapFlag
		= pDciPdu->tbSwapFlag;
	if ( TB_2_INDEX == tbIndex)
	{
		/* SPR 2422 Changes Start */
		/* In FAPI DLSch pdu mcs parameter is actually the modulation scheme.
		 * So filling the modulation scheme in the same */

		/* Cyclomatic_complexity_changes_start */
		getModSchemeFromMcsIndex(pDciPdu->tb2Info.modulationType,
				&(pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.mcs));
		/* Cyclomatic_complexity_changes_end */

		/* SPR 2422 Changes End */
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.redundancyVersion
			= pDciPdu->tb2Info.redundancyVer;
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.dlschPduLen
			= MAC_PHY_CONVERT_16(pDciPdu->tb2Info.tbSize);
	}
	else
	{
		/* SPR 2422 Changes Start */
		/* In FAPI DLSch pdu mcs parameter is actually the modulation scheme.
		 * So filling the modulation scheme in the same */

		/* Cyclomatic_complexity_changes_start */
		getModSchemeFromMcsIndex(pDciPdu->tb1Info.modulationType,
				&(pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.mcs));
		/* Cyclomatic_complexity_changes_end */

		/* SPR 2422 Changes End */
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.redundancyVersion
			= pDciPdu->tb1Info.redundancyVer;
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.dlschPduLen
			=MAC_PHY_CONVERT_16(pDciPdu->tb1Info.tbSize);
	}

	/** SI_MSG_MCS FIX START **/

	/* CMAS_CODE START */
	if(FAPI_RA_RNTI_P_RNTI_SI_RNTI == getRntiTypeForRnti(pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.rnti,internalCellIndex))
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.mcs= QPSK;
	}
	/* CMAS_CODE END */

	/** SI_MSG_MCS FIX END **/

	/* + CQI_5.0 */

	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transmissionScheme = 
		pDciPdu->transmissionScheme; 
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfLayers = pDciPdu->numLayer;
	if ( TX_MODE_3 == pDciPdu->transmissionMode )  /* 3053 */
	{
		/* SPR 5807 Fix start */
		//#ifdef TDD_CONFIG
		if (!isOnlyOneTb ) 
		{
			pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transmissionScheme =
				FAPI_TX_DIVERSITY;
			pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfLayers = 
				cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas;
		}
		//#endif
		/* SPR 5807 Fix end */
	} /* 3053 */

	/* SPR 11403 fix start */
	/* code snippet is commented, as in case of TM4, when 
	   only 1 TB is valid, tx_scheme is being changed into TX_DIVERSITY, while precoding index
	   is not set accordingly. */
	/* SPR 11403 fix end */
	/* +  SPR 1476 , 1487*/
#ifdef PERF_STATS

	UPDATE_UE_TX_MODE_RI_STATS(pDciPdu->ueIndex,
			pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transmissionScheme);
#endif
	/*numOfSubBand needs to be filled only for TX_MODE_3,4,5. Otherwise 
	 *it should be 0
	 */
	fillTransmissionMode(&pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transmissionMode,pDciPdu);
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfSubBand = 0;

	/*SPR 13698 changes start */
	if(((pDciPdu->transmissionMode == TX_MODE_7) && (DCI_FORMAT_1 == pDciPdu->dciFormat)) ||
			((pDciPdu->transmissionMode == TX_MODE_8) && (DCI_FORMAT_2B == pDciPdu->dciFormat)))
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector = 1;
	}
	else
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector = 0;
	}
	/*SPR 13698 changes end */

	/* 4x4 DL MIMO CHG START */  
	if ( (TX_MODE_4 == pDciPdu->transmissionMode) || 
			(TX_MODE_3 == pDciPdu->transmissionMode))  
	{
		/*SPR 13698 changes start*/ 
		fillCodebookInfoInDlschPdu(pDlConfigPdu, pDciPdu,internalCellIndex);        
		/*SPR 13698 changes end*/ 
	}
	/* + TM6_5.2 */
	else if(TX_MODE_6 == pDciPdu->transmissionMode)
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfSubBand
			= 1; /* only wideband supported - for now hard coded 1 */
		/*SPR 13698 changes start*/ 
		fillCodebookInfoInDlschPduForTM6(pDlConfigPdu,pDciPdu,internalCellIndex);
		/*SPR 13698 changes end*/ 
	}
	/* - TM6_5.2 */
	/* - CQI_5.0 */
	/* -  SPR 1476 , 1487*/
	/* SPR# 903 Fix start */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.ueCatagoryCapacity
		=  pDciPdu->ueCategory;
	/* SPR 3377 */
	if (0 == pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.ueCatagoryCapacity)
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.ueCatagoryCapacity = 5;
	}
/*SPR 21001 Changes end */
	/* SPR 3377 */
	/* SPR# 903 Fix end */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.pA = pDciPdu->pA;
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.deltaPowerOffsetAIndex
		= DEFAULT_DELTA_POWER_OFFSET_INDEX;
	/* CMAS_CODE START */
	//pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nGap = 0;
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nGap = pDciPdu->nGAP;
	/* CMAS_CODE END */
	/* SPR 1492 changes start */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nPRB = pDciPdu->nPRB;
	/* SPR 1492 changes end */
	/* + TM7_8 Changes Start */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numRbPerSubBand =  
		cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->dlAvailableRBs;
#ifdef FAPI_DEVIATIONS    
#ifndef FAPI_4_0_COMPLIANCE
	if( DCI_FORMAT_2B == pDciPdu->dciFormat)
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nScId = pDciPdu->scramblingId;
	}
	else
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nScId = 0;
	}
#endif
#endif
	/* + TM7_EPRE */
	if((TX_MODE_7 == pDciPdu->transmissionMode)&&
			(DCI_FORMAT_1 == pDciPdu->dciFormat)&&
			(QPSK == pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.mcs))
	{
		tempPdschEpreToUeRsRatio_p = 
			&tempVendorSpecificForDLConfig_p->pdschEpreToUeRsRatioInfo[*numOfUEs]; 
		tempPdschEpreToUeRsRatio_p->rnti = pDciPdu->rnti; 
		tempPdschEpreToUeRsRatio_p->pdschEpreToUeRsRatio = pDciPdu->pdschEpreToUeRsRatio;
		(*numOfUEs)++;
		tempVendorSpecificForDLConfig_p->numOfUEs = *numOfUEs;
	}
	/* - TM7_EPRE */
/*SPR 21001 Changes start */
	bfVector = &pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.bfVector[0];
	UInt16 size = 0;
	for(count =0; count < pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector; count++)
	{
		bfVector->subBandIndex = (pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector -1);
		bfVector->numOfAntenna = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas;
		size = sizeof(FAPI_beamFormingVectorInfo_st);

		for(counter = 0; counter < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas; counter++)
		{
			size += sizeof(UInt16);
			bfValue_per_antenna = (UInt16 *)(&bfVector->bfValue_per_antenna[counter]);
			*bfValue_per_antenna = pDciPdu->bfValue[counter];
		}
		size -= sizeof(UInt16);
		bfVector = (FAPI_beamFormingVectorInfo_st *)((UInt8 *)bfVector + size);
	}
/*SPR 21001 Changes end */
	/* - TM7_8 Changes End */

	fillDLSCHPDUR9Params(&pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.release9Param,pDciPdu); 
	fillDLSCHPDUR10Params(&pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.release10Param);




	/* Now fill in the PDU size */


	/* + TM7_8 Changes Start */
/*SPR 21001 Changes start */
	pDlConfigPdu->pduSize = MAC_PHY_CONVERT_16( (
				offsetof(FAPI_dlConfigPDUInfo_st, dlConfigpduInfo) + 
				(offsetof(FAPI_dlSCHConfigPDUInfo_st, subBandInfo) )
				+ (sizeof(uint8_t) * pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfSubBand)
				+ (sizeof(FAPI_beamFormingVectorInfo_st) * 
					(pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector)) +
				((pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector) *
				 (sizeof(uint16_t)*(cellSpecificParams_g.\
						    cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
						    numOfTxAnteenas - 1)))));
/*SPR 21001 Changes end */

	/* - TM7_8 Changes End */
	/* pdu size should be a multiple of 4 bytes */
	/* SPR 1132 starts */ 
	if ( pDlConfigPdu->pduSize % sizeof(UInt32) )
	{
		pDlConfigPdu->pduSize += (sizeof(UInt32)) - (pDlConfigPdu->pduSize % sizeof(UInt32));
	}
	/* SPR 1132 ends */ 
	/* No data in pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.bfVector */
	/*pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8 *)pDlConfigPdu)
	  + pDlConfigPdu->pduSize);*/
	/* Logging for PDSCH Power */
	LOG_MAC_MSG(MAC_PDSCH_POWER_ID,LOGDEBUG,MAC_DL_PC,\
			getCurrentTick(),\
			pDciPdu->ueIndex,pDciPdu->pA,pDciPdu->ueCategory,0,0,\
			0.0,0.0,__func__,"DL Total Transmission Power for PDSCH");
	pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8*)pDlConfigPdu) 
			+ pDlConfigPdu->pduSize);
	return pNextPdu;
}
#endif	
/*****************************************************************************
 * Function Name  :  addDlschPduToDlConfig
 * Inputs         :  pDlConfigPdu - Pointer to message buffer to fill in the
 *                                  PDU,
 *                   pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message,
 *                   pduIndex,tbIndex,isOnlyOneTb,*tempVendorSpecificForDLConfig_p
 *                   internalCellIndex - cell id  
 * Outputs        :  None
 * Returns        :  Pointer to the buffer which can be used to fill in the
 *                   next PDU
 * Description    :  This fucntion fills in the message with the DLSCH PDU data
 *                   and returns the pointer to the buffer which can be used to
 *                   fill in the next PDU.
 *****************************************************************************/
 FAPI_dlConfigPDUInfo_st *addDlschPduToDlConfig(
		FAPI_dlConfigPDUInfo_st *pDlConfigPdu, DCIPduInfo *pDciPdu,
		UInt32 pduIndex, UInt8 tbIndex
		/* SPR 5807 Fix */       
		/* SPR 9627 Fix start*/
		//#ifdef TDD_CONFIG
		,UInt8 isOnlyOneTb 
		//#endif
		/* SPR 9627 Fix end */
		/* SPR 5807 Fix */       
		/* + TM7_EPRE */
		,FAPI_VendorSpecificForDLConfig_st *tempVendorSpecificForDLConfig_p, 
		UInt16 *numOfUEs,InternalCellIndex internalCellIndex)
/* - TM7_EPRE */
{
	FAPI_dlConfigPDUInfo_st     *pNextPdu = PNULL;
	/* + TM7_8 Changes Start */
#ifndef FLEXRAN
	UInt8 count = 0;
	UInt8 counter = 0;
	FAPI_beamFormingVectorInfo_st     *bfVector = PNULL;
	UInt16 *bfValue_per_antenna = PNULL;
	FAPI_PdschEpreToUeRsRatio_st  *tempPdschEpreToUeRsRatio_p = PNULL;
#endif
	/* - TM7_8 Changes End */
	pDlConfigPdu->pduType = FAPI_DLSCH_PDU;
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.pduIndex = MAC_PHY_CONVERT_16((UInt16)pduIndex);
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.rnti = MAC_PHY_CONVERT_16(pDciPdu->rnti);
	/* SPR# 892 Fix start */
	if ( RA_POLICY_2_LOCALIZED <= pDciPdu->resrcAllocType )
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.resAllocationType 
			= FAPI_RA_POLICY_2 ; 
	}
	else
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.resAllocationType
			= pDciPdu->resrcAllocType;
	}
	/* SPR# 892 Fix End */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.vRBAssignmentFlag
		= pDciPdu->virtulResrcBlkType;
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.rbCoding
		= MAC_PHY_CONVERT_32(pDciPdu->resrcAssgmntBitMap);
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transportBlocks
		= tbIndex;
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.tbToCodeWordSwapFlag
		= pDciPdu->tbSwapFlag;
	if ( TB_2_INDEX == tbIndex)
	{
		/* SPR 2422 Changes Start */
		/* In FAPI DLSch pdu mcs parameter is actually the modulation scheme.
		 * So filling the modulation scheme in the same */

		/* Cyclomatic_complexity_changes_start */
		getModSchemeFromMcsIndex(pDciPdu->tb2Info.modulationType,
				&(pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.mcs));
		/* Cyclomatic_complexity_changes_end */

		/* SPR 2422 Changes End */
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.redundancyVersion
			= pDciPdu->tb2Info.redundancyVer;
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.dlschPduLen
			= MAC_PHY_CONVERT_16(pDciPdu->tb2Info.tbSize);
	}
	else
	{
		/* SPR 2422 Changes Start */
		/* In FAPI DLSch pdu mcs parameter is actually the modulation scheme.
		 * So filling the modulation scheme in the same */

		/* Cyclomatic_complexity_changes_start */
		getModSchemeFromMcsIndex(pDciPdu->tb1Info.modulationType,
				&(pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.mcs));
		/* Cyclomatic_complexity_changes_end */

		/* SPR 2422 Changes End */
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.redundancyVersion
			= pDciPdu->tb1Info.redundancyVer;
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.dlschPduLen
			= MAC_PHY_CONVERT_16(pDciPdu->tb1Info.tbSize);
	}
/* txMode field addition FIX starts */
/* SPR 19288 change start */
/* SPR 19288 change end */
/* txMode field addition FIX ends */

	/** SI_MSG_MCS FIX START **/

	/* CMAS_CODE START */
	if(FAPI_RA_RNTI_P_RNTI_SI_RNTI == getRntiTypeForRnti(pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.rnti,internalCellIndex))
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.mcs = QPSK;
	}
	/* CMAS_CODE END */

	/** SI_MSG_MCS FIX END **/

	/* + CQI_5.0 */

	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transmissionScheme = 
		pDciPdu->transmissionScheme; 
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfLayers = pDciPdu->numLayer;

	if ( TX_MODE_3 == pDciPdu->transmissionMode )  /* 3053 */
	{
		/* SPR 5807 Fix start */
		/* SPR 9627 Fix start */
		//#ifdef TDD_CONFIG
		if (!isOnlyOneTb ) 
		{
			pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transmissionScheme =
				FAPI_TX_DIVERSITY;

			pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfLayers =  
				cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas;
		}
		//#endif
		/* SPR 9627 Fix end */

		/* SPR 5807 Fix end */
	}
	/* SPR 11403 fix start */ 
	/* Currently the following code snippet is commented, as in case of TM4, when 
	   only 1 TB is valid, tx_scheme is being changed into TX_DIVERSITY, while precoding index
	   is not set accordingly. */
	//  else if ( TX_MODE_4 == pDciPdu->transmissionMode )
	//  {
	/* SPR 5807 Fix start */
	/* SPR 9627 Fix start */
	//#ifdef TDD_CONFIG
	/* SPR 9627 Fix end */
	/*	if ( !isOnlyOneTb) 
		{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transmissionScheme =
		FAPI_TX_DIVERSITY;

		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfLayers =  
		cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas;
		}*/
	/* SPR 9627 Fix start */
	//#endif
	/* SPR 9627 Fix end */
	/* SPR 5807 Fix end */
	//}
	/* SPR 11403 fix end */ 
#ifdef PERF_STATS

	UPDATE_UE_TX_MODE_RI_STATS(pDciPdu->ueIndex,
			pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.transmissionScheme);
#endif
	/*numOfSubBand needs to be filled only for TX_MODE_3,4,5. Otherwise 
	 *it should be 0
	 */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfSubBand = 0;

#ifdef FLEXRAN
	UInt8 *subBandInfo_p = PNULL;         
	UInt8 subbandCount = pDciPdu->numberOfSubbands;

	if ( (TX_MODE_4 == pDciPdu->transmissionMode) || 
			(TX_MODE_3 == pDciPdu->transmissionMode))  
	{
                 pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfSubBand = pDciPdu->numberOfSubbands;
                 subBandInfo_p=((UInt8 *)(&pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.subBandInfo[0]));
		 if (subbandCount != 0)
		 {
			 while(subbandCount < (DIVIDE_BY_TWO(MAX_SUBBANDS)))
			 {
				 *subBandInfo_p =  pDciPdu->subbandtoPMIInfo[subbandCount];
				 subBandInfo_p = subBandInfo_p + sizeof(UInt8);
				 subbandCount--;
			 }
		 }
	}
	/* 4x4 DL MIMO CHG END */  
	/* + TM6_5.2 */
	else if(TX_MODE_6 == pDciPdu->transmissionMode)
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfSubBand
			= 1; /* only wideband supported - for now hard coded 1 */
		subBandInfo_p=((UInt8 *)(&pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.subBandInfo[0]));
		*subBandInfo_p = pDciPdu->subbandtoPMIInfo[subbandCount];
		subBandInfo_p = subBandInfo_p + sizeof(UInt8);
	}
        else
        {
		subBandInfo_p=((UInt8 *)(&pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.subBandInfo[0]));
        }

        FAPI_dlSCHConfigPDUInfoPart2_st *dlschPart2 = (FAPI_dlSCHConfigPDUInfoPart2_st *)subBandInfo_p;
      
        dlschPart2->ueCatagoryCapacity  
		=  pDciPdu->ueCategory;

	/* SPR 3377 */
	if (0 == dlschPart2->ueCatagoryCapacity)
	{
		dlschPart2->ueCatagoryCapacity = 5;
	}
        if (dlschPart2->ueCatagoryCapacity > 7 || dlschPart2->ueCatagoryCapacity < 1)
        {
            fprintf(stderr,"ueCat check fail ueCat %d\n\n", dlschPart2->ueCatagoryCapacity);
        }
	//dlschPart2->pA = pDciPdu->pA;
	dlschPart2->pA = 2;
	dlschPart2->deltaPowerOffsetAIndex = DEFAULT_DELTA_POWER_OFFSET_INDEX;
        dlschPart2->nGap = pDciPdu->nGAP;
        dlschPart2->nPRB = pDciPdu->nPRB;
	dlschPart2->transmissionMode = pDciPdu->transmissionMode;
	dlschPart2->numRbPerSubBand = 
		cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->dlAvailableRBs;
	dlschPart2->numbfVector = 0;
        
        FAPI_dlSCHConfigPDUInfoPart3_st *dlschPart3 = (FAPI_dlSCHConfigPDUInfoPart3_st *)&dlschPart2->bfVector[0];

        dlschPart3->n_scid = 0;
#else
	if(((pDciPdu->transmissionMode == TX_MODE_7) && (DCI_FORMAT_1 == pDciPdu->dciFormat)) ||
			((pDciPdu->transmissionMode == TX_MODE_8) && (DCI_FORMAT_2B == pDciPdu->dciFormat)))
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector = 1;
	}
	else
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector = 0;
	}
	/* 4x4 DL MIMO CHG START */  
	if ( (TX_MODE_4 == pDciPdu->transmissionMode) || 
			(TX_MODE_3 == pDciPdu->transmissionMode))  
	{
		/*SPR 13698 changes start*/ 
		fillCodebookInfoInDlschPdu(pDlConfigPdu, pDciPdu,internalCellIndex);        
		/*SPR 13698 changes end*/ 
	}
	/* 4x4 DL MIMO CHG END */  
	/* + TM6_5.2 */
	else if(TX_MODE_6 == pDciPdu->transmissionMode)
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfSubBand
			= 1; /* only wideband supported - for now hard coded 1 */
		/*SPR 13698 changes start*/ 
		fillCodebookInfoInDlschPduForTM6(pDlConfigPdu,pDciPdu,internalCellIndex);
		/*SPR 13698 changes end*/ 
	}
	/* - CQI_5.0 */
	/* -  SPR 1476 , 1487*/
	/* SPR# 903 Fix start */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.ueCatagoryCapacity
		=  pDciPdu->ueCategory;
	/* SPR 3377 */
	if (0 == pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.ueCatagoryCapacity)
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.ueCatagoryCapacity = 5;
	}
	/* SPR 3377 */
	/* SPR# 903 Fix end */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.pA = pDciPdu->pA;
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.deltaPowerOffsetAIndex
		= DEFAULT_DELTA_POWER_OFFSET_INDEX;
	/* CMAS_CODE START */
	//pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nGap = 0;
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nGap = pDciPdu->nGAP;
	/* CMAS_CODE END */
	/* SPR 1492 changes start */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nPRB = pDciPdu->nPRB;
	/* SPR 1492 changes end */
	/* + TM7_8 Changes Start */
	pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numRbPerSubBand =  
		cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->dlAvailableRBs;
#ifdef FAPI_DEVIATIONS    
#ifndef FAPI_4_0_COMPLIANCE
	if( DCI_FORMAT_2B == pDciPdu->dciFormat)
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nScId = pDciPdu->scramblingId;
	}
	else
	{
		pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.nScId = 0;
	}
#endif
#endif
	/* + TM7_EPRE */
	if((TX_MODE_7 == pDciPdu->transmissionMode)&&
			(DCI_FORMAT_1 == pDciPdu->dciFormat)&&
			(QPSK == pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.mcs))
	{
		tempPdschEpreToUeRsRatio_p = 
			&tempVendorSpecificForDLConfig_p->pdschEpreToUeRsRatioInfo[*numOfUEs]; 
		tempPdschEpreToUeRsRatio_p->rnti = pDciPdu->rnti; 
		tempPdschEpreToUeRsRatio_p->pdschEpreToUeRsRatio = pDciPdu->pdschEpreToUeRsRatio;
		(*numOfUEs)++;
		tempVendorSpecificForDLConfig_p->numOfUEs = *numOfUEs;
	}
	/* - TM7_EPRE */

	bfVector = &pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.bfVector[0];
	UInt16 size = 0;
	for(count =0; count < pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector; count++)
	{
		bfVector->subBandIndex = (pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector -1);
		bfVector->numOfAntenna = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas;
		size = sizeof(FAPI_beamFormingVectorInfo_st);

		for(counter = 0; counter < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas; counter++)
		{
			size += sizeof(UInt16);
			bfValue_per_antenna = (UInt16 *)(&bfVector->bfValue_per_antenna[counter]);
			*bfValue_per_antenna = pDciPdu->bfValue[counter];
		}
		size -= sizeof(UInt16);
		bfVector = (FAPI_beamFormingVectorInfo_st *)((UInt8 *)bfVector + size);
	}
#endif
	/* - TM7_8 Changes End */
	/* Now fill in the PDU size */


	/* + TM7_8 Changes Start */
#ifdef FLEXRAN
#if 0	//ysl bugfix 0816
	pDlConfigPdu->pduSize = MAC_PHY_CONVERT_16( (
				offsetof(FAPI_dlConfigPDUInfo_st, dlConfigpduInfo) + 
				(offsetof(FAPI_dlSCHConfigPDUInfo_st, subBandInfo) ) 
                                + 10 /* size of FAPI_dlSCHConfigPDUInfo_part2_st except FAPI_beamFormingVectorInfo_st */
                                + 1 /* size of FAPI_dlSCHConfigPDUInfo_part3_st*/
				+ (sizeof(uint8_t) * pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfSubBand)));


    if ( pDlConfigPdu->pduSize % sizeof(UInt32) )
    {
        pDlConfigPdu->pduSize += (sizeof(UInt32) - pDlConfigPdu->pduSize);
    }
#else
	UInt8 pduSize =  (offsetof(FAPI_dlConfigPDUInfo_st, dlConfigpduInfo) + 
				(offsetof(FAPI_dlSCHConfigPDUInfo_st, subBandInfo) ) 
								+ 10 /* size of FAPI_dlSCHConfigPDUInfo_part2_st except FAPI_beamFormingVectorInfo_st */
								+ 1 /* size of FAPI_dlSCHConfigPDUInfo_part3_st*/
				+ (sizeof(uint8_t) * pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfSubBand));

	if ((pduSize % sizeof(UInt32)) > 0)
	{
		pduSize += (sizeof(UInt32) - (pduSize % sizeof(UInt32)));
	}
	pDlConfigPdu->pduSize = MAC_PHY_CONVERT_16(pduSize);
#endif
#else
	pDlConfigPdu->pduSize = MAC_PHY_CONVERT_16( (
				offsetof(FAPI_dlConfigPDUInfo_st, dlConfigpduInfo) + 
				(offsetof(FAPI_dlSCHConfigPDUInfo_st, subBandInfo) )
				+ (sizeof(uint8_t) * pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numOfSubBand)
				+ (sizeof(FAPI_beamFormingVectorInfo_st) * 
					(pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector)) +
				((pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.numbfVector) *
				 (sizeof(uint16_t)*(cellSpecificParams_g.\
						    cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
						    numOfTxAnteenas - 1)))));

	/* - TM7_8 Changes End */
	/* pdu size should be a multiple of 4 bytes */
	/* SPR 1132 starts */ 
	if ( pDlConfigPdu->pduSize % sizeof(UInt32) )
	{
		pDlConfigPdu->pduSize += (sizeof(UInt32)) - (pDlConfigPdu->pduSize % sizeof(UInt32));
	}
#endif
	/* SPR 1132 ends */ 
	/* No data in pDlConfigPdu->dlConfigpduInfo.DlSCHPdu.bfVector */
	/*pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8 *)pDlConfigPdu)
	  + pDlConfigPdu->pduSize);*/
	/* Logging for PDSCH Power */
	LOG_MAC_MSG(MAC_PDSCH_POWER_ID,LOGDEBUG,MAC_DL_PC,\
			getCurrentTick(),\
			pDciPdu->ueIndex,pDciPdu->pA,pDciPdu->ueCategory,0,0,\
			0.0,0.0,__func__,"DL Total Transmission Power for PDSCH");

	pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8*)pDlConfigPdu) 
			+ pDlConfigPdu->pduSize);


	return pNextPdu;
}
/*****************************************************************************
 * Function Name  :  addPchPduToDlConfig
 * Inputs         :  pDlConfigPdu - Pointer to message buffer to fill in the
 *                                  PDU,
 *                   pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message and
 *                   pduIndex - Index of data
 * Outputs        :  None
 * Returns        :  Pointer to the buffer which can be used to fill in the
 *                   next PDU
 * Description    :  This function fills in the message with the PCH PDU data
 *                   and returns the pointer to the buffer which can be used to
 *                   fill in the next PDU.
 *****************************************************************************/
STATIC  FAPI_dlConfigPDUInfo_st *addPchPduToDlConfig(
		FAPI_dlConfigPDUInfo_st *pDlConfigPdu, DCIPduInfo *pDciPdu,
		UInt32 pduIndex)
{
	FAPI_dlConfigPDUInfo_st     *pNextPdu = PNULL;
	pDlConfigPdu->pduType = FAPI_PCH_PDU;
	pDlConfigPdu->pduSize
		= /* MAC_PHY_CONVERT_16( */ (
					offsetof(FAPI_dlConfigPDUInfo_st, dlConfigpduInfo) +
					sizeof(FAPI_pchPduConfigInfo_st))/*)*/;
	pDlConfigPdu->dlConfigpduInfo.PCHPdu.pchPduLen = MAC_PHY_CONVERT_16(pDciPdu->tb1Info.tbSize);
	pDlConfigPdu->dlConfigpduInfo.PCHPdu.pduIndex = MAC_PHY_CONVERT_16(pduIndex);
	pDlConfigPdu->dlConfigpduInfo.PCHPdu.pRNTI = MAC_PHY_CONVERT_16(PAGING_RNTI);
	/* SPR 1365 PCH_SPR_START */
        /* SPR 21993 Fix Start */
#ifdef FAPI_4_0_COMPLIANCE
        if(pDciPdu->dciFormat == DCI_FORMAT_1C)
        {
	    /* RAT policy RAT Type2 value is 3  for 1C */
            pDlConfigPdu->dlConfigpduInfo.PCHPdu.resAllocationType
                = FAPI_RA_POLICY_2_1C;
	}
	else
	{
            /* RAT policy RAT Type2 value is 2  for 1A,1B,1D */
            pDlConfigPdu->dlConfigpduInfo.PCHPdu.resAllocationType
                = FAPI_RA_POLICY_2 ;
	}
#else
        pDlConfigPdu->dlConfigpduInfo.PCHPdu.resAllocationType
            = FAPI_RA_POLICY_2 ;
#endif
        /* SPR 21993 Fix End */ 
 
	pDlConfigPdu->dlConfigpduInfo.PCHPdu.vRBAssignmentFlag
		= pDciPdu->virtulResrcBlkType;
	pDlConfigPdu->dlConfigpduInfo.PCHPdu.rbCoding
		= MAC_PHY_CONVERT_32(pDciPdu->resrcAssgmntBitMap);
	pDlConfigPdu->dlConfigpduInfo.PCHPdu.mcs = FAPI_PCH_QPSK;
	/* SPR 1365 PCH_SPR_END */
	pDlConfigPdu->dlConfigpduInfo.PCHPdu.redundancyVersion
		= pDciPdu->tb1Info.redundancyVer;
	pDlConfigPdu->dlConfigpduInfo.PCHPdu.numOftransportBlocks
		= 1; /* Only 1 in PCH */
	pDlConfigPdu->dlConfigpduInfo.PCHPdu.tbToCodeWordSwapFlag
		=  0; /* Not used */
	/* + SPR_7309 */
/* SPR 19288 change start */
	if (pDciPdu->numLayer == 1)
	{
		pDlConfigPdu->dlConfigpduInfo.PCHPdu.transmissionScheme
			= DEFAULT_TRANSMISSION_SCHEME;
	}
	else 
	{
		pDlConfigPdu->dlConfigpduInfo.PCHPdu.transmissionScheme
			= FAPI_TX_DIVERSITY; 
	}
	/* - SPR_7309 */
	pDlConfigPdu->dlConfigpduInfo.PCHPdu.numOfLayers
		= pDciPdu->numLayer;
/* SPR 19288 change end */

	pDlConfigPdu->dlConfigpduInfo.PCHPdu.codeBookIndex = 0; /* not used */
	pDlConfigPdu->dlConfigpduInfo.PCHPdu.ueCatagoryCapacity
		= 0; /* Not used in PCH */
	pDlConfigPdu->dlConfigpduInfo.PCHPdu.pA = pDciPdu->pA;
	/* CLPC_REWORK CHG */
	pDlConfigPdu->dlConfigpduInfo.PCHPdu.txPower = 
		MAC_PHY_CONVERT_16(pDciPdu->txPower);
	/* CLPC_REWORK_CHG END */
	pDlConfigPdu->dlConfigpduInfo.PCHPdu.nPRB = pDciPdu->tpc;
#ifdef FAPI_4_0_COMPLIANCE
	/*FAPI2_1 changes start*/
	pDlConfigPdu->dlConfigpduInfo.PCHPdu.nGap=pDciPdu->nGAP;
	/*FAPI2_1 changes end*/
#endif
/* SPR 19288 change start */
/* SPR 19288 change end */


	/* pdu size should be a multiple of 4 bytes */
	/* SPR 1132 starts */ 
	if ( pDlConfigPdu->pduSize % sizeof(UInt32) )
	{
		pDlConfigPdu->pduSize += sizeof(UInt32) - (pDlConfigPdu->pduSize % sizeof(UInt32));
	}
	/* SPR 1132 ends*/ 
	/*pNextPdu = (FAPI_dlConfigPDUInfo_st *)( ( (UInt8 *)pDlConfigPdu)
	  + sizeof(FAPI_pchPduConfigInfo_st) );*/
	pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8 *)pDlConfigPdu)
			+ pDlConfigPdu->pduSize);
	return pNextPdu;
}
/* SPR 5346 changes start */   
/*****************************************************************************
 * Function Name  :  dummyTxDLControlMsg
 * Inputs         :  phyDelay - The delay of the execution leg in terms of the 
 *                              number of sub-frames w.r.t. PHY and the current 
 *                              processing being done at MAC(is intended for PHY),
 *                   currentGlobalTick - The present Global Tick,
 *                   sysFrame - Frame number of sysFrame and
 *                   subFrame - Frame number of subFrame
 *                   internalCellIndex - cell id  
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will create the entire control buffer it can 
 *                   create the data buffer if the falg isDataFlag is set, the 
 *                   function will also send the data buffer.
 *****************************************************************************/
MacRetType dummyTxDLControlMsg(UInt8 phyDelay,
        /* +- SPR 17777 */
		UInt32 sysFrame,
		UInt32 subFrame,
		InternalCellIndex internalCellIndex)
{
	UInt32  subFrameNum = 0;
	UInt32  sysFrameNum = 0;
	UInt16  sfnSf = 0;
	UInt32  numDci = 0;
	UInt32  numPdu = 0;
	UInt32  length = 0;
	UInt32  numPdschRnti = 0;
/* SPR 19288 change start */
/* SPR 19288 change end */
#ifdef LAYER2_PHYSIM_THREAD
	UInt8 *msgBuffer = PNULL;
#else
	UInt8   *msgBuffer = pDLCfgMsgBuff_gp[internalCellIndex];
#endif
	FAPI_dlConfigPDUInfo_st     *pNextPdu = PNULL;
	FAPI_dlConfigRequest_st     *pDlConfigReq = PNULL;;

#ifdef TDD_CONFIG 
	UInt8 sfType = getSubframeType(((subFrame + DL_ADVANCE_TICK) % MAX_SUB_FRAME), internalCellIndex);
	if ((DL == sfType) || (SP == sfType))
	{    
#endif    

#ifdef LAYER2_PHYSIM_THREAD
		GET_MEM_FROM_POOL(UInt8,msgBuffer,CONTROL_BUFFER_SIZE, PNULL);
#endif

		subFrameNum = (subFrame + phyDelay) % MAX_SUB_FRAME;
		/* Add overflow from subframe to sysframe and wrap around it at
		   1024 (MAX_SFN_VALUE) */
		sysFrameNum =
			(sysFrame + (subFrame + phyDelay)/MAX_SUB_FRAME) % MAX_SFN_VALUE;

		/* Generate SFN/SF value from sysFrame and subFrame
		   (Add phyDelay to subFrame and check overflow for both values) */
		sfnSf = GENERATE_SUBFRAME_SFNSF(sysFrameNum, subFrameNum);


		/* Get the pointer to Fill in API parameters */
		pDlConfigReq = (FAPI_dlConfigRequest_st *)(msgBuffer + L1_API_HEADER_LEN);

		/* Get the pointer to first PDU in message buffer to start filling the
		   PDUs */
		pNextPdu = (FAPI_dlConfigPDUInfo_st *)&pDlConfigReq->dlConfigpduInfo[0];
		length += ((uintptr_t)pNextPdu - (uintptr_t)pDlConfigReq);

		/* Fill buffer header */
        /* +- SPR 17777 */
		fillMacApiBufferHeader(msgBuffer,
				PHY_DL_CONFIG_REQUEST, length);
        /* +- SPR 17777 */

		/* Fill in all the PDU configurations */
		pDlConfigReq->sfnsf = MAC_PHY_CONVERT_16(sfnSf);
		pDlConfigReq->length = MAC_PHY_CONVERT_16(length);
		/* CLPC_REWORK CHG */
		pDlConfigReq->txPowerForPCFICH =  
			MAC_PHY_CONVERT_16(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
					cellParams_p->commonDLPowerControlInfo.\
					pcfichPowerOffset);
		/* CLPC_REWORK_CHG END */
		pDlConfigReq->numDCI = numDci;
		pDlConfigReq->numOfPDU = MAC_PHY_CONVERT_16(numPdu);
		pDlConfigReq->numOfPDSCHRNTI = numPdschRnti;
        /*BUG:13001:start*/
		pDlConfigReq->cfi = currentCFIValue_g[internalCellIndex];
        /*BUG:13001:end*/

#ifdef FLEXRAN
   g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_DL_CFG_REQ; 
#endif

/* SPR 19288 change start */
/* SPR 19288 change end */

		/* Send data to Physical */
		if (0 >= sendDataToPhysical (msgBuffer, length + L1_API_HEADER_LEN, internalCellIndex) )
		{
			LOG_MAC_MSG(L2_SOCKET_SENDTO_FAIL_ID,LOGERROR,L2_SYS_FAIL,\
					getCurrentTick(),\
					__LINE__,0,0,0,0,\
					0.0,0.0,__FILE__,__func__);
		}
#ifdef TDD_CONFIG
	}
#endif

#ifdef TDD_CONFIG
	sfType = getSubframeType(((subFrame + macParams_g.ulConfigAdvanceSubFrames) % MAX_SUB_FRAME), 
			internalCellIndex);
	if ((UL == sfType) || (SP == sfType))
	{    
#endif

#ifdef FLEXRAN
   g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_UL_CFG_REQ; 
#endif
 
		dummyPrepareULConfigMsg (
                /* +- SPR 17777 */
				sysFrame, 
				subFrame,
				internalCellIndex
				);
#ifdef TDD_CONFIG
	}
#endif     
/* SPR 19288 change start */
/* SPR 19288 change end */
	return MAC_SUCCESS;
}
/* SPR 19288 change start */
/* SPR 19288 change end */
/* SPR 5346 changes end */   
/*****************************************************************************
 * Function Name  :  addMchPduToDLConfig
 * Inputs         :  pDlDataPdu - Pointer to message buffer to fill in the
 *                                PDU.
 *                   pduIndex - Index of PDU
 * Outputs        :  None
 * Returns        :  Pointer to the buffer which can be used to fill in the
 *                   next PDU
 * Description    :  This function fills in the message with the MCH PDU data
 *                   and returns the pointer to the buffer which can be used to
 *                   fill in the next PDU.
 *****************************************************************************/
#ifdef LTE_EMBMS_SUPPORTED

STATIC  FAPI_dlConfigPDUInfo_st *addMchPduToDLConfig(
        FAPI_dlConfigPDUInfo_st *pDlConfigPdu,
        MchPduInfo *mchPduInfo_p,
        UInt32  pduIndex,
        InternalCellIndex internalCellIndex)
{
    FAPI_dlConfigPDUInfo_st     *pNextPdu = PNULL;
    pDlConfigPdu->pduType = FAPI_MCH_PDU;

	pDlConfigPdu->pduSize= (offsetof(FAPI_dlConfigPDUInfo_st, dlConfigpduInfo) +\
					sizeof(FAPI_mchConfigPDUInfo_st));

    /*Fill MCH pdu fields*/
    pDlConfigPdu->dlConfigpduInfo.MCHPdu.rnti = MAC_PHY_CONVERT_16(MCH_PDU_RNTI);
    pDlConfigPdu->dlConfigpduInfo.MCHPdu.mchPduLen = mchPduInfo_p->tbSize ;
    pDlConfigPdu->dlConfigpduInfo.MCHPdu.pduIndex = pduIndex;
    pDlConfigPdu->dlConfigpduInfo.MCHPdu.resAllocationType = 0xFF; //Not required
    /* SPR 18337 fix start */
    getModSchemeFromMcsIndex(mchPduInfo_p->mcs,
                &(pDlConfigPdu->dlConfigpduInfo.MCHPdu.modulationType));
    /* SPR 18337 fix end */
    pDlConfigPdu->dlConfigpduInfo.MCHPdu.rbCoding = 0xFFFFFFFF; //Not required
    pDlConfigPdu->dlConfigpduInfo.MCHPdu.txPower =  \
               cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
               cellParams_p->eMbmsTransPower;
    pDlConfigPdu->dlConfigpduInfo.MCHPdu.mbsfnAreaId = mchPduInfo_p->areaId;


    if ( pDlConfigPdu->pduSize % sizeof(UInt32) )
    {
        pDlConfigPdu->pduSize += sizeof(UInt32) - (pDlConfigPdu->pduSize % sizeof(UInt32));
    }

    pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8 *)pDlConfigPdu)
            + pDlConfigPdu->pduSize);

    return pNextPdu;
}
#endif

/*****************************************************************************
 * Function Name  :  txDLControlMsg
 * Inputs         :  phyDelay - The delay of the execution leg in terms of the 
 *                              number of sub-frames w.r.t. PHY and the current 
 *                              processing being done at MAC(is intended for PHY),
 *                   currentGlobalTick - The present Global Tick,
 *                   sysFrame - Frame number of sysFrame and
 *                   subFrame - Frame number of subFrame
 *                   internalCellIndex - cell id  
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will create the entire control buffer it can 
 *                   create the data buffer if the falg isDataFlag is set, the 
 *                   function will also send the data buffer.
 *****************************************************************************/
/*CA Changes start*/
/* DL DATA SEP Changes */
extern SInt32 ev_fd_tx_g[MAX_NUM_CELL];

UInt32 ulCheckPDUnum=0;

/* SPR 9399 Changes Start */
#ifdef DL_DATA_SEPARATION    
#ifndef DL_CONFIG_DATA_SYNC_DISABLED
extern SInt32 ev_fd_tx_Control_g[MAX_NUM_CELL];
#endif
#endif
/* SPR 9399 Changes End */
/* SPR 8712 changes */
extern SInt32 ev_fd_pusch_g;
/* SPR 8712 changes */

MacRetType txDLControlMsg(UInt8 phyDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 sysFrame,
		UInt32 subFrame,
		InternalCellIndex internalCellIndex)
{
	UInt8   containerTick = 0;
	UInt32  subFrameNum = 0;
	UInt32  sysFrameNum = 0;
	UInt32  pduIndex = 0;
	UInt16  sfnSf = 0;
	UInt32  numDci = 0;
	/*+ Dyn UEs To Be SchPerTTI +*/
	UInt32  numUEDci = 0;
	/*- Dyn UEs To Be SchPerTTI -*/
	UInt32  numPdu = 0;
	UInt32  length = 0;
	/* + TM7_EPRE */
	UInt8  vendorSpLen = 0;
	/* - TM7_EPRE */
	UInt32  numPdschRnti = 0;
	UInt8   cceCount = 0;
	UInt16  cceNumber = 0;
	UInt8   localCount = 0;
#ifdef LAYER2_PHYSIM_THREAD
	UInt8 *msgBuffer = PNULL;
#else
	/*OPT_SPR_4119_4120_4122_4123_CHANGES_START*/
	UInt8   *msgBuffer = pDLCfgMsgBuff_gp[internalCellIndex];
	/* SPR 9917 changes Start */
	memSet(pDLCfgMsgBuff_gp[internalCellIndex], 0, CONTROL_BUFFER_SIZE);
	/* SPR 9917 changes End */
	/*OPT_SPR_4119_4120_4122_4123_CHANGES_START*/
#endif

	FAPI_dlConfigPDUInfo_st     *pNextPdu = PNULL;
	FAPI_dlConfigPDUInfo_st     *pPrevPdu = PNULL;
    FAPI_dlConfigRequest_st     *pDlConfigReq = PNULL;
	DciCCEContainerInfo* dciCCEContainerInfo_p = PNULL;
	DCIPduInfo      *pDciPduInfo = PNULL;
	DciCCEInfo      *pDciCceInfo = PNULL;
#ifdef LTE_EMBMS_SUPPORTED
    MchPduInfo *pMchPduInfo = PNULL;
#endif
	/* SPR 609 changes start */    
	//DLUEContextInfo *ueContextInfo_p = PNULL; 
	//DLHARQProcess *harqProcess_p = PNULL;
	/* SPR 609 changes end */    
	/* SPR 1868 changes start */ 
	UInt8 flagForDciFormat0 = 0;
	/* SPR 1868 changes end */ 
#ifdef FDD_CONFIG
    TempCRNTICtx *tempUEContext_p = PNULL;
#endif
	FAPI_VendorSpecificForDLConfig_st tempVendorSpecificForDLConfig ; 
	UInt16 numOfUEs = 0; 
	tempVendorSpecificForDLConfig.numOfUEs = 0; 
	FAPI_VendorSpecificForDLConfig_st *tempVendorSpecificForDLConfig_p = PNULL;    


	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Entry",__func__);
#ifdef LAYER2_PHYSIM_THREAD
	GET_MEM_FROM_POOL(UInt8,msgBuffer,CONTROL_BUFFER_SIZE, PNULL);
#endif
	containerTick = (currentGlobalTick + phyDelay) % MAX_PDCCH_CONTAINER;
	dciCCEContainerInfo_p = (dciCCEContainerInfo_gp[internalCellIndex] + containerTick);
	localCount = dciCCEContainerInfo_p->countCmnChannelMsg;

	subFrameNum = (subFrame + phyDelay) % MAX_SUB_FRAME;
	/* Add overflow from subframe to sysframe and wrap around it at
	   1024 (MAX_SFN_VALUE) */
	sysFrameNum =
		(sysFrame + (subFrame + phyDelay)/MAX_SUB_FRAME) % MAX_SFN_VALUE;

	/* Generate SFN/SF value from sysFrame and subFrame
	   (Add phyDelay to subFrame and check overflow for both values) */
	sfnSf = GENERATE_SUBFRAME_SFNSF(sysFrameNum, subFrameNum);

	/* Get the pointer to Fill in API parameters */
	pDlConfigReq = (FAPI_dlConfigRequest_st *)(msgBuffer + L1_API_HEADER_LEN);

	/* Get the pointer to first PDU in message buffer to start filling the
	   PDUs */
	pNextPdu = (FAPI_dlConfigPDUInfo_st *)&pDlConfigReq->dlConfigpduInfo[0];
	length += ((uintptr_t)pNextPdu - (uintptr_t)pDlConfigReq);


	if (dciCCEContainerInfo_p->mibData_p)
	{
		/* Fill in BCH PDUs - PDU stored in dciCCEContainerInfo_p->mibData_p */
        pPrevPdu = pNextPdu;
		LOG_UT(MAC_BCH_CTL_TX_ID,LOGDEBUG,MAC_L1_INF,\
				currentGlobalTick,dciCCEContainerInfo_p->mibDataLen,\
				pduIndex,DEFAULT_TX_POWER,0,0,0.0,0.0,__func__,"BCH_DCI_PDU");

		pNextPdu = addBchPduToDlConfig(pNextPdu, dciCCEContainerInfo_p,
				pduIndex,internalCellIndex);

		if (pPrevPdu != pNextPdu)
		{
			numPdu++;
			pduIndex++;
            length += ((uintptr_t)pNextPdu - (uintptr_t)pPrevPdu);
        }
	}
#ifdef LTE_EMBMS_SUPPORTED
    pMchPduInfo = mchPduInfo_gp[internalCellIndex];

    if (pMchPduInfo != PNULL)
    {
        pPrevPdu = pNextPdu;
        /*MCH PDU*/
        pNextPdu = addMchPduToDLConfig(pNextPdu,pMchPduInfo,pduIndex,internalCellIndex);
        if(pMchPduInfo->dataTB_p == PNULL)
        {
            /*Free mempool taken at encoder for Mch pdu*/
            freeMemPool(pMchPduInfo);
            mchPduInfo_gp[internalCellIndex]=PNULL;
        }

        if (pPrevPdu != pNextPdu)
        {
            numPdu++;
            pduIndex++;
            length += ((uintptr_t)pNextPdu - (uintptr_t)pPrevPdu);
        }
    }
#endif

	/* Loop through all DCI PDUs (stored in dciCCEContainerInfo_g) */
    /*EMBMS: cceCount will be 1 if DCI 1C to be sent */ 
	cceCount = dciCCEContainerInfo_p->countOfCCEAllocated;
	while (cceCount--)
	{
		cceNumber = dciCCEContainerInfo_p->arrayCCEAllocated[cceCount];
		pDciCceInfo = &(dciCCEContainerInfo_p->dciCCEInfo[cceNumber]);

		/* If CCE is used (not free), then use the DCI PDU info to send msg. For
		 * PDCCH Order, Harq Process Id will always be invalid as no data PDU is
		 * sent, thus OR condition in the below if statement*/
		if (!pDciCceInfo->isCCEFree && 
				((pDciCceInfo->dciPduInfo.harqProcessId != INVALID_HARQ_ID)
				 || (PDCCH_ORDER_MSG == pDciCceInfo->msgType)
				 || (CCCH_MSG == pDciCceInfo->msgType)
                 || (TPC_MSG == pDciCceInfo->msgType)))
        {
            /* DCI present, so increment the count */
            pDciPduInfo = &pDciCceInfo->dciPduInfo;

            /* SPR 3877 FIX START */
            /* Updating tick in RRCConnectionSetup */
            /* SPR 21652 Fix + */
            /* In case of UE Conext not exist or 
             * RRC Connection reject 
             * the ueIndex is filled with crnti and
             * Harq is filled with INVALID_HARQ_ID 
             * So in this case we need not to fetch the UE Context.
             */
			if ( (CCCH_MSG == pDciCceInfo->msgType) && 
                    (pDciCceInfo->dciPduInfo.harqProcessId != INVALID_HARQ_ID))
            /* SPR 21652 Fix - */
            {    
                DLUEContextInfo     *dlUEContextInfo_p = PNULL;
                DLUEContext         *dlUEContext_p = PNULL;

                /*SPR 6497 Fix Start*/
                ULUEContextInfo *ulUEContextInfo_p = PNULL;
                ULUEContext  *ulUEContext_p = PNULL;
                /*SPR 6497 Fix End*/

                dlUEContextInfo_p = &dlUECtxInfoArr_g[pDciPduInfo->ueIndex];
                dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;

                /*SPR 6497 Fix Start*/
                ulUEContextInfo_p = &ulUECtxInfoArr_g[pDciPduInfo->ueIndex];
                /*SPR 6997 Fix Start*/
                if(PNULL != ulUEContextInfo_p)
                {
                    ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
                    if(PNULL != ulUEContext_p)
                    {
                        /*Enable SRS State in DL UE Context*/
                        if(ulUEContext_p->srsConfigInfo.srsConfigType == 
                                SRS_DEDICATED_CONFIG_TYPE_SETUP_WAITING)
                        {
                            ulUEContext_p->srsConfigInfo.srsConfigType = 
                                SRS_DEDICATED_CONFIG_TYPE_SETUP;
                        }
                    }
                }
                /*SPR 6997 Fix End*/
                /*SPR 6497 Fix End*/

                /* SPR 3888 FIX START */
                /* DCM crash fix start */
                if (PNULL != dlUEContext_p)
                {
                    dlUEContext_p->lastSyncDetectedTick = getCurrentTick();
                }
                /* DCM crash fix end */
                /* SPR 3888 FIX START */
            }    
            /* SPR 3877 FIX END */
            /* Updating tick in RRCConnectionSetup */

            if (pDciPduInfo->dciFormat == DCI_FORMAT_0 ||
                    pDciPduInfo->dciFormat == DCI_FORMAT_0_MSG3 ||
                    pDciPduInfo->dciFormat == DCI_FORMAT_3 ||
                    pDciPduInfo->dciFormat == DCI_FORMAT_3A ||
                    pDciPduInfo->dciFormat == MAX_NUM_DCI_FORMAT)
            {
                /* SPR 1868 changes start */ 
                flagForDciFormat0 = 1;
                /* SPR 1868 changes end */ 
                continue;
            }

#ifndef DL_DATA_SEPARATION            
            /* Check is invalid as dataTB_p & dataTB2_p would be null with DL Data path separation */
            /*SPR 5993 Fix Start*/
            /* SPS Deactivation SPR 7171 Fix Start */
            if (SPS_DEACTIVATION != pDciCceInfo->msgType &&
                    /* SPS Deactivation SPR 7171 Fix End */
                    PNULL == pDciPduInfo->dataTB_p && PNULL == pDciPduInfo->dataTB2_p &&
                    /*START:SPR:7183*/
                    PDCCH_ORDER_MSG != pDciCceInfo->msgType &&
                    TPC_MSG != pDciCceInfo->msgType)
                /*END:SPR:7183*/
            {
                continue;
            }
            /*SPR 5993 Fix End*/
#endif

            if ((SPS_OCCASION != pDciCceInfo->msgType) &&
                    (cceNumber < MAX_CCE)
#ifdef LTE_EMBMS_SUPPORTED
                 && (pDciCceInfo->msgType != EMBMS_DCI1C_NOTIFICATION)
#endif
                 )
            {
                /* Fill in DCI PDU in DL_CONFIG.Request message */
                pPrevPdu = pNextPdu;

                LOG_UT(MAC_DCI_TX_ID,LOGDEBUG,MAC_L1_INF,\
                        currentGlobalTick,pDciPduInfo->ueIndex,\
                        pDciPduInfo->harqProcessId,pDciPduInfo->dciFormat,\
                        pDciPduInfo->isTBOneValid,pDciPduInfo->isTBTwoValid,\
                        0.0,0.0,__func__,"DATA_DCI_PDU_TX");
                /* SPR 6967 Fix Start */
                setTxIndFromHarq(pDciPduInfo,internalCellIndex
                        /* + SPR 17733 */
#ifdef FDD_CONFIG
                        ,pDciCceInfo->msgType
#endif
                        /* + SPR 17733 */
                        );
                /* SPR 6967 Fix End */
                /*SPR 13698 fix start*/
                pNextPdu = (* addDciToDLConfigFunction)(pNextPdu, pDciPduInfo, pDciCceInfo,internalCellIndex);
                /*SPR 13698 fix end*/                
                if (pPrevPdu != pNextPdu)
                {
                    /* Increase number of DCI only if add DCI was successful */
                    numDci++;
                    /*+ Dyn UEs To Be SchPerTTI +*/
                    if(pDciPduInfo->rnti != 0xFFFF)
                    {
                        numUEDci++;
                    }
                    /*- Dyn UEs To Be SchPerTTI -*/
                    numPdu++;
                    length += ((uintptr_t)pNextPdu - (uintptr_t)pPrevPdu);
#ifdef UTFWK_SIMULATION
                    UInt32 rntiType =  getRntiTypeForRnti(pDciPduInfo->rnti,internalCellIndex);
                    addEntryInHarqList(pDciPduInfo->ueIndex,subFrame,rntiType );
#endif
                }	
                LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"pdu size DCI = %d\n", pPrevPdu->pduSize);
                /*EMBMS: CCE count for DCI 1C is updated in alloc PDCCH EL*/
            }
            /* SPS_CHG_END */

            /* SPR 1380 Temp Fix Start*/
            /*!
             *  \code
             *  MacRetType txDLControlMsg(UInt8 phyDelay,
             UInt32 currentGlobalTick,
             UInt32 sysFrame,
             UInt32 subFrame)
             {
             ...
             if ((SPS_OCCASION != pDciCceInfo->msgType) &&
             (cceNumber < MAX_CCE))
             {
             ...
             - Add DciPduToDlConfig
             }
             if((INVALID_HARQ_ID != pDciPduInfo->harqProcessId)
             && (SPS_DEACTIVATION !=  pDciCceInfo->msgType)
             )
             {
             ...
             - Add DlschPduToDlConfig
             ...
             }
             }
             \endcode
             */
            if( INVALID_HARQ_ID != pDciPduInfo->harqProcessId )
            {
                if (SPS_DEACTIVATION !=  pDciCceInfo->msgType)
                {
                    UInt32 ueIndex = 0;
                    DLUEContextInfo *ueContextInfo_p = PNULL;
                    DLHARQProcess *harqProcess_p = PNULL;

                    ueIndex=pDciPduInfo->ueIndex;

                    if(MAX_UE_SUPPORTED > ueIndex)
                    {

                        /* + SPR 17733 */
#ifdef FDD_CONFIG
                        if(CONTENTION_RESL_MSG ==  pDciCceInfo->msgType)
                        {
                            tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].tempCRNTICtx_p;
                            if (tempUEContext_p == PNULL)
                            {
                                LOG_MSG(MAC_ULSCH_FREQ_SEL_SCH_ID,LOGWARNING,MAC_PUSCH,
                                        getCurrentTick(), ueIndex, 0,0,0,0, 0,0,
                                        __func__,"");
                                continue;
                            }
                            harqProcess_p = &tempUEContext_p->dlHarqCtx_p->
                                harqProcess[pDciPduInfo->harqProcessId];
                        }
                        else
                        {
#endif
                            /* - SPR 17733 */
                            ueContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
                            /* coverity fix 99544 */
                            /* + SPR 11469 Changes */
                            if(IS_PCELL_OF_UE(ueContextInfo_p->dlUEContext_p, internalCellIndex))
                            {
                                harqProcess_p =
                                    &ueContextInfo_p->dlUEContext_p->dlHarqContext_p
                                    ->harqProcess[pDciPduInfo->harqProcessId];
                            }
                            else
                            {
                                harqProcess_p =
                                    &ueContextInfo_p->dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlHarqContext_p
                                    ->harqProcess[pDciPduInfo->harqProcessId];
                            }
                            /* - SPR 11469 Changes */
                            /* + SPR 17733 */
#ifdef FDD_CONFIG
                        }
#endif
                        /* - SPR 17733 */


                        addDlschPduForValidHarqId(pDciPduInfo,&pPrevPdu,&pNextPdu,&pduIndex,
                                /* +- SPR 17777 */
                                &numPdu,&length,&numPdschRnti,
#ifdef UTFWK_SIMULATION
                                subFrame,
#endif
                                /* +- SPR 17777 */
                                harqProcess_p->isTBOneValid,harqProcess_p->isTBTwoValid,
                                &tempVendorSpecificForDLConfig,&numOfUEs
                                ,internalCellIndex);
                    }
                }
            }
            else
            {
                addDlschPduForInvalidHarqId(pDciPduInfo,&pPrevPdu,&pNextPdu,&pduIndex,
                        &numPdu,&length,&numPdschRnti,
                        &tempVendorSpecificForDLConfig,&numOfUEs
                        ,internalCellIndex);

#ifdef SHM_PRINT_ENABLED
                ttiDciLoggingInfo *log_ptr = LOG_TTI_MSG(MAC_TTI_DCI_LOG_INFO);
                if(log_ptr) 
                {
                    fillttiDciLoggingInfo (log_ptr , pDciCceInfo, internalCellIndex);
                }
#endif
            }
            LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"pdu size DLSCH = %d\n", pPrevPdu->pduSize);
        }
#ifdef DL_UL_SPLIT
		/* In case if RB allocation for a UE was failed, 
		 * then convert that UE.s ULCOnfig message so that 
		 * no HARQ info will be detected by PHY in the desired sub frame */
		else if ( (pDciCceInfo->isCCEFree == CCE_FREE ) && 
				(pDciCceInfo->dciPduInfo.harqProcessId == INVALID_HARQ_ID) )
		{
			/*SPR_10440_Changes Start*/
			pDciPduInfo = &pDciCceInfo->dciPduInfo;
			if (pDciPduInfo->dciFormat == DCI_FORMAT_0 ||
					pDciPduInfo->dciFormat == DCI_FORMAT_0_MSG3 ||
					pDciPduInfo->dciFormat == DCI_FORMAT_3 ||
					pDciPduInfo->dciFormat == DCI_FORMAT_3A)
			{
				continue;
			}
			/*SPR_10440_Changes End*/

			PdcchULOutputInfo* tempPdcchULOutput_p = (pdcchULOutputInfo_gp[internalCellIndex]+containerTick);
			ULSchUciRachPoolInfo * ulSchUciPoolInfo_p = NULL;
			ULSchUciInfo   * ulSchUciInfo_p = NULL;
			ulSchUciPoolInfo_p = ulSchUciRachContainer_gp[internalCellIndex] + containerTick;
			if (ULSCH_UCI_INVALID_INDEX !=
					tempPdcchULOutput_p->directIndexingUEInfo[pDciCceInfo->dciPduInfo.ueIndex].ulschUciInfoIndex)
			{
				ulSchUciInfo_p =
					&ulSchUciPoolInfo_p->ulschUciInfo[tempPdcchULOutput_p->directIndexingUEInfo[\
					pDciCceInfo->dciPduInfo.ueIndex].ulschUciInfoIndex];
				/* Call below function to convert UL Config PDU Type when RB Allocation fails 
				   in PDSCH and UL config needs to be send without Harq.*/
				convertULConfigPduTypeRBAllocFail(ulSchUciInfo_p);
			}
		}
#endif        
	}

	while (localCount--)
	{
		cceNumber = dciCCEContainerInfo_p->
			dciCCEInfoCmnChannel[localCount].cceIndex;
		pDciCceInfo = &(dciCCEContainerInfo_p->dciCCEInfo[cceNumber]);
		pDciPduInfo = &pDciCceInfo->dciPduInfo;
		/** SPR 8634 Changes Start **/
		if (pDciCceInfo->isCCEFree)
		{
			pDciCceInfo->msgType = INVALID_MSG;
			continue;
		}
		/** SPR 8634 Changes End **/

		LOG_UT(MAC_COMM_TX_ID,LOGDEBUG,MAC_L1_INF,\
				currentGlobalTick,\
				dciCCEContainerInfo_p->dciCCEInfoCmnChannel[localCount].msgType ,\
				pDciPduInfo->ueIndex,pDciPduInfo->harqProcessId,\
				pDciPduInfo->dciFormat,pDciPduInfo->cceIndex,\
				0.0,0.0,__func__,"COMM_DCI_MSG_TX");
		handleCommChControlMsg(dciCCEContainerInfo_p->dciCCEInfoCmnChannel[localCount].msgType,
				&pPrevPdu,&pNextPdu,pDciPduInfo,pDciCceInfo,&pduIndex,
				&numPdu,&length,&numPdschRnti,&numDci,
				&tempVendorSpecificForDLConfig,&numOfUEs
				,internalCellIndex);
	}

#ifdef TDD_CONFIG
	UInt8 isSpecialSubFrame = FALSE;
	isSpecialSubFrame = checkForSpecialSubframe(subFrameNum, internalCellIndex);
#endif   
	if(0 != cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->prsConfigInfo.prsSubframeCounter 
#ifdef TDD_CONFIG
        && (FALSE == isSpecialSubFrame)
#endif
            )
	{
		UInt8 prsMutingFlag = 
			cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->prsConfigInfo.prsMutingConfig\
			[cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
			cellParams_p->prsConfigInfo.prsOccasionCounter - 1];

		if(TRUE == prsMutingFlag) 
		{
			pPrevPdu = pNextPdu;
			pNextPdu = addPrsPduToDlConfig(pNextPdu,internalCellIndex);
			numPdu++;
			length += ((uintptr_t)pNextPdu - (uintptr_t)pPrevPdu);
		}
		cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->prsConfigInfo.prsSubframeCounter--;

		if (!cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->prsConfigInfo.prsSubframeCounter)
		{
			cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->prsConfigInfo.expiredPRSSubframeCounterTTI = getCurrentTick(); 
		}
	}
	/* Send the message to phy even if there is no PDU available */
	{
		LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"numpdu = %d\n", numPdu);
		/* Fill buffer header */
		/*OPT_SPR_4119_4120_4122_4123_CHANGES_START*/
        /* +- SPR 17777 */
		fillMacApiBufferHeader(msgBuffer,
				PHY_DL_CONFIG_REQUEST, length);
        /* +- SPR 17777 */
		/*OPT_SPR_4119_4120_4122_4123_CHANGES_END*/

		/* + TM7_EPRE */
		tempVendorSpecificForDLConfig_p = &tempVendorSpecificForDLConfig;
		if(tempVendorSpecificForDLConfig_p->numOfUEs)
		{
			fillVendorSpecificInfo(msgBuffer,tempVendorSpecificForDLConfig_p,&vendorSpLen);
		}
		/* - TM7_EPRE */

		/* Fill in all the PDU configurations */
		pDlConfigReq->sfnsf = MAC_PHY_CONVERT_16(sfnSf);
		/* + TM7_EPRE */
		pDlConfigReq->length = MAC_PHY_CONVERT_16(length + vendorSpLen);
		/* - TM7_EPRE */
		/* CLPC_REWORK CHG */
		pDlConfigReq->txPowerForPCFICH =  
			MAC_PHY_CONVERT_16(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
					cellParams_p->commonDLPowerControlInfo.pcfichPowerOffset);
		/* CLPC_REWORK_CHG END */
		pDlConfigReq->numDCI = numDci;
		/*+ Dyn UEs To Be SchPerTTI +*/
		if(numUEDci > 0)
		{
			LOG_MAC_MSG(MAC_DL_UE_TO_BE_SCHEDULED,\
					LOGBRIEF,\
					MAC_L1_INF,\
					getCurrentTick(),
					numUEDci,\
					0,0,0,0,0,0,\
					__func__,"");
		}
		/*- Dyn UEs To Be SchPerTTI -*/
		pDlConfigReq->numOfPDU = MAC_PHY_CONVERT_16(numPdu);
		pDlConfigReq->numOfPDSCHRNTI = numPdschRnti;

        ulCheckPDUnum=pDlConfigReq->numOfPDU-pDlConfigReq->numDCI;
		
#ifdef LTE_EMBMS_SUPPORTED
        if( isEmbmsValidSf_g[internalCellIndex] == TRUE)
        {
            pDlConfigReq->numOfPDSCHRNTI = 0;
        }
#endif

		/* SPR 1467 changes start */        
		/* SPR 1868 changes start */        
		if( (pDlConfigReq->numOfPDU) || (flagForDciFormat0) )
			/* SPR 1868 changes end */        
		{
            /*BUG:13001:start*/
            /* code removed */
            /*BUG:13001:end*/
			/* SPR 17737 fix start */
			/* code removed */
			/* SPR 17737 fix end */
#ifdef PERF_STATS
			/*  Increment the Counter for the  Number of subframes with\
			 *  PDCCH CFI set to 1 or 2 or 3 */
			/* + PERF_CA */
                /*++ SPR 19809 */
                        gMacCellPerfStats_p[internalCellIndex]->lteCellMacDLPerfStats.\
                                noOfSubFrameWithPdcchCfi[pDlConfigReq->cfi - 1] ++;
                /*-- SPR 19809 */
			/* - PERF_CA */
#endif
		}
/* SPR 17737 fix start */
        /*BUG:13001:start*/
        pDlConfigReq->cfi = currentCFIValue_g[internalCellIndex];
        /*BUG:13001:end*/    
        /* SPR 16028 FIX START */
#ifdef TDD_CONFIG
        if (currentCFIValue_g[internalCellIndex] == 0)
        {
            pDlConfigReq->cfi = 2;
        }
#endif
            /* SPR 16028 FIX END */

		/* SPR 1467 changes end */        

		LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"sfnsf = %d, length = %d\n", pDlConfigReq->sfnsf,
				pDlConfigReq->length);
		/* Fill in MCH PDUs - not to be done as of now */
/* SPR 19288 change start */
/* SPR 19288 change end */

#ifdef FLEXRAN
   g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_DL_CFG_REQ; 
#endif
	
       /* if((pDlConfigReq->numOfPDU==3) && (pDlConfigReq->numDCI==1)&&(pDlConfigReq->numOfPDSCHRNTI==1))
        {
           fprintf(stderr," length [%d] vendorSpLen [%d] Tick [%llu]\n", length,vendorSpLen, getCurrentTick());
        }*/
		/* Send data to Physical */
		/* + TM7_EPRE */
		if (0 >= sendDataToPhysical (msgBuffer, length + vendorSpLen + L1_API_HEADER_LEN,internalCellIndex) )
		{
			/* - TM7_EPRE */
			LOG_MAC_MSG(L2_SOCKET_SENDTO_FAIL_ID,LOGERROR,L2_SYS_FAIL,\
					getCurrentTick(),\
					__LINE__,0,0,0,0,\
					0.0,0.0,__FILE__,__func__);

			LTE_MAC_UT_LOG(LOG_MAJOR, MAC_MOD_TRANSMITTER,"Send data failed");
		}
	}
	/* SPR 8712 changes */
#if (defined(DL_DATA_SEPARATION) && defined(DL_UL_SPLIT))
#ifdef FDD_CONFIG
	eventFD_t tempfdPusch;
	eventfd_read_wrapper(ev_fd_pusch_g,&tempfdPusch);
#endif    
#endif    
	/* SPR 8712 changes */
#ifndef DL_UL_SPLIT_TDD
	/* Send the HI_DCI0.Request */
	txDLHiDci0Msg( phyDelay, 
			currentGlobalTick,
			sysFrame, 
			subFrame,
			internalCellIndex
		     );
#endif
#ifdef FDD_CONFIG
	prepareULConfigMsg (
			phyDelay, 
			currentGlobalTick, 
			sysFrame, 
			subFrame,
			internalCellIndex
			);
#endif                        
#ifdef DL_DATA_SEPARATION
	/* SPR 9399 Changes Start */
#ifndef DL_CONFIG_DATA_SYNC_DISABLED
	eventfd_write_wrapper(ev_fd_tx_Control_g[internalCellIndex],1);
#endif    
	/* SPR 9399 Changes End */
#endif    
	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Exit",__func__);
	return MAC_SUCCESS;
}
/* Coverity # 63409*/

/*****************************************************************************
 * Function Name  :  addDlschDciPduToDlData
 * Inputs         :  pDlDataPdu - Pointer to message buffer to fill in the
 *                                  PDU.
 *                   pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message,
 *                   pduIndex and tbIndex
 * Outputs        :  None 
 * Returns        :  Pointer to the buffer which can be used to fill in the
 *                   next PDU
 * Description    :  This function fills in the message with the DLSCH DCI PDU data
 *                   and returns the pointer to the buffer which can be used to
 *                   fill in the next PDU.
 *****************************************************************************/
#ifdef FLEXRAN
extern uint8_t macPhyBuffer[10000];
#endif
STATIC  FAPI_dlPduInfo_st *addDlschDciPduToDlData(
		FAPI_dlPduInfo_st *pDlDataPdu, DCIPduInfo *pDciPdu,
		UInt32 pduIndex, UInt8 tbIndex

/* SPR 19288 change start */
/* SPR 19288 change end */
#if defined(FLEXRAN)
        ,InternalCellIndex internalCellIndex
#endif
	)
{
#ifdef FLEXRAN
        unsigned char * targetMem = (unsigned char *)g_pSharedMemAddr[internalCellIndex][FAPI_SHMA_DL_BUFFER];
#endif
	FAPI_dlPduInfo_st       *pNextPdu = PNULL;
	FAPI_dlTLVInfo_st       *pDlTlv = PNULL;

/* SPR 19288 change start */
/* SPR 19288 change end */
#ifndef FLEXRAN
	UInt32                  tagLen = 0;
#endif
#ifndef FLEXRAN
	UInt32                  pduLen = sizeof(FAPI_dlPduInfo_st)
		- 1*sizeof(FAPI_dlTLVInfo_st);
#endif

	pDlDataPdu->pduIndex = MAC_PHY_CONVERT_16(pduIndex);

	pDlTlv = (FAPI_dlTLVInfo_st *)&pDlDataPdu->dlTLVInfo[0];
	pDlDataPdu->numOfTLV = MAC_PHY_CONVERT_32(0x0001);
	


#ifndef REAL_UE_ENVIORNMENT 
	/* DL Data Sep Changes */
	/* CAUTION : Used as rnti ; only in simulator enviornment - only for debugging purpose 
	   Please take care while updating tag in TLV */
	pDlTlv->tag = MAC_PHY_CONVERT_16(pDciPdu->rnti);
#else
/* SPR 19288 change start */
    pDlTlv->tag = MAC_PHY_CONVERT_16(0);/* Currently we only support val */
/*SPR 21001 Changes end */
/* SPR 19288 change end */
#endif
#ifdef FLEXRAN
	 pDlTlv->tag = MAC_PHY_CONVERT_16(1);
#endif

	if ( TB_1_INDEX == tbIndex)
	{
#ifndef FLEXRAN
		if (0 != (pDciPdu->tb1Info.tbSize % 4) )
		{
			/* tag length has to be a multiple of 4 */
			tagLen = (((pDciPdu->tb1Info.tbSize / 4) + 1) * 4);
		}
		else
		{
			tagLen = pDciPdu->tb1Info.tbSize;
		}
		pduLen += (offsetof(FAPI_dlTLVInfo_st, value)) + tagLen;
#endif
		pDlTlv->tagLen = MAC_PHY_CONVERT_16(pDciPdu->tb1Info.tbSize);
		

/* SPR 19288 change start */
#if !defined(FLEXRAN)
/*SPR 21001 Changes start */
		/* Purify Start*/
		memSet ( ( (UInt8 *)pDlTlv->value) + pDciPdu->tb1Info.tbSize, 
				0, tagLen - pDciPdu->tb1Info.tbSize);
		/* Purify End*/
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
		if (pDciPdu->dataTB_p!= NULL)
		{
/* SPR 19288 change start */
#ifdef FLEXRAN
       //msgExtract implementation for FLEXRAN
        targetMem = targetMem + dlDataOffset[internalCellIndex];
	msgExtract(pDciPdu->dataTB_p,DLSCH_PDU_OFFSET,targetMem ,pDciPdu->tb1Info.tbSize);
        pDlTlv->value = dlDataOffset[internalCellIndex];
        dlDataOffset[internalCellIndex] += pDciPdu->tb1Info.tbSize;

        if ((MODULO_EIGHT(dlDataOffset[internalCellIndex])) != 0)
        {
            dlDataOffset[internalCellIndex] += (MODULO_EIGHT(dlDataOffset[internalCellIndex]));
        }
#else
	msgExtract(pDciPdu->dataTB_p,DLSCH_PDU_OFFSET,(UInt8 *)pDlTlv->value,pDciPdu->tb1Info.tbSize);
#endif
/*SPR 21001 Changes end */
/* SPR 19288 change end */
		}
	}
	else
	{
#ifndef FLEXRAN
		if (0 != (pDciPdu->tb2Info.tbSize % 4) )
		{
			/* tag length has to be a multiple of 4 */
			tagLen = (((pDciPdu->tb2Info.tbSize / 4) + 1) * 4);
		}
		else
		{
			tagLen = pDciPdu->tb2Info.tbSize;
		}
		pduLen += (offsetof(FAPI_dlTLVInfo_st, value)) + tagLen;
#endif
		pDlTlv->tagLen = MAC_PHY_CONVERT_16(pDciPdu->tb2Info.tbSize);
		
/*SPR 21001 Changes start */
		/* Purify Start*/
#ifndef FLEXRAN
		memSet ( ( (UInt8 *)pDlTlv->value) + pDciPdu->tb2Info.tbSize, 
				0, tagLen - pDciPdu->tb2Info.tbSize);
/*SPR 21001 Changes end */
#endif
		/* Purify End*/
		/* SPR 3679 Changes Start */
		if (pDciPdu->dataTB2_p!= NULL)
		{
/* SPR 19288 change start */
#ifdef FLEXRAN
        /* msgExtract implemenatation for FLEXRAN */
        targetMem = targetMem + dlDataOffset[internalCellIndex];
	msgExtract(pDciPdu->dataTB2_p,DLSCH_PDU_OFFSET,targetMem ,pDciPdu->tb2Info.tbSize);
        pDlTlv->value = dlDataOffset[internalCellIndex];
        dlDataOffset[internalCellIndex] += pDciPdu->tb2Info.tbSize;

        if ((MODULO_EIGHT(dlDataOffset[internalCellIndex])) != 0)
        {
            dlDataOffset[internalCellIndex] += (MODULO_EIGHT(dlDataOffset[internalCellIndex]));
        }
#else
	msgExtract(pDciPdu->dataTB2_p,DLSCH_PDU_OFFSET,(UInt8 *)pDlTlv->value,pDciPdu->tb2Info.tbSize);
#endif
/*SPR 21001 Changes end */
/* SPR 19288 change end */
		}
	}
#ifdef FLEXRAN_FAPI_LOGGING
#if 1
    if(0 != flexranFapiLoggingEnable_g)
    {
	uint32_t debugOffset = 0;
	uint16_t dataLegnth = pDlTlv->tagLen > MAX_LOG_TB_DATA_SIZE ? MAX_LOG_TB_DATA_SIZE : pDlTlv->tagLen;

	FAPI_l1ApiMsg_st* pFapiHdr = (FAPI_l1ApiMsg_st*)macPhyBuffer;
	pFapiHdr->msgId = FAPI_DEBUG_DL_DATA;
	pFapiHdr->lenVendorSpecific = 0;
	pFapiHdr->msgLen = sizeof(fapi_debug_t) + dataLegnth;
	debugOffset += 4;

	fapi_debug_t* pDebugHdr = (fapi_debug_t*)(macPhyBuffer + debugOffset);
	pDebugHdr->frameNum = sysFrameNum_flexran[internalCellIndex];
	pDebugHdr->subframeNum = subFrameNum_flexran[internalCellIndex];
	pDebugHdr->crnti = pDciPdu->rnti;
	pDebugHdr->pduIdx = 0; // fill this if information is available
	pDebugHdr->handle = pDlTlv->value;
	debugOffset += sizeof(fapi_debug_t);

	uint8_t* pDebugInfo = (uint8_t*)(macPhyBuffer + debugOffset);
	memcpy(pDebugInfo, targetMem, dataLegnth);
	do_tap_fun((void*)pFapiHdr, pFapiHdr->msgLen + 4, internalCellIndex);
    }
#endif	
#endif
#ifdef FLEXRAN
	pDlDataPdu->pduLen = MAC_PHY_CONVERT_16(16);
#else
	pDlDataPdu->pduLen = MAC_PHY_CONVERT_16(pduLen);
#endif
	pNextPdu = (FAPI_dlPduInfo_st *)(((UInt8 *)pDlDataPdu)
#ifdef FLEXRAN
			+ pDlDataPdu->pduLen);
#else
			+ pduLen);
#endif

	return pNextPdu;
}

/*****************************************************************************
 * Function Name  :  addBchPduToDlData
 * Inputs         :  pDlDataPdu - Pointer to message buffer to fill in the
 *                                PDU.
 *                   pDciCce - Pointer to DciCCEContainerInfo and
 *                   pduIndex - Index of PDU
 * Outputs        :  None
 * Returns        :  Pointer to the buffer which can be used to fill in the
 *                   next PDU
 * Description    :  This function fills in the message with the BCH PDU data
 *                   and returns the pointer to the buffer which can be used to
 *                   fill in the next PDU.
 *****************************************************************************/
STATIC  FAPI_dlPduInfo_st *addBchPduToDlData(
		FAPI_dlPduInfo_st *pDlDataPdu, DciCCEContainerInfo *pDciCce,
		UInt32 pduIndex
/* SPR 19288 change start */
/* SPR 19288 change end */
#ifdef FLEXRAN
	, InternalCellIndex internalCellIndex
#endif
                                                   )

{
#ifdef FLEXRAN
        unsigned char * targetMem = (unsigned char *)g_pSharedMemAddr[internalCellIndex][FAPI_SHMA_DL_BUFFER];
#endif
	FAPI_dlPduInfo_st       *pNextPdu = PNULL;
	FAPI_dlTLVInfo_st       *pDlTlv = PNULL;
/* SPR 19288 change start */
#ifndef FLEXRAN
	UInt32                  tagLen = 0;
#endif
	UInt16                  pduLen = 0;
	pDlDataPdu->pduIndex = MAC_PHY_CONVERT_16(pduIndex);
	pDlDataPdu->numOfTLV = MAC_PHY_CONVERT_32(1); /* 1 BCH MAC PDU data */

	pDlTlv = (FAPI_dlTLVInfo_st *)&pDlDataPdu->dlTLVInfo[0];
/* SPR 19288 change start */
#if defined(FLEXRAN)
/*SPR 21001 Changes start */
        pDlTlv->tag = MAC_PHY_CONVERT_16(1);
#else
	pDlTlv->tag = MAC_PHY_CONVERT_16(0); /* Currently we only support value */
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
#ifndef FLEXRAN
	if (0 != (pDciCce->mibDataLen % 4) )
	{
		/* tag length has to be a multiple of 4 */
		tagLen = (((pDciCce->mibDataLen / 4) + 1) * 4);
	}
	else
	{
		tagLen = pDciCce->mibDataLen;
	}
#endif
	pDlTlv->tagLen = MAC_PHY_CONVERT_16(pDciCce->mibDataLen);
/* SPR 19288 change start */
#if !defined(FLEXRAN)
/*SPR 21001 Changes start */
	/* Purify Start*/
	memSet ( ( (UInt8 *)pDlTlv->value) + pDciCce->mibDataLen, 
			0, tagLen - pDciCce->mibDataLen);
	/* Purify End*/
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
/* SPR 19288 change start */
#ifdef FLEXRAN
        /* msgExtract implementation for FLEXRAN */
        targetMem = targetMem + dlDataOffset[internalCellIndex];
	msgExtract(pDciCce->mibData_p,BCH_PDU_OFFSET,targetMem ,pDciCce->mibDataLen);
        pDlTlv->value = dlDataOffset[internalCellIndex];
        dlDataOffset[internalCellIndex] += pDciCce->mibDataLen;

        if ((MODULO_EIGHT(dlDataOffset[internalCellIndex])) != 0)
        {
            dlDataOffset[internalCellIndex] += (MODULO_EIGHT(dlDataOffset[internalCellIndex]));
        }
#else
	msgExtract(pDciCce->mibData_p, BCH_PDU_OFFSET, (UInt8 *)pDlTlv->value, pDciCce->mibDataLen);
#endif
/* SPR 19288 change end */
	/* SPR 1391 Start*/

	/*
	 * pDlDataPdu->pduLen = MAC_PHY_CONVERT_16((sizeof(FAPI_dlPduInfo_st)
	 - 1*sizeof(uint32_t))
	 + (sizeof(FAPI_dlTLVInfo_st) - 1*sizeof(uint32_t))
	 + tagLen);
	 */
#ifdef FLEXRAN_FAPI_LOGGING
#if 1
    if(0 != flexranFapiLoggingEnable_g)
    {
               uint32_t debugOffset = 0;
	       uint16_t dataLegnth = pDlTlv->tagLen > MAX_LOG_TB_DATA_SIZE ? MAX_LOG_TB_DATA_SIZE : pDlTlv->tagLen;

                FAPI_l1ApiMsg_st* pFapiHdr = (FAPI_l1ApiMsg_st*)macPhyBuffer;
                pFapiHdr->msgId = FAPI_DEBUG_DL_DATA;
                pFapiHdr->lenVendorSpecific = 0;
                pFapiHdr->msgLen = sizeof(fapi_debug_t) + dataLegnth;
                debugOffset += 4;

                fapi_debug_t* pDebugHdr = (fapi_debug_t*)(macPhyBuffer + debugOffset);
		pDebugHdr->frameNum = sysFrameNum_flexran[internalCellIndex];
		pDebugHdr->subframeNum = subFrameNum_flexran[internalCellIndex];
                pDebugHdr->crnti = 0;
                pDebugHdr->pduIdx = 0; // fill this if information is available
                pDebugHdr->handle = pDlTlv->value;
                debugOffset += sizeof(fapi_debug_t);

                uint8_t* pDebugInfo = (uint8_t*)(macPhyBuffer + debugOffset);
                memcpy(pDebugInfo, targetMem, dataLegnth);

                do_tap_fun((void*)pFapiHdr, pFapiHdr->msgLen + 4, internalCellIndex);
    }
#endif	
#endif

#ifndef FLEXRAN
	pduLen = (sizeof(FAPI_dlPduInfo_st) - 1*sizeof(uint32_T)) + tagLen;
#else
        pduLen = 16; /* Fixed size for FLEXRAN*/
#endif

	pDlDataPdu->pduLen = MAC_PHY_CONVERT_16(pduLen);

	/* SPR 1391 End*/
/* SPR 19288 change start */
        pNextPdu = (FAPI_dlPduInfo_st *)(((UInt8 *)pDlDataPdu)
                        + pduLen);
/*SPR 21001 Changes end */
/* SPR 19288 change end */

	return pNextPdu;
}
/* + coverity 42176 */
#ifdef MAC_AUT_TEST
/*****************************************************************************
 * Function Name  :  addDlschPduToDlData
 * Inputs         :  pDlDataPdu - Pointer to message buffer to fill in the
 *                                  PDU,
 *                   pDciPdu - Pointer to DCI PDU containing the info to create and
 *                   pduIndex - Index of PDU
 * Outputs        :  None
 * Returns        :  Pointer to the buffer which can be used to fill in the
 *                   next PDU
 * Description    :  This function fills in the message with the DLSCH PDU data
 *                   and returns the pointer to the buffer which can be used to
 *                   fill in the next PDU.
 *****************************************************************************/
STATIC  FAPI_dlPduInfo_st *addDlschPduToDlData(
		FAPI_dlPduInfo_st *pDlDataPdu, DCIPduInfo *pDciPdu,
		UInt32 pduIndex)
{
	FAPI_dlPduInfo_st       *pNextPdu = PNULL;
	FAPI_dlTLVInfo_st       *pDlTlv = PNULL;
	UInt32                  tagLen = 0;
	UInt16                  pduLen = 0;
	pDlDataPdu->pduIndex = MAC_PHY_CONVERT_16(pduIndex);
	pDlDataPdu->numOfTLV = MAC_PHY_CONVERT_32(1); /* 1 BCH MAC PDU data */

	pDlTlv = (FAPI_dlTLVInfo_st *)&pDlDataPdu->dlTLVInfo[0];

	pDlTlv->tag = MAC_PHY_CONVERT_16(0); /* Currently we only support value */
	if (0 != (pDciPdu->tb1Info.tbSize % 4) )
	{
		/* tag length has to be a multiple of 4 */
		tagLen = (((pDciPdu->tb1Info.tbSize / 4) + 1) * 4);
	}
	else
	{
		tagLen = pDciPdu->tb1Info.tbSize;
	}
	pDlTlv->tagLen = MAC_PHY_CONVERT_16(tagLen);
	msgExtract(pDciPdu->dataTB_p, DLSCH_PDU_OFFSET, (UInt8 *)pDlTlv->value, pDciPdu->tb1Info.tbSize);

	pduLen = ((sizeof(FAPI_dlPduInfo_st)
				- 1*sizeof(FAPI_dlTLVInfo_st))
			+ (sizeof(FAPI_dlTLVInfo_st) - 1*sizeof(FAPI_dlTLVInfo_st))
			+ tagLen);
	pDlDataPdu->pduLen = MAC_PHY_CONVERT_16(pduLen);

	pNextPdu = (FAPI_dlPduInfo_st *)(((UInt8 *)pDlDataPdu) + pduLen);

	return pNextPdu;
}
#endif
/* - coverity 42176 */

/*****************************************************************************
 * Function Name  :  addPchPduToDlData
 * Inputs         :  pDlDataPdu - Pointer to message buffer to fill in the
 *                                  PDU,
 *                   pDciPdu - Pointer to DCI PDU containing the info to create and
 *                   pduIndex - Index of PDU
 * Outputs        :  None
 * Returns        :  Pointer to the buffer which can be used to fill in the
 *                   next PDU
 * Description    :  This function fills in the message with the PCH PDU data
 *                   and returns the pointer to the buffer which can be used to
 *                   fill in the next PDU.
 *****************************************************************************/
STATIC  FAPI_dlPduInfo_st *addPchPduToDlData(
		FAPI_dlPduInfo_st *pDlDataPdu, DCIPduInfo *pDciPdu,
		UInt32 pduIndex
/* SPR 19288 change start */
/* SPR 19288 change end */
#ifdef FLEXRAN
	, InternalCellIndex internalCellIndex
#endif
                                                  )

{
#ifdef FLEXRAN
        unsigned char * targetMem = (unsigned char *)g_pSharedMemAddr[internalCellIndex][FAPI_SHMA_DL_BUFFER];
#endif
	FAPI_dlPduInfo_st       *pNextPdu = PNULL;
	FAPI_dlTLVInfo_st       *pDlTlv = PNULL;
	UInt32                  tagLen = 0;
	UInt16                  pduLen = 0;  
/* SPR 19288 change start */
/* SPR 19288 change end */
	pDlDataPdu->pduIndex = MAC_PHY_CONVERT_16(pduIndex);
/*SPR 21001 Changes end */
	pDlDataPdu->numOfTLV = MAC_PHY_CONVERT_32(1); /* 1 BCH MAC PDU data */

	pDlTlv = (FAPI_dlTLVInfo_st *)&pDlDataPdu->dlTLVInfo[0];
/* SPR 19288 change start */
/* SPR 19288 change end */
       pDlTlv->tag = MAC_PHY_CONVERT_16(0); /* Currently we only support value */
/*SPR 21001 Changes end */
#ifdef FLEXRAN
	pDlTlv->tag = MAC_PHY_CONVERT_16(1); /* Currently we only support value */
#else
	pDlTlv->tag = MAC_PHY_CONVERT_16(0); /* Currently we only support value */
	if (0 != (pDciPdu->tb1Info.tbSize % 4) )
	{
		/* tag length has to be a multiple of 4 */
		tagLen = (((pDciPdu->tb1Info.tbSize / 4) + 1) * 4);
	}
	else
	{
		tagLen = pDciPdu->tb1Info.tbSize;
	}
#endif
	pDlTlv->tagLen = MAC_PHY_CONVERT_16(pDciPdu->tb1Info.tbSize);
	/* Purify Start*/
#ifndef FLEXRAN
/*SPR 21001 Changes start */
	memSet ( ( (UInt8 *)pDlTlv->value) + pDciPdu->tb1Info.tbSize, 
			0, tagLen - pDciPdu->tb1Info.tbSize);
/*SPR 21001 Changes end */
#endif
	/* Purify End*/

/* SPR 19288 change start */
/*SPR 21001 Changes start */
#ifdef FLEXRAN
       /* msgExtract implementation for FLEXRAN */
        targetMem = targetMem + dlDataOffset[internalCellIndex];
	msgExtract(pDciPdu->dataTB_p,PCH_PDU_OFFSET,targetMem ,pDciPdu->tb1Info.tbSize);
        pDlTlv->value = dlDataOffset[internalCellIndex];
        dlDataOffset[internalCellIndex] += pDciPdu->tb1Info.tbSize;

        if ((MODULO_EIGHT(dlDataOffset[internalCellIndex])) != 0)
        {
            dlDataOffset[internalCellIndex] += (MODULO_EIGHT(dlDataOffset[internalCellIndex]));
        }
#else
	msgExtract(pDciPdu->dataTB_p, PCH_PDU_OFFSET, (UInt8 *)pDlTlv->value, pDciPdu->tb1Info.tbSize);
#endif
/*SPR 21001 Changes end */

/*#ifndef FLEXRAN
        pduLen = 16;  //Fixed size for FLEXRAN
#else*/
	pduLen = ((sizeof(FAPI_dlPduInfo_st)
				- 1*sizeof(FAPI_dlTLVInfo_st))
			+ offsetof(FAPI_dlTLVInfo_st, value)
			+ tagLen);
// #endif
#ifdef FLEXRAN_FAPI_LOGGING
#if 1
    if(0 != flexranFapiLoggingEnable_g)
    {
               uint32_t debugOffset = 0;
	       uint16_t dataLegnth = pDlTlv->tagLen > MAX_LOG_TB_DATA_SIZE ? MAX_LOG_TB_DATA_SIZE : pDlTlv->tagLen;

                FAPI_l1ApiMsg_st* pFapiHdr = (FAPI_l1ApiMsg_st*)macPhyBuffer;
                pFapiHdr->msgId = FAPI_DEBUG_DL_DATA;
                pFapiHdr->lenVendorSpecific = 0;
                pFapiHdr->msgLen = sizeof(fapi_debug_t) + dataLegnth;
                debugOffset += 4;

                fapi_debug_t* pDebugHdr = (fapi_debug_t*)(macPhyBuffer + debugOffset);
		pDebugHdr->frameNum = sysFrameNum_flexran[internalCellIndex];
		pDebugHdr->subframeNum = subFrameNum_flexran[internalCellIndex];
                pDebugHdr->crnti = pDciPdu->rnti;
                pDebugHdr->pduIdx = 0; // fill this if information is available
                pDebugHdr->handle = pDlTlv->value;
                debugOffset += sizeof(fapi_debug_t);

                uint8_t* pDebugInfo = (uint8_t*)(macPhyBuffer + debugOffset);
                memcpy(pDebugInfo, targetMem, dataLegnth);

                do_tap_fun((void*)pFapiHdr, pFapiHdr->msgLen + 4, internalCellIndex);
    }
#endif	
#endif

	pDlDataPdu->pduLen = MAC_PHY_CONVERT_16(pduLen);
/* SPR 19288 change end */
	pNextPdu = (FAPI_dlPduInfo_st *)(((UInt8 *)pDlDataPdu) + pduLen);
	return pNextPdu;
}
#ifdef FAPI_4_0_COMPLIANCE
/*****************************************************************************
 * Function Name  :  fillHiPduRelease10Param 
 * Inputs         :   pDlHiPdu - Pointer to message buffer to fill in the
 *                                  PDU.
 * Returns        :  Void 
 * Description    :  This fucntion fills in the message with the HI Rel10 data
 *****************************************************************************/

void fillHiPduRelease10Param(FAPI_dlHiPduInfo_st *pDlHiPdu)
{
	pDlHiPdu->release10Param.hiValue2 = HI_ACK;
	pDlHiPdu->release10Param.flag_TB2=DEFAULT_FLAG_TB2;

}
#endif
/*****************************************************************************
 * Function Name  :  addHiPduToHiDci0
 * Inputs         :  pDlHiPdu - Pointer to structure FAPI_dlHiPduInfo_st,
 *                   pHiPdu - Pointer to HIPduArray
 *                   internalCellIndex - cell id  
 * Outputs        :  None
 * Returns        :  Pointer to the buffer which can be used to fill in the
 *                   next PDU
 * Description    :  This fucntion fills in the message with the HI DCI PDU data
 *                   and returns the pointer to the buffer which can be used to
 *                   fill in the next PDU.
 *****************************************************************************/
STATIC  FAPI_dlHiPduInfo_st *addHiPduToHiDci0(
		FAPI_dlHiPduInfo_st *pDlHiPdu, HIPduArray *pHiPdu
		,InternalCellIndex internalCellIndex)
{
	/* CR changes start */
#ifdef TDD_CONFIG
	UInt8 cqiValue = 0;
	UInt16 txPowerOffset = 0;
#endif
	/* CR changes end */
	FAPI_dlHiPduInfo_st     *pNextPdu = PNULL;
	/* FDD_Warning_Fix */
#ifdef TDD_CONFIG
	UInt16 ueIndex = 0;
	DLUEContext * dlUEContext_p = PNULL;
#endif
	/* FDD_Warning_Fix */
	MacHIPduInfo* pMacHiPdu = &(pHiPdu->macHIPduInfo[pHiPdu->count - 1]);
	RNTIInfo *rntiInfo_p        = PNULL;

	pDlHiPdu->pduType = FAPI_HI_PDU;
	pDlHiPdu->rbStart = pMacHiPdu->rbStart; 
	pDlHiPdu->cyclicShift2_forDMRS = pMacHiPdu->nDmrs;
	pDlHiPdu->hiValue = pMacHiPdu->hiValue;
	/*TDD Config 0 Changes Start*/     
#ifdef TDD_CONFIG
	/*For TDD config 0 the value of IPhich is 1 corresponding to UL Tx in
	 * subframe 4 and 9 else it is 0*/
	pDlHiPdu->iPHICH = pMacHiPdu->iPhich;
#else
	pDlHiPdu->iPHICH = 0; /* Not needed in FDD mode */
#endif    
	/*TDD Config 0 Changes End*/     
	pDlHiPdu->hipduSize = sizeof(FAPI_dlHiPduInfo_st);

	/* CLPC_CHG */
	rntiInfo_p = getUeIdxFromRNTIMap (pMacHiPdu->crnti,internalCellIndex);
	if (PNULL != rntiInfo_p)
	{
		/* FDD_Warning_Fix */
#ifdef TDD_CONFIG
		ueIndex = rntiInfo_p->index;
		dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
		/*BUG:10658:start*/
		if (PNULL != dlUEContext_p)
		{
			/*BUG:10658:end*/
#endif
			/* FDD_Warning_Fix */
			/* CR changes start */
#ifdef FDD_CONFIG
			pDlHiPdu->txPower = pMacHiPdu->txPower;
#elif TDD_CONFIG
			/* SPR 9511 changes start */
			if (dlUEContext_p && dlUEContext_p->dlCQIInfo.isWideBandReportAvailable)
				/* SPR 9511 changes end */
			{
				if (((TX_MODE_3 == dlUEContext_p->dlMIMOInfo.transmissionMode) ||
							(TX_MODE_4 == dlUEContext_p->dlMIMOInfo.transmissionMode) || 
							(TX_MODE_8 == dlUEContext_p->dlMIMOInfo.transmissionMode)) &&
						(dlUEContext_p->dlCQIInfo.widebandCQICodeWordOne >
						 dlUEContext_p->dlCQIInfo.widebandCQICodeWordTwo))
				{ 
					cqiValue = dlUEContext_p->dlCQIInfo.widebandCQICodeWordTwo;
				}
				else
				{
					cqiValue = dlUEContext_p->dlCQIInfo.widebandCQICodeWordOne;
				}

				txPowerOffset = getPHICHPowerOffset(cqiValue,internalCellIndex);
			}
			else 
			{
				txPowerOffset = 0;
			}

			pDlHiPdu->txPower = txPowerOffset +  
				cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
				cellParams_p->commonDLPowerControlInfo.phichPowerOffset;
#endif
			/* CR changes end */
			/* SPR 3267 START */
			if (pDlHiPdu->txPower > MAX_TX_POWER_OFFSET)
			{
				pDlHiPdu->txPower = MAX_TX_POWER_OFFSET;
			}
			/* SPR 3267 END */
			pDlHiPdu->txPower = MAC_PHY_CONVERT_16(pDlHiPdu->txPower);
#ifdef TDD_CONFIG
			/*BUG:10658:start*/
		}
		else
		{
			LOG_MAC_MSG(MAC_DATA_TX_ID3,LOGBRIEF,MAC_L1_INF,\
					getCurrentTick(),\
					ueIndex,\
					rntiInfo_p->rntiFlag,\
					0,\
					0,\
					0,\
					0.0,0.0,__func__,"");

		}
		/*BUG:10658:end*/
#endif
	}
	/* CLPC_CHG END */

	pHiPdu->count--;
#ifdef FAPI_4_0_COMPLIANCE
	fillHiPduRelease10Param(pDlHiPdu);
#endif

	pNextPdu = (FAPI_dlHiPduInfo_st *)(((UInt8 *)pDlHiPdu)
			+ sizeof(FAPI_dlHiPduInfo_st)  );

	return pNextPdu;
}
#ifdef FAPI_4_0_COMPLIANCE
/*****************************************************************************
 * Function Name  :  fillULDciRelease10Param 
 * Inputs         :  pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message.
 *                   tempDciPdu - pointer to temporary dci pdu.
 * Outputs        : None
 * Returns        :  void
 * Description    :  This function fills in Release 10 Parameters in DCI UL PDU
 *****************************************************************************/
void fillULDciRelease10Param( FAPI_dlDCIPduInfo_st *pDciPdu
		/** SPR 11161 Fix : Start ***/
		,DCIPduInfo *tempDciPdu
		/** SPR 11161 Fix : End ***/
		/*SPR 13892 fix start*/
		,InternalCellIndex internalCellIndex
	/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
		,ULHarqInfo* ulHarqProcessInfo_p
#endif
	/* SPR 21958 PUSCH RAT1 Support End */
		)
/*SPR 13892 fix end*/
{
	/*SPR 13892 fix start*/
	UInt8 NULRB;
	UInt8 NDLRB;
	NDLRB=cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
	      cellParams_p->dlAvailableRBs;
	NULRB=cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
	      cellParams_p->ulAvailableRBs;
	/*SPR 13892 fix end*/

	pDciPdu->release10Param.crossCarrierScheduling = DEFAULT_CROSSCARRIER_SCHED_FLAG  ;
	pDciPdu->release10Param.carrierIndicator = DEFAULT_CARRIER_INDICATOR ;
	/** SPR 11161 Fix : Start ***/
	pDciPdu->release10Param.sizeOfCqiCsiField = tempDciPdu->csiFieldSize;
	/** SPR 11161 Fix : End ***/
	pDciPdu->release10Param.srsFlag = DEFAULT_SRS_FLAG ;
	pDciPdu->release10Param.srsRequest = DEFAULT_SRS_REQUEST  ;
	/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
    /* SPR 22317 Fix Start */
	if((PNULL != ulHarqProcessInfo_p) && (TRUE == ulHarqProcessInfo_p->RAT1Allocation))
    /* SPR 22317 Fix End */
	{
		pDciPdu->release10Param.resourceAllocationType = RESOURCE_ALLOC_TYPE_ONE; 	
		pDciPdu->release10Param.rbCoding = tempDciPdu->rbCodingForPUSCHRAT1;
		pDciPdu->release10Param.resourceAllocationFlag = DEFAULT_RESOURCE_ALLOC_FLAG;
	}
	else
	{
#endif
		/* SPR 21958 PUSCH RAT1 Support End */

		pDciPdu->release10Param.resourceAllocationType = DEFAULT_RESOURCE_ALLOC_TYPE ;
		pDciPdu->release10Param.rbCoding = MAC_PHY_CONVERT_32(DEFAULT_RESOURCE_BLOCK_CODING) ;
	/*SPR 13892 fix start*/
	if(NULRB<=NDLRB)
	{    
		pDciPdu->release10Param.resourceAllocationFlag = DEFAULT_RESOURCE_ALLOC_FLAG;
	}
	else
	{
		pDciPdu->release10Param.resourceAllocationFlag = 0;
	}
	/*SPR 13892 fix end*/
		/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
	}
#endif
	/* SPR 21958 PUSCH RAT1 Support End */

	pDciPdu->release10Param.mcs_2 = DEFAULT_MCS2;
	pDciPdu->release10Param.newDataIndicator_2 = DEFAULT_NEW_DATAINDICATION2 ;
	pDciPdu->release10Param.tpmi = DEFAULT_TPMI ;
	pDciPdu->release10Param.numAntennaPorts=DEFAULT_NUM_ANTENNA_PORTS;

}
/*****************************************************************************
 * Function Name  :  fillDciFormat0PduR10
 * Inputs         :  pDciMsg - Pointer to message buffer to fill in the PDU.
 *                :  pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message.
 *                   subFrameNum,dci0ForTempUEFlag,msgType,
 *                   internalCellIndex - cell id  
 * Outputs        : None
 * Returns        :  void
 * Description    :  This function fills in the message and will be used in case PHY Supports 
 *                     Release 10 
 *****************************************************************************/
 void fillDciFormat0PduR10(
#ifdef TDD_CONFIG
        FAPI_dlDCIPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu,UInt32 dci0ForTempUEFlag,InternalCellIndex internalCellIndex)
#elif FDD_CONFIG
/* +- SPR 17777 */
    FAPI_dlDCIPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu,
#if defined(UTFWK_SIMULATION) || defined(ULHARQ_TIMER_PROC) /*SPR 18979 +-*/
    UInt32 subFrameNum,
#endif
    UInt32 dci0ForTempUEFlag,MsgType msgType
    ,InternalCellIndex internalCellIndex)
    /* +- SPR 17777 */
#endif

{
#ifdef ULHARQ_TIMER_PROC
/*GCC warning fix+*/    
#ifdef LOG_PRINT_ENABLED    
    /* SPR 15909 fix start */
    tickType_t curTTI = getCurrentTick();
    /* SPR 15909 fix end */
#endif    
/*GCC warning fix-*/    
#endif

    ULUEContextInfo *ueContextInfo_p = PNULL;
    UInt16 ueIndex;
    //UInt16 ueIndex;
    UInt8 tempHarqProcessId  = pDciPdu->harqProcessId;
	/*SPR_3305_FIX_START*/
	ULHarqInfo* ulHarqProcessInfo_p            = PNULL;
	// ULUEContextInfo *ueContextInfo_p = PNULL;
#ifdef FDD_CONFIG
    /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
    /* SPR 20012 Fix Start */
	UInt8  tempToggleVal=0;
    /* SPR 20012 Fix End */
    /* SPR 19679: TDD HARQ Multiplexing  Changes End */
	TempCRNTICtx *tempUEContext_p = PNULL;
    /* + SPS_TDD_Changes */
    UInt32 rntiType =  getRntiTypeForRnti(pDciPdu->rnti,internalCellIndex);
    /* - SPS_TDD_Changes */
#endif
	/*SPR_3305_FIX_END*/
	ueIndex = pDciPdu->ueIndex;
	/*SPR_3305_FIX_START*/

	tempHarqProcessId = pDciPdu->harqProcessId;
#ifdef FDD_CONFIG 
	if(TRUE == dci0ForTempUEFlag)
	{
		tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].tempCRNTICtx_p;

		if (PNULL == tempUEContext_p)
		{
			return;
		}

		if(INVALID_HARQ_ID != tempHarqProcessId)
		{    
			ulHarqProcessInfo_p = &(tempUEContext_p->ulHarq);
		}    

	}
	else
        /* +- SPR 17777 */
#else
  LTE_GCC_UNUSED_PARAM(dci0ForTempUEFlag)
    /* +- SPR 17777 */
#endif
	{
		ueContextInfo_p = &ulUECtxInfoArr_g[ueIndex];

		if (PNULL == ueContextInfo_p->ulUEContext_p)
		{
			return;
		}

		if(INVALID_HARQ_ID != tempHarqProcessId)
		{
			ulHarqProcessInfo_p = 
				&(ueContextInfo_p->ulUEContext_p->ulresInfo[tempHarqProcessId]);
		}
	}

	/* SPR 2236 Begins*/
	/*SPR_3305_FIX_END*/

	/* SPR 2236 Ends */
	/* SPR 1859 FIX begins */
	pDciMsg->mcs = pDciPdu->tb1Info.modulationType;
	/* SPR 1859 FIX ends */

	pDciMsg->aggLevel = pDciPdu->aggregationLvl;
	pDciMsg->rnti = MAC_PHY_CONVERT_16(pDciPdu->rnti);
	pDciMsg->rbStart = pDciPdu->startRBIndex;
	pDciMsg->numOfRB = pDciPdu->numOfRBs;
	pDciMsg->cyclicShift2_forDMRS = pDciPdu->nDmrs;
	pDciMsg->freqEnabledFlag = 0;
	pDciMsg->freqHoppingBits = 0;
	pDciMsg->ueTxAntennaSelection = 0; /* not configured */
	pDciMsg->tpc = pDciPdu->tpc;
	pDciMsg->cqiRequest = pDciPdu->cqiRequest;

	/* CLPC_REWORK CHG */
	pDciMsg->txPower = MAC_PHY_CONVERT_16(pDciPdu->txPower);
	/* CLPC_REWORK_CHG END */
	/*SPR 13841 Fix start*/
#ifdef TDD_CONFIG
	/*For TDD Config 0 the value of ulIndex can be 1,2 or 3 depending on the U
	 * frame on which the data is to be transmitted.For other TDD configs the
	 * value is 0*/
	pDciMsg->ulIndex = pDciPdu->ulIndex;
	pDciMsg->newDataIndication = pDciPdu->tb1Info.txIndicator;
	/* Resetting ulIndex to 0 */
	pDciPdu->ulIndex = 0;
	pDciMsg->dlAssignmentIndex = pDciPdu->dai;
#elif FDD_CONFIG
	pDciMsg->ulIndex = 0; 
	pDciMsg->dlAssignmentIndex = 0;
#endif
	/*SPR 13841 Fix end*/


	if(INVALID_HARQ_ID != tempHarqProcessId)
	{
		if(NEW_TX  == pDciPdu->tb1Info.txIndicator)
		{
            /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
            /* SPR 20012 Fix Start */
#ifdef FDD_CONFIG
			tempToggleVal = ulHarqProcessInfo_p->txIndicator;
			ulHarqProcessInfo_p->txIndicator = tempToggleVal ^ NEW_TX;
#endif
            /* SPR 20012 Fix End */
            /* SPR 19679: TDD HARQ Multiplexing  Changes End */
#ifdef UTFWK_SIMULATION
#ifdef FDD_CONFIG
			UInt32 rntiType =  getRntiTypeForRnti(pDciPdu->rnti,internalCellIndex);
			processGrant(pDciPdu->ueIndex,pDciPdu->harqProcessId);
			addEntryInCrcIndList(pDciPdu->ueIndex,
					subFrameNum, 
					tempHarqProcessId, rntiType);
#endif /* FDD_CONFIG */
#endif /* UTFWK_SIMULATION */
		}

		/* no need to toggle for retransmission */

		pDciMsg->newDataIndication = ulHarqProcessInfo_p->txIndicator;    

		pDciMsg->mcs =  pDciPdu->tb1Info.modulationType;

	}        
	else
	{
		LTE_MAC_UT_LOG(LOG_WARNING,MAC_MOD_TRANSMITTER,\
				"INVALID HARQID %d\n",tempHarqProcessId);
	}
#ifdef FDD_CONFIG    
	/* SPS changes start */   
	if (FAPI_SPS_CRNTI == rntiType)
	{
		if (SPS_ACTIVATION == msgType)
		{
			pDciMsg->tpc = 0;
			pDciMsg->cyclicShift2_forDMRS = 0;
			pDciMsg->newDataIndication = 0;   //v_g 
		}
		else if (SPS_DEACTIVATION == msgType)
		{
			pDciMsg->tpc = 0;
			pDciMsg->cyclicShift2_forDMRS = 0;
			pDciMsg->mcs = 31;
			pDciMsg->rbStart = 0;
			pDciMsg->numOfRB = 0;
			pDciMsg->newDataIndication = 0;    
		}
	}
#endif   
	/* SPS changes end */   

    LOG_MAC_MSG(MAC_DCI0_TX_ID,LOGBRIEF,MAC_L1_INF,
            curTTI,
            pDciPdu->ueIndex,pDciPdu->harqProcessId,
            pDciMsg->newDataIndication,
            pDciMsg->mcs,
            pDciMsg->numOfRB,
            0.0,0.0,__func__,"DCI0_PDU_TX");

	pDciPdu->harqProcessId=INVALID_HARQ_ID;
	/* + SPR 5551 */
	pDciPdu->cqiRequest = 0;
	/* - SPR 5551 */
	/** SPR 11161 Fix : Start ***/
	/*SPR 13892 fix start*/
	fillULDciRelease10Param( pDciMsg,pDciPdu,internalCellIndex
	/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
			,ulHarqProcessInfo_p
#endif
	/* SPR 21958 PUSCH RAT1 Support End */
			);
	/*SPR 13892 fix end*/
	/** SPR 11161 Fix : End ***/
}
#endif /*FAPI*/
/*****************************************************************************
 * Function Name  :  fillDciFormat0Pdu
 * Inputs         :  pDciMsg - Pointer to message buffer to fill in the PDU,
 *                :  pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message.
 *                   subFrameNum,
 *                   dci0ForTempUEFlag,
 *                   msgType,
 *                   internalCellIndex
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function fills in the message with DCI Format 0 data.
 *****************************************************************************/
 void fillDciFormat0Pdu(
#ifdef TDD_CONFIG
        FAPI_dlDCIPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu,UInt32 dci0ForTempUEFlag,
#elif FDD_CONFIG
        /* +- SPR 17777 */
        FAPI_dlDCIPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu,
#if defined(UTFWK_SIMULATION) || defined(ULHARQ_TIMER_PROC)
        UInt32 subFrameNum,
#endif
        UInt32 dci0ForTempUEFlag,MsgType msgType,
        /* +- SPR 17777 */
#endif
        InternalCellIndex internalCellIndex)
{
#ifdef FDD_CONFIG
    ULUEContextInfo *ueContextInfo_p = PNULL;
    UInt16 ueIndex;
    //UInt16 ueIndex;
    UInt8 tempHarqProcessId  = pDciPdu->harqProcessId;
    /*SPR_3305_FIX_START*/
    UInt8  tempToggleVal=0;
    ULHarqInfo* ulHarqProcessInfo_p            = PNULL;
    // ULUEContextInfo *ueContextInfo_p = PNULL;
    TempCRNTICtx *tempUEContext_p = PNULL;
    UInt32 rntiType =  getRntiTypeForRnti(pDciPdu->rnti,internalCellIndex);
    /*SPR_3305_FIX_END*/
    ueIndex = pDciPdu->ueIndex;
    /*SPR_3305_FIX_START*/

    tempHarqProcessId = pDciPdu->harqProcessId;
    if(TRUE == dci0ForTempUEFlag)
    {
        tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].tempCRNTICtx_p;

        if (PNULL == tempUEContext_p)
        {
            return;
        }

        if(INVALID_HARQ_ID != tempHarqProcessId)
        {    
            ulHarqProcessInfo_p = &(tempUEContext_p->ulHarq);
        }    

    }
    else
    {
        ueContextInfo_p = &ulUECtxInfoArr_g[ueIndex];

        if (PNULL == ueContextInfo_p->ulUEContext_p)
        {
            return;
        }

        if(INVALID_HARQ_ID != tempHarqProcessId)
        {
            ulHarqProcessInfo_p = 
                &(ueContextInfo_p->ulUEContext_p->ulresInfo[tempHarqProcessId]);
        }
    }
    /* +- SPR 17777 */
#else
  LTE_GCC_UNUSED_PARAM(dci0ForTempUEFlag)
    /* +- SPR 17777 */
#endif

    /* SPR 2236 Begins*/
    /*SPR_3305_FIX_END*/

    /* SPR 2236 Ends */
    /* SPR 1859 FIX begins */
    pDciMsg->mcs = pDciPdu->tb1Info.modulationType;
    /* SPR 1859 FIX ends */

    pDciMsg->aggLevel = pDciPdu->aggregationLvl;
    pDciMsg->rnti = MAC_PHY_CONVERT_16(pDciPdu->rnti);
    pDciMsg->rbStart = pDciPdu->startRBIndex;
    pDciMsg->numOfRB = pDciPdu->numOfRBs;
    pDciMsg->cyclicShift2_forDMRS = pDciPdu->nDmrs;
    pDciMsg->freqEnabledFlag = 0;
    pDciMsg->freqHoppingBits = 0;
    pDciMsg->ueTxAntennaSelection = 0; /* not configured */
    pDciMsg->tpc = pDciPdu->tpc;
    pDciMsg->cqiRequest = pDciPdu->cqiRequest;

    /* CLPC_REWORK CHG */
#ifdef FAPI_DEVIATIONS
    pDciMsg->txPower = MAC_PHY_CONVERT_16(pDciPdu->txPower);
#endif

    /* CLPC_REWORK_CHG END */
    /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
    /*For TDD Config 0 the value of ulIndex can be 1,2 or 3 depending on the U
     * frame on which the data is to be transmitted.For other TDD configs the
     * value is 0*/
    pDciMsg->ulIndex = pDciPdu->ulIndex;
    pDciMsg->newDataIndication = pDciPdu->tb1Info.txIndicator;
    /* Resetting ulIndex to 0 */
    pDciPdu->ulIndex = 0;
    pDciMsg->dlAssignmentIndex = pDciPdu->dai;
#elif FDD_CONFIG
    pDciMsg->ulIndex = 0; 
    pDciMsg->dlAssignmentIndex = 0;
/*SPR 21001 Changes end */
#endif    
    /*TDD Config 0 Changes End*/     


#ifdef FDD_CONFIG
    if(INVALID_HARQ_ID != tempHarqProcessId)
    {
        if(NEW_TX  == pDciPdu->tb1Info.txIndicator)
        {
            tempToggleVal = ulHarqProcessInfo_p->txIndicator;
            ulHarqProcessInfo_p->txIndicator = tempToggleVal ^ NEW_TX;
#ifdef UTFWK_SIMULATION
            UInt32 rntiType =  getRntiTypeForRnti(pDciPdu->rnti,internalCellIndex);
            processGrant(pDciPdu->ueIndex,pDciPdu->harqProcessId);
            addEntryInCrcIndList(pDciPdu->ueIndex,
                    subFrameNum, 
                    tempHarqProcessId, rntiType);
#endif /* UTFWK_SIMULATION */
        }

        /* no need to toggle for retransmission */

        pDciMsg->newDataIndication = ulHarqProcessInfo_p->txIndicator;    

        pDciMsg->mcs =  pDciPdu->tb1Info.modulationType;

    }        
    else
    {
        LTE_MAC_UT_LOG(LOG_WARNING,MAC_MOD_TRANSMITTER,\
                "INVALID HARQID %d\n",tempHarqProcessId);
    }
    /* SPS changes start */   
    if (FAPI_SPS_CRNTI == rntiType)
    {
        if (SPS_ACTIVATION == msgType)
        {
            pDciMsg->tpc = 0;
            pDciMsg->cyclicShift2_forDMRS = 0;
            pDciMsg->newDataIndication = 0;   //v_g 
        }
        else if (SPS_DEACTIVATION == msgType)
        {
            pDciMsg->tpc = 0;
            pDciMsg->cyclicShift2_forDMRS = 0;
            pDciMsg->mcs = 31;
            pDciMsg->rbStart = 0;
            pDciMsg->numOfRB = 0;
            pDciMsg->newDataIndication = 0;    
        }
    }
#endif   
    /* SPS changes end */   
  
    LOG_MAC_MSG(MAC_DCI0_TX_ID,LOGBRIEF,MAC_L1_INF,
            getCurrentTick(),
            pDciPdu->ueIndex,pDciPdu->harqProcessId, 
            pDciMsg->newDataIndication,
            pDciMsg->mcs,
            pDciMsg->numOfRB,
            0.0,0.0,__func__,"DCI0_PDU_TX");
 
    pDciPdu->harqProcessId=INVALID_HARQ_ID;
    /* + SPR 5551 */
    pDciPdu->cqiRequest = 0;
    /* - SPR 5551 */
#ifdef FAPI_4_0_COMPLIANCE
    /** SPR 11161 Fix : Start ***/
    /*SPR 13892 fix start*/
    fillULDciRelease10Param( pDciMsg, pDciPdu, internalCellIndex
	/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
		    ,ulHarqProcessInfo_p
#endif	
	/* SPR 21958 PUSCH RAT1 Support End */
		    );
    /*SPR 13892 fix end*/
    /** SPR 11161 Fix : End ***/
#endif


}
/*****************************************************************************
 * Function Name  :  fillDciFormat3Or3APdu
 * Inputs         :  pDlDciPdu - Pointer to structure FAPI_dlDCIPduInfo_st and
 *                :  pDciPdu - Pointer to DCI PDU containing the info to create
 *                             the PDU message.
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function fills in the message with DCI Format3/3A data.
 *****************************************************************************/

STATIC  void fillDciFormat3Or3APdu(
		FAPI_dlDCIPduInfo_st *pDlDciPdu, 
		DCIPduInfo *pDciPdu)
{
	Char8 binary[33] = {'0'};
	UInt32 size = sizeof(binary) - 1;
	binary[size] = '\0';
	pDlDciPdu->aggLevel = pDciPdu->aggregationLvl;
	/* CLPC_REWORK CHG */
	pDlDciPdu->rnti = MAC_PHY_CONVERT_16(pDciPdu->rnti);

#ifdef FAPI_DEVIATIONS    
	pDlDciPdu->txPower = MAC_PHY_CONVERT_16(pDciPdu->txPower);
#endif

	pDlDciPdu->tpcBitMap = MAC_PHY_CONVERT_32(pDciPdu->tpcBitmap);
	CONVERT_INT32_TO_BINARY(binary, size, pDciPdu->tpcBitmap);
	/* CLPC_REWORK_CHG END */

	/* SPR 6389 changes start */
	pDlDciPdu->tpc = TPC_NO_CHANGE;
	/* SPR 6389 changes end */

	/* SPR 7811 changes start */
	pDlDciPdu->cqiRequest = 0;
	/* SPR 7811 changes end */

	LOG_MAC_MSG(MAC_TRANSMITTER_DCI_3Or3A, LOGDEBUG, MAC_L1_INF,
			getCurrentTick(), 
			pDciPdu->aggregationLvl,
			pDciPdu->rnti,
			pDciPdu->dciFormat,
			0,0,
			DEFAULT_FLOAT_VALUE,
			DEFAULT_FLOAT_VALUE, __func__, binary);

}
/*****************************************************************************
 * Function Name  :  addDciPduToHiDci0
 * Inputs         :  pDlDciPdu - Pointer to structure FAPI_dlDCIPduInfo_st and
 *                   pDciPdu - Pointer to DCI PDU containing the info to create
 *                   *pDciCceInfo,
 *                   subFrameNum,
 *                   internalCellIndex - cell id  
 * Outputs        :  None
 * Returns        :  Pointer to the buffer which can be used to fill in the
 *                   next PDU
 * Description    :  This fucntion fills in the message with the DCI PDU data
 *                   and returns the pointer to the buffer which can be used to
 *                   fill in the next PDU.
 *****************************************************************************/
STATIC  FAPI_dlDCIPduInfo_st *addDciPduToHiDci0(
		FAPI_dlDCIPduInfo_st *pDlDciPdu, DCIPduInfo *pDciPdu,
#ifdef TDD_CONFIG
		DciCCEInfo *pDciCceInfo,
#elif FDD_CONFIG
		DciCCEInfo *pDciCceInfo,UInt32 subFrameNum,
#endif
		InternalCellIndex internalCellIndex

		)
{
	FAPI_dlDCIPduInfo_st     *pNextPdu = PNULL;

	pDlDciPdu->pduType = FAPI_DCI_UL_PDU;
	pDlDciPdu->cceIndex = pDciPdu->cceIndex;
    /* +- SPR 17777 */
#ifdef FDD_CONFIG
#ifndef UTFWK_SIMULATION
  LTE_GCC_UNUSED_PARAM(subFrameNum)
#endif
#endif
    /* +- SPR 17777 */
#ifdef UTFWK_SIMULATION
#ifdef TDD_CONFIG
#elif FDD_CONFIG
	//addEntryInCrcIndList(pDciPdu->ueIndex,subFrameNum);
	//mac_reset_cce(pDciPdu->aggregationLvl, pDciCceInfo);
#endif
#endif
	//End
	switch (pDciPdu->dciFormat)
	{
		case DCI_FORMAT_0:
			{
				pDlDciPdu->ulDCIFormat = FAPI_UL_DCI_FORMAT_0;
				pDlDciPdu->ulDCIPDUSize = sizeof(FAPI_dlDCIPduInfo_st);
#ifdef TDD_CONFIG
				(*fillDciFormat0PduFunction)(pDlDciPdu, pDciPdu,FALSE,internalCellIndex);
				/*TDD_FAPI_2.4 changes END*/
#elif FDD_CONFIG
				/* +- SPR 17777 */
#if defined(UTFWK_SIMULATION) || defined(ULHARQ_TIMER_PROC) /*SPR 18979 +-*/
				(*fillDciFormat0PduFunction)(pDlDciPdu, pDciPdu,subFrameNum,FALSE, pDciCceInfo->msgType,internalCellIndex);
#else
				(*fillDciFormat0PduFunction)(pDlDciPdu, pDciPdu,FALSE, pDciCceInfo->msgType,internalCellIndex);
#endif
				/* +- SPR 17777 */
#endif
				mac_reset_cce(pDciPdu->aggregationLvl, pDciCceInfo,internalCellIndex);
				break;
			}
		case DCI_FORMAT_0_MSG3:
			{
				pDlDciPdu->ulDCIFormat = FAPI_UL_DCI_FORMAT_0;
				pDlDciPdu->ulDCIPDUSize = sizeof(FAPI_dlDCIPduInfo_st);
#ifdef TDD_CONFIG
				(*fillDciFormat0PduFunction)(pDlDciPdu, pDciPdu,TRUE,internalCellIndex);	
				/*TDD_FAPI_2.4 changes END*/
#elif FDD_CONFIG
                /* +- SPR 17777 */
#if defined(UTFWK_SIMULATION) || defined(ULHARQ_TIMER_PROC) /*SPR 18979 +-*/
				(*fillDciFormat0PduFunction)(pDlDciPdu, pDciPdu,subFrameNum,TRUE, pDciCceInfo->msgType,internalCellIndex);
#else
				(*fillDciFormat0PduFunction)(pDlDciPdu, pDciPdu,TRUE, pDciCceInfo->msgType,internalCellIndex);
#endif
                /* +- SPR 17777 */
#endif
				mac_reset_cce(pDciPdu->aggregationLvl, pDciCceInfo,internalCellIndex);
				break;
			}


		case DCI_FORMAT_3:
			{
				pDlDciPdu->ulDCIFormat = FAPI_UL_DCI_FORMAT_3;
				pDlDciPdu->ulDCIPDUSize = sizeof(FAPI_dlDCIPduInfo_st);

				/* call function fillDciFormat3Or3APdu(pDlDciPdu, pDciPdu) */
				fillDciFormat3Or3APdu(pDlDciPdu, pDciPdu);
				mac_reset_cce(pDciPdu->aggregationLvl, pDciCceInfo,internalCellIndex);
				break;

				LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,
						"[%s] Exit Wrong DCI format %d, PDU dropped",
						__func__, pDciPdu->dciFormat);
				return pDlDciPdu;
				break;
			}
		case DCI_FORMAT_3A:
			{
				pDlDciPdu->ulDCIFormat = FAPI_UL_DCI_FORMAT_3A;
				pDlDciPdu->ulDCIPDUSize = sizeof(FAPI_dlDCIPduInfo_st);
				fillDciFormat3Or3APdu(pDlDciPdu, pDciPdu);
				mac_reset_cce(pDciPdu->aggregationLvl, pDciCceInfo,internalCellIndex);
				break;
			}
		case DCI_FORMAT_1:
		case DCI_FORMAT_1A:
		case DCI_FORMAT_1B:
		case DCI_FORMAT_1C:
		case DCI_FORMAT_1D:
		case DCI_FORMAT_2:
		case DCI_FORMAT_2A:
		default:
			{
				pDlDciPdu->ulDCIFormat = 0;
				LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,
						"[%s] Exit Wrong DCI format %d, PDU dropped",
						__func__, pDciPdu->dciFormat);
				/* This PDU will be dropped, returning the current pointer */
				return pDlDciPdu;
			}
	}
	pNextPdu = (FAPI_dlDCIPduInfo_st *)(((UInt8 *)pDlDciPdu)
			+ pDlDciPdu->ulDCIPDUSize );
	return pNextPdu;
}
/*****************************************************************************
 * Function Name  :  txDLHiDci0Msg
 * Inputs         :  phyDelay - phy delay to use,
 *                   currentGlobalTick - current global tick for MAC,
 *                   sysFrame - SFN and subFrame - SF
 *                   internalCellIndex - cell id  
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function transmits the HiDci PDU.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129087) */
MacRetType txDLHiDci0Msg(UInt8 phyDelay,
/* SPR 5599 changes end (ZIP ID 129087) */
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
		UInt32 sysFrame,
		UInt32 subFrame
		,InternalCellIndex internalCellIndex)
{
	UInt8   containerTick = 0;
	UInt32  subFrameNum = 0;
	UInt32  sysFrameNum = 0;
	UInt32  numHiPdu = 0;
	UInt32  numDciPdu = 0;
	/*+ Dyn UEs To Be SchPerTTI +*/
	UInt32  numUEDci = 0;
	/*- Dyn UEs To Be SchPerTTI -*/
	UInt32  length = offsetof(FAPI_dlHiDCIPduInfo_st, hidciPduInfo);//;0;
	UInt16  sfnSf = 0;
	UInt8   cceCount = 0;
	UInt32  cceNumber = 0;
	HIPduArray  *pHiPduNode = PNULL;
	UInt8       *pNextPdu = PNULL;
	UInt8       *pPrevPdu = PNULL;
	UInt8  tempToggleVal=0;
	ULHarqInfo*  ulHhpId_p=PNULL;
#ifdef LAYER2_PHYSIM_THREAD
	UInt8 *msgBuffer = PNULL;
#else
	/*OPT_SPR_4119_4120_4122_4123_CHANGES_START*/
	UInt8 *msgBuffer = pHIDCICfgMsgBuff_gp[internalCellIndex];
	/*OPT_SPR_4119_4120_4122_4123_CHANGES_END*/

	/* SPR 9917 changes Start */
	memSet(pHIDCICfgMsgBuff_gp[internalCellIndex], 0, CONTROL_BUFFER_SIZE);
	/* SPR 9917 changes End*/
#endif
	DCIPduInfo                  *pDciPduInfo = PNULL;
	//#ifdef TDD_CONFIG
#ifdef UTFWK_SIMULATION
	ULHarqInfo* ulHarqProcessInfo_p            = PNULL;
	UInt8 tempHarqProcessId            = 255;
#endif //UTFWK_SIMULATION
	//#endif //TDD_CONFIG
	DciCCEContainerInfo*        dciCCEContainerInfo_p = PNULL;
	DciCCEInfo                  *pDciCceInfo = PNULL;
	FAPI_dlHiDCIPduInfo_st      *pHiDciPdu = PNULL;
#ifdef LAYER2_PHYSIM_THREAD
	GET_MEM_FROM_POOL(UInt8,msgBuffer,CONTROL_BUFFER_SIZE, PNULL);
#endif

	containerTick = (currentGlobalTick + phyDelay) % MAX_PDCCH_CONTAINER;
	dciCCEContainerInfo_p = (dciCCEContainerInfo_gp[internalCellIndex] + containerTick);

	/* Add phyDelay and wrap around subFrameNum at 10 (MAX_SUB_FRAME) */
	subFrameNum = (subFrame + phyDelay) % MAX_SUB_FRAME;

	/* Add overflow from subframe to sysframe and wrap around it at
	   1024 (MAX_SFN_VALUE) */
	sysFrameNum =
		(sysFrame + (subFrame + phyDelay)/MAX_SUB_FRAME) % MAX_SFN_VALUE;

	/* Generate SFN/SF value from sysFrame and subFrame
	   (Add phyDelay to subFrame and check overflow for both values) */
	sfnSf = GENERATE_SUBFRAME_SFNSF(sysFrameNum, subFrameNum);

	/* Get HI PDU node */
	pHiPduNode = (hiPduContainer_gp[internalCellIndex] + containerTick);

	pHiDciPdu = (FAPI_dlHiDCIPduInfo_st *)(msgBuffer + L1_API_HEADER_LEN);
	pNextPdu = &pHiDciPdu->hidciPduInfo[0];



	/* Fill HI Node in msg buffer */
	while(pHiPduNode->count != 0)
	{
		pPrevPdu = pNextPdu;
		{
#if (defined LOG_UT_ENABLED) || (defined UTFWK_SIMULATION)
			MacHIPduInfo *ppMacHiPdu;
			ppMacHiPdu = &(pHiPduNode->macHIPduInfo[pHiPduNode->count - 1]);
#endif

#ifdef UTFWK_SIMULATION
			//MacHIPduInfo *ppMacHiPdu; /* TDD Warning Fix */  
			ppMacHiPdu = &(pHiPduNode->macHIPduInfo[pHiPduNode->count - 1]);
			if (FAPI_HI_NACK == ppMacHiPdu->hiValue )
			{
				UInt16 hiUeIndex = getUeIdxFromRNTIMap((ppMacHiPdu->crnti)->index,internalCellIndex);
				UInt32 rntiType =  getRntiTypeForRnti(ppMacHiPdu->crnti,internalCellIndex);
#ifdef FDD_CONFIG
				tempHarqProcessId = (currentGlobalTick + phyDelay + 4) % 
					MAX_HARQ_PROCESS_NB;
#elif TDD_CONFIG
				/* TDD Config 0 and 6 Changes Start */
				/*QOS_TDD_FIX*/
				ulHarqProcessId = getFreeULHarqId(ulUECtxInfoArr_g[hiUeIndex].ulUEContext_p,\
						/* SPR 11257 Changes Start*/        
                        /* +- SPR 17777 */
                        subFrameNum, ulSubFrameNum, currentGlobalTick,internerCellIndex);
                /* +- SPR 17777 */
				/* SPR 11257 Changes End*/
				/* TDD Config 0 and 6 Changes End */
				/*QOS_TDD_FIX*/

#endif /*FDD/TDD */
				ulHarqProcessInfo_p = &(ulUECtxInfoArr_g[hiUeIndex].
						ulUEContext_p->ulresInfo[tempHarqProcessId]);

				if ( HARQ_PROCESS_FREE != ulHarqProcessInfo_p->harqStatus )
				{
					addEntryInCrcIndList(hiUeIndex, subFrameNum, tempHarqProcessId,rntiType);
				}
			}
#endif /*UTFWK_SIMULATION */
			LOG_UT(MAC_HI_TX_ID,LOGDEBUG,MAC_L1_INF,\
					currentGlobalTick,\
					ppMacHiPdu->rbStart,ppMacHiPdu->nDmrs, \
					ppMacHiPdu->hiValue,0,0,\
					0.0,0.0,__func__,"HI_TX");
		}     
		pNextPdu = (UInt8 *)addHiPduToHiDci0(
				(FAPI_dlHiPduInfo_st *)pNextPdu, pHiPduNode,internalCellIndex);
		if (pPrevPdu != pNextPdu)
		{
			numHiPdu++;
			length += ((uintptr_t)pNextPdu - (uintptr_t)pPrevPdu);
		}
	}

	cceCount = dciCCEContainerInfo_p->countOfCCEAllocated;    
	while (cceCount--)
	{
		cceNumber = dciCCEContainerInfo_p->arrayCCEAllocated[cceCount];
		pDciCceInfo = &(dciCCEContainerInfo_p->dciCCEInfo[cceNumber]);

		/* If CCE is used (not free), then use the DCI PDU info to send msg */
		if (!pDciCceInfo->isCCEFree)
		{
			/* DCI present, so increment the count */
			pDciPduInfo = &pDciCceInfo->dciPduInfo;

			if ( (pDciPduInfo->dciFormat != DCI_FORMAT_0 &&
						pDciPduInfo->dciFormat != DCI_FORMAT_0_MSG3 &&
						pDciPduInfo->dciFormat != DCI_FORMAT_3 &&
						pDciPduInfo->dciFormat != DCI_FORMAT_3A) ||
					( (pDciCceInfo->msgType == SPS_OCCASION) && (DCI_FORMAT_0 == pDciPduInfo->dciFormat))||
					(MAX_NUM_DCI_FORMAT == pDciPduInfo->dciFormat))
			{
				if( (pDciCceInfo->msgType == SPS_OCCASION) && (DCI_FORMAT_0 == pDciPduInfo->dciFormat) )  
				{
					if(NEW_TX  == pDciPduInfo->tb1Info.txIndicator)
					{
						ulHhpId_p = 
							&(ulUECtxInfoArr_g[pDciPduInfo->ueIndex].ulUEContext_p->ulresInfo[pDciPduInfo->harqProcessId]);
						tempToggleVal = ulHhpId_p->txIndicator;
						ulHhpId_p->txIndicator = tempToggleVal ^ NEW_TX;
					}

				}
				continue;
			}

#ifdef TDD_CONFIG
#ifdef UTFWK_SIMULATION
			tempHarqProcessId = pDciPduInfo->harqProcessId;
#endif //UTFWK_SIMULATION

			/* + SPR_14039 */
			if (IS_CA_ENABLED())
			{    
				pDciCceInfo->dciPduInfo.dai = getDai(pDciCceInfo->dciPduInfo.ueIndex,
						currentGlobalTick, subFrameNum, internalCellIndex);

			}
			/* - SPR_14039 */
#endif //TDD_CONFIG
			/* Fill in DCI PDU in DL_CONFIG.Request message */
			pPrevPdu = pNextPdu;
			/* Deleted it should not be called*/
			//setTxIndFromHarq(pDciPduInfo);
			pNextPdu = (UInt8 *)addDciPduToHiDci0((FAPI_dlDCIPduInfo_st *)
#ifdef TDD_CONFIG			
					pNextPdu, pDciPduInfo, pDciCceInfo,
#elif FDD_CONFIG
					pNextPdu, pDciPduInfo, pDciCceInfo,subFrameNum, 
#endif
					internalCellIndex
					);
#ifdef UTFWK_SIMULATION
			UInt32 rntiType =  getRntiTypeForRnti(pDciPduInfo->rnti,internalCellIndex);
#ifdef TDD_CONFIG
			if(NEW_TX == pDciPduInfo->tb1Info.txIndicator)
			{
				ulHarqProcessInfo_p = &(ulUECtxInfoArr_g[pDciPduInfo->ueIndex].ulUEContext_p->ulresInfo[tempHarqProcessId]);
				if(ulHarqProcessInfo_p->riLength)
				{
					processGrant(pDciPduInfo->ueIndex,tempHarqProcessId);
				}
				else
				{
				}
				addEntryInCrcIndList( pDciPduInfo->ueIndex, subFrameNum, tempHarqProcessId,rntiType);
			}
#endif
#endif
			if (pPrevPdu != pNextPdu)
			{
				/* Increase number of DCI only if add DCI was successful */
				numDciPdu++;
				/*+ Dyn UEs To Be SchPerTTI +*/
				if(pDciPduInfo->rnti != 0xFFFF)
				{
					numUEDci++;
				}
				/*- Dyn UEs To Be SchPerTTI -*/
				length += ((uintptr_t)pNextPdu - (uintptr_t)pPrevPdu);
			}
		}
	}
	if (0 < numDciPdu || 0 < numHiPdu)
	{
		/* Fill buffer header */
		/*OPT_SPR_4119_4120_4122_4123_CHANGES_START*/
        /* +- SPR 17777 */
		fillMacApiBufferHeader(msgBuffer,
				PHY_DL_HI_DCI0_REQUEST, length);
        /* +- SPR 17777 */
		/*OPT_SPR_4119_4120_4122_4123_CHANGES_END*/
		pHiDciPdu->sfnsf = MAC_PHY_CONVERT_16(sfnSf);
#ifdef FLEXRAN
		pHiDciPdu->sfnsf_t = MAC_PHY_CONVERT_16(sfnSf);
#endif
		pHiDciPdu->numOfDCI = numDciPdu;
		pHiDciPdu->numOfHI = numHiPdu;

/* SPR 19288 change start */
/* SPR 19288 change end */

#ifdef FLEXRAN
   g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_HI_DCI0; 
#endif

		if (0 >= sendDataToPhysical (msgBuffer, length + L1_API_HEADER_LEN,internalCellIndex) )
		{
			LOG_MAC_MSG(L2_SOCKET_SENDTO_FAIL_ID,LOGERROR,L2_SYS_FAIL,\
					getCurrentTick(),\
					__LINE__,0,0,0,0,\
					0.0,0.0,__FILE__,__func__);
			LTE_MAC_UT_LOG(LOG_MAJOR, MAC_MOD_TRANSMITTER,"Send data failed");
		}
		/*+ Dyn UEs To Be SchPerTTI +*/
		if(numUEDci > 0)
		{
			LOG_MAC_MSG(MAC_UL_UE_TO_BE_SCHEDULED,\
					LOGBRIEF,\
					MAC_L1_INF,\
					getCurrentTick(),
					numUEDci,\
					0,0,0,0,0,0,\
					__func__," ");
		}
		/*- Dyn UEs To Be SchPerTTI -*/
	}
#ifdef LAYER2_PHYSIM_THREAD
	else
	{
		freeMemPool(msgBuffer);
	}
#endif
	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Exit",__func__);
	return MAC_SUCCESS;
}
/* MAC_TTI_LOG Start */
#ifdef SHM_PRINT_ENABLED
/*****************************************************************************
 * Function Name  :  fillttiDciLoggingInfo 
 * Inputs         :  log_ptr - pointer to ttiDciLoggingInfo and
 *                   tempDciCCEInfo_p - pointer to DciCCEInfo
 *                   internalCellIndex - cell id  
 * Outputs        :  fills ttiDciLoggingInfo structure
 * Returns        :  None
 * Description    :  This function will fill ttiDciLoggingInfo from DciCCEInfo
 *****************************************************************************/
STATIC  void fillttiDciLoggingInfo (ttiDciLoggingInfo *log_ptr,
		DciCCEInfo *tempDciCCEInfo_p, InternalCellIndex internalCellIndex)
{


	ULUEContext *ueULContext_p     = PNULL;
	DLUEContext *ueDLContext_p     = PNULL;

	log_ptr->logid = MAC_TTI_DCI_LOG_INFO;
	log_ptr->systemFrame =  getSysFrameNumer(internalCellIndex); 
	log_ptr->subframe= getSubFrameNumber(internalCellIndex);
	log_ptr->rnti = tempDciCCEInfo_p->dciPduInfo.rnti;
	log_ptr->ueID = tempDciCCEInfo_p->dciPduInfo.ueIndex;
	log_ptr->numOfRBs = tempDciCCEInfo_p->dciPduInfo.numOfRBs;
	log_ptr->dciFormat = tempDciCCEInfo_p->dciPduInfo.dciFormat;
	log_ptr->numOfLayers = tempDciCCEInfo_p->dciPduInfo.numLayer;
	log_ptr->numOfTB = tempDciCCEInfo_p->dciPduInfo.numTB;
	log_ptr->harqID = tempDciCCEInfo_p->dciPduInfo.harqProcessId;
	log_ptr->ratPolicy = tempDciCCEInfo_p->dciPduInfo.resrcAllocType;
	log_ptr->rbCoding = tempDciCCEInfo_p->dciPduInfo.startRBIndex;
	log_ptr->tpc = tempDciCCEInfo_p->dciPduInfo.tpc;
	log_ptr->transmissionScheme = tempDciCCEInfo_p->dciPduInfo.transmissionScheme;
	log_ptr->cceIndex = tempDciCCEInfo_p->dciPduInfo.cceIndex;
	log_ptr->aggregationLevel = tempDciCCEInfo_p->dciPduInfo.aggregationLvl;
	log_ptr->rntiType = getRntiTypeForRnti(log_ptr->rnti, internalCellIndex); 
	log_ptr->searchSpace = 0;
	log_ptr->dlQueueLoad = 0;
	log_ptr->ulBSR = 0;

	log_ptr->tbSize1 = tempDciCCEInfo_p->dciPduInfo.tb1Info.tbSize;
	log_ptr->rv1 = tempDciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer;
	log_ptr->ndi1 = tempDciCCEInfo_p->dciPduInfo.tb1Info.txIndicator;
	log_ptr->mcsIndex1 = tempDciCCEInfo_p->dciPduInfo.tb1Info.modulationType;

	log_ptr->tbSize2 = tempDciCCEInfo_p->dciPduInfo.tb2Info.tbSize;
	log_ptr->rv2 = tempDciCCEInfo_p->dciPduInfo.tb2Info.redundancyVer;
	log_ptr->ndi2 = tempDciCCEInfo_p->dciPduInfo.tb2Info.txIndicator;
	log_ptr->mcsIndex2 = tempDciCCEInfo_p->dciPduInfo.tb2Info.modulationType;

	ueULContext_p     = ulUECtxInfoArr_g[tempDciCCEInfo_p->dciPduInfo.ueIndex].ulUEContext_p;
	ueDLContext_p     = dlUECtxInfoArr_g[tempDciCCEInfo_p->dciPduInfo.ueIndex].dlUEContext_p;


	if(PNULL != ueDLContext_p)
	{
		log_ptr->dlQueueLoad = ueDLContext_p->ueQueueLoad;
	}

	if(PNULL != ueULContext_p)
	{
		log_ptr->ulBSR = ueULContext_p->bsrNetTotal;
	}

	return; 
}
#endif

/*****************************************************************************
 * Function Name  :  addMchPduToDlData
 * Inputs         :  pDlDataPdu - Pointer to message buffer to fill in the
 *                                PDU.
 *                   pduIndex - Index of PDU
 * Outputs        :  None
 * Returns        :  Pointer to the buffer which can be used to fill in the
 *                   next PDU
 * Description    :  This function fills in the message with the MCH PDU data
 *                   and returns the pointer to the buffer which can be used to
 *                   fill in the next PDU.
 *****************************************************************************/
#ifdef LTE_EMBMS_SUPPORTED
STATIC  FAPI_dlPduInfo_st *addMchPduToDlData(
        FAPI_dlPduInfo_st *pDlDataPdu, MchPduInfo *mchPduInfo_p,
        UInt32 pduIndex)
{
    FAPI_dlPduInfo_st       *pNextPdu = PNULL;
    FAPI_dlTLVInfo_st       *pDlTlv = PNULL;
    UInt32                  tagLen = 0;
    UInt32                  pduLen = sizeof(FAPI_dlPduInfo_st)
        - 1*sizeof(FAPI_dlTLVInfo_st);
    pDlDataPdu->pduIndex = MAC_PHY_CONVERT_16(pduIndex);
    pDlDataPdu->numOfTLV = MAC_PHY_CONVERT_32(0x1); /* 1 MCH MAC PDU data */

    pDlTlv = (FAPI_dlTLVInfo_st *)&pDlDataPdu->dlTLVInfo[0];

    pDlTlv->tag = MAC_PHY_CONVERT_16(0); /* Currently we only support value */

    if (0 != (mchPduInfo_p->tbSize % 4) )
    {
        /* tag length has to be a multiple of 4 */
        tagLen = (((mchPduInfo_p->tbSize / 4) + 1) * 4);
    }
    else
    {
        tagLen = mchPduInfo_p->tbSize;
    }
    pDlTlv->tagLen = MAC_PHY_CONVERT_16(mchPduInfo_p->tbSize);
    pduLen += (offsetof(FAPI_dlTLVInfo_st, value)) + tagLen;

    /* Purify Start*/
    memSet ( ( (UInt8 *)pDlTlv->value) + mchPduInfo_p->tbSize,
            0, tagLen - mchPduInfo_p->tbSize);
    /* Purify End*/
    if (mchPduInfo_p->dataTB_p!= NULL)
    {
        msgExtract(mchPduInfo_p->dataTB_p,DLSCH_PDU_OFFSET,(UInt8 *)pDlTlv->value,mchPduInfo_p->tbSize);
    }

     pDlDataPdu->pduLen = MAC_PHY_CONVERT_16(pduLen);
     pNextPdu = (FAPI_dlPduInfo_st *)(((UInt8 *)pDlDataPdu)
             + pduLen);

     return pNextPdu;
}

#endif


/* MAC_TTI_LOG Start */
/*****************************************************************************
 * Function Name  :  txDLDataMsgReq
 * Inputs         :  phyDelay - The delay of the execution leg in terms of the 
 *                              number of sub-frames w.r.t. PHY and the current
 *                              processing being done at MAC(is intended for PHY),
 *                   currentGlobalTick - The present Global Tick,
 *                   sysFrame and subFrame
 *                   internalCellIndex - cell id  
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will create and send the data buffer and will
 *                   take it's input from the global dciCCEInfoArray.
 *****************************************************************************/
MacRetType txDLDataMsgReq(UInt8 phyDelay,
		/* SPR 15909 fix start */
		tickType_t currentGlobalTick,
		/* SPR 15909 fix end */
		UInt32 sysFrame,
		UInt32 subFrame,
		InternalCellIndex internalCellIndex)
{
	UInt8   containerTick = 0;
	UInt32  subFrameNum = 0;
	UInt32  sysFrameNum = 0;
	UInt32  pduIndex = 0;
	UInt16  sfnSf = 0;
	UInt32  numPdu = 0;
	UInt8   localCount = 0;
	UInt32  length = 0;
	UInt32  cceNumber = 0;
#ifdef LTE_EMBMS_SUPPORTED
    MchPduInfo *pMchPduInfo = PNULL;
    UInt64 totalMchPduTBSize = 0;
    UDouble32 embmsDlDataRate = 0;
#endif

	/*OPT_SPR_4119_4120_4122_4123_CHANGES_START*/
	UInt8   *msgBuffer = pTxReqMsgBuff_gp[internalCellIndex];
	/*OPT_SPR_4119_4120_4122_4123_CHANGES_END*/
	/* SPR 9917 changes Start */
	/* + SPR 13990 Changes */
	CellConfigParams* cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

	if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4 && cellParams_p->dlAvailableRBs == 100)
	{
		memSet(pTxReqMsgBuff_gp[internalCellIndex], 0, 2 * DATA_BUFFER_SIZE);
	}
	else
	{
		memSet(pTxReqMsgBuff_gp[internalCellIndex], 0, DATA_BUFFER_SIZE);
	}
	/* - SPR 13990 Changes */
	/* SPR 9917 changes End */
	FAPI_dlDataTxRequest_st     *pTxReq = PNULL;
	FAPI_dlPduInfo_st           *pNextPdu = PNULL;
	FAPI_dlPduInfo_st           *pPrevPdu = PNULL;
	DciCCEContainerInfo*        dciCCEContainerInfo_p = PNULL;
	DCIPduInfo                  *pDciPduInfo = PNULL;
	DciCCEInfo                  *pDciCceInfo = PNULL;

	/* SPR 19288 change start */
	/* SPR 19288 change end */
	/* SPR 609 changes start */    
	//DLUEContextInfo *ueContextInfo_p = PNULL; 
	//DLHARQProcess *harqProcess_p = PNULL;
	/* SPR 609 changes start */    
#ifdef DL_DATA_SEPARATION    
	/* SPR 9399 Changes Start */
#ifndef DL_CONFIG_DATA_SYNC_DISABLED
	eventFD_t tempfd;
	eventfd_read_wrapper(ev_fd_tx_Control_g[internalCellIndex],&tempfd);
#endif    
	/* SPR 9399 Changes End */
#endif    
	/* SPR 10411 changes start */ 
	/* SPR 10411 changes end */ 

#ifdef FLEXRAN
    dlDataOffset[internalCellIndex] = (void *)g_pSharedMemAddrSegments[internalCellIndex][MODULO_FOUR(currentGlobalTick)] - g_pSharedMemAddr[internalCellIndex][FAPI_SHMA_DL_BUFFER];
#endif

	containerTick = (currentGlobalTick + phyDelay) % MAX_PDCCH_CONTAINER;
	dciCCEContainerInfo_p = (dciCCEContainerInfo_gp[internalCellIndex] + containerTick);

	localCount = dciCCEContainerInfo_p->countCmnChannelMsg;
	/* Add phyDelay and wrap around subFrameNum at 10 (MAX_SUB_FRAME) */
	subFrameNum = (subFrame + phyDelay) % MAX_SUB_FRAME;

	/* Add overflow from subframe to sysframe and wrap around it at
	   1024 (MAX_SFN_VALUE) */
	sysFrameNum =
		(sysFrame + (subFrame + phyDelay)/MAX_SUB_FRAME) % MAX_SFN_VALUE;

#ifdef FLEXRAN
        subFrameNum_flexran[internalCellIndex] = subFrameNum;
        sysFrameNum_flexran[internalCellIndex] = sysFrameNum;
#endif
	/* Generate SFN/SF value from sysFrame and subFrame
	   (Add phyDelay to subFrame and check overflow for both values) */
	sfnSf = GENERATE_SUBFRAME_SFNSF(sysFrameNum, subFrameNum);

	/* Get the pointer to Fill in API parameters */
	pTxReq = (FAPI_dlDataTxRequest_st *)(msgBuffer + L1_API_HEADER_LEN);

	/* Get the pointer to first PDU in message buffer to start filling the
	   PDUs */
	pNextPdu = (FAPI_dlPduInfo_st *)&pTxReq->dlPduInfo[0];
	length += ((uintptr_t)pNextPdu - (uintptr_t)pTxReq);

	
	if (dciCCEContainerInfo_p->mibData_p)
	{
		/* Fill in BCH PDUs - PDU stored in dciCCEContainerInfo_p->mibData_p */

		LOG_UT(MAC_BCH_DATA_TX_ID,LOGDEBUG,MAC_L1_INF,\
				currentGlobalTick,\
				dciCCEContainerInfo_p->mibDataLen,\
				pduIndex,0,0,0,\
				0.0,0.0,__func__,"BCH_DATA_PDU");
		pPrevPdu = pNextPdu;
		pNextPdu = addBchPduToDlData(pNextPdu, dciCCEContainerInfo_p,
				pduIndex
				/* SPR 19288 change start */
#ifdef FLEXRAN
	, internalCellIndex
#endif
				/* SPR 19288 change end */                      
				);
		/* DL DATA SEP Changes */
		//msgFree(dciCCEContainerInfo_p->mibData_p);
		//dciCCEContainerInfo_p->mibData_p = PNULL;
		if (pPrevPdu != pNextPdu)
		{
			pduIndex++;
			numPdu++;
			length += ((uintptr_t)pNextPdu - (uintptr_t)pPrevPdu);
			/* DL DATA SEP Changes */
			// dciCCEContainerInfo_p->mibData_p = PNULL;
			/* SPR 19288 change start */
			/* SPR 19288 change end */
		}
	}
#ifdef LTE_EMBMS_SUPPORTED
    pMchPduInfo = mchPduInfo_gp[internalCellIndex];

    if (pMchPduInfo != PNULL)
    {
        pPrevPdu = pNextPdu;
        pNextPdu = addMchPduToDlData(pNextPdu,pMchPduInfo,pduIndex);
        
        if (pPrevPdu != pNextPdu)
        {
            pduIndex++;
            numPdu++;
            length += ((uintptr_t)pNextPdu - (uintptr_t)pPrevPdu);
        }
        
        msgFree(mchPduInfo_gp[internalCellIndex]->dataTB_p);
        /*SSI fix*/
        freeMemPool(mchPduInfo_gp[internalCellIndex]);
        mchPduInfo_gp[internalCellIndex] = PNULL;
    } 

    /******KPI change******/
    /* print and Reset both variables */

    if(getCurrentTick()%1000 == 0)
    {
        totalMchPduTBSize = kpiMchPduTBSize_g; 
        embmsDlDataRate = (totalMchPduTBSize * 8)/(1024*1024);

        LOG_MAC_INFO(L2_EMBMS_CAT,"EMBMS DL throughput %lf \n",embmsDlDataRate);
        kpiMchPduTBSize_g = 0;
    }

    /******KPI change********/

#endif


	/* Cyclomatic_complexity_changes_start */
	processAllocatedCCEArray(currentGlobalTick,
			dciCCEContainerInfo_p,
			&pNextPdu,
			&pPrevPdu,
			&pduIndex,
			&numPdu,
			&length,
			internalCellIndex
			/* SPR 19288 change start */
			/* SPR 19288 change end */                                          
			); 
	/* Cyclomatic_complexity_changes_end */

	while (localCount--)
	{
		cceNumber = dciCCEContainerInfo_p->
			dciCCEInfoCmnChannel[localCount].cceIndex;
		pDciCceInfo = &(dciCCEContainerInfo_p->dciCCEInfo[cceNumber]);
		pDciPduInfo = &pDciCceInfo->dciPduInfo;

		if (pDciCceInfo->isCCEFree)
		{
			/* 3778_FIX_START */
			/* + coverity 32195 */
			pDciCceInfo->msgType = INVALID_MSG;
			/* - coverity 32195 */
			/* 3778_FIX_END */
			continue;
		}

		LOG_UT(MAC_COMM_DATA_TX_ID,LOGDEBUG,MAC_L1_INF,\
				currentGlobalTick,\
				dciCCEContainerInfo_p->dciCCEInfoCmnChannel[localCount].msgType,\
				pDciPduInfo->ueIndex,\
				pDciPduInfo->harqProcessId,\
				pDciPduInfo->dciFormat,\
				pDciPduInfo->tb1Info.tbSize,\
				0.0,0.0,__func__,"COMM_DATA_MSG_TX");
		LOG_MAC_MSG(MAC_DATA_TX_ID3,LOGDEBUG,MAC_L1_INF,\
				currentGlobalTick,\
				pDciPduInfo->ueIndex,\
				dciCCEContainerInfo_p->dciCCEInfoCmnChannel[localCount].msgType,\
				pDciPduInfo->dciFormat,\
				pDciPduInfo->tb1Info.tbSize,\
				0,0.0,0.0,__func__,"COMM_DATA_MSG_TX");

		/*Cyclomatic Complexity changes - starts here */
		handleCommChDataMsg(dciCCEContainerInfo_p->dciCCEInfoCmnChannel[localCount].msgType,
				pDciPduInfo,&pPrevPdu,&pNextPdu,&pduIndex,&numPdu,
				&length,sysFrameNum,subFrameNum,
                internalCellIndex
				/* SPR 19288 change start */
				/* SPR 19288 change end */                                      
				);
		/*Cyclomatic Complexity changes - ends here */

		/* DL DATA SEP Changes */
		//mac_reset_cce(pDciPduInfo->aggregationLvl, pDciCceInfo);
		/* 3778_FIX_START */
		/* DL DATA SEP Changes */
		/* 3778_FIX_END */
	}

    /*if(numPdu!=ulCheckPDUnum)
    {
        fprintf(stderr,"mumPdu %d, ulChecknum %d, tick %llu\n",numPdu,ulCheckPDUnum,getCurrentTick());
    }*/
	
	if (0 < numPdu)
	{
		LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER, "num Data pdu = %d\n", numPdu);

		/* Fill in API Parameters */
		pTxReq->sfnsf = MAC_PHY_CONVERT_16(sfnSf);
		pTxReq->numOfPDU = MAC_PHY_CONVERT_16(numPdu);
		
		/* Fill buffer header */
		/*OPT_SPR_4119_4120_4122_4123_CHANGES_START*/
		/* +- SPR 17777 */
		fillMacApiBufferHeader(msgBuffer,
				PHY_DL_TX_REQUEST, length );
		/* +- SPR 17777 */
		/* SPR 19288 change start */
		/* SPR 19288 change end */
		/*OPT_SPR_4119_4120_4122_4123_CHANGES_END*/
		/*(uintptr_t)pNextPdu - (uintptr_t)pTxReq);*/
		/* Send data to Physical */
		totalMacDllinkBytesSend_g += MULTIPLY_BY_EIGHT(length);
		/* SPR 19288 change start */
#if   FLEXRAN
     g_flexran_channelId[internalCellIndex] = FAPI_SHMA_CH_TX_REQ; 
// <Send TX REQ as per ASTRI mechanism>
		if (0 >= sendDataToPhysical (msgBuffer, length + L1_API_HEADER_LEN,internalCellIndex) )
		{
			LOG_MAC_MSG(L2_SOCKET_SENDTO_FAIL_ID,LOGERROR,L2_SYS_FAIL,\
					getCurrentTick(),\
					__LINE__,0,0,0,0,\
					0.0,0.0,__FILE__,__func__);

			LTE_MAC_UT_LOG(LOG_MAJOR, MAC_MOD_TRANSMITTER,"Send data failed");
		}

#else 

		if (0 >= sendDataToPhysical (msgBuffer, length + L1_API_HEADER_LEN,internalCellIndex) )
		{
			LOG_MAC_MSG(L2_SOCKET_SENDTO_FAIL_ID,LOGERROR,L2_SYS_FAIL,\
					getCurrentTick(),\
					__LINE__,0,0,0,0,\
					0.0,0.0,__FILE__,__func__);

			LTE_MAC_UT_LOG(LOG_MAJOR, MAC_MOD_TRANSMITTER,"Send data failed");
		}
/*SPR 21001 Changes end */
#endif
		/* SPR 19288 change end */
		/* + SPR_10291 */
		LOG_MAC_MSG(L2_TX_DATA_SEND,LOGDEBUG,MAC_L1_INF,\
				getCurrentTick(),\
				length,numPdu,0,0,0,\
				0.0,0.0,__func__, __FILE__);
		/* - SPR_10291 */
	}
	/* SPR 19288 change start */
	/* SPR 19288 change end */

#ifdef LAYER2_PHYSIM_THREAD
	else
	{
		freeMemPool(msgBuffer);
	}
#else
	/*OPT_SPR_4119_4120_4122_4123_CHANGES_START*/
	//freeMemPool(msgBuffer);
	/*OPT_SPR_4119_4120_4122_4123_CHANGES_END*/

#ifdef DL_DATA_SEPARATION   
	if(numPdu)
	{
		eventfd_write_wrapper(ev_fd_tx_g[internalCellIndex],numPdu);
	}
	else
	{
		eventfd_write_wrapper(ev_fd_tx_g[internalCellIndex],1);
	}
#endif
#endif

	/* TTIB_Code Start */
#ifdef FDD_CONFIG
	/* dciCCEContainerInfo_p->countOfCCEAllocated = 0;*/
	/* TTIB_Code End */
#endif


	LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] Exit",__func__);
	return MAC_SUCCESS;
}
#ifdef DL_UL_SPLIT
/*****************************************************************************
 * Function Name  : convertULConfigPduTypeRBAllocFail
 * Inputs         : ulSchUciInfo_p - pointer to ULSchUciInfo
 * Outputs        : None
 * Returns        : None
 * Description    : This function converts the UL Config PDU Type when RB 
 *                  Allocation fails in PDSCH and UL config needs to be send 
 *                  without Harq.
 *****************************************************************************/
STATIC void convertULConfigPduTypeRBAllocFail (ULSchUciInfo* ulSchUciInfo_p)
{
	switch (ulSchUciInfo_p->pduType)
	{
		case MAC_SS_PDU_ULSCH_HARQ:
			ulSchUciInfo_p->pduType = MAC_SS_PDU_ULSCH ;
			break;
		case MAC_SS_PDU_ULSCH_CQI_HARQ_RI:
			ulSchUciInfo_p->pduType = MAC_SS_PDU_ULSCH_CQI_RI ;
			break;
		case MAC_SS_PDU_UCI_SR_HARQ:
			ulSchUciInfo_p->pduType = MAC_SS_PDU_UCI_SR ;
			break;
		case MAC_SS_PDU_UCI_CQI_HARQ:
			ulSchUciInfo_p->pduType = MAC_SS_PDU_UCI_CQI ;
			break;
		case MAC_SS_PDU_UCI_CQI_SR_HARQ:
			ulSchUciInfo_p->pduType = MAC_SS_PDU_UCI_CQI_SR ;
			break;
		case MAC_SS_PDU_UCI_HARQ:
			ulSchUciInfo_p->pduType = MAC_SS_PDU_INVALID ;
			break;
		default:
			lteWarning("Incorrect ulSchUciInfo_p->pduType = %d\n",
					ulSchUciInfo_p->pduType);
			break;
	}   

}
#endif


/* + PRS_CHANGES */
/*****************************************************************************
 * Function Name  :  addPrsPduToDlConfig
 * Inputs         :  pDlConfigPdu - Pointer to message buffer to fill in the
 *                                  PDU.
 *                   internalCellIndex - cell id  
 * Outputs        :  None 
 * Returns        :  Pointer to the buffer which can be used to fill in the
 *                   next PDU
 * Description    :  This fucntion fills in the message with the PRS PDU data
 *                   and returns the pointer to the buffer which can be used to
 *                   fill in the next PDU.
 *****************************************************************************/
static  FAPI_dlConfigPDUInfo_st *addPrsPduToDlConfig(
		FAPI_dlConfigPDUInfo_st *pDlConfigPdu,
		InternalCellIndex internalCellIndex)
{
	FAPI_dlConfigPDUInfo_st     *pNextPdu = PNULL;

	pDlConfigPdu->pduType = FAPI_PRS_PDU;

	pDlConfigPdu->pduSize
		= MAC_PHY_CONVERT_16( (
					offsetof(FAPI_dlConfigPDUInfo_st, dlConfigpduInfo) +
					sizeof(FAPI_prsPduConfigInfo_st) ) );
/* SPR 19288 change start */
	pDlConfigPdu->dlConfigpduInfo.PRSPdu.transmissionPower = 
		MAC_PHY_CONVERT_16(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
				cellParams_p->prsConfigInfo.prsTransmissionPower);
/* SPR 19288 change end */

	/* pdu size should be a multiple of 4 bytes */ 
	if ( pDlConfigPdu->pduSize % 4 )
	{
		pDlConfigPdu->pduSize += sizeof(UInt32) - \
					 (pDlConfigPdu->pduSize % sizeof(UInt32));
	}

	pNextPdu = (FAPI_dlConfigPDUInfo_st *)(((UInt8 *)pDlConfigPdu)
			+ pDlConfigPdu->pduSize);
	return pNextPdu;
}
/* - PRS_CHANGES */



/*Cyclomatic Complexity changes - starts here*/
/*****************************************************************************
 * Function Name  : handleCommChControlMsg
 * Inputs         : msgType - Variable of Msgtype,
 *		            pPrevPdu - double pointer to structure FAPI_dlConfigPDUInfo_st,
 *                  pNextPdu - double pointer to structure FAPI_dlConfigPDUInfo_st,
 *                  pDciPduInfo - pointer to DCIPduInfo,
 *                  pDciCceInfo - pointer to DciCCEInfo,
 *                  pduIndex - Index of PDU,
 *                  numPdu - number of pdu,
 *                  length - length of msg,
 *                  numPdschRnti - number of PDSCH rnti,
 *                  numDci - number of DCI pdu,
 *                  tempVendorSpecificForDLConfig_p - Pointer to structure 
 *                                                    FAPI_VendorSpecificForDLConfig_st and
 *                  numOfUEs - Number of UE's
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function calls different functions to form DL config
 *                  control message, according to the MsgType.
 ******************************************************************************/
STATIC  void  handleCommChControlMsg(MsgType msgType,
		FAPI_dlConfigPDUInfo_st **pPrevPdu,
		FAPI_dlConfigPDUInfo_st **pNextPdu,
		DCIPduInfo *pDciPduInfo,
		DciCCEInfo *pDciCceInfo,
		UInt32 *pduIndex,
		UInt32 *numPdu,
		UInt32 *length,
		UInt32 *numPdschRnti,
		UInt32 *numDci,
		/* + TM7_EPRE */
		FAPI_VendorSpecificForDLConfig_st *tempVendorSpecificForDLConfig_p, 
		UInt16 *numOfUEs
		/* - TM7_EPRE */
		,InternalCellIndex internalCellIndex)
{
    UInt8 isOnlyOneTb = FALSE;
    isOnlyOneTb = pDciPduInfo->isTBOneValid & pDciPduInfo->isTBTwoValid;
    /* Cov_121046_Fix_Start */
    if(PNULL != *pNextPdu)
    {
        /* Cov_121046_Fix_End */
    switch(msgType)
    {
        /*
           Here we handle the case of common control channel message
           which is sent as a DLSCH PDU to the phy
           */
#ifdef LTE_EMBMS_SUPPORTED
        case EMBMS_DCI1C_NOTIFICATION:
			{
				*pPrevPdu = *pNextPdu;
				*pNextPdu = (*addDciToDLConfigFunction)(*pNextPdu, pDciPduInfo, pDciCceInfo,internalCellIndex);
				if (*pPrevPdu != *pNextPdu)
				{
					/* Increase number of DCI only if add DCI was successful */
					(*numDci)++;
					(*numPdu)++;
					(*length) += ((uintptr_t)*pNextPdu - (uintptr_t)*pPrevPdu);
				}
                break;
            }  
#endif

        case BCCH_MSG:
        case RAR_MSG:
        case CONTENTION_RESL_MSG:
        {
            *pPrevPdu = *pNextPdu;
            *pNextPdu = (*addDciToDLConfigFunction)(*pNextPdu, pDciPduInfo, pDciCceInfo,internalCellIndex);
            if (*pPrevPdu != *pNextPdu)
            {
                /* Increase number of DCI only if add DCI was successful */
                (*numDci)++;
                (*numPdu)++;
                (*length) += ((uintptr_t)*pNextPdu - (uintptr_t)*pPrevPdu);
            }
            /* Fill in DLSCH PDUs */
            *pPrevPdu = *pNextPdu;
            *pNextPdu = (*addDlschToDLConfigFunction)(*pNextPdu, pDciPduInfo,
                    *pduIndex, TB_1_INDEX
                    /* SPR 5807 Fix */
                    /* SPR 9627 Fix start */
                    //#ifdef TDD_CONFIG
                    ,isOnlyOneTb
                    //#endif
                    /* SPR 9627 Fix end */
                    /* SPR 5807 Fix */
                    /* + TM7_EPRE */
                    /* As For ContentionRsoultion Msg, DciFormat is 1A, so 
                     * numOfUEs will not be incremented */
                    ,tempVendorSpecificForDLConfig_p,numOfUEs
                    /* - TM7_EPRE */
                    ,internalCellIndex);
            if (*pPrevPdu != *pNextPdu)
            {
                /* MAC_TTI_LOG Start */
#ifdef SHM_PRINT_ENABLED
                ttiDciLoggingInfo *log_ptr = LOG_TTI_MSG(MAC_TTI_DCI_LOG_INFO);
                if(log_ptr) 
                {
                    fillttiDciLoggingInfo (log_ptr , pDciCceInfo, internalCellIndex);
                }
#endif
                /* MAC_TTI_LOG End */
                (*numPdu)++;
                (*numPdschRnti)++;
                (*pduIndex)++;
                (*length) += ((uintptr_t)*pNextPdu - (uintptr_t)*pPrevPdu);
            }

            LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,
                    "[%s]Send Common MsgType[%d] ( RAR_MSG [3] CCCH_MSG[4] CONTENTION_RESL_MSG [17]",
                    __func__, msgType);
            break; 
        }
        case PCCH_MSG:
        {
            *pPrevPdu = *pNextPdu;
            *pNextPdu = (*addDciToDLConfigFunction)(*pNextPdu, pDciPduInfo, pDciCceInfo,internalCellIndex);
            if (*pPrevPdu != *pNextPdu)
            {
                /* Increase number of DCI only if add DCI was successful */
                (*numDci)++;
                (*numPdu)++;
                (*length) += ((uintptr_t)*pNextPdu - (uintptr_t)*pPrevPdu);
            }

            /* Fill in PCH PDUs */
            *pPrevPdu = *pNextPdu;
            *pNextPdu = addPchPduToDlConfig(*pNextPdu, pDciPduInfo,
                    *pduIndex);
            if (*pPrevPdu != *pNextPdu)
            {
                /* MAC_TTI_LOG Start */
#ifdef SHM_PRINT_ENABLED
                ttiDciLoggingInfo *log_ptr = LOG_TTI_MSG(MAC_TTI_DCI_LOG_INFO);
                if(log_ptr) 
                {
                    fillttiDciLoggingInfo (log_ptr , pDciCceInfo, internalCellIndex);
                }
#endif
                /* MAC_TTI_LOG End */
                (*numPdu)++;
                (*numPdschRnti)++;
                (*pduIndex)++;
                (*length) += ((uintptr_t)*pNextPdu - (uintptr_t)*pPrevPdu);
            }
            break; 
        }
        default:
        {
            break;
        }
    }
        /* Cov_121046_Fix_Start */
    }
    /* Cov_121046_Fix_End */
    /* increment pduIndex for every BCH, MCH, PCH or DLSCH PDU */
}
/*****************************************************************************
 * Function Name  : addDlschPduForInvalidHarqId
 * Inputs         : pDciPduInfo - pointer to DCIPduInfo,
 *                  pPrevPdu   - double pointer to FAPI_dlConfigPDUInfo_st,
 *                  pNextPdu    - double pointer to FAPI_dlConfigPDUInfo_st,
 *                  pduIndex - PDU Index,
 *                  numPdu - number of pdu,
 *                  length - length of msg,
 *                  numPdschRnti - number of PDSCH rnti and
 *                  numOfUEs - Number of UE's
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function calls function to add DLSCH pdu to DL config
 *                  for invalid Harq ID.
 ****************************************************************************/
STATIC  void addDlschPduForInvalidHarqId(DCIPduInfo *pDciPduInfo,
        FAPI_dlConfigPDUInfo_st **pPrevPdu,
        FAPI_dlConfigPDUInfo_st **pNextPdu,
        UInt32 *pduIndex,
        UInt32 *numPdu,
        UInt32 *length,
        UInt32 *numPdschRnti,
        /* + TM7_EPRE */
        FAPI_VendorSpecificForDLConfig_st *tempVendorSpecificForDLConfig_p, 
        UInt16 *numOfUEs
        /* - TM7_EPRE */
        ,InternalCellIndex internalCellIndex)


{

    /* SPR 5807 Fix */       
    /* SPR 9627 Fix start */
    //#ifdef TDD_CONFIG
    UInt8 isOnlyOneTb = FALSE;
    isOnlyOneTb = pDciPduInfo->isTBOneValid & pDciPduInfo->isTBTwoValid;
    //#endif
    /* SPR 9627 Fix end */
    /* SPR 5807 Fix */       

    /* SPR 941 */
    if ( TRUE == pDciPduInfo->isTBOneValid )
    {
        LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] isTBOneValid valid \n ", __func__);
        /* Fill in DLSCH PDUs */
        *pPrevPdu = *pNextPdu;
        *pNextPdu = (*addDlschToDLConfigFunction)(*pNextPdu, pDciPduInfo,
                *pduIndex, TB_1_INDEX
                /* SPR 5807 Fix */
                /* SPR 9627 Fix start */
                //#ifdef TDD_CONFIG
                ,isOnlyOneTb
                //#endif
                /* SPR 9627 Fix end */
                /* SPR 5807 Fix */
                /* + TM7_EPRE */
                ,tempVendorSpecificForDLConfig_p,numOfUEs
                /* - TM7_EPRE */
                ,internalCellIndex);

        if (*pPrevPdu != *pNextPdu)
        {
            (*numPdu)++;
            (*numPdschRnti)++;
            (*pduIndex)++;
            (*length) += ((uintptr_t)*pNextPdu - (uintptr_t)*pPrevPdu);
        }
    }
    /* SPR 941 */
    /* Fill in DLSCH PDUs */
    if ( TRUE == pDciPduInfo->isTBTwoValid )
    {
        LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_TRANSMITTER,"[%s] isTBTwoValid valid \n ", __func__);

        *pPrevPdu = *pNextPdu;
        *pNextPdu = (*addDlschToDLConfigFunction)(*pNextPdu, pDciPduInfo,
                *pduIndex, TB_2_INDEX
                /* SPR 5807 Fix */
                /* SPR 9627 Fix start */
                //#ifdef TDD_CONFIG
                ,isOnlyOneTb
                //#endif
                /* SPR 9627 Fix end */
                /* SPR 5807 Fix */
                /* + TM7_EPRE */
                /* As TB2 can never be valid for TM7, so numOfUEs will not be incremented for TB2 */
                ,tempVendorSpecificForDLConfig_p,numOfUEs
                /* - TM7_EPRE */
                ,internalCellIndex);
        if (*pPrevPdu != *pNextPdu)
        {
            (*numPdu)++;
			if(isOnlyOneTb==FALSE)
			{
			    (*numPdschRnti)++;
			}
            (*pduIndex)++;
            (*length) += ((uintptr_t)*pNextPdu - (uintptr_t)*pPrevPdu);
        }
    }
}
/*****************************************************************************
 * Function Name  : addDlschpduforvalidharqid
 * Inputs         : pDciPduInfo - pointer to DCIPduInfo,
 *                  pPrevPdu - double pointer to FAPI_dlConfigPDUInfo_st,
 *                  pNextPdu - double pointer to FAPI_dlConfigPDUInfo_st,
 *                  pduIndex - pdu index,
 *                  numPdu - number of pdu,
 *                  length - length of msg,
 *                  numPdschRnti - number of pdsch rnti,
 *                  subFrame - current subframe,
 *                  isTBOneValid - tb1 flag of harqprocess,
 *                  isTBTwoValid - tb2 flag of harqprocess,
 *                  tempVendorSpecificForDLConfig_p - pointer to structure
 *                                                    FAPI_VendorSpecificForDLConfig_st  and
 *                  numOfUEs - Number of UE's
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function calls function to add dlsch pdu to dl config
 *                  for valid harq id.
 ****************************************************************************/

STATIC  void addDlschPduForValidHarqId(DCIPduInfo *pDciPduInfo,
		FAPI_dlConfigPDUInfo_st **pPrevPdu,
		FAPI_dlConfigPDUInfo_st **pNextPdu,
		UInt32 *pduIndex,
		UInt32 *numPdu,
		UInt32 *length,
		UInt32 *numPdschRnti,
        /* +- SPR 17777 */
#ifdef UTFWK_SIMULATION
		UInt32 subFrame,
#endif
        /* +- SPR 17777 */
		UInt8 isTBOneValid,
		UInt8 isTBTwoValid,
		/* + TM7_EPRE */
		FAPI_VendorSpecificForDLConfig_st *tempVendorSpecificForDLConfig_p, 
		UInt16 *numOfUEs
		/* - TM7_EPRE */
		,InternalCellIndex internalCellIndex)
{

	/* SPR 5807 Fix */       
	/* SPR 9627 Fix start */
	//#ifdef TDD_CONFIG
	UInt8 isOnlyOneTb = FALSE;
	isOnlyOneTb = isTBOneValid & isTBTwoValid;
	//#endif 
	/* SPR 9627 Fix end */
	/* SPR 5807 Fix */       
	/* SPR 941 */
	if (TRUE== isTBOneValid )
	{
		/* Fill in DLSCH PDUs */
		*pPrevPdu = *pNextPdu;
		*pNextPdu = (*addDlschToDLConfigFunction)(*pNextPdu, 
				pDciPduInfo, *pduIndex, TB_1_INDEX
				/* SPR 5807 Fix */
				/* SPR 9627 Fix start */
				//#ifdef TDD_CONFIG
				,isOnlyOneTb
				//#endif
				/* SPR 9627 Fix end */
				/* SPR 5807 Fix */
				/* + TM7_EPRE */
				,tempVendorSpecificForDLConfig_p,numOfUEs,internalCellIndex);
		/* - TM7_EPRE */

		if (*pPrevPdu != *pNextPdu)
		{
			(*numPdu)++;
			(*numPdschRnti)++;
			(*pduIndex)++;
			(*length) += ((uintptr_t)*pNextPdu - (uintptr_t)*pPrevPdu);
#ifdef UTFWK_SIMULATION
			if( SPS_OCCASION == pDciCceInfo->msgType )
			{
				UInt32 rntiType = getRntiTypeForRnti(
						pDciPduInfo->rnti,internalCellIndex);
				addEntryInHarqList( pDciPduInfo->ueIndex, 
						subFrame, rntiType );
			}
#endif
		}
	}
	/* SPR 1388 Start */
	else
	{
		pDciPduInfo->isTBOneValid = FALSE;
		pDciPduInfo->tb1Info.modulationType = 0;
		pDciPduInfo->tb1Info.redundancyVer = 1;
	}
	/* SPR 1388 End*/
	/* SPR 941 */
	if ( isTBTwoValid )
	{
		/* Fill in DLSCH PDUs */
		*pPrevPdu = *pNextPdu;
		*pNextPdu = (*addDlschToDLConfigFunction)(*pNextPdu, pDciPduInfo,
				*pduIndex, TB_2_INDEX
				/* SPR 5807 Fix */
				/* SPR 9627 Fix start */
				//#ifdef TDD_CONFIG
				,isOnlyOneTb
				//#endif
				/* SPR 9627 Fix end */
				/* SPR 5807 Fix */
				/* + TM7_EPRE */
				/* As TB2 can never be valid for TM7, so numOfUEs will not be incremented for TB2 */
				,tempVendorSpecificForDLConfig_p,numOfUEs
				/* - TM7_EPRE */
				,internalCellIndex);
		if (*pPrevPdu != *pNextPdu)
		{
			(*numPdu)++;
            if(isOnlyOneTb==FALSE)
            {
                (*numPdschRnti)++;
            }
			
			(*pduIndex)++;
			(*length) += ((uintptr_t)*pNextPdu - (uintptr_t)*pPrevPdu);
		}
	}
	/* SPR 1388 Start */
	else
	{
		pDciPduInfo->isTBTwoValid = FALSE;
		pDciPduInfo->tb2Info.modulationType = 0;
		pDciPduInfo->tb2Info.redundancyVer = 1;
	}
	/* SPR 1388 End */

}
/*****************************************************************************
 * Function Name  : addDlschDciPduForCCCHMsg
 * Inputs         : pDciPduInfo - pointer to DCIPduInfo,
 *                  pPrevPdu - double pointer to FAPI_dlPduInfo_st,
 *                  pNextPdu - double pointer to FAPI_dlPduInfo_st,
 *                  pduIndex - pdu index,
 *                  numPdu - number of pdu,
 *                  length - length of msg
 * Outputs        : None
 * Returns        : None
 * Description    : This function calls function to add dlsch Pdu 
 *                  for data for CCCH msg Type.
 ****************************************************************************/
STATIC  void addDlschDciPduForCCCHMsg(DCIPduInfo *pDciPduInfo,
		FAPI_dlPduInfo_st **pPrevPdu,
		FAPI_dlPduInfo_st **pNextPdu,
		UInt32 *pduIndex,
		UInt32 *numPdu,
		UInt32 *length
/* SPR 19288 change start */
/* SPR 19288 change end */
#if defined(FLEXRAN)
                ,InternalCellIndex internalCellIndex
#endif
)
{
	/* SPR 941 */
	if ( pDciPduInfo->isTBOneValid )
	{
		*pNextPdu = addDlschDciPduToDlData(*pNextPdu, pDciPduInfo,
				*pduIndex, TB_1_INDEX
/* SPR 19288 change start */
/* SPR 19288 change end */
#if defined(FLEXRAN)
               ,internalCellIndex
#endif

                                                  );
		msgFree(pDciPduInfo->dataTB_p);
		pDciPduInfo->dataTB_p = PNULL;
		if (*pPrevPdu != *pNextPdu)
		{
			(*numPdu)++;
			(*pduIndex)++;
			(*length) += ((uintptr_t)*pNextPdu - (uintptr_t)*pPrevPdu);
/* SPR 19288 change start */
/* SPR 19288 change end */

		}
	}

	/* SPR 941 */
	if( pDciPduInfo->isTBTwoValid )
	{
		*pPrevPdu = *pNextPdu;
		*pNextPdu = addDlschDciPduToDlData(*pNextPdu, pDciPduInfo,
				*pduIndex, TB_2_INDEX
/* SPR 19288 change start */
/* SPR 19288 change end */
#if defined(FLEXRAN)
              ,internalCellIndex
#endif
                                                  );
		msgFree(pDciPduInfo->dataTB_p);
		pDciPduInfo->dataTB_p = PNULL;
		if (*pPrevPdu != *pNextPdu)
		{
			(*numPdu)++;
			(*pduIndex)++;
			(*length) += ((uintptr_t)*pNextPdu - (uintptr_t)*pPrevPdu);
/* SPR 19288 change start */
/* SPR 19288 change end */

		}

	}
}
/*****************************************************************************
 * Function Name  : handleCommChDataMsg
 * Inputs         : msgType - MsgType Variable,
 *                  pDciPduInfo - pointer to DCIPduInfo,
 *		            pPrevPdu - double pointer to FAPI_dlPduInfo_st,
 *                  pNextPdu - double pointer to FAPI_dlPduInfo_st,
 *                  pduIndex - PDU Index,
 *                  numPdu - number of pdu,
 *                  length - length of msg,
 *                  sysFrameNum - currebnt SFN and
 *                  subFrameNum - current SF
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function calls different functions to form DL config
 *                  data message, according to the MsgType.
 ******************************************************************************/

STATIC  void handleCommChDataMsg(MsgType msgType,
		DCIPduInfo *pDciPduInfo,
		FAPI_dlPduInfo_st **pPrevPdu,
		FAPI_dlPduInfo_st **pNextPdu,
		UInt32 *pduIndex,
		UInt32 *numPdu,
		UInt32 *length,
		UInt32 sysFrameNum,
		UInt32 subFrameNum,
		InternalCellIndex internalCellIndex
/* SPR 19288 change start */
/* SPR 19288 change end */
                )
{                                       
	switch(msgType)
	{
		/*
		   Here we handle the case of common control channel message
		   which is sent as a DLSCH PDU to the phy
		 */
		/* SPR 1613 changes start */            
		//           case CCCH_MSG:
		/* SPR 1613 changes end */            
		case BCCH_MSG:
		case RAR_MSG:
		case CONTENTION_RESL_MSG:
			{
				/* Fill in DLSCH PDUs */
				*pPrevPdu = *pNextPdu;
				*pNextPdu = addDlschDciPduToDlData(*pNextPdu, pDciPduInfo,
						*pduIndex, TB_1_INDEX
/* SPR 19288 change start */
/* SPR 19288 change end */                                                            
#if defined(FLEXRAN)
        ,internalCellIndex
#endif
                                                                  );
				if (*pPrevPdu != *pNextPdu)
				{
					(*numPdu)++;
					(*pduIndex)++;
					(*length) += ((uintptr_t)*pNextPdu - (uintptr_t)*pPrevPdu);
/* SPR 19288 change start */
/* SPR 19288 change end */

				}
				{
					msgFree(pDciPduInfo->dataTB_p);
					pDciPduInfo->dataTB_p = PNULL;     
				}    
				break; 
			}
		case PCCH_MSG:
			{
				/* Fill in PCH PDUs */
				*pPrevPdu = *pNextPdu;
				*pNextPdu = addPchPduToDlData(*pNextPdu, pDciPduInfo,
						*pduIndex
/* SPR 19288 change start */
#ifdef FLEXRAN
	, internalCellIndex
#endif
/* SPR 19288 change end */
                                                             );
				msgFree(pDciPduInfo->dataTB_p);
				pDciPduInfo->dataTB_p = PNULL;
				if (*pPrevPdu != *pNextPdu)
				{
					(*numPdu)++;
					(*pduIndex)++;
					(*length) += ((uintptr_t)*pNextPdu - (uintptr_t)*pPrevPdu);
/* SPR 19288 change start */
/* SPR 19288 change end */
				}
				/* Free the Paging node */
				freePagingMsg(sysFrameNum, subFrameNum,internalCellIndex);
				break; 
			}
		default:
			{
				break;
			}
	}
}


/*****************************************************************************
 * Function Name  : processAllocatedCCEArray
 * Inputs         : currentGlobalTick - Current global tick,
 *                  dciCCEContainerInfo_p - pointer to CCE Container,
 *		            pPrevPdu - double pointer to FAPI_dlPduInfo_st,
 *                  pNextPdu - double pointer to FAPI_dlPduInfo_st,
 *                  pTxReq - double pointer to PHY_TX_REQ,
 *                  pduIndex - PDU Index,
 *                  numPdu - number of pdu,
 *                  length - length of msg,
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes allocated CCE and add it to DCI PDU.
 ******************************************************************************/
 /*SPR 15909 fix start*/
void processAllocatedCCEArray(tickType_t currentGlobalTick,
 /*SPR 15909 fix end*/
		DciCCEContainerInfo *dciCCEContainerInfo_p,
		FAPI_dlPduInfo_st **pNextPdu,
		FAPI_dlPduInfo_st **pPrevPdu,
        UInt32 *pduIndex,
        UInt32 *numPdu,
        UInt32 *length,
        InternalCellIndex internalCellIndex
/* SPR 19288 change start */
/* SPR 19288 change end */
        )
{
    UInt8      cceCount  = 0;
    UInt32     cceNumber = 0;
    DCIPduInfo *pDciPduInfo = PNULL;
    DciCCEInfo *pDciCceInfo = PNULL;
#ifdef KPI_STATS
	UInt32 activeIndexForMacStats = (gMacStats.
			pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
#endif
	/*EICIC +*/
	UInt32 sysFrameNum 			= 0;
	UInt32 subFrameNum 			= 0;
	UInt8  absSubFrameNum 		= 0;
	UInt8 numBitsAbsPattern = 0;

#ifdef FDD_CONFIG
	numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
#else
	if (UL_DL_CONFIG_0 == cellSpecificParams_g.cellConfigAndInitParams_p\
			[internalCellIndex]->cellParams_p->subFrameAssign)
	{
		numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
	}
	else if (UL_DL_CONFIG_6 == cellSpecificParams_g.cellConfigAndInitParams_p\
			[internalCellIndex]->cellParams_p->subFrameAssign)
	{
		numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
	}
	else
	{
		numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
	}
#endif

	getSFAndSFN(&sysFrameNum, &subFrameNum,internalCellIndex);

	absSubFrameNum = ((((sysFrameNum )*MAX_SUB_FRAME) +(subFrameNum + PHY_DL_DELAY)) %numBitsAbsPattern) ;

	/* Check if absUsageRequired is set then update the AbsSubframeCount counter along with numABSoccasionsKpi*/
	if (LTE_TRUE == usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][absSubFrameNum])
	{
		if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportConfigParams.absUsageRequired)
		{
			rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p[ rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->activeReportIndexEicic].numABSoccasionsKpi++;	
		} 
#ifdef KPI_STATS
		gMacStats.pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats].AbsSubframeCount++;
#endif 

	}
	/* EICIC - */

	cceCount = dciCCEContainerInfo_p->countOfCCEAllocated;    
	while (cceCount--)
	{
		cceNumber = dciCCEContainerInfo_p->arrayCCEAllocated[cceCount];
		pDciCceInfo = &(dciCCEContainerInfo_p->dciCCEInfo[cceNumber]);
		/*
		   check validity of the cceIndex.
		 */
		if (!pDciCceInfo->isCCEFree)
		{    
			/* + Coverity_31400 */  
			pDciPduInfo = &pDciCceInfo->dciPduInfo;
			if (pDciPduInfo->harqProcessId != INVALID_HARQ_ID)
				/* - Coverity_31400 */  
			{
				/* Fill in DLSCH PDUs */
                *pPrevPdu = *pNextPdu;
#ifdef UTFWK_SIMULATION
				if (pDciPduInfo->dciFormat == DCI_FORMAT_0)
#else
					/* 3778_FIX_START */
					if (pDciPduInfo->dciFormat == DCI_FORMAT_0 ||
							pDciPduInfo->dciFormat == DCI_FORMAT_3 ||
							pDciPduInfo->dciFormat == DCI_FORMAT_3A ||
							pDciPduInfo->dciFormat == MAX_NUM_DCI_FORMAT)
						/* 3778_FIX_END */
#endif
					{
						continue;
					}
				/* SPR_4729_CHANGES_START */
				if (PNULL == pDciPduInfo->dataTB_p && PNULL == pDciPduInfo->dataTB2_p)
				{
					/* SPR 5993 Fix Start*/
					DLUEContextInfo *ueContextInfo_p = PNULL;
					DLHARQProcess *harqProcess_p = PNULL;

					/* Continue in case of DCI is only sent and no DLSCH 
					 * is present */
					if( PDCCH_ORDER_MSG == pDciCceInfo->msgType ||
							SPS_DEACTIVATION ==  pDciCceInfo->msgType ||
							TPC_MSG == pDciCceInfo->msgType )
					{
						continue;
					}
					/* SPR 10145 fix start */
					if(pDciCceInfo->isCCEFree || INVALID_HARQ_ID == pDciPduInfo->harqProcessId)
					{
						/* CCEs already reset */
					}
					else
					{
						if(!(dlUECtxInfoArr_g[pDciPduInfo->ueIndex].pendingDeleteFlag))
						{
							DLUEContext *dlUEContext_p = dlUECtxInfoArr_g[pDciPduInfo->ueIndex].dlUEContext_p;
							if(PNULL != dlUEContext_p)
							{
								/* + SPR 11469 Changes */
								DLHARQEntity *dlHarqContext_p = PNULL;
								if(IS_PCELL_OF_UE(dlUEContext_p,internalCellIndex))
								{
									dlHarqContext_p = dlUEContext_p->dlHarqContext_p;
								}
								else
								{
									dlHarqContext_p = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlHarqContext_p;
								}
								/* - SPR 11469 Changes */
								if((PNULL != dlHarqContext_p) && (INVALID_HARQ_ID != pDciPduInfo->harqProcessId) )
								{
									freeDLHarqProcess(dlUEContext_p, 
											&dlHarqContext_p->harqProcess[pDciPduInfo->harqProcessId],
											internalCellIndex);
								}
								else
								{
									/* HARQ ID valid or CCE allocated */
									/* Probable Crash Saved : HARQ Process 
									 * free by another thread */
									continue;
								}
							}
							else
							{
								/* HARQ ID valid or CCE allocated */
								/* Probable Crash Saved : DLUE context
								 * free by another thread */
								continue;
							}
						}
					}
					/* SPR 10145 fix end */

					if(MAX_UE_SUPPORTED > pDciPduInfo->ueIndex)
					{
						ueContextInfo_p = &dlUECtxInfoArr_g[pDciPduInfo->ueIndex];
						/* + SPR 11469 Changes */
						if(IS_PCELL_OF_UE(ueContextInfo_p->dlUEContext_p,internalCellIndex))
						{
							harqProcess_p =
								&ueContextInfo_p->dlUEContext_p->dlHarqContext_p
								->harqProcess[pDciPduInfo->harqProcessId];
						}
						else
						{
							harqProcess_p =
								&ueContextInfo_p->dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->
								ScelldlHarqContext_p->harqProcess[pDciPduInfo->harqProcessId];
						}
						/* - SPR 11469 Changes */
						mac_reset_cce(pDciPduInfo->aggregationLvl, pDciCceInfo,internalCellIndex);
						/*pdcch for scell changes start*/
						mac_reset_extra_cce( dciCCEContainerInfo_p,pDciCceInfo,ueContextInfo_p->dlUEContext_p,internalCellIndex);
						/*pdcch for scell changes end*/
						/*COVERITY:31400:START*/
						if(harqProcess_p->harqProcessId != INVALID_HARQ_ID)
						{
							/*COVERITY:31400:END*/
							freeDLHarqProcess(ueContextInfo_p->dlUEContext_p, harqProcess_p,internalCellIndex);
							/*COVERITY:31400:START*/
						}
						/*COVERITY:31400:END*/
						/* Coverity 61998 + SPR 9203 Fix Starts*/
						/* + SPR 11341 Changes */
						/* No need to delete schedule Lc List here */
						/*
						   if(DL_QOS_BASED_SCHEDULING == 
						   cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
						   dlSchdConfig.dlSchedulerStrategy) 
						   {
						   if (dlUECtxInfoArr_g[pDciPduInfo->ueIndex].dlUEContext_p != PNULL)
						   {
						   deleteScheduledLCList( 
						   dlUECtxInfoArr_g[pDciPduInfo->ueIndex].dlUEContext_p, internalCellIndex);
						   }
						   }
						 */
						/* - SPR 11341 Changes */
						/* Coverity 61998 + SPR 9203 Fix Ends*/
					}

					/* SPR 5993 Fix End*/
					continue;
				}
				/* SPR_4729_CHANGES_END */
				{
					LOG_UT(MAC_DATA_TX_ID,LOGDEBUG,MAC_L1_INF,\
							currentGlobalTick,\
							pDciPduInfo->ueIndex,\
							pDciPduInfo->harqProcessId,\
							pDciPduInfo->dciFormat,\
							pDciPduInfo->tb1Info.tbSize,\
							((pDciPduInfo->isTBTwoValid)? pDciPduInfo->tb2Info.tbSize : 0),\
							0.0,0.0,__func__,"DATA_PDU_TX");
				}            
				LOG_MAC_MSG(MAC_DATA_TX_ID2,LOGBRIEF,MAC_L1_INF,\
						currentGlobalTick,\
						pDciPduInfo->ueIndex,\
						pDciPduInfo->dciFormat,\
						pDciPduInfo->tb1Info.tbSize,\
						((pDciPduInfo->isTBTwoValid)? pDciPduInfo->tb2Info.tbSize : 0),\
						0,0.0,0.0,__func__,"DATA_PDU_TX");
				/* SPR 941 */
				if ( pDciPduInfo->isTBOneValid )
				{
#ifndef UTFWK_SIMULATION
                    *pNextPdu = addDlschDciPduToDlData(*pNextPdu, pDciPduInfo,
                            *pduIndex, TB_1_INDEX
/* SPR 19288 change start */
/* SPR 19288 change end */
#if defined(FLEXRAN)
        ,internalCellIndex
#endif

                                                      );
#endif	
                    if (*pPrevPdu != *pNextPdu)
                    {
                        (*numPdu)++;
                        (*pduIndex)++;
                        (*length) += ((uintptr_t)(*pNextPdu) - (uintptr_t)(*pPrevPdu));
/* SPR 19288 change start */
/* SPR 19288 change end */

                    }
                    /* SPR_4729_CHANGES_START */
                    pDciPduInfo->dataTB_p = PNULL;
                    /* SPR_4729_CHANGES_END */
                }

                /* SPR 941 */
                if( pDciPduInfo->isTBTwoValid )
                {
                    *pPrevPdu = *pNextPdu;
#ifndef UTFWK_SIMULATION
                    *pNextPdu = addDlschDciPduToDlData(*pNextPdu, pDciPduInfo,
                            *pduIndex, TB_2_INDEX
/* SPR 19288 change start */
/* SPR 19288 change end */
#if defined(FLEXRAN)
        ,internalCellIndex
#endif

                                                      );
#endif
                    if (*pPrevPdu != *pNextPdu)
                    {
                        (*numPdu)++;
                        (*pduIndex)++;
                        (*length) += ((uintptr_t)(*pNextPdu) - (uintptr_t)(*pPrevPdu));
/* SPR 19288 change start */
/* SPR 19288 change end */
                    }
					/* SPR_4729_CHANGES_START */
					pDciPduInfo->dataTB2_p = PNULL; 
					/* SPR_4729_CHANGES_END */

				}
				/* SPS_CHG */
				if (SPS_OCCASION == pDciCceInfo->msgType)
				{

					/*KlockWork Warning Changes Start */
					/*                    if (pDciCceInfo->isCCEFree)
							      {
							      ltePanic("pDciCceInfo->isCCEFree is wrong[%d] \n", pDciCceInfo->isCCEFree);    
							      }
							      else
							      {
					 */
					/*KlockWork Warning Changes End */
					pDciCceInfo->isCCEFree = CCE_FREE; 
					/*KlockWork Warning Changes Start */
					/*                    }
					 */
					/*KlockWork Warning Changes End */
				}
				else
				{
					/* DL DATA SEP Changes */
					//mac_reset_cce(pDciPduInfo->aggregationLvl, pDciCceInfo);
				}

				/* 3778_FIX_START */
				/* DL DATA SEP Changes */
				//  pDciPduInfo->harqProcessId = INVALID_HARQ_ID;
				/* 3778_FIX_END */
				/*EICIC +*/
				/* Calculate the absPRBUsage to be reported to RRM in EICIC
				   PERIODIC REPORTS*/
				if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportConfigParams.absUsageRequired)
				{
					/* EICIC_Coverity CID 69797 Start */
					//if (NULL != pDciCceInfo)
					//{
					/* EICIC_Coverity CID 69797 End */
					if (NULL != dlUECtxInfoArr_g[pDciCceInfo->dciPduInfo.ueIndex].dlUEContext_p)
					{
						if(VICTIM_UE == dlUECtxInfoArr_g[pDciCceInfo->dciPduInfo.ueIndex].dlUEContext_p->userType)
						{
							rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->\
								eicicReportInd.dlEICICReportInfo_p[rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->\
								activeReportIndexEicic].absPRBUsage+=pDciPduInfo->numOfRBs ;
						}	   
					}
					/* EICIC_Coverity CID 69797 Start */	   
					//}
					/* EICIC_Coverity CID 69797 End */	
				}

				/* EICIC -*/
			} 
			else if(CCCH_MSG == pDciCceInfo->msgType)
			{
				/* Fill in DLSCH PDUs */
                *pPrevPdu = *pNextPdu;
				pDciPduInfo = &pDciCceInfo->dciPduInfo;
				if ((pDciPduInfo->dciFormat == DCI_FORMAT_0) || 
						(pDciPduInfo->dciFormat == DCI_FORMAT_0_MSG3) ||    
						(pDciPduInfo->dataTB_p == NULL))
				{
					continue;
				}

				/*Cyclomatic Complexity changes - starts here */
		addDlschDciPduForCCCHMsg(pDciPduInfo,pPrevPdu,pNextPdu,pduIndex,numPdu,length
/* SPR 19288 change start */
/* SPR 19288 change end */                           
#if defined(FLEXRAN)
        ,internalCellIndex
#endif
                                       );
			}
			/* SPR 10411 changes start */ 
#ifdef KPI_STATS
			gMacStats.pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats].totalDLPRBUsage += 
				pDciPduInfo->numOfRBs ;
#endif
			/* SPR 10411 changes end */ 

		}/* End of if(tempDciPduInfo_p->cceIndex ) */
	}
	return;
}


