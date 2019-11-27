/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRelayS1Sap.h,v 1.2 2010/02/18 07:53:34 gur19479 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains variable declarations for Packet
 *                   : Relay and S1-AP Interface. 
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRelayS1Sap.h,v $
 * Revision 1.2  2010/02/18 07:53:34  gur19479
 * updated for cause
 *
 * Revision 1.1.2.3  2010/01/20 22:37:14  gur19479
 * updated for Peer GTP packet handing
 *
 * Revision 1.1.2.2  2009/12/22 09:12:43  gur21897
 * checkin after update
 *
 * Revision 1.1.2.1  2009/11/25 09:57:08  cm_lteenodeb
 * code checkin
 *
 *
 ****************************************************************************/

#ifndef LTE_RELAY_M1_SAP_H
#define LTE_RELAY_M1_SAP_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRelayGtpuSap.h"
#include "lteTypes.h"
#include "lteRelay.h"
#include "lteList.h"
#include "lteQueue.h"
#include "lteRelayAdapter.h"
#include "lteRelayCommon.h"


/*************************************************************************/
/*        API(Message)ID used on M1U- Packet relay Interface   */
/*************************************************************************/
/* SPR 22248 MBMS Changes Start */
#define M1U_SESSION_START           	17 
#define M1U_SESSION_START_CNF 		18
#define M1U_SESSION_STOP 		19
#define M1U_SESSION_STOP_CNF		20
#define M1U_SESSION_UPDATE		21
#define M1U_SESSION_UPDATE_CNF		22
#define M1U_CREATE_AREA_ENTITY_REQ	23
#define M1U_CREATE_AREA_ENTITY_CNF	24
#define M1U_DELETE_AREA_ENTITY_REQ	25
#define M1U_DELETE_AREA_ENTITY_CNF	26
/*SPR 18132 fix start*/
#define M1U_RELAY_MIN_API_ID   M1U_SESSION_START
#define M1U_RELAY_MAX_API_ID   M1U_DELETE_AREA_ENTITY_REQ
/*SPR 18132 fix end*/

#define TUNNEL_INFO 	19
#define AREA_LC_LIST 	 	20
#define TEID_CONFIG_INFO_LIST 	 	21
 /* SPR 22248 MBMS Changes Stop */

#define M1U_RESP_MSG_SIZE 20
#define MAX_EMBMS_SESSIONS 232
#define TUNNEL_INFO_IPV4_LENGTH 17
#define TUNNEL_INFO_IPV6_LENGTH 41
#define IPV6_STRING_LEN 50
#define MAX_AREA_ID 16
#define MAX_EMBMS_LCID 29
#define AREA_LC_LIST_LEN 6
#define M1U_SESSION_START_MIN_LEN (API_HDR+13)
#define M1U_SESSION_STOP_MIN_LEN  (API_HDR+1)
#define M1U_SESSION_UPDATE_MIN_LEN (API_HDR+30)
#define M1U_CREATE_AREA_MIN_LEN (API_HDR+5)
/*SPR 18697 fix start*/
#define M1U_DELETE_AREA_MIN_LEN (API_HDR+26)
/*SPR 18697 fix end*/
/*************************************************************************/
/*        TIGGER ID used For MulticastIp joining or Disjoing   */
/*************************************************************************/
/*Triggers of Mip*/

#define ADD_TRIGGER 1
#define DEL_TRIGGER 2


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

/* structure used for Configure Sap Request between M1 and Relay Interface */

/*validating LC ID */
/* coverity fix 93769 start*/
#define IS_LC_ID_INVALID(ui8_lcId) \
    (((ui8_lcId < 30 )) ? \
      RELAY_GTPU_SUCCESS : RELAY_GTPU_INVALID_LC_ID )
/* coverity fix 93769 end*/
/* validating AREA ID */
#define IS_AREA_ID_VALID(ui8_areaIndex) \
    (((ui8_areaIndex >= MAX_AREA_ID ) ) ? \
        RELAY_GTPU_INVALID_AREA_ID :  RELAY_GTPU_SUCCESS)

/* checking if AREA Context already exists */
#define IS_AREA_ID_NOT_EXISTS(ui8_areaIndex) \
    ((gtpuAreaContextList_g[ui8_areaIndex] != PNULL) ? \
       RELAY_GTPU_AREA_ID_EXISTS :  RELAY_GTPU_SUCCESS)

/* SPR 18458 Fix Start */        
#define ENQUEUE_AREA_NODE(areaQueue,node) \
   enQueue(areaQueue,node)
/* SPR 18458 Fix End */        
    
typedef struct TunnelInfoT
{
	UInt32 teid;   /* BMSC teid for Session*/
	UInt8  ipBytes; /*Ip bytes represent length of IP. IPv4: 4 IPv6: 16 */
	UInt8 mIp[RELAY_ADDR_LEN_IPV6];    /*MulticastIp could be IPV4 or Ipv6*/
    UInt8 sIp[RELAY_ADDR_LEN_IPV6];/*source_IP*/
}TunnelInfo;

typedef struct MIpNodeInfoT {
	LTE_LIST_NODE mIpAnchor; /*Multicast ip anchor*/
    UInt8 ipBytes;
	UInt8 mIp[RELAY_ADDR_LEN_IPV6]; /*multicast ip*/
	UInt8 sIp[RELAY_ADDR_LEN_IPV6]; /*multicast ip*/
	LTE_QUEUE teIdQueue; /*teid queue list of multicastip*/
}M1UMipNodeInfo;

typedef struct TeidNodeT {
	LTE_QUEUE_NODE qN; /*QueueNode of teid*/
	UInt32 teid; /*TunnelEndIdentifer*/
}TeidNode;

typedef struct M1UsessionAreaLcInfoT
{
    UInt8 areaId;
    UInt8 lcId;
}M1USessionAreaLcInfo;

typedef struct M1UCreateAreaReqT
{
    TunnelInfo tunnel;
	UInt8 listCount;
    /* SPR 18387 fix start */
	M1USessionAreaLcInfo sessionSapAreaLcInfo[MAX_AREA_LIST_COUNT]; 
    /* SPR 18387 fix end */
}M1UCreateAreaReq;

/* SPR 18458 Fix Start */        
#define MCCHMAPINDEX 255
/* DS array type*/
typedef struct relayMcchMapT
{
    /*queue on mcchMapIndex */
    LTE_QUEUE mcchMapAreaIdQueue;
}relayMcchMap;

typedef struct TunnelInfoNodeT
{
    LTE_LIST_NODE nodeAnchor;
    TunnelInfo     tunnel;
}TunnelInfoNode;

typedef struct mcchMapAreaIdNodeT
{
    /*anchor node*/
    LTE_LIST_NODE       mcchMapAreaIdAnchor;
    UInt8 lcId;
    TunnelInfoNode* tunnelInfoNode_p;
}mcchMapAreaIdNode;
/*For syncMspMapIndex */

typedef struct M1uAreaContextT
{
    UInt8     valid;
    TunnelInfoNode* tunnelInfoNode_p;
}M1uAreaContext;

extern relayMcchMap relayMcchMapIndex_g[MAX_AREA_ID][MCCHMAPINDEX+1];  
extern M1uAreaContext M1uAreaContext_g[MAX_AREA_ID][MAX_EMBMS_LCID];

typedef enum {
    PR_INDEX_ERR = -1,
    PR_FAIL,
    PR_SUCCESS
}pr_return_t;
/* SPR 18458 Fix End */        
/****************************************************************************
 *   Function Name  : processM1uSessionStartRequest
 *   Inputs         : sessionReq_p,
 *                      msgLen
 *   Outputs        : None
 *   Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 *   Description    : This funtion process SAP M1U session start request .
 *****************************************************************************/
UInt32 processM1uSessionStartRequest(UInt8 *sessionReq_p, UInt16 msgLen);
UInt32 processM1uCreateAreaEntityReq(UInt8 *sessionReq_p, UInt16 msgLen);
UInt32 processM1uDeleteAreaEntityReq(UInt8 *sessionReq_p, UInt16 msgLen);
/****************************************************************************
 *    Function Name  : processM1uSessionStopRequest
 *    Inputs         : sessionReq_p,
 *                      msgLen
 *    Outputs        : None
 *    Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 *    Description    : This funtion process SAP M1U session stop request .
 *****************************************************************************/
UInt32 processM1uSessionStopRequest(UInt8 *sessionReq_p, UInt16 msgLen);

/****************************************************************************
 *    Function Name  : processM1uSessionUpdateRequest
 *    Inputs         : sessionReq_p,
 *                      msgLen
 *    Outputs        : None
 *    Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 *    Description    : This funtion process SAP M1U session Update request .
 *****************************************************************************/
UInt32 processM1uSessionUpdateRequest(UInt8 *sessionReq_p, UInt16 msgLen);

/****************************************************************************
 *   Function Name  : initMipList
 *   Inputs         : None
 *   Outputs        : None
 *   Returns        :
 *   Description    : This funtion process SAP M1U session Update request .
 *****************************************************************************/
void initmIpList(void);
/****************************************************************************
 *   Function Name  : processMipAndTeid
 *   Inputs         : mIp - nulticastIp
 *                      teid - tunnelidentifier
 *                       trigger - Either ADD/DEL
 *   Outputs        :
 *   Returns        :
 *   Description    : This function process for triggering joining or disjoing
 *                    request for MulticastIp
 *          
  ****************************************************************************/
UInt8 processMipAndTeid(UInt8 *mIp,
        UInt8 *sIp,
        UInt8  ipBytes,
		UInt32 teid,
		UInt8 trigger);

        /*SPR 18111 fix */
UInt8 updateMipAndTeid(UInt8 *oldMip,UInt8 *oldSip,
        UInt8 *newMip,
        UInt8 *newSip,
        UInt8  newIpBytes,
        UInt8  oldIpBytes,
        UInt32 oldTeid,
        UInt32 newTeid);
        /*SPR 18111 fix */
 UInt8 updateTeidTree(TunnelInfo *newSessionSapReq_p,TunnelInfo *oldSessionSapReq_p);
 UInt8 addNodeToTeidTree(TunnelInfo* sessionSapReq_p);
 UInt8 deleteNodeFromTeidTree(TunnelInfo *sessionSapReq_p);
/****************************************************************************
 *  Function Name  : parseSessionSapAreaLcInfo
 *  Inputs         : UInt8 *sessionReq_p
 *                   UInt16 *tagLen
 *     *                  SessionSapAreaLcInfo *sessionSapAreaLcInfo_p,
 *      *                  UInt16 *current_p
 *       * Outputs        : None
 *        * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 *         * Description    : This funtion process SAP M1U LC INFO for
 *          *                  Create Area Entity request API.
 *           ****************************************************************************/
UInt32 parseSessionSapAreaLcInfo(
                                 UInt8 *sessionReq_p,
                                 UInt16 *tagLen_p,
                                 M1USessionAreaLcInfo *sessionSapAreaLcInfo_p,
                                 UInt16 *current_p
                                );

/****************************************************************************
 * Function Name  : deleteAreaLclistFromTeidNode 
 * Inputs         : mIp - nulticastIp
 * 		    teid - tunnelidentifier
 * 		    trigger - Either ADD/DEL
 * Outputs        : 
 * Returns        :
 * Description    : This function process the following 
 * 		   1. Fetch teid node from teidtree with key as TEID
 * 		   2. Delete AreaLclist from the existing list info
 * 		     
 ****************************************************************************/
UInt32 DeleteAreaLclistFromTeidNode(
		M1UCreateAreaReq* sessionSapReq_p); 

/****************************************************************************
 * Function Name  : addAreaLclistFromTeidNode 
 * Inputs         : sessionSapReq_p - areaLclistinfo &teid infomration 
 * Outputs        : 
 * Returns        :
 * Description    : This function process the following 
 * 		   1. Fetch teid node from teidtree with key as TEID
 * 		   2. Add AreaLclist with existing list info
 * 		     
 ****************************************************************************/
UInt32 addAreaLcListFromTeidNode(
		M1UCreateAreaReq* sessionSapReq_p);
/****************************************************************************
 * Function Name  : deleteM1uAreaLcNode
 * Inputs         : UInt8 areaIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function deletes area lc context from teId node
 ***************************************************************************/
void deleteM1uAreaLcNode(UInt8 areaIndex);
/****************************************************************************
 * Function Name  : addM1uAreaLcNode
 * Inputs         : UInt8 areaIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function add area lc node into teId Tree
 ***************************************************************************/
void addM1uAreaLcNode(UInt8 areaIndex,mcchMapAreaIdNode *mcchMapAreaIdNode_p);
#endif
