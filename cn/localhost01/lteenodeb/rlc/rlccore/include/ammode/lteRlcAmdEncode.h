/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcAmdEncode.h,v 1.1.6.2.2.1 2010/09/21 15:49:25 gur20491 Exp $ 
 *
 ****************************************************************************
 *
 *  File Description : 
 *                  This file desclares the Encoding Operations of Am Entity
 *
 ****************************************************************************
 * Revision Details 
 * ----------------
 *
 * $Log: lteRlcAmdEncode.h,v $
 * Revision 1.1.6.2.2.1  2010/09/21 15:49:25  gur20491
 * FAPI changes
 *
 * Revision 1.1.2.1  2010/08/10 09:37:45  gur19836
 * Files added for RLC AM Release
 *
 * Revision 1.1.6.10  2010/07/29 07:16:28  gur19836
 * SO datatype changed from UInt8 to UInt16
 *
 * Revision 1.1.6.9  2010/07/28 04:18:22  gur19836
 * UT Bug Fixes
 *
 * Revision 1.1.6.8  2010/07/17 11:33:23  gur23054
 * Review Comments fixed
 *
 * Revision 1.1.6.7  2010/07/15 06:30:30  gur23054
 * Review comments fixes
 *
 * Revision 1.1.6.6  2010/07/05 09:43:26  gur23054
 * Corrected Indentation
 *
 * Revision 1.1.6.5  2010/07/01 07:23:56  gur23054
 * encodeAmdPduSegment function declared
 *
 * Revision 1.1.6.4  2010/06/23 05:24:52  gur23054
 * Exported functions added
 *
 * Revision 1.1.6.3  2010/06/22 09:14:20  gur19836
 * Compilation errors removed
 *
 * Revision 1.1.6.2  2010/06/21 08:11:10  gur23054
 * Rlc AM Base Code Merged
 *
 * Revision 1.1.6.1  2010/06/17 10:59:39  gur19836
 * RLC AM Base Code Merged
 *
 * Revision 1.1.2.4  2009/11/10 09:05:42  gur19140
 * RLC AM encoding minor changes
 *
 * Revision 1.1.2.3  2009/10/28 09:01:42  gur19140
 * Minor changes for file headers
 *
 *
 *
 ****************************************************************************/
#ifndef AMDENCODE_H
#define AMDENCODE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcTypes.h"
#include "lteRlcEntityDef.h"
#include "lteRlcAmdPdu.h"
#include "lteRlcAmdSdu.h"
#include "lteRlcTxProcessor.h"
#include "lteRlcStat.h"



#define RLC_PDU_LI_MAX_SIZE 2047
#define RLC_MAX_SDU_IN_PDU 512
#define RLC_MID_SDU_SEGMENTED       0x03
#define RLC_FIRST_SDU_SEGMENTED     0x02
#define RLC_LAST_SDU_SEGMENTED      0x01
#define RLC_NO_SDU_SEGMENTED        0x00

/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef struct RlcAmdPduSegInfoT
{
    UInt32  SegHdrSize;
    UInt32  rawDataSize;
    UInt16  numDfe;
    UInt16  soStart;
    /* + SPR 17439 */
    UInt32  soEnd;
    /* - SPR 17439 */
    UInt8   fi;
    UInt8   LSF;
    UInt8   P;
    UInt16  dfeLength[RLC_MAX_SDU_IN_PDU];
} RlcAmdPduSegInfo;

typedef struct RlcAmdPduFixHT {
    UInt8 DC;
    UInt8 RF;
    UInt8 P;
    UInt8 FI;
    UInt8 E;
    UInt16 SN;
}RlcAmdPduFixH;

typedef struct RlcAmdPduSegmentFixHT {
    UInt8 DC;
    UInt8 RF;
    UInt8 P;
    UInt8 FI;
    UInt8 E;
    UInt8 LSF;
    UInt16 SN;
    UInt16 SO;
}RlcAmdPduSegmentFixH;

typedef struct RlcAmdControlPduFixHT {
    UInt8 DC;       /* 1 bit  */
    UInt8 CPT;      /* 3 bit  */
    UInt8 ACK_SN;   /* 10 bit */
    UInt8 E1;        /* 1 bit */
}RlcAmdControlPduFixH;


/****************************************************************************
 * Exported Functions
 ****************************************************************************/
UInt32 rlcEncodeAmdControlRawPdu(
        RxAMEntity *pAmRxE,
        RlcPduBuffer *pduFormed_p,
        UInt32 *requestedDataSize
        ); 

UInt32 rlcEncodeAmdDataRawPdu(
        TxAMEntity *pAmTxE,
        RlcPduBuffer * pduFormed,
        UInt32 requestedDataSize,
        UInt8 * pollBit,
        /* SPR 15909 fix start */
        tickType_t *pktArrTime
        /* SPR 15909 fix end */
        );

UInt32 rlcEncodeAmdReTxPdu(
        TxAMEntity *pAmTxE,
        RlcPduBuffer * pduFormed,
        UInt32 requestedDataSize,
        UInt8 * pollBit,
        RLCHarqFailureInfo* rlcHarqFailureInfo_p,
        /* SPR 15909 fix start */
        tickType_t *pktArrTime
        /* SPR 15909 fix end */
        );

/* Rlc Rel 2.0 change start*/
UInt32 rlcFillStatusBuffer(
        RlcStatusBuffer* rlcStatusBuffer_p,
        UInt32 transId,
        UInt16 deliveryStatus
        );

/* SPR 5599_129119 Fix Start */
UInt32 rlcEncodeAmdPduSegment(
/* SPR 5599_129119 Fix End */
        TxAMEntity* pAmTxE,
        RlcPduBuffer* pduFormed_p,
        UInt32 requestedDataSize,
        AmReTransmission* amReTxPdu_p,
        UInt8 *pollBit,
        AmNackListNode* amNackList_p,
        UInt8 segmentedPduType,
        RLCHarqFailureInfo* rlcHarqFailureInfo_p
        );
/* Rlc Rel 2.0 change end */


UInt32 rlcSendMaxReTxThresholdInd
(
    TxAMEntity* pAmTxE,
    AmReTransmission * amReTxPdu_p
);
UInt32 rlcInitAmdPduFixHeader
(
    TxAMEntity *pAmTxE,
    RlcAmdPduFixH *amdPduHdr_p
);
UInt32 rlcAssignRlcAmdPduFixHeader 
(
    UInt8 *base_data_p,
    RlcAmdPduFixH *amdPduHdr_p
);
#endif  /* AMDENCODE_H */
