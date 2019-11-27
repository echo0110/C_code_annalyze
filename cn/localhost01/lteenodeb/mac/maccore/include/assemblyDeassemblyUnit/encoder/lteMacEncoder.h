/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacEncoder.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the declarations necessary for
 *                     Encoder module.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacEncoder.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.24  2009/07/31 09:07:55  gur18569
 * replaced lteMacRlcPdcpInterface.h with lteLayer2MacRlcInterface.h
 *
 * Revision 1.23  2009/07/28 14:31:51  gur11912
 * Updated for HARQ Timer start in case of retransmission
 *
 * Revision 1.22  2009/07/27 07:49:29  gur12140
 * Incorporated Comments
 *
 * Revision 1.21  2009/07/15 07:00:25  gur18569
 * also included lteMacRlcPdcpInterface.h
 *
 * Revision 1.20  2009/07/15 06:26:39  gur18569
 * renamed lteMacRlcPdcpInterface.h to lteMacRlcInterface.h
 *
 * Revision 1.19  2009/07/14 08:04:24  gur18569
 * added encThreadCount field to tbPopulation struct
 *
 * Revision 1.18  2009/07/07 15:15:18  gur18569
 * added putEntryInSchEncoderQueue declaration
 *
 * Revision 1.17  2009/07/02 10:29:10  gur18569
 * added an arg to putEntryInEncoderQueue
 *
 * Revision 1.16  2009/07/02 05:29:09  gur18569
 * added clean up func declaration
 *
 * Revision 1.15  2009/06/16 12:22:39  gur18569
 * changes in putEntryInEncoderNode signature
 *
 * Revision 1.14  2009/06/08 15:42:22  gur12140
 * Removed Linking Errors
 *
 * Revision 1.13  2009/06/01 05:52:17  gur18569
 * added functionality of writing into tx queues
 *
 * Revision 1.12  2009/05/29 05:56:45  gur18569
 * added func decl of processEncoderQueue
 *
 * Revision 1.11  2009/05/28 11:57:36  gur18569
 * removed compilation errors
 *
 * Revision 1.10  2009/05/19 16:05:48  gur18569
 * incorporated review comments
 *
 * Revision 1.9  2009/05/15 07:10:13  gur18569
 * added scheduledTick to EncoderNodeInfo
 *
 * Revision 1.8  2009/05/14 13:25:35  gur18569
 * added comments
 *
 * Revision 1.7  2009/05/10 13:13:33  gur18569
 * fixed errors
 *
 * Revision 1.6  2009/05/10 09:48:37  gur12140
 * Removed Compilation Error
 *
 * Revision 1.5  2009/05/10 09:23:03  gur12140
 * Removed Compilation Error
 *
 * Revision 1.4  2009/05/10 09:18:26  gur18569
 * added encoderContainerIndex in EncoderNodeInfo struct
 *
 * Revision 1.3  2009/05/10 08:59:39  gur18569
 * removed endindication arg of putEntryInEncoderQueue
 *
 * Revision 1.2  2009/05/10 08:41:59  gur18569
 * added structures and macros
 *
 * Revision 1.1  2009/05/05 04:43:49  gur18569
 * Initial version (kenobi@gur18569)
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_ENCODER_H
#define LTE_MAC_ENCODER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacDLUEContext.h"
#include "lteMacRlcInterface.h"
#include "lteMacExecutionLegs.h"
#include "lteLayer2MacRlcInterface.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/*This macro does the encoding of one byte MAC subheader*/
#define  MAC_SUBHEADER_ONE_BYTE(dataTB_p,ext,lcId)\
        *dataTB_p++ = ext << 5 | lcId;

/*This macro does the encoding of two byte MAC subheader*/
#define MAC_SUBHEADER_TWO_BYTE(dataTB_p,ext,lenLCData,lcId)\
        *dataTB_p++ = ext << 5 | lcId;\
        *dataTB_p++ = lenLCData & 0x7f;
        
/*This macro does the encoding of three byte MAC subheader*/
#define MAC_SUBHEADER_THREE_BYTE(dataTB_p,ext,lenLCData,lcId)\
        *dataTB_p++ = ext << 5 | lcId;\
        *dataTB_p = 0x80;\
        *dataTB_p++ |= ((lenLCData >> 8) & 0x7f);\
        *dataTB_p++ = lenLCData & 0x00ff;

#define MAC_SUBHEADER_LENGTH_ONE_BYTE   1
#define MAC_SUBHEADER_LENGTH_TWO_BYTE   2
#define MAC_SUBHEADER_LENGTH_THREE_BYTE 3
#define MAX_TWO_BYTE_SUBHEADER_LENGTH 127


/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef struct SDUHeaderInfoT
{
    RLCDataBuffer*     lcInfo_p;    /* Array storing the LCInfo pointers */
    UInt32    numOfLCMultiplexed;   /* Number of LCs to be multiplexed 
                                     * for creating a Transport Block. */
}SDUHeaderInfo;

typedef struct EncoderQueueNodeT
{
    /*data type of anchor*/
    LTE_SNODE encoderNodeAnchor;

    /* scheduledTick is used to validate whether the encoding request
     * has arrived at right tick or not , whether the packet will
     * reach phy in the right tick or not.*/
    /* SPR 15909 fix start */
    tickType_t scheduledTick;
    /* SPR 15909 fix end */

    /*Pointer to RLC buffer received from RLC*/
    void * transmissionAndRLCData_p;

    /*ueIndex of UE for which encoding request is sent*/
    UInt16 ueIndex;

    /*harqProcess id of the harq associated with transmission of TB encoded
     * from this request*/
    UInt8 harqProcessId;

    /*Index of dciCCEContainerInfo_g[dciCCEInfoContainerIndex].
     *  dciCCEInfo[cceIndex] which is necessary to update pduInfo*/
    UInt8 cceIndex;

    /* If this value is set then it is an indication to 
     * processEncoderQueue to stop processing*/
    UInt8 dummyNode;
}EncoderQueueNode;

typedef struct EncoderThreadInfoT
{
    /*identifier to the thread*/
    ULong32 tId;

    /*index of the EncoderThreadPoolArr, which identifies this encoder thread*/
    UInt8  encoderThreadIndex;
}EncoderThreadInfo;

typedef struct EncoderNodeT
{
    /* This index is incremented by transmitter whn reading from cceIndexArr*/
    /* Both the indexes are to be reset by transmitter if the content of 
     * cceIndexArr read at readIndex is INVALID_CCE_INDEX*/
    UInt8      readIndex;

    /* This index is incremented by encoder whn filling the cceIndexArr*/
    UInt8      writeIndex;

    /* An array of cce index filled by encoder , this is read by 
     * transmitter while transmitting data msgs*/
    UInt8       cceIndexArr[MAX_CCE_INDEX + MAX_SPS_CCE];
#if (!defined(DL_UL_SPLIT) || defined(DL_DATA_SEPARATION))
    /* This queue is used for encoding request from RLC */
    LTE_SQUEUE encoderQueue;
#elif CIRC_QUEUE_IMPL
    ENCODER_CIRC_TYPE encoderCircQueue;
#endif    

    /* This queue is used for encoding request from scheduler directly */
    LTE_SQUEUE encoderQueueForScheduler;

#ifdef DL_UL_SPLIT
    SInt32 encoderQueueIdDL;
    SInt32 encoderQueueIdUL;
#endif

}EncoderNode;

typedef struct EncoderContainerNodeT
{
    EncoderNode encoderNode;
}EncoderContainerNode;

/* This structure is used by scheduler or any module which wants to
 * give encoding requests but with out any multiplexing.*/
typedef struct EncoderNodeInfoT{
    /* scheduledTick is used to validate whether the encoding request
     * has arrived at right tick or not , whether the packet will
     * reach phy in the right tick or not.*/
    /* SPR 15909 fix start */
    tickType_t scheduledTick;
    /* SPR 15909 fix end */
    /*harqProcess id of the harq associated with transmission of TB encoded
     * from this request*/
    UInt8 harqProcessId;

    /*Index of dciCCEContainerInfo_g[dciCCEInfoContainerIndex].
     *  dciCCEInfo[cceIndex] which is necessary to update pduInfo*/
    UInt8 cceIndex;
}EncoderNodeInfo;

/* This structure is used internally by encoder*/
typedef struct TBPopulationStructT
{
    /*Pointer to data populated by RLC */
    RLCDataBuffer * rlcDataBuffer_p;

    /*Pointer to HARQTBStructure of either TB1 or TB2 */
    DLHARQTBInfo * harqTB_p;

    /*pointer to paddingForTB field in TransmissionAndRLCData , it is 
     * necessary to update this field after RLC data validation because
     * of it's use in case of RETX*/
    UInt32 padding;

    /*Number of fields present in RLC data buffer*/
    UInt8 lcCount;
    
    /* Timing advance value to be encoded*/
    UInt8 taValue;

    /* Value of drxFlag in ue context is set into this field*/
    UInt8 drxValue;

    /*This index in necessary because in case of spatial multiplexing 
     * for encoding TB two we need to start iterating RLC buffer from
     * lcCount for TB one.*/
    UInt8 rlcBufferStartIndex;

    /*Index into totalDlBytesSend_g array */
    UInt8 encThreadCount;
    UInt8 macScellCEFlag;
}TBPopulationStruct;
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 /* + SPR 11209 Changes */
extern EncoderContainerNode *encoderContainerArr_gp[MAX_NUM_CELL][MAX_NUM_CELL];
 /* - SPR 11209 Changes */
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

MacRetType createTBForSCELLTADRXMsg(UInt32 timingAdvanceValue ,
                                       UInt32 drxValue ,
                                       DLHARQTBInfo * harqTB_p,
                                       UInt8 *dataTB_p,
                                       UInt8 macScellCEFlag);

MacRetType encodeTB( DLHARQProcess * hProcess_p,

                          TransmissionAndRLCData * transmissionAndRLCData_p ,
                          DLUEContext* ueContext_p,
                          UInt8 * dataTB_p,
                          /* +- SPR 17777 */
                          TBPopulationStruct * tbPopulationStruct_p,
                          /* SPR 430 changes start */
                          UInt32* newQueueLoadEncoded,
                          /* +- SPR 17777 */
                          InternalCellIndex internalCellIndex);

void  putEntryInSchEncoderQueue(UInt32 ueIndex,
                EncoderNodeInfo* encoderValFromSch_p,
                UInt32 encoderContainerIndex,
                InternalCellIndex internalCellIndex);
void  putEntryInEncoderQueue(UInt32 ueIndex,UInt32 lcCount,
                /* + SPR 5584 COVERITY CID 25564 FIX */
                TransmissionAndRLCData** transmissionAndRLCData_p,
                /* - SPR 5584 COVERITY CID 25564 FIX */
                UInt32 encoderContainerIndex
#ifdef DL_DATA_SEPARATION
                /* SPR 15909 fix start */
                , tickType_t currentTick
                /* SPR 15909 fix start */
#endif                
                ,InternalCellIndex internalCellIndex
                );
 
                        /* SPR 15909 fix start */
void processEncoderQueue(tickType_t currentTick,
                        /* SPR 15909 fix end */
                         UInt32 delayToPhy,
                         /* +- SPR 17777 */
                         UInt32 subFrameNumber,
                         UInt32 isTxFunctionalityAdded,
                         /* +- SPR 17777 */
                         InternalCellIndex internalCellIndex,
 /* + SPR 11209 Changes */
                         InternalCellIndex encoderQIndex);
 /* - SPR 11209 Changes */

#ifdef DL_DATA_SEPARATION
                        /* SPR 15909 fix start */
UInt16 processRlcEncoderQueuePerUE(tickType_t currentTick,
                        /* SPR 15909 fix end */
                         UInt32 delayToPhy,
                         /* +- SPR 18490 */
                         UInt32 subFrameNumber,
                         UInt32 isTxFunctionalityAdded,
                         UInt8 *lastUeFlag_p,
                         InternalCellIndex internalCellIndex,
                         InternalCellIndex rlcCellIndex);

                        /* SPR 15909 fix start */
void processMACEncoderQueue(tickType_t currentTick,
                        /* SPR 15909 fix end */
                         UInt32 delayToPhy,
                         /* +- SPR 18490 */
                         UInt32 subFrameNumber,
                         UInt32 isTxFunctionalityAdded,
                         /* +- SPR 18490 */
                         InternalCellIndex internalCellIndex,
                         InternalCellIndex rlcCellIndex);
#endif

void initEncoderModule(UInt8  numOfCells);
void cleanupEncoderModule(InternalCellIndex internalCellIndex);
#endif  /* LTE_MAC_ENCODER_H */
