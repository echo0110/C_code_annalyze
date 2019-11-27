/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpExtScheduler.h,v 1.1.1.1.16.1 2010/10/25 08:31:13 gur21010 Exp $
 *
 ****************************************************************************
 *
 *  File Description : External scheduler interface definition.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpExtScheduler.h,v $
 * Revision 1.1.1.1.16.1  2010/10/25 08:31:13  gur21010
 * Warnings Removed
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.3.2.2  2009/10/09 06:06:45  gur11318
 * variable flag change UInt8 to UInt16.
 *
 * Revision 1.3.2.3.2.1  2009/09/30 12:32:44  gur20439
 * oam and rrc ip seperated.
 *
 * Revision 1.3.2.3  2009/07/09 02:39:55  gur19479
 * updated for port and ip
 *
 * Revision 1.3.2.2  2009/07/07 08:07:27  gur19836
 * Compilation Issues Resolved
 *
 * Revision 1.3.2.1  2009/06/09 11:27:21  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/26 14:55:24  gur19140
 * *** empty log message ***
 *
 * Revision 1.2  2009/05/22 06:04:27  gur20548
 * bugs fixed
 *
 * Revision 1.1  2009/05/20 13:15:56  gur20548
 * Header files name changed
 *
 * Revision 1.4  2009/05/15 09:05:22  gur19140
 * bug fixes
 *
 * Revision 1.3  2009/05/14 11:04:51  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef _EXT_SCHEDULER_DEF_
#define _EXT_SCHEDULER_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <lteTypes.h>
#include "ltePdcpLteDefs.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

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

/****************************************************************************
 * Function Name  : esStart 
 * Inputs         : .
 * Outputs        : None.
 * Returns        : None.
 * Description    : Runs External scheduler.
 ****************************************************************************/
/* + SPR 17439 */
void    esStart(void);

/****************************************************************************
 * Function Name  : esStop 
 * Inputs         : .
 * Outputs        : None.
 * Returns        : None.
 * Description    : Stops External scheduler.
 ****************************************************************************/
void    esStop(void);

UInt32 cleanUpPDCPLayer (void);

/* Execution leg for processing incoming UDP messages
 */
void pdcpElTxProcessSocket(UInt8 *buf,UInt32 length);

/* - SPR 17439 */
void setPdcpCommPort(UInt16 flag,UInt16 pdcpRxPort,UInt16 oamTxPort, UInt16 rrcTxPort, 
                     Char8 *oamIp_p, Char8 *rrcIp_p, 
                     /*SPR 20863 FIX*/
                     Char8 *pdcpIp,Char8 *rrmIp_p, UInt16 rrmTxPort
                     /*SPR 20863 FIX*/
                     /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
                     ,Char8 *gtpuIp_p,UInt16 gtpuRxPort,UInt16 pdcpGtpuRxPort
#endif
                     /*SPR 21188 -*/
);
#if  !defined(PDCP_GTPU_INTF)
void calculatePdcpLatency(UInt32 ueIndex,TimeSpec *startTime,UInt32 entityType,UInt32 msgType);
#endif
/* + SPR 17439 */
void dumpPdcpLatency(void);
/* - SPR 17439 */
void sendFreetoGTP ( void *q, void *buf_p );
void mbSendDataPktsToPr(void *buf_p, UInt16 buf_len);

void pdcpTrigInitInd( UInt8 instanceId );

#endif /* _EXT_SCHEDULER_DEF_ */
