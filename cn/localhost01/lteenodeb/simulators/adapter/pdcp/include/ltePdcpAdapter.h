/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpAdapter.h,v 1.2.16.2 2010/10/26 16:41:56 gur20056 Exp $
 *
 ****************************************************************************
 *
 *  File Description : Definitions for PDCP Adapter
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpAdapter.h,v $
 * Revision 1.2.16.2  2010/10/26 16:41:56  gur20056
 * anil: pdcp crash fix
 *
 * Revision 1.2.16.1  2010/10/25 08:58:42  gur21010
 * Warnings Removed
 *
 * Revision 1.2  2010/03/08 07:52:32  gur19479
 * updated for Ip Address
 *
 * Revision 1.1.1.1  2010/02/11 04:51:25  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.4.2.1  2009/11/18 14:20:31  gur19479
 * updated for ppp sockets
 *
 * Revision 1.1.2.4  2009/07/28 15:36:42  gur19836
 * Downlink Issues Resolved, Latency Measurement code added
 *
 * Revision 1.1.2.3  2009/07/20 10:29:30  gur11318
 * Updated to reove warnings for T.G. UChar8 array.
 *
 * Revision 1.1.2.2  2009/07/07 07:56:31  gur19479
 * Compilation Issues resolved
 *
 * Revision 1.1.2.1  2009/06/30 12:44:25  gur19836
 * Initial Version
 *
 ****************************************************************************/


#ifndef PDCP_ADAPTER_H
#define PDCP_ADAPTER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "port.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "lteMemPool.h"
#include "lteMsgPool.h"
#include "lteTypes.h"
#include "lteThread.h"
#include "lteSync.h"
#include "lteMisc.h"
#include "lteRelayAdapter.h"
#include "ltePdcpContext.h"
#include "lteLog.h"

/*SPR 2030 fix Start*/
#ifndef UE_SIM_TESTING
#include "logging.h"
#include "alarm.h"
#endif
/*SPR 2030 fix End*/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define NORMAL_MODE 1
#define LOOPBACK_MODE 0
//#define FAILURE -1
#define BLOCKING 1
#define NON_BLOCKING 0
#define BUFFERSIZE 2000
#define ALLOWED 1
#define API_HEADER_LEN 5
/* 128 UE changes start */
/* +- SPR 18268 */
/* SPR 20526 Fix Start */
#define MAX_UE MAX_UE_SUPPORTED
/* SPR 20526 Fix End */
/* +- SPR 18268 */
/* 128 UE changes end */
#define MIN_DRB_LC 3
#define MAX_DRB_LC 10
#define MAX_LC 10
#define MIN_UE 0
#define MIN_LC 1

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/*SPR 2030 fix Start*/
/* SPR 15909 fix start */
extern tickType_t relayGtpuCurrentTick_g;
/* SPR 15909 fix end */
/*SPR 2030 fix End*/
/* SPR #2017 fix Start */
#define GTP_MIN_HDR_LEN 8
 /* SPR #2017 fix End */

#define PDCP_ADAPTER_TID_INFO 6
#define PDCP_ADAPTER_UE_INFO_FOR_REL 4
#ifdef UE_SIM_TESTING
#define MU_ADAPTER_PORT 21212
#else
#define MU_ADAPTER_PORT 21211
#endif
#define MU_ADAPTER_IP   "127.0.0.1"

void initGtpuGlueSocket();

/* + COVERITY 5.0 */
Char8 trafficGeneratorIP[20];
Char8 adapterIP[20];
/* - COVERITY 5.0 */
UInt16 trafficGeneratorPort;
UInt16 pdcpAdapterRxPort;
#pragma pack(push,1)
typedef struct header
{
    UInt16      ueIndex;
    UInt8       lcId;
    UInt16      dataSize;
    UInt8       data[2500];
} Header, *LP_Header;
#pragma pack(pop)

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
//extern int   pselect(int   n,   fd_set  *readfds,  fd_set  *writefds,  fd_set
  //      *exceptfds, const struct timespec *timeout, const sigset_t *sigmask);

extern void pdcpDataReq( UInt16 ueIndex, UInt16 lcId, UInt32  transactionId,
        void* data_p, UInt16 size,UInt8 instanceId );

void initPdcpAdapter();
void setPumping(UInt8 flag);
void cleanupPdcpAdapter();

void pdcpDataInd( UInt16 ueIndex, UInt16 lcId, void* data_p, UInt16 size);
void pdcpDataStatusInd( UInt16 ueIndex, UInt16 lcId, 
                        UInt16 deliveryStatus );

void addTunnelIdentier(UInt32, UInt16, UInt8);
extern UInt32 processGtpuAdapterDataReq(UInt8 *,UInt16);
/* SPR 21804 Deleted code */


/****************************************************************************
 * Function Name  : pdcpSourceDlDataRelocInd
 * Description    : The PDCP layer calls this API to forward Downlink SDUs
 *                  to upper layer at source eNodeB in case of handover
 ****************************************************************************/
UInt16 pdcpSourceDlDataRelocInd(UInt16 ueIndex, UInt8 lcId, UInt16 sn,
    void *data_p, UInt32 size);

/****************************************************************************
 * Function Name  : pdcpSourceUlDataRelocInd
 * Description    : The PDCP layer calls this API to forward Uplink SDUs
 *                  to upper layer at source eNodeB in case of handover
 ****************************************************************************/
UInt16 pdcpSourceUlDataRelocInd(UInt16 ueIndex, UInt8 lcId, UInt16 sn,
    void *data_p, UInt32 size
#ifdef KPI_STATS
    ,InternalCellIndex internalCellIndex
#endif    
   );

/****************************************************************************
 * Function Name  : processGtpuAdapterDlDataRelocReq
 * Description    : This funtion is implemented by packet relay.It is used to 
 *                  forward DL PDCP SDUs to packet relay in case of handover.
 ****************************************************************************/
extern UInt32 processGtpuAdapterDlDataRelocReq(
    UInt8 *gtpuDataReq_p,
    UInt16 pdcp_seq_no);

/****************************************************************************
 * Function Name  : processGtpuAdapterUlDataRelocReq
 * Description    : This funtion is implemented by packet relay.It is used to 
 *                  forward UL PDCP SDUs to packet relay in case of handover.
 ****************************************************************************/
extern UInt32 processGtpuAdapterUlDataRelocReq(
    UInt8 *gtpuDataReq_p,
    UInt16 pdcp_seq_no);
/* SPR 3085 Fix Start */
void pdcpEndOfDataForwardingInd(UInt16 ueIndex, UInt8 lcId,
                                           UInt8 isEndMarkerIndRecvd);
extern void processAdapterEndOfDataFwdInd(UInt16 ueIndex, UInt8 lcId,
                                          UInt8 isEndMarkerIndRecvd );
/* SPR 3085 Fix End */ 

#endif
