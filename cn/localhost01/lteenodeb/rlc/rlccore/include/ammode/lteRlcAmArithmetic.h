/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcAmArithmetic.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:20 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *                  This file declares arithmatic operations for AM Entity.
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRlcAmArithmetic.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:20  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:11:49  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.3.28.5  2010/07/05 09:25:08  gur19479
 * updated for coding guideline
 *
 * Revision 1.3.28.4  2010/07/03 08:48:24  gur19479
 * updtaed for macros
 *
 * Revision 1.3.28.3  2010/07/03 06:02:41  gur19479
 * updated for resegmentation
 *
 * Revision 1.3.18.8  2009/12/15 17:08:05  gur19140
 * Stat variable issue resolved
 *
 * Revision 1.3.18.7  2009/12/04 07:38:46  gur19140
 * RLC AM State variable changes
 *
 * Revision 1.3.18.6  2009/11/26 08:21:39  gur19140
 * Downlink related uses
 *
 * Revision 1.3.18.5  2009/11/24 07:00:21  gur19140
 * resolve testing issue
 *
 * Revision 1.3.18.4  2009/11/12 09:24:15  gur19140
 * Minor Updatation
 *
 * Revision 1.3.18.3  2009/10/28 09:01:42  gur19140
 * Minor changes for file headers
 *
 * Revision 1.3.18.2  2009/10/28 05:36:00  gur19140
 * Added Arq functionality
 *
 * Revision 1.3.18.1  2009/10/14 05:26:14  gur19140
 * Changes regadring RLC AM Intial Phase
 *
 * Revision 1.3  2009/03/18 05:08:34  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.2  2009/01/29 05:35:22  gur20294
 * after code review comments incorporated
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 *
 ****************************************************************************/

#ifndef AMARITHMETIC_H
#define AMARITHMETIC_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcArithmetic.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/



#define RLC_PADDING_DONE 0x00
/* Macro to encode the D/C */
#define RLC_FILL_DC_AND_UPDATE_BIT_POS_IDX(buffer,dc,idx,bitPos)\
    buffer[idx] = (dc<<(--bitPos))
 
/* Macro to encode the CPT */
#define RLC_FILL_CPT_AND_UPDATE_BIT_POS_IDX(buffer,cpt,idx,bitPos)\
    bitPos -=RLC_AM_CPT_SIZE;\
    buffer[idx] |= (cpt<<(bitPos))

/* Macro to encode the FI */
#define RLC_FILL_FI_AND_UPDATE_BIT_POS_IDX(buffer,fi,idx,bitPos)\
    bitPos -=RLC_AM_FI_SIZE;\
    buffer[idx] |= (fi<<(bitPos))

/* Macro to encode the ACK SN */
#define RLC_FILL_ACK_SN_AND_UPDATE_BIT_POS_IDX(buffer,ack_sn,idx,bitPos)\
    buffer[idx++]|=(ack_sn>>(RLC_AM_SN_SIZE-bitPos));\
    if (bitPos < 2)\
        buffer[idx++] |= (((ack_sn)&(RLC_AM_SN_MASK>>bitPos))>>bitPos),\
            bitPos += RLC_BYTE;\
    buffer[idx] |= (((ack_sn)&(RLC_AM_SN_MASK>>bitPos)) \
        <<(RLC_BYTE -(RLC_AM_SN_SIZE - bitPos)));\
    bitPos =(RLC_BYTE - RLC_AM_MOD_8(RLC_AM_SN_SIZE - bitPos)); \
    if(!(bitPos<RLC_BYTE))\
       (bitPos=RLC_BYTE,idx++)
    //bitPos<RLC_BYTE?:(bitPos=RLC_BYTE,idx++)TDD Warning Fix

/* Macro to append padding if required */
#define RLC_CHECK_AND_ADDPEND_PADDING(buffer,idx,bitPos)\
   if(!(bitPos<RLC_BYTE))\
      (buffer[idx]|=RLC_PADDING_DONE)
//   bitPos>RLC_BYTE?:(buffer[idx]|=RLC_PADDING_DONE)TDD Warning Fix

/* Macro to encode the NACK SN */
#define RLC_FILL_NACK_SN_AND_UPDATE_BIT_POS_IDX(buffer,nack_sn,idx,bitPos)\
    RLC_FILL_ACK_SN_AND_UPDATE_BIT_POS_IDX(buffer,nack_sn,idx,bitPos)

/* Macro to encode the SN */
#define RLC_FILL_SN_AND_UPDATE_BIT_POS_IDX(buffer,sn,idx,bitPos)\
    RLC_FILL_ACK_SN_AND_UPDATE_BIT_POS_IDX(buffer,sn,idx,bitPos)

/* Macro to encode the E1 */
#define RLC_FILL_E1_AND_UPDATE_BIT_POS_IDX(buffer,e1,idx,bitPos)\
    buffer[idx] |= e1<<(--bitPos);\
    bitPos?:(bitPos=RLC_BYTE,idx++) 

/* Macro to encode the E2 */
#define RLC_FILL_E2_AND_UPDATE_BIT_POS_IDX(buffer,e2,idx,bitPos)\
    RLC_FILL_E1_AND_UPDATE_BIT_POS_IDX(buffer,e2,idx,bitPos)

/* Macro to encode the E */
#define RLC_FILL_E_AND_UPDATE_BIT_POS_IDX(buffer,e,idx,bitPos)\
    RLC_FILL_E1_AND_UPDATE_BIT_POS_IDX(buffer,e,idx,bitPos)

/* Macro to encode the P */
#define RLC_FILL_P_AND_UPDATE_BIT_POS_IDX(buffer,p,idx,bitPos)\
    RLC_FILL_E1_AND_UPDATE_BIT_POS_IDX(buffer,p,idx,bitPos)

/* Macro to encode the RF */
#define RLC_FILL_RF_AND_UPDATE_BIT_POS_IDX(buffer,rf,idx,bitPos)\
    RLC_FILL_E1_AND_UPDATE_BIT_POS_IDX(buffer,rf,idx,bitPos)

/* Macro to encode the LSF */
#define RLC_FILL_LSF_AND_UPDATE_BIT_POS_IDX(buffer,lsf,idx,bitPos)\
    RLC_FILL_E1_AND_UPDATE_BIT_POS_IDX(buffer,lsf,idx,bitPos)

/** SPR 2588 Changes Start **/

/*SPR 6862 Fix Start*/
/*SPR 17025 Fix start*/
#define RLC_FILL_SO_START_AND_UPDATE_BIT_POS_IDX(buffer,so_start,idx, bitPos) \
    do {\
        UInt32 tempSoStartVar = so_start; \
        tempSoStartVar = (tempSoStartVar << ((24 + bitPos - \
                                RLC_AM_SO_SIZE)% RLC_BYTE)); \
        UInt32 bufferData= ((RLC_AM_SO_SIZE - bitPos)<= RLC_BYTE)?(LTE_GET_U16BIT(&buffer[idx])):(LTE_GET_U24BIT(&buffer[idx]));\
        tempSoStartVar |= bufferData; \
        if(RLC_AM_SO_SIZE - bitPos <= RLC_BYTE) \
        LTE_SET_U16BIT(&buffer[idx],tempSoStartVar); \
        else \
        LTE_SET_U24BIT(&buffer[idx],tempSoStartVar); \
        idx += ((RLC_BYTE - bitPos + RLC_AM_SO_SIZE)/ 8); \
        /* SPR 18122 Changes Start*/\
        bitPos = 8 - MODULO((23 - bitPos),8); \
    /* SPR 18122 Changes End*/ \
     }while (0)
/*SPR 17025 Fix end*/
/*SPR 6862 Fix end*/

#define RLC_GET_ACK_SN_AND_UPDATE_BIT_POS_IDX(buffer,ack_sn,idx,bitPos)\
    do {\
        UInt32 tempAckSn = LTE_GET_U32BIT(&buffer[idx]); \
        ack_sn = (tempAckSn >> (24 + bitPos - RLC_AM_SN_SIZE)) & 0x03ff; \
      /* SPR 18122 Changes Start*/\
      idx += DIVIDE_BY_EIGHT(RLC_BYTE - bitPos + RLC_AM_SN_SIZE); \
        bitPos = 8 - MODULO((18 - bitPos),8); \
   /* SPR 18122 Changes End*/\
    } while (0)


#define RLC_GET_SO_START_AND_UPDATE_BIT_POS_IDX(buffer,so_start,idx, bitPos)\
    do { \
        UInt32 tempSoStart = LTE_GET_U32BIT(&buffer[idx]); \
        tempSoStart = tempSoStart >>(24 + bitPos - RLC_AM_SO_SIZE) ; \
        so_start = (tempSoStart & 0x7FFF); \
     /* SPR 18122 Changes Start*/\
     idx += DIVIDE_BY_EIGHT(RLC_BYTE - bitPos + RLC_AM_SO_SIZE) ; \
       bitPos = 8 - MODULO((23 - bitPos),8); \
   /* SPR 18122 Changes End*/\
    } while(0)


/** SPR 2588 Changes End **/

/* Macro to encode the SO END */
#define RLC_FILL_SO_END_AND_UPDATE_BIT_POS_IDX(buffer,so_end,idx,bitPos) \
    RLC_FILL_SO_START_AND_UPDATE_BIT_POS_IDX(buffer,so_end,idx,bitPos)

/* Macro to encode the SO */
#define RLC_FILL_SO_AND_UPDATE_BIT_POS_IDX(buffer,so,idx,bitPos) \
    RLC_FILL_SO_START_AND_UPDATE_BIT_POS_IDX(buffer,so,idx,bitPos)

/* Macro to encode the LI */
#define RLC_FILL_LI_AND_UPDATE_BIT_POS_IDX(buffer,li,idx,bitPos)\
    buffer[idx++]|=(li>>(RLC_AM_LI_SIZE-bitPos));\
    if (bitPos < 3)\
        buffer[idx++] |= (((li)&(RLC_AM_LI_MASK>>bitPos))>>bitPos),\
            bitPos += RLC_BYTE;\
    buffer[idx] |= (((li)&(RLC_AM_LI_MASK>>bitPos)) \
        <<(RLC_BYTE -(RLC_AM_LI_SIZE - bitPos)));\
    bitPos =(RLC_BYTE - RLC_AM_MOD_8(RLC_AM_LI_SIZE - bitPos)); \
    if(!(bitPos<RLC_BYTE))\
       (bitPos=RLC_BYTE,idx++)

    //bitPos<RLC_BYTE?:(bitPos=RLC_BYTE,idx++) TDD Warning Fix


/* Macro to decode the NACK SN */
#define RLC_GET_NACK_SN_AND_UPDATE_BIT_POS_IDX(buffer,nack_sn,idx,bitPos)\
    RLC_GET_ACK_SN_AND_UPDATE_BIT_POS_IDX(buffer,nack_sn,idx,bitPos)

/* Macro to decode the SN */
#define RLC_GET_SN_AND_UPDATE_BIT_POS_IDX(buffer,sn,idx,bitPos)\
    RLC_GET_ACK_SN_AND_UPDATE_BIT_POS_IDX(buffer,sn,idx,bitPos)

/* Macro to decode the E1 */
#define RLC_GET_E1_AND_UPDATE_BIT_POS_IDX(buffer,e1,idx,bitPos)\
    e1 = buffer[idx]>>(--bitPos)&0x01;\
    bitPos?:(bitPos=RLC_BYTE,idx++)

/* Macro to decode the E2 */
#define RLC_GET_E2_AND_UPDATE_BIT_POS_IDX(buffer,e2,idx,bitPos)\
    RLC_GET_E1_AND_UPDATE_BIT_POS_IDX(buffer,e2,idx,bitPos)

/* Macro to decode the E */
#define RLC_GET_E_AND_UPDATE_BIT_POS_IDX(buffer,e,idx,bitPos)\
    RLC_GET_E1_AND_UPDATE_BIT_POS_IDX(buffer,e,idx,bitPos)

/* Macro to decode the RF */
#define RLC_GET_RF_AND_UPDATE_BIT_POS_IDX(buffer,rf,idx,bitPos)\
    RLC_GET_E1_AND_UPDATE_BIT_POS_IDX(buffer,rf,idx,bitPos)

/* Macro to decode the LSF */
#define RLC_GET_LSF_AND_UPDATE_BIT_POS_IDX(buffer,lsf,idx,bitPos)\
    RLC_GET_E1_AND_UPDATE_BIT_POS_IDX(buffer,lsf,idx,bitPos)

/* Macro to decode the P */
#define RLC_GET_P_AND_UPDATE_BIT_POS_IDX(buffer,p,idx,bitPos)\
    RLC_GET_E1_AND_UPDATE_BIT_POS_IDX(buffer,p,idx,bitPos)

/* Macro to decode the D/C */
#define RLC_GET_DC_AND_UPDATE_BIT_POS_IDX(buffer,dc,idx,bitPos)\
    RLC_GET_E1_AND_UPDATE_BIT_POS_IDX(buffer,dc,idx,bitPos)


/* Macro to decode the SO END */
#define RLC_GET_SO_END_AND_UPDATE_BIT_POS_IDX(buffer,so_end,idx,bitPos) \
    RLC_GET_SO_START_AND_UPDATE_BIT_POS_IDX(buffer,so_end,idx,bitPos)

/* Macro to decode the SO */
#define RLC_GET_SO_AND_UPDATE_BIT_POS_IDX(buffer,so,idx,bitPos) \
    RLC_GET_SO_START_AND_UPDATE_BIT_POS_IDX(buffer,so,idx,bitPos)

/* Macro to decode the LI */
#define RLC_GET_LI_AND_UPDATE_BIT_POS_IDX(buffer,li,idx,bitPos)\
    li = buffer[idx++]&(BYTE_MASK>>(RLC_BYTE-bitPos));\
    if (bitPos<3)\
        li = ((li<<RLC_BYTE)|buffer[idx++]),bitPos+=RLC_BYTE;\
    bitPos=RLC_AM_LI_SIZE-bitPos;\
    li = ((li<<bitPos)|\
        (buffer[idx]&(BYTE_MASK<<(RLC_BYTE-bitPos)))>>(RLC_BYTE-bitPos));\
    bitPos < RLC_BYTE?(bitPos=RLC_BYTE-bitPos):idx++

/* Macro to decode the CPT */
#define RLC_GET_CPT_AND_UPDATE_BIT_POS_IDX(buffer,cpt,idx,bitPos)\
    bitPos -=RLC_AM_CPT_SIZE;\
    cpt=((buffer[idx]>>bitPos)&(RLC_AM_CPT_MASK>>bitPos))

/* Macro to decode the FI */
#define RLC_GET_FI_AND_UPDATE_BIT_POS_IDX(buffer,fi,idx,bitPos)\
    bitPos -=RLC_AM_FI_SIZE;\
    fi=((buffer[idx]>>bitPos)&(RLC_AM_FI_MASK>>bitPos))

/*cheching range of current tid in between first and last tid*/
#define TRANSID_RANGE_CHECK_SDUQ(ui32_ftid,ui32_ltid,ui32_ctid)\
    ui32_ftid<ui32_ltid?(ui32_ctid>ui32_ftid?(ui32_ctid<=ui32_ltid?1:0):0):\
            (ui32_ftid<ui32_ctid?(ui32_ctid<=-1?1:0):(ui32_ctid<=ui32_ltid?1:0))

#define GET_RETX_QUEUE_INDEX_FROM_SN(SEQNUM) \
                SEQNUM = SEQNUM & 511

 #define GET_LEAST_NACK_SN(leastNackRcv , nackSn, vtA, vtMS)\
      ( isAMLT(leastNackRcv, nackSn, vtA)? leastNackRcv: nackSn)
 

/* SPR 5599 Fix Start */
UInt32 isWithInTxWindow( UInt16 vtA, UInt16 vtSN, UInt16 vtMS);

int isWithInRxWindow( UInt16 vrUR,
                      UInt16 rxSN,
                      UInt16 vrMR
                    );
UInt32 isAMEqual( UInt16 x,
                  UInt16 y,
                  UInt16 modBase
                );
UInt32 isAMGT ( UInt16 x,
                UInt16 y,
                UInt16 modBase
              );
UInt32 isAMLT ( UInt16 x,
                UInt16 y,
                UInt16 modBase
              );
UInt32 isAMGTE ( UInt16 x,
                 UInt16 y,
                 UInt16 modBase
                      );
UInt32 isAMLTE ( UInt16 x,
                 UInt16 y,
                 UInt16 modBase
               );
/** RLC Nack List Changes Start **/
UInt16 getNackListSize ( UInt16 x,
                 UInt16 y
               );
/** RLC Nack List Changes End **/
/* SPR 5599 Fix End */




#endif  
