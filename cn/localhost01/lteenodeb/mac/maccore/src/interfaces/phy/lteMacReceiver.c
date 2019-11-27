/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacReceiver.c,v 1.3.6.1.4.2.2.9.4.3 2010/11/10 06:39:10 gur10121 Exp $
 *
 ******************************************************************************
 *
 *  File Description : The file contains the functions interfacing the MAC 
 *                     and receiving messages from the Physical layer. 
 *                     - Reading the message from the socket/message Queue, 
 *                       decoding it and then  call the different function 
 *                       correspondence to the the API ID.(Uplink Control 
 *                       Message, Uplink Data Message,Error Indication message,
 *                       Subframe indication. )
 *
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacReceiver.c,v $
 * Revision 1.3.6.1.4.2.2.9.4.3  2010/11/10 06:39:10  gur10121
 * Entry is SR queue  modified
 *
 * Revision 1.3.6.1.4.2.2.9.4.2  2010/11/09 13:32:59  gur10121
 * Container Tick is modified from current tti to recv tti
 *
 * Revision 1.3.6.1.4.2.2.9.4.1  2010/11/08 06:06:23  gur10121
 * Semi static changes merging
 *
 * Revision 1.3.6.1.4.2.2.9  2010/10/29 15:54:08  gur20056
 * rupesh - multi attach detach fix in pdcpadapter ULTG DLTG pdcp and mac
 *
 * Revision 1.3.6.1.4.2.2.8  2010/10/25 08:36:45  gur23971
 * stats related changes
 *
 * Revision 1.3.6.1.4.2.2.7.2.1  2010/10/27 17:13:53  gur10121
 * UT fixes + review comments incorporation
 *
 * Revision 1.3.6.1.4.2.2.7  2010/10/22 12:44:10  gur04640
 * Statistics code merged from branch BRFramework_RRC1_0_RLC_AM_MAC_8_8_FAPI_optimizations
 *
 * Revision 1.3.6.1.4.2.2.6.2.1  2010/10/13 10:57:13  gur04640
 * Changes done for implementing Stats using shared memory
 *
 * Revision 1.3.6.1.4.2.2.6.2.1  2010/10/12 02:45:30  gur04640
 * Changes done for implementing Stats using shared memory
 *
 * Revision 1.3.6.1.4.2.2.6  2010/10/04 12:07:38  gur10121
 * SPR 688 Fix. Move the ltewarning to UT info Trace
 *
 * Revision 1.3.6.1.4.2.2.5  2010/09/29 11:55:43  gur04640
 * removed ltePanic on UL packet drop
 *
 * Revision 1.3.6.1.4.2.2.4  2010/09/28 09:33:53  gur10694
 * Adding format specifier to print error code also
 *
 * Revision 1.3.6.1.4.2.2.3  2010/09/28 06:19:57  gur10121
 * UE crash RNTI change
 *
 * Revision 1.3.6.1.4.2.2.2  2010/09/27 14:38:12  gur10694
 * UE config Fix and Review comments
 *
 * Revision 1.3.6.1.4.2.2.1  2010/09/21 15:46:09  gur20491
 * FAPI changes
 *
* Revision 1.59.20.4  2010/08/30 16:28:31  gur24420
* removed unwanted code
*
* Revision 1.59.20.3  2010/08/12 10:18:25  gur24420
* Changes made to encode MAC-PHY Interfaces messages in little endian format
*
* Revision 1.59.20.2  2010/08/03 10:53:08  gur24420
* Converted transport notation to Network byte order
*
* Revision 1.59.20.1  2010/07/21 09:34:58  gur24420
* Aligned changes with respect to eNode Integration activity(based on RMI-NetLogic Code).
* Overall Changes Cover:-
* 1. Created memory pool for UL data
* 2. Added a warning log statement
* 3. Commented few lines
* 4. Endianness fix.
* 5. Changed data type used for port number variable from u32 to u16
*
* Revision 1.59  2010/03/26 10:47:16  gur15697
* Fix related to Demultiplexing of UL data when multiple logical channels are there in case of zero buffer copy
*
* Revision 1.58  2009/12/22 05:08:16  gur11912
* modified ackNackEntry_p->ttiCounter to ackNackNode_p->packetSubFrame
*
* Revision 1.57  2009/12/18 10:43:38  gur11912
* passed subFrame instead of currentTTI to DLAckNackNode
*
* Revision 1.56  2009/12/01 15:39:18  gur19413
* added new function call recvDataFromPhysicalNonBlocking
*
* Revision 1.55  2009/11/20 11:16:06  gur18550
* delta Bsr Updation
*
* Revision 1.54  2009/10/30 06:56:39  gur11912
* Reverted RAW sockets changes
*
* Revision 1.53  2009/10/21 05:11:40  gur19413
* Socket Functionality Modified
*
* Revision 1.52  2009/10/05 08:07:20  gur11912
* optimization changes
*
* Revision 1.51  2009/09/24 20:48:06  gur19413
* handling SR
*
* Revision 1.50  2009/09/24 17:18:10  gur11912
* processOnlySubFrame modified for msgId other than subframe indication
*
* Revision 1.49  2009/09/16 11:17:14  gur19413
* removed compilation error
*
* Revision 1.48  2009/09/16 09:54:15  gur11912
* updated for all ack case
*
* Revision 1.47  2009/09/09 13:58:05  gur11912
* fixed for memory leak
*
* Revision 1.46  2009/09/02 15:45:34  gur19413
* socket interface comm with phy bug fix.
*
* Revision 1.45  2009/08/28 12:54:32  gur19413
* processOnlySubFrameInd modified for subframe handling
*
* Revision 1.44  2009/08/24 12:51:59  gur11912
* macPhyRegistrationSuccessful_g defined as volatile
*
* Revision 1.43  2009/08/24 06:10:32  gur19413
* added UT_TESTING flag
*
* Revision 1.42  2009/07/29 15:59:41  gur19413
* ut bug fixed
*
* Revision 1.41  2009/07/28 12:33:07  gur19413
* validation CQI related changes.
*
* Revision 1.40  2009/07/28 06:57:57  gur19413
* UT bugs fixed
*
* Revision 1.39  2009/07/24 06:51:52  gur19413
* added new function processOnlySubFrameInd
*
* Revision 1.38  2009/07/22 07:30:32  gur19413
* msgSegNext error related changes.
*
    * Revision 1.37  2009/07/16 11:56:23  gur19413
* set zeroBuffer_p =PNULL (processControlRX)
    *
    * Revision 1.36  2009/07/16 10:12:52  gur19413
* removed unused line (segment)
    *
    * Revision 1.35  2009/07/16 09:20:31  gur19413
    * RACH UT bug fixed
    *
    * Revision 1.34  2009/07/15 13:10:34  gur19413
    * CRC Invalid handling changed.
    *
    * Revision 1.33  2009/07/15 06:44:34  gur19413
    * added new logs for testing
    *
    * Revision 1.32  2009/07/14 06:36:46  gur19413
    * changed global variable to frequencySelective_g
    *
    * Revision 1.31  2009/07/13 12:08:13  gur19413
    * un comment the code
    *
    * Revision 1.30  2009/07/13 10:14:13  gur18569
    * removed UT bugs
    *
    * Revision 1.29  2009/07/13 09:20:14  gur19413
    * added warning statement in validateTick
    *
    * Revision 1.28  2009/07/13 04:33:52  gur19413
    * removed warnings
    *
    * Revision 1.27  2009/07/13 04:27:56  gur19413
    * changed log type
    *
    * Revision 1.26  2009/07/10 05:43:23  gur19413
    * ONE_BYTE macro change to SHIFT_EIGHT_BITS Macro
    *
    * Revision 1.25  2009/07/09 06:08:54  gur19413
    * added ttiCounter in dl harq node.
    *
    * Revision 1.24  2009/07/09 05:45:36  gur18569
    * fixed ut bug
    *
    * Revision 1.23  2009/07/07 05:03:51  gur19413
    * changes in ERROR IND.
    *
    * Revision 1.22  2009/07/02 07:45:07  gur19413
    * UT Bugs Removed
    *
    * Revision 1.20  2009/06/23 11:15:17  gur19413
    * added SR queue variable
    *
    * Revision 1.19  2009/06/22 08:52:07  gur15697
    * RACH review comments incorporated
    *
    * Revision 1.18  2009/06/22 08:16:53  gur19413
    * updated validatePHYDelayAndTick function
    *
    * Revision 1.17  2009/06/22 07:18:23  gur19413
    * remove UT_TESTING macro
    *
    * Revision 1.16  2009/06/22 04:50:59  gur19413
    * changing CQI Handling
    *
    * Revision 1.15  2009/06/19 10:31:45  gur19413
    * added ueIndex in Cqi Node and remove validation range check for RNTI.
    *
    * Revision 1.14  2009/06/19 08:19:51  gur19413
    * changed sr queue from globalSrQueue_g to ulGrantGlobalSrQueue_g
    *
    * Revision 1.13  2009/06/18 14:16:50  gur19413
    * change Sr node structure
    *
    * Revision 1.12  2009/06/18 14:07:34  gur19413
    * added new check for ue pending delete flag
    *
    * Revision 1.11  2009/06/17 00:13:51  gur19413
    * bugs fixed regarding zero buffer copy
    *
    * Revision 1.10  2009/06/10 12:41:15  gur19413
    * comments incorportated
    *
    * Revision 1.8  2009/06/09 12:42:09  gur19413
    * added function prototype in private section
    *
    * Revision 1.7  2009/06/05 15:10:21  gur18569
    * removed compilation errors
    *
    * Revision 1.6  2009/06/05 08:51:42  gur19413
    * function prototype changed
    *
    * Revision 1.5  2009/06/04 10:10:59  gur19413
    * remove compilation error
    *
    * Revision 1.4  2009/06/04 09:42:44  gur19413
    * added comments
    *
    * Revision 1.3  2009/06/02 09:18:14  gur19413
    * remove compilation error
    *
    * Revision 1.2  2009/05/28 08:46:21  gur19413
    * Initial Version
    *
    * Revision 1.1  2009/05/25 22:49:24  gur19413
    * Initial Version
    *
    *
    *****************************************************************************/


/******************************************************************************
     * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
    * Project Includes
*****************************************************************************/
#include "lteMacCellConfMgr.h" 
#include "lteFapiMacPhyInterface.h"
#include "lteMacReceiver.h"
#include "lteMacRandomAccess.h"
#include "lteMacSFNMgr.h"
#include "lteMacTimingAdvanceMgr.h"
#include "lteMacDemux.h"
#include "lteMacParseUtil.h"
#include "lteMacDLUEContext.h"
#include "lteMacULSchedulerStrategy.h"
#include "lteMacParseOAMMsg.h"
#include "lteMacOAMInterface.h"
#include "lteMacULCSIManager.h" /* ULA_CHG */
    /*SPR_3305_FIX_START*/
/* msg3_retx_tdd_support_start */
#include "lteMacRandomAccess.h"
/* msg3_retx_tdd_support_end */
    /*SPR_3305_FIX_END*/
    /* +DYNAMIC_ICIC */
#include "lteMacRRMPeriodicMgr.h"
    /* -DYNAMIC_ICIC */
#include "lteMacULPowerControlData.h"
#include "lteCommonStatsManager.h"
#include "lteMacDLSpsScheduler.h"
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
#include "lteMacTddMgr.h"
#ifdef OTA_DL_ADVANCE_2
#include "lteMacPUCCH.h" 
#endif
#endif
#include "lteMacPhyCellReconfig.h" 

/* SPR 19288 change start */
/* SPR 19288 change end */
#ifdef LTE_EMBMS_SUPPORTED
#include "lteMacEmbmsPorting.h"    
#endif    
#ifdef LAYER2_PHYSIM_THREAD
#include "lteEnbPhySim.h"
    /* + E_CID_5.3 */
#include "lteMacECIDMgr.h"
    /* - E_CID_5.3 */

#ifdef PHYSIM_DEBUG
    extern UInt16 sfnsf_g ;
#endif
/* + PERF_CA GBL */
extern UInt16   lLteMacStatsRfSfn_g[MAX_NUM_CELL];
extern UInt16   lLteMacStatsRfSf_g[MAX_NUM_CELL];
/* - PERF_CA GBL */

#endif
#ifdef OVERSHOOT_CORRECTION
    extern OverLoadLevel currTickOverLoad_g[MAX_NUM_CELL];  
#endif

#include "ltePerfStats.h"

#include "lteMacULSpsScheduler.h"

/******************************************************************************
      Private Definitions
*****************************************************************************/
    /*FAPI L2 AS MASTER START */
#ifdef FAPI_L2_MASTER
    /*SPR 13047 fix*/
    extern UInt8 startExecutionAfterPhyStart_g[MAX_NUM_CELL];
#endif
    /*FAPI L2 AS MASTER END */
/* SPR 19288 change start */
/* SPR 19288 change end */


/*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
#include "lteMacULResourceInfoMgr.h"
#endif
/*TDD Config 0 Changes End*/
#include "lteMacPUSCH.h"
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    HdUeContextInfo hdUeContextInfo_g;
#endif
    /*HD FDD Changes End*/
    /* RX_IND_PORTING Changes */
    /*CA Changes start  */
 extern LTE_SQUEUE* recvDataFromPhyQ_gp[MAX_NUM_CELL];
 extern StatsInfo statsInfo_g[MAX_NUM_CELL];
 extern UlConfigMap ulConfigMap_g[MAX_NUM_CELL][MAX_UL_CONFIG_CONTAINER_MAP_SIZE];
 extern TpcSentMap tpcSentMap_g[MAX_NUM_CELL][MAX_TPC_CONTAINER_MAP_SIZE];


#ifdef TDD_CONFIG
extern ULResourceAllocationInfo ulRAInfo_g[MAX_RA_INFO_INDEX];
/* This index specifies the updated RRM information */
extern UInt8  ulRAInfoIndex_g[MAX_NUM_CELL];
#endif

/******************************************************************************
      Private Types
*****************************************************************************/
/*SPR 15909 Fix start*/
extern void (*processULSchIndFunction)(void *,
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
/* + SPR_10185 */    
extern void (*processSRSIndicationFunction)(void *,
        void *,
        tickType_t ,InternalCellIndex );
/* - SPR_10185 */    
extern void  (*processRACHIndicationFunction)(void *, tickType_t,FAPI_VendorSpecificForRACHInd_st *,InternalCellIndex );
extern void (* processCQIIndicationFunction)(void *,
        void *,
        tickType_t,
        /*SPR 5620 START*/
#ifdef TDD_CONFIG
        UInt16,
#endif
        /*SPR 5620 END*/
        UInt16,
        InternalCellIndex
        );
extern void (* processHARQIndicationFunction)(void *,tickType_t,UInt16,UInt8,InternalCellIndex );
/*SPR 15909 Fix end*/
#ifdef LTE_EMBMS_SUPPORTED
#ifdef TDD_CONFIG
extern UInt32 sendMacTriggerToSyncAndCheckMacPreOppCalc (
        UInt32 dlDelay,
        InternalCellIndex internalCellIndex);
#endif
#endif
extern void enableUplinkThroughput(UInt32 enable, InternalCellIndex internalCellIndex);
extern void initCQIMgr(UInt8 numOfCells);


/*SPR 21347 fix start*/
/*SPR 21347 fix end*/

STATIC SInt8 offsetLevel_g[] = {0,1,2,
    WIDEBAND_SPATIAL_DIFFERENTIAL_CQI_AT_INDEX_3,
    -WIDEBAND_SPATIAL_DIFFERENTIAL_CQI_AT_INDEX_4,
    -3,-2,-1};
/* As per table 7.2.1-4 (Mapping of differential CQI value to Offset level) 
 * in Specs 36.213 ver 9.3.0 
 */
STATIC UInt8 offsetLevelForUESelectedSubbandMode_g[] = {
    UE_SELECTED_DIFFERENTIAL_CQI_AT_INDEX_0,
    2,3,
    UE_SELECTED_DIFFERENTIAL_CQI_AT_INDEX_3};

/* As per table 7.2.1-2 (Mapping of differential CQI value to Offset level) 
 * in Specs 36.213 ver 9.3.0 
 */
STATIC SInt8 offsetLevelForHigherLayerConfiguredMode_g[] = {0,1,
    HIGHER_LAYER_SUBBAND_DIFFERENTIAL_CQI_AT_INDEX_2,
    -HIGHER_LAYER_SUBBAND_DIFFERENTIAL_CQI_AT_INDEX_3};
/*mandeep changes start*/
/*mandeep changes stop*/
#ifdef TDD_CONFIG

#define MAX_M_COUNT_FOR_CONFIG_TWO   4
#define MAX_M_COUNT_FOR_CONFIG_THREE 3
#define MAX_M_COUNT_FOR_CONFIG_FOUR  4

/* SPR 19679: TDD HARQ Multiplexing Changes Start */
/* Storing HARQ status index according to table Table 10.1.3.1-1 ,
 * specification 36.213. This is impacting only config 2,3,4 and M > 3 */
UInt8 harqToDLSfMappingForMultiplexing_g[TOTAL_NUM_UL_DL_CONFIG][MAX_SUB_FRAME] =
   {
     {0,0,0,0,0,0,0,0,0,0},
     {0,0,0,0,0,0,0,0,0,0},
     {1,3,0,2,0,1,3,0,2,0},
     {0,1,0,0,0,2,0,0,0,0},
     {0,2,0,0,3,1,1,2,3,0},
     {0,0,0,0,0,0,0,0,0,0},
     {0,0,0,0,0,0,0,0,0,0}    
   };

#define MAX_M_COUNT_FOR_CONFIG_ONE   2

#define INVALID_DL_SF_INDEX_VALUE 10

#define BIT_DECODING_FOR_HARQ_MULTIPLEXING(ackNackType,operation)\
{\
    /* Decoding value for a particular DL sf and copy same value 
    * for both codeword in first and second nibble of ackNackType */ \
    ackNackType = ((operation & 0x07)| ((operation & 0x07) << 4)); \
}    
/* Decoding HARQ according to feedback mode multiplexing and M = 2 */
#define  CHECK_PUCSH_PUCCH_TX_AND_STORE_HARQ_STATUS_WITH_MULTIPLEXING_AND_M_TWO(\
         containerIndex,dlUEContext_p,sf,opType,operation)\
{\
    UInt8 isHarqOnPusch = 0;\
    UInt8 isHarqOnPucch = 0;\
    UInt8 dlSF = 0; \
    UInt8 isNewOrAdaptiveTransmission  = 0; \
    UInt8 spsOccasionIndex = INVALID_DL_SF_INDEX_VALUE; \
    UInt8 schUsed = DRA_SCHEDULING; \
    \
    CHECK_IS_HARQ_BLER_RECEIVED_ON_PUCCH(dlUEContext_p->ueIndex,\
            sf,isHarqOnPucch); \
    \
    isHarqOnPusch = !(isHarqOnPucch);\
    \
    /* Checking whether this PUSCH transmission was NEW transmission
     * or adaptive retransmission */ \
    CHECK_IS_PUSCH_TRANSMISSION_WITH_DCI0(dlUEContext_p->ueIndex,\
            sf,isNewOrAdaptiveTransmission);  \
    \
    /* This is the case when PUSCH transmission was done with DCI0 i.e.
     * new transmission or adaptive retransmssion */\
    if (isHarqOnPusch && isNewOrAdaptiveTransmission) \
    {    \
        for (dlSF = 0; dlSF < MAX_M_COUNT_FOR_CONFIG_ONE; dlSF++) \
        {\
            /* Check if scheduling was done for a particular DL subframe */  \
            if ((dlUEContext_p->pdschTxMapContainer[containerIndex][sf]) \
                    & (bitmask[opType[dlSF].dlSubFrameNum])) \
            { \
                schUsed = dlUEContext_p->dlHarqContext_p->harqProcess[dlUEContext_p->\
                     harqProcessId[opType[dlSF].containerIndex][opType[dlSF].dlSubFrameNum]].schUsed; \
                \
                /* Check if this DL-sf was SPS  occasion or normal Dl data
                 * scheduling */ \
                \
                if (DRA_SCHEDULING == schUsed) \
                { \
                    BIT_DECODING_FOR_HARQ_MULTIPLEXING(opType[dlSF].ackNackType,\
                                                        operation); \
                    operation >>= 4;  \
                } \
                else \
                { \
                    /* Storing DL-Sf of SPS occasion */\
                    spsOccasionIndex = dlSF; \
                } \
            } \
        }\
        /* If any DL-Sf was SPS occasion then store HARQ status at 
         * spsOccasionIndex and decode HARQ bits from operation  
         * at index after normal data scheduling */ \
        if (spsOccasionIndex != INVALID_DL_SF_INDEX_VALUE) \
        { \
            BIT_DECODING_FOR_HARQ_MULTIPLEXING( \
                    opType[spsOccasionIndex].ackNackType,operation); \
        }    \
    }    \
    else \
    { \
        /* This is case of HARQ on PUCCH / PUSCH without DCI i.e. non adaptive
         * retransmission or UL -SPS occasion */ \
        opType[0].ackNackType = (operation & 0x07)|((operation & 0x07) << 4);\
        opType[1].ackNackType = (((operation & 0x70) >> 4)| \
                                (((operation & 0x70) >> 4) << 4));\
    } \
/* SPR 19679: TDD HARQ Multiplexing Changes Start */\
    /*Code Removed*/\
/* SPR 19679: TDD HARQ Multiplexing Changes End */\
}

/* Decoding HARQ according to feedback mode multiplexing and M = 3/4 */
#define  CHECK_PUCSH_PUCCH_TX_AND_STORE_HARQ_STATUS_WITH_MULTIPLEXING_AND_M_THREE_OR_FOUR(\
                dlUEContext_p,sf,isHarqOnPucch,isHarqOnPusch,dlSF,opType,\
                operation,countM,containerIndex,tddConfig)\
{\
    UInt8 isNewOrAdaptiveTransmission  = 0; \
    UInt8 spsOccasionIndex = INVALID_DL_SF_INDEX_VALUE; \
    UInt8 schUsed = DRA_SCHEDULING; \
    UInt8 harqStatus = 0;\
    UInt8 *harqToDLSfMappingForMultiplexing = \
                    harqToDLSfMappingForMultiplexing_g[tddConfig];\
    \
    CHECK_IS_HARQ_BLER_RECEIVED_ON_PUCCH(dlUEContext_p->ueIndex,\
            sf,isHarqOnPucch); \
    \
    isHarqOnPusch = !(isHarqOnPucch);\
    \
    /* Checking whether this PUSCH transmission was NEW transmission
     * or adaptive retransmission */ \
    CHECK_IS_PUSCH_TRANSMISSION_WITH_DCI0(dlUEContext_p->ueIndex,\
            sf,isNewOrAdaptiveTransmission);  \
    \
    /* This is the case when PUSCH transmission was done with DCI0 i.e.
     * new transmission or adaptive retransmssion */\
    if (isHarqOnPusch && isNewOrAdaptiveTransmission) \
    {    \
        for (dlSF = 0; dlSF < countM; dlSF++) \
        {\
            /* Check if scheduling was done for a particular DL subframe */  \
            if ((dlUEContext_p->pdschTxMapContainer[containerIndex][sf]) \
                    & (bitmask[opType[dlSF].dlSubFrameNum])) \
            { \
                schUsed = dlUEContext_p->dlHarqContext_p->harqProcess[dlUEContext_p->\
                    harqProcessId[opType[dlSF].containerIndex][opType[dlSF].dlSubFrameNum]].schUsed; \
                \
                /* Check if this DL-sf was SPS  occasion or normal Dl data
                 * scheduling */ \
                \
                if (DRA_SCHEDULING == schUsed) \
                { \
                    BIT_DECODING_FOR_HARQ_MULTIPLEXING(opType[dlSF].ackNackType,\
                                                        operation); \
                    operation >>= 4;  \
                } \
                else \
                { \
                    /* Storing DL-Sf of SPS occasion */\
                    spsOccasionIndex = dlSF; \
                } \
            } \
            \
        } \
        \
        /* If any DL-Sf was SPS occasion then store HARQ status at 
         * spsOccasionIndex and decode HARQ bits from operation  
         * at index after normal data scheduling */ \
        if (spsOccasionIndex != INVALID_DL_SF_INDEX_VALUE) \
        { \
            BIT_DECODING_FOR_HARQ_MULTIPLEXING( \
                    opType[spsOccasionIndex].ackNackType,operation); \
        }\
    } \
    else \
    { \
        /* This is case of HARQ on PUCCH / PUSCH without DCI i.e. non adaptive
         * retransmission or UL -SPS occasion  
         * HARQ status indexing is according to Table 10.1.3.1-1 ,
         * specification 36.213. 
         */ \
        \
        for (dlSF = 0; dlSF < countM; dlSF++) \
        { \
            BIT_DECODING_FOR_HARQ_MULTIPLEXING(harqStatus,operation); \
            opType[harqToDLSfMappingForMultiplexing[opType[dlSF].dlSubFrameNum]].\
                                                          ackNackType = harqStatus;\
            operation >>= 4; \
        } \
    } \
    \
}
/* SPR 19679: TDD HARQ Multiplexing Changes End */

/* SPR 19679: TDD HARQ Multiplexing Changes Start */\
#define CHECK_MOD_TYPE_AND_HARQ_STORE_OPERATION_WITH_M_TWO(modeType,operation, \
                                     opType,containerIndex,\
                                     sf,dlUEContext_p )\
/* SPR 19679: TDD HARQ Multiplexing Changes End */\
{ \
    if (HARQ_CHANNEL_SELECTION_MODE == modeType) \
    {    \
        /* For Channel selection and M = 2 ,acc to specs 36.213 Table 10.1.3.2-4 \
         * format stored in  every nibble of operation is \
         *    nibble 3   nibble 2   nibble 1   nibble 0   \
         *    SF2 Scell  SF1 Scell  SF2 Pcell  SF1 Pcell \
         * so harq response for first SF is in nibble2 nibble0. \
         * For second SF is nibble3 nibble1. \
         */  \
        \
            /* SPR 19679: TDD HARQ Multiplexing Changes Start */\
            opType[0].ackNackType = (operation & 0x07) |(((operation & 0x0700)>> 8 ) << 4); \
            opType[1].ackNackType = ((operation & 0x70) >> 4)  |(((operation & 0x7000)>> 12 ) << 4); \
            /* SPR 19679: TDD HARQ Multiplexing Changes End */\
    } \
    /* SPR 14056 Start */ \
    /* SPR 19679: TDD HARQ Multiplexing Changes Start */\
    else if(HARQ_MULTIPLEXING_MODE == modeType)\
    {\
        /* For multiplexing mode and M = 3,acc to spec 36.213 section 10.1.3.1
         * and 7.3
         *
         * 1) If UL transmission was done on PUCCH or PUSCH w/o DCI0 i.e.
         * non-adpaptive retransmission or Ul-SPS occasion :
         *   Whether scheduling was done on both DL-Sfs or if in any Dl-sf
         *   data was not scheduled or any dl-sf was SPS occasion
         *       nibble 1   nibble 0 
         *       SF2        SF1  
         *       
         * 2) If UL transmission was done on PUSCH with DCI0 i.e. new
         * transmission or adaptive retransmisson.
         *     A) If scheduling was done on both DL SFs then HARQ response of
         *         DL-sf one is received in nibble 0, HARQ response of DL-sf
         *         two is received in nibble 1.
         *         nibble 1   nibble 0
         *         SF2        SF1
         *
         *     B) If first DL-sf was not scheduled then HARQ response of DL-sf 
         *        two is received in nibble 0 
         *        nibble 1   nibble 0
         *                   SF2
         *
         *     C) If first DL -sf was SPS occasion then  HARQ response of DL-sf
         *        two is received in nibble 0 and SPS occasion is received
         *        in nibble 1
         *         nibble 1   nibble 0
         *         SF1        SF2
         *
         */\
        CHECK_PUCSH_PUCCH_TX_AND_STORE_HARQ_STATUS_WITH_MULTIPLEXING_AND_M_TWO(\
               containerIndex,dlUEContext_p,sf,opType,operation);\
    }\
    /* SPR 19679: TDD HARQ Multiplexing Changes End */\
    else  \
    { \
       /* SPR 19679: TDD HARQ Multiplexing Changes Start */\
        opType[0].ackNackType = operation;\
        opType[1].ackNackType = operation;\
       /* SPR 19679: TDD HARQ Multiplexing Changes End */\
    }\
    /* SPR 14056 End */ \
    \
/* SPR 19679: TDD HARQ Multiplexing  Changes Start */\
    /* Resetting particular UL bit,if set while transmitting PUSCH with DCI0 */ \
    RESET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(dlUEContext_p->ueIndex, sf);\
/* SPR 19679: TDD HARQ Multiplexing  Changes End */\
}  


/* SPR 19679: TDD HARQ Multiplexing  Changes Start */
#define CHECK_MOD_TYPE_AND_HARQ_STORE_OPERATION_WITH_M_THREE_OR_FOUR(modeType, \
            operation,opType,dlUEContext_p,containerIndex,sf,countM,tddConfig)\
/* SPR 19679: TDD HARQ Multiplexing  Changes End */\
\
{ \
    UInt16 operationTypeForPCell = 0; \
    UInt16 operationTypeForSCell = 0; \
    UInt8 isHarqOnPucch = LTE_FALSE; \
    UInt8 isHarqOnPusch = LTE_FALSE; \
    UInt8 dlSF = 0;\
    DLUESCellContext *dlUeScellContext_p = \
                        dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX];\
    if (HARQ_CHANNEL_SELECTION_MODE == modeType) \
    { \
        /* For Channel selection and M = 3,acc to spec 36.213 section 10.1.3.2.1 \
         * format stored in every nibble of operation is \
         *    A) If scheduling is done on each M DL subframes \
         *       nibble 5   nibble 4   nibble 3   nibble 2   nibble 1   nibble 0 \
         *       SF3 Scell  SF2 Scell  SF1 Scell  SF3 Pcell  SF2 Pcell  SF1 Pcell \
         *       \
         *       so harq response for first SF is in nibble3 nibble0 \
         *       For second SF is in nibble4 nibble1 and so-on . \
         *       \
         *    B) If out of M subframe any DL data is not scheduled on Pcell/Scell  \
         *       eg scheduling is not done on Scell of second DL,using concept  \
         *       of DAI i.e. DL Subframe DAI = j + 1  then value is present in \
         *       nibble j :- \
         *       nibble 5   nibble 4   nibble 3   nibble 2   nibble 1   nibble 0  \
         *                  SF3 Scell  SF1 Scell  SF3 Pcell  SF2 Pcell  SF1 Pcell \
         *       \
         *  Above concept is used for M = 4.  \
         */\
         \
        /* Extracting PCell and Scell harq responses from operation */\
        if (HARQ_MULTIPLEX_THREE == countM) \
        { \
             operationTypeForPCell = (operation & 0xFFF); \
             operation = operation >> (countM * 4); \
             operationTypeForSCell = (operation & 0xFFF); \
        } \
        else if (HARQ_MULTIPLEX_FOUR == countM) \
        {  \
            operationTypeForPCell = (operation & 0xFFFF); \
            operation = operation >> (countM * 4); \
            operationTypeForSCell = (operation & 0xFFFF); \
        } \
        \
        CHECK_IS_HARQ_BLER_RECEIVED_ON_PUCCH(dlUEContext_p->ueIndex,\
                       sf,isHarqOnPucch); \
        \
        isHarqOnPusch = !(isHarqOnPucch);\
        for (dlSF = 0; dlSF < countM; dlSF++) \
        { \
           /* Check if scheduling done on PCell for a particular DL Subframe */  \
           if ((dlUEContext_p->pdschTxMapContainer[containerIndex][sf]) \
                          & (bitmask[opType[dlSF].dlSubFrameNum])) \
           { \
               opType[dlSF].ackNackType = operationTypeForPCell & 0x07; \
               operationTypeForPCell >>= 4; \
           } \
           else if (isHarqOnPusch) \
           { \
               operationTypeForPCell >>= 4; \
           }   \
           \
           /* Check if scheduling done on SCell for a particular DL Subframe */  \
           if (dlUeScellContext_p && \
               (dlUeScellContext_p->pdschTxMapContainerForSCell[containerIndex][sf])\
                         & (bitmask[opType[dlSF].dlSubFrameNum])) \
           { \
               opType[dlSF].ackNackType |= ((operationTypeForSCell & 0x07) << 4); \
               operationTypeForSCell  >>= 4; \
           }  \
           else if (isHarqOnPusch) \
           { \
               operationTypeForSCell >>= 4; \
           } \
        } \
    }\
    /* SPR 19679: TDD HARQ Multiplexing Changes Start */\
    else if (HARQ_MULTIPLEXING_MODE == modeType) \
    {\
        /* For multiplexing mode and M = 3,acc to spec 36.213 section 10.1.3.1
         * and 7.3
         *
         * 1) If UL transmission was done on PUCCH or PUSCH w/o DCI0 i.e.
         * non-adpaptive retransmission or Ul-SPS occasion :
         *   Whether scheduling was done on all DL-Sfs or if in any Dl-sf 
         *   data was not scheduled or any dl-sf was SPS occasion 
         *       nibble 2   nibble 1   nibble 0 
         *       SF3        SF2        SF1  
         *       
         * 2) If UL transmission was done on PUSCH with DCI0 i.e. new
         * transmission or adaptive retransmisson.
         *     A) If scheduling was done on all DL SFs then HARQ response of
         *         DL-sf one is received in nibble 0, HARQ response of DL-sf
         *         two is received in nibble 1 and HARQ response of DL-sf
         *         three is received in nibble  2.
         *         nibble 2   nibble 1   nibble 0
         *         SF3        SF2        SF1
         *
         *     B) If first DL-sf was not scheduled then HARQ response of DL-sf 
         *        two is received in nibble 0 and HARQ response of DL-sf
         *        three is received in nibble  1.
         *        nibble 2   nibble 1   nibble 0
         *                   SF3        SF2
         *
         *     C) If first DL -sf was SPS occasion then  HARQ response of DL-sf
         *        two is received in nibble 0 and HARQ response of DL-sf
         *        three is received in nibble 1 ans HARQ response of
         *        SPS occasion is received in nibble 2.
         *         nibble 2   nibble 1   nibble 0
         *         SF1        SF3        SF2
         *
         *  Above concept is used for M = 4.  \
         */\
        \
        CHECK_PUCSH_PUCCH_TX_AND_STORE_HARQ_STATUS_WITH_MULTIPLEXING_AND_M_THREE_OR_FOUR(\
                dlUEContext_p,sf,isHarqOnPucch,\
                isHarqOnPusch,dlSF,opType,operation,countM,\
                containerIndex,tddConfig);\
    } \
    /* SPR 19679: TDD HARQ Multiplexing Changes End */ \
    else  \
    { \
       for (dlSF = 0; dlSF < countM; dlSF++) \
       { \
            opType[dlSF].ackNackType = operation; \
       } \
    }\
/* SPR 19679: TDD HARQ Multiplexing  Changes Start */\
    /* Resetting particular UL bit,if set while transmitting PUSCH with DCI0 */ \
    RESET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(dlUEContext_p->ueIndex, sf); \
/* SPR 19679: TDD HARQ Multiplexing  Changes End */\
}


typedef struct DLAckNackOperationT
{
  UInt16 ackNackType;
  UInt8 dlSubFrameNum;
  /* SPR 19679: TDD HARQ Multiplexing Changes Start */
  UInt8 containerIndex;
  /* SPR 19679: TDD HARQ Multiplexing Changes End */
} DLAckNackOperation;
/* - CA_TDD_HARQ_CHANGES */ 
#endif 
/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/


/* + CQI_4.1 */
STATIC void getSubbandNum(UInt8 *subbandInfo_p,
        UInt32 combinatorialIndex,
        UInt8 M,
        InternalCellIndex internalCellIndex);

STATIC void getCombinatorialIndexAndPMIForRIMoreThan1(UInt8  *messageBuffer_p,
        UInt32 *combinatorialIndex,
        UInt8  *wbPmiValue,
        UInt8  *sbPmiValue,
        InternalCellIndex internalCellIndex);

STATIC UInt32 getCombinatorialIndexFromL(UInt8 *messageBuffer_p,
        InternalCellIndex internalCellIndex);

STATIC void getPMIForAperiodicMode22RI1(UInt8 *messageBuffer_p,
        UInt8 *wbPmiValue,
        UInt8 *sbPmiValue,
        InternalCellIndex internalCellIndex);

/* Cyclomatic_complexity_changes_start */
void getPMIForAperiodicMode22RI1NumTxAntenna1Or2(UInt8  *messageBuffer_p,
        UInt8 *wbPmiValue,
        UInt8 *sbPmiValue,
        CellConfigParams *cellParams_p);

void getPMIForAperiodicMode22RI1NumTxAntenna4(UInt8  *messageBuffer_p,
        UInt8 *wbPmiValue,
        UInt8 *sbPmiValue,
        CellConfigParams *cellParams_p);

UInt64 getPMIValueCW0ForMode1_2Bytes0(UInt8 *messageBuffer_p,
        UInt8 numberOfBits);

UInt64 getPMIValueCW0ForMode1_2Bytes1(UInt8 *messageBuffer_p,
        UInt8 numberOfBits);

UInt64 getPMIValueCW0ForMode1_2Bytes2(UInt8 *messageBuffer_p,
        UInt8 numberOfBits);

UInt64 getPMIValueCW0ForMode1_2Bytes3(UInt8 *messageBuffer_p,
        UInt8 numberOfBits);

UInt64 getPMIValueCW0ForMode1_2Bytes4(UInt8 *messageBuffer_p,
        UInt8 numberOfBits);

UInt64 getPMIValueCW0ForMode1_2Bytes5(UInt8 *messageBuffer_p,
        UInt8 numberOfBits);

UInt64 getPMIValueCW0ForMode1_2Bytes6(UInt8 *messageBuffer_p,
        UInt8 numberOfBits);
/* Cyclomatic_complexity_changes_end */

STATIC UInt32 getSubbandDifferentialCqiCW0(UInt8 *messageBuffer_p,
        UInt8 numberOfBytes, UInt8 numberOfBits);

STATIC void getSubbandDiffCqiWidebandCqiPmiCW1Mode3_1(UInt8 *messageBuffer_p, 
        UInt8 numberOfBytes, 
        UInt8 numberOfBits, 
        UInt8 *widebandCQIValue1,
        UInt8 *pmiValue, 
        UInt32 *spatialDifferentialCqiForCW1,
        InternalCellIndex internalCellIndex);
/* + CQI_5.0 */
/* + SPR_5415 */
/* +- SPR 18268 */
void pushNodeForCQIProcessing(UInt16 ueIndex,
        /* +- SPR 18268 */
        UInt8 riValue,
        ueExpectedReportType cqiExpectedType,
        ULUEContext *ulUEContext_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex);
/*CA Changes end  */
/* - SPR_5415 */
STATIC void getWBPMIForRIMoreThan1(UInt8  *messageBuffer_p,
        UInt8  *wbPmiValue,
        InternalCellIndex internalCellIndex);

STATIC void getWBPMIForAperiodicMode22RI1(UInt8 *messageBuffer_p,
        UInt8 *wbPmiValue,
        InternalCellIndex internalCellIndex);

STATIC void getWidebandCqiPmiCW1Mode3_1(UInt8 *messageBuffer_p, 
        UInt8 numberOfBytes, 
        UInt8 numberOfBits, 
        UInt8 *widebandCQIValue1,
        UInt8 *pmiValue,
        InternalCellIndex internalCellIndex); 
/* - CQI_5.0 */

/* Cyclomatic_complexity_changes_start */
void getWBPMIForAperiodicMode22RI1NumTxAntenna1Or2(UInt8 *messageBuffer_p,
        UInt8 *wbPmiValue,
        CellConfigParams *cellParams_p);

void getWBPMIForAperiodicMode22RI1NumTxAntenna4(UInt8 *messageBuffer_p,
        UInt8 *wbPmiValue,
        CellConfigParams *cellParams_p);
/* Cyclomatic_complexity_changes_end */

/* + SPR_4991 */
STATIC MacRetType  getSubbandCqiValue(UInt32 SpatialDifferentialCqi, 
        UEReportedCQINodeInfo *ueReportedCQIInfo_p);

STATIC MacRetType getSubbandCqiValueMode31(UInt32 SpatialDifferentialCqiCW0,
        UInt32 SpatialDifferentialCqiCW1, 
        UEReportedCQINodeInfo *ueReportedCQINodeInfo_p,
        UInt8 ri
#ifdef PERF_STATS
        ,UInt8 transmissionMode
#endif
        );
/* - SPR_4991 */

STATIC UInt32 getSwappedCombinatorialIndex(UInt8 L,UInt32 tempCombinatorialIndex);

STATIC UInt8 getPMIValueCW0ForMode31(UInt8 *messageBuffer_p,UInt8 numberOfBits,
        InternalCellIndex internalCellIndex);

STATIC void  getPMIValueCW1ForMode1_2(UInt8 *messageBuffer_p,
        UInt8 startIndex,
        UInt8 endIndex,
        UInt8 *pmiInfo12);

STATIC void  getPMIValueCW0ForMode1_2(UInt8 *messageBuffer_p,UInt8 numberOfBytes, 
        UInt8 numberOfBits,UInt8 N,UInt8 *pmiInfo12);
/* 4x2 MIMO S */
/* +- SPR 17777 */
STATIC void  getPMIValueCWForAnt4Mode1_2(UInt8 *messageBuffer_p,UInt8 numberOfBytes, 
        UInt8 numberOfBits,UInt8 ri,UInt8 *pmiInfo12);
/* +- SPR 17777 */
/* 4x2 MIMO E */

/* - CQI_4.1.0 */
/* validate the System Frame Number and Sub Frame number of 
   the recieved message buffer and also fill the tick of the 
   message recieved. */
 STATIC MacRetType validatePHYDelayAndTick( UInt32 systemFrameNum, 
        UInt32 subFrameNum, 
        /* SPR 15909 fix start */
        tickType_t *tti_p,
        /* SPR 15909 fix end */
        UInt16 msgId,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
        );

#ifdef DL_UL_SPLIT_TDD
 STATIC MacRetType validatePHYDelayAndTickUl( UInt32 systemFrameNum, 
                                                  UInt32 subFrameNum, 
                                                  /* SPR 15909 fix start */
                                                  tickType_t *tti_p
                                                  /* SPR 15909 fix end */
                                                );
#endif

#ifdef UTFWK_SIMULATION						   
/* This function creates a new SrNode and inserts this node into the global
 * globalSrQueue.*/
 void  putEntryInSRQueue( UInt32 ueIndex, 
        /* SPR 15909 fix start */
        tickType_t recvTTI
        /* SPR 15909 fix end */
        );
#else                            
/* This function creates a new SrNode and inserts this node into the global
 * globalSrQueue.*/
STATIC  void  putEntryInSRQueue( UInt32 ueIndex, 
        /* SPR 15909 fix start */                             
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex);
/*CA Changes end  */
#endif
/* This function decode and store into the container to uplink Ack Nack */
#ifndef UTFWK_SIMULATION
STATIC  void crcAckNackHandling( UInt32 crcAckNack,
        UInt32 ueIndex,
                                       /* SPR 15909 fix start */
                                       tickType_t recvTTI,
                                       /* SPR 15909 fix end */
        UInt32 ulContainerIndex, 
        UInt32 rntiType,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */ 
        /* SPR 11509 Fix Start */
        ,UInt8 isForcedAckDueToMaxRetx
        /* SPR 11509 Fix End */
        );
#endif

/* msg3_retx_tdd_support_start */
/*SPR_3305_FIX_START*/
STATIC  void crcAckNackHandlingForTcrnti( UInt32 crcAckNack,
        UInt32 ueIndex,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt32 ulContainerIndex,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */ 
        );
/*SPR_3305_FIX_END*/
/* msg3_retx_tdd_support_end */


/*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
 void  updateHdNextReportTimeout(UInt32 sfn, UInt16 ue);
#endif
/*HD FDD Changes End*/
/* + TM7_8 Changes Start */
 UInt16 calculateTimeAvgAoA(UInt16 lastSavedAoAValue 
        ,UInt16 newAoAValue);
/* + TM7_8 Changes End*/

/* 4x4 DL MIMO CHG START */
static UInt16 isRIInCodebookSubsetRestriction( UInt8 RI,
        UInt8 ueCategory,
        UInt8 transMode,
        UInt8 *cbsrValue,
        InternalCellIndex internalCellIndex);
/* 4x4 DL MIMO CHG END */
/* CA_phase2_csi_code Start */
/* SPR 11215 Changes Start */
#ifdef FAPI_4_0_COMPLIANCE
/* SPR 11215 Changes End */
STATIC  void handleCQIReportWithUlCqiLessThanCQISINRThreshold(UInt8 numberOfCCReported,
        ULUEContext *ulUEContext_p,
        DLUEContext *dlUEContext_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt8 *bitMaskForAperiodicReport_p,
        /* +- SPR 17777 */
        UInt16 subFrameNum
/** SPR 14032 Fix : Start **/
#ifdef TDD_CONFIG
        ,UInt16 sysFrameNum
#endif
/** SPR 14032 Fix : End **/
        );
        /* +- SPR 17777 */
STATIC  void processCQIReportForOneCellForCAUE(ULUEContext *ulUEContext_p,
        DLUEContext *dlUEContext_p,
        FAPI_cqiPduIndicationR9_st *cqiPdu_p,
        UInt32  datalen,
        UInt16 subFrameNum,
#ifdef TDD_CONFIG
        UInt16 sysFrameNum,
#endif
        UInt8 *data_p,
        UInt16  ueIndex,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex);
#endif
/* SPR 11215 Changes Start */

#ifdef FAPI_4_0_COMPLIANCE
/* SPR 11215 Changes End */
/* coverity 64950 28June2014 */
/* coverity 64950 28June2014 */
STATIC  UInt8 calculateSizeOfReport(UInt8 ri,
        ueExpectedReportType  cqiReportType,
        InternalCellIndex internalCellIndex);
/* coverity 64950 28June2014 */
/* coverity 64950 28June2014 */

/* coverity 64951 28June2014 */
/* coverity 64951 28June2014 */
#ifdef FAPI_4_0_COMPLIANCE /*CID 64951*/ 
STATIC  UInt8 getNumCellsForCSIReport(UInt8 cellIdxOfExpectedReportType,
        UInt8 scellCount);
#endif
STATIC  void handleCQIReportWithDataOffsetNull(ULUEContext *ulUEContext_p,
        DLUEContext *dlUEContext_p,
        /* SPR 15909 fix start */
     	tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt16  ueIndex,
        FAPI_cqiPduIndicationR9_st *cqiPdu_p,
        UInt16 subFrameNum,
#ifdef TDD_CONFIG
        UInt16 sysFrameNum,
#endif
        UInt8 *bitMaskForAperiodicReport_p,
        InternalCellIndex internalCellIndex,
        UInt8 numOfCCReported);
#endif

/* coverity 64952 28June2014 */
#ifdef FAPI_4_0_COMPLIANCE
/* coverity 64952 28June2014 */
STATIC  void decodeAperiodicCqiReportsR10(UInt8 *data_p,
        ueExpectedReportType cqiReportType,
        UInt16 ueIndex,
        ULUEContext *ulUEContext_p,
       /* SPR 15909 fix start */
	tickType_t recvTTI,
       /* SPR 15909 fix end */
        UInt8 transmissionMode,
        DLUEContext *dlUEContext_p,
        /* +- SPR 17777 */
        /*CA Changes start */
        InternalCellIndex internalCellIndex,
        UInt8 *numOfBitsDecoded_p);
/* coverity 64952 28June2014 */
#endif
	
/* coverity 64952 28June2014 */

/* CA_phase2_csi_code End */
/*
 * This is array which contains the mask bit that is
 * used for setting bitmask in the DL UE Context with
 * respect to the TX subframe number.
 * */

UInt32 bitmask[MAX_SUB_FRAME] = {
    BITMASK_0,
    BITMASK_1,
    BITMASK_2,
    BITMASK_3,
    BITMASK_4,
    BITMASK_5,
    BITMASK_6,
    BITMASK_7,
    BITMASK_8,
    BITMASK_9 
};
/*
 * This is array which contains the mask bit that is
 * used for resetting bitmask in the DL UE Context with
 * respect to the TX subframe number.
 * */
UInt32 resetBitmask[MAX_SUB_FRAME] = {
    RESET_BITMASK_0,
    RESET_BITMASK_1,
    RESET_BITMASK_2,
    RESET_BITMASK_3,
    RESET_BITMASK_4,
    RESET_BITMASK_5,
    RESET_BITMASK_6,
    RESET_BITMASK_7,
    RESET_BITMASK_8,
    RESET_BITMASK_9
};
/*CID 109569 FixStart*/
STATIC  void  updateTpcForPhyErrorInd(
        /* +- SPR 17777 */
        ErrIndPowerControlType pcType,
        UInt8 mapIdx,InternalCellIndex internalCellIndex );
/*CID 109569 FixEnd*/
/* +- SPR 17777 */
STATIC  void  handleFapiMsgSubFrameErrForDlConfig(
        tickType_t currentTick,
#ifdef TDD_CONFIG
        FAPI_errMsgBody2_st   *errMsgBody2_p,
        FAPI_l1ErrorCodes_en errorCode,
#endif
        InternalCellIndex internalCellIndex);
STATIC  void  handleFapiMsgSubFrameErrForUlConfig(
        FAPI_errMsgBody2_st   *errMsgBody2_p,
       /* SPR 15909 fix start */
        tickType_t currentTick,
       /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex);
STATIC  void  handleFapiMsgInvalidSfnForHiDci0(
        FAPI_errMsgBody1_st *errMsgBody1_p,
       /* SPR 15909 fix start */
        tickType_t currentTick,
       /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex);
#ifdef TDD_CONFIG
/* SPR 11230 + 12261 fix start */
/* SPR 15909 fix start */
void handleDlConfigErrForReTx( tickType_t currentTick, 
        /* SPR 15909 fix end */
        FAPI_errMsgBody2_st   *errMsgBody2_p,
        FAPI_l1ErrorCodes_en errorCode, InternalCellIndex internalCellIndex );
/* SPR 11230 + 12261 fix end */
/* SPR 23102 Fix + */
 MacRetType calculateHarqExpTickMissAndArgs(
/* SPR 23102 Fix - */        
        ExecutionLegFunctionArg *execLegFuncArg_p,
        ElArgumentTypes harqExpiryArg,
       /* SPR 15909 fix start */
        tickType_t currTick,
       /* SPR 15909 fix end */
        UInt8 airSf, UInt16 airSfn, 
        UInt16 totalTickDiffNum,
        /* SPR 11110 fix start */
        UInt8 scenarioType );
        /* SPR 11110 fix end */
void tddPutEntryInAckNackQueueForConfig0( UInt32 ueIndex,
        UInt32 operation,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt16 sfn,   
        UInt16 sf,
        InternalCellIndex internalCellIndex,
        /* + CA_TDD_HARQ_CHANGES */ 
        UInt8 modeType
       /* - CA_TDD_HARQ_CHANGES */
        );

void tddPutEntryInAckNackQueueForConfig1( UInt32 ueIndex,
        UInt32 operation,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt16 sfn,   
        UInt16 sf,
        InternalCellIndex internalCellIndex,
        /* + CA_TDD_HARQ_CHANGES */ 
        UInt8 modeType
        /* - CA_TDD_HARQ_CHANGES */
        );

void tddPutEntryInAckNackQueueForConfig2( UInt32 ueIndex,
        UInt32 operation,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt16 sfn,  
        UInt16 sf,
        InternalCellIndex internalCellIndex,
        /* + CA_TDD_HARQ_CHANGES */
        UInt8 modeType
        /* - CA_TDD_HARQ_CHANGES */
        );

void tddPutEntryInAckNackQueueForConfig3( UInt32 ueIndex,
        UInt32 operation,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt16 sfn,
        UInt16 sf,
        InternalCellIndex internalCellIndex,
        /* + CA_TDD_HARQ_CHANGES */
        UInt8 modeType
        /* - CA_TDD_HARQ_CHANGES */
        );

void tddPutEntryInAckNackQueueForConfig4( UInt32 ueIndex,
        UInt32 operation,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt16 sfn,
        UInt16 sf,
        InternalCellIndex internalCellIndex,
        /* + CA_TDD_HARQ_CHANGES */  
        UInt8 modeType
        /* - CA_TDD_HARQ_CHANGES */
        );

void tddPutEntryInAckNackQueueForConfig5( UInt32 ueIndex,
        UInt32 operation,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt16 sfn,
        UInt16 sf,
        InternalCellIndex internalCellIndex,
        /* + CA_TDD_HARQ_CHANGES */
        UInt8 modeType
        /* - CA_TDD_HARQ_CHANGES */
        );

void tddPutEntryInAckNackQueueForConfig6( UInt32 ueIndex,
        UInt32 operation,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt16 sfn, 
        UInt16 sf,
        InternalCellIndex internalCellIndex,
        /* + CA_TDD_HARQ_CHANGES */
        UInt8 modeType
        /* - CA_TDD_HARQ_CHANGES */
        );
/*
 * Function pointer table for the function to add an entry in the 
 * Ack/Nack queue for the received ack nack
 * for different UL-DL configuration
 */

tddPutEntryInAckNackQueueForDiffConfig 
tddPutEntryInAckNackQueueForDiffConfig_g[TOTAL_NUM_UL_DL_CONFIG] =
{
    tddPutEntryInAckNackQueueForConfig0,
    tddPutEntryInAckNackQueueForConfig1,
    tddPutEntryInAckNackQueueForConfig2,
    tddPutEntryInAckNackQueueForConfig3,
    tddPutEntryInAckNackQueueForConfig4,
    tddPutEntryInAckNackQueueForConfig5,
    tddPutEntryInAckNackQueueForConfig6
};

/*
 * pointer to hold the tddPutEntryInAckNackQueueForDiffConfig_g function 
 * for current UL-DL configuration
 */
/** CA-TDD Changes Start:06 **/
tddPutEntryInAckNackQueueForDiffConfig tddPutEntryInAckNackQueueForCurConfig_p[MAX_NUM_CELL];
/** CA-TDD Changes End:06 **/

/*TDD special bundling changes start*/
/*
 * Table 36213:7.3-1: Mapping between multiple ACK/NACK responses and  
 * Number of ACK among multiple (U-dai + N-sps ) ACK/NACK responses   
 *      b(0),b(1)
 *
 * 0    0   , 0  (UE detect at least one DL assignment is missed) 
 * 1    1   , 1
 * 2    1   , 0
 * 3    0   , 1
 * 4    1   , 1
 * 5    1   , 0
 * 6    0   , 1
 * 7    1   , 1
 * 8    1   , 0
 * 9    0   , 1
 * */
#define MAX_NUMBER_TDD_ACK_NACK     10
UInt8 tddHarqSpecialbundlingMap_g[MAX_NUMBER_TDD_ACK_NACK] = { 0, 1, 2, 3, 1, 2, 3, 1, 2, 3};
/*TDD special bundling changes end*/

 void waitOnSelectPhyMsg(SInt32 phyRxFD);
 void clearFDForPhyMsg(InternalCellIndex internalCellIndex);
/* CA Changes start */ 
#ifndef FLEXRAN
extern SInt32 phyRxSockFD_g[MAX_NUM_CELL];
#endif
#ifdef FLEXRAN
#ifdef FLEXRAN_FAPI_LOGGING
extern SInt32 flexranFapiLoggingEnable_g;
#endif
UInt32 numOfPreambles_g[MAX_NUM_CELL] = {0};
UInt32 numOfCrcNack_g[MAX_NUM_CELL] = {0};
UInt32 numOfCrc_g[MAX_NUM_CELL] = {0};
UInt32 numOfHarqNack_g[MAX_NUM_CELL] = {0};
UInt32 numOfHarq_g[MAX_NUM_CELL] = {0};
extern void do_tap_fun(void* data, uint32_t len, InternalCellIndex cellId);
extern uint8_t macPhyBuffer[10000];
extern SInt32 flexranFapiLoggingEnable_g;
#endif
/* CA Changes end */ 

#endif
/* + E_CID_5.3 */
STATIC  void checkValidReportsAndPutEntryInEcidMgr(UInt16 ueIndex,
        UInt8 validReports,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex);
/*CA Changes end  */
/* SPR 12364 Fix Starts */
STATIC  void harqParamBackupForUlSchInd(ULHarqInfo *ulHarqInfo);
/*SPR 12364 Fix Ends */
/* FAPI1.1 Compilation Fix Start */
/* Flag FAPI_4_0_COMPLIANCE removed */
/* FAPI1.1 Compilation Fix End */

STATIC  void decodeCqiReports(UInt8 *data_p,
        ueExpectedReportType cqiReportType,
        UInt16 ueIndex,
        ULUEContext *ulUEContext_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt8 transmissionMode,
        DLUEContext *dlUEcontext_p,
        UInt16 subFrameNum,
        InternalCellIndex internalCellIndex);
STATIC  void handleRiReportWithDataOffsetNull(ueExpectedReportType cqiReportType,
        ULUEContext *ulUEContext_p,
        DLUEContext *dlUEContext_p,
        UInt16 ueIndex,
        UInt8  *ri,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex,
        /* CA_phase2_csi_code start */
        UInt8 servCellIndex
        /* CA_phase2_csi_code end */
        );
#ifdef FAPI_4_0_COMPLIANCE
void handleUlschIndVendorSpecificInfoR9(FAPI_VendorSpecificR9_st *vendorSpecific_p, 
        UInt8 *validReports_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex);
void handleVendorSpecificInfoR9( FAPI_VendorSpecificR9_st *vendorSpecific_p,
        UInt8 *validReports_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex);
void fillTimingAdvanceR9(FAPI_ulDataPduIndicationR9_st *ulschPdu_p,
        RNTIInfo *rntiInfo_p,
        UInt8 *validReports_p ,
        InternalCellIndex internalCellIndex);
STATIC  UInt8 handleRIAndCheckForAperiodicTrigger(UInt16 ueIndex,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        FAPI_cqiPduIndication_st * cqiPdu_p,
        ueExpectedReportType  cqiReportType,
        /* +- SPR 17777 */
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex);
#endif
/* SPR 9829 changes start */
STATIC  void macProcessVendorSpecificFieldsUlschInd(
                                      FAPI_UEMeasurementIndication_st *ueMeasInfoForUEs_p,
                                      /* SPR 15909 fix start */
                                      tickType_t recvTTI,
                                      /* SPR 15909 fix end */
                                      InternalCellIndex internalCellIndex);
STATIC  void macProcessVendorSpecificFieldsRachInd(
                                      FAPI_UEMeasurementInRACH_st *ueMeasInfoInRACHForUEs_p,
                                      /* SPR 15909 fix start */
                                      tickType_t recvTTI,
                                      /* SPR 15909 fix end */
                                      InternalCellIndex internalCellIndex);
/*CID 69608 Fix start*/
STATIC  void macProcessVendorSpecificFieldsSRSCQIInd(
                                      FAPI_UEMeasurementIndication_st *ueMeasInfoForUEs_p,
                                      /* SPR 15909 fix start */
                                      tickType_t recvTTI,
                                      /* SPR 15909 fix end */
                                      InternalCellIndex internalCellIndex);
/*CID 69608 Fix end*/
/* SPR 9829 changes end */
/*CA Changes start  */
TempTimingAdvandeR9  tempTimingAdvanceR9_g[MAX_NUM_CELL][MAC_MAX_NUM_OF_RA_PREAMBLES];
/*CA Changes end  */
/* - E_CID_5.3 */
#ifdef FDD_CONFIG
#ifndef DL_UL_SPLIT
/* + SPR 17733 */
STATIC void putEntryInAckNackQueueForTcrnti( UInt32 ueIndex, UInt32 operation,
        tickType_t recvTTI, InternalCellIndex internalCellIndex );
/* - SPR 17733 */
#endif
#endif



/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
extern void processControlInfoFromPhysical (UInt8  * msgBuf, Int bytesRead);

extern void handleTimingAdvance(DLUEContext *dlUEContext_p, UInt32 ueIndex,UInt16 timingAdvance,
        InternalCellIndex internalCellIndex, tickType_t currentTTI);

extern UInt16 pucchUciUlschMap_g[MAX_UE_SUPPORTED];

#ifdef DL_UL_SPLIT
extern void putEntryInAckNackQueueUsingICC( UInt32 ueIndex, 
        /* CA_phase2_harq_code Start */
        UInt16 operation, 
        /* CA_phase2_harq_code End */
        /* SPR 15909 fix start */
        tickType_t recvTTI
        /* SPR 15909 fix end */
        /*SPR 6169 Fix Start */
        ,UInt32 schUsed
        /*SPR 6169 Fix End */
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */
        );
/* warning removed*/
extern MacRetType putEntryInULContainerNackQueueUsingICC( SInt32 ulContainerNackQIdUL , ContainerNackQueueNode *nackNode_p);
#endif
/* Downlink or Harq Ack/Nack global Queue*/
#ifdef FDD_CONFIG
#ifdef CIRC_QUEUE_IMPL
/*CA Changes start  */
ACK_NACK_CIRC_TYPE* ackNackCircQueue_gp[MAX_NUM_CELL] = {PNULL};
extern sem_t waitForDLHarqAckNack_g[MAX_NUM_CELL];

extern sem_t waitForULNackQueue_g[MAX_NUM_CELL];
extern sem_t waitForULNackForTempUE_g[MAX_NUM_CELL];
#endif

#ifndef DL_UL_SPLIT
LTE_SQUEUE* ackNackQueue_gp[MAX_NUM_CELL] = {PNULL};
/*CA Changes end  */
#ifdef FDD_CONFIG
/* + SPR 17733 */
LTE_SQUEUE* ackNackQueueForTcrnti_gp[MAX_NUM_CELL];
/* - SPR 17733 */
#endif
#endif
#endif

#ifdef TDD_CONFIG
/** CA-TDD Changes Start:06 **/
LTE_SQUEUE tddAckNackQueue_g[MAX_NUM_CELL];
/** CA-TDD Changes End:06 **/
#endif

/* Start of change for VARIBLE Declaration */
/* SRP 3608 changes start */
/*CA Changes start  */
extern LTE_SQUEUE* ulHighPriorityGrantQueue_gp[MAX_NUM_CELL];
/*CA Changes end  */
/* SPR 3608 changes end */
/* End  of change for VARIBLE Declaration */

/*Registration with PHY Layer*/
volatile UInt32 macPhyRegistrationSuccessful_g[MAX_NUM_CELL];
/*CA Changes start  */
UInt32 totalMacUllinkBytesSend_g[MAX_NUM_CELL];
UInt32 numOfrachInd_g[MAX_NUM_CELL];

/*CA Changes end  */
UInt16 totalNumberOfTickDiffDl_g[MAX_NUM_CELL] = {0};
UInt16 totalNumberOfTickDiffUl_g[MAX_NUM_CELL] = {0};
/* Contains the tick miss received as in DL_CONFIG Request for SFN_OUT_OF_SYNC
 * errorCode */
UInt16 totalTickMissErrIndDl_g[MAX_NUM_CELL] = {0};
/* Contains the tick miss received as in HI_DCI_0 Request for INVALID_SFN_MSG
 * errorCode */
UInt16 totalTickMissErrIndUl_g[MAX_NUM_CELL] = {0};
UInt16 countPhyErrorIndication_g[MAX_NUM_CELL] ={0};
/* Following variable counts the Number of DL Config error Indications for 
 * which the OAM notification corresponding to error indications need not 
 * be sent. */
UInt16 numErrIndDlConfig_g[MAX_NUM_CELL] ={0};
/* Following variable counts the Number of HI_DCI0 error Indications for 
 * which the OAM notification corresponding to error indications need not 
 * be sent. */
UInt16 numErrIndHiDci0_g[MAX_NUM_CELL] = {0};

#ifdef OVERSHOOT_CORRECTION
/** CA-TDD Changes Start **/
extern OverLoadLevel macOverLoadLevel_g[MAX_NUM_CELL];
/** CA-TDD Changes Stop **/
#endif
extern UInt8 phyReleaseCapability_g ;
/*+EICIC*/
UInt16 cqiSubFrameNum_g;
#define UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_CQI_RECEIVED(ueReportedCQINodeInfo_p,internalCellIndex) \
{\
    UInt32 Subframe; \
    UInt32 Sysframe; \
    getSFAndSFN(&Sysframe, &Subframe,internalCellIndex);\
    if(cqiSubFrameNum_g>Subframe) \
    	Sysframe -= 1; \
    ueReportedCQINodeInfo_p->subframe= cqiSubFrameNum_g; \
    ueReportedCQINodeInfo_p->systemFrame= Sysframe;  \
}   

/*-EICIC*/


/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/* SPR 609 changes start */ 
/* SPR 609 changes end */  

/*****************************************************************************
 * Function Name  : initPHYReceiver 
 * Inputs         : numCellsConfigured
 * Outputs        : None
 * Returns        : None
 * Description    : This function Initialising the global queue and 
 global variables.
 *****************************************************************************/
/*CA Changes start  */
void initPHYReceiver(UInt8 numCellsConfigured)
    /*CA Changes end  */
{
    UInt8 cellCount;
    UInt16 macMaxNumOfUeSupported = 0;
    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Entry. ",__func__);
#ifdef FDD_CONFIG      
    /* Creating the mempool for dowlink Ack Nack Node */
    /* SPR 18122 Changes Start*/
    createMemPool(sizeof(AckNackQueueNode), MAC_MAX_RESOURCE_BLOCKS 
            * DIVIDE_BY_TWO(MAX_HARQ_PROCESS_NB));
#elif TDD_CONFIG
    createMemPool(sizeof( tddAckNackQueueNode), MAC_MAX_RESOURCE_BLOCKS
            * DIVIDE_BY_TWO(MAX_HARQ_PROCESS_NB));
    /* SPR 18122 Changes End*/
#endif                                
    /* Start 128 UE: Memory Changes */
    /* Creating the mempool for SR Queue Node */       
    /* CA Changes start */
    for (cellCount = 0; cellCount < numCellsConfigured; cellCount++)
    { 
        macMaxNumOfUeSupported  = cellSpecificParams_g.\
                                  cellConfigAndInitParams_p[cellCount]->initParams_p->macMaxNumOfUeSupported;

        createMemPool(sizeof(ULGrantRequestQueueNode), (macMaxNumOfUeSupported));
        /* Creating the mempool for CQI node */
        createMemPool(sizeof(UEReportedCQINodeInfo), (macMaxNumOfUeSupported)); 
        /* End 128 UE: Memory Changes */
    }
    /* CA Changes end */
    /* Creating the mempool for UL Data */

    /* Creating mempool for RA message --In RandomAccess.c */

    /* Initialising Downlink Ack Nack Queue.*/
#ifdef FDD_CONFIG
#ifndef DL_UL_SPLIT

    /*CA Changes start  */
    createMemPool(sizeof(LTE_SQUEUE), numCellsConfigured);
    for (cellCount = 0; cellCount < numCellsConfigured; cellCount++)
    {
        GET_MEM_FROM_POOL(LTE_SQUEUE, ackNackQueue_gp[cellCount],sizeof(LTE_SQUEUE), PNULL);
        if (PNULL != ackNackQueue_gp[cellCount]) 
        {            
            sQueueInit(ackNackQueue_gp[cellCount]);
        }             
    }
    /*Spr 17733 Merge Fix Start */
    for (cellCount = 0; cellCount < numCellsConfigured; cellCount++)
    {
        GET_MEM_FROM_POOL(LTE_SQUEUE, ackNackQueueForTcrnti_gp[cellCount],sizeof(LTE_SQUEUE), PNULL);
        if (PNULL != ackNackQueueForTcrnti_gp[cellCount]) 
        {            
            sQueueInit(ackNackQueueForTcrnti_gp[cellCount]);
        }             
    }
    /*Spr 17733 Merge Fix End */

    /*CA Changes start  */

#endif
#elif TDD_CONFIG
    /** CA-TDD Changes Start:06 **/
    for (cellCount = 0; cellCount < numCellsConfigured; cellCount++)
    {
        sQueueInit(&tddAckNackQueue_g[cellCount]);
    }
    /** CA-TDD Changes End:06 **/
#endif

    /* Initialising the SR Queue*/
    /*CA Changes start  */
    createMemPool(sizeof(LTE_SQUEUE), numCellsConfigured);
    for (cellCount = 0; cellCount < numCellsConfigured; cellCount++)
    {
        GET_MEM_FROM_POOL(LTE_SQUEUE, ulGrantGlobalSrQueue_gp[cellCount],sizeof(LTE_SQUEUE), PNULL);
        if (PNULL != ulGrantGlobalSrQueue_gp[cellCount]) 
        {            
            sQueueInit(ulGrantGlobalSrQueue_gp[cellCount]);
        }             
    }
    /*CA Changes start  */
    /* CA Changes start */
    /* Initialising CQI manager memory.*/
    initCQIMgr(numCellsConfigured);
    /* CA Changes end */
    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Exit. ",__func__);
}
/*****************************************************************************
 * Function Name  : recvRegistrationCNFFromPhy 
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used by Execution scheduler to check
 *                  that MAC is registered with PHY.
 *****************************************************************************/
/*CA Changes start  */
void  recvRegistrationCNFFromPhy(InternalCellIndex internalCellIndex)
{
    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Entry. ",__func__);

    /* + Coverity_Stack Fix */
    UInt8 *msgBuf = PNULL;
    GET_MEM_FROM_POOL(UInt8,msgBuf,SH_MESSAGE_UL_BUF_SIZE, PNULL);
    /* - Coverity_Stack Fix */
    /*+COVERITY 5.3.0 - 32101*/
    if( PNULL == msgBuf )
    {
        ltePanic("Memory Allocation failed in %s\n",__func__);
        return;
    }
    /*-COVERITY 5.3.0 - 32101*/
    UInt32 msgLen          = 0;
    UInt32 resp            = MAC_SUCCESS;
    SInt32 bytesRead       = 0;
    UInt32 sysFrameNum     = 0;
    UInt32 subFrameNum     = 0;
    UInt8  *msg_p        = PNULL;

    /* Non Blocking Msg Queue or Socket Interface*/ 
    bytesRead = recvDataFromPhysical ((void *)msgBuf, internalCellIndex);   
    msg_p = msgBuf;

    /* Message buffer length should be greater than 10(size of API Header)*/
    if (bytesRead > MAC_API_HEADER_LEN ) 
    {
        /* Parse the buffer header */
        /* skip the length of transaction ID*/
        msg_p += 2;
        msg_p += 6;
        msgLen = LTE_PHY_INT_GET_U16BIT(msg_p);
        msg_p += 2;
        msgLen -= MAC_API_HEADER_LEN;
        /* response will be either Success or Failure if Success 
           then also return SFN & SF. */
        resp = *msg_p++;  
        if ( resp == MAC_FAILURE )
        {
            lteWarning("Mac Recv Failure Response = %d\n",resp);
            freeMemPool(msgBuf);
            return;
        }
        /*update the msgLen */
        msgLen -= 1;
        /*decode the System Frame Number*/
        /* SPR536 */
        sysFrameNum = LTE_PHY_INT_GET_U16BIT(msg_p);
        msg_p += SIZEOF_UINT16;
        /* decode the SubFrame Number */
        subFrameNum = *msg_p++;
        macRegistrationCnf(sysFrameNum, subFrameNum, internalCellIndex);     
        /*update the Global Variable For Registration Successfully */
        macPhyRegistrationSuccessful_g[internalCellIndex] = TRUE;
    }
    /* + Coverity_Stack Fix */
    freeMemPool(msgBuf);
    /* + Coverity_Stack Fix */
    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Registraton Successfully Exit. ",__func__);
}
/*CA Changes end  */

/*SS_S2*/
/*SPR 2311 Start*/
#define GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(report) ((report & 0x0F))
#define GET_WIDEBAND_VALUE_FROM_LAST_2_BITS(report1) ((report1 & 0xC0) >> 6) 
#define GET_WIDEBAND_VALUE_FROM_FIRST_2_BITS(report2) ((report2 & 0x03))
#define GET_WIDEBAND_VALUE_FROM_LAST_4_BITS(report4) ((report4 & 0xF0) >> 4)
#define GET_WIDEBAND_VALUE_FROM_MIDDLE_4_BITS(report5) ((report5 & 0x3C) >> 2)
/* + CQI_4.1 */
#define GET_VALUE_FROM_FIRST_4_BITS(report)\
    GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(report) 
#define GET_VALUE_FROM_LAST_2_BITS(report1)\
    GET_WIDEBAND_VALUE_FROM_LAST_2_BITS(report1) 
#define GET_VALUE_FROM_FIRST_2_BITS(report2)\
    GET_WIDEBAND_VALUE_FROM_FIRST_2_BITS(report2) 
#define GET_VALUE_FROM_LAST_4_BITS(report3)\
    GET_WIDEBAND_VALUE_FROM_LAST_4_BITS(report3) 
#define  GET_VALUE_FROM_MIDDLE_4_BITS(report4)\
    GET_WIDEBAND_VALUE_FROM_MIDDLE_4_BITS(report4) 
#define GET_VALUE_FROM_FIRST_BIT(report6) ((report6 & 0x01))
#define GET_VALUE_FROM_FIRST_3_BITS(report7) ((report7 & 0x07))
#define GET_VALUE_FROM_FIRST_5_BITS(report8) ((report8 & 0x1F))
#define GET_VALUE_FROM_FIRST_6_BITS(report9) ((report9 & 0x3F))
#define GET_VALUE_FROM_LAST_6_BITS(report10) ((report10 & 0xFC) >> 2)
#define GET_VALUE_FROM_FIRST_7_BITS(report11) ((report11 & 0x7F))
#define GET_VALUE_FROM_LAST_BIT(report12) ((report12 & 0x80) >> 7)
/*4X2 MIMO S*/
#define GET_VALUE_FROM_FIRST_8_BITS(report13) ((report13 & 0xFF))   
#define GET_VALUE_FROM_LAST_7_BITS(report14) ((report14 & 0xFE) >> 1)
#define GET_VALUE_FROM_LAST_5_BITS(report15) ((report15 & 0xF8) >> 3)
#define GET_VALUE_FROM_LAST_3_BITS(report16) ((report16 & 0xE0) >> 5)
/*4X2 MIMO S*/
#define GET_VALUE_FROM_FIRST_10_BITS(\
        messageBuffer_p,tempCombIndex)\
{\
    tempCombIndex = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);\
    messageBuffer_p = messageBuffer_p + 1;\
    tempCombIndex |= *messageBuffer_p << 2;\
    messageBuffer_p = messageBuffer_p + 1;\
}
#define GET_VALUE_FROM_FIRST_12_BITS(\
        messageBuffer_p,combinatorialIndex1)\
{\
    combinatorialIndex1 = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);\
    messageBuffer_p = messageBuffer_p + 1;\
    combinatorialIndex1 |= *messageBuffer_p << 4;\
    messageBuffer_p = messageBuffer_p + 1;\
}

#define GET_VALUE_FROM_FIRST_16_BITS(\
        messageBuffer_p,temp)\
{\
    temp = *messageBuffer_p;\
    messageBuffer_p = messageBuffer_p + 1;\
    temp |= *messageBuffer_p << 8;\
    messageBuffer_p = messageBuffer_p + 1;\
}

#define GET_VALUE_FROM_FIRST_24_BITS(\
        messageBuffer_p,temp)\
{\
    temp = *messageBuffer_p;\
    messageBuffer_p = messageBuffer_p + 1;\
    temp |= *messageBuffer_p << 8;\
    messageBuffer_p = messageBuffer_p + 1;\
    temp | = *messageBuffer_p << 16;\
    messageBuffer_p = messageBuffer_p + 1;\
}
#define GET_VALUE_FROM_FIRST_32_BITS(\
        messageBuffer_p,temp)\
{\
    temp = *messageBuffer_p;\
    messageBuffer_p = messageBuffer_p + 1;\
    temp |= *messageBuffer_p << 8;\
    messageBuffer_p = messageBuffer_p + 1;\
    temp | = *messageBuffer_p << 16;\
    messageBuffer_p = messageBuffer_p + 1;\
    temp | = *messageBuffer_p << 24;\
    messageBuffer_p = messageBuffer_p + 1;\
}
#define GET_VALUE_FROM_FIRST_40_BITS(\
        messageBuffer_p,temp)\
{\
    temp = *messageBuffer_p;\
    messageBuffer_p = messageBuffer_p + 1;\
    temp |= *messageBuffer_p << 8;\
    messageBuffer_p = messageBuffer_p + 1;\
    temp | = *messageBuffer_p << 16;\
    messageBuffer_p = messageBuffer_p + 1;\
    temp | = *messageBuffer_p << 24;\
    messageBuffer_p = messageBuffer_p + 1;\
    temp | = *messageBuffer_p << 32;\
    messageBuffer_p = messageBuffer_p + 1;\
}
#define GET_VALUE_FROM_FIRST_48_BITS(\
        messageBuffer_p,temp)\
{\
    temp = *messageBuffer_p;\
    messageBuffer_p = messageBuffer_p + 1;\
    temp |= *messageBuffer_p << 8;\
    messageBuffer_p = messageBuffer_p + 1;\
    temp | = *messageBuffer_p << 16;\
    messageBuffer_p = messageBuffer_p + 1;\
    temp | = *messageBuffer_p << 24;\
    messageBuffer_p = messageBuffer_p + 1;\
    temp | = *messageBuffer_p << 32;\
    messageBuffer_p = messageBuffer_p + 1;\
    temp | = *messageBuffer_p << 40;\
    messageBuffer_p = messageBuffer_p + 1;\
}

#define GET_4_BIT_SWAPPED_VALUE(report) (\
        ((report & 0x08) >> 3)\
        |((report & 0x04) >> 1)\
        |((report & 0x02) << 1)\
        |((report & 0x01) << 3))

#define GET_3_BIT_SWAPPED_VALUE(report) (\
        ((report & 0x04) >> 2)\
        |(report & 0x02)\
        |((report & 0x01) << 2))

#define GET_2_BIT_SWAPPED_VALUE(report) (\
        ((report & 0x02) >> 1)\
        |((report & 0x01) << 1))

#define VALIDATE_CQI_VALUE_FOR_MIN(cqiValue,ueReportedCQINodeInfo_p)\
{\
    if (INVALID_CQI_INDEX == cqiValue)\
    { \
        lteWarning("Invalid CQI [ %d] received in [%s] on line num [%d]\n",\
                cqiValue ,__func__,__LINE__);\
        \
        if(ueReportedCQINodeInfo_p)\
        {\
            freeMemPool(ueReportedCQINodeInfo_p);\
        }\
        return ;\
    }\
}

#define VALIDATE_CQI_VALUE_FOR_MAX(cqiValue,widebandCQIValue,offsetLevel)\
{\
    if ( MAX_CQI_INDEX <= cqiValue )\
    {\
        lteWarning("Invalid subband Cqi received [ %d]  for CW1 in [%s] on"\
                "line num [ %d] Offset Value = %d , widebandCQICW0 = %d \n",\
                cqiValue,__func__,__LINE__,widebandCQIValue,offsetLevel);\
        \
        freeMemPool(ueReportedCQINodeInfo_p);\
        return;\
    }\
}

#define VALIDATE_CQI_VALUE(cqiValueCW0,cqiValueCW1,offsetLevel,\
        ueReportedCQINodeInfo_p)\
{\
    if (( MAX_CQI_INDEX <= cqiValueCW1) || \
            (INVALID_CQI_INDEX >= cqiValueCW1))\
    {\
        lteWarning("Invalid CQI [ %d] received for CW1 in [%s] on line num"\
                "[ %d] ,Offset value = %d , CQIValueCW0 = %d \n",\
                cqiValueCW1,__func__,__LINE__,offsetLevel,cqiValueCW0);\
        \
        if(ueReportedCQINodeInfo_p)\
        {\
            freeMemPool(ueReportedCQINodeInfo_p);\
        }\
        return;\
    }\
}    

#define VALIDATE_COMBINATORIAL_INDEX(combinatorialIndex,\
        ueReportedCQINodeInfo_p,internalCellIndex)\
{\
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
    cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;\
    if (cellParams_p->maxCombinatorialIndex < combinatorialIndex)\
    {\
        lteWarning("Invalid combinatorial index [%d] received in [%s] on "\
                "line num [%d]\n ",combinatorialIndex,__func__,__LINE__);\
        freeMemPool(ueReportedCQINodeInfo_p);\
        return;\
    }\
}
/* Bandwidth Fix */
/*SPR-15700 Fix start*/
#define VALIDATE_PREFERRED_SUBBAND(bandwidthPartNum,preferredSubband,\
        ueReportedCQINodeInfo_p,internalCellIndex)\
{\
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
    cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;\
    switch(cellParams_p->dlAvailableRBs)\
    {\
        case 25: if((1 == bandwidthPartNum) && (2 < preferredSubband))\
                 {\
                     lteWarning("Invalid Preferred subband [%d] received at"\
                             "bandwidthPart [%d] in [%s] on line num "\
                             " [%d]\n",preferredSubband,bandwidthPartNum,\
                             __func__,__LINE__);\
                     freeMemPool(ueReportedCQINodeInfo_p);\
                     return;\
                 }\
                 break;\
        case 50: if(2 < preferredSubband)\
                 {\
                     lteWarning("Preferred subband [%d] received is out of "\
                                "range in [%s] on line num [%d]\n ",\
                                preferredSubband ,__func__,__LINE__);\
                     freeMemPool(ueReportedCQINodeInfo_p);\
                     return;\
                 }\
                 break;\
        case 75: if((1 < bandwidthPartNum) && (1 < preferredSubband))\
                 {\
                     lteWarning("Preferred subband [%d] received is out of "\
                                "range in [%s] on line num [%d]\n ",\
                                preferredSubband ,__func__,__LINE__);\
                     freeMemPool(ueReportedCQINodeInfo_p);\
                     return;\
                 }\
                 break;\
        case 100: if((0 < bandwidthPartNum) && (2 < preferredSubband))\
                  {\
                      lteWarning("Invalid Preferred subband [%d] received at"\
                              "bandwidthPart [%d] in [%s] on line num "\
                              " [%d]\n",preferredSubband,bandwidthPartNum,\
                              __func__,__LINE__);\
                      freeMemPool(ueReportedCQINodeInfo_p);\
                      return;\
                  }\
                  break;\
        default: \
                 break;\
    }\
} 
/*SPR-15700 Fix end*/
/* Bandwidth Fix */

#define DIFFERENTIAL_SUBBAND_BITS  2
/* - CQI_4.1 */
/*SPR 2311 End*/
#define NUMBER_OF_BITS_IN_1BYTE 8
#define WIDEBAND_BITS 4
#ifndef FAPI_4_0_COMPLIANCE

/*****************************************************************************
 * Function Name  : processUlHarqInd 
 * Inputs         : pHarqMsg - pointer to FAPI_harqIndication_st,
 *                  recvTTI, sysFrameNum, subFrameNum,
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for processing UL HARQ indication from
 *                  PHY
 *****************************************************************************/
#ifdef FDD_CONFIG
 void processUlHarqInd( void* msg_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt16 sysFrameNum, 
        UInt8 subFrameNum,
        /*CA Changes start  */      
        InternalCellIndex internalCellIndex)
/*CA Changes end  */      
{

    UInt32              iterator = 0; 
    /*FAPI2_1changes start*/
    FAPI_harqIndication_st *pHarqMsg = (FAPI_harqIndication_st *)msg_p; 
    /*FAPI2_1changes end*/

    UInt16              ueIndex = 0;
    UInt16              recvRnti = 0;
    FAPI_fddHarqPduIndication_st    *pFddHarqPdu = &pHarqMsg->harqPduInfo[0];
    RNTIInfo                        *pRntiInfo = PNULL;
    /* SPR 4934 Changes*/
    TempCRNTICtx *tempUEContext_p = PNULL;
    /* SPR 4934 Changes*/

    /* CA_phase2_harq_code Start */
    UInt16       dlAckNack          = 0;
    /* CA_phase2_harq_code End */

#ifdef LOG_PRINT_ENABLED
    /* SPR 15909 fix start */
    tickType_t currentTTI=getCurrentTick();
    /* SPR 15909 fix end */
#endif        
    for (iterator = MAC_PHY_CONVERT_16(pHarqMsg->numOfHarq); iterator > 0; iterator--)
    {
        recvRnti = MAC_PHY_CONVERT_16(pFddHarqPdu->rnti); 
        /*CA Changes start  */
        pRntiInfo = getUeIdxFromRNTIMap(recvRnti, internalCellIndex);
        /*CA Changes end  */
        if ( (PNULL == pRntiInfo || FREERNTI == pRntiInfo->rntiFlag))
        {
            /* SPR#688 Changes Start */ 
            LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"processUlHarqInd:Recv Rnti %d is not Active at Mac Layer\n",pFddHarqPdu->rnti);
            /* SPR#688 Changes End */
             /*SPR 19565 warning fix*/
            LOG_MAC_MSG(MAC_HI_RX_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
                    currentTTI,\
                    recvRnti, 
#ifdef __x86_64__
                    (UInt64)pRntiInfo,
#else
                    (UInt32)pRntiInfo,
#endif
                    (pRntiInfo ? pRntiInfo->rntiFlag : 0), \
                    recvTTI,sysFrameNum,\
                    0.0,0.0,__func__,"HI_RX_RNTIERR");
            /*SPR 19565 warning fix*/

            pFddHarqPdu = (FAPI_fddHarqPduIndication_st *)
                (((UInt8 *)pFddHarqPdu)
                 + sizeof(FAPI_fddHarqPduIndication_st));
            continue;
        }
        else if ( CRNTI == pRntiInfo->rntiFlag )
        {
            ueIndex = pRntiInfo->index;
            if ( dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
            {
                /* SPR#688 Changes Start */ 
                LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"processUlHarqInd: Pending Delete Flag True or UE Index %d is not Active",ueIndex);
                /* SPR#688 Changes End */ 

                LOG_MAC_MSG(MAC_HI_RX_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
                        currentTTI,\
                        recvRnti, \
                        ueIndex,\
                        recvTTI,0,0,\
                        0.0,0.0,__func__,"HI_RX_UEDELETE");

                pFddHarqPdu = (FAPI_fddHarqPduIndication_st *)
                    (((UInt8 *)pFddHarqPdu)
                     + sizeof(FAPI_fddHarqPduIndication_st));
                continue;
            }
        }
        else if ( SPS_RNTI == pRntiInfo->rntiFlag )
        {
            ueIndex = pRntiInfo->index;
            if ( dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
            {
                LOG_MAC_MSG(MAC_HI_RX_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
                        currentTTI,\
                        recvRnti, \
                        ueIndex,\
                        recvTTI,0,0,\
                        0.0,0.0,__func__,"HI_RX_UEDELETE");

                pFddHarqPdu = (FAPI_fddHarqPduIndication_st *)
                    (((UInt8 *)pFddHarqPdu)
                     + sizeof(FAPI_fddHarqPduIndication_st));
                continue;
            }
        }
        else if ( TCRNTI == pRntiInfo->rntiFlag )
        {
            ueIndex = pRntiInfo->index;
            /* SPR 4934 Changes*/
            tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].tempCRNTICtx_p;
            if(CONTENTION_SENT == tempUEContext_p->status)
            {
                /* If this is ACK */
                if(ACK == pFddHarqPdu->harqTB1)
/*SPR 21001 Changes end */
                {
                    ContentionResAckNode *contentionAckQueueNode_p = PNULL;
                    GET_MEM_FROM_POOL(ContentionResAckNode,contentionAckQueueNode_p,sizeof(ContentionResAckNode), PNULL);

                    /* Rel 5.3: Coverity 24453 Fix Start */
                    if (PNULL != contentionAckQueueNode_p)
                    {
                        contentionAckQueueNode_p->ueIndex = ueIndex;
                        /*CA Changes start  */
                        if (pushNode(contentionResolutionMsgAckQ_gp[internalCellIndex], 
                                    &contentionAckQueueNode_p->nodeAnchor))
                            /*CA Changes end  */
                        {
                            lteWarning("pushNode() failed for "
                                    "contentionResolutionMsgAckQ_g queue");

                            LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGFATAL,L2_SYS_FAIL,\
                                    getCurrentTick(),\
                                    __LINE__,0,0,0,0,\
                                    0.0,0.0,__FILE__,__func__);

                            /* SPR 924  Start */ 
                            freeMemPool(contentionAckQueueNode_p);
                            /* SPR 924  End */ 
                        }
                        else
                        {
                            /* SPR 4934 Changes*/
                            tempUEContext_p->status = CONTENTION_ACK_RECD;
                            /* SPR 4934 Changes*/
                            LOG_MAC_MSG(MAC_CONTENTION_ACK_RCVD_ID,LOGDEBUG,MAC_L1_INF,\
                                    currentTTI,\
                                    recvRnti, \
                                    ueIndex,\
                                    recvTTI,0,0,\
                                    0.0,0.0,__func__,"CONTENTION_ACK_RCVD");
                        }
                    }
                    else
                    {
                        /* Not able to allocate Memory */
                    }
                    /* Rel 5.3: Coverity Fix End */
                    /* + SPR 17733 */
/*SPR 21001 Changes start */
/* SPR 22569 Fix + */
#if !defined (DL_UL_SPLIT) && !defined (INTEL_NOCA_L1)
/* SPR 22569 Fix - */
                   dlAckNack = (pFddHarqPdu->harqTB1 & 0x07);
               putEntryInAckNackQueueForTcrnti( ueIndex,
                          dlAckNack, subFrameNum,internalCellIndex );
#endif
/*SPR 21001 Changes end */
                }
                else
                {
/*SPR 21001 Changes start */
/* SPR 22569 Fix + */
#if !defined (DL_UL_SPLIT) && !defined (INTEL_NOCA_L1)
/* SPR 22569 Fix - */
                    /* Coverity_Fix 96897 ++ */
                    dlAckNack = 0;
                    /* Coverity_Fix 96897 -- */
                  putEntryInAckNackQueueForTcrnti( ueIndex,
                            dlAckNack, subFrameNum ,internalCellIndex );
                    /* - SPR 17733 */
#endif
/*SPR 21001 Changes end */
                }
            }
            /* SPR 4934 Changes*/
        }
        if (TCRNTI != pRntiInfo->rntiFlag)
        {
            /* Its an CRNTI */
            /* Instead of using only first two value ACK/ NACK now we are
             * considering all the values received from FAPI interface i.e 1-7
             */
            dlAckNack = (pFddHarqPdu->harqTB1 & 0x07) |
                ((pFddHarqPdu->harqTB2 & 0x07) << 4);
/*SPR 21001 Changes end */

            LOG_UT(MAC_HI_CRNTI_RX_ID,LOGDEBUG,MAC_L1_INF,\
                    currentTTI,\
                    ueIndex,\
                    pFddHarqPdu->harqTB1,\
                    pFddHarqPdu->harqTB2,\
                    subFrameNum,\
                    recvTTI,\
                    0.0,0.0,__func__,"HI_CRNTI_RX");

            putEntryInAckNackQueue(ueIndex,
                    dlAckNack,
                    subFrameNum,
                    pRntiInfo->rntiFlag
                    /* 32.425 Events code changes start*/
                    /* +- SPR 17777 */
#ifndef DL_UL_SPLIT
#ifdef KPI_STATS
                    ,recvTTI
#endif
#endif
                    /* +- SPR 17777 */
                    /* 32.425 Events code changes end*/
                    /*CA Changes start  */
                    ,internalCellIndex
                    /*CA Changes end  */
                    /* + SPR_15971 */
                    ,HARQ_MODE_1A
                    /* - SPR_15971 */

                    );
        }
/*SPR 21001 Changes start */
        pFddHarqPdu = (FAPI_fddHarqPduIndication_st *)(((UInt8 *)pFddHarqPdu)
                + sizeof(FAPI_fddHarqPduIndication_st));
/*SPR 21001 Changes end */
    }
}
#elif TDD_CONFIG
/*****************************************************************************
 * Function Name  : processUlHarqInd
 * Inputs         : pHarqMsg - pointer to FAPI_harqIndication_st,
 *                  recvTTI, sysFrameNum, subFrameNum,
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for processing UL HARQ indication from
 *                  PHY.
 *****************************************************************************/
 void processUlHarqInd( void *msg_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt16 sysFrameNum,
        UInt8 subFrameNum,
        InternalCellIndex internalCellIndex )
{
    UInt32              dlAckNack = 0;
    UInt16              ueIndex = 0;
    FAPI_tddHarqPduIndication_st   *pTddHarqPdu = PNULL;
    FAPI_tddBundlingHarqInfo_st    *pTddBundlingPdu = PNULL;
    FAPI_tddMultiplexingHarqInfo_st *pTddMultiplexingPdu = PNULL;
    RNTIInfo                       *pRntiInfo = PNULL;
    /* SPR 4934 Changes*/
    TempCRNTICtx *tempUEContext_p = PNULL;
    /* SPR 4934 Changes*/
    FAPI_harqIndication_st *pHarqMsg=(FAPI_harqIndication_st*)msg_p; 
    UInt16              numOfHarq = MAC_PHY_CONVERT_16(pHarqMsg->numOfHarq);

    /* SPR 10922 chnages start */
    /* The 1st pTddHarqPdu to the beginning of pHarqMsg is 4 bytes */
    UInt16             harqPduOffset = 4;
    /* SPR 10922 chnages end */

	UInt8 numOfAckNack = 0;

    /* The RNTI passed to the PHY in an uplink subframe configuration PDU.
     * Value: 1 . 65535.  */
    while (numOfHarq--)
    {
        /* SPR 10922 chnages start */
        /* Advance pTddHarqPdu to the next HARQ PDU.
           harqPduOffset varies based on HARQ mode.
           Refer to table 76-79 in FAPI spec v1.1 */
        pTddHarqPdu = ( FAPI_tddHarqPduIndication_st *)
            (((UInt8 *)pHarqMsg)
             + harqPduOffset);
        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
        if (HARQ_BUNDLING_MODE == pTddHarqPdu->mode) /* bundling */
        {
#ifdef FLEXRAN
            harqPduOffset += (10 + pTddHarqPdu->numOfAckNack);
#else
            harqPduOffset += 10;
#endif
        }
        else if (HARQ_MULTIPLEXING_MODE == pTddHarqPdu->mode) /* multiplexing */
        {
#ifdef FLEXRAN
            harqPduOffset += (10 + pTddHarqPdu->numOfAckNack);
#else
            harqPduOffset += 12;
#endif
        }
        else if (HARQ_SPECIAL_BUNDLING_MODE == pTddHarqPdu->mode) /* special bundling */
        {
#ifdef FLEXRAN
            harqPduOffset += 11;
#else
            harqPduOffset += 9;
#endif
        }
		else if( HARQ_CHANNEL_SELECTION_MODE == pTddHarqPdu->mode) /* Channel Selection */
        {
#ifdef FLEXRAN
            harqPduOffset += (10 + pTddHarqPdu->numOfAckNack);
#else        
            /*SPR 14274 fix start*/
            harqPduOffset = harqPduOffset + 12 +
                        ( sizeof(FAPI_tddChannelSelectionHarqInfo_st) *
                          (pTddHarqPdu->release9Param.numOfAckNack));
            /*SPR 14274 fix end*/
#endif			
        }
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
        else
        {
            ltePanic ("[%s] Invalid Value of mode %d",
                    __func__, pTddHarqPdu->mode);
        }
        /* SPR 10922 changes end */
        pRntiInfo = getUeIdxFromRNTIMap(MAC_PHY_CONVERT_16(pTddHarqPdu->rnti),internalCellIndex);

        if ( (PNULL == pRntiInfo 
                    || FREERNTI == pRntiInfo->rntiFlag))
        {
            LTE_MAC_UT_LOG( LOG_INFO, 
                    RECV_INTF,
                    "\n[%s]Recv Rnti %d is not Active at Mac Layer\n",
                    __func__, 
                    pTddHarqPdu->rnti);

            continue;
        }
        else if ( CRNTI == pRntiInfo->rntiFlag )
        {
            dlAckNack          = 0;
            ueIndex = pRntiInfo->index;
            if ( dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
            {
                LTE_MAC_UT_LOG( LOG_INFO, RECV_INTF,
                        "\n[%s] Pending Delete Flag True or UE Index %d is not Active\n",
                        __func__, ueIndex);
                continue;
            }
        }
        else if ( SPS_RNTI == pRntiInfo->rntiFlag )
        {
            dlAckNack          = 0;
            ueIndex = pRntiInfo->index;
            if ( dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
            {
                LTE_MAC_UT_LOG( LOG_INFO, RECV_INTF,
                        "\n[%s] Pending Delete Flag True or UE Index %d is not Active\n",
                        __func__, ueIndex);
                continue;
            }
        }

        /* The format of the ACK/NACK response expected. The bundling and
         *  0 = BUNDLING
         *  1 = MULTIPLEXING 
         *  2 = SPECIAL BUNDLING
         *  3 = CHANNAL SELECTION
         */

        if (  pTddHarqPdu->mode > 3)  //TWO CA MOD ZS
        {
            /* SPR# 9438,9471 fix start */  
            /* return from this function as harq received is not currect a\hence discard further processing */
            lteWarning ("[%s] Invalid Value of mode=%d\n", __func__, pTddHarqPdu->mode);

            return;
            /* SPR# 9438,9471 fix end */  

        }

        /* The number of ACK/NACK results reported for this UE.
         * Value: 1 . 4
         * Value: 0 . 9 (Special Bundling)
         * uint8_t     numOfAckNack;*/
        if ( pTddHarqPdu->numOfAckNack > 9)
        {
            /* SPR# 9438,9471 fix start */
            /* return from this function as harq received is not currect a\hence discard further processing */
            lteWarning("Invalid value recv for numOfAckNack=%d\n",pTddHarqPdu->numOfAckNack);

            return;
            /* SPR# 9438,9471 fix end */
        }
        /* Indicates HARQ results.
           Range 1 . 7
           1 = ACK
           2 = NACK
           3 = ACK or NACK
           4 = DTX
           5 = ACK or DTX
           6 = NACK or DTX
           7 = ACK or NACK or DTX    

           uint8_t  value0;
           uint8_t  value1;
           */

        if (TCRNTI != pRntiInfo->rntiFlag)
        {
            /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
            dlAckNack          = 0;
            if (pTddHarqPdu->mode == HARQ_BUNDLING_MODE)//bundling mode
            {
                pTddBundlingPdu = (FAPI_tddBundlingHarqInfo_st *)&pTddHarqPdu->harqBuffer; 
                /* Instead of using only first two value ACK/ NACK now we are
                 * considering all the values received from FAPI interface
                 * i.e 1-7 */
#ifdef FLEXRAN
                 if (pTddHarqPdu->numOfAckNack == 2)
                 {
					 if (pTddBundlingPdu->value0 != 1)
						 numOfHarqNack_g[internalCellIndex]++;
					 if (pTddBundlingPdu->value1 != 1)
						 numOfHarqNack_g[internalCellIndex]++;
                 }
                 else if (pTddHarqPdu->numOfAckNack == 1)
                 {
					 if (pTddBundlingPdu->value0 != 1)
						 numOfHarqNack_g[internalCellIndex]++;
                 }
				 numOfHarq_g[internalCellIndex] += pTddHarqPdu->numOfAckNack;
#endif
                dlAckNack = (pTddBundlingPdu->value0  & 0x07) |
                    ( (pTddBundlingPdu->value1  & 0x07) << 4);
                tddPutEntryInAckNackQueueForCurConfig_p[internalCellIndex]( ueIndex,
                        dlAckNack,
                        recvTTI,
                        sysFrameNum,
                        subFrameNum,
                        internalCellIndex,
                        pTddHarqPdu->mode
                        );

				LOG_MAC_MSG (MAC_HARQ_IND_RECV_ID, LOGBRIEF, MAC_DL_HARQ,\
						getCurrentTick(),
						pTddHarqPdu->rnti,
						pTddHarqPdu->mode,
						pTddHarqPdu->numOfAckNack,
						dlAckNack,
						internalCellIndex,
						DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
						__func__, "CRNTI_HARQ_BUNDLING");
  
                /*Rel_523_Coverity_10765 Fix start*/
                if (pTddBundlingPdu->value0 == FAPI_NACK)
                    pTddBundlingPdu->value0 = NACK;
                /*Rel_523_Coverity_10765 Fix end*/
            }
            else if(pTddHarqPdu->mode == HARQ_MULTIPLEXING_MODE) //multiplexing mode
            {
                pTddMultiplexingPdu = (FAPI_tddMultiplexingHarqInfo_st *)&pTddHarqPdu->harqBuffer;
                dlAckNack = ((pTddMultiplexingPdu->value0 & 0x07)|
                        ((pTddMultiplexingPdu->value1 & 0x07) << 4)|
                        ((pTddMultiplexingPdu->value2 & 0x07) << 8)|
                        ((pTddMultiplexingPdu->value3 & 0x07) << 12));

                tddPutEntryInAckNackQueueForCurConfig_p[internalCellIndex]( ueIndex,
                        dlAckNack,
                        recvTTI,
                        sysFrameNum,
                        subFrameNum,
                        internalCellIndex,
                        pTddHarqPdu->mode
                        );

				LOG_MAC_MSG (MAC_HARQ_IND_RECV_ID, LOGBRIEF, MAC_DL_HARQ,\
						getCurrentTick(),
						pTddHarqPdu->rnti,
						pTddHarqPdu->mode,
						pTddHarqPdu->numOfAckNack,
						dlAckNack,
						internalCellIndex,
						DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
						__func__, "CRNTI_HARQ_MULTIPLEXING");
            }
            else if(HARQ_SPECIAL_BUNDLING_MODE == pTddHarqPdu->mode)
            {
                pTddBundlingPdu = (FAPI_tddBundlingHarqInfo_st *)&pTddHarqPdu->harqBuffer; 
#if 0
                if(tddHarqSpecialbundlingMap_g[pTddHarqPdu->numOfAckNack] == pTddBundlingPdu->value0)
                {
                    dlAckNack = FAPI_ACK | FAPI_ACK << 4; /* Considering all ACKs*/
                }
                else
                {
                    dlAckNack = FAPI_NACK | FAPI_NACK << 4;/* Considering all NACKs*/
#ifdef FLEXRAN
		    numOfHarqNack_g[internalCellIndex] += pTddHarqPdu->numOfAckNack;
#endif
                }
#else
				UInt8 nBundle = dlUECtxInfoArr_g[ueIndex].dlUEContext_p->nBundle[sysFrameNum % 2][subFrameNum];
				if ((0 != pTddBundlingPdu->value0)
					&& (tddHarqSpecialbundlingMap_g[nBundle] == pTddBundlingPdu->value0))
                {
                    dlAckNack = FAPI_ACK | FAPI_ACK << 4; /* Considering all ACKs*/
                }
                else
                {
#ifdef MAC_CA_WORKAROUND
                    dlAckNack = FAPI_ACK | FAPI_ACK << 4;/* Considering all ACKs*/
#else
                    dlAckNack = FAPI_NACK | FAPI_NACK << 4; /* Considering all NACKs*/
#endif
#ifdef FLEXRAN
					numOfHarqNack_g[internalCellIndex] += nBundle;
#endif
                }
                numOfHarq_g[internalCellIndex] += nBundle;
#endif

                tddPutEntryInAckNackQueueForCurConfig_p[internalCellIndex]( ueIndex,
                        dlAckNack,
                        recvTTI,
                        sysFrameNum,
                        subFrameNum,
                        internalCellIndex,
                        pTddHarqPdu->mode
                        );

				LOG_MAC_MSG (MAC_HARQ_IND_RECV_ID, LOGBRIEF, MAC_DL_HARQ,\
						getCurrentTick(),
						pTddHarqPdu->rnti,
						pTddHarqPdu->mode,
						nBundle,
						dlAckNack,
						internalCellIndex,
						DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
						__func__, "CRNTI_HARQ_SPECIAL_BUNDLING");
            }
			/* + CA_TDD_HARQ_CHANGES */
            else if (HARQ_CHANNEL_SELECTION_MODE == pTddHarqPdu->mode) /* Channel selection */
            {

                numOfAckNack = pTddHarqPdu->numOfAckNack;

                while(numOfAckNack--)
                {
                    pTddHarqPdu->harqBuffer[numOfAckNack]=1;
                    dlAckNack |= 
                        (pTddHarqPdu->harqBuffer[numOfAckNack] & 0x07 )\
                        << (4 * numOfAckNack);
                }

                tddPutEntryInAckNackQueueForCurConfig_p[internalCellIndex]( ueIndex,
                        dlAckNack,
                        recvTTI,
                        sysFrameNum,
                        subFrameNum,
                        internalCellIndex,
                        pTddHarqPdu->mode
                        );

				LOG_MAC_MSG (MAC_HARQ_IND_RECV_ID, LOGBRIEF, MAC_DL_HARQ,\
						getCurrentTick(),
						pTddHarqPdu->rnti,
						pTddHarqPdu->mode,
						pTddHarqPdu->numOfAckNack,
						dlAckNack,
						internalCellIndex,
						DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
						__func__, "CRNTI_HARQ_CHANNEL_SELECTION");

            }     
            /* - CA_TDD_HARQ_CHANGES */
        }

        if ( TCRNTI == pRntiInfo->rntiFlag )
        {
            ueIndex = pRntiInfo->index;
            /* SPR 4934 Changes*/
            tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].tempCRNTICtx_p;
            if(CONTENTION_SENT == tempUEContext_p->status)
            {
                if (pTddHarqPdu->mode == HARQ_BUNDLING_MODE)//bundling mode
                {
                    /* If this is ACK */
                    pTddBundlingPdu = (FAPI_tddBundlingHarqInfo_st *)&pTddHarqPdu->harqBuffer; 
                    /* SPR 19679: TDD HARQ Multiplexing Changes End */
                    if( FAPI_ACK == pTddBundlingPdu->value0)
                    {
                        ContentionResAckNode *contentionAckQueueNode_p = PNULL;
                        GET_MEM_FROM_POOL(ContentionResAckNode,contentionAckQueueNode_p,sizeof(ContentionResAckNode), PNULL);
                        /*+COVERITY 5.3.0 - 24453*/
                        if ( PNULL != contentionAckQueueNode_p )
                        {
                            contentionAckQueueNode_p->ueIndex = ueIndex;
                            if (pushNode(contentionResolutionMsgAckQ_gp[internalCellIndex],
                                        &contentionAckQueueNode_p->nodeAnchor))
                            {
                                lteWarning("pushNode() failed for "
                                        "contentionResolutionMsgAckQ_g queue");
                                freeMemPool(contentionAckQueueNode_p);
                            }
                            /* SPR 4934 Changes*/
                            else
                            {
                                tempUEContext_p->status = CONTENTION_ACK_RECD;
                            }
                            /* SPR 4934 Changes*/
                        }
                        /*-COVERITY 5.3.0 - 24453*/
                    }
                    
					LOG_MAC_MSG (MAC_HARQ_IND_RECV_ID, LOGINFO, MAC_DL_HARQ,\
							getCurrentTick(),
							pTddHarqPdu->rnti,
							pTddHarqPdu->mode,
							pTddHarqPdu->numOfAckNack,
							pTddBundlingPdu->value0,
							internalCellIndex,
							DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
							__func__, "TCRNTI_HARQ");	
                }

                /* MAC shall decode contention resolution TDD ACK-NACK
                 * according to TDD HARQ bundling feedback only. Refer
                 * specification 36.331 section 9.2.4 */
            }
            /* SPR 4934 Changes*/
        }
    }
}
#endif
#endif
/*FAPI2_1 changes start*/
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : processUlHarqIndR9
 * Inputs         : 1. HARQ Ind message 2. TTI 3. SFN 4. SF
 *                  5. internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for processing UL HARQ indication from
 *                  PHY in case Release Capability is Release9 and Later
 *****************************************************************************/
 void processUlHarqIndR9( void* msg_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt16 sysFrameNum, 
        UInt8 subFrameNum,
        /*CA Changes start  */      
        InternalCellIndex internalCellIndex)
/*CA Changes end  */   
{
    UInt16              ueIndex = 0;
    UInt16              recvRnti = 0;
    /*FAPI2_1changes start*/
    FAPI_harqIndicationR9_st *pHarqMsg = (FAPI_harqIndicationR9_st *)msg_p; 
    UInt8 numacknack=0;
    /*FAPI2_1changes end*/
    /* SPR 15322 fix start */
    UInt16  numOfHarq = MAC_PHY_CONVERT_16(pHarqMsg->numOfHarq);
    UInt16  harqPduOffset = HARQ_IND_CONST_LEN_R9;
    FAPI_fddHarqPduIndicationR9_st    *pFddHarqPdu = PNULL;
    /* SPR 15322 fix end */
    RNTIInfo                        *pRntiInfo = PNULL;
    /* SPR 4934 Changes*/
    TempCRNTICtx *tempUEContext_p = PNULL;
    /* SPR 4934 Changes*/

    /* CA_phase2_harq_code Start */
    UInt16       dlAckNack          = 0;
    UInt8        numOfAckNack        = 0;
    /* CA_phase2_harq_code End */
    
    /* SPR 15322 fix start */
    while(numOfHarq--)
    {
        pFddHarqPdu = (FAPI_fddHarqPduIndicationR9_st *) (((UInt8 *)pHarqMsg) + harqPduOffset);
       
        /* Here, using offsetof() to calculate value of harqPduOffset as, 
	 * sizeof(FAPI_fddHarqPduIndicationR9_st) returns value 16 by adding 
	 * three bytes padding in the internal structure FAPI_fddHarqR9params_st,
	 * due to which incorrect value of harqPduOffset is calculated. */ 

        harqPduOffset +=  ((offsetof(FAPI_fddHarqPduIndicationR9_st,release9Param)) + 
                          (offsetof(FAPI_fddHarqR9params_st, HARQTB)) +
                          (sizeof(UInt8) * pFddHarqPdu->release9Param.numOfAckNack));

    /* SPR 15322 fix end */
        recvRnti = MAC_PHY_CONVERT_16(pFddHarqPdu->rnti); 
        pRntiInfo = getUeIdxFromRNTIMap(recvRnti,internalCellIndex);

        /* Supported Modes 
         *  0 = Format 1a/1b
         *  1 = ChannelSelectio
         *  2 = Format3
         */

        if (  pFddHarqPdu->release9Param.mode > MAX_MODE_FDDHARQ_REL9)
            /*TDD special bundling changes end*/
        {

            /* +- SPR 17777 */
            LOG_MAC_MSG(MAC_HI_MODE ,LOGDEBUG,MAC_L1_INF,\
                    0,\
                    recvRnti,\
                    pFddHarqPdu->release9Param.mode,\
                    recvTTI,sysFrameNum,0,\
                    0.0,0.0,__func__,"INVALID_MODE_VALUE");
            /* +- SPR 17777 */

            /* SPR 15322 Fix Start */
            //code removed 
            /* SPR 15322 _Fix End*/

            continue;


        }
        /* The number of ACK/NACK results reported for this UE.
         *  Value: 1 . 2 Format 1a/1b 
         *  Value: 1 . 4 Channel Selection
         *  Value: 1 . 10  Format 3
         */

	    /* + Coverity 72757 */
        /* There can be maximum 4 HARQ status in FDD when 2 DL CA carriers are used */
        if ( pFddHarqPdu->release9Param.numOfAckNack > MAX_NUMACKNACK_FDDHARQ_REL9_SUPPORTED ||
                pFddHarqPdu->release9Param.numOfAckNack == 0) 
	    /* - Coverity 72757 */
        {


            LOG_MAC_MSG( MAC_HI_NUM_ACK_NACK ,LOGDEBUG,MAC_L1_INF,\
                    0,\
                    recvRnti,\
                    pFddHarqPdu->release9Param.numOfAckNack	,\
                    recvTTI,0,0,\
                    0.0,0.0,__func__,"INVALID_NUM_ACK_NACK_VALUE");

            /* SPR 15322 Fix Start */
            //code removed 
            /* SPR 15322 _Fix End*/
            continue;

        }


        if ( (PNULL == pRntiInfo || FREERNTI == pRntiInfo->rntiFlag))
        {
            /* SPR#688 Changes Start */ 
            LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"processUlHarqIndR9:Recv Rnti %d is not Active at Mac Layer\n",pFddHarqPdu->rnti);
            /* SPR#688 Changes End */
/* SPR 20636 Changes Start */
            LOG_MAC_MSG(MAC_HI_RX_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
                    0,\
                    recvRnti,\
                    (ADDR)pRntiInfo,\
                    (pRntiInfo ? pRntiInfo->rntiFlag : 0), \
                    recvTTI,0,\
                    0.0,0.0,__func__,"HI_RX_RNTIERR");

/* SPR 20636 Changes End */
            /* SPR 15322 Fix Start */
            //code removed 
            /* SPR 15322 _Fix End*/
            continue;
        }
        else if ( CRNTI == pRntiInfo->rntiFlag )
        {
            dlAckNack          = 0;
            ueIndex = pRntiInfo->index;
            if ( dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
            {
                /* SPR#688 Changes Start */ 
                LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"processUlHarqIndR9: Pending Delete Flag True or UE Index %d is not Active",ueIndex);
                /* SPR#688 Changes End */ 

                LOG_MAC_MSG(MAC_HI_RX_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
                        0,\
                        recvRnti,\
                        ueIndex,\
                        recvTTI,0,0,\
                        0.0,0.0,__func__,"HI_RX_UEDELETE");

            /* SPR 15322 Fix Start */
            //code removed 
            /* SPR 15322 _Fix End*/
                continue;
            }
        }
        else if ( SPS_RNTI == pRntiInfo->rntiFlag )
        {
            dlAckNack          = 0;
            ueIndex = pRntiInfo->index;
            if ( dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
            {
                LOG_MAC_MSG(MAC_HI_RX_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
                        0,\
                        recvRnti,\
                        ueIndex,\
                        recvTTI,0,0,\
                        0.0,0.0,__func__,"HI_RX_UEDELETE");

            /* SPR 15322 Fix Start */
            //code removed 
            /* SPR 15322 _Fix End*/
                continue;
            }
        }
        else if ( TCRNTI == pRntiInfo->rntiFlag )
	{
		dlAckNack          = 0;
		ueIndex = pRntiInfo->index;
		/* SPR 4934 Changes*/
		tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].tempCRNTICtx_p;
		if(CONTENTION_SENT == tempUEContext_p->status)
		{
			/* If this is ACK */
			if(ACK == pFddHarqPdu->release9Param.HARQTB[numacknack])
			{
				ContentionResAckNode *contentionAckQueueNode_p = PNULL;
				GET_MEM_FROM_POOL(ContentionResAckNode,contentionAckQueueNode_p,sizeof(ContentionResAckNode), PNULL);
				if (PNULL != contentionAckQueueNode_p)
				{

					contentionAckQueueNode_p->ueIndex = ueIndex;
					if (pushNode(contentionResolutionMsgAckQ_gp[internalCellIndex], 
								&contentionAckQueueNode_p->nodeAnchor))
					{
						lteWarning("pushNode() failed for "
								"contentionResolutionMsgAckQ_g queue");

						LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGFATAL,L2_SYS_FAIL,\
								getCurrentTick(),\
								__LINE__,0,0,0,0,\
								0.0,0.0,__FILE__,__func__);

						/* SPR 924  Start */ 
						freeMemPool(contentionAckQueueNode_p);
						/* SPR 924  End */ 
					}
					else
					{
						/* SPR 4934 Changes*/
						tempUEContext_p->status = CONTENTION_ACK_RECD;
						/* SPR 4934 Changes*/
						LOG_MAC_MSG(MAC_CONTENTION_ACK_RCVD_ID,LOGDEBUG,MAC_L1_INF,\
								0,\
								pFddHarqPdu->rnti,\
								ueIndex,\
								recvTTI,0,0,\
								0.0,0.0,__func__,"CONTENTION_ACK_RCVD");
					}
				}
				/* + SPR 17733 */
/* SPR 22569 Fix + */
#if !defined (DL_UL_SPLIT) && !defined (INTEL_NOCA_L1)
/* SPR 22569 Fix - */
				dlAckNack = (pFddHarqPdu->release9Param.HARQTB[numacknack] & 0x07);
				putEntryInAckNackQueueForTcrnti( ueIndex,
						dlAckNack, subFrameNum,internalCellIndex );
#endif
			}
			else
			{
				/* coverity fix 99542 */
/* SPR 22569 Fix + */
#if !defined (DL_UL_SPLIT) && !defined (INTEL_NOCA_L1)
/* SPR 22569 Fix - */
				dlAckNack = 0; /* This should be zero for NACK/DTX */
				putEntryInAckNackQueueForTcrnti( ueIndex,
						dlAckNack, subFrameNum ,internalCellIndex );
				/* - SPR 17733 */
#endif
			}
		}
		/* SPR 4934 Changes*/
	}

        if (TCRNTI != pRntiInfo->rntiFlag)
        {
            /* Its an CRNTI */
            /* Instead of using only first two value ACK/ NACK now we are
             * considering all the values received from FAPI interface i.e 1-7
             */

            /* CA_phase2_harq_code Start */

            numOfAckNack = pFddHarqPdu->release9Param.numOfAckNack;
            while(numOfAckNack--)
            {
                	dlAckNack |= 
                	(*(pFddHarqPdu->release9Param.HARQTB + numOfAckNack) & 0x07 )\
                      	<< (4 * numOfAckNack);
            }

            /* CA_phase2_harq_code End */

            LOG_UT(MAC_HI_CRNTI_RX_ID,LOGDEBUG,MAC_L1_INF,\
                    0,\
                    ueIndex,\
            /* CA_phase2_harq_code Start */
                    pFddHarqPdu->release9Param.HARQTB[0],\
                    pFddHarqPdu->release9Param.HARQTB[1],\
            /* CA_phase2_harq_code End */
                    subFrameNum,\
                    recvTTI,\
                    0.0,0.0,__func__,"HI_CRNTI_RX");

            putEntryInAckNackQueue(ueIndex,
                    dlAckNack,
                    subFrameNum,
                    pRntiInfo->rntiFlag
                    /* 32.425 Events code changes start*/
                    /* +- SPR 17777 */
#ifndef DL_UL_SPLIT
#ifdef KPI_STATS
                    ,recvTTI
#endif
#endif
                    /* +- SPR 17777 */
                    /* 32.425 Events code changes end*/
                    /*CA Changes start  */
                    ,internalCellIndex
                    /* + SPR_15971 */
                    ,pFddHarqPdu->release9Param.mode
                    /* - SPR_15971 */
                    /*CA Changes end  */
                    );
        }
    /* SPR 15322 fix start */
#ifndef REAL_UE_ENVIORNMENT 
        /* Considering Harq pdu Size is multiple of 4 */
        if ( harqPduOffset % sizeof(UInt32) )
        {
            harqPduOffset += (sizeof(UInt32)) - (harqPduOffset % sizeof(UInt32));
        }
#endif
    }
    /* SPR 15322 fix end */
}
#elif TDD_CONFIG
/*****************************************************************************
 * Function Name  : processUlHarqIndR9
 * Inputs         : 1. HARQ Ind message 2. TTI 3. SFN 4. SF
 *                  5. internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for processing UL HARQ indication from
 *                  PHY in case of TDD and release capability Release9 and later
 *****************************************************************************/
 void processUlHarqIndR9( void *msg_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt16 sysFrameNum,
        UInt8 subFrameNum,
        /*CA Changes start  */      
        InternalCellIndex internalCellIndex)
/*CA Changes end  */     
{
    UInt32              dlAckNack = 0;
    UInt16              ueIndex = 0;
        /*TDD_FAPI_2.4 changes  START*/
    /* SPR 14555 Start */
    UInt16               harqPduOffset=4;
    /* SPR 14555 End */
        /*TDD_FAPI_2.4 changes  END*/



    FAPI_tddHarqPduIndicationR9_st   *pTddHarqPdu = PNULL;
    /*TDD_FAPI_2.4 changes  START*/
    FAPI_tddBundlingHarqInfoR9_st    *pTddBundlingPdu = PNULL;
    FAPI_tddBundlingHarqInfoR9_st    *pTddBundlingPdu_next = PNULL;
    /*TDD_FAPI_2.4 changes  END*/
    /* + CA_TDD_HARQ_CHANGES */
    FAPI_tddChannelSelectionHarqInfo_st *pTddChannelSelectionPdu = PNULL;
    UInt8 numOfAckNack = 0;
    /* - CA_TDD_HARQ_CHANGES */
    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
    FAPI_tddMultiplexingHarqInfoR9_st *pTddMultiplexingR9Pdu = PNULL;
    /* SPR 19679: TDD HARQ Multiplexing Changes End */
    RNTIInfo                       *pRntiInfo = PNULL;
    /* SPR 4934 Changes*/
    TempCRNTICtx *tempUEContext_p = PNULL;
    /* SPR 4934 Changes*/
    FAPI_harqIndicationR9_st *pHarqMsg=(FAPI_harqIndicationR9_st*)msg_p; 
    UInt16  numOfHarq = MAC_PHY_CONVERT_16(pHarqMsg->numOfHarq);
    /* The RNTI passed to the PHY in an uplink subframe configuration PDU.
Value: 1 . 65535.  */
    /*  uint16_t    rnti;  */
    while (numOfHarq--)
    {
        /* SPR 11771 Start */
 /*TddHarqPdu = (FAPI_tddHarqPduIndicationR9_st *)&pHarqMsg->harqPduInfo[iterator++];*/
        /*TDD_FAPI_2.4 changes  start */
        pTddHarqPdu = ( FAPI_tddHarqPduIndicationR9_st *)
            (((UInt8 *)pHarqMsg)
             + harqPduOffset);

        /* + SPR 13928 Changes */
        if (HARQ_BUNDLING_MODE == pTddHarqPdu->mode) /* bundling */
        {
        
            harqPduOffset = harqPduOffset + 12 + sizeof(FAPI_tddBundlingHarqInfoR9_st) * (pTddHarqPdu->release9Param .numOfAckNack);

        }
        else if (HARQ_MULTIPLEXING_MODE == pTddHarqPdu->mode) /* multiplexing */
        {
       
            /* SPR 19679: TDD HARQ Multiplexing Changes Start */
            harqPduOffset = harqPduOffset + 12 + 
                           (sizeof(FAPI_tddMultiplexingHarqInfoR9_st) * 
                           (pTddHarqPdu->release9Param.numOfAckNack));
            /* SPR 19679: TDD HARQ Multiplexing Changes End */			
        }
        else if (HARQ_SPECIAL_BUNDLING_MODE == pTddHarqPdu->mode) /* special bundling */
        {
       
            harqPduOffset += 16;

        }
        else if( HARQ_CHANNEL_SELECTION_MODE == pTddHarqPdu->mode) /* Channel Selection */
        {
            /*SPR 14274 fix start*/
            harqPduOffset = harqPduOffset + 12 +
                        ( sizeof(FAPI_tddChannelSelectionHarqInfo_st) *
                          (pTddHarqPdu->release9Param.numOfAckNack));
            /*SPR 14274 fix end*/
        }
        /* - SPR 13928 Changes */
        else
        {
            ltePanic ("[%s] Invalid Value of mode %d",
                    __func__, pTddHarqPdu->mode);
        }
        /*TDD_FAPI_2.4 changes  End */

        
        
        pTddBundlingPdu = (FAPI_tddBundlingHarqInfoR9_st *)&pTddHarqPdu->release9Param.harqBuffer[0];
        /*SWAP*/

        /*SWAP*/
        /* SPR 11771 End */
        pRntiInfo = getUeIdxFromRNTIMap(MAC_PHY_CONVERT_16(pTddHarqPdu->rnti),internalCellIndex);

        if ( (PNULL == pRntiInfo 
                    || FREERNTI == pRntiInfo->rntiFlag))
        {
            LTE_MAC_UT_LOG( LOG_INFO, 
                    RECV_INTF,
                    "\n[%s]Recv Rnti %d is not Active at Mac Layer\n",
                    __func__, 
                    pTddHarqPdu->rnti);

            continue;
        }
        else if ( CRNTI == pRntiInfo->rntiFlag )
        {
            dlAckNack          = 0;
            ueIndex = pRntiInfo->index;
            if ( dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
            {
                LTE_MAC_UT_LOG( LOG_INFO, RECV_INTF,
                        "\n[%s] Pending Delete Flag True or UE Index %d is not Active\n",
                        __func__, ueIndex);
                continue;
            }
        }
        /* + SPS_TDD_Changes */
        else if ( SPS_RNTI == pRntiInfo->rntiFlag )
        {
            dlAckNack          = 0;
            ueIndex = pRntiInfo->index;
            if ( dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
            {
                LTE_MAC_UT_LOG( LOG_INFO, RECV_INTF,
                        "\n[%s] Pending Delete Flag True or UE Index %d is not Active\n",
                        __func__, ueIndex);
                continue;
            }
        }
        /* - SPS_TDD_Changes */ 

        /* The format of the ACK/NACK response expected. The bundling and
         *  0 = BUNDLING
         *  1 = MULTIPLEXING 
         *  2 = SPECIAL BUNDLING
         *  3=  Channel Selection
         *  4= Format3
         */

        /*TDD special bundling changes start*/
        if (  pTddHarqPdu->mode > MAX_MODE_TDDHARQ_REL9)
            /*TDD special bundling changes end*/
        {
            ltePanic ("[%s] Invalid Value of mode",
                    __func__, pTddHarqPdu->mode);

            LOG_MAC_MSG(MAC_HI_MODE ,LOGDEBUG,MAC_L1_INF,\
                    0,\
                    pTddHarqPdu->rnti,\
                    pTddHarqPdu->mode,\
                    recvTTI,0,0,\
                    0.0,0.0,__func__,"INVALID_MODE_VALUE");

            continue;


        }

        /* The number of ACK/NACK results reported for this UE.
Value: 1 . 4 Format 1a/1b Bundling/MUltiplexing
Value: 1 . 8 Channel Selection
Value: 1 . 20  Format 3
uint8_t     numOfAckNack;*/
        /*TDD special bundling changes start*/
        /* + Coverity_69463 */
        if ( pTddHarqPdu->release9Param.numOfAckNack > (MAX_NUMBER_TDD_ACK_NACK - 1))
        /* - Coverity_69463 */
        {
            LOG_MAC_MSG(MAC_HI_NUM_ACK_NACK ,LOGDEBUG,MAC_L1_INF,\
                    0,\
                    pTddHarqPdu->rnti,\
                    pTddHarqPdu->release9Param.numOfAckNack,\
                    recvTTI,0,0,\
                    0.0,0.0,__func__,"INVALID_NUM_ACK_NACK_VALUE");

            continue;

        }
        /*TDD special bundling changes end*/
        /* Indicates HARQ results.
           Range 1 . 7
           1 = ACK
           2 = NACK
           3 = ACK or NACK
           4 = DTX
           5 = ACK or DTX
           6 = NACK or DTX
           7 = ACK or NACK or DTX    

           uint8_t  value0;
           uint8_t  value1;
         */

        if (TCRNTI != pRntiInfo->rntiFlag)
        {
            /* SPR 19679: TDD HARQ Multiplexing Changes Start */
            pTddBundlingPdu = (FAPI_tddBundlingHarqInfoR9_st *)
                &pTddHarqPdu->release9Param.harqBuffer[0];
            /* SPR 19679: TDD HARQ Multiplexing Changes End */
            if (HARQ_BUNDLING_MODE == pTddHarqPdu->mode)//bundling mode
            {
                /* Instead of using only first two value ACK/ NACK now we are
                 * considering all the values received from FAPI interface
                 * i.e 1-7 */
                /*TDD_FAPI_2.4 changes  START*/ 
              /*SPR 13884 changes start */ 
                if(pTddHarqPdu->release9Param.numOfAckNack == 1)
                {
                    dlAckNack = (pTddBundlingPdu->value0  & 0x07) |
                                 ( (pTddBundlingPdu->value0  & 0x07) << 4);
                }
                else
                {

                pTddBundlingPdu_next = (FAPI_tddBundlingHarqInfoR9_st *)((UInt8 *)pTddBundlingPdu + sizeof(FAPI_tddBundlingHarqInfoR9_st));
                    dlAckNack = (pTddBundlingPdu->value0  & 0x07) |
                                 ( (pTddBundlingPdu_next->value0  & 0x07) << 4);
                }
              /*SPR 13884 changes End */  
                /*TDD_FAPI_2.4 changes  END*/
                /* for CA-TDD handle 4 values */
                tddPutEntryInAckNackQueueForCurConfig_p[internalCellIndex]( ueIndex,
                        dlAckNack,
                        recvTTI,
                        sysFrameNum,
                        subFrameNum,
                        internalCellIndex,
                        /* + CA_TDD_HARQ_CHANGES */
                        pTddHarqPdu->mode
                       /* - CA_TDD_HARQ_CHANGES */
                        );
                /* SPR 19679: TDD HARQ Multiplexing Changes Start */
                if (pTddBundlingPdu->value0 == FAPI_NACK)
                    pTddBundlingPdu->value0 = NACK;
                /* SPR 19679: TDD HARQ Multiplexing Changes End */
            }
            else if(HARQ_MULTIPLEXING_MODE == pTddHarqPdu->mode) //multiplexing mode
            {
                /* SPR 19679: TDD HARQ Multiplexing Changes Start */
                pTddMultiplexingR9Pdu = 
                    (FAPI_tddMultiplexingHarqInfoR9_st *)&pTddHarqPdu->release9Param.harqBuffer[0];
                numOfAckNack = pTddHarqPdu->release9Param.numOfAckNack;
                while(numOfAckNack--)
                {
                    dlAckNack |= 
                        (pTddMultiplexingR9Pdu[numOfAckNack].value0 & 0x07 )\
                        << (4 *  numOfAckNack);
                }
                
                tddPutEntryInAckNackQueueForCurConfig_p[internalCellIndex]( ueIndex,
                        dlAckNack,
                        recvTTI,
                        sysFrameNum,
                        subFrameNum,
                        internalCellIndex,
                        pTddHarqPdu->mode
                        );
                /* SPR 19679: TDD HARQ Multiplexing Changes End */

            }
            else if(HARQ_SPECIAL_BUNDLING_MODE == pTddHarqPdu->mode)
            {
                /* SPR 19679: TDD HARQ Multiplexing Changes Start */
                pTddBundlingPdu = (FAPI_tddBundlingHarqInfoR9_st *)&pTddHarqPdu->release9Param.harqBuffer[0];
                /* SPR 19679: TDD HARQ Multiplexing Changes End */
                /*TDD special bundling changes start*/
                if(tddHarqSpecialbundlingMap_g[pTddHarqPdu->release9Param.numOfAckNack]
                        == pTddBundlingPdu->value0)
                {
                    dlAckNack = FAPI_ACK | FAPI_ACK << 4; /* Considering all ACKs*/
                    /* Response for SCell */
                    dlAckNack |= FAPI_ACK  << 8 | FAPI_ACK  << 12;
                }
                else
                {
                    dlAckNack = FAPI_NACK | FAPI_NACK << 4;/* Considering all NACKs*/
                    /* Response for SCell */
                    dlAckNack |= FAPI_NACK  << 8 | FAPI_NACK  << 12;
                }
                tddPutEntryInAckNackQueueForCurConfig_p[internalCellIndex]( ueIndex,
                        dlAckNack,
                        recvTTI,
                        sysFrameNum,
                        subFrameNum,
                        internalCellIndex,
                        /* + CA_TDD_HARQ_CHANGES */
                        pTddHarqPdu->mode
                        /* - CA_TDD_HARQ_CHANGES */
                        );
                /*TDD special bundling changes end*/

            }
            /* + CA_TDD_HARQ_CHANGES */
            else if (HARQ_CHANNEL_SELECTION_MODE == pTddHarqPdu->mode) /* Channel selection */
            {

                pTddChannelSelectionPdu = 
                   (FAPI_tddChannelSelectionHarqInfo_st *)&pTddHarqPdu->release9Param.harqBuffer[0];

                numOfAckNack = pTddHarqPdu->release9Param.numOfAckNack;

                while(numOfAckNack--)
                {
                    dlAckNack |= 
                        (pTddChannelSelectionPdu[numOfAckNack].value0 & 0x07 )\
                        << (4 * numOfAckNack);
                }

                tddPutEntryInAckNackQueueForCurConfig_p[internalCellIndex]( ueIndex,
                        dlAckNack,
                        recvTTI,
                        sysFrameNum,
                        subFrameNum,
                        internalCellIndex,
                        pTddHarqPdu->mode
                        );

            }     
            /* - CA_TDD_HARQ_CHANGES */
        }

        /*Rel_523_Coverity_10765 Fix start*/
        if (pTddBundlingPdu->value0 == FAPI_NACK)
            pTddBundlingPdu->value0 = NACK;
        /*Rel_523_Coverity_10765 Fix end*/

        if ( TCRNTI == pRntiInfo->rntiFlag )
        {
            ueIndex = pRntiInfo->index;
            /* SPR 4934 Changes*/
            /* SPR 19679: TDD HARQ Multiplexing Changes Start */
            pTddBundlingPdu = (FAPI_tddBundlingHarqInfoR9_st *)
                &pTddHarqPdu->release9Param.harqBuffer[0];
            /* SPR 19679: TDD HARQ Multiplexing Changes End */
            tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].tempCRNTICtx_p;
            if(CONTENTION_SENT == tempUEContext_p->status)
            {
                if (pTddHarqPdu->mode == 0)//bundling mode
                {

                    /* If this is ACK */

                    /*Rel_523_Coverity_10765 Fix start*/
                    if( FAPI_ACK == pTddBundlingPdu->value0)
                        /*Rel_523_Coverity_10765 Fix end*/
                    {
                        ContentionResAckNode *contentionAckQueueNode_p = PNULL;
                        GET_MEM_FROM_POOL(ContentionResAckNode,contentionAckQueueNode_p,
                                sizeof(ContentionResAckNode), PNULL);
                        /*+COVERITY 5.3.0 - 24453*/
                        if ( PNULL != contentionAckQueueNode_p )
                        {
                            contentionAckQueueNode_p->ueIndex = ueIndex;
                            if (pushNode(contentionResolutionMsgAckQ_gp[internalCellIndex],
                                        &contentionAckQueueNode_p->nodeAnchor))
                            {
                                lteWarning("pushNode() failed for "
                                        "contentionResolutionMsgAckQ_g queue");
                                freeMemPool(contentionAckQueueNode_p);
                            }
                            /* SPR 4934 Changes*/
                            else
                            {
                                tempUEContext_p->status = CONTENTION_ACK_RECD;
                            }
                            /* SPR 4934 Changes*/
                        }
                        /*-COVERITY 5.3.0 - 24453*/
                    }
                }
                /* SPR 19679: TDD HARQ Multiplexing Changes Start */
                /* LTE MAC shall decode contention resolution TDD ACK-NACK
                 * according to TDD HARQ bundling feedback only. Refer
                 * specification 36.331 section 9.2.4,
                 * so removing the check for harq mulitplexing mode */
                /* SPR 19679: TDD HARQ Multiplexing Changes End */
            }
            /* SPR 4934 Changes*/
        }
    }
}
#endif
#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : processUlHarqIndR8
 * Inputs         : 1. POINTER TO HARQ Ind message 2. TTI 3. SFN 4. SF
 *                  5. internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for processing UL HARQ indication from
 *                  PHY in case PHY Supports Release8
 *****************************************************************************/
 void processUlHarqIndR8( void* msg_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt16 sysFrameNum, 
        UInt8 subFrameNum,
        /*CA Changes start  */      
        InternalCellIndex internalCellIndex)
/*CA Changes end  */     
{

    /*FAPI2_1changes start*/
    FAPI_harqIndicationR8_st *pHarqMsg = (FAPI_harqIndicationR8_st *)msg_p; 
    /*FAPI2_1changes end*/
    UInt32              iterator = 0;
    UInt32              dlAckNack = 0;
    UInt16              ueIndex = 0;
    FAPI_fddHarqPduIndicationR8_st    *pFddHarqPdu = &pHarqMsg->harqPduInfo[0];
    RNTIInfo                        *pRntiInfo = PNULL;
    /* SPR 4934 Changes*/
    TempCRNTICtx *tempUEContext_p = PNULL;
    /* SPR 4934 Changes*/
#ifdef LOG_PRINT_ENABLED
    UInt16 recvRnti = 0;
#endif        
    for (iterator = MAC_PHY_CONVERT_16(pHarqMsg->numOfHarq); iterator > 0; iterator--)
    {
#ifdef LOG_PRINT_ENABLED
        recvRnti = MAC_PHY_CONVERT_16(pFddHarqPdu->rnti);
#endif        
        pRntiInfo = getUeIdxFromRNTIMap(pFddHarqPdu->rnti,internalCellIndex);
        if ( (PNULL == pRntiInfo || FREERNTI == pRntiInfo->rntiFlag))
        {
            /* SPR#688 Changes Start */ 
            LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"processUlHarqIndR8:Recv Rnti %d is not Active at Mac Layer\n",pFddHarqPdu->rnti);
            /* SPR#688 Changes End */

            /* +- SPR 17777 */
/* SPR 20636 Changes Start */
            LOG_MAC_MSG(MAC_HI_RX_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
                    0,\
                    recvRnti, \
                    (ADDR)pRntiInfo,\
                    (pRntiInfo ? pRntiInfo->rntiFlag : 0), \
                    recvTTI,sysFrameNum,\
                    0.0,0.0,__func__,"HI_RX_RNTIERR");
/* SPR 20636 Changes End */
            /* +- SPR 17777 */

            pFddHarqPdu = (FAPI_fddHarqPduIndicationR8_st *)
                (((UInt8 *)pFddHarqPdu)
                 + sizeof(FAPI_fddHarqPduIndicationR8_st));
            continue;
        }
        else if ( CRNTI == pRntiInfo->rntiFlag )
        {
            ueIndex = pRntiInfo->index;
            if ( dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
            {
                /* SPR#688 Changes Start */ 
                LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"processUlHarqIndR8: Pending Delete Flag True or UE Index %d is not Active",ueIndex);
                /* SPR#688 Changes End */ 

                LOG_MAC_MSG(MAC_HI_RX_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
                        0,\
                        recvRnti,\
                        ueIndex,\
                        recvTTI,0,0,\
                        0.0,0.0,__func__,"HI_RX_UEDELETE");

                pFddHarqPdu = (FAPI_fddHarqPduIndicationR8_st *)
                    (((UInt8 *)pFddHarqPdu)
                     + sizeof(FAPI_fddHarqPduIndicationR8_st));
                continue;
            }
        }
        else if ( SPS_RNTI == pRntiInfo->rntiFlag )
        {
            ueIndex = pRntiInfo->index;
            if ( dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
            {
                LOG_MAC_MSG(MAC_HI_RX_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
                        0,\
                        recvRnti,\
                        ueIndex,\
                        recvTTI,0,0,\
                        0.0,0.0,__func__,"HI_RX_UEDELETE");

                pFddHarqPdu = (FAPI_fddHarqPduIndicationR8_st *)
                    (((UInt8 *)pFddHarqPdu)
                     + sizeof(FAPI_fddHarqPduIndicationR8_st));
                continue;
            }
        }
        else if ( TCRNTI == pRntiInfo->rntiFlag )
        {
            ueIndex = pRntiInfo->index;
            /* SPR 4934 Changes*/
            tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].tempCRNTICtx_p;
            if(CONTENTION_SENT == tempUEContext_p->status)
            {
                /* If this is ACK */
                if(ACK == pFddHarqPdu->release8Param.harqTB1)
                {
                    ContentionResAckNode *contentionAckQueueNode_p = PNULL;
                    GET_MEM_FROM_POOL(ContentionResAckNode,contentionAckQueueNode_p,
                            sizeof(ContentionResAckNode), PNULL);
                    if (PNULL != contentionAckQueueNode_p)
                    {

                        contentionAckQueueNode_p->ueIndex = ueIndex;
                        if (pushNode(contentionResolutionMsgAckQ_gp[internalCellIndex], 
                                    &contentionAckQueueNode_p->nodeAnchor))
                        {
                            lteWarning("pushNode() failed for "
                                    "contentionResolutionMsgAckQ_g queue");

                            LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGFATAL,L2_SYS_FAIL,\
                                    getCurrentTick(),\
                                    __LINE__,0,0,0,0,\
                                    0.0,0.0,__FILE__,__func__);

                            /* SPR 924  Start */ 
                            freeMemPool(contentionAckQueueNode_p);
                            /* SPR 924  End */ 
                        }
                        else
                        {
                            /* SPR 4934 Changes*/
                            tempUEContext_p->status = CONTENTION_ACK_RECD;
                            /* SPR 4934 Changes*/
                            LOG_MAC_MSG(MAC_CONTENTION_ACK_RCVD_ID,LOGDEBUG,MAC_L1_INF,\
                                    0,\
                                    pFddHarqPdu->rnti,\
                                    ueIndex,\
                                    recvTTI,0,0,\
                                    0.0,0.0,__func__,"CONTENTION_ACK_RCVD");
                        }
                    }
                }
            }
            /* SPR 4934 Changes*/
        }
        if (TCRNTI != pRntiInfo->rntiFlag)
        {
            /* Its an CRNTI */
            /* Instead of using only first two value ACK/ NACK now we are
             * considering all the values received from FAPI interface i.e 1-7
             */
            dlAckNack = (pFddHarqPdu->release8Param.harqTB1 & 0x07) |
                ((pFddHarqPdu->release8Param.harqTB2 & 0x07) << 4);


            LOG_UT(MAC_HI_CRNTI_RX_ID,LOGDEBUG,MAC_L1_INF,\
                    0,\
                    ueIndex,\
                    pFddHarqPdu->release8Param.harqTB1,\
                    pFddHarqPdu->release8Param.harqTB2,\
                    subFrameNum,\
                    recvTTI,\
                    0.0,0.0,__func__,"HI_CRNTI_RX");

            putEntryInAckNackQueue(ueIndex,
                    dlAckNack,
                    subFrameNum,
                    pRntiInfo->rntiFlag
                    /* 32.425 Events code changes start*/
                    /* +- SPR 17777 */
#ifndef DL_UL_SPLIT
#ifdef KPI_STATS
                    ,recvTTI
#endif
#endif
                    /* +- SPR 17777 */
                    /* 32.425 Events code changes end*/
                    /*CA Changes start  */
                    ,internalCellIndex
                    /*CA Changes end  */
                    /* + SPR_15971 */
                    ,HARQ_MODE_1A
                    /* - SPR_15971 */

                    );
        }
        pFddHarqPdu = (FAPI_fddHarqPduIndicationR8_st *)(((UInt8 *)pFddHarqPdu)
                + sizeof(FAPI_fddHarqPduIndicationR8_st));
    }

} 
#elif TDD_CONFIG
/*****************************************************************************
 * Function Name  : processUlHarqIndR8
 * Inputs         : 1. HARQ Ind message 2. TTI 3. SFN 4. SF
 *                  5. internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for processing UL HARQ indication from
 *                  PHY in case of TDD with Realease capability Release8
 *****************************************************************************/
 void processUlHarqIndR8( void *msg_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt16 sysFrameNum,
        UInt8 subFrameNum,
        /*CA Changes start  */      
        InternalCellIndex internalCellIndex)
/*CA Changes end  */     

{
    UInt32              dlAckNack = 0;
        /*TDD_FAPI_2.4 changes  START */
    UInt32              harqPduOffset=4;
        /*TDD_FAPI_2.4 changes  END*/
    UInt16              ueIndex = 0;
    FAPI_tddHarqPduIndicationR8_st   *pTddHarqPdu = PNULL;
    FAPI_tddBundlingHarqInfo_st    *pTddBundlingPdu = PNULL;
    RNTIInfo                       *pRntiInfo = PNULL;
    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
    FAPI_tddMultiplexingHarqInfo_st *pTddMultiplexingPdu = PNULL;
    /* SPR 19679: TDD HARQ Multiplexing Changes End */
    /* SPR 4934 Changes*/
    TempCRNTICtx *tempUEContext_p = PNULL;
    /* SPR 4934 Changes*/

    FAPI_harqIndicationR8_st *pHarqMsg=(FAPI_harqIndicationR8_st*)msg_p; 
    UInt16              numOfHarq = MAC_PHY_CONVERT_16(pHarqMsg->numOfHarq);
    /* The RNTI passed to the PHY in an uplink subframe configuration PDU.
Value: 1 . 65535.  */
    /*  uint16_t    rnti;  */
    while (numOfHarq--)
    {
        /*TDD_FAPI_2.4 changes  START */

        pTddHarqPdu = ( FAPI_tddHarqPduIndicationR8_st *)
                (((UInt8 *)pHarqMsg)
                 + harqPduOffset);
        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
        if (HARQ_BUNDLING_MODE == pTddHarqPdu->mode) /* bundling */
        {
            harqPduOffset += 16;
        }
        else if (HARQ_MULTIPLEXING_MODE == pTddHarqPdu->mode) /* multiplexing */
        {
            harqPduOffset += 16;
        }
        else if (HARQ_SPECIAL_BUNDLING_MODE == pTddHarqPdu->mode) /* special bundling */
        {
            harqPduOffset += 16;
        }
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
        else
        {
            ltePanic ("[%s] Invalid Value of mode %d",
                        __func__, pTddHarqPdu->mode);
        }    
        /*TDD_FAPI_2.4 changes  END*/
        pRntiInfo = getUeIdxFromRNTIMap(MAC_PHY_CONVERT_16(pTddHarqPdu->rnti),internalCellIndex);

        if ( (PNULL == pRntiInfo 
                    || FREERNTI == pRntiInfo->rntiFlag))
        {
            LTE_MAC_UT_LOG( LOG_INFO, 
                    RECV_INTF,
                    "\n[%s]Recv Rnti %d is not Active at Mac Layer\n",
                    __func__, 
                    pTddHarqPdu->rnti);

            continue;
        }
        else if ( CRNTI == pRntiInfo->rntiFlag )
        {
            dlAckNack          = 0;
            ueIndex = pRntiInfo->index;
            if ( dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
            {
                LTE_MAC_UT_LOG( LOG_INFO, RECV_INTF,
                        "\n[%s] Pending Delete Flag True or UE Index %d is not Active\n",
                        __func__, ueIndex);
                continue;
            }
        }
        /* + SPS_TDD_Changes */
        else if ( SPS_RNTI == pRntiInfo->rntiFlag )
        {
            dlAckNack          = 0;
            ueIndex = pRntiInfo->index;
            if ( dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
            {
                LTE_MAC_UT_LOG( LOG_INFO, RECV_INTF,
                        "\n[%s] Pending Delete Flag True or UE Index %d is not Active\n",
                        __func__, ueIndex);
                continue;
            }
        }
        /* - SPS_TDD_Changes */ 

        /* The format of the ACK/NACK response expected. The bundling and
         *  0 = BUNDLING
         *  1 = MULTIPLEXING 
         *  2 = SPECIAL BUNDLING
         */

        /*TDD special bundling changes start*/
        if (  pTddHarqPdu->mode > MAX_MODE_TDDHARQ_REL8)
            /*TDD special bundling changes end*/
        {
            LOG_MAC_MSG(MAC_HI_MODE ,LOGDEBUG,MAC_L1_INF,\
                    0,\
                    pTddHarqPdu->rnti,\
                    pTddHarqPdu->mode,\
                    recvTTI,0,0,\
                    0.0,0.0,__func__,"INVALID_MODE_VALUE");
            continue;

        }

        /* The number of ACK/NACK results reported for this UE.
Value: 1 . 4
Value: 0 . 9 (Special Bundling)
uint8_t     numOfAckNack;*/
        /*TDD special bundling changes start*/
        if ( pTddHarqPdu->release8Param.numOfAckNack > MAX_NUMACKNACK_TDDHARQ_REL8)
        {

            LOG_MAC_MSG(MAC_HI_NUM_ACK_NACK ,LOGDEBUG,MAC_L1_INF,\
                    0,\
                    pTddHarqPdu->rnti,\
                    pTddHarqPdu->release8Param.numOfAckNack,\
                    recvTTI,0,0,\
                    0.0,0.0,__func__,"INVALID_NUM_ACK_NACK_VALUE");
            continue;

        }
        /*TDD special bundling changes end*/
        /* Indicates HARQ results.
           Range 1 . 7
           1 = ACK
           2 = NACK
           3 = ACK or NACK
           4 = DTX
           5 = ACK or DTX
           6 = NACK or DTX
           7 = ACK or NACK or DTX    

           uint8_t  value0;
           uint8_t  value1;
         */

        if (TCRNTI != pRntiInfo->rntiFlag)
        {
             /* SPR 19679: TDD HARQ Multiplexing Changes Start */
            if (pTddHarqPdu->mode == HARQ_BUNDLING_MODE)//bundling mode
            {
                pTddBundlingPdu = (FAPI_tddBundlingHarqInfo_st *)&pTddHarqPdu->
                    release8Param.harqBuffer[0]; 
                /* SPR 19679: TDD HARQ Multiplexing Changes End */
                /* Instead of using only first two value ACK/ NACK now we are
                 * considering all the values received from FAPI interface
                 * i.e 1-7 */

                dlAckNack = (pTddBundlingPdu->value0  & 0x07) |
                    ( (pTddBundlingPdu->value1  & 0x07) << 4);

                tddPutEntryInAckNackQueueForCurConfig_p[internalCellIndex]( ueIndex,
                        dlAckNack,
                        recvTTI,
                        sysFrameNum,
                        subFrameNum,
                        internalCellIndex,
                        /* + CA_TDD_HARQ_CHANGES */
                        pTddHarqPdu->mode
                        /* - CA_TDD_HARQ_CHANGES */
                        );
                /* SPR 19679: TDD HARQ Multiplexing Changes Start */
                /*Rel_523_Coverity_10765 Fix start*/
                if (pTddBundlingPdu->value0 == FAPI_NACK)
                    pTddBundlingPdu->value0 = NACK;
                /*Rel_523_Coverity_10765 Fix end*/
                /* SPR 19679: TDD HARQ Multiplexing Changes End */
            }
             /* SPR 19679: TDD HARQ Multiplexing Changes Start */
            else if(pTddHarqPdu->mode == HARQ_MULTIPLEXING_MODE) //multiplexing mode
            {
                pTddMultiplexingPdu = (FAPI_tddMultiplexingHarqInfo_st *)
                    &pTddHarqPdu->release8Param.harqBuffer[0];
                dlAckNack = ((pTddMultiplexingPdu->value0 & 0x07)|
                        ((pTddMultiplexingPdu->value1 & 0x07) << 4)|
                        ((pTddMultiplexingPdu->value2 & 0x07) << 8)|
                        ((pTddMultiplexingPdu->value3 & 0x07) << 12));

                    tddPutEntryInAckNackQueueForCurConfig_p[internalCellIndex]( ueIndex,
                            dlAckNack,
                            recvTTI,
                            sysFrameNum,
                            subFrameNum,
                            internalCellIndex,
                            /* + CA_TDD_HARQ_CHANGES */
                            pTddHarqPdu->mode
                            /* - CA_TDD_HARQ_CHANGES */
                            );
            }
            else if(pTddHarqPdu->mode == HARQ_SPECIAL_BUNDLING_MODE)
            {
                pTddBundlingPdu = (FAPI_tddBundlingHarqInfo_st *)&pTddHarqPdu->
                    release8Param.harqBuffer[0]; 
                /* SPR 19679: TDD HARQ Multiplexing Changes End */
                /*TDD special bundling changes start*/
                if(tddHarqSpecialbundlingMap_g[pTddHarqPdu->release8Param.numOfAckNack] == pTddBundlingPdu->value0)
                {
                    dlAckNack = FAPI_ACK | FAPI_ACK << 4; /* Considering all ACKs*/
                }
                else
                {
                    dlAckNack = FAPI_NACK | FAPI_NACK << 4;/* Considering all NACKs*/
                }
                tddPutEntryInAckNackQueueForCurConfig_p[internalCellIndex]( ueIndex,
                        dlAckNack,
                        recvTTI,
                        sysFrameNum,
                        subFrameNum,
                        internalCellIndex,
                        /* + CA_TDD_HARQ_CHANGES */
                        pTddHarqPdu->mode
                        /* - CA_TDD_HARQ_CHANGES */
                        );
                /*TDD special bundling changes end*/

            }
        }
        if ( TCRNTI == pRntiInfo->rntiFlag )
        {
            ueIndex = pRntiInfo->index;
            /* SPR 4934 Changes*/
            tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].tempCRNTICtx_p;
            if(CONTENTION_SENT == tempUEContext_p->status)
            {
                 /* SPR 19679: TDD HARQ Multiplexing Changes Start */
                if (pTddHarqPdu->mode == HARQ_BUNDLING_MODE)//bundling mode
                {
                    pTddBundlingPdu = (FAPI_tddBundlingHarqInfo_st *)&pTddHarqPdu->
                        release8Param.harqBuffer[0]; 
                     /* SPR 19679: TDD HARQ Multiplexing Changes End */
                    /* If this is ACK */

                    /*Rel_523_Coverity_10765 Fix start*/
                    if( FAPI_ACK == pTddBundlingPdu->value0)
                        /*Rel_523_Coverity_10765 Fix end*/
                    {
                        ContentionResAckNode *contentionAckQueueNode_p = PNULL;
                        GET_MEM_FROM_POOL(ContentionResAckNode,contentionAckQueueNode_p,
                                sizeof(ContentionResAckNode), PNULL);
                        /*+COVERITY 5.3.0 - 24453*/
                        if ( PNULL != contentionAckQueueNode_p )
                        {
                            contentionAckQueueNode_p->ueIndex = ueIndex;
                            if (pushNode(contentionResolutionMsgAckQ_gp[internalCellIndex],
                                        &contentionAckQueueNode_p->nodeAnchor))
                            {
                                lteWarning("pushNode() failed for "
                                        "contentionResolutionMsgAckQ_g queue");
                                freeMemPool(contentionAckQueueNode_p);
                            }
                            /* SPR 4934 Changes*/
                            else
                            {
                                tempUEContext_p->status = CONTENTION_ACK_RECD;
                            }
                            /* SPR 4934 Changes*/
                        }
                        /*-COVERITY 5.3.0 - 24453*/
                    }
                }
                /* SPR 19679: TDD HARQ Multiplexing Changes Start */
                /* LTE MAC shall decode contention resolution TDD ACK-NACK
                 * according to TDD HARQ bundling feedback only. Refer
                 * specification 36.331 section 9.2.4,
                 * so removing the check for harq mulitplexing mode */
                /* SPR 19679: TDD HARQ Multiplexing Changes End */
            }
            /* SPR 4934 Changes*/
        }
    }
}
#endif
#endif                
/*****************************************************************************
 * Function Name  : processULCrcInd 
 * Inputs         : crcMsg_p - pointer to FAPI_crcIndication_st,
 *                  recvTTI, currentTick, subFrameNum and sysFrameNum
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for processing UL CRC indication from
 *                  PHY.
 *****************************************************************************/

STATIC  void processULCrcInd(FAPI_crcIndication_st *crcMsg_p, 
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        tickType_t currentTick
        /* SPR 15909 fix end */
        /* msg3_retx_tdd_support_start */
        ,UInt32  subFrameNum 
        ,UInt32  sysFrameNum
        /* msg3_retx_tdd_support_end */
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex 
        /*CA Changes end  */
        )
{
    UInt16                      ueIndex = 0;
    UInt32                      iterator = 0;
    UInt32                      tcrntiFlag = FALSE;
    UInt32                      crcFlag = 0;
    UInt16                      recvRnti = 0;
    UInt32                      ulContainerIndex = 0;
    RNTIInfo                    *rntiInfo_p = PNULL;
    FAPI_crcPduIndication_st    *crcPdu_p = &crcMsg_p->crcPduInfo[0];
#ifdef FDD_CONFIG    
#ifdef ULHARQ_TIMER_PROC
    /*QOS_KLOCKWORK_FIX*/
    UInt8 timerExpsubFrameNum = subFrameNum;
    /*QOS_KLOCKWORK_FIX*/
#endif
    /*Container Index*/
    ulContainerIndex = recvTTI % MAX_CONTAINER_NUM;
#elif TDD_CONFIG
/* msg3_retx_tdd_support_start */
/*QOS_KLOCKWORK_FIX*/
    UInt8 timerExpsubFrameNum = subFrameNum;
/*QOS_KLOCKWORK_FIX*/
/* msg3_retx_tdd_support_end */
    /* SPR 15909 fix start */
    tickType_t phichTTI = ( recvTTI + getHiPduSubframeNum(subFrameNum, internalCellIndex)); /* MEAS_GAP_CHG */
    /* SPR 15909 fix end */
    ulContainerIndex = phichTTI % getMaxULCrcContainer(internalCellIndex);
#endif

    /*SPR_3305_FIX_START*/
    TempCRNTICtx *tempCRNTICtx_p = PNULL;
    /*SPR_3305_FIX_END*/

    /* FORCED_ACK SPR#9393 Changes Start */
    ULUEContextInfo* ulUEContextInfo_p = PNULL;
    ULHarqInfo* ulHarqProcess_p = PNULL;
    UInt8 ulHarqProcessId       = 0;
    /* FORCED_ACK SPR#9393 Changes End */
    /* SPR 11509 Fix Start */
    UInt8 isForcedAckDueToMaxRetx = LTE_FALSE;
    /* SPR 11509 Fix End */
    
    for (iterator = MAC_PHY_CONVERT_16(crcMsg_p->numOfCrc); iterator > 0; iterator--)
    {
        tcrntiFlag = FALSE;
        recvRnti =  MAC_PHY_CONVERT_16(crcPdu_p->rnti); 
        /*CA Changes start  */
        rntiInfo_p = getUeIdxFromRNTIMap(recvRnti, internalCellIndex);
        /*CA Changes end  */
        if ( (PNULL == rntiInfo_p || FREERNTI == rntiInfo_p->rntiFlag))
        {
            /* SPR#688 Changes Start */ 
            LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"processULCrcInd: Recv Rnti %d is not Active at Mac Layer\n",crcPdu_p->rnti);
            /* SPR#688 Changes End */
/* SPR 20636 Changes Start */
            LOG_MAC_MSG(MAC_CRC_RX_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
                    0,\
                    recvRnti,\
                    (ADDR)rntiInfo_p,\
                    (rntiInfo_p ? rntiInfo_p->rntiFlag : 0), \
                    recvTTI,0,\
                    0.0,0.0,__func__,"CRC_RX_RNTIERR");

/* SPR 20636 Changes End */
            /* Process next CRC PDU */
            crcPdu_p = (FAPI_crcPduIndication_st *)
                (((UInt8 *)crcPdu_p)
                 + sizeof(FAPI_crcPduIndication_st));
            continue;
        }
        else if ( ( CRNTI == rntiInfo_p->rntiFlag ) || 
                ( SPS_RNTI == rntiInfo_p->rntiFlag ) )
        {
            ueIndex = rntiInfo_p->index;
            /* SPR 4807 Changes Start */
            if ( TRUE == dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag ) 
                /* SPR 5317 Changes Start */
                /*(FALSE ==
                 * dlUECtxInfoArr_g[ueIndex].dlUEContext_p->tcrntiFlag ))*/
                /* SPR 4807 Changes End */
            {
                if (!dlUECtxInfoArr_g[ueIndex].dlUEContext_p || 
                        FALSE == dlUECtxInfoArr_g[ueIndex].dlUEContext_p->tcrntiFlag ) 
                {
                    /* SPR#688 Changes Start */ 
                    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"processULCrcInd: Pending Delete Flag True or UE Index %d is not Active\n",ueIndex);
                    /* SPR#688 Changes End */
                    /* +- SPR 17777 */
                    LOG_MAC_MSG(MAC_CRC_RX_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
                            0,\
                            recvRnti,\
                            ueIndex,\
                            recvTTI,currentTick,0,\
                            0.0,0.0,__func__,"CRC_RX_UEDELETE");
                    /* +- SPR 17777 */

                    /* Process next CRC PDU */
                    crcPdu_p = (FAPI_crcPduIndication_st *)
                        (((UInt8 *)crcPdu_p)
                         + sizeof(FAPI_crcPduIndication_st));
                    continue;
                }
                /* SPR 5317 Changes End */
            }
        }
        else if ( TCRNTI == rntiInfo_p->rntiFlag )
        {
            ueIndex = rntiInfo_p->index;
            tcrntiFlag = TRUE; 
        }

        /*SPR_3305_FIX_START*/
        if (tcrntiFlag)
        {
            tempCRNTICtx_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].\
                             tempCRNTICtx_p;
            if (PNULL != tempCRNTICtx_p)
            {
/* msg3_retx_tdd_support_start */
                /*CA Changes start  */
                ulHarqTimerForMsg3Stop(tempCRNTICtx_p, timerExpsubFrameNum,sysFrameNum, internalCellIndex);
                /*CA Changes end  */
/* msg3_retx_tdd_support_end */

            }
            else
            {
                ltePanic("tempContext is NULL\n");
            }
            crcFlag = crcPdu_p->crcFlag;
/* msg3_retx_tdd_support_start */
            /*CA Changes start  */

            crcAckNackHandlingForTcrnti(crcFlag, ueIndex, recvTTI ,ulContainerIndex, internalCellIndex);
            /*CA Changes end  */
/* msg3_retx_tdd_support_end */
            /*SPR_5155_Changes*/
            /*Next CRC PDU Access code has been removed*/
            /*SPR_5155_End*/
        }

        else
        {
            /*SPR_3305_FIX_END*/
            crcFlag = crcPdu_p->crcFlag; 
#ifdef FLEXRAN
            if (crcFlag != 0)
            {
                numOfCrcNack_g[internalCellIndex]++;
            }
            numOfCrc_g[internalCellIndex]++;
#endif

     /* FORCED_ACK SPR#9393 Changes Start */
	ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
    ULUEContext *ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
	if( PNULL == ulUEContext_p )
        {
		LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] MAC_FAILURE"
                        "UECONTEXT NULL FOR UEINDEX %d CTTI %d\n",__func__,ueIndex,currentTick);
		continue;
	}

#ifdef FDD_CONFIG    
            /* MEAS_GAP_CHG */

         UInt8 tempId = MODULO_SIXTEEN(recvTTI);
         ulHarqProcessId = ulUEContextInfo_p->ulUEContext_p->ttiHarqMap[tempId];
         ulHarqProcess_p = &ulUEContextInfo_p->ulUEContext_p->ulresInfo[ulHarqProcessId];
	/* FORCED_ACK SPR#9393 Changes End */

#ifdef ULHARQ_TIMER_PROC
            /* ulUEContextInfo_p   = &ulUECtxInfoArr_g[ueIndex]; */ /*
                                                                       MEAS_GAP_CHG */

            LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"CTTI %d SUB FRAME %d"
                    "EXPIRY SUBFRAME %d ACK/NACK %d\n",currentTick,
                    /*SPR 1257 changes start*/
                    subFrameNum,timerExpsubFrameNum,crcFlag);
            /*SPR 1257 changes end*/

            LOG_UT(MAC_CRC_CRNTI_RX_ID,LOGDEBUG,MAC_L1_INF,\
                    0,\
                    recvRnti,\
                    ueIndex,\
                    crcFlag,\
                    timerExpsubFrameNum,recvTTI,\
                    0.0,0.0,__func__,"CRC_RX_CRNTI");

            if (PNULL != ulUEContextInfo_p->ulUEContext_p) 
            {

                /* TTIB_Code Start */                                

                if(ulUEContextInfo_p->ulUEContext_p->ulresInfo[ulHarqProcessId].isTTIBundled)
                {

                    /* SPR 15909 fix start */
                    tickType_t ttiBundleFirstTick= (ulUEContextInfo_p->ulUEContext_p->ulresInfo[ulHarqProcessId].ttiBundleFirstTick);
                    /* SPR 15909 fix end */

                    LTE_MAC_UT_LOG(LOG_DETAIL,RECV_INTF,"[%s] ulHarqProcessId [%d] ttiBundleFirstTick [%d] isTTIDataRcvd [%d]\n"
                            ,__func__,ulHarqProcessId,ttiBundleFirstTick,isTTIDataRcvd);
                    if(recvTTI == (ttiBundleFirstTick + TTIB_FIRST_TO_LAST_TICK_OFFSET))
                    {
                        if(MAC_FAILURE == (ulHarqTimerStop(ulUEContextInfo_p->ulUEContext_p,
                                /* +- SPR 17777 */
                                subFrameNum,sysFrameNum,
                                /* +- SPR 17777 */
                                /*CA Changes start  */
                                internalCellIndex)))
			{ 
				LOG_MAC_MSG( MAC_UL_HARQ_TIMER_STOP_FAIL, LOGWARNING, MAC_UL_HARQ,
               			getCurrentTick(), __LINE__, ulUEContextInfo_p->ulUEContext_p->ueIndex, recvTTI,
               			subFrameNum, ulHarqProcessId, 0,0, __func__, "");
			}
                        /*CA Changes end  */
                    }
                    else
                    {
                        LOG_MAC_MSG( MAC_RECV_ERR_FOR_TTIB_DATA, LOGDEBUG, 
                                MAC_L1_INF, getCurrentTick(), __LINE__,
                                ueIndex, ulHarqProcessId, recvTTI, 
                                ttiBundleFirstTick, 0,0, __func__, "");
                        /* Process next CRC PDU */
                        crcPdu_p = (FAPI_crcPduIndication_st *)
                            (((UInt8 *)crcPdu_p)
                             + sizeof(FAPI_crcPduIndication_st));

                        /*++ SPR 19156 */
                        /* Taking local backup of all neccesory parameters which will be
                        * used later when it receive ulSchInd for this CRC Ack */
                        harqParamBackupForUlSchInd( &ulUEContextInfo_p->ulUEContext_p->ulresInfo[
                            ulHarqProcessId]);
                             
                        ulUEContextInfo_p->ulUEContext_p->ttiPUSCHHarq[MODULO_SIXTEEN(recvTTI)] = 
                            ulHarqProcessId;
                        /*-- SPR 19156*/
                        continue;
                    }
                }
                else
                {    
                        if(MAC_FAILURE == (ulHarqTimerStop(ulUEContextInfo_p->ulUEContext_p,
                                /* +- SPR 17777 */
                                subFrameNum,sysFrameNum,
                                /* +- SPR 17777 */
                                /*CA Changes start  */
                                internalCellIndex)))
			{ 
				LOG_MAC_MSG( MAC_UL_HARQ_TIMER_STOP_FAIL, LOGWARNING, MAC_UL_HARQ,
               			getCurrentTick(), __LINE__, ulUEContextInfo_p->ulUEContext_p->ueIndex, recvTTI,
               			subFrameNum, ulHarqProcessId, 0,0, __func__, "");
			}
                        /*CA Changes end  */
                }
                /* TTIB_Code End */                                
            }
            else
            {
                LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] MAC_FAILURE"
                        "UECONTEXT NULL FOR UEINDEX %d CTTI %d\n",__func__,ueIndex,currentTick);
                LOG_MAC_MSG(MAC_CRC_RX_UECONTEXT_NULL_ID,LOGDEBUG,MAC_L1_INF,\
                        0,\
                        recvRnti,\
                        ueIndex,\
                        crcFlag,\
                        timerExpsubFrameNum,recvTTI,\
                        0.0,0.0,__func__,"CRC_RX_UECONTEXT_NULL");
            }
#endif

            /* FORCED_ACK SPR#9393 Changes Start */
            if (ulHarqProcess_p->currentRetransCount >= (ulUEContextInfo_p->ulUEContext_p->maxUlHARQTx-1))
            {
                /* SPR 11509 Fix Start */
                if(crcFlag == 1)
                {
                    isForcedAckDueToMaxRetx = LTE_TRUE;
                }
                /* SPR 11509 Fix End */
                crcFlag = 0; /*0=CRC_OK*/
            }
            /* FORCED_ACK SPR#9393 Changes End */

            /* SPR 11509 Fix Start */
            /*CA Changes start  */
            crcAckNackHandling(crcFlag, ueIndex, recvTTI ,ulContainerIndex, rntiInfo_p->rntiFlag, internalCellIndex,isForcedAckDueToMaxRetx);	
            /* SPR 11509 Fix End */
#else
            /* FORCED_ACK SPR#9393 Changes Start */
            ulHarqProcessId = getULHarqIDFromSubFrame(subFrameNum, ulUEContextInfo_p->ulUEContext_p, internalCellIndex,sysFrameNum);

            if (ulHarqProcessId != 255)
            {
                ulHarqProcess_p = &ulUEContextInfo_p->ulUEContext_p->ulresInfo[ulHarqProcessId];
                if (ulHarqProcess_p->currentRetransCount >= (ulUEContextInfo_p->ulUEContext_p->maxUlHARQTx -1))
                {
                    /* SPR 11509 Fix Start */
                    if(crcFlag == 1)
                    {
                        isForcedAckDueToMaxRetx = LTE_TRUE;
                    }
                    /* SPR 11509 Fix End */
                    crcFlag = 0; /*0 = CRC_OK*/
                }
                /* FORCED_ACK SPR#9393 Changes End */

                /* SPR 11509 Fix Start */
                crcAckNackHandling(crcFlag, ueIndex, recvTTI ,ulContainerIndex, rntiInfo_p->rntiFlag, internalCellIndex, isForcedAckDueToMaxRetx);	
                /* SPR 11509 Fix End */
                /*CA Changes end  */
            }
#endif
        }
        /* Process next CRC PDU */
        crcPdu_p = (FAPI_crcPduIndication_st *)(((UInt8 *)crcPdu_p)
                + sizeof(FAPI_crcPduIndication_st));
    }
    return;
}
/*****************************************************************************
 * Function Name  : processULErrInd 
 * Inputs         : errMsg_p - pointer to structure FAPI_phyErrorIndication_st
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for processing UL Err indication from
 *                  PHY.
 *****************************************************************************/
/*CA Changes start  */
/*CA Changes start  */
STATIC  void processULErrInd(FAPI_phyErrorIndication_st *errMsg_p,
    InternalCellIndex internalCellIndex )
    /*CA Changes end  */
{
    UInt8 mapIdx = 0;
    UInt8 harqRttTimerFactor = 0;
    UInt16 totalTickMissErrIndDl = 0;
    UInt16  sysFrameNum = 0;
    UInt16  subFrameNum = 0;
    UInt16 recvdSysFrameNum = 0;
    UInt16 recvdSubFrameNum = 0;
    /* SPR 15909 fix start */
    tickType_t currentTick = getCurrentTick();
    /* SPR 15909 fix end */
    FAPI_errMsgBody1_st   errMsgBody1 = {0};
    FAPI_errMsgBody2_st   errMsgBody2 = {0};
#ifndef LOG_PRINT_ENABLED
#endif

#ifdef LOG_PRINT_ENABLED
    FAPI_errMsgBody3_st   errMsgBody3 = {0};
    FAPI_errMsgBody4_st   errMsgBody4 = {0};
#endif

    LOG_MAC_MSG( MAC_RECV_ERR_IND_FRM_PHY, LOGERROR, MAC_L1_INF,
            getCurrentTick(), __LINE__, errMsg_p->msgId, errMsg_p->errorCode,
            0,0, 0,0, __func__,"");

    switch(errMsg_p->errorCode)
    {
        case FAPI_MSG_INVALID_STATE:
        {
            /* Notify OAM for Error Indication received from L1 
             * OAM may restart the cell */
            sendMacPhyOamErrIndication( MAC_PHY_OAM_ERR_MSG_INVALID_STATE, 
                    internalCellIndex );
            break;
        }
        case FAPI_SFN_OUT_OF_SYNC:
        {
            ErrIndPowerControlType pcType = ERR_IND_TPC_FOR_PUCCH;
            /* FAPI_SFN_OUT_OF_SYNC will be received in case of SFN mismatch 
             * in DL Config */
            errMsgBody1 = errMsg_p->errMsgBody.msgBody1;
            /* Received  SFN/SF in error message */

            recvdSysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF(
                    errMsgBody1.recvdSfnSf); 
            recvdSubFrameNum = GET_SUBFRAME_FROM_SFNSF(
                    errMsgBody1.recvdSfnSf);   

            /* expected SFN/SF in error message */
            sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF(
                    errMsgBody1.expectedSfnSf); 
            subFrameNum = GET_SUBFRAME_FROM_SFNSF(
                    errMsgBody1.expectedSfnSf);
            /*CA Changes start  */
            countPhyErrorIndication_g[internalCellIndex]++ ;
            /*CA Changes end  */

#ifdef OVERSHOOT_CORRECTION
            /*TDD Tick overshoot changes start*/
            currTickOverLoad_g[internalCellIndex] = OVERLOAD_LEVEL_CRITICAL;
            /*TDD Tick overshoot changes end*/
#endif
            LOG_MAC_MSG(MAC_PHY_ERR_SFNSF_MSG_ID,LOGWARNING,MAC_L1_INF,
                    getCurrentTick(),
                    countPhyErrorIndication_g[internalCellIndex], 
                    recvdSysFrameNum,recvdSubFrameNum,
                    sysFrameNum,subFrameNum,
                    0.0,0.0,__func__,"ERR_SFNSF_MSG");
            /* Assuming Recvd TTI is old and expected is new that is not 
             * processed */
            /* SPR 15909 fix start */
            tickType_t expTTI = (sysFrameNum * MAX_SUB_FRAME) + subFrameNum;
            tickType_t recvTTI = (recvdSysFrameNum * MAX_SUB_FRAME) + 
                recvdSubFrameNum;
            tickType_t maxTTI  = ( MAX_SYS_FRAME_NUM * MAX_SUB_FRAME) + 
                (MAX_SUB_FRAME - 1);
            /* SPR 15909 fix end */

            /* Calculate the differnece between expected and received SFN/SF */
            if( recvTTI < expTTI)
            {
                totalTickMissErrIndDl_g[internalCellIndex] = expTTI - recvTTI;
            }
            else
            {
                /* wrap around case */
                totalTickMissErrIndDl_g[internalCellIndex] = 
                    ((maxTTI - recvTTI) + expTTI) + 1;
            }

            /* Copy the Tick Miss information to local variable to send it 
             * later to OAM as it will be reset by EL called in TDD */
            totalTickMissErrIndDl = totalTickMissErrIndDl_g[internalCellIndex];

            /* MAC will not update its internal TTI due to this error and will
             * only rely upon SubFrame.Indication to align its internal TTI */
#ifdef TDD_CONFIG
            harqRttTimerFactor = tddHarqRttTimerFactor( 
                    /* SPR 12198 fix start */
                    recvdSubFrameNum, internalCellIndex );
                    /* SPR 12198 fix end */

            LOG_MAC_MSG( MAC_ON_SFN_MISMATCH, LOGDEBUG, MAC_L1_INF,
                    getCurrentTick(), __LINE__, 
                    totalTickMissErrIndDl_g[internalCellIndex], 
                    harqRttTimerFactor, numErrIndDlConfig_g[internalCellIndex],
                    0, 0,0, __func__, "DL");
            if( totalTickMissErrIndDl_g[internalCellIndex] >= harqRttTimerFactor )
            {
                UInt8 airSf = subFrameNum;
                UInt16 airSfn = sysFrameNum;
                ExecutionLegFunctionArg execLegFuncArg = {0};
                if( MAC_SUCCESS == calculateHarqExpTickMissAndArgs(
                            &execLegFuncArg, ELARG_DL_HARQ_EXPIRY_U_SF, 
                            ( currentTick - PHY_ERROR_IND_DELAY_DL ) +
                            totalTickMissErrIndDl_g[internalCellIndex], 
                            airSf, airSfn, 
                            totalTickMissErrIndDl_g[internalCellIndex],
                            /* SPR 11110 fix start */
                            ERROR_SCENARIO ) )
                            /* SPR 11110 fix end */
                {
                    executionFunctionArrTdd_g[PROCESS_HARQ_TIMER_EXPIRY](
                            &execLegFuncArg);
                }
            }
            /* SPR 11230 fix start */
            else
            {
                /* Even if the error is there in one DL CONFIG, NACK will be
                 * considered for all the packets corresponding to all D/S 
                 * subframes and re-transmitions shall be performed for all
                 * D/S subframes. */
                /* For handling of re-transmissions in TDD */
                /* SPR 12261 fix start */
                handleDlConfigErrForReTx( getCurrentTick(), 
                        &errMsgBody2, errMsg_p->errorCode, internalCellIndex );
                /* SPR 12261 fix end */
            }
            /* SPR 11230 fix end */
#elif FDD_CONFIG
            harqRttTimerFactor = MAX_NUM_OF_TICK_MISSED;
            LOG_MAC_MSG( MAC_ON_SFN_MISMATCH, LOGDEBUG, MAC_L1_INF,
                    getCurrentTick(), __LINE__, 
                    totalTickMissErrIndDl_g[internalCellIndex], 
                    harqRttTimerFactor, numErrIndDlConfig_g[internalCellIndex],
                    0, 0,0, __func__, "DL");
#endif 

            /* Notify OAM for Error Indication received from L1 */ 
            if( harqRttTimerFactor <= totalTickMissErrIndDl )
            {
                if( 0 == numErrIndDlConfig_g[internalCellIndex] )
                {
                    /* Set the number of error indications that can be received 
                     * for DL Config so that notifications to OAM is not 
                     * flooded */
                    numErrIndDlConfig_g[internalCellIndex] = totalTickMissErrIndDl;

                    sendMacPhyOamErrIndication( 
                            MAC_PHY_OAM_ERR_SFN_OUT_OF_SYNC_BY_MORE_THAN_THRESHOLD, 
                            internalCellIndex );
                }
            }
            else
            {
                /* Reset the variables as HARQ recovery will be done 
                 * automatically once timer gets expired */
                totalTickMissErrIndDl_g[internalCellIndex] = 0;
            }

            /* Calculate Map Index */
            mapIdx = recvTTI % MAX_TPC_CONTAINER_MAP_SIZE; 
            /* Revert the TPC if TPC command sent */
            /* +- SPR 17777 */
            updateTpcForPhyErrorInd( pcType, mapIdx,
                    internalCellIndex);
            /* +- SPR 17777 */

            break;
        }
        case FAPI_MSG_INVALID_SFN:
        {
            /* FAPI_MSG_INVALID_SFN will be received in case of SFN mismatch
             * in HI_DCI_0 or TX_REQUEST */
            errMsgBody1 = errMsg_p->errMsgBody.msgBody1;

            countPhyErrorIndication_g[internalCellIndex]++ ;
#ifdef OVERSHOOT_CORRECTION
            currTickOverLoad_g[internalCellIndex] = OVERLOAD_LEVEL_CRITICAL;
#endif 
            /* Check for message Id for which FAPI_MSG_INVALID_SFN is 
             * received */
            switch(  errMsg_p->msgId  )
            {
                case PHY_DL_HI_DCI0_REQUEST:
                {
                    /* Handles the MSG_INVALID_SFN Error Indication recieved 
                     * from phy when HI_DCI0.request has been sent to Phy */
                    handleFapiMsgInvalidSfnForHiDci0( &errMsgBody1, 
                            currentTick,internalCellIndex); 
                    break;
                }
                case PHY_DL_TX_REQUEST:
                {
                    /* No specific recovery actions will be taken as for 
                       - BCH recovery is not required, 
                       - PCH will be automatically recovered by paging 
                         repetition and 
                       - user DLSCH data will be recovered via retransmission 
                         when HARQ DTX is received */
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;

        } 

        case FAPI_MSG_HI_ERR:
        {
#ifdef LOG_PRINT_ENABLED
            errMsgBody3 = errMsg_p->errMsgBody.msgBody3;
#endif

            countPhyErrorIndication_g[internalCellIndex]++ ;
            LOG_MAC_MSG(MAC_PHY_ERR_HI_MSG_ID, LOGERROR, MAC_L1_INF,
                    getCurrentTick(), 
                    countPhyErrorIndication_g[internalCellIndex], 
                    errMsgBody3.subErrCode, errMsgBody3.phichLowestulRbIndex,
                    0,0, 0,0, __func__,"ERR_HI_MSG");

            /* Send message to OAM about the error */
            sendMacPhyOamErrIndication( MAC_PHY_OAM_ERR_MSG_HI_ERR, 
                    internalCellIndex );
            break;
        }
        case FAPI_MSG_TX_ERR:
        {
            /* No specific recovery actions will be taken as for 
               - BCH recovery is not required, 
               - PCH will be automatically recovered by paging repetition and 
               - user DLSCH data will be recovered via retransmission when 
               HARQ DTX is received */
#ifdef LOG_PRINT_ENABLED
            errMsgBody4 = errMsg_p->errMsgBody.msgBody4;
#endif
            countPhyErrorIndication_g[internalCellIndex]++ ;
            LOG_MAC_MSG(MAC_PHY_ERR_MSG_TX_ID, LOGERROR, MAC_L1_INF,
                    getCurrentTick(), __LINE__, 
                    countPhyErrorIndication_g[internalCellIndex], 
                    errMsgBody4.subErrCode, errMsgBody4.pduIndex, 0,
                    0,0, __func__, "MSG_TX_ERR");
            break;
        }
        case FAPI_MSG_SUBFRAME_ERR:
        {
            errMsgBody2 = errMsg_p->errMsgBody.msgBody2;

            countPhyErrorIndication_g[internalCellIndex]++ ;
            LOG_MAC_MSG(MAC_PHY_ERR_SUBFRAME_ID, LOGERROR, MAC_L1_INF,
                    getCurrentTick(), 
                    countPhyErrorIndication_g[internalCellIndex], 
                    errMsgBody2.subErrCode, errMsgBody2.direction, 
                    errMsgBody2.rnti, errMsgBody2.pduType, 0,0, 
                    __func__,"MSG_SUBFRAME_ERR");

            /* Check for message Id for which FAPI_MSG_SUBFRAME_ERR is received */
            switch(  errMsg_p->msgId  )
            {
                case PHY_DL_CONFIG_REQUEST:
                {
                    /* DL HARQ timer expiry functionality will perform the DL 
                     * HARQ recovery. If power control commands had been sent 
                     * to UE(s) in the DL_Config.Request for which 
                     * Error.Indication has been received, then MAC will update
                     * the total power correction sent to the UE as if the 
                     * latest TPC was not sent in DL_Config.Request. 
                     * No recovery will be performed for other PDUs. */
                    /* +- SPR 17777 */
                    handleFapiMsgSubFrameErrForDlConfig( currentTick,
                            /* SPR 12261 fix start */
#ifdef TDD_CONFIG
                            &errMsgBody2,
                            errMsg_p->errorCode,
#endif
                            internalCellIndex);
                    /* +- SPR 17777 */
                            /* SPR 12261 fix end */
                    break;
                }
                case PHY_UL_CONFIG_REQUEST:
                {
                    handleFapiMsgSubFrameErrForUlConfig( &errMsgBody2,currentTick ,internalCellIndex);
                    break;
                }
            }
            break;
        }
        /*TDD Tick overshoot changes start*/
        case FAPI_MSG_BCH_MISSING:
        {
            countPhyErrorIndication_g[internalCellIndex]++ ;
#ifdef OVERSHOOT_CORRECTION
            currTickOverLoad_g[internalCellIndex] = OVERLOAD_LEVEL_CRITICAL;
#endif
            LOG_MAC_MSG(MAC_PHY_ERR_BCH_MISSING_ID, LOGERROR, MAC_L1_INF, 
                    getCurrentTick(), __LINE__, countPhyErrorIndication_g[internalCellIndex],
                    0,0,0, 0,0, __func__,"MSG_BCH_MISSING");
            /* Notify OAM for Error Indication received from L1 
             * OAM may restart the cell */
            sendMacPhyOamErrIndication( MAC_PHY_OAM_ERR_MSG_BCH_MISSING, 
                    internalCellIndex );
            break;
        }
        /*TDD Tick overshoot changes end*/
        default:
        {
            /* Nothing to be done */
            break;
        }
    }
}

#ifndef DISABLE_UL_CQI_PROCESSING
/******************************************************************************
 * Function name : putULSCHEntryInSINRReportQueue 
 * Inputs        : ueIndex: Will be used to obtain the grant information,
 *                 recvTTI: Will be used to obtain the grant information,
 *                 ulCqi: Receieved SINR in UL,
 *                 isULNackReceived : ULNackReceived flag,
 *                 currentTTI: Current global tick
                   internalCellIndex - Cell-Index at MAC
 * Outputs       : Whether node was pushed in queue or not
 * Returns       : MAC_SUCCESS/MAC_FAILURE
 * Description   : This function will enqueue the received SINR information 
 *                 which will be processed by the execution leg - 
 *                 ElProcessSINRReports.
 *****************************************************************************/
/* SPR 2437 Start */
STATIC MacRetType putULSCHEntryInSINRReportQueue(UInt16 ueIndex,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt8 ulCqi,
        UInt8 isULNackReceived,
        /* SPR 15909 fix start */
        /* +- SPR 17777 */
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
        tickType_t currentTTI,
#endif
        /* +- SPR 17777 */
        /* SPR 15909 fix end */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
/* SPR 2437 End */
{
    ULUEContext *ulUEContext_p = PNULL;
    ULHarqInfo*   tempULHarqInfo_p = PNULL;
    UInt8 harqProcId = 0;
    /* ++ SPR 20612 */
#ifdef FDD_CONFIG
    UInt8 tempHarqId = 0;
#endif
    /* -- SPR 20612 */
    SINRReportNode *sinrReportNode_p = NULL;
#ifdef TDD_CONFIG    
    /* TDD Config 6 Changes Start */
    UInt8 subframeNum  = 0;
    UInt16 sysFrameNum = 0;
    GET_SFN_SF_FROM_TTI(recvTTI, sysFrameNum, subframeNum)
    /* TDD Config 6 Changes End */
#endif

    ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
    /* Get the Harq information to obtain the grant information (startPRB and
     * numOfPRB) corresponding to which this ULSCH indication has been 
     * received.
     */
     /* ++ SPR 20612 */
#ifdef FDD_CONFIG
    tempHarqId = MODULO_SIXTEEN(recvTTI);
    /* If ULSCH Ind receive before CRC
     * indication then ulHarqProcessId
     * will not be equal to
     * TTI_MAP_FREE*/
    harqProcId = ulUEContext_p->ttiHarqMap[tempHarqId];
    if(TTI_MAP_FREE == harqProcId)
    {
        harqProcId = ulUEContext_p->ttiPUSCHHarq[tempHarqId];
    }
    tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[harqProcId]);

    if( ( tempULHarqInfo_p != PNULL ) && 
            ( TRUE == tempULHarqInfo_p->ulSchReqInfo.isTTIBundled ) &&
            ( TRUE == tempULHarqInfo_p->ulSchReqInfo.isTTIDataRcvd))
    {
        /*No need to enqueue SINR report */
        return MAC_FAILURE;
    }

    /* -- SPR 20612 */
#elif TDD_CONFIG
    /* TDD Config 6 Changes Start */
    harqProcId = getULHarqIDFromSubFrame(subframeNum, ulUEContext_p, internalCellIndex, sysFrameNum);
    /* TDD Config 6 Changes End */
    /* SPR 2437 Start */
    if(INVALID_HARQ == harqProcId)
    {
        return MAC_FAILURE;
    }
    /* SPR 2437 End */
    /* ++ SPR 20612 */
    tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[harqProcId]);
    /* -- SPR 20612 */
#endif
    /* SPR 2437 Start */
    GET_MEM_NODE_SINR_REPORT_Q(sinrReportNode_p,SINRReportNode);

    /* Rel 5.3: Coverity 24457 Fix Start */
    if (PNULL == sinrReportNode_p)
    {
        /* Not able to allocate Memory */
        return MAC_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */

    /* SPR 2437 End */
    sinrReportNode_p->ueIndex      = ueIndex;
    /* SPR 3531 START */
    if (isULNackReceived == TRUE)
    {
        sinrReportNode_p->reportType   = SINR_REPORT_TYPE_ULSCH_NACK;
    }
    else
    {
        sinrReportNode_p->reportType   = SINR_REPORT_TYPE_ULSCH;
    }
    /* SPR 3531 END */
    sinrReportNode_p->startPRB     = tempULHarqInfo_p->riStart;
    sinrReportNode_p->numOfPRB     = tempULHarqInfo_p->riLength;
    /* Time Averaging changes start */
    sinrReportNode_p->recvTTI  = recvTTI;
    /* Time Averaging changes end */
    sinrReportNode_p->sinrArray[0] = ulCqi;
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
    /*CA Changes start  */
    if(!(ENQUEUE_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex] + ((currentTTI + MAC_TICK_UPDATED) % SINR_CONTAINER_SIZE),
                    SINRReportNode,(void *)&(sinrReportNode_p->nodeAnchor))))
        /*CA Changes end  */
    {
        /* +- SPR 17777 */
        LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                getCurrentTick(),\
                __LINE__,currentTTI,0,0,0,\
                0.0,0.0,__FILE__,__func__);
        /* +- SPR 17777 */

        FREE_MEM_NODE_SINR_REPORT_Q(sinrReportNode_p);
        return MAC_FAILURE;   
    } 
#else
    /*CA Changes start  */
    if(!(ENQUEUE_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex],
                    SINRReportNode, (void *)&(sinrReportNode_p->nodeAnchor))))
        /*CA Changes end  */
    {
        LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                getCurrentTick(),\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);

        FREE_MEM_NODE_SINR_REPORT_Q(sinrReportNode_p);
        return MAC_FAILURE;   
    }    
#endif
    LOG_UT(MAC_UL_CSI_PUT_ULSCH_SINR_IN_QUEUE_ID, LOGDEBUG, MAC_UL_CSI, \
            currentTTI,\
            ueIndex, SINR_REPORT_TYPE_ULSCH, \
            sinrReportNode_p->startPRB, sinrReportNode_p->numOfPRB,\
            sinrReportNode_p->sinrArray[0], \
            (UDouble32)harqProcId, 0, \
            __func__,"PUT_ULSCH_SINR_IN_QUEUE");

    /* SPR 2437 Start */
    return MAC_SUCCESS;
    /* SPR 2437 End */
}
#endif /*DISABLE_UL_CQI_PROCESSING*/
#ifdef FAPI_4_0_COMPLIANCE
/*****************************************************************************
 * Function Name  : fillTimingAdvanceR9 
 * Inputs         : UL Sch message
 *                  rntiInfo_p
 *                  validReports_p
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for filling TA R9 from UL Data indication from
 *                  PHY
 *****************************************************************************/
void fillTimingAdvanceR9(FAPI_ulDataPduIndicationR9_st *ulschPdu_p,
        RNTIInfo *rntiInfo_p,
        UInt8 *validReports_p ,
        InternalCellIndex internalCellIndex)

{

    UInt16 timingAdvanceR9 = INVALID_TIMING_ADVANCE_R9_VALUE;
    UInt16  ueIndex       = 0;
    TempCRNTICtx *tempUEContext_p = PNULL;
    ULUEContext *ulUEContext_p = PNULL;
    EcidReportParams *eCidReportParams_p = PNULL;

    if((PNULL != rntiInfo_p ) && (PNULL !=ulschPdu_p) )
    {

        timingAdvanceR9 = (MAX_TIMING_ADVANCE_R9_VALUE >=MAC_PHY_CONVERT_16(ulschPdu_p->release9Param.timingAdvanceR9) )?\
                          MAC_PHY_CONVERT_16(ulschPdu_p->release9Param.timingAdvanceR9):INVALID_TIMING_ADVANCE_R9_VALUE;

        *validReports_p |= (MAX_TIMING_ADVANCE_R9_VALUE >= timingAdvanceR9)? TRUE:FALSE;

        if(TCRNTI == rntiInfo_p->rntiFlag)
        {

            tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->\
                              tempCRNTICtxArray[rntiInfo_p->index].tempCRNTICtx_p;
            if(PNULL != tempUEContext_p)
            {
                tempUEContext_p->validReports = *validReports_p;
                tempUEContext_p->type1TAValue = timingAdvanceR9;
            }

        }
        else
        {
            ueIndex=rntiInfo_p->index;
            ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
            if((TRUE != ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag) && \
                    (PNULL != ulUEContext_p))
            {
                eCidReportParams_p = &ulUEContext_p->eCidReportParams;
                eCidReportParams_p->type1TAValue = timingAdvanceR9;
            }


        }

    }

}
/*****************************************************************************
 * Function Name  : processULSchIndR9 
 * Inputs         : 1. UL Sch message 2. Pointer to zero copy buffer 3. msg len
 *                  4. TTI 5. Demux flag 6. dl Delay flag 7. Thread index
 *                  8. internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for processing UL Data indication from
 *                  PHY in case PHY Supports Release9 or later
 *****************************************************************************/
/* + TM7_8 Changes Start */
/* + TM7_8 Changes Start */
 void processULSchIndR9(void *VendorSpecificParam_p,
        void *buffer,
        void *zeroBufCpy_p,
        UInt32 msgLen,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt32 withDemux,
        UInt32 dlDelay,
        UInt32 threadIndex,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */

        )
/* - TM7_8 Changes End */
{
    UInt16  recvRnti      = 0;
    UInt16  ueIndex       = 0;
    /*  UInt16  tcRnti        = 0; TDD Warning Fix */
    UInt16  iterator     = 0;
    UInt32  dataLen       = 0;
    UInt32  tcrntiFlag   = 0;
    RNTIInfo *rntiInfo_p = PNULL;
    FAPI_ulDataPduIndicationR9_st *ulDataInfo_p = PNULL;
    /* TA_CHG */
    UInt16  timingAdvance = 0; /* SPR 2628 Changes */
    /* +CLPC_CHG_LJA */
    SInt8 powerControlDeltaTFAndLogFactor = 0;
    /* -CLPC_CHG_LJA */
    UInt8 riLength = 0;
    UInt8 ulHarqProcessId = 0;
    UInt8 validReports = 0;
    /* +- SPR 18490 */
#ifndef DISABLE_UL_CQI_PROCESSING
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
    tickType_t currentTTI = getCurrentTickUl();
    /* SPR 15909 fix end */
#endif
#endif
    /* +- SPR 18490 */
    /*CLPC_CHG_RILEN*/
    FAPI_VendorSpecificR9_st *vendorSpecific_p=(FAPI_VendorSpecificR9_st*)VendorSpecificParam_p;
    FAPI_rxULSCHIndicationR9_st *ulSchMsg_p = (FAPI_rxULSCHIndicationR9_st *)buffer;
    UInt16  numOfPdu     =MAC_PHY_CONVERT_16(ulSchMsg_p->numOfPdu);
#ifndef DISABLE_UL_CQI_PROCESSING
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD)) 
    /*CA Changes start  */
    UInt32 sinrReportQueueCount = COUNT_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex] + (currentTTI % SINR_CONTAINER_SIZE));
#else
    UInt32 sinrReportQueueCount = COUNT_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex]);
    /*CA Changes end  */
#endif
#endif  /*DISABLE_UL_CQI_PROCESSING*/ 
    TempCRNTICtx *tempUEContext_p = PNULL;
    /* SPR 2437 Start*/
    /* SPR 1864 Start */
#ifdef TDD_CONFIG
    /* SPR 1864 End */
    /* TDD Config 6 Changes Start */
    UInt8 subframeNum  = 0;
    UInt16 sysFrameNum = 0;
    GET_SFN_SF_FROM_TTI(recvTTI, sysFrameNum, subframeNum)
    /* TDD Config 6 Changes End */
    /* SPR 1864 Start */
#endif

    ULHarqInfo *harqProcess_p = PNULL;

    /* SPR 1864 End */
    ULUEContextInfo* ulUEContextInfo_p = PNULL;
    /* SPR 2437 End */
    /* + TM7_8 Changes Start */
    /* - TM7_8 Changes End */

    /* TA_CHG */
/* SPR 19288 change start */
    void *demuxDataZeroBuf_p = PNULL;
    void *demuxDataZeroBufTmp_p = PNULL;
    UInt32 splitOffset = ULSCH_IND_START_OF_PDU_OFFSET + 
        ( numOfPdu * sizeof(FAPI_ulDataPduIndicationR9_st));
    SPLIT_ZERO_BUFFER_COPY(zeroBufCpy_p, demuxDataZeroBuf_p, splitOffset);
/* SPR 19288 change end */
    for (iterator = 0; iterator < numOfPdu; iterator++)
    {
#ifdef FDD_CONFIG
        UInt8 ttiBundleFlag = FALSE;
#endif
        tcrntiFlag = FALSE;
        ulDataInfo_p = (FAPI_ulDataPduIndicationR9_st*)
            &ulSchMsg_p->ulDataPduInfo[iterator]; 
        recvRnti = MAC_PHY_CONVERT_16(ulDataInfo_p->rnti);
        /* getUeIndexFromRNTIMap returns NULL if not configured RARNTI or 
         * CRNTI Range and rntiFlag == FREERNTI is  deactive.
         */
        rntiInfo_p = getUeIdxFromRNTIMap(recvRnti,internalCellIndex);
        if ( PNULL == rntiInfo_p || FREERNTI == rntiInfo_p->rntiFlag )
        {
            lteWarning("processULSchIndR9:Recv Rnti %d is not Active at Mac Layer\n",recvRnti);

            /* +- SPR 17777 */
/* SPR 20636 Changes Start */
            LOG_MAC_MSG(MAC_ULSCH_RX_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
                    0,\
                    recvRnti,\
                    (ADDR)rntiInfo_p,\
                    (rntiInfo_p ? rntiInfo_p->rntiFlag : 0), \
                    recvTTI,msgLen,\
                    0.0,0.0,__func__,"ULSCH_RX_RNTIERR");
/* SPR 20636 Changes End */
            /* +- SPR 17777 */
            /* SPR 11612 Changes Start */
            /*
            ** Before freeing Msg Buffer, copy data buffer of next pdu to new
            ** Msg Buffer by using SPLIT_ZERO_BUFFER_COPY.
            **
            ** Else,  Working on freed Msg Buffer on next for() loop cycle
            */
            dataLen = MAC_PHY_CONVERT_16(ulDataInfo_p->length);
            if (dataLen)
            {                
/* SPR 19288 change start */
                splitOffset = dataLen;
                SPLIT_ZERO_BUFFER_COPY(demuxDataZeroBuf_p, demuxDataZeroBufTmp_p, splitOffset);

                /*coverity 25191,25192 fix*/
                msgFree(demuxDataZeroBuf_p);
                /*coverity 25191,25192 fix*/
                demuxDataZeroBuf_p = demuxDataZeroBufTmp_p;
/* SPR 19288 change end */
			}
            /* SPR 9526 Fix */ 
            continue ;
            /* SPR 9526 Fix */
        }

        /* Throughput correction */
        if (statsInfo_g[internalCellIndex].ulThroughPutEnable)
        {
            statsInfo_g[internalCellIndex].totalUplinkBytesSend +=  MULTIPLY_BY_EIGHT( 
                    MAC_PHY_CONVERT_16(ulDataInfo_p->length));
        }
        else
        {
            enableUplinkThroughput(1, internalCellIndex);
            statsInfo_g[internalCellIndex].totalUplinkBytesSend += MULTIPLY_BY_EIGHT(
                    MAC_PHY_CONVERT_16(ulDataInfo_p->length));
        }	
        /* Throughput correction */

        if ( TCRNTI == rntiInfo_p->rntiFlag )
        {
            /*tcRnti = rntiInfo_p->index; TDD Warning Fix */
            tcrntiFlag = TRUE;
            /*Power Control Change*/
            tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[rntiInfo_p->index].tempCRNTICtx_p;
            /*check if msg3 is received or contention is already sent*/
            if (tempUEContext_p->status == (MSG3_RECEIVED | CONTENTION_SENT))
            { 
                /* SPR 11612 Changes Start */
                /*
                ** Before freeing Msg Buffer, copy data buffer of next pdu to new
                ** Msg Buffer by using SPLIT_ZERO_BUFFER_COPY and then free Msg Buffer
                ** before continue to loop.
                */
                dataLen = MAC_PHY_CONVERT_16(ulDataInfo_p->length);
/* SPR 19288 change start */
                splitOffset = dataLen;
                SPLIT_ZERO_BUFFER_COPY(demuxDataZeroBuf_p, demuxDataZeroBufTmp_p, splitOffset);
                msgFree(demuxDataZeroBuf_p);
                demuxDataZeroBuf_p = demuxDataZeroBufTmp_p;
                /* SPR 11612 Changes End */
/* SPR 19288 change end */
                continue;
            }
            tempUEContext_p->msg3BuffInfo.sinr  = ulDataInfo_p->ulCqi;

            /* SPR 2333 Changes Begin */ 
            if((!ulDataInfo_p->dataOffset) || (!ulDataInfo_p->length)) 
            {
                harqProcess_p = &( tempUEContext_p->ulHarq );
                /* Need to perform adaptive re-transmission as it is the case of
                 * DTX */
                if( PNULL != harqProcess_p )
                {
                    if( MIN_SINR_FOR_TPC_CALC_IN_NACK >= ulDataInfo_p->ulCqi )
                    {
                        harqProcess_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                        /* set newDataIndicator to 0 so that adaptive retx can
                         * happen in NACK Queue processing at PDCCH */
                        harqProcess_p->newDataIndicator = 0;
                    }
                }

                continue;
            }
            /* SPR 2333 Changes Ends */ 
        }
        else 
        {   

            /* + Coverity_24790 */
#ifdef PERF_STATS
            /* Update the Counters for the total successfully transferred data
             * volume on MAC level in the Uplink */
            gMacUePerfStats_p[rntiInfo_p->index].lteUeMacULPerfStats.\
                totalUlBytesReceived += MAC_PHY_CONVERT_16(ulDataInfo_p->length);
#endif

            /* - Coverity_24790 */
            ueIndex = rntiInfo_p->index;/*SPR 695 OPT FIX*/
            /* SPR 2437 Start */
            ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];

            /* + COVERITY 5.0 */
            if ( ulUEContextInfo_p == PNULL)
            {
                /* SPR 11612 Changes Start */
                /*
                ** Before freeing Msg Buffer, copy data buffer of next pdu to new
                ** Msg Buffer by using SPLIT_ZERO_BUFFER_COPY and then free Msg Buffer
                ** before continue to loop.
                */
                dataLen = MAC_PHY_CONVERT_16(ulDataInfo_p->length);
/* SPR 19288 change start */
                splitOffset = dataLen;
                SPLIT_ZERO_BUFFER_COPY(demuxDataZeroBuf_p, demuxDataZeroBufTmp_p, splitOffset);
                msgFree(demuxDataZeroBuf_p);
                demuxDataZeroBuf_p = demuxDataZeroBufTmp_p;
/* SPR 19288 change end */
                continue;
                /* SPR 11612 Changes End */
            }
            /* - COVERITY 5.0 */
            /* SPR 2437 End */

            /* SPR 4807 Changes Start */
            if ( TRUE == dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag ) 
                /* SPR 5317 Changes Start */
                /*(FALSE ==
                 * dlUECtxInfoArr_g[ueIndex].dlUEContext_p->tcrntiFlag ))*/
                /* SPR 4807 Changes End */
            {
                if (!dlUECtxInfoArr_g[ueIndex].dlUEContext_p || 
                        FALSE == dlUECtxInfoArr_g[ueIndex].dlUEContext_p->tcrntiFlag )
                {
                    lteWarning(" Pending Delete Flag True for UE Index %d ",
                            ueIndex);

                    LOG_MAC_MSG(MAC_ULSCH_RX_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
                            0,\
                            recvRnti,\
                            ueIndex,\
                            recvTTI,0,0,\
                            0.0,0.0,__func__,"ULSCH_RX_UEDELETE");
                    /* SPR 11612 Changes Start */
                    /*
                    ** Before freeing Msg Buffer, copy data buffer of next pdu to new
                    ** Msg Buffer by using SPLIT_ZERO_BUFFER_COPY and then free Msg Buffer
                    ** before continue to loop.
                    */
                    dataLen = MAC_PHY_CONVERT_16(ulDataInfo_p->length);
/* SPR 19288 change start */
                    splitOffset = dataLen;
                    SPLIT_ZERO_BUFFER_COPY(demuxDataZeroBuf_p, demuxDataZeroBufTmp_p, splitOffset);
                    msgFree(demuxDataZeroBuf_p);
                    demuxDataZeroBuf_p = demuxDataZeroBufTmp_p;
/* SPR 19288 change end */
                    /* SPR 11612 Changes End */
                    continue;
                }
                /* SPR 5317 Changes End */

            }
#ifndef DISABLE_UL_CQI_PROCESSING
            /* ULA_CHG */
            /* To prevent overloading of sinrReportQueue_g, not more than
             * MAC_MAX_SINR_REPORTS_QUEUE_LENGTH nodes should be present in
             * the queue at any time.
             */           
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
            if (sinrReportQueueCount < MAC_MAX_SINR_REPORT_QUEUE_LENGTH_SPLIT)
#else
            /*coverity fix  59961 <NESTING_INDENT_MISMATCH> starts */
            if (sinrReportQueueCount < MAC_MAX_SINR_REPORTS_QUEUE_LENGTH)
#endif
            {
                /* This function will enqueue the received SINR information 
                 * which will be processed by the execution leg - 
                 * ElProcessSINRReports
                 */
                /* SPR 2728 Start */
                /*We should ignore SINR value if dataOffset is 0 in ULSCH PDU*/
                if((!ulDataInfo_p->dataOffset)||(!ulDataInfo_p->length))
                {
                    // SPR 3531 START
                    /* NACK CASE  */
                    /* +- SPR 17777 */
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
                    if(MAC_SUCCESS == putULSCHEntryInSINRReportQueue(ueIndex, \
                                /* Time Averaging changes start */
                                recvTTI, \
                                ulDataInfo_p->ulCqi,\
                                TRUE,\
                                currentTTI,
                                /*CA Changes start  */
                                internalCellIndex))
#else
                    if(MAC_SUCCESS == putULSCHEntryInSINRReportQueue(ueIndex, \
                                /* Time Averaging changes start */
                                recvTTI, \
                                ulDataInfo_p->ulCqi,\
                                TRUE,\
                                /*CA Changes start  */
                                internalCellIndex))
#endif
                        /* +- SPR 17777 */
                        /*CA Changes end  */
                        /// SPR 3531 END
                    {
                        sinrReportQueueCount++;
                    }
                }
                else
                {
                    /* SPR 2728 End */
                    /* SPR 2437 Start */
                    /* +- SPR 17777 */
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
                    if(MAC_SUCCESS == putULSCHEntryInSINRReportQueue(ueIndex, \
                                /* Time Averaging changes start */
                                recvTTI, \
                                ulDataInfo_p->ulCqi,\
                                FALSE,\
                                currentTTI,
                                /*CA Changes start  */
                                internalCellIndex))
#else
                    if(MAC_SUCCESS == putULSCHEntryInSINRReportQueue(ueIndex, \
                                /* Time Averaging changes start */
                                recvTTI, \
                                ulDataInfo_p->ulCqi,\
                                FALSE,\
                                /*CA Changes start  */
                                internalCellIndex))
#endif
                        /* +- SPR 17777 */
                        /*CA Changes end  */
                        /* Time Averaging changes end */
                    {
                        sinrReportQueueCount++;
                    }
                    /* SPR 2437 End */
                    /* SPR 2728 Start */
                }
                /* SPR 2728 End */
            }
            else
            {
                LOG_MAC_MSG(MAC_L1_INF_SINR_QUEUE_FULL_ID, LOGWARNING, MAC_L1_INF,\
                        0, \
                        ueIndex, recvTTI, 0, 0, 0, \
                        0, 0, \
                        __func__,"SINR_QUEUE_FULL");
            }
            /*coverity fix  59961 <NESTING_INDENT_MISMATCH> ends */
#endif /*DISABLE_UL_CQI_PROCESSING*/           
            /*CLPC_CHG_RILEN*/
            /* +COVERITY 10735 */
            if(ulUEContextInfo_p->ulUEContext_p)
            {
                harqProcess_p = &(ulUEContextInfo_p->ulUEContext_p->ulresInfo[ulHarqProcessId]);
                /* COVERITY 10735 */
#ifdef FDD_CONFIG
                /* TTIB_Code Start */

                UInt8 tempId = MODULO_SIXTEEN(recvTTI);
                /* If ULSCH Ind receive before CRC
                * indication then ulHarqProcessId
                * will not be equal to
                * TTI_MAP_FREE*/
                /*++ SPR 19181 */
                ulHarqProcessId = ulUEContextInfo_p->ulUEContext_p->ttiHarqMap[tempId];
                if(TTI_MAP_FREE == ulHarqProcessId)
                {
                    /*-- SPR 19181 */
                    /** SPR 12364 fix Starts**/
                    ulHarqProcessId = ulUEContextInfo_p->ulUEContext_p->ttiPUSCHHarq[tempId];
                    /** SPR 12364 fix Ends **/
                }
                harqProcess_p = &(ulUEContextInfo_p->ulUEContext_p->ulresInfo[ulHarqProcessId]);

                /** SPR 12364 fix Starts**/
                if( ( harqProcess_p != PNULL ) && 
                    ( TRUE == harqProcess_p->ulSchReqInfo.isTTIBundled ) )
                    /** SPR 12364 fix Ends **/
                {
                    ttiBundleFlag = TRUE;
                }  
                /** SPR 12364 fix Starts**/
                riLength =  harqProcess_p->ulSchReqInfo.riLength; 
                /** SPR 12364 fix Ends **/

#elif TDD_CONFIG
                /* TDD Config 6 Changes Start */
                ulHarqProcessId = getULHarqIDFromSubFrame(subframeNum, ulUEContextInfo_p->ulUEContext_p, internalCellIndex, sysFrameNum);
                /** SPR 14758 Changes Start **/
                if(ulHarqProcessId == INVALID_HARQ)
                {
                    dataLen = MAC_PHY_CONVERT_16(ulDataInfo_p->length);
                    splitOffset = dataLen;
                    SPLIT_ZERO_BUFFER_COPY(demuxDataZeroBuf_p, demuxDataZeroBufTmp_p, splitOffset);
                    msgFree(demuxDataZeroBuf_p);
                    demuxDataZeroBuf_p = demuxDataZeroBufTmp_p;
                    continue;
                }
                /** SPR 14758 Changes End **/
                harqProcess_p = &(ulUEContextInfo_p->ulUEContext_p->ulresInfo[ulHarqProcessId]);
                /* TDD Config 6 Changes End */
                /*klockwork warning fix*/ 
                if((harqProcess_p != PNULL) && (ulHarqProcessId != INVALID_HARQ))
                {
                    riLength =  harqProcess_p->riLength;
                }
                else
                {
                    riLength = 0;
                }
#endif
                /*CLPC_CHG_RILEN*/
                /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                if( harqProcess_p->ulSchReqInfo.hdFddUlNackHandlingFlag == TRUE)   
                    /** SPR 12364 fix Ends **/
                {
                    LOG_MAC_MSG(MAC_HD_FDD_DISCARD_DATA, LOGWARNING, MAC_L1_INF,\
                            getCurrentTick(), \
                            ulUEContextInfo_p->ulUEContext_p->ueIndex,ulHarqProcessId,\
                            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE, \
                            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE, \
                            FUNCTION_NAME,"DISCARD_DATA");
                    /* SPR 11612 Changes Start */
                    /*
                    ** Before freeing Msg Buffer, copy data buffer of next pdu to new
                    ** Msg Buffer by using SPLIT_ZERO_BUFFER_COPY and then free Msg Buffer
                    ** before continue to loop.
                    */
                    dataLen = MAC_PHY_CONVERT_16(ulDataInfo_p->length);
                    splitOffset = dataLen;
                    SPLIT_ZERO_BUFFER_COPY(demuxDataZeroBuf_p, demuxDataZeroBufTmp_p, splitOffset);
                    msgFree(demuxDataZeroBuf_p);
                    demuxDataZeroBuf_p = demuxDataZeroBufTmp_p;
                    /* SPR 11612 Changes End */

                    continue;
                }
#endif
                /*HD FDD Changes End*/

                /* ULA_CHG */            
                /* SPR 2437 Start */
#ifdef TDD_CONFIG
                /* SPR 2509 CHG start */
                /* SPR 4560 Changes */
                /* CRC_ULSCH_IND SPR 8722 Fix Start */
                /*coverity 56644*/
                /* SPR_10082_FIX_START */
                /* SPR 11509 Fix Start */
                if(((ulHarqProcessId != INVALID_HARQ) && (ulDataInfo_p->dataOffset) && (ulDataInfo_p->length))
                    || (harqProcess_p->currentRetransCount >= (ulUEContextInfo_p->ulUEContext_p->maxUlHARQTx -1))) 
                {
                    /* SPR_10082_FIX_END */
                    harqProcess_p->crcUlschIndFlag |= ULSCH_IND_RCVD_FLAG;
                }
                /*coverity 56644*/
                if((CRC_ULSCH_IND_RCVD_FLAG == harqProcess_p->crcUlschIndFlag))
                        /* CRC_ULSCH_IND SPR 8722 Fix End */
                {
                    resetHarqId( subframeNum,ulUEContextInfo_p->ulUEContext_p,internalCellIndex, sysFrameNum);
                }
                /* SPR 4560 Changes */

#endif
                /* SPR 2437 End */          
                /*dlDelay and null pointer check?*/
                /* Time Averaging changes start */
                powerControlDeltaTFAndLogFactor = ulUEContextInfo_p->ulUEContext_p->powerControlDeltaTFAndLogFactor[recvTTI % MAX_SUB_FRAME]; 
                /* Time Averaging changes end */
                /* -CLPC_CHG_LJA */
                /* +COVERITY 10735 */
#ifdef PERF_STATS
                UPDATE_UE_PUSCH_SINR_STATS(ueIndex, ulDataInfo_p->ulCqi);
#endif
            }
            /* -COVERITY 10735 */
        }
        dataLen = MAC_PHY_CONVERT_16(ulDataInfo_p->length);
        totalMacUllinkBytesSend_g[internalCellIndex] += MULTIPLY_BY_EIGHT(dataLen);
        /*SPR_3061_FIX*/
        if((!ulDataInfo_p->dataOffset)||(!dataLen))
        {
            /* Need to perform adaptive re-transmission as it is the case of
             * DTX */
            if( PNULL != harqProcess_p )
            {
                if( MIN_SINR_FOR_TPC_CALC_IN_NACK >= ulDataInfo_p->ulCqi )
                {
                    harqProcess_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                    /* set newDataIndicator to 0 so that adaptive retx can
                     * happen in NACK Queue processing at PDCCH */
                    harqProcess_p->newDataIndicator = 0;
                }
            }

            continue;
        }
        /*SPR_3061_FIX*/

        /* TA_CHG */
        if (TRUE != tcrntiFlag)
        {
            DLUEContext *newUEcontext_p = PNULL;
            newUEcontext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
            /* SPR 2628 Changes Start */
            timingAdvance = MAC_PHY_CONVERT_16(ulDataInfo_p->timingAdvance);

            handleTimingAdvance(newUEcontext_p, ueIndex, timingAdvance,internalCellIndex, getCurrentTickUl());

            LOG_UT(MAC_TA_INSERTION,LOGDEBUG,MAC_L1_INF,\
                    0,\
                    getCurrentTick(),newUEcontext_p->lastSyncDetectedTick,\
                    ueIndex,timingAdvance,\
                    DEFAULT_INT_VALUE,\
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                    FUNCTION_NAME,"LAST_SYNC_TICK");

            newUEcontext_p->lastSyncDetectedTick = getCurrentTick();
        }
        /* SPR 2333 Changes Begin */
        if((!ulDataInfo_p->dataOffset)||(!dataLen))
        {
            continue;
        }
        /* SPR 2333 Changes Ends */ 
        fillTimingAdvanceR9(ulDataInfo_p,rntiInfo_p,&validReports,internalCellIndex);

        /* TA_CHG */

        splitOffset = dataLen;
        SPLIT_ZERO_BUFFER_COPY(demuxDataZeroBuf_p, demuxDataZeroBufTmp_p, splitOffset);
        LOG_UT(MAC_ULSCH_RX_DATA_ID,LOGDEBUG,MAC_L1_INF,\
                0,\
                ueIndex,recvRnti,\
                recvTTI,dataLen,withDemux,\
                (UDouble32)threadIndex,(UDouble32)dlDelay,__func__,"ULSCH_RX_DATA");

#ifdef FDD_CONFIG
        /* TTIB_Code Start */
        if(TRUE == ttiBundleFlag)
        {
            /** SPR 12364 fix Starts**/
            if( TRUE == ulUEContextInfo_p->ulUEContext_p->
                    ulresInfo[ulHarqProcessId].ulSchReqInfo.isTTIBundled &&
                    FALSE == ulUEContextInfo_p->ulUEContext_p->
                    ulresInfo[ulHarqProcessId].ulSchReqInfo.isTTIDataRcvd )
            {
                ulUEContextInfo_p->ulUEContext_p->ulresInfo[ulHarqProcessId].
                    ulSchReqInfo.isTTIDataRcvd = TRUE;
                /** SPR 12364 fix Ends **/
                /*++ SPR 19156 */
                ulUEContextInfo_p->ulUEContext_p->ulresInfo[ulHarqProcessId].isTTIDataRcvd = TRUE;
                /*--SPR 19156 */                
            }
            else
            {
                msgFree(demuxDataZeroBuf_p);
                demuxDataZeroBuf_p = demuxDataZeroBufTmp_p;
                continue;
            }
        }
#endif


        if (TRUE == withDemux )
        { 
            /* SPR 4751 changes */
            /* +- SPR 17777 */
            processDemuxData(ueIndex, recvRnti, recvTTI, demuxDataZeroBuf_p,
                    dataLen, powerControlDeltaTFAndLogFactor, threadIndex, dlDelay, 0,internalCellIndex);
            /* +- SPR 17777 */
            /* SPR 4751 changes */
            msgFree(demuxDataZeroBuf_p);
        }
        else
        {
            /* +CLPC_CHG_LJA */
            /* SPR 4751 changes */
            putDataInDemuxQueue(ueIndex, recvRnti, recvTTI, demuxDataZeroBuf_p,
                    dataLen,powerControlDeltaTFAndLogFactor, riLength, ulHarqProcessId,internalCellIndex);
            /* SPR 4751 changes */
            /* -CLPC_CHG_LJA */
        }
        demuxDataZeroBuf_p = demuxDataZeroBufTmp_p;
    }

    handleUlschIndVendorSpecificInfoR9(vendorSpecific_p,&validReports,recvTTI,internalCellIndex);
    msgFree(demuxDataZeroBuf_p);
    return;
}
/*****************************************************************************
 * Function Name  : handleUlschVendorSpecificInfoR9 
 * Inputs         : vendorSpecific_p-pointer to Vendor specific Parameters Info
 *                  validReports_p-pointer to valid reports
 *                  recvTTI
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for processing AOA report
 *****************************************************************************/
void handleUlschIndVendorSpecificInfoR9(FAPI_VendorSpecificR9_st *vendorSpecific_p, 
        UInt8 *validReports_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{

    /* +- SPR 18268 */
    UInt16 numOfUEs = 0;
    /* +- SPR 18268 */
    EcidReportParams *eCidReportParams_p = PNULL;
    UInt16 *angleOfArrival = PNULL;
    UInt16 rnti = 0;
    UInt16  ueIndex       = 0;
    /* +- SPR 18268 */
    UInt16 ueCount=0;
    /* +- SPR 18268 */
    UInt8 count = 0;
    FAPI_UEMeasurementIndicationR9_st *ueMeasInfoForUEs_p = PNULL;
    RNTIInfo *rntiInfo_p = PNULL;
    TempCRNTICtx *tempUEContext_p = PNULL;
    ULUEContext *ulUEContext_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
    FAPI_AOA_RecievedForUE_st *aoaRel9Info = PNULL;

    if(PNULL != vendorSpecific_p)
    {
        ueMeasInfoForUEs_p = (FAPI_UEMeasurementIndicationR9_st *)&
            vendorSpecific_p->ueMeasInfoForUEs;
        if(PNULL != ueMeasInfoForUEs_p)
        {
            numOfUEs = ueMeasInfoForUEs_p->numOfUEs;
            for(ueCount = 0; ueCount < numOfUEs; ueCount++)
            {
                rnti = MAC_PHY_CONVERT_16(ueMeasInfoForUEs_p->aoaRel9Info[ueCount].rnti);
                rntiInfo_p = getUeIdxFromRNTIMap(rnti,internalCellIndex);
                if (PNULL != rntiInfo_p)
                {
                    /* SPR 10487 START */
                    if(FREERNTI != rntiInfo_p->rntiFlag)
                    {
                    /* SPR 10487 END */
                        if(TCRNTI == rntiInfo_p->rntiFlag )
                        {
                            VALIDATE_AND_UPDATE_AOA_REPORTS(
                                    ueMeasInfoForUEs_p->aoaRel9Info,
                                    angleOfArrival,
                                    *validReports_p,
                                    ueCount );
                            tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->\
                                              tempCRNTICtxArray[rntiInfo_p->index].tempCRNTICtx_p;
                            if(PNULL != tempUEContext_p)
                            {
                                tempUEContext_p->validReports = *validReports_p;
                                tempUEContext_p->type1TaAOAReceivedTTI = recvTTI;
                                for(count = 0; count < MAX_NUM_ANTENNA; count ++)
                                {
                                    tempUEContext_p->angleOfArrival[count] =
                                        *(angleOfArrival + count);
                                }
                            }
                            continue;
                        }

                        else
                        {
                            ueIndex = rntiInfo_p->index;
                            ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
                            if((TRUE != ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag) && \
                                    PNULL != ulUEContext_p)
                            {
                                eCidReportParams_p = &ulUEContext_p->eCidReportParams;
                                VALIDATE_AND_UPDATE_AOA_REPORTS(
                                        ueMeasInfoForUEs_p->aoaRel9Info,
                                        angleOfArrival,
                                        *validReports_p, 
                                        ueCount);

                                SAVE_ECID_AOA_REPORT_IN_UE_CONTXT(eCidReportParams_p,
                                        angleOfArrival,
                                        recvTTI);

                                if((NO_REPORT_TYPE_REQUIRED != 
                                            eCidReportParams_p->typeofReportsRequired) &&
                                        (TYPE_2_TA_REPORT_REQUIRED != 
                                         eCidReportParams_p->typeofReportsRequired))
                                {
                                    checkValidReportsAndPutEntryInEcidMgr(ueIndex,
                                            *validReports_p,internalCellIndex);
                                }
                            }

                            dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
                            CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                                             cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
                            if((TRUE != dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag) && \
                                    PNULL != dlUEContext_p)
                            {     
                                aoaRel9Info =  &ueMeasInfoForUEs_p->aoaRel9Info[ueCount];
                                for (count = 0; count < cellParams_p->numOfTxAnteenas; count++)
                                {
                                    if(MAX_VALUE_OF_AOA >= aoaRel9Info->AoA[count])
                                    {
                                        dlUEContext_p->AoA[count] = 
                                            calculateTimeAvgAoA(dlUEContext_p->AoA[count] ,aoaRel9Info->AoA[count]);  
                                    }
                                }
                            }

                        }
                    }
                }
            }
        }
    }

}
#endif
/* + TM7_8 Changes Start */
 void processULSchInd(void *VendorSpecificParam_p, 
        void *msg_p,
        void *zeroBufCpy_p,
        UInt32 msgLen,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt32 withDemux,
        UInt32 dlDelay,
        UInt32 threadIndex,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
/* SPR 19288 change start */
/* SPR 19288 change end */

        )
/* - TM7_8 Changes End */
{
    UInt16  recvRnti      = 0;
    UInt16  ueIndex       = 0;
    /*  UInt16  tcRnti        = 0; TDD Warning Fix */
    UInt16  iterator     = 0;
    FAPI_rxULSCHIndication_st *ulSchMsg_p=(FAPI_rxULSCHIndication_st*)msg_p;
    FAPI_VendorSpecific_st *vendorSpecific_p=(FAPI_VendorSpecific_st*)VendorSpecificParam_p;
    UInt16  numOfPdu     = MAC_PHY_CONVERT_16(ulSchMsg_p->numOfPdu);
    UInt32  dataLen       = 0;
    UInt32  tcrntiFlag   = 0;
    RNTIInfo *rntiInfo_p = PNULL;
    FAPI_ulDataPduIndication_st   *ulDataInfo_p = PNULL;
    /* TA_CHG */
    SInt16  timingAdvance = 0;
    /* +CLPC_CHG_LJA */
    SInt8 powerControlDeltaTFAndLogFactor = 0;
    /* -CLPC_CHG_LJA */
    UInt8 riLength = 0;
    UInt8 ulHarqProcessId = 0;
    /* SPR 15909 fix start */
    /*coverity 97030 +*/
    /* +- SPR 18490 */
#ifndef DISABLE_UL_CQI_PROCESSING
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
    tickType_t currentTTI = getCurrentTick();
#endif    
#endif    
    /* +- SPR 18490 */
    /*coverity 97030 -*/
    /* SPR 15909 fix end */
    ULUEContextInfo* ulUEContextInfo_p = PNULL;
    /*CLPC_CHG_RILEN*/
#ifndef DISABLE_UL_CQI_PROCESSING
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))/*SPR 18490 +-*/
    /*CA Changes start  */
    UInt32 sinrReportQueueCount = COUNT_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex] + (currentTTI % SINR_CONTAINER_SIZE));
#else
    UInt32 sinrReportQueueCount = COUNT_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex]);
    /*CA Changes end  */
#endif
#endif  /*DISABLE_UL_CQI_PROCESSING*/  
    TempCRNTICtx *tempUEContext_p = PNULL;
    /* SPR 2437 Start*/
    /* SPR 1864 Start */
#ifdef TDD_CONFIG
    /* SPR 1864 End */
    /* TDD Config 6 Changes Start */
    UInt8 subframeNum  = 0;
    UInt16 sysFrameNum = 0;
    GET_SFN_SF_FROM_TTI(recvTTI, sysFrameNum, subframeNum)
    /* TDD Config 6 Changes End */
    /* SPR 1864 Start */
#endif
    ULHarqInfo *harqProcess_p = PNULL;
    /* SPR 1864 End */


    /* SPR 1864 End */
    /* SPR 2437 End */
    /* + TM7_8 Changes Start */
    /* - TM7_8 Changes End */

    /* TA_CHG */
/* SPR 19288 change start */
#ifndef FLEXRAN
/*SPR 21001 Changes start */
    void *demuxDataZeroBuf_p = PNULL;
    void *demuxDataZeroBufTmp_p = PNULL;
/*SPR 21001 Changes end */
#endif
    /* + E_CID_5.3 */
    FAPI_UEMeasurementIndication_st *ueMeasInfoForUEs_p = PNULL;
    /* - E_CID_5.3 */
#ifndef FLEXRAN
/*SPR 21001 Changes start */
    UInt32 splitOffset = ULSCH_IND_START_OF_PDU_OFFSET + 
        ( numOfPdu * sizeof(FAPI_ulDataPduIndication_st));
    SPLIT_ZERO_BUFFER_COPY(zeroBufCpy_p, demuxDataZeroBuf_p, splitOffset);
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
/* SPR 19288 change start */
#if defined(FLEXRAN)
/*SPR 21001 Changes start */
	UInt8   *handle_p = PNULL;
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */

    for (iterator = 0; iterator < numOfPdu; iterator++)
    {
#ifdef FDD_CONFIG
        UInt8 ttiBundleFlag = FALSE;
#endif
        tcrntiFlag = FALSE;
        ulDataInfo_p = (FAPI_ulDataPduIndication_st *)
            &ulSchMsg_p->ulDataPduInfo[iterator]; 
        recvRnti = MAC_PHY_CONVERT_16(ulDataInfo_p->rnti);
        /* getUeIndexFromRNTIMap returns NULL if not configured RARNTI or 
         * CRNTI Range and rntiFlag == FREERNTI is  deactive.
         */
        /*CA Changes start  */
        rntiInfo_p = getUeIdxFromRNTIMap(recvRnti, internalCellIndex);
        /*CA Changes end  */
        if ( PNULL == rntiInfo_p || FREERNTI == rntiInfo_p->rntiFlag )
        {
            lteWarning("processULSchInd:Recv Rnti %d is not Active at Mac Layer\n",recvRnti);

            /* +- SPR 17777 */
/* SPR 20636 Changes Start */
            LOG_MAC_MSG(MAC_ULSCH_RX_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
                    0/*coverity 97030 +-*/,\
                    recvRnti,\
                    (ADDR)rntiInfo_p,\
                    (rntiInfo_p ? rntiInfo_p->rntiFlag : 0), \
                    recvTTI,msgLen,\
                    0.0,0.0,__func__,"ULSCH_RX_RNTIERR");
/* SPR 20636 Changes End */
            /* +- SPR 17777 */
            /* SPR 11612 Changes Start */
            /*
            ** Before freeing Msg Buffer, copy data buffer of next pdu to new
            ** Msg Buffer by using SPLIT_ZERO_BUFFER_COPY.
            **
            ** Else,  Working on freed Msg Buffer on next for() loop cycle that
            ** was the issue.
            */
/* SPR 19288 change start */
#ifndef FLEXRAN
/*SPR 21001 Changes start */
            dataLen = MAC_PHY_CONVERT_16(ulDataInfo_p->length);
            if (dataLen)
            {                
                splitOffset = dataLen;
                SPLIT_ZERO_BUFFER_COPY(demuxDataZeroBuf_p, demuxDataZeroBufTmp_p, splitOffset);

                /*coverity 25191,25192 fix*/
                msgFree(demuxDataZeroBuf_p);
                /*coverity 25191,25192 fix*/
                demuxDataZeroBuf_p = demuxDataZeroBufTmp_p;
            }
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
            /* SPR 9526 Fix */ 
            continue ;
            /* SPR 9526 Fix */
        }

        /* Throughput correction */
        if (statsInfo_g[internalCellIndex].ulThroughPutEnable)
        {
            statsInfo_g[internalCellIndex].totalUplinkBytesSend +=  MULTIPLY_BY_EIGHT( 
                    MAC_PHY_CONVERT_16(ulDataInfo_p->length));
        }
        else
        {
            enableUplinkThroughput(1, internalCellIndex);
            statsInfo_g[internalCellIndex].totalUplinkBytesSend += MULTIPLY_BY_EIGHT(
                    MAC_PHY_CONVERT_16(ulDataInfo_p->length));
        }	
        /* Throughput correction */

        if ( TCRNTI == rntiInfo_p->rntiFlag )
        {
            /*tcRnti = rntiInfo_p->index; TDD Warning Fix */
            tcrntiFlag = TRUE;
            /*Power Control Change*/
            tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[rntiInfo_p->index].tempCRNTICtx_p;
            /*check if msg3 is received or contention is already sent*/
            if (tempUEContext_p->status == (MSG3_RECEIVED | CONTENTION_SENT))
            { 
                /* SPR 11612 Changes Start */
                /*
                ** Before freeing Msg Buffer, copy data buffer of next pdu to new
                ** Msg Buffer by using SPLIT_ZERO_BUFFER_COPY and then free Msg Buffer
                ** before continue to loop.
                */
/* SPR 19288 change start */
                dataLen = MAC_PHY_CONVERT_16(ulDataInfo_p->length);
#ifndef FLEXRAN
/*SPR 21001 Changes start */
                splitOffset = dataLen;
                SPLIT_ZERO_BUFFER_COPY(demuxDataZeroBuf_p, demuxDataZeroBufTmp_p, splitOffset);
                msgFree(demuxDataZeroBuf_p);
                demuxDataZeroBuf_p = demuxDataZeroBufTmp_p;
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */

                /* SPR 11612 Changes End */
                continue;
            }
            tempUEContext_p->msg3BuffInfo.sinr  = ulDataInfo_p->ulCqi;

            /* SPR 2333 Changes Begin */ 
            if((!ulDataInfo_p->dataOffset) || (!ulDataInfo_p->length)) 
            {
                harqProcess_p = &( tempUEContext_p->ulHarq );
                /* Need to perform adaptive re-transmission as it is the case of
                 * DTX */
                if( PNULL != harqProcess_p )
                {
                    if( MIN_SINR_FOR_TPC_CALC_IN_NACK >= ulDataInfo_p->ulCqi )
                    {
                        harqProcess_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                        /* set newDataIndicator to 0 so that adaptive retx can
                         * happen in NACK Queue processing at PDCCH */
                        harqProcess_p->newDataIndicator = 0;
                    }
                }

                continue;
            }
            /* SPR 2333 Changes Ends */ 
        }
        else 
        {   

            /* + Coverity_24790 */
#ifdef PERF_STATS
            /* Update the Counters for the total successfully transferred data
             * volume on MAC level in the Uplink */
            gMacUePerfStats_p[rntiInfo_p->index].lteUeMacULPerfStats.\
                totalUlBytesReceived += MAC_PHY_CONVERT_16(ulDataInfo_p->length);
#endif

            /* - Coverity_24790 */
            ueIndex = rntiInfo_p->index;/*SPR 695 OPT FIX*/
            /* SPR 2437 Start */
            ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];

            /* + COVERITY 5.0 */
            if ( ulUEContextInfo_p == PNULL)
            {
                /* SPR 11612 Changes Start */
                /*
                ** Before freeing Msg Buffer, copy data buffer of next pdu to new
                ** Msg Buffer by using SPLIT_ZERO_BUFFER_COPY and then free Msg Buffer
                ** before continue to loop.
                */
/* SPR 19288 change start */
                dataLen = MAC_PHY_CONVERT_16(ulDataInfo_p->length);
#ifndef FLEXRAN
/*SPR 21001 Changes start */
                splitOffset = dataLen;
                SPLIT_ZERO_BUFFER_COPY(demuxDataZeroBuf_p, demuxDataZeroBufTmp_p, splitOffset);
                msgFree(demuxDataZeroBuf_p);
                demuxDataZeroBuf_p = demuxDataZeroBufTmp_p;
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
                continue;
                /* SPR 11612 Changes End */
            }
            /* - COVERITY 5.0 */
            /* SPR 2437 End */

            /* SPR 4807 Changes Start */
            if ( TRUE == dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag ) 
                /* SPR 5317 Changes Start */
                /*(FALSE ==
                 * dlUECtxInfoArr_g[ueIndex].dlUEContext_p->tcrntiFlag ))*/
                /* SPR 4807 Changes End */
            {
                if (!dlUECtxInfoArr_g[ueIndex].dlUEContext_p || 
                        FALSE == dlUECtxInfoArr_g[ueIndex].dlUEContext_p->tcrntiFlag )
                {
                    lteWarning(" Pending Delete Flag True for UE Index %d ",
                            ueIndex);

                    LOG_MAC_MSG(MAC_ULSCH_RX_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
                            0/*coverity 97030 +-*/,\
                            recvRnti,\
                            ueIndex,\
                            recvTTI,0,0,\
                            0.0,0.0,__func__,"ULSCH_RX_UEDELETE");
                    /* SPR 11612 Changes Start */
                    /*
                    ** Before freeing Msg Buffer, copy data buffer of next pdu to new
                    ** Msg Buffer by using SPLIT_ZERO_BUFFER_COPY and then free Msg Buffer
                    ** before continue to loop.
                    */
/* SPR 19288 change start */
                    dataLen = MAC_PHY_CONVERT_16(ulDataInfo_p->length);
#ifndef FLEXRAN
/*SPR 21001 Changes start */
                    splitOffset = dataLen;
                    SPLIT_ZERO_BUFFER_COPY(demuxDataZeroBuf_p, demuxDataZeroBufTmp_p, splitOffset);
                    msgFree(demuxDataZeroBuf_p);
                    demuxDataZeroBuf_p = demuxDataZeroBufTmp_p;
/*SPR 21001 Changes end*/
#endif
/* SPR 19288 change end */
                    /* SPR 11612 Changes End */
                    continue;
                }
                /* SPR 5317 Changes End */

            }
#ifndef DISABLE_UL_CQI_PROCESSING
            /* ULA_CHG */
            /* To prevent overloading of sinrReportQueue_g, not more than
             * MAC_MAX_SINR_REPORTS_QUEUE_LENGTH nodes should be present in
             * the queue at any time.
             */           
#ifdef DL_UL_SPLIT
            if (sinrReportQueueCount < MAC_MAX_SINR_REPORT_QUEUE_LENGTH_SPLIT)
#else
            if (sinrReportQueueCount < MAC_MAX_SINR_REPORTS_QUEUE_LENGTH)
#endif
            {
                /* This function will enqueue the received SINR information 
                 * which will be processed by the execution leg - 
                 * ElProcessSINRReports
                 */
                /* SPR 2728 Start */
                /*We should ignore SINR value if dataOffset is 0 in ULSCH PDU*/
                if((!ulDataInfo_p->dataOffset)||(!ulDataInfo_p->length))
                {
                    // SPR 3531 START
                    /* NACK CASE  */
                    /* +- SPR 17777 */
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
                    if(MAC_SUCCESS == putULSCHEntryInSINRReportQueue(ueIndex, \
                                /* Time Averaging changes start */
                                recvTTI, \
                                ulDataInfo_p->ulCqi,\
                                TRUE,\
                                currentTTI,
                                /*CA Changes start  */
                                internalCellIndex))
#else
                    if(MAC_SUCCESS == putULSCHEntryInSINRReportQueue(ueIndex, \
                                /* Time Averaging changes start */
                                recvTTI, \
                                ulDataInfo_p->ulCqi,\
                                TRUE,\
                                /*CA Changes start  */
                                internalCellIndex))
#endif
                        /* +- SPR 17777 */
                        /*CA Changes end  */
                        /// SPR 3531 END
                    {
                        sinrReportQueueCount++;
                    }
                }
                else
                {
                    /* SPR 2728 End */
                    /* SPR 2437 Start */
                    /* +- SPR 17777 */
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
                    if(MAC_SUCCESS == putULSCHEntryInSINRReportQueue(ueIndex, \
                                /* Time Averaging changes start */
                                recvTTI, \
                                ulDataInfo_p->ulCqi,\
                                FALSE,\
                                currentTTI,
                                /* Time Averaging changes end */
                                /*CA Changes start  */
                                internalCellIndex))
#else
                    if(MAC_SUCCESS == putULSCHEntryInSINRReportQueue(ueIndex, \
                                /* Time Averaging changes start */
                                recvTTI, \
                                ulDataInfo_p->ulCqi,\
                                FALSE,\
                                /* Time Averaging changes end */
                                /*CA Changes start  */
                                internalCellIndex))
#endif
                        /* +- SPR 17777 */
                        /*CA Changes end  */
                    {
                        sinrReportQueueCount++;
                    }
                    /* SPR 2437 End */
                    /* SPR 2728 Start */
                }
                /* SPR 2728 End */
            }
            else
            {
                LOG_MAC_MSG(MAC_L1_INF_SINR_QUEUE_FULL_ID, LOGWARNING, MAC_L1_INF,\
                        0/*coverity 97030 +-*/, \
                        ueIndex, recvTTI, 0, 0, 0, \
                        0, 0, \
                        __func__,"SINR_QUEUE_FULL");
            }
#endif /*DISABLE_UL_CQI_PROCESSING*/           
            /*CLPC_CHG_RILEN*/
            /* +COVERITY 10735 */
            if(ulUEContextInfo_p->ulUEContext_p)
            {
                harqProcess_p = &(ulUEContextInfo_p->ulUEContext_p->ulresInfo[ulHarqProcessId]);
                /* -COVERITY 10735 */
#ifdef FDD_CONFIG

                /* TTIB_Code Start */
                UInt8 tempId = MODULO_SIXTEEN(recvTTI);
                /* If ULSCH Ind receive before CRC
                * indication then ulHarqProcessId
                * will not be equal to
                * TTI_MAP_FREE*/
                /*++ SPR 19181 */
                ulHarqProcessId = ulUEContextInfo_p->ulUEContext_p->ttiHarqMap[tempId];
                if(TTI_MAP_FREE == ulHarqProcessId)
                {
                    /*-- SPR 19181 */
                    /** SPR 12364 fix Starts**/
                    ulHarqProcessId = ulUEContextInfo_p->ulUEContext_p->ttiPUSCHHarq[tempId];
                    /** SPR 12364 fix Ends **/
                }
                harqProcess_p = &(ulUEContextInfo_p->ulUEContext_p->ulresInfo[ulHarqProcessId]);
                /*klockwork warning fix*/ 
                /** SPR 12364 fix Starts**/
                if( ( harqProcess_p != PNULL ) && 
                    ( TRUE == harqProcess_p->ulSchReqInfo.isTTIBundled ) )
                    /** SPR 12364 fix Ends **/
                {
                    ttiBundleFlag = TRUE;
                }
                /** SPR 12364 fix Starts**/
                riLength =  harqProcess_p->ulSchReqInfo.riLength; 
                /** SPR 12364 fix Ends **/
                /* TTIB_Code End */

#elif TDD_CONFIG
                /* TDD Config 6 Changes Start */
                ulHarqProcessId = getULHarqIDFromSubFrame(subframeNum, ulUEContextInfo_p->ulUEContext_p, 
                        internalCellIndex,sysFrameNum);
                harqProcess_p = &(ulUEContextInfo_p->ulUEContext_p->ulresInfo[ulHarqProcessId]);
                /* TDD Config 6 Changes End */
                /*klockwork warning fix*/ 
                if((harqProcess_p != PNULL)&& (ulHarqProcessId != INVALID_HARQ))
                {
                    riLength =  harqProcess_p->riLength;
                }
                else
                {
                    riLength = 0;
					fprintf(stderr, "set ri to zero in ulSchInd\n");
                }
#endif
                /*CLPC_CHG_RILEN*/
                /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                /** SPR 12364 fix Starts**/
                if( harqProcess_p->ulSchReqInfo.hdFddUlNackHandlingFlag == TRUE)   
                    /** SPR 12364 fix Ends **/
                {
                    LOG_MAC_MSG(MAC_HD_FDD_DISCARD_DATA, LOGWARNING, MAC_L1_INF,\
                            getCurrentTick(), \
                            ulUEContextInfo_p->ulUEContext_p->ueIndex,ulHarqProcessId,\
                            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE, \
                            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE, \
                            FUNCTION_NAME,"DISCARD_DATA");
                    /* SPR 11612 Changes Start */
                    /*
                    ** Before freeing Msg Buffer, copy data buffer of next pdu to new
                    ** Msg Buffer by using SPLIT_ZERO_BUFFER_COPY and then free Msg Buffer
                    ** before continue to loop.
                    */
                    dataLen = MAC_PHY_CONVERT_16(ulDataInfo_p->length);
/* SPR 19288 change start */
#ifndef FLEXRAN
/*SPR 21001 Changes start */
                    splitOffset = dataLen;
                    SPLIT_ZERO_BUFFER_COPY(demuxDataZeroBuf_p, demuxDataZeroBufTmp_p, splitOffset);
                    msgFree(demuxDataZeroBuf_p);
                    demuxDataZeroBuf_p = demuxDataZeroBufTmp_p;
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
                    /* SPR 11612 Changes End */
                    continue;
                }
#endif
                /*HD FDD Changes End*/

                /* ULA_CHG */            
                /* SPR 2437 Start */
#ifdef TDD_CONFIG
                /* SPR 2509 CHG start */
                /* SPR 4560 Changes */
                /* CRC_ULSCH_IND SPR 8722 Fix Start */
                /*coverity 56644*/
                /* SPR_10082_FIX_START */
                /* SPR_19950_FIX_START */
                if (ulHarqProcessId != INVALID_HARQ)
                {
                    if(((ulDataInfo_p->dataOffset) && (ulDataInfo_p->length))||(harqProcess_p->currentRetransCount >= (ulUEContextInfo_p->ulUEContext_p->maxUlHARQTx -1))) 
                    {
                        /* SPR_10082_FIX_END */
                        ulUEContextInfo_p->ulUEContext_p->ulresInfo[ulHarqProcessId].crcUlschIndFlag |= ULSCH_IND_RCVD_FLAG;
                    }
                    /*coverity 56644*/
                    if ((CRC_ULSCH_IND_RCVD_FLAG == 
                                ulUEContextInfo_p->ulUEContext_p->ulresInfo[ulHarqProcessId].crcUlschIndFlag))
                    {
                        resetHarqId( subframeNum,ulUEContextInfo_p->ulUEContext_p,internalCellIndex,sysFrameNum);
                    }
                }
                /* SPR_19950_FIX_END */
                /* SPR 4560 Changes */
#endif
                /* SPR 2437 End */          
                /*dlDelay and null pointer check?*/
                /* Time Averaging changes start */
                powerControlDeltaTFAndLogFactor = ulUEContextInfo_p->ulUEContext_p->powerControlDeltaTFAndLogFactor[recvTTI % MAX_SUB_FRAME]; 
                /* Time Averaging changes end */
                /* -CLPC_CHG_LJA */
                /* +COVERITY 10735 */
#ifdef PERF_STATS
                UPDATE_UE_PUSCH_SINR_STATS (ueIndex, ulDataInfo_p->ulCqi);
#endif
            }
            /* -COVERITY 10735 */
        }
        dataLen = MAC_PHY_CONVERT_16(ulDataInfo_p->length);
        /*CA Changes start  */
        totalMacUllinkBytesSend_g[internalCellIndex] += MULTIPLY_BY_EIGHT(dataLen);
        /*CA Changes end  */
        /*SPR_3061_FIX*/
        if((!ulDataInfo_p->dataOffset)||(!dataLen))
        {  
            /* Need to perform adaptive re-transmission as it is the case of
             * DTX */
            if( PNULL != harqProcess_p )
            {
                if( MIN_SINR_FOR_TPC_CALC_IN_NACK >= ulDataInfo_p->ulCqi )
                {
                    harqProcess_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                    /* set newDataIndicator to 0 so that adaptive retx can
                     * happen in NACK Queue processing at PDCCH */
                    harqProcess_p->newDataIndicator = 0;
                }
            }
            continue;
        }
        /*SPR_3061_FIX*/

        /* TA_CHG */
        if (TRUE != tcrntiFlag)
        {
            DLUEContext *newUEcontext_p = PNULL;
            newUEcontext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
            /* SPR 2628 Changes Start */
            timingAdvance = MAC_PHY_CONVERT_16(ulDataInfo_p->timingAdvance);
#ifdef FLEXRAN
            timingAdvance = timingAdvance - MAC_TIMING_ADVANCE_DEFAULT_VAL + MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31;
#endif
            if ((timingAdvance > MAC_TIMING_ADVANCE_VAL_MAX) || (timingAdvance < 0))
            {
                lteWarning("Invalid TA value = %d in ULSCH\n", timingAdvance);
            }
            else
            {
                handleTimingAdvance(newUEcontext_p, ueIndex, (UInt16)timingAdvance, internalCellIndex, getCurrentTickUl());

                LOG_MAC_MSG(MAC_TA_INSERTION,LOGBRIEF,MAC_L1_INF,\
                    getCurrentTick(),\
                    getCurrentTick(),newUEcontext_p->lastSyncDetectedTick,\
                    ueIndex,timingAdvance,\
                    DEFAULT_INT_VALUE,\
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                    FUNCTION_NAME,"LAST_SYNC_TICK");
                
                newUEcontext_p->lastSyncDetectedTick = getCurrentTick();
            }
        }
        /* SPR 2333 Changes Begin */
        if((!ulDataInfo_p->dataOffset)||(!dataLen))
        {
            continue;
        }
        /* SPR 2333 Changes Ends */ 
        /* TA_CHG */
/* SPR 19288 change start */


#ifdef FLEXRAN
        /* Extract Offset to reach MAC PDU (from handle), read MAc PDU from separate shared memory
           through flexran_ipc_get_ul_databuffer and copy the MAC PDU */

        QSEGMENT segment;
        UInt8* pdu_ptr     = NULL;
        void *dataBuffer_p = NULL;
		   
        dataBuffer_p = msgAlloc(PNULL,0,0,0);

	    msgReserve(dataBuffer_p, 0, dataLen);
        if (PNULL ==  msgSegNext(dataBuffer_p, 0, &segment))
        {
            ltePanic("Error In msgSegNext\n");
            break;
        }
        handle_p = segment.base;

        if(ulDataInfo_p->dataOffset != 0)
        {
	        pdu_ptr = (UInt8*)(flexran_ipc_get_ul_databuffer(internalCellIndex) + 
                              (UInt32)ulDataInfo_p->handle);			
       
            memcpy(handle_p, pdu_ptr,dataLen);
        }
	    else
	    {
   	        // CRC fail ?
	    }
#ifdef FLEXRAN_FAPI_LOGGING
    if(0 != flexranFapiLoggingEnable_g)
    {
	    uint32_t debugOffset = 0;
	    uint16_t dataLegnth = 0;

	    if (ulDataInfo_p->dataOffset != 0) // CRC not fail
		    dataLegnth = ulDataInfo_p->length > MAX_LOG_TB_DATA_SIZE ? MAX_LOG_TB_DATA_SIZE : ulDataInfo_p->length;

	    FAPI_l1ApiMsg_st* pFapiHdr = (FAPI_l1ApiMsg_st*)macPhyBuffer;
	    pFapiHdr->msgId = FAPI_DEBUG_UL_DATA;
	    pFapiHdr->lenVendorSpecific = 0;
	    pFapiHdr->msgLen = sizeof(fapi_debug_t) + dataLegnth;
	    debugOffset += 4;

	    fapi_debug_t* pDebugHdr = (fapi_debug_t*)(macPhyBuffer + debugOffset);
	    pDebugHdr->frameNum = sysFrameNum;
	    pDebugHdr->subframeNum = subframeNum;
	    pDebugHdr->crnti = ulDataInfo_p->rnti;
	    pDebugHdr->pduIdx = 0; // fill this if information is available
	    pDebugHdr->handle = ulDataInfo_p->handle;
	    debugOffset += sizeof(fapi_debug_t);

	    uint8_t* pDebugInfo = (uint8_t*)(macPhyBuffer + debugOffset);
	    if (dataLegnth > 0)
	    {
		    memcpy(pDebugInfo, handle_p, dataLegnth);
		    do_tap_fun((void*)pFapiHdr, pFapiHdr->msgLen + 4, internalCellIndex);
	    }
    }
#endif
#endif

/* SPR 19288 change end */
/* SPR 19288 change start */
#ifndef FLEXRAN
/*SPR 21001 Changes start */
        splitOffset = dataLen;
        SPLIT_ZERO_BUFFER_COPY(demuxDataZeroBuf_p, demuxDataZeroBufTmp_p, splitOffset);
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
        LOG_UT(MAC_ULSCH_RX_DATA_ID,LOGDEBUG,MAC_L1_INF,\
                0/*coverity 97030 +-*/,\
                ueIndex,recvRnti,\
                recvTTI,dataLen,withDemux,\
                (UDouble32)threadIndex,(UDouble32)dlDelay,__func__,"ULSCH_RX_DATA");

#ifdef FDD_CONFIG

        /* TTIB_Code Start */
        if(TRUE == ttiBundleFlag)
        {
            /** SPR 12364 fix Starts**/
            if( TRUE == ulUEContextInfo_p->ulUEContext_p->
                    ulresInfo[ulHarqProcessId].ulSchReqInfo.isTTIBundled &&
                    FALSE == ulUEContextInfo_p->ulUEContext_p->
                    ulresInfo[ulHarqProcessId].ulSchReqInfo.isTTIDataRcvd )
            {
                ulUEContextInfo_p->ulUEContext_p->ulresInfo[ulHarqProcessId].
                    ulSchReqInfo.isTTIDataRcvd = TRUE;
                /** SPR 12364 fix Ends **/
                /*++ SPR 19156 */
                ulUEContextInfo_p->ulUEContext_p->ulresInfo[ulHarqProcessId].isTTIDataRcvd = TRUE;
                /*--SPR 19156 */                
            }
            else
            {
/* SPR 19288 change start */
#ifndef FLEXRAN
/*SPR 21001 Changes start */
                msgFree(demuxDataZeroBuf_p);
                demuxDataZeroBuf_p = demuxDataZeroBufTmp_p;
                continue;
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
            }
        }
        /* TTIB_Code End */
#endif


        if (TRUE == withDemux )
        {
/* SPR 19288 change start */
#if defined(FLEXRAN)
/*SPR 21001 Changes start */
/* SPR 4751 changes */
            processDemuxData(ueIndex, recvRnti, recvTTI, dataBuffer_p,
                    dataLen, powerControlDeltaTFAndLogFactor, threadIndex, dlDelay,0,internalCellIndex);
/* SPR 4751 changes */
            msgFree(dataBuffer_p);
#else
            /* SPR 4751 changes */
            processDemuxData(ueIndex, recvRnti, recvTTI, demuxDataZeroBuf_p,
                    /* +- SPR 17777 */
                    dataLen, powerControlDeltaTFAndLogFactor, threadIndex, dlDelay,0, 
                    /*CA Changes start  */
                    internalCellIndex);
            /*CA Changes end  */
            /* SPR 4751 changes */
#ifndef FLEXRAN
            msgFree(demuxDataZeroBuf_p);
#endif
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
        }
        else
        {
/* SPR 19288 change start */
#if defined(FLEXRAN)
/*SPR 21001 Changes start */
            putDataInDemuxQueue(ueIndex, recvRnti, recvTTI, dataBuffer_p,
                    dataLen, powerControlDeltaTFAndLogFactor, riLength, ulHarqProcessId, internalCellIndex);
#else
            /* +CLPC_CHG_LJA */
            /* SPR 4751 changes */
            putDataInDemuxQueue(ueIndex, recvRnti, recvTTI, demuxDataZeroBuf_p,
                    /*CA Changes start  */
                    dataLen,powerControlDeltaTFAndLogFactor, riLength, ulHarqProcessId, internalCellIndex);
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
            /*CA Changes end  */
            /* SPR 4751 changes */
            /* -CLPC_CHG_LJA */
        }
/* SPR 19288 change start */
#ifndef FLEXRAN
/*SPR 21001 Changes start */
        demuxDataZeroBuf_p = demuxDataZeroBufTmp_p;
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
    }

    /* + E_CID_5.3 */
    if(PNULL != vendorSpecific_p)
    {
        ueMeasInfoForUEs_p = (FAPI_UEMeasurementIndication_st *)&
            vendorSpecific_p->ueMeasInfoForUEs;
        /* SPR 9829 changes start */
        macProcessVendorSpecificFieldsUlschInd(ueMeasInfoForUEs_p,recvTTI,internalCellIndex);
        /* SPR 9829 changes end */
    }
    /* - E_CID_5.3 */
/* SPR 19288 change start */
#ifndef FLEXRAN
/*SPR 21001 Changes start */
    msgFree(demuxDataZeroBuf_p);
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
    return;
}
#ifdef FAPI_4_0_COMPLIANCE
/*****************************************************************************
 * Function Name  : processRACHIndicationR9 
 * Inputs         : 1. msg_p  - pointer to message.
 *                  2. recvTTI  - Its represent the tick of the message
 *                              recieved.
 *                  vendorSpecific_p
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the RACH Indication msg from PHY  in case 
 *                    Release capability is Release9 or Later
 *****************************************************************************/
void processRACHIndicationR9( void *msg_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        FAPI_VendorSpecificForRACHInd_st *vendorSpecific_p,
        InternalCellIndex internalCellIndex)
{
    FAPI_rachIndicationR9_st *rachMsg_p=(FAPI_rachIndicationR9_st *)msg_p;
    UInt32  numOfPreambles  = rachMsg_p->numOfPreamble;
    UInt16  iterator = 0;   
    FAPI_rachPduIndicationR9_st  *rachPdu_p = PNULL;
    UInt32 RARReqQueueSize = sizeof(RAReqQueueNode);
    RAReqQueueNode *raReqQueueNode_p = PNULL;
    RAPreambleReq  *raPreambleReq_p  = PNULL;
    /*UInt32 timingAdvanceType2 = 0;*/
    /*UInt8 count = 0;*/
    /* +- SPR 18268 */
    UInt16 ueIndex = INVALID_UE_INDEX;
    /* +- SPR 18268 */
    /* 	UInt8 eCidNumOfpreambles = 0;*/
    UInt16 timingAdvanceR9 =  INVALID_TIMING_ADVANCE_R9_VALUE;
    ULUEContext *ulUEContext_p = PNULL;
    /*FAPI_UEMeasurementInRACH_st *ueMeasInfoInRACHForUEs_p = PNULL;*/
    ContentionFreePreamble *contFreePreamble_p = PNULL;
    UInt8 numOfAntenna = 0;
    /* - E_CID_5.3 */
  LTE_GCC_UNUSED_PARAM(vendorSpecific_p)
    for (iterator = 0; iterator < numOfPreambles; iterator++)
    {
        numOfrachInd_g[internalCellIndex]++;
        rachPdu_p = (FAPI_rachPduIndicationR9_st *)
            &rachMsg_p->rachPduInfo[iterator];
        GET_MEM_FROM_POOL(RAReqQueueNode,raReqQueueNode_p,RARReqQueueSize,PNULL);
        if (PNULL != raReqQueueNode_p)
        {

            raReqQueueNode_p->raRNTI           = rachPdu_p->rnti;
            raReqQueueNode_p->ttiRAReqReceived = recvTTI;
            raPreambleReq_p = &raReqQueueNode_p->raPreambleInfo;
            /* preamble value 0 to 63*/
            raPreambleReq_p->preamble = rachPdu_p->preamble; 
            /* Timing advance value 0 to 1282*/ 
            raPreambleReq_p->timingAdvance = MAC_PHY_CONVERT_16(rachPdu_p->timingAdvance);
            if ( raPreambleReq_p->timingAdvance > MAX_RA_TA_VALUE )
            {
                lteWarning("Invalid Timing Advance value\n");
                freeMemPool(raReqQueueNode_p);
                raReqQueueNode_p = PNULL;
                continue;
            }
            /* The below values dont come from PHY as per FAPI interface, 
               so setting them to zero for time being */
            raPreambleReq_p->hoppingBit = 0;
            raPreambleReq_p->tmcs = 0;
            raPreambleReq_p->ulDelay = 0;
            /* +- SPR 17777 */
            raPreambleReq_p->tpc = getTPCForMsg3(internalCellIndex);
            /* +- SPR 17777 */
            /*Entry in RQ Queue*/

            putEntryInRAReqQueue(raReqQueueNode_p,internalCellIndex);
        }
        else
        {
            /* Not able to allocate Memory */
            continue;
        }
        /* Rel 5.3: Coverity Fix End */


        /* FAPI2_1 Changes start */
        timingAdvanceR9 = (MAX_TIMING_ADVANCE_R9_VALUE >=
                MAC_PHY_CONVERT_16(rachPdu_p->release9Param.timingAdvanceR9))?\
            MAC_PHY_CONVERT_16(rachPdu_p->release9Param.timingAdvanceR9):\
            INVALID_TIMING_ADVANCE_R9_VALUE;
        contFreePreamble_p = getContentionFreePreambleInfo(rachPdu_p->preamble,internalCellIndex);

        /* FAPI2_1 Changes end */
        if(PNULL != contFreePreamble_p)
        {
            ueIndex = contFreePreamble_p->ueIdentifier;
            /*Store TA R9 value in temp contxt and updated it in Temp UE
             * context*/
            /*SPR 12275 fix, adding validation for ueIndex*/ 
            if(INVALID_UE_ID <= ueIndex) 
            {
                continue;
            }
            ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
            if(PNULL != ulUEContext_p)
            {

                /*If RRM requested for type1 and/or type2 TA reports then */
                if(ulUEContext_p->eCidReportParams.typeofReportsRequired & (TRUE << 1))
                {
                    ulUEContext_p->eCidReportParams.type2TAValue = timingAdvanceR9;
                    if(INVALID_TIMING_ADVANCE_R9_VALUE != timingAdvanceR9)
                    {
                        ulUEContext_p->eCidReportParams.type2TAreceivedTTI = recvTTI;
                        if(TYPE_2_TA_REPORT_REQUIRED == 
                                ulUEContext_p->eCidReportParams.typeofReportsRequired)
                        {
                            ulUEContext_p->eCidReportParams.reportStatus = REPORTS_RECEIVED;
                            putEntryInEcidMeasReportQueue(ueIndex, \
                                    ulUEContext_p->eCidReportParams.typeofReportsRequired,\
                                    MAC_SUCCESS,internalCellIndex);
                        }
                        else if(WAIT_FOR_TA_2_REPORT_ONLY_FOR_RELEASE_8_UE == \
                                ulUEContext_p->eCidReportParams.reportStatus)
                        {
                            ulUEContext_p->eCidReportParams.type1TAValue = INVALID_TIMING_ADVANCE_R9_VALUE;
                            for (numOfAntenna  = 0; numOfAntenna < MAX_NUM_ANTENNA; numOfAntenna ++)
                                ulUEContext_p->eCidReportParams.angleOfArrival[numOfAntenna] = 
                                    INVALID_ANGLE_OF_ARRIVAL_VALUE;
                            putEntryInEcidMeasReportQueue(ueIndex,\
                                    ulUEContext_p->eCidReportParams.typeofReportsRequired,\
                                    MAC_PARTIAL_SUCCESS,internalCellIndex); 

                        }
                        else
                        {
                            /*if UE is exists then store UE index in temp contxt
                             * which will be used to find for which MSG3 reception was failed */
                            tempTimingAdvanceR9_g[internalCellIndex][rachPdu_p->preamble].ueIndex = ueIndex;

                            ulUEContext_p->eCidReportParams.reportStatus = 
                                WAIT_FOR_REPORTS_TA_1_OR_AOA_AFTER_TA_2_RECEIVED;
                        }
                    }
                    else
                    {
                        if(TYPE_2_TA_REPORT_REQUIRED != 
                                ulUEContext_p->eCidReportParams.typeofReportsRequired)
                        {
                            /*if UE is exists then store UE index in temp contxt
                             * which will be used to find for which MSG3 reception was failed */
                            tempTimingAdvanceR9_g[internalCellIndex][rachPdu_p->preamble].ueIndex = ueIndex;

                            ulUEContext_p->eCidReportParams.type2TAreceivedTTI = recvTTI;
                            ulUEContext_p->eCidReportParams.reportStatus =
                                WAIT_FOR_REPORTS_TA_1_OR_AOA_AFTER_TA_2_RECEIVED;
                        }
                        else
                        {
                            putEntryInEcidMeasReportQueue(ueIndex,\
                                    NO_REPORT_TYPE_REQUIRED,\
                                    MAC_FAILURE,internalCellIndex); 
                        }

                    }
                }
            }
        }
        else
        {
            tempTimingAdvanceR9_g[internalCellIndex][rachPdu_p->preamble].type2TAValue = timingAdvanceR9;
            tempTimingAdvanceR9_g[internalCellIndex][rachPdu_p->preamble].type2TAreceivedTTI = recvTTI;
        }
    }

    return ;
}
#endif
/*****************************************************************************
 * Function Name  : processRACHIndication 
 * Inputs         : msg_p  - pointer to RACH message(Payload part).
 *                  recvTTI  - Its represent the tick of the message recieved
 *                  vendor_Specific_p - pointer to vendor specific field
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the RACH Indication msg from PHY.
 *****************************************************************************/
void processRACHIndication(  void *msg_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        FAPI_VendorSpecificForRACHInd_st *vendorSpecific_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
        )
{
    FAPI_rachIndication_st *rachMsg_p = (FAPI_rachIndication_st *)msg_p;
    UInt32  numOfPreambles  = rachMsg_p->numOfPreamble;
    UInt16  iterator = 0;   
    FAPI_rachPduIndication_st  *rachPdu_p = PNULL;
    UInt32 RARReqQueueSize = sizeof(RAReqQueueNode);
    RAReqQueueNode *raReqQueueNode_p = PNULL;
    RAPreambleReq  *raPreambleReq_p  = PNULL;
    /* + E_CID_5.3 */
    FAPI_UEMeasurementInRACH_st *ueMeasInfoInRACHForUEs_p = PNULL;
    /* - E_CID_5.3 */
   
#ifdef FLEXRAN
    numOfPreambles_g[internalCellIndex] += numOfPreambles;
#endif

    for (iterator = 0; iterator < numOfPreambles; iterator++)
    {
        /*CA Changes start  */
        numOfrachInd_g[internalCellIndex]++;
        /*CA Changes end  */
        rachPdu_p = (FAPI_rachPduIndication_st *)
            &rachMsg_p->rachPduInfo[iterator];
        GET_MEM_FROM_POOL(RAReqQueueNode,raReqQueueNode_p,RARReqQueueSize,PNULL);

        /* Rel 5.3: Coverity 24452 Fix Start */
        if (PNULL != raReqQueueNode_p)
        {
            raReqQueueNode_p->raRNTI           = MAC_PHY_CONVERT_16(rachPdu_p->rnti);
            raReqQueueNode_p->ttiRAReqReceived = recvTTI;
            raPreambleReq_p = &raReqQueueNode_p->raPreambleInfo;
            /* preamble value 0 to 63*/
            raPreambleReq_p->preamble = rachPdu_p->preamble; 
            /* Timing advance value 0 to 1282*/ 
            raPreambleReq_p->timingAdvance = MAC_PHY_CONVERT_16(rachPdu_p->timingAdvance);
            /*SPR 21258 fix start*/
            if ( raPreambleReq_p->timingAdvance > MAX_RA_TA_VALUE )
                 /*SPR 21258 fix end*/
            {
                lteWarning("Invalid Timing Advance value\n");
                freeMemPool(raReqQueueNode_p);
                raReqQueueNode_p = PNULL;
                continue;
            }
            /* The below values dont come from PHY as per FAPI interface, 
               so setting them to zero for time being */
            raPreambleReq_p->hoppingBit = 0;
            raPreambleReq_p->tmcs = 0;
            raPreambleReq_p->ulDelay = 0;
            /* +- SPR 17777 */
            raPreambleReq_p->tpc = getTPCForMsg3(internalCellIndex);
            /* +- SPR 17777 */
            /*Entry in RQ Queue*/

            /*CA Changes start  */
            putEntryInRAReqQueue(raReqQueueNode_p, internalCellIndex);
            /*CA Changes end  */
        }
        else
        {
            /* Not able to allocate Memory */
            continue;
        }
        /* Rel 5.3: Coverity Fix End */

        /* + E_CID_5.3 */
        /*CA Changes start  */
        tempTimingAdvanceR9_g[internalCellIndex][rachPdu_p->preamble].type2TAValue = 
            INVALID_TIMING_ADVANCE_R9_VALUE;
        tempTimingAdvanceR9_g[internalCellIndex][rachPdu_p->preamble].type2TAreceivedTTI = recvTTI;
        /*CA Changes end  */
    }
/*SPR 21001 Changes start */
    if(PNULL != vendorSpecific_p)
    {
        ueMeasInfoInRACHForUEs_p =
            (FAPI_UEMeasurementInRACH_st *)&vendorSpecific_p->ueMeasInfoInRACHForUEs;
        /* SPR 9829 changes start */
        macProcessVendorSpecificFieldsRachInd(ueMeasInfoInRACHForUEs_p,recvTTI,
                internalCellIndex);
        /* SPR 9829 changes end */
    }
/*SPR 21001 Changes end */
    /* - E_CID_5.3 */
    return ;
}
/*****************************************************************************
 * Function Name  : processSRIndication 
 * Inputs         : srMsg_p  - pointer to SR message(Payload part),
 *                  recvTTI  - Its represent the tick of the message recieved
 *                  dlDelay - Downlink Delay
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the SR Indication msg from PHY.
 *****************************************************************************/
/*****************************************************************************
 * Function Name  : processSRIndication 
 * Inputs         : srMsg_p  - pointer to SR message(Payload part),
 *                  recvTTI  - Its represent the tick of the message recieved
 *                  dlDelay - Downlink Delay
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the SR Indication msg from PHY.
 *****************************************************************************/
void processSRIndication(FAPI_rxSRIndication_st *srMsg_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        /* Rel9_upgrade_CR410 */
        UInt32 dlDelay,
        /* Rel9_upgrade_CR410 */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
        )
{
    UInt16  ueIndex       = 0;
    UInt16  iterator      = 0;
    UInt16  recvRnti      = 0;
    /*    UInt16  tcRnti        = 0;
          UInt8   tcrntiFlag    = FALSE; TDD Warning Fix */
    RNTIInfo *rntiInfo_p   = PNULL;
    FAPI_srPduIndication_st *srPdu_p = PNULL;
    /* Rel9_upgrade_CR410 */
    ULUEContext *ulUEContext_p = PNULL;
    UInt16   numOfSr  =  MAC_PHY_CONVERT_16(srMsg_p->numOfSr); 
    /* Rel9_upgrade_CR410 */
    /*coverity 97029 +-*/
    for (iterator = 0; iterator < numOfSr; iterator++)
    {
        srPdu_p = (FAPI_srPduIndication_st *)&srMsg_p->srPduInfo[iterator]; 
        recvRnti = MAC_PHY_CONVERT_16(srPdu_p->rnti);

        /* getUeIndexFromRNTIMap returns NULL if not configured RARNTI or 
         * CRNTI Range and rntiFlag == FREERNTI is  deactive.
         */
        /*CA Changes start  */
        rntiInfo_p = getUeIdxFromRNTIMap(recvRnti, internalCellIndex);
        /*CA Changes end  */
        if (PNULL == rntiInfo_p || FREERNTI == rntiInfo_p->rntiFlag)
        {
            lteWarning("processSRIndication:Recv Rnti %d is not Active at Mac Layer\n",recvRnti);   

/* SPR 20636 Changes Start */
            LOG_MAC_MSG(MAC_SR_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
                    0/*coverity 97029 +-*/,\
                    recvRnti,\
                    (ADDR)rntiInfo_p,\
                    (rntiInfo_p ? rntiInfo_p->rntiFlag : 0), \
                    recvTTI,0,\
                    0.0,0.0,__func__,"SR_RNTIERR");

/* SPR 20636 Changes End */
            continue ;
        }
        /*        if (TCRNTI == rntiInfo_p->rntiFlag)
                  {
                  tcRnti = rntiInfo_p->index;
                  tcrntiFlag = TRUE;
                  }
                  else */ /* TDD Warning Fix */
        if (TCRNTI != rntiInfo_p->rntiFlag)
        {    
            ueIndex = rntiInfo_p->index;/*FIX SPR 695 OPT*/
            if (dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag)
            {
                lteWarning(" Pending Delete Flag True for UE Index %d ",
                        ueIndex);
                LOG_MAC_MSG(MAC_SR_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
                        0/*coverity 97029 +-*/,\
                        recvRnti,\
                        ueIndex,\
                        recvTTI,0,0,\
                        0.0,0.0,__func__,"SR_UEDELETE");
                continue;

            }
        }
        ulUEContext_p = ulUECtxInfoArr_g[rntiInfo_p->index].ulUEContext_p;
        /* SPR 4659 change */
        if(PNULL == ulUEContext_p) 
        {
            lteWarning("Invalid UE Index %d is not present in current system",
                    rntiInfo_p->index);
            LOG_MAC_MSG(MAC_SR_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
                    0/*coverity 97029 +-*/,\
                    recvRnti,\
                    ueIndex,\
                    recvTTI,0,0,\
                    0.0,0.0,__func__,"Invalid SR Request from UE");
            continue;
        }
        /* SPR 4659 change */
        /* Rel9_upgrade_CR410 : Start sr prohibit timer */
        if (ulUEContext_p->pucchConfigInfo.srProhibitTimerValv920)
        {
            ulUEContext_p->pucchConfigInfo.srProhibitExpiryTTI = recvTTI 
                + ulUEContext_p->pucchConfigInfo.srProhibitTimerValv920 
                - dlDelay ;
#ifdef LOG_PRINT_ENABLED
            LOG_MAC_MSG(MAC_SR_PROHIBIT_TIMER_ID,LOGDEBUG,MAC_L1_INF,\
                    0/*coverity 97029 +-*/,\
                    recvRnti,\
                    ueIndex,\
                    recvTTI,\
                    ulUEContext_p->pucchConfigInfo.srProhibitTimerValv920,\
                    ulUEContext_p->pucchConfigInfo.srProhibitExpiryTTI,\
                    0.0,0.0,__func__,"SR_PROHIBIT_TIMER_STARTED");
#endif
        }
        /* Rel9_upgrade_CR410 */ 
        /*CA Changes start  */
        putEntryInSRQueue(ueIndex, recvTTI, internalCellIndex);
        /*CA Changes end  */
    }
    return;
}
#ifndef DISABLE_UL_CQI_PROCESSING
/******************************************************************************
 * Function name : putSRSEntryInSINRReportQueue 
 * Inputs        : ueIndex - UE Identifier,
 *                 srsPdu - pointer to SRS PDU Indiacation Msg
 *                 recvTTI - Msg Recevied TTI
 *                 currentTTI - Current TTI
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : MAC_SUCCESS/MAC_FAILURE
 * DESCRIPTION   : This function will enqueue the SINR information received in 
 *                 the SRS report which will be processed by the execution leg
 *                 - ElProcessSINRReports.
 *****************************************************************************/
/* coverity-530 CID 25184 */
STATIC MacRetType putSRSEntryInSINRReportQueue(UInt16 ueIndex, 
        /* coverity-530 CID 25184 */
        FAPI_srsPduIndication_st *srsPdu,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        tickType_t currentTTI,
        /* SPR 15909 fix end */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    UInt8 idx = 0;
    SINRReportNode *sinrReportNode_p = NULL;
    GET_MEM_NODE_SINR_REPORT_Q(sinrReportNode_p,SINRReportNode);
    /* coverity-530 CID 25184 */
    if(PNULL == sinrReportNode_p) 
    {
        return MAC_FAILURE;
    }
    /* coverity-530 CID 25184 */
    sinrReportNode_p->ueIndex    = ueIndex;
    sinrReportNode_p->reportType = SINR_REPORT_TYPE_SRS;
    sinrReportNode_p->startPRB   = srsPdu->rbStart;
    sinrReportNode_p->numOfPRB   = srsPdu->numOfRB;
    sinrReportNode_p->recvTTI    = recvTTI;

    if(srsPdu->numOfRB > 16)
    {
        memCpy(sinrReportNode_p->sinrArray,srsPdu->snr, srsPdu->numOfRB);
    }
    else
    {
        for(idx = 0; idx < srsPdu->numOfRB; idx++)
        {
            sinrReportNode_p->sinrArray[idx] = srsPdu->snr[idx];
        }
    }

#ifdef DL_UL_SPLIT
    /*CA Changes start  */
    if(! (ENQUEUE_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex] + ((currentTTI + MAC_TICK_UPDATED) % SINR_CONTAINER_SIZE), 
                    SINRReportNode,(void *)&(sinrReportNode_p->nodeAnchor))))
        /*CA Changes end  */
    {
        LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                getCurrentTick(),\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);

        FREE_MEM_NODE_SINR_REPORT_Q(sinrReportNode_p);
        /* coverity-530 CID 25184 */
        return MAC_FAILURE;   
        /* coverity-530 CID 25184 */
    } 
#else
    /*CA Changes start  */
    if(! (ENQUEUE_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex],
                    SINRReportNode,(void *)&(sinrReportNode_p->nodeAnchor))))
        /*CA Changes end  */
    {
        /* +- SPR 17777 */
        LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                getCurrentTick(),\
                __LINE__,currentTTI,0,0,0,\
                0.0,0.0,__FILE__,__func__);
        /* +- SPR 17777 */

        FREE_MEM_NODE_SINR_REPORT_Q(sinrReportNode_p);
        /* coverity-530 CID 25184 */
        return MAC_FAILURE;   
        /* coverity-530 CID 25184 */
    } 
#endif 
    LOG_UT(MAC_UL_CSI_PUT_SRS_SINR_IN_QUEUE_ID, LOGDEBUG, MAC_UL_CSI, currentTTI,\
            ueIndex, SINR_REPORT_TYPE_SRS, \
            sinrReportNode_p->startPRB, sinrReportNode_p->numOfPRB,\
            0, \
            0, 0, \
            __func__,"PUT_SRS_SINR_IN_QUEUE");
    /* coverity-530 CID 25184 */
    return MAC_SUCCESS;   
    /* coverity-530 CID 25184 */

}
/* ULA_UTP */
#ifdef FAPI_4_0_COMPLIANCE
/******************************************************************************
 * FUNCTION NAME : putSRSEntryInSINRReportQueueR9 
 * INPUTS        : UInt16 ueIndex:
 *                 FAPI_srsPduIndication_st *srsPdu:
 *                 recvTTI
 *                 currentTTI
 *                 internalCellIndex - Cell-Index at MAC
 * OUTPUTS       : void 
 * DESCRIPTION   : This function will enqueue the SINR information received in 
 *                 the SRS report which will be processed by the execution leg
 *                 - ElProcessSINRReports
 *****************************************************************************/
/* coverity-530 CID 25184 */
STATIC MacRetType putSRSEntryInSINRReportQueueR9(UInt16 ueIndex, 
        /* coverity-530 CID 25184 */
        FAPI_srsPduIndicationR9_st *srsPdu,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        tickType_t currentTTI,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
{
    UInt8 idx = 0;
    SINRReportNode *sinrReportNode_p = NULL;
    GET_MEM_NODE_SINR_REPORT_Q(sinrReportNode_p,SINRReportNode);
    /* coverity-530 CID 25184 */
    if(PNULL == sinrReportNode_p) 
    {
        return MAC_FAILURE;
    }
    /* coverity-530 CID 25184 */
    sinrReportNode_p->ueIndex    = ueIndex;
    sinrReportNode_p->reportType = SINR_REPORT_TYPE_SRS;
    sinrReportNode_p->startPRB   = srsPdu->rbStart;
    sinrReportNode_p->numOfPRB   = srsPdu->numOfRB;
    sinrReportNode_p->recvTTI    = recvTTI;

    if(srsPdu->numOfRB > 16)
    {
        memCpy(sinrReportNode_p->sinrArray,srsPdu->snr, srsPdu->numOfRB);
    }
    else
    {
        for(idx = 0; idx < srsPdu->numOfRB; idx++)
        {
            sinrReportNode_p->sinrArray[idx] = srsPdu->snr[idx];
        }
    }

#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
    /*CA Changes start  */
    if(! (ENQUEUE_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex] + ((currentTTI + MAC_TICK_UPDATED) % SINR_CONTAINER_SIZE), 
                    SINRReportNode,(void *)&(sinrReportNode_p->nodeAnchor))))
        /*CA Changes end  */
    {
        LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                getCurrentTick(),\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);

        FREE_MEM_NODE_SINR_REPORT_Q(sinrReportNode_p);
        /* coverity-530 CID 25184 */
        return MAC_FAILURE;   
        /* coverity-530 CID 25184 */
    } 
#else
    /*CA Changes start  */
    if(! (ENQUEUE_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex],
                    SINRReportNode,(void *)&(sinrReportNode_p->nodeAnchor))))
        /*CA Changes end  */
    {
        /* +- SPR 17777 */
        LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                getCurrentTick(),\
                __LINE__,currentTTI,0,0,0,\
                0.0,0.0,__FILE__,__func__);
        /* +- SPR 17777 */

        FREE_MEM_NODE_SINR_REPORT_Q(sinrReportNode_p);
        /* coverity-530 CID 25184 */
        return MAC_FAILURE;   
        /* coverity-530 CID 25184 */
    } 
#endif 
    LOG_UT(MAC_UL_CSI_PUT_SRS_SINR_IN_QUEUE_ID, LOGDEBUG, MAC_UL_CSI, currentTTI,\
            ueIndex, SINR_REPORT_TYPE_SRS, \
            sinrReportNode_p->startPRB, sinrReportNode_p->numOfPRB,\
            0, \
            0, 0, \
            __func__,"PUT_SRS_SINR_IN_QUEUE");
    /* coverity-530 CID 25184 */
    return MAC_SUCCESS;   
    /* coverity-530 CID 25184 */

}
/* ULA_UTP */

#endif /*DISABLE_UL_CQI_PROCESSING*/
#endif	
/* SPR 15322 fix start */
//code removed 
/*SPR 15322 fix end */
#ifdef FAPI_4_0_COMPLIANCE
/*****************************************************************************
 * Function Name  : processSRSIndicationR9
 * Inputs         : 1. VendorSpecificParam_p
 *                  2. srsMsg_p  - pointer to SRS message(Payload part).
 *                  3. recvTTI  - Its represent the tick of the message
 *                              recieved.
 *                  4. internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the SRS Indication message that is
 *                  received from PHY layer in case PHY Supports Release9 and
 *                  Later
 *****************************************************************************/
/* + TM7_8 Changes Start */
void processSRSIndicationR9(void* VendorSpecificParam_p,
        void * msg_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
{
    /* - TM7_8 Changes End */
    UInt8   tcrntiFlag = 0;
    UInt16  numUE = 0;
    /*  UInt16  tcRnti = 0; TDD Warning Fix */
    UInt16  recvRnti = 0;
    UInt16  ueIndex = 0;
    UInt16  timingAdvance = 0; /* SPR 2628 Changes */
    UInt32  snr_array_len = 0; /* ULA_UTP */
    RNTIInfo *rntiInfo_p = PNULL;
    FAPI_srsPduIndicationR9_st *srsPdu_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */
    FAPI_srsIndicationR9_st *srsMsg_p = (FAPI_srsIndicationR9_st *)msg_p;
    FAPI_VendorSpecificR9_st *vendorSpecific_p = ( FAPI_VendorSpecificR9_st *) VendorSpecificParam_p;
    /* + E_CID_5.3 */
    UInt16 timingAdvanceR9 = INVALID_TIMING_ADVANCE_R9_VALUE;
    UInt8 validReports = 0;
    ULUEContext *ulUEContext_p = PNULL;
    EcidReportParams  *eCidReportParams_p = PNULL;
    /* - E_CID_5.3 */


#ifndef DISABLE_UL_CQI_PROCESSING
#ifdef DL_UL_SPLIT
    /*CA Changes start  */
    UInt32 sinrReportQueueCount = COUNT_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex] + (currentTTI % SINR_CONTAINER_SIZE));
#else
    UInt32 sinrReportQueueCount = COUNT_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex]);
    /*CA Changes end  */
#endif
#endif /*DISABLE_UL_CQI_PROCESSING*/

    LTE_MAC_UT_LOG(LOG_INFO, RECV_INTF, "[%s] Entry. ",__func__);
    LTE_MAC_UT_LOG(LOG_INFO, RECV_INTF, "SRS indication received :\n");

    srsPdu_p = (FAPI_srsPduIndicationR9_st *)&(srsMsg_p->srsPduInfo[0]);

    LTE_MAC_UT_LOG(LOG_INFO, RECV_INTF, "SRS Indication :sfnsf = %d "
            "numOfUe = %d", srsMsg_p->sfnsf, srsMsg_p->numOfUe);


    for(numUE = 0; numUE < srsMsg_p->numOfUe; numUE++)
    {
        LTE_MAC_UT_LOG(LOG_INFO, RECV_INTF , "handle = %d  rnti = %d  "
                "dopplerEstimation = %d timingAdvance = %d numOfRB = %d "
                "rbStart = %d\n", srsPdu_p->handle, 
                srsPdu_p->rnti, srsPdu_p->dopplerEstimation,
                srsPdu_p->timingAdvance,srsPdu_p->numOfRB,
                srsPdu_p->rbStart);

        LOG_UT(MAC_SRS_UE_RECV_ID,LOGDEBUG,MAC_L1_INF,\
                currentTTI,\
                srsPdu_p->handle,\
                srsPdu_p->rnti,\
                srsPdu_p->timingAdvance,\
                srsPdu_p->numOfRB,\
                srsPdu_p->rbStart,\
                (UDouble32)srsMsg_p->numOfUe,0.0,__func__,"SRS_UE_RECVD_ID");

        recvRnti = MAC_PHY_CONVERT_16(srsPdu_p->rnti);
        rntiInfo_p = getUeIdxFromRNTIMap(recvRnti,internalCellIndex);
        if (PNULL == rntiInfo_p || FREERNTI == rntiInfo_p->rntiFlag)
        {
            lteWarning("processSRSIndicationR9:Recv Rnti %d is not Active at "
                    "Mac Layer\n", recvRnti);  

            /* SPR 20636 Changes Start */
            LOG_MAC_MSG(MAC_SRS_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
                    currentTTI ,\
                    recvRnti,\
                    (ADDR)rntiInfo_p,\
                    (rntiInfo_p ? rntiInfo_p->rntiFlag : 0), \
                    recvTTI,0,\
                    0.0,0.0,__func__,"SRS_RNTIERR");

            /* SPR 20636 Changes End */
            return ;
        }
        ueIndex = rntiInfo_p->index;
        dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
        if ( TCRNTI == rntiInfo_p->rntiFlag )
        {
            /*  tcRnti = rntiInfo_p->index; TDD Warning Fix */
            tcrntiFlag = TRUE;
        }
        else 
        {    
            ueIndex = rntiInfo_p->index;
            if ( dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag)
            {
                lteWarning("Pending Delete Flag True for UE Index %d ",
                        ueIndex);
                LOG_UT(MAC_SRS_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
                        currentTTI ,\
                        recvRnti,\
                        ueIndex,\
                        recvTTI,0,0,\
                        0.0,0.0,__func__,"SRS_UEDELETE");
                continue;

            }
        }
        if (TRUE != tcrntiFlag)
        {
            dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
            /* SPR 2628 Changes Start */
            timingAdvance = MAC_PHY_CONVERT_16(srsPdu_p->timingAdvance);
            if ( timingAdvance > MAC_TIMING_ADVANCE_VAL_MAX)
            {
                lteWarning("Invalid TA value = %d in SRS Indication\n", 
                        timingAdvance);
            }
            /*SPR_3061_3068_FIX*/
            else
            {
                handleTimingAdvance(dlUEContext_p, ueIndex, timingAdvance,internalCellIndex, getCurrentTickUl());
            }


            LOG_MAC_MSG(MAC_TA_INSERTION,LOGBRIEF,MAC_L1_INF,\
                    currentTTI,\
                    getCurrentTick(),dlUEContext_p->lastSyncDetectedTick,\
                    ueIndex,timingAdvance,\
                    DEFAULT_INT_VALUE,\
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                    FUNCTION_NAME,"LAST_SYNC_TICK");
            /* SPR 2628 Changes End */
        }

        /* FAPI2.1 Changes start */
        ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
        if((TRUE != ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag) && \
                (PNULL != ulUEContext_p))
        {
            eCidReportParams_p = &ulUEContext_p->eCidReportParams;
            timingAdvanceR9 = (MAX_TIMING_ADVANCE_R9_VALUE >=MAC_PHY_CONVERT_16(srsPdu_p->release9Param.timingAdvanceR9) )?\
                              MAC_PHY_CONVERT_16(srsPdu_p->release9Param.timingAdvanceR9):INVALID_TIMING_ADVANCE_R9_VALUE;
            eCidReportParams_p->type1TAValue = timingAdvanceR9;
            validReports |= (MAX_TIMING_ADVANCE_R9_VALUE >= timingAdvanceR9)? TRUE:FALSE;
        }
        else
        {
            continue;
        }
        /* FAPI2.1 Changes end */

        /*SPR_3061_3068_FIX*/
#ifndef DISABLE_UL_CQI_PROCESSING
        /* ULA_CHG */
        /* To prevent overloading of sinrReportQueue_g, not more than
         * MAC_MAX_SINR_REPORTS_QUEUE_LENGTH nodes should be present in
         * the queue at any time.
         */ 
        if (sinrReportQueueCount < MAC_MAX_SINR_REPORTS_QUEUE_LENGTH)
        {
            /* This function will enqueue the received SRS report which will
             * be processed by the execution leg- ElProcessSINRReports
             */
            /* +- SPR 17777 */
            if( MAC_FAILURE !=putSRSEntryInSINRReportQueueR9(ueIndex,srsPdu_p,recvTTI,
                        currentTTI,
                        internalCellIndex))
                /* +- SPR 17777 */
            {
                sinrReportQueueCount++;
                /* + SPR 21811 Fix */
                /* Set flag in UE context whenever First SRS Indication receievd for UE*/
                ulUEContext_p =  ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
                if ((PNULL != ulUEContext_p) && !(ulUEContext_p->firstSRSIndRecvd))
                {
                    ulUEContext_p->firstSRSIndRecvd = TRUE;
                }
                /* - SPR 21811 Fix */
            }
        }
        else
        {
            LOG_MAC_MSG(MAC_L1_INF_SINR_QUEUE_FULL_ID, LOGWARNING, MAC_L1_INF,\
                    currentTTI, \
                    ueIndex, recvTTI, 0, 0, 0, \
                    0, 0, \
                    __func__,"SINR_QUEUE_FULL");
            break;
        }
        /* ULA_CHG */
#endif /* DISABLE_UL_CQI_PROCESSING */
        /* ULA_UTP */
        snr_array_len = (srsPdu_p->numOfRB * sizeof(uint8_t));
        /* SPR 15322 fix start */
        srsPdu_p = (FAPI_srsPduIndicationR9_st*)((UInt8 *)srsPdu_p + 
                SRS_PDU_CONST_LEN_R9 + 
                snr_array_len);
        /* SPR 15322 fix end */

        snr_array_len = 0;
    }

    handleVendorSpecificInfoR9(vendorSpecific_p,&validReports,recvTTI,internalCellIndex);

    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Exit. ",__func__);
    return;
}

/* SS_S2 */
#endif /*DISABLE_UL_CQI_PROCESSING*/
/*****************************************************************************
 * Function Name  : processSRSIndication
 * Inputs         : vendorSpecific_p - pointer to vendor specific field,
 *                  srsMsg_p  - pointer to SRS message(Payload part)
 *                  recvTTI  - Its represent the tick of the message recieved.
 *                  internalCellIndex - Cell-Index at MAC
 *
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the SRS Indication message that is
 *                  received from PHY layer 
 *****************************************************************************/
/* + TM7_8 Changes Start */
void processSRSIndication(void* VendorSpecificParam_p,
        void * msg_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */                          
{
    /* - TM7_8 Changes End */
    UInt8   tcrntiFlag = 0;
    UInt16  numUE = 0;
    /*  UInt16  tcRnti = 0; TDD Warning Fix */
    UInt16  recvRnti = 0;
    UInt16  ueIndex = 0;
    SInt16  timingAdvance = 0; /* SPR 2628 Changes */
    UInt32  snr_array_len = 0; /* ULA_UTP */
    RNTIInfo *rntiInfo_p = PNULL;
    FAPI_srsPduIndication_st *srsPdu_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */
    /* + TM7_8 Changes Start */
    /* + TM7_8 Changes End */
    FAPI_srsIndication_st *srsMsg_p = (FAPI_srsIndication_st *)msg_p;

    FAPI_VendorSpecific_st *vendorSpecific_p=(FAPI_VendorSpecific_st*)VendorSpecificParam_p;
    /* + E_CID_5.3 */
    FAPI_UEMeasurementIndication_st *ueMeasInfoForUEs_p = PNULL;
    /* - E_CID_5.3 */

#ifndef DISABLE_UL_CQI_PROCESSING
    /* + SPR 21811 Fix */
    ULUEContext *ulUEContext_p = PNULL;       
    /* - SPR 21811 Fix */
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
    /*CA Changes start  */
    UInt32 sinrReportQueueCount = COUNT_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex] + (currentTTI % SINR_CONTAINER_SIZE));
#else
    UInt32 sinrReportQueueCount = COUNT_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex]);
    /*CA Changes end  */
#endif
#endif /*DISABLE_UL_CQI_PROCESSING*/
    LTE_MAC_UT_LOG(LOG_INFO, RECV_INTF, "[%s] Entry. ",__func__);
    LTE_MAC_UT_LOG(LOG_INFO, RECV_INTF, "SRS indication received :\n");

    srsPdu_p = (FAPI_srsPduIndication_st *)&(srsMsg_p->srsPduInfo[0]);

    LTE_MAC_UT_LOG(LOG_INFO, RECV_INTF, "SRS Indication :sfnsf = %d "
            "numOfUe = %d", srsMsg_p->sfnsf, srsMsg_p->numOfUe);

    for(numUE = 0; numUE < srsMsg_p->numOfUe; numUE++)
    {
        LTE_MAC_UT_LOG(LOG_INFO, RECV_INTF , "handle = %d  rnti = %d  "
                "dopplerEstimation = %d timingAdvance = %d numOfRB = %d "
                "rbStart = %d\n", srsPdu_p->handle, 
                srsPdu_p->rnti, srsPdu_p->dopplerEstimation,
                srsPdu_p->timingAdvance,srsPdu_p->numOfRB,
                srsPdu_p->rbStart);

        LOG_UT(MAC_SRS_UE_RECV_ID,LOGDEBUG,MAC_L1_INF,\
                currentTTI,\
                srsPdu_p->handle,\
                srsPdu_p->rnti,\
                srsPdu_p->timingAdvance,\
                srsPdu_p->numOfRB,\
                srsPdu_p->rbStart,\
                (UDouble32)srsMsg_p->numOfUe,0.0,__func__,"SRS_UE_RECVD_ID");

        recvRnti = MAC_PHY_CONVERT_16(srsPdu_p->rnti);
        /*CA Changes start  */
        rntiInfo_p = getUeIdxFromRNTIMap(recvRnti, internalCellIndex);
        /*CA Changes end  */
        if (PNULL == rntiInfo_p || FREERNTI == rntiInfo_p->rntiFlag)
        {
            lteWarning("processSRSIndication:Recv Rnti %d is not Active at "
                    "Mac Layer\n", recvRnti);  

            /* SPR 20636 Changes Start */
            LOG_MAC_MSG(MAC_SRS_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
                    currentTTI ,\
                    recvRnti,\
                    (ADDR)rntiInfo_p,\
                    (rntiInfo_p ? rntiInfo_p->rntiFlag : 0), \
                    recvTTI,0,\
                    0.0,0.0,__func__,"SRS_RNTIERR");

            /* SPR 20636 Changes End */
            /* SPR 9526 Fix */ 
            continue ;
            /* SPR 9526 Fix */ 
        }
        ueIndex = rntiInfo_p->index;
        dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
        if ( TCRNTI == rntiInfo_p->rntiFlag )
        {
            /*  tcRnti = rntiInfo_p->index; TDD Warning Fix */
            tcrntiFlag = TRUE;
        }
        else 
        {    
            ueIndex = rntiInfo_p->index;
            if ( dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag)
            {
                lteWarning("Pending Delete Flag True for UE Index %d ",
                        ueIndex);
                LOG_UT(MAC_SRS_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
                        currentTTI ,\
                        recvRnti,\
                        ueIndex,\
                        recvTTI,0,0,\
                        0.0,0.0,__func__,"SRS_UEDELETE");
                continue;

            }
        }
        if (TRUE != tcrntiFlag)
        {
            dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

            timingAdvance = MAC_PHY_CONVERT_16(srsPdu_p->timingAdvance);

#ifdef FLEXRAN
            timingAdvance = timingAdvance - MAC_TIMING_ADVANCE_DEFAULT_VAL + MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31;
#endif

            if ((timingAdvance > MAC_TIMING_ADVANCE_VAL_MAX) || (timingAdvance < 0))
            {
                lteWarning("Invalid TA value = %d in SRS Indication\n", 
                        timingAdvance);
            }
            /*SPR_3061_3068_FIX*/
            else
            {
                handleTimingAdvance(dlUEContext_p, ueIndex, (UInt16)timingAdvance, internalCellIndex, getCurrentTickUl());
            }
            LOG_MAC_MSG(MAC_TA_INSERTION,LOGBRIEF,MAC_L1_INF,\
                    currentTTI,\
                    getCurrentTick(),dlUEContext_p->lastSyncDetectedTick,\
                    ueIndex,timingAdvance,\
                    DEFAULT_INT_VALUE,\
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                    FUNCTION_NAME,"LAST_SYNC_TICK");
            /* SPR 2628 Changes End */
        }
        /*SPR_3061_3068_FIX*/
#ifndef DISABLE_UL_CQI_PROCESSING
        /* ULA_CHG */
        /* To prevent overloading of sinrReportQueue_g, not more than
         * MAC_MAX_SINR_REPORTS_QUEUE_LENGTH nodes should be present in
         * the queue at any time.
         */ 
        if (sinrReportQueueCount < MAC_MAX_SINR_REPORTS_QUEUE_LENGTH)
        {
            /* This function will enqueue the received SRS report which will
             * be processed by the execution leg- ElProcessSINRReports
             */
            /* coverity-530 CID 25184 */
            /*CA Changes start  */
            /* +- SPR 17777 */
            if(MAC_FAILURE != putSRSEntryInSINRReportQueue(ueIndex,srsPdu_p,recvTTI, 
                        currentTTI,
                        internalCellIndex))
                /* +- SPR 17777 */
                /*CA Changes end  */
                /* coverity-530 CID 25184 */
            {
                sinrReportQueueCount++;
                /* + SPR 21811 Fix */
                /* Set flag in UE context whenever First SRS Indication receievd for UE*/
                ulUEContext_p =  ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
                if ((PNULL != ulUEContext_p) && !(ulUEContext_p->firstSRSIndRecvd))
                {
                    ulUEContext_p->firstSRSIndRecvd = TRUE;
                }
                /* - SPR 21811 Fix */
            }
        }
        else
        {
            LOG_MAC_MSG(MAC_L1_INF_SINR_QUEUE_FULL_ID, LOGWARNING, MAC_L1_INF,\
                    currentTTI, \
                    ueIndex, recvTTI, 0, 0, 0, \
                    0, 0, \
                    __func__,"SINR_QUEUE_FULL");
            break;
        }
        /* ULA_CHG */
#endif /* DISABLE_UL_CQI_PROCESSING */
        /* ULA_UTP */
        snr_array_len = (srsPdu_p->numOfRB * sizeof(uint8_t));
        srsPdu_p = (FAPI_srsPduIndication_st*)((UInt8 *)srsPdu_p + 
                SRS_PDU_CONST_LEN +
                snr_array_len);
        snr_array_len = 0;
    }

    /* + E_CID_5.3 */
    if(PNULL != vendorSpecific_p)
    {
        ueMeasInfoForUEs_p = (FAPI_UEMeasurementIndication_st *)&
            vendorSpecific_p->ueMeasInfoForUEs;
        /* SPR 9829 changes start */
        macProcessVendorSpecificFieldsSRSCQIInd(ueMeasInfoForUEs_p,recvTTI,internalCellIndex);
        /* SPR 9829 changes end */
    }

    /* - E_CID_5.3 */
    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Exit. ",__func__);
    return;
}
/* SS_S2 */

/*****************************************************************************
 * Function Name  : processMeasReportIndication
 * Inputs         : measReportInd_p  - pointer to measurement report msg,
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the measurement report Indication 
 *                  message that is received from PHY layer.
 *****************************************************************************/
/*CA Changes start  */
void processMeasReportIndication(FAPI_measReportInd_st *measReportInd_p, InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    /* SPR 21038 start */
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /* SPR 21038 end */   
    /* updating the FAPI measurment report to the shared memory global structure */
    /* If time averaging is enabled then MAC will do the time averaging of the 
     * parameters coming from PHY using Infinite Impulse response(IIR) algorithm */
    if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportConfigParams.layer1ReportAveragingEnabled) 
    {
        /* SPR 21038 start */
        for(;count<cellParams_p->dlAvailableRBs;count++)
        /* SPR 21038 end   */
        {
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportInd.layer1ReportInfo_p->interferencePerPRB[count]= 
                ((TIME_AVG_COEFFICIENT_IIR)*(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportInd.layer1ReportInfo_p->interferencePerPRB[count])+ \
                 (1024-TIME_AVG_COEFFICIENT_IIR)*(measReportInd_p->interferencePerPRB[count]))/1024;

        }

        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportInd.layer1ReportInfo_p->thermalNoise =
            ((TIME_AVG_COEFFICIENT_IIR)*(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportInd.layer1ReportInfo_p->thermalNoise) + \
             (1024-TIME_AVG_COEFFICIENT_IIR)*(measReportInd_p->thermalNoise))/1024;

        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportInd.layer1ReportInfo_p->dlRsTxPower =
            ((TIME_AVG_COEFFICIENT_IIR)*(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportInd.layer1ReportInfo_p->dlRsTxPower) + \
             (1024-TIME_AVG_COEFFICIENT_IIR)*(measReportInd_p->dlRsTxPower))/1024;
    }

    else
    {
        /* SPR 21038 start */
        for(;count<cellParams_p->dlAvailableRBs;count++)
        /* SPR 21038 end   */
        {
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportInd.layer1ReportInfo_p->interferencePerPRB[count]=
                measReportInd_p->interferencePerPRB[count];
        }

        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportInd.layer1ReportInfo_p->thermalNoise = measReportInd_p->thermalNoise;
        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportInd.layer1ReportInfo_p->dlRsTxPower = measReportInd_p->dlRsTxPower;
    }
}
/* 4x4 DL MIMO CHG START */
/*****************************************************************************
 * Function Name  : isRIInCodebookSubsetRestriction
 * Inputs         : RI  - Received Rank Indicator from UE,
 *                  ueCategory - Category of the UE,
 *                  transMode  -  Transmission mode of UE 
 *                  cbsrValue  - Pointer of CBSR
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : Zero if wrong value received else non zero value.
 * Description    : This function validate the reporeted rank indicator value
 *                  with configured CBSR.
 ******************************************************************************/
static UInt16 isRIInCodebookSubsetRestriction( UInt8 RI,
        UInt8 ueCategory,
        UInt8 transMode,
        UInt8 *cbsrValue,
        InternalCellIndex internalCellIndex)
{
    UInt16 result  = 0;
    /* SPR 12108 Changes Start */
#ifdef CSI_VALIDATION
    /* SPR 10177 */
    if(TX_MODE_8 == transMode)
    {
        return MAC_SUCCESS;
    }
    /* For 4x4 and 4x2 MIMO */
    if(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
    {
        if (TX_MODE_4 == transMode)
        {
            if (RANK_INDICATOR_4 == RI)
            {
                result = cbsrValue[CBSR_IDX_0] + cbsrValue[CBSR_IDX_1];
            }
            else if (RANK_INDICATOR_3 == RI)
            {
                result = cbsrValue[CBSR_IDX_2] + cbsrValue[CBSR_IDX_3];
            }
            else if (RANK_INDICATOR_2 == RI)
            {
                result = cbsrValue[CBSR_IDX_4] + cbsrValue[CBSR_IDX_5];
            }
            else
            {
                result = cbsrValue[CBSR_IDX_6] + cbsrValue[CBSR_IDX_7];
            }
        }
        else if (TX_MODE_3 == transMode)
        {
            /* Here we nee to check the layer has been
             * supported or not */
            result = ((0x01 << (RI+SHIFT_BY_3_BIT))&cbsrValue[CBSR_IDX_0]);
        }
        /* SPR 10177 changes start */
        /* SPR 12240 Changes Start */
        else if (TX_MODE_8 == transMode)
        {
            if (RANK_INDICATOR_2 == RI)
            {
                result = cbsrValue[CBSR_IDX_0] + cbsrValue[CBSR_IDX_1];
            }
            else
            {
                result = cbsrValue[CBSR_IDX_2] + cbsrValue[CBSR_IDX_3];
            }
        }
        /* SPR 12240 Changes End */
        /* SPR 10177 changes end */
        else
        {
            /* errorlog for wrong transmission mode */
            LOG_MAC_MSG(MAC_RRC_TRANS_MODE_ID,LOGERROR,MAC_L1_INF,\
                    getCurrentTick(),ueCategory,transMode,\
                    RI,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                    __func__,"Wrong transmission mode Received");
        }
    }
    else if (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
    {
        /* SPR 12240 Changes Start */
        /* transmission mode 4 or 8 */
        if ((TX_MODE_4 == transMode) || 
                (TX_MODE_8 == transMode))
        {
            /* SPR 12240 Changes End */
            if (RANK_INDICATOR_2 == RI)
            {   /*Read bit 7 or 6 in cbsr*/
                result = (cbsrValue[CBSR_IDX_0] >> SHIFT_BY_6_BIT);
            }
            else
            {   /*Read bit 5, 4, 3 or 2 in cbsrValue*/
                result = ((cbsrValue[CBSR_IDX_0] & 0x3C) >> SHIFT_BY_2_BIT);
            }
        }
        else if (TX_MODE_3 == transMode) /* transmission mode 3 */
        {
            result = ((0x01 << (RI+SHIFT_BY_5_BIT))&cbsrValue[CBSR_IDX_0]);
        }
        else
        {
            /* wrong trans mode recieved*/
            LOG_MAC_MSG(MAC_RRC_TRANS_MODE_ID,LOGERROR,MAC_L1_INF,\
                    getCurrentTick(),ueCategory,transMode,\
                    RI,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                    __func__,"Wrong transmission mode Received");
        }
    }
    else
    {
        /* errorlog for wrong antenna configuration */
        LOG_MAC_MSG(MAC_RRC_NUM_ANT_ID,LOGERROR,MAC_L1_INF,\
                getCurrentTick(),ueCategory,transMode,\
                RI,cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
                ->cellParams_p->numOfTxAnteenas,DEFAULT_INT_VALUE,\
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                __func__,"Wrong Antenna Configuration Received");
    }
#else
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(ueCategory)
  LTE_GCC_UNUSED_PARAM(transMode)
  LTE_GCC_UNUSED_PARAM(cbsrValue)
  LTE_GCC_UNUSED_PARAM(internalCellIndex)
  LTE_GCC_UNUSED_PARAM(RI)
    /* +- SPR 17777 */
    result = 1;
#endif
    /* SPR 12108 Changes End */
    return result;
}
/* 4x4 DL MIMO CHG END */

/*****************************************************************************
 * Function Name  : decodeAperiodicCqiMode1_2
 * Inputs         : msgBuffer - pointer to PDU recieved,
 *                  ueIndex   - ueIndex of UE,
 *                  rank      - rank Indicator,
 *                  ulUEContext_p - pointer to the UL UE context 
 *                  recvTTI  - Its represent the tick of the message recieved.
 *                  internalCellIndex - Cell-Index at MAC
 *                  numOfBitsDecoded_p
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the decoding of aperiodic 
 *                  PDU of mode 1_2. 
 *****************************************************************************/
void decodeAperiodicCqiMode1_2(UInt8 *messageBuffer_p, 
        UInt16 ueIndex, 
        UInt8 rank,
        ULUEContext * ulUEContext_p,
        /* + CQI_5.0 */
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex,
         /* CA_phase2_csi_code */
        UInt8 *numOfBitsDecoded_p)
/*CA Changes end  */
/* - CQI_5.0 */
{
    UInt8 widebandCQIValue = 0;
    UInt8 widebandCQIValue1 = 0;
    UInt8 numberOfBytes = 0;
    UInt8 numberOfBits = 0;
    UInt8 N = 0;
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UEReportedCQINodeInfo *ueReportedCQINodeInfo_p = PNULL;
    /* CA_phase2_csi_code */
    *numOfBitsDecoded_p = 0;

    /* + CQI_4.1 */
    /*SPR 21022 fix start*/
    widebandCQIValue = GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
    /*SPR 21022 fix end*/
    widebandCQIValue = ((GET_4_BIT_SWAPPED_VALUE(widebandCQIValue))
            - DL_ADAPTATION_CORRECTION);

#ifdef PERF_STATS
    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
        totalCQICW1Reported[widebandCQIValue]++;
    if(rank > 0) /*CID 97505*/
    {
	    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
        	totalRankReported[rank - 1]++;
    }
#endif


    /* KPI changes start */
#ifdef KPI_STATS
    LteMacKPIStats  *pLteMacKPIStats       = PNULL;
    UInt32 activeIndexForMacStats = 0;

    /* + KPI_CA */
    activeIndexForMacStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
    pLteMacKPIStats = &(gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
    /* - KPI_CA */
    pLteMacKPIStats->totalCQICW1Reported[widebandCQIValue]++;
#endif
    /* KPI changes end */

    /* SPR 14353 Changes Start */
    VALIDATE_CQI_VALUE_FOR_MIN(widebandCQIValue,PNULL); 
    /* SPR 14353 Changes End */

    GET_MEM_FROM_POOL(UEReportedCQINodeInfo,ueReportedCQINodeInfo_p,
            sizeof(UEReportedCQINodeInfo),PNULL);

    /* Rel 5.3: Coverity 24437 Fix Start */
    if (PNULL == ueReportedCQINodeInfo_p)
    {
        /* Not able to allocate Memory */
        return;
    }
    /* Rel 5.3: Coverity Fix End */
    ueReportedCQINodeInfo_p->ueIndex = ueIndex;
    ueReportedCQINodeInfo_p->mode = APERIODIC_CQI_MODE_1_2;

    AperiodicCQIMode12 *aperiodicCQIMode12_p =
        &(ueReportedCQINodeInfo_p->cqiNode.aperiodicCQIMode12);
    aperiodicCQIMode12_p->widebandCQIValueCodeword0 = widebandCQIValue;
    /* CA_phase2_csi_code */ 
    *numOfBitsDecoded_p = *numOfBitsDecoded_p + WIDEBAND_BITS;

    N = cellParams_p->N;

    if (rank >1)
    {    
        widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
        widebandCQIValue1 = ((GET_4_BIT_SWAPPED_VALUE(widebandCQIValue1)) 
                - DL_ADAPTATION_CORRECTION); 

#ifdef PERF_STATS
        gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
            totalCQICW2Reported[widebandCQIValue1]++;
#endif
  /* KPI changes start */
#ifdef KPI_STATS
        pLteMacKPIStats->totalCQICW2Reported[widebandCQIValue1]++;
#endif
        /* KPI changes end */
        /* SPR 14353 Changes Start */
        VALIDATE_CQI_VALUE_FOR_MIN(widebandCQIValue1,ueReportedCQINodeInfo_p); 
        /* SPR 14353 Changes End */


        LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                getCurrentTick(),ueIndex,widebandCQIValue,widebandCQIValue1,\
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,__func__,"WB_BOTH_CW");

        messageBuffer_p = messageBuffer_p + 1;
        /* CA_phase2_csi_code */ 
        *numOfBitsDecoded_p = *numOfBitsDecoded_p + WIDEBAND_BITS;
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        {
            if (N > NUMBER_OF_BITS_IN_1BYTE)
            {
                N = NUMBER_OF_BITS_IN_1BYTE;
            }


            /* Fetch and fill PMI values from first byte i.e. subband 0 to N*/
            getPMIValueCW1ForMode1_2(messageBuffer_p,
                    0,
                    N,
                    aperiodicCQIMode12_p->pmiInfo12
                    );

            if (cellParams_p->N > NUMBER_OF_BITS_IN_1BYTE)
            {
                messageBuffer_p++;

                /* Fetch and fill PMI values from next byte i.e. subband 8 onwards */
                getPMIValueCW1ForMode1_2(messageBuffer_p,
                        N,
                        cellParams_p->N,
                        &(aperiodicCQIMode12_p->pmiInfo12[N])
                        );
            }
             /* CA_phase2_csi_code start*/
            /* According to 36.212 bitwidth for RI>1 NumOFTxAntenna_2 = N */
            *numOfBitsDecoded_p = *numOfBitsDecoded_p + cellParams_p->N;
             /* CA_phase2_csi_code end*/
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            /* bytes to move (4N)/8 ; */
            /* numberOfBytes = ((4*(cellParams_p->N))/8);*/
            numberOfBytes = 
                ((MULTIPLY_BY_FOUR(N))/ NUMBER_OF_BITS_IN_1BYTE);

            /* bits to move ahead of bytes (4N)%8 */
            /* numberOfBits = 
               ((4*(cellParams_p->N)%8);*/
            numberOfBits = 
                ((MULTIPLY_BY_FOUR(N))% NUMBER_OF_BITS_IN_1BYTE);

            getPMIValueCWForAnt4Mode1_2(messageBuffer_p, 
                    numberOfBytes,
                    numberOfBits,
                    /* +- SPR 17777 */
                    rank,
                    aperiodicCQIMode12_p->pmiInfo12
                    );
             /* CA_phase2_csi_code start*/
            /* According to 36.212 bitwidth for RI>1 NumOFTxAntenna_2 = 4N */
            *numOfBitsDecoded_p = *numOfBitsDecoded_p + (4 * (cellParams_p->N));
             /* CA_phase2_csi_code end*/
        }
    }
    else
    {
        if((cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_1) || \
                (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2))
        {

            /* bytes to move (2N-4)/8 ; */
            /* numberOfBytes = ((2*(cellParams_p->N) - 4)/8);*/
            numberOfBytes = 
                ((MULTIPLY_BY_TWO(N) - WIDEBAND_BITS)/ NUMBER_OF_BITS_IN_1BYTE);

            /* bits to move ahead of bytes (2N-4)%8 */
            /* numberOfBits = 
               ((2*(cellParams_p->N) -4)%8);*/
            numberOfBits = 
                ((MULTIPLY_BY_TWO(N) - WIDEBAND_BITS)% NUMBER_OF_BITS_IN_1BYTE);

            getPMIValueCW0ForMode1_2(messageBuffer_p, 
                    numberOfBytes,
                    numberOfBits,
                    N,
                    aperiodicCQIMode12_p->pmiInfo12
                    );
             /* CA_phase2_csi_code start*/
            /* According to 36.212 Sec 5.2.2.6.1 bitwidth for RI = 1 
             * NumOFTxAntenna_2 = 2N */
            *numOfBitsDecoded_p = *numOfBitsDecoded_p + (2 * cellParams_p->N);
             /* CA_phase2_csi_code end*/

        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            /* bytes to move (4N-4)/8 ; */
            /* numberOfBytes = ((4*(cellParams_p->N) - 4)/8);*/
            numberOfBytes = 
                ((MULTIPLY_BY_FOUR(N) - WIDEBAND_BITS)/ NUMBER_OF_BITS_IN_1BYTE);

            /* bits to move ahead of bytes (2N-4)%8 */
            /* numberOfBits = 
               ((4*(cellParams_p->N) -4)%8);*/
            numberOfBits = 
                ((MULTIPLY_BY_FOUR(N) - WIDEBAND_BITS)% NUMBER_OF_BITS_IN_1BYTE);

            getPMIValueCWForAnt4Mode1_2(messageBuffer_p, 
                    numberOfBytes,
                    numberOfBits,
                    /* +- SPR 17777 */
                    rank,
                    aperiodicCQIMode12_p->pmiInfo12
                    );
            /* According to 36.212 Sec 5.2.2.6.1 bitwidth for RI = 1 
             * NumOFTxAntenna_4 = 4N */
            *numOfBitsDecoded_p = *numOfBitsDecoded_p + (4 * cellParams_p->N);
             /* CA_phase2_csi_code end*/
        }

        LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                getCurrentTick(),ueIndex,widebandCQIValue,DEFAULT_INT_VALUE,\
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,__func__,"WB_CW0");

    }
    aperiodicCQIMode12_p->widebandCQIValueCodeword0 = widebandCQIValue;
    aperiodicCQIMode12_p->widebandCQIValueCodeword1 = widebandCQIValue1;
    aperiodicCQIMode12_p->numberOfSubbands = cellParams_p->N;
    /* + CQI_5.0 */
    aperiodicCQIMode12_p->receivedRI = rank;
    aperiodicCQIMode12_p->reportReceivedTTI = recvTTI;
    /* - CQI_5.0 */

    /* SPR 99999 CQI with 255 UE Fix start */
    UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_CQI_RECEIVED(ueReportedCQINodeInfo_p,internalCellIndex)
    /* SPR 23483 Changes Start */
    if(CIRC_FAILURE == ENQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo,
                /*CA Changes start  */
                &(ueReportedCQINodeInfo_p->nodeAnchor), internalCellIndex))
        /*CA Changes end  */
    /* SPR 23483 Changes End */
    {
        freeMemPool(ueReportedCQINodeInfo_p);
        ueReportedCQINodeInfo_p = NULL;	
        return;
    }
    /* SPR 99999 Fix end */


    /* CLPC_CHG */
    /* CA_phase2_csi_code Start */
    /* Since UL CA not supported so no need to update these values
     * in Scell context */
    if(ulUEContext_p->internalCellIndex == internalCellIndex)
    {
        ulUEContext_p->isDLWideBandCQIReportAvailable = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne = widebandCQIValue; 
        ulUEContext_p->dlWidebandCQICodeWordTwo = widebandCQIValue1;
    }
    /* CA_phase2_csi_code End */
    /* CLPC_CHG_END */
    return;
}

/*****************************************************************************
 * Function Name  : decodeAperiodicCqiMode2_0
 * Inputs         : msgBuffer  - pointer to PDU recieved
 *                  ueIndex    - ueIndex of UE
 *                  transmissionMode  - Transmission Mode
 *                  ri - rank Indicator and
 *                  ulUEContext_p - pointer to the UL UE context
 *                  internalCellIndex - Cell-Index at MAC
 *                  numOfBitsDecoded_p
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the decoding of aperiodic 
 *                  PDU of mode 2_0. 
 *****************************************************************************/
void decodeAperiodicCqiMode2_0(UInt8 *messageBuffer_p,
        UInt16 ueIndex,
        UInt8  transmissionMode,
        /* + CQI_5.0 */
        UInt8 ri, 
        /* - CQI_5.0 */
        ULUEContext * ulUEContext_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex,
        /* CA_phase2_csi_code */
        UInt8 *numOfBitsDecoded_p)
/*CA Changes end  */
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt8 widebandCQIValueCW0 = 0;
    /* + CQI_4.1 */
    UInt8 widebandCQIValueCW1 = 0;
    UInt8 differentialCQI = 0;
    UInt8 cqiValueCW0 = 0;
    UInt8 cqiValueCW1 = 0;
    UInt8 offsetLevel = 0;
    UInt32 combinatorialIndex = 0;
    UInt8 preferredSubbands = cellParams_p->preferredSubbands; 
    /* - CQI_4.1 */
    /* + CQI_5.0 */
    UInt8 isfreqSelectiveSchDLOn = cellParams_p->freqSelectiveSchDL;
    /* - CQI_5.0 */

    UEReportedCQINodeInfo *ueReportedCQINodeInfo_p = PNULL;
    /* CA_phase2_csi_code */
    *numOfBitsDecoded_p = 0;

    /* + CQI_4.1 */
    /*SPR 21022 fix start*/
    widebandCQIValueCW0 = GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
    /*SPR 21022 fix end*/ 
    widebandCQIValueCW0 = ((GET_4_BIT_SWAPPED_VALUE(widebandCQIValueCW0))
            - DL_ADAPTATION_CORRECTION);

#ifdef PERF_STATS
    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
        totalCQICW1Reported[widebandCQIValueCW0]++;
#endif 

    /* KPI changes start */
#ifdef KPI_STATS
    LteMacKPIStats  *pLteMacKPIStats       = PNULL;
    UInt32 activeIndexForMacStats = 0;

    /* + KPI_CA */
    activeIndexForMacStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
    pLteMacKPIStats = &(gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
    /* - KPI_CA */
    pLteMacKPIStats->totalCQICW1Reported[widebandCQIValueCW0]++;
#endif
    /* KPI changes end */

    /* SPR 14353 Changes Start */
    VALIDATE_CQI_VALUE_FOR_MIN(widebandCQIValueCW0,PNULL);
    /* SPR 14353 Changes End */

    GET_MEM_FROM_POOL(UEReportedCQINodeInfo,ueReportedCQINodeInfo_p,
            sizeof(UEReportedCQINodeInfo),PNULL);

    /* Rel 5.3: Coverity 24438 Fix Start */
    if (PNULL == ueReportedCQINodeInfo_p)
    {
        /* Not able to allocate Memory */
        return;
    }
    /* Rel 5.3: Coverity Fix End */

    ueReportedCQINodeInfo_p->ueIndex = ueIndex;
    /* CA_phase2_csi_code */ 
    *numOfBitsDecoded_p = *numOfBitsDecoded_p + WIDEBAND_BITS;

    /* + CQI_5.0 */
    if (isfreqSelectiveSchDLOn)
    {    
        differentialCQI = GET_2_BIT_SWAPPED_VALUE(((*messageBuffer_p) & 0x30 )>> 4);

        offsetLevel = offsetLevelForUESelectedSubbandMode_g[differentialCQI];

        /* Differential CQI offset level = 
         *       M selected subbands CQI index - wideband CQI index
         * from specs 36.213 sec 7.2 
         */      

        cqiValueCW0 = (widebandCQIValueCW0 + offsetLevel) - DL_ADAPTATION_CORRECTION;

        VALIDATE_CQI_VALUE_FOR_MAX(cqiValueCW0,widebandCQIValueCW0,offsetLevel); 

#ifdef PERF_STATS
        gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
            totalCQICW1Reported[cqiValueCW0]++;
#endif
        /* KPI changes start */
#ifdef KPI_STATS
        pLteMacKPIStats->totalCQICW1Reported[cqiValueCW0]++;
#endif
        /* KPI changes end */
      

        combinatorialIndex = getCombinatorialIndexFromL(messageBuffer_p,
                internalCellIndex);

        VALIDATE_COMBINATORIAL_INDEX(combinatorialIndex,ueReportedCQINodeInfo_p,
                internalCellIndex);
    }
    /*Cov_fix_6May_63552_Start*/
    /* CA_phase2_csi_code start */
    *numOfBitsDecoded_p = *numOfBitsDecoded_p + DIFFERENTIAL_SUBBAND_BITS;
    *numOfBitsDecoded_p = *numOfBitsDecoded_p + cellParams_p->LBitLabelForAperiodic;
    /* CA_phase2_csi_code end */
    /*Cov_fix_6May_63552_Stop*/
    /* - CQI_5.0 */

    /* if Tx is Open loop MIMO and ri > 1 , CQI values(both subband and wideband) 
     * for CW0 is copied to CW1 ref. 36.213 sec 7.2.1 
     */
    /*CID 97505 fix start */
    if (((TX_MODE_3 == transmissionMode)&& (0 < ri)) ||
    /*CID 97505 fix end */
            /* + TM7_8 Changes Start */
            ((1 < cellParams_p->numOfTxAnteenas)&&
             ((TX_MODE_7 == transmissionMode)||(TX_MODE_8 == transmissionMode))))
        /* - TM7_8 Changes End */
    {

#ifdef PERF_STATS
        /* Coverity 97505 Fix Start */
        if (0 < ri)
        {
            gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
                totalRankReported[ri - 1]++;
        }
        /* Coverity 97505 Fix End */
#endif 

        ueReportedCQINodeInfo_p->mode = APERIODIC_CQI_MODE_2_2;

        AperiodicCQIMode22 *aperiodicCQIMode22_p = 
            &(ueReportedCQINodeInfo_p->
                    cqiNode.aperiodicCQIMode22);

        aperiodicCQIMode22_p->widebandCQIValueCodeword0 = widebandCQIValueCW0;
        /* + CQI_5.0 */
        if( 1 < ri )
        {
            widebandCQIValueCW1 = widebandCQIValueCW0;
            cqiValueCW1 = cqiValueCW0;
        } 

        aperiodicCQIMode22_p->widebandCQIValueCodeword1 = widebandCQIValueCW1;
        aperiodicCQIMode22_p->receivedRI = ri;
        /* TDD SB_CQI */
        aperiodicCQIMode22_p->widebandPMIValue = INVALID_PMI_VALUE;
        /* TDD SB_CQI */

        if (isfreqSelectiveSchDLOn)
        {     
            aperiodicCQIMode22_p->cqiValueCodeword0 = cqiValueCW0;
            aperiodicCQIMode22_p->cqiValueCodeword1 = cqiValueCW1;
            /* TDD SB_CQI */
            aperiodicCQIMode22_p->subbandPMIValue = INVALID_PMI_VALUE;
            /* TDD SB_CQI */
            aperiodicCQIMode22_p->numberOfSubbands = preferredSubbands;

            getSubbandNum((aperiodicCQIMode22_p->subbandInfo22), 
                    combinatorialIndex,
                    preferredSubbands,
                    internalCellIndex);
        }

        LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                getCurrentTick(),ueIndex,widebandCQIValueCW0,widebandCQIValueCW1,\
                cqiValueCW0,cqiValueCW1,DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                __func__,"WB_SB_BOTH_CW");

    }
    else
    {    
        ueReportedCQINodeInfo_p->mode = APERIODIC_CQI_MODE_2_0;

        AperiodicCQIMode20 *aperiodicCQIMode20_p = 
            &(ueReportedCQINodeInfo_p->
                    cqiNode.aperiodicCQIMode20);

        aperiodicCQIMode20_p->widebandCQIValue = widebandCQIValueCW0;

        if (isfreqSelectiveSchDLOn)
        {
            aperiodicCQIMode20_p->subbandCQIValue = cqiValueCW0;
            aperiodicCQIMode20_p->numberOfSubbands = preferredSubbands;
            getSubbandNum((aperiodicCQIMode20_p->subbandInfo20), 
                    combinatorialIndex,
                    preferredSubbands,
                    internalCellIndex); 
        }

        LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                getCurrentTick(),ueIndex,widebandCQIValueCW0,DEFAULT_INT_VALUE,\
                cqiValueCW0,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,__func__,"WB_SB_CW0");
    }

    /* SPR 99999 CQI with 255 UE Fix start */
    UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_CQI_RECEIVED(ueReportedCQINodeInfo_p,internalCellIndex)
    /* SPR 23483 Changes Start */
    if(CIRC_FAILURE == ENQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo,
                /*CA Changes start  */
                &(ueReportedCQINodeInfo_p->nodeAnchor), internalCellIndex))
        /*CA Changes end  */
    /* SPR 23483 Changes End */
    {
        freeMemPool(ueReportedCQINodeInfo_p);
        ueReportedCQINodeInfo_p = NULL;	
        return;
    }
    /* SPR 99999 Fix end */

    /* - CQI_4.1 */


    /* CLPC_CHG - adding cqi value to ul ue context as well */
    /* CA_phase2_csi_code Start */
    /* Since UL CA not supported so no need to update these values
     * in Scell context */
    if(ulUEContext_p->internalCellIndex == internalCellIndex)
    {
        ulUEContext_p->isDLWideBandCQIReportAvailable = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne =  widebandCQIValueCW0;
        ulUEContext_p->dlWidebandCQICodeWordTwo =  widebandCQIValueCW1;
    }
    /* CLPC_CHG_END */
    /* - CQI_5.0 */
    return;
}

/*****************************************************************************
 * Function Name  : decodeAperiodicCqiMode2_2
 * Inputs         : messageBuffer_p  - pointer to PDU recieved,
 *                  ueIndex - Index of UE,
 *                  ri - rank Indicator 
 *                  ulUEContext_p - pointer to the UL UE context
 *                  internalCellIndex - Cell-Index at MAC
 *                  numOfBitsDecoded_p
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the decoding of aperiodic 
 *                  PDU of mode 2_2.
 *****************************************************************************/
void decodeAperiodicCqiMode2_2(UInt8 *messageBuffer_p, 
        UInt16 ueIndex,
        UInt8 ri,
        ULUEContext * ulUEContext_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex,
        /* CA_phase2_csi_code */
        UInt8 *numOfBitsDecoded_p)
/*CA Changes end  */
{
    UInt8 widebandCQIValue = 0;
    UInt8 widebandCQIValue1 = 0;
    /* + CQI_4.1 */
    UInt8 cqiValue0 = 0;
    UInt8 cqiValue1 = 0;
    UInt8 wbPmiValue = 0;
    UInt8 sbPmiValue = 0;
    UInt8 differentialCQI = 0;
    UInt8 offsetLevel = 0;
    UInt32 combinatorialIndex = 0;
    /* CA_phase2_csi_code */
    *numOfBitsDecoded_p = 0;

    /* - CQI_4.1 */
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /* + CQI_5.0 */
    UInt8 isfreqSelectiveSchDLOn = cellParams_p->freqSelectiveSchDL;
    /* - CQI_5.0 */
    UEReportedCQINodeInfo *ueReportedCQINodeInfo_p = PNULL;

    /* + CQI_4.1 */
        /*SPR 19992 Start*/
    /*SPR 21022 fix start*/ 
    widebandCQIValue = GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
    widebandCQIValue = ((GET_4_BIT_SWAPPED_VALUE(widebandCQIValue))
            - DL_ADAPTATION_CORRECTION);
    /*SPR 21022 fix end*/
        /*SPR 19992 End*/

#ifdef PERF_STATS
    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
        totalCQICW1Reported[widebandCQIValue]++;
    /* Coverity 97505 Fix Start */
    if (0 < ri)
    {
    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
        totalRankReported[ri - 1]++;
    }
    /* Coverity 97505 Fix End */

#endif 


    /* KPI changes start */
#ifdef KPI_STATS
    LteMacKPIStats  *pLteMacKPIStats       = PNULL;
    UInt32 activeIndexForMacStats = 0;

    /* + KPI_CA */
    activeIndexForMacStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
    pLteMacKPIStats = &(gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
    /* - KPI_CA */
    pLteMacKPIStats->totalCQICW1Reported[widebandCQIValue]++;
#endif
    /* KPI changes end */

    /* SPR 14353 Changes Start */
    VALIDATE_CQI_VALUE_FOR_MIN(widebandCQIValue,PNULL);
    /* SPR 14353 Changes End */

    GET_MEM_FROM_POOL(UEReportedCQINodeInfo,ueReportedCQINodeInfo_p,
            sizeof(UEReportedCQINodeInfo),PNULL);

    /* Rel 5.3: Coverity 24439 Fix Start */
    if (PNULL == ueReportedCQINodeInfo_p)
    {
        /* Not able to allocate Memory */
        return;
    }
    /* Rel 5.3: Coverity Fix End */

    ueReportedCQINodeInfo_p->ueIndex = ueIndex;
    ueReportedCQINodeInfo_p->mode = APERIODIC_CQI_MODE_2_2;

    AperiodicCQIMode22 *aperiodicCQIMode22_p =
        &(ueReportedCQINodeInfo_p->cqiNode.aperiodicCQIMode22);

    /* + CQI_5.0 */
    if (isfreqSelectiveSchDLOn)
    {
        differentialCQI = GET_2_BIT_SWAPPED_VALUE(((*messageBuffer_p) & 0x30 )>> 4);
        offsetLevel =  offsetLevelForUESelectedSubbandMode_g[differentialCQI];

        /* Differential CQI offset level = 
         *       M selected subbands CQI index - wideband CQI index
         * from specs 36.213 sec 7.2 
         */      

        cqiValue0 = (widebandCQIValue + offsetLevel) - DL_ADAPTATION_CORRECTION;
        VALIDATE_CQI_VALUE_FOR_MAX(cqiValue0,widebandCQIValue,offsetLevel);

#ifdef PERF_STATS
        gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
            totalCQICW1Reported[cqiValue0]++;
#endif
        /* KPI changes start */
#ifdef KPI_STATS
        pLteMacKPIStats->totalCQICW1Reported[cqiValue0]++;
#endif
        /* KPI changes end */
    }
    /* - CQI_5.0 */
    /* CA_phase2_csi_code start */
    /* for first codeword */
    /* 4 WB Bits + 2 Bits Sub. Diff CQI */
    *numOfBitsDecoded_p = WIDEBAND_BITS + DIFFERENTIAL_SUBBAND_BITS;
    /* CA_phase2_csi_code end */

    if (1 < ri)
    {    
        /*SPR 2311 Start*/
        widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
        messageBuffer_p = messageBuffer_p + 1;
        widebandCQIValue1 |= 
            GET_WIDEBAND_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) << 2;

        widebandCQIValue1 = ((GET_4_BIT_SWAPPED_VALUE(widebandCQIValue1)) 
                - DL_ADAPTATION_CORRECTION);
#ifdef PERF_STATS
        gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
            totalCQICW2Reported[widebandCQIValue1]++;
#endif


        /* KPI changes start */
#ifdef KPI_STATS
        LteMacKPIStats  *pLteMacKPIStats       = PNULL;
        UInt32 activeIndexForMacStats = 0;

        /* + KPI_CA */
        activeIndexForMacStats = (gMacStats.
                pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
        pLteMacKPIStats = &(gMacStats.
                pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
        /* - KPI_CA */
        pLteMacKPIStats->totalCQICW2Reported[widebandCQIValue1]++;
#endif
        /* KPI changes end */

        /* SPR 14353 Changes Start */
        VALIDATE_CQI_VALUE_FOR_MIN(widebandCQIValue1,ueReportedCQINodeInfo_p); 
        /* SPR 14353 Changes Start */

        /* + CQI_5.0 */
        if (isfreqSelectiveSchDLOn)
        {
            /*SPR 2311 End*/
            differentialCQI = 
                GET_2_BIT_SWAPPED_VALUE(((*messageBuffer_p) & 0x0C) >> 2);

            offsetLevel = offsetLevelForUESelectedSubbandMode_g[differentialCQI];

            /* Differential CQI offset level = 
             *       M selected subbands CQI index - wideband CQI index.
             * from specs 36.213 sec 7.2 
             */      
            cqiValue1 = (widebandCQIValue1 + offsetLevel) - DL_ADAPTATION_CORRECTION;

            VALIDATE_CQI_VALUE_FOR_MAX(cqiValue1,widebandCQIValue1,offsetLevel);

#ifdef PERF_STATS
            gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
                totalCQICW2Reported[cqiValue1]++;
#endif
            /* KPI changes start */
#ifdef KPI_STATS
            pLteMacKPIStats->totalCQICW2Reported[cqiValue1]++;
#endif
            /* KPI changes end */

            getCombinatorialIndexAndPMIForRIMoreThan1(messageBuffer_p,
                    &combinatorialIndex,
                    &wbPmiValue,
                    &sbPmiValue,
                    internalCellIndex);
            VALIDATE_COMBINATORIAL_INDEX(combinatorialIndex,ueReportedCQINodeInfo_p,
                    internalCellIndex);
        }
        else
        {
            messageBuffer_p = messageBuffer_p + ((DIFFERENTIAL_SUBBAND_BITS +
                        DIFFERENTIAL_SUBBAND_BITS + cellParams_p->LBitLabelForAperiodic) 
                    /(NUMBER_OF_BITS_IN_1BYTE)); /* 4x2 MIMO Changes*/
            getWBPMIForRIMoreThan1(messageBuffer_p,
                    &wbPmiValue,
                    internalCellIndex);
        }
    /* CA_phase2_csi_code start */
    /* for Second  codeword */
    /* 4 WB Bits + 2 Bits Sub. Diff CQI + L */
    *numOfBitsDecoded_p = *numOfBitsDecoded_p + WIDEBAND_BITS + 
        DIFFERENTIAL_SUBBAND_BITS + cellParams_p->LBitLabelForAperiodic;
    /* PMI bits */
    if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
    {
        /* 8 bit PMI */
        *numOfBitsDecoded_p = *numOfBitsDecoded_p + 8;
    }
    else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
    {
        /* 2 bit PMI */
        *numOfBitsDecoded_p = *numOfBitsDecoded_p + 2;
    }
    /* CA_phase2_csi_code end */

        LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                getCurrentTick(),ueIndex,widebandCQIValue,widebandCQIValue1,\
                cqiValue0,cqiValue1,DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                __func__,"WB_SB_BOTH_CW");

    } 
    else
    {    
        if (isfreqSelectiveSchDLOn)
        {    
            combinatorialIndex = getCombinatorialIndexFromL(messageBuffer_p,
                    internalCellIndex);

            VALIDATE_COMBINATORIAL_INDEX(combinatorialIndex,ueReportedCQINodeInfo_p,
                    internalCellIndex);

            /* Incrementing Buffer to get the exact position to extract PMI 
             *  buffer pointer needs to move by 
             *(4 wideband bits + 2 differential subband CQI bits + L bit value)/8
             */

            messageBuffer_p = messageBuffer_p + ((WIDEBAND_BITS + 
                        DIFFERENTIAL_SUBBAND_BITS + cellParams_p->LBitLabelForAperiodic)
                    / (NUMBER_OF_BITS_IN_1BYTE));
            getPMIForAperiodicMode22RI1(messageBuffer_p,&wbPmiValue,&sbPmiValue,
                    internalCellIndex);
        }
        else
        {
            messageBuffer_p = messageBuffer_p + ((WIDEBAND_BITS + 
                        DIFFERENTIAL_SUBBAND_BITS + cellParams_p->LBitLabelForAperiodic)
                    / (NUMBER_OF_BITS_IN_1BYTE)); /* 4x2 MIMO Changes */
            getWBPMIForAperiodicMode22RI1(messageBuffer_p,&wbPmiValue,
                    internalCellIndex);
        }  
       /* CA_phase2_csi_code start */
        /* update L bit and PMI bit */
    if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
    {
        /* 8 bit PMI */
        *numOfBitsDecoded_p = *numOfBitsDecoded_p + 
            cellParams_p->LBitLabelForAperiodic + 8;
    }
    else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
    {
        /* 4 bit PMI */
        *numOfBitsDecoded_p = *numOfBitsDecoded_p + 
            cellParams_p->LBitLabelForAperiodic + 4;
    }
    /* CA_phase2_csi_code end */

        LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                getCurrentTick(),ueIndex,widebandCQIValue,DEFAULT_INT_VALUE,\
                cqiValue0,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,__func__,"WB_SB_CW0");

    } 

    aperiodicCQIMode22_p->widebandCQIValueCodeword0 = widebandCQIValue;
    aperiodicCQIMode22_p->widebandCQIValueCodeword1 = widebandCQIValue1;
    aperiodicCQIMode22_p->widebandPMIValue = wbPmiValue;

    if (isfreqSelectiveSchDLOn)
    {
        aperiodicCQIMode22_p->cqiValueCodeword0 = cqiValue0;
        aperiodicCQIMode22_p->cqiValueCodeword1 = cqiValue1;
        aperiodicCQIMode22_p->subbandPMIValue = sbPmiValue;
        aperiodicCQIMode22_p->numberOfSubbands = cellParams_p->preferredSubbands;

        getSubbandNum((aperiodicCQIMode22_p->subbandInfo22), 
                combinatorialIndex,
                aperiodicCQIMode22_p->numberOfSubbands,
                internalCellIndex);
    }

    aperiodicCQIMode22_p->receivedRI = ri;
    /* - CQI_5.0 */

    /* SPR 99999 CQI with 255 UE Fix start */
    UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_CQI_RECEIVED(ueReportedCQINodeInfo_p,internalCellIndex)
    /* SPR 23483 Changes Start */
    if(CIRC_FAILURE == ENQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo,
                /*CA Changes start  */
                &(ueReportedCQINodeInfo_p->nodeAnchor), internalCellIndex))
        /*CA Changes end  */
    /* SPR 23483 Changes End */
    {
        freeMemPool(ueReportedCQINodeInfo_p);
        return;
    }
    /* SPR 99999 Fix end */


    /* -  CQI_4.1 */

    /* CLPC_CHG */
    /* CA_phase2_csi_code Start */
    /* Since UL CA not supported so no need to update these values
     * in Scell context */
    if(ulUEContext_p->internalCellIndex == internalCellIndex)
    {
        ulUEContext_p->isDLWideBandCQIReportAvailable = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne = widebandCQIValue;
        ulUEContext_p->dlWidebandCQICodeWordTwo = widebandCQIValue1;
    }
    /* CA_phase2_csi_code End */
    /* CLPC_CHG_END */
    return;
}

/*****************************************************************************
 * Function Name  : decodeAperiodicCqiMode3_0
 * Inputs         : messageBuffer_p - pointer to PDU recieved,
 *                  ueIndex    - ueIndex of UE,
 *                  transmissionMode  - Transmission Mode,
 *                  ri - Rank Indicator, 
 *                  ulUEContext_p - pointer to the UL UE context
 *                  internalCellIndex - Cell-Index at MAC
 *                  numOfBitsDecoded_p
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the decoding of aperiodic 
 *                  PDU of mode 3_0 or mode 3_1 if ri == 1.
 *****************************************************************************/
void decodeAperiodicCqiMode3_0(UInt8 *messageBuffer_p,
        UInt16 ueIndex,
        UInt8 transmissionMode,
        /* + CQI_5.0 */
        UInt8 ri,
        /* - CQI_5.0 */
        ULUEContext *ulUEContext_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex,
         /* CA_phase2_csi_code */
        UInt8 *numOfBitsDecoded_p)
/*CA Changes end  */
{
    UInt32 spatialDifferentialCqiForCW0 = 0;
    UInt8 numberOfBytes = 0;
    UInt8 numberOfBits = 0;
    UInt8 widebandCQIValue = 0;
    /* + CQI_5.0 */
    UInt8 widebandCQIValue1 = 0;
    /* - CQI_5.0 */
    UInt8 N = 0;
    UEReportedCQINodeInfo *ueReportedCQINodeInfo_p = PNULL;
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt8 isfreqSelectiveSchDLOn =  cellParams_p->freqSelectiveSchDL;
    /* CA_phase2_csi_code */
    *numOfBitsDecoded_p = 0;
    /* + CQI_4.1 */
     /*SPR 21022 fix start*/ 
    widebandCQIValue = GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
    widebandCQIValue = ((GET_4_BIT_SWAPPED_VALUE(widebandCQIValue))
            - DL_ADAPTATION_CORRECTION);
     /*SPR 21022 fix end*/ 

#ifdef PERF_STATS
    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
        totalCQICW1Reported[widebandCQIValue]++;
#endif

    /* KPI changes start */
#ifdef KPI_STATS
    LteMacKPIStats  *pLteMacKPIStats       = PNULL;
    UInt32 activeIndexForMacStats = 0;

    /* + KPI_CA */
    activeIndexForMacStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
    pLteMacKPIStats = &(gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
    /* - KPI_CA */
    pLteMacKPIStats->totalCQICW1Reported[widebandCQIValue]++;
#endif
    /* KPI changes end */

    /* SPR 14353 Changes Start */
    VALIDATE_CQI_VALUE_FOR_MIN(widebandCQIValue,PNULL);
    /* SPR 14353 Changes End */
    /* CA_phase2_csi_code */ 
    *numOfBitsDecoded_p = *numOfBitsDecoded_p + WIDEBAND_BITS;

    GET_MEM_FROM_POOL(UEReportedCQINodeInfo,ueReportedCQINodeInfo_p,
            sizeof(UEReportedCQINodeInfo),PNULL);

    /* Rel 5.3: Coverity 24440 Fix Start */
    if (PNULL == ueReportedCQINodeInfo_p)
    {
        /* Not able to allocate Memory */
        return;
    }
    /* Rel 5.3: Coverity Fix End */
    ueReportedCQINodeInfo_p->ueIndex = ueIndex;

    /* CA_phase2_csi_code */ 
    *numOfBitsDecoded_p = *numOfBitsDecoded_p + (2 * (cellParams_p->N));

    /* + CQI_5.0 */
    if (isfreqSelectiveSchDLOn)
    {    
        N = cellParams_p->N;

        /* subband value 2N */
        /* bits to move 2N-4 */
        /* bytes to move (2N-4)/8 ; */
        /* numberOfBytes = ((2*(cellParams_p->N) - 4)/8);*/
        numberOfBytes = ((MULTIPLY_BY_TWO(N) - WIDEBAND_BITS) /
                (NUMBER_OF_BITS_IN_1BYTE));

        /* bits to move ahead of bytes (2N-4)%8 */
        /* numberOfBits = 
           ((2*(cellParams_p->N) -4)%8);*/
        numberOfBits = ((MULTIPLY_BY_TWO(N) - WIDEBAND_BITS) % 
                (NUMBER_OF_BITS_IN_1BYTE));

        spatialDifferentialCqiForCW0 = getSubbandDifferentialCqiCW0(messageBuffer_p,
                numberOfBytes,
                numberOfBits);
    }
    /* - CQI_5.0 */

    /* if TM is open loop MIMO and ri > 1 , CQI values(both subband and wideband) 
     * for CW0 is copied to CW1 ref. 36.213 sec 7.2.1 
     */
    if ((TX_MODE_3 == transmissionMode)||
            /* + TM7_8 Changes Start */
            ((1 < cellParams_p->numOfTxAnteenas)&&
             ((TX_MODE_7 == transmissionMode)||(TX_MODE_8 == transmissionMode))))
        /* - TM7_8 Changes End */
    {
#ifdef PERF_STATS
		/* Coverity 97505 Fix Start */
		if (0 < ri)
		{
			gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
				totalRankReported[ri - 1]++;
		}
		/* Coverity 97505 Fix End */
#endif
        ueReportedCQINodeInfo_p->mode = APERIODIC_CQI_MODE_3_1;

        AperiodicCQIMode31 *aperiodicCQIMode31_p = 
            &(ueReportedCQINodeInfo_p->
                    cqiNode.aperiodicCQIMode31);

        aperiodicCQIMode31_p->widebandCQIValueCodeword0 = widebandCQIValue;
        /* + CQI_5.0 */
        if (1 < ri)
        {    
            widebandCQIValue1 = widebandCQIValue; 

        }   

        aperiodicCQIMode31_p->widebandCQIValueCodeword1 = widebandCQIValue1;
        aperiodicCQIMode31_p-> receivedRI = ri;
        aperiodicCQIMode31_p->widebandPMI = 0;

        if (isfreqSelectiveSchDLOn)
        {
            aperiodicCQIMode31_p->numberOfSubbands = N;

            /* + SPR_4991 */
            if ( MAC_FAILURE == getSubbandCqiValueMode31(spatialDifferentialCqiForCW0,
                        spatialDifferentialCqiForCW0,
                        ueReportedCQINodeInfo_p,
                        ri
#ifdef PERF_STATS
                        ,transmissionMode
#endif                                  
                        )  
               ) 
            {
                return;
            }

            /* - SPR_4991 */
        }

        LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                getCurrentTick(),ueIndex,widebandCQIValue,widebandCQIValue,\
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,__func__,"WB_SB_BOTH_CW");

    }    
    else
    {
        ueReportedCQINodeInfo_p->mode = APERIODIC_CQI_MODE_3_0;

        AperiodicCQIMode30 *aperiodicCQIMode30_p = &(ueReportedCQINodeInfo_p->
                cqiNode.aperiodicCQIMode30);

        aperiodicCQIMode30_p->widebandCQIValue = widebandCQIValue;

        if (isfreqSelectiveSchDLOn)
        {
            aperiodicCQIMode30_p->numberOfSubbands = N;  

            /* + SPR_4991 */
            if (MAC_FAILURE  ==  getSubbandCqiValue(spatialDifferentialCqiForCW0,
                        ueReportedCQINodeInfo_p))
            {
                return;
            }

            /* - SPR_4991 */
        }

        LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                getCurrentTick(),ueIndex,widebandCQIValue,DEFAULT_INT_VALUE,\
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,__func__,"WB_SB_CW0");

    }

    /* SPR 99999 CQI with 255 UE Fix start */
    UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_CQI_RECEIVED(ueReportedCQINodeInfo_p,internalCellIndex)
    /* SPR 23483 Changes Start */
    if(CIRC_FAILURE == ENQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo,
                /*CA Changes start  */
                &(ueReportedCQINodeInfo_p->nodeAnchor), internalCellIndex))
        /*CA Changes end  */
    /* SPR 23483 Changes End */
    {
        freeMemPool(ueReportedCQINodeInfo_p);
        return;
    }
    /* SPR 99999 Fix end */


    /* - CQI_4.1 */
    /*CLPC_CHG */
    /* CA_phase2_csi_code Start */
    /* Since UL CA not supported so no need to update these values
     * in Scell context */
    if(ulUEContext_p->internalCellIndex == internalCellIndex)
    {
        ulUEContext_p->isDLWideBandCQIReportAvailable = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne = widebandCQIValue;
        ulUEContext_p->dlWidebandCQICodeWordTwo = widebandCQIValue1;
    }
    /* CA_phase2_csi_code End */
    /* - CQI_5.0 */
    /* CLPC_CHG_END */
    return;
}

/*****************************************************************************
 * Function Name  : decodeAperiodicCqiMode3_1
 * Inputs         : messageBuffer_p - pointer to PDU recieved,
 *                  ueIndex - ueIndex of UE ,
 *                  ri - rank Indicator,
 *                  transmissionMode - mode of transmission, 
 *                  ulUEContext_p - pointer to Uplink UE Context
 *                  internalCellIndex - Cell-Index at MAC
 *                  numOfBitsDecoded_p
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the decoding of aperiodic 
 *                  PDU of mode 3_1.
 *****************************************************************************/
void decodeAperiodicCqiMode3_1(UInt8 *messageBuffer_p,
        UInt16 ueIndex, 
        UInt8 ri,
        /* + TM6_5.2 */
        UInt8 transmissionMode,
        /* - TM6_5.2 */
        ULUEContext * ulUEContext_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex,
         /* CA_phase2_csi_code */
        UInt8 *numOfBitsDecoded_p)
/*CA Changes end  */
{
    UInt8 widebandCQIValue = 0;
    UInt8 widebandCQIValue1 = 0;
    UInt8 numberOfBytes = 0;
    UInt8 numberOfBits = 0;
    /* + CQI_4.1 */
    UInt32 spatialDifferentialCqiForCW0 = 0;
    UInt32 spatialDifferentialCqiForCW1 = 0;    
    UInt8  pmiValue = 0;
    UInt8 N = 0;
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /* - CQI_4.1 */
    /* + CQI_5.0 */
    UInt8 isfreqSelectiveSchDLOn = cellParams_p->freqSelectiveSchDL; 
    /* - CQI_5.0 */

    UEReportedCQINodeInfo *ueReportedCQINodeInfo_p = PNULL;
    /* CA_phase2_csi_code */
    *numOfBitsDecoded_p = 0;

    /* + CQI_4.1 */
    /*SPR 21022 fix start*/
    widebandCQIValue = GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
    widebandCQIValue = ((GET_4_BIT_SWAPPED_VALUE(widebandCQIValue))
            - DL_ADAPTATION_CORRECTION);
    /*SPR 21022 fix end*/

#ifdef PERF_STATS
    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
        totalCQICW1Reported[widebandCQIValue]++;

    /* Coverity 97505 Fix Start */
    if (0 < ri)
    {
    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
        totalRankReported[ri - 1]++;
    }
    /* Coverity 97505 Fix End */
#endif


    /* KPI changes start */
#ifdef KPI_STATS
    LteMacKPIStats  *pLteMacKPIStats       = PNULL;
    UInt32 activeIndexForMacStats = 0;

    /* + KPI_CA */
    activeIndexForMacStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
    pLteMacKPIStats = &(gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
    /* - KPI_CA */
    pLteMacKPIStats->totalCQICW1Reported[widebandCQIValue]++;
#endif
    /* KPI changes end */

    /* SPR 14353 Changes Start */
    VALIDATE_CQI_VALUE_FOR_MIN(widebandCQIValue,PNULL);
    /* SPR 14353 Changes End */

    GET_MEM_FROM_POOL(UEReportedCQINodeInfo,ueReportedCQINodeInfo_p,
            sizeof(UEReportedCQINodeInfo),PNULL);

    /* Rel 5.3: Coverity 24441 Fix Start */
    if (PNULL == ueReportedCQINodeInfo_p)
    {
        /* Not able to allocate Memory */
        return;
    }
    /* Rel 5.3: Coverity Fix End */

    ueReportedCQINodeInfo_p->ueIndex = ueIndex;
    ueReportedCQINodeInfo_p->mode = APERIODIC_CQI_MODE_3_1;

    AperiodicCQIMode31 *aperiodicCQIMode31_p =
        &(ueReportedCQINodeInfo_p->cqiNode.aperiodicCQIMode31);

    N = cellParams_p->N;

    /* subband value 2N */
    /* bits to move 2N-4 */
    /* bytes to move (2N-4)/8 ; */
    /* numberOfBytes = ((2*(cellParams_p->N) - 4)/8);*/
    /* CA_phase2_csi_code */
    *numOfBitsDecoded_p = WIDEBAND_BITS + MULTIPLY_BY_TWO(N);
    numberOfBytes = 
        ((MULTIPLY_BY_TWO(N) - WIDEBAND_BITS)/(NUMBER_OF_BITS_IN_1BYTE));

    /* bits to move ahead of bytes (2N-4)%8 */
    /* numberOfBits = 
       ((2*(cellParams_p->N) -4)%8);*/
    numberOfBits = 
        ((MULTIPLY_BY_TWO(N) - WIDEBAND_BITS)%(NUMBER_OF_BITS_IN_1BYTE));

    /* + CQI_5.0 */
    if (isfreqSelectiveSchDLOn)
    {
        spatialDifferentialCqiForCW0 = getSubbandDifferentialCqiCW0(messageBuffer_p,
                numberOfBytes,
                numberOfBits);
    }
    /* SPR 6453 changes start */
    /* Incrementing Buffer to get the exact position to wideband CQI,
     * subband CQI for codeword 1 and PMI 
     * buffer pointer needs to move by 
     *(4 wideband bits + 2N subband value) / 8    
     */
    messageBuffer_p = messageBuffer_p + 
        ((WIDEBAND_BITS + MULTIPLY_BY_TWO(N)) / (NUMBER_OF_BITS_IN_1BYTE));
    /* - SPR 8517 Fix */
    /* SPR 6453 changes end */

    if (1 < ri)
    {    
        if (isfreqSelectiveSchDLOn)
        {
            getSubbandDiffCqiWidebandCqiPmiCW1Mode3_1(messageBuffer_p,
                    numberOfBytes,
                    numberOfBits,
                    &widebandCQIValue1,
                    &pmiValue,
                    &spatialDifferentialCqiForCW1,
                    internalCellIndex);
        }
        else
        {    
            getWidebandCqiPmiCW1Mode3_1(messageBuffer_p,
                    numberOfBytes,
                    numberOfBits,
                    &widebandCQIValue1,
                    &pmiValue,
                    internalCellIndex);
        }
        widebandCQIValue1 = ((GET_4_BIT_SWAPPED_VALUE(widebandCQIValue1))
                - DL_ADAPTATION_CORRECTION);
        /* - CQI_5.0 */
        /* CA_phase2_csi_code start */
        *numOfBitsDecoded_p = *numOfBitsDecoded_p + (WIDEBAND_BITS + MULTIPLY_BY_TWO(
                    cellParams_p->N)); 
        /* pmi bits */
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        {
            /* 1 bit PMI- ref spec 36.212 sec-5.2.2.6.2 */
           *numOfBitsDecoded_p = *numOfBitsDecoded_p + 1;
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            /* 4 bit PMI- ref spec 36.212 sec-5.2.2.6.2 */
           *numOfBitsDecoded_p = *numOfBitsDecoded_p + 4;
        }
        /* CA_phase2_csi_code end */

#ifdef PERF_STATS
        gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
            totalCQICW2Reported[widebandCQIValue1]++;
#endif

        /* KPI changes start */
#ifdef KPI_STATS
        pLteMacKPIStats->totalCQICW2Reported[widebandCQIValue1]++;
#endif
        /* KPI changes end */

        /* SPR 14353 Changes Start */
        VALIDATE_CQI_VALUE_FOR_MIN(widebandCQIValue1,ueReportedCQINodeInfo_p);
        /* SPR 14353 Changes End */

        LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                getCurrentTick(),ueIndex,widebandCQIValue,widebandCQIValue1,\
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,__func__,"WB_BOTH_CW");
    } 
    else
    {
        pmiValue = getPMIValueCW0ForMode31(messageBuffer_p,numberOfBits,
                internalCellIndex);

        if((cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_1) || \
                (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2))
        {
            pmiValue = GET_2_BIT_SWAPPED_VALUE(pmiValue);
            /* 2 bit PMI- ref spec 36.212 sec-5.2.2.6.2 */
           *numOfBitsDecoded_p = *numOfBitsDecoded_p + 2;
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            pmiValue = GET_4_BIT_SWAPPED_VALUE(pmiValue);
            /* 4 bit PMI- ref spec 36.212 sec-5.2.2.6.2 */
           *numOfBitsDecoded_p = *numOfBitsDecoded_p + 4;
        }
        LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                getCurrentTick(),ueIndex,widebandCQIValue,DEFAULT_INT_VALUE,\
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                __func__,"WB_SB_CW0");
    }

    aperiodicCQIMode31_p->widebandCQIValueCodeword0 = widebandCQIValue;
    aperiodicCQIMode31_p->widebandCQIValueCodeword1 = widebandCQIValue1;

    aperiodicCQIMode31_p->widebandPMI = pmiValue;

    /* +CQI_5.0 */
    if (isfreqSelectiveSchDLOn)
    {
        aperiodicCQIMode31_p->numberOfSubbands = N;
        /* + SPR_4991 */
        if ( MAC_FAILURE ==  getSubbandCqiValueMode31(spatialDifferentialCqiForCW0,
                    spatialDifferentialCqiForCW1,
                    ueReportedCQINodeInfo_p,
                    ri
#ifdef PERF_STATS
                    /* + TM6_5.2 */
                    ,transmissionMode 
                    /* - TM6_5.2 */
#endif                              
                    )
           )
        {
            return;
        }
    }
    /* - SPR_4991 */
    aperiodicCQIMode31_p-> receivedRI = ri;
    /* - CQI_5.0 */

    /* SPR 99999 CQI with 255 UE Fix start */
    UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_CQI_RECEIVED(ueReportedCQINodeInfo_p,internalCellIndex)
    /* SPR 23483 Changes Start */
    if(CIRC_FAILURE == ENQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo,
                /*CA Changes start  */
                &(ueReportedCQINodeInfo_p->nodeAnchor), internalCellIndex))
        /*CA Changes end  */
    /* SPR 23483 Changes End */
    {
        freeMemPool(ueReportedCQINodeInfo_p);
        return;
    }
    /* SPR 99999 Fix end */

    /* - CQI_4.1 */
    /* CLPC_CHG */
    /* CA_phase2_csi_code Start */
    /* Since UL CA not supported so no need to update these values
     * in Scell context */
    if(ulUEContext_p->internalCellIndex == internalCellIndex)
    {
        ulUEContext_p->isDLWideBandCQIReportAvailable = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne = widebandCQIValue;
        ulUEContext_p->dlWidebandCQICodeWordTwo = widebandCQIValue1;
    }
    /* CA_phase2_csi_code End */
    /* CLPC_CHG_END */
    return;

}

/*****************************************************************************
 * Function Name  : decodePeriodicCqiMode11Type2 
 * Inputs         : messageBuffer_p  - pointer to PDU recieved,
 *                  ueIndex - ueIndex of UE,
 *                  ri - rank indicator, 
 *                  ulUEContext_p - pointer to Uplink UE Context
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the decoding of 
 *                  Periodic PDU of Mode 1_1 Type 2.
 *****************************************************************************/
void decodePeriodicCqiMode11Type2(UInt8 *messageBuffer_p,
        UInt16 ueIndex,
        UInt8 ri,
        ULUEContext *ulUEContext_p,
        InternalCellIndex internalCellIndex)
{
    UInt8 widebandCQIValue = 0;
    SInt8 widebandCQIValue1 = 0;
    UInt8 differentialCQI = 0;
    SInt8 offsetLevel = 0;
    UInt8 widebandPMI = 0;
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UEReportedCQINodeInfo *ueReportedCQINodeInfo_p = PNULL;
        /*SPR 19992 Start*/
    widebandCQIValue = GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
        /*SPR 19992 End*/

    widebandCQIValue = ((GET_4_BIT_SWAPPED_VALUE(widebandCQIValue))
            - DL_ADAPTATION_CORRECTION);

#ifdef PERF_STATS
    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
        totalCQICW1Reported[widebandCQIValue]++;
#endif 


    /* KPI changes start */
#ifdef KPI_STATS
    LteMacKPIStats  *pLteMacKPIStats       = PNULL;
    UInt32 activeIndexForMacStats = 0;

    /* + KPI_CA */
    activeIndexForMacStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
    pLteMacKPIStats = &(gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
    /* - KPI_CA */
    pLteMacKPIStats->totalCQICW1Reported[widebandCQIValue]++;
#endif
    /* KPI changes end */

    /* SPR 14353 Changes Start */
    VALIDATE_CQI_VALUE_FOR_MIN(widebandCQIValue,PNULL);
    /* SPR 14353 Changes End */

    GET_MEM_FROM_POOL(UEReportedCQINodeInfo,ueReportedCQINodeInfo_p,
            sizeof(UEReportedCQINodeInfo),PNULL);

    /* Rel 5.3: Coverity  24444 Fix Start */
    if (PNULL == ueReportedCQINodeInfo_p)
    {
        /* Not able to allocate Memory */
        return;
    }
    /* Rel 5.3: Coverity Fix End */

    ueReportedCQINodeInfo_p->ueIndex = ueIndex;
    ueReportedCQINodeInfo_p->mode = PERIODIC_CQI_MODE_1_1_TYPE_2;

    PeriodicMode11Type2 *periodicMode11Type2_p =  
        &(ueReportedCQINodeInfo_p->cqiNode.periodicMode11Type2);
    if(1 < ri)
    {   
        /*Differential CQI is present in most significant 3 bits of lower nibble.
         * Source 5.2.3.3.1-2 in 36.212*/ 
        /* + SPR 1476*/
        /*SPR 2311 Start*/
        /*SPR 19992 Start*/
        differentialCQI = GET_3_BIT_SWAPPED_VALUE(((*messageBuffer_p) & 0x70 ) >> 4);
        /*SPR 19992 end*/
        /*SPR 2311 End*/
        /* - SPR 1476*/

        offsetLevel = offsetLevel_g[differentialCQI];

        /* Codeword 1 offset level = 
         *     wideband CQI index for codeword 0 -  widebandCQI index for CW 1.
         * from specs 36.213 sec 7.2 
         */

        widebandCQIValue1 = (widebandCQIValue - offsetLevel) 
            - DL_ADAPTATION_CORRECTION;

        /* SPR 11627, 11350 Fix Start */
        VALIDATE_CQI_VALUE(widebandCQIValue,
                widebandCQIValue1,
                offsetLevel,
                ueReportedCQINodeInfo_p);
        /* SPR 11627, 11350 Fix End */

#ifdef PERF_STATS       
        /*coverity 56639-code removed*/
        gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
            totalCQICW2Reported[widebandCQIValue1]++;
#endif 

#ifdef KPI_STATS
        LteMacKPIStats  *pLteMacKPIStats       = PNULL;
        UInt32 activeIndexForMacStats = 0;

        activeIndexForMacStats = (gMacStats.
                pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
        pLteMacKPIStats = &(gMacStats.
                pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
        pLteMacKPIStats->totalCQICW2Reported[widebandCQIValue1]++;
#endif
        if(2 == cellParams_p->numOfTxAnteenas)
        {
            /*SPR 2311 Start*/
        /*SPR 19992 Start*/
            widebandPMI = GET_VALUE_FROM_LAST_BIT(*messageBuffer_p);
        /*SPR 19992 End*/
            /*SPR 2311 End*/
        }
        else if(4 == cellParams_p->numOfTxAnteenas)
        {
            /*SPR 2311 Start*/
        /*SPR 19992 Start*/
            /*SPR 2311 Start*/
            widebandPMI = ((*((UInt16 *)messageBuffer_p))& 0x0780) >> 7;
            widebandPMI = GET_4_BIT_SWAPPED_VALUE(widebandPMI);
            /*SPR 2311 End*/
        /*SPR 19992 End*/
            /*SPR 2311 End*/
        }
        /* + CQI_4.1*/
        LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                getCurrentTick(),ueIndex,widebandCQIValue,widebandCQIValue1,\
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                __func__,"WB_BOTH_CW");
    }
    /* + SPR 1476*/
    else
    {
        /*SPR 2311 Start*/
	/*SPR 19992 Start*/
        if(2 == cellParams_p->numOfTxAnteenas)
        {
            widebandPMI = GET_2_BIT_SWAPPED_VALUE(
                    ((*messageBuffer_p) & 0x30) >> 4);
	/*SPR 19992 End*/
        }
        else if(4 == cellParams_p->numOfTxAnteenas)
        {
        /*SPR 19992 start*/
            widebandPMI = GET_4_BIT_SWAPPED_VALUE(
                    ((*messageBuffer_p) & 0xF0) >> 4);
        /*SPR 19992 End*/
        }
        else
        {
            ltePanic("Number of Anteenas is neither 2 nor 4\n");
        }
        /*SPR 2311 End*/
        LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                getCurrentTick(),ueIndex,widebandCQIValue,DEFAULT_INT_VALUE,\
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                __func__,"WB_CW0");
        /* - CQI_4.1*/
    }
    /* - SPR 1476*/
    periodicMode11Type2_p->widebandCQIValueCodeword0 = widebandCQIValue;
    periodicMode11Type2_p->widebandCQIValueCodeword1 = widebandCQIValue1;
    /* + CL-MIMO LJA*/
    periodicMode11Type2_p->widebandPMI = widebandPMI;

    /* SPR 99999 CQI with 255 UE Fix start */
    UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_CQI_RECEIVED(ueReportedCQINodeInfo_p,internalCellIndex)
    /* SPR 23483 Changes Start */
    if(CIRC_FAILURE == ENQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo,
                /*CA Changes start  */
                &(ueReportedCQINodeInfo_p->nodeAnchor), internalCellIndex))
        /*CA Changes end  */
    /* SPR 23483 Changes End */
    {
        freeMemPool(ueReportedCQINodeInfo_p);
			ueReportedCQINodeInfo_p = NULL;	
        return;
    }
    /* SPR 99999 Fix end */

    /* - CL-MIMO LJA*/
    /* CLPC_CHG */
    /* CA_phase2_csi_code Start */
    /* Since UL CA not supported so no need to update these values
     * in Scell context */
    if(ulUEContext_p->internalCellIndex == internalCellIndex)
    {
        ulUEContext_p->isDLWideBandCQIReportAvailable = TRUE;
        ulUEContext_p->dlWidebandCQICodeWordOne = widebandCQIValue; 
        ulUEContext_p->dlWidebandCQICodeWordTwo = widebandCQIValue1; 
    }
    /* CA_phase2_csi_code end */
    /* CLPC_CHG_END */ 

    return;
}

/* + CQI_5.0 */
/*****************************************************************************
 * Function Name  : decodePeriodicCqiMode11Type3 
 * Inputs         : messageBuffer_p - pointer to PDU recieved,
 *                  ueIndex - ueIndex of UE,
 *                  dlUEContext_p - pointer to Downlink UE Context 
 *                  ri_p - pointer to the ueReportedRI
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the decoding of Periodic PDU of 
 *                  Mode 1_1 Type 3.
 ****************************************************************************/
void decodePeriodicCqiMode11Type3(UInt8 *messageBuffer_p,
        UInt16 ueIndex, 
        DLUEContext *dlUEContext_p,
        UInt8 *ri_p,
        InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    UEReportedCQINodeInfo *ueReportedCQINodeInfo_p = PNULL;
    InternalCellIndex  servCellId = INVALID_CELL_INDEX;

    /* UE will report 1 bit so we will increment it by 1 to get exact RI value. 
     * Also, updating RI in UL context. */
    /** SPR 14505 changes **/
    if ( (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4) &&
            ((UE_CATEGORY_5 == dlUEContext_p->ueCategory) || (UE_CATEGORY_8 == dlUEContext_p->ueCategory) )
   /** SPR 14505 changes **/         
	)
    {
        *ri_p = (GET_2_BIT_SWAPPED_VALUE((*messageBuffer_p) & 0x03u)) + 1;
    }
    else
    {
/*SPR 19496 fix start*/
        *ri_p = (*(messageBuffer_p) & 0x01u) + 1;
/*SPR 19496 fix end*/
    }
    /* 4x4 DL MIMO CHG START */                                     
    /* SPR 10177 changes Start */
     /* CA_phase2_csi_code start */
    if(dlUEContext_p->internalCellIndex == internalCellIndex)
    {
        /* Pcell */
        if ((TX_MODE_8 != dlUEContext_p->dlMIMOInfo.transmissionMode) &&
        /* SPR 10177 changes End */
                !(isRIInCodebookSubsetRestriction( *ri_p,                   
                        dlUEContext_p->ueCategory,                      
                        dlUEContext_p->dlMIMOInfo.transmissionMode,     
                        dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue,
                        internalCellIndex))) 
            /* 4x4 DL MIMO CHG END */                                       
        {
            LOG_MAC_MSG(MAC_RECEIVED_RI_LOG_ID,LOGERROR,MAC_L1_INF,\
                    getCurrentTick(),ueIndex,dlUEContext_p->ueCategory,\
                    *ri_p,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                    __func__,"Invalid Periodic RI received for Mode11");
            return;
        }
    }
    else
    {
        /* Scell */
        servCellId = getServeCellIndexFromInternalCellId(dlUEContext_p,
                internalCellIndex);
        /*klockwork warning fix*/
        if(servCellId <= MAX_NUM_SCELL)
        {
            if (!(isRIInCodebookSubsetRestriction( *ri_p,                   
                            dlUEContext_p->ueCategory,                      
                            dlUEContext_p->dlUeScellContext_p[servCellId]->ScelldlMIMOInfo.transmissionMode,
                            dlUEContext_p->dlUeScellContext_p[servCellId]->
                            codebookSubsetRestrictionv1020.cbsrValue,
                            internalCellIndex))) 
                /* 4x4 DL MIMO CHG END */                                       
            {
                LOG_MAC_MSG(MAC_RECEIVED_RI_LOG_ID,LOGERROR,MAC_L1_INF,\
                        getCurrentTick(),ueIndex,dlUEContext_p->ueCategory,\
                        *ri_p,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                        __func__,"Invalid Periodic RI received for Mode11");
                return;
            }
        }
    }
    /* CA_phase2_csi_code end */
    GET_MEM_FROM_POOL(UEReportedCQINodeInfo,ueReportedCQINodeInfo_p,
            sizeof(UEReportedCQINodeInfo),PNULL);

    /* Rel 5.3: Coverity 24445 Fix Start */
    if (PNULL != ueReportedCQINodeInfo_p)
    {
        ueReportedCQINodeInfo_p->ueIndex = ueIndex;
        ueReportedCQINodeInfo_p->mode = PERIODIC_CQI_MODE_1_1_TYPE_3;

        PeriodicMode11Type3 *periodicMode11Type3_p =  
            &(ueReportedCQINodeInfo_p->cqiNode.periodicMode11Type3);

        periodicMode11Type3_p->riValue = *ri_p;
        LOG_MAC_MSG(MAC_RECEIVED_RI_LOG_ID,LOGERROR,MAC_L1_INF,\
                getCurrentTick(),ueIndex,dlUEContext_p->ueCategory,\
                *ri_p,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                __func__,"RI received");
#ifdef PERF_STATS
        gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
            totalRankReported[*ri_p - 1]++;
#endif 

        /* SPR 99999 CQI with 255 UE Fix start */
        UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_CQI_RECEIVED(ueReportedCQINodeInfo_p,internalCellIndex)
            /* SPR 23483 Changes Start */
            if(CIRC_FAILURE == ENQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo,
                        /*CA Changes start  */
                        &(ueReportedCQINodeInfo_p->nodeAnchor), internalCellIndex))
                /*CA Changes end  */
            /* SPR 23483 Changes End */
            {
                freeMemPool(ueReportedCQINodeInfo_p);
                ueReportedCQINodeInfo_p = NULL;	
            }
        /* SPR 99999 Fix end */
    }
    else
    {
        /* Not able to allocate Memory */
    }
    /* Rel 5.3: Coverity Fix End */

    return;
}

/*****************************************************************************
 * Function Name  : decodePeriodicCqiMode10Type3 
 * Inputs         : messageBuffer_p  - pointer to PDU recieved,
 *                  ueIndex - ueIndex of UE,
 *                  dlUEContext_p - pointer to DLUEContext 
 *                  ri_p - pointer to the UE reported RI
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the decoding of Periodic PDU of 
 *                  Mode 1_0 Type 3 with transmission mode 3 i.e open loop MIMO.
 ****************************************************************************/
/*SPR 3140 CHANGES START*/
void decodePeriodicCqiMode10Type3(UInt8       *messageBuffer_p,
        UInt16      ueIndex,
        DLUEContext *dlUEContext_p,
        UInt8       *ri_p,
        InternalCellIndex internalCellIndex)
{
    /* CID 114237 FIX Start*/
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /* CID 114237 FIX End*/
    InternalCellIndex  servCellId = INVALID_CELL_INDEX;

    UEReportedCQINodeInfo *ueReportedCQINodeInfo_p = PNULL;

    /* UE will report 1 bit so we will increment it by 1 to get exact RI value. 
     * Updating RI in UL context */
    /** SPR 14505 changes **/
    if( (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4) && 
            ( (UE_CATEGORY_5 == dlUEContext_p->ueCategory) || (UE_CATEGORY_8 == dlUEContext_p->ueCategory) ) 
      )
    /** SPR 14505 changes **/
    {
        *ri_p = (GET_2_BIT_SWAPPED_VALUE((*messageBuffer_p) & 0x03u)) + 1;
    }
    else 
    {
        *ri_p = (*(messageBuffer_p) & 0x01u) + 1;
    }
    /* 4x4 DL MIMO CHG START */
    /* SPR 10177 changes Start */
     /* CA_phase2_csi_code start */
    if(dlUEContext_p->internalCellIndex == internalCellIndex)
    {
        /* Pcell */
        if ((TX_MODE_8 != dlUEContext_p->dlMIMOInfo.transmissionMode) &&
        /* SPR 10177 changes End */
                !(isRIInCodebookSubsetRestriction (*ri_p,
                        dlUEContext_p->ueCategory,
                        dlUEContext_p->dlMIMOInfo.transmissionMode,
                        dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue,
                        internalCellIndex)))
        {
            LOG_MAC_MSG(MAC_RECEIVED_RI_LOG_ID,LOGERROR,MAC_L1_INF,\
                    getCurrentTick(),ueIndex,dlUEContext_p->ueCategory,\
                    *ri_p,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                    __func__,"Invalid Periodic RI received in Mode10");
            return;
        }
    }
    else
    {
        /* Scell */
        servCellId = getServeCellIndexFromInternalCellId(dlUEContext_p,
                internalCellIndex);
        if(servCellId <= MAX_NUM_SCELL)
        {
	    if (!(isRIInCodebookSubsetRestriction (*ri_p,
						dlUEContext_p->ueCategory,
						dlUEContext_p->dlUeScellContext_p[servCellId]->ScelldlMIMOInfo.transmissionMode,
						dlUEContext_p->dlUeScellContext_p[servCellId]->
						codebookSubsetRestrictionv1020.cbsrValue,
						internalCellIndex)))
	    {
		    LOG_MAC_MSG(MAC_RECEIVED_RI_LOG_ID,LOGERROR,MAC_L1_INF,\
				    getCurrentTick(),ueIndex,dlUEContext_p->ueCategory,\
				    *ri_p,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
				    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
				    __func__,"Invalid Periodic RI received in Mode10");
		    return;
	    }
	}
    }
     /* CA_phase2_csi_code end */
    /* 4x4 DL MIMO CHG END */
#ifdef PERF_STATS
    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
        totalRankReported[*ri_p - 1]++;
#endif 


    GET_MEM_FROM_POOL(UEReportedCQINodeInfo,ueReportedCQINodeInfo_p,
            sizeof(UEReportedCQINodeInfo),PNULL);
    /* Rel 5.3: Coverity 24442 Fix Start */
    if (PNULL != ueReportedCQINodeInfo_p)
    {
        ueReportedCQINodeInfo_p->ueIndex = ueIndex;
        ueReportedCQINodeInfo_p->mode = PERIODIC_CQI_MODE_1_0_TYPE_3;

        PeriodicMode10Type3 *periodicMode10Type3_p =  
            &(ueReportedCQINodeInfo_p->cqiNode.periodicMode10Type3);

        periodicMode10Type3_p->riValue = *ri_p;

        LOG_MAC_MSG(MAC_RECEIVED_RI_LOG_ID,LOGERROR,MAC_L1_INF,\
                getCurrentTick(),ueIndex,dlUEContext_p->ueCategory,\
                *ri_p,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                __func__,"RI received");
#ifdef PERF_STATS
        gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
            totalRankReported[*ri_p - 1]++;
#endif 
        /* SPR 99999 CQI with 255 UE Fix start */
        UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_CQI_RECEIVED(ueReportedCQINodeInfo_p,internalCellIndex)
        /* SPR 23483 Changes Start */
        if(CIRC_FAILURE == ENQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo,
                    /*CA Changes start  */
                    &(ueReportedCQINodeInfo_p->nodeAnchor), internalCellIndex))
            /*CA Changes end  */
        /* SPR 23483 Changes End */
        {
            freeMemPool(ueReportedCQINodeInfo_p);
            ueReportedCQINodeInfo_p = NULL;	
        }
        /* SPR 99999 Fix end */
    }
    else
    {
        /* Not able to allocate Memory */
    }
    /* Rel 5.3: Coverity Fix End */
    return;
}
/*SPR 3140 CHANGES END */

/* - CQI_5.0 */
/*****************************************************************************
 * Function Name  : decodePeriodicCqiMode10Type4 
 * Inputs         : messageBuffer_p  - pointer to PDU recieved,
 *                  ueIndex - ueIndex of UE,
 *                  transmissionMode - mode of transmission,
 *                  ri - rank indicator, 
 *                  ulUEContext_p - pointer to ULUEContext
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the decoding of 
 *                  Periodic PDU of Mode 1_0 Type 4.
 *****************************************************************************/

void decodePeriodicCqiMode10Type4(UInt8 *messageBuffer_p, 
        UInt16 ueIndex,
        UInt8 transmissionMode,
        UInt8 ri,
        ULUEContext *ulUEContext_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    UInt8 widebandCQIValueCW0 = 0;
    UInt8 widebandCQIValueCW1 = 0;
    UEReportedCQINodeInfo *ueReportedCQINodeInfo_p = PNULL;
    /* + CQI_4.1 */
    /*SPR_2311*/
//Check
    widebandCQIValueCW0 = GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
    /*SPR_2311*/
    widebandCQIValueCW0 = ((GET_4_BIT_SWAPPED_VALUE(widebandCQIValueCW0)) 
            - DL_ADAPTATION_CORRECTION);

#ifdef PERF_STATS
    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
        totalCQICW1Reported[widebandCQIValueCW0]++;
#endif

    /* KPI changes start */
#ifdef KPI_STATS
    LteMacKPIStats  *pLteMacKPIStats       = PNULL;
    UInt32 activeIndexForMacStats = 0;

    /* + KPI_CA */
    activeIndexForMacStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
    pLteMacKPIStats = &(gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
    /* - KPI_CA */
    pLteMacKPIStats->totalCQICW1Reported[widebandCQIValueCW0]++;
#endif
    /* KPI changes end */

    /* SPR 14353 Changes Start */
    VALIDATE_CQI_VALUE_FOR_MIN(widebandCQIValueCW0,PNULL);
    /* SPR 14353 Changes End */

    GET_MEM_FROM_POOL(UEReportedCQINodeInfo,ueReportedCQINodeInfo_p,
            sizeof(UEReportedCQINodeInfo),PNULL);
    /* Rel 5.3: Coverity 24443 Fix Start */
    if (PNULL != ueReportedCQINodeInfo_p)
    {
        ueReportedCQINodeInfo_p->ueIndex = ueIndex;

        ueReportedCQINodeInfo_p->mode = PERIODIC_CQI_MODE_1_0_TYPE_4;

        PeriodicMode10Type4 *periodicMode10Type4_p = 
            &(ueReportedCQINodeInfo_p->
                    cqiNode.periodicMode10Type4);
        /*4x4 DL MIMO CHG START*/ 
        /*SPR 8192 FIX START*/
        if ((( TX_MODE_3 == transmissionMode) || 
                    /* + TM7_8 Changes Start */ 
                    (TX_MODE_8 == transmissionMode))&& ( 2 <= ri)) 
            /* - TM7_8 Changes End */ 
            /*SPR 8192 FIX END*/
            /*4x4 DL MIMO CHG END*/   
        {
            widebandCQIValueCW1 = widebandCQIValueCW0;

        }    
        periodicMode10Type4_p->widebandCQIValueCW0 = widebandCQIValueCW0;
        periodicMode10Type4_p->widebandCQIValueCW1 = widebandCQIValueCW1;

        LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                getCurrentTick(),ueIndex,widebandCQIValueCW0,DEFAULT_INT_VALUE,\
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                __func__,"WB_CW0");
        /* - CQI_4.1*/
        /* CLPC_CHG */
        /* CA_phase2_csi_code Start */
        /* Since UL CA not supported so no need to update these values
         * in Scell context */
        if(ulUEContext_p->internalCellIndex == internalCellIndex)
        {
            ulUEContext_p->isDLWideBandCQIReportAvailable = TRUE;
            ulUEContext_p->dlWidebandCQICodeWordOne =
                periodicMode10Type4_p->widebandCQIValueCW0;
            ulUEContext_p->dlWidebandCQICodeWordTwo = 
                periodicMode10Type4_p->widebandCQIValueCW1;
        }
        /* CA_phase2_csi_code end */
        /* SPR 99999 CQI with 255 UE Fix start */
           UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_CQI_RECEIVED(ueReportedCQINodeInfo_p,internalCellIndex)
        /* SPR 23483 Changes Start */
        if(CIRC_FAILURE == ENQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo,
                    /*CA Changes start  */
                    &(ueReportedCQINodeInfo_p->nodeAnchor), internalCellIndex))
            /*CA Changes end  */
        /* SPR 23483 Changes End */
        {
            freeMemPool(ueReportedCQINodeInfo_p);
        }
        /* SPR 99999 Fix end */
    }
    else
    {
        /* Not able to allocate Memory */
    }
    /* Rel 5.3: Coverity Fix End */


    return;
}

/*****************************************************************************
 * Function Name  : decodePeriodicCqiMode20Type1 
 * Inputs         : messageBuffer_p  - pointer to PDU recieved,
 *                  ueIndex - Index of UE,
 *                  transmissionMode -  Transmission Mode,
 *                  ri - UE reported RI, 
 *                  bandwidthPartNum - for which bandwidth part number UE 
 *                                     has repoted subband.
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the decoding of 
 *                  Periodic PDU of Mode 2_0 Type 1.
 *****************************************************************************/
/* + CQI_4.1 */
void decodePeriodicCqiMode20Type1(UInt8 *messageBuffer_p,
        UInt16 ueIndex,
        UInt8 transmissionMode,
        /* + CQI_5.0 */
        UInt8 ri,
        /* - CQI_5.0 */
        UInt8 bandwidthPartNum,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    UInt8 subbandCQICW0 = 0;
    UInt8 subbandCQICW1 = 0;

    UInt8 preferredSubband = 0;
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UEReportedCQINodeInfo *ueReportedCQINodeInfo_p = PNULL;

    if ( FREQUENCY_SELECTIVE_ON == cellParams_p->freqSelectiveSchDL )
    {
        /*SPR 21022 fix start*/
        subbandCQICW0 = GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
        /*SPR 21022 fix end*/
        subbandCQICW0 = ((GET_4_BIT_SWAPPED_VALUE(subbandCQICW0))
                - DL_ADAPTATION_CORRECTION);

#ifdef PERF_STATS
        gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
            totalCQICW1Reported[subbandCQICW0]++;
#endif 

        /* KPI changes start */
#ifdef KPI_STATS
        LteMacKPIStats  *pLteMacKPIStats       = PNULL;
        UInt32 activeIndexForMacStats = 0;

        /* + KPI_CA */
        activeIndexForMacStats = (gMacStats.
                pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
        pLteMacKPIStats = &(gMacStats.
                pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
        /* - KPI_CA */
        pLteMacKPIStats->totalCQICW1Reported[subbandCQICW0]++;
#endif
        /* KPI changes end */

        /* SPR 14353 Changes Start */
        VALIDATE_CQI_VALUE_FOR_MIN(subbandCQICW0,PNULL);
        /* SPR 14353 Changes End */

        GET_MEM_FROM_POOL(UEReportedCQINodeInfo,ueReportedCQINodeInfo_p,
                sizeof(UEReportedCQINodeInfo),PNULL);

        /* Rel 5.3: Coverity 24446 Fix Start */
        if (PNULL == ueReportedCQINodeInfo_p)
        {
            /* Not able to allocate Memory */
            return;
        }
        /* Rel 5.3: Coverity Fix End */

        ueReportedCQINodeInfo_p->ueIndex = ueIndex;
        if ( 1 == cellParams_p->LBitLabel )
        {
            preferredSubband = ((*messageBuffer_p) & 0x10) >> 4 ;
        }
        else 
        {
            preferredSubband = ((*messageBuffer_p) & 0x30) >> 4 ;
            preferredSubband = GET_2_BIT_SWAPPED_VALUE(preferredSubband);
        }

        VALIDATE_PREFERRED_SUBBAND(bandwidthPartNum,
                preferredSubband,
                ueReportedCQINodeInfo_p,
                internalCellIndex);

        /* if Tx mode is open loop MIMO and ri > 1  , CQI values for CW0 is 
         * copied to CW1 ref. 36.213 sec 7.2.2.
         */
        if ((TX_MODE_3 == transmissionMode)||
                /* + TM7_8 Changes Start */
                ((1 < cellParams_p->numOfTxAnteenas)&&
                 ((TX_MODE_7 == transmissionMode) ||(TX_MODE_8 == transmissionMode))))
            /* - TM7_8 Changes End */
        {
            ueReportedCQINodeInfo_p->mode = PERIODIC_CQI_MODE_2_1_TYPE_1;

            PeriodicMode21Type1 *periodicMode21Type1_p = 
                &(ueReportedCQINodeInfo_p->
                        cqiNode.periodicMode21Type1);

            periodicMode21Type1_p->bandwidthPart = bandwidthPartNum;

            periodicMode21Type1_p->lBitLabel =  preferredSubband;

            periodicMode21Type1_p->cqiValueCodeword0 = subbandCQICW0;

            /* + CQI_5.0 */
            if (ri > 1)
            {
                subbandCQICW1 = subbandCQICW0;

            }

            periodicMode21Type1_p->cqiValueCodeword1 = subbandCQICW1;

            LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                    getCurrentTick(),ueIndex,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,subbandCQICW0,subbandCQICW1,\
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                    __func__,"SB_RI>1");

            /* - CQI_5.0 */

        }
        else
        {
            ueReportedCQINodeInfo_p->mode = PERIODIC_CQI_MODE_2_0_TYPE_1;

            PeriodicMode20Type1 *periodicMode20Type1_p = 
                &(ueReportedCQINodeInfo_p->
                        cqiNode.periodicMode20Type1);

            periodicMode20Type1_p->bandwidthPart = bandwidthPartNum;

            periodicMode20Type1_p->lBitLabel =  preferredSubband;

            periodicMode20Type1_p->cqiValue = subbandCQICW0;

            LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                    getCurrentTick(),ueIndex,DEFAULT_INT_VALUE,\
                    DEFAULT_INT_VALUE,subbandCQICW0,DEFAULT_INT_VALUE,\
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                    __func__,"SB_CW0");
        }
        /* SPR 99999 CQI with 255 UE Fix start */
        UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_CQI_RECEIVED(ueReportedCQINodeInfo_p,internalCellIndex)
        /* SPR 23483 Changes Start */
        if(CIRC_FAILURE == ENQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo,
                    /*CA Changes start  */
                    &(ueReportedCQINodeInfo_p->nodeAnchor), internalCellIndex))
            /*CA Changes end  */
        /* SPR 23483 Changes End */
        {
            freeMemPool(ueReportedCQINodeInfo_p);

        }
        /* SPR 99999 Fix end */

    }
    /* - CQI_4.1*/
    return;
}
/* + CQI_4.1 */

/* + CQI_5.0 */
/*****************************************************************************
 * Function Name  : decodePeriodicCqiMode20Type3 
 * Inputs         : messageBuffer_p  - pointer to PDU recieved,
 *                  ueIndex - ueIndex of UE,
 *                  dlUEContext_p - pointer to DLUEContext, 
 *                  ri_p - pointer to the ueReportedRI
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the decoding of Periodic PDU of 
 *                  Mode 2_0 Type 3 with transmission mode 3 i.e.  open loop MIMO.
 *****************************************************************************/

void decodePeriodicCqiMode20Type3(UInt8 *messageBuffer_p, 
        UInt16 ueIndex,
        DLUEContext *dlUEContext_p,
        UInt8 *ri_p,
        InternalCellIndex internalCellIndex)
{
    UEReportedCQINodeInfo *ueReportedCQINodeInfo_p = PNULL;

    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    InternalCellIndex  servCellId = INVALID_CELL_INDEX;

    /* UE will report 1 bit so we will increment it by 1 to get exact RI value.
     * Also updating RI in UL context.*/
    /** SPR 14505 changes **/
    if( (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4) && 
            ((UE_CATEGORY_5 == dlUEContext_p->ueCategory) || (UE_CATEGORY_8 == dlUEContext_p->ueCategory) )
      )
    /** SPR 14505 changes **/
    {
        /*SPR 21022 fix start*/
        *ri_p = (GET_2_BIT_SWAPPED_VALUE((*messageBuffer_p) & 0x03u)) + 1;
    }
    else
    {
         /*SPR 21022 fix start*/  
        *ri_p = (*(messageBuffer_p) & 0x01u) + 1;
         /*SPR 21022 fix end*/
    }


        
    /* 4x4 DL MIMO CHG START */
     /* CA_phase2_csi_code start */
    if(dlUEContext_p->internalCellIndex == internalCellIndex)
    {
        /* Pcell */
	/* SPR 10177 changes start */
	if ((TX_MODE_8 != dlUEContext_p->dlMIMOInfo.transmissionMode) &&
        /* SPR 10177 changes end */
        (!(isRIInCodebookSubsetRestriction( *ri_p,
                        dlUEContext_p->ueCategory,
                        dlUEContext_p->dlMIMOInfo.transmissionMode,
                        dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue,
                        internalCellIndex))))
            /* 4x4 DL MIMO CHG END */
        {
            LOG_MAC_MSG(MAC_RECEIVED_RI_LOG_ID,LOGERROR,MAC_L1_INF,\
                    getCurrentTick(),ueIndex,dlUEContext_p->ueCategory,\
                    *ri_p,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                    __func__,"Invalid Periodic RI received for Mode20");
            return;
        }
    }
    else
    {
        /* Scell */
        servCellId = getServeCellIndexFromInternalCellId(dlUEContext_p,
                internalCellIndex);
        /*kolckwork warning fix*/
        if(servCellId <= MAX_NUM_SCELL)
        {
	    if ((TX_MODE_8 != dlUEContext_p->dlUeScellContext_p[servCellId]->ScelldlMIMOInfo.transmissionMode) &&
			    (!(isRIInCodebookSubsetRestriction( *ri_p,
								dlUEContext_p->ueCategory,
								dlUEContext_p->dlUeScellContext_p[servCellId]->ScelldlMIMOInfo.transmissionMode,
								dlUEContext_p->dlUeScellContext_p[servCellId]->
								codebookSubsetRestrictionv1020.cbsrValue,
								internalCellIndex))))
		    /* 4x4 DL MIMO CHG END */
	    {
		    LOG_MAC_MSG(MAC_RECEIVED_RI_LOG_ID,LOGERROR,MAC_L1_INF,\
				    getCurrentTick(),ueIndex,dlUEContext_p->ueCategory,\
				    *ri_p,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
				    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
				    __func__,"Invalid Periodic RI received for Mode20");
		    return;
	    }
	}
    }
     /* CA_phase2_csi_code end */
   *ri_p = (*(messageBuffer_p) & 0x01u) + 1;
#ifdef PERF_STATS
    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
        totalRankReported[*ri_p - 1]++;
#endif 

    GET_MEM_FROM_POOL(UEReportedCQINodeInfo,ueReportedCQINodeInfo_p,
            sizeof(UEReportedCQINodeInfo),PNULL);

    /* Rel 5.3: Coverity 24447 Fix Start */
    if (PNULL != ueReportedCQINodeInfo_p)
    {
        ueReportedCQINodeInfo_p->ueIndex = ueIndex;
        ueReportedCQINodeInfo_p->mode = PERIODIC_CQI_MODE_2_0_TYPE_3;

        PeriodicMode20Type3 *periodicMode20Type3_p =  
            &(ueReportedCQINodeInfo_p->cqiNode.periodicMode20Type3);

        periodicMode20Type3_p->riValue = *ri_p;
        LOG_MAC_MSG(MAC_RECEIVED_RI_LOG_ID,LOGERROR,MAC_L1_INF,\
                getCurrentTick(),ueIndex,dlUEContext_p->ueCategory,\
                *ri_p,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                __func__,"RI received");
#ifdef PERF_STATS
        gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
            totalRankReported[*ri_p - 1]++;
#endif
        /* SPR 99999 CQI with 255 UE Fix start */
        UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_CQI_RECEIVED(ueReportedCQINodeInfo_p,internalCellIndex)
        /* SPR 23483 Changes Start */
        if(CIRC_FAILURE == ENQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo,
                    /*CA Changes start  */
                    &(ueReportedCQINodeInfo_p->nodeAnchor), internalCellIndex))
            /*CA Changes end  */
        /* SPR 23483 Changes End */
        {
            freeMemPool(ueReportedCQINodeInfo_p);
				ueReportedCQINodeInfo_p = NULL;	
        }
        /* SPR 99999 Fix end */
    }
    else
    {
        /* Not able to allocate Memory */
    }
    /* Rel 5.3: Coverity Fix End */
    return;
}

/* - CQI_5.0 */
/*****************************************************************************
 * Function Name  :  decodePeriodicCqiMode20Type4
 * Inputs         : messageBuffer_p - pointer to PDU recieved,
 *                  ueIndex - ueIndex of UE,
 *                  transmissionMode - transmission mode,
 *                  ri - Rank Indicator,
 *                  ulUEContext_p - pointer to ULUEContext
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the decoding of the 
 *                  Periodic Mode 2_0 Type 4 PDU.
 *****************************************************************************/

void decodePeriodicCqiMode20Type4(UInt8 *messageBuffer_p, 
        UInt16 ueIndex,
        UInt8 transmissionMode,
        UInt8 ri,
        ULUEContext *ulUEContext_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    UInt8 widebandCQIValueCW0 = 0;
    UInt8 widebandCQIValueCW1 = 0;
    UEReportedCQINodeInfo *ueReportedCQINodeInfo_p = PNULL;
/*SPR 19992 Start*/
    /* + CQI_4.1 */
    /*SPR 2311 Start*/
    widebandCQIValueCW0 = GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
    /*SPR 2311 Start*/
    widebandCQIValueCW0 = ((GET_4_BIT_SWAPPED_VALUE(widebandCQIValueCW0))
            - DL_ADAPTATION_CORRECTION);
        /*SPR 19992 End*/
#ifdef PERF_STATS
    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
        totalCQICW1Reported[widebandCQIValueCW0]++;
#endif 

    /* KPI changes start */
#ifdef KPI_STATS
    LteMacKPIStats  *pLteMacKPIStats       = PNULL;
    UInt32 activeIndexForMacStats = 0;

    /* + KPI_CA */
    activeIndexForMacStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
    pLteMacKPIStats = &(gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
    /* - KPI_CA */
    pLteMacKPIStats->totalCQICW1Reported[widebandCQIValueCW0]++;
#endif
    /* KPI changes end */

    /* SPR 14353 Changes Start */
    VALIDATE_CQI_VALUE_FOR_MIN(widebandCQIValueCW0,PNULL);     
    /* SPR 14353 Changes End */

    GET_MEM_FROM_POOL(UEReportedCQINodeInfo,ueReportedCQINodeInfo_p,
            sizeof(UEReportedCQINodeInfo),PNULL);
    /* Rel 5.3: Coverity  24448 Fix Start */
    if (PNULL != ueReportedCQINodeInfo_p)
    {
        ueReportedCQINodeInfo_p->ueIndex = ueIndex;
        ueReportedCQINodeInfo_p->mode = PERIODIC_CQI_MODE_2_0_TYPE_4;

        PeriodicMode20Type4 *periodicMode20Type4_p = 
            &(ueReportedCQINodeInfo_p->cqiNode.periodicMode20Type4);
        /*4x4 DL MIMO CHG START*/ 
        /*SPR 9182 FIX START*/
        if ( (( TX_MODE_3 == transmissionMode) || 
                    /* + TM7_8 Changes Start */ 
                    (TX_MODE_8 == transmissionMode))&& ( 2 <= ri)) 

            /*SPR 9182 FIX END*/
            /*SPR 9182 FIX END*/
            /*4x4 DL MIMO CHG END*/
        {
            widebandCQIValueCW1 = widebandCQIValueCW0;

        }    
        periodicMode20Type4_p->widebandCQIValueCW0 = widebandCQIValueCW0;
        periodicMode20Type4_p->widebandCQIValueCW1 = widebandCQIValueCW1;

        LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                getCurrentTick(),ueIndex,widebandCQIValueCW0,\
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                __func__,"WB_CW0");
        /* - CQI_4.1*/
        /* CLPC_CHG */
        /* CA_phase2_csi_code Start */
        /* Since UL CA not supported so no need to update these values
         * in Scell context */
        if(ulUEContext_p->internalCellIndex == internalCellIndex)
        {
            ulUEContext_p->isDLWideBandCQIReportAvailable = TRUE;
            ulUEContext_p->dlWidebandCQICodeWordOne =
                periodicMode20Type4_p->widebandCQIValueCW0;
            ulUEContext_p->dlWidebandCQICodeWordTwo =
                periodicMode20Type4_p->widebandCQIValueCW1;
        }
        /* CA_phase2_csi_code end */
        /* CLPC_CHG_END */

        /* SPR 99999 CQI with 255 UE Fix start */
        UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_CQI_RECEIVED(ueReportedCQINodeInfo_p,internalCellIndex)
        /* SPR 23483 Changes Start */
        if(CIRC_FAILURE == ENQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo,
                    /*CA Changes start  */
                    &(ueReportedCQINodeInfo_p->nodeAnchor), internalCellIndex))
            /*CA Changes end  */
        /* SPR 23483 Changes End */
        {
            freeMemPool(ueReportedCQINodeInfo_p);
            	ueReportedCQINodeInfo_p = NULL;	
        }
        /* SPR 99999 Fix end */
    }
    else
    {
        /* Not able to allocate Memory */
    }
    /* Rel 5.3: Coverity Fix End */
    return;
}

/* + CQI_4.1 */
/*****************************************************************************
 * Function Name  : decodePeriodicCqiMode21Type1 
 * Inputs         : messageBuffer_p - pointer to PDU recieved,
 *                  ueIndex - Index of UE,
 *                  ri - rank Indicator, 
 *                  bandwidthPartNum - for which bandwidth part number UE
 *                                        has repoted subband.
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the decoding of 
 *                  Periodic PDU of Mode 2_1 Type 1 for r1 > 1.
 *****************************************************************************/
void decodePeriodicCqiMode21Type1(UInt8 *messageBuffer_p,
        UInt32 ueIndex,
        UInt8 ri,
        UInt8 bandwidthPartNum,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{

    UInt8 subbandCQI0 = 0;
    SInt8 subbandCQI1 = 0;
    UInt8 differentialCQI = 0;
    SInt8 offsetLevel = 0;
    UInt8 preferredSubband = 0;
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UEReportedCQINodeInfo *ueReportedCQINodeInfo_p = PNULL;

    if ( FREQUENCY_SELECTIVE_ON == cellParams_p->freqSelectiveSchDL )
    {
        /*SPR 21022 fix start*/
        subbandCQI0 = GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
        /*SPR 21022 fix end*/  
        subbandCQI0 = ((GET_4_BIT_SWAPPED_VALUE(subbandCQI0))
                - DL_ADAPTATION_CORRECTION);

#ifdef PERF_STATS
        gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
            totalCQICW1Reported[subbandCQI0]++;
#endif

        /* KPI changes start */
#ifdef KPI_STATS
        LteMacKPIStats  *pLteMacKPIStats       = PNULL;
        UInt32 activeIndexForMacStats = 0;

        /* + KPI_CA */
        activeIndexForMacStats = (gMacStats.
                pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
        pLteMacKPIStats = &(gMacStats.
                pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
        /* - KPI_CA */
        pLteMacKPIStats->totalCQICW1Reported[subbandCQI0]++;
#endif
        /* KPI changes end */

        /* SPR 14353 Changes Start */
        VALIDATE_CQI_VALUE_FOR_MIN(subbandCQI0,PNULL);   
        /* SPR 14353 Changes Start */

        GET_MEM_FROM_POOL(UEReportedCQINodeInfo,ueReportedCQINodeInfo_p,
                sizeof(UEReportedCQINodeInfo),PNULL);

        /* Rel 5.3: Coverity  24449 Fix Start */
        if (PNULL == ueReportedCQINodeInfo_p)
        {
            /* Not able to allocate memory */
            return;
        }
        /* Rel 5.3: Coverity Fix End */

        ueReportedCQINodeInfo_p->ueIndex = ueIndex;
        ueReportedCQINodeInfo_p->mode = PERIODIC_CQI_MODE_2_1_TYPE_1;

        PeriodicMode21Type1 *periodicMode21Type1_p =
            &(ueReportedCQINodeInfo_p->cqiNode.periodicMode21Type1);
        if(1 < ri)
        {    
            differentialCQI = GET_3_BIT_SWAPPED_VALUE(
                    ((*messageBuffer_p) & 0x70 ) >> 4);

            offsetLevel = offsetLevel_g[differentialCQI];

            /* Codeword 1 offset level = 
             * subband CQI index for CW0 - subband CQI index for CW1
             * from spec 36213 sec 7.2.2 
             */
            subbandCQI1 = (subbandCQI0 - offsetLevel)
                - DL_ADAPTATION_CORRECTION;
            /* SPR 11627 Changes Start */
            VALIDATE_CQI_VALUE(subbandCQI0,
                    subbandCQI1,
                    offsetLevel,
                    ueReportedCQINodeInfo_p);
            /* SPR 11627 Changes End */

#ifdef PERF_STATS       
            gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
                totalCQICW2Reported[subbandCQI1]++;
#endif

            /* KPI changes start */
#ifdef KPI_STATS
            LteMacKPIStats  *pLteMacKPIStats       = PNULL;
            UInt32 activeIndexForMacStats = 0;

            /* + KPI_CA */
            activeIndexForMacStats = (gMacStats.
                    pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
            pLteMacKPIStats = &(gMacStats.
                    pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
            /* - KPI_CA */
            pLteMacKPIStats->totalCQICW2Reported[subbandCQI1]++;
#endif
            /* KPI changes end */

            if (1 == cellParams_p->LBitLabel)
            {
                preferredSubband = ((*messageBuffer_p) & 0x80) >> 7 ;
            }
            else
            {
                preferredSubband = ((*messageBuffer_p) & 0x80) >> 7;
                messageBuffer_p = messageBuffer_p + 1;
                preferredSubband |= ((*messageBuffer_p) & 0x01) << 1;

                preferredSubband = GET_2_BIT_SWAPPED_VALUE(preferredSubband);

            }
            VALIDATE_PREFERRED_SUBBAND(bandwidthPartNum,preferredSubband,
                    ueReportedCQINodeInfo_p,
                    internalCellIndex);

            LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                    getCurrentTick(),ueIndex,DEFAULT_INT_VALUE,\
                    DEFAULT_INT_VALUE,subbandCQI0,subbandCQI1,\
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                    __func__,"SB_BOTH_CW");

        }
        else
        {    
            if ( 1 == cellParams_p->LBitLabel )
            {
                preferredSubband = ((*messageBuffer_p) & 0x10) >> 4 ;
            }
            else 
            {
                preferredSubband = ((*messageBuffer_p) & 0x30) >> 4 ;
                preferredSubband = GET_2_BIT_SWAPPED_VALUE(preferredSubband);
            }

            VALIDATE_PREFERRED_SUBBAND(bandwidthPartNum,preferredSubband,
                    ueReportedCQINodeInfo_p,
                    internalCellIndex);

            LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                    getCurrentTick(),ueIndex,DEFAULT_INT_VALUE,\
                    DEFAULT_INT_VALUE,subbandCQI0,DEFAULT_INT_VALUE,\
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                    __func__,"SB_CW0");
        }

        periodicMode21Type1_p->bandwidthPart = bandwidthPartNum;

        periodicMode21Type1_p->lBitLabel =  preferredSubband;

        periodicMode21Type1_p->cqiValueCodeword0 = subbandCQI0;
        periodicMode21Type1_p->cqiValueCodeword1 = subbandCQI1;

        /* SPR 99999 CQI with 255 UE Fix start */
        UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_CQI_RECEIVED(ueReportedCQINodeInfo_p,internalCellIndex)
        /* SPR 23483 Changes Start */
        if(CIRC_FAILURE == ENQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo,
                    /*CA Changes start  */
                    &(ueReportedCQINodeInfo_p->nodeAnchor), internalCellIndex))
            /*CA Changes end  */
        /* SPR 23483 Changes End */
        {
            freeMemPool(ueReportedCQINodeInfo_p);
        }
        /* SPR 99999 Fix end */
    }    
    return;
}
/* - CQI_4.1 */

/*****************************************************************************
 * Function Name  : decodePeriodicCqiMode21Type2 
 * Inputs         : messageBuffer_p - pointer to PDU recieved
 *                  ueIndex - Index of UE,
 *                  ri - rank Indicator, 
 *                  ulUEContext_p - pointer to ULUEContext.
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the decoding of 
 *                  Periodic PDU of Mode 2_1 Type 2.
 *****************************************************************************/
void decodePeriodicCqiMode21Type2(UInt8 *messageBuffer_p,
        UInt16 ueIndex,
        UInt8 ri,
        ULUEContext *ulUEContext_p,
        InternalCellIndex internalCellIndex)
{
    UInt8 widebandCQIValue = 0;
    /* + CQI_4.1 */
    SInt8 widebandCQIValue1 = 0;
    /* - CQI_4.1 */
    UInt8 differentialCQI = 0;
    SInt8 offsetLevel = 0;
    UInt8 widebandPMI = 0;

    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UEReportedCQINodeInfo *ueReportedCQINodeInfo_p = PNULL;

    /* + CQI_4.1 */
        /*SPR 19992 Start*/
    widebandCQIValue = GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
    widebandCQIValue = ((GET_4_BIT_SWAPPED_VALUE(widebandCQIValue))
            - DL_ADAPTATION_CORRECTION);
        /*SPR 19992 End*/
#ifdef PERF_STATS
    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
        totalCQICW1Reported[widebandCQIValue]++;
#endif

    /* KPI changes start */
#ifdef KPI_STATS
    LteMacKPIStats  *pLteMacKPIStats       = PNULL;
    UInt32 activeIndexForMacStats = 0;

    /* + KPI_CA */
    activeIndexForMacStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
    pLteMacKPIStats = &(gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
    /* - KPI_CA */
    pLteMacKPIStats->totalCQICW1Reported[widebandCQIValue]++;
#endif
    /* KPI changes end */

    /* SPR 14353 Changes Start */
    VALIDATE_CQI_VALUE_FOR_MIN(widebandCQIValue,PNULL);   
    /* SPR 14353 Changes End */

    GET_MEM_FROM_POOL(UEReportedCQINodeInfo,ueReportedCQINodeInfo_p,
            sizeof(UEReportedCQINodeInfo),PNULL);
    /* Rel 5.3: Coverity 24450  Fix Start */
    if (PNULL == ueReportedCQINodeInfo_p)
    {
        /* Not able to allocate Memory */
        return;
    }
    /* Rel 5.3: Coverity Fix End */

    ueReportedCQINodeInfo_p->ueIndex = ueIndex;
    ueReportedCQINodeInfo_p->mode = PERIODIC_CQI_MODE_2_1_TYPE_2;

    PeriodicMode21Type2 *periodicMode21Type2_p = 
        &(ueReportedCQINodeInfo_p->cqiNode.periodicMode21Type2);
    /* - CQI_4.1 */

    if(1 <  ri)
    {
        /* + CQI_4.1 */
        /*Differential CQI is present in most significant 3 bits of lower nibble.
         * Source 5.2.3.3.1-2 in 36.212*/ 
        /* + SPR 1476*/
        /*SPR 2311 Start*/
        differentialCQI = GET_3_BIT_SWAPPED_VALUE(
                ((*messageBuffer_p) & 0x70 ) >> 4);

        /*SPR 2311 End*/
        /* - SPR 1476*/
        offsetLevel = offsetLevel_g[differentialCQI];

        /* Codeword 1 offset level = 
         *     wideband CQI index for codeword 0 -  widebandCQI index for CW 1.
         * from specs 36.213 sec 7.2 
         */
        widebandCQIValue1= (widebandCQIValue - offsetLevel)
            - DL_ADAPTATION_CORRECTION;

        /* SPR 11627, 11350 Changes Start */
        VALIDATE_CQI_VALUE(widebandCQIValue,
                widebandCQIValue1,
                offsetLevel,
                ueReportedCQINodeInfo_p);  
        /* SPR 11627, 11350 Changes End */
#ifdef PERF_STATS       
        gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
            totalCQICW2Reported[widebandCQIValue1]++;
#endif

        /* KPI changes start */
#ifdef KPI_STATS
        LteMacKPIStats  *pLteMacKPIStats       = PNULL;
        UInt32 activeIndexForMacStats = 0;

        /* + KPI_CA */
        activeIndexForMacStats = (gMacStats.
                pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
        pLteMacKPIStats = &(gMacStats.
                pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
        /* - KPI_CA */
        pLteMacKPIStats->totalCQICW2Reported[widebandCQIValue1]++;
#endif
        /* KPI changes end */

        /* - CQI_4.1 */  
        /* + CL-MIMO LJA*/
        if(2 == cellParams_p->numOfTxAnteenas)
        {
            /*SPR 2311 Start*/
            /* + CQI_4.1 */
            widebandPMI = GET_VALUE_FROM_LAST_BIT(*messageBuffer_p);
            /* - CQI_4.1 */
            /*SPR 2311 End*/
        }
        else if(4 == cellParams_p->numOfTxAnteenas)
        {
            /*SPR 2311 Start*/
            widebandPMI = GET_VALUE_FROM_LAST_BIT(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            widebandPMI |= ((*messageBuffer_p) & 0x07) << 1;
            /*SPR 2311 End*/
            widebandPMI = GET_4_BIT_SWAPPED_VALUE(widebandPMI);
        }
        else
        {
            ltePanic("Number of Anteenas is neither 2 nor 4\n");
        }

        /* + CQI_4.1*/
        LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                getCurrentTick(),ueIndex,widebandCQIValue,widebandCQIValue1,\
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                __func__,"WB_BOTH_CW");
    }
    else
    {
        /*SPR 2311 Start*/
        if(2 == cellParams_p->numOfTxAnteenas)
        {
            widebandPMI = GET_2_BIT_SWAPPED_VALUE(
                    ((*messageBuffer_p) & 0x30) >> 4);
        }
        else if(4 == cellParams_p->numOfTxAnteenas)
        {
            widebandPMI = GET_4_BIT_SWAPPED_VALUE( 
                    ((*messageBuffer_p) & 0xF0) >> 4);
        }

        LOG_MAC_MSG(MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
                getCurrentTick(),ueIndex,widebandCQIValue,DEFAULT_INT_VALUE,\
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                __func__,"WB_CW0");
        /* - CQI_4.1*/
        /*SPR 2311 End*/
    }
    periodicMode21Type2_p->widebandCQIValueCodeword0 = widebandCQIValue;
    periodicMode21Type2_p->widebandCQIValueCodeword1 = widebandCQIValue1;
    periodicMode21Type2_p->widebandPMI = widebandPMI;

    /* SPR 99999 CQI with 255 UE Fix start */
    UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_CQI_RECEIVED(ueReportedCQINodeInfo_p,internalCellIndex)
    /* SPR 23483 Changes Start */
    if(CIRC_FAILURE == ENQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo,
                /*CA Changes start  */
                &(ueReportedCQINodeInfo_p->nodeAnchor), internalCellIndex))
        /*CA Changes end  */
    /* SPR 23483 Changes End */
    {
        freeMemPool(ueReportedCQINodeInfo_p);
			ueReportedCQINodeInfo_p = NULL;	
        return;
    }
    /* SPR 99999 Fix end */

    /* CLPC_CHG */
    /* CA_phase2_csi_code Start */
    /* Since UL CA not supported so no need to update these values
     * in Scell context */
    if(ulUEContext_p->internalCellIndex == internalCellIndex)
    {
    ulUEContext_p->isDLWideBandCQIReportAvailable = TRUE;
    ulUEContext_p->dlWidebandCQICodeWordOne = widebandCQIValue; 
    ulUEContext_p->dlWidebandCQICodeWordTwo = widebandCQIValue1;
    }
    /* CA_phase2_csi_code end */
    /* CLPC_CHG_END */
    return;
}


/* + CQI_5.0 */
/*****************************************************************************
 * Function Name  : decodePeriodicCqiMode21Type3 
 * Inputs         : messageBuffer_p - pointer to PDU recieved
 *                  ueIndex - Index of UE,
 *                  dlUEContext_p - pointer to DL UE Context, 
 *                  ri_p - pointer to the ueReportedRI.
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the decoding of Periodic PDU of 
 *                  Mode 2_1 Type 3.
 *****************************************************************************/
void decodePeriodicCqiMode21Type3(UInt8       *messageBuffer_p, 
        UInt16      ueIndex,
        DLUEContext *dlUEContext_p,
        UInt8       *ri_p,
        InternalCellIndex internalCellIndex)
{   

    UEReportedCQINodeInfo *ueReportedCQINodeInfo_p = PNULL;

    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    InternalCellIndex  servCellId = INVALID_CELL_INDEX;

    /* UE will report 1 bit so we will increment it by 1 to get exact RI value.
     * Also, updating RI in UL context */
    /** SPR 14505 changes **/
    if ( (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4) &&
            ((UE_CATEGORY_5 == dlUEContext_p->ueCategory) || (UE_CATEGORY_8 == dlUEContext_p->ueCategory) )
       )     
    /** SPR 14505 changes **/
    {
        /*SPR 21022 fix start*/
        *ri_p = (GET_2_BIT_SWAPPED_VALUE((*messageBuffer_p) & 0x03u)) + 1;
    }
    else
    {
        *ri_p = (*(messageBuffer_p) & 0x01u) + 1;
         /*SPR 21022 fix end*/
     }
    /* 4x4 DL MIMO CHG START */
    /* SPR 10177 changes Start */
    if(dlUEContext_p->internalCellIndex == internalCellIndex)
    {
        /* Pcell */
	if ((TX_MODE_8 != dlUEContext_p->dlMIMOInfo.transmissionMode) &&
        /* SPR 10177 changes End */
            (!(isRIInCodebookSubsetRestriction( *ri_p,
                        dlUEContext_p->ueCategory,
                        dlUEContext_p->dlMIMOInfo.transmissionMode,
                        dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue,
                        internalCellIndex))))
            /* 4x4 DL MIMO CHG END */
        {
            LOG_MAC_MSG(MAC_RECEIVED_RI_LOG_ID,LOGERROR,MAC_L1_INF,\
                    getCurrentTick(),ueIndex,dlUEContext_p->ueCategory,\
                    *ri_p,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                    __func__,"Invalid Periodic RI received for Mode21");
            return;
        }
    }
    else
    {
        /* Scell */
        servCellId = getServeCellIndexFromInternalCellId(dlUEContext_p,
                internalCellIndex);
        /*klockwork warning fix*/
	if ((servCellId <= MAX_NUM_SCELL) && (TX_MODE_8 != dlUEContext_p->dlUeScellContext_p[servCellId]->ScelldlMIMOInfo.transmissionMode) &&
        	 (!(isRIInCodebookSubsetRestriction( *ri_p,
                        dlUEContext_p->ueCategory,
                        dlUEContext_p->dlUeScellContext_p[servCellId]->ScelldlMIMOInfo.transmissionMode,
                        dlUEContext_p->dlUeScellContext_p[servCellId]->
                        codebookSubsetRestrictionv1020.cbsrValue,
                        internalCellIndex))))
            /* 4x4 DL MIMO CHG END */
        {
            LOG_MAC_MSG(MAC_RECEIVED_RI_LOG_ID,LOGERROR,MAC_L1_INF,\
                    getCurrentTick(),ueIndex,dlUEContext_p->ueCategory,\
                    *ri_p,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                    __func__,"Invalid Periodic RI received for Mode21");
            return;
        }
    }
     /* CA_phase2_csi_code end */


#ifdef PERF_STATS
    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
        totalRankReported[*ri_p - 1]++;
#endif 

    GET_MEM_FROM_POOL(UEReportedCQINodeInfo,ueReportedCQINodeInfo_p,
            sizeof(UEReportedCQINodeInfo),PNULL);

    /* Rel 5.3: Coverity 24451 Fix Start */
    if (PNULL != ueReportedCQINodeInfo_p)
    {
        ueReportedCQINodeInfo_p->ueIndex = ueIndex;
        ueReportedCQINodeInfo_p->mode = PERIODIC_CQI_MODE_2_1_TYPE_3;

        PeriodicMode21Type3 *periodicMode21Type3_p =  
            &(ueReportedCQINodeInfo_p->cqiNode.periodicMode21Type3);

        periodicMode21Type3_p->riValue = *ri_p;

        LOG_MAC_MSG(MAC_RECEIVED_RI_LOG_ID,LOGERROR,MAC_L1_INF,\
                getCurrentTick(),ueIndex,dlUEContext_p->ueCategory,\
                *ri_p,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                __func__,"RI received");

#ifdef PERF_STATS
        gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
            totalRankReported[*ri_p - 1]++;
#endif 
        /* SPR 99999 CQI with 255 UE Fix start */
        UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_CQI_RECEIVED(ueReportedCQINodeInfo_p,internalCellIndex)
        /* SPR 23483 Changes Start */
        if(CIRC_FAILURE == ENQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo,
                    /*CA Changes start  */
                    &(ueReportedCQINodeInfo_p->nodeAnchor), internalCellIndex))
            /*CA Changes end  */
        /* SPR 23483 Changes End */
        {
            freeMemPool(ueReportedCQINodeInfo_p);
        }
        /* SPR 99999 Fix end */
    }
    else
    {
        /* Not able to allocate Memory */
    }
    /* Rel 5.3: Coverity Fix End */
    return;
}
/* - CQI_5.0 */

#ifdef FAPI_4_0_COMPLIANCE
#endif
/*SPR 19433 +-*/
#ifdef FAPI_4_0_COMPLIANCE
/*****************************************************************************
 * Function Name  : processCQIIndicationR8 
 * Inputs         : 1. cqiMsg_p  - pointer to CQI message(Payload part).
 *                  2. recvTTI  - Its represent the tick of the message
 *                              recieved.
 *                  3. VendorSpecificParam_p
 *                  4. subFrameNum
 *                  5. internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the CQI Indication msg from PHY in case
 *                  PHY Supports Release 8  
 *****************************************************************************/
/* SPR_4261_FIX Start */ 
/*Based on L1 interface we are keeping this value 0*/
#define RI_MULTIPLEX_WITH_PUSCH   0
/* SPR_4261_FIX End */ 
/* + TM7_8 Changes Start */
void processCQIIndicationR8(void *VendorSpecificParam_p,
        void *msg_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        /*SPR 5620 START*/
#ifdef TDD_CONFIG
        UInt16 sysFrameNum,
#endif
        /*SPR 5620 END*/
        UInt16 subFrameNum,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */

        )
/* - TM7_8 Changes End */
{
    UInt8   *data_p       = PNULL;
    UInt16  ueIndex       = 0;
    UInt16  iterator      = 0;
    UInt16  recvRnti      = 0;
    UInt32  datalen       = 0;
    ueExpectedReportType  cqiReportType = EXPECTED_REPORT_INVALID;
    FAPI_rxCqiIndication_st *cqiMsg_p = (FAPI_rxCqiIndication_st *)msg_p;
    FAPI_VendorSpecific_st *vendorSpecific_p=(FAPI_VendorSpecific_st*)VendorSpecificParam_p;
    UInt16  numOfPdu      = MAC_PHY_CONVERT_16(cqiMsg_p->numOfCqi);
    RNTIInfo *rntiInfo_p  = PNULL;
    FAPI_cqiPduIndication_st *cqiPdu_p = PNULL; 
    ULUEContext *ulUEContext_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
    /* TA_CHG */
    UInt16  timingAdvance = 0; /* SPR 2628 Changes */
    /* TA_CHG */
    
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */
    /*SPR 8166 change start */
    UInt8 *refPdu_p = PNULL;
    UInt16  offsetNextPdu = 0;
    /*SPR 8166 change end */

    /* + TM7_8 Changes Start */
    UInt16 count = 0;/*Coverity 69582 +-*/
    /* - TM7_8 Changes End */
    FAPI_UEMeasurementIndication_st *ueMeasInfoForUEs_p = PNULL;

    /* + CQI_4.1 */
    UInt8 transmissionMode = 0;
    /* - CQI_4.1 */

    if(cqiMsg_p->numOfCqi)
    {
        refPdu_p = ((UInt8 *)&cqiMsg_p->cqiPduInfo[0]) +
            (numOfPdu * sizeof(FAPI_cqiPduIndication_st));
        for (iterator = 0; iterator < numOfPdu; iterator++)
        {
            /* SPR 4073 Start*/
            count = COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(internalCellIndex);
            if (count == NUM_PERIODIC_REPORT_NODES)
            {
                /* Due to configured CQI Configuration of all the UEs, the influx of reports
                 * is more that what L2 has been able to process per TTI as per NUM_CQI_NODES
                 * and the reports are just getting accumulated, exhausting system memory.
                 * Hence drop the report nodes till the situation improves*/
                break;
                /* SPR 4073 End*/
            }
            cqiPdu_p = (FAPI_cqiPduIndication_st *)&cqiMsg_p->cqiPduInfo[iterator]; 
            recvRnti = MAC_PHY_CONVERT_16(cqiPdu_p->rnti);

            /* getUeIndexFromRNTIMap returns NULL if not configured RARNTI or 
             * CRNTI Range and rntiFlag == FREERNTI is  deactive.
             */
            rntiInfo_p = getUeIdxFromRNTIMap(recvRnti,internalCellIndex);
            if (PNULL == rntiInfo_p || FREERNTI == rntiInfo_p->rntiFlag)
            {
                lteWarning("processCQIIndicationR8: Recv Rnti %d is not Active at "
                        "Mac Layer\n",recvRnti);   

/* SPR 20636 Changes Start */
                LOG_MAC_MSG(MAC_CQI_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
                        currentTTI,\
                        recvRnti,\
                        (ADDR)rntiInfo_p,\
                        (rntiInfo_p ? rntiInfo_p->rntiFlag : 0), \
                        recvTTI,0,\
                        0.0,0.0,__func__,"CQI_RNTIERR");
/* SPR 20636 Changes End */

                continue ;
            }

            /*SPR_3061_3068_FIX*/
            if (TCRNTI == rntiInfo_p->rntiFlag)
            {
                /* + CQI_5.0 */
                /* If Dataoffset is 0, it means that decoding of CQI has failed for 
                 * that UE so continue */
                if(0 == cqiPdu_p->dataOffset)
                {
                    continue;
                }    

                /* - CQI_5.0 */
                /* tcRnti = rntiInfo_p->index; TDD Warning Fix */

            }  
            else 
            {
                ueIndex = rntiInfo_p->index;
                if (dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag)
                {
                    lteWarning(" Pending Delete Flag True for UE Index %d %s",
                            ueIndex,__func__);

                    LOG_MAC_MSG(MAC_CQI_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
                            currentTTI,\
                            recvRnti,\
                            ueIndex,\
                            recvTTI,0,0,\
                            0.0,0.0,__func__,"CQI_UEDELETE");
                    continue;
                }

                dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
                if( PNULL ==  dlUEContext_p )
                {
                    continue;
                }
                ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
                timingAdvance = MAC_PHY_CONVERT_16(cqiPdu_p->cqiRiInformation.timingAdvance);
                /* + CQI_5.0 */
                /*SPR 5620 START*/
#ifdef FDD_CONFIG
                cqiReportType = 
                      ulUEContext_p->reportFormatMap[subFrameNum].ueExpectedReport;
#elif TDD_CONFIG
                      /* SPR 18122 Changes Start*/
                cqiReportType = 
                      ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].ueExpectedReport;
                      /* SPR 18122 Changes End*/
#endif
                /*SPR 5620 END*/

                /* If Dataoffset is 0, it means either only RI received at PUSCH or
                 * decoding of CQI has failed for that UE.
                 * If decoding of CQI has failed or  ulCqi is less than threshold then 
                 * don't update DL MCS so continue */
#ifdef PERF_STATS
                UPDATE_UE_PUCCH_SINR_STATS( ueIndex, 
                        cqiPdu_p->cqiRiInformation.ulCqi, subFrameNum);
#endif
                /* + SPR_5415  */
                /* +- SPR 17777 */
                if (LTE_FALSE == handleRIAndCheckForAperiodicTrigger(ueIndex, 
                            recvTTI, cqiPdu_p, cqiReportType,
                            internalCellIndex ) )
                    /* +- SPR 17777 */
                {
                    continue;  
                }

                handleTimingAdvance(dlUEContext_p, ueIndex, timingAdvance, 
                        internalCellIndex, getCurrentTickUl());

                /*CQI_PDCCH_ORDER_FIX_START*/
                if (UL_SYNC_LOSS_OFF == 
                        checkSyncLossTimerExpiry(dlUEContext_p))
                {
                    dlUEContext_p->lastSyncDetectedTick = getCurrentTick();
                }

                /* - SPR_5415 */
                /*CQI_PDCCH_ORDER_FIX_END*/
                LOG_MAC_MSG(MAC_TA_INSERTION,LOGBRIEF,MAC_L1_INF,\
                        currentTTI,\
                        getCurrentTick(),dlUEContext_p->lastSyncDetectedTick,\
                        ueIndex,timingAdvance,\
                        DEFAULT_INT_VALUE,\
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                        FUNCTION_NAME,"LAST_SYNC_TICK");
                /* SPR 2628 Changes End */
                /* CQI will come on CRNTI and not on T-CRNTI.
                 * Check if CQI is received on PUCCH  and power control based on 
                 * SINR is enable .
                 * If yes thn call updateAndCheckCqiSinr to trigger TPC  
                 * based on received SINR. */
                /* Time Averaging changes start */
                CHECK_AND_UPDATE_CQI_SINR(dlUEContext_p, cqiPdu_p->cqiRiInformation.ulCqi,
                        subFrameNum, currentTTI,internalCellIndex);
                /* Time Averaging changes end */

                /* calculate type of report */

                datalen =  MAC_PHY_CONVERT_16(cqiPdu_p->length); 
                data_p = offsetNextPdu + (UInt8*)(refPdu_p);
                offsetNextPdu +=  datalen;


                /* + CL-MIMO LJA*/    
                if (cqiPdu_p->cqiRiInformation.ri)
                {
                    /* 4x2 MIMO S */
                    /* 4x4 DL MIMO CHG START */
                    /* SPR 10177 changes start */
                    if ((TX_MODE_8 == dlUEContext_p->dlMIMOInfo.transmissionMode) ||
                            /* SPR 10177 changes end */
                            (isRIInCodebookSubsetRestriction( cqiPdu_p->cqiRiInformation.ri,
                                                              dlUEContext_p->ueCategory,
                                                              dlUEContext_p->dlMIMOInfo.transmissionMode,
                                                              dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue,
                                                              internalCellIndex)))
                    {
                        ulUEContext_p->pucchConfigInfo.ueReportedRI= cqiPdu_p->cqiRiInformation.ri;
                    } 
                    /* 4x4 DL MIMO CHG END */
                    /* 4x2 MIMO E */
                }
                /* SPR_4261_FIX Start */ 
                if ( datalen == RI_MULTIPLEX_WITH_PUSCH)
                {
                    return;
                }
                /* SPR_4261_FIX End */ 

#ifdef LOG_PRINT_ENABLED
    			UInt8   ri = ulUEContext_p->pucchConfigInfo.ueReportedRI;
#endif
                /* - CL-MIMO LJA*/

                /* + CQI_4.1 */
                transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
                /* - CQI_4.1 */

                LOG_MAC_MSG(MAC_CQI_DECODE_ID,LOGINFO,MAC_L1_INF,\
                        currentTTI,\
                        cqiReportType,\
                        ueIndex,\
                        recvTTI,datalen,ri,\
                        0.0,0.0,__func__,"CQI_DECODE");

                decodeCqiReports(data_p,
                        cqiReportType,
                        ueIndex,
                        ulUEContext_p,
                        recvTTI,
                        transmissionMode,
                        dlUEContext_p,
                        subFrameNum,
                        internalCellIndex);
                /*Cyclomatic Complexity changes - ends here */
            }
        }
    }

    /* + E_CID_5.3 */
    if(PNULL != vendorSpecific_p)
    {
        ueMeasInfoForUEs_p = (FAPI_UEMeasurementIndication_st *)&
            vendorSpecific_p->ueMeasInfoForUEs;
        /* SPR 9829 changes start */
        macProcessVendorSpecificFieldsSRSCQIInd(ueMeasInfoForUEs_p,recvTTI,internalCellIndex);
        /* SPR 9829 changes end */
    }
    /* - E_CID_5.3 */

    return;

}

/*****************************************************************************
 * Function Name  : processCQIIndicationR9 
 * Inputs         : 1. msg_p
 *                  2. recvTTI  - Its represent the tick of the message
 *                              recieved.
 *                  3. VendorSpecificParam_p
 *                  4. subFrameNum
 *                  5. internalCellIndex   
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the CQI Indication msg from PHY in 
 *                  case PHY Supports Release 9 or Later
 *****************************************************************************/

/* + TM7_8 Changes Start */
void processCQIIndicationR9(void *VendorSpecificParam_p,
        void *msg_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
#ifdef TDD_CONFIG
        UInt16 sysFrameNum,
#endif
        UInt16 subFrameNum,
        InternalCellIndex internalCellIndex)
{
    UInt8   *data_p       = PNULL;
    UInt8   ri            = 0;
    UInt16  ueIndex       = 0;
    UInt16  iterator      = 0;
    UInt16  recvRnti      = 0;
    UInt32  datalen       = 0;
    ueExpectedReportType  cqiReportType = EXPECTED_REPORT_INVALID;
    FAPI_rxCqiIndicationR9_st *cqiMsg_p = (FAPI_rxCqiIndicationR9_st *)msg_p;
    FAPI_VendorSpecificR9_st *vendorSpecific_p=(FAPI_VendorSpecificR9_st*)VendorSpecificParam_p;
    UInt16  numOfPdu      = MAC_PHY_CONVERT_16(cqiMsg_p->numOfCqi);
    RNTIInfo *rntiInfo_p  = PNULL;
    FAPI_cqiPduIndicationR9_st *cqiPdu_p = PNULL; 
    ULUEContext *ulUEContext_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
    UInt16  timingAdvance = 0;
    UInt8 numberOfCCReported=0;
    UInt8 numCellsReportExpected = MAX_NUM_PCELL;

    
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */
    UInt8 bitMaskForAperiodicReport = 0;
    UInt8 numOfBitsDecoded = 0;
    UInt8 numOfCCProcessed = 0;
    UInt8 servCellIndex;
    DLUESCellContext *dlUeScellContext_p = PNULL;
    UInt8 cellIdxOfExpectedReportType = 0;
    UInt8 numOfBitsOfScellReport = 0; 
/*ca-tdd pucch code changes*/
    UInt8 totalBits = 0; 
    UInt8 offset = 0; 
    UInt8 remainingBitsInWord = 0; 
    UInt8 endIndex; 
    UInt32 temp32Var; 
    UInt32 tempArr[3] = {0};
    InternalCellIndex ueInternalCellIndex = INVALID_CELL_INDEX;
    DLCQIInfo *dlCQIInfo_p = PNULL;
    UInt8 riIndex = 0;
    /* SPR 15909 fix start */
    tickType_t ulAperiodicCqiRequestTTI;
    /* SPR 15909 fix end */
    UInt8 *refPdu_p = PNULL;
    UInt16  offsetNextPdu = 0;

    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt16 count = 0;/*Coverity 69581 +-*/
    UInt16 timingAdvanceR9 = INVALID_TIMING_ADVANCE_R9_VALUE;
    UInt8 validReports = 0;
    EcidReportParams   *eCidReportParams_p = PNULL;

    UInt8 transmissionMode = 0;

    if(cqiMsg_p->numOfCqi)
    {
        refPdu_p = ((UInt8 *)&cqiMsg_p->cqiPduInfo[0]) +
            (numOfPdu * sizeof(FAPI_cqiPduIndicationR9_st));

        for (iterator = 0; iterator < numOfPdu; iterator++)
        {
            bitMaskForAperiodicReport = 0;
/*ca-tdd pucch code changes*/
            riIndex = 0;
            numOfBitsDecoded = 0;
            cqiPdu_p = (FAPI_cqiPduIndicationR9_st *)&cqiMsg_p->cqiPduInfo[iterator]; 
            recvRnti = MAC_PHY_CONVERT_16(cqiPdu_p->rnti);

            /* getUeIndexFromRNTIMap returns NULL if not configured RARNTI or 
             * CRNTI Range and rntiFlag == FREERNTI is  deactive.
             */
            rntiInfo_p = getUeIdxFromRNTIMap(recvRnti,internalCellIndex);
            if (PNULL == rntiInfo_p || FREERNTI == rntiInfo_p->rntiFlag)
            {
                lteWarning("processCQIIndicationR9: Recv Rnti %d is not Active at "
                        "Mac Layer\n",recvRnti);   

/* SPR 20636 Changes Start */
                LOG_MAC_MSG(MAC_CQI_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
                        currentTTI,\
                        recvRnti,\
                        (ADDR)rntiInfo_p,\
                        (rntiInfo_p ? rntiInfo_p->rntiFlag : 0), \
                        recvTTI,0,\
                        0.0,0.0,__func__,"CQI_RNTIERR");

/* SPR 20636 Changes End */
                continue ;
            }

            if (TCRNTI == rntiInfo_p->rntiFlag)
            {
                /* If Dataoffset is 0, it means that decoding of CQI has failed for 
                 * that UE so continue */
                if(0 == cqiPdu_p->dataOffset)
                {
                    continue;
                }    
            }  
            else 
            {
                ueIndex = rntiInfo_p->index;
                if (dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag)
                {
                    lteWarning(" Pending Delete Flag True for UE Index %d %s",
                            ueIndex,__func__);

                    LOG_MAC_MSG(MAC_CQI_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
                            currentTTI,\
                            recvRnti,\
                            ueIndex,\
                            recvTTI,0,0,\
                            0.0,0.0,__func__,"CQI_UEDELETE");
                    continue;
                }

                dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
                /* Coverity 63549 Fix Start */
                if( PNULL ==  dlUEContext_p )
                {
                    continue;
                }
                /* Coverity 63549 Fix End */
                ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
                if(PNULL == ulUEContext_p)
                {
                    continue;
                }
                /* CA_phase2_csi_code start */
/*ca-tdd pucch code changes*/
#ifdef FDD_CONFIG
                cellIdxOfExpectedReportType = 
                         ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType;
#elif TDD_CONFIG
                         
                         /* SPR 18122 Changes Start*/
                cellIdxOfExpectedReportType = 
                        ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].cellIdxOfExpectedReportType;
                       
                        /* SPR 18122 Changes End*/
#endif
                if( 0 == dlUEContext_p->scellCount)
                {
                    /* Non CA UE */
                    count = COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(internalCellIndex);
                    if (count == NUM_PERIODIC_REPORT_NODES)
                    {
                        /* Due to configured CQI Configuration of all the UEs, the influx of reports
                         * is more that what L2 has been able to process per TTI as per NUM_CQI_NODES
                         * and the reports are just getting accumulated, exhausting system memory.
                         * Hence drop the report nodes till the situation improves*/
                        continue;
                    }
                }
                else
                {
                    /* CA is configured so get number of cells for which report
                     * is expected */
                    numCellsReportExpected = getNumCellsForCSIReport(cellIdxOfExpectedReportType,
                            dlUEContext_p->scellCount);
                    if(0 == numCellsReportExpected)
                    {
/* SPR 20636 Changes Start */
                        LOG_MAC_MSG(MAC_CQI_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
                                currentTTI,\
                                recvRnti,\
                                (ADDR)rntiInfo_p,\
                                (rntiInfo_p ? rntiInfo_p->rntiFlag : 0), \
                                recvTTI,0,\
                                0.0,0.0,__func__,"Cell Index of Expected Report Not Set"\
                                "for CA UE");
/* SPR 20636 Changes End */
                        continue;
                    }
                }
                timingAdvance =MAC_PHY_CONVERT_16(cqiPdu_p->cqiRiInformation.timingAdvance);

                /*FAPI 2.4 changes start*/
                /* RJ_Comment incorporation- moving this code at position where
                 * normal TA value is handled after checking threshold */
               /* if(TRUE != ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag)
                        
                {
                    eCidReportParams_p = &ulUEContext_p->eCidReportParams;
                    timingAdvanceR9 = (MAX_TIMING_ADVANCE_R9_VALUE >=MAC_PHY_CONVERT_16(cqiPdu_p->cqiRiInformation.timingAdvanceR9) )?\
                                      MAC_PHY_CONVERT_16(cqiPdu_p->cqiRiInformation.timingAdvanceR9):INVALID_TIMING_ADVANCE_R9_VALUE;
                    eCidReportParams_p->type1TAValue = timingAdvanceR9;
                    validReports |= (MAX_TIMING_ADVANCE_R9_VALUE >= timingAdvanceR9)? TRUE:FALSE;
                }
                else
                {
                    continue;
                }*/
                /*FAPI 2.4 changes end*/

#ifdef FDD_CONFIG
                cqiReportType = 
                        ulUEContext_p->reportFormatMap[subFrameNum].ueExpectedReport;
#elif TDD_CONFIG

                        /* SPR 18122 Changes Start*/
                cqiReportType = 
                        ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].ueExpectedReport;

                        /* SPR 18122 Changes End*/
#endif

                /* If Dataoffset is 0, it means either only RI received at PUSCH or
                 * decoding of CQI has failed for that UE.
                 * If decoding of CQI has failed or  ulCqi is less than threshold then 
                 * don't update DL MCS so continue */
#ifdef PERF_STATS
                UPDATE_UE_PUCCH_SINR_STATS(ueIndex, cqiPdu_p->cqiRiInformation.ulCqi, subFrameNum);
#endif
                /* CA_phase2_csi_code start */
                numberOfCCReported = cqiPdu_p->cqiRiInformation.numberOfCCReported;
                if  (0 == cqiPdu_p->dataOffset) 
                {
                    if(1 == numCellsReportExpected)
                    {
                        handleCQIReportWithDataOffsetNull(ulUEContext_p,
                                dlUEContext_p,recvTTI,ueIndex,cqiPdu_p,
                                subFrameNum,
#ifdef TDD_CONFIG
                                sysFrameNum,
#endif
                                &bitMaskForAperiodicReport,
                                internalCellIndex,
                                numberOfCCReported);
                    }
                    else
                    {
                        /* report is for multiple cell, so only Aperiodic-Rel10 CA UE */
                        numOfCCProcessed = 0;
                        for(servCellIndex = 0; servCellIndex <= MAX_NUM_SCELL;
                                servCellIndex++)
                        {
                            if(!servCellIndex)
                            {
                                ueInternalCellIndex = internalCellIndex;
                                dlCQIInfo_p = &(dlUEContext_p->dlCQIInfo);
                                ulAperiodicCqiRequestTTI = ulUEContext_p->aperiodicCqiRequestTTI;

                            }
                            else
                            {
                                if(PNULL == dlUEContext_p->dlUeScellContext_p[servCellIndex])
                                {
                                    continue;
                                }
                                ueInternalCellIndex = dlUEContext_p->
                                    dlUeScellContext_p[servCellIndex]->internalCellIndex;
                                dlCQIInfo_p = &(dlUEContext_p->dlUeScellContext_p[servCellIndex]->
                                        ScelldlCQIInfo);
                                ulAperiodicCqiRequestTTI = ulUEContext_p->ulUeScellContext_p[servCellIndex]->
                                    aperiodicCqiRequestTTI;
                            }
                            if(numOfCCProcessed != numCellsReportExpected)
                            {
                                if(cellIdxOfExpectedReportType & servingCellBitMask_g[servCellIndex])
                                {
                                    count = COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(
                                            ueInternalCellIndex);
                                    if(count < NUM_PERIODIC_REPORT_NODES)
                                    {
                                        if (APERIODIC_CQI_REPORTING_MODE ==
                                                dlCQIInfo_p->cqiTypeIndicator) 
                                        { 
                                            /* If only aperiodic is  ON and data offset is 0 then 
                                             *  even if below condition is not met then also we will 
                                             *  skip from here.
                                             */  
                                            if   (recvTTI == ulUEContext_p->lastTriggeredAperiodicTTI)
                                            {
                                                /* SPR 4484 changes start */
                                                /* CA_phase2_csi_code */
                                                if((currentTTI > (dlCQIInfo_p->aperiodicCqiRequestTTI + 
                                                                APERIODIC_CQI_REQ_OFFSET)) && (currentTTI >
                                                                (ulAperiodicCqiRequestTTI + APERIODIC_CQI_REQ_OFFSET)))
                                                {
                                                    bitMaskForAperiodicReport |= servingCellBitMask_g[servCellIndex];
                                                }
                                                /* SPR 4484 changes end */
                                            }
                                        }
                                    }
                                    numOfCCProcessed += 1;
                                }
                            }
                            else
                            {
                                break;
                            }

                        }
                        /* reset bitmask cellIdxOfExpectedReportType */
#ifdef FDD_CONFIG
                ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType = 0; 

#elif TDD_CONFIG
                
                /* SPR 18122 Changes Start*/
                ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].cellIdxOfExpectedReportType = 0;
                
                /* SPR 18122 Changes End*/
#endif

                    }/*else end*/
                    /* push node for aperidic trigger  */
                    if(bitMaskForAperiodicReport)
                    {
                        putEntryInULAperiodicCQIQueue(ueIndex, internalCellIndex
                                ,bitMaskForAperiodicReport
                                );
                    }
                    /* CA_phase2_csi_code end */
                    continue;
                }
                else if (cqiPdu_p->cqiRiInformation.ulCqi < cellParams_p->pucchCQISINRThreshold)
                {
                    /* CA_phase2_csi_code Start */
                    /** SPR 14032 Fix : Start **/
                    handleCQIReportWithUlCqiLessThanCQISINRThreshold(numCellsReportExpected,
                            ulUEContext_p,dlUEContext_p,recvTTI,&bitMaskForAperiodicReport,
                            /* +- SPR 17777 */
                            subFrameNum
#ifdef TDD_CONFIG
                            ,sysFrameNum
#endif
                            );
                            /* +- SPR 17777 */
                    /** SPR 14032 Fix : End **/
                    /* push node for aperidic trigger  */
                    if(bitMaskForAperiodicReport)
                    {
                        putEntryInULAperiodicCQIQueue(ueIndex, internalCellIndex
                                ,bitMaskForAperiodicReport
                                );
                    }
                    /* CA_phase2_csi_code End */
                    continue;
                }   
                handleTimingAdvance(dlUEContext_p, ueIndex, timingAdvance,internalCellIndex, getCurrentTickUl());
                /*FAPI 2.4 changes start*/
               if(TRUE != ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag)
                        
                {
                    eCidReportParams_p = &ulUEContext_p->eCidReportParams;
                    timingAdvanceR9 = (MAX_TIMING_ADVANCE_R9_VALUE >=MAC_PHY_CONVERT_16
                            (cqiPdu_p->cqiRiInformation.timingAdvanceR9) )?\
                                      MAC_PHY_CONVERT_16(cqiPdu_p->cqiRiInformation.
                                              timingAdvanceR9):INVALID_TIMING_ADVANCE_R9_VALUE;
                    eCidReportParams_p->type1TAValue = timingAdvanceR9;
                    validReports |= (MAX_TIMING_ADVANCE_R9_VALUE >= timingAdvanceR9)? TRUE:FALSE;
                }
                else
                {
                    continue;
                }
                /*FAPI 2.4 changes end*/

                /*CQI_PDCCH_ORDER_FIX_START*/
                if (UL_SYNC_LOSS_OFF == 
                        checkSyncLossTimerExpiry(dlUEContext_p))
                {
                    dlUEContext_p->lastSyncDetectedTick = getCurrentTick();
                }

                /* - SPR_5415 */
                /*CQI_PDCCH_ORDER_FIX_END*/
                LOG_MAC_MSG(MAC_TA_INSERTION,LOGBRIEF,MAC_L1_INF,\
                        currentTTI,\
                        getCurrentTick(),dlUEContext_p->lastSyncDetectedTick,\
                        ueIndex,timingAdvance,\
                        DEFAULT_INT_VALUE,\
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                        FUNCTION_NAME,"LAST_SYNC_TICK");
                /* SPR 2628 Changes End */
                /* CQI will come on CRNTI and not on T-CRNTI.
                 * Check if CQI is received on PUCCH  and power control based on 
                 * SINR is enable .
                 * If yes thn call updateAndCheckCqiSinr to trigger TPC  
                 * based on received SINR. */
                /* Time Averaging changes start */
                CHECK_AND_UPDATE_CQI_SINR(dlUEContext_p, cqiPdu_p->cqiRiInformation.ulCqi,
                        subFrameNum, currentTTI,internalCellIndex);
                /* Time Averaging changes end */

                /* calculate type of report */

                datalen = MAC_PHY_CONVERT_16(cqiPdu_p->length); 
                data_p = offsetNextPdu + (UInt8*)(refPdu_p);
                offsetNextPdu += datalen;

                /* + CL-MIMO LJA*/  
                /* CA_phase2_csi_code Start */

                if(dlUEContext_p->scellCount)
                {
                    /* Rel 10 CA UE */
                    if(1 == numCellsReportExpected)
                    {
                        processCQIReportForOneCellForCAUE(ulUEContext_p,dlUEContext_p,
                                cqiPdu_p,
                                datalen,
                                subFrameNum,
#ifdef TDD_CONFIG
                                sysFrameNum,
#endif 
                                data_p,
                                ueIndex,
                                recvTTI,
                                internalCellIndex);
                    }
/*ca-tdd pucch code changes*/
                    else
                    {
                        /* Aperiodic report for multiple cells */
                        for(servCellIndex = PCELL_SERV_CELL_IX; servCellIndex <= dlUEContext_p->scellCount;
                                servCellIndex++)
                        {
                            if(!servCellIndex)
                            {
                                if(cellIdxOfExpectedReportType & servingCellBitMask_g[servCellIndex])
                                {
                                    /* Pcell */
                                    if ((cqiPdu_p->cqiRiInformation.numberOfCCReported)
                                            &&(cqiPdu_p->cqiRiInformation.ri[riIndex]))
                                    {
                                        if (isRIInCodebookSubsetRestriction( cqiPdu_p->
                                                    cqiRiInformation.ri[servCellIndex],
                                                    dlUEContext_p->ueCategory,
                                                    dlUEContext_p->dlMIMOInfo.transmissionMode,
                                                    dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue,internalCellIndex))
                                        {
                                            ulUEContext_p->pucchConfigInfo.ueReportedRI= 
                                                cqiPdu_p->cqiRiInformation.ri[servCellIndex];
                                        } 
                                        /* 4x4 DL MIMO CHG END */
                                        /* 4x2 MIMO E */
                                    }
                                    riIndex++;
                                    /* If RI value is not reported for first
                                     * cell for which report was expected, then
                                     * it means RI is 0 and riIndex is
                                     * incremented */
                                    count =  COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(internalCellIndex);
                                    if(NUM_PERIODIC_REPORT_NODES == count)
                                    {
                                        /* calculate number of Bits in Pcell report, so that
                                         * while decoding Scell report buffer can be shifted accordingly */
                                        numOfBitsDecoded = 
                                            calculateSizeOfReport(ulUEContext_p->pucchConfigInfo.ueReportedRI
#ifdef FDD_CONFIG
                                                    ,ulUEContext_p->reportFormatMap[subFrameNum].
                                                    ueExpectedReport,
#elif TDD_CONFIG

                                                    /* SPR 18122 Changes Start*/
                                                    ,ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].
                                                     ueExpectedReport,

                                                     /* SPR 18122 Changes End*/
#endif
                                                    internalCellIndex);
                                        continue;
                                    }
                                    /* SPR_4261_FIX Start */ 
                                    if ( datalen == RI_MULTIPLEX_WITH_PUSCH)
                                    {
                                        return;
                                    }
                                    /* SPR_4261_FIX End */ 
                                    ri = ulUEContext_p->pucchConfigInfo.ueReportedRI;
                                    transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
#ifdef FDD_CONFIG
                                    cqiReportType = ulUEContext_p->reportFormatMap[subFrameNum].ueExpectedReport;
#elif TDD_CONFIG

                                    /* SPR 18122 Changes Start*/
                                    cqiReportType = ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].ueExpectedReport;
                                    
                                    /* SPR 18122 Changes End*/
#endif
                                    decodeAperiodicCqiReportsR10(data_p,
                                            cqiReportType,
                                            ueIndex,
                                            ulUEContext_p,
                                            recvTTI,
                                            transmissionMode,
                                            dlUEContext_p,
                                            /* +- SPR 17777 */
                                            internalCellIndex,
                                            &numOfBitsDecoded
                                            );
                                }
                            }
                            else
                            {
                                /* Scell */
                                if(PNULL == dlUEContext_p->dlUeScellContext_p[servCellIndex])
                                {
                                    continue;
                                }
                                dlUeScellContext_p = dlUEContext_p->dlUeScellContext_p[servCellIndex];
                                internalCellIndex = dlUeScellContext_p->internalCellIndex;
                                if(cellIdxOfExpectedReportType & servingCellBitMask_g[servCellIndex])
                                {
                                    count = COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(
                                            internalCellIndex);
                                    if(NUM_PERIODIC_REPORT_NODES == count)
                                    {
                                        continue;
                                        /* In case when we will have multiple
                                         * Scells, then we have to shift the buffer
                                         * to point to the report of second Scell
                                         * */
                                    }
                                    if ((cqiPdu_p->cqiRiInformation.numberOfCCReported) &&
                                            (cqiPdu_p->cqiRiInformation.ri[riIndex]))
                                    {
                                        if (isRIInCodebookSubsetRestriction(cqiPdu_p->
                                                    cqiRiInformation.ri[servCellIndex],
                                                    dlUEContext_p->ueCategory,
                                                    dlUeScellContext_p->ScelldlMIMOInfo.transmissionMode,
                                                    dlUeScellContext_p->codebookSubsetRestrictionv1020.cbsrValue,
                                                    internalCellIndex))
                                        {
                                            ulUEContext_p->ulUeScellContext_p[servCellIndex]->
                                                scellpucchConfigInfo.ueReportedRI= 
                                                cqiPdu_p->cqiRiInformation.ri[servCellIndex];
                                        } 
                                        /* 4x4 DL MIMO CHG END */
                                        /* 4x2 MIMO E */
                                    }
                                    riIndex++;
                                    ri = ulUEContext_p->ulUeScellContext_p[servCellIndex]->
                                        scellpucchConfigInfo.ueReportedRI; 
                                    transmissionMode = dlUeScellContext_p->ScelldlMIMOInfo.transmissionMode;
                                    /** SPR 14032 Fix : Start **/
#ifdef FDD_CONFIG                                    
                                    cqiReportType = ulUEContext_p->
                                        ulUeScellContext_p[servCellIndex]->
                                        cqiPmiRiReportFormatMapSCell[subFrameNum].ueExpectedReport;
#elif TDD_CONFIG

                                        /* SPR 18122 Changes Start*/
                                    cqiReportType = ulUEContext_p->
                                        ulUeScellContext_p[servCellIndex]->
                                        cqiPmiRiReportFormatMapSCell[MODULO(sysFrameNum,2)][subFrameNum].ueExpectedReport;

                                        /* SPR 18122 Changes End*/
#endif                                        
                                    /** SPR 14032 Fix : End **/
                                    numOfBitsOfScellReport = 
                                        calculateSizeOfReport(ri,cqiReportType,
                                                internalCellIndex);
                                    if(0 == numOfBitsOfScellReport)
                                    {
                                        continue;
                                    }
                                    // numOfBitsDecoded contain num of bits decoded
                                    // for Pcell
                                    if(0 == (numOfBitsDecoded % NUMBER_OF_BITS_IN_1BYTE))
                                    {
                                        data_p = data_p + (numOfBitsDecoded / NUMBER_OF_BITS_IN_1BYTE);
                                        decodeAperiodicCqiReportsR10(data_p,
                                                cqiReportType,
                                                ueIndex,
                                                ulUEContext_p,
                                                recvTTI,
                                                transmissionMode,
                                                dlUEContext_p,
                                                /* +- SPR 17777 */
                                                internalCellIndex,
                                                &numOfBitsDecoded
                                                );
                                    }
                                    else
                                    {
                                        offset = numOfBitsDecoded % NUM_OF_BITS_IN_32_BIT_WORD;
                                        totalBits = numOfBitsDecoded + numOfBitsOfScellReport;
                                        if(totalBits <= NUM_OF_BITS_IN_32_BIT_WORD)
                                        {
                                            memCpy(tempArr,data_p,4);
                                            tempArr[ZERO] = tempArr[ZERO] >> numOfBitsDecoded;
                                            decodeAperiodicCqiReportsR10((UInt8 *)tempArr,
                                                    cqiReportType,
                                                    ueIndex,
                                                    ulUEContext_p,
                                                    recvTTI,
                                                    transmissionMode,
                                                    dlUEContext_p,
                                                    /* +- SPR 17777 */
                                                    internalCellIndex,
                                                    &numOfBitsDecoded
                                                    );
                                        }
                                        else
                                        {
                                            /*increment data_p to point to the 32
                                              bit word containing report for Scell*/
                                            if(numOfBitsDecoded > NUM_OF_BITS_IN_32_BIT_WORD)
                                            {
                                                data_p = data_p + 4;
                                            }
                                            remainingBitsInWord = NUM_OF_BITS_IN_32_BIT_WORD - offset;
                                            if((NUM_OF_BITS_IN_32_BIT_WORD + remainingBitsInWord)>= numOfBitsOfScellReport)
                                            {
                                                memCpy(tempArr, data_p ,
                                                        8);
                                                endIndex = ONE;
                                            }
                                            else
                                            {
                                                memCpy(tempArr, data_p ,
                                                        12);
                                                endIndex = 2;
                                            }
                                            if(ONE == endIndex)
                                            {
                                                tempArr[ZERO] = tempArr[ZERO] >> offset;
                                                temp32Var = tempArr[ONE];
                                                temp32Var = temp32Var << (NUM_OF_BITS_IN_32_BIT_WORD- offset);
                                                tempArr[ZERO]|= temp32Var;
                                                if(numOfBitsOfScellReport > NUM_OF_BITS_IN_32_BIT_WORD)
                                                {
                                                    tempArr[ONE] = tempArr[ONE] >> offset;
                                                }

                                            }
                                            else
                                            {
                                                tempArr[ZERO] = tempArr[ZERO] >> offset;
                                                temp32Var = tempArr[ONE];
                                                temp32Var = temp32Var << (NUM_OF_BITS_IN_32_BIT_WORD- offset);
                                                tempArr[ZERO]|= temp32Var;
                                                tempArr[ONE] = tempArr[ONE] >> offset;
                                                /* rest offset bits in tempArr[1] are
                                                 * to be copied from tempArr[2]*/
                                                temp32Var = tempArr[2];
                                                temp32Var = temp32Var << (NUM_OF_BITS_IN_32_BIT_WORD- offset);
                                                tempArr[ONE]|= temp32Var;
                                                tempArr[2] = tempArr[2] >> offset;

                                            }
                                            decodeAperiodicCqiReportsR10((UInt8 *)tempArr,
                                                    cqiReportType,
                                                    ueIndex,
                                                    ulUEContext_p,
                                                    recvTTI,
                                                    transmissionMode,
                                                    dlUEContext_p,
                                                    /* +- SPR 17777 */
                                                    internalCellIndex,
                                                    &numOfBitsDecoded
                                                    );
                                        }

                                    }
                                }
                            }

                        }// for ends
                    }
                    /* reset bitmask cellIdxOfExpectedReportType */
#ifdef FDD_CONFIG
                    ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType = 0;
#elif TDD_CONFIG

                    /* SPR 18122 Changes Start*/
                    ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].cellIdxOfExpectedReportType = 0;

                    /* SPR 18122 Changes End*/
#endif
                }
                else
                {
                    /* either Rel9 or Rel 10 without CA */
                    /* 4x2 MIMO S */
                    /* 4x4 DL MIMO CHG START */
                    /* SPR 10177 changes start */
                    if (cqiPdu_p->cqiRiInformation.ri[0])
                    {
                        if ((TX_MODE_8 == dlUEContext_p->dlMIMOInfo.transmissionMode) ||
                                /* SPR 10177 changes end */
                                (isRIInCodebookSubsetRestriction( cqiPdu_p->cqiRiInformation.ri[0],
                                                                  dlUEContext_p->ueCategory,
                                                                  dlUEContext_p->dlMIMOInfo.transmissionMode,
                                                                  dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue,internalCellIndex)))
                        {
                            ulUEContext_p->pucchConfigInfo.ueReportedRI= cqiPdu_p->cqiRiInformation.ri[0];
                        } 
                        /* 4x4 DL MIMO CHG END */
                        /* 4x2 MIMO E */
                    }
                    /* SPR_4261_FIX Start */ 
                    if ( datalen == RI_MULTIPLEX_WITH_PUSCH)
                    {
                        return;
                    }
                    /* SPR_4261_FIX End */ 

                    ri = ulUEContext_p->pucchConfigInfo.ueReportedRI;
                    /* - CL-MIMO LJA*/

                    /* + CQI_4.1 */
                    transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
                    /* - CQI_4.1 */

                    LOG_MAC_MSG(MAC_CQI_DECODE_ID,LOGINFO,MAC_L1_INF,\
                            currentTTI,\
                            cqiReportType,\
                            ueIndex,\
                            recvTTI,datalen,ri,\
                            0.0,0.0,__func__,"CQI_DECODE");

                    decodeCqiReports(data_p,
                            cqiReportType,
                            ueIndex,
                            ulUEContext_p,
                            recvTTI,
                            transmissionMode,
                            dlUEContext_p,
                            subFrameNum,
                            internalCellIndex);
                    /*Cyclomatic Complexity changes - ends here */
                }
            }
        }
    }


    handleVendorSpecificInfoR9(vendorSpecific_p,&validReports,recvTTI,internalCellIndex);

    /* - E_CID_5.3 */

    return;
}
/* CA_phase2_csi_code Start */
/* coverity 64950 28June2014 */
/* coverity 64950 28June2014 */
/*****************************************************************************
 * Function Name  : calculateSizeOfReport
 * Inputs         : 1. ri
 *                  2. cqiReportType
 *                  3. internalCellIndex  
 * Outputs        : None
 * Returns        : None
 * Description    : This function return the bitWidth for Aperiodic report
 *****************************************************************************/
STATIC  UInt8 calculateSizeOfReport(UInt8 ri,
        ueExpectedReportType  cqiReportType,
        InternalCellIndex internalCellIndex)
{
    UInt8 numberOfBits = 0;
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    switch(cqiReportType)
    {
        case APERIODIC_MODE_1_2:
            if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
            {
                if(ri >1)
                {
                    numberOfBits = APERIODIC_MODE_12_RI_GT_1_FOR_4_ANT_BITS(cellParams_p);
                }
                else
                {
                    numberOfBits = APERIODIC_MODE_12_RI_1_FOR_4_ANT_BITS(cellParams_p);
                }
            }
            else
            {
                if(ri > 1)
                {
                    numberOfBits = APERIODIC_MODE_12_RI_2_BITS(cellParams_p);
                }
                else
                {
                    numberOfBits = APERIODIC_MODE_12_RI_1_BITS(cellParams_p);
                }
            }
            break;

        case APERIODIC_MODE_2_0:
            numberOfBits = APERIODIC_MODE_20_BITS(cellParams_p);
            break;

        case APERIODIC_MODE_2_2:
            if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
            {
                if(ri >1)
                {
                    numberOfBits = APERIODIC_MODE_22_RI_GT_1_FOR_4_ANT_BITS(cellParams_p);
                }
                else
                {
                    numberOfBits = APERIODIC_MODE_22_RI_1_FOR_4_ANT_BITS(cellParams_p);
                }
            }
            else
            {
                if(ri > 1)
                {
                    numberOfBits = APERIODIC_MODE_22_RI_2_BITS(cellParams_p);
                }
                else
                {
                    numberOfBits = APERIODIC_MODE_22_RI_1_BITS(cellParams_p);
                }
            }
            break;

        case APERIODIC_MODE_3_0:
            numberOfBits = APERIODIC_MODE_30_BITS(cellParams_p);
            break;

        case APERIODIC_MODE_3_1:
            if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
            {
                if(ri >1)
                {
                    numberOfBits =  APERIODIC_MODE_31_RI_GT_1_FOR_4_ANT_BITS(cellParams_p);
                }
                else
                {
                    numberOfBits = APERIODIC_MODE_31_RI_1_FOR_4_ANT_BITS(cellParams_p);
                }
            }
            else
            {
                if(ri > 1)
                {
                    numberOfBits = APERIODIC_MODE_31_RI_2_BITS(cellParams_p);
                }
                else
                {
                    numberOfBits = APERIODIC_MODE_31_RI_1_BITS(cellParams_p);
                }
            }
            break;
        default:
            lteWarning("Report Type Decoding not supported %d\n", 
                    /* PURIFY FIXES 2.4.1 start */
                    (UInt32)cqiReportType);
            /* PURIFY FIXES 2.4.1  end*/
            break;
    }
    return numberOfBits;
}
/* coverity 64950 28June2014 */
/* coverity 64950 28June2014 */
/*****************************************************************************
 * Function Name  : handleCQIReportWithDataOffsetNull
 * Inputs         : 1. ulUEContext_p
 *                  2. dlUEContext_p
 *                  3. recvTTI  - Its represent the tick of the message
 *                              recieved.
 *                  4. ueIndex
 *                  5. cqiPdu_p- pointer to the cqi pdu info
 *                  6. subFrameNum
 *                  7. bitMaskForAperiodicReport_p
 *                  8. internalCellIndex
 *                  9. numOfCCReported
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the cqi pdu in case data offset
 *                  is NULL.
 *****************************************************************************/
STATIC  void handleCQIReportWithDataOffsetNull(ULUEContext *ulUEContext_p,
        DLUEContext *dlUEContext_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt16  ueIndex,
        FAPI_cqiPduIndicationR9_st *cqiPdu_p,
        UInt16 subFrameNum,
#ifdef TDD_CONFIG
        UInt16 sysFrameNum,
#endif
        UInt8 *bitMaskForAperiodicReport_p,
        InternalCellIndex internalCellIndex,
        UInt8 numOfCCReported)
{
    ueExpectedReportType  cqiReportType = EXPECTED_REPORT_INVALID;
    /* SPR 15909 fix start */
    tickType_t currentTick = 0;
    /* SPR 15909 fix end */
    UInt8 cellIdxOfExpectedReportType = 0;
    UInt8 servCellIndex; 
    InternalCellIndex ueInternalCellIndex = INVALID_CELL_INDEX;
    DLCQIInfo *dlCQIInfo_p = PNULL;
    /* SPR 15909 fix start */
    tickType_t ulAperiodicCqiRequestTTI;
    /* SPR 15909 fix end */
    UInt16 count = 0;/*Coverity 69583 +-*/
    /** SPR 14032 Fix : Start **/
#ifdef FDD_CONFIG
    cqiReportType = 
        ulUEContext_p->reportFormatMap[subFrameNum].ueExpectedReport;
    cellIdxOfExpectedReportType = 
        ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType;
#elif TDD_CONFIG

    /* SPR 18122 Changes Start*/
    cqiReportType = ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].
        ueExpectedReport;
    cellIdxOfExpectedReportType = 
        ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].cellIdxOfExpectedReportType;

    /* SPR 18122 Changes End*/
#endif
    /* CA TDD PUCCH Changes*/        
/** SPR 14032 Fix : End **/
    if(dlUEContext_p->scellCount)
    {
        /* Rel 10 CA Ue */
        /* check serving cell index of cell for which report
         * is expected */
        for(servCellIndex = PCELL_SERV_CELL_IX; servCellIndex <= MAX_NUM_SCELL;
                servCellIndex++)
        {
            if(!servCellIndex)
            {
                /* Pcell */
                ueInternalCellIndex = internalCellIndex;
                dlCQIInfo_p = &(dlUEContext_p->dlCQIInfo);
                ulAperiodicCqiRequestTTI = ulUEContext_p->aperiodicCqiRequestTTI;
            }
            else
            {
                /* Scell */
                if(PNULL == dlUEContext_p->dlUeScellContext_p[servCellIndex])
                {
                    continue;
                }
                ueInternalCellIndex = dlUEContext_p->
                    dlUeScellContext_p[servCellIndex]->internalCellIndex;
/** SPR 14032 Fix : Start **/
#ifdef FDD_CONFIG
                cqiReportType = ulUEContext_p->ulUeScellContext_p[servCellIndex]->
                    cqiPmiRiReportFormatMapSCell[subFrameNum].ueExpectedReport;
#elif TDD_CONFIG
                cqiReportType = ulUEContext_p->ulUeScellContext_p[servCellIndex]->
                    
                    /* SPR 18122 Changes Start*/
                    cqiPmiRiReportFormatMapSCell[MODULO(sysFrameNum,2)][subFrameNum].ueExpectedReport;

                    /* SPR 18122 Changes End*/
#endif                    
                dlCQIInfo_p = &(dlUEContext_p->dlUeScellContext_p[servCellIndex]->
                        ScelldlCQIInfo);
                ulAperiodicCqiRequestTTI = ulUEContext_p->ulUeScellContext_p[servCellIndex]->
                    aperiodicCqiRequestTTI;

            }
            count = COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(ueInternalCellIndex);
            if((cellIdxOfExpectedReportType & servingCellBitMask_g[servCellIndex])
                    && (count != NUM_PERIODIC_REPORT_NODES))
            {
                /* SPR 4484 changes start */
                if (APERIODIC_CQI_REPORTING_MODE ==
                        dlCQIInfo_p->cqiTypeIndicator) 
                { 
                    /* If only aperiodic is  ON and data offset is 0 then 
                     *  even if below condition is not met then also we will 
                     *  skip from here.
                     */  
                    if   (recvTTI == ulUEContext_p->lastTriggeredAperiodicTTI)
                    {
                        /* SPR 4484 changes start */
                        /* CA_phase2_csi_code Start */
                        /* As part of CA Arch doc-Chap 15 Sec-15.5.9,
                         * The receiver module shall insert single node in
                         * ulGrantAperiodicCqiQueue_gp (of PCell) if for CA UEs the
                         * condition to trigger Aperoidic CSI occurs for single or
                         * both cells and update bits of node-variable
                         * aperiodicCQIToBeRequested */
                        /* Now in ulUEContext- aperiodicCQIToBeRequested will be set
                         * by UL strategy */
                        currentTick = GET_CURRENT_TICK_FOR_CELL(ueInternalCellIndex);
                        if((currentTick > (dlCQIInfo_p->aperiodicCqiRequestTTI + 
                                        APERIODIC_CQI_REQ_OFFSET)) && (currentTick >
                                        (ulAperiodicCqiRequestTTI + APERIODIC_CQI_REQ_OFFSET)))
                        {
                            *bitMaskForAperiodicReport_p |= servingCellBitMask_g[servCellIndex];
                        }
                        /* SPR 4484 changes end */
                    }
                }
                else
                {
                    if(numOfCCReported == 1)
                    {
                        if(cqiPdu_p->cqiRiInformation.ri[numOfCCReported -1])
                        {
                            handleRiReportWithDataOffsetNull(cqiReportType,
                                    ulUEContext_p,
                                    dlUEContext_p,
                                    ueIndex,
                                    &(cqiPdu_p->cqiRiInformation.ri[numOfCCReported - 1]),
                                    ueInternalCellIndex,
                                    /* CA_phase2_csi_code */
                                    servCellIndex);
                            /* +- SPR 17777 */
                        }
                    }
                }

            }
        }
        /* reset bitmask cellIdxOfExpectedReportType */
/** SPR 14032 Fix : Start **/
#ifdef FDD_CONFIG
        ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType = 0;
#elif TDD_CONFIG

        /* SPR 18122 Changes Start*/
        ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].cellIdxOfExpectedReportType = 0;

        /* SPR 18122 Changes End*/
#endif        
/** SPR 14032 Fix : End **/
    }
    else
    {
        /* Rel 9 or Rel 10 UE(without CA)*/
        /* SPR 4484 changes start */
        if (APERIODIC_CQI_REPORTING_MODE ==
                dlUEContext_p->dlCQIInfo.cqiTypeIndicator) 
        { 
            /* If only aperiodic is  ON and data offset is 0 then 
             *  even if below condition is not met then also we will 
             *  skip from here.
             */  
            if   (recvTTI == ulUEContext_p->lastTriggeredAperiodicTTI)
            {
                /* SPR 4484 changes start */
                /* CA_phase2_csi_code */
                currentTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
                if((currentTick > (dlUEContext_p->dlCQIInfo.aperiodicCqiRequestTTI + 
                                APERIODIC_CQI_REQ_OFFSET)) && (currentTick >
                                (ulUEContext_p->aperiodicCqiRequestTTI + APERIODIC_CQI_REQ_OFFSET)))
                    *bitMaskForAperiodicReport_p |= servingCellBitMask_g
                        [PCELL_SERV_CELL_IX];
                /* SPR 4484 changes end */
            }
        }
        else
        {
            if(numOfCCReported == 1)
            {
                if(cqiPdu_p->cqiRiInformation.ri[numOfCCReported - 1])
                {
                    handleRiReportWithDataOffsetNull(cqiReportType,
                            ulUEContext_p,
                            dlUEContext_p,
                            ueIndex,
                            &(cqiPdu_p->cqiRiInformation.ri[numOfCCReported-1]),
                            internalCellIndex,
                            /* CA_phase2_csi_code  */
                            PCELL_SERV_CELL_IX);
                    /* +- SPR 17777 */
                }
            }
        }
    }
}

/*****************************************************************************
 * Function Name  : handleCQIReportWithUlCqiLessThanCQISINRThreshold
 * Inputs         : 1. numberOfCCReported
 *                  2. ulUEContext_p
 *                  3. dlUEContext_p
 *                  4. recvTTI  - Its represent the tick of the message
 *                              recieved.
 *                  5. bitMaskForAperiodicReport           
 *                  6. subFrameNum
 *                  7. internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the cqi pdu in case ulcqi
 *                  is less than CQISINRThreshold
 *****************************************************************************/
STATIC  void handleCQIReportWithUlCqiLessThanCQISINRThreshold(UInt8 numberOfCCReported,
        ULUEContext *ulUEContext_p,
        DLUEContext *dlUEContext_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt8 *bitMaskForAperiodicReport_p,
        /* +- SPR 17777 */
        UInt16 subFrameNum
/** SPR 14032 Fix : Start **/
#ifdef TDD_CONFIG
        ,UInt16 sysFrameNum
#endif
/** SPR 14032 Fix : End **/
        )
        /* +- SPR 17777 */
{
    UInt8 servCellIndex;
    UInt8 cellIdxOfExpectedReportType = 0;
    UInt8 numOfCCProcessed = 0;
    DLCQIInfo *dlCQIInfo_p = PNULL;
/** SPR 14032 Fix : Start **/
    /* SPR 15909 fix start */
    tickType_t ulAperiodicCqiRequestTTI;
    /* SPR 15909 fix end */
#ifdef FDD_CONFIG
/** SPR 14032 Fix : End **/
    cellIdxOfExpectedReportType = 
          ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType;
#elif TDD_CONFIG

          /* SPR 18122 Changes Start*/
    cellIdxOfExpectedReportType = 
          ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].cellIdxOfExpectedReportType;
        
        /* SPR 18122 Changes End*/
#endif
    
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */
/** SPR 14032 Fix : Start **/
/* CA TDD pucch code change**/
    if(dlUEContext_p->scellCount)
    {
        /* Rel 10 CA Ue */
        /* check serving cell index of cell for which report
         * is expected */
        for(servCellIndex = 0; servCellIndex <= MAX_NUM_SCELL; servCellIndex++)
        {
            if(!servCellIndex)
            {
                /* Pcell */
                dlCQIInfo_p = &(dlUEContext_p->dlCQIInfo);
                ulAperiodicCqiRequestTTI = ulUEContext_p->aperiodicCqiRequestTTI;
            }
            else
            {
                if(PNULL == dlUEContext_p->dlUeScellContext_p[servCellIndex])
                {
                    continue;
                }
                dlCQIInfo_p = &(dlUEContext_p->dlUeScellContext_p[servCellIndex]->
                        ScelldlCQIInfo);
                ulAperiodicCqiRequestTTI = ulUEContext_p->ulUeScellContext_p[servCellIndex]->
                    aperiodicCqiRequestTTI;
            }
            if(numOfCCProcessed != numberOfCCReported)
            {
                if(cellIdxOfExpectedReportType & servingCellBitMask_g[servCellIndex])
                {
                    if (APERIODIC_CQI_REPORTING_MODE ==
                            dlCQIInfo_p->cqiTypeIndicator) 
                    { 
                        /* If only aperiodic is  ON and data offset is 0 then 
                         *  even if below condition is not met then also we will 
                         *  skip from here.
                         */  
                        if   (recvTTI == ulUEContext_p->lastTriggeredAperiodicTTI)
                        {
                            /* SPR 4484 changes start */
                            if((currentTTI > (dlCQIInfo_p->aperiodicCqiRequestTTI + 
                                            APERIODIC_CQI_REQ_OFFSET)) && (currentTTI >
                                            (ulAperiodicCqiRequestTTI + APERIODIC_CQI_REQ_OFFSET)))
                            {
                                *bitMaskForAperiodicReport_p |= servingCellBitMask_g[servCellIndex];
                            }
                            /* SPR 4484 changes end */
                        }
                    }
                    numOfCCProcessed += 1;
                }
            }
            else
            {
                break;
            }
        }
        /* reset bitmask cellIdxOfExpectedReportType */
#ifdef FDD_CONFIG      
        ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType = 0;
#elif TDD_CONFIG

        /* SPR 18122 Changes Start*/
        ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].cellIdxOfExpectedReportType = 0;
        
        /* SPR 18122 Changes End*/
#endif        
/** SPR 14032 Fix : End **/
    }
    else
    {
/* CA TDD pucch code change**/
        /* Rel 9 or Rel 10 UE(without CA)*/
        /* SPR 4484 changes start */
        if (APERIODIC_CQI_REPORTING_MODE ==
                dlUEContext_p->dlCQIInfo.cqiTypeIndicator) 
        { 
            /* If only aperiodic is  ON and data offset is 0 then 
             *  even if below condition is not met then also we will 
             *  skip from here.
             */  
            if   (recvTTI == ulUEContext_p->lastTriggeredAperiodicTTI)
            {
                /* SPR 4484 changes start */
                if((currentTTI > (dlUEContext_p->dlCQIInfo.aperiodicCqiRequestTTI + 
                                APERIODIC_CQI_REQ_OFFSET)) && (currentTTI >
                                (ulUEContext_p->aperiodicCqiRequestTTI + APERIODIC_CQI_REQ_OFFSET)))
                {
                    *bitMaskForAperiodicReport_p |= servingCellBitMask_g[PCELL_SERV_CELL_IX];
                }
                /* SPR 4484 changes end */
            }
        }
    }

}
/* CA_phase2_csi_code end */

/* CA_phase2_csi_code Start */
/*****************************************************************************
 * Function Name  : processCQIReportForOneCellForCAUE
 * Inputs         :  ulUEContext_p
 *                   dlUEContext_p
 *                   datalen
 *                   recvTTI  - Its represent the tick of the message
 *                              recieved.
 *                   data_p
 *                   ueIndex
 *                   cqiPdu_p- pointer to the cqi pdu info
 *                   subFrameNum
 *                   internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the cqi pdu in case data offset
 *                  is NULL.
 *****************************************************************************/
STATIC  void processCQIReportForOneCellForCAUE(ULUEContext *ulUEContext_p,
        DLUEContext *dlUEContext_p,
        FAPI_cqiPduIndicationR9_st *cqiPdu_p,
        UInt32  datalen,
        UInt16 subFrameNum,
#ifdef TDD_CONFIG
        UInt16 sysFrameNum,
#endif
        UInt8 *data_p,
        UInt16  ueIndex,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
{
/*ca-tdd pucch code changes*/
    UInt8 transmissionMode = 0;
    ueExpectedReportType  cqiReportType = EXPECTED_REPORT_INVALID;
    UInt16   count = 0;/*Coverity 69584 +-*/
    UInt8 cellIdxOfExpectedReportType;
    DLUESCellContext *dlUeScellContext_p = PNULL;
    UInt8 servCellIndex;
#ifdef FDD_CONFIG
    cqiReportType = ulUEContext_p->reportFormatMap[subFrameNum].ueExpectedReport;
    cellIdxOfExpectedReportType = 
           ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType;
#elif TDD_CONFIG

           /* SPR 18122 Changes Start*/
    cqiReportType = ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].ueExpectedReport;
    cellIdxOfExpectedReportType = 
           ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].cellIdxOfExpectedReportType;

        /* SPR 18122 Changes End*/
#endif
    /* report is either for PCell or Scell */
    if(cellIdxOfExpectedReportType & servingCellBitMask_g[PCELL_SERV_CELL_IX])
    {
        count = COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(internalCellIndex);
        if(count < NUM_PERIODIC_REPORT_NODES)
        {
            /* Pcell */
            /* 4x2 MIMO S */
            /* 4x4 DL MIMO CHG START */
            if ((1 == cqiPdu_p->cqiRiInformation.numberOfCCReported)
                    && (cqiPdu_p->cqiRiInformation.ri[0]))
            {
                if (isRIInCodebookSubsetRestriction( cqiPdu_p->cqiRiInformation.ri[0],
                            dlUEContext_p->ueCategory,
                            dlUEContext_p->dlMIMOInfo.transmissionMode,
                            dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue,internalCellIndex))
                {
                    ulUEContext_p->pucchConfigInfo.ueReportedRI= cqiPdu_p->cqiRiInformation.ri[0];
                } 
                /* 4x4 DL MIMO CHG END */
                /* 4x2 MIMO E */
            }
            /* SPR_4261_FIX Start */ 
            if ( datalen == RI_MULTIPLEX_WITH_PUSCH)
            {
                return;
            }
            /* SPR_4261_FIX End */ 
            transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
            
            LOG_MAC_MSG(MAC_CQI_DECODE_ID,LOGINFO,MAC_L1_INF,\
                    getCurrentTick(),\
                    cqiReportType,\
                    ueIndex,\
                    recvTTI, datalen, 
                    ulUEContext_p->pucchConfigInfo.ueReportedRI,
                    0.0,0.0,__func__,"CQI_DECODE");
            decodeCqiReports(data_p,
                    cqiReportType,
                    ueIndex,
                    ulUEContext_p,
                    recvTTI,
                    transmissionMode,
                    dlUEContext_p,
                    subFrameNum,
                    internalCellIndex);
            /*Cyclomatic Complexity changes - ends here */
        }
        /* For TDD_CA . pls take care */
    }
    for(servCellIndex = START_SCELL_INDEX; servCellIndex <= MAX_NUM_SCELL;
            servCellIndex++)
    {
        dlUeScellContext_p = dlUEContext_p->dlUeScellContext_p[servCellIndex];
        if(PNULL == dlUeScellContext_p)
        {
            continue;
        }
        count = COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(dlUeScellContext_p->internalCellIndex);
        if(cellIdxOfExpectedReportType & servingCellBitMask_g[servCellIndex])
        {
            if(count < NUM_PERIODIC_REPORT_NODES)
            {
                /* 4x2 MIMO S */
                /* 4x4 DL MIMO CHG START */
                if ((1 == cqiPdu_p->cqiRiInformation.numberOfCCReported)
                    && (cqiPdu_p->cqiRiInformation.ri[0]))
                {
                    if (isRIInCodebookSubsetRestriction( cqiPdu_p->cqiRiInformation.ri[0],
                                dlUEContext_p->ueCategory,
                                dlUeScellContext_p->ScelldlMIMOInfo.transmissionMode,
                                dlUeScellContext_p->codebookSubsetRestrictionv1020.cbsrValue,
                                dlUeScellContext_p->internalCellIndex))
                    {
                        ulUEContext_p->ulUeScellContext_p[servCellIndex]->
                            scellpucchConfigInfo.ueReportedRI =  
                            cqiPdu_p->cqiRiInformation.ri[0];
                    } 
                    /* 4x4 DL MIMO CHG END */
                    /* 4x2 MIMO E */
                }
                /* SPR_4261_FIX Start */ 
                if ( datalen == RI_MULTIPLEX_WITH_PUSCH)
                {
                    return;
                }
                /* SPR_4261_FIX End */ 
/** SPR 14032 Fix : Start **/
#ifdef FDD_CONFIG                
                cqiReportType = ulUEContext_p->
                    ulUeScellContext_p[servCellIndex]->
                    cqiPmiRiReportFormatMapSCell[subFrameNum].ueExpectedReport;
#elif TDD_CONFIG

                    /* SPR 18122 Changes Start*/
                cqiReportType = ulUEContext_p->
                    ulUeScellContext_p[servCellIndex]->
                    cqiPmiRiReportFormatMapSCell[MODULO(sysFrameNum,2)][subFrameNum].ueExpectedReport;

                    /* SPR 18122 Changes End*/
#endif                    
/** SPR 14032 Fix : End **/
                transmissionMode = dlUeScellContext_p->ScelldlMIMOInfo.transmissionMode;
                
                LOG_MAC_MSG(MAC_CQI_DECODE_ID, LOGINFO, MAC_L1_INF, 
                        getCurrentTick(), cqiReportType, ueIndex,
                        recvTTI, datalen, 
                        ulUEContext_p->ulUeScellContext_p[servCellIndex]->
                        scellpucchConfigInfo.ueReportedRI,
                        0.0,0.0,__func__,"CQI_DECODE");
                decodeCqiReports(data_p,
                        cqiReportType,
                        ueIndex,
                        ulUEContext_p,
                        recvTTI,
                        transmissionMode,
                        dlUEContext_p,
                        subFrameNum,
                        dlUeScellContext_p->internalCellIndex
                        );
                /*Cyclomatic Complexity changes - ends here */
            }
        }
        break;
    }
}

/* coverity 64951 28June2014 */
/* coverity 64951 28June2014 */
/*****************************************************************************
 * Function Name  : getNumCellsForCSIReport
 * Inputs         : 1.cellIdxOfExpectedReportType 
 *                  2.scellCount 
 * Outputs        : None
 * Returns        : Number of cells for which report is expected
 * Description    : This function returns Number of cells for which report is
 *                  expected
 *****************************************************************************/
STATIC  UInt8 getNumCellsForCSIReport(UInt8 cellIdxOfExpectedReportType,
        UInt8 scellCount)
{
    UInt8 servCellIndex;
    UInt8 numOfCells = 0;
    for(servCellIndex = PCELL_SERV_CELL_IX; servCellIndex <= scellCount; 
            servCellIndex++)
    {
        if(cellIdxOfExpectedReportType & servingCellBitMask_g[servCellIndex])
        {
            numOfCells++;
        }
    }
    return numOfCells;
}
/* coverity 64951 28June2014 */
/* coverity 64951 28June2014 */
/*****************************************************************************
 * Function Name  : handleVendorSpecificInfoR9 
 * Inputs         : 1. vendorSpecific_p  - pointer to vendorSpecific Information
 *                   
 *                  2. validReports_p- pointer to valid report 
 *                  3. recvTTI
 *                  4. internalCellIndex - Cell_index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the AOA received in Vendorspecific 
 *                   information
 * *****************************************************************************/
void handleVendorSpecificInfoR9( FAPI_VendorSpecificR9_st *vendorSpecific_p,
        UInt8 *validReports_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt16 *angleOfArrival = PNULL;
    ULUEContext *ulUEContext_p = PNULL;
    EcidReportParams  *eCidReportParams_p = PNULL;
    FAPI_UEMeasurementIndicationR9_st *ueMeasInfoForUEs_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
    FAPI_AOA_RecievedForUE_st *aoaRel9Info = PNULL;
    /* +- SPR 18268 */
    UInt16 count = 0;
    UInt16 numOfUEs = 0;
    /* +- SPR 18268 */
    UInt16 rnti=0;
    UInt16  ueIndex = 0;
    UInt8 counter = 0;
    RNTIInfo *rntiInfo_p = PNULL;

    if(PNULL != vendorSpecific_p)
    {
        ueMeasInfoForUEs_p = (FAPI_UEMeasurementIndicationR9_st *)&
            vendorSpecific_p->ueMeasInfoForUEs;
        if(PNULL != ueMeasInfoForUEs_p)
        {
            numOfUEs = ueMeasInfoForUEs_p->numOfUEs;
            for(count = 0; count < numOfUEs;count++)
            {
                rnti = MAC_PHY_CONVERT_16(ueMeasInfoForUEs_p->aoaRel9Info[count].rnti);
                rntiInfo_p = getUeIdxFromRNTIMap(rnti,internalCellIndex);
                if(PNULL != rntiInfo_p)
                {
                    if(FREERNTI != rntiInfo_p->rntiFlag)
                    {
                        ueIndex = rntiInfo_p->index;
                        ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
                        if((TRUE != ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag) && \
                                PNULL != ulUEContext_p)
                        {

                            eCidReportParams_p = &ulUEContext_p->eCidReportParams;

                            VALIDATE_AND_UPDATE_AOA_REPORTS(
                                    ueMeasInfoForUEs_p->aoaRel9Info,
                                    angleOfArrival,
                                    *validReports_p,
                                    count);

                            SAVE_ECID_AOA_REPORT_IN_UE_CONTXT(eCidReportParams_p,
                                    angleOfArrival,
                                    recvTTI);

                            if(NO_REPORT_TYPE_REQUIRED != eCidReportParams_p->typeofReportsRequired && \
                                    TYPE_2_TA_REPORT_REQUIRED != eCidReportParams_p->typeofReportsRequired)
                            {
                                checkValidReportsAndPutEntryInEcidMgr(ueIndex,
                                        *validReports_p,internalCellIndex);
                            }
                        }

                        if (dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag)
                        {
                            lteWarning(" Pending Delete Flag True for UE Index %d %s",
                                    ueIndex,__func__);
                            continue ;
                        }
                        dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
                        /* SPR 7973 chages start */
                        if( PNULL ==  dlUEContext_p )
                        {
                            continue;
                        }
                        /* SPR 7973 chages end */

                        aoaRel9Info =  &ueMeasInfoForUEs_p->aoaRel9Info[count];
                        for (counter = 0; counter < cellParams_p->numOfTxAnteenas; counter++)
                        {
                            if(MAX_VALUE_OF_AOA >= aoaRel9Info->AoA[counter])
                            {
                                dlUEContext_p->AoA[counter] = 
                                    calculateTimeAvgAoA(dlUEContext_p->AoA[counter] ,aoaRel9Info->AoA[counter]);  
                            }
                        }

                    }
                }
            }
        }
    }
}
#else
/*****************************************************************************
 * Function Name  : processCQIIndication
 * Inputs         : vendorSpecific_p - pointer to vendor specific field,
 *                  cqiMsg_p  - pointer to CQI message(Payload part),
 *                  recvTTI  - Its represent the tick of the message receiived,
 *                  sysFrameNum and subFrameNum
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the CQI Indication msg from PHY.
 *****************************************************************************/
/* SPR_4261_FIX Start */ 
/*Based on L1 interface we are keeping this value 0*/
#define RI_MULTIPLEX_WITH_PUSCH   0
/*SPR 19433 +*/

#if 0
void processCQIIndication(void *VendorSpecificParam_p,
        void *msg_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        /*SPR 5620 START*/
#ifdef TDD_CONFIG
        UInt16 sysFrameNum,
#endif
        /*SPR 5620 END*/
        UInt16 subFrameNum,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
        )
/* - TM7_8 Changes End */
{
    UInt8   *data_p       = PNULL;
    UInt16  ueIndex       = 0;
    UInt16  iterator      = 0;
    UInt16  recvRnti      = 0;
    UInt32  datalen       = 0;
    ueExpectedReportType  cqiReportType = EXPECTED_REPORT_INVALID;
    FAPI_rxCqiIndication_st *cqiMsg_p = (FAPI_rxCqiIndication_st *)msg_p;
    FAPI_VendorSpecific_st *vendorSpecific_p=(FAPI_VendorSpecific_st*)VendorSpecificParam_p;
    UInt16  numOfPdu      = MAC_PHY_CONVERT_16(cqiMsg_p->numOfCqi);
    RNTIInfo *rntiInfo_p  = PNULL;
    FAPI_cqiPduIndication_st *cqiPdu_p = PNULL; 
    ULUEContext *ulUEContext_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
    UInt16  timingAdvance = 0; /* SPR 2628 Changes */
    
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */
    /*SPR 8166 change start */
    UInt8 *refPdu_p = PNULL;
    UInt16  offsetNextPdu = 0;
    /*SPR 8166 change end */
    /* + E_CID_5.3 */
    UInt8 count = 0;
    FAPI_UEMeasurementIndication_st *ueMeasInfoForUEs_p = PNULL;
    /* - E_CID_5.3 */
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    /* + CQI_4.1 */
    UInt8 transmissionMode = 0;
    /* - CQI_4.1 */
    if(cqiMsg_p->numOfCqi)
    {
        /*SPR 8166 change start */
        refPdu_p = ((UInt8 *)&cqiMsg_p->cqiPduInfo[0]) +
            (numOfPdu * sizeof(FAPI_cqiPduIndication_st));
        /*SPR 8166 change end */
        for (iterator = 0; iterator < numOfPdu; iterator++)
        {
            /* SPR 4073 Start*/
            /*CA Changes start  */
            count = COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(internalCellIndex);
            /*CA Changes end  */
            if (count == NUM_PERIODIC_REPORT_NODES)
            {
                /* Due to configured CQI Configuration of all the UEs, the influx of reports
                 * is more that what L2 has been able to process per TTI as per NUM_CQI_NODES
                 * and the reports are just getting accumulated, exhausting system memory.
                 * Hence drop the report nodes till the situation improves*/
                break;
                /* SPR 4073 End*/
            }
            cqiPdu_p = (FAPI_cqiPduIndication_st *)&cqiMsg_p->cqiPduInfo[iterator]; 
            recvRnti =  MAC_PHY_CONVERT_16(cqiPdu_p->rnti);

            /* getUeIndexFromRNTIMap returns NULL if not configured RARNTI or 
             * CRNTI Range and rntiFlag == FREERNTI is  deactive.
             */
            /*CA Changes start  */
            rntiInfo_p = getUeIdxFromRNTIMap(recvRnti, internalCellIndex);
            /*CA Changes end  */
            if (PNULL == rntiInfo_p || FREERNTI == rntiInfo_p->rntiFlag)
            {
                lteWarning("processCQIIndication: Recv Rnti %d is not Active at "
                        "Mac Layer\n",recvRnti);   
                LOG_MAC_MSG(MAC_CQI_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
                        currentTTI,\
                        recvRnti,
#ifndef __x86_64__
                        (UInt32)rntiInfo_p,
#else
                        (UInt64)rntiInfo_p,
#endif
                        (rntiInfo_p ? rntiInfo_p->rntiFlag : 0), \
                        recvTTI,0,\
                        0.0,0.0,__func__,"CQI_RNTIERR");
                continue ;
            }
            /*SPR_3061_3068_FIX*/
            if (TCRNTI == rntiInfo_p->rntiFlag)
            {
                /* + CQI_5.0 */
                /* If Dataoffset is 0, it means that decoding of CQI has failed for 
                 * that UE so continue */
                if(0 == cqiPdu_p->dataOffset)
                {
                    continue;
                } 

                /* - CQI_5.0 */
                /* tcRnti = rntiInfo_p->index; TDD Warning Fix */

            }  
            else 
            {
                ueIndex = rntiInfo_p->index;
                if (dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag)
                {
                    lteWarning(" Pending Delete Flag True for UE Index %d %s",
                            ueIndex,__func__);
                    LOG_MAC_MSG(MAC_CQI_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
                            currentTTI,\
                            recvRnti,\
                            ueIndex,\
                            recvTTI,0,0,\
                            0.0,0.0,__func__,"CQI_UEDELETE");
                    continue;
                }
                dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
                ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
                timingAdvance = MAC_PHY_CONVERT_16(cqiPdu_p->timingAdvance);
                /* + CQI_5.0 */
                /*SPR 5620 START*/
#ifdef FDD_CONFIG
                cqiReportType = ulUEContext_p->reportFormatMap[subFrameNum].
                    ueExpectedReport;
#elif TDD_CONFIG

                    /* SPR 18122 Changes Start*/
                cqiReportType = ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].
                    ueExpectedReport;

                    /* SPR 18122 Changes End*/
#endif
                /*SPR 5620 END*/

                /* If Dataoffset is 0, it means either only RI received at PUSCH or
                 * decoding of CQI has failed for that UE.
                 * If decoding of CQI has failed or  ulCqi is less than threshold then 
                 * don't update DL MCS so continue */
#ifdef PERF_STATS
                UPDATE_UE_PUCCH_SINR_STATS(ueIndex, cqiPdu_p->ulCqi, subFrameNum);
#endif

                /* + SPR_5415  */
                if  (0 == cqiPdu_p->dataOffset) 
                {
                    /* SPR 4484 changes start */
                    if (APERIODIC_CQI_REPORTING_MODE ==
                            dlUEContext_p->dlCQIInfo.cqiTypeIndicator) 
                    { 
                        /* If only aperiodic is  ON and data offset is 0 then 
                         *  even if below condition is not met then also we will 
                         *  skip from here.
                         */  
                        if   (recvTTI == ulUEContext_p->lastTriggeredAperiodicTTI)
                        {
                            /* SPR 4484 changes start */
                            ulUEContext_p->aperiodicCQIToBeRequested = TRUE;
                            /* SPR 4484 changes end */
                        }
                        continue;  
                    }
                    else
                    {
                        /*Cyclomatic Complexity changes - starts here */
                        handleRiReportWithDataOffsetNull(cqiReportType,
                                ulUEContext_p,
                                dlUEContext_p,
                                ueIndex,
#if defined(FLEXRAN)
/*SPR 21001 Changes start */
                                &(cqiPdu_p->ri[0]),
#else
                                &(cqiPdu_p->ri),
/*SPR 21001 Changes end */
#endif
                                internalCellIndex,
                                /* CA_phase2_csi_code */
                                0);
                        /* +- SPR 17777 */
                        /*Cyclomatic Complexity changes - ends here */
                    }
                    continue;
                }
                else if (cqiPdu_p->ulCqi < cellParams_p->pucchCQISINRThreshold)
                { 
                    /* SPR 4484 changes start */
                    if ((APERIODIC_CQI_REPORTING_MODE ==
                                dlUEContext_p->dlCQIInfo.cqiTypeIndicator) &&
                            (recvTTI == ulUEContext_p->lastTriggeredAperiodicTTI))
                    {
                        ulUEContext_p->aperiodicCQIToBeRequested = TRUE;

                    }
                    /* SPR 4484 changes end */
                    /* - CQI_5.0 */
                    continue;  
                }  
                /*CA Changes start  */
                    handleTimingAdvance(dlUEContext_p, ueIndex, timingAdvance,internalCellIndex, getCurrentTickUl());
                /*CA Changes end  */
                /*CQI_PDCCH_ORDER_FIX_START*/
                if (UL_SYNC_LOSS_OFF == 
                        checkSyncLossTimerExpiry(dlUEContext_p))
                {
                    dlUEContext_p->lastSyncDetectedTick = getCurrentTick();
                }
                /* - SPR_5415 */
                /*CQI_PDCCH_ORDER_FIX_END*/
                LOG_UT(MAC_TA_INSERTION,LOGDEBUG,MAC_L1_INF,\
                        currentTTI,\
                        getCurrentTick(),dlUEContext_p->lastSyncDetectedTick,\
                        ueIndex,timingAdvance,\
                        DEFAULT_INT_VALUE,\
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                        FUNCTION_NAME,"LAST_SYNC_TICK");
                /* SPR 2628 Changes End */
                /* CQI will come on CRNTI and not on T-CRNTI.
                 * Check if CQI is received on PUCCH  and power control based on 
                 * SINR is enable .
                 * If yes thn call updateAndCheckCqiSinr to trigger TPC  
                 * based on received SINR. */
                /* Time Averaging changes start */
                CHECK_AND_UPDATE_CQI_SINR(dlUEContext_p, cqiPdu_p->ulCqi,
                        subFrameNum, currentTTI
                        /* CA changes Start */
                        ,internalCellIndex
                        /* CA changes End */
                        );
                /* Time Averaging changes end */
                /* calculate type of report */
                /* SPR 8166 change start */ 
                datalen = MAC_PHY_CONVERT_16(cqiPdu_p->length); 
                data_p = offsetNextPdu + (UInt8*)(refPdu_p);
                offsetNextPdu += datalen;
                /* SPR 8166 change end */ 
                /* + CL-MIMO LJA*/   
#if defined(FLEXRAN)
/*SPR 21001 Changes start */
                if (cqiPdu_p->ri[0])
#else
		if (cqiPdu_p->ri)
/*SPR 21001 Changes end */
#endif
                {
                    /* 4x2 MIMO S */
                    /* 4x4 DL MIMO CHG START */
                    /* SPR 10177 changes start */
                    if ((TX_MODE_8 == dlUEContext_p->dlMIMOInfo.transmissionMode) ||
                            /* SPR 10177 changes end */
#if defined(FLEXRAN)
/*SPR 21001 Changes start */
                            (isRIInCodebookSubsetRestriction( cqiPdu_p->ri[0],
                                                              dlUEContext_p->ueCategory,
                                                              dlUEContext_p->dlMIMOInfo.transmissionMode,
                                                              dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue,internalCellIndex)))
#else
                            (isRIInCodebookSubsetRestriction( cqiPdu_p->ri,
                                                              dlUEContext_p->ueCategory,
                                                              dlUEContext_p->dlMIMOInfo.transmissionMode,
                                                              dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue,internalCellIndex)))
/*SPR 21001 Changes end */
#endif
                    {
#ifdef FLEXRAN
                        ulUEContext_p->pucchConfigInfo.ueReportedRI= cqiPdu_p->ri[0];
#else
                        ulUEContext_p->pucchConfigInfo.ueReportedRI= cqiPdu_p->ri;
#endif
                    } 
                    /* 4x4 DL MIMO CHG END */
                    /* 4x2 MIMO E */
                }
                /* SPR_4261_FIX Start */ 
                if ( datalen == RI_MULTIPLEX_WITH_PUSCH)
                {
                    return;
                }
                /* SPR_4261_FIX End */ 
                transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
                LOG_MAC_MSG(MAC_CQI_DECODE_ID,LOGINFO,MAC_L1_INF,\
                        currentTTI,\
                        cqiReportType,\
                        ueIndex,\
                        recvTTI,datalen, 
                        ulUEContext_p->pucchConfigInfo.ueReportedRI,
                        0.0,0.0,__func__,"CQI_DECODE");
                decodeCqiReports(data_p,
                        cqiReportType,
                        ueIndex,
                        ulUEContext_p,
                        recvTTI,
                        transmissionMode,
                        dlUEContext_p,
                        subFrameNum,
                        internalCellIndex);
                /*Cyclomatic Complexity changes - ends here */
            }
        }
    }

    if(PNULL != vendorSpecific_p)
    {
        ueMeasInfoForUEs_p = (FAPI_UEMeasurementIndication_st *)&
            vendorSpecific_p->ueMeasInfoForUEs;
        /* SPR 9829 changes start */
        macProcessVendorSpecificFieldsSRSCQIInd(ueMeasInfoForUEs_p,recvTTI,internalCellIndex);
        /* SPR 9829 changes end */
    }
}
#endif
/* coverity 64950 28June2014 */
/* coverity 64950 28June2014 */
/*****************************************************************************
 * Function Name  : handleCQIReportWithDataOffsetNull
 * Inputs         : 1. ulUEContext_p
 *                  2. dlUEContext_p
 *                  3. recvTTI  - Its represent the tick of the message
 *                              recieved.
 *                  4. ueIndex
 *                  5. cqiPdu_p- pointer to the cqi pdu info
 *                  6. subFrameNum
 *                  7. bitMaskForAperiodicReport_p
 *                  8. internalCellIndex
 *                  9. numOfCCReported
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the cqi pdu in case data offset
 *                  is NULL.
 *****************************************************************************/
STATIC inline void handleCQIReportWithDataOffsetNull(ULUEContext *ulUEContext_p,
        DLUEContext *dlUEContext_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt16  ueIndex,
        FAPI_cqiPduIndication_st *cqiPdu_p,
        UInt16 subFrameNum,
#ifdef TDD_CONFIG
        UInt16 sysFrameNum,
#endif
        UInt8 *bitMaskForAperiodicReport_p,
        InternalCellIndex internalCellIndex,
        UInt8 numOfCCReported)
{
    ueExpectedReportType  cqiReportType = EXPECTED_REPORT_INVALID;
    /* SPR 15909 fix start */
    tickType_t currentTick = 0;
    /* SPR 15909 fix end */
    UInt8 cellIdxOfExpectedReportType = 0;
    UInt8 servCellIndex; 
    InternalCellIndex ueInternalCellIndex = INVALID_CELL_INDEX;
    DLCQIInfo *dlCQIInfo_p = PNULL;
    /* SPR 15909 fix start */
    tickType_t ulAperiodicCqiRequestTTI;
    /* SPR 15909 fix end */
    UInt16 count = 0;/*Coverity 69583 +-*/
    /** SPR 14032 Fix : Start **/
#ifdef FDD_CONFIG
    cqiReportType = 
        ulUEContext_p->reportFormatMap[subFrameNum].ueExpectedReport;
    cellIdxOfExpectedReportType = 
        ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType;
#elif TDD_CONFIG

    /* SPR 18122 Changes Start*/
    cqiReportType = ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].
        ueExpectedReport;
    cellIdxOfExpectedReportType = 
        ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].cellIdxOfExpectedReportType;

    /* SPR 18122 Changes End*/
#endif
    /* CA TDD PUCCH Changes*/        
/** SPR 14032 Fix : End **/
    if(dlUEContext_p->scellCount)
    {
        /* Rel 10 CA Ue */
        /* check serving cell index of cell for which report
         * is expected */
        for(servCellIndex = PCELL_SERV_CELL_IX; servCellIndex <= MAX_NUM_SCELL;
                servCellIndex++)
        {
            if(!servCellIndex)
            {
                /* Pcell */
                ueInternalCellIndex = internalCellIndex;
                dlCQIInfo_p = &(dlUEContext_p->dlCQIInfo);
                ulAperiodicCqiRequestTTI = ulUEContext_p->aperiodicCqiRequestTTI;
            }
            else
            {
                /* Scell */
                if(PNULL == dlUEContext_p->dlUeScellContext_p[servCellIndex])
                {
                    continue;
                }
                ueInternalCellIndex = dlUEContext_p->
                    dlUeScellContext_p[servCellIndex]->internalCellIndex;
/** SPR 14032 Fix : Start **/
#ifdef FDD_CONFIG
                cqiReportType = ulUEContext_p->ulUeScellContext_p[servCellIndex]->
                    cqiPmiRiReportFormatMapSCell[subFrameNum].ueExpectedReport;
#elif TDD_CONFIG
                cqiReportType = ulUEContext_p->ulUeScellContext_p[servCellIndex]->
                    
                    /* SPR 18122 Changes Start*/
                    cqiPmiRiReportFormatMapSCell[MODULO(sysFrameNum,2)][subFrameNum].ueExpectedReport;

                    /* SPR 18122 Changes End*/
#endif                    
                dlCQIInfo_p = &(dlUEContext_p->dlUeScellContext_p[servCellIndex]->
                        ScelldlCQIInfo);
                ulAperiodicCqiRequestTTI = ulUEContext_p->ulUeScellContext_p[servCellIndex]->
                    aperiodicCqiRequestTTI;

            }
            count = COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(ueInternalCellIndex);
            if((cellIdxOfExpectedReportType & servingCellBitMask_g[servCellIndex])
                    && (count != NUM_PERIODIC_REPORT_NODES))
            {
                /* SPR 4484 changes start */
                if (APERIODIC_CQI_REPORTING_MODE ==
                        dlCQIInfo_p->cqiTypeIndicator) 
                { 
                    /* If only aperiodic is  ON and data offset is 0 then 
                     *  even if below condition is not met then also we will 
                     *  skip from here.
                     */  
                    if   (recvTTI == ulUEContext_p->lastTriggeredAperiodicTTI)
                    {
                        /* SPR 4484 changes start */
                        /* CA_phase2_csi_code Start */
                        /* As part of CA Arch doc-Chap 15 Sec-15.5.9,
                         * The receiver module shall insert single node in
                         * ulGrantAperiodicCqiQueue_gp (of PCell) if for CA UEs the
                         * condition to trigger Aperoidic CSI occurs for single or
                         * both cells and update bits of node-variable
                         * aperiodicCQIToBeRequested */
                        /* Now in ulUEContext- aperiodicCQIToBeRequested will be set
                         * by UL strategy */
                        currentTick = GET_CURRENT_TICK_FOR_CELL(ueInternalCellIndex);
                        if((currentTick > (dlCQIInfo_p->aperiodicCqiRequestTTI + 
                                        APERIODIC_CQI_REQ_OFFSET)) && (currentTick >
                                        (ulAperiodicCqiRequestTTI + APERIODIC_CQI_REQ_OFFSET)))
                        {
                            *bitMaskForAperiodicReport_p |= servingCellBitMask_g[servCellIndex];
                        }
                        /* SPR 4484 changes end */
                    }
                }
                else
                {
                    if(numOfCCReported == 1)
                    {
                        if(cqiPdu_p->ri[numOfCCReported -1])
                        {
                            handleRiReportWithDataOffsetNull(cqiReportType,
                                    ulUEContext_p,
                                    dlUEContext_p,
                                    ueIndex,
                                    &(cqiPdu_p->ri[numOfCCReported - 1]),
                                    ueInternalCellIndex,
                                    /* CA_phase2_csi_code */
                                    servCellIndex);
                            /* +- SPR 17777 */
                        }
                    }
                }

            }
        }
        /* reset bitmask cellIdxOfExpectedReportType */
/** SPR 14032 Fix : Start **/
#ifdef FDD_CONFIG
        ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType = 0;
#elif TDD_CONFIG

        /* SPR 18122 Changes Start*/
        ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].cellIdxOfExpectedReportType = 0;

        /* SPR 18122 Changes End*/
#endif        
/** SPR 14032 Fix : End **/
    }
    else
    {
        /* Rel 9 or Rel 10 UE(without CA)*/
        /* SPR 4484 changes start */
        if (APERIODIC_CQI_REPORTING_MODE ==
                dlUEContext_p->dlCQIInfo.cqiTypeIndicator) 
        { 
            /* If only aperiodic is  ON and data offset is 0 then 
             *  even if below condition is not met then also we will 
             *  skip from here.
             */  
            if   (recvTTI == ulUEContext_p->lastTriggeredAperiodicTTI)
            {
                /* SPR 4484 changes start */
                /* CA_phase2_csi_code */
                currentTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
                if((currentTick > (dlUEContext_p->dlCQIInfo.aperiodicCqiRequestTTI + 
                                APERIODIC_CQI_REQ_OFFSET)) && (currentTick >
                                (ulUEContext_p->aperiodicCqiRequestTTI + APERIODIC_CQI_REQ_OFFSET)))
                    *bitMaskForAperiodicReport_p |= servingCellBitMask_g
                        [PCELL_SERV_CELL_IX];
                /* SPR 4484 changes end */
            }
        }
        else
        {
            if(numOfCCReported == 1)
            {
                if(cqiPdu_p->ri[numOfCCReported - 1])
                {
                    handleRiReportWithDataOffsetNull(cqiReportType,
                            ulUEContext_p,
                            dlUEContext_p,
                            ueIndex,
                            &(cqiPdu_p->ri[numOfCCReported-1]),
                            internalCellIndex,
                            /* CA_phase2_csi_code  */
                            PCELL_SERV_CELL_IX);
                    /* +- SPR 17777 */
                }
            }
        }
    }
}

/*****************************************************************************
 * Function Name  : handleCQIReportWithUlCqiLessThanCQISINRThreshold
 * Inputs         : 1. numberOfCCReported
 *                  2. ulUEContext_p
 *                  3. dlUEContext_p
 *                  4. recvTTI  - Its represent the tick of the message
 *                              recieved.
 *                  5. bitMaskForAperiodicReport           
 *                  6. subFrameNum
 *                  7. internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the cqi pdu in case ulcqi
 *                  is less than CQISINRThreshold
 *****************************************************************************/
STATIC inline void handleCQIReportWithUlCqiLessThanCQISINRThreshold(UInt8 numberOfCCReported,
        ULUEContext *ulUEContext_p,
        DLUEContext *dlUEContext_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt8 *bitMaskForAperiodicReport_p,
        /* +- SPR 17777 */
        UInt16 subFrameNum
/** SPR 14032 Fix : Start **/
#ifdef TDD_CONFIG
        ,UInt16 sysFrameNum
#endif
/** SPR 14032 Fix : End **/
        )
        /* +- SPR 17777 */
{
    UInt8 servCellIndex;
    UInt8 cellIdxOfExpectedReportType = 0;
    UInt8 numOfCCProcessed = 0;
    DLCQIInfo *dlCQIInfo_p = PNULL;
/** SPR 14032 Fix : Start **/
    /* SPR 15909 fix start */
    tickType_t ulAperiodicCqiRequestTTI;
    /* SPR 15909 fix end */
#ifdef FDD_CONFIG
/** SPR 14032 Fix : End **/
    cellIdxOfExpectedReportType = 
          ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType;
#elif TDD_CONFIG

          /* SPR 18122 Changes Start*/
    cellIdxOfExpectedReportType = 
          ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].cellIdxOfExpectedReportType;
        
        /* SPR 18122 Changes End*/
#endif
    
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */
/** SPR 14032 Fix : Start **/
/* CA TDD pucch code change**/
    if(dlUEContext_p->scellCount)
    {
        /* Rel 10 CA Ue */
        /* check serving cell index of cell for which report
         * is expected */
        for(servCellIndex = 0; servCellIndex <= MAX_NUM_SCELL; servCellIndex++)
        {
            if(!servCellIndex)
            {
                /* Pcell */
                dlCQIInfo_p = &(dlUEContext_p->dlCQIInfo);
                ulAperiodicCqiRequestTTI = ulUEContext_p->aperiodicCqiRequestTTI;
            }
            else
            {
                if(PNULL == dlUEContext_p->dlUeScellContext_p[servCellIndex])
                {
                    continue;
                }
                dlCQIInfo_p = &(dlUEContext_p->dlUeScellContext_p[servCellIndex]->
                        ScelldlCQIInfo);
                ulAperiodicCqiRequestTTI = ulUEContext_p->ulUeScellContext_p[servCellIndex]->
                    aperiodicCqiRequestTTI;
            }
            if(numOfCCProcessed != numberOfCCReported)
            {
                if(cellIdxOfExpectedReportType & servingCellBitMask_g[servCellIndex])
                {
                    if (APERIODIC_CQI_REPORTING_MODE ==
                            dlCQIInfo_p->cqiTypeIndicator) 
                    { 
                        /* If only aperiodic is  ON and data offset is 0 then 
                         *  even if below condition is not met then also we will 
                         *  skip from here.
                         */  
                        if   (recvTTI == ulUEContext_p->lastTriggeredAperiodicTTI)
                        {
                            /* SPR 4484 changes start */
                            if((currentTTI > (dlCQIInfo_p->aperiodicCqiRequestTTI + 
                                            APERIODIC_CQI_REQ_OFFSET)) && (currentTTI >
                                            (ulAperiodicCqiRequestTTI + APERIODIC_CQI_REQ_OFFSET)))
                            {
                                *bitMaskForAperiodicReport_p |= servingCellBitMask_g[servCellIndex];
                            }
                            /* SPR 4484 changes end */
                        }
                    }
                    numOfCCProcessed += 1;
                }
            }
            else
            {
                break;
            }
        }
        /* reset bitmask cellIdxOfExpectedReportType */
#ifdef FDD_CONFIG      
        ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType = 0;
#elif TDD_CONFIG

        /* SPR 18122 Changes Start*/
        ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].cellIdxOfExpectedReportType = 0;
        
        /* SPR 18122 Changes End*/
#endif        
/** SPR 14032 Fix : End **/
    }
    else
    {
/* CA TDD pucch code change**/
        /* Rel 9 or Rel 10 UE(without CA)*/
        /* SPR 4484 changes start */
        if (APERIODIC_CQI_REPORTING_MODE ==
                dlUEContext_p->dlCQIInfo.cqiTypeIndicator) 
        { 
            /* If only aperiodic is  ON and data offset is 0 then 
             *  even if below condition is not met then also we will 
             *  skip from here.
             */  
            if   (recvTTI == ulUEContext_p->lastTriggeredAperiodicTTI)
            {
                /* SPR 4484 changes start */
                if((currentTTI > (dlUEContext_p->dlCQIInfo.aperiodicCqiRequestTTI + 
                                APERIODIC_CQI_REQ_OFFSET)) && (currentTTI >
                                (ulUEContext_p->aperiodicCqiRequestTTI + APERIODIC_CQI_REQ_OFFSET)))
                {
                    *bitMaskForAperiodicReport_p |= servingCellBitMask_g[PCELL_SERV_CELL_IX];
                }
                /* SPR 4484 changes end */
            }
        }
    }

}
/* CA_phase2_csi_code end */

/* CA_phase2_csi_code Start */
/*****************************************************************************
 * Function Name  : processCQIReportForOneCellForCAUE
 * Inputs         :  ulUEContext_p
 *                   dlUEContext_p
 *                   datalen
 *                   recvTTI  - Its represent the tick of the message
 *                              recieved.
 *                   data_p
 *                   ueIndex
 *                   cqiPdu_p- pointer to the cqi pdu info
 *                   subFrameNum
 *                   internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the cqi pdu in case data offset
 *                  is NULL.
 *****************************************************************************/
STATIC inline void processCQIReportForOneCellForCAUE(ULUEContext *ulUEContext_p,
        DLUEContext *dlUEContext_p,
        FAPI_cqiPduIndication_st *cqiPdu_p,
        UInt32  datalen,
        UInt16 subFrameNum,
#ifdef TDD_CONFIG
        UInt16 sysFrameNum,
#endif
        UInt8 *data_p,
        UInt16  ueIndex,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
{
/*ca-tdd pucch code changes*/
    UInt8 transmissionMode = 0;
    ueExpectedReportType  cqiReportType = EXPECTED_REPORT_INVALID;
    UInt16   count = 0;/*Coverity 69584 +-*/
    UInt8 cellIdxOfExpectedReportType;
    DLUESCellContext *dlUeScellContext_p = PNULL;
    UInt8 servCellIndex;
#ifdef FDD_CONFIG
    cqiReportType = ulUEContext_p->reportFormatMap[subFrameNum].ueExpectedReport;
    cellIdxOfExpectedReportType = 
           ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType;
#elif TDD_CONFIG

           /* SPR 18122 Changes Start*/
    cqiReportType = ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].ueExpectedReport;
    cellIdxOfExpectedReportType = 
           ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].cellIdxOfExpectedReportType;

        /* SPR 18122 Changes End*/
#endif
    /* report is either for PCell or Scell */
    if(cellIdxOfExpectedReportType & servingCellBitMask_g[PCELL_SERV_CELL_IX])
    {
        count = COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(internalCellIndex);
        if(count < NUM_PERIODIC_REPORT_NODES)
        {
            /* Pcell */
            /* 4x2 MIMO S */
            /* 4x4 DL MIMO CHG START */
            if ((1 == cqiPdu_p->numberOfCCReported)
                    && (cqiPdu_p->ri[0]))
            {
                if (isRIInCodebookSubsetRestriction( cqiPdu_p->ri[0],
                            dlUEContext_p->ueCategory,
                            dlUEContext_p->dlMIMOInfo.transmissionMode,
                            dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue,internalCellIndex))
                {
                    ulUEContext_p->pucchConfigInfo.ueReportedRI= cqiPdu_p->ri[0];
                } 
                /* 4x4 DL MIMO CHG END */
                /* 4x2 MIMO E */
            }
            /* SPR_4261_FIX Start */ 
            if ( datalen == RI_MULTIPLEX_WITH_PUSCH)
            {
                return;
            }
            /* SPR_4261_FIX End */ 
            transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
            
            LOG_MAC_MSG(MAC_CQI_DECODE_ID,LOGINFO,MAC_L1_INF,\
                    getCurrentTick(),\
                    cqiReportType,\
                    ueIndex,\
                    recvTTI, datalen, 
                    ulUEContext_p->pucchConfigInfo.ueReportedRI,
                    0.0,0.0,__func__,"CQI_DECODE");
            decodeCqiReports(data_p,
                    cqiReportType,
                    ueIndex,
                    ulUEContext_p,
                    recvTTI,
                    transmissionMode,
                    dlUEContext_p,
                    subFrameNum,
                    internalCellIndex);
            /*Cyclomatic Complexity changes - ends here */
        }
        /* For TDD_CA . pls take care */
    }
    for(servCellIndex = START_SCELL_INDEX; servCellIndex <= MAX_NUM_SCELL;
            servCellIndex++)
    {
        dlUeScellContext_p = dlUEContext_p->dlUeScellContext_p[servCellIndex];
        if(PNULL == dlUeScellContext_p)
        {
            continue;
        }
        count = COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(dlUeScellContext_p->internalCellIndex);
        if(cellIdxOfExpectedReportType & servingCellBitMask_g[servCellIndex])
        {
            if(count < NUM_PERIODIC_REPORT_NODES)
            {
                /* 4x2 MIMO S */
                /* 4x4 DL MIMO CHG START */
                if ((1 == cqiPdu_p->numberOfCCReported)
                    && (cqiPdu_p->ri[0]))
                {
                    if (isRIInCodebookSubsetRestriction( cqiPdu_p->ri[0],
                                dlUEContext_p->ueCategory,
                                dlUeScellContext_p->ScelldlMIMOInfo.transmissionMode,
                                dlUeScellContext_p->codebookSubsetRestrictionv1020.cbsrValue,
                                dlUeScellContext_p->internalCellIndex))
                    {
                        ulUEContext_p->ulUeScellContext_p[servCellIndex]->
                            scellpucchConfigInfo.ueReportedRI =  
                            cqiPdu_p->ri[0];
                    } 
                    /* 4x4 DL MIMO CHG END */
                    /* 4x2 MIMO E */
                }
                /* SPR_4261_FIX Start */ 
                if ( datalen == RI_MULTIPLEX_WITH_PUSCH)
                {
                    return;
                }
                /* SPR_4261_FIX End */ 
/** SPR 14032 Fix : Start **/
#ifdef FDD_CONFIG                
                cqiReportType = ulUEContext_p->
                    ulUeScellContext_p[servCellIndex]->
                    cqiPmiRiReportFormatMapSCell[subFrameNum].ueExpectedReport;
#elif TDD_CONFIG

                    /* SPR 18122 Changes Start*/
                cqiReportType = ulUEContext_p->
                    ulUeScellContext_p[servCellIndex]->
                    cqiPmiRiReportFormatMapSCell[MODULO(sysFrameNum,2)][subFrameNum].ueExpectedReport;

                    /* SPR 18122 Changes End*/
#endif                    
/** SPR 14032 Fix : End **/
                transmissionMode = dlUeScellContext_p->ScelldlMIMOInfo.transmissionMode;
                
                LOG_MAC_MSG(MAC_CQI_DECODE_ID, LOGINFO, MAC_L1_INF, 
                        getCurrentTick(), cqiReportType, ueIndex,
                        recvTTI, datalen, 
                        ulUEContext_p->ulUeScellContext_p[servCellIndex]->
                        scellpucchConfigInfo.ueReportedRI,
                        0.0,0.0,__func__,"CQI_DECODE");
                decodeCqiReports(data_p,
                        cqiReportType,
                        ueIndex,
                        ulUEContext_p,
                        recvTTI,
                        transmissionMode,
                        dlUEContext_p,
                        subFrameNum,
                        dlUeScellContext_p->internalCellIndex
                        );
                /*Cyclomatic Complexity changes - ends here */
            }
        }
        break;
    }
}

/* coverity 64951 28June2014 */
/* coverity 64951 28June2014 */
/*****************************************************************************
 * Function Name  : getNumCellsForCSIReport
 * Inputs         : 1.cellIdxOfExpectedReportType 
 *                  2.scellCount 
 * Outputs        : None
 * Returns        : Number of cells for which report is expected
 * Description    : This function returns Number of cells for which report is
 *                  expected
 *****************************************************************************/
STATIC inline UInt8 getNumCellsForCSIReport(UInt8 cellIdxOfExpectedReportType,
        UInt8 scellCount)
{
    UInt8 servCellIndex;
    UInt8 numOfCells = 0;
    for(servCellIndex = PCELL_SERV_CELL_IX; servCellIndex <= scellCount; 
            servCellIndex++)
    {
        if(cellIdxOfExpectedReportType & servingCellBitMask_g[servCellIndex])
        {
            numOfCells++;
        }
    }
    return numOfCells;
}
/* coverity 64951 28June2014 */
/* coverity 64951 28June2014 */

/*****************************************************************************
 * Function Name  : handleVendorSpecificInfoR9 
 * Inputs         : 1. vendorSpecific_p  - pointer to vendorSpecific Information
 *                   
 *                  2. validReports_p- pointer to valid report 
 *                  3. recvTTI
 *                  4. internalCellIndex - Cell_index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the AOA received in Vendorspecific 
 *                   information
 * *****************************************************************************/
void handleVendorSpecificInfoR9( FAPI_VendorSpecific_st *vendorSpecific_p,
        UInt8 *validReports_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt16 *angleOfArrival = PNULL;
    ULUEContext *ulUEContext_p = PNULL;
    EcidReportParams  *eCidReportParams_p = PNULL;
    FAPI_UEMeasurementIndication_st *ueMeasInfoForUEs_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
    FAPI_AOA_REL9TARecievedForUE_st *aoaRel9Info = PNULL;
    /* +- SPR 18268 */
    UInt16 count = 0;
    UInt16 numOfUEs = 0;
    /* +- SPR 18268 */
    UInt16 rnti=0;
    UInt16  ueIndex = 0;
    UInt8 counter = 0;
    RNTIInfo *rntiInfo_p = PNULL;

    if(PNULL != vendorSpecific_p)
    {
        ueMeasInfoForUEs_p = (FAPI_UEMeasurementIndication_st *)&
            vendorSpecific_p->ueMeasInfoForUEs;
        if(PNULL != ueMeasInfoForUEs_p)
        {
            numOfUEs = ueMeasInfoForUEs_p->numOfUEs;
            for(count = 0; count < numOfUEs;count++)
            {
                rnti = MAC_PHY_CONVERT_16(ueMeasInfoForUEs_p->aoaRel9TAInfo[count].rnti);
                rntiInfo_p = getUeIdxFromRNTIMap(rnti,internalCellIndex);
                if(PNULL != rntiInfo_p)
                {
                    if(FREERNTI != rntiInfo_p->rntiFlag)
                    {
                        ueIndex = rntiInfo_p->index;
                        ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
                        if((TRUE != ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag) && \
                                PNULL != ulUEContext_p)
                        {

                            eCidReportParams_p = &ulUEContext_p->eCidReportParams;

                            VALIDATE_AND_UPDATE_AOA_REPORTS(
                                    ueMeasInfoForUEs_p->aoaRel9TAInfo,
                                    angleOfArrival,
                                    *validReports_p,
                                    count);

                            SAVE_ECID_AOA_REPORT_IN_UE_CONTXT(eCidReportParams_p,
                                    angleOfArrival,
                                    recvTTI);

                            if(NO_REPORT_TYPE_REQUIRED != eCidReportParams_p->typeofReportsRequired && \
                                    TYPE_2_TA_REPORT_REQUIRED != eCidReportParams_p->typeofReportsRequired)
                            {
                                checkValidReportsAndPutEntryInEcidMgr(ueIndex,
                                        *validReports_p,internalCellIndex);
                            }
                        }

                        if (dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag)
                        {
                            lteWarning(" Pending Delete Flag True for UE Index %d %s",
                                    ueIndex,__func__);
                            continue ;
                        }
                        dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
                        /* SPR 7973 chages start */
                        if( PNULL ==  dlUEContext_p )
                        {
                            continue;
                        }
                        /* SPR 7973 chages end */

                        aoaRel9Info =  &ueMeasInfoForUEs_p->aoaRel9TAInfo[count];
                        for (counter = 0; counter < cellParams_p->numOfTxAnteenas; counter++)
                        {
                            if(MAX_VALUE_OF_AOA >= aoaRel9Info->AoA[counter])
                            {
                                dlUEContext_p->AoA[counter] = 
                                    calculateTimeAvgAoA(dlUEContext_p->AoA[counter] ,aoaRel9Info->AoA[counter]);  
                            }
                        }

                    }
                }
            }
        }
    }
}

/*****************************************************************************
 * Function Name  : decodeAperiodicCqiReportsR10
 * Inputs         : data_p - pointer to PDU recieved,
 *                  cqiReportType- Cqi Mode,
 *                  ueIndex   - ueIndex of UE,
 *                  ulUEContext_p - pointer to the UL UE context,
 *                  cqiInfoSch_p - pointer to CQIListElement,
 *                  recvTTI  - Its represent the tick of the message
 *                              recieved,
 *                  transmissionMode: Transmission mode of the UE,
 *                  dlUEContext_p - pointer to DLUEContext and
 *                  subFrameNum - current Subframe
 *                  internalCellIndex - Cell_index at MAC
 *                  numOfBitsDecoded_p
 * Outputs        : decodes the cqi report received for reporttype.
 * Returns        : void
 * Description    : This function decodes the cqi report received for report type.
 ****************************************************************************/
                    STATIC inline void decodeAperiodicCqiReportsR10(UInt8 *data_p,
                            ueExpectedReportType cqiReportType,
                            UInt16 ueIndex,
                            ULUEContext *ulUEContext_p,
                            /* SPR 15909 fix start */
                            tickType_t recvTTI,
                            /* SPR 15909 fix end */
                            UInt8 transmissionMode,
                            DLUEContext *dlUEContext_p,
                            /* +- SPR 17777 */
                            /*CA Changes start */
                            InternalCellIndex internalCellIndex,
                            UInt8 *numOfBitsDecoded_p)
                    /*CA Changes end */
{
    UInt8 ri = 0;
    InternalCellIndex  servCellId = INVALID_CELL_INDEX;
    if(ulUEContext_p->internalCellIndex == internalCellIndex)
    {
        /* Pcell */
        ri = ulUEContext_p->pucchConfigInfo.ueReportedRI;
    }
    else
    {
        /* Scell */
        servCellId = getServeCellIndexFromInternalCellId(dlUEContext_p,
                internalCellIndex);
        /*kolckwork warning fix*/
        if(servCellId <= MAX_NUM_SCELL)
        {
            ri = ulUEContext_p->ulUeScellContext_p[servCellId]->scellpucchConfigInfo.ueReportedRI;
        }
    }

    /* + coverity 72986 */
    if ( ri > 0 )
    {
        switch(cqiReportType)
        {
            case APERIODIC_MODE_1_2:

            decodeAperiodicCqiMode1_2(data_p,
                    ueIndex, 
                    ri,
                    ulUEContext_p,
                    /* + CQI_5.0 */
                    recvTTI,
                    internalCellIndex,
                    /* CA_phase2_csi_code */
                    numOfBitsDecoded_p);
            /* - CQI_5.0 */
            if(ulUEContext_p->internalCellIndex == internalCellIndex)
            {
                /* Pcell */
                dlUEContext_p->dlCQIInfo.latestAperiodicReportRcvd = TRUE;
            }
            else
            {
                /* Scell */
                /*kolckwork warning fix*/
                if(servCellId <= MAX_NUM_SCELL)
                { 
                    dlUEContext_p->dlUeScellContext_p[servCellId]->ScelldlCQIInfo.
                        latestAperiodicReportRcvd = TRUE;
                }
            }
            break;

            case APERIODIC_MODE_2_0:

            decodeAperiodicCqiMode2_0(data_p,
                    ueIndex, 
                    transmissionMode,
                    /* + CQI_5.0 */
                    ri,
                    /* - CQI_5.0 */
                    ulUEContext_p,
                    internalCellIndex,
                    /* CA_phase2_csi_code */
                    numOfBitsDecoded_p);
            break;    

            case APERIODIC_MODE_2_2:

            /* + CQI_4.1 */
            decodeAperiodicCqiMode2_2(data_p, 
                    ueIndex,
                    ri,
                    ulUEContext_p,
                    internalCellIndex,
                    /* CA_phase2_csi_code */
                    numOfBitsDecoded_p);
            break; 
            /* - CQI_4.1 */

            case APERIODIC_MODE_3_0:

            decodeAperiodicCqiMode3_0(data_p, 
                    ueIndex,
                    transmissionMode,
                    /* + CQI_5.0 */
                    ri,
                    /* - CQI_5.0 */
                    ulUEContext_p,
                    internalCellIndex,
                    /* CA_phase2_csi_code */
                    numOfBitsDecoded_p);
            break;    

            case APERIODIC_MODE_3_1:

            /* + CQI_4.1 */
            decodeAperiodicCqiMode3_1(data_p, 
                    ueIndex,
                    ri,
                    /* + TM6_5.2 */
                    transmissionMode,
                    /* - TM6_5.2 */
                    ulUEContext_p,
                    internalCellIndex,
                    /* CA_phase2_csi_code */
                    numOfBitsDecoded_p);
            break;
            /* - CQI_4.1 */

            default:
            lteWarning("Report Type Decoding not supported %d\n", 
                    /* PURIFY FIXES 2.4.1 start */
                    (UInt32)cqiReportType);
            /* PURIFY FIXES 2.4.1  end*/
            break;
        }   
    }
    else
    {
        lteWarning("Invalid Rank Indicator Recieved");
    }
    /* - coverity 72986 */
}
/* coverity 64952 28June2014 */

/* coverity 64950 28June2014 */
/* coverity 64950 28June2014 */
/*****************************************************************************
 * Function Name  : calculateSizeOfReport
 * Inputs         : 1. ri
 *                  2. cqiReportType
 *                  3. internalCellIndex  
 * Outputs        : None
 * Returns        : None
 * Description    : This function return the bitWidth for Aperiodic report
 *****************************************************************************/
STATIC inline UInt8 calculateSizeOfReport(UInt8 ri,
        ueExpectedReportType  cqiReportType,
        InternalCellIndex internalCellIndex)
{
    UInt8 numberOfBits = 0;
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    switch(cqiReportType)
    {
        case APERIODIC_MODE_1_2:
            if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
            {
                if(ri >1)
                {
                    numberOfBits = APERIODIC_MODE_12_RI_GT_1_FOR_4_ANT_BITS(cellParams_p);
                }
                else
                {
                    numberOfBits = APERIODIC_MODE_12_RI_1_FOR_4_ANT_BITS(cellParams_p);
                }
            }
            else
            {
                if(ri > 1)
                {
                    numberOfBits = APERIODIC_MODE_12_RI_2_BITS(cellParams_p);
                }
                else
                {
                    numberOfBits = APERIODIC_MODE_12_RI_1_BITS(cellParams_p);
                }
            }
            break;

        case APERIODIC_MODE_2_0:
            numberOfBits = APERIODIC_MODE_20_BITS(cellParams_p);
            break;

        case APERIODIC_MODE_2_2:
            if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
            {
                if(ri >1)
                {
                    numberOfBits = APERIODIC_MODE_22_RI_GT_1_FOR_4_ANT_BITS(cellParams_p);
                }
                else
                {
                    numberOfBits = APERIODIC_MODE_22_RI_1_FOR_4_ANT_BITS(cellParams_p);
                }
            }
            else
            {
                if(ri > 1)
                {
                    numberOfBits = APERIODIC_MODE_22_RI_2_BITS(cellParams_p);
                }
                else
                {
                    numberOfBits = APERIODIC_MODE_22_RI_1_BITS(cellParams_p);
                }
            }
            break;

        case APERIODIC_MODE_3_0:
            numberOfBits = APERIODIC_MODE_30_BITS(cellParams_p);
            break;

        case APERIODIC_MODE_3_1:
            if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
            {
                if(ri >1)
                {
                    numberOfBits =  APERIODIC_MODE_31_RI_GT_1_FOR_4_ANT_BITS(cellParams_p);
                }
                else
                {
                    numberOfBits = APERIODIC_MODE_31_RI_1_FOR_4_ANT_BITS(cellParams_p);
                }
            }
            else
            {
                if(ri > 1)
                {
                    numberOfBits = APERIODIC_MODE_31_RI_2_BITS(cellParams_p);
                }
                else
                {
                    numberOfBits = APERIODIC_MODE_31_RI_1_BITS(cellParams_p);
                }
            }
            break;
        default:
            lteWarning("Report Type Decoding not supported %d\n", 
                    /* PURIFY FIXES 2.4.1 start */
                    (UInt32)cqiReportType);
            /* PURIFY FIXES 2.4.1  end*/
            break;
    }
    return numberOfBits;
}


/* + TM7_8 Changes Start */
void processCQIIndication(void *VendorSpecificParam_p,
        void *msg_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
#ifdef TDD_CONFIG
        UInt16 sysFrameNum,
#endif
        UInt16 subFrameNum,
        InternalCellIndex internalCellIndex)
{
    UInt8   *data_p       = PNULL;
    UInt8   ri            = 0;
    UInt16  ueIndex       = 0;
    UInt16  iterator      = 0;
    UInt16  recvRnti      = 0;
    UInt32  datalen       = 0;
    ueExpectedReportType  cqiReportType = EXPECTED_REPORT_INVALID;
    FAPI_rxCqiIndication_st *cqiMsg_p = (FAPI_rxCqiIndication_st *)msg_p;
    FAPI_VendorSpecific_st *vendorSpecific_p=(FAPI_VendorSpecific_st*)VendorSpecificParam_p;
    UInt16  numOfPdu      = MAC_PHY_CONVERT_16(cqiMsg_p->numOfCqi);
    RNTIInfo *rntiInfo_p  = PNULL;
    FAPI_cqiPduIndication_st *cqiPdu_p = PNULL; 
    ULUEContext *ulUEContext_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
    SInt16  timingAdvance = 0;
    UInt8 numberOfCCReported=0;
    UInt8 numCellsReportExpected = MAX_NUM_PCELL;

    
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */
    UInt8 bitMaskForAperiodicReport = 0;
    UInt8 numOfBitsDecoded = 0;
    UInt8 numOfCCProcessed = 0;
    UInt8 servCellIndex;
    DLUESCellContext *dlUeScellContext_p = PNULL;
    UInt8 cellIdxOfExpectedReportType = 0;
    UInt8 numOfBitsOfScellReport = 0; 
/*ca-tdd pucch code changes*/
    UInt8 totalBits = 0; 
    UInt8 offset = 0; 
    UInt8 remainingBitsInWord = 0; 
    UInt8 endIndex; 
    UInt32 temp32Var; 
    UInt32 tempArr[3] = {0};
    InternalCellIndex ueInternalCellIndex = INVALID_CELL_INDEX;
    DLCQIInfo *dlCQIInfo_p = PNULL;
    UInt8 riIndex = 0;
    /* SPR 15909 fix start */
    tickType_t ulAperiodicCqiRequestTTI;
    /* SPR 15909 fix end */
    UInt8 *refPdu_p = PNULL;
    UInt16  offsetNextPdu = 0;

    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt16 count = 0;/*Coverity 69581 +-*/
    UInt16 timingAdvanceR9 = INVALID_TIMING_ADVANCE_R9_VALUE;
    UInt8 validReports = 0;
    EcidReportParams   *eCidReportParams_p = PNULL;

    UInt8 transmissionMode = 0;

    if(cqiMsg_p->numOfCqi)
    {
        refPdu_p = ((UInt8 *)&cqiMsg_p->cqiPduInfo[0]) +
            (numOfPdu * sizeof(FAPI_cqiPduIndication_st));

        for (iterator = 0; iterator < numOfPdu; iterator++)
        {
            bitMaskForAperiodicReport = 0;
/*ca-tdd pucch code changes*/
            riIndex = 0;
            numOfBitsDecoded = 0;
            cqiPdu_p = (FAPI_cqiPduIndication_st *)&cqiMsg_p->cqiPduInfo[iterator]; 
            recvRnti = MAC_PHY_CONVERT_16(cqiPdu_p->rnti);

            /* getUeIndexFromRNTIMap returns NULL if not configured RARNTI or 
             * CRNTI Range and rntiFlag == FREERNTI is  deactive.
             */
            rntiInfo_p = getUeIdxFromRNTIMap(recvRnti,internalCellIndex);
            if (PNULL == rntiInfo_p || FREERNTI == rntiInfo_p->rntiFlag)
            {
                lteWarning("processCQIIndication: Recv Rnti %d is not Active at "
                        "Mac Layer\n",recvRnti);   

/* SPR 20636 Changes Start */
                LOG_MAC_MSG(MAC_CQI_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
                        currentTTI,\
                        recvRnti,\
                        (ADDR)rntiInfo_p,\
                        (rntiInfo_p ? rntiInfo_p->rntiFlag : 0), \
                        recvTTI,0,\
                        0.0,0.0,__func__,"CQI_RNTIERR");

/* SPR 20636 Changes End */
                continue ;
            }

            if (TCRNTI == rntiInfo_p->rntiFlag)
            {
                /* If Dataoffset is 0, it means that decoding of CQI has failed for 
                 * that UE so continue */
                if(0 == cqiPdu_p->dataOffset)
                {
                    continue;
                }    
            }  
            else 
            {
                ueIndex = rntiInfo_p->index;
                if (dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag)
                {
                    lteWarning(" Pending Delete Flag True for UE Index %d %s",
                            ueIndex,__func__);

                    LOG_MAC_MSG(MAC_CQI_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
                            currentTTI,\
                            recvRnti,\
                            ueIndex,\
                            recvTTI,0,0,\
                            0.0,0.0,__func__,"CQI_UEDELETE");
                    continue;
                }

                dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
                /* Coverity 63549 Fix Start */
                if( PNULL ==  dlUEContext_p )
                {
                    continue;
                }
                /* Coverity 63549 Fix End */
                ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
                if(PNULL == ulUEContext_p)
                {
                    continue;
                }
                /* CA_phase2_csi_code start */
/*ca-tdd pucch code changes*/
#ifdef FDD_CONFIG
                cellIdxOfExpectedReportType = 
                         ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType;
#elif TDD_CONFIG
                         
                         /* SPR 18122 Changes Start*/
                cellIdxOfExpectedReportType = 
                        ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].cellIdxOfExpectedReportType;
                       
                        /* SPR 18122 Changes End*/
#endif
                if( 0 == dlUEContext_p->scellCount)
                {
                    /* Non CA UE */
                    count = COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(internalCellIndex);
                    if (count == NUM_PERIODIC_REPORT_NODES)
                    {
                        /* Due to configured CQI Configuration of all the UEs, the influx of reports
                         * is more that what L2 has been able to process per TTI as per NUM_CQI_NODES
                         * and the reports are just getting accumulated, exhausting system memory.
                         * Hence drop the report nodes till the situation improves*/
                        continue;
                    }
                }
                else
                {
                    /* CA is configured so get number of cells for which report
                     * is expected */
                    numCellsReportExpected = getNumCellsForCSIReport(cellIdxOfExpectedReportType,
                            dlUEContext_p->scellCount);
                    if(0 == numCellsReportExpected)
                    {
/* SPR 20636 Changes Start */
                        LOG_MAC_MSG(MAC_CQI_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
                                currentTTI,\
                                recvRnti,\
                                (ADDR)rntiInfo_p,\
                                (rntiInfo_p ? rntiInfo_p->rntiFlag : 0), \
                                recvTTI,0,\
                                0.0,0.0,__func__,"Cell Index of Expected Report Not Set"\
                                "for CA UE");
/* SPR 20636 Changes End */
                        continue;
                    }
                }
                timingAdvance =MAC_PHY_CONVERT_16(cqiPdu_p->timingAdvance);
    
#ifdef FLEXRAN
                timingAdvance = timingAdvance - MAC_TIMING_ADVANCE_DEFAULT_VAL + MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31;
#endif

                /*FAPI 2.4 changes start*/
                /* RJ_Comment incorporation- moving this code at position where
                 * normal TA value is handled after checking threshold */
               /* if(TRUE != ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag)
                        
                {
                    eCidReportParams_p = &ulUEContext_p->eCidReportParams;
                    timingAdvanceR9 = (MAX_TIMING_ADVANCE_R9_VALUE >=MAC_PHY_CONVERT_16(cqiPdu_p->cqiRiInformation.timingAdvanceR9) )?\
                                      MAC_PHY_CONVERT_16(cqiPdu_p->cqiRiInformation.timingAdvanceR9):INVALID_TIMING_ADVANCE_R9_VALUE;
                    eCidReportParams_p->type1TAValue = timingAdvanceR9;
                    validReports |= (MAX_TIMING_ADVANCE_R9_VALUE >= timingAdvanceR9)? TRUE:FALSE;
                }
                else
                {
                    continue;
                }*/
                /*FAPI 2.4 changes end*/

#ifdef FDD_CONFIG
                cqiReportType = 
                        ulUEContext_p->reportFormatMap[subFrameNum].ueExpectedReport;
#elif TDD_CONFIG

                        /* SPR 18122 Changes Start*/
                cqiReportType = 
                        ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].ueExpectedReport;

                        /* SPR 18122 Changes End*/
#endif

                /* If Dataoffset is 0, it means either only RI received at PUSCH or
                 * decoding of CQI has failed for that UE.
                 * If decoding of CQI has failed or  ulCqi is less than threshold then 
                 * don't update DL MCS so continue */
#ifdef PERF_STATS
                UPDATE_UE_PUCCH_SINR_STATS(ueIndex, cqiPdu_p->ulCqi, subFrameNum);
#endif
                /* CA_phase2_csi_code start */
                numberOfCCReported = cqiPdu_p->numberOfCCReported;
                if  (0 == cqiPdu_p->dataOffset) 
                {
                    if(1 == numCellsReportExpected)
                    {
                        handleCQIReportWithDataOffsetNull(ulUEContext_p,
                                dlUEContext_p,recvTTI,ueIndex,cqiPdu_p,
                                subFrameNum,
#ifdef TDD_CONFIG
                                sysFrameNum,
#endif
                                &bitMaskForAperiodicReport,
                                internalCellIndex,
                                numberOfCCReported);
                    }
                    else
                    {
                        /* report is for multiple cell, so only Aperiodic-Rel10 CA UE */
                        numOfCCProcessed = 0;
                        for(servCellIndex = 0; servCellIndex <= MAX_NUM_SCELL;
                                servCellIndex++)
                        {
                            if(!servCellIndex)
                            {
                                ueInternalCellIndex = internalCellIndex;
                                dlCQIInfo_p = &(dlUEContext_p->dlCQIInfo);
                                ulAperiodicCqiRequestTTI = ulUEContext_p->aperiodicCqiRequestTTI;

                            }
                            else
                            {
                                if(PNULL == dlUEContext_p->dlUeScellContext_p[servCellIndex])
                                {
                                    continue;
                                }
                                ueInternalCellIndex = dlUEContext_p->
                                    dlUeScellContext_p[servCellIndex]->internalCellIndex;
                                dlCQIInfo_p = &(dlUEContext_p->dlUeScellContext_p[servCellIndex]->
                                        ScelldlCQIInfo);
                                ulAperiodicCqiRequestTTI = ulUEContext_p->ulUeScellContext_p[servCellIndex]->
                                    aperiodicCqiRequestTTI;
                            }
                            if(numOfCCProcessed != numCellsReportExpected)
                            {
                                if(cellIdxOfExpectedReportType & servingCellBitMask_g[servCellIndex])
                                {
                                    count = COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(
                                            ueInternalCellIndex);
                                    if(count < NUM_PERIODIC_REPORT_NODES)
                                    {
                                        if (APERIODIC_CQI_REPORTING_MODE ==
                                                dlCQIInfo_p->cqiTypeIndicator) 
                                        { 
                                            /* If only aperiodic is  ON and data offset is 0 then 
                                             *  even if below condition is not met then also we will 
                                             *  skip from here.
                                             */  
                                            if   (recvTTI == ulUEContext_p->lastTriggeredAperiodicTTI)
                                            {
                                                /* SPR 4484 changes start */
                                                /* CA_phase2_csi_code */
                                                if((currentTTI > (dlCQIInfo_p->aperiodicCqiRequestTTI + 
                                                                APERIODIC_CQI_REQ_OFFSET)) && (currentTTI >
                                                                (ulAperiodicCqiRequestTTI + APERIODIC_CQI_REQ_OFFSET)))
                                                {
                                                    bitMaskForAperiodicReport |= servingCellBitMask_g[servCellIndex];
                                                }
                                                /* SPR 4484 changes end */
                                            }
                                        }
                                    }
                                    numOfCCProcessed += 1;
                                }
                            }
                            else
                            {
                                break;
                            }

                        }
                        /* reset bitmask cellIdxOfExpectedReportType */
#ifdef FDD_CONFIG
                ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType = 0; 

#elif TDD_CONFIG
                
                /* SPR 18122 Changes Start*/
                ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].cellIdxOfExpectedReportType = 0;
                
                /* SPR 18122 Changes End*/
#endif

                    }/*else end*/
                    /* push node for aperidic trigger  */
                    if(bitMaskForAperiodicReport)
                    {
                        putEntryInULAperiodicCQIQueue(ueIndex, internalCellIndex
                                ,bitMaskForAperiodicReport
                                );
                    }
                    /* CA_phase2_csi_code end */
                    continue;
                }
                else if (cqiPdu_p->ulCqi < cellParams_p->pucchCQISINRThreshold)
                {
                    /* CA_phase2_csi_code Start */
                    /** SPR 14032 Fix : Start **/
                    handleCQIReportWithUlCqiLessThanCQISINRThreshold(numCellsReportExpected,
                            ulUEContext_p,dlUEContext_p,recvTTI,&bitMaskForAperiodicReport,
                            /* +- SPR 17777 */
                            subFrameNum
#ifdef TDD_CONFIG
                            ,sysFrameNum
#endif
                            );
                            /* +- SPR 17777 */
                    /** SPR 14032 Fix : End **/
                    /* push node for aperidic trigger  */
                    if(bitMaskForAperiodicReport)
                    {
                        putEntryInULAperiodicCQIQueue(ueIndex, internalCellIndex
                                ,bitMaskForAperiodicReport
                                );
                    }
                    /* CA_phase2_csi_code End */
                    continue;
                }   

                if ((timingAdvance > MAC_TIMING_ADVANCE_VAL_MAX) || (timingAdvance < 0))
                {
                    lteWarning("Invalid TA value = %d in CQI\n", timingAdvance);
                }
                else
                {
                    handleTimingAdvance(dlUEContext_p, ueIndex, (UInt16)timingAdvance,internalCellIndex, getCurrentTickUl());
                    /*FAPI 2.4 changes start*/
                   if(TRUE != ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag)
                            
                    {
                        eCidReportParams_p = &ulUEContext_p->eCidReportParams;
                        timingAdvanceR9 = (MAX_TIMING_ADVANCE_R9_VALUE >=MAC_PHY_CONVERT_16
                                (cqiPdu_p->timingAdvance) )?\
                                          MAC_PHY_CONVERT_16(cqiPdu_p->
                                                  timingAdvance):INVALID_TIMING_ADVANCE_R9_VALUE;
                        eCidReportParams_p->type1TAValue = timingAdvanceR9;
                        validReports |= (MAX_TIMING_ADVANCE_R9_VALUE >= timingAdvanceR9)? TRUE:FALSE;
                    }
                    else
                    {
                        continue;
                    }
                    /*FAPI 2.4 changes end*/

                    LOG_MAC_MSG(MAC_TA_INSERTION,LOGBRIEF,MAC_L1_INF,\
                            currentTTI,\
                            getCurrentTick(),dlUEContext_p->lastSyncDetectedTick,\
                            ueIndex,timingAdvance,\
                            DEFAULT_INT_VALUE,\
                            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                            FUNCTION_NAME,"LAST_SYNC_TICK");
                }
                        
                /*CQI_PDCCH_ORDER_FIX_START*/
                if (UL_SYNC_LOSS_OFF == 
                        checkSyncLossTimerExpiry(dlUEContext_p))
                {
                    dlUEContext_p->lastSyncDetectedTick = getCurrentTick();
                }
                /*CQI_PDCCH_ORDER_FIX_END*/

                /* CQI will come on CRNTI and not on T-CRNTI.
                 * Check if CQI is received on PUCCH  and power control based on 
                 * SINR is enable .
                 * If yes thn call updateAndCheckCqiSinr to trigger TPC  
                 * based on received SINR. */
                /* Time Averaging changes start */
                CHECK_AND_UPDATE_CQI_SINR(dlUEContext_p, cqiPdu_p->ulCqi,
                        subFrameNum, currentTTI,internalCellIndex);
                /* Time Averaging changes end */

                /* calculate type of report */

                datalen = MAC_PHY_CONVERT_16(cqiPdu_p->length); 
                data_p = offsetNextPdu + (UInt8*)(refPdu_p);
                offsetNextPdu += datalen;

                /* + CL-MIMO LJA*/  
                /* CA_phase2_csi_code Start */

                if(dlUEContext_p->scellCount)
                {
                    /* Rel 10 CA UE */
                    if(1 == numCellsReportExpected)
                    {
                        processCQIReportForOneCellForCAUE(ulUEContext_p,dlUEContext_p,
                                cqiPdu_p,
                                datalen,
                                subFrameNum,
#ifdef TDD_CONFIG
                                sysFrameNum,
#endif 
                                data_p,
                                ueIndex,
                                recvTTI,
                                internalCellIndex);
                    }
/*ca-tdd pucch code changes*/
                    else
                    {
                        /* Aperiodic report for multiple cells */
                        for(servCellIndex = PCELL_SERV_CELL_IX; servCellIndex <= dlUEContext_p->scellCount;
                                servCellIndex++)
                        {
                            if(!servCellIndex)
                            {
                                if(cellIdxOfExpectedReportType & servingCellBitMask_g[servCellIndex])
                                {
                                    /* Pcell */
                                    if ((cqiPdu_p->numberOfCCReported)
                                            &&(cqiPdu_p->ri[riIndex]))
                                    {
                                        if (isRIInCodebookSubsetRestriction( cqiPdu_p->ri[servCellIndex],
                                                    dlUEContext_p->ueCategory,
                                                    dlUEContext_p->dlMIMOInfo.transmissionMode,
                                                    dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue,internalCellIndex))
                                        {
                                            ulUEContext_p->pucchConfigInfo.ueReportedRI= 
                                                cqiPdu_p->ri[servCellIndex];
                                        } 
                                        /* 4x4 DL MIMO CHG END */
                                        /* 4x2 MIMO E */
                                    }
                                    riIndex++;
                                    /* If RI value is not reported for first
                                     * cell for which report was expected, then
                                     * it means RI is 0 and riIndex is
                                     * incremented */
                                    count =  COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(internalCellIndex);
                                    if(NUM_PERIODIC_REPORT_NODES == count)
                                    {
                                        /* calculate number of Bits in Pcell report, so that
                                         * while decoding Scell report buffer can be shifted accordingly */
                                        numOfBitsDecoded = 
                                            calculateSizeOfReport(ulUEContext_p->pucchConfigInfo.ueReportedRI
#ifdef FDD_CONFIG
                                                    ,ulUEContext_p->reportFormatMap[subFrameNum].
                                                    ueExpectedReport,
#elif TDD_CONFIG

                                                    /* SPR 18122 Changes Start*/
                                                    ,ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].
                                                     ueExpectedReport,

                                                     /* SPR 18122 Changes End*/
#endif
                                                    internalCellIndex);
                                        continue;
                                    }
                                    /* SPR_4261_FIX Start */ 
                                    if ( datalen == RI_MULTIPLEX_WITH_PUSCH)
                                    {
                                        return;
                                    }
                                    /* SPR_4261_FIX End */ 
                                    ri = ulUEContext_p->pucchConfigInfo.ueReportedRI;
                                    transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
#ifdef FDD_CONFIG
                                    cqiReportType = ulUEContext_p->reportFormatMap[subFrameNum].ueExpectedReport;
#elif TDD_CONFIG

                                    /* SPR 18122 Changes Start*/
                                    cqiReportType = ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].ueExpectedReport;
                                    
                                    /* SPR 18122 Changes End*/
#endif
                                    decodeAperiodicCqiReportsR10(data_p,
                                            cqiReportType,
                                            ueIndex,
                                            ulUEContext_p,
                                            recvTTI,
                                            transmissionMode,
                                            dlUEContext_p,
                                            /* +- SPR 17777 */
                                            internalCellIndex,
                                            &numOfBitsDecoded
                                            );
                                }
                            }
                            else
                            {
                                /* Scell */
                                if(PNULL == dlUEContext_p->dlUeScellContext_p[servCellIndex])
                                {
                                    continue;
                                }
                                dlUeScellContext_p = dlUEContext_p->dlUeScellContext_p[servCellIndex];
                                internalCellIndex = dlUeScellContext_p->internalCellIndex;
                                if(cellIdxOfExpectedReportType & servingCellBitMask_g[servCellIndex])
                                {
                                    count = COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(
                                            internalCellIndex);
                                    if(NUM_PERIODIC_REPORT_NODES == count)
                                    {
                                        continue;
                                        /* In case when we will have multiple
                                         * Scells, then we have to shift the buffer
                                         * to point to the report of second Scell
                                         * */
                                    }
                                    if ((cqiPdu_p->numberOfCCReported) &&
                                            (cqiPdu_p->ri[riIndex]))
                                    {
                                        if (isRIInCodebookSubsetRestriction(cqiPdu_p->
                                                    ri[servCellIndex],
                                                    dlUEContext_p->ueCategory,
                                                    dlUeScellContext_p->ScelldlMIMOInfo.transmissionMode,
                                                    dlUeScellContext_p->codebookSubsetRestrictionv1020.cbsrValue,
                                                    internalCellIndex))
                                        {
                                            ulUEContext_p->ulUeScellContext_p[servCellIndex]->
                                                scellpucchConfigInfo.ueReportedRI= 
                                                cqiPdu_p->ri[servCellIndex];
                                        } 
                                        /* 4x4 DL MIMO CHG END */
                                        /* 4x2 MIMO E */
                                    }
                                    riIndex++;
                                    ri = ulUEContext_p->ulUeScellContext_p[servCellIndex]->
                                        scellpucchConfigInfo.ueReportedRI; 
                                    transmissionMode = dlUeScellContext_p->ScelldlMIMOInfo.transmissionMode;
                                    /** SPR 14032 Fix : Start **/
#ifdef FDD_CONFIG                                    
                                    cqiReportType = ulUEContext_p->
                                        ulUeScellContext_p[servCellIndex]->
                                        cqiPmiRiReportFormatMapSCell[subFrameNum].ueExpectedReport;
#elif TDD_CONFIG

                                        /* SPR 18122 Changes Start*/
                                    cqiReportType = ulUEContext_p->
                                        ulUeScellContext_p[servCellIndex]->
                                        cqiPmiRiReportFormatMapSCell[MODULO(sysFrameNum,2)][subFrameNum].ueExpectedReport;

                                        /* SPR 18122 Changes End*/
#endif                                        
                                    /** SPR 14032 Fix : End **/
                                    numOfBitsOfScellReport = 
                                        calculateSizeOfReport(ri,cqiReportType,
                                                internalCellIndex);
                                    if(0 == numOfBitsOfScellReport)
                                    {
                                        continue;
                                    }
                                    // numOfBitsDecoded contain num of bits decoded
                                    // for Pcell
                                    if(0 == (numOfBitsDecoded % NUMBER_OF_BITS_IN_1BYTE))
                                    {
                                        data_p = data_p + (numOfBitsDecoded / NUMBER_OF_BITS_IN_1BYTE);
                                        decodeAperiodicCqiReportsR10(data_p,
                                                cqiReportType,
                                                ueIndex,
                                                ulUEContext_p,
                                                recvTTI,
                                                transmissionMode,
                                                dlUEContext_p,
                                                /* +- SPR 17777 */
                                                internalCellIndex,
                                                &numOfBitsDecoded
                                                );
                                    }
                                    else
                                    {
                                        offset = numOfBitsDecoded % NUM_OF_BITS_IN_32_BIT_WORD;
                                        totalBits = numOfBitsDecoded + numOfBitsOfScellReport;
                                        if(totalBits <= NUM_OF_BITS_IN_32_BIT_WORD)
                                        {
                                            memCpy(tempArr,data_p,4);
                                            tempArr[ZERO] = tempArr[ZERO] >> numOfBitsDecoded;
                                            decodeAperiodicCqiReportsR10((UInt8 *)tempArr,
                                                    cqiReportType,
                                                    ueIndex,
                                                    ulUEContext_p,
                                                    recvTTI,
                                                    transmissionMode,
                                                    dlUEContext_p,
                                                    /* +- SPR 17777 */
                                                    internalCellIndex,
                                                    &numOfBitsDecoded
                                                    );
                                        }
                                        else
                                        {
                                            /*increment data_p to point to the 32
                                              bit word containing report for Scell*/
                                            if(numOfBitsDecoded > NUM_OF_BITS_IN_32_BIT_WORD)
                                            {
                                                data_p = data_p + 4;
                                            }
                                            remainingBitsInWord = NUM_OF_BITS_IN_32_BIT_WORD - offset;
                                            if((NUM_OF_BITS_IN_32_BIT_WORD + remainingBitsInWord)>= numOfBitsOfScellReport)
                                            {
                                                memCpy(tempArr, data_p ,
                                                        8);
                                                endIndex = ONE;
                                            }
                                            else
                                            {
                                                memCpy(tempArr, data_p ,
                                                        12);
                                                endIndex = 2;
                                            }
                                            if(ONE == endIndex)
                                            {
                                                tempArr[ZERO] = tempArr[ZERO] >> offset;
                                                temp32Var = tempArr[ONE];
                                                temp32Var = temp32Var << (NUM_OF_BITS_IN_32_BIT_WORD- offset);
                                                tempArr[ZERO]|= temp32Var;
                                                if(numOfBitsOfScellReport > NUM_OF_BITS_IN_32_BIT_WORD)
                                                {
                                                    tempArr[ONE] = tempArr[ONE] >> offset;
                                                }

                                            }
                                            else
                                            {
                                                tempArr[ZERO] = tempArr[ZERO] >> offset;
                                                temp32Var = tempArr[ONE];
                                                temp32Var = temp32Var << (NUM_OF_BITS_IN_32_BIT_WORD- offset);
                                                tempArr[ZERO]|= temp32Var;
                                                tempArr[ONE] = tempArr[ONE] >> offset;
                                                /* rest offset bits in tempArr[1] are
                                                 * to be copied from tempArr[2]*/
                                                temp32Var = tempArr[2];
                                                temp32Var = temp32Var << (NUM_OF_BITS_IN_32_BIT_WORD- offset);
                                                tempArr[ONE]|= temp32Var;
                                                tempArr[2] = tempArr[2] >> offset;

                                            }
                                            decodeAperiodicCqiReportsR10((UInt8 *)tempArr,
                                                    cqiReportType,
                                                    ueIndex,
                                                    ulUEContext_p,
                                                    recvTTI,
                                                    transmissionMode,
                                                    dlUEContext_p,
                                                    /* +- SPR 17777 */
                                                    internalCellIndex,
                                                    &numOfBitsDecoded
                                                    );
                                        }

                                    }
                                }
                            }

                        }// for ends
                    }
                    /* reset bitmask cellIdxOfExpectedReportType */
#ifdef FDD_CONFIG
                    ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType = 0;
#elif TDD_CONFIG

                    /* SPR 18122 Changes Start*/
                    ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].cellIdxOfExpectedReportType = 0;

                    /* SPR 18122 Changes End*/
#endif
                }
                else
                {
                    /* either Rel9 or Rel 10 without CA */
                    /* 4x2 MIMO S */
                    /* 4x4 DL MIMO CHG START */
                    /* SPR 10177 changes start */
                    if (cqiPdu_p->ri[0])
                    {
                        if ((TX_MODE_8 == dlUEContext_p->dlMIMOInfo.transmissionMode) ||
                                /* SPR 10177 changes end */
                                (isRIInCodebookSubsetRestriction( cqiPdu_p->ri[0],
                                                                  dlUEContext_p->ueCategory,
                                                                  dlUEContext_p->dlMIMOInfo.transmissionMode,
                                                                  dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue,internalCellIndex)))
                        {
                            ulUEContext_p->pucchConfigInfo.ueReportedRI= cqiPdu_p->ri[0];
                        } 
                        /* 4x4 DL MIMO CHG END */
                        /* 4x2 MIMO E */
                    }
                    /* SPR_4261_FIX Start */ 
                    if ( datalen == RI_MULTIPLEX_WITH_PUSCH)
                    {
                        return;
                    }
                    /* SPR_4261_FIX End */ 

                    ri = ulUEContext_p->pucchConfigInfo.ueReportedRI;
                    /* - CL-MIMO LJA*/

                    /* + CQI_4.1 */
                    transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
                    /* - CQI_4.1 */

                    LOG_MAC_MSG(MAC_CQI_DECODE_ID,LOGINFO,MAC_L1_INF,\
                            currentTTI,\
                            cqiReportType,\
                            ueIndex,\
                            recvTTI,datalen,ri,\
                            0.0,0.0,__func__,"CQI_DECODE");

                    decodeCqiReports(data_p,
                            cqiReportType,
                            ueIndex,
                            ulUEContext_p,
                            recvTTI,
                            transmissionMode,
                            dlUEContext_p,
                            subFrameNum,
                            internalCellIndex);
                    /*Cyclomatic Complexity changes - ends here */
                }
            }
        }
    }


    handleVendorSpecificInfoR9(vendorSpecific_p,&validReports,recvTTI,internalCellIndex);

    /* - E_CID_5.3 */

    return;
}

#endif
/*SPR 19433 -*/

#if   (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
/*****************************************************************************
 * Function Name  : processRxSubFrameInd 
 * Inputs         : dlDelay - Downlink Delay
 *                  threadIndex - thread ID
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : None
 * Returns        : MacRetType 
 * Description    : This function Process the uplink  messages received
 and Sub Frame Indication Received from PHY
 *****************************************************************************/
MacRetType processRxSubFrameInd( UInt32 dlDelay,
        UInt32 threadIndex,
        /*CA Changes start  */       
        InternalCellIndex  internalCellIndex
        /*CA Changes end  */       
        )
{
    /* SPR 19288 change start */
    QSEGMENT segment;
    /* SPR 19288 change end */

#ifdef LAYER2_PHYSIM_THREAD
    macPhyMsg *ulMessage = PNULL;
#endif
    /* FAPI L2 AS MASTER START */
#ifndef FAPI_L2_MASTER
    /* SPR 19288 change start */
    UInt8 tickReceived=0;
    /* SPR 19288 change end */

    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t tick;
    /* SPR 15909 fix end */
#endif
    /*HD FDD Changes End*/
#endif
    /* FAPI L2 AS MASTER END */
    UInt16 sysFrameNum   = 0;
    UInt8  subFrameNum   = 0;
    /* SPR 15909 fix start */
    tickType_t recvTTI       = 0;
    /* SPR 15909 fix end */
    UInt16 msgId         = 0;
    UInt16 msgLen        = 0;
    UInt8  *messageBuffer_p      = PNULL;
    UInt8  *msg_p        = PNULL;
    /* SPR 15909 fix start */
    tickType_t currentTick = getCurrentTick();
    /* SPR 15909 fix end */
    FAPI_l1ApiMsg_st    *pL1Api = PNULL;
    FAPI_subFrameIndication_st  *pSubFrameInd = PNULL;
    FAPI_harqIndication_st      *pHarqMsg = PNULL;
    FAPI_crcIndication_st       *crcMsg_p = PNULL; 
    FAPI_phyErrorIndication_st  *errMsg_p = PNULL; 
    FAPI_rxULSCHIndication_st   *ulSchMsg_p = PNULL; 
    FAPI_rachIndication_st      *rachMsg_p = PNULL;
    FAPI_rxSRIndication_st      *srMsg_p = PNULL;
    FAPI_rxCqiIndication_st   *cqiMsg_p = PNULL; 
    FAPI_srsIndication_st       *srsMsg_p = PNULL;



    /* +DYNAMIC_ICIC */
    FAPI_measReportInd_st  *measReportInd_p = PNULL;
    FAPI_measReportInfo_st *measReportInfo_p = PNULL; 
    /* -DYNAMIC_ICIC */   
    /* + TM7_8 Changes Start */
    FAPI_VendorSpecific_st *vendorSpecificData_p = PNULL;
    /* - TM7_8 Changes End */
    /* + E_CID_5.3 */
    FAPI_VendorSpecificForRACHInd_st *vendorSpecificRach_p = PNULL;
    /* - E_CID_5.3 */
    /* RX_IND_PORTING Changes */
    ReceivePhyDataQueueNode *recvPhyDataQueueNode_p = PNULL;
    /* RX_IND_PORTING Changes */
    MacRetType resp = MAC_SUCCESS;
#ifdef TDD_CONFIG
    /* SPR 1257 changes start */
    /* SPR 1257 changes end */
#endif

    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Entry. ",__func__);
    /* Receive the data from the Physical Layer Until the subframeIndicaiton
       recvd or numOfMsg read as configured*/
    /* SPR 19288 change start */
    /*mandeep changes start*/
    /*mandeep changes stop*/
    /* SPR 19288 change end */

    while(1)
    {
        /* SPR 19288 change start */
        /*SPR 21001 Changes start */
        /*CA Changes start  */
        recvPhyDataQueueNode_p = (ReceivePhyDataQueueNode *)popNode(recvDataFromPhyQ_gp[internalCellIndex]);
        /*CA Changes end  */
        /*SPR 21001 Changes end */


        if(PNULL == recvPhyDataQueueNode_p)
        {
            return MAC_SUCCESS;
        }


#ifndef LAYER2_PHYSIM_THREAD
        if (PNULL ==  msgSegNext(recvPhyDataQueueNode_p->recvDataBuffer_p, 0, &segment))
        {
            lteWarning("Error In msgSegNext\n");

            LOG_MAC_MSG(L2_MSG_SEGNEXT_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                    currentTick,\
                    __LINE__,0,0,0,0,\
                    0.0,0.0,__FILE__,__func__);

            resp = MAC_FAILURE;
            if(recvPhyDataQueueNode_p->recvDataBuffer_p)
            {
                msgFree(recvPhyDataQueueNode_p->recvDataBuffer_p);
                recvPhyDataQueueNode_p->recvDataBuffer_p = PNULL;
            }

            freeMemPool(recvPhyDataQueueNode_p);
            recvPhyDataQueueNode_p = PNULL;

            continue;
        }
        msg_p = segment.base;
#endif
        /*SPR 21001 Changes end */
        /* SPR 19288 change end */
        /* SPR 19288 change start */

            /* Message buffer length should be greater than 10(size of API
             * Header)*/
        if (recvPhyDataQueueNode_p->byteRead > L1_API_HEADER_LEN )
            /*SPR 21001 Changes end */
            /* SPR 19288 change end */
        {
#ifdef LAYER2_PHYSIM_THREAD
            if (PNULL ==  msgSegNext(recvPhyDataQueueNode_p->recvDataBuffer_p, 0, &segment))
            {
                ltePanic("Error In msgSegNext:%s \n",__func__);

                LOG_MAC_MSG(L2_MSG_SEGNEXT_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                        currentTick,\
                        __LINE__,0,0,0,0,\
                        0.0,0.0,__FILE__,__func__);

                resp = MAC_FAILURE;
                if(recvPhyDataQueueNode_p->recvDataBuffer_p)
                {
                    msgFree(recvPhyDataQueueNode_p->recvDataBuffer_p);
                    recvPhyDataQueueNode_p->recvDataBuffer_p = PNULL;
                }

                freeMemPool(recvPhyDataQueueNode_p);
                recvPhyDataQueueNode_p = PNULL;

                continue;
            }
            msg_p=segment.base;
#endif
            messageBuffer_p = (UInt8 *)msg_p;
            pL1Api = (FAPI_l1ApiMsg_st *)messageBuffer_p;
            msgId = pL1Api->msgId;
            msgLen = MAC_PHY_CONVERT_16(pL1Api->msgLen);
            vendorSpecificData_p = PNULL;
            LTE_MAC_UT_LOG(LOG_DETAIL,RECV_INTF,"msgId = %d,msgLen = %d\n",
                    msgId, msgLen);
            switch ( msgId )
            {
                case PHY_UL_SUBFRAME_INDICATION:
                {
                    /* SPR 19288 change start */

                    pSubFrameInd = (FAPI_subFrameIndication_st *)
                        &pL1Api->msgBody[0];
                    sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF(
                            pSubFrameInd->sfnsf);
                    subFrameNum = GET_SUBFRAME_FROM_SFNSF(pSubFrameInd->sfnsf);
#ifdef FLEXRAN
                    if ((getCurrentTick()%10000) == 0)
                    {
                        fprintf(stderr,"cellId [%d] Tick [%llu] numPrach [%u] numCRC Nack/All [%u / %u] numHarq Nack/All [%u / %u]\n",
                        	internalCellIndex, getCurrentTick(),numOfPreambles_g[internalCellIndex],
                        	numOfCrcNack_g[internalCellIndex], numOfCrc_g[internalCellIndex],
                        	numOfHarqNack_g[internalCellIndex], numOfHarq_g[internalCellIndex]);
    
                        LOG_MAC_MSG(MAC_PERIOD_CELL_LOG_INFO_ID,LOGWARNING,MAC_L1_INF,
                                currentTick_g[internalCellIndex],
                                numOfPreambles_g[internalCellIndex],
                            	numOfCrcNack_g[internalCellIndex], numOfCrc_g[internalCellIndex],
                            	numOfHarqNack_g[internalCellIndex], numOfHarq_g[internalCellIndex],
                                0.0,0.0,"",""); 

                        numOfPreambles_g[internalCellIndex] = 0;
                        numOfCrcNack_g[internalCellIndex] = 0;
                        numOfCrc_g[internalCellIndex] = 0;
                        numOfHarqNack_g[internalCellIndex] = 0;
                        numOfHarq_g[internalCellIndex] = 0;
                    }
#endif

#ifdef OTA_DL_ADVANCE_2
                    /* OTA_DL_ADVANCE changes start */
#ifdef TDD_CONFIG
                    GET_REQUIRED_TICK(sysFrameNum ,subFrameNum, 1);
#endif
                    /* OTA_DL_ADVANCE changes end */
#endif
                    /* FAPI L2 AS MASTER START */
#ifdef FAPI_L2_MASTER
                    /*
                     ** Update the Tick, system frame and subframe Number for L2.
                     **
                     ** This function will be invoked by ISR once system
                     ** has been configured to send tick to L2.
                     */
                    if (startExecutionAfterPhyStart_g[internalCellIndex] != TRUE)
                    {
                        /* compilation 10-JUN-2014 fix start */
                        processTickReceivedOnL2(internalCellIndex);
                        /* compilation 10-JUN-2014 fix end */

                        /* Validate the subframe and sysframe number */
                        validateSFIFrmL1(sysFrameNum, subFrameNum,internalCellIndex);
                    }
#else


#ifdef OVERSHOOT_CORRECTION
                    processSubFrameIndicationForTickMiss(sysFrameNum,subFrameNum,internalCellIndex);
#else

                    processSubFrameInd(sysFrameNum,subFrameNum,internalCellIndex);
#endif
#ifdef LTE_EMBMS_SUPPORTED
#ifdef TDD_CONFIG
                    sendMacTriggerToSyncAndCheckMacPreOppCalc(DL_ADVANCE_TICK, internalCellIndex);
#endif
                    updateMCCHUpdateTimeForAreaIndex(internalCellIndex);
#endif
#if !defined(FLEXRAN)
                    tickReceived=1;
#endif

#ifdef DL_UL_SPLIT
                    /* CA Stats Changes Start */
                    lteMacUpdateStatsPerfStart(DL_OVERSHOOT_STATS, internalCellIndex);
                    lteMacUpdateStatsPerfStart(UL_OVERSHOOT_STATS, internalCellIndex);
#else
                    lteMacUpdateStatsPerfStart(internalCellIndex);
                    /* CA Stats Changes End */
#endif
                    /* RDTSC_AND_CLOCK_START_TDD;*/
#endif
                    /* FAPI L2 AS MASTER END */
                    /* SPR 19288 change end */
                    break;
                }
                case PHY_UL_HARQ_INDICATION:
                {
                    /* typecaseting to FAPI_harqIndication_st just to extract sfnsf even in case 
                       FAPI_4_0_COMPLIANCE is defined */

                    pHarqMsg = (FAPI_harqIndication_st *)&pL1Api->msgBody[0];
                    sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( pHarqMsg->sfnsf);
                    subFrameNum = GET_SUBFRAME_FROM_SFNSF(pHarqMsg->sfnsf);
                    /* validate the system Frame/ SubFrameNum and also fill the
                       tick of recieved message buffer*/
                    if (MAC_FAILURE == validatePHYDelayAndTick(sysFrameNum,
                                subFrameNum, &recvTTI, msgId, 
                                internalCellIndex ) )
                    {
                        lteWarning("Packet Dropped due to delayed Recvd "
                                "(msgId =%x)\n",msgId);
                        LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                currentTick,\
                                PHY_UL_HARQ_INDICATION,sysFrameNum,subFrameNum,\
                                getSysFrameNumer(internalCellIndex),getSubFrameNumber(internalCellIndex),\
                                0.0,0.0,__func__,"HARQ_INV_SFNSF");

                        break;
                    }
                    /*CA Changes start  */       
                    (*processHARQIndicationFunction)(pHarqMsg, recvTTI, sysFrameNum, subFrameNum, internalCellIndex); 
                    /*CA Changes end  */       
                    break;
                }
                case PHY_UL_CRC_INDICATION:
                {
                    {
                        /* SPR 21660_2 changes end */
                        crcMsg_p = (FAPI_crcIndication_st *)&pL1Api->msgBody[0];
                        sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( crcMsg_p->sfnsf);
                        subFrameNum = GET_SUBFRAME_FROM_SFNSF(crcMsg_p->sfnsf);
                        /* validate the system Frame/ SubFrameNum and also fill the
                           tick of recieved message buffer*/
                        if (MAC_FAILURE == validatePHYDelayAndTick(sysFrameNum,
                                    subFrameNum, &recvTTI, msgId, 
                                    internalCellIndex ) )
                        {

                            lteWarning("Packet Dropped due to delayed Recvd "
                                    "(msgId =%x)\n",msgId);
                            LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                    currentTick,\
                                    PHY_UL_CRC_INDICATION,sysFrameNum,subFrameNum,\
                                    getSysFrameNumer(internalCellIndex),getSubFrameNumber(internalCellIndex),\
                                    0.0,0.0,__func__,"CRC_INV_SFNSF");

                            break;
                        }
                        /* Now process all the CRC pdus from the CRC.Indication
                         * message */

                        /* msg3_retx_tdd_support_start */
                        /*CA Changes start  */
                        {
                            processULCrcInd(crcMsg_p, recvTTI, currentTick,subFrameNum,sysFrameNum, internalCellIndex);
                        }
                        /*CA Changes end  */
                        /* msg3_retx_tdd_support_end */
                        /* SPR 21660_2 changes start */
                    }
                    /* SPR 21660_2 changes end */
                    break;
                }
                case PHY_ERROR_INDICATION:
                {
                    errMsg_p = (FAPI_phyErrorIndication_st *)&pL1Api->msgBody[0];

                    /*CA Changes start  */
                    processULErrInd(errMsg_p, internalCellIndex);
                    /* SPR 19288 change start */
                    /* SPR 19288 change end */

                    /*CA Changes end  */
                    break;
                }
                case PHY_UL_RX_ULSCH_INDICATION:
                {
                    /* SPR 19288 change start */
                    /* SPR 19288 change end */

                    ulSchMsg_p = (FAPI_rxULSCHIndication_st *)&pL1Api->msgBody[0];
                    sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( ulSchMsg_p->sfnsf);
                    subFrameNum = GET_SUBFRAME_FROM_SFNSF(ulSchMsg_p->sfnsf);
                    /* validate the system Frame/ SubFrameNum and also fill the
                       tick of recieved message buffer*/
                    if (MAC_FAILURE == validatePHYDelayAndTick(sysFrameNum,
                                subFrameNum, &recvTTI, msgId, 
                                internalCellIndex))
                    {
                        lteWarning("Packet Dropped due to delayed Recvd "
                                "(msgId =%x)\n",msgId);
                        LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                currentTick,\
                                PHY_UL_RX_ULSCH_INDICATION,sysFrameNum,subFrameNum,\
                                getSysFrameNumer(internalCellIndex),getSubFrameNumber(internalCellIndex),\
                                0.0,0.0,__func__,"ULSCH_INV_SFNSF");
                        break;
                    }
                    /* SPR 19288 change end */
                    /* + TM7_8 Changes Start */
                    if(pL1Api->lenVendorSpecific != 0)
                    {
                        vendorSpecificData_p = (FAPI_VendorSpecific_st *) (pL1Api->msgBody + msgLen);
                    }
                    /* SPR 21660_2 changes start */
                    {
                        /* SPR 21660_2 changes end */

                        (*processULSchIndFunction)(vendorSpecificData_p,ulSchMsg_p,
                                /* SPR 19288 change start */
#if  defined FLEXRAN
                                0
#else
                                recvPhyDataQueueNode_p->recvDataBuffer_p
#endif
                                /* SPR 19288 change end */

                                ,msgLen, recvTTI, 
                                /* Time Averaging changes start */
                                0 /*withoutDemux*/, dlDelay, threadIndex,
                                /*CA Changes start  */
                                internalCellIndex
                                /* SPR 19288 change start */
                                /* SPR 19288 change end */

                                );
                        /* SPR 21660_2 changes start */
                    }
                    /* SPR 21660_2 changes end */
                    /*CA Changes end  */
                    /* Time Averaging changes end */
                    /* - TM7_8 Changes End */
                    break;
                }
                case PHY_UL_RACH_INDICATION:
                {
                    rachMsg_p = (FAPI_rachIndication_st *)&pL1Api->msgBody[0];
                    sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( rachMsg_p->sfnsf);
                    subFrameNum = GET_SUBFRAME_FROM_SFNSF(rachMsg_p->sfnsf);
                    /* validate the system Frame/ SubFrameNum and also fill the
                       tick of recieved message buffer*/
                    if (MAC_FAILURE == validatePHYDelayAndTick(sysFrameNum,
                                subFrameNum, &recvTTI, msgId, 
                                internalCellIndex))
                    {

                        lteWarning("Packet Dropped due to delayed Recvd "
                                "(msgId =%x)\n",msgId);
                        LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                currentTick,\
                                PHY_UL_RACH_INDICATION,sysFrameNum,subFrameNum,\
                                getSysFrameNumer(internalCellIndex),getSubFrameNumber(internalCellIndex),\
                                0.0,0.0,__func__,"RACH_INV_SFNSF");

                        break;
                    }

                    if(pL1Api->lenVendorSpecific != 0)
                    {
                        vendorSpecificRach_p = (FAPI_VendorSpecificForRACHInd_st *)((UInt8 *)(pL1Api->msgBody )+ msgLen);
                    }

                    /*CA Changes start  */
                    (*processRACHIndicationFunction)(rachMsg_p, recvTTI,vendorSpecificRach_p, internalCellIndex);
                    /*CA Changes end  */
                    break;
                }
                case PHY_UL_RX_SR_INDICATION:
                {
                    srMsg_p = (FAPI_rxSRIndication_st *)&pL1Api->msgBody[0];
                    sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( srMsg_p->sfnsf);
                    subFrameNum = GET_SUBFRAME_FROM_SFNSF(srMsg_p->sfnsf);
                    /* validate the system Frame/ SubFrameNum and also fill the
                       tick of recieved message buffer*/
                    if (MAC_FAILURE == validatePHYDelayAndTick(sysFrameNum,
                                subFrameNum, &recvTTI, msgId,
                                internalCellIndex))
                    {

                        lteWarning("Packet Dropped due to delayed Recvd "
                                "(msgId =%x)\n",msgId);
                        LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                currentTick,\
                                PHY_UL_RX_SR_INDICATION,sysFrameNum,subFrameNum,\
                                getSysFrameNumer(internalCellIndex),getSubFrameNumber(internalCellIndex),\
                                0.0,0.0,__func__,"SR_INV_SFNSF");

                        break;
                    }
                    /* Rel9_upgrade_CR410 */

                    /*CA Changes start  */
                    processSRIndication(srMsg_p, recvTTI,dlDelay, internalCellIndex);
                    /*CA Changes end  */
                    /* Rel9_upgrade_CR410 */

                    break;
                }

                case PHY_UL_RX_CQI_INDICATION:
                {
                    cqiMsg_p = (FAPI_rxCqiIndication_st*)&pL1Api->msgBody[0];
                    sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( cqiMsg_p->sfnsf);
                    subFrameNum = GET_SUBFRAME_FROM_SFNSF(cqiMsg_p->sfnsf);
                    /* validate the system Frame/ SubFrameNum and also fill the
                       tick of recieved message buffer*/
                    if(MAC_FAILURE == validatePHYDelayAndTick(sysFrameNum,
                                subFrameNum, &recvTTI, msgId, 
                                internalCellIndex))
                    {

                        lteWarning("Packet Dropped due to delayed Recvd "
                                "(msgId =%x)\n",msgId);
                        LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                currentTick,\
                                PHY_UL_RX_CQI_INDICATION,sysFrameNum,subFrameNum,\
                                getSysFrameNumer(internalCellIndex),getSubFrameNumber(internalCellIndex),\
                                0.0,0.0,__func__,"CQI_INV_SFNSF");

                        break;
                    }
                    /* + TM7_8 Changes Start */
                    if(pL1Api->lenVendorSpecific != 0)
                    {
                        vendorSpecificData_p = (FAPI_VendorSpecific_st *) (pL1Api->msgBody + msgLen);
                    }
                    (*processCQIIndicationFunction)(vendorSpecificData_p,cqiMsg_p, recvTTI,
                            /*SPR 5620 START*/ 
#ifdef TDD_CONFIG
                            sysFrameNum,
#endif
                            /*SPR 5620 END*/ 
                            subFrameNum,
                            /*CA Changes start  */
                            internalCellIndex
                            /*CA Changes start  */
                            );
                    /* - TM7_8 Changes End */
                    break;
                }
                case PHY_UL_SRS_INDICATION:
                {
                    srsMsg_p = (FAPI_srsIndication_st *)&pL1Api->msgBody[0];
                    sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( srsMsg_p->sfnsf);
                    subFrameNum = GET_SUBFRAME_FROM_SFNSF(srsMsg_p->sfnsf);
                    /* validate the system Frame/ SubFrameNum and also fill the
                       tick of recieved message buffer*/
                    if (MAC_FAILURE == validatePHYDelayAndTick(sysFrameNum,
                                subFrameNum, &recvTTI, msgId, 
                                internalCellIndex))
                    {

                        lteWarning("Packet Dropped due to delayed Recvd "
                                "(msgId =%x)\n",msgId);
                        LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                currentTick,\
                                PHY_UL_SRS_INDICATION,sysFrameNum,subFrameNum,\
                                getSysFrameNumer(internalCellIndex),getSubFrameNumber(internalCellIndex),\
                                0.0,0.0,__func__,"CQI_INV_SFNSF");

                        break;
                    }
                    /* + TM7_8 Changes Start */
                    if(pL1Api->lenVendorSpecific != 0)
                    {
                        vendorSpecificData_p = (FAPI_VendorSpecific_st *) (pL1Api->msgBody + msgLen);
                    }
                    /*CA Changes start  */
                    (*processSRSIndicationFunction)(vendorSpecificData_p,srsMsg_p,recvTTI, internalCellIndex);
                    /*CA Changes end  */
                    /* - TM7_8 Changes End */
                    break;
                }

                /* +DYNAMIC_ICIC */  
                case PHY_MEASUREMENT_REPORT_INDICATION:
                {
                    measReportInfo_p = (FAPI_measReportInfo_st*)&pL1Api->msgBody[0];
                    measReportInd_p = (FAPI_measReportInd_st*)&(measReportInfo_p->measReportInfo); 
                    sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( measReportInfo_p->sfnsf);
                    subFrameNum = GET_SUBFRAME_FROM_SFNSF(measReportInfo_p->sfnsf);

                    /* validate the system Frame/ SubFrameNum and also fill the
                       tick of recieved message buffer*/
                    if (MAC_FAILURE == validatePHYDelayAndTick(sysFrameNum,
                                subFrameNum, &recvTTI, msgId,
                                internalCellIndex))
                    {
                        lteWarning("Packet Dropped due to delayed Recvd "
                                "(msgId =%x)\n",msgId);
                        //Log msg
                        break;
                    }
                    processMeasReportIndication(measReportInd_p, internalCellIndex);
                    break;
                }


                default:
                lteWarning("2....Invalid API ID Received = %x\n",msgId);

                LOG_MAC_MSG(MAC_PHY_INV_MSG_ID,LOGWARNING,MAC_L1_INF,\
                        currentTick,\
                        msgId,0,0,0,0,\
                        0.0,0.0,__func__,"INV_MSG_ID");

                break;
            }
        }
        else
        {
            /* SPR 19288 change start */
            /*SPR 21001 Changes start */
            if (recvPhyDataQueueNode_p->byteRead != 0)
            {
                lteWarning("Invalid Message size %d received.",recvPhyDataQueueNode_p->byteRead);
                LOG_MAC_MSG(MAC_PHY_INV_MSG_LEN_ID,LOGWARNING,MAC_L1_INF,\
                        currentTick,\
                        recvPhyDataQueueNode_p->byteRead,0,0,0,0,\
                        0.0,0.0,__func__,"INV_MSG_LEN");

            }
            /* CID:58277 Fix Start  */
            resp = MAC_FAILURE;
            /* CID:58277 Fix End  */
        }

        /* FAPI L2 AS MASTER START */
#ifndef FAPI_L2_MASTER

        if(tickReceived) 
        {

#ifdef HD_FDD_CONFIG
            /*Update dlDelay value which will be used for updation of sched maps of HD FDD UEs*/
            if(hdUeContextInfo_g.numHdUe > 0)
            {
                tick=getCurrentTick() + PHY_DL_DELAY;
                updateHDUeSubFrameCntxtInfo(tick);
            }

#endif
            /* HD FDD Changes End */
#ifdef DL_UL_SPLIT
            putEntryInAckNackQueue(INVALID_UE_ID, 255 , 
                    subFrameNum, FAPI_OTHER_CRNTI
                    /* +- SPR 17777 */
                    /*CA Changes start  */
                    ,internalCellIndex
                    /*CA Changes end  */
                    );
#endif
            break;
        }
#endif
        /* SPR 19288 change start */
        /*SPR 21001 Changes start */
        /* FAPI L2 AS MASTER END */
        /* SPR 15513 FIX start*/
        if((recvPhyDataQueueNode_p->recvDataBuffer_p) && (msgId !=
                    PHY_STOP_INDICATION))
            /* SPR 15513 FIX end*/ 
        {
            msgFree(recvPhyDataQueueNode_p->recvDataBuffer_p);
            recvPhyDataQueueNode_p->recvDataBuffer_p = PNULL;
        }
        /* SPR 15513 FIX start*/
        recvPhyDataQueueNode_p->recvDataBuffer_p = PNULL;
        /* SPR 15513 FIX end*/
        freeMemPool(recvPhyDataQueueNode_p);
        recvPhyDataQueueNode_p = PNULL;
        /*SPR 21001 Changes end */
        /* SPR 19288 change end */
    }
    /* SPR 19288 change start */
    /*SPR 21001 Changes start */
    /* SPR 15513 FIX start*/
    if((recvPhyDataQueueNode_p->recvDataBuffer_p)&& (msgId !=
                PHY_STOP_INDICATION))
        /* SPR 15513 FIX end*/
    {
        msgFree(recvPhyDataQueueNode_p->recvDataBuffer_p);
        recvPhyDataQueueNode_p->recvDataBuffer_p = PNULL;
    }
    /* SPR 15513 FIX start*/
    recvPhyDataQueueNode_p->recvDataBuffer_p = PNULL;
    /* SPR 15513 FIX end*/
    freeMemPool(recvPhyDataQueueNode_p);
    recvPhyDataQueueNode_p = PNULL;	
    /*SPR 21001 Changes end */
    /* SPR 19288 change end */
    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Exit. ",__func__);
    return resp;
}

#else

/* SS_S2 */
/*****************************************************************************
 * Function Name  : processRxSubFrameInd 
 * Inputs         : dlDelay - Downlink Delay
 *                  threadIndex - thread ID
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : None
 * Returns        : MacRetType 
 * Description    : This function Process the uplink  messages received
 and Sub Frame Indication Received from PHY
 *****************************************************************************/
void processRxSubFrameInd( UInt32 dlDelay,
        UInt32 threadIndex,
        /*CA Changes start  */       
        InternalCellIndex  internalCellIndex
        /*CA Changes end  */       
        )
{
    void *zeroBufCpy_p     = PNULL;
    QSEGMENT segment;
    /* SPR 3444 changes start */
    /* SPR 3444 changes end */
#ifdef LAYER2_PHYSIM_THREAD
    macPhyMsg *ulMessage = PNULL;
#endif
    /* FAPI L2 AS MASTER START */
#ifndef FAPI_L2_MASTER
    UInt8 tickReceived=0;
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t tick;
    /* SPR 15909 fix end */
#endif
    /*HD FDD Changes End*/
#endif
    /* FAPI L2 AS MASTER END */
    UInt16 sysFrameNum   = 0;
    UInt8  subFrameNum   = 0;
    /* SPR 15909 fix start */
    tickType_t recvTTI       = 0;
    /* SPR 15909 fix end */
    UInt16 msgId         = 0;
    UInt16 msgLen        = 0;
    SInt32 bytesRead     = 0;
    UInt8  *messageBuffer_p      = PNULL;
    UInt8  *msg_p        = PNULL;
    /* SPR 15909 fix start */
    tickType_t currentTick = getCurrentTick();
    /* SPR 15909 fix end */
    FAPI_l1ApiMsg_st    *pL1Api = PNULL;
    FAPI_subFrameIndication_st  *pSubFrameInd = PNULL;
    FAPI_harqIndication_st      *pHarqMsg = PNULL;
    FAPI_rxCqiIndication_st   *cqiMsg_p = PNULL; 
#ifndef DL_UL_SPLIT_TDD
    FAPI_srsIndication_st       *srsMsg_p = PNULL;
    FAPI_rxULSCHIndication_st   *ulSchMsg_p = PNULL; 
    FAPI_rachIndication_st      *rachMsg_p = PNULL;
    FAPI_crcIndication_st       *crcMsg_p = PNULL; 
    FAPI_phyErrorIndication_st  *errMsg_p = PNULL; 
    FAPI_rxSRIndication_st      *srMsg_p = PNULL;
    /* TDD-SPLIT-GCC Warning */
    /* + SPR_7503 */
    FAPI_VendorSpecificForRACHInd_st *vendorSpecificRach_p = PNULL;
    /* - SPR_7503 */
    /* TDD-SPLIT-GCC Warning */
#endif
    /* SPR 23393 Changes Start */
    /* SPR 23393 Changes End */

    /* +DYNAMIC_ICIC */
    FAPI_measReportInd_st  *measReportInd_p = PNULL;
    FAPI_measReportInfo_st *measReportInfo_p = PNULL; 
    /* -DYNAMIC_ICIC */   

    /* + TM7_8 Changes Start */

    FAPI_VendorSpecific_st *vendorSpecificData_p = PNULL;
    /* - TM7_8 Changes End */

    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Entry. ",__func__);
    /* Receive the data from the Physical Layer Until the subframeIndicaiton
       recvd or numOfMsg read as configured*/


    /* FAPI AS L2 MASTER START */
#ifndef FAPI_L2_MASTER
#ifdef TDD_CONFIG
    while ((currentTick == getCurrentTick()))

#elif FDD_CONFIG
        do
#endif
        {
#endif
            /* FAPI AS L2 MASTER End */

#ifdef TDD_CONFIG
            waitOnSelectPhyMsg(phyRxSockFD_g);
#endif
#ifdef PHYSIM_DEBUG

            sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF(sfnsf_g);
            subFrameNum = GET_SUBFRAME_FROM_SFNSF(sfnsf_g);
			/* + PERF_CA GBL */
            lLteMacStatsRfSfn_g[internalCellIndex] = sysFrameNum;
            lLteMacStatsRfSf_g[internalCellIndex] = subFrameNum;
			/* - PERF_CA GBL */

#ifdef DL_UL_SPLIT
            /* CA Stats Changes Start */
            lteMacUpdateStatsPerfStart(DL_OVERSHOOT_STATS, internalCellIndex);
            lteMacUpdateStatsPerfStart(UL_OVERSHOOT_STATS, internalCellIndex);
#elif DL_UL_SPLIT_TDD
            lteMacUpdateStatsPerfStart(DL_OVERSHOOT_STATS);
#else
            lteMacUpdateStatsPerfStart(internalCellIndex);
            /* CA Stats Changes End */
#endif
            if(1 == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellState)
            {
                if(first)
                {
                    if(sysFrameNum * 10+subFrameNum != (psfnsf+1)%10240)

                    {
                        /*
                        // Shouldn't come here
                        */
                    }

                }
                else
                    first=1;

                psfnsf=sysFrameNum * 10+subFrameNum;
            }
#endif
            /* Received Data from MSQ or Socket Interface */
            while (1)
            {
#ifndef LAYER2_PHYSIM_THREAD
                if (zeroBufCpy_p)
                {
                    msgFree(zeroBufCpy_p);
                }
                zeroBufCpy_p = msgAlloc(PNULL,0,0,0);
                msgReserve(zeroBufCpy_p,0,BUFFLEN);

                if (PNULL ==  msgSegNext(zeroBufCpy_p, 0, &segment))
                {
                    lteWarning("Error In msgSegNext\n");

                    LOG_MAC_MSG(L2_MSG_SEGNEXT_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                            currentTick,\
                            __LINE__,0,0,0,0,\
                            0.0,0.0,__FILE__,__func__);

                    continue;
                }
                msg_p = segment.base;

                bytesRead = recvDataFromPhysical (msg_p, internalCellIndex);
#else
                if (zeroBufCpy_p)
                {
                    msgFree(zeroBufCpy_p);
                }
                bytesRead = recvDataFromPhySim (&zeroBufCpy_p);
#endif
                /* Message buffer length should be greater than 10(size of API
                 * Header)*/
                if (bytesRead > L1_API_HEADER_LEN )
                {
#ifdef LAYER2_PHYSIM_THREAD
                    if (PNULL ==  msgSegNext(zeroBufCpy_p, 0, &segment))
                    {
                        lteWarning("Error In msgSegNext\n");

                        LOG_MAC_MSG(L2_MSG_SEGNEXT_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                                currentTick,\
                                __LINE__,0,0,0,0,\
                                0.0,0.0,__FILE__,__func__);

                    }
                    msg_p=segment.base;
#endif
                    messageBuffer_p = (UInt8 *)msg_p;
                    pL1Api = (FAPI_l1ApiMsg_st *)messageBuffer_p;
                    msgId = pL1Api->msgId;
#ifndef DL_UL_SPLIT_TDD
                    msgLen = MAC_PHY_CONVERT_16(pL1Api->msgLen);
                    LTE_MAC_UT_LOG(LOG_DETAIL,RECV_INTF,"msgId = %d,msgLen = %d\n",
                            msgId, msgLen);
#endif
                    switch ( msgId )
                    {
                        case PHY_UL_SUBFRAME_INDICATION:
                        {
                            pSubFrameInd = (FAPI_subFrameIndication_st *)
                                &pL1Api->msgBody[0];
                            sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF(
                                    pSubFrameInd->sfnsf);
                            subFrameNum = GET_SUBFRAME_FROM_SFNSF(pSubFrameInd->sfnsf);

#ifdef OTA_DL_ADVANCE_2
                            /* OTA_DL_ADVANCE changes start */
#ifdef TDD_CONFIG
                            GET_REQUIRED_TICK(sysFrameNum ,subFrameNum, 1);
#endif
                            /* OTA_DL_ADVANCE changes end */
#endif
                            /* FAPI L2 AS MASTER START */
#ifdef FAPI_L2_MASTER
                            /*
                             ** Update the Tick, system frame and subframe Number.
                             **
                             ** This function will be invoked by ISR once system
                             ** has been configured to send tick to L2.
                             */
                            if (startExecutionAfterPhyStart_g[internalCellIndex] != TRUE)
                            {
                                processTickReceivedOnL2();

                                /* Validate the subframe and sysframe number */
                                validateSFIFrmL1(sysFrameNum, subFrameNum,internalCellIndex);
                            }
#else

                            processSubFrameIndicationForTickMiss(sysFrameNum,subFrameNum,internalCellIndex);
                            tickReceived=1;
#ifdef DL_UL_SPLIT
                            lteMacUpdateStatsPerfStart(DL_OVERSHOOT_STATS,internalCellIndex);
                            lteMacUpdateStatsPerfStart(UL_OVERSHOOT_STATS,internalCellIndex);
#elif DL_UL_SPLIT_TDD
                            lteMacUpdateStatsPerfStart(DL_OVERSHOOT_STATS);
#else
                            lteMacUpdateStatsPerfStart(internalCellIndex);
#endif
                            /* RDTSC_AND_CLOCK_START_TDD;*/
#endif
                            /* FAPI L2 AS MASTER END*/
                            break;
                        }
                        case PHY_UL_HARQ_INDICATION:
                        {
                            pHarqMsg = (FAPI_harqIndication_st *)&pL1Api->msgBody[0];
                            sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( pHarqMsg->sfnsf);
                            subFrameNum = GET_SUBFRAME_FROM_SFNSF(pHarqMsg->sfnsf);
                            /* validate the system Frame/ SubFrameNum and also fill the
                               tick of recieved message buffer*/
                            if (MAC_FAILURE == validatePHYDelayAndTick(sysFrameNum,
                                        subFrameNum, &recvTTI, msgId, 
                                        internalCellIndex))
                            {
                                lteWarning("Packet Dropped due to delayed Recvd "
                                        "(msgId =%x)\n",msgId);
                                LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                        currentTick,\
                                        PHY_UL_HARQ_INDICATION,sysFrameNum,subFrameNum,\
                                        getSysFrameNumer(internalCellIndex),getSubFrameNumber(internalCellIndex),\
                                        0.0,0.0,__func__,"HARQ_INV_SFNSF");

                                break;
                            }
                            /*CA Changes start  */       
                            (*processHARQIndicationFunction)(pHarqMsg, recvTTI, sysFrameNum, subFrameNum, internalCellIndex); 
                            /*CA Changes end  */       
                            break;
                        }
#ifndef DL_UL_SPLIT_TDD  /*In DL UL SPLIT handled in differnt function for UL Thread*/
                        case PHY_UL_CRC_INDICATION:
                        {
                            {
                                /* SPR 21660_2 changes end */
                                crcMsg_p = (FAPI_crcIndication_st *)&pL1Api->msgBody[0];
                                sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( crcMsg_p->sfnsf);
                                subFrameNum = GET_SUBFRAME_FROM_SFNSF(crcMsg_p->sfnsf);
                                /* validate the system Frame/ SubFrameNum and also fill the
                                   tick of recieved message buffer*/
                                if (MAC_FAILURE == validatePHYDelayAndTick(sysFrameNum,
                                            subFrameNum, &recvTTI, msgId, 
                                            internalCellIndex))
                                {
                                    lteWarning("Packet Dropped due to delayed Recvd "
                                            "(msgId =%x)\n",msgId);
                                    LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                            currentTick,\
                                            PHY_UL_CRC_INDICATION,sysFrameNum,subFrameNum,\
                                            getSysFrameNumer(internalCellIndex),getSubFrameNumber(internalCellIndex),\
                                            0.0,0.0,__func__,"CRC_INV_SFNSF");

                                    break;
                                }
                                /* Now process all the CRC pdus from the CRC.Indication
                                 * message */

                                /* msg3_retx_tdd_support_start */
                                /*CA Changes start  */
                                processULCrcInd(crcMsg_p, recvTTI, currentTick,subFrameNum,sysFrameNum, internalCellIndex);
                                /*CA Changes end  */
                                /* msg3_retx_tdd_support_end */
                                /* SPR 21660_2 changes start */
                            }
                            /* SPR 21660_2 changes end */
                            break;
                        }
                        case PHY_ERROR_INDICATION:
                        {
                            errMsg_p = (FAPI_phyErrorIndication_st *)&pL1Api->msgBody[0];
                            /*CA Changes start  */
                            processULErrInd(errMsg_p, internalCellIndex);
                            /*CA Changes start  */
                            break;
                        }
                        case PHY_UL_RX_ULSCH_INDICATION:
                        {
                            ulSchMsg_p = (FAPI_rxULSCHIndication_st *)&pL1Api->msgBody[0];
                            sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( ulSchMsg_p->sfnsf);
                            subFrameNum = GET_SUBFRAME_FROM_SFNSF(ulSchMsg_p->sfnsf);
                            /* validate the system Frame/ SubFrameNum and also fill the
                               tick of recieved message buffer*/
                            if (MAC_FAILURE == validatePHYDelayAndTick(sysFrameNum,
                                        subFrameNum, &recvTTI, msgId, 
                                        internalCellIndex))
                            {
                                lteWarning("Packet Dropped due to delayed Recvd "
                                        "(msgId =%x)\n",msgId);
                                LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                        currentTick,\
                                        PHY_UL_RX_ULSCH_INDICATION,sysFrameNum,subFrameNum,\
                                        getSysFrameNumer(internalCellIndex),getSubFrameNumber(internalCellIndex),\
                                        0.0,0.0,__func__,"ULSCH_INV_SFNSF");
                                break;
                            }
                            /* + TM7_8 Changes Start */
                            if(pL1Api->lenVendorSpecific != 0)
                            {
                                vendorSpecificData_p = (FAPI_VendorSpecific_st *) (pL1Api->msgBody + msgLen);
                            }

                            (*processULSchIndFunction)(vendorSpecificData_p,ulSchMsg_p, zeroBufCpy_p, msgLen, recvTTI, 
                                    /* Time Averaging changes start */
                                    0 /*withoutDemux*/, dlDelay, threadIndex,
                                    /*CA Changes start  */
                                    internalCellIndex

);
                            break;
                        }
                        case PHY_UL_RACH_INDICATION:
                        {
                            rachMsg_p = (FAPI_rachIndication_st *)&pL1Api->msgBody[0];
                            sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( rachMsg_p->sfnsf);
                            subFrameNum = GET_SUBFRAME_FROM_SFNSF(rachMsg_p->sfnsf);
                            /* validate the system Frame/ SubFrameNum and also fill the
                               tick of recieved message buffer*/
                            if (MAC_FAILURE == validatePHYDelayAndTick(sysFrameNum,
                                        subFrameNum, &recvTTI, msgId, 
                                        internalCellIndex))
                            {
                                lteWarning("Packet Dropped due to delayed Recvd "
                                        "(msgId =%x)\n",msgId);
                                LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                        currentTick,\
                                        PHY_UL_RACH_INDICATION,sysFrameNum,subFrameNum,\
                                        getSysFrameNumer(internalCellIndex),getSubFrameNumber(internalCellIndex),\
                                        0.0,0.0,__func__,"RACH_INV_SFNSF");

                                break;
                            }

                            /* + SPR_7503 */
                            if(pL1Api->lenVendorSpecific != 0)
                            {
                                vendorSpecificRach_p = (FAPI_VendorSpecificForRACHInd_st *)((UInt8 *)(pL1Api->msgBody )+ msgLen);
                            }
                            /*CA Changes start  */
                            (*processRACHIndicationFunction)(rachMsg_p, recvTTI,vendorSpecificRach_p, internalCellIndex);
                            /*CA Changes end  */
                            /* - SPR_7503 */
                            break;
                        }
                        case PHY_UL_RX_SR_INDICATION:
                        {
                            srMsg_p = (FAPI_rxSRIndication_st *)&pL1Api->msgBody[0];
                            sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( srMsg_p->sfnsf);
                            subFrameNum = GET_SUBFRAME_FROM_SFNSF(srMsg_p->sfnsf);
                            /* validate the system Frame/ SubFrameNum and also fill the
                               tick of recieved message buffer*/
                            if (MAC_FAILURE == validatePHYDelayAndTick(sysFrameNum,
                                        subFrameNum, &recvTTI, msgId,
                                        internalCellIndex))
                            {
                                lteWarning("Packet Dropped due to delayed Recvd "
                                        "(msgId =%x)\n",msgId);
                                LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                        currentTick,\
                                        PHY_UL_RX_SR_INDICATION,sysFrameNum,subFrameNum,\
                                        getSysFrameNumer(internalCellIndex),getSubFrameNumber(internalCellIndex),\
                                        0.0,0.0,__func__,"SR_INV_SFNSF");

                                break;
                            }
                            /* Rel9_upgrade_CR410 */
                            /*CA Changes start  */
                            processSRIndication(srMsg_p, recvTTI,dlDelay, internalCellIndex);
                            /*CA Changes end  */
                            /* Rel9_upgrade_CR410 */

                            break;
                        }
#endif /*End of DL_UL_SPLIT_TDD*/

                        case PHY_UL_RX_CQI_INDICATION:
                        {
                            cqiMsg_p = (FAPI_rxCqiIndication_st*)&pL1Api->msgBody[0];
                            sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( cqiMsg_p->sfnsf);
                            subFrameNum = GET_SUBFRAME_FROM_SFNSF(cqiMsg_p->sfnsf);
                            /* validate the system Frame/ SubFrameNum and also fill the
                               tick of recieved message buffer*/
                            if(MAC_FAILURE == validatePHYDelayAndTick(sysFrameNum,
                                        subFrameNum, &recvTTI, msgId, 
                                        internalCellIndex))
                            {
                                lteWarning("Packet Dropped due to delayed Recvd "
                                        "(msgId =%x)\n",msgId);
                                LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                        currentTick,\
                                        PHY_UL_RX_CQI_INDICATION,sysFrameNum,subFrameNum,\
                                        getSysFrameNumer(internalCellIndex),getSubFrameNumber(internalCellIndex),\
                                        0.0,0.0,__func__,"CQI_INV_SFNSF");

                                break;
                            }
                            /* + TM7_8 Changes Start */
                            if(pL1Api->lenVendorSpecific != 0)
                            {
                                vendorSpecificData_p = (FAPI_VendorSpecific_st *) (pL1Api->msgBody + msgLen);
                            }

                            (*processCQIIndicationFunction)(vendorSpecificData_p,cqiMsg_p, recvTTI,
                                    /*SPR 5620 START*/ 
#ifdef TDD_CONFIG
                                    sysFrameNum,
#endif
                                    /*SPR 5620 END*/ 
                                    subFrameNum,
                                    /*CA Changes start  */
                                    internalCellIndex
                                    /*CA Changes start  */ 
                                    );
                            /* - TM7_8 Changes End */
                            break;
                        }
#ifndef DL_UL_SPLIT_TDD
                        case PHY_UL_SRS_INDICATION:
                        {
                            srsMsg_p = (FAPI_srsIndication_st *)&pL1Api->msgBody[0];
                            sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( srsMsg_p->sfnsf);
                            subFrameNum = GET_SUBFRAME_FROM_SFNSF(srsMsg_p->sfnsf);
                            /* validate the system Frame/ SubFrameNum and also fill the
                               tick of recieved message buffer*/
                            if (MAC_FAILURE == validatePHYDelayAndTick(sysFrameNum,
                                        subFrameNum, &recvTTI, msgId, 
                                        internalCellIndex))
                            {
                                lteWarning("Packet Dropped due to delayed Recvd "
                                        "(msgId =%x)\n",msgId);
                                LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                        currentTick,\
                                        PHY_UL_SRS_INDICATION,sysFrameNum,subFrameNum,\
                                        getSysFrameNumer(internalCellIndex),getSubFrameNumber(internalCellIndex),\
                                        0.0,0.0,__func__,"CQI_INV_SFNSF");

                                break;
                            }
                            /* + TM7_8 Changes Start */
                            if(pL1Api->lenVendorSpecific != 0)
                            {
                                vendorSpecificData_p = (FAPI_VendorSpecific_st *) (pL1Api->msgBody + msgLen);
                            }
                            /*CA Changes start  */
                            (*processSRSIndicationFunction)(vendorSpecificData_p,srsMsg_p,recvTTI, internalCellIndex);
                            /*CA Changes end  */
                            /* - TM7_8 Changes End */
                            break;
                        }
#endif /*End of DL_UL_SPLIT_TDD*/
                        /* +DYNAMIC_ICIC */  
                        case PHY_MEASUREMENT_REPORT_INDICATION:
                        {
                            measReportInfo_p = (FAPI_measReportInfo_st*)&pL1Api->msgBody[0];
                            measReportInd_p = (FAPI_measReportInd_st*)&(measReportInfo_p->measReportInfo); 
                            sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( measReportInfo_p->sfnsf);
                            subFrameNum = GET_SUBFRAME_FROM_SFNSF(measReportInfo_p->sfnsf);

                            /* validate the system Frame/ SubFrameNum and also fill the
                               tick of recieved message buffer*/
                            if (MAC_FAILURE == validatePHYDelayAndTick(sysFrameNum,
                                        subFrameNum, &recvTTI, msgId,
                                        internalCellIndex))
                            {
                                lteWarning("Packet Dropped due to delayed Recvd "
                                        "(msgId =%x)\n",msgId);
                                //Log msg
                                break;
                            }
                            /*CA Changes start  */
                            processMeasReportIndication(measReportInd_p, internalCellIndex);
                            /*CA Changes start  */
                            break;
                        }
                        /* -DYNAMIC_ICIC */  
                        default:
                        lteWarning("2....Invalid API ID Received = %x\n",msgId);

                        LOG_MAC_MSG(MAC_PHY_INV_MSG_ID,LOGWARNING,MAC_L1_INF,\
                                currentTick,\
                                msgId,0,0,0,0,\
                                0.0,0.0,__func__,"INV_MSG_ID");

                        break;
                    }
                }
                else
                {
                    if (bytesRead != 0)
                    {
                        lteWarning("Invalid Message size %d received.", bytesRead);
                        LOG_MAC_MSG(MAC_PHY_INV_MSG_LEN_ID,LOGWARNING,MAC_L1_INF,\
                                currentTick,\
                                bytesRead,0,0,0,0,\
                                0.0,0.0,__func__,"INV_MSG_LEN");

                    }
                    break;
                }
#ifdef TDD_CONFIG
                if (TRUE == phyReconfigOngoing_g)
                {
                    /* Check if any data on phyControlSockFD_g */
                    UInt8   msgBuf[FAPI_CTRL_MSG_MAX_BUF_SZ] = {0};
                    Int bytesRead = 0;
                    if( 0 != (bytesRead = recvControlInfoFromPhysical ((void *)msgBuf)))
                    {
                        processControlInfoFromPhysical (msgBuf,bytesRead);
                    }

                }
#endif
#ifdef MAC_AUT_TEST
                /* :ONLY FOR CUnit TESTING: In case of DL_UL_SPLIT, where instead of reading 
                   the messages from queue, here the messages are received directly from socket. 
                   So to break the while(1) and while ((currentTick == getCurrentTick())) below 
                   assigments are done. */
                            tickReceived = 1;
                            currentTick = currentTick + 2;
#endif
                            /* FAPI L2 AS MASTER START */
#ifndef FAPI_L2_MASTER
                            if(tickReceived) 
                            {
                                /* HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                                /*Update dlDelay value which will be used for updation of sched maps of HD FDD UEs*/
                                if(hdUeContextInfo_g.numHdUe > 0)
                                {
                                    tick=getCurrentTick() + PHY_DL_DELAY;
                                    updateHDUeSubFrameCntxtInfo(tick);
                                }

#endif
                                /* HD FDD Changes End */
#ifdef DL_UL_SPLIT
                                putEntryInAckNackQueue(INVALID_UE_ID, 255 , 
                                        subFrameNum, FAPI_OTHER_CRNTI
                                        /* 32.425 Events code changes start*/
                                        /* +- SPR 17777 */
                                        /* 32.425 Events code changes end*/
                                        /*CA Changes start  */
                                        ,internalCellIndex
                                        ,HARQ_MODE_1A
                                        /*CA Changes end  */
                                        );
#endif
                                break;
                            }

#endif

            } /* While(1) */
            msgFree(zeroBufCpy_p);
            zeroBufCpy_p = PNULL;
#ifdef DL_UL_SPLIT
            /* Put the Delimeter node entry in ackNackQueue_g queue so that the DL Thread moves onto next(i.e.) EL. */
            /* To create the DUMMY  Just make the operationType as 255(i.e Invalid) to distinguish it for the normal nodes. */
            // USED FOR ICC so Its a Wrapper
#endif
            /* FAPI AS L2 MASTER START */
#ifndef FAPI_L2_MASTER
        }/*until subframeIndication Changed.*/

#ifdef FDD_CONFIG
        while (0);
#endif
#endif
                /* FAPI AS L2 MASTER END */
#ifndef DL_UL_SPLIT_TDD
                resetHandleVal ( internalCellIndex);
#endif
                LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Exit. ",__func__);
}

#endif


#ifdef DL_UL_SPLIT_TDD
/*****************************************************************************
 * Function Name  : processRxSubFrameIndUl 
 * Inputs         : dlDelay - Downlink Delay
 threadIndex - thread ID
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the uplink  messages received
 and Sub Frame Indication Received from PHY
 *****************************************************************************/
void processRxSubFrameIndUl( UInt32 dlDelay,
        UInt32 threadIndex
        )
{
    void *zeroBufCpy_p     = PNULL;
    QSEGMENT segment;
    UInt8 tickReceived=0;
    UInt16 sysFrameNum   = 0;
    UInt8  subFrameNum   = 0;
    /* SPR 15909 fix start */
    tickType_t recvTTI       = 0;
    /* SPR 15909 fix end */
    UInt16 msgId         = 0;
    UInt16 msgLen        = 0;
    SInt32 bytesRead     = 0;
    UInt8  *messageBuffer_p      = PNULL;
    UInt8  *msg_p        = PNULL;
    /* SPR 15909 fix start */
    tickType_t currentTick = getCurrentTickUl();
    /* SPR 15909 fix end */
    FAPI_l1ApiMsg_st    *pL1Api = PNULL;
    FAPI_subFrameIndication_st  *pSubFrameInd = PNULL;
    FAPI_crcIndication_st       *crcMsg_p = PNULL; 
    FAPI_phyErrorIndication_st  *errMsg_p = PNULL; 
    FAPI_rxULSCHIndication_st   *ulSchMsg_p = PNULL; 
    FAPI_rachIndication_st      *rachMsg_p = PNULL;
    FAPI_rxSRIndication_st      *srMsg_p = PNULL;
    FAPI_srsIndication_st       *srsMsg_p = PNULL;
    /* + TM7_8 Changes Start */
    FAPI_VendorSpecific_st *vendorSpecificData_p = PNULL;
    /* - TM7_8 Changes End */
    /* + E_CID_5.3 */
    FAPI_VendorSpecificForRACHInd_st *vendorSpecificRach_p = PNULL;
    /* - E_CID_5.3 */

    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Entry. ",__func__);
    /* Receive the data from the Physical Layer Until the subframeIndicaiton
       recvd or numOfMsg read as configured*/

    while ((currentTick == getCurrentTickUl()))
    {
        waitOnSelectPhyMsg(phyRxSockUlFD_g);
#ifdef PHYSIM_DEBUG

        sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF(sfnsf_g);
        subFrameNum = GET_SUBFRAME_FROM_SFNSF(sfnsf_g);
        lLteMacStatsRfSfn_g = sysFrameNum;
        lLteMacStatsRfSf_g = subFrameNum;

        lteMacUpdateStatsPerfStart(UL_OVERSHOOT_STATS);

        if(1==phyStarted_g)
        {
            if(first)
            {
                if(sysFrameNum * 10+subFrameNum != (psfnsf+1)%10240)

                {
                    // Debug traces 
                }

            }
            else
                first=1;

            psfnsf=sysFrameNum * 10+subFrameNum;
        }
#endif
        /* Received Data from MSQ or Socket Interface */
        while (1)
        {
#ifndef LAYER2_PHYSIM_THREAD
            if (zeroBufCpy_p)
            {
                msgFree(zeroBufCpy_p);
            }
            zeroBufCpy_p = msgAlloc(PNULL,0,0,0);
            msgReserve(zeroBufCpy_p,0,BUFFLEN);

            if (PNULL ==  msgSegNext(zeroBufCpy_p, 0, &segment))
            {
                lteWarning("Error In msgSegNext\n");

                LOG_MAC_MSG(L2_MSG_SEGNEXT_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                        currentTick,\
                        __LINE__,0,0,0,0,\
                        0.0,0.0,__FILE__,__func__);

                continue;
            }
            msg_p = segment.base;
#endif

#ifndef LAYER2_PHYSIM_THREAD
            bytesRead = recvUlDataFromPhysical(msg_p);
#else
            if (zeroBufCpy_p)
            {
                msgFree(zeroBufCpy_p);
            }
            bytesRead = recvUlDataFromPhysical(&zeroBufCpy_p);
#endif
            /* Message buffer length should be greater than 10(size of API
             * Header)*/
            if (bytesRead > L1_API_HEADER_LEN )
            {
#ifdef LAYER2_PHYSIM_THREAD
                if (PNULL ==  msgSegNext(zeroBufCpy_p, 0, &segment))
                {
                    ltePanic("Error In msgSegNext:%s \n",__func__);

                    LOG_MAC_MSG(L2_MSG_SEGNEXT_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                            currentTick,\
                            __LINE__,0,0,0,0,\
                            0.0,0.0,__FILE__,__func__);

                }
                msg_p=segment.base;
#endif
                messageBuffer_p = (UInt8 *)msg_p;
                pL1Api = (FAPI_l1ApiMsg_st *)messageBuffer_p;
                msgId = pL1Api->msgId;
                msgLen = MAC_PHY_CONVERT_16(pL1Api->msgLen);
                LTE_MAC_UT_LOG(LOG_DETAIL,RECV_INTF,"msgId = %d,msgLen = %d\n",
                        msgId, msgLen);
                switch ( msgId )
                {
                    case PHY_UL_SUBFRAME_INDICATION:
                    {
                        pSubFrameInd = (FAPI_subFrameIndication_st *)
                            &pL1Api->msgBody[0];
                        sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF(
                                pSubFrameInd->sfnsf);
                        subFrameNum = GET_SUBFRAME_FROM_SFNSF(pSubFrameInd->sfnsf);

#ifdef OTA_DL_ADVANCE_2
                        /* OTA_DL_ADVANCE changes start */
#ifdef TDD_CONFIG
                        GET_REQUIRED_TICK(sysFrameNum ,subFrameNum, 1);
#endif
                        /* OTA_DL_ADVANCE changes end */
#endif

                        processSubFrameIndUl(sysFrameNum,subFrameNum);

                        tickReceived=1;
                        lteMacUpdateStatsPerfStart(UL_OVERSHOOT_STATS);
                        /* RDTSC_AND_CLOCK_START_TDD;*/
                        break;
                    }
                    case PHY_UL_CRC_INDICATION:
                    {
                        crcMsg_p = (FAPI_crcIndication_st *)&pL1Api->msgBody[0];
                        sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( crcMsg_p->sfnsf);
                        subFrameNum = GET_SUBFRAME_FROM_SFNSF(crcMsg_p->sfnsf);
                        /* validate the system Frame/ SubFrameNum and also fill the
                           tick of recieved message buffer*/
                        if (MAC_FAILURE == validatePHYDelayAndTickUl(sysFrameNum,
                                    subFrameNum, &recvTTI))
                        {

                            lteWarning("Packet Dropped due to delayed Recvd "
                                    "(msgId =%x)\n",msgId);
                            LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                    currentTick,\
                                    PHY_UL_CRC_INDICATION,sysFrameNum,subFrameNum,\
                                    getSysFrameNumer(),getSubFrameNumber(),\
                                    0.0,0.0,__func__,"CRC_INV_SFNSF");

                            break;
                        }
                        /* Now process all the CRC pdus from the CRC.Indication
                         * message */
                        processULCrcInd(crcMsg_p, recvTTI, currentTick,subFrameNum,sysFrameNum);
                        break;
                    }
                    case PHY_ERROR_INDICATION:
                    {
                        errMsg_p = (FAPI_phyErrorIndication_st *)&pL1Api->msgBody[0];
                        processULErrInd(errMsg_p);
                        break;
                    }
                    case PHY_UL_RX_ULSCH_INDICATION:
                    {
                        ulSchMsg_p = (FAPI_rxULSCHIndication_st *)&pL1Api->msgBody[0];
                        sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( ulSchMsg_p->sfnsf);
                        subFrameNum = GET_SUBFRAME_FROM_SFNSF(ulSchMsg_p->sfnsf);
                        /* validate the system Frame/ SubFrameNum and also fill the
                           tick of recieved message buffer*/
                        if (MAC_FAILURE == validatePHYDelayAndTickUl(sysFrameNum,
                                    subFrameNum, &recvTTI))
                        {
                            lteWarning("Packet Dropped due to delayed Recvd "
                                    "(msgId =%x)\n",msgId);
                            LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                    currentTick,\
                                    PHY_UL_RX_ULSCH_INDICATION,sysFrameNum,subFrameNum,\
                                    getSysFrameNumer(),getSubFrameNumber(),\
                                    0.0,0.0,__func__,"ULSCH_INV_SFNSF");
                            break;
                        }
                        /* + TM7_8 Changes Start */
                        if(pL1Api->lenVendorSpecific != 0)
                        {
                            vendorSpecificData_p = (FAPI_VendorSpecific_st *) (pL1Api->msgBody + msgLen);
                        }

                        processULSchInd(vendorSpecificData_p,ulSchMsg_p, zeroBufCpy_p, msgLen, recvTTI, 
                                /* Time Averaging changes start */
                                0 /*withoutDemux*/, dlDelay, threadIndex);
                        /* Time Averaging changes end */
                        /* - TM7_8 Changes End */
                        break;
                    }
                    case PHY_UL_RACH_INDICATION:
                    {
                        rachMsg_p = (FAPI_rachIndication_st *)&pL1Api->msgBody[0];
                        sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( rachMsg_p->sfnsf);
                        subFrameNum = GET_SUBFRAME_FROM_SFNSF(rachMsg_p->sfnsf);
                        /* validate the system Frame/ SubFrameNum and also fill the
                           tick of recieved message buffer*/
                        if (MAC_FAILURE == validatePHYDelayAndTickUl(sysFrameNum,
                                    subFrameNum, &recvTTI))
                        {

                            lteWarning("Packet Dropped due to delayed Recvd "
                                    "(msgId =%x)\n",msgId);
                            LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                    currentTick,\
                                    PHY_UL_RACH_INDICATION,sysFrameNum,subFrameNum,\
                                    getSysFrameNumer(),getSubFrameNumber(),\
                                    0.0,0.0,__func__,"RACH_INV_SFNSF");

                            break;
                        }
                        if(pL1Api->lenVendorSpecific != 0)
                        {
                            vendorSpecificRach_p = (FAPI_VendorSpecificForRACHInd_st *)((UInt8 *)(pL1Api->msgBody )+ msgLen);
                        }
                        processRACHIndication(rachMsg_p, recvTTI, vendorSpecificRach_p);
                        break;
                    }
                    case PHY_UL_RX_SR_INDICATION:
                    {
                        srMsg_p = (FAPI_rxSRIndication_st *)&pL1Api->msgBody[0];
                        sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( srMsg_p->sfnsf);
                        subFrameNum = GET_SUBFRAME_FROM_SFNSF(srMsg_p->sfnsf);
                        /* validate the system Frame/ SubFrameNum and also fill the
                           tick of recieved message buffer*/
                        if (MAC_FAILURE == validatePHYDelayAndTickUl(sysFrameNum,
                                    subFrameNum, &recvTTI))
                        {

                            lteWarning("Packet Dropped due to delayed Recvd "
                                    "(msgId =%x)\n",msgId);
                            LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                    currentTick,\
                                    PHY_UL_RX_SR_INDICATION,sysFrameNum,subFrameNum,\
                                    getSysFrameNumer(),getSubFrameNumber(),\
                                    0.0,0.0,__func__,"SR_INV_SFNSF");

                            break;
                        }
                        /* Rel9_upgrade_CR410 */
                        processSRIndication(srMsg_p, recvTTI,dlDelay);
                        /* Rel9_upgrade_CR410 */
                        break;
                    }
                    case PHY_UL_SRS_INDICATION:
                    {
                        srsMsg_p = (FAPI_srsIndication_st *)&pL1Api->msgBody[0];
                        sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( srsMsg_p->sfnsf);
                        subFrameNum = GET_SUBFRAME_FROM_SFNSF(srsMsg_p->sfnsf);
                        /* validate the system Frame/ SubFrameNum and also fill the
                           tick of recieved message buffer*/
                        if (MAC_FAILURE == validatePHYDelayAndTickUl(sysFrameNum,
                                    subFrameNum, &recvTTI))
                        {

                            lteWarning("Packet Dropped due to delayed Recvd "
                                    "(msgId =%x)\n",msgId);
                            LOG_MAC_MSG(MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
                                    currentTick,\
                                    PHY_UL_SRS_INDICATION,sysFrameNum,subFrameNum,\
                                    getSysFrameNumer(),getSubFrameNumber(),\
                                    0.0,0.0,__func__,"CQI_INV_SFNSF");

                            break;
                        }
                        /* + TM7_8 Changes Start */
                        if(pL1Api->lenVendorSpecific != 0)
                        {
                            vendorSpecificData_p = (FAPI_VendorSpecific_st *) (pL1Api->msgBody + msgLen);
                        }
                        processSRSIndication(vendorSpecificData_p,srsMsg_p,recvTTI);
                        /* - TM7_8 Changes End */
                        break;
                    }
                    default:
                    lteWarning("2....Invalid API ID Received = %x\n",msgId);

                    LOG_MAC_MSG(MAC_PHY_INV_MSG_ID,LOGWARNING,MAC_L1_INF,\
                            currentTick,\
                            msgId,0,0,0,0,\
                            0.0,0.0,__func__,"INV_MSG_ID");

                    break;
                }
            }
            else
            {
                if (bytesRead != 0)
                {
                    lteWarning("Invalid Message size %d received.", bytesRead);
                    LOG_MAC_MSG(MAC_PHY_INV_MSG_LEN_ID,LOGWARNING,MAC_L1_INF,\
                            currentTick,\
                            bytesRead,0,0,0,0,\
                            0.0,0.0,__func__,"INV_MSG_LEN");

                }
                break;
            }
#ifdef MAC_AUT_TEST
            /* :ONLY FOR CUnit TESTING: In case of DL_UL_SPLIT, where instead of reading 
               the messages from queue, here the messages are received directly from socket. 
               So to break the while(1) and while ((currentTick == getCurrentTick())) below 
               assigments are done. */
                        tickReceived = 1;
                        currentTick = currentTick + 2;
#endif
                        if(tickReceived) 
                        {
                            break;
                        }
        } /* While(1) */
        msgFree(zeroBufCpy_p);
        zeroBufCpy_p = PNULL;
    }/*until subframeIndication Changed.*/

    resetHandleVal ();
    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Exit. ",__func__);
}


/*****************************************************************************
 * Function Name  : processRxFirstSubFrameIndUl 
 * Inputs         : void
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the first uplink Sub Frame Indication 
 *                  Received from PHY after Cell Setup
 *****************************************************************************/

                                void processRxFirstSubFrameIndUl(void)
{
    void *zeroBufCpy_p     = PNULL;
    QSEGMENT segment;
    UInt8 tickReceived=0;
    UInt16 sysFrameNum   = 0;
    UInt8  subFrameNum   = 0;
    UInt16 msgId         = 0;
    SInt32 bytesRead     = 0;
    UInt8  *messageBuffer_p      = PNULL;
    UInt8  *msg_p        = PNULL;
    /* SPR 15909 fix start */
    tickType_t currentTick = getCurrentTickUl();
    /* SPR 15909 fix end */
    FAPI_l1ApiMsg_st    *pL1Api = PNULL;
    FAPI_subFrameIndication_st  *pSubFrameInd = PNULL;

    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Entry. ",__func__);
    /* Receive the data from the Physical Layer Until the first subframeIndicaiton
       recvd or numOfMsg read as configured*/

    while ((currentTick == getCurrentTickUl()))
    {
        waitOnSelectPhyMsg(phyRxSockUlFD_g);
        /* Received Data from MSQ or Socket Interface */
        while (1)
        {
            if (zeroBufCpy_p)
            {
                msgFree(zeroBufCpy_p);
            }
            zeroBufCpy_p = msgAlloc(PNULL,0,0,0);
            msgReserve(zeroBufCpy_p,0,BUFFLEN);

            if (PNULL ==  msgSegNext(zeroBufCpy_p, 0, &segment))
            {
                lteWarning("Error In msgSegNext\n");

                LOG_MAC_MSG(L2_MSG_SEGNEXT_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                        currentTick,\
                        __LINE__,0,0,0,0,\
                        0.0,0.0,__FILE__,__func__);
                continue;
            }
            msg_p = segment.base;

            bytesRead = recvUlDataFromPhysical(msg_p);
            /* Message buffer length should be greater than 10(size of API
             * Header)*/
            if (bytesRead > L1_API_HEADER_LEN )
            {
                messageBuffer_p = (UInt8 *)msg_p;
                pL1Api = (FAPI_l1ApiMsg_st *)messageBuffer_p;
                msgId = pL1Api->msgId;
                LTE_MAC_UT_LOG(LOG_DETAIL,RECV_INTF,"msgId = %d,msgLen = %d\n",
                        msgId, MAC_PHY_CONVERT_16(pL1Api->msgLen));
                switch ( msgId )
                {
                    case PHY_UL_SUBFRAME_INDICATION:
                    {
                        pSubFrameInd = (FAPI_subFrameIndication_st *)
                            &pL1Api->msgBody[0];
                        sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF(
                                pSubFrameInd->sfnsf);
                        subFrameNum = GET_SUBFRAME_FROM_SFNSF(pSubFrameInd->sfnsf);

#ifdef OTA_DL_ADVANCE_2
                        /* OTA_DL_ADVANCE changes start */
#ifdef TDD_CONFIG
                        GET_REQUIRED_TICK(sysFrameNum ,subFrameNum, 1);
#endif
                        /* OTA_DL_ADVANCE changes end */
#endif

                        processFirstSubFrameIndUl(sysFrameNum,subFrameNum);

                        tickReceived=1;
                        break;
                    }
                    default:
                    lteWarning("2....Invalid API ID Received = %x\n",msgId);

                    LOG_MAC_MSG(MAC_PHY_INV_MSG_ID,LOGWARNING,MAC_L1_INF,\
                            currentTick,\
                            msgId,0,0,0,0,\
                            0.0,0.0,__func__,"INV_MSG_ID");

                    break;
                }
            }
            else
            {
                if (bytesRead != 0)
                {
                    lteWarning("Invalid Message size %d received.", bytesRead);
                    LOG_MAC_MSG(MAC_PHY_INV_MSG_LEN_ID,LOGWARNING,MAC_L1_INF,\
                            currentTick,\
                            bytesRead,0,0,0,0,\
                            0.0,0.0,__func__,"INV_MSG_LEN");

                }
                break;
            }
#ifdef MAC_AUT_TEST
            /* :ONLY FOR CUnit TESTING: In case of DL_UL_SPLIT, where instead of reading 
               the messages from queue, here the messages are received directly from socket. 
               So to break the while(1) and while ((currentTick == getCurrentTick())) below 
               assigments are done. */
                tickReceived = 1;
                currentTick = currentTick + 2;
#endif
                if(tickReceived) 
                {
                    break;
                }
        } /* While(1) */
        msgFree(zeroBufCpy_p);
        zeroBufCpy_p = PNULL;
    }/*until subframeIndication Changed.*/

    resetHandleVal ();
    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Exit. ",__func__);
}

#endif
#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : putEntryInAckNackQueue
 * Inputs         : ueIndex - index of UE,
 *                  operation - ACK or NACK,
 *                  recvTTI , rntiFlag,
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : Puts the AckNackNode in ackNackQueue.
 * Returns        : None
 * Description    : This function creates a new ackNackNode and inserts this
 *                  node in the ackNackQueue to be processed by worker thread.
 *****************************************************************************/
                        void putEntryInAckNackQueue( UInt32 ueIndex, 
                                /* CA_phase2_harq_code Start */
                                UInt16 operation,
                                /* CA_phase2_harq_code End */
                                /* SPR 15909 fix start */
                                tickType_t recvTTI,
                                /* SPR 15909 fix end */
                                UInt32 rntiFlag
                                /* 32.425 Events code changes start*/
                                /* +- SPR 17777 */
#ifndef DL_UL_SPLIT
#ifdef KPI_STATS
                                /* SPR 15909 fix start */
                                ,tickType_t rcvTTI
                                /* SPR 15909 fix end */
#endif
#endif
                                /* +- SPR 17777 */
                                /* 32.425 Events code changes end*/
                                /*CA Changes start  */
                                ,InternalCellIndex  internalCellIndex
                        /* + SPR_15971 */
                        ,UInt8 mode
                        /* - SPR_15971 */
                        /*CA Changes end  */

                        )
{
    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] ueIndex [%d] Ack/Nack[%d] RecvTTI [%d] Entry. ",
            __func__, ueIndex, operation, recvTTI);

#ifndef DL_UL_SPLIT
    AckNackQueueNode *ackNackEntry_p = PNULL;
    GET_MEM_FROM_POOL(AckNackQueueNode,ackNackEntry_p,sizeof(AckNackQueueNode), PNULL);

    /* Rel 5.3: Coverity 24455 Fix Start */
    if (PNULL != ackNackEntry_p)
    {
        ackNackEntry_p->operationType = operation;
        ackNackEntry_p->ueIndex = ueIndex;
        ackNackEntry_p->packetSubFrame = recvTTI;
        /* + SPR_15971 */
        ackNackEntry_p->mode = mode;
        /* - SPR_15971 */
        /* 32.425 Events code changes start*/
#ifdef KPI_STATS
        ackNackEntry_p->recvTTI = rcvTTI;
#endif
        /* 32.425 Events code changes end*/
        /* SPS_CHG */

        if(FAPI_SPS_CRNTI ==  rntiFlag)
        {
            ackNackEntry_p->schUsed = SPS_SCHEDULING; 
        }
        else
        {
            ackNackEntry_p->schUsed = DRA_SCHEDULING; 
        }

        /* SPS_CHG */ 
        /*CA Changes start  */
        /*coverity fix  25432*/
        if(pushNode((ackNackQueue_gp[internalCellIndex]),
                    &(ackNackEntry_p->ackNackAnchor)))
        {
            freeMemPool(ackNackEntry_p);
            return; 
        }
        /*coverity fix 25432*/
        LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] No. of DL Ack/Nack Nodes = %d .Exit. ",
                __func__, sQueueCount(ackNackQueue_gp[internalCellIndex]));
        /*CA Changes end  */

        LOG_UT(MAC_HI_RX_INS_ID,LOGDEBUG,MAC_L1_INF,\
                getCurrentTick(),\
                ueIndex,\
                operation,\
                recvTTI,0,0,\
                0.0,0.0,__func__,"HI_RX_INS");
    }
    else
    {
        /* Not able to allocate Memory */
    }
    /* Rel 5.3: Coverity Fix End */

#else
    (void)mode;/*SPR 18490 +-*/
    /*SPR 6169 Fix Start */
    UInt32 schUsed = DRA_SCHEDULING;

    if(FAPI_SPS_CRNTI ==  rntiFlag)
    {
        schUsed = SPS_SCHEDULING; 
    }
    else
    {
        schUsed = DRA_SCHEDULING;
    }

    /*SPR 6169 Fix End */

    putEntryInAckNackQueueUsingICC(ueIndex, operation, recvTTI
            /*SPR 6169 Fix Start */
            ,schUsed
            /*SPR 6169 Fix End */
            /*CA Changes start  */
            ,internalCellIndex
            /*CA Changes end  */
            );
#endif
    return;
}

/* + SPR 17733 */
#ifdef FDD_CONFIG
#if !defined DL_UL_SPLIT 
/*****************************************************************************
 * Function Name  : putEntryInAckNackQueueForTcrnti
 * Inputs         : ueIndex - index of UE,
 *                  operation - ACK or NACK,
 *                  recvTTI 
 * Outputs        : Puts the AckNackNode in ackNackQueue.
 * Returns        : None
 * Description    : This function creates a new ackNackNode and inserts this
 *                  node in the ackNackQueue to be processed by worker thread.
 *****************************************************************************/
void putEntryInAckNackQueueForTcrnti( UInt32 ueIndex, UInt32 operation,
        tickType_t recvTTI,InternalCellIndex internalCellIndex )
{
    AckNackQueueNode *ackNackEntry_p = PNULL;
    GET_MEM_FROM_POOL(AckNackQueueNode,ackNackEntry_p,sizeof(AckNackQueueNode), PNULL);

    if (PNULL != ackNackEntry_p)
    {
        ackNackEntry_p->operationType = operation;
        ackNackEntry_p->ueIndex = ueIndex;
        ackNackEntry_p->packetSubFrame = recvTTI;
        ackNackEntry_p->schUsed = DRA_SCHEDULING; 

        /* coverity fix 99564 */
        if(pushNode((ackNackQueueForTcrnti_gp[internalCellIndex]),
                    &(ackNackEntry_p->ackNackAnchor)))
        {
            freeMemPool(ackNackEntry_p);
            return;
        }

        LOG_UT(MAC_HI_RX_INS_ID,LOGDEBUG,MAC_L1_INF,\
                getCurrentTick(),\
                ueIndex,\
                operation,\
                recvTTI,0,0,\
                0.0,0.0,__func__,"HI_RX_INS");
    }
}
#endif
#endif
/* - SPR 17733 */
#ifdef DL_UL_SPLIT
/*****************************************************************************
 * Function Name  : putEntryInContResAckQueue
 * Inputs         : ueIndex - Ue Identifier,
 *                  currentTTI - Current tick, 
 *                  recvTTI - MEssage received tick,
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : Puts the ContentionResolution Node in contentionResolutionMsgAckQ_g
 * Returns        : None
 * Description    : This function creates a new ContentionResAckNode and inserts this
 *                  node in the contentionResolutionMsgAckQ_g to be processed by worker thread.
 ******************************************************************************/
/*CA Changes start  */
void putEntryInContResAckQueue( UInt32 ueIndex,
        /* SPR 15909 fix start */
        tickType_t currentTTI,
        tickType_t recvTTI,
        /* SPR 15909 fix end */ 
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    ContentionResAckNode *contentionAckQueueNode_p = PNULL;

    GET_MEM_FROM_POOL(ContentionResAckNode,contentionAckQueueNode_p,sizeof(ContentionResAckNode), PNULL);

    /* COVERITY CID 26409 Fix start */ 
    if (!contentionAckQueueNode_p)
    {
        LOG_MAC_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,\
                getCurrentTick(),\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);
        return;
    }
    /* COVERITY CID 26409 Fix end */ 

    contentionAckQueueNode_p->ueIndex = ueIndex;
    /*CA Changes start  */
    if (pushNode(contentionResolutionMsgAckQ_gp[internalCellIndex],
                /*CA Changes end  */
                &contentionAckQueueNode_p->nodeAnchor))
    {
        LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGFATAL,L2_SYS_FAIL,\
                getCurrentTick(),\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);

        freeMemPool(contentionAckQueueNode_p);
    }
    else
    {
        LOG_MAC_MSG(MAC_CONTENTION_ACK_RCVD_ID,LOGDEBUG,MAC_L1_INF,\
                currentTTI,\
                0,\
                ueIndex,\
                recvTTI,0,0,\
                0.0,0.0,__func__,"CONTENTION_ACK_RCVD");
    }

    return;
}
#endif

#endif

#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : tddPutEntryInAckNackQueueForConfig0
 * Inputs         : ueIndex - Index of UE,
 *                  operation - ACK or NACK,
 *                  recvTTI, sfn, sf,
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : Puts the AckNackNode in ackNackQueue.
 * Returns        : None
 * Description    : This function creates a new ackNackNode and inserts this
 *                  node in the ackNackQueue to be processed by worker thread.
 *****************************************************************************/
        void tddPutEntryInAckNackQueueForConfig0(
                UInt32 ueIndex,
                UInt32 operation,
                /* SPR 15909 fix start */
                tickType_t recvTTI,
                /* SPR 15909 fix end */
                UInt16 sfn,
                UInt16 sf,
                InternalCellIndex internalCellIndex,
                /* + CA_TDD_HARQ_CHANGES */
                UInt8 modeType
                /* - CA_TDD_HARQ_CHANGES */
                )
{
    /* TDD Config 0 Changes Start */
    LTE_MAC_UT_LOG(LOG_WARNING,RECV_INTF,
            "[%s] ueIndex [%d] Ack/Nack[%d]  sfn [%d] sf [%d] Entry. ",
            __func__, ueIndex, operation, sfn, sf);


    UInt8     containerIndex = 0;
    DLUEContext *dlUEContext_p = PNULL;
    UInt8 prevContainerIndex = 0;
    if( TRUE == dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
    {
        return;
    }
    dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

    containerIndex = MODULO_TWO(sfn);
    prevContainerIndex = MODULO_TWO(containerIndex + 1);
    switch(sf)
    {
        case SUB_FRAME_2:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex))
        {        

            populateAndPushAcknNackNode(SUB_FRAME_6,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
        }
        break;

        case SUB_FRAME_4:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex))
        {                        
            prevContainerIndex = containerIndex;
            populateAndPushAcknNackNode(SUB_FRAME_0,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
        }
        break;

        case SUB_FRAME_7:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex))
        {                        
            prevContainerIndex = containerIndex;
            populateAndPushAcknNackNode(SUB_FRAME_1,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
        }
        break;

        case SUB_FRAME_9:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex))
        {        
            prevContainerIndex = containerIndex;
            populateAndPushAcknNackNode(SUB_FRAME_5,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);

        }
        break;
    }

    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] No. of DL Ack/Nack Nodes = %d .Exit. ",
            __func__, sQueueCount(&(tddAckNackQueue_g[internalCellIndex])));
    /* TDD Config 0 Changes End */
    return;
}

/*****************************************************************************
 * Function Name  : tddPutEntryInAckNackQueueForConfig1
 * Inputs         : ueIndex - Index of UE,
 *                  operation - ACK or NACK,
 *                  recvTTI, sfn, sf,
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : Puts the AckNackNode in ackNackQueue.
 * Returns        : None
 * Description    : This function creates a new ackNackNode and inserts this
 *                  node in the ackNackQueue to be processed by worker thread.
 *****************************************************************************/
            void tddPutEntryInAckNackQueueForConfig1(
                    UInt32 ueIndex,
                    UInt32 operation,
                    /* SPR 15909 fix start */
                    tickType_t recvTTI,
                    /* SPR 15909 fix end */
                    UInt16 sfn,
                    UInt16 sf,
                    InternalCellIndex internalCellIndex,
                    /* + CA_TDD_HARQ_CHANGES */
                    UInt8 modeType)
            /* - CA_TDD_HARQ_CHANGES */
{
    UInt8     containerIndex = 0;
    DLUEContext *dlUEContext_p = PNULL;
    UInt8 prevContainerIndex = 0;
    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
    DLAckNackOperation opType[MAX_M_COUNT_FOR_CONFIG_ONE] = {{0,0}};
    /* SPR 19679: TDD HARQ Multiplexing Changes End */    
    LTE_MAC_UT_LOG(LOG_DETAIL,RECV_INTF,
            "[%s] ueIndex [%d] Ack/Nack[%d]  sfn [%d] sf [%d] Entry. ",
            __func__, ueIndex, operation, sfn, sf);

    /* SPR 6495 fix start */
    if( TRUE == dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
    {
        return;
    }
    /* SPR 6495 fix end */
    dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

    containerIndex = MODULO_TWO(sfn);
    prevContainerIndex = MODULO_TWO(containerIndex + 1);
    switch(sf)
    {
        /* Cyclomatic_complexity_changes_start */
        case SUB_FRAME_2:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex))
        {

            /* SPR 19679: TDD HARQ Multiplexing Changes Start */
            opType[0].dlSubFrameNum = SUB_FRAME_5;
            opType[1].dlSubFrameNum = SUB_FRAME_6;
            opType[0].containerIndex = prevContainerIndex;
            opType[1].containerIndex = prevContainerIndex;

            /* + CA_TDD_HARQ_CHANGES */
            CHECK_MOD_TYPE_AND_HARQ_STORE_OPERATION_WITH_M_TWO(modeType,
                    operation,opType,containerIndex,SUB_FRAME_2,dlUEContext_p); 
            /* SPR 19679: TDD HARQ Multiplexing Changes End */

            populateAndPushAcknNackNode(SUB_FRAME_5,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
                    opType[0].ackNackType,
                    /* SPR 19679: TDD HARQ Multiplexing  Changes End */
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);

            populateAndPushAcknNackNode(SUB_FRAME_6,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
                    opType[1].ackNackType,
                    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            /* - CA_TDD_HARQ_CHANGES */

        }
        break;

        case SUB_FRAME_3:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex))
        {
            populateAndPushAcknNackNode(SUB_FRAME_9,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);

        }
        break;

        case SUB_FRAME_7:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex))
        {
            /* SPR 19679: TDD HARQ Multiplexing Changes Start */
            opType[0].dlSubFrameNum = SUB_FRAME_0;
            opType[1].dlSubFrameNum = SUB_FRAME_1;
            opType[0].containerIndex = containerIndex;
            opType[1].containerIndex = containerIndex;

            /* + CA_TDD_HARQ_CHANGES */
            CHECK_MOD_TYPE_AND_HARQ_STORE_OPERATION_WITH_M_TWO(modeType,
                    operation,opType,containerIndex,SUB_FRAME_7,dlUEContext_p); 
            /* SPR 19679: TDD HARQ Multiplexing Changes End */
            prevContainerIndex = containerIndex;
            populateAndPushAcknNackNode(SUB_FRAME_0,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
                    opType[0].ackNackType,
                    /* SPR 19679: TDD HARQ Multiplexing Changes End */
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_1,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
                    opType[1].ackNackType,
                    /* SPR 19679: TDD HARQ Multiplexing Changes End */
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);

            /* - CA_TDD_HARQ_CHANGES */
        }

        break;

        case SUB_FRAME_8:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex
                    ))
        {
            prevContainerIndex = containerIndex;
            populateAndPushAcknNackNode(SUB_FRAME_4,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);

        }
        break;
    }
    /* Cyclomatic_complexity_changes_end */
    return;
}


/*****************************************************************************
 * Function Name  : tddPutEntryInAckNackQueueForConfig2
 * Inputs         : ueIndex - Index of UE,
 *                  operation - ACK or NACK,
 *                  recvTTI, sfn,sf,
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : Puts the AckNackNode in ackNackQueue.
 * Returns        : None
 * Description    : This function creates a new ackNackNode and inserts this
 *                  node in the ackNackQueue to be processed by worker thread.
 *****************************************************************************/
            void tddPutEntryInAckNackQueueForConfig2(
                    UInt32 ueIndex,
                    UInt32 operation,
                    /* SPR 15909 fix start */
                    tickType_t recvTTI,
                    /* SPR 15909 fix end */
                    UInt16 sfn,
                    UInt16 sf,
                    InternalCellIndex internalCellIndex,
                    /* + CA_TDD_HARQ_CHANGES */
                    UInt8 modeType
                    )
            /* - CA_TDD_HARQ_CHANGES */
{

    UInt8     containerIndex = 0;
    DLUEContext *dlUEContext_p = PNULL;
    UInt8 prevContainerIndex = 0;
    /* + CA_TDD_HARQ_CHANGES */
    DLAckNackOperation opType[MAX_M_COUNT_FOR_CONFIG_TWO] = {{0,0}};
    /* - CA_TDD_HARQ_CHANGES */
    /* SPR 6495 fix start */
    if( TRUE == dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
    {
        return;
    }
    /* SPR 6495 fix end */
    dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

    containerIndex = MODULO_TWO(sfn);
    prevContainerIndex = MODULO_TWO(containerIndex + 1);

    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
    opType[0].containerIndex = prevContainerIndex;
    opType[1].containerIndex = prevContainerIndex;
    opType[2].containerIndex = prevContainerIndex;
    opType[3].containerIndex = prevContainerIndex;
    /* SPR 19679: TDD HARQ Multiplexing Changes End */

    LTE_MAC_UT_LOG(LOG_WARNING,RECV_INTF,
            "[%s] ueIndex [%d] Ack/Nack[%d]  sfn [%d] sf [%d] Entry. ",
            __func__, ueIndex, operation, sfn, sf);

    switch(sf)
    {
        /* Cyclomatic_complexity_changes_start */
        case SUB_FRAME_7:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex
                    ))
        {
            /* + CA_TDD_HARQ_CHANGES */
            opType[0].dlSubFrameNum = SUB_FRAME_9;
            opType[1].dlSubFrameNum = SUB_FRAME_0;
            opType[2].dlSubFrameNum = SUB_FRAME_1;
            opType[3].dlSubFrameNum = SUB_FRAME_3;
            /* SPR 19679: TDD HARQ Multiplexing Changes Start */
            /* Updating conatiner index */
            opType[1].containerIndex = containerIndex;
            opType[2].containerIndex = containerIndex;
            opType[3].containerIndex = containerIndex;

            CHECK_MOD_TYPE_AND_HARQ_STORE_OPERATION_WITH_M_THREE_OR_FOUR(
                    modeType,operation,opType,dlUEContext_p,containerIndex,
                    SUB_FRAME_7,HARQ_MULTIPLEX_FOUR,UL_DL_CONFIG_2);
            /* SPR 19679: TDD HARQ Multiplexing Changes End */
            populateAndPushAcknNackNode(SUB_FRAME_9,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[0].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            prevContainerIndex = containerIndex;
            populateAndPushAcknNackNode(SUB_FRAME_0,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[1].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_1,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[2].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_3,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[3].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);

            /* - CA_TDD_HARQ_CHANGES */
        }
        break;

        case SUB_FRAME_2:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex
                    ))
        {
            /* + CA_TDD_HARQ_CHANGES */
            opType[0].dlSubFrameNum = SUB_FRAME_4;
            opType[1].dlSubFrameNum = SUB_FRAME_5;
            opType[2].dlSubFrameNum = SUB_FRAME_6;
            opType[3].dlSubFrameNum = SUB_FRAME_8;

            /* SPR 19679: TDD HARQ Multiplexing Changes Start */
            CHECK_MOD_TYPE_AND_HARQ_STORE_OPERATION_WITH_M_THREE_OR_FOUR(modeType, \
                    operation,opType,dlUEContext_p,containerIndex,
                    SUB_FRAME_2, HARQ_MULTIPLEX_FOUR,UL_DL_CONFIG_2);
            /* SPR 19679: TDD HARQ Multiplexing Changes End */

            populateAndPushAcknNackNode(SUB_FRAME_4,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[0].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_5,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[1].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_6,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[2].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_8,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[3].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            /* - CA_TDD_HARQ_CHANGES */
            /*Coverity 65725 Start*/
        }
        break;
    }
    /*Coverity 65725 End*/
    /* Cyclomatic_complexity_changes_end */
    /** CA-TDD Changes Start:06 **/
    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] No. of DL Ack/Nack Nodes = %d .Exit. ",
            __func__, sQueueCount(&(tddAckNackQueue_g[internalCellIndex])));
    /** CA-TDD Changes End:06 **/
    return;
}

/*****************************************************************************
 * Function Name  : tddPutEntryInAckNackQueueForConfig3
 * Inputs         : ueIndex - Index of UE,
 *                  operation - ACK or NACK,
 *                  recvTTI, sfn ,sf,
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : Puts the AckNackNode in ackNackQueue.
 * Returns        : None
 * Description    : This function creates a new ackNackNode and inserts this
 *                  node in the ackNackQueue to be processed by worker thread.
 *****************************************************************************/
            void tddPutEntryInAckNackQueueForConfig3(
                    UInt32 ueIndex,
                    UInt32 operation,
                    /* SPR 15909 fix start */
                    tickType_t recvTTI,
                    /* SPR 15909 fix end */
                    UInt16 sfn,
                    UInt16 sf,
                    InternalCellIndex internalCellIndex,
                    /* + CA_TDD_HARQ_CHANGES */
                    UInt8 modeType
                    /* - CA_TDD_HARQ_CHANGES */
                    )
{
    UInt8     containerIndex = 0;
    DLUEContext *dlUEContext_p = PNULL;
    UInt8 prevContainerIndex = 0;
    /* + CA_TDD_HARQ_CHANGES */
    DLAckNackOperation opType[MAX_M_COUNT_FOR_CONFIG_THREE] = {{0,0}};
    /* - CA_TDD_HARQ_CHANGES */
    LTE_MAC_UT_LOG(LOG_WARNING,RECV_INTF,
            "[%s] ueIndex [%d] Ack/Nack[%d]  sfn [%d] sf [%d] Entry. ",
            __func__, ueIndex, operation, sfn, sf);

    /* SPR 6495 fix start */
    if( TRUE == dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
    {
        return;
    }
    /* SPR 6495 fix end */

    dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    containerIndex = MODULO_TWO(sfn);
    prevContainerIndex = MODULO_TWO(containerIndex + 1);

    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
    opType[0].containerIndex = prevContainerIndex;
    opType[1].containerIndex = prevContainerIndex;
    opType[2].containerIndex = prevContainerIndex;
    /* SPR 19679: TDD HARQ Multiplexing Changes End */

    switch(sf)
    {
        /* Cyclomatic_complexity_changes_start */
        case SUB_FRAME_2:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex
                    ))
        {
            /* + CA_TDD_HARQ_CHANGES */
            opType[0].dlSubFrameNum = SUB_FRAME_1;
            opType[1].dlSubFrameNum = SUB_FRAME_5;
            opType[2].dlSubFrameNum = SUB_FRAME_6;

            /* SPR 19679: TDD HARQ Multiplexing Changes Start */
            CHECK_MOD_TYPE_AND_HARQ_STORE_OPERATION_WITH_M_THREE_OR_FOUR(
                    modeType,operation,opType,dlUEContext_p,containerIndex,
                    SUB_FRAME_2,HARQ_MULTIPLEX_THREE,UL_DL_CONFIG_3);
            /* SPR 19679: TDD HARQ Multiplexing Changes End */

            populateAndPushAcknNackNode(SUB_FRAME_1,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[0].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_5,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[1].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_6,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[2].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);

            /* - CA_TDD_HARQ_CHANGES */
        }

        break;

        case SUB_FRAME_3:

        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex
                    ))
        {
            /* SPR 19679: TDD HARQ Multiplexing Changes Start */
            opType[0].dlSubFrameNum = SUB_FRAME_7;
            opType[1].dlSubFrameNum = SUB_FRAME_8;

            /* + CA_TDD_HARQ_CHANGES */
            CHECK_MOD_TYPE_AND_HARQ_STORE_OPERATION_WITH_M_TWO(modeType,
                    operation,opType,containerIndex,SUB_FRAME_3,dlUEContext_p); 
            /* SPR 19679: TDD HARQ Multiplexing Changes End */
            populateAndPushAcknNackNode(SUB_FRAME_7,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[0].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_8,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[1].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            /* - CA_TDD_HARQ_CHANGES */

        }
        break;

        case SUB_FRAME_4:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex))
        {                        
            /* SPR 19679: TDD HARQ Multiplexing Changes Start */
            opType[0].dlSubFrameNum = SUB_FRAME_9;
            opType[1].dlSubFrameNum = SUB_FRAME_0;
            /* Updating container value */
            opType[1].containerIndex = containerIndex;

            /* + CA_TDD_HARQ_CHANGES */
            CHECK_MOD_TYPE_AND_HARQ_STORE_OPERATION_WITH_M_TWO(modeType,
                    operation,opType,containerIndex,SUB_FRAME_4, dlUEContext_p); 
            /* SPR 19679: TDD HARQ Multiplexing Changes End */

            populateAndPushAcknNackNode(SUB_FRAME_9,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[0].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            prevContainerIndex = containerIndex;

            populateAndPushAcknNackNode(SUB_FRAME_0,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[1].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);

            /* - CA_TDD_HARQ_CHANGES */
        }
        break;
    }
    /* Cyclomatic_complexity_changes_end */
    /** CA-TDD Changes Start:06 **/
    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] No. of DL Ack/Nack Nodes = %d .Exit. ",
            __func__, sQueueCount(&(tddAckNackQueue_g[internalCellIndex])));
    /** CA-TDD Changes End:06 **/
    return;
}



/*****************************************************************************
 * Function Name  : tddPutEntryInAckNackQueueForConfig4
 * Inputs         : ueIndex - Index of UE,
 *                  operation - ACK or NACK,
 *                  recvTTI, sfn, sf,
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : Puts the AckNackNode in ackNackQueue.
 * Returns        : None
 * Description    : This function creates a new ackNackNode and inserts this
 *                  node in the ackNackQueue to be processed by worker thread.
 *****************************************************************************/
                    void tddPutEntryInAckNackQueueForConfig4(
                            UInt32 ueIndex,
                            UInt32 operation,
                            /* SPR 15909 fix start */
                            tickType_t recvTTI,
                            /* SPR 15909 fix end */
                            UInt16 sfn,
                            UInt16 sf,
                            InternalCellIndex internalCellIndex,
                            /* + CA_TDD_HARQ_CHANGES */
                            UInt8 modeType
                            /* - CA_TDD_HARQ_CHANGES */
                            )
{

    UInt8     containerIndex = 0;
    DLUEContext *dlUEContext_p = PNULL;
    UInt8 prevContainerIndex = 0;
    /* + CA_TDD_HARQ_CHANGES */
    DLAckNackOperation opType[MAX_M_COUNT_FOR_CONFIG_FOUR] = {{0,0}};
    /* - CA_TDD_HARQ_CHANGES */
    LTE_MAC_UT_LOG(LOG_WARNING,RECV_INTF,
            "[%s] ueIndex [%d] Ack/Nack[%d]  sfn [%d] sf [%d] Entry. ",
            __func__, ueIndex, operation, sfn, sf);

    /* SPR 6495 fix start */
    if( TRUE == dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
    {
        return;
    }
    /* SPR 6495 fix end */
    dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

    containerIndex = MODULO_TWO(sfn);
    prevContainerIndex = MODULO_TWO(containerIndex + 1 );

    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
    opType[0].containerIndex = prevContainerIndex;
    opType[1].containerIndex = prevContainerIndex;
    opType[2].containerIndex = prevContainerIndex;
    opType[3].containerIndex = prevContainerIndex;
    /* SPR 19679: TDD HARQ Multiplexing Changes End */

    switch(sf)
    {
        /* Cyclomatic_complexity_changes_start */
        case SUB_FRAME_2:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex))
        {

            /* + CA_TDD_HARQ_CHANGES */
            opType[0].dlSubFrameNum = SUB_FRAME_0;
            opType[1].dlSubFrameNum = SUB_FRAME_1;
            opType[2].dlSubFrameNum = SUB_FRAME_4;
            opType[3].dlSubFrameNum = SUB_FRAME_5;

            /* SPR 19679: TDD HARQ Multiplexing Changes Start */
            CHECK_MOD_TYPE_AND_HARQ_STORE_OPERATION_WITH_M_THREE_OR_FOUR(
                    modeType,operation,opType,dlUEContext_p,containerIndex,
                    SUB_FRAME_2,HARQ_MULTIPLEX_FOUR,UL_DL_CONFIG_4);
            /* SPR 19679: TDD HARQ Multiplexing Changes End */

            populateAndPushAcknNackNode(SUB_FRAME_0,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[0].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_1,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[1].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_4,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[2].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_5,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[3].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            /* - CA_TDD_HARQ_CHANGES */

        }
        break;

        case SUB_FRAME_3:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex))
        {  

            /* + CA_TDD_HARQ_CHANGES */
            opType[0].dlSubFrameNum = SUB_FRAME_6;
            opType[1].dlSubFrameNum = SUB_FRAME_7;
            opType[2].dlSubFrameNum = SUB_FRAME_8;
            opType[3].dlSubFrameNum = SUB_FRAME_9;

            /* SPR 19679: TDD HARQ Multiplexing Changes Start */
            CHECK_MOD_TYPE_AND_HARQ_STORE_OPERATION_WITH_M_THREE_OR_FOUR(
                    modeType,operation,opType,dlUEContext_p,containerIndex,
                    SUB_FRAME_3, HARQ_MULTIPLEX_FOUR,UL_DL_CONFIG_4);
            /* SPR 19679: TDD HARQ Multiplexing Changes End */

            populateAndPushAcknNackNode(SUB_FRAME_6,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[0].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_7,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[1].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_8,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[2].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_9,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    opType[3].ackNackType,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            /* - CA_TDD_HARQ_CHANGES */

        }
        break;
    }
    /* Cyclomatic_complexity_changes_end */

    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] No. of DL Ack/Nack Nodes = %d .Exit. ",
            __func__, sQueueCount(&(tddAckNackQueue_g[internalCellIndex])));
    return;
}

/*****************************************************************************
 * Function Name  : tddPutEntryInAckNackQueueForConfig5
 * Inputs         : ueIndex - Index of UE,
 *                  operation - ACK or NACK,
 *                  recvTTI, sfn, sf,
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : Puts the AckNackNode in ackNackQueue.
 * Returns        : None
 * Description    : This function creates a new ackNackNode and inserts this
 *                  node in the ackNackQueue to be processed by worker thread.
 *****************************************************************************/
        void tddPutEntryInAckNackQueueForConfig5(
                UInt32 ueIndex,
                UInt32 operation,
                /* SPR 15909 fix start */
                tickType_t recvTTI,
                /* SPR 15909 fix end */
                UInt16 sfn,
                UInt16 sf,
                InternalCellIndex internalCellIndex,
                /* + CA_TDD_HARQ_CHANGES */
                UInt8 modeType
                /* - CA_TDD_HARQ_CHANGES */
                )
{
    UInt8     containerIndex = 0;
    DLUEContext *dlUEContext_p = PNULL;
    UInt8 prevContainerIndex = 0;

    LTE_MAC_UT_LOG(LOG_WARNING,RECV_INTF,
            "[%s] ueIndex [%d] Ack/Nack[%d]  sfn [%d] sf [%d] Entry. ",
            __func__, ueIndex, operation, sfn, sf);
    /* SPR 6495 fix start */
    if( TRUE == dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
    {
        return;
    }
    /* SPR 6495 fix end */
    dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

    containerIndex = MODULO_TWO(sfn);
    prevContainerIndex = MODULO_TWO(containerIndex + 1);
    switch(sf)
    {
        /* Cyclomatic_complexity_changes_start */
        case SUB_FRAME_2:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex
                    ))
        {
            populateAndPushAcknNackNode(SUB_FRAME_0,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_1,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_3,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_4,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_5,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_6,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);

            populateAndPushAcknNackNode(SUB_FRAME_7,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            populateAndPushAcknNackNode(SUB_FRAME_8,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
            prevContainerIndex = containerIndex;
            populateAndPushAcknNackNode(SUB_FRAME_9,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);

        }
        break;
    }
    /* Cyclomatic_complexity_changes_end */
    return;
}

/*****************************************************************************
 * Function Name  : tddPutEntryInAckNackQueueForConfig6
 * Inputs         : ueIndex - Index of UE,
 *                  operation - ACK or NACK,
 *                  recvTTI, sfn, sf,
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : Puts the AckNackNode in ackNackQueue.
 * Returns        : None
 * Description    : This function creates a new ackNackNode and inserts this
 *                  node in the ackNackQueue to be processed by worker thread.
 *****************************************************************************/
                    void tddPutEntryInAckNackQueueForConfig6(
                            UInt32 ueIndex,
                            UInt32 operation,
                            /* SPR 15909 fix start */
                            tickType_t recvTTI,
                            /* SPR 15909 fix end */
                            UInt16 sfn,
                            UInt16 sf,
                            InternalCellIndex internalCellIndex,
                            /* + CA_TDD_HARQ_CHANGES */
                            UInt8 modeType
                            /* - CA_TDD_HARQ_CHANGES */
                            )
{
    LTE_MAC_UT_LOG(LOG_WARNING,RECV_INTF,
            "[%s] ueIndex [%d] Ack/Nack[%d]  sfn [%d] sf [%d] Entry. ",
            __func__, ueIndex, operation, sfn, sf);

    UInt8     containerIndex = 0;
    DLUEContext *dlUEContext_p = PNULL;
    UInt8 prevContainerIndex = 0;
    /* SPR 6495 fix start */
    if( TRUE == dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
    {
        return;
    }
    /* SPR 6495 fix end */
    dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

    containerIndex = MODULO_TWO(sfn);
    prevContainerIndex = MODULO_TWO(containerIndex + 1);
    switch(sf)
    {
        /* Cyclomatic_complexity_changes_start */
        case SUB_FRAME_2:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex))
        {
            populateAndPushAcknNackNode(SUB_FRAME_5,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);

        }
        break;


        case SUB_FRAME_3:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex
                    ))
        {                        
            populateAndPushAcknNackNode(SUB_FRAME_6,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);


        }
        break;

        case SUB_FRAME_4:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex))
        {                        
            populateAndPushAcknNackNode(SUB_FRAME_9,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);

        }
        break;

        case SUB_FRAME_7:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex))
        {                        
            prevContainerIndex = containerIndex;
            populateAndPushAcknNackNode(SUB_FRAME_0,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);

        }
        break;

        case SUB_FRAME_8:
        if (MAC_SUCCESS == deleteEntryFromTimerList(
                    dlUEContext_p,
                    containerIndex,
                    sf,
                    internalCellIndex))
        {        
            prevContainerIndex = containerIndex;
            populateAndPushAcknNackNode(SUB_FRAME_1,
                    prevContainerIndex,
                    containerIndex,
                    ueIndex,
                    operation,
                    recvTTI,
                    sf,
                    internalCellIndex,
                    modeType);
        }
        break;
    }
    /* Cyclomatic_complexity_changes_end */
    /** CA-TDD Changes Start:06 **/
    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] No. of DL Ack/Nack Nodes = %d .Exit. ",
            __func__, sQueueCount(&(tddAckNackQueue_g[internalCellIndex])));
    /** CA-TDD Changes End:06 **/
    return;
}
#endif

/*****************************************************************************
 * Function Name  : crcACKNACkHandling
 * Inputs         : crcAckNack - Its represent the Uplink Ack/Nack
 *                  ueIndex  - Indx of UE,
 *                  recvTTI -  Recieved tick of the message buffer of CRC Ack/Nack,
 *                  ulContainerIndex , rntiType,
 *                  internalCellIndex - Cell_index at MAC
 *                  isForcedAckDueToMaxRetx
 * Outputs        : None 
 * Returns        : None
 * Description    : This function decode the Uplink Ack Nack and entry into 
 *                  the container queue.
 *****************************************************************************/
                    void crcAckNackHandling( UInt32 crcAckNack,
                            UInt32 ueIndex,
                            /* SPR 15909 fix start */
                            tickType_t recvTTI,
                            /* SPR 15909 fix end */
                            UInt32 ulContainerIndex,
                            UInt32 rntiType,
                            /*CA Changes start  */
                            InternalCellIndex internalCellIndex
                            /*CA Changes end  */ 
                            /* SPR 11509 Fix Start */
                            ,UInt8 isForcedAckDueToMaxRetx 
                            /* SPR 11509 Fix End */
                            )
{
    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] UeIndex [%d] CRC_Ack/Nack [%d] RecvTTI[%d] ulContainerIndex [%d] Entry. ",
            __func__,ueIndex, crcAckNack, recvTTI, ulContainerIndex);
    ULUEContextInfo* ulUEContextInfo_p = PNULL;
    /* Uplink Container used for Uplin ACK/Nack */
    UplinkContainer *currentContainer_p = PNULL;
    /*put the UE ID in NACK queue of the container */
    ContainerAckQueueNode *ackNode_p = PNULL;
    /*put the UE ID in NACK queue of the container */
    ContainerNackQueueNode *nackNode_p = PNULL;
    UInt8 ulHarqProcessId = 0;
    /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
    UInt8 dci0Index = 0;
#endif

    /* TDD Config 0 Changes End */
    /* SPS_CHG */
    UInt32 schUsed = DRA_SCHEDULING;
    /* SPS_CHG */
    /*SPR_3061_FIX*/
    DLUEContext *dlUEcontext_p = PNULL;
    /*SPR_3061_FIX*/
    /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
    UInt8 rbMapNum          = (recvTTI + getnextUlSfOffsetFromCurrentUlSf((recvTTI % MAX_SUB_FRAME),
                /* CA TDD CHANGES START */
                internalCellIndex
                /* CA TDD CHANGES END */
                )) % getMaxULRBMaps(internalCellIndex);
    RbMapNode *rbMapNode_p  = PNULL;
    UInt8 subframeNum  = 0;
    UInt16 sysFrameNum = 0; 
#endif
    /*TDD Config 0 Changes End*/
    ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
    ULUEContext *ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
    if (PNULL == ulUEContext_p)
    {
        return;
    }
#ifdef FDD_CONFIG
    /* TTIB_Code Start */
    ulHarqProcessId = ulUEContext_p->ttiHarqMap[MODULO_SIXTEEN(recvTTI)];
    /* + coverity 42165 */
    if (MAX_HARQ_PROCESS_NB <= ulHarqProcessId)
        /* - coverity 42165 */
    {
        return;
    }

    if(ulUEContext_p->ulresInfo[ulHarqProcessId].isTTIBundled)
    {
        UInt8 isTTIDataRcvd = (ulUEContext_p->ulresInfo[ulHarqProcessId].isTTIDataRcvd);

        LTE_MAC_UT_LOG(LOG_DETAIL,RECV_INTF,"[%s]:[%d] ulHarqProcessId [%d] ttiBundleFirstTick [%d] isTTIDataRcvd [%d]\n",
                ,__func__,__LINE__,ulHarqProcessId,(ulUEContext_p->ulresInfo[ulHarqProcessId].ttiBundleFirstTick),isTTIDataRcvd);

        //CODE_COMMENT_A3_FIXED
        /*
         * If data is recieved (crcInd Ack)
         * in any first 3 ticks of bundle 
         * then Ack can be sent for whole Bundle
         */

        if( crcAckNack && isTTIDataRcvd  )
        {
            crcAckNack = 0; 
        }

    }

    /* TTIB_Code End */
#endif

    /*It represents the current container*/
    /*CA Changes start  */
    currentContainer_p = ulContainerPool_gp[internalCellIndex] + ulContainerIndex;
    /*CA Changes end  */
#ifdef LOG_PRINT_ENABLED    
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */
#endif     

    /* SPS_CHG */
    LP_MacULSpsStatus macUlSpsStatus_p = PNULL;

    /* SPS_CHG */
    if(FAPI_SPS_CRNTI ==  rntiType)
    {
        schUsed = SPS_SCHEDULING; 
    }
    else
    {
        schUsed = DRA_SCHEDULING; 
    }
    /* SPS_CHG */ 

    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    if((TRUE == ulUEContext_p->isHDFddFlag) &&  /* if Half Duplex UE */
            (1 != crcAckNack) &&  /* If CRC ack is received */
            (TRUE == ulUEContext_p->ulresInfo[ulHarqProcessId].hdFddUlNackHandlingFlag))
    {
        /* Mark the flag as false and treat this ack as nack because the PHICH
         * resources will not be listened by UE. So retransmission to be performed */
        LOG_MAC_MSG(MAC_HD_FDD_CHANGE_CRC_ACK_TO_NACK, LOGWARNING, MAC_L1_INF,\
                getCurrentTick(), \
                ueIndex,ulHarqProcessId,\
                ulUEContext_p->ulresInfo[ulHarqProcessId].hdFddUlNackHandlingFlag,\
                crcAckNack,ulContainerIndex,recvTTI, \
                DEFAULT_FLOAT_VALUE, \
                FUNCTION_NAME,"CHANGE_CRC_ACK_TO_NACK");

        crcAckNack = 1;
    }
    /* Currently no handling for crcAckNack = 2 is done as it is not set and defined L1 FAPI */
#endif
    /*HD FDD Changes End*/

    /*( 0 - Correct Data )*/
    if ( crcAckNack == 0 )
    {

#ifdef PERF_STATS
        /* SPR 11509 Fix Start */
        if (isForcedAckDueToMaxRetx != LTE_TRUE)
        {
            UPDATE_UE_HARQ_UL_SUCC_FAIL_MODSCHEME_STATS(ueIndex , crcAckNack,
                    ulUEContext_p->modulationScheme);
        }
        /* SPR 11509 Fix End */
#ifdef FDD_CONFIG         
        UPDATE_RADIOTHP_RES_UL_STATS(ueIndex, ulHarqProcessId, recvTTI%MAX_SUB_FRAME,internalCellIndex);
#endif
#endif

#ifdef TDD_CONFIG
        /* SPR 2437 Start */
        /* SPR 1864 Start */
        /* Unused variable removed */
        /* SPR 1864 End */
        ulHarqProcessId = resetHarqAndUpdateDeltaBsr( recvTTI,
                ulUEContext_p, 
                internalCellIndex
                );

        /* SPR 9608 changes start */
#ifdef PERF_STATS
        UPDATE_RADIOTHP_RES_UL_STATS(ueIndex, ulHarqProcessId, recvTTI%MAX_SUB_FRAME, internalCellIndex);
#endif
        /* SPR 9608 changes end */

        /* + coverity 42165 */
        if (MAX_HARQ_PROCESS_NB <= ulHarqProcessId)
            /* - coverity 42165 */
        {
            return;
        }
        /* SPR 2437 End */
#elif FDD_CONFIG
        if(!ulUEContextInfo_p->pendingDeleteFlag)
        {
            ulUEContext_p->
                ulresInfo[ulHarqProcessId].deltabsrLoad = 0; 
        }
        /* SPR 6690 fix Start */
        if (HARQ_PROCESS_FREE == ulUEContext_p->ulresInfo[ulHarqProcessId].harqStatus)
        {
            return;
        }
        /* SPR 6690 fix End */
#endif              
        /* + SPS_TDD_Changes */       
        /* SPS_CHG */
#ifdef TDD_CONFIG
        if( (SPS_SCHEDULING != schUsed ) /* && (FALSE == checkSpsActTti(ulUEContext_p, ulHarqProcessId))*/ )
#endif
            /* - SPS_TDD_Changes */
        {
            /** SPR 12364 fix Starts **/
            /* Taking local backup of all neccesory parameters which will be 
             * used later when it receive ulSchInd for this CRC Ack */
            harqParamBackupForUlSchInd( &ulUEContext_p->ulresInfo[
                    ulHarqProcessId]);
            /** SPR 12364 fix Ends **/
            ulUEContext_p->ulresInfo[ulHarqProcessId].harqStatus\
                = HARQ_PROCESS_FREE;   /* TX_RETX */
            /*TTI Bundling Code Start*/
#ifdef FDD_CONFIG

            if(TRUE == ulUEContext_p->ulresInfo[ulHarqProcessId].isTTIBundled)
            {
                UInt8 counter = 0;
                UInt8 harqMapId = 0;
                harqMapId = ulUEContext_p->ulresInfo[ulHarqProcessId].ttiBundleFirstTick;
                for(counter = 0 ; counter < TTI_BUNDLE_SIZE ; counter++)
                {
                    ulUEContext_p->ttiHarqMap[MODULO_SIXTEEN(harqMapId+counter)] = TTI_MAP_FREE;
                }
            }
            else
            {
                ulUEContext_p->ttiHarqMap[ulHarqProcessId + MAC_DEFAULT_NUM_HARQ_PROCESSES] = TTI_MAP_FREE;
                ulUEContext_p->ttiHarqMap[ulHarqProcessId] = TTI_MAP_FREE;
            }
            /*++SPR 19156 */
            /** SPR 12364 fix Start **/
            ulUEContext_p->ttiPUSCHHarq[MODULO_SIXTEEN(recvTTI)] = 
                ulHarqProcessId;
            /** SPR 12364 fix Ends **/
            /*--SPR 19156 */
#endif              
        }
        /* CA Stats Changes Start */
        LTE_MAC_UL_PER_UE_DEBUG_STATS(ueIndex,getCurrentTick() , ulHarqProcessId,schUsed, crcAckNack,\
                ulUEContext_p->ulresInfo[ulHarqProcessId].currentRetransCount,
                internalCellIndex);
        /* CA Stats Changes End */

        GET_MEM_FROM_POOL(ContainerAckQueueNode,ackNode_p,sizeof(ContainerAckQueueNode), PNULL); 

        /* Rel 5.3: Coverity 24433 Fix Start */
        if (PNULL == ackNode_p)
        {
            /* Not able to allocate Memory */
            return;
        }
        /* Rel 5.3: Coverity Fix End */

        ackNode_p->ueIdx = ueIndex;
        ackNode_p->ttiCounter = recvTTI;
        ackNode_p->crcValid  = 1;
        /* SPS_CHG */
        ackNode_p->schUsed = schUsed;
        /* SPS_CHG */
        /* SPR HI_FIX START */
        ackNode_p->tcRntiFlag = FALSE;
        /* SPR HI_FIX END */

        /*bug_10441_changes start*/
        /* SPR 11509 Fix Start */
        if (LTE_TRUE == isForcedAckDueToMaxRetx)
        {
            ackNode_p->isLastRetx = TRUE;
        }
        else
        {
            ackNode_p->isLastRetx = FALSE;
            /* SPR 20964 fix start */
            dlUEcontext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
            dlUEcontext_p->lastSyncDetectedTick = getCurrentTick();
            /* SPR 20964 fix end */
        }
        /* SPR 11509 Fix End */
        /*bug_10441_changes end*/

        ULHarqInfo* tempULHarqInfo_p = 
            &(ulUEContext_p->ulresInfo[ulHarqProcessId]);
        ackNode_p->ulHarqProcessId = ulHarqProcessId;
#ifdef TDD_CONFIG            
        ackNode_p->allocatedLength = tempULHarqInfo_p->riLength;
        ackNode_p->allocatedIndex = tempULHarqInfo_p->riStart;
#endif            
        /*SPR_3061_FIX*/
        /* SPR 20964 fix start */
        /* code removed */
        /* lastSyncDetectedTick is already updated in else part of
         * isForcedAckDueToMaxRetx */
                /* SPR 20964 fix end */
                /*SPR_3061_FIX*/
                /* Put the nackNode in nackQueue of containerPool */

                /* COVERITY CID 25414 Fix start */
                if( pushNode(&(currentContainer_p->ackQueue), 
                            &(ackNode_p->ackNodeAnchor)) )
                {
                    freeMemPool(ackNode_p);
                    return;
                }
                /* COVERITY CID 25414 Fix end */
#ifdef TDD_CONFIG            
                /*PHICH_SPR Fix 12758 start */
                ulUEContext_p->ulresInfo[ulHarqProcessId].forcedAckFlag = FORCED_ACK_RESET;
                /* SPR Fix 12758 end */
                /* CRC_ULSCH_IND SPR 8722 Fix Start */
                /* TDD Config 6 Changes Start */
                GET_SFN_SF_FROM_TTI(recvTTI, sysFrameNum, subframeNum)
                    tempULHarqInfo_p->crcUlschIndFlag |= CRC_IND_RCVD_FLAG;
                if(CRC_ULSCH_IND_RCVD_FLAG == tempULHarqInfo_p->crcUlschIndFlag) 
                {
                    resetHarqId( subframeNum,ulUEContext_p, internalCellIndex, sysFrameNum);
                }
                /* TDD Config 6 Changes End */
                /* CRC_ULSCH_IND SPR 8722 Fix End */
#endif            

                /* SPS_CHG */
                /* TTIB_Code Start */    
                /* + SPS_TDD_Changes */	
                if (/* (TRUE == checkSpsActTti(ulUEContext_p, ulHarqProcessId)) && */
                        /* - SPS_TDD_Changes */
#ifdef FDD_CONFIG
                        ( ((ulUEContext_p->ulresInfo[ulHarqProcessId].isTTIBundled)&&
                           ((TTI_BUNDLE_SIZE - 1) == tempULHarqInfo_p->currentRetransCount)) ||
                          (!(ulUEContext_p->ulresInfo[ulHarqProcessId].isTTIBundled)&&
                           (!tempULHarqInfo_p->currentRetransCount)) )&&

#else
                        (!tempULHarqInfo_p->currentRetransCount) &&
#endif
                        (SPS_SCHEDULING == ackNode_p->schUsed) )
                    /* TTIB_Code End */    
                {
                    GET_MEM_FROM_POOL(MacULSpsStatus, macUlSpsStatus_p,
                            sizeof(MacULSpsStatus), PNULL);
                    if( PNULL == macUlSpsStatus_p )
                    {
                        ltePanic("Memory isexhausted\n");
                        /* Klockwork warning Changes Start */
                        return ;
                        /* Klockwork warning Changes End */
                    }
                    macUlSpsStatus_p->ueIndex = ueIndex;
                    macUlSpsStatus_p->response = FIRST_ACK_NACK;
                    /*CA Changes start  */
                    /* + Coverity Id - 32583 */
                    if(!ENQUEUE_MAC_UL_SPS_Q(ulSpsStatusQ_gp[internalCellIndex],
                                macUlSpsStatus_p))
                    {
                        freeMemPool(macUlSpsStatus_p);
                        macUlSpsStatus_p = PNULL;	
                    }
                    /* - Coverity Id - 32583 */
                    /* CA Stats Changes Start */
                    /* Update Q Stats */
                    LTE_MAC_QUEUE_USED( UL_SPS_STATUS_Q,
                            QCOUNT_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex] ),
                            internalCellIndex);
                    /*CA Changes end  */
                    /* CA Stats Changes End */
                }
                /* SPS_CHG */
    }
    /* Case of Error */
    else if ( crcAckNack == 1)
    {
#ifdef PERF_STATS
        UPDATE_UE_HARQ_UL_SUCC_FAIL_MODSCHEME_STATS(ueIndex , crcAckNack,
                ulUEContext_p->modulationScheme);
        /* CA Stats Changes Start */
        LTE_MAC_QUEUE_USED( UL_SPS_STATUS_Q,
                QCOUNT_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex] ),
                internalCellIndex);
        /* CA Stats Changes End */

#ifdef FDD_CONFIG         
        UPDATE_RADIOTHP_RES_UL_STATS(ueIndex, ulHarqProcessId, recvTTI%MAX_SUB_FRAME,internalCellIndex);
#endif
#endif

#ifdef TDD_CONFIG        
        /* TDD Config 0 Changes Start */
        ulHarqProcessId = resetHarqAndUpdateDeltaBsr( recvTTI,
                ulUEContext_p, internalCellIndex);
        if (MAX_HARQ_PROCESS_NB <= ulHarqProcessId)
        {
            return;
        }
#elif FDD_CONFIG
        dci0Index = (recvTTI + FDD_HARQ_OFFSET) %
            NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;     
#endif            
        /* CA Stats Changes Start */

        /* SPR 9608 changes start */
#ifdef PERF_STATS
        UPDATE_RADIOTHP_RES_UL_STATS(ueIndex, ulHarqProcessId, recvTTI%MAX_SUB_FRAME, internalCellIndex);
#endif
        /* SPR 9608 changes end */

        /*bug_10441_changes start*/
        ULHarqInfo* tempULHarqInfo_p = 
            &(ulUEContext_p->ulresInfo[ulHarqProcessId]);
        if (tempULHarqInfo_p->currentRetransCount >= (ulUEContext_p->maxUlHARQTx -1))
        {
            GET_MEM_FROM_POOL(ContainerAckQueueNode,ackNode_p,sizeof(ContainerAckQueueNode), PNULL); 
            /* Coverity 24433 changes start */
            if(NULL == ackNode_p)
            {
                /* Not able to allocate Memory */
                return;
            }   
            /* Coverity 24433 changes end */
            ackNode_p->ueIdx = ueIndex;
            ackNode_p->ttiCounter = recvTTI;
            ackNode_p->crcValid  = 1;
            /* SPS_CHG */
            ackNode_p->schUsed = schUsed;
            /* SPS_CHG */
#ifdef FDD_CONFIG
            ackNode_p->tcRntiFlag = FALSE;
#endif
            /* + SPS_TDD_Changes */
            if( (SPS_SCHEDULING != schUsed ) /* && (FALSE == checkSpsActTti(ulUEContextInfo_p->ulUEContext_p, ulHarqProcessId))*/ )
            {
                ulUEContextInfo_p->ulUEContext_p->ulresInfo[ulHarqProcessId].harqStatus\
                    = HARQ_PROCESS_FREE;   /* TX_RETX */
            }
            ackNode_p->isLastRetx = TRUE;
            ackNode_p->ulHarqProcessId = ulHarqProcessId;
#ifdef TDD_CONFIG            
            ackNode_p->allocatedLength = tempULHarqInfo_p->riLength;
            ackNode_p->allocatedIndex = tempULHarqInfo_p->riStart;
#endif  
            /* Coverity_25414_Fix Start */
            if( pushNode(&(currentContainer_p->ackQueue), 
                        &(ackNode_p->ackNodeAnchor)))
            {
                freeMemPool(ackNode_p);
            }
            /* Coverity_25414_Fix End */
            return;
        }
        /*bug_10441_changes end*/
        LTE_MAC_UL_PER_UE_DEBUG_STATS(ueIndex,getCurrentTick() , ulHarqProcessId, schUsed,  crcAckNack,\
                ulUEContext_p->ulresInfo[ulHarqProcessId].currentRetransCount,
                internalCellIndex);
        /* CA Stats Changes End */
        GET_MEM_NODE_NACK_CIRC_Q(nackNode_p, ContainerNackQueueNode);

        /* Rel 5.3: Coverity 24434 Fix Start */
        if (PNULL == nackNode_p)
        {
            /* Not able to allocate Memory */
            return;
        }
        /* Rel 5.3: Coverity Fix End */

        nackNode_p->ueIdx = ueIndex;
        nackNode_p->ttiCounter = recvTTI;
        /* SPS_CHG */
#ifdef TDD_CONFIG
        nackNode_p->ulSubFrame = (recvTTI + getnextUlSfOffsetFromCurrentUlSf((recvTTI % MAX_SUB_FRAME ),
                    /* CA TDD CHANGES START */
                    internalCellIndex
                    /* CA TDD CHANGES END */
                    )) % MAX_SUB_FRAME;
#endif
        nackNode_p->schUsed = schUsed;
        nackNode_p->ulHarqProcessId = ulHarqProcessId;
        /* SPS_CHG */
#ifndef DL_UL_SPLIT
        /* SPS_CHG */
        if ( ( (TRUE == checkSpsActTti(ulUEContext_p, ulHarqProcessId)) ||
                    (TRUE == checkSpsOccasionTti(ulUEContext_p, ulHarqProcessId)) ) &&
                (SPS_SCHEDULING == nackNode_p->schUsed) )

        {
            /* TTIB_Code Start */    
#ifdef FDD_CONFIG
            if(ulUEContext_p->ulresInfo[ulHarqProcessId].isTTIBundled)
            {
                /* Coverity 26386 Fix Start */
                if(!ENQUEUE_NACK_QUEUE_FOR_TTIB_UE_Q(&(currentContainer_p->nackQueueForTtiBundledUe),
                            ContainerNackQueueNode,&(nackNode_p->nackNodeAnchor)))
                {
                    FREE_MEM_NODE_NACK_CIRC_Q(nackNode_p);
                    /* SPR 16916 FIX START */
#ifdef KPI_STATS
                    freeULHarqProcess(&ulUEContext_p->ulresInfo[ulHarqProcessId],
                            ulUEContextInfo_p->ulUEContext_p->ueIndex,
                            internalCellIndex);
#else
                    freeULHarqProcess(&ulUEContext_p->ulresInfo[ulHarqProcessId]);
#endif
                    /* SPR 16916 FIX END */
                    return;
                }
                /* Coverity 26386 Fix End */
            }
            else
#endif      
            {
                if(pushNode(&(currentContainer_p->nackQueueForSps),
                            &(nackNode_p->nackNodeAnchor)))
                {

                    /*coverity fix 25414*/
                    FREE_MEM_NODE_NACK_CIRC_Q(nackNode_p);
                    return; 
                    /*coverity fix 25414*/

                }
            }
            /* TTIB_Code End */    
        }
        else
        {

            /* TTIB_Code Start */    
#ifdef FDD_CONFIG
            if(ulUEContext_p->ulresInfo[ulHarqProcessId].isTTIBundled)
            {
                /* Coverity 26386 Fix Start */
                if(!ENQUEUE_NACK_QUEUE_FOR_TTIB_UE_Q(&(currentContainer_p->nackQueueForTtiBundledUe),
                            ContainerNackQueueNode,&(nackNode_p->nackNodeAnchor)))
                {
                    FREE_MEM_NODE_NACK_CIRC_Q(nackNode_p);
                    /* SPR 16916 FIX START */
#ifdef KPI_STATS
                    freeULHarqProcess(&ulUEContext_p->ulresInfo[ulHarqProcessId],
                            ulUEContextInfo_p->ulUEContext_p->ueIndex,
                            internalCellIndex);
#else
                    freeULHarqProcess(&ulUEContext_p->ulresInfo[ulHarqProcessId]);
#endif
                    /* SPR 16916 FIX END */
                    return;
                }
                /* Coverity 26386 Fix End */

                LTE_MAC_UT_LOG(LOG_DETAIL,LOG_UL_HARQ,
                        "[%s]:[%d] Node pushed in nackQueueForTtiBundledUe.\n",__func__,__LINE__);
            }
            else
            {

                /*coverity fix 25414*/
                if(pushNode(&(currentContainer_p->nackQueue),&(nackNode_p->nackNodeAnchor)))
                {
                    FREE_MEM_NODE_NACK_CIRC_Q(nackNode_p);
                    return; 
                }
                /*coverity fix 25414*/
            }
#else
            /*coverity fix 25414*/
            if(pushNode(&(currentContainer_p->nackQueue),&(nackNode_p->nackNodeAnchor)))
            {
                FREE_MEM_NODE_NACK_CIRC_Q(nackNode_p);
                return; 
            }
            /*coverity fix 25414*/
#endif
            /* TTIB_Code End */    

        }
        /* SPS_CHG */
#else
#ifdef CIRC_QUEUE_IMPL
        ENQUEUE_NACK_CIRC_Q(&(currentContainer_p->nackCircQueue),
                ContainerNackQueueNode,
                &(nackNode_p->nackNodeAnchor));
        /*CA Changes start  */
        semPostDefault(&waitForULNackQueue_g[internalCellIndex]);
        /*CA Changes end  */
#else
        putEntryInULContainerNackQueueUsingICC(currentContainer_p->ulContainerQIdUL,
                nackNode_p);
#endif
#endif
#ifdef FDD_CONFIG       
        /* SPR 5798 Fix Start */
        currentContainer_p->nackQueueCount++;
        /* SPR 5798 Fix End */

        /* ICIC changes start */
        /*CA Changes start  */
        totalRetxRb_g[internalCellIndex][ulUEContext_p->userLocationType][dci0Index] +=
            ulUEContext_p->ulresInfo[ulHarqProcessId].riLength; /* TX_RETX */

        if (ulUEContext_p->userLocationType == CE_USER)
        {
            totalRetxRb_g[internalCellIndex][CC_USER][dci0Index] +=
                ulUEContext_p->ulresInfo[ulHarqProcessId].riLength; /* TX_RETX */
        }
        /*CA Changes end  */
        /* ICIC changes end */
#else
        /*TDD Config 0 Changes Start*/
        rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
        if(FALSE == rbMapNode_p->advanceAllocFlag)
        {
            *rbMapNode_p = (ulRAInfo_gp[internalCellIndex] +
                    ulRAInfoIndex_g[internalCellIndex])->rbMapInitInfo;
            /* We have allocated PRACH Resource ony but we need to
             ** reserve advance PUCCH resources for this rbMap, thats why this value is set,
             ** which means, no need to do init now.
             **/
            rbMapNode_p->advanceAllocFlag = TDD_ONLY_INIT_DONE;
        }

        rbMapNode_p->totalRetxRb[ulUEContext_p->userLocationType] +=
            ulUEContext_p->ulresInfo[ulHarqProcessId].riLength;
        if (ulUEContext_p->userLocationType == CE_USER)
        {
            rbMapNode_p->totalRetxRb[CC_USER] +=
                ulUEContext_p->ulresInfo[ulHarqProcessId].riLength; 
        }
        rbMapNode_p->nackQueueCount++;
        /*TDD Config 0 Changes End*/
#endif
        /*PHICH_SPR Fix 12758 start */
        ulUEContext_p->ulresInfo[ulHarqProcessId].forcedAckFlag = FORCED_ACK_RESET;
        /*SPR Fix 12758 end */
    }/* Invalid CRC */
#ifdef FDD_CONFIG
    else if ( crcAckNack == 2)
    {
        ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
        if(!ulUEContextInfo_p->pendingDeleteFlag)
        {
            ulUEContext_p->
                ulresInfo[ulHarqProcessId].deltabsrLoad = 0; 
        }
        GET_MEM_FROM_POOL(ContainerAckQueueNode,ackNode_p,sizeof(ContainerAckQueueNode), PNULL);

        /* Rel 5.3: Coverity 24433 Fix Start */
        if (PNULL == ackNode_p)
        {
            /* Not able to allocate Memory */
            return;
        }
        /* Rel 5.3: Coverity Fix End */

        ackNode_p->ueIdx = ueIndex;
        ackNode_p->ttiCounter = recvTTI;
        ackNode_p->crcValid  = 0;
        /*bug_10441_changes start*/
        ackNode_p->isLastRetx = FALSE;
        /*bug_10441_changes end*/
        /* SPS_CHG */
        ackNode_p->schUsed = schUsed;
        ackNode_p->ulHarqProcessId = ulHarqProcessId;
        /* SPS_CHG */
        /* Put the nackNode in nackQueue of containerPool */
        /* COVERITY CID 25414 Fix start */
        if ( pushNode(&(currentContainer_p->ackQueue), 
                    &(ackNode_p->ackNodeAnchor)) )
        {
            lteWarning ("pushNode() failed for nack Circ Queue");
            freeMemPool(ackNode_p);
        }
        /* COVERITY CID 25414 Fix end */
    }
#endif

    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s]No. of Ack Nodes [%d] Nack Nodes [%d] in Container [%d] Exit. ",
            __func__, sQueueCount(&(currentContainer_p->ackQueue)),currentContainer_p->nackQueueCount,
            ulContainerIndex);
    LOG_MAC_MSG(MAC_CRC_RX_CRCFLAG_INS_ID,LOGBRIEF,MAC_L1_INF,\
            currentTTI,\
            crcAckNack,\
            sQueueCount(&(currentContainer_p->ackQueue)),\
            currentContainer_p->nackQueueCount,\
            ulContainerIndex,\
            ueIndex,\
            0.0,0.0,__func__,"CRC_RX_CRCFLAG_INS_CRNTI");

    return ;
}

/*SPR_3305_FIX_START*/
/*****************************************************************************
 * Function Name  : crcAckNackHandlingForTcrnti
 * Inputs         : crcAckNack - Its represent the Uplink Ack/Nack
 *                  ueIndex  - Indx of UE,
 *                  recvTTI -  Recieved tick of the message buffer of CRC Ack/Nack
 *                  ulContainerIndex
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function decode the Uplink Ack Nack and entry into
 *                  the container queue.
 ******************************************************************************/
        /* msg3_retx_tdd_support_start */
        void crcAckNackHandlingForTcrnti( UInt32 crcAckNack,
                UInt32 ueIndex,
                /* SPR 15909 fix start */
                tickType_t recvTTI,
                /* SPR 15909 fix end */
                UInt32 ulContainerIndex,
                /*CA Changes start  */
                InternalCellIndex internalCellIndex
                /*CA Changes end  */ 
                )
{
    /* Uplink Container used for Uplin ACK/Nack */
    UplinkContainer *currentContainer_p = PNULL;
    /*put the UE ID in NACK queue of the container */
    ContainerAckQueueNode *ackNode_p = PNULL;
    /*put the UE ID in NACK queue of the container */
    ContainerNackQueueNode *nackNode_p = PNULL;

#ifdef FDD_CONFIG
    UInt8 dci0Index = 0;
#endif
    /*SPR_3071_FIX*/
    TempCRNTICtx *tempCRNTICtx_p = PNULL;
    /*SPR_3071_FIX*/

    /* ICIC changes start */
    ULUEContextInfo* ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
    ULUEContext *ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
    /* ICIC changes end */
    /* msg3_retx_tdd_support_start */
#ifdef TDD_CONFIG
    ULHarqInfo*   tempULHarqInfo_p = PNULL;
    UInt8 ulHarqProcessId = 0;
    UInt8 subFrameNum = recvTTI%MAX_SUB_FRAME;
    UInt8 ulSubFrame= (recvTTI + getnextUlSfOffsetFromCurrentUlSf(subFrameNum,
                /* CA TDD CHANGES START */
                internalCellIndex
                /* CA TDD CHANGES END */
                )) % MAX_SUBFRAME;
    UInt8 rbMapNum = (recvTTI + getnextUlSfOffsetFromCurrentUlSf(subFrameNum,
                /* CA TDD CHANGES START */
                internalCellIndex
                /* CA TDD CHANGES END */
                )) % getMaxULRBMaps(internalCellIndex);
    RbMapNode *rbMapNode_p  = PNULL;
    /*TDD Config 0 Changes End*/
#endif
    /* msg3_retx_tdd_support_end */       
    /* + SPR 5111 fix */
    UInt8 ueLocType = CC_USER;
    if(PNULL != ulUEContext_p)
    {
        ueLocType = ulUEContext_p->userLocationType;
    }
    /* - SPR 5111 fix */




    /*It represents the current container*/
    /*CA Changes start  */
    currentContainer_p = ulContainerPool_gp[internalCellIndex] + ulContainerIndex;
    /*CA Changes end  */
#ifdef LOG_PRINT_ENABLED
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */
#endif

    tempCRNTICtx_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].tempCRNTICtx_p;

    if(PNULL == tempCRNTICtx_p)
    {
        return;
    }
#ifdef TDD_CONFIG
    /* SPR 11257 Changes Start*/ 
    ulHarqProcessId = tempCRNTICtx_p->ulHarq.ulHarqProcessId;
    if ( ulHarqProcessId == INVALID_HARQ_ID)
    {
        return ;
    }
    /* SPR 11257 Changes End*/
#endif
    /*( 0 - Correct Data )*/
    if ( crcAckNack == 0 )
    {
        /* msg3_retx_tdd_support_start */
        tempCRNTICtx_p->ulHarq.deltabsrLoad = 0;
        /* msg3_retx_tdd_support_end */
        tempCRNTICtx_p->ulHarq.harqStatus = HARQ_PROCESS_FREE;   /* TX_RETX */
        GET_MEM_FROM_POOL(ContainerAckQueueNode,ackNode_p,
                sizeof(ContainerAckQueueNode), PNULL);

        /* Rel 5.3: Coverity 24435 Fix Start */
        if (PNULL == ackNode_p)
        {
            /* Not able to allocate Memory */
            return;
        }
        /* Rel 5.3: Coverity Fix End */

        ackNode_p->ueIdx = ueIndex;
        ackNode_p->ttiCounter = recvTTI;
        ackNode_p->crcValid  = 1;
        /* msg3_retx_tdd_support_start */
        ackNode_p->tcRntiFlag = TRUE;
        /* msg3_retx_tdd_support_end */
#ifdef TDD_CONFIG
        tempULHarqInfo_p = &(tempCRNTICtx_p->ulHarq);
        /* msg3_retx_tdd_support_start */
        ackNode_p->ulHarqProcessId = ulHarqProcessId;
        /* msg3_retx_tdd_support_end */
        /*ackNode_p->allocatedLength =
         * tempULHarqInfo_p->advancedReserveRB.allocatedLength;
         * ackNode_p->allocatedIndex =
         * tempULHarqInfo_p->advancedReserveRB.allocatedIndex;
         */
        ackNode_p->allocatedLength = tempULHarqInfo_p->riLength;
        ackNode_p->allocatedIndex = tempULHarqInfo_p->riStart;
#endif
        /* Put the nackNode in nackQueue of containerPool */
        /* + Coverity  32432 */
        if(pushNode(&(currentContainer_p->ackQueue), &(ackNode_p->ackNodeAnchor)))
        {
            freeMemPool(ackNode_p);
            return ; 
        }
        /* - Coverity  32432 */

    }
    /* Case of Error */
    else if ( crcAckNack == 1)
    {

        GET_MEM_NODE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(nackNode_p,
                ContainerNackQueueNode);

        /* Rel 5.3: Coverity 24436 Fix Start */
        if (PNULL == nackNode_p)
        {
            /* Not able to allocate Memory */
            return;
        }
        /* Rel 5.3: Coverity Fix End */

#ifdef TDD_CONFIG
        /* msg3_retx_tdd_support_start */
        nackNode_p->ulHarqProcessId = ulHarqProcessId;
        /* msg3_retx_tdd_support_end */
        nackNode_p->ulSubFrame = ulSubFrame;
#elif FDD_CONFIG
        dci0Index = (recvTTI + FDD_HARQ_OFFSET) %
            NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
#endif
        nackNode_p->ueIdx = ueIndex;
        nackNode_p->ttiCounter = recvTTI;
#ifndef DL_UL_SPLIT
        /*SPR_Msg3_RETX_WO_DL_UL_FIX_START*/
        /* + Coverity  32432 */
        if(pushNode(&(currentContainer_p->nackQueueForTempUE),
                    &(nackNode_p->nackNodeAnchor)))
        {
            FREE_MEM_NODE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(nackNode_p);
            return ; 
        }
        /* - Coverity  32432 */

        /*SPR_Msg3_RETX_WO_DL_UL_FIX_END*/
#else
#ifdef CIRC_QUEUE_IMPL
        ENQUEUE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(&(currentContainer_p->nackCircQueueForTempUE),
                ContainerNackQueueNode,
                &(nackNode_p->nackNodeAnchor));
        /*CA Changes start  */
        semPostDefault(&waitForULNackForTempUE_g[internalCellIndex]);
        /*CA Changes end  */
#else
        putEntryInULContainerNackQueueUsingICC(currentContainer_p->ulContainerNackQueueForTempUEIdUL,
                nackNode_p);
#endif
#endif
#if FDD_CONFIG       
        /* SPR 5798 Fix Start */
        currentContainer_p->nackQueueCount++;
        /* SPR 5798 Fix End */

        /* ICIC changes start */
        /* + SPR 5111 fix */
        /*CA Changes start  */
        totalRetxRb_g[internalCellIndex][ueLocType][dci0Index] +=
            tempCRNTICtx_p->ulHarq.riLength; /* TX_RETX */

        if (ueLocType == CE_USER)
        {
            totalRetxRb_g[internalCellIndex][CC_USER][dci0Index] +=
                tempCRNTICtx_p->ulHarq.riLength; /* TX_RETX */
        }
        /*CA Changes end  */
        /* - SPR 5111 fix */
        /* ICIC changes end */
#else
        /*TDD Config 0 Changes Start*/
        rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
        if(FALSE == rbMapNode_p->advanceAllocFlag)
        {
            *rbMapNode_p = (ulRAInfo_gp[internalCellIndex] +
                    ulRAInfoIndex_g[internalCellIndex])->rbMapInitInfo;
            /* TDD Config 0 Changes Start */
            /* We have allocated PRACH Resource ony but we need to
             * reserve advance PUCCH resources for this rbMap, thats why this value is set,
             * which means, no need to do init now.
             **/
            rbMapNode_p->advanceAllocFlag = TDD_ONLY_INIT_DONE;
            /* TDD Config 0 Changes End */
        }

        rbMapNode_p->totalRetxRb[ueLocType] +=
            tempCRNTICtx_p->ulHarq.riLength;
        if (ueLocType == CE_USER)
        {
            rbMapNode_p->totalRetxRb[CC_USER] +=
                tempCRNTICtx_p->ulHarq.riLength; 
        }
        rbMapNode_p->nackQueueCount++;
        /*TDD Config 0 Changes End*/
#endif
    }/* Invalid CRC */
    else if ( crcAckNack == 2)
    {
        /* msg3_retx_tdd_support_start */
        tempCRNTICtx_p->ulHarq.deltabsrLoad = 0;
        /* msg3_retx_tdd_support_end */
        GET_MEM_FROM_POOL(ContainerAckQueueNode,ackNode_p,sizeof(ContainerAckQueueNode), PNULL);

        /* Rel 5.3: Coverity 24435 Fix Start */
        if (PNULL == ackNode_p)
        {
            /* Not able to allocate Memory */
            return;
        }
        /* Rel 5.3: Coverity Fix End */
        /* msg3_retx_tdd_support_start */
#ifdef TDD_CONFIG
        ackNode_p->ulHarqProcessId = ulHarqProcessId;
#endif
        /* msg3_retx_tdd_support_end */
        ackNode_p->ueIdx = ueIndex;
        ackNode_p->ttiCounter = recvTTI;
        ackNode_p->crcValid  = 0;
        /* msg3_retx_tdd_support_start */
        ackNode_p->tcRntiFlag = TRUE;
        /* msg3_retx_tdd_support_end */
        /* + Coverity 32432 */
        /* Put the nackNode in nackQueue of containerPool */
        if(pushNode(&(currentContainer_p->ackQueue), &(ackNode_p->ackNodeAnchor)))
        {
            freeMemPool(ackNode_p);
            return ;
        }
        /* - Coverity  32432 */
    }
    LOG_MAC_MSG(MAC_CRC_RX_CRCFLAG_INS_ID,LOGBRIEF,MAC_L1_INF,\
            currentTTI,\
            crcAckNack,\
            sQueueCount(&(currentContainer_p->ackQueue)),\
            currentContainer_p->nackQueueCount,\
            ulContainerIndex,\
            ueIndex,\
            0.0,0.0,__func__,"CRC_RX_CRCFLAG_INS_TCRNTI");

    return ;
}
/* msg3_retx_tdd_support_end */
/*SPR_3305_FIX_END*/

/*****************************************************************************
 * Function Name  : putEntryInSRQueue
 * Inputs         : ueIndex - Index of UE and
 *                  recvTTI - recieved tick of the message buffer of CRC Ack/Nack
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : None
 * Returns        : NONE
 * Description    : This function creates a new srNode and inserts
 this node in the globalSrQueue.*/
            /*****************************************************************************/
            /*CA Changes start  */
            void putEntryInSRQueue(UInt32 ueIndex,
                    /* SPR 15909 fix start */
                    tickType_t recvTTI,
                    /* SPR 15909 fix end */ 
                    InternalCellIndex internalCellIndex)
            /*CA Changes end  */
{
    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] UE Index [%d] RecvTTI [%d] Entry. ",
            __func__, ueIndex, recvTTI);
    UInt32 netBsrLoad = 0;                   
    ULUEContext *ulUeCtx_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
    /* SR Queue Node is used store SR Info*/
    /* SPR 6390 Changes Starts */
#ifdef REAL_UE_ENVIORNMENT
    if( ulUeCtx_p->bsrNetTotal )
    {
        if(UL_QOS_SCHEDULING == cellSpecificParams_g.\
                cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
                ulSchdConfig.ulSchedulerStrategy)
        {
            UInt8 lcgId = 0;
            for(lcgId = 0; lcgId < NUM_OF_LCG  ; lcgId++)
            {
                ulUeCtx_p->bsrNet[lcgId] = 0;
                putEntryInBSRQueue(ueIndex,
                        lcgId,
                        0,
                        DELETE_TRIGGER,
                        /*CA Changes start  */
                        internalCellIndex);
                /*CA Changes end  */
            }
        }
        else
        {
            putEntryInBSRQueue(ueIndex,
                    0,
                    0,
                    DELETE_TRIGGER,
                    /*CA Changes end  */
                    internalCellIndex);
            /*CA Changes end  */
        }
        ulUeCtx_p->bsrNetTotal = 0;
    }
#endif
    /* SPR 6390 Changes ends */
    netBsrLoad = ulUeCtx_p->bsrNetTotal;

    if ( 0 == netBsrLoad )
    {
        ULGrantRequestQueueNode* srNode_p = PNULL;
        /*SR starvation fix start */
        if(!ulUeCtx_p->srNode_p)
        {

            GET_MEM_FROM_POOL(ULGrantRequestQueueNode,srNode_p,sizeof(ULGrantRequestQueueNode),PNULL);

            /* Rel 5.3: Coverity 24456 Fix Start */
            if (PNULL != srNode_p)
            {
                srNode_p->ueIndex            = ueIndex;
                srNode_p->ttiCounter         = recvTTI;
                srNode_p->ulGrantRequestType = SR_ULGRANT_REQ_MSG;  
                /* Put the srNode in global srQueue */
                /* SPR 3608 Start */
                if(ulUeCtx_p->prioritySchedulingReqUL)
                {
                    /*CA Changes start  */
                    /* COVERITY CID 25433 Fix start */
                    if (pushNode(ulHighPriorityGrantQueue_gp[internalCellIndex], 
                                &(srNode_p->ulGrantRequestNodeAnchor)) )
                    {
                        lteWarning ("pushNode() failed for ul HighPriority Grant Queue");
                        freeMemPool(srNode_p);
                        srNode_p = PNULL;
                    }
                    /* COVERITY CID 25433 Fix end */
                    /*CA Changes start  */
                }
                else
                {
                    /*CA Changes start  */
                    /* COVERITY CID 25433 Fix start */
                    if (pushNode(ulGrantGlobalSrQueue_gp[internalCellIndex], 
                                &(srNode_p->ulGrantRequestNodeAnchor)))
                    {
                        lteWarning ("pushNode() failed for ul Grant Global Sr Queue");
                        freeMemPool(srNode_p);
                        srNode_p = PNULL;
                    }
                    /* COVERITY CID 25433 Fix end */
                    /*CA Changes start  */
                }
                /* SPR 3608 end */
                LOG_UT(MAC_SR_RCV_IND_ID,LOGDEBUG,MAC_L1_INF,\
                        getCurrentTick(),\
                        ueIndex,\
                        recvTTI,netBsrLoad,0,0,\
                        0.0,0.0,__func__,"SR_RCV_IND");
                ulUeCtx_p->srNode_p = srNode_p;
            }
            else
            {
                /* Not able to allocate Memory */
            }
            /* Rel 5.3: Coverity Fix End */
        }
        else
        {
            {
                ulUeCtx_p->srNode_p->ttiCounter = recvTTI;
            }
        }
    }
    /* SR Drop Log Start */
    else
    {

        /* +- SPR 17779 */
        LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID_1,\
                LOGDEBUG,MAC_UL_Strategy,\
                getCurrentTick(),\
                ueIndex,\
                __LINE__,netBsrLoad,0,0,0.0,0.0,__func__,
                "SrDrop_BsrNonZero");
    }
    /* SR Drop Log End */
    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s]No. Of SR Nodes [%d] Exit. ",
            __func__,sQueueCount(ulGrantGlobalSrQueue_gp[internalCellIndex]));
}

/*****************************************************************************
 * Function Name  : validatePHYDelayAndTick 
 * Inputs         : systemFrameNum - system frame number of msg recieved.
 *                  subFrameNum    - subframe number of msg recieved.
 *                  tti_p          - fill the current tick of the recieved
 *                                   msg buffer.
 *                  msgId          - L1 API from which this function is called.
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is used to validate the system Frame number
 *                  and subFrame number and also check the wrap around condiiton
 *                  and fill the validate tick recieved from the msg buffer.
 *****************************************************************************/
STATIC  MacRetType validatePHYDelayAndTick( UInt32 systemFrameNum, 
        UInt32 subFrameNum,
        /* SPR 15909 fix start */
        tickType_t *tti_p,
        /* SPR 15909 fix end */
        UInt16 msgId,
        InternalCellIndex internalCellIndex )
{ 
    /* SPR 15909 fix start */
    tickType_t curTTI = 0;
    /* SPR 15909 fix end */
    UInt32 curSysFrameNum = 0;
    UInt32 curSubFrameNum = 0;

    /* get current System Number and SubFrame Number */
    getSFAndSFN(&curSysFrameNum, &curSubFrameNum,internalCellIndex);


    /* Msg SFN and Current SFN are equal. */
    if  (systemFrameNum == curSysFrameNum)
    {
        /* Msg SF is greater than Current SF*/
        if (subFrameNum > curSubFrameNum)
        {
            LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Curr SF[%d] Msg SF [%d]. curreSFN = %d msgSFN = %d\n",
                    __func__,curSubFrameNum,subFrameNum, curSysFrameNum, systemFrameNum);

            return MAC_FAILURE;
        }
        else
        {    /* Current SF is greater than Msg SF
                but Difference b/w both SF is greater than PHY_DELAY */ 
            /*SPR 19864 Fix start*/
                if ( ( curSubFrameNum - subFrameNum) > PHY_DELAY && 
                        (( PHY_UL_RACH_INDICATION != msgId )  
                         ) ) 
//#endif  //Check again
                    /*SPR 19864 Fix end*/
                {
                    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Curr SF[%d] Msg SF [%d]. diff greater than PHY Diff [%d]\n",
                            __func__,curSubFrameNum,subFrameNum,PHY_DELAY);
                    LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF," curreSFN = %d msgSFN = %d\n", curSysFrameNum, systemFrameNum);

                    return MAC_FAILURE;
                }
                else
                {
                    curTTI = getCurrentTick();
                    *tti_p = curTTI - (curSubFrameNum - subFrameNum);
                    return MAC_SUCCESS;
                }
        }
    } /* Msg SFN is smaller than Current SFN */
    else if (systemFrameNum < curSysFrameNum)
    {
        /* Diff b/w both SFN should not be greater than One.*/
        if ( (curSysFrameNum - systemFrameNum) > 1)
        {
            LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Curr SFN[%d] Msg SFN [%d]. diff > 1 curSF = %d msgSF = %d\n",
                    __func__,curSysFrameNum,systemFrameNum,curSubFrameNum,subFrameNum );

            return MAC_FAILURE;
        }
    }/* Msg SFN is greater than Current SFN */
    else if (systemFrameNum > curSysFrameNum)
    {
        /*Wrap around condition*/
        if ((0 != curSysFrameNum) || ((MAX_SFN_VALUE - 1) != systemFrameNum))
        {
            lteWarning("[%s] [Wrap Around Cond ]Msg SFN > Current SFN ** Curr SFN[%d] Msg SFN [%d].  ",
                    __func__,curSysFrameNum,systemFrameNum);
            LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"curSF = %d msgSF = %d\n",curSubFrameNum,subFrameNum);

            return MAC_FAILURE;
        }
    }

    /* Validate the Sub Frame number  */
    if (subFrameNum < curSubFrameNum)
    {
        lteWarning("[Wrap Round Cond](Msg SFN [%d] , Current SFN [%d])  Msg SF [%d] < Curr SF[%d]",
                systemFrameNum,curSysFrameNum, subFrameNum, curSubFrameNum);

        return MAC_FAILURE;
    }
    else
    {
            if ( ((10 - subFrameNum) + curSubFrameNum) > PHY_DELAY &&
                    (( PHY_UL_RACH_INDICATION != msgId ) 
                    ))
            {
                lteWarning("msg SFN [%d ] Current SFN [%d] (10 - subFrameNum[%d]) + curSubFrameNum[%d]) greater than PHY Delay [%d]",
                        systemFrameNum,curSysFrameNum, subFrameNum,curSubFrameNum,PHY_DELAY);

                return MAC_FAILURE;
            }
            else
            {
                curTTI = getCurrentTick();
                *tti_p = curTTI - ((10 - subFrameNum) + curSubFrameNum);

            }
        return MAC_SUCCESS;
    }
}

#ifdef DL_UL_SPLIT_TDD
/*****************************************************************************
 * Function Name  : validatePHYDelayAndTickUl 
 * Inputs         : systemFrameNum - system frame number of msg recieved.
 *                  subFrameNum    - subframe number of msg recieved.
 *                  tti_p          - fill the current tick of the recieved
 *                                   msg buffer.
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function is used to validate the system Frame number
 *                  and subFrame number for TDD UL thread and also check the 
 *                  wrap around condiitonand fill the validate tick recieved 
 *                  from the msg buffer. DL thread is using function 
 *                  validatePHYDelayAndTick
 *****************************************************************************/
    STATIC  MacRetType validatePHYDelayAndTickUl( UInt32 systemFrameNum, 
            UInt32 subFrameNum, 
            /* SPR 15909 fix start */
            tickType_t *tti_p
            /* SPR 15909 fix end */
            )
{ 
    /* SPR 15909 fix start */
    tickType_t curTTI = 0;
    /* SPR 15909 fix end */
    UInt32 curSysFrameNum = 0;
    UInt32 curSubFrameNum = 0;

    /* get current System Number and SubFrame Number */
    getUlSFAndSFN(&curSysFrameNum, &curSubFrameNum);


    /* Msg SFN and Current SFN are equal. */
    if  (systemFrameNum == curSysFrameNum)
    {
        /* Msg SF is greater than Current SF*/
        if (subFrameNum > curSubFrameNum)
        {
            LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Curr SF[%d] Msg SF [%d]. curreSFN = %d msgSFN = %d\n",
                    __func__,curSubFrameNum,subFrameNum, curSysFrameNum, systemFrameNum);

            return MAC_FAILURE;
        }
        else
        {    /* Current SF is greater than Msg SF
                but Difference b/w both SF is greater than PHY_DELAY */ 
            if ((curSubFrameNum - subFrameNum) > PHY_DELAY)
            {
                LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Curr SF[%d] Msg SF [%d]. diff greater than PHY Diff [%d]\n",
                        __func__,curSubFrameNum,subFrameNum,PHY_DELAY);
                LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF," curreSFN = %d msgSFN = %d\n", curSysFrameNum, systemFrameNum);

                return MAC_FAILURE;
            }
            else
            {
                curTTI = getCurrentTickUl();
                *tti_p = curTTI - (curSubFrameNum - subFrameNum);
                return MAC_SUCCESS;
            }
        }
    } /* Msg SFN is smaller than Current SFN */
    else if (systemFrameNum < curSysFrameNum)
    {
        /* Diff b/w both SFN should not be greater than One.*/
        if ( (curSysFrameNum - systemFrameNum) > 1)
        {
            LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"[%s] Curr SFN[%d] Msg SFN [%d]. diff > 1 curSF = %d msgSF = %d\n",
                    __func__,curSysFrameNum,systemFrameNum,curSubFrameNum,subFrameNum );

            return MAC_FAILURE;
        }
    }/* Msg SFN is greater than Current SFN */
    else if (systemFrameNum > curSysFrameNum)
    {
        /*Wrap around condition*/
        if ((0 != curSysFrameNum) || ((MAX_SFN_VALUE - 1) != systemFrameNum))
        {
            lteWarning("[%s] [Wrap Around Cond ]Msg SFN > Current SFN ** Curr SFN[%d] Msg SFN [%d].  ",
                    __func__,curSysFrameNum,systemFrameNum);
            LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF,"curSF = %d msgSF = %d\n",curSubFrameNum,subFrameNum);

            return MAC_FAILURE;
        }
    }

    /* Validate the Sub Frame number  */
    if (subFrameNum < curSubFrameNum)
    {
        lteWarning("[Wrap Round Cond](Msg SFN [%d] , Current SFN [%d])  Msg SF [%d] < Curr SF[%d]",
                systemFrameNum,curSysFrameNum, subFrameNum, curSubFrameNum);

        return MAC_FAILURE;
    }
    else
    {
        if (((10 - subFrameNum) + curSubFrameNum) > PHY_DELAY)
        {
            lteWarning("msg SFN [%d ] Current SFN [%d] (10 - subFrameNum[%d]) + curSubFrameNum[%d]) greater than PHY Delay [%d]",
                    systemFrameNum,curSysFrameNum, subFrameNum,curSubFrameNum,PHY_DELAY);

            return MAC_FAILURE;
        }
        else
        {
            curTTI = getCurrentTickUl();
            *tti_p = curTTI - ((10 - subFrameNum) + curSubFrameNum);

        }
        return MAC_SUCCESS;
    }
}
#endif
#ifdef TDD_CONFIG
/** CA-TDD Changes Start:06 **/
extern ElArguments elArgsTdd_g[MAX_NUM_CELL][MAX_SUB_FRAME];
/** CA-TDD Changes End:06 **/

/*****************************************************************************
 * Function Name  : calculateHarqExpTickMissAndArgs 
 * Inputs         : execLegFuncArg_p - pointer to  ExecutionLegFunctionArg,
 *                  harqExpiryArg, airSf, airSfn, totalTickDiffNum and scenarioType
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function processes HARQ and calculates tick and miss
 *                  args.
 *****************************************************************************/
            MacRetType calculateHarqExpTickMissAndArgs(
                    ExecutionLegFunctionArg *execLegFuncArg_p,
                    ElArgumentTypes harqExpiryArg,
                    /* SPR 15909 fix start */
                    tickType_t currTick,
                    /* SPR 15909 fix end */
                    UInt8 airSf,
                    UInt16 airSfn, UInt16 totalTickDiffNum,
                    /* SPR 11110 fix start */
                    UInt8 scenarioType )
            /* SPR 11110 fix end */
{
    UInt16 tempTotalTickDiff = totalTickDiffNum;
    MacRetType retVal = MAC_FAILURE;

    while( ( totalTickDiffNum > 0 ) && 
            elArgsTdd_g[execLegFuncArg_p->cellIndex][airSf].elArgArray[harqExpiryArg] == MAX_SUBFRAME)
    {
        totalTickDiffNum--;
        if (airSf == SUB_FRAME_0)
        {
            airSf = SUB_FRAME_9;
            if(0 == airSfn)
            {
                airSfn = MAX_SYS_FRAME_NUM;
            }
            else
            {
                airSfn--;
            }
        }
        else
        {
            airSf--;
        }
    }
    /* SPR 11110 fix start */
    if ( ( ( totalTickDiffNum > 0 ) &&
                ( tempTotalTickDiff != totalTickDiffNum ) ) ||
            ( ERROR_SCENARIO == scenarioType) )
        /* SPR 11110 fix end */
    {
        /* Free All HARQ process from previous UL Sub Frame*/
        execLegFuncArg_p->globalTick = currTick - (tempTotalTickDiff - totalTickDiffNum);
        execLegFuncArg_p->subFrame = airSf;
        execLegFuncArg_p->sysFrame = airSfn;
        execLegFuncArg_p->advanceDLGlobalTick =
            execLegFuncArg_p->globalTick + DL_ADVANCE_TICK;
        /* SPR 11110 fix start */
        if( ERROR_SCENARIO == scenarioType &&
                MAX_SUBFRAME == elArgsTdd_g[execLegFuncArg_p->cellIndex][airSf].elArgArray[harqExpiryArg] )
        {
            execLegFuncArg_p->execLegSpecificArg1 = MAX_SUBFRAME - 1;
        }
        else
        {            
            execLegFuncArg_p->execLegSpecificArg1 = elArgsTdd_g[execLegFuncArg_p->cellIndex][airSf].elArgArray[harqExpiryArg];
        }
        /* SPR 11110 fix end */
        /* set isFreeOnly to TRUE to free HARQ processes in case of tick miss*/
        execLegFuncArg_p->execLegSpecificArg2 = LTE_TRUE;

        airSf += DL_ADVANCE_TICK; 
        if ( airSf >= MAX_SUB_FRAME)
        {
            airSf -= MAX_SUB_FRAME;
            airSfn++;

            if ( airSfn == MAX_SFN_VALUE)
            {
                airSfn = 0;
            }
        }
        execLegFuncArg_p->advanceDLSubframe = airSf;
        execLegFuncArg_p->advanceDLSystemframe = airSfn;
        retVal = MAC_SUCCESS;
    }
    return retVal;
}
#endif


/* TTIB_Code Start */
#ifdef FDD_CONFIG
/* Maximum possible value of tick miss without any
   contention is TTI_BUNDLE_RTT - MAXIMUM ADVANCE ENQUEUE
   where, TTI_BUNDLE_RTT = 16 and
   MAXIMUM ADVANCE ENQUEUE = 9
   */

#define MAX_TTIB_TICK_MISS_ALLOWED  7
    /*****************************************************************************
     * Function Name  : freeTTIBundlingNodesForTickMiss 
     * Inputs         : totalNumberOfTickDiff - TTI difference to current proccessing
     *                  currentTick - current TTI,
     *                  ulDelay - uplink Delay
     *                  internalCellIndex - Cell_index at MAC
     * Outputs        : None
     * Returns        : MAC_SUCCESS/MAC_FAILURE
     * Description    : This function delete the TTI Bundling nodes 
     *                  if TICK Overshoot occurs. 
     *****************************************************************************/
void freeTTIBundlingNodesForTickMiss(UInt8 totalNumberOfTickDiff,
        /* SPR 15909 fix start */
        tickType_t currentTick,
        /* SPR 15909 fix end */
        UInt8 ulDelay,
        InternalCellIndex internalCellIndex)
{
    UInt8  qCount           = 0;
    UInt8  bundleQueueIndex = 0;
    /* SPR 15909 fix start */
    tickType_t tickCount        =0;
    /* SPR 15909 fix end */
    TtiBScheduleNode *ttiBScheduleNode_p = PNULL;
    if(MAX_TTIB_TICK_MISS_ALLOWED < totalNumberOfTickDiff)
    {
        totalNumberOfTickDiff = MAX_TTIB_TICK_MISS_ALLOWED;
    }

    tickCount = (currentTick + ulDelay) - totalNumberOfTickDiff;
    for(; tickCount < (currentTick + ulDelay); tickCount++)
    {
        bundleQueueIndex = MODULO_SIXTEEN(tickCount);
        /*CA Changes start  */
        qCount = COUNT_TTIB_SCHEDULE_Q(
                (ttibScheduleQueue_gp[internalCellIndex] + bundleQueueIndex));
        /*CA Changes end  */
        while(qCount-- )
        {
            /*CA Changes start  */
            DEQUEUE_TTIB_SCHEDULE_Q(
                    (ttibScheduleQueue_gp[internalCellIndex] + bundleQueueIndex),
                    TtibScheduleNode,(void **)&ttiBScheduleNode_p);
            /*CA Changes end  */
            if (NULL != ttiBScheduleNode_p)
            {
                FREE_MEM_NODE_TTIB_SCHEDULE_Q(ttiBScheduleNode_p);
            }
        }/* End of While(qCount)*/
    }/* End of For(tickCount)*/
}
#endif
/* TTIB_Code End */


/*****************************************************************************
 * Function Name  : processSubFrameIndicationForTickMiss 
 * Inputs         : systemFrameNum, subFrameNum
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function process the Sub Frame Indiaction  
 *                  if TICK Overshoot occurs. 
 *****************************************************************************/
/* CA Changes start */ 
void processSubFrameIndicationForTickMiss(UInt16 systemFrameNum,
        UInt8 subFrameNum,
        InternalCellIndex  internalCellIndex)
{
    UInt32 curSysFrameNum = 0;
    UInt32 curSubFrameNum = 0;
    UInt32 counter = 0;
#ifdef OVERSHOOT_CORRECTION
    UInt8 numOfUEScheduled = cellSpecificParams_g.\
                             cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
                             dlSchdConfig.maxUEsToBeScheduledDL;
#endif
    /* get current System Number and SubFrame Number */
    getSFAndSFN(&curSysFrameNum, &curSubFrameNum, internalCellIndex);
    if( curSysFrameNum > 1023)
    {
        curSysFrameNum = 0;
    }

    /* SPR 4727 changes start */ 
    if (systemFrameNum >= MAX_SFN_VALUE)
    {
        /*Wrap around condition*/
        systemFrameNum -= MAX_SFN_VALUE;
    }
    /* SPR 4727 changes end */ 

    /* SPR 15909 fix start */
    tickType_t curTTI = (curSysFrameNum*10 + curSubFrameNum);
    tickType_t recvTTI = (systemFrameNum*10 + subFrameNum);
    /* SPR 15909 fix end */
    UInt32 offset = 0;
    if(curTTI > recvTTI)
    {     
        offset = (MAX_SFN_VALUE * MAX_SUB_FRAME) ;
    }     
    totalNumberOfTickDiffDl_g[internalCellIndex] = offset + recvTTI - curTTI;

    if(totalNumberOfTickDiffDl_g[internalCellIndex] > 1)
    {

#ifdef OVERSHOOT_CORRECTION
        currTickOverLoad_g[internalCellIndex] = OVERLOAD_LEVEL_CRITICAL;
        /* SPR 21292 fix start */
        UInt32 count = 0;
        UInt32 rbMapNum = 0;
        UInt32 containerTick = 0;
        ULSchUciRachPoolInfo *ulschUciPoolInfo_p = PNULL;
        for (count = 0;  count < totalNumberOfTickDiffDl_g[internalCellIndex]; count++)
        {
            containerTick = (getCurrentTick() + PHY_DL_DELAY + count) % MAX_PDCCH_CONTAINER;
            ulschUciPoolInfo_p = (ulSchUciRachContainer_gp[internalCellIndex] + containerTick);
            ulschUciPoolInfo_p->numSRSPdus = 0;
            ulschUciPoolInfo_p->numberOfInfoPresent = 0;
            ulschUciPoolInfo_p->isPrachResourceReserved = 0;
            ulschUciPoolInfo_p->prachFreqOffset = 0;
            rbMapNum = MODULO_SIXTEEN((getCurrentTick() + PHY_DL_DELAY + count + FDD_HARQ_OFFSET));
            resetRBsAtLocation(rbMapNum,internalCellIndex);

        }
        /* SPR 21292 fix end */


#endif
        LOG_MAC_MSG(MAC_PHY_TICK_MISS,LOGWARNING,MAC_L1_INF,\
                systemFrameNum,subFrameNum,curSysFrameNum,curSubFrameNum,\
                0,0,0,0,__func__,"CRITICAL_OVERLOAD_LEVEL\
                set");
    }
    /* Copy the value of DL tick miss to UL */
    totalNumberOfTickDiffUl_g[internalCellIndex] = totalNumberOfTickDiffDl_g[internalCellIndex] ;

    for(counter =0; counter < totalNumberOfTickDiffDl_g[internalCellIndex]; counter++)
    {
        processSubFrameInd( systemFrameNum, subFrameNum, internalCellIndex);
    }
    /* TTIB_Code Start */
#ifdef FDD_CONFIG
    if(totalNumberOfTickDiffUl_g[internalCellIndex] > 1)
    {
        freeTTIBundlingNodesForTickMiss(totalNumberOfTickDiffUl_g[internalCellIndex],
                getCurrentTick(),
                PHY_DELAY,
                internalCellIndex);
    }
#endif
    /* TTIB_Code End */


#ifdef OVERSHOOT_CORRECTION
    modifiedNoOfUEsAfterOverLoadDL(&numOfUEScheduled, internalCellIndex);
#endif

#ifdef TDD_CONFIG

    if(totalNumberOfTickDiffDl_g[internalCellIndex] > 1)
    {
        UInt8 airSf = subFrameNum;
        UInt16 airSfn = systemFrameNum;
        ExecutionLegFunctionArg execLegFuncArg = {0};
        /** SPR 14866 Changes Start **/
        execLegFuncArg.cellIndex = internalCellIndex;
        /** SPR 14866 Changes End **/
        if (MAC_SUCCESS == calculateHarqExpTickMissAndArgs(&execLegFuncArg, 
                    ELARG_DL_HARQ_EXPIRY_U_SF, getCurrentTick(), airSf, airSfn,
                    totalNumberOfTickDiffDl_g[internalCellIndex],
                    /* SPR 11110 fix start */
                    TICK_MISS_SCENARIO ) )
            /* SPR 11110 fix end */
        {
            executionFunctionArrTdd_g[PROCESS_HARQ_TIMER_EXPIRY](&execLegFuncArg);
        }
        if (MAC_SUCCESS == calculateHarqExpTickMissAndArgs(&execLegFuncArg, 
                    ELARG_UL_HARQ_EXPIRY_U_SF, getCurrentTick(), airSf, airSfn,
                    totalNumberOfTickDiffUl_g[internalCellIndex],
                    /* SPR 11110 fix start */
                    TICK_MISS_SCENARIO ) )
            /* SPR 11110 fix end */
        {
            executionFunctionArrTdd_g[PROCESS_UL_HARQ_TIMER_EXPIRY](&execLegFuncArg);
        }
    }
#endif 

}

/* FAPI L2 AS MASTER START */
#ifdef FAPI_L2_MASTER
/*****************************************************************************
 * Function Name  : processAndValidateL2TickBeforeELExecution 
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function invokes before executing ELs if new L2 tick 
 *                  is received. Also, this function takes action if any tick
 *                  overshoot is occurred.
 *****************************************************************************/
                void processAndValidateL2TickBeforeELExecution(
                        InternalCellIndex  internalCellIndex)
{

#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix end */
    tickType_t tick = 0;
    /* SPR 15909 fix end */
#endif
    UInt32 curSysFrameNum = 0;
    UInt32 curSubFrameNum = 0;
    UInt32 counter = 0;
    UInt32 subFrameFrmL2Tick = 0;
    UInt32 sysFrameFrmL2Tick = 0;

#ifdef OVERSHOOT_CORRECTION
    UInt8 numOfUEScheduled = cellSpecificParams_g.\
                             cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
                             dlSchdConfig.maxUEsToBeScheduledDL;
#endif
    /* get current System Number and SubFrame Number from MAC Tick */
    getSFAndSFN(&curSysFrameNum, &curSubFrameNum, internalCellIndex);
    getSFAndSFNFrmL2Tick(&sysFrameFrmL2Tick, &subFrameFrmL2Tick, internalCellIndex);

    /* SPR 15909 fix start */
    tickType_t curTTI  = ((curSysFrameNum*MAX_SUB_FRAME) + curSubFrameNum);
    tickType_t recvTTI = ((sysFrameFrmL2Tick*MAX_SUB_FRAME) + subFrameFrmL2Tick);
    /* SPR 15909 fix end */
    UInt32 offset = 0;
    if(curTTI > recvTTI)
    {     
        offset = (MAX_SFN_VALUE * MAX_SUB_FRAME) ;
    }     
    totalNumberOfTickDiffDl_g[internalCellIndex] = offset + recvTTI - curTTI;

    if(totalNumberOfTickDiffDl_g[internalCellIndex] > 1)
    {
#ifdef OVERSHOOT_CORRECTION
        currTickOverLoad_g[internalCellIndex] = OVERLOAD_LEVEL_CRITICAL;
#endif
        LOG_MAC_MSG(MAC_PHY_TICK_MISS,LOGWARNING,MAC_L1_INF,\
                sysFrameFrmL2Tick,subFrameFrmL2Tick,curSysFrameNum,curSubFrameNum,\
                0,0,0,0,__func__,"CRITICAL_OVERLOAD_LEVEL\
                set");
    }
    /* Copy the value of DL tick miss to UL */
    totalNumberOfTickDiffUl_g[internalCellIndex] = totalNumberOfTickDiffDl_g[internalCellIndex] ;

    /* compilation 12-JUN-2014 fix start */
#ifdef FAPI_L2_MASTER
    for(counter =0; counter < totalNumberOfTickDiffDl_g[internalCellIndex]; counter++)
    {
        initL2TickBeforeELExecution(internalCellIndex);
    }
#endif
    /* compilation 12-JUN-2014 fix end */
    /* HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    /*Update dlDelay value which will be used for updation of sched maps of HD FDD UEs*/
    if(hdUeContextInfo_g.numHdUe > 0)
    {
        tick=getCurrentTick() + PHY_DL_DELAY;
        updateHDUeSubFrameCntxtInfo(tick);
    }

#endif
    /* HD FDD Changes End */
#ifdef DL_UL_SPLIT
    putEntryInAckNackQueue(INVALID_UE_ID, 255 ,
            subFrameFrmL2Tick, FAPI_OTHER_CRNTI
            /* +- SPR 17777 */
            );
#endif

    /* TTIB_Code Start */
#ifdef FDD_CONFIG
    if(totalNumberOfTickDiffUl_g[internalCellIndex] > 1)
    {
        freeTTIBundlingNodesForTickMiss(totalNumberOfTickDiffUl_g[internalCellIndex],
                getCurrentTick(),
                PHY_DELAY,internalCellIndex);
    }
#endif
    /* TTIB_Code End */


#ifdef OVERSHOOT_CORRECTION
    modifiedNoOfUEsAfterOverLoadDL(&numOfUEScheduled, internalCellIndex);
#endif

#ifdef TDD_CONFIG

    if(totalNumberOfTickDiffDl_g[internalCellIndex] > 1)
    {
        UInt8 airSf = subFrameFrmL2Tick;
        UInt16 airSfn = sysFrameFrmL2Tick;
        ExecutionLegFunctionArg execLegFuncArg = {0};
        if (MAC_SUCCESS == calculateHarqExpTickMissAndArgs(&execLegFuncArg, ELARG_DL_HARQ_EXPIRY_U_SF,
                    getCurrentTick(), airSf, airSfn, 
                    totalNumberOfTickDiffDl_g[internalCellIndex],
                    /* SPR 11110 fix start */
                    TICK_MISS_SCENARIO ) )
            /* SPR 11110 fix end */
        {
            executionFunctionArrTdd_g[PROCESS_HARQ_TIMER_EXPIRY](&execLegFuncArg);
        }
        if (MAC_SUCCESS == calculateHarqExpTickMissAndArgs(&execLegFuncArg, 
                    ELARG_UL_HARQ_EXPIRY_U_SF,
                    getCurrentTick(), airSf, airSfn, 
                    totalNumberOfTickDiffUl_g[internalCellIndex],
                    /* SPR 11110 fix start */
                    TICK_MISS_SCENARIO ) )
            /* SPR 11110 fix end */
        {
            executionFunctionArrTdd_g[PROCESS_UL_HARQ_TIMER_EXPIRY](&execLegFuncArg);
        }
    }
#endif 

#ifdef DL_UL_SPLIT
    lteMacUpdateStatsPerfStart(DL_OVERSHOOT_STATS,internalCellIndex);
    lteMacUpdateStatsPerfStart(UL_OVERSHOOT_STATS,internalCellIndex);
#else
    lteMacUpdateStatsPerfStart(internalCellIndex);
#endif

}
#endif
/* FAPI L2 AS MASTER END */

#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : waitOnSelectPhyMsg 
 * Inputs         : phyRxFD
 * Outputs        : None
 * Returns        : None 
 * Description    : This function  waits on socket to read message from PHY.
 *****************************************************************************/
 void waitOnSelectPhyMsg(SInt32 phyRxFD)
{
#ifndef MAC_AUT_TEST
    /* Received Data from MSQ or Socket Interface */
    /* Fix for CPU Hogging */
    fd_Set readfd;
    lteFD_ZERO(&readfd);
    lteFD_SET(phyRxFD, &readfd);
    if (-1 == select_wrapper(phyRxFD + 1, &readfd, NULL, NULL, NULL))
    {
        LTE_MAC_UT_LOG(LOG_INFO,RECV_INTF, "2.select failed, errno = %d\n", errno);
    }
    lteFD_CLR(phyRxFD,&readfd);
#endif
}
#endif
/* + CQI_4.1 */
/*****************************************************************************
 * Function Name  : getSubbandNum
 * Inputs         : subbandInfo_p - pointer to the 
 *                                    aperiodicCQIMode22_p->subbandInfo22/
 *                                    aperiodicCQIMode20_p->subbandInfo20
 *                                    to store subband position.
 *                  CombinatorialIndex - position of M selected subbands.
 *                  M  - number of UE preferred  subband.
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : Preferred subband numbers.
 * Returns        : None.
 * Description    : This function is used to find the UE preferred subbands
 *                  with the help combinatorial index using formula defined in
 *                  7.2.1 sec.in  36.213 specification.
 ******************************************************************************/

                    void getSubbandNum(UInt8 *subbandInfo_p,UInt32 combinatorialIndex,UInt8 M,
                            InternalCellIndex internalCellIndex)
{
    UInt8 subbandNum = 1;
    UInt8 tempSubbandNum = 1;
    UInt8 N = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
              cellParams_p->ueSelectedN;
    UInt32 nCm  = 0; /* This will contain Binomial coefficient 
                      *  of N and M from nCm_g */
    UInt8  counter = 0;

    for (counter = 0; counter < M; counter++)
    {
        subbandNum = tempSubbandNum;
        nCm = getBinomialCoefficient((N - subbandNum ),(M - counter),internalCellIndex);

        while (nCm > combinatorialIndex)
        {
            subbandNum = subbandNum + 1;
            nCm = getBinomialCoefficient((N - subbandNum ),(M - counter),internalCellIndex); 
        }

        subbandInfo_p[counter] = subbandNum - 1;

        tempSubbandNum = subbandNum + 1;

        combinatorialIndex = combinatorialIndex - nCm;
    }
}


/*****************************************************************************
 * Function Name  : getCombinatorialIndexAndPMIForRIMoreThan1
 * Inputs         : messageBuffer_p - buffer pointer,
 *                  combinatorialIndex - pointer to be used for storing 
 *                  combinatorialIndex,
 *                  wbPmiValue - to be used for storing PMI value 
 *                  sbPmiValue
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : PMI and combinatorialIndex value.
 * Returns        : None.
 * Description    : This function is used to extract PMI and combinatorialIndex
 *                   value.            
 ******************************************************************************/

void getCombinatorialIndexAndPMIForRIMoreThan1(UInt8  *messageBuffer_p,
        UInt32 *combinatorialIndex,
        UInt8 *wbPmiValue,
        UInt8 *sbPmiValue,
        InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = \
                                     cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt8 L = cellParams_p->LBitLabelForAperiodic;
    UInt32 tempCombIndex = 0;
    UInt8 tempPMIValue = 0;

    switch (L)
    {    
        case 2:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        {
            tempCombIndex = GET_2_BIT_SWAPPED_VALUE(((*messageBuffer_p) & 0x30 )
                    >> 4);
            tempPMIValue = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempCombIndex = GET_2_BIT_SWAPPED_VALUE(((*messageBuffer_p) & 0x30 )
                    >> 4);
            tempPMIValue = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p) << 2;
        }
        break;
        case 3:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        { 
            tempCombIndex = GET_3_BIT_SWAPPED_VALUE(((*messageBuffer_p) & 0x70 )
                    >> 4);
            tempPMIValue = GET_VALUE_FROM_LAST_BIT(*messageBuffer_p); 

            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p) << 1;
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempCombIndex = GET_3_BIT_SWAPPED_VALUE(((*messageBuffer_p) & 0x70 )
                    >> 4);
            tempPMIValue = GET_VALUE_FROM_LAST_BIT(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_7_BITS(*messageBuffer_p) <<1;
        }
        break;
        case 4:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        {  
            tempCombIndex = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            tempCombIndex = GET_4_BIT_SWAPPED_VALUE(tempCombIndex);

            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue = GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p);
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempCombIndex = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            tempCombIndex = GET_4_BIT_SWAPPED_VALUE(tempCombIndex);
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue = GET_VALUE_FROM_FIRST_8_BITS(*messageBuffer_p); 
        }
        break;   
        case 5:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        {
            tempCombIndex = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempCombIndex |= GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p) << 4;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = ((*messageBuffer_p) & 0x06 ) >> 1;
        }
        else if (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempCombIndex = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempCombIndex |= GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p) << 4;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = GET_VALUE_FROM_LAST_7_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p) << 7;
        }
        break;   
        case 6:
        if( cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        { 
            tempCombIndex = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempCombIndex |= GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) << 4;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = ((*messageBuffer_p) & 0x0C ) >> 2;
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempCombIndex = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempCombIndex |= GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) << 4;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = GET_VALUE_FROM_LAST_6_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_2_BITS (*messageBuffer_p) << 6;
        }
        break;
        case 7:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        {
            tempCombIndex = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempCombIndex |= GET_VALUE_FROM_FIRST_3_BITS(*messageBuffer_p) << 4;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = ((*messageBuffer_p) & 0x18 ) >> 3;
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempCombIndex = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempCombIndex |= GET_VALUE_FROM_FIRST_3_BITS(*messageBuffer_p) << 4;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = GET_VALUE_FROM_LAST_5_BITS (*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_3_BITS(*messageBuffer_p) << 5;
        }
        break;
        case 8:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        {
            tempCombIndex = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempCombIndex |= GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p) << 4;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = ((*messageBuffer_p) & 0x30 ) >> 4;
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempCombIndex = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempCombIndex |= GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p) << 4;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p) << 4;
        }
        break;  
        case 9:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        {
            tempCombIndex = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempCombIndex |= GET_VALUE_FROM_FIRST_5_BITS(*messageBuffer_p) << 4;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = ((*messageBuffer_p) & 0x60 ) >> 5;
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempCombIndex= GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempCombIndex |= GET_VALUE_FROM_FIRST_5_BITS(*messageBuffer_p) << 4;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = GET_VALUE_FROM_LAST_3_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_5_BITS (*messageBuffer_p) << 3;
        }
        break;
        case 10:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        {
            tempCombIndex = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempCombIndex |= GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p) << 4;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempCombIndex = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempCombIndex |= GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p) << 4;
            tempCombIndex =  getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p) <<2;
        }
        break;
        case 11:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        {
            tempCombIndex = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempCombIndex |= GET_VALUE_FROM_FIRST_7_BITS(*messageBuffer_p) << 4;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = GET_VALUE_FROM_LAST_BIT(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p) << 1;
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempCombIndex = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempCombIndex |= GET_VALUE_FROM_FIRST_7_BITS(*messageBuffer_p) << 4;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = GET_VALUE_FROM_LAST_BIT(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_7_BITS(*messageBuffer_p) << 1; 
        }
        break;
        case 12:

        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        {
            GET_VALUE_FROM_FIRST_12_BITS(messageBuffer_p,tempCombIndex);
            /* pointer already incremented in the macro */
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p);
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            GET_VALUE_FROM_FIRST_12_BITS(messageBuffer_p,tempCombIndex);
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = GET_VALUE_FROM_FIRST_8_BITS(*messageBuffer_p);
        }
        break;  
        case 13:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        {
            GET_VALUE_FROM_FIRST_12_BITS(messageBuffer_p,tempCombIndex);
            tempCombIndex |= GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p) << 12;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = ((*messageBuffer_p) & 0x06 ) >> 1;
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            GET_VALUE_FROM_FIRST_12_BITS(messageBuffer_p,tempCombIndex);
            tempCombIndex |= GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p) << 12;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = GET_VALUE_FROM_LAST_7_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p +1;
            tempPMIValue |=GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p) << 7;
        }
        /* + coverity 54531 */
        break;
        /* - coverity 54531 */
        case 14:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        {
            GET_VALUE_FROM_FIRST_12_BITS(messageBuffer_p,tempCombIndex);
            tempCombIndex |= GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) << 12;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = ((*messageBuffer_p) & 0x0C ) >> 2;
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            GET_VALUE_FROM_FIRST_12_BITS(messageBuffer_p,tempCombIndex);
            tempCombIndex |= GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) << 12;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = GET_VALUE_FROM_LAST_6_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p +1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) <<6;
        }
        break;
        case 15:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        {
            GET_VALUE_FROM_FIRST_12_BITS(messageBuffer_p,tempCombIndex);
            tempCombIndex |= GET_VALUE_FROM_FIRST_3_BITS(*messageBuffer_p) << 12;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = ((*messageBuffer_p) & 0x18 ) >> 3;
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            GET_VALUE_FROM_FIRST_12_BITS(messageBuffer_p,tempCombIndex);
            tempCombIndex |= GET_VALUE_FROM_FIRST_3_BITS(*messageBuffer_p) << 12;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = GET_VALUE_FROM_LAST_5_BITS (*messageBuffer_p);
            messageBuffer_p = messageBuffer_p +1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_3_BITS(*messageBuffer_p) <<5;
        }
        break; 
        case 16:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        {
            GET_VALUE_FROM_FIRST_12_BITS(messageBuffer_p,tempCombIndex);
            tempCombIndex |= GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p) << 12;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = ((*messageBuffer_p) & 0x30 ) >> 4;
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            GET_VALUE_FROM_FIRST_12_BITS(messageBuffer_p,tempCombIndex);
            tempCombIndex |= GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p) << 12;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p +1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
        }
        break; 
        case 17:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        {
            GET_VALUE_FROM_FIRST_12_BITS(messageBuffer_p,tempCombIndex);
            tempCombIndex |= GET_VALUE_FROM_FIRST_5_BITS(*messageBuffer_p) << 12;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = ((*messageBuffer_p) & 0x60 ) >> 5;
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            GET_VALUE_FROM_FIRST_12_BITS(messageBuffer_p,tempCombIndex);
            tempCombIndex |= GET_VALUE_FROM_FIRST_5_BITS(*messageBuffer_p) << 12;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = GET_VALUE_FROM_LAST_3_BITS (*messageBuffer_p);
            messageBuffer_p = messageBuffer_p +1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_5_BITS(*messageBuffer_p) << 3;
        }
        break;
        case 18:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
        {
            GET_VALUE_FROM_FIRST_12_BITS(messageBuffer_p,tempCombIndex);
            tempCombIndex |= GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p) << 12;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = ((*messageBuffer_p) & 0xC0 ) >> 6;
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            GET_VALUE_FROM_FIRST_12_BITS(messageBuffer_p,tempCombIndex);
            tempCombIndex |= GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p) << 12;
            tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);

            tempPMIValue = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p +1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_6_BITS (*messageBuffer_p) << 2;
        }
        break;
        default:
        break;
    }
    *combinatorialIndex = tempCombIndex;
    if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
    {
        *wbPmiValue = (tempPMIValue & 0x01);
        *sbPmiValue = ((tempPMIValue & 0x02) >> 1);
    }
    else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
    {
        *wbPmiValue = (tempPMIValue & 0x0F);
        *sbPmiValue = ((tempPMIValue & 0xF0) >> 4);
    }
    else
    {
        /*failure*/
    }

    return;
}

/* + CQI_5.0 */
/*****************************************************************************
 * Function Name  : getWBPMIForRIMoreThan1
 * Inputs         : messageBuffer_p - buffer pointer
 *                  wbPmiValue - to be used for storing WB PMI value
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : PMI value.
 * Returns        : None.
 * Description    : This function is used to extract WB PMI.            
 ******************************************************************************/

void getWBPMIForRIMoreThan1(UInt8  *messageBuffer_p,
        UInt8 *wbPmiValue,InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt8 L = cellParams_p->LBitLabelForAperiodic;
    UInt8 tempPMIValue = 0;

    switch (L)
    {    
        case 2:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempPMIValue = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p)<< 2; 
        }
        else
            tempPMIValue = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
        break;
        case 3:
        case 11:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempPMIValue = GET_VALUE_FROM_LAST_BIT(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p)<< 1; 
        }
        else
        {
            tempPMIValue = GET_VALUE_FROM_LAST_BIT(*messageBuffer_p); 
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p) << 1;
        }
        break;
        case 4:
        case 12:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempPMIValue = *messageBuffer_p;
        }
        else
        {
            tempPMIValue = GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p);
        }
        break;   
        case 5:
        case 13:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempPMIValue = ((*messageBuffer_p) & 0xFE) >> 1;
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p) << 7;
        }
        else
        {
            tempPMIValue = ((*messageBuffer_p) & 0x06 ) >> 1;
            break;   
        }
        case 6:
        case 14:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempPMIValue = ((*messageBuffer_p) & 0xFC) >> 2;
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) << 6;
        }
        else
        {
            tempPMIValue = ((*messageBuffer_p) & 0x0C ) >> 2;
        }
        break;
        case 7:
        case 15:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempPMIValue = ((*messageBuffer_p) & 0xF8) >> 3;
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_3_BITS(*messageBuffer_p) << 5;
        }
        else
        {
            tempPMIValue = ((*messageBuffer_p) & 0x18 ) >> 3;
        }
        break;
        case 8:
        case 16:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempPMIValue = ((*messageBuffer_p) & 0xF0) >> 4;
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= ((*messageBuffer_p) & 0x0F) << 4;
        }
        else
        {
            tempPMIValue = ((*messageBuffer_p) & 0x30 ) >> 4;
        }
        break;  
        case 9:
        case 17:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempPMIValue = GET_VALUE_FROM_LAST_3_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_5_BITS(*messageBuffer_p) << 3;
        }
        else
        {
            tempPMIValue = ((*messageBuffer_p) & 0x60 ) >> 5;
        }
        break;
        case 10:
        case 18:
        if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            tempPMIValue = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempPMIValue |= GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p) << 2;
        }
        else
        {
            tempPMIValue = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
        }
        break;
        default:
        break;
    }
    if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
    {
        *wbPmiValue = GET_4_BIT_SWAPPED_VALUE(tempPMIValue & 0x0F);
    }
    else
        *wbPmiValue = (tempPMIValue & 0x01);

    return;
}
/* - CQI_5.0 */
/*****************************************************************************
 * Function Name  : getCombinatorialIndexFromL
 * Inputs         : messageBuffer_p - buffer pointer
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : None 
 * Returns        : CombinatorialIndex value.
 * Description    : This function is used to extract combinatorialIndex value.            
 ******************************************************************************/

STATIC UInt32 getCombinatorialIndexFromL(UInt8  *messageBuffer_p,
        InternalCellIndex internalCellIndex)
{

    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    UInt32 tempCombIndex = 0;
    UInt8 L =  cellParams_p->LBitLabelForAperiodic;

    switch (L)
    {    
        case 2:

        tempCombIndex = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
        tempCombIndex = GET_2_BIT_SWAPPED_VALUE(tempCombIndex);
        break;

        case 3: 

        tempCombIndex = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
        messageBuffer_p = messageBuffer_p + 1;
        tempCombIndex |= GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p) << 2;
        tempCombIndex = GET_3_BIT_SWAPPED_VALUE(tempCombIndex);

        break;

        case 4:

        tempCombIndex = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
        messageBuffer_p = messageBuffer_p + 1;
        tempCombIndex |= GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) << 2;
        tempCombIndex = GET_4_BIT_SWAPPED_VALUE(tempCombIndex);
        break;


        case 5:

        tempCombIndex = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
        messageBuffer_p = messageBuffer_p + 1;
        tempCombIndex |= GET_VALUE_FROM_FIRST_3_BITS(*messageBuffer_p) << 2;
        tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);
        break;

        case 6:

        tempCombIndex = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
        messageBuffer_p = messageBuffer_p + 1;
        tempCombIndex |= GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p) << 2;
        tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);
        break;    

        case 7:

        tempCombIndex = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
        messageBuffer_p = messageBuffer_p + 1;
        tempCombIndex |= GET_VALUE_FROM_FIRST_5_BITS(*messageBuffer_p) << 2;
        tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);
        break;

        case 8:

        tempCombIndex = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
        messageBuffer_p = messageBuffer_p + 1;
        tempCombIndex |= GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p) << 2;
        tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);
        break;

        case 9:

        tempCombIndex = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
        messageBuffer_p = messageBuffer_p + 1;
        tempCombIndex |= GET_VALUE_FROM_FIRST_7_BITS(*messageBuffer_p) << 2;
        tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);
        break;

        case 10:

        GET_VALUE_FROM_FIRST_10_BITS(messageBuffer_p,tempCombIndex);
        tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);
        break;

        case 11:

        GET_VALUE_FROM_FIRST_10_BITS(messageBuffer_p,tempCombIndex);
        tempCombIndex |= GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p) << 10;
        tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);
        break;

        case 12:

        GET_VALUE_FROM_FIRST_10_BITS(messageBuffer_p,tempCombIndex);
        tempCombIndex |= GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) << 10;
        tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);
        break;

        case 13:

        GET_VALUE_FROM_FIRST_10_BITS(messageBuffer_p,tempCombIndex);
        tempCombIndex |= GET_VALUE_FROM_FIRST_3_BITS(*messageBuffer_p) << 10;
        tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);
        break;

        case 14:

        GET_VALUE_FROM_FIRST_10_BITS(messageBuffer_p,tempCombIndex);
        tempCombIndex |= GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p) << 10;
        tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);
        break;

        case 15:

        GET_VALUE_FROM_FIRST_10_BITS(messageBuffer_p,tempCombIndex);
        tempCombIndex |= GET_VALUE_FROM_FIRST_5_BITS(*messageBuffer_p) << 10;
        tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);
        break;

        case 16:

        GET_VALUE_FROM_FIRST_10_BITS(messageBuffer_p,tempCombIndex);
        tempCombIndex |= GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p) << 10;
        tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);
        break;

        case 17:

        GET_VALUE_FROM_FIRST_10_BITS(messageBuffer_p,tempCombIndex);
        tempCombIndex |= GET_VALUE_FROM_FIRST_7_BITS(*messageBuffer_p) << 10;
        tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);
        break;

        case 18:

        GET_VALUE_FROM_FIRST_10_BITS(messageBuffer_p,tempCombIndex);
        tempCombIndex |= *messageBuffer_p << 10;
        tempCombIndex = getSwappedCombinatorialIndex(L,tempCombIndex);
        break;

        default:

        break;

    }        
    return tempCombIndex;

}

/*****************************************************************************
 * Function Name  : getPMIForAperiodicMode22RI1
 * Inputs         : messageBuffer_p - buffer pointer
 *                  wbPmiValue ,sbPmiValue,
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : None 
 * Returns        : CombinatorialIndex value. 
 * Description    : This function is used to extract PMI value in 
 *                  AperiodicMode22   for RI = 1         
 ******************************************************************************/

STATIC void getPMIForAperiodicMode22RI1(UInt8  *messageBuffer_p,
        UInt8 *wbPmiValue,
        UInt8 *sbPmiValue,
        InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    /* Cyclomatic_complexity_changes_start */
    if((cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_1)||\
            ((cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)))
    {
        getPMIForAperiodicMode22RI1NumTxAntenna1Or2(messageBuffer_p,
                wbPmiValue,
                sbPmiValue,
                cellParams_p);
    }
    else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
    {
        getPMIForAperiodicMode22RI1NumTxAntenna4(messageBuffer_p,
                wbPmiValue,
                sbPmiValue,
                cellParams_p);
    }
    else
    {
        /* Do nothing */
    }
    /* Cyclomatic_complexity_changes_end */

    return;
}

/* + CQI_5.0 */
/*****************************************************************************
 * Function Name  : getWBPMIForAperiodicMode22RI1
 * Inputs         : messageBuffer_p - buffer pointer and wbPmiValue,
 *                  wbPmiValue,
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : None 
 * Returns        : CombinatorialIndex value. 
 * Description    : This function is used to extract PMI value in 
 *                  AperiodicMode22   for RI = 1         
 ******************************************************************************/
STATIC void getWBPMIForAperiodicMode22RI1(UInt8 *messageBuffer_p,
        UInt8 *wbPmiValue,
        InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    /* Cyclomatic_complexity_changes_start */
    if((cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_1)||\
            (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2))
    {
        getWBPMIForAperiodicMode22RI1NumTxAntenna1Or2(messageBuffer_p,
                wbPmiValue,
                cellParams_p);
    }
    else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
    {
        getWBPMIForAperiodicMode22RI1NumTxAntenna4(messageBuffer_p,
                wbPmiValue,
                cellParams_p);
    }
    else
    {
    }

    return;
    /* Cyclomatic_complexity_changes_end */
}
/* - CQI_5.0 */

/*****************************************************************************
 * Function Name  : getSwappedCombinatorialIndex 
 * Inputs         : L - bits to be used for combinatorial index
 *                  tempCombinatorialIndex - extracted CombinatorialIndex value 
 * Outputs        : None 
 * Returns        : Swapped CombinatorialIndex value.
 * Description    : This function is used to swap CombinatorialIndex value in 
 *                  AperiodicMode22      
 ******************************************************************************/

STATIC UInt32 getSwappedCombinatorialIndex(UInt8 L,UInt32 tempCombinatorialIndex)
{

    UInt32 swappedCombinatorialIndex = 0;
    UInt32  tempCombinatorialIndex1 = 0;
    UInt32  tempCombinatorialIndex2 = 0;
    UInt32  tempSwapped1            = 0;
    UInt32  tempSwapped2            = 0;
    UInt8  count                   = 0;
    
    if(L % 2 != 0)
    {
        /* SPR 18122 Changes Start*/
        swappedCombinatorialIndex = tempCombinatorialIndex & (1 << (DIVIDE_BY_TWO(L)));
    }

    for(count = 0 ; count < (DIVIDE_BY_TWO(L)) ; count++ )
        /* SPR 18122 Changes End*/
    {
        tempCombinatorialIndex1 = tempCombinatorialIndex & (1 << count);

        tempCombinatorialIndex2 = 
            tempCombinatorialIndex & ( 1 << ((L - count) - 1));

        tempSwapped1 = tempCombinatorialIndex1 << ((L - 2*count) - 1);
        tempSwapped2 = tempCombinatorialIndex2 >> ((L - 2*count) - 1);

        swappedCombinatorialIndex |= (tempSwapped1 | tempSwapped2);
    }

    return swappedCombinatorialIndex;
}


/*****************************************************************************
 * Function Name  : getSubbandDifferentialCqiCW0 
 * Inputs         : messageBuffer_p - buffer pointer
 *                  numberOfBytes - number of bytes to be moved
 *                  numberOfBits - number of bits to be moved
 * Outputs        : Subband differential CQI
 * Returns        : Subband differential CQI
 * Description    : This function is used to extract Subband differential CQI
 *                  for codeword 0 in AperiodicMode 3_0 and 3_1      
 ******************************************************************************/
STATIC UInt32 getSubbandDifferentialCqiCW0(UInt8 *messageBuffer_p,
        UInt8 numberOfBytes, 
        UInt8 numberOfBits)
{

    UInt32 spatialDifferentialCqiForCW0 = 0;
    UInt32 tempSpatialDiffForCW0 = 0;
    spatialDifferentialCqiForCW0 = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
    messageBuffer_p = messageBuffer_p + 1;


    if(0 == numberOfBytes)
    {
        switch(numberOfBits)
        {

            case 2 :
            spatialDifferentialCqiForCW0 |= 
                GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) << 4;
            break;

            case 4 :
            spatialDifferentialCqiForCW0 |=
                GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p) << 4;    
            break;

            case 6 : 
            spatialDifferentialCqiForCW0 |=
                GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p) << 4;
            break;

        }
    }

    else if(1 == numberOfBytes)
    {

        spatialDifferentialCqiForCW0 |= *messageBuffer_p << 4;
        messageBuffer_p = messageBuffer_p + 1;

        switch(numberOfBits)
        {

            case 2 :
            spatialDifferentialCqiForCW0 |= 
                GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) << 12;
            break;

            case 4 :
            spatialDifferentialCqiForCW0 |=
                GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p) << 12;    
            break;

            case 6 : 
            spatialDifferentialCqiForCW0 |=
                GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p) << 12;
            break;

        }
    }

    else if(2 == numberOfBytes)
    {

        GET_VALUE_FROM_FIRST_16_BITS(messageBuffer_p,tempSpatialDiffForCW0);
        spatialDifferentialCqiForCW0 |= tempSpatialDiffForCW0 << 4;

        switch(numberOfBits)
        {

            case 2 :
            spatialDifferentialCqiForCW0 |= 
                GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) << 20;
            break;

            case 4 :
            spatialDifferentialCqiForCW0 |=
                GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p) << 20;    
            break;

            case 6 : 
            spatialDifferentialCqiForCW0 |=
                GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p) << 20;
            break;

        }  
    }
    return spatialDifferentialCqiForCW0;
}


/*****************************************************************************
 * Function Name  : getSubbandDiffCqiWidebandCqiPmiCW1Mode3_1
 * Inputs         : messageBuffer_p - buffer pointer,
 *                  numberOfBytes - number of bytes to be moved,
 *                  numberOfBits - number of bits to be moved,
 *                  widebandCQIValue1, pmiValue,spatialDifferentialCqiForCW1
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : WidebandCQI for codeword 1
 *                  Subband differential CQI for codeword 1
 *                  PMI value
 * Returns        : None
 * Description    : This function is used to extract Wideband CQI,Subband 
 *                  differential CQI for codeword 1 and PMI in AperiodicMode
 *                  3_1 when rank > 1     
 ******************************************************************************/

STATIC void getSubbandDiffCqiWidebandCqiPmiCW1Mode3_1(UInt8 *messageBuffer_p, 
        UInt8 numberOfBytes, 
        UInt8 numberOfBits, 
        UInt8 *widebandCQIValue1,
        UInt8 *pmiValue, 
        UInt32 *spatialDifferentialCqiForCW1,
        InternalCellIndex internalCellIndex)

{
    UInt32 spatialDiffCqiForCW1 = 0;
    UInt32 tempSpatialDiffForCW1 = 0; 
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    if(0 == numberOfBytes)
    {
        switch(numberOfBits)
        {
            case 0 :
            *widebandCQIValue1 =
                GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);

            spatialDiffCqiForCW1 = 
                GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);

            messageBuffer_p = messageBuffer_p + 1;
            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p);
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;

            case 2 : 
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_MIDDLE_4_BITS(
                    *messageBuffer_p);
            spatialDiffCqiForCW1 = 
                GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            spatialDiffCqiForCW1 |= 
                GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p) << 2;

            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = ((*messageBuffer_p) & 0x10) >> 4;
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;

            case 4 :
            *widebandCQIValue1 = 
                GET_WIDEBAND_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);

            messageBuffer_p = messageBuffer_p + 1;
            spatialDiffCqiForCW1 = *messageBuffer_p ;

            messageBuffer_p = messageBuffer_p + 1;
            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p);
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_4_BITS (*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;

            case 6 :
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_LAST_2_BITS(
                    *messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            *widebandCQIValue1 |= GET_WIDEBAND_VALUE_FROM_FIRST_2_BITS(
                    *messageBuffer_p) << 2;

            spatialDiffCqiForCW1 = 
                GET_VALUE_FROM_LAST_6_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            spatialDiffCqiForCW1 |= 
                GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p) << 6;

            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = ((*messageBuffer_p) & 0x10) >> 4;
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;
        }
    }
    else if(1 == numberOfBytes)
    {
        switch(numberOfBits)
        {
            case 0 :
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(
                    *messageBuffer_p);

            GET_VALUE_FROM_FIRST_12_BITS(messageBuffer_p,spatialDiffCqiForCW1);

            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p); 
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;
            case 2 : 
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_MIDDLE_4_BITS(
                    *messageBuffer_p);

            GET_VALUE_FROM_FIRST_10_BITS(messageBuffer_p,
                    spatialDiffCqiForCW1);
            spatialDiffCqiForCW1 |= GET_VALUE_FROM_FIRST_4_BITS(
                    *messageBuffer_p) << 10;
            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = ((*messageBuffer_p) & 0x10) >> 4;
            }
            else if (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;
            case 4 :
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_LAST_4_BITS(
                    *messageBuffer_p);

            messageBuffer_p = messageBuffer_p + 1;
            GET_VALUE_FROM_FIRST_16_BITS(messageBuffer_p,
                    spatialDiffCqiForCW1);

            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p);
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;
            case 6 :
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_LAST_2_BITS(
                    *messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            *widebandCQIValue1 |= GET_WIDEBAND_VALUE_FROM_FIRST_2_BITS(
                    *messageBuffer_p) << 2;

            spatialDiffCqiForCW1 = 
                GET_VALUE_FROM_LAST_6_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            spatialDiffCqiForCW1 |= (*messageBuffer_p) << 6;
            messageBuffer_p = messageBuffer_p + 1;
            spatialDiffCqiForCW1 |= GET_VALUE_FROM_FIRST_4_BITS(
                    *messageBuffer_p) << 14;

            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = ((*messageBuffer_p) & 0x10) >> 4;
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue  = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }

            break;
        }
    }
    else if(2 == numberOfBytes)
    {
        switch(numberOfBits)
        {
            case 0 :
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(
                    *messageBuffer_p);

            spatialDiffCqiForCW1 = 
                GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            GET_VALUE_FROM_FIRST_16_BITS(messageBuffer_p,
                    tempSpatialDiffForCW1);
            spatialDiffCqiForCW1 |= tempSpatialDiffForCW1 << 4;

            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p);
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }

            break;
            case 2 : 
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_MIDDLE_4_BITS(
                    *messageBuffer_p);

            spatialDiffCqiForCW1 = 
                GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            GET_VALUE_FROM_FIRST_16_BITS(messageBuffer_p,
                    tempSpatialDiffForCW1);
            spatialDiffCqiForCW1 |= tempSpatialDiffForCW1 << 2;
            spatialDiffCqiForCW1 |= 
                GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p) << 18;


            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = ((*messageBuffer_p) & 0x10) >> 4;
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;
            case 4 :
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_LAST_4_BITS(
                    *messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;

            GET_VALUE_FROM_FIRST_16_BITS(messageBuffer_p,
                    spatialDiffCqiForCW1);
            spatialDiffCqiForCW1 |= *messageBuffer_p << 16;

            messageBuffer_p = messageBuffer_p + 1;

            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p);
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;
            case 6 :
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_LAST_2_BITS(
                    *messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            *widebandCQIValue1 |= GET_WIDEBAND_VALUE_FROM_FIRST_2_BITS(
                    *messageBuffer_p) << 2;

            spatialDiffCqiForCW1 = 
                GET_VALUE_FROM_LAST_6_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            GET_VALUE_FROM_FIRST_16_BITS(messageBuffer_p,
                    tempSpatialDiffForCW1);
            spatialDiffCqiForCW1 |= tempSpatialDiffForCW1 << 6;
            spatialDiffCqiForCW1 |=
                GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p) << 22;

            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = ((*messageBuffer_p) & 0x10) >> 4;
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;
        }
    }
    *spatialDifferentialCqiForCW1 = spatialDiffCqiForCW1;
}

/* + CQI_5.0 */
/*****************************************************************************
 * Function Name  : getWidebandCqiPmiCW1Mode3_1 
 * Inputs         : messageBuffer_p - buffer pointer,
 *                  numberOfBytes - number of bytes to be moved,
 *                  numberOfBits - number of bits to be moved,
 *                  widebandCQIValue1 , pmiValue
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : WidebandCQI for codeword 1
 *                  Subband differential CQI for codeword 1
 *                  PMI value
 * Returns        : None
 * Description    : This function is used to extract Wideband CQI,Subband 
 *                  differential CQI for codeword 1 and PMI in AperiodicMode
 *                  3_1 when rank > 1     
 ******************************************************************************/
STATIC void getWidebandCqiPmiCW1Mode3_1(UInt8 *messageBuffer_p, 
        UInt8 numberOfBytes, 
        UInt8 numberOfBits, 
        UInt8 *widebandCQIValue1,
        UInt8 *pmiValue,
        InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    if(0 == numberOfBytes)
    {
        switch(numberOfBits)
        {
            case 0 :
            *widebandCQIValue1 =
                GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p); 
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;

            case 2 : 
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_MIDDLE_4_BITS(
                    *messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = ((*messageBuffer_p) & 0x10) >> 4;
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;

            case 4 :
            *widebandCQIValue1 = 
                GET_WIDEBAND_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);

            messageBuffer_p = messageBuffer_p + 1;
            messageBuffer_p = messageBuffer_p + 1;
            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p);
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;

            case 6 :
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_LAST_2_BITS(
                    *messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            *widebandCQIValue1 |= GET_WIDEBAND_VALUE_FROM_FIRST_2_BITS(
                    *messageBuffer_p) << 2;
            messageBuffer_p = messageBuffer_p + 1;
            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = ((*messageBuffer_p) & 0x10) >> 4;
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;
        }
    }
    else if(1 == numberOfBytes)
    {

        switch(numberOfBits)
        {

            case 0 :
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(
                    *messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            messageBuffer_p = messageBuffer_p + 1;
            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p); 
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;

            case 2 : 
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_MIDDLE_4_BITS(
                    *messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            messageBuffer_p = messageBuffer_p + 1;
            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = ((*messageBuffer_p) & 0x10) >> 4;
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;

            case 4 :
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_LAST_4_BITS(
                    *messageBuffer_p);

            messageBuffer_p = messageBuffer_p + 1;
            messageBuffer_p = messageBuffer_p + 1;
            messageBuffer_p = messageBuffer_p + 1;
            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p);
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_4_BITS (*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;

            case 6 :
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_LAST_2_BITS(
                    *messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            *widebandCQIValue1 |= GET_WIDEBAND_VALUE_FROM_FIRST_2_BITS(
                    *messageBuffer_p) << 2;
            messageBuffer_p = messageBuffer_p + 1;
            messageBuffer_p = messageBuffer_p + 1;
            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = ((*messageBuffer_p) & 0x10) >> 4;
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;
        }
    }
    else if(2 == numberOfBytes)
    {

        switch(numberOfBits)
        {

            case 0 :
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(
                    *messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            messageBuffer_p = messageBuffer_p + 1;
            messageBuffer_p = messageBuffer_p + 1;
            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p); 
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }

            break;

            case 2 : 
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_MIDDLE_4_BITS(
                    *messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            messageBuffer_p = messageBuffer_p + 1;
            messageBuffer_p = messageBuffer_p + 1;
            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = ((*messageBuffer_p) & 0x10) >> 4;
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;

            case 4 :
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_LAST_4_BITS(
                    *messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            messageBuffer_p = messageBuffer_p + 1;
            messageBuffer_p = messageBuffer_p + 1;
            messageBuffer_p = messageBuffer_p + 1;
            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p);
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;

            case 6 :
            *widebandCQIValue1 = GET_WIDEBAND_VALUE_FROM_LAST_2_BITS(
                    *messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            *widebandCQIValue1 |= GET_WIDEBAND_VALUE_FROM_FIRST_2_BITS(
                    *messageBuffer_p) << 2;
            messageBuffer_p = messageBuffer_p + 1;
            messageBuffer_p = messageBuffer_p + 1;
            messageBuffer_p = messageBuffer_p + 1;
            if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2)
            {
                *pmiValue = ((*messageBuffer_p) & 0x10) >> 4;
            }
            else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
            {
                *pmiValue = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
                *pmiValue = GET_4_BIT_SWAPPED_VALUE(*pmiValue);
            }
            break;
        }
    }
}
/* - CQI_5.0 */

/*****************************************************************************
 * Function Name  : getSubbandCqiValue 
 * Inputs         : spatialDifferentialCqi - extracted subband differential CQI
 *                  ueReportedCQINodeInfo_p - pointer to UEReportedCQINodeInfo.
 *                  numberOfSubBands,
 *                  widebandCQIValue
 * Outputs        : Subband CQI value for each subband
 * Returns        : MacRetType
 * Description    : This function is used to obtain subband CQI value for each
 *                  Subband from the extracted subband differential CQI in
 *                  Aperidoc Mode 3_0 when ri = 1     
 ******************************************************************************/
STATIC MacRetType  getSubbandCqiValue(UInt32 spatialDifferentialCqi, 
        UEReportedCQINodeInfo *ueReportedCQINodeInfo_p)
{
    UInt8 tempSpatialDiffCqi = 0;
    UInt8 cqiValue = 0;
    UInt8 offsetLevel = 0;
    /* + Coverity 20386 */
    if (ueReportedCQINodeInfo_p)
    {
        AperiodicCQIMode30 *aperiodicCQIMode30_p = 
            &(ueReportedCQINodeInfo_p->cqiNode.aperiodicCQIMode30);

        UInt8 widebandCQIValue = aperiodicCQIMode30_p->widebandCQIValue;
        UInt8 numberOfSubBands = aperiodicCQIMode30_p->numberOfSubbands; 
        UInt8 *subbandCqiInfo_p = aperiodicCQIMode30_p->subbandCQIInfo30;

        while (numberOfSubBands--)
        {
            tempSpatialDiffCqi = GET_2_BIT_SWAPPED_VALUE(
                    spatialDifferentialCqi & 0x03);   
            offsetLevel = 
                offsetLevelForHigherLayerConfiguredMode_g[tempSpatialDiffCqi];

            /* Subband differential CQI offset level = 
             *                 subband CQI index - wideband CQI index.
             * specs 36.213 ,sec 7.2 
             */
            cqiValue = (offsetLevel + widebandCQIValue) - DL_ADAPTATION_CORRECTION;

#ifdef PERF_STATS
            if (cqiValue < MAX_CQI_INDEX)
            {
                gMacUePerfStats_p[ueReportedCQINodeInfo_p->ueIndex].lteUeMacULPerfStats.\
                    totalCQICW1Reported[cqiValue]++;
            } 
#endif
            /* SPR 4991 changes start */
            if ((MAX_CQI_INDEX <= cqiValue) || (INVALID_CQI_INDEX >= cqiValue))
            {
                freeMemPool(ueReportedCQINodeInfo_p);
                return MAC_FAILURE;
            }
            /* SPR 4991 changes end */

            *subbandCqiInfo_p = cqiValue;
            subbandCqiInfo_p++;
            spatialDifferentialCqi = spatialDifferentialCqi >> 2;

        }
        /* + SPR_4991 */
        return MAC_SUCCESS;
        /* - SPR_4991 */
    }
    else
    {
        return MAC_FAILURE; 
    }
    /* - Coverity 20386 */
}

/*****************************************************************************
 * Function Name  : getSubbandCqiValueMode31 
 * Inputs         : spatialDifferentialCqiCW0 -subband differential CQI for CW0,
 *                  spatialDifferentialCqiCW1 -subband differential CQI for CW1,
 *                  ueReportedCQINodeInfo_p - pointer to UEReportedCQINodeInfo
 *                  ri - rank Indicator.  
 * Outputs        : Subband CQI value for each subband
 * Returns        : MacRetType
 * Description    : This function is used to obtain subband CQI value for each
 *                  Subband from the extracted subband differential CQI for 
 *                  both codewords in Aperidoc Mode 3_1 when ri > 1    
 ******************************************************************************/
       STATIC MacRetType getSubbandCqiValueMode31(UInt32 spatialDifferentialCqiCW0,
               UInt32 spatialDifferentialCqiCW1,
               UEReportedCQINodeInfo *ueReportedCQINodeInfo_p,
               UInt8 ri
#ifdef PERF_STATS
               ,UInt8 transmissionMode
#endif
               )
{
    UInt8 spatialDiffCqiCW = 0;
    UInt8 cqiValue = 0;
    UInt8 offsetLevel = 0;
    /* + Coverity 20387 */
    if (ueReportedCQINodeInfo_p) 
    {
        AperiodicCQIMode31 *aperiodicCQIMode31_p = 
            &(ueReportedCQINodeInfo_p->cqiNode.aperiodicCQIMode31);

        UInt8 widebandCQIValueCW0 = aperiodicCQIMode31_p->widebandCQIValueCodeword0;
        UInt8 widebandCQIValueCW1 = aperiodicCQIMode31_p->widebandCQIValueCodeword1;

        UInt8 numberOfSubBands = aperiodicCQIMode31_p->numberOfSubbands; 
        SubBandInfo31 *subbandInfo_p = aperiodicCQIMode31_p->subbandInfo31;

        while ( numberOfSubBands-- )
        {
            spatialDiffCqiCW = GET_2_BIT_SWAPPED_VALUE( 
                    spatialDifferentialCqiCW0 & 0x03);   

            /* Subband differential CQI offset level = 
             *  subband CQI index - wideband CQI index.
             * specs 36.213 ,sec 7.2 
             */

            offsetLevel = 
                offsetLevelForHigherLayerConfiguredMode_g[spatialDiffCqiCW];

            cqiValue = (offsetLevel + widebandCQIValueCW0) 
                - DL_ADAPTATION_CORRECTION;
#ifdef PERF_STATS
            if (cqiValue < MAX_CQI_INDEX)
            {
                gMacUePerfStats_p[ueReportedCQINodeInfo_p->ueIndex].lteUeMacULPerfStats.\
                    totalCQICW1Reported[cqiValue]++;
            } 
#endif
            /* SPR 4991 changes start */
            if ((MAX_CQI_INDEX <= cqiValue) || (INVALID_CQI_INDEX >= cqiValue))
            {
                freeMemPool(ueReportedCQINodeInfo_p);
                return MAC_FAILURE;
            } 

            /* SPR 4991 changes end */

            subbandInfo_p->subbandCQIValueCodeword0 = cqiValue;

            spatialDifferentialCqiCW0 = spatialDifferentialCqiCW0 >> 2;

            if (1 < ri)
            {

                spatialDiffCqiCW = GET_2_BIT_SWAPPED_VALUE(
                        spatialDifferentialCqiCW1 & 0x03);   
                offsetLevel = 
                    offsetLevelForHigherLayerConfiguredMode_g[spatialDiffCqiCW];

                /* Subband differential CQI offset level = 
                 *                 subband CQI index - wideband CQI index.
                 * specs 36.213 ,sec 7.2 
                 */
                cqiValue = (offsetLevel + widebandCQIValueCW1)
                    - DL_ADAPTATION_CORRECTION;

#ifdef PERF_STATS
                if (((TX_MODE_4 == transmissionMode) || (TX_MODE_8 == transmissionMode)) &&
                        (cqiValue < MAX_CQI_INDEX))
                {
                    gMacUePerfStats_p[ueReportedCQINodeInfo_p->ueIndex].lteUeMacULPerfStats.\
                        totalCQICW2Reported[cqiValue]++; 
                }
#endif

                /* SPR 4991 changes start */
                if ((MAX_CQI_INDEX <= cqiValue) || (INVALID_CQI_INDEX >= cqiValue))
                {
                    lteWarning("Invalid CQI [ %d] received for CW1 in [%s] on line num"
                            "[ %d] ,Offset value = %d , WBCQIValue1 = %d \n",
                            cqiValue,__func__,__LINE__,offsetLevel,widebandCQIValueCW1);

                    freeMemPool(ueReportedCQINodeInfo_p);
                    return MAC_FAILURE;
                }    
                /* SPR 4991 changes end */

                subbandInfo_p->subbandCQIValueCodeword1 = cqiValue;
                spatialDifferentialCqiCW1 = spatialDifferentialCqiCW1 >> 2;
            }
            else
            {
                subbandInfo_p->subbandCQIValueCodeword1 = INVALID_CQI_INDEX;
            }

            subbandInfo_p++;
        }

        /* + SPR_4991 */
        return MAC_SUCCESS;
        /* - SPR_4991 */
    }
    else 
    {
        return MAC_FAILURE;
    } 
    /* - Coverity 20387 */
}

/*****************************************************************************
 * Function Name  : getPMIValueCW0ForMode31 
 * Inputs         : messageBuffer_p - buffer pointer
 *                  numberOfBits - number of bits to be moved
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : PMI value
 * Returns        : PMI Value
 * Description    : This function is used to extract PMI value in
 *                  Aperidoc Mode 3_1 when rank = 1    
 ******************************************************************************/

STATIC UInt8 getPMIValueCW0ForMode31(UInt8 *messageBuffer_p,UInt8 numberOfBits,
        InternalCellIndex internalCellIndex)
{
    UInt8 pmiValue = 0;
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    switch(numberOfBits)
    {

        case 0 : 
        if((cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_1) || \
                (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2))
        { 
            pmiValue = ((*messageBuffer_p) & 0x03);
        }
        else if (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            pmiValue = ((*messageBuffer_p) & 0x0F);
        }
        /* + coverity 54532 */
        break;
        /* - coverity 54532 */

        case 2 :
        if((cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_1) || \
                (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2))
        {
            pmiValue = ((*messageBuffer_p) & 0x0C) >> 2;
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            pmiValue = ((*messageBuffer_p) & 0x3C) >> 2;
        }
        /* + coverity 54532 */
        break;
        /* - coverity 54532 */

        case 4 :
        if((cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_1) || \
                (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2))
        {
            pmiValue = ((*messageBuffer_p) & 0x30) >> 4;
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            pmiValue = ((*messageBuffer_p) & 0xF0) >> 4;
        }
        /* + coverity 54532 */
        break;
        /* - coverity 54532 */

        case 6 : 
        if((cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_1) || \
                (cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2))
        {
            pmiValue = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
        }
        else if(cellParams_p->numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
        {
            pmiValue = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p+1;
            pmiValue |= GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p);
        }
        /* + coverity 54532 */
        break;
        /* - coverity 54532 */

    }     
    return pmiValue; 
}

/* 4x2 MIMO S */
/*****************************************************************************
 * Function Name  : getPMIValueCWForAnt4Mode1_2
 * Inputs         : messageBuffer_p - buffer pointer,
 *                  numberOfBytes - number of bytes to be moved,
 *                  numberOfBits - number of bits to be moved,
 *                  N, rank ,pmiInfo12
 * Outputs        : PMI value for each subband
 * Returns        : PMI Value
 * Description    : This function is used to extract PMI value for each subband
 *                  in Aperidoc Mode 1_2 when rank = 1
 ******************************************************************************/
        STATIC void getPMIValueCWForAnt4Mode1_2(UInt8 *messageBuffer_p,
                UInt8 numberOfBytes,
                UInt8 numberOfBits,
                /* +- SPR 17777 */
                UInt8 rank,
                UInt8 *pmiInfo12)
{
    /* For first 4 bits */
    if (rank == 1)
    {
        *pmiInfo12 = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
        *pmiInfo12 = GET_4_BIT_SWAPPED_VALUE(*pmiInfo12);
        pmiInfo12++;
        messageBuffer_p = messageBuffer_p + 1;
    }
    while (numberOfBytes--)
    {
        /* First 4-bit form the messageBuffer_p pointer */
        *pmiInfo12 = GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
        *pmiInfo12 = GET_4_BIT_SWAPPED_VALUE(*pmiInfo12);
        pmiInfo12++;
        /* Last 4-bit form the messageBuffer_p pointer */
        *pmiInfo12 = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
        *pmiInfo12 = GET_4_BIT_SWAPPED_VALUE(*pmiInfo12);
        pmiInfo12++;
        messageBuffer_p++;
    }
    if (numberOfBits)
    {
        *pmiInfo12 = GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
        *pmiInfo12 = GET_4_BIT_SWAPPED_VALUE(*pmiInfo12);
    }
    return;
}
/* 4x2 MIMO S */

/*****************************************************************************
 * Function Name  : getPMIValueCW0ForMode1_2 
 * Inputs         : messageBuffer_p - buffer pointer,
 *                  numberOfBytes - number of bytes to be moved,
 *                  numberOfBits - number of bits to be moved,
 *                  N and pmiInfo12
 * Outputs        : PMI value for each subband
 * Returns        : PMI Value
 * Description    : This function is used to extract PMI value for each subband
 *                  in Aperidoc Mode 1_2 when rank = 1    
 ******************************************************************************/

STATIC void getPMIValueCW0ForMode1_2(UInt8 *messageBuffer_p,
        UInt8 numberOfBytes, 
        UInt8 numberOfBits,
        UInt8 N,
        UInt8 *pmiInfo12)
{

    UInt64 pmiValueCW0 = 0;
    UInt16 tempPMIValue = 0;

    /* For first 4 bits */
    pmiValueCW0 = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
    messageBuffer_p = messageBuffer_p + 1;

    /* Cyclomatic_complexity_changes_start */
    if(0 == numberOfBytes)
    {
        pmiValueCW0 |= getPMIValueCW0ForMode1_2Bytes0(messageBuffer_p, numberOfBits);
    }

    else if(1 == numberOfBytes)
    {
        pmiValueCW0 |= getPMIValueCW0ForMode1_2Bytes1(messageBuffer_p, numberOfBits);
    }

    else if(2 == numberOfBytes)
    {
        pmiValueCW0 |= getPMIValueCW0ForMode1_2Bytes2(messageBuffer_p, numberOfBits);
    }

    else if(3 == numberOfBytes)
    {
        pmiValueCW0 |= getPMIValueCW0ForMode1_2Bytes3(messageBuffer_p, numberOfBits);
    }

    else if(4 == numberOfBytes)
    {
        pmiValueCW0 |= getPMIValueCW0ForMode1_2Bytes4(messageBuffer_p, numberOfBits);
    }

    else if(5 == numberOfBytes)
    {
        pmiValueCW0 |= getPMIValueCW0ForMode1_2Bytes5(messageBuffer_p, numberOfBits);
    }

    else if(6 == numberOfBytes)
    {
        pmiValueCW0 |= getPMIValueCW0ForMode1_2Bytes6(messageBuffer_p, numberOfBits);
    }

    while (N--)
    {
        tempPMIValue = pmiValueCW0 & 0x03;   
        tempPMIValue = GET_2_BIT_SWAPPED_VALUE(tempPMIValue);
        *pmiInfo12 = tempPMIValue;
        pmiInfo12++;
        pmiValueCW0 = pmiValueCW0 >> 2;
    }
    return;
    /* Cyclomatic_complexity_changes_end */
}

/*****************************************************************************
 * Function Name  : getPMIValueCW1ForMode1_2 
 * Inputs         : messageBuffer_p - buffer pointer,
 *                  startIndex, endIndex and pmiInfo12
 * Outputs        : PMI Value for each subband
 * Returns        : None
 * Description    : This function is used to extract PMI value for each subband
 *                  in Aperidoc Mode 1_2 when rank > 1    
 ******************************************************************************/

STATIC void  getPMIValueCW1ForMode1_2(UInt8 *messageBuffer_p,
        UInt8 startIndex,
        UInt8 endIndex, 
        UInt8 *pmiInfo12)
{

    UInt8 tempPMIValue = 0;
    UInt8 tempBuff = *messageBuffer_p;

    while (startIndex++ < endIndex)
    {
        tempPMIValue = tempBuff & 0x01;   

        *pmiInfo12 = tempPMIValue;
        pmiInfo12++;
        tempBuff = tempBuff >> 1;

    }
}

/* - CQI_4.1.0 */


/* + CQI_5.0 */
/*****************************************************************************
 * Function Name  : pushNodeForCQIProcessing
 * Inputs         : ueIndex -  ueIndex of UE,
 *                  riValue - received RI value,
 *                  cqiExpectedType - RI type and
 *                  ulUEContext_p - pointer to ULUEContext
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function will push node of respective RI type node.
 *****************************************************************************/
    /* + SPR_5415 */
    /* +- SPR 18268 */
    void pushNodeForCQIProcessing(UInt16 ueIndex,
            /* +- SPR 18268 */
            UInt8 riValue,
            ueExpectedReportType cqiExpectedType,
            ULUEContext *ulUEContext_p,
            /*CA Changes start  */
            InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{   

    UEReportedCQINodeInfo *ueReportedCQINodeInfo_p = PNULL;
    /* +COVERITY 5.0 */
    PeriodicMode10Type3 *periodicMode10Type3_p = PNULL;
    PeriodicMode11Type3 *periodicMode11Type3_p = PNULL;
    PeriodicMode20Type3 *periodicMode20Type3_p = PNULL;
    PeriodicMode21Type3 *periodicMode21Type3_p = PNULL;
    /* -COVERITY 5.0 */
    UInt8 *ueReportedPeriodicRI_p = PNULL;

    GET_MEM_FROM_POOL(UEReportedCQINodeInfo,ueReportedCQINodeInfo_p,
            sizeof(UEReportedCQINodeInfo),PNULL);

    /* Rel 5.3: Coverity 24454 Fix Start */
    if (PNULL != ueReportedCQINodeInfo_p)
    {
        /* CA_phase2_csi_code Start */
        if(ulUEContext_p->internalCellIndex == internalCellIndex)
        {
            ueReportedPeriodicRI_p = &(ulUEContext_p->pucchConfigInfo.ueReportedPeriodicRI);
        }
        else
        {
            ueReportedPeriodicRI_p = &(ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->
                    scellpucchConfigInfo.ueReportedPeriodicRI);
        }

        ueReportedCQINodeInfo_p->ueIndex = ueIndex;

        switch ( cqiExpectedType )
        {    
            case PERIODIC_MODE_1_0_TYPE_3:

            *ueReportedPeriodicRI_p = riValue;
            ueReportedCQINodeInfo_p->mode = PERIODIC_CQI_MODE_1_0_TYPE_3;
            periodicMode10Type3_p =  
                &(ueReportedCQINodeInfo_p->cqiNode.periodicMode10Type3);
            periodicMode10Type3_p->riValue = riValue;
            break;

            case PERIODIC_MODE_1_1_TYPE_3:

            *ueReportedPeriodicRI_p = riValue;
            ueReportedCQINodeInfo_p->mode = PERIODIC_CQI_MODE_1_1_TYPE_3;
            periodicMode11Type3_p =  
                &(ueReportedCQINodeInfo_p->cqiNode.periodicMode11Type3);
            periodicMode11Type3_p->riValue = riValue;
            break;

            case PERIODIC_MODE_2_0_TYPE_3: 

            *ueReportedPeriodicRI_p = riValue;
            ueReportedCQINodeInfo_p->mode = PERIODIC_CQI_MODE_2_0_TYPE_3;
            periodicMode20Type3_p =  
                &(ueReportedCQINodeInfo_p->cqiNode.periodicMode20Type3);
            periodicMode20Type3_p->riValue = riValue;
            break;

            case PERIODIC_MODE_2_1_TYPE_3: 

            *ueReportedPeriodicRI_p = riValue;
            ueReportedCQINodeInfo_p->mode = PERIODIC_CQI_MODE_2_1_TYPE_3;
            periodicMode21Type3_p =  
                &(ueReportedCQINodeInfo_p->cqiNode.periodicMode21Type3);
            periodicMode21Type3_p->riValue = riValue;
            break;

            default:

            lteWarning("Invalid RI type\n");
            freeMemPool(ueReportedCQINodeInfo_p);
            return;
        }

        /* SPR 99999 CQI with 255 UE Fix start */
        UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_CQI_RECEIVED(ueReportedCQINodeInfo_p,internalCellIndex)
            /* SPR 23483 Changes Start */
            if(CIRC_FAILURE == ENQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo,
                        /*CA Changes start  */
                        &(ueReportedCQINodeInfo_p->nodeAnchor), internalCellIndex))
                /*CA Changes start  */
            /* SPR 23483 Changes End */
            {
                freeMemPool(ueReportedCQINodeInfo_p);
                ueReportedCQINodeInfo_p = NULL;	
            }
        /* SPR 99999 Fix end */
    }
    else
    {
        /* Not able to allocate Memory */
    }
    /* Rel 5.3: Coverity Fix End */

    return;
}

/* - SPR_5415 */
/* - CQI_5.0 */


/** HD FDD Changes start **/
#ifdef HD_FDD_CONFIG
/*****************************************************************************
 * Function Name  :updateHDUeSubFrameCntxtInfo 
 * Inputs         : tick - current system tick 
 * Outputs        : update information about reporting counters and 
 *                  saves consolidated reporting information for +9 in HD SchedMap,
 *                  recreates HD sched map for subframe 2-9 of configration has changed
 * Returns        : void
 * Description    : This function updates all schedular map of HD UE's
 *                  as per semistatic map.
 *****************************************************************************/
            /* SPR 15909 fix start */
 void  updateHDUeSubFrameCntxtInfo(tickType_t tick)
    /* SPR 15909 fix end */
{
    UInt8 ctr=0;
    UInt16 ue=0;
    /** can see performance gain **/
    /** to track multiple call in same subframe**/
    /* SPR 15909 fix start */
    static tickType_t lastTick = 0;
    /* SPR 15909 fix end */
    if(lastTick < tick)
    {
        /** for all HD UEs **/
        for(ctr = 0; ctr < hdUeContextInfo_g.numHdUe; ctr++)
        {
            ue = hdUeContextInfo_g.hdUeIndex[ctr];

            /* Check UE index validity to avoid out of bound access, but
             * this should not happen */
            if (ue >= MAX_UE_SUPPORTED)
            {
                LOG_MAC_MSG(MAC_HD_FDD_WRONG_UE_INDEX, LOGWARNING, MAC_L1_INF,\
                        getCurrentTick(), \
                        ue,DEFAULT_INT_VALUE,\
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE, \
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE, \
                        FUNCTION_NAME,"WRONG_UE_INDEX");
                continue; /* check next UE */
            }

            /** if configuration for this UE is not changed yet **/
            if(!hdUeContextInfo_g.configChange[ue])
            {
                /* if configuration is same as last subframe, for every tick the last subframe
                 * (10th frame at + 9 offset) of Sched map will be updated */
                UPDATE_HD_UE_SCHED_MAP(tick+(HD_UE_SCHED_MAP_SIZE)-1, ue);
                /* check and upate timeouts for all reports, those are expiring in currenttick+4 subframe */
                updateHdNextReportTimeout((tick+(HD_UE_SCHED_MAP_SIZE)-1), ue);
            }
            else
            {
                /** if configuration for this UE is changed by reconfigration **/
                UInt8 i = 0;
                /** update schedmap for 0-9 */
                for(i = 0; i < HD_UE_SCHED_MAP_SIZE; i++)
                {
                    /*Reinit HD UE reporint bits in the schedule map*/
                    /* Coverity 83225 Fix Start */
                    /* REINIT_HD_UE_REPORTING_BITS(tick+i, ue); */
                    /* Coverity 83225 Fix End */
                    UPDATE_HD_UE_SCHED_MAP(tick+i, ue);
                    /* check and upate timeouts for all reports, those are expiring in currenttick+4 subframe */
                    updateHdNextReportTimeout((tick+i), ue);
                }


                /** reconfiguration changed have been implemented **/
                hdUeContextInfo_g.configChange[ue] = FALSE;
            }
        }
        lastTick = tick;
    }
    else if(lastTick == tick)
    {
        /* fucntion should be called only once per subframe */
        ltePanic("Func updateHDUeSubFrameCntxtInfo is being called twice in one subframe");
    }
}

/*****************************************************************************
 * Function Name  : updateHdNextReportTimeout
 * Inputs         : sfn - current system tick
 *                  ue - ueIndex
 * Outputs        : updates the next reporting timeouts in the HD UE context
 * Returns        : void
 * Description    : This function updates HD UE Context as per semistatic map
 ****************************************************************************/
 void  updateHdNextReportTimeout(UInt32 sfn, UInt16 ue)
{
    UInt32 schedMapOffset=(sfn)%HD_UE_SCHED_MAP_SIZE;

    if((hdUeContextInfo_g.hdfddUeSchedMap[schedMapOffset].hdfddUeSfMap[ue]) & (HD_FDD_BITMASK_UL_REPORTS) )
    {
        UInt8 type=0;
        for(type=0; type<HD_UE_NUM_OF_REPORTS; type++)
        {
            /* if corresponding report bit is set, means the condition is hit where
             * corresponding report was sent by UE */
            if(GET_BIT(type, hdUeContextInfo_g.hdfddUeSchedMap[(schedMapOffset)].hdfddUeSfMap[ue]))
            {
                /* for oneShot SRS handling, chk if SRS map and bit is set i.e., SRS is already scheduled */
                if ((type == HD_UE_STATIC_MAP_SRS) &&
                        (0 == hdUeContextInfo_g.hdUeReportingInfo[ue][type].oneShot) )
                {
                    HD_UE_RESET_REPORT_INFO(ue, type)
                }
                else
                {
                    hdUeContextInfo_g.hdUeReportingTimeOuts[ue][type] +=
                        hdUeContextInfo_g.hdUeReportingInfo[ue][type].periodicity;
                }
            }
        }
    }
}

#endif
/* HD FDD Changes End */
/* SPR 19288 change start */
/* SPR 19288 change end */

/* + TM7_8 Changes Start */
/*****************************************************************************
 * Function Name  : calculateTimeAvgAoA
 * Inputs         : lastSavedAoAValue: last Saved AoAValue in ueContext
 *                  newAoAValue - new AoAValue received
 * Outputs        : It does time averaging of received and previous Aoa Value.
 * Returns        : TimeAveraged value of Aoa
 * Description    : This function update time averaging of received and previous Aoa Value.
 ****************************************************************************/
 UInt16 calculateTimeAvgAoA(UInt16 lastSavedAoAValue ,UInt16 newAoAValue)
{
    UInt16 timeAveragedTAAoA = 0;

    timeAveragedTAAoA = ((TIME_AVG_COEFFICIENT_IIR * lastSavedAoAValue) + 
            ((1024 - TIME_AVG_COEFFICIENT_IIR)*newAoAValue))/1024;

    return timeAveragedTAAoA;
}
/* + TM7_8 Changes End*/
/* + E_CID_5.3 */
/* Coverity # 62402*/
/*****************************************************************************
 * Function Name  : checkValidReportsAndPutEntryInEcidMgr
 * Inputs         : ueIndex - Ue Index and
 *                  validReports - Type of valid E-CID reports received
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : none
 * Returns        : none
 * Description    : This function check whether received ECID reports are valid 
 *                  are there and then put an Entry in ECID manager queue
 ****************************************************************************/

    STATIC  void checkValidReportsAndPutEntryInEcidMgr(UInt16 ueIndex,
            UInt8 validReports,
            /*CA Changes start  */
            InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{

    ULUEContext *ulUEContext_p = PNULL;

    ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;


    /*if L2 is waiting for TA type 1 and/or AOA after reception of TA type2
     *      if so then validate and update validReports
     *      if all reports requested by RRM are valid then send reports with SUCCCESS
     *      else if some of the reports are valid then send reports with PARTIAL_SUCCESS
     *      else send respose as FAILURE
     *else validate TA type 1 and AOA and update validReports
     *      if all reports requested by RRM are valid then send reports with SUCCCESS
     *      else if some of the reports are valid then send reports with PARTIAL_SUCCESS
     *      else send respose as FAILURE
     */
    /* + SPR_8082 */
    if((WAIT_FOR_REPORTS_TA_1_OR_AOA_AFTER_TA_2_RECEIVED == 
                ulUEContext_p->eCidReportParams.reportStatus) && \
            (ulUEContext_p->eCidReportParams.rrmReqRecvTick <= \
             ulUEContext_p->eCidReportParams.type1TaAOAReceivedTTI))
        /* - SPR_8082 */
    {
        validReports |= ((MAX_TIMING_ADVANCE_R9_VALUE >= \
                    ulUEContext_p->eCidReportParams.type2TAValue)? TRUE:FALSE) << 1;

        validReports &= ulUEContext_p->eCidReportParams.typeofReportsRequired;

        if(validReports == ulUEContext_p->eCidReportParams.typeofReportsRequired)
        {
            putEntryInEcidMeasReportQueue(ueIndex,\
                    ulUEContext_p->eCidReportParams.typeofReportsRequired, \
                    MAC_SUCCESS, \
                    /*CA Changes start  */
                    internalCellIndex);
            /*CA Changes end  */
        }
        else if(validReports)
        {
            putEntryInEcidMeasReportQueue(ueIndex,\
                    ulUEContext_p->eCidReportParams.typeofReportsRequired, \
                    MAC_PARTIAL_SUCCESS, \
                    /*CA Changes start  */
                    internalCellIndex);
            /*CA Changes end  */

        }
        else
        {
            putEntryInEcidMeasReportQueue(ueIndex,\
                    NO_REPORT_TYPE_REQUIRED,MAC_FAILURE, \
                    /*CA Changes start  */
                    internalCellIndex);
            /*CA Changes end  */

        }
        /* SPR_8672_FIX */
        CHECK_AND_DELETE_NODE_FROM_DUMMY_DCI0_LIST(
                ulUEContext_p->eCidReportParams.dummyDCI0Entry_p,ulUEContext_p->eCidReportParams.dummyDCI0PresentFlag,
                internalCellIndex);
        ulUEContext_p->eCidReportParams.dummyDCI0PresentFlag = FLAG_RESET; 
        /* SPR_8672_FIX */
        /*update with no report type required*/
        ulUEContext_p->eCidReportParams.typeofReportsRequired = 
            NO_REPORT_TYPE_REQUIRED;

    }
    else
    {
        /*Check if L2 is waiting for TA type 2 when RRM requests for TA type 1 
         * and/or AOA along with TA type 2 report then don't
         * process any report until it receive latest reports*/
        if(WAIT_FOR_TA_2_REPORT != ulUEContext_p->eCidReportParams.reportStatus && \
                (ulUEContext_p->eCidReportParams.rrmReqRecvTick <= \
                 ulUEContext_p->eCidReportParams.type1TaAOAReceivedTTI))
        {
            validReports &= ulUEContext_p->eCidReportParams.typeofReportsRequired;

            if(validReports == ulUEContext_p->eCidReportParams.typeofReportsRequired)
            {
                putEntryInEcidMeasReportQueue(ueIndex,\
                        ulUEContext_p->eCidReportParams.typeofReportsRequired, \
                        MAC_SUCCESS, \
                        /*CA Changes start  */
                        internalCellIndex);
                /*CA Changes end  */
            }
            else if(validReports)
            {
                putEntryInEcidMeasReportQueue(ueIndex,\
                        ulUEContext_p->eCidReportParams.typeofReportsRequired, \
                        MAC_PARTIAL_SUCCESS, \
                        /*CA Changes start  */
                        internalCellIndex);
                /*CA Changes end  */

            }
            else
            {
                putEntryInEcidMeasReportQueue(ueIndex,\
                        NO_REPORT_TYPE_REQUIRED,MAC_FAILURE, \
                        /*CA Changes start  */
                        internalCellIndex);
                /*CA Changes end  */

            }
            /*update with no report type required*/
            /* SPR_8672_FIX */
            CHECK_AND_DELETE_NODE_FROM_DUMMY_DCI0_LIST(
                    ulUEContext_p->eCidReportParams.dummyDCI0Entry_p,ulUEContext_p->eCidReportParams.dummyDCI0PresentFlag,
                    internalCellIndex);
            ulUEContext_p->eCidReportParams.dummyDCI0PresentFlag = FLAG_RESET; 
            /* SPR_8672_FIX */
            ulUEContext_p->eCidReportParams.typeofReportsRequired = 
                NO_REPORT_TYPE_REQUIRED;
        }

    }
}
/* - E_CID_5.3 */

/* FAPI1.1 Compilation Fix Start */
/* Flag FAPI_4_0_COMPLIANCE removed */
/* FAPI1.1 Compilation Fix End */
/*****************************************************************************
 * Function Name  : decodeCqiReports
 * Inputs         : data_p - pointer to PDU recieved,
 *                  cqiReportType- Cqi Mode,
 *                  ueIndex   - ueIndex of UE,
 *                  ulUEContext_p - pointer to the UL UE context,
 *                  cqiInfoSch_p - pointer to CQIListElement,
 *                  recvTTI  - Its represent the tick of the message
 *                              recieved,
 *                  transmissionMode: Transmission mode of the UE,
 *                  dlUEContext_p - pointer to DLUEContext and
 *                  subFrameNum - current Subframe
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : decodes the cqi report received for reporttype.
 * Returns        : void
 * Description    : This function decodes the cqi report received for report type.
 ****************************************************************************/
STATIC  void decodeCqiReports(UInt8 *data_p,
        ueExpectedReportType cqiReportType,
        UInt16 ueIndex,
        ULUEContext *ulUEContext_p,
        /* SPR 15909 fix start */
        tickType_t recvTTI,
        /* SPR 15909 fix end */
        UInt8 transmissionMode,
        DLUEContext *dlUEContext_p,
        UInt16 subFrameNum,
        /*CA Changes start */
        InternalCellIndex internalCellIndex)
/*CA Changes end */
{
    UInt8 ri = 0;
    UInt8 numOfBitsDecoded = 0; /* CA_phase2_csi_code */
    InternalCellIndex  servCellId = INVALID_CELL_INDEX;
    /* CA_phase2_csi_code start */

    /* +EICIC*/
    cqiSubFrameNum_g=subFrameNum;
    /*-EICIC*/

    if(ulUEContext_p->internalCellIndex == internalCellIndex)
    {
        /* Pcell */
        ri = ulUEContext_p->pucchConfigInfo.ueReportedRI;
    }
    else
    {
        /* Scell */
        servCellId = getServeCellIndexFromInternalCellId(dlUEContext_p,
                internalCellIndex);
        /*klockwork warning fix*/
        if(servCellId <= MAX_NUM_SCELL)
        {
            ri = ulUEContext_p->ulUeScellContext_p[servCellId]->scellpucchConfigInfo.ueReportedRI;
        }
    }
    /* CA_phase2_csi_code end */

    /* + coverity 72986 */
    if ( ri > 0 )
    {
        switch(cqiReportType)
        {
            /* Cyclomatic_complexity_changes_start */
            case APERIODIC_MODE_1_2:
            case APERIODIC_MODE_2_0:
            case APERIODIC_MODE_2_2:
            case APERIODIC_MODE_3_0:
            case APERIODIC_MODE_3_1:
            decodeAperiodicCqi(cqiReportType,
                    data_p,
                    ueIndex,
                    ri,
                    ulUEContext_p,
                    dlUEContext_p,
                    recvTTI,
                    transmissionMode,
                    internalCellIndex,
                    &numOfBitsDecoded,
                    servCellId);
            break;
            /* - CQI_4.1 */

            case PERIODIC_MODE_1_0_TYPE_3:
            case PERIODIC_MODE_1_0_TYPE_4:
            case PERIODIC_MODE_1_1_TYPE_2:
            case PERIODIC_MODE_1_1_TYPE_3:
            case PERIODIC_MODE_2_0_TYPE_1:
            case PERIODIC_MODE_2_0_TYPE_3:
            case PERIODIC_MODE_2_0_TYPE_4:
            case PERIODIC_MODE_2_1_TYPE_1:
            case PERIODIC_MODE_2_1_TYPE_2:
            case PERIODIC_MODE_2_1_TYPE_3:
            case PERIODIC_MODE_ALL_TYPE_3:
            decodePeriodicCqi(cqiReportType,
                    data_p,
                    ueIndex,
                    /* +- SPR 17777 */
                    dlUEContext_p,
                    transmissionMode,
                    ulUEContext_p,
                    internalCellIndex,
                    servCellId,
                    subFrameNum);
            break;

            default:
            lteWarning("Report Type Decoding not supported %d\n", 
                    /* PURIFY FIXES 2.4.1 start */
                    (UInt32)cqiReportType);
            /* PURIFY FIXES 2.4.1  end*/
            break;
            /* Cyclomatic_complexity_changes_end */
        }
    }
    else
    {
        lteWarning("Invalid Rank Indicator Recieved");
    }
    /* - coverity 72986 */
}
/* CA_phase2_csi_code Start */
/* coverity 64952 28June2014 */
#ifdef FAPI_4_0_COMPLIANCE
/*ca-tdd pucch code changes*/
/* coverity 64952 28June2014 */
/*****************************************************************************
 * Function Name  : decodeAperiodicCqiReportsR10
 * Inputs         : data_p - pointer to PDU recieved,
 *                  cqiReportType- Cqi Mode,
 *                  ueIndex   - ueIndex of UE,
 *                  ulUEContext_p - pointer to the UL UE context,
 *                  cqiInfoSch_p - pointer to CQIListElement,
 *                  recvTTI  - Its represent the tick of the message
 *                              recieved,
 *                  transmissionMode: Transmission mode of the UE,
 *                  dlUEContext_p - pointer to DLUEContext and
 *                  subFrameNum - current Subframe
 *                  internalCellIndex - Cell_index at MAC
 *                  numOfBitsDecoded_p
 * Outputs        : decodes the cqi report received for reporttype.
 * Returns        : void
 * Description    : This function decodes the cqi report received for report type.
 ****************************************************************************/
                    STATIC  void decodeAperiodicCqiReportsR10(UInt8 *data_p,
                            ueExpectedReportType cqiReportType,
                            UInt16 ueIndex,
                            ULUEContext *ulUEContext_p,
                            /* SPR 15909 fix start */
                            tickType_t recvTTI,
                            /* SPR 15909 fix end */
                            UInt8 transmissionMode,
                            DLUEContext *dlUEContext_p,
                            /* +- SPR 17777 */
                            /*CA Changes start */
                            InternalCellIndex internalCellIndex,
                            UInt8 *numOfBitsDecoded_p)
                    /*CA Changes end */
{
    UInt8 ri = 0;
    InternalCellIndex  servCellId = INVALID_CELL_INDEX;
    if(ulUEContext_p->internalCellIndex == internalCellIndex)
    {
        /* Pcell */
        ri = ulUEContext_p->pucchConfigInfo.ueReportedRI;
    }
    else
    {
        /* Scell */
        servCellId = getServeCellIndexFromInternalCellId(dlUEContext_p,
                internalCellIndex);
        /*kolckwork warning fix*/
        if(servCellId <= MAX_NUM_SCELL)
        {
            ri = ulUEContext_p->ulUeScellContext_p[servCellId]->scellpucchConfigInfo.ueReportedRI;
        }
    }

    /* + coverity 72986 */
    if ( ri > 0 )
    {
        switch(cqiReportType)
        {
            case APERIODIC_MODE_1_2:

            decodeAperiodicCqiMode1_2(data_p,
                    ueIndex, 
                    ri,
                    ulUEContext_p,
                    /* + CQI_5.0 */
                    recvTTI,
                    internalCellIndex,
                    /* CA_phase2_csi_code */
                    numOfBitsDecoded_p);
            /* - CQI_5.0 */
            if(ulUEContext_p->internalCellIndex == internalCellIndex)
            {
                /* Pcell */
                dlUEContext_p->dlCQIInfo.latestAperiodicReportRcvd = TRUE;
            }
            else
            {
                /* Scell */
                /*kolckwork warning fix*/
                if(servCellId <= MAX_NUM_SCELL)
                { 
                    dlUEContext_p->dlUeScellContext_p[servCellId]->ScelldlCQIInfo.
                        latestAperiodicReportRcvd = TRUE;
                }
            }
            break;

            case APERIODIC_MODE_2_0:

            decodeAperiodicCqiMode2_0(data_p,
                    ueIndex, 
                    transmissionMode,
                    /* + CQI_5.0 */
                    ri,
                    /* - CQI_5.0 */
                    ulUEContext_p,
                    internalCellIndex,
                    /* CA_phase2_csi_code */
                    numOfBitsDecoded_p);
            break;    

            case APERIODIC_MODE_2_2:

            /* + CQI_4.1 */
            decodeAperiodicCqiMode2_2(data_p, 
                    ueIndex,
                    ri,
                    ulUEContext_p,
                    internalCellIndex,
                    /* CA_phase2_csi_code */
                    numOfBitsDecoded_p);
            break; 
            /* - CQI_4.1 */

            case APERIODIC_MODE_3_0:

            decodeAperiodicCqiMode3_0(data_p, 
                    ueIndex,
                    transmissionMode,
                    /* + CQI_5.0 */
                    ri,
                    /* - CQI_5.0 */
                    ulUEContext_p,
                    internalCellIndex,
                    /* CA_phase2_csi_code */
                    numOfBitsDecoded_p);
            break;    

            case APERIODIC_MODE_3_1:

            /* + CQI_4.1 */
            decodeAperiodicCqiMode3_1(data_p, 
                    ueIndex,
                    ri,
                    /* + TM6_5.2 */
                    transmissionMode,
                    /* - TM6_5.2 */
                    ulUEContext_p,
                    internalCellIndex,
                    /* CA_phase2_csi_code */
                    numOfBitsDecoded_p);
            break;
            /* - CQI_4.1 */

            default:
            lteWarning("Report Type Decoding not supported %d\n", 
                    /* PURIFY FIXES 2.4.1 start */
                    (UInt32)cqiReportType);
            /* PURIFY FIXES 2.4.1  end*/
            break;
        }   
    }
    else
    {
        lteWarning("Invalid Rank Indicator Recieved");
    }
    /* - coverity 72986 */
}
/* coverity 64952 28June2014 */
#endif
/* coverity 64952 28June2014 */
/* CA_phase2_csi_code End */


/*****************************************************************************
 * Function Name  : handleRiReportWithDataOffsetNull
 * Inputs         : cqiReportType - cqi report type,
 *                  ulUEContext_p - pointer to the UlUEContext,
 *                  dlUEContext_p - pointer to the DlUEContext,
 *                  ueIndex - UE index
 *                  cqiPduToSch_p -  pointer to SchedDLCqiInfoReq
 *                  cqiInfoSch_p - pointer to CQIListElement
 *                  ri :rankIndicator value
 *                  subFrameNum - current subframe
 *                  internalCellIndex - Cell_index at MAC
 *                  servCellIndex - serving cellIndex
 * Outputs        : Fills Ri report informations in the cqi node. 
 * Returns        : void
 * Description    : This function handles Ri report received when data offset is 
 *                  null.
 ****************************************************************************/
STATIC  void  handleRiReportWithDataOffsetNull(
        ueExpectedReportType cqiReportType,
        ULUEContext *ulUEContext_p,
        DLUEContext *dlUEContext_p,
        UInt16 ueIndex,
        UInt8  *ri,
        InternalCellIndex internalCellIndex,
        UInt8 servCellIndex)
    /* +- SPR 17777 */
{
    /* CA_phase2_csi_code Start */
    TransmissonMode transmissionMode;
    UInt8 *cbsrValue;
    if(dlUEContext_p->internalCellIndex == internalCellIndex)
    {
        /* Pcell */
        transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
        cbsrValue =  dlUEContext_p->dlMIMOInfo.cbsrInfo.cbsrValue;
    }
    else
    {
        /* Scell */
        transmissionMode =  dlUEContext_p->dlUeScellContext_p[servCellIndex]->
            ScelldlMIMOInfo.transmissionMode;
        cbsrValue = dlUEContext_p->dlUeScellContext_p[servCellIndex]->
            codebookSubsetRestrictionv1020.cbsrValue;
    }

    /* CA_phase2_csi_code End */
    if ( *ri ) 
    {
#ifdef PERF_STATS
        gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.\
            totalRankReported[*ri - 1]++;
#endif 
        /* 4x4 DL MIMO CHG START */
        /* SPR 10177 changes start */
        if ((TX_MODE_8 == dlUEContext_p->dlMIMOInfo.transmissionMode) ||
                /* SPR 10177 changes end */
                (isRIInCodebookSubsetRestriction( *ri,
                                                  dlUEContext_p->ueCategory,
                                                  transmissionMode,
                                                  cbsrValue,
                                                  internalCellIndex) ))
        {
            pushNodeForCQIProcessing(ueIndex, *ri,
                    cqiReportType,ulUEContext_p,
                    internalCellIndex);
        } 
        /* 4x4 DL MIMO CHG START */
    }    
}
/*Cyclomatic Complexity changes - ends here */
/*****************************************************************************
 * Function Name  : handleFapiMsgSubFrameErrForDlConfig 
 * Inputs         : FAPI_errMsgBody2_st   errMsgBody2,
 *                  
 *                  tickType_t currentTick,
 *                  FAPI_l1ErrorCodes_en errorCode
 *                  InternalCellIndex internalCellIndex - Cell_index at MAC
 * Outputs        : NA. 
 * Returns        : void
 * Description    : This function handles the Error Indication received in
 *                  DL_CONFIG.request.
 *                  - DL HARQ recovery will be done 
 *                  - If power control commands had been sent to UE(s) in the
 *                    DL_Config.Request for which Error.Indication has been 
 *                    received, then MAC will update the total power 
 *                    correction sent to the UE as if the latest TPC was
 *                    not sent in DL_Config.Request
 *                  - No recovery will be performed for other PDUs
 ****************************************************************************/
        /*CID 83612 Fix Start*/ 
/* +- SPR 17777 */
STATIC  void  handleFapiMsgSubFrameErrForDlConfig(
        tickType_t currentTick,
#ifdef TDD_CONFIG
        FAPI_errMsgBody2_st   *errMsgBody2_p,
        FAPI_l1ErrorCodes_en errorCode, 
#endif
        InternalCellIndex internalCellIndex )
    /* +- SPR 17777 */
{
    ErrIndPowerControlType pcType = ERR_IND_TPC_FOR_PUCCH;
    UInt8 idx   = (currentTick - PHY_ERROR_IND_DELAY_DL) 
        % MAX_TPC_CONTAINER_MAP_SIZE;

    /* +- SPR 17777 */
    updateTpcForPhyErrorInd( pcType, idx,internalCellIndex);
    /* +- SPR 17777 */

    /* SPR 12261 fix start */
#ifdef TDD_CONFIG
    handleDlConfigErrForReTx( currentTick, errMsgBody2_p, errorCode, 
            internalCellIndex );
#endif
    /* SPR 12261 fix end */
}
/*CID 83612 Fix end*/ 
/*****************************************************************************
 * Function Name  : updateTpcForPhyErrorInd 
 * Inputs         : 
 *                  ErrIndPowerControlType pcType (PUCCH or PUSCH)
 *                  UInt8  idx(map Index)
 *                  internalCellIndex
 * Outputs        : NA. 
 * Returns        : void
 * Description    : This function handles the ERROR INDICATION If power 
 *                  control commands had been sent to UE(s)which 
 *                  Error.Indication has been received, then MAC will update
 *                  the total power correction sent to the UE as if the latest
 *                  TPC was not sent 
 ****************************************************************************/
        STATIC  void  updateTpcForPhyErrorInd( 
                /* +- SPR 17777 */
                ErrIndPowerControlType pcType, UInt8 idx,InternalCellIndex internalCellIndex )
{
    UInt8 index = 0;
    UInt16 ueIndex = 0;
    RNTIInfo        *rntiInfo_p        = PNULL;
    DLUEContext     *dlUEContext_p     = PNULL;
    ULUEContext     *ulUEContext_p     = PNULL;
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    ULUEContextInfo *ulUEContextInfo_p = PNULL;

    /* Traverse all the element of tpcSentMap and find the pdu */
    for (index = 0; index < tpcSentMap_g[internalCellIndex][idx].numUe; index++)
    {
        LP_TpcSent tpcSentInfo_p = &tpcSentMap_g[internalCellIndex][idx].tpcSentInfo[index];
        /* Check and get the ueIndex and ue Context from rnti */
        rntiInfo_p = getUeIdxFromRNTIMap(tpcSentInfo_p->rnti,internalCellIndex);
        if ( ( PNULL == rntiInfo_p )|| ( FREERNTI == rntiInfo_p->rntiFlag ) )
        {
            continue;
        }
        ueIndex = rntiInfo_p->index;

        /* Check the power control applied */
        if ((ERR_IND_TPC_FOR_PUCCH) == (pcType) && 
                (ERR_IND_TPC_FOR_PUCCH) == (tpcSentInfo_p->pcType)) 
        {
            dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
            if ( ( TRUE  == dlUEContextInfo_p->pendingDeleteFlag ) ||
                    ( PNULL == dlUEContextInfo_p->dlUEContext_p ))
            {
                continue;
            }
            dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;

            /* revert the power control applied previously */
            dlUEContext_p->currPowerCorrectionForPucch -= 
                tpcSentInfo_p->powerCorrectionVal;
            if (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                    cellParams_p->powerControlEnableInfo.cqiSinrClpcPucchEnable)
            {
                dlUEContext_p->powerControlPucchTpcValue = 0;
            }

            LOG_MAC_MSG( MAC_REVERT_POWER_CONTROL, LOGDEBUG, MAC_L1_INF,
                    getCurrentTick(), idx, ueIndex, 
                    dlUEContext_p->currPowerCorrectionForPucch, 
                    tpcSentInfo_p->powerCorrectionVal,
                    tpcSentMap_g[internalCellIndex][idx].numUe,
                    0,0, __func__, "PUCCH");
        }
        else if ((ERR_IND_TPC_FOR_PUSCH) == (pcType) && 
                (ERR_IND_TPC_FOR_PUSCH) == (tpcSentInfo_p->pcType))
        {
            ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
            if ( ( TRUE  == ulUEContextInfo_p->pendingDeleteFlag ) ||
                    ( PNULL == ulUEContextInfo_p->ulUEContext_p ))
            {
                continue;
            }
            ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;

            /* revert the power control applied previously */
            ulUEContext_p->currPowerCorrectionForPusch -=
                tpcSentInfo_p->powerCorrectionVal;

            LOG_MAC_MSG( MAC_REVERT_POWER_CONTROL, LOGDEBUG, MAC_L1_INF,
                    getCurrentTick(), idx, ueIndex, 
                    ulUEContext_p->currPowerCorrectionForPusch, 
                    tpcSentInfo_p->powerCorrectionVal, 
                    tpcSentMap_g[internalCellIndex][idx].numUe,
                    0,0, __func__, "PUSCH");
        } 
    }
}
/* SPR 11230 fix start */
#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : handleDlConfigErrForReTx 
 * Inputs         : 
 *                  tickType_t currentTick,
 *                  FAPI_errMsgBody2_st *errMsgBody2_p,
 *                  FAPI_l1ErrorCodes_en errorCode,
 *                  internalCellIndex
 * Outputs        : NA. 
 * Returns        : void
 * Description    : This function handles the retransmissions in TDD for 
 *                  SFN_OUT_OF_SYNC/MSG_SUNFRAME_ERROR and sets the flag to 
 *                  indicate error has been received so that node cannot be 
 *                  deleted if there is ACK for this node and hence, can 
 *                  easily perform re-transmissions after DL HARQ expiry.
 ****************************************************************************/
/* SPR 12261 fix start */
void handleDlConfigErrForReTx( 
        /* SPR 15909 fix start */
        tickType_t currentTick,
        /* SPR 15909 fix end */
        FAPI_errMsgBody2_st   *errMsgBody2_p,
        FAPI_l1ErrorCodes_en errorCode, InternalCellIndex internalCellIndex )
/* SPR 12261 fix end */
{
    LP_HarqSent    harqSent_p = PNULL;
    DLUEContext    *dlUeCtx_p   = PNULL;       
    DLUEContextInfo *dlUeCtxInfo_p = PNULL;
    /* SPR 12261 fix start */
    RNTIInfo        *rntiInfo_p = PNULL;
    /* SPR 12261 fix end */
    TDDHarqTimerExpiryListNode *expListNode_p = PNULL;
    /* SPR 15909 fix start */
    tickType_t containerTick = (currentTick  - PHY_ERROR_IND_DELAY_DL ) %
        MAX_HARQ_CONTAINER_MAP_SIZE ;
    /* SPR 15909 fix end */
    /* Number of PDU's sent in HARQ MAP */
    /* CA TDD Changes Start */
    UInt16 numOfPdu = harqSentMap_g[internalCellIndex][containerTick].numUe;
    /* CA TDD Changes End */
    UInt16 idx     = 0;
    UInt16 ueIndex = 0;
    UInt8  indexSfn = 0;
    UInt8  dlAckNackSf = 0;

    /* Finding the HARQ PDU */
    for (idx = 0; idx < numOfPdu; idx++)
    {
        /* CA TDD Changes Start */
        harqSent_p = &harqSentMap_g[internalCellIndex][containerTick].harqSentInfo[idx];
        /* CA TDD Changes End */

        indexSfn = harqSent_p->indexSfn;
        dlAckNackSf = harqSent_p->dlAckNackSf;
        ueIndex  = harqSent_p->ueIndex;

        dlUeCtxInfo_p = &dlUECtxInfoArr_g[ueIndex];
        if( LTE_TRUE == dlUeCtxInfo_p->pendingDeleteFlag )
        {
            /* Pending delete flag is set */
            continue;
        }
        dlUeCtx_p = dlUeCtxInfo_p->dlUEContext_p;
        if( PNULL == dlUeCtx_p )
        {
            /* dlUeCtx_p is NULL */
            continue;
        }

        /* SPR 12261 fix start */
        if( FAPI_MSG_SUBFRAME_ERR == errorCode )
        {
            /* Check and get the ueIndex and ue Context from rnti */
            rntiInfo_p = getUeIdxFromRNTIMap( errMsgBody2_p->rnti, 
                    internalCellIndex);
            if ( ( PNULL == rntiInfo_p ) ||
                    ( FREERNTI == rntiInfo_p->rntiFlag ) )
            {
                continue;
            }

            /* Check for ueIndex for which error is reported */
            if( rntiInfo_p->index == ueIndex )
            {
                /* Get the expiry Node for this UE */
                expListNode_p =
                    dlUeCtx_p->tddHarqTimerExpiryNode[indexSfn][dlAckNackSf];
                if( PNULL != expListNode_p )
                {
                    LOG_MAC_MSG( MAC_DL_CONFIG_ERR_RECVD, LOGDEBUG, MAC_L1_INF,
                            getCurrentTick(), __LINE__, ueIndex, indexSfn, 
                            dlAckNackSf, 0, 0,0, __func__, "");
                    /* Set that error is received for this node in DL Timer 
                     * List */
                    expListNode_p->isErrForNode = LTE_TRUE;
                }
                break;
            }
        }
        else
        {
            /* SPR 12261 fix end */
            /* Get the expiry Node for this UE */
            expListNode_p =
                dlUeCtx_p->tddHarqTimerExpiryNode[indexSfn][dlAckNackSf];
            if( PNULL != expListNode_p )
            {
                LOG_MAC_MSG( MAC_DL_CONFIG_ERR_RECVD, LOGDEBUG, MAC_L1_INF,
                        getCurrentTick(), __LINE__, ueIndex, indexSfn, 
                        dlAckNackSf, 0, 0,0, __func__, "");
                /* Set that error is received for this node in DL Timer List */
                expListNode_p->isErrForNode = LTE_TRUE;
            }
        }
    }
}
#endif	

/* SPR 11230 fix end */
/*****************************************************************************
 * Function Name  : handleFapiMsgSubFrameErrForUlConfig 
 * Inputs         : FAPI_errMsgBody2_st   errMsgBody2,
 *                  tickType_t currentTick,
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : NA. 
 * Returns        : void
 * Description    : This function handles  the PDU Error Indication received in
 *                  UL_CONFIG.request message. 
 *                  - For ULSCH, ULSCH_CQI_RI,ULSCH_HARQ,ULSCH_CQI_HARQ_RI
 *                    HI-Ack will be sent and UL HARQ Process  will be freed.
 *                  - For other PDUs No action will be taken.
 ****************************************************************************/
/*CID 83592 Fix start*/
STATIC  void  handleFapiMsgSubFrameErrForUlConfig(
        FAPI_errMsgBody2_st   *errMsgBody2_p,
        /* SPR 15909 fix start */
        tickType_t currentTick,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
{
    UInt32 crcFlag = 0;
    /* SPR 15909 fix start */
    tickType_t ulSchRecvTTI = currentTick - PHY_ERROR_IND_DELAY_UL;
    /* SPR 15909 fix end */
    UInt32 subFrameNum = ulSchRecvTTI % MAX_SUB_FRAME;
    UInt8 harqTimerStopFlag = FALSE;
#ifdef FDD_CONFIG                    
    UInt32 sysFrameNum = MODULO_ONEZEROTWOFOUR(ulSchRecvTTI / MAX_SUB_FRAME);
    UInt32 ulContainerIndex = ulSchRecvTTI % MAX_CONTAINER_NUM;
#elif TDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t phichTTI = ( ulSchRecvTTI + getHiPduSubframeNum(subFrameNum, internalCellIndex ));
    /* SPR 15909 fix end */
    UInt32 ulContainerIndex = phichTTI % getMaxULCrcContainer(internalCellIndex);
    UInt8 packetSubframeNum = 0;
    UInt8 hiPduSubframeNum = 0;
#endif
    UInt16 idx = 0;
    UInt16 ueIndex = 0;
    /* SPR 15909 fix start */
    tickType_t containerTick = (currentTick  - PHY_ERROR_IND_DELAY_UL ) %
        MAX_UL_CONFIG_CONTAINER_MAP_SIZE ;
    /* SPR 15909 fix end */
    /* Number of PDU's sent in UL_CONFIG */
    UInt16  numOfPdu = ulConfigMap_g[internalCellIndex][containerTick].numUe;

    TempCRNTICtx *tempCRNTICtx_p = PNULL;
    RNTIInfo        *rntiInfo_p        = PNULL;
    UlConfigInfo    *ulConfigInfo_p    = PNULL;
#ifdef FDD_CONFIG
    ULUEContext     *ulUEContext_p     = PNULL;
#endif
    ULUEContextInfo *ulUEContextInfo_p = PNULL;
#ifdef TDD_CONFIG
    packetSubframeNum = ( currentTick - PHY_ERROR_IND_DELAY_UL ) % MAX_SUB_FRAME;
    if (!isValidULSubFrameForHIPdu(packetSubframeNum, &hiPduSubframeNum, internalCellIndex ))
    {
        ltePanic("Invalid UL subframe [%d]\n", packetSubframeNum);
    }
#endif

    if (0 == errMsgBody2_p->rnti )
    {
        /* If the error occurred in a MCH, or BCH, PDU this value is set 
         * to 0 */
        LOG_MAC_MSG( MAC_NO_ACTION_UL_CONFIG_ERR_PDU, LOGDEBUG, MAC_L1_INF,
                getCurrentTick(), __LINE__, errMsgBody2_p->rnti, 
                0, 0, 0, 0,0, __func__, "");
        return ;
    }

    /* Finding the PDU in UL_CONFIG for which error is received */
    for (idx = 0; idx < numOfPdu; idx++)
    {
        ulConfigInfo_p = 
            &ulConfigMap_g[internalCellIndex][containerTick].ulConfigInfo[idx];
        if ((errMsgBody2_p->rnti == ulConfigInfo_p->rnti) &&
                (errMsgBody2_p->pduType == ulConfigInfo_p->pduType))
        {
            /* PDU Found */
            LOG_MAC_MSG( MAC_UL_CONFIG_ERR_PDU, LOGDEBUG, MAC_L1_INF,
                    getCurrentTick(), __LINE__, ulConfigInfo_p->rnti, 
                    ulConfigInfo_p->pduType, numOfPdu, idx, 
                    0,0, __func__, "");
            break;
        }
    }
    /* If any pdu found in map we will do recovery for all pdus */
    if( idx != numOfPdu)
    {
        for (idx = 0; idx < numOfPdu; idx++)
        {
            /*Coverity 73463 Changes Start*/
            /*ulConfigInfo_p = 
              &ulConfigMap_g[internalCellIndex][containerTick].ulConfigInfo[idx];*/
            /*Coverity 73463 Changes End*/
            switch( errMsgBody2_p->pduType )
            {
                /* Intensional switch case fall through */
                case  MAC_SS_PDU_ULSCH :
                case  MAC_SS_PDU_ULSCH_CQI_RI :
                case  MAC_SS_PDU_ULSCH_HARQ :
                case  MAC_SS_PDU_ULSCH_CQI_HARQ_RI :
                {
                    /* Check and get the ueIndex and ue Context from rnti */
                    rntiInfo_p = getUeIdxFromRNTIMap(errMsgBody2_p->rnti,internalCellIndex);
                    if ( ( PNULL == rntiInfo_p ) ||
                            ( FREERNTI == rntiInfo_p->rntiFlag ) )
                    {
                        break;
                    }
                    else if ( TCRNTI == rntiInfo_p->rntiFlag )
                    {
                        /* Check for TCRNTI flag */
                        ueIndex = rntiInfo_p->index;
                        tempCRNTICtx_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].
                            tempCRNTICtx_p;
                        if (PNULL != tempCRNTICtx_p)
                        {
                            /* stop UL HARQ timer for MSG3 */
#ifdef FDD_CONFIG
#ifdef ULHARQ_TIMER_PROC
                            ulHarqTimerForMsg3Stop(tempCRNTICtx_p, 
                                    subFrameNum, sysFrameNum,internalCellIndex);
#endif
                            /* SPR 13196 fix start */
                            crcFlag = 1; /*NACK*/
                            /* SPR 13196 fix end */
                            crcAckNackHandlingForTcrnti( crcFlag, ueIndex, 
                                    ulSchRecvTTI ,ulContainerIndex,internalCellIndex);
#endif
                        }
                        else
                        {
                            break;
                        }
                    }
                    else if( ( CRNTI == rntiInfo_p->rntiFlag ) || 
                            ( SPS_RNTI == rntiInfo_p->rntiFlag ) )
                    {
                        /* Check for CRNTI or SPS_RNTI flag */
                        ueIndex = rntiInfo_p->index;
                        ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
                        if ( ( TRUE == ulUEContextInfo_p->pendingDeleteFlag ) ||
                                ( PNULL == ulUEContextInfo_p->ulUEContext_p ))
                        {
                            break;
                        }

#ifdef FDD_CONFIG
                        ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
#ifdef ULHARQ_TIMER_PROC
                        /* Stop the HARQ timer */
                        UInt8 tempId = MODULO_SIXTEEN( ulSchRecvTTI );
                        UInt8 ulHarqProcessId = 
                            ulUEContext_p->ttiHarqMap[ tempId ];
                        ULHarqInfo* ulHarqProcess_p =
                            &ulUEContext_p->ulresInfo[ulHarqProcessId];

                        if( ulHarqProcess_p->isTTIBundled )
                        {
                            /* SPR 15909 fix start */
                            tickType_t ttiBundleFirstTick = 
                                ulHarqProcess_p->ttiBundleFirstTick;
                            /* SPR 15909 fix end */
                            if( ulSchRecvTTI == ( ttiBundleFirstTick + 
                                        TTIB_FIRST_TO_LAST_TICK_OFFSET ) )
                            {
                                if(MAC_FAILURE == (ulHarqTimerStop(ulUEContext_p,
                                                /* +- SPR 17777 */
                                                subFrameNum,sysFrameNum,
                                                /* +- SPR 17777 */
                                                /*CA Changes start  */
                                                internalCellIndex)))
                                {	 
                                    LOG_MAC_MSG( MAC_UL_HARQ_TIMER_STOP_FAIL, LOGWARNING, MAC_UL_HARQ,
                                            getCurrentTick(), __LINE__, ulUEContext_p->ueIndex, ulSchRecvTTI,
                                            subFrameNum, ulHarqProcessId, 0,0, __func__, "");
                                }
                                harqTimerStopFlag = TRUE; 
                            }
                            else
                            {
                                LOG_MAC_MSG( MAC_RECV_ERR_FOR_TTIB_DATA, LOGDEBUG,
                                        MAC_L1_INF, getCurrentTick(), __LINE__,
                                        ueIndex, ulHarqProcessId, ulSchRecvTTI, 
                                        ttiBundleFirstTick, 0,0, __func__, "");
                                break;
                            }
                        }
                        else
                        {
                            if(MAC_FAILURE == (ulHarqTimerStop(ulUEContext_p,
                                            /* +- SPR 17777 */
                                            subFrameNum,sysFrameNum,
                                            /* +- SPR 17777 */
                                            /*CA Changes start  */
                                            internalCellIndex)))
                            {	 
                                LOG_MAC_MSG( MAC_UL_HARQ_TIMER_STOP_FAIL, LOGWARNING, MAC_UL_HARQ,
                                        getCurrentTick(), __LINE__, ulUEContext_p->ueIndex, ulSchRecvTTI,
                                        subFrameNum, ulHarqProcessId, 0,0, __func__, "");
                            }
                            harqTimerStopFlag = TRUE; 
                        }
#endif
#endif
                        /* SPR 13196 fix start */
                        crcFlag = 1; /*NACK*/
                        /* SPR 13196 fix end */
#ifdef TDD_CONFIG
                        harqTimerStopFlag = TRUE; 
#endif
                        if(harqTimerStopFlag)
                        {
                            crcAckNackHandling(crcFlag, ueIndex, 
                                    ulSchRecvTTI, ulContainerIndex, 
                                    rntiInfo_p->rntiFlag,
                                    internalCellIndex
                                    /* SPR 11509 Fix Start */
                                    ,LTE_FALSE);

                            /* SPR 11509 Fix End */
                        }
                    }
                    break;
                }

                /* Intensional switch case fall through */
                case  MAC_SS_PDU_UCI_CQI :
                case  MAC_SS_PDU_UCI_SR :
                case  MAC_SS_PDU_UCI_HARQ :
                case  MAC_SS_PDU_UCI_SR_HARQ :
                case  MAC_SS_PDU_UCI_CQI_HARQ :
                case  MAC_SS_PDU_UCI_CQI_SR :
                case  MAC_SS_PDU_UCI_CQI_SR_HARQ :
                case  MAC_SS_PDU_SRS :
                /* SPR 11215 Changes Start */
#if defined(FAPI_1_1_COMPLIANCE) || defined(FAPI_4_0_COMPLIANCE)
                /* SPR 11215 Changes End */
                case  MAC_SS_PDU_HARQ_BUFFER_RELEASE :
#endif
                default :
                {
                    /* No action Required */
                    break;
                }
            }
        }
    }
}
/****************************************************************************
 * Function Name  : handleFapiMsgInvalidSfnForHiDci0 
 * Inputs         : FAPI_errMsgBody1_st   errMsgBody1,
 *                  tickType_t currentTick
 *                  internalCellIndex
 * Outputs        : NA. 
 * Returns        : void
 * Description    : This function handles the Error MSG_INVALID_SFN_ERR 
 *                  Indication received in HI_DCI0.request.
 *                  - if the difference in SFN/SF is more than a threshold, 
 *                    UL HARQ timer expiry functionality will perform the UL 
 *                    HARQ recovery. 
 *                  - MAC will also try to perform adaptive retransmission in
 *                    this case. 
 *                  - If the difference is more or equal to the threshold, 
 *                    all UL HARQ processes will be freed. 
 *                  - MAC will not update its internal TTI due to the error 
 *                    above and will only rely upon SubFrame.Indication to 
 *                    align its internal TTI. 
 *                  - If power control commands had been sent to UE(s) in the
 *                    HI_DCI0.Request for DCI 0/3/3A for which Error 
 *                    Indication has been received, then MAC will update the
 *                    total power correction sent to the UE as if the latest
 *                    TPC was not sent in HI_DCI0.Request.
 ****************************************************************************/
/*CID 83603 Fix Start*/
STATIC  void  handleFapiMsgInvalidSfnForHiDci0(
        FAPI_errMsgBody1_st *errMsgBody1_p, 
        /* SPR 15909 fix start */
        tickType_t currentTick,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex )
{
    ErrIndPowerControlType  pcType = ERR_IND_TPC_FOR_PUSCH;
    UInt16 sysFrameNum = 0;
    UInt16 subFrameNum = 0;
    UInt16 recvdSysFrameNum = 0;
    UInt16 recvdSubFrameNum = 0;
    UInt16 totalTickMissErrIndUl = 0;
    UInt8  mapIdx = 0; 
    /* SPR 11238 fix start */
    UInt8  harqRttTimerFactor = 0;
    /* SPR 11238 fix end */

    /* Received  SFN/SF in error message */
    recvdSysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF( 
            errMsgBody1_p->recvdSfnSf );
    recvdSubFrameNum = GET_SUBFRAME_FROM_SFNSF(
            errMsgBody1_p->recvdSfnSf);   

    /* expected SFN/SF in error message */
    sysFrameNum = GET_SYSFRAMENUM_FROM_SFNSF(
            errMsgBody1_p->expectedSfnSf); 
    subFrameNum = GET_SUBFRAME_FROM_SFNSF(
            errMsgBody1_p->expectedSfnSf);    

    LOG_MAC_MSG( MAC_PHY_ERR_SFNSF_MSG_ID, LOGERROR, MAC_L1_INF,
            getCurrentTick(), countPhyErrorIndication_g[internalCellIndex], 
            recvdSysFrameNum, recvdSubFrameNum, sysFrameNum, 
            subFrameNum, 0,0, __func__,"MsgInvalidSFN");

    /* Assuming Recvd TTI is old and expected is new that is not processed */
    /* SPR 15909 fix start */
    tickType_t expTTI = (sysFrameNum * MAX_SUB_FRAME) + subFrameNum;
    tickType_t recvTTI = (recvdSysFrameNum * MAX_SUB_FRAME) + recvdSubFrameNum ;
    tickType_t maxTTI  = ( MAX_SYS_FRAME_NUM * MAX_SUB_FRAME) + 
        (MAX_SUB_FRAME - 1);
    /* SPR 15909 fix end */

    /* Calculate the differnece between expected and received SFN/SF */
    if( recvTTI < expTTI)
    {
        totalTickMissErrIndUl_g[internalCellIndex] = expTTI - recvTTI;
    }
    else
    {
        /* wrap around case */
        totalTickMissErrIndUl_g[internalCellIndex] = ((maxTTI - recvTTI) + expTTI) + 1;
    }

    /* Copy the Tick Miss information to local variable to send it 
     * later to OAM as it will be reset by EL called in TDD */
    totalTickMissErrIndUl = totalTickMissErrIndUl_g[internalCellIndex];

    /* MAC will not update its internal TTI due to this error and will
     * only rely upon SubFrame.Indication to align its internal TTI */

#ifdef TDD_CONFIG
    /* SPR 11238 fix start */
    harqRttTimerFactor = roundTripTime( (recvdSubFrameNum + 
                getExactULSubFrame( recvdSubFrameNum, internalCellIndex)),
            /* CA TDD CHANGES START */
            internalCellIndex
            /* CA TDD CHANGES END */
            );
    if( totalTickMissErrIndUl_g[internalCellIndex] >= harqRttTimerFactor )
        /* SPR 11238 fix end */
    {
        UInt8 airSf = subFrameNum;
        UInt16 airSfn = sysFrameNum;
        ExecutionLegFunctionArg execLegFuncArg = {0};
        if (MAC_SUCCESS == calculateHarqExpTickMissAndArgs(
                    &execLegFuncArg, ELARG_UL_HARQ_EXPIRY_U_SF,
                    ( currentTick - PHY_ERROR_IND_DELAY_UL ) + 
                    totalTickMissErrIndUl_g[internalCellIndex], 
                    airSf, airSfn, totalTickMissErrIndUl_g[internalCellIndex],
                    /* SPR 11110 fix start */
                    ERROR_SCENARIO ) )
            /* SPR 11110 fix end */
        {
            executionFunctionArrTdd_g[PROCESS_UL_HARQ_TIMER_EXPIRY](
                    &execLegFuncArg);
        }
    }
#elif FDD_CONFIG
    /* SPR 11238 fix start */
    harqRttTimerFactor = MAX_NUM_OF_TICK_MISSED;
    /* SPR 11238 fix end */
    if( totalTickMissErrIndUl_g[internalCellIndex] > 1)
    {
        freeTTIBundlingNodesForTickMiss( totalTickMissErrIndUl_g[internalCellIndex],
                ( currentTick - PHY_ERROR_IND_DELAY_UL ) + 
                totalTickMissErrIndUl_g[internalCellIndex], PHY_DELAY ,internalCellIndex);
    }
#endif
    LOG_MAC_MSG( MAC_ON_SFN_MISMATCH, LOGDEBUG, MAC_L1_INF,
            getCurrentTick(), __LINE__, totalTickMissErrIndUl, 
            MAX_NUM_OF_TICK_MISSED, numErrIndHiDci0_g[internalCellIndex], 0, 
            0,0, __func__, "UL");

    /* Notify OAM for Error Indication received from L1 */ 
    /* SPR 11238 fix start */
    if( harqRttTimerFactor <= totalTickMissErrIndUl )
        /* SPR 11238 fix end */
    {
        if( 0 == numErrIndHiDci0_g[internalCellIndex] )
        {
            /* Set the number of error indications that can be received for
             * HI_DCI0 so that notifications to OAM is not flooded */
            numErrIndHiDci0_g[internalCellIndex] = totalTickMissErrIndUl;

            sendMacPhyOamErrIndication( 
                    MAC_PHY_OAM_ERR_SFN_OUT_OF_SYNC_BY_MORE_THAN_THRESHOLD, 
                    internalCellIndex );
        }
    }
    else
    {
        /* Reset the variables as UL HARQ recovery will be done 
         * automatically once timer gets expired */
        totalTickMissErrIndUl_g[internalCellIndex] = 0;
    }

    mapIdx = recvTTI % MAX_TPC_CONTAINER_MAP_SIZE; 
    /* Revert the TPC if TPC command sent */
    /* +- SPR 17777 */
    updateTpcForPhyErrorInd(pcType, mapIdx,internalCellIndex);
    /* +- SPR 17777 */
}
/*CID 83603 Fix End*/
/* SPR 9829 changes start */
/*****************************************************************************
 * Function Name  : macProcessVendorSpecificFieldsUlschInd
 * Inputs         : ueMeasInfoForUEs_p -pointer to the FAPI_UEMeasurementIndication_st
 *                                      structure
 *                  internalCellIndex - Cell_index at MAC
 *                  recvTTI - It represents the tick of the message receiived 
 * Outputs        : None.
 * Returns        : void
 * Description    : This function handles the vendor specific fields received in 
 *                  ULSCH Indication. 
 ****************************************************************************/
                    /*CID 69606 Fix start*/
                    STATIC  void  macProcessVendorSpecificFieldsUlschInd(FAPI_UEMeasurementIndication_st *ueMeasInfoForUEs_p,
                            /* SPR 15909 fix start */
                            tickType_t recvTTI,
                            /* SPR 15909 fix end */
                            InternalCellIndex internalCellIndex)
{
    UInt16   rnti     = 0;
    UInt8    count    = 0;
    UInt16   ueIndex  = 0;
    /* +- SPR 18268 */
    UInt16    ueCount  = 0;
    UInt16    numOfUEs =	 0;
    /* +- SPR 18268 */
    UInt8 validReports = 0;
    RNTIInfo *rntiInfo_p = PNULL;
    EcidReportParams *eCidReportParams_p = PNULL;
    TempCRNTICtx *tempUEContext_p = PNULL;
    ULUEContext  *ulUEContext_p  = PNULL;
    DLUEContext  *dlUEContext_p  = PNULL;
    FAPI_AOA_REL9TARecievedForUE_st *aoaRel9TAInfo = PNULL;
    UInt16 timingAdvanceR9 = INVALID_TIMING_ADVANCE_R9_VALUE;    
    UInt16 *angleOfArrival = PNULL;

    if(PNULL != ueMeasInfoForUEs_p)
    {
        numOfUEs = ueMeasInfoForUEs_p->numOfUEs;
        for(ueCount = 0; ueCount < numOfUEs; ueCount++)
        {
            rnti = MAC_PHY_CONVERT_16(ueMeasInfoForUEs_p->aoaRel9TAInfo[ueCount].rnti);
            rntiInfo_p = getUeIdxFromRNTIMap(rnti,internalCellIndex);
            if (PNULL != rntiInfo_p)
            {   
                if(TCRNTI == rntiInfo_p->rntiFlag && (FREERNTI != rntiInfo_p->rntiFlag))
                {
                    VALIDATE_AND_UPDATE_TA1_AOA_REPORTS(
                            ueMeasInfoForUEs_p->aoaRel9TAInfo,
                            timingAdvanceR9,
                            angleOfArrival,
                            validReports,
                            ueCount );
                    tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->\
                                      tempCRNTICtxArray[rntiInfo_p->index].tempCRNTICtx_p;
                    if(PNULL != tempUEContext_p)
                    {
                        tempUEContext_p->validReports = validReports;
                        tempUEContext_p->type1TAValue = timingAdvanceR9;
                        tempUEContext_p->type1TaAOAReceivedTTI = recvTTI;
                        for(count = 0; count < MAX_NUM_ANTENNA; count ++)
                        {
                            tempUEContext_p->angleOfArrival[count] =
                                *(angleOfArrival + count);
                        }
                    }
                    continue;
                }

                else
                {
                    ueIndex = rntiInfo_p->index;
                    /* +SPR 10769 Changes */
                    if( ueIndex < MAX_UE_SUPPORTED )
                    {
                        ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
                        if((TRUE != ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag) && \
                                PNULL != ulUEContext_p)
                        {
                            eCidReportParams_p = &ulUEContext_p->eCidReportParams;
                            VALIDATE_AND_UPDATE_TA1_AOA_REPORTS(
                                    ueMeasInfoForUEs_p->aoaRel9TAInfo,
                                    timingAdvanceR9,
                                    angleOfArrival,
                                    validReports, 
                                    ueCount);

                            SAVE_ECID_REPORTS_IN_UE_CONTXT(eCidReportParams_p,
                                    timingAdvanceR9,
                                    angleOfArrival,
                                    recvTTI);

                            if((NO_REPORT_TYPE_REQUIRED != 
                                        eCidReportParams_p->typeofReportsRequired) &&
                                    (TYPE_2_TA_REPORT_REQUIRED != 
                                     eCidReportParams_p->typeofReportsRequired))
                            {
                                checkValidReportsAndPutEntryInEcidMgr(ueIndex,
                                        validReports, internalCellIndex);
                            }
                        }

                        dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

                        if((TRUE != dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag) && \
                                PNULL != dlUEContext_p)
                        {     
                            aoaRel9TAInfo =  &ueMeasInfoForUEs_p->aoaRel9TAInfo[ueCount];
                            for (count = 0; count < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
                                    ->cellParams_p->numOfTxAnteenas; count++)
                            {
                                if(MAX_VALUE_OF_AOA >= aoaRel9TAInfo->AoA[count])
                                {
                                    dlUEContext_p->AoA[count] = 
                                        calculateTimeAvgAoA(dlUEContext_p->AoA[count] ,aoaRel9TAInfo->AoA[count]); 
                                }
                            }
                        }
                    }
                    /* SPR 10769 Changes */
                }
            }
        }
    }
}
/*CID 69607 Fix Start*/
/*****************************************************************************
 * Function Name  : macProcessVendorSpecificFieldsRachInd
 * Inputs         : ueMeasInfoInRACHForUEs_p -pointer to the FAPI_UEMeasurementInRACH_st
 *                                            structure
 *                  recvTTI - It represents the tick of the message receiived 
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : None
 * Returns        : void
 * Description    : This function handles the vendor specific fields received in 
 *                  RACH Indication. 
 ****************************************************************************/

                STATIC  void macProcessVendorSpecificFieldsRachInd(FAPI_UEMeasurementInRACH_st *ueMeasInfoInRACHForUEs_p,
                        /* SPR 15909 fix start */
                        tickType_t recvTTI,
                        /* SPR 15909 fix end */ 
                        InternalCellIndex internalCellIndex)
{
    /* + E_CID_5.3 */
    UInt8 count = 0;
    UInt8 preamble = 0;
    /* +- SPR 18268 */
    UInt16 ueIndex = INVALID_UE_INDEX;
    /* +- SPR 18268 */
    UInt8 eCidNumOfpreambles = 0;
    UInt16 timingAdvanceR9 =  INVALID_TIMING_ADVANCE_R9_VALUE;
    ULUEContext *ulUEContext_p = PNULL;
    ContentionFreePreamble *contFreePreamble_p = PNULL;
    UInt8 numOfAntenna = 0;
    /* - E_CID_5.3 */

    if(PNULL != ueMeasInfoInRACHForUEs_p)
    {
        eCidNumOfpreambles = ueMeasInfoInRACHForUEs_p->numOfpreambles;

        for (count = 0; count < eCidNumOfpreambles; count++)
        {
            preamble = ueMeasInfoInRACHForUEs_p->rel9TAInfo[count].preamble;
            contFreePreamble_p = getContentionFreePreambleInfo(preamble,internalCellIndex);
            timingAdvanceR9 = (MAX_TIMING_ADVANCE_R9_VALUE >=
                    ueMeasInfoInRACHForUEs_p->rel9TAInfo[count].timingAdvanceR9)?\
                ueMeasInfoInRACHForUEs_p->rel9TAInfo[count].timingAdvanceR9:\
                INVALID_TIMING_ADVANCE_R9_VALUE;

            if(PNULL != contFreePreamble_p)
            {
                ueIndex = contFreePreamble_p->ueIdentifier;
                /*Store TA R9 value in temp contxt and updated it in Temp UE
                 * context*/
                /*SPR 12275 fix, adding validation for ueIndex*/ 
                if(INVALID_UE_ID <= ueIndex) 
                {  
                    continue;
                }
                ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
                if(PNULL != ulUEContext_p)
                {

                    /*If RRM requested for type1 and/or type2 TA reports then */
                    if(ulUEContext_p->eCidReportParams.typeofReportsRequired & (TRUE << 1))
                    {
                        ulUEContext_p->eCidReportParams.type2TAValue = timingAdvanceR9;
                        if(INVALID_TIMING_ADVANCE_R9_VALUE != timingAdvanceR9)
                        {
                            ulUEContext_p->eCidReportParams.type2TAreceivedTTI = recvTTI;
                            if(TYPE_2_TA_REPORT_REQUIRED == 
                                    ulUEContext_p->eCidReportParams.typeofReportsRequired)
                            {
                                ulUEContext_p->eCidReportParams.reportStatus = REPORTS_RECEIVED;
                                putEntryInEcidMeasReportQueue(ueIndex, \
                                        ulUEContext_p->eCidReportParams.typeofReportsRequired,\
                                        MAC_SUCCESS,internalCellIndex);
                            }
                            else if(WAIT_FOR_TA_2_REPORT_ONLY_FOR_RELEASE_8_UE == \
                                    ulUEContext_p->eCidReportParams.reportStatus)
                            {
                                ulUEContext_p->eCidReportParams.type1TAValue = INVALID_TIMING_ADVANCE_R9_VALUE;
                                for (numOfAntenna  = 0; numOfAntenna < MAX_NUM_ANTENNA; numOfAntenna ++)
                                    ulUEContext_p->eCidReportParams.angleOfArrival[numOfAntenna] = INVALID_ANGLE_OF_ARRIVAL_VALUE;
                                putEntryInEcidMeasReportQueue(ueIndex,\
                                        ulUEContext_p->eCidReportParams.typeofReportsRequired,\
                                        MAC_PARTIAL_SUCCESS,internalCellIndex); 

                            }
                            else
                            {
                                /*if UE is exists then store UE index in temp contxt
                                 * which will be used to find for which MSG3 reception was failed */
                                tempTimingAdvanceR9_g[internalCellIndex][preamble].ueIndex = ueIndex;

                                ulUEContext_p->eCidReportParams.reportStatus = 
                                    WAIT_FOR_REPORTS_TA_1_OR_AOA_AFTER_TA_2_RECEIVED;
                            }
                        }
                        else
                        {
                            if(TYPE_2_TA_REPORT_REQUIRED != 
                                    ulUEContext_p->eCidReportParams.typeofReportsRequired)
                            {
                                /*if UE is exists then store UE index in temp contxt
                                 * which will be used to find for which MSG3 reception was failed */
                                tempTimingAdvanceR9_g[internalCellIndex][preamble].ueIndex = ueIndex;

                                ulUEContext_p->eCidReportParams.type2TAreceivedTTI = recvTTI;
                                ulUEContext_p->eCidReportParams.reportStatus =
                                    WAIT_FOR_REPORTS_TA_1_OR_AOA_AFTER_TA_2_RECEIVED;
                            }
                            else
                            {
                                putEntryInEcidMeasReportQueue(ueIndex,\
                                        NO_REPORT_TYPE_REQUIRED,\
                                        MAC_FAILURE,internalCellIndex); 
                            }

                        }
                    }
                }
            }
            else
            {
                tempTimingAdvanceR9_g[internalCellIndex][preamble].type2TAValue = timingAdvanceR9;
                tempTimingAdvanceR9_g[internalCellIndex][preamble].type2TAreceivedTTI = recvTTI;
            }
        }
    }
}
/*CID 69607 Fix Start*/
/*CID 69608 Fix start*/
/*****************************************************************************
 * Function Name  : macProcessVendorSpecificFieldsSRSCQIInd
 * Inputs         : ueMeasInfoForUEs_p -pointer to the FAPI_UEMeasurementIndication_st
 *                                      structure
 *                  recvTTI - It represents the tick of the message receiived 
 *                  internalCellIndex - Cell_index at MAC
 * Outputs        : None
 * Returns        : void
 * Description    : This function handles the vendor specific fields received in 
 *                  CQI Indication and SRS Indication as well. 
 ****************************************************************************/

                                STATIC  void macProcessVendorSpecificFieldsSRSCQIInd(FAPI_UEMeasurementIndication_st *ueMeasInfoForUEs_p,
                                        /* SPR 15909 fix start */
                                        tickType_t recvTTI,
                                        /* SPR 15909 fix end */
                                        InternalCellIndex internalCellIndex)
{
    UInt16  ueIndex       = 0;
    /* + E_CID_5.3 */
    /* +- SPR 18268 */
    UInt16 count = 0;
    UInt16 numOfUEs = 0;
    /* +- SPR 18268 */
    UInt16 timingAdvanceR9 = 0;
    UInt16 rnti = 0;
    UInt16 *angleOfArrival = PNULL;
    UInt8 validReports = 0;
    EcidReportParams   *eCidReportParams_p = PNULL;
    /* - E_CID_5.3 */
    ULUEContext *ulUEContext_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
    RNTIInfo *rntiInfo_p  = PNULL;
    UInt8 counter = 0;   
    FAPI_AOA_REL9TARecievedForUE_st *aoaRel9TAInfo = PNULL;

    if(PNULL != ueMeasInfoForUEs_p)
    {
        numOfUEs = ueMeasInfoForUEs_p->numOfUEs;
        for(count = 0; count < numOfUEs; count++)
        {
            rnti = MAC_PHY_CONVERT_16(ueMeasInfoForUEs_p->aoaRel9TAInfo[count].rnti);
            rntiInfo_p = getUeIdxFromRNTIMap(rnti,internalCellIndex);
            if(PNULL != rntiInfo_p)
            {
                if(FREERNTI != rntiInfo_p->rntiFlag)
                {
                    ueIndex = rntiInfo_p->index;
                    ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
                    if((TRUE != ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag) && \
                            (PNULL != ulUEContext_p))
                    {

                        eCidReportParams_p = &ulUEContext_p->eCidReportParams;

                        VALIDATE_AND_UPDATE_TA1_AOA_REPORTS(
                                ueMeasInfoForUEs_p->aoaRel9TAInfo,
                                timingAdvanceR9,
                                angleOfArrival,
                                validReports,
                                count);

                        SAVE_ECID_REPORTS_IN_UE_CONTXT(eCidReportParams_p,
                                timingAdvanceR9,
                                angleOfArrival,
                                recvTTI);

                        if((NO_REPORT_TYPE_REQUIRED !=
                                    eCidReportParams_p->typeofReportsRequired) &&\
                                (TYPE_2_TA_REPORT_REQUIRED !=
                                 eCidReportParams_p->typeofReportsRequired))
                        {
                            checkValidReportsAndPutEntryInEcidMgr(ueIndex,
                                    validReports,internalCellIndex);
                        }
                    }

                    if (dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag)
                    {
                        lteWarning(" Pending Delete Flag True for UE Index %d %s",
                                ueIndex,__func__);
                        continue ;
                    }
                    dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
                    /* SPR 7973 chages start */
                    if( PNULL ==  dlUEContext_p )
                    {
                        continue;
                    }
                    /* SPR 7973 chages end */

                    aoaRel9TAInfo =  &ueMeasInfoForUEs_p->aoaRel9TAInfo[count];
                    for (counter = 0; counter < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
                            ->cellParams_p->numOfTxAnteenas; counter++)
                    {
                        if(MAX_VALUE_OF_AOA >= aoaRel9TAInfo->AoA[counter])
                        {
                            dlUEContext_p->AoA[counter] = 
                                calculateTimeAvgAoA(dlUEContext_p->AoA[counter] ,aoaRel9TAInfo->AoA[counter]);  
                        }
                    }

                }
            }
        }
    }

}
/*CID 69608 Fix end*/
/* SPR 9829 changes end */

/** SPR 12364 fix Start **/
/*****************************************************************************
 * Function Name  : harqParamBackupForUlSchInd
 * Inputs         : ULHarqInfo
 * Outputs        : None 
 * Returns        : None 
 * Description    : This function takes backup of few harq parameters which 
 will be later used when ulSchInd is received.
 *****************************************************************************/
STATIC  void harqParamBackupForUlSchInd(ULHarqInfo *ulHarqInfo)
{
    UInt8 count = 0;
    /*SPR 16417 Changes Start*/
    ulHarqInfo->ulSchReqInfo.riLength = ulHarqInfo->riLength;
#ifdef FDD_CONFIG
    ulHarqInfo->ulSchReqInfo.isTTIBundled = ulHarqInfo->isTTIBundled;
    ulHarqInfo->ulSchReqInfo.isSpsEnabled = ulHarqInfo->isSpsEnabled;
    ulHarqInfo->ulSchReqInfo.isTTIDataRcvd = ulHarqInfo->isTTIDataRcvd;
#ifdef  HD_FDD_CONFIG
    ulHarqInfo->ulSchReqInfo.hdFddUlNackHandlingFlag = ulHarqInfo->hdFddUlNackHandlingFlag;
#endif
#if defined(KPI_STATS) || defined(PERF_STATS)
    ulHarqInfo->ulSchReqInfo.rbForKpiStats = ulHarqInfo->rbForKpiStats;
#endif
#endif
    /*SPR 16417 Changes End*/
    for(count = 0; count < MAX_LOGICAL_CHANNEL; count++)
    {
        ulHarqInfo->ulSchReqInfo.ulTokenInfo[count].isLCScheduled = ulHarqInfo->ulTokenInfo[count].isLCScheduled;
        ulHarqInfo->ulSchReqInfo.ulTokenInfo[count].scheduledToken = ulHarqInfo->ulTokenInfo[count].scheduledToken;
        /* Now reset these values in actual harq context so that it doesn't effect scheduling in this tti */
        ulHarqInfo->ulTokenInfo[count].isLCScheduled = FALSE;
        ulHarqInfo->ulTokenInfo[count].scheduledToken = 0;
    }
    for(count = 0; count < NUM_OF_LCG; count++)
    {
        ulHarqInfo->ulSchReqInfo.grantPerLcg[count] = ulHarqInfo->grantPerLcg[count];
    }
}
/** SPR 12364 fix Ends **/
/* Cyclomatic_complexity_changes_start */
/*****************************************************************************
 * Function Name  : populateAndPushAcknNackNode
 * Inputs         : dlSubFrameNum -  subframe at which DL scheduled
 *                  prevContainerIndex - container index for DL scheduled in last frame
 *                  containerIndex - current container index
 *                  ueIndex -  Index of UE
 *                 operation -  ACK or NACK
 *                  recvTTI,sf
 *
 * Outputs        : None
 * Returns        : void
 * Description    :This function will enque acknack node to queue after
 *                 filling required fields in node
 *****************************************************************************/

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
            )
{
    tddAckNackQueueNode *ackNackEntry_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
    /* + CA_TDD_HARQ_CHANGES */
    DLUESCellContext *dlUeScellContext_p = PNULL;
    UInt8 isScheduledInPCell = LTE_FALSE;
    UInt8 isScheduledInSCell = LTE_FALSE;
    UInt8 packetSubFrameNum = (recvTTI) % MAX_SUB_FRAME;
    /* + SPR_13810 */
    dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    /* - SPR_13810 */
    dlUeScellContext_p = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX];
    /* - CA_TDD_HARQ_CHANGES */
    if((dlUEContext_p->pdschTxMapContainer[containerIndex][sf])&(bitmask[dlSubFrameNum]))
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,Log_DL_HARQ,
                "\nReceiver : Before Processing the HARQ Acknowledgement"
                "Current_SF[%d] Tx_Map_Conatiner_Index[%d]"
                " HARQ_Cont_Indx [%d], HARQ_Process_ID [%d], Bit_Map [%x]",
                sf, containerIndex, prevContainerIndex,
                dlUEContext_p->harqProcessId[prevContainerIndex][dlSubFrameNum],
                dlUEContext_p->pdschTxMapContainer[containerIndex][sf]);

        dlUEContext_p->pdschTxMapContainer[containerIndex][sf] =
            (dlUEContext_p->pdschTxMapContainer[containerIndex][sf]) & (resetBitmask[dlSubFrameNum]) ;
        /* + CA_TDD_HARQ_CHANGES */
        isScheduledInPCell = LTE_TRUE;
    }

    if ((IS_CA_ENABLED()) && (dlUeScellContext_p) &&
            ( (dlUeScellContext_p->pdschTxMapContainerForSCell[containerIndex][sf]) &
              (bitmask[dlSubFrameNum])) )
    {
        dlUeScellContext_p->pdschTxMapContainerForSCell[containerIndex][sf] =
            ( (dlUeScellContext_p->pdschTxMapContainerForSCell[containerIndex][sf]) 
              & (resetBitmask[dlSubFrameNum])) ;

        isScheduledInSCell = LTE_TRUE;
    }

    if (isScheduledInPCell || isScheduledInSCell)
    {   
        /* - CA_TDD_HARQ_CHANGES */
        /* + SPR_8600 */
        GET_MEM_FROM_POOL(tddAckNackQueueNode,ackNackEntry_p,
                sizeof(tddAckNackQueueNode), PNULL);
        /*ackNackEntry_p = (tddAckNackQueueNode *)getMemFromPool(
         *                   sizeof(tddAckNackQueueNode), PNULL);*/
        /* - SPR_8600 */
        if( ackNackEntry_p )
        {
            ackNackEntry_p->operationType = operation;
            ackNackEntry_p->ueIndex = ueIndex;
            ackNackEntry_p->packetSubFrame = packetSubFrameNum;
            ackNackEntry_p->indexSfn      = containerIndex;
            /* + CA_TDD_HARQ_CHANGES */

            if(isScheduledInPCell)
            {    
                ackNackEntry_p->harqProcessId[PCELL_SERV_CELL_IX] = 
                    dlUEContext_p->harqProcessId[prevContainerIndex][dlSubFrameNum];

                dlUEContext_p->harqProcessId[prevContainerIndex][dlSubFrameNum] = 
                    INVALID_HARQ_ID ;
            }
            else 
            {
                ackNackEntry_p->harqProcessId[PCELL_SERV_CELL_IX] = 
                    INVALID_HARQ_ID;
            }    
            if (isScheduledInSCell) 
            {
                ackNackEntry_p->harqProcessId[START_SCELL_INDEX] = 
                    dlUeScellContext_p->harqProcessSCellId[prevContainerIndex][dlSubFrameNum];

                dlUeScellContext_p->harqProcessSCellId[prevContainerIndex][dlSubFrameNum] 
                    = INVALID_HARQ_ID ;
            }   
            else
            {
                ackNackEntry_p->harqProcessId[START_SCELL_INDEX] = INVALID_HARQ_ID;
            } 

            ackNackEntry_p->modeType = modeType;

            /* - CA_TDD_HARQ_CHANGES */
            /* + 32.425 Events changes */
#ifdef KPI_STATS
            ackNackEntry_p->recvTTI = recvTTI;
#endif
            /* EICIC +*/
            ackNackEntry_p->dlSubframeNum = dlSubFrameNum;
            /*EICIC -*/

            /* - 32.425 Events changes */
            /* + SPS_TDD_Changes */
            ackNackEntry_p->schUsed= dlUEContext_p->dlHarqContext_p->\
                                     harqProcess[ackNackEntry_p->harqProcessId[PCELL_SERV_CELL_IX]].schUsed;
            /* - SPS_TDD_Changes */
            /*Coverity ID <> Fix Changes Start*/
            if(pushNode(&(tddAckNackQueue_g[internalCellIndex]),
                        &(ackNackEntry_p->ackNackAnchor)))
            {
                LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGFATAL,L2_SYS_FAIL,\
                        getCurrentTick(),\
                        __LINE__,0,0,0,0,\
                        0.0,0.0,__FILE__,__func__);

                freeMemPool(ackNackEntry_p);
            }
            /*Coverity ID <> Fix Changes End*/
        }
        else
        {
            lteWarning("Invalid Harq Id is recvd\n");
        }
    }
    LTE_MAC_UT_LOG(LOG_DETAIL,Log_DL_HARQ,
            "\nReceiver : After Processing the HARQ Acknowledgement"
            "Current_SF[%d] Tx_Map_Conatiner_Index[%d]"
            " HARQ_Cont_Indx [%d], HARQ_Process_ID [%d], Bit_Map [%x]",
            sf, containerIndex, prevContainerIndex,
            dlUEContext_p->harqProcessId[prevContainerIndex][dlSubFrameNum],
            dlUEContext_p->pdschTxMapContainer[containerIndex][sf]);
    return;
}
#endif
/*****************************************************************************
 * Function Name  : decodeAperiodicCqi
 * Inputs         : cqiReportType,
 *                  data_p,
 *                  ueIndex,
 *                  ri,
 *                  ulUEContext_p,
 *                  dlUEContext_p,
 *                  recvTTI,
 *                  internalCellIndex,
 *                  numOfBitsDecoded,
 *                  transmissionMode,
 *                  servCellId
 * Outputs        : None
 * Returns        : void
 * Description    :This function will decode Aperiodic Cqi
 ******************************************************************************/

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
                        InternalCellIndex servCellId)
{

    switch(cqiReportType)
    {
        case APERIODIC_MODE_1_2: 
        decodeAperiodicCqiMode1_2(data_p,
                ueIndex, 
                ri,
                ulUEContext_p,
                /* + CQI_5.0 */
                recvTTI,
                internalCellIndex,
                /* CA_phase2_csi_code */
                numOfBitsDecoded_p);
        /* - CQI_5.0 */
        /* CA_phase2_csi_code Start */
        if(ulUEContext_p->internalCellIndex == internalCellIndex)
        {
            /* Pcell */
            dlUEContext_p->dlCQIInfo.latestAperiodicReportRcvd = TRUE;
        }
        else
        {
            /* Scell */
            /*Coverity Fix 97505*/
            if(servCellId<=MAX_NUM_SCELL)
            {
                dlUEContext_p->dlUeScellContext_p[servCellId]->ScelldlCQIInfo.
                    latestAperiodicReportRcvd = TRUE;
            }
        }
        /* CA_phase2_csi_code End */
        break;

        case APERIODIC_MODE_2_0:

        decodeAperiodicCqiMode2_0(data_p,
                ueIndex, 
                transmissionMode,
                /* + CQI_5.0 */
                ri,
                /* - CQI_5.0 */
                ulUEContext_p,
                internalCellIndex,
                /* CA_phase2_csi_code */
                numOfBitsDecoded_p);
        break;    

        case APERIODIC_MODE_2_2:

        /* + CQI_4.1 */
        decodeAperiodicCqiMode2_2(data_p, 
                ueIndex,
                ri,
                ulUEContext_p,
                internalCellIndex,
                /* CA_phase2_csi_code */
                numOfBitsDecoded_p);
        break; 
        /* - CQI_4.1 */

        case APERIODIC_MODE_3_0:

        decodeAperiodicCqiMode3_0(data_p, 
                ueIndex,
                transmissionMode,
                /* + CQI_5.0 */
                ri,
                /* - CQI_5.0 */
                ulUEContext_p,
                internalCellIndex,
                /* CA_phase2_csi_code */
                numOfBitsDecoded_p);
        break;    

        case APERIODIC_MODE_3_1:

        /* + CQI_4.1 */
        decodeAperiodicCqiMode3_1(data_p, 
                ueIndex,
                ri,
                /* + TM6_5.2 */
                transmissionMode,
                /* - TM6_5.2 */
                ulUEContext_p,
                internalCellIndex,
                /* CA_phase2_csi_code */
                numOfBitsDecoded_p);
        break;

        default:
        lteWarning("Report Type Decoding not supported %d\n", 
                /* PURIFY FIXES 2.4.1 start */
                (UInt32)cqiReportType);
        /* PURIFY FIXES 2.4.1  end*/
        break;
    }

    return;
}
/*****************************************************************************
 * Function Name  : decodePeriodicCqi
 * Inputs         : cqiReportType,
 *                  data_p,
 *                  ueIndex,
 *                  ri,
 *                  ulUEContext_p,
 *                  dlUEContext_p,
 *                  internalCellIndex,
 *                  transmissionMode,
 *                  servCellId,
 *                  subFrameNum
 * Outputs        : None
 * Returns        : void
 * Description    :This function will decode Aperiodic Cqi
 *******************************************************************************/

void  decodePeriodicCqi(ueExpectedReportType cqiReportType,
                        UInt8 *data_p,
                        UInt16 ueIndex,
                        /* +- SPR 17777 */
                        DLUEContext *dlUEContext_p,
                        UInt8 transmissionMode,
                        ULUEContext *ulUEContext_p,
                        InternalCellIndex internalCellIndex,
                        InternalCellIndex servCellId,
                        UInt16 subFrameNum)
{
/* SPR 22569 Fix + */
#ifdef INTEL_NOCA_L1
/* SPR 22569 Fix - */
    UInt8   *ri_p         = PNULL;

    if(ulUEContext_p->internalCellIndex == internalCellIndex)
    {
        ri_p = &ulUEContext_p->pucchConfigInfo.ueReportedRI;
    }
    else
    {
#endif
        servCellId = getServeCellIndexFromInternalCellId(dlUEContext_p,
                internalCellIndex);
/* SPR 22569 Fix + */
#ifdef INTEL_NOCA_L1
/* SPR 22569 Fix - */
        ri_p = &ulUEContext_p->ulUeScellContext_p[servCellId]->scellpucchConfigInfo.ueReportedRI;
    }
#endif
    switch(cqiReportType)
    {
        case PERIODIC_MODE_1_0_TYPE_3: 
        if(ulUEContext_p->internalCellIndex == internalCellIndex)
        {
            /* Pcell */
            decodePeriodicCqiMode10Type3(data_p, 
                    ueIndex,
                    dlUEContext_p,
/* SPR 22569 Fix + */
#ifdef INTEL_NOCA_L1
/* SPR 22569 Fix - */
                    ri_p,
#else
                    &ulUEContext_p->pucchConfigInfo.ueReportedPeriodicRI,
#endif
                    internalCellIndex);
        }
        else
        {
            /* Scell */
            decodePeriodicCqiMode10Type3(data_p, 
                    ueIndex,
                    dlUEContext_p,
/* SPR 22569 Fix + */
#ifdef INTEL_NOCA_L1
/* SPR 22569 Fix - */
                    ri_p,
#else
                    &ulUEContext_p->ulUeScellContext_p[servCellId]->
                    scellpucchConfigInfo.ueReportedPeriodicRI,
#endif
                    internalCellIndex);
        }
        break;

        case PERIODIC_MODE_1_0_TYPE_4:

        if(ulUEContext_p->internalCellIndex == internalCellIndex)
        {
            /* Pcell */
            decodePeriodicCqiMode10Type4(data_p, 
                    ueIndex,
                    transmissionMode,
                    /* SPR 22098 + */
/* SPR 22569 Fix + */
#ifdef INTEL_NOCA_L1
/* SPR 22569 Fix - */
                    *ri_p,
#else
                    ulUEContext_p->pucchConfigInfo.ueReportedPeriodicRI,
#endif
                    /* SPR 22098 - */
                    ulUEContext_p,
                    internalCellIndex);
        }
        else
        {
            /* Scell */
            decodePeriodicCqiMode10Type4(data_p, 
                    ueIndex,
                    transmissionMode,
                    /* SPR 22098 + */
/* SPR 22569 Fix + */
#ifdef INTEL_NOCA_L1
/* SPR 22569 Fix - */
                    *ri_p,
#else
                    ulUEContext_p->ulUeScellContext_p[servCellId]->
                    scellpucchConfigInfo.ueReportedPeriodicRI,
#endif
                    /* SPR 22098 - */
                    ulUEContext_p,
                    internalCellIndex);
        }
        break;

        case PERIODIC_MODE_1_1_TYPE_2:

        if(ulUEContext_p->internalCellIndex == internalCellIndex)
        {
            decodePeriodicCqiMode11Type2(data_p, 
                    ueIndex, 
                    ulUEContext_p->pucchConfigInfo.ueReportedPeriodicRI,
                    ulUEContext_p,
                    internalCellIndex);
        }
        else
        {
            /* Scell */
            decodePeriodicCqiMode11Type2(data_p, 
                    ueIndex, 
                    ulUEContext_p->ulUeScellContext_p[servCellId]->
                    scellpucchConfigInfo.ueReportedPeriodicRI,
                    ulUEContext_p,
                    internalCellIndex);
        }
        break;

        case PERIODIC_MODE_1_1_TYPE_3:

        if(ulUEContext_p->internalCellIndex == internalCellIndex)
        {
            decodePeriodicCqiMode11Type3(data_p,
                    ueIndex,
                    dlUEContext_p,
/* SPR 22569 Fix + */
#ifdef INTEL_NOCA_L1
/* SPR 22569 Fix - */
                    ri_p,
#else
                    &ulUEContext_p->pucchConfigInfo.ueReportedPeriodicRI,
#endif
                    internalCellIndex);
        }
        else
        {
            /* Scell */
            decodePeriodicCqiMode11Type3(data_p,
                    ueIndex,
                    dlUEContext_p,
/* SPR 22569 Fix + */
#ifdef INTEL_NOCA_L1
/* SPR 22569 Fix - */
                    ri_p,
#else
                    &ulUEContext_p->ulUeScellContext_p[servCellId]->
                    scellpucchConfigInfo.ueReportedPeriodicRI,
#endif
                    internalCellIndex);
        }
        break;

        case PERIODIC_MODE_2_0_TYPE_1:

        if(ulUEContext_p->internalCellIndex == internalCellIndex)
        {
            decodePeriodicCqiMode20Type1(data_p, 
                    ueIndex,
                    transmissionMode,
                    ulUEContext_p->pucchConfigInfo.ueReportedPeriodicRI,
                    ulUEContext_p->subframeToBandwidthPartNo[subFrameNum],
                    internalCellIndex);
        }
        else
        {
            decodePeriodicCqiMode20Type1(data_p, 
                    ueIndex,
                    transmissionMode,
                    ulUEContext_p->ulUeScellContext_p[servCellId]->
                    scellpucchConfigInfo.ueReportedPeriodicRI,
                    /* SPR 12488 Fix */
                    ulUEContext_p->ulUeScellContext_p[servCellId]->subframeToBandwidthPartNo[subFrameNum],
                    internalCellIndex);
        }

        break;

        case PERIODIC_MODE_2_0_TYPE_3:

        if(ulUEContext_p->internalCellIndex == internalCellIndex)
        {
            decodePeriodicCqiMode20Type3(data_p, 
                    ueIndex,
                    dlUEContext_p,
/* SPR 22569 Fix + */
#ifdef INTEL_NOCA_L1
/* SPR 22569 Fix - */
                    ri_p,
#else
                    &ulUEContext_p->pucchConfigInfo.ueReportedPeriodicRI,
#endif
                    internalCellIndex);
        }
        else
        {
            decodePeriodicCqiMode20Type3(data_p, 
                    ueIndex,
                    dlUEContext_p,
/* SPR 22569 Fix + */
#ifdef INTEL_NOCA_L1
/* SPR 22569 Fix - */
                    ri_p,
#else
                    &ulUEContext_p->ulUeScellContext_p[servCellId]->
                    scellpucchConfigInfo.ueReportedPeriodicRI,
#endif
                    internalCellIndex);
        }
        break;

        case PERIODIC_MODE_2_0_TYPE_4:
        if(ulUEContext_p->internalCellIndex == internalCellIndex)
        {
            decodePeriodicCqiMode20Type4(data_p, 
                    ueIndex,
                    transmissionMode,
                    ulUEContext_p->pucchConfigInfo.ueReportedPeriodicRI,
                    ulUEContext_p,
                    internalCellIndex);
        }
        else
        {
            /* Scell */
            decodePeriodicCqiMode20Type4(data_p, 
                    ueIndex,
                    transmissionMode,
                    ulUEContext_p->ulUeScellContext_p[servCellId]->
                    scellpucchConfigInfo.ueReportedPeriodicRI,
                    ulUEContext_p,
                    internalCellIndex);
        }
        break;

        case PERIODIC_MODE_2_1_TYPE_1:
        if(ulUEContext_p->internalCellIndex == internalCellIndex)
        {
            decodePeriodicCqiMode21Type1(data_p, 
                    ueIndex,
                    ulUEContext_p->pucchConfigInfo.ueReportedPeriodicRI,
                    ulUEContext_p->subframeToBandwidthPartNo[subFrameNum],
                    internalCellIndex);
        }
        else
        {
            /* Scell */
            decodePeriodicCqiMode21Type1(data_p, 
                    ueIndex,
                    ulUEContext_p->ulUeScellContext_p[servCellId]->
                    scellpucchConfigInfo.ueReportedPeriodicRI,
                    /* SPR 12488 Fix */
                    ulUEContext_p->ulUeScellContext_p[servCellId]->subframeToBandwidthPartNo[subFrameNum],
                    internalCellIndex);
        }

        break;

        case PERIODIC_MODE_2_1_TYPE_2:
        if(ulUEContext_p->internalCellIndex == internalCellIndex)
        {
            decodePeriodicCqiMode21Type2(data_p, 
                    ueIndex, 
                    ulUEContext_p->pucchConfigInfo.ueReportedPeriodicRI,
                    ulUEContext_p,
                    internalCellIndex);
        }
        else
        {
            /* Scell */
            decodePeriodicCqiMode21Type2(data_p, 
                    ueIndex, 
                    ulUEContext_p->ulUeScellContext_p[servCellId]->
                    scellpucchConfigInfo.ueReportedPeriodicRI,
                    ulUEContext_p,
                    internalCellIndex);
        }
        break;

        case PERIODIC_MODE_2_1_TYPE_3:

        if(ulUEContext_p->internalCellIndex == internalCellIndex)
        {
            decodePeriodicCqiMode21Type3(data_p, 
                    ueIndex,
                    dlUEContext_p,
/* SPR 22569 Fix + */
#ifdef INTEL_NOCA_L1
/* SPR 22569 Fix - */
                    ri_p,
#else
                    &ulUEContext_p->pucchConfigInfo.ueReportedPeriodicRI,
#endif
                    internalCellIndex);
        }
        else
        {
            /* Scell */
            decodePeriodicCqiMode21Type3(data_p, 
                    ueIndex,
                    dlUEContext_p,
/* SPR 22569 Fix + */
#ifdef INTEL_NOCA_L1
/* SPR 22569 Fix - */
                    ri_p,
#else
                    &ulUEContext_p->ulUeScellContext_p[servCellId]->
                    scellpucchConfigInfo.ueReportedPeriodicRI,
#endif
                    internalCellIndex);
        }

        break;
        case PERIODIC_MODE_ALL_TYPE_3:
        if(ulUEContext_p->internalCellIndex == internalCellIndex)
        {
            decodePeriodicCqiMode10Type3(data_p, 
                    ueIndex,
                    dlUEContext_p,
/* SPR 22569 Fix + */
#ifdef INTEL_NOCA_L1
/* SPR 22569 Fix - */
                    ri_p,
#else
                    &ulUEContext_p->pucchConfigInfo.ueReportedPeriodicRI,
#endif
                    internalCellIndex);
        }
        else
        {
            /* Scell */
            decodePeriodicCqiMode10Type3(data_p, 
                    ueIndex,
                    dlUEContext_p,
/* SPR 22569 Fix + */
#ifdef INTEL_NOCA_L1
/* SPR 22569 Fix - */
                    ri_p,
#else
                    &ulUEContext_p->ulUeScellContext_p[servCellId]->
                    scellpucchConfigInfo.ueReportedPeriodicRI,
#endif
                    internalCellIndex);
        }
        break;
        default:
        lteWarning("Report Type Decoding not supported %d\n", 
                (UInt32)cqiReportType);
        break;
    }   
}

/*****************************************************************************
 * Function Name  : getPMIValueCW0ForMode1_2Bytes0 
 * Inputs         : messageBuffer_p - buffer pointer,
 *                  numberOfBits - number of bits to be moved,
 * Outputs        : None 
 * Returns        : PMI Value
 * Description    : This function is used to extract PMI value for each subband
 *                  in Aperidoc Mode 1_2 when rank = 1 for Bytes zero.
 ******************************************************************************/
UInt64 getPMIValueCW0ForMode1_2Bytes0(UInt8 *messageBuffer_p, UInt8 numberOfBits)
{
    UInt64 pmiValueCW0 = 0;

    switch(numberOfBits)
    {
        case 2 :
        pmiValueCW0 |= 
            GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) << 4;
        break;

        case 4 :
        pmiValueCW0 |= 
            GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p) << 4;    
        break;

        case 6 : 
        pmiValueCW0 |= 
            GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p) << 4;
        break;
    }

    return pmiValueCW0;
}

/*****************************************************************************
 * Function Name  : getPMIValueCW0ForMode1_2Bytes1
 * Inputs         : messageBuffer_p - buffer pointer,
 *                  numberOfBits - number of bits to be moved,
 * Outputs        : None 
 * Returns        : PMI Value
 * Description    : This function is used to extract PMI value for each subband
 *                  in Aperidoc Mode 1_2 when rank = 1 for Bytes 1.
 ******************************************************************************/
                    UInt64 getPMIValueCW0ForMode1_2Bytes1(UInt8 *messageBuffer_p, UInt8 numberOfBits)
{
    UInt64 pmiValueCW0 = 0;

    /* For number Byte 1 and number of bits 0 */  
    pmiValueCW0 |= *messageBuffer_p << 4;
    messageBuffer_p = messageBuffer_p + 1;

    switch(numberOfBits)
    {
        case 2 :
        pmiValueCW0 |=
            GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) << 12;
        break;

        case 4 :
        pmiValueCW0 |=
            GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p) << 12;
        break;

        case 6 : 
        pmiValueCW0 |=
            GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p) << 12;
        break;
    }

    return pmiValueCW0;
}

/*****************************************************************************
 * Function Name  : getPMIValueCW0ForMode1_2Bytes2
 * Inputs         : messageBuffer_p - buffer pointer,
 *                  numberOfBits - number of bits to be moved,
 * Outputs        : None 
 * Returns        : PMI Value
 * Description    : This function is used to extract PMI value for each subband
 *                  in Aperidoc Mode 1_2 when rank = 1 for Bytes 2.
 ******************************************************************************/
UInt64 getPMIValueCW0ForMode1_2Bytes2(UInt8 *messageBuffer_p, UInt8 numberOfBits)
{
    UInt64 tempPMIValueCW0;
    UInt64 pmiValueCW0 = 0;

    /* For number Byte 2 and number of bits 0 */  
    GET_VALUE_FROM_FIRST_16_BITS(messageBuffer_p, tempPMIValueCW0);
    pmiValueCW0 |= tempPMIValueCW0 << 4;
    switch(numberOfBits)
    {
        case 2 :
        pmiValueCW0 |=
            GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) << 20;
        break;

        case 4 :

        pmiValueCW0 |= 
            GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p) << 20; 
        break;

        case 6 : 
        pmiValueCW0 |= 
            GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p) << 20; 
        break;

    }

    return pmiValueCW0;
}

/*****************************************************************************
 * Function Name  : getPMIValueCW0ForMode1_2Bytes3
 * Inputs         : messageBuffer_p - buffer pointer,
 *                  numberOfBits - number of bits to be moved,
 * Outputs        : None 
 * Returns        : PMI Value
 * Description    : This function is used to extract PMI value for each subband
 *                  in Aperidoc Mode 1_2 when rank = 1 for Bytes 3.
 ******************************************************************************/
UInt64 getPMIValueCW0ForMode1_2Bytes3(UInt8 *messageBuffer_p, UInt8 numberOfBits)
{
    UInt64 tempPMIValueCW0;
    UInt64 pmiValueCW0 = 0;

    /* For number Byte 3 and number of bits 0 */
    GET_VALUE_FROM_FIRST_16_BITS(messageBuffer_p, tempPMIValueCW0);
    tempPMIValueCW0 = *messageBuffer_p;
    messageBuffer_p = messageBuffer_p + 1;
    pmiValueCW0 |= tempPMIValueCW0 << 4;
    switch(numberOfBits)
    {
        case 2 :
        pmiValueCW0 |=
            GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) << 28;
        break;

        case 4 :

        pmiValueCW0 |= 
            GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p) << 28; 
        break;

        case 6 : 
        pmiValueCW0 |= 
            GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p) << 28; 
        break;
    }

    return pmiValueCW0;
}

/*****************************************************************************
 * Function Name  : getPMIValueCW0ForMode1_2Bytes4
 * Inputs         : messageBuffer_p - buffer pointer,
 *                  numberOfBits - number of bits to be moved,
 * Outputs        : None 
 * Returns        : PMI Value
 * Description    : This function is used to extract PMI value for each subband
 *                  in Aperidoc Mode 1_2 when rank = 1 for Bytes 4.
 ******************************************************************************/
UInt64 getPMIValueCW0ForMode1_2Bytes4(UInt8 *messageBuffer_p, UInt8 numberOfBits)
{
    UInt64 tempPMIValueCW0;
    UInt64 pmiValueCW0 = 0;

    /* For number Byte 4 and number of bits 0 */
    GET_VALUE_FROM_FIRST_16_BITS(messageBuffer_p, tempPMIValueCW0);
    GET_VALUE_FROM_FIRST_16_BITS(messageBuffer_p, tempPMIValueCW0);
    pmiValueCW0 |= tempPMIValueCW0 << 4;
    switch(numberOfBits)
    {

        case 2 :
        tempPMIValueCW0 = GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p);
        tempPMIValueCW0 = tempPMIValueCW0 << 36;
        pmiValueCW0 |= tempPMIValueCW0;
        break;

        case 4 :

        tempPMIValueCW0 = GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
        tempPMIValueCW0 = tempPMIValueCW0 << 36;
        pmiValueCW0 |= tempPMIValueCW0;
        break;

        case 6 : 
        tempPMIValueCW0 = GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p);
        tempPMIValueCW0 = tempPMIValueCW0 << 36;
        pmiValueCW0 |= tempPMIValueCW0;
        break;
    }  

    return pmiValueCW0;
}

/*****************************************************************************
 * Function Name  : getPMIValueCW0ForMode1_2Bytes5
 * Inputs         : messageBuffer_p - buffer pointer,
 *                  numberOfBits - number of bits to be moved,
 * Outputs        : None 
 * Returns        : PMI Value
 * Description    : This function is used to extract PMI value for each subband
 *                  in Aperidoc Mode 1_2 when rank = 1 for Bytes 5.
 ******************************************************************************/
UInt64 getPMIValueCW0ForMode1_2Bytes5(UInt8 *messageBuffer_p, UInt8 numberOfBits)
{
    UInt64 tempPMIValueCW0;
    UInt64 pmiValueCW0 = 0;

    /* For number Byte 5 and number of bits 0 */
    GET_VALUE_FROM_FIRST_16_BITS(messageBuffer_p, tempPMIValueCW0);
    GET_VALUE_FROM_FIRST_16_BITS(messageBuffer_p, tempPMIValueCW0);
    tempPMIValueCW0 = *messageBuffer_p;
    messageBuffer_p =messageBuffer_p+1;
    pmiValueCW0 |= tempPMIValueCW0 << 4;
    switch(numberOfBits)
    {
        case 2 :
        tempPMIValueCW0 = GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p);
        tempPMIValueCW0 = tempPMIValueCW0 << 44;
        pmiValueCW0 |= tempPMIValueCW0;
        break;

        case 4 :
        tempPMIValueCW0 = GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
        tempPMIValueCW0 = tempPMIValueCW0 << 44;
        pmiValueCW0 |= tempPMIValueCW0;

        break;

        case 6 : 
        tempPMIValueCW0 = GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p);
        tempPMIValueCW0 = tempPMIValueCW0 << 44;
        pmiValueCW0 |= tempPMIValueCW0;
        break;
    }

    return pmiValueCW0;
}

/*****************************************************************************
 * Function Name  : getPMIValueCW0ForMode1_2Bytes6
 * Inputs         : messageBuffer_p - buffer pointer,
 *                  numberOfBits - number of bits to be moved,
 * Outputs        : None 
 * Returns        : PMI Value
 * Description    : This function is used to extract PMI value for each subband
 *                  in Aperidoc Mode 1_2 when rank = 1 for Bytes 6.
 ******************************************************************************/
UInt64 getPMIValueCW0ForMode1_2Bytes6(UInt8 *messageBuffer_p, UInt8 numberOfBits)
{
    UInt64 tempPMIValueCW0;
    UInt64 pmiValueCW0 = 0;

    /* For number Byte 6 and number of bits 0 */
    GET_VALUE_FROM_FIRST_16_BITS(messageBuffer_p, tempPMIValueCW0);
    GET_VALUE_FROM_FIRST_16_BITS(messageBuffer_p, tempPMIValueCW0);
    GET_VALUE_FROM_FIRST_16_BITS(messageBuffer_p, tempPMIValueCW0);
    pmiValueCW0 |= tempPMIValueCW0 << 4;
    switch(numberOfBits)
    {

        case 2 :
        tempPMIValueCW0 = GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p);
        tempPMIValueCW0 = tempPMIValueCW0 << 52;
        pmiValueCW0 |= tempPMIValueCW0;
        break;

        case 4 :
        tempPMIValueCW0 = GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
        tempPMIValueCW0 = tempPMIValueCW0 << 52;
        pmiValueCW0 |= tempPMIValueCW0;

        break;

        case 6 : 
        tempPMIValueCW0 = GET_VALUE_FROM_FIRST_6_BITS(*messageBuffer_p);
        tempPMIValueCW0 = tempPMIValueCW0 << 52;
        pmiValueCW0 |= tempPMIValueCW0;
        break;
    }  

    return pmiValueCW0;
}

/*****************************************************************************
 * Function Name  : getWBPMIForAperiodicMode22RI1NumTxAntenna1Or2
 * Inputs         : messageBuffer_p - buffer pointer and wbPmiValue,
 *                  wbPmiValue,
 *                  cellParams_p - pointer to cell parameters
 * Outputs        : None 
 * Returns        : CombinatorialIndex value. 
 * Description    : This function is used to extract PMI value in 
 *                  AperiodicMode22   for RI = 1 for number of tx antenna 1 or 2 
 ******************************************************************************/
void getWBPMIForAperiodicMode22RI1NumTxAntenna1Or2(UInt8 *messageBuffer_p,
        UInt8 *wbPmiValue,
        CellConfigParams *cellParams_p)
{
    UInt8 tempWBPmi = 0;

    switch (cellParams_p->LBitLabelForAperiodic)
    {    
        case 2:
        {
            tempWBPmi = GET_WIDEBAND_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p);
        }
        break;
        case 3: 
        case 11:
        {
            tempWBPmi = ((*messageBuffer_p) & 0x06 ) >> 1;
        }
        break;
        case 4:
        case 12:   
        {
            tempWBPmi = ((*messageBuffer_p) & 0x0C) >> 2;
        }
        break;
        case 5:
        case 13:
        {
            tempWBPmi = ((*messageBuffer_p) & 0x18 ) >> 3;
        }
        break;
        case 6:
        case 14:     
        {    
            tempWBPmi = ((*messageBuffer_p) & 0x30) >> 4;
        }
        break;
        case 7:
        case 15:
        {
            tempWBPmi = ((*messageBuffer_p) & 0x60 ) >> 5;
        }
        break;

        case 8:
        case 16:   
        {
            tempWBPmi = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
        }
        break;
        case 9:
        case 17:
        {
            tempWBPmi = GET_VALUE_FROM_LAST_BIT(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempWBPmi |= GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p) << 1;
        }
        break;

        case 10:
        case 18:
        {   
            tempWBPmi = GET_WIDEBAND_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p);
        }
        break;
        default:
        break;
    }    

    *wbPmiValue = GET_2_BIT_SWAPPED_VALUE(tempWBPmi);

    return;
}

/*****************************************************************************
 * Function Name  : getWBPMIForAperiodicMode22RI1NumTxAntenna4
 * Inputs         : messageBuffer_p - buffer pointer and wbPmiValue,
 *                  wbPmiValue,
 *                  cellParams_p - pointer to cell parameters
 * Outputs        : None 
 * Returns        : CombinatorialIndex value. 
 * Description    : This function is used to extract PMI value in 
 *                  AperiodicMode22   for RI = 1 for number of antenna 4        
 ******************************************************************************/
void getWBPMIForAperiodicMode22RI1NumTxAntenna4(UInt8 *messageBuffer_p,
        UInt8 *wbPmiValue,
        CellConfigParams *cellParams_p)
{
    UInt8 tempWBPmi = 0;

    switch (cellParams_p->LBitLabelForAperiodic)
    {    
        case 2:
        {
            tempWBPmi = GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
        }
        break;
        case 3: 
        case 11:
        {
            tempWBPmi = ((*messageBuffer_p) & 0x1E ) >> 1;
        }
        break;
        case 4:
        case 12:   
        {
            tempWBPmi = ((*messageBuffer_p) & 0x3C) >> 2;
        }
        break;
        case 5:
        case 13:
        {
            tempWBPmi =  ((*messageBuffer_p) & 0x78 ) >> 3;
        }
        break;
        case 6:
        case 14:     
        {    
            tempWBPmi = ((*messageBuffer_p) & 0xF0) >> 4;
        }
        break;
        case 7:
        case 15:
        {
            tempWBPmi = GET_VALUE_FROM_LAST_3_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempWBPmi |= GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p) << 3;
        }
        break;

        case 8:
        case 16:   
        {
            tempWBPmi = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempWBPmi |= GET_WIDEBAND_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) << 2;
        }
        break;
        case 9:
        case 17:
        {
            tempWBPmi = GET_VALUE_FROM_LAST_BIT(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempWBPmi |= GET_VALUE_FROM_FIRST_3_BITS(*messageBuffer_p) <<1;
        }
        break;

        case 10:
        case 18:
        {
            tempWBPmi = GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);  
        }
        break;
        default:
        break;
    }    

    *wbPmiValue = GET_4_BIT_SWAPPED_VALUE(tempWBPmi);

    return;
}

/*****************************************************************************
 * Function Name  : getPMIForAperiodicMode22RI1NumTxAntenna1Or2
 * Inputs         : messageBuffer_p - buffer pointer
 *                  wbPmiValue ,sbPmiValue,
 *                  cellParams_p - Cell configuration parameter
 * Outputs        : None 
 * Returns        : CombinatorialIndex value. 
 * Description    : This function is used to extract PMI value in 
 *                  AperiodicMode22   for RI = 1 for number of antenna 1 or 2
 ******************************************************************************/
void getPMIForAperiodicMode22RI1NumTxAntenna1Or2(UInt8  *messageBuffer_p,
        UInt8 *wbPmiValue,
        UInt8 *sbPmiValue,
        CellConfigParams *cellParams_p)
{
    UInt8 tempWBPmi = 0;
    UInt8 tempSBPmi = 0;

    switch (cellParams_p->LBitLabelForAperiodic)
    {    
        case 2:
        {
            tempWBPmi = GET_WIDEBAND_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p);
            tempSBPmi = ((*messageBuffer_p) & 0x0C ) >> 2;
        }
        break;
        case 3: 
        case 11:
        {
            tempWBPmi = ((*messageBuffer_p) & 0x06 ) >> 1;
            tempSBPmi = ((*messageBuffer_p) & 0x18 ) >> 3;
        }
        break;
        case 4:
        case 12:
        {
            tempWBPmi = ((*messageBuffer_p) & 0x0C) >> 2;
            tempSBPmi = ((*messageBuffer_p) & 0x30) >> 4;
        }
        break;
        case 5:
        case 13:    
        {
            tempWBPmi = ((*messageBuffer_p) & 0x18 ) >> 3;
            tempSBPmi = ((*messageBuffer_p) & 0x60 ) >> 5;
        }
        break;
        case 6:
        case 14:
        {
            tempWBPmi = ((*messageBuffer_p) & 0x30) >> 4;
            tempSBPmi = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
        }
        break;
        case 7:
        case 15:
        {
            tempWBPmi = ((*messageBuffer_p) & 0x60 ) >> 5;
            tempSBPmi = ((*messageBuffer_p) & 0x80 ) >> 7;
            messageBuffer_p = messageBuffer_p + 1;
            tempSBPmi |= GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p) << 1;
        }
        break;
        case 8:
        case 16:
        {
            tempWBPmi = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempSBPmi = GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p);
        }
        break;
        case 9:
        case 17:
        {
            tempWBPmi = GET_VALUE_FROM_LAST_BIT(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempWBPmi |= GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p) << 1;
            tempSBPmi = ((*messageBuffer_p) & 0x06) >> 1;
        }
        break;
        case 10:
        case 18:
        {
            tempWBPmi = GET_WIDEBAND_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p);
            tempSBPmi = ((*messageBuffer_p) & 0x0C ) >> 2;
        }
        break;
        default:

        break;
    }

    *wbPmiValue = GET_2_BIT_SWAPPED_VALUE(tempWBPmi);
    *sbPmiValue = GET_2_BIT_SWAPPED_VALUE(tempSBPmi);

    return;
}

/*****************************************************************************
 * Function Name  : getPMIForAperiodicMode22RI1NumTxAntenna4
 * Inputs         : messageBuffer_p - buffer pointer
 *                  wbPmiValue ,sbPmiValue,
 *                  cellParams_p - Cell configuration parameter
 * Outputs        : None 
 * Returns        : CombinatorialIndex value. 
 * Description    : This function is used to extract PMI value in 
 *                  AperiodicMode22   for RI = 1 for number of antenna 4
 ******************************************************************************/
void getPMIForAperiodicMode22RI1NumTxAntenna4(UInt8  *messageBuffer_p,
        UInt8 *wbPmiValue,
        UInt8 *sbPmiValue,
        CellConfigParams *cellParams_p)
{
    UInt8 tempWBPmi = 0;
    UInt8 tempSBPmi = 0;

    switch (cellParams_p->LBitLabelForAperiodic)
    {    
        case 2:
        {
            tempWBPmi = GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
            tempSBPmi = GET_WIDEBAND_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
        }
        break;
        case 3: 
        case 11:
        {
            tempWBPmi = ((*messageBuffer_p) & 0x1E ) >> 1;
            tempSBPmi = GET_VALUE_FROM_LAST_3_BITS(*messageBuffer_p);
            messageBuffer_p =messageBuffer_p + 1;
            tempSBPmi |= GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p) << 3;
        }
        break;
        case 4:
        case 12:
        {
            tempWBPmi = ((*messageBuffer_p) & 0x3C) >> 2;
            tempSBPmi = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
            messageBuffer_p =messageBuffer_p + 1;
            tempSBPmi |= GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) << 2;
        }
        break;
        case 5:
        case 13:    
        {
            tempWBPmi = ((*messageBuffer_p) & 0x78 ) >> 3;
            tempSBPmi = GET_VALUE_FROM_LAST_BIT(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p +1;
            tempSBPmi = GET_VALUE_FROM_FIRST_3_BITS(*messageBuffer_p) << 1;
        }
        break;
        case 6:
        case 14:
        {
            tempWBPmi = GET_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
            messageBuffer_p =messageBuffer_p +1;
            tempSBPmi = GET_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
        }
        break;
        case 7:
        case 15:
        {
            tempWBPmi = GET_VALUE_FROM_LAST_3_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p +1;
            tempWBPmi |= GET_VALUE_FROM_FIRST_BIT(*messageBuffer_p) << 3;
            tempSBPmi = ((*messageBuffer_p) & 0x1E ) >> 1;
        }
        break;
        case 8:
        case 16:
        {
            tempWBPmi = GET_VALUE_FROM_LAST_2_BITS(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempWBPmi |= GET_VALUE_FROM_FIRST_2_BITS(*messageBuffer_p) << 2;

            tempSBPmi = ((*messageBuffer_p) & 0x3C ) >> 2;
        }
        break;
        case 9:
        case 17:
        {
            tempWBPmi = GET_VALUE_FROM_LAST_BIT(*messageBuffer_p);
            messageBuffer_p = messageBuffer_p + 1;
            tempWBPmi |= GET_VALUE_FROM_FIRST_3_BITS(*messageBuffer_p) << 1;
            tempSBPmi =((*messageBuffer_p) & 0x78) >> 3;
        }
        break;
        case 10:
        case 18:
        {
            tempWBPmi = GET_WIDEBAND_VALUE_FROM_FIRST_4_BITS(*messageBuffer_p);
            tempSBPmi = GET_WIDEBAND_VALUE_FROM_LAST_4_BITS(*messageBuffer_p);
        }
        break;
        default:

        break;
    }

    *wbPmiValue = GET_4_BIT_SWAPPED_VALUE(tempWBPmi);
    *sbPmiValue = GET_4_BIT_SWAPPED_VALUE(tempSBPmi);

    return;
}
#ifdef FAPI_4_0_COMPLIANCE
/*****************************************************************************
 * Function Name  : handleRIAndCheckForAperiodicTrigger
 * Inputs         : ueIndex : Ue Id
 *                  recvTTI : TTI at which msg is received.
 *                  cqiPdu_p : pointer to CQI message 
 *                  cqiReportType : Expect cqi report type
 *                  subFrameNum : subframe number
 *                  internalCellIndex :internal Cell Id  
 * Outputs        : None 
 * Returns        : Whether this PDU is to be processed further. 
 * Description    : This function is used handle RI and aperiodic trigger, if 
 *                  data offset is 0 or cqi threshold is less than threshold.
 *****************************************************************************/
            STATIC  UInt8 handleRIAndCheckForAperiodicTrigger(UInt16 ueIndex,
                    /* SPR 15909 fix start */
                    tickType_t recvTTI,
                    /* SPR 15909 fix end */
                    FAPI_cqiPduIndication_st *cqiPdu_p,
                    ueExpectedReportType  cqiReportType,
                    /* +- SPR 17777 */
                    /* +- SPR 17777 */
                    InternalCellIndex internalCellIndex)
{
    UInt8 bitMaskForAperiodicReport = 0;
    ULUEContext *ulUEContext_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
    UInt8  istobeProcessedNext = LTE_TRUE;   

    dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;  
    ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;

    if  (0 == cqiPdu_p->dataOffset) 
    {
        /* SPR 4484 changes start */
        /* If only aperiodic is  ON and data offset is 0 then 
         *  even if below condition is not met then also we will 
         *  skip from here.
         */  
        if ((APERIODIC_CQI_REPORTING_MODE ==
                    dlUEContext_p->dlCQIInfo.cqiTypeIndicator) && 
                (recvTTI == ulUEContext_p->lastTriggeredAperiodicTTI ))
            /* SPR 4484 changes start */
        {
            bitMaskForAperiodicReport |= 1;
            putEntryInULAperiodicCQIQueue(ueIndex, internalCellIndex
                    ,bitMaskForAperiodicReport
                    );
            /* SPR 4484 changes end */
        }
        else if  (cqiPdu_p->cqiRiInformation.ri) 
        {
            handleRiReportWithDataOffsetNull(cqiReportType,
                    ulUEContext_p,
                    dlUEContext_p,
                    ueIndex,
                    &(cqiPdu_p->cqiRiInformation.ri),
                    internalCellIndex,
                    0);
            /* +- SPR 17777 */
        }    
        istobeProcessedNext = LTE_FALSE;  
    }
    else if (cqiPdu_p->cqiRiInformation.ulCqi < cellSpecificParams_g.\
            cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->pucchCQISINRThreshold)
    { 

        /* SPR 4484 changes start */
        if ((APERIODIC_CQI_REPORTING_MODE ==
                    dlUEContext_p->dlCQIInfo.cqiTypeIndicator) &&
                (recvTTI == ulUEContext_p->lastTriggeredAperiodicTTI))
        {
            bitMaskForAperiodicReport |= 1;
            putEntryInULAperiodicCQIQueue(ueIndex, internalCellIndex
                    ,bitMaskForAperiodicReport
                    );
        }

        /* SPR 4484 changes end */
        istobeProcessedNext = LTE_FALSE;
    }   

    return istobeProcessedNext;
}
#endif




