/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacPUSCH.h,v 1.1.1.1.6.1.20.1 2010/11/08 06:06:20 gur10121 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the data structures and the function
 *                     declearation of pusch module. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacPUSCH.h,v $
 * Revision 1.1.1.1.6.1.20.1  2010/11/08 06:06:20  gur10121
 * Semi static changes merging
 *
 * Revision 1.1.1.1.6.1.14.3  2010/10/29 10:25:44  gur10121
 *  Semi Static UT fixes
 *
 * Revision 1.1.1.1.6.1.14.2  2010/10/27 17:13:51  gur10121
 * UT fixes + review comments incorporation
 *
 * Revision 1.1.1.1.6.1.12.4  2010/10/21 07:56:39  gur10121
 * Semi static UT fixes
 *
 * Revision 1.1.1.1.6.1.12.3  2010/10/15 18:53:15  gur10121
 * Semi staic Changesdone and checked in
 *
 * Revision 1.1.1.1.6.1.12.2  2010/10/14 10:34:57  gur10121
 * Semi Static header file changes
 *
 * Revision 1.1.1.1.6.1.12.1  2010/10/14 08:23:01  gur10121
 * Semi Static header file changes
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.18  2009/10/06 08:13:52  gur11912
 * modified for uplink throughput
 *
 * Revision 1.17  2009/09/14 07:08:03  gur18550
 * Logs Updated
 *
 * Revision 1.16  2009/07/28 08:55:56  gur18550
 * totalUplinkBytesSend_g Added
 *
 * Revision 1.15  2009/07/14 06:00:38  gur18550
 * #ifdef  PDCCH_DELAY_THREE
 *
 * Revision 1.14  2009/07/14 05:45:14  gur18550
 * Function Def Changed
 *
 * Revision 1.13  2009/07/10 06:20:23  gur18550
 * DELAY_THREE related Changes
 *
 * Revision 1.12  2009/07/07 13:07:25  gur18550
 * File Updated
 *
 * Revision 1.11  2009/07/01 06:47:36  gur18550
 * File Updated
 *
 * Revision 1.10  2009/06/29 07:28:02  gur19413
 * extern initPUSCH function.
 *
 * Revision 1.9  2009/06/24 15:15:55  gur18550
 * File Updated
 *
 * Revision 1.8  2009/06/23 15:57:54  gur18550
 * Final Review Comments Inc
 *
 * Revision 1.7  2009/06/22 08:05:57  gur18550
 * Final Comments Inc
 *
 * Revision 1.6  2009/06/04 07:44:42  gur18550
 * Comments Incorporated
 *
 * Revision 1.5  2009/06/03 15:47:10  gur18550
 * Comments incoporated
 *
 * Revision 1.4  2009/05/29 10:19:17  gur18569
 * removed compilation error
 *
 * Revision 1.3  2009/05/12 10:25:46  gur18550
 * file Updated
 *
 * Revision 1.2  2009/05/06 07:38:13  gur18550
 * File Updated
 *
 * Revision 1.1  2009/04/30 05:14:32  gur18550
 * INITIAL VERSION
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_PUSCH_H
#define LTE_MAC_PUSCH_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacULUEContext.h"
#include "lteMacPHICH.h"
#include "lteMacPUCCH.h"
#include "lteMacTBSize.h"
#include "lteMacULPowerControlData.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

#define LOG_PUSCH "PUSCH"

 /* ULA_CHG */
#define MAC_FILL_UL_RESOURCE_ALLOCATION_INFO(r, start, numRBs, internalCellIndex) \
{\
    (r)->allocatedLength = (numRBs);\
    (r)->allocatedIndex  = (start);\
    (r)->allocatedRiv    = getRIV(start,numRBs, internalCellIndex);\
}

    /* ICIC changes start */
#define MAC_UL_RESERVE_RESOURCES(start,\
        numOfRBs,res_p,bufferRBs_p,inputRBs,userLocType,\
        rbMapNode_p,rbMap_p, internalCellIndex)\
{\
    reserveAlreadyAvailableResources( \
            (start), \
            (numOfRBs), \
            (&rbMapNode_p->availableResourceInfo[userLocType]),\
            (rbMap_p));\
    MAC_FILL_UL_RESOURCE_ALLOCATION_INFO((res_p),(start),(numOfRBs),internalCellIndex);\
    /* ICIC changes start */\
    *bufferRBs_p -= (numOfRBs) - (inputRBs);\
    /* cell center resources are updated as it contains cell edge region also.\
     * we do not update cell edge resources when this is called for cell center user,\
     * we would have already processed cell edge users*/\
    if(CE_USER == userLocType)\
    {\
        updateCCAvailRes(&rbMapNode_p->availableResourceInfo[CC_CE_REGION],\
                res_p);\
        /* ICIC changes end */\
    }\
}
/* ICIC changes end */

/*CA Changes start  */
#define MAC_UL_SPS_RESERVE_RESOURCES(start,\
        numOfRBs,res_p,allocatedMCS_p,mcsIdx,inputRBs,userLocType,\
        rbMapNode_p,rbMap_p, internalCellIndex)\
       /* SPR 5295 changes start*/\
{ /* SPS RBs have been marked in PDCCH hence only update RBs here */ \
    /*coverity 530 CID 32739 */\
     (res_p)->allocatedRiv    = getRIV(start,numOfRBs,internalCellIndex);\
    /*coverity 530 CID 32739 */\
     *(allocatedMCS_p) = (mcsIdx);\
     /* ICIC changes start */\
     /* cell center resources are updated as it contains cell edge region also. \
     *       *      * we do not update cell edge resources when this is called for cell center user, \
     *             *           * we would have already processed cell edge users*/\
     if(CE_USER == userLocType)\
     {\
          updateCCAvailRes(&rbMapNode_p->availableResourceInfo[CC_CE_REGION],\
          res_p);\
     /* ICIC changes end */\
     }\
}
/*CA Changes end  */
/* ULA_CHG */
/* ICIC changes start */
#define WB_SB_MCS_DIFF_THRESHOLD 16
/* ICIC changes end */

/*UL_MU_MIMO_CHG Start*/
#define ABS(a)   (((a) < 0) ? -(a) : (a))
/*Review Comment Fix-- Value changed from 5 to 11 such that all MCS values in 
 * any particular Modulation Type can be considered for MU MIMO candidate. 
 * Value 11 is chosen because in UL Modulation Type-2 there are 11 MCS*/
#define UL_MU_MIMO_DIFFERENCE_THRESHOLD 11
/*UL_MU_MIMO_CHG End*/

#define MAX_ULSCH_UCI_SUPPORTED  MAX_UE_SUPPORTED
#ifdef FDD_CONFIG
#define FDD_NEXT_DCI_OFFSET 8 
#endif

#define LAST_UE 1
/* CR changes start */
#ifdef FDD_CONFIG
#define FDD_DATA_OFFSET  4
#endif
/* CR changes end */
/*TDD Config 0 Changes Start*/
#define COMMON_DCI_COUNT 2   /* Indicates common DCI0 for UL TX in n+7 and n+k U frames*/
#define COMMON_DCI_PUSCH_FAILURE 1 /*Indicates pusch failure in n+k, hence schedule count should be 1*/
#define INVALID_COUNT    255 /* Invalid scheduled count in case of ReTx*/
#define ULTX_OFFSET      7   /* Indicates ULTx in n+7 */
#define SET_LSB          1   /* To set the value of ulIndex for n+7*/
#define SET_MSB          2   /* To set the value of ulIndex for n+k*/
#define MAX_IPHICH_VALUE 2
/*TDD Config 0 Changes End*/ 

/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
#define INVALID_VALUE_FOR_UPLINK_RAT1 0xFF
#endif
/* SPR 21958 PUSCH RAT1 Support End */

/* SPR 19679: TDD HARQ Multiplexing Changes Start */
#ifdef TDD_CONFIG 
/* In this bitmap, bit is set whenever PUSCH transmission is done with DCI0 */
extern UInt16 puschNewOrAdaptiveTxMap_g[MAX_UE_SUPPORTED];

/* MACRO will check at a UL-sf PUSCH transmission was done with DCI0 or not 
 * Bits are set on new Ul transmission or adaptive retranmission 
 */
#define CHECK_IS_PUSCH_TRANSMISSION_WITH_DCI0(ueIndex, subFrameNumber,       \
                isNewOrAdaptiveTransmission)                        \
{                                                                           \
    isNewOrAdaptiveTransmission =                                                         \
    (0!= (puschNewOrAdaptiveTxMap_g[ueIndex] & bitmask[subFrameNumber]))?TRUE:FALSE; \
}

/* Set the puschNewOrAdaptiveTxMap_g[ueIndex] bitmask to indicate that  
 * PUSCH transmission was done with DCI0 or not at a particular UL-Sf */
#define SET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(ueIndex, subFrameNum)            \
{\
    puschNewOrAdaptiveTxMap_g[ueIndex] |= bitmask[subFrameNum];                     \
}

/* Reset the puschNewOrAdaptiveTxMap_g[ueIndex] bitmask */
#define RESET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(ueIndex, subFrameNum)          \
{                                                                           \
    puschNewOrAdaptiveTxMap_g[ueIndex] =                                           \
        puschNewOrAdaptiveTxMap_g[ueIndex] & resetBitmask[subFrameNum];            \
}

 
/* Check if this is HARQ pdu then fill HARQ size according HARQ
 * on PUSCH and set bit to indicate harq on PUSCH with DCI 0 */  
#define SET_HARQ_SIZE_AND_BIT_FOR_UL_TX_WITH_DCI0_FOR_MUX(ulschUciRachInfo_p,\
               ulUEContext_p,internalCellIndex,ulSubFrame,numberOfInfoPresent,\
               ulUeScheduledInfo_p )\
{\
    /*SPR 20193 Changes Start*/\
    DLUEContext *ueDLContext_p = (&dlUECtxInfoArr_g[ulUEContext_p->ueIndex])->dlUEContext_p;\
    /*SPR 20260 Changes Start*/\
    if(ueDLContext_p && !(ueDLContext_p->scellCount))\
    /*SPR 20260 Changes End*/\
    /*SPR 20193 Changes End*/\
    {\
	UInt8  M = globalDlAssociationSetForHarqMultiplexing_gp\
	                         [internalCellIndex][ulSubFrame].count;\
	UInt8 i = 0; \
	UInt8 k = 0; \
	UInt8 isHARQPDU = 0; \
	SInt32 signedOffset = 0; \
	UInt32 dlIndex = 0; \
	UInt8 size = 0; \
	\
	switch (ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType) \
	{ \
		case MAC_SS_PDU_ULSCH_HARQ: \
		case MAC_SS_PDU_ULSCH_CQI_HARQ_RI: \
		case MAC_SS_PDU_UCI_HARQ: \
		case MAC_SS_PDU_UCI_SR_HARQ : \
		case MAC_SS_PDU_UCI_CQI_HARQ: \
		case MAC_SS_PDU_UCI_CQI_SR_HARQ: \
		     isHARQPDU = LTE_TRUE; \
                     break;\
		default: \
	             isHARQPDU = LTE_FALSE;\
                     break; \
	} \
	if ( isHARQPDU && ( M > 1)) \
	{ \
		for (i = M; i > 0; i--) \
		{ \
			k = globalDlAssociationSetForHarqMultiplexing_gp[internalCellIndex][ulSubFrame].\
              			kwithSmallM[i - 1].k;\
			signedOffset = (ulSubFrame - k); \
			signedOffset = signedOffset % MAX_SUB_FRAME;\
			dlIndex = (signedOffset) < 0?(signedOffset + MAX_SUB_FRAME):(signedOffset); \
			\
			if (INVALID_CCE_INDEX != ulUeScheduledInfo_p->dlCceIndexMuxPusch[dlIndex]) \
			{ \
				size++; \
                           ulUeScheduledInfo_p->dlCceIndexMuxPusch[dlIndex] = INVALID_CCE_INDEX;\
			} \
		} \
		/*SPR 20564 Changes Start*/\
		/*code removed*/\
		/*SPR 20564 Changes End*/\
		ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].\
                                         dlHarqInfo.harqSize = size; \
        \
		/* As this is new transmission i.e. PUSCH with DCI0 so setting
		 * puschNewOrAdaptiveTxMap_g bitmap for a UE */ \
		SET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(ulUEContext_p->ueIndex, \
				ulSubFrame); \
	} \
    }\
}
#endif 
/* SPR 19679: TDD HARQ Multiplexing Changes End */
/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct ULGrantInfoT
{
        ULUEContext* ulUEContext_p;
#ifdef TDD_CONFIG       
        UeScheduledInfo * ueScheduledInfo_p;    
        UInt8 rbMapNum;
        UInt8 ulSubframe;
#elif FDD_CONFIG        
        DirectIndexingUEInfo*  tempDirectIndexingUEInfo_p;
        UInt8 subFrameNumber;
#endif
        DciCCEInfo* tempDciCCEInfo_p;
        UInt8 tempHarqProcessId;
        /* +- SPR 18268 */
        UInt16 tempUeIndex;
        /* +- SPR 18268 */
        PdcchULNewUE *pdcchULNewUE_p;
}ULGrantInfo;

/* CSI_P2_PUSCH_CHANGES Start */
typedef struct MACSSCqiScellInfoT
{
    /* The size of the DL CQI/PMI in bits in case of rank 1 report.
Value: 0 . 255   */
    UInt8     dlCqiPmiSizeRank_1;

    /* The size of the DL CQI/PMI in bits in case of rank>1 report.
Value: 0 . 255   */
    UInt8     dlCqiPmiSizeRankGT_1;

    /* The size of RI in bits
Value:1 . 2    */
    UInt8     riSize;
    UInt8     rankIndicator;

}MACSSCqiScellInfo;
/* CSI_P2_PUSCH_CHANGES End */

/*Semi Static changes start*/
/* Semi static info for CQI/PMI/RI start */
typedef struct MACSSCqiInfoT
{
    /* The size of the DL CQI/PMI in bits in case of rank 1 report.
Value: 0 . 255   */
    UInt8     dlCqiPmiSizeRank_1;

    /* The size of the DL CQI/PMI in bits in case of rank>1 report.
Value: 0 . 255   */
    UInt8     dlCqiPmiSizeRankGT_1;

    /* The size of RI in bits
Value:1 . 2    */
    UInt8     riSize;

    /* Delta offset for CQI. This value is fixed for a UE and allocated in 
       RRC connection setup.
Value: 0 . 15    */
    UInt8     deltaOffsetCQI;

    /* Delta offset for RI. This value is fixed for a UE and allocated in 
       RRC connection setup.
Value: 0 . 15     */
    UInt8     deltaOffsetRI;

    /* SS_S2 */
    UInt8     rankIndicator;
    
    UInt16    pucchIndex;  
    /* SS_S2 */
     /*FAPI2_1 changes start*/
    UInt8     reportType;
    /*FAPI2_1 changes end*/
    /* CSI_P2_PUSCH_CHANGES Start */
    UInt8     bitMaskForAperiodicReport;
    MACSSCqiScellInfo cqiRiScellInfo[MAX_NUM_SCELL];
    /* CSI_P2_PUSCH_CHANGES End */

}MACSSCqiInfo;
/* Semi static info for CQI/PMI/RI end */

/* Semi static info for HARQ Start */
#ifdef TDD_CONFIG
/* TDD Harq Information */
typedef struct MacSSTddHarqInfoT
{
    /* For ACK_NACK Mode 0 (Bundling) and 1 (Multiplexing), 
    this is the size of the ACK/NACK in bits.
    Value: 1 . 4
    For Special Bundling this is the expected number of ACK/NACK 
    responses (UDAI + NSPS).
    Value: 0 . 9     */
    UInt8   harqSize;
    /* The number of ACK/NACK responses received in this subframe. 
    For TDD only. 
    Value: 0 . 4
    (Value 0 is only valid for Special Bundling.)   */
    UInt8   numOfPUCCHResource;

    /* SPR 1680 changes start*/
    /* HARQ resource 0, value: 0 . 2047 */
    UInt16   n_PUCCH_1_0;

    /* HARQ resource 1, value: 0 . 2047 */
    UInt16   n_PUCCH_1_1;

    /* HARQ resource 2, value: 0 . 2047 */
    UInt16   n_PUCCH_1_2;

    /* HARQ resource 3, value: 0 . 2047 */
    UInt16   n_PUCCH_1_3;
    /* SPR 1680 changes end*/
    /* The format of the ACK/NACK response expected. For TDD only.
    0 = BUNDLING
    1 = MULTIPLEXING        */
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
    UInt8   ackNackMode;
#else    
    UInt8   acknackMode;
#endif
    
    /* Delta Offset HARQ value: 0 . 15 */
    UInt8   deltaOffsetHARQ;

    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
    /* HARQ size when PUSCH with DCI0 is sent with HARQ Multiplexing */
    /*SPR 20564 Changes Start*/
    /*Code Removed*/
    /*SPR 20564 Changes End*/
    /* SPR 19679: TDD HARQ Multiplexing Changes End */
}MacSSTddHarqInfo;
/* TDD Harq Information Ends */

#elif FDD_CONFIG
/* FDD Harq Information */
#ifdef FAPI_4_0_COMPLIANCE
typedef struct MacSSFddHarqInfoT
{
    /* The PUCCH Index value for ACK/NACK
    Value: 0 . 2047    */
    UInt16 n_PUCCH_1_0;
    UInt16  n_PUCCH_1_1;
    UInt16  n_PUCCH_1_2;
    UInt16  n_PUCCH_1_3;


    /*  The size of the ACK/NACK in bits.
    Value: 1 . 2   */
    UInt8   harqSize;
    UInt8   ackNackMode;
    UInt8   numOfPUCCHResource;
    UInt8   deltaOffsetHARQ;

}MacSSFddHarqInfo;
#else

typedef struct MacSSFddHarqInfoT
{
    /* The PUCCH Index value for ACK/NACK
    Value: 0 . 2047    */
    UInt16  pucchIndex;

    /*  The size of the ACK/NACK in bits.
    Value: 1 . 2   */
    UInt8   harqSize;
    
    /* Delta Offset HARQ value: 0 . 15 */
    UInt8   deltaOffsetHARQ;

}MacSSFddHarqInfo;
/* FDD Harq Information Ends */
#endif
#endif
/* Semi static info for HARQ End */



/* Semi static info for SR */
typedef struct MACSSSrInfoT
{
    /* The PUCCH Index value  
Value: 0 . 2047    */
    UInt16   pucchIndex;

}MACSSSrInfo;

#if  defined(FAPI_1_1_COMPLIANCE)||  defined(FAPI_4_0_COMPLIANCE)
typedef struct MacSSHarqBufferReleaseInfoT
{
    UInt16  rnti;

}MacSSHarqBufferReleaseInfo;
#endif
/*Cyclomatic Complexity changes - starts here*/
void lteMacSsCalcHarqSize(UInt8 * harqSize,
                          DCIFormat dlDciFormat);
/*Cyclomatic Complexity changes - ends here*/
#define LTE_MAC_SS_CALC_HARQ_SIZE(harqSize,dlDciFormat)\
{\
    /*  Here,calculate HARQ Size can be changed \
     *        * at transmitter under SPR 3053\
     *              */ \
    switch(dlDciFormat)\
    {\
        case DCI_FORMAT_0 :\
        case DCI_FORMAT_1 :\
        case DCI_FORMAT_1A :\
        case DCI_FORMAT_1B :\
        case DCI_FORMAT_1C :\
        case DCI_FORMAT_1D :\
                    harqSize = 1;\
                    break;\
        case DCI_FORMAT_2 :\
        case DCI_FORMAT_2A:\
        /* + TM7_8 Changes Start */\
        case DCI_FORMAT_2B:\
        /* - TM7_8 Changes End */\
        case DCI_FORMAT_3 :\
        case DCI_FORMAT_3A :\
                    harqSize = 2;\
                    break;\
        default:\
                    harqSize = 2;\
                    break;\
    }\
}\
/* Semi static info for SR */

/*Semi Static changes End*/


typedef enum MACUlConfigPduInfoT
{
    MAC_SS_PDU_ULSCH,
    /*SPR 1115 CHG*/
    MAC_SS_PDU_ULSCH_CQI_RI,
    MAC_SS_PDU_ULSCH_HARQ,
    MAC_SS_PDU_ULSCH_CQI_HARQ_RI,
    MAC_SS_PDU_UCI_CQI,
    MAC_SS_PDU_UCI_SR,
    MAC_SS_PDU_UCI_HARQ,
    MAC_SS_PDU_UCI_SR_HARQ,
    MAC_SS_PDU_UCI_CQI_HARQ,
    MAC_SS_PDU_UCI_CQI_SR,
    MAC_SS_PDU_UCI_CQI_SR_HARQ,
    MAC_SS_PDU_SRS,
    /*SPR 1115*/
#if (defined(FAPI_1_1_COMPLIANCE) && (!defined(FAPI_4_0_COMPLIANCE)))
    MAC_SS_PDU_HARQ_BUFFER_RELEASE,
#endif
#ifdef FAPI_4_0_COMPLIANCE
    MAC_SS_PDU_HARQ_BUFFER_RELEASE,
    /*FAPI2_1 changes start*/    
    MAC_SS_PDU_ULSCH_UCI_CSI_PDU,
    MAC_SS_PDU_ULSCH_UCI_HARQ_PDU,
    MAC_SS_PDU_ULSCH_CSI_UCI_HARQ_PDU,
#endif
    /*FAPI2_1 changes end*/    
    MAC_SS_PDU_INVALID

}MACUlConfigPduInfo;


/*Main Data structure for storing semi static info*/
typedef struct ULSchUCIInfoT
{
    MACUlConfigPduInfo pduType;
    UInt16  rnti;
    ULHarqInfo* ulHarqInfo_p;
    UInt8   ulHarqProcessId;
    /* SRS_CHG */
    /* Nsrs: 0 = No overlap, 1 = overlap -> populated from srsPresent*/
    UInt8           nSRS;
    MACSSSrInfo srInfo;    
    MACSSCqiInfo    cqiRiInfo;
#ifdef TDD_CONFIG
    MacSSTddHarqInfo dlHarqInfo;
#elif FDD_CONFIG
    MacSSFddHarqInfo dlHarqInfo;
#endif
#ifdef TDD_CONFIG
    /* SRS_CHG */
    /*Pointer to scheduled info in UE context. It will be used to reset the 
    ulSchUciInfoIndex while preparing UL_CONFIG.request. It is used only in case
    of TDD. In case of FDD, directIndexingArray is used.*/
    UeScheduledInfo *ueScheduledInfo_p;                                
    /* SRS_CHG */
	/*FDD_SRS*/
#elif FDD_CONFIG
   DirectIndexingUEInfo *ueDirectIndexInfo_p;                                
	/*FDD_SRS*/
#endif

/* T2200 Changes start */
/* T2200 Changes stop */
#if   (defined(FAPI_1_1_COMPLIANCE) || defined(FAPI_4_0_COMPLIANCE))
   MacSSHarqBufferReleaseInfo     harqReleaseInfo;
#endif
}ULSchUciInfo;

/* SRS_CHG */
/* This structure contains parameter information that is required to prepare UL
 * Config PDU for SRS */
typedef struct ULSRSPduInfoT
{
    UInt16  rnti;
    SRSDedicatedSetupInfo   *srsDedicatedSetupInfo_p;
}ULSRSPduInfo;
/* SRS_CHG */

typedef struct ULSchUCIInfoPoolT
{
    UInt8  isPrachResourceReserved;
    UInt8  prachFreqOffset;
    /* SRS_CHG */
    UInt8   isSRSPresent;
    UInt8   numSRSPdus;
    ULSRSPduInfo srsPduInfo[MAX_UE_SUPPORTED];
    /* SRS_CHG */
    UInt16  numberOfInfoPresent;
    ULSchUciInfo  ulschUciInfo[MAX_ULSCH_UCI_SUPPORTED];

}ULSchUciRachPoolInfo;


/* UL_MU_MIMO_CHG Start */
typedef struct  BuddyPairInfoT
{
    UInt32 maxOfMinDataSize;        /* Maximum of minDataSize for both UEs*/
    UInt32 maxOfMaxDataSize;        /* Maximum of maxDataSize for both UEs*/
    UInt32 minAvgMCS;                 /*specifies the minimum of avgMCS for both UEs*/
    UInt8  strictAllocFlag;          /* will be set if strictAlloc is TRUE for atleast one of the UE */
    UInt8  minStrictRBReq;           /* Max of Min RB req if above strictAllocFlag is set */
    /*specifies the minimum of highest MCS received by MAC in UL for both UEs*/
    UInt8  minOfHighestReportedMCS; /*Used for FSS only*/
    /* ATB changes */
    UInt32 buddyMaxPowerFlag;
    UInt32 buddyMinPowerFlag;
    /* ATB changes */
    /*SPR_9204_START*/
    UInt32 buddyMaxDataSize;
    UInt32 buddyAvgMCS; 
    /*SPR_9204_END*/
} BuddyPairInfo;
/* UL_MU_MIMO_CHG End */

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
extern UL_GRANT_PUSCH_QUEUE_TYPE* ulGrantPUSCHFailureQueue_gp[MAX_NUM_CELL];
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern UInt32 totalUplinkBytesSend_g;

/*CA Changes start  */
extern UplinkContainer* ulContainerPool_gp[MAX_NUM_CELL];
/*CA Changes end  */

/*Semi Static changes start */
/*CA Changes start  */
extern ULSchUciRachPoolInfo* ulSchUciRachContainer_gp[MAX_NUM_CELL];
/*CA Changes end  */
/*Semi Static changes End */
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
#ifdef FDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType allocatePUSCHResourcesForNewUEs(
/* SPR 5599 changes end (ZIP ID 129059) */
                    ULUEContext* ulUEContext_p,
                    PdcchULNewUE *pdcchULNewUE_p,
                    DirectIndexingUEInfo*  tempDirectIndexingUEInfo_p ,
                    AllocatedResInfo* resourceReserved_p,
                    UInt8 subFrameNumber,
                    UInt32 freeRBs,
                    SInt8 *bufferRBs_p,
                    /* Time Averaging changes start */
                    TpcTriggeredToPowerCorrection tpc,
                    UInt32 *ueMaxPowerFlag_p,
                    UInt32 *minPowerFlag_p,
                    UInt8 spsAllocFlag,
                    /* Time Averaging changes end */
                    /* UL_MU_MIMO_CHG Start */
                    UInt8 hiIndex, 
                    /* SPR 15909 fix start */
                    tickType_t currentGlobalTick, 
                    /* SPR 15909 fix end */
                    UInt8 ulDelay,
                    /* UL_MU_MIMO_CHG End */
                    /*CA Changes start  */
                    InternalCellIndex internalCellIndex
                    /*CA Changes end  */
                    );
                   
                   
#elif TDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType allocatePUSCHResourcesForNewUEs(
/* SPR 5599 changes end (ZIP ID 129059) */
                    ULUEContext* ulUEContext_p,
                    PdcchULNewUE *pdcchULNewUE_p,
                    UeScheduledInfo *ueScheduledInfo_p,
                    AllocatedResInfo* resourceReserved_p,
                    UInt8 subFrameNumber,
                    UInt32 freeRBs,
                    SInt8 *bufferRBs_p,
                    /* Time Averaging changes start */
                    TpcTriggeredToPowerCorrection tpc,
                    UInt32 *ueMaxPowerFlag_p,
                    UInt32 *ueMinPowerFlag_p,
                    /* Time Averaging changes end */
                    /* + SPS_TDD_Changes */
                    UInt8 spsAllocFlag,
                    /* - SPS_TDD_Changes */
                    /* UL_MU_MIMO_CHG Start */
                    UInt8 hiIndex,
                    /* SPR 15909 fix start */
                    tickType_t currentGlobalTick,
                    /* SPR 15909 fix end */
                    /* UL_MU_MIMO_CHG End */
                    InternalCellIndex itnernalCellIndex,
		    /* SPR 11331 Fix Start */
                     /* SPR 15909 fix start */
                    tickType_t ulSchTTI
                    /* SPR 15909 fix end */
		    /* SPR 11331 Fix End */
                    /*TDD Config 0 Changes Start*/
                    ,UInt8 ulSubFrameNum
                    /*TDD Config 0 Changes End*/
                    ,ExecutionLegFunctionArg* execArgs
                    );

#endif                    

#ifdef FDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129059) */
void processULNewUeInPUSCH(
/* SPR 5599 changes end (ZIP ID 129059) */
                                 UInt8 ulDelay,
                                 UInt8 containerTick,
                                 /* +- SPR 17777 */
                                 /* SPR 15909 fix start */
                                 tickType_t currentGlobalTick,
                                 /* SPR 15909 fix end */
                                 UInt8* numberOfULUEsToBeScheudled_p,
                                 /* CR changes start */
                                 UInt8 hiIndex,
                                 /* ICIC changes start */
                                 SInt8 *bufferRBs,
                                 UInt8 userLocType,
                                 /* ICIC changes end */
                                 /* CR changes end */
                                 /*CA Changes start  */
                                 InternalCellIndex internalCellIndex,
                                 /*CA Changes end  */
                                 ExecutionLegFunctionArg* execArgs);


/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType processPUSCHLogicForDelayPath2( UInt8 ulDelay,
/* SPR 5599 changes end (ZIP ID 129059) */
                                           /* SPR 15909 fix start */
                                           tickType_t currentGlobalTick,
                                           /* SPR 15909 fix end */
                                           UInt8  subFrameNum,
                                           /*CA Changes start  */
                                           InternalCellIndex  cellIndex,
                                           /*CA Changes end  */
                                           ExecutionLegFunctionArg* execArgs);

#elif TDD_CONFIG 
/* SPR 5599 changes start (ZIP ID 129059) */
void processULNewUeInPUSCH(
/* SPR 5599 changes end (ZIP ID 129059) */
        PdcchULOutputInfo* tempPdcchULOutput_p,
        UInt8 containerTick,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt8 subframeNum,
        UInt8 rbMapNum,
        /* SPR 5620 START*/
        UInt16 ulGrantSysFrameNum,
        /* SPR 5620 END*/
        UInt16 ulGrantSubFrameNum,
        UInt16* numberOfULUEsToBeScheudled_p,
        /* ICIC changes start */
        SInt8 *bufferRBs,
        UInt8 locType,
        InternalCellIndex internalCellIndex,
        ExecutionLegFunctionArg* execArgs);
       /* ICIC changes end */

		
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType processPUSCHLogicForDelayPath2( UInt8 ulDelay,
/* SPR 5599 changes end (ZIP ID 129059) */
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt16 SFN,
        UInt8  subFrameNum,
        InternalCellIndex internalCellIndex,
        ExecutionLegFunctionArg* execArgs
        );
 /* +- SPR 17777 */
#endif
 
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType processULGrants(ULGrantInfo* ulGrantInfo,
/* SPR 5599 changes end (ZIP ID 129059) */
                                   /* SPR 15909 fix start */
                                  tickType_t currentGlobalTick, 
                                  /* SPR 15909 fix end */
                                  UInt32* tempPUSCHResources_p,
                                  SInt8 *bufferRBs,
                                  /* Time Averaging changes start */
                                  TpcTriggeredToPowerCorrection tpc,
                                  /* Rel9_upgrade_CR410 */
#ifdef FDD_CONFIG
                                  UInt8 ulDelay,
#elif TDD_CONFIG
                                  UInt8 subFrameNumber,
#endif
                                  /* Rel9_upgrade_CR410 */
                                   UInt32 *ueMaxPowerFlag
                                  /* Time Averaging changes end */
                                  /* CR changes start */
#ifdef FDD_CONFIG
                                  ,UInt8 hiSubFrameNum
                                  /* CR changes end */
#endif
                                  ,UInt32 *ueMinPowerFlag_p
                                  /* + SPS_TDD_Changes */
                                  ,UInt8 spsAllocFlag
                                  /* - SPS_TDD_Changes */
                                  /*CA Changes start */
                                  ,InternalCellIndex internalCellIndex
                                  /*CA Changes end */
/*Fix for SPR 12477, adding subFrameNum*/   
#ifdef FDD_CONFIG
                                  ,UInt8 subFrameNum 
#endif
                                  ,ExecutionLegFunctionArg* execArgs
                                  );

/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType putEntryInPUSCHFailureQueue(
/* SPR 5599 changes end (ZIP ID 129059) */
                                       UInt16 ueIndex, 
                                       /* SPR 15909 fix start */
                                       tickType_t ttiCounter,
                                       /* SPR 15909 fix end */
                                       ULGrantRequestType ulGrantRequestType,
                                       /*CA Changes start  */
                                       InternalCellIndex internalCellIndex);
                                       /*CA Changes end  */


/*CA Changes start  */
extern void initPUSCH(UInt8 numCellsConfigured);
/*CA Changes end  */

/* DRX_CHG */
extern void putEntryInDRXULSchedulerEventQueue(UInt16 ueIndex, 
                                               /* SPR 15909 fix start */
                                               tickType_t globalTick,
                                               /* SPR 15909 fix end */
                                               InternalCellIndex internalCellIndex);
/* DRX_CHG */
/* SPR 5599 changes start (ZIP ID 129059) */
void assignRBsForMaxData(PdcchULNewUE *pdcchULNewUE_p, 
/* SPR 5599 changes end (ZIP ID 129059) */
                                  UInt8 *requiredRB_p,
                                  /* Time Averaging changes start */
                                  UInt32 tempPUSCHResources,
                                  SInt8 tpc,
                                  UInt32 *ueMaxPowerFlag_p,UInt32 *ueMinPowerFlag_p
                                  /* Time Averaging changes end */
                                  /* +DYNAMIC_ICIC */
                                  ,ULUEContext* ulUEContext_p,
                                  InternalCellIndex internalCellIndex);
                                  /* -DYNAMIC_ICIC*/
/* ICIC changes start */
void fillSemiStaticInfoForULSCH (ULSchUciRachPoolInfo *ulschUciRachInfo_p,
#ifdef FDD_CONFIG
                                 DirectIndexingUEInfo *directIndexingUE_p,
#elif TDD_CONFIG
                                 UeScheduledInfo *directIndexingUE_p,
#endif
                                 ULUEContextInfo *ulUEContextInfo_p,
                                 ULHarqInfo* ulHarqInfo_p,
                                 UInt8   ulHarqProcessId,
                                 /* SPR 5620 START*/
#ifdef TDD_CONFIG
                                 UInt16 sysFrameNum,
#endif
                                 /* SPR 5620 END*/
                                 UInt8 subFrameNum,
                                 /* + CQI_5.0 */
                                 UInt8 aperiodicCQIRequestedFlag,
                                 InternalCellIndex internalCellIndex);
                                 /* - CQI_5.0 */

UInt8 updateCEAvailRes(AvailableResInfo *availRes_p,
        UInt8  startIndex,
        UInt8  numPhysicalResources,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex);
        /*CA Changes end  */

UInt8 updateCCAvailRes(AvailableResInfo *availRes_p,
        AllocatedResInfo *resourceReserved_p);

/* ICIC changes end */
/*SPR 7086 Fix Start*/
UInt8 nSRSDetermineOnCollision(
    ULUEContext *ulUEContext_p, 
    UInt8 ulHarqProcessId, 
    UInt8 ulSubFrameNum,
    InternalCellIndex internalCellIndex);
/*SPR 7086 Fix Start*/

/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
UInt8 setUplinkResourcesAsBusy(RbMapNode *rbMapNode_p, /*RB Map for RAT0 */
		              UInt8 s0, /*Start RB Index for RAT0, Start RBG Index for RAT1 */
			      UInt8 s1, /*End RB in case of RAT0, s1 for RAT1 */
			      UInt8 s2, /* s2 for RAT1 */
			      UInt8 s3, /* s3 for RAT1 */
			      UInt8 allocationType, /* 0/1 indicating RAT0/RAT1 */
                  InternalCellIndex internalCellIndex);


MacRetType allocateResourceForUEThroughRAT1( UInt32 numOfResourcesReq,
    UInt32 minReqdRBs,
    AllocatedResInfo *allocatedResInfo_p,
    UInt32 strictAllocatedFlag,
    AvailableResInfo *availRes_p,
    RbMapNode *rbMapNode_p,
    InternalCellIndex internalCellIndex); 

void populateCombinatorialIndexTable (InternalCellIndex internalCellIndex);

UInt16 getCombinatorialIndex(UInt8 s0,UInt8 s1,UInt8 s2,UInt8 s3);

UInt16 calculateCombiIndex(UInt8 s0,UInt8 s1,UInt8 s2,UInt8 s3,
		           InternalCellIndex internalCellIndex);
void getAvailableFreeSets(RBGSet *rbgarr,
	             UInt8 bitmap,
	             UInt8 *arrIdx,
	             UInt8 *lastBit,
	             UInt8 startRBGId,
		     RbMapNode *rbMapNode_p,
             /* SPR 22263 Fix Start */
             UInt8 *actualSetsFound);
             /* SPR 22263 Fix End */

void getRbgIndexes(RBGSet *rbgarr,
	          UInt8 arrSize,
	          UInt8 *s0,
	          UInt8 *s2,
	          UInt8 *s2Size,
	          UInt8 rbgNeed);

#endif
/* SPR 21958 PUSCH RAT1 Support End */
/*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
/*This function will calculate the phich val for Tdd*/
extern UInt8 calculateIPhichValue(
        UInt8 ulSubFrameNum,
        InternalCellIndex internalCellIndex);

/*This function will block the phich resources at the ristart and ndmrs
 * provided.It will also update the harq info*/
extern void reserveHiAndUpdateHarqInfo(UInt8 iPhich,
        UInt8 riStart,
        UInt8 nDmrs,
        UInt8 phichSubFrameNum,
        ULHarqInfo* tempULHarqInfo_p,
        InternalCellIndex internalCellIndex);

/*SPR 19504 fix start*/
 void acksOnlyByBundlingOrMequalsOne(
        UInt32 subFrameNum,
        UInt32  ueIndex,
        UInt32  rbMapNum,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UeScheduledInfo * ulUeScheduledInfo_p,
        /*SPR 19504 fix start*/
        UInt8 puschFailureFlag,
        /*SPR 19504 fix end*/
        InternalCellIndex internalCellIndex);
/*SPR 19504 fix end*/
#endif

#ifdef MAC_AUT_TEST 
/*
 * This enum is used for testing the common function for FSS and non FSS allocation
 * For TDD Config 0, second execution of PUSCH, to track the reason of failure
 * of the function.
 */
typedef enum MacRbAllocationFailureInfoT
{
    MAC_INVALID_INFO,
    MAC_INSUFFICIENT_FREE_RBs,
    MAC_INSUFFICIENT_BUFFER_RBs,
    MAC_EXCEEDING_WB_SB_MCS_DIFF_THRESHOLD,
    MAC_MCS_GREATER_THAN_AVG_MCS,
    MAC_RI_LENGTH_LESS_THAN_MIN_RBs,
    MAC_SAME_NDMRS_UNAVAILABLE,
    MAC_ALLOCATION_FAILURE,
    MAC_RI_LENGTH_GREATER_THAN_MAX_RBs
}MacRbAllocationFailureInfo;
#endif
/*TDD Config 0 Changes End*/

#endif  /* LTE_MAC_PUSCH_H */
