/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpRrcSap.h,v 1.1.1.1.6.2 2010/08/02 08:57:18 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : Definitions for Interface between the PDCP layer and 
 *                     RRC layer
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpRrcSap.h,v $
 * Revision 1.1.1.1.6.2  2010/08/02 08:57:18  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.1  2009/06/09 11:33:04  gur19140
 * *** empty log message ***
 *
 * Revision 1.1  2009/05/20 13:22:18  gur20548
 * Changed header file name
 *
 * Revision 1.3  2009/05/15 12:22:41  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef _LIB_I_RRC_SAP_DEF_
#define _LIB_I_RRC_SAP_DEF_

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
/* RRC API commands and responses list */
/* RRC API commands list */
#if defined(UE_SIM_TESTING) || defined(MAC_AUT_TEST)
enum RrcCmdListT
{
    PDCP_CREATE_UE_ENTITY_REQ  = 0,     /*0*/
    PDCP_RECONFIG_UE_ENTITY_REQ,        /*1*/
    PDCP_DELETE_UE_ENTITY_REQ,          /*2*/
    PDCP_SRB_DATA_REQ,                  /*3*/
    
    PDCP_RRC_LAST_REQ,
};

/* RRC API responses list */
enum RrcResponseListT
{
    PDCP_CREATE_UE_ENTITY_CNF = 0,       /*0*/
    PDCP_RECONFIG_UE_ENTITY_CNF,         /*1*/
    PDCP_DELETE_UE_ENTITY_CNF,           /*2*/
    PDCP_SRB_DATA_IND,                   /*3*/	
    PDCP_SRB_DATA_STATUS_IND,            /*4*/
    
    PDCP_RRC_LAST_CNF,
};
#else

#define     PDCP_CREATE_UE_ENTITY_CNF         0
#define     PDCP_RECONFIG_UE_ENTITY_CNF       1
#define     PDCP_DELETE_UE_ENTITY_CNF         2
#define     PDCP_SRB_DATA_IND                 3
#define     PDCP_SRB_DATA_STATUS_IND          4
#endif

#define	    PDCP_SN_HFN_STATUS_REQ	          5
#define	    PDCP_SN_HFN_STATUS_RESP	          6
#define	    PDCP_SN_HFN_STATUS_IND	          7
#define	    PDCP_DATA_BUFFER_STOP_IND    	  8
#define	    PDCP_MAC_I_REQ		              9
#define	    PDCP_MAC_I_RESP		             10
#define     PDCP_SUSPEND_UE_ENTITY_REQ       11
#define     PDCP_SUSPEND_UE_ENTITY_CNF 	     12
#define     PDCP_RE_ESTABLISH_UE_ENTITY_REQ  13
#define     PDCP_RE_ESTABLISH_UE_ENTITY_CNF  14
#define     PDCP_RESUME_UE_ENTITY_REQ        15
#define     PDCP_RESUME_UE_ENTITY_CNF        16
#define     PDCP_CHANGE_CRNTI_REQ            17
#define     PDCP_CHANGE_CRNTI_CNF            18
/* SPR 3607 Changes Start */
#define     PDCP_COUNT_WRAPAROUND_IND        19
/* SPR 3607 Changes End */
/* SPR 3175 Changes Start */
#define     PDCP_NOTIFY_INTEGRITY_FAILURE    20
/* SPR 3175 Changes End */
#define 	PDCP_DRB_COUNT_MSB_REQ           21
#define 	PDCP_DRB_COUNT_MSB_RESP          22

#ifndef UE_SIM_TESTING
#define     PDCP_CREATE_UE_ENTITY_REQ        23
#define     PDCP_RECONFIG_UE_ENTITY_REQ      24
#define     PDCP_DELETE_UE_ENTITY_REQ        25
#define     PDCP_SRB_DATA_REQ                26
#endif
#define     PDCP_UPDATE_LWA_TRAFFIC_IND      27
#define     PDCP_UPDATE_TRAFFIC_IND          27
#define     PDCP_CELL_CONFIG_REQ             28 
#define     PDCP_CELL_CONFIG_RESP            29
/* + SPR 19066 */
#define     PDCP_HO_PREP_INFO_REQ            30 
#define     PDCP_HO_PREP_INFO_RESP           31
#define     PDCP_INACTIVE_UES_IND            32
/* - SPR 19066 */

/* RRC API TAGs list for commands */
#ifdef ENDC_ENABLED
typedef enum DataFlowControlStatusT
{
	STOP_DATA_TRAFFIC = 0,
	START_DATA_TRAFFIC
}DataFlowControlStatus;
#endif

enum RrcCmdTagsListT
{
    PDCP_CREATE_SRB_ENTITY = 0,              /*0*/
    PDCP_CREATE_DRB_ENTITY,                  /*1*/
    PDCP_CONFIGURE_INTEGRITY_PROTECTION,     /*2*/
    PDCP_CONFIGURE_DISCARD_TIMER,            /*3*/
    PDCP_CONFIGURE_SN_SIZE,                  /*4*/
    PDCP_CONFIGURE_ROHC,                     /*5*/
    /* SPR_1235_FIX_START*/
    PDCP_CONFIGURE_SRB_CIPHERING,            /*6*/
    /* SPR_1235_FIX_END*/
    PDCP_CONFIGURE_STATUS_REPORT_REQUIRED,   /*7*/
    PDCP_DELETE_SRB_ENTITY,                  /*8*/
    PDCP_DELETE_DRB_ENTITY,                  /*9*/
    PDCP_RECONFIG_SRB_ENTITY,                /*10*/
    PDCP_RECONFIG_DRB_ENTITY,                /*11*/
    /* SPR_1235_FIX_START*/
    PDCP_CONFIGURE_DRB_CIPHERING,            /*12*/
    /* SPR_1235_FIX_END*/
    PDCP_HO_TRIGGERED,                       /*13*/
    PDCP_LC_SN_HFN_STATUS,                   /*14*/
    PDCP_UL_RECV_SN,                         /*15*/
    PDCP_MESSAGE,                            /*16*/
    PDCP_MSG_AUTHENTICATION_CODE,            /*17*/
    PDCP_RB_ENTITY,                          /*18*/
    /* SPR 3608 changes start */
    PDCP_CONFIGURE_UE_PSR,	             /*19*/
    /* SPR 3608 changes end */
    /* SPR 3607 fix start */
    PDCP_SECURITY_OFFSET,	             /*20*/
    /* SPR 3607 fix end */
    PDCP_SPS_CONFIG,                         /*21*/
    PDCP_SPS_DL_INFO,                        /*22*/
    PDCP_SPS_DL_SETUP_INFO,                  /*23*/
    PDCP_SPS_UL_INFO,                        /*24*/
    PDCP_SPS_UL_SETUP_INFO,                  /*25*/
    PDCP_DRB_COUNT_MSB_LIST,                 /*26*/
    PDCP_DRB_COUNT_MSB_INFO,                 /*27*/
    PDCP_DRB_COUNT_MSB_UPLINK,               /*28*/
    PDCP_DRB_COUNT_MSB_DOWNLINK,             /*29*/
   PDCP_LOSSY_HO_REQUIRED,                   /*30*/
   /* SPR 17157 Fix start */
   RRC_PDCP_MACI_REQ_TAG,                            /*31*/
   /* SPR 17157 Fix end */
    PDCP_CONFIGURE_LWA_BEARER,               /*32*/
    PDCP_CONFIGURE_LWA_BEARER_TYPE,          /*33*/
     /* + SPR 19066 */
    RRC_PDCP_UE_INACTIVITY_TIMER_CONFIG_TAG, /*34*/
    RRC_PDCP_UE_INACTIVE_TIME_LAPSED_TAG,    /*35*/
    RRC_PDCP_UE_INACTIVE_TIME_REQ_TAG,       /*36*/
    RRC_PDCP_MACI_HO_REQ_TAG,                /*37*/
    /* - SPR 19066 */ 
    PDCP_CMD_TAG_LAST,
};

/* RRC API TAGs list for responses */
enum RrcResponseTagsListT
{
    PDCP_CREATE_SRB_ENTITY_ERROR = 0,        /*0*/
    PDCP_CREATE_DRB_ENTITY_ERROR,            /*1*/
    PDCP_DELETE_SRB_ENTITY_ERROR,            /*2*/
    PDCP_DELETE_DRB_ENTITY_ERROR,            /*3*/
    PDCP_RECONFIG_SRB_ENTITY_ERROR,          /*4*/
    PDCP_RECONFIG_DRB_ENTITY_ERROR,          /*5*/
    PDCP_SRB_DATA_STATUS_ERROR,              /*6*/
    PDCP_RB_ENTITY_ERROR,                    /*7*/
    /* SPR 17157 Fix start */
    RRC_PDCP_MACI_RESP_TAG,							 /*8*/
    /* SPR 17157 Fix end */
   /* + SPR 19066 */
    RRC_PDCP_UE_INACTIVE_TIME_RESP_TAG,      /*9*/
    RRC_PDCP_INACTIVE_UE_INFO_TAG,           /*10*/ 
    /* - SPR 19066 */
    PDCP_RESPONSE_TAG_LAST,
};

/*Minimum API REQ message sizes */
#define PDCP_RRC_CREATE_UE_ENTITY_REQ_MIN_SIZE       ( sizeof(TlvHeader) + sizeof(RrcCreateUeEntityReq) )       
#define PDCP_RRC_RECONFIG_UE_ENTITY_REQ_MIN_SIZE     ( sizeof(TlvHeader) + sizeof(RrcReconfigUeEntityReq) )       
#define PDCP_RRC_DELETE_UE_ENTITY_REQ_MIN_SIZE       ( sizeof(TlvHeader) + sizeof(RrcDeleteUeEntityReq) )       
#define PDCP_RRC_PDCP_SRB_DATA_REQ_MIN_SIZE          ( sizeof(TlvHeader) + sizeof(RrcSrbDataReq) )
#define PDCP_RRC_PDCP_SN_HFN_STATUS_REQ_MIN_SIZE ( sizeof(TlvHeader) + \
                                                 sizeof (RrcSnHfnStatusReq) )
#define PDCP_RRC_PDCP_MAC_I_REQ_MIN_SIZE ( sizeof(TlvHeader) + sizeof (RrcMacIReq) )
/* SPR 6711 fix start */
#define PDCP_RRC_PDCP_SN_HFN_STATUS_IND_MIN_SIZE ( sizeof(TlvHeader) + \
                                         sizeof (RrcSnHfnStatusInd) )
/* SPR 6711 fix end */
#define PDCP_SUSPEND_UE_ENTITY_REQ_MIN_SIZE ( sizeof(TlvHeader) + \
                                           sizeof (RrcSuspendUeEntityReq) )
#define PDCP_RESUME_UE_ENTITY_REQ_MIN_SIZE  ( sizeof(TlvHeader) + \
                                           sizeof (RrcResumeUeEntityReq) )
#define PDCP_RE_ESTABLISH_UE_ENTITY_REQ_MIN_SIZE ( sizeof(TlvHeader) + \
                                           sizeof (RrcReestablishUeEntityReq) )
#define PDCP_DATA_BUFFER_STOP_IND_MIN_SIZE ( sizeof(TlvHeader) + \
                                           sizeof(RrcDataBufferStopInd) )
/* SPR 3607 Changes Start */
#define PDCP_RRC_PDCP_COUNT_WRAPAROUND_REQ_MIN_SIZE ( sizeof(TlvHeader) + \
                                          sizeof (RrcCountWraparoundReq) )
/* SPR 3607 Changes End */
/* SPR 3175 Changes Start */
#define PDCP_RRC_PDCP_NOTIFY_INTEGRITY_FAILURE_REQ_MIN_SIZE ( \
                   sizeof(TlvHeader) + sizeof (RrcNotifyIntegrityFailureInd) )
/* SPR 3175 Changes End */

/*COUNTER_CHK_START*/
#define PDCP_RRC_PDCP_DRB_COUNT_MSB_REQ_MIN_SIZE ( sizeof(TlvHeader) + \
                                                 sizeof (RrcDrbCountMsbReq) )
/*COUNTER_CHK_END*/
/* + SPR 19066 */
#define PDCP_RRC_HO_PREP_INFO_REQ_MIN_SIZE ( sizeof(TlvHeader) + \
        sizeof(RrcHoPrepInfoReq) )
/* - SPR 19066 */ 
/*Maximum API CNF message size */
#define PDCP_RRC_CNF_MAX_SIZE       ( 1024 )       

/* Supported ROHC profiles */
enum RrcRohcProfilesT
{
    profile0x0001 = 0,
    profile0x0002,
    profile0x0003,
    profile0x0004,
    profile0x0006,
    profile0x0101,
    profile0x0102,
    profile0x0103,
    profile0x0104,
    
    profileLast,
};

/* + SPR 19066 */
/* For UE Inactivity Timer */
enum RrcPdcpUeInactiveTimeT
{
    UE_INACTIVITY_TIME_S1,
    UE_INACTIVITY_TIME_S2,
    UE_INACTIVITY_TIME_S3,
    UE_INACTIVITY_TIME_S5,
    UE_INACTIVITY_TIME_S7,
    UE_INACTIVITY_TIME_S10,
    UE_INACTIVITY_TIME_S15,
    UE_INACTIVITY_TIME_S20,
    UE_INACTIVITY_TIME_S25,
    UE_INACTIVITY_TIME_S30,
    UE_INACTIVITY_TIME_S40,
    UE_INACTIVITY_TIME_S50,
    UE_INACTIVITY_TIME_MIN1,
    UE_INACTIVITY_TIME_MIN1S20,
    UE_INACTIVITY_TIME_MIN1S40,
    UE_INACTIVITY_TIME_MIN2,
    UE_INACTIVITY_TIME_MIN2S30,
    UE_INACTIVITY_TIME_MIN3,
    UE_INACTIVITY_TIME_MIN3S30,
    UE_INACTIVITY_TIME_MIN4,
    UE_INACTIVITY_TIME_MIN5,
    UE_INACTIVITY_TIME_MIN6,
    UE_INACTIVITY_TIME_MIN7,
    UE_INACTIVITY_TIME_MIN8,
    UE_INACTIVITY_TIME_MIN9,
    UE_INACTIVITY_TIME_MIN10,
    UE_INACTIVITY_TIME_MIN12,
    UE_INACTIVITY_TIME_MIN14,
    UE_INACTIVITY_TIME_MIN17,
    UE_INACTIVITY_TIME_MIN20,
    UE_INACTIVITY_TIME_MIN24,
    UE_INACTIVITY_TIME_MIN28,
    UE_INACTIVITY_TIME_MIN33,
    UE_INACTIVITY_TIME_MIN38,
    UE_INACTIVITY_TIME_MIN44,
    UE_INACTIVITY_TIME_MIN50,
    UE_INACTIVITY_TIME_HR1,
    UE_INACTIVITY_TIME_HR1MIN30,
    UE_INACTIVITY_TIME_HR2,
    UE_INACTIVITY_TIME_HR2MIN30,
    UE_INACTIVITY_TIME_HR3,
    UE_INACTIVITY_TIME_HR3MIN30,
    UE_INACTIVITY_TIME_HR4,
    UE_INACTIVITY_TIME_HR5,
    UE_INACTIVITY_TIME_HR6,
    UE_INACTIVITY_TIME_HR8,
    UE_INACTIVITY_TIME_HR10,
    UE_INACTIVITY_TIME_HR13,
    UE_INACTIVITY_TIME_HR16,
    UE_INACTIVITY_TIME_HR20,
    UE_INACTIVITY_TIME_DAY1,
    UE_INACTIVITY_TIME_DAY1HR12,
    UE_INACTIVITY_TIME_DAY2,
    UE_INACTIVITY_TIME_DAY2HR12,
    UE_INACTIVITY_TIME_DAY3,
    UE_INACTIVITY_TIME_DAY4,
    UE_INACTIVITY_TIME_DAY5,
    UE_INACTIVITY_TIME_DAY7,
    UE_INACTIVITY_TIME_DAY10,
    UE_INACTIVITY_TIME_DAY14,
    UE_INACTIVITY_TIME_DAY19,
    UE_INACTIVITY_TIME_DAY24,
    UE_INACTIVITY_TIME_DAY30,
    UE_INACTIVITY_TIME_DAYMORETHAN30,

    MAX_UE_INACTIVITY_TIME
} RrcPdcpUeInactiveTime;

#define PDCP_MAX_UE_AUDIT_REPORT            10
/* - SPR 19066 */
/* SPR 3608 changes start */
#define PDCP_STATUS_REPORT_TIMER_VALUE      100
/* SPR 3608 changes end */
/* Start of change for VARIBLE Declaration */
/* End of change for VARIBLE Declaration */
/****************************************************************************
 * Exported Types
 ****************************************************************************/
/****************************************************************************
    APIs structure definitions 
 ****************************************************************************/

#define ATTRIBUTE_PACKED  __attribute__ ((packed))

/* Mandatory parameters structure definition for any RRC APIs */
/* Klocworks fix */
struct RrcHeaderT
{
    UInt16      ueIndex;    /* UE identifier */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcHeaderT RrcHeader, *LP_RrcHeader;

/* PDCP_CREATE_UE_ENTITY_REQ API - Mandatory parameters structure definition*/
struct RrcCreateUeEntityReqT
{
    RrcHeader   header;             /* General RRC API header */
    UInt16      crnti;              /* unique identification of UE */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcCreateUeEntityReqT RrcCreateUeEntityReq, *LP_RrcCreateUeEntityReq;

/* PDCP_RECONFIG_UE_ENTITY_REQ API - Mandatory parameters structure definition*/
struct RrcReconfigUeEntityReqT
{
    RrcHeader   header;             /* General RRC API header */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcReconfigUeEntityReqT RrcReconfigUeEntityReq, *LP_RrcReconfigUeEntityReq;

/* PDCP_DELETE_UE_ENTITY_REQ API - Mandatory parameters structure definition*/
struct RrcDeleteUeEntityReqT
{
    RrcHeader   header;             /* General RRC API header */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcDeleteUeEntityReqT RrcDeleteUeEntityReq, *LP_RrcDeleteUeEntityReq;

/* PDCP_SRB_DATA_REQ API - Mandatory parameters structure definition*/
struct RrcSrbDataReqT
{
    UInt32      transactionId;      /* transactionId - Transaction Id. */
    UInt16      ueIndex;            /* UE identifier */
    UInt8       lcId;               /* logical channel identifier */
    UInt8       serviceReqested;    /* type of service requested. i.e. whether success or failure response is expected or not */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcSrbDataReqT RrcSrbDataReq, *LP_RrcSrbDataReq;

/* PDCP_SUSPEND_UE_ENTITY_REQ API - Mandatory parameters structure definition*/
struct RrcSuspendUeEntityReqT
{
    RrcHeader   header;             /* General RRC API header */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcSuspendUeEntityReqT RrcSuspendUeEntityReq, *LP_RrcSuspendUeEntityReq;

/* PDCP_SN_HFN_STATUS_REQ API - Manadatory parameters structure definition*/
struct RrcSnHfnStatusReqT
{
    RrcHeader   header;             /* General RRC API header */
    UInt8     sn_hfn_required;
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcSnHfnStatusReqT RrcSnHfnStatusReq, *LP_RrcSnHfnStatusReq;

/* PDCP_SN_HFN_STATUS_IND API - Manadatory parameters structure definition*/
struct RrcSnHfnStatusIndT
{
    RrcHeader   header;             /* General RRC API header */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcSnHfnStatusIndT RrcSnHfnStatusInd, *LP_RrcSnHfnStatusInd;

/* PDCP_DATA_BUFFER_STOP_IND API - Manadatory parameters structure definition*/
struct RrcDataBufferStopIndT
{
    RrcHeader   header;             /* General RRC API header */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcDataBufferStopIndT RrcDataBufferStopInd, *LP_RrcDataBufferStopInd;

/* PDCP_MAC_I_REQ API - Manadatory parameters structure definition*/
struct RrcMacIReqT
{
    RrcHeader   header;             /* General RRC API header */
    UInt8       lcId;               /* logical channel identifier */
    UInt16      rbDirection;
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcMacIReqT RrcMacIReq, *LP_RrcMacIReq;
 
/* PDCP_RE_ESTABLISH_UE_ENTITY_REQ API - Mandatory parameters structure definition*/
struct RrcReestablishUeEntityReqT
{
    RrcHeader   header;             /* General RRC API header */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcReestablishUeEntityReqT RrcReestablishUeEntityReq, *LP_RrcReestablishUeEntityReq;

/* PDCP_RESUME_UE_ENTITY_REQ API - Mandatory parameters structure definition*/
struct RrcResumeUeEntityReqT
{
    RrcHeader   header;             /* General RRC API header */
    UInt16      resumeDirection;    /* in which direction (either uplink or downlink or both) */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcResumeUeEntityReqT RrcResumeUeEntityReq, *LP_RrcResumeUeEntityReq;
    
/* PDCP_CHANGE_CRNTI_REQ API - Mandatory parameters structure definition*/
struct RrcChangeCrntiReqT
{
    RrcHeader   header;             /* General RRC API header */
    UInt16      oldCrnti;
    UInt16      newCrnti;
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcChangeCrntiReqT RrcChangeCrntiReq, *LP_RrcChangeCrntiReq;

/* PDCP_CREATE_UE_ENTITY_CNF API - Mandatory parameters structure definition*/
struct RrcCreateUeCnfT
{
    RrcHeader   header;             /* General RRC API header */
    UInt16      responseCode;       /* response code */ 
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcCreateUeCnfT RrcCreateUeCnf, *LP_RrcCreateUeCnf;

/* PDCP_RECONFIG_UE_ENTITY_CNF API - Mandatory parameters structure definition*/
struct RrcReconfigUeCnfT
{
    RrcHeader   header;             /* General RRC API header */
    UInt16      responseCode;       /* response code */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcReconfigUeCnfT RrcReconfigUeCnf, *LP_RrcReconfigUeCnf;

/* PDCP_DELETE_UE_ENTITY_CNF API - Mandatory parameters structure definition*/
struct RrcDeleteUeCnfT
{
    RrcHeader   header;             /* General RRC API header */
    UInt16      responseCode;       /* response code */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcDeleteUeCnfT RrcDeleteUeCnf, *LP_RrcDeleteUeCnf;

/* PDCP_SRB_DATA_IND API - Mandatory parameters structure definition*/
struct RrcSrbDataIndT
{
    RrcHeader   header;             /* General RRC API header */
    UInt8       lcId;               /* logical channel identifier */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcSrbDataIndT RrcSrbDataInd, *LP_RrcSrbDataInd;

/* PDCP_SRB_DATA_STATUS_IND API - Mandatory parameters structure definition*/
struct RrcSrbDataStatusStatusIndT
{
    UInt32      transactionId;      /* transactionId - Transaction Id. */
    UInt16      ueIndex;            /* UE identifier */
    UInt8       lcId;               /* logical channel identifier */
    UInt16      responseCode;       /* response code */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct  RrcSrbDataStatusStatusIndT RrcSrbDataStatusStatusInd, *LP_RrcSrbDataStatusStatusInd;

/* PDCP_SN_HFN_STATUS_RESP API - Mandatory parameters structure definition*/
struct RrcSnHfnStatusRespT
{
    UInt16      ueIndex;            /* UE identifier */
    UInt16      responseCode;       /* Either SUCCESS or FAILURE */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcSnHfnStatusRespT RrcSnHfnStatusResp, *LP_RrcSnHfnStatusResp;

/* PDCP_MAC_I_RESP API - Mandatory parameters structure definition*/
struct RrcMacIRespT
{
    RrcHeader   header;             /* General RRC API header */
    UInt16      responseCode;       /* Either SUCCESS or FAILURE */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcMacIRespT RrcMacIResp, *LP_RrcMacIResp;

/* PDCP_SUSPEND_UE_ENTITY_CNF API - Mandatory parameters structure definition*/
struct RrcSuspendUeEntityCnfT
{
    RrcHeader   header;             /* General RRC API header */
    UInt16      responseCode;       /* Either SUCCESS or FAILURE */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcSuspendUeEntityCnfT RrcSuspendUeEntityCnf, *LP_RrcSuspendUeEntityCnf;

/* PDCP_RE_ESTABLISH_UE_ENTITY_CNF API - Mandatory parameters structure definition*/
struct RrcReestablishUeEntityCnfT
{
    RrcHeader   header;             /* General RRC API header */
    UInt16      responseCode;       /* Either SUCCESS or FAILURE */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcReestablishUeEntityCnfT RrcReestablishUeEntityCnf, *LP_RrcReestablishUeEntityCnf;

/* PDCP_RESUME_UE_ENTITY_CNF API - Mandatory parameters structure definition*/
struct RrcResumeUeEntityCnfT
{
    RrcHeader   header;             /* General RRC API header */
    UInt16      responseCode;       /* Either SUCCESS or FAILURE */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcResumeUeEntityCnfT RrcResumeUeEntityCnf, *LP_RrcResumeUeEntityCnf;

/* PDCP_CHANGE_CRNTI_CNF API - Mandatory parameters structure definition*/
struct RrcChangeCrntiCnfT
{
    RrcHeader   header;             /* General RRC API header */
    UInt16      responseCode;       /* Either SUCCESS or FAILURE */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcChangeCrntiCnfT RrcChangeCrntiCnf, *LP_RrcChangeCrntiCnf;  

/* SPR 3607 Changes Start */
/* PDCP_COUNT_WRAPAROUND_IND API - Mandatory parameters structure definition*/
struct RrcCountWraparoundIndT
{
    RrcHeader   header;             /* General RRC API header */
}ATTRIBUTE_PACKED;
/* SPR 3607 Changes End */
typedef struct RrcCountWraparoundIndT RrcCountWraparoundInd, *LP_RrcCountWraparoundInd;

/* SPR 3175 Changes Start */
/* PDCP_NOTIFY_INTEGRITY_FAILURE_IND API - Mandatory parameters structure definition*/
struct RrcNotifyIntegrityFailureIndT
{
    RrcHeader   header;             /* General RRC API header */
    UInt8       lcId;               /* logical channel identifier */
}ATTRIBUTE_PACKED;
/* SPR 3175 Changes End */
typedef struct RrcNotifyIntegrityFailureIndT RrcNotifyIntegrityFailureInd, *LP_RrcNotifyIntegrityFailureInd;

/*COUNTER_CHK_START*/
/* PDCP_DRB_COUNT_MSB_REQ API - Manadatory parameters structure definition*/
struct RrcDrbCountMsbReqT
{
    UInt16      ueIndex;            /* UE identifier */
}ATTRIBUTE_PACKED ;

typedef struct RrcDrbCountMsbReqT RrcDrbCountMsbReq, *LP_RrcDrbCountMsbReq;
/* PDCP_DRB_COUNT_MSB_RESP API - Mandatory parameters structure definition*/
struct RrcDrbCountMsbRespT
{
    UInt16      ueIndex;            /* UE identifier */
    UInt16      responseCode;       /* Either SUCCESS or FAILURE */
}ATTRIBUTE_PACKED;
typedef struct RrcDrbCountMsbRespT RrcDrbCountMsbResp, *LP_RrcDrbCountMsbResp;
/*COUNTER_CHK_END*/
        
/****************************************************************************
    Tags structure definitions 
 ****************************************************************************/

/* PDCP_CREATE_SRB_ENTITY API tag - Mandatory parameters structure definition*/
struct RrcTagCreateSrbEntityT
{
    /* SPR 3276 changes start */
    UInt8       srbId;           /* SRB identifier */
    /* SPR 3276 changes end */
    UInt16      rlcMode;        /* RLC link mode (UM, AM) */
    UInt16      rbDirection;    /* Radio bearer link type (TX, RX, Both) */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcTagCreateSrbEntityT RrcTagCreateSrbEntity, *LP_RrcTagCreateSrbEntity;

/* PDCP_CREATE_DRB_ENTITY API tag - Mandatory parameters structure definition*/
struct RrcTagCreateDrbEntityT
{
    UInt8       lcId;           /* logical channel identifier */
    /* SPR 3276 changes start */
    UInt8       radioBearer;    /* Radio Bearer Id */
    /* SPR 3276 changes start */
    /* BYTE_ALLIGNMENT_FIX START */
#ifndef UE_SIM_TESTING
    UInt8       qci;            /* QCI value 1 to 9 */ //vijay
#endif    
    /* BYTE_ALLIGNMENT_FIX END */
    UInt16      rlcMode;        /* RLC link mode (UM, AM) */
    UInt16      rbDirection;    /* Radio bearer link type (TX, RX, Both) */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcTagCreateDrbEntityT RrcTagCreateDrbEntity, *LP_RrcTagCreateDrbEntity;

/* PDCP_RECONFIG_SRB_ENTITY API tag - Mandatory parameters structure definition*/
struct RrcTagReconfigSrbEntityT
{
    UInt8       lcId;           /* logical channel identifier */
    UInt16      rbDirection;    /* Radio bearer link type (TX, RX, Both) */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcTagReconfigSrbEntityT RrcTagReconfigSrbEntity, *LP_RrcTagReconfigSrbEntity;

/* PDCP_RECONFIG_DRB_ENTITY API tag - Mandatory parameters structure definition*/
struct RrcTagReconfigDrbEntityT
{
    UInt8       lcId;           /* logical channel identifier */
    UInt16      rbDirection;    /* Radio bearer link type (TX, RX, Both) */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcTagReconfigDrbEntityT RrcTagReconfigDrbEntity, *LP_RrcTagReconfigDrbEntity;

/* PDCP_DELETE_SRB_ENTITY,
   PDCP_DELETE_DRB_ENTITY API tags - Mandatory parameters structure definition*/
struct RrcTagDeleteEntityT
{
    UInt8       lcId;           /* logical channel identifier */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcTagDeleteEntityT RrcTagDeleteEntity, *LP_RrcTagDeleteEntity;

/* PDCP_CONFIGURE_INTEGRITY_PROTECTION API tag - Mandatory parameters structure definition*/
struct RrcTagConfigureIntegrityT
{
    /* SPR 6551 Fix Start */
    UInt16       AlgorithmId;                       /* Integrity protection algorithm id (0 = disabled) */
    UInt8        Key[LTE_SECURITY_KEY_LENGTH];      /* RLC integrity key */
    /* SPR 6551 Fix End */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcTagConfigureIntegrityT RrcTagConfigureIntegrity, *LP_RrcTagConfigureIntegrity;

/* PDCP_CONFIGURE_CIPHERING API tag - Mandatory parameters structure definition*/
struct RrcTagConfigureCipheringT
{
    /* SPR 6551 Fix Start */
    UInt16       AlgorithmId;                       /* Ciphering  algorithm id (0 = disabled) */
    UInt8        Key[LTE_SECURITY_KEY_LENGTH];      /* RLC Ciphering key */
    /* SPR 6551 Fix End */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcTagConfigureCipheringT RrcTagConfigureCiphering, *LP_RrcTagConfigureCiphering;

struct RrcTagHoTriggeredT
{
    UInt8 hoTriggered;
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcTagHoTriggeredT  RrcTagHoTriggered, *LP_RrcTagHoTriggered;

/* SPR 3608 changes start */
struct RrcTagConfigureUePsrT
{
    UInt8 configureUePsr;
}ATTRIBUTE_PACKED;
/* SPR 3608 changes end */
typedef struct RrcTagConfigureUePsrT RrcTagConfigureUePsr, *LP_RrcTagConfigureUePsr;

#ifdef ENDC_ENABLED
struct RrcUpdateTrafficIndT
{
    RrcHeader   header;             /* General RRC API header */
    UInt8       lcId;               /* logical channel identifier */
    UInt16      traffic_ind;        /*STOP_TRAFFIC = 1,START_TRAFFIC*/
}ATTRIBUTE_PACKED;
typedef struct RrcUpdateTrafficIndT RrcUpdateTrafficInd, *LP_RrcUpdateTrafficInd;
#define PDCP_UPDATE_TRAFFIC_IND_MIN_SIZE ( sizeof(TlvHeader) + \
                                           sizeof(RrcUpdateTrafficInd) )
#endif


/* SPR 3607 fix start */
struct RrcTagSecurityOffsetT
{
    UInt8 securityOffset;
}ATTRIBUTE_PACKED ;
/* SPR 3607 fix end */
typedef struct RrcTagSecurityOffsetT RrcTagSecurityOffset, *LP_RrcTagSecurityOffset;

/* PDCP_CONFIGURE_DISCARD_TIMER API tag - Mandatory parameters structure definition*/
struct RrcTagConfigureDiscardTimerT
{
    UInt16       discardTimer;                       /* Discard Timer value */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcTagConfigureDiscardTimerT RrcTagConfigureDiscardTimer, *LP_RrcTagConfigureDiscardTimer;

/* PDCP_CONFIGURE_SN_SIZE API tag - Mandatory parameters structure definition*/
struct RrcTagConfigureSnSizeT
{
    UInt8       snSize;                       /* SN size in bits */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcTagConfigureSnSizeT RrcTagConfigureSnSize, *LP_RrcTagConfigureSnSize;

/* PDCP_CONFIGURE_STATUS_REPORT_REQUIRED API tag - Mandatory parameters structure definition*/
struct RrcTagConfigureStatusReportRequiredT
{
    UInt8       statusReportRequired;                       /* Allow status report sending (for AM RX DRB Only )*/
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcTagConfigureStatusReportRequiredT
          RrcTagConfigureStatusReportRequired, *LP_RrcTagConfigureStatusReportRequired;

/* PDCP_CONFIGURE_ROHC API tag - Mandatory parameters structure definition*/
struct RrcTagConfigureRohcT
{
    UInt16	profileIDsMask;	/* ROHC compression profile identifiers */
    UInt16	maxCid;			/* Highest context ID number to be used by the compressor. */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcTagConfigureRohcT RrcTagConfigureRohc, *LP_RrcTagConfigureRohc;

/* PDCP_CONFIGURE_SN_SIZE API tag - Mandatory parameters structure definition*/
struct RrcTagQciT
{
    UInt8       qci;                       /* Qci size in bits */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;

/* SPR 1757 changes end */
typedef struct RrcTagQciT RrcTagQci, *LP_RrcTagQci;
/* PDCP_CREATE_DRB_ENTITY_ERROR,
   PDCP_CREATE_SRB_ENTITY_ERROR,
   PDCP_DELETE_SRB_ENTITY_ERROR,
   PDCP_DELETE_DRB_ENTITY_ERROR API tag - Mandatory parameters structure definition*/
struct RrcTagEntityErrorT
{
    UInt8       lcId;           /* logical channel identifier */
    UInt16      Response;       /* tag processing result code */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcTagEntityErrorT RrcTagEntityError, *LP_RrcTagEntityError;

/* PDCP_SRB_DATA_STATUS_ERROR API tag - Mandatory parameters structure definition*/
struct RrcTagSrbDataStatusErrorT
{
    UInt16      Response;       /* tag processing result code */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcTagSrbDataStatusErrorT RrcTagSrbDataStatusError, *LP_RrcTagSrbDataStatusError;

/* PDCP_MESSAGE tag - Manadatory parameters structure definition*/
struct RrcMessageMacIT
{
    UInt8   cellIdentity[MACI_LEN];
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcMessageMacIT RrcMessageMacI, *LP_RrcMessageMacI;

/* PDCP_MSG_AUTHENTICATION_CODE response tag - Manadatory parameters structure
   definition*/
struct RrcMacIResponseT
{
    UInt8   cellIdentity[MACI_LEN];
    UInt16  Response;       /* tag processing result code */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
/* SPR 1757 changes end */
typedef struct RrcMacIResponseT RrcMacIResponse, *LP_RrcMacIResponse;

struct RrcTagSnHfnStatusT
{
    UInt8   lcId;
    UInt16  dlSnCnt;
    UInt32  dlHfnCnt;
    UInt16  ulSnCnt;
    UInt32  ulHfnCnt;
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcTagSnHfnStatusT RrcTagSnHfnStatus, *LP_RrcTagSnHfnStatus;

struct RrcTagUlRecvSnT
{
/* SPR 3574 Changes Start */
    UInt8   ulRecvSn[BITMAP_SIZE];
/* SPR 3574 Changes end */
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcTagUlRecvSnT RrcTagUlRecvSn, *LP_RrcTagUlRecvSn;

/* PDCP_RB_ENTITY tag - Manadatory parameters structure definition*/
struct RrcTagRbEntityT
{
    UInt8 lcId;
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcTagRbEntityT RrcTagRbEntity, *LP_RrcTagRbEntity;

/* PDCP_RB_ENTITY_ERROR tag - Manadatory parameters structure definition*/
struct RrcTagRbEntityErrorT
{
    UInt8    lcid;
    UInt8    response;
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED;
/* SPR 1757 changes end */
typedef struct RrcTagRbEntityErrorT RrcTagRbEntityError, *LP_RrcTagRbEntityError;
   
/* PDCP_SPS_DL_INFO tag - Manadatory parameters structure definition */
struct RrcTagSpsDlInfoT
{
    UInt8    requestType;
}ATTRIBUTE_PACKED;

typedef struct RrcTagSpsDlInfoT RrcTagSpsDlInfo, *LP_RrcTagSpsDlInfo;
/* PDCP_SPS_DL_SETUP_INFO tag - Manadatory parameters structure definition */
struct RrcTagSpsDlSetupInfoT
{
    UInt8    semiPersistentSchedIntervalDl;
}ATTRIBUTE_PACKED;
typedef struct RrcTagSpsDlSetupInfoT RrcTagSpsDlSetupInfo, *LP_RrcTagSpsDlSetupInfo;

/* PDCP_SPS_UL_INFO tag - Manadatory parameters structure definition */
struct RrcTagSpsUlInfoT
{
    UInt8    requestType;
}ATTRIBUTE_PACKED;
typedef struct RrcTagSpsUlInfoT RrcTagSpsUlInfo, *LP_RrcTagSpsUlInfo;

/* PDCP_SPS_UL_SETUP_INFO tag - Manadatory parameters structure definition */
struct RrcTagSpsUlSetupInfoT
{
    UInt8    semiPersistentSchedIntervalUl;
}ATTRIBUTE_PACKED;

typedef struct  RrcTagSpsUlSetupInfoT RrcTagSpsUlSetupInfo, *LP_RrcTagSpsUlSetupInfo;
/*COUNTER_CHK_START*/
/* PDCP_DRB_COUNT_MSB_LIST tag - Mandatory parameters structure definition*/
struct RrcTagDrbCountMsbListT
{
    UInt8       lcId;           /* logical channel identifier */
    UInt16      rbDirection;    /* Radio bearer link type (TX, RX, Both) */
}ATTRIBUTE_PACKED ;
typedef struct RrcTagDrbCountMsbListT RrcTagDrbCountMsbList, *LP_RrcTagDrbCountMsbList;

/* PDCP_DRB_COUNT_MSB_INFO tag - Mandatory parameters structure definition*/
struct RrcTagDrbCountMsbInfoT
{
    UInt8       lcId;           /* logical channel identifier */
}ATTRIBUTE_PACKED ;
typedef struct RrcTagDrbCountMsbInfoT RrcTagDrbCountMsbInfo, *LP_RrcTagDrbCountMsbInfo;

/* PDCP_DRB_COUNT_MSB_UPLINK tag - Mandatory parameters structure definition*/
struct RrcTagDrbCountMsbUpLinkT
{
    UInt32      countMsb;           /* count MSB for Up Link */
}ATTRIBUTE_PACKED ;
typedef struct RrcTagDrbCountMsbUpLinkT RrcTagDrbCountMsbUpLink, *LP_RrcTagDrbCountMsbUpLink;

/* PDCP_DRB_COUNT_MSB_DOWNLINK tag - Mandatory parameters structure definition*/
struct RrcTagDrbCountMsbDownLinkT
{
    UInt32      countMsb;           /* count MSB for Down Link */
}ATTRIBUTE_PACKED;
typedef struct RrcTagDrbCountMsbDownLinkT RrcTagDrbCountMsbDownLink, *LP_RrcTagDrbCountMsbDownLink;
/*COUNTER_CHK_END*/

/* Lossy HO changes start */

/* PDCP_LOSSY_HO_REQUIRED tag - Manadatory parameters structure definition*/
struct RrcTagLossyHoRequiredT
{
    UInt8       lcId;
/* SPR 1757 changes start */
}ATTRIBUTE_PACKED ;
typedef struct RrcTagLossyHoRequiredT RrcTagLossyHoRequired, *LP_RrcTagLossyHoRequired;

/* Lossy HO changes stop */
/* SPR 17157 Fix start */
/* PDCP_MACI_REQ tag in create/reconfig UE */
typedef struct RrcTagMaciReqT
{
    UInt8  lcId;
    UInt16 rbDirection;
	/* Encoded String to calculate MACI, which is remaining length of the TLV */
}ATTRIBUTE_PACKED RrcTagMaciReq, *LP_RrcTagMaciReq;

/* PDCP_MACI_RESP response tag - Manadatory parameters structure definition*/
typedef struct RrcTagMaciRespT
{
    UInt32 maci; /* MACI calculated to be returned to RRC */
}ATTRIBUTE_PACKED RrcTagMaciResp, *LP_RrcTagMaciResp;
/* SPR 17157 Fix end */


/* + SPR 19066 */
/* PDCP_HO_PREP_INFO_REQ API - Manadatory parameters structure definition*/
typedef struct RrcHoPrepInfoReqT
{
    RrcHeader   header;             /* General RRC API header */
}ATTRIBUTE_PACKED RrcHoPrepInfoReq, *LP_RrcHoPrepInfoReq;

/* PDCP_HO_PREP_INFO_RESP API - Manadatory parameters structure definition*/
typedef struct RrcHoPrepInfoRespT
{
    RrcHeader   header;             /* General RRC API header */
    UInt16      responseCode;
}ATTRIBUTE_PACKED RrcHoPrepInfoResp, *LP_RrcHoPrepInfoResp;

/* PDCP_UE_INACTIVE_TIME_REQ response tag - 
 * Manadatory parameters structure definition */
typedef struct RrcTagInactTimeReqdT
{
    UInt8 inactTimeValReqd;
}ATTRIBUTE_PACKED RrcTagInactTimeReqd, *LP_RrcTagInactTimeReqd;

/* PDCP_UE_INACTIVITY_TIMER_CONFIG and 
 * PDCP_UE_INACTIVE_TIME_RESP response tag - 
 * Manadatory parameters structure definition */
typedef struct RrcTagInactivityTimerT
{
    UInt32 inactivityTimerVal;
}ATTRIBUTE_PACKED RrcTagInactivityTimer, *LP_RrcTagInactivityTimer;

/* PDCP_INACTIVE_UES_IND API - Mandatory parameters structure definition */
typedef struct RrcInactiveUesIndT
{
    UInt8 numOfInactiveUe;
} ATTRIBUTE_PACKED RrcInactiveUesInd, *LP_RrcInactiveUesInd;

/* PDCP_INACTIVE_UE_INFO Response Tag - Mandatory parameters structure definition */
typedef struct RrcInactiveUeInfoT
{
    UInt16      ueIndex;    /* UE identifier */
} ATTRIBUTE_PACKED RrcInactiveUeInfo, *LP_RrcInactiveUeInfo;
/* - SPR 19066 */
 
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
 * Function Name  : pdcpRrcSendMessage 
 * Inputs         : data_p  - pointer to TLV API buffer.
                    size  -  TLV API buffer size.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will be used by the PDCP layer for sending API message to 
                    the RRC layer.
                    The PDCP layer application has to implement this function because
                    it should be portable..
 ****************************************************************************/
extern void    pdcpRrcSendMessage( UInt8 * data_p, UInt16 size
        );


/****************************************************************************
 * Function Name  : pdcpRrcProcessMessage 
 * Inputs         : data_p  - pointer to TLV API buffer.
                    size  -  TLV API buffer size.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will be used by the PDCP layer application for delivering RRC API 
                    to the PDCP layer. The RRC API message will be processed immediately and 
                    after processing the message buffer will be freed using freeMemPool function call.
 ****************************************************************************/
void    pdcpRrcProcessMessage( UInt8 * data_p, UInt16 size );

/****************************************************************************
 * Function Name  : pdcpRrcSrbDataStatusInd 
 * Inputs         : tlvTransactionId - Transaction Id from TLV header.
                    ueIndex - UE Id.
                    lcId - logical channel id.
                    transactionId - Transaction Id.
                    responseCode - PDCP_SUCCESS/PDCP_FAIL
                    response - SRB SDU/PDU processing failing code.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will be used by the PDCP layer application for delivering to RRC layer 
                    the Integrity protection fail indication according to TS 36323-840 5.1.2.2 (RX)
                    On Tx side, it can be response of PDCP_SRB_DATA_REQ API for SRB SDU processing status indication. 
                    It wil be sent only if serviceRequested filed is set to true
 ****************************************************************************/
void    pdcpRrcSrbDataStatusInd( 
                                UInt16 tlvTransactionId, 
                                UInt16 ueIndex, 
                                UInt16 lcId, 
                                UInt32 transactionId, 
                                UInt16 responseCode, 
                                UInt16 response,
                                /* SPR 1825 changes start */
                                void* data_p,
                                UInt8 dataInZCB,
                                UInt32 dataSize
                                );
                                /* SPR 1825 changes end */

/****************************************************************************
 * Function Name  : pdcpRrcProcessMessage 
 * Inputs         : ueIndex - UE Id.
                    lcId - logical channel id.
                    data_p - pointer ZCB message.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will be used by the PDCP layer application for delivering SRB SDU to RRC layer. 
                    It creates PDCP_SRB_DATA_IND TLV message and sends it to RRC.
 ****************************************************************************/
void    pdcpRrcDataInd( UInt16 ueIndex, UInt16 lcId, void* data_p );


/* SPR 3607 changes Start */

/****************************************************************************
 * Function Name  : pdcpSendCountWraparoundInd
 * Inputs         : ueIndex - On which Count Wraparound has been done.
 * Outputs        : None.
 * Returns        : None.
 * Description    : It sends PDCP_COUNT_WRAPAROUND_IND message to RRC.
 ******************************************************************************/
void pdcpSendCountWraparoundInd( UInt16 ueIndex );

/* SPR 3607 changes End */

/* SPR 3175 changes Start */

/****************************************************************************
 * Function Name  : pdcpSendNotifyIntegrityFailureInd
 * Inputs         : ueIndex - On which IntegrityFailure occured.
 * Inputs         : lcId - On which IntegrityFailure occured.
 * Outputs        : None.
 * Returns        : None.
 * Description    : It sends PDCP_COUNT_WRAPAROUND_IND message to RRC.
 ******************************************************************************/
void pdcpSendNotifyIntegrityFailureInd( UInt16 ueIndex, UInt16 lcId );

/* SPR 3175 changes End */

/* + SPR 19066 */
/****************************************************************************
 * Function Name  : pdcpPrepareSendReport
 * Inputs         : numInactiveUe - number of inactive UEs
 *                  listInactiveUe_p - Contains the list of inactive UEs
 * Outputs        : None.
 * Returns        : None.
 * Description    : It constructs the message for L3 for all the inactive UEs
 *                  and send the same.
 ****************************************************************************/
void pdcpPrepareSendReport( UInt16 numInactiveUe, UInt16 *listInactiveUe_p 
#ifndef UE_SIM_TESTING
        ,InternalCellIndex  cellIndex
#endif
        );
/* - SPR 19066 */
#endif  /*_LIB_I_OAM_SAP_DEF_*/
