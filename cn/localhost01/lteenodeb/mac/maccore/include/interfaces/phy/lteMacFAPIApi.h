/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacFAPIApi.h,v 1.1.4.7.4.1 2010/11/08 06:06:15 gur10121 Exp $
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
 * $Log: lteMacFAPIApi.h,v $
 * Revision 1.1.4.7.4.1  2010/11/08 06:06:15  gur10121
 * Semi static changes merging
 *
 * Revision 1.1.4.7  2010/10/25 06:46:21  gur19942
 * Defined default values for CELL config and UE config parameters.
 *
 * Revision 1.1.4.6.2.3  2010/10/29 15:09:41  gur10121
 * ULCONFIG_SEND_ADVANCE updated to 1
 *
 * Revision 1.1.4.6.2.2  2010/10/27 17:13:48  gur10121
 * UT fixes + review comments incorporation
 *
 * Revision 1.1.4.6  2010/10/20 06:01:45  gur20491
 * Added structure for additional parameter parsing as per RRC 2.0.1
 *
 * Revision 1.1.4.5.2.3  2010/10/21 09:16:18  gur10121
 * UE config put under MAC_SEMI_STATIC_INF flag
 *
 * Revision 1.1.4.5.2.2  2010/10/21 07:53:30  gur10121
 * Semi static UT fixes
 *
 * Revision 1.1.4.5.2.1  2010/10/18 14:36:44  gur19942
 * semistatic changes for ulconfig.
 *
 * Revision 1.1.4.5  2010/10/08 12:58:28  gur19942
 * Changes related to UL SCH putting in Q.
 *
 * Revision 1.1.4.4  2010/10/07 09:34:57  gur10694
 * Fapi UT fixes.
 *
 * Revision 1.1.4.3  2010/09/27 14:46:47  gur10694
 * Compilation break fix
 *
 * Revision 1.1.4.2  2010/09/27 14:36:59  gur10694
 * UE config Fix and Review comments
 *
 * Revision 1.1.4.1  2010/09/21 15:45:52  gur20491
 * FAPI changes
 *
 * Revision 1.1.2.2  2010/09/08 14:17:00  gur20439
 * FAPI UL Changes merged
 *
 * Revision 1.1.2.1  2010/09/07 11:21:07  gur20439
 * new files added for FAPI changes
 *
 * Revision 1.1.2.1  2010/08/13 08:48:49  gur10694
 * initial creation
 *
 *
 * Revision 0.1  2010/07/28 22:49:24  gur10694
 * Initial Version
 *
 *****************************************************************************/

#ifndef LTE_MAC_PHY_INTF_FF
#define LTE_MAC_PHY_INTF_FF

#include "lteTypes.h"
#include "lteMisc.h"
#include "lteSQueue.h"
#include "lteMacTypes.h"
#include "lteFapiMacPhyInterface.h"
#include "lteMacPUSCH.h"
extern SInt32 txSockFD_g;
extern SockAddr tx_addr_g;
/* SPR 19288 change start */
/* SPR 19288 change end */
/**
 * Data: MACRO 
 *
 * Name: FAPI_MAX_NUM_TLVS_CONFIG_PHY
 *
 * Description: This MACRO descibes the Mamimum number of TLVs that are sent to
 * PHY in PHY Cell configuration request
 */

/*SPR 8513 Start
 2 New TLVs have been added in PRS. Hence, max number of TLVs
 needs to be increased*/


#ifdef TDD_CONFIG
/* SPR 19288 change start */
#define FAPI_MAX_NUM_TLVS_CONFIG_PHY   40 
/* SPR 19288 change end */
#else
/* SPR 19288 change start */
/*SPR 21001 Changes start */
#define FAPI_MAX_NUM_TLVS_CONFIG_PHY    38
/* SPR 19288 change end */
#endif
/*SPR 8513 End*/
        
/**
 * Data: MACRO 
 *
 * Name: FAPI_FRAME_IND_SFN_MASK
 *
 * Description: This MACRO descibes the bit porition of presence of SFN in frmae
 * indication from PHY
 */
#define FAPI_FRAME_IND_SFN_MASK         0xFFF0

/**
 * Data: MACRO 
 *
 * Name: FAPI_FRAME_IND_SF_MASK
 *
 * Description: This MACRO descibes the bit porition of presence of SF in frmae
 * indication from PHY
 */
#define FAPI_FRAME_IND_SF_MASK          0x000F

/**
 * Data: MACRO 
 *
 * Name: PHICH_POWER_OFFSET
 *
 * Description: This MACRO descibes the value of PHICH power offset attribute
 * required in PHY Cell configuration
 * Value 0 - 10000
 * Range (-6db to 4db in steps of 0.0001db)
 */
#define  PHICH_POWER_OFFSET                1 

/**
 * Data: MACRO 
 *
 * Name: PCFICH_POWER_OFFSET
 *
 * Description: This MACRO descibes the value of PCFICH power offset attribute
 * required in PHY Cell configuration 
 * Value 0 - 10000
 * Range (-6db to 4db in steps of 0.0001db)
 */
#define  PCFICH_POWER_OFFSET                1 

/**
 * Data: MACRO 
 *
 * Name: DATA_REPORT_MODE 
 *
 * Description: This MACRO descibes the value of data report mode attibute
 * required in PHY Cell configuration 
 * Value 0 or 1. See FF API doc for description on these values 
 */
/* SPR 1594 changes start */
#define  DATA_REPORT_MODE                  0 
/* SPR 1594 changes end */

/**
 * Data: MACRO 
 *
 * Name: MAX_VAL_PHYSICAL_CELL_ID 
 *
 * Description: This MACRO descibes the maximum value of physical cell id
 */
#define  MAX_VAL_PHYSICAL_CELL_ID               503 

/**
 * Data: MACRO 
 *
 * Name: MAX_VAL_SYNC_SIGNAL 
 *
 * Description: This MACRO descibes the maximum value of primary and secondary
 * sync signal 
 */
#define  MAX_VAL_SYNC_SIGNAL                 9999 

/**
 * Data: MACRO 
 *
 * Name: MAX_VAL_BW_CONFIG 
 *
 * Description: This MACRO descibes the maximum value of bandwidth configuration
 * sync signal 
 */
#define  MAX_VAL_BW_CONFIG                       7 

/**
 * Data: MACRO 
 *
 * Name: MAX_VAL_CONFIG_INDEX 
 *
 * Description: This MACRO descibes the maximum value of configuration index
 */
#define  MAX_VAL_CONFIG_INDEX                      63 

/**
 * Data: MACRO 
 *
 * Name: MAX_VAL_ROOT_SEQ_INDEX 
 *
 * Description: This MACRO descibes the maximum value of root seq index
 */
#define  MAX_VAL_ROOT_SEQ_INDEX                     837 

/**
 * Data: MACRO 
 *
 * Name: MAX_VAL_ZERO_COR_ZONE 
 *
 * Description: This MACRO descibes the maximum value of zero corelation zone
 */
#define  MAX_VAL_ZERO_COR_ZONE                      15 

/**
 * Data: MACRO 
 *
 * Name: MAX_VAL_GRP_ASSIGN 
 *
 * Description: This MACRO descibes the maximum value of group assignment
 */
#define  MAX_VAL_GRP_ASSIGN                         29 

/**
 * Data: MACRO 
 *
 * Name: MAX_VAL_CYC_SHIFT1_FOR_DMRS 
 *
 * Description: This MACRO descibes the maximum value of cycclic shift for DMRS
 */
#define  MAX_VAL_CYC_SHIFT1_FOR_DMRS                7 

/**
 * Data: MACRO 
 *
 * Name: MAX_VAL_NUM_OF_SUB_BANDS 
 *
 * Description: This MACRO descibes the maximum value of num of sub bands
 */
#define  MAX_VAL_NUM_OF_SUB_BANDS                   4  

/**
 * Data: MACRO 
 *
 * Name: MAX_VAL_DELTA_PUCCH_SHIFT 
 *
 * Description: This MACRO descibes the maximum value of delta PUCCH shift
 */
#define  MAX_VAL_DELTA_PUCCH_SHIFT                   3  

/**
 * Data: MACRO 
 *
 * Name: MAX_VAL_NCQIRB 
 *
 * Description: This MACRO descibes the maximum value of nCQIRB
 */
#define  MAX_VAL_NCQIRB                              98  

/**
 * Data: MACRO 
 *
 * Name: MAX_VAL_NANCS 
 *
 * Description: This MACRO descibes the maximum value of nAnCs
 */
#define  MAX_VAL_NANCS                               7  

/**
 * Data: MACRO 
 *
 * Name: MAX_VAL_N1_PUCCH_AN 
 *
 * Description: This MACRO descibes the maximum value of n1PUCCHAn
 */
#define  MAX_VAL_N1_PUCCH_AN                          2047 

/**
 * Data: MACRO 
 *
 * Name: MAX_VAL_PHICH_RESOURCE 
 *
 * Description: This MACRO descibes the maximum value of PHICH Resource 
 */
/* SPR 2159 Changes Start */
#define  MAX_VAL_PHICH_RESOURCE                         3   
/* SPR 2159 Changes End */

/**
 * Data: MACRO 
 *
 * Name: MAX_VAL_PB
 *
 * Description: This MACRO descibes the maximum value of pB 
 */
#define  MAX_VAL_PB                                     3  

/**
 * Data: MACRO 
 *
 * Name: MAX_VAL_UL_RS_HOPPING
 *
 * Description: This MACRO descibes the maximum value of uplink RS hopping 
 */
#define  MAX_VAL_UL_RS_HOPPING                           2   

/**
 * Data: MACRO 
 *
 * Name: MAX_VAL_DUPLEX_MODE
 *
 * Description: This MACRO descibes the maximum value of duplexing mode 
 */
#define  MAX_VAL_DUPLEX_MODE                              2 

/**
 * Data: MACRO 
 *
 * Name: MAX_VAL_BUFF_SIZE
 *
 * Description: This MACRO descibes the maximum value of msg buffer 
 */
#define  MAX_VAL_BUFF_SIZE                               255 

/**
 * Data: MACRO 
 *
 * Name: FAPI_MAX_READ_RETRY
 *
 * Description: This MACRO descibes the maximum number of message read retries
 * 
 */
#define FAPI_MAX_READ_RETRY                               3

/**
 * Data: MACRO 
 *
 * Name: FAPI_MAX_PARAM_REQUEST_RETRY
 *
 * Description: This MACRO descibes the maximum number of parame request message retries
 * 
 */
#define FAPI_MAX_PARAM_REQUEST_RETRY 3

/**
* Data : MACRO
* Name : RECONFIG_OFFSET_NUM
* Description : This Macro describes the number of offset values available
*               during PHY Reconfig request message 
**/
#define RECONFIG_OFFSET_NUM 21


/* Enumeration for decoding Uplink and Downlink Bandwidth
 * in PHY Cell config Request Message 
 */

/**
* Data : MACRO 
* Name : NUM_BITS_PHY_RECONFIG_PARAM
* Description : This Macro describes the number of bits in the bitset 
                available 
**/
#define NUM_BITS_PHY_RECONFIG_PARAM 8

/*
 * Data : MACRO
 * Name : INVALID_RECONFIG_PARAM_OFFSET
 * Description : Macro for invalid offset to optional paramters
 **/ 
#define INVALID_RECONFIG_PARAM_OFFSET -1
#ifdef LTE_EMBMS_SUPPORTED
/*SPR 18013 fix start */
#define PHY_EMBMS_NOT_SUPPORTED 0
#define PHY_EMBMS_SUPPORTED 1
/*SPR 18013 fix end */
#endif
typedef enum
{
    PHY_RECONF_PARAM    = 0,
    SYNC_SIGNALS        = 1,
    PRACH_CONFIGURATION = 2,
    PUSCH_CONFIGURATION = 3,
    PUCCH_CONFIGURATION = 4,
    PHICH_CONFIGURATION = 5,
    PDSCH_CONFIGURATION = 6,
    DUPLEX_MODE = 7, 
    UL_EAR_FCN  = 8,
    DL_EAR_FCN  = 9,
    ANTENNAE    = 10,
    UL_TX_BW    = 11,
    DL_TX_BW    = 12,
    UL_CYC_PRE  = 13,
    DL_CYC_PRE  = 14,
    SRS_CONFIG  = 15,
    PHY_CELL_ID = 16,
    PCFICH_OFF  = 17, 
    PHICH_OFF   = 18,
/* SPR 3802 */
    MAX_EPRE    = 19,
/* SPR 3802 */
    /* + PRS_CHANGES */
    PRS_CONFIG  = 20
    /* - PRS_CHANGES */    
}phy_reconfig_struct_indices_et;


typedef enum
{
    UL_TX_BANDWIDTH_6RB = 0,
    UL_TX_BANDWIDTH_15RB = 1,
    UL_TX_BANDWIDTH_25RB = 2,
    UL_TX_BANDWIDTH_50RB = 3,
    UL_TX_BANDWIDTH_75RB = 4,
    UL_TX_BANDWIDTH_100RB = 5
}ul_tx_bandwidth_et;

typedef enum
{
    DL_TX_BANDWIDTH_6RB = 0,
    DL_TX_BANDWIDTH_15RB = 1,
    DL_TX_BANDWIDTH_25RB = 2,
    DL_TX_BANDWIDTH_50RB = 3,
    DL_TX_BANDWIDTH_75RB = 4,
    DL_TX_BANDWIDTH_100RB = 5
} dl_tx_bandwidth_et;


/* Cell Start Stop Chg */
/* Enum for Bit Mask Matching */
typedef enum
{
    BIT_FIRST   = 0x01,
    BIT_SECOND  = 0x02,
    BIT_THIRD   = 0x04,
    BIT_FOURTH  = 0x08,
    BIT_FIFTH   = 0x10,
    BIT_SIXTH   = 0x20,
    BIT_SEVENTH = 0x40,
    BIT_EIGTH   = 0x80,
    BIT_NINTH   = 0x0100,
    BIT_TENTH   = 0x0200,
/* SPR 3802 */
    BIT_FIFTEENTH = 0x4000,
/* SPR 3802 */
    /* + PRS_CHANGES */
    BIT_SIXTEENTH = 0x8000,
    /* - PRS_CHANGES */
}bit_mask_setting_et;
/* Cell Start Stop Chg */

/**
* Data : MACRO 
* Name : GENERATE_SUBFRAME_SFNSF
* Description : This Macro describes the sfn-sf used at L1-L2 interface 
                16 bit SFN/SF has bits 0-3 = SF bits 4-15 = SFN
**/
#define GENERATE_SUBFRAME_SFNSF(sfn,sf) (MAC_PHY_CONVERT_16(( ( (sfn) << NUM_OF_SF_BITS) | ( (sf) & (FAPI_FRAME_IND_SF_MASK)) )))

/**
* Data : MACRO 
* Name : GET_SYSFRAMENUM_FROM_SFNSF
*      : GET_SUBFRAME_FROM_SFNSF
* Description : This Macro retrives the sfn-sf used at L1-L2 interface 
                16 bit SFN/SF has bits 0-3 = SF bits 4-15 = SFN
**/
#define GET_SYSFRAMENUM_FROM_SFNSF(sfnSf)   (((MAC_PHY_CONVERT_16(sfnSf)) & (FAPI_FRAME_IND_SFN_MASK)) >> NUM_OF_SF_BITS)
#define GET_SUBFRAME_FROM_SFNSF(sfnSf)      ((MAC_PHY_CONVERT_16(sfnSf)) & (FAPI_FRAME_IND_SF_MASK))

/**
 * Data: MACRO 
 *
 * Name: L1_API_HEADER_LEN
 *
 * Description: This MACRO descibes the L1 message API Header length
 */
#define L1_API_HEADER_LEN       4

/**
 * Data: Struct 
 *
 * Name: ULRefSigConfigPhyT
 *
 * Description: This structure contains Referece Signal parameters required in
 * PHY Cell Configuration   
 */
typedef struct ULRefSigConfigPhyT
{
  UInt16  uplinkRSHoping;
  UInt16  groupAssignment;
  UInt16  cyclicShift1forDMRS;
}ULRefSigConfigPhyT;

/**
 * Data: Struct 
 *
 * Name: SRSConfigPhy
 *
 * Description: This structure contains SRS config parameters required in
 * PHY Cell Configuration   
 */
typedef struct SRSConfigPhyT
{
  UInt16  bandWidthConfiguration;
  UInt16  macUpPTS;
  UInt16  SRSSubframeConfiguration;
  UInt8  srsAckNackSimulTx;
}SRSConfigPhy;

/**
 * Data: Struct 
 *
 * Name: PUCCHConfigPhy
 *
 * Description: This structure contains PUCCH config parameters required in
 * PHY Cell Configuration   
 */
typedef struct PUCCHConfigPhyT
{
  UInt16  deltaPUCCHShift;
  UInt16  nCQIRB;
  UInt16  nAnCs;
  UInt16  n1PucchAn;
}PUCCHConfigPhy;

/**
 * Data: Struct 
 *
 * Name: PUSCHConfigPhy
 *
 * Description: This structure contains PUSCH config parameters required in
 * PHY Cell Configuration   
 */
typedef struct PUSCHConfigPhyT
{
  UInt16  hoppingMode;
  UInt16  hoppingOffset;
  UInt16  numOfSubBand;
  UInt8   groupHopEnable;
  UInt8   groupAssignPUSCH;
  UInt8   seqHopEnable;
  UInt8   cyclicShift;
}PUSCHConfigPhy;

/**
 * Data: Struct 
 *
 * Name: PRACHConfigPhy
 *
 * Description: This structure contains PRACH config parameters required in
 * PHY Cell Configuration   
 */
typedef struct PRACHConfigPhyT
{
  UInt16  configurationIndex;
  UInt16  rootSeqIndex;
  UInt16  zeroCorelationZoneConfig;
  UInt16  highSpeedFlag;
  UInt16  freqOffset;
}PRACHConfigPhy;

/**
 * Data: Struct 
 *
 * Name: SCHConfigPhyT
 *
 * Description: This structure contains SCH config parameters required in
 * PHY Cell Configuration   
 */
typedef struct SCHConfigPhyT
{
  UInt16  primarySyncSignal;
  UInt16  secondarySyncSignal;
  UInt16  physicalCellId;
}SCHConfigPhy;

/**
 * Data: Struct 
 *
 * Name: PHICHConfigPhy
 *
 * Description: This structure contains PHICH config parameters required in
 * PHY Cell Configuration   
 */
typedef struct PHICHConfigPhyT
{
  UInt16  phichResource;
  UInt16  phichDuration;
  UInt16  phichPowOffset;
}PHICHConfigPhy;

/**
 * Data: Struct 
 *
 * Name: RFConfigPhy
 *
 * Description: This structure contains RF config parameters required in
 * PHY Cell Configuration   
 */
typedef struct RFConfigPhyT
{
  UInt16  dlChannelBW;
  UInt16  ulChannelBW;
  SInt8  refSignalPower;
  UInt16  txAntennaPort;
  UInt16  rxAntennaPort;
}RFConfigPhy;
#ifdef FAPI_4_0_COMPLIANCE
typedef struct MBSFNConfigT 
{
  UInt16  mbsfnAreaID;
}MBSFNConfigT;

#endif
/* + PRS_CHANGES */
typedef struct PRSConfigPhyT
{
    UInt8 updateFlag;
    UInt8 prsBandWidth;
    UInt8 prsCyclicPrifix;
}PRSConfigPhy;


/**
 * Data: Struct 
 *
 * Name: CellConfigPhy
 *
 * Description: This structure contains all the cell config parameters required
 * in PHY Cell Configuration (contains parameters both for FDD and TDD)   
 */
typedef struct CellConfigPhyT
{
  UInt16            duplexingMode;
  UInt16            pcfichPowerOffset;
  UInt16            pb;
  UInt16            dlCyclicPrefixType;
  UInt16            ulCyclicPrefixType;
  UInt16            dataReportingMode;
  UInt16            sfnsf;
  UInt16            phyState; 
  UInt16            dlBandWidthSupport;
  UInt16            ulBandWidthSupport;
  UInt16            dlModulationSupport;
  UInt16            ulModulationSupport;
  UInt16            phyAntennaCapability;
  RFConfigPhy       rfConfig;
  PHICHConfigPhy    phichConfig;
  SCHConfigPhy      schConfig;
  PRACHConfigPhy    prachConfig;
  PUSCHConfigPhy    puschConfig;
  PUCCHConfigPhy    pucchConfig;
  SRSConfigPhy      srsConfig;
  ULRefSigConfigPhyT ulRefSigConfig;
/* SPR 3802 */
  UInt8             maxEpre;
/* SPR 3802 */
#ifdef TDD_CONFIG
  UInt16 subFrameAssign;
  UInt16 splSubFramePatterns;
#endif
  PRSConfigPhy    prsPhyConfig;
#ifdef FAPI_4_0_COMPLIANCE
  MBSFNConfigT    mbsfnConfig;
  UInt16          releaseCapability;
  UInt16          mbsfnCapability;
#endif
/*+ SPR 18399*/
/*- SPR 18399*/
}CellConfigPhy;


/* Cell Start Stop Chh */
/* Structure having parameter coming from L3 in 
 * PHY Reconfig request */
typedef struct CellReConfigPhyMajorT
{
  UInt16            duplexingMode;
  UInt8             duplexingMode_flag;
  UInt16            pb;
  UInt8             pb_flag;
  UInt16            dlCyclicPrefixType;
  UInt8             dlCyclicPrefixType_flag;
  UInt16            ulCyclicPrefixType;
  UInt8             ulCyclicPrefixType_flag;
  UInt16            dlChannelBW;
  UInt8             dlChannelBW_flag;
  UInt16            ulChannelBW;
  UInt8             ulChannelBW_flag;
  SInt8            refSignalPower;
  UInt8             refSignalPower_flag;
  UInt16            txAntennaPort;
  UInt8             txAntennaPort_flag;
  UInt16            rxAntennaPort;
  UInt8             rxAntennaPort_flag;
  UInt16            phichResource;
  UInt8             phichResource_flag;
  UInt16            phichDuration;
  UInt8             phichDuration_flag;
  UInt16            primarySyncSignal;
  UInt8             primarySyncSignal_flag;
  UInt16            secondarySyncSignal;
  UInt8             secondarySyncSignal_flag;
  UInt16            physicalCellId;
  UInt8             physicalCellId_flag;
  UInt16            configurationIndex;
  UInt8             configurationIndex_flag;
  UInt16            rootSeqIndex;
  UInt8             rootSeqIndex_flag;
  UInt16            zeroCorelationZoneConfig;
  UInt8             zeroCorelationZoneConfig_flag;
  UInt16            highSpeedFlag;
  UInt8             highSpeedFlag_flag;
  UInt16            freqOffset;
  UInt8             freqOffset_flag;
  UInt16            hoppingMode;
  UInt8             hoppingMode_flag;
  UInt16            hoppingOffset;
  UInt8             hoppingOffset_flag;
  UInt16            numOfSubBand;
  UInt8             numOfSubBand_flag;
  UInt8             groupHopEnable;
  UInt8             groupHopEnable_flag;
  UInt8             groupAssignPUSCH;
  UInt8             groupAssignPUSCH_flag;
  UInt8             seqHopEnable;
  UInt8             seqHopEnable_flag;
  UInt8             cyclicShift;
  UInt8             cyclicShift_flag;
  UInt16            deltaPUCCHShift;
  UInt8             deltaPUCCHShift_flag;
  UInt16            nCQIRB;
  UInt8             nCQIRB_flag;
  UInt16            nAnCs;
  UInt8             nAnCs_flag;
  UInt16            n1PucchAn;
  UInt8             n1PucchAn_flag;
  UInt16            bandWidthConfiguration;
  UInt8             bandWidthConfiguration_flag;
  /*SPR 3802*/
  UInt8             maxEpre;
  /* + PRS_CHANGES */
  PRSConfigPhy      prsPhyConfig;
  UInt8             prsBandWidth_flag;
  UInt8             prsCyclicPrifix_flag;
  /* - PRS_CHANGES */
  /*SPR 3802*/
}ReConfigPhyCellMajor;

/* Cell Start Stop Chh */

SInt32 sendPhyCellRespToRRC(UInt16 msgId, SInt32 transId,
        MacRetType retVal, RrcCellIndex cellIndex);
/***********************************************************/
/**
 * Data: MACRO 
 *
 * Name: SIZE_OF_TLV_OF_VALUE_LEN_4
 *
 * Description: Defines size of tlv which contains value of 4 bytes.
 * it includes 
 * 	1 byte for tlv tag 
 * 	1 byte for tlv len
 * 	4 bytes for tlv value
 * 	2 byte padding to make multiple of 4 bytes.
 */
#define SIZE_OF_TLV_OF_VALUE_LEN_4	8

/***********************************************************/
/**
 * Data: MACRO 
 *
 * Name: SIZE_OF_TLV_OF_VALUE_LEN_4
 *
 * Description: Defines size of tlv which contains value of 4 bytes.
 * it includes 
 * 	1 byte for tlv tag 
 * 	1 byte for tlv len
 * 	2 bytes for tlv value
 */
#define SIZE_OF_TLV_OF_VALUE_LEN_2	4

/***********************************************************/
/**
 * Data: MACRO 
 *
 * Name: SIZE_OF_TLV_OF_VALUE_LEN_4
 *
 * Description: Defines size of tlv which contains value of 4 bytes.
 * it includes 
 * 	1 byte for tlv tag 
 * 	1 byte for tlv len
 * 	1 bytes for tlv value
 * 	1 byte padding to make multiple of 4 bytes.
 */
#define SIZE_OF_TLV_OF_VALUE_LEN_1	4

/**
 * Data: MACRO 
 *
 * Name: NO_OF_TLV_IN_UE_CONFIG_API
 *
 * Description: Max no of TLVs present in UE config API
 */
#define  NO_OF_TLV_IN_UE_CONFIG_API     20 

/**
 * Data: MACRO 
 *
 * Name: UE_CONF_NO_OF_TLV_OF_VALUE_LEN_4
 *
 * Description: No of TLVs of size 4 present in UE config API
 */
#define  UE_CONF_NO_OF_TLV_OF_VALUE_LEN_4  1

/**
 * Data: MACRO 
 *
 * Name: UE_CONF_NO_OF_TLV_OF_VALUE_LEN_2
 *
 * Description: No of TLVs of size 4 present in UE config API
 */
#define  UE_CONF_NO_OF_TLV_OF_VALUE_LEN_2  10

/**
 * Data: MACRO 
 *
 * Name: UE_CONF_NO_OF_TLV_OF_VALUE_LEN_1
 *
 * Description: No of TLVs of size 4 present in UE config API
 */
#define  UE_CONF_NO_OF_TLV_OF_VALUE_LEN_1  9

/**
 * Data: MACRO 
 *
 * Name: TOTAL_SIZE_OF_TLV_VAL_IN_UE_CONFIG_API
 *
 * Description: Size of all the values in UE config API
 */
#define  TOTAL_SIZE_OF_TLV_VAL_IN_UE_CONFIG_API      33

/**
 * Data: MACRO 
 *
 * Name: FILL_TLV_STRUCT_VAL_ONE_BYTE
 *
 * Description: Fills TLV with tag and one octets value.
 */
#define FILL_TLV_STRUCT_VAL_ONE_BYTE(tlv_p, tag, val)\
{\
    tlv_p[0] = tag;\
    tlv_p[1] = 4;\
    tlv_p[2] = val;\
}

/**
 * Data: MACRO 
 *
 * Name: 
 *
 * Description: These macros defines the bit mask for accessing parameters in 
 * PHY_CREATE_UE_ENTITY_REQ from RRC.
 */
#define LTE_MAC_PDSCH_CONFIG_DEDICATED_BIT_MASK         1<<0
#define LTE_MAC_PUCCH_CONFIG_DEDICATED_BIT_MASK         1<<1
#define LTE_MAC_PUSCH_CONFIG_DEDICATED_BIT_MASK         1<<2
#define LTE_MAC_UL_POWER_CONTRL_DICATED_BIT_MASK        1<<3
#define LTE_MAC_TPC_PDCCH_CONF_PUCCH_BIT_MASK           1<<4
#define LTE_MAC_TPC_PDCCH_CONF_PUSCH_BIT_MASK           1<<5
#define LTE_MAC_CQI_REPORTING_BIT_MASK                  1<<6
#define LTE_MAC_SRS_UL_CONFIG_BIT_MASK                  1<<7
#define LTE_MAC_ANTENNA_INFO_BIT_MASK                   1<<8
#define LTE_MAC_SR_CONFIG_BIT_MASK                      1<<9

/**
 * Data: MACRO 
 *
 * Name: 
 *
 * Description: These macros defines the bit mask for accessing 
*  ACK/NACK parameters in PHY_CREATE_UE_ENTITY_REQ from RRC.
 */
#define LTE_MAC_ACK_NACK_REP_PARAM_BIT_MASK             1<<0
#define LTE_MAC_TDD_ACK_NACK_FEEDBACK_BIT_MASK          1<<1


/**
 * Data: MACRO 
 *
 * Name: 
 *
 * Description: These macros defines the bit mask for accessing 
*  CQI reporting parameters in PHY_CREATE_UE_ENTITY_REQ from RRC.
 */
#define LTE_MAC_CQI_REPORT_MODE_APERIODIC_BIT_MASK      1<<0
#define LTE_MAC_CQI_REPORT_MODE_PERIODIC_BIT_MASK       1<<1

/**
 * Data: Struct 
 *
 * Name: SRConfig_st
 *
 * Description: Contains the SR config parameters.
 */
typedef struct SRConfig_st
{
    UInt16      pucchResInd;
    UInt8       srConfIndex;
} SRConfig_st;

/**
 * Data: Struct 
 *
 * Name: SRSConfig_st
 *
 * Description: Describes the SPS configuration parameters.
 */
typedef struct SRSConfig_st
{
    UInt8       srsBW;
    UInt8       hoppingBW;
    UInt8       freqPos;
    UInt8       srsDuration;
    UInt16      srsConfIndex;
    UInt8       transComb;
    UInt8       cyclicShift;
} SRSConfig_st;

/**
 * Data: Struct 
 *
 * Name: AckNackConf_st 
 *
 * Description: Contains Ack/Nack parameters description as required 
 *   for UE config request.
 */
typedef struct AckNackConf_st
{
    UInt8       anRepFact;
    UInt16      pucchAnRep;
    UInt8       tddAckNackMode;
} AckNackConf_st;

/**
 * Data: Struct 
 *
 * Name: CqiReporting_st
 *
 * Description: Describes parameter information for CQI reporting.
 */
typedef struct CqiReporting_st
{
    UInt16      pucchResIndex;
    UInt16      pmiIndex;
    UInt16      riIndex;
    UInt8       ackNack;
} CqiReporting_st;

/**
 * Data: Struct 
 *
 * Name: UeConfigPhy_st
 *
 * Description: This structure will be used to extract and store information
 *  from UE config request coming from RRC. This information later will 
 *  be used for preparing UE config request which will be sent to layer 1.
 */
typedef struct UeConfigPhy_st
{
    UInt32              rnti;
    CqiReporting_st     cqiReporting;
    AckNackConf_st      ackNackConf;
    SRSConfig_st        srsConfig;
    SRConfig_st         srConfig;
} UeConfigPhy_st;

typedef struct ueConfigReqNode
{
    LTE_SNODE nodeAnchor;
    UInt16    transId;
    UInt8     *msg_p;
    UInt8     len;
    UInt16    ueIndex;
} ueConfigReqNode;

typedef struct ueConfigTransIdNode
{
    LTE_SNODE nodeAnchor;
    UInt16    transId;
    UInt16    ueIndex;
} ueConfigTransIdNode;

/**
 * Data: MACRO 
 *
 * Name: 
 *
 * Description: These macros defines the numbers required for reference signal
 * conversion.
 */
#define MAX_VALUE_OF_REF_SIG_IN_DB  0
#define MULTIPLY_COEFF_FOR_REF_SIG  4

/**
 * Data: MACRO 
 *
 * Name: 
 *
 * Description: These macros defines the numbers required for synchronization 
 * signal conversion.
 */
#define MIN_VALUE_OF_SYNC_SIGNAL    6
#define MULTIPLY_COEFF_FOR_SYNC_SIGNAL 1000
 /* FAPI2_1 chages start*/
#define RELEASE8_MASK                          0x0001      
#define RELEASE9_MASK                          0x0002        
#define RELEASE10_MASK                         0x0004
//#ifdef IPR_NBIOT
/* SPR 21660_2 changes start */
#define RELEASE13_MASK                         0x0020
/* SPR 21660_2 changes end */
//#endif
#define CALC_RIVALUES(riSize) ( ONE<<riSize)
 /* FAPI2_1 chages end*/


/**
 * Data: Struct 
 *
 * Name: ulConfigQueueNode
 *
 * Description: This structure is used for storing UL config msg in UL config
 *  Queue.
 */
typedef struct
{
    FAPI_l1ApiMsg_st * ulConfigReq_p;
    UInt16 len;
} ulConfigQueueNode;

/**
 * Data: MACRO 
 *
 * Name: 
 *
 * Description: This macro indiactes the no of tick before desired SFNSF at 
 * which PHY expects the UL Config
 * Range is 1,2,3.
 * Please verify that MAX_ULCONFIG_DELAY  should be same as 
 *  ULCONFIG_SEND_ADVANCE  
 */
#define ULCONFIG_SEND_ADVANCE 1

void  initUlConfigReqQueue (InternalCellIndex internalCellIndex);
/**
 * Data: MACRO 
 *
 * Name: 
 *
 * Description: These macros define the default values for UE config msg.
 */
#define  DEFAULT_CQI_PUCCH_RES_INDEX	       0
#define  DEFAULT_CQI_PMI_CONF_INDEX	       0
#define  DEFAULT_CQI_RI_CONF_INDEX	       0
#define  DEFAULT_CQI_SIMUL_ACK_NACK	       0
#define  DEFAULT_AN_REPETITION_FACTOR          2
#define  DEFAULT_AN_N1PUCCH_AN_REP             0
#define  DEFAULT_TDD_ACK_NACK_FEED_MODE        0
#define  DEFAULT_SRS_BANDWIDTH                 0
#define  DEFAULT_SRS_HOPPING_BANDWIDTH         0
#define  DEFAULT_FREQ_DOMAIN_POSITION          0
#define  DEFAULT_SRS_DURATION		       0
#define  DEFAULT_ISRS_SRS_CONF_INDEX           0
#define  DEFAULT_SRSCONFIG_MAX_UP_PTS                    0
#define  DEFAULT_TRANSMISSTION_COMB            0
#define  DEFAULT_SOUNDING_REF_CYC_SHIFT        0
#define  DEFAULT_SR_PUCCH_RES_INDEX            0
#define  DEFAULT_SR_CONFIG_INDEX               0
#define  DEFAULT_SPS_DL_CONF_SCH_INTERVAL      10
#define  DEFAULT_SPS_DL__N1PUCCH_AN_PERSISTANT 0
 /* SPR 1699 changes start */
#define  PUSCH_HOPPING_OFFSET                  98
 /* SPR 1699 changes end */ 

/*Added for Power Control*/
#define MAX_PHICH_POWER_OFFSET 10000
#define MAX_PCFICH_POWER_OFFSET 10000

/**
 * Data: MACRO 
 *
 * Name: 
 *
 * Description: These macros define the default values for CELL config msg.
 */
#define DEFAULT_RFCONFIG_ULCHANNEL_BANDWIDTH  6
#define DEFAULT_RFCONFIG_DLCHANNEL_BANDWIDTH  6
#define DEFAULT_SRSCONFIG_BANDWIDTH_CONFIG    0
#define DEFAULT_SRSCONFIG_SUBFRM_CONFIG       0
#define DEFAULT_SRSCONFIG_ACKNACK_SIMUL       0



/**
 * Data: MACRO 
 *
 * Name: 
 *
 * Description: These macros defines max size of UL config MSG.
 */
#define MAX_SIZE_OF_SINGLE_PDU    36
#define MAX_NO_PDU                MAX_ULSCH_UCI_SUPPORTED
#define MAX_ULCONF_BUFF_SIZE      MAX_SIZE_OF_SINGLE_PDU * MAX_NO_PDU

/**
 * Data: MACRO 
 *
 * Name: 
 *
 * Description: These macros defines max size of UL config MSG.
 */
#define GET_MOD_TYPE_FROM_MCS_INDEX(mcs,modType)\
{\
    if((MCS_INDEX_QPSK_0 <= mcs )&& (mcs<= MCS_INDEX_QPSK_9))\
    {\
        modType = QPSK;\
    }\
    else if ((MCS_INDEX_16QAM_10 <=mcs) && (mcs<= MCS_INDEX_16QAM_16))\
    {\
        modType = SIXTEENQAM;\
    }\
    else if ((MCS_INDEX_64QAM_17 <=mcs) && (mcs <= MCS_INDEX_64QAM_28))\
    {\
        modType = SIXFOURQAM;\
    }\
}

/* SPR 1868 */\
/* SPR 3600 End */
/* SPR 3600 Start */
#define GET_MOD_TYPE_FROM_MCS_INDEX_UL(mcs,modType,enb64QAMFlag,ueCategory)\
{     /* + SPR 4434 */\
    if((mcs <= UL_MCS_INDEX_QPSK_10) || (mcs == MCS_INDEX_CQI_WITHOUT_DATA))\
    /* - SPR 4434 */\
    {\
        modType = QPSK;\
    }\
    else if ((UL_MCS_INDEX_16QAM_11 <= mcs) && (mcs <= UL_MCS_INDEX_16QAM_20))\
    {\
        modType = SIXTEENQAM;\
    }\
    else if ((UL_MCS_INDEX_64QAM_21 <= mcs) && (mcs <= UL_MCS_INDEX_64QAM_28))\
    {\
            if((0 == enb64QAMFlag)||(ueCategory < 5 ) )\
            {\
                modType = SIXTEENQAM;\
            }\
            else\
            {\
                modType = SIXFOURQAM;\
            }\
    }\
}
/* SPR 3600 End */
/* SPR 1868 */\
/* + Coverity 24392 */
#define GET_MOD_TYPE_FROM_MCS_INDEX_UL_UE_CATEGORY_1(mcs,modType,enb64QAMFlag) \
{     \
    if((mcs <= UL_MCS_INDEX_QPSK_10) || (mcs == MCS_INDEX_CQI_WITHOUT_DATA))\
      \
    {\
        modType = QPSK;\
    }\
    else \
    {\
        modType = SIXTEENQAM;\
    }\
}
/* - Coverity 24392 */
SInt32 sendUEConfigRespToRRC(UInt16 transId, MacRetType retVal, 
              UInt16 ueIndex,InternalCellIndex internalCellIndex);
MacRetType fapiSendPhyParamReqAndWaitForResp( void );
#define DEFAULT_REDUNDANCY_VERSION    0

SInt32 handleUeReconfigReq (UInt8 *msg_p,
                          UInt16 transactionId
                          ,InternalCellIndex internalCellIndex);
SInt32 handleUeDeleteReq (UInt8 *msg_p,
                        UInt16 transactionId
                        ,InternalCellIndex internalCellIndex);
void prepareULConfigMsg (
#ifdef TDD_CONFIG
        UInt8  newSf, 
#else
        UInt32 phyDelay,
#endif
        /* SPR 15909 fix start */
        tickType_t currentTick,
        /* SPR 15909 fix start */
        UInt32 systemFrameNum,
        UInt8 subFrameNum,
        InternalCellIndex internalCellIndex        
        );

void initUlConfigReqQueue (InternalCellIndex internalCellIndex);
/*FAPI_4_0_COMPLIANCE*/
FAPI_dlConfigPDUInfo_st *addDciPduToDlConfig(
        FAPI_dlConfigPDUInfo_st *pDlConfigPdu, DCIPduInfo *pDciPdu,
        DciCCEInfo *pDciCceInfo,InternalCellIndex internalCellIndex);
FAPI_dlConfigPDUInfo_st *addDciPduToDlConfigR8(
        FAPI_dlConfigPDUInfo_st *pDlConfigPdu, DCIPduInfo *pDciPdu,
        DciCCEInfo *pDciCceInfo,InternalCellIndex internalCellIndex);
FAPI_dlConfigPDUInfo_st *addDciPduToDlConfigR9(
        FAPI_dlConfigPDUInfo_st *pDlConfigPdu, DCIPduInfo *pDciPdu,
        DciCCEInfo *pDciCceInfo,InternalCellIndex internalCellIndex
);
FAPI_dlConfigPDUInfo_st *addDciPduToDlConfigR10(
        FAPI_dlConfigPDUInfo_st *pDlConfigPdu, DCIPduInfo *pDciPdu,
        DciCCEInfo *pDciCceInfo,InternalCellIndex internalCellIndex);



FAPI_dlConfigPDUInfo_st *addDlschPduToDlConfig(
        FAPI_dlConfigPDUInfo_st *pDlConfigPdu, DCIPduInfo *pDciPdu,
        UInt32 pduIndex, UInt8 tbIndex
        /* SPR 5807 Fix */
        ,UInt8 isOnlyOneTb
        /* SPR 5807 Fix */
	,FAPI_VendorSpecificForDLConfig_st *tempVendorSpecificForDLConfig_p,UInt16 *numOfUEs,
    InternalCellIndex internalCellIndex
        );
FAPI_dlConfigPDUInfo_st *addDlschPduToDlConfigR8(
        FAPI_dlConfigPDUInfo_st *pDlConfigPdu, DCIPduInfo *pDciPdu,
        UInt32 pduIndex, UInt8 tbIndex
        /* SPR 5807 Fix */
        ,UInt8 isOnlyOneTb
        /* SPR 5807 Fix */
	,FAPI_VendorSpecificForDLConfig_st *tempVendorSpecificForDLConfig_p,UInt16 *numOfUEs,InternalCellIndex internalCellIndex
        );
FAPI_dlConfigPDUInfo_st *addDlschPduToDlConfigR9(
        FAPI_dlConfigPDUInfo_st *pDlConfigPdu, DCIPduInfo *pDciPdu,
        UInt32 pduIndex, UInt8 tbIndex
        /* SPR 5807 Fix */
        ,UInt8 isOnlyOneTb
        /* SPR 5807 Fix */
	,FAPI_VendorSpecificForDLConfig_st *tempVendorSpecificForDLConfig_p,UInt16 *numOfUEs,InternalCellIndex internalCellIndex
        );
FAPI_dlConfigPDUInfo_st *addDlschPduToDlConfigR10(
        FAPI_dlConfigPDUInfo_st *pDlConfigPdu, DCIPduInfo *pDciPdu,
        UInt32 pduIndex, UInt8 tbIndex
        /* SPR 5807 Fix */
        ,UInt8 isOnlyOneTb
	,FAPI_VendorSpecificForDLConfig_st *tempVendorSpecificForDLConfig_p,UInt16 *numOfUEs,InternalCellIndex internalCellIndex
        /* SPR 5807 Fix */
        );
/* adding functions */
void prepareUciHarqPdu (
       FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p,InternalCellIndex internalCellIndex
        );
void prepareUciSrHarqPdu (
       FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p,InternalCellIndex internalCellIndex
        );
void prepareUciCqiHarqPdu(
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p,InternalCellIndex internalCellIndex
        );
void prepareUciCqiSrHarqPdu(
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
		ULSchUciInfo * ulSchUciInfo_p,InternalCellIndex internalCellIndex
		);
/*SPR 13698 changes start*/
#ifdef MAC_AUT_TEST
UInt8 missingorInvalidTlv_g[2];
#endif
/*SPR 13698 changes end*/
#ifdef FAPI_4_0_COMPLIANCE
void prepareUciHarqPduR9(
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p,
		InternalCellIndex internalCellIndex
        );
void prepareUciSrHarqPduR9(
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p,
		InternalCellIndex internalCellIndex
        );

void prepareUciCqiHarqPduR9(
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
		ULSchUciInfo * ulSchUciInfo_p,
		InternalCellIndex internalCellIndex
		);
void prepareUciCqiSrHarqPduR9(
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
		ULSchUciInfo * ulSchUciInfo_p,
		InternalCellIndex internalCellIndex
		);
void fillUCIHarqInfoR9(FAPI_fddHarqPduInfoR9AndLater_st *uciHarq_p, ULSchUciInfo * ulSchUciInfo_p);
UInt8 fillCqiRiInfoR9AndLater(FAPI_cqiRiPduInfoRel9AndLater_stT * ulschCqiRi_p,
        ULSchUciInfo * ulSchUciInfo_p);
#endif


	/* adding functions */
MacRetType prepareUlschCqiRiPdu (
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p
        /*SPR 1547 Start*/
        ,UInt8 srsPresent,
        /*SPR 1547 End*/
		InternalCellIndex internalCellIndex
        );
MacRetType prepareUlschCqiRiPduR9(
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p
        /*SPR 1547 Start*/
        ,UInt8 srsPresent,
		/*SPR 1547 End*/
		InternalCellIndex internalCellIndex
        );
MacRetType prepareUlschCqiHarqRiPdu(
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p
        /*SPR 1547 Start*/
        ,UInt8 srsPresent,
        /*SPR 1547 End*/
		InternalCellIndex internalCellIndex
        );
MacRetType prepareUlschCqiHarqRiPduR9(
        FAPI_ulPDUConfigInfo_st *ulPduConf_p,
        ULSchUciInfo * ulSchUciInfo_p
        /*SPR 1547 Start*/
        ,UInt8 srsPresent,
        /*SPR 1547 End*/
        InternalCellIndex internalCellIndex
        );

MacRetType prepareUlschPduR10 (
        FAPI_ulSCHPduInfo_st *ulSchPdu_p,
        ULSchUciInfo * ulSchUciInfo_p
        /*SPR 1547 Start*/
        ,UInt8 srsPresent,
        /*SPR 1547 End*/
		InternalCellIndex internalCellIndex
        );
void fillDciFormat0PduR10(
    #ifdef TDD_CONFIG
        /* coverity 64948 64949 28 June2014 */
        FAPI_dlDCIPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu ,UInt32 dci0ForTempUEFlag, InternalCellIndex
        /* TDD_FAPI_2.4 changes END */
        /* coverity 64948 64949 28 June2014 */
    #elif FDD_CONFIG
        /* +- SPR 17777 */
        FAPI_dlDCIPduInfo_st *pDciMsg, DCIPduInfo *pDciPdu,
#if defined(UTFWK_SIMULATION) || defined(ULHARQ_TIMER_PROC) /*SPR 18979 +-*/
        UInt32 subFrameNum,
#endif
        UInt32 dci0ForTempUEFlag,MsgType msgType
    , InternalCellIndex
    /* +- SPR 17777 */
    #endif
 ); 
 void processULSchInd(void *,
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
		 );

 void processULSchIndR9(void *,
		void *,
		void *,
		UInt32,
		tickType_t,
		UInt32,
		UInt32,
		UInt32,
		InternalCellIndex
		);
void processCQIIndicationR9(void *,
		void *,
                        tickType_t ,
                        /*SPR 5620 START*/
#ifdef TDD_CONFIG
                        UInt16 ,
#endif
                        /*SPR 5620 END*/
                        UInt16 ,
						InternalCellIndex
                        );
/* - TM7_8 Changes End */
void processCQIIndicationR8(void *,
                        void *,
                        tickType_t ,
                        /*SPR 5620 START*/
#ifdef TDD_CONFIG
                        UInt16 ,
#endif
                        /*SPR 5620 END*/
                        UInt16 ,
						InternalCellIndex 
                        );
/* - TM7_8 Changes End */
void fillSRRelease10Param(
FAPI_srPduInfo_st * srPdu_p);
void processSRSIndicationR9(void *,
                          void *,
                          tickType_t,InternalCellIndex );
MacRetType parsePRSConfig(UInt16 bitMask,UInt8 **msg_pp,UInt16 *remLength_p,InternalCellIndex);

void processSRSIndication(void *,
                          void *,
                          tickType_t,InternalCellIndex);
void processRACHIndicationR9( void *,
                        tickType_t ,FAPI_VendorSpecificForRACHInd_st *,InternalCellIndex);

void processRACHIndication( void *,
                        tickType_t ,FAPI_VendorSpecificForRACHInd_st *,InternalCellIndex);

void processUlHarqIndR9( void* ,
             tickType_t ,
             UInt16 ,
         UInt8 ,InternalCellIndex);

void processUlHarqIndR8( void* ,
             tickType_t ,
             UInt16 ,
         UInt8 ,InternalCellIndex);
/* SPR 15909 fix end */
MacRetType prepareUlschPdu(
        FAPI_ulSCHPduInfo_st *ulSchPdu_p,
        ULSchUciInfo * ulSchUciInfo_p
        /*SPR 1547 Start*/
        ,UInt8 srsPresent,
        /*SPR 1547 End*/
		InternalCellIndex
        );

void fillUCIHarqInfo(FAPI_fddHarqPduInfo_st *uciHarq_p, ULSchUciInfo * ulSchUciInfo_p);

/* SPR 15909 fix start */
void processCQIIndication(void *,
                        void *,
                        tickType_t ,
                        /*SPR 5620 START*/
#ifdef TDD_CONFIG
                        UInt16 ,
#endif
                        /*SPR 5620 END*/
                        UInt16,
						InternalCellIndex
                        );

/* - TM7_8 Changes End */
void processUlHarqInd( void* ,
            tickType_t ,
             UInt16 ,
         UInt8 ,InternalCellIndex);
/* SPR 15909 fix end */
/* SPS_TDD Changes + */
/*****************************************************************************
 * Function Name  :  getRntiTypeForRnti
 * Inputs         :  rnti - RNTI whose type is to required.
 * Outputs        :  None
 * Returns        :  rntiType.
 * Description    :  This fucntion returns the rnti type.
 *****************************************************************************/
 UInt8 getRntiTypeForRnti(UInt16 rnti, InternalCellIndex internalCellIndex);
/* SPS_TDD Changes - */

#ifdef FLEXRAN

#define IPC_SEMAPHORE

typedef enum
{
	FAPI_SHMA_CH_PARA_REQ = 0,
	FAPI_SHMA_CH_CFG_REQ,
	FAPI_SHMA_CH_START_REQ,
	FAPI_SHMA_CH_STOP_REQ,
	FAPI_SHMA_CH_DL_CFG_REQ,
	FAPI_SHMA_CH_UL_CFG_REQ,
	FAPI_SHMA_CH_HI_DCI0,
	FAPI_SHMA_CH_TX_REQ,
	FAPI_SHMA_CH_PARA_RSP,
	FAPI_SHMA_CH_CFG_RSP,
	FAPI_SHMA_CH_STOP_IND,
	FAPI_SHMA_CH_ERROR_IND,
	FAPI_SHMA_CH_SUBFRAME_IND,
	FAPI_SHMA_CH_HARQ_IND,
	FAPI_SHMA_CH_CRC_IND,
	FAPI_SHMA_CH_RX_ULSCH_IND,
	FAPI_SHMA_CH_RACH_IND,
	FAPI_SHMA_CH_SRS_IND,
	FAPI_SHMA_CH_RX_SR_IND,
	FAPI_SHMA_CH_RX_CQI_IND,
	FAPI_SHMA_DL_BUFFER,
	FAPI_SHMA_UL_BUFFER,	
	FAPI_SHMA_BBU_PKG_IN, // for L1D test
	FAPI_SHMA_BBU_PKG_OUT,// for L1D test
	FAPI_SHMA_CH_END,
} FAPI_SHMA_CH_T;	

#define IPC_READY_MAGIC_NUMBER 0x6970637265616479 //ASCII "IPCREADY"

//#define MAX_CELL_NUM 16
#define FLEXRAN_MAX_CELL_NUM 2

#define IPC_QUEUE_NUMBER FAPI_SHMA_CH_END
#define IPC_USED_BY_PHY 0
#define IPC_USED_BY_MAC 1

#define FLEXRAN_MAX_QUEUE_SIZE 16
#define FLEXRAN_MAX_MSG_SIZE 3072
#define MAX_BBU_PKG_SIZE 9000  // for L1D test

#define FLEXRAN_DL_BUFFER_SIZE 0x1000000 // 16MB
#define FLEXRAN_UL_BUFFER_SIZE 0x1000000 // 16MB

#define IPC_SUCCESS (0)
#define IPC_NOT_INIT_ERR (-1)
#define IPC_INPUT_PARA_ERR (-2)
#define IPC_SEND_QUEUE_FULL_ERR (-3)
#define IPC_SEND_EDQUOT_ERR (-4)
#define IPC_SEND_ENOBUFS_ERR (-5)
#define IPC_RECV_BUF_ERR (-6)
#define IPC_UNKNOWN_ERR (-7)

typedef struct
{
	char* name;
	unsigned int counter;
	unsigned int size;
}ipc_queue_s;

typedef struct
{
	/*write index, can only be updated by sending side after sending a message*/
	unsigned int nWriteIdx;
	
	/*read index, can only be updated by receiving side after received a message,
		no messages when nWriteIdx = nReadIdx*/
	unsigned int nReadIdx;
	
	/*in each message, the 1st 4 byte is length, remaining bytes are payload*/
	/* Variable length and payload field:
		Each PDUData block includes:
			1)UWORD32 nLen: message length
			2)UWORD8 pMsg[nLen]: message payload
			
			|-|---|-|-|---|-------|---|-------------|---------------|
Byte 		|0|...|3|4|...|3+nLen|....| ... ...     |ring counter-1|
			|--------|------------|---|-----------------------------|
pMsgBuf 	|nLen- |ringSize |.....	| ... ... |
			|--------|------------|---|-----------------------------|
			|---No.1 message ------|.... |---No.(ring counter-1) message--|*/
	unsigned char pMsgBuf[1];
	
}memory_header_s;

typedef struct
{
	unsigned short maxQueueNumPerQueue[IPC_QUEUE_NUMBER];
	unsigned long totalBytesPerQueue[IPC_QUEUE_NUMBER];
	unsigned int errorNumPerQueue[IPC_QUEUE_NUMBER];
}ipcStatisticStruct;

extern int flexran_ipc_init(unsigned int cell_num,unsigned char type, int mac_cell_id);
extern void flexran_ipc_exit();
extern int flexran_ipc_is_channel_ready(unsigned int cell_id, FAPI_SHMA_CH_T queue_id, char* buf, unsigned int len);
extern int flexran_ipc_send_msg(unsigned int cell_index, FAPI_SHMA_CH_T ipcId, char* buf, unsigned int len);
extern int flexran_ipc_recv_msg(unsigned int cell_index, FAPI_SHMA_CH_T ipcId, char* buf, unsigned int len);
extern int flexran_ipc_set_statistic(ipcStatisticStruct* data);

extern char* flexran_ipc_get_dl_databuffer(unsigned int cell_index);
extern char* flexran_ipc_get_ul_databuffer(unsigned int cell_index);

extern char g_txBBUBuf[MAX_BBU_PKG_SIZE];

#ifdef IPC_SEMAPHORE
extern int flexran_ipc_notify_mac_ready(unsigned int cell_index);
extern int flexran_ipc_notify_phy_ready(unsigned int cell_index);

extern int flexran_ipc_timewait_phy_ready(unsigned int cell_index, const struct timespec *timeout);
extern int flexran_ipc_timewait_mac_ready(unsigned int cell_index, const struct timespec *timeout);
extern int flexran_ipc_wait_phy_ready(unsigned int cell_index);
extern int flexran_ipc_wait_mac_ready(unsigned int cell_index);
extern int flexran_ipc_trywait_phy_ready(unsigned int cell_index);
extern int flexran_ipc_trywait_mac_ready(unsigned int cell_index);
#endif

#endif


#endif /* LTE_MAC_PHY_INTF_FF */
