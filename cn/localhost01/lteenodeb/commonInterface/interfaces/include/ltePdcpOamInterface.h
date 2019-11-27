/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2014 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpOamInterface.h,v 1.1.1.1 2014/02/01 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Definitions for Interface between the PDCP layer and 
 *                     OAM stack entity
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpOamInterface.h,v $
 * Revision 1.1.1.1  2014/02/01 04:51:26  cm_intel
 * eNB framework for intel
 *
 ****************************************************************************/

#ifndef LTE_PDCP_OAM_INTERFACE_H
#define LTE_PDCP_OAM_INTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTypes.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/
#define PDCP_VAR_SIZE(x) 1
 /* The range of channels IDs supported for SRB */
#define LTE_SRB_LC_ID_LOW_BOUND     1
#define LTE_SRB_LC_ID_HIGH_BOUND    2

 /* The range of channels IDs supported for DRB */
#define LTE_DRB_LC_ID_LOW_BOUND     3
#define LTE_DRB_LC_ID_HIGH_BOUND    10

/* Max SRB = (LTE_SRB_LC_ID_HIGH_BOUND - LTE_SRB_LC_ID_LOW_BOUND + 1) */
#define LTE_MAX_SRB 2
/* LTE_MAX_DRB  = LTE_DRB_LC_ID_HIGH_BOUND - LTE_DRB_LC_ID_LOW_BOUND + 1 */
#define LTE_MAX_DRB 8

/* SPR 10021 fix start */
/* Following index used for number of KPI values reported */
#define MAX_KPI_INDEX_PDCP 80
/* SPR 10021 fix end */
/*SPR 17663 + */
#define LTE_SRB_LC_ID (LTE_SRB_LC_ID_HIGH_BOUND - LTE_SRB_LC_ID_LOW_BOUND + 1)
#define LTE_DRB_LC_ID (LTE_DRB_LC_ID_HIGH_BOUND - LTE_DRB_LC_ID_LOW_BOUND + 1)
/*SPR 17663 - */
/*+- SPR 10577*/
/****************************************************************************
* Following structure defines SPS Interval profile 
****************************************************************************/
typedef struct PdcpSpsIntervalProfileT
{
    /* Silence packet PDCP SDU size (i.e. before header compression and after
     * GTP header removal) when IPv4 header is present. This is used for
     * silence packet detection. */
    UInt32 silencePdcpSduSizeIpv4;  /*^ M, 0 ^*/
    /* Silence packet PDCP SDU size (i.e. before header compression and after
     * GTP header removal) when IPv6 header is present. This is used for
     * silence packet detection. */
    UInt32 silencePdcpSduSizeIpv6;  /*^ M, 0 ^*/
    /* When enabled, eNB shall detect silence period and deactivate SPS */
    UInt8 enableSilenceDetection;  /*^ M, 0 ^*/
    /* Interpacket arrival time of silence packets after which silence period
     * is detected. The unit of parameter is number of SPS interval */
    UInt8 interPktDelayForSilenceDetection;  /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8 pading[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}*LP_PdcpSpsIntervalProfile, PdcpSpsIntervalProfile;

/****************************************************************************
* Following structure defines Init parameters for serving cells 
****************************************************************************/
/* + Layer2 NON CA Changes */
typedef struct PdcpSecondaryCellInitInfoT
{
    UInt8       bitMask;  /*^ M, 0 ^*/        /* Count Wraparound Threshold bitMask*/
    UInt8       cellId;  /*^ M, 0 ^*/ /* Secondary Cell Index */
    UInt16      maxUeNumber;  /*^ M, 0 ^*/ /* Maximum number of supported UEs for this cell 
                                              of given cell index */
    UInt32      thresholdValue;  /*^ M, 0 ^*/ /* the maximum threshold value for Count 
                                                 Wraparound*/
    /* list of DL SPS intervals supported. By default only 20ms shall be
     * enabled and rest of the values disabled.
     * Values from index 10 to 15 are for future extensions */
    UInt8 supportedSpsDlIntervalList[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* list of UL SPS intervals supported. By default only 20ms shall be
     * enabled and rest of the values disabled.
     * Values from index 10 to 15 are for future extensions */
    UInt8 supportedSpsUlIntervalList[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* SPS interval profile parameters */
    PdcpSpsIntervalProfile spsProfileParams[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* Array containing list of QCIs for which SPS is enabled. Indices
     * corresponds to QCI. Value corr to SPS enabled/disabled for that QCI. */
    UInt8  qciSpslist[MAX_QCI] ; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* for enbling/disabling SPS scheduling in MAC */
    UInt8  enableSps;  /*^ M, 0 ^*/
    /* For byte alignment */
    UInt8 pading[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}PdcpSecondaryCellInfo;

/* - Layer2 NON CA Changes */

/****************************************************************************
 * PDCP_INIT_LAYER_REQ API - Mandatory parameters structure definition 
 ****************************************************************************/
typedef struct PdcpOamInitLayerReqT
{
    UInt16      maxUeNumber; /*^ M, 0 ^*/   /* Maximum number of supported UEs */
    UInt16      rntiStartRange; /*^ M, 0 ^*/ /* the starting value of rnti to be supported 
                                                for ueId */
    UInt16      rntiEndRange; /*^ M, 0 ^*/   /* the maximum rnti value to be supported 
                                                for ueId */
    UInt8       logLevel; /*^ M, 0 ^*/
#ifndef UE_SIM_TESTING
    /* SPR 3607 Changes Start */
    UInt8       bitMask; /*^ M, 0 ^*/        /* Count Wraparound Threshold bitMask*/
    UInt32      thresholdValue;  /*^ M, 0 ^*/ /* the maximum threshold value for Count
                                                 Wraparound*/
    /* SPR 3607 Changes End */
    UInt16    healthMonitoringTimeInterval; /*^ M, 0 ^*/
    /* for enbling/disabling SPS scheduling in MAC */
    UInt8  enableSps; /*^ M, 0 ^*/
    /* Total Number of Serving Cells */
    UInt8  sCellCount; /*^ M, 0 ^*/ 
    /* list of DL SPS intervals supported. By default only 20ms shall be
     * enabled and rest of the values disabled.
     * Values from index 10 to 15 are for future extensions */
    UInt8 supportedSpsDlIntervalList[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* list of UL SPS intervals supported. By default only 20ms shall be
     * enabled and rest of the values disabled.
     * Values from index 10 to 15 are for future extensions */
    UInt8 supportedSpsUlIntervalList[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* SPS interval profile parameters */
    PdcpSpsIntervalProfile spsProfileParams[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/

    /* MAX_INTERNAL_CELL_INDEX is number of total cells - 1, as this IE includes
     * info for second cell onwards */ 
    PdcpSecondaryCellInfo   sCellInfo[MAX_INTERNAL_CELL_INDEX]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* Array containing list of QCIs for which SPS is enabled. Indices
     * corresponds to QCI. Value corr to SPS enabled/disabled for that QCI. */
    UInt8  qciSpslist[MAX_QCI] ; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt8 padding[3]; /* For byte allignment */

#endif
} PdcpOamInitLayerReq, *LP_PdcpOamInitLayerReq; /*^ API, PDCP_INIT_LAYER_REQ ^*/

/****************************************************************************
 * PDCP_INIT_LAYER_CNF,
 * PDCP_SET_LOG_LEVEL_CNF,
 * PDCP_RESET_STATS_CNF,
 * PDCP_SET_LOG_CATEGORY_RESP - Mandatory parameters structure definition 
 ****************************************************************************/
typedef struct OamCnfT
{
    UInt16      responseCode; /*^ M, 0 ^*/    /* Response code */
} PdcpOamCnf, *LP_PdcpOamCnf; /*^ API, PDCP_INIT_LAYER_CNF ^*/

/****************************************************************************
 * PDCP_SET_LOG_LEVEL_REQ API - Mandatory parameters structure definition 
 ****************************************************************************/
typedef struct PdcpOamSetLogLevelReqT
{
    UInt32       logLevel; /*^ M, 0 ^*/
} PdcpOamSetLogLevelReq, *LP_PdcpOamSetLogLevelReq; /*^ API, PDCP_SET_LOG_LEVEL_REQ ^*/

/****************************************************************************
 * PDCP_GET_LOG_LEVEL_REQ API - Mandatory parameters structure definition 
 ****************************************************************************/
/* No Payload Required */

/****************************************************************************
 * PDCP_GET_LOG_LEVEL_RESP API - Mandatory parameters structure definition 
 ****************************************************************************/
typedef struct PdcpOamGetLogLevelRespT
{
    UInt32       logLevel; /*^ M, 0 ^*/
} PdcpOamGetLogLevelResp, *LP_PdcpOamGetLogLevelResp; /*^ API, PDCP_GET_LOG_LEVEL_RESP ^*/

/******************************************************************************
 *  PDCP_ENABLE_LOG_CATEGORY_REQ
 ******************************************************************************/
typedef struct PdcpOamEnableLogCategoryReqT
{
    UInt64 logCategory; /*^ M, 0 ^*/
}PdcpOamEnableLogCategoryReq, *LP_PdcpOamEnableLogCategoryReq; /*^ API, PDCP_ENABLE_LOG_CATEGORY_REQ ^*/

/******************************************************************************
 *  PDCP_DISABLE_LOG_CATEGORY_REQ
 ******************************************************************************/
typedef struct PdcpOamDisableLogCategoryReqT
{
    UInt64 logCategory; /*^ M, 0 ^*/
}PdcpOamDisableLogCategoryReq, *LP_PdcpOamDisableLogCategoryReq; /*^ API, PDCP_DISABLE_LOG_CATEGORY_REQ ^*/

/****************************************************************************
 * PDCP_GET_LOG_CATEGORY_REQ API - Mandatory parameters structure definition 
 ****************************************************************************/
/* No Payload Required */

/****************************************************************************
 * PDCP_GET_LOG_CATEGORY_RESP API - Mandatory parameters structure definition 
 ****************************************************************************/
typedef struct PdcpOamGetLogCategoryRespT
{
    UInt64      logCategory; /*^ M, 0 ^*/
} PdcpOamGetLogCategoryResp, *LP_PdcpOamGetLogCategoryResp; /*^ API, PDCP_GET_LOG_CATEGORY_RESP ^*/

/****************************************************************************
 * PDCP_CONFIGURE_KPI_STATS_REQ API - Mandatory parameters structure definition 
 ****************************************************************************/
typedef struct PdcpConfigureKpiStatsReqParamsT 
{
    UInt16 durationT;  /*^ M, 0 ^*/             /* KPI timer value */
    UInt8  periodicReporting;  /*^ M, 0 ^*/    /* Indicates periodic reporting required or
                                      not */
    UInt8  kpiToReportBitMap; /*^ GLOBAL ^*/     /* Each bit of this bitmap represents KPI */
} PdcpConfigureKpiStatsReqParams, *LP_PdcpConfigureKpiStatsReqParams; /*^ API, PDCP_CONFIGURE_KPI_STATS_REQ, RRM_PDCP_CONFIGURE_KPI_STATS_REQ ^*/

/* Structure definition for API : PDCP_CONFIGURE_KPI_STATS_RESP
 * Structure name               :PdcpConfigureKpiStatsResp */
typedef struct PdcpConfigureKpiStatsRespT {
/* SPR 9068 Fix Start */
    UInt16 responseCode; /*^ M, 0 ^*/         /* PDCP_SUCCESS or an Error Code */
/* SPR 9068 Fix End */
} PdcpConfigureKpiStatsResp, *LP_PdcpConfigureKpiStatsResp; /*^ API, PDCP_CONFIGURE_KPI_STATS_RESP ^*/

/* Structure definition for API : PDCP_KPI_STATS_IND
 * Structure name               :PdcpKpiStatsIndParams */
typedef struct PdcpKpiStatsIndParamsT {
    UInt32 durationTTI;  /*^ M, 0 ^*/
    UInt32 totalDlDrbDiscardRatePerQci[MAX_QCI]; /*^ T, 1, OCTET_STRING, FIXED ^*/ 
    UInt32 totalUlLossRatePerQci[MAX_QCI]; /*^ T, 4, OCTET_STRING, FIXED ^*/
    UInt32 totalSduRcvdDlperQci[MAX_QCI]; /*^ T, 8, OCTET_STRING, FIXED ^*/
    UInt32 totalSduRcvdUlperQci[MAX_QCI]; /*^ T, 32, OCTET_STRING, FIXED ^*/
    /* KPI changes start */  
    UInt32 dlCtrlPdcpSduBitRate; /*^ T, 64 ^*/
    UInt32 ulCtrlPdcpSduBitRate; /*^ T, 128 ^*/
    /* KPI changes end */

} PdcpKpiStatsIndParams, *LP_PdcpKpiStatsIndParams; /*^ API, PDCP_KPI_STATS_IND, RRM_PDCP_KPI_STATS_IND, RRM_PDCP_GET_KPI_STATS_CNF ^*/

/* Structure definition for API : PDCP_GET_KPI_STATS_REQ
 * Structure name               :PdcpGetKpiStatsReqParams */
typedef struct PdcpGetKpiStatsReqParamsT 
{
    UInt16 resetStats;   /*^ M, 0 ^*/    /* 0 (false) , 1 (true) */
    UInt8  kpiToGetBitMap; /*^ GLOBAL ^*/     /* Each bit of this bitmap represents KPI */
} PdcpGetKpiStatsReqParams, *LP_PdcpGetKpiStatsReqParams; /*^ API, PDCP_GET_KPI_STATS_REQ, RRM_PDCP_GET_KPI_STATS_REQ ^*/
/*SPR 17663 + */
#ifdef OAM_COMPILE
/* SPR 15692 fix end */
#define MAX_KPI_INDEX 80

/* Structure definition for API : PDCP_KPI_THP_STATS_IND
 * Structure name               :PdcpKpiThpStatsIndParams */
typedef struct PdcpKpiThpStatsIndParamsT 
{
    UInt32 avgDurationT;  /*^ M, 0 ^*/
    UInt16 ueIndex[ MAX_KPI_INDEX ]; /*^ M, 0, OCTET_STRING, FIXED^*/
    UInt8  qci[ MAX_KPI_INDEX ]; /*^ M, 0, OCTET_STRING, FIXED^*/
/*spr 19711 fix start*/ 
	/*code deleted*/
/*spr 19711 fix end*/ 
/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/
    UInt64 pdcpSduBitDl[MAX_KPI_INDEX]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt64 pdcpSduBitUl[MAX_KPI_INDEX]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* + SPR 10577 */
    UInt8 endIndicator; /*^ M, 0 ^*/
    /* padding is added for byte alignment,this is reserved and can 
     * be used for new fields in future.Its value should be ignored */
    UInt8 padding[3]; 
    /* - SPR 10577 */
} PdcpKpiThpStatsIndParams, *LP_PdcpKpiThpStatsIndParams; /*^ API, PDCP_KPI_THP_STATS_IND,RRM_PDCP_KPI_THP_STATS_IND,RRM_PDCP_GET_KPI_THP_STATS_CNF   ^*/
/*Wiresect Changes*/
/* Statistic items for Integrity protection feature */
typedef struct PdcpIntegrityStatsT
{
    UInt32 numberOfPacketsIntegrityProtectedUsingEIA0;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsIntegrityVerifiedUsingEIA0;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsIntegrityProtectedUsingEIA1;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsIntegrityVerifiedUsingEIA1;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsIntegrityProtectedUsingEIA2;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsIntegrityVerifiedUsingEIA2;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsRejectedByIntegrityVerification;
    /*^ M, 0 ^*/
}PdcpIntegrityStats;

/* Statistic items for Ciphering feature */
typedef struct PdcpCipheringStatsT
{
    UInt32 numberOfPacketsCipheredUsingEIA0;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsDecipheredUsingEIA0;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsCipheredUsingEIA1;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsDecipheredUsingEIA1;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsCipheredUsingEIA2;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsDecipheredUsingEIA2;
    /*^ M, 0 ^*/
}PdcpCipheringStats;

/* Statistic items for Ciphering feature */
typedef struct PdcpRohcStatsT
{
    UInt32 numberOfIRPacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfIRPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfIRDYNPacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfIRDYNPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfACKProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfACKCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfNACKProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfNACKCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfSNACKProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfSNACKCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfR0PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfR0CRCPacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfUO0PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfR1PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfR1IDPacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfR1TSPacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfUO1PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfUO1IDPacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfUO1TSPacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfUOR2PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfUOR2IDPacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfUOR2TSPacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfR0PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfR0CRCPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfUO0PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfR1PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfR1IDPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfR1TSPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfUO1PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfUO1IDPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfUO1TSPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfUOR2PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfUOR2IDPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfUOR2TSPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransU2OInit;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransU2RInit;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransO2RInit;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransO2UInit;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransR2UInit;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransR2OInit;
    /*^ M, 0 ^*/
    UInt32 numberOfTimesCrcFailed;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransU2ORcvd;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransU2RRcvd;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransO2RRcvd;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransO2URcvd;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransR2URcvd;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransR2ORcvd;
    /*^ M, 0 ^*/
    UInt32 numberOfP0NormalPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfP0NormalPacketsProcessed;
    /*^ M, 0 ^*/
    /* Profile 6 packet types stats */
    UInt32 numberOfIRCRP6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfIRCRP6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfCOP6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfCOP6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfRND1P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfRND1P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfRND2P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfRND2P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfRND3P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfRND3P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfRND4P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfRND4P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfRND5P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfRND5P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfRND6P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfRND6P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfRND7P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfRND7P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfRND8P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfRND8P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ1P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ1P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ2P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ2P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ3P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ3P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ4P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ4P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ5P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ5P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ6P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ6P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ7P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ7P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ8P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ8P6PacketsProcessed;
    /*^ M, 0 ^*/
}PdcpRohcStats;
/* UE Stats data structues */
/* Group of Statistic items for SRB entity*/
typedef struct StatsSrbDataT
{
    /*SPR 3189 changes start */
    UInt8 srbId ;
    /*^ M, 0 ^*/
    /*SPR 3189 changes end */
    /* Downlink stats */
    UInt32 numberOfDlSrbSduReceived;
    /*^ M, 0 ^*/
    UInt32 numberOfDlSrbPduTransmitted;
    /*^ M, 0 ^*/
    UInt32 numberOfSrbDeliveryIndSent;
    /*^ M, 0 ^*/
    UInt32 numberOfDlSrbPduDropNackRecvd;
    /*^ M, 0 ^*/
    UInt32 numberOfDlSrbSduDiscarded;
    /*^ M, 0 ^*/
    /* Uplink stats */
    UInt32 numberOfSrbUlPduReceived;
    /*^ M, 0 ^*/
    UInt32 numberOfSrbUlSduTransmitted;
    /*^ M, 0 ^*/
    UInt32 numberOfUlSrbSduDropped;
    /*^ M, 0 ^*/
} StatsSrbData, *LP_StatsSrbData;
//} StatsSrbData;

/* Group of Statistic items for DRB entity*/
typedef struct StatsDrbDataT
{
    /*SPR 3189 changes start */
    UInt8 lcId ;
    /*^ M, 0 ^*/
    /*SPR 3189 changes end */
    /* Downlink stats */
    UInt32 numberOfDlDrbSduReceived;
    /*^ M, 0 ^*/
    UInt32 numberOfDlDrbPduTransmitted;
    /*^ M, 0 ^*/
    UInt32 numberOfDlDrbPduDropForNack;
    /*^ M, 0 ^*/
    UInt32 numberOfDlDrbPduDropForRoHCFailure;
    /*^ M, 0 ^*/
    UInt32 numberOfDrbDeliveryIndSent;
    /*^ M, 0 ^*/
    UInt32 numberOfDlDrbSduDiscarded;
    /*^ M, 0 ^*/
    /* Uplink stats */
    UInt32 numberOfDrbUlSduReceived;
    /*^ M, 0 ^*/
    UInt32 numberOfDrbUlPduTransmitted;
    /*^ M, 0 ^*/
    UInt32 numberOfDrbUlPduDropDueToRoHcFailure;
    /*^ M, 0 ^*/
    UInt32 numberOfDrbUlPduDropAsSizeOORange;
    /*^ M, 0 ^*/
    /*SPR 3811 changes start */
    UInt32 numberOfDrbUlPduDropAsReservedBitSet;
    /*^ M, 0 ^*/
    /*SPR 3811 changes end */

} StatsDrbData, *LP_StatsDrbData;

/* UE entity Statistic item */
typedef struct StatsUeT
{
    UInt16 crnti;
    /*^ M, 0 ^*/
    UInt16 statNumActiveSrbLc ; /*^ M, 0, READ ^*/
    
    UInt16 statNumActiveDrbLc ; /*^ M, 0, READ ^*/
   
    /*StatsSrbData  srbs[LTE_SRB_LC_ID_HIGH_BOUND - LTE_SRB_LC_ID_LOW_BOUND + 1];*/
    StatsSrbData  srbs[LTE_SRB_LC_ID];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, statNumActiveSrbLc ^*/
    /*StatsDrbData  drbs[LTE_DRB_LC_ID_HIGH_BOUND - LTE_DRB_LC_ID_LOW_BOUND + 1];*/
    StatsDrbData  drbs[LTE_DRB_LC_ID];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, statNumActiveDrbLc ^*/
}__attribute__ ((packed)) StatsUe, *LP_StatsUe;

typedef struct PdcpStatsT {
    PdcpIntegrityStats statsIntegrity;  /*^ M, 0 ^*/ 
    PdcpCipheringStats statsCiphering;  /*^ M, 0 ^*/ 
    PdcpRohcStats statsRohc;  /*^ M, 0 ^*/ 
    UInt16 statNumActiveUe ;  /*^ M, 0 ^*/ 
    /*CSR 40799 chg start*/ 
    StatsUe statsUEs[MAX_UE_SUPPORTED]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/ 
    /*CSR 40799 chg end*/
}__attribute__ ((packed)) PdcpStats;
typedef struct StatusSrbEntityT {
    UInt16 rlcMode;
    /*^ M, 0 ^*/
    UInt16 direction;
    /*^ M, 0 ^*/
    /*SPR 3189 changes start */
    UInt8 srbId ;
    /*^ M, 0 ^*/
    /*SPR 3189 changes end */
    /* For byte alignment */
    UInt8 pading[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}StatusSrbEntity;

typedef struct StatusDrbEntityT {
    UInt16 rlcMode;
    /*^ M, 0 ^*/
    UInt16 direction;
    /*^ M, 0 ^*/
    UInt16 discardTimerValue;
    /*^ M, 0 ^*/
    UInt16 statusReprtField;
    /*^ M, 0 ^*/
    UInt16 snFieldLength;
    /*^ M, 0 ^*/
    UInt16 rohcProfileUsed;
    /*^ M, 0 ^*/
    /*SPR 3189 changes start */
    UInt8 lcId ;
    /*^ M, 0 ^*/
    /*SPR 3189 changes end */
    /* For byte alignment */
    UInt8 pading[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}StatusDrbEntity;

typedef struct PdcpStatusT {
    UInt16 ueIndex ;
    /*^ M, 0 ^*/
    UInt16 crnti;
    /*^ M, 0 ^*/
    UInt16 integrityAlgoUsed;
    /*^ M, 0 ^*/
    UInt16 cipheringSrbAlgoUsed;
    /*^ M, 0 ^*/
    UInt16 cipheringDrbAlgoUsed;
    /*^ M, 0 ^*/
    UInt16 numActiveSrbEntities;
    /*^ M, 0, READ ^*/
    UInt16 numActiveDrbEntities;
    /*^ M, 0, READ ^*/
    /* For byte alignment */
    UInt8 pading[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    StatusSrbEntity srbs[LTE_SRB_LC_ID];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numActiveSrbEntities ^*/
    StatusDrbEntity drbs[LTE_DRB_LC_ID];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numActiveDrbEntities ^*/
} PdcpStatus ;
/* +- 10577 */
/* PDCP UE STATUS REQUEST message */
typedef struct PdcpStatusReqT {
    UInt16 ueStartRange ;
    /*^ M, 0 ^*/
    UInt16 ueEndRange ;
    /*^ M, 0 ^*/
}PdcpStatusReq, *LP_PdcpStatusReq;	/*^ API, PDCP_GET_STATUS_REQ ^*/

typedef struct PdcpStatsCnfT{
    UInt16 responseCode;
    /*^ M, 0 ^*/
    PdcpIntegrityStats statsIntegrity;
    /*^ M, 0 ^*/
    PdcpCipheringStats statsCiphering;
    /*^ M, 0 ^*/
    PdcpRohcStats statsRohc;
    /*^ M, 0 ^*/
    UInt16 statNumActiveUe ;
    /*^ M, 0 ^*/
    UInt16  startUeId;
    /*^ M, 0 ^*/
    UInt8  numOfUe;
    /*^ M, 0 ^*/
    UInt8  UesLeft;
    /*^ M, 0 ^*/
    StatsUe statsUEs_p[PDCP_VAR_SIZE(statNumActiveUe)];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}__attribute__ ((packed)) PdcpStatsCnf, *LP_PdcpStatsCnf;/*^ API, PDCP_GET_STATS_RESP ^*/
/* PDCP UE STATUS RESPONSE message */
typedef struct PdcpStatusCnfT{
    UInt16 responseCode;
    /*^ M, 0 ^*/
    UInt16 numActiveUE;
    /*^ M, 0 ^*/
    PdcpStatus data_p[PDCP_VAR_SIZE(numActiveUE)];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}PdcpUeStatusCnf, *LP_PdcpUeStatusCnf;	/*^ API, PDCP_GET_STATUS_RESP ^*/
/* SPR 15692 fix start */
/* Following index used for number of KPI values reported */
#endif
/*Wiresect Changes*//*Done*/
/*spr 22474 start*/
/* OAM_PDCP_GET_DEBUG_INFO_REQ */
typedef struct PdcpGetDebugInfoReqParamsT {
    /* This field informs the type of debugging information needed */
    UInt16 debugType; /*^ M, 0 ^*/ /* Check Enum GetDebugType */
}PdcpGetDebugInfoReqParams, *LP_PdcpGetDebugInfoReqParams;

/* PDCP_OAM_GET_DEBUG_INFO_RESP */
typedef struct PdcpGetDebugInfoRespParamsT {
    UInt16 responseCode; /*^ M, 0 ^*/   // SUCCESS or an Error Code
    /* This field informs the type of debugging information sent */
    UInt16 debugType; /*^ M, 0 ^*/ /* Check Enum GetDebugType */
    union {
         L2MemPoolStats csplMemPoolStats;
         L2MsgPoolStats csplMsgPoolStats;
    }u;
}PdcpGetDebugInfoRespParams, *LP_PdcpGetDebugInfoRespParams;
/*spr 22474 end*/

#endif
