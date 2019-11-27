/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: pdcpInternalEvents.h 2017/03/15 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : PDCP Internal events (exchanged between Master, 
 *                     Workers and Accumulator threads) definitions.
 *
 ***************************************************************************/

#ifndef _PDCP_INTERNAL_EVENTS_H_
#define _PDCP_INTERNAL_EVENTS_H_

#if defined(CRAN_RLC_PDCP_SPLIT) && !defined(UE_SIM_TESTING)

/***************************************************************************
 * Project specific Includes
 **************************************************************************/
#include "lteTypes.h"
#include "lteMisc.h"

//#include <sys/types.h>
//#include <sys/socket.h>
#define RX_EVENT_Q_SIZE         500
#define MAX_WORKER_THREADS      10
#define MAX_MASTER_THREADS      2
#define MAX_RECV_BUFF_SIZE      32000
#define FALSE                   0
#define TRUE                    1

/* Enum for events exchanged between Master/Worker/Accumulator threads */
typedef enum 
{
    /* Event from Worker to Accumulator for updating KPI stats
     * of a particular worker thread */
    UPDATE_KPI_STATS_IND,

    /* Event from Worker to Accumulator for updating PDCP Stats
     * of a particular worker thread */
    UPDATE_PDCP_STATS_IND,

    /* Event from Master thread to Accumulator thread for sending
     * events received from External modules. */
    EXTERNAL_EVENT_IND,

}PdcpInternalEventType;


/* Information passed on between Master/Workers/Accumulator threads */
typedef struct PdcpInternalEventT 
{
    PdcpInternalEventType eventType;
    UInt8*                msgBuf;
    UInt16                msgLen;
}PdcpInternalEvent, *LP_PdcpInternalEvent;


/* Information passed from a Worker thread to Accumulator thread
 * for updating worker thread specific KPI stats maintained by 
 * accumulator thread. */
typedef struct ThreadSpecificKpiStatsT
{
    /* KPI Data collected by the worker thread */
    UInt8*  kpiInfo;

} ThreadSpecificKpiStats;


/* Information passed from a Worker thread to Accumulator thread
 * for updating worker thread specific PDCP stats maintained by 
 * accumulator thread. */
typedef struct ThreadSpecificStatsT
{
    /* Stats collected by the worker thread */
    UInt8*  statsInfo;

} ThreadSpecificStats;


/* Information passed on by master thread to worker/Accumulator
 * thread on receiving a new message from external entity. */
typedef struct ExtenalEventIndT
{
    /* Message buffer */
    UInt8*  msgBuf;
}ExternalEventInd, *LP_ExternalEventInd;


#endif // CRAN_RLC_PDCP_SPLIT

#endif  // _PDCP_INTERNAL_EVENTS_H_
