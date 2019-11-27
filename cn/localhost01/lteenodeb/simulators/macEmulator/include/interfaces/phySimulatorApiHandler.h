/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: phySimulatorApiHandler.h,v 1.3.6.1.4.2.2.1 2010/09/21 15:53:26 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : Physical simulator Header File
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
*/

#ifndef LTE_API_HANDLER_H
#define LTE_API_HANDLER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/msg.h>

#include "lteLayer2Types.h"
#include "lteLayer2UplinkUEContext.h"
//#include "lteLayer2UplinkScheduler.h"
#include "lteLayer2UEContext.h"
//#include "lteLayer2UplinkDeMultiplexer.h"
#include "lteLayer2ParseUtil.h"
#include "lteLayer2MacPhyInterface.h"

#include "lteFapiMacPhyInterface.h"

#ifdef TDD_CONFIG
#include "lteLayer2TDDMgr.h"
#endif
/* +CA_NA */
#include "lteCircQueue.h"
/* -CA_NA */

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Project Definitions
 ****************************************************************************/
#define RECV_BUFF_SIZE 64000
#define UPLK_PORT 9999
#define PHY_PORT  8888
#define SH_MESSAGE_BUF_SIZE (10000*2)
#define SHM_MSG_DL_ID 0x12345678
#define SHM_MSG_UL_ID 0x12345679
#define CONTROL_API_BUFFER_SIZE 9000
#define DATA_API_BUFFER_SIZE 9000
#define PEER_MAC  0x01
//#define MAC_EMU_PHY 0x03
#define MAC_EMU_PHY 0x06
#define SFN_SF_LEN 3
#define MAC_API_HEADER_LENGTH 10 
#define REG_SUCCESS 1
#define REG_FAILURE 0

#define HARQ_FEEDBACK_FILE_PATH "../cfg/AckNackConfig.cfg"
/*FAPI_ERROR_INDICATION_CHANGES_START*/
#define PHY_ERROR_CONFIG_FILE_PATH "../cfg/PhyErrorConfig.cfg"
#define PHY_ERR_MAX_LINE_SIZE 250
#define ABS(a)   (((a) < 0) ? -(a) : (a))
/*FAPI_ERROR_INDICATION_CHANGES_END*/

#ifdef FDD_CONFIG
#ifdef UE_SIM_SPLIT

#define PEER_MAC_DELAY 0
#define PEER_MAC_ADVANCE 2

#else
#define PEER_MAC_DELAY 2
/* SPR */
#define PEER_MAC_ADVANCE 1
#endif

#endif


#define MAX_PREAMBLE_RANGE 64
#define MAX_RA_RNTI_RANGE 60
#define START_T_CRNTI 61
#define MAX_CRNTI_RANGE 0xFFF3 
//This macro move into lteLayer2TDDMgr.h #define PEER_MAC_DELAY 0
#define MAX_NO_UE 100
/* SPR 1864 Start */
/* Warning removed Macro INVALID_UE_INDEX removed*/
/* SPR 1864 End */
#define TOTALPACKETS 10
#define MAX_LINE_SIZE 200

#ifdef FDD_CONFIG
/** For TTIB_Code */
#define TTI_BUNDLE_RTT 16
#define TTIB_INVALID_VALUE 255
#endif

/* + CL-MIMO LJA*/
#define MAX_SUB_BAND 28
/* - CL-MIMO LJA*/
#define DIFF_HARQ_SUBFRAME 2
#define NEW_TX 1
#define RE_TX 0
#define MAX_CCE 200
#define MAX_MSG_ID 100
#define SF_DIFF 3
#define MAX_SF_DIFF 2
#define MAX_CCE_INDEX 100

#define MAC_REGISTER_REQ 0x01
#define MAC_DL_CONTROL_MSG_REQ 0x02
#define MAC_DL_DATA_MSG_REQ 0x03
#define MAC_REGISTER_CNF 51
 
#define MAC_SUBFRAME_START_IND 101
#define MAC_UL_CONTROL_MSG_IND 102
#define MAC_UL_DATA_MSG_IND 103 
#define MAC_UL_SUBFRAME_CONFIG_IND 104
#define MAC_ERROR_IND 105
#define APERIODIC_PDU 0x1C

#define NUM_OF_PRB 100   /* For RA Policy */
#define HARQ_ACK 1
#define CRC_CORRECT 0
#define CRC_ERROR 1
#define CRC_INVALID 2
#define MAX_NUM_CRNTI 10
#define MAX_NUM_PREAMBLE 10
#define MSG3_WITH_ONLY_CCCH_PDU_LEN   MAX_CCCH_SDU+1
#define MSG3_WITHOUT_CCCH_PDU_LEN     3
#define MSG3_WITH_CCCH_PDU_LEN        MSG3_WITH_ONLY_CCCH_PDU_LEN+MSG3_WITHOUT_CCCH_PDU_LEN+1
//#define WAIT_RAR 0x01
//#define WAIT_CONTENTION_RESOLUTION 0x02
#define CONTROL_FLAG 0x01
#define APERIODIC_MODE 0
#define PERIODIC_MODE 1

#define HARQ_TB_INFO_TAG_LEN 5
#define SR_HARQ_PDU_INFO_TAG_LEN (4 + 2*HARQ_TB_INFO_TAG_LEN)
#define CRC_HARQ_PDU_INFO_TAG_LEN (5 + 2*HARQ_TB_INFO_TAG_LEN) 
#define SR_PDU_INFO_TAG_LEN 4
#define HARQ_PDU_INFO_TAG_LEN  (4 + 2*HARQ_TB_INFO_TAG_LEN)
#define CRC_PDU_INFO_TAG_LEN 5
#define RACH_PDU_INFO_TAG_LEN 4
#define TIMING_ADVANCE_PDU_INFO_TAG_LEN 5
#define ULSCH_PDU_TAG_LEN 4
#define RA_INFO_LENGTH 7
#define MAC_DLSCH_PDU_INFO_TAG_LEN 4
#define MAC_PCH_PDU_INFO_TAG_LEN 4 
#define MAC_BCH_PDU_INFO_TAG_LEN 6
#define MAC_HI_PDU_INFO_TAG_LEN 8
#define UCI_CQI_PDU_TAG_LEN 5
#define ULSCH_CQI_RI_PDU_TAG_LEN 5

#define DCI_FORMAT_0_TAG_LEN   0x0d
#define DCI_FORMAT_1_TAG_LEN   0x12
#define DCI_FORMAT_1A_TAG_LEN  0x14
#define DCI_FORMAT_1B_TAG_LEN  0x14
#define DCI_FORMAT_1D_TAG_LEN  0x14
#define MAC_TBINFO_TAG_LEN     0x0b
#define DCI_FORMAT_2_TAG_LEN   2*MAC_TBINFO_TAG_LEN + 0x0e
#define DCI_FORMAT_2A_TAG_LEN  2*MAC_TBINFO_TAG_LEN + 0x0e
 

#define MAX_SIMAP_TABLE        2

/*512 is the maximum periodicity of the SI messages*/
#define MAX_SIMAP_TABLE_NODES  512
#define MAX_SI_REPETITIONS  1
#define TOTAL_NUM_PERIODICITY 11

#define SUB_BAND 1
#define SUB_BAND_CQI 2
#define WBAND_PMI 3
/* TA_CHG */
#define TIMING_ADVANCE_VALUE_RACH           300 
#define TIMING_ADVANCE_VALUE_ULSCH          305 
#define TIMING_ADVANCE_VALUE_SRS            306 /* SRS_TA */
/* TA_CHG */

/* Macros for TDD */
#define MAX_RADIO_FRAME_FOR_DL_ACK_NACK  8
#define MAX_NO_TB                        2
#define TB1_INDEX                       0
#define TB2_INDEX                       1

#define AGGREGATION_LEVEL_ONE 1
#define AGGREGATION_LEVEL_TWO 2
#define AGGREGATION_LEVEL_FOUR 4
#define AGGREGATION_LEVEL_EIGHT 8

#define MAC_TDD_UL_DL_CONFIGURATION_0   0
#define MAC_TDD_UL_DL_CONFIGURATION_1   1
#define MAC_TDD_UL_DL_CONFIGURATION_2   2
#define MAC_TDD_UL_DL_CONFIGURATION_3   3
#define MAC_TDD_UL_DL_CONFIGURATION_4   4
#define MAC_TDD_UL_DL_CONFIGURATION_5   5
#define MAC_TDD_UL_DL_CONFIGURATION_6   6

#define MAX_UL_DL_CONFIG                7

#define MAX_CODE_BOOK_INDEX 25
#define MAX_BF_VECTOR        1
#define MAX_NO_OF_ANTENNAS   4

#define MAX_PDU  80

#define     ZERO_OF_EIGHT_DL_NACK   0
#define     ONE_OF_EIGHT_DL_NACK    1
#define     TWO_OF_EIGHT_DL_NACK    2
#define     THREE_OF_EIGHT_DL_NACK  3
#define     FOUR_OF_EIGHT_DL_NACK   4
#define     FIVE_OF_EIGHT_DL_NACK   5
#define     SIX_OF_EIGHT_DL_NACK    6
#define     SEVEN_OF_EIGHT_DL_NACK  7
#define     ALL_DL_NACKS            8
/* HD FDD Changes Start */
#define SUBFRAME_DIFF_UE_ENB  4 
/* HD FDD Changes End */
/* SPR 11588 Changes Start */
#define MAX_ARR_SIZE 64
/* SPR 11588 Changes End */
/*multi UE/TTi fix start*/
#define MAX_UE_PER_TICK 32
/*multi UE/TTi fix end*/
enum
{
    MSG_OK,
    MSG_DECODE_ERROR,
    MSG_TOO_LATE
};
enum
{
   PERIODICITY_2 = 2,
   PERIODICITY_5 = 5,
   PERIODICITY_10 = 10,
   PERIODICITY_20 = 20,
   PERIODICITY_32 = 32,
   PERIODICITY_40 = 40,
   PERIODICITY_64 = 64,
   PERIODICITY_80 = 80,
   PERIODICITY_128 = 128,
   PERIODICITY_160 = 160,
   PERIODICITY_256 = 256
};

typedef enum DCIFormatT
{
    DCI_FORMAT_0 = 0,
    DCI_FORMAT_1, 
    DCI_FORMAT_1A,
    DCI_FORMAT_1B,
    DCI_FORMAT_1C,
    DCI_FORMAT_1D, 
    DCI_FORMAT_2,
    DCI_FORMAT_2A,
    DCI_FORMAT_3, 
    DCI_FORMAT_3A,
    /* + TM7_8 Changes Start */
    DCI_FORMAT_2B,
    /* - TM7_8 Changes End */
    MAX_NUM_DCI_FORMAT
} DCIFormat;

#define CCCH_LCID       0
#define LCID_0          0
#define LCID_1          1
#define LCID_2          2
#define LCID_3          3
#define LCID_4          4
#define LCID_5          5
#define LCID_6          6
#define LCID_7          7
#define LCID_8          8
#define LCID_9          9
#define LCID_10         10
#define LCID_11         11
#define LCID_12         12
#define LCID_13         13
#define LCID_14         14
#define LCID_15         15
#define LCID_16         16
#define LCID_17         17
#define LCID_18         18
#define LCID_19         19
#define LCID_20         20
#define LCID_21         21
#define LCID_22         22
#define LCID_23         23
#define LCID_24         24
#define LCID_25         25
#define LCID_26         26
#define LCID_27         27
#define LCID_28         28
#define LCID_31         31
#define MAC_CE_LCID     30
/* TA_CHG */
#define TA_LCID           29
/* TA_CHG */
/*ca phase-3 changes*/
#define SCELL_LCID      27
#define SCELL_ACT_DEACT_MASK 0x02
/*ca phase-3 changes*/
#define PADDING_LCH     31
#define UL_PDU_EXTN_FLAG          0x20
#if 0
typedef enum PHYTagIndentifierT
{

    MAC_DCI_PDU_INFO = 0x01,
    MAC_HI_PDU_INFO,              
    DCI_0,        
    DCI_1,                
    DCI_1A,               
    DCI_1B,              
    DCI_1C,               
    DCI_1D,               
    DCI_2,                 
    DCI_2A,
    MAC_TB_INFO,                  
    MAC_BCH_PDU_INFO,             
    MAC_DLSCH_PDU_INFO,           
    MAC_PCH_PDU_INFO,             
    SR_HARQ_PDU_INFO,             
    CRC_HARQ_PDU_INFO,            
    CRC_PDU_INFO,                 
    SR_PDU_INFO,                  
    HARQ_PDU_INFO,                
    HARQ_TB_PDU_INFO,             
    RACH_PDU_INFO,                
    ULSCH_PDU,                    
    ULSCH_CQI_RI_PDU,             
    UCI_CQI_PDU,                      
    TIMING_ADVANCE_PDU_INFO,      
    DL_ERROR_INFO,                
    UL_ERROR_INFO,
    APERIODIC_CQI_MODE_2_0,
    APERIODIC_CQI_MODE_3_0,
    APERIODIC_CQI_MODE_3_1,
    APERIODIC_CQI_MODE_1_2,
    APERIODIC_CQI_MODE_2_2,
    PERIODIC_CQI_MODE_1_0_TYPE_3,
    PERIODIC_CQI_MODE_1_0_TYPE_4,
    PERIODIC_CQI_MODE_1_1_TYPE_2,
    PERIODIC_CQI_MODE_1_1_TYPE_3,
    PERIODIC_CQI_MODE_2_0_TYPE_1,
    PERIODIC_CQI_MODE_2_0_TYPE_3,
    PERIODIC_CQI_MODE_2_0_TYPE_4,
    PERIODIC_CQI_MODE_2_1_TYPE_1,
    PERIODIC_CQI_MODE_2_1_TYPE_2,
    PERIODIC_CQI_MODE_2_1_TYPE_3

}PHYTagIndentifier;
#endif

/*Valid SI Periodicity values*/
typedef enum TransMissionModeT
{
    SINGLE_ANTENNA_PORT_0 = 1,
    TX_DIVERSITY,
    OPEN_LOOP_SPATIAL_MULTIPLEXING,
    CLOSED_LOOP_SPATIAL_MULTIPLEXING,
    MULTI_USER_MIMO,
    CLOSED_LOOP_RANK_1_PRECODING,
    SINGLE_ANTENNA_PORT_5,
    TX_MODE_INVALID
}TransMissionMode;
typedef enum SIPeriodicityT
{
    SI_PERIODICITY_8   = 8,
    SI_PERIODICITY_16  = 16,
    SI_PERIODICITY_32  = 32,
    SI_PERIODICITY_64  = 64,
    SI_PERIODICITY_128 = 128,
    SI_PERIODICITY_256 = 256,
    SI_PERIODICITY_512 = 512,
    
    /*Add new entry above MAX_SI_PERIODICITY and 
     *set MAX_SI_PERIODICITY to max value of the 
     *above filelds
     */    
    MAX_SI_PERIODICITY = SI_PERIODICITY_512
}SIPeriodicity;
typedef enum SIMsgTypeT
{
    INVALID_SI_MSG = 0,
    SI_MSG_1,
    SI_MSG_2,
    SI_MSG_3,
    SI_MSG_4,
    SI_MSG_5,
    SI_MSG_6,
    SI_MSG_7,
    SI_MSG_8,
    SI_MSG_9,
    SI_MSG_10,
    MAX_SI_MSG = SI_MSG_10
}SIMsgType;
/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
/* Enumeration for Different bandwidth supported */
typedef enum  DlBandwidthT
{
    DL_BW_1DOT4_MHZ = 0,
    DL_BW_3_MHZ,
    DL_BW_5_MHZ,
    DL_BW_10_MHZ,
    DL_BW_15_MHZ,
    DL_BW_20_MHZ,
    MAX_DL_BW
}DlBandwidth;
typedef enum DlResBlksT
{
    DL_RES_BLKS_6RBS = 6,
    DL_RES_BLKS_15RBS = 15,
    DL_RES_BLKS_25RBS = 25,
    DL_RES_BLKS_50RBS = 50,
    DL_RES_BLKS_75RBS = 75,
    DL_RES_BLKS_100RBS = 100
}DlResBlks;
UInt16 calcBinomialCoef(UInt8 tempVar1,UInt8 tempVar2);
#endif
/* SPR 21958 PUSCH RAT1 Support End */
/* +CA_NA */
/* CA_START_UP_PHASE_2 */
typedef struct phyDlDataQT {
    LTE_CIRCQUEUE DlDataCircQ[MAX_NUM_CELLS];
}phyDlDataQ;

phyDlDataQ dlSchPduQ[MAX_NUM_CELLS];
/*SPR 12832 fix added tb1Valid, tb2Valid*/ 
typedef struct dlSchPduInfoT {
    UInt8                    tb1Valid;
    UInt8                    tb2Valid;
    UInt16               dlSCHPduCount;
    UInt16                   rnti;
    FAPI_rntiType_en         rntiType;
    FAPI_dlDCIFormatInfo_en  dciFormat;

}dlSchPduInfo;
dlSchPduInfo scellDlSchInfo[MAX_NUM_CELLS][MAX_NUM_CELLS][MAX_NUM_UE];
/* CA_START_UP_PHASE_2 */


typedef struct dlConfigQT{
    LTE_CIRCQUEUE DlConfigCircQ[MAX_NUM_CELLS];
}dlConfigQ;

dlConfigQ dlConfigData[MAX_NUM_CELLS];
/* -CA_NA */

typedef struct shMsgBuf {
         UInt32   mtype;
         UInt8    mtext[SH_MESSAGE_BUF_SIZE];
         } shMessageQBuf;

typedef struct MsgIdStructT {
         UInt8 msgId;
         RNTIType rntiType; // it can be {PRNTI, SIRNTI, RA-RNTI, T-CRNTI}
         UInt8 dataExpectingCounter;
         UInt16 crnti;
	     UInt16 dciFormatType;
         UInt32 tbSize;
         UInt8 numberOfTB; 
} MsgIdStruct;
typedef struct PeriodicCQITableT
{
    UInt32 periodicity;
    UInt32 numOfUEs;
    UInt16 ueId[MAX_NO_UE];
}PeriodicCQITable;
#pragma pack(push, enter_packing,1)
typedef struct RAInfoT
{
    UInt8  preamble; // used only 5 bits
    UInt8  hoppingBit; //(0 - 1)
    UInt8  tmcs; //( 0 -10)
    UInt8  tpc;  //(0 -10)
    UInt8  ulDelay; //( 0 - 10)
    UInt16 timingAdvance;
}RAInfo;
#pragma pack(pop,enter_packing)



typedef struct PreambleLookUpT{
	UInt8     preamble;
        UInt8     raReqCount; 
        RARequest raReq[MAX_NUM_PREAMBLE];
}PreambleLookUp;
typedef struct TAInfoT {
       UInt16 taValue;
       UInt16 ueId;
}TAInfo;
typedef struct TAContext {
       UInt16 numOfUe;
       TAInfo taInfoArr[MAX_NUM_UE];
}TAContext;

typedef struct queuePointerNodeT {
         LTE_SQUEUE controlQueue;
         LTE_SQUEUE dataQueue;
}QueuePointerNode;

typedef struct CQIInfoT {
	UInt16 ulCqiPdu;
	UInt16 length;
	UInt16 cqiMode;
        UInt16 crnti;
}CQIInfo;
typedef struct ProcessRecvQueueNodeT
{
   LTE_SNODE   processRecvAnchor;  
   UInt16      systemFrameNum;
   UInt8       subFrame;
   UInt8       qIndex;
   UInt16      crnti;
   UInt32      tbSize;
   UInt8       crc;
   UInt8       cqiRequest;
   UInt8       harqIndex;
   RNTIInfo    *rntiInfo_p;
}ProcessRecvQueueNode;

typedef struct statsGeneralT
{
    UInt32 statsRaResponsePending;
    UInt32 statsContentionResUnSuccess;
    UInt32 statsContentionResSuccess;
    //UInt32 statsCommonChannel;
    UInt32 statsPaging;
    UInt32 statsBroadcast;
    UInt32 statsMIB;  /* Maintaining a separate counter for MIB */
    /* Maintain counter of success and failure cases for SFN synchronization */
    UInt32 statsSuccessSFNSynch;
    UInt32 statsUnSuccessSFNSynch;
} statsGeneral;


typedef struct SIMsgT
{
    void *msg_p;
    UInt8 msgLen;
    UInt8 numRB;
}SIMsg;
#define MAX_SI_MSG 512
typedef struct SIMsgInfoT
{
    /*Contains the SI message information*/
    SIMsg siMsgBuff[MAX_SI_MSG];

    /*Nummber of SI messages in siMsgBuff[] array*/
    UInt8 numSImsg;
}SIMsgInfo;

typedef struct SubFrameMapT
{
    /*If this flag is set schedule the SI message*/
    UInt8 sendSIMsgFlag;

    /*Type of SI message to be scheduled*/
    SIMsgType siMsgType;
}SubFrameMap;

typedef struct SIMapTableT
{
    /*This contains SI message scheduling info for 10 subframes*/
    SubFrameMap subFrame[MAX_SUBFRAME];
}SIMapTable;

typedef struct SIInfoT
{
    /*siMsg[] array current index inuse*/
    UInt8       siMsgActiveIndex;

    /*siMapTable[] array current index inuse*/
    UInt8       siMapActiveIndex;

    /*siMsgUpdateFlag and siMapUpdateFlag is set when a new set of 
     *SI messagese received from RRC and new Map table created
     */
    UInt8       siMsgUpdateFlag;
    UInt8       siMapUpdateFlag;

    /*SFN from which new SI messages received is to be applied*/
    UInt16      sfn;
    
    SIMsgInfo   siMsg[2];
    SIMapTable  siMapTable[MAX_SIMAP_TABLE][MAX_SIMAP_TABLE_NODES];
}SIInfo;
/*This structure is used to temprarly store the decoded SI messages
 *when received from RRC 
 */
typedef struct SIMsgInfoFromRRCT
{
    UInt8  startSubFrame;
    UInt8  msgLen;
    UInt16 siPeriodicity;
    UInt16 startSFN;
    void   *msg_p;    
    UInt8  numRB;    
}SIMsgInfoFromRRC;


#define MAX_DISTRIBUTED_VRB 100
#define MAX_SUBSETS 4
#define GAP_VALUE_FOR_RA_DISTRIBUTED 1

typedef struct RAT0InfoT
{
    UInt32 rbgSize;    
    UInt32 rbgCount;
}RAT0Info;

typedef struct SubsetInfoT
{
    UInt8 spanWidth;
    UInt8 maxAddressableRBCount;
}SubsetInfo;

typedef struct RAT1InfoT
{
     /*This is array of SubsetInfo Structure which stores information of free 
      * RBs for a Subset*/
     SubsetInfo subsetInfo[MAX_SUBSETS]; 
     UInt8 subsetCount;
     UInt8 maxAddressableRBCount;
} RAT1Info;



typedef struct RAT2DistributedVRBInfoT
{
    /*Below will store  the PRB number which is mapped to Even slot of the VRB*/
    UInt8 prbMappedToEvenSlotOfVRB ;
    /*Below will store  the PRB number which is mapped to Odd slot of the VRB*/
    UInt8 prbMappedToOddSlotOfVRB;   
} RAT2DistributedVRBInfo;

typedef struct RAT2DistributedInfoT
{
    /*This ia array of RAT2DistributedVRBInfo structure which stores information
     * of Free Localized VRB*/
     RAT2DistributedVRBInfo rat2DistributedVRBInfo[MAX_DISTRIBUTED_VRB + 3];
         UInt8 maxDistributedVRB;  
}RAT2DistributedInfo;

typedef struct ResourceAllocationPolicyInfoT
{
    /* This  stores the information of  free RBG in case of Resource Allocation
     *  Policy 0.Each set bit  position represent the free RBG position */
    RAT0Info rat0Info;
    /* This is  structure  which stores the  the information of 
    RBs ofSubset of RAT 1 policy  and each index mapped to subset Index */
    RAT1Info rat1Info;
    /* This represents a structure   which stores information of Distributed VRB
     *  in case of RAT 2 Distributed */
    RAT2DistributedInfo rat2DistributedInfo;    
    
}ResourceAllocationPolicyInfo;

//Pradeep
typedef struct DemuxDLDataBaseT
{
    UInt8  lchId; /*Logical channel ID*/
    UInt32 length; /*Data length for this logical channel id*/
}DemuxDLDataBase;


typedef struct ULHarqTimerNodeT
{
    UInt16 crnti;
    UInt16 ueIndex;
    UInt16 sfn;
    UInt8   isValid;
    UInt8   harqProcessId; 
    UInt8   qIndex;	
    UInt8 	sf;
}ULHarqTimerNode;

typedef struct ULHarqTimerEntityListT
{
    /* The count will contain the number of valid field in the array*/
    UInt16 count;
    /* This array will store the harqTimerNode */       
    ULHarqTimerNode ulHarqTimerNodeArr[100];
}ULHarqTimerEntityList;

/* this array will store the harqs of UE on per subframe basis.*/

ResourceAllocationPolicyInfo raPolicyInfo_g;

#define SPLIT_ZERO_BUFFER_COPY(org_p, dst_p, splitPos)\
{\
    dst_p = msgAlloc(PNULL, 0, 0, 0);\
    msgSplit((void *)org_p, splitPos, (void *)dst_p);\
}

extern RNTIInfo *rntiToIdxTbl_g[MAX_NUM_CELLS];
extern UInt8 cellConfigured_g[MAX_NUM_CELLS];
extern UInt8 cellConfigRecvFlag[MAX_NUM_CELLS];
extern FAPI_phyStates_en phyState_g[MAX_NUM_CELLS];
extern UInt16   sfnsf_g[MAX_NUM_CELLS];
extern volatile UInt16 SFN_g[MAX_NUM_CELLS];
extern volatile UInt8  SF_g[MAX_NUM_CELLS];

extern UInt16 lowerCRNTIBound_g;
extern UChar8 l2ModuleIP[HOSTNAME_MAX_LEN];
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
//extern UInt8 initMAC( UInt16 *rxenbMacPort,UInt16 *txMacPort,UInt16 rxOamMacEmuPort,
        /* SPR 5313 changes start */
  //      UInt16 txOamPort, UInt16 txRRCPort, UInt16 rxenbMacPortForHo, UInt8 cellIndex);
        /* SPR 5313 changes end */
void getNackPos(UInt16  totalPackets, UInt8  nackSend, UInt8 *arr_p);
     /* SPR 15909 fix start */
tickType_t getCurrentglobaltick();
     /* SPR 15909 fix end */
extern MacRetType distributeRBsAndSendReqToRLC ( UInt16 ueIndex,
                          						 UInt32 tbsize,
					                             UInt8 bsrPresentFlag,
                                                 void *uldataNode_p,
					                             UInt8 **dataPtr_p,
						                         UInt16 * dataLen_p,
                                                 UInt8 phrTriggered
#ifdef FDD_CONFIG /* For TTIB_Code */
                                                 ,TransmissionAndRLCData **transmissionBuffer_p
#endif
                                                 ,UInt8 cellIndex );
extern UInt32 determineTBSize(UInt8 MCSIndex, UInt8 numRb);

extern void createSIMapTable(SIMsgInfoFromRRC *siMsgInfoReceived_p,
                             UInt32 numSIMsg,
                             UInt32 siWindowSize);

extern void cleanSIMsg(SIInfo *siMsgInfo_p, 
                       UInt32 siMsgActiveIndex,
                       UInt32 siMapActiveIndex);
extern void prepareSduSubHeader(UInt8** sendBufer_p,UInt8 R1,
        UInt8 R2, UInt8 E, UInt8 lcId,UInt8 format, UInt32 length);
extern UInt8 getBSRIdxfrmBytes( UInt32 bytes );
extern void prepareLongBSR(dynamicUEContext *ulDynUEContext_p, UInt16 crnti,
                            UInt8 sf, UInt16 sfn, UInt8 qIndex);
extern UInt32 rlcInitDbReq();
extern void tickIndicationToRLC();
extern UInt8* calculateSubHeaderLength(UInt8 *headerPtr_p,
                                        UInt32 *len_p,
                                        UInt32 *dataPtrPos_p);

extern void dlDataDeMultiplexAndSend(DemuxDLDataBase *demuxData_p,
                                     UInt8   *origDataPtr_p,
                                     UInt8   *dataPtr_p,
                                     UInt32  pos,
                                     UInt32  dataPtrPos,
                                     UInt16  ueIndex,
                                     UInt8 pduType,
                                     UInt8 txCellIndex);

extern UInt32 processDLDemuxData( UInt8  *data_p,
                           void   *zeroBufCpy_p,
                           UInt32 dataLen,
                           UInt16 ueIndex,
                           UInt8 txCellIndex);
extern void prepareRAInfo(UInt8 raReqIndex);
extern void pushEntryInOnlyBsrQueue(  UInt8  *bsrPdu_p,
                               UInt16 bsrPduLen,
                               UInt16 crnti,
                               UInt8  sendSF,
                               UInt16 sendSFN,
                               UInt8  qIndex,
                              UInt16 pduChoice,
                               UInt8  cqiMode );


typedef struct
{
    LTE_SNODE  anchor;
    SInt32      len;
    void      *msg;
} macPhyDataNode;

typedef enum TBValidFlagT
{
    IstTB_VALID_2ndTB_INVALID = 1,
    IstTB_INVALID_2ndTB_VALID,
    BOTH_TB_VALID,
    SISO_CASE

}TBValidFlag;

typedef struct FAPI_dciConfig_stT
{
    UInt8                    tb1Valid;
    UInt8                    tb2Valid;
    UInt16                   rnti;
    FAPI_dlDCIFormatInfo_en  dciFormat;
}FAPI_dciConfig_st;

typedef struct FAPI_dlschConfig_stT
{
    UInt8                    tb1Valid;
    UInt8                    tb2Valid;
    UInt16                   pduCount;
    UInt16                   rnti;
    UInt16                   dlschPduLen;
    FAPI_dlDCIFormatInfo_en  dciFormat;
    FAPI_rntiType_en         rntiType;

}FAPI_dlschConfig_st;

typedef struct FAPI_bchConfig_stT
{
    UInt16 bchPduLen;
    FAPI_rntiType_en         rntiType;

}FAPI_bchConfig_st;
typedef struct FAPI_pchConfig_stT
{
    UInt16                   pchrnti;
    UInt16                   pchPduLen;
    FAPI_dlDCIFormatInfo_en  dciFormat;
    FAPI_rntiType_en         rntiType;

}FAPI_pchConfig_st;
/*  
typedef struct FAPI_prsPduConfigInfo_stT
{
    UInt16 transmissionPower; 
}FAPI_prsPduConfigInfo_st;*/

#ifdef LTE_EMBMS_SUPPORTED
typedef struct FAPI_mchPduConfigInfo_stT
{
    UInt8                    mchAreaId;
    UInt16                   mchrnti;
    UInt16                   mchPduLen;
    FAPI_rntiType_en         rntiType;
}FAPI_mchPduConfigInfo_st;
#endif

/*+ Dyn UEs To Be SchPerTTI +*/
#define FAPI_MAX_DCI_PDU 20
/*- Dyn UEs To Be SchPerTTI -*/
typedef struct fapiMsgIdStructT 
{
    UInt16               recvSfnSf;
    UInt16               pchPduCount;
    UInt16               bchPduCount;
    UInt16               dlSCHPduCount;
    UInt16               dciPduCount;
#ifdef LTE_EMBMS_SUPPORTED
    /* +MBMS_CHANGE */
    UInt16               mchPduCount;
    /* -MBMS_CHANGE */
#endif    
    FAPI_pchConfig_st    pch;
    FAPI_bchConfig_st    bchPdu;
    FAPI_dlschConfig_st  dlSCHPdu;
    FAPI_dciConfig_st    dciConfig[FAPI_MAX_DCI_PDU];
    /* + PRS_CHANGE */
    FAPI_prsPduConfigInfo_st    prs; 
    /* - PRS_CHANGE */
#ifdef LTE_EMBMS_SUPPORTED
    /* +MBMS_CHANGE */
    FAPI_mchPduConfigInfo_st    mchPdu;
    /* -MBMS_CHANGE */
#endif    

}fapiMsgIdStruct;

/* DL_DATA_SEP changes */
typedef struct dlMsg_stT{
    char msg[20000];
    fapiMsgIdStruct msgIdArr[255];
    UInt8 datamsg;
    UInt16 recvSFN;
    UInt16 recvSF;
} dlMsg_t;

#define FAPI_SFN_MASK 0xFFF0
#define FAPI_SFN_VALUE_SHIFT 4
#define FAPI_SF_MASK  0x000F
#define FAPI_RACH_INDICATION_BODY_LEN 9
#define FAPI_RACH_INDICATION_BODY_LEN_R9 15
#define FAPI_RACH_INDICATION_BODY_LEN_R13 20
#define FAPI_CRC_INDICATION_BODY_LEN 7
#define FAPI_MAX_UL_DATA_SIZE 10000

/*The below values are dummy*/
#define DOPPLER_ESTIMATION 15 
#define TIMING_ADVANCE 20    
#define NUM_OF_RB 1         
#define RB_START 1         
#define SNR 1             

#if 0
typedef struct FAPI_harqIndQueueNode_stT
{
  LTE_SNODE   harqNodeAnchor;
  UInt16      sfnsf;
  #ifdef FDD_CONFIG
  FAPI_fddHarqPduIndication_st harqPduInfo;
  #elif TDD_CONFIG
  FAPI_tddHarqPduIndication_st harqPduInfo;
  #endif

}FAPI_harqIndQueueNode_st;
#endif

#ifndef TDD_CONFIG
LTE_SQUEUE harqQueue[MAX_NUM_CELLS][MAX_SUBFRAME];
#else
LTE_SQUEUE harqQueue[MAX_NUM_CELLS][MAX_UL_ARRAY];
#endif

/* SS_S2 */
#ifndef TDD_CONFIG
LTE_SQUEUE cqiIndQueue[MAX_NUM_CELLS][MAX_SUBFRAME];
#else
LTE_SQUEUE cqiIndQueue[MAX_NUM_CELLS][MAX_UL_ARRAY];
#endif
/* SS_S2 */

typedef struct FAPI_ulRachIndicationNode_stT
{
    LTE_SNODE rachNodeAnchor;
    UInt8 *rachInd_p;
    UInt16 sfnsf;
    UInt32 msgLen;

}FAPI_ulRachIndicationNode_st;


#ifndef TDD_CONFIG
/* SPR 11549 fix start */
#define MAX_RACH_ARRAY 20
LTE_SQUEUE rachIndQueue[MAX_NUM_CELLS][MAX_RACH_ARRAY];
/* SPR 11549 fix end */
#else
LTE_SQUEUE rachIndQueue[MAX_NUM_CELLS][MAX_UL_ARRAY];
#endif

typedef struct FAPI_ulCrcIndicationNode_stT
{
    LTE_SNODE crcNodeAnchor;
    UInt16 sfnsf;
    FAPI_crcPduIndication_st crcPduInfo;

}FAPI_ulCrcIndicationNode_st;

#ifndef TDD_CONFIG
LTE_SQUEUE crcIndQueue[MAX_NUM_CELLS][MAX_SUBFRAME];
#else
LTE_SQUEUE crcIndQueue[MAX_NUM_CELLS][MAX_UL_ARRAY];
#endif
typedef struct FAPI_ulSrsIndicationNode_stT
{
    LTE_SNODE srsNodeAnchor;
    UInt16 sfnsf;
    UInt16 rnti;
    UInt16 handle;

}FAPI_ulSrsIndicationNode_st;

#ifndef TDD_CONFIG
LTE_SQUEUE srsIndQueue[MAX_NUM_CELLS][MAX_SUBFRAME];
#else
LTE_SQUEUE srsIndQueue[MAX_NUM_CELLS][MAX_UL_ARRAY];
#endif
#if 0

typedef struct FAPI_ulSrIndicationNode_stT
{
    LTE_SNODE srNodeAnchor;
    UInt16 sfnsf;
    FAPI_srPduIndication_st srPduInfo;

}FAPI_ulSrIndicationNode_st;

#endif

#ifndef TDD_CONFIG
LTE_SQUEUE srIndQueue[MAX_NUM_CELLS][MAX_SUBFRAME];
#else
LTE_SQUEUE srIndQueue[MAX_NUM_CELLS][MAX_UL_ARRAY];
#endif

/*SPR 12148 start */
LTE_SQUEUE pdcchOrderQueue_g[MAX_NUM_CELLS];
/*SPR 12148 end */
typedef struct FAPI_ulPdcchOrderNode_stT
{
    LTE_SNODE pdcchOrderNodeAnchor;
    UInt16 crnti; 
    UInt8 preambleIdx;
}FAPI_ulPdcchOrderNode_st;
typedef struct ulschDataQueueNodeT 
{
  LTE_SNODE   ulDataAnchor;
  UInt8  qIndex;
  UInt8  SF;
  UInt8  *pdudata_p;
  UInt8 isCqiSet;
  UInt16 rnti;
  UInt16 sendSfnSf;
  UInt16 SFN;
  UInt16 pduLen;
}ulschDataQueueNode;
#ifdef FDD_CONFIG
/** For TTIB_Code */
typedef struct ttibDataQueueNodeT
{
    LTE_QUEUE_NODE  node;
    UInt8 harqIndex;
    UInt16 sendSfnSf;
    UInt8 qIndex;
    UInt8 ttibqIndex;
    UInt8 phichReceiveIndex;
    UInt8 delay;
    FAPI_dlDCIPduInfo_st dciPdu;
    UInt8 recvSF;
    UInt16 recvSFN;
}ttibDataQueueNode;
#endif

#ifndef TDD_CONFIG
LTE_SQUEUE ulDataQueue[MAX_NUM_CELLS][MAX_SUBFRAME];
#else
LTE_SQUEUE ulDataQueue[MAX_NUM_CELLS][MAX_UL_ARRAY];
#endif

#ifdef FDD_CONFIG
/** For TTIB_Code */
/* SPR_11526_Fix Start */
LTE_QUEUE ttibDataQueue_g[MAX_NUM_CELLS][TTI_BUNDLE_RTT];
/* SPR_11526_Fix End */
#endif

#ifndef TDD_CONFIG
LTE_SQUEUE onlyBsrPhrQueue[MAX_NUM_CELLS][MAX_SUBFRAME];
#else
LTE_SQUEUE onlyBsrPhrQueue[MAX_NUM_CELLS][MAX_UL_ARRAY];
#endif
void fapiReadMacMsg( UInt8 *recvDataBuffer_p, fapiMsgIdStruct *msgIdArr_p, UInt8 cellIndex
        );

UInt32 preambleRangeStart_g;
UInt32 preambleRangeEnd_g;
UInt32 mcsIndexPdcchOrder_g;
UInt32 numRBPdcchOrder_g;
UInt32 mcsIndexHO_g;
UInt32 numRBHO_g;
UInt32 mcsIndexPreambleGroupA_g;
UInt32 numRBPreambleGroupA_g;
UInt32 mcsIndexPreambleGroupB_g;
UInt32 numRBPreambleGroupB_g;

/* SPR 609 changes start */
extern  void fapiulDispatcher(UInt8 cellIndex);
#ifndef UESIM_PDCP_THREAD
extern void* rlcAndPdcpMsgHandler(UInt8 cellIndex);
#else
extern void* rlcAndPdcpMsgHandler(void* param)
#endif
extern void processEnodebRrcPhyMsg(UInt64 numPdu, UInt8 cellIndex);
extern void cleanupUePhySim(SInt32 *rxEnodebRRCSocketFD_g,SInt32 *txEnodebRRCSocketFD_g);
extern UInt16 pdcpContextIsInitialized(void);
extern void prepareAndSendRespMsg(UInt8 *inMsg_p, UInt8 *outMsg_p, UInt16 resp);
extern void parseRRCMsg(UInt8* msgBuf, SInt32 bytesRead);
/* +- SPR 17777 */
extern void handleOamRequests(UInt8* recvBuffer);
/* +- SPR 17777 */
extern void pdcpOamProcessMessage( UInt8 * data_p, UInt16 size );
extern void pdcpRrcProcessMessage( UInt8 * data_p, UInt16 size );
extern inline void initULHarqTimerArray();
extern void fapiPrepareLongBSR(dynamicUEContext *ulDynUEContext_p, 
                              UInt16 crnti, UInt16 sendSfnSf, UInt8 qIndex, ulschDataQueueNode* ulDataNode_p, UInt32 tbSize); 
/* SPR 609 changes end */
#define MAX_ULCONFIG_DELAY 1

#ifdef UE_SIM_SPLIT
#define MAX_DL_NUM_MESSAGE 2
LTE_SQUEUE dlMsgStoreQueue;
void initIPCInterfaceUEPHYAndUEMAC( void );
SInt32 txUePhySocketFD_g;
SockAddr uePhyLayerAddr_g;
SInt32 rxUeMacSocketFD_g;
SockAddr ueMacLayerAddr_g;
#endif

/* SPR 11549 fix start */
typedef enum PreambleFormatT
{
    PREAMBLE_FORMAT_0 = 0,
    PREAMBLE_FORMAT_1,
    PREAMBLE_FORMAT_2,
    PREAMBLE_FORMAT_3,
#ifdef TDD_CONFIG
    PREAMBLE_FORMAT_4,
#endif
    INVALID_PREAMBLE_FORMAT
}PreambleFormat;

#ifdef FDD_CONFIG
typedef enum PRACHSysFrameNumT
{
    EVEN = 0,
    ANY,
    INVALID_PRACH_SFN
}PRACHSysFrameNum;

/*Don't change the order of structure elements*/
typedef struct PRACHConfigInfoT
{
    PreambleFormat   preambleFormat;
    PRACHSysFrameNum prachSFN;
    UInt8 subFrame[MAX_SUBFRAME];
}PRACHConfigInfo;
#endif
/* SPR 11549 fix end */
/* SSI MBMS Changes Start */
#if (defined(LTE_EMBMS_SUPPORTED) && !defined(LAYER2_DLTG_THREAD))
extern SInt32          tgGlueTx;
SockAddr               tgGlueAddr;
struct sockaddr_in6 sockAddr6;
/*#define                TGIP         "1.1.1.1"
#define                TGPORT       10061*/
#define             TGIP "172.16.81.57"
#define                TGPORT       6666
#endif
/* SSI MBMS Changes End */

#endif
