/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRelayOamSap.h,v 1.1.1.1 2010/02/11 04:51:36 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains variable declarations for Packet
 *                   : Relay and OAM Interface. API format exposed
 *                   : to OAM is same as required for GTP-U.
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRelayOamSap.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:36  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.1  2009/11/25 09:57:08  cm_lteenodeb
 * code checkin
 *
 *
 ****************************************************************************/

#ifndef  LTE_RELAY_OAM_SAP_H
#define LTE_RELAY_OAM_SAP_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRelayOamInterface.h"
#include "lteRelay.h"

/* Trace types */
#define RELAY_GTPU_INCOMING_TRACE 0x0001
#define RELAY_GTPU_OUTGOING_TRACE 0x0002
#define RELAY_GTPU_FSM_TRACE      0x0004
#define RELAY_GTPU_ERROR_TRACE    0x0008

#define RELAY_GTPU_NO_TRACE       0
#define RELAY_GTPU_BRIEF_TRACE    1
#define RELAY_GTPU_DETAILED_TRACE 2

#define RELAY_GTPU_ECHO_RSP_TMR   0

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
/*SPR 17585 +*/
extern UInt8 s1UInitInd_g;
/*SPR 17585 -*/

/****************************************************************************
 * Function Name  : processGtpuSetTimerReq
 * Inputs         : setTimerReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process set timer request for GTPU.
 ****************************************************************************/
UInt32 processGtpuSetTimerReq(UInt8 *setTimerReq_p, UInt16 msgLen);


/****************************************************************************
 * Function Name  : processGtpuGetTimerReq
 * Inputs         : getTimerReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process get timer request for GTPU.
 ****************************************************************************/
UInt32 processGtpuGetTimerReq(UInt8 *getTimerReq_p, UInt16 msgLen);


/****************************************************************************
 * Function Name  : processGtpuGetMaxRetriesReq
 * Inputs         : getMaxRetriesReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process get max retires request for GTPU.
 ****************************************************************************/
UInt32 processGtpuGetMaxRetriesReq(UInt8 *getMaxRetriesReq_p, UInt16 msgLen);


/****************************************************************************
 * Function Name  : processGtpuSetMaxRetriesReq
 * Inputs         : setMaxRetriesReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process set max retries request for GTPU.
 ****************************************************************************/
UInt32 processGtpuSetMaxRetriesReq(UInt8 *setMaxRetriesReq_p, UInt16 msgLen);


/****************************************************************************
 * Function Name  : processGtpuInitTraceLvlReq
 * Inputs         : initTraceLvlReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process init trace level request for GTPU.
 ****************************************************************************/
UInt32 processGtpuInitTraceLvlReq(UInt8 *initTraceLvlReq_p, UInt16 msgLen);


/****************************************************************************
 * Function Name  : processGtpuGetTraceLvlReq
 * Inputs         : getTraceLvlReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process Get trace level request for GTPU.
 ****************************************************************************/
UInt32 processGtpuGetTraceLvlReq(UInt8 *getTraceLvlReq_p, UInt16 msgLen);


/****************************************************************************
 * Function Name  : processGtpuSetTraceLvlReq
 * Inputs         : setTraceLvlReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process set trace level request for GTPU.
 ****************************************************************************/
UInt32 processGtpuSetTraceLvlReq(UInt8 *setTraceLvlReq_p, UInt16 msgLen);


/****************************************************************************
 * Function Name  : processGtpuInitDbReq
 * Inputs         : initDbReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process Init DB Req.
 ****************************************************************************/
UInt32 processGtpuInitDbReq(UInt8 *initDbReq_p, UInt16 msgLen);


/****************************************************************************
 * Function Name  : processGtpuInitDbCnf
 * Inputs         : initDbCnf_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process init db cnf.
 ****************************************************************************/
UInt32 processGtpuInitDbCnf(UInt8 *initDbCnf_p, UInt16 msgLen);


/****************************************************************************
 * Function Name  : processGtpuSetTimerCnf
 * Inputs         : setTimerCnf_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process set timer cnf.
 ****************************************************************************/
UInt32 processGtpuSetTimerCnf(UInt8 *setTimerCnf_p, UInt16 msgLen);

/****************************************************************************
 * Function Name  : processGtpuGetTimerCnf
 * Inputs         : getTimerCnf_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process get timer cnf.
 ****************************************************************************/
UInt32 processGtpuGetTimerCnf(UInt8 *getTimerCnf_p, UInt16 msgLen);

/****************************************************************************
 * Function Name  : processGtpuGetMaxRetriesCnf
 * Inputs         : getMaxRetriesCnf_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process get max retires cnf.
 ****************************************************************************/
UInt32 processGtpuGetMaxRetriesCnf(UInt8 *getMaxRetriesCnf_p, UInt16 msgLen);

/****************************************************************************
 * Function Name  : processGtpuSetMaxRetriesCnf
 * Inputs         : setMaxRetriesCnf_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process set max retries cnf.
 ****************************************************************************/
UInt32 processGtpuSetMaxRetriesCnf(UInt8 *setMaxRetriesCnf_p, UInt16 msgLen);


/****************************************************************************
 * Function Name  : processGtpuInitTraceLvlCnf
 * Inputs         : initTraceLvlCnf_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process init trace level cnf.
 ****************************************************************************/
UInt32 processGtpuInitTraceLvlCnf(UInt8 *initTraceLvlCnf_p, UInt16 msgLen);


/****************************************************************************
 * Function Name  : processGtpuSetTraceLvlCnf
 * Inputs         : setTraceLvlCnf_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process set trace level cnf.
 ****************************************************************************/
UInt32 processGtpuSetTraceLvlCnf(UInt8 *setTraceLvlCnf_p, UInt16 msgLen);


/****************************************************************************
 * Function Name  : processGtpuGetTraceLvlCnf
 * Inputs         : setTraceLvlCnf_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process Get trace level cnf.
 ****************************************************************************/
UInt32 processGtpuGetTraceLvlCnf(UInt8 *getTraceLvlCnf_p, UInt16 msgLen);

/* SPR 2030 fix Start */
/* Logging API Req Start for packet Relay + eGTPu*/
UInt32 processGtpuSetLogLevelReq(UInt8 *setLogLevReq, UInt16 msgLen);
UInt32 processGtpuGetLogLevelReq(UInt8 *getLogLevReq, UInt16 msgLen);
UInt32 processGtpuEnableLogCategoryReq(UInt8 *enLogCatReq, UInt16 msgLen);
UInt32 processGtpuDisableLogCategoryReq(UInt8 *disLogCatReq, UInt16 msgLen);
UInt32 processGtpuGetLogCategoryReq(UInt8 *getLogCatReq, UInt16 msgLen);
/* SPR 2030 fix End */

/* SPR 7085 fix Start */
/****************************************************************************
 * Function Name  : processGtpuGetStatsReq
 * Inputs         : getStatsReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process Get stats Req.
 ****************************************************************************/
UInt32 processGtpuGetStatsReq(UInt8 *getStatsReq_p, UInt16 msgLen);
/****************************************************************************
 * Function Name  : processGtpuGetStatsResp
 * Inputs         : getStatsCnf_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process Get stats Resp.
 ****************************************************************************/
UInt32 processGtpuGetStatsResp(UInt8 *getStatsCnf_p, UInt16 msgLen);
/****************************************************************************
 * Function Name  : handleGtpuGetStatsReq
 * Inputs         : getStatsReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process Get stats Resp.
 ****************************************************************************/
UInt32 handleGtpuGetStatsReq(relayGtpuGetStatsReq *getStatsReq_p);
/* SPR 7085 fix End */

UInt32 processGtpuGetHealthMonitoringReq(UInt8 *hsmReq, UInt16 msgLen);

/* SPR 13765 fix Start */
/****************************************************************************
 * Function Name  : processGtpuErrorInd
 * Inputs         : gtpuErrorMsg_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process relay gtpu error indication.
 ****************************************************************************/
UInt32 processGtpuErrorInd(UInt8 *gtpuErrorMsg_p, UInt16 msgLen);
/* SPR 13765 fix END */

/*spr 22474 start*/
/****************************************************************************
 * Function Name  : processGtpuGetDebugInfoReq
 * Inputs		      : getDebugInfoReq_p, msgLen
 * Outputs	      : None
 * Returns	      : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description	      : This funtion process Get Debug Info Request
 ****************************************************************************/
UInt32 processGtpuGetDebugInfoReq (UInt8 *getDebugInfoReq_p, UInt16 msgLen);
/*spr 22474 end*/
/*SPR 17585 +*/
/****************************************************************************
 * Function Name  : s1UInitIndication
 * Inputs         : None
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion prepares S1U Init indications to OAM.
 ****************************************************************************/
UInt32 s1UInitIndication(void);
/*SPR 17585 -*/
#endif
