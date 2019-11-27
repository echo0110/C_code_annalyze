/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacTransmitter.h,v 1.1.1.1.6.1.4.2.2.1 2010/09/21 15:45:54 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : THIS FILE CONTAINS THE DATA STRUCTURES AND THE FUNCTION
 *                     DELCLEARATION FOR THE BUFFER (CONTROL/DATA) TO BE SENT
 *                     TO THE PHYSICAL CHANNEL INTERFACE. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacTransmitter.h,v $
 * Revision 1.1.1.1.6.1.4.2.2.1  2010/09/21 15:45:54  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.21  2009/11/20 06:12:47  gur11912
 * passed system Frame and Sub Frame in txDLControl, data and common channel legs
 *
 * Revision 1.20  2009/08/27 14:30:58  gur11912
 * Removed support of data message alongwith control message
 *
 * Revision 1.19  2009/08/25 05:51:25  gur12140
 * Optimized Mux
 *
 * Revision 1.18  2009/07/29 09:11:26  gur15697
 * UT bugfix
 *
 * Revision 1.17  2009/07/28 14:32:57  gur11912
 * HARQ Timer start moved to DL Data Message
 *
 * Revision 1.16  2009/07/11 08:41:43  gur18550
 * Removed Redundant functions of bit shifts
 *
 * Revision 1.15  2009/06/25 13:30:10  gur18550
 * merging error removed
 *
 * Revision 1.14  2009/06/24 16:12:56  gur18550
 * Final review Comments Inc
 *
 * Revision 1.13  2009/06/18 13:45:13  gur18550
 * Errors removed
 *
 * Revision 1.12  2009/06/12 05:01:02  gur18550
 * MIB &  Shared Memory Related Changes
 *
 * Revision 1.11  2009/06/11 10:37:12  gur18550
 * compilation errors removed
 *
 * Revision 1.10  2009/06/11 07:55:09  gur18550
 * File Updated
 *
 * Revision 1.9  2009/06/11 07:04:48  gur18569
 * removed compilation error
 *
 * Revision 1.8  2009/06/11 05:19:02  gur18550
 * MAC_REGISTER_REQ Added on Revant's Request
 *
 * Revision 1.7  2009/06/10 15:14:59  gur18569
 * removed linking and compilation errors
 *
 * Revision 1.6  2009/05/28 12:43:49  gur18550
 * Review Comments Incorporated
 *
 * Revision 1.5  2009/05/27 04:34:02  gur18550
 * API VALUE updated
 *
 * Revision 1.4  2009/05/19 15:22:25  gur18550
 * Changes in Memory allocation for Data
 *
 * Revision 1.3  2009/05/19 06:56:01  gur18550
 * Function Updated
 *
 * Revision 1.2  2009/05/14 16:52:41  gur18550
 * Function Updated
 *
 * Revision 1.1  2009/04/30 05:08:00  gur18550
 * INITIAL VERSION
 *
 * ORIGINAL CREATION:   GUR18550 (ANSHUMAN BOSE)
 *
 ****************************************************************************/

#ifndef LTE_MAC_TRANSMITTER_H
#define LTE_MAC_TRANSMITTER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacPHICH.h"
#include "lteMacPhyInterface.h"
#include "lteMacDLHarqMgr.h"
#include "logging.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

#define INVALID_MSG_IDENTIFIER 255
#define LEN_BCH_PDU 6
/* SPR 1365 PCH_SPR_START */ 
#define FAPI_PCH_QPSK 0
/* SPR 1365 PCH_SPR_End */ 
 
/* SPR# 892 Start  */
#define FAPI_RA_POLICY_2 2
/* SPR# 892 Fix End */
 /*SPR 21993 Fix Start */
#define FAPI_RA_POLICY_2_1C 3
 /*SPR 21993 Fix End */

#define LEN_BCH_PDU 6
#define TB_1_INDEX 1
#define TB_2_INDEX 2
#define INVALID_MSG_IDENTIFIER 255
#ifdef LTE_EMBMS_SUPPORTED
#define MCH_PDU_RNTI 0xFFFD
#endif

/*
    The MAX_SIZE_OF_DCI_PDU contains the size of dci pdu + the size of dci  
    format 2 (including the size of TB_INFO)
*/
#define MAX_SIZE_OF_DCI_PDU 36

/*
    The size of HI PDU is fixed to 8 bytes.
*/
#define MAX_SIZE_OF_HI_PDU 20
/*
   The FIXED_SIZE_OF_DL_CONTROL_MSG also contains the 10 bytes for 
   the API HEADER
*/
#define FIXED_SIZE_OF_DL_CONTROL_MSG 20

#define CONTROL_BUFFER_SIZE (FIXED_SIZE_OF_DL_CONTROL_MSG + MAX_SIZE_OF_DCI_PDU * \
                            (MAX_DL_UE_SCHEDULED + MAX_UL_UE_SCHEDULED) * 2 +\
                             MAX_SIZE_OF_HI_PDU * MAX_UL_UE_SCHEDULED)

#ifdef FDD_CONFIG

#define SHIFT_EIGHT_BITS_MASK(x) (x & 0xFF)
#define SHIFT_SIXTEEN_BITS_MASK(x) ((x & 0xFF00) >> 8)
#define SHIFT_TWENTY_FOUR_BITS_MASK(x) ((x & 0xFF0000) >> 16)
#define SHIFT_THIRTY_TWO_BITS_MASK(x) ((x & 0xFF000000) >> 24)



/****************************************************************************
 * Exported Types
 ****************************************************************************/



/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/


MacRetType createDLControlDCIFormat0Buffer (UInt8 *controlBufferForPhy,
                                            DCIPduInfo* tempDciPduInfo_p,
                                            UInt32 *index);

MacRetType createDLControlDCIFormat1Buffer(UInt8 *controlBufferForPhy,
                                           DCIPduInfo* tempDciPduInfo_p,
                                           UInt32 *index);


MacRetType createDLControlDCIFormat1ABuffer(UInt8 *controlBufferForPhy,
                                            DCIPduInfo* tempDciPduInfo_p,
                                            UInt32 *index);


MacRetType createDLControlDCIFormat1BBuffer(UInt8 *controlBufferForPhy,
                                            DCIPduInfo* tempDciPduInfo_p,
                                            UInt32 *index);



MacRetType createDLControlDCIFormat1CBuffer(UInt8 *controlBufferForPhy,
                                            DCIPduInfo* tempDciPduInfo_p,
                                            UInt32 *index);

MacRetType createDLControlDCIFormat1DBuffer(
                                            UInt8 *controlBufferForPhy,
                                            DCIPduInfo* tempDciPduInfo_p,
                                            UInt32 *index);

MacRetType createDLControlDCIFormat2Buffer(UInt8 *controlBufferForPhy,
                                           DCIPduInfo* tempDciPduInfo_p,
                                           UInt32 *index);


MacRetType createDLControlDCIFormat2ABuffer(UInt8 *controlBufferForPhy,
                                            DCIPduInfo* tempDciPduInfo_p,
                                            UInt32 *index);


#ifdef FDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType createAndSendDataBufferReq( 
/* SPR 5599 changes end (ZIP ID 129059) */
                                  UInt16 sysFrameNumber,
                                  UInt8 subFrameNumber, 
                                  UInt8 harqSubFrameNum,
                                  /* SPR 15909 fix start */
                                  tickType_t scheduledExpiryTick,
                                  /* SPR 15909 fix end */
                                  DciCCEInfo* tempDciCCEInfo_p);
#elif TDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType createAndSendDataBufferReq( 
/* SPR 5599 changes end (ZIP ID 129059) */
                                  UInt16 sysFrameNumber,
                                  UInt8 subFrameNumber, 
                                  UInt8 harqSubFrameNum,
                                  /* SPR 15909 fix start */
                                  tickType_t scheduledExpiryTick,
                                  DciCCEInfo* tempDciCCEInfo_p,
                                  tickType_t currentGlobalTick,
                                  /* SPR 15909 fix end */
                                  UInt32 phyDelay
                                  );

#endif								  



/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType createAndSendCmnChannelData(
/* SPR 5599 changes end (ZIP ID 129059) */
                                UInt16 sysFrameNumber,
                                UInt8 subFrameNumber,
                                /* +- SPR 17777 */
#ifdef TDD_CONFIG
                                /* SPR 15909 fix start */
                                tickType_t currentGlobalTick,
                                /* SPR 15909 fix end */
                                UInt8 phyDelay,
#endif
                                /* +- SPR 17777 */
                                DciCCEContainerInfo* dciCCEContainerInfo_p,
                                InternalCellIndex internalCellIndex);




/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType txCmnChannelData(
/* SPR 5599 changes end (ZIP ID 129059) */
                                   UInt8 ulDelay,
                                   /* SPR 15909 fix start */
                                   tickType_t currentGlobalTick,
                                   /* SPR 15909 fix end */
                                   UInt32 sysFrame,
                                   UInt32 subFrame,
                                   InternalCellIndex internalCellIndex);


/* SPR 5599 changes start (ZIP ID 129059) */
void createDLDataMsgReq( UInt8 *dataBufferForPhy,
/* SPR 5599 changes end (ZIP ID 129059) */
                               DciCCEInfo* tempDciCCEInfo_p,
                               UInt32 *index,
                               UInt8 multipleTB);





/* SPR 5599 changes start (ZIP ID 129059) */
void createDLDataMsgReqForCmnChannel(
/* SPR 5599 changes end (ZIP ID 129059) */
                           UInt8 *dataBufferForPhy,
                           DciCCEInfo* tempDciCCEInfo_p,
                           DCICCEInfoCmnChannel* tempDCICCEInfoCmnChannel_p,
                           /* +- SPR 17777 */
                           UInt32 *index);


/* SPR 5599 changes start (ZIP ID 129059) */
void createDLDataMsgReqForMIB(
/* SPR 5599 changes end (ZIP ID 129059) */
                           UInt8 *dataBufferForPhy,
                           DciCCEContainerInfo* dciCCEContainerInfo_p,
                           UInt32 *index);

/* SPR 5599 changes start (ZIP ID 129059) */
void txMacRegisterReq(InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129059) */

#endif

/* SPR 5599 changes start (ZIP ID 129059) */
/* + SPR 17439 */
UInt16 getTransactionId(void);
/* - SPR 17439 */
MacRetType txDLControlMsg(UInt8 ulDelay,
/* SPR 5599 changes end (ZIP ID 129059) */
                                  /* SPR 15909 fix start */
                                 tickType_t currentGlobalTick,
                                  /* SPR 15909 fix end */
                                 UInt32 sysFrame,
                                 UInt32 subFrame,
                        InternalCellIndex internalCellIndex);
/* SPR 5599 changes start (ZIP ID 129059) */
/* SPR 5346 changes start */                                 
MacRetType dummyTxDLControlMsg(UInt8 ulDelay,
        /* +- SPR 17777 */
                                      UInt32 sysFrame,
                                      UInt32 subFrame,
                           InternalCellIndex internalCellInde);
/* SPR 5346 changes end */                                 

/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType txDLDataMsgReq(
/* SPR 5599 changes end (ZIP ID 129059) */
								  UInt8 ulDelay,
				                                  /* SPR 15909 fix start */
								  tickType_t currentGlobalTick,
                                				  /* SPR 15909 fix end */
								  UInt32 sysFrame,
								  UInt32 subFrame,
                                 InternalCellIndex internalCellIndex);


/* SPR 5599 changes start (ZIP ID 129059) */
/* SPR 609 changes start */  
void createDLDataMsgReqForMIB(UInt8 *dataBufferForPhy,
/* SPR 5599 changes end (ZIP ID 129059) */
                                            DciCCEContainerInfo* dciCCEContainerInfo_p,
                                            UInt32 *index);

/* SPR 5599 changes start (ZIP ID 129059) */
void createDLDataMsgReqForCmnChannel(UInt8 *dataBufferForPhy,
/* SPR 5599 changes end (ZIP ID 129059) */
                                                   DciCCEInfo* tempDciCCEInfo_p,
                                                   DCICCEInfoCmnChannel* tempDCICCEInfoCmnChannel_p,
                                                   /* +- SPR 17777 */
                                                   UInt32 *index);
#ifdef TDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType ulHarqTimerStart( UInt8 subframe,
/* SPR 5599 changes end (ZIP ID 129059) */
                                    UInt16 sysFrameNum,   
                                    UInt16 ueIndex,
                                    UInt8 harqProcessId,
                                    /* + SPS_TDD_Changes */
                                    UInt8 schUsed,
                                    /* - SPS_TDD_Changes */
                                    /*CA Changes start */
                                    InternalCellIndex internalCellIndex,
                                    /*CA Changes end */
				                    /*TDD Config 0 Changes Start*/
                                    UInt8 ulSubFrameNum);
                                    /*TDD Config 0 changes End*/
#endif
/* SPR 609 changes end */ 
/*OPT_SPR_4119_4120_4122_4123_CHANGES_START*/
void init_ul_msgbuff(UInt8 numOfCells);
void init_dl_msgbuff(UInt8 numOfCells);
/*OPT_SPR_4119_4120_4122_4123_CHANGES_END*/


#ifdef DL_UL_SPLIT_TDD
MacRetType txDLHiDci0Msg(UInt8 phyDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 sysFrame,
        UInt32 subFrame);
#else
 MacRetType txDLHiDci0Msg(UInt8 phyDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 sysFrame,
        UInt32 subFrame,
        InternalCellIndex internalCellIndex);
#endif
/*mandeep1 changes start*/
/*mandeep1 changes stop*/

#endif  /* LTE_MAC_TRANSMITTER_H */
