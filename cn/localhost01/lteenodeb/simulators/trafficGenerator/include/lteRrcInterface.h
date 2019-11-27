/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteRrcInterface.h,v 1.2 2010/02/18 07:34:48 gur19479 Exp $
 *
 ****************************************************************************
 *
 *  File Description :
 *  This file describes APIs for RRC to controls the configuration of Entity. 
 *  The presence of configuration parameters depends on the mode of operation
 *  each Entity is configured with.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRrcInterface.h,v $
 * Revision 1.2  2010/02/18 07:34:48  gur19479
 * updated module id
 *
 * Revision 1.1.2.4.2.1  2009/09/24 21:45:45  gur19479
 * updated for video and audio processing
 *
 * Revision 1.1.2.4  2009/08/12 08:23:52  gur19836
 * Graceful shutdown of traffic generator code added
 *
 * Revision 1.1.2.3  2009/08/10 11:02:12  gur19140
 * mimo changes in tg and memory leak fixes
 *
 * Revision 1.1.2.2  2009/07/14 14:08:22  gur19836
 * CR Comments Disposed
 *
 * Revision 1.1.2.1  2009/07/12 09:37:31  gur19479
 * updated for traffic generator
 *
 *
 ****************************************************************************/

#ifndef LTE_TRAFFIC_GEN_RRCINTERFACE_H
#define LTE_TRAFFIC_GEN_RRCINTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "lteTypes.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/* 128 UE changes start */
/* 255 UE changes start */
/* +- SPR 18268 */
/* SPR 20526 Changes Start*/
#define MAX_UE_ID       MAX_UE_SUPPORTED /* maximum number of UE supported */
/* SPR 20526 Changes End*/
/* +- SPR 18268 */
/* 255 UE changes end */
/* 128 UE changes end */
#define MAX_LC_ID       10  /* maximum number of logical channels per UE */

#define TRAFFIC_GEN_INVALID_ID -1

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* TRAFFIC GENERATOR - RRC Related APIs*/
#define     UE_CREATE_ENTITY_REQ            0
#define     UE_RECONFIG_ENTITY_REQ          1 
#define     UE_DELETE_ENTITY_REQ            2
#define     TRAFFIC_GEN_START_STOP_REQ      3
#define     TRAFFIC_GEN_CLEANUP_REQ         4
#define     RECONFIG_UE_ADD_REQ             4
#define     RECONFIG_UE_DEL_REQ             5
#define     UE_CREATE_ENTITY_CNF            6
#define     UE_RECONFIG_ENTITY_CNF          7 
#define     UE_DELETE_ENTITY_CNF            8
#define     TRAFFIC_GEN_START_STOP_CNF      9 
/*Traffic Generator response code*/
#define     TRAFFIC_GEN_MODULE_ID         100 
#define     TRAFFIC_GEN_API_HEADER_LEN    10U

enum
{
    TRAFFIC_GEN_FAILURE = 0,
    TRAFFIC_GEN_SUCCESS,
    TRAFFIC_GEN_PARTIAL_SUCCESS
};

typedef struct ueLcInfoT {
    UInt8 lcId;
    UInt8 rbDirection; /*Radio bearer link type (TX, RX, Both).*/
}ueLcInfo;

typedef struct ueConfigReqT {
    UInt16  ueIndex ;       /* user equipment index.  */
    UInt16  ueId ;
    UInt8   lcCount;
    ueLcInfo lcInfo[MAX_LC_ID];
}ueConfigReq ;

typedef struct ueDeleteInfoT {
    UInt8    lcId;
}ueDeleteInfo;

typedef struct ueDeleteReq {
    UInt16  ueIndex ;       /* user equipment index.  */
}ueDeleteReq;

typedef struct ueReConfigReqT {
    UInt16  ueIndex ;       /* user equipment index.  */
    UInt16 ueCreateCount;
    UInt16 ueDeleteCount;
    ueLcInfo ueAddReq[MAX_LC_ID];
    ueDeleteInfo ueDelReq[MAX_LC_ID];
}ueReconfigReq ;

typedef struct ueTrafficCtrlReqT {
    UInt8 trafficCtrlBit;
    UInt8 periodicCtrlBit;
    UInt8 packetCount;	
}ueTrafficCtrlReq;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* TRAFFIC_GEN_SUCCESS when deleted successfuly .*/

UInt32 ueDelReq(ueDeleteReq *ueDeleteReq_p);

UInt32 ueReConfigureReq(ueReconfigReq *ueReconfigReq_p);

UInt32 ueConfigureReq(ueConfigReq *ueConfigReq_p);

UInt32 ueTrafficStartStopReq(ueTrafficCtrlReq *ueTrafficCtrlReq_p);

UInt32 ueHandleResponseMsgForRrc(UChar8 *buffer_p, UInt16 msgLen);

UInt32 ueHandleResponseMsgForOam(UChar8 *buffer_p, UInt16 msgLen);

UInt32 processCreateUeReq(UInt8 *createUeReq_p);

UInt32 processReconfigUeReq(UInt8 *reconfigUeReq_p);

UInt32 processDeleteUeReq( UInt8 *deleteUeReq_p);

UInt32 processTrafficGenStartStopReq(UInt8 *trafficStartStopReq_p);

UInt32 processTrafficGenCleanupReq(UInt8 *trafficStartStopReq_p);

UInt32 processCreateUeCnf(UInt16 transId, UInt16 modId, UInt16 response, UInt16 ueIndex);

UInt32 processReconfigUeCnf(UInt16 transId, UInt16 modId, UInt16 response, UInt16 ueIndex);

UInt32 processDeleteUeCnf(UInt16 transId, UInt16 modId, UInt16 response, UInt16 ueIndex);

UInt32 processTrafficGenStartStopCnf(UInt16 transId, UInt16 modId, UInt16 response, UInt16 ueIndex);

UInt16 initTrafficGen();

UInt32 trafficGenInitDbReq();

#endif  /* TRAFFIC_GEN-RRC INTERFACE_H */
