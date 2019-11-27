/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacDLUEContext.h,v 1.1.1.1.6.1.4.2.2.1 2010/09/21 15:45:48 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains Downlink related Data structures 
 *                     and global variables.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacDLUEContext.h,v $
 * Revision 1.1.1.1.6.1.4.2.3.1  2012/07/20 14:49:48  gur31292
 * Changes related to DCI Format3A
 *
 * Revision 1.1.1.1.6.1.4.2.2.1  2010/09/21 15:45:48  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.21.34.1  2010/07/26 09:28:02  gur23951
 * Added variable "newDataIndicator" in DLHARQTBInfo structure
 *
 * Revision 1.21  2009/08/02 10:24:51  gur12140
 * Added ratPolicyUsed Parameter
 *
 * Revision 1.20  2009/07/31 09:08:36  gur18569
 * removed lteMacRlcPdcpInterface.h
 *
 * Revision 1.19  2009/07/15 07:00:59  gur19413
 * added new header file
 *
 * Revision 1.18  2009/07/15 06:43:15  gur19413
 * header file name changed.
 *
 * Revision 1.17  2009/07/09 08:21:33  gur18550
 * Removed ueIndex from TransmissionAndRlcData
 *
 * Revision 1.16  2009/07/07 08:08:22  gur20052
 * changes related to the HARQ implementation
 *
 * Revision 1.15  2009/07/03 06:25:36  gur12140
 * Incorporated Comments
 *
 * Revision 1.14  2009/07/01 04:22:01  gur15697
 * review comments incorporated
 *
 * Revision 1.13  2009/06/23 12:47:03  gur18550
 * removed numLCsInRLCDataArr
 *
 * Revision 1.12  2009/06/17 13:28:40  gur18550
 * UInt32 sizeTBTwo
 *
 * Revision 1.11  2009/06/15 13:08:17  gur12140
 * Added WideBand PMI Index and RankIndicator in DL UE Context Structure
 *
 * Revision 1.10  2009/06/12 07:06:50  gur11083
 * adding new Paramter dlCodingRate in DLUEContext
 *
 * Revision 1.9  2009/06/08 15:43:25  gur12140
 * Removed Linking Errors
 *
 * Revision 1.8  2009/06/08 10:07:14  gur18550
 * TransmissionRLCBuffer
 *
 * Revision 1.7  2009/06/08 09:57:02  gur18550
 * TransmissionRLCBuffer
 *
 * Revision 1.6  2009/06/05 15:21:13  gur18550
 * File Updated
 *
 * Revision 1.5  2009/06/02 07:11:51  gur20052
 * updated the name of pointer to ueNonZeroQueueLoadList_g to incorporated comments
 *
 * Revision 1.4  2009/05/28 12:25:48  gur20052
 * Added the harqExpiry Tick
 *
 * Revision 1.3  2009/05/28 06:50:46  gur18550
 * harExpiryTick
 *
 * Revision 1.2  2009/05/28 05:58:45  gur12140
 * Changes variable name of DLHARQTBInfo
 *
 * Revision 1.1  2009/05/27 17:19:00  gur19413
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_DL_UE_CONTEXT_H
#define LTE_MAC_DL_UE_CONTEXT_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacRlcInterface.h"
#include "lteMacCQI.h"
/* + CL-MIMO LJA*/ 
/* +COVERITY 10525 */
#include "lteMacParseUtil.h"
/* - CL-MIMO LJA*/
#include "lteMacPowerControl.h"
#include "lteMacSFNMgr.h"
/* +COVERITY 10525 */
#include "lteMacDRXMgr.h" /* DRX_CHG */
#include "lteMacSpsCommonTypes.h"
#include "lteMacPowerControl.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/**************************************************************************n
 * Exported Definitions
 ****************************************************************************/
typedef YTNODE DlPriorityLcNode;
typedef YTREE MACPriorityLcTree;


/****************************************************************************
 * Exported Types
 ****************************************************************************/

#ifdef TDD_CONFIG
#define MAX_DL_TIMER_CONTAINER 2
#endif
/* + CQI_5.0 */
#define IMMEDIATE_DL_RI_UPDATE   0
#define LATER_DL_RI_UPDATE       2
/* - CQI_5.0 */
#define INVALID_DL_DEACT_CMD_THLD 0



typedef enum DLScheduleFlagT
{
    DL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON,
    DL_QOS_DRX_ON_MEAS_GAP_ON,
    DL_QOS_DRX_ON_SYNC_LOSS_ON,
    DL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON,
    DL_QOS_DRX_ON,
    DL_QOS_MEAS_GAP_ON,
    DL_QOS_SYNC_LOSS_ON,
    DL_QOS_NO_TRANSMISSION,
    DL_QOS_NO_NEW_TRANSMISSION,
    DL_QOS_ALREADY_TRANSMITED,
    DL_QOS_NEW_TRANSMISSION,
    DL_QOS_MAX_STATE,
}DLScheduleFlag;
/*ca chngs start*/
typedef enum DLCAScellStateFlagT
{
    DL_CA_SCELL_INIT,
    DL_CA_SCELL_DEACTIVATED,
    DL_CA_SCELL_CONFIGURED,
    DL_CA_SCELL_ACTIVATION_ON_GOING,
    DL_CA_SCELL_ACTIVE,
    DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_MAC,
    DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_RRM,
    MAX_NUM_DL_CA_SCELL_STATE 
}DLCAScellStateFlag;

typedef enum DLCAQosEventT
{
    DL_CA_EVENT_RRC_UE_CREATE,
    DL_CA_EVENT_SCELL_RELEASE,
    DL_CA_EVENT_RRM_SCELL_DEACTIVATION,
    DL_CA_EVENT_RRM_SCELL_ACTIVATION,
    DL_CA_EVENT_MAC_SCELL_DEACT_CE_TRIGGER,
    DL_CA_EVENT_MAC_SCELL_ACT_CE_TRIGGER,
    DL_CA_EVENT_MAC_SCELL_ACT_CE_SENT,
    DL_CA_EVENT_DEACTIVATION_TIMER_EXPIRE,
    DL_CA_EVENT_ACTIVATION_TIMER_EXPIRE,
    DL_CA_EVENT_RRM_DEACTIVATION_MAC_CE_SENT,
    DL_CA_EVENT_MAC_DEACTIVATION_MAC_CE_SENT,
    DL_CA_EVENT_RRM_DEACTIVATION_TIMER_EXPIRE,
/*SPR 17561 FIX*/
#ifdef FDD_CONFIG
    /* SPR 16026 changes start */
    DL_CA_EVENT_DEACT_MAC_CE_HARQ_FAILURE,
    /*If max dl re-transmission reached */
    DL_CA_EVENT_ACT_MAC_CE_HARQ_FAILURE,
    /* SPR 16026 changes end */
    /*if incase of NACK but max re-tx not reached*/
    DL_CA_EVENT_ACT_MAC_CE_HARQ_NACK,
#endif
/*SPR 17561 FIX*/
    INVALID_DL_CA_EVENT_MAC_SCELL_CE,
    MAX_NUM_DL_CA_EVENT  
}DLCAQosEvent;
/*ca chngs end*/

typedef struct TransmissionAndRLCDataT
{
    /* This array is populated by RLC , Each array element
     * corresponds to one LC with requested size specified.
     * RLC creates data buffer for each LC and size of data
     * filled is put into bufferLength
     */
    RLCDataBuffer rlcDataBufferArr[MAX_NUMBER_OF_LOGICAL_CHANNEL];

    /* The harq process id to which this transmissionAndRLCData
     * belongs to*/
    UInt8 harqProcessId;

    UInt8 cceIndex;

    /*If this value is set , this means putEntryInEncoderQueue will
     * put dummy nodes in all queues at that tick to indicate end of
     * encoding requests for that tick*/
    UInt8 endIndication;

    UInt8 lcCountTBOne;
    UInt8 lcCountTBTwo;
    UInt8 tbTwoLCStartIndex;

    /* This field indicates number of valid TBs or number TBs using this
     * TransmissionAndRLCData buffer , when this is equal to zero the
     * TransmissionAndRLCData can be freed*/
    UInt8 numValidTBs;
     /* _RLC_AM_MODE_ */

    /** CA Phase2 Changes Start **/
    UInt8 encoderCellIndex ;
    /** CA Phase2 Changes End **/

    /* scheduledTick is used to validate whether the encoding request
     * has arrived at right tick or not , whether the packet will
     * reach phy in the right tick or not.*/
    /* SPR 15909 fix start */
    tickType_t scheduledTick;
    /* SPR 15909 fix end */

    /* This field will valid only in case the field forTB of
     * RLCDataBuffer is set to TB_ONE */
    UInt32 paddingBytesForTBOne;

    /* This field will valid only in case
     * the field forTB of  RLCDataBuffer is set to TB_TWO */
    UInt32 paddingBytesForTBTwo;

    /* size of first TB */
    UInt32 sizeTBOne;

    /* size of second TB */
    UInt32 sizeTBTwo;

    RLCHarqFailureInfo  rlcHarqFailureInfo[MAX_NUMBER_OF_LOGICAL_CHANNEL];
}TransmissionAndRLCData;

typedef struct DLHARQTBInfoT
{
    /* The enum which signifies the type of DATA */
    MsgType   taDrxFlag;
    /* This field indicates whether the transmission is RE or new */
    UInt8   txIndicator;
    /* The following field contains the no. of re-transmission of the HARQ */
    UInt8   current_TX_NB;
    /* This field is related to the redundancy version */
    UInt8   irVersion;
    /* The modelling and coding index */
    UInt8   mcsIndex;
    /* This field denmotes the TB SIZE expressed in BYTES */
    UInt32  tbSize;
    UInt32  retransmissionTBSize;
    UInt32  tbStartOffset;
    UInt32  paddingBytes;
    UInt32  dataStartIndex;
    TransmissionAndRLCData* transmissionAndRLCData_p;
    ARQInfo arqInfo[MAX_NUMBER_OF_LOGICAL_CHANNEL];
    /* This field is toggled for new transmission, retained same value for retransmission */
    UInt8   newDataIndicator; /* _NDI_SPR_ */
    MsgType macScellCEFlag;
}DLHARQTBInfo;

typedef struct DLHARQProcessT
{
    /* This field will contain the HARQ ID assigned */
    UInt8    harqProcessId;
    /*used to signify that particular HARQ process is available or not*/
    UInt8 harqStatus;
    /* In case when the TB one is valid the below field is set to VALID */
    UInt8 isTBOneValid;
    /* In case when the TB one is valid the below field is set to VALID */
    UInt8 isTBTwoValid;
    /* Indicates the sub frame number when the HARQ was sent to the PHY */
    UInt8 subFrameNum;
    /* indicate sub band, wide band or no frequency selective type of transmission*/
    UInt8 transmissionType;
    UInt8    assignedResourcesNB;
    UInt8    *transmissionBuffer_p;
    /* SPR 3679 Changes Start */
    UInt8    *transmissionBufferTB2_p;
    /* SPR 3679 Changes End */
    /* + CQI_5.0 */
    UInt8 pmiIndex;
    /* - CQI_5.0 */
    /* SPS_CHG */
    UInt8 isSpsOnlyPaddingPresent;
    /* SPS_CHG */
    /*4x4 DL MIMO CHG START*/
    UInt8 numOfLayer;/*Max num of layer can be supported as per cqi threshold*/
    /*4x4 DL MIMO CHG END*/
    UInt8  scellActBitmask;      
    UInt8  scellDeactBitmask;
#ifdef TDD_CONFIG
	UInt8 subFrameType;
	/* + SPS_TDD_Changes */
	UInt8 schUsed;
	/* - SPS_TDD_Changes */
#endif  
#if defined (KPI_STATS) || defined (PERF_STATS)
    /* SPR 16916 FIX START */
    /* +- */
    /* SPR 16916 FIX END */
 /* 32.425 Events code changes start*/
    /* SPR 16613 fix start */
    UInt8             commonIndex[MAX_LOGICAL_CHANNEL - 3];
    /* SPR 16613 fix end */
    UInt32            pdcpBytes[MAX_LOGICAL_CHANNEL - 3];
 /* 32.425 Events code changes end*/
#endif
    /* The following field will contain the TICK for which this HARQ process should expire*/
    /* SPR 15909 fix start */
    tickType_t dlharqRttExpiryTick;
    /* SPR 15909 fix end */
    UInt32   totalTBSize;
    /*RETX_CHG*/
    /* SPR 15909 fix start */
    tickType_t    drxRetxExpiryTTI;
    /* SPR 15909 fix end */
    /*RETX_CHG*/
    RATPolicy ratPolicyUsed;
    DLHARQTBInfo       dlHARQTBOneInfo;
    DLHARQTBInfo       dlHARQTBTwoInfo;
    /* RETX_CHG */
    DCIFormat    dciFormat;
    /* RETX_CHG */
#if defined(PERF_STATS) 
    /* Total REs used after subtracting REs which Overlap with MIB, PSS and SSS*/
    UInt32 totalREsUsed;
#endif
} DLHARQProcess;


typedef struct DLHARQEntityT
{
    UInt8        numHARQProcess;
    /* This field will contain the index of the timer node */
    UInt8        dlHarqTimerNodeArrrayIndex[MAX_SUB_FRAME];
    DLHARQProcess  harqProcess[MAX_HARQ_PROCESS_NB];
    /* This array is a direct indexing array for HARQ
       process and expresses the Harq Expiry tick */
}DLHARQEntity;


/* This structure is used in priortised LC list node, 
 * for storing the weight of the logical channel. Also 
 * to store the scheduled bytes of LC. 
 */
typedef struct DLQoSStrategyLCNodeT
{
    /* Anchor to LC node in the RB Tree */
    DlPriorityLcNode  lcTreeAnchor; 
    /* Anchor to LC node in the scheduled LC list */
    LTE_LIST_NODE     scheduledLcAnchor;
    /* Anchor to Next LC node in the eligible LC list */
    LTE_LIST_NODE     eligibleLcAnchor;
    /* Indicates the UE Id */
    UInt16 ueIndex;
    /* Indicates the LC Id */
    UInt8   lcId;
    /* Indicates whether LC in Eligible List or not */
    UInt8  isPresentInEligibleList;
    /* Indicate the total bytes scheduled for LC, 
     * Data bytes + external RLC header 
     */
    UInt16 schedulableBytes;
    UInt16 schedulableBytesPcell;
    /* stores the weight of the LC, this will be used as key for RB-Tree */
    UInt32 totLCWeight; 
    /* Back reference for LC info */
    struct DLLogicalChannelInfoT *lcInfo_p;
    /* Indicates the LC node is been scheduled or not
     * Used during the Re-Travesal scheduling
     */
    SInt32 schedulableTokensPcell;
    DLUESCellStartegyNode *dLUESCellStartegyNode_p[MAX_NUM_SCELL + MAX_NUM_PCELL];

    /* + SPR 12349 Changes */
    /* Indicates total bytesAllocated for an LC across all Cells */
    UInt32 requestedDataSize;
    /* - SPR 12349 Changes */
    /* SPR 16116 fix start */
    UInt32 totLCWeightWithLP;
    /* SPR 16116 fix end */
}DLQosStrategyLCNode;


/* DL qos strategy start */
typedef struct DlLcQosInfoT
{
    /* Indicates the QCI of the Logical Channel */
    UInt8 QCI;
    /* Indicates the Type of the channel i.e. GBR or NON-GBR */
    UInt8 channelType;
    /* Indicates QCI priority*/
    UInt8 priority;
    /* RLC Fixed header Size */
    UInt8 rlcFixedHeaderSize;
    /* Indicates the Latency of the Logical channel */
    UInt16 pktDlyBdgt;
    /* Indicates the size of the Packets */
    UInt16 avgPktSize;
    /* Indicates the Gauranteed bit Rate */
    UInt32 GBR;
    /* Indicates the Maximum Bit Rate */
    UInt32 MBR;
    /* Indicates Packet Error Rate */
    UInt32 pktErrRate;
    /* Indicates the RLC AM status */
    RlcAmStatus eRlcAmStatus;
    /* Indicates teh control Queue Load */
    UInt32 ctrlQload;
    /* Indicates the Retransmission Queue Load */
    UInt32 reTxnQueueLoad;
    /* Indicates the first retansmission Size */
    UInt32 retxnPduSize;
    /* Indicates the TokenRate */
    UInt32 tokenRate;
    /* Indicates the Tokens Available in bytes */
    SInt32 availToken;
    /* DESIGN REVIEW CHANGES INCORPORATION  START */
    /* Indicates the token depth */
    TokenUpdateFlag  eTokenUpdateFlag;
    /* Indicates the token depth */
    /* SPR 11115 Fix Start */
    SInt32  tokenLimit;
    /* SPR 11115 Fix End */
    /* DESIGN REVIEW CHANGES INCORPORATION  END */
    /* Indicates the token depth */
    UInt32  tokenDepth;
    /* Indicates PDB priority*/
    UInt8  pdbPriority;
    /* Indicates QCI priority*/
    UInt8  qciPriority;
    /* Indicates Qload priority*/
    UInt8 qLoadPriority;
    /* Indicates Token priority*/
    UInt8 tokenPriority;
    /* Indicates the Total tokens scheduled in the scheduled window */
    UInt32  totSchdTknInWindow;
    /* Indicates the current index in the scheduled window array */
    UInt8  curTknWindowInd; 
    /*Indicates whether strict scheduling will be applied or not*/
    UInt8 strictSchedulingFlag;
    /* word  Padding */
    UInt8 PAD;

    /* Stores the tokens scheduled in every TOKEN_UPDATE_PERIOD in 
     * last MAX_TKN_SCHD_WINDOW periods
     */
    UInt32 scheduledToken[MAX_TKN_SCHD_WINDOW];
    /* Indicates the map between the qload and index to priority */
    UInt32  lcQlTable   [MAX_QLOAD_PRIORITIES];
    /* Indicates the map between the PDB and index to priority */
    UInt16  lcPdbTable   [MAX_PDB_PRIORITIES];
    /* Indicates teh SRB weight, if LC is SRB value will be INITNITE_PRIORITY
     * else it will be Zero
     */
    UInt16 srbWeight;
    /* Indicates the map between the token value and index to priority */
    UInt32  lcTknTable      [MAX_TOKEN_PRIORITIES];
    /* pointer to Strategy LC node */
    DLQosStrategyLCNode  *dlQosStrategyLCNode_p;
    /* CA changes Start */
    UInt8 bandWidthDistribution[MAX_NUM_SCELL + MAX_NUM_PCELL];
    UInt32 qosPriority [MAX_NUM_SCELL + MAX_NUM_PCELL];
    /* CA changes End */
}DlLcQosInfo;
/* DL qos strategy stop */



typedef struct DLLogicalChannelInfoT
{
    /*indicates the data available for this logical channel*/
    UInt32 queueLoad;
    /*contain the information regarding when this logical channel was scheduled*/
   /* SPR 15909 fix start */
    tickType_t lastScheduledTick;
   /* SPR 15909 fix end */
    /* 0-TM,1 -UM, 2-AM */
    UInt8 rlcMode;
    /*1,2,3 Conversational -1, Interactive . 2, Backgorund - 3*/
    UInt8 rlcSNFieldLength;
    /*Indicates the id of this logical channel if valid*/
    UInt8  logicalChannelId;
    /*Indicates the priority of this logical channel*/
    UInt8  logicalChannelPriority;
    /* DL logical channel Qos Info */
    DlLcQosInfo    dlLcQosInfo;
    /* flag to check whehter LcNode is already present in the tree */
    UInt8          isLCNodePresent;
    /* Flag to check whether the SPS is configured for this LC or not*/
    UInt8 isSpsLc ;
    /* SPS_CHG */
    /* SRP 3608 changes start */
    UInt8 enbPSRReqFlag;
    /* SPR 3608 changes end */
    /* SPS_CHG */
    /* Indicates Packet Arrival Time Stamp */
    /* SPR 15909 fix start */
    tickType_t pktArrivalTS;
    /* SPR 15909 fix end */
    /* ROHC Profile of the sps LC */
    UInt16 rohcProfile;
}DLLogicalChannelInfo;


typedef struct DLSchedulerAlgoInfoT
{
    /* for optimising on modscheme factor calc*/
    UInt32              uePriorityFactor;
    /* for optimising on modscheme factor calc*/
    UInt32              modulationSchemeFactor;
    /* Weighted Queue Load of the UE*/
    UInt32              weightedQueueLoad;
    /* number of logical channel ID with the non Zero Queue Load of the UE*/
    UInt8              numLCWithNonZeroQueueLoad;
    /* Indicates Sum of Logical Channel priority of UE*/
    UInt32              sumLCPriority;
    /* Weighted Priority of the UE based on scheduling algorithm*/
    UInt32              weightedPriority;
    /*  This parameter indicates the calculation tick of weighted Priority*/
    /* SPR 15909 fix start */
    tickType_t          lastCompWTPriorityTick;
    /* SPR 15909 fix end */
}DLSchedulerAlgoInfo;

typedef struct DLMIMOInfoT
{
    /* + CL-MIMO LJA*/
    /*Represents the codebookSubsetRestrictionConfigured by the upper layers*/
    codebookSubsetRestrictionInfo cbsrInfo;
    /* - CL-MIMO LJA*/
    /* Its represent the transmission Mode. */
    /* coverity-530 CID-32321*/
    TransmissonMode     transmissionMode;
    /* coverity-530 CID-32321*/
    /*Its represent the total number of layer  */
    UInt8               precodingIndex;
    /* + CQI_5.0 */
    UInt8               dlRi;
    /* - CQI_5.0 */
    /* + TM6_5.2 */
    /* This flag is set true when Aperiodic 12 Mode is configured and used while filling TPMI */
    UInt8               isAperMode12;
    /* - TM6_5.2 */
    /* CA Changes start */
    UInt8               numOfLayerMIMO;
    /* CA Changes End */
    /* +EICIC */
    UInt8               dlRi_2;
    /* -EICIC */ 
}DLMIMOInfo;

typedef struct DLCQIInfoT
{
    UInt8               cqiMode;
    /* Its indicate the cqiIndicator Periodic or Aperiodic  */
    UInt8               cqiTypeIndicator;
    /* indicates that request for aperiodic CQI is made */
    UInt8               aperiodicCQIRequested;
    /* This flag represent the sub band report is available or not. */
    UInt8               isSubBandReportAvailable;
    /* This flag represent the wideband report is available or not  */
    UInt8               isWideBandReportAvailable;
    UInt8               modulationSchemeCodeWordOne;
    UInt8               modulationSchemeCodeWordTwo;

    /* Wideband CQI values for Codewords*/
    UInt8               widebandCQICodeWordOne;
    UInt8               widebandCQICodeWordTwo;
    
    UInt8               mcsIndexCodeWordOne;
    UInt8               mcsIndexCodeWordTwo;
    UInt8               wideBandPMIIndex;
    SInt8 currentDLBLERForCW0;
    SInt8 currentDLBLERForCW1;
    UInt8 dlBlerCalculatedCounterForCW0;
    UInt8 dlBlerCalculatedCounterForCW1;
    /*EICIC*/	
    SInt32 previousDLBLERForCW0_2;
    SInt32 previousDLBLERForCW1_2;
    UInt8 dlBlerCalculatedCounterForCW0_2;
    UInt8 dlBlerCalculatedCounterForCW1_2;
    /*EICIC*/	
    UInt8 latestAperiodicReportRcvd;
    UInt8 initialCqiValue;

    /* Its value can be either LATER_DL_RI_UPDATE when RI toggled from 1 to 2 or  
     * IMMEDIATE_DL_RI_UPDATE when RI toggled from 2 to 1 or
     * not toggled.
     */
    UInt8 riUpdationFlag;
    /* - CQI_5.0 */
    /* + SPR 5953 Fix */
    UInt8 cqiRequestType;
    /* - SPR 5953 Fix */
	/* SPR 4769 changes start */
    UInt8 previousTimeAveragedWBCQICW0;
    UInt8 previousTimeAveragedWBCQICW1;
	/* SPR 4769 changes endt */
    /* SPR 13571 fix start */
    UInt8 prevDlBlerCalculatedForCW0;
    UInt8 prevDlBlerCalculatedForCW1;
    UInt8 prevDlBlerCalculatedForCW0_2;
    UInt8 prevDlBlerCalculatedForCW1_2;

    /* SPR 13571 fix end */
    /* CA changes start */
    UInt8     trigger1r10; 
    UInt8     trigger2r10;
    /* This field is pointer of type SubBandReportInfo
       structure which stores subband report */
    SubBandReportInfo   *subBandReportInfo_p;
    /* + CQI_4.1 */
    /* SPR 15909 fix start */
    tickType_t aperiodicCqiRequestTTI;
    /* SPR 15909 fix end */
    SInt32 previousDLBLERForCW0;
    SInt32 previousDLBLERForCW1;
    /* - CQI_4.1 */
    /* + CQI_5.0 */
    SInt16 cqiCorrectionFactorForCW0;
    SInt16 cqiCorrectionFactorForCW1;
    /* SPR 15909 fix start */
    tickType_t reportReceivedTTI;
    /*when MAC Scell Activation CE Sent then this will be checked against currentTTI. 
    If currentTTI > isCQIReportValidTTI, then all cqiMap of SCell will be flushed*/
    tickType_t    isCQIReportValidTTI; 
    /* SPR 15909 fix end */
    /*+EICIC */
    UInt8 riUpdationFlag_2;
    /* SPR 21996 Fix Start */
    /* It stores the RI value reported by UE to update until next CQI report on Later updation
     *  when present RI is 1 and received one is greater than 1*/
    UInt8 dlRiToUpdate;
    /* SPR 21996 Fix End */
    SInt16 cqiCorrectionFactorForCW0_2;
    SInt16 cqiCorrectionFactorForCW1_2;
    UInt8 previousTimeAveragedWBCQICW0_2;
    UInt8 previousTimeAveragedWBCQICW1_2;
    UInt8 widebandCQICodeWordOne_2;
    UInt8  widebandCQICodeWordTwo_2;
    UInt8  mcsIndexCodeWordOne_2;
    UInt8  mcsIndexCodeWordTwo_2;
    UInt8  modulationSchemeCodeWordOne_2;
    UInt8  modulationSchemeCodeWordTwo_2;
    UInt8  wideBandPMIIndex_2;
    /* This flag represent the sub band report is available or not. */
    UInt8  isSubBandReportAvailable_2;
    /* This flag represent the wideband report is available or not  */
    UInt8  isWideBandReportAvailable_2;
    /* SPR 15909 fix start */
    tickType_t reportReceivedTTI_2;
    /* SPR 15909 fix end */
    UInt8 latestAperiodicReportRcvd_2;
    SInt8 currentDLBLERForCW0_2;
    SInt8 currentDLBLERForCW1_2;
    /*-EICIC */
    /* CA changes End */
}DLCQIInfo;


#ifdef TDD_CONFIG
typedef struct TDDHarqTimerExpiryQueueNodeT
{
    /*Node Anchor of the Node*/
    LTE_LIST_NODE   nodeAnchor;
    /*UE Identification in MAC*/
    UInt16          ueIndex;
	/* + SPS_TDD_Changes */
	UInt32 schUsed;
	/* - SPS_TDD_Changes */
    /* SPR 11230 fix start */
    UInt8           isErrForNode;
    /* SPR 11230 fix end */
    /* SPR 16429 fix start */
    UInt8 padding; 
    /* SPR 16429 fix end */
}TDDHarqTimerExpiryListNode;

#endif

typedef struct DRXTimerNodeT
{
    LTE_LIST_NODE           anchorNode;
    /* SPR 15909 fix start */
    tickType_t              expiryTTI;
    /* SPR 15909 fix end */
    struct DLUEContextT     *ueContext_p;
/*START : DRX_MAC_CE*/
    DRXTimerNodeType        drxTimerNodeType;
/*END   : DRX_MAC_CE*/
}DRXTimerNode;



/* This strucure contains DRX variables required to maintain DRX state 
 * machine */
typedef struct DRXStateVariablesT
{
    UInt8          drxNumShortCyclesElapsed; 
/*START : DRX_MAC_CE*/
    UInt8          isDrxMacCeTimerRunning;
    UInt16         drxCycleTimerExpiryIndex;
/*START_DRX_OPT*/   
    /* SPR 15909 fix start */
    tickType_t      drxStartTTIOfMacCeTimer;/*The TTI on which the MacCeTimer started*/
/*END_DRX_OPT*/   
/*END   : DRX_MAC_CE*/
    tickType_t      drxShortCycleNextOnDurationTTI;
    tickType_t      drxLongCycleNextOnDurationTTI;
    tickType_t      drxLastOnDurationTTI;/*The TTI on which the Onduration was last triggered*/
    /* SPR 15909 fix end */
    DRXTimerNode   *drxTimerNode_p; 
/*START : DRX_MAC_CE*/
    DRXTimerNode   *drxMacCeTimerNode_p; 
/*END   : DRX_MAC_CE*/
    DRXState       drxCurrentState;
    DRXCycle       drxNextCycle;
    DRXSubState    drxSubState;
/*START : DRX_MAC_CE*/
    UInt16         drxMacCeTimerExpiryIndex;
    UInt8          isMacCeToBeScheduled;
/*END   : DRX_MAC_CE*/
    /* SPR 22594 fix start */
    UInt8 isInactivityExpiredAfterOnDuration;
    /* SPR 22594 fix end */
}DRXStateVariables;

/* This structure contains parameters that are required to maintain DRX specific
 * statistics */
typedef struct DRXStatsT
{
    UInt8     numDRXLongCycles;
    UInt32    totalActiveTime;
    UInt32    totallnactiveTime;
}DRXStats;

/* This strucure contains DRX related parameters */
typedef struct DRXContextT
{
    DRXStateVariables    drxStateVariables;
    DRXConfig            drxConfig;
    DRXStats             drxStats;
}DRXContext;
/* DRX_CHG */

/* CLPC_CHG */
typedef struct TpcInfoForPucchT
{
    /* Indicate the index calculated from tpc rnti configured as 
       tpcRnti - startTpcPucchRnti */
    UInt8 tpcRntiIndex;

    /* Indicate the Offset in the bitmap for DCI 3/3A */
    UInt8 tpcBitMapIndex;
    
    /* Indicate the configured TPC-RNTI for PUCCH that can be used to send the
     * TPC using DCI Format 3/3A to the UE. It shall be set INVALID_TPC_RNTI, 
     * is no TPC RNTI is configured for the UE for PUCCH power control */
    UInt16 tpcRnti;

    /* Indicate the last Tick at which any TPC was applied by the UE for PUCCH.
     * If no TPC is sent to UE before, then this shall be zero */
    /* SPR 15909 fix start */
    tickType_t lastAppliedTick;
    /* SPR 15909 fix end */
    
    /* Indicate the last TPC was applied by the UE for PUCCH.
     * If no TPC is sent to UE before, then this shall be invalid */
    SInt8 lastAppliedTpc;

    /* Indicate the triggered wideband power control command that can be sent 
     * to UE. */
    SInt8 tpcTriggeredValue;

    /* Indicate whether the last TPC Value sent for this UE was fake Value  
     * if yes then the tpc value would be either 1(if fakeTpcValue was 0)
     * or -1 (if fakeTpcValue was -1) this is relevant only for DCI3A */
    SInt8 fakeTpcValue;

    /* The pointer to UE node added in triggeredUeList in context corresponding
     * to TPC.RNTI configured for PUCCH for the UE. This shall be NULL if the 
     * TPC is not triggered for the UE using DCI format 3/3A. */
    TriggeredUEListNode* triggeredUeListNodeInTpcRntiContext_p;

    /* The pointer to UE node added in configuredUeList in context corresponding
     * to TPC.RNTI configured for PUCCH for the UE. This shall be NULL if the 
     * TPC is not configured for the UE using DCI format 3A. */
    ConfiguredUEListNode* configuredUeListNodeInTpcRntiContext_p;

}TpcInfoForPucch;

/* This structure shall contain the fields useful for calculating PUCCH BLER. */
typedef struct BlerInfoForPUCCHT
{
    /* Total BLER on PUCCH. */
    /* SPR 3530 START */
    SInt8 pucchBler;
    /* SPR 3530 END */
    
    /* Counter for the number of samples for which BLER has been calculated*/
    UInt16 blerCalculatedCounter;

    /* Counter to maintain when last BLER was calculated */
    UInt32 lastBlerCoolOffCounter;
    /* Bler changes start */
    /* Bler calculated for the number of samples.
     * this will be then overwritten to pucchBler;
     */
    SInt32 blerCalculated;
    /* Bler changes end */

}BlerInfoForPUCCH;
/* CLPC_CHG END */

/* Following structure for PDSCH res reservation successful */
typedef struct SpsResourceAllocatorInfoT
{
    /* This stores the resource assignment information of allocated RBs */
    UInt32 resourceAssignmentBitMap; 
    /* This stores the number of allocateRB */
    UInt8 allocatedRB;
    UInt8 startingVRBIndex;
    UInt8 allocatedMCS;
} *LP_SpsResourceAllocatorInfo, SpsResourceAllocatorInfo;

/* Following structure contains the SPS information for DL for an UE */
typedef struct SpsDlInfoT
{
    /* SPS LC ID */
    UInt8  lcId;
    /* 0 - RELEASE, 1 - SETUP */
    UInt8  requestType;
    /* SPR 20456 start */
    /* code removed */
    /* SPR 20456 stop */
    /* Flag to indicate if UE WB MCS is below SPS MCS Threshold*/
    UInt8 isUeDlWbMcsBelowThresh;
    /* Number of Empty transmissions in SPS occasion */
    UInt8  numOfSpsEmptyTx;
    /* HARQ process Id used at the time of occasion or SDL processing */
    UInt8  hpId;
    /* Maximum number of packets that can be received before activation due to
     * SPS-PDSCH failure */
    UInt8  maxPktsAfterDeactivatedForPdschFailure;
    /* SPR 19038 Fix Start */
    /* Flag to indicate if ue's meas gap is colliding with sps occasion
     * in case meas gap has been configured after sps activation
     */
    UInt8 isUeMeasGapCollisionWithSpsOcc;
    /* SPR 19038 Fix End */
    /* This stores UE's DRX OFF index.To be used by RM */
    UInt8 ueDrxTTI[MAX_SPS_INTERVAL];
    /* This stores UE's MEAS GAP OFF index. To be used by RM */
    UInt8 ueMeasGapTTI[MAX_SPS_INTERVAL];
    /* SpsCrnti to be allocated when spsDlSetupInfo_p is recived 0 is invalid 
     * value */
    UInt16 SpsCrnti;
    /* Index used for Activation/Reactivation */
    UInt16 ttiIndex;
    /* Index used for Deactivation */
    UInt16 deactTtiIndex;
    /* Number of DCI attempts made for Activation/Deactivation/Reactivation */
    UInt16 numDciAttempts;
    /* Pointer to the node of UE either residing in SDL or occasion list */
    void   *sdlOccNode_p;
    /* Pointer to the second node of UE either residing in SDL or occasion list
     * for 20 ms periodicity */
    void   *sdlOccNode20_p;
    /* Contains the SPS State of the UE */
    /* + Coverity 32317 */
    SpsStates  spsState;
    /* - Coverity 32317 */
    /* Head packet size in RLC SPS LC queue (size of PDCP PDU) */
    UInt32 headPktSize;
    /* Latest Opportunity provided to SPS LC */
    UInt32 spsOpp;
    /* Average RLC PDU size */
    UInt32 avgRlcSduSize;
    /* Packet arrival TS */
    /* SPR 15909 fix start */
    tickType_t pktArrivalTs;
    /* SPR 15909 fix end */
    /* Present if request is Setup */
    LP_SpsDlSetupInfo spsDlSetupInfo_p;
    /* Structure for RBs info UE */
    SpsResourceAllocatorInfo resAllocInfo;
} *LP_SpsDlInfo, SpsDlInfo;

/* CA Changes Start */
typedef DLHARQEntity SCELLDLHARQEntity;
typedef DLMIMOInfo SCELLDLMIMOInfo;
typedef DLCQIInfo SCELLDLCQIInfo;
typedef DLHARQProcess SCELLDLHARQProcess;
typedef DCIFormat SCELLDCIFormat;
/** SPR 12457 Fix : Start**/
typedef struct SCELLPUCCHREQINFOT{
    /** Possible Values : 0x0, 0x01,0x02,0xFF **/
    UInt8 schedulingStatus;
    UInt8 tpc;
    /** SPR 14277 Changes Start **/
#ifdef TDD_CONFIG
    UInt8 sCellSubFrame;
    UInt8 sCellSchInfo[MAX_SUB_FRAME];
#endif
    /** SPR 14277 Changes End **/
}SCELLPUCCHREQINFO;
/** SPR 12457 Fix : End **/

/* Stage-III SCELL Deactivation Timer Related Changes Start */
typedef struct SCellDeactTimerNodeT
{
    LTE_LIST_NODE           anchorNode;
    /* SPR 15909 fix start */
    tickType_t              expiryTTI;
    /* SPR 15909 fix end */
    /*Index in DLUESCellContext array where scell Context is actually present in the UEContext*/
    ServingCellIndex        sCellArrayIndex; 
    UInt16                  ueIndex;
}ScellDeactTimerNode;
typedef struct ScellStateVariablesT
{
    ScellDeactTimerNode   *scellTimerNode_p; 
    UInt16                scellCycleTimerExpiryIndex;
    /*Shall store the count of instances when QLoad reaches the threshold*/
    UInt8                 scellActThresCount;
    UInt8                 scellDeactThresCount;
}ScellStateVariables;

typedef struct scellDeactExpiryTimerListT
{
    LTE_LIST expiryTimerList;
}scellDeactExpiryTimerList;

/*size of deactivation expiry timer list array (scellDeactExpiryTimerList_gp)*/
#define MAX_SCELL_DEACT_TIMER               16
/*Value of Hash Key used to insert timer node in 
deactivation expiry timer list array (scellDeactExpiryTimerList_gp)*/
#define MAX_SCELL_DEACT_TIMER_MOD_VALUE     MAX_SCELL_DEACT_TIMER - 1
/*Invalid Scell expiry Index in the deactivation expiry timer list array (scellDeactExpiryTimerList_gp)*/
#define INVALID_SCELL_DEACT_EXPIRY_INDEX    0xFF
/*Invalid Serving Cell Index*/
#define INVALID_SERV_CELL_INDEX             INVALID_8_VAL /* Value is 0xFF*/
/*8 is the T+8 value which is used to change from one scell state to another*/
#define SCELL_STATE_CHANGE_TIMER_OFFSET     8  
/*ActDeactBitMask */
#define SCELL_ACT_BIT_SET                   1
#define SCELL_DEACT_BIT_SET                 2
#define SCELL_ACT_DEACT_BIT_RESET           0
#define SCELL_SHIFT_GET_SCELL_ID            1
/*Shall free the memory for the scell deactivation timernode*/
#define FREE_MEM_SCELL_DEACT_TIMER_NODE(node_p) \
{ \
    if(node_p)\
    freeMemPool(node_p);\
    node_p=PNULL;\
}

/************************************************************************
 * Macro Name     :  RESET_SCELL_UE_QLOAD_CHECKLIST 
 * Description    :  This function inits the DL ca scell qload check list 
 *                   by initializing the values.
 *Macro shall reset the parameters of the structure at the end of tick
 ************************************************************************/
#define RESET_SCELL_UE_QLOAD_CHECKLIST(internalCellIndex) \
{\
      memSet((void *)&dlCAScellUeQloadCheckList_g[START_SCELL_INDEX],0,sizeof(DLCAScellUeQloadCheckList)); \
      memSet((void *)&dlCAScellUeQloadCheckList_g[internalCellIndex],0,sizeof(DLCAScellUeQloadCheckList));\
}


/* Stage-III SCELL Deactivation Timer Related Changes End */

typedef struct DLCATimerNodeT
{
    /*Is the node valid */
    UInt8   isValid;
    /*scell id for which timer node is maintained*/
    InternalCellIndex   scellId; 
    UInt16  ueIndex;
    /*current state of scell*/
    DLCAScellStateFlag dlCAScellState;
    /* SPR 15909 fix start */
    tickType_t dlCAExpiryTick;
    /* SPR 15909 fix end */
}DLCATimerNode;

/*This structure contains Scell specific information 
*required for downlink processing*/
typedef struct DLUESCellContextT
{
    InternalCellIndex internalCellIndex;
    UInt8 isScellActivateStatus;
    UInt8 isPmiRiConfigured_V920;
    UInt8 userLocationType;
    UInt8 pA;
    UInt8 numOfLayer;
    UInt8 isHDFddFlag;
    UInt8 isSubbandPMIAvailable;
    UInt8 aperiodicMode12Rcvd;
    /*bit mask to buffer the activation/deactivation request in ongoing states*/
    UInt8 ActDeactBitMask;
    /* + 11491 SPR Changes */
    UInt8 isPdcchAllocatedFlag;
    /* - 11491 SPR Changes */
    /* SPR 16422 START */
    /* Store CQI validity timer for cell which is to be added as an Scell */
    UInt8 cqiValidityTimer;
    /* SPR 16422 END */

    CodebookSubsetRestrictionv1020 codebookSubsetRestrictionv1020;
    /** SPR 12457 Fix : Start**/
    SCELLPUCCHREQINFO pucchReqInfo;
    /** SPR 12457 Fix : End **/
    UInt16 maxTBSize;
    UInt32 maxRestrictedTB;
    UInt32 permissibleTBSize;
    UInt32 maxPermissibleTBSize;
    struct  DLStrategyTxNodeT * dlStrategyTxNode_p;
    SCELLDLHARQEntity           *ScelldlHarqContext_p;
    SCELLDLMIMOInfo             ScelldlMIMOInfo;
    SCELLDLCQIInfo              ScelldlCQIInfo;
    SCELLDCIFormat              Scelltm6DCIFormat;
    DLCATimerNode  *dlCATimerNode_p;
    DLCAScellStateFlag scellState;
    DLScheduleFlag scheduleFlag;
    UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p;
    /* Stage-III SCELL Deactivation Timer Related Changes Start */
    ScellStateVariables scellStateVariables;
    /* Stage-III SCELL Deactivation Timer Related Changes End */
#ifdef TDD_CONFIG
    /* This container is used to check scheduling of DL subframes group
     */
    UInt16  pdschTxMapContainerForSCell[MAX_DL_TIMER_CONTAINER][MAX_SUB_FRAME];
    /* Storing the Harq Process Id correspondence the DL subframe
     * */  
    UInt8 harqProcessSCellId[MAX_DL_TIMER_CONTAINER][MAX_SUB_FRAME];

    UInt8 tddDaiOfSCell[MAX_SUB_FRAME];
    UInt32 tddDaiLastScheduledOfSCell[MAX_SUB_FRAME];
    
#endif    
}DLUESCellContext;
/*CA changes end*/
/* + CA_TDD_HARQ_CHANGES */
/* CA_phase2_harq_code Start */
typedef struct SecCellRetxInfoT
{
    UInt8 servCellIndex;
    UInt8 harqProcessId;
    UInt8 schUsed;
}SecCellRetxInfo; 


/* CA_phase2_harq_code End */
/*RETX_CHG*/
typedef struct DLRetxQueueNodeT
{
    LTE_SNODE anchorNode;

    UInt16 ueIndex;

    UInt8 harqProcessId;
    UInt8 isNodeInvalid;
    /* SPS_CHG */
    UInt8 schUsed;
    /* SPS_CHG */

    /* CA_harq_code start */
    UInt8 sCellCount;
    /* CA_harq_code end */

    /* CA_harq_code start */
    SecCellRetxInfo sCellRetxInfo[MAX_NUM_SCELL];
    /* CA_harq_code end */

}DLRetxQueueNode;


/*RETX_CHG*/
/* - CA_TDD_HARQ_CHANGES */
typedef struct DLUEContextT
{
    /*indicates the C-RNTI assigned to this UE*/
    UInt16              crnti;
    UInt16              ueIndex;
    /* Flag is set when DLUEContext */
    /* created during connection Hand Over  */
    UInt16              rrcUeIndex;

    UInt8               tcrntiFlag;
    /*indicates the priority of this UE*/
    UInt8               uePriority;

    /* ICIC changes start */
    /* Stores user location type 
     * Value 0 -> Cell Center
     * Value 1 -> Cell Edge */
    /* + coverity 54168 to 54176 */
    UInt8  userLocationType;
    /* - coverity 54168 to 54176 */
    /* isAlreadyInCEPriorityList is used in DL RR to prvent a duplicate entry in queue */
    UInt8 isAlreadyInCEPriorityList;
    /* ICIC changes end */

    /* SPR 903 Start */
    /*indicates the category of this UE*/
    UInt8               ueCategory;
    /* SPR 903 End */
	
	/* ULSCH_HARQ_issue_fix + */
#ifdef FLEXRAN
	//ysl bugfix 0716
	//in TDD, nBundle is updated with DAI, but when a HARQ.indication is received, mac needs to find
	//  nBundle for DL subframes corresponding to this UL subframe, not to use the newest DAI
	//for example, in (2,1) MAC receive HARQ.indication for (1,7), the DL subframes are (0,9)(1,0)(1,1)(1,3),
	//but the newest DAI is already updated for latest DL subframes
    UInt8  nBundle[2/*for target ul sfn ping-pong*/][MAX_SUBFRAME/*UL subframe*/];
#endif
	/* ULSCH_HARQ_issue_fix - */
    /*Added for Power Control*/
    /*CLPC_REWORK_CHG*/
    UInt8 pA;
    /*CLPC_REWORK_CHG*/
    UInt8               assignedRBQueueLoadSchIndex;
    UInt8               assignedRBQueueLoadRLCIndex;
    /*indicates the composite queue load of all logical channels of the UE*/
    UInt32              ueQueueLoad;
    /*A pointer to nodeAnchor of the UE in ueNonZeroQueueLoadList_g*/
    LTE_LIST_NODE *     dlUENonZeroQueueLoadAndHarqList_p ;
    /* An array of LogicalChannelInfo where each enty corresponds to details
     * regarding the UEs logical channel*/
    DLLogicalChannelInfo  logicalChannel[MAX_NUMBER_OF_LOGICAL_CHANNEL];
    /*This structure contains info when contention free RACH is triggered due to
     * UL Sync Loss or Handover*/
    UlSyncLossOrHOInfoInDL      ulSyncLossOrHOInfoInDL;
    /*indicates the current state of UE*/
    /*  */
    UInt32              assignedRBQueueLoad[MAX_TEMP_QUEUELOAD_ARR_SIZE];
    DLSchedulerAlgoInfo  dlSchedulerAlgoInfo;
    /*Its indicate the reserved Rbs for Ue. */
    UInt32               isAlreadyConsideredFlag ; // rbsReservedForUE;

    /* +- SPR 16738 */
    /* Number of Logical channels with non zero queueload */
    UInt8              numLCWithNonZeroQueueLoad;
    /*indicates the maximum LCID available for this UE*/
    UInt8               maxLcId;
    /*signifies that this max number of resources can be used in one TTI for this UE*/
    UInt8               maxNbOfRBs;

    /*global tick count indicating when this UE was scheduled*/
    /* SPR 15909 fix start */
    tickType_t              lastScheduledTick;
    /*START:6172*/
    tickType_t              advlastScheduledTick;
    /* SPR 15909 fix end */
    /*END:6172*/
    /*Its represent the Timing Advance Value. */
    UInt16              timingAdvanceValue;
    UInt16              timingAdvanceAbsValue;

    /* CLPC_CHG */
    /* Indicate the average value of SINR received in CQI_INDICATION */
    UInt8 averageSinrForPucch;

    /* Incremented every time a a TPC trigger is received(i.e. for SINR) 
     * It is checked against the coolOff period of SINR 
     */  
    UInt8 lastSINRCoolOffCounter;
    /* CLPC_CHG END */

    /*It represent the DRX Flag (1-  ON or 0-OFF). */
    UInt8               drxFlag;

    /* Its indicate pdcch is allocated or not. */
    UInt8               isPdcchAllocatedFlag;
    /* This Indicate is the ACK NACK is pending for TB in which DRX has been
       sent .If true this means we won't sent DRX Again */
    UInt8               pendingDRXAckNack;
    UInt8                  simultaneousAckNackAndCQI;
    /* Indicates the number of LC.s scheduled in current tick */
    UInt8  scheduledLcCount;
    /*SPR_3061_FIX*/
    /*SPR_3061_FIX*/
    /*Tick at which lastTA was sent to UE to be in Sync */
        /* SPR 15909 fix start */
    tickType_t              lastTASentTick;
        /* SPR 15909 fix end */
    /*SPR_3061_FIX*/
    /*SPR_3061_FIX*/
    /*indicates the current bit rate of the UE*/
    UInt32              effectiveBitRate;
    /*indicates in which tick last DL data was sent to UE*/
    /* SPR 15909 fix start */
    tickType_t              lastActivityTick;
    /* SPR 15909 fix end */
    /* Pointes to the DL HARQ context*/
    DLHARQEntity           *dlHarqContext_p;
    DLMIMOInfo             dlMIMOInfo;
    DLCQIInfo              dlCQIInfo;

    /* CLPC_CHG */
    /* Contains the fields useful for calculating PUCCH BLER */
    BlerInfoForPUCCH blerInfoForPUCCH;

    /* Store the TPC information for PUCCH */
    TpcInfoForPucch  tpcInfoForPucch;
    /* CLPC_CHG END */

    /* SPR 2063 start */
    /* SPR 2063 end */

    /*Pointes to the UE Specific Search Space Information*/
    UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p;
    /*Pointes to the UE Specific Search Space Information for SPS-CRNTI*/
    UESpecificSearchSpaceInfo *spsUeSearchSpaceInfo_p;
    /* DL qos strategy start */
    /* Indicates the maximum TB Size that an UE can receive per Tick */
    UInt16 maxTBSize;
    /* Indicates the maximum Data bytes an UE can receive per Tick in a TB */
    UInt16 maxSISOTBSize;
    /* Indicates the maximum Data Bytes an UE can receive per Tick */
    UInt16 maxMIMOTBSize;
    /* Stores the priority of the current MCS */
    UInt16 mcsPriority;
    /* Stores the AMBR related info */
    DLAmbrInfo      dlAmbrInfo;

    /* List to maintain the eligible LC.s */
    LTE_LIST dlEligibleLcList;
    /* List to maintain the scheduled LC.s */
    LTE_LIST dlScheduledLcList;

    /* NON-GBR LC Info */
    UInt8 nonGbrLcCount;
    UInt8 nonGbrLcInfo[MAX_NUMBER_OF_LOGICAL_CHANNEL];
    /* GBR LC Info */
    UInt8 gbrLcCount;
    UInt8 gbrLcInfo[MAX_NUMBER_OF_LOGICAL_CHANNEL];
    /* Indicates the transmission state of the LC for the tick */
    DLScheduleFlag scheduleFlag;
    /* Pointer to the Strategy Tx Node */
    struct DLStrategyTxNodeT * dlStrategyTxNode_p;
    /* DL qos strategy stop */
#ifdef TDD_CONFIG
    /* Storing the Harq Process Id correspondence the DL subframe
     * */  
    UInt8 harqProcessId[MAX_DL_TIMER_CONTAINER][MAX_SUB_FRAME];
    /* This container is used for Ack/Nack Bundling 
     * */
    UInt16 pdschTxMapContainer[MAX_DL_TIMER_CONTAINER][MAX_SUB_FRAME];
    /* This container is used to store the address of the node 
     * */
    TDDHarqTimerExpiryListNode  *tddHarqTimerExpiryNode[MAX_DL_TIMER_CONTAINER][MAX_SUB_FRAME];

    UInt8 tddDai[MAX_SUB_FRAME];
    UInt32 tddDaiLastScheduled[MAX_SUB_FRAME];
	/* + SPS_TDD_Changes */
	UInt8 tddDaiSPS[MAX_SUB_FRAME];
	/* - SPS_TDD_Changes */
    /* Temporary node to fill retx info */
    DLRetxQueueNode  tempRetxNode;
    /* NACK response per cell for Retx node entry */
    UInt8 nackFlag[MAX_NUM_CELL];
    /* Retx queue entry per DL */
    DLRetxQueueNode *retxQueueNode_p[MAX_SUB_FRAME];
#endif
    /* DRX_CHG */
    /* This pointer stores the pointer to active UE node delete tmp because
     *        of DRX/ Meas Gap */
    LTE_LIST_NODE   *activeUENodeTempDeleted_p;
    DRXContext      drxContext;
    /* SPR 15909 fix start */
    tickType_t drxLastSavedTTI;	
    /* SPR 15909 fix end */
    /* DRX_CHG */
#ifdef KPI_STATS
    UInt8 qciInfoFlag[MAX_QCI];
    /* SPR 16916 FIX START */
    /* This flag will be set true for a qci when qload
     * becomes zero, and based on this flag the stat
     * activeUeCountDL will be decreased on receiving 
     * harq Ack or after max re-tx. */
    UInt8 zeroQloadFlag[MAX_QCI][MAX_HARQ_PROCESS_NB];
    /* SPR 16916 FIX END */
#endif
    UInt8           drxConfigType;
    /* CLPC_CHG */
    /* The current power control offset from initial power
     * that is indicated to the UE for PUCCH */
    SInt8 currPowerCorrectionForPucch;
    /* Time Averaging changes start */
    /* PUCCH TPC value maintained for time averaging of tpc value*/
    SInt8 powerControlPucchTpcValue;
    /* Time Averaging changes end */
   /*SPR 15208 Fix*/ 
    /*To guard multiple TA command for TA timer expiry */
    UInt8 TA_ScheduleFlag;
    /*SPR 15208 Fix*/ 
 /* Flag to be used to for scheduling type used currently */
    UInt8       schType;
    /* + CQI_5.0 */ 
    UInt8 isSubbandPMIAvailable;
    UInt8 actualPMIValueForDLSCH;
    UInt8 aperiodicMode12Rcvd;
    /* - CQI_5.0 */ 
    /* SRP 3608 changes start */
    UInt8 prioritySchedulingReqDL;
    UInt8 enbPsrCount;
    UInt8 recvdEnbPsrCount;
    UInt8 isPresentInPriorityQueue;
    /* SPR 3608 changes end */
    /* SPR 3607 Fix Start */
    UInt8 intraEnbHOFlag;
    /* SPR 3607 Fix End */
    /* SPR 4725 Start */
    UInt8 srbQloadAvailbl;
    /* SPR 4725 End */
    /* UE_CAPAB_ADD_PARAM Start */
    UInt8 Rat1Support;
    /* UE_CAPAB_ADD_PARAM End */
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    /*This flag will indicate whether UE is HD FDD or not
     * 1/TRUE - UE is HD FDD
     * 0/FALSE - UE is FD HDD*/
    UInt8 isHDFddFlag;
#endif
    /*HD FDD Changes End*/
    /*START : SPS : DRX_MAC_CE*/
    UInt8 nonGbrSPSLcCount;
    UInt8 gbrSPSLcCount;
    /*END   : SPS : DRX_MAC_CE*/
    UInt8 numOfOverLapRbs;
    /* + TM7_8 Changes Start */
    UInt8 isPmiRiConfigured_V920;
    /*BeamForming algorithm to be used SINR based or AOA based.*/
    UInt8 beamFormingAlgoToBeUsed;
    /*Ratio of PDSCH EPRE to UE specific Reference signal
     *Valid only when Transmission Mode is 7 and modulation scheme is QPSK */
    SInt8 pdschEpreToUeRsRatio;
    /* - TM7_8 Changes End */
    /*+ Dyn UEs To Be SchPerTTI +*/
    UInt8 isUEConsideredForTotalActiveUeDL;
    /*- Dyn UEs To Be SchPerTTI -*/
    /* CA changes start */
    InternalCellIndex internalCellIndex;
    UInt8 ueCategoryV1020; /* UE category info for R10 */
    /*Count of Scells configured for this UE*/
    UInt8 scellCount;
    UInt8 scellDeactivationTimer;
    UInt8 isMACCESendToUE;
    UInt8 numOfScellsSchedulingAllowed;
    UInt8 numOfScellsSchedulingNotAllowed;
    /* SPR 16422 START END */
    UInt8 scheduleScellCE;

    UInt8 highestMcsIndex;
    UInt8 schedulingActivateScell[MAX_NUM_SCELL];
    UInt8 schedulingDeactivateScell[MAX_NUM_SCELL];
    UInt8 bandWidthDistribution[MAX_NUM_SCELL + MAX_NUM_PCELL];
    /* MEAS_GAP_CHG */
    UInt8           measGapConfigType;
    MeasGapContextDL  measGapContext;
    /* MEAS_GAP_CHG */
    /*SPR_3061_FIX*/
    UInt16 tempContextIndex; 
    /* SPR 15909 fix start */
    tickType_t lastSyncDetectedTick;
    /* SPR 15909 fix end */
    UInt16 timeAlignmentTimer;
    /*SPR_3061_FIX*/
    /* + SPR_3939 */
    UInt16 syncTimerDelta;
    /* SPR 15909 fix start */
    tickType_t lastTATriggeredTick;
    /* SPR 15909 fix end */
    /* - SPR_3939 */
    UInt32          maxRestrictedTB;
    /*TA Avging */
    UInt32 lastCalculatedTAAverage; 
    /*TA Avging */
    SpsDlInfo   spsDlInfo; /* Contains SPS related information */

    /*Soft Buffer Changes Start*/
    UInt32 permissibleTBSize;
    UInt32 maxPermissibleTBSize;
    /*Soft Buffer Changes End*/

    /*Dynamic PDCCH + 5117 changes starts */
    UInt32 bothTbsize;
    /*Dynamic PDCCH + 5117 changes ends */
    /* + TM6_5.2 */
    /* + coverity 32774, 41643, 41642*/
    DCIFormat tm6DCIFormat;
    /* - coverity 32774, 41643, 41642 */
    /* - TM6_5.2 */
    /*Angle of arrival value for 4 anteenas*/
    UInt16 AoA[NUM_OF_TX_ANTEENA_VALUE_4];
    /*Secondary Cell contexts for each of the SCELL configured for the UE*/
    DLUESCellContext *dlUeScellContext_p[MAX_NUM_SCELL + MAX_NUM_PCELL];
    PucchFormatR10 pucchFormatR10;
    /*CA changes end*/
   	/*EICIC +*/
	UserType			userType;
	/*EICIC -*/
    /*SPR 21948 Changes Start */
    /* Changes under spr 21762 have been reverted */
    /*SPR 21948 Changes End */
    /* + SPR 22352 fix */
    /* This bitmap will be updated when TA/DRX/SCELL CE needs to be sent */
    /* From LSB: bit 1 -TA, bit 2- DRX, bit 3- SCELL ACT CE,
     * bit 4- SCEL DEACT, bit 5- scellID */
    UInt8    TaDrxScellBitmap;
    /* - SPR 22352 fix */
    /* SPR 23402 Changes Start */
    /* MOCN Changes Start */
    UInt8 operatorId; 
    /*MOCN SPR 21913 Start*/
    UInt64 qciPerTBSize[MAX_QCI];
    UInt64 qciPerTBSizePerOperatorPerQCI[MAX_NUM_MOCN_OPERATOR][MAX_QCI];
    /*MOCN SPR 21913 End*/
    /* MOCN Changes End */
    /* SPR 23402 Changes End */
}DLUEContext;


/* DRX_CHG */
/* This structure contains all the parameters that are stored in the DRX Timer
 * Node*/
typedef struct RetransmitQueueNodeT
{
    /*data type of anchor*/
    LTE_SNODE retransmitAnchor;
    /*identifier to one of the HARQ Processes of the UE*/
    UInt8          harqProcessId;
}RetransmitQueueNode;

typedef struct DLUEContextInfoT
{
    /* This flag is set by RRC interface when UE deletion request comes
       and if PendingDelete is True or 1 then UE Context either
       deleted or is to be deleted.*/
    UInt8              pendingDeleteFlag;
    /* CA changes start */
    UInt8 scellRelFlag;
    /* CA changes end */
    DLUEContext       *dlUEContext_p;
    /* SPR 13492 Changes Start */
#ifdef KPI_STATS
    UInt8 inSessionTimePerQCIFlag[MAX_QCI];
#endif
    /* SPR 13492 Changes End */
}DLUEContextInfo;


/* HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG

/** number of reports CQI, RI, SRS, SR **/
#define HD_UE_NUM_OF_REPORTS    4

/* Periodicity of reports less than 4 is not
 * recommended for HD FDD UE */
#define HD_UE_MIN_REPORT_PERIODICITY   4

/** size of sched map, keeps data for 10 subframes **/
#define HD_UE_SCHED_MAP_SIZE    10

/** to calculte the x-th bit from Byte **/
#define GET_BIT(x, byte) (byte>>(x) & 0x01)

/** bitmask for all UL report bits **/
#define HD_FDD_BITMASK_UL_REPORTS 0x0F

/** the fourth offset, used for UL data after DCI0 or HARQ after DCI1x/2x **/
#define HD_FDD_SCHED_MAP_OFFSET_FOUR 4
/** the eighth offset, used for PHICH HARQ for UL data after DCI0 **/
#define HD_FDD_SCHED_MAP_OFFSET_EIGHT 8

/* Flag used for reconfiguration when RF-params not received */
#define HD_FDD_FLAG_INVALID        0xFF

/* bit implication of schedmap byte for SR */
#define HD_UE_SCHED_UL_BITMAP_SR               0x01

/* bit implication of schedmap byte for HARQ for DL data */
#define HD_UE_SCHED_BITMAP_DL_HARQ             0x10

/* bit implication of schedmap byte for UL data */
#define HD_UE_SCHED_BITMAP_PUSCH               0x20

/* bit implication of schedmap byte for HARQ for UL data */
#define HD_UE_SCHED_BITMAP_UL_HARQ             0x40

/* bit implication of schedmap byte for DL data */
#define HD_UE_SCHED_BITMAP_PDSCH               0x80
/* bit implication of schedmap byte for DCI info */
#define HD_UE_SCHED_BITMAP_PDCCH               0x80

/* bit-0   SR
 * bit-1   RI
 * bit-2   CQI/PMI
 * bit-3   SRS
 * bit-4   HARQ for DL data in Uplink
 * bit-5   UL data */
#define HD_UE_SCHED_UL_BITMAP    0x3F

/* bit-6   HARQ for UL data in downlink
 * bit-7   DL Data */
#define HD_UE_SCHED_DL_BITMAP    0xC0

/** offsets of report in reporting info array - hdUeReportingInfo_t **/
#define HD_UE_STATIC_MAP_SR                 0
#define HD_UE_STATIC_MAP_RI                 1
#define HD_UE_STATIC_MAP_CQI_PMI            2
#define HD_UE_STATIC_MAP_SRS                3
#define HD_UE_STATIC_MAP_INVALID            4

/*Schedule map 
 *------------------------------------------------
 *|0x80||0x40||0x20||0x10||0x08||0x04||0x02||0x01|
 *------------------------------------------------
 *| Dl || UL || UL || DL || SRS|| CQI|| RI ||SR ||
 *|Data||Harq||Data||Harq||    || PMI||    ||   ||
 *-------------------------------------------------
 *        ^           ^
 *        |           |
 * HARQ for UL data HARQ for DL data 
 *   in downlink    in Uplink
 *
 */

/* updates schedMap byte for subframe sfn for ue by setting bits for reports that are timedout in sfn 
 *----------------------------------
 *|0 |   1 | 2| 3| 4| 5| 6| 7| 8| 9|
 *----------------------------------
 *|  | 0x04|  |  |  |  |  |  |  |  |
 *----------------------------------
 *      ^
 *      |
 *   Current SFN 
 * set CQI reporting if timeout happens
 * Note:0-9 represents subframes
 */
#define UPDATE_HD_UE_SCHED_MAP(sfn, ue) \
        hdUeContextInfo_g.hdfddUeSchedMap[(sfn)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[ue] = (\
        ((!(hdUeContextInfo_g.hdUeReportingTimeOuts[ue][HD_UE_STATIC_MAP_SR]-(sfn)))<< HD_UE_STATIC_MAP_SR ) |\
        ((!(hdUeContextInfo_g.hdUeReportingTimeOuts[ue][HD_UE_STATIC_MAP_RI]-(sfn)))<< HD_UE_STATIC_MAP_RI ) | \
        ((!(hdUeContextInfo_g.hdUeReportingTimeOuts[ue][HD_UE_STATIC_MAP_CQI_PMI]-(sfn)))<< HD_UE_STATIC_MAP_CQI_PMI )| \
        ((!(hdUeContextInfo_g.hdUeReportingTimeOuts[ue][HD_UE_STATIC_MAP_SRS]-(sfn)))<< HD_UE_STATIC_MAP_SRS ) )

/* Reinit the periodic reporting bits in case of config change */
#define REINIT_HD_UE_REPORTING_BITS(sfn, ue) \
        (hdUeContextInfo_g.hdfddUeSchedMap[(sfn)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[ue] &= ~(HD_FDD_BITMASK_UL_REPORTS))

/* Checks DL can be scheduled for HD UE
 * if current tick is not UL (used for sending DCI1x/2x and DL data)
 * and current tick +4 is not DL (used for receiving HARQ by UE)
 *-------------------------------
 *|0 | 1| 2| 3| 4| 5| 6| 7| 8| 9|
 *-------------------------------
 *|DL|  |  |  |UL|  |  |  |  |  |-->macro results TRUE (also returns true if empty)
 *-------------------------------
 *-------------------------------
 *|UL|  |  |  |DL|  |  |  |  |  |-->macro results FALSE
 *-------------------------------
 * ^           ^
 * |           |
 * X           X+4
 *(X-represents tick)
 * Note:0-9 represents subframes
 */
#define CAN_SCHEDULE_DL_FOR_HD_UE(tick, ue, map) \
    (!((map[(tick)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[ue] & HD_UE_SCHED_UL_BITMAP /**& ~(hdUeContextInfo_g.ulSrPresentFlag[ue])**/ )||\
       (map[(tick+HD_FDD_SCHED_MAP_OFFSET_FOUR)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[ue] & HD_UE_SCHED_DL_BITMAP /**& ~(hdUeContextInfo_g.ulSrPresentFlag[ue])**/ )))

/* Checks UL can be scheduled for HD UE
 * if current tick is not UL (used for sending DCI0)
 * and current tick + 4 is not DL (used for receiving UL data)
 * and current tick + 8 is not UL (used for sending PHICH)
 *-------------------------------
 *|0 | 1| 2| 3| 4| 5| 6| 7| 8| 9|
 *-------------------------------
 *|DL|  |  |  |UL|  |  |  |DL|  |-->macro results TRUE (also returns true if empty)
 *-------------------------------
 *-------------------------------
 *|UL|  |  |  |DL|  |  |  |UL|  |-->macro results FALSE
 *-------------------------------
 * ^           ^           ^
 * |           |           |
 * X           X+4        X+8
 *(X-represents tick)
 * Note:0-9 represents subframes
 */
#define CAN_SCHEDULE_UL_FOR_HD_UE(tick, ue, map)\
    (!((map[(tick)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[ue] & HD_UE_SCHED_UL_BITMAP)||\
       (map[(tick+HD_FDD_SCHED_MAP_OFFSET_FOUR)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[ue] & HD_UE_SCHED_DL_BITMAP)||\
       (map[(tick+HD_FDD_SCHED_MAP_OFFSET_EIGHT)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[ue] & HD_UE_SCHED_UL_BITMAP) ))


#define CAN_SCHEDULE_TPC_FOR_HD_UE(tick, ue, map) \
    (!(map[(tick)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[ue] & HD_UE_SCHED_UL_BITMAP))\

/* Schedule the DL for HD UE
 * Mark Current subframe X as DL X+4 as UL
 *-------------------------------
 *|0 | 1| 2| 3| 4| 5| 6| 7| 8| 9|
 *-------------------------------
 *|DL|  |  |  |UL|  |  |  |  |  |
 *-------------------------------
 * ^           ^           
 * |           |           
 * X           X+4        
 *(X-represents tick)
 * Note:0-9 represents subframes */
#define SCHEDULE_DL_FOR_HD_UE(tick, ue, map,internalCellIndex)    \
        if(CAN_SCHEDULE_DL_FOR_HD_UE(tick, ue, map))\
        {\
            gMacStats.pStats->lteMacHdFddStats.numDLSchedule++; \
            gMacStats.pStats->lteMacHdFddStats.numTimesUESchedDL[ue]++; \
            map[(tick)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[ue] |= HD_UE_SCHED_BITMAP_PDSCH;    \
            map[(tick+HD_FDD_SCHED_MAP_OFFSET_FOUR)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[ue] |= HD_UE_SCHED_BITMAP_DL_HARQ;  \
        }\
        else\
        {\
            LOG_MAC_MSG(MAC_HD_FDD_DL_CLASH_WITH_UL,LOGFATAL,MAC_RRC_INF, \
                    getCurrentTick(),ue, \
                    LINE_NUMBER,cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
                    ->initParams_p->dlSchdConfig.dlSchedulerStrategy, \
                    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
                     ->initParams_p->ulSchdConfig.ulSchedulerStrategy,DEFAULT_INT_VALUE, \
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE, \
                    FUNCTION_NAME, \
                    "MAC_HD_FDD_DL_CLASH_WITH_UL"); \
        }

/* Schedule the UL for HD UE
 * Mark Current subframe X as UL X+4 as DL
 * and X+8 ad UL
 *-------------------------------
 *|0 | 1| 2| 3| 4| 5| 6| 7| 8| 9|
 *-------------------------------
 *|UL|  |  |  |DL|  |  |  |UL|  |
 *-------------------------------
 * ^           ^           ^
 * |           |           |
 * X           X+4        X+8
 *(X-represents tick)
 * Note:0-9 represents subframes */
#define SCHEDULE_UL_FOR_HD_UE(tick, ue, map,internalCellIndex)\
        if(CAN_SCHEDULE_UL_FOR_HD_UE(tick, ue, map)) \
        {\
            gMacStats.pStats->lteMacHdFddStats.numULSchedule++; \
            gMacStats.pStats->lteMacHdFddStats.numTimesUESchedUL[ue]++; \
            map[(tick)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[ue] |= HD_UE_SCHED_BITMAP_PDCCH;\
            map[(tick+HD_FDD_SCHED_MAP_OFFSET_FOUR)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[ue] |= HD_UE_SCHED_BITMAP_PUSCH;\
            map[(tick+HD_FDD_SCHED_MAP_OFFSET_EIGHT)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[ue] |= HD_UE_SCHED_BITMAP_UL_HARQ;  \
        }\
        else \
        { \
            LOG_MAC_MSG(MAC_HD_FDD_UL_CLASH_WITH_DL,LOGFATAL,MAC_RRC_INF, \
                    getCurrentTick(),ue, \
                    LINE_NUMBER,cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
                     ->initParams_p->dlSchdConfig.dlSchedulerStrategy, \
                     cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
                     ->initParams_p->ulSchdConfig.ulSchedulerStrategy,DEFAULT_INT_VALUE, \
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE, \
                    FUNCTION_NAME, \
                    "MAC_HD_FDD_UL_CLASH_WITH_DL"); \
        }

/*SPR 5424 fix start*/
/* Schedule the UL for HD UE
 * Mark Current subframe X as DL X+4 as UL
 * and X+8 ad DL
 *-------------------------------
 *|0 | 1| 2| 3| 4| 5| 6| 7| 8| 9|
 *-------------------------------
 *|DL|  |  |  |UL|  |  |  |DL|  |
 *-------------------------------
 * ^           ^           ^
 * |           |           |
 * X           X+4        X+8
 *(X-represents tick)
 * Note:0-9 represents subframes */
/*Marking the current+4 UL and current+8 DL respectively in NACK scenario even we are not able to schedule current+4 as UL current+8 as DL,
 *strategy will take the decision on current+4 and current+8 to schedule DL/UL at respective tick if its free and marked as UL/DL it treats UL/DL
 *else if we configure it DL+UL, strategy wont configure DL for that UE.Let ReTx of UE happen as UE works on adaptive Transmission*/
#define SCHEDULE_UL_NACK_FOR_HD_UE(tick, ue, map)\
        {\
            map[(tick)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[ue] |= HD_UE_SCHED_BITMAP_PDCCH;\
            map[(tick+HD_FDD_SCHED_MAP_OFFSET_FOUR)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[ue] |= HD_UE_SCHED_BITMAP_PUSCH;\
            map[(tick+HD_FDD_SCHED_MAP_OFFSET_EIGHT)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[ue] |= HD_UE_SCHED_BITMAP_UL_HARQ;  \
        }
/*SPR 5424 fix end*/


/* Checks if the current frame is DL and next frame is for UL to reduced MCS
 *---------------------------------
 *|UL|DL|DL|DL|DL|UL|UL|UL|DL|DL|
 *---------------------------------
 *               ^
 *               |
 *               |
 *             Reduced MCS required
 *             In switching between
 *              DL and UL
 */
#define HD_GET_REDUCED_MCS_SINGLE_CW_DL_UL_SWITCH(tick, dlUEContext_p, mcsIndex)\
        if(dlUEContext_p->isHDFddFlag &&\
           (hdUeContextInfo_g.hdfddUeSchedMap[tick%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[dlUEContext_p->ueIndex] & HD_UE_SCHED_DL_BITMAP) &&\
           (hdUeContextInfo_g.hdfddUeSchedMap[(tick+1)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[dlUEContext_p->ueIndex] & HD_UE_SCHED_UL_BITMAP))\
        {\
            mcsIndex = getHDReducedMCSFromCQI(mcsIndex);\
        }

/* Checks if the UE is HD FDD,current frame is DL NEW_TX and next frame is for UL to reduced MCS
 * else fill the default MCS values from the Dl context
 *---------------------------------
 *|UL|DL|DL|DL|DL|UL|UL|UL|DL|DL|
 *---------------------------------
 *               ^
 *               |
 *               |
 *             Reduced MCS required
 *             In switching between
 *              DL and UL
 */
#define HD_GET_REDUCED_MCS_CW1_CW2_DL_UL_SWITCH(tick, dlUEContext_p, mcsIndexCW1, mcsIndexCW2)\
        if((dlUEContext_p->isHDFddFlag) &&\
           (hdUeContextInfo_g.hdfddUeSchedMap[tick%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[dlUEContext_p->ueIndex] & HD_UE_SCHED_DL_BITMAP) &&\
           (hdUeContextInfo_g.hdfddUeSchedMap[(tick+1)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[dlUEContext_p->ueIndex] & HD_UE_SCHED_UL_BITMAP))\
        {\
            mcsIndexCW1 = getHDReducedMCSFromCQI(mcsIndexCW1);\
            mcsIndexCW2 = getHDReducedMCSFromCQI(mcsIndexCW2);\
        }

/** sets a periodic map for a UE with given 
 * pr- priodicity 
 * type - CQI/PMI/RI, SR, SRS
 * **/

#define HD_UE_SET_REPORT_INFO(sfn, offset, pr, type, ue) \
       { \
       UInt32 timeout = 0; \
       if(pr) \
       {\
           hdUeContextInfo_g.hdUeReportingInfo[ue][type].reportOffset = offset; \
           hdUeContextInfo_g.hdUeReportingInfo[ue][type].periodicity = pr; \
           timeout = ((sfn/pr) * pr) + offset; \
           /* If the current SFN is already passed, increment the timeout by next periodicity */ \
           if (timeout <= sfn) \
           { \
           hdUeContextInfo_g.hdUeReportingTimeOuts[ue][type] = timeout + pr; \
           } \
           else \
           { \
           hdUeContextInfo_g.hdUeReportingTimeOuts[ue][type] = timeout; \
           } \
       }\
       else\
       {\
           hdUeContextInfo_g.hdUeReportingInfo[ue][type].periodicity = 0; \
       }\
      }

/* Reset the reports when the release request in UE reconfiguration */
/* Also update the configChange so that sched-map could be updted for all SFs */
#define HD_UE_RESET_REPORT_INFO(ue, type) \
       hdUeContextInfo_g.hdUeReportingInfo[ue][type].oneShot = 0; \
       hdUeContextInfo_g.hdUeReportingInfo[ue][type].reportOffset = 0; \
       hdUeContextInfo_g.hdUeReportingInfo[ue][type].periodicity = 0; \
       hdUeContextInfo_g.hdUeReportingTimeOuts[ue][type] = 0; \
       hdUeContextInfo_g.configChange[ue] = TRUE;

/* Increment the statistics counter UE denied UL resources due to
 * DL clash in the same TTI of HD FDD UE of ueId*/
#define UPDATE_UE_DENIED_UL_RESOURCE(ueId)\
        gMacStats.pStats->lteMacHdFddStats.totalNumTimesUEdeniedUL[ueId]++;

/* Increment the statistics counter UE denied DL resources due to
 * UL clash in the same TTI of HD FDD UE of ueId*/
#define UPDATE_UE_DENIED_DL_RESOURCE(ueId)\
        gMacStats.pStats->lteMacHdFddStats.totalNumTimesUEdeniedDL[ueId]++;

/* Reset the statistics counter UE denied DL and UL resources due to
 *  * release of HD FDD UE of ueId*/
#define RESET_UE_STATS_RESOURCES(ueId)\
        gMacStats.pStats->lteMacHdFddStats.hdFddFlag[ueId]=FALSE; \
        gMacStats.pStats->lteMacHdFddStats.numTimesUESchedDL[ueId]=0; \
        gMacStats.pStats->lteMacHdFddStats.numTimesUESchedUL[ueId]=0; \
        gMacStats.pStats->lteMacHdFddStats.totalNumTimesUEdeniedDL[ueId]=0;\
        gMacStats.pStats->lteMacHdFddStats.totalNumTimesUEdeniedUL[ueId]=0;

/*Structure to store Semi static Map*/

typedef struct hdUeReportingInfoT
{
    UInt8 oneShot; /* used for SRS if it is one timereport or indefinite */
    UInt16 reportOffset;
    UInt32 periodicity;
}HdUeReportingInfo;

/* Structure to store Sched Map for Number of UE Supported 
 * Stores the data for next 10 subframes including current */
typedef struct hdUeSchedMapT
{
  UInt8 hdfddUeSfMap[MAX_UE_SUPPORTED];
} HdUeSchedMap;

/*Structure to store HD UE context */
typedef struct hdUeContextInfoT{
    UInt8 numHdUe; /* Number of Half Duplex UE in system */
    /* This variable stores the compact array of UE Index for numHdUe */
    UInt16 hdUeIndex[MAX_UE_SUPPORTED];

    /* These below variable use the UE Index stored in hdUeIndex for
     * storing the resepective params */

    /* This variable is used to track the case when UE reconfiguration happens
     * and the schedule-map needs to be updated for all sub frames */
    UInt8 configChange[MAX_UE_SUPPORTED];

    /* This informs whether SR is expected from UE or not 
     * FALSE - indicates SR expected from UE 
     * TRUE - indicates SR not expected as BSR is non-zero */
    UInt8 ulSrPresentFlag[MAX_UE_SUPPORTED];

    /** HD FDD schedular Map  **/
    HdUeSchedMap hdfddUeSchedMap[HD_UE_SCHED_MAP_SIZE];

    /** information of all reporting types  **/
    HdUeReportingInfo hdUeReportingInfo[MAX_UE_SUPPORTED][HD_UE_NUM_OF_REPORTS];

    /** next timeout for a report of HD FDD UE**/
    UInt32 hdUeReportingTimeOuts[MAX_UE_SUPPORTED][HD_UE_NUM_OF_REPORTS];
}HdUeContextInfo;

/** access handle for Half Duplex UE Context information **/
extern HdUeContextInfo hdUeContextInfo_g;

#endif
/* HD FDD Changes End*/
/*CA chngs start */
typedef struct  TopSchedulableUEInfoT
{
    UInt16 ueIndex;
    /* SPR 16116 fix start */
    UInt32 totLCWeight;
    /* SPR 16116 fix end */
} TopSchedulableUEInfo;
typedef struct  TopSchedulableUEsT
{
    UInt8  arrCount;
    /* SPR 16116 fix start */
    TopSchedulableUEInfo   topSchedulableUEInfo[MAX_TOP_SCHEDULABLE_UE_LIST_SIZE];
    /* SPR 16116 fix end */
}TopSchedulableUEs;
extern TopSchedulableUEs topSchedulableUEs_g[MAX_NUM_SCELL + MAX_NUM_PCELL];
extern TopSchedulableUEs topSchedulableVictimUEs_g[MAX_NUM_SCELL + MAX_NUM_PCELL];
/*CA chngs end */
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern DLUEContextInfo dlUECtxInfoArr_g[MAX_UE_SUPPORTED];
extern LTE_LIST  ueNonZeroQueueLoadAndHarqList_g;
/*CA Changes start */
extern LTE_SQUEUE*    pendingUEDeleteQueue_gp[MAX_NUM_CELL];
/*CA Changes end */
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* Cyclomatic_complexity_changes_start */
        /* SPR 15909 fix start */
 void checkForAperiodicCQITrigger(tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        DLCQIInfo *dlCQIInfo_p,
        DLUEContext *dlUEContext_p,
        UInt8 periodicModeCqiValue,
        UInt8 initialAperiodicReportedCQI,
        UInt8 subBandValueDiff,
        InternalCellIndex internalCellIndex);
/* Cyclomatic_complexity_changes_end */

#endif  /* FILENAME_H */

