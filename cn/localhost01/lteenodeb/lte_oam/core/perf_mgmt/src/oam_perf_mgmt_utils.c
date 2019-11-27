/*************************************************************************************
 * File Name    : oam_perf_mgmt_utils.c
 * Description : This file contains the periodic and non periodic KPI , Stats fetching 
 *               and reset functionality of Performence management module.
 * Revision History :
 * Date            Author          Reference       Comments
 * ---------       -------------   -----------     -------------------------------
 * September,2012  Neeraj Kumar    OAM Rel 2.1     Incorporated changes for new API
 *                                                for reset kpi and stats. 
 *
 * June,2013       Neeraj Kumar    OAM Rel 2.1     Incorporated changes for TR SPV functionality.
 * September,2013  Raveen Sharma   CSR 51606 Fix   Code Changes for CSR Fix
 * October,2013    Neeraj Kumar    CSR 55318 Fix   Code Changes for CSR Fix
 * April,2014      Jyotsana Sharma CSR 10610 Fix   Code Changes for CSR Fix
 * May,2014       Shalu Sagar     Klockwork Fix   Code Changes for Klockwork Warnings                                     
 * 
 *
 * May, 2014	   Swetank         SPR 10566 Fix   Code Changes
 * May 2014	   Swetank	   SPR 10565 Fix  Code Changes	
 * June 2014       Swetank         SPR 10987 Fix   Enhancement for new KPI support
 * May 2014        Swetank	   SPR 10023 Fix   Modified code for handling multiple batches of
 *				                   PDCP messages for KPI.
 * May, 2014	   Swetank        SPR 10566 Fix  Code Changes
 * May 2014	   	   Swetank	      SPR 10565 Fix  Code Changes	
 * May 2014	       Swetank	      SPR 11120 Fix  Code Changes	
 * July,2014       Aditi Singh      SPR 9620 Fix        Guard timer Added
 * July 2014	   Swetank	      SPR 12844 Fix         Code Changes	
 * July 2014	   Swetank	      SPR 12822 Fix         Code Changes	
 * July 2014	   NITIN 	      transaction ID Fixes      Code Changes	
 * Aug 20144	   NITIN 	      global transaction changes  Code Changes
 * June 2014	   Swetank	      SPR 11402 Fix  Code Changes	
 * July 2014	   Swetank	      SPR 12814 Fix  Code Changes	
 * Nov 2014       Karuna Bhardwaj   eICIC Enhancement
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 *************************************************************************************/
#include<oam_proto.h>
#include<oam_types.h>
#include<oam_defines.h>
#include<oam_qcli_proto.h>
#include<oam_perf_mgmt.h>
#include<oam_config_tcb.h>
#include<oam_l2_if.h>
#include<oam_s1ap_il_composer.h>
#include<oam_s1ap_il_parser.h>
#include<oam_x2ap_il_composer.h>
#include<oam_x2ap_il_parser.h>
#include<oam_rrc_il_composer.h>
#include<oam_rrc_il_parser.h>
#include<oam_rrm_parser.h>
#include<oam_sim_utils.h>
#include<oam_sim.h>
#include<oam_fault_mgmt.h>
#include<oam_rrm_composer.h>
#include<lteCommonStatsManager.h>
#include<oam_fm_alarm_defs.h>
#include<dirent.h>

/* CSR 57877 FIx Start */
#include <oam_mgmt_intf_api.h>
/* CSR 57877 FIx End*/

/* LWA changes start*/
/* LWA changes end*/

#if defined(LIBXML_WRITER_ENABLED) && defined(LIBXML_OUTPUT_ENABLED)
#define MY_ENCODING "UTF-8"
#endif
/* SPR 15018 START */
#define NUM_BUFF 100
/* SPR 15018 END */
#define OAM_RRC_KPI_MAP  63 
/*SPR 21941 Start*/
extern lower_layer_init_info_t l2_comm_info[OAM_MAX_NUM_L2_INSTANCE];
/*SPR 21941 End*/
/* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
#include<rrc_m2apOam_intf.h>
#include<oam_m2ap_il_composer.h>
#include<oam_m2ap_il_parser.h>
#endif
/* EMBMS Changes End */


/* Global Variable */
UInt8 g_config_lower_layer_resp = OAM_TWO;
UInt8 g_cli_kpi_config_flag = OAM_FALSE;
/*SPR 20171 Unused Counter */
/* SPR_12955_CHANGES */
extern UInt8 cli_flag;
/*SPR 10566 FIX*/
extern QLOCK pm_mutex_lock;    
/*SPR 10566 FIX*/
/* CSR No 00055803 Start */
UInt8 g_resp_sent_user_interface = OAM_ZERO;
/* CSR No 00055803 End */

/* SPR 15491 START */
extern oam_set_config_req_t oam_set_config_req;
/* SPR 15491 END*/

oam_cli_reset_flag                          cli_reset_flag;
oam_pm_rrc_reset_t                          pm_rrc_reset;
/*spr 19296 fix start*/
oam_mac_kpi_info_t                              mac_kpi_info;
/*spr 19296 fix end*/
oam_pdcp_kpi_info_t                         pdcp_kpi_info;
/* SPR_11231_START */
oam_pm_pdcp_kpi_thp_data_t g_oam_pm_pdcp_kpi_thp_data;
oam_pdcp_kpi_ind_handler_t g_pdcp_kpi_ind_handler;
/* SPR 14019 FIX Start */
#ifdef PERF_STATS
/* SPR 14019 FIX End */
oam_pdcp_perf_stats_ind_handler_t g_oam_pdcp_perf_stats_ind_handler;
/* SPR 14019 FIX Start */
#endif
/* SPR 14019 FIX End */
/* SPR_11231_END*/
/*spr 19296 fix start*/
oam_mac_kpi_ind_handler_t                   g_mac_kpi_ind_handler;
/*      
 ** DESCRIPTION: ConsolidatedMacThpStatsInd is head pointer to link list containing data for API 
 ** MAC_KPI_THP_STATS_IND
 **/
oam_mac_thp_batch_node *ConsolidatedMacThpStatsInd = OAM_NULL;

/*
 ** DESCRIPTION: ConsolidatedGetMacThpStats is head pointer to link list containing data for API 
 ** MAC_GET_KPI_THP_STATS_CNF
 **/

oam_mac_thp_batch_node *ConsolidatedGetMacThpStats = OAM_NULL;
/*spr 19296 fix end*/


oam_l2_direct_stats_handler_t               oam_mac_direct_stats_hndlr;
oam_l2_direct_stats_handler_t               oam_pdcp_direct_stats_hndlr;
oam_pm_mac_kpi_stats_ind_params_name_t      g_mac_kpi_stats_ind_params_name;
oam_config_param_st                         oam_config;
oam_set_config_req_t                        oam_set_config_req;
oam_pdcp_kpi_stats_ind_params_name_t        g_pdcp_kpi_stats_ind_params_name;
/* SPR 18861 */
UInt32 g_pm_xml_total_element_count_offset = 0; 
UInt32 g_pm_xml_mac_param_start = 0;
UInt32 g_pm_xml_pdcp_param_start = 0;
/* SPR 18861 */

SInt32 Config_Flag = OAM_ZERO;

UInt8 temp_buff_mcc_n_mnc[100];
UInt8 temp_target_str[7];
/* + SPR 17439 */
static void oam_pm_write_data_to_rrm_file(void);
/* - SPR 17439 */

extern oam_pm_xml_params_t g_pm_xml_params;
extern oam_cm_cntxt_t *p_cm_cntxt;
extern oam_prov_req_t oam_prov_req;

/* Static Variables and Functions */
/* SPR 16807 Fix Start */
char *g_pm_xml_file_names[MAX_PM_XML_FILE_NAMES] = {OAM_ZERO};
/* SPR 16807 Fix End */
/* SPR_12747_CHANGES */

LOCAL UInt16 elements_in_pm_xml_mac_param_name = OAM_ZERO;
LOCAL UInt16 elements_in_pm_xml_pdcp_param_name = OAM_ZERO;
LOCAL UInt16 elements_in_pm_xml_param_name = OAM_ZERO;
LOCAL UInt16 elements_in_pm_xml_conn_st_param_name = OAM_ZERO;
LOCAL UInt16 elements_in_pm_xml_erab_param_name = OAM_ZERO;
LOCAL UInt16 elements_in_pm_xml_paging_param_name = OAM_ZERO;
LOCAL UInt16 elements_in_pm_xml_s1sig_param_name = OAM_ZERO;
LOCAL UInt16 elements_in_pm_xml_ho_param_name = OAM_ZERO;
LOCAL UInt16 pm_total_elements_in_xml = OAM_ONE;
LOCAL UInt32 job_id = OAM_ONE;
LOCAL oam_rrc_kpi_info_t                    rrc_kpi_info;
LOCAL oam_rrm_kpi_info_t                    rrm_kpi_info;
LOCAL oam_rrc_get_cell_stats_resp_name_t    rrc_get_cell_stats_resp_name;
LOCAL oam_handle_multiple_config_resp_t      oam_handle_multiple_config_resp;
extern oam_rrm_xml_params_t g_pm_rrm_xml_params;
LOCAL char *pm_xml_param_value[PM_XML_MAX_PARAM_VALUE];
LOCAL char *pm_rrm_xml_param_value[PM_XML_MAX_PARAM_VALUE];
LOCAL xmlTextWriterPtr writer = OAM_NULL;
LOCAL xmlTextWriterPtr rrm_writer = OAM_NULL;
LOCAL UInt16 pm_total_elements_in_rrm_xml = OAM_ONE;
LOCAL UInt16 elements_in_pm_rrm_xml_param_name = OAM_ZERO;

LOCAL oam_return_et oam_get_pm_xml_filename(char *pm_xml_filename);
LOCAL oam_return_et oam_get_rrm_pm_xml_filename(char *pm_xml_filename);
/* + SPR 17439 */
LOCAL void oam_pm_xml_file_close(void);
LOCAL oam_return_et oam_start_periodic_upload_time(char *uploadTimeStr);
LOCAL void oam_start_periodic_upload_interval_timer(void);
LOCAL oam_return_et oam_pm_xml_start_pm_reporting(void);
/* SPR_12955_FIXES */
LOCAL void oam_fill_pm_xml_params_and_config_lower_layers(UInt16 trans_id);
/* SPR_12955_FIXES */
LOCAL void oam_pm_xml_init_info(void);
LOCAL void oam_send_file_upload_request(void);

/* SPR 15018 START */
void oam_write_xml_measInfo(UInt8 *endTime);
void oam_calculate_endTime(UInt8 *endTime);
/* SPR 15018 END */

LOCAL oam_return_et oam_pm_rrm_xml_file_close(void);
LOCAL void oam_pm_write_rrm_xml_param_name(xmlTextWriter *rrm_writer, char **value_xml_param);
LOCAL oam_return_et oam_pm_rrm_xml_file_open(void);
/* - SPR 17439 */

LOCAL oam_return_et oam_pm_xml_ftp_server_upload(UInt32 *curl_code);
LOCAL void oam_pm_mem_free_of_xml_file_names(UInt16 count);
LOCAL oam_return_et  oam_pm_get_file_names(UInt16 *xml_file_count, oam_error_code_et *p_error_code);
UInt8 pm_xml_cell_id_to_read = OAM_ZERO;
#define M_OAM_ONE -1
/*
 * DESCRIPTION: ConsolidatedPdcpThpStatsInd is head pointer to link list containing data for API 
 * PDCP_KPI_THP_STATS_IND
 */
oam_pdcp_thp_batch_node *ConsolidatedPdcpThpStatsInd = OAM_NULL;
/*
 * DESCRIPTION: ConsolidatedGetPdcpThpStats is head pointer to link list containing data for API 
 * PDCP_GET_KPI_THP_STATS_CNF
 */
oam_pdcp_thp_batch_node *ConsolidatedGetPdcpThpStats = OAM_NULL;
/*
 * DESCRIPTION: ConsolidatedPdcpUePerfStats is head pointer to link list containing data for API 
 * PDCP_GET_UE_PERF_STATS_CNF
 */
/* SPR 14019 FIX Start */
#ifdef PERF_STATS
/* SPR 14019 FIX End */
oam_pdcp_ue_perf_stats_batch_node *ConsolidatedPdcpUePerfStats = OAM_NULL;
/* SPR 14019 FIX Start */
#endif
/* SPR 14019 FIX End */

/* SPR 15018 START */
UInt8 endTime_buffer[100] = {OAM_ZERO};
/* SPR 15018 END */

char *pm_xml_rrm_param_name[]={
	"num_of_admitted_csg_user",
	"num_of_admitted_non_csg_user",
	"num_of_ue_admission_success",
	"num_of_ue_admission_fail",
	"num_of_erb_setup_success",
	"num_of_erb_setup_fail",
	"num_of_erb_modify_success",
	"num_of_erb_modify_fail",
	"num_of_erb_release_success",
	"num_of_erb_release_fail",
	"avg_dl_allocated_gbr_prb",
	"avg_ul_allocated_gbr_prb",
	"dl_allocated_ngbr_prb",
	"ul_allocated_ngbr_prb",
	"num_of_geran_ho_attempt",
	"num_of_geran_ho_fail",
	"num_of_utran_ho_attempt",
	"num_of_utran_ho_fail",
	"num_of_eutran_ho_attempt",
	"num_of_eutran_ho_fail",
	"num_of_geran_hi_success",
	"num_of_geran_hi_fail",
	"num_of_utran_hi_success",
	"num_of_utran_hi_fail",
	"num_of_eutran_hi_success",
	"num_of_eutran_hi_fail",
	"num_of_enb_init_ho_csg_usr",
	"num_of_enb_init_ho_non_csg_usr",
	"num_of_enb_init_ue_release",
	"num_pucch_res_alloc_attempts",
	"num_of_sr_res_alloc_fail",
	"num_of_sr_cqi_alloc_fail",
	"total_cell_unavailable_time",
	/* BUG_FIXED_12710 */
	"num_of_ca_ue",
	/* BUG_FIXED_12710 */
	OAM_NULL
};





char *pm_xml_mac_param_name[]={
	"RRU.PrbDl.QCI1",    
	"RRU.PrbDl.QCI2",    
	"RRU.PrbDl.QCI3",    
	"RRU.PrbDl.QCI4",    
	"RRU.PrbDl.QCI5",    
	"RRU.PrbDl.QCI6",    
	"RRU.PrbDl.QCI7",    
	"RRU.PrbDl.QCI8",    
	"RRU.PrbDl.QCI9",    

	"RRU.PrbUl.QCI1",    
	"RRU.PrbUl.QCI2",    
	"RRU.PrbUl.QCI3",    
	"RRU.PrbUl.QCI4",    
	"RRU.PrbUl.QCI5",    
	"RRU.PrbUl.QCI6",    
	"RRU.PrbUl.QCI7",    
	"RRU.PrbUl.QCI8",    
	"RRU.PrbUl.QCI9",    

	"RRU.PrbTotDl",
	"RRU.PrbTotUl",

	"DRB.UEActiveUl.QCI1",
	"DRB.UEActiveUl.QCI2",
	"DRB.UEActiveUl.QCI3",
	"DRB.UEActiveUl.QCI4",
	"DRB.UEActiveUl.QCI5",
	"DRB.UEActiveUl.QCI6",
	"DRB.UEActiveUl.QCI7",
	"DRB.UEActiveUl.QCI8",
	"DRB.UEActiveUl.QCI9",

	"DRB.UEActiveDl.QCI1",
	"DRB.UEActiveDl.QCI2",
	"DRB.UEActiveDl.QCI3",
	"DRB.UEActiveDl.QCI4",
	"DRB.UEActiveDl.QCI5",
	"DRB.UEActiveDl.QCI6",
	"DRB.UEActiveDl.QCI7",
	"DRB.UEActiveDl.QCI8",
	"DRB.UEActiveDl.QCI9",

	"DRB.UEActiveUl.Sum",
	"DRB.UEActiveDl.Sum",

	"RRU.RachPreambleDedMean",
	"RRU.RachPreambleAMean",
	"RRU.RachPreambleBMean",

	"DRB.PdcpSduDelayDl.QCI1",
	"DRB.PdcpSduDelayDl.QCI2",
	"DRB.PdcpSduDelayDl.QCI3",
	"DRB.PdcpSduDelayDl.QCI4",
	"DRB.PdcpSduDelayDl.QCI5",
	"DRB.PdcpSduDelayDl.QCI6",
	"DRB.PdcpSduDelayDl.QCI7",
	"DRB.PdcpSduDelayDl.QCI8",
	"DRB.PdcpSduDelayDl.QCI9",
	"DRB.PdcpSduDelayDl.Sum",
/* SPR 23209 FIXED START */
    "RRU.OP1.ID",    
    "RRU.OP1.PrbDl.QCI1",    
    "RRU.OP1.PrbDl.QCI2",    
    "RRU.OP1.PrbDl.QCI3",    
    "RRU.OP1.PrbDl.QCI4",    
    "RRU.OP1.PrbDl.QCI5",    
    "RRU.OP1.PrbDl.QCI6",    
    "RRU.OP1.PrbDl.QCI7",    
    "RRU.OP1.PrbDl.QCI8",    
    "RRU.OP1.PrbDl.QCI9",    
    "RRU.OP1.PrbUl.QCI1",    
    "RRU.OP1.PrbUl.QCI2",    
    "RRU.OP1.PrbUl.QCI3",    
    "RRU.OP1.PrbUl.QCI4",    
    "RRU.OP1.PrbUl.QCI5",    
    "RRU.OP1.PrbUl.QCI6",    
    "RRU.OP1.PrbUl.QCI7",    
    "RRU.OP1.PrbUl.QCI8",    
    "RRU.OP1.PrbUl.QCI9",    
    "DRB.OP1.UEActiveUl.QCI1",
    "DRB.OP1.UEActiveUl.QCI2",
    "DRB.OP1.UEActiveUl.QCI3",
    "DRB.OP1.UEActiveUl.QCI4",
    "DRB.OP1.UEActiveUl.QCI5",
    "DRB.OP1.UEActiveUl.QCI6",
    "DRB.OP1.UEActiveUl.QCI7",
    "DRB.OP1.UEActiveUl.QCI8",
    "DRB.OP1.UEActiveUl.QCI9",
    "DRB.OP1.UEActiveDl.QCI1",
    "DRB.OP1.UEActiveDl.QCI2",
    "DRB.OP1.UEActiveDl.QCI3",
    "DRB.OP1.UEActiveDl.QCI4",
    "DRB.OP1.UEActiveDl.QCI5",
    "DRB.OP1.UEActiveDl.QCI6",
    "DRB.OP1.UEActiveDl.QCI7",
    "DRB.OP1.UEActiveDl.QCI8",
    "DRB.OP1.UEActiveDl.QCI9",
    "RRU.OP2.ID",    
    "RRU.OP2.PrbDl.QCI1",    
    "RRU.OP2.PrbDl.QCI2",    
    "RRU.OP2.PrbDl.QCI3",    
    "RRU.OP2.PrbDl.QCI4",    
    "RRU.OP2.PrbDl.QCI5",    
    "RRU.OP2.PrbDl.QCI6",    
    "RRU.OP2.PrbDl.QCI7",    
    "RRU.OP2.PrbDl.QCI8",    
    "RRU.OP2.PrbDl.QCI9",    
    "RRU.OP2.PrbUl.QCI1",    
    "RRU.OP2.PrbUl.QCI2",    
    "RRU.OP2.PrbUl.QCI3",    
    "RRU.OP2.PrbUl.QCI4",    
    "RRU.OP2.PrbUl.QCI5",    
    "RRU.OP2.PrbUl.QCI6",    
    "RRU.OP2.PrbUl.QCI7",    
    "RRU.OP2.PrbUl.QCI8",    
    "RRU.OP2.PrbUl.QCI9",    
    "DRB.OP2.UEActiveUl.QCI1",
    "DRB.OP2.UEActiveUl.QCI2",
    "DRB.OP2.UEActiveUl.QCI3",
    "DRB.OP2.UEActiveUl.QCI4",
    "DRB.OP2.UEActiveUl.QCI5",
    "DRB.OP2.UEActiveUl.QCI6",
    "DRB.OP2.UEActiveUl.QCI7",
    "DRB.OP2.UEActiveUl.QCI8",
    "DRB.OP2.UEActiveUl.QCI9",
    "DRB.OP2.UEActiveDl.QCI1",
    "DRB.OP2.UEActiveDl.QCI2",
    "DRB.OP2.UEActiveDl.QCI3",
    "DRB.OP2.UEActiveDl.QCI4",
    "DRB.OP2.UEActiveDl.QCI5",
    "DRB.OP2.UEActiveDl.QCI6",
    "DRB.OP2.UEActiveDl.QCI7",
    "DRB.OP2.UEActiveDl.QCI8",
    "DRB.OP2.UEActiveDl.QCI9",
    "RRU.OP3.ID",    
    "RRU.OP3.PrbDl.QCI1",    
    "RRU.OP3.PrbDl.QCI2",    
    "RRU.OP3.PrbDl.QCI3",    
    "RRU.OP3.PrbDl.QCI4",    
    "RRU.OP3.PrbDl.QCI5",    
    "RRU.OP3.PrbDl.QCI6",    
    "RRU.OP3.PrbDl.QCI7",    
    "RRU.OP3.PrbDl.QCI8",    
    "RRU.OP3.PrbDl.QCI9",    
    "RRU.OP3.PrbUl.QCI1",    
    "RRU.OP3.PrbUl.QCI2",    
    "RRU.OP3.PrbUl.QCI3",    
    "RRU.OP3.PrbUl.QCI4",    
    "RRU.OP3.PrbUl.QCI5",    
    "RRU.OP3.PrbUl.QCI6",    
    "RRU.OP3.PrbUl.QCI7",    
    "RRU.OP3.PrbUl.QCI8",    
    "RRU.OP3.PrbUl.QCI9",    
    "DRB.OP3.UEActiveUl.QCI1",
    "DRB.OP3.UEActiveUl.QCI2",
    "DRB.OP3.UEActiveUl.QCI3",
    "DRB.OP3.UEActiveUl.QCI4",
    "DRB.OP3.UEActiveUl.QCI5",
    "DRB.OP3.UEActiveUl.QCI6",
    "DRB.OP3.UEActiveUl.QCI7",
    "DRB.OP3.UEActiveUl.QCI8",
    "DRB.OP3.UEActiveUl.QCI9",
    "DRB.OP3.UEActiveDl.QCI1",
    "DRB.OP3.UEActiveDl.QCI2",
    "DRB.OP3.UEActiveDl.QCI3",
    "DRB.OP3.UEActiveDl.QCI4",
    "DRB.OP3.UEActiveDl.QCI5",
    "DRB.OP3.UEActiveDl.QCI6",
    "DRB.OP3.UEActiveDl.QCI7",
    "DRB.OP3.UEActiveDl.QCI8",
    "DRB.OP3.UEActiveDl.QCI9",
    "RRU.OP4.ID",    
    "RRU.OP4.PrbDl.QCI1",    
    "RRU.OP4.PrbDl.QCI2",    
    "RRU.OP4.PrbDl.QCI3",    
    "RRU.OP4.PrbDl.QCI4",    
    "RRU.OP4.PrbDl.QCI5",    
    "RRU.OP4.PrbDl.QCI6",    
    "RRU.OP4.PrbDl.QCI7",    
    "RRU.OP4.PrbDl.QCI8",    
    "RRU.OP4.PrbDl.QCI9",    
    "RRU.OP4.PrbUl.QCI1",    
    "RRU.OP4.PrbUl.QCI2",    
    "RRU.OP4.PrbUl.QCI3",    
    "RRU.OP4.PrbUl.QCI4",    
    "RRU.OP4.PrbUl.QCI5",    
    "RRU.OP4.PrbUl.QCI6",    
    "RRU.OP4.PrbUl.QCI7",    
    "RRU.OP4.PrbUl.QCI8",    
    "RRU.OP4.PrbUl.QCI9",    
    "DRB.OP4.UEActiveUl.QCI1",
    "DRB.OP4.UEActiveUl.QCI2",
    "DRB.OP4.UEActiveUl.QCI3",
    "DRB.OP4.UEActiveUl.QCI4",
    "DRB.OP4.UEActiveUl.QCI5",
    "DRB.OP4.UEActiveUl.QCI6",
    "DRB.OP4.UEActiveUl.QCI7",
    "DRB.OP4.UEActiveUl.QCI8",
    "DRB.OP4.UEActiveUl.QCI9",
    "DRB.OP4.UEActiveDl.QCI1",
    "DRB.OP4.UEActiveDl.QCI2",
    "DRB.OP4.UEActiveDl.QCI3",
    "DRB.OP4.UEActiveDl.QCI4",
    "DRB.OP4.UEActiveDl.QCI5",
    "DRB.OP4.UEActiveDl.QCI6",
    "DRB.OP4.UEActiveDl.QCI7",
    "DRB.OP4.UEActiveDl.QCI8",
    "DRB.OP4.UEActiveDl.QCI9",
    "RRU.OP5.ID",    
    "RRU.OP5.PrbDl.QCI1",    
    "RRU.OP5.PrbDl.QCI2",    
    "RRU.OP5.PrbDl.QCI3",    
    "RRU.OP5.PrbDl.QCI4",    
    "RRU.OP5.PrbDl.QCI5",    
    "RRU.OP5.PrbDl.QCI6",    
    "RRU.OP5.PrbDl.QCI7",    
    "RRU.OP5.PrbDl.QCI8",    
    "RRU.OP5.PrbDl.QCI9",    
    "RRU.OP5.PrbUl.QCI1",    
    "RRU.OP5.PrbUl.QCI2",    
    "RRU.OP5.PrbUl.QCI3",    
    "RRU.OP5.PrbUl.QCI4",    
    "RRU.OP5.PrbUl.QCI5",    
    "RRU.OP5.PrbUl.QCI6",    
    "RRU.OP5.PrbUl.QCI7",    
    "RRU.OP5.PrbUl.QCI8",    
    "RRU.OP5.PrbUl.QCI9",    
    "DRB.OP5.UEActiveUl.QCI1",
    "DRB.OP5.UEActiveUl.QCI2",
    "DRB.OP5.UEActiveUl.QCI3",
    "DRB.OP5.UEActiveUl.QCI4",
    "DRB.OP5.UEActiveUl.QCI5",
    "DRB.OP5.UEActiveUl.QCI6",
    "DRB.OP5.UEActiveUl.QCI7",
    "DRB.OP5.UEActiveUl.QCI8",
    "DRB.OP5.UEActiveUl.QCI9",
    "DRB.OP5.UEActiveDl.QCI1",
    "DRB.OP5.UEActiveDl.QCI2",
    "DRB.OP5.UEActiveDl.QCI3",
    "DRB.OP5.UEActiveDl.QCI4",
    "DRB.OP5.UEActiveDl.QCI5",
    "DRB.OP5.UEActiveDl.QCI6",
    "DRB.OP5.UEActiveDl.QCI7",
    "DRB.OP5.UEActiveDl.QCI8",
    "DRB.OP5.UEActiveDl.QCI9",
    "RRU.OP6.ID",    
    "RRU.OP6.PrbDl.QCI1",    
    "RRU.OP6.PrbDl.QCI2",    
    "RRU.OP6.PrbDl.QCI3",    
    "RRU.OP6.PrbDl.QCI4",    
    "RRU.OP6.PrbDl.QCI5",    
    "RRU.OP6.PrbDl.QCI6",    
    "RRU.OP6.PrbDl.QCI7",    
    "RRU.OP6.PrbDl.QCI8",    
    "RRU.OP6.PrbDl.QCI9",    
    "RRU.OP6.PrbUl.QCI1",    
    "RRU.OP6.PrbUl.QCI2",    
    "RRU.OP6.PrbUl.QCI3",    
    "RRU.OP6.PrbUl.QCI4",    
    "RRU.OP6.PrbUl.QCI5",    
    "RRU.OP6.PrbUl.QCI6",    
    "RRU.OP6.PrbUl.QCI7",    
    "RRU.OP6.PrbUl.QCI8",    
    "RRU.OP6.PrbUl.QCI9",    
    "DRB.OP6.UEActiveUl.QCI1",
    "DRB.OP6.UEActiveUl.QCI2",
    "DRB.OP6.UEActiveUl.QCI3",
    "DRB.OP6.UEActiveUl.QCI4",
    "DRB.OP6.UEActiveUl.QCI5",
    "DRB.OP6.UEActiveUl.QCI6",
    "DRB.OP6.UEActiveUl.QCI7",
    "DRB.OP6.UEActiveUl.QCI8",
    "DRB.OP6.UEActiveUl.QCI9",
    "DRB.OP6.UEActiveDl.QCI1",
    "DRB.OP6.UEActiveDl.QCI2",
    "DRB.OP6.UEActiveDl.QCI3",
    "DRB.OP6.UEActiveDl.QCI4",
    "DRB.OP6.UEActiveDl.QCI5",
    "DRB.OP6.UEActiveDl.QCI6",
    "DRB.OP6.UEActiveDl.QCI7",
    "DRB.OP6.UEActiveDl.QCI8",
    "DRB.OP6.UEActiveDl.QCI9",

/* SPR 23209 FIXED END */

	/* SPR 13492 Changes Start */
	"ERAB.SessionTimeUE",
	"ERAB.SessionTime.QCI1",
	"ERAB.SessionTime.QCI2",
	"ERAB.SessionTime.QCI3",
	"ERAB.SessionTime.QCI4",
	"ERAB.SessionTime.QCI5",
	"ERAB.SessionTime.QCI6",
	"ERAB.SessionTime.QCI7",
	"ERAB.SessionTime.QCI8",
	"ERAB.SessionTime.QCI9",
	"ERAB.SessionTime.Sum",
	/* SPR 13492 Changes End */

/*spr 19296 fix start*/
        "DRB.PdcpSduAirLossRateDl.QCI1",
        "DRB.PdcpSduAirLossRateDl.QCI2",
        "DRB.PdcpSduAirLossRateDl.QCI3",
        "DRB.PdcpSduAirLossRateDl.QCI4",
        "DRB.PdcpSduAirLossRateDl.QCI5",
        "DRB.PdcpSduAirLossRateDl.QCI6",
        "DRB.PdcpSduAirLossRateDl.QCI7",
        "DRB.PdcpSduAirLossRateDl.QCI8",
        "DRB.PdcpSduAirLossRateDl.QCI9",
        "DRB.PdcpSduAirLossRateDl.Sum",
/*spr 19296 fix end*/

	/* SPR 12670 Changes Start */
	"CARR.WBCQIDist.Bin0", /* 0-15 represent CQI value*/
	"CARR.WBCQIDist.Bin1",
	"CARR.WBCQIDist.Bin2",
	"CARR.WBCQIDist.Bin3",
	"CARR.WBCQIDist.Bin4",
	"CARR.WBCQIDist.Bin5",
	"CARR.WBCQIDist.Bin6",
	"CARR.WBCQIDist.Bin7",
	"CARR.WBCQIDist.Bin8",
	"CARR.WBCQIDist.Bin9",
	"CARR.WBCQIDist.Bin10",
	"CARR.WBCQIDist.Bin11",
	"CARR.WBCQIDist.Bin12",
	"CARR.WBCQIDist.Bin13",
	"CARR.WBCQIDist.Bin14",
	"CARR.WBCQIDist.Bin15",
	/* SPR 12670 Changes Start */
/*spr 19296 fix start*/
	"DRB.IPThpDl.QCI1",	
	"DRB.IPThpDl.QCI2",	
	"DRB.IPThpDl.QCI3",	
	"DRB.IPThpDl.QCI4",	
	"DRB.IPThpDl.QCI5",	
	"DRB.IPThpDl.QCI6",	
	"DRB.IPThpDl.QCI7",	
	"DRB.IPThpDl.QCI8",	
	"DRB.IPThpDl.QCI9",	

	"DRB.IPThpUl.QCI1",	
	"DRB.IPThpUl.QCI2",	
	"DRB.IPThpUl.QCI3",	
	"DRB.IPThpUl.QCI4",	
	"DRB.IPThpUl.QCI5",	
	"DRB.IPThpUl.QCI6",	
	"DRB.IPThpUl.QCI7",	
	"DRB.IPThpUl.QCI8",	
	"DRB.IPThpUl.QCI9",	

	"DRB.IpLateDL.QCI1",	
	"DRB.IpLateDL.QCI2",	
	"DRB.IpLateDL.QCI3",	
	"DRB.IpLateDL.QCI4",	
	"DRB.IpLateDL.QCI5",	
	"DRB.IpLateDL.QCI6",	
	"DRB.IpLateDL.QCI7",	
	"DRB.IpLateDL.QCI8",	
	"DRB.IpLateDL.QCI9",	
/*spr 19296 fix end*/
	OAM_NULL};

char *pm_xml_pdcp_param_name[]={
	"DRB.PdcpSduDropRateDl.QCI1",
	"DRB.PdcpSduDropRateDl.QCI2",
	"DRB.PdcpSduDropRateDl.QCI3",
	"DRB.PdcpSduDropRateDl.QCI4",
	"DRB.PdcpSduDropRateDl.QCI5",
	"DRB.PdcpSduDropRateDl.QCI6",
	"DRB.PdcpSduDropRateDl.QCI7",
	"DRB.PdcpSduDropRateDl.QCI8",
	"DRB.PdcpSduDropRateDl.QCI9",
	"DRB.PdcpSduDropRateDl.Sum",

/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/

	"DRB.PdcpSduLossRateUl.QCI1",
	"DRB.PdcpSduLossRateUl.QCI2",
	"DRB.PdcpSduLossRateUl.QCI3",
	"DRB.PdcpSduLossRateUl.QCI4",
	"DRB.PdcpSduLossRateUl.QCI5",
	"DRB.PdcpSduLossRateUl.QCI6",
	"DRB.PdcpSduLossRateUl.QCI7",
	"DRB.PdcpSduLossRateUl.QCI8",
	"DRB.PdcpSduLossRateUl.QCI9",
	"SRB.PdcpSduBitrateDl",
	"SRB.PdcpSduBitrateUl",

/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/

	"DRB.PdcpSduBitrateDl.QCI1",  
	"DRB.PdcpSduBitrateDl.QCI2",  
	"DRB.PdcpSduBitrateDl.QCI3",  
	"DRB.PdcpSduBitrateDl.QCI4",  
	"DRB.PdcpSduBitrateDl.QCI5",  
	"DRB.PdcpSduBitrateDl.QCI6",  
	"DRB.PdcpSduBitrateDl.QCI7",  
	"DRB.PdcpSduBitrateDl.QCI8",  
	"DRB.PdcpSduBitrateDl.QCI9",  

	"DRB.PdcpSduBitrateUl.QCI1",  
	"DRB.PdcpSduBitrateUl.QCI2",  
	"DRB.PdcpSduBitrateUl.QCI3",  
	"DRB.PdcpSduBitrateUl.QCI4",  
	"DRB.PdcpSduBitrateUl.QCI5",  
	"DRB.PdcpSduBitrateUl.QCI6",  
	"DRB.PdcpSduBitrateUl.QCI7",  
	"DRB.PdcpSduBitrateUl.QCI8",  
	"DRB.PdcpSduBitrateUl.QCI9",  

	"DRB.PdcpSduBitrateDlMax",
	"DRB.PdcpSduBitrateUlMax",
	OAM_NULL};


char *elements_in_pm_xml_mac_param_name_thp[]={
	OAM_NULL};

char *pm_xml_conn_st_param_name[]={
/*SPR 14383 START*/
#if ENUM_KPI_CAUSE_FLAG
    "RRC.ConnEstabAtt.0",
    "RRC.ConnEstabAtt.1",
    "RRC.ConnEstabAtt.2",
    "RRC.ConnEstabAtt.3",
    "RRC.ConnEstabAtt.4",
/* SPR 18861 */
    "RRC.ConnEstabAtt.5",
    "RRC.ConnEstabAtt.6",
    "RRC.ConnEstabAtt.7",
    "RRC.ConnEstabAtt.Sum",
/* SPR 18861 */
    "RRC.ConnEstabSucc.0",
    "RRC.ConnEstabSucc.1",
    "RRC.ConnEstabSucc.2",
    "RRC.ConnEstabSucc.3",
    "RRC.ConnEstabSucc.4",
/* SPR 18861 */
    "RRC.ConnEstabSucc.5",
    "RRC.ConnEstabSucc.6",
    "RRC.ConnEstabSucc.7",
    "RRC.ConnEstabSucc.Sum",
    "RRC.ConnEstabFail.0",
    "RRC.ConnEstabFail.1",
    "RRC.ConnEstabFail.2",
    "RRC.ConnEstabFail.3",
    "RRC.ConnEstabFail.4",
    "RRC.ConnEstabFail.5",
    "RRC.ConnEstabFail.6",
    "RRC.ConnEstabFail.7",
    "RRC.ConnEstabFail.Sum",
/* SPR 18861 */
#else
	"RRC.ConnEstabAtt.Emergency",
	"RRC.ConnEstabAtt.HighPriorityAccess",
	"RRC.ConnEstabAtt.MtAccess",
	"RRC.ConnEstabAtt.MoSignalling",
	"RRC.ConnEstabAtt.MoData",
/* SPR 18861 */
	"RRC.ConnEstabAtt.delayTolerantAccessV1020",
	"RRC.ConnEstabAtt.spare2",
	"RRC.ConnEstabAtt.spare1",
/* SPR 18861 */
	"RRC.ConnEstabSucc.Emergency",
	"RRC.ConnEstabSucc.HighPriorityAccess",
	"RRC.ConnEstabSucc.MtAccess",
	"RRC.ConnEstabSucc.MoSignalling",
	"RRC.ConnEstabSucc.MoData",
/* SPR 18861 */
	"RRC.ConnEstabSucc.delayTolerantAccessV1020",
	"RRC.ConnEstabSucc.spare2",
	"RRC.ConnEstabSucc.spare1",
    "RRC.ConnEstabFail.Emergency",
    "RRC.ConnEstabFail.HighPriorityAccess",
    "RRC.ConnEstabFail.MtAccess",
    "RRC.ConnEstabFail.MoSignalling",
    "RRC.ConnEstabFail.MoData",
    "RRC.ConnEstabFail.delayTolerantAccessV1020",
    "RRC.ConnEstabFail.spare2",
    "RRC.ConnEstabFail.spare1",
/* SPR 18861 */
#endif    
	/*SPR 14383 END*/
    "RRC.ConnEstabFaileNBCause.Congestion",
	"RRC.ConnEstabFaileNBCause.Unspecified",
	/*SPR 10987 FIX */
	"RRC.ConnEstabFaileNBCause.EnergySaving",
    /*SPR 14383 START*/
#if ENUM_KPI_CAUSE_FLAG
    "RRC.ConnReEstabAtt.0",
    "RRC.ConnReEstabAtt.1",
    "RRC.ConnReEstabAtt.2",
    "RRC.ConnReEstabSucc.0",
    "RRC.ConnReEstabSucc.1",
    "RRC.ConnReEstabSucc.2",
#else
    "RRC.ConnReEstabAtt.ReconfigurationFailure",
    "RRC.ConnReEstabAtt.HandoverFailure",
    "RRC.ConnReEstabAtt.OtherFailure",
    "RRC.ConnReEstabSucc.ReconfigurationFailure",
    "RRC.ConnReEstabSucc.HandoverFailure",
    "RRC.ConnReEstabSucc.OtherFailure",
#endif
    /*SPR 14383 END*/

    /*SPR 10987 FIX */
	/*SPR 10987 FIX */
	"RRC.ConnReEstabAtt.Sum",				
	/*SPR 10987 FIX */
	"RRC.ConnReEstabSucc.Sum",
	/*SPR 10987 FIX */
    /*SPR 19596 Start*/
#if ENUM_KPI_CAUSE_FLAG
	"RRC.ConnReEstabFail.0",
	"RRC.ConnReEstabFail.1",
	"RRC.ConnReEstabFail.2",
#else
    /*SPR 19596 End*/
	"RRC.ConnReEstabFail.ReconfigurationFailure",
	"RRC.ConnReEstabFail.HandoverFailure",	
	"RRC.ConnReEstabFail.OtherFailure",		
#endif /*SPR 19596*/
    
	"RRC.ConnReEstabFail.Sum",				
	/*SPR 10987 FIX */
	"RRC.ConnMean",
	"RRC.ConnMax",
    /*SPR 14383 START*/
#if ENUM_KPI_CAUSE_FLAG
    "RRC.ConnEstabTimeMean.0",
    "RRC.ConnEstabTimeMean.1",
    "RRC.ConnEstabTimeMean.2",
    "RRC.ConnEstabTimeMean.3",
    "RRC.ConnEstabTimeMean.4",
/* SPR 18861 */
    "RRC.ConnEstabTimeMean.5",
    "RRC.ConnEstabTimeMean.6",
    "RRC.ConnEstabTimeMean.7",
/* SPR 18861 */
    "RRC.ConnEstabTimeMax.0",
    "RRC.ConnEstabTimeMax.1",
    "RRC.ConnEstabTimeMax.2",
    "RRC.ConnEstabTimeMax.3",
    "RRC.ConnEstabTimeMax.4",
/* SPR 18861 */
    "RRC.ConnEstabTimeMax.5",
    "RRC.ConnEstabTimeMax.6",
    "RRC.ConnEstabTimeMax.7",
/* SPR 18861 */
#else
    "RRC.ConnEstabTimeMean.Emergency",
	"RRC.ConnEstabTimeMean.HighPriorityAccess",
	"RRC.ConnEstabTimeMean.MtAccess",
	"RRC.ConnEstabTimeMean.MoSignalling",
	"RRC.ConnEstabTimeMean.MoData",
/* SPR 18861 */
	"RRC.ConnEstabTimeMean.delayTolerantAccessV1020",
	"RRC.ConnEstabTimeMean.spare2",
	"RRC.ConnEstabTimeMean.spare1",
/* SPR 18861 */
	"RRC.ConnEstabTimeMax.Emergency",
	"RRC.ConnEstabTimeMax.HighPriorityAccess",
	"RRC.ConnEstabTimeMax.MtAccess",
	"RRC.ConnEstabTimeMax.MoSignalling",
	"RRC.ConnEstabTimeMax.MoData",
/* SPR 18861 */
	"RRC.ConnEstabTimeMax.delayTolerantAccessV1020",
	"RRC.ConnEstabTimeMax.spare2",
	"RRC.ConnEstabTimeMax.spare1",
/* SPR 18861 */
#endif    
	
    "UECNTX.RelReq.Sum",
	/*SPR 10987 FIX start*/
    /*SPR 19596 Start*/
#if ENUM_KPI_CAUSE_FLAG 
    "UECNTX.RelReq.1.0",
    "UECNTX.RelReq.1.1",
    "UECNTX.RelReq.1.2",
    "UECNTX.RelReq.1.3",
    "UECNTX.RelReq.1.4",
    "UECNTX.RelReq.1.5",
    "UECNTX.RelReq.1.6",
    "UECNTX.RelReq.1.7",
    "UECNTX.RelReq.1.8",
    "UECNTX.RelReq.1.9",
    "UECNTX.RelReq.1.10",
    "UECNTX.RelReq.1.11",
    "UECNTX.RelReq.1.12",
    "UECNTX.RelReq.1.13",
    "UECNTX.RelReq.1.14",
    "UECNTX.RelReq.1.15",
    "UECNTX.RelReq.1.16",
    "UECNTX.RelReq.1.17",
    "UECNTX.RelReq.1.18",
    "UECNTX.RelReq.1.19",
    "UECNTX.RelReq.1.20",
    "UECNTX.RelReq.1.21",
    "UECNTX.RelReq.1.22",
    "UECNTX.RelReq.1.23",
    "UECNTX.RelReq.1.24",
    "UECNTX.RelReq.1.25",
    "UECNTX.RelReq.1.26",
    "UECNTX.RelReq.1.27",
    "UECNTX.RelReq.1.28",
    "UECNTX.RelReq.1.29",
    "UECNTX.RelReq.1.30",
    "UECNTX.RelReq.1.31",
    "UECNTX.RelReq.1.32",
    "UECNTX.RelReq.1.33",
    "UECNTX.RelReq.1.34",
    "UECNTX.RelReq.1.35",
#else
    /*SPR 19596 End*/

	"UECNTX.RelReq.Unspecified",
	"UECNTX.RelReq.Tx2RelocOverallExpiry",
	"UECNTX.RelReq.SuccessfulHandover",
	"UECNTX.RelReq.ReleaseDueToEutranGeneratedReason",
	"UECNTX.RelReq.HandoverCancelled",
	"UECNTX.RelReq.PartialHandover",
	"UECNTX.RelReq.HoFailureInTargetEPCeNBOrTargetSystem",
	"UECNTX.RelReq.HoTargetNotAllowed",
	"UECNTX.RelReq.TS1RelocOverallExpiry",
	"UECNTX.RelReq.TS1RelocPrepExpiry",
	"UECNTX.RelReq.CellNotAvailable",
	"UECNTX.RelReq.UnknownTargetID",
	"UECNTX.RelReq.NoRadioResourcesAvailableInTargetCell",
	"UECNTX.RelReq.UnknownMmeUeS1apId",
	"UECNTX.RelReq.UnknownEnbUeS1apId",
	"UECNTX.RelReq.UnknownPairUeS1apId",  /*SPR 19596*/
	"UECNTX.RelReq.HandoverDesirableForRadioReason",
	"UECNTX.RelReq.TimeCriticalHandover",
	"UECNTX.RelReq.ResourceOptimisationHandover",
	"UECNTX.RelReq.ReduceLoadInServingCell",
 	"UECNTX.RelReq.UserInactivity",   /*SPR 19596*/
	"UECNTX.RelReq.RadioConnectionWithUeLost",
	"UECNTX.RelReq.LoadBalancingTauRequired",
	"UECNTX.RelReq.CsFallbackTriggered",
	"UECNTX.RelReq.UeNotAvailableForPsService",
	"UECNTX.RelReq.RadioResourcesNotAvailable",
	"UECNTX.RelReq.FailureInRadioInterfaceProcedure",
	"UECNTX.RelReq.InvalidQosCombination",
	"UECNTX.RelReq.InterratRedirection",
	"UECNTX.RelReq.InteractionWithOtherProcedure",
	"UECNTX.RelReq.UnknownERABID",
	"UECNTX.RelReq.MultipleERABIDInstances",
	"UECNTX.RelReq.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported",
	"UECNTX.RelReq.S1IntraSystemHandoverTriggered",
	"UECNTX.RelReq.S1InterSystemHandoverTriggered",
	"UECNTX.RelReq.X2HandoverTriggered",
 /*SPR 19596 Start*/

#endif


#if ENUM_KPI_CAUSE_FLAG 
    "UECNTX.RelReq.2.0",
    "UECNTX.RelReq.2.1",
#else
 /*SPR 19596 End*/

	"UECNTX.RelReq.TransportResourceUnavailable",
	"UECNTX.RelReq.TransportUnspecified",
 /*SPR 19596 Start*/

#endif
#if ENUM_KPI_CAUSE_FLAG 
    "UECNTX.RelReq.3.0",
    "UECNTX.RelReq.3.1",
    "UECNTX.RelReq.3.2",
    "UECNTX.RelReq.3.3",
#else
 /*SPR 19596 End*/

	"UECNTX.RelReq.NormalRelease",
	"UECNTX.RelReq.AuthenticationFailure",
	"UECNTX.RelReq.Detach",
	"UECNTX.RelReq.Unspecified4",
 /*SPR 19596 Start*/

#endif    
#if ENUM_KPI_CAUSE_FLAG 
    "UECNTX.RelReq.4.0",
    "UECNTX.RelReq.4.1",
    "UECNTX.RelReq.4.2",
    "UECNTX.RelReq.4.3",
    "UECNTX.RelReq.4.4",
    "UECNTX.RelReq.4.5",
    "UECNTX.RelReq.4.6",
#else
 /*SPR 19596 End*/

	"UECNTX.RelReq.TransferSyntaxError",
	"UECNTX.RelReq.AbstractSyntaxErrorReject",
	"UECNTX.RelReq.AbstractSyntaxErrorIgnoreAndNotify",
	"UECNTX.RelReq.MessageNotCompatibleWithReceiverState",
	"UECNTX.RelReq.SemanticError",
	"UECNTX.RelReq.AbstractSyntaxErrorFalselyConstructedMessage",
	"UECNTX.RelReq.Unspecified_1",
#endif    /*SPR 19596*/

	/*SPR 10987 FIX end*/
 /*SPR 19596 Start*/

    /*SPR 14383 END*/
#if ENUM_KPI_CAUSE_FLAG
    "UECNTX.RelReq.5.0",
    "UECNTX.RelReq.5.1",
    "UECNTX.RelReq.5.2",
    "UECNTX.RelReq.5.3",
    "UECNTX.RelReq.5.4",
    "UECNTX.RelReq.5.5",
 /*SPR 19596 End*/

#else
    /*SPR 19596 Code Deleted*/

    "UECNTX.RelReq.ControlProcessingOverload",
	"UECNTX.RelReq.NotEnoughUserPlaneProcessingResources",
 /*SPR 19596 Start*/

    "UECNTX.RelReq.HardwareFailure",
    "UECNTX.RelReq.OMIntervention",
 /*SPR 19596 End*/

	"UECNTX.RelReq.S1APUnspecified",
	"UECNTX.RelReq.S1APUnknownPLMN",
#endif     /*SPR 19596*/

	"UECNTX.RelSuccNbr",

	OAM_NULL};

char *pm_xml_erab_param_name[]={
"ERAB.EstabInitAttNbr.QCI1",
	"ERAB.EstabInitAttNbr.QCI2",
	"ERAB.EstabInitAttNbr.QCI3",
	"ERAB.EstabInitAttNbr.QCI4",
	"ERAB.EstabInitAttNbr.QCI5",
	"ERAB.EstabInitAttNbr.QCI6",
	"ERAB.EstabInitAttNbr.QCI7",
	"ERAB.EstabInitAttNbr.QCI8",
	"ERAB.EstabInitAttNbr.QCI9",
	"ERAB.EstabInitAttNbr.Sum",
	"ERAB.EstabInitSuccNbr.QCI1",
	"ERAB.EstabInitSuccNbr.QCI2",
	"ERAB.EstabInitSuccNbr.QCI3",
	"ERAB.EstabInitSuccNbr.QCI4",
	"ERAB.EstabInitSuccNbr.QCI5",
	"ERAB.EstabInitSuccNbr.QCI6",
	"ERAB.EstabInitSuccNbr.QCI7",
	"ERAB.EstabInitSuccNbr.QCI8",
	"ERAB.EstabInitSuccNbr.QCI9",
	/*SPR 10987 FIX start*/
	"ERAB.EstabInitSuccNbr.Sum",
	/*SPR 10987 FIX end*/
	"ERAB.EstabInitFailNbr.Sum",
	
    /*SPR 14383 START*/
#if ENUM_KPI_CAUSE_FLAG
    "ERAB.EstabInitFailNbr.1.12",
    "ERAB.EstabInitFailNbr.1.21",
    "ERAB.EstabInitFailNbr.1.26",
    "ERAB.EstabInitFailNbr.2.0",
    "ERAB.EstabInitFailNbr.5.0",
    "ERAB.EstabInitFailNbr.5.1",
#else
    "ERAB.EstabInitFailNbr.NoRadioResourcesAvailableInTargetCell",
    "ERAB.EstabInitFailNbr.RadioConnectionWithUELost",
    "ERAB.EstabInitFailNbr.FailureInTheRadioInterfaceProcedure",
    "ERAB.EstabInitFailNbr.TransportResourceUnavailable",
    "ERAB.EstabInitFailNbr.ControlProcessingOverload",
    "ERAB.EstabInitFailNbr.NotEnoughUserPlaneProcessingResourcesAvailable",
#endif
    /*SPR 14383 END*/

	"ERAB.EstabAddAttNbr.QCI1",
	"ERAB.EstabAddAttNbr.QCI2",
	"ERAB.EstabAddAttNbr.QCI3",
	"ERAB.EstabAddAttNbr.QCI4",
	"ERAB.EstabAddAttNbr.QCI5",
	"ERAB.EstabAddAttNbr.QCI6",
	"ERAB.EstabAddAttNbr.QCI7",
	"ERAB.EstabAddAttNbr.QCI8",
	"ERAB.EstabAddAttNbr.QCI9",
	"ERAB.EstabAddSuccNbr.QCI1",
	"ERAB.EstabAddSuccNbr.QCI2",
	"ERAB.EstabAddSuccNbr.QCI3",
	"ERAB.EstabAddSuccNbr.QCI4",
	"ERAB.EstabAddSuccNbr.QCI5",
	"ERAB.EstabAddSuccNbr.QCI6",
	"ERAB.EstabAddSuccNbr.QCI7",
	"ERAB.EstabAddSuccNbr.QCI8",
	"ERAB.EstabAddSuccNbr.QCI9",
	"ERAB.EstabAddFailNbr.Sum",
	/*SPR 19596 Code Deleted*/

	
 /*SPR 19596 Start*/
	
#if ENUM_KPI_CAUSE_FLAG
    "ERAB.EstabAddFailNbr.1.0",
    "ERAB.EstabAddFailNbr.1.1",
    "ERAB.EstabAddFailNbr.1.2",
    "ERAB.EstabAddFailNbr.1.3",
    "ERAB.EstabAddFailNbr.1.4",
    "ERAB.EstabAddFailNbr.1.5",
    "ERAB.EstabAddFailNbr.1.6",
    "ERAB.EstabAddFailNbr.1.7",
    "ERAB.EstabAddFailNbr.1.8",
    "ERAB.EstabAddFailNbr.1.9",
    "ERAB.EstabAddFailNbr.1.10",
    "ERAB.EstabAddFailNbr.1.11",
    "ERAB.EstabAddFailNbr.1.12",
    "ERAB.EstabAddFailNbr.1.13",
    "ERAB.EstabAddFailNbr.1.14",
    "ERAB.EstabAddFailNbr.1.15",
    "ERAB.EstabAddFailNbr.1.16",
    "ERAB.EstabAddFailNbr.1.17",
    "ERAB.EstabAddFailNbr.1.18",
    "ERAB.EstabAddFailNbr.1.19",
    "ERAB.EstabAddFailNbr.1.20",
    "ERAB.EstabAddFailNbr.1.21",
    "ERAB.EstabAddFailNbr.1.22",
    "ERAB.EstabAddFailNbr.1.23",
    "ERAB.EstabAddFailNbr.1.24",
    "ERAB.EstabAddFailNbr.1.25",
    "ERAB.EstabAddFailNbr.1.26",
    "ERAB.EstabAddFailNbr.1.27",
    "ERAB.EstabAddFailNbr.1.28",
    "ERAB.EstabAddFailNbr.1.29",
    "ERAB.EstabAddFailNbr.1.30",
    "ERAB.EstabAddFailNbr.1.31",
    "ERAB.EstabAddFailNbr.1.32",
    "ERAB.EstabAddFailNbr.1.33",
    "ERAB.EstabAddFailNbr.1.34",
    "ERAB.EstabAddFailNbr.1.35",
#else
 /*SPR 19596 End*/

	/*SPR 10987 FIX start*/
	"ERAB.EstabAddFailNbr.Unspecified_2",
	"ERAB.EstabAddFailNbr.Tx2RelocoverallExpiry",
	"ERAB.EstabAddFailNbr.SuccessfulHandover",
	"ERAB.EstabAddFailNbr.ReleaseDueToEutranGeneratedReason",
	"ERAB.EstabAddFailNbr.HandoverCancelled",
	"ERAB.EstabAddFailNbr.PartialHandover",
	"ERAB.EstabAddFailNbr.HoFailureInTargetEPCeNBOrTargetSystem",
	"ERAB.EstabAddFailNbr.HoTargetNotAllowed",
	"ERAB.EstabAddFailNbr.TS1RelocoverallExpiry",
	"ERAB.EstabAddFailNbr.TS1relocprepExpiry",
	"ERAB.EstabAddFailNbr.CellNotAvailable",
	"ERAB.EstabAddFailNbr.UnknownTargetID",
    "ERAB.EstabAddFailNbr.NoRadioResourcesAvailableInTargetCell",   /*SPR 19596*/
	"ERAB.EstabAddFailNbr.UnknownMmeUeS1apId",
	"ERAB.EstabAddFailNbr.UnknownEnbUeS1apId",
	"ERAB.EstabAddFailNbr.UnknownPairUeS1apId",
	"ERAB.EstabAddFailNbr.HandoverDesirableForRadioReason",
	"ERAB.EstabAddFailNbr.TimeCriticalHandover",
	"ERAB.EstabAddFailNbr.ResourceOptimisationHandover",
	"ERAB.EstabAddFailNbr.ReduceLoadInServingCell",
	"ERAB.EstabAddFailNbr.UserInactivity",
    "ERAB.EstabAddFailNbr.RadioConnectionWithUELost",     /*SPR 19596 End*/
	"ERAB.EstabAddFailNbr.LoadBalancingTauRequired",
	"ERAB.EstabAddFailNbr.CsFallbackTriggered",
	"ERAB.EstabAddFailNbr.UeNotAvailableForPsService",
	"ERAB.EstabAddFailNbr.RadioResourcesNotAvailable",
	"ERAB.EstabAddFailNbr.FailureInRadioInterfaceProcedure",
	"ERAB.EstabAddFailNbr.InvalidQosCombination",
	"ERAB.EstabAddFailNbr.InterratRedirection",
	"ERAB.EstabAddFailNbr.InteractionWithOtherProcedure",
	"ERAB.EstabAddFailNbr.UnknownERABID",
	"ERAB.EstabAddFailNbr.MultipleERABIDInstances",
	"ERAB.EstabAddFailNbr.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported",
	"ERAB.EstabAddFailNbr.S1IntraSystemHandoverTriggered",
	"ERAB.EstabAddFailNbr.S1IinterSystemHandoverTriggered",
	"ERAB.EstabAddFailNbr.X2HOTriggered",
#endif  /*SPR 19596*/
	/*SPR 10987 FIX end*/
    /*SPR 14383 START*/
#if ENUM_KPI_CAUSE_FLAG
    /*SPR 19596 Start*/
        /*Code Deleted*/
    /*SPR 19596 End*/
    "ERAB.EstabAddFailNbr.2.0",
    "ERAB.EstabAddFailNbr.5.0",
    "ERAB.EstabAddFailNbr.5.1",
    /*SPR 19596 Start*/

    "ERAB.EstabAddFailNbr.5.2",
    "ERAB.EstabAddFailNbr.5.3",
    "ERAB.EstabAddFailNbr.5.4",
    "ERAB.EstabAddFailNbr.5.5",
    /*SPR 19596 End*/

#else
    /*SPR 19596 Code Deleted*/
    "ERAB.EstabAddFailNbr.TransportResourceUnavailable",
    "ERAB.EstabAddFailNbr.ControlProcessingOverload",
    "ERAB.EstabAddFailNbr.NotEnoughUserPlaneProcessingResourcesAvailable",
    /*SPR 19596 Start*/
	/*SPR 10987 FIX start*/
	"ERAB.EstabAddFailNbr.HardwareFailure",
	"ERAB.EstabAddFailNbr.OmIntervention",
	"ERAB.EstabAddFailNbr.Unspecified",
	"ERAB.EstabAddFailNbr.UnknownPLMN",
	/*SPR 10987 FIX end*/
    /*SPR 19596 End*/

#endif
    /*SPR 14383 END*/

    "ERAB.EstabTimeMean.QCI1",
	"ERAB.EstabTimeMean.QCI2",
	"ERAB.EstabTimeMean.QCI3",
	"ERAB.EstabTimeMean.QCI4",
	"ERAB.EstabTimeMean.QCI5",
	"ERAB.EstabTimeMean.QCI6",
	"ERAB.EstabTimeMean.QCI7",
	"ERAB.EstabTimeMean.QCI8",
	"ERAB.EstabTimeMean.QCI9",
	"ERAB.EstabTimeMax.QCI1",
	"ERAB.EstabTimeMax.QCI2",
	"ERAB.EstabTimeMax.QCI3",
	"ERAB.EstabTimeMax.QCI4",
	"ERAB.EstabTimeMax.QCI5",
	"ERAB.EstabTimeMax.QCI6",
	"ERAB.EstabTimeMax.QCI7",
	"ERAB.EstabTimeMax.QCI8",
	"ERAB.EstabTimeMax.QCI9",
	"ERAB.RelEnbNbr.QCI1",
	"ERAB.RelEnbNbr.QCI2",
	"ERAB.RelEnbNbr.QCI3",
	"ERAB.RelEnbNbr.QCI4",
	"ERAB.RelEnbNbr.QCI5",
	"ERAB.RelEnbNbr.QCI6",
	"ERAB.RelEnbNbr.QCI7",
	"ERAB.RelEnbNbr.QCI8",
	"ERAB.RelEnbNbr.QCI9",
	"ERAB.RelEnbNbr.Sum",
	 /*SPR 19596 Start*/
	
#if ENUM_KPI_CAUSE_FLAG
    "ERAB.RelEnbNbr.1.0",
    /*SPR_16913_START*/
    "ERAB.RelEnbNbr.1.1",
    /*SPR_16913_END*/
    "ERAB.RelEnbNbr.1.2",
    "ERAB.RelEnbNbr.1.3",
    "ERAB.RelEnbNbr.1.4",
    "ERAB.RelEnbNbr.1.5",
    "ERAB.RelEnbNbr.1.6",
    "ERAB.RelEnbNbr.1.7",
    "ERAB.RelEnbNbr.1.8",
    "ERAB.RelEnbNbr.1.9",
    "ERAB.RelEnbNbr.1.10",
    "ERAB.RelEnbNbr.1.11",
    "ERAB.RelEnbNbr.1.12",
    "ERAB.RelEnbNbr.1.13",
    "ERAB.RelEnbNbr.1.14",
    "ERAB.RelEnbNbr.1.15",
    "ERAB.RelEnbNbr.1.16",
    "ERAB.RelEnbNbr.1.17",
    "ERAB.RelEnbNbr.1.18",
    "ERAB.RelEnbNbr.1.19",
    "ERAB.RelEnbNbr.1.20",
    "ERAB.RelEnbNbr.1.21",
    "ERAB.RelEnbNbr.1.22",
    "ERAB.RelEnbNbr.1.23",
    "ERAB.RelEnbNbr.1.24",
    "ERAB.RelEnbNbr.1.25",
    "ERAB.RelEnbNbr.1.26",
    "ERAB.RelEnbNbr.1.27",
    "ERAB.RelEnbNbr.1.28",
    "ERAB.RelEnbNbr.1.29",
    "ERAB.RelEnbNbr.1.30",
    "ERAB.RelEnbNbr.1.31",
    "ERAB.RelEnbNbr.1.32",
    "ERAB.RelEnbNbr.1.33",
    "ERAB.RelEnbNbr.1.34",
    "ERAB.RelEnbNbr.1.35",
    "ERAB.RelEnbNbr.1.36",
#else
	/*SPR 10987 FIX start*/
	"ERAB.RelEnbNbr.Unspecified_2",
    /*SPR 19596 End*/

    /*SPR_16913_START*/
    "ERAB.RelEnbNbr.TX2RELOCoverallExpiry",
    /*SPR_16913_END*/
    /*SPR 19596 Code Deleted*/

	"ERAB.RelEnbNbr.SuccessfulHandover",
	"ERAB.RelEnbNbr.ReleaseDueToEutranGeneratedReason",
	"ERAB.RelEnbNbr.HandoverCancelled",
	"ERAB.RelEnbNbr.PartialHandover",
	"ERAB.RelEnbNbr.HoFailureInTargetEPCeNBOrTargetSystem",
	"ERAB.RelEnbNbr.HoTargetNotAllowed",
    /*SPR 19596 Start*/
    "ERAB.RelEnbNbr.TS1RelocOverallExpiry",
	"ERAB.RelEnbNbr.TS1relocprepExpiry", 
    /*SPR 19596 End*/
	"ERAB.RelEnbNbr.CellNotAvailable",
	"ERAB.RelEnbNbr.UnknownTargetID",
	"ERAB.RelEnbNbr.NoRadioResourcesAvailableInTargetCell",
	"ERAB.RelEnbNbr.UnknownMmeUeS1apId",
	"ERAB.RelEnbNbr.UnknownEnbUeS1apId",
	"ERAB.RelEnbNbr.UnknownPairUeS1apId",
	"ERAB.RelEnbNbr.HandoverDesirableForRadioReason",
	"ERAB.RelEnbNbr.TimeCriticalHandover",
	"ERAB.RelEnbNbr.ResourceOptimisationHandover",
	"ERAB.RelEnbNbr.ReduceLoadInServingCell",
	"ERAB.RelEnbNbr.UserInactivity",  /*SPR 19596*/
	"ERAB.RelEnbNbr.RadioConnectionWithUELost",
	"ERAB.RelEnbNbr.LoadBalancingTauRequired",
	"ERAB.RelEnbNbr.CsFallbackTriggered",
	"ERAB.RelEnbNbr.UeNotAvailableForPsService",
	"ERAB.RelEnbNbr.RadioResourcesNotAvailable",
	"ERAB.RelEnbNbr.FailureInRadioInterfaceProcedure",
	"ERAB.RelEnbNbr.InvalidQosCombination",
	"ERAB.RelEnbNbr.InterratRedirection",
	"ERAB.RelEnbNbr.InteractionWithOtherProcedure",
	"ERAB.RelEnbNbr.UnknownERABID",
	"ERAB.RelEnbNbr.MultipleERABIDInstances",
	"ERAB.RelEnbNbr.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported",
	"ERAB.RelEnbNbr.S1IntraSystemHandoverTriggered",
	"ERAB.RelEnbNbr.S1InterSystemHandoverTriggered",
	"ERAB.RelEnbNbr.X2HOTriggered",
    /*SPR 19596 Start*/

    /*SPR 14383 START*/
    "ERAB.RelEnbNbr.RedirectionTowards1xRTT",
    /*SPR 14383 END*/
#endif    
#if ENUM_KPI_CAUSE_FLAG
	"ERAB.RelEnbNbr.2.0",
	"ERAB.RelEnbNbr.2.1",
#else   
    /*SPR 19596 End*/

	"ERAB.RelEnbNbr.TransportResourceUnavailable",
	"ERAB.RelEnbNbr.unspecified_3",
    /*SPR 19596 Start*/

#endif    
#if ENUM_KPI_CAUSE_FLAG
	"ERAB.RelEnbNbr.3.0",
	"ERAB.RelEnbNbr.3.1",
	"ERAB.RelEnbNbr.3.2",
	"ERAB.RelEnbNbr.3.3",
#else
    /*SPR 19596 End*/

	"ERAB.RelEnbNbr.NormalRelease",
	"ERAB.RelEnbNbr.AuthenticationFailure",
	"ERAB.RelEnbNbr.Detach",
	"ERAB.RelEnbNbr.Unspecified_4",
    /*SPR 19596 Start*/

#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"ERAB.RelEnbNbr.4.0",
	"ERAB.RelEnbNbr.4.1",
	"ERAB.RelEnbNbr.4.2",
	"ERAB.RelEnbNbr.4.3",
	"ERAB.RelEnbNbr.4.4",
	"ERAB.RelEnbNbr.4.5",
	"ERAB.RelEnbNbr.4.6",
#else
    /*SPR 19596 End*/

	"ERAB.RelEnbNbr.TransferSyntaxError",
	"ERAB.RelEnbNbr.AbstractSyntaxErrorReject",
	"ERAB.RelEnbNbr.AbstractSyntaxErrorIgnoreAndNotify",
	"ERAB.RelEnbNbr.MessageNotCompatibleWithReceiverState",
	"ERAB.RelEnbNbr.SemanticError",
	"ERAB.RelEnbNbr.AbstractSyntaxErrorFalselyConstructedMessage",
	"ERAB.RelEnbNbr.Unspecified_1",
    /*SPR 19596 Start*/

#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"ERAB.RelEnbNbr.5.0",
	"ERAB.RelEnbNbr.5.1",
	"ERAB.RelEnbNbr.5.2",
	"ERAB.RelEnbNbr.5.3",
	"ERAB.RelEnbNbr.5.4",
	"ERAB.RelEnbNbr.5.5",
#else
    /*SPR 19596 End*/

	"ERAB.RelEnbNbr.ControlProcessingOverload",
	"ERAB.RelEnbNbr.NotEnoughUserPlaneProcessingResources",
	"ERAB.RelEnbNbr.HardwareFailure",
	"ERAB.RelEnbNbr.OmIntervention",
	"ERAB.RelEnbNbr.Unspecified",
	"ERAB.RelEnbNbr.UnknownPLMN",
#endif     /*SPR 19596 */
	/*SPR 10987 FIX end*/
	"ERAB.RelAttNbr.QCI1",
	"ERAB.RelAttNbr.QCI2",
	"ERAB.RelAttNbr.QCI3",
	"ERAB.RelAttNbr.QCI4",
	"ERAB.RelAttNbr.QCI5",
	"ERAB.RelAttNbr.QCI6",
	"ERAB.RelAttNbr.QCI7",
	"ERAB.RelAttNbr.QCI8",
	"ERAB.RelAttNbr.QCI9",
	"ERAB.RelAttNbr.Sum",/*SPR 19611 */
	"ERAB.RelSuccNbr.QCI1",
	"ERAB.RelSuccNbr.QCI2",
	"ERAB.RelSuccNbr.QCI3",
	"ERAB.RelSuccNbr.QCI4",
	"ERAB.RelSuccNbr.QCI5",
	"ERAB.RelSuccNbr.QCI6",
	"ERAB.RelSuccNbr.QCI7",
	"ERAB.RelSuccNbr.QCI8",
	"ERAB.RelSuccNbr.QCI9",
	"ERAB.RelSuccNbr.Sum",/*SPR 19611 */
	"ERAB.RelFailNbr.Sum",
 /*SPR 19596 Start*/

#if ENUM_KPI_CAUSE_FLAG
	"ERAB.RelFailNbr.1.0",
	"ERAB.RelFailNbr.1.1",
	"ERAB.RelFailNbr.1.2",
	"ERAB.RelFailNbr.1.3",
	"ERAB.RelFailNbr.1.4",
	"ERAB.RelFailNbr.1.5",
	"ERAB.RelFailNbr.1.6",
	"ERAB.RelFailNbr.1.7",
	"ERAB.RelFailNbr.1.8",
	"ERAB.RelFailNbr.1.9",
	"ERAB.RelFailNbr.1.10",
	"ERAB.RelFailNbr.1.11",
	"ERAB.RelFailNbr.1.12",
	"ERAB.RelFailNbr.1.13",
	"ERAB.RelFailNbr.1.14",
	"ERAB.RelFailNbr.1.15",
	"ERAB.RelFailNbr.1.16",
	"ERAB.RelFailNbr.1.17",
	"ERAB.RelFailNbr.1.18",
	"ERAB.RelFailNbr.1.19",
	"ERAB.RelFailNbr.1.20",
	"ERAB.RelFailNbr.1.21",
	"ERAB.RelFailNbr.1.22",
	"ERAB.RelFailNbr.1.23",
	"ERAB.RelFailNbr.1.24",
	"ERAB.RelFailNbr.1.25",
	"ERAB.RelFailNbr.1.26",
	"ERAB.RelFailNbr.1.27",
	"ERAB.RelFailNbr.1.28",
	"ERAB.RelFailNbr.1.29",
	"ERAB.RelFailNbr.1.30",
	"ERAB.RelFailNbr.1.31",
	"ERAB.RelFailNbr.1.32",
	"ERAB.RelFailNbr.1.33",
	"ERAB.RelFailNbr.1.34",
	"ERAB.RelFailNbr.1.35",
    /*SPR 19596 End*/

#else
/*SPR 19596 Code Deleted*/

    /*SPR 10987 FIX start*/
/*SPR 19596 Code Deleted*/
	"ERAB.RelFailNbr.Unspecified_2",
	"ERAB.RelFailNbr.Tx2RelocOverallExpiry",
	"ERAB.RelFailNbr.SuccessfulHandover",
	"ERAB.RelFailNbr.ReleaseDueToEutranGeneratedReason",
	"ERAB.RelFailNbr.HandoverCancelled",
	"ERAB.RelFailNbr.PartialHandover",
	"ERAB.RelFailNbr.HoFailureInTargetEPCeNBOrTargetSystem",
	"ERAB.RelFailNbr.HoTargetNotAllowed",
	"ERAB.RelFailNbr.TS1RelocOverallExpiry",
	"ERAB.RelFailNbr.TS1RelocPrepExpiry",
	"ERAB.RelFailNbr.CellNotAvailable",
	"ERAB.RelFailNbr.UnknownTargetID",
	"ERAB.RelFailNbr.NoRadioResourcesAvailableInTargetCell",
	"ERAB.RelFailNbr.UnknownMmeUeS1apId",
	"ERAB.RelFailNbr.UnknownEnbUeS1apId",
	"ERAB.RelFailNbr.UnknownPairUeS1apId",
	"ERAB.RelFailNbr.HandoverDesirableForRadioReason",
	"ERAB.RelFailNbr.TimeCriticalHandover",
	"ERAB.RelFailNbr.ResourceOptimisationHandover",
	"ERAB.RelFailNbr.ReduceLoadInServingCell",
	"ERAB.RelFailNbr.NoUserActivity",   /*SPR 19596*/
	"ERAB.RelFailNbr.RadioConnectionWithUELost",
	"ERAB.RelFailNbr.LoadBalancingTauRequired",
	"ERAB.RelFailNbr.CsFallbackTriggered",
	"ERAB.RelFailNbr.UeNotAvailableForPsService",
	"ERAB.RelFailNbr.RadioResourcesNotAvailable",
	"ERAB.RelFailNbr.FailureInRadioInterfaceProcedure",
	"ERAB.RelFailNbr.InvalidQosCombination",
	"ERAB.RelFailNbr.InterratRedirection",
	"ERAB.RelFailNbr.InteractionWithOtherProcedure",
	"ERAB.RelFailNbr.UnknownERABID",
	"ERAB.RelFailNbr.MultipleERABIDInstances",
	"ERAB.RelFailNbr.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported",
	"ERAB.RelFailNbr.S1IntraSystemHandoverTriggered",
	"ERAB.RelFailNbr.S1InterSystemHandoverTriggered",
	"ERAB.RelFailNbr.X2HOTriggered",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"ERAB.RelFailNbr.2.0",
	"ERAB.RelFailNbr.2.1",
#else
	"ERAB.RelFailNbr.TransportResourceUnavailable",
	"ERAB.RelFailNbr.unspecified_3",
#endif    
#if ENUM_KPI_CAUSE_FLAG
	"ERAB.RelFailNbr.3.0",
	"ERAB.RelFailNbr.3.1",
	"ERAB.RelFailNbr.3.2",
	"ERAB.RelFailNbr.3.3",
#else   
    /*SPR 19596 End*/

	"ERAB.RelFailNbr.NormalRelease",
	"ERAB.RelFailNbr.AuthenticationFailure",
	"ERAB.RelFailNbr.Detach",
	"ERAB.RelFailNbr.Unspecified_4",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"ERAB.RelFailNbr.4.0",
	"ERAB.RelFailNbr.4.1",
	"ERAB.RelFailNbr.4.2",
	"ERAB.RelFailNbr.4.3",
	"ERAB.RelFailNbr.4.4",
	"ERAB.RelFailNbr.4.5",
	"ERAB.RelFailNbr.4.6",
#else
    /*SPR 19596 End*/

	"ERAB.RelFailNbr.TransferSyntaxError",
	"ERAB.RelFailNbr.AbstractSyntaxErrorReject",
	"ERAB.RelFailNbr.AbstractSyntaxErrorIgnoreAndNotify",
	"ERAB.RelFailNbr.MessageNotCompatibleWithReceiverState",
	"ERAB.RelFailNbr.SemanticError",
	"ERAB.RelFailNbr.AbstractSyntaxErrorFalselyConstructedMessage",
	"ERAB.RelFailNbr.Unspecified_1",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"ERAB.RelFailNbr.5.0",
	"ERAB.RelFailNbr.5.1",
	"ERAB.RelFailNbr.5.2",
	"ERAB.RelFailNbr.5.3",
	"ERAB.RelFailNbr.5.4",
	"ERAB.RelFailNbr.5.5",
#else
    "ERAB.RelFailNbr.ControlProcessingOverload",
    /*SPR 19596 End*/
	"ERAB.RelFailNbr.NotEnoughUserPlaneProcessingResources",
	"ERAB.RelFailNbr.HardwareFailure",
	"ERAB.RelFailNbr.OmIntervention",
	"ERAB.RelFailNbr.Unspecified",
	"ERAB.RelFailNbr.UnknownPLMN",
#endif     /*SPR 19596 */
	/*SPR 10987 FIX end*/
	"ERAB.RelActNbr.QCI1",
	"ERAB.RelActNbr.QCI2",
	"ERAB.RelActNbr.QCI3",
	"ERAB.RelActNbr.QCI4",
	"ERAB.RelActNbr.QCI5",
	"ERAB.RelActNbr.QCI6",
	"ERAB.RelActNbr.QCI7",
	"ERAB.RelActNbr.QCI8",
	"ERAB.RelActNbr.QCI9",
	"ERAB.ModQoSAttNbr.QCI1",
	"ERAB.ModQoSAttNbr.QCI2",
	"ERAB.ModQoSAttNbr.QCI3",
	"ERAB.ModQoSAttNbr.QCI4",
	"ERAB.ModQoSAttNbr.QCI5",
	"ERAB.ModQoSAttNbr.QCI6",
	"ERAB.ModQoSAttNbr.QCI7",
	"ERAB.ModQoSAttNbr.QCI8",
	"ERAB.ModQoSAttNbr.QCI9",
	"ERAB.ModQoSSuccNbr.QCI1",
	"ERAB.ModQoSSuccNbr.QCI2",
	"ERAB.ModQoSSuccNbr.QCI3",
	"ERAB.ModQoSSuccNbr.QCI4",
	"ERAB.ModQoSSuccNbr.QCI5",
	"ERAB.ModQoSSuccNbr.QCI6",
	"ERAB.ModQoSSuccNbr.QCI7",
	"ERAB.ModQoSSuccNbr.QCI8",
	"ERAB.ModQoSSuccNbr.QCI9",
	"ERAB.ModQoSFailNbr.Sum",
	
 /*SPR 19596 Start*/
#if ENUM_KPI_CAUSE_FLAG
    "ERAB.ModQoSFailNbr.1.0",
    "ERAB.ModQoSFailNbr.1.1",
    "ERAB.ModQoSFailNbr.1.2",
    "ERAB.ModQoSFailNbr.1.3",
    "ERAB.ModQoSFailNbr.1.4",
    "ERAB.ModQoSFailNbr.1.5",
    "ERAB.ModQoSFailNbr.1.6",
    "ERAB.ModQoSFailNbr.1.7",
    "ERAB.ModQoSFailNbr.1.8",
    "ERAB.ModQoSFailNbr.1.9",
    "ERAB.ModQoSFailNbr.1.10",
    "ERAB.ModQoSFailNbr.1.11",
    "ERAB.ModQoSFailNbr.1.12",
    "ERAB.ModQoSFailNbr.1.13",
    "ERAB.ModQoSFailNbr.1.14",
    "ERAB.ModQoSFailNbr.1.15",
    "ERAB.ModQoSFailNbr.1.16",
    "ERAB.ModQoSFailNbr.1.17",
    "ERAB.ModQoSFailNbr.1.18",
    "ERAB.ModQoSFailNbr.1.19",
    "ERAB.ModQoSFailNbr.1.20",
    "ERAB.ModQoSFailNbr.1.21",
    "ERAB.ModQoSFailNbr.1.22",
    "ERAB.ModQoSFailNbr.1.23",
    "ERAB.ModQoSFailNbr.1.24",
    "ERAB.ModQoSFailNbr.1.25",
    "ERAB.ModQoSFailNbr.1.26",
    "ERAB.ModQoSFailNbr.1.27",
    "ERAB.ModQoSFailNbr.1.28",
    "ERAB.ModQoSFailNbr.1.29",
    "ERAB.ModQoSFailNbr.1.30",
    "ERAB.ModQoSFailNbr.1.31",
    "ERAB.ModQoSFailNbr.1.32",
    "ERAB.ModQoSFailNbr.1.33",
    "ERAB.ModQoSFailNbr.1.34",
    "ERAB.ModQoSFailNbr.1.35",
    /*SPR 19596 End*/

#else
 /*SPR 19596 Code deleted*/


	/*SPR 10987 FIX start*/
     /*SPR 19596 Code deleted*/
	"ERAB.ModQoSFailNbr.Unspecified_2",
	"ERAB.ModQoSFailNbr.Tx2RelocOverallExpiry",
	"ERAB.ModQoSFailNbr.SuccessfulHandover",
	"ERAB.ModQoSFailNbr.ReleaseDueToEutranGeneratedReason",
	"ERAB.ModQoSFailNbr.HandoverCancelled",
	"ERAB.ModQoSFailNbr.PartialHandover",
	"ERAB.ModQoSFailNbr.HoFailureInTargetEPCeNBOrTargetSystem",
	"ERAB.ModQoSFailNbr.HoTargetNotAllowed",
	"ERAB.ModQoSFailNbr.TS1RelocOverallExpiry",
	"ERAB.ModQoSFailNbr.TS1RelocPrepExpiry",
	"ERAB.ModQoSFailNbr.CellNotAvailable",
	"ERAB.ModQoSFailNbr.UnknownTargetID",
    "ERAB.ModQoSFailNbr.NoRadioResourcesAvailableInTargetCell",  /*SPR 19596*/
	"ERAB.ModQoSFailNbr.UnknownMmeUeS1apId",
	"ERAB.ModQoSFailNbr.UnknownEnbUeS1apId",
	"ERAB.ModQoSFailNbr.UnknownPairUeS1apId",
	"ERAB.ModQoSFailNbr.HandoverDesirableForRadioReason",
	"ERAB.ModQoSFailNbr.TimeCriticalHandover",
	"ERAB.ModQoSFailNbr.ResourceOptimisationHandover",
	"ERAB.ModQoSFailNbr.ReduceLoadInServingCell",
	"ERAB.ModQoSFailNbr.NoUserActivity",      /*SPR 19596*/
	"ERAB.ModQoSFailNbr.RadioConnectionWithUELost",
	"ERAB.ModQoSFailNbr.LoadBalancingTauRequired",
	"ERAB.ModQoSFailNbr.CsFallbackTriggered",
	"ERAB.ModQoSFailNbr.UeNotAvailableForPsService",
	"ERAB.ModQoSFailNbr.RadioResourcesNotAvailable",
	"ERAB.ModQoSFailNbr.FailureInRadioInterfaceProcedure",
	"ERAB.ModQoSFailNbr.InvalidQosCombination",
	"ERAB.ModQoSFailNbr.InterratRedirection",
	"ERAB.ModQoSFailNbr.InteractionWithOtherProcedure",
	"ERAB.ModQoSFailNbr.UnknownERABID",
	"ERAB.ModQoSFailNbr.MultipleERABIDInstances",
	"ERAB.ModQoSFailNbr.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported",
	"ERAB.ModQoSFailNbr.S1IntraSystemHandoverTriggered",
	"ERAB.ModQoSFailNbr.S1InterSystemHandoverTriggered",
	"ERAB.ModQoSFailNbr.X2HOTriggered",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"ERAB.ModQoSFailNbr.2.0",
	"ERAB.ModQoSFailNbr.2.1",
#else
    "ERAB.ModQoSFailNbr.TransportResourceUnavailable",
	"ERAB.ModQoSFailNbr.unspecified_3",
#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"ERAB.ModQoSFailNbr.3.0",
	"ERAB.ModQoSFailNbr.3.1",
	"ERAB.ModQoSFailNbr.3.2",
	"ERAB.ModQoSFailNbr.3.3",
#else
    /*SPR 19596 End*/
	"ERAB.ModQoSFailNbr.NormalRelease",
	"ERAB.ModQoSFailNbr.AuthenticationFailure",
	"ERAB.ModQoSFailNbr.Detach",
	"ERAB.ModQoSFailNbr.Unspecified_4",
    /*SPR 19596 Start*/

#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"ERAB.ModQoSFailNbr.4.0",
	"ERAB.ModQoSFailNbr.4.1",
	"ERAB.ModQoSFailNbr.4.2",
	"ERAB.ModQoSFailNbr.4.3",
	"ERAB.ModQoSFailNbr.4.4",
	"ERAB.ModQoSFailNbr.4.5",
	"ERAB.ModQoSFailNbr.4.6",
#else
    /*SPR 19596 End*/

	"ERAB.ModQoSFailNbr.TransferSyntaxError",
	"ERAB.ModQoSFailNbr.AbstractSyntaxErrorReject",
	"ERAB.ModQoSFailNbr.AbstractSyntaxErrorIgnoreAndNotify",
	"ERAB.ModQoSFailNbr.MessageNotCompatibleWithReceiverState",
	"ERAB.ModQoSFailNbr.SemanticError",
	"ERAB.ModQoSFailNbr.AbstractSyntaxErrorFalselyConstructedMessage",
	"ERAB.ModQoSFailNbr.Unspecified_1",
    /*SPR 19596 Start*/

#endif    
#if ENUM_KPI_CAUSE_FLAG
	"ERAB.ModQoSFailNbr.5.0",
	"ERAB.ModQoSFailNbr.5.1",
	"ERAB.ModQoSFailNbr.5.2",
	"ERAB.ModQoSFailNbr.5.3",
	"ERAB.ModQoSFailNbr.5.4",
	"ERAB.ModQoSFailNbr.5.5",

#else
    /*SPR 14383 START*/
    "ERAB.ModQoSFailNbr.ControlProcessingOverload",
    "ERAB.ModQoSFailNbr.NotEnoughUserPlaneProcessingResourcesAvailable",
    /*SPR 14383 END*/
    /*SPR 19596 End*/

	"ERAB.ModQoSFailNbr.HardwareFailure",
	"ERAB.ModQoSFailNbr.OmIntervention",
	"ERAB.ModQoSFailNbr.Unspecified",
	"ERAB.ModQoSFailNbr.UnknownPLMN",
#endif   /*SPR 19596 */ 
	/*SPR 10987 FIX end*/
	"ERAB.UsageNbrMean.QCI1",
	"ERAB.UsageNbrMean.QCI2",
	"ERAB.UsageNbrMean.QCI3",
	"ERAB.UsageNbrMean.QCI4",
	"ERAB.UsageNbrMean.QCI5",
	"ERAB.UsageNbrMean.QCI6",
	"ERAB.UsageNbrMean.QCI7",
	"ERAB.UsageNbrMean.QCI8",
	"ERAB.UsageNbrMean.QCI9",
/* SPR 18861 */
	"ERAB.UsageNbrMean.Sum",
/* SPR 18861 */
	"ERAB.UsageNbrMax.QCI1",
	"ERAB.UsageNbrMax.QCI2",
	"ERAB.UsageNbrMax.QCI3",
	"ERAB.UsageNbrMax.QCI4",
	"ERAB.UsageNbrMax.QCI5",
	"ERAB.UsageNbrMax.QCI6",
	"ERAB.UsageNbrMax.QCI7",
	"ERAB.UsageNbrMax.QCI8",
	"ERAB.UsageNbrMax.QCI9",
/* SPR 18861 */
	"ERAB.UsageNbrMax.Sum",
/* SPR 18861 */
	OAM_NULL};

char *pm_xml_ho_param_name[]={
	"HO.InterEnbOutPrepAtt",
	"HO.InterEnbOutAtt.Sum",
	"HO.InterEnbOutSucc.Sum",
	/*SPR 10987 FIX start*/
 /*SPR 19596 Start*/
#if ENUM_KPI_CAUSE_FLAG
	"HO.InterEnbOutAtt.1.0",
	"HO.InterEnbOutAtt.1.1",
	"HO.InterEnbOutAtt.1.2",
	"HO.InterEnbOutAtt.1.3",
	"HO.InterEnbOutAtt.1.4",
	"HO.InterEnbOutAtt.1.5",
	"HO.InterEnbOutAtt.1.6",
	"HO.InterEnbOutAtt.1.7",
	"HO.InterEnbOutAtt.1.8",
	"HO.InterEnbOutAtt.1.9",
	"HO.InterEnbOutAtt.1.10",
	"HO.InterEnbOutAtt.1.11",
	"HO.InterEnbOutAtt.1.12",
	"HO.InterEnbOutAtt.1.13",
	"HO.InterEnbOutAtt.1.14",
	"HO.InterEnbOutAtt.1.15",
	"HO.InterEnbOutAtt.1.16",
	"HO.InterEnbOutAtt.1.17",
	"HO.InterEnbOutAtt.1.18",
	"HO.InterEnbOutAtt.1.19",
	"HO.InterEnbOutAtt.1.20",
	"HO.InterEnbOutAtt.1.21",
	"HO.InterEnbOutAtt.1.22",
	"HO.InterEnbOutAtt.1.23",
	"HO.InterEnbOutAtt.1.24",
	"HO.InterEnbOutAtt.1.25",
	"HO.InterEnbOutAtt.1.26",
	"HO.InterEnbOutAtt.1.27",
	"HO.InterEnbOutAtt.1.28",
	"HO.InterEnbOutAtt.1.29",
	"HO.InterEnbOutAtt.1.30",
	"HO.InterEnbOutAtt.1.31",
	"HO.InterEnbOutAtt.1.32",
	"HO.InterEnbOutAtt.1.33",
	"HO.InterEnbOutAtt.1.34",
	"HO.InterEnbOutAtt.1.35",
#else   
    /*SPR 19596 End*/

	"HO.InterEnbOutAtt.Unspecified",
	"HO.InterEnbOutAtt.Tx2RelocOverallExpiry",
	"HO.InterEnbOutAtt.SuccessfulHandover",
	"HO.InterEnbOutAtt.ReleaseDueToEutranGeneratedReason",
	"HO.InterEnbOutAtt.HandoverCancelled",
	"HO.InterEnbOutAtt.PartialHandover",
	"HO.InterEnbOutAtt.HoFailureInTargetEPCeNBOrTargetSystem",
	"HO.InterEnbOutAtt.HoTargetNotAllowed",
	"HO.InterEnbOutAtt.TS1RelocOverallExpiry",
	"HO.InterEnbOutAtt.TS1RelocPrepExpiry",
	"HO.InterEnbOutAtt.CellNotAvailable",
	"HO.InterEnbOutAtt.UnknownTargetID",
	"HO.InterEnbOutAtt.NoRadioResourcesAvailableInTargetCell",
	"HO.InterEnbOutAtt.UnknownMmeUeS1apId",
	"HO.InterEnbOutAtt.UnknownEnbUeS1apId",
	"HO.InterEnbOutAtt.UnknownPairUeS1apId",  /*SPR 19596*/
	"HO.InterEnbOutAtt.HandoverDesirableForRadioReason",
	"HO.InterEnbOutAtt.TimeCriticalHandover",
	"HO.InterEnbOutAtt.ResourceOptimisationHandover",
	"HO.InterEnbOutAtt.ReduceLoadInServingCell",
	"HO.InterEnbOutAtt.NoUserActivity",  /*SPR 19596 */
	"HO.InterEnbOutAtt.RadioConnectionWithUeLost",
	"HO.InterEnbOutAtt.LoadBalancingTauRequired",
	"HO.InterEnbOutAtt.CsFallbackTriggered",
	"HO.InterEnbOutAtt.UeNotAvailableForPsService",
	"HO.InterEnbOutAtt.RadioResourcesNotAvailable",
	"HO.InterEnbOutAtt.FailureInRadioInterfaceProcedure",
	"HO.InterEnbOutAtt.InvalidQosCombination",
	"HO.InterEnbOutAtt.InterratRedirection",
	"HO.InterEnbOutAtt.InteractionWithOtherProcedure",
	"HO.InterEnbOutAtt.UnknownERABID",
	"HO.InterEnbOutAtt.MultipleERABIDInstances",
	"HO.InterEnbOutAtt.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported",
	"HO.InterEnbOutAtt.S1IntraSystemHandoverTriggered",
	"HO.InterEnbOutAtt.S1InterSystemHandoverTriggered",
	"HO.InterEnbOutAtt.X2HandoverTriggered",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG
	"HO.InterEnbOutAtt.2.0",
	"HO.InterEnbOutAtt.2.1",
#else
	"HO.InterEnbOutAtt.TransportResourceUnavailable",
	"HO.InterEnbOutAtt.TransportUnspecified",
#endif    
#if ENUM_KPI_CAUSE_FLAG
	"HO.InterEnbOutAtt.3.0",
	"HO.InterEnbOutAtt.3.1",
	"HO.InterEnbOutAtt.3.2",
	"HO.InterEnbOutAtt.3.3",
#else
    /*SPR 19596 End*/

	"HO.InterEnbOutAtt.NormalRelease",
	"HO.InterEnbOutAtt.AuthenticationFailure",
	"HO.InterEnbOutAtt.Detach",
	"HO.InterEnbOutAtt.Unspecified4",
    /*SPR 19596 Start*/

#endif    
#if ENUM_KPI_CAUSE_FLAG
	"HO.InterEnbOutAtt.4.0",
	"HO.InterEnbOutAtt.4.1",
	"HO.InterEnbOutAtt.4.2",
	"HO.InterEnbOutAtt.4.3",
	"HO.InterEnbOutAtt.4.4",
	"HO.InterEnbOutAtt.4.5",
	"HO.InterEnbOutAtt.4.6",
#else
    /*SPR 19596 End*/

	"HO.InterEnbOutAtt.TransferSyntaxError",
	"HO.InterEnbOutAtt.AbstractSyntaxErrorReject",
	"HO.InterEnbOutAtt.AbstractSyntaxErrorIgnoreAndNotify",
	"HO.InterEnbOutAtt.MessageNotCompatibleWithReceiverState",
	"HO.InterEnbOutAtt.SemanticError",
	"HO.InterEnbOutAtt.AbstractSyntaxErrorFalselyConstructedMessage",
	"HO.InterEnbOutAtt.Unspecified_1",
    /*SPR 19596 Start*/

#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"HO.InterEnbOutAtt.5.0",
	"HO.InterEnbOutAtt.5.1",
	"HO.InterEnbOutAtt.5.2",
	"HO.InterEnbOutAtt.5.3",
	"HO.InterEnbOutAtt.5.4",
	"HO.InterEnbOutAtt.5.5",

#else
    /*SPR 19596 End*/

	"HO.InterEnbOutAtt.ControlProcessingOverload",
	"HO.InterEnbOutAtt.NotEnoughUserPlaneProcessingResources",
	 /*SPR 19596 Start*/
    "HO.InterEnbOutAtt.HardwareFailure",
	"HO.InterEnbOutAtt.OMIntervention",
    /*SPR 19596 End*/

	"HO.InterEnbOutAtt.S1APUnspecified",
	"HO.InterEnbOutAtt.S1APUnknownPLMN",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"HO.InterEnbOutSucc.1.0",
	"HO.InterEnbOutSucc.1.1",
	"HO.InterEnbOutSucc.1.2",
	"HO.InterEnbOutSucc.1.3",
	"HO.InterEnbOutSucc.1.4",
	"HO.InterEnbOutSucc.1.5",
	"HO.InterEnbOutSucc.1.6",
	"HO.InterEnbOutSucc.1.7",
	"HO.InterEnbOutSucc.1.8",
	"HO.InterEnbOutSucc.1.9",
	"HO.InterEnbOutSucc.1.10",
	"HO.InterEnbOutSucc.1.11",
	"HO.InterEnbOutSucc.1.12",
	"HO.InterEnbOutSucc.1.13",
	"HO.InterEnbOutSucc.1.14",
	"HO.InterEnbOutSucc.1.15",
	"HO.InterEnbOutSucc.1.16",
	"HO.InterEnbOutSucc.1.17",
	"HO.InterEnbOutSucc.1.18",
	"HO.InterEnbOutSucc.1.19",
	"HO.InterEnbOutSucc.1.20",
	"HO.InterEnbOutSucc.1.21",
	"HO.InterEnbOutSucc.1.22",
	"HO.InterEnbOutSucc.1.23",
	"HO.InterEnbOutSucc.1.24",
	"HO.InterEnbOutSucc.1.25",
	"HO.InterEnbOutSucc.1.26",
	"HO.InterEnbOutSucc.1.27",
	"HO.InterEnbOutSucc.1.28",
	"HO.InterEnbOutSucc.1.29",
	"HO.InterEnbOutSucc.1.30",
	"HO.InterEnbOutSucc.1.31",
	"HO.InterEnbOutSucc.1.32",
	"HO.InterEnbOutSucc.1.33",
	"HO.InterEnbOutSucc.1.34",
	"HO.InterEnbOutSucc.1.35",
#else
    /*SPR 19596 End*/

	"HO.InterEnbOutSucc.Unspecified",
	"HO.InterEnbOutSucc.Tx2RelocOverallExpiry",
	"HO.InterEnbOutSucc.SuccessfulHandover",
	"HO.InterEnbOutSucc.ReleaseDueToEutranGeneratedReason",
	"HO.InterEnbOutSucc.HandoverCancelled",
	"HO.InterEnbOutSucc.PartialHandover",
	"HO.InterEnbOutSucc.HoFailureInTargetEPCeNBOrTargetSystem",
	"HO.InterEnbOutSucc.HoTargetNotAllowed",
	"HO.InterEnbOutSucc.TS1RelocOverallExpiry",
	"HO.InterEnbOutSucc.TS1RelocPrepExpiry",
	"HO.InterEnbOutSucc.CellNotAvailable",
	"HO.InterEnbOutSucc.UnknownTargetID",
	"HO.InterEnbOutSucc.NoRadioResourcesAvailableInTargetCell",
	"HO.InterEnbOutSucc.UnknownMmeUeS1apId",
	"HO.InterEnbOutSucc.UnknownEnbUeS1apId",
 	"HO.InterEnbOutSucc.UnknownPairUeS1apId",  /*SPR 19596*/
	"HO.InterEnbOutSucc.HandoverDesirableForRadioReason",
	"HO.InterEnbOutSucc.TimeCriticalHandover",
	"HO.InterEnbOutSucc.ResourceOptimisationHandover",
	"HO.InterEnbOutSucc.ReduceLoadInServingCell",
	"HO.InterEnbOutSucc.NoUserActivity",   /*SPR 19596*/
	"HO.InterEnbOutSucc.RadioConnectionWithUeLost",
	"HO.InterEnbOutSucc.LoadBalancingTauRequired",
	"HO.InterEnbOutSucc.CsFallbackTriggered",
	"HO.InterEnbOutSucc.UeNotAvailableForPsService",
	"HO.InterEnbOutSucc.RadioResourcesNotAvailable",
	"HO.InterEnbOutSucc.FailureInRadioInterfaceProcedure",
	"HO.InterEnbOutSucc.InvalidQosCombination",
	"HO.InterEnbOutSucc.InterratRedirection",
	"HO.InterEnbOutSucc.InteractionWithOtherProcedure",
	"HO.InterEnbOutSucc.UnknownERABID",
	"HO.InterEnbOutSucc.MultipleERABIDInstances",
	"HO.InterEnbOutSucc.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported",
	"HO.InterEnbOutSucc.S1IntraSystemHandoverTriggered",
	"HO.InterEnbOutSucc.S1InterSystemHandoverTriggered",
	"HO.InterEnbOutSucc.X2HandoverTriggered",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG
	"HO.InterEnbOutSucc.2.0",
	"HO.InterEnbOutSucc.2.1",
#else   
    /*SPR 19596 End*/

	"HO.InterEnbOutSucc.TransportResourceUnavailable",
	"HO.InterEnbOutSucc.TransportUnspecified",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG
	"HO.InterEnbOutSucc.3.0",
	"HO.InterEnbOutSucc.3.1",
	"HO.InterEnbOutSucc.3.2",
	"HO.InterEnbOutSucc.3.3",
#else   
    /*SPR 19596 End*/

	"HO.InterEnbOutSucc.NormalRelease",
	"HO.InterEnbOutSucc.AuthenticationFailure",
	"HO.InterEnbOutSucc.Detach",
	"HO.InterEnbOutSucc.Unspecified4",
    /*SPR 19596 Start*/

#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"HO.InterEnbOutSucc.4.0",
	"HO.InterEnbOutSucc.4.1",
	"HO.InterEnbOutSucc.4.2",
	"HO.InterEnbOutSucc.4.3",
	"HO.InterEnbOutSucc.4.4",
	"HO.InterEnbOutSucc.4.5",
	"HO.InterEnbOutSucc.4.6",
#else   
    /*SPR 19596 End*/

	"HO.InterEnbOutSucc.TransferSyntaxError",
	"HO.InterEnbOutSucc.AbstractSyntaxErrorReject",
	"HO.InterEnbOutSucc.AbstractSyntaxErrorIgnoreAndNotify",
	"HO.InterEnbOutSucc.MessageNotCompatibleWithReceiverState",
	"HO.InterEnbOutSucc.SemanticError",
	"HO.InterEnbOutSucc.AbstractSyntaxErrorFalselyConstructedMessage",
	"HO.InterEnbOutSucc.Unspecified_1",
    /*SPR 19596 Start*/
#endif
#if ENUM_KPI_CAUSE_FLAG
	"HO.InterEnbOutSucc.5.0",
	"HO.InterEnbOutSucc.5.1",
	"HO.InterEnbOutSucc.5.2",
	"HO.InterEnbOutSucc.5.3",
	"HO.InterEnbOutSucc.5.4",
	"HO.InterEnbOutSucc.5.5",
#else   
    /*SPR 19596 End*/

	"HO.InterEnbOutSucc.ControlProcessingOverload",
	"HO.InterEnbOutSucc.NotEnoughUserPlaneProcessingResources",
	 /*SPR 19596 Start*/
    "HO.InterEnbOutSucc.HardwareFailure",
	"HO.InterEnbOutSucc.OMIntervention",
    /*SPR 19596 End*/

	"HO.InterEnbOutSucc.S1APUnspecified",
	"HO.InterEnbOutSucc.S1APUnknownPLMN",
#endif     /*SPR 19596*/
	/*SPR 10987 FIX end*/
	"HO.IntraFreqOutAtt",
	"HO.IntraFreqOutSucc",
	"HO.InterFreqMeasGapOutAtt",
	"HO.InterFreqMeasGapOutSucc",
	"HO.InterFreqNoMeasGapOutAtt",
	"HO.InterFreqNoMeasGapOutSucc",
	"HO.DrxOutAtt",
	"HO.DrxOutSucc",
	"HO.NoDrxOutAtt",
	"HO.NoDrxOutSucc",
 /*SPR 19596 Start*/
#if ENUM_KPI_CAUSE_FLAG 
	"HO.IntraEnbOutAtt.1.0",
	"HO.IntraEnbOutAtt.1.1",
	"HO.IntraEnbOutAtt.1.2",
	"HO.IntraEnbOutAtt.1.3",
	"HO.IntraEnbOutAtt.1.4",
	"HO.IntraEnbOutAtt.1.5",
	"HO.IntraEnbOutAtt.1.6",
	"HO.IntraEnbOutAtt.1.7",
	"HO.IntraEnbOutAtt.1.8",
	"HO.IntraEnbOutAtt.1.9",
	"HO.IntraEnbOutAtt.1.10",
	"HO.IntraEnbOutAtt.1.11",
	"HO.IntraEnbOutAtt.1.12",
	"HO.IntraEnbOutAtt.1.13",
	"HO.IntraEnbOutAtt.1.14",
	"HO.IntraEnbOutAtt.1.15",
	"HO.IntraEnbOutAtt.1.16",
	"HO.IntraEnbOutAtt.1.17",
	"HO.IntraEnbOutAtt.1.18",
	"HO.IntraEnbOutAtt.1.19",
	"HO.IntraEnbOutAtt.1.20",
	"HO.IntraEnbOutAtt.1.21",
	"HO.IntraEnbOutAtt.1.22",
	"HO.IntraEnbOutAtt.1.23",
	"HO.IntraEnbOutAtt.1.24",
	"HO.IntraEnbOutAtt.1.25",
	"HO.IntraEnbOutAtt.1.26",
	"HO.IntraEnbOutAtt.1.27",
	"HO.IntraEnbOutAtt.1.28",
	"HO.IntraEnbOutAtt.1.29",
	"HO.IntraEnbOutAtt.1.30",
	"HO.IntraEnbOutAtt.1.31",
	"HO.IntraEnbOutAtt.1.32",
	"HO.IntraEnbOutAtt.1.33",
	"HO.IntraEnbOutAtt.1.34",
	"HO.IntraEnbOutAtt.1.35",
#else
    /*SPR 19596 End*/

	/*SPR 10987 FIX start*/
    /*SPR 19596 Code deleted*/

	"HO.IntraEnbOutAtt.Unspecified",
	"HO.IntraEnbOutAtt.Tx2RelocOverallExpiry",
	"HO.IntraEnbOutAtt.SuccessfulHandover",
	"HO.IntraEnbOutAtt.ReleaseDueToEutranGeneratedReason",
	"HO.IntraEnbOutAtt.HandoverCancelled",
	"HO.IntraEnbOutAtt.PartialHandover",
	"HO.IntraEnbOutAtt.HoFailureInTargetEPCeNBOrTargetSystem",
	"HO.IntraEnbOutAtt.HoTargetNotAllowed",
	"HO.IntraEnbOutAtt.TS1RelocOverallExpiry",
	"HO.IntraEnbOutAtt.TS1RelocPrepExpiry",
	"HO.IntraEnbOutAtt.CellNotAvailable",
	"HO.IntraEnbOutAtt.UnknownTargetID",
	"HO.IntraEnbOutAtt.NoRadioResourcesAvailableInTargetCell",
	"HO.IntraEnbOutAtt.UnknownMmeUeS1apId",
	"HO.IntraEnbOutAtt.UnknownEnbUeS1apId",
	"HO.IntraEnbOutAtt.UnknownPairUeS1apId",  /*SPR 19596*/
	"HO.IntraEnbOutAtt.HandoverDesirableForRadioReason",
	"HO.IntraEnbOutAtt.TimeCriticalHandover",
	"HO.IntraEnbOutAtt.ResourceOptimisationHandover",
	"HO.IntraEnbOutAtt.ReduceLoadInServingCell",
	"HO.IntraEnbOutAtt.NoUserActivity",  /*SPR 19596*/
	"HO.IntraEnbOutAtt.RadioConnectionWithUeLost",
	"HO.IntraEnbOutAtt.LoadBalancingTauRequired",
	"HO.IntraEnbOutAtt.CsFallbackTriggered",
	"HO.IntraEnbOutAtt.UeNotAvailableForPsService",
	"HO.IntraEnbOutAtt.RadioResourcesNotAvailable",
	"HO.IntraEnbOutAtt.FailureInRadioInterfaceProcedure",
	"HO.IntraEnbOutAtt.InvalidQosCombination",
	"HO.IntraEnbOutAtt.InterratRedirection",
	"HO.IntraEnbOutAtt.InteractionWithOtherProcedure",
	"HO.IntraEnbOutAtt.UnknownERABID",
	"HO.IntraEnbOutAtt.MultipleERABIDInstances",
	"HO.IntraEnbOutAtt.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported",
	"HO.IntraEnbOutAtt.S1IntraSystemHandoverTriggered",
	"HO.IntraEnbOutAtt.S1InterSystemHandoverTriggered",
	"HO.IntraEnbOutAtt.X2HandoverTriggered",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"HO.IntraEnbOutAtt.2.0",
	"HO.IntraEnbOutAtt.2.1",
#else    
    /*SPR 19596 End*/

	"HO.IntraEnbOutAtt.TransportResourceUnavailable",
	"HO.IntraEnbOutAtt.TransportUnspecified",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG
	"HO.IntraEnbOutAtt.3.0",
	"HO.IntraEnbOutAtt.3.1",
	"HO.IntraEnbOutAtt.3.2",
	"HO.IntraEnbOutAtt.3.3",
#else   
    /*SPR 19596 End*/

	"HO.IntraEnbOutAtt.NormalRelease",
	"HO.IntraEnbOutAtt.AuthenticationFailure",
	"HO.IntraEnbOutAtt.Detach",
	"HO.IntraEnbOutAtt.Unspecified4",
 /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG
	"HO.IntraEnbOutAtt.4.0",
	"HO.IntraEnbOutAtt.4.1",
	"HO.IntraEnbOutAtt.4.2",
	"HO.IntraEnbOutAtt.4.3",
	"HO.IntraEnbOutAtt.4.4",
	"HO.IntraEnbOutAtt.4.5",
	"HO.IntraEnbOutAtt.4.6",
#else
    /*SPR 19596 End*/

	"HO.IntraEnbOutAtt.TransferSyntaxError",
	"HO.IntraEnbOutAtt.AbstractSyntaxErrorReject",
	"HO.IntraEnbOutAtt.AbstractSyntaxErrorIgnoreAndNotify",
	"HO.IntraEnbOutAtt.MessageNotCompatibleWithReceiverState",
	"HO.IntraEnbOutAtt.SemanticError",
	"HO.IntraEnbOutAtt.AbstractSyntaxErrorFalselyConstructedMessage",
	"HO.IntraEnbOutAtt.Unspecified_1",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG
	"HO.IntraEnbOutAtt.5.0",
	"HO.IntraEnbOutAtt.5.1",
	"HO.IntraEnbOutAtt.5.2",
	"HO.IntraEnbOutAtt.5.3",
	"HO.IntraEnbOutAtt.5.4",
	"HO.IntraEnbOutAtt.5.5",
#else
    /*SPR 19596 End*/

	"HO.IntraEnbOutAtt.ControlProcessingOverload",
	"HO.IntraEnbOutAtt.NotEnoughUserPlaneProcessingResources",
	 /*SPR 19596 Start*/
    "HO.IntraEnbOutAtt.HardwareFailure",
	"HO.IntraEnbOutAtt.OMIntervention",
    /*SPR 19596 End*/

	"HO.IntraEnbOutAtt.S1APUnspecified",
	"HO.IntraEnbOutAtt.S1APUnknownPLMN",
#endif         /*SPR 19596*/
	"HO.IntraEnbOutAtt.Sum",
 /*SPR 19596 Start*/
#if ENUM_KPI_CAUSE_FLAG 
	"HO.IntraEnbOutSucc.1.0",
	"HO.IntraEnbOutSucc.1.1",
	"HO.IntraEnbOutSucc.1.2",
	"HO.IntraEnbOutSucc.1.3",
	"HO.IntraEnbOutSucc.1.4",
	"HO.IntraEnbOutSucc.1.5",
	"HO.IntraEnbOutSucc.1.6",
	"HO.IntraEnbOutSucc.1.7",
	"HO.IntraEnbOutSucc.1.8",
	"HO.IntraEnbOutSucc.1.9",
	"HO.IntraEnbOutSucc.1.10",
	"HO.IntraEnbOutSucc.1.11",
	"HO.IntraEnbOutSucc.1.12",
	"HO.IntraEnbOutSucc.1.13",
	"HO.IntraEnbOutSucc.1.14",
	"HO.IntraEnbOutSucc.1.15",
	"HO.IntraEnbOutSucc.1.16",
	"HO.IntraEnbOutSucc.1.17",
	"HO.IntraEnbOutSucc.1.18",
	"HO.IntraEnbOutSucc.1.19",
	"HO.IntraEnbOutSucc.1.20",
	"HO.IntraEnbOutSucc.1.21",
	"HO.IntraEnbOutSucc.1.22",
	"HO.IntraEnbOutSucc.1.23",
	"HO.IntraEnbOutSucc.1.24",
	"HO.IntraEnbOutSucc.1.25",
	"HO.IntraEnbOutSucc.1.26",
	"HO.IntraEnbOutSucc.1.27",
	"HO.IntraEnbOutSucc.1.28",
	"HO.IntraEnbOutSucc.1.29",
	"HO.IntraEnbOutSucc.1.30",
	"HO.IntraEnbOutSucc.1.31",
	"HO.IntraEnbOutSucc.1.32",
	"HO.IntraEnbOutSucc.1.33",
	"HO.IntraEnbOutSucc.1.34",
	"HO.IntraEnbOutSucc.1.35",
#else   
    /*SPR 19596 End*/

	"HO.IntraEnbOutSucc.Unspecified",
	"HO.IntraEnbOutSucc.Tx2RelocOverallExpiry",
	"HO.IntraEnbOutSucc.SuccessfulHandover",
	"HO.IntraEnbOutSucc.ReleaseDueToEutranGeneratedReason",
	"HO.IntraEnbOutSucc.HandoverCancelled",
	"HO.IntraEnbOutSucc.PartialHandover",
	"HO.IntraEnbOutSucc.HoFailureInTargetEPCeNBOrTargetSystem",
	"HO.IntraEnbOutSucc.HoTargetNotAllowed",
	"HO.IntraEnbOutSucc.TS1RelocOverallExpiry",
	"HO.IntraEnbOutSucc.TS1RelocPrepExpiry",
	"HO.IntraEnbOutSucc.CellNotAvailable",
	"HO.IntraEnbOutSucc.UnknownTargetID",
	"HO.IntraEnbOutSucc.NoRadioResourcesAvailableInTargetCell",
	"HO.IntraEnbOutSucc.UnknownMmeUeS1apId",
	"HO.IntraEnbOutSucc.UnknownEnbUeS1apId",
 	"HO.IntraEnbOutSucc.UnknownPairUeS1apId",  /*SPR 19596*/
	"HO.IntraEnbOutSucc.HandoverDesirableForRadioReason",
	"HO.IntraEnbOutSucc.TimeCriticalHandover",
	"HO.IntraEnbOutSucc.ResourceOptimisationHandover",
	"HO.IntraEnbOutSucc.ReduceLoadInServingCell",
	"HO.IntraEnbOutSucc.NoUserActivity",  /*SPR 19596*/
	"HO.IntraEnbOutSucc.RadioConnectionWithUeLost",
	"HO.IntraEnbOutSucc.LoadBalancingTauRequired",
	"HO.IntraEnbOutSucc.CsFallbackTriggered",
	"HO.IntraEnbOutSucc.UeNotAvailableForPsService",
	"HO.IntraEnbOutSucc.RadioResourcesNotAvailable",
	"HO.IntraEnbOutSucc.FailureInRadioInterfaceProcedure",
	"HO.IntraEnbOutSucc.InvalidQosCombination",
	"HO.IntraEnbOutSucc.InterratRedirection",
	"HO.IntraEnbOutSucc.InteractionWithOtherProcedure",
	"HO.IntraEnbOutSucc.UnknownERABID",
	"HO.IntraEnbOutSucc.MultipleERABIDInstances",
	"HO.IntraEnbOutSucc.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported",
	"HO.IntraEnbOutSucc.S1IntraSystemHandoverTriggered",
	"HO.IntraEnbOutSucc.S1InterSystemHandoverTriggered",
	"HO.IntraEnbOutSucc.X2HandoverTriggered",
    /*SPR 19596 Start*/

#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"HO.IntraEnbOutSucc.2.0",
	"HO.IntraEnbOutSucc.2.1",
#else
    /*SPR 19596 End*/

	"HO.IntraEnbOutSucc.TransportResourceUnavailable",
	"HO.IntraEnbOutSucc.TransportUnspecified",
    /*SPR 19596 Start*/

#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"HO.IntraEnbOutSucc.3.0",
	"HO.IntraEnbOutSucc.3.1",
	"HO.IntraEnbOutSucc.3.2",
	"HO.IntraEnbOutSucc.3.3",
#else   
    /*SPR 19596 End*/

	"HO.IntraEnbOutSucc.NormalRelease",
	"HO.IntraEnbOutSucc.AuthenticationFailure",
	"HO.IntraEnbOutSucc.Detach",
	"HO.IntraEnbOutSucc.Unspecified4",
    /*SPR 19596 Start*/

#endif
#if ENUM_KPI_CAUSE_FLAG 
	"HO.IntraEnbOutSucc.4.0",
	"HO.IntraEnbOutSucc.4.1",
	"HO.IntraEnbOutSucc.4.2",
	"HO.IntraEnbOutSucc.4.3",
	"HO.IntraEnbOutSucc.4.4",
	"HO.IntraEnbOutSucc.4.5",
	"HO.IntraEnbOutSucc.4.6",
#else   
    /*SPR 19596 End*/

	"HO.IntraEnbOutSucc.TransferSyntaxError",
	"HO.IntraEnbOutSucc.AbstractSyntaxErrorReject",
	"HO.IntraEnbOutSucc.AbstractSyntaxErrorIgnoreAndNotify",
	"HO.IntraEnbOutSucc.MessageNotCompatibleWithReceiverState",
	"HO.IntraEnbOutSucc.SemanticError",
	"HO.IntraEnbOutSucc.AbstractSyntaxErrorFalselyConstructedMessage",
	"HO.IntraEnbOutSucc.Unspecified_1",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"HO.IntraEnbOutSucc.5.0",
	"HO.IntraEnbOutSucc.5.1",
	"HO.IntraEnbOutSucc.5.2",
	"HO.IntraEnbOutSucc.5.3",
	"HO.IntraEnbOutSucc.5.4",
	"HO.IntraEnbOutSucc.5.5",
#else
    /*SPR 19596 End*/

	"HO.IntraEnbOutSucc.ControlProcessingOverload",
	"HO.IntraEnbOutSucc.NotEnoughUserPlaneProcessingResources",
	 /*SPR 19596 Start*/
    "HO.IntraEnbOutSucc.HardwareFailure",
	"HO.IntraEnbOutSucc.OMIntervention",
    /*SPR 19596 End*/
	"HO.IntraEnbOutSucc.S1APUnspecified",
	"HO.IntraEnbOutSucc.S1APUnknownPLMN",
#endif     /*SPR 19596 */
	"HO.IntraEnbOutSucc.Sum",
	"HO.RRC.IratIncMobility.LAI",
	/* SPR_12747_CHANGES */
	/*SPR 10987 FIX end*/
	OAM_NULL};

/* SPR_12747_CHANGES */
char *pm_xml_ho_target_param_name[]={
/*SPR 14383 START*/
#if ENUM_KPI_CAUSE_FLAG
    /*SPR 19596 Start*/
	"HO.IartOutAtt.1.1",
    "HO.IartOutAtt.1.2",
    "HO.IartOutAtt.1.3",
    "HO.IartOutAtt.1.4",
    "HO.IartOutAtt.1.5",
    "HO.IartOutAtt.1.6",
    "HO.IartOutAtt.1.7",
    "HO.IartOutAtt.1.8",
    "HO.IartOutAtt.1.9",
    "HO.IartOutAtt.1.10",
    "HO.IartOutAtt.1.11",
    "HO.IartOutAtt.1.12",
    "HO.IartOutAtt.1.13",
    "HO.IartOutAtt.1.14",
    "HO.IartOutAtt.1.15",
    "HO.IartOutAtt.1.16",
    "HO.IartOutAtt.1.17",
    "HO.IartOutAtt.1.18",
    "HO.IartOutAtt.1.19",
    "HO.IartOutAtt.1.20",
    "HO.IartOutAtt.1.21",
    "HO.IartOutAtt.1.22",
    "HO.IartOutAtt.1.23",
    "HO.IartOutAtt.1.24",
    "HO.IartOutAtt.1.25",
    "HO.IartOutAtt.1.26",
    "HO.IartOutAtt.1.27",
    "HO.IartOutAtt.1.28",
    "HO.IartOutAtt.1.29",
    "HO.IartOutAtt.1.30",
    "HO.IartOutAtt.1.31",
    "HO.IartOutAtt.1.32",
    "HO.IartOutAtt.1.33",
    "HO.IartOutAtt.1.34",
    "HO.IartOutAtt.1.35",
    /*SPR 19596 End*/
#else
/*SPR 14383 END*/    
    "HO.IartOutAtt.Tx2RelocOverallExpiry",
	"HO.IartOutAtt.SuccessfulHandover",
	"HO.IartOutAtt.ReleaseDueToEutranGeneratedReason",
	"HO.IartOutAtt.HandoverCancelled",
	"HO.IartOutAtt.PartialHandover",
	"HO.IartOutAtt.HoFailureInTargetEPCeNBOrTargetSystem",
	"HO.IartOutAtt.HoTargetNotAllowed",
	"HO.IartOutAtt.TS1RelocOverallExpiry",
	"HO.IartOutAtt.TS1RelocPrepExpiry",
	"HO.IartOutAtt.CellNotAvailable",
	"HO.IartOutAtt.UnknownTargetID",
	"HO.IartOutAtt.NoRadioResourcesAvailableInTargetCell",
	"HO.IartOutAtt.UnknownMmeUeS1apId",
	"HO.IartOutAtt.UnknownEnbUeS1apId",
	"HO.IartOutAtt.UnknownPairUeS1apId",
	"HO.IartOutAtt.HandOverDesirableForRadioReason",
	"HO.IartOutAtt.TimeCriticalHandover",
	"HO.IartOutAtt.ResourceOptimisationHandover",
	"HO.IartOutAtt.ReduceLoadInServingCell",
	"HO.IartOutAtt.UserInactivity",
	"HO.IartOutAtt.RadioConnectionWithUeLost",
	"HO.IartOutAtt.LoadBalancingTauRequired",
	"HO.IartOutAtt.CsFallbackTriggered",
	"HO.IartOutAtt.UeNotAvailableForPsService",
	"HO.IartOutAtt.RadioResourcesNotAvailable",
	"HO.IartOutAtt.FailureInRadioInterfaceProcedure",
	"HO.IartOutAtt.InvalidQosCombination",
	"HO.IartOutAtt.InterratRedirection",
	"HO.IartOutAtt.InteractionWithOtherProcedure",
	"HO.IartOutAtt.UnknownERABID",
	"HO.IartOutAtt.MultipleERABIDInstances",
	"HO.IartOutAtt.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported",
	"HO.IartOutAtt.S1IntraSystemHandoverTriggered",
	"HO.IartOutAtt.S1InterSystemHandoverTriggered",
	"HO.IartOutAtt.X2HOTriggered",
/*SPR 14383 START*/    
#endif
/*SPR 14383 END*/
    /*SPR 19596 Start*/
#if ENUM_KPI_CAUSE_FLAG
	"HO.IartOutAtt.2.0",
	"HO.IartOutAtt.2.1",
#else
    /*SPR 19596 End*/

    /* SPR 14383 END */
	"HO.IartOutAtt.TransportResourceUnavailable",
	"HO.IartOutAtt.Unspecified_3",
 /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"HO.IartOutAtt.3.0",
	"HO.IartOutAtt.3.1",
	"HO.IartOutAtt.3.2",
	"HO.IartOutAtt.3.3",
#else   
    /*SPR 19596 End*/

	"HO.IartOutAtt.NormalRelease",
	"HO.IartOutAtt.AuthenticationFailure",
	"HO.IartOutAtt.Detach",
	"HO.IartOutAtt.Unspecified_4",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"HO.IartOutAtt.4.0",
	"HO.IartOutAtt.4.1",
	"HO.IartOutAtt.4.2",
	"HO.IartOutAtt.4.3",
	"HO.IartOutAtt.4.4",
	"HO.IartOutAtt.4.5",
	"HO.IartOutAtt.4.6",
#else
    /*SPR 19596 End*/
	"HO.IartOutAtt.TransferSyntaxError",
	"HO.IartOutAtt.AbstractSyntaxErrorReject",
	"HO.IartOutAtt.AbstractSyntaxErrorIgnoreAndNotify",
	"HO.IartOutAtt.MessageNotCompatibleWithReceiverState",
	"HO.IartOutAtt.SemanticError",
	"HO.IartOutAtt.AbstractSyntaxErrorFalselyConstructedMessage",
	"HO.IartOutAtt.Unspecified_1",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"HO.IartOutAtt.5.0",
	"HO.IartOutAtt.5.1",
	"HO.IartOutAtt.5.2",
	"HO.IartOutAtt.5.3",
	"HO.IartOutAtt.5.4",
	"HO.IartOutAtt.5.5",
#else   
    /*SPR 19596 End*/

	"HO.IartOutAtt.ControlProcessingOverload",
	"HO.IartOutAtt.NotEnoughUserPlaneProcessingResources",
	 /*SPR 19596 Start*/
    "HO.IartOutAtt.HardwareFailure",
	"HO.IartOutAtt.OmIntervention",
    /*SPR 19596 End*/
	"HO.IartOutAtt.Unspecified",
	"HO.IartOutAtt.UnknownPLMN",
#endif     /*SPR 19596*/

	"HO.IartOutAtt.Sum",
    /*SPR 19596 Code deleted*/


    /*SPR 14383 START*/
#if ENUM_KPI_CAUSE_FLAG
    /*SPR 19596 Start*/

    "HO.IartOutSucc.1.0",
    "HO.IartOutSucc.1.1",
    "HO.IartOutSucc.1.2",
    "HO.IartOutSucc.1.3",
    "HO.IartOutSucc.1.4",
    "HO.IartOutSucc.1.5",
    "HO.IartOutSucc.1.6",
    "HO.IartOutSucc.1.7",
    "HO.IartOutSucc.1.8",
    "HO.IartOutSucc.1.9",
    "HO.IartOutSucc.1.10",
    "HO.IartOutSucc.1.11",
    "HO.IartOutSucc.1.12",
    "HO.IartOutSucc.1.13",
    "HO.IartOutSucc.1.14",
    "HO.IartOutSucc.1.15",
    "HO.IartOutSucc.1.16",
    "HO.IartOutSucc.1.17",
    "HO.IartOutSucc.1.18",
    "HO.IartOutSucc.1.19",
    "HO.IartOutSucc.1.20",
    "HO.IartOutSucc.1.21",
    "HO.IartOutSucc.1.22",
    "HO.IartOutSucc.1.23",
    "HO.IartOutSucc.1.24",
    "HO.IartOutSucc.1.25",
    "HO.IartOutSucc.1.26",
    "HO.IartOutSucc.1.27",
    "HO.IartOutSucc.1.28",
    "HO.IartOutSucc.1.29",
    "HO.IartOutSucc.1.30",
    "HO.IartOutSucc.1.31",
    "HO.IartOutSucc.1.32",
    "HO.IartOutSucc.1.33",
    "HO.IartOutSucc.1.34",
    "HO.IartOutSucc.1.35",
    /*SPR 19596 End*/

#else
    /*SPR 14383 END*/
	"HO.IartOutSucc.Unspecified_2",        /*SPR 19596*/
	"HO.IartOutSucc.Tx2RelocOverallExpiry",
	"HO.IartOutSucc.SuccessfulHandover",
	"HO.IartOutSucc.ReleaseDueToEutranGeneratedReason",
	"HO.IartOutSucc.HandoverCancelled",
	"HO.IartOutSucc.PartialHandover",
	"HO.IartOutSucc.HoFailureInTargetEPCeNBOrTargetSystem",
	"HO.IartOutSucc.HoTargetNotAllowed",
	"HO.IartOutSucc.TS1RelocOverallExpiry",
	"HO.IartOutSucc.TS1RelocPrepExpiry",
	"HO.IartOutSucc.CellNotAvailable",
	"HO.IartOutSucc.UnknownTargetID",
	"HO.IartOutSucc.NoRadioResourcesAvailableInTargetCell",
	"HO.IartOutSucc.UnknownMmeUeS1apId",
	"HO.IartOutSucc.UnknownEnbUeS1apId",
	"HO.IartOutSucc.UnknownPairUeS1apId",
	"HO.IartOutSucc.HandoverDesirableForRadioReason",
	"HO.IartOutSucc.TimeCriticalHandover",
	"HO.IartOutSucc.ResourceOptimisationHandover",
	"HO.IartOutSucc.ReduceLoadInServingCell",
	"HO.IartOutSucc.UserInactivity",
	"HO.IartOutSucc.RadioConnectionWithUeLost",
	"HO.IartOutSucc.LoadBalancingTauRequired",
	"HO.IartOutSucc.CsFallbackTriggered",
	"HO.IartOutSucc.UeNotAvailableForPsService",
	"HO.IartOutSucc.RadioResourcesNotAvailable",
	"HO.IartOutSucc.FailureInRadioInterfaceProcedure",
	"HO.IartOutSucc.InvalidQosCombination",
	"HO.IartOutSucc.InterratRedirection",
	"HO.IartOutSucc.InteractionWithOtherProcedure",
	"HO.IartOutSucc.UnknownERABID",
	"HO.IartOutSucc.MultipleERABIDInstances",
	"HO.IartOutSucc.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported",
	"HO.IartOutSucc.S1IntraSystemHandoverTriggered",
	"HO.IartOutSucc.S1InterSystemHandoverTriggered",
	"HO.IartOutSucc.X2HOTriggered",
/*SPR 14383 START*/    
#endif
/* SPR 14383 END */
 /*SPR 19596 Start*/
#if ENUM_KPI_CAUSE_FLAG
	"HO.IartOutSucc.2.0",
	"HO.IartOutSucc.2.1",
#else   
    /*SPR 19596 End*/

	"HO.IartOutSucc.TransportResourceUnavailable",
	"HO.IartOutSucc.Unspecified_3",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG
	"HO.IartOutSucc.3.0",
	"HO.IartOutSucc.3.1",
	"HO.IartOutSucc.3.2",
	"HO.IartOutSucc.3.3",
#else   
    /*SPR 19596 End*/

	"HO.IartOutSucc.NormalRelease",
	"HO.IartOutSucc.AuthenticationFailure",
	"HO.IartOutSucc.Detach",
	"HO.IartOutSucc.Unspecified_4",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"HO.IartOutSucc.4.0",
	"HO.IartOutSucc.4.1",
	"HO.IartOutSucc.4.2",
	"HO.IartOutSucc.4.3",
	"HO.IartOutSucc.4.4",
	"HO.IartOutSucc.4.5",
	"HO.IartOutSucc.4.6",
#else   
    /*SPR 19596 End*/

	"HO.IartOutSucc.TransferSyntaxError",
	"HO.IartOutSucc.AbstractSyntaxErrorReject",
	"HO.IartOutSucc.AbstractSyntaxErrorIgnoreAndNotify",
	"HO.IartOutSucc.MessageNotCompatibleWithReceiverState",
	"HO.IartOutSucc.SemanticError",
	"HO.IartOutSucc.AbstractSyntaxErrorFalselyConstructedMessage",
	"HO.IartOutSucc.Unspecified_1",
 /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"HO.IartOutSucc.5.0",
	"HO.IartOutSucc.5.1",
	"HO.IartOutSucc.5.2",
	"HO.IartOutSucc.5.3",
	"HO.IartOutSucc.5.4",
	"HO.IartOutSucc.5.5",
#else
	"HO.IartOutSucc.ControlProcessingOverload",
	"HO.IartOutSucc.NotEnoughUserPlaneProcessingResources",
	"HO.IartOutSucc.HardwareFailure",
    /*SPR 19596 End*/

	"HO.IartOutSucc.OmIntervention",
    /*SPR 19596 Code Deleted*/

	"HO.IartOutSucc.Unspecified",
	"HO.IartOutSucc.UnknownPLMN",
#endif         /*SPR 19596*/ 
	"HO.IartOutSucc.Sum",
	"HO.Failure.Num",
    /*SPR 19596 Start*/
#if ENUM_KPI_CAUSE_FLAG
	"HO.OutAttTarget.1.0",
	"HO.OutAttTarget.1.1",
	"HO.OutAttTarget.1.2",
	"HO.OutAttTarget.1.3",
	"HO.OutAttTarget.1.4",
	"HO.OutAttTarget.1.5",
	"HO.OutAttTarget.1.6",
	"HO.OutAttTarget.1.7",
	"HO.OutAttTarget.1.8",
	"HO.OutAttTarget.1.9",
	"HO.OutAttTarget.1.10",
	"HO.OutAttTarget.1.11",
	"HO.OutAttTarget.1.12",
	"HO.OutAttTarget.1.13",
	"HO.OutAttTarget.1.14",
	"HO.OutAttTarget.1.15",
	"HO.OutAttTarget.1.16",
	"HO.OutAttTarget.1.17",
	"HO.OutAttTarget.1.18",
	"HO.OutAttTarget.1.19",
	"HO.OutAttTarget.1.20",
	"HO.OutAttTarget.1.21",
	"HO.OutAttTarget.1.22",
	"HO.OutAttTarget.1.23",
	"HO.OutAttTarget.1.24",
	"HO.OutAttTarget.1.25",
	"HO.OutAttTarget.1.26",
	"HO.OutAttTarget.1.27",
	"HO.OutAttTarget.1.28",
	"HO.OutAttTarget.1.29",
	"HO.OutAttTarget.1.30",
	"HO.OutAttTarget.1.31",
	"HO.OutAttTarget.1.32",
	"HO.OutAttTarget.1.33",
	"HO.OutAttTarget.1.34",
	"HO.OutAttTarget.1.35",
#else
    /*SPR 19596 End*/

	"HO.OutAttTarget.Unspecified_2",
	"HO.OutAttTarget.Tx2RelocOverallExpiry",
	"HO.OutAttTarget.SuccessfulHandover",
	"HO.OutAttTarget.ReleaseDueToEutranGeneratedReason",
	"HO.OutAttTarget.HandoverCancelled",
	"HO.OutAttTarget.PartialHandover",
	"HO.OutAttTarget.HoFailureInTargetEPCeNBOrTargetSystem",
	"HO.OutAttTarget.HoTargetNotAllowed",
	"HO.OutAttTarget.TS1RelocOverallExpiry",
	"HO.OutAttTarget.TS1RelocPrepExpiry",
	"HO.OutAttTarget.CellNotAvailable",
	"HO.OutAttTarget.UnknownTargetID",
	"HO.OutAttTarget.NoRadioResourcesAvailableInTargetCell",
	"HO.OutAttTarget.UnknownMmeUeS1apId",
	"HO.OutAttTarget.UnknownEnbUeS1apIdUECNTX",
	"HO.OutAttTarget.UnknownPairUeS1apId",  /*SPR 19596*/
	"HO.OutAttTarget.HandoverDesirableForRadioReason",
	"HO.OutAttTarget.TimeCriticalHandover",
	"HO.OutAttTarget.ResourceOptimisationHandover",
	"HO.OutAttTarget.ReduceLoadInServingCell",
	"HO.OutAttTarget.UserInactivity",   /*SPR 19596*/
	"HO.OutAttTarget.RadioConnectionWithUeLost",
	"HO.OutAttTarget.LoadBalancingTauRequired",
	"HO.OutAttTarget.CsFallbackTriggered",
	"HO.OutAttTarget.UeNotAvailableForPsService",
	"HO.OutAttTarget.RadioResourcesNotAvailable",
	"HO.OutAttTarget.FailureInRadioInterfaceProcedure",
	"HO.OutAttTarget.InvalidQosCombination",
	"HO.OutAttTarget.InterratRedirection",
	"HO.OutAttTarget.InteractionWithOtherProcedure",
	"HO.OutAttTarget.UnknownERABID",
	"HO.OutAttTarget.MultipleERABIDInstances",
	"HO.OutAttTarget.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported",
	"HO.OutAttTarget.S1IntraSystemHandoverTriggered",
	"HO.OutAttTarget.S1InterSystemHandoverTriggered",
	"HO.OutAttTarget.X2HandoverTriggered",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG
	"HO.OutAttTarget.2.0",
	"HO.OutAttTarget.2.1",
#else
    /*SPR 19596 End*/
	"HO.OutAttTarget.TransportResourceUnavailable",
	"HO.OutAttTarget.TransportUnspecified",
    /*SPR 19596 Start*/
#endif
#if ENUM_KPI_CAUSE_FLAG
	"HO.OutAttTarget.3.0",
	"HO.OutAttTarget.3.1",
	"HO.OutAttTarget.3.2",
	"HO.OutAttTarget.3.3",
#else
    /*SPR 19596 End*/

	"HO.OutAttTarget.NormalRelease",
	"HO.OutAttTarget.AuthenticationFailure",
	"HO.OutAttTarget.Detach",
	"HO.OutAttTarget.Unspecified4",
    /*SPR 19596 Start*/
#endif
#if ENUM_KPI_CAUSE_FLAG 
	"HO.OutAttTarget.4.0",
	"HO.OutAttTarget.4.1",
	"HO.OutAttTarget.4.2",
	"HO.OutAttTarget.4.3",
	"HO.OutAttTarget.4.4",
	"HO.OutAttTarget.4.5",
	"HO.OutAttTarget.4.6",
#else   
    /*SPR 19596 End*/

	"HO.OutAttTarget.TransferSyntaxError",
	"HO.OutAttTarget.AbstractSyntaxErrorReject",
	"HO.OutAttTarget.AbstractSyntaxErrorIgnoreAndNotify",
	"HO.OutAttTarget.MessageNotCompatibleWithReceiverState",
    /*SPR 19596 Start*/
    "HO.OutAttTarget.SemanticError",
    "HO.OutAttTarget.AbstractSyntaxErrorFalselyConstructedMessage",
    "HO.OutAttTarget.Unspecified_1",	
#endif

#if ENUM_KPI_CAUSE_FLAG 
	"HO.OutAttTarget.5.0",
	"HO.OutAttTarget.5.1",
	"HO.OutAttTarget.5.2",
	"HO.OutAttTarget.5.3",
	"HO.OutAttTarget.5.4",
	"HO.OutAttTarget.5.5",
#else
    /*SPR 19596 End*/

	"HO.OutAttTarget.ControlProcessingOverload",
	"HO.OutAttTarget.NotEnoughUserPlaneProcessingResources",
	 /*SPR 19596 Start*/
    "HO.OutAttTarget.HardwareFailure",
	"HO.OutAttTarget.OMIntervention",
    /*SPR 19596 End*/
	"HO.OutAttTarget.S1APUnspecified",
	"HO.OutAttTarget.S1APUnknownPLMN",
#endif     /*SPR 19596*/
	"HO.OutAttTarget.Sum",
    /*SPR 19596 Start*/
#if ENUM_KPI_CAUSE_FLAG 
	"HO.OutSuccTarget.1.0",
	"HO.OutSuccTarget.1.1",
	"HO.OutSuccTarget.1.2",
	"HO.OutSuccTarget.1.3",
	"HO.OutSuccTarget.1.4",
	"HO.OutSuccTarget.1.5",
	"HO.OutSuccTarget.1.6",
	"HO.OutSuccTarget.1.7",
	"HO.OutSuccTarget.1.8",
	"HO.OutSuccTarget.1.9",
	"HO.OutSuccTarget.1.10",
	"HO.OutSuccTarget.1.11",
	"HO.OutSuccTarget.1.12",
	"HO.OutSuccTarget.1.13",
	"HO.OutSuccTarget.1.14",
	"HO.OutSuccTarget.1.15",
	"HO.OutSuccTarget.1.16",
	"HO.OutSuccTarget.1.17",
	"HO.OutSuccTarget.1.18",
	"HO.OutSuccTarget.1.19",
	"HO.OutSuccTarget.1.20",
	"HO.OutSuccTarget.1.21",
	"HO.OutSuccTarget.1.22",
	"HO.OutSuccTarget.1.23",
	"HO.OutSuccTarget.1.24",
	"HO.OutSuccTarget.1.25",
	"HO.OutSuccTarget.1.26",
	"HO.OutSuccTarget.1.27",
	"HO.OutSuccTarget.1.28",
	"HO.OutSuccTarget.1.29",
	"HO.OutSuccTarget.1.30",
	"HO.OutSuccTarget.1.31",
	"HO.OutSuccTarget.1.32",
	"HO.OutSuccTarget.1.33",
	"HO.OutSuccTarget.1.34",
	"HO.OutSuccTarget.1.35",
#else
    /*SPR 19596 End*/

	"HO.OutSuccTarget.Unspecified_2",
	"HO.OutSuccTarget.Tx2RelocOverallExpiry",
	"HO.OutSuccTarget.SuccessfulHandover",
	"HO.OutSuccTarget.ReleaseDueToEutranGeneratedReason",
	"HO.OutSuccTarget.HandoverCancelled",
	"HO.OutSuccTarget.PartialHandover",
	"HO.OutSuccTarget.HoFailureInTargetEPCeNBOrTargetSystem",
	"HO.OutSuccTarget.HoTargetNotAllowed",
	"HO.OutSuccTarget.TS1RelocOverallExpiry",
	"HO.OutSuccTarget.TS1RelocPrepExpiry",
	"HO.OutSuccTarget.CellNotAvailable",
	"HO.OutSuccTarget.UnknownTargetID",
	"HO.OutSuccTarget.NoRadioResourcesAvailableInTargetCell",
	"HO.OutSuccTarget.UnknownMmeUeS1apId",
	"HO.OutSuccTarget.UnknownEnbUeS1apIdUECNTX",
	"HO.OutSuccTarget.UnknownPairUeS1apId",   /*SPR 19596*/
	"HO.OutSuccTarget.HandoverDesirableForRadioReason",
	"HO.OutSuccTarget.TimeCriticalHandover",
	"HO.OutSuccTarget.ResourceOptimisationHandover",
	"HO.OutSuccTarget.ReduceLoadInServingCell",
	"HO.OutSuccTarget.UserInactivity",   /*SPR 19596*/
	"HO.OutSuccTarget.RadioConnectionWithUeLost",
	"HO.OutSuccTarget.LoadBalancingTauRequired",
	"HO.OutSuccTarget.CsFallbackTriggered",
	"HO.OutSuccTarget.UeNotAvailableForPsService",
	"HO.OutSuccTarget.RadioResourcesNotAvailable",
	"HO.OutSuccTarget.FailureInRadioInterfaceProcedure",
	"HO.OutSuccTarget.InvalidQosCombination",
	"HO.OutSuccTarget.InterratRedirection",
	"HO.OutSuccTarget.InteractionWithOtherProcedure",
	"HO.OutSuccTarget.UnknownERABID",
	"HO.OutSuccTarget.MultipleERABIDInstances",
	"HO.OutSuccTarget.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported",
	"HO.OutSuccTarget.S1IntraSystemHandoverTriggered",
	"HO.OutSuccTarget.S1InterSystemHandoverTriggered",
	"HO.OutSuccTarget.X2HandoverTriggered",
    /*SPR 19596 Start*/

#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"HO.OutSuccTarget.2.0",
	"HO.OutSuccTarget.2.1",
#else
    /*SPR 19596 End*/

	"HO.OutSuccTarget.TransportResourceUnavailable",
	"HO.OutSuccTarget.TransportUnspecified",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG
	"HO.OutSuccTarget.3.0",
	"HO.OutSuccTarget.3.1",
	"HO.OutSuccTarget.3.2",
	"HO.OutSuccTarget.3.3",
#else   
    /*SPR 19596 End*/
	"HO.OutSuccTarget.NormalRelease",
	"HO.OutSuccTarget.AuthenticationFailure",
	"HO.OutSuccTarget.Detach",
	"HO.OutSuccTarget.Unspecified4",
    /*SPR 19596 Start*/
#endif    
#if ENUM_KPI_CAUSE_FLAG 
	"HO.OutSuccTarget.4.0",
	"HO.OutSuccTarget.4.1",
	"HO.OutSuccTarget.4.2",
	"HO.OutSuccTarget.4.3",
	"HO.OutSuccTarget.4.4",
	"HO.OutSuccTarget.4.5",
	"HO.OutSuccTarget.4.6",
#else
    /*SPR 19596 End*/

	"HO.OutSuccTarget.TransferSyntaxError",
	"HO.OutSuccTarget.AbstractSyntaxErrorReject",
	"HO.OutSuccTarget.AbstractSyntaxErrorIgnoreAndNotify",
	"HO.OutSuccTarget.MessageNotCompatibleWithReceiverState",
     /*SPR 19596 Start*/
    "HO.OutSuccTarget.SemanticError",
    "HO.OutSuccTarget.AbstractSyntaxErrorFalselyConstructedMessage",
    "HO.OutSuccTarget.Unspecified_1",	
#endif    
#if ENUM_KPI_CAUSE_FLAG
	"HO.OutSuccTarget.5.0",
	"HO.OutSuccTarget.5.1",
	"HO.OutSuccTarget.5.2",
	"HO.OutSuccTarget.5.3",
	"HO.OutSuccTarget.5.4",
	"HO.OutSuccTarget.5.5",
#else
    /*SPR 19596 End*/

	"HO.OutSuccTarget.ControlProcessingOverload",
	"HO.OutSuccTarget.NotEnoughUserPlaneProcessingResources",
     /*SPR 19596 Start*/
    "HO.OutSuccTarget.HardwareFailure",
	"HO.OutSuccTarget.OMIntervention",
    /*SPR 19596 End*/
	"HO.OutSuccTarget.S1APUnspecified",
	"HO.OutSuccTarget.S1APUnknownPLMN",
#endif     /*SPR 19596*/
	/* SPR 13398 FIX START */
	"HO.OutSuccTarget.Sum",
	/* SPR 13398 FIX END */
	OAM_NULL
};
/* SPR_12747_CHANGES */

/* SPR 19351 Code Removed*/

char *pm_xml_s1sig_param_name[]={
	"S1SIG.ConnEstabAtt",
	"S1SIG.ConnEstabSucc",
	OAM_NULL};
/*SPR 21866 Start*/
/* SPR 14504 FIX START */
/* if increasing/decreasing entries in the below table,
 *  *  * the macro value OAM_PM_MAX_IRAT_INC_MOB_LAI_CNT
 *   *   * should also be increased/decrease accordingly and vice versa */
/* SPR 21719 FIX START */
/* Code Remove  */  
/* SPR 21719 FIX END */
/* code removed */
/* SPR 14504 FIX END */
/*SPR 21866 End*/


char *pm_xml_paging_param_name[] = {
	"PAG.DiscardedNbr",
	/*SPR 10987 FIX start*/
	"PAG.SuccNbr",
	"PAG.ReceivedNbr",
	/*SPR 10987 FIX end*/
	OAM_NULL};

/*SPR 22367 Fix Start*/
    /*Code deleted*/
/*SPR 22367 Fix End*/

UInt16  g_pm_src_module_id = OAM_ZERO;

/*************************************************************************
 *  Function Name -      oam_pm_mem_free_of_xml_file_names 
 *  
 *  Description -        This function frees the memory allocated for g_pm_xml_file_names    
 *
 *  Inputs -             Count (UInt16 type)
 *
 *  Outputs -            NONE
 *
 *  Returns -            NONE
 ***********************************************************************************/
void oam_pm_mem_free_of_xml_file_names
(
 UInt16 count
 )
{
	oam_counter_t loop_counter = OAM_ZERO;
	OAM_FUN_ENTRY_TRACE();

	for(loop_counter = OAM_ZERO; loop_counter < count; loop_counter++)
	{
		oam_mem_free(g_pm_xml_file_names[loop_counter], OAM_NULL);
		g_pm_xml_file_names[loop_counter] = OAM_NULL;
	}

	OAM_FUN_EXIT_TRACE();
}

/*************************************************************************
 *  Function Name -      oam_pm_get_file_names 
 *  
 *  Description -        this function fetches the filenames in specified directory    
 *
 *  Inputs -             UInt16* type xml_file_count and error_code
 *
 *  Outputs -            NONE
 *
 *  Returns -            OAM_SUCCESS/OAM_FAILURE
 ***********************************************************************************/
oam_return_et
oam_pm_get_file_names
(
 UInt16 *xml_file_count,
 oam_error_code_et *p_error_code
 )
{
	OAM_FUN_ENTRY_TRACE();

    DIR  *dir_des;
	struct dirent *dir;
    int ret;
    UInt16 pm_length = OAM_ZERO;
	dir_des = opendir("./PM_XML_UPLOAD");
	if (dir_des)
	{
		/* SPR 16807 Fix Start */
		while (((dir = readdir(dir_des)) != OAM_NULL) && 
				(*xml_file_count < MAX_PM_XML_FILE_NAMES))
			/* SPR 16807 Fix End */
        /*SPR 22498 FIXED START*/
        {

            OAM_LOG(OAM, OAM_ERROR, "oam_pm_get_file_names g_pm_xml_file_names %s", dir->d_name);
            if(!oam_strcmp(dir->d_name, ".") )
            {
                continue;
            }
            else if(!oam_strcmp(dir->d_name, ".."))
            {
                continue;
            }

            else
            {
                pm_length = oam_strlen(dir->d_name) + OAM_ONE ;                  

                if(OAM_FAILURE == oam_mem_alloc(pm_length, (void *)&g_pm_xml_file_names[*xml_file_count], p_error_code))
                {
                    OAM_LOG(OAM, OAM_ERROR, "Memory Allocation failed for g_pm_xml_file_names");
                    /* coverity 40460 -1*/
                    ret=OAM_CLOSEDIR(dir_des);
                    if(ret== M_OAM_ONE)
                    {
                        OAM_LOG(OAM, OAM_ERROR, "Error while closing directory");
                    }
                    OAM_FUN_EXIT_TRACE();
                    return OAM_FAILURE;
                }
                OAM_LOG(OAM, OAM_INFO, " oam_pm_get_file_names g_pm_xml_file_names %s", dir->d_name);
                oam_strncpy(g_pm_xml_file_names[(*xml_file_count)++], dir->d_name,pm_length);
                g_pm_xml_file_names[pm_length -1] = '\0';


            }
        }
        /*SPR 22498 FIXED END*/
		/* coverity 40460 -2*/    
		ret=OAM_CLOSEDIR(dir_des);
		if(ret== M_OAM_ONE)
		{
			OAM_LOG(OAM, OAM_ERROR, "Error while closing directory");
		}
	}


	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/*************************************************************************
 *  Function Name -      oam_pm_xml_ftp_server_upload 
 *  
 *  Description -        This function handles the upload functionality of PM xml files    
 *
 *  Inputs -             NONE 
 *
 *  Outputs -            NONE
 *
 *  Returns -            OAM_SUCCESS/OAM_FAILURE
 ***********************************************************************************/
	oam_return_et 
oam_pm_xml_ftp_server_upload(UInt32 * curl_code)
{
	OAM_FUN_ENTRY_TRACE();

	oam_counter_t loop_counter = OAM_ZERO; 
	/* coverity 41284 */
	oam_error_code_et p_error_code = NO_ERROR; 
	UInt16 xml_file_count = OAM_ZERO;
	Char8  rm_cmd[OAM_CMD_SIZE]={OAM_ZERO};
	oam_nms_ftp_server_params_t p_oam_nms_ftp_server_params_t;    
	if(OAM_FAILURE == oam_pm_get_file_names(&xml_file_count, &p_error_code))
	{
		OAM_LOG(OAM, OAM_WARNING, "oam_pm_get_file_names failed");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_LOG(OAM, OAM_DETAILED, "Total files in PM_XML_UPLOAD folder is %d", xml_file_count);


	oam_memset((void*)&p_oam_nms_ftp_server_params_t, OAM_ZERO, sizeof(oam_nms_ftp_server_params_t));

	p_oam_nms_ftp_server_params_t.Enable = oam_prov_req.oam_nms_ftp_server_params.Enable;

	/* coverity 54194-1,2,3 */
    /* SPR 14472 START */
	oam_strncpy((char *)&p_oam_nms_ftp_server_params_t.ftp_server_url, 
                (char *)oam_prov_req.oam_igd_params.perf_mgmt_params.URL, 
                OAM_FTP_URL_NAME_SIZE - OAM_ONE);

	oam_strncpy((char *)&p_oam_nms_ftp_server_params_t.ftp_server_username, 
                (char *)oam_prov_req.oam_igd_params.perf_mgmt_params.Username, 
                OAM_FTP_USERNAME_SIZE - OAM_ONE);

	oam_strncpy((char *)&p_oam_nms_ftp_server_params_t.ftp_server_password, 
                (char *)oam_prov_req.oam_igd_params.perf_mgmt_params.Password, 
                OAM_FTP_PSWD_SIZE - OAM_ONE);
    /* SPR 14472 END */

	/* coverity 54194 */
	oam_strncpy((char *)&p_oam_nms_ftp_server_params_t.local_download_upload_path, "./PM_XML_UPLOAD", oam_strlen("./PM_XML_UPLOAD")+OAM_ONE);


	for(loop_counter = OAM_ZERO; loop_counter < xml_file_count; loop_counter++)
	{
		if(OAM_FAILURE == oam_file_upload_to_server(&p_oam_nms_ftp_server_params_t,g_pm_xml_file_names[loop_counter], curl_code))
		{
			OAM_LOG(OAM, OAM_ERROR, "oam_file_upload_to_server failed for %s", g_pm_xml_file_names[loop_counter]);
			oam_pm_mem_free_of_xml_file_names(xml_file_count);
			OAM_FUN_EXIT_TRACE();
			return OAM_FAILURE;
		} 

	}

	for(loop_counter = OAM_ZERO; loop_counter < xml_file_count; loop_counter++)
	{
		oam_strncpy(rm_cmd,"rm -rf ",sizeof("rm -rf "));
		oam_strncat(rm_cmd,"./PM_XML_UPLOAD/*",(oam_strlen("./PM_XML_UPLOAD/*") +OAM_ONE));
		if(M_OAM_ONE == oam_system(rm_cmd))
		{
			OAM_LOG(OAM,OAM_WARNING,"System Command for removing PM_XML_UPLOAD directory failed");
            /* SPR_19838 : Coverity_10443 Fix Start */
            oam_pm_mem_free_of_xml_file_names(xml_file_count);
            OAM_FUN_EXIT_TRACE();
			return OAM_FAILURE;
            /* SPR_19838 : Coverity_10443 Fix Stop */
		}

	}
    /* SPR_19838 : Coverity_10443 Fix Start */
    if (xml_file_count >0)
    {
        oam_pm_mem_free_of_xml_file_names(xml_file_count);
    }
    /* SPR_19838 : Coverity_10443 Fix Stop */

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/*************************************************************************
 *  Function Name -     oam_pm_write_data_to_file 
 *  
 *  Description -       This function handles the Data writing in PM xml files    
 *
 *  Inputs -             NONE 
 *
 *  Outputs -            NONE
 *
 *  Returns -            NONE 
 ***********************************************************************************/
/* + SPR 17439 */
void oam_pm_write_data_to_file(void)
{
/* - SPR 17439 */
	OAM_FUN_ENTRY_TRACE();
	oam_pm_write_data_to_rrm_file();
	/* write the current collated data in pm xml file */
	while(OAM_NULL !=  rrc_kpi_info.pm_kpi_info_first_Node)
	{
		OAM_LOG(OAM,OAM_DETAILED,"writing data to pm file ");
		oam_pm_read_data_from_link_list();
	}
	OAM_FUN_EXIT_TRACE();
}

/*************************************************************************
 *  Function Name -     oam_pm_write_data_to_rrm_file 
 *  
 *  Description -       This function handles the Data writing in RRM KPI files    
 *
 *  Inputs -             NONE 
 *
 *  Outputs -            NONE
 *
 *  Returns -            NONE 
 ***********************************************************************************/
void oam_pm_write_data_to_rrm_file(
/* + SPR 17439 */
 void
)
/* - SPR 17439 */
{
	/* SPR 12698 FIX START */
	char temp_buff[TMP_STR_BUFF_SIZE] = {OAM_ZERO};
	/* SPR 12698 FIX END */
	UInt16 rrm_counter = OAM_ZERO;
	UInt16 rrm_counter_temp = OAM_ZERO;

	/* write the current collated data in pm xml file */
	while(OAM_NULL !=  rrm_kpi_info.pm_kpi_info_first_Node)
	{
		oam_pm_rrm_read_data_from_link_list();

		if(OAM_ZERO == file_close.rrm_flag)
		{
			for(rrm_counter = OAM_ZERO; rrm_counter < elements_in_pm_rrm_xml_param_name ;rrm_counter++)
			{
				rrm_counter_temp = rrm_counter + OAM_ONE;
				/* SPR 12698 FIX START */
				oam_snprintf(temp_buff,TMP_STR_BUFF_SIZE, "%d",(rrm_counter_temp));  
				/* SPR 12698 FIX END */
				xmlTextWriterStartElement(rrm_writer, (xmlChar *)"r");
				xmlTextWriterWriteAttribute(rrm_writer, (xmlChar *)"p",(xmlChar *)temp_buff);
				xmlTextWriterWriteString(rrm_writer,(xmlChar *)pm_rrm_xml_param_value[rrm_counter]);
				xmlTextWriterEndElement(rrm_writer);

			}

		}
		xmlTextWriterFlush(rrm_writer);
	}
}


/***************************************************************************
 * Function Name  : oam_layer_send_req
 * Description    : This function handles the request from QCLI to the
 *                  respective API in the Layer
 * Inputs         : p_api_buf : API Buffer
 * Outputs        : none
 * Returns        : msg_length
 ******************************************************************************/
	oam_return_et
oam_layer_send_req(void *p_api_buf)
{
	OAM_FUN_ENTRY_TRACE();
	UInt16 api_id = OAM_ZERO;
	oam_error_code_et p_error_code = NO_ERROR; /* Coverity_14DEC */
	api_id   = get_word_from_header(p_api_buf+ OAM_API_ID);
	switch(api_id)
	{
		case OAM_GET_KPI_MAC_REQ:
			{
				oam_mac_get_KPI_stats_req_t mac_get_kpi_stats_req;
				oam_show_kpi_mac_t *p_oam_show_kpi_mac =
					(oam_show_kpi_mac_t *)(p_api_buf + QCLI_HEADER_SIZE);

				mac_get_kpi_stats_req.resetStats = p_oam_show_kpi_mac->reset;
				mac_get_kpi_stats_req.kpiToReportBitMap = p_oam_show_kpi_mac->kpi_type;
					OAM_LOG(OAM,OAM_DETAILEDALL,"reset %d report bitmap 0x%x  ",
							 p_oam_show_kpi_mac->reset, p_oam_show_kpi_mac->kpi_type);
				/* Calling MAC Get KPI Stats Function Call */
				if (OAM_FAILURE == oam_mac_get_kpi_stats_req(
							&mac_get_kpi_stats_req, OAM_MODULE_ID, OAM_ONE, &p_error_code))
				{
					OAM_LOG(OAM,OAM_ERROR,"Error in sending MAC_GET_KPI_STATS_REQ with error code %d",
							p_error_code);
					OAM_FUN_EXIT_TRACE();
					return OAM_FAILURE;
				}
				break;
			}
		case OAM_GET_KPI_PDCP_REQ:
			{
				oam_show_kpi_pdcp_t *p_oam_show_kpi_pdcp =
					(oam_show_kpi_pdcp_t *)(p_api_buf + QCLI_HEADER_SIZE);
				oam_pdcp_get_KPI_stats_req_t pdcp_get_kpi_stats_req;

				pdcp_get_kpi_stats_req.kpiToGetBitMap = OAM_PDCP_KPIBITMAP; 
				pdcp_get_kpi_stats_req.resetStats = p_oam_show_kpi_pdcp->reset;
				/* Calling PDCP Get KPI Stats Function Call */
				if (OAM_FAILURE == oam_pdcp_get_kpi_stats_req(
							&pdcp_get_kpi_stats_req, OAM_MODULE_ID, OAM_ONE, &p_error_code))
				{
					OAM_LOG(OAM,OAM_ERROR,"Error in sending PDCP_GET_KPI_STATS_REQ with error code %d",
							p_error_code);
					OAM_FUN_EXIT_TRACE();
					return OAM_FAILURE;
				}
				break;
			}
		case OAM_RRC_GET_CELL_STATS_REQ:
			{
				oam_pm_show_rrc_cell_stats_t *p_oam_show_rrc_cell_stats =
					(oam_pm_show_rrc_cell_stats_t *)(p_api_buf + QCLI_HEADER_SIZE);
				oam_rrc_get_cell_stats_req_t oam_rrc_get_cell_stats_req;

				oam_memset(&oam_rrc_get_cell_stats_req,OAM_ZERO,sizeof(oam_rrc_get_cell_stats_req_t));

				oam_rrc_get_cell_stats_req.cell_index = p_oam_show_rrc_cell_stats->cell_index;
				oam_rrc_get_cell_stats_req.reset_stats = p_oam_show_rrc_cell_stats->reset_stats;
				oam_rrc_get_cell_stats_req.bitmask = p_oam_show_rrc_cell_stats->bitmask;
				break;
			}
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_qcli_send_resp
 * Description    : This function handles the response from respective layer
 *                  to the QCLI
 * Inputs         : p_api_buf : API Buffer
 * Outputs        : none
 * Returns        : msg_length
 ******************************************************************************/
	oam_return_et
oam_qcli_send_resp(void *p_api_buf)
{
	UInt16 api_id=OAM_ZERO;
	/* SPR 14019 FIX Start */
#ifdef PERF_STATS
	/* SPR 14019 FIX End */
	oam_counter_t loop_counter = OAM_ZERO;
	UInt16 str_count = OAM_ZERO;    
	UInt16 dest_id=OAM_ZERO;
	/* SPR 14019 FIX Start */
#endif
	/* SPR 14019 FIX End */
	/* CSR No 00055803 Start */
	UInt32 bit_mask = OAM_ZERO;
	/* CSR No 00055803 End */
	oam_error_code_et p_error_code = NO_ERROR; /* Coverity_14DEC */
	UInt16 trans_id=OAM_ZERO;
	/*SPR_10579 Start*/ 
	SInt32 length_read= OAM_ZERO;
	UInt16 msg_size = OAM_ZERO;
	/*SPR_10579 End*/ 
	UInt8 cell_index = OAM_ZERO;
    /* SPR 17883 fix start */
    UInt16 transaction_id=OAM_ZERO;
    /* SPR 17883 fix end */

	OAM_FUN_ENTRY_TRACE();

	cell_index = (UInt8)(*((UInt8*)(p_api_buf) + OAM_INTF_HDR_CELL_IDX_OFFSET));
	api_id   = get_word_from_header(p_api_buf+OAM_API_ID);
	trans_id = get_word_from_header((UInt8*)(p_api_buf) +OAM_ZERO);
	/* SPR 14019 FIX Start */
#ifdef PERF_STATS
	/* SPR 14019 FIX End */
	dest_id = get_word_from_header((UInt8*)(p_api_buf) +OAM_TWO);
	/* SPR 14019 FIX Start */
#endif
	/* SPR 14019 FIX End */

	switch(api_id)
	{
		case PDCP_CONFIGURE_KPI_STATS_RESP:
			{
				if (!(cell_index == oam_prov_req.cell_cntxt.curr_cell_cid) &&
						(oam_get_rrm_cell_config_idx(cell_index)
						 == OAM_ERROR_RETURN_VAL)) {
					OAM_LOG(OAM,OAM_WARNING,"Received invalid cell_index = %d in PDCP_CONFIGURE_KPI_STATS_RESP",cell_index);
					OAM_FUN_EXIT_TRACE();
					return OAM_FAILURE;
				}

				oam_pdcp_config_KPI_stats_conf_t *p_oam_pdcp_config_kpi_stats_conf = PNULL;

				/* SPR_12955_FIXES */
				p_oam_pdcp_config_kpi_stats_conf = (oam_pdcp_config_KPI_stats_conf_t *)(p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE);
				/* SPR_12955_FIXES */

				/* SPR 17883 fix start */
                /* SPR_12955_FIXES */
				/* CSR No 00055803 Start */
                transaction_id = get_word_from_header((UInt8*)(p_api_buf) + OAM_TRANSACTION_ID);
                oam_tcb_update_bitmask(transaction_id,OAM_TRANS_PDCP,&bit_mask);
				OAM_LOG(OAM, OAM_INFO, "PDCP_CONFIGURE_KPI_STATS_RESP:"
                        " cell_index = %d Trans_id = %d bitmask = %d", 
                        cell_index, trans_id, bit_mask);	
				/* SPR_12955_FIXES */
                /* CSR No 00055803 Start */
                /* SPR 17883 fix end */
				
				/* CSR No 00055803 End */

				if(OAM_ONE == p_oam_pdcp_config_kpi_stats_conf->responseCode)
				{
					/*SPR_10579 Start*/
					oam_handle_multiple_config_resp.success_fail_bitmask = 
						oam_handle_multiple_config_resp.success_fail_bitmask & OAM_MAC_RRC_CONFIG_RESP_VALUE; 
					/*SPR_10579 End*/
				}
				/* CSR No 00055803 Start */
				if(OAM_FAILURE  ==  oam_handle_kpi_config_success_fail_resp_handle(p_api_buf,bit_mask))
				{

					p_error_code = (oam_error_code_et)OAM_INTERNAL_ERROR;
					OAM_LOG(OAM,OAM_ERROR, "PDCP_CONFIGURE_KPI_STATS_RESP handling failed, errorCode = %d",p_error_code);

					OAM_FUN_EXIT_TRACE();
					return OAM_FAILURE;
				}
				break; 
				/* CSR No 00055803 End */

			}
		case MAC_CONFIGURE_KPI_STATS_CNF:
			{
				if (!(cell_index == oam_prov_req.cell_cntxt.curr_cell_cid) &&
						(oam_get_rrm_cell_config_idx(cell_index)
						 == OAM_ERROR_RETURN_VAL)) {
					OAM_LOG(OAM,OAM_ERROR,"Received invalid cell_index %d in MAC_CONFIGURE_KPI_STATS_CNF",cell_index);
					OAM_FUN_EXIT_TRACE();
					return OAM_FAILURE;
				}

                /* + SPR 18022 */
				oam_mac_config_KPI_stats_conf_t *p_oam_mac_config_kpi_stats_conf =
                    (oam_mac_config_KPI_stats_conf_t *)(p_api_buf + 
                            LOWER_LAYER_INTERFACE_HEADER_SIZE);
                /* - SPR 18022 */

               /* SPR 17883 fix start */
               transaction_id = get_word_from_header((UInt8*)(p_api_buf) + OAM_TRANSACTION_ID);
               /* SPR 17883 fix end */

				if(OAM_ONE == p_oam_mac_config_kpi_stats_conf->responseCode)
				{
					/*SPR_10579 Start*/
					oam_handle_multiple_config_resp.success_fail_bitmask = oam_handle_multiple_config_resp.success_fail_bitmask & OAM_PDCP_RRC_CONFIG_RESP_VALUE; //reseting mac bit
					/*SPR_10579 End*/
				}
				/* CSR No 00055803 Start */
                /* SPR 17883 fix start */
				/* SPR_12955_FIXES */
                oam_tcb_update_bitmask(transaction_id,OAM_TRANS_MAC,&bit_mask);
				/* SPR_12955_FIXES */
                /* SPR 17883 fix end */

				OAM_LOG(OAM,OAM_INFO, "MAC_CONFIGURE_KPI_STATS_RESP bitmask = %d Trans_id[%d]",
                        bit_mask, trans_id);

				if(OAM_FAILURE  ==  oam_handle_kpi_config_success_fail_resp_handle(p_api_buf,bit_mask))
				{
					p_error_code = (oam_error_code_et)OAM_INTERNAL_ERROR;
					OAM_LOG(OAM,OAM_WARNING, "Error in handling MAC_CONFIGURE_KPI_STATS_RESP, errorCode = %d",p_error_code);
					/*PM_BUG*/
					OAM_FUN_EXIT_TRACE();
					return OAM_FAILURE;
				}
				break;
				/* CSR No 00055803 End */

			}

			/* SPR 14019 FIX Start */
#ifdef PERF_STATS
			/* SPR 14019 FIX End */
		case PDCP_GET_UE_PERF_STATS_CNF:
			{
				LOCAL SInt8 total_UE = OAM_ZERO;
				/* coverity 64381 */
				UInt8 **temp_arr = OAM_NULL;
				UInt16 numOfUe = OAM_ZERO;
				oam_counter_t loop_count1 = OAM_ZERO;
				UInt16 DlSRB = OAM_ZERO;
				UInt16 UlSRB = OAM_ZERO;
				/* SPR 13857 START */
				/* size_x is being defined as two as GPV response contains two parameters i.e.
				   "Total downlink SRB Volume transmitted for UE" and
				   "Total uplink SRB Volume transmitted for UE" */
				UInt32 size_x = OAM_TWO; /* this variable will be used for the first  dimension */
				/* SPR 13857 END*/               
				/*SPR 10023 FIX*/
				if(oam_handle_get_ue_pdcp_perf_stats_batch_data(p_api_buf, &p_error_code, &total_UE)
						== OAM_SUCCESS)
				{
                    OAM_LOG(OAM, OAM_INFO, "PDCP_GET_UE_PERF_STATS_CNF Received");
					oam_pdcp_ue_perf_stats_batch_node *temp = OAM_NULL;
					/*SPR 10023 FIX*/
					oam_pdcp_get_ue_perf_stats_cnf_t *p_oam_pdcp_get_ue_perf_stats_cnf = PNULL;
#ifndef OAM_UT_FRAMEWORK
					if(OAM_FAILURE == oam_process_tcb_for_layer_resp(
								p_api_buf,&trans_id,&dest_id))
					{
						oam_construct_n_send_get_resp_to_usr(OAM_RESULT_FAILURE,
								OAM_ERR_REQUEST_DENIED,
								OAM_VERSION_ID, OAM_MODULE_ID,
								dest_id, trans_id, OAM_ZERO, OAM_ZERO,
								"No TCB Entry Found For The Request",
								&p_error_code);

						OAM_LOG(OAM,OAM_ERROR,"No TCB Entry Found for"
									" PDCP_GET_UE_PERF_STATS_CNF message, Trans_id=%d",trans_id );
						OAM_FUN_EXIT_TRACE();
						return OAM_FAILURE;
					}
#endif
					/*SPR 10023 FIX*/
					temp = ConsolidatedPdcpUePerfStats;
					/*SPR 10023 FIX*/

					/*
					 *                  * DESCRIPTION: While loop below reads all the nodes from 
					 *                                   * "oam_pdcp_ue_perf_stats_batch_node" link list.
					 *                                                    */

					/*SPR 10023 FIX*/
					while(temp != OAM_NULL)
					{
						p_oam_pdcp_get_ue_perf_stats_cnf = &temp->data;
						/*SPR 10023 FIX*/
						if(p_oam_pdcp_get_ue_perf_stats_cnf->responseCode)
						{
							numOfUe = p_oam_pdcp_get_ue_perf_stats_cnf->numOfUePresent;
							if(numOfUe)
							{
								/*SPR 10023 FIX: removed code*/
								for(loop_count1 = OAM_ZERO; loop_count1 < numOfUe; loop_count1++)
								{
									DlSRB += p_oam_pdcp_get_ue_perf_stats_cnf->pdcpUePerfStatsResp
										[loop_count1].pdcpRbPerfStats.totalDlSrbBytesSent;
									UlSRB += p_oam_pdcp_get_ue_perf_stats_cnf->pdcpUePerfStatsResp
										[loop_count1].pdcpRbPerfStats.totalUlSrbBytesRcvd;
								}
							}
							/*SPR 10023 FIX*/
							else
							{
								OAM_LOG(OAM,OAM_INFO,"received empty batch"
										"in PDCP_GET_UE_PERF_STATS_CNF ");
							}
							/*SPR 10023 FIX*/
						}
						else
						{
							/*SPR 10023 FIX*/
							OAM_LOG(OAM,OAM_ERROR,"Received failure response code [%u] in"
									" PDCP_GET_UE_PERF_STATS_CNF message", 
									p_oam_pdcp_get_ue_perf_stats_cnf->responseCode);
							/*SPR 10023 FIX*/
							oam_construct_n_send_get_resp_to_usr(
									OAM_RESULT_FAILURE,
									OAM_ERR_INTERNAL_ERROR,
									OAM_VERSION,
									OAM_MODULE_ID,QCLI_MODULE_ID,
									trans_id,
									OAM_ZERO,OAM_ZERO,"PDCP LAYER FAILURE RESPONSE",
									&p_error_code
									);
							delete_pdcp_perf_stats_data_from_link_list();
							return OAM_FAILURE;
						}
						/*SPR 10023 FIX*/
						temp = temp->next;
						/*SPR 10023 FIX*/
					}

					if (total_UE == OAM_ZERO)
					{
						if (OAM_FAILURE == oam_mem_alloc((sizeof(UInt8 *)),
									(void *)&temp_arr,
									&p_error_code))
						{
							OAM_LOG(OAM, OAM_ERROR,
									"Memory allocation to temp_arr failed"
									" with Error Code:%d",
									p_error_code);
							OAM_FUN_EXIT_TRACE();
							return OAM_FAILURE;
						}
						for (loop_counter = OAM_ZERO; loop_counter < 2; loop_counter++)
						{
							if(OAM_FAILURE == oam_mem_alloc((200 * sizeof(UInt8)),(void *)&temp_arr[loop_counter],&p_error_code))
							{
								OAM_LOG(OAM, OAM_ERROR,
										"Memory allocation to temp_arr failed"
										" with Error Code:%d",
										p_error_code);
                                /* SPR 13857 START */
								OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, loop_counter, p_error_code);
								/* SPR 13857 END*/
								OAM_FUN_EXIT_TRACE();
								return OAM_FAILURE;
							}
						}
					}
					else
					{
						if (OAM_FAILURE == oam_mem_alloc((2 * total_UE* sizeof(UInt8 *)),(void *)&temp_arr,&p_error_code))
						{
							OAM_LOG(OAM, OAM_ERROR,
									"Memory allocation to temp_arr failed"
									" with Error Code:%d",
									p_error_code);
							OAM_FUN_EXIT_TRACE();
							return OAM_FAILURE;

						}
						for (loop_counter = OAM_ZERO; loop_counter < 2; loop_counter++)
						{
							if(OAM_FAILURE == oam_mem_alloc((200 * sizeof(UInt8)),(void *)&temp_arr[loop_counter],&p_error_code))
							{
								OAM_LOG(OAM, OAM_ERROR,
										"Memory allocation to temp_arr failed"
										" with Error Code:%d",
										p_error_code);
                                /* SPR 13857 START */
								OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, loop_counter, p_error_code);
								/* SPR 13857 END*/
								OAM_FUN_EXIT_TRACE();
								return OAM_FAILURE;
							}
						}
					}
					oam_sprintf((char *)(char *)temp_arr[str_count++],"%s = %u","Total"
							" downlink SRB Volume transmitted for UE", DlSRB);
					oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Total"
							" uplink SRB Volume transmitted for UE",UlSRB);

					oam_construct_n_send_gpv_response(
							(UInt8 *)"Show.PerfStats.PDCP",
							temp_arr,
							str_count,
							trans_id,
							&p_error_code,
							QCLI_MODULE_ID,
							0,
							0);

					/*SPR 10023 FIX*/
					total_UE = OAM_ZERO;
					/* 
					 *                  * DESCRIPTION: This function will delete all the nodes of 
					 *                                   * "oam_pdcp_ue_perf_stats_batch_node" link list
					 *                                                    */
					delete_pdcp_perf_stats_data_from_link_list();
					/* COVERITY: 71355 FIX START */
					/* SPR 13857 START */
					OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, size_x, p_error_code);
					//oam_mem_free(temp_arr, &p_error_code);
					/* SPR 13857 END*/
					temp_arr = OAM_NULL;     
					/*SPR 10023 FIX*/
					/* COVERITY: 71355 FIX END */
				}
				else
				{
					OAM_LOG(OAM,OAM_INFO,"received with PDCP_GET_UE_PERF_STATS_CNF"
							" with endIndicator 0");
				}
				break;
			}
			/* SPR 14019 FIX Start */
#endif
			/* SPR 14019 FIX End */
			/*SPR_10579 Start*/
		case RRM_OAM_CONFIG_KPI_RESP:
			{ 
				if (!(cell_index == oam_prov_req.cell_cntxt.curr_cell_cid) &&
						(oam_get_rrm_cell_config_idx(cell_index)
						 == OAM_ERROR_RETURN_VAL)) {
					OAM_LOG(OAM,OAM_ERROR,"Received invalid cell_index %d in RRM_OAM_CONFIG_KPI_RESP",cell_index);
					OAM_FUN_EXIT_TRACE();
					return OAM_FAILURE;
				}

				rrm_oam_config_kpi_resp_t rrm_oam_config_kpi_resp ;
				oam_memset(&rrm_oam_config_kpi_resp, OAM_ZERO, sizeof(rrm_oam_config_kpi_resp_t));
				msg_size = get_word_from_header((UInt8*)(p_api_buf) + OAM_INTF_HDR_MSG_SIZE_OFFSET);
				if(OAM_SUCCESS == oam_parse_rrm_oam_config_kpi_resp(&rrm_oam_config_kpi_resp,
							p_api_buf+LOWER_LAYER_INTERFACE_HEADER_SIZE,
							msg_size-LOWER_LAYER_INTERFACE_HEADER_SIZE,
							&length_read))
				{
					if(OAM_SUCCESS == rrm_oam_config_kpi_resp.response)
					{
						if(cell_index == oam_prov_req.cell_cntxt.curr_cell_cid)
						{
							oam_handle_multiple_config_resp.success_fail_bitmask = 
								oam_handle_multiple_config_resp.success_fail_bitmask & 
								OAM_MAC_PDCP_RRC_CONFIG_RESP_VALUE; //reseting mac bit
						}
					}
					else
					{
						OAM_LOG(OAM,OAM_INFO, "Response of RRM_OAM_CONFIG_KPI_RESP is Failure");

					}
				}
				else
				{
					OAM_LOG(OAM, OAM_INFO, "RRM_OAM_CONFIG_KPI_RESP message parsing failed");
				}


				if(cell_index == oam_prov_req.cell_cntxt.curr_cell_cid)
                {
                    /* SPR_12955_FIXES */	
                    /* SPR 17883 fix start */
                    transaction_id = get_word_from_header((UInt8*)(p_api_buf) + OAM_TRANSACTION_ID);
                    oam_tcb_update_bitmask(transaction_id,OAM_TRANS_RRM,&bit_mask);
                    /* SPR 17883 fix end */
                    /* SPR_12955_FIXES */	
		    OAM_LOG(OAM, OAM_INFO, "RRM_OAM_CONFIG_KPI_RESP:cell_index = %d"
				    ", Trans_id = %d, bitmask = %d", 
				    cell_index, trans_id, bit_mask);

                    if(OAM_FAILURE  ==  oam_handle_kpi_config_success_fail_resp_handle(p_api_buf,bit_mask))
                    {
                        OAM_LOG(OAM,OAM_ERROR, "Failed to Handled RRM_OAM_CONFIG_KPI_RESP in Multiple handling");
                        OAM_FUN_EXIT_TRACE();
                        return OAM_FAILURE;
                    }
                }
				OAM_LOG(OAM,OAM_DETAILED, "Successfully Handled RRM_OAM_CONFIG_KPI_RESP");

				break;
			} 
			/*SPR_10579 End*/
		default:
			{
				OAM_LOG(OAM,OAM_WARNING, "Invalid api_id=%d received!!!", api_id);
				OAM_FUN_EXIT_TRACE();
				return OAM_FAILURE;
			}
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/* CSR No 00055803 Start */
/****************************************************************************
 * Function Name  : oam_handle_kpi_config_success_fail_resp_handle 
 * Description    : This function send kpi config response to user interface
 * Inputs         : p_api_buf : API Buffer
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_handle_kpi_config_success_fail_resp_handle
(
 void  *p_api_buf,
 UInt32 bit_mask   
 )
{
	oam_error_code_et p_error_code = NO_ERROR; /* Coverity_14DEC */
	UInt16 trans_id=OAM_ZERO;
	UInt16 transaction_id=OAM_ZERO;  /* CSR 00058135 */
	UInt16 dest_id=OAM_ZERO;
	OAM_FUN_ENTRY_TRACE();
    /* SPR 17883 fix start */
    transaction_id  = get_word_from_header((UInt8*)(p_api_buf) + OAM_TRANSACTION_ID);
    /* SPR 17883 fix end */

	if((OAM_ZERO == bit_mask) && (OAM_ZERO == oam_handle_multiple_config_resp.success_fail_bitmask) )
	{ 
		/* CSR 00058135 Changes Start */
        /* +- SPR 17883 */
		/* SPR_12955_FIXES */
		OAM_LOG(OAM, OAM_INFO, "Trans_id = %d", transaction_id);	
		/* SPR_12955_FIXES */
#ifndef OAM_UT_FRAMEWORK
		if(OAM_FAILURE == oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(
					transaction_id,&trans_id,&dest_id))
			/* CSR 00058135 Changes End */
		{
			if(OAM_ONE == g_resp_sent_user_interface)
			{
				/* SPR 8942 PM_FIX  */        g_config_lower_layer_resp =  OAM_RESULT_FAILURE;
				g_resp_sent_user_interface = OAM_ZERO;				
				oam_construct_n_send_set_resp_to_usr(OAM_RESULT_FAILURE,
						OAM_ZERO,(UInt8 *)"CONFIGURATION KPI FAILED",oam_strlen("CONFIGURATION KPI FAILED") + OAM_ONE,
						OAM_VERSION_ID, OAM_MODULE_ID,
                        /* SPR 15491 START */
                        oam_set_config_req.src_id, trans_id, OAM_ZERO, OAM_ZERO,
                        /* SPR 15491 END */
						&p_error_code);

			}
			OAM_FUN_EXIT_TRACE();
			return OAM_FAILURE;
		}
#endif
		if(OAM_ONE == g_resp_sent_user_interface)
		{
			g_resp_sent_user_interface = OAM_ZERO;				
			/* SPR 8942 PM_FIX  */            g_config_lower_layer_resp =  OAM_RESULT_SUCCESS;
			oam_construct_n_send_set_resp_to_usr(OAM_RESULT_SUCCESS,
					OAM_ZERO,OAM_NULL,OAM_ZERO,
					OAM_VERSION_ID, OAM_MODULE_ID,
                    /* SPR 15491 START */
                    oam_set_config_req.src_id, trans_id, OAM_ZERO, OAM_ZERO,
                    /* SPR 15491 END */
					&p_error_code); 
		}
	}
	else if((OAM_ZERO == bit_mask) && (OAM_ZERO != oam_handle_multiple_config_resp.success_fail_bitmask) )
	{
		/* CSR 00058135 Changes Start */
        /* +- SPR 17883 */
		/* SPR_12955_FIXES */
		OAM_LOG(OAM, OAM_INFO, "Trans_id = %d", trans_id);	
		/* SPR_12955_FIXES */
#ifndef OAM_UT_FRAMEWORK
		if(OAM_FAILURE == oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(
					transaction_id,&trans_id,&dest_id))
			/* CSR 00058135 Changes End */
		{

			if(OAM_ONE == g_resp_sent_user_interface)
			{
				g_resp_sent_user_interface = OAM_ZERO;				
				/* SPR 8942 PM_FIX  */g_config_lower_layer_resp =  OAM_RESULT_FAILURE;
				oam_construct_n_send_set_resp_to_usr(OAM_RESULT_FAILURE,
						OAM_ZERO,(UInt8 *)"CONFIGURATION KPI FAILED",oam_strlen("CONFIGURATION KPI FAILED") + OAM_ONE,
						OAM_VERSION_ID, OAM_MODULE_ID,
                        /* SPR 15491 START */
                        oam_set_config_req.src_id, trans_id, OAM_ZERO, OAM_ZERO,
                        /* SPR 15491 END */
						&p_error_code);
			}

			OAM_FUN_EXIT_TRACE();
			return OAM_FAILURE;
		}
#endif
		if(OAM_ONE == g_resp_sent_user_interface)
		{
			/* SPR 8942 PM_FIX  */ g_config_lower_layer_resp =  OAM_RESULT_FAILURE;
			g_resp_sent_user_interface = OAM_ZERO;				
			oam_construct_n_send_set_resp_to_usr(OAM_RESULT_FAILURE,
					OAM_ZERO,(UInt8 *)"CONFIGURATION KPI FAILED",
					oam_strlen("CONFIGURATION KPI FAILED") + OAM_ONE,
					OAM_VERSION_ID, OAM_MODULE_ID,
                    /* SPR 15491 START */
                    oam_set_config_req.src_id, trans_id, OAM_ZERO, OAM_ZERO,
                    /* SPR 15491 END */
					&p_error_code);

		}
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/* CSR No 00055803 End */

/****************************************************************************

 * Function Name  : oam_rrc_pm_handle_get_cell_stats_resp 
 * Description    : This function fills oam_rrc_get_cell_stats_resp_t structure 

 * Inputs         : p_api : void pointer 
 *                : data_len : data length(UInt16 type)
 * Outputs        : cell_stats_resp : pointer to structure oam_rrc_get_cell_stats_resp_t
 *                : p_error_code : pointer to error_code
 * Returns        : OAM_FAILURE/OAM_SUCCESS 
 ****************************************************************************/
/* SPR 17777 fix */
oam_return_et
oam_rrc_fill_non_periodic_get_cell_stats_resp
(

 oam_rrc_get_cell_stats_resp_t *oam_rrc_get_cell_stats_resp,
 UInt32 *str_count_2
 )
{
	char temp_str[300] = {OAM_ZERO};  

	UInt32 str_count = OAM_ZERO;
	UInt8 index  = OAM_ZERO;
	oam_counter_t loop_counter = OAM_ZERO;
	oam_counter_t loop_counter1 = OAM_ZERO;
    
    /* SPR 14504 FIX START */
    /*code removed*/
    /* SPR 14504 FIX END */

	OAM_FUN_ENTRY_TRACE();

	pm_l3_file_ptr = oam_fopen("l3_kpi.log","w");
	if(OAM_NULL == pm_l3_file_ptr)
	{

		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	index = oam_rrc_get_cell_stats_resp->cell_index;
	if (OAM_SUCCESS == oam_rrc_get_cell_stats_resp->response)
	{      
		OAM_FILE_PRINT(pm_l3_file_ptr,"Cell Index",OAM_ZERO, index);  

		if((oam_rrc_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_ALL_STATS_PRESENT))
		{
			oam_rrc_get_cell_stats_resp->bitmask |=
				RRC_OAM_STATISTICS_CONNECTION_RELATED_STATS_PRESENT;
			oam_rrc_get_cell_stats_resp->bitmask |=
				RRC_OAM_STATISTICS_ERAB_RELATED_STATS_PRESENT;
			oam_rrc_get_cell_stats_resp->bitmask |=
				RRC_OAM_STATISTICS_UE_ASSOC_S1_CONN_RELATED_STATS_PRESENT;
			oam_rrc_get_cell_stats_resp->bitmask |=
				RRC_OAM_STATISTICS_PAGING_RELATED_STATS_PRESENT;
		}

		if(oam_rrc_get_cell_stats_resp->bitmask &
				RRC_OAM_STATISTICS_CONNECTION_RELATED_STATS_PRESENT)
		{
			if (oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.bitmask 
					& RRC_OAM_STATISTICS_CONNECTION_ESTB_STATS_PRESENT )
			{
				for(index = OAM_ZERO;
						index < RRC_ESTABLISHMENT_CAUSE_INDEX;
						index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"rrc_connEstabAtt_cause",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connection_establishment.rrc_connEstabAtt_cause[index]);  
				}


				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"rrc_connEstabAtt_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connection_establishment.rrc_connEstabAtt_sum);  

				for(index = OAM_ZERO; index < RRC_ESTABLISHMENT_CAUSE_INDEX; index++)
				{

					OAM_FILE_PRINT(pm_l3_file_ptr,"rrc_connEstabSucc_cause",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connection_establishment.rrc_connEstabSucc_cause[index]);

				}

				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"rrc_connEstabSucc_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connection_establishment.rrc_connEstabSucc_sum);

				for(index = OAM_ZERO; index < RRC_ESTABLISHMENT_CAUSE_INDEX; index++)
				{


					OAM_FILE_PRINT(pm_l3_file_ptr,"rrc_connEstabFail_cause",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connection_establishment.rrc_connEstabFail_cause[index]);

				}


				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"rrc_connEstabFail_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connection_establishment.rrc_connEstabFail_sum);


				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"rrc_connEstabFaileNBCause_Unspecified",(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connection_establishment.rrc_connEstabFaileNBCause_Unspecified);
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"rrc_ConnEstabFaileNBCause_Congestion",(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connection_establishment.rrc_ConnEstabFaileNBCause_Congestion);

				/*SPR 10987 FIX start*/
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"rrc_ConnEstabFaileNBCause_EnergySaving",(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connection_establishment.rrc_ConnEstabFaileNBCause_EnergySaving);

				/*SPR 10987 FIX end*/

			}
			if (oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.bitmask 
					& RRC_OAM_STATISTICS_CONNECTION_RE_ESTB_STATS_PRESENT)
			{
				for(index = OAM_ZERO; index < RRC_REESTABLISHMENT_CAUSE_INDEX;
						index++)
				{

					OAM_FILE_PRINT(pm_l3_file_ptr,"rrc_connReEstabAtt_cause",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connection_reestablishment.rrc_connReEstabAtt_cause[index]);

				}


				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"rrc_connReEstabAtt_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connection_reestablishment.rrc_connReEstabAtt_sum);         
				for(index = OAM_ZERO; index < RRC_REESTABLISHMENT_CAUSE_INDEX; index++)
				{

					OAM_FILE_PRINT(pm_l3_file_ptr,"rrc_connReEstabSucc_cause",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connection_reestablishment.rrc_connReEstabSucc_cause[index]);

				}

				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"rrc_connReEstabSucc_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connection_reestablishment.rrc_connReEstabSucc_sum);
				for(index = OAM_ZERO; index < RRC_REESTABLISHMENT_CAUSE_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"rrc_connReEstabFail_cause",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connection_reestablishment.rrc_connReEstabFail_cause[index]);    
				}


				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"rrc_connReEstabFail_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connection_reestablishment.rrc_connReEstabFail_sum);
			}
			/*SPR 10987 start*/
			OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"rrcConnectionNumber",(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrcConnectionNumber);
			/*SPR 10987 end*/
/* SPR 18861 */
			OAM_LOG(OAM,OAM_DETAILED," rrcConnectionNumber %d ", (UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrcConnectionNumber);
/* SPR 18861 */
			if (oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.bitmask 
					& RRC_OAM_STATISTICS_CONNECTION_SETUP_TIME_STATS_PRESENT)
			{
				for(index = OAM_ZERO; index < RRC_ESTABLISHMENT_CAUSE_INDEX; index++)
				{
					if (oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connEstabTime_cause[index].count > OAM_ZERO)
					{
						OAM_FILE_PRINT(pm_l3_file_ptr,"rrc_connEstabTime_Mean.rrc_connEstabTime_cause", index,
								(UInt32)(oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connEstabTime_cause[index].rrc_connEstabTime_Sum/
									oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connEstabTime_cause[index].count));

/* SPR 18861 */
					OAM_LOG(OAM,OAM_DETAILEDALL,"CLI For cause %d got estabTimeSum = %d count %d and so mean %d ",
                            index,
                 (UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connEstabTime_cause[index].rrc_connEstabTime_Sum, 
                oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connEstabTime_cause[index].count,
                 (UInt32)(oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connEstabTime_cause[index].rrc_connEstabTime_Sum/ 
                oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connEstabTime_cause[index].count)
						);
/* SPR 18861 */
					}
					else
					{
						OAM_FILE_PRINT(pm_l3_file_ptr,"rrc_connEstabTime_Mean.rrc_connEstabTime_cause", index,
								(UInt32)(oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connEstabTime_cause[index].rrc_connEstabTime_Sum));
					}
					OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"rrc_connEstabTime_Max",(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connEstabTime_cause[index].rrc_connEstabTime_Max);
/* SPR 18861 */
					OAM_LOG(OAM,OAM_DETAILEDALL,"CLI For cause %d got estabTimeMax = %d  ",
                            index,
                           (UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.rrc_connEstabTime_cause[index].rrc_connEstabTime_Max
						);
/* SPR 18861 */
				}
			}
			if (oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.bitmask 
					& RRC_OAM_STATISTICS_UE_CONTEXT_RELEASE_STATS_PRESENT)
			{
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; index++)
				{

					OAM_FILE_PRINT(pm_l3_file_ptr,"ueCtxt_relReq_cause_radioNetwork",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.ue_contxt_rel_stats.ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[index]
						      );   
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_TRANSPORT_INDEX; index++)
				{

					OAM_FILE_PRINT(pm_l3_file_ptr,"ueCtxt_relReq_cause_transport",index, (UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.ue_contxt_rel_stats.ueCtxt_relReq_cause.ueCtxt_relReq_cause_transport[index]
						      );

				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
				{

					OAM_FILE_PRINT(pm_l3_file_ptr,"ueCtxt_relReq_cause_nas",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.ue_contxt_rel_stats.ueCtxt_relReq_cause.ueCtxt_relReq_cause_nas[index]
						      );
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_PROTOCOL_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"ueCtxt_relReq_cause_protocol",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.ue_contxt_rel_stats.ueCtxt_relReq_cause.ueCtxt_relReq_cause_protocol[index]
						      );  
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
				{

					OAM_FILE_PRINT(pm_l3_file_ptr,"ueCtxt_relReq_cause_misc",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.ue_contxt_rel_stats.ueCtxt_relReq_cause.ueCtxt_relReq_cause_misc[index]
						      );
				}

				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ueCtxt_relReq_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.ue_contxt_rel_stats.ueCtxt_relReq_sum);

				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ue_contxt_rel_stats.ueContext_RelSuccNbr",(UInt32)oam_rrc_get_cell_stats_resp->rrc_connection_related_stats.ue_contxt_rel_stats.ueContext_RelSuccNbr);
			}
		}
		if(oam_rrc_get_cell_stats_resp->bitmask &
				RRC_OAM_STATISTICS_ERAB_RELATED_STATS_PRESENT)
		{
			if (oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.bitmask 
					& RRC_OAM_STATISTICS_ERAB_SETUP_STATS_PRESENT)
			{
				for(index = OAM_ONE; index < RRC_MAX_QCI_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_estabInitAttNbr_qci",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estabInitAttNbr_qci[index]
						      );
				}

				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"erab_estabInitAttNbr_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estabInitAttNbr_sum);
				for(index = OAM_ONE; index < RRC_MAX_QCI_INDEX; index++)
				{

					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_estabInitSuccNbr_qci",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estabInitSuccNbr_qci[index]
						      );  
				}
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"erab_estabInitSuccNbr_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estabInitSuccNbr_sum
						);       

				for(index = OAM_ZERO;
						index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; index++)
				{

					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_estabInitFailNbr_cause_radioNetwork",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estab_init_fail_stats.erab_estabInitFailNbr_cause_radioNetwork[index]
						      );  
				}
				for(index = OAM_ZERO;
						index < RRC_S1AP_CAUSE_TRANSPORT_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_estabInitFailNbr_cause_transport",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estab_init_fail_stats.erab_estabInitFailNbr_cause_transport[index]
						      );
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
				{

					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_estabInitFailNbr_cause_nas",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estab_init_fail_stats.erab_estabInitFailNbr_cause_nas[index]
						      );
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_PROTOCOL_INDEX ; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_estabInitFailNbr_cause_protocol",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estab_init_fail_stats.erab_estabInitFailNbr_cause_protocol[index]
						      );
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_estabInitFailNbr_cause_misc",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estab_init_fail_stats.erab_estabInitFailNbr_cause_misc[index]
						      );
				}
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"erab_estabInitFailNbr_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estabInitFailNbr_sum);           
				for(index = OAM_ONE; index < RRC_MAX_QCI_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_estabAddAttNbr_qci",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estabAddAttNbr_qci[index]
						      );
				}

				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"erab_estabAddAttNbr_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estabAddAttNbr_sum);

				for(index = OAM_ONE; index < RRC_MAX_QCI_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_estabAddSuccNbr_qci",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estabAddSuccNbr_qci[index]
						      );
				}

				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"erab_estabAddSuccNbr_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estabAddSuccNbr_sum);

				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; index++)
				{

					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_estabAddFailNbr_cause_radioNetwork",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estab_add_fail_stats.erab_estabAddFailNbr_cause_radioNetwork[index]);
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_TRANSPORT_INDEX; index++)
				{

					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_estabAddFailNbr_cause_transport",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estab_add_fail_stats.erab_estabAddFailNbr_cause_transport[index]
						      );
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_estabAddFailNbr_cause_nas",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estab_add_fail_stats.erab_estabAddFailNbr_cause_nas[index]
						      );
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_PROTOCOL_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_estabAddFailNbr_cause_protocol",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estab_add_fail_stats.erab_estabAddFailNbr_cause_protocol[index]
						      );
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_estabAddFailNbr_cause_misc",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estab_add_fail_stats.erab_estabAddFailNbr_cause_misc[index]
						      );
				}


				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"erab_estabAddFailNbr_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estabAddFailNbr_sum
						);

				for(index = OAM_ONE; index < RRC_MAX_QCI_INDEX; index++)
				{ 
					if (oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estabTime_qci[index].count > OAM_ZERO)
					{ 
						OAM_FILE_PRINT(pm_l3_file_ptr,"erab_estabTime_mean_qci",index,
								(UInt32)(oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estabTime_qci[index].erab_estabTime_Sum/oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estabTime_qci[index].count));
					}
					else
					{
						OAM_FILE_PRINT(pm_l3_file_ptr,"erab_estabTime_mean_qci",index,
								(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estabTime_qci[index].erab_estabTime_Sum);

					}
				}
				/* ERAB.EstabTimeMax on QCI basis */
				for(index = OAM_ONE; index < RRC_MAX_QCI_INDEX; index++)
				{  
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_estabTime_max_qci",index,
							(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_setup_stats.erab_estabTime_qci[index].erab_estabTime_Max);
				}
			}
			if (oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.bitmask & RRC_OAM_STATISTICS_ERAB_RELEASE_STATS_PRESENT)
			{
				for(index = OAM_ONE; index < RRC_MAX_QCI_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_relEnbNbr_qci",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.erab_relEnbNbr_qci[index]);
				}

				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"erab_relEnbNbr_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.erab_relEnbNbr_sum);
				for(index = OAM_ZERO;
						index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX;
						index++)
				{

					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_relEnbNbr_cause_radioNetwork",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.erab_rel_enb_cause_stats.erab_relEnbNbr_cause_radioNetwork[index]);
				}
				for(index = OAM_ZERO;
						index < RRC_S1AP_CAUSE_TRANSPORT_INDEX;
						index++)
				{

					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_relEnbNbr_cause_transport",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.erab_rel_enb_cause_stats.erab_relEnbNbr_cause_transport[index]);  
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_relEnbNbr_cause_nas",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.erab_rel_enb_cause_stats.erab_relEnbNbr_cause_nas[index]);  
				}
				for(index = OAM_ZERO;
						index < RRC_S1AP_CAUSE_PROTOCOL_INDEX ;
						index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_relEnbNbr_cause_protocol",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.erab_rel_enb_cause_stats.erab_relEnbNbr_cause_protocol[index]);
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_relEnbNbr_cause_misc",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.erab_rel_enb_cause_stats.erab_relEnbNbr_cause_misc[index]);  
				}
				for(index = OAM_ONE; index < RRC_MAX_QCI_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_relAttNbr_qci",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.erab_relAttNbr_qci[index]
						      ); 
				}

				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"erab_relAttNbr_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.erab_relAttNbr_sum);       

				for(index = OAM_ONE; index < RRC_MAX_QCI_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_relSuccNbr_qci",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.erab_relSuccNbr_qci[index]);       
				}
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"erab_relSuccNbr_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.erab_relSuccNbr_sum);           
				for(index = OAM_ZERO;
						index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX;
						index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_relFailNbr_cause_radioNetwork",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.rel_fail_cause_stats.erab_relFailNbr_cause_radioNetwork[index]);
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_TRANSPORT_INDEX;
						index++)
				{

					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_relFailNbr_cause_transport",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.rel_fail_cause_stats.erab_relFailNbr_cause_transport[index]);  
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_relFailNbr_cause_nas",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.rel_fail_cause_stats.erab_relFailNbr_cause_nas[index]);
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_PROTOCOL_INDEX;
						index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_relFailNbr_cause_protocol",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.rel_fail_cause_stats.erab_relFailNbr_cause_protocol[index]);
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_relFailNbr_cause_misc",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.rel_fail_cause_stats.erab_relFailNbr_cause_misc[index]);

				}

				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"erab_relFailNbr_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.erab_relFailNbr_sum);

				for(index = OAM_ONE; index < RRC_MAX_QCI_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_relActNbr_qci",index, (UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.erab_relActNbr_qci[index]);           
				}
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"erab_relActNbr_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_release_stats.erab_relActNbr_sum);          
			}

			if (oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.bitmask & RRC_OAM_STATISTICS_ERAB_MODIFY_STATS_PRESENT)
			{
				for(index = OAM_ONE; index < RRC_MAX_QCI_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_modQosAttNbr_qci",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_mod_stats.erab_modQosAttNbr_qci[index]);
				}

				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"erab_modQosAttNbr_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_mod_stats.erab_modQosAttNbr_sum);           
				for(index = OAM_ONE; index < RRC_MAX_QCI_INDEX; index++)
				{

					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_modQosSuccNbr_qci",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_mod_stats.erab_modQosSuccNbr_qci[index]);
				}
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"erab_modQosSuccNbr_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_mod_stats.erab_modQosSuccNbr_sum);
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX;
						index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_modQosFailNbr_cause_radioNetwork",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_mod_stats.erab_mod_fail_cause_stats.erab_modQosFailNbr_cause_radioNetwork[index]);           
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_TRANSPORT_INDEX;
						index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_modQosFailNbr_cause_transport",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_mod_stats.erab_mod_fail_cause_stats.erab_modQosFailNbr_cause_transport[index]);           
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_modQosFailNbr_cause_nas",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_mod_stats.erab_mod_fail_cause_stats.erab_modQosFailNbr_cause_nas[index]); 
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_PROTOCOL_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_modQosFailNbr_cause_protocol",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_mod_stats.erab_mod_fail_cause_stats.erab_modQosFailNbr_cause_protocol[index]);           
				}
				for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_modQosFailNbr_cause_misc",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_mod_stats.erab_mod_fail_cause_stats.erab_modQosFailNbr_cause_misc[index]);
				}
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"erab_modQosFailNbr_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_mod_stats.erab_modQosFailNbr_sum);

			}
			if (oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.bitmask & RRC_OAM_STATISTICS_ERAB_NUMBER_STATS_PRESENT)
			{
				/* SPR_11919_FIX_START */
				for(index = OAM_ONE; index < RRC_MAX_QCI_INDEX; index++)
					/* SPR_11919_FIX_END */
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"erab_sim_num_stats.erabSimNumber_qci",index,(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_sim_num_stats.erabSimNumber_qci[index]);
				}
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"erab_sim_num_stats.erabSimNumber_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_erab_related_stats.erab_sim_num_stats.erabSimNumber_sum);
			}
		}

		if(oam_rrc_get_cell_stats_resp->bitmask &
				RRC_OAM_STATISTICS_UE_ASSOC_S1_CONN_RELATED_STATS_PRESENT)
		{
			OAM_LOG(OAM,OAM_DETAILED,
					"RRC UE-associated logical S1-connection Stats");

			OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"rrc_ue_assoc_s1_conn_related_stats.s1sig_connEstabAtt",(UInt32)oam_rrc_get_cell_stats_resp->rrc_ue_assoc_s1_conn_related_stats.s1sig_connEstabAtt);           
			OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"s1sig_connEstabSucc",(UInt32)oam_rrc_get_cell_stats_resp->rrc_ue_assoc_s1_conn_related_stats.s1sig_connEstabSucc);
		}

		if(oam_rrc_get_cell_stats_resp->bitmask &
				RRC_OAM_STATISTICS_PAGING_RELATED_STATS_PRESENT)
		{
			OAM_LOG(OAM,OAM_DETAILED,"Number of RRC Paging Stats");
			OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"pag_discardedNbr",(UInt32)oam_rrc_get_cell_stats_resp->rrc_paging_related_stats.pag_discardedNbr);    
			OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"pag_succNbr",(UInt32)oam_rrc_get_cell_stats_resp->rrc_paging_related_stats.pag_succNbr);    
			OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"pag_receivedNbr",(UInt32)oam_rrc_get_cell_stats_resp->rrc_paging_related_stats.pag_receivedNbr);    
		}

		if (oam_rrc_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_COMMON_LA_OVERLAP_INTER_RAT_STATS_PRESENT)
		{
            /* SPR 14504 FIX START */
            /*code removed*/
            /* SPR 14504 FIX END */
			OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"rrc_IratIncMobility_LAI",(UInt32)oam_rrc_get_cell_stats_resp->rrc_IratIncMobility_LAI);    
		}
		if(oam_rrc_get_cell_stats_resp->bitmask &
				RRC_OAM_STATISTICS_HANDOVER_RELATED_STATS_PRESENT)
		{
			if(oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.bitmask & 
					RRC_OAM_INTRA_ENB_HO_STATS_PRESENT)   /* PM_OAM_CHANGES_RRC */
			{
				for(loop_counter = OAM_ZERO; loop_counter < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IntraEnbOutAtt_Cause_radioNetwork",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[loop_counter]); 
				for(loop_counter = OAM_ZERO; loop_counter < RRC_S1AP_CAUSE_TRANSPORT_INDEX; loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IntraEnbOutAtt_Cause_transport",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_transport[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter < RRC_S1AP_CAUSE_NAS_INDEX; loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IntraEnbOutAtt_Cause_nas",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_nas[loop_counter]);  
				for(loop_counter = OAM_ZERO; loop_counter < RRC_S1AP_CAUSE_PROTOCOL_INDEX; loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IntraEnbOutAtt_Cause_protocol", loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_protocol[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter < RRC_S1AP_CAUSE_MISC_INDEX; loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IntraEnbOutAtt_Cause_misc",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_misc[loop_counter]);

				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_IntraEnbOutAtt_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.intra_enb_ho_stats.ho_IntraEnbOutAtt_sum);

				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX ; loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IntraEnbOutSucc_Cause_radioNetwork",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[loop_counter]);   
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_TRANSPORT_INDEX; loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IntraEnbOutSucc_Cause_transport",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_transport[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_NAS_INDEX; loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IntraEnbOutSucc_Cause_nas",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_nas[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_PROTOCOL_INDEX ; loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IntraEnbOutSucc_Cause_protocol",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_protocol[loop_counter]); 
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_MISC_INDEX ; loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IntraEnbOutSucc_Cause_misc",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_misc[loop_counter]);

				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_IntraEnbOutSucc_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.intra_enb_ho_stats.ho_IntraEnbOutSucc_sum); /* PM_OAM_CHANGES_RRC */
			}

			if(oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.bitmask &
					RRC_OAM_INTER_ENB_HO_STATS_PRESENT)   /* PM_OAM_CHANGES_RRC */
			{	
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX ; loop_counter++)

					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutAtt_Cause_radioNetwork",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_TRANSPORT_INDEX;loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutAtt_Cause_transport",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_transport[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_NAS_INDEX; loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutAtt_Cause_nas",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_nas[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_PROTOCOL_INDEX;loop_counter++)

					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutAtt_Cause_protocol",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_protocol[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_MISC_INDEX;loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutAtt_Cause_misc",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_misc[loop_counter]);

				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_InterEnbOutAtt_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutAtt_sum);


				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX;loop_counter++)


					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutSucc_Cause_radioNetwork",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_TRANSPORT_INDEX;loop_counter++)

					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutSucc_Cause_transport",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_transport[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_NAS_INDEX;loop_counter++)

					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutSucc_Cause_nas",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_nas[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_PROTOCOL_INDEX;loop_counter++)

					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutSucc_Cause_protocol",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_protocol[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_MISC_INDEX;loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutSucc_Cause_misc",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_misc[loop_counter]);
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_InterEnbOutSucc_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutSucc_sum);

				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_InterEnbOutPrepAtt",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutPrepAtt);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX ; loop_counter++)

					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutAtt_Cause_radioNetwork",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[loop_counter]); 
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_TRANSPORT_INDEX ; loop_counter++)

					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutAtt_Cause_transport",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_transport[loop_counter]);  
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_NAS_INDEX ; loop_counter++)

					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutAtt_Cause_nas",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_nas[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_PROTOCOL_INDEX ; loop_counter++)

					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutAtt_Cause_protocol",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_protocol[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_MISC_INDEX; loop_counter++)

					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutAtt_Cause_misc",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_misc[loop_counter]); 
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_InterEnbOutAtt_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutAtt_sum);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_TRANSPORT_INDEX;loop_counter++)

					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutSucc_Cause_transport",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_transport[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_NAS_INDEX;loop_counter++)

					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutSucc_Cause_nas",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_nas[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_PROTOCOL_INDEX;loop_counter++)

					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutSucc_Cause_protocol",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_protocol[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_MISC_INDEX;loop_counter++)

					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_InterEnbOutSucc_Cause_misc",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_misc[loop_counter]);
			}
			/* PM_OAM_CHANGES_RRC_START */
			if(oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.bitmask &
					RRC_OAM_MEAS_ON_NEIGH_CELL_HO_STATS_PRESENT)   /* PM_OAM_CHANGES_RRC */
			{
				for(loop_counter = OAM_ZERO; loop_counter < oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.count ; loop_counter++)
				{
					for(loop_counter1 = OAM_ZERO; loop_counter1 <RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX;loop_counter1++)
					{
						OAM_FILE_PRINT_WITH_PARAM(pm_l3_file_ptr, "meas_on_neigh_cell_ho_stats", "ho_OutAttTarget_Cause_radioNetwork", loop_counter, loop_counter1,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork[loop_counter1]);
					}
					for(loop_counter1 = OAM_ZERO; loop_counter1 <RRC_S1AP_CAUSE_TRANSPORT_INDEX;loop_counter1++)
					{
						OAM_FILE_PRINT_WITH_PARAM(pm_l3_file_ptr, "meas_on_neigh_cell_ho_stats", "ho_OutAttTarget_Cause_transport", loop_counter, loop_counter1,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].ho_OutAttTarget_cause.ho_OutAttTarget_Cause_transport[loop_counter1]);
					}	

					for(loop_counter1 = OAM_ZERO; loop_counter1 <RRC_S1AP_CAUSE_NAS_INDEX;loop_counter1++)
					{
						OAM_FILE_PRINT_WITH_PARAM(pm_l3_file_ptr, "meas_on_neigh_cell_ho_stats", "ho_OutAttTarget_Cause_nas", loop_counter, loop_counter1,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].ho_OutAttTarget_cause.ho_OutAttTarget_Cause_nas[loop_counter1]);
					}	

					for(loop_counter1 = OAM_ZERO; loop_counter1 <RRC_S1AP_CAUSE_PROTOCOL_INDEX;loop_counter1++)
					{
						OAM_FILE_PRINT_WITH_PARAM(pm_l3_file_ptr, "meas_on_neigh_cell_ho_stats", "ho_OutAttTarget_Cause_protocol", loop_counter, loop_counter1,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].ho_OutAttTarget_cause.ho_OutAttTarget_Cause_protocol[loop_counter1]);
					}	

					for(loop_counter1 = OAM_ZERO; loop_counter1 <RRC_S1AP_CAUSE_MISC_INDEX;loop_counter1++)
					{
						OAM_FILE_PRINT_WITH_PARAM(pm_l3_file_ptr, "meas_on_neigh_cell_ho_stats", "ho_OutAttTarget_Cause_misc", loop_counter, loop_counter1,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].ho_OutAttTarget_cause.ho_OutAttTarget_Cause_misc[loop_counter1]);
					}	

					OAM_FILE_PRINT_WITH_PARAM_WITHOUT_INDEX(pm_l3_file_ptr, "meas_on_neigh_cell_ho_stats", "ho_OutAttTarget_sum", loop_counter1,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].ho_OutAttTarget_sum);	

					for(loop_counter1 = OAM_ZERO; loop_counter1 <RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX;loop_counter1++)
					{
						OAM_FILE_PRINT_WITH_PARAM(pm_l3_file_ptr, "meas_on_neigh_cell_ho_stats", "ho_OutSuccTarget_Cause_radioNetwork", loop_counter, loop_counter1,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork[loop_counter1]);
					}	

					for(loop_counter1 = OAM_ZERO; loop_counter1 <RRC_S1AP_CAUSE_TRANSPORT_INDEX;loop_counter1++)
					{
						OAM_FILE_PRINT_WITH_PARAM(pm_l3_file_ptr, "meas_on_neigh_cell_ho_stats", "ho_OutSuccTarget_Cause_transport", loop_counter, loop_counter1,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_transport[loop_counter1]);
					}	

					for(loop_counter1 = OAM_ZERO; loop_counter1 <RRC_S1AP_CAUSE_NAS_INDEX;loop_counter1++)
					{
						OAM_FILE_PRINT_WITH_PARAM(pm_l3_file_ptr, "meas_on_neigh_cell_ho_stats", "ho_OutSuccTarget_Cause_nas", loop_counter, loop_counter1,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_nas[loop_counter1]);
					}	

					for(loop_counter1 = OAM_ZERO; loop_counter1 <RRC_S1AP_CAUSE_PROTOCOL_INDEX;loop_counter1++)
					{
						OAM_FILE_PRINT_WITH_PARAM(pm_l3_file_ptr, "meas_on_neigh_cell_ho_stats", "ho_OutSuccTarget_Cause_protocol", loop_counter, loop_counter1,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_protocol[loop_counter1]);
					}	

					for(loop_counter1 = OAM_ZERO; loop_counter1 <RRC_S1AP_CAUSE_MISC_INDEX;loop_counter1++)
					{
						OAM_FILE_PRINT_WITH_PARAM(pm_l3_file_ptr, "meas_on_neigh_cell_ho_stats", "ho_OutSuccTarget_Cause_misc", loop_counter, loop_counter1,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_misc[loop_counter1]);
					}	

					OAM_FILE_PRINT_WITH_PARAM_WITHOUT_INDEX(pm_l3_file_ptr, "meas_on_neigh_cell_ho_stats", "ho_OutSuccTarget_sum", loop_counter, (UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].ho_OutSuccTarget_sum);	

				}
			}	

			/* PM_OAM_CHANGES_RRC_END */
			if(oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.bitmask &
					RRC_OAM_INTRA_FREQ_HO_STATS_PRESENT)   /* PM_OAM_CHANGES_RRC */
			{	
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_IntraFreqOutAtt",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.intra_freq_ho_stats.ho_IntraFreqOutAtt);
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_IntraFreqOutSucc",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.intra_freq_ho_stats.ho_IntraFreqOutSucc);
			} 	
			if(oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.bitmask &
					RRC_OAM_INTER_FREQ_HO_STATS_PRESENT)   /* PM_OAM_CHANGES_RRC */
			{	
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_InterFreqMeasGapOutAtt",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_freq_ho_stats.ho_InterFreqMeasGapOutAtt);
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_InterFreqMeasGapOutSucc",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_freq_ho_stats.ho_InterFreqMeasGapOutSucc);
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_InterFreqNoMeasGapOutAtt",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_freq_ho_stats.ho_InterFreqNoMeasGapOutAtt);
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_InterFreqNoMeasGapOutSucc",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_freq_ho_stats.ho_InterFreqNoMeasGapOutSucc);
			}

			if(oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.bitmask &
					RRC_OAM_DRX_NON_DRX_HO_STATS_PRESENT)   /* PM_OAM_CHANGES_RRC */
			{

				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_DrxOutAtt",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.drx_non_drx_ho_stats.ho_DrxOutAtt);
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_DrxOutSucc",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.drx_non_drx_ho_stats.ho_DrxOutSucc);
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_NoDrxOutAtt",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.drx_non_drx_ho_stats.ho_NoDrxOutAtt);
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_NoDrxOutSucc",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.drx_non_drx_ho_stats.ho_NoDrxOutSucc);
			}
			if(oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.bitmask &
					RRC_OAM_INTER_RAT_HO_STATS_PRESENT)   /* PM_OAM_CHANGES_RRC */
			{
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX;loop_counter++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IratOutAtt_Cause_radioNetwork",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_rat_ho_stats_list.inter_rat_ho_stats[OAM_ZERO].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[loop_counter]);	
				}
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_TRANSPORT_INDEX;loop_counter++)
				{
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IratOutAtt_Cause_transport",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_rat_ho_stats_list.inter_rat_ho_stats[OAM_ZERO].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_transport[loop_counter]);
				}

				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_NAS_INDEX;loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IratOutAtt_Cause_nas",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_rat_ho_stats_list.inter_rat_ho_stats[OAM_ZERO].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_nas[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_PROTOCOL_INDEX;loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IratOutAtt_Cause_protocol",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_rat_ho_stats_list.inter_rat_ho_stats[OAM_ZERO].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_protocol[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_MISC_INDEX;loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IratOutAtt_Cause_misc",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_rat_ho_stats_list.inter_rat_ho_stats[OAM_ZERO].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_misc[loop_counter]);

				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_IratOutAtt_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_rat_ho_stats_list.inter_rat_ho_stats[OAM_ZERO].ho_IratOutAtt_sum);

				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX;loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IratOutSucc_Cause_radioNetwork",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_rat_ho_stats_list.inter_rat_ho_stats[OAM_ZERO].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_TRANSPORT_INDEX;loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IratOutSucc_Cause_transport",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_rat_ho_stats_list.inter_rat_ho_stats[OAM_ZERO].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_transport[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_NAS_INDEX;loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IratOutSucc_Cause_nas",loop_counter,(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_rat_ho_stats_list.inter_rat_ho_stats[OAM_ZERO].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_nas[loop_counter]);
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_PROTOCOL_INDEX;loop_counter++)
					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IratOutSucc_Cause_protocol",loop_counter,
							(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_rat_ho_stats_list.inter_rat_ho_stats[OAM_ZERO].ho_IratOutSucc_cause.
							ho_IratOutSucc_Cause_protocol[loop_counter]);                                         
				for(loop_counter = OAM_ZERO; loop_counter <RRC_S1AP_CAUSE_MISC_INDEX;loop_counter++)

					OAM_FILE_PRINT(pm_l3_file_ptr,"ho_IratOutSucc_Cause_misc",loop_counter, (UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_rat_ho_stats_list.inter_rat_ho_stats[OAM_ZERO].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_misc[loop_counter]);
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_IratOutSucc_sum",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.inter_rat_ho_stats_list.inter_rat_ho_stats[OAM_ZERO].ho_IratOutSucc_sum);
			}
			if(oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.bitmask &
					RRC_OAM_HO_FAILURE_NUM_STATS_PRESENT)   /* PM_OAM_CHANGES_RRC */
			{
				OAM_FILE_PRINT_WITHOUT_INDEX(pm_l3_file_ptr,"ho_failure_num",(UInt32)oam_rrc_get_cell_stats_resp->rrc_handover_related_stats.ho_failure_num);
			}

		} 
		*str_count_2 = str_count;
	}
	else
	{
		OAM_LOG(OAM,OAM_INFO,
				"RRC_OAM_GET_CELL_STATS_RESP[FAILURE] with cause[%d]", 
				oam_rrc_get_cell_stats_resp->response);
	}
	OAM_FCLOSE(pm_l3_file_ptr);

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;

}

/*spr 19296 fix start*/
	/*Code deleted*/
/*spr 19296 fix end*/


/****************************************************************************
 * Function Name  : oam_pm_mac_kpi_stat_store 
 * Description    : This function fills oam_mac_kpi_stats_ind_params_t structure
 * Inputs         : p_api_buf
 * Outputs        : p_mac_kpi_stats_ind_params
 * Returns        : OAM_SUCCESS
 ****************************************************************************/
oam_return_et
oam_pm_mac_kpi_stat_store
(
 void* p_api_buf,
 oam_mac_kpi_stats_ind_params_t *p_mac_kpi_stats_ind_params
 )
{
	UInt8*    p_msg;
	p_msg = OAM_NULL;
	oam_mac_KPI_stats_ind_t *p_oam_mac_kpi_stats_ind;
	p_oam_mac_kpi_stats_ind = OAM_NULL;

	oam_counter_t loop_counter  = OAM_ZERO;
    /* SPR 23209 FIXED START*/    
    oam_counter_t loop_counter2  = OAM_ZERO;
    /* SPR 23209 FIXED END*/    


	OAM_FUN_ENTRY_TRACE();

	p_msg      = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_oam_mac_kpi_stats_ind = (oam_mac_KPI_stats_ind_t *)p_msg;

	/*SPR 10566 FIX*/
#ifndef OAM_UT_FRAMEWORK
	qvLock(pm_mutex_lock);
#endif
	/*SPR 10566 FIX*/
	p_mac_kpi_stats_ind_params->DurationTTI = p_oam_mac_kpi_stats_ind->DurationTTI;


	OAM_LOG(OAM,OAM_DETAILED,"Received  MAC_KPI_STATS_IND from MAC");
	OAM_LOG(OAM,OAM_DETAILED,"MAC_KPI_STATS_IND with duration[%u]",
			p_oam_mac_kpi_stats_ind->DurationTTI);

	/* To Create Current Time Stamp Value */
	create_time_stamp();
	OAM_LOG(OAM,OAM_DETAILED, "Current TimeStamp[%s]", time_buffer);

	p_mac_kpi_stats_ind_params->totalPRBUsageDL = p_oam_mac_kpi_stats_ind->totalPRBUsageDL;

	p_mac_kpi_stats_ind_params->totalPRBUsageUL = p_oam_mac_kpi_stats_ind->totalPRBUsageUL;

	/* SPR 13492 Changes Start */
	p_mac_kpi_stats_ind_params->totalSessionTimeUE = p_oam_mac_kpi_stats_ind->totalSessionTimeUE;
	/* SPR 13492 Changes End */

	/* loop for RRM_MAX_QCI */ 
	for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI;loop_counter++)
	{
		p_mac_kpi_stats_ind_params->totalPRBUsagePerQciDL[loop_counter] = p_oam_mac_kpi_stats_ind->totalPRBUsagePerQciDL[loop_counter];
		p_mac_kpi_stats_ind_params->totalPRBUsagePerQciUL[loop_counter] = p_oam_mac_kpi_stats_ind->totalPRBUsagePerQciUL[loop_counter];
		p_mac_kpi_stats_ind_params->numberOfActiveUEPerQciDL[loop_counter] = p_oam_mac_kpi_stats_ind->numberOfActiveUEPerQciDL[loop_counter];
		p_mac_kpi_stats_ind_params->numberOfActiveUEPerQciUL[loop_counter] = p_oam_mac_kpi_stats_ind->numberOfActiveUEPerQciUL[loop_counter];
		p_mac_kpi_stats_ind_params->totalPktDelayPerQciDL[loop_counter] = p_oam_mac_kpi_stats_ind->totalPktDelayPerQciDL[loop_counter];
		p_mac_kpi_stats_ind_params->totalPktAckedInDLPerQci[loop_counter] = p_oam_mac_kpi_stats_ind->totalPktAckedInDLPerQci[loop_counter];
		/* SPR 13492 Changes Start */
		p_mac_kpi_stats_ind_params->totalSessionTimePerQci[loop_counter]=p_oam_mac_kpi_stats_ind->totalSessionTimePerQci[loop_counter];   
		/* SPR 13492 Changes End */
/*spr 19296 fix start*/
		p_mac_kpi_stats_ind_params->totalDlDrbUuLossRatePerQci[loop_counter]=p_oam_mac_kpi_stats_ind->totalDlDrbUuLossRatePerQci[loop_counter];   
		p_mac_kpi_stats_ind_params->totalSduAckRcvdDlperQci[loop_counter]=p_oam_mac_kpi_stats_ind->totalSduAckRcvdDlperQci[loop_counter];   
/*spr 19296 fix end*/
	}
    /* SPR 23209 FIXED START */
    for(loop_counter = OAM_ZERO;loop_counter < OAM_MAX_PLMNLIST_OBJECTS;loop_counter++)
    {
        p_mac_kpi_stats_ind_params->kpiPerOperator[loop_counter].operatorId = 
            p_oam_mac_kpi_stats_ind->kpiPerOperator[loop_counter].operatorId;

        for(loop_counter2 = OAM_ZERO;loop_counter2 < MAX_QCI;loop_counter2++)
        {
            p_mac_kpi_stats_ind_params->kpiPerOperator[loop_counter].totalPRBUsagePerQciDL[loop_counter2] 
                = p_oam_mac_kpi_stats_ind->kpiPerOperator[loop_counter].totalPRBUsagePerQciDL[loop_counter2];

            p_mac_kpi_stats_ind_params->kpiPerOperator[loop_counter].totalPRBUsagePerQciUL[loop_counter2] 
                = p_oam_mac_kpi_stats_ind->kpiPerOperator[loop_counter].totalPRBUsagePerQciUL[loop_counter2];

            p_mac_kpi_stats_ind_params->kpiPerOperator[loop_counter].numberOfActiveUEPerQciDL[loop_counter2] 
                = p_oam_mac_kpi_stats_ind->kpiPerOperator[loop_counter].numberOfActiveUEPerQciDL[loop_counter2];

            p_mac_kpi_stats_ind_params->kpiPerOperator[loop_counter].numberOfActiveUEPerQciUL[loop_counter2] 
                = p_oam_mac_kpi_stats_ind->kpiPerOperator[loop_counter].numberOfActiveUEPerQciUL[loop_counter2];
        }
    }
   
    /* SPR 23209 FIXED END */


	p_mac_kpi_stats_ind_params->rcvdRandomAccessPreamblesContentionFree = p_oam_mac_kpi_stats_ind->rcvdRandomAccessPreamblesContentionFree;
	p_mac_kpi_stats_ind_params->rcvdRandomAccessPreamblesGroupA = p_oam_mac_kpi_stats_ind->rcvdRandomAccessPreamblesGroupA;
	p_mac_kpi_stats_ind_params->rcvdRandomAccessPreamblesGroupB = p_oam_mac_kpi_stats_ind->rcvdRandomAccessPreamblesGroupB;
	/*eICIC_PHASE_1_2_CHANGES_START*/
	p_mac_kpi_stats_ind_params->AbsSubframeCount = p_oam_mac_kpi_stats_ind->AbsSubframeCount;
	/*eICIC_PHASE_1_2_CHANGES_END*/
 
    OAM_LOG(OAM, OAM_INFO, " Received  MAC_KPI_STATS_IND from MAC "
            "with duration[%u]. Current TimeStamp[%s]."
            " RachCF = %llu RachA = %llu RachB = %llu TIME_UNIT_TO_MILLISEC = %d ",
                              p_oam_mac_kpi_stats_ind->DurationTTI,
                              time_buffer, p_mac_kpi_stats_ind_params->rcvdRandomAccessPreamblesContentionFree,
                              p_mac_kpi_stats_ind_params->rcvdRandomAccessPreamblesGroupA,
                              p_mac_kpi_stats_ind_params->rcvdRandomAccessPreamblesGroupB,
                              TIME_UNIT_TO_MILLISEC
                              );


	/* SPR 12670 Changes Start */
	for(loop_counter = OAM_ZERO; loop_counter < MAX_CQI_INDEX; ++loop_counter)
	{
		p_mac_kpi_stats_ind_params->totalCQICW1Reported[loop_counter] = 
			p_oam_mac_kpi_stats_ind->totalCQICW1Reported[loop_counter];
		p_mac_kpi_stats_ind_params->totalCQICW2Reported[loop_counter] = 
			p_oam_mac_kpi_stats_ind->totalCQICW2Reported[loop_counter];
	
        OAM_LOG(OAM,OAM_DETAILEDALL,"Values Stored totalPRBUsageDL%llu CQI ID =%d CW1 %d CW2 %d ",
			 p_oam_mac_kpi_stats_ind->totalPRBUsageDL, 
             loop_counter,
             p_mac_kpi_stats_ind_params->totalCQICW1Reported[loop_counter],
             p_mac_kpi_stats_ind_params->totalCQICW2Reported[loop_counter]
             );
	}
	/* SPR 12670 Changes End */

	/*SPR 10566 FIX*/
#ifndef OAM_UT_FRAMEWORK
	qvUnlock(pm_mutex_lock);
#endif
	/*SPR 10566 FIX*/
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

void
oam_pm_rrm_read_data_from_link_list
(
/* + SPR 17439 */
 void
)
/* - SPR 17439 */
{
	oam_pm_rrm_kpi_info_Node_t *p_pm_kpi_info_Node = rrm_kpi_info.pm_kpi_info_first_Node;
	oam_error_code_et error_code;

	OAM_FUN_ENTRY_TRACE();
	if(p_pm_kpi_info_Node == OAM_NULL)
	{
		OAM_LOG(OAM,OAM_DETAILED,"data not found");
	}
	else
	{
		if (OAM_NULL != rrm_kpi_info.pm_kpi_info_first_Node)
		{
			oam_pm_rrm_read_linked_list_kpi_file(&p_pm_kpi_info_Node->oam_rrm_kpi_ind);
			rrm_kpi_info.pm_kpi_info_first_Node = rrm_kpi_info.pm_kpi_info_first_Node->next_node_ptr;
			/* Klockwork Fix */
			oam_mem_free((void *)p_pm_kpi_info_Node,&error_code);
			p_pm_kpi_info_Node = OAM_NULL;    
		}

	}
	OAM_FUN_EXIT_TRACE();
}

/****************************************************************************
 * Function Name  : oam_add_in_mac_link_list 
 * Description    : This function make a node in  mac link list and fill data in new
 *                : node 
 * Inputs         : NONE
 * Outputs        : p_kpi_info :pointer to structure oam_kpi_info_t
 * Returns        : OAM_FAILURE/OAM_SUCCESS
 ****************************************************************************/
oam_return_et
oam_add_in_mac_link_list
(
 oam_kpi_info_t *p_kpi_info,
 UInt8          cell_index,
 void*          p_kpi_buf
 )
{
	oam_error_code_et p_error_code = NO_ERROR; /* Coverity_14DEC */
	OAM_FUN_ENTRY_TRACE();

	OAM_NULL_CHECK(OAM_NULL != p_kpi_info);
	oam_pm_kpi_info_Node_t *oam_pm_kpi_info_Node = OAM_NULL;

	if (OAM_FAILURE == oam_mem_alloc(sizeof(oam_pm_kpi_info_Node_t),(void *)&oam_pm_kpi_info_Node,&p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to oam_pm_kpi_info_Node failed"
                " with Error Code:%d",
				p_error_code);
		OAM_FUN_EXIT_TRACE();
		/* Not Enough Memory */
		return OAM_FAILURE;
	}

	OAM_LOG(OAM,OAM_DETAILED,"address = %p",oam_pm_kpi_info_Node);

	oam_pm_kpi_info_Node->next_node_ptr = OAM_NULL;

	oam_pm_updat_nxt_node_ptr_of_link_lst((oam_kpi_info_t*)p_kpi_info,(oam_pm_kpi_info_Node_t*)oam_pm_kpi_info_Node,cell_index);

	oam_pm_mac_kpi_stat_store (
			p_kpi_buf,
			&(oam_pm_kpi_info_Node->MacKpiStatsIndParams));
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/* SPR_11231_START */
/****************************************************************************
 * Function Name  : oam_add_in_pdcp_ind_link_list 
 * Description    : This function make a node in  pdcp link list and fill data in new
 *                : node 
 * Inputs         : NONE 
 * Outputs        : p_kpi_info :pointer to structure oam_kpi_info_t
 * Returns        : OAM_FAILURE/OAM_SUCCESS
 ****************************************************************************/
oam_return_et
oam_add_in_pdcp_ind_link_list
(
 oam_pdcp_kpi_info_t *p_kpi_info
 )
{
	oam_error_code_et p_error_code;
	UInt16 index = OAM_ZERO;
	OAM_FUN_ENTRY_TRACE();

	OAM_ASSERT(OAM_NULL != p_kpi_info);
	oam_pm_pdcp_kpi_info_node_t *oam_pm_pdcp_kpi_info_Node = OAM_NULL;

	if (OAM_FAILURE == oam_mem_alloc(sizeof(oam_pm_pdcp_kpi_info_node_t),(void *)&oam_pm_pdcp_kpi_info_Node,&p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to oam_pm_pdcp_kpi_info_Node failed"
                " with Error Code:%d",
				p_error_code);
		OAM_FUN_EXIT_TRACE();
		/* Not Enough Memory */
		return OAM_FAILURE;
	}

	oam_pm_pdcp_kpi_info_Node->next_node_ptr = OAM_NULL;
	oam_pm_pdcp_kpi_info_Node->next_batch_node = OAM_NULL;

    g_pdcp_kpi_ind_handler.num_pdcp_kpi_stats_ind = OAM_ZERO;
    /*SPR 20171 Start*/
    OAM_LOG(OAM,OAM_DETAILED,"address = %p, num_pdcp_kpi_stats_ind = %d",
            oam_pm_pdcp_kpi_info_Node,g_pdcp_kpi_ind_handler.num_pdcp_kpi_stats_ind);
    /*SPR 20171 End*/ 

	oam_pm_pdcp_updat_nxt_node_ptr_of_link_lst ((
				oam_pdcp_kpi_info_t *)p_kpi_info,
			(oam_pm_pdcp_kpi_info_node_t *)oam_pm_pdcp_kpi_info_Node);

	for(index = OAM_ZERO; index < oam_prov_req.num_cells_configured; index++)
	{
		oam_pm_pdcp_kpi_info_Node->oam_pm_pdcp_kpi_stats_node[index].cell_index = 
			(UInt8)*(UInt8 *)(g_pdcp_kpi_ind_handler.p_kpi_buf[index] + OAM_INTF_HDR_CELL_IDX_OFFSET);
		oam_pm_pdcp_kpi_info_Node->oam_pm_pdcp_kpi_stats_node[index].Is_ind_present = OAM_TRUE;
		oam_pm_pdcp_kpi_stat_ind (
				g_pdcp_kpi_ind_handler.p_kpi_buf[index],
				&(oam_pm_pdcp_kpi_info_Node->oam_pm_pdcp_kpi_stats_node[index].oam_pdcp_kpi_stats_ind));
        /* SPR 22296 Fix Start */
	/*SPR 10023 FIX*/
        oam_pm_pdcp_kpi_info_Node->oam_pm_pdcp_kpi_stats_node[index].p_oam_pdcp_kpi_thp_stats_ind = OAM_NULL;
        if (oam_add_pdcp_kpi_thp_ind(&(oam_pm_pdcp_kpi_info_Node->oam_pm_pdcp_kpi_stats_node[index].p_oam_pdcp_kpi_thp_stats_ind),
                    oam_pm_pdcp_kpi_info_Node->oam_pm_pdcp_kpi_stats_node[index].cell_index) == OAM_FAILURE)
	{
		OAM_LOG(OAM,OAM_WARNING, "Error adding PDCP_KPI_THP_STATS_IND in PDCP Link List, error=%d", p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
    }
	ConsolidatedPdcpThpStatsInd = OAM_NULL;
	/* SPR 22296 Fix End */
	/*SPR 10023 FIX*/
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/* SPR_11231_END*/
/****************************************************************************
 * Function Name  : oam_pm_rrm_updat_nxt_node_ptr_of_link_lst 
 * Description    : This function update a node in rrm link list 
 * Inputs         : p_oam_pm_kpi_info_Node     
 * Outputs        : p_oam_rrm_kpi_info
 * Returns        : NONE 
 ****************************************************************************/
void 
oam_pm_rrm_updat_nxt_node_ptr_of_link_lst
(
 oam_rrm_kpi_info_t *p_oam_rrm_kpi_info,	 
 oam_pm_rrm_kpi_info_Node_t *p_oam_pm_kpi_info_Node
 )
{     
	OAM_FUN_ENTRY_TRACE();
	oam_pm_rrm_kpi_info_Node_t *p_pm_kpi_info_Node_tmp;    

	OAM_ASSERT(OAM_NULL != p_oam_pm_kpi_info_Node);

	if(OAM_NULL != p_oam_rrm_kpi_info)
	{ 
		if(p_oam_rrm_kpi_info->pm_kpi_info_first_Node == OAM_NULL)
		{
			if(OAM_NULL != p_oam_pm_kpi_info_Node)
			{ 
				p_oam_rrm_kpi_info->pm_kpi_info_first_Node = p_oam_pm_kpi_info_Node;
			}
		}
		else
		{
			p_pm_kpi_info_Node_tmp = p_oam_rrm_kpi_info->pm_kpi_info_first_Node;

			while(p_pm_kpi_info_Node_tmp->next_node_ptr != OAM_NULL)
			{
				p_pm_kpi_info_Node_tmp  = p_pm_kpi_info_Node_tmp->next_node_ptr;

			}
			if(OAM_NULL != p_oam_pm_kpi_info_Node)
			{ 
				p_pm_kpi_info_Node_tmp->next_node_ptr = p_oam_pm_kpi_info_Node;   
			}
		}
	}

	OAM_FUN_EXIT_TRACE(); 
}


/****************************************************************************
 * Function Name  : oam_add_in_rrm_link_list 
 * Description    : This function make a node in rrm link list 
 * Inputs         :  p_rrm_kpi_info 
 * Outputs        :  NONE
 * Returns        : NONE 
 ****************************************************************************/
/* SPR 17777 fix */
oam_return_et
oam_add_in_rrm_link_list
(
 oam_rrm_kpi_info_t *p_rrm_kpi_info
 )
{
	oam_error_code_et p_error_code;
	OAM_FUN_ENTRY_TRACE();

	oam_pm_rrm_kpi_info_Node_t *oam_pm_kpi_info_Node = OAM_NULL;

	if (OAM_FAILURE == oam_mem_alloc(sizeof(oam_pm_rrm_kpi_info_Node_t),(void *)&oam_pm_kpi_info_Node,&p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to oam_pm_kpi_info_Node failed"
                " with Error Code:%d",
				p_error_code);
		OAM_FUN_EXIT_TRACE();
		/* Not Enough Memory */
		return OAM_FAILURE;
	}

	OAM_LOG(OAM,OAM_DETAILED,"address=%p",oam_pm_kpi_info_Node);
	oam_pm_kpi_info_Node->next_node_ptr = OAM_NULL;
	oam_pm_rrm_updat_nxt_node_ptr_of_link_lst(p_rrm_kpi_info,(oam_pm_rrm_kpi_info_Node_t *)oam_pm_kpi_info_Node);

	oam_pm_rrm_kpi_stat_ind(g_rrm_kpi_ind_handler.rrm_kpi_buf,&(oam_pm_kpi_info_Node->oam_rrm_kpi_ind));

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_add_in_rrc_link_list 
 * Description    : This function make a node in rrc link list and fill data in new
 *                : node 
 * Inputs         : p_input_arr : pointer to an array 
 * Outputs        : p_kpi_info : pointer to structure oam_rrc_kpi_info_t 
 * Returns        : OAM_FAILURE/OAM_SUCCESS
 ****************************************************************************/
/* SPR 17777 fix */
oam_return_et
oam_add_in_rrc_link_list
(
 oam_rrc_kpi_info_t *p_kpi_info
 )
{
	oam_error_code_et p_error_code;
	UInt32 data_len = OAM_ZERO;  
	SInt32 length = OAM_ZERO;
	oam_pm_rrc_kpi_info_Node_t   *p_oam_pm_kpi_info_Node = OAM_NULL;
	g_l3_kpi_ind_handler.ind_bit_mask = OAM_RRC_KPI_MAP;  
	OAM_FUN_ENTRY_TRACE();

	OAM_ASSERT(OAM_NULL != p_kpi_info);
	data_len = get_word_from_header((UInt8*)(g_l3_kpi_ind_handler.p_kpi_handover_buf) +
			OAM_INTF_HDR_MSG_SIZE_OFFSET);     

	if (OAM_FAILURE == oam_mem_alloc(sizeof(oam_pm_rrc_kpi_info_Node_t),(void *)&p_oam_pm_kpi_info_Node,&p_error_code))
	{     
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_oam_pm_kpi_info_Node failed"
                " with Error Code:%d",
				p_error_code);
		OAM_FUN_EXIT_TRACE();
		/* Not Enough Memory */
		return OAM_FAILURE;
	}     

	p_oam_pm_kpi_info_Node->next_node_ptr = OAM_NULL;

	oam_pm_rrc_updat_nxt_node_ptr_of_link_lst(p_kpi_info,p_oam_pm_kpi_info_Node);

	data_len = get_word_from_header((UInt8*)(g_l3_kpi_ind_handler.p_kpi_irat_mobility_buf) +
			OAM_INTF_HDR_MSG_SIZE_OFFSET);
	length = OAM_ZERO; 
	g_l3_kpi_ind_handler.p_kpi_irat_mobility_buf = g_l3_kpi_ind_handler.p_kpi_irat_mobility_buf + (LOWER_LAYER_INTERFACE_HEADER_SIZE); 
	/*SPR 10566 FIX*/
#ifndef OAM_UT_FRAMEWORK
	qvLock(pm_mutex_lock);
#endif
	/*SPR 10566 FIX*/
	if ( OAM_FAILURE == oam_rrc_il_parse_rrc_oam_irat_mobility_stats_ind(
				&(p_oam_pm_kpi_info_Node->irat_mobility_stats_ind),
				(UInt8*)(g_l3_kpi_ind_handler.p_kpi_irat_mobility_buf),
				data_len - LOWER_LAYER_INTERFACE_HEADER_SIZE,
				&length))
	{
		OAM_LOG(OAM,OAM_WARNING,
				"RRC_OAM_IRAT_MOBILITY_STATS_IND parsing Failure");
		/*SPR 10566 FIX*/
#ifndef OAM_UT_FRAMEWORK
		qvUnlock(pm_mutex_lock);
#endif
		/*SPR 10566 FIX*/
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	data_len = get_word_from_header((UInt8*)(g_l3_kpi_ind_handler.p_kpi_ue_assoc_buf) +
			OAM_INTF_HDR_MSG_SIZE_OFFSET);
	length = OAM_ZERO; 
	g_l3_kpi_ind_handler.p_kpi_ue_assoc_buf = g_l3_kpi_ind_handler.p_kpi_ue_assoc_buf + (LOWER_LAYER_INTERFACE_HEADER_SIZE); 
	if ( OAM_FAILURE == oam_rrc_il_parse_rrc_oam_ue_assoc_s1_conn_stats_ind(
				&(p_oam_pm_kpi_info_Node->ue_assoc_s1_conn_stats_ind_t),
				(UInt8*)(g_l3_kpi_ind_handler.p_kpi_ue_assoc_buf),
				data_len-LOWER_LAYER_INTERFACE_HEADER_SIZE,
				&length))
	{
		OAM_LOG(OAM,OAM_WARNING,
				"RRC_OAM_UE_ASSOC_S1_CONN_STATS_IND Parsing Failure");
		/*SPR 10566 FIX*/
#ifndef OAM_UT_FRAMEWORK
		qvUnlock(pm_mutex_lock);
#endif
		/*SPR 10566 FIX*/
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	if(g_l3_kpi_ind_handler.p_kpi_paging_buf != OAM_NULL)
	{
		data_len = get_word_from_header((UInt8*)(g_l3_kpi_ind_handler.p_kpi_paging_buf) +
				OAM_INTF_HDR_MSG_SIZE_OFFSET);
		length = OAM_ZERO; 
		g_l3_kpi_ind_handler.p_kpi_paging_buf  = g_l3_kpi_ind_handler.p_kpi_paging_buf + (LOWER_LAYER_INTERFACE_HEADER_SIZE); 
		if ( OAM_FAILURE == oam_rrc_il_parse_rrc_oam_paging_stats_ind(
					&(p_oam_pm_kpi_info_Node->paging_stats_ind_t),
					(UInt8*)(g_l3_kpi_ind_handler.p_kpi_paging_buf),
					data_len-LOWER_LAYER_INTERFACE_HEADER_SIZE,
					&length))
		{
			OAM_LOG(OAM,OAM_WARNING,
					"RRC_OAM_PAGING_STATS_IND Parsing Failure");
			/*SPR 10566 FIX*/
#ifndef OAM_UT_FRAMEWORK
			qvUnlock(pm_mutex_lock);
#endif
			/*SPR 10566 FIX*/
			OAM_FUN_EXIT_TRACE();
			return OAM_FAILURE;
		}
	}
	data_len = get_word_from_header((UInt8*)(g_l3_kpi_ind_handler.p_kpi_conn_buf) +
			OAM_INTF_HDR_MSG_SIZE_OFFSET);
	length = OAM_ZERO; 
	g_l3_kpi_ind_handler.p_kpi_conn_buf = g_l3_kpi_ind_handler.p_kpi_conn_buf + (LOWER_LAYER_INTERFACE_HEADER_SIZE); 
	if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_rrc_connection_stats_ind(
				&(p_oam_pm_kpi_info_Node->rrc_connection_stats_ind),
				(UInt8*)(g_l3_kpi_ind_handler.p_kpi_conn_buf),
				data_len-LOWER_LAYER_INTERFACE_HEADER_SIZE,
				&length))
	{
		OAM_LOG(OAM,OAM_WARNING,
				"RRC_OAM_RRC_CONNECTION_STATS_IND Parsing Failure");
		/*SPR 10566 FIX*/
#ifndef OAM_UT_FRAMEWORK
		qvUnlock(pm_mutex_lock);
#endif
		/*SPR 10566 FIX*/
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	data_len = get_word_from_header((UInt8*)(g_l3_kpi_ind_handler.p_kpi_erab_buf) +
			OAM_INTF_HDR_MSG_SIZE_OFFSET);
	length = OAM_ZERO; 
	g_l3_kpi_ind_handler.p_kpi_erab_buf = g_l3_kpi_ind_handler.p_kpi_erab_buf + (LOWER_LAYER_INTERFACE_HEADER_SIZE); 
	if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_erab_stats_ind(
				&(p_oam_pm_kpi_info_Node->erab_stats_ind_t),
				(UInt8*)(g_l3_kpi_ind_handler.p_kpi_erab_buf),
                /*SPR 23209 FIXED START*/
				data_len -LOWER_LAYER_INTERFACE_HEADER_SIZE,
                /*SPR 23209 FIXED END*/
				&length))
	{
		OAM_LOG(OAM,OAM_WARNING,
				"RRC_OAM_ERAB_STATS_IND Parsing Failure");
		/*SPR 10566 FIX*/
#ifndef OAM_UT_FRAMEWORK
		qvUnlock(pm_mutex_lock);
#endif
		/*SPR 10566 FIX*/
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	data_len = get_word_from_header((UInt8*)(g_l3_kpi_ind_handler.p_kpi_handover_buf) +
			OAM_INTF_HDR_MSG_SIZE_OFFSET);
	length = OAM_ZERO; 
	g_l3_kpi_ind_handler.p_kpi_handover_buf = g_l3_kpi_ind_handler.p_kpi_handover_buf + (LOWER_LAYER_INTERFACE_HEADER_SIZE); 
	if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_handover_stats_ind(
				&(p_oam_pm_kpi_info_Node->oam_rrc_handover_stats_ind),
				(UInt8*)(g_l3_kpi_ind_handler.p_kpi_handover_buf),
                /*SPR 23209 FIXED START*/
				data_len-LOWER_LAYER_INTERFACE_HEADER_SIZE,
                /*SPR 23209 FIXED END*/
				&length))
	{
		OAM_LOG(OAM,OAM_WARNING,
				"RRC_OAM_HANDOVER_STATS_IND Parsing Failure");
		/*SPR 10566 FIX*/
#ifndef OAM_UT_FRAMEWORK
		qvUnlock(pm_mutex_lock);
#endif
		/*SPR 10566 FIX*/
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}


    /* SPR 15018 START */
    oam_calculate_endTime(p_oam_pm_kpi_info_Node->timeStamp);
    /* SPR 15018 END */


	/*SPR 10566 FIX*/
#ifndef OAM_UT_FRAMEWORK
	/* COVERITY: 63659 FIX START */
	qvUnlock(pm_mutex_lock);
	/* COVERITY: 63659 FIX END */
#endif
	/*SPR 10566 FIX*/
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pm_updat_nxt_node_ptr_of_link_lst 
 * Description    : This function update next node pointer field of link_lst
 * Inputs         : p_kpi_info : pointer to structure oam_kpi_info_t 
 * Outputs        : oam_pm_kpi_info_Node_t : pointer to structure oam_pm_kpi_info_Node_t
 * Returns        : NONE 
 ****************************************************************************/
oam_return_et 
oam_pm_updat_nxt_node_ptr_of_link_lst
(
 oam_kpi_info_t *p_kpi_info,
 oam_pm_kpi_info_Node_t *p_oam_pm_kpi_info_Node,
 UInt8 cell_index
 )
{     
	OAM_FUN_ENTRY_TRACE();     
	oam_pm_kpi_info_Node_t *pm_kpi_info_Node_tmp;    

	OAM_ASSERT(OAM_NULL != p_oam_pm_kpi_info_Node);
	OAM_NULL_CHECK(OAM_NULL != p_kpi_info);
	if((OAM_NULL != p_kpi_info) && (OAM_NULL != p_oam_pm_kpi_info_Node))
	{
		if(p_kpi_info->pm_kpi_info_first_Node == OAM_NULL)
		{

			p_kpi_info->pm_kpi_info_first_Node = p_oam_pm_kpi_info_Node; 
			p_kpi_info->pm_kpi_info_first_Node->cell_index = cell_index; 
		}
		else
		{
			pm_kpi_info_Node_tmp = p_kpi_info->pm_kpi_info_first_Node;

			while(pm_kpi_info_Node_tmp->next_node_ptr != OAM_NULL)
			{
				pm_kpi_info_Node_tmp  = pm_kpi_info_Node_tmp->next_node_ptr;
			}
			pm_kpi_info_Node_tmp->next_node_ptr = p_oam_pm_kpi_info_Node;   
			p_oam_pm_kpi_info_Node->cell_index = cell_index;
		}
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/* SPR_11231_START */
/****************************************************************************
 * Function Name  : oam_pm_pdcp_updat_nxt_node_ptr_of_link_lst
 * Description    : This function update next node pointer field of link_lst
 * Inputs         : p_kpi_info : Pointer to structure oam_pdcp_kpi_info_t 
 * Outputs        : p_oam_pm_kpi_info_Node : pointer to structure oam_pm_pdcp_kpi_info_Node_t
 * Returns        : NONE 
 ****************************************************************************/
void 
oam_pm_pdcp_updat_nxt_node_ptr_of_link_lst
( 
 oam_pdcp_kpi_info_t *p_kpi_info,
 oam_pm_pdcp_kpi_info_node_t *p_oam_pm_pdcp_kpi_info_Node
 )
{     
	OAM_FUN_ENTRY_TRACE();
	oam_pm_pdcp_kpi_info_node_t *p_pm_pdcp_kpi_info_Node_tmp = OAM_NULL;    

	OAM_ASSERT(OAM_NULL != p_oam_pm_pdcp_kpi_info_Node);
	OAM_ASSERT(OAM_NULL != p_kpi_info);

	if (OAM_NULL != p_kpi_info)
	{
		if(p_kpi_info->pm_pdcp_kpi_info_first_Node == OAM_NULL)
		{
			if(OAM_NULL != p_oam_pm_pdcp_kpi_info_Node)
			{ 
				p_kpi_info->pm_pdcp_kpi_info_first_Node = p_oam_pm_pdcp_kpi_info_Node; 
			}
		}
		else
		{
			p_pm_pdcp_kpi_info_Node_tmp = p_kpi_info->pm_pdcp_kpi_info_first_Node;

			while(p_pm_pdcp_kpi_info_Node_tmp->next_node_ptr != OAM_NULL)
			{
				p_pm_pdcp_kpi_info_Node_tmp  = p_pm_pdcp_kpi_info_Node_tmp->next_node_ptr;

			}
			if(OAM_NULL != p_oam_pm_pdcp_kpi_info_Node)
			{ 
				p_pm_pdcp_kpi_info_Node_tmp->next_node_ptr = p_oam_pm_pdcp_kpi_info_Node;   
			}
		}

	}
	else
	{
		OAM_LOG(OAM, OAM_INFO,"NULL value of p_kpi_info received");
		OAM_FUN_EXIT_TRACE();
	}
	OAM_FUN_EXIT_TRACE(); 
}

/* SPR_11231_END */
/****************************************************************************
 * Function Name  : oam_pm_rrc_updat_nxt_node_ptr_of_link_lst
 * Description    : This function update next node pointer field of link_lst
 * Inputs         : p_kpi_info : 
 * Outputs        : p_oam_pm_kpi_info_Node :
 * Returns        : void

 ****************************************************************************/

void 
oam_pm_rrc_updat_nxt_node_ptr_of_link_lst
(   
 oam_rrc_kpi_info_t *p_kpi_info,
 oam_pm_rrc_kpi_info_Node_t *p_oam_pm_kpi_info_Node
 )
{     
	oam_pm_rrc_kpi_info_Node_t *p_pm_kpi_info_Node_tmp;    

	OAM_ASSERT(OAM_NULL != p_oam_pm_kpi_info_Node);
	OAM_ASSERT(OAM_NULL != p_kpi_info);

	OAM_FUN_ENTRY_TRACE();
    /*SPR 20171 Start*/
    OAM_LOG(OAM,OAM_DETAILED,"Thread Id :[%lu]",pthread_self());
    /*SPR 20171 End*/


	if(OAM_NULL != p_kpi_info)
	{
		if(p_kpi_info->pm_kpi_info_first_Node == OAM_NULL)
		{
			if(OAM_NULL != p_oam_pm_kpi_info_Node)
			{
				p_kpi_info->pm_kpi_info_first_Node = p_oam_pm_kpi_info_Node; 
                /*SPR 20171 Start*/
                OAM_LOG(OAM,OAM_DETAILED,"Thread Id :[%lu] Making first Node = %p",pthread_self(),p_oam_pm_kpi_info_Node);		
                /*SPR 20171 End*/
            }
            else
            {
                /*SPR 20171 Start*/
                OAM_LOG(OAM,OAM_ERROR,"Thread Id :[%lu] p_oam_pm_kpi_info_Node NULL",pthread_self());									
                /*SPR 20171 End*/

			}
		}
		else
		{
			p_pm_kpi_info_Node_tmp = p_kpi_info->pm_kpi_info_first_Node;
            /*SPR 20171 Start*/
            OAM_LOG(OAM,OAM_DETAILED,"Thread Id :[%lu] NON NULL first Node = %p",pthread_self(),p_kpi_info->pm_kpi_info_first_Node);		
            /*SPR 20171 End*/

			while(p_pm_kpi_info_Node_tmp->next_node_ptr != OAM_NULL)
			{
				p_pm_kpi_info_Node_tmp  = p_pm_kpi_info_Node_tmp->next_node_ptr;
                /*SPR 20171 Start*/
                OAM_LOG(OAM,OAM_DETAILED,"Thread Id :[%lu] first Node Not NULL Traversing next ptr %p",pthread_self(),p_pm_kpi_info_Node_tmp->next_node_ptr);						
                /*SPR 20171 End*/

			}
			if(OAM_NULL != p_oam_pm_kpi_info_Node)
			{
				p_pm_kpi_info_Node_tmp->next_node_ptr = p_oam_pm_kpi_info_Node;   
			}
            else
            {
                /*SPR 20171 Start*/
                OAM_LOG(OAM,OAM_INFO,"Thread Id :[%lu] p_oam_pm_kpi_info_Node NULL",pthread_self());								
                /*SPR 20171 End*/

            }
		}

	}
	OAM_FUN_EXIT_TRACE();

}

/****************************************************************************
 * Function Name  : oam_pm_rrc_updat_nxt_node_ptr_of_link_lst
 * Description    : This function update next node pointer field of link_lst
 * Inputs         : p_kpi_info : pointer to structure oam_rrc_kpi_info_t
 * Outputs        : p_oam_pm_kpi_info_Node : pointer to structure oam_pm_rrc_kpi_info_Node_t
 * Returns        : NONE
 ****************************************************************************/
void 
	oam_pm_init_kpi_info
(
/* + SPR 17439 */
 void
)
/* - SPR 17439 */
{
	OAM_FUN_ENTRY_TRACE();
/*spr 19296 fix start*/
	oam_pm_kpi_mac_info_strct_init(&mac_kpi_info);
/*spr 19296 fix end*/
	oam_pm_kpi_pdcp_info_strct_init(&pdcp_kpi_info);
	oam_pm_kpi_rrc_info_strct_init(&rrc_kpi_info);
	OAM_FUN_EXIT_TRACE();
}

/****************************************************************************
 * Function Name  : oam_pm_init 
 * Inputs         : NONE 
 * Outputs        : NONE
 * Returns        : NONE
 * Description    : This function calls different init function to initialise PM module
 ****************************************************************************/
void
	oam_pm_init
(
/* + SPR 17439 */
 void
)
/* - SPR 17439 */
{
	OAM_FUN_ENTRY_TRACE();
	oam_pm_init_kpi_info();

	oam_pm_pdcp_init_kpi_name();
	oam_pm_rrc_init_kpi_thp_name();

	oam_pm_bitmask_init();
	oam_pm_xml_init_info();
	file_close.rrm_flag = OAM_ZERO;
	file_close.flag = OAM_ZERO;
	cli_reset_flag.mac_reset_flag = OAM_ZERO;
	cli_reset_flag.pdcp_reset_flag = OAM_ZERO;
	OAM_FUN_EXIT_TRACE();
}


/****************************************************************************
 * Function Name  : oam_pm_init_l2_kpi_ind_struct 
 * Inputs         : p_kpi_ind_handl_strt : pointer to structure oam_pm_init_l2_kpi_ind_struct
 * Outputs        : NONE
 * Returns        : NONE
 * Description    : this function initialise l2 kpi ind structure
 ****************************************************************************/
void oam_pm_init_l2_kpi_ind_struct(oam_l2_kpi_ind_handler_t *p_kpi_ind_handl_strt)
{
	OAM_ASSERT(OAM_NULL != p_kpi_ind_handl_strt);
	OAM_FUN_ENTRY_TRACE();
	if (OAM_NULL != p_kpi_ind_handl_strt)
	{
		p_kpi_ind_handl_strt->p_kpi_buf = OAM_NULL;
		p_kpi_ind_handl_strt->kpi_resp_recvd_flag = OAM_ZERO;
	}
	OAM_FUN_EXIT_TRACE();
}

/* SPR_11231_START */

/****************************************************************************
 * Function Name  : search_and_del_pdcp_node_for_cell_id 
 * Inputs         : p_pdcp_node_ptr: pointer to structure oam_pm_pdcp_kpi_info_Node_t 
 : rrc_cell_id : Cell ID 
 * Outputs        : NONE
 * Returns        : NONE
 * Description    : this function search and delete pdcp node acc to cell id 
 ****************************************************************************/
UInt16
search_and_del_pdcp_node_for_cell_id
(
 oam_pm_pdcp_kpi_info_node_t *p_pdcp_node_ptr, 
 UInt8 rrc_cell_id,
 UInt16 *read_index
 )
{
	oam_bool_et kpi_found = OAM_FALSE;
	UInt16 index = OAM_ZERO;
	OAM_FUN_ENTRY_TRACE();
	oam_pm_pdcp_kpi_info_node_t *curr = pdcp_kpi_info.pm_pdcp_kpi_info_first_Node;
	if(OAM_NULL != curr )
	{
		for(index = OAM_ZERO; index < oam_prov_req.num_cells_configured;index++)
		{
			if((curr->oam_pm_pdcp_kpi_stats_node[index].cell_index == rrc_cell_id)
					&& (OAM_TRUE == curr->oam_pm_pdcp_kpi_stats_node[index].Is_ind_present))
			{
				OAM_LOG(OAM, OAM_DETAILED, "Entry Found for cell_index = %d in pdcp link list", rrc_cell_id);
				kpi_found = OAM_TRUE;
				oam_memcpy((void *)&(p_pdcp_node_ptr->oam_pm_pdcp_kpi_stats_node[index]), 
						(void *)&(curr->oam_pm_pdcp_kpi_stats_node[index]), 
						sizeof(oam_pm_pdcp_kpi_stats_node_t));
				p_pdcp_node_ptr->next_batch_node  = curr->next_batch_node;
				curr->oam_pm_pdcp_kpi_stats_node[index].Is_ind_present = OAM_FALSE;
				break;	
			}
		}
	}
	else
	{
		OAM_LOG(OAM, OAM_DETAILED, "No entry found in pdcp link list");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	if(OAM_FALSE == kpi_found )
	{
		OAM_LOG(OAM, OAM_INFO, "No Entry found for cell_index = %d in pdcp link list", rrc_cell_id);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	*read_index = index;
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/* SPR_11231_END */

/*spr 19296 fix start*/


/****************************************************************************
 * Function Name  : search_and_del_mac_node_for_cell_id 
 * Inputs         : p_mac_node_ptr: pointer to structure oam_pm_kpi_info_Node_t 
 : rrc_cell_id : Cell ID 
 * Outputs        : NONE
 * Returns        : NONE
 * Description    : this function search and delete mac node acc to cell id 
 ****************************************************************************/
UInt16
search_and_del_mac_node_for_cell_id
(
 oam_pm_mac_kpi_info_node_t *p_mac_node_ptr, 
 UInt8 rrc_cell_id,
 UInt16 *read_index
 )
{
	oam_bool_et kpi_found = OAM_FALSE;
	UInt16 index = OAM_ZERO;
	OAM_FUN_ENTRY_TRACE();
	oam_pm_mac_kpi_info_node_t *curr = mac_kpi_info.pm_mac_kpi_info_first_Node;
	if(OAM_NULL != curr )
	{
		for(index = OAM_ZERO; index < oam_prov_req.num_cells_configured;index++)
		{
			if((curr->oam_pm_mac_kpi_stats_node[index].cell_index == rrc_cell_id)
					&& (OAM_TRUE == curr->oam_pm_mac_kpi_stats_node[index].Is_ind_present))
			{
				OAM_LOG(OAM, OAM_DETAILED, "Entry Found for cell_index = %d in mac link list", rrc_cell_id);
				kpi_found = OAM_TRUE;
				oam_memcpy((void *)&(p_mac_node_ptr->oam_pm_mac_kpi_stats_node[index]), 
						(void *)&(curr->oam_pm_mac_kpi_stats_node[index]), 
						sizeof(oam_pm_mac_kpi_stats_node_t));
				/* SPR 22296 Fix Start */
				/* Code Removed */
				/* SPR 22296 Fix End */
				curr->oam_pm_mac_kpi_stats_node[index].Is_ind_present = OAM_FALSE;
				break;	
			}
		}
	}
	else
	{
		OAM_LOG(OAM, OAM_DETAILED, "No entry found in mac link list");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	if(OAM_FALSE == kpi_found )
	{
		OAM_LOG(OAM, OAM_INFO, "No Entry found for cell_index = %d in mac link list", rrc_cell_id);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	*read_index = index;
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/*spr 19296 fix end*/
/*spr 19296 fix start*/

/****************************************************************************
 * Function Name  : oam_pm_mac_read_linked_list_kpi_thp_file 
 * Description    :
 * Inputs         : 
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/

oam_return_et
oam_pm_mac_read_linked_list_kpi_thp_file
(
 /* SPR 22296 Fix Start */
 oam_mac_thp_batch_node *p_oam_mac_kpi_thp_stats_ind,
 /* SPR 22296 Fix End */
 UInt16 *mac_str_count 
 )
{
	OAM_FUN_ENTRY_TRACE();

	oam_counter_t counter   = OAM_ZERO;
	/* SPR_12419_START */
	oam_counter_t loop_counter   = OAM_ZERO;
	/* SPR 22296 Fix Start */
	oam_mac_thp_batch_node *p_temp = OAM_NULL;
    /* CID 118130 Fix Start */
	oam_error_code_et error_code = NO_ERROR;
    /* CID 118130 Fix End */
	/* SPR 22296 Fix End */
	UInt64 pm_dlIPThp[RRM_MAX_QCI] = {OAM_ZERO};
	UInt64 pm_ulIPThp[RRM_MAX_QCI] = {OAM_ZERO};
	/* SPR_12419_END */
	/*CSR 81339 start*/
	UInt32 totalThpTimeUl[RRM_MAX_QCI] = {OAM_ZERO};
	UInt32 totalThpTimeDl[RRM_MAX_QCI] = {OAM_ZERO};
	UInt32 totallatSample[RRM_MAX_QCI] = {OAM_ZERO};
	/*CSR 81339 end*/
	UInt32 pm_DlIpLatency[RRM_MAX_QCI] = {OAM_ZERO};
	/* SPR 22296 Fix Start */
	while (p_oam_mac_kpi_thp_stats_ind != OAM_NULL)
	{
		for(counter = OAM_ZERO; counter<MAX_KPI_INDEX; counter++)
		{
			if(OAM_ZERO < (p_oam_mac_kpi_thp_stats_ind->data.qci[counter]) && (p_oam_mac_kpi_thp_stats_ind->data.qci[counter] <= RRM_MAX_QCI))
			{
				/*CSR 81339 start*/
				OAM_LOG(OAM, OAM_DETAILED, "Received QCI[%d] entry valid from MAC ", p_oam_mac_kpi_thp_stats_ind->data.qci[counter]);
				/* SPR_12419_START */
                /* SPR 20961 Fix Start */
				if(INVALID_THP_STATS_ENTRY != p_oam_mac_kpi_thp_stats_ind->data.thpVolDl[counter])
                /* SPR 20961 Fix End */
				{
					pm_dlIPThp[p_oam_mac_kpi_thp_stats_ind->data.qci[counter] - 1] += p_oam_mac_kpi_thp_stats_ind->data.thpVolDl[counter];
					totalThpTimeDl[p_oam_mac_kpi_thp_stats_ind->data.qci[counter] - 1] += p_oam_mac_kpi_thp_stats_ind->data.thpTimeDl[counter];
				}
                /* SPR 20961 Fix Start */
				if(INVALID_THP_STATS_ENTRY != p_oam_mac_kpi_thp_stats_ind->data.thpVolUl[counter])
                /* SPR 20961 Fix End */
				{
					OAM_LOG(OAM, OAM_DETAILED, "pm_ulIPThp[p_oam_mac_kpi_thp_stats_ind->data.qci:%llu"
							"p_oam_mac_kpi_thp_stats_ind->data.thpVolUl[counter]:%llu for counter %d is:",
							pm_ulIPThp[p_oam_mac_kpi_thp_stats_ind->data.qci[counter] - 1],
							p_oam_mac_kpi_thp_stats_ind->data.thpVolUl[counter], counter);
					pm_ulIPThp[p_oam_mac_kpi_thp_stats_ind->data.qci[counter] - 1] += p_oam_mac_kpi_thp_stats_ind->data.thpVolUl[counter];
					totalThpTimeUl[p_oam_mac_kpi_thp_stats_ind->data.qci[counter] - 1] += p_oam_mac_kpi_thp_stats_ind->data.thpTimeUl[counter];
				}
				if(0xFFFFFFFF != p_oam_mac_kpi_thp_stats_ind->data.latTime[counter])
				{
					pm_DlIpLatency[p_oam_mac_kpi_thp_stats_ind->data.qci[counter] -1] += p_oam_mac_kpi_thp_stats_ind->data.latTime[counter];
					totallatSample[p_oam_mac_kpi_thp_stats_ind->data.qci[counter] - 1] += p_oam_mac_kpi_thp_stats_ind->data.latSample[counter];
				}
			}	
			else
			{
                OAM_LOG(OAM, OAM_INFO, "Invalid Entry for QCI[%d]"
                        " at index %d while reading mac kpi thp stats ",
						p_oam_mac_kpi_thp_stats_ind->data.qci[counter],
                        counter);
				break;
			}
		}
		p_temp = p_oam_mac_kpi_thp_stats_ind;
		p_oam_mac_kpi_thp_stats_ind = p_oam_mac_kpi_thp_stats_ind->next;
		if(OAM_NULL != p_temp)
		{
			oam_mem_free((void *)p_temp, &error_code);
		}
		/* SPR 22296 Fix End */
	}

	for(loop_counter = OAM_ZERO; ((loop_counter < RRM_MAX_QCI) && ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
	{
		/* DRB.IPThpDl.QCI */
		/*Klokwork Fix */ 
		/*SPR 11402 start*/ 
		if (totalThpTimeDl[loop_counter] != OAM_ZERO)
		{
			/* SPR 13790 FIX START */    
			oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],
					"%llu",
					((pm_dlIPThp[loop_counter] * OAM_EIGHT) / totalThpTimeDl[loop_counter]));
			/* SPR 13790 FIX END */    
		}
		else
		{
			oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++], "%u", OAM_ZERO);
		}
		/*SPR 11402 end*/
	}
	for(loop_counter = OAM_ZERO; ((loop_counter < RRM_MAX_QCI) && ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
	{
		/* DRB.IPThpUl.QCI */
		/*Klokwork Fix */ 
		/*SPR 11402 start*/
		if (totalThpTimeUl[loop_counter] != OAM_ZERO)
		{
			/* SPR 13790 FIX START */    
			oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],
					"%llu",
					((pm_ulIPThp[loop_counter] * OAM_EIGHT) / totalThpTimeUl[loop_counter]));
			/* SPR 13790 FIX END */   
		}
		else
		{
			oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++], "%u", OAM_ZERO);
		}
		/*SPR 11402 end*/
	}

	for(counter = OAM_ZERO; ((counter < RRM_MAX_QCI) && ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE));counter++)
	{   
		/* DRB.IpLateDL.QCI */
		/*SPR 11402 start*/
		if (totallatSample[counter] != OAM_ZERO)
		{
			oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],
					"%u",
					(pm_DlIpLatency[counter] / totallatSample[counter]));
		}
		else
		{
			oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++], "%u", OAM_ZERO);
		}
		/*SPR 11402 end*/
	}	



	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pm_mac_read_linked_list_kpi_file 
 * Description    : This function read pdcp list data and print into l2_kpi_log
 * Inputs         : p_oam_pdcp_kpi : pointer to structure oam_pdcp_KPI_stats_ind_t
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_pm_mac_read_linked_list_kpi_file
(
 oam_mac_KPI_stats_ind_t *p_oam_mac_kpi,
 /* SPR 22296 Fix Start */
 UInt16 *mac_str_count,  
 UInt8 rrc_cell_id
 /* SPR 22296 Fix End */
 )
{
	OAM_FUN_ENTRY_TRACE();
	OAM_NULL_CHECK(OAM_NULL != p_oam_mac_kpi);
	/* SPR-11133 FIX Start */
	UInt32 PRB_UL = OAM_ZERO;
	UInt32 PRB_DL = OAM_ZERO;
	float  dlfactor = OAM_ZERO;
	float  ulfactor = OAM_ZERO;
	/* SPR-11133 FIX End */
    /* SPR 23209 FIXED START */
    UInt16 loop_counter2 = OAM_ZERO;
    /* SPR 23209 FIXED END*/
	UInt32 total_active_UE_ul = OAM_ZERO;
	UInt32 total_active_UE_dl = OAM_ZERO;
	SInt8 cell_config_idx = OAM_ERROR_RETURN_VAL;
	UInt16 loop_counter = OAM_ZERO;
	UInt64 pdcpSduDelayDl_Sum = OAM_ZERO;/*SPR 19596, 19590*/
	UInt64 totalSessionTimePerQci_Sum = OAM_ZERO;   /* SPR 13492 Changes */
	UInt32 macSduAirLossRateDl_sum = OAM_ZERO;
	/* SPR 22296 Fix Start */
	/* Code Removed */
	/* SPR 22296 Fix End */
	/* SPR-15429 START */
	UInt32 totalNumberOfSamplingOccasions = OAM_ZERO;
    /* SPR 20171 Start, Number of UE are 256 */
	UInt16 avg_active_UE_dl = OAM_ZERO;
	UInt16 avg_active_UE_ul = OAM_ZERO;
    /*SPR 20171 End */
	/* SPR-15429 END */

	/* SPR-11133 FIX Start */
	/* SPR 22296 Fix Start */
	cell_config_idx = rrc_cell_id;
	/* SPR 22296 Fix End */
	if(OAM_ERROR_RETURN_VAL == cell_config_idx) {
		OAM_LOG(OAM, OAM_WARNING, "Invalid cell context[cell_config_idx=%d]", cell_config_idx);
		return OAM_FAILURE;
	}
	/* SPR_12259_FIX_START */
	oam_rrm_cell_config_req_t  *p_rrm_cell_config = &oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req[cell_config_idx]; 
	/* SPR_12259_FIX_END */

	PRB_UL =  p_rrm_cell_config->operator_info.
		admission_control_info.ul_prb_budget;
	PRB_DL =  p_rrm_cell_config->operator_info.
		admission_control_info.dl_prb_budget;

	dlfactor =  (oam_prov_req.prb_usage_factor.num_dl_schedulable / 10.0);
	ulfactor =  (oam_prov_req.prb_usage_factor.num_ul_schedulable / 10.0);
	/* SPR-11133 FIX End */
	/* SPR 18861 */
	OAM_LOG(OAM, OAM_DETAILED, "PRB_DL = %d PRB_UL %d OAMPer %d dlfac %f ulfac %f DurTTI %d nm_dl_sch %d num_ul_sch %d ", 
			PRB_DL, PRB_UL,OAM_PERCENTAGE_CONVERSION_VAL, dlfactor, ulfactor,
			p_oam_mac_kpi->DurationTTI,
			oam_prov_req.prb_usage_factor.num_dl_schedulable,
			oam_prov_req.prb_usage_factor.num_ul_schedulable 
	       );
	/* SPR 18861 */

	//RRU.PrbDl.QCI
	/* SPR 19259 Fix Start */
	if(*mac_str_count<PM_XML_MAX_PARAM_VALUE)
	{
	for(loop_counter = OAM_ZERO;((loop_counter < MAX_QCI) && ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
		/* SPR 19259 Fix End */
	{
			if(*mac_str_count == PM_XML_MAX_PARAM_VALUE)
			break;
		OAM_LOG(OAM, OAM_DETAILED, "p_oam_mac_kpi->DurationTTI = %d ",p_oam_mac_kpi->DurationTTI); 
		if (p_oam_mac_kpi->DurationTTI != OAM_ZERO)
		{
			/* SPR-11133 FIX Start */
			oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",
					(UInt8)(((p_oam_mac_kpi->totalPRBUsagePerQciDL[loop_counter]) * OAM_PERCENTAGE_CONVERSION_VAL) / (PRB_DL * dlfactor * p_oam_mac_kpi->DurationTTI)));
			/* SPR-11133 FIX End */
			/* SPR 18861 */
			OAM_LOG(OAM, OAM_DETAILED, "QCI = %d totalPRBUsagePerQciDL %llu PrbDL.Qci written = %d ", 
					loop_counter, p_oam_mac_kpi->totalPRBUsagePerQciDL[loop_counter],
					(UInt8)(((p_oam_mac_kpi->totalPRBUsagePerQciDL[loop_counter]) * OAM_PERCENTAGE_CONVERSION_VAL) / (PRB_DL * dlfactor * p_oam_mac_kpi->DurationTTI))
			       );
			/* SPR 18861 */
		}
		else
		{
			oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",OAM_ZERO);
		}
	}
	}
	//RRU.PrbUl.QCI
	/* SPR 19259 Fix Start */
	if(*mac_str_count<PM_XML_MAX_PARAM_VALUE)
	{
	for(loop_counter = OAM_ZERO;((loop_counter < MAX_QCI) && ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
		/* SPR 19259 Fix End */
	{
		if (p_oam_mac_kpi->DurationTTI != OAM_ZERO)
		{
			/* SPR-11133 FIX Start */
			oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",
					(UInt8)(((p_oam_mac_kpi->totalPRBUsagePerQciUL[loop_counter]) * OAM_PERCENTAGE_CONVERSION_VAL) / (PRB_UL * ulfactor *p_oam_mac_kpi->DurationTTI)));
			/* SPR-11133 FIX End */
			/* SPR 18861 */
			OAM_LOG(OAM, OAM_DETAILED, "QCI = %d totalPRBUsagePerQciUL %llu PrbUL.Qci written = %d ",
					loop_counter, p_oam_mac_kpi->totalPRBUsagePerQciUL[loop_counter],
					(UInt8)(((p_oam_mac_kpi->totalPRBUsagePerQciUL[loop_counter]) * OAM_PERCENTAGE_CONVERSION_VAL) / (PRB_UL * ulfactor *p_oam_mac_kpi->DurationTTI))
			       );
			/* SPR 18861 */
		}
		else
		{
			oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",OAM_ZERO);
			OAM_LOG(OAM, OAM_DETAILED, "totalPRBUsagePerQciUL[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
		}
	}
	}

	//RRU.PrbTotDl
	/* SPR 19259 Fix Start */
	if(*mac_str_count<PM_XML_MAX_PARAM_VALUE)
	{
		if (p_oam_mac_kpi->DurationTTI != OAM_ZERO) 
		/* SPR 19259 Fix End */
	{
		/* SPR-11133 FIX Start */
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",(UInt8)(((p_oam_mac_kpi->totalPRBUsageDL)* OAM_PERCENTAGE_CONVERSION_VAL)/((PRB_DL)* dlfactor *(p_oam_mac_kpi->DurationTTI))));
		/* SPR-11133 FIX End */
		/* SPR 18861 */
		OAM_LOG(OAM, OAM_DETAILED, "QCI = %d totalPRBUsageDL %llu .PrbTotDl written = %d ", 
				loop_counter, p_oam_mac_kpi->totalPRBUsageDL,
				(UInt8)(((p_oam_mac_kpi->totalPRBUsageDL)* OAM_PERCENTAGE_CONVERSION_VAL)/((PRB_DL)* dlfactor *(p_oam_mac_kpi->DurationTTI)))
		       );
		/* SPR 18861 */
    }
    /* SPR 19838 : CID 109077 Fix Start */
    else if((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
    {
        oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",OAM_ZERO);
        OAM_LOG(OAM, OAM_DETAILED, "totalPRBUsageDL[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
    }
    /* SPR 19838 : CID 109077 Fix End */
	//RRU.PrbTotUl
	/* SPR 19259 Fix Start */
	if(*mac_str_count<PM_XML_MAX_PARAM_VALUE)
	{
		if (p_oam_mac_kpi->DurationTTI != OAM_ZERO)
		/* SPR 19259 Fix End */
	{
		/* SPR-11133 FIX Start */
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",(UInt8)(((p_oam_mac_kpi->totalPRBUsageUL)* OAM_PERCENTAGE_CONVERSION_VAL)/((PRB_UL)* ulfactor * (p_oam_mac_kpi->DurationTTI))));
		/* SPR-11133 FIX End */
		/* SPR 18861 */
		OAM_LOG(OAM, OAM_DETAILED, "QCI = %d totalPRBUsageUL %llu .PrbTotUl written = %d ", 
				loop_counter, p_oam_mac_kpi->totalPRBUsageUL,
				(UInt8)(((p_oam_mac_kpi->totalPRBUsageUL)* OAM_PERCENTAGE_CONVERSION_VAL)/((PRB_UL)* ulfactor *(p_oam_mac_kpi->DurationTTI)))
		       );
		/* SPR 18861 */
	}
    /* SPR 19838 : CID 109077 Fix Start */
    else if((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",OAM_ZERO);
		OAM_LOG(OAM, OAM_DETAILED, "totalPRBUsageUL[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	}
	}
	/* loop for RRM_MAX_QCI */
	//DRB.UEActiveUl.QCI
	/* SPR-15429 START */
	totalNumberOfSamplingOccasions = (p_oam_mac_kpi->DurationTTI)/ACTIVE_USER_SAMPLING_TIMER_TTI;
	/* SPR 19259 Fix Start */
	if(*mac_str_count<PM_XML_MAX_PARAM_VALUE)
	{
	for(loop_counter = OAM_ZERO;((loop_counter < MAX_QCI) && ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
		/* SPR 19259 Fix End */
	{
		if (totalNumberOfSamplingOccasions != OAM_ZERO)
		{
			avg_active_UE_ul = (p_oam_mac_kpi->numberOfActiveUEPerQciUL[loop_counter])/totalNumberOfSamplingOccasions;
			oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",avg_active_UE_ul);
			total_active_UE_ul += avg_active_UE_ul;
            /*SPR 20171 Start */
			OAM_LOG(OAM, OAM_DETAILED, " p_oam_mac_kpi->numberOfActiveUEPerQciUL[loop_counter%d ]  = %llu  totalNumberOfSamplingOccasions =%d p_oam_mac_kpi->DurationTTI %d so avg_active_UE_ul =%d and total_active_UE_ul = %d  ",loop_counter, 
					p_oam_mac_kpi->numberOfActiveUEPerQciUL[loop_counter],totalNumberOfSamplingOccasions,p_oam_mac_kpi->DurationTTI,avg_active_UE_ul,total_active_UE_ul  );
            /*SPR 20171 End */
		}
		else
		{
			oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",OAM_ZERO);
			OAM_LOG(OAM, OAM_DETAILED, "numberOfActiveUEPerQciUL[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
		}
	}
	}

	//DRB.UEActiveDl.QCI
	/* SPR 19259 Fix Start */
	if(*mac_str_count<PM_XML_MAX_PARAM_VALUE)
	{
	for(loop_counter = OAM_ZERO;((loop_counter < MAX_QCI) && ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
		/* SPR 19259 Fix End */
	{
		if (totalNumberOfSamplingOccasions != OAM_ZERO)
		{
			avg_active_UE_dl = (p_oam_mac_kpi->numberOfActiveUEPerQciDL[loop_counter])/totalNumberOfSamplingOccasions; 
			oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",avg_active_UE_dl);
			total_active_UE_dl += avg_active_UE_dl;
            /*SPR 20171 Start*/
            OAM_LOG(OAM, OAM_DETAILED, " p_oam_mac_kpi->numberOfActiveUEPerQciDL[loop_counter%d ]  = %llu totalNumberOfSamplingOccasions =%d p_oam_mac_kpi->DurationTTI %d so avg_active_UE_dl =%d and  total_active_UE_dl = %d  ",loop_counter, 
                    p_oam_mac_kpi->numberOfActiveUEPerQciDL[loop_counter],totalNumberOfSamplingOccasions,p_oam_mac_kpi->DurationTTI,avg_active_UE_dl,total_active_UE_dl  );
            /*SPR 20171 Start*/

		}
		else
		{
			oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",OAM_ZERO);
			OAM_LOG(OAM, OAM_DETAILED, "numberOfActiveUEPerQciDL[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
		}
	}
	}
	/*spr 19583 fix start*/
	/*code deleted*/
	/*spr 19583 fix end*/
	//DRB.UEActiveUl.Sum
	/* SPR 19259 Fix Start */
	if ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		/*spr 19583 fix start*/
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",(UInt32)total_active_UE_ul);
		/*spr 19583 fix end*/
		OAM_LOG(OAM, OAM_DETAILED, "DRB.UEActiveUl.Sum[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	}
	/* SPR 19259 Fix End */
	if ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
	{	
		//DRB.UEActiveDl.Sum
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",(UInt32)total_active_UE_dl);
		OAM_LOG(OAM, OAM_DETAILED, "DRB.UEActiveDl.Sum[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	}
	/* SPR 19259 Fix End */

	/* SPR-15429 END */

	//RRU.RachPreambleDedMean
	/* SPR 19259 Fix Start */
	if ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		if (p_oam_mac_kpi->DurationTTI != OAM_ZERO) 
		/* SPR 19259 Fix End */
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%llu",(p_oam_mac_kpi->rcvdRandomAccessPreamblesContentionFree*TIME_UNIT_TO_MILLISEC) / p_oam_mac_kpi->DurationTTI);
		OAM_LOG(OAM, OAM_DETAILED, "RRU.RachPreambleDedMean[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	}
    /* SPR 19838 : CID 109077 Fix Start */
	else if((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",OAM_ZERO);
		OAM_LOG(OAM, OAM_DETAILED, "RRU.RachPreambleDedMean[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	}
	}
	//RRU.RachPreambleAMean
	/* SPR 19259 Fix Start */
	if ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		if (p_oam_mac_kpi->DurationTTI != OAM_ZERO)
		/* SPR 19259 Fix End */
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%llu",(p_oam_mac_kpi->rcvdRandomAccessPreamblesGroupA * TIME_UNIT_TO_MILLISEC)/ p_oam_mac_kpi->DurationTTI);
		OAM_LOG(OAM, OAM_DETAILED, "RRU.RachPreambleAMean[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	}
    /* SPR 19838 : CID 108805 Fix Start */
	else if((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",OAM_ZERO);
		OAM_LOG(OAM, OAM_DETAILED, "RRU.RachPreambleAMean[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	}
	}
	//RRU.RachPreambleBMean
	/* SPR 19259 Fix Start */
	if ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		if (p_oam_mac_kpi->DurationTTI != OAM_ZERO) 
		/* SPR 19259 Fix End */
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%llu",(p_oam_mac_kpi->rcvdRandomAccessPreamblesGroupB * TIME_UNIT_TO_MILLISEC)/ p_oam_mac_kpi->DurationTTI);
		OAM_LOG(OAM, OAM_DETAILED, "RRU.RachPreambleBMean[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	}
    /* SPR 19838 : CID 108805 Fix Start */
	else if((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",OAM_ZERO);
		OAM_LOG(OAM, OAM_DETAILED, "RRU.RachPreambleBMean[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	}
	}
	//DRB.PdcpSduDelayDl.QCI
	/* SPR 19259 Fix Start */
	if ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		for(loop_counter = OAM_ZERO;(loop_counter < MAX_QCI) ;loop_counter++)
		/* SPR 19259 Fix End */
	{
		if(OAM_ZERO == p_oam_mac_kpi->totalPktAckedInDLPerQci[loop_counter] )
		{
			oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%llu",
					p_oam_mac_kpi->totalPktDelayPerQciDL[loop_counter]);
            /*SPR 20171 Start */
            OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s No Packets Acked DRB.PdcpSduDelayDl.QCI[%d]",
                    (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1],(loop_counter + 1));
            /*SPR 20171 End */
       		pdcpSduDelayDl_Sum += p_oam_mac_kpi->totalPktDelayPerQciDL[loop_counter];			/*SPR 19596, 19590*/
		}
		else
		{
			oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%llu",(p_oam_mac_kpi->totalPktDelayPerQciDL[loop_counter])/(p_oam_mac_kpi->totalPktAckedInDLPerQci[loop_counter]));
			/*SPR 20171 Start */
            OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s  DRB.PdcpSduDelayDl.QCI[%d]", 
				(*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1],(loop_counter + 1));
            /*SPR 20171 End */
			pdcpSduDelayDl_Sum += ((p_oam_mac_kpi->totalPktDelayPerQciDL[loop_counter]/p_oam_mac_kpi->totalPktAckedInDLPerQci[loop_counter]));

		}
	}
	}
	if ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		/* SPR 19259 Fix Start */
		/* SPR 22296 Fix Start */
		if ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
				oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",pdcpSduDelayDl_Sum); /*SPR 19596, 19590*/
		OAM_LOG(OAM, OAM_DETAILED, "pdcpSduDelayDl_Sum[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);	/* coverity 109084 +-*/
		/* SPR 22296 Fix End */
	/* SPR 19259 Fix End */
	}

    /* SPR 23209 FIXED START */
    //RRU.OP.PrbDl.QCI
    if(*mac_str_count<PM_XML_MAX_PARAM_VALUE)
    {
        for(loop_counter2 = OAM_ZERO;loop_counter2 < MAX_MOCN_OPERATOR;loop_counter2++)
        {
            oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",
                    p_oam_mac_kpi->kpiPerOperator[loop_counter2].operatorId);

            OAM_LOG(OAM, OAM_DETAILED, "OperatorId[%d]", p_oam_mac_kpi->kpiPerOperator[loop_counter2].operatorId); 
            //RRU.PrbDl.QCI
            for(loop_counter = OAM_ZERO;((loop_counter < MAX_QCI) && ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
            {
                if (p_oam_mac_kpi->DurationTTI != OAM_ZERO)
                {
                    /* SPR-11133 FIX Start */
                    oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",
                            (UInt8)(((p_oam_mac_kpi->kpiPerOperator[loop_counter2].
                                        totalPRBUsagePerQciDL[loop_counter]) * OAM_PERCENTAGE_CONVERSION_VAL) / (PRB_DL * dlfactor * p_oam_mac_kpi->DurationTTI)));
                    OAM_LOG(OAM, OAM_DETAILED, "KPIPerOperator QCI = %d totalPRBUsagePerQciDL %llu PrbDL.Qci written = %d ", 
                            loop_counter, p_oam_mac_kpi->kpiPerOperator[loop_counter2].totalPRBUsagePerQciDL[loop_counter],
                            (UInt8)(((p_oam_mac_kpi->kpiPerOperator[loop_counter2].totalPRBUsagePerQciDL[loop_counter]) * OAM_PERCENTAGE_CONVERSION_VAL) / (PRB_DL * dlfactor * p_oam_mac_kpi->DurationTTI)));
                    /* SPR-11133 FIX End */
                }
                else
                {
                    oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",OAM_ZERO);
                }
            }
            //RRU.PrbUl.QCI
            for(loop_counter = OAM_ZERO;((loop_counter < MAX_QCI) && ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
            {
                if (p_oam_mac_kpi->DurationTTI != OAM_ZERO)
                {
                    /* SPR-11133 FIX Start */
                    oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",
                            (UInt8)(((p_oam_mac_kpi->kpiPerOperator[loop_counter2].
                                        totalPRBUsagePerQciUL[loop_counter]) * OAM_PERCENTAGE_CONVERSION_VAL) / (PRB_UL * ulfactor *p_oam_mac_kpi->DurationTTI)));
                    /* SPR-11133 FIX End */
                    OAM_LOG(OAM, OAM_DETAILED, "KPIPerOperator QCI = %d totalPRBUsagePerQciUL %llu PrbUL.Qci written = %d ", 
                            loop_counter, p_oam_mac_kpi->kpiPerOperator[loop_counter2].totalPRBUsagePerQciUL[loop_counter],
                            (UInt8)(((p_oam_mac_kpi->kpiPerOperator[loop_counter2].totalPRBUsagePerQciUL[loop_counter]) * OAM_PERCENTAGE_CONVERSION_VAL) / (PRB_UL * ulfactor * p_oam_mac_kpi->DurationTTI)));
                }
                else
                {
                    oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",OAM_ZERO);
                }
            }

            //DRB.UEActiveUl.QCI
            for(loop_counter = OAM_ZERO;((loop_counter < MAX_QCI) && ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
            {
                if (totalNumberOfSamplingOccasions != OAM_ZERO)
                {
                    avg_active_UE_ul = (p_oam_mac_kpi->kpiPerOperator[loop_counter2].
                            numberOfActiveUEPerQciUL[loop_counter])/totalNumberOfSamplingOccasions;
                    oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",avg_active_UE_ul);
                }
                else
                {
                    oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",OAM_ZERO);
                }
            }

            //DRB.UEActiveDl.QCI
            for(loop_counter = OAM_ZERO;((loop_counter < MAX_QCI) && ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
            {
                if (totalNumberOfSamplingOccasions != OAM_ZERO)
                {
                    avg_active_UE_dl = (p_oam_mac_kpi->kpiPerOperator[loop_counter2].
                            numberOfActiveUEPerQciDL[loop_counter])/totalNumberOfSamplingOccasions; 
                    oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",avg_active_UE_dl);
                }
                else
                {
                    oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",OAM_ZERO);
                }
            }
        }
    }
   
    /* SPR 23209 FIXED END */
    /* SPR 23499 Fix End */
	/* SPR 13492 Changes Start */
    /* SPR 19838 : CID 109077 Fix Start */
    if ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
    {
        /*SPR 20171 Start*/ 
        oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%llu",p_oam_mac_kpi->totalSessionTimeUE);
        /*SPR 20171 End*/ 
        OAM_LOG(OAM, OAM_DETAILED, "totalSessionTimeUE[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
    }
    /* SPR 19838 : CID 109077 Fix End */
	for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI;loop_counter++)
	{
        /* SPR 19838 : CID 109077 Fix Start */
        if ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
        {
            /*SPR 20171 Start*/ 
		    oam_sprintf((char
                        *)pm_xml_param_value[(*mac_str_count)++],"%llu",p_oam_mac_kpi->totalSessionTimePerQci[loop_counter]);
            /*SPR 20171 End*/ 
		    OAM_LOG(OAM, OAM_DETAILED, "totalSessionTimePerQci[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
		    totalSessionTimePerQci_Sum +=
			            p_oam_mac_kpi->totalSessionTimePerQci[loop_counter]; 
        }
        /* SPR 19838 : CID 109077 Fix End */
	}	
    /* SPR 19838 : CID 109077 Fix Start */
    if ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
    {
        /*SPR 20171 Start*/ 
        oam_sprintf((char *)
                pm_xml_param_value[(*mac_str_count)++],"%llu",totalSessionTimePerQci_Sum);
        /*SPR 20171 Start*/ 

        OAM_LOG(OAM, OAM_DETAILED, "totalSessionTimePerQci[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
    }
    /* SPR 19838 : CID 109077 Fix End */
	/* SPR 13492 Changes End */

	/* SPR 12670 Changes Start */

	}
	//DRB.PdcpSduAirLossRateDl
	if((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI ;loop_counter++)
	{
		if(OAM_ZERO == (p_oam_mac_kpi->totalSduAckRcvdDlperQci[loop_counter] + 
					p_oam_mac_kpi->totalDlDrbUuLossRatePerQci[loop_counter]))
		{ 
			/* SPR_12419_END */
			oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",OAM_ZERO );
			OAM_LOG(OAM, OAM_DETAILED, "totalSduAckRcvdDlperQci[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
		}
		else
		{

			/* SPR_12531_START */     
			if(p_oam_mac_kpi->totalSduAckRcvdDlperQci[loop_counter] + p_oam_mac_kpi->totalDlDrbUuLossRatePerQci[loop_counter])
			{        

				/* SPR 14004 Changes Start */
				/*fix_15482_start CID 81800*/
				oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",((UInt64)((UInt64)p_oam_mac_kpi->totalDlDrbUuLossRatePerQci[loop_counter] * PM_PDCP_KPI_MULTIPLIER)/
							/*SPR 11749 start*/
							(UInt64)(p_oam_mac_kpi->totalSduAckRcvdDlperQci[loop_counter] + 
								p_oam_mac_kpi->totalDlDrbUuLossRatePerQci[loop_counter]))); 
				OAM_LOG(OAM, OAM_DETAILED, "totalSduAckRcvdDlperQci[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
				/*fix_15482_end CID 81800*/
				/*SPR 11749 end*/
			}
			/* SPR_12419_START */
			macSduAirLossRateDl_sum += ((UInt64)((UInt64)(p_oam_mac_kpi->totalDlDrbUuLossRatePerQci[loop_counter]) * PM_PDCP_KPI_MULTIPLIER) /(UInt64)(p_oam_mac_kpi->totalDlDrbUuLossRatePerQci[loop_counter] + p_oam_mac_kpi->totalSduAckRcvdDlperQci[loop_counter])); 
			/* SPR_12419_END */
			/* SPR 14004 Changes End */
		}
	}
	}
	/* coverity 41027 - 1 */
	if((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%u",macSduAirLossRateDl_sum );
		OAM_LOG(OAM, OAM_DETAILED, "macSduAirLossRateDl_sum[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	}

	//CARR.WBCQIDist.Bin
	if((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
	for(loop_counter = OAM_ZERO; loop_counter < MAX_CQI_INDEX; ++loop_counter)
	{
        /* SPR 19838 : CID 109077 Fix Start */
        if ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE)
        {
            oam_sprintf((char *)
                    pm_xml_param_value[(*mac_str_count)++],"%u",
                    (p_oam_mac_kpi->totalCQICW1Reported[loop_counter] +
                     p_oam_mac_kpi->totalCQICW2Reported[loop_counter]));
            OAM_LOG(OAM, OAM_DETAILED, "totalCQICW1Reported[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
        }
        /* SPR 19838 : CID 109077 Fix End */
    	}
	}
	/* SPR 12670 Changes End */
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;

}

void
	oam_pm_mac_read_linked_list_kpi_thp_file_dummy
(
 UInt16 *mac_str_count
 )
{
	OAM_FUN_ENTRY_TRACE();

	oam_counter_t loop_counter   = OAM_ZERO;
	SInt16 dummy_val = M_OAM_ONE;

	for(loop_counter = OAM_ZERO; ((loop_counter < RRM_MAX_QCI) && ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
	}
	for(loop_counter = OAM_ZERO; ((loop_counter < RRM_MAX_QCI) && ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
	}
	for(loop_counter = OAM_ZERO; ((loop_counter < RRM_MAX_QCI) && ((*mac_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
	}

	OAM_FUN_EXIT_TRACE();
}


void
	oam_pm_mac_read_linked_list_kpi_file_dummy
(
 UInt16 *mac_str_count  
 )
{
	OAM_FUN_ENTRY_TRACE();
	UInt16 loop_counter = OAM_ZERO;
	SInt16 dummy_val = M_OAM_ONE;

	for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI;loop_counter++)
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
		OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	}
	for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI;loop_counter++)
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
		OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	}
	oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
	OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
	OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);

	for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI;loop_counter++)
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
		OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	}

	for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI;loop_counter++)
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
		OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	}
	oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
	OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
	OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);

	oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
	OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
	OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
	OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI;loop_counter++)
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
		OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	} 
	oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
	OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);

	for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI;loop_counter++)
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
		OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	} 
	oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
	OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);


	for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI;loop_counter++)
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
		OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	} 
	oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
	OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);


	for(loop_counter = OAM_ZERO;loop_counter < MAX_CQI_INDEX;loop_counter++)
	{
		oam_sprintf((char *)pm_xml_param_value[(*mac_str_count)++],"%d",dummy_val);
		OAM_LOG(OAM, OAM_DETAILED, "read pm_xml_param_value[%d] = %s", (*mac_str_count - 1), pm_xml_param_value[*mac_str_count - 1]);
	} 

	OAM_FUN_EXIT_TRACE();
}

/****************************************************************************
 * Function Name  : oam_pm_mac_read_data_from_link_list
 * Description    : Function read  mac link list node
 * Inputs         : p_kpi_info : pointer to structure oam_kpi_info_t
 * Outputs        : NONE 
 * Returns        : NONE 
 * ****************************************************************************/
void 
	oam_pm_mac_read_data_from_link_list
(
 oam_mac_kpi_info_t  *p_kpi_info,
 UInt8 rrc_cell_id,
 oam_counter_t loop_counter
 )
{
	/*spr 19296 fix start*/
	oam_error_code_et error_code = NO_ERROR;    
	UInt16 index = OAM_ZERO;
	oam_bool_et read_node_ongoing = OAM_FALSE;
	oam_pm_mac_kpi_info_node_t *current_node = OAM_NULL;
	oam_pm_mac_kpi_info_node_t *p_mac_node_ptr = OAM_NULL;
	UInt16 read_index = OAM_ZERO;
	OAM_ASSERT(OAM_NULL != p_kpi_info);
	/*spr 19296 fix end*/
	UInt16 pm_xml_mac_read_list_count = OAM_ZERO; 
	UInt16 mac_str_count = OAM_ZERO; 
	UInt16 ret = OAM_ZERO;
	OAM_FUN_ENTRY_TRACE();

	current_node = mac_kpi_info.pm_mac_kpi_info_first_Node;

	if (current_node != OAM_NULL)
	{
		if (OAM_FAILURE == oam_mem_alloc(sizeof(oam_pm_mac_kpi_info_node_t),
					(void *)&p_mac_node_ptr,&error_code))
		{
			OAM_LOG(OAM, OAM_ERROR,
					"Memory allocation to p_mac_node_ptr failed"
					" with Error Code:%d",
					error_code);
			OAM_FUN_EXIT_TRACE();

		}
		else
		{    
			/* SPR 18861 */
			mac_str_count = (g_pm_xml_total_element_count_offset* loop_counter) + g_pm_xml_mac_param_start;  
			/* SPR 18861 */
			ret = search_and_del_mac_node_for_cell_id(p_mac_node_ptr, rrc_cell_id, &read_index);
			if (ret == OAM_SUCCESS)
			{
				OAM_LOG(OAM, OAM_DETAILED, "node is being selected for cell_index = %d", rrc_cell_id);
				/* SPR 22296 Fix Start */
				oam_pm_mac_read_linked_list_kpi_file(
						&(p_mac_node_ptr->oam_pm_mac_kpi_stats_node[read_index].oam_mac_kpi_stats_ind), 
						&mac_str_count, rrc_cell_id);
				oam_pm_mac_read_linked_list_kpi_thp_file(
						p_mac_node_ptr->oam_pm_mac_kpi_stats_node[read_index].p_oam_mac_kpi_thp_stats_ind, 
						&mac_str_count);

				pm_xml_mac_read_list_count++;
				p_mac_node_ptr->oam_pm_mac_kpi_stats_node[read_index].p_oam_mac_kpi_thp_stats_ind = OAM_NULL;
				/* SPR 22296 Fix End */
			}
			else
			{
				OAM_LOG(OAM,OAM_WARNING,"Node with cell_index = %d not found in MAC LINK LIST", rrc_cell_id);
				oam_pm_mac_read_linked_list_kpi_file_dummy(&mac_str_count);
				oam_pm_mac_read_linked_list_kpi_thp_file_dummy(&mac_str_count);
				pm_xml_mac_read_list_count++;
				OAM_FUN_EXIT_TRACE();	
			}



			if(OAM_FAILURE == oam_mem_free((void *)p_mac_node_ptr,&error_code))
			{
				OAM_LOG(OAM,OAM_ERROR,"oam_mem_free failed ");
				OAM_FUN_EXIT_TRACE();
			}
			p_mac_node_ptr = OAM_NULL;
		}        
	}
	else
	{
        /*SPR 20960 Fix Start*/
		OAM_LOG(OAM,OAM_WARNING,"Node with cell_index = %d not found in MAC LINK LIST", rrc_cell_id);
        /* code removed */
        /*SPR 20960 Fix Start*/
		return;
	}

	for(index = OAM_ZERO; index < oam_prov_req.num_cells_configured; index++)
	{
		if(OAM_TRUE == current_node->oam_pm_mac_kpi_stats_node[index].Is_ind_present)
		{
			read_node_ongoing = OAM_TRUE;
		}
	}


	if(OAM_FALSE == read_node_ongoing)
	{
		OAM_LOG(OAM, OAM_DETAILED, "Read node of MAC Link list for cell_index = %d", rrc_cell_id );
		/* Now free the Node which have completed reading part */
		mac_kpi_info.pm_mac_kpi_info_first_Node = mac_kpi_info.pm_mac_kpi_info_first_Node->next_node_ptr;
		oam_mem_free((void *)current_node,&error_code);
		current_node = OAM_NULL;


	}
	OAM_FUN_EXIT_TRACE();
}

/*spr 19296 fix end*/
/****************************************************************************
 * Function Name  : oam_pm_pdcp_read_data_from_link_list 
 * Description    : Function read pdcp link list node
 * Inputs         : p_kpi_info : pointer to structure oam_kpi_info_t
 : rrc_cell_id : Cell ID
 * Outputs        : NONE 
 * Returns        : NONE 
 * ****************************************************************************/
void
	oam_pm_pdcp_read_data_from_link_list
(
 oam_pdcp_kpi_info_t  *p_kpi_info,
 UInt8 rrc_cell_id,
 oam_counter_t loop_counter
 )
{
	UInt16 pm_xml_pdcp_read_list_count = OAM_ZERO; 
	oam_error_code_et error_code = NO_ERROR;    
	UInt16 index = OAM_ZERO;
	oam_bool_et read_node_ongoing = OAM_FALSE;
	oam_pm_pdcp_kpi_info_node_t *current_node = pdcp_kpi_info.pm_pdcp_kpi_info_first_Node;
	/* Coverity Fix */
	oam_pm_pdcp_kpi_info_node_t *p_pdcp_node_ptr = OAM_NULL;
	OAM_ASSERT(OAM_NULL != p_kpi_info);
	UInt16 pdcp_str_count = OAM_ZERO; 
	UInt16 ret = OAM_ZERO;
	UInt16 read_index = OAM_ZERO;

	OAM_FUN_ENTRY_TRACE();
	if (current_node != OAM_NULL)
	{
		if (OAM_FAILURE == oam_mem_alloc(sizeof(oam_pm_pdcp_kpi_info_node_t),
					(void *)&p_pdcp_node_ptr,&error_code))
		{
			OAM_LOG(OAM, OAM_ERROR,
					"Memory allocation to p_pdcp_node_ptr failed"
					" with Error Code:%d",
					error_code);
			OAM_FUN_EXIT_TRACE();

		}
		else
		{    
			ret = search_and_del_pdcp_node_for_cell_id(p_pdcp_node_ptr, rrc_cell_id, &read_index);
			if (ret == OAM_SUCCESS)
			{
				OAM_LOG(OAM, OAM_DETAILED, "node is being selected for cell_index = %d", rrc_cell_id);
				/* SPR 18861 */
				pdcp_str_count = (g_pm_xml_total_element_count_offset* loop_counter) + g_pm_xml_pdcp_param_start ;
				/* SPR 18861 */
				oam_pm_pdcp_read_linked_list_kpi_file(
						&(p_pdcp_node_ptr->oam_pm_pdcp_kpi_stats_node[read_index].oam_pdcp_kpi_stats_ind), 
						&pdcp_str_count);
				/* SPR 22296 Fix Start */
				oam_pm_pdcp_read_linked_list_kpi_thp_file( 
						p_pdcp_node_ptr->oam_pm_pdcp_kpi_stats_node[read_index].p_oam_pdcp_kpi_thp_stats_ind, &pdcp_str_count); 
				pm_xml_pdcp_read_list_count++;
				p_pdcp_node_ptr->oam_pm_pdcp_kpi_stats_node[read_index].p_oam_pdcp_kpi_thp_stats_ind = OAM_NULL;
				/* SPR 22296 Fix End */
			}
			else
			{
				/* SPR 18861 */
				pdcp_str_count = (g_pm_xml_total_element_count_offset * loop_counter) + g_pm_xml_pdcp_param_start ;
				/* SPR 18861 */
				OAM_LOG(OAM,OAM_WARNING,"Node with cell_index = %d not found in PDCP LINK LIST", rrc_cell_id);
				oam_pm_pdcp_read_linked_list_kpi_file_dummy(&pdcp_str_count);
				oam_pm_pdcp_read_linked_list_kpi_thp_file_dummy(&pdcp_str_count);
				pm_xml_pdcp_read_list_count++;
				OAM_FUN_EXIT_TRACE();	

			}
			if(OAM_FAILURE == oam_mem_free((void *)p_pdcp_node_ptr,&error_code))
			{
				OAM_LOG(OAM,OAM_ERROR,"oam_mem_free failed ");
				OAM_FUN_EXIT_TRACE();
			}
			p_pdcp_node_ptr = OAM_NULL;
		}        
	}
	else
	{
        /*SPR 20960 Fix Start*/
		OAM_LOG(OAM,OAM_WARNING,"Node with cell_index = %d not found in PDCP LINK LIST", rrc_cell_id);
        /* code removed*/
        /*SPR 20960 Fix End*/
		return;
	}

	for(index = OAM_ZERO; index < oam_prov_req.num_cells_configured; index++)
	{
		if(OAM_TRUE == current_node->oam_pm_pdcp_kpi_stats_node[index].Is_ind_present)
		{
			read_node_ongoing = OAM_TRUE;
		}
	}


	if(OAM_FALSE == read_node_ongoing)
	{
		OAM_LOG(OAM, OAM_DETAILED, "Read node of PDCP Link list for cell_index = %d", rrc_cell_id );
		/* Now free the Node which have completed reading part */
		pdcp_kpi_info.pm_pdcp_kpi_info_first_Node = pdcp_kpi_info.pm_pdcp_kpi_info_first_Node->next_node_ptr;
		oam_mem_free((void *)current_node,&error_code);
		current_node = OAM_NULL;


	}
	OAM_FUN_EXIT_TRACE();
}



/****************************************************************************
 * Function Name  : oam_pm_read_data_from_link_list 
 * Description    : Function read data from  link lists maintained for all layers
 * Inputs         : NONE 
 * Outputs        : NONE 
 * Returns        : NONE 
 * ****************************************************************************/
void
oam_pm_read_data_from_link_list
(
 /* + SPR 17439 */
 void
 )
/* - SPR 17439 */
{
	oam_pm_rrc_kpi_info_Node_t *p_pm_kpi_info_Node = rrc_kpi_info.pm_kpi_info_first_Node;
	oam_counter_t loop_counter = OAM_ZERO;    
	oam_error_code_et error_code;
	/* SPR 17777 fix code removed */

	OAM_FUN_ENTRY_TRACE();
	/* SPR 13093 START */
	for(loop_counter = OAM_ZERO; loop_counter < oam_prov_req.num_cells_configured ;loop_counter++)
	{
		g_pm_xml_params.rrc_params.RRC_ConnMax[loop_counter] = OAM_ZERO;

	}
	/* SPR 13093 END */
	if (OAM_NULL != rrc_kpi_info.pm_kpi_info_first_Node)
	{
        /*SPR 20171 Start*/
        OAM_LOG(OAM, OAM_DETAILED, "%s Reading RRC KPI Link List Node ",__func__);
        /*SPR 20171 Start*/

		/* SPR 17777 fix */
		oam_pm_rrc_read_linked_list_kpi_file(p_pm_kpi_info_Node);
		if (rrc_kpi_info.pm_kpi_info_first_Node->next_node_ptr != OAM_NULL)
		{
			rrc_kpi_info.pm_kpi_info_first_Node = rrc_kpi_info.pm_kpi_info_first_Node->next_node_ptr;
			if(OAM_FAILURE == oam_mem_free((void *)p_pm_kpi_info_Node,&error_code))
			{
				OAM_LOG(OAM, OAM_ERROR, "Error in oam_mem_free"); 
			}
			p_pm_kpi_info_Node = OAM_NULL;
		}
		else
		{
			if(OAM_FAILURE == oam_mem_free((void *)p_pm_kpi_info_Node,&error_code))
			{
				OAM_LOG(OAM, OAM_ERROR, "Error in oam_mem_free"); 
			}
			p_pm_kpi_info_Node = OAM_NULL;
			rrc_kpi_info.pm_kpi_info_first_Node = OAM_NULL;
		}
	}
	else
	{ 
		OAM_LOG(OAM,OAM_DETAILED,"Data not found in rrc link list");
	}
	OAM_FUN_EXIT_TRACE();
}
/*spr 19296 fix start*/

/****************************************************************************
 * Function Name  : oam_pm_kpi_mac_info_strct_init
 * Description    : Function initialize p_kpi_info structure
 * Inputs         : NONE 
 * Outputs        : p_kpi_info: pointer to structure oam_pdcp_kpi_info_t 
 * Returns        : NONE 
 *****************************************************************************/
void 
	oam_pm_kpi_mac_info_strct_init
(
 oam_mac_kpi_info_t  *p_kpi_info
 )
{
	UInt8 index = OAM_ZERO;

	OAM_ASSERT(OAM_NULL != p_kpi_info);
	OAM_FUN_ENTRY_TRACE();

	if(OAM_NULL != p_kpi_info)
	{
		for(index = OAM_ZERO; index < OAM_MAC_KPI_MAX_CONFIG_TIME; index++)
			p_kpi_info->configuration_time[index] = OAM_ZERO;

		p_kpi_info->kpi_info_store_on_off_flag = OAM_ZERO;

		oam_pm_init_periodic_info(&(p_kpi_info->periodic_info));

		p_kpi_info->pm_mac_kpi_info_first_Node = OAM_NULL;
		p_kpi_info->ind_bit_mask = OAM_ZERO;
		p_kpi_info->direct_bit_mask = OAM_ZERO;
	}
	OAM_FUN_EXIT_TRACE();
}
/*spr 19296 fix end*/

/****************************************************************************
 * Function Name  : oam_pm_kpi_pdcp_info_strct_init
 * Description    : Function initialize p_kpi_info structure
 * Inputs         : NONE 
 * Outputs        : p_kpi_info: pointer to structure oam_pdcp_kpi_info_t 
 * Returns        : NONE 
 *****************************************************************************/
void 
	oam_pm_kpi_pdcp_info_strct_init
(
 oam_pdcp_kpi_info_t  *p_kpi_info
 )
{
	UInt8 index = OAM_ZERO;

	OAM_ASSERT(OAM_NULL != p_kpi_info);
	OAM_FUN_ENTRY_TRACE();

	if(OAM_NULL != p_kpi_info)
	{
		for(index = OAM_ZERO; index < OAM_PDCP_KPI_MAX_CONFIG_TIME; index++)
			p_kpi_info->configuration_time[index] = OAM_ZERO;

		p_kpi_info->kpi_info_store_on_off_flag = OAM_ZERO;

		oam_pm_init_periodic_info(&(p_kpi_info->periodic_info));

		p_kpi_info->pm_pdcp_kpi_info_first_Node = OAM_NULL;
		p_kpi_info->ind_bit_mask = OAM_ZERO;
		p_kpi_info->direct_bit_mask = OAM_ZERO;
	}
	OAM_FUN_EXIT_TRACE();
}

/****************************************************************************
 * Function Name  : oam_pm_kpi_rrc_info_strct_init
 * Description    : Function initialize p_kpi_info structure
 * Inputs         : NONE 
 * Outputs        : p_kpi_info: pointer to structure oam_rrc_kpi_info_t 
 * Returns        : NONE 
 *****************************************************************************/
void 
	oam_pm_kpi_rrc_info_strct_init
(
 oam_rrc_kpi_info_t  *p_kpi_info
 )
{
	UInt8 index;

	OAM_ASSERT(OAM_NULL != p_kpi_info);
	OAM_FUN_ENTRY_TRACE();

	if(OAM_NULL != p_kpi_info)
	{
		for(index = OAM_ZERO; index < OAM_RRC_KPI_MAX_CONFIG_TIME; index++)
			p_kpi_info->configuration_time[index] = OAM_ZERO;

		p_kpi_info->kpi_info_store_on_off_flag = OAM_ZERO;

		oam_pm_init_periodic_info(&(p_kpi_info->periodic_info));

		p_kpi_info->pm_kpi_info_first_Node = OAM_NULL;

		p_kpi_info->cell_index = OAM_ZERO;

		p_kpi_info->reset_stats = OAM_ZERO;

		p_kpi_info->rrc_sampling_start_flag = OAM_ZERO;

	}
	OAM_FUN_EXIT_TRACE();
}

/****************************************************************************
 * Function Name  : oam_pm_kpi_info_strct_init
 * Description    : Function initialize p_kpi_info structure
 * Inputs         : NONE 
 * Outputs        : p_kpi_info: pointer to structure oam_kpi_info_t 
 * Returns        : NONE 
 ****************************************************************************/
void 
	oam_pm_kpi_info_strct_init
(
 oam_kpi_info_t *p_kpi_info
 )
{
	UInt8 index;

	OAM_ASSERT(OAM_NULL != p_kpi_info);
	OAM_FUN_ENTRY_TRACE();

	if(OAM_NULL != p_kpi_info)
	{
		for(index = OAM_ZERO; index < OAM_KPI_MAX_CONFIG_TIME; index++)
			p_kpi_info->configuration_time[index] = OAM_ZERO;

		p_kpi_info->kpi_info_store_on_off_flag = OAM_ZERO;

		oam_pm_init_periodic_info(&(p_kpi_info->periodic_info));

		p_kpi_info->pm_kpi_info_first_Node = OAM_NULL;

		OAM_LOG(OAM,OAM_DETAILED,"kpi_info_store_on_off_flag=%u. "
                "pm_kpi_info_first_Node=%p", 
                p_kpi_info->kpi_info_store_on_off_flag,
                p_kpi_info->pm_kpi_info_first_Node);

	}
	OAM_FUN_EXIT_TRACE();

}
/****************************************************************************
 * Function Name  : oam_pm_init_periodic_info 
 * Description    : Function initialize p_periodic_info structure 
 * Inputs         : NONE
 * Outputs        : p_periodic_info : pointer to structure oam_periodic_info_t  
 * Returns        : NONE
 ****************************************************************************/
void
	oam_pm_init_periodic_info
(
 oam_periodic_info_t *p_periodic_info
 )
{

	OAM_ASSERT(OAM_NULL != p_periodic_info);
	OAM_FUN_ENTRY_TRACE();

	if(OAM_NULL != p_periodic_info)
	{
		p_periodic_info->sampling_intrvl = OAM_ZERO;
		p_periodic_info->upload_intrvl = OAM_ZERO;

		OAM_LOG(OAM,OAM_DETAILED,"sampling_intrvl= %d. "
                "upload_intrvl %d",
                p_periodic_info->sampling_intrvl,
                p_periodic_info->upload_intrvl);
	}
	OAM_FUN_EXIT_TRACE();
}

/****************************************************************************
 * Function Name  : oam_pm_pdcp_kpi_stat_ind 
 * Description    : function fill oam_pdcp_KPI_stats_ind_t structure from 
 *                : p_api_buf buf  
 * Inputs         : p_api_buf : buffer for msg storage
 * Outputs        : p_KPI_stats_ind : pointer to structure oam_pdcp_KPI_stats_ind_t
 * Returns        : OAM_SUCCESS 
 ****************************************************************************/
oam_return_et
	oam_pm_pdcp_kpi_stat_ind
(
 void* p_api_buf,
 oam_pdcp_KPI_stats_ind_t *p_KPI_stats_ind     
 )
{
	oam_counter_t loop_counter = OAM_ZERO;
	oam_pdcp_KPI_stats_ind_t *p_oam_pdcp_kpi_stats_ind;
	p_oam_pdcp_kpi_stats_ind = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();
	OAM_ASSERT(OAM_NULL != p_KPI_stats_ind);

	p_api_buf                    = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_oam_pdcp_kpi_stats_ind     = (oam_pdcp_KPI_stats_ind_t *)p_api_buf;

	/* To Create Current Time Stamp Value */
	create_time_stamp();
	OAM_LOG(OAM,OAM_INFO, "Received PDCP_KPI_STATS_IND from PDCP"
	        " with duration[%d].",
            " Current TimeStamp[%s]",
			p_oam_pdcp_kpi_stats_ind->durationTTI/OAM_MSEC_IN_ONE_SEC
            , time_buffer);

	if(OAM_NULL != p_KPI_stats_ind)
	{
		/*SPR 10566 FIX*/
#ifndef OAM_UT_FRAMEWORK
		qvLock(pm_mutex_lock);
#endif
		/*SPR 10566 FIX*/
		/*SPR_17954_Fix_Start*/
		oam_memset(p_KPI_stats_ind,OAM_ZERO,sizeof(oam_pdcp_KPI_stats_ind_t));
		/*SPR_17954_Fix_End*/
		p_KPI_stats_ind->durationTTI = p_oam_pdcp_kpi_stats_ind->durationTTI;

		for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI;loop_counter++)
		{
			if(p_oam_pdcp_kpi_stats_ind->totalSduRcvdDlperQci[loop_counter] != OAM_ZERO)
			{
				OAM_LOG(OAM,OAM_DETAILED, "For QCI [%d]", loop_counter+OAM_ONE);

				OAM_LOG(OAM,OAM_DETAILED, "For QCI [%d] totalDlDrbDiscardRatePerQci[%u]", 
						loop_counter+OAM_ONE, 
						p_oam_pdcp_kpi_stats_ind->totalDlDrbDiscardRatePerQci[loop_counter]);
				p_KPI_stats_ind->totalDlDrbDiscardRatePerQci[loop_counter] = p_oam_pdcp_kpi_stats_ind->totalDlDrbDiscardRatePerQci[loop_counter];
				/*spr 19296 fix start*/
				/*code deleted*/
				/*spr 19296 fix end*/

				OAM_LOG(OAM,OAM_DETAILED, "For QCI [%d] totalUlLossRatePerQci[%u]", 
						loop_counter+OAM_ONE, 
						p_oam_pdcp_kpi_stats_ind->totalUlLossRatePerQci[loop_counter]);
				p_KPI_stats_ind->totalUlLossRatePerQci[loop_counter] = p_oam_pdcp_kpi_stats_ind->totalUlLossRatePerQci[loop_counter];
				OAM_LOG(OAM,OAM_DETAILED, "For QCI [%d] totalSduRcvdDlperQci[%u]", 
						loop_counter+OAM_ONE, 
						p_oam_pdcp_kpi_stats_ind->totalSduRcvdDlperQci[loop_counter]);
				p_KPI_stats_ind->totalSduRcvdDlperQci[loop_counter] = p_oam_pdcp_kpi_stats_ind->totalSduRcvdDlperQci[loop_counter];
				/*spr 19296 fix start*/
				/*code deleted*/
				/*spr 19296 fix end*/

				OAM_LOG(OAM,OAM_DETAILED, "For QCI [%d] totalSduRcvdUlperQci[%u]", 
						loop_counter+OAM_ONE, 
						p_oam_pdcp_kpi_stats_ind->totalSduRcvdUlperQci[loop_counter]);
				p_KPI_stats_ind->totalSduRcvdUlperQci[loop_counter] = p_oam_pdcp_kpi_stats_ind->totalSduRcvdUlperQci[loop_counter];
			}
		}
		OAM_LOG(OAM,OAM_DETAILED, "dlCtrlPdcpSduBitRate[%u]", 
				p_oam_pdcp_kpi_stats_ind->dlCtrlPdcpSduBitRate);
		p_KPI_stats_ind->dlCtrlPdcpSduBitRate = p_oam_pdcp_kpi_stats_ind->dlCtrlPdcpSduBitRate;
		OAM_LOG(OAM,OAM_DETAILED, "ulCtrlPdcpSduBitRate[%u]", 
				p_oam_pdcp_kpi_stats_ind->ulCtrlPdcpSduBitRate);
		p_KPI_stats_ind->ulCtrlPdcpSduBitRate = p_oam_pdcp_kpi_stats_ind->ulCtrlPdcpSduBitRate;
		/*SPR 10566 FIX*/
#ifndef OAM_UT_FRAMEWORK
		qvUnlock(pm_mutex_lock);
#endif
		/*SPR 10566 FIX*/
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/*SPR 10023 FIX start*/
/****************************************************************************
 * Function Name  : insert_batch_data_in_kpi_node
 * Description    : function inserts all batches of PDCP THP KPI data into the
 *                  pdcp throuput KPI node
 * Inputs         : p_KPI_THP_stats_ind : Pointer to oam_pdcp_thp_batch_node
 *                  data
 * Outputs        : p_KPI_THP_stats_ind :
 * Returns        : OAM_SUCCESS / OAM_FAILURE
 ****************************************************************************/
oam_pdcp_thp_batch_node *
	insert_batch_data_in_kpi_node
(
 oam_pdcp_thp_batch_node **p_KPI_THP_stats_ind,
 oam_error_code_et *p_error_code
 )
{
	oam_pdcp_thp_batch_node *temp_ptr = OAM_NULL;
	oam_pdcp_thp_batch_node *curr_ptr= OAM_NULL;
	OAM_FUN_ENTRY_TRACE();

	if (OAM_FAILURE == oam_mem_alloc(sizeof(oam_pdcp_thp_batch_node),(void *)&temp_ptr, p_error_code))
	{
            OAM_LOG(OAM, OAM_ERROR,
                    "Memory allocation to temp_ptr failed"
                    " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_NULL;
	}
	temp_ptr->next = OAM_NULL;

	if (*p_KPI_THP_stats_ind == OAM_NULL)
	{
		*p_KPI_THP_stats_ind = temp_ptr;
		OAM_FUN_EXIT_TRACE();
		return *p_KPI_THP_stats_ind;
	}
	else
	{
		curr_ptr = *p_KPI_THP_stats_ind;
		while (curr_ptr->next != OAM_NULL)
		{
			curr_ptr = curr_ptr->next;
		}
		curr_ptr->next = temp_ptr;
		OAM_FUN_EXIT_TRACE();
		/* return curr_ptr; */
		return temp_ptr;
	}
}

/*SPR 10023 FIX end*/
/****************************************************************************
 * Function Name  : oam_add_pdcp_kpi_thp_ind 
 * Description    : function fill p_KPI_THP_stats_ind structure from 
 *                : p_api_buf buffer  
 * Inputs         : p_api_buf : buffer for msg storage 
 * Outputs        : p_KPI_THP_stats_ind :
 * Returns        : OAM_SUCCESS/OAM_FAILURE 
 ****************************************************************************/
/* SPR 17777 fix */
oam_return_et 
	oam_add_pdcp_kpi_thp_ind
(
 /* SPR 22296 Fix Start */
 oam_pdcp_thp_batch_node **p_pdcp_kpi_thp_stats_ind,
 UInt8 cell_idx
 /* SPR 22296 Fix End */
 )
{

    OAM_FUN_ENTRY_TRACE();
    /* SPR 22296 Fix Start */
    oam_pdcp_thp_batch_node *p_temp = OAM_NULL;
    oam_pdcp_thp_batch_node *p_last_saved_node = OAM_NULL;
    oam_pdcp_thp_batch_node *p_cur_lst_node = OAM_NULL;
    qvLock(pm_mutex_lock);

    p_temp = ConsolidatedPdcpThpStatsInd;

    while (p_temp != OAM_NULL)
    {
        if(p_temp->cell_idx == cell_idx)
        {
            if(*p_pdcp_kpi_thp_stats_ind == OAM_NULL)
            {
                *p_pdcp_kpi_thp_stats_ind = p_temp;
                p_last_saved_node = p_temp;
            }
            else
            {
                /* CID 118127 Fix Start */
                if(OAM_NULL != p_last_saved_node)
                { 
                    p_last_saved_node->next = p_temp;
                    p_last_saved_node = p_temp;
                }
                /* CID 118127 Fix End */
            }
            if (p_temp == ConsolidatedPdcpThpStatsInd)
            {
                ConsolidatedPdcpThpStatsInd = p_temp->next;
            }
            if (OAM_NULL != p_cur_lst_node)
            {
                p_cur_lst_node->next = p_temp->next;
            }
        }
        else
        {
            p_cur_lst_node = p_temp;
        }
        p_temp = p_temp->next;
    }
    if (OAM_NULL != p_last_saved_node)
    {
        p_last_saved_node->next = OAM_NULL;
    }
    /* SPR 22296 Fix End */
    qvUnlock(pm_mutex_lock);
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pm_rrm_kpi_stat_ind 
 * Description    : function fill oam_rrm_kpi_ind_t structure from 
 *                : p_api_buf buf  
 * Inputs         : p_api_buf : buffer for msg storage
 * Outputs        : p_KPI_stats_ind : pointer to structure oam_rrm_kpi_ind_t
 * Returns        : OAM_SUCCESS 
 ****************************************************************************/
oam_return_et
	oam_pm_rrm_kpi_stat_ind
(
 void* p_api_buf,
 oam_rrm_kpi_ind_t *p_KPI_stats_ind     
 )
{
	/* Coverity CID: 55206 fix */
	UInt16 msg_len = OAM_ZERO;
	msg_len = get_word_from_header((U8*)(p_api_buf) + 8);
	UInt8 *p_tmp_buf = (UInt8*)p_api_buf;
	SInt32 length = OAM_ZERO;
	oam_return_et retVal = OAM_FAILURE;
	OAM_FUN_ENTRY_TRACE();
	OAM_NULL_CHECK(OAM_NULL != p_KPI_stats_ind)

		p_tmp_buf   =  p_tmp_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/*SPR 10566 FIX*/
#ifndef OAM_UT_FRAMEWORK
	qvLock(pm_mutex_lock);
#endif
	/*SPR 10566 FIX*/

	retVal = oam_parse_rrm_oam_kpi_ind(p_KPI_stats_ind,
			p_tmp_buf,
			msg_len - LOWER_LAYER_INTERFACE_HEADER_SIZE,
			&length);

	/*SPR 10566 FIX*/
#ifndef OAM_UT_FRAMEWORK
	qvUnlock(pm_mutex_lock);
#endif
	/*SPR 10566 FIX*/


	OAM_LOG(OAM, OAM_INFO, "Received RRM_OAM_KPI_IND stats from RRM");

	OAM_FUN_EXIT_TRACE();
	return retVal;
}
/****************************************************************************
 * Function Name  : oam_pm_pdcp_init_kpi_name
 * Description    : Function initlize g_pdcp_kpi_stats_ind_params_name structure
 *                : from kpi name 
 * Inputs         : none 
 * Outputs        : none
 * Returns        : none 
 ****************************************************************************/
void 
oam_pm_pdcp_init_kpi_name
(
 /* + SPR 17439 */
 void
 )
/* - SPR 17439 */
{

	oam_counter_t loop_counter = OAM_ZERO; 

	UInt8 bitmask = OAM_ZERO;
	bitmask = (g_pdcp_kpi_stats_ind_params_name.bitmask[OAM_ZERO]);

	for(loop_counter=OAM_ZERO;loop_counter<MAX_PDCP_KPI_ELEMENT;loop_counter++)
		oam_set_bit_in_var(&bitmask, loop_counter);

	g_pdcp_kpi_stats_ind_params_name.bitmask[OAM_ZERO] = bitmask;

}
/****************************************************************************
 * Function Name  : oam_pm_rrc_init_kpi_thp_name 
 * Description    : Function initlize rrc_get_cell_stats_resp_name structure
 *                : from stats name 
 * Inputs         : none
 * Outputs        : none
 * Returns        : none
 ****************************************************************************/
void 
oam_pm_rrc_init_kpi_thp_name
(
 /* + SPR 17439 */
 void
 )
/* - SPR 17439 */
{
	oam_counter_t loop_counter = OAM_ZERO; 

	UInt8 bitmask = OAM_ZERO;
	bitmask = rrc_get_cell_stats_resp_name.bitmask[OAM_ZERO]; 

	for(loop_counter=OAM_ZERO;loop_counter<MAX_RRC_KPI_ELEMENT;loop_counter++)
		oam_set_bit_in_var(&bitmask,loop_counter);
	rrc_get_cell_stats_resp_name.bitmask[OAM_ZERO] = bitmask;
}
/****************************************************************************
 * Function Name  : oam_set_bit_in_array
 * Description    : This function set bit in an array
 * Inputs         : it takes array and bit no as input.
 * Outputs        : none
 * Returns        : none
 ****************************************************************************/
	oam_return_et oam_set_bit_in_array
(
 UInt8	 *p_bit_array,
 UInt16   bit_num
 )
{

	OAM_FUN_ENTRY_TRACE();
	OAM_NULL_CHECK(OAM_NULL != p_bit_array);

	UInt16 search_byte = OAM_ZERO;
	UInt16 search_bit = OAM_ZERO;
	UInt8 mask  = OAM_ZERO;
	UInt16 bit_shift = OAM_ONE;

	search_byte = bit_num / OAM_NO_OF_BITS;
	search_bit = bit_num % OAM_NO_OF_BITS;
	mask = bit_shift << (7-search_bit);
	OAM_LOG(OAM, OAM_DETAILED, "value of search_byte %d."
	        " value of search_bit %d."
            " value of mask %d.",
            search_byte, search_bit, mask);
	if(OAM_NULL != p_bit_array)
	{
		p_bit_array[search_byte] = p_bit_array[search_byte]|mask;
		OAM_LOG(OAM, OAM_DETAILED, "value of array[search_byte] %d",p_bit_array[search_byte]);
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;

}

/****************************************************************************
 * Function Name  : oam_set_bit_in_var
 * Description    : This function set bit in an var
 * Inputs         : it takes var and bit no as input.
 * Outputs        : none
 * Returns        : none
 ****************************************************************************/
	oam_return_et oam_set_bit_in_var
(
 UInt8   *p_bit_var,
 UInt16   bit_num
 )
{
	OAM_FUN_ENTRY_TRACE();
	OAM_NULL_CHECK(OAM_NULL != p_bit_var);

	UInt16 search_bit = OAM_ZERO;
	UInt8 mask  = OAM_ZERO;
	UInt16 bit_shift = OAM_ONE;

	search_bit = bit_num % OAM_NO_OF_BITS;
	mask = bit_shift << (7-search_bit);
	OAM_LOG(OAM, OAM_DETAILED, "value of search_bit %d."
	    " value of mask %d",
        search_bit, mask);
	if(OAM_NULL != p_bit_var)
	{
		*p_bit_var = *p_bit_var|mask;
		OAM_LOG(OAM, OAM_DETAILED, "value of array %d",*p_bit_var);
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_handle_rrc_stats_recv_resp 
 * Description    : Function handle rrc stats recived response 
 * Inputs         : p_api_buf : buffer for msg storage
 * Outputs        : NONE  
 * Returns        : OAM_SUCCESS
 ****************************************************************************/
oam_return_et
	oam_handle_rrc_stats_recv_resp
(
 void     *p_api_buf,
 UInt16   trans_id,
 UInt16   dest_id
 )
{
	oam_counter_t loop_counter   = OAM_ZERO; 
	UInt8 **temp_arr;     /* this is the array name */
	UInt32 size_x; /* this variable will be used for the first  dimension */
	UInt16 size_y; /* this variable will be used for the second dimension */
	/* suppose we want an array of char: a[4][100] */
	size_x = OAM_TWO;
	size_y = OAM_HUNDRED;
	/*PM_BUG*/
	/*PM_BUG*/
	UInt32 str_count = OAM_ZERO;
	oam_rrc_get_cell_stats_resp_t *oam_rrc_get_cell_stats_resp = OAM_NULL;
	SInt32 length = OAM_ZERO;
	OAM_NULL_CHECK(OAM_NULL != p_api_buf);/* Coverity FIX 33647 */
	OAM_FUN_ENTRY_TRACE();

	oam_error_code_et p_error_code;
	UInt32 data_len = OAM_ZERO;
	data_len = get_word_from_header((UInt8*)(p_api_buf) + OAM_INTF_HDR_MSG_SIZE_OFFSET);     
	/* SPR 17883 fix start */
	UInt16 transaction_id = OAM_ZERO;
	transaction_id = get_word_from_header((UInt8*)(p_api_buf) + OAM_TRANSACTION_ID);
	/* SPR 17883 fix end */

	p_api_buf = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE; 
	if (OAM_FAILURE == oam_mem_alloc(sizeof(oam_rrc_get_cell_stats_resp_t),
				(void *)&oam_rrc_get_cell_stats_resp,&p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to oam_rrc_get_cell_stats_resp failed"
				" with Error Code:%d",
				p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;

	}


	if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_get_cell_stats_resp(oam_rrc_get_cell_stats_resp,
				(UInt8*)p_api_buf,(data_len - LOWER_LAYER_INTERFACE_HEADER_SIZE),
				&length))
	{
		OAM_LOG(OAM,OAM_INFO,
				"RRC_OAM_GET_CELL_STATS_RESP Parsing Failure");
		/* Coverity CID 85267 Fix Start */
		oam_mem_free(oam_rrc_get_cell_stats_resp, &p_error_code);
		/* Coverity CID 85267 Fix End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	size_x = cal_len_non_periodic_cell_stats_resp();


	/*  allocate storage for an array of pointer to char */
	if (OAM_FAILURE == oam_mem_alloc((size_x * sizeof(UInt8 *)),(void *)&temp_arr,&p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to temp_arr failed"
				" with Error Code:%d",
				p_error_code);
		/* Coverity CID 85267 Fix Start */
		oam_mem_free(oam_rrc_get_cell_stats_resp,&p_error_code);
		/* Coverity CID 85267 Fix End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;

	}

	/* for each pocharer, allocate storage for an array of chars */
	for (loop_counter = OAM_ZERO; loop_counter < size_x; loop_counter++)
	{
		if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)&temp_arr[loop_counter],&p_error_code))
		{
            OAM_LOG(OAM, OAM_ERROR,
                    "Memory allocation to temp_arr failed"
                    " with Error Code:%d",
					p_error_code);
			/*SPR 13857 START*/
			OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, loop_counter, p_error_code);
			/*SPR 13857 END*/
			/* Coverity ID : 72253 Fix Start*/
			oam_mem_free(oam_rrc_get_cell_stats_resp,&p_error_code);
			/* Coverity ID : 72253 Fix End*/
			OAM_FUN_EXIT_TRACE();
			return OAM_FAILURE;
		}
	}
	/* SPR 17777 fix */
	oam_rrc_fill_non_periodic_get_cell_stats_resp
		(
		 oam_rrc_get_cell_stats_resp,
		 &str_count
		);

	str_count = 0;
	oam_sprintf((char *)temp_arr[str_count++],"%s","RRC KPI report being too large has been dumped into file l3_kpi.log at location LTE_IPR/bin.");
	if(cli_flag == OAM_TRUE)
	{
		cli_flag = OAM_FALSE;
		/* SPR 17883 fix start */
		/* SPR_12955_FIXES */
		oam_construct_n_send_gpv_response((UInt8 *)"L3 RRC KPI",temp_arr,str_count,
				transaction_id,&p_error_code,dest_id, OAM_ZERO, OAM_ZERO);
		/* SPR_12955_FIXES */
		/* SPR 17883 fix end */

	}	
	/*SPR 13857 START*/
	OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, size_x, p_error_code);
	/*SPR 13857 START*/
	/* Coverity ID : 72253 Fix Start*/
	oam_mem_free(oam_rrc_get_cell_stats_resp,&p_error_code);
	/* Coverity ID : 72253 Fix End*/
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}




/****************************************************************************
 * Function Name  : oam_pm_update_oam_periodic_info_t
 * Description    : Function update oam_periodic_info_t structure
 * Inputs         : sampling_intrvl_val: sampling interval value
 *                : upload_intrvl_val : upload interval value
 * Outputs        : p_periodic_info : pointer to structure oam_periodic_info_t for periodic information  
 * Returns        : OAM_SUCCESS
 ****************************************************************************/
oam_return_et
	oam_pm_update_oam_periodic_info_t
(
 oam_periodic_info_t *p_periodic_info,
 UInt16 sampling_intrvl_val, 
 UInt16 upload_intrvl_val 
 )
{
	OAM_FUN_ENTRY_TRACE();
	p_periodic_info->upload_intrvl = upload_intrvl_val;     
	p_periodic_info->sampling_intrvl = sampling_intrvl_val;     

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pm_update_mac_kpi_info 
 * Description    : Function updates pdcp_kpi_info structure
 * Inputs         : kpi_on_off_flag: Flag for KPI ON/OFF condition
 *                : sampling_intrvl_val: sampling interval value
 *                : upload_intrvl_val :upload interval value
 *                : bit_mask : UInt16 type bitmask
 * Outputs        : NONE 
 * Returns        : OAM_SUCCESS 
 ****************************************************************************/

oam_return_et
	oam_pm_update_mac_kpi_info
(
 UInt64 kpi_on_off_flag,
 UInt16 sampling_intrvl_val, 
 UInt16 upload_intrvl_val,   
 UInt16 bit_mask 
 )
{
	OAM_FUN_ENTRY_TRACE();

	g_mac_kpi_stats_ind_params_name.bitmask = bit_mask;/* SPR 17949 */

	OAM_LOG(OAM,OAM_DETAILED,"g_mac_kpi_stats_ind_params_name.bitmask =  0x%x,bit_mask 0x%x ",
			g_mac_kpi_stats_ind_params_name.bitmask,bit_mask);    
	oam_pm_update_oam_periodic_info_t
		(
		 &(mac_kpi_info.periodic_info),
		 sampling_intrvl_val, 
		 upload_intrvl_val
		);
	mac_kpi_info.kpi_info_store_on_off_flag = kpi_on_off_flag;
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pm_update_pdcp_kpi_info 
 * Description    : Function updates pdcp_kpi_info structure 
 * Inputs         : kpi_on_off_flag: Flag for KPI ON/OFF condition
 *                : sampling_intrvl_val: sampling interval value
 *                : upload_intrvl_val :upload interval value
 *                : bit_mask : UInt8 type bitmask
 * Outputs        : NONE 
 * Returns        : OAM_SUCCESS 
 ****************************************************************************/
oam_return_et
	oam_pm_update_pdcp_kpi_info
(
 UInt64 kpi_on_off_flag,
 UInt16 sampling_intrvl_val, 
 UInt16 upload_intrvl_val,   
 UInt8 bit_mask     
 )
{
	UInt8 bit_value = OAM_ONE;
	UInt16 bit_position = OAM_ZERO;     

	oam_counter_t loop_counter_1   = OAM_ZERO; 

	g_pdcp_kpi_stats_ind_params_name.bitmask[OAM_ZERO] = OAM_ZERO;

	OAM_FUN_ENTRY_TRACE();

	for(loop_counter_1=OAM_ZERO;loop_counter_1<5;loop_counter_1++)
	{
		if((bit_mask)&(bit_value))
		{
			oam_set_bit_in_array_lsb(g_pdcp_kpi_stats_ind_params_name.bitmask ,bit_position,1); 
		} 
		bit_value = bit_value<<OAM_ONE;
		bit_position++;
	}
	oam_pm_update_oam_periodic_info_t
		(
		 &(pdcp_kpi_info.periodic_info),
		 sampling_intrvl_val, 
		 upload_intrvl_val
		); 
	pdcp_kpi_info.kpi_info_store_on_off_flag = !kpi_on_off_flag;

	g_pdcp_kpi_ind_handler.kpi_thp_flag = OAM_FALSE;	
	if( ( (g_pdcp_kpi_stats_ind_params_name.bitmask[OAM_ZERO]) & 0x01) ||
			((g_pdcp_kpi_stats_ind_params_name.bitmask[OAM_ZERO]) & 0x02) ||
			((g_pdcp_kpi_stats_ind_params_name.bitmask[OAM_ZERO]) & 0x04))
	{
		pdcp_kpi_info.direct_bit_mask = 0x01;
	} 
	if(((g_pdcp_kpi_stats_ind_params_name.bitmask[OAM_ZERO]) & 0x08)  ||
			((g_pdcp_kpi_stats_ind_params_name.bitmask[OAM_ZERO]) & 0x10))
	{
		pdcp_kpi_info.direct_bit_mask = pdcp_kpi_info.direct_bit_mask | 0x02;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_pm_update_rrc_kpi_info 
 * Description    : Function updates rrc_kpi_info 
 * Inputs         : p_oam_show_rrc_cell_stats : Pointer to a structure oam_pm_show_rrc_cell_stats_t 
 *                : kpi_on_off_flag : Flag for KPI ON/OFF condition
 *                : sampling_intrvl_val: sampling interval value
 *                : upload_intrvl_val :
 * Outputs        : NONE 
 * Returns        : OAM_SUCCESS
 ****************************************************************************/

oam_return_et
	oam_pm_update_rrc_kpi_info
(
 oam_rrc_configure_kpi_stats_req_params_t *p_rrc_config_kpi_stats_req,  
 UInt8 kpi_on_off_flag,
 UInt16 sampling_intrvl_val, 
 UInt16 upload_intrvl_val 
 )
{

	UInt8 bit_value = OAM_ONE;
	UInt16 bit_position = OAM_ZERO;     
	oam_counter_t loop_counter_1   = OAM_ZERO; 

	rrc_get_cell_stats_resp_name.bitmask[OAM_ZERO] = OAM_ZERO;

	OAM_FUN_ENTRY_TRACE();
	OAM_NULL_CHECK(OAM_NULL != p_rrc_config_kpi_stats_req);


	for(loop_counter_1=OAM_ZERO;loop_counter_1<6;loop_counter_1++)
	{
		if((p_rrc_config_kpi_stats_req->kpiToReportBitMap)&(bit_value))
		{
			oam_set_bit_in_array_lsb(rrc_get_cell_stats_resp_name.bitmask,bit_position, OAM_ONE);
		}
		bit_value = bit_value<<OAM_ONE;
		bit_position++;
	}

	oam_pm_update_oam_periodic_info_t
		(
		 &(rrc_kpi_info.periodic_info),
		 sampling_intrvl_val, 
		 upload_intrvl_val
		);
	rrc_kpi_info.kpi_info_store_on_off_flag = kpi_on_off_flag;


	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pm_timer_start 
 * Description    : Function starts PM module timer
 * Inputs         : p_oam_pm_timer_expr_data:  
 *                : oam_pm_timer_val:  
 * Outputs        : 
 * Returns        :OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
	QTIMER oam_pm_timer_start
(
 Char8 *p_oam_pm_timer_expr_data,
 UInt64 oam_pm_timer_val
 )
{
	OAM_FUN_ENTRY_TRACE();
#ifdef OAM_UT_FRAMEWORK
	OAM_FUN_EXIT_TRACE();
	return OAM_GUARD_TIMER_TYPE;
#endif
	QTIME timer_timeout;
	QTIMER timer_id = OAM_ZERO;

	QMODULE module;

	OAM_NULL_CHECK(OAM_NULL != p_oam_pm_timer_expr_data)

		timer_timeout.s=oam_pm_timer_val;
	timer_timeout.us=OAM_ZERO;

	module = qvGetService(PM_MODULE_ID);
	if(OAM_NULL == module)
	{   
		OAM_LOG(OAM,OAM_ERROR,"Failed to get Service Name");
		return timer_id;
	}

#ifndef OAM_UT_FRAMEWORK
	timer_id = qvModuleTimerStart(module, &timer_timeout,
			p_oam_pm_timer_expr_data,
			OAM_ZERO, OAM_NULL);
    if(PNULL != timer_id)
    {
        OAM_LOG(OAM, OAM_BRIEF, "Timer Started: Timer [%p],"
                " TimerDuration [%u]sec [%u]msec",
                timer_id, timer_timeout.s, OAM_ZERO);
    }
    else
    {
		OAM_LOG(OAM, OAM_WARNING, "Timer Start failed");
    }
#endif
	OAM_FUN_EXIT_TRACE();
	return timer_id;
}

/****************************************************************************
 * Function Name  : oam_pm_timer_stop
 * Description    : Function stops PM module timer
 * Inputs         : timerId:  
 *                : oam_pm_timer_val:  
 * Outputs        : 
 * Returns        :OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et 
	oam_pm_timer_stop
(
 QTIMER timer_id
 )
{
	OAM_FUN_ENTRY_TRACE();
	SInt32 error_code = OAM_ZERO;
	UInt32 result = OAM_ZERO; 
	QTIME timer_left_for_timeout;

	if (OAM_NULL == timer_id)
		return OAM_FAILURE;

	result = qvModuleTimerStop(timer_id, &timer_left_for_timeout, &error_code);

	if (OAM_ZERO == result)
	{
		OAM_LOG(OAM,OAM_ERROR,"Failed to stop timer with error %d",
				error_code);
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_convert_byte_to_int 
 * Description    : Function 4 byte to int
 * Inputs         : source : array of bytes 
 * Outputs        :
 * Returns        : dest : int var 
 ****************************************************************************/
UInt32
	oam_convert_byte_to_int
(
 UInt8 source[]
 )
{
	UInt16 index = OAM_ZERO;
	UInt32 dest = OAM_ZERO;

	/* PM_CELL_ID_FIX_CHANGES_START */
	dest |= (source[index++] & OAM_ALL_8_BIT_SET);
	dest = ((dest << OAM_BYTE_SIZE_IN_BITS) | (source[index++] & OAM_ALL_8_BIT_SET));
	dest = ((dest << OAM_BYTE_SIZE_IN_BITS) | (source[index++] & OAM_ALL_8_BIT_SET));
	dest = ((dest << OAM_BYTE_SIZE_IN_BITS) | (source[index++] & 0xF0));

	dest >>= OAM_NIBBLE_SIZE_IN_BITS;	
	/* PM_CELL_ID_FIX_CHANGES_END */
	return dest;
}
/****************************************************************************
 * Function Name  : oam_PM_intf_process_timer 
 * Description    : Function process timer timeouts
 * Inputs         : p_timer_buf : 
 *                : p_context : 
 * Outputs        : 
 * Returns        : OAM_SUCCESS 
 ****************************************************************************/
oam_return_et 
	oam_PM_intf_process_timer
(
 oam_timer_t timer_id,
 void   *p_timer_buf,
 void   *p_context
 )
{
	Char8 curl_error_msg[OAM_CURL_ERROR_MSG_ZIZE] = {OAM_ZERO};	
	UInt32 curl_error_code = OAM_ZERO;
	/* Spr 12880 Fix Start*/
	UInt8 CellId = OAM_INVALID_CELL_ID;
	/* Spr 12880 Fix Start*/
	/* Coverity FIX 40892 */
	/* SPR 17777 fix  code removed */
	OAM_FUN_ENTRY_TRACE();

	OAM_NULL_CHECK(OAM_NULL != p_context);
	OAM_NULL_CHECK(OAM_NULL != p_timer_buf);

	/* SPR 17777 fix */
	OAM_LOG(OAM,OAM_BRIEF,"Timer expired: Timer [%p] p_context [%p]", 
            timer_id, p_context);
	if(oam_strcmp(p_timer_buf,"PERIODIC_UPLOAD_INTERVAL_TIMER")==OAM_ZERO)
	{
        /*SPR 23209 FIXED START*/
        if(OAM_ZERO != g_pm_xml_params.pm_xml_upload_interval_timer)
        {
		oam_pm_write_data_to_file();
		/* Write the File footer in the PM xml file and close it*/
		oam_pm_xml_file_close();
		g_pm_xml_params.pm_xml_file_ready_to_upload = OAM_TRUE;

		g_pm_xml_params.pm_xml_upload_interval_timer_diff = 
			g_pm_xml_params.pm_xml_upload_interval_timer;
		/*open pm xml file and write the initial stuff */
		/* SPR 17777 fix */
		if(OAM_FAILURE  == oam_pm_xml_file_open())
		{
			OAM_LOG(OAM,OAM_ERROR,"Error in File opening ");
			return OAM_FAILURE;
		}
		/* start the timer for next upload interval*/
		g_pm_xml_params.pm_xml_upload_interval_timer_id = oam_pm_timer_start("PERIODIC_UPLOAD_INTERVAL_TIMER",
				g_pm_xml_params.pm_xml_upload_interval_timer);
        /*SPR 20171 Start*/

        OAM_LOG(OAM,OAM_DETAILED,"PERIODIC_UPLOAD_INTERVAL_TIMER expired timer value started= %llu Id %p ",\
                g_pm_xml_params.pm_xml_upload_interval_timer,
                g_pm_xml_params.pm_xml_upload_interval_timer_id);				
        /*SPR 20171 End*/

		if (OAM_ZERO == g_pm_xml_params.pm_xml_upload_interval_timer_id)
		{
			return OAM_FAILURE;
		}    
	}
        /*SPR 23209 FIXED END*/
    }
	else if(oam_strcmp(p_timer_buf,"PERIODIC_UPLOAD_TIMER")==OAM_ZERO)
	{
		time_t t;
		oam_time(&t);
		/* Send Request to TR069 Agent with the data and file*/
		if(oam_prov_req.oam_tr069_req.tr069_mode)
		{
			oam_send_file_upload_request();
		}
		else
		{
			if(OAM_FAILURE == oam_pm_xml_ftp_server_upload(&curl_error_code))
			{
				oam_sprintf(curl_error_msg,  "FTP Upload Failed with libcurl error code : %d", curl_error_code);

				OAM_LOG(OAM, OAM_ERROR, "oam_pm_xml_ftp_server_upload failed with error: %u", curl_error_code);
				OAM_ALARM(PM_DATA_UPLOAD_FAILURE_ALARM_ID,
						2, NOTIFY_NEW_ALARM, PROCESSING_ERROR_ALARM,
						UNAVAILABLE, EXPEDITED_ALARM,
						/* Spr 12880 Fix Start*/
						curl_error_msg, OAM_NULL,CellId);
				/* Spr 12880 Fix Start*/
			}
		}
		g_pm_xml_params.pm_xml_upload_timer_id = 
			oam_pm_timer_start("PERIODIC_UPLOAD_TIMER", g_pm_xml_params.pm_xml_upload_interval_timer);

		if (OAM_ZERO == g_pm_xml_params.pm_xml_upload_timer_id)
		{
			return OAM_FAILURE;
		}    
	}
	/* SPR 15018 START */
	/* SPR 15018 DESCRIPTION: removed timer for KPI_CONFIG_REQUEST */

	else if(oam_strcmp(p_timer_buf,"PERIODIC_PM_RRM_XML_FILE_WRITER")==OAM_ZERO)
	{
		g_pm_rrm_xml_params.pm_rrm_xml_file_write_timer_id = oam_pm_timer_start("PERIODIC_PM_RRM_XML_FILE_WRITER", g_pm_rrm_xml_params.pm_rrm_xml_file_write_timer); 

		if (OAM_ZERO == g_pm_rrm_xml_params.pm_rrm_xml_file_write_timer_id)
		{
			return OAM_FAILURE;
		}    
		if(OAM_TRUE == g_pm_rrm_xml_params.rrm_periodic_reporting_ongoing)
		{
			oam_pm_write_data_to_rrm_file();
		}

	}
	else if(oam_strcmp(p_timer_buf,"PERIODIC_PM_XML_FILE_WRITER")==OAM_ZERO)
	{

		g_pm_xml_params.pm_xml_file_write_timer_id = oam_pm_timer_start("PERIODIC_PM_XML_FILE_WRITER",
				g_pm_xml_params.pm_xml_file_write_timer); 


		if (OAM_ZERO == g_pm_xml_params.pm_xml_file_write_timer_id)
		{
			return OAM_FAILURE;
		}    

		do
		{
			oam_pm_read_data_from_link_list();
		}while(OAM_NULL !=  rrc_kpi_info.pm_kpi_info_first_Node); 
		/*SPR 12814 FIX START*/
		xmlTextWriterFlush(writer);
		/*SPR 12814 FIX END*/		
        /*SPR 20171 Start*/
        /* SPR 22304 Fix Start */
        OAM_LOG(OAM,OAM_DETAILED,"PERIODIC_UPLOAD_TIMER SECOND = %d",mac_kpi_info.periodic_info.upload_intrvl);
        /* SPR 22304 Fix End */
        /*SPR 20171 End*/

	}	
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pm_bitmask_init 
 * Inputs         : NONE
 * Outputs        : NONE
 * Returns        : NONE
 * Description    : This function initialises bitmasks
 ****************************************************************************/
void
oam_pm_bitmask_init(
		/* + SPR 17439 */
		void
		)
/* - SPR 17439 */
{

	OAM_FUN_ENTRY_TRACE();

	g_mac_kpi_stats_ind_params_name.bitmask = OAM_ZERO;/* SPR 17949 */
	g_pdcp_kpi_stats_ind_params_name.bitmask[OAM_ZERO] = OAM_ZERO;

	rrc_get_cell_stats_resp_name.bitmask[OAM_ZERO] = OAM_ZERO;

	OAM_FUN_EXIT_TRACE();
}
/****************************************************************************
 * Function Name  : oam_pm_xml_file_open
 * Description    : This function opens l2_kpi.log in write mode
 * Inputs         : p_error_code : pointer to error_code
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
void oam_parse_time_from_string(char *str,char des_str[9])
{
	UInt8 i = OAM_ZERO;

	while (str[i] != ':')
	{
		i++;
	}
	des_str[0]=str[i-2];
	des_str[1]=str[i-1];
	des_str[2]=str[i];
	des_str[3]=str[i+1];
	des_str[4]=str[i+2];
	des_str[5]=str[i+3];
	des_str[6]=str[i+4];
	des_str[7]=str[i+5];
	des_str[8]='\0';

}


/****************************************************************************
 * Function Name  : oam_pm_xml_file_open 
 * Description    : Function opens xml file
 * Inputs         :p_error_code
 * Outputs        : 
 * Returns        : OAM_SUCCESS 
 ****************************************************************************/
/* SPR 17777 fix */
oam_return_et
	oam_pm_xml_file_open
(
 void
 )
{
	OAM_FUN_ENTRY_TRACE();
	char des_str[OAM_NINE] = {OAM_ZERO};
	/* SPR 15018 START */
	/* SPR 15018 DESCRIPTION: removed variable i */
	/* SPR 15018 END */
	int rc = OAM_ZERO;
	time_t timer = OAM_ZERO;
	Char8  tmpbuf2[50] = {OAM_ZERO};
	struct tm* tm_info;
	/*SPR 17531 Fixe Start */
	struct tm tm_info_tmp,tm_info1;
	/*SPR 17531 Fixe End */

	/* SPR 12698 FIX START */
	char num_buffer[NUM_BUFF_SIZE] = {OAM_ZERO};
	/* SPR 12698 FIX END */
	LIBXML_TEST_VERSION
		char mod_offset[OAM_TEN];
	char offset[OAM_TEN];
	UInt8 temp_arr[OAM_TWO] = {OAM_ZERO};
	oam_return_et retVal = OAM_SUCCESS;
	/* SPR 15018 START */
	/* SPR 15018 DESCRIPTION: removed variable value_in_pm_xml_param_name*/
	/* SPR 15018 END */
	char *temp_vender_name = "vendor";
	if(OAM_FAILURE == oam_pm_rrm_xml_file_open())
	{
		OAM_LOG(OAM, OAM_ERROR, "oam_pm_rrm_xml_file_open failed ");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}


	retVal = oam_get_pm_xml_filename(g_pm_xml_params.pm_xml_filename);

	writer = xmlNewTextWriterFilename(g_pm_xml_params.pm_xml_filename, OAM_ZERO);

	if (writer == OAM_NULL) 
	{
		OAM_LOG(OAM,OAM_ERROR, "testXmlwriterFilename: Error creating the xml writer");
		retVal = OAM_FAILURE;
		return retVal;
	}

	file_close.flag = OAM_ZERO;
	rc = xmlTextWriterStartDocument(writer, OAM_NULL, MY_ENCODING, OAM_NULL);
	if (rc < OAM_ZERO) 
	{
		OAM_LOG(OAM,OAM_ERROR, "testXmlwriterFilename: Error at xmlTextWriterStartDocument");
		retVal =  OAM_FAILURE;
		return retVal;
	}

	rc = xmlTextWriterSetIndent(writer,OAM_ONE);
	if (rc < OAM_ZERO)
	{
		OAM_LOG(OAM,OAM_ERROR, "testXmlwriterFilename: Error at xmlTextWriterSetIndent");
		retVal =  OAM_FAILURE;
		return retVal;
	}

	xmlTextWriterSetIndent(writer,OAM_ONE);

	xmlTextWriterStartElementNS(writer,OAM_NULL,BAD_CAST (xmlChar*)"measCollecFile", 
			(xmlChar*)"http://www.3gpp.org/ftp/specs/archive/32_series/32.435#measCollec");
	xmlTextWriterWriteAttribute(writer, BAD_CAST (xmlChar*)"xmlns:xsi",(xmlChar*)"http://www.w3.org/2001/XMLSchema-instance");
	xmlTextWriterWriteAttribute(writer, BAD_CAST (xmlChar*)"xsi:schemaLocation",
			(xmlChar*)"http://www.3gpp.org/ftp/specs/archive/32_series/32.435#measCollec http://www.3gpp.org/ftp/specs/archive/32_series/32.435#measCollec");

	xmlTextWriterStartElement(writer, BAD_CAST (xmlChar*)"fileHeader");
	xmlTextWriterWriteAttribute(writer, BAD_CAST (xmlChar*)"fileFormatVersion",(xmlChar*) "32.435 V9.1");

	/* if manufacturer is read from TR196 file then fill in else put it as NULL ""*/
	xmlTextWriterWriteAttribute(writer, BAD_CAST "vendorName",(xmlChar *)(temp_vender_name));


	/*Read dnPrefix from TR169 file in case its not present then put it as NULL ""*/
	/*Coverity FIX 40455 */
	if(OAM_ZERO != oam_strcmp((char *)oam_prov_req.oam_igd_params.services_params.DNPrefix,(const char *)temp_arr))
	{
		xmlTextWriterWriteAttribute(writer, BAD_CAST (xmlChar*)"dnPrefix", (xmlChar*)oam_prov_req.oam_igd_params.services_params.DNPrefix);
	}
	else
	{
		xmlTextWriterWriteAttribute(writer, BAD_CAST (xmlChar*)"dnPrefix",(xmlChar*)"");
	}

	xmlTextWriterStartElement(writer, BAD_CAST (xmlChar*)"fileSender");
	/* read OUI and Sno from prov structure and fill in*/
	oam_memset(&num_buffer, OAM_ZERO, sizeof(num_buffer));

	/*Coverity FIX 40455 */
	if((OAM_ZERO == oam_strcmp((char *)oam_prov_req.oam_igd_params.device_info_params.ManufacturerOUI,(const char *)temp_arr)) 
			&& 
			(OAM_ZERO == oam_strcmp((char *)oam_prov_req.oam_igd_params.device_info_params.SerialNumber,(const char *)temp_arr))
	  )
	{
		/* SPR 12698 FIX START */
		oam_snprintf(num_buffer,NUM_BUFF_SIZE,"FAPService=1,eNB=%s","");
		/* SPR 12698 FIX END */
	}
	else
	{
		/*Coverity FIX 40455 */
		if(OAM_ZERO == oam_strcmp((char *)oam_prov_req.oam_igd_params.device_info_params.ManufacturerOUI,(const char *)temp_arr))
		{
			/* SPR 12698 FIX START */
			oam_snprintf(num_buffer,NUM_BUFF_SIZE,"FAPService=1,eNB=%d-%d",0,
					oam_prov_req.oam_igd_params.device_info_params.SerialNumber);
			/* SPR 12698 FIX END */
		}
		/*Coverity FIX 40455 */
		else if(OAM_ZERO == oam_strcmp((char *)oam_prov_req.oam_igd_params.device_info_params.SerialNumber,(const char *)temp_arr))
		{
			/* SPR 12698 FIX START */
			oam_snprintf(num_buffer,NUM_BUFF_SIZE,"FAPService=1,eNB=%d-%d",oam_prov_req.oam_igd_params.device_info_params.ManufacturerOUI,
					0);
			/* SPR 12698 FIX END */
		}
		else
		{
			/* SPR 12698 FIX START */
			oam_sprintf(num_buffer,"FAPService=1,eNB=%d-%d",oam_prov_req.oam_igd_params.device_info_params.ManufacturerOUI,
					oam_prov_req.oam_igd_params.device_info_params.SerialNumber);
			/* SPR 12698 FIX END */
		}
	}

	xmlTextWriterWriteAttribute(writer, BAD_CAST (xmlChar*)"localDn",(xmlChar *)(num_buffer));
	oam_memset(&num_buffer, OAM_ZERO, sizeof(num_buffer));

	/* Fill in product class if not present then fill in NULL*/
	if (strncmp((char*)oam_prov_req.oam_igd_params.device_info_params.ProductClass, "", sizeof("")) != OAM_ZERO)
	{
		xmlTextWriterWriteAttribute(writer, BAD_CAST "elementType",(xmlChar *)(oam_prov_req.oam_igd_params.device_info_params.ProductClass));
		oam_memset(&num_buffer, OAM_ZERO, sizeof(num_buffer));
	}
	else
	{
		xmlTextWriterWriteAttribute(writer, BAD_CAST (xmlChar*)"elementType",(xmlChar *)"");

	}
	xmlTextWriterEndElement(writer);

	OAM_TIME(&timer);
	/*SPR 17531 Fixe Start */
	tm_info = localtime_r (&timer , &tm_info1);
	/*SPR 17531 Fixe End*/
	OAM_NULL_CHECK(NULL != tm_info);/* Coverity FIX 35428 */
	tm_info_tmp = *tm_info;
	oam_sprintf(tmpbuf2,"%d-%d-%dT",tm_info->tm_year + (1900),tm_info->tm_mon + 1,tm_info->tm_mday);
	strftime(offset, 10, "%z", &tm_info_tmp);
	oam_sprintf(mod_offset,"%c%c%c:%c%c",offset[0],offset[1],offset[2],offset[3],offset[4]);

	OAM_NULL_CHECK(NULL != OAM_CTIME(&timer));/* Coverity FIX 35427 */ 
	oam_parse_time_from_string(OAM_CTIME(&timer),des_str);
	oam_strcat(tmpbuf2,des_str);  

	strftime(offset, 10, "%z", tm_info);
	oam_sprintf(mod_offset,"%c%c%c:%c%c",offset[0],offset[1],offset[2],offset[3],offset[4]);
	oam_strcat(tmpbuf2,mod_offset);

	oam_memset(tmpbuf2, OAM_ZERO, 50);
	strftime(tmpbuf2, 50, "%Y-%m-%dT%H:%M:%S", tm_info);
	oam_strcat(tmpbuf2,mod_offset);

	xmlTextWriterStartElement(writer, BAD_CAST (xmlChar*)"measCollec");
	xmlTextWriterWriteAttribute(writer, BAD_CAST (xmlChar*)"beginTime",(xmlChar*)tmpbuf2);
	xmlTextWriterEndElement(writer);
	xmlTextWriterEndElement(writer);

	xmlTextWriterStartElement(writer, BAD_CAST (xmlChar*)"measData");

	xmlTextWriterStartElement(writer, BAD_CAST (xmlChar*)"managedElement");
	/* If the value of the dnPrefix is present in the prov structure then fill in the values*/
	oam_memset(&num_buffer, OAM_ZERO, sizeof(num_buffer));

	/*Coverity FIX 40455 */
	if((OAM_ZERO == oam_strcmp((char *)oam_prov_req.oam_igd_params.device_info_params.ManufacturerOUI,(const char *)temp_arr)) 
			&& 
			(OAM_ZERO == oam_strcmp((char *)oam_prov_req.oam_igd_params.device_info_params.SerialNumber,(const char *)temp_arr))
	  )
	{
		/* SPR 12698 FIX START */
		oam_snprintf(num_buffer,NUM_BUFF_SIZE,"FAPService=1,eNB=%s","");
		/* SPR 12698 FIX END */
	}
	else
	{
		/*Coverity FIX 40455 */
		if(OAM_ZERO == oam_strcmp((char *)oam_prov_req.oam_igd_params.device_info_params.ManufacturerOUI,(const char *)temp_arr))
		{
			/* SPR 12698 FIX START */
			oam_snprintf(num_buffer,NUM_BUFF_SIZE,"FAPService=1,eNB=%d-%d",0,
					oam_prov_req.oam_igd_params.device_info_params.SerialNumber);
			/* SPR 12698 FIX END */

		}
		/*Coverity FIX 40455 */
		else if(OAM_ZERO == oam_strcmp((char *)oam_prov_req.oam_igd_params.device_info_params.SerialNumber,(const char *)temp_arr))
		{
			/* SPR 12698 FIX START */
			oam_snprintf(num_buffer,NUM_BUFF_SIZE,"FAPService=1,eNB=%d-%d",oam_prov_req.oam_igd_params.device_info_params.ManufacturerOUI,
					0);
			/* SPR 12698 FIX END */
		}
		else
		{
			/* SPR 12698 FIX START */
			oam_sprintf(num_buffer,"FAPService=1,eNB=%d-%d",oam_prov_req.oam_igd_params.device_info_params.ManufacturerOUI,
					oam_prov_req.oam_igd_params.device_info_params.SerialNumber);
			/* SPR 12698 FIX END */
		}
	}

	xmlTextWriterWriteAttribute(writer, BAD_CAST (xmlChar*)"localDn",(xmlChar *)(num_buffer));
	oam_memset(&num_buffer, OAM_ZERO, sizeof(num_buffer));

	xmlTextWriterWriteAttribute(writer, BAD_CAST (xmlChar*)"userLabel",(xmlChar*)"eNB");
	xmlTextWriterWriteAttribute(writer, BAD_CAST (xmlChar*)"swVersion",
			(xmlChar*)oam_prov_req.oam_igd_params.device_info_params.SoftwareVersion);
	/* SPR 15018 START */
	/* SPR 15018 DESCRIPTION: removed section of code */
	/* SPR 15018 END */
	OAM_FUN_EXIT_TRACE();
	return retVal;
}

/****************************************************************************
 * Function Name  : oam_pm_rrm_xml_file_open
 * Description    : Function opens xml file
 * Inputs         :
 * Outputs        : 
 * Returns        : OAM_SUCCESS 
 ****************************************************************************/

oam_return_et
oam_pm_rrm_xml_file_open
(
 /* + SPR 17439 */
 void
 )
/* - SPR 17439 */
{
	OAM_FUN_ENTRY_TRACE();
	char des_str[OAM_NINE] = {OAM_ZERO};
	UInt8 i = OAM_ZERO;
	int rc = OAM_ZERO;
	time_t timer = OAM_ZERO;
	Char8 tmpbuf2[50] = {OAM_ZERO};
	struct tm* tm_info;
	/*SPR 17531 Fixe Start */
	struct tm tm_info_tmp,tm_info1;
	/*SPR 17531 Fixe End */

	/* SPR 12698 FIX START */
	char num_buffer[100] = {OAM_ZERO};
	/* SPR 12698 FIX END */
	LIBXML_TEST_VERSION
		char offset[OAM_TEN];
	char **value_in_pm_xml_param_name = pm_xml_rrm_param_name;

	oam_get_rrm_pm_xml_filename((char*)g_pm_rrm_xml_params.pm_rrm_xml_filename);

	rrm_writer = xmlNewTextWriterFilename(g_pm_rrm_xml_params.pm_rrm_xml_filename, OAM_ZERO);

	if (rrm_writer == OAM_NULL) 
	{
		OAM_LOG(OAM,OAM_ERROR, "testXmlwriterFilename: Error creating the xml writer");
		return OAM_FAILURE;
	}

	file_close.rrm_flag = OAM_ZERO;
	rc = xmlTextWriterStartDocument(rrm_writer, OAM_NULL, MY_ENCODING, OAM_NULL);
	if (rc < OAM_ZERO) 
	{
		OAM_LOG(OAM,OAM_ERROR, "testXmlwriterFilename: Error at xmlTextWriterStartDocument");
		return OAM_FAILURE;
	}

	rc = xmlTextWriterSetIndent(rrm_writer,OAM_ONE);
	if (rc < OAM_ZERO)
	{
		OAM_LOG(OAM,OAM_ERROR, "testXmlwriterFilename: Error at xmlTextWriterSetIndent");
		return OAM_FAILURE;
	}
	xmlTextWriterSetIndent(rrm_writer,OAM_ONE);

	xmlTextWriterStartElementNS(rrm_writer,OAM_NULL,BAD_CAST (xmlChar*)"measCollecFile", 
			(xmlChar*)"http://www.3gpp.org/ftp/specs/archive/32_series/32.435#measCollec");
	xmlTextWriterWriteAttribute(rrm_writer, BAD_CAST (xmlChar*)"xmlns:xsi",(xmlChar*)"http://www.w3.org/2001/XMLSchema-instance");
	xmlTextWriterWriteAttribute(rrm_writer, BAD_CAST (xmlChar*)"xsi:schemaLocation",
			(xmlChar*)"http://www.3gpp.org/ftp/specs/archive/32_series/32.435#measCollec http://www.3gpp.org/ftp/specs/archive/32_series/32.435#measCollec");

	xmlTextWriterStartElement(rrm_writer, BAD_CAST (xmlChar*)"fileHeader");
	xmlTextWriterWriteAttribute(rrm_writer, BAD_CAST (xmlChar*)"fileFormatVersion",(xmlChar*) "32.435 V9.1");

	/* if manufacturer is read from TR196 file then fill in else put it as NULL "" */
	xmlTextWriterWriteAttribute(rrm_writer, BAD_CAST "vendorName",(xmlChar *)(oam_prov_req.oam_igd_params.device_info_params.ManufacturerOUI));
	/*Read dnPrefix from TR169 file in case its not present then put it as NULL ""*/
	xmlTextWriterWriteAttribute(rrm_writer, BAD_CAST (xmlChar*)"dnPrefix", (xmlChar*)oam_prov_req.oam_igd_params.services_params.DNPrefix);
	xmlTextWriterEndElement(rrm_writer);


	xmlTextWriterStartElement(rrm_writer, BAD_CAST (xmlChar*)"fileSender");
	/* read OUI and Sno from prov structure and fill in*/
	oam_memset(&num_buffer, OAM_ZERO, sizeof(num_buffer));
	/* SPR 12698 FIX START */
	oam_snprintf(num_buffer,NUM_BUFF_SIZE,"FAPService=1,HeNB=%d-%d",oam_prov_req.oam_igd_params.device_info_params.ManufacturerOUI,
			oam_prov_req.oam_igd_params.device_info_params.SerialNumber); 
	/* SPR 12698 FIX END */
	xmlTextWriterWriteAttribute(rrm_writer, BAD_CAST (xmlChar*)"localDn",(xmlChar *)(num_buffer));
	oam_memset(&num_buffer, OAM_ZERO, sizeof(num_buffer));

	/* Fill in product class if not present then fill in NULL*/
	if (strncmp((char*)oam_prov_req.oam_igd_params.device_info_params.ProductClass, "", sizeof("")) != OAM_ZERO)
	{
		xmlTextWriterWriteAttribute(rrm_writer, BAD_CAST "elementType",(xmlChar *)(oam_prov_req.oam_igd_params.device_info_params.ProductClass));
		oam_memset(&num_buffer, OAM_ZERO, sizeof(num_buffer));
	}
	else
	{
		xmlTextWriterWriteAttribute(rrm_writer, BAD_CAST (xmlChar*)"elementType",(xmlChar *)"");

	}
	xmlTextWriterEndElement(rrm_writer);

	OAM_TIME(&timer);
	/*SPR 17531 Fixe Start */
	tm_info = localtime_r (&timer , &tm_info1);
	/*SPR 17531 Fixe End*/
	OAM_NULL_CHECK(NULL != tm_info);/* Coverity FIX 35428 */
	tm_info_tmp = *tm_info;
	oam_sprintf(tmpbuf2,"%d-%d-%dT",tm_info->tm_year + (1900),tm_info->tm_mon + 1,tm_info->tm_mday);
	strftime(offset, 10, "%z", &tm_info_tmp);


	OAM_NULL_CHECK(NULL != OAM_CTIME(&timer));/* Coverity FIX 35427 */ 
	oam_parse_time_from_string(OAM_CTIME(&timer),des_str);
	oam_strcat(tmpbuf2,des_str);  

	xmlTextWriterStartElement(rrm_writer, BAD_CAST (xmlChar*)"measCollec");
	xmlTextWriterWriteAttribute(rrm_writer, BAD_CAST (xmlChar*)"beginTime",(xmlChar*)tmpbuf2);//timebuf1);
	xmlTextWriterEndElement(rrm_writer);

	xmlTextWriterStartElement(rrm_writer, BAD_CAST (xmlChar*)"measData");

	xmlTextWriterStartElement(rrm_writer, BAD_CAST (xmlChar*)"managedElement");
	/* If the value of the dnPrefix is present in the prov structure then fill in the values*/
	oam_memset(&num_buffer, OAM_ZERO, sizeof(num_buffer));
	/* SPR 12698 FIX START */
	oam_snprintf(num_buffer,NUM_BUFF_SIZE,"FAPService=1,HeNB=%d-%d",oam_prov_req.oam_igd_params.device_info_params.ManufacturerOUI,
			oam_prov_req.oam_igd_params.device_info_params.SerialNumber); 
	/* SPR 12698 FIX END */
	xmlTextWriterWriteAttribute(rrm_writer, BAD_CAST (xmlChar*)"localDn",(xmlChar *)(num_buffer));
	oam_memset(&num_buffer, OAM_ZERO, sizeof(num_buffer));   

	xmlTextWriterWriteAttribute(rrm_writer, BAD_CAST (xmlChar*)"userLabel",(xmlChar*)"eNB");
	xmlTextWriterWriteAttribute(rrm_writer, BAD_CAST (xmlChar*)"swVersion",
			(xmlChar*)oam_prov_req.oam_igd_params.device_info_params.SoftwareVersion);
	xmlTextWriterEndElement(rrm_writer);

	xmlTextWriterStartElement(rrm_writer, BAD_CAST (xmlChar*)"MeasInfo");

	xmlTextWriterStartElement(rrm_writer, BAD_CAST (xmlChar*)"job");
	/* SPR 12698 FIX START */
	oam_sprintf(num_buffer,"%d",job_id); 
	/* SPR 12698 FIX END */
	xmlTextWriterWriteAttribute(rrm_writer, BAD_CAST (xmlChar*)"jobId",(xmlChar *)(num_buffer));
	job_id++;
	xmlTextWriterEndElement(rrm_writer);
	oam_memset(&num_buffer, OAM_ZERO, sizeof(num_buffer));

	xmlTextWriterStartElement(rrm_writer, BAD_CAST "granPeriod");
	/* SPR 12698 FIX START */
	oam_sprintf(num_buffer,"PT%dS",g_pm_xml_params.pm_xml_upload_interval_timer);
	/* SPR 12698 FIX END */
	xmlTextWriterWriteAttribute(rrm_writer, BAD_CAST "duration",(xmlChar*)num_buffer);

	/*timer contains the current time, increment by the time till which we plan to 
	  collect stats and enter the file*/
	timer += g_pm_xml_params.pm_xml_upload_interval_timer_diff;
	/*SPR 17531 Fixe Start */
	tm_info = localtime_r (&timer , &tm_info1);
	/*SPR 17531 Fixe End*/

	OAM_NULL_CHECK(NULL != tm_info);/* Coverity FIX 35428 */
	tm_info_tmp = *tm_info;

	for(i = OAM_ZERO; i<50; i++)
		tmpbuf2[i] = OAM_ZERO; 

	oam_sprintf(tmpbuf2,"%d-%d-%dT",tm_info->tm_year + (1900),tm_info->tm_mon + OAM_ONE,tm_info->tm_mday);

	OAM_NULL_CHECK(NULL != OAM_CTIME(&timer));/* Coverity FIX 35427 */ 
	oam_parse_time_from_string(OAM_CTIME(&timer),des_str);
	oam_strcat(tmpbuf2,des_str);  

	xmlTextWriterWriteAttribute(rrm_writer, BAD_CAST (xmlChar*)"endtime",(xmlChar*)tmpbuf2);
	xmlTextWriterEndElement(rrm_writer);

	xmlTextWriterStartElement(rrm_writer, BAD_CAST (xmlChar*)"repPeriod");
	xmlTextWriterWriteAttribute(rrm_writer, BAD_CAST (xmlChar*)"duration",(xmlChar *)num_buffer);
	xmlTextWriterEndElement(rrm_writer);

	value_in_pm_xml_param_name = pm_xml_rrm_param_name;
	oam_pm_write_rrm_xml_param_name(rrm_writer,value_in_pm_xml_param_name);

	xmlTextWriterFlush(rrm_writer);
	OAM_LOG(OAM,OAM_DETAILED,"Total Elements:::%d", pm_total_elements_in_rrm_xml);
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;		  
}

void oam_pm_write_rrm_xml_param_name(xmlTextWriter *rrm_writer, char **value_xml_param)
{
	UInt16 index = OAM_ZERO;
	xmlChar num_buff[OAM_TWENTY_FIVE];
	oam_memset(num_buff, OAM_ZERO, sizeof(num_buff));

	while(*value_xml_param)
	{
		index++;
		oam_sprintf((char*)num_buff,"%d",pm_total_elements_in_rrm_xml); 
		xmlTextWriterStartElement(rrm_writer, (xmlChar*)"measType");
		xmlTextWriterWriteAttribute(rrm_writer, (xmlChar*)"p",num_buff);
		xmlTextWriterWriteString(rrm_writer,(xmlChar *)(*value_xml_param));
		xmlTextWriterEndElement(rrm_writer);
		value_xml_param++;
		pm_total_elements_in_rrm_xml++;
		oam_memset(&num_buff, OAM_ZERO, oam_strlen((char*)num_buff));
	}

}
void oam_pm_write_xml_param_name(xmlTextWriter *writer, char **value_xml_param)
{
	UInt16 index = OAM_ZERO;
	xmlChar num_buff[OAM_TWENTY_FIVE];
	oam_memset(num_buff, OAM_ZERO, sizeof(num_buff));

	while(*value_xml_param)
	{
		index++;
		oam_sprintf((char*)num_buff,"%d",pm_total_elements_in_xml); 
		xmlTextWriterStartElement(writer, (xmlChar*)"measType");
		xmlTextWriterWriteAttribute(writer, (xmlChar*)"p",num_buff);
		xmlTextWriterWriteString(writer,(xmlChar *)(*value_xml_param));
		xmlTextWriterEndElement(writer);
		value_xml_param++;
		pm_total_elements_in_xml++;
		oam_memset(&num_buff, OAM_ZERO, oam_strlen((char*)num_buff));
	}

}


/****************************************************************************
 * Function Name  : oam_pm_rrm_xml_file_close
 * Description    : Function closes rrm xml file
 * Inputs         :
 * Outputs        : 
 * Returns        : OAM_SUCCESS 
 ****************************************************************************/
oam_return_et
oam_pm_rrm_xml_file_close(
		/* + SPR 17439 */
		void
		)
/* - SPR 17439 */
{
	time_t timer;
	xmlChar timebuf2[OAM_TWENTY_FIVE];
	struct tm* tm_info;
	char mod_offset[OAM_TEN];
	char offset[OAM_TEN];
	Char8 tmpbuf2[50] = {OAM_ZERO}; 
	Char8 des_str[OAM_NINE] = {OAM_ZERO};
	/* COV_FIX_18_JUN */
	xmlChar * str_data = OAM_NULL;
	/* COV_FIX_18_JUN */
	OAM_TIME(&timer);

	tm_info =OAM_GMTIME(&timer);
	OAM_NULL_CHECK(NULL != tm_info);/* Coverity FIX 35426 52763*/
	oam_sprintf(tmpbuf2,"%d-%d-%dT",tm_info->tm_year + (1900),tm_info->tm_mon + 1,tm_info->tm_mday);

	OAM_ASSERT(NULL != OAM_CTIME(&timer));/* Coverity FIX 35425 */
	oam_parse_time_from_string(OAM_CTIME(&timer),des_str);
	oam_strcat(tmpbuf2, des_str);  

	strftime(offset, 10, "%z", tm_info);
	oam_sprintf(mod_offset,"%c%c%c:%c%c",offset[0],offset[1],offset[2],offset[3],offset[4]);
	oam_memset(timebuf2, OAM_ZERO, sizeof(timebuf2));
	strftime((char *)timebuf2, sizeof(timebuf2), "%Y-%m-%dT%H:%M:%S", tm_info);
	oam_strcat((char *)timebuf2,mod_offset);
	OAM_FUN_ENTRY_TRACE();
/* SPR 20928 Fix Start*/
    if(OAM_ZERO == file_close.rrm_flag)
    {
         OAM_LOG(OAM ,OAM_DETAILED ,"  In function oam_pm_rrm_xml_file_close");
	xmlTextWriterEndElement(rrm_writer);
	xmlTextWriterEndElement(rrm_writer); 

        xmlTextWriterStartElement(rrm_writer,(xmlChar *) "fileFooter");

        xmlTextWriterStartElement(rrm_writer, (xmlChar *) "measCollec");
	/* COV_FIX_18_JUN */
	str_data = (xmlChar *)OAM_STRDUP((char *)timebuf2);
        xmlTextWriterWriteAttribute(rrm_writer, (xmlChar *) "endTime",str_data);
	/* COV_FIX_18_JUN */
	xmlTextWriterEndElement(rrm_writer);
	xmlTextWriterEndElement(rrm_writer);


	xmlTextWriterEndDocument(rrm_writer);
	xmlKeepBlanksDefault(OAM_ONE);
	xmlFreeTextWriter(rrm_writer);
    }
/* SPR 20928 Fix End*/
	file_close.rrm_flag = OAM_ONE;

	/* RRM_KPI_INDEXING_FIX */
	pm_total_elements_in_rrm_xml = OAM_ONE;
	/* RRM_KPI_INDEXING_FIX */

	oam_free(str_data);
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;

}

/****************************************************************************
 * Function Name  : oam_pm_xml_file_close
 * Description    : This function closes pm file 
 * Inputs         : p_error_code : pointer to error_code
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/

oam_return_et oam_get_pm_xml_filename(char *pm_xml_filename)
{
	OAM_FUN_ENTRY_TRACE();
	/*SPR 17531 Fixe Start */
	struct tm *tm_info ,tm_info1 ;
	/*SPR 17531 Fixe End */
	char tempBuf[OAM_TWENTY_FIVE];
	char dest_str[OAM_NINE];
	char num_buf[50];
	time_t now = OAM_TIME(OAM_NULL);
	time_t next = OAM_TIME(OAM_NULL);
	UInt8 temp_arr[OAM_TWO] = {OAM_ZERO};
	/* SPR 14997 START */
	char temp_utc_buf[6] = {OAM_ZERO};
	/* SPR 14997 END */

	now++;
	tm_info = OAM_GMTIME(&now);
	OAM_NULL_CHECK(NULL != tm_info);/* Coverity FIX 35426 52764*/

	/*SPR_16767_START*/
	if (g_pm_xml_params.pm_xml_sampling_interval_timer != 
			g_pm_xml_params.pm_xml_upload_interval_timer)
	{
		oam_strcpy(pm_xml_filename,"C");
	}
	else
	{
		oam_strcpy(pm_xml_filename,"A");
	}
	/*SPR_16767_END*/

	if (tm_info->tm_mon < OAM_NINE)
	{
		if(tm_info->tm_mday <OAM_TEN)
		{
			oam_sprintf((char *)tempBuf,"%d0%d0%d.",tm_info->tm_year + (1900),tm_info->tm_mon + OAM_ONE,tm_info->tm_mday);
		}
		else
		{
			oam_sprintf((char *)tempBuf,"%d0%d%d.",tm_info->tm_year + (1900),tm_info->tm_mon + OAM_ONE,tm_info->tm_mday);
		}
	}
	else
	{
		if(tm_info->tm_mday < OAM_TEN)
		{
			oam_sprintf((char *)tempBuf,"%d%d0%d.",tm_info->tm_year + (1900),tm_info->tm_mon + OAM_ONE,tm_info->tm_mday);
		}
		else
		{
			oam_sprintf((char *)tempBuf,"%d%d%d.",tm_info->tm_year + (1900),tm_info->tm_mon + OAM_ONE,tm_info->tm_mday);
		}
	}
	oam_strcat(pm_xml_filename, tempBuf);

	OAM_ASSERT(NULL != OAM_CTIME(&now));/* Coverity FIX 35425 */
	oam_parse_time_from_string(OAM_CTIME(&now),(char *)dest_str);
	oam_memset(tempBuf, OAM_ZERO, 25);
	oam_sprintf((char *)tempBuf,"%c%c%c%c", dest_str[0], dest_str[1], dest_str[3], dest_str[4]);
	oam_strcat(pm_xml_filename, tempBuf);
	/*SPR 17531 Fixe Start/End */
	/* SPR 14997 START */
	/*******************************************/
	if(tm_info->tm_gmtoff < 0)
		oam_sprintf((char *)temp_utc_buf,"%c",'-');
	else
		oam_sprintf((char *)temp_utc_buf,"%c",'+');

	if((oam_abs(tm_info->tm_gmtoff)/3600) < 10)
		oam_sprintf(((char *)temp_utc_buf + OAM_ONE),"0%d",(oam_abs(tm_info->tm_gmtoff)/3600));
	else
		oam_sprintf(((char *)temp_utc_buf + OAM_ONE),"%d",(oam_abs(tm_info->tm_gmtoff)/3600));

	if(((oam_abs(tm_info->tm_gmtoff)%3600)/60) < 10)
		oam_sprintf(((char *)temp_utc_buf + OAM_THREE),"0%d",(oam_abs(tm_info->tm_gmtoff)%3600)/60);
	else
		oam_sprintf(((char *)temp_utc_buf + OAM_THREE),"%d",(oam_abs(tm_info->tm_gmtoff)%3600)/60);

	temp_utc_buf[5] = '\0';

	oam_strcat(pm_xml_filename, temp_utc_buf);
	/* SPR 14997 END */
	/*SPR 17531 Fixe Start */
	oam_strcat(pm_xml_filename,"-");
	/*SPR 17531 Fixe End */

	/*Coverity fix start : 83335 */
	if(3600 == g_pm_xml_params.pm_xml_upload_interval_timer)
	{
		next = now + ( g_pm_xml_params.pm_xml_upload_interval_timer -
				(((UInt64)60)*((time_t)(tm_info->tm_min)) + ((time_t)(tm_info->tm_sec)))); // coverity_fix 83313 +-
	}
	else
	{
		next = now + ( g_pm_xml_params.pm_xml_upload_interval_timer -
				/*SPR 17531 Fixe Start */
				(((UInt64)3600) * ((time_t)(tm_info->tm_hour)) +((UInt64) 60)*((time_t)(tm_info->tm_min)) + ((time_t)(tm_info->tm_sec))));      // coverity_fix 83335 +-
		/*SPR 17531 Fixe End */
	}
	/*Coverity fix End : 83335 */
	next++;
	/*SPR_16767_START*/
	/*Enddate field shall only be included if the
	 * Type field is set to "C" or "D"*/
	if (g_pm_xml_params.pm_xml_sampling_interval_timer != 
			g_pm_xml_params.pm_xml_upload_interval_timer)
	{
		/*SPR 17531 Fixe Start */
		/*SPR 17531 Fixe Start */
		tm_info = localtime_r(&next,&tm_info1);
		/*SPR 17531 Fixe End */
		/*SPR 17531 Fixe End*/
		OAM_NULL_CHECK(NULL != tm_info);/* Coverity FIX 35426 52762*/
		if (tm_info->tm_mon < 9 )
		{
			if(tm_info->tm_mday <10)
			{
				oam_sprintf((char *)tempBuf,"%d0%d0%d.",tm_info->tm_year + (1900),tm_info->tm_mon + 1,tm_info->tm_mday);
			}
			else
			{
				oam_sprintf((char *)tempBuf,"%d0%d%d.",tm_info->tm_year + (1900),tm_info->tm_mon + 1,tm_info->tm_mday);
			}
		}
		else
		{
			if(tm_info->tm_mday <10)
			{
				oam_sprintf((char *)tempBuf,"%d%d0%d.",tm_info->tm_year + (1900),tm_info->tm_mon + 1,tm_info->tm_mday);
			}
			else
			{
				oam_sprintf((char *)tempBuf,"%d%d%d.",tm_info->tm_year + (1900),tm_info->tm_mon + 1,tm_info->tm_mday);
			}
		}

		oam_strcat(pm_xml_filename, tempBuf);
	}
	/*SPR_16767_END*/

	OAM_ASSERT(NULL != OAM_CTIME(&next));/* Coverity FIX 35425 */
	oam_parse_time_from_string(OAM_CTIME(&next),(char *)dest_str);
	oam_memset(tempBuf, OAM_ZERO, OAM_TWENTY_FIVE);
	oam_sprintf((char *)tempBuf,"%c%c%c%c", dest_str[OAM_ZERO], dest_str[OAM_ONE], dest_str[3], dest_str[OAM_FOUR]);
	oam_strcat(pm_xml_filename, tempBuf);
	/*SPR 17531 Fixe Start/End */

	/* SPR 14997 START */
	oam_strcat(pm_xml_filename, temp_utc_buf);
	/* SPR 14997 END */

	/*SPR 17531 Fixe Start */
	oam_sprintf(num_buf,"_%s",oam_prov_req.oam_igd_params.device_info_params.ManufacturerOUI);
	oam_strcat(pm_xml_filename,num_buf);
	/*SPR 17531 Fixe End */
	oam_memset(num_buf, OAM_ZERO, 50);
	/* COVERITY FIX - 10134 */
	if(oam_strcmp((char *)oam_prov_req.oam_igd_params.device_info_params.SerialNumber,(const char *)temp_arr)!=OAM_ZERO)
	{
		oam_sprintf(num_buf,"-%s",oam_prov_req.oam_igd_params.device_info_params.SerialNumber);
		oam_strcat(pm_xml_filename,num_buf);
	}
	oam_strcat(pm_xml_filename,".xml");
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_get_rrm_pm_xml_filename
 * Description    : Function gets xml filename 
 * Inputs         :pm_xml_filename
 * Outputs        : 
 * Returns        : OAM_SUCCESS 
 ****************************************************************************/
oam_return_et oam_get_rrm_pm_xml_filename(char *pm_xml_filename)
{
	struct tm* tm_info;
	char tempBuf[OAM_TWENTY_FIVE];
	char dest_str[OAM_NINE];
	time_t now = OAM_TIME(OAM_NULL);
	time_t next = OAM_TIME(OAM_NULL);
	OAM_FUN_ENTRY_TRACE();
	now++;
	tm_info = OAM_GMTIME(&now);
	OAM_NULL_CHECK(NULL != tm_info);/* Coverity FIX 35426 52764*/
	oam_strcpy(pm_xml_filename,"RRM_KPI_");

	if (tm_info->tm_mon < 9)
	{
		if(tm_info->tm_mday <10)
		{
			oam_sprintf((char *)tempBuf,"%d0%d0%d.",tm_info->tm_year + (1900),tm_info->tm_mon + OAM_ONE,tm_info->tm_mday);
		}
		else
		{
			oam_sprintf((char *)tempBuf,"%d0%d%d.",tm_info->tm_year + (1900),tm_info->tm_mon + OAM_ONE,tm_info->tm_mday);
		}
	}
	else
	{
		if(tm_info->tm_mday <10)
		{
			oam_sprintf((char *)tempBuf,"%d%d0%d.",tm_info->tm_year + (1900),tm_info->tm_mon + OAM_ONE,tm_info->tm_mday);
		}
		else
		{
			oam_sprintf((char *)tempBuf,"%d%d%d.",tm_info->tm_year + (1900),tm_info->tm_mon + OAM_ONE,tm_info->tm_mday);
		}
	}
	oam_strcat(pm_xml_filename, tempBuf);

	OAM_ASSERT(NULL != OAM_CTIME(&now));/* Coverity FIX 35425 */
	oam_parse_time_from_string(OAM_CTIME(&now),(char *)dest_str);
	oam_memset(tempBuf, OAM_ZERO, OAM_TWENTY_FIVE);
	oam_sprintf((char *)tempBuf,"%c%c%c%c", dest_str[0], dest_str[1], dest_str[3], dest_str[4]);
	oam_strcat(pm_xml_filename, tempBuf);
	oam_strcat(pm_xml_filename,"-");
	/*Coverity fix start : 83335 */
	if(3600 == g_pm_xml_params.pm_xml_upload_interval_timer)
	{
		next = now + ( g_pm_xml_params.pm_xml_upload_interval_timer -
				(((UInt64)60)*((time_t)(tm_info->tm_min)) + ((time_t)(tm_info->tm_sec)))); // coverity_fix 83315 +-
	}
	else
	{
		next = now + ( g_pm_xml_params.pm_xml_upload_interval_timer -
				(((UInt64)3600) * ((time_t)(tm_info->tm_hour)) +((UInt64)
					60)*((time_t)(tm_info->tm_min)) + ((time_t)(tm_info->tm_sec)))); // coverity_fix     83334 +-
	}
	/*Coverity fix End : 83335 */
	next++;
	tm_info = OAM_GMTIME(&next);
	OAM_NULL_CHECK(NULL != tm_info);/* Coverity FIX 35426 52762*/
	if (tm_info->tm_mon < 9 )
	{
		if(tm_info->tm_mday <10)
		{
			oam_sprintf((char *)tempBuf,"%d0%d0%d.",tm_info->tm_year + (1900),tm_info->tm_mon + 1,tm_info->tm_mday);
		}
		else
		{
			oam_sprintf((char *)tempBuf,"%d0%d%d.",tm_info->tm_year + (1900),tm_info->tm_mon + 1,tm_info->tm_mday);
		}
	}
	else
	{
		if(tm_info->tm_mday <10)
		{
			oam_sprintf((char *)tempBuf,"%d%d0%d.",tm_info->tm_year + (1900),tm_info->tm_mon + 1,tm_info->tm_mday);
		}
		else
		{
			oam_sprintf((char *)tempBuf,"%d%d%d.",tm_info->tm_year + (1900),tm_info->tm_mon + 1,tm_info->tm_mday);
		}
	}

	oam_strcat(pm_xml_filename, tempBuf);

	OAM_ASSERT(NULL != OAM_CTIME(&next));/* Coverity FIX 35425 */
	oam_parse_time_from_string(OAM_CTIME(&next),(char *)dest_str);
	oam_memset(tempBuf, OAM_ZERO, 25);
	oam_sprintf((char *)tempBuf,"%c%c%c%c", dest_str[0], dest_str[1], dest_str[3], dest_str[4]);
	oam_strcat(pm_xml_filename, tempBuf);
	oam_strcat(pm_xml_filename,".xml");
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_pm_xml_file_close
 * Description    : This function closes pm file 
 * Inputs         : p_error_code : pointer to error_code
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
void
oam_pm_xml_file_close(
		/* + SPR 17439 */
		void
		)
/* - SPR 17439 */
{
	/* SPR 15018 START */
	/* SPR 15018 DESCRIPTION: removed timer, timebuf2[25],tm_info, mod_offset[10], offset[10], tmpbuf2[50], des_str[9]
	 *      * and code chunk */
	/* SPR 15366 START */
	char mv_cmd[STR_LEN_128] = {OAM_ZERO};
	/* SPR 15366 END */
	/* COV_FIX_18_JUN */
	//code removed SPR 20928 
	/* COV_FIX_18_JUN */

	/* coverity 52764 */
	OAM_FUN_ENTRY_TRACE();

	/* SPR 15018 START */
	/* SPR 15018 DESCRIPTION: removed xmlTextWriterEndElement*/
	/* SPR 15018 END*/
	/* OAM long run crash while running periodic start + */
	//xmlTextWriterEndElement(writer); 
	/* OAM long run crash while running periodic start - */
    /*SPR 20928 Fix Start*/
    /* Coverity 113007 Fix Start */
    if(OAM_ZERO == file_close.flag)
    {
        /* SPR 22180 FIX START */
        xmlTextWriterEndElement(writer);
        xmlTextWriterEndElement(writer);
        /* SPR 22180 FIX END */
        //xmlTextWriterStartElement(writer, BAD_CAST "fileFooter");
        xmlTextWriterStartElement(writer, (xmlChar *) "fileFooter");

        //xmlTextWriterStartElement(writer, BAD_CAST "measCollec");
        xmlTextWriterStartElement(writer, (xmlChar *) "measCollec");
        /* SPR 17176 changes start */
        //xmlTextWriterWriteAttribute(writer, BAD_CAST "endTime",endTime_buffer);
        oam_calculate_endTime(endTime_buffer);
        xmlTextWriterWriteAttribute(writer, (xmlChar *) "endTime",endTime_buffer);
        /* SPR 17176 changes end */
        /* COV_FIX_18_JUN */

        /* SPR 22180 FIX START */
        /* Code Removed  */
        /* SPR 22180 FIX END */

        xmlTextWriterEndDocument(writer);
        xmlKeepBlanksDefault(1);
        xmlFreeTextWriter(writer);


        /* SPR 15366 FIX : code removed */
    }
    file_close.flag = OAM_ONE;
    /*SPR 20928 Fix End*/
    oam_sprintf(mv_cmd,"mv %s %s", g_pm_xml_params.pm_xml_filename, "./PM_XML_UPLOAD"); 
    pm_total_elements_in_xml = OAM_ONE;

    /* COV_FIX_18_JUN */
    /* Coverity 113007 Fix End */
    //code removed SPR 20928 
    /* COV_FIX_18_JUN */
    oam_system(mv_cmd); 
    oam_pm_rrm_xml_file_close();	
    OAM_FUN_EXIT_TRACE();

}


void
	oam_pm_pdcp_read_linked_list_kpi_file_dummy
(
 UInt16 *pdcp_str_count  
 )
{
	OAM_FUN_ENTRY_TRACE();
	oam_counter_t loop_counter   = OAM_ZERO;
	SInt16 dummy_val = M_OAM_ONE;

	for(loop_counter = OAM_ZERO;((loop_counter < MAX_QCI) && ((*pdcp_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
	{
		oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],"%d",dummy_val );
	}
	if((*pdcp_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],"%d",dummy_val );
	}
	/*spr 19296 fix start*/
	/*code deleted*/
	/*spr 19296 fix end*/

	for(loop_counter = OAM_ZERO;((loop_counter < MAX_QCI) && ((*pdcp_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
	{
		oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],"%d",dummy_val );
	}
	/* SPR 19335 start */
	if((*pdcp_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],"%d",dummy_val );
	}
	if((*pdcp_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],"%d",dummy_val );
	}
/* SPR 19335 end */

	OAM_FUN_EXIT_TRACE();
}

/****************************************************************************
 * Function Name  : oam_pm_pdcp_read_linked_list_kpi_file 
 * Description    : This function read pdcp list data and print into l2_kpi_log
 * Inputs         : p_oam_pdcp_kpi : pointer to structure oam_pdcp_KPI_stats_ind_t
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
	oam_pm_pdcp_read_linked_list_kpi_file
(
 oam_pdcp_KPI_stats_ind_t *p_oam_pdcp_kpi,
 UInt16 *pdcp_str_count  
 )
{
	OAM_FUN_ENTRY_TRACE();
	OAM_NULL_CHECK(OAM_NULL != p_oam_pdcp_kpi);
	oam_counter_t loop_counter   = OAM_ZERO;
	UInt32 pdcpSduDropRateDl_Sum = OAM_ZERO;
	/*spr 19296 fix start*/
	/*code deleted*/
	/*spr 19296 fix end*/
	UInt32 pdcpSduLossRateUl_sum = OAM_ZERO;

	for(loop_counter = OAM_ZERO;((loop_counter < MAX_QCI) && ((*pdcp_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
	{
		if(OAM_ZERO == p_oam_pdcp_kpi->totalSduRcvdDlperQci[loop_counter])
		{ 
			oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],"%u",OAM_ZERO );
		}
		else
		{
			/* SPR 14004 Changes Start */
			oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],"%u",((UInt64)((UInt64)p_oam_pdcp_kpi->totalDlDrbDiscardRatePerQci[loop_counter] * PM_PDCP_KPI_MULTIPLIER)/(UInt64)(p_oam_pdcp_kpi->totalSduRcvdDlperQci[loop_counter]))); 
			pdcpSduDropRateDl_Sum += ((UInt64)((UInt64)p_oam_pdcp_kpi->totalDlDrbDiscardRatePerQci[loop_counter] * PM_PDCP_KPI_MULTIPLIER)/(UInt64)p_oam_pdcp_kpi->totalSduRcvdDlperQci[loop_counter]); 
			/* SPR 14004 Changes End */
		}
	}
	/* coverity 41027 - 2 */
	if((*pdcp_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],"%u", pdcpSduDropRateDl_Sum);
	}
	/*spr 19296 fix start*/
	/*code deleted*/
	/*spr 19296 fix end*/
	for(loop_counter = OAM_ZERO;((loop_counter < MAX_QCI) && ((*pdcp_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
	{

		if(OAM_ZERO == p_oam_pdcp_kpi->totalSduRcvdUlperQci[loop_counter] ) 
		{
			oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],"%d",OAM_ZERO);
		}
		else
		{
			/* SPR 14004 Changes Start */
			oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],"%u",((UInt64)((UInt64)p_oam_pdcp_kpi->totalUlLossRatePerQci[loop_counter] * PM_PDCP_KPI_MULTIPLIER)/(UInt64)(p_oam_pdcp_kpi->totalSduRcvdUlperQci[loop_counter]))); 
			pdcpSduLossRateUl_sum += ((UInt64)((UInt64)p_oam_pdcp_kpi->totalUlLossRatePerQci[loop_counter] * PM_PDCP_KPI_MULTIPLIER)/(UInt64)(p_oam_pdcp_kpi->totalSduRcvdUlperQci[loop_counter])); 
			/* SPR 14004 Changes End */
		}
	}

	/* coverity 41027 - 1 */
	if((*pdcp_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		if (p_oam_pdcp_kpi->durationTTI)
		{
			oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],
					"%u",
					/*SPR 11402 start*/
					((p_oam_pdcp_kpi->dlCtrlPdcpSduBitRate * OAM_EIGHT)/
					 /*SPR 11402 end*/
					 (p_oam_pdcp_kpi->durationTTI))); 
		}	
		else
		{
			oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],"%d",OAM_ZERO); 
		}
	}	
	/* coverity 41027 - 2 */
	if((*pdcp_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		if (p_oam_pdcp_kpi->durationTTI)
		{
			oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],
					"%u",
					/*SPR 11402 start*/
					(p_oam_pdcp_kpi->ulCtrlPdcpSduBitRate * OAM_EIGHT)/
					/*SPR 11402 end*/
					(p_oam_pdcp_kpi->durationTTI));
		}
		else
		{
			oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],"%d",OAM_ZERO); 
		}
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

void
	oam_pm_pdcp_read_linked_list_kpi_thp_file_dummy
(
 UInt16 *pdcp_str_count 
 )
{
	OAM_FUN_ENTRY_TRACE();

	oam_counter_t loop_counter   = OAM_ZERO;
	SInt16 dummy_val = M_OAM_ONE;
     /*SPR 20682 fix+-*/
    for(loop_counter = OAM_ZERO; ((loop_counter < RRM_MAX_QCI) && ((*pdcp_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
    {
        oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],"%d",dummy_val);
    }
    for(loop_counter = OAM_ZERO; ((loop_counter < RRM_MAX_QCI) && ((*pdcp_str_count)<PM_XML_MAX_PARAM_VALUE));loop_counter++)
    {
        oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],"%d",dummy_val);
    }
     /*SPR 20682 fix+-*/

	if((*pdcp_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],"%d",dummy_val);
	}
	if((*pdcp_str_count) < PM_XML_MAX_PARAM_VALUE)
	{		
		oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],"%d",dummy_val);
	}

	OAM_FUN_EXIT_TRACE();
}

/*SPR 10023 FIX start*/
/****************************************************************************
 * Function Name  : delete_pdcp_batch_node_from_pdcp_kpi_node
 * Description    : deletes the batch nodes from PDCP KPI THP node
 * Inputs         : curr_ptr : pointer to oam_pdcp_thp_batch_node node
 * Outputs        : NONE
 * Returns        : NONE
 ****************************************************************************/
void
	delete_pdcp_batch_node_from_pdcp_kpi_node
(
 oam_pm_pdcp_kpi_info_node_t *pdcp_node
 )
{
	OAM_FUN_ENTRY_TRACE();
	oam_error_code_et error_code = NO_ERROR;
	oam_pdcp_thp_batch_node *p_ptr = OAM_NULL;
	if (pdcp_node->next_batch_node->next == OAM_NULL)
	{
		if (OAM_FAILURE == oam_mem_free((void *)pdcp_node->next_batch_node,&error_code))
		{
			OAM_LOG(OAM,OAM_ERROR,"Error in freeing memory error code %d",
					error_code);
			OAM_FUN_EXIT_TRACE();
			return;
		}
		pdcp_node->next_batch_node = OAM_NULL;
	}
	else
	{
		p_ptr = pdcp_node->next_batch_node;
		pdcp_node->next_batch_node = pdcp_node->next_batch_node->next;
		if (OAM_FAILURE == oam_mem_free((void *)p_ptr,&error_code))
		{
			OAM_LOG(OAM,OAM_ERROR,"Error in freeing memory error code %d",
					error_code);
			OAM_FUN_EXIT_TRACE();
			return;
		}
		p_ptr = OAM_NULL;
	}
	OAM_FUN_EXIT_TRACE();
}
/*SPR 10023 FIX end*/

/****************************************************************************
 * Function Name  : oam_pm_pdcp_read_linked_list_kpi_thp_file 
 * Description    :
 * Inputs         : 
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/

oam_return_et
	oam_pm_pdcp_read_linked_list_kpi_thp_file
(
 /* SPR 22296 Fix Start */
 oam_pdcp_thp_batch_node *p_oam_pdcp_kpi_thp_stats_ind,
 /* SPR 22296 Fix End */
 UInt16 *pdcp_str_count 
 )
{
	OAM_FUN_ENTRY_TRACE();

	oam_counter_t counter   = OAM_ZERO;
	/* SPR 22296 Fix Start */
	oam_pdcp_thp_batch_node *p_temp = OAM_NULL;
    /* CID 118130 Fix Start */
	oam_error_code_et error_code = NO_ERROR;
    /* CID 118126 Fix End */
	/* SPR 22296 Fix End */
	/* SPR_12419_START */
	/*spr 19296 fix start*/
	/*code deleted*/
	/*spr 19296 fix end*/

	UInt32 avgDuration[RRM_MAX_QCI] = {OAM_ZERO};
	/*CSR 81339 end*/
	/*spr 19296 fix start*/
	/*code deleted*/
	/*spr 19296 fix end*/
	/* SPR_12419_START */
	UInt64 oam_PdcpSduBitrateDl[RRM_MAX_QCI] =  {OAM_ZERO};
	UInt64 oam_PdcpSduBitrateUl[RRM_MAX_QCI] =  {OAM_ZERO};
	/* SPR_12419_END */
	/* SPR 13790 FIX START */
	UInt64 Total_oam_PdcpSduBitrateDl =  OAM_ZERO;
	UInt64 Total_oam_PdcpSduBitrateUl =  OAM_ZERO;
	/*SPR_10610_Changes +*/
	static UInt64 Max_oam_PdcpSduBitrateUl = OAM_ZERO;
	static UInt64 Max_oam_PdcpSduBitrateDl = OAM_ZERO;
	/*SPR_10610_Changes -*/
	/* SPR 13790 FIX END*/
	/*SPR 10023 FIX start*/
	/*SPR 10023 FIX end*/
	/*SPR 11402 start */
	/* SPR 22296 Fix Start */
	while (p_oam_pdcp_kpi_thp_stats_ind != OAM_NULL)
	{
		for(counter = OAM_ZERO; counter<MAX_KPI_INDEX; counter++)
		{
			if(OAM_ZERO < p_oam_pdcp_kpi_thp_stats_ind->data.qci[counter] && (p_oam_pdcp_kpi_thp_stats_ind->data.qci[counter] <= RRM_MAX_QCI))
			{
				/*CSR 81339 start*/
				OAM_LOG(OAM, OAM_DETAILED, "Received QCI[%d] entry valid from PDCP ", p_oam_pdcp_kpi_thp_stats_ind->data.qci[counter]);
				/* SPR_12419_START */
				/*spr 19296 fix start*/
				/*code deleted*/
				/*spr 19296 fix end*/
                /* SPR 20961 Fix Start */
				if(INVALID_THP_STATS_ENTRY != p_oam_pdcp_kpi_thp_stats_ind->data.pdcpSduBitDl[counter])
				{
					oam_PdcpSduBitrateDl[p_oam_pdcp_kpi_thp_stats_ind->data.qci[counter] -1] += p_oam_pdcp_kpi_thp_stats_ind->data.pdcpSduBitDl[counter];
				}
				if(INVALID_THP_STATS_ENTRY != p_oam_pdcp_kpi_thp_stats_ind->data.pdcpSduBitUl[counter])
                /* SPR 20961 Fix End */
				{
					oam_PdcpSduBitrateUl[p_oam_pdcp_kpi_thp_stats_ind->data.qci[counter] -1 ] += p_oam_pdcp_kpi_thp_stats_ind->data.pdcpSduBitUl[counter];
				}   
				/* SPR_12648_FIX_START */
				avgDuration[p_oam_pdcp_kpi_thp_stats_ind->data.qci[counter] - 1] = 
					p_oam_pdcp_kpi_thp_stats_ind->data.avgDurationT;
				/* SPR_12648_FIX_END */
				/*SPR 11402 start */ 
			}	
			else
			{
				OAM_LOG(OAM, OAM_INFO, "Invalid Entry for QCI[%u] for counter[%u] while reading pdcp kpi thp stats ",
						p_oam_pdcp_kpi_thp_stats_ind->data.qci[counter], counter);
				break;
			}
		}
		p_temp = p_oam_pdcp_kpi_thp_stats_ind;
		p_oam_pdcp_kpi_thp_stats_ind = p_oam_pdcp_kpi_thp_stats_ind->next;
		if(OAM_NULL != p_temp)
		{
			oam_mem_free((void *)p_temp, &error_code);
		}
	}
	/* SPR 22296 Fix End */

	/*spr 19296 fix start*/
	/*code deleted*/
	/*spr 19296 fix end*/

	//DRB.PdcpSduBitrateDl.QCI    
	/* coverity 41028 - 1 */
	for(counter = OAM_ZERO; ((counter < RRM_MAX_QCI) && ((*pdcp_str_count)<PM_XML_MAX_PARAM_VALUE));counter++)
	{	
		/*SPR 11402 start*/
		if (avgDuration[counter] != OAM_ZERO)
		{
			/* SPR 13790 FIX START */    
			oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],
					"%llu",
					((oam_PdcpSduBitrateDl[counter] * OAM_EIGHT) / avgDuration[counter]));
			Total_oam_PdcpSduBitrateDl += ((oam_PdcpSduBitrateDl[counter] * OAM_EIGHT) / avgDuration[counter]);
			/* SPR 13790 FIX END */    
		}
		else
		{
			oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++], "%u", OAM_ZERO); 
		}
		/*SPR 11402 end*/
		/* SPR 13790 FIX START */    
		/* Total_oam_PdcpSduBitrateDl += oam_PdcpSduBitrateDl[counter]; */
		/* SPR 13790 FIX END */    
		oam_PdcpSduBitrateDl[counter] = OAM_ZERO;
	}	
	/*SPR_10610_Changes +*/
	if(Max_oam_PdcpSduBitrateDl < Total_oam_PdcpSduBitrateDl)
	{
		Max_oam_PdcpSduBitrateDl = Total_oam_PdcpSduBitrateDl;
	}
	/*SPR_10610_Changes -*/


	//DRB.PdcpSduBitrateUl.QCI    
	/* coverity 41028 - 2 */
	for(counter = OAM_ZERO; ((counter < RRM_MAX_QCI) && ((*pdcp_str_count)<PM_XML_MAX_PARAM_VALUE));counter++)
	{	
		/*SPR 11402 start*/
		if (avgDuration[counter] != OAM_ZERO)
		{
			/* SPR 13790 FIX START */    
			oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],
					"%llu",
					((oam_PdcpSduBitrateUl[counter] * OAM_EIGHT) / avgDuration[counter]));
			Total_oam_PdcpSduBitrateUl += ((oam_PdcpSduBitrateUl[counter] * OAM_EIGHT) / avgDuration[counter]);
			/* SPR 13790 FIX END */    

		}
		else
		{
			oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++], "%u", OAM_ZERO);
		}
		/*SPR 11402 end*/
		/* SPR 13790 FIX START */    
		/* Total_oam_PdcpSduBitrateUl += oam_PdcpSduBitrateUl[counter]; */
		/* SPR 13790 FIX END */    
		oam_PdcpSduBitrateUl[counter] = OAM_ZERO;
	}	
	/*SPR_10610_Changes +*/
	if(Max_oam_PdcpSduBitrateUl < Total_oam_PdcpSduBitrateUl)
	{
		Max_oam_PdcpSduBitrateUl = Total_oam_PdcpSduBitrateUl;
	}
	/*SPR_10610_Changes -*/


	//Maintain max value and print the same
	//DRB.PdcpSduBitrateDlMax
	/* coverity 41028 -1, 41028 -2*/
	if((*pdcp_str_count)<PM_XML_MAX_PARAM_VALUE)
	{
		/*SPR_10610_Changes +*/
		/* SPR 13790 FIX START */    
		oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],"%llu",Max_oam_PdcpSduBitrateDl);
		/* SPR 13790 FIX END */    
		/*SPR_10610_Changes -*/
	}
	/* coverity 41028 431206*/
	if((*pdcp_str_count) < PM_XML_MAX_PARAM_VALUE)
	{		
		//DRB.PdcpSduBitrateUlMax
		/*SPR_10610_Changes +*/
		/* SPR 13790 FIX START */    
		oam_sprintf((char *)pm_xml_param_value[(*pdcp_str_count)++],"%llu",Max_oam_PdcpSduBitrateUl);
		/* SPR 13790 FIX END */    
		/*SPR_10610_Changes -*/
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_pm_rrm_read_linked_list_kpi_file
 * Description    : 
 * Inputs         : p_oam_pm_rrm_kpi_ind
 * Outputs        : 
 * Returns        : OAM_SUCCESS 
 ****************************************************************************/
oam_return_et
	oam_pm_rrm_read_linked_list_kpi_file
(
 oam_rrm_kpi_ind_t *p_oam_pm_rrm_kpi_ind
 )
{
	OAM_FUN_ENTRY_TRACE();
	UInt16 str_count = OAM_ZERO;

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_admitted_csg_user);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_admitted_non_csg_user);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_ue_admission_success);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_ue_admission_fail);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_erb_setup_success);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_erb_setup_fail);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_erb_modify_success);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_erb_modify_fail);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_erb_release_success);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_erb_release_fail);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).avg_dl_allocated_gbr_prb);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).avg_ul_allocated_gbr_prb);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).dl_allocated_ngbr_prb);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).ul_allocated_ngbr_prb);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_geran_ho_attempt);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_geran_ho_fail);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_utran_ho_attempt);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_utran_ho_fail);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_eutran_ho_attempt);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_eutran_ho_fail);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_geran_hi_success);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_geran_hi_fail);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_utran_hi_success);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_utran_hi_fail);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_eutran_hi_success);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_eutran_hi_fail);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_enb_init_ho_csg_usr);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_enb_init_ho_non_csg_usr);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_enb_init_ue_release);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_pucch_res_alloc_attempts);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_sr_res_alloc_fail);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_sr_cqi_alloc_fail);

	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).total_cell_unavailable_time);
	/* BUG_FIXED_12710 */ 
	oam_sprintf((char *)pm_rrm_xml_param_value[str_count++],"%d",(p_oam_pm_rrm_kpi_ind->kpi_data).num_of_ca_ue);

	/* BUG_FIXED_12710 */ 

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;    

}
/****************************************************************************
 * Function Name  : fill_plmn_id_from_mcc_mnc 
 * Description    : function fill plmn id from mcc n from mnc
 * Inputs         :
 * Outputs        : plmn id
 * Returns        : void 
 ****************************************************************************/
	void fill_plmn_id_from_mcc_mnc
(
 UInt8 out_plmn_id[],
 plmn_identity_t in_plmn_id
 )
{
	UInt8 count = OAM_ZERO;
	oam_sprintf((char *)&out_plmn_id[count++],"%d",in_plmn_id.mcc[1]);

	oam_sprintf((char *)&out_plmn_id[count++],"%d",in_plmn_id.mcc[0]);

	if(in_plmn_id.mnc.count == OAM_THREE)
		oam_sprintf((char *)&out_plmn_id[count++],"%d",in_plmn_id.mnc.mnc[2]);

	oam_sprintf((char *)&out_plmn_id[count++],"%d",in_plmn_id.mcc[2]);
	oam_sprintf((char *)&out_plmn_id[count++],"%d",in_plmn_id.mnc.mnc[1]);
	oam_sprintf((char *)&out_plmn_id[count++],"%d",in_plmn_id.mnc.mnc[0]);
}

/* SPR_12747_CHANGES */
/****************************************************************************
 * Function Name  : oam_pm_write_ho_related_params_to_file 
 * Description    : Function write ho related params into pm xml file 
 * Inputs         : NONE
 * Outputs        : NONE
 * Returns        : void 
 ****************************************************************************/
void oam_pm_write_ho_related_params_to_file(UInt16 count)
{
    char temp_buff[OAM_TWENTY_FIVE] = {OAM_ZERO};
    /* KlockWork_Fix */ 
    UInt32 counter = OAM_ZERO;
    /* KlockWork_Fix */ 
    UInt16 index = OAM_ZERO;
    /* SPR 22296 Fix Start */
    xmlChar num_buff[STR_LEN_256] = {OAM_ZERO}; 
    oam_memset((void *)num_buff, OAM_ZERO, sizeof(num_buff));
    /* SPR 22296 Fix End */
    if(OAM_ZERO == file_close.flag)
    {
        /* SPR 17176 changes start */
        oam_write_xml_measInfo(endTime_buffer);
        xmlTextWriterStartElement(writer, (xmlChar *)"measValue");
        /* SPR 22296 Fix Start */
        oam_snprintf((char *)num_buff,(sizeof(num_buff)-OAM_ONE),(char *)"Cell=%s,EUtranRelation=%s",temp_buff_mcc_n_mnc,temp_target_str);
        xmlTextWriterWriteAttribute(writer, (xmlChar *)"measObjLdn",num_buff);
        /* SPR 22296 Fix End */
        /* SPR 17176 changes end */
        /* SPR_12640_FIX_START */ 
        /* SPR 18861 */
        for(index = OAM_ZERO; index < g_pm_xml_total_element_count_offset ;index++) 
        /* SPR 18861 */
        /* SPR_12640_FIX_END */ 
        {
            OAM_LOG(OAM, OAM_DETAILED, "Index while writing into xml file = %d",index );
            oam_sprintf(temp_buff,"%d",index+OAM_ONE);
            xmlTextWriterStartElement(writer, (xmlChar *)"r");
            xmlTextWriterWriteAttribute(writer, (xmlChar *)"p",(xmlChar *)temp_buff);
            /* KlockWork_Fix */ 
            /* SPR 18861 */
            counter = index  + (count * g_pm_xml_total_element_count_offset);
            /* SPR 18861 */

            /*SPR 20171 Start*/
            OAM_LOG(OAM, OAM_DETAILED, "Cell id = %d Index while writing into xml file = %d "
                    "taken from pm_xml offset %d "
                    "g_pm_xml_total_element_count_offset %d ",
                    count, index, counter, g_pm_xml_total_element_count_offset );
            /*SPR 20171 End*/

            if(counter < PM_XML_MAX_PARAM_VALUE)
            /*Coverity Fix 110761 start */
            {
                xmlTextWriterWriteString(writer,(xmlChar *)pm_xml_param_value[counter]);

                /*SPR 20171 Start*/
                OAM_LOG(OAM, OAM_DETAILED, "Cell id = %d Index while writing into xml file = %d "
                        "taken from pm_xml offset %d "
                        "g_pm_xml_total_element_count_offset %d Value written %s ",
                        count, index, counter, g_pm_xml_total_element_count_offset,pm_xml_param_value[counter] );
                /*SPR 20171 End*/
            }
            /*Coverity Fix 110761 end */

            /* KlockWork_Fix */ 
            xmlTextWriterEndElement(writer);
        }
        /*SPR 22537 Fix Start*/
        /*Code Removed*/
        /*SPR 22537 Fix Stop*/
    }
}
/* SPR_12747_CHANGES */

/* SPR_12747_CHANGES */
/****************************************************************************
 * Function Name  : oam_pm_rrc_read_linked_list_kpi_file 
 * Description    :
 * Inputs         : 
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
/* SPR 17777 fix */
oam_return_et
	oam_pm_rrc_read_linked_list_kpi_file
(
 oam_pm_rrc_kpi_info_Node_t *oam_pm_rrc_kpi_node
 )
{

	OAM_FUN_ENTRY_TRACE();
    /* SPR 23209 FIXED START*/
    UInt16 loop_counter_2 = OAM_ZERO;
    /* SPR 23209 FIXED END*/

	UInt16 rrc_str_count = OAM_ZERO; 
	SInt8 ret = OAM_ZERO; 
	oam_counter_t loop_counter   = OAM_ZERO;
	oam_counter_t loop_counter1   = OAM_ZERO;
	UInt8 index = OAM_ZERO;

    /* SPR 14504 FIX START */
    /*code removed*/ 
    /* SPR 14504 FIX END */

    /*SPR 20171 code deleted */ 
	/* SPR 15018 START */
	oam_memcpy(endTime_buffer,oam_pm_rrc_kpi_node->timeStamp, 100);
	/* SPR 15018 END */

	for(loop_counter = OAM_ZERO; (loop_counter<oam_pm_rrc_kpi_node->rrc_connection_stats_ind.count) ;loop_counter++)
	{  
		/* SPR 18861 */
		rrc_str_count = (g_pm_xml_total_element_count_offset* loop_counter) + PM_XML_CONN_ST_PARAM_START; 
        /*SPR 20171 Start*/

        OAM_LOG(OAM, OAM_DETAILED, "Thread Id :[%lu] Start Writing RRC parameters at offset %d g_pm_xml_total_element_count_offset =%d "
                "PM_XML_CONN_ST_PARAM_START %d "
                "for cell id = %d ", pthread_self(),rrc_str_count,g_pm_xml_total_element_count_offset,PM_XML_CONN_ST_PARAM_START,loop_counter);
        /*SPR 20171 End*/


		/* SPR 18861 */
		/*index - 0 ->emergency,
		  index - 1 ->highPriorityAccess,
		  index - 2 ->mt-Access,
		  index - 3 ->mo-Signalling,
		  index - 4 ->mo-Data */
		/* Logic to be written for selecting the cell by getting cell index  */
		/*[bug 2] Description: wrong index was passed to oam_rrm_cell_config_req*/
		ret = oam_get_rrm_cell_config_idx(oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].cell_index);
		/* Cov 63646/63654/63463/63648/63653  Fix */
		if (ret == OAM_ERROR_RETURN_VAL) 
		{
			OAM_LOG(OAM,OAM_INFO,"Stats received for unconfigured cell_index %d", ret);
			return OAM_FAILURE;
		}
		/* Cov 63646/63654/63463/63648/63653  Fix */

		pm_xml_cell_id_to_read = oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].cell_index;
		oam_sprintf((char *)temp_buff_mcc_n_mnc,"%s-%d",
				oam_prov_req.oam_igd_params.cell_config_params.PLMNID,
				oam_convert_byte_to_int(oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req[ret].global_cell_info.
					eutran_global_cell_id.cell_identity));
        /*SPR 20171 Start*/

        OAM_LOG(OAM, OAM_DETAILED, "cell_index receieved from RRC : %d mcc_mnc %s", pm_xml_cell_id_to_read,temp_buff_mcc_n_mnc);
        /*SPR 20171 End*/

		/* SPR 18861 */
		//RRC.ConnEstabAtt.Cause

		/* SPR 18861 */
		//index - 0 ->emergency,
		//index - 1 ->highPriorityAccess,
		//index - 2 ->mt-Access,
		//index - 3 ->mo-Signalling,
		//index - 4 ->mo-Data
		/* SPR 18861 */
		//index - 5 ->delayTolerantAccess-v1020
		//index - 6 ->spare2
		//index - 7 ->spare1
		/* SPR 18861 */
		if (oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].bitmask 
				& RRC_OAM_STATISTICS_CONNECTION_ESTB_IND_STATS_PRESENT)
		{
			/* SPR 18861 */
			OAM_LOG(OAM, OAM_DETAILED, "number of causes to write to xml for Conn Est and Succ : %d", RRC_ESTABLISHMENT_CAUSE_INDEX);
			/* SPR 18861 */
				/* SPR 18861 */
				for(index = OAM_ZERO; (index <RRC_ESTABLISHMENT_CAUSE_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE);  index++)
					/* SPR 18861 */
				{
                    /*SPR 20171 Start*/
                    OAM_LOG(OAM, OAM_DETAILED, "For cell id = %d Writing RRC Params at offset %d rrc_connEstabAtt_cause[cause%d] = %d "
                            "in hex = 0x%x ",
                            loop_counter,rrc_str_count,
                            index,
                            oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrc_connection_establishment.
                            rrc_connEstabAtt_cause[index],
                            oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrc_connection_establishment.
                            rrc_connEstabAtt_cause[index]					
                           );


                    /*SPR 20171 End*/

					oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
							oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].
							rrc_connection_establishment.rrc_connEstabAtt_cause[index]);

                    /*SPR 20171 Start*/
                    OAM_LOG(OAM, OAM_DETAILED, "For cell id = %d Writing RRC Params at offset %d rrc_connEstabAtt_cause[cause%d] = %d "
                            "in hex = 0x%x ",
                            loop_counter,rrc_str_count,
                            index,
                            oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrc_connection_establishment.
                            rrc_connEstabAtt_cause[index],
                            oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrc_connection_establishment.
                            rrc_connEstabAtt_cause[index]					
                           );


                    /*SPR 20171 End*/

				}
			/* SPR 18861 */
			//RRC.ConnEstabAtt.Sum
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].
						rrc_connection_establishment.rrc_connEstabAtt_sum);

			//RRC.ConnEstabSucc.Cause

			/* SPR 18861 */
			//index - 0 ->emergency,
			//index - 1 ->highPriorityAccess,
			//index - 2 ->mt-Access,
			//index - 3 ->mo-Signalling,
			//index - 4 ->mo-Data
			/* SPR 18861 */
			//index - 5 ->delayTolerantAccess-v1020
			//index - 6 ->spare2
			//index - 7 ->spare1
			for(index = OAM_ZERO;(index < RRC_ESTABLISHMENT_CAUSE_INDEX)&& (rrc_str_count < PM_XML_MAX_PARAM_VALUE) ;index++)
				/* SPR 18861 */
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].
						rrc_connection_establishment.
						rrc_connEstabSucc_cause[index]);
			}
			/* SPR 18861 */
			//RRC.ConnEstabSucc.Sum
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].
						rrc_connection_establishment.rrc_connEstabSucc_sum);


			//RRC.ConnEstabFail.Cause

			for(index = OAM_ZERO;(index < RRC_ESTABLISHMENT_CAUSE_INDEX)&& (rrc_str_count < PM_XML_MAX_PARAM_VALUE) ;index++)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].
						rrc_connection_establishment.
						rrc_connEstabFail_cause[index]);
			}
			//RRC.ConnEstabFail.Sum

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].
						rrc_connection_establishment.rrc_connEstabFail_sum);
			}

			/* SPR 18861 */
			//RRC.ConnEstabFaileNBCause.Congestion

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].
						rrc_connection_establishment.
						rrc_ConnEstabFaileNBCause_Congestion);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//RRC.ConnEstabFaileNBCause.Unspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].
						rrc_connection_establishment.
						rrc_connEstabFaileNBCause_Unspecified);
			}
			/*SPR 10987 start*/
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//RRC.ConnEstabFaileNBCause.EnergySaving.
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].
						rrc_connection_establishment.
						rrc_ConnEstabFaileNBCause_EnergySaving);
			}
			/*SPR 10987 end*/
            /*SPR 20171 Start*/

            OAM_LOG(OAM,OAM_DETAILEDALL,"RRC.ConnEstabFaileNBCause.EnergySaving.=%s written at index %d ",
                    pm_xml_param_value[rrc_str_count-1], rrc_str_count-1);

            /*SPR 20171 End*/

		}
		else
		{
            //RRC.ConnEstabAtt.Cause /*SPR 20171 Missing Counter*/
			//index - 0 ->emergency,
			//index - 1 ->highPriorityAccess,
			//index - 2 ->mt-Access,
			//index - 3 ->mo-Signalling,
			//index - 4 ->mo-Data
			/* SPR 18861 */
			//index - 5 ->delayTolerantAccess-v1020
			//index - 6 ->spare2
			//index - 7 ->spare1
            /*SPR 20171 Start */
            OAM_LOG(OAM, OAM_DETAILEDALL, "For cell id = %d  RRC_OAM_STATISTICS_CONNECTION_ESTB_IND_STATS_PRESENT not there ",
                    loop_counter);
            /*SPR 20171 End */
			for(index = OAM_ZERO; (index <RRC_ESTABLISHMENT_CAUSE_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE);  index++)
				/* SPR 18861 */
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",OAM_ZERO);
			}
            //RRC.ConnEstabAtt.Sum /*SPR 20171 Start*/
            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",OAM_ZERO);
            //RRC.ConnEstabSucc.Cause /*SPR 20171 End */
			//index - 0 ->emergency,
			//index - 1 ->highPriorityAccess,
			//index - 2 ->mt-Access,
			//index - 3 ->mo-Signalling,
			//index - 4 ->mo-Data
			/* SPR 18861 */
			//index - 5 ->delayTolerantAccess-v1020
			//index - 6 ->spare2
			//index - 7 ->spare1
			for(index = OAM_ZERO;(index < RRC_ESTABLISHMENT_CAUSE_INDEX)&& (rrc_str_count < PM_XML_MAX_PARAM_VALUE) ;index++)
				/* SPR 18861 */
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						OAM_ZERO);
			}
            /*SPR 20171 New counters added start */

            //RRC.ConnEstabSucc.Sum 
            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",OAM_ZERO);

            //RRC.ConnEstabFail.Cause 

            for(index = OAM_ZERO;(index < RRC_ESTABLISHMENT_CAUSE_INDEX)&& (rrc_str_count < PM_XML_MAX_PARAM_VALUE) ;index++)
            {
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
                        OAM_ZERO);
            }
            //RRC.ConnEstabFail.Sum 

            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",OAM_ZERO);
            /*SPR 20171 New counters added End */

			//RRC.ConnEstabFaileNBCause.Congestion

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//RRC.ConnEstabFaileNBCause.Unspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//RRC.ConnEstabFaileNBCause.EnergySaving.
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						OAM_ZERO);
			}
            /*SPR 20171 Start */
            OAM_LOG(OAM,OAM_DETAILEDALL,"RRC.ConnEstabFaileNBCause.EnergySaving.=%s written at index %d ",
                    pm_xml_param_value[rrc_str_count-1], rrc_str_count-1);
            /*SPR 20171 End */

		}
		if (oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].bitmask 
				& RRC_OAM_STATISTICS_CONNECTION_RE_ESTB_STATS_PRESENT)
		{
			//index - 0 ->ReconfigurationFailure
			//index - 1 ->HandoverFailure
			//index - 2 ->OtherFailure
			for(index = OAM_ZERO; (index < 3) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE);index++)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrc_connection_reestablishment.
						rrc_connReEstabAtt_cause[index]);
			}
			/*SPR 10987 start*/
			/*rework start*/
			//index - 0 ->ReconfigurationFailure
			//index - 1 ->HandoverFailure
			//index - 2 ->OtherFailure
			for(index = OAM_ZERO; (index < 3)&& (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrc_connection_reestablishment.rrc_connReEstabSucc_cause[index]);
			}
			/* coverity 64326 */
			//RRC.ConnReEstabAtt.Sum
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{

				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.
						rrc_connection[loop_counter].
						rrc_connection_reestablishment.rrc_connReEstabAtt_sum);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//RRC.ConnReEstabSucc.Sum
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.
						rrc_connection[loop_counter].
						/*SPR 12844 FIX START*/
						rrc_connection_reestablishment.rrc_connReEstabSucc_sum);
				/*SPR 12844 FIX START*/
			}
			/*rework end*/
			/*SPR 10987 end*/

			//index - 0 ->ReconfigurationFailure
			//index - 1 ->HandoverFailure
			//index - 2 ->OtherFailure
			for(index = OAM_ZERO; (index < 3)&& (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrc_connection_reestablishment.rrc_connReEstabFail_cause[index]);
			}

			/*SPR 10987 start*/
			//RRC.ConnReEstabFail.Sum
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrc_connection_reestablishment.rrc_connReEstabFail_sum);
				/*SPR 10987 end*/
			}
            /*SPR 20171 Start */
            OAM_LOG(OAM,OAM_DETAILEDALL,"RRC.ConnReEstabFail.Sum =%s written at index %d ",
                    pm_xml_param_value[rrc_str_count-1], rrc_str_count-1);
            /*SPR 20171 End */


		}
		else
		{
            /*SPR 20171 Start */

            OAM_LOG(OAM, OAM_INFO, "For cell id = %d  RRC_OAM_STATISTICS_CONNECTION_RE_ESTB_STATS_PRESENT not there ",
                    loop_counter);
            /*SPR 20171 End */

			for(index = OAM_ZERO; (index < 12) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE);index++)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO); 
			}
            /*SPR 20171 Start */

            OAM_LOG(OAM,OAM_DETAILEDALL,"RRC.ConnReEstabFail.Sum =%s written at index %d ",
                    pm_xml_param_value[rrc_str_count-1], rrc_str_count-1);
            /*SPR 20171 End */

		}
		//rrc_connEstabSucc_sum  shall be both mean and max in our case
		//RRC.ConnMean
		/* SPR_12641_FIX_START */ 
		if(g_pm_xml_params.rrc_params.RRC_ConnMax[loop_counter] < 
				oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrcConnectionNumber)
		{
			g_pm_xml_params.rrc_params.RRC_ConnMax[loop_counter] = 
				oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrcConnectionNumber;
		}

		/* SPR_12641_FIX_END */ 
		if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
		{
			/*RRC.ConnMean*/
			oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", 
					(oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrcConnectionNumber));
		}

		if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
		{
			/* SPR 13093 START */
			//RRC.ConnMax
			oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
					g_pm_xml_params.rrc_params.RRC_ConnMax[loop_counter]);
			/* SPR 13093 END */
		}

		if (oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].bitmask 
				& RRC_OAM_STATISTICS_CONNECTION_SETUP_TIME_IND_STATS_PRESENT)
		{
            //RRC.ConnEstabTimeMean.Cause /*SPR 20171*/  
			//index - 0 ->emergency,
			//index - 1 ->highPriorityAccess,
			//index - 2 ->mt-Access,
			//index - 3 ->mo-Signalling,
			//index - 4 ->mo-Data
			/* SPR 18861 */
			//index - 5 ->delayTolerantAccess-v1020
			//index - 6 ->spare2
			//index - 7 ->spare1
			for(index = OAM_ZERO; (index < RRC_ESTABLISHMENT_CAUSE_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
				/* SPR 18861 */
			{ 
				if(OAM_ZERO != oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrc_connEstabTime_cause[index].count)  
				{ 
					oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
							oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrc_connEstabTime_cause[index].rrc_connEstabTime_Sum/
							oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrc_connEstabTime_cause[index].count);
					/* SPR 18861 */

					OAM_LOG(OAM,OAM_DETAILEDALL,"For cause %d got estabTimeSum = %d count %d and so mean %d ",
							index,
							oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrc_connEstabTime_cause[index].rrc_connEstabTime_Sum,
							oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrc_connEstabTime_cause[index].count,
							oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrc_connEstabTime_cause[index].rrc_connEstabTime_Sum/
							oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrc_connEstabTime_cause[index].count
					       );
					/* SPR 18861 */
				}
				else
				{

					oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
							oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrc_connEstabTime_cause[index].rrc_connEstabTime_Sum);
				}  
			}
            //RRC.ConnEstabTimeMax.Cause  /*SPR 20171*/
			//index - 0 ->emergency,
			//index - 1 ->highPriorityAccess,
			//index - 2 ->mt-Access,
			//index - 3 ->mo-Signalling,
			//index - 4 ->mo-Data
			/* SPR 18861 */
			//index - 5 ->delayTolerantAccess-v1020
			//index - 6 ->spare2
			//index - 7 ->spare1
			for(index = OAM_ZERO; (index < RRC_ESTABLISHMENT_CAUSE_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
				/* SPR 18861 */
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrc_connEstabTime_cause[index].rrc_connEstabTime_Max);
				/* SPR 18861 */
				OAM_LOG(OAM,OAM_DETAILEDALL,"For cause %d got estabTimeMax = %d  ",
						index,
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].rrc_connEstabTime_cause[index].rrc_connEstabTime_Max
				       );
				/* SPR 18861 */
			}
            /*SPR 20171 Start */

            OAM_LOG(OAM,OAM_DETAILEDALL,"RRC.ConnEstabTimeMax.Cause =%s written at index %d ",
                    pm_xml_param_value[rrc_str_count-1], rrc_str_count-1);
            /*SPR 20171 End*/

		}
		else
		{
            /*SPR 20171 Start */

            OAM_LOG(OAM, OAM_DETAILEDALL, "For cell id = %d  RRC_OAM_STATISTICS_CONNECTION_SETUP_TIME_IND_STATS_PRESENT not there ",
                    loop_counter);
            /*SPR 20171 End */

            //RRC.ConnEstabTimeMean.Cause /*SPR 20171*/
			//index - 0 ->emergency,
			//index - 1 ->highPriorityAccess,
			//index - 2 ->mt-Access,
			//index - 3 ->mo-Signalling,
			//index - 4 ->mo-Data
			/* SPR 18861 */
			//index - 5 ->delayTolerantAccess-v1020
			//index - 6 ->spare2
			//index - 7 ->spare1
			for(index = OAM_ZERO; (index < RRC_ESTABLISHMENT_CAUSE_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
				/* SPR 18861 */
			{ 
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",OAM_ZERO);
			}
            //RRC.ConnEstabTimeMax.Cause /*SPR 20171*/
			//index - 0 ->emergency,
			//index - 1 ->highPriorityAccess,
			//index - 2 ->mt-Access,
			//index - 3 ->mo-Signalling,
			//index - 4 ->mo-Data
			/* SPR 18861 */
			//index - 5 ->delayTolerantAccess-v1020
			//index - 6 ->spare2
			//index - 7 ->spare1
			for(index = OAM_ZERO; (index < RRC_ESTABLISHMENT_CAUSE_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
				/* SPR 18861 */
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",OAM_ZERO);
			}
            /*SPR 20171 Start */

            OAM_LOG(OAM,OAM_DETAILEDALL,"RRC.ConnEstabTimeMax.Cause =%s written at index %d ",
                    pm_xml_param_value[rrc_str_count-1], rrc_str_count-1);
            /*SPR 20171 End */

		}
		if (oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].bitmask 
				& RRC_OAM_STATISTICS_UE_CONTEXT_RELEASE_IND_STATS_PRESENT)
		{
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.Sum
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_sum);
			}

			/*SPR 10987 start*/
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.Unspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_unspecified_2]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.Tx2RelocOverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_tx2relocoverall_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.SuccessfulHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_successful_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.ReleaseDueToEutranGeneratedReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_release_due_to_eutran_generated_reason]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.HandoverCancelled
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_handover_cancelled]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.PartialHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_partial_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.HoFailureInTargetEPCeNBOrTargetSystem
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_ho_failure_in_target_EPC_eNB_or_target_system]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.HoTargetNotAllowed
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_ho_target_not_allowed]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.TS1RelocOverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_tS1relocoverall_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.TS1RelocPrepExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_tS1relocprep_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.CellNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_cell_not_available]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.UnknownTargetID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_unknown_targetID]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.NoRadioResourcesAvailableInTargetCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_no_radio_resources_available_in_target_cell]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.UnknownMmeUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_unknown_mme_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.UnknownEnbUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_unknown_enb_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/
				//UECNTX.RelReq.UnknownPairUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_unknown_pair_ue_s1ap_id]);
			}			
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
                /*SPR 19596 End*/
			{
				//UECNTX.RelReq.HandoverDesirableForRadioReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_handover_desirable_for_radio_reason]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.TimeCriticalHandover,
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_time_critical_handover]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.ResourceOptimisationHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_resource_optimisation_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.ReduceLoadInServingCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_reduce_load_in_serving_cell]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/
                //UECNTX.RelReq.UserInactivity
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_user_inactivity]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 End*/
				//UECNTX.RelReq.RadioConnectionWithUeLost
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_radio_connection_with_ue_lost]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.LoadBalancingTauRequired
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_load_balancing_tau_required]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.CsFallbackTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_cs_fallback_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.UeNotAvailableForPsService
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_ue_not_available_for_ps_service]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.RadioResourcesNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_radio_resources_not_available]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.FailureInRadioInterfaceProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_failure_in_radio_interface_procedure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.InvalidQosCombination
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_invalid_qos_combination]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.InterratRedirection
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_interrat_redirection]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.InteractionWithOtherProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_interaction_with_other_procedure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.UnknownERABID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_unknown_E_RAB_ID]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.MultipleERABIDInstances
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_multiple_E_RAB_ID_instances]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_encryption_and_or_integrity_protection_algorithms_not_supported]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.S1IntraSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_s1_intra_system_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.S1InterSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_s1_inter_system_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.X2HandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_radioNetwork[s1ap_x2_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.TransportResourceUnavailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_transport[s1ap_transport_resource_unavailable]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.TransportUnspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_transport[s1ap_unspecified_3]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.NormalRelease
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_nas[normal_release]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.AuthenticationFailure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_nas[authentication_failure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.Detach
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_nas[detach]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.Unspecified4
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_nas[unspecified_4]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.TransferSyntaxError
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_protocol[transfer_syntax_error]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.AbstractSyntaxErrorReject
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_protocol[abstract_syntax_error_reject]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.AbstractSyntaxErrorIgnoreAndNotify
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_protocol[abstract_syntax_error_ignore_and_notify]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.MessageNotCompatibleWithReceiverState
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_protocol[message_not_compatible_with_receiver_state]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.SemanticError
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_protocol[semantic_error]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.AbstractSyntaxErrorFalselyConstructedMessage
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_protocol[abstract_syntax_error_falsely_constructed_message]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.Unspecified_1
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_protocol[unspecified_1]);
			}
			/*SPR 10987 end*/
            /*SPR 19596 Start*/
			/*SPR 10987 start*/
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.ControlProcessingOverload
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_misc[s1ap_control_processing_overload]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.NotEnoughUserPlaneProcessingResources
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_misc[s1ap_not_enough_user_plane_processing_resources]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.HardwareFailure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_misc[s1ap_hardware_failure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.OMIntervention
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_misc[s1ap_om_intervention]);
			}
            /*SPR 19596 End*/

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.S1APUnspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_misc[s1ap_unspecified]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.S1APUnknownPLMN
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.
						ueCtxt_relReq_cause.ueCtxt_relReq_cause_misc[s1ap_unknown_PLMN]);
			}
			/*SPR 10987 end*/
			//UECNTX. RelSuccNbr
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->rrc_connection_stats_ind.rrc_connection[loop_counter].ue_contxt_rel_stats.ueContext_RelSuccNbr );
			}
            /*SPR 20171 Start */
            OAM_LOG(OAM,OAM_DETAILEDALL,"UECNTX. RelSuccNbr =%s written at index %d ",
                    pm_xml_param_value[rrc_str_count-1], rrc_str_count-1);
            /*SPR 20171 End */

		}
		else
		{
            /*SPR 20171 Start */

            OAM_LOG(OAM, OAM_DETAILEDALL, "For cell id = %d  RRC_OAM_STATISTICS_UE_CONTEXT_RELEASE_IND_STATS_PRESENT not there ",
                    loop_counter);
            /*SPR 20171 end */

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.Sum
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 20171 Start */
                //code deleted
                /*SPR 20171 End */
				//UECNTX.RelReq.Unspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.Tx2RelocOverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.SuccessfulHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.ReleaseDueToEutranGeneratedReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.HandoverCancelled
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.PartialHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.HoFailureInTargetEPCeNBOrTargetSystem
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.HoTargetNotAllowed
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.TS1RelocOverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.TS1RelocPrepExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.CellNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.UnknownTargetID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.NoRadioResourcesAvailableInTargetCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.UnknownMmeUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.UnknownEnbUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.HandoverDesirableForRadioReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.TimeCriticalHandover,
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.ResourceOptimisationHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.ReduceLoadInServingCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.RadioConnectionWithUeLost
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.LoadBalancingTauRequired
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.CsFallbackTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.UeNotAvailableForPsService
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.RadioResourcesNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.FailureInRadioInterfaceProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.InvalidQosCombination
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.InterratRedirection
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.InteractionWithOtherProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.UnknownERABID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.MultipleERABIDInstances
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.S1IntraSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.S1InterSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.X2HandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.TransportResourceUnavailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.TransportUnspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.NormalRelease
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.AuthenticationFailure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.Detach
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.Unspecified4
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.TransferSyntaxError
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.AbstractSyntaxErrorReject
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.AbstractSyntaxErrorIgnoreAndNotify
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.MessageNotCompatibleWithReceiverState
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.SemanticError
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.AbstractSyntaxErrorFalselyConstructedMessage
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.Unspecified_1
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			/*SPR 10987 end*/
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 20171 Start */
                //UECNTX.RelReq.NoUserActivity

                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
            }
            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
            {
                 /*SPR 20171 End*/
				//UECNTX.RelReq.OMIntervention
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.HardwareFailure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			/*SPR 10987 start*/
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.ControlProcessingOverload
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.NotEnoughUserPlaneProcessingResources
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.S1APUnspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX.RelReq.S1APUnknownPLMN
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//UECNTX. RelSuccNbr
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
			/*SPR 10987 end*/
            /*SPR 20171 Start */

            OAM_LOG(OAM,OAM_DETAILEDALL,"UECNTX. RelSuccNbr =%s written at index %d ",
                    pm_xml_param_value[rrc_str_count-1], rrc_str_count-1);
            /*SPR 20171 End */


		}

		if(oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].bitmask 
				& RRC_OAM_STATISTICS_ERAB_SETUP_IND_STATS_PRESENT) 
		{
			//ERAB.EstabInitAttNbr
			for(index = OAM_ONE; (index < RRC_MAX_QCI_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estabInitAttNbr_qci[index] );
			}
			/*SPR 10987 Start*/
			//ERAB.EstabInitAttNbr.Sum
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estabInitAttNbr_sum);
			}
			/*SPR 10987 End*/

			//ERAB.EstabInitSuccNbr
			for(index = OAM_ONE; (index < RRC_MAX_QCI_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estabInitSuccNbr_qci[index]  );
			}

			/*SPR 10987 Start*/
			//ERAB.EstabInitSuccNbr.Sum
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{     
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estabInitSuccNbr_sum);
				/*SPR 10987 End*/
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{        
				//ERAB.EstabInitFailNbr.Sum
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estabInitFailNbr_sum);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{            
				//ERAB.EstabInitFailNbr.ControlProcessingOverload
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_init_fail_stats.
						erab_estabInitFailNbr_cause_misc[s1ap_control_processing_overload]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{    
				//ERAB.EstabInitFailNbr.FailureInTheRadioInterfaceProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_init_fail_stats.
						erab_estabInitFailNbr_cause_radioNetwork[s1ap_failure_in_radio_interface_procedure]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabInitFailNbr.NoRadioResourcesAvailableInTargetCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_init_fail_stats.
						erab_estabInitFailNbr_cause_radioNetwork[s1ap_no_radio_resources_available_in_target_cell]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabInitFailNbr.NotEnoughUserPlaneProcessingResourcesAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_init_fail_stats.
						erab_estabInitFailNbr_cause_misc[s1ap_not_enough_user_plane_processing_resources]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{    
				//ERAB.EstabInitFailNbr.RadioConnectionWithUELost
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_init_fail_stats.
						erab_estabInitFailNbr_cause_radioNetwork[s1ap_radio_connection_with_ue_lost]);
			}
            /*SPR 19596 Code Deleted*/


			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Changes Start*/

				//ERAB.EstabInitFailNbr.TransportResourceUnavailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_init_fail_stats.
						erab_estabInitFailNbr_cause_transport[s1ap_transport_resource_unavailable]);
			}

			//ERAB.EstabAddAttNbr
			for(index = OAM_ONE; (index < RRC_MAX_QCI_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estabAddAttNbr_qci[index] );
			}
			//ERAB.EstabAddSuccNbr
			for(index = OAM_ONE; (index < RRC_MAX_QCI_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estabAddSuccNbr_qci[index]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.Sum
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estabAddFailNbr_sum);
			}
           /* SPR 20171 code deleted */ 
            /*SPR 19596 End*/


			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.Unspecified_2
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_unspecified_2]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.Tx2RelocoverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_tx2relocoverall_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.SuccessfulHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_successful_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.ReleaseDueToEutranGeneratedReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_release_due_to_eutran_generated_reason]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.HandoverCancelled
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_handover_cancelled]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.PartialHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_partial_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.HoFailureInTargetEPCeNBOrTargetSystem
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_ho_failure_in_target_EPC_eNB_or_target_system]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.HoTargetNotAllowed
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_ho_target_not_allowed]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.TS1RelocoverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_tS1relocoverall_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.TS1relocprepExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_tS1relocprep_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.CellNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_cell_not_available]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.UnknownTargetID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_unknown_targetID]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/

				//ERAB.EstabAddFailNbr.NoRadioResourcesAvailableInTargetCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_no_radio_resources_available_in_target_cell]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 End*/

				//ERAB.EstabAddFailNbr.UnknownMmeUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_unknown_mme_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.UnknownEnbUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_unknown_enb_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.UnknownPairUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_unknown_pair_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.HandoverDesirableForRadioReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_handover_desirable_for_radio_reason]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.TimeCriticalHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_time_critical_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.ResourceOptimisationHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_resource_optimisation_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.ReduceLoadInServingCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_reduce_load_in_serving_cell]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.UserInactivity
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_user_inactivity]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/
                //ERAB.EstabAddFailNbr.RadioConnectionWithUELost
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
                        oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
                        erab_setup_stats.erab_estab_add_fail_stats.
                        erab_estabAddFailNbr_cause_radioNetwork[s1ap_radio_connection_with_ue_lost]);
            }
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
            {
                /*SPR 19596 End*/

				//ERAB.EstabAddFailNbr.LoadBalancingTauRequired
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_load_balancing_tau_required]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.CsFallbackTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_cs_fallback_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.UeNotAvailableForPsService
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_ue_not_available_for_ps_service]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.RadioResourcesNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_radio_resources_not_available]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.FailureInRadioInterfaceProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_failure_in_radio_interface_procedure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.InvalidQosCombination
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_invalid_qos_combination]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.InterratRedirection
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_interrat_redirection]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.InteractionWithOtherProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_interaction_with_other_procedure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.UnknownERABID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_unknown_E_RAB_ID]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.MultipleERABIDInstances
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_multiple_E_RAB_ID_instances]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_encryption_and_or_integrity_protection_algorithms_not_supported]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.S1IntraSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_s1_intra_system_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.S1IinterSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_s1_inter_system_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.X2HOTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_radioNetwork[s1ap_x2_handover_triggered]);
			}
			/*SPR 10987 End*/

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 20171 Start*/
                /*SPR 19596 Start*/
                //ERAB.EstabAddFailNbr.TransportResourceUnavailable
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
                        oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
                        erab_setup_stats.erab_estab_add_fail_stats.
                        erab_estabAddFailNbr_cause_transport[s1ap_transport_resource_unavailable]);
                /*SPR 20171 end*/
            }

            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
            {
				//ERAB.EstabAddFailNbr.ControlProcessingOverload
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
                        oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
                        erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_misc[s1ap_control_processing_overload]);
            }
            /*SPR 20171 Start*/

            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
            {
				//ERAB.EstabAddFailNbr.NotEnoughUserPlaneProcessingResources
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_misc[s1ap_not_enough_user_plane_processing_resources]);
			}
            /*SPR 20171 End*/

            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
            {    
				//ERAB.EstabAddFailNbr.HardwareFailure
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
                        oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
                        erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_misc[s1ap_hardware_failure]);
            }
            /*SPR 20171 Start*/

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.EstabAddFailNbr.OmIntervention
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_misc[s1ap_om_intervention]);
			}
            /*SPR 19596 End*/

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{    
                /*SPR 19596 Start*/
				//ERAB.EstabAddFailNbr.Unspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_misc[s1ap_unspecified]);
            }
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{    
				//ERAB.EstabAddFailNbr.UnknownPLMN
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estab_add_fail_stats.
						erab_estabAddFailNbr_cause_misc[s1ap_unknown_PLMN]);
			}
            /*SPR 19596 End*/
            /*SPR 20171 End*/

			//ERAB.EstabTimeMean on QCI Basis
			for(index = OAM_ONE; (index < RRC_MAX_QCI_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{  
				if(OAM_ZERO != oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].erab_setup_stats.erab_estabTime_qci[index].count)    
				{  
					oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
							(int)(oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
								erab_setup_stats.erab_estabTime_qci[index].erab_estabTime_Sum/
								oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
								erab_setup_stats.erab_estabTime_qci[index].count));
				}
				else
				{

					oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
							(oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
							 erab_setup_stats.erab_estabTime_qci[index].erab_estabTime_Sum));
				}   
			}
			//ERAB.EstabTimeMax on QCI basis
			for(index = OAM_ONE; (index < RRC_MAX_QCI_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{ 
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_setup_stats.erab_estabTime_qci[index].erab_estabTime_Max);
			}
            /*SPR 20171 Start */

            OAM_LOG(OAM,OAM_DETAILEDALL,"ERAB.EstabTimeMax[RRC_MAX_QCI_INDEX (%d)-1] =%s written at index %d ",
                    RRC_MAX_QCI_INDEX, pm_xml_param_value[rrc_str_count-1], rrc_str_count-1);
            /*SPR 20171 End */

		}
		else
		{
            /*SPR 20171 Start */

            OAM_LOG(OAM, OAM_DETAILEDALL, "For cell id = %d  RRC_OAM_STATISTICS_ERAB_SETUP_IND_STATS_PRESENT not there ",
                    loop_counter);
            /*SPR 20171 End */


			/*Filling counter 115 times as number of KPI supported for ERAB SETUP IND is 115*/
			for(index = OAM_ONE; index < 110 && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",OAM_ZERO);
			}
            /*SPR 20171 Start */

            OAM_LOG(OAM,OAM_DETAILEDALL,"ERAB.EstabTimeMax[RRC_MAX_QCI_INDEX (%d)-1] =%s written at index %d ",
                    RRC_MAX_QCI_INDEX, pm_xml_param_value[rrc_str_count-1], rrc_str_count-1);
            /*SPR 20171 End */

		}
		if (oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].bitmask 
				& RRC_OAM_STATISTICS_ERAB_RELEASE_IND_STATS_PRESENT)
		{
			//ERAB.RelEnbNbr on QCI basis
			for(index = OAM_ONE; (index < RRC_MAX_QCI_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{  
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_relEnbNbr_qci[index]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.Sum
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_relEnbNbr_sum);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{    
				//ERAB.RelEnbNbr.Unspecified_2
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_unspecified_2]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{ 
				//ERAB.RelEnbNbr.Tx2RelocOverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_tx2relocoverall_expiry]);
			}
            /*SPR 19596 Code Deleted*/
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{    
				//ERAB.RelEnbNbr.SuccessfulHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_successful_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.ReleaseDueToEutranGeneratedReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_release_due_to_eutran_generated_reason]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.HandoverCancelled
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_handover_cancelled]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.PartialHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_partial_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.HoFailureInTargetEPCeNBOrTargetSystem
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_ho_failure_in_target_EPC_eNB_or_target_system]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.HoTargetNotAllowed
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_ho_target_not_allowed]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                 /*SPR 19596 Start*/
				//ERAB.RelEnbNbr.TS1RelocOverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_tS1relocoverall_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			 {
                 /*SPR 19596 End*/
				//ERAB.RelEnbNbr.TS1RelocPrepExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_tS1relocprep_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.CellNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_cell_not_available]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.UnknownTargetID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_unknown_targetID]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.NoRadioResourcesAvailableInTargetCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_no_radio_resources_available_in_target_cell]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.UnknownMmeUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_unknown_mme_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.UnknownEnbUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_unknown_enb_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.UnknownPairUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_unknown_pair_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.HandoverDesirableForRadioReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_handover_desirable_for_radio_reason]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.TimeCriticalHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_time_critical_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.ResourceOptimisationHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_resource_optimisation_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.ReduceLoadInServingCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_reduce_load_in_serving_cell]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
			 /*SPR 19596 Start*/
            //ERAB.RelEnbNbr.UserInactivity
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_user_inactivity]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 End*/
				//ERAB.RelEnbNbr.RadioConnectionWithUELost
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_radio_connection_with_ue_lost]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.LoadBalancingTauRequired
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_load_balancing_tau_required]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.CsFallbackTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_cs_fallback_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.UeNotAvailableForPsService
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_ue_not_available_for_ps_service]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.RadioResourcesNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_radio_resources_not_available]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.FailureInRadioInterfaceProcedure.
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_failure_in_radio_interface_procedure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.InvalidQosCombination
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_invalid_qos_combination]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.InterratRedirection
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_interrat_redirection]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.InteractionWithOtherProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_interaction_with_other_procedure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.UnknownERABID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_unknown_E_RAB_ID]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.MultipleERABIDInstances
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_multiple_E_RAB_ID_instances]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_encryption_and_or_integrity_protection_algorithms_not_supported]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.S1IntraSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_s1_intra_system_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.S1InterSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_s1_inter_system_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.X2HOTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_radioNetwork[s1ap_x2_handover_triggered]);
			}
            /*SPR 19596 Start*/
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{ 
                //ERAB.RelEnbNbr.RedirectionTowards1xRTT
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
                        (oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
                         erab_release_stats.erab_rel_enb_cause_stats.
                         erab_relEnbNbr_cause_radioNetwork[s1ap_redirection_towards_1xRTT]));
            }
            /*SPR 19596 End*/
			/*SPR 10987 End*/


			/*SPR 10987 Start*/
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.TransportResourceUnavailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_transport[s1ap_transport_resource_unavailable]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.unspecified_3
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_transport[s1ap_unspecified_3]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.NormalRelease
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_nas[normal_release]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.AuthenticationFailure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_nas[authentication_failure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.Detach
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_nas[detach]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.Unspecified_4
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_nas[unspecified_4]);
			}
			/*SPR 10987 End*/
			/*SPR 10987 Start*/
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.TransferSyntaxError
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_protocol[transfer_syntax_error]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.AbstractSyntaxErrorReject
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_protocol[abstract_syntax_error_reject]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.AbstractSyntaxErrorIgnoreAndNotify
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_protocol[abstract_syntax_error_ignore_and_notify]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.MessageNotCompatibleWithReceiverState
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_protocol[message_not_compatible_with_receiver_state]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.SemanticError
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_protocol[semantic_error]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.AbstractSyntaxErrorFalselyConstructedMessage
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_protocol[abstract_syntax_error_falsely_constructed_message]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.Unspecified_1
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_protocol[unspecified_1]);
			}
			/*SPR 10987 End*/

			/*SPR 10987 Start*/
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.ControlProcessingOverload
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_misc[s1ap_control_processing_overload]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.NotEnoughUserPlaneProcessingResources
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_misc[s1ap_not_enough_user_plane_processing_resources]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.HardwareFailure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_misc[s1ap_hardware_failure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.OmIntervention
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_misc[s1ap_om_intervention]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.Unspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_misc[s1ap_unspecified]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelEnbNbr.UnknownPLMN
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_rel_enb_cause_stats.
						erab_relEnbNbr_cause_misc[s1ap_unknown_PLMN]);
			}
			/*SPR 10987 End*/
			//ERAB.RelAttNbr on QCI Basis
			for(index = OAM_ONE; (index <RRC_MAX_QCI_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{  
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_relAttNbr_qci[index]);
			}
            /*SPR 19611 start*/
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
			//ERAB.RelAttNbr.Sum
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_relAttNbr_sum);
			}

			//ERAB.RelSuccNbr
			for(index = OAM_ONE; (index < RRC_MAX_QCI_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{  
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_relSuccNbr_qci[index]);
			}
            /*SPR 19611 End */

            /* SPR 23209 FIXED START */
            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
            {
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
                        oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
                        erab_release_stats.erab_relSuccNbr_sum);
            }
           
            /* SPR 23209 FIXED END */

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
			//ERAB.RelSuccNbr.Sum
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_relSuccNbr_sum);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.Sum
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_relFailNbr_sum);
			}
            /*SPR 19596 Code deleted*/

			/*SPR 10987 Start*/
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{    
                /*SPR 19596 Code Deleted*/

				//ERAB.RelFailNbr.Unspecified_2
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_unspecified_2]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{    
				//ERAB.RelFailNbr.Tx2RelocOverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_tx2relocoverall_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{    
				//ERAB.RelFailNbr.SuccessfulHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_successful_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.ReleaseDueToEutranGeneratedReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_release_due_to_eutran_generated_reason]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.HandoverCancelled
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_handover_cancelled]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.PartialHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_partial_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.HoFailureInTargetEPCeNBOrTargetSystem
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_ho_failure_in_target_EPC_eNB_or_target_system]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.HoTargetNotAllowed
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_ho_target_not_allowed]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.TS1RelocOverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_tS1relocoverall_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.TS1RelocPrepExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_tS1relocprep_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.CellNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_cell_not_available]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.UnknownTargetID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_unknown_targetID]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.NoRadioResourcesAvailableInTargetCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_no_radio_resources_available_in_target_cell]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.UnknownMmeUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_unknown_mme_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.UnknownEnbUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_unknown_enb_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.UnknownPairUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_unknown_pair_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.HandoverDesirableForRadioReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_handover_desirable_for_radio_reason]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.TimeCriticalHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_time_critical_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.ResourceOptimisationHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_resource_optimisation_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.ReduceLoadInServingCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_reduce_load_in_serving_cell]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/
				//ERAB.RelFailNbr.NoUserActivity
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_user_inactivity]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
                /*SPR 19596 End*/

			{
				//ERAB.RelFailNbr.RadioConnectionWithUELost
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_radio_connection_with_ue_lost]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.LoadBalancingTauRequired
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_load_balancing_tau_required]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.CsFallbackTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_cs_fallback_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.UeNotAvailableForPsService
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_ue_not_available_for_ps_service]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.RadioResourcesNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_radio_resources_not_available]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.FailureInRadioInterfaceProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_failure_in_radio_interface_procedure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.InvalidQosCombination
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_invalid_qos_combination]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.InterratRedirection
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_interrat_redirection]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.InteractionWithOtherProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_interaction_with_other_procedure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.UnknownERABID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_unknown_E_RAB_ID]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.MultipleERABIDInstances
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_multiple_E_RAB_ID_instances]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_encryption_and_or_integrity_protection_algorithms_not_supported]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.S1IntraSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_s1_intra_system_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.S1InterSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_s1_inter_system_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.X2HOTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_radioNetwork[s1ap_x2_handover_triggered]);
			}
			//ERAB.RelFailNbr.TransportResourceUnavailable
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_transport[s1ap_transport_resource_unavailable]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.unspecified_3
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_transport[s1ap_unspecified_3]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.NormalRelease
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_nas[normal_release]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.AuthenticationFailure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_nas[authentication_failure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.Detach
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_nas[detach]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.Unspecified_4
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_nas[unspecified_4]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.TransferSyntaxError
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_protocol[transfer_syntax_error]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.AbstractSyntaxErrorReject
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_protocol[abstract_syntax_error_reject]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.AbstractSyntaxErrorIgnoreAndNotify
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_protocol[abstract_syntax_error_ignore_and_notify]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.MessageNotCompatibleWithReceiverState
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_protocol[message_not_compatible_with_receiver_state]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.SemanticError
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_protocol[semantic_error]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.AbstractSyntaxErrorFalselyConstructedMessage
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_protocol[abstract_syntax_error_falsely_constructed_message]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.Unspecified_1
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_protocol[unspecified_1]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				 /*SPR 19596 Start*/

                //ERAB.RelFailNbr.ControlProcessingOverload
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_misc[s1ap_control_processing_overload]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 End*/
				//ERAB.RelFailNbr.NotEnoughUserPlaneProcessingResources
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_misc[s1ap_not_enough_user_plane_processing_resources]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.HardwareFailure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_misc[s1ap_hardware_failure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.OmIntervention
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_misc[s1ap_om_intervention]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.Unspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_misc[s1ap_unspecified]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.RelFailNbr.UnknownPLMN
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.rel_fail_cause_stats.
						erab_relFailNbr_cause_misc[s1ap_unknown_PLMN]);
			}
			/*SPR 10987 End*/

			//ERAB.RelActNbr on QCI basis
			for(index = OAM_ONE; (index < RRC_MAX_QCI_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{  
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_release_stats.erab_relActNbr_qci[index]);
			}
            /*SPR 20171 Start */

            OAM_LOG(OAM,OAM_DETAILEDALL,"ERAB.RelActNbr[RRC_MAX_QCI_INDEX (%d)-1] =%s written at index %d ",
                    RRC_MAX_QCI_INDEX, pm_xml_param_value[rrc_str_count-1], rrc_str_count-1);
            /*SPR 20171 End */

		}
		else
		{
            /*SPR 20171 Start */

            OAM_LOG(OAM, OAM_DETAILEDALL, "For cell id = %d  RRC_OAM_STATISTICS_ERAB_RELEASE_IND_STATS_PRESENT not there ",
                    loop_counter);

			/*Filling counter 154 times as number of KPI supported for ERAB REL IND is 154*/

            for(index = OAM_ONE; (index < 153) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
                /*SPR 20171 End */

			{  
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",OAM_ZERO);
			}
            /*SPR 20171 Start */

            OAM_LOG(OAM,OAM_DETAILEDALL,"ERAB.RelActNbr[RRC_MAX_QCI_INDEX (%d)-1] =%s written at index %d ",
                    RRC_MAX_QCI_INDEX, pm_xml_param_value[rrc_str_count-1], rrc_str_count-1);
            /*SPR 20171 End */

		}
		if (oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].bitmask 
				& RRC_OAM_STATISTICS_ERAB_MODIFY_IND_STATS_PRESENT)
		{
			//ERAB.ModQoSAttNbr on QCI basis
			for(index = OAM_ONE; (index < RRC_MAX_QCI_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_modQosAttNbr_qci[index]);
			}

			//ERAB.ModQoSSuccNbr on QCI basis
			for(index = OAM_ONE; (index < RRC_MAX_QCI_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_modQosSuccNbr_qci[index]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{    
				//ERAB.ModQoSFailNbr.Sum
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_modQosFailNbr_sum);
			}
            /*SPR 19596 Code Deleted*/

			/*SPR 10987 Start*/
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{    
                /*SPR 19596 Code Deleted */
				//ERAB.ModQoSFailNbr.Unspecified_2
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_unspecified_2]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{    
				//ERAB.ModQoSFailNbr.Tx2RelocOverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_tx2relocoverall_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{    
				//ERAB.ModQoSFailNbr.SuccessfulHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_successful_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.ReleaseDueToEutranGeneratedReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_release_due_to_eutran_generated_reason]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.HandoverCancelled
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_handover_cancelled]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.PartialHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_partial_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.HoFailureInTargetEPCeNBOrTargetSystem
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_ho_failure_in_target_EPC_eNB_or_target_system]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.HoTargetNotAllowed
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_ho_target_not_allowed]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.TS1RelocOverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_tS1relocoverall_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.TS1RelocPrepExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_tS1relocprep_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.CellNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_cell_not_available]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.UnknownTargetID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_unknown_targetID]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/

                //ERAB.ModQoSFailNbr.NoRadioResourcesAvailableInTargetCell
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
                        oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
                        erab_mod_stats.erab_mod_fail_cause_stats.
                        erab_modQosFailNbr_cause_radioNetwork[s1ap_no_radio_resources_available_in_target_cell]);
            }
            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
            {
                /*SPR 19596 End*/
				//ERAB.ModQoSFailNbr.UnknownMmeUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_unknown_mme_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.UnknownEnbUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_unknown_enb_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.UnknownPairUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_unknown_pair_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.HandoverDesirableForRadioReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_handover_desirable_for_radio_reason]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.TimeCriticalHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_time_critical_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.ResourceOptimisationHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_resource_optimisation_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.ReduceLoadInServingCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_reduce_load_in_serving_cell]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/
                //ERAB.ModQoSFailNbr.NoUserActivity
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
                        oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
                        erab_mod_stats.erab_mod_fail_cause_stats.
                        erab_modQosFailNbr_cause_radioNetwork[s1ap_user_inactivity]);
            }

            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
            {
                /*SPR 19596 End*/

				//ERAB.ModQoSFailNbr.RadioConnectionWithUELost
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_radio_connection_with_ue_lost]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.LoadBalancingTauRequired
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_load_balancing_tau_required]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.CsFallbackTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_cs_fallback_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.UeNotAvailableForPsService
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_ue_not_available_for_ps_service]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.RadioResourcesNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_radio_resources_not_available]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.FailureInRadioInterfaceProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_failure_in_radio_interface_procedure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.InvalidQosCombination
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_invalid_qos_combination]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.InterratRedirection
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_interrat_redirection]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.InteractionWithOtherProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_interaction_with_other_procedure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.UnknownERABID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_unknown_E_RAB_ID]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.MultipleERABIDInstances
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_multiple_E_RAB_ID_instances]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_encryption_and_or_integrity_protection_algorithms_not_supported]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.S1IntraSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_s1_intra_system_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.S1InterSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_s1_inter_system_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.X2HOTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_radioNetwork[s1ap_x2_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/
				//ERAB.ModQoSFailNbr.TransportResourceUnavailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_transport[s1ap_transport_resource_unavailable]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 End*/
				//ERAB.ModQoSFailNbr.unspecified_3
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_transport[s1ap_unspecified_3]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.NormalRelease
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_nas[normal_release]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.AuthenticationFailure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_nas[authentication_failure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.Detach
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_nas[detach]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.Unspecified_4
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_nas[unspecified_4]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.TransferSyntaxError
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_protocol[transfer_syntax_error]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.AbstractSyntaxErrorReject
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_protocol[abstract_syntax_error_reject]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.AbstractSyntaxErrorIgnoreAndNotify
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_protocol[abstract_syntax_error_ignore_and_notify]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.MessageNotCompatibleWithReceiverState
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_protocol[message_not_compatible_with_receiver_state]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.SemanticError
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_protocol[semantic_error]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.AbstractSyntaxErrorFalselyConstructedMessage
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_protocol[abstract_syntax_error_falsely_constructed_message]);
             /*SPR 19596 Start*/
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.Unspecified_1
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_protocol[unspecified_1]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.ControlProcessingOverload
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_misc[s1ap_control_processing_overload]);
			 /*SPR 19596 End*/

            }
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/
                //ERAB.ModQoSFailNbr.NotEnoughUserPlaneProcessingResourcesAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_misc[s1ap_not_enough_user_plane_processing_resources]);
			}
            /*SPR 19596 End*/

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.HardwareFailure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_misc[s1ap_hardware_failure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.OmIntervention
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_misc[s1ap_om_intervention]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.Unspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_misc[s1ap_unspecified]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//ERAB.ModQoSFailNbr.UnknownPLMN
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].
						erab_mod_stats.erab_mod_fail_cause_stats.
						erab_modQosFailNbr_cause_misc[s1ap_unknown_PLMN]);
			}
			/*SPR 10987 End*/
            /*SPR 20171 Start */

            OAM_LOG(OAM,OAM_DETAILEDALL,"ERAB.ModQoSFailNbr.UnknownPLMN =%s written at index %d ",
                    pm_xml_param_value[rrc_str_count-1], rrc_str_count-1);
            /*SPR 20171 End */

		}
		else
		{
            /*SPR 20171 Start */

            OAM_LOG(OAM, OAM_DETAILEDALL, "For cell id = %d  RRC_OAM_STATISTICS_ERAB_NUMBER_IND_STATS_PRESENT not there ",
                    loop_counter);
            /*SPR 20171 End */

			//ERAB.ModQoSAttNbr on QCI basis
			for(index = OAM_ONE; (index < 75) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",OAM_ZERO);
			}
            /*SPR 20171 Start */

            OAM_LOG(OAM,OAM_DETAILEDALL,"ERAB.ModQoSFailNbr.UnknownPLMN =%s written at index %d ",
                    pm_xml_param_value[rrc_str_count-1], rrc_str_count-1);
            /*SPR 20171 End */

		}
		if(oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].bitmask 
				& RRC_OAM_STATISTICS_ERAB_NUMBER_IND_STATS_PRESENT)
		{
			/* SPR 18861 */
            /*SPR 20171 code deleted*/

			UInt32 tot_erab_num_sum = 0;
			UInt32 tot_erab_max_sum = 0;

			/* SPR 18861 */
			//ERAB.UsageNbrMean per QCI basis
			for(index = OAM_ONE; (index < RRC_MAX_QCI_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{
				/* SPR 18861 */
				g_pm_xml_params.rrc_params.ERAB_UsageNbr_Count[index]++;
				/* SPR 18861 */
				g_pm_xml_params.rrc_params.ERAB_UsageNbrSum_QCI[index] += oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].erab_sim_num_stats.
					erabSimNumber_qci[index];

				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						(g_pm_xml_params.rrc_params.ERAB_UsageNbrSum_QCI[index]/g_pm_xml_params.rrc_params.ERAB_UsageNbr_Count[index]));

				/* SPR 18861 */
				OAM_LOG(OAM, OAM_DETAILEDALL, "ERAB_UsageNbr_Count[index %d] %d ERAB_UsageNbrSum_QCI[index %d] %d And Mean %d ",
						index,  
						g_pm_xml_params.rrc_params.ERAB_UsageNbr_Count[index],
						index,
						g_pm_xml_params.rrc_params.ERAB_UsageNbrSum_QCI[index],
						(g_pm_xml_params.rrc_params.ERAB_UsageNbrSum_QCI[index]/g_pm_xml_params.rrc_params.ERAB_UsageNbr_Count[index])
				       );
				if(g_pm_xml_params.rrc_params.ERAB_UsageNbrSum_QCI[index])
				{
                    /*SPR 20171 code deleted*/
    				/* SPR 19562 */
	    			tot_erab_num_sum += (g_pm_xml_params.rrc_params.ERAB_UsageNbrSum_QCI[index]/
	    								g_pm_xml_params.rrc_params.ERAB_UsageNbr_Count[index]);
                    OAM_LOG(OAM, OAM_DETAILEDALL, "ERAB_UsageNbr_Count[index %d] %d ERAB_UsageNbrSum_QCI[index %d] %d tot_erab_num_sum %d",
							index,  
							g_pm_xml_params.rrc_params.ERAB_UsageNbr_Count[index],
							index,
							g_pm_xml_params.rrc_params.ERAB_UsageNbrSum_QCI[index],
                            tot_erab_num_sum 
					       );
				}
			}
			//ERAB.UsageNbrMean.Sum
			/* SPR 19078: Coverity_107801 Fix Start */
            /* SPR 19562 start */
            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",tot_erab_num_sum);

                OAM_LOG(OAM, OAM_DETAILEDALL, "tot_erab_num_sum %d ",tot_erab_num_sum);
			}
			else
			{
				OAM_LOG(OAM, OAM_ERROR, "rrc_str_count(%d) > PM_XML_MAX_PARAM_VALUE(%d)",rrc_str_count,PM_XML_MAX_PARAM_VALUE);
			}
            /* SPR 19562 end*/
			/* SPR 19078: Coverity_107801 Fix End */
			/* SPR 18861 */
			//ERAB.UsageNbrMax per QCI basis
			for(index = OAM_ONE; (index < RRC_MAX_QCI_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{
				if ( g_pm_xml_params.rrc_params.ERAB_UsageNbrMax_QCI[index] < 
						oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].erab_sim_num_stats.
						erabSimNumber_qci[index])
				{
					g_pm_xml_params.rrc_params.ERAB_UsageNbrMax_QCI[index] = oam_pm_rrc_kpi_node->erab_stats_ind_t.erab_stats[loop_counter].erab_sim_num_stats.
						erabSimNumber_qci[index];
				}
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", g_pm_xml_params.rrc_params.ERAB_UsageNbrMax_QCI[index]);

				/* SPR 18861 */
				tot_erab_max_sum +=	g_pm_xml_params.rrc_params.ERAB_UsageNbrMax_QCI[index];
				OAM_LOG(OAM, OAM_DETAILEDALL, "QCI %d  ERAB_UsageNbrMax_QCI[index] %d tot_erab_max_sum  %d ",
						index,
						g_pm_xml_params.rrc_params.ERAB_UsageNbrMax_QCI[index],
						tot_erab_max_sum
				       );
			}

			//ERAB.UsageNbrMax.Sum 
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", tot_erab_max_sum);
			/* SPR 18861 */
		}
		else
		{
            /*SPR 20171 Start */

            OAM_LOG(OAM, OAM_DETAILEDALL, "For cell id = %d  RRC_OAM_STATISTICS_ERAB_NUMBER_IND_STATS_PRESENT not there ",
                    loop_counter);
            /*SPR 20171 End */

			//ERAB.UsageNbrMean per QCI basis
			for(index = OAM_ONE; (index < RRC_MAX_QCI_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}

            /*SPR 20171 Start*/
            //ERAB.UsageNbrMean.Sum
            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)			
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
            /*SPR 20171 End*/
			//ERAB.UsageNbrMax per QCI basis
			for(index = OAM_ONE; (index < RRC_MAX_QCI_INDEX) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
            /*SPR 20171 Start*/
            //ERAB.UsageNbrMax.Sum
            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
            /*SPR 20171 End*/

		}
		if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
		{
			//S1SIG.ConnEstabAtt
			oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
					oam_pm_rrc_kpi_node->ue_assoc_s1_conn_stats_ind_t.
					ue_assoc_s1_conn_stats[loop_counter].s1sig_connEstabAtt);
		}

		if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
		{
			//S1SIG.ConnEstabSucc
			oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
					oam_pm_rrc_kpi_node->ue_assoc_s1_conn_stats_ind_t.
					ue_assoc_s1_conn_stats[loop_counter].s1sig_connEstabSucc);
		}

		//    rrc_str_count = PM_XML_PAGING_PARAM_START; 
		if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
		{
			//PAG.DiscardedNbr
			oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
					oam_pm_rrc_kpi_node->paging_stats_ind_t.rrc_paging_stats[loop_counter].pag_discardedNbr);
		}
		/*SPR 10987 Start*/
		if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
		{
			//PAG.SuccNbr
			oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
					oam_pm_rrc_kpi_node->paging_stats_ind_t.rrc_paging_stats[loop_counter].pag_succNbr);
		}

		if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
		{
			//PAG.ReceivedNbr
			oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
					oam_pm_rrc_kpi_node->paging_stats_ind_t.rrc_paging_stats[loop_counter].pag_receivedNbr);
		}

		/*SPR 10987 End*/
		if(oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].bitmask 
				& RRC_OAM_INTER_ENB_HO_IND_STATS_PRESENT)
		{
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//loop_counter loop counter problem
				//Handover stats
				//HO.InterEnbOutPrepAtt
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.ho_InterEnbOutPrepAtt);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.Sum
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.ho_InterEnbOutAtt_sum);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.Sum
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_sum);
			}
            /*SPR 23209 FIXED START*/
            if((oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].bitmask &
                        RRC_OAM_MEAS_ON_NEIGH_CELL_HO_IND_STATS_PRESENT ) &&
                    ( oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].meas_on_neigh_cell_ho_stats_list.count != 0))
            {
                for(loop_counter_2 = OAM_ZERO;loop_counter_2 < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; loop_counter_2++) 
                { 
                    oam_eutran_causes.ho_OutAttTarget_Cause_radioNetwork += oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork[loop_counter_2];       
                }


                for(loop_counter_2 = OAM_ZERO;loop_counter_2 < RRC_S1AP_CAUSE_TRANSPORT_INDEX; loop_counter_2++)  
                {
                    oam_eutran_causes.ho_OutAttTarget_Cause_transport += oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_transport[loop_counter_2];       
                }


                for(loop_counter_2 = OAM_ZERO;loop_counter_2 < RRC_S1AP_CAUSE_NAS_INDEX; loop_counter_2++)  
                {
                    oam_eutran_causes.ho_OutAttTarget_Cause_nas += oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_nas[loop_counter_2];
                }


                for(loop_counter_2 = OAM_ZERO;loop_counter_2 < RRC_S1AP_CAUSE_PROTOCOL_INDEX; loop_counter_2++)  
                {
                    oam_eutran_causes.ho_OutAttTarget_Cause_protocol += oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_protocol[loop_counter_2];
                }


                for(loop_counter_2 = OAM_ZERO;loop_counter_2 < RRC_S1AP_CAUSE_MISC_INDEX; loop_counter_2++)  
                {
                    oam_eutran_causes.ho_OutAttTarget_Cause_misc += oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_misc[loop_counter_2];
                }


                oam_eutran_causes.ho_OutAttTarget_Cause_sum  = oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                    meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].
                    ho_OutAttTarget_sum;


                /*SPR 14405 START*/
                //HO.OutSuccTarget.sum
                /*SPR 14405 END*/
                for(loop_counter_2 = OAM_ZERO;loop_counter_2 < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; loop_counter_2++)
                {
                    oam_eutran_causes.ho_OutSuccTarget_Cause_radioNetwork += oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork[loop_counter_2];
                }


                for(loop_counter_2 = OAM_ZERO;loop_counter_2 < RRC_S1AP_CAUSE_TRANSPORT_INDEX; loop_counter_2++)
                {
                    oam_eutran_causes.ho_OutSuccTarget_Cause_transport += oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_transport[loop_counter_2];
                }


                for(loop_counter_2 = OAM_ZERO;loop_counter_2 < RRC_S1AP_CAUSE_NAS_INDEX; loop_counter_2++)
                {
                    oam_eutran_causes.ho_OutSuccTarget_Cause_nas += oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_nas[loop_counter_2];
                }


                for(loop_counter_2 = OAM_ZERO;loop_counter_2 < RRC_S1AP_CAUSE_PROTOCOL_INDEX; loop_counter_2++)
                {
                    oam_eutran_causes.ho_OutSuccTarget_Cause_protocol += oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_protocol[loop_counter_2];
                }


                for(loop_counter_2 = OAM_ZERO;loop_counter_2 < RRC_S1AP_CAUSE_MISC_INDEX; loop_counter_2++)
                {
                    oam_eutran_causes.ho_OutSuccTarget_Cause_misc += oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_misc[loop_counter_2];
                }


                oam_eutran_causes.ho_OutSuccTarget_Cause_sum  = oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                    meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].
                    ho_OutSuccTarget_sum;

                fill_plmn_id_from_mcc_mnc(temp_target_str,oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].
                        ecgi.plmn_identity);
                        //target_id.enb_id.global_enb_id.plmn_identity);
            }
            /*SPR 23209 FIXED END*/

            else
            {
                OAM_LOG(OAM,OAM_ERROR,"\n BITMASK NOT FOUND and Count is Zero ");
            }


			/*SPR 10987 Start*/
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Code Deleted*/

				//HO.InterEnbOutAtt.Unspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_unspecified_2]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.Tx2RelocOverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_tx2relocoverall_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.SuccessfulHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_successful_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.ReleaseDueToEutranGeneratedReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_release_due_to_eutran_generated_reason]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.HandoverCancelled
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_handover_cancelled]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.PartialHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_partial_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.HoFailureInTargetEPCeNBOrTargetSystem
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_ho_failure_in_target_EPC_eNB_or_target_system]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.HoTargetNotAllowed
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_ho_target_not_allowed]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.TS1RelocOverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_tS1relocoverall_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.TS1RelocPrepExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_tS1relocprep_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.CellNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_cell_not_available]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.UnknownTargetID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_unknown_targetID]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.NoRadioResourcesAvailableInTargetCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_no_radio_resources_available_in_target_cell]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.UnknownMmeUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_unknown_mme_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.UnknownEnbUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_unknown_enb_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                 /*SPR 19596 Start*/
                //HO.InterEnbOutAtt.UnknownPairUeS1apId
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_unknown_pair_ue_s1ap_id]);
            }	
            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
            {
                /*SPR 19596 End*/
				//HO.InterEnbOutAtt.HandoverDesirableForRadioReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_handover_desirable_for_radio_reason]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.TimeCriticalHandover,
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_time_critical_handover]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.ResourceOptimisationHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_resource_optimisation_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.ReduceLoadInServingCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_reduce_load_in_serving_cell]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				 /*SPR 19596 Start*/
                //HO.InterEnbOutAtt.NoUserActivity
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_user_inactivity]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 End*/
				//HO.InterEnbOutAtt.RadioConnectionWithUeLost
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_radio_connection_with_ue_lost]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.LoadBalancingTauRequired
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_load_balancing_tau_required]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.CsFallbackTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_cs_fallback_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.UeNotAvailableForPsService
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_ue_not_available_for_ps_service]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.RadioResourcesNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_radio_resources_not_available]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.FailureInRadioInterfaceProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_failure_in_radio_interface_procedure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.InvalidQosCombination
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_invalid_qos_combination]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.InterratRedirection
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_interrat_redirection]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.InteractionWithOtherProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_interaction_with_other_procedure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.UnknownERABID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_unknown_E_RAB_ID]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.MultipleERABIDInstances
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_multiple_E_RAB_ID_instances]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_encryption_and_or_integrity_protection_algorithms_not_supported]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.S1IntraSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_s1_intra_system_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.S1InterSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_s1_inter_system_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.X2HandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_radioNetwork[s1ap_x2_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.TransportResourceUnavailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_transport[s1ap_transport_resource_unavailable]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.TransportUnspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_transport[s1ap_unspecified_3]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.NormalRelease
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_nas[normal_release]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.AuthenticationFailure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_nas[authentication_failure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.Detach
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_nas[detach]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.Unspecified4
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_nas[unspecified_4]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.TransferSyntaxError
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_protocol
						[transfer_syntax_error]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.AbstractSyntaxErrorReject
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_protocol
						[abstract_syntax_error_reject]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.AbstractSyntaxErrorIgnoreAndNotify
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_protocol
						[abstract_syntax_error_ignore_and_notify]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.MessageNotCompatibleWithReceiverState
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_protocol
						[message_not_compatible_with_receiver_state]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.SemanticError
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_protocol
						[semantic_error]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.AbstractSyntaxErrorFalselyConstructedMessage
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_protocol
						[abstract_syntax_error_falsely_constructed_message]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.Unspecified_1
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_protocol
						[unspecified_1]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/
                //HO.InterEnbOutAtt.ControlProcessingOverload
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_misc
                        [s1ap_control_processing_overload]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                //HO.InterEnbOutAtt.NotEnoughUserPlaneProcessingResources
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_misc
						[s1ap_not_enough_user_plane_processing_resources]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.HardwareFailure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_misc
						[s1ap_hardware_failure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                //HO.InterEnbOutAtt.OMIntervention
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_misc
                        [s1ap_om_intervention]);
			}
            /*SPR 19596 End*/

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.S1APUnspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_misc
						[s1ap_unspecified]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutAtt.S1APUnknownPLMN
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutAtt_cause.ho_InterEnbOutAtt_Cause_misc
						[s1ap_unknown_PLMN]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Code Deleted*/

				//HO.InterEnbOutSucc.Unspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_unspecified_2]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.Tx2RelocOverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_tx2relocoverall_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.SuccessfulHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_successful_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.ReleaseDueToEutranGeneratedReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_release_due_to_eutran_generated_reason]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.HandoverCancelled
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_handover_cancelled]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.PartialHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_partial_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.HoFailureInTargetEPCeNBOrTargetSystem
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_ho_failure_in_target_EPC_eNB_or_target_system]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.HoTargetNotAllowed
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_ho_target_not_allowed]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.TS1RelocOverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_tS1relocoverall_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.TS1RelocPrepExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_tS1relocprep_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.CellNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_cell_not_available]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.UnknownTargetID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_unknown_targetID]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.NoRadioResourcesAvailableInTargetCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_no_radio_resources_available_in_target_cell]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.UnknownMmeUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_unknown_mme_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.UnknownEnbUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_unknown_enb_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/
                //HO.InterEnbOutSucc.UnknownPairUeS1apId
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_unknown_pair_ue_s1ap_id]);
            }		
            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
            {
                /*SPR 19596 End*/
				//HO.InterEnbOutSucc.HandoverDesirableForRadioReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_handover_desirable_for_radio_reason]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.TimeCriticalHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_time_critical_handover]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.ResourceOptimisationHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_resource_optimisation_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.ReduceLoadInServingCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_reduce_load_in_serving_cell]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/
                //HO.InterEnbOutSucc.NoUserActivity
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_user_inactivity]);
            }
            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
            {
                /*SPR 19596 End*/

				//HO.InterEnbOutSucc.RadioConnectionWithUeLost
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_radio_connection_with_ue_lost]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.LoadBalancingTauRequired
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_load_balancing_tau_required]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.CsFallbackTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_cs_fallback_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.UeNotAvailableForPsService
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_ue_not_available_for_ps_service]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.RadioResourcesNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_radio_resources_not_available]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.FailureInRadioInterfaceProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_failure_in_radio_interface_procedure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.InvalidQosCombination
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_invalid_qos_combination]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.InterratRedirection
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_interrat_redirection]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.InteractionWithOtherProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_interaction_with_other_procedure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.UnknownERABID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_unknown_E_RAB_ID]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.MultipleERABIDInstances
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_multiple_E_RAB_ID_instances]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_encryption_and_or_integrity_protection_algorithms_not_supported]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.S1IntraSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_s1_intra_system_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.S1InterSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_s1_inter_system_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.X2HandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_radioNetwork[s1ap_x2_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.TransportResourceUnavailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_transport[s1ap_transport_resource_unavailable]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.TransportUnspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_transport[s1ap_unspecified_3]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.NormalRelease
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_nas[normal_release]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.AuthenticationFailure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_nas[authentication_failure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.Detach
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_nas[detach]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.Unspecified4
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_enb_ho_stats.ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_nas[unspecified_4]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.TransferSyntaxError
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_protocol
						[transfer_syntax_error]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.AbstractSyntaxErrorReject
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_protocol
						[abstract_syntax_error_reject]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.AbstractSyntaxErrorIgnoreAndNotify
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_protocol
						[abstract_syntax_error_ignore_and_notify]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.MessageNotCompatibleWithReceiverState
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_protocol
						[message_not_compatible_with_receiver_state]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.SemanticError
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_protocol
						[semantic_error]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.AbstractSyntaxErrorFalselyConstructedMessage
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_protocol
						[abstract_syntax_error_falsely_constructed_message]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.Unspecified_1
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_protocol
						[unspecified_1]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/
                //HO.InterEnbOutSucc.ControlProcessingOverload
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_misc
                        [s1ap_control_processing_overload]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                //HO.InterEnbOutSucc.NotEnoughUserPlaneProcessingResources
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_misc
                        [s1ap_not_enough_user_plane_processing_resources]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.HardwareFailure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_misc
						[s1ap_hardware_failure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                //HO.InterEnbOutSucc.OMIntervention
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_misc
                        [s1ap_om_intervention]);
			}
            /*SPR 19596 End*/
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.S1APUnspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_misc
						[s1ap_unspecified]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterEnbOutSucc.S1APUnknownPLMN
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].inter_enb_ho_stats.
						ho_InterEnbOutSucc_cause.ho_InterEnbOutSucc_Cause_misc
						[s1ap_unknown_PLMN]);
			}

			/*SPR 10987 End*/
            /*SPR 20171 Start */

            OAM_LOG(OAM,OAM_DETAILEDALL,"HO.InterEnbOutSucc.S1APUnknownPLMN =%s written at index %d ",
                    pm_xml_param_value[rrc_str_count-1], rrc_str_count-1);
            /*SPR 20171 End */

		}
		else
		{
            /*SPR 20171 Start */

            OAM_LOG(OAM, OAM_DETAILEDALL, "For cell id = %d  RRC_OAM_INTER_ENB_HO_IND_STATS_PRESENT not there ",
                    loop_counter);

            for(index = OAM_ONE; (index < 112) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
            /*SPR 20171 End */

			{
				if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
				{
					oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",OAM_ZERO);
				}
			}
            /*SPR 20171 Start */
            OAM_LOG(OAM,OAM_DETAILEDALL,"HO.InterEnbOutSucc.S1APUnknownPLMN =%s written at index %d ",
                    pm_xml_param_value[rrc_str_count-1], rrc_str_count-1);
            /*SPR 20171 End */

		}
		if (oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].bitmask
				& RRC_OAM_INTRA_FREQ_HO_IND_STATS_PRESENT)
		{
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraFreqOutAtt
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_freq_ho_stats.ho_IntraFreqOutAtt);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraFreqOutSucc
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_freq_ho_stats.ho_IntraFreqOutSucc);
			}
		}
		else
		{
            /*SPR 20171 Start */

            OAM_LOG(OAM, OAM_DETAILEDALL, "For cell id = %d  RRC_OAM_INTRA_FREQ_HO_IND_STATS_PRESENT not there ",
                    loop_counter);
            /*SPR 20171 End */

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraFreqOutAtt
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraFreqOutSucc
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
		}
		if (oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].bitmask
				& RRC_OAM_INTER_FREQ_HO_IND_STATS_PRESENT)
		{
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterFreqMeasGapOutAtt
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_freq_ho_stats.ho_InterFreqMeasGapOutAtt);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterFreqMeasGapOutSucc
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_freq_ho_stats.ho_InterFreqMeasGapOutSucc);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterFreqNoMeasGapOutAtt
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_freq_ho_stats.ho_InterFreqNoMeasGapOutAtt);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterFreqNoMeasGapOutSucc
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						inter_freq_ho_stats.ho_InterFreqNoMeasGapOutSucc);
			}
		}
		else
		{
            /*SPR 20171 Start */

            OAM_LOG(OAM, OAM_DETAILEDALL, "For cell id = %d  RRC_OAM_INTER_FREQ_HO_IND_STATS_PRESENT not there ",
                    loop_counter);
            /*SPR 20171 End */

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterFreqMeasGapOutAtt
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterFreqMeasGapOutSucc
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterFreqNoMeasGapOutAtt
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.InterFreqNoMeasGapOutSucc
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
		}

		if (oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].bitmask
				& RRC_OAM_DRX_NON_DRX_HO_IND_STATS_PRESENT)
		{
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.DrxOutAtt
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						drx_non_drx_ho_stats.ho_DrxOutAtt);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.DrxOutSucc
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						drx_non_drx_ho_stats.ho_DrxOutSucc);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{

				//HO.NoDrxOutAtt
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						drx_non_drx_ho_stats.ho_NoDrxOutAtt);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.NoDrxOutSucc
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						drx_non_drx_ho_stats.ho_NoDrxOutSucc);
			}
		}
		else
		{

            /*SPR 20171 Start */

            OAM_LOG(OAM, OAM_DETAILEDALL, "For cell id = %d  RRC_OAM_DRX_NON_DRX_HO_IND_STATS_PRESENT not there ",
                    loop_counter);

            /*SPR 20171 End */

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.DrxOutAtt
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.DrxOutSucc
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{

				//HO.NoDrxOutAtt
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.NoDrxOutSucc
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d", OAM_ZERO);
			}
		}
		/*SPR 10987 Start*/
		if (oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].bitmask
				& RRC_OAM_INTRA_ENB_HO_IND_STATS_PRESENT)
		{
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Code Deleted*/

				//HO.IntraEnbOutAtt.Unspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_unspecified_2]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.Tx2RelocOverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_tx2relocoverall_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.SuccessfulHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_successful_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.ReleaseDueToEutranGeneratedReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_release_due_to_eutran_generated_reason]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.HandoverCancelled
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_handover_cancelled]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.PartialHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_partial_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.HoFailureInTargetEPCeNBOrTargetSystem
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_ho_failure_in_target_EPC_eNB_or_target_system]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.HoTargetNotAllowed
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_ho_target_not_allowed]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.TS1RelocOverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_tS1relocoverall_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.TS1RelocPrepExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_tS1relocprep_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.CellNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_cell_not_available]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.UnknownTargetID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_unknown_targetID]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.NoRadioResourcesAvailableInTargetCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_no_radio_resources_available_in_target_cell]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.UnknownMmeUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_unknown_mme_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.UnknownEnbUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_unknown_enb_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/
                //HO.IntraEnbOutAtt.UnknownPairUeS1apId
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_unknown_pair_ue_s1ap_id]);
            }			
            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
                /*SPR 19596 End*/
            {
				//HO.IntraEnbOutAtt.HandoverDesirableForRadioReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_handover_desirable_for_radio_reason]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.TimeCriticalHandover,
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_time_critical_handover]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.ResourceOptimisationHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_resource_optimisation_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.ReduceLoadInServingCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_reduce_load_in_serving_cell]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/
                //HO.IntraEnbOutAtt.NoUserActivity
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_user_inactivity]);
            }
            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
            {
                /*SPR 19596 End*/
				//HO.IntraEnbOutAtt.RadioConnectionWithUeLost
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_radio_connection_with_ue_lost]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.LoadBalancingTauRequired
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_load_balancing_tau_required]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.CsFallbackTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_cs_fallback_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.UeNotAvailableForPsService
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_ue_not_available_for_ps_service]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.RadioResourcesNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_radio_resources_not_available]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.FailureInRadioInterfaceProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_failure_in_radio_interface_procedure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.InvalidQosCombination
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_invalid_qos_combination]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.InterratRedirection
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_interrat_redirection]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.InteractionWithOtherProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_interaction_with_other_procedure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.UnknownERABID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_unknown_E_RAB_ID]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.MultipleERABIDInstances
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_multiple_E_RAB_ID_instances]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_encryption_and_or_integrity_protection_algorithms_not_supported]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.S1IntraSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_s1_intra_system_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.S1InterSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_s1_inter_system_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.X2HandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_radioNetwork[s1ap_x2_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.TransportResourceUnavailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_transport[s1ap_transport_resource_unavailable]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.TransportUnspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_transport[s1ap_unspecified_3]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.NormalRelease
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_nas[normal_release]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.AuthenticationFailure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_nas[authentication_failure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.Detach
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_nas[detach]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.Unspecified4
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_nas[unspecified_4]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.TransferSyntaxError
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_protocol
						[transfer_syntax_error]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.AbstractSyntaxErrorReject
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_protocol
						[abstract_syntax_error_reject]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.AbstractSyntaxErrorIgnoreAndNotify
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_protocol
						[abstract_syntax_error_ignore_and_notify]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.MessageNotCompatibleWithReceiverState
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_protocol
						[message_not_compatible_with_receiver_state]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.SemanticError
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_protocol
						[semantic_error]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.AbstractSyntaxErrorFalselyConstructedMessage
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_protocol
						[abstract_syntax_error_falsely_constructed_message]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.Unspecified_1
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_protocol
						[unspecified_1]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/
                //HO.IntraEnbOutAtt.ControlProcessingOverload
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_misc
                        [s1ap_control_processing_overload]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                //HO.IntraEnbOutAtt.NotEnoughUserPlaneProcessingResources
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_misc
                        [s1ap_not_enough_user_plane_processing_resources]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                //HO.IntraEnbOutAtt.HardwareFailure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_misc
                        [s1ap_hardware_failure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                //HO.IntraEnbOutAtt.OMIntervention
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_misc
                        [s1ap_om_intervention]);
			}
            /*SPR 19596 End*/
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.S1APUnspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_misc
						[s1ap_unspecified]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.S1APUnknownPLMN
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutAtt_cause.ho_IntraEnbOutAtt_Cause_misc
						[s1ap_unknown_PLMN]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutAtt.Sum
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutAtt_sum);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Code Deleted*/
				//HO.IntraEnbOutSucc.Unspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_unspecified_2]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.Tx2RelocOverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_tx2relocoverall_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.SuccessfulHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_successful_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.ReleaseDueToEutranGeneratedReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_release_due_to_eutran_generated_reason]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.HandoverCancelled
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_handover_cancelled]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.PartialHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_partial_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.HoFailureInTargetEPCeNBOrTargetSystem
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_ho_failure_in_target_EPC_eNB_or_target_system]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.HoTargetNotAllowed
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_ho_target_not_allowed]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.TS1RelocOverallExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_tS1relocoverall_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.TS1RelocPrepExpiry
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_tS1relocprep_expiry]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.CellNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_cell_not_available]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.UnknownTargetID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_unknown_targetID]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.NoRadioResourcesAvailableInTargetCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_no_radio_resources_available_in_target_cell]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.UnknownMmeUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_unknown_mme_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.UnknownEnbUeS1apId
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_unknown_enb_ue_s1ap_id]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/
                //HO.IntraEnbOutSucc.UnknownPairUeS1apId
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_unknown_pair_ue_s1ap_id]);
            }		
            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
            {
                /*SPR 19596 End*/
				//HO.IntraEnbOutSucc.HandoverDesirableForRadioReason
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_handover_desirable_for_radio_reason]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.TimeCriticalHandover,
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_time_critical_handover]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.ResourceOptimisationHandover
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_resource_optimisation_handover]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.ReduceLoadInServingCell
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_reduce_load_in_serving_cell]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/
                //HO.IntraEnbOutSucc.NoUserActivity
                oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_user_inactivity]);
            }

            if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
                /*SPR 19596 End*/
            {
				//HO.IntraEnbOutSucc.RadioConnectionWithUeLost
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_radio_connection_with_ue_lost]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.LoadBalancingTauRequired
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_load_balancing_tau_required]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.CsFallbackTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_cs_fallback_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.UeNotAvailableForPsService
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_ue_not_available_for_ps_service]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.RadioResourcesNotAvailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_radio_resources_not_available]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.FailureInRadioInterfaceProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_failure_in_radio_interface_procedure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.InvalidQosCombination
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_invalid_qos_combination]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.InterratRedirection
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_interrat_redirection]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.InteractionWithOtherProcedure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_interaction_with_other_procedure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.UnknownERABID
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_unknown_E_RAB_ID]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.MultipleERABIDInstances
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_multiple_E_RAB_ID_instances]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_encryption_and_or_integrity_protection_algorithms_not_supported]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.S1IntraSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_s1_intra_system_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.S1InterSystemHandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_s1_inter_system_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.X2HandoverTriggered
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_radioNetwork[s1ap_x2_handover_triggered]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.TransportResourceUnavailable
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_transport[s1ap_transport_resource_unavailable]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.TransportUnspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_transport[s1ap_unspecified_3]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.NormalRelease
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_nas[normal_release]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.AuthenticationFailure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_nas[authentication_failure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.Detach
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_nas[detach]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.Unspecified4
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
						intra_enb_ho_stats.ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_nas[unspecified_4]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.TransferSyntaxError
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_protocol
						[transfer_syntax_error]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.AbstractSyntaxErrorReject
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_protocol
						[abstract_syntax_error_reject]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.AbstractSyntaxErrorIgnoreAndNotify
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_protocol
						[abstract_syntax_error_ignore_and_notify]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.MessageNotCompatibleWithReceiverState
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_protocol
						[message_not_compatible_with_receiver_state]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.SemanticError
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_protocol
						[semantic_error]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.AbstractSyntaxErrorFalselyConstructedMessage
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_protocol
						[abstract_syntax_error_falsely_constructed_message]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.Unspecified_1
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_protocol
						[unspecified_1]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                /*SPR 19596 Start*/
                //HO.IntraEnbOutSucc.ControlProcessingOverload
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_misc
                        [s1ap_control_processing_overload]);
			}

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                //HO.IntraEnbOutSucc.NotEnoughUserPlaneProcessingResources
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_misc
                        [s1ap_not_enough_user_plane_processing_resources]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                //HO.IntraEnbOutSucc.HardwareFailure
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_misc
                        [s1ap_hardware_failure]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
                //HO.IntraEnbOutSucc.OMIntervention
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_misc
                        [s1ap_om_intervention]);
			}
            /*SPR 19596 End*/

			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.S1APUnspecified
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_misc
						[s1ap_unspecified]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.S1APUnknownPLMN
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutSucc_cause.ho_IntraEnbOutSucc_Cause_misc
						[s1ap_unknown_PLMN]);
			}
			if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
			{
				//HO.IntraEnbOutSucc.Sum
				oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
						oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
						handover_stats[loop_counter].intra_enb_ho_stats.
						ho_IntraEnbOutSucc_sum);
			}
            /*SPR 20171 Start */

            OAM_LOG(OAM,OAM_DETAILEDALL,"HO.IntraEnbOutSucc.Sum  =%s written at index %d ",
                    pm_xml_param_value[rrc_str_count-1], rrc_str_count-1);
            /*SPR 20171 End */

		}
		else
		{
            /*SPR 20171 Start */

            OAM_LOG(OAM, OAM_DETAILEDALL, "For cell id = %d  RRC_OAM_INTRA_ENB_HO_IND_STATS_PRESENT not there ",
                    loop_counter);
            /*SPR 20171 End */


            for(index = OAM_ONE; (index < 112) && (rrc_str_count < PM_XML_MAX_PARAM_VALUE); index++)
			{
				if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
				{
					oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",OAM_ZERO);
				}
			}
            /*SPR 20171 Start */

            OAM_LOG(OAM,OAM_DETAILEDALL,"HO.IntraEnbOutSucc.Sum  =%s written at index %d ",
                    pm_xml_param_value[rrc_str_count-1], rrc_str_count-1);
            /*SPR 20171 End */

		}
		/*SPR 10987 End*/
		/*SPR 10987 Start*/
		if(rrc_str_count < PM_XML_MAX_PARAM_VALUE)
		{
			//HO.RRC.IratIncMobility.LAI
			oam_sprintf((char *)pm_xml_param_value[rrc_str_count++],"%d",
					oam_pm_rrc_kpi_node->irat_mobility_stats_ind.rrc_irat_mobility_stats[loop_counter].
					rrc_IratIncMobility_LAI);
		}
		/* SPR_12747_CHANGES */
		/* redundant code has been removed */
		/* SPR_12747_CHANGES */
		OAM_LOG(OAM,OAM_DETAILED,"value of rrc_str_count = %d",rrc_str_count);
        /*SPR 20171 code deleted*/
		/*SPR 10987 End*/
		/*CSR 00074708 FIX*/
		oam_pm_mac_read_data_from_link_list(&mac_kpi_info, pm_xml_cell_id_to_read, loop_counter);
		oam_pm_pdcp_read_data_from_link_list(&pdcp_kpi_info, pm_xml_cell_id_to_read, loop_counter);

        /*SPR 14504 FIX START */
        /*code removed*/ 
        /*SPR 14504 FIX END */
 
		g_pm_xml_params.rrc_params.RRC_ConnSum = OAM_ZERO;
		g_pm_xml_params.rrc_params.RRC_Conn_Count = OAM_ZERO;
		/*CSR00074708 FIX*/
		for(loop_counter1 = OAM_ZERO; loop_counter1 < RRC_MAX_QCI_INDEX; loop_counter1++)
		{
			g_pm_xml_params.rrc_params.ERAB_UsageNbr_Count[loop_counter1] = OAM_ZERO;
			g_pm_xml_params.rrc_params.ERAB_UsageNbrSum_QCI[loop_counter1] = OAM_ZERO;
		}
		oam_pm_write_ho_related_params_to_file(loop_counter);
		/* SPR 22296 Fix Start */
		oam_pm_write_ho_target_param(oam_pm_rrc_kpi_node, loop_counter, pm_xml_cell_id_to_read);
		/* SPR 22296 Fix End */
		/*CSR00074708 FIX*/
	}
	/*CSR 00074708 FIX*/
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/* SPR_12747_CHANGES */


/****************************************************************************
 * Function Name  :oam_compose_rrc_config_req 
 * Inputs         : p_rrc_config_stats_req : A pointer to oam_rrc_config_stats_req_t
 : cli_recv_bitmask: UInt8 type bitmask

 * Outputs        : p_p_buffer : to store the corresponding message
 * Returns        :OAM_SUCCESS/OAM_FAILURE
 * Description    : This function compose the message for rrc configuration request 
 ****************************************************************************/
oam_return_et
	oam_compose_rrc_config_req
(
 oam_rrc_config_stats_req_t *p_rrc_config_stats_req,
 UInt16 cli_recv_bitmask,
 UInt8  *p_p_buffer
 )
{
	UInt16 oam_recv_bitmask = OAM_ZERO;
	UInt16 oam_sampling_period = OAM_ZERO;

	OAM_FUN_ENTRY_TRACE();
	OAM_ASSERT(OAM_NULL != p_rrc_config_stats_req);

	oam_sampling_period = rrc_kpi_info.periodic_info.sampling_intrvl;  

	oam_cp_pack_UInt16(p_p_buffer, (void *)&cli_recv_bitmask
			,"cli_recv_bitmask"
			);

	p_p_buffer += sizeof(p_rrc_config_stats_req->bitmask);

	oam_cp_pack_UInt8(p_p_buffer, (void *)&(rrc_kpi_info.kpi_info_store_on_off_flag) 
			,"oam_periodic_reporting"
			);
	p_p_buffer += sizeof(p_rrc_config_stats_req->rrc_periodic_reporting);

	oam_recv_bitmask = OAM_ZERO;
	if(oam_bit_read_in_array_lsb((rrc_get_cell_stats_resp_name.bitmask),0)    )
	{
		oam_recv_bitmask = oam_recv_bitmask | RRC_OAM_STATISTICS_REQ_CONNECTION_ESTB_CONFIG_STATS_PRESENT;
		oam_recv_bitmask = oam_recv_bitmask | RRC_OAM_STATISTICS_REQ_CONNECTION_RE_ESTB_CONFIG_STATS_PRESENT;
		oam_recv_bitmask = oam_recv_bitmask | RRC_OAM_STATISTICS_REQ_CONNECTION_NUMBER_CONFIG_STATS_PRESENT;
		oam_recv_bitmask = oam_recv_bitmask | RRC_OAM_STATISTICS_REQ_CONNECTION_SETUP_TIME_CONFIG_STATS_PRESENT;
		oam_recv_bitmask = oam_recv_bitmask | RRC_OAM_STATISTICS_REQ_CONNECTION_UE_CTX_REL_CONFIG_STATS_PRESENT;

		oam_cp_pack_UInt16(p_p_buffer, (void *)&oam_recv_bitmask 
				,"oam_recv_bitmask"
				);   
		p_p_buffer += sizeof(p_rrc_config_stats_req->rrc_connection_stats.bitmask);


		oam_cp_pack_UInt16(p_p_buffer, (void *)&oam_sampling_period
				,"oam_sampling_period"
				);
		p_p_buffer += sizeof(p_rrc_config_stats_req->rrc_connection_stats.sampling_period);

	}

	oam_recv_bitmask = OAM_ZERO;

	if(oam_bit_read_in_array_lsb((rrc_get_cell_stats_resp_name.bitmask),1)    )
	{
		oam_recv_bitmask = oam_recv_bitmask | RRC_OAM_STATISTICS_REQ_ERAB_SETUP_CONFIG_STATS_PRESENT;
		oam_recv_bitmask = oam_recv_bitmask | RRC_OAM_STATISTICS_REQ_ERAB_RELEASE_CONFIG_STATS_PRESENT;
		oam_recv_bitmask = oam_recv_bitmask | RRC_OAM_STATISTICS_REQ_ERAB_MODIFY_CONFIG_STATS_PRESENT;
		oam_recv_bitmask = oam_recv_bitmask | RRC_OAM_STATISTICS_REQ_ERAB_NUMBER_CONFIG_STATS_PRESENT;


		oam_cp_pack_UInt16(p_p_buffer, (void *)&oam_recv_bitmask 
				,"oam_recv_bitmask"
				);   
		p_p_buffer += sizeof(p_rrc_config_stats_req->rrc_erab_stats.bitmask);


		oam_cp_pack_UInt16(p_p_buffer, (void *)&oam_sampling_period
				,"oam_sampling_period"
				);
		p_p_buffer += sizeof(p_rrc_config_stats_req->rrc_erab_stats.sampling_period);

	}
	oam_recv_bitmask = OAM_ZERO;
	if(oam_bit_read_in_array_lsb((rrc_get_cell_stats_resp_name.bitmask),2)    )
	{
		oam_recv_bitmask = oam_recv_bitmask |RRC_OAM_STATISTICS_REQ_INTRA_ENB_HO_RELATED_CONFIG_STATS_PRESENT;
		oam_recv_bitmask = oam_recv_bitmask |RRC_OAM_STATISTICS_REQ_INTER_ENB_HO_RELATED_CONFIG_STATS_PRESENT;
		oam_recv_bitmask = oam_recv_bitmask |RRC_OAM_STATISTICS_REQ_MEAS_ON_NEIGH_CELL_HO_CONFIG_STATS_PRESENT;
		oam_recv_bitmask = oam_recv_bitmask |RRC_OAM_STATISTICS_REQ_INTRA_FREQ_HO_RELATED_CONFIG_STATS_PRESENT;
		oam_recv_bitmask = oam_recv_bitmask |RRC_OAM_STATISTICS_REQ_INTER_FREQ_HO_RELATED_CONFIG_STATS_PRESENT;
		oam_recv_bitmask = oam_recv_bitmask |RRC_OAM_STATISTICS_REQ_DRX_NON_DRX_HO_RELATED_CONFIG_STATS_PRESENT;
		oam_recv_bitmask = oam_recv_bitmask |RRC_OAM_STATISTICS_REQ_INTER_RAT_HO_CONFIG_STATS_PRESENT;
		oam_recv_bitmask = oam_recv_bitmask |RRC_OAM_STATISTICS_REQ_HO_FAILURE_NUM_CONFIG_STATS_PRESENT;

		oam_cp_pack_UInt16(p_p_buffer, (void *)&oam_recv_bitmask
				,"oam_recv_bitmask"
				);
		p_p_buffer += sizeof(p_rrc_config_stats_req->rrc_handover_stats.bitmask);


		oam_cp_pack_UInt16(p_p_buffer, (void *)&oam_sampling_period
				,"oam_sampling_period"
				);
		p_p_buffer += sizeof(p_rrc_config_stats_req->rrc_handover_stats.sampling_period);

	}


	if(oam_bit_read_in_array_lsb((rrc_get_cell_stats_resp_name.bitmask),3)    )
	{ 
		oam_cp_pack_UInt16(p_p_buffer, (void *)&oam_sampling_period
				,"oam_sampling_period"
				);
		p_p_buffer += sizeof(p_rrc_config_stats_req->rrc_ue_assoc_s1_conn_stats.sampling_period);
	}
	if(oam_bit_read_in_array_lsb((rrc_get_cell_stats_resp_name.bitmask),4)    )
	{
		oam_cp_pack_UInt16(p_p_buffer, (void *)&oam_sampling_period
				,"oam_sampling_period"
				);
		p_p_buffer += sizeof(p_rrc_config_stats_req->rrc_paging_stats.sampling_period);
	}

	if(oam_bit_read_in_array_lsb((rrc_get_cell_stats_resp_name.bitmask),5)    )
	{
		oam_cp_pack_UInt16(p_p_buffer, (void *)&oam_sampling_period
				,"oam_sampling_period"
				);

	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pm_send_rrc_kpi_config_req
 * Inputs         : p_error_code: A pointer to oam_error_code
 : recv_bitmask : UInt16 type bitmask
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This Function sends the rrc_kpi_config_req from oam to rrc
 ****************************************************************************/
oam_return_et
oam_pm_send_rrc_kpi_config_req
(
 /* SPR_12955_FIXES */
 UInt16 trans_id,
 /* SPR_12955_FIXES */
 oam_error_code_et *p_error_code,
 UInt16  recv_bitmask 
 )
{
	UInt16    transaction_id = OAM_ZERO;
	oam_length_t msg_length = OAM_ZERO;
	oam_length_t  msg_api_length = OAM_ZERO;
	UInt8 *p_msg = OAM_NULL;
	UInt8 *p_rrc_msg = OAM_NULL;
	UInt16 src_module_id = OAM_ZERO;
	oam_rrc_config_stats_req_t oam_rrc_config_stats_req;    

	OAM_FUN_ENTRY_TRACE();

	oam_memset((void *)&oam_rrc_config_stats_req,OAM_ZERO,sizeof(oam_rrc_config_stats_req));

	/* SPR 15491 START */
	if (oam_prov_req.oam_igd_params.perf_mgmt_params.Enable == OAM_ONE)
	{
		oam_rrc_config_stats_req.rrc_periodic_reporting = g_pm_xml_params.pm_xml_sampling_interval_timer;
	}
	else
	{
		oam_rrc_config_stats_req.rrc_periodic_reporting = OAM_ZERO;
	}
	/*SPR 15491 END */

	msg_length = sizeof(oam_rrc_config_stats_req_t); 
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* check for failure */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	p_rrc_msg = p_msg;


	/* SPR_12955_FIXES */
	transaction_id = trans_id;
	/* SPR_12955_FIXES */
	src_module_id = OAM_MODULE_ID;
	/* Fill CSPL header */
	OAM_LOG(OAM,OAM_DETAILED,"Trans_id = %d sent to RRC for RRC_OAM_CONFIG_STATS_REQ",
			transaction_id);
	oam_construct_api_header(p_rrc_msg, RRC_VERSION_ID, src_module_id,
			RRC_MODULE_ID, RRC_OAM_CONFIG_STATS_REQ,
			msg_api_length); 

	p_rrc_msg = p_rrc_msg + OAM_CSPL_HDR_SIZE; 


	oam_construct_interface_api_header(p_rrc_msg, transaction_id,
			src_module_id, RRC_MODULE_ID,
			RRC_OAM_CONFIG_STATS_REQ,
			msg_length, oam_prov_req.cell_cntxt.curr_cell_cid);

	p_rrc_msg =p_rrc_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	oam_rrc_config_stats_req.bitmask = recv_bitmask;
	oam_compose_rrc_config_req(&oam_rrc_config_stats_req,(UInt16)rrc_get_cell_stats_resp_name.bitmask[0],p_rrc_msg);


	if(OAM_FAILURE == oam_send_message(p_msg, RRC_MODULE_ID, p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in sending RRC_OAM_CONFIG_STATS_REQ message to RRC with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	Config_Flag = 1;  
	g_l3_kpi_ind_handler.ind_bit_mask = 55; 
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_hdle_rrc_config_resp
 * Inputs         : p_api : A void pointer
 *                : data_len : data length 
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This Function handles the rrc configuration response 
 ****************************************************************************/
oam_return_et
oam_hdle_rrc_config_resp
(
 void *p_api
 /*SPR 17777 fix*/
 )
{
	/* CSR No 00055803 Start */
	UInt32 bit_mask = OAM_ZERO;
	UInt16 trans_id = OAM_ZERO,
	       transaction_id = OAM_ZERO; 
	/* CSR No 00055803 End */
	UInt16 dest_id = OAM_ZERO;
	oam_error_code_et p_error_code;
	oam_rrc_config_stats_resp_t p_rrc_config_stats_resp; 

	OAM_FUN_ENTRY_TRACE();

	/* SPR 17883 fix start */
	/* SPR_12955_FIXES */
	transaction_id  = get_word_from_header((UInt8*)(p_api) + OAM_TRANSACTION_ID);
	/* SPR_12955_FIXES */
	/* SPR 17883 fix end */
	p_api = p_api + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Getting the response value */
	oam_cp_unpack_UInt8((void *)&(p_rrc_config_stats_resp.response),p_api
			, "p_rrc_config_stats_resp->response"
			);

	/* CSR No 00055803 Start */
	/* SPR_12955_FIXES */
	oam_tcb_update_bitmask(transaction_id,OAM_TRANS_RRC,&bit_mask);
	/* SPR_12955_FIXES */

	OAM_LOG(OAM,OAM_DETAILED, "RRC_OAM_CONFIG_STATS_RESP bitmask = %d",bit_mask);
	/* CSR No 00055803 End */

	if(OAM_ONE == p_rrc_config_stats_resp.response)
	{
		/*SPR_10579 Start*/
		oam_handle_multiple_config_resp.success_fail_bitmask = oam_handle_multiple_config_resp.success_fail_bitmask & OAM_MAC_PDCP_CONFIG_RESP_VALUE; 
		/*SPR_10579 End*/
	}
	/* CSR No 00055803 Start */
	if((OAM_ZERO == bit_mask) && (OAM_ZERO == oam_handle_multiple_config_resp.success_fail_bitmask) )
	{
		/* Get the transaction id as sent to lower layer */
		/* SPR_12955_FIXES */
		if(OAM_FAILURE == oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(
					transaction_id,&trans_id,&dest_id))					
		{
			if(OAM_ONE == g_resp_sent_user_interface)
			{
				g_resp_sent_user_interface = OAM_ZERO;				
				oam_construct_n_send_set_resp_to_usr(OAM_RESULT_FAILURE,
						OAM_ZERO,(UInt8 *)"CONFIGURATION KPI FAILED",oam_strlen("CONFIGURATION KPI FAILED") + 1,
						OAM_VERSION_ID, OAM_MODULE_ID,
						/* SPR 15491 START */
						oam_set_config_req.src_id, trans_id, OAM_ZERO, OAM_ZERO,
						/* SPR 15491 END */
						&p_error_code);
			}

			OAM_FUN_EXIT_TRACE();
			return OAM_FAILURE;
		}
		if(OAM_ONE == g_resp_sent_user_interface)
		{
			g_resp_sent_user_interface = OAM_ZERO;				
			oam_construct_n_send_set_resp_to_usr(OAM_RESULT_SUCCESS,
					OAM_ZERO,OAM_NULL,OAM_ZERO,
					OAM_VERSION_ID, OAM_MODULE_ID,
					/* SPR 15491 START */
					oam_set_config_req.src_id, trans_id, OAM_ZERO, OAM_ZERO,
					/* SPR 15491 END */
					&p_error_code);
		}
	}
	else if((OAM_ZERO == bit_mask) && (OAM_ZERO != oam_handle_multiple_config_resp.success_fail_bitmask) )
	{
		/* SPR_12955_FIXES */
		if(OAM_FAILURE == oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(
					transaction_id,&trans_id,&dest_id))

		{
			if(OAM_ONE == g_resp_sent_user_interface)
			{
				g_resp_sent_user_interface = OAM_ZERO;				
				oam_construct_n_send_set_resp_to_usr(OAM_RESULT_FAILURE,
						OAM_ZERO,(UInt8 *)"CONFIGURATION KPI FAILED",oam_strlen("CONFIGURATION KPI FAILED") + 1,
						OAM_VERSION_ID, OAM_MODULE_ID,
						/* SPR 15491 START */
						oam_set_config_req.src_id, trans_id, OAM_ZERO, OAM_ZERO,
						/* SPR 15491 END */
						&p_error_code);

			}
			OAM_FUN_EXIT_TRACE();
			return OAM_FAILURE;
		}
		if(OAM_ONE == g_resp_sent_user_interface)
		{
			g_resp_sent_user_interface = OAM_ZERO;				
			oam_construct_n_send_set_resp_to_usr(OAM_RESULT_FAILURE,
					OAM_ZERO,(UInt8 *)"CONFIGURATION KPI FAILED",oam_strlen("CONFIGURATION KPI FAILED") + 1, 
					OAM_VERSION_ID, OAM_MODULE_ID,
					/* SPR 15491 START */
					oam_set_config_req.src_id, trans_id, OAM_ZERO, OAM_ZERO,
					/* SPR 15491 END */
					&p_error_code);
		}
	} 
	/* CSR No 00055803 End */
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
/***************************************************************************
 * Function Name  : oam_handle_m2ap_get_stat_resp
 * Description    : This function handle M2AP get stat response
 * Inputs         : p_oam_m2ap_get_stat_resp
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_handle_m2ap_get_stat_resp
(
 m2ap_oam_get_stat_resp_t *p_oam_m2ap_get_stat_resp,
 UInt16 transaction_id,
 UInt16 dest_id,
 oam_error_code_et *p_error_code
 )
{

    UInt32          count               = OAM_ZERO;
    UInt8           **temp_arr; /* this is the array name */
    UInt32          rows                = OAM_ZERO; /* this variable will be used for the first  dimension */
    UInt32          cols                = OAM_ZERO; /* this variable will be used for the second dimension */
    UInt32          str_count           = OAM_ZERO;
    UInt8           service_area_count  = OAM_ZERO;
    oam_counter_t   loop_counter        = OAM_ZERO;
    oam_counter_t   loop_counter_1      = OAM_ZERO;

    OAM_FUN_ENTRY_TRACE();
    rows = OAM_ZERO;
    cols = OAM_TEN * OAM_TWENTY;

    /* Initializing count with Max number of STATS 25 */
    /* SPR 18056 Fix Start*/
    count = OAM_TWENTY_EIGHT;        
    /* SPR 18056 Fix End*/
    rows = count;
    service_area_count = p_oam_m2ap_get_stat_resp->session_stats_list.count;

    if ( OAM_ZERO == service_area_count )
    {
        OAM_LOG(OAM,OAM_ERROR,"service_area_count : %d", service_area_count);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    /*  allocate storage for an array of pointer to char */
    /* SPR 18056 Fix Start*/
    if (OAM_FAILURE == oam_mem_alloc(((rows * (p_oam_m2ap_get_stat_resp->
                            session_stats_list.count)) * sizeof(UInt8 *)),
                /* SPR 18056 Fix End*/
                (void *)&temp_arr,p_error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"\nError in Memory Allocation with error code %d",
                *p_error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;

    }

    /* for each pocharer, allocate storage for an array of chars */
    /* SPR 18056 Fix Start*/
    for (loop_counter = OAM_ZERO; loop_counter < (rows *
                (p_oam_m2ap_get_stat_resp->session_stats_list.count));
            loop_counter++)
        /* SPR 18056 Fix End*/
    {
        if(OAM_FAILURE == oam_mem_alloc((cols * sizeof(UInt8)),
                    (void *)&temp_arr[loop_counter],p_error_code))
        {
            OAM_LOG(OAM, OAM_ERROR,
                    "\nError in Memory Allocation with error code %d",
                    *p_error_code);
            OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, loop_counter, *p_error_code);

            OAM_FUN_EXIT_TRACE();
            return OAM_FAILURE;
        }
    }


    if( OAM_SUCCESS == p_oam_m2ap_get_stat_resp->response )
    {
        OAM_LOG(OAM, OAM_DETAILED, "\n\tno.of Service Areas: %d", service_area_count);
        /* Loop for no. of sessions */
        for(;loop_counter_1 < service_area_count;loop_counter_1++)
        {
            OAM_LOG(OAM, OAM_DETAILED,
                    "\n---------------- Session[%d]Stats --------------",
                    (loop_counter_1 + OAM_ONE));


            OAM_LOG(OAM, OAM_DETAILED,
                    "\n##############Session Start Stats##############\n\n");

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Session Start Area ID", p_oam_m2ap_get_stat_resp->
                    session_stats_list.m2ap_session_start_stats[loop_counter_1].area_id);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Session Start Attempt Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_session_start_stats[loop_counter_1].session_start_attempt_sum);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Session Start Fail Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_session_start_stats[loop_counter_1].
                    session_start_fail_sum);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Session Start Success Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_session_start_stats[loop_counter_1].
                    session_start_succ_sum);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u", 
                    "Session Start Time", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_session_start_stats[loop_counter_1].
                    session_start_time);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Session Start Time Max", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_session_start_stats[loop_counter_1].
                    session_start_time_max);

            OAM_LOG(OAM, OAM_DETAILED,
                    "\n##############Session Stop Stats##############\n\n");

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Session Stop Area ID", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_session_stop_stats[loop_counter_1].area_id);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Session Stop Attempt Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_session_stop_stats[loop_counter_1].
                    session_stop_attempt_sum);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Session Stop Fail Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_session_stop_stats[loop_counter_1].
                    session_stop_fail_sum);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Session Stop Success Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_session_stop_stats[loop_counter_1].
                    session_stop_succ_sum);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Session Stop Implicit", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_session_stop_stats[loop_counter_1].
                    session_stop_implicit);

            OAM_LOG(OAM, OAM_DETAILED,
                    "\n##############Session Update Stats##############\n\n");

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Session Update Area ID", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_session_update_stats[loop_counter_1].area_id);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Session Update Attempt Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_session_update_stats[loop_counter_1].
                    session_update_attempt_sum);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Session Update Fail Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_session_update_stats[loop_counter_1].
                    session_update_fail_sum);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Session Update Success Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_session_update_stats[loop_counter_1].
                    session_update_succ_sum);

            OAM_LOG(OAM, OAM_DETAILED,
                    "\n##############Session Counting Stats##############\n\n");

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Counting Stats Area ID", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_counting_stats[loop_counter_1].area_id);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Counting Req Attempt Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_counting_stats[loop_counter_1].
                    session_counting_req_attempt_sum);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Counting Resp Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_counting_stats[loop_counter_1].
                    session_counting_response_sum);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Counting Resp Recvd From UE SUM", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_counting_stats[loop_counter_1].
                    session_couting_response_received_from_UEs_sum);


            OAM_LOG(OAM, OAM_DETAILED,
                    "\n##############Session ENB Config Update Stats##############\n\n");

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "ENB Config Update Attempt Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_enb_config_update_stats.
                    session_enb_config_update_attempt_sum);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "ENB Config Update Success Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_enb_config_update_stats.
                    session_enb_config_update_success_sum);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "ENB Config Update Fail Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_enb_config_update_stats.
                    session_enb_config_update_fail_sum);

            OAM_LOG(OAM, OAM_DETAILED,
                    "\n##############Session MCE Config Update Stats###############\n\n");

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "MCE Config Update Attempt Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_mce_config_update_stats.
                    session_mce_config_update_attempt_sum);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "MCE Config Update Success Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_mce_config_update_stats.
                    session_mce_config_update_success_sum);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "MCE Config Update Fail Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_mce_config_update_stats.
                    session_mce_config_update_fail_sum);

            OAM_LOG(OAM, OAM_DETAILED,
                    "\n##############Err Indication Stats###############\n\n");

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Err Ind Recieved Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_err_ind_stats.
                    error_ind_received_sum);

            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Err Ind Sent Sum", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    m2ap_err_ind_stats.
                    error_ind_sent_sum);

            OAM_LOG(OAM, OAM_DETAILED,
                    "\n##############Transient Session Start Stats###############\n\n");
            oam_sprintf((char *)(char *)temp_arr[str_count++], "%s=%u",
                    "Transient Session Start Stat", p_oam_m2ap_get_stat_resp->
                    session_stats_list.
                    transient_session_start_stat);

        }
        /* SPR 18056 Fix Start*/
        oam_construct_n_send_gpv_response((UInt8 *)"Show.KPI.M2AP",
                temp_arr, str_count, transaction_id, p_error_code, dest_id,
                OAM_ZERO, OAM_ZERO);
        /* SPR 18056 Fix End*/

    }
    else
    {
        OAM_LOG(OAM, OAM_ERROR, "Failure response received in "
                "M2AP_OAM_GET_STAT_RESP");
        oam_construct_n_send_get_resp_to_usr(
                OAM_RESULT_FAILURE,
                OAM_ERR_INTERNAL_ERROR,
                OAM_VERSION,
                OAM_MODULE_ID,dest_id,
                transaction_id,
                OAM_ZERO, OAM_ZERO,"M2AP FAILURE RESPONSE",
                p_error_code
                );
        OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, rows, *p_error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, rows, *p_error_code);

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}
#endif
/* EMBMS Changes End */
/***************************************************************************
 * Function Name  : oam_handle_rrm_get_kpi_resp
 * Description    : This function handle RRM get KPI response
 * Inputs         : p_rrm_oam_get_kpi_resp
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
	oam_handle_rrm_get_kpi_resp
(
 rrm_oam_get_kpi_resp_t *p_rrm_oam_get_kpi_resp,
 UInt16 transaction_id,
 UInt16 dest_id,
 oam_error_code_et *p_error_code
 )
{

	UInt32 count = OAM_ZERO;
	UInt8 **temp_arr;     /* this is the array name */
	UInt32 size_x = OAM_ZERO; /* this variable will be used for the first  dimension */
	UInt32 size_y = OAM_ZERO; /* this variable will be used for the second dimension */
	UInt32 str_count = OAM_ZERO;
	oam_counter_t   loop_counter      = OAM_ZERO;

	OAM_FUN_ENTRY_TRACE();

	size_x = OAM_ZERO;
	size_y = OAM_TEN * OAM_TWENTY;

	/* Initializing count with Max number of KPI 64 */

	count =65;        
	size_x = count;
	/*  allocate storage for an array of pointer to char */
	if (OAM_FAILURE == oam_mem_alloc((size_x * sizeof(UInt8 *)),(void *)&temp_arr,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to temp_arr failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;

	}

	/* for each pocharer, allocate storage for an array of chars */
	for (loop_counter = OAM_ZERO; loop_counter < size_x; loop_counter++)
	{
		if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)&temp_arr[loop_counter],p_error_code))
		{
			OAM_LOG(OAM, OAM_ERROR,
					"Memory allocation to temp_arr failed"
					" with Error Code:%d",
					*p_error_code);
			/*SPR 13857 START*/
			OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, loop_counter, *p_error_code);
			/*SPR 13857 END*/
			OAM_FUN_EXIT_TRACE();
			return OAM_FAILURE;
		}
	}

	if( OAM_SUCCESS == p_rrm_oam_get_kpi_resp->response )
	{
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[0] & 0x01 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of admitted csg user",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_admitted_csg_user);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[0] & 0x02 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of admitted non-csg user",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_admitted_non_csg_user);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[0] & 0x04 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of UE admission success",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_ue_admission_success);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[0] & 0x08 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of UE admission fail",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_ue_admission_fail);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[0] & 0x10 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of ERB setup success",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_erb_setup_success);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[0] & 0x20 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of ERB setup fail",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_erb_setup_fail);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[0] & 0x40 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of ERB modify success",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_erb_modify_success);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[0] & 0x80 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of ERB modify fail",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_erb_modify_fail);
		}


		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[1] & 0x01 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of ERB release success",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_erb_release_success);
		}   
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[1] & 0x02 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of ERB release fail",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_erb_release_fail);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[1] & 0x04 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Total DL allocated GBR PRB",
					p_rrm_oam_get_kpi_resp->kpi_data.avg_dl_allocated_gbr_prb);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[1] & 0x08 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Total UL allocated GBR PRB",
					p_rrm_oam_get_kpi_resp->kpi_data.avg_ul_allocated_gbr_prb);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[1] & 0x10 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","DL allocated NGBR PRB",
					p_rrm_oam_get_kpi_resp->kpi_data.dl_allocated_ngbr_prb);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[1] & 0x20 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","UL allocated NGBR PRB",
					p_rrm_oam_get_kpi_resp->kpi_data.ul_allocated_ngbr_prb);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[1] & 0x40 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of GERAN HO success",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_geran_ho_attempt);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[1] & 0x80 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of GERAN HO fail",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_geran_ho_fail);
		}

		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[2] & 0x01 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of UTRAN HO success",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_utran_ho_attempt);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[2] & 0x02 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of UTRAN HO fail",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_utran_ho_fail);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[2] & 0x04 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of EUTRAN HO attempt",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_eutran_ho_attempt);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[2] & 0x08 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of EUTRAN HO fail",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_eutran_ho_fail);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[2] & 0x10 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of GERAN HI success",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_geran_hi_success);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[2] & 0x20 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of GERAN HI fail",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_geran_hi_fail);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[2] & 0x40 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of UTRAN HI success",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_utran_hi_success);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[2] & 0x80 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of UTRAN HI fail",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_utran_hi_fail);
		}



		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[3] & 0x01 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of EUTRAN HI success",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_eutran_hi_success);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[3] & 0x02 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of EUTRAN HI fail",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_eutran_hi_fail);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[3] & 0x04 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of ENB init HO CSG user",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_enb_init_ho_csg_usr);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[3] & 0x08 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of ENB init HO NON CSG user",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_enb_init_ho_non_csg_usr);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[3] & 0x10 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of ENB init UE release",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_enb_init_ue_release);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[3] & 0x20 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of PUCCH resource allocation attempts",
					p_rrm_oam_get_kpi_resp->kpi_data.num_pucch_res_alloc_attempts);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[3] & 0x40 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of SR resource allocation fail",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_sr_res_alloc_fail);
		}
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[3] & 0x80 )
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Number of SR CQI allocation fail",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_sr_cqi_alloc_fail);
		}
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","Total Cell Unavailable Time",
				p_rrm_oam_get_kpi_resp->kpi_data.total_cell_unavailable_time);


		/* BUG_FIXED_12710 */
		if( p_rrm_oam_get_kpi_resp->kpi_data.kpi_to_report.bitmap[4] & 0x02)
		{
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%u","num_of_ca_ue",
					p_rrm_oam_get_kpi_resp->kpi_data.num_of_ca_ue);
		}
		/* BUG_FIXED_12710 */

		oam_construct_n_send_gpv_response((UInt8 *)"Show.KPI.RRM",temp_arr,str_count,
				transaction_id,p_error_code,dest_id,OAM_ZERO,OAM_ZERO);

	}
	else
	{
		oam_construct_n_send_get_resp_to_usr(
				OAM_RESULT_FAILURE,
				OAM_ERR_INTERNAL_ERROR,
				OAM_VERSION,
				OAM_MODULE_ID,dest_id,
				transaction_id,
				OAM_ZERO,OAM_ZERO,"RRM LAYER FAILURE RESPONSE",
				p_error_code
				);
	}
	/*SPR 13857 START*/
	OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, size_x, *p_error_code);
	/*SPR 13857 END*/

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


#ifdef OAM_SON_ENABLE
/***************************************************************************
 * Function Name  : oam_handle_son_oam_mro_ho_counter_report
 * Description    : This function sends stats configuration to OAM
 * Inputs         : p_api_buf  :pointer to PDCP get stats confirmation
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *****************************************************************************/
oam_return_et
oam_handle_son_oam_mro_ho_counter_report
(
 son_oam_mro_ho_counter_report_t *p_mro_ho_counter_report
 /*SPR 17777 fix*/
 )
{

	UInt16 count = OAM_ZERO;
	UInt8 str_count = OAM_ZERO;
	UInt8  index = OAM_ZERO;
	char temp_str [OAM_TWENTY_FOUR];
	oam_memset(temp_str,'\0',OAM_TWENTY_FOUR);
	/* Coverity ID 72097 Fix Start*/
	UInt16 num_of_parents = OAM_ZERO;
	/* Coverity ID 72097 Fix End*/
	num_of_parents = 4 + (p_mro_ho_counter_report->neighbour_list_size * 8);
	oam_inform_api_t *oam_inform_api[num_of_parents];

	oam_error_code_et err_code = NO_ERROR;
	OAM_FUN_ENTRY_TRACE();

	for( index=0 ; index < num_of_parents ; index ++ )
	{
		if(OAM_FAILURE == oam_mem_alloc(sizeof(oam_inform_api_t), (void**)&oam_inform_api[index],
					&err_code))
		{
			OAM_LOG(OAM, OAM_ERROR,
					"Memory allocation to oam_inform_api failed"
					" with Error Code:%d",
					err_code);
			/*SPR 13857 START*/
			OAM_PM_INFORM_API_MEM_FREE(oam_inform_api, index, err_code);
			/*SPR 13857 END*/
			return OAM_FAILURE;
		}

	}

	{
		oam_strncpy((char *)oam_inform_api[str_count]->parent_name,"InternetGatewayDevice.FAP.PerfMgmt",sizeof("InternetGatewayDevice.FAP.PerfMgmt"));

		oam_inform_api[str_count]->num_of_param=OAM_ONE;

		oam_memset(oam_inform_api[str_count]->param_list[OAM_ZERO], OAM_ZERO,sizeof(oam_inform_api[str_count]->param_list[OAM_ZERO]));
		oam_sprintf((char *)oam_inform_api[str_count]->param_list[OAM_ZERO],"%s=%d","son_oam_mro_ho_counter_report->bitmask",
				p_mro_ho_counter_report->bitmask);

		str_count++;
	}

	{
		oam_strncpy((char *)oam_inform_api[str_count]->parent_name,"InternetGatewayDevice.FAP.PerfMgmt",sizeof("InternetGatewayDevice.FAP.PerfMgmt"));

		oam_inform_api[str_count]->num_of_param=OAM_ONE;

		oam_memset(oam_inform_api[str_count]->param_list[OAM_ZERO], OAM_ZERO,sizeof(oam_inform_api[str_count]->param_list[OAM_ZERO]));

		if(p_mro_ho_counter_report->srv_cgi.plmn_id.num_mnc_digit == 3)
		{
			oam_sprintf((char *)oam_inform_api[str_count]->param_list[OAM_ZERO],"%s=%d%d%d%d%d%d","son_oam_mro_ho_counter_report->srv_cgi->plmn_id",
					p_mro_ho_counter_report->srv_cgi.plmn_id.mcc[2],p_mro_ho_counter_report->srv_cgi.plmn_id.mcc[1],
					p_mro_ho_counter_report->srv_cgi.plmn_id.mcc[0],p_mro_ho_counter_report->srv_cgi.plmn_id.mnc[2],
					p_mro_ho_counter_report->srv_cgi.plmn_id.mnc[1],p_mro_ho_counter_report->srv_cgi.plmn_id.mnc[0]);
		}
		else
		{
			oam_sprintf((char *)oam_inform_api[str_count]->param_list[OAM_ZERO],"%s=%d%d%d%d%d","son_oam_mro_ho_counter_report->srv_cgi->plmn_id",
					p_mro_ho_counter_report->srv_cgi.plmn_id.mcc[2],p_mro_ho_counter_report->srv_cgi.plmn_id.mcc[1],
					p_mro_ho_counter_report->srv_cgi.plmn_id.mcc[0],p_mro_ho_counter_report->srv_cgi.plmn_id.mnc[1],
					p_mro_ho_counter_report->srv_cgi.plmn_id.mnc[0]);
		}

		str_count++;
	}

	{
		oam_strncpy((char *)oam_inform_api[str_count]->parent_name,"InternetGatewayDevice.FAP.PerfMgmt",sizeof("InternetGatewayDevice.FAP.PerfMgmt"));

		oam_inform_api[str_count]->num_of_param=OAM_ONE;

		oam_memset(oam_inform_api[str_count]->param_list[OAM_ZERO], OAM_ZERO,sizeof(oam_inform_api[str_count]->param_list[OAM_ZERO]));

		oam_sprintf((char *)oam_inform_api[str_count]->param_list[OAM_ZERO],"%s=%d%d%d%d","son_oam_mro_ho_counter_report->srv_cgi->cell_identity",
				p_mro_ho_counter_report->srv_cgi.cell_identity[3],p_mro_ho_counter_report->srv_cgi.cell_identity[2],
				p_mro_ho_counter_report->srv_cgi.cell_identity[1],p_mro_ho_counter_report->srv_cgi.cell_identity[0]);

		str_count++;
	}


	{
		oam_strncpy((char *)oam_inform_api[str_count]->parent_name,"InternetGatewayDevice.FAP.PerfMgmt",sizeof("InternetGatewayDevice.FAP.PerfMgmt"));

		oam_inform_api[str_count]->num_of_param=OAM_ONE;

		oam_memset(oam_inform_api[str_count]->param_list[OAM_ZERO], OAM_ZERO,sizeof(oam_inform_api[str_count]->param_list[OAM_ZERO]));

		oam_sprintf((char *)oam_inform_api[str_count]->param_list[OAM_ZERO],"%s=%d","son_oam_mro_ho_counter_report->neighbour_list_size",
				p_mro_ho_counter_report->neighbour_list_size);

		str_count++;
	}

	for(count=OAM_ZERO;count < p_mro_ho_counter_report->neighbour_list_size; count++)
	{

		oam_strncpy((char *)oam_inform_api[str_count]->parent_name,"InternetGatewayDevice.FAP.PerfMgmt",sizeof("InternetGatewayDevice.FAP.PerfMgmt"));

		oam_inform_api[str_count]->num_of_param=OAM_ONE;

		oam_memset(oam_inform_api[str_count]->param_list[OAM_ZERO], OAM_ZERO,sizeof(oam_inform_api[str_count]->param_list[OAM_ZERO]));
		oam_sprintf((char *)oam_inform_api[str_count]->param_list[OAM_ZERO],"%s=%d","son_oam_mro_ho_counter_report->ho_counter_list->bitmask",
				p_mro_ho_counter_report->ho_counter_list[count].bitmask);

		str_count++;


		{
			oam_strncpy((char *)oam_inform_api[str_count]->parent_name,"InternetGatewayDevice.FAP.PerfMgmt",sizeof("InternetGatewayDevice.FAP.PerfMgmt"));

			oam_inform_api[str_count]->num_of_param=OAM_ONE;

			oam_memset(oam_inform_api[str_count]->param_list[OAM_ZERO], OAM_ZERO,sizeof(oam_inform_api[str_count]->param_list[OAM_ZERO]));

			if(p_mro_ho_counter_report->ho_counter_list[count].nbr_cgi.plmn_id.num_mnc_digit == 3)
			{
				oam_sprintf((char *)oam_inform_api[str_count]->param_list[OAM_ZERO],"%s=%d%d%d%d%d%d","son_oam_mro_ho_counter_report->nbr_cgi->plmn_id",
						p_mro_ho_counter_report->ho_counter_list[count].nbr_cgi.plmn_id.mcc[2],
						p_mro_ho_counter_report->ho_counter_list[count].nbr_cgi.plmn_id.mcc[1],
						p_mro_ho_counter_report->ho_counter_list[count].nbr_cgi.plmn_id.mcc[0],
						p_mro_ho_counter_report->ho_counter_list[count].nbr_cgi.plmn_id.mnc[2],
						p_mro_ho_counter_report->ho_counter_list[count].nbr_cgi.plmn_id.mnc[1],
						p_mro_ho_counter_report->ho_counter_list[count].nbr_cgi.plmn_id.mnc[0]);
			}
			else
			{
				oam_sprintf((char *)oam_inform_api[str_count]->param_list[OAM_ZERO],"%s=%d%d%d%d%d","son_oam_mro_ho_counter_report->nbr_cgi->plmn_id",
						p_mro_ho_counter_report->ho_counter_list[count].nbr_cgi.plmn_id.mcc[2],
						p_mro_ho_counter_report->ho_counter_list[count].nbr_cgi.plmn_id.mcc[1],
						p_mro_ho_counter_report->ho_counter_list[count].nbr_cgi.plmn_id.mcc[0],
						p_mro_ho_counter_report->ho_counter_list[count].nbr_cgi.plmn_id.mnc[1],
						p_mro_ho_counter_report->ho_counter_list[count].nbr_cgi.plmn_id.mnc[0]);
			}

			str_count++;
		}

		{
			oam_strncpy((char *)oam_inform_api[str_count]->parent_name,"InternetGatewayDevice.FAP.PerfMgmt",sizeof("InternetGatewayDevice.FAP.PerfMgmt"));

			oam_inform_api[str_count]->num_of_param=OAM_ONE;

			oam_memset(oam_inform_api[str_count]->param_list[OAM_ZERO], OAM_ZERO,sizeof(oam_inform_api[str_count]->param_list[OAM_ZERO]));

			oam_sprintf((char *)oam_inform_api[str_count]->param_list[OAM_ZERO],"%s=%d%d%d%d","son_oam_mro_ho_counter_report->nbr_cgi->cell_identity",
					p_mro_ho_counter_report->ho_counter_list[count].nbr_cgi.cell_identity[3],
					p_mro_ho_counter_report->ho_counter_list[count].nbr_cgi.cell_identity[2],
					p_mro_ho_counter_report->ho_counter_list[count].nbr_cgi.cell_identity[1],
					p_mro_ho_counter_report->ho_counter_list[count].nbr_cgi.cell_identity[0]);

			str_count++;
		}


		{
			oam_strncpy((char *)oam_inform_api[str_count]->parent_name,"InternetGatewayDevice.FAP.PerfMgmt",sizeof("InternetGatewayDevice.FAP.PerfMgmt"));

			oam_inform_api[str_count]->num_of_param=OAM_ONE;

			oam_memset(oam_inform_api[str_count]->param_list[OAM_ZERO], OAM_ZERO,sizeof(oam_inform_api[str_count]->param_list[OAM_ZERO]));

			oam_sprintf((char *)oam_inform_api[str_count]->param_list[OAM_ZERO],"%s=%d","son_oam_mro_ho_counter_report->ho_counter_list->num_too_early_ho",
					p_mro_ho_counter_report->ho_counter_list[count].num_too_early_ho);

			str_count++;
		}

		{
			oam_strncpy((char *)oam_inform_api[str_count]->parent_name,"InternetGatewayDevice.FAP.PerfMgmt",sizeof("InternetGatewayDevice.FAP.PerfMgmt"));

			oam_inform_api[str_count]->num_of_param=OAM_ONE;

			oam_memset(oam_inform_api[str_count]->param_list[OAM_ZERO], OAM_ZERO,sizeof(oam_inform_api[str_count]->param_list[OAM_ZERO]));

			oam_sprintf((char *)oam_inform_api[str_count]->param_list[OAM_ZERO],"%s=%d","son_oam_mro_ho_counter_report->ho_counter_list->num_too_late_ho",
					p_mro_ho_counter_report->ho_counter_list[count].num_too_late_ho);

			str_count++;
		}


		{
			oam_strncpy((char *)oam_inform_api[str_count]->parent_name,"InternetGatewayDevice.FAP.PerfMgmt",sizeof("InternetGatewayDevice.FAP.PerfMgmt"));

			oam_inform_api[str_count]->num_of_param=OAM_ONE;

			oam_memset(oam_inform_api[str_count]->param_list[OAM_ZERO], OAM_ZERO,sizeof(oam_inform_api[str_count]->param_list[OAM_ZERO]));

			oam_sprintf((char *)oam_inform_api[str_count]->param_list[OAM_ZERO],"%s=%d","son_oam_mro_ho_counter_report->ho_counter_list->num_ho_to_wrong_cell_f",
					p_mro_ho_counter_report->ho_counter_list[count].num_ho_to_wrong_cell_f);

			str_count++;
		}


		{
			oam_strncpy((char *)oam_inform_api[str_count]->parent_name,"InternetGatewayDevice.FAP.PerfMgmt",sizeof("InternetGatewayDevice.FAP.PerfMgmt"));

			oam_inform_api[str_count]->num_of_param=OAM_ONE;

			oam_memset(oam_inform_api[str_count]->param_list[OAM_ZERO], OAM_ZERO,sizeof(oam_inform_api[str_count]->param_list[OAM_ZERO]));

			oam_sprintf((char *)oam_inform_api[str_count]->param_list[OAM_ZERO],"%s=%d","son_oam_mro_ho_counter_report->ho_counter_list->num_ho_to_wrong_cell_r",
					p_mro_ho_counter_report->ho_counter_list[count].num_ho_to_wrong_cell_r);

			str_count++;
		}

		{
			oam_strncpy((char *)oam_inform_api[str_count]->parent_name,"InternetGatewayDevice.FAP.PerfMgmt",sizeof("InternetGatewayDevice.FAP.PerfMgmt"));

			oam_inform_api[str_count]->num_of_param=OAM_ONE;

			oam_memset(oam_inform_api[str_count]->param_list[OAM_ZERO], OAM_ZERO,sizeof(oam_inform_api[str_count]->param_list[OAM_ZERO]));

			oam_sprintf((char *)oam_inform_api[str_count]->param_list[OAM_ZERO],"%s=%d","son_oam_mro_ho_counter_report->ho_counter_list->total_ho_attempted",
					p_mro_ho_counter_report->ho_counter_list[count].total_ho_attempted);

			str_count++;
		}


	}

	oam_compose_and_send_inform_api((oam_inform_api_t **)&oam_inform_api,
			num_of_parents,(UInt8 *)"KPI Value\0",QCLI_MODULE_ID , OAM_MODULE_ID);

	/*SPR 13857 START*/
	for( index=0 ; index < num_of_parents ; index ++ )
	{

		/* Coverity ID 72097 Fix Start*/
		if(OAM_FAILURE == oam_mem_free((void*)oam_inform_api[index],
					&err_code))
			/* Coverity ID 72097 Fix End*/
		{
			OAM_LOG(OAM, OAM_ERROR,
					"Memory deallocation failed with error code %d", err_code);
			return OAM_FAILURE;
		}
	}
	/*SPR 13857 END*/


	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;

}
#endif
/***************************************************************************
 * Function Name  : oam_handle_pdcp_get_stats_cnf
 * Description    : This function sends stats configuration to OAM
 * Inputs         : p_api_buf  :pointer to PDCP get stats confirmation
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_handle_pdcp_get_stats_cnf
(
 void* p_api_buf,
 /*SPR 17777 fix*/
 UInt16 transaction_id,
 UInt16 dest_id,
 oam_error_code_et *p_error_code
 )
{

	/* Coverity CID 63294 Fix Start*/
	UInt8 *temp_arr = OAM_NULL;
	/*Coverity CID 63294 Fix End*/
	FILE *p_pdcp_stats=OAM_NULL;
	UInt8 cell_index = OAM_ZERO;
	oam_pdcp_stats_cnf_t *p_pdcp_stats_resp = OAM_NULL;
	p_pdcp_stats_resp  = OAM_NULL;
	oam_counter_t   loop_counter      = OAM_ZERO;
	oam_counter_t   loop_counter1     = OAM_ZERO;
	oam_counter_t   loop_counter2     = OAM_ZERO;

	OAM_FUN_ENTRY_TRACE();

	cell_index = (UInt8)(*((UInt8*)(p_api_buf) + OAM_INTF_HDR_CELL_IDX_OFFSET));
	if (!(cell_index == oam_prov_req.cell_cntxt.curr_cell_cid) &&
			(oam_get_rrm_cell_config_idx(cell_index) == OAM_ERROR_RETURN_VAL)) {
		oam_construct_n_send_get_resp_to_usr(OAM_RESULT_FAILURE,
				OAM_ERR_REQUEST_DENIED,
				OAM_VERSION_ID, OAM_MODULE_ID,
				dest_id, transaction_id, OAM_ZERO, OAM_ZERO,
				"Received Invalid Cell Index",
				p_error_code);
		OAM_LOG(OAM,OAM_INFO,"Received invalid cell_index:%d",cell_index);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	if (OAM_FAILURE == oam_mem_alloc(STR_LEN_128,(void *)&temp_arr,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to temp_arr failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;

	}

    /*SPR 20870 +*/
	p_api_buf            = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_pdcp_stats_resp    = (oam_pdcp_stats_cnf_t*)p_api_buf;
    if(p_pdcp_stats_resp->startUeId == 0)
    {
	p_pdcp_stats=oam_fopen("L2_PDCP_Stats.log","w+");
    }
    else
    {
	p_pdcp_stats=oam_fopen("L2_PDCP_Stats.log","a+");
    }
    /*SPR 20870 -*/
	if(OAM_NULL==p_pdcp_stats)
	{
		OAM_LOG(OAM,OAM_WARNING,"Unable to create file L2_PDCP_Stats.log for PDCP stats logging");
		/* Coverity ID 63294 Fix Start*/
		oam_mem_free((void *)temp_arr,p_error_code);
		/* Coverity ID 63294 Fix End*/
		return OAM_FAILURE;
	}

	/* Coverity CID 63294 Fix Start
	   Code Removed
	   Coverity CID 63294 Fix End*/
	if (OAM_SUCCESS == p_pdcp_stats_resp->responseCode)
	{   
    /*SPR 20870 +*/
        if(!p_pdcp_stats_resp->startUeId)
        {
		/* CSR_00058645_START */
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfPacketsIntegrityProtectedUsingEIA0",
				p_pdcp_stats_resp->statsIntegrity.numberOfPacketsIntegrityProtectedUsingEIA0);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfPacketsIntegrityVerifiedUsingEIA0",
				p_pdcp_stats_resp->statsIntegrity.numberOfPacketsIntegrityVerifiedUsingEIA0);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfPacketsIntegrityProtectedUsingEIA1",
				p_pdcp_stats_resp->statsIntegrity.numberOfPacketsIntegrityProtectedUsingEIA1);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfPacketsIntegrityVerifiedUsingEIA1",
				p_pdcp_stats_resp->statsIntegrity.numberOfPacketsIntegrityVerifiedUsingEIA1);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfPacketsIntegrityProtectedUsingEIA2", 
				p_pdcp_stats_resp->statsIntegrity.numberOfPacketsIntegrityProtectedUsingEIA2);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfPacketsIntegrityVerifiedUsingEIA2",
				p_pdcp_stats_resp->statsIntegrity.numberOfPacketsIntegrityVerifiedUsingEIA2 );
		/* CSR_00058645_START */
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfPacketsRejectedByIntegrityVerification",
				p_pdcp_stats_resp->statsIntegrity.numberOfPacketsRejectedByIntegrityVerification );
		/* CSR_00058645_END*/

		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfPacketsCipheredUsingEIA0",
				p_pdcp_stats_resp->statsCiphering.numberOfPacketsCipheredUsingEIA0);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfPacketsDecipheredUsingEIA0",
				p_pdcp_stats_resp->statsCiphering.numberOfPacketsDecipheredUsingEIA0);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfPacketsCipheredUsingEIA1",
				p_pdcp_stats_resp->statsCiphering.numberOfPacketsCipheredUsingEIA1);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfPacketsDecipheredUsingEIA1",
				p_pdcp_stats_resp->statsCiphering.numberOfPacketsDecipheredUsingEIA1);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfPacketsCipheredUsingEIA2",
				p_pdcp_stats_resp->statsCiphering.numberOfPacketsCipheredUsingEIA2);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfPacketsDecipheredUsingEIA2",
				p_pdcp_stats_resp->statsCiphering.numberOfPacketsDecipheredUsingEIA2);

		OAM_LOG(OAM,OAM_DETAILED,"--------------  ROHC STATS ------------------");

		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfIRPacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfIRPacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfIRPacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfIRPacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfIRDYNPacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfIRDYNPacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfIRDYNPacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfIRDYNPacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfACKProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfACKProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfACKCreated",
				p_pdcp_stats_resp->statsRohc.numberOfACKCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfNACKProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfNACKProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfNACKCreated",
				p_pdcp_stats_resp->statsRohc.numberOfNACKCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfSNACKProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfSNACKProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfSNACKCreated",
				p_pdcp_stats_resp->statsRohc.numberOfSNACKCreated);

		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfR0PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfR0PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfR0CRCPacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfR0CRCPacketsProcessed);
		/* CSR_00058645_START */
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfUO0PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfUO0PacketsProcessed);
		/* CSR_00058645_END */
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfR1PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfR1PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfR1IDPacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfR1IDPacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfR1TSPacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfR1TSPacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfUO1PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfUO1PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfUO1IDPacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfUO1IDPacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfUO1TSPacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfUO1TSPacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfUOR2PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfUOR2PacketsProcessed);

		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfUOR2IDPacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfUOR2IDPacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfUOR2TSPacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfUOR2TSPacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfR0PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfR0PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfR0CRCPacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfR0CRCPacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfUO0PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfUO0PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfR1PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfR1PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfR1IDPacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfR1IDPacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfR1TSPacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfR1TSPacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfUO1PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfUO1PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfUO1IDPacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfUO1IDPacketsCreated);

		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfUO1TSPacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfUO1TSPacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfUOR2PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfUOR2PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfUOR2IDPacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfUOR2IDPacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfUOR2TSPacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfUOR2TSPacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfModeTransU2OInit",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransU2OInit);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfModeTransU2RInit",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransU2RInit);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfModeTransO2RInit",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransO2RInit);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfModeTransO2UInit",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransO2UInit);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfModeTransR2UInit",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransR2UInit);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfModeTransR2OInit",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransR2OInit);

		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfTimesCrcFailed",
				p_pdcp_stats_resp->statsRohc.numberOfTimesCrcFailed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfModeTransU2ORcvd",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransU2ORcvd);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfModeTransU2RRcvd",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransU2RRcvd);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfModeTransO2RRcvd",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransO2RRcvd);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfModeTransO2URcvd",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransO2URcvd);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfModeTransR2URcvd",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransR2URcvd);
		/* CSR_00058645_START */
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfModeTransR2ORcvd",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransR2ORcvd);
		/* CSR_00058645_END */
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfP0NormalPacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfP0NormalPacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfP0NormalPacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfP0NormalPacketsProcessed);
		/* Rohc Profile 6 New Parameter ADD */ 
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfIRCRP6PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfIRCRP6PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfIRCRP6PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfIRCRP6PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfCOP6PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfCOP6PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfCOP6PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfCOP6PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfRND1P6PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfRND1P6PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfRND1P6PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfRND1P6PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfRND2P6PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfRND2P6PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfRND2P6PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfRND2P6PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfRND3P6PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfRND3P6PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfRND3P6PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfRND3P6PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfRND4P6PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfRND4P6PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfRND4P6PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfRND4P6PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfRND5P6PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfRND5P6PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfRND5P6PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfRND5P6PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfRND6P6PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfRND6P6PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfRND6P6PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfRND6P6PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfRND7P6PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfRND7P6PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfRND7P6PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfRND7P6PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfRND8P6PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfRND8P6PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfRND8P6PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfRND8P6PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfSEQ1P6PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfSEQ1P6PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfSEQ1P6PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfSEQ1P6PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfSEQ2P6PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfSEQ2P6PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfSEQ2P6PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfSEQ2P6PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfSEQ3P6PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfSEQ3P6PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfSEQ3P6PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfSEQ3P6PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfSEQ4P6PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfSEQ4P6PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfSEQ4P6PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfSEQ4P6PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfSEQ5P6PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfSEQ5P6PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfSEQ5P6PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfSEQ5P6PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfSEQ6P6PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfSEQ6P6PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfSEQ6P6PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfSEQ6P6PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfSEQ7P6PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfSEQ7P6PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfSEQ7P6PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfSEQ7P6PacketsProcessed);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfSEQ8P6PacketsCreated",
				p_pdcp_stats_resp->statsRohc.numberOfSEQ8P6PacketsCreated);
		oam_fprintf(p_pdcp_stats,"%s=%d\n","NumberOfSEQ8P6PacketsProcessed",
				p_pdcp_stats_resp->statsRohc.numberOfSEQ8P6PacketsProcessed);

		oam_fprintf(p_pdcp_stats,"%s=%d\n","StatNumActiveUe",
				p_pdcp_stats_resp->statNumActiveUe);
        }
		for (loop_counter = OAM_ZERO; loop_counter < p_pdcp_stats_resp->
				numOfUe;loop_counter++)
		{
			oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.crnti",
					p_pdcp_stats_resp->statsUEs_p[loop_counter].crnti);

			for (loop_counter1 = OAM_ZERO; loop_counter1 < p_pdcp_stats_resp->
					statsUEs_p[loop_counter].statNumActiveSrbLc;
					loop_counter1++)
			{
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.srbs.NumberOfDlSrbSduReceived",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].srbs[loop_counter1].numberOfDlSrbSduReceived);
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.srbs.NumberOfDlSrbPduTransmitted",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].srbs[loop_counter1].numberOfDlSrbPduTransmitted);
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.srbs.NumberOfSrbDeliveryIndSent",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].srbs[loop_counter1].numberOfSrbDeliveryIndSent);
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.srbs.NumberOfDlSrbPduDropNackRecvd",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].srbs[loop_counter1].numberOfDlSrbPduDropNackRecvd);
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.srbs.NumberOfDlSrbSduDiscarded",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].srbs[loop_counter1].numberOfDlSrbSduDiscarded);
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.srbs.NumberOfSrbUlPduReceived",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].srbs[loop_counter1].numberOfSrbUlPduReceived);
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.srbs.NumberOfSrbUlSduTransmitted",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].srbs[loop_counter1].numberOfSrbUlSduTransmitted);
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.srbs.NumberOfUlSrbSduDropped",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].srbs[loop_counter1].numberOfUlSrbSduDropped);
			}
			for (loop_counter2 = OAM_ZERO; loop_counter2 < p_pdcp_stats_resp->
					statsUEs_p[loop_counter].statNumActiveDrbLc;
					loop_counter2++)
			{
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.drbs.NumberOfDlDrbSduReceived",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].drbs[loop_counter2].numberOfDlDrbSduReceived);
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.drbs.NumberOfDlDrbPduTransmitted",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].drbs[loop_counter2].numberOfDlDrbPduTransmitted);
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.drbs.NumberOfDlDrbPduDropForNack",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].drbs[loop_counter2].numberOfDlDrbPduDropForNack);
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.drbs.NumberOfDlDrbPduDropForRoHCFailure",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].drbs[loop_counter2].numberOfDlDrbPduDropForRoHCFailure);
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.drbs.NumberOfDrbDeliveryIndSent",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].drbs[loop_counter2].numberOfDrbDeliveryIndSent);
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.drbs.NumberOfDlDrbSduDiscarded",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].drbs[loop_counter2].numberOfDlDrbSduDiscarded);
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.drbs.NumberOfDrbUlSduReceived",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].drbs[loop_counter2].numberOfDrbUlSduReceived);
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.drbs.NumberOfDrbUlPduTransmitted",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].drbs[loop_counter2].numberOfDrbUlPduTransmitted);
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.drbs.NumberOfDrbUlPduDropDueToRoHcFailure",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].drbs[loop_counter2].numberOfDrbUlPduDropDueToRoHcFailure);
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.drbs.NumberOfDrbUlPduDropAsSizeOORange",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].drbs[loop_counter2].numberOfDrbUlPduDropAsSizeOORange);
				oam_fprintf(p_pdcp_stats,"%s=%d\n","StatsUEs_p.drbs.NumberOfDrbUlPduDropAsReservedBitSet",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].drbs[loop_counter2].numberOfDrbUlPduDropAsReservedBitSet);
			}
			/* CSR_00058645_END */
		} 

		/* CSR_00058645_START */
		/* Code part remvoed here */
		/* CSR_00058645_END */
        if(p_pdcp_stats_resp->UesLeft == 0)
        {
		oam_sprintf((char *)temp_arr,"%s","PDCP Stats being too large has been dumped into file L2_PDCP_Stats.log at location LTE_IPR/bin.");
		/* CSR_00058645_START */
		oam_construct_n_send_gpv_response((UInt8 *)"L2 PDCP STATS",&temp_arr,OAM_ONE,
				transaction_id,p_error_code,dest_id,OAM_ZERO,OAM_ZERO);
		/* CSR_00058645_END */
		if(OAM_FAILURE == oam_mem_free((void *)temp_arr,p_error_code))
		{

			OAM_LOG(OAM,OAM_ERROR,"oam_mem_free failed ");

		}               
		temp_arr = OAM_NULL;
        }
        /*SPR 20870 +*/
	}               
	else
	{
		oam_construct_n_send_get_resp_to_usr(
				OAM_RESULT_FAILURE,
				OAM_ERR_INTERNAL_ERROR,
				OAM_VERSION,
				OAM_MODULE_ID,dest_id,
				transaction_id,
				OAM_ZERO,OAM_ZERO,"PDCP LAYER FAILURE RESPONSE",
				p_error_code
				);         

	}

	oam_fclose(p_pdcp_stats);
	/* Coverity ID 63294 Fix Start*/
	oam_mem_free((void *)temp_arr,p_error_code);
	/* Coverity ID 63294 Fix End*/
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_handle_mac_get_stats_cnf
 * Description    : This function handles stats cnf received from MAC
 * Inputs         : p_api_buf   :pointer to MAC get stats confirmation
 *                : msg_len     :message length
 * Outputs        : p_error_code:Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_handle_mac_get_stats_cnf
(
 void* p_api_buf,
 /*SPR 17777 fix*/
 UInt16 transaction_id,
 UInt16 dest_id,
 oam_error_code_et *p_error_code

 )
{
	oam_mac_stats_resp_t   *p_mac_stats_resp;
	oam_counter_t loop_counter   = OAM_ZERO;
	p_mac_stats_resp  = OAM_NULL;

	UInt8 **temp_arr = OAM_NULL;
	UInt8 size_x = OAM_ZERO; 
	UInt8 size_y = OAM_ZERO;
	/* suppose we want an array of char: a[4][100] */
	size_x = OAM_FOUR;
	size_y = OAM_HUNDRED;
	OAM_FUN_ENTRY_TRACE();


	/*BUG_PM*/
	UInt8 cell_index = (UInt8)(*((UInt8*)(p_api_buf) + OAM_INTF_HDR_CELL_IDX_OFFSET)); 
	if (!(cell_index == oam_prov_req.cell_cntxt.curr_cell_cid)&&
			(oam_get_rrm_cell_config_idx(cell_index) == OAM_ERROR_RETURN_VAL)) {
		oam_construct_n_send_get_resp_to_usr(OAM_RESULT_FAILURE,
				OAM_ERR_REQUEST_DENIED,
				OAM_VERSION_ID, OAM_MODULE_ID,
				dest_id, transaction_id, OAM_ZERO, OAM_ZERO,
				"Received Invalid Cell Index",
				p_error_code);

		OAM_LOG(OAM,OAM_INFO,"Received invalid cell_index:%d",cell_index);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	/*  allocate storage for an array of pointer to char */
	if (OAM_FAILURE == oam_mem_alloc((size_x * sizeof(UInt8 *)),(void *)&temp_arr,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to temp_arr failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;

	}

	/* for each pocharer, allocate storage for an array of chars */
	for (loop_counter = OAM_ZERO; loop_counter < size_x; loop_counter++)
	{
		if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)&temp_arr[loop_counter],p_error_code))
		{
			OAM_LOG(OAM, OAM_ERROR,
					"Memory allocation to temp_arr failed"
					" with Error Code:%d",
					*p_error_code);
			/*SPR 13857 START*/
			OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, loop_counter, *p_error_code);
			/*SPR 13857 END*/
			OAM_FUN_EXIT_TRACE();
			return OAM_FAILURE;
		} 
		oam_memset(temp_arr[loop_counter], OAM_ZERO, (size_y * sizeof(UInt8)));
	}

	p_api_buf            = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_mac_stats_resp    = (oam_mac_stats_resp_t*)p_api_buf;

	if (OAM_SUCCESS == p_mac_stats_resp->responseCode)
    {  
        /* + SPR 19808 */
        oam_sprintf((char *)temp_arr[OAM_ZERO],"%s=%lf","dlThroughPut(bits per sec)",
                p_mac_stats_resp->dlThroughPut);
        oam_sprintf((char *)temp_arr[1],"%s=%lf","ulThroughPut(bits per sec)",
                p_mac_stats_resp->ulThroughPut);
        /* - SPR 19808 */
        oam_sprintf((char *)temp_arr[2],"%s=%lf","dlSpectralEfficiency",
                p_mac_stats_resp->dlSpectralEfficiency);
        oam_sprintf((char *)temp_arr[3],"%s=%lf","ulSpectralEfficiency",
                p_mac_stats_resp->ulSpectralEfficiency);
        oam_construct_n_send_gpv_response((UInt8 *)"Show.Stats.L2.MAC",temp_arr,4,
                transaction_id,p_error_code,dest_id,OAM_ZERO,OAM_ZERO);
    }               
	else
	{
		OAM_LOG(OAM,OAM_INFO,"Received error in response from MAC in MAC_GET_STATS_CNF");
		oam_construct_n_send_get_resp_to_usr(
				OAM_RESULT_FAILURE,
				OAM_ERR_INTERNAL_ERROR,
				OAM_VERSION,
				OAM_MODULE_ID,dest_id,
				transaction_id,
				OAM_ZERO,OAM_ZERO,"MAC LAYER FAILURE RESPONSE", 
				p_error_code     
				);
	}
	/*SPR 13857 START*/
	OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, size_x, *p_error_code);
	/*SPR 13857 END*/

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_handle_rlc_get_stats_cnf
 * Description    : This function Handles the stats cnf frm RLC
 * Inputs         : p_api_buf : Pointer to API Message
 *                : msg_len   : Total API Message length
 * Outputs        : p_error_code  : Pointer to error message
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_handle_rlc_get_stats_cnf
(
 void* p_api_buf,
 /*SPR 17777 fix*/
 UInt16 transaction_id,
 UInt16 dest_id,
 oam_error_code_et *p_error_code

 )
{
  oam_counter_t loop_counter   = OAM_ZERO;
  /* SPR 21415 Fix Start */
  UInt8 *temp_arr = OAM_NULL;
  FILE *p_rlc_stats=OAM_NULL;
  /* SPR 21415 Fix End */
  UInt32 size_x = OAM_ZERO; 
  UInt32 size_y = OAM_ZERO;
  /* SPR 21415 Fix Start */
  /* Code removed */
  /* SPR 21415 Fix End */
  UInt32 count =OAM_ZERO;

  Oam_Rlc_Get_Stat_Cnf_t *p_oam_rlc_get_stats_cnf = OAM_NULL;
  p_oam_rlc_get_stats_cnf  = OAM_NULL;
  oam_counter_t num_of_active_UE = OAM_ZERO;

  /* suppose we want an array of char: a[4][100] */
  size_x = OAM_ZERO;
  size_y = OAM_HUNDRED;
  OAM_FUN_ENTRY_TRACE();


  p_api_buf            = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
  p_oam_rlc_get_stats_cnf = (Oam_Rlc_Get_Stat_Cnf_t *)p_api_buf;

  /* calculating count, use for memory allocation */ 
  if (OAM_SUCCESS == p_oam_rlc_get_stats_cnf->responseCode)
  { 
    count = p_oam_rlc_get_stats_cnf->lteUEStat.numOfActiveUE;
    size_x =  count*5;  
  } 
  size_x = size_x + 19;

  /*  allocate storage for an array of pointer to char */
  if (OAM_FAILURE == oam_mem_alloc((size_x * sizeof(UInt8 *)),(void *)&temp_arr,p_error_code))
  {    
      OAM_LOG(OAM, OAM_ERROR,
              "Memory allocation to temp_arr failed"
              " with Error Code:%d",
        *p_error_code);
    OAM_FUN_EXIT_TRACE();
    return OAM_FAILURE;
  }    
  /* SPR 21415 Fix Start */
  p_rlc_stats=oam_fopen(OAM_RLC_STATS_LOG_FILE_NAME,"w+");

  if(OAM_NULL == p_rlc_stats)
  {
    OAM_LOG(OAM,OAM_WARNING,"Unable to create file %s for RLC stats logging",OAM_RLC_STATS_LOG_FILE_NAME);
    oam_mem_free((void *)temp_arr,p_error_code);
    OAM_FUN_EXIT_TRACE();
    return OAM_FAILURE;
  }
  /* SPR 21415 Fix End */

  /* for each pocharer, allocate storage for an array of chars */
  for (loop_counter = 0; loop_counter < size_x; loop_counter++)
  {    
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)&temp_arr[loop_counter],p_error_code))
    {    
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to temp_arr failed"
                " with Error Code:%d",
          *p_error_code);
      /* SPR 21415 Fix Start */
      oam_mem_free((void *)temp_arr,p_error_code);
      /* CID Fix 114896 +*/
      oam_fclose(p_rlc_stats);
      /* CID Fix 114896 -*/
      /* SPR 21415 Fix End */
      OAM_FUN_EXIT_TRACE();
      return OAM_FAILURE;
    }    
  }  

  if (OAM_SUCCESS == p_oam_rlc_get_stats_cnf->responseCode)
  {   
    /* SPR 21415 Fix Start */
    oam_fprintf(p_rlc_stats,"%s=%d\n",
        "LteRlcTMStatInfo.rxTmdPdu",p_oam_rlc_get_stats_cnf->lteTMStat.rxTmdPdu);
    oam_fprintf(p_rlc_stats,"%s=%d\n",
        "LteTMStat.txTmdPdu",p_oam_rlc_get_stats_cnf->lteTMStat.txTmdPdu);
    oam_fprintf(p_rlc_stats,"%s=%d\n","LteUMStat.rxUmdSduDiscarded",
        p_oam_rlc_get_stats_cnf->lteUMStat.rxUmdSduDiscarded);
    oam_fprintf(p_rlc_stats,"%s=%d\n","LteUMStat.txUmdPdu",
        p_oam_rlc_get_stats_cnf->lteUMStat.txUmdPdu);
    oam_fprintf(p_rlc_stats,"%s=%d\n","LteUMStat.rxUmdSduTxUL",
        p_oam_rlc_get_stats_cnf->lteUMStat.rxUmdSduTxUL);
    oam_fprintf(p_rlc_stats,"%s=%d\n","LteUMStat.rxIncompleteUmdSduDiscarded",
        p_oam_rlc_get_stats_cnf->lteUMStat.rxIncompleteUmdSduDiscarded);
    oam_fprintf(p_rlc_stats,"%s=%d\n","LteUMStat.rxUmdPdu",
        p_oam_rlc_get_stats_cnf->lteUMStat.rxUmdPdu);
    oam_fprintf(p_rlc_stats,"%s=%d\n","LteUMStat.rxUmdPduDropped",
        p_oam_rlc_get_stats_cnf->lteUMStat.rxUmdPduDropped);
    oam_fprintf(p_rlc_stats,"%s=%d\n","LteUMStat.umdSduReceivedFromPdcp",
        p_oam_rlc_get_stats_cnf->lteUMStat.umdSduReceivedFromPdcp);
    oam_fprintf(p_rlc_stats,"%s=%d\n","LteUMStat.umdSduTransmittedToMac",
        p_oam_rlc_get_stats_cnf->lteUMStat.umdSduTransmittedToMac);
    oam_fprintf(p_rlc_stats,"%s=%d\n","LteAMStat.amdPduTransmitted",
        p_oam_rlc_get_stats_cnf->lteAMStat.amdPduTransmitted);
    oam_fprintf(p_rlc_stats,"%s=%d\n","LteAMStat.amdPduReceived",
        p_oam_rlc_get_stats_cnf->lteAMStat.amdPduReceived);
    oam_fprintf(p_rlc_stats,"%s=%d\n","LteAMStat.amdPduDropped",
        p_oam_rlc_get_stats_cnf->lteAMStat.amdPduDropped);
    oam_fprintf(p_rlc_stats,"%s=%d\n","LteAMStat.amdIncompSduDiscard",
        p_oam_rlc_get_stats_cnf->lteAMStat.amdIncompSduDiscard);
    oam_fprintf(p_rlc_stats,"%s=%d\n","LteAMStat.amdSduReceivedFromPdcp",
        p_oam_rlc_get_stats_cnf->lteAMStat.amdSduReceivedFromPdcp);
    oam_fprintf(p_rlc_stats,"%s=%d\n","LteAMStat.amdSduTransmittedToPdcp",
        p_oam_rlc_get_stats_cnf->lteAMStat.amdSduTransmittedToPdcp);
    oam_fprintf(p_rlc_stats,"%s=%d\n","LteAMStat.amdSduDropped",
        p_oam_rlc_get_stats_cnf->lteAMStat.amdSduDropped);
    oam_fprintf(p_rlc_stats,"%s=%d\n","LteAMStat.amdSduTransmitedToMac",
        p_oam_rlc_get_stats_cnf->lteAMStat.amdSduTransmitedToMac);
    oam_fprintf(p_rlc_stats,"%s=%d\n","LteUEStat.numOfActiveUE",
        p_oam_rlc_get_stats_cnf->lteUEStat.numOfActiveUE);
    /* SPR 21415 Fix End */


    for (num_of_active_UE = OAM_ZERO;
        num_of_active_UE < p_oam_rlc_get_stats_cnf->lteUEStat.numOfActiveUE;
        num_of_active_UE++)
    {
      /* SPR 21415 Fix Start */
      oam_fprintf(p_rlc_stats,"%s=%d\n","LteUEStat.numOfActiveUE.crnti",
          p_oam_rlc_get_stats_cnf->lteUEStat.rlcUeStats[num_of_active_UE].crnti);
      oam_fprintf(p_rlc_stats,"%s=%d\n","LteUEStat.numOfActiveUE.sduReceivedFromPdcp",
          p_oam_rlc_get_stats_cnf->lteUEStat.rlcUeStats[num_of_active_UE].sduReceivedFromPdcp);
      oam_fprintf(p_rlc_stats,"%s=%d\n",
          "LteUEStat.numOfActiveUE.pduTransmittedToMac",p_oam_rlc_get_stats_cnf->
          lteUEStat.rlcUeStats[num_of_active_UE].pduTransmittedToMac);
      oam_fprintf(p_rlc_stats,"%s=%d\n","LteUEStat.numOfActiveUE.sduTransmittedToPdcp",
          p_oam_rlc_get_stats_cnf->lteUEStat.rlcUeStats[num_of_active_UE].sduTransmittedToPdcp);
      oam_fprintf(p_rlc_stats,"%s=%d\n","LteUEStat.numOfActiveUE.pduReceivedFromMac",
          p_oam_rlc_get_stats_cnf->lteUEStat.rlcUeStats[num_of_active_UE].pduReceivedFromMac);
      /* SPR 21415 Fix End */
    }
    /* SPR 21415 Fix Start */
    /* Code Removed */
    oam_sprintf((char *)temp_arr,"%s %s %s","RLC Stats being too large has been dumped into file",OAM_RLC_STATS_LOG_FILE_NAME,
         "at location LTE_IPR/bin.");
    oam_construct_n_send_gpv_response((UInt8 *)"Show.Stats.L2.RLC",&temp_arr,OAM_ONE,
        transaction_id,p_error_code,dest_id,OAM_ZERO,OAM_ZERO);
    /* SPR 21415 Fix End */
  }               
  else
  {
    oam_construct_n_send_get_resp_to_usr(
        OAM_RESULT_FAILURE,
        OAM_ERR_INTERNAL_ERROR,
        OAM_VERSION,
        OAM_MODULE_ID,dest_id,
        transaction_id,
        OAM_ZERO,OAM_ZERO,"RLC LAYER FAILURE RESPONSE",
        p_error_code
        );

  }
  /* SPR 21415 Fix Start */
  oam_fclose(p_rlc_stats);
  if(OAM_FAILURE == oam_mem_free((void *)temp_arr,p_error_code))
  {

    OAM_LOG(OAM,OAM_ERROR,"oam_mem_free failed ");

  }               
  temp_arr = OAM_NULL;
  /* SPR 21415 Fix End */


  OAM_FUN_EXIT_TRACE();
  return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_s1ap_handle_stats_resp
 * Description    : This function handles the S1AP OAM Stats Response
 * Inputs         : p_api
 *                : data_len
 * Outputs        : p_error_code :Error Code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
	oam_s1ap_handle_stats_resp
(
 void* p_api,
 UInt16            trans_id,
 UInt16            dest_id,
 oam_error_code_et *p_error_code
 )
{
	UInt32 count = OAM_ZERO;
	oam_counter_t loop_counter   = OAM_ZERO;
	UInt8 **temp_arr = OAM_NULL;
	UInt32 size_x = OAM_ZERO;
	UInt32 size_y = OAM_ZERO;
	UInt32 str_count = OAM_ZERO;

	SInt32   length         = OAM_ZERO;
	UInt8    no_of_mme      = OAM_ZERO;
	UInt8    mme_index      = OAM_ZERO;
	/* CSR 00058587 Fix Start */
	oam_s1ap_status_resp_t oam_s1ap_status_resp;
	/* CSR 00058587 Fix End */
	UInt16    data_len        = OAM_ZERO;

	size_x = OAM_ZERO;
	size_y = OAM_HUNDRED;

	OAM_FUN_ENTRY_TRACE();
	data_len = get_word_from_header((U8*)(p_api) + 8);

	p_api = p_api + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	/* CSR 00058587 Fix Start */
	if (OAM_FAILURE == oam_rrc_il_parse_s1ap_oam_status_resp(
				&oam_s1ap_status_resp,
				(UInt8*)p_api,
				(data_len - LOWER_LAYER_INTERFACE_HEADER_SIZE),
				&length))
		/* CSR 00058587 Fix End */
	{
		OAM_LOG(OAM,OAM_WARNING,"S1AP_OAM_STATUS_RESP Parsing Failure");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	/* CSR 00058587 Fix Start */
	no_of_mme = oam_s1ap_status_resp.active_mme_count;
	/* CSR 00058587 Fix End */
	count = no_of_mme; 
	size_x =  count*2;
	size_x = size_x +1;
	if (OAM_ZERO == no_of_mme)
	{
		size_x = size_x +2;/*Adding two more stringd for 0 reporting*/
	}

	/*  allocate storage for an array of pointer to char */
	if (OAM_FAILURE == oam_mem_alloc((size_x * sizeof(UInt8 *)),(void *)&temp_arr,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to temp_arr failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;

	}

	/* for each pocharer, allocate storage for an array of chars */
	for (loop_counter = OAM_ZERO; loop_counter < size_x; loop_counter++)
	{
		if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)&temp_arr[loop_counter],p_error_code))
		{
			OAM_LOG(OAM, OAM_ERROR,
					"Memory allocation to temp_arr failed"
					" with Error Code:%d",
					*p_error_code);
			/*SPR 13857 START*/
			OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, loop_counter, *p_error_code);
			/*SPR 13857 END*/
			OAM_FUN_EXIT_TRACE();
			return OAM_FAILURE;
		}
	}

	/* Getting the mme count value */
	OAM_LOG(OAM,OAM_DETAILED,"S1AP_OAM_STATUS_RESP with Active MME : %d",no_of_mme);

	oam_sprintf((char *)temp_arr[str_count++],"%s=%d","Active MME ",no_of_mme);

	for (mme_index = OAM_ZERO; mme_index < no_of_mme; mme_index++)
	{
		/* CSR 00058587 Fix Start */
		oam_sprintf((char *)temp_arr[str_count++],"%s=%d","MME Id",
				oam_s1ap_status_resp.active_mme_list[mme_index].mme_id);
		OAM_LOG(OAM,OAM_DETAILED,"MME[%d] MME Id : %d. Number of Active UE : %d.",
				(mme_index+1), oam_s1ap_status_resp.active_mme_list[mme_index].mme_id, 
				oam_s1ap_status_resp.active_mme_list[mme_index].count_of_ue);

		oam_sprintf((char *)temp_arr[str_count++],"%s=%d","Number of Active UE",
				oam_s1ap_status_resp.active_mme_list[mme_index].count_of_ue);
		/* CSR 00058587 Fix End*/
	}

	if (OAM_ZERO == no_of_mme)
	{
		oam_sprintf((char *)temp_arr[str_count++],"%s=%d","MME Id", no_of_mme);
		oam_sprintf((char *)temp_arr[str_count++],"%s=%d","Number of Active UE", no_of_mme);
	}
	oam_construct_n_send_gpv_response((UInt8 *) "Show.Stats.L3.S1AP",temp_arr,str_count,
			trans_id,p_error_code,dest_id,OAM_ZERO,OAM_ZERO);

	/*SPR 13857 START*/
	OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, size_x, *p_error_code);
	/*SPR 13857 END*/

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_send_mac_reset_stats_req
 * Description    : This function sends reset stats request to MAC
 * Inputs         : src_module_id  :Source module identifier
 *                : transaction_id :transaction identifier
 * Outputs        : p_error_code   :pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
	oam_send_mac_reset_stats_req
(
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg = OAM_NULL;
	p_msg     = OAM_NULL;
	UInt8 *p_mac_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_api_length = OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			MAC_MODULE_ID, MAC_RESET_STATS_REQ, msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
			MAC_MODULE_ID, MAC_RESET_STATS_REQ, OAM_ZERO,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Send message to MAC */
	if(OAM_FAILURE == oam_send_message(p_msg, MAC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending MAC_RESET_STATS_REQ message to MAC with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_handle_mac_reset_stats_cnf
 * Description    : This function handles RESET STATS cnf received from MAC
 * Inputs         : p_api_buf  :pointer to MAC reset stats config
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
	oam_handle_mac_reset_stats_cnf
(
 void* p_api_buf,
 UInt16 trans_id,
 UInt16 dest_id 
 )
{
	oam_mac_reset_stats_resp_t *p_mac_reset_stats_resp;
	p_mac_reset_stats_resp = OAM_NULL;
	oam_error_code_et p_error_code = NO_ERROR; 
	UInt8 temp_ret_staus = OAM_ZERO;
	OAM_FUN_ENTRY_TRACE();
	UInt8 cell_index = (UInt8)(*((UInt8*)(p_api_buf) + OAM_INTF_HDR_CELL_IDX_OFFSET));
	if (!(cell_index == oam_prov_req.cell_cntxt.curr_cell_cid)&&
			(oam_get_rrm_cell_config_idx(cell_index) == OAM_ERROR_RETURN_VAL)) {
		OAM_LOG(OAM,OAM_INFO,"Received invalid cell_index:%d",cell_index);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	p_api_buf           = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_mac_reset_stats_resp = (oam_mac_reset_stats_resp_t *)p_api_buf;

	if(OAM_ONE ==  p_mac_reset_stats_resp->responseCode)
	{
		temp_ret_staus = OAM_ZERO;
	}
	else if(OAM_ZERO  == p_mac_reset_stats_resp->responseCode)
	{
		temp_ret_staus = OAM_ONE;
	}
	oam_construct_n_send_set_resp_to_usr(temp_ret_staus,OAM_ZERO,OAM_NULL,OAM_ZERO,OAM_VERSION_ID,
			OAM_MODULE_ID, dest_id, trans_id, OAM_ZERO, OAM_ZERO,
			&p_error_code);
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_send_pdcp_reset_stats_req
 * Description    : This function sends reset stats request to PDCP
 * Inputs         : src_module_id  :Source module identifier
 *                : transaction_id :transaction identifier
 * Outputs        : p_error_code   :pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
	oam_send_pdcp_reset_stats_req
(
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg = OAM_NULL;
	p_msg = OAM_NULL;
	UInt8 *p_pdcp_msg = OAM_NULL;
	p_pdcp_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_api_length = OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_pdcp_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_pdcp_msg, OAM_VERSION_ID, src_module_id,
			PDCP_MODULE_ID, PDCP_RESET_STATS_REQ, msg_api_length);

	/* Fill interface header */
	p_pdcp_msg = p_pdcp_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
			PDCP_MODULE_ID, PDCP_RESET_STATS_REQ,OAM_ZERO,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Send message to PDCP */
	if(OAM_FAILURE == oam_send_message(p_msg, PDCP_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending PDCP_RESET_STATS_REQ message to PDCP with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_handle_pdcp_reset_stats_cnf
 * Description    : Handles pdcp reset stats cnf received from PDCP
 * Inputs         : p_api_buf :pointer to PDCP reset stats confirmation
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
	oam_handle_pdcp_reset_stats_cnf
(
 void *p_api_buf
 )
{
	oam_pdcp_cnf_t *p_pdcp_cnf = OAM_NULL;
	p_pdcp_cnf  = OAM_NULL;
	oam_error_code_et p_error_code = NO_ERROR;
	UInt16 trans_id = OAM_ZERO;
	UInt16 trans_id_resp = OAM_ZERO;    
	UInt16 dest_id = OAM_ZERO;
	UInt8 temp_ret_staus = OAM_ZERO;
	UInt8 cell_index = OAM_ZERO;
	oam_error_code_et err = NO_ERROR;
	OAM_FUN_ENTRY_TRACE();
	/* SPR 9620 CHANGE START */
	trans_id = get_word_from_header(p_api_buf);
#ifndef OAM_UT_FRAMEWORK
	if(OAM_FAILURE == oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(trans_id, &trans_id_resp, &dest_id))
	{
		OAM_LOG(OAM,OAM_INFO, "oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id function failed");
		return OAM_FAILURE;
	}
#endif
	/* SPR 9620 CHANGE END */    

	cell_index = (UInt8)(*((UInt8*)(p_api_buf) + OAM_INTF_HDR_CELL_IDX_OFFSET));
	if (!(cell_index == oam_prov_req.cell_cntxt.curr_cell_cid) &&
			(oam_get_rrm_cell_config_idx(cell_index) == OAM_ERROR_RETURN_VAL)) {
		oam_construct_n_send_get_resp_to_usr(OAM_RESULT_FAILURE,
				OAM_ERR_REQUEST_DENIED,
				OAM_VERSION_ID, OAM_MODULE_ID,
				dest_id, trans_id, OAM_ZERO, OAM_ZERO,
				"Received Invalid Cell Index",
				&err);

		OAM_LOG(OAM,OAM_INFO,"Received invalid cell_index:%d",cell_index);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	p_api_buf    = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_pdcp_cnf   = (oam_pdcp_cnf_t *)p_api_buf;

	if(OAM_ONE ==  p_pdcp_cnf->responseCode)
	{
		temp_ret_staus = OAM_ZERO;
	}
	else if(OAM_ZERO  == p_pdcp_cnf->responseCode)
	{
		temp_ret_staus = OAM_ONE;
	}
	/* SPR_12955_FIXES */	
	oam_construct_n_send_set_resp_to_usr(temp_ret_staus,OAM_ZERO,OAM_NULL,OAM_ZERO,OAM_VERSION_ID,
			OAM_MODULE_ID,dest_id, trans_id_resp, OAM_ZERO, OAM_ZERO,
			&p_error_code);
	/* SPR_12955_FIXES */	
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_send_rlc_reset_stats_req
 * Description    : This function sends reset stats request to RLC
 * Inputs         : src_module_id : Source module identifier
 *                : transaction_id : Interface transaction identifier
 * Outputs        : p_error_code : pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
	oam_send_rlc_reset_stats_req
(
 UInt16 src_module_id,
 UInt16 transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_length_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg = OAM_NULL;
	UInt8 *p_rlc_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();
	msg_api_length = OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	OAM_LOG(OAM,OAM_DETAILED,"RLC_RESET_STATS_REQ with length %d",msg_api_length);

	/* Allocate buffer */
	if(OAM_FAILURE  == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	p_rlc_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_rlc_msg, OAM_VERSION_ID, src_module_id,
			RLC_MODULE_ID, RLC_RESET_STATS_REQ, msg_api_length);

	/* Fill interface header */
	p_rlc_msg = p_rlc_msg + OAM_CSPL_HDR_SIZE;

	oam_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
			RLC_MODULE_ID, RLC_RESET_STATS_REQ, OAM_ZERO,
			oam_prov_req.cell_cntxt.curr_cell_cid);


	/* Send message to */
	if(OAM_FAILURE == oam_send_message(p_msg, RLC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending RLC_RESET_STATS_REQ message to RLC with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_handle_rlc_reset_stats_cnf
 * Description    : Handles RLC reset stats cnf received from RLC
 * Inputs         : p_api_buf :pointer to RLC reset stats confirmation
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
	oam_handle_rlc_reset_stats_cnf
(
 void *p_api_buf
 )
{
	Oam_Rlc_Cnf_t *p_rlc_reset_stats_cnf = OAM_NULL;
	oam_error_code_et p_error_code = NO_ERROR;
	UInt16 trans_id = OAM_ZERO;
	UInt16 dest_id = OAM_ZERO;
	UInt16 trans_id_resp = OAM_ZERO;    
	UInt8 temp_ret_staus = OAM_ZERO;

	OAM_FUN_ENTRY_TRACE();

	/* SPR 9620 CHANGE START */
	trans_id = get_word_from_header(p_api_buf);

#ifndef OAM_UT_FRAMEWORK
	if(OAM_FAILURE == oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(trans_id, &trans_id_resp, &dest_id))
	{
		OAM_LOG(OAM,OAM_INFO, "oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id function failed");
		return OAM_FAILURE;
	}
#endif
	/* SPR 9620 CHANGE END */


	p_api_buf    = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_rlc_reset_stats_cnf = (Oam_Rlc_Cnf_t *)p_api_buf;

	if(OAM_ONE ==  p_rlc_reset_stats_cnf->responseCode)
	{
		temp_ret_staus = OAM_ZERO;
	}
	else if(OAM_ZERO  == p_rlc_reset_stats_cnf->responseCode)
	{
		temp_ret_staus = OAM_ONE;
	}
	/* SPR_12955_FIXES */	
	oam_construct_n_send_set_resp_to_usr(temp_ret_staus,OAM_ZERO,OAM_NULL,OAM_ZERO,OAM_VERSION_ID,
			OAM_MODULE_ID,dest_id, trans_id_resp, OAM_ZERO, OAM_ZERO,
			&p_error_code);
	/* SPR_12955_FIXES */	
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_set_bit_in_array_lsb
 * Description    : 
 * Inputs         :  :pointer to RLC reset stats confirmation
 * Outputs        : none
 * Returns        : 
 ****************************************************************************/
void
	oam_set_bit_in_array_lsb
(
 UInt8   *bit_array,
 UInt16  bit_num,
 UInt16  array_size
 )
{
	UInt16   search_byte = OAM_ZERO;
	UInt16   search_bit = OAM_ZERO;
	UInt8    mask  = OAM_ZERO;
	UInt16   bit_shift = OAM_ONE;

	search_byte = bit_num/8;
	array_size = array_size -1;
	array_size = array_size - search_byte;
	search_bit = bit_num%8;
	mask  = bit_shift << (search_bit);
	bit_array[array_size] = bit_array[array_size]|mask;
}


/****************************************************************************
 ** Function Name  : oam_bit_read_in_array_lsb
 ** Inputs         : it takes array and bit no as input.
 ** Outputs        : none
 ** Returns        : unsigned char  
 ** Description    : This function returns value of a particular bit.
 *****************************************************************************/
UInt8  oam_bit_read_in_array_lsb
(
 UInt8	 *bit_array,
 UInt16	 bit_num
 )
{
	OAM_FUN_ENTRY_TRACE();

	UInt16 search_byte = OAM_ZERO;
	UInt16 search_bit = OAM_ZERO;
	UInt8 mask  = OAM_ZERO;
	UInt16 bit_shift = OAM_ONE;
	UInt8 bit_val = OAM_ZERO;

	search_byte = bit_num/OAM_NO_OF_BITS;
	search_bit = bit_num%OAM_NO_OF_BITS;
	mask = bit_shift<<(search_bit);

	bit_val=bit_array[search_byte]&mask;

	if (OAM_ZERO == bit_val)
	{
		return OAM_ZERO;
	}
	return OAM_ONE;
}

/*spr 19296 fix start*/

/**************************************************************************************
 * Function Name -      add_to_get_mac_thp_batch_link_list
 * Description -        This function adds the MAC_GET_KPI_THP_STATS_CNF data to 
 *			link list 
 * Inputs -             p_oam_mac_kpi_thp_stats_ind - pointer to MAC_GET_KPI_THP_STATS
 *			data 
 *			error_code - pointer to oam_error_code_et
 * Outputs -            NONE
 * Returns -            NONE
 **************************************************************************************/
void
	add_to_get_mac_thp_batch_link_list
(
 oam_mac_KPI_THP_stats_ind_t *p_oam_mac_kpi_thp_stats_ind,
 oam_error_code_et *p_error_code
 )
{
	OAM_FUN_ENTRY_TRACE();
	oam_mac_thp_batch_node *temp = OAM_NULL;
	oam_mac_thp_batch_node *current = OAM_NULL;

	/*
	 * DESCRIPTION: Here function creates singly link list with node containing of type
	 * "oam_mac_KPI_THP_stats_ind_t".
	 */

	if (OAM_FAILURE == oam_mem_alloc(sizeof(oam_mac_thp_batch_node),(void *)&temp,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to temp failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return ;
	}
	oam_memcpy(&temp->data, p_oam_mac_kpi_thp_stats_ind,sizeof(oam_mac_KPI_THP_stats_ind_t));
	temp->next = OAM_NULL;
	if(ConsolidatedGetMacThpStats == OAM_NULL)
	{
		ConsolidatedGetMacThpStats = temp;
	}
	else
	{
		current = ConsolidatedGetMacThpStats;
		while(current->next != OAM_NULL)
		{
			current = current->next;
		}
		current->next = temp;
	}
}

/***********************************************************************************
 * Function Name -      oam_handle_get_mac_thp_batch_data
 * Description -        This function handles the batches of MAC_KPI_THP_STATS_IND
 *			and adds the data into link list
 * Inputs -             p_api_buf  - void type pointer as buffer containing 
 *			MAC_KPI_THP_STATS_IND data.
 *			error_code - pointer to oam_error_code_et
 * Outputs -            NONE
 * Returns -            OAM_SUCCESS/OAM_FAILURE
 ************************************************************************************/
oam_return_et
	oam_handle_get_mac_thp_batch_data
(
 void *p_api_buf, 
 oam_error_code_et *p_error_code
 )
{
	oam_mac_KPI_THP_stats_ind_t *p_oam_mac_kpi_thp_stats_ind = OAM_NULL;
	oam_return_et ret_val = OAM_FAILURE;
	OAM_FUN_ENTRY_TRACE();

	p_api_buf                    = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_oam_mac_kpi_thp_stats_ind = (oam_mac_KPI_THP_stats_ind_t *)p_api_buf;
	add_to_get_mac_thp_batch_link_list(p_oam_mac_kpi_thp_stats_ind, p_error_code);
	if(p_oam_mac_kpi_thp_stats_ind->endIndicator == OAM_ONE)
	{
		OAM_LOG(OAM, OAM_INFO, "Received end indicator value %u",
				p_oam_mac_kpi_thp_stats_ind->endIndicator);
		ret_val = OAM_SUCCESS;

	}
	else
	{
		OAM_LOG(OAM, OAM_INFO, "Received end indicator value %u",
				p_oam_mac_kpi_thp_stats_ind->endIndicator);
	}
	OAM_FUN_EXIT_TRACE();
	return ret_val;
}

/* SPR 14019 FIX Start */
/****************************************************************************
 ** Function Name  : oam_pm_mac_updat_nxt_node_ptr_of_link_lst
 ** Description    : This function update next node pointer field of link_lst
 ** Inputs         : p_kpi_info : Pointer to structure oam_mac_kpi_info_t 
 ** Outputs        : p_oam_pm_kpi_info_Node : pointer to structure oam_pm_mac_kpi_info_Node_t
 ** Returns        : NONE 
 *****************************************************************************/
void
	oam_pm_mac_updat_nxt_node_ptr_of_link_lst
(
 oam_mac_kpi_info_t *p_kpi_info,
 oam_pm_mac_kpi_info_node_t *p_oam_pm_mac_kpi_info_Node
 )
{
	OAM_FUN_ENTRY_TRACE();
	oam_pm_mac_kpi_info_node_t *p_pm_mac_kpi_info_Node_tmp = OAM_NULL;

	OAM_ASSERT(OAM_NULL != p_oam_pm_mac_kpi_info_Node);
	OAM_ASSERT(OAM_NULL != p_kpi_info);

	if (OAM_NULL != p_kpi_info)
	{
		if(p_kpi_info->pm_mac_kpi_info_first_Node == OAM_NULL)
		{
			if(OAM_NULL != p_oam_pm_mac_kpi_info_Node)
			{
				p_kpi_info->pm_mac_kpi_info_first_Node = p_oam_pm_mac_kpi_info_Node;
			}
		}
		else
		{
			p_pm_mac_kpi_info_Node_tmp = p_kpi_info->pm_mac_kpi_info_first_Node;

			while(p_pm_mac_kpi_info_Node_tmp->next_node_ptr != OAM_NULL)
			{
				p_pm_mac_kpi_info_Node_tmp  = p_pm_mac_kpi_info_Node_tmp->next_node_ptr;

			}
			if(OAM_NULL != p_oam_pm_mac_kpi_info_Node)
			{
				p_pm_mac_kpi_info_Node_tmp->next_node_ptr = p_oam_pm_mac_kpi_info_Node;
			}
		}
	}
	else
	{
		OAM_LOG(OAM, OAM_INFO,"NULL value of p_kpi_info received");
		OAM_FUN_EXIT_TRACE();
	}
	OAM_FUN_EXIT_TRACE();
}

/****************************************************************************
 ** Function Name  : oam_pm_pdcp_kpi_stat_ind 
 ** Description    : function fill oam_pdcp_KPI_stats_ind_t structure from 
 **                : p_api_buf buf  
 ** Inputs         : p_api_buf : buffer for msg storage
 ** Outputs        : p_KPI_stats_ind : pointer to structure oam_pdcp_KPI_stats_ind_t
 **Returns        : OAM_SUCCESS 
 *****************************************************************************/
oam_return_et
	oam_pm_mac_kpi_stat_ind
(
 void* p_api_buf,
 oam_mac_KPI_stats_ind_t *p_KPI_stats_ind
 )
{
	oam_counter_t loop_counter = OAM_ZERO;
    /* SPR 23499 Fix Start */
	oam_counter_t operator_count = OAM_ZERO;
    /* SPR 23499 Fix End */
	oam_mac_KPI_stats_ind_t *p_oam_mac_kpi_stats_ind;
	p_oam_mac_kpi_stats_ind = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();
	OAM_ASSERT(OAM_NULL != p_KPI_stats_ind);

	p_api_buf                    = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_oam_mac_kpi_stats_ind     = (oam_mac_KPI_stats_ind_t *)p_api_buf;

	/* To Create Current Time Stamp Value */
	create_time_stamp();
	OAM_LOG(OAM,OAM_INFO, "Received MAC_KPI_STATS_IND. Current TimeStamp[%s]."
            " MAC_KPI_STATS_IND with duration[%d]", 
            time_buffer,
	    p_oam_mac_kpi_stats_ind->DurationTTI/OAM_MSEC_IN_ONE_SEC);

	if(OAM_NULL != p_KPI_stats_ind)
	{
#ifndef OAM_UT_FRAMEWORK
		qvLock(pm_mutex_lock);
#endif
		oam_memset(p_KPI_stats_ind,OAM_ZERO,sizeof(oam_mac_KPI_stats_ind_t));
		p_KPI_stats_ind->DurationTTI = p_oam_mac_kpi_stats_ind->DurationTTI;
		p_KPI_stats_ind->totalPRBUsageDL = p_oam_mac_kpi_stats_ind->totalPRBUsageDL;
		p_KPI_stats_ind->totalPRBUsageUL = p_oam_mac_kpi_stats_ind->totalPRBUsageUL;
		p_KPI_stats_ind->totalSessionTimeUE = p_oam_mac_kpi_stats_ind->totalSessionTimeUE;


		for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI;loop_counter++)
		{
			p_KPI_stats_ind->totalPRBUsagePerQciDL[loop_counter] = p_oam_mac_kpi_stats_ind->totalPRBUsagePerQciDL[loop_counter];
			p_KPI_stats_ind->totalPRBUsagePerQciUL[loop_counter] = p_oam_mac_kpi_stats_ind->totalPRBUsagePerQciUL[loop_counter];
			p_KPI_stats_ind->numberOfActiveUEPerQciDL[loop_counter] = p_oam_mac_kpi_stats_ind->numberOfActiveUEPerQciDL[loop_counter];
			p_KPI_stats_ind->numberOfActiveUEPerQciUL[loop_counter] = p_oam_mac_kpi_stats_ind->numberOfActiveUEPerQciUL[loop_counter];
			p_KPI_stats_ind->totalPktDelayPerQciDL[loop_counter] = p_oam_mac_kpi_stats_ind->totalPktDelayPerQciDL[loop_counter];
			p_KPI_stats_ind->totalPktAckedInDLPerQci[loop_counter] = p_oam_mac_kpi_stats_ind->totalPktAckedInDLPerQci[loop_counter];
			p_KPI_stats_ind->totalSessionTimePerQci[loop_counter]=p_oam_mac_kpi_stats_ind->totalSessionTimePerQci[loop_counter];
			p_KPI_stats_ind->totalDlDrbUuLossRatePerQci[loop_counter] = p_oam_mac_kpi_stats_ind->totalDlDrbUuLossRatePerQci[loop_counter];
			p_KPI_stats_ind->totalSduAckRcvdDlperQci[loop_counter] = p_oam_mac_kpi_stats_ind->totalSduAckRcvdDlperQci[loop_counter];

		}
		p_KPI_stats_ind->rcvdRandomAccessPreamblesContentionFree = p_oam_mac_kpi_stats_ind->rcvdRandomAccessPreamblesContentionFree;
		p_KPI_stats_ind->rcvdRandomAccessPreamblesGroupA = p_oam_mac_kpi_stats_ind->rcvdRandomAccessPreamblesGroupA;
		p_KPI_stats_ind->rcvdRandomAccessPreamblesGroupB = p_oam_mac_kpi_stats_ind->rcvdRandomAccessPreamblesGroupB;
		/*eICIC_PHASE_1_2_CHANGES_START*/
		p_KPI_stats_ind->AbsSubframeCount = p_oam_mac_kpi_stats_ind->AbsSubframeCount;
		/*eICIC_PHASE_1_2_CHANGES_END*/

		for(loop_counter = OAM_ZERO; loop_counter < MAX_CQI_INDEX; ++loop_counter)
		{
			p_KPI_stats_ind->totalCQICW1Reported[loop_counter] =
				p_oam_mac_kpi_stats_ind->totalCQICW1Reported[loop_counter];
			p_KPI_stats_ind->totalCQICW2Reported[loop_counter] =
				p_oam_mac_kpi_stats_ind->totalCQICW2Reported[loop_counter];

			OAM_LOG(OAM,OAM_DETAILEDALL,"Values Stored totalPRBUsageDL = %llu CQI ID =%d CW1 %d CW2 %d ",
					p_oam_mac_kpi_stats_ind->totalPRBUsageDL,
					loop_counter,
					p_KPI_stats_ind->totalCQICW1Reported[loop_counter],
					p_KPI_stats_ind->totalCQICW2Reported[loop_counter]
			       );
		}
        /* SPR 23499 Fix Start */
        for(operator_count = OAM_ZERO; operator_count < MAX_NUM_MOCN_OPERATOR; operator_count++)
        {
            p_KPI_stats_ind->kpiPerOperator[operator_count].operatorId = p_oam_mac_kpi_stats_ind->kpiPerOperator[operator_count].operatorId; 
            for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI;loop_counter++)
            {
                p_KPI_stats_ind->kpiPerOperator[operator_count].totalPRBUsagePerQciDL[loop_counter] = 
                    p_oam_mac_kpi_stats_ind->kpiPerOperator[operator_count].totalPRBUsagePerQciDL[loop_counter];
                p_KPI_stats_ind->kpiPerOperator[operator_count].totalPRBUsagePerQciUL[loop_counter] = 
                    p_oam_mac_kpi_stats_ind->kpiPerOperator[operator_count].totalPRBUsagePerQciUL[loop_counter];
                p_KPI_stats_ind->kpiPerOperator[operator_count].numberOfActiveUEPerQciDL[loop_counter] = 
                    p_oam_mac_kpi_stats_ind->kpiPerOperator[operator_count].numberOfActiveUEPerQciDL[loop_counter];
                p_KPI_stats_ind->kpiPerOperator[operator_count].numberOfActiveUEPerQciUL[loop_counter] = 
                    p_oam_mac_kpi_stats_ind->kpiPerOperator[operator_count].numberOfActiveUEPerQciUL[loop_counter];
            }
        }
        /* SPR 23499 Fix End */

#ifndef OAM_UT_FRAMEWORK
		qvUnlock(pm_mutex_lock);
#endif
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;

}

/****************************************************************************
 ** Function Name  : oam_add_mac_kpi_thp_ind 
 ** Description    : function fill p_KPI_THP_stats_ind structure from 
 **                : p_api_buf buffer  
 ** Inputs         : p_api_buf : buffer for msg storage 
 ** Outputs        : p_KPI_THP_stats_ind :
 ** Returns        : OAM_SUCCESS/OAM_FAILURE 
 *****************************************************************************/
/* SPR 17777 fix */
oam_return_et
	oam_add_mac_kpi_thp_ind
(
 /* SPR 22296 Fix Start */
 oam_mac_thp_batch_node **p_mac_kpi_thp_stats_ind, 
 UInt8 cell_idx
 /* SPR 22296 Fix End */
 )
{

    OAM_FUN_ENTRY_TRACE();
    /* SPR 22296 Fix Start */
    oam_mac_thp_batch_node *p_temp = OAM_NULL;
    oam_mac_thp_batch_node *p_last_saved_node = OAM_NULL;
    oam_mac_thp_batch_node *p_cur_lst_node = OAM_NULL;
    qvLock(pm_mutex_lock);

    p_temp = ConsolidatedMacThpStatsInd;
    while (p_temp != OAM_NULL)
    {
        if(p_temp->cell_idx == cell_idx)
        {
            if(*p_mac_kpi_thp_stats_ind == OAM_NULL)
            {
                *p_mac_kpi_thp_stats_ind = p_temp;
                p_last_saved_node = p_temp;
            }
            else
            { 
                /* CID 118126 Fix Start */
                if(OAM_NULL != p_last_saved_node)
                { 
                    p_last_saved_node->next = p_temp;
                    p_last_saved_node = p_temp;
                }
                /* CID 118126 Fix End */
            }
            if (p_temp == ConsolidatedMacThpStatsInd)
            {
                ConsolidatedMacThpStatsInd = p_temp->next;
            }
            if (OAM_NULL != p_cur_lst_node)
            {
                p_cur_lst_node->next = p_temp->next;
            }
        }
        else
        {
            p_cur_lst_node = p_temp;
        }
        p_temp = p_temp->next;
    }
    if (OAM_NULL != p_last_saved_node)
    {
        p_last_saved_node->next = OAM_NULL;
    }
    /* SPR 22296 Fix End */
    qvUnlock(pm_mutex_lock);
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}


/****************************************************************************
 ** Function Name  : oam_add_in_mac_ind_link_list 
 ** Description    : This function make a node in  pdcp link list and fill data in new
 **                : node 
 ** Inputs         : NONE 
 ** Outputs        : p_kpi_info :pointer to structure oam_kpi_info_t
 ** Returns        : OAM_FAILURE/OAM_SUCCESS
 *****************************************************************************/
oam_return_et
	oam_add_in_mac_ind_link_list
(
 oam_mac_kpi_info_t *p_kpi_info
 )
{
	oam_error_code_et p_error_code;
	UInt16 index = OAM_ZERO;
	OAM_FUN_ENTRY_TRACE();

	OAM_ASSERT(OAM_NULL != p_kpi_info);
	oam_pm_mac_kpi_info_node_t *oam_pm_mac_kpi_info_Node = OAM_NULL;

	if (OAM_FAILURE == oam_mem_alloc(sizeof(oam_pm_mac_kpi_info_node_t),(void *)&oam_pm_mac_kpi_info_Node,&p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to oam_pm_mac_kpi_info_Node failed"
				" with Error Code:%d",
				p_error_code);
		OAM_FUN_EXIT_TRACE();
		/* Not Enough Memory */
		return OAM_FAILURE;
	}

	OAM_LOG(OAM,OAM_DETAILED,"address = %p",oam_pm_mac_kpi_info_Node);
	oam_pm_mac_kpi_info_Node->next_node_ptr = OAM_NULL;
	/* SPR 22296 Fix Start */
	/* Code Removed */
	/* SPR 22296 Fix End */

	g_mac_kpi_ind_handler.num_mac_kpi_stats_ind = OAM_ZERO;
	oam_pm_mac_updat_nxt_node_ptr_of_link_lst ((
				oam_mac_kpi_info_t *)p_kpi_info,
			(oam_pm_mac_kpi_info_node_t *)oam_pm_mac_kpi_info_Node);

	for(index = OAM_ZERO; index < oam_prov_req.num_cells_configured; index++)
	{
		oam_pm_mac_kpi_info_Node->oam_pm_mac_kpi_stats_node[index].cell_index =
			(UInt8)*(UInt8 *)(g_mac_kpi_ind_handler.p_kpi_buf[index] + OAM_INTF_HDR_CELL_IDX_OFFSET);
		/* SPR 22296 Fix Start */
		oam_pm_mac_kpi_info_Node->oam_pm_mac_kpi_stats_node[index].Is_ind_present = OAM_TRUE;
		oam_pm_mac_kpi_stat_ind (
				g_mac_kpi_ind_handler.p_kpi_buf[index],
				&(oam_pm_mac_kpi_info_Node->oam_pm_mac_kpi_stats_node[index].oam_mac_kpi_stats_ind));
		oam_pm_mac_kpi_info_Node->oam_pm_mac_kpi_stats_node[index].p_oam_mac_kpi_thp_stats_ind = OAM_NULL;
	/*SPR 10023 FIX*/
		if (oam_add_mac_kpi_thp_ind(&(oam_pm_mac_kpi_info_Node->oam_pm_mac_kpi_stats_node[index].p_oam_mac_kpi_thp_stats_ind), 
					oam_pm_mac_kpi_info_Node->oam_pm_mac_kpi_stats_node[index].cell_index) == OAM_FAILURE)
	{
		OAM_LOG(OAM,OAM_WARNING, "Error adding MAC_KPI_THP_STATS_IND in MAC Link List, error=%d", p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	}
	ConsolidatedMacThpStatsInd = OAM_NULL;
	/* SPR 22296 Fix End */
	/*SPR 10023 FIX*/
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;

}

/****************************************************************************
 ** Function Name  : oam_mac_kpi_buffer_free 
 ** Description    : This function frees the buffer created to store 
 **                  MAC KPI IND
 ** Inputs         : NONE 
 ** Outputs        : NONE
 ** Returns        : NONE 
 *****************************************************************************/

void
oam_mac_kpi_buffer_free
(
 /* + SPR 17439 */
 void
 )
/* - SPR 17439 */
{
	oam_error_code_et error_code = NO_ERROR;
	oam_counter_t index = OAM_ZERO;
	OAM_FUN_ENTRY_TRACE();

	for(index = OAM_ZERO; index< oam_prov_req.num_cells_configured; index++)
	{
		if(OAM_NULL != g_mac_kpi_ind_handler.p_kpi_buf[index])
			oam_mem_free(g_mac_kpi_ind_handler.p_kpi_buf[index], &error_code);
		g_mac_kpi_ind_handler.p_kpi_buf[index] = OAM_NULL;
	}
	if(OAM_NULL != g_mac_kpi_ind_handler.p_kpi_thp_buf)
		oam_mem_free(g_mac_kpi_ind_handler.p_kpi_thp_buf, &error_code);
	g_mac_kpi_ind_handler.p_kpi_thp_buf = OAM_NULL;

	g_mac_kpi_ind_handler.kpi_thp_flag = OAM_FALSE;
	g_mac_kpi_ind_handler.num_mac_kpi_stats_ind = OAM_ZERO;
	OAM_FUN_EXIT_TRACE();
}

/***********************************************************************************
 ** Function Name -      add_to_mac_thp_batch_link_list
 ** Description -        This function adds the MAC_KPI_THP_STATS_IND data to link list
 ** Inputs -             p_oam_mac_kpi_thp_stats_ind - pointer to PDCP_KPI_THP_STATS_IND
 **                                                              data 
 **                                                              error_code - pointer to oam_error_code_et
 ** Outputs -            NONE
 ** Returns -            NONE
 *************************************************************************************/
void
	add_to_mac_thp_batch_link_list
(
 oam_mac_KPI_THP_stats_ind_t *p_oam_mac_kpi_thp_stats_ind,
 /* SPR 22296 Fix Start */
 oam_error_code_et *p_error_code,
 UInt8 cell_id
 /* SPR 22296 Fix End */
 )
{
	OAM_FUN_ENTRY_TRACE();
	oam_mac_thp_batch_node *temp = OAM_NULL;
	/*SPR 10023 : removed code*/
	oam_mac_thp_batch_node *current = OAM_NULL;

	/*
	 * DESCRIPTION: Here function creates singly link list with node containing of type
	 ** "oam_mac_KPI_THP_stats_ind_t".
	 **/

	if (OAM_FAILURE == oam_mem_alloc(sizeof(oam_mac_thp_batch_node),(void *)&temp,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to temp failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return ;
	}
	oam_memcpy(&temp->data, p_oam_mac_kpi_thp_stats_ind,sizeof(oam_mac_KPI_THP_stats_ind_t));
	temp->next = OAM_NULL;
	/* SPR 22296 Fix Start */
	temp->cell_idx = cell_id;
	/* SPR 22296 Fix End */
	if(ConsolidatedMacThpStatsInd == OAM_NULL)
	{
		ConsolidatedMacThpStatsInd = temp;
	}
	else
	{
		current = ConsolidatedMacThpStatsInd;
		while(current->next != OAM_NULL)
		{
			current = current->next;
		}
		current->next = temp;
	}

}


/***********************************************************************************
 ** Function Name -      oam_handle_mac_thp_batch_data
 ** Description -        This function handles the batches of MAC_KPI_THP_STATS_IND
 **                      and adds the data into link list
 ** Inputs -             p_api_buf  - void type pointer as buffer containing 
 **                      MAC_KPI_THP_STATS_IND data.
 **                      error_code - pointer to oam_error_code_et
 ** Outputs -            NONE
 ** Returns -            OAM_SUCCESS/OAM_FAILURE
 *************************************************************************************/

oam_return_et
	oam_handle_mac_thp_batch_data
(
 void *p_api_buf,
 /* SPR 22296 Fix Start */
 oam_error_code_et *p_error_code,
 UInt8 cell_id
 /* SPR 22296 Fix End */
 )
{
	oam_mac_KPI_THP_stats_ind_t *p_oam_mac_kpi_thp_stats_ind = OAM_NULL;
	oam_return_et ret_val = OAM_FAILURE;
	OAM_FUN_ENTRY_TRACE();

	p_api_buf                    = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_oam_mac_kpi_thp_stats_ind = (oam_mac_KPI_THP_stats_ind_t *)p_api_buf;
	/* SPR 22296 Fix Start */
	add_to_mac_thp_batch_link_list(p_oam_mac_kpi_thp_stats_ind, p_error_code, cell_id);
	/* SPR 22296 Fix End */
	if(p_oam_mac_kpi_thp_stats_ind->endIndicator == OAM_ONE)
	{
		OAM_LOG(OAM, OAM_INFO, "Received end indicator value %u",
				p_oam_mac_kpi_thp_stats_ind->endIndicator);
		ret_val = OAM_SUCCESS;

	}
	else
	{
		OAM_LOG(OAM, OAM_INFO, "Received end indicator value %u",
				p_oam_mac_kpi_thp_stats_ind->endIndicator);
	}
	OAM_FUN_EXIT_TRACE();
	return ret_val;
}

/****************************************************************************
 * Function Name  : oam_mac_handle_kpi_inds
 * Description    : Function handles kpi indications
 * Inputs         : p_api_buf
 * Outputs        : 
 * Returns        : OAM_SUCCESS/OAM_FAILURE 
 ****************************************************************************/
oam_return_et
	oam_mac_handle_kpi_inds
(
 void *p_api_buf
 )
{
	UInt16 api_id = OAM_ZERO;
	UInt16 msg_len = OAM_ZERO;
	oam_error_code_et p_err = NO_ERROR;
	UInt8 cell_index = (UInt8)(*((UInt8*)(p_api_buf) + OAM_INTF_HDR_CELL_IDX_OFFSET));
	msg_len = get_word_from_header((U8*)(p_api_buf) + OAM_INTF_HDR_MSG_SIZE_OFFSET);
	api_id   = get_word_from_header((UInt8*)(p_api_buf) + OAM_EXT_API_ID_FROM_HEADER);
	OAM_FUN_ENTRY_TRACE();
	switch(api_id)
	{   
		case MAC_KPI_STATS_IND:
			{

				OAM_LOG(OAM,OAM_DETAILED, "MAC_KPI_STATS_IND recieved for cell_index = %d", cell_index);
				if ((oam_get_rrm_cell_config_idx(cell_index)
							== OAM_ERROR_RETURN_VAL))
				{
					OAM_LOG(OAM,OAM_INFO,"Received Unconfigured cell_index = %u in MAC_KPI_STATS_IND", cell_index);
					OAM_FUN_EXIT_TRACE();
					return OAM_FAILURE;
				}
				else
				{
					/* SPR 22296 Fix Start */
					/* KPI Stats ind received for all cells from MAC */
					if((g_mac_kpi_ind_handler.num_mac_kpi_stats_ind) >= oam_prov_req.num_cells_configured)
						/* SPR 22296 Fix End */
					{
						OAM_LOG(OAM, OAM_DETAILED, "Not Expecting indication from "
								"MAC_KPI_STATS_IND for cell_index = %u", cell_index);
						OAM_FUN_EXIT_TRACE();
						return OAM_FAILURE;
					}
					if (OAM_FAILURE == oam_mem_alloc(msg_len,
								&(g_mac_kpi_ind_handler.p_kpi_buf[g_mac_kpi_ind_handler.num_mac_kpi_stats_ind]),
								&p_err))
					{
						OAM_LOG(OAM, OAM_ERROR,
								"Memory allocation to g_mac_kpi_ind_handler.p_kpi_buf failed"
								" with Error Code:%d",
								p_err);
						OAM_FUN_EXIT_TRACE();
						/* Not Enough Memory */
						return OAM_FAILURE;
					}
					oam_memcpy(g_mac_kpi_ind_handler.p_kpi_buf[g_mac_kpi_ind_handler.num_mac_kpi_stats_ind],
							p_api_buf,msg_len);
					g_mac_kpi_ind_handler.num_mac_kpi_stats_ind++;
					/*KPI Stats received for all cells and KPI-THP Stats are also received*/

					OAM_LOG(OAM,OAM_DETAILED, "num_mac_kpi_stats_ind = %d."
					    " oam_prov_req.num_cells_configured = %d."
					    " g_mac_kpi_ind_handler.kpi_thp_flag = %d.",
					    g_mac_kpi_ind_handler.num_mac_kpi_stats_ind,
					    oam_prov_req.num_cells_configured,
					    g_mac_kpi_ind_handler.kpi_thp_flag);
					/* SPR 22296 Fix Start */
					if(g_mac_kpi_ind_handler.num_mac_kpi_stats_ind == oam_prov_req.num_cells_configured
							&& oam_prov_req.num_cells_configured == g_mac_kpi_ind_handler.kpi_thp_flag )
					/* SPR 22296 Fix End */
					{
						if (oam_add_in_mac_ind_link_list(&mac_kpi_info) == OAM_FAILURE)
						{
							OAM_LOG(OAM,OAM_WARNING, "Error adding MAC_KPI_STATS_IND stats in MAC Link List");
							oam_mac_kpi_buffer_free();
							OAM_FUN_EXIT_TRACE();
							return OAM_FAILURE;
						}
						else
						{
							oam_mac_kpi_buffer_free();
							OAM_LOG(OAM, OAM_DETAILED, "Successfully added MAC_KPI_STATS_IND stats in linked list");
						}
					}

				}
				break;
			} 
		case MAC_KPI_THP_STATS_IND:
			{

				/* CSR No 00055318 Start */
				OAM_LOG(OAM,OAM_DETAILED, "Received MAC_KPI_THP_STATS_IND for cell_index = %d", cell_index);
                /* SPR 22296 Fix Start */
                /*SPR 21950 Start*/
                if(g_mac_kpi_ind_handler.kpi_thp_flag >= oam_prov_req.num_cells_configured)
                    /* SPR 22296 Fix End */
                    /*SPR 21950 End*/
					{
						OAM_LOG(OAM, OAM_INFO, "Not Expecting indication from "
								"MAC_KPI_THP_STATS_IND for cell_index = %u", cell_index);
						OAM_FUN_EXIT_TRACE();
						return OAM_FAILURE;

					}
					/* CSR No 00055318 end */

                if(oam_handle_mac_thp_batch_data(p_api_buf, &p_err, cell_index) == OAM_SUCCESS)
                {	

                    /*SPR 21950 Start*/
					g_mac_kpi_ind_handler.kpi_thp_flag++;
                    /* SPR 22296 Fix Start */
					if(g_mac_kpi_ind_handler.num_mac_kpi_stats_ind == oam_prov_req.num_cells_configured
                            && oam_prov_req.num_cells_configured == g_mac_kpi_ind_handler.kpi_thp_flag)
                        /* SPR 22296 Fix End */
                        /*SPR 21950 End*/
					{
						if (oam_add_in_mac_ind_link_list(&mac_kpi_info) == OAM_FAILURE)
						{
							OAM_LOG(OAM,OAM_WARNING, "Error adding MAC_KPI_THP_STATS_IND stats in MAC Link List");
							oam_mac_kpi_buffer_free();
							OAM_FUN_EXIT_TRACE();
							return OAM_FAILURE;
						}
						else
						{
							oam_mac_kpi_buffer_free();
							OAM_LOG(OAM, OAM_DETAILED, "Successfully added MAC_KPI_THP_STATS_IND stats in linked list");

						}
					}
				}
				else
				{
					OAM_LOG(OAM, OAM_DETAILED, "Receieved MAC_KPI_THP_STATS_IND with non-zero end indicator");
				}		
			}
			break;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/*spr 19296 fix end*/

/* SPR_11231 _CHANGES_START */

/****************************************************************************
 * Function Name  : oam_pdcp_handle_kpi_inds
 * Description    : Function handles kpi indications
 * Inputs         : p_api_buf
 * Outputs        :
 * Returns        : OAM_SUCCESS
 ****************************************************************************/
	oam_return_et oam_pdcp_handle_kpi_inds
(
 void *p_api_buf
 )
{
	UInt16 api_id = OAM_ZERO;
	UInt16 msg_len = OAM_ZERO;
	oam_error_code_et p_err = NO_ERROR;
	UInt8 cell_index = (UInt8)(*((UInt8*)(p_api_buf) + OAM_INTF_HDR_CELL_IDX_OFFSET));
	msg_len = get_word_from_header((U8*)(p_api_buf) + OAM_INTF_HDR_MSG_SIZE_OFFSET);
	api_id   = get_word_from_header((UInt8*)(p_api_buf) + OAM_EXT_API_ID_FROM_HEADER);
	OAM_FUN_ENTRY_TRACE();
	switch(api_id)
	{   
		case PDCP_KPI_STATS_IND:
			{

				/* CSR No 00055318 Start */
				OAM_LOG(OAM,OAM_DETAILED, "PDCP_KPI_STATS_IND recieved for cell_index = %d", cell_index);
				/* CSR No 00055318 end */
				if ((oam_get_rrm_cell_config_idx(cell_index)
							== OAM_ERROR_RETURN_VAL))
				{
					OAM_LOG(OAM,OAM_INFO,"Received Unconfigured cell_index = %u"
							" in PDCP_KPI_STATS_IND", cell_index);
					OAM_FUN_EXIT_TRACE();
					return OAM_FAILURE;
				}
				else
				{
                    /*SPR 19335/19509  start*/
                    /* SPR 22296 Fix Start */
                    /* KPI Stats ind received for all cells from PDCP */
                    if((g_pdcp_kpi_ind_handler.num_pdcp_kpi_stats_ind) >= oam_prov_req.num_cells_configured)
                        /* SPR 22296 Fix End */
					{
						oam_pdcp_kpi_buffer_free();
						OAM_LOG(OAM, OAM_INFO, "Not Expecting PDCP_KPI_STATS_IND "
							"num_pdcp_kpi_stats_ind(%d),num_cells_configured(%d) Freeing prev memory taken",
							g_pdcp_kpi_ind_handler.num_pdcp_kpi_stats_ind,
							oam_prov_req.num_cells_configured
							);
					/*SPR 19355/19509 end */
					}
					if (OAM_FAILURE == oam_mem_alloc(msg_len,
								&(g_pdcp_kpi_ind_handler.p_kpi_buf[g_pdcp_kpi_ind_handler.num_pdcp_kpi_stats_ind]),
								&p_err))
					{
						OAM_LOG(OAM, OAM_ERROR,
								"Memory allocation to g_pdcp_kpi_ind_handler.p_kpi_buf failed"
								" with Error Code:%d",
								p_err);
						OAM_FUN_EXIT_TRACE();
						/* Not Enough Memory */
						return OAM_FAILURE;
					}
					oam_memcpy(g_pdcp_kpi_ind_handler.p_kpi_buf[g_pdcp_kpi_ind_handler.num_pdcp_kpi_stats_ind],
							p_api_buf,msg_len);
					g_pdcp_kpi_ind_handler.num_pdcp_kpi_stats_ind++;
                    /*SPR 20171 Start */

                    OAM_LOG(OAM, OAM_DETAILED, "num_cells_configured(%d) kpi_thp_flag(%d) num_pdcp_kpi_stats_ind becomes %d",
                            oam_prov_req.num_cells_configured,
                            g_pdcp_kpi_ind_handler.kpi_thp_flag,
                            g_pdcp_kpi_ind_handler.num_pdcp_kpi_stats_ind
                           );
                    /*SPR 20171 End */
                    /*SPR 21950 Start*/
                    /* SPR 22296 Fix Start */
					if(g_pdcp_kpi_ind_handler.num_pdcp_kpi_stats_ind == oam_prov_req.num_cells_configured
                            && oam_prov_req.num_cells_configured == g_pdcp_kpi_ind_handler.kpi_thp_flag)
                        /* SPR 22296 Fix End */
                        /*SPR 21950 End*/
					{
						if (oam_add_in_pdcp_ind_link_list(&pdcp_kpi_info) == OAM_FAILURE)
						{
							OAM_LOG(OAM,OAM_WARNING, "Error adding PDCP_KPI_STATS_IND stats in PDCP Link List");
							oam_pdcp_kpi_buffer_free();
							OAM_FUN_EXIT_TRACE();
							return OAM_FAILURE;
						}
						else
						{
							oam_pdcp_kpi_buffer_free();
							OAM_LOG(OAM, OAM_DETAILED, "Successfully added PDCP_KPI_STATS_IND stats in linked list");
						}
					}

				}
				break;
			} 
		case PDCP_KPI_THP_STATS_IND:
			{

				/* CSR No 00055318 Start */
				OAM_LOG(OAM,OAM_DETAILED, "Received PDCP_KPI_THP_STATS_IND for cell_index = %d", cell_index);
				/* CSR No 00055318 end */
                /*SPR 19335/19509 code deleted */
					/* CSR No 00055318 Start */
                /* SPR 22296 Fix Start */
                if(g_pdcp_kpi_ind_handler.kpi_thp_flag >= oam_prov_req.num_cells_configured)
                    /* SPR 22296 Fix End */
					{
                    /*SPR 20171 Start */
                    oam_pdcp_kpi_buffer_free(); 				
                    OAM_LOG(OAM, OAM_DETAILED, "Not Expecting indication from PDCP_KPI_THP_STATS_IND Freeing memory taken. "
                        "num_cells_configured(%d) kpi_thp_flag(%d) num_pdcp_kpi_stats_ind %d",
                            oam_prov_req.num_cells_configured,
                            g_pdcp_kpi_ind_handler.kpi_thp_flag,
                            g_pdcp_kpi_ind_handler.num_pdcp_kpi_stats_ind
                           );			
                    /*SPR 20171 end */

					}
					/* CSR No 00055318 end */
                /* SPR 22296 Fix Start */
                if(oam_handle_pdcp_thp_batch_data(p_api_buf, &p_err, cell_index) == OAM_SUCCESS)
                    /* SPR 22296 Fix End */
                {
                    /*SPR 21950 Start*/
					g_pdcp_kpi_ind_handler.kpi_thp_flag++;
/*SPR 21950 End*/
                    /* SPR 20171 Start */ 
                    OAM_LOG(OAM, OAM_DETAILED, "num_cells_configured(%d) kpi_thp_flag(%d) num_pdcp_kpi_stats_ind %d",
                            oam_prov_req.num_cells_configured,
                            g_pdcp_kpi_ind_handler.kpi_thp_flag,
                            g_pdcp_kpi_ind_handler.num_pdcp_kpi_stats_ind
                           );
                    /* SPR 20171 End */

                    /* SPR 22296 Fix Start */
					if(g_pdcp_kpi_ind_handler.num_pdcp_kpi_stats_ind == oam_prov_req.num_cells_configured
                            && oam_prov_req.num_cells_configured == g_pdcp_kpi_ind_handler.kpi_thp_flag)
                        /* SPR 22296 Fix End */
					{
						if (oam_add_in_pdcp_ind_link_list(&pdcp_kpi_info) == OAM_FAILURE)
						{
							OAM_LOG(OAM,OAM_WARNING, "Error adding PDCP_KPI_THP_STATS_IND stats in PDCP Link List");
							oam_pdcp_kpi_buffer_free();
							OAM_FUN_EXIT_TRACE();
							return OAM_FAILURE;
						}
						else
						{
							oam_pdcp_kpi_buffer_free();
							OAM_LOG(OAM, OAM_DETAILED, "Successfully added PDCP_KPI_THP_STATS_IND stats in linked list");

						}
					}
				}
				else
				{
					OAM_LOG(OAM, OAM_DETAILED, "Receieved PDCP_KPI_THP_STATS_IND with non-zero end indicator");
				}		
			}
			break;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pdcp_kpi_buffer_free 
 * Description    : This function frees the buffer created to store 
 * 		    PDCP KPI IND
 * Inputs         : NONE 
 * Outputs        : NONE
 * Returns        : NONE 
 ****************************************************************************/
void
oam_pdcp_kpi_buffer_free
(
 /* + SPR 17439 */
 void
 )
/* - SPR 17439 */
{
	oam_error_code_et error_code = NO_ERROR;
	oam_counter_t index = OAM_ZERO;
	OAM_FUN_ENTRY_TRACE();	

	for(index = OAM_ZERO; index< oam_prov_req.num_cells_configured; index++)
	{
		if(OAM_NULL != g_pdcp_kpi_ind_handler.p_kpi_buf[index])    
			oam_mem_free(g_pdcp_kpi_ind_handler.p_kpi_buf[index], &error_code);
		g_pdcp_kpi_ind_handler.p_kpi_buf[index] = OAM_NULL;
	}
	if(OAM_NULL != g_pdcp_kpi_ind_handler.p_kpi_thp_buf)    
		oam_mem_free(g_pdcp_kpi_ind_handler.p_kpi_thp_buf, &error_code);
	g_pdcp_kpi_ind_handler.p_kpi_thp_buf = OAM_NULL;

	g_pdcp_kpi_ind_handler.kpi_thp_flag = OAM_FALSE;
	g_pdcp_kpi_ind_handler.num_pdcp_kpi_stats_ind = OAM_ZERO;
    /*SPR 20171 Start */

    OAM_LOG(OAM, OAM_DETAILED, "num_cells_configured(%d) kpi_thp_flag(%d) num_pdcp_kpi_stats_ind %d",
            oam_prov_req.num_cells_configured,
            g_pdcp_kpi_ind_handler.kpi_thp_flag,
            g_pdcp_kpi_ind_handler.num_pdcp_kpi_stats_ind
           );

    /*SPR 20171 End */

	OAM_FUN_EXIT_TRACE();	
}

/* SPR_11231 _CHANGES_END*/

/****************************************************************************
 * Function Name  : oam_rrm_kpi_ind_handle
 * Description    : 
 * Inputs         : p_api_buf
 * Outputs        :
 * Returns        : OAM_SUCCESS
 ****************************************************************************/
oam_return_et
	oam_rrm_kpi_ind_handle
(
 void *p_api_buf
 )
{
	OAM_FUN_ENTRY_TRACE();
	UInt8 cell_index = (UInt8)(*((UInt8*)(p_api_buf) + OAM_INTF_HDR_CELL_IDX_OFFSET));
	oam_error_code_et p_err = NO_ERROR;
	UInt16 msg_len = OAM_ZERO;
	msg_len = get_word_from_header((U8*)(p_api_buf) + 8);

	OAM_LOG(OAM, OAM_DETAILED, "RRM_OAM_KPI_IND with cell_index = %d/n", cell_index);
	if (OAM_FAILURE == oam_mem_alloc(msg_len,&(g_rrm_kpi_ind_handler.rrm_kpi_buf),&p_err))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to g_rrm_kpi_ind_handler.rrm_kpi_buf failed"
				" with Error Code:%d",
				p_err);
		OAM_FUN_EXIT_TRACE();
		/* Not Enough Memory */
		return OAM_FAILURE;
	}

	oam_memcpy(g_rrm_kpi_ind_handler.rrm_kpi_buf,p_api_buf,msg_len);
	/* SPR 17777 fix */
	oam_add_in_rrm_link_list(&rrm_kpi_info);

	OAM_FUN_EXIT_TRACE(); /* Coverity FIX 36179 */
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_rrc_kpi_stats_inds_handling
 * Description    : 
 * Inputs         : p_api_buf
 * Outputs        :
 * Returns        : OAM_SUCCESS
 ****************************************************************************/
oam_return_et
	oam_rrc_kpi_stats_inds_handling
(
 void *p_api_buf  
 )
{
	UInt16         api_id   = OAM_ZERO;
	oam_error_code_et p_err;
	UInt16 msg_len;
	msg_len = get_word_from_header((U8*)(p_api_buf) + OAM_INTF_HDR_MSG_SIZE_OFFSET);
	api_id   = get_word_from_header((UInt8*)(p_api_buf) + OAM_EXT_API_ID_FROM_HEADER); 
	UInt8 cell_index = (UInt8)(*((UInt8*)(p_api_buf) + OAM_INTF_HDR_CELL_IDX_OFFSET));
	OAM_LOG(OAM, OAM_DETAILED, "Handling rrc kpi indication stats for api_id = %d, cell_index = %d", 
			api_id, cell_index);
	OAM_FUN_ENTRY_TRACE();
	switch(api_id)
	{    
		case RRC_OAM_RRC_CONNECTION_STATS_IND:
			if(g_l3_kpi_ind_handler.ind_bit_mask &0x01)
			{ 
				OAM_LOG(OAM,OAM_DETAILED, "RRC_OAM_RRC_CONNECTION_STATS_IND received with indication");
				if (OAM_FAILURE == oam_mem_alloc(msg_len,&(g_l3_kpi_ind_handler.p_kpi_conn_buf),&p_err))
				{
					OAM_LOG(OAM, OAM_ERROR,
							"Memory allocation to g_l3_kpi_ind_handler.p_kpi_conn_buf failed"
							" with Error Code:%d",
							p_err);
					OAM_FUN_EXIT_TRACE();
					/* Not Enough Memory */
					return OAM_FAILURE;
				}
				oam_memcpy(g_l3_kpi_ind_handler.p_kpi_conn_buf,p_api_buf,msg_len);
				g_l3_kpi_ind_handler.ind_bit_mask = (g_l3_kpi_ind_handler.ind_bit_mask & 0xFE);
				if(OAM_ZERO == g_l3_kpi_ind_handler.ind_bit_mask)
				{

					g_l3_kpi_ind_handler.ind_bit_mask =
						rrc_get_cell_stats_resp_name.bitmask[OAM_ZERO];
					/* SPR 17777 fix */
					oam_add_in_rrc_link_list(&rrc_kpi_info);
				}


			}
			/* CSR No 00055318 Start */
			else
			{
				OAM_LOG(OAM,OAM_DETAILED, "RRC_OAM_RRC_CONNECTION_STATS_IND  Recieved (not expecting indication)");
			}
			/* CSR No 00055318 end */
			break;   
		case RRC_OAM_ERAB_STATS_IND:
			if(g_l3_kpi_ind_handler.ind_bit_mask &0x02)
			{
				OAM_LOG(OAM,OAM_DETAILED, "RRC_OAM_ERAB_STATS_IND received with indication");
				if (OAM_FAILURE == oam_mem_alloc(msg_len,&(g_l3_kpi_ind_handler.p_kpi_erab_buf),&p_err))
				{
					OAM_LOG(OAM, OAM_ERROR,
							"Memory allocation to g_l3_kpi_ind_handler.p_kpi_erab_buf failed"
							" with Error Code:%d",
							p_err);
					OAM_FUN_EXIT_TRACE();
					/* Not Enough Memory */
					return OAM_FAILURE;
				}
				oam_memcpy(g_l3_kpi_ind_handler.p_kpi_erab_buf,p_api_buf,msg_len);
				g_l3_kpi_ind_handler.ind_bit_mask = (g_l3_kpi_ind_handler.ind_bit_mask & 0xFD);
				if(OAM_ZERO == g_l3_kpi_ind_handler.ind_bit_mask)
				{

					g_l3_kpi_ind_handler.ind_bit_mask =
						rrc_get_cell_stats_resp_name.bitmask[OAM_ZERO];
					/* SPR 17777 fix */
					oam_add_in_rrc_link_list(&rrc_kpi_info);
				}
			}
			/* CSR No 00055318 Start */
			else
			{
				OAM_LOG(OAM,OAM_DETAILED, "RRC_OAM_ERAB_STATS_IND Recieved (not expecting indication)");
			}
			/* CSR No 00055318 end */

			break;
		case RRC_OAM_HANDOVER_STATS_IND:
			if(g_l3_kpi_ind_handler.ind_bit_mask&0x04)
			{
				OAM_LOG(OAM,OAM_DETAILED, "RRC_OAM_HANDOVER_STATS_IND received with indication");
				if (OAM_FAILURE == oam_mem_alloc(msg_len,&(g_l3_kpi_ind_handler.p_kpi_handover_buf),&p_err))
				{
					OAM_LOG(OAM, OAM_ERROR,
							"Memory allocation to g_l3_kpi_ind_handler.p_kpi_handover_buf failed"
							" with Error Code:%d",
							p_err);
					OAM_FUN_EXIT_TRACE();
					/* Not Enough Memory */
					return OAM_FAILURE;
				}
				oam_memcpy(g_l3_kpi_ind_handler.p_kpi_handover_buf,p_api_buf,msg_len);
				g_l3_kpi_ind_handler.ind_bit_mask = (g_l3_kpi_ind_handler.ind_bit_mask & 0xFB);
				if(OAM_ZERO == g_l3_kpi_ind_handler.ind_bit_mask)
				{

					g_l3_kpi_ind_handler.ind_bit_mask =
						rrc_get_cell_stats_resp_name.bitmask[OAM_ZERO];
					/* SPR 17777 fix */
					oam_add_in_rrc_link_list(&rrc_kpi_info);
				}
			}
			/* CSR No 00055318 Start */
			else
			{
				OAM_LOG(OAM,OAM_DETAILED, "RRC_OAM_HANDOVER_STATS_IND Recieved (not expecting indication)");
			}
			/* CSR No 00055318 end */
			break;

		case RRC_OAM_PAGING_STATS_IND:
			if(g_l3_kpi_ind_handler.ind_bit_mask&0x08)
			{
				OAM_LOG(OAM,OAM_DETAILED, "RRC_OAM_PAGING_STATS_IND received with indication");
				if (OAM_FAILURE == oam_mem_alloc(msg_len,&(g_l3_kpi_ind_handler.p_kpi_paging_buf),&p_err))
				{
					OAM_LOG(OAM, OAM_ERROR,
							"Memory allocation to g_l3_kpi_ind_handler.p_kpi_paging_buf failed"
							" with Error Code:%d",
							p_err);
					OAM_FUN_EXIT_TRACE();
					/* Not Enough Memory */
					return OAM_FAILURE;
				}
				oam_memcpy(g_l3_kpi_ind_handler.p_kpi_paging_buf,p_api_buf,msg_len);
				g_l3_kpi_ind_handler.ind_bit_mask = (g_l3_kpi_ind_handler.ind_bit_mask & 0xF7);
				if(OAM_ZERO == g_l3_kpi_ind_handler.ind_bit_mask)
				{
					g_l3_kpi_ind_handler.ind_bit_mask =
						rrc_get_cell_stats_resp_name.bitmask[OAM_ZERO];
					/* SPR 17777 fix */
					oam_add_in_rrc_link_list(&rrc_kpi_info);
				}
			}
			/* CSR No 00055318 Start */
			else
			{
				OAM_LOG(OAM,OAM_DETAILED, "RRC_OAM_PAGING_STATS_IND Recieved (not expecting indication)");
			}
			/* CSR No 00055318 end */
			break;

		case RRC_OAM_UE_ASSOC_S1_CONN_STATS_IND:
			if(g_l3_kpi_ind_handler.ind_bit_mask&0x10)
			{
				OAM_LOG(OAM,OAM_DETAILED, "RRC_OAM_UE_ASSOC_S1_CONN_STATS_IND received with indication");
				if (OAM_FAILURE == oam_mem_alloc(msg_len,&(g_l3_kpi_ind_handler.p_kpi_ue_assoc_buf),&p_err))
				{
					OAM_LOG(OAM, OAM_ERROR,
							"Memory allocation to g_l3_kpi_ind_handler.p_kpi_ue_assoc_buf failed"
							" with Error Code:%d",
							p_err);
					OAM_FUN_EXIT_TRACE();
					/* Not Enough Memory */
					return OAM_FAILURE;
				}
				oam_memcpy(g_l3_kpi_ind_handler.p_kpi_ue_assoc_buf,p_api_buf,msg_len);
				g_l3_kpi_ind_handler.ind_bit_mask = (g_l3_kpi_ind_handler.ind_bit_mask& 0xEF);
				if(OAM_ZERO == g_l3_kpi_ind_handler.ind_bit_mask)
				{
					g_l3_kpi_ind_handler.ind_bit_mask =
						rrc_get_cell_stats_resp_name.bitmask[OAM_ZERO];
					/* SPR 17777 fix */
					oam_add_in_rrc_link_list(&rrc_kpi_info);
				}
			}
			/* CSR No 00055318 Start */
			else
			{
				OAM_LOG(OAM,OAM_DETAILED, "RRC_OAM_UE_ASSOC_S1_CONN_STATS_IND Recieved (not expecting indication)");
			}
			/* CSR No 00055318 end */
			break; 

		case RRC_OAM_IRAT_MOBILITY_STATS_IND:
			if(g_l3_kpi_ind_handler.ind_bit_mask&0x20)
			{
				OAM_LOG(OAM,OAM_DETAILED, "RRC_OAM_IRAT_MOBILITY_STATS_IND received with indication");
				if (OAM_FAILURE == oam_mem_alloc(msg_len,&(g_l3_kpi_ind_handler.p_kpi_irat_mobility_buf),&p_err))
				{
					OAM_LOG(OAM, OAM_ERROR,
							"Memory allocation to g_l3_kpi_ind_handler.p_kpi_irat_mobility_buf failed"
							" with Error Code:%d",
							p_err);
					OAM_FUN_EXIT_TRACE();
					/* Not Enough Memory */
					return OAM_FAILURE;
				}
				oam_memcpy(g_l3_kpi_ind_handler.p_kpi_irat_mobility_buf,p_api_buf,msg_len);
				g_l3_kpi_ind_handler.ind_bit_mask = (g_l3_kpi_ind_handler.ind_bit_mask & 0xDF);
				if(OAM_ZERO == g_l3_kpi_ind_handler.ind_bit_mask)
				{
					g_l3_kpi_ind_handler.ind_bit_mask =
						rrc_get_cell_stats_resp_name.bitmask[OAM_ZERO];
					/* SPR 17777 fix */
					oam_add_in_rrc_link_list(&rrc_kpi_info);
				}
			}
			/* CSR No 00055318 Start */
			else
			{
				OAM_LOG(OAM,OAM_DETAILED, "RRC_OAM_IRAT_MOBILITY_STATS_IND Recieved (not expecting indication)");
			}
			/* CSR No 00055318 end */
			break;
	}
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_update_rrc_kpi_inds_handling_strct
 * Description    : Function handles kpi stats update
 * Inputs         : 
 * Outputs        :
 * Returns        : 
 ****************************************************************************/

void
oam_update_rrc_kpi_inds_handling_strct(
		/* + SPR 17439 */
		void
		)
/* - SPR 17439 */
{
	g_l3_kpi_ind_handler.ind_bit_mask = OAM_RRC_KPI_MAP;  
	g_l3_kpi_ind_handler.p_kpi_conn_buf = OAM_NULL;
	g_l3_kpi_ind_handler.p_kpi_erab_buf = OAM_NULL;
	g_l3_kpi_ind_handler.p_kpi_handover_buf = OAM_NULL;
	g_l3_kpi_ind_handler.p_kpi_paging_buf = OAM_NULL;
	g_l3_kpi_ind_handler.p_kpi_ue_assoc_buf = OAM_NULL;
	g_l3_kpi_ind_handler.p_kpi_irat_mobility_buf = OAM_NULL;
}
/****************************************************************************
 * Function Name  : oam_mac_get_kpi_stats_req
 * Description    : This function sends the rlc get kpi stats request to MAC
 * Inputs         : p_mac_get_kpi_stats_req :Pointer to MAC get kpi stats
 *                  request
 *                : src_module_id    :Source module identifier
 *                : transaction_id   :transaction identifier
 * Outputs        : p_error_code     :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
	oam_mac_get_kpi_stats_req
(
 oam_mac_get_KPI_stats_req_t *p_mac_get_kpi_stats_req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length     = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	UInt8 *p_mac_msg;
	p_msg  = OAM_NULL;
	p_mac_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	/* Get API length */
	msg_length  = sizeof(oam_mac_get_KPI_stats_req_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE +  LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* SPR_12955_FIXES */	
	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			MAC_MODULE_ID, MAC_GET_KPI_STATS_REQ, msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
			MAC_MODULE_ID, MAC_GET_KPI_STATS_REQ, msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Incrementing the pointer to point to the structure
	   oam_mac_get_KPI_stats_req_t */
	p_mac_msg = p_mac_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	oam_memcpy((void *)p_mac_msg, p_mac_get_kpi_stats_req,
			sizeof(oam_mac_get_KPI_stats_req_t));

	/* OAM send message to MAC */
	if(OAM_FAILURE == oam_send_message(p_msg, MAC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending MAC_GET_KPI_STATS_REQ message to MAC with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}



/****************************************************************************
 * Function Name  : oam_cli_str_cpy
 * Description    : This function copy src string in des string
 * Inputs         : 
 * Outputs        : des : output string
 * Returns        : 
 ****************************************************************************/
void 
	oam_cli_str_cpy
(
 char *des,
 char *src
 )
{
	int i = 0;
	for(i =0;i<100;i++)
		des[i] = 0x20;

	i = 0;
	while(src[i] != '\0')
	{
		des[i] = src[i];
		i++;
	}
    /* SPR 24124/SES-507 Fix Start */
	des[i] = '\0';
    /* SPR 24124/SES-507 Fix End */

}

/*spr 19296 fix start*/

/****************************************************************************
 * Function Name  : oam_mac_get_kpi_direct_thp_stats_cnf
 * Description    : This function sends the pdcp get kpi stats cnf to OAM
 * Inputs         :
 * Outputs        : p_error_code     :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
	oam_mac_get_kpi_direct_thp_stats_cnf
(
 UInt16 res_trans_id 
 )
{
    /*SPR 23209 FIX START*/
    oam_counter_t loop_counter2 = OAM_ZERO;
    /*SPR 23209 FIX END*/
    /* SPR 24124/SES-507 Fix Start */
    FILE *p_mac_kpi = OAM_NULL;
    /* SPR 24124/SES-507 Fix End */
    UInt8 temp_str_cli[200];
    UInt8 temp_arr_cli_2[200];
    UInt8 **temp_arr= OAM_NULL;
    /* SPR 24124/SES-507 Fix +- */
    UInt8 str_count = OAM_ZERO;
    oam_counter_t loop_counter = OAM_ZERO;
    oam_counter_t loop_counter1 = OAM_ZERO;
    UInt32 totallatSample[RRM_MAX_QCI] = {OAM_ZERO};
    UInt32 totalThpTimeUl[RRM_MAX_QCI] = {OAM_ZERO};
    UInt32 totalThpTimeDl[RRM_MAX_QCI] = {OAM_ZERO};
    UInt16 trans_id = OAM_ZERO,transaction_id = OAM_ZERO;
    UInt16 dest_id = OAM_ZERO;
    UInt32 totalNumberOfSamplingOccasions = OAM_ZERO; 
    oam_mac_KPI_stats_ind_t  *p_oam_mac_get_kpi_stats_conf;
    oam_mac_KPI_THP_stats_ind_t *p_oam_mac_get_kpi_thp_stats_cnf;
    p_oam_mac_get_kpi_thp_stats_cnf = OAM_NULL;
    p_oam_mac_get_kpi_stats_conf = OAM_NULL;
    oam_error_code_et p_error_code; //= OAM_NULL;
    oam_mac_thp_batch_node *temp = ConsolidatedGetMacThpStats;
    UInt8 PRB_UL             = OAM_ZERO;
    UInt8 PRB_DL             = OAM_ZERO;
    UInt32 durationTTI     = OAM_ZERO;
    float dlfactor = OAM_ZERO;
    float ulfactor = OAM_ZERO;

    UInt64 pm_dlIPThp[RRM_MAX_QCI] = {OAM_ZERO};	
    UInt64 pm_ulIPThp[RRM_MAX_QCI] = {OAM_ZERO};	
    UInt32 pm_dlIPLatency[RRM_MAX_QCI] = {OAM_ZERO};	
    UInt8 size_x; /* this variable will be used for the first  dimension */
    UInt8 size_y;
    SInt8 cell_index = oam_get_rrm_cell_config_idx(
            oam_prov_req.cell_cntxt.curr_cell_cid);
    /*coverity 109076 fix start*/	
    if(OAM_ERROR_RETURN_VAL == cell_index)
    {
        OAM_LOG(OAM, OAM_INFO, "Invalid cell_index[%d] in oam_get_rrm_cell_config_idx",cell_index);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    /*coverity 109076 fix end*/	
    /*SPR 19807 fix start*/
    oam_rrm_cell_config_req_t  *p_rrm_cell_config = &oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req[cell_index];
    /*SPR 19807 fix end*/

    OAM_FUN_ENTRY_TRACE();
    OAM_LOG(OAM, OAM_INFO, "Trans_id : %d", transaction_id);	
    transaction_id = res_trans_id; 

    dlfactor = (oam_prov_req.prb_usage_factor.num_dl_schedulable / 10.0);
    ulfactor = (oam_prov_req.prb_usage_factor.num_ul_schedulable / 10.0);

    PRB_UL =  p_rrm_cell_config->operator_info.
        admission_control_info.ul_prb_budget;
    PRB_DL =  p_rrm_cell_config->operator_info.
        admission_control_info.dl_prb_budget;



#ifndef OAM_UT_FRAMEWORK
    if(OAM_FAILURE == oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(transaction_id,&trans_id,&dest_id))
    {
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
#endif

    /* SPR 24124/SES-507 Fix Start */
    size_x = OAM_TWO;
    size_y = OAM_HUNDRED;
    /* Code Removed */
    /* SPR 24124/SES-507 Fix End */
    /*  allocate storage for an array of pointer to char */
    if (OAM_FAILURE == oam_mem_alloc((size_x * sizeof(UInt8 *)),(void *)&temp_arr,&p_error_code))
    {
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to temp_arr failed"
                " with Error Code:%d",
                p_error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;

    }
    /* SPR 24124/SES-507 Fix Start */
    p_mac_kpi = oam_fopen(OAM_MAC_KPI_LOG_FILE_NAME,"w+");

    if(OAM_NULL == p_mac_kpi)
    {
        OAM_LOG(OAM,OAM_ERROR,"Unable to create file %s for MAC KPI logging",OAM_MAC_KPI_LOG_FILE_NAME);
        oam_mem_free((void *)temp_arr,&p_error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    /* SPR 24124/SES-507 Fix End */

    /* for each pocharer, allocate storage for an array of chars */
    for (loop_counter = OAM_ZERO; loop_counter < size_x; loop_counter++)
    {
        if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)&temp_arr[loop_counter],&p_error_code))
        {
            OAM_LOG(OAM, OAM_ERROR,
                    "Memory allocation to temp_arr failed"
                    " with Error Code:%d",
                    p_error_code);
            /*SPR 13857 START*/
            OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, loop_counter, p_error_code);
            /*SPR 13857 END*/
            OAM_FUN_EXIT_TRACE();
            /* coverity_237519_fix_start */
            oam_fclose(p_mac_kpi);
            /* coverity_237519_fix_end */
            return OAM_FAILURE;
        }
        /*SPR 20256 FIXED START*/
        oam_memset(temp_arr[loop_counter], OAM_ZERO, (size_y * sizeof(UInt8)));
        /*SPR 20256 FIXED END*/

    }


    OAM_LOG(OAM,OAM_DETAILED,"loop_counter = %d",loop_counter);	/* coverity 109088 +-*/
    oam_mac_direct_stats_hndlr.p_kpi_stats_buf = oam_mac_direct_stats_hndlr.p_kpi_stats_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    p_oam_mac_get_kpi_stats_conf = (oam_mac_KPI_stats_ind_t*)((oam_mac_direct_stats_hndlr.p_kpi_stats_buf));


    durationTTI = p_oam_mac_get_kpi_stats_conf->DurationTTI;
    if( (PRB_DL * durationTTI * dlfactor) != OAM_ZERO)
    {
        if(OAM_ZERO != p_oam_mac_get_kpi_stats_conf->totalPRBUsageDL)
        {
            /* CSR_00058645_START */
            oam_cli_str_cpy((char *)temp_str_cli,"TotalPRBUsageDL");
            /* SPR 24124/SES-507 Fix Start */
            oam_fprintf(p_mac_kpi, "%s=%u\n", temp_str_cli,(UInt8)(((p_oam_mac_get_kpi_stats_conf->totalPRBUsageDL) * OAM_PERCENTAGE_CONVERSION_VAL)/
                        (PRB_DL * dlfactor * durationTTI)));
            /* SPR 24124/SES-507 Fix End */

        }
        else
        {
            oam_cli_str_cpy((char *)temp_str_cli,"TotalPRBUsageDL");
            /* SPR 24124/SES-507 Fix Start */
            oam_fprintf(p_mac_kpi,"%s=%d\n", temp_str_cli, OAM_ZERO);
            /* SPR 24124/SES-507 Fix End */
        }
    }
    else
    {
        /*SPR 20171 Start */

        OAM_LOG(OAM, OAM_INFO, " DIVIDE BY ZERO cell_index = %d durationTTI=%d dl_prb_budget(PRB_DL) %d ,dlfactor %f ,oam_prov_req.prb_usage_factor.num_dl_schedulable = %d", 
                cell_index,
                durationTTI,
                PRB_DL,dlfactor,
                oam_prov_req.prb_usage_factor.num_dl_schedulable
               );
        /*SPR 20171 End */

        oam_cli_str_cpy((char *)temp_str_cli,"TotalPRBUsageDL");
        /* SPR 24124/SES-507 Fix Start */
        oam_fprintf(p_mac_kpi,"%s=%d\n",temp_str_cli, OAM_ZERO);
        /* SPR 24124/SES-507 Fix End */
    }

    if( (PRB_UL * durationTTI * ulfactor) != OAM_ZERO)
    {

        if(OAM_ZERO != p_oam_mac_get_kpi_stats_conf-> totalPRBUsageUL  )
        {
            oam_cli_str_cpy((char *)temp_str_cli,"TotalPRBUsageUL");
            /* SPR 24124/SES-507 Fix Start */
            oam_fprintf(p_mac_kpi,"%s=%u\n", temp_str_cli,(UInt8)(((p_oam_mac_get_kpi_stats_conf-> totalPRBUsageUL) * OAM_PERCENTAGE_CONVERSION_VAL)/
                        (PRB_UL * ulfactor * durationTTI)));
            /* SPR 24124/SES-507 Fix End */
        }
        else
        {
            oam_cli_str_cpy((char *)temp_str_cli,"TotalPRBUsageUL");
            /* SPR 24124/SES-507 Fix Start */
            oam_fprintf(p_mac_kpi,"%s=%d\n", temp_str_cli,OAM_ZERO);
            /* SPR 24124/SES-507 Fix End */
        }
    }
    else
    {
        /*SPR 20171 Start */

        OAM_LOG(OAM, OAM_INFO, " DIVIDE BY ZERO cell_index = %d durationTTI=%d ul_prb_budget(PRB_UL) %d ,ulfactor %f oam_prov_req.prb_usage_factor.num_ul_schedulable %d", 
                cell_index,
                durationTTI,
                PRB_UL,ulfactor,
                oam_prov_req.prb_usage_factor.num_ul_schedulable
               );
        /*SPR 20171 End */

        oam_cli_str_cpy((char *)temp_str_cli,"TotalPRBUsageUL");
        /* SPR 24124/SES-507 Fix Start */
        oam_fprintf(p_mac_kpi,"%s=%d\n",temp_str_cli,OAM_ZERO);
        /* SPR 24124/SES-507 Fix End */
    }
    /* SPR 13492 Changes Start */
    oam_cli_str_cpy((char *)temp_str_cli,"InSessionActiveTimeUE");
    /*SPR 19511 */
    /* SPR 24124/SES-507 Fix Start */
    oam_fprintf(p_mac_kpi,"%s=%llu\n",temp_str_cli,
            (UInt64)(p_oam_mac_get_kpi_stats_conf->totalSessionTimeUE));
    /* SPR 24124/SES-507 Fix End */

    /* SPR 13492 Changes End */

    for(loop_counter1 = OAM_ZERO; loop_counter1<(RRM_MAX_QCI);loop_counter1++)
    {

        if(OAM_ZERO != (p_oam_mac_get_kpi_stats_conf->totalDlDrbUuLossRatePerQci[loop_counter1] + p_oam_mac_get_kpi_stats_conf->totalSduAckRcvdDlperQci[loop_counter1]))
        {
            oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                    "TotalDlDrbUuLossRatePerQci",loop_counter1 + 1);
            oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
            /* SPR 24124/SES-507 Fix Start */
            oam_fprintf(p_mac_kpi,"%s=%u\n", temp_str_cli,(((UInt64)((UInt64)(p_oam_mac_get_kpi_stats_conf->
                                    totalDlDrbUuLossRatePerQci[loop_counter1]) * PM_PDCP_KPI_MULTIPLIER )/
                            (UInt64)((UInt64)(p_oam_mac_get_kpi_stats_conf->totalSduAckRcvdDlperQci[loop_counter1]) + (UInt64)(p_oam_mac_get_kpi_stats_conf->totalDlDrbUuLossRatePerQci[loop_counter1])))));  
            /* SPR 24124/SES-507 Fix End */
        }
        else
        {
            oam_sprintf((char *)temp_arr_cli_2,"%s[%u]",
                    "TotalDlDrbUuLossRatePerQci",loop_counter1 + 1);
            oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
            /* SPR 24124/SES-507 Fix Start */
            oam_fprintf(p_mac_kpi,"%s=%d\n",temp_str_cli,OAM_ZERO);
            /* SPR 24124/SES-507 Fix End */

        }
        if( durationTTI != OAM_ZERO)
        {
            if((PRB_DL * dlfactor) != OAM_ZERO)
            {
                oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                        "TotalPRBUsagePerQciDL",loop_counter1 + OAM_ONE);
                oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
                /* SPR-11133 FIX Start */
                /* SPR 24124/SES-507 Fix Start */
                oam_fprintf(p_mac_kpi,"%s=%u\n",temp_str_cli,(UInt8)((((p_oam_mac_get_kpi_stats_conf->
                                        totalPRBUsagePerQciDL[loop_counter1])* OAM_PERCENTAGE_CONVERSION_VAL))/
                            (PRB_DL * dlfactor *durationTTI)));
                /* SPR 24124/SES-507 Fix End */
            }
            else
            {
                OAM_LOG(OAM,OAM_INFO,"DIVIDE BY ZERO ERROR ");
                oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                        "TotalPRBUsagePerQciDL",loop_counter1 + OAM_ONE);
                oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
                /* SPR 24124/SES-507 Fix Start */
                oam_fprintf(p_mac_kpi,"%s=%d\n", temp_str_cli,OAM_ZERO);
                /* SPR 24124/SES-507 Fix End */
            }
            if((PRB_UL * ulfactor) != OAM_ZERO)
            {
                oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                        "TotalPRBUsagePerQciUL",loop_counter1 + OAM_ONE);
                oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);

                /* SPR 24124/SES-507 Fix Start */
                oam_fprintf(p_mac_kpi,"%s=%u\n",temp_str_cli,(UInt8)((((p_oam_mac_get_kpi_stats_conf->
                                        totalPRBUsagePerQciUL[loop_counter1])* OAM_PERCENTAGE_CONVERSION_VAL))/
                            (PRB_UL * ulfactor * durationTTI)));
                /* SPR 24124/SES-507 Fix End */
                /* SPR-11133 FIX End */
            }
            else
            {
                OAM_LOG(OAM,OAM_INFO,"DIVIDE BY ZERO ERROR ");
                oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                        "TotalPRBUsagePerQciUL",loop_counter1 + OAM_ONE);
                oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
                /* SPR 24124/SES-507 Fix Start */
                oam_fprintf(p_mac_kpi,"%s=%d\n",temp_str_cli,OAM_ZERO);
                /* SPR 24124/SES-507 Fix End */
            }
            /* SPR-15429 START */
            totalNumberOfSamplingOccasions =
                durationTTI/ACTIVE_USER_SAMPLING_TIMER_TTI;
            /* SPR 24124/SES-507 Fix Start */
            /* Code Removed */
            /* SPR 24124/SES-507 Fix End */


            /* gur30485 : SPR 6792 Fix Start */

            /* SPR 20995 changes start */
            if(totalNumberOfSamplingOccasions != OAM_ZERO) 
            {    
                /* SPR 24124/SES-507 Fix Start */
                oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                        "NumberOfActiveUEPerQciDL",loop_counter1 + OAM_ONE);
                oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
                /* SPR 20995 changes end */
                oam_fprintf(p_mac_kpi,"%s=%llu\n",temp_str_cli,((p_oam_mac_get_kpi_stats_conf->
                                numberOfActiveUEPerQciDL[loop_counter1])/totalNumberOfSamplingOccasions));
                /* SPR 24124/SES-507 Fix End */

                oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                        "NumberOfActiveUEPerQciUL",loop_counter1 + OAM_ONE);
                oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
                /* gur30485 : SPR 6792 Fix Start */
                /* SPR 24124/SES-507 Fix Start */
                oam_fprintf(p_mac_kpi,"%s=%llu\n",temp_str_cli,((p_oam_mac_get_kpi_stats_conf->
                                numberOfActiveUEPerQciUL[loop_counter1])/totalNumberOfSamplingOccasions));
                /* SPR 24124/SES-507 Fix End */

                /* SPR 20995 changes start */
            }
            /* SPR 20995 changes end */
            /* SPR-15429 END */
        }
        else
        {
            OAM_LOG(OAM,OAM_INFO,"DIVIDE BY ZERO ERROR ");
            oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                    "TotalPRBUsagePerQciDL",loop_counter1 + OAM_ONE);
            oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
            /* SPR 24124/SES-507 Fix Start */
            oam_fprintf(p_mac_kpi,"%s=%d\n", temp_str_cli,OAM_ZERO);
            /* SPR 24124/SES-507 Fix End */

            oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                    "TotalPRBUsagePerQciUL",loop_counter1 + OAM_ONE);
            oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
            /* SPR 24124/SES-507 Fix Start */
            oam_fprintf(p_mac_kpi,"%s=%d\n", temp_str_cli,OAM_ZERO);
            /* SPR 24124/SES-507 Fix End */

            oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                    "NumberOfActiveUEPerQciDL",loop_counter1 + OAM_ONE);
            oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
            /* SPR 24124/SES-507 Fix Start */
            oam_fprintf(p_mac_kpi,"%s=%d\n", temp_str_cli,OAM_ZERO);

            oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                    "NumberOfActiveUEPerQciUL",loop_counter1 + OAM_ONE);
            oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
            oam_fprintf(p_mac_kpi,"%s=%d\n", temp_str_cli,OAM_ZERO);
            /* SPR 24124/SES-507 Fix End */

        }
        if(OAM_ZERO != p_oam_mac_get_kpi_stats_conf->totalPktAckedInDLPerQci[loop_counter1])
        {
            oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                    "TotalPktDelayPerQciDL",loop_counter1 + OAM_ONE);
            oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);


            /* SPR 24124/SES-507 Fix Start */
            oam_fprintf(p_mac_kpi,"%s=%llu\n",temp_str_cli,((p_oam_mac_get_kpi_stats_conf->
                            totalPktDelayPerQciDL[loop_counter1])/
                        (p_oam_mac_get_kpi_stats_conf->
                         totalPktAckedInDLPerQci[loop_counter1])));
            /* SPR 24124/SES-507 Fix End */
            /*SPR 19511 start */
            OAM_LOG(OAM, OAM_DETAILED, " cell_index = %d durationTTI=%d RachCF = %llu RachA = %llu RachB = %llu "
                    "TIME_UNIT_TO_MILLISEC = %d PRB_DL %d ,dlfactor %f ,PRB_UL %d ,ulfactor %f "
                    "totalPktDelayPerQciDL[loop_counter1]%llu "
                    "totalPktAckedInDLPerQci[loop_counter1=%d] = %llu totalSessionTimePerQci[loop_counter1]=%llu", 
                    cell_index,
                    durationTTI,
                    p_oam_mac_get_kpi_stats_conf->rcvdRandomAccessPreamblesContentionFree,
                    p_oam_mac_get_kpi_stats_conf->rcvdRandomAccessPreamblesGroupA,
                    p_oam_mac_get_kpi_stats_conf->rcvdRandomAccessPreamblesGroupB,
                    TIME_UNIT_TO_MILLISEC,
                    PRB_DL,dlfactor,
                    PRB_UL,ulfactor,
                    p_oam_mac_get_kpi_stats_conf->totalPktDelayPerQciDL[loop_counter1],
                    loop_counter1,
                    p_oam_mac_get_kpi_stats_conf->totalPktAckedInDLPerQci[loop_counter1],
                    p_oam_mac_get_kpi_stats_conf->totalSessionTimePerQci[loop_counter1]                     
                   );
            /*SPR 19511 end*/

        }
        else
        {
            /* SPR 17951 */
            /*SPR 19511 start */
            OAM_LOG(OAM, OAM_DETAILED, " No packet Acked cell_index = %d durationTTI=%d RachCF = %llu "
                    "RachA = %llu RachB = %llu TIME_UNIT_TO_MILLISEC = %d PRB_DL %d ,dlfactor %f ,PRB_UL %d ,"
                    " ulfactor %f  totalPktDelayPerQciDL[loop_counter1]%llu "
                    "totalPktAckedInDLPerQci[loop_counter1=%d] = %llu totalSessionTimePerQci[loop_counter1]=%llu", 
                    cell_index,
                    durationTTI,
                    p_oam_mac_get_kpi_stats_conf->rcvdRandomAccessPreamblesContentionFree,
                    p_oam_mac_get_kpi_stats_conf->rcvdRandomAccessPreamblesGroupA,
                    p_oam_mac_get_kpi_stats_conf->rcvdRandomAccessPreamblesGroupB,
                    TIME_UNIT_TO_MILLISEC,
                    PRB_DL,dlfactor,
                    PRB_UL,ulfactor,
                    p_oam_mac_get_kpi_stats_conf->totalPktDelayPerQciDL[loop_counter1],
                    loop_counter1,
                    p_oam_mac_get_kpi_stats_conf->totalPktAckedInDLPerQci[loop_counter1],
                    p_oam_mac_get_kpi_stats_conf->totalSessionTimePerQci[loop_counter1]
                   );
            /*SPR 19511 end */

            oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                    "TotalPktDelayPerQciDL",loop_counter1 + OAM_ONE);
            oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
            /*SPR 20171 Start */
            /* SPR 24124/SES-507 Fix Start */
            oam_fprintf(p_mac_kpi,"%s=%llu\n",temp_str_cli,p_oam_mac_get_kpi_stats_conf->totalPktDelayPerQciDL[loop_counter1]);
            /* SPR 24124/SES-507 Fix End */
            /*SPR 20171 End */                    
        }
        /* SPR 13492 Changes Start */
        oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                "InSessionActiveTimePerQci",loop_counter1 + OAM_ONE);
        oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
        /*SPR 20171 Start */
        /* SPR 24124/SES-507 Fix Start */
        oam_fprintf(p_mac_kpi,"%s=%llu\n",temp_str_cli,(UInt64)(p_oam_mac_get_kpi_stats_conf->
                    totalSessionTimePerQci[loop_counter1]));
        /* SPR 24124/SES-507 Fix End */
        /*SPR 20171 End*/
        /* SPR 13492 Changes End */
    }

    /* SPR 23209 FIXED START */
    for(loop_counter2 = OAM_ZERO; loop_counter2<MAX_NUM_MOCN_OPERATOR ;loop_counter2++)
    {
        oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                "OperatorID",loop_counter2 + OAM_ONE);
        oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
        /* SPR-11133 FIX Start */
        /* SPR 24124/SES-507 Fix Start */
        oam_fprintf(p_mac_kpi,"%s=%u\n",temp_str_cli,(UInt8)
                p_oam_mac_get_kpi_stats_conf->kpiPerOperator[loop_counter2].operatorId);
        /* SPR 24124/SES-507 Fix End */
        for(loop_counter1 = OAM_ZERO; loop_counter1< (RRM_MAX_QCI);loop_counter1++)
        {
            if( durationTTI != OAM_ZERO)
            {
                oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                        "TotalPRBUsagePerQciDL",loop_counter1 + OAM_ONE);
                oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
                /* SPR-11133 FIX Start */
                /* SPR 24124/SES-507 Fix Start */
                oam_fprintf(p_mac_kpi,"%s=%u\n",temp_str_cli,(UInt8)((((p_oam_mac_get_kpi_stats_conf->
                                        kpiPerOperator[loop_counter2].totalPRBUsagePerQciDL[loop_counter1])* OAM_PERCENTAGE_CONVERSION_VAL))/
                            (PRB_DL * dlfactor *durationTTI)));
                /* SPR 24124/SES-507 Fix End */

                oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                        "TotalPRBUsagePerQciUL",loop_counter1 + OAM_ONE);
                oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);

                /* SPR 24124/SES-507 Fix Start */
                oam_fprintf(p_mac_kpi,"%s=%u\n",temp_str_cli,(UInt8)((((p_oam_mac_get_kpi_stats_conf->
                                        kpiPerOperator[loop_counter2].totalPRBUsagePerQciUL[loop_counter1])* OAM_PERCENTAGE_CONVERSION_VAL))/
                            (PRB_UL * ulfactor * durationTTI)));
                /* SPR 24124/SES-507 Fix End */
                /* SPR-11133 FIX End */

                /* SPR-15429 START */        

                totalNumberOfSamplingOccasions = durationTTI/ACTIVE_USER_SAMPLING_TIMER_TTI;
                /* SPR 24124/SES-507 Fix Start */
                if(OAM_ZERO != totalNumberOfSamplingOccasions)
                {
                    oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                            "NumberOfActiveUEPerQciDL",loop_counter1 + OAM_ONE);
                    oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
                    /* gur30485 : SPR 6792 Fix Start */
                    /*MOCN SPR 21713 Fix Start*/
                    /* SPR 24124/SES-507 Fix Start */
                    oam_fprintf(p_mac_kpi,"%s=%llu\n",temp_str_cli,((p_oam_mac_get_kpi_stats_conf->
                                    kpiPerOperator[loop_counter2].numberOfActiveUEPerQciDL[loop_counter1])/totalNumberOfSamplingOccasions));
                    /* SPR 24124/SES-507 Fix End */
                    /*MOCN SPR 21713 Fix End*/

                    oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                            "NumberOfActiveUEPerQciUL",loop_counter1 + OAM_ONE);
                    oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
                    /* gur30485 : SPR 6792 Fix Start */
                    /*MOCN SPR 21713 Fix Start*/
                    /* SPR 24124/SES-507 Fix Start */
                    oam_fprintf(p_mac_kpi,"%s=%llu\n",temp_str_cli,((p_oam_mac_get_kpi_stats_conf->
                                    kpiPerOperator[loop_counter2].numberOfActiveUEPerQciUL[loop_counter1])/totalNumberOfSamplingOccasions));
                    /* SPR 24124/SES-507 Fix End */
                    /*MOCN SPR 21713 Fix End*/
                }
                /* SPR 24124/SES-507 Fix End */
                /* SPR-15429 END */        
            }
            else
            {
                oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                        "TotalPRBUsagePerQciDL",loop_counter1 + OAM_ONE);
                oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
                /* SPR 24124/SES-507 Fix Start */
                oam_fprintf(p_mac_kpi,"%s=%d\n", temp_str_cli,OAM_ZERO);
                /* SPR 24124/SES-507 Fix End */

                oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                        "TotalPRBUsagePerQciUL",loop_counter1 + OAM_ONE);
                oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
                /* SPR 24124/SES-507 Fix Start */
                oam_fprintf(p_mac_kpi,"%s=%d\n", temp_str_cli,OAM_ZERO);
                /* SPR 24124/SES-507 Fix End */

                oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                        "NumberOfActiveUEPerQciDL",loop_counter1 + OAM_ONE);
                oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
                /* SPR 24124/SES-507 Fix Start */
                oam_fprintf(p_mac_kpi,"%s=%d\n", temp_str_cli,OAM_ZERO);
                /* SPR 24124/SES-507 Fix End */

                /* SPR 24124/SES-507 Fix Start */
                oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                        "NumberOfActiveUEPerQciUL",loop_counter1 + OAM_ONE);
                oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
                oam_fprintf(p_mac_kpi,"%s=%d\n", temp_str_cli,OAM_ZERO);
                /* SPR 24124/SES-507 Fix End */

            }
        }
    }
    /*SPR 23209 FIXED END  */


    if(durationTTI != OAM_ZERO )
    {
        /* SPR 24124/SES-507 Fix Start */
        oam_fprintf(p_mac_kpi,"%s=%llu\n","RcvdRandomAccessPreamblesContentionFree",
                ((p_oam_mac_get_kpi_stats_conf->
                  rcvdRandomAccessPreamblesContentionFree * TIME_UNIT_TO_MILLISEC)/
                 durationTTI));

        oam_fprintf(p_mac_kpi,"%s=%llu\n","RcvdRandomAccessPreamblesGroupA",
                ((p_oam_mac_get_kpi_stats_conf->
                  rcvdRandomAccessPreamblesGroupA * TIME_UNIT_TO_MILLISEC)/
                 durationTTI));

        oam_fprintf(p_mac_kpi,"%s=%llu\n","RcvdRandomAccessPreamblesGroupB",
                ((p_oam_mac_get_kpi_stats_conf->
                  rcvdRandomAccessPreamblesGroupB * TIME_UNIT_TO_MILLISEC)/
                 durationTTI));
        /* SPR 24124/SES-507 Fix End */
    }
    else
    {
        OAM_LOG(OAM,OAM_INFO,"DIVIDE BY ZERO ERROR ");
        /* SPR 24124/SES-507 Fix Start */
        oam_fprintf(p_mac_kpi,"%s=%d\n",
                "RcvdRandomAccessPreamblesContentionFree",OAM_ZERO);
        oam_fprintf(p_mac_kpi,"%s=%d\n",
                "RcvdRandomAccessPreamblesGroupA",OAM_ZERO);
        oam_fprintf(p_mac_kpi,"%s=%d\n",
                "RcvdRandomAccessPreamblesGroupB",OAM_ZERO);
        /* SPR 24124/SES-507 Fix End */
    }
    /* CSR_00058645_END */
    /* SPR 12670 Changes Start */
    for(loop_counter1 = OAM_ZERO; loop_counter1 < MAX_CQI_INDEX; ++loop_counter1)
    {
        oam_sprintf((char *)temp_arr_cli_2,"%s[%d]",
                "TotalWideBandCQIReported",loop_counter1);
        oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
        /* SPR 24124/SES-507 Fix Start */
        oam_fprintf(p_mac_kpi,"%s=%u\n",temp_str_cli,(UInt32)((p_oam_mac_get_kpi_stats_conf->
                        totalCQICW1Reported[loop_counter1]) +
                    (p_oam_mac_get_kpi_stats_conf->
                     totalCQICW2Reported[loop_counter1])));
        /* SPR 24124/SES-507 Fix End */
    }
    /* SPR 12670 Changes End */


    /* 
     * DESCRIPTION: While loop below reads all the nodes from 
     * "ConsolidatedGetMacThpStats" link list.
     */
    while(temp != OAM_NULL)
    {
        p_oam_mac_get_kpi_thp_stats_cnf = &temp->data;
        /*SPR FIX 10023*/
        for(loop_counter = OAM_ZERO; loop_counter<MAX_KPI_INDEX; loop_counter++)
        {
            if(0xFF != p_oam_mac_get_kpi_thp_stats_cnf->qci[loop_counter] && p_oam_mac_get_kpi_thp_stats_cnf->qci[loop_counter] <= RRM_MAX_QCI)
            {
                /* SPR_12419_START */
                OAM_LOG(OAM, OAM_DETAILED, "Received QCI[%d] entry valid from MAC ", p_oam_mac_get_kpi_thp_stats_cnf->qci[loop_counter]);
                /* SPR 20961 Fix Start */
                if((INVALID_THP_STATS_ENTRY != p_oam_mac_get_kpi_thp_stats_cnf->thpVolDl[loop_counter]) &&
                        (0xFFFFFFFFFFFFFFFEULL != p_oam_mac_get_kpi_thp_stats_cnf->thpVolDl[loop_counter]))
                {
                    pm_dlIPThp[p_oam_mac_get_kpi_thp_stats_cnf->qci[loop_counter] -1] += 
                        p_oam_mac_get_kpi_thp_stats_cnf->thpVolDl[loop_counter];
                    totalThpTimeDl[p_oam_mac_get_kpi_thp_stats_cnf->qci[loop_counter] -1] += p_oam_mac_get_kpi_thp_stats_cnf->thpTimeDl[loop_counter];
                }
                if((INVALID_THP_STATS_ENTRY != p_oam_mac_get_kpi_thp_stats_cnf->thpVolUl[loop_counter]) &&
                        (0xFFFFFFFFFFFFFFFEULL != p_oam_mac_get_kpi_thp_stats_cnf->thpVolUl[loop_counter]))
                    /* SPR 20961 Fix End */
                {
                    pm_ulIPThp[p_oam_mac_get_kpi_thp_stats_cnf->qci[loop_counter] -1] += 
                        p_oam_mac_get_kpi_thp_stats_cnf->thpVolUl[loop_counter];
                    totalThpTimeUl[p_oam_mac_get_kpi_thp_stats_cnf->qci[loop_counter] -1] += p_oam_mac_get_kpi_thp_stats_cnf->thpTimeUl[loop_counter];
                }
                if(0xFFFFFFFF != p_oam_mac_get_kpi_thp_stats_cnf->latTime[loop_counter])
                {
                    pm_dlIPLatency[p_oam_mac_get_kpi_thp_stats_cnf->qci[loop_counter] - 1] += 
                        p_oam_mac_get_kpi_thp_stats_cnf->latTime[loop_counter];
                    totallatSample[p_oam_mac_get_kpi_thp_stats_cnf->qci[loop_counter] -1] += p_oam_mac_get_kpi_thp_stats_cnf->latSample[loop_counter];
                }

            }
            else
            {
                OAM_LOG(OAM, OAM_INFO, "Invalid Entry for QCI[%u] for counter[%u] in MAC_GET_KPI_THP_STATS_CNF",
                        p_oam_mac_get_kpi_thp_stats_cnf->qci[loop_counter], loop_counter);
                break;	
            }
        }
        temp = temp->next;
        /*SPR FIX 10023*/
    }
    /*SPR FIX 10023*/

    for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI ;loop_counter++)
    {
        /* SPR_12419_START */
        if(totalThpTimeDl[loop_counter] != OAM_ZERO)
        {
            oam_sprintf((char *)temp_arr_cli_2,"DlIpThpQCI[%d]",
                    loop_counter + 1);
            oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
            /* SPR 24124/SES-507 Fix Start */
            oam_fprintf(p_mac_kpi,"%s=%llu\n",temp_str_cli,((pm_dlIPThp[loop_counter] * OAM_EIGHT) / totalThpTimeDl[loop_counter]));
            /* SPR 24124/SES-507 Fix End */
            /* SPR_12419_END */
        }
        else
        {
            oam_sprintf((char *)temp_arr_cli_2,"DlIpThpQCI[%d]",
                    loop_counter + 1);
            oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
            /* SPR 24124/SES-507 Fix Start */
            oam_fprintf(p_mac_kpi,"%s=%u\n",temp_str_cli,OAM_ZERO);
            /* SPR 24124/SES-507 Fix End */
        }
    }
    for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI ;loop_counter++)
    {
        /* SPR_12419_START */
        if(totalThpTimeUl[loop_counter] != OAM_ZERO)
            /* SPR_12419_END */
        {
            oam_sprintf((char *)temp_arr_cli_2,"UlIpThpQCI[%d]",
                    loop_counter + 1);
            oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
            /* SPR 24124/SES-507 Fix Start */
            oam_fprintf(p_mac_kpi,"%s=%llu\n",temp_str_cli,((pm_ulIPThp[loop_counter ] * OAM_EIGHT) / totalThpTimeUl[loop_counter]));
            /* SPR 24124/SES-507 Fix End */
        }
        else
        {
            oam_sprintf((char *)temp_arr_cli_2,"UlIpThpQCI[%d]",
                    loop_counter + 1);
            oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
            /* SPR 24124/SES-507 Fix Start */
            oam_fprintf(p_mac_kpi,"%s=%u\n",temp_str_cli,OAM_ZERO);
            /* SPR 24124/SES-507 Fix End */
        }
    }

    if(p_oam_mac_get_kpi_thp_stats_cnf != OAM_NULL)
    {
        for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI ;loop_counter++)
        {

            if (OAM_ZERO == totallatSample[loop_counter])
            {
                oam_sprintf((char *)temp_arr_cli_2,
                        "DlIPLatencyQCI[%d]",
                        loop_counter + 1);
                oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
                /* SPR 24124/SES-507 Fix Start */
                oam_fprintf(p_mac_kpi,"%s=%d\n",temp_str_cli, OAM_ZERO);
                /* SPR 24124/SES-507 Fix End */
            }
            else
            {
                oam_sprintf((char *)temp_arr_cli_2,"DlIPLatencyQCI[%d]",
                        loop_counter + 1);
                oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
                /* SPR 24124/SES-507 Fix Start */
                oam_fprintf(p_mac_kpi,"%s=%u\n",temp_str_cli,pm_dlIPLatency[loop_counter ]/ totallatSample[loop_counter]);
                /* SPR 24124/SES-507 Fix End */
            }
        }
    }

    /* 
     * DESCRIPTION: This function will delete all the nodes of 
     * "ConsolidatedGetMacThpStats" link list
     */

    delete_mac_batch_data_from_link_list((SInt32)MAC_GET_KPI_THP_STATS_CNF);
    /*SPR FIX 10023*/
    /* SPR 24124/SES-507 Fix Start */
    oam_sprintf((char *)temp_arr[str_count++],"%s %s %s","MAC KPI being too large has been dumped into file", 
            OAM_MAC_KPI_LOG_FILE_NAME, "at location LTE_IPR/bin.");
    oam_construct_n_send_gpv_response((UInt8 *)"MAC KPI",temp_arr,str_count,
            trans_id,&p_error_code,dest_id,OAM_ZERO,OAM_ZERO);
    /* SPR 24124/SES-507 Fix End */

    /*SPR 19511  start*/
    if(durationTTI != OAM_ZERO )
        OAM_LOG(OAM, OAM_DETAILED, " cell_index = %d durationTTI=%d RachCF = %llu RachA = %llu RachB = %llu "
                "TIME_UNIT_TO_MILLISEC = %d  PRB_DL %d ,dlfactor %f ,PRB_UL %d ,ulfactor %f  totalPRBUsageDL %llu "
                "totalPRBUsageUL %llu  totalSessionTimeUE %llu Total char Ptrs used %d and memory taken for %d Ptrs", 
                cell_index,
                durationTTI,
                p_oam_mac_get_kpi_stats_conf->rcvdRandomAccessPreamblesContentionFree,
                p_oam_mac_get_kpi_stats_conf->rcvdRandomAccessPreamblesGroupA,
                p_oam_mac_get_kpi_stats_conf->rcvdRandomAccessPreamblesGroupB,
                TIME_UNIT_TO_MILLISEC,
                PRB_DL,dlfactor,
                PRB_UL,ulfactor,
                p_oam_mac_get_kpi_stats_conf->totalPRBUsageDL,
                p_oam_mac_get_kpi_stats_conf->totalPRBUsageUL,
                p_oam_mac_get_kpi_stats_conf->totalSessionTimeUE,
                str_count,size_x

               );
    /*SPR 19511 end */

    /* SPR 24124/SES-507 Fix Start */
    oam_fclose(p_mac_kpi);
    /* SPR 24124/SES-507 Fix End */
    /*SPR 13857 START*/
    OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, size_x, p_error_code);
    /*SPR 13857 END*/


    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;

}
/*spr 19296 fix end*/
/*spr 19296 fix start*/
/*code deleted*/
/*spr 19296 fix end*/

/****************************************************************************
 * Function Name  : oam_pdcp_get_kpi_stats_req
 * Description    : This function sends the pdcp get kpi stats request to PDCP
 * Inputs         : p_pdcp_get_kpi_stats_req :Pointer to PDCP get kpi stats
 *                  request
 *                : src_module_id   :Source module identifier
 *                : transaction_id  :transaction identifier
 * Outputs        : p_error_code    :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_pdcp_get_kpi_stats_req
(
 oam_pdcp_get_KPI_stats_req_t *p_pdcp_get_kpi_stats_req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length     = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	UInt8 *p_pdcp_msg;
	p_msg        = OAM_NULL;
	p_pdcp_msg   = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	/* Get API length */
	msg_length  = sizeof(oam_pdcp_get_KPI_stats_req_t);
	msg_api_length =msg_length + OAM_CSPL_HDR_SIZE +  LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_pdcp_msg = p_msg;

	/* SPR_12955_FIXES */	
	/* Fill CSPL header */
	oam_construct_api_header(p_pdcp_msg, OAM_VERSION_ID, src_module_id,
			PDCP_MODULE_ID, PDCP_GET_KPI_STATS_REQ, msg_api_length);

	/* Fill interface header */
	p_pdcp_msg = p_pdcp_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
			PDCP_MODULE_ID, PDCP_GET_KPI_STATS_REQ, msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Incrementing the pointer to point to the structure
	   oam_mac_init_req_params_t */
	p_pdcp_msg = p_pdcp_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	oam_memcpy((void *)p_pdcp_msg, p_pdcp_get_kpi_stats_req,
			sizeof(oam_pdcp_get_KPI_stats_req_t));

	/* OAM send message to PDCP */
	if(OAM_FAILURE == oam_send_message(p_msg, PDCP_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending PDCP_GET_KPI_STATS_REQ message to PDCP with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}


	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pdcp_get_kpi_direct_thp_stats_cnf
 * Description    : This function sends the pdcp get kpi stats cnf to OAM
 * Inputs         :
 * Outputs        : p_error_code     :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
/*SPR 10023 FIX*/
/* SPR_12955_FIXES */	
oam_return_et
oam_pdcp_get_kpi_direct_thp_stats_cnf
(
 UInt16 res_trans_id 
 )
/*SPR 10023 FIX*/
/* SPR_12955_FIXES */	
{
	/*CSR 81339 end*/
/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/
	/*SPR_10610_Changes +*/
	UInt8 temp_str_cli[200];
	UInt8 temp_arr_cli_2[200];
	UInt8 **temp_arr= OAM_NULL;
	UInt8 count = OAM_ZERO;
	UInt8 str_count = OAM_ZERO;
	UInt32 avgDuration[RRM_MAX_QCI] = {OAM_ZERO};
	oam_counter_t loop_counter = OAM_ZERO;
	UInt16 trans_id = OAM_ZERO,transaction_id = OAM_ZERO;
	UInt16 dest_id = OAM_ZERO;
	oam_pdcp_KPI_stats_ind_t  *p_oam_pdcp_get_kpi_stats_conf;
	oam_pdcp_KPI_THP_stats_ind_t *p_oam_pdcp_get_kpi_thp_stats_cnf;
	p_oam_pdcp_get_kpi_thp_stats_cnf = OAM_NULL;
	p_oam_pdcp_get_kpi_stats_conf = OAM_NULL;
	oam_error_code_et p_error_code; //= OAM_NULL;
	oam_pdcp_thp_batch_node *temp = ConsolidatedGetPdcpThpStats;

	/* SPR_12419_START */
/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/
	UInt64 pm_pdcpsdubitrateDl[RRM_MAX_QCI] = {OAM_ZERO};	
	UInt64 pm_pdcpsdubitrateUl[RRM_MAX_QCI] = {OAM_ZERO};	
	UInt8 size_x; /* this variable will be used for the first  dimension */
	UInt8 size_y;

	OAM_FUN_ENTRY_TRACE();
	/* SPR_12955_FIXES */	
	OAM_LOG(OAM, OAM_INFO, "Trans_id : %d", transaction_id);	
	transaction_id = res_trans_id; 
	/* SPR_12955_FIXES */	

#ifndef OAM_UT_FRAMEWORK
	if(OAM_FAILURE == oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(transaction_id,&trans_id,&dest_id))
	{
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
#endif

	size_x = OAM_ZERO;
	size_y = 200;

	count = OAM_ZERO;

	for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI;loop_counter++)
		count  = count + 3;


	for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI ;loop_counter++)
		count  = count + 2;

	for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI ;loop_counter++)
		count  = count + 2;

	for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI ;loop_counter++)
		count  = count + 2;

	size_x = count;
	/*  allocate storage for an array of pointer to char */
	if (OAM_FAILURE == oam_mem_alloc((size_x * sizeof(UInt8 *)),(void *)&temp_arr,&p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to temp_arr failed"
				" with Error Code:%d",
				p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;

	}

	/* for each pocharer, allocate storage for an array of chars */
	for (loop_counter = OAM_ZERO; loop_counter < size_x; loop_counter++)
	{
		if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)&temp_arr[loop_counter],&p_error_code))
		{
			OAM_LOG(OAM, OAM_ERROR,
					"Memory allocation to temp_arr failed"
					" with Error Code:%d",
					p_error_code);
			/*SPR 13857 START*/
			OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, loop_counter, p_error_code);
			/*SPR 13857 END*/
			OAM_FUN_EXIT_TRACE();
			return OAM_FAILURE;
		}
	}


	oam_pdcp_direct_stats_hndlr.p_kpi_stats_buf = oam_pdcp_direct_stats_hndlr.p_kpi_stats_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/*SPR 10023 FIX: removed code*/
	p_oam_pdcp_get_kpi_stats_conf = (oam_pdcp_KPI_stats_ind_t*)((oam_pdcp_direct_stats_hndlr.p_kpi_stats_buf));

	for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI;loop_counter++)
	{
		if(OAM_ZERO != p_oam_pdcp_get_kpi_stats_conf->totalSduRcvdDlperQci[loop_counter])
		{
			if(OAM_ZERO != p_oam_pdcp_get_kpi_stats_conf->totalDlDrbDiscardRatePerQci[loop_counter])
			{
				/* SPR_12419_START */
				oam_sprintf((char *)temp_arr_cli_2,"%s[%u]",
						"TotalDlDrbDiscardRatePerQci",loop_counter + 1);
				/* SPR_12419_END */
				oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);

				OAM_LOG(OAM, OAM_DETAILED, "totalDlDrbDiscardRatePerQci[%u]: %u. "
						"totalSduRcvdDlperQci[%u]: %u",
						loop_counter,p_oam_pdcp_get_kpi_stats_conf->totalDlDrbDiscardRatePerQci[loop_counter],
						loop_counter,p_oam_pdcp_get_kpi_stats_conf->totalSduRcvdDlperQci[loop_counter]);
				/* SPR 14004 Changes Start */
				oam_sprintf((char *)temp_arr[str_count++],"%s=%u",
						temp_str_cli,(((UInt64)((UInt64)p_oam_pdcp_get_kpi_stats_conf->
									totalDlDrbDiscardRatePerQci[loop_counter] * PM_PDCP_KPI_MULTIPLIER)/
								(UInt64)(p_oam_pdcp_get_kpi_stats_conf->
									totalSduRcvdDlperQci[loop_counter])))); 
				/* SPR 14004 Changes End */
			}
			else
			{
				/* SPR_12419_START */
				oam_sprintf((char *)temp_arr_cli_2,"%s[%u]",
						"TotalDlDrbDiscardRatePerQci",loop_counter + 1);
				/* SPR_12419_END */
				oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
				oam_sprintf((char *)temp_arr[str_count++],"%s=%u",
						temp_str_cli,0);

			}
			/*SPR 11749 start*/
/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/
		}
		else
		{
			/* CSR_00058645_START */
			oam_sprintf((char *)temp_arr_cli_2,"%s[%u]",
					"TotalDlDrbDiscardRatePerQci",loop_counter + 1);
			/* CSR_00058645_END */
			oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
			oam_sprintf((char *)temp_arr[str_count++],"%s=%u",
					temp_str_cli,0);
/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/

		}

		if(OAM_ZERO != p_oam_pdcp_get_kpi_stats_conf->totalSduRcvdUlperQci[loop_counter])
		{
			/* SPR_12419_START */
			/* CSR_00058645_START */
			oam_sprintf((char *)temp_arr_cli_2,"%s[%u]",
					"TotalUlLossRatePerQci",loop_counter + 1);
			/* CSR_00058645_END */
			oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);

			OAM_LOG(OAM, OAM_DETAILED, "totalUlLossRatePerQci[%d]: %u. ", 
			        "totalSduRcvdDlperQci[%d]: %u", 
					loop_counter,p_oam_pdcp_get_kpi_stats_conf->totalUlLossRatePerQci[loop_counter],
					loop_counter,p_oam_pdcp_get_kpi_stats_conf->totalSduRcvdDlperQci[loop_counter]);
			/* SPR 14004 Changes Start */
			oam_sprintf((char *)temp_arr[str_count++],"%s=%u",
					temp_str_cli,(((UInt64)((UInt64)p_oam_pdcp_get_kpi_stats_conf->
								totalUlLossRatePerQci[loop_counter] * PM_PDCP_KPI_MULTIPLIER)/
							(UInt64)(p_oam_pdcp_get_kpi_stats_conf->
								totalSduRcvdUlperQci[loop_counter])))); 
			/* SPR_12419_END */
			/* SPR 14004 Changes End */
		}
		else
		{
			/* CSR_00058645_START */
			oam_sprintf((char *)temp_arr_cli_2,"%s[%u]",
					"TotalUlLossRatePerQci",loop_counter + 1);
			/* CSR_00058645_END */
			oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
			oam_sprintf((char *)temp_arr[str_count++],"%s=%u",
					temp_str_cli,0);

		} 
	}

	if(p_oam_pdcp_get_kpi_stats_conf->durationTTI)
	{
		OAM_LOG(OAM, OAM_DETAILED, "dlCtrlPdcpSduBitRate: %u", 
				p_oam_pdcp_get_kpi_stats_conf->dlCtrlPdcpSduBitRate);
		oam_sprintf((char *)temp_arr_cli_2,"%s",
				"DlCtrlPdcpSduBitRate");
		oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);

		oam_sprintf((char *)temp_arr[str_count++],"%s=%u",
				temp_str_cli,(p_oam_pdcp_get_kpi_stats_conf->
					dlCtrlPdcpSduBitRate * OAM_EIGHT)/p_oam_pdcp_get_kpi_stats_conf->durationTTI);
	}	 
	else
	{
		OAM_LOG(OAM, OAM_DETAILED, "dlCtrlPdcpSduBitRate: %u",
				p_oam_pdcp_get_kpi_stats_conf->dlCtrlPdcpSduBitRate);
		oam_sprintf((char *)temp_arr_cli_2,"%s",
				"DlCtrlPdcpSduBitRate");
		oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);

		oam_sprintf((char *)temp_arr[str_count++],"%s=%d",
				temp_str_cli,OAM_ZERO);

	}		 
	if(p_oam_pdcp_get_kpi_stats_conf->durationTTI)
	{
		OAM_LOG(OAM, OAM_DETAILED, "ulCtrlPdcpSduBitRate: %u", 
				p_oam_pdcp_get_kpi_stats_conf->ulCtrlPdcpSduBitRate);
		oam_sprintf((char *)temp_arr_cli_2,"%s",
				"UlCtrlPdcpSduBitRate");
		oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);

		oam_sprintf((char *)temp_arr[str_count++],"%s=%u",
				temp_str_cli,(p_oam_pdcp_get_kpi_stats_conf->
					ulCtrlPdcpSduBitRate * OAM_EIGHT)/p_oam_pdcp_get_kpi_stats_conf->durationTTI);
	}	 
	else
	{
		OAM_LOG(OAM, OAM_DETAILED, "ulCtrlPdcpSduBitRate: %u",
				p_oam_pdcp_get_kpi_stats_conf->ulCtrlPdcpSduBitRate);
		oam_sprintf((char *)temp_arr_cli_2,"%s",
				"UlCtrlPdcpSduBitRate");
		oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);

		oam_sprintf((char *)temp_arr[str_count++],"%s=%d",
				temp_str_cli,OAM_ZERO);

	}		 
	/*SPR FIX 10023*/

	/* 
	 * DESCRIPTION: While loop below reads all the nodes from 
	 * "ConsolidatedGetPdcpThpStats" link list.
	 */

	while(temp != OAM_NULL)
	{
		p_oam_pdcp_get_kpi_thp_stats_cnf = &temp->data;
		/*SPR FIX 10023*/
		for(loop_counter = OAM_ZERO; loop_counter<MAX_KPI_INDEX; loop_counter++)
		{
			if(0xFF != p_oam_pdcp_get_kpi_thp_stats_cnf->qci[loop_counter] && p_oam_pdcp_get_kpi_thp_stats_cnf->qci[loop_counter] <= RRM_MAX_QCI)
			{
				/* SPR_12419_START */
				OAM_LOG(OAM, OAM_DETAILED, "Received QCI[%d] entry valid from PDCP ", p_oam_pdcp_get_kpi_thp_stats_cnf->qci[loop_counter]);
/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/
                /* SPR 20961 Fix Start */
                if(INVALID_THP_STATS_ENTRY != p_oam_pdcp_get_kpi_thp_stats_cnf->pdcpSduBitDl[loop_counter])
				{
					pm_pdcpsdubitrateDl[p_oam_pdcp_get_kpi_thp_stats_cnf->qci[loop_counter] -1] += 
						p_oam_pdcp_get_kpi_thp_stats_cnf->pdcpSduBitDl[loop_counter];
				}
          
				if(INVALID_THP_STATS_ENTRY != p_oam_pdcp_get_kpi_thp_stats_cnf->pdcpSduBitUl[loop_counter])
                /* SPR 20961 Fix End */
				{
					pm_pdcpsdubitrateUl[p_oam_pdcp_get_kpi_thp_stats_cnf->qci[loop_counter] -1] += 
						p_oam_pdcp_get_kpi_thp_stats_cnf->pdcpSduBitUl[loop_counter];
				}
				/* SPR_12419_END */
				avgDuration[p_oam_pdcp_get_kpi_thp_stats_cnf->qci[loop_counter] -1] = p_oam_pdcp_get_kpi_thp_stats_cnf->avgDurationT;

			}
			else
			{
				OAM_LOG(OAM, OAM_INFO, "Invalid Entry for QCI[%u] for counter[%u] in PDCP_GET_KPI_THP_STATS_RESP",
						p_oam_pdcp_get_kpi_thp_stats_cnf->qci[loop_counter], loop_counter);
				break;	
			}
		}
		temp = temp->next;
		/*SPR FIX 10023*/
	}
	/*SPR FIX 10023*/

/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/
	for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI ;loop_counter++)
	{

		oam_sprintf((char *)temp_arr_cli_2,"SduBitrateDlQCI[%d]",
				loop_counter + 1);
		oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
		if (avgDuration[loop_counter] != OAM_ZERO)
		{
			oam_sprintf((char *)temp_arr[str_count++],"%s=%llu",
					temp_str_cli,((pm_pdcpsdubitrateDl[loop_counter] * OAM_EIGHT)/ avgDuration[loop_counter]));
		}
		else
		{
			oam_sprintf((char *)temp_arr[str_count++],"%s=%u",temp_str_cli, OAM_ZERO);
		}
	}

	for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI ;loop_counter++)
	{
		oam_sprintf((char *)temp_arr_cli_2,"SduBitrateUlQCI[%d]",
				loop_counter + 1);
		oam_cli_str_cpy((char *)temp_str_cli,(char *)temp_arr_cli_2);
		if (avgDuration[loop_counter] != OAM_ZERO)
		{
			oam_sprintf((char *)temp_arr[str_count++],"%s=%llu",
					temp_str_cli,((pm_pdcpsdubitrateUl[loop_counter] * OAM_EIGHT) / avgDuration[loop_counter]));
		}
		else
		{
			oam_sprintf((char *)temp_arr[str_count++],"%s=%u", temp_str_cli, OAM_ZERO);
		}
	}

	/*SPR FIX 10023*/

	/* 
	 * DESCRIPTION: This function will delete all the nodes of 
	 * "ConsolidatedGetPdcpThpStats" link list
	 */

	delete_pdcp_batch_data_from_link_list((SInt32)PDCP_GET_KPI_THP_STATS_RESP);
	/*SPR FIX 10023*/

	oam_construct_n_send_gpv_response((UInt8 *)"PDCP",temp_arr,str_count,
			trans_id,&p_error_code,dest_id,OAM_ZERO,OAM_ZERO);

	/*SPR 13857 START*/
	OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, size_x, p_error_code);
	/*SPR 13857 END*/


	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;

}




UInt32 cal_len_non_periodic_cell_stats_resp(
/* + SPR 17439 */
 void
)
/* - SPR 17439 */
{
	oam_counter_t index = OAM_ZERO;
	oam_counter_t index_2 = OAM_ZERO;

	UInt16 length = OAM_ZERO; 

	length += 2; 

	length += 1;  
	for(index = OAM_ZERO; index < RRC_ESTABLISHMENT_CAUSE_INDEX; index++)
	{
		length = length + 3;
	}
	length +=5;

	for(index = OAM_ZERO; index < RRC_REESTABLISHMENT_CAUSE_INDEX; index++)
	{
		length = length + 3;
	}
	length +=3;

	for(index = OAM_ZERO; index < RRC_ESTABLISHMENT_CAUSE_INDEX; index++) 
		length +=3;

	length +=2;

	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; index++)
	{
		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_TRANSPORT_INDEX; index++)
	{

		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
	{

		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_PROTOCOL_INDEX; index++)
	{
		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
	{
		++length;
	}


	for(index = OAM_ZERO; index < RRC_MAX_QCI_INDEX; index++)
	{
		++length;
	}
	++length;
	for(index = OAM_ZERO; index < RRC_MAX_QCI_INDEX; index++)
	{
		++length;
	}
	++length;  
	for(index = OAM_ZERO;
			index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; index++)
	{
		++length;
	}
	for(index = OAM_ZERO;
			index < RRC_S1AP_CAUSE_TRANSPORT_INDEX; index++)
	{
		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
	{
		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_PROTOCOL_INDEX ; index++)
	{
		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
	{
		++length;
	}

	++length; 
	for(index = OAM_ZERO; index < RRC_MAX_QCI_INDEX; index++)
	{
		++length; 
	}

	++length; 

	for(index = OAM_ZERO; index < RRC_MAX_QCI_INDEX; index++)
	{
		++length; 
	}

	++length; 
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; index++)
	{
		++length;            
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_TRANSPORT_INDEX; index++)
	{
		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
	{
		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_PROTOCOL_INDEX; index++)
	{
		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
	{
		++length;
	}
	++length;

	for(index = OAM_ZERO; index < RRC_MAX_QCI_INDEX; index++)
	{
		++length;
		++length;
		++length;
	}


	for(index = OAM_ZERO; index < RRC_MAX_QCI_INDEX; index++)
	{
		++length;
		++length;
		++length;
		++length;
	}
	length = length + 5;

	for(index = OAM_ZERO;
			index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX;
			index++)
	{
		++length;
	}
	for(index = OAM_ZERO;
			index < RRC_S1AP_CAUSE_TRANSPORT_INDEX;
			index++)
	{
		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
	{
		++length;
	}
	for(index = OAM_ZERO;
			index < RRC_S1AP_CAUSE_PROTOCOL_INDEX ;
			index++)
	{
		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
	{
		++length;
	}



	for(index = OAM_ZERO;
			index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX;
			index++)
	{
		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_TRANSPORT_INDEX;
			index++)
	{
		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
	{
		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_PROTOCOL_INDEX;
			index++)
	{
		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
	{
		++length;
	}

	for(index = OAM_ZERO; index < RRC_MAX_QCI_INDEX; index++)
	{
		++length;
	}
	++length;
	for(index = OAM_ZERO; index < RRC_MAX_QCI_INDEX; index++)
	{
		++length;
	}
	++length;

	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX;
			index++)
	{
		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_TRANSPORT_INDEX;
			index++)
	{
		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
	{
		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_PROTOCOL_INDEX; index++)
	{
		++length;
	}
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
	{
		++length;

	}
	++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
	{
		++length;
	}
	++length;

	++length;
	length = length + 2;


	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_TRANSPORT_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_PROTOCOL_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_TRANSPORT_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_PROTOCOL_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
		++length;

	length = length + 3;

	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_TRANSPORT_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_PROTOCOL_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
		++length;

	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_TRANSPORT_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_PROTOCOL_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
		++length;
	for(index_2 = OAM_ZERO; index_2 < OAM_FIVE; index_2++)
	{ 
		length = length + 2;
		for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; index++)
			++length;
		for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_TRANSPORT_INDEX; index++)
			++length;
		for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
			++length;
		for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_PROTOCOL_INDEX; index++)
			++length;
		for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
			++length;


		for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; index++)
			++length;
		for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_TRANSPORT_INDEX; index++)
			++length;
		for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
			++length;
		for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_PROTOCOL_INDEX; index++)
			++length;
		for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
			++length;


	}   


	length = length + 2;

	length = length + 4;

	length = length + 4;

	length = length + 2;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_TRANSPORT_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_PROTOCOL_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_TRANSPORT_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_NAS_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_PROTOCOL_INDEX; index++)
		++length;
	for(index = OAM_ZERO; index < RRC_S1AP_CAUSE_MISC_INDEX; index++)
		++length;
	length = length + 2; 
	length = length + 2; 

	return length;         
}
/****************************************************************************
 * Function Name  : oam_element_in_pm_xml_param_name
 * Description    : This function gets the KPI pack elements
 * Inputs         : param_name - pack name
 * Outputs        :
 * Returns        : NONE
 ******************************************************************************/
int oam_element_in_pm_xml_param_name(char *param_name[])
{
	OAM_FUN_ENTRY_TRACE();
	/* Coverity FIX 35664 */
	char **value_in_param_name = param_name;
	UInt16  index = 0;
	UInt16 element_index = 0;	

	/* count the KPIs for which the PM xml info needs to be fetched */
	while((*value_in_param_name != OAM_NULL) && (index < PM_XML_MAX_PARAM_VALUE))
	{
		OAM_LOG(OAM,OAM_DETAILED,"Values read in structure: name = %s , index = %d", *value_in_param_name,element_index);
		value_in_param_name++;
		element_index++;
		index = element_index;
	}

	OAM_FUN_EXIT_TRACE(); /* Coverity FIX 36176 */
	return element_index;
}

/****************************************************************************
 * Function Name  : oam_pm_xml_init_info
 * Description    : This function initializes the pm pack info
 * Inputs         : param_name - pack name
 * Outputs        :
 * Returns        : NONE
 ******************************************************************************/
void oam_pm_xml_init_info(
/* + SPR 17439 */
 void
)
/* - SPR 17439 */
{

	OAM_FUN_ENTRY_TRACE();

	char mkdir_cmd[OAM_FIFTY_FIVE] = {OAM_ZERO};

	elements_in_pm_xml_conn_st_param_name = oam_element_in_pm_xml_param_name(pm_xml_conn_st_param_name);

	elements_in_pm_xml_erab_param_name = oam_element_in_pm_xml_param_name(pm_xml_erab_param_name);

	elements_in_pm_xml_paging_param_name = oam_element_in_pm_xml_param_name(pm_xml_paging_param_name);

	elements_in_pm_xml_s1sig_param_name = oam_element_in_pm_xml_param_name(pm_xml_s1sig_param_name);

	elements_in_pm_xml_ho_param_name = oam_element_in_pm_xml_param_name(pm_xml_ho_param_name);
    
/* SPR 18861 */
    g_pm_xml_mac_param_start = elements_in_pm_xml_conn_st_param_name + elements_in_pm_xml_erab_param_name + 
        elements_in_pm_xml_paging_param_name + elements_in_pm_xml_s1sig_param_name + elements_in_pm_xml_ho_param_name;
    
/* SPR 18861 */
 
	elements_in_pm_xml_mac_param_name = oam_element_in_pm_xml_param_name(pm_xml_mac_param_name); 

/* SPR 18861 */
    g_pm_xml_pdcp_param_start = g_pm_xml_mac_param_start + elements_in_pm_xml_mac_param_name; 
/* SPR 18861 */

	elements_in_pm_xml_pdcp_param_name = oam_element_in_pm_xml_param_name(pm_xml_pdcp_param_name); 

	elements_in_pm_rrm_xml_param_name = oam_element_in_pm_xml_param_name(pm_xml_rrm_param_name);

	/* SPR_12747_CHANGES */
	/* SPR 14792 Changes Start */
	/* Total no. of  KPI counters(Other than Inter-RAT  HO counters), 
	 * to be stored in PM_XML_UPLOAD file is calculated 
	 * and memory allocation is done below for pm_xml_param_value */
    /*SPR 20171 Start*/
    elements_in_pm_xml_param_name = elements_in_pm_xml_conn_st_param_name + 
									elements_in_pm_xml_erab_param_name + 
									elements_in_pm_xml_paging_param_name + 
									elements_in_pm_xml_s1sig_param_name + 
									elements_in_pm_xml_ho_param_name + 
									elements_in_pm_xml_mac_param_name + 
									elements_in_pm_xml_pdcp_param_name; 
	/*SPR 20171 End*/
	OAM_LOG(OAM,OAM_DETAILED,"Elements in pm_xml_conn_st_param_name :::::: %d.\n"
	    "Elements in pm_xml_erab_param_name :::::: %d.\n"
	    "Elements in pm_xml_paging_param_name :::::: %d.\n"
	    "Elements in pm_xml_s1sig_param_name :::::: %d.\n"
	    "Elements in pm_xml_ho_param_name :::::: %d.\n"
	    "Elements in g_pm_xml_mac_param_start :::::: %d.\n"
	    "Elements in pm_xml_mac_param_name :::::: %d.\n"
	    "Elements in g_pm_xml_pdcp_param_start :::::: %d.\n"
	    "Elements in pm_xml_pdcp_param_name :::::: %d.\n"
	    "Elements in pm_rrm_xml_param_name :::::: %d.\n"
	    "Elements::::::%d",
	    elements_in_pm_xml_conn_st_param_name,
	    elements_in_pm_xml_erab_param_name,
	    elements_in_pm_xml_paging_param_name,
	    elements_in_pm_xml_s1sig_param_name,
	    elements_in_pm_xml_ho_param_name,
	    g_pm_xml_mac_param_start,
	    elements_in_pm_xml_mac_param_name,
	    g_pm_xml_pdcp_param_start,
	    elements_in_pm_xml_pdcp_param_name,
	    elements_in_pm_rrm_xml_param_name,
	    elements_in_pm_xml_param_name);

	/* SPR 14792 Changes End */
	/*Coverity FIX 35665 */
	oam_error_code_et error_code = NO_ERROR;
	UInt16  index = OAM_ZERO;

	for ( index= OAM_ZERO ; ((index<(oam_prov_req.num_cells_configured * elements_in_pm_xml_param_name)) 
				&& (index < PM_XML_MAX_PARAM_VALUE)); index++)
	{ 
		if(OAM_FAILURE == 
				oam_mem_alloc(20,(void *)&(pm_xml_param_value[index]),
					&error_code))
		{
			OAM_LOG(OAM, OAM_ERROR,
					"Memory allocation to pm_xml_param_value failed"
					" with Error Code:%d",
					error_code);
			return;
		}
		else
		{
			oam_sprintf(pm_xml_param_value[index], "Dummy");
		}
	}
/* SPR 18861 */
    g_pm_xml_total_element_count_offset = elements_in_pm_xml_param_name;
	for (index=0; ((index< g_pm_xml_total_element_count_offset) && (index < PM_XML_MAX_PARAM_VALUE)); index++)
/* SPR 18861 */
	{
		if(OAM_FAILURE ==
				oam_mem_alloc(OAM_TWENTY,(void *)&(pm_rrm_xml_param_value[index]),
					&error_code))
		{
			OAM_LOG(OAM, OAM_ERROR,
					"Memory allocation to pm_rrm_xml_param_value failed"
					" with Error Code:%d",
					error_code);
			return;
		}
		else
		{
			oam_sprintf(pm_rrm_xml_param_value[index], "Dummy");
		}
	}

	oam_strncpy(mkdir_cmd,"mkdir ./PM_XML_UPLOAD",sizeof("mkdir ./PM_XML_UPLOAD"));
	oam_system(mkdir_cmd);

	OAM_FUN_EXIT_TRACE();
}

/****************************************************************************
 * Function Name  : oam_pm_xml_handle_spv_for_pm_params
 * Description    : This function handles SPV commands to handle PM feature
 * Inputs         : param_name - pack name
 * Outputs        :
 * Returns        : NONE
 ******************************************************************************/
/* SPR_12955_FIXES */
oam_return_et oam_pm_xml_handle_spv_for_pm_params
(
 UInt16 trans_id
 )
/* SPR_12955_FIXES */
{
	OAM_FUN_ENTRY_TRACE();
	oam_return_et retVal = OAM_SUCCESS; 	
	oam_error_code_et p_error_code = NO_ERROR;
	if(OAM_TRUE == g_pm_xml_params.periodic_reporting_ongoing)
	{
		/* check if the disabled req has been received */
		if (OAM_ZERO == g_pm_xml_params.pm_xml_file_reporting)
		{
			/* close the current file and stop upload interval timer and the file write timer */
			if(*(int*)g_pm_xml_params.pm_xml_upload_interval_timer_id > 0)
			{
#ifndef OAM_UT_FRAMEWORK
				oam_pm_timer_stop(g_pm_xml_params.pm_xml_upload_interval_timer_id);
#endif
				g_pm_xml_params.pm_xml_upload_interval_timer_id = 0;
			}
			if (*(int*)g_pm_xml_params.pm_xml_upload_timer_id > 0)
			{
#ifndef OAM_UT_FRAMEWORK
				oam_pm_timer_stop(g_pm_xml_params.pm_xml_upload_timer_id);
#endif
				g_pm_xml_params.pm_xml_upload_timer_id = 0;
			}
			if (*(int*)g_pm_xml_params.pm_xml_file_write_timer_id > 0)
			{
#ifndef OAM_UT_FRAMEWORK
				oam_pm_timer_stop(g_pm_xml_params.pm_xml_file_write_timer_id);
#endif
				g_pm_xml_params.pm_xml_file_write_timer_id = 0;
			}
			oam_pm_xml_file_close();
			/* SPR_12955_FIXES */
			oam_fill_pm_xml_params_and_config_lower_layers(trans_id);
			/* SPR_12955_FIXES */
			/* upload request shall be automatically sent when upload timer expires
			   periodic reporting ongoing flag shall be OAM_FALSE now since reporting has been disabled */
			g_pm_xml_params.periodic_reporting_ongoing = OAM_FALSE;
		}
		else
		{
			/*SPV for Upload Interval timer/upload timer/sampling interval
			  have been received.
			  stop the upload interval timer and the file write timer */
			if(*(int*)g_pm_xml_params.pm_xml_upload_interval_timer_id>0)
			{
#ifndef OAM_UT_FRAMEWORK
				oam_pm_timer_stop(g_pm_xml_params.pm_xml_upload_interval_timer_id);
#endif
				g_pm_xml_params.pm_xml_upload_interval_timer_id = 0;
			}
			if (*(int*)g_pm_xml_params.pm_xml_file_write_timer_id>0)
			{
#ifndef OAM_UT_FRAMEWORK
				oam_pm_timer_stop(g_pm_xml_params.pm_xml_file_write_timer_id);
#endif
				g_pm_xml_params.pm_xml_file_write_timer_id = 0;
			}
			oam_pm_xml_file_close();
			/* SPR 22296 Fix Start */
			if((g_pm_xml_params.spv_bitmask & PM_XML_SPV_SAMPLING_INTERVAL_TIMER)
				||(g_pm_xml_params.spv_bitmask & PM_XML_SPV_FILE_WRITE_TIMER)
				||(g_pm_xml_params.spv_bitmask & PM_XML_SPV_UPLOAD_INTERVAL_TIMER))
			{
				/* SPR_12955_FIXES */
				oam_fill_pm_xml_params_and_config_lower_layers(trans_id);
				/* SPR_12955_FIXES */
				oam_construct_n_send_set_resp_to_usr(OAM_RESULT_SUCCESS,
						OAM_ZERO,OAM_NULL,OAM_ZERO,
						OAM_VERSION_ID, OAM_MODULE_ID,
						/* SPR_12955_FIXES */
						oam_set_config_req.src_id, trans_id, OAM_ZERO, OAM_ZERO,
						/* SPR_12955_FIXES */
						&p_error_code);

			}
			/* SPR 22296 Fix End */


			/* and open a new one for fresh reporting */
			retVal= oam_pm_xml_start_pm_reporting();
			file_close.flag = OAM_ZERO; 
		}
	}
	else
	{
		/* SPR_12955_FIXES */	
		oam_fill_pm_xml_params_and_config_lower_layers(trans_id);
		/* SPR_12955_FIXES */
        /*SPR 21420 FIXED START*/
		if (OAM_ONE == g_pm_xml_params.pm_xml_file_reporting)
		{
			retVal = oam_pm_xml_start_pm_reporting();
		}
        /*SPR 21420 FIXED END*/

        /*SPR 21077 Fixed End*/    
		/*SPR_10579 Start*/
		oam_handle_multiple_config_resp.bit_mask = 0x07;
		/*SPR_10579 End*/
		oam_update_rrc_kpi_inds_handling_strct();
		file_close.flag = OAM_ZERO; 
	} 
	g_pm_xml_params.spv_bitmask = 0;
	OAM_FUN_EXIT_TRACE();
	return retVal;	
}

/****************************************************************************
 * Function Name  : oam_fill_pm_xml_params_and_config_lower_layers
 * Description    : This function send the configuration of PM to lower layer
 * Inputs         : param_name - pack name
 * Outputs        :
 * Returns        : NONE
 ******************************************************************************/
/* SPR_12955_FIXES */	
void oam_fill_pm_xml_params_and_config_lower_layers
(
 UInt16 trans_id
 )
/* SPR_12955_FIXES */	
{
    OAM_FUN_ENTRY_TRACE();
    /*SPR 21941 Start*/
    UInt8 l2_instance = OAM_ZERO;
    /*SPR 21941 End*/
	oam_mac_config_KPI_stats_req_t mac_config_kpi_stats_req;
	oam_pdcp_config_KPI_stats_req_t pdcp_config_kpi_stats_req;
	oam_rrc_configure_kpi_stats_req_params_t rrc_config_kpi_stats_req;
	/* coverity 35664 */
	oam_error_code_et error_code = NO_ERROR;
	oam_counter_t index = OAM_ZERO;
	if(TR069_MODULE_ID == oam_set_config_req.src_id)
	{
		g_pm_xml_params.pm_xml_sampling_interval_timer = oam_prov_req.oam_igd_params.perf_mgmt_params.sampling_interval_timer * (60); 
		g_pm_xml_params.pm_xml_upload_interval_timer = oam_prov_req.oam_igd_params.perf_mgmt_params.PeriodicUploadInterval;
		g_pm_xml_params.pm_xml_file_write_timer = oam_prov_req.oam_igd_params.perf_mgmt_params.file_write_timer * (60);
        /*SPR 20171 Start */

        OAM_LOG(OAM, OAM_DETAILED, "TR069 : Sampling timer = %d Upload Timer %llu, File Write Timer %llu",
                g_pm_xml_params.pm_xml_sampling_interval_timer,  
                g_pm_xml_params.pm_xml_upload_interval_timer,
                g_pm_xml_params.pm_xml_file_write_timer
               );		
        /*SPR 20171 End */

	}

	mac_config_kpi_stats_req.durationT = g_pm_xml_params.pm_xml_sampling_interval_timer;
	pdcp_config_kpi_stats_req.durationT = g_pm_xml_params.pm_xml_sampling_interval_timer;

    /* SPR 15491 START */
    if (oam_prov_req.oam_igd_params.perf_mgmt_params.Enable == OAM_ONE)
    {
        rrc_config_kpi_stats_req.durationT = g_pm_xml_params.pm_xml_sampling_interval_timer * 1000;
        pdcp_config_kpi_stats_req.periodicReporting = g_pm_xml_params.pm_xml_file_reporting;
        mac_config_kpi_stats_req.periodicReporting = g_pm_xml_params.pm_xml_file_reporting;
    }
    else
    {
        rrc_config_kpi_stats_req.durationT = OAM_ZERO;
        pdcp_config_kpi_stats_req.periodicReporting = OAM_ZERO;
        mac_config_kpi_stats_req.periodicReporting = OAM_ZERO;
    }
    /* SPR 15491 END */

	mac_config_kpi_stats_req.kpiToReportBitMap = OAM_MAC_KPIBITMAP;
	pdcp_config_kpi_stats_req.kpiToReportBitMap = OAM_PDCP_KPIBITMAP;
	rrc_config_kpi_stats_req.kpiToReportBitMap = OAM_RRC_KPIBITMAP;	 

    /* SPR 15491 START */
    /* Code Removed */
    /* SPR 15491 END */

	oam_handle_multiple_config_resp.bit_mask = OAM_HANDLE_MULTI_CONFIG_RESP;
	oam_handle_multiple_config_resp.success_fail_bitmask = OAM_HANDLE_MULTI_CONFIG_RESP; 

	/* CSR No 00055803 Start */
	/* KPI_CONFIG_REQUEST timer removed */
	/* CSR No 00055803 End */ 
	if(OAM_ZERO == g_pm_xml_params.pm_xml_file_reporting)
	{
		rrc_config_kpi_stats_req.periodicReporting = 0; 
		g_pm_rrm_xml_params.rrm_periodic_reporting_ongoing = OAM_FALSE; 
	}
	else if (1  == g_pm_xml_params.pm_xml_file_reporting)
	{
		rrc_config_kpi_stats_req.periodicReporting = 1; 
		g_pm_rrm_xml_params.rrm_periodic_reporting_ongoing = OAM_TRUE;
		rrc_config_kpi_stats_req.durationT = g_pm_xml_params.pm_xml_sampling_interval_timer ;
		g_pm_rrm_xml_params.rrm_periodic_duration = (g_pm_xml_params.pm_xml_sampling_interval_timer);

	}


	oam_pm_update_mac_kpi_info( 
			g_pm_xml_params.pm_xml_file_reporting, 
			g_pm_xml_params.pm_xml_sampling_interval_timer,
			g_pm_xml_params.pm_xml_upload_interval_timer,   
			mac_config_kpi_stats_req.kpiToReportBitMap 
			);
	oam_pm_update_pdcp_kpi_info(
			g_pm_xml_params.pm_xml_file_reporting, 
			g_pm_xml_params.pm_xml_sampling_interval_timer,
			g_pm_xml_params.pm_xml_upload_interval_timer, 
			pdcp_config_kpi_stats_req.kpiToReportBitMap
			);
	oam_pm_update_rrc_kpi_info
		(
		 &rrc_config_kpi_stats_req,
		 g_pm_xml_params.pm_xml_file_reporting,
		 g_pm_xml_params.pm_xml_sampling_interval_timer,
		 rrc_config_kpi_stats_req.kpiToReportBitMap
		);
	if (NULL != p_cm_cntxt)
	{
		if(p_cm_cntxt->state != OAM_CM_ST_ACTIVE )
		{
			OAM_LOG(OAM, OAM_DETAILED, "Cell is not in Active State");
		}

		else

		{

			OAM_LOG(OAM, OAM_INFO, "Trans_id: %d", trans_id);	
/*SPR 21941 Start*/
            for(l2_instance=OAM_ZERO;l2_instance<OAM_MAX_NUM_L2_INSTANCE;l2_instance++)
            {
                if(PDCP_OAM_INIT_CNF & l2_comm_info[l2_instance].init_flag)
                {
			if (OAM_FAILURE == oam_mac_config_kpi_stats_req(
						/* SPR_12955_FIXES */	
                                &mac_config_kpi_stats_req, OAM_MODULE_ID, trans_id, &error_code, l2_instance))
				/* SPR_12955_FIXES */	
			{
				OAM_LOG(OAM,OAM_INFO,"Error in oam_mac_config_kpi_stats_req with error code %d",
						error_code);

			}
			if (OAM_FAILURE == oam_pdcp_config_kpi_stats_req(
						/* SPR_12955_FIXES */	
                                &pdcp_config_kpi_stats_req, OAM_MODULE_ID, trans_id, &error_code, l2_instance))
				/* SPR_12955_FIXES */	
			{
				OAM_LOG(OAM,OAM_INFO,"Error in oam_pdcp_config_kpi_stats_req with error code %d",
						error_code);

			}
                }
            }
/*SPR 21941 End*/
			if (OAM_FAILURE == oam_pm_send_rrc_kpi_config_req(trans_id, &error_code, rrc_config_kpi_stats_req.kpiToReportBitMap)) 			                                {
				OAM_LOG(OAM,OAM_ERROR,"Error in oam_pm_send_rrc_kpi_config_req with error code %d",
						error_code);
			}
			for(index = OAM_ZERO; index < oam_prov_req.num_cells_configured; index++)
			{        
				/* SPR_12955_FIXES */
				if(OAM_FAILURE == oam_pm_rrm_send_config_req(oam_prov_req.cell_cntxt.cell_ctxt_status[index].cid, trans_id, &error_code))
					/* SPR_12955_FIXES */
				{
					OAM_LOG(OAM,OAM_INFO,"Error in oam_pm_rrm_send_config_req with error code %d",
							error_code);
				}
			}
			/* SPR 14019 FIX Start */
#ifdef PERF_STATS
			/* SPR 14019 FIX End */
			if(OAM_FAILURE == oam_pm_pdcp_send_config_perf_stats_req(&error_code))
			{
				OAM_LOG(OAM,OAM_INFO,"Error in oam_pm_pdcp_send_config_perf_stats_req with error code %d",
						error_code);
			}
			/* SPR 14019 FIX Start */
#endif
			/* SPR 14019 FIX End */

		}
	}
	OAM_FUN_EXIT_TRACE();
}
/****************************************************************************
 * Function Name  : oam_pm_xml_start_pm_reporting
 * Description    : This function starts PM reporting 
 * Inputs         : param_name - pack name
 * Outputs        :
 * Returns        : NONE
 ******************************************************************************/
oam_return_et oam_pm_xml_start_pm_reporting(
/* + SPR 17439 */
 void
)
/* - SPR 17439 */
{
	oam_return_et retVal = OAM_SUCCESS;
	/* coverity 35665 */
    /* SPR 17777 fix code removd */

	OAM_FUN_ENTRY_TRACE();
	/*SPR 12822 FIX START*/
	qvLock(pm_mutex_lock);
	OAM_LOG(OAM, OAM_DETAILED,"Holding Mutex in context");
	/*SPR 12822 FIX END*/
	if (OAM_ONE == g_pm_xml_params.pm_xml_file_reporting)
	{
		oam_start_periodic_upload_interval_timer();
        /* SPR 17777 fix */
		if(OAM_FAILURE  == oam_pm_xml_file_open())
		{
			OAM_LOG(OAM,OAM_WARNING,"Error in File opening ");
		}

		retVal = oam_start_periodic_upload_time(g_pm_xml_params.periodic_upload_timer_str);

		g_pm_xml_params.pm_xml_file_write_timer_id = 
			oam_pm_timer_start("PERIODIC_PM_XML_FILE_WRITER",
					g_pm_xml_params.pm_xml_file_write_timer);
		if (OAM_ZERO == g_pm_xml_params.pm_xml_file_write_timer_id)
		{
			OAM_LOG(OAM, OAM_DETAILED,"Releasing Mutex for context");
			/* coverity fix 66593 start */
			qvUnlock(pm_mutex_lock); 
			/* coverity fix 66593 end */
			return OAM_FAILURE;
		}    

		g_pm_rrm_xml_params.pm_rrm_xml_file_write_timer_id = oam_pm_timer_start("PERIODIC_PM_RRM_XML_FILE_WRITER", g_pm_rrm_xml_params.pm_rrm_xml_file_write_timer); 
		if (OAM_ZERO == g_pm_rrm_xml_params.pm_rrm_xml_file_write_timer_id)
		{
			OAM_LOG(OAM, OAM_DETAILED,"Releasing Mutex for context");
			/* coverity fix 66593 start */
			qvUnlock(pm_mutex_lock); 
			/* coverity fix 66593 end */
			return OAM_FAILURE;
		}    
		g_pm_xml_params.periodic_reporting_ongoing = OAM_TRUE;
	}
	/*SPR 12822 FIX START*/
	OAM_LOG(OAM, OAM_DETAILED,"Releasing Mutex for context");
	qvUnlock(pm_mutex_lock);
	/*SPR 12822 FIX END*/
	OAM_FUN_EXIT_TRACE();   
	return retVal;

}

/****************************************************************************
 * Function Name  : oam_start_periodic_upload_interval_timer
 * Description    : This function handles the upload timer 
 * Inputs         : param_name - pack name
 * Outputs        :
 * Returns        : NONE
 ******************************************************************************/
void oam_start_periodic_upload_interval_timer(
/* + SPR 17439 */
 void
)
/* - SPR 17439 */
{
	struct tm* tm_info;
	time_t now = OAM_TIME(NULL);
	OAM_FUN_ENTRY_TRACE();
	int diff = 0;
	tm_info = OAM_LOCALTIME(&now);

	/* Coverity FIX 41140 */
	if( NULL == tm_info ) { OAM_LOG(OAM,OAM_DETAILED,"PERIODIC_UPLOAD_INTERVAL_TIMER is NULL "); return; }    

	if ( 3600 == g_pm_xml_params.pm_xml_upload_interval_timer)
	{
		diff = 60 * (59 - tm_info->tm_min) + (58 - tm_info->tm_sec);
	}
	else
	{
		diff = 60*60*(24-tm_info->tm_hour) + 60 * (59 - tm_info->tm_min) + (58 - tm_info->tm_sec);
	}
	diff++; 
	OAM_LOG(OAM,OAM_DETAILED,"diff now, upload_interval_time %d", diff);



	if(diff<=0)
		diff = g_pm_xml_params.pm_xml_upload_interval_timer;




	g_pm_xml_params.pm_xml_upload_interval_timer_id = 
		oam_pm_timer_start("PERIODIC_UPLOAD_INTERVAL_TIMER",
				diff);
	if (OAM_ZERO == g_pm_xml_params.pm_xml_upload_interval_timer_id)
	{
		return;
	}    
	g_pm_xml_params.pm_xml_upload_interval_timer_diff = diff;
	OAM_FUN_EXIT_TRACE();
}

/****************************************************************************
 * Function Name  : oam_start_periodic_upload_time
 * Description    : This function handles the upload timer
 * Inputs         : param_name - pack name
 * Outputs        :
 * Returns        : NONE
 ******************************************************************************/
oam_return_et oam_start_periodic_upload_time(char *uploadTimeStr)
{
	struct tm time_struct;
	time_t now = OAM_TIME(NULL);
	time_t uploadTime = OAM_TIME(NULL);
	char a, b; 
	int diff = 0;
	int now_sec = 0;
	int upload_sec = 0;
	UInt16 scanresult = 0;
	struct tm* tm_info;
	oam_return_et retVal = OAM_SUCCESS;
	OAM_FUN_ENTRY_TRACE();
	tm_info = OAM_GMTIME(&now); 
	OAM_NULL_CHECK(NULL != tm_info);/* Coverity FIX 35426 52765 */

	OAM_ASSERT(NULL != OAM_CTIME(&now));/* Coverity FIX 35425 */
	OAM_LOG(OAM,OAM_DETAILED,"current time::%s", OAM_CTIME(&now));
	scanresult =  snscanf( uploadTimeStr, sizeof(time_struct.tm_year)+sizeof(time_struct.tm_mon)+
			sizeof(time_struct.tm_mday)+sizeof(a)+sizeof(time_struct.tm_hour)+
			sizeof(time_struct.tm_min)+sizeof(time_struct.tm_sec)+sizeof(b)+OAM_FIVE,
			"%d-%d-%d%c%d:%d:%d%c",
			&time_struct.tm_year,
			&time_struct.tm_mon,
			&time_struct.tm_mday,&a,
			&time_struct.tm_hour,
			&time_struct.tm_min,
			&time_struct.tm_sec,&b);

	time_struct.tm_year -= 1900;
	/* CSR00070207 FIX*/
	time_struct.tm_mon -= 1; 
	/* CSR00070207 FIX*/
	time_struct.tm_isdst= 0;
	uploadTime = OAM_MKTIME(&time_struct);
	if(uploadTime == M_OAM_ONE)
	{
		OAM_LOG(OAM,OAM_INFO,"Invalid uploadTime[%ld]", uploadTime);
		return OAM_FAILURE;       
	}
	OAM_LOG(OAM,OAM_DETAILED,"UploadTime read after parsing the string entered::%s %d",
			OAM_CTIME(&uploadTime), scanresult);
	OAM_LOG(OAM,OAM_DETAILED,"Current Time of the system::%s",
			OAM_CTIME(&now));

	if(g_pm_xml_params.pm_xml_upload_interval_timer == 3600)
	{
		upload_sec = time_struct.tm_min * 60 + time_struct.tm_sec;
		now_sec = tm_info->tm_min *60 + tm_info->tm_sec;

		if ( upload_sec > now_sec)
		{
			diff = upload_sec - now_sec;
		}
		else
		{
			diff =  3600 -(now_sec - upload_sec);
		}
	}
	else
	{
		upload_sec = 60*60*time_struct.tm_hour + time_struct.tm_min * 60 + time_struct.tm_sec;
		now_sec = 60*60*tm_info->tm_hour + tm_info->tm_min *60 + tm_info->tm_sec;

		if ( upload_sec > now_sec)
		{
			diff = upload_sec - now_sec;
		}
		else
		{
			diff =  86400 - (now_sec - upload_sec);
		}

	}
	OAM_LOG(OAM,OAM_DETAILED,"Upload time Diff[%d]", diff);

	if(diff <=0)
		diff = g_pm_xml_params.pm_xml_upload_interval_timer;
	g_pm_xml_params.pm_xml_upload_timer_id = oam_pm_timer_start("PERIODIC_UPLOAD_TIMER", diff);
	if (OAM_ZERO == g_pm_xml_params.pm_xml_upload_timer_id)
	{
		return OAM_FAILURE;
	}    
	OAM_FUN_EXIT_TRACE();
	return retVal;

}
/* SPR 22296 Fix Start */
#ifndef PDCP_GTPU_INTF
/****************************************************************************
 * Function Name  : oam_handle_pm_reporting_at_startup
 * Description    : This function handles pm reporting at startup
 * Inputs         : param_name - pack name
 * Outputs        :
 * Returns        : NONE
 ******************************************************************************/
oam_return_et oam_handle_pm_reporting_at_startup(
/* + SPR 17439 */
 void
)
/* - SPR 17439 */
{
	oam_return_et retVal = OAM_SUCCESS; 
	/* SPR_12955_FIXES */	
	UInt16 trans_id = OAM_ZERO;
	/* SPR_12955_FIXES */	
	OAM_FUN_ENTRY_TRACE();

	if(OAM_ONE == oam_prov_req.oam_igd_params.perf_mgmt_params.Enable)
	{
		g_pm_xml_params.pm_xml_file_reporting = 1;

		OAM_LOG(OAM, OAM_DETAILED, "init PeriodicUploadInterval:%d. "
		        "init sampling_interval_timer :%d. "
		        "init PeriodicUploadTime:%s. "
		        "init file_write_timer:%d.",
				oam_prov_req.oam_igd_params.perf_mgmt_params.PeriodicUploadInterval,
				oam_prov_req.oam_igd_params.perf_mgmt_params.sampling_interval_timer,
				oam_prov_req.oam_igd_params.perf_mgmt_params.PeriodicUploadTime,
				oam_prov_req.oam_igd_params.perf_mgmt_params.file_write_timer);

		g_pm_xml_params.pm_xml_upload_interval_timer = oam_prov_req.oam_igd_params.perf_mgmt_params.PeriodicUploadInterval;

		g_pm_xml_params.pm_xml_sampling_interval_timer = oam_prov_req.oam_igd_params.perf_mgmt_params.sampling_interval_timer * 60;

		oam_strcpy((char*)g_pm_xml_params.periodic_upload_timer_str, 
				(const char*)oam_prov_req.oam_igd_params.perf_mgmt_params.PeriodicUploadTime);

		g_pm_xml_params.pm_xml_file_write_timer = oam_prov_req.oam_igd_params.perf_mgmt_params.file_write_timer * (60);
		g_pm_rrm_xml_params.pm_rrm_xml_file_write_timer = 3600;

		/* SPR_12955_FIXES */	
		trans_id = oam_get_new_trans_id();	
		retVal = oam_pm_xml_handle_spv_for_pm_params(trans_id);
		/* SPR_12955_FIXES */	
		/*SPR 11120 FIX */
		Config_Flag = OAM_ONE;
		/*SPR 11120 FIX */
	}
	OAM_FUN_EXIT_TRACE();
	return retVal;
}
#endif
/* SPR 22296 Fix End */

/****************************************************************************
 * Function Name  : oam_send_file_upload_request
 * Description    : This function handles upload request of PM xml file
 * Inputs         : param_name - pack name
 * Outputs        :
 * Returns        : NONE
 ******************************************************************************/
void oam_send_file_upload_request(
/* + SPR 17439 */
 void
)
/* - SPR 17439 */
{
	UInt16 trans_id = OAM_ZERO;
	char username[255] = {OAM_ZERO};
	char password[255] = {OAM_ZERO};
	char directory_path[255] = {OAM_ZERO};
	char cwd[1024] = {OAM_ZERO};
	char url[256] = {OAM_ZERO};

	UInt16 dir_size = OAM_ZERO;
	oam_sprintf(username,"Username=%s", oam_prov_req.oam_igd_params.perf_mgmt_params.Username);
	oam_sprintf(password, "Password=%s", oam_prov_req.oam_igd_params.perf_mgmt_params.Password);
	oam_sprintf(url, "URL=%s", oam_prov_req.oam_igd_params.perf_mgmt_params.URL);

	OAM_GETCWD(cwd,sizeof(cwd));

	dir_size = OAM_ZERO;
	oam_strcpy(directory_path,"DirectoryPath=");
	dir_size = oam_strlen(directory_path);
	oam_strcpy(&directory_path[dir_size],cwd);
	dir_size = OAM_ZERO;
	dir_size = oam_strlen(directory_path);
	oam_strcpy(&directory_path[dir_size],"/PM_XML_UPLOAD/");
	OAM_LOG(OAM,OAM_DETAILED,"directory_path=%s",directory_path);

	char *pm_param_list[] = {"AdminControlCommand=FileUpload",url,username,password,directory_path};

	trans_id = oam_get_new_trans_id();
	OAM_LOG(OAM,OAM_DETAILED,"Sending Upload Request to TR069");
    /* SPR 17777 fix */
	oam_tr69_controll_cmd_req(pm_param_list, OAM_FOUR, OAM_SUB_MODULE_PM,OAM_MODULE_ID,
			TR069_MODULE_ID, OAM_ONE, trans_id, OAM_API_VERSION_1, OAM_ZERO);

}

void oam_pm_xml_handle_file_upload_response(UInt8 upload_status)
{
	OAM_FUN_ENTRY_TRACE();
	/* Spr 12880 Fix Start*/
	UInt8 CellId = OAM_INVALID_CELL_ID;
	/* Spr 12880 Fix Start*/
	if( OAM_ZERO == upload_status )
	{
		OAM_LOG(OAM,OAM_DETAILED,"In Success Case of File Upload response");
		if(OAM_ONE == g_pm_xml_params.is_pm_xml_alarm_raised)
		{
			OAM_ALARM(PM_DATA_UPLOAD_FAILURE_ALARM_ID,
					5, NOTIFY_CLEARED_ALARM, PROCESSING_ERROR_ALARM,
					UNAVAILABLE, EXPEDITED_ALARM,
					/* Spr 12880 Fix Start*/
					"file upload response success", OAM_NULL,CellId);
			/* Spr 12880 Fix Start*/
			g_pm_xml_params.is_pm_xml_alarm_raised = OAM_ZERO;
		}
	}
	if (OAM_ONE == upload_status)
	{
		OAM_LOG(OAM,OAM_DETAILED,"In Failure Case of File Upload response");
		if(OAM_ZERO == g_pm_xml_params.is_pm_xml_alarm_raised)
		{
			OAM_ALARM(PM_DATA_UPLOAD_FAILURE_ALARM_ID,
					2, NOTIFY_NEW_ALARM, PROCESSING_ERROR_ALARM,
					UNAVAILABLE, EXPEDITED_ALARM,
					/* Spr 12880 Fix Start*/
					"file upload response failed", OAM_NULL,CellId);
			/* Spr 12880 Fix Start*/
			g_pm_xml_params.is_pm_xml_alarm_raised = OAM_ONE;
		}
	}

	OAM_FUN_EXIT_TRACE();
}

/****************************************************************************
 * Function Name  : oam_send_rrc_reset_config_request
 * Description    : This function sends  RESET CELL STATS REQ to RRC layer
 * Inputs         : Pointer to message buffer
 * Outputs        : p_error_code       : Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
	oam_return_et
oam_send_rrc_reset_config_request(void *p_api_buf, oam_error_code_et *p_error_code)
{
	oam_rrc_reset_cell_stats_req_t rrc_reset_cell_stats_req;
	UInt16 trans_id = get_word_from_header((UInt8 *)p_api_buf + OAM_ZERO);
	oam_memset(&rrc_reset_cell_stats_req, OAM_ZERO, sizeof(oam_rrc_reset_cell_stats_req_t));
	OAM_FUN_ENTRY_TRACE();
	/* SPR_12609_FIX_START */
	rrc_reset_cell_stats_req.cell_index = oam_prov_req.cell_cntxt.curr_cell_cid;
	/* SPR_12609_FIX_END */
	if (OAM_FAILURE == oam_rrc_il_send_rrc_oam_reset_cell_stats_req(
				&rrc_reset_cell_stats_req, OAM_MODULE_ID, RRC_MODULE_ID, trans_id,
				oam_prov_req.cell_cntxt.curr_cell_cid))
	{
		/* coverity 62584 */
		*p_error_code = OAM_INTERNAL_ERROR;
		OAM_LOG(OAM,OAM_ERROR,"Error in oam_send_rrc_reset_config_request");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_handle_rrc_stats_n_reset_req
 * Description    : This function handles stats from RRC layer
 * Inputs         :
 * Outputs        : p_error_code     :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
	oam_return_et
oam_handle_rrc_stats_n_reset_req(void *p_api_buf)
{
	/*start: CSR00070663 FIX*/
	UInt16 trans_id = OAM_ZERO,transaction_id=OAM_ZERO;
	UInt16 dest_id = OAM_ZERO;
	oam_return_et ret_val = OAM_SUCCESS;
	OAM_FUN_ENTRY_TRACE();

	/* SPR_12955_FIXES */
	OAM_LOG(OAM, OAM_INFO, "Trans_id : %d", transaction_id);	
    /* SPR 17883 fix start */
    transaction_id  = get_word_from_header((UInt8*)(p_api_buf) + OAM_TRANSACTION_ID);
    /* SPR 17883 fix end */

	/* SPR_12955_FIXES */
#ifndef OAM_UT_FRAMEWORK
	if(OAM_FAILURE == oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(transaction_id,&trans_id,&dest_id))
	{
		OAM_LOG(OAM,OAM_INFO,"Error finding TCB Entry");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
#endif
	ret_val = oam_handle_rrc_stats_recv_resp(p_api_buf,trans_id,dest_id);
	if (ret_val == OAM_FAILURE)
	{
		OAM_LOG(OAM,OAM_INFO,"Error Handling RRC_OAM_GET_CELL_STATS_RESP");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	/*end: CSR00070663 FIX*/
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_handle_rrc_reset_resp
 * Description    : This function handles reset stats response received from 
 *		    RRC in response to request RRC_OAM_RESET_CELL_STATS_REQ
 *		    sent from OAM
 * Inputs         : Pointer to message buffer received from RRC in response
 *		    to message  RRC_OAM_RESET_CELL_STATS_REQ
 * Outputs        : Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
	oam_return_et
oam_handle_rrc_reset_resp(void *p_api_buf, oam_error_code_et *p_error_code)
{
	/*start: CSR00070663 FIX*/
	UInt16 trans_id = OAM_ZERO,transaction_id=OAM_ZERO;
	UInt16 dest_id = OAM_ZERO;
	UInt16 data_len = OAM_ZERO;
	SInt32 length = OAM_ZERO;
	oam_rrc_reset_cell_stats_resp_t oam_rrc_get_cell_stats_resp;
	oam_error_code_et received_error = NO_ERROR;
	OAM_FUN_ENTRY_TRACE();

	received_error = *p_error_code;
	/* SPR_12955_FIXES */	
    /* SPR 17883 fix start */
    transaction_id  = get_word_from_header((UInt8*)(p_api_buf) + OAM_TRANSACTION_ID);
	OAM_LOG(OAM, OAM_INFO, "Trans_id: %d", transaction_id);	
    /* SPR 17883 fix end */
	/* SPR_12955_FIXES */	
	if(OAM_FAILURE == oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(transaction_id,&trans_id,&dest_id))
	{
		OAM_LOG(OAM,OAM_INFO,"Error finding TCB Entry");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	oam_memset(&oam_rrc_get_cell_stats_resp, OAM_ZERO, sizeof(oam_rrc_reset_cell_stats_resp_t));
	data_len = get_word_from_header((UInt8*)(p_api_buf) + OAM_INTF_HDR_MSG_SIZE_OFFSET);
	if (OAM_FAILURE == oam_rrc_il_parse_rrc_oam_reset_cell_stats_resp(&oam_rrc_get_cell_stats_resp,
				p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE,(data_len - LOWER_LAYER_INTERFACE_HEADER_SIZE),
				&length))
	{
		OAM_LOG(OAM,OAM_INFO,
				"RRC_OAM_RESET_CELL_STATS_RESP Parsing Failure");
		*p_error_code = OAM_INTERNAL_ERROR; /* Cov Cid 62585 Fix */
		oam_construct_n_send_set_resp_to_usr(OAM_RESULT_FAILURE,
				OAM_ERR_INTERNAL_ERROR,(UInt8 *)"RECEIVED FAILURE FROM RRC",oam_strlen("RECEIVED FAILURE FROM RRC") + OAM_ONE,
				OAM_VERSION_ID, OAM_MODULE_ID,
				dest_id, trans_id, OAM_ZERO, OAM_ZERO,
				p_error_code);
		if (*p_error_code != OAM_INTERNAL_ERROR)
		{
			OAM_LOG(OAM,OAM_ERROR,"Error sending response to user, err = %d",
					*p_error_code);
		}
		cli_flag = OAM_FALSE;
		OAM_FUN_EXIT_TRACE();
		return OAM_SUCCESS;
	}
	/* SPR_12609_FIX_START */ 
	if (oam_rrc_get_cell_stats_resp.cell_index != oam_prov_req.cell_cntxt.curr_cell_cid)
		/* SPR_12609_FIX_END */ 
	{
		OAM_LOG(OAM,OAM_INFO,
				"Received Invalid cell_index[%d] from RRC",
                oam_rrc_get_cell_stats_resp.cell_index);
		*p_error_code = OAM_INTERNAL_ERROR;
		oam_construct_n_send_set_resp_to_usr(OAM_RESULT_FAILURE,
				OAM_ERR_INTERNAL_ERROR,(UInt8 *)"RECEIVED INVALID CELL INDEX FROM RRC",oam_strlen("RECEIVED INVALID CELL INDEX FROM RRC") + OAM_ONE,
				OAM_VERSION_ID, OAM_MODULE_ID,
				dest_id, trans_id, OAM_ZERO, OAM_ZERO,
				p_error_code);
		if (*p_error_code != OAM_INTERNAL_ERROR)
		{
			OAM_LOG(OAM,OAM_ERROR,"Error sending response to user, err = %d",
					*p_error_code);
		}
		cli_flag = OAM_FALSE;
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	if (oam_rrc_get_cell_stats_resp.response != RRC_FAILURE)
	{
		OAM_LOG(OAM,OAM_DETAILED,"Successfully sent the response [RRC_OAM_RESET_CELL_STATS_RESP] to user");
		oam_construct_n_send_set_resp_to_usr(OAM_RESULT_SUCCESS,
				OAM_ZERO,OAM_NULL,OAM_ZERO,
				OAM_VERSION_ID, OAM_MODULE_ID,
				dest_id, trans_id, OAM_ZERO, OAM_ZERO,
				p_error_code);
		if (*p_error_code != received_error)
		{
			OAM_LOG(OAM,OAM_ERROR,"Error sending response to user, err =  %d",
					*p_error_code);
		}
		cli_flag = OAM_FALSE;
	}
	else
	{
		OAM_LOG(OAM,OAM_INFO,"Failure Resetting the RRC KPI Stats");
		*p_error_code = OAM_INTERNAL_ERROR;
		oam_construct_n_send_set_resp_to_usr(OAM_RESULT_FAILURE,
				OAM_ERR_INTERNAL_ERROR,(UInt8 *)"RECEIVED FAILURE FROM RRC",oam_strlen("RECEIVED FAILURE FROM RRC") + OAM_ONE,
				OAM_VERSION_ID, OAM_MODULE_ID,
				dest_id, trans_id, OAM_ZERO, OAM_ZERO,
				p_error_code);
		if (*p_error_code != OAM_INTERNAL_ERROR)
		{
			OAM_LOG(OAM,OAM_ERROR,"Error sending response to user %d",
					*p_error_code);
		}
		cli_flag = OAM_FALSE;
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	/*end: CSR00070663 FIX*/
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_send_config_gpv_resp
 * Description    : This function sends configuration request
 * Inputs         : param_name - pack name
 * Outputs        :
 * Returns        : NONE
 ******************************************************************************/
	oam_return_et
oam_send_config_gpv_resp(
/* + SPR 17439 */
 void
)
/* - SPR 17439 */
{
	oam_error_code_et p_error_code;
	oam_counter_t loop_counter   = OAM_ZERO;
	UInt8 str_count = OAM_ZERO; 
	UInt8 **temp_arr;     /* this is the array name */
	UInt8 size_x; /* this variable will be used for the first  dimension */
	UInt8 size_y; /* this variable will be used for the second dimension */
	/* suppose we want an array of char: a[4][100] */
	size_x = OAM_ZERO;
	size_y = 255;
	UInt8 bit_mask = OAM_ZERO;

	OAM_FUN_ENTRY_TRACE();
	bit_mask = OAM_ONE;
	for(loop_counter=OAM_ZERO;loop_counter < OAM_NO_OF_BITS;loop_counter++)
	{
		if(oam_config.config_param_bit_mask & bit_mask)
		{
			size_x = size_x + OAM_ONE;
		}   
		bit_mask = bit_mask << OAM_ONE;              
	} 

	/*  allocate storage for an array of pointer to char */
	if (OAM_FAILURE == oam_mem_alloc((size_x * sizeof(UInt8 *)),(void *)&temp_arr,&p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to temp_arr failed"
				" with Error Code:%d",
				p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;

	}

	/* for each pocharer, allocate storage for an array of chars */
	for (loop_counter = OAM_ZERO; loop_counter < size_x; loop_counter++)
	{
		if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)&temp_arr[loop_counter],&p_error_code))
		{
			OAM_LOG(OAM, OAM_ERROR,
					"Memory allocation to temp_arr failed"
					" with Error Code:%d",
					p_error_code);
			/*SPR 13857 START*/
			OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, loop_counter, p_error_code);
			/*SPR 13857 END*/
			OAM_FUN_EXIT_TRACE();
			return OAM_FAILURE;
		}
		oam_memset(temp_arr[loop_counter], OAM_ZERO, (size_y * sizeof(UInt8)));
	}

	if((oam_config.config_param_bit_mask & OAM_CONFIG_USERNAME_BIT) && (str_count < size_x))  
		oam_sprintf((char *)temp_arr[str_count++],"%s=%s","Username",
				oam_prov_req.oam_igd_params.perf_mgmt_params.Username);
	if((oam_config.config_param_bit_mask & OAM_CONFIG_PASSWORD_BIT) && (str_count < size_x))  
		oam_sprintf((char *)temp_arr[str_count++],"%s=%s","Password",
				oam_prov_req.oam_igd_params.perf_mgmt_params.Password);
	if((oam_config.config_param_bit_mask & OAM_CONFIG_URL_BIT) && (str_count < size_x))  
		oam_sprintf((char *)temp_arr[str_count++],"%s=%s","URL",
				oam_prov_req.oam_igd_params.perf_mgmt_params.URL);

	if((oam_config.config_param_bit_mask & OAM_CONFIG_ENABLE_BIT) && (str_count < size_x))  
		oam_sprintf((char *)temp_arr[str_count++],"%s=%d","Enable",
				oam_prov_req.oam_igd_params.perf_mgmt_params.Enable);

	if((oam_config.config_param_bit_mask & OAM_CONFIG_SAMPLING_TIMER_BIT ) && (str_count < size_x))  
		oam_sprintf((char *)temp_arr[str_count++],"%s=%d","sampling_interval_timer",
				oam_prov_req.oam_igd_params.perf_mgmt_params.sampling_interval_timer);

	if(oam_config.config_param_bit_mask & OAM_CONFIG_PERIODIC_UPLOAD_TIME_BIT && (str_count < size_x))  
		oam_sprintf((char *)temp_arr[str_count++],"%s=%s","PeriodicUploadTime",
				oam_prov_req.oam_igd_params.perf_mgmt_params.PeriodicUploadTime);

	if(oam_config.config_param_bit_mask & OAM_CONFIG_FILE_WRITE_TIMER_BIT && (str_count < size_x))  
		oam_sprintf((char *)temp_arr[str_count++],"%s=%d","file_write_timer",
				oam_prov_req.oam_igd_params.perf_mgmt_params.file_write_timer);

	if(oam_config.config_param_bit_mask & OAM_CONFIG_PERIODIC_UPLOAD_INTERVAL_BIT && (str_count < size_x))  
		oam_sprintf((char *)temp_arr[str_count++],"%s=%d","PeriodicUploadInterval",
				oam_prov_req.oam_igd_params.perf_mgmt_params.PeriodicUploadInterval);

	oam_construct_n_send_gpv_response((UInt8 *)"InternetGatewayDevice.FAP.PerfMgmt.Config",temp_arr,str_count,
			oam_config.config_param_trans_id,&p_error_code,oam_config.config_des_module_id, OAM_ZERO, OAM_ZERO);

	/*SPR 13857 START*/
	OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, size_x, p_error_code);
	/*SPR 13857 END*/

	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_send_ConfigNumberOfEntries_gpv_resp
 * Description    : This function sends the response to GPV request
 * Inputs         : param_name - pack name
 * Outputs        :
 * Returns        : NONE
 ******************************************************************************/
	oam_return_et
oam_send_ConfigNumberOfEntries_gpv_resp(UInt16 trans_id, UInt16 dest_id)
{
	oam_error_code_et p_error_code;
	oam_counter_t loop_counter   = OAM_ZERO;
	UInt8 str_count = OAM_ZERO; 
	UInt8 **temp_arr;     /* this is the array name */
	UInt8 size_x; /* this variable will be used for the first  dimension */
	UInt8 size_y; /* this variable will be used for the second dimension */
	/* suppose we want an array of char: a[4][100] */
	size_x = OAM_ZERO;
	size_y = 255;

	OAM_FUN_ENTRY_TRACE();

	size_x = OAM_ONE;

	/*  allocate storage for an array of pointer to char */
	if (OAM_FAILURE == oam_mem_alloc((size_x * sizeof(UInt8 *)),(void *)&temp_arr,&p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to temp_arr failed"
				" with Error Code:%d",
				p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;

	}

	/* for each pocharer, allocate storage for an array of chars */
	for (loop_counter = OAM_ZERO; loop_counter < size_x; loop_counter++)
	{
		if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)&temp_arr[loop_counter],&p_error_code))
		{
			OAM_LOG(OAM, OAM_ERROR,
					"Memory allocation to temp_arr failed"
					" with Error Code:%d",
					p_error_code);
			/*SPR 13857 START*/
			OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, loop_counter, p_error_code);
			/*SPR 13857 END*/

			OAM_FUN_EXIT_TRACE();
			return OAM_FAILURE;
		}
		oam_memset(temp_arr[loop_counter], OAM_ZERO, (size_y * sizeof(UInt8)));
	}


	oam_sprintf((char *)temp_arr[str_count++],"%s=%d","ConfigNumberOfEntries",
			oam_prov_req.oam_igd_params.perf_mgmt_params.ConfigNumberOfEntries);

	oam_construct_n_send_gpv_response((UInt8 *)"InternetGatewayDevice.FAP.PerfMgmt",temp_arr,str_count,
			trans_id,&p_error_code,dest_id, OAM_ZERO, OAM_ZERO);
	/*SPR 13857 START*/
	OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr, size_x, p_error_code);
	/*SPR 13857 END*/

	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pm_rrm_send_config_req
 * Description    : This function sends rrm config request to rrm
 * Inputs         : param_name - pack name
 * Outputs        :
 * Returns        : NONE
 ******************************************************************************/
oam_return_et
oam_pm_rrm_send_config_req
(
 UInt8 cell_index, 
 /* SPR_12955_FIXES */
 UInt16 trans_id,
 /* SPR_12955_FIXES */
 oam_error_code_et *p_error_code
 )
{
	OAM_FUN_ENTRY_TRACE();
	oam_counter_t kpi_count = OAM_ZERO;

	/* Coverity 61351 */
	SInt8 cell_config_idx = OAM_ERROR_RETURN_VAL;
	cell_config_idx = oam_get_rrm_cell_config_idx(
			cell_index);
	/* Coverity_Fix */
	if(OAM_ERROR_RETURN_VAL == cell_config_idx)
	{
		OAM_LOG(OAM, OAM_INFO, "Invalid [cell_index=%d] in oam_get_rrm_cell_config_idx", cell_config_idx);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	/* Coverity_Fix */

	oam_rrm_cell_config_req_t  *p_rrm_cell_config = &oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req[cell_config_idx];


	/*SPR_10579 Start*/ /*SPR_10579 End*/ 
	oam_rrm_config_kpi_req_t *p_oam_rrm_config_kpi_req = OAM_NULL;
	/*SPR_10579 Start*/ /*SPR_10579 End*/ 

	if(OAM_FAILURE == oam_mem_alloc(sizeof(oam_rrm_config_kpi_req_t), (void*)&p_oam_rrm_config_kpi_req, p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_oam_rrm_config_kpi_req failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;

	}

	oam_memset(p_oam_rrm_config_kpi_req, OAM_ZERO, sizeof(oam_rrm_config_kpi_req_t));

   /* SPR 15491 START */
    if (oam_prov_req.oam_igd_params.perf_mgmt_params.Enable == OAM_ONE)
    {
        p_oam_rrm_config_kpi_req->periodic_reporting = g_pm_rrm_xml_params.rrm_periodic_reporting_ongoing;
    }
    else
    {
        p_oam_rrm_config_kpi_req->periodic_reporting = OAM_ZERO;
    }
    /* SPR 15491 END */

	if (g_pm_rrm_xml_params.rrm_periodic_reporting_ongoing)
	{
		p_oam_rrm_config_kpi_req->bitmask = RRM_OAM_CONFIG_KPI_DURATION_PRESENT;
		if(g_pm_rrm_xml_params.rrm_periodic_duration > RRM_KPI_DURATION_MAX)
		{
			p_oam_rrm_config_kpi_req->duration = RRM_KPI_DURATION_MAX; 
		}
		p_oam_rrm_config_kpi_req->duration = g_pm_rrm_xml_params.rrm_periodic_duration;

	}
	oam_memcpy((void*)&(p_oam_rrm_config_kpi_req->cell_id),&p_rrm_cell_config->global_cell_info.eutran_global_cell_id,
			sizeof(rrm_oam_eutran_global_cell_id_t));

	for( kpi_count=0; kpi_count < RRM_MAX_KPI - RRM_FOUR; kpi_count++)
	{
		p_oam_rrm_config_kpi_req->kpi_to_report.bitmap[kpi_count] = 0xff;
	}
	/* SPR 10010 AND 13396 Fix Start */
	p_oam_rrm_config_kpi_req->kpi_to_report.bitmap[OAM_FOUR] = OAM_THREE;
	/* SPR 10010 AND 13396 Fix End */

	/*SPR_10579 Start*/ 
	if(OAM_FAILURE == oam_rrm_send_rrm_oam_config_kpi_req(p_oam_rrm_config_kpi_req,
				OAM_ONE,
				RRM_MODULE_ID,
				/* SPR_12955_FIXES */
				trans_id,
				/* SPR_12955_FIXES */
				cell_index))
	{
		/*SPR_10579 End*/
        /*+- coverity_88034_fix*/
		OAM_LOG(OAM, OAM_ERROR, "Config Request to RRM failed");
		/* Coverity ID : 72200 Fix Start*/
		oam_mem_free(p_oam_rrm_config_kpi_req,p_error_code);
		/* Coverity ID : 72200 Fix End*/
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
    /* SPR 21077 Fixed Start */
	/* Coverity ID : 72200 Fix Start*/
	//oam_mem_free(p_oam_rrm_config_kpi_req,p_error_code);
	/* Coverity ID : 72200 Fix End*/
    /* SPR 21077 Fixed End */
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/*************************************************************************
 *  Function Name -      oam_pm_pdcp_send_config_perf_stats_req 
 *  
 *  Description -        This function sends config perf stats request to pdcp    
 *
 *  Inputs -             error_code
 *
 *  Outputs -            NONE
 *
 *  Returns -            OAM_SUCCESS/OAM_FAILURE
 ***********************************************************************************/

/* SPR 14019 FIX Start */
#ifdef PERF_STATS
/* SPR 14019 FIX End */
oam_return_et
oam_pm_pdcp_send_config_perf_stats_req
(
 oam_error_code_et *p_error_code
 )
{
	OAM_FUN_ENTRY_TRACE();
	oam_pdcp_config_perf_stats_req_t oam_pdcp_config_perf_stats_req ;

	oam_memset((void *)&oam_pdcp_config_perf_stats_req, OAM_ZERO, sizeof(oam_pdcp_config_perf_stats_req_t));


	oam_pdcp_config_perf_stats_req.duration = g_pm_xml_params.pm_xml_sampling_interval_timer;
	oam_pdcp_config_perf_stats_req.periodicReporting = g_pm_xml_params.pm_xml_file_reporting;
	oam_pdcp_config_perf_stats_req.uePerfStatsToReportBitMap = OAM_PDCP_UE_PERF_KPIBITMAP;
	oam_pdcp_config_perf_stats_req.cellPerfStatsToReportBitMap = OAM_PDCP_CELL_PERF_KPIBITMAP;

	if (OAM_FAILURE == oam_pm_pdcp_config_perf_stats_req(&oam_pdcp_config_perf_stats_req, OAM_MODULE_ID, OAM_ONE, p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in oam_pm_pdcp_config_perf_stats_req with error code %d",
				*p_error_code); /* Coverity_14DEC */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pm_pdcp_config_perf_stats_req
 * Description    : This function sends PDCP Perf stats request
 * Inputs         : 
 * Outputs        :
 * Returns        : NONE
 ******************************************************************************/
oam_return_et
oam_pm_pdcp_config_perf_stats_req
(
 oam_pdcp_config_perf_stats_req_t *p_oam_pdcp_config_perf_stats_req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length      = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg = OAM_NULL;
	UInt8 *p_pdcp_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_length = sizeof(oam_pdcp_config_perf_stats_req_t);

	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_pdcp_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_pdcp_msg, OAM_VERSION_ID, src_module_id,
			PDCP_MODULE_ID, PDCP_CONFIGURE_PERF_STATS_REQ , msg_api_length);

	/* Fill interface header */
	p_pdcp_msg = p_pdcp_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
			PDCP_MODULE_ID, PDCP_CONFIGURE_PERF_STATS_REQ, msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Incrementing the pointer to point to the structure oam_pdcp_config_perf_stats_req_t */
	p_pdcp_msg = p_pdcp_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	oam_memcpy((void *)p_pdcp_msg,p_oam_pdcp_config_perf_stats_req ,sizeof(oam_pdcp_config_perf_stats_req_t));

	/* Send message to PDCP*/
	if(OAM_FAILURE == oam_send_message(p_msg, PDCP_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending PDCP_CONFIGURE_PERF_STATS_REQ message to PDCP with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/*************************************************************************
 *  *  Function Name -      oam_pm_handle_pdcp_config_perf_stats_cnf 
 *   *  
 *    *  Description -        This function handles config perf stats cnf from pdcp    
 *     *
 *      *  Inputs -             void type pointer as buffer
 *       *
 *        *  Outputs -            NONE
 *         *
 *          *  Returns -            OAM_SUCCESS/OAM_FAILURE
 *           ***********************************************************************************/
oam_return_et
oam_pm_handle_pdcp_config_perf_stats_cnf
(
 void *p_api_buf
 )
{

	oam_pdcp_config_perf_stats_cnf_t *p_oam_pdcp_config_perf_stats_cnf = OAM_NULL;
	UInt8 cell_index = OAM_ZERO;
	OAM_FUN_ENTRY_TRACE();
	UInt16 trans_id=OAM_ZERO;
	UInt16 dest_id=OAM_ZERO;
	oam_error_code_et error_code = NO_ERROR;
	trans_id = get_word_from_header((UInt8*)(p_api_buf) +OAM_ZERO);
	dest_id = get_word_from_header((UInt8*)(p_api_buf) +OAM_TWO);

	cell_index = (UInt8)(*((UInt8*)(p_api_buf) + OAM_INTF_HDR_CELL_IDX_OFFSET));
	if (!(cell_index == oam_prov_req.cell_cntxt.curr_cell_cid) &&
			(oam_get_rrm_cell_config_idx(cell_index) == OAM_ERROR_RETURN_VAL))
	{
		error_code = (oam_error_code_et)OAM_ERR_REQUEST_DENIED;
		oam_construct_n_send_get_resp_to_usr(OAM_RESULT_FAILURE,
				OAM_ERR_REQUEST_DENIED,
				OAM_VERSION_ID, OAM_MODULE_ID,
				dest_id, trans_id, OAM_ZERO, OAM_ZERO,
				"Received Invalid Cell Index from PDCP",
				&error_code);

		OAM_LOG(OAM,OAM_INFO,"Received invalid cell_index:%d",cell_index);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	p_api_buf    = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_oam_pdcp_config_perf_stats_cnf   = (oam_pdcp_config_perf_stats_cnf_t *)p_api_buf;
	if(OAM_ZERO  == p_oam_pdcp_config_perf_stats_cnf->responseCode)
	{
		OAM_LOG(OAM,OAM_INFO,"Error in PDCP_CONFIGURE_PERF_STATS_CNF response code");
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/* SPR 14019 FIX Start */
#endif
/* SPR 14019 FIX End */
/****************************************************************************
 * Function Name  : oam_s1ap_send_stats_ind
 * Description    : This function handles S1ap stats IND
 * Inputs         : 
 * Outputs        :
 * Returns        : NONE
 ******************************************************************************/
oam_return_et
oam_s1ap_send_stats_ind
(
 UInt16     src_module_id,
 UInt16     transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_length_t msg_length = OAM_ZERO, msg_api_length = OAM_ZERO;
	UInt8 *p_msg = OAM_NULL, *p_s1apOam_msg = OAM_NULL;
	OAM_FUN_ENTRY_TRACE();
	/* Get API length */
	msg_length = OAM_ZERO;

	/* CSR 00058587 Fix Start */  
	OAM_LOG(OAM,OAM_DETAILED, "S1AP_OAM_STATUS_REQ with length %u", msg_length);
	/* CSR 00058587 Fix End */  
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if (OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		/* Not Enough Memory */
		return OAM_FAILURE;
	}

	p_s1apOam_msg = p_msg;

	/* Fill CSPL header */
	/* CSR 00058587 Fix Start */  
	oam_construct_api_header(p_s1apOam_msg, OAM_VERSION_ID, src_module_id,
			RRC_MODULE_ID, S1AP_OAM_STATUS_REQ, msg_api_length);
	/* Fill interface header */
	p_s1apOam_msg = p_s1apOam_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_s1apOam_msg, transaction_id,
			src_module_id,RRC_MODULE_ID, S1AP_OAM_STATUS_REQ, msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);
	/* CSR 00058587 Fix End */  
	/* Fill S1AP_OAM_RESET_REQ message */


	/* Send message to */
	if(OAM_FAILURE == oam_send_message(p_msg, RRC_MODULE_ID, p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in sending Message to RRC with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/* LWA changes start*/
/* LWA changes end*/

/*SPR 10023 FIX start*/
/* SPR 14019 FIX Start */
#ifdef PERF_STATS
/* SPR 14019 FIX End */
/***********************************************************************************
 * Function Name -      delete_pdcp_perf_stats_data_from_link_list
 * Description -        This function deletes the PDCP_GET_UE_PERF_STATS_CNF data from
 *						link list 
 * Inputs -             NONE
 * Outputs -            NONE
 * Returns -            NONE
 ************************************************************************************/
	void
delete_pdcp_perf_stats_data_from_link_list(
/* + SPR 17439 */
 void
)
/* - SPR 17439 */
{
	oam_pdcp_ue_perf_stats_batch_node *temp = OAM_NULL;
	OAM_FUN_ENTRY_TRACE();
	temp = ConsolidatedPdcpUePerfStats;
	oam_error_code_et error_code = NO_ERROR;
	while (ConsolidatedPdcpUePerfStats != OAM_NULL)
	{
		if(ConsolidatedPdcpUePerfStats->next != OAM_NULL)
		{
			temp = ConsolidatedPdcpUePerfStats;
			ConsolidatedPdcpUePerfStats = ConsolidatedPdcpUePerfStats->next;
			oam_mem_free((void *)temp,&error_code);
			temp = OAM_NULL;
		}
		else
		{
			oam_mem_free((void *)ConsolidatedPdcpUePerfStats,&error_code);
			ConsolidatedPdcpUePerfStats = OAM_NULL;
			temp = OAM_NULL;
		}
	}
	OAM_FUN_EXIT_TRACE();
}

/* SPR 14019 FIX Start */
#endif
/* SPR 14019 FIX End */
/***********************************************************************************
 * Function Name -      delete_pdcp_batch_data_from_link_list
 * Description -        This function deletes the PDCP_KPI_THP_STATS_IND data from
 *								link list 
 * Inputs -             api_id- API id
 *								data 
 * Outputs -            NONE
 * Returns -            NONE
 ************************************************************************************/
	void
delete_pdcp_batch_data_from_link_list(SInt32 api_id)
{
	OAM_FUN_ENTRY_TRACE();
	oam_pdcp_thp_batch_node *temp = OAM_NULL;
	oam_pdcp_thp_batch_node *curr_head = OAM_NULL;
	oam_error_code_et error_code = NO_ERROR;
	if (api_id == PDCP_KPI_THP_STATS_IND)
	{
		temp = ConsolidatedPdcpThpStatsInd;
		curr_head = ConsolidatedPdcpThpStatsInd;
	}
	else if(api_id == PDCP_GET_KPI_THP_STATS_RESP)
	{
		temp = ConsolidatedGetPdcpThpStats;
		curr_head = ConsolidatedGetPdcpThpStats;
	}
	else
	{
		OAM_LOG(OAM,OAM_WARNING,"Received invalid api_id = %d",api_id);
		return;
	}
	while (temp != OAM_NULL)
	{
		if(temp->next != OAM_NULL)
		{
			temp = curr_head;
			curr_head = curr_head->next;
			if (OAM_FAILURE == oam_mem_free((void *)temp,&error_code))
			{
				OAM_LOG(OAM,OAM_ERROR,"Error in freeing memory error code %d",
						error_code);
				OAM_FUN_EXIT_TRACE();
				return ;
			}
			temp = OAM_NULL;
		}
		else
		{
			if (OAM_FAILURE == oam_mem_free((void *)curr_head,&error_code))
			{
				OAM_LOG(OAM,OAM_ERROR,"Error in freeing memory error code %d",
						error_code);
				OAM_FUN_EXIT_TRACE();
				return ;
			}
			curr_head = OAM_NULL;
			temp = OAM_NULL;
		}
	}
	/*SPR 10023 FIX */
	if(api_id == PDCP_GET_KPI_THP_STATS_RESP)
	{
		ConsolidatedGetPdcpThpStats = OAM_NULL;
	}
	else if (api_id == PDCP_KPI_THP_STATS_IND)
	{
		ConsolidatedPdcpThpStatsInd = OAM_NULL;
	}
	/*SPR 10023 FIX */
	OAM_FUN_EXIT_TRACE();
}

/*spr 19296 fix start*/

/***********************************************************************************
 * Function Name -      delete_mac_batch_data_from_link_list
 * Description -        This function deletes the MAC_KPI_THP_STATS_IND data from
 *								link list 
 * Inputs -             api_id- API id
 *								data 
 * Outputs -            NONE
 * Returns -            NONE
 ************************************************************************************/
void
delete_mac_batch_data_from_link_list(SInt32 api_id)
{
	OAM_FUN_ENTRY_TRACE();
	oam_mac_thp_batch_node *temp = OAM_NULL;
	oam_mac_thp_batch_node *curr_head = OAM_NULL;
	oam_error_code_et error_code = NO_ERROR;
	if (api_id == MAC_KPI_THP_STATS_IND)
	{
		temp = ConsolidatedMacThpStatsInd;
		curr_head = ConsolidatedMacThpStatsInd;
	}
	else if(api_id == MAC_GET_KPI_THP_STATS_CNF)
	{
		temp = ConsolidatedGetMacThpStats;
		curr_head = ConsolidatedGetMacThpStats;
	}
	else
	{
		OAM_LOG(OAM,OAM_WARNING,"Received invalid api_id = %d",api_id);
		return;
	}
	while (temp != OAM_NULL)
	{
		if(temp->next != OAM_NULL)
		{
			temp = curr_head;
			curr_head = curr_head->next;
			if (OAM_FAILURE == oam_mem_free((void *)temp,&error_code))
			{
				OAM_LOG(OAM,OAM_ERROR,"Error in freeing memory error code %d",
						error_code);
				OAM_FUN_EXIT_TRACE();
				return ;
			}
			temp = OAM_NULL;
		}
		else
		{
			if (OAM_FAILURE == oam_mem_free((void *)curr_head,&error_code))
			{
				OAM_LOG(OAM,OAM_ERROR,"Error in freeing memory error code %d",
						error_code);
				OAM_FUN_EXIT_TRACE();
				return ;
			}
			curr_head = OAM_NULL;
			temp = OAM_NULL;
		}
	}
	if(api_id == MAC_GET_KPI_THP_STATS_CNF)
	{
		ConsolidatedGetMacThpStats = OAM_NULL;
	}
	else if (api_id == MAC_KPI_THP_STATS_IND)
	{
		ConsolidatedMacThpStatsInd = OAM_NULL;
	}
	OAM_FUN_EXIT_TRACE();
}
/*spr 19296 fix end*/

/* SPR 14019 FIX Start */
#ifdef PERF_STATS
/* SPR 14019 FIX End */


/***********************************************************************************
 * Function Name -      add_to_get_ue_pdcp_perf_stats_batch_link_list
 * Description -        This function adds the PDCP_GET_UE_PERF_STATS_CNF data to 
 *								link list 
 * Inputs -             oam_pdcp_get_ue_perf_stats_cnf_t - pointer to 
 *								PDCP_GET_UE_PERF_STATS_CNF
 *								data 
 *								error_code - pointer to oam_error_code_et
 * Outputs -            NONE
 * Returns -            NONE
 ************************************************************************************/
void
add_to_get_ue_pdcp_perf_stats_batch_link_list
(
 oam_pdcp_get_ue_perf_stats_cnf_t *p_oam_ue_pdcp_ue_perf_stats,
 oam_error_code_et *p_error_code
 )
{
	OAM_FUN_ENTRY_TRACE();
	/*
	 * DESCRIPTION: Here function creates singly link list with node containing of type
	 * "oam_pdcp_get_ue_perf_stats_cnf_t".
	 */
	oam_pdcp_ue_perf_stats_batch_node *temp = OAM_NULL;
	/*SPR 10023 FIX start: removed code*/
	oam_pdcp_ue_perf_stats_batch_node *current = OAM_NULL;
	if (OAM_FAILURE == oam_mem_alloc(sizeof(oam_pdcp_ue_perf_stats_batch_node),(void *)&temp,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to temp failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return ;
	}
	oam_memcpy(&temp->data, p_oam_ue_pdcp_ue_perf_stats,sizeof(oam_pdcp_get_ue_perf_stats_cnf_t));
	temp->next = OAM_NULL;
	if(ConsolidatedPdcpUePerfStats== OAM_NULL)
	{
		ConsolidatedPdcpUePerfStats = temp;
	}
	else
	{
		current = ConsolidatedPdcpUePerfStats;
		while(current->next != OAM_NULL)
		{
			current = current->next;
		}
		current->next = temp;
	}
}
/* SPR 14019 FIX Start */
#endif
/* SPR 14019 FIX End */

/**************************************************************************************
 * Function Name -      add_to_get_pdcp_thp_batch_link_list
 * Description -        This function adds the PDCP_GET_KPI_THP_STATS_CNF data to 
 *			link list 
 * Inputs -             p_oam_pdcp_kpi_thp_stats_ind - pointer to PDCP_GET_KPI_THP_STATS
 *			data 
 *			error_code - pointer to oam_error_code_et
 * Outputs -            NONE
 * Returns -            NONE
 **************************************************************************************/
void
add_to_get_pdcp_thp_batch_link_list
(
 oam_pdcp_KPI_THP_stats_ind_t *p_oam_pdcp_kpi_thp_stats_ind,
 oam_error_code_et *p_error_code
 )
{
	OAM_FUN_ENTRY_TRACE();
	oam_pdcp_thp_batch_node *temp = OAM_NULL;
	/*SPR 10023 FIX start: removed code*/
	oam_pdcp_thp_batch_node *current = OAM_NULL;

	/*
	 * DESCRIPTION: Here function creates singly link list with node containing of type
	 * "oam_pdcp_KPI_THP_stats_ind_t".
	 */

	if (OAM_FAILURE == oam_mem_alloc(sizeof(oam_pdcp_thp_batch_node),(void *)&temp,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to temp failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return ;
	}
	oam_memcpy(&temp->data, p_oam_pdcp_kpi_thp_stats_ind,sizeof(oam_pdcp_KPI_THP_stats_ind_t));
	temp->next = OAM_NULL;
	if(ConsolidatedGetPdcpThpStats == OAM_NULL)
	{
		ConsolidatedGetPdcpThpStats = temp;
	}
	else
	{
		current = ConsolidatedGetPdcpThpStats;
		while(current->next != OAM_NULL)
		{
			current = current->next;
		}
		current->next = temp;
	}
}

/***********************************************************************************
 * Function Name -      add_to_pdcp_thp_batch_link_list
 * Description -        This function adds the PDCP_KPI_THP_STATS_IND data to link list
 * Inputs -             p_oam_pdcp_kpi_thp_stats_ind - pointer to PDCP_KPI_THP_STATS_IND
 *								data 
 *								error_code - pointer to oam_error_code_et
 * Outputs -            NONE
 * Returns -            NONE
 ************************************************************************************/
void
add_to_pdcp_thp_batch_link_list
(
 oam_pdcp_KPI_THP_stats_ind_t *p_oam_pdcp_kpi_thp_stats_ind,
 /* SPR 22296 Fix Start */
 oam_error_code_et *p_error_code,
 UInt8 cell_id
 /* SPR 22296 Fix End */
 )
{
	OAM_FUN_ENTRY_TRACE();
	oam_pdcp_thp_batch_node *temp = OAM_NULL;
	/*SPR 10023 : removed code*/
	oam_pdcp_thp_batch_node *current = OAM_NULL;

	/*
	 * DESCRIPTION: Here function creates singly link list with node containing of type
	 * "oam_pdcp_KPI_THP_stats_ind_t".
	 */

	if (OAM_FAILURE == oam_mem_alloc(sizeof(oam_pdcp_thp_batch_node),(void *)&temp,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to temp failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return ;
	}
	oam_memcpy(&temp->data, p_oam_pdcp_kpi_thp_stats_ind,sizeof(oam_pdcp_KPI_THP_stats_ind_t));
	temp->next = OAM_NULL;
	/* SPR 22296 Fix Start */
	temp->cell_idx = cell_id;
	/* SPR 22296 Fix End */
	if(ConsolidatedPdcpThpStatsInd == OAM_NULL)
	{
		ConsolidatedPdcpThpStatsInd = temp;
	}
	else
	{
		current = ConsolidatedPdcpThpStatsInd;
		while(current->next != OAM_NULL)
		{
			current = current->next;
		}
		current->next = temp;
	}
}
/***********************************************************************************
 * Function Name -      oam_handle_pdcp_thp_batch_data
 * Description -        This function handles the batches of PDCP_KPI_THP_STATS_IND
 *			and adds the data into link list
 * Inputs -             p_api_buf  - void type pointer as buffer containing 
 *			PDCP_KPI_THP_STATS_IND data.
 *			error_code - pointer to oam_error_code_et
 * Outputs -            NONE
 * Returns -            OAM_SUCCESS/OAM_FAILURE
 ************************************************************************************/
oam_return_et
oam_handle_pdcp_thp_batch_data
(
 void *p_api_buf, 
 /* SPR 22296 Fix Start */
 oam_error_code_et *p_error_code,
 UInt8 cell_id
 /* SPR 22296 Fix End */
 )
{
	oam_pdcp_KPI_THP_stats_ind_t *p_oam_pdcp_kpi_thp_stats_ind = OAM_NULL;
	oam_return_et ret_val = OAM_FAILURE;
	OAM_FUN_ENTRY_TRACE();

	p_api_buf                    = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_oam_pdcp_kpi_thp_stats_ind = (oam_pdcp_KPI_THP_stats_ind_t *)p_api_buf;
	/* SPR 22296 Fix Start */
	add_to_pdcp_thp_batch_link_list(p_oam_pdcp_kpi_thp_stats_ind, p_error_code, cell_id);
	/* SPR 22296 Fix End */
	if(p_oam_pdcp_kpi_thp_stats_ind->endIndicator == OAM_ONE)
	{
		OAM_LOG(OAM, OAM_INFO, "Received end indicator value %u",
				p_oam_pdcp_kpi_thp_stats_ind->endIndicator);
		ret_val = OAM_SUCCESS;

	}
	else
	{
		OAM_LOG(OAM, OAM_INFO, "Received end indicator value %u",
				p_oam_pdcp_kpi_thp_stats_ind->endIndicator);
	}
	OAM_FUN_EXIT_TRACE();
	return ret_val;
}

/***********************************************************************************
 * Function Name -      oam_handle_get_pdcp_thp_batch_data
 * Description -        This function handles the batches of PDCP_KPI_THP_STATS_IND
 *			and adds the data into link list
 * Inputs -             p_api_buf  - void type pointer as buffer containing 
 *			PDCP_KPI_THP_STATS_IND data.
 *			error_code - pointer to oam_error_code_et
 * Outputs -            NONE
 * Returns -            OAM_SUCCESS/OAM_FAILURE
 ************************************************************************************/
oam_return_et
oam_handle_get_pdcp_thp_batch_data
(
 void *p_api_buf, 
 oam_error_code_et *p_error_code
 )
{
	oam_pdcp_KPI_THP_stats_ind_t *p_oam_pdcp_kpi_thp_stats_ind = OAM_NULL;
	oam_return_et ret_val = OAM_FAILURE;
	OAM_FUN_ENTRY_TRACE();

	p_api_buf                    = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_oam_pdcp_kpi_thp_stats_ind = (oam_pdcp_KPI_THP_stats_ind_t *)p_api_buf;
	add_to_get_pdcp_thp_batch_link_list(p_oam_pdcp_kpi_thp_stats_ind, p_error_code);
	if(p_oam_pdcp_kpi_thp_stats_ind->endIndicator == OAM_ONE)
	{
		OAM_LOG(OAM, OAM_INFO, "Received end indicator value %u",
				p_oam_pdcp_kpi_thp_stats_ind->endIndicator);
		ret_val = OAM_SUCCESS;

	}
	else
	{
		OAM_LOG(OAM, OAM_INFO, "Received end indicator value %u",
				p_oam_pdcp_kpi_thp_stats_ind->endIndicator);
	}
	OAM_FUN_EXIT_TRACE();
	return ret_val;
}
/* SPR 14019 FIX Start */
#ifdef PERF_STATS
/* SPR 14019 FIX End */
/**************************************************************************************
 * Function Name -      oam_handle_get_ue_pdcp_perf_stats_batch_data
 * Description -        This function handles the batches of PDCP_GET_KPI_THP_STATS_CNF 
 *								and adds the data into link list
 * Inputs -             p_api_buf  - void type pointer as buffer containing 
 *								PDCP_GET_KPI_THP_STATS_CNF data.
 *								error_code - pointer to oam_error_code_et
 * Outputs -            NONE
 * Returns -            OAM_SUCCESS/OAM_FAILURE
 **************************************************************************************/
oam_return_et
oam_handle_get_ue_pdcp_perf_stats_batch_data
(
 void *p_api_buf, 
 oam_error_code_et *p_error_code,
 SInt8 *UE_count
 )
{
	oam_pdcp_get_ue_perf_stats_cnf_t *oam_pdcp_ue_perf_stats = OAM_NULL;
	oam_return_et ret_val = OAM_FAILURE;
	OAM_FUN_ENTRY_TRACE();

	p_api_buf  = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	oam_pdcp_ue_perf_stats = (oam_pdcp_get_ue_perf_stats_cnf_t *)p_api_buf;
	add_to_get_ue_pdcp_perf_stats_batch_link_list(oam_pdcp_ue_perf_stats, p_error_code);
	*UE_count += oam_pdcp_ue_perf_stats->numOfUePresent;
	if(oam_pdcp_ue_perf_stats->endIndicator == OAM_ONE)
	{
		OAM_LOG(OAM, OAM_INFO, "Received end indicator value %u",
				oam_pdcp_ue_perf_stats->endIndicator);
		ret_val = OAM_SUCCESS;

	}
	else
	{
		OAM_LOG(OAM, OAM_INFO, "Received end indicator value %u",
				oam_pdcp_ue_perf_stats->endIndicator);
	}
	OAM_FUN_EXIT_TRACE();
	return ret_val;
}
/* SPR 14019 FIX Start */
#endif
/* SPR 14019 FIX End */
/*SPR 10023 FIX end*/
/****************************************************************************
 * Function Name  : oam_send_pdcp_reset_config_request
 * Inputs         : 
 * Outputs        : 
 * Returns        : OAM_SUCCESS
 * Description    :
 ****************************************************************************/
/*SPR 11120 FIX start*/
	void
oam_pm_stop_all_timer(
/* + SPR 17439 */
 void
)
/* - SPR 17439 */
{
	if(OAM_TRUE == g_pm_xml_params.periodic_reporting_ongoing)
	{
		//check if the disabled req has been received
		if (OAM_ZERO == g_pm_xml_params.pm_xml_file_reporting)
		{
			//close the current file and stop upload interval timer and the file write timer
			if(*(int*)g_pm_xml_params.pm_xml_upload_interval_timer_id > 0)
			{
				oam_pm_timer_stop(g_pm_xml_params.pm_xml_upload_interval_timer_id);
				g_pm_xml_params.pm_xml_upload_interval_timer_id = 0;
			}
			if (*(int*)g_pm_xml_params.pm_xml_upload_timer_id > 0)
			{
				oam_pm_timer_stop(g_pm_xml_params.pm_xml_upload_timer_id);
				g_pm_xml_params.pm_xml_upload_timer_id = 0;
			}
			if (*(int*)g_pm_xml_params.pm_xml_file_write_timer_id > 0)
			{
				oam_pm_timer_stop(g_pm_xml_params.pm_xml_file_write_timer_id);
				g_pm_xml_params.pm_xml_file_write_timer_id = 0;
			}
		}
	}
}

/* SPR 15018 START */
    void
oam_write_xml_measInfo(UInt8 *endTime)
{
    char num_buffer[NUM_BUFF] = {OAM_ZERO};
    char **value_in_pm_xml_param_name = pm_xml_conn_st_param_name;
    OAM_FUN_ENTRY_TRACE();
    OAM_LOG(OAM,OAM_DETAILED,"in funct oam_write_xml_measInfo");

    xmlTextWriterStartElement(writer, BAD_CAST (xmlChar*)"measInfo");

    oam_memset(&num_buffer, OAM_ZERO, sizeof(num_buffer));

    xmlTextWriterStartElement(writer, BAD_CAST "granPeriod");
    oam_snprintf(num_buffer, NUM_BUFF,"PT%dS", g_pm_xml_params.pm_xml_sampling_interval_timer);

    xmlTextWriterWriteAttribute(writer, BAD_CAST "duration",(xmlChar*)num_buffer);

    /* timer contains the current time, increment by the time till which we plan to
     *      * collect stats and enter the file
     *           */

    xmlTextWriterWriteAttribute(writer, BAD_CAST (xmlChar*)"endtime",(xmlChar*)(char *)endTime);
    xmlTextWriterEndElement(writer);

    xmlTextWriterStartElement(writer, BAD_CAST (xmlChar*)"repPeriod");
    oam_snprintf(num_buffer, NUM_BUFF,"PT%dS", g_pm_xml_params.pm_xml_upload_interval_timer);
    xmlTextWriterWriteAttribute(writer, BAD_CAST (xmlChar*)"duration",(xmlChar *)num_buffer);
    xmlTextWriterEndElement(writer);


    value_in_pm_xml_param_name = pm_xml_conn_st_param_name;
    oam_pm_write_xml_param_name(writer,value_in_pm_xml_param_name);

    value_in_pm_xml_param_name = pm_xml_erab_param_name;
    oam_pm_write_xml_param_name(writer,value_in_pm_xml_param_name);

    value_in_pm_xml_param_name = pm_xml_s1sig_param_name;
    oam_pm_write_xml_param_name(writer,value_in_pm_xml_param_name);

    value_in_pm_xml_param_name = pm_xml_paging_param_name;
    oam_pm_write_xml_param_name(writer,value_in_pm_xml_param_name);

    value_in_pm_xml_param_name = pm_xml_ho_param_name;
    oam_pm_write_xml_param_name(writer,value_in_pm_xml_param_name);
/*SPR 21866 Start*/
    /* SPR 14504 FIX START */
    /* SPR 21719 FIX START */
    /* Code Remove */
    /* SPR 21719 FIX END */
    /* code removed */
    /* SPR 14504 FIX END */
/*SPR 21866 End*/
/* SPR 19351 Code Removed */
    value_in_pm_xml_param_name = pm_xml_mac_param_name;
    oam_pm_write_xml_param_name(writer,value_in_pm_xml_param_name);

    value_in_pm_xml_param_name = pm_xml_pdcp_param_name;
    oam_pm_write_xml_param_name(writer,value_in_pm_xml_param_name);

    value_in_pm_xml_param_name = pm_xml_ho_target_param_name;
    oam_pm_write_xml_param_name(writer,value_in_pm_xml_param_name);

    pm_total_elements_in_xml = OAM_ONE;

    OAM_LOG(OAM,OAM_DETAILED,"Total Elements:::%d", pm_total_elements_in_xml);
    OAM_FUN_EXIT_TRACE();

}


void
oam_calculate_endTime(UInt8 *endTime)
{
    char num_buffer[NUM_BUFF] = {OAM_ZERO};
    time_t timer = OAM_ZERO;
    UInt8 tmpbuf2[100] = {OAM_ZERO};
    struct tm* tm_info;
    struct tm tm_info_tmp;
    char offset[10];
    char mod_offset[10];
    char des_str[9] = {OAM_ZERO};

    OAM_FUN_ENTRY_TRACE();
    oam_memset(endTime, OAM_ZERO, 100);
    oam_memset(&num_buffer, OAM_ZERO, sizeof(num_buffer));
    oam_snprintf(num_buffer,NUM_BUFF,"PT%dS",g_pm_xml_params.pm_xml_upload_interval_timer);

    //timer contains the current time, increment by the time till which we plan to collect stats and enter the file

    oam_time(&timer);
    tm_info =oam_gmtime(&timer);
    if (tm_info == NULL)
    {
        return;
    }
    tm_info_tmp = *tm_info;
    oam_sprintf((char*)tmpbuf2,"%d-%d-%dT",tm_info->tm_year + (1900),tm_info->tm_mon + 1,tm_info->tm_mday);
    strftime(offset, 10, "%z", &tm_info_tmp);
    oam_sprintf(mod_offset,"%c%c%c:%c%c",offset[0],offset[1],offset[2],offset[3],offset[4]);
    if (oam_ctime(&timer) == NULL)
    {
        return;
    }
    oam_parse_time_from_string(oam_ctime(&timer),des_str);
    oam_strcat(tmpbuf2,des_str);
    OAM_LOG(OAM, OAM_INFO, "des_str = %s",des_str);

    strftime(offset, 10, "%z", tm_info);
    oam_sprintf(mod_offset,"%c%c%c:%c%c",offset[0],offset[1],offset[2],offset[3],offset[4]);
    oam_strcat((char *)tmpbuf2,mod_offset);
    oam_memcpy((void *)endTime, (void *)tmpbuf2, oam_strlen((char *)tmpbuf2));
    OAM_LOG(OAM, OAM_DETAILED, "endTime = %s", endTime);

    OAM_FUN_EXIT_TRACE();
}
/* SPR 15018 END */


/* SPR_12747_CHANGES */
/****************************************************************************
 * Function Name  : oam_pm_write_ho_target_param 
 * Inputs         : oam_pm_rrc_kpi_info_Node_t type pointer
 * Outputs        : NONE 
 * Returns        : NONE
 * Description    : This function read out HO IRAT KPI's received from RRC
 ****************************************************************************/
void oam_pm_write_ho_target_param
(
    oam_pm_rrc_kpi_info_Node_t *oam_pm_rrc_kpi_node,
    /* SPR 22296 Fix Start */
    oam_counter_t loop_counter,
    UInt8 rrc_cell_id
    /* SPR 22296 Fix End */
)
{
	UInt16 index = OAM_ZERO;
	UInt16 meas_count = OAM_ZERO;
	UInt16 meas_str_count = OAM_ZERO;
    UInt16  meas_target_count = OAM_ZERO;
    UInt16 ho_irat_count = OAM_ZERO;
    oam_counter_t target_counter = OAM_ZERO;
    /*SPR 21675 Start*/
    /*SPR 22537 Fix Start*/
    /*Code Removed*/
    /*SPR 22537 Fix Start*/
	UInt8 temp_buff[STR_LEN_256] = {OAM_ZERO};
	/*SPR 21675 End*/
	oam_error_code_et error_code = NO_ERROR;
	OAM_FUN_ENTRY_TRACE();
	UInt16 rrc_max_target = OAM_ZERO;
	UInt32 max_ho_target_count = OAM_ZERO;	 /*SPR 19596 */
	/* SPR 14792 Changes Start */
	/* Allocate mem for pm_xml_ho_target_param_name 
	 * for HO target KPI counter, which is re-used 
	 * for updation of HO meas KPI counters */
	/* SPR 14792 Changes End */
	/* Coverity CID 116216 Fix Start */
	/*SPR 22367 Fix Start*/
	UInt8  **pm_xml_ho_param_value = OAM_NULL;	/*SPR 19596*/
	/*SPR 22367 Fix End*/
	/* Coverity CID 116216 Fix End */
	/* SPR 17176 changes start */
	oam_sprintf((char *)temp_target_str,"%s","0000");
	/* SPR 17176 changes end */
	/*SPR 22367 Fix Start*/
	/*In max_ho_target_count, 2 is added for counters ho_IratOutAtt_sum and
	 * ho_IratOutSucc_sum and 1 is subtracted for 
	 * s1ap_unspecified_2 which occurs only once for HO.IartOut attributes
	 * So effectively only 1 is added*/
	UInt16  size_of_arr = s1ap_CauseProtocol_Root_max +
		s1ap_relReq_cause_nas_max + oam_s1ap_CauseMisc_Root_max +
		s1ap_CauseTransport_Root_max + oam_s1ap_CauseRadioNetwork_Root_max;

	max_ho_target_count = (size_of_arr * OAM_TWO) + OAM_ONE;

	/* Coverity CID 116216 Fix Start */
	if(OAM_FAILURE == (UInt8**) oam_mem_alloc((OAM_PM_TOTAL_MEAS_KPI_COUNT * sizeof(UInt8*)),(void *)&pm_xml_ho_param_value, &error_code))
	{
        OAM_LOG(OAM,OAM_ERROR,"unable to allocate memory!!%d", error_code);
        return;
	}
	/* Coverity CID 116216 Fix End */
	/* SPR 22296 Fix Start */
	if (oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].cell_index == rrc_cell_id)
    {
        /* SPR 22296 Fix End */
        for(index = OAM_ZERO; index < OAM_PM_TOTAL_MEAS_KPI_COUNT; index++)
        /*SPR 22367 Fix End*/
        {
            if(OAM_FAILURE ==  oam_mem_alloc(OAM_TWENTY_FIVE ,(void *)&(pm_xml_ho_param_value[index]), &error_code))
            {
                OAM_LOG(OAM, OAM_ERROR,
                        "Memory allocation to pm_xml_ho_param_value failed"
                        " with Error Code:%d",
                        error_code);
                /* Coverity CID 116216 Fix Start */
                OAM_PM_2D_TEMP_ARR_MEM_FREE(pm_xml_ho_param_value, index, error_code);
                /* Coverity CID 116216 Fix End */
                return;
            }
            else
            {
                /* SPR 17176 changes start */
                oam_sprintf((char *)pm_xml_ho_param_value[index], "NULL");
                /* SPR 17176 changes end */
            }
        }

        if (oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].bitmask
                & RRC_OAM_INTER_RAT_HO_IND_STATS_PRESENT)
        {
            rrc_max_target = oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                inter_rat_ho_stats_list.count;
            /* SPR 19257 */  
            if(rrc_max_target > RRC_MAX_CONNECTION)
            {
                OAM_LOG(OAM, OAM_WARNING, "rrc_max_target(%d) > RRC_MAX_CONNECTION (%d ) making rrc_max_target = 1 as it is ERROR condition",
                        rrc_max_target, RRC_MAX_CONNECTION);	
                rrc_max_target = 1;
            }
            /* SPR 19257 */ 
            for(target_counter = OAM_ZERO; target_counter < rrc_max_target; target_counter++)
            {
                oam_memset((void *)temp_target_str, OAM_ZERO, 7);
                fill_plmn_id_from_mcc_mnc(temp_target_str,oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].
                        /* SPR 19166 Start */
                        target_cgi.cgi_info.plmn_identity);
                /* SPR 19166 Stop */
                OAM_LOG(OAM, OAM_DETAILED, "reading inter_rat_ho_stat: count: %d", target_counter);
                /* SPR 19257 */ 
                ho_irat_count = OAM_ZERO;
                /* SPR 19257 */ 

                //HO.IartOutAtt.Tx2RelocOverallExpiry
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_tx2relocoverall_expiry]);
                //HO.IartOutAtt.SuccessfulHandover
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_successful_handover]);
                //HO.IartOutAtt.ReleaseDueToEutranGeneratedReason
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_release_due_to_eutran_generated_reason]);
                //HO.IartOutAtt.HandoverCancelled
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_handover_cancelled]);
                //HO.IartOutAtt.PartialHandover
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_partial_handover]);
                //HO.IartOutAtt.HoFailureInTargetEPCeNBOrTargetSystem
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_ho_failure_in_target_EPC_eNB_or_target_system]);
                //HO.IartOutAtt.HoTargetNotAllowed
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_ho_target_not_allowed]);
                //HO.IartOutAtt.TS1RelocOverallExpiry
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_tS1relocoverall_expiry]);
                //HO.IartOutAtt.TS1RelocPrepExpiry
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_tS1relocprep_expiry]);
                //HO.IartOutAtt.CellNotAvailable
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_cell_not_available]);
                //HO.IartOutAtt.UnknownTargetID
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_unknown_targetID]);
                //HO.IartOutAtt.NoRadioResourcesAvailableInTargetCell
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_no_radio_resources_available_in_target_cell]);
                //HO.IartOutAtt.UnknownMmeUeS1apId
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_unknown_mme_ue_s1ap_id]);
                //HO.IartOutAtt.UnknownEnbUeS1apId
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_unknown_enb_ue_s1ap_id]);
                //HO.IartOutAtt.UnknownPairUeS1apId
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_unknown_pair_ue_s1ap_id]);
                //HO.IartOutAtt.HandOverDesirableForRadioReason
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_handover_desirable_for_radio_reason]);
                //HO.IartOutAtt.TimeCriticalHandover
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_time_critical_handover]);
                //HO.IartOutAtt.ResourceOptimisationHandover
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_resource_optimisation_handover]);
                //HO.IartOutAtt.ReduceLoadInServingCell
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_reduce_load_in_serving_cell]);
                //HO.IartOutAtt.UserInactivity
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_user_inactivity]);
                //HO.IartOutAtt.RadioConnectionWithUeLost
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_radio_connection_with_ue_lost]);
                //HO.IartOutAtt.LoadBalancingTauRequired
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_load_balancing_tau_required]);
                //HO.IartOutAtt.CsFallbackTriggered
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_cs_fallback_triggered]);
                //HO.IartOutAtt.UeNotAvailableForPsService
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_ue_not_available_for_ps_service]);
                //HO.IartOutAtt.RadioResourcesNotAvailable
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_radio_resources_not_available]);
                //HO.IartOutAtt.FailureInRadioInterfaceProcedure
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_failure_in_radio_interface_procedure]);
                //HO.IartOutAtt.InvalidQosCombination
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_invalid_qos_combination]);
                //HO.IartOutAtt.InterratRedirection
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_interrat_redirection]);
                //HO.IartOutAtt.InteractionWithOtherProcedure
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_interaction_with_other_procedure]);
                //HO.IartOutAtt.UnknownERABID
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_unknown_E_RAB_ID]);
                //HO.IartOutAtt.MultipleERABIDInstances
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_multiple_E_RAB_ID_instances]);
                //HO.IartOutAtt.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_encryption_and_or_integrity_protection_algorithms_not_supported]);
                //HO.IartOutAtt.S1IntraSystemHandoverTriggered
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_s1_intra_system_handover_triggered]);
                //HO.IartOutAtt.S1InterSystemHandoverTriggered
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_s1_inter_system_handover_triggered]);
                //HO.IartOutAtt.X2HOTriggered
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_radioNetwork[s1ap_x2_handover_triggered]);
                //HO.IartOutAtt.TransportResourceUnavailable
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_transport[s1ap_transport_resource_unavailable]);
                //HO.IartOutAtt.Unspecified_3
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_transport[s1ap_unspecified_3]);
                //HO.IartOutAtt.NormalRelease
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_nas[normal_release]);
                //HO.IartOutAtt.AuthenticationFailure
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_nas[authentication_failure]);
                //HO.IartOutAtt.Detach
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_nas[detach]);
                //HO.IartOutAtt.Unspecified_4
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_nas[unspecified_4]);
                //HO.IartOutAtt.TransferSyntaxError
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_protocol[transfer_syntax_error]);
                //HO.IartOutAtt.AbstractSyntaxErrorReject
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_protocol[abstract_syntax_error_reject]);
                //HO.IartOutAtt.AbstractSyntaxErrorIgnoreAndNotify
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_protocol[abstract_syntax_error_ignore_and_notify]);
                //HO.IartOutAtt.MessageNotCompatibleWithReceiverState
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_protocol[message_not_compatible_with_receiver_state]);
                //HO.IartOutAtt.SemanticError
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_protocol[semantic_error]);
                //HO.IartOutAtt.AbstractSyntaxErrorFalselyConstructedMessage
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_protocol[abstract_syntax_error_falsely_constructed_message]);
                //HO.IartOutAtt.Unspecified_1
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_protocol[unspecified_1]);
                /*SPR 19596 Code Deleted*/

                //HO.IartOutAtt.ControlProcessingOverload
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_misc[s1ap_control_processing_overload]);
                //HO.IartOutAtt.NotEnoughUserPlaneProcessingResources
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_misc[s1ap_not_enough_user_plane_processing_resources]);
                /*SPR 19596 Start*/
                //HO.IartOutAtt.HardwareFailure
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_misc[s1ap_hardware_failure]);
                //HO.IartOutAtt.OmIntervention
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_misc[s1ap_om_intervention]);
                /*SPR 19596 End*/
                //HO.IartOutAtt.Unspecified
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_misc[s1ap_unspecified]);
                //HO.IartOutAtt.UnknownPLMN
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_cause.ho_IratOutAtt_Cause_misc[s1ap_unknown_PLMN]);
                //HO.IartOutAtt.Sum
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutAtt_sum);
                //HO.IartOutSucc.Unspecified_2
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_unspecified_2]);
                //HO.IartOutSucc.Tx2RelocOverallExpiry
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_tx2relocoverall_expiry]);
                //HO.IartOutSucc.SuccessfulHandover
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_successful_handover]);
                //HO.IartOutSucc.ReleaseDueToEutranGeneratedReason
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_release_due_to_eutran_generated_reason]);
                //HO.IartOutSucc.HandoverCancelled
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_handover_cancelled]);
                //HO.IartOutSucc.PartialHandover
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_partial_handover]);
                //HO.IartOutSucc.HoFailureInTargetEPCeNBOrTargetSystem
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_ho_failure_in_target_EPC_eNB_or_target_system]);
                //HO.IartOutSucc.HoTargetNotAllowed
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_ho_target_not_allowed]);
                //HO.IartOutSucc.TS1RelocOverallExpiry
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_tS1relocoverall_expiry]);
                //HO.IartOutSucc.TS1RelocPrepExpiry
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_tS1relocprep_expiry]);
                //HO.IartOutSucc.CellNotAvailable
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_cell_not_available]);
                //HO.IartOutSucc.UnknownTargetID
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_unknown_targetID]);
                //HO.IartOutSucc.NoRadioResourcesAvailableInTargetCell
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_no_radio_resources_available_in_target_cell]);
                //HO.IartOutSucc.UnknownMmeUeS1apId
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_unknown_mme_ue_s1ap_id]);
                //HO.IartOutSucc.UnknownEnbUeS1apId
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_unknown_enb_ue_s1ap_id]);
                //HO.IartOutSucc.UnknownPairUeS1apId
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_unknown_pair_ue_s1ap_id]);
                //HO.IartOutSucc.HandoverDesirableForRadioReason
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_handover_desirable_for_radio_reason]);
                //HO.IartOutSucc.TimeCriticalHandover
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_time_critical_handover]);
                //HO.IartOutSucc.ResourceOptimisationHandover
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_resource_optimisation_handover]);
                //HO.IartOutSucc.ReduceLoadInServingCell
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_reduce_load_in_serving_cell]);
                //HO.IartOutSucc.UserInactivity
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_user_inactivity]);
                //HO.IartOutSucc.RadioConnectionWithUeLost
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_radio_connection_with_ue_lost]);
                //HO.IartOutSucc.LoadBalancingTauRequired
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_load_balancing_tau_required]);
                //HO.IartOutSucc.CsFallbackTriggered
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_cs_fallback_triggered]);
                //HO.IartOutSucc.UeNotAvailableForPsService
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_ue_not_available_for_ps_service]);
                //HO.IartOutSucc.RadioResourcesNotAvailable
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_radio_resources_not_available]);
                //HO.IartOutSucc.FailureInRadioInterfaceProcedure
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_failure_in_radio_interface_procedure]);
                //HO.IartOutSucc.InvalidQosCombination
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_invalid_qos_combination]);
                //HO.IartOutSucc.InterratRedirection
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_interrat_redirection]);
                //HO.IartOutSucc.InteractionWithOtherProcedure
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_interaction_with_other_procedure]);
                //HO.IartOutSucc.UnknownERABID
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_unknown_E_RAB_ID]);
                //HO.IartOutSucc.MultipleERABIDInstances
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_multiple_E_RAB_ID_instances]);
                //HO.IartOutSucc.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_encryption_and_or_integrity_protection_algorithms_not_supported]);
                //HO.IartOutSucc.S1IntraSystemHandoverTriggered
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_s1_intra_system_handover_triggered]);
                //HO.IartOutSucc.S1InterSystemHandoverTriggered
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_s1_inter_system_handover_triggered]);
                //HO.IartOutSucc.X2HOTriggered
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_radioNetwork[s1ap_x2_handover_triggered]);
                //HO.IartOutSucc.TransportResourceUnavailable
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_transport[s1ap_transport_resource_unavailable]);
                //HO.IartOutSucc.Unspecified_3
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_transport[s1ap_unspecified_3]);
                //HO.IartOutSucc.NormalRelease
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_nas[normal_release]);
                //HO.IartOutSucc.AuthenticationFfailure
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_nas[authentication_failure]);
                //HO.IartOutSucc.Detach
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_nas[detach]);
                //HO.IartOutSucc.Unspecified_4
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_nas[unspecified_4]);
                //HO.IartOutSucc.TransferSyntaxError
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_protocol[transfer_syntax_error]);
                //HO.IartOutSucc.AbstractSyntaxErrorReject
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_protocol[abstract_syntax_error_reject]);
                //HO.IartOutSucc.AbstractSyntaxErrorIgnoreAndNotify
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_protocol[abstract_syntax_error_ignore_and_notify]);
                //HO.IartOutSucc.MessageNotCompatibleWithReceiverState
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_protocol[message_not_compatible_with_receiver_state]);
                //HO.IartOutSucc.SemanticError
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_protocol[semantic_error]);
                //HO.IartOutSucc.AbstractSyntaxErrorFalselyConstructedMessage
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_protocol[abstract_syntax_error_falsely_constructed_message]);
                //HO.IartOutSucc.Unspecified_1
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_protocol[unspecified_1]);
                /*SPR 19596 Start*/
                //HO.IartOutSucc.ControlProcessingOverload
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_misc[s1ap_control_processing_overload]);
                //HO.IartOutSucc.NotEnoughUserPlaneProcessingResources
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_misc[s1ap_not_enough_user_plane_processing_resources]);
                //HO.IartOutSucc.HardwareFailure
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_misc[s1ap_hardware_failure]);
                //HO.IartOutSucc.OmIntervention
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_misc[s1ap_om_intervention]);
                /*SPR 19596 End*/
                //HO.IartOutSucc.Unspecified
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_misc[s1ap_unspecified]);
                //HO.IartOutSucc.UnknownPLMN
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_cause.ho_IratOutSucc_Cause_misc[s1ap_unknown_PLMN]);
                //HO.IartOutSucc.Sum
                oam_sprintf((char *)pm_xml_ho_param_value[ho_irat_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        inter_rat_ho_stats_list.inter_rat_ho_stats[target_counter].ho_IratOutSucc_sum);
                /*SPR 20171 Start */
                OAM_LOG(OAM, OAM_DETAILED, "ho_irat_count = %d  counters written in local memory."
                        " mcc_mnc %s target %s.",ho_irat_count,temp_buff_mcc_n_mnc,temp_target_str);
                /*SPR 20171 End */

                /*SPR 22537 Fix Start*/
                /*Code Removed*/
                /*SPR 22537 Fix Stop*/
            }
        }
        /*SPR 22537 Fix Start*/
        /*Code Removed*/
        /*SPR 22537 Fix Stop*/

        /*SPR 22367 Fix Start*/
        for(index = OAM_ZERO; index < max_ho_target_count;index++)
        /*SPR 22367 Fix End*/
        {
            /*SPR 20171 Start */
            OAM_LOG(OAM, OAM_DETAILED, "RRC_OAM_INTER_RAT_HO_IND_STATS_PRESENT not there "
                    "g_pm_xml_total_element_count_offset+1 = %d  Index while writing ho target param = %d value = %s",
                    g_pm_xml_total_element_count_offset + 1 ,index, pm_xml_ho_param_value[index] );
            /*SPR 20171 End */

            /* SPR 18861 */
            /*SPR 21675 Start*/
            oam_snprintf((char *)temp_buff,(sizeof(temp_buff)-OAM_ONE),"%d",index + elements_in_pm_xml_param_name + OAM_ONE);
            /*SPR 21675 End*/
            /* SPR 18861 */
            xmlTextWriterStartElement(writer, (xmlChar *)"r");
            xmlTextWriterWriteAttribute(writer, (xmlChar *)"p",(xmlChar *)temp_buff);
            xmlTextWriterWriteString(writer,(xmlChar *)pm_xml_ho_param_value[index]);
            xmlTextWriterEndElement(writer);
        }
        /*SPR 22537 Fix Start*/
        /*Code Removed*/
        /*SPR 22537 Fix Stop*/
        /* SPR 17176 changes end */

        /* HO.Failure.Num */
        /* SPR 19642 */
        oam_sprintf((char *)pm_xml_ho_param_value[0],"%d",
                oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].ho_failure_num);

        OAM_LOG(OAM, OAM_DETAILED, "g_pm_xml_total_element_count_offset+1 = %d	Index while writing ho target param = %d value = %d",
                g_pm_xml_total_element_count_offset + 1, index, pm_xml_ho_param_value[0] );
        /*SPR 21675 Start*/
        oam_snprintf((char *)temp_buff,(sizeof(temp_buff)-OAM_ONE),"%d",
                (index + g_pm_xml_total_element_count_offset + OAM_ONE));	
        /*SPR 21675 End*/
        xmlTextWriterStartElement(writer, (xmlChar *)"r");
        xmlTextWriterWriteAttribute(writer, (xmlChar *)"p",(xmlChar *)temp_buff);
        xmlTextWriterWriteString(writer,(xmlChar *)pm_xml_ho_param_value[0]);
        xmlTextWriterEndElement(writer);

        /*SPR 22537 Fix Start*/
        /*Code Removed*/
        /*SPR 22537 Fix Stop*/

        /*SPR 19642 */
        /*SPR 22367 Fix Start*/
        for(index = OAM_ZERO; index < (max_ho_target_count + OAM_ONE); index++)
            /*SPR 22367 Fix End*/
        {
            oam_sprintf((char *)pm_xml_ho_param_value[index], "NULL");
        }
        /*SPR 19642 */

        if (oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].bitmask
                & RRC_OAM_MEAS_ON_NEIGH_CELL_HO_IND_STATS_PRESENT )
        {
            /* SPR 19257 */ 
            meas_target_count = oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.
                handover_stats[loop_counter].
                meas_on_neigh_cell_ho_stats_list.count;
            /* SPR 19257 */ 
            if(meas_target_count > RRC_MAX_CONNECTION)
            {
                OAM_LOG(OAM, OAM_WARNING, "meas_target_count(%d) > RRC_MAX_CONNECTION (%d ) making meas_target_count = 1 as it is ERROR condition",
                        meas_target_count, RRC_MAX_CONNECTION);	
                meas_target_count = 1;
            }
            /* SPR 19257 */ 		

            for(meas_count = OAM_ZERO; meas_count < meas_target_count ;meas_count++)
            {
                /* Meas on neigh cell ho stats changes start */
                OAM_LOG(OAM, OAM_DETAILED, "reading meas_on_neigh_cell_ho_stats_list: count: %d", meas_count);

                meas_str_count = OAM_ZERO;
                /* SPR 19257 */ 
                oam_memset((void *)temp_target_str, OAM_ZERO, 7);
                fill_plmn_id_from_mcc_mnc(temp_target_str,oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].
                        /* SPR 19166 Start */
                        ecgi.plmn_identity);
                /* SPR 19166 Stop */

                /*fill_plmn_id_from_mcc_mnc(temp_target_str,oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                  meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[loop_counter].
                  SPR 19166 Fix Start 
                  ecgi.plmn_identity);
                  SPR 19166 Fix End */
                /* SPR 19257 */ 
                /*SPR 10987 Start*/
                //HO.OutAttTarget.UserInactivity
                /* SPR 19257 */ 
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_user_inactivity]);
                /* SPR 19257 */ 
                //HO.OutAttTarget.Unspecified_2
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_unspecified_2]);
                //HO.OutAttTarget.Tx2RelocOverallExpiry
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_tx2relocoverall_expiry]);
                //HO.OutAttTarget.SuccessfulHandover
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_successful_handover]);
                //HO.OutAttTarget.ReleaseDueToEutranGeneratedReason
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_release_due_to_eutran_generated_reason]);
                //HO.OutAttTarget.HandoverCancelled
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_handover_cancelled]);
                //HO.OutAttTarget.PartialHandover
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_partial_handover]);
                //HO.OutAttTarget.HoFailureInTargetEPCeNBOrTargetSystem
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_ho_failure_in_target_EPC_eNB_or_target_system]);
                //HO.OutAttTarget.HoTargetNotAllowed
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_ho_target_not_allowed]);
                //HO.OutAttTarget.TS1RelocOverallExpiry
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_tS1relocoverall_expiry]);
                //HO.OutAttTarget.TS1RelocPrepExpiry
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_tS1relocprep_expiry]);
                //HO.OutAttTarget.CellNotAvailable
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_cell_not_available]);
                //HO.OutAttTarget.UnknownTargetID
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_unknown_targetID]);
                //HO.OutAttTarget.NoRadioResourcesAvailableInTargetCell
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_no_radio_resources_available_in_target_cell]);
                //HO.OutAttTarget.UnknownMmeUeS1apId
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_unknown_mme_ue_s1ap_id]);
                //HO.OutAttTarget.UnknownEnbUeS1apIdUECNTX
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_unknown_enb_ue_s1ap_id]);
                /*SPR 19596 Start*/
                //HO.OutAttTarget.UnknownPairUeS1apId
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_unknown_pair_ue_s1ap_id]);		
                /*SPR 19596 End*/
                //HO.OutAttTarget.HandoverDesirableForRadioReason
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_handover_desirable_for_radio_reason]);
                //HO.OutAttTarget.TimeCriticalHandover
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_time_critical_handover]);
                //HO.OutAttTarget.ResourceOptimisationHandover
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_resource_optimisation_handover]);
                //HO.OutAttTarget.ReduceLoadInServingCell
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_reduce_load_in_serving_cell]);
                //HO.OutAttTarget.RadioConnectionWithUeLost
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_radio_connection_with_ue_lost]);
                //HO.OutAttTarget.LoadBalancingTauRequired
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_load_balancing_tau_required]);
                //HO.OutAttTarget.CsFallbackTriggered
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_cs_fallback_triggered]);
                //HO.OutAttTarget.UeNotAvailableForPsService
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_ue_not_available_for_ps_service]);
                //HO.OutAttTarget.RadioResourcesNotAvailable
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_radio_resources_not_available]);
                //HO.OutAttTarget.FailureInRadioInterfaceProcedure
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_failure_in_radio_interface_procedure]);
                //HO.OutAttTarget.InvalidQosCombination
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_invalid_qos_combination]);
                //HO.OutAttTarget.InterratRedirection
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_interrat_redirection]);
                //HO.OutAttTarget.InteractionWithOtherProcedure
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_interaction_with_other_procedure]);
                //HO.OutAttTarget.UnknownERABID
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_unknown_E_RAB_ID]);
                //HO.OutAttTarget.MultipleERABIDInstances
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_multiple_E_RAB_ID_instances]);
                //HO.OutAttTarget.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_encryption_and_or_integrity_protection_algorithms_not_supported]);
                //HO.OutAttTarget.S1IntraSystemHandoverTriggered
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_s1_intra_system_handover_triggered]);
                //HO.OutAttTarget.S1InterSystemHandoverTriggered
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_s1_inter_system_handover_triggered]);
                //HO.OutAttTarget.X2HandoverTriggered
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_radioNetwork
                        [s1ap_x2_handover_triggered]);
                //HO.OutAttTarget.TransportResourceUnavailable
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_transport
                        [s1ap_transport_resource_unavailable]);
                //HO.OutAttTarget.TransportUnspecified
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_transport
                        [s1ap_unspecified_3]);
                //HO.OutAttTarget.NormalRelease
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_nas
                        [normal_release]);
                //HO.OutAttTarget.AuthenticationFailure
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_nas
                        [authentication_failure]);
                //HO.OutAttTarget.Detach
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_nas
                        [detach]);
                //HO.OutAttTarget.Unspecified4
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_nas
                        [unspecified_4]);
                //HO.OutAttTarget.TransferSyntaxError
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_protocol
                        [transfer_syntax_error]);
                //HO.OutAttTarget.AbstractSyntaxErrorReject
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_protocol
                        [abstract_syntax_error_reject]);
                //HO.OutAttTarget.AbstractSyntaxErrorIgnoreAndNotify
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_protocol
                        [abstract_syntax_error_ignore_and_notify]);
                //HO.OutAttTarget.MessageNotCompatibleWithReceiverState
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_protocol
                        [message_not_compatible_with_receiver_state]);
                /*SPR 19596 Start*/
                //HO.OutAttTarget.SemanticError
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_protocol
                        [semantic_error]);
                //HO.OutAttTarget.AbstractSyntaxErrorFalselyConstructedMessage
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_protocol
                        [abstract_syntax_error_falsely_constructed_message]);
                //HO.OutAttTarget.Unspecified_1
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_protocol
                        [unspecified_1]);			
                /*SPR 19596 End*/
                //HO.OutAttTarget.ControlProcessingOverload
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_misc
                        [s1ap_control_processing_overload]);
                //HO.OutAttTarget.NotEnoughUserPlaneProcessingResources
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_misc
                        [s1ap_not_enough_user_plane_processing_resources]);
                /*SPR 19596 Start*/
                //HO.OutAttTarget.HardwareFailure
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_misc
                        [s1ap_hardware_failure]);			
                //HO.OutAttTarget.OMIntervention
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_misc
                        [s1ap_om_intervention]);
                /*SPR 19596 End*/
                //HO.OutAttTarget.S1APUnspecified
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_misc
                        [s1ap_unspecified]);
                //HO.OutAttTarget.S1APUnknownPLMN
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_cause.ho_OutAttTarget_Cause_misc
                        [s1ap_unknown_PLMN]);
                //HO.OutAttTarget.Sum
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutAttTarget_sum);
                /*SPR 19596 Code Deleted*/
                //HO.OutSuccTarget.Unspecified_2
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_unspecified_2]);
                //HO.OutSuccTarget.Tx2RelocOverallExpiry
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_tx2relocoverall_expiry]);
                //HO.OutSuccTarget.SuccessfulHandover
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_successful_handover]);
                //HO.OutSuccTarget.ReleaseDueToEutranGeneratedReason
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_release_due_to_eutran_generated_reason]);
                //HO.OutSuccTarget.HandoverCancelled
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_handover_cancelled]);
                //HO.OutSuccTarget.PartialHandover
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_partial_handover]);
                //HO.OutSuccTarget.HoFailureInTargetEPCeNBOrTargetSystem
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_ho_failure_in_target_EPC_eNB_or_target_system]);
                //HO.OutSuccTarget.HoTargetNotAllowed
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_ho_target_not_allowed]);
                //HO.OutSuccTarget.TS1RelocOverallExpiry
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_tS1relocoverall_expiry]);
                //HO.OutSuccTarget.TS1RelocPrepExpiry
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_tS1relocprep_expiry]);
                //HO.OutSuccTarget.CellNotAvailable
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_cell_not_available]);
                //HO.OutSuccTarget.UnknownTargetID
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_unknown_targetID]);
                //HO.OutSuccTarget.NoRadioResourcesAvailableInTargetCell
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_no_radio_resources_available_in_target_cell]);
                //HO.OutSuccTarget.UnknownMmeUeS1apId
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_unknown_mme_ue_s1ap_id]);
                //HO.OutSuccTarget.UnknownEnbUeS1apIdUECNTX
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_unknown_enb_ue_s1ap_id]);
                /*SPR 19596 Start*/
                //HO.OutSuccTarget.UnknownPairUeS1apId
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_unknown_pair_ue_s1ap_id]);		
                /*SPR 19596 End*/
                //HO.OutSuccTarget.HandoverDesirableForRadioReason
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_handover_desirable_for_radio_reason]);
                //HO.OutSuccTarget.TimeCriticalHandover
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_time_critical_handover]);
                //HO.OutSuccTarget.ResourceOptimisationHandover
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_resource_optimisation_handover]);
                //HO.OutSuccTarget.ReduceLoadInServingCell
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_reduce_load_in_serving_cell]);
                /*SPR 19596 Start*/
                //HO.OutSuccTarget.UserInactivity
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_user_inactivity]);
                /*SPR 19596 End*/
                //HO.OutSuccTarget.RadioConnectionWithUeLost
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_radio_connection_with_ue_lost]);
                //HO.OutSuccTarget.LoadBalancingTauRequired
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_load_balancing_tau_required]);
                //HO.OutSuccTarget.CsFallbackTriggered
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_cs_fallback_triggered]);
                //HO.OutSuccTarget.UeNotAvailableForPsService
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_ue_not_available_for_ps_service]);
                //HO.OutSuccTarget.RadioResourcesNotAvailable
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_radio_resources_not_available]);
                //HO.OutSuccTarget.FailureInRadioInterfaceProcedure
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_failure_in_radio_interface_procedure]);
                //HO.OutSuccTarget.InvalidQosCombination
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_invalid_qos_combination]);
                //HO.OutSuccTarget.InterratRedirection
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_interrat_redirection]);
                //HO.OutSuccTarget.InteractionWithOtherProcedure
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_interaction_with_other_procedure]);
                //HO.OutSuccTarget.UnknownERABID
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_unknown_E_RAB_ID]);
                //HO.OutSuccTarget.MultipleERABIDInstances
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_multiple_E_RAB_ID_instances]);
                //HO.OutSuccTarget.EncryptionAndOrIntegrityProtectionAlgorithmsNotSupported
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_encryption_and_or_integrity_protection_algorithms_not_supported]);
                //HO.OutSuccTarget.S1IntraSystemHandoverTriggered
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_s1_intra_system_handover_triggered]);
                //HO.OutSuccTarget.S1InterSystemHandoverTriggered
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_s1_inter_system_handover_triggered]);
                //HO.OutSuccTarget.X2HandoverTriggered
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_radioNetwork
                        [s1ap_x2_handover_triggered]);
                //HO.OutSuccTarget.TransportResourceUnavailable
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_transport
                        [s1ap_transport_resource_unavailable]);
                //HO.OutSuccTarget.TransportUnspecified
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_transport
                        [s1ap_unspecified_3]);
                //HO.OutAttTarget.NormalRelease
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_nas
                        [normal_release]);
                //HO.OutAttTarget.AuthenticationFailure
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_nas
                        [authentication_failure]);
                //HO.OutSuccTarget.Detach
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_nas
                        [detach]);
                //HO.OutAttTarget.Unspecified4
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_nas
                        [unspecified_4]);
                //HO.OutSuccTarget.TransferSyntaxError
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_protocol
                        [transfer_syntax_error]);
                //HO.OutSuccTarget.AbstractSyntaxErrorReject
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_protocol
                        [abstract_syntax_error_reject]);
                //HO.OutSuccTarget.AbstractSyntaxErrorIgnoreAndNotify
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_protocol
                        [abstract_syntax_error_ignore_and_notify]);
                //HO.OutSuccTarget.MessageNotCompatibleWithReceiverState
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_protocol
                        [message_not_compatible_with_receiver_state]);
                /*SPR 19596 Start*/
                //HO.OutSuccTarget.SemanticError
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_protocol
                        [semantic_error]);
                //HO.OutSuccTarget.AbstractSyntaxErrorFalselyConstructedMessage
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_protocol
                        [abstract_syntax_error_falsely_constructed_message]);
                //HO.OutSuccTarget.Unspecified_1
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_protocol
                        [unspecified_1]);	
                /*SPR 19596 End*/
                //HO.OutSuccTarget.ControlProcessingOverload
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_misc
                        [s1ap_control_processing_overload]);
                //HO.OutSuccTarget.NotEnoughUserPlaneProcessingResources
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_misc
                        [s1ap_not_enough_user_plane_processing_resources]);
                /*SPR 19596 Start*/
                //HO.OutSuccTarget.HardwareFailure
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_misc
                        [s1ap_hardware_failure]);			
                //HO.OutSuccTarget.OMIntervention
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_misc
                        [s1ap_om_intervention]);
                /*SPR 19596 End*/
                //HO.OutSuccTarget.S1APUnspecified
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_misc
                        [s1ap_unspecified]);
                //HO.OutSuccTarget.S1APUnknownPLMN
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_cause.ho_OutSuccTarget_Cause_misc
                        [s1ap_unknown_PLMN]);
                //HO.OutSuccTarget.Sum
                oam_sprintf((char *)pm_xml_ho_param_value[meas_str_count++],"%d",
                        oam_pm_rrc_kpi_node->oam_rrc_handover_stats_ind.handover_stats[loop_counter].
                        meas_on_neigh_cell_ho_stats_list.meas_on_neigh_cell_ho_stats[meas_count].
                        ho_OutSuccTarget_sum);
                /*SPR 22537 Fix Start*/
                /*Code Removed*/
                /*SPR 22537 Fix Stop*/
            }
        }
        /*SPR 22537 Fix Start*/
        /*Code Removed*/
        /*SPR 22537 Fix Stop*/
        /*SPR 22367 Fix Start*/
        for(index = OAM_ZERO; index < (max_ho_target_count + OAM_ONE);index++)
        /*SPR 22367 Fix End*/
        {
            /*SPR 20171 Start */
            /*SPR 22367 Fix Start*/
            OAM_LOG(OAM, OAM_DETAILED, "RRC_OAM_MEAS_ON_NEIGH_CELL_HO_IND_STATS_PRESENT not there "
                    "g_pm_xml_total_element_count_offset+  max_ho_target_count(%d)+2 = %d "
                    "Index while writing ho target param = %d value = %s\n",
                    max_ho_target_count,(g_pm_xml_total_element_count_offset + max_ho_target_count+OAM_TWO), 
                    index, pm_xml_ho_param_value[index]
                    );
            /*SPR 22367 Fix End*/
            /*SPR 20171 End*/

            /* SPR 18861 */
            /*SPR 21675 Start*/
            oam_snprintf((char *)temp_buff,(sizeof(temp_buff)-OAM_ONE),"%d",index+ (elements_in_pm_xml_param_name +  max_ho_target_count + OAM_TWO));
            /*SPR 22367 Fix End*/
            /*SPR 21675 End*/
            /* SPR 18861 */
            xmlTextWriterStartElement(writer, (xmlChar *)"r");
            xmlTextWriterWriteAttribute(writer, (xmlChar *)"p",(xmlChar *)temp_buff);
            xmlTextWriterWriteString(writer,(xmlChar *)pm_xml_ho_param_value[index]);
            xmlTextWriterEndElement(writer);
        }
        xmlTextWriterEndElement(writer);
        /*  coverity 94824 fix start */
        /* code removed */ /* ho_irat_count = OAM_ZERO; */
        /*  coverity 94824 fix end */
        /* SPR 17176 changes end */
        /*SPR 22537 Fix Start*/
        /*Code Removed*/
        /*SPR 22537 Fix Stop*/
        /* Coverity CID 116216 Fix Start */
        OAM_PM_2D_TEMP_ARR_MEM_FREE(pm_xml_ho_param_value, OAM_PM_TOTAL_MEAS_KPI_COUNT, error_code);
        /* Coverity CID 116216 Fix End */
        /* SPR 15018 START */
        /* SPR 15018 DESCRIPTION: This ends the "measInfo" tag */
        xmlTextWriterEndElement(writer);
        /* SPR 22296 Fix Start */
    }
    else
    {
        /* Coverity CID 147346 Fix Start */
        if (pm_xml_ho_param_value != NULL)
            oam_mem_free(pm_xml_ho_param_value, &error_code); //_SUR_COV_ CID: 122024 
        /* Coverity CID 147346 Fix end */
    }
    /* SPR 22296 Fix End */
    /* SPR 15018 END */
    OAM_FUN_EXIT_TRACE();
}
/* SPR_12747_CHANGES */
/*SPR 11120 FIX end*/

oam_return_et oam_send_pdcp_reset_config_request
(/* + SPR 17439 */
 void)
/* - SPR 17439 */
{
    /*SPR 21941 Start*/
    UInt8 l2_instance = OAM_ZERO;
    /*SPR 21941 End*/
	oam_pdcp_config_KPI_stats_req_t pdcp_config_kpi_stats_req;
	oam_error_code_et error_code = NO_ERROR;
	cli_reset_flag.pdcp_reset_flag = OAM_ONE;
	OAM_FUN_ENTRY_TRACE();
	oam_memset((void *)&pdcp_config_kpi_stats_req, OAM_ZERO, sizeof(oam_pdcp_config_KPI_stats_req_t));
	pdcp_config_kpi_stats_req.durationT = OAM_PDCP_CONFIG_KPI_STATS_REQ_DURATION;
	pdcp_config_kpi_stats_req.kpiToReportBitMap = OAM_PDCP_KPIBITMAP;
    /*SPR 21941 Start*/
    for(l2_instance=OAM_ZERO;l2_instance<OAM_MAX_NUM_L2_INSTANCE;l2_instance++)
    {
        if(PDCP_OAM_INIT_CNF & l2_comm_info[l2_instance].init_flag)
        {
            pdcp_config_kpi_stats_req.periodicReporting  = OAM_ZERO;
            if (OAM_FAILURE == oam_pdcp_config_kpi_stats_req(
                        &pdcp_config_kpi_stats_req, OAM_MODULE_ID, OAM_ONE, &error_code, l2_instance))
            {
                OAM_LOG(OAM,OAM_ERROR,"Error in oam_pdcp_config_kpi_stats_req with error code %d",
                        error_code); /* Coverity_14DEC */
                OAM_FUN_EXIT_TRACE();
                return OAM_FAILURE;
            }
        } 
    }

    /*SPR 21941 End*/

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/* SPR 15366 START */
void oam_pm_handle_shutdown_command(UInt8 CellId)
{
	UInt32 curl_error_code = OAM_ZERO;
    Char8 curl_error_msg[OAM_CURL_ERROR_MSG_ZIZE] = {OAM_ZERO};	
    OAM_FUN_ENTRY_TRACE();
    if(g_pm_xml_params.periodic_reporting_ongoing != 0)
    {
        //stop all running timers
        //stop all running timers
        if(*(int*)g_pm_xml_params.pm_xml_upload_interval_timer_id >0)
        {
            oam_pm_timer_stop(g_pm_xml_params.pm_xml_upload_interval_timer_id);
            g_pm_xml_params.pm_xml_upload_interval_timer_id = 0;
        }
        /*SPR 20928 Fix Start*/
        if(g_pm_xml_params.pm_xml_file_write_timer_id != 0)
        {
            oam_pm_timer_stop(g_pm_xml_params.pm_xml_file_write_timer_id);
            g_pm_xml_params.pm_xml_file_write_timer_id = 0;
        }
        if(g_pm_xml_params.pm_xml_upload_timer_id != 0)
        /*SPR 20928 Fix End*/
        {
            oam_pm_timer_stop(g_pm_xml_params.pm_xml_upload_timer_id);
            g_pm_xml_params.pm_xml_upload_timer_id = 0;
        }

        oam_pm_write_data_to_file();

        //close the pm xml file
        oam_pm_xml_file_close();
        //send upload request for file to TR069 client
        if(oam_prov_req.oam_tr069_req.tr069_mode)
        {
            oam_send_file_upload_request();
        }
        else
        {
            if(OAM_FAILURE == oam_pm_xml_ftp_server_upload(&curl_error_code))
            {
                oam_sprintf(curl_error_msg,  "FTP Upload Failed with libcurl error code : %d", curl_error_code);
                OAM_LOG(OAM, OAM_ERROR, "oam_pm_xml_ftp_server_upload failed");
                OAM_ALARM(PM_DATA_UPLOAD_FAILURE_ALARM_ID,2,
                        NOTIFY_NEW_ALARM,PROCESSING_ERROR_ALARM,
                        UNAVAILABLE,EXPEDITED_ALARM,
                        "PM Data Upload Failure",curl_error_msg,CellId);
            }
        }

    }

    OAM_FUN_EXIT_TRACE();

}
/* SPR 15366 END */
