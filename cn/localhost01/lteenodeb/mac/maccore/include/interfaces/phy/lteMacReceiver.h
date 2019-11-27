/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacReceiver.h,v 1.1.1.1.6.1.6.1 2010/09/27 14:37:15 gur10694 Exp $
 *  $Id: lteMacReceiver.h,v 1.1.1.1.6.1.6.1 2010/09/27 14:37:15 gur10694 Exp $
 *  $Id: lteMacReceiver.h,v 1.1.1.1.6.1.6.1 2010/09/27 14:37:15 gur10694 Exp $
 *  $Id: lteMacReceiver.h,v 1.1.1.1.6.1.6.1 2010/09/27 14:37:15 gur10694 Exp $
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
 * $Log: lteMacReceiver.h,v $
 * Revision 1.1.1.1.6.1.6.1  2010/09/27 14:37:15  gur10694
 * UE config Fix and Review comments
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.32  2009/12/22 05:09:01  gur11912
 * modified ttiCounter to packetSubFrame
 *
 * Revision 1.31  2009/08/28 12:55:33  gur19413
 * processOnlySubFrameInd modified for subframe handling
 *
 * Revision 1.30  2009/08/24 12:52:52  gur11912
 * macPhyRegistrationSuccessful_g declared as volatile
 *
 * Revision 1.29  2009/07/30 10:56:46  gur11912
 * changed BUFFLEN to 20000
 *
 * Revision 1.28  2009/07/28 12:33:52  gur19413
 * cqi related macro value changed.
 *
 * Revision 1.27  2009/07/24 06:52:39  gur19413
 * added new func prototype processOnlySubFrameInd
 *
 * Revision 1.26  2009/07/22 04:36:54  gur19413
 * changed the macro value MAX_WIDEBANDC1_VALUE_1_1_TYPE_2 15
 *
 * Revision 1.25  2009/07/15 06:45:23  gur19413
 * added new macro MAX_RI_VALUE
 *
 * Revision 1.24  2009/07/13 04:35:24  gur19413
 * added new macro for log
 *
 * Revision 1.23  2009/07/07 05:07:19  gur19413
 * convert the file dos2Unix format
 *
 * Revision 1.22  2009/07/02 07:45:57  gur19413
 * modify the CQI Structures.
 *
 * Revision 1.21  2009/06/23 11:17:33  gur19413
 * added extern SR Queue
 *
 * Revision 1.20  2009/06/22 05:02:19  gur19413
 * added new Macro PERIODIC_CQI & APERIODIC CQI
 *
 * Revision 1.19  2009/06/19 10:36:04  gur19413
 * function name change processRxFrameInd to processRxSubFrameInd
 *
 * Revision 1.18  2009/06/19 06:45:52  gur18550
 * ttiCounter in AckNackQueueNode
 *
 * Revision 1.17  2009/06/18 14:11:32  gur19413
 * change the type of operation field in dl Ack Nck
 *
 * Revision 1.16  2009/06/18 09:13:37  gur15697
 * lowerCRNTIBound_g and upperCRNTIBound_g modified to  lowerRNTIBound_g and upperRNTIBound_g
 *
 * Revision 1.15  2009/06/16 15:18:54  gur12140
 * Added UeIndex Parameter
 *
 * Revision 1.14  2009/06/10 13:25:56  gur19413
 * comments incorporated
 *
 * Revision 1.12  2009/06/09 12:33:44  gur19413
 * renamed function name
 *
 * Revision 1.13  2009/06/09 13:12:41  gur18569
 * incorporatd code changes
 *
 * Revision 1.12  2009/06/09 12:33:44  gur19413
 * renamed function name
 *
 * Revision 1.11  2009/06/08 14:27:31  gur19413
 * remove macro id
 *
 * Revision 1.10  2009/06/05 10:32:10  gur19413
 * added acknack structure node
 *
 * Revision 1.9  2009/06/04 10:11:26  gur19413
 * remove compilation error
 *
 * Revision 1.8  2009/06/04 09:44:00  gur19413
 * added new macro
 *
 * Revision 1.7  2009/06/03 16:46:36  gur18550
 * File Updated
 *
 * Revision 1.5  2009/05/29 10:21:38  gur18569
 * removed compilation error
 *
 * Revision 1.4  2009/05/29 05:27:23  gur19413
 * remove ,
 *
 * Revision 1.3  2009/05/25 22:51:16  gur19413
 * added new macro and functions prototypes
 *
 * Revision 1.2  2009/05/22 12:27:43  gur19413
 * Initial Version
 *
 * Revision 1.1  2009/05/21 12:45:34  gur19413
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_RECEIVER_H
#define LTE_MAC_RECEIVER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacPhyInterface.h"
#include "lteMacRandomAccess.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define MAX_SUB_FRAME_NUM   9
/*UL_MU_MIMO_CHG Start */
#define BUFFLEN             SH_MESSAGE_UL_BUF_SIZE
/*UL_MU_MIMO_CHG End */
#define MAX_RA_TA_VALUE     1282
/*SPR 21001 Changes end */
/* SPR 19288 change start */
/* SPR 19288 change end */

/***********************************************************************/

#define RECV_INTF         "Receiver Interface"

/* for check Aperiodic or Periodic Mode */
#define PERIODIC_MODE           1
#define APERIODIC_MODE          0
/* Mandatory Length use for Validation */

/* + CQI_4.1 */
#define MIN_CQI_INDEX       1
#define INVALID_CQI_INDEX   0
#define WIDEBAND_SPATIAL_DIFFERENTIAL_CQI_AT_INDEX_3       3
#define WIDEBAND_SPATIAL_DIFFERENTIAL_CQI_AT_INDEX_4       4
#define HIGHER_LAYER_SUBBAND_DIFFERENTIAL_CQI_AT_INDEX_2   2
#define HIGHER_LAYER_SUBBAND_DIFFERENTIAL_CQI_AT_INDEX_3   1
#define UE_SELECTED_DIFFERENTIAL_CQI_AT_INDEX_0            1
#define UE_SELECTED_DIFFERENTIAL_CQI_AT_INDEX_3            4
#define DL_ADAPTATION_CORRECTION 0
/* - CQI_4.1 */

/* APERIODIC CQI MODE */
/****** APERIODIC_CQI_MODE_2_0 *********/

#define MIN_MANDATORY_LEN_ACQI_20       8
#define MAX_LEN_ACQI_20                 32

#define MAX_WIDEBAND_CQI_VALUE          15 /* Any value among 0 - 15*/
#define MAX_SUBBAND_CQI_VALUE           15 /* Any value among 0 - 15*/
#define MAX_NUM_OF_SUBBANDS_ACQI_20     25 /* Any value among 1 - 25*/ 
/* For Subband Info */
#define MAX_SUBBAND_NUM_ACQI_20         24 /* Any value among 0 - 24*/

/****** APERIODIC_CQI_MODE_3_0 *********/

#define MIN_MANDATORY_LEN_ACQI_30       8 
#define MAX_MANDATORY_LEN_ACQI_30       32

#define MAX_WIDEBAND_VALUE_ACQI_30      15 /* Any value among 0 - 15*/ 
#define MAX_NUM_SUBBAND_ACQI_30         13 /* Any value among 1 - 13*/
/* For Subband Info */
#define MAX_SUBBAND_NUM_ACQI_30         12 /* Any value among 0 - 12*/
#define MAX_CQI_INDEX_ACQI_30           15 /* Any value among 0 - 15*/

/****** APERIODIC_CQI_MODE_3_1 *********/

#define MIN_MANDATORY_LEN_ACQI_31       8
#define MAX_MANDATORY_LEN_ACQI_31       47

#define MAX_WIDEBANDC0_VALUE_ACQI_31    15 /* Any value among 0 - 15*/    
#define MAX_WIDEBANDC1_VALUE_ACQI_31    15 /* Any value among 0 - 15*/
#define MAX_WIDEBAND_PMI_VALUE_ACQI_31  4 
/* For Subband Info */
#define MAX_SUBBAND_NUM_ACQI_31         12 /* Any value among 0 - 12*/
#define MAX_SUBBANDCQIC0_VALUE_ACQI_3   15 /* Any value among 0 - 15*/
#define MAX_SUBBANDCQIC1_VALUE_ACQI_3   15 /* Any value among 0 - 15*/

/****** APERIODIC_CQI_MODE_1_2 *********/
#define MIN_MANDATORY_LEN_ACQI_12       7
#define MAX_MANDATORY_LEN_ACQI_12       33

#define MAX_CQIVALUEC0_ACQI_12          15 /* Any value among 0 - 15*/
#define MAX_CQIVALUEC1_ACQI_12          15 /* Any value among 0 - 15*/
#define MAX_NUM_SUBBAND_VALUE_ACQI_12   13 /* Any value among 1 - 13*/
#define MAX_SUBBAND_VALUE_ACQI_12       12 /* Any value among 0 - 12*/  
#define MAX_PMI_VALUE_ACQI_12            4 /*1, 2 or 4*/ 

/****** APERIODIC_CQI_MODE_2_2 *********/
#define MIN_MANDATORY_LEN_ACQI_22       11
#define MAX_MANDATORY_LEN_ACQI_22       35

#define MAX_WIDEBANDC0_VALUE_ACQI_22    15  /* Any value among 0 - 15*/
#define MAX_WIDEBANDC1_VALUE_ACQI_22    15  /* Any value among 0 - 15*/
#define MAX_CQIVALUEC0_VALUE_ACQI_22    15  /* Any value among 0 - 15*/
#define MAX_CQIVALUEC1_VALUE_ACQI_22    15  /* Any value among 0 - 15*/
#define MAX_PMI_VALUE_ACQI_22           4   
#define MAX_NUM_SUBBANDS_VALUE_ACQI_22  25  /* Any value among 1 - 25*/
#define MAX_SUBBAND_VALUE_ACQI_22       24  /* Any value among 0 - 24*/

/***********************************/

/* PERIODIC CQI MODE*/

#define MAX_RI_VALUE                         2
#define INVALID_WIDEBAND_PMI_3_VALUE         3
/****** PERIODIC_CQI_MODE_1_0_TYPE_3 *********/

#define MANDATORY_LEN_CQI_MODE_1_0_TYPE_3   5 

/****** PERIODIC_CQI_MODE_1_0_TYPE_4 *********/

#define MANDATORY_LEN_CQI_MODE_1_0_TYPE_4   5
#define MAC_WIDEBAND_VALUE_1_0_TYPE_4       15  /* Any value among 0 - 15*/ 

/****** PERIODIC_CQI_MODE_1_1_TYPE_2 *********/

#define MANDATORY_LEN_CQI_MODE_1_1_TYPE_2   7
#define MAX_WIDEBANDC0_VALUE_1_1_TYPE_2     15
#define MAX_WIDEBANDC1_VALUE_1_1_TYPE_2     15  /* Valid when RI >1*/
#define MAX_WIDEBANDPMI_VALUE_1_1_TYPE_2    4  /* 3 is not valid value*/

/****** PERIODIC_CQI_MODE_1_1_TYPE_3 *********/

#define MANDATORY_LEN_CQI_MODE_1_1_TYPE_3   5

/****** PERIODIC_CQI_MODE_2_0_TYPE_1  *********/

#define MANDATORY_LEN_CQI_MODE_2_0_TYPE_1   7
#define MAX_BANDWIDTHPART_VALUE_2_0_TYPE_1  4 /* 1,2,3,4 depending on the Bandwidth*/
#define MAX_LBITLABEL_VALUE_2_0_TYPE_1      4 /* 1,2,3,4 Position of the M selected subbands. */
#define MAX_CQI_VALUE_2_0_TYPE_1            15 /* Any value among 0 - 15*/

/****** PERIODIC_CQI_MODE_2_0_TYPE_3  *********/

#define MANDATORY_LEN_CQI_MODE_2_0_TYPE_3   5
#define MAX_RI_VALUE_2_0_TYPE_3             2 

/****** PERIODIC_CQI_MODE_2_0_TYPE_4  *********/

#define MANDATORY_LEN_CQI_MODE_2_0_TYPE_4   5
#define MAX_WIDEBAND_VALUE_2_0_TYPE_4       15  /* Any value among 0 - 15*/

/****** PERIODIC_CQI_MODE_2_1_TYPE_1  *********/

#define MANDATORY_LEN_CQI_MODE_2_1_TYPE_1   8 
#define MAX_BANDWIDTHPART_VALUE_2_1_TYPE_1  4 /* 1,2,3,4 depending on the Bandwidth*/
#define MAX_LBITLABEL_VALUE_2_1_TYPE_1      4  
#define MAX_CQIVALUEC0_VALUE_2_1_TYPE_1     15 /* Any value among 0 - 15*/ 
#define MAX_CQIVALUEC1_VALUE_2_1_TYPE_1     15 /* Any value among 0 - 15*/

/****** PERIODIC_CQI_MODE_2_1_TYPE_2  *********/

#define MANDATORY_LEN_CQI_MODE_2_1_TYPE_2   7
#define MAX_WIDEBANDC0_VALUE_2_1_TYPE_2     15 /* Any value among 0 - 15*/
#define MAX_WIDEBANDC1_VALUE_2_1_TYPE_2     15 /* Any value among 0 - 15*/
#define MAX_WIDEBANDPMI_VALUE_2_1_TYPE_2    4  /* value 3 is not valid */

/****** PERIODIC_CQI_MODE_2_1_TYPE_3  *********/

#define MANDATORY_LEN_CQI_MODE_2_1_TYPE_3   5  
/*****************************************************************/

/* 
Depending on Configured RBs 
1 (For DL RBs 8-10),
3 (For DL RBs 11-26),
5 (For DL RBs 27-63),
6 (For DL RBs 64 . 110)
*/
#define MAX_VALUE_SUBBAND_INFO_20     6
#define MAX_VALUE_SUBBAND_INFO_22     6
/*********************************************************************/
#define MAX_VALUE_SUBBAND_INFO_30     13 
#define MAX_VALUE_SUBBAND_INFO_31     13 
#define MAX_VALUE_SUBBAND_INFO_12     13 
/*
   For MACRO "ULSCH_IND_START_OF_PDU_OFFSET", the offset is calculated as below:
   (For struct FAPI_l1ApiMsg_st)
   FAPI_msgIdType_t     msgId;  -> 1 byte
   uint8_t              lenVendorSpecific; -> 1 byte
   uint16_t             msgLen; -> 2 bytes
   ( For struct FAPI_rxULSCHIndication_st)
   uint16_t               sfnsf; -> 2 bytes
   uint16_t               numOfPdu; -> 2 bytes
 */
#define ULSCH_IND_START_OF_PDU_OFFSET   (1 + 1 + 2 + 2 + 2)
 /*+ E_CID_5.3*/
#define MAX_TIMING_ADVANCE_R9_VALUE 7690      
#define MAX_ANGLE_OF_ARRIVAL_VALUE 719
#define INVALID_TIMING_ADVANCE_R9_VALUE 0xFFFF
#define INVALID_ANGLE_OF_ARRIVAL_VALUE 0xFFFF 
 /*- E_CID_5.3*/
 /**/
/* SPR 15322 fix start */
#ifdef FAPI_4_0_COMPLIANCE
#ifdef FDD_CONFIG
#define HARQ_IND_CONST_LEN_R9 4
#endif
#define SRS_PDU_CONST_LEN_R9 16
#endif
#define SRS_PDU_CONST_LEN 12
/* SPR 15322 fix end */

typedef enum ErrorCodeT
{
   MSG_OK = 1,
   MSG_DECODE_ERROR,
   MSG_TOO_LATE
}ErrorCode;

#ifdef TDD_CONFIG
/* + CA_TDD_HARQ_CHANGES */
/*SPR 17466 +-*/
typedef enum TddHARQSentModeR9T
{
    HARQ_BUNDLING_MODE = 0,
    HARQ_MULTIPLEXING_MODE = 1,
    HARQ_SPECIAL_BUNDLING_MODE = 2,
    HARQ_CHANNEL_SELECTION_MODE = 3,
    HARQ_FORMAT_THREE_MODE = 4,
    HARQ_INVALID_TYPE 
}TddHARQSentModeR9;
/*SPR 17466 +-*/
typedef enum TddHarqMValT
{
   HARQ_MULTIPLEX_ONE = 1,
   HARQ_MULTIPLEX_TWO = 2,
   HARQ_MULTIPLEX_THREE = 3,
   HARQ_MULTIPLEX_FOUR= 4,
   INVALID_HARQ_MULTIPLEX
   
}TDDHarqMVal;
/* - CA_TDD_HARQ_CHANGES */
#endif

typedef struct AperiodicCQIMode20T
{
  UInt8 widebandCQIValue;
  UInt8 subbandCQIValue;
  UInt8 numberOfSubbands;
  /* MAX_VALUE_SUBBAND_INFO_20         6*/
  /* + CQI_4.1 */
  UInt8 subbandInfo20[MAX_VALUE_SUBBAND_INFO_20];
 /* - CQI_4.1 */
}AperiodicCQIMode20;

/*********************************************/

typedef struct AperiodicCQIMode30T
{
   UInt8 widebandCQIValue;
   UInt8 numberOfSubbands;
   /* MAX_VALUE_SUBBAND_INFO_30    13 */
   /* + CQI_4.1 */
   UInt8 subbandCQIInfo30[MAX_VALUE_SUBBAND_INFO_30];
   /* - CQI_4.1 */
}AperiodicCQIMode30;

/********************************************/
typedef struct SubBandInfo31T
{
    UInt8 subbandCQIValueCodeword0;
    UInt8 subbandCQIValueCodeword1;
}SubBandInfo31;
typedef struct aperiodicCQIMode31T
{
   UInt8 widebandCQIValueCodeword0;
   UInt8 widebandCQIValueCodeword1;
   UInt8 widebandPMI;
   UInt8 numberOfSubbands;
   /* MAX_VALUE_SUBBAND_INFO_31       13 */
   SubBandInfo31 subbandInfo31[MAX_VALUE_SUBBAND_INFO_31];
   /* + CQI_5.0 */
   UInt8  receivedRI;
   /* - CQI_5.0 */
}AperiodicCQIMode31;

/******************************************/

typedef struct AperiodicCQIMode12
{
   UInt8 widebandCQIValueCodeword0;
   UInt8 widebandCQIValueCodeword1;
   UInt8 numberOfSubbands;
   /* MAX_VALUE_SUBBAND_INFO_12       13 */
   /* + CQI_4.1 */
   UInt8 pmiInfo12[MAX_VALUE_SUBBAND_INFO_12];
   /* - CQI_4.1 */
   /* + CQI_5.0 */
   UInt8  receivedRI;
   /* SPR 15909 fix start */
   tickType_t reportReceivedTTI;
   /* SPR 15909 fix end */
   /* - CQI_5.0 */
}AperiodicCQIMode12;

/***************************************/
typedef struct AperiodicCQIMode22T
{
   UInt8 widebandCQIValueCodeword0;
   UInt8 widebandCQIValueCodeword1;
   UInt8 cqiValueCodeword0;
   UInt8 cqiValueCodeword1;
   UInt8 numberOfSubbands;
   /* MAX_VALUE_SUBBAND_INFO_22 6*/
   /* + CQI_4.1 */
   UInt8 widebandPMIValue;
   UInt8 subbandPMIValue;
   UInt8 subbandInfo22[MAX_VALUE_SUBBAND_INFO_22];
   /* - CQI_4.1 */
   /* + CQI_5.0 */
   UInt8  receivedRI;
   /* - CQI_5.0 */
}AperiodicCQIMode22;

 
/**************************************
*        Peroidic Reports
*************************************/

typedef struct PeriodicMode10Type3T
{
    UInt8 riValue;

}PeriodicMode10Type3;

typedef struct PeriodicMode10Type4T
{
   UInt8 widebandCQIValueCW0;
   UInt8 widebandCQIValueCW1;

}PeriodicMode10Type4;


typedef struct PeriodicMode20Type1T
{
   UInt8 bandwidthPart;
   UInt8 lBitLabel;
   UInt8 cqiValue;
}PeriodicMode20Type1;

typedef struct PeriodicMode20Type3T
{
   UInt8 riValue;

}PeriodicMode20Type3;

typedef struct PeriodicMode20Type4T
{
    UInt8 widebandCQIValueCW0;
    UInt8 widebandCQIValueCW1;

}PeriodicMode20Type4;


typedef struct PeriodicMode11Type2T
{
   UInt8 widebandCQIValueCodeword0;
   UInt8 widebandCQIValueCodeword1;
   UInt8 widebandPMI;
}PeriodicMode11Type2;

typedef struct PeriodicMode11Type3T
{
 UInt8 riValue;
}PeriodicMode11Type3;


typedef struct PeriodicMode21Type1T
{
   UInt8 bandwidthPart; 
   UInt8 lBitLabel;
   UInt8 cqiValueCodeword0;
   UInt8 cqiValueCodeword1; 
}PeriodicMode21Type1;


typedef struct PeriodicMode21Type2T
{
  UInt8 widebandCQIValueCodeword0;
  UInt8 widebandCQIValueCodeword1;
  UInt8 widebandPMI;
}PeriodicMode21Type2;


typedef struct PeriodicMode21Type3T
{
  UInt8 riValue;
}PeriodicMode21Type3;

typedef union CQINodeT
{
 AperiodicCQIMode20 aperiodicCQIMode20;
 AperiodicCQIMode30 aperiodicCQIMode30;
 AperiodicCQIMode31 aperiodicCQIMode31; 
 AperiodicCQIMode12 aperiodicCQIMode12;
 AperiodicCQIMode22 aperiodicCQIMode22;
 PeriodicMode10Type3 periodicMode10Type3;
 PeriodicMode10Type4 periodicMode10Type4;
 PeriodicMode20Type1 periodicMode20Type1;
 PeriodicMode20Type3 periodicMode20Type3;
 PeriodicMode20Type4 periodicMode20Type4;
 PeriodicMode11Type2 periodicMode11Type2;
 PeriodicMode11Type3 periodicMode11Type3;
 PeriodicMode21Type1 periodicMode21Type1;
 PeriodicMode21Type2 periodicMode21Type2;
 PeriodicMode21Type3 periodicMode21Type3;
}CQINode;


typedef struct UEReportedCQINodeInfoT
{
    LTE_SNODE   nodeAnchor; 
    PHYTagIndentifier mode;
    UInt16 ueIndex;
    CQINode cqiNode;
    UInt32 subframe;
    UInt32 systemFrame;
}UEReportedCQINodeInfo;
/**************************************/
/*WithFreqSelective*/

typedef struct WidebandReportT
{
    UInt8 widebandCodeword0;
    UInt8 widebandCodeword1;
    UInt8 widebandPMI;
    UInt8 riValue;
}WidebandReport;

/**************************************/


#ifdef FDD_CONFIG

    /* CA_phase2_harq_code Start */
typedef struct AckNackQueueNodeT
{
    /*data type of anchor*/
    LTE_SNODE ackNackAnchor;
    /* SPS_CHG  */
    UInt32 schUsed;
    /* SPS_CHG  */
    /* SPR 15909 fix start */
    tickType_t packetSubFrame;
    /* SPR 15909 fix end */
    /* 32.425 Events code changes start*/
#ifdef KPI_STATS
   /* SPR 15909 fix start */
    tickType_t recvTTI;
   /* SPR 15909 fix end */
#endif
    /* 32.425 Events code changes end*/
    /*Index of UE in ueDLContextArr_g*/
    UInt16 ueIndex;
    /*identification of ACK or NACK*/
    UInt16 operationType;
    /*SPR_3305_FIX_START*/
    UInt8 tcRntiFlag;
    /*SPR_3305_FIX_END*/
    /* + SPR_15971 */
    UInt8 mode; 
    /* - SPR_15971 */
}AckNackQueueNode;
    /* CA_phase2_harq_code End */
#endif


#ifdef TDD_CONFIG
typedef struct tddAckNackQueueNodeT
{
    /*data type of anchor*/
    LTE_SNODE ackNackAnchor;
    /*Index of UE in ueDLContextArr_g*/
    UInt16 ueIndex;
    /*identification of ACK or NACK*/
    /* + CA_TDD_HARQ_CHANGES */
    UInt16 operationType;
    /* - CA_TDD_HARQ_CHANGES */
    UInt32 packetSubFrame;
    UInt8 harqProcessId[MAX_NUM_CELL];
    UInt8 modeType;
    /* 100MS_SUPPORT : start */
#ifdef KPI_STATS
   /* SPR 15909 fix start */
    tickType_t recvTTI;
   /* SPR 15909 fix end */
#endif
    /* 100MS_SUPPORT : end */
	/* + SPS_TDD_Changes */
	UInt32 schUsed;
	/* - SPS_TDD_Changes */
    UInt8 indexSfn;
    /*EICIC +*/
    UInt32 dlSubframeNum;
    /*EICIC -*/
}tddAckNackQueueNode;
#endif
/*+ E_CID_5.3*/
typedef struct TempTimingAdvandeR9T
{
    /* +- SPR 18268 */
    UInt16 ueIndex;
    /* +- SPR 18268 */
    UInt16 type2TAValue;
    UInt16 type2TAreceivedTTI;
}TempTimingAdvandeR9;
/*- E_CID_5.3*/

/*  empty_ul_sf change end*/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#ifdef FDD_CONFIG
#define MAX_CONTAINER_NUM 8
#endif
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/*CA Changes start  */
extern DL_SCHEDULER_REPORT_QUEUE_TYPE* dlSchedulerReportQueue_gp[MAX_NUM_CELL];
/*CA Changes end  */

/*Added for Power Control*/
extern TempCRNTICtxInfo* tempCrntiCtx_gp[MAX_NUM_CELL];
#ifdef FDD_CONFIG

/*Queue for sending CQI to DL*/
/*Registration with PHY Layer*/
extern volatile UInt32 macPhyRegistrationSuccessful_g[MAX_NUM_CELL];
#endif

/* Downlink Ack Nack Queue */
#ifdef FDD_CONFIG
#ifdef CIRC_QUEUE_IMPL
/*CA Changes start  */
extern ACK_NACK_CIRC_TYPE* ackNackCircQueue_gp[MAX_NUM_CELL];
#else
extern LTE_SQUEUE* ackNackQueue_gp[MAX_NUM_CELL];
#ifndef DL_UL_SPLIT
/* + SPR 17733 */
extern LTE_SQUEUE* ackNackQueueForTcrnti_gp[MAX_NUM_CELL];
/* - SPR 17733 */
#endif
/*CA Changes end  */
#endif
#elif TDD_CONFIG
/** CA-TDD Changes Start:06 **/
extern LTE_SQUEUE tddAckNackQueue_g[MAX_NUM_CELL];
/** CA-TDD Changes End:06 **/
#endif

/*CA Changes start  */
extern LTE_SQUEUE* ulGrantGlobalSrQueue_gp[MAX_NUM_CELL];
/*CA Changes end  */
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* This function initialize the queue's and called by initPHYInterface. */
/*CA Changes start  */
void initPHYReceiver(UInt8 numCellsConfigured);

#ifdef FDD_CONFIG

/* This function is used to register the MAC Layer with PHY Layer and
   called by execution Scheduler. */
void  recvRegistrationCNFFromPhy(InternalCellIndex internalCellIndex);
/*CA Changes end  */

#endif
/* This function is used to receive the buffer from the phy layer
   until the subframe indication received and called by execution 
   scheduler. */
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
MacRetType processRxSubFrameInd (UInt32 dlDelay, 
                                 UInt32 threadIndex,
                                 /*CA Changes start  */
                                 InternalCellIndex  nternalCellIndex 
                                 /*CA Changes end  */
                                );
#else
void processRxSubFrameInd (UInt32 dlDelay, 
                           UInt32 threadIndex,
                           /*CA Changes start  */
                           InternalCellIndex  internalCellIndex
                           /*CA Changes end  */
                          );
#ifdef DL_UL_SPLIT_TDD
void processRxSubFrameIndUl( UInt32 dlDelay,
        UInt32 threadIndex
        );
void processRxFirstSubFrameIndUl(void);
#endif
#endif
/* SPR 609 changes start */  

#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
void lteMacUpdateStatsPerfStart(UInt32 maxStats, InternalCellIndex internalCellIndex);
#else
void lteMacUpdateStatsPerfStart(InternalCellIndex internalCellIndex);
#endif

/* CA Stats Changes Start */
#ifdef DL_DATA_SEPARATION
void lteMacUpdateDLDataSepStatsPerfStart( InternalCellIndex internalCellIndex);
#endif
/* CA Stats Changes End */

void resetHandleVal (InternalCellIndex  cellIndex);
/* + SPR 17439 */
void processUeConfigMsgQueue (void);
/* - SPR 17439 */
#ifdef TDD_CONFIG
 MacRetType ulHarqTimerStop(ULUEContext* ulUEContext_p,
                                  /* SPR 15909 fix start */
                                  tickType_t  recvTTI,
                                  /* SPR 15909 fix end */
                                  /*CA Changes start  */
                                  InternalCellIndex  cellIndex,
                                  /*TDD Config 0 Changes Start*/ 
                                  UInt8   ulSubframe
                                  /*TDD Config 0 Changes end*/
                                  );
                                  /*CA Changes end  */
#endif
/* SPR 609 changes end */    
#ifdef UTFWK_SIMULATION
/*CA Changes start  */
/* SPR 15909 fix start */
void crcAckNackHandling( UInt32 crcAckNack,UInt32 ueIndex,tickType_t recvTTI,UInt32 ulContainerIndex,UInt32 rntiType, InternalCellIndex cellIndex);
void putEntryInAckNackQueue( UInt32 ueIndex,UInt32 operation,tickType_t recvTTI, InternalCellIndex cellIndex, UInt32 rntiFlag);
/*CA Changes end  */
#endif

/*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
 void updateHDUeSubFrameCntxtInfo(tickType_t tick);
/* SPR 15909 fix end */
#endif
/*HD FDD Changes End*/
/* FAPI L2 AS MASTER START */
/* This function creates a new ackNackNode and inserts this node in the
 * ackNackQueue.*/
/* SPR 609 changes start */ 
#ifdef FDD_CONFIG
#ifndef UTFWK_SIMULATION
/* S */
 void putEntryInAckNackQueue( UInt32 ueIndex, 
                                    UInt16 operation,
                                    /* SPR 15909 fix start */
                                    tickType_t recvTTI,
                                    UInt32 rntiFlag
                                    /* 32.425 Events code changes start*/
                                    /* +- SPR 17777 */
#ifndef DL_UL_SPLIT
#ifdef KPI_STATS
                                    ,tickType_t rcvTTI
                                    /* SPR 15909 fix end */
                                    /* +- SPR 17777 */
#endif
#endif
                                    ,InternalCellIndex internalCellIndex
                                    /* 32.425 Events code changes end*/
                                    /* + SPR_15971 */
                                    , UInt8 mode
                                    /* - SPR_15971 */

                                         );
/* SPS_CHG */
#endif

 /* + SPR_15971 */
typedef enum HarqSentModeR9T
{
    HARQ_MODE_1A = 0,
    HARQ_MODE_1B = 1,
    HARQ_INVALID_TYPE
}HarqSentModeR9;
 /* - SPR_15971 */
#endif
/* SPR 609 changes end */  
/* FAPI L2 AS MASTER END */

#endif  /* LTE_MAC_RECEIVER_H */
/*+ E_CID_5.3*/
/*This macro is used to fetch E_CID reports and findout the valid reports
 * the first 3 bits from LSB of validReport are used to find valid reports
 *
 * Bit 0 Indicates invalid report
 * Bit 1 Indicates valid reports
 *
 * validReports from LSB bit 1:TA type 1
 *                       bit 2:TA type 2
 *                       bit 3:AOA
 *               */
#define VALIDATE_AND_UPDATE_TA1_AOA_REPORTS(ueMeasReport_p,timingAdvanceR9,\
                                             angleOfArrival,validReports,count)\
{\
    UInt8 i;\
    UInt8 flag = FALSE;\
    timingAdvanceR9 = (MAX_TIMING_ADVANCE_R9_VALUE >= ueMeasReport_p[count].timingAdvanceR9)?\
                       ueMeasReport_p[count].timingAdvanceR9:INVALID_TIMING_ADVANCE_R9_VALUE;\
    angleOfArrival = (UInt16 *)&ueMeasReport_p[count].AoA[0];\
    for(i = 0; i < MAX_NUM_ANTENNA; i++)\
    {\
       if (MAX_ANGLE_OF_ARRIVAL_VALUE >= *(angleOfArrival + i))\
        {\
            flag = TRUE;\
        }\
       else\
        {\
            *(angleOfArrival + i) = INVALID_ANGLE_OF_ARRIVAL_VALUE;\
        }\
    }\
    validReports |= (MAX_TIMING_ADVANCE_R9_VALUE >= timingAdvanceR9)? TRUE:FALSE;\
    validReports |= (((flag == TRUE)?TRUE:FALSE) << 2);\
}
#define VALIDATE_AND_UPDATE_AOA_REPORTS(ueMeasReport_p,\
                                             angleOfArrival,validReports,count)\
{\
	UInt8 i;\
	UInt8 flag = FALSE;\
	angleOfArrival = (UInt16 *)&ueMeasReport_p[count].AoA[0];\
	for(i = 0; i < MAX_NUM_ANTENNA; i++)\
	{\
		if (MAX_ANGLE_OF_ARRIVAL_VALUE >= *(angleOfArrival + i))\
		{\
			flag = TRUE;\
		}\
		else\
		{\
			*(angleOfArrival + i) = INVALID_ANGLE_OF_ARRIVAL_VALUE;\
		}\
	}\
	validReports |= (((flag == TRUE)?TRUE:FALSE) << 2);\
}


#define SAVE_ECID_REPORTS_IN_UE_CONTXT(eCidReportParams_p,timingAdvanceR9,\
                                       angleOfArrival,recvTTI)\
{\
    UInt8 i;\
    eCidReportParams_p->type1TAValue = timingAdvanceR9;\
    eCidReportParams_p->type1TaAOAReceivedTTI = recvTTI;\
    for(i = 0; i < MAX_NUM_ANTENNA; i++)\
    {\
        eCidReportParams_p->angleOfArrival[i] = *(angleOfArrival + i);\
    }\
}

#define SAVE_ECID_AOA_REPORT_IN_UE_CONTXT(eCidReportParams_p,\
                                       angleOfArrival,recvTTI)\
{\
    UInt8 i;\
    eCidReportParams_p->type1TaAOAReceivedTTI = recvTTI;\
    for(i = 0; i < MAX_NUM_ANTENNA; i++)\
    {\
        eCidReportParams_p->angleOfArrival[i] = *(angleOfArrival + i);\
    }\
}
/* Cyclomatic_complexity_changes_start */
#ifdef TDD_CONFIG
void populateAndPushAcknNackNode(UInt8 dlSubFrameNum,
                            UInt8 prevContainerIndex,
                            UInt8 containerIndex,
                            UInt32 ueIndex,
                            UInt32 operation,
                            /* SPR 15909 fix start */
                            tickType_t recvTTI,
                            /* SPR 15909 fix end */
                            UInt16 sf,
                            InternalCellIndex internalCellIndex,
                            UInt8 modeType
                          );
#endif

void  decodePeriodicCqi(ueExpectedReportType cqiReportType,
        UInt8 *data_p,
        UInt16 ueIndex,
        /* +- SPR 17777 */
        DLUEContext *dlUEContext_p,
        UInt8 transmissionMode,
        ULUEContext *ulUEContext_p,
        InternalCellIndex internalCellIndex,
        InternalCellIndex servCellId,
        UInt16 subFrameNum);

void decodeAperiodicCqi(ueExpectedReportType cqiReportType,
        UInt8 *data_p,
        UInt16 ueIndex,
        UInt8 ri,
        ULUEContext *ulUEContext_p,
        DLUEContext *dlUEContext_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt8 transmissionMode,
        InternalCellIndex internalCellIndex,
        UInt8 *numOfBitsDecoded_p,
        InternalCellIndex servCellId);
/* Cyclomatic_complexity_changes_end */
/*- E_CID_5.3*/
void freeTTIBundlingNodesForTickMiss(UInt8 totalNumberOfTickDiff,
        /* SPR 15909 fix start */
        tickType_t currentTick,
        /* SPR 15909 fix end */
        UInt8 ulDelay,
        InternalCellIndex internalCellIndex);
 /*mandeep changes start*/
 /*mandeep changes stop*/
