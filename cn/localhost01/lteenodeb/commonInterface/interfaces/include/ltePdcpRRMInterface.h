/**************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc. All Rights Reserved.
 *
 **************************************************************************
 *
 * $Id: ltePdcpRRMInterface.h 
 *
 **************************************************************************
 *
 * File Description : This file contains the definitions of the APIs 
 *                    defined for PDCP-RRM interface.
 *
 **************************************************************************/
#ifndef LTE_PDCP_RRM_INTERFACE_H
#define LTE_PDCP_RRM_INTERFACE_H
/**************************************************************************
 * Project Includes
 * *************************************************************************/
#include "lteTypes.h"
#include "lteL2RRMInterface.h"
/* API ID */
#define RRM_PDCP_REQ_MESSAGE_API_START 1
/* RRM to PDCP message to configure congestion notification status for ECN */
#define RRM_PDCP_UE_DRB_ECN_CE_REQ (RRM_PDCP_REQ_MESSAGE_API_START + 0)
/* RRM to PDCP message to configure ECN Parameters*/
#define RRM_PDCP_CONFIG_PARAMS_REQ (RRM_PDCP_REQ_MESSAGE_API_START + 1)
/* KPI */
#define RRM_PDCP_CONFIGURE_KPI_STATS_REQ (RRM_PDCP_REQ_MESSAGE_API_START + 2)
#define RRM_PDCP_GET_KPI_STATS_REQ       (RRM_PDCP_REQ_MESSAGE_API_START + 3)
/* KPI */

#define RRM_PDCP_CNF_MESSAGE_API_START 100
/* PDCP to RRM confirm message for congestion notification status for ECN */
#define RRM_PDCP_UE_DRB_ECN_CE_CNF (RRM_PDCP_CNF_MESSAGE_API_START + 0)
/* PDCP to RRM confirm message for configuration of  ECN Parameters*/
#define RRM_PDCP_CONFIG_PARAMS_CNF (RRM_PDCP_CNF_MESSAGE_API_START + 1)
/* PDCP to RRM indication message for ECN transport capability of ongoing packets*/
#define RRM_PDCP_UE_DRB_ECN_CE_IND (RRM_PDCP_CNF_MESSAGE_API_START + 2)
/* KPI */
#define RRM_PDCP_CONFIGURE_KPI_STATS_CNF (RRM_PDCP_CNF_MESSAGE_API_START + 3)
#define RRM_PDCP_KPI_STATS_IND           (RRM_PDCP_CNF_MESSAGE_API_START + 4)
#define RRM_PDCP_GET_KPI_STATS_CNF       (RRM_PDCP_CNF_MESSAGE_API_START + 5)
#define RRM_PDCP_KPI_THP_STATS_IND       (RRM_PDCP_CNF_MESSAGE_API_START + 6)
#define RRM_PDCP_GET_KPI_THP_STATS_CNF   (RRM_PDCP_CNF_MESSAGE_API_START + 7)
/* KPI */

/* API ID END */

/* Default values for Watermarks and Trigger shall be used by OAM/RRM
  only when ECN feature is enabled */
#define DEFAULT_ECN_FEATURE_STATUS  ECN_NOT_APPLIED
#define DEFAULT_ECN_TRIGGER         ECN_BOTH_TRIGGERS_APPLIED
#define DEFAULT_LOW_WATER_MARK      0x50
#define DEFAULT_HIGH_WATER_MARK     0x5A
/* Minimum value of watermark which can be configured via RRM*/
#define WATERMARK_START_RANGE         1
/* Maximum value of watermark which can be configured via RRM*/
#define WATERMARK_END_RANGE         100

/* Maximum number of DRBs supported */
#define MAX_CONFIGURED_DRB      0x08

#define MAX_PDCP_RRM_MSG_SZ 6144

/* ECN STATUS possible values */
#define ECN_CLEAR_CONGESTION            0x00
#define ECN_APPLY_CONGESTION            0x01

/**************************************************************************
 * Private Definitions
 *************************************************************************/
#define RRM_PDCP_UE_DRB_ECN_CE_REQ_LENGTH         sizeof(RrmPdcpDrbEcnCeReq)
#define RRM_PDCP_CONFIG_PARAMS_REQ_LENGTH         sizeof(RrmPdcpEcnConfigParamReq)
#define RRM_PDCP_UE_DRB_ECN_CE_CNF_LENGTH         sizeof(RrmPdcpDrbEcnCeCnf)
#define RRM_PDCP_CONFIG_PARAMS_CNF_LENGTH         sizeof(RrmPdcpEcnConfigParamCnf)
#define RRM_PDCP_UE_DRB_ECN_CE_IND_LENGTH         sizeof(RrmPdcpDrbEcnCeInd)
#define CONGESTION_SET_TO_RRM           0
#define CONGESTION_CANT_SET_TO_RRM      1

/***************************************************************************
 * Enum Definitions For APIs
 ***************************************************************************/

typedef enum EcnFeatureStatusT
{
    ECN_NOT_APPLIED = 0, /*ECN trigger not applied in UL and DL*/
    ECN_APPLIED_IN_UL,   /*ECN trigger applied in UL only*/
    ECN_APPLIED_IN_DL,   /*ECN trigger applied in DL only*/
    ECN_APPLIED_IN_BOTH_UL_DL /*ECN trigger applied both in UL and DL*/
}EcnFeatureStatus;

typedef enum EcnTriggerInfoT
{
    ECN_T1_BASED_ON_WATERMARKS = 1, /*Trigger-1 based on low & highWaterMark to be applied only for DL*/
    ECN_T2_BASED_ON_RRM_DETECTION,  /*Trigger-2 based on RRM detection to be applied both for UL and DL*/
    ECN_BOTH_TRIGGERS_APPLIED       /*Both triggers are applied*/
}EcnTriggerInfo;

typedef enum PdcpRrmErrorsT
{
    /* When ECN trigger for RRM detection is not applied, RRM request PDCP 
     * to configure the congestion notification status for DRB's
     * this error shall be sent to RRM*/
    PDCP_RRM_ERR_RRM_ECN_DETECTION_NOT_ENABLED = 3, 
    /* When RRM request PDCP to configure ECN for invalid UE */
    PDCP_RRM_ERR_UE_CONTEXT_NOT_INITIALIZED,
    /* When RRM request PDCP to configure ECN for invalid DRB */
    PDCP_RRM_ERR_LC_CONTEXT_NOT_INITIALIZED,
}PdcpRrmErrors;    

/***************************************************************************
* Structure Definitions For ecnReq
**************************************************************************/

/* This structure configure non-blocked frequency partitions and other
 * configuration for reporting */

typedef struct RrmPdcpEcnConfigParamReqT
{
    UInt8      ecnFeatureStatus; /*^ M, 0 ^*/
    UInt8      ecnTrigger; /*^ M, 0 ^*/
    UInt8      lowWaterMark; /*^ M, 0 ^*/
    UInt8      highWaterMark;  /*^ M, 0 ^*/
}RrmPdcpEcnConfigParamReq, *LP_RrmPdcpEcnConfigParamReq;   /*^ API, RRM_PDCP_CONFIG_PARAMS_REQ ^*/ 

typedef struct RrmPdcpCeLcListReqT
{
    UInt8 lcId; /*^ M, 0 ^*/              /* LC Id*/
    UInt8 direction; /*^ M, 0 ^*/        /* direction of LC ID's DL (TX -> 0) UL (RX -> 1) or BOTH -> 2*/
    UInt8 congestionStatus; /*^ M, 0 ^*/ /* congestion notification status to RRM enble/disable (1/0)*/
    UInt8 pad; /*^ M, 0 ^*/ /*Added to make struture 32 bit aligned, reserved for future use*/
}RrmPdcpCeLcListReq;    

typedef struct RrmPdcpDrbEcnCeReqT
{
    UInt16              ueIndex; /*^ M, 0 ^*/
    UInt8               numOfLc; /*^ M, 0 ^*/ /* number of LC Id's which shall be configured for congestion notification*/
    UInt8               pad;  /*^ M, 0 ^*/ /*Added to make struture 32 bit aligned, reserved for future use*/
    RrmPdcpCeLcListReq  ceLc[MAX_CONFIGURED_DRB]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}RrmPdcpDrbEcnCeReq, *LP_RrmPdcpDrbEcnCeReq; /*^ API, RRM_PDCP_UE_DRB_ECN_CE_REQ ^*/

/***************************************************************************
* Structure Definitions For ecn confirm/resp/inication messages
**************************************************************************/

typedef struct PdcpRrmEcnConfigParamCnfT
{
    UInt8   respCode; /*^ M, 0 ^*/ /*Response Code*/
    UInt8   pad[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/ /*Added to make struture 32 bit aligned, reserved for future use*/
}RrmPdcpEcnConfigParamCnf, *LP_RrmPdcpEcnConfigParamCnf;   /*^ API, RRM_PDCP_CONFIG_PARAMS_CNF ^*/ 

typedef struct RrmPdcpCeLcListRespT
{
    UInt8 lcId; /*^ M, 0 ^*/
    UInt8 direction; /*^ M, 0 ^*/
    UInt8 result; /*^ M, 0 ^*/ /* Congestion Notification status is successfully set or not ( FAIL/SUCCESS) */
    UInt8 pad; /*^ M, 0 ^*/ /*Added to make struture 32 bit aligned, reserved for future use*/
}RrmPdcpCeLcListResp;    

typedef struct RrmPdcpDrbEcnCeCnfT
{
    UInt16                ueIndex; /*^ M, 0 ^*/
    UInt8                 respCode; /*^ M, 0 ^*/ /* FAIL/PARTIAL_SUCCESS/SUCCESS */
    UInt8                 numOfLc;  /*^ M, 0 ^*/
    RrmPdcpCeLcListResp   ceLc[MAX_CONFIGURED_DRB]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}RrmPdcpDrbEcnCeCnf, *LP_RrmPdcpDrbEcnCeCnf;   /*^ API, RRM_PDCP_UE_DRB_ECN_CE_CNF ^*/ 

typedef struct RrmPdcpDrbEcnCeIndT
{
    UInt16  ueIndex;  /*^ M, 0 ^*/
    UInt8   lcId;  /*^ M, 0 ^*/
    UInt8   direction;  /*^ M, 0 ^*/
    UInt8   congestionStatus;  /*^ M, 0 ^*/ /* Indicate whether ongoing packets are ECN Transport capable or not (0 -> ECN capable, 1 -> ECN not capable) */
    UInt8   pad[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/ /*Added to make struture 32 bit aligned, reserved for future use*/
}RrmPdcpDrbEcnCeInd;   /*^ API, RRM_PDCP_UE_DRB_ECN_CE_IND ^*/ 

/* SPR 9068 Fix Start */
/* Structure definition for API : RRM_PDCP_CONFIGURE_KPI_STATS_CNF
 * Structure name               :PdcpRrmConfigureKpiStatsResp */
typedef struct PdcpRrmConfigureKpiStatsRespT {
    UInt32 responseCode; /*^ M, 0 ^*/         /* PDCP_SUCCESS or an Error Code */
} PdcpRrmConfigureKpiStatsResp, *LP_PdcpRrmConfigureKpiStatsResp; /*^ API, RRM_PDCP_CONFIGURE_KPI_STATS_CNF ^*/
/* SPR 9068 Fix End */
#endif

