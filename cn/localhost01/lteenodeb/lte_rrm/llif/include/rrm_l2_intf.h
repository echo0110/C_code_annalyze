/*! \file	rrm_l2_intf.h 
 *  \brief 	This file contains the the UE context information.
 *                  and contains the information about procedural data.
 *  \date 	March 23, 2012
 *  \author gur26991
*/
/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
*/

#ifndef _RRM_L2_INTF_H_
#define	_RRM_L2_INTF_H_
/*! \headerfile rrm_defines.h <>
 */
#include "rrm_defines.h"
/*! \headerfile rrm_cell_rrmim_intf.h <>
 */
#include  "rrm_cell_rrmim_intf.h"
#include  "lteMacRRMInterface.h"
#include  "lteMacOAMInterface.h"
#include  "ltePdcpRRMInterface.h"
#include  "ltePdcpOamInterface.h"
/*! \enum load_value
 *  \brief An enum that denotes the type of load
 */

typedef enum
{
    LOAD_LOW,
    LOAD_MEDIUM,
    LOAD_HIGH,
    LOAD_OVERLOAD
}load_value;
#define MAX_INTERFERENCE_PER_PRB     100
#define MAX_UE_PER_BATCH              10
# define MAC_CONFIGURE_PERF_STATS_REQ          (MAC_MESSAGE_API_REQ_START + 7)
/*RRM KPI STATS*/
#define MAC_CONFIGURE_PERF_STATS_CNF           (RRM_MAC_CNF_MESSAGE_API_START + 10)
#define MAX_MAC_UE_STATS_IND		32

#define MAC_CELL_PERF_STATS_IND             (RRM_MAC_IND_MESSAGE_API_START + 4)
/* review comment changes start */
/* bit 1 of byte 0 indicatesi,
 * restricted CSI measurement is configured
  * LTE MAC Api doc */
#define RRM_MAC_UE_PERIODIC_CSI_SUBFRAME_SET2_REPORT_PRESENCE_BITMASK 0x02
/* eICIC_PHASE_1_2_CHANGES_END */
/* review comment changes end */

/* pdcp_ind macros */
#define INVALID_THP_ENTRY        0xFFFFFFFF
#define INVALID_THP_STATS_ENTRY        0xFFFFFFFFFFFFFFFFULL




/* SPR 15692 fix start */
/* Following index used for number of KPI values reported */
#define MAX_KPI_INDEX 80

/* Structure definition for API : PDCP_KPI_THP_STATS_IND
 *  * Structure name               :PdcpKpiThpStatsIndParams */
typedef struct PdcpKpiThpStatsIndParamsT
{
    UInt32 avgDurationT;  /*^ M, 0 ^*/
    UInt16 ueIndex[ MAX_KPI_INDEX ]; /*^ M, 0, OCTET_STRING, FIXED^*/
    UInt8  qci[ MAX_KPI_INDEX ]; /*^ M, 0, OCTET_STRING, FIXED^*/
/*spr 19711 fix start*/ 
	/*code deleted*/
/*spr 19711 fix end*/ 
    UInt64 pdcpSduBitDl[MAX_KPI_INDEX]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt64 pdcpSduBitUl[MAX_KPI_INDEX]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* + SPR 10577 */
    /* + SPR 10813 */
    UInt8 endIndicator; /*^ M, 0 ^*/
    /* padding is added for byte alignment,this is reserved and can
     *      * be used for new fields in future.Its value should be ignored */
    UInt8 padding[3];
    /* - SPR 10577 */
    /* - SPR 10813 */
} PdcpKpiThpStatsIndParams, *LP_PdcpKpiThpStatsIndParams; /*^ API, PDCP_KPI_THP_STATS_IND ^*/
/* SPR 15692 fix end */


/* Following index used for number of KPI values reported */

/*! \   rrm_ue_stats_t
 *  \brief This struct contains ue stats information 
 *  \param ue_index Indicates the ue index
 *  \param uplinkSINRValue Indicate uplink SINR value (0 - 255 )
 *  \param downlinkSINRValueCW0 Indicate downlink SINR Value CW0 (0 - 255 )
 *  \param downlinkSINRValueCW1 Indicate downlink SINR Value CW1 (0 - 255 )
 *  \param uplinkBlerValue Indicate Uplink Bler value (0 - 255)
 *  \param downlinkBlerValueCW0 Indicate downlink Bler Value CW0(0 -100 )
 *  \param downlinkBlerValueCW1 Indicate downlink Bler Value CW1(0 -100 )
 *  \param uplinkPathlossValue indicate uplink path loss value
 *  \param timingAdvanceValue indicate timing advance value
 */
/* eICIC_PHASE_1_2_CHANGES_START */
/*! \   rrm_ue_mac_stats_report_t
 *  \brief This struct contains ue mac stats report parameters
 *  \param ue_count contains the count of UEs
 *  \param ue_stats Pointer to ue stats structure
 */
/* ++ SPR 20078 Code removed -- */
typedef struct _rrm_ue_mac_stats_report_t
{
    U16                ue_count;
/* + SPR 20078 */
    PeriodicReportInfo     *ue_stats; 
/* - SPR 20078 */

}rrm_ue_mac_stats_report_t;
/*LTE_RRM_TM_SWITCH_END*/

/*!  \  rrm_l2_get_snr_report
 *   \brief This struct is used to get SNR report at layer2
 *   \param ue_index denotes the index of UE
 */
typedef struct _rrm_l2_get_snr_report
{
    rrm_ue_index_t              ue_index;
}rrm_l2_get_snr_report_t;

/*!  \ rrm_l2_snr_report
 *   \brief This struct contains layer2 SNR report parameters info
 *   \param ue_index denotes the index of UE
 *   \param averageULSINR Denotes average uplink sinr value
 *   \param averageDLSNR Denotes average downlink sinr value
 *   \param timingAdvanceValue Denotes timing advance value
 */
typedef struct _rrm_l2_snr_report
{
    rrm_ue_index_t              ue_index;
    U8                          averageULSINR;
    U8                          averageDLSNR;
    U16                         timingAdvanceValue;
}rrm_l2_snr_report_t;


/*
 *Start: added for Load indication feature
 */
#define PLATFORM_RRM_LOAD_INDICATON  1
#define PLATFORM_RRM_UPDATE_BACKHAUL 2 


/* Fix for MAC API 12 Bytes header */

/* DYNAMIC ICIC CHANGES START   */
/*! \fn rrm_return_et rrm_send_mac_config_req(rrm_mac_config_req_t *p_mac_config_req, U16 src_module_id, U16 transaction_id)
 *  \brief Add CSPL and INTF header and send the message to MAC
 *  \param p_mac_config_req
 *  \param src_module_id 
 *  \param transaction_id 
 *  \param cell_index
 */
rrm_return_et
rrm_send_mac_config_req
(
  RrmMacCellConfigReq *p_mac_config_req,/* Pointer to API specific information. */
 U16 src_module_id,  /* Source module identifier */
 U16 transaction_id, /* Interface transaction identifier */
 U8  cell_index
 );

/*! \fn rrm_return_et rrm_send_mac_kpi_req(rrm_mac_config_kpi_stats_req *l2_kpi_stats_req, U16 src_module_id, U16 transaction_id) 
 *  \brief Add CSPL and INTF header and send the message to MAC
 *  \param l2_kpi_stats_req 
 *  \param src_module_id 
 *  \param transaction_id
 *  \param cell_index
 */
rrm_return_et
rrm_send_mac_kpi_req
(
  MacConfigureKpiStatsReqParams *l2_kpi_stats_req,/* Pointer to API specific information. */
 U16 src_module_id,  /* Source module identifier */
 U16 transaction_id, /* Interface transaction identifier */
 U8  cell_index
 );
/*  DYNAMIC ICIC CHANGES START */
rrm_return_et
l2_reconfig_scheduler_process_msg
(
 rrm_void_t  *l2_reconf_scheduler_req,/* Pointer to API specific information. */
 U16         src_module_id,  /* Source module identifier */
 U16         transaction_id, /* Interface transaction identifier */
 U8          cell_index      /* cell index */
 );

/*  DYNAMIC ICIC CHANGES END   */

/***************************************************************************
 * Structure Definitions For API UE Reconfig Req 
 **************************************************************************/

/*! \rrm_mac_ecid_meas_report_resp_t
 *  \brief This struct is used for the response sent by MAC to RRM with the requested ECID meas quantifier report
 *  \param bitmask
 *  \param ue_index
 *  \param response_code
 *  \param type1_enb_RxTx_report
 *  \param type2_enb_RxTx_report
 *  \param AoA_report
 */
#define RRM_MAC_TYPE1_ENB_RXTX_REPORT_PRESENT   0x01
#define RRM_MAC_TYPE2_ENB_RXTX_REPORT_PRESENT   0x02
#define RRM_MAC_AOA_REPORT_PRESENT              0x04

/* RACH_OPTIMIZATION_CHANGES_START */
typedef struct _rrm_mac_config_perf_stats_req_t
{
    U32         duration;
    U32         periodicReporting;
    U32         uePerfStatsToReportBitMap;
    U32         cellPerfStatsToReportBitMap;
}rrm_mac_config_perf_stats_req_t;
/*API MAC_CONFIGURE_PERF_STATS_REQ*/

typedef struct _rrm_mac_config_perf_stats_cnf_t
{
    U32         response_code;
}rrm_mac_config_perf_stats_cnf_t;


/*API MAC_CONFIGURE_PERF_STATS_CNF*/
# define MAX_TA_RANGE       12

typedef struct _rach_succ_fail_perf_stats_t
{
    U32                             totalRcvdMsg3RandomAccessPreamblesCBRA;  
    U32                             totalRcvdMsg3RandomAccessPreamblesCFRA; 
    U32                             totalFailedRandomAccessPreamblesMsg2CBRA; 
    U32                             totalFailedRandomAccessPreamblesMsg2CFRA;  
    U32                             totalAllocatedRandomAccessPreambles;    
    U32                             totalSuccRandomAccessTA[MAX_TA_RANGE];  
    U32                             totalUnassignedFalseCFRA;  
    U32                             totalUnassignedSumCFRA;  
}rach_succ_fail_perf_stats_t;


#define MAX_QCI 9
typedef struct _cell_harq_perf_stats_t
{
    U32         totalDLPRBUsageforHarq[MAX_QCI]; 
    U32         totalDLPRBUsageforHarqRetx;    
}cell_harq_perf_stats_t;


#define MAX_CFI               4
#define MAX_CCE_UTIL_INDEX    20
typedef struct _pdcch_utilization_stats_t
{
    U32         total_pdcch_cce_util[MAX_CCE_UTIL_INDEX]; 
    U32         no_of_sub_frame_with_cce_util_more_than_zero; 
    U32         no_of_sub_frame_with_pdcch_cfi[MAX_CFI]; 
    U32         total_cce_avail;  
}pdcch_utilization_stats_t;


#define MAX_SRB 3
typedef struct _cell_prb_usage_perf_stats_t
{
    U32     total_ul_prb_usage_srb[MAX_SRB];
    U32     total_dl_prb_usage_srb[MAX_SRB];
    U32     total_dl_prb_usage_pcch;
    U32     total_dl_prb_usage_bcch;
    U32     total_dl_prb_available;
    U32     total_ul_prb_available;
}cell_prb_usage_perf_stats_t;


typedef struct _rrm_cell_perf_active_ue_dl_ul_stats_t
{
    U16    totalActiveUEsInDl;
    U16    totalActiveUEsInUl;
}rrm_cell_perf_active_ue_dl_ul_stats_t;

typedef struct _cell_mac_perf_stats_resp_t
{
    U64                                    durationTTI;/*SPR 18564 +-*/
    cell_harq_perf_stats_t                 cell_harq_perf_stats;
    rach_succ_fail_perf_stats_t            rach_succ_fail_perf_stats;
    pdcch_utilization_stats_t              pdcch_utilization_stats;
    U32                                    total_pucch_res_allocation_attempts;
    cell_prb_usage_perf_stats_t            cell_prb_usage_perf_stats;
    rrm_cell_perf_active_ue_dl_ul_stats_t  cell_total_active_ue_dl_ul_stats;
}cell_mac_perf_stats_resp_t;
typedef struct _rrm_mac_cell_perf_stats_ind_params_t
{
    cell_mac_perf_stats_resp_t     cell_mac_perf_stats_resp;
}rrm_mac_cell_perf_stats_ind_params_t;

/* API MAC_CELL_PERF_STATS_IND */
/* RACH_OPTIMIZATION_CHANGES_END */

/*! \fn     rrm_parse_mac_ecid_meas_report_resp
 *  \brief  This funtion parses the ECID meas report response from MAC
 *  \param  U8 *p_api_buf
 *  \param  U16 data_len
 *  \param  rrm_mac_ecid_meas_report_resp_t **p_api_ue_data
 */
rrm_return_et
rrm_parse_mac_ecid_meas_report_resp(
        U8                                *p_api_buf,
    /*SPR 17777 +-*/
                    MacRrmEcidMeasurementResp  **p_api_ue_data
        );
/* UE Positioning end */

/* 
 *END:Added for Radio Congestion Control Feature 
 */

/*! \fn         rrm_return_et
 *              rrm_l2_get_l2_ue_snr_report(U8 *p_api_buf,U16 data_len)
 *  \brief      This funtion will use to get the SINR ta response
 *				from L2
 *  \param  p_api_buf Buffer to store response
 *  \param  data_len  length of the data
 */
rrm_return_et 
rrm_l2_get_l2_ue_snr_report (
        U8 *p_api_buf
    /*SPR 17777 +-*/
        );

/*EICIC_TDD_CHANGES_START*/
rrm_void_t rrm_copy_tdd_bits
(
   U8  abs_size,
   U8 *p_abs_pattern
);

/*LTE_RRM_TM_SWITCH_START*/
/*! \fn         rrm_return_et
 *              rrm_mac_ue_periodic_stats_report(U8 *p_api_buf, U16 data_len,rrm_ue_mac_stats_report_t **)
 *  \brief      This funtion will use to get the SINR ta response
 *                              from L2
 *  \param  p_api_buf Buffer to store response
 *  \param  data_len length of the data
 */
rrm_return_et
rrm_mac_ue_periodic_stats_report (
        U8 *p_api_buf,
    /*SPR 17777 +-*/
        rrm_ue_mac_stats_report_t **
        );
/*LTE_RRM_TM_SWITCH_END*/

/*EICIC_TDD_CHANGES_START*/
/*U8
rrm_find_abs_frame_size(tdd_frame_structure_t *p_physical_layer_param_tdd_frame_structure);*/
/*EICIC_TDD_CHANGES_END*/

/*! \fn         rrm_return_et
 *              rrm_mac_ue_periodic_stats_report(U8 *p_api_buf, U16 data_len,rrm_ue_mac_stats_report_t **)
 *  \brief      This funtion will use to get the SINR ta response
 *                              from L2
 *  \param  p_api_buf Buffer to store response
 *  \param  data_len length of the data
 */
rrm_return_et
rrm_parse_mac_ue_reconfig_cnf( U8                 *p_api_buf,
    /*SPR 17777 +-*/
                               RrmMacUEReConfigCnf 
                                                  **p_api_ue_data );

rrm_return_et 
rrm_send_mac_ue_reconfig_req
(
 RrmMacUeReconfigReq *p_rrm_mac_ue_reconfig_req,/* Pointer to API specific information. */
 U16 src_module_id,  /* Source module identifier */
 U16 transaction_id, /* Interface transaction identifier */
 U8  cell_index      /* cell_index */
 );

/* eICIC_PHASE_1_2_CHANGES_START */
rrm_return_et build_and_send_l2_ue_reconfig_req_for_victim 
(
     rrm_cell_index_t     cell_index ,
     rrm_ue_index_t       *p_ue_index_list,
     U32                  num_of_ues,
     U8                   dl_interference_info
);

/* SPR 21251 +- */
/* eICIC_PHASE_1_2_CHANGES_END */

/* RACH_OPTIMIZATION_CHANGES_START */
rrm_return_et
l2_perf_stats_req
(
 rrm_void_t  *l2_perf_stats_req,/* Pointer to API specific information. */
 U16         src_module_id,  /* Source module identifier */
 U16         transaction_id, /* Interface transaction identifier */
 U8          cell_index      /* cell index */
 );
/* RACH_OPTIMIZATION_CHANGES_END */
#endif /*! _RRM_L2_INTF_H_ */
