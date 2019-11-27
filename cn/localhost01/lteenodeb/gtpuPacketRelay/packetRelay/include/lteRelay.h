/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRelay.h,v 1.3.4.2 2010/08/02 08:57:15 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains variable declarations for Packet
 *                   : Relay.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRelay.h,v $
 * Revision 1.3.4.2  2010/08/02 08:57:15  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.2  2010/02/18 07:54:21  gur19479
 * updated for tunnel ids
 *
 * Revision 1.1.2.2  2010/01/20 22:35:26  gur19479
 * updated for Peer GTP Packet handing
 *
 * Revision 1.1.2.1  2009/11/25 09:57:08  cm_lteenodeb
 * code checkin
 *
 *
 ****************************************************************************/

#ifndef LTE_RELAY_H
#define LTE_RELAY_H


#include "lteTypes.h"
#include "lteMisc.h"
#include "lteMemPool.h"
#include "egtpu_glob.h"
#include "egtpu_ietypes.h"
#include "egtpu_glob_struct.h"
#include "egtpu_entr.h"
/* SPR 2030 fix Start*/
/* SPR 2241 fix Start*/
#ifndef UE_SIM_TESTING
#include "logging.h"
#include "alarm.h" 
#endif
/* SPR 2241 fix End*/
#include "lteLayer2CommanTypes.h"
#include "lteOamInterface.h"
#include "lteConfigFileHdlr.h"
/* SPR 2030 fix End*/
#include "lteLayer2PdcpGtpuInterface.h"

extern egtpu_void_t egtpu_process_msg (egtpu_global_t *, U8bit *);
extern egtpu_global_t *p_global;

#define GTPU_MODULE_ID          9
#define RELAY_OAM_MODULE_ID     1
#define RELAY_S1AP_MODULE_ID    3
#define RELAY_MODULE_ID         4
#define RELAY_ADAPTER_MODULE_ID 10
#define MAX_MODULE_ID         RELAY_ADAPTER_MODULE_ID

/*************************************************************************/
/*        IEs used on S1U- Packet relay Interface   */
/*************************************************************************/
#define S1U_IE_SEQ_NUM                    1
#define S1U_IE_REORDERING_REQD            2
#define S1U_IE_SEQ_DISABLE                3
#define S1U_IE_GSN_ADDR_RECOVERED_TAG     4          /* SPR 13765 fix */
#define S1U_IE_GSN_ADDR                   5
#define S1U_IE_GSN_ADDR_TAG               5
#define S1U_IE_QOS_PROFILE                6
#define S1U_IE_RELAY_SETUP_SAP_REQ        7
#define S1U_IE_RELAY_SETUP_SAP_CNF_TAG    8
#define S1U_IE_RELAY_REL_SAP_REQ          9 
#define S1U_IE_RELAY_REL_SAP_CNF_TAG      10
#define S1U_IE_TUNNEL_INFO                11
#define S1U_IE_TEID_SELF                  12
#define S1U_IE_TEID_SELF_TAG              12
#define S1U_IE_BUFFER_IND                 13
#define S1U_IE_TEID_PEER                  14
#define S1U_IE_RELEASE_TUNNEL_INFO        15
#define S1U_IE_TUNNEL_INFO_CNF_TAG        16
#define S1U_IE_SEQ_NUM_TAG                1 
/** INTRA_ENB_HO Fix Start */
#define S1U_IE_PDCP_EXT_FLAG_TAG          17      
/** INTRA_ENB_HO Fix End */      

#if defined (ENDC_ENABLED) && !defined(UE_SIM_TESTING)
#define S1U_IE_DC_BEARER_TYPE_INFO        22
#define S1U_IE_SAP_PARAM_TAG              23
#endif


/*************************************************************************/
/*        API(Message)ID used on S1U- Packet relay Interface   */
/*************************************************************************/
#define S1U_CREATE_UE_ENTITY_REQ        1 
#define S1U_RECONFIGURE_UE_ENTITY_REQ   5
#define S1U_DELETE_UE_ENTITY_REQ        3             
#define S1U_CREATE_UE_ENTITY_CNF        2
#define S1U_RECONFIGURE_UE_ENTITY_CNF   6
#define S1U_DELETE_UE_ENTITY_CNF        4
/* SPR 13378 fix start */
#define S1U_ERROR_IND                   7
/* SPR 13378 fix end */
/*+SPR 3456*/
#define S1U_PATH_FAILURE_IND            8
#define S1U_PATH_SUCCESS_IND            10 
/*-SPR 3456*/
#define S1U_END_MARKER_IND              9

/* Intra-eNB HO Changes Start */
#define RRC_S1U_INTRA_ENB_DATA_FWD_REQ      11
#define RRC_S1U_INTRA_ENB_DATA_FWD_CNF      12
#define RRC_S1U_LOCAL_PATH_SWITCH_REQ       13
#define RRC_S1U_LOCAL_PATH_SWITCH_RESP      14
/* Intra-eNB HO Changes End */


#ifdef ENDC_ENABLED
#define X2U_LC_RELEASE_IND                  31
#define X2U_SUSPEND_UE_ENTITY_REQ           32
#define X2U_SUSPEND_UE_ENTITY_CNF           33
#endif

#define MAX_RAB_ID                      8

/************************************************************************/

/************************************************************************/
/*           GTPU CONTROL APIS (S1AP and Packet Relay Interface)        */
/************************************************************************/
#define RELAY_BASE_CP_API                    20
#define RELAY_GTPU_SETUP_SAP_REQ             RELAY_BASE_CP_API + 0
#define RELAY_GTPU_RELEASE_SAP_REQ           RELAY_BASE_CP_API + 1

/*#define RELAY_GTPU_CP_SETUP_SAP_CNF          RELAY_BASE_CP_API + 2 */
#define RELAY_GTPU_CP_SETUP_SAP_CNF          250
#define RELAY_GTPU_CP_RELEASE_SAP_CNF        251 

#define RELAY_GTPU_CP_ERROR_IND              RELAY_BASE_CP_API + 4
#define RELAY_GTPU_CP_PATH_FAILURE_IND       RELAY_BASE_CP_API + 5
#define RELAY_GTPU_CP_END_MARKER_IND         RELAY_BASE_CP_API + 6
/*+SPR #3456*/
#define RELAY_GTPU_CP_PATH_SUCCESS_IND       RELAY_BASE_CP_API + 7
#define RELAY_MAX_CP_API                     RELAY_GTPU_CP_PATH_SUCCESS_IND + 1
/*-SPR #3456*/



/************************************************************************/

#define RELAY_BASE_ADAPTER_API               30
#define RELAY_GTPU_DATA_REQ                  RELAY_BASE_ADAPTER_API + 0
#define RELAY_GTPU_DATA_IND                  RELAY_BASE_ADAPTER_API + 1
#define RELAY_GTPU_PDCP_END_MARKER_IND       RELAY_BASE_ADAPTER_API + 2
 
/*To remove warning arrary out of bound */
/*#define RELAY_MAX_ADAPTER_API                RELAY_GTPU_DATA_IND + 1*/
#define RELAY_MAX_ADAPTER_API                RELAY_MAX_LM_API + 1
#define GTPU_RELAY_UNITDATA_REQ              150
/************************************************************************/
/*           Constant Values                                            */
/************************************************************************/

#define RELAY_GTPU_FAILURE            0
#define RELAY_GTPU_SUCCESS            1
#define MAX_PAYLOAD_SIZE              64000
/* COVERITY 10309 */
#define MAX_RECV_BUFF_SIZE            2000
/* COVERITY 10309 */
#define CSPL_HDR                      16 
#define START_PAYLOAD                 64
#define START_OFFSET_DL               5
#define START_OFFSET_UL               59
#define MAX_API_LEN                   1000
#define TUNNEL_ID_HDR                 5
#define RELAY_GTPU_TRANS_ID_RESERVED  0
#define RELAY_GTPU_TRANS_ID_NOT_REQD  0
#define RELAY_GTPU_LEN_NOT_REQD       0
#define RELAY_GTPU_NOT_IN_USE         0
#define RELAY_RECV_ALL_RSP            0
#define RELAY_GTPU_TUNNEL_ID_INVALID  0
#define RELAY_GTPU_IN_USE             1
#define RELAY_GTPU_TUNNEL_ID_TRUE     1
#define RELAY_GTPU_TRANS_ID_TRUE      1
#define RELAY_ADDR_LEN_IPV4           4
/*#define RELAY_ADDR_LEN_IPV6                           6*/

/*  SPR 14462 Fix Starts */ 
#define RELAY_HUNDRED                 100
#define RELAY_TEN                     10
#define RELAY_FOUR                    4
/*  SPR 14462 Fix Ends */

/* 255 UE changes start */
#define RELAY_GTPU_MAX_UE  MAX_UE_SUPPORTED
/* 255 UE changes start */
#define RELAY_GTPU_MAX_LC             8 
#define RELAY_TAG_LENGTH              3

#define RELAY_SEQ_NO_PRESENT          0x02


/*  + Layer2 NON CA Changes */  


#define OAM_L2_API_HDR  INTERFACE_API_HEADER_LEN
#define API_HDR INTERFACE_API_HEADER_LEN

#define PACKET_RELAY_VALIDATE_OAM_RRC_CELL_ID(cellId) \
layer2CommonLookupIfValidCellIndex(cellId); 

#define PACKET_RELAY_GET_RRC_CELL_INDEX(internalCellIndex) \
layer2CommonGetRRCCellIndex(internalCellIndex);

#define PACKET_RELAY_GET_INTERNAL_CELL_INDEX(cellId) \
layer2CommonGetInternalCellIndex(cellId);

/*  - Layer2 NON CA Changes */  

#define RELAY_TAG_LEN                 3
#define RELAY_API_ID_POS              6  
#define RELAY_MSG_LEN_POS             8  
#define RELAY_SIZE_OF_TAG             2
#define RELAY_SIZE_OF_LEN             2
#define RELAY_SIZE_OF_TUNNELID        4
#define RELAY_SIZE_OF_SEQ_NUM         2 
#define RELAY_ADDR_LEN_IPV4           4
/*IPV6_CHANGE - START*/
#define RELAY_ADDR_LEN_IPV6           16
#define IPV6_ADDR_LEN		RELAY_ADDR_LEN_IPV6
/*IPV6_CHANGE - END*/
#define SOCKET_BIND_FAIL              -1
#define RELAY_INVALID_FD              -1
#define FAILED_TO_SEND_ON_UDP_SOCK    -1

#define RELAY_GTPU_PERR_UNEXPECTED_MSG 2

#define TRANS_ID_NO_REQ                0
#define TRANS_ID_DUPLICATE_REQ         1
#define DETACH_INFO_SEND_TO_DLTG       1 
#define NO_DETACH_INFO_SEND_TO_DLTG    0
#if !defined(UE_SIM_TESTING) &&  defined (ENDC_ENABLED)
#define MAX_NUM_TUNNEL_PER_LC          4
#else
#define MAX_NUM_TUNNEL_PER_LC          3
#endif
#define MAX_NUM_FWD_TUNNEL_PER_LC      2    

#define ADAPTER_GLUE_IP    "127.0.0.1"
#define ADAPTER_GLUE_PORT  21211
#define TG_GLUE_IP         "127.0.0.1"
#define TG_GLUE_PORT       6666
/* LIPA changes start */
#define SECONDARY_TG_PORT  7777
/* LIPA changes end */


#define RELAY_GTPU_MAX_TRANS_ID RELAY_GTPU_MAX_UE * RELAY_GTPU_MAX_LC * MAX_NUM_TUNNEL_PER_LC 
/* Handover Coding - Prashant Changes Start*/
#define INVALID_TUNNEL_ID  0xFFFFFFFF
/* Handover Coding - Prashant Changes End*/

#define INVALID_TUNNEL_TYPE 0xFF
#define GTPU_CAUSE_SUCCESS 0
#define GTPU_CAUSE_RSRC_NOT_AVAILABLE 2
/* SPR 7085 fix start */
#define RELAY_NOT_RESET  1
/* SPR 7085 fix end */
/* SPR 18387 fix start */
#ifdef LTE_EMBMS_SUPPORTED
#define MAX_AREA_LIST_COUNT 255
#endif
/* SPR 18387 fix end */

#ifdef ENDC_ENABLED
#define MIN_S1U_IE_LCID_LENGTH 5
#endif 
/* Function Pointers to handle all APIs of all Modules */

/* Start of change for VARIBLE Declaration */
extern UInt32 (*processRelayMessage[RELAY_MAX_ADAPTER_API])(UInt8 *buffer_p, UInt16 msgLen);

/* Function Pointers to handle all responses for all modules */
extern UInt32 (*sendConfirmationRsp[RELAY_MAX_ADAPTER_API])
    (UInt8 *buffer_p,UInt16 transId, UInt16 totalLen);

/* End of change for VARIBLE Declaration */

/* to read a 16 bit value starting at the location p_buff */
#define RELAY_GTPU_GET_U16BIT(p_buff)                                        \
    ((UInt16)(*(p_buff) << 8) |                                        \
        (UInt16)(*(p_buff + 1)))

/* to read a 24 bit value starting at the location p_buff */
#define RELAY_GTPU_GET_U24BIT(p_buff)                                        \
    ((UInt32)(*(p_buff) << 16) |                                       \
        (UInt32)(*(p_buff + 1) << 8) |                                 \
        (UInt32)(*(p_buff + 2)))

/* to read a 32 bit value starting at the location p_buff */
#define RELAY_GTPU_GET_U32BIT(p_buff)                                        \
    ((UInt32)(*(p_buff) << 24) |                                       \
        (UInt32)(*(p_buff + 1) << 16) |                                \
        (UInt32)(*(p_buff + 2) << 8) |                                 \
        (UInt32)(*(p_buff + 3)))
    /*SPR 2030 fix Start*/
/* to read a 64 bit value starting at the location p_buff */
#define RELAY_GTPU_GET_U64BIT(p_buff) \
        (((UInt64)(*(p_buff)) << 56) |                                   \
         ((UInt64)(*(p_buff + 1)) << 48 ) |                              \
          ((UInt64)(*(p_buff + 2)) << 40 ) |                            \
           ((UInt64)(*(p_buff + 3)) << 32 ) |                            \
            ((UInt64)(*(p_buff + 4)) << 24 ) |                            \
             ((UInt64)(*(p_buff + 5)) << 16 ) |                            \
              ((UInt64)(*(p_buff + 6)) << 8 ) |                           \
               (UInt64)(*(p_buff + 7)))
    /*SPR 2030 fix End*/

/* macros to set U16bit, U24bit, and UInt32 values onto buffer */
/* p_buff must be typecast to a (UInt8 *) before using these macros */

/*SPR 3587 Fix Start*/
/* to write a 16 bit port number starting from the location p_buff */
#define RELAY_SET_PORT_NUM_IE(p_buff, port_num) \
    do {                                        \
        *(p_buff) = (UInt8)((port_num) >> 8) ;  \
        *(p_buff + 1) = (UInt8)(port_num);      \
    } while (0)

/* to write a 32 bit IPv4 Address starting from the location p_buff */
#define RELAY_SET_ENTITY_ADDR_IE(p_buff, ip_addr)   \
    do {                                         \
        *(p_buff) = (UInt8)((ip_addr) >> 24);    \
        *(p_buff + 1) = (UInt8)((ip_addr) >> 16);\
        *(p_buff + 2) = (UInt8)((ip_addr) >> 8); \
        *(p_buff + 3) = (UInt8)(ip_addr);        \
    } while (0)

#define RELAY_GTPU_SET_VALUE(p_buff, val) \
    do {                                  \
        *(p_buff) = (UInt8)(val);         \
       } while(0)
/*SPR 3587 Fix End*/

/* to write a 16 bit value starting from the location p_buff */
#define RELAY_GTPU_SET_U16BIT(p_buff, val)                                   \
    do {                                                               \
        *(p_buff) = (UInt8)((val) >> 8) ;                              \
        *(p_buff + 1) = (UInt8)(val);                                  \
    } while (0)

/* to write a 24 bit value starting from the location p_buff */
#define RELAY_GTPU_SET_U24BIT(p_buff, val)                                   \
    do {                                                               \
        *(p_buff) = (UInt8)((val) >> 16);                              \
        *(p_buff + 1) = (UInt8)((val) >> 8);                           \
        *(p_buff + 2) = (UInt8)(val);                                  \
    } while (0)

/* to write a 32 bit value starting from the location p_buff */
#define RELAY_GTPU_SET_U32BIT(p_buff, val)                                   \
    do {                                                               \
        *(p_buff) = (UInt8)((val) >> 24);                              \
        *(p_buff + 1) = (UInt8)((val) >> 16);                          \
        *(p_buff + 2) = (UInt8)((val) >> 8);                           \
        *(p_buff + 3) = (UInt8)(val);                                  \
    } while (0)

    /*SPR 2030 fix Start*/
#define RELAY_GTPU_SET_U64BIT(p_buff, val)                                   \
    do {                                                               \
        *(p_buff) = (UInt8)((val) >> 56);                              \
        *(p_buff + 1) = (UInt8)((val) >> 48);                          \
        *(p_buff + 2) = (UInt8)((val) >> 40);                           \
        *(p_buff + 3) = (UInt8)((val) >> 32);                           \
        *(p_buff + 4) = (UInt8)((val) >> 24);                              \
        *(p_buff + 5) = (UInt8)((val) >> 16);                          \
        *(p_buff + 6) = (UInt8)((val) >> 8);                           \
        *(p_buff + 7) = (UInt8)(val);                                  \
    } while (0)
    /*SPR 2030 fix End*/

/* To get the index for tunnel Id */

/************************************************************************/
/*           Packet Relay SAP Flags                                     */
/************************************************************************/

/* Macros of SAP flags */
#define PR_EGTPU_RX_SAP                             0x01
#define PR_EGTPU_TX_SAP                             0x02
#define PR_EGTPU_FROM_RELAY_SAP                     0x04
#define PR_EGTPU_TO_RELAY_SAP                       0x08
#define PR_EGTPU_RELOC_DATA_SAP                     0x10
#define PR_EGTPU_FWD_SAP                            0x20
#define PR_EGTPU_DIRECTION_SAP                      0x40
#define PR_EGTPU_ALL_SAP                            0x0f



#define SOURCE_ENB_VALUE 0
#define TARGET_ENB_VALUE 1

/* LIPA changes start */
#define S1U_SECONDARY_TUNNEL_SAP_FLAG               0x0e
/* LIPA changes end */

#if defined(ENDC_ENABLED) &&  !defined(UE_SIM_TESTING)
#define S1U_SN_TERMINTED_SPLIT_BEARER_SAP_FLAG      0x9F
#endif


/*SPR 2431 fix Start*/
/* Global to store myAddress received in INITDB_REQ*/


/* Start of change for VARIBLE Declaration */
extern UInt8 myAddr[RELAY_ADDR_LEN_IPV6];
/*IPV6_CHANGE - START*/
extern UInt8 myAddr4[RELAY_ADDR_LEN_IPV4];
extern UInt8 myAddr6[RELAY_ADDR_LEN_IPV6];
extern UInt8 myAddrType;
/*IPV6_CHANGE - END*/
/* End of change for VARIBLE Declaration */

/*SPR 2431 fix End*/
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

typedef enum tunnelType_enT
{
    NORMAL_BEARER,
    DL_FORWARDING, 
    UL_FORWARDING
#if defined(ENDC_ENABLED) &&  !defined(UE_SIM_TESTING)
	,SCG_SPLIT_BEARER
#endif

}tunnelType_en;

typedef enum rbDirection_enT
{
    DIR_TX,
    DIR_RX,
    DIR_BOTH_TX_RX
}rbDirection_en;
/*SPR 21366 +*/
#if defined PDCP_GTPU_INTF
typedef struct gtpuPdcpMapT
{
    Char8 pdcpIpAddr[20]; /* For holding PDCP IP */
    UInt32 pdcpRxPort;    /* Port No of PDCP for receiving messages from other modules */
    SockAddr pdcpRxAddr;  /* PDCP Recv address   */
    SInt8 pdcpRxFd;       /* File des for PDCP RX */
}gtpuPdcpMap;
#ifdef ENDC_ENABLED
typedef struct x2uRlcMapT
{
    Char8 rlcIpAddr[20]; /* For holding PDCP IP */
    SockAddr rlcRxAddr;  /* PDCP Recv address   */
    SInt8 rlcRxFd;       /* File des for PDCP RX */
}x2uRlcMap;
#endif
#endif 
/*SPR 21366 -*/
/* This structure is used for storing configuration info for packet relay */
typedef struct relayConfigInfoTag
{
 /*SPR #2294 - 
   Removed following
   1.gtpuTxFd,
   2.gtpuRxFd,
   3.oamTxFd,
   4.adapterTxFd,
   5.adapterFromAddr,
   6.gtpuFromAddr,
  */
    SInt8 oamS1ApRxFd;      /* Common file des for OAM S1 AP RX */
    SInt8 s1ApTxFd;         /* File des for S1 Tx */
    SInt8 adapterRxFd;      /* File des for Adapter RX */
    SInt8 oamSocketInf[20]; /* For holding host name of OAM */ 
    SInt8 s1ApSocketInf[20]; /* For holding host name of S1 AP */ 
    Char8 adapterSocketInf[20]; /* For holding host name of Adapter */ 
    Char8 peerTgInf[20];/* For holding the peer TG */
    /*IPV6_CHANGE - START*/
    Char8 peerTgInfV6[64];/* For holding the peer TG - IPv6*/
    Char8 peerTgInfAddrType;   /* For holding the address type IPv4/IPv6/Both */
    /*IPV6_CHANGE - END*/
    UInt32 s1ApTxPort;      /* Port No for S1 AP TX */
    UInt32 s1ApRxPort;      /* Port No for S1 AP RX */
    UInt32 oamRxPort;       /* File des for OAM RX */
    SockAddr oamFromAddr; /* OAM Recv address */
    SockAddr s1ApFromAddr; /* S1 Recv address */
    SockAddr oamToAddr; /* OAM Tx address */
    SockAddr s1ApToAddr; /* S1-AP Tx address */
    SockAddr adapterToAddr; /* Adapter Tx address */
    SockAddr gtpuToAddr; /* gtpu Tx address */
    /* SPR 6949 Fix Start */
    SockAddr sgwToAddr; /* gtpu Tx address */
    sockAddrIn6 sgwToAddrIn6; /* gtpu Tx address */
    /* SPR 6949 Fix End */
    /* LIPA changes start */
    Char8 secPeerTgInf[20];/* For holding the secondary peer TG */
    Char8 secPeerTgInfV6[64];/* For holding the secondary peer TG - IPv6*/
    Char8 secPeerTgInfAddrType;   /* For holding the address type IPv4/IPv6/Both */
    /* LIPA changes end */
#if defined PDCP_GTPU_INTF
    /*SPR 21366 +*/
    UInt32 gtpuRxPort;     /* Port Number used by GTPU for receiving the message from other module */
    SockAddr gtpuFromAddr; /* GTPU Recv address */
    SInt8 gtpuRxFd;        /* File descriptor for GTPU receive socket */
    gtpuPdcpMap gtpu_Pdcp_Ip_port[MAX_INSTANCE_ID]; /*PDCP IP/Port info for sending the message to PDCP*/
#ifdef ENDC_ENABLED
    UInt32 rlcRxPort;    /* Port No of PDCP for receiving messages from other modules */
    x2uRlcMap   x2uRlc_Ip_Port[MAX_INSTANCE_ID];
#endif
#endif

}relayConfigInfo; 
   /*SPR 21366 -*/
/*UInt32 s1ApTxFd_g;
UInt32 oamTxFd_g;*/

/************************************************************************/
/*           GTPU And RELAY APIS                                        */
/************************************************************************/

/* This structure is used for storing Tunnel Identifier */
typedef struct tunnelIdMapTag
{
    UInt8 inUseFlag;        /* Flag which indicates whether this structure is
                               in use or not */
    /* SPR 5978 changes start */
    RrcCellIndex cellIndex;
    /* SPR 5978 changes end */
    UInt16   rrcUeIndex;

    UInt8 numOfsapMsg;      /* To indicate numner of SAP meesages*/
    /* SPR 5481 Fix Start */
    UInt8 EmiFlag;          /* Flag which Indicate the EMI is received or not for releasing the 
                               Forwarding tunnels in case of Data Forwarding is on */
    /* SPR 7844 changes start */
    /* Flag to indicate if it is a target eNodeB, By default it is FALSE */
    UInt8 isTargetENB;      
    /* SPR 7844 changes end */
    /* SPR 6758 changes start */
    /* Flag to indicate Local EMI is received from PDCP so that relay can
     * delete forwarding tunnels */
    UInt8 isLocalEndMarkerIndRecvd;
    /* SPR 6758 changes end */
    /* SPR 5481 Fix End */
    UInt32 tunnelId;        /* To indicate Tunnel Identifier */
    UInt32 tunnelDlFwdId;   /* To indicate Tunnel Identifier for DL Data Forwarding*/
    UInt32 tunnelUlFwdId;   /* To indicate Tunnel Identifier for UL Data Forwarding*/

    /*SPR 2952 Changes Start*/
    UInt32 transId;
    /*SPR 2952 Changes End*/
    /* SPR 5481 Fix Start */
    UInt32 transIdRsp;       /* To indicate the TransIdRsp for Deleting the Forwarding tunnels */ 
    /* SPR 5481 Fix End */
    /* 08032014 604 HO Changes */
    UInt32 normalBearerTransId;
    /* 08032014 604 HO Changes */
    /* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
	DCBearerType dcBearerType;
	UInt8 tunnelAddLen;
	UInt8 x2uCtxCreated;
	/*to store transaction Id created at the time of S1U_RECONFIGURE for split bearer*/
	UInt32 dcTransId;
	/*to store tunned created at the time of S1U_RECONFIGURE for split bearer*/
	UInt32 dcTunnelId;
#endif
	/* OPTION3X Changes End */

}TunnelIdMap;


/* This structure is used for mapping of ue Index and Lc Id */
typedef struct ueIndexLcIdMapTag
{
    UInt8 inUseFlag;        /* Flag which indicates whether this structure is 
                               in use or not */
    UInt16 ueIndex;         /* To indicate ue index */
    UInt8  lcId;            /* To indicate logical channel indentifier */
}UeIndexLcIdMap;

/* This structure is used for mapping of ueIndex, Lc Id and Tunnel id */
typedef struct ueIndexLcIdTunnelIdMapTag
{
    UInt8 inUseFlag;        /* Flag which indicates whether this structure is
                               in use or not */
    UInt16 ueIndex;         /* To indicate ue index */ 
    UInt8 lcId;             /* To indicate logical channel indentifier */
    UInt8 tunnelType;       /* To indicate tunnel type(Normal/DL-UL Forwarding)  */
    UInt8  sapFlags;         /* To identify tunnel properties*/ 
    UInt32 tunnelId;        /* To indicate tunnel identifier */
}UeIndexLcIdTunnelIdMap;

/* This structure is used for identification of transaction identifier IE */
typedef struct relayGtpuTrasIdTag
{
    UInt8 tag;              /* To indicate the TAG of transaction identifier */
    UInt32 tansId;          /* To indicate the value of transaction identifier */
}relayGtpuTrasId;

/* This structure is used for identification of tunnel identifier */
typedef struct relayGtpuTidTag
{
   UInt8 tag;              /* To indicate the TAG of transaction identifier */
   UInt16 length;	
   UInt32 tunnelId;        /* To indicate the value of tunnel identifier */
   UInt32 tunnelDlFwdId;   /* To indicate Tunnel Identifier for DL Data Forwarding*/
   UInt32 tunnelUlFwdId;   /* To indicate Tunnel Identifier for UL Data Forwarding*/
}relayGtpuTid;

typedef struct tlvHeaderTag
{
   UInt16 tag;              /* To indicate the TAG of tag */
   UInt16 length;          /* to indicate lenght */
   UInt8 *data;           /* To indicate data */
}tlvHeader;

/* This structure is used for identification of Seq No */
typedef struct tvSNHeaderTag
{
    UInt8 tag;              /* To indicate the TAG of Sequence no */
    UInt16 SquenceNo;       /* To indicate the value of Seq No */
}tvSNHeader;

/* This structure is used for TV Header format */
typedef struct tvHeaderTag
{
    UInt8 tag;
    UInt8 value;
}tvHeader;

typedef struct tunnelInfoHeaderTag
 {
     UInt8 tag;
     UInt16 length;
     UInt8 value;
}tunnelInfoHeader;

/* This structure is used for UE Context */
typedef struct ueContextTag
{
    UInt8 bitMask;              /* To indicate bitMask */
    UInt8 lcId;                 /* To indicate logical channel identifier */
    UInt32 transId;             /* To indicate transaction identifier */
    UInt32 tunnelId;            /* To indicate tunnel identifier */
    UInt8 cause;                /* To indicate cause */
    UInt16 ulSeqNo;             /* To indicate UL Seq Number */
    UInt16 dlSeqNo;             /* To indicate DL Seq Number */
    UInt8 *pTransportLayerData;
}UeContext;

/* This structure is used for UE Context Response */
typedef struct ueContextForRspTag
{
    UInt16 tid;                /* To indicate transaction identifier for response to S1AP*/
    UInt16 apiIdForRsp;         /* API Id for Response*/
    UInt16 ueIndex;            /* To indicate UE index */
    UInt8 lcCount;             /* To indicate logical channel count */
    UInt8 isFinished;          /* To indicate when need to create a response */
    UeContext ueContext[RELAY_GTPU_MAX_LC]; /* Array of UE Context */
}UeContextForRsp;

typedef struct transIdMappingForLMTag
{
    UInt8 isValid;
/*  + Layer2 NON CA Changes */  
    RrcCellIndex cellId;
/* - Layer2 NON CA Changes */  
    UInt16 transId;
}transIdMappingForLM;

/*typedef struct setupSapCnfDb
  {
     UInt8 lcId;
        UInt32 tunnelId;
  } SetupSapCnfDb;*/

typedef struct S1UeReconfigRBInfoT
{
	UInt32 teid_self;
	UInt32 teid_peer;
    UInt32 teid_selfDlFwd;
    UInt32 teid_peerDlFwd;
    UInt32 teid_selfUlFwd;
    UInt32 teid_peerUlFwd;
	UInt8 lc_id;
    UInt8 isNormalTunnelPresentFlag;
    UInt8 isDlTunnelPresentFlag;
    UInt8 isUlTunnelPresentFlag;
#ifdef ENDC_ENABLED
	UInt8 isScgTunnelPresentFlag;
#endif


}S1UeReconfigCnfInfo;

/* SPR 5599_129174 Fix Start */
typedef struct S1UeReconfigCnfInfoT
/* SPR 5599_129174 Fix End */
{
    UInt16 trans_id;
    UInt16 src_id;
    UInt16 dst_id;
    UInt16 api_id;
    UInt16 api_buf_len;
    UInt16 lcCount;
    S1UeReconfigCnfInfo rbInfo[MAX_RAB_ID];
/*  + Layer2 NON CA Changes */  
    RrcCellIndex cellIndex;
/*  - Layer2 NON CA Changes */  
}S1uUEReconfigCnfInfo;
    
/* Start of change for VARIBLE Declaration */
extern S1uUEReconfigCnfInfo s1uUEReconfigCnfInfo_g[RELAY_GTPU_MAX_UE];
/* End of change for VARIBLE Declaration */

typedef struct seqHeaderTag
{
   UInt8 tag;              /* To indicate the TAG of tag */
   UInt16 length;          /* to indicate lenght */
   UInt8 value;           /* To indicate data */
}seqHeader;

typedef struct reorderReqHeaderTag
{
   UInt8 tag;              /* To indicate the TAG of tag */
   UInt16 length;          /* to indicate lenght */
   UInt8 value;           /* To indicate data */
}reorderReqHeader;
typedef struct qosProfileHeaderTag
{
   UInt8 tag;              /* To indicate the TAG of tag */
   UInt16 length;          /* to indicate lenght */
   UInt8 allocRetPriority;
   UInt8 *data;           /* To indicate data */
}qosProfileHeader;

/* Structure for Forwarding request to PDCP in case of handover*/
typedef struct dataFwdInfoT
{
    UInt8   lcId;                /* Logical Channel Identifier*/
    UInt8   ulDataFwdFlag;       /* Uplink Data Forwarding Flag */
}dataFwdInfoStruct;

typedef struct prDataFwdReqT
{
    UInt16  ueIndex;    /* UE Identifier */
    UInt8   count;      /* No of LCs*/
    UInt8   Reserved;   /* this field is required for alignment. */
    /* SPR 16770 fix start */
    dataFwdInfoStruct dataFwdInfo[MAX_RAB_ID];
    /* SPR 16770 fix end */
}prDataFwdReq;

/* INTRA_ENB_HO Fix Start */
typedef struct pdcpextFlagHeaderTag
{
    UInt8 tag;              /* To indicate the TAG of tag */
    UInt16 length;          /* to indicate lenght */
    UInt8 value;           /* To indicate data */
}pdcpextFlagHeader;

/* INTRA_ENB_HO Fix End */

/* Mapping Maintained for tunnel id from Ue Index and lc Id */
/* + Coverity_20889_20888 */

/* Start of change for VARIBLE Declaration */
/* 255 UE changes start */
extern TunnelIdMap relayMapBasedOnUeIndexLcId_g [RELAY_GTPU_MAX_UE][RELAY_GTPU_MAX_LC];
/* 255 UE changes end */

/* Mapping Maintained for transaction identifier */
extern UeIndexLcIdTunnelIdMap relayMapBasedOnTransId_g[RELAY_GTPU_MAX_TRANS_ID];
/* Global Variable for maintaining configuration information */
extern relayConfigInfo relayConfigInfo_g;
/* End of change for VARIBLE Declaration */

/* - Coverity_20889_20888 */
/* LIPA changes start */
/* Mapping Maintained for transaction identifier for secondary tunnel */
extern UeIndexLcIdTunnelIdMap relayMapBasedOnTransIdForSecTunnel_g[RELAY_GTPU_MAX_TRANS_ID];
/* LIPA changes end */

/*Following Macros are applicable for both SETUP and RELEASE tunnel Cnf*/
#define RELAY_MANDATORY_PRESENT       0x01
#define RELAY_TUNNEL_ID_PRESENT       0x02
#define RELAY_GSN_ADDR_PRESENT        0x04
typedef struct relayTunnelInfoTag
{
    UInt8 bitMask;
    UInt8 cause;             /*Mandatory*/ 
    UInt16 rbDirection;      /*Mandatory*/
    UInt32 teidSelf;
    UInt32 teidPeer;         
    UInt8 addrLen;
    UInt8  transportAddr[RELAY_ADDR_LEN_IPV6];  /* To identify transport Address
                                      (Conditional, Present if teidPeer is present)*/
}relayTunnelInfo;

typedef struct relayRelTunnelInfoTag
{
    UInt8 bitMask;
    UInt32 teidSelf; /*Mandatory*/
    UInt8 addrLen;
    UInt8  transportAddr[RELAY_ADDR_LEN_IPV6];  /* To identify transport Address
                                      (Conditional, Present if teidPeer is present)*/
}relayRelTunnelInfo;

/* structure used for Configure Sap Conf between S1 and Relay Interface */

/*Following Macros are applicable for both SETUP and RELEASE*/
#define RELAY_MANDATORY_PRESENT       0x01
#define RELAY_SEQ_NO_PRESENT          0x02

typedef struct relaySetupCnfTag
{
    UInt8 bitMask;
    UInt8 lcId;                       /* To indicate the lcId (Mandatory) */
    UInt8 cause;                      /* To indicate the cause (Mandatory) */

    UInt8 tunnelCount;                /* To indicate number of tunnels in Resp */
    relayTunnelInfo tunnelInfo[MAX_NUM_TUNNEL_PER_LC];
    UInt16 seqNumUplink;          /* To indicate UL Sequence Number
                                         (Conditional present if cause contains success)*/
    UInt16 seqNumDnlink;          /* To indicate DL Sequence Number
                                         (Conditional, present if cause contains success)*/
}relaySetupCnf;

typedef struct relayReleaseCnfTag
{
    UInt8 bitMask;
    UInt8 lcId;                       /* To indicate the lcId (Mandatory) */
    UInt8 cause;                      /* To indicate the cause (Mandatory) */
    UInt8 tunnelCount;                /* To indicate number of tunnels in Resp */
    relayRelTunnelInfo relTunnelInfo[MAX_NUM_TUNNEL_PER_LC];
    UInt16 seqNumUplink;          /* To indicate UL Sequence Number
                                         (Conditional present if cause contains success)*/
    UInt16 seqNumDnlink;          /* To indicate DL Sequence Number
                                         (Conditional, present if cause contains success)*/
}relayReleaseCnf;

typedef struct relayS1ReconfigSapCnfTag
{
    UInt16 ueIndex;                 /* To identify UE uniquley (Mandatory)*/
    UInt16 transIdForRsp;                /* To indicate transaction identifier for response to S1AP*/
    UInt16 apiIdForRsp;             /* API Id for Response*/
    UInt8 lcCount;                  /* To indicate number of RABs in Req (Mandatory)*/
    RrcCellIndex  cellIndex;                      
    relaySetupCnf setupSapCnf[MAX_RAB_ID];  /* To identify setup confirmation array (Mandatory)*/
    relayReleaseCnf releaseSapCnf[MAX_RAB_ID]; /* To identify Release Confirmation*/
}relayS1ReconfigSapCnf;

/* Start of change for VARIBLE Declaration */
extern relayS1ReconfigSapCnf s1ReconfigSapCnf_g;
extern transIdMappingForLM transIdMappingForLM_g[RELAY_MAX_LM_API];
/* End of change for VARIBLE Declaration */

/********QOS+DSCP Changes **************************************************/

#define MAX_DSCP_VALUES 64
#define MAX_VALID_DSCP 21

typedef enum DscpClassT
{
    CS0 =  0, 
    CS1 = 8, 
    AF11 = 10, 
    AF12 = 12, 
    AF13 = 14, 
    CS2  = 16, 
    AF21 = 18,  
    AF22 = 20,  
    AF23 = 22,  
    CS3  = 24,  
    AF31 = 26,  
    AF32 = 28,  
    AF33 = 30,  
    CS4  = 32,  
    AF41 = 34,  
    AF42 = 36,  
    AF43 = 38,  
/* SPR 19288 change start */
    CS5  = 40,  
    EF   = 46,  
    CS6  = 48,  
    CS7  = 56  
/* SPR 19288 change end */ 
}dscpClass_en;

extern UInt8 Dscp[MAX_VALID_DSCP];


typedef struct relayDscpSockInfoTag
{
    /* SPR 14327 fix start */
    SInt32 sockfd; /* Socketfd for this dscp value*/
    /* SPR 14327 fix end */
}relayDscpSockInfo;

/* Start of change for VARIBLE Declaration */
extern relayDscpSockInfo relayDscpToSockFdMap_g[MAX_DSCP_VALUES];
/*IPV6_CHANGE - START*/
extern relayDscpSockInfo relayDscpToSockFdMap6_g[MAX_DSCP_VALUES];
/*IPV6_CHANGE - END*/
/* End of change for VARIBLE Declaration */


/****************************************************************************/
/****************************************************************************
 * Function Name  : lteRelayMsgHandler
 * Inputs         : buf, data
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion is the entry point of Packet Relay.
 ****************************************************************************/
/* +- SPR 17777 */
void lteRelayMsgHandler
(
    void *buf
);
/* +- SPR 17777 */

/******************************************************************************
 * Function Name  : initPacketRelayOamFunPtr
 * Inputs         : None
 * Outputs        : None
 * Returns        : None.
 * Description    : This funtion initalise function pointers for OAM Interface.
 *****************************************************************************/
void initPacketRelayOamFunPtr(void);

/******************************************************************************
 * Function Name  : initPacketRelayRrcFunPtr
 * Inputs         : None
 * Outputs        : None
 * Returns        : None.
 * Description    : This funtion initalise function pointers for RRC Interface.
 ******************************************************************************/

void initPacketRelayRrcFunPtr(void);

/****************************************************************************
 * Function Name  : initPacketRelayAdapterFunPtr
 * Inputs         : None
 * Outputs        : None
 * Returns        : None.
 * Description    : This funtion initalise function pointers for Adapter.
 ****************************************************************************/

void initPacketRelayAdapterFunPtr(void);


/****************************************************************************
 * Function Name  : initPrepareAndSendRspFunPtr
 * Inputs         : None
 * Outputs        : None
 * Returns        : None.
 * Description    : This funtion initalise function pointers for prepare and
 *                : send response to respective module.
 ****************************************************************************/
void initPrepareAndSendRspFunPtr(void);

/****************************************************************************
 * Function Name  : initPacketRelayGlobVariables
 * Inputs         : None
 * Outputs        : None
 * Returns        : None.
 * Description    : This funtion initalise all global variables.
 ****************************************************************************/
void initPacketRelayGlobVariables(void);
void lteRelaySockHandler(UInt8 *rcvBuffer,SInt32 bytesRead);
SInt32 recvFromRelaySockHandler( UInt8 *rcvBuffer, UInt32 rcvBuffer_len, 
        UInt32 moduleId, SockAddr *sockAddr );
/*handleGtpuRegistration func moved in gtpuMain.c */
/* Start of change for VARIBLE Declaration */
#ifdef LTE_EMBMS_SUPPORTED
void initPacketRelayEmbmsStructures(void);
#endif
extern SockAddr tgGlueAddress_g;
extern SInt32 glueTx_g;
/*IPV6_CHANGE - START*/
extern sockAddrIn6 tgGlueAddress6_g;
extern  SInt32 glueTx6_g;
/*IPV6_CHANGE - END*/
extern UInt32 g_transId;
/* LIPA changes start */
extern SInt32 secondaryTG_g;
extern SInt32 secondaryTG6_g;
extern SockAddr secondaryTGAddress_g;
extern sockAddrIn6 secondaryTGAddress6_g;
/* LIPA changes end */
/* End of change for VARIBLE Declaration */

#endif

