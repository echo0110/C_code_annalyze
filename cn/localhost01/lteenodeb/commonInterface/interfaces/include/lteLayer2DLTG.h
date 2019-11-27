/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2011 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2DLTG.h$
 *
 ****************************************************************************
 *
 *  File Description : This file contains all declaration needed for LAYER2_DLTG_THREAD
 *
 ****************************************************************************
 *Revision Details
 * ----------------
 * Revision 1.1  2011/9/20 17:37:08  gur26001
 *
 ****************************************************************************/
#ifndef INCLUDED_LTELAYER2_DL_TG_H
#define INCLUDED_LTELAYER2_DL_TG_H

/****************************************************************************
 * standard Includes
 ****************************************************************************/
#include <pthread.h>
#include <sched.h>
/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteThread.h"
#include "lteSQueue.h"
/* LAYER2_DLTG_THREAD Compilation fix Start */
#include "lteTypes.h"
/* LAYER2_DLTG_THREAD Compilation fix End */

/****************************************************************************
 *  MACROS
 *****************************************************************************/
/* SPR 3915 Fix Start */
#define START_OFFSET_DL_GTP 64
/* SPR 3915 Fix End */
/*255 UE support */
#define MAX_PKTS_IN_QUE 15000

#define MAX_PKT_SIZE    1200
#define GTP_HDR 8
#define VALUE_INVALID 0xFFFFFFFF

extern UInt32 seqNum_g;
extern LTE_SQUEUE dlTgDataPktQ_g;

typedef struct dlTgDataPktT {
    LTE_SNODE sNode;
    UInt32  dataLength;     /*length of data*/
    UInt8* dlTgDataPktBuf_p; /*pointer to data*/
} dlTgDataPkt;

#define QINIT_DLTG_DATA_PKT_Q()\
    sQueueInit(&dlTgDataPktQ_g)

#define QCOUNT_DLTG_DATA_PKT_Q()\
    sQueueCount(&dlTgDataPktQ_g)

#define QGETFIRST_DLTG_DATA_PKT_Q()\
    getFirstNode(&dlTgDataPktQ_g)

#define QGETNEXT_DLTG_DATA_PKT_Q()\
    getNextNode(&dlTgDataPktQ_g)

#define DEQUEUE_DLTG_DATA_PKT_Q()\
    (dlTgDataPkt*)popNode(&dlTgDataPktQ_g)\

#define ENQUEUE_DLTG_DATA_PKT_Q(dlTgDataPkt_p)\
    pushNode(&dlTgDataPktQ_g,&(dlTgDataPkt_p)->sNode)

extern UInt8 * pDlDataQueue;
extern UDouble32  totalThrputVar_g;
extern UDouble32  totalNoOfPktVar_g;
extern UInt32 packetSize_g;
extern UInt32 noOfPktVar_g;
extern UInt32 timeInMs_g;
extern UInt32 timeInMs_g_rx;
/*SPR 15909 fix start*/
extern tickType_t noOfTicks_g; 
/*SPR 15909 fix end*/
extern UInt32 fixedPktPerTick_g;
extern UInt32 remainingPkt_g;
extern UInt32 extraBurstCounter_g;
extern UInt32 trafficProfileVar_g;
extern UInt32 pktCounter_g;
extern UInt32 dlTgCoreNum_g;
extern UInt64 txTickCount_g;
extern UInt16 ueTurn_g;
extern UInt16 ueStart_g;
extern SInt16 lastUe_g;
extern SInt16 ueId_g;
extern SInt16 ueId_rx;
extern UInt32 putDisplayVar_g;

/************************************* FUNCTION DECLARATION **********************/

/* SPR 15909 fix start */
/* + SPR 17439 */
extern tickType_t getCurrentTick(void);
/* - SPR 17439 */
/* SPR 15909 fix end */

UInt8 lteMacCheckDLTGInput(/*SPR 17777 fix +-*/
                           UInt32 ueId,
                           UInt32 ueStart,
                           int argcount);

void normalModeForMacExecSchedular( void );

#endif
