/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2UplinkUEContext.h,v 1.2.6.1.4.2.2.1 2010/09/21 15:53:19 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains declarations for the data
 *                      structures common to some modules.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteLayer2UplinkUEContext.h,v $
 * Revision 1.2.6.1.4.2.2.1  2010/09/21 15:53:19  gur20491
 * FAPI changes
 *
 * Revision 1.2.6.1  2010/08/02 08:57:18  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.1.1.1  2010/06/01 07:49:19  cm_netlogic
 * code import
 *
 * Revision 1.2  2010/02/13 10:44:27  gur15697
 * changes for UL flow Stability
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.16  2009/11/09 13:01:54  gur11912
 * modified for UL NACK cases
 *
 * Revision 1.15  2009/09/02 13:30:41  gur20439
 * Uplink Data not going for NACK case fix.
 *
 * Revision 1.14  2009/07/29 15:16:59  gur20439
 * look and feel to simulator fix.
 *
 * Revision 1.13  2009/07/29 10:27:59  gur15697
 * cleanup bug fix
 *
 * Revision 1.12  2009/07/24 14:39:20  gur20439
 * support added for non adaptive retransmission for UPLINK.
 *
 * Revision 1.11  2009/07/17 15:02:57  gur20439
 * Random access fix.
 *
 * Revision 1.10  2009/07/17 06:25:35  gur20439
 * changes related to rrc interface modification incorporated.
 *
 * Revision 1.9  2009/07/15 14:33:31  gur20439
 * timing advance fix.
 *
 * Revision 1.8  2009/07/15 14:24:34  gur20439
 * bug fix for crc.
 *
 * Revision 1.7  2009/07/14 14:44:54  gur20439
 * rrc interface code.
 *
 * Revision 1.6  2009/06/15 13:51:34  gur20439
 * modifed for release 1.2.
 *
 * Revision 1.5  2009/06/10 14:29:57  gur20439
 * changes for random access.
 *
 * Revision 1.4  2009/06/08 13:56:20  gur20439
 * modified for release 1.2
 *
 * Revision 1.3  2009/06/01 10:52:12  gur20439
 * changes for mac 1.2.
 *
 * Revision 1.2  2009/05/26 07:32:36  gur20439
 * modified for mac release 1.2
 *
 * Revision 1.1  2009/03/30 10:20:16  gur19836
 * Initial Version
 *
 * Revision 1.9  2009/02/23 07:38:51  gur10445
 * UL BSR FIX
 *
 * Revision 1.8  2009/01/29 13:25:53  gur20052
 * Added Changed related to the Stats MAnager of Uplink
 *
 * Revision 1.7  2009/01/20 07:53:44  gur19836
 * Changes done for interface with RRC Layer
 *
 * Revision 1.6  2009/01/15 05:02:22  gur20052
 * Review comments of vandana are incorporated.Added a new field bsrNetTotal in dynamic context contains sum of the BSR of all the 4 logical channel groups
 *
 * Revision 1.5  2009/01/12 09:43:17  gur19836
 * numLogicalChannels variable added in UL UE Context
 *
 * Revision 1.4  2008/12/23 07:48:20  gur20052
 * Changes of SR implementation incorporated
 *
 * Revision 1.3  2008/12/19 12:16:50  gur20052
 * sRPeriodicity is added in dynamic UE Context
 *
 * Revision 1.2  2008/12/10 06:05:20  gur19836
 * updated for linking
 *
 * Revision 1.1  2008/12/08 13:12:27  gur12905
 * Initial Revision phase - 2
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_UPLINK_UECONTEXT_H
#define LTE_UPLINK_UECONTEXT_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteLayer2RRCInterface.h"

#include "lteFapiMacPhyInterface.h"

//#include "phySimulatorApiHandler.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Constants
 ****************************************************************************/
/*+ CL-MIMO RSM */
#define PMI_ARRAY_SIZE 4
/*- CL-MIMO RSM */
#define	NUM_OF_LCG	4
#define MAX_CCCH_SDU 10
#define MAX_NUM_OF_CQI 10
#define MAX_NUM_OF_PERIODIC_CQI 10
#define MAX_NUM_OF_APERIODIC_CQI 5 
#ifdef TDD_CONFIG
#define MAX_HARQ_CONT 2
#endif
/*crc changes*/
#define QSIZE 80
/*crc changes*/

/* ULA_CHG */
#define MAX_UE_PROFILES 5
/* + CQI_4.1 */
/* CLPC_CHG */
#define NUM_PROFILE_ID 5
/* CLPC_CHG */
/* - CQI_4.1 */
#define MAX_SRS_PROFILES 5
#define MAX_SRS_SINR_PROFILES 5
/* +DYNAMIC_ICIC */
#define MAX_INTER_PROFILES 5
/* -DYNAMIC_ICIC */

/*Added for Power Control*/
#define MAX_PHR_PROFILES 5
#define NUM_PHR_PARAMETER 3
#define MAX_ULSCH_PROFILES 5
#define MAX_ULSCH_SINR_PROFILES 5
/* CLPC_CHG */
#define MAX_CQI_PROFILES 5
#define MAX_CQI_SINR_PROFILES 5
/* CLPC_CHG */
#define MAX_PRB 100
#define MAX_SINR_VAL 255
#define NUM_SRS_PARAMETER 4
#define NUM_ULSCH_PARAMETER 1
/* CLPC_CHG */
#define NUM_CQI_PARAMETER 1
//#define MAX_LINE_SIZE 256

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
UInt8 lastUeProfileId_g;
/*CLPC_CHG*/
extern UInt8 perAckTb1_g[MAX_NUM_CELLS];
extern UInt8 perNackTb1_g[MAX_NUM_CELLS];
extern UInt8 perUnsureTb1_g[MAX_NUM_CELLS];
extern UInt8 perAckTb2_g[MAX_NUM_CELLS];
extern UInt8 perNackTb2_g[MAX_NUM_CELLS];
extern UInt8 perUnsureTb2_g[MAX_NUM_CELLS];
/*CLPC_CHG*/



/* ULA_CHG */

typedef struct subBandInfoT{
    UInt8       cqiIndex;   /*CQI index received*/
    UInt8       subbandNum;       /*Sub band number value*/
    UInt8       pmiValue;
    UInt8       subbandCQIValueCodeword0;
    UInt8       subbandCQIValueCodeword1;
}subBandInfo;

typedef struct AperiodicInfoT
{
   UInt8 reportType;
   UInt8 wideBandIndex;
   UInt8 widebandCQIValueCodeword0;
   UInt8 widebandCQIValueCodeword1;
   UInt8 widebandPMI;
   UInt8 cqiValueCodeWord0;
   UInt8 cqiValueCodeWord1;
   UInt8 cqiVal;
   UInt8 subBandLength;
   UInt8 pmiValue;
   subBandInfo *subCqiInfo;
}AperiodicInfo;

typedef struct PeriodicInfoT
{
   UInt8 reportType;
   UInt8 periodicity;
   UInt8 wideBandIndex;
   UInt8 widebandCQIValueCodeword0;
   UInt8 widebandCQIValueCodeword1;
   UInt8 widebandPMI;
   UInt8 riVal;
   UInt8 bandwidthPart;
   UInt8 Lbitlabel;
   UInt8 cqiVal;
   UInt8 cqiValueCodeWord0;
   UInt8 cqiValueCodeWord1;
}PeriodicInfo;

typedef struct PeriodicCQIT
{
   UInt8 numInstances;
   UInt8 nextToSend;
   PeriodicInfo periodicInfo[MAX_NUM_OF_PERIODIC_CQI];
}PeriodicCQI;

typedef struct APeriodicCQIT
{
   UInt8 numInstances;
   UInt8 nextToSend;
   AperiodicInfo aperiodicInfo[MAX_NUM_OF_APERIODIC_CQI];
}APeriodicCQI;

typedef struct PDUInfoT {
       UInt16 PDULength;
       UInt8 *Pdudata_p;
}PDUInfo;
typedef struct UlControlQueueNodeT {
	LTE_SNODE   ulControlAnchor;
	UInt8 SF;
        //UInt8 crcInd;
        UInt8 crc;
        UInt8 twoTbFlag;
        UInt8 tb1harqInfo;
        UInt8 tb2harqInfo;
        UInt8 contentionAckFlag;
	UInt16 crnti;
	UInt16 SFN;
	UInt16 PDUChoice;
        PDUInfo pdu;
}UlControlQueueNode;
typedef struct UlDataQueueNodeT 
{
  LTE_SNODE   ulDataAnchor;
  UInt8 SF;
  UInt8 cqiMode;
  UInt8  msg3Flag;
  UInt16 crnti;
  UInt16 SFN;
	UInt16 PDUChoice;
        PDUInfo pdu;
  UInt8*  cqiPdu_p;
  UInt32  cqiLen;
  UInt8  qIndex;
}UlDataQueueNode;
typedef enum RNTITypeE {
         NONE = 0,
         PRNTI = 1, 
         SIRNTI, 
         RA_RNTI, 
         T_CRNTI,
         CRNTI,
         SPS_CRNTI
}RNTIType;
typedef struct RNTIInfoT{
         UInt8 state;
         UInt16 ueIndex;
         UInt16 rnti;
         UInt16 RaRnti;
         RNTIType rntiType;
         UInt8  ccchSDUArr[MAX_CCCH_SDU];
         //UInt16 rnti;
         APeriodicCQI *aperiodiccqiInfo;
         PeriodicCQI *periodiccqiInfo;
}RNTIInfo;
typedef enum UEStateE {
        UE_INACTIVE = 0x00,
        UE_NOT_ADDED,
        SR_NOT_SENT,
        SG_WAIT,
        WAIT_RAR,
        WAIT_CONTENTION_RESOLUTION,
        WAIT_UL_GRANT,
        UE_ACTIVE,
        WAIT_FOR_DL_CCCH_MSG
}ueState;



/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef struct FAPI_ulSrIndicationNode_stT
{
    LTE_SNODE srNodeAnchor;
    UInt16 sfnsf;
    FAPI_srPduIndication_st srPduInfo;

}FAPI_ulSrIndicationNode_st;

/* SS_S2 */
typedef struct FAPI_cqiIndQueueNode_stT
{
    LTE_SNODE   cqiNodeAnchor;
    UInt16      sfnsf;
#if defined(FAPI_4_0_COMPLIANCE) && (defined(FAPI_RELEASE9) ||defined(FAPI_RELEASE10))
    FAPI_cqiPduIndicationR9_st cqiPduInfo;
#else
    FAPI_cqiPduIndication_st cqiPduInfo;
#endif
}FAPI_cqiIndQueueNode_st;
/* SS_S2 */
/*structure for the UL HARQ information*/
typedef struct ULHarqInfoT{
    UInt32  deltabsrLoad; /* UL_BSR_FIX*/
    UInt8	riStart; /*indicates the starting index of the resorces*/
    UInt8	riLength;	/*indicates number of contigiously allocated RBs*/
    UInt8	ackNack;	/*indicates ACK or NACK (0 for NACK /1 for ACK) */
    UInt8	currentRetransCount;/*indicates the retrans count for current TTI/HARQ*/
    UInt8   ulgrantrecvCount;
    UInt32  tbSizeForNackCase;
#ifdef UESIM_ENHANCEMENTS
    UInt8   numOfRB[10];
    UInt8   riLen;
#endif    
#ifdef FDD_CONFIG
    /** For TTIB_Code */
    FAPI_dlDCIPduInfo_st dciPdu;
    TransmissionAndRLCData    *transmissionBuffer_p;
#endif
}ULHarqInfo;
#ifdef TDD_CONFIG
typedef struct ULHARQProcessT
{
    SInt8    harqProcessId;   /* The assigned HARQ Process Id */
    /* Used to signify that particular HARQ process is available or not */
    UInt8 harqStatus;
    /* indicate sub band, wide band or no frequency selective type of transmission*/
    UInt8 transmissionType;
    /* The following field will contain the TICK for which this HARQ process should
       expire */
    UInt8   ulgrantrecvCount;
    UInt8   currentRetransCount;/*indicates the retrans count for current TTI/HARQ*/

    UInt8    assignedResourcesNB;
    UInt8    *transmissionBuffer_p;
    UInt32   totalTBSize;
    UInt8    *timer_p;
    /*UESIM ADAPTIVE RETX FIX*/
#ifdef TDD_CONFIG
    FAPI_dlDCIPduInfo_st dciPdu;
    UInt8   ulSchSendFlag;
#endif
    /*UESIM ADAPTIVE RETX FIX*/
    //ULHARQTBInfo       ulHARQTBOneInfo;
} ULHARQProcess;

typedef struct ULHARQEntityT
{
    UInt32       nackQueueLoad;
    LTE_SQUEUE   retransmissionQueue;
    UInt8        numHARQProcess;
    UInt8        hiReceived[MAX_SUBFRAME];
    SInt8        harqId[MAX_SUBFRAME];
    ULHARQProcess   ulHarqProcess[TDD_MAX_UL_HARQ_PROCESS];
} ULHARQEntity;

typedef struct ULSRIScheduleT
{
    UInt16 sfn;
    UInt8  sf;
    UInt8 isValid;  // TRUE if the SFN and SF were populated to schedule a SRI at least once
} ULSRISchedule;

typedef struct DaiTrackingRegisterT
{
    UInt8 daiCount;         // Counter to keep track of received data
    UInt8 lastReceivedDai;  // DAI value received in last non-zero DCI format info
        //Used for the cases when both UL and DL of a UE are active
    UInt8 daiUpdated;
    UInt8 isTB1Valid;
    UInt8 isTB2Valid;
    UInt8 ackNackFieldTB1;
    UInt8 ackNackFieldTB2;
    UInt8 daiValue;
/* SPR 19679: TDD HARQ Multiplexing Changes Start*/
    UInt8 vuldai;
/* SPR 19679: TDD HARQ Multiplexing Changes End*/
} DaiTrackingRegister;

typedef struct tddBundlingHarqInfo
{
    UInt8 value0;
    UInt8 value1;
    /* SPR 10867 Fix Start */
    UInt8 padding[2];
    /* SPR 10867 Fix End */
}tddBundlingHarqInfo_st;
/* SPR 19679: TDD HARQ Multiplexing Changes Start*/
typedef struct tddMutliplexingHarqInfo_st
{
    UInt8 value0;
    UInt8 value1;
    UInt8 spsOcc;
    UInt8 daiIndexValue;
    /** Stores SF for which HARQ is scheduled **/
    UInt32 scheduledSF;
}tddMultiplexingHarqInfo_st;
/* SPR 19679: TDD HARQ Multiplexing Changes End */
/* + CA_TDD_HARQ_CHANGES */
typedef struct tddChhannelSelectionHarqInfo
{
    UInt8 value0;
    /** Stores SF for which HARQ is scheduled **/
    UInt32 scheduledSF;
}tddChannelSelectionHarqInfo_st;
/* - CA_TDD_HARQ_CHANGES */

/* TDD HARQ PDU Info Structure */
typedef struct tddHarqPduIndication_st
{
    /* The handle received in the ULSCH PDU or UCI PDU. */
    uint32_t    handle;

    /* The RNTI passed to the PHY in an uplink subframe configuration PDU. 
    Value: 1 . 65535.  */
    uint16_t    rnti;

    /* The format of the ACK/NACK response expected. The bundling and 
    multiplexing options are passed to the PHY in an uplink subframe 
    configuration PDU. If the ACK/NACK is combined with either CQI or SR 
    information then a special ACK/NACK encoding is used which reports 
    the number of ACKs, rather than providing specific ACK/NACK values. 
    This is identified separately and called SPECIAL_BUNDLING in this API. 
    0 = BUNDLING
    1 = MULTIPLEXING   */
    uint8_t     mode;

    /* The number of ACK/NACK results reported for this UE.
    Value: 1 . 4 
    For Special Bundling this is the expected number of ACK/NACK 
    responses (UDAI + NSPS).
    Value: 0 . 9  */
    uint8_t     numOfAckNack;

    /* The format of the data is dependent on the HARQ mode; 
    BUNDLING, MULTIPLEXING, or SPECIAL BUNDLING.  
    need to typecast it with appropriate TTD Harq structures
    defined above */
    /* Below need to made be an union of all the acknack modes supported */
 //   uint8_t     harqBuffer[FAPI_VAR_SIZE(x)];
    tddBundlingHarqInfo_st harqBuffer;
    /* SPR 19679: TDD HARQ Multiplexing Changes Start*/
    tddMultiplexingHarqInfo_st multiplexingHarqBuffer[4];
    /* SPR 19679: TDD HARQ Multiplexing Changes End */

    /* + CA_TDD_HARQ_CHANGES */
    /* The format of the data is for CHANNEL SELECTIO HARQ mode; 
    need to typecast it with appropriate TTD Harq structures
    defined above */
    /** This value is kept in accordance with M=4 & 2serving cells */
    tddChannelSelectionHarqInfo_st channelSelectionHarqBuffer[8];
    /* - CA_TDD_HARQ_CHANGES */

}tddHarqPduIndication_st;

#endif
typedef struct FAPI_harqIndQueueNode_stT
{
  LTE_SNODE   harqNodeAnchor;
  UInt16      sfnsf;
  #ifdef FDD_CONFIG
#ifndef FAPI_4_0_COMPLIANCE
  FAPI_fddHarqPduIndication_st  harqPduInfo
#else
#if (defined(FAPI_RELEASE10) || defined(FAPI_RELEASE9))
  FAPI_fddHarqPduIndicationR9_st harqPduInfo;
#else
  FAPI_fddHarqPduIndicationR8_st harqPduInfo;
#endif
#endif
  #elif TDD_CONFIG
  /* + CA_TDD_HARQ_CHANGES */
  UInt8 isScheduledOnPUSCH;
/* SPR 19679: TDD HARQ Multiplexing Changes Start*/
  UInt8 isScheduledOnPUSCHForMux;
/* SPR 19679: TDD HARQ Multiplexing Changes End*/
  /* - CA_TDD_HARQ_CHANGES */
  tddHarqPduIndication_st harqPduInfo;
  #endif

}FAPI_harqIndQueueNode_st;

typedef struct HarqIndQueueNodeInfoT{
    UInt8 state;
    UInt8 qIndex;
    UInt8 isScell;
}HarqIndQueueNodeInfo;




typedef struct DedicatedULPowerControlInfoT
{
    /*Indicates UE specific component for calculating transmitted
     *      * power for UE*/
    SInt8 p0UePusch;
    /*Used to calculate UE Transmit power for PUSCH */
    UInt8 deltaMcsEnabled;
    /*Indicates whether accumulated power control is enabled or not */
    UInt8 accumulationEnabled;
}DedicatedULPowerControlInfo;

/*CLPC_CHG*/

/* Structure maintained in UE context to be used for occasion */
typedef struct SpsOccUeContextT
{
    FAPI_dlDCIPduInfo_st dciPdu;
    UInt8 recvSF;
    UInt16 recvSFN;
    /* + SPS_TDD_Changes */
    UInt8 startSF;
    UInt16 startSFN;
    UInt32 N;
    /* - SPS_TDD_Changes */
    UInt8 delay;
} *LP_SpsOccUeContext, SpsOccUeContext; 
#ifdef FDD_CONFIG
    /** For TTIB_Code */
typedef struct TTIBundlingInfoT
{
        UInt8 bundleStartIndex;
}TTIBundlingInfo;
#endif
#if 0
/*CA changes start*/
/*This structure contains Scell specific information
*required for uplink processing*/

/*CA changes start*/
typedef struct CqiConfigT
{
    UInt8  cqiIndication;
    UInt8  cqiMode;
    UInt16 cqiPMIConfigIndex;
    UInt16 cqiPucchResourceIndex;
    UInt16 riConfigIndex;
    UInt16 cqiformatIndicatorPeriodic;
    UInt8  cqiformatIndicatorPeriodic_subband_k;
    UInt8  periodicModeType;
};

/*This structure contains Scell specific information
*required for uplink processing*/
typedef struct ULUESCellContextT
{
    UInt8 scellIndex;
    /*pucch format configuration for release 10 UE */
    PucchFormatR10 pucchFormatR10;
    /*Carrier Aggregationi config parameters for release 10 UE*/
    // CarrierAggrConfig carrierAggrConfig;
    CqiConfig cqiConfig;
}ULUESCellContext;
/*CA changes end*/
#endif
#ifdef TDD_CONFIG
typedef enum AckNackModeT
{
    BUNDLING = 0,
    MULTIPLEXING,
    SPL_BUNDLING,
/* + CA_TDD_HARQ_CHANGES */
    CHANNEL_SELECTION
/* - CA_TDD_HARQ_CHANGES */
}AckNackMode;
#endif
/*UEContext for semi-static UE information*/

#ifdef FDD_CONFIG
#define MAX_CSI_MEASUREMENT_PATTERN 5
#else
#define MAX_CSI_MEASUREMENT_PATTERN 9
#endif
typedef struct ueContextForUplinkT {

    ULHarqInfo      ulharqInfo[MAX_HARQ_PROCESS_NB];
#ifdef TDD_CONFIG
    ULHARQEntity *ulHarqEntity_p;
#endif
    UInt16      ueIdx;	 /*indicates the UE ID assigned to this UE*/
    UInt16        crnti; 
    /*indicates the priority of this UE*/
    UInt8       uePriority;        /*LLD ? */

    /* max harq tx changes */
    //UInt8       maxRB;
    UInt8       maxUlHarqTx;
    /* max harq tx changes */

    //UInt8       uePriorityFactor;
    UInt8       modulationSchemeFactor;
    UInt8       modulationScheme;
    /* max no. of resources can be used in one TTI for this UE*/
    //UInt8       maxNbOfRBs;             
    /*indicates the modulation scheme to be used for particular UE*/
    UInt8       mcsIndex;
    UInt8       ServiceType;
    UInt8       rlcMode;
    UInt8 	simultaneousACKNACKAndCQI;
    UInt32      codingrate;
    //UInt8       redundancy;
    ulLogicalChannelInfo	 lcInfo[MAX_NUMBER_OF_LOGICAL_CHANNEL];
    //UInt8		inUseFlag;	/*1 indicates that the current context is in USE*/
    //UInt8   pendingDeleteFlag;/*1 indicates current UE context to be deleted*/
    // UInt8   sRPeriodicity;
    UInt8   numLogicalChannels;
    UInt8   requestType;
    UInt8 srconfigflag;
    UInt8 srSetupflag;
    UInt8 cqiIndication;
    /* + CA_PHASE2 */
    UInt8 cqiinfoflag;
    /* - CA_PHASE2 */
    UInt8 cqiMode;
    /* + CQI_4.1 */
    UInt16 cqiPMIConfigIndex;
    UInt16 cqiPucchResourceIndex;
    UInt16 riConfigIndex;
    UInt16  cqiformatIndicatorPeriodic;
    UInt8 cqiformatIndicatorPeriodic_subband_k;
    UInt8 periodicModeType;
    /* + SPR_8414 */
    UInt8 cqiRequestType;
    /* - SPR_8414 */
    /* - CQI_4.1 */
    UInt16  srPucchResourceIndex;
    UInt8   srConfigurationIndex;
#ifdef TDD_CONFIG
    UInt8 dlAckNackMode;

    UInt8       srPeriodicity;          // Calculated from srConfigIndex, see func calcFirstSRISchedule
    UInt8       srSubframeOffset;
    ULSRISchedule lastSRISchedule;
#endif
    UInt8   bsrPresentFlag;
    UInt8   crntiCEFlag;
    UInt8   phrFlag;
    UInt8   phrIndex;
    CqiInfo cqiInfo;
    UInt32  queueLoad;
    /*SPR 15909 fix start*/
    tickType_t  txTTI;
    /*SPR 15909 fix end*/
    UInt32  tbSize;
    UInt8  taToBeSentFlag;
    UInt8  timingAdvance;
    UInt8  hiReceived;
    //UlControlQueueNode *ulControlNode_p[4];
    //UlDataQueueNode    *ulDataNode_p[4];
    UlControlQueueNode *ulControlNode_p[10];
    UlDataQueueNode    *ulDataNode_p[10];
    // _ULHARQ_RETX_  start  
    PDUInfo    ulDataPduInfo[10];
    // _ULHARQ_RETX_  end  
    UInt8       ulHarqTimerNodeArrrayIndex[10];
    FAPI_ulSrIndicationNode_st *srPduNode_p;
    UInt8     ulSchPduRcvdFlag;
    UInt8    uciSrRcvdFlag;
#ifdef TDD_CONFIG
    /* daiTrackingRegister: contains DAI counters and DAI values received
     *      * during that last non-zero DCI format info received for a group of DL
     *           * subframes whose Ack/Nacks are to be bundled/multiplexed together. */
    /* + CA_TDD_HARQ_CHANGES */
    DaiTrackingRegister daiTrackingRegister[MAX_NUM_CELL][2][MAX_SUBFRAME];
    /* - CA_TDD_HARQ_CHANGES */
    
    FAPI_harqIndQueueNode_st *harqNode_p[MAX_HARQ_CONT][MAX_SUBFRAME];
#endif

    /* SPR 1298 */
    /* SPR 15909 fix start */
    tickType_t latDci0RcvdTick;
    /* SPR 15909 fix end */
    UInt8 ackNackIndex;
    UInt8 sysNum;
    /* + SPR 14341 Changes */
    UInt8 scellAckNackIndex;
    UInt8 scellSysNum;
    /* - SPR 14341 Changes */

    UInt8 crcIndex;
    UInt8 shiftInd;
    /* Additional Context variables to Track SR Sending, BSR Reporting and Grant*/
    UInt32 lastQUpdateTime;
    UInt8  srTriggered;
    UInt8  availGrantCounter;
     /* + CL-MIMO RSM*/
     codebookSubsetRestrictionInfo codeBookSubsetfromRRC;
     /* - CL-MIMO RSM*/
    /* ULA_CHG */
    UInt8 ueProfileId;
    UInt8 srsSeqUsedIdx;
    UInt8 startPosition;
    UInt8 enableHopping;
    UInt8 ulschSinrSeqUsedIdx;
    UInt8 hoppedBw;
    /* ULA_CHG */

    /*crc changes*/
    void *crcIndRefNode_p[QSIZE];
    /*crc changes*/
/* SPR 11713 START */
    UInt8 rowSCellCounter;
    UInt8 columnSCellCounter;
/* SPR 11713 END */
/* SPR 2234 Fix Begin*/
#ifdef FDD_CONFIG
    UInt8 rowCounter;
    UInt8 columnCounter;
#endif
    /* SPR 2234 Fix End*/
    /* SPR 2333 Fix Begin*/
    void *ulschIndRefNode_p[QSIZE];
    /* SPR 2333 Fix End*/

    /* HO reset support */
    UInt8 resetFlag;
    /* HO reset */
    /*CLPC_CHG*/
     /* SPR 15909 fix start */
    tickType_t phrTime;
     /* SPR 15909 fix end */
    UInt8 cqiSinrSeqUsedIdx;
    /*Configuration information of TPC for PUSCH*/
    TpcPdcchConfig tpcPdcchConfigPusch;
    /*Contains information elements of dedicated uplink power control.*/
    DedicatedULPowerControlInfo dedicatedULPowerControlInfo;
    SInt8 tpcPUSCH;
    SInt8 pcMax;
    UInt8 isDedicatedULPowerControlInfoPresent;
    UInt8 isTpcPdcchConfigPuschPresent;
    /* The current power control offset from initial power
     *      * that is indicated to the UE for PUSCH */
    SInt8 currPowerCorrectionForPusch;

    /* The previous power control offset from initial power
     *      * that was indicated to the UE for PUSCH */
    SInt8 prevPowerCorrectionForPusch;

    /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
    UInt8 isHDFddFlag;
    UInt8 scheduleMap[MAX_SUBFRAME];
#endif
    /* HD FDD Changes End */
    /*CLPC_CHG*/
     /*SPS_CHG*/
     uint8_t     ulNdi;
     LP_MacSpsOccList   occListNode_p;
     LP_MacSpsOccList   occListNode20_p;
     /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
     /* used for 3rd node, Periodicity 40 and 20 */ 
     LP_MacSpsOccList   occListNode40_p;
     /* used for 4th node, Periodicity 20*/ 
     LP_MacSpsOccList   occListNode60_p;
#endif
     /* - SPS_TDD_Changes */
     UInt8     isSpsOccasion;  
     UInt16    ttiIndex;
     UInt16    ulEmptyTx;
     UInt8 isSpsUlInfoPresent;
     /* SPS Configuration Information for Uplink */
     spsUlInfo spsUplinkInfo;
     SpsOccUeContext spsOccCtx;
     /*SPS_CHG*/
     UInt8     ueCategory;  
     /*TTI Bundling Start*/

#ifdef FDD_CONFIG
    /** For TTIB_Code */
     /* SPR 15909 fix start */
     tickType_t chkNDI[MAX_TTIB_HARQ_PROCESS_NB];
     /* SPR 15909 fix end */
     TTIBundlingInfo ttibInfo;
#endif

     UInt8 ttiBundling;
     /* + TM7_8 Changes Start */
     UInt8 cqiMaskV920;
     UInt8 pmiRiReportV920;
     /* - TM7_8 Changes End */
	 /* + CA_PHASE2 changes start */
	 UInt8     trigger1r10;
	 UInt8     trigger2r10;
	 UInt8 ueCategoryV1020;
	 UInt8 ueCategoryReqType;
	 PucchFormatR10 pucchFormatR10;
	 CarrierAggrConfig carrierAggrConfig;
	 /* - CA_PHASE2 changes end*/

    /* + CA_TDD_HARQ_CHANGES */
#ifdef TDD_CONFIG
     AckNackMode ackNackMode;
#endif
    /* - CA_TDD_HARQ_CHANGES */
     /*TTI Bundling End*/
#if 0
       /*CA changes start*/
       /*primary cell index*/
       UInt8 cellIndex;
       /*Count of Scells configured for this UE*/
       UInt8 scellCount;
       /*Secondary Cell contexts for each of the SCELL configured for the UE*/
       ULUESCellContext *ulUeScellContext_p[MAX_NUM_SCELL];
       /*CA changes end*/
#endif
	/* EICIC +*/
     UInt8 cqiConfigRequestType;
     UInt16 cqiPMIConfigIndex_2;
     UInt16 riConfigIndex_2;
     UInt8 cqiSubsetConfigRequestType;
     UInt8 csiMeasurementSubset1[NUM_BITS_ABS_PATTERN];
     UInt8 csiMeasurementSubset2[NUM_BITS_ABS_PATTERN];
     UInt8 ulIMcs;
     UInt8 srProhibitTimerR13;
     UInt8 periodicBsrTimerR13;
     UInt8 retxBsrTimerR13;
     UInt8 ackNackNumRepetitionR13;
     UInt8 nPuschAllSymbolsR13;
     UInt8 groupHoppingDisabledR13;
     UInt8 p0UeNpuschR13;

	/* EICIC -*/
}ueContextForUplink;
/*structure of the dynamic UE information to be stored and owned by scheduler*/
typedef struct dynamicUEContextT {
#ifdef FDD_CONFIG
    ULHarqInfo      ulresInfo[MAX_HARQ_PROCESS_NB]; /*Resource allocation
                                                     *information of the last 8 TTIs*/
#endif                                                     
    UInt32  bsrNet[NUM_OF_LCG];  /*BSR information of the 4 logical channel groups*/
    UInt8   lcCount[NUM_OF_LCG];    /* Indicates number of LCs configured for LCG */
    UInt8   cqiInd;                 /*CQI flag indicating CQI required or not (0 or 1) */
    UInt8   inUseFlag;  /*1 indicates that the current context is in USE*/
    UInt8   pendingToDeleteFlag; /*1 indicates current UE context to be deleted*/
     /* SPR 15909 fix start */
    tickType_t   lastScheduledTick;      /*TTI in which a UE was last scheduled*/
     /* SPR 15909 fix end */
    UInt32  bsrNetTotal;        /*Sum of the BSR of all the 4 logical channel groups */
}dynamicUEContext;

#if 0

/*struct carrying information for scheduler thread. 	*/
typedef struct schedulerThreadInfoT{
	UInt64 		tid; /*thread id of the thread */
	SInt8		containerNumber;/*container number on which scheduler has to work.*/
	UInt32		*container_p; /*pointer to container on which scheduler has to work*/
} schedulerThreadInfo;


/* structure carrying pending Delete indexes */
typedef struct pendingDeleteNodeT {
    LTE_SNODE  nodeAnchor;
    UInt16     idx;
    UInt16     transactionId;
} PendingDeleteNode;



/****************************************************************************
 * Exported Constants
 ****************************************************************************/



/****************************************************************************
 * Exported Variables
 ****************************************************************************/



/****************************************************************************
 * Exported Functions
 ****************************************************************************/

void *initControllerThread (void *);

UInt16 fetchTempRntiFromPool ();

UInt32 fetchBufferSizeFromIndex (UInt16 index);

UInt16 getUeIdFromUeIdMap (UInt16 rnti);

void freeTempRntiFromPool (UInt16 rnti);

ueContextForUplink *fetchUplinkUeContextInfo(UInt16 ueIdx);

#endif


#endif		/*	LTE_UPLINK_UECONTEXT_H	*/
