/*****************************************************************************
 * File Name        : oam_perf_mgmt.h
 *
 * Description      : This file contains includes for PERF management 
 *
 * Revision History :
 *
 * Date         Author           Reference            Comments
 * -----        ------           ---------            --------
 * April,2012  Neeraj Kumar     OAM Design Document   Initial Version                
 * Sept,2013   Raveen Sharma    CSR 51606 Fix         Code Changes 
 *                                                    for CSR Fix
 * May,2014    Swetank Srivastava   SPR 10566 Fix	  Code Change
 * May,2014    Nitin   Vijay        PDCP KPI Code	  Linked list Changes
 * June,2014   Swetank Srivastava   SPR 10987 Fix	  Code Change
 * May,2014    Swetank Srivastava   SPR 10023 Fix	  Structure and function defination 
 *													  for pdcp batch processing
 * Aug 2014     NITIN VIJAY         PM trans ID changes
 * Nov 2014     Karuna Bhardwaj     eICIC Enhancement
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 ****************************************************************************/

#ifndef OAM_PM_H
#define OAM_PM_H

/****************************************************************************
 **  INCLUDES
 ****************************************************************************/
#include <oam_config_tcb.h>
#include <libxml/xmlwriter.h>
#include <oam_sim.h>
/* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
#include<rrc_m2apOam_intf.h>
#endif
/* EMBMS Changes End */


/****************************************************************************
 **  MACROS
 ****************************************************************************/

/*SPR 13857 START*/
#define OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr,loop_counter, p_error_code)\
{\
       oam_counter_t loop_counter2 = OAM_ZERO;\
       for (loop_counter2 = OAM_ZERO; loop_counter2 < loop_counter; loop_counter2++)\
       {\
                  oam_mem_free(temp_arr[loop_counter2], &p_error_code);\
              }\
       oam_mem_free(temp_arr, &p_error_code);\
}

#define OAM_PM_INFORM_API_MEM_FREE(temp_arr,loop_counter, p_error_code)\
{\
       oam_counter_t loop_counter2 = OAM_ZERO;\
       for (loop_counter2 = OAM_ZERO; loop_counter2 < loop_counter; loop_counter2++)\
       {\
                  oam_mem_free(temp_arr[loop_counter2], &p_error_code);\
              }\
}
/*SPR 13857 END*/

/*Coverity FIX 52856 */
#define OAM_FILE_PRINT(file_ptr,str,index,value)  \
{\
   oam_sprintf(temp_str,"%s[%d]",str,index); \
   oam_fprintf(file_ptr,"%-55s=%4u\n",temp_str,value); \
}
/* PM_OAM_CHANGES_RRC_START */
#define OAM_FILE_PRINT_WITH_PARAM(file_ptr,str1,str2,index1,index2,value)  \
{\
   oam_sprintf(temp_str,"%s[%d].%s[%d]",str1,index1,str2,index2); \
   oam_fprintf(file_ptr,"%-55s=%4u\n",temp_str,value); \
}

#define OAM_FILE_PRINT_WITH_PARAM_WITHOUT_INDEX(file_ptr,str1,str2,index1,value)  \
{\
   oam_sprintf(temp_str,"%s[%d].%s",str1,index1,str2); \
   oam_fprintf(file_ptr,"%-55s=%4u\n",temp_str,value); \
}
/* PM_OAM_CHANGES_RRC_END */

#define OAM_FILE_PRINT_WITHOUT_INDEX(file_ptr,str,value)  \
{\
   oam_sprintf(temp_str,"%s",str); \
   oam_fprintf(file_ptr,"%-55s=%4u\n",temp_str,value);\
}

#define RRC_OAM_RESET_CELL_STATISTICS_PRESENT  0x20
#define OAM_TRANSACTION_ID                        0
#define OAM_SOURCE_ID                             2
#define OAM_API_ID                                6
/* SPR_10579 Start*/
#define OAM_HANDLE_MULTI_CONFIG_RESP           0x0F
/* SPR_10579 End*/
#define OAM_CONFIG_KPI_TIMER_VALUE               15
#define OAM_PDCP_RRC_CONFIG_RESP_VALUE         0x05
#define OAM_MAC_RRC_CONFIG_RESP_VALUE          0x03
#define OAM_MAC_PDCP_CONFIG_RESP_VALUE         0x06

/* SPR_10579 Start*/
#define OAM_PDCP_RRC_RRM_CONFIG_RESP_VALUE     0x0E  /*Resetting MAC Bit */
#define OAM_MAC_RRC_RRM_CONFIG_RESP_VALUE      0x0D  /*Resetting PDCP Bit */
#define OAM_MAC_PDCP_RRM_CONFIG_RESP_VALUE     0x0B  /*Resetting RRC Bit */
#define OAM_MAC_PDCP_RRC_CONFIG_RESP_VALUE     0x07  /*Resetting RRM Bit */
/* SPR_10579 End*/

/********************************PM_CODE**************************************/
#define  OAM_CONFIG_ENABLE_BIT                     0x01
#define  OAM_CONFIG_SAMPLING_TIMER_BIT             0x02
#define  OAM_CONFIG_PERIODIC_UPLOAD_TIME_BIT       0x04
#define  OAM_CONFIG_FILE_WRITE_TIMER_BIT           0x08
#define  OAM_CONFIG_PERIODIC_UPLOAD_INTERVAL_BIT   0x10

#define  OAM_CONFIG_URL_BIT                        0x20
#define  OAM_CONFIG_USERNAME_BIT                   0x40
#define  OAM_CONFIG_PASSWORD_BIT                   0x80

#define BIT_IN_ONE_BYTE                               8

#define KPI_TYPE_VALUE				                157
/*eICIC_PHASE_1_2_CHANGES_START*/
#define MAX_MAC_KPI_ELEMENT                          13
/*eICIC_PHASE_1_2_CHANGES_END*/
#define MAX_MAC_KPI_THP_ELEMENT                       4
#define MAX_PDCP_KPI_ELEMENT                          6
#define MAX_PDCP_KPI_THP_ELEMENT                      5   
#define MAX_RRC_KPI_ELEMENT                           6  
/*	OAM_MAC_KPIBITMAP indicates which KPI should be reported to the OAM. 
	KPI.s corresponding to which bit is set should be reported.
	-	The first bit (LSB) represents totalPRBUsage for DL.
	-	The second bit represents totalPRBUsage for UL
	-	The third bit represents totalPRBUsage per QCI for DL
	-	The fourth bit represents totalPRBUsage per QCI for UL
	-	The fifth bit represents numberOfActive UE per QCI for DL
	-	The sixth bit represents numberOfActive UE per QCI for UL
	-	The seventh bit represents rcvdRandomAccessPreambles
	-	The eighth bit represents packetDelay per QCI for DL	
	-	The ninth bit represents throughput for DL (This bit is not supported )
	-	The tenth bit represents DL CQI distribution.
    -   The eleventh bit represent ERAB activity counters (In session time for UE and ERAB)
*/
/*spr 19296 fix start*/
#define OAM_MAC_KPIBITMAP                            4095     //3839    
#define OAM_PDCP_KPIBITMAP                           63 
/*spr 19296 fix end*/
#define OAM_PDCP_UE_PERF_KPIBITMAP                   31
#define OAM_PDCP_CELL_PERF_KPIBITMAP                 31
#define OAM_RRC_KPIBITMAP                            63
#define OAM_PDCP_KPI_MAX_CONFIG_TIME                  8
/* SPR 14019 FIX Start */
#ifdef PERF_STATS
/* SPR 14019 FIX End */
#define OAM_PDCP_PERF_STATS_MAX_CONFIG_TIME           8
#define OAM_PDCP_CONFIG_PERF_STATS_REQ_DURATION     100
/* SPR 14019 FIX Start */
#endif
/* SPR 14019 FIX End */

#define OAM_KPI_MAX_CONFIG_TIME                       8
#define OAM_RRC_KPI_MAX_CONFIG_TIME                   8
#define OAM_PDCP_CONFIG_KPI_STATS_REQ_DURATION      100

#define OAM_MAC_CONFIG_KPI_STATS_REQ_DURATION       100
#define OAM_MAC_KPI_STATS_IND_MAX_BITMASK           255

#define PM_XML_SPV_PM_REPORT_EN_DISABLE           0x01 
#define PM_XML_SPV_UPLOAD_INTERVAL_TIMER           0x02
#define PM_XML_SPV_UPLOAD_TIMER                    0x04
#define PM_XML_SPV_SAMPLING_INTERVAL_TIMER         0x08
#define PM_XML_SPV_FILE_WRITE_TIMER                0x16
#define PM_XML_MAX_PARAM_VALUE                     20000
#define PM_PDCP_KPI_MULTIPLIER			   1000000
#define RRM_KPI_DURATION_MAX                       900

/* SPR 14504 FIX START */
/*SPR_16913_START*/

/*SPR 23209 FIXED START*/
#define PM_XML_TOTAL_ELEMENT_COUNT_OFFSET           558 /* Total no of pegs except Eutran HO params */
/*SPR 23209 FIXED END*/

/*SPR_16913_END*/
/* SPR 14504 FIX END */

/*SPR 23209 FIXED START*/
#define PM_XML_TOTAL_ELEMENT_COUNT                  558 /* Total pegs in PM XML File */
/*SPR 23209 FIXED END*/


/* SPR_12747_CHANGES */
/* #define PM_XML_TOTAL_ELEMENT_COUNT                  1026 *//* Total pegs in PM XML File */ 
/* SPR 14005 Changes Start  */
/* SPR 14005 Changes End  */
/*#define PM_XML_TOTAL_ELEMENT_COUNT_OFFSET           1026 *//* Total no of pegs except Eutran HO params */ 
/* SPR 14050 Changes Start  */
/* SPR 14792 Changes Start  */
/* Current OFFSET count for following:
 * pm_xml_mac_param_name   : 80
 * pm_xml_pdcp_param_name  : 78
 * pm_xml_conn_st_param_name : 93
 * pm_xml_erab_param_name  : 351
 * pm_xml_ho_param_name    : 232
 * pm_xml_s1sig_param_name : 2
 * pm_xml_paging_param_name: 3 */ 

/* SPR 18861 */
/* Hard Coded Offset Removed */
/* SPR 18861 */
/* SPR 14792 Changes End  */
/* SPR 14050 Changes End */
/* Below are the Offset values for each params counter starting in xml file to map exact peg */
#define PM_XML_CONN_ST_PARAM_START                  0
/*SPR 10987 Start*/
#define PM_XML_ERAB_PARAM_START                     93
/* SPR 17176 changes start */

/*SPR 23209 FIXED START*/
/*SPR_17907_start*/
#define PM_XML_S1SIG_PARAM_START                    189
#define PM_XML_PAGING_PARAM_START                   191
#define PM_XML_HO_PARAM_START                       192
/* SPR 14504 FIX START */
/*SPR 23209 FIXED END*/
/*SPR_16913_START*/
/*SPR_16913_END*/
/*SPR 23209 FIXED START*/
#define PM_XML_MAC_PARAM_START                      205
#define PM_XML_PDCP_PARAM_START                     480
/*SPR 23209 FIXED END*/


/* SPR 14504 FIX END */

/*SPR_17907_start*/
/* SPR 17176 changes end */
/* SPR_12747_CHANGES */

/* SPR 18861 */
/* Hard Coded Offset Removed */
/* SPR 18861 */
/* SPR 14005 Changes Start  */
/* SPR 17176 changes start */
/* SPR 18861 */
/* Hard Coded Offset Removed */
/* SPR 18861 */
/* SPR 17176 changes end */
/* SPR 14005 Changes End  */
/*SPR 10987 End*/
/* SPR_12747_CHANGES */
/*#define PM_XML_EUTRAN_HO_PARAM_START                333*/ /* Offset counter at which Eutran HO param will start to dump in XML file */
/* SPR_12747_CHANGES */
/* SPR_11231_START */
#define MAX_NUM_BATCH 				    14
/* SPR_11231_START */

/*SPR 22367 Fix Start*/
/*SPR 10987 start*/
typedef enum {
	transfer_syntax_error = 0,
	abstract_syntax_error_reject = 1,
	abstract_syntax_error_ignore_and_notify = 2,
	message_not_compatible_with_receiver_state = 3,
	semantic_error = 4,
	abstract_syntax_error_falsely_constructed_message = 5,
	unspecified_1 = 6,
    s1ap_CauseProtocol_Root_max = 7
} s1ap_CauseProtocol_Root;

typedef enum {
	normal_release,
	authentication_failure,
	detach,
	unspecified_4,
    s1ap_relReq_cause_nas_max
}s1ap_relReq_cause_nas;

/*SPR 10987 end*/

typedef enum {
	s1ap_control_processing_overload = 0,
	s1ap_not_enough_user_plane_processing_resources = 1,
	s1ap_hardware_failure = 2,
	s1ap_om_intervention = 3,
	s1ap_unspecified = 4,
	s1ap_unknown_PLMN = 5,
    oam_s1ap_CauseMisc_Root_max = 6
} oam_s1ap_CauseMisc_Root;

typedef enum {
	s1ap_redirection_towards_1xRTT = 36,
	s1ap_not_supported_QCI_value = 37,
	s1ap_invalid_CSG_Id = 38,
	s1ap_CauseRadioNetwork_Ext_max = 39
} s1ap_CauseRadioNetwork_Ext;

typedef enum {
	s1ap_transport_resource_unavailable = 0,
	s1ap_unspecified_3 = 1,
    s1ap_CauseTransport_Root_max = 2
} s1ap_CauseTransport_Root;


typedef enum {
	s1ap_unspecified_2 = 0,
	s1ap_tx2relocoverall_expiry = 1,
	s1ap_successful_handover = 2,
	s1ap_release_due_to_eutran_generated_reason = 3,
	s1ap_handover_cancelled = 4,
	s1ap_partial_handover = 5,
	s1ap_ho_failure_in_target_EPC_eNB_or_target_system = 6,
	s1ap_ho_target_not_allowed = 7,
	s1ap_tS1relocoverall_expiry = 8,
	s1ap_tS1relocprep_expiry = 9,
	s1ap_cell_not_available = 10,
	s1ap_unknown_targetID = 11,
	s1ap_no_radio_resources_available_in_target_cell = 12,
	s1ap_unknown_mme_ue_s1ap_id = 13,
	s1ap_unknown_enb_ue_s1ap_id = 14,
	s1ap_unknown_pair_ue_s1ap_id = 15,
	s1ap_handover_desirable_for_radio_reason = 16,
	s1ap_time_critical_handover = 17,
	s1ap_resource_optimisation_handover = 18,
	s1ap_reduce_load_in_serving_cell = 19,
	s1ap_user_inactivity = 20,
	s1ap_radio_connection_with_ue_lost = 21,
	s1ap_load_balancing_tau_required = 22,
	s1ap_cs_fallback_triggered = 23,
	s1ap_ue_not_available_for_ps_service = 24,
	s1ap_radio_resources_not_available = 25,
	s1ap_failure_in_radio_interface_procedure = 26,
	s1ap_invalid_qos_combination = 27,
	s1ap_interrat_redirection = 28,
	s1ap_interaction_with_other_procedure = 29,
	s1ap_unknown_E_RAB_ID = 30,
	s1ap_multiple_E_RAB_ID_instances = 31,
	s1ap_encryption_and_or_integrity_protection_algorithms_not_supported = 32,
	s1ap_s1_intra_system_handover_triggered = 33,
	s1ap_s1_inter_system_handover_triggered = 34,
	s1ap_x2_handover_triggered = 35,
    oam_s1ap_CauseRadioNetwork_Root_max = 36
} oam_s1ap_CauseRadioNetwork_Root;

#define OAM_PM_TOTAL_MEAS_KPI_COUNT 			(MAX_NUM_CELLS * RRC_MAX_CONNECTION * \
                                                ((oam_s1ap_CauseMisc_Root_max + s1ap_CauseRadioNetwork_Ext_max \
                                                 + s1ap_CauseTransport_Root_max + s1ap_CauseProtocol_Root_max +\
                                                  s1ap_relReq_cause_nas_max)*OAM_TWO) + OAM_TWO) 
/*SPR 22367 Fix End*/


/* SPR 12698 FIX START */
#define TMP_STR_BUFF_SIZE 256
#define NUM_BUFF_SIZE 100
/* SPR 12698 FIX END */
/* SPR 16807 Fix Start */
#define MAX_PM_XML_FILE_NAMES  100
/* SPR 16807 Fix End */

oam_return_et
oam_fill_cell_stats_req_param
(
    oam_rrc_get_cell_stats_req_t  *oam_rrc_get_cell_stats_req,
    UInt16                        bitmask,
    UInt8                         reset_stats,
    U8                            cell_index
);

oam_return_et 
oam_maintain_PM
(
    UInt8  *oam_pm_timer_expr_data,
    UInt64  oam_pm_timer_val
);

oam_return_et 
oam_PM_intf_process_timer
(
    oam_timer_t  timer_id,
    void         *p_timer_buf,
    void         *p_context
);

UInt8  
oam_bit_read_in_array
(
    UInt8  *bit_array,
    UInt16  bit_num
);


/****************************************************************************
 **  ENUMERATIONS / STRUCTURES
 ****************************************************************************/

typedef MacKpiStatsIndParams    oam_mac_kpi_stats_ind_params_t;
typedef MacKpiThpStatsIndParams oam_mac_kpi_thps_stats_ind_params_t;
typedef rrm_oam_kpi_ind_t oam_rrm_kpi_ind_t;
typedef rrm_oam_config_kpi_req_t oam_rrm_config_kpi_req_t;

/* SPR_11231_START */
typedef UInt8 oam_pm_kpi_bitmap;

/* SPR_11231_END */

typedef struct
{
    UInt8  flag;
    UInt8  rrm_flag;
}file_close_t;

file_close_t file_close;
typedef struct
{
    UInt16  bitmask;
    UInt8   cell_index;
    UInt8   reset_stats;
}oam_pm_show_rrc_cell_stats_t;

typedef struct
{
    UInt16  sampling_intrvl; 
    UInt16  upload_intrvl;   
}oam_periodic_info_t;

/*spr 19296 fix start*/
#define OAM_MAC_KPI_MAX_CONFIG_TIME                  8
typedef struct _oam_mac_thp_batch_node
{
	/* SPR 22296 Fix Start */
        UInt8 cell_idx;
	/* SPR 22296 Fix End */
        oam_mac_KPI_THP_stats_ind_t data;
        struct _oam_mac_thp_batch_node *next;
}oam_mac_thp_batch_node;

typedef struct oam_pm_mac_kpi_stats_node
{
        UInt8 cell_index;
        oam_bool_et Is_ind_present;
        oam_mac_KPI_stats_ind_t  oam_mac_kpi_stats_ind;
	/* SPR 22296 Fix Start */
        oam_mac_thp_batch_node *p_oam_mac_kpi_thp_stats_ind;
	/* SPR 22296 Fix End */
}oam_pm_mac_kpi_stats_node_t;

typedef struct oam_pm_mac_kpi_info_node
{
        UInt8 cell_index;
        oam_pm_mac_kpi_stats_node_t oam_pm_mac_kpi_stats_node[MAX_NUM_CELLS];
        struct _oam_mac_thp_batch_node *next_batch_node;
        struct oam_pm_mac_kpi_info_node *next_node_ptr;
}oam_pm_mac_kpi_info_node_t;

typedef struct
{
    UInt8                       configuration_time[OAM_MAC_KPI_MAX_CONFIG_TIME];
    UInt8                       kpi_info_store_on_off_flag;
    oam_periodic_info_t         periodic_info;
    oam_pm_mac_kpi_info_node_t *pm_mac_kpi_info_first_Node;
    UInt8                       ind_bit_mask;
    UInt8                       direct_bit_mask;
}oam_mac_kpi_info_t;

typedef struct
{
    UInt16 num_mac_kpi_stats_ind;          
    /*SPR 21950 Start*/
    UInt8  kpi_thp_flag;               
    /*SPR 21950 End*/
    void  *p_kpi_buf[MAX_NUM_CELLS];
    void  *p_kpi_thp_buf;
}oam_mac_kpi_ind_handler_t;

void
delete_mac_batch_data_from_link_list(SInt32 api_id);
              
oam_return_et
oam_mac_get_kpi_direct_thp_stats_cnf(UInt16 res_trans_id);

oam_return_et
oam_handle_get_mac_thp_batch_data
(
 void *p_api_buf,
 oam_error_code_et *p_error_code
 );
/*spr 19296 fix end*/

typedef struct oam_pm_kpi_info_Node_t
{
    UInt8 cell_index;
    oam_mac_kpi_stats_ind_params_t      MacKpiStatsIndParams;
    oam_mac_kpi_thps_stats_ind_params_t mac_kpi_thps_stats_ind_params;
    struct oam_pm_kpi_info_Node_t       *next_node_ptr;
}oam_pm_kpi_info_Node_t;

typedef struct 
{
	/*SPR 10023 FIX*/
    void  *p_kpi_stats_buf;
    UInt8 direct_kpi_resp_recvd_flag;
}oam_l2_direct_stats_handler_t;

/*SPR 10023 FIX*/
typedef struct _oam_pdcp_thp_batch_node
{
	/* SPR 22296 Fix Start */
	UInt8 cell_idx;
	/* SPR 22296 Fix End */
	oam_pdcp_KPI_THP_stats_ind_t data;
	struct _oam_pdcp_thp_batch_node *next;
}oam_pdcp_thp_batch_node;

/* SPR 14019 FIX Start */
#ifdef PERF_STATS
/* SPR 14019 FIX End */
typedef struct _oam_pdcp_ue_perf_stats_batch_node
{
	oam_pdcp_get_ue_perf_stats_cnf_t data;
	struct _oam_pdcp_ue_perf_stats_batch_node *next;
}oam_pdcp_ue_perf_stats_batch_node;
/* SPR 14019 FIX Start */
#endif
/* SPR 14019 FIX End */
/*SPR 10023 FIX end*/
/* AUT_BUG_FIXES */

typedef struct oam_pm_pdcp_kpi_info_Node_t
{
    oam_pdcp_KPI_stats_ind_t            oam_pdcp_KPI_ind;
/*SPR 10023 FIX start*/
    struct _oam_pdcp_thp_batch_node *next_batch_node;
/*SPR 10023 FIX end*/
    struct oam_pm_pdcp_kpi_info_Node_t  *next_node_ptr;
}oam_pm_pdcp_kpi_info_Node_t;

typedef struct oam_pm_rrc_kpi_info_Node_t
{
    UInt8 cell_index;
      oam_rrc_irat_mobility_stats_ind_t     irat_mobility_stats_ind;
      rrc_oam_ue_assoc_s1_conn_stats_ind_t  ue_assoc_s1_conn_stats_ind_t;
      rrc_oam_paging_stats_ind_t            paging_stats_ind_t;
      rrc_oam_rrc_connection_stats_ind_t    rrc_connection_stats_ind;
      oam_rrc_erab_stats_ind_t              erab_stats_ind_t;
      oam_rrc_handover_stats_ind_t          oam_rrc_handover_stats_ind;
      struct oam_pm_rrc_kpi_info_Node_t     *next_node_ptr;
      /*SPR 15018 START*/
      UInt8 timeStamp[100];
      /*SPR 15018 END*/

/*SPR 10023 FIX start*/
    struct _oam_pdcp_thp_batch_node *next_batch_node;
/*SPR 10023 FIX end*/
}oam_pm_rrc_kpi_info_Node_t;

typedef struct oam_pm_rrm_kpi_info_Node_t
{
    oam_rrm_kpi_ind_t oam_rrm_kpi_ind;
    struct oam_pm_rrm_kpi_info_Node_t *next_node_ptr;
}oam_pm_rrm_kpi_info_Node_t;

typedef struct
{
    UInt16  flag;
}oam_pm_rrc_reset_t;

/*Global structure for keeping get kpi reset status*/
typedef struct
{
   UInt16 flag;
}oam_pm_rrm_reset_t;
oam_pm_rrm_reset_t pm_rrm_reset;

/* SPR_11231_START */
typedef struct oam_pm_pdcp_kpi_stats_node
{
	UInt8 cell_index;
	oam_bool_et Is_ind_present;
 	oam_pdcp_KPI_stats_ind_t  oam_pdcp_kpi_stats_ind;	
	/* SPR 22296 Fix Start */
	oam_pdcp_thp_batch_node   *p_oam_pdcp_kpi_thp_stats_ind;
	/* SPR 22296 Fix End */
}oam_pm_pdcp_kpi_stats_node_t;

typedef struct oam_pm_pdcp_kpi_thp_data
{
	oam_pdcp_KPI_THP_stats_ind_t  oam_pdcp_kpi_thp_ind[MAX_NUM_BATCH];	
}oam_pm_pdcp_kpi_thp_data_t;

typedef struct oam_pm_pdcp_kpi_info_node
{
	/* AUT_BUG_FIXES */
	oam_pm_pdcp_kpi_stats_node_t oam_pm_pdcp_kpi_stats_node[MAX_NUM_CELLS];	
/*SPR 10023 FIX start*/
 	struct _oam_pdcp_thp_batch_node *next_batch_node;
/*SPR 10023 FIX end*/
//	oam_pm_pdcp_kpi_thp_data_t oam_pm_pdcp_kpi_thp_data;
	struct oam_pm_pdcp_kpi_info_node *next_node_ptr;
}oam_pm_pdcp_kpi_info_node_t;



typedef struct
{
    UInt8                       configuration_time[OAM_PDCP_KPI_MAX_CONFIG_TIME];
    UInt8                       kpi_info_store_on_off_flag;
    oam_periodic_info_t         periodic_info;
    oam_pm_pdcp_kpi_info_node_t *pm_pdcp_kpi_info_first_Node;
    UInt8                       ind_bit_mask;
    UInt8                       direct_bit_mask;
}oam_pdcp_kpi_info_t; 

/* SPR_11231_END */


/* SPR_11231_START */
typedef struct
{
    UInt16 num_pdcp_kpi_stats_ind;	
    /*SPR 21950 Start*/
    UInt8  kpi_thp_flag;               
    /*SPR 21950 End*/
    void  *p_kpi_buf[MAX_NUM_CELLS];
    void  *p_kpi_thp_buf;
}oam_pdcp_kpi_ind_handler_t;


/* SPR_11231_END*/
typedef struct 
{
    UInt8                  configuration_time[OAM_KPI_MAX_CONFIG_TIME];
    UInt8                  kpi_info_store_on_off_flag;
    oam_periodic_info_t    periodic_info;
    oam_pm_kpi_info_Node_t *pm_kpi_info_first_Node;
}oam_kpi_info_t; 


typedef struct 
{
    UInt8                      configuration_time[OAM_RRC_KPI_MAX_CONFIG_TIME];
    UInt8                      kpi_info_store_on_off_flag;
    oam_periodic_info_t        periodic_info;
    oam_pm_rrc_kpi_info_Node_t *pm_kpi_info_first_Node;
    UInt8                      cell_index;
    UInt8                      reset_stats; 
    UInt8                      rrc_sampling_start_flag;
}oam_rrc_kpi_info_t; 

typedef struct 
{
        
    oam_periodic_info_t periodic_info;
    oam_pm_rrm_kpi_info_Node_t *pm_kpi_info_first_Node;
}oam_rrm_kpi_info_t;





typedef struct
{
    void  *p_kpi_buf;
    void  *p_kpi_ind2_buf;
    UInt8  kpi_resp_recvd_flag;
}oam_l2_kpi_ind_handler_t;

typedef struct 
{
    UInt16  bitmask;  /* SPR 17949 */ 
}oam_pm_mac_kpi_stats_ind_params_name_t;


typedef struct 
{
    Char8  *avgDurationT;
    Char8  *ueIndex;
    Char8  *qci;
/*spr 19296 fix start*/
	/*Code deleted*/
/*spr 19296 fix end*/
}oam_mac_kpi_thp_stats_ind_params_name_t;


typedef struct 
{
    UInt8  bitmask[1]; 
}oam_pdcp_kpi_stats_ind_params_name_t;


typedef struct 
{
    UInt8  bitmask[1]; 
    Char8  *avgDurationT;
    Char8  *ueIndex;
    Char8  *qci;
/*spr 19711 fix start*/
	/*Code deleted*/
/*spr 19711 fix end*/
}oam_pdcp_Kpi_thp_stats_ind_params_name_t;

typedef struct 
{
    Char8  *pag_discardedNbr;
    Char8  *pag_succNbr;
}oam_rrc_csc_stat_paging_name_t;

typedef struct 
{
    Char8  *s1sig_connEstabAtt;
    Char8  *s1sig_connEstabSucc;
}oam_rrc_uecc_stat_ue_assoc_s1_connection_name_t;


typedef struct 
{
    Char8  *erab_estabInitFailNbr_cause_radioNetwork;
    Char8  *erab_estabInitFailNbr_cause_transport;
    Char8  *erab_estabInitFailNbr_cause_nas;
    Char8  *erab_estabInitFailNbr_cause_protocol;
    Char8  *erab_estabInitFailNbr_cause_misc;
}oam_rrc_uecc_erab_estab_init_fail_name_t;

typedef struct 
{
   Char8  *erab_estabAddFailNbr_cause_radioNetwork;
   Char8  *erab_estabAddFailNbr_cause_transport;
   Char8  *erab_estabAddFailNbr_cause_nas;
   Char8  *erab_estabAddFailNbr_cause_protocol;
   Char8  *erab_estabAddFailNbr_cause_misc;
}oam_rrc_uecc_oam_erab_estab_add_fail_name_t;


typedef struct 
{
    Char8  *erab_estabInitAttNbr_qci;
    Char8  *erab_estabInitAttNbr_sum;
    Char8  *erab_estabInitSuccNbr_qci;
    Char8  *erab_estabInitSuccNbr_sum;
    oam_rrc_uecc_erab_estab_init_fail_name_t erab_estab_init_fail_stats;
    Char8* erab_estabInitFailNbr_sum;
    Char8* erab_estabAddAttNbr_qci;
    Char8* erab_estabAddAttNbr_sum;
    Char8* erab_estabAddSuccNbr_qci;
    Char8* erab_estabAddSuccNbr_sum;
    oam_rrc_uecc_oam_erab_estab_add_fail_name_t erab_estab_add_fail_stats;
    Char8* erab_estabAddFailNbr_sum;
    Char8* erab_estabTimeMean;
    Char8*erab_estabTimeMax;
} oam_rrc_uecc_stat_erab_setup_name_t;

typedef struct 
{
    Char8  *erab_relEnbNbr_cause_radioNetwork;
    Char8  *erab_relEnbNbr_cause_transport;
    Char8  *erab_relEnbNbr_cause_nas;
    Char8  *erab_relEnbNbr_cause_protocol;
    Char8  *erab_relEnbNbr_cause_misc;
}oam_rrc_uecc_erab_rel_enb_cause_name_t;

typedef struct 
{
    Char8  *erab_relFailNbr_cause_radioNetwork;
    Char8  *erab_relFailNbr_cause_transport;
    Char8  *erab_relFailNbr_cause_nas;
    Char8  *erab_relFailNbr_cause_protocol;
    Char8  *erab_relFailNbr_cause_misc;
}oam_rrc_uecc_rel_fail_cause_name_t;


typedef struct 
{
    Char8  *erab_relEnbNbr_qci;
    Char8  *erab_relEnbNbr_sum;
    oam_rrc_uecc_erab_rel_enb_cause_name_t erab_rel_enb_cause_stats;
    Char8  *erab_relAttNbr_qci;
    Char8  *erab_relAttNbr_sum;
    Char8  *erab_relSuccNbr_qci;
    Char8  *erab_relSuccNbr_sum;
    oam_rrc_uecc_rel_fail_cause_name_t rel_fail_cause_stats;
    Char8  *erab_relFailNbr_sum;
}oam_rrc_uecc_stat_erab_release_name_t;

typedef struct 
{
    Char8  *erab_modQosFailNbr_cause_radioNetwork;
    Char8  *erab_modQosFailNbr_cause_transport;
    Char8  *erab_modQosFailNbr_cause_nas;
    Char8  *erab_modQosFailNbr_cause_protocol;
    Char8  *erab_modQosFailNbr_cause_misc;
}oam_rrc_uecc_stat_erab_mod_fail_cause_name_t;


typedef struct 
{
    Char8  *erab_modQosAttNbr_qci;
    Char8  *erab_modQosAttNbr_sum;
    Char8  *erab_modQosSuccNbr_qci;
    Char8  *erab_modQosSuccNbr_sum;
    oam_rrc_uecc_stat_erab_mod_fail_cause_name_t erab_mod_fail_cause_stats;
    Char8 *erab_modQosFailNbr_sum;
} oam_rrc_uecc_stat_erab_modification_name_t;

typedef struct 
{
    Char8  *erab_usageNbrMean;
    Char8  *erab_usageNbrMax;
}oam_rrc_uecc_stat_erab_number_name_t;


typedef struct 
{
    oam_rrc_uecc_stat_erab_setup_name_t          erab_setup_stats;
    oam_rrc_uecc_stat_erab_release_name_t        erab_release_stats;
    oam_rrc_uecc_stat_erab_modification_name_t   erab_mod_stats;
    oam_rrc_uecc_stat_erab_number_name_t         erab_num_stats;
}oam_rrc_erab_related_stats_name_t;

typedef struct 
{
    Char8  *rrc_connEstabAtt_cause;
    Char8  *rrc_connEstabAtt_sum;
    Char8  *rrc_connEstabSucc_cause;
    Char8  *rrc_connEstabSucc_sum;
    Char8  *rrc_connEstabFail_cause;
    Char8  *rrc_connEstabFail_sum;
} oam_rrc_uecc_stat_connection_establishment_name_t;

typedef struct 
{
    Char8  *rrc_connReEstabAtt_cause;
    Char8  *rrc_connReEstabAtt_sum;
    Char8  *rrc_connReEstabSucc_cause;
    Char8  *rrc_connReEstabSucc_sum;
    Char8  *rrc_connReEstabFail_cause;
    Char8  *rrc_connReEstabFail_sum;
}oam_rrc_uecc_stat_connection_reestablishment_name_t;

typedef struct 
{
    Char8  *ueCtxt_relReq_cause_radioNetwork;
    Char8  *ueCtxt_relReq_cause_transport;
    Char8  *ueCtxt_relReq_cause_nas;
    Char8  *ueCtxt_relReq_cause_protocol;
    Char8  *ueCtxt_relReq_cause_misc;
}oam_rrc_uecc_ue_ctxt_rel_req_fail_cause_name_t;

typedef struct 
{
    oam_rrc_uecc_ue_ctxt_rel_req_fail_cause_name_t  ueCtxt_relReq_cause;
    Char8  *ueCtxt_relReq_sum;
    Char8  *ueContext_RelSuccNbr;
} oam_rrc_uecc_stat_ue_context_release_name_t;

typedef struct 
{
    oam_rrc_uecc_stat_connection_establishment_name_t   rrc_connection_establishment;
    oam_rrc_uecc_stat_connection_reestablishment_name_t rrc_connection_reestablishment;
    oam_rrc_uecc_stat_ue_context_release_name_t         ue_contxt_rel_stats;
}oam_rrc_connection_related_stats_name_t;


typedef struct 
{
    UInt8  bitmask[1];    
}oam_rrc_get_cell_stats_resp_name_t; 


typedef struct
{
    oam_periodic_info_t  periodic_info;
}oam_all_kpi_info_t;

typedef struct
{
    UInt8   bit_mask;
    UInt8   success_fail_bitmask; 
    QTIMER  confog_kpi_id;    
}oam_handle_multiple_config_resp_t;


typedef struct
{
    oam_pm_rrc_kpi_info_Node_t *p_pm_rrc_kpi_info_first_Node;
}oam_handle_read_multiple_link_list_t; 

typedef struct
{
    UInt16  xml_str_count;
}oam_xml_str_count_t;


typedef struct
{
    UInt16  config_param_trans_id;
    UInt8   config_param_bit_mask;
    UInt16  config_des_module_id;
}oam_config_param_st;


typedef struct
{
    UInt16  sampling_timer;
}oam_timer_val_t;

typedef struct
{
    UInt16  src_id; 
    UInt8   flag;
    UInt16  trans_id;
}oam_set_config_req_t;


typedef struct
{
    UInt8  mac_reset_flag; 
    UInt8  pdcp_reset_flag; 
}oam_cli_reset_flag;

typedef struct FtpFile_t {
    char filename[STR_LEN_256];
    FILE *stream;
}FtpFile;

/***************************************************************************
 **  Extern Variable
 ****************************************************************************/

extern prov_request_t                               prov_req;
extern oam_pm_rrc_reset_t                           pm_rrc_reset;

/*spr 19296 fix start*/
extern oam_mac_kpi_info_t                               mac_kpi_info;
/*spr 19296 fix end*/

extern oam_pdcp_kpi_ind_handler_t                   g_pdcp_kpi_ind_handler; 
extern oam_pdcp_kpi_info_t                          pdcp_kpi_info;
extern oam_l2_direct_stats_handler_t                oam_mac_direct_stats_hndlr;
extern oam_l2_direct_stats_handler_t                oam_pdcp_direct_stats_hndlr;
extern oam_pm_mac_kpi_stats_ind_params_name_t       g_mac_kpi_stats_ind_params_name;
extern oam_pdcp_kpi_stats_ind_params_name_t         g_pdcp_kpi_stats_ind_params_name;
extern oam_config_param_st                          oam_config;
extern oam_set_config_req_t                         oam_set_config_req;
extern oam_cli_reset_flag                           cli_reset_flag;



/****************************************************************************
 **  FUNCTION PROTOTYPES
 ****************************************************************************/
/*spr 19296 fix start*/
void 
oam_pm_mac_read_data_from_link_list
(
    oam_mac_kpi_info_t  *kpi_info,
    UInt8 cell_id,
    oam_counter_t loop	
);
/*spr 19296 fix end*/
void
oam_pm_read_data_from_link_list
(
/* + SPR 17439 */
 void 
/* - SPR 17439 */
);


void 
oam_pm_pdcp_read_data_from_link_list
(
    oam_pdcp_kpi_info_t  *kpi_info,
    UInt8 cell_id,
    oam_counter_t loop	
);

/*spr 19296 fix start*/
void 
oam_pm_kpi_mac_info_strct_init
(
 oam_mac_kpi_info_t  *p_kpi_info
 );
/*spr 19296 fix end*/

void
oam_pm_kpi_pdcp_info_strct_init
(
    oam_pdcp_kpi_info_t  *p_kpi_info
);

void
oam_pm_kpi_rrc_info_strct_init
(
    oam_rrc_kpi_info_t  *p_kpi_info
);

void 
oam_pm_init_periodic_info
(
    oam_periodic_info_t  *periodic_info
);

oam_return_et 
oam_pm_updat_nxt_node_ptr_of_link_lst
(
    oam_kpi_info_t          *kpi_info,
    oam_pm_kpi_info_Node_t  *oam_pm_kpi_info_Node,
    UInt8                   cell_index
);

void 
oam_pm_init_kpi_info
(
    void
);
void 
oam_pm_init
(
    void
);

void 
oam_pm_init_l2_kpi_ind_struct
(
    oam_l2_kpi_ind_handler_t  *kpi_ind_handl_strt
);

void 
oam_pm_init_l2_kpi_ind_handler
(
    void
);

oam_return_et
oam_pm_mac_get_kpi_thp_stats_store
(
    void                                 *p_api_buf,
    oam_mac_kpi_thps_stats_ind_params_t  *p_mac_kpi_thps_stats_ind
);

oam_return_et
oam_pm_mac_kpi_stat_store
(
    void                            *p_api_buf,
    oam_mac_kpi_stats_ind_params_t  *p_mac_kpi_stats_ind_params
);
oam_return_et
oam_pm_pdcp_kpi_stat_ind
(
    void                      *p_api_buf,
    oam_pdcp_KPI_stats_ind_t  *p_KPI_stats_ind     
);

/*
 * SPR10023 fix, removed a parameter from function defination
 */
oam_return_et
oam_pm_pdcp_kpi_thp_stats_ind
(
    oam_pdcp_thp_batch_node **p_KPI_THP_stats_ind,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_add_in_pdcp_link_list
(
    oam_pdcp_kpi_info_t  *kpi_info
);


oam_return_et 
oam_set_bit_in_array
(
    UInt8   *p_bit_array,
    UInt16  bit_num
);

void 
oam_pm_mac_init_kpi_name
(
    void
);

void 
oam_pm_mac_init_kpi_thp_name
(
    void
);

void 
oam_pm_pdcp_init_kpi_name
(
    void
);

void 
oam_pm_pdcp_init_kpi_thp_name
(
    void
);

void 
oam_pm_rrc_init_kpi_thp_name
(
    void
);

oam_return_et
oam_rrc_pm_handle_get_cell_stats_resp
(
    void                           *p_api,
    UInt16                         data_len,
    oam_error_code_et              *p_error_code,
    oam_rrc_get_cell_stats_resp_t  *cell_stats_resp
);

oam_return_et
oam_pm_update_pdcp_kpi_info
(
    UInt64  src_kpi_on_off_flag,
    UInt16  sampling_intrvl_val,
    UInt16  upload_intrvl_val,
    UInt8   bitmask
);

oam_return_et
oam_pm_update_mac_kpi_info
(
    UInt64 src_kpi_on_off_flag,
    UInt16 sampling_intrvl_val,
    UInt16 upload_intrvl_val,   
    UInt16 bit_mask 
);

QTIMER
oam_pm_timer_start
(
    Char8   *oam_pm_timer_expr_data,
    UInt64  oam_pm_timer_val
);

oam_return_et
oam_pm_kpi_periodic_upload_timer_start
(
    UInt8   kpi_on_off_flag,
    Char8   *oam_pm_timer_expr_data,
    UInt64  oam_pm_timer_val
);

oam_return_et
oam_pm_all_periodic_upload_timer_start
(
    void
);

oam_return_et
oam_rrc_send_get_cell_stats_req
(         
 oam_rrc_get_cell_stats_req_t  *p_oam_rrc_get_cell_stats_req,
 UInt16                        src_module_id,
 UInt16                        transaction_id,
 oam_error_code_et             *p_error_code
 );

void 
oam_pm_rrc_updat_nxt_node_ptr_of_link_lst
(
    oam_rrc_kpi_info_t          *p_kpi_info,
    oam_pm_rrc_kpi_info_Node_t  *p_oam_pm_kpi_info_Node
);
/* SPR_11231_START */
void 
oam_pm_pdcp_updat_nxt_node_ptr_of_link_lst
(
    oam_pdcp_kpi_info_t          *p_kpi_info,
    oam_pm_pdcp_kpi_info_node_t  *p_oam_pm_kpi_info_Node
);
/* SPR 17777 fix */
oam_return_et 
oam_add_pdcp_kpi_thp_ind
(
 /* SPR 22296 Fix Start */
 oam_pdcp_thp_batch_node **p_pdcp_kpi_thp_stats_ind,
 UInt8 cell_idx
 /* SPR 22296 Fix End */
 );

/* + SPR 17439 */
void 
oam_pdcp_kpi_buffer_free(void);
/* - SPR 17439 */

/* SPR_11231_END */

oam_return_et
oam_fill_cell_stats_req_param
(
    oam_rrc_get_cell_stats_req_t  *oam_rrc_get_cell_stats_req,
    UInt16                         bitmask,
    UInt8                          reset_stats,
    UInt8                          cell_index
);


oam_return_et
oam_pm_rrc_kpi_periodic_sampling_timer_start
(            
/* + SPR 17439 */
 void 
/* - SPR 17439 */
);

oam_return_et
oam_add_in_mac_link_list
(
    oam_kpi_info_t  *p_kpi_info,
    UInt8            cell_index,
    void*            p_api_buf
);

oam_return_et
oam_handle_rrc_stats_recv_resp
(
    void  *p_api_buf,
    UInt16   trans_id,
    UInt16   dest_id
);

void 
oam_pm_bitmask_init
(
/* + SPR 17439 */
 void 
/* - SPR 17439 */
);

FILE *pm_l2_file_ptr,*pm_l3_file_ptr;


oam_return_et
oam_pm_l2_file_open
(
    oam_error_code_et  *p_error_code
);

oam_return_et
oam_pm_pdcp_read_linked_list_kpi_file
(
    oam_pdcp_KPI_stats_ind_t  *p_oam_pdcp_kpi,
    UInt16 *pdcp_str_count 
);
/*SPR 10023 FIX start*/
oam_return_et
oam_pm_pdcp_read_linked_list_kpi_thp_file
(
 /* SPR 22296 Fix Start */
 oam_pdcp_thp_batch_node *p_oam_pdcp_kpi_thp_stats_ind,
 /* SPR 22296 Fix End */
    UInt16 *pdcp_str_count 
);
/*SPR 10023 FIX end*/

void
oam_pm_mac_read_linked_list_kpi_dummy
(
    UInt16 *mac_str_count 
);

void
oam_pm_pdcp_read_linked_list_kpi_file_dummy
(
    UInt16 *mac_str_count
);

void
oam_pm_pdcp_read_linked_list_kpi_thp_file_dummy
(
    UInt16 *mac_str_count
);

void
oam_pm_pdcp_perf_read_linked_list_ue_stats_file_dummy
(
    UInt16 *mac_str_count
);

void
oam_pm_write_ho_related_params_to_file( UInt16 counter);

oam_return_et
oam_pm_mac_read_linked_list_kpi
(
    oam_mac_kpi_stats_ind_params_t *p_mac_kpi_stats_ind_params,
    UInt16 *mac_str_count 
); 


oam_return_et
oam_pm_update_oam_periodic_info_t
(
     oam_periodic_info_t *p_periodic_info,
     UInt16               sampling_intrvl_val,
     UInt16               upload_intrvl_val
);

oam_return_et
oam_send_get_rlc_stats_req
(
     UInt16            src_module_id,
     UInt16            transaction_id,
     oam_error_code_et *p_error_code
);

oam_return_et
oam_handle_rlc_get_stats_cnf
(
    void              *p_api_buf,
    /*SPR 17777 fix*/
    UInt16             transaction_id,
    UInt16              dest_id,
    oam_error_code_et *p_error_code

);

/* + SPR 17439 */
UInt8
get_cell_id(void);
/* - SPR 17439 */

oam_return_et
oam_hdle_rrc_config_resp
(   
    void         *p_api
    /*SPR 17777 fix*/
);

oam_return_et
oam_pm_send_rrc_kpi_config_req
(
/* SPR_12955_FIXES */
    UInt16  trans_id,
/* SPR_12955_FIXES */
    oam_error_code_et *p_error_code,
    UInt16             recv_bitmask
);

oam_return_et      
oam_compose_rrc_config_req
(
     oam_rrc_config_stats_req_t *p_rrc_config_stats_req,
     UInt16 cli_recv_bitmask,
     UInt8  *p_p_buffer
);

oam_return_et
oam_handle_rrc_cell_config_resp
(
    void    *p_api_buf,
    UInt16   transaction_id,
    UInt16   msg_len
);

oam_return_et
oam_handle_pdcp_get_stats_cnf
(
    void               *p_api_buf,
    /*SPR 17777 fix*/
    UInt16              transaction_id,
    UInt16              dest_id,
    oam_error_code_et  *p_error_code

);

oam_return_et
oam_handle_mac_get_stats_cnf
(
    void              *p_api_buf,                              
    /*SPR 17777 fix*/
    UInt16             transaction_id,
    UInt16             dest_id,
    oam_error_code_et *p_error_code
 
);

oam_return_et
oam_s1ap_handle_stats_resp
(
    void               *p_api,
    UInt16             trans_id,
    UInt16             dest_id,
    oam_error_code_et  *p_error_code
);

    /*SPR 17777 fix*/

oam_return_et
fill_name_value_pair_get_resp
(
    UInt8    *des_arr,
    UInt16   *ret_val_len,
    UInt8     name[],
    UInt16    val
);

oam_return_et
fill_parent_node_info
(   
    UInt8   *des_arr,
    UInt16   total_len_child_nodes,
    UInt8    total_num_param,
    UInt8    par_name[],
    UInt16  *ret_val_len
);  

oam_return_et
fill_common_resp_info
(
    UInt8  *des_arr,
    UInt8   status,
    UInt8   error_string[],
    UInt16 *ret_val_len

);

UInt16
cal_len_of_child_nodes
(
    UInt8  *p_child_node_buf[],
    UInt8   no_of_parameter
);

UInt16
cal_len_of_common_n_parent_node_info
(
    UInt8  *p_parent_node_name
);

oam_return_et
oam_fill_name_value_pair
(
    UInt8  *des_arr,
    UInt16 *ret_val_len,
    UInt8  *p_child_node_name_value_pair
);

typedef struct 
{
    UInt16 durationT;              /* KPI timer value */
    UInt8  periodicReporting;      /* Indicates periodic reporting required or not */
    UInt8  kpiToReportBitMap;      /* Each bit of this bitmap represents KPI */
    UInt16 rrc_upload_time_intrval;
}oam_rrc_configure_kpi_stats_req_params_t;

oam_return_et
oam_pm_update_rrc_kpi_info
(
    oam_rrc_configure_kpi_stats_req_params_t *p_rrc_config_kpi_stats_req, 
    UInt8                                     src_kpi_on_off_flag,
    UInt16                                    sampling_intrvl_val,
    UInt16                                    upload_intrvl_val
);

UInt8 
oam_bit_read_in_array_lsb
(
    UInt8  *bit_array,
    UInt16  bit_num
);

typedef struct
{
    UInt8 ind_bit_mask;
    void  *p_kpi_conn_buf;
    void  *p_kpi_erab_buf;
    void  *p_kpi_handover_buf;
    void  *p_kpi_paging_buf;
    void  *p_kpi_ue_assoc_buf;
    void  *p_kpi_irat_mobility_buf;
}oam_l3_kpi_ind_handler_t;

oam_l3_kpi_ind_handler_t g_l3_kpi_ind_handler;

typedef struct
{
    void *rrm_kpi_buf;
}oam_rrm_kpi_ind_handler_t; 

oam_rrm_kpi_ind_handler_t g_rrm_kpi_ind_handler;


/* SPR 14019 FIX Start */
#ifdef PERF_STATS
/* SPR 14019 FIX End */
typedef struct
{
    UInt8 config_ind_mask;
    void *p_pdcp_ue_buf;
    void *p_pdcp_cell_buf;
}oam_pdcp_perf_stats_ind_handler_t;
/* SPR 14019 FIX Start */
#endif
/* SPR 14019 FIX End */




oam_return_et
oam_rrc_kpi_stats_inds_handling
(
    void *p_api_buf   
);
void
oam_update_rrc_kpi_inds_handling_strct
(
/* + SPR 17439 */
 void 
/* - SPR 17439 */
);


oam_return_et
oam_rrc_parse_rrc_ue_assoc_s1_conn_stats
(
    rrc_ue_assoc_s1_conn_stats_t  *p_assoc_s1_conn_stats,
    UInt8                         *p_src,
    SInt32                         length_left,
    SInt32                        *p_length_read,
    oam_error_code_et             *p_error_code
);

oam_return_et
oam_rrc_parse_irat_mobility_stats
(
    oam_rrc_irat_mobility_stats_t  *p_irat_mobility_stats,
    UInt8                          *p_src,
    SInt32                          length_left,
    SInt32                         *p_length_read,
    oam_error_code_et              *p_error_code
);


oam_return_et
oam_rrc_parse_rrc_paging_stats
(
    oam_rrc_paging_stats_t  *rrc_paging_stats,
    UInt8                   *p_src,
    SInt32                   length_left,
    SInt32                  *p_length_read,
    oam_error_code_et       *p_error_code
);

oam_return_et
oam_parse_rrc_conn_stats_ind_t
(
    oam_rrc_connection_stats_ind_t  *p_conn_stats_ind,
    UInt8                           *p_src,
    SInt32                           length_left,
    SInt32                          *p_length_read,
    oam_error_code_et               *p_error_code
);


oam_return_et
oam_rrc_handover_stats_ind
(
    void               *p_api_buf,
    oam_error_code_et  *p_error_code
);

oam_return_et
oam_rrc_parse_handover_stats_ind
(
    oam_rrc_handover_stats_ind_t  *p_oam_rrc_handover_stats_ind,
    UInt8                         *p_src,
    SInt32                         length_left,
    SInt32                        *p_length_read,
    oam_error_code_et             *p_error_code
);

oam_return_et
oam_rrc_parse_handover_stats
(
    oam_rrc_handover_stats_t  *p_oam_rrc_handover_stats,
    UInt8                     *p_src,
    SInt32                     length_left,
    SInt32                    *p_length_read,
    oam_error_code_et         *p_error_code
);

oam_return_et
oam_rrc_parse_intra_enb_ho_stats
(
    oam_rrc_intra_enb_ho_stats_t  *p_oam_rrc_intra_enb_ho_stats,
    UInt8                         *p_src,
    SInt32                         length_left,
    SInt32                        *p_length_read,
    oam_error_code_et             *p_error_code
);

oam_return_et
oam_rrc_parse_ho_intraenboutatt_cause
(
    oam_rrc_ho_IntraEnbOutAtt_cause_t  *p_oam_rrc_ho_IntraEnbOutAtt_cause,
    UInt8                              *p_src,
    SInt32                             length_left,
    SInt32                             *p_length_read,
    oam_error_code_et                  *p_error_code
);

oam_return_et
oam_rrc_parse_ho_intraenboutsucc_cause
(
    oam_rrc_ho_IntraEnbOutSucc_cause_t  *p_oam_rrc_ho_IntraEnbOutSucc_cause,
    UInt8                               *p_src,
    SInt32                               length_left,
    SInt32                              *p_length_read,
    oam_error_code_et                   *p_error_code
);

oam_return_et
oam_rrc_parse_inter_enb_ho_stats
(
    oam_rrc_inter_enb_ho_stats_t *p_oam_rrc_inter_enb_ho_stats,
    UInt8                        *p_src,
    SInt32                        length_left,
    SInt32                       *p_length_read,
    oam_error_code_et            *p_error_code
);

oam_return_et
oam_rrc_parse_ho_interenboutatt_cause
(
    oam_rrc_ho_InterEnbOutAtt_cause_t  *p_oam_rrc_ho_InterEnbOutAtt_cause,
    UInt8                              *p_src,
    SInt32                              length_left,
    SInt32                             *p_length_read,
    oam_error_code_et                  *p_error_code
);

oam_return_et
oam_rrc_parse_ho_interenboutsucc_cause
(
    oam_rrc_ho_InterEnbOutSucc_cause_t  *p_oam_rrc_ho_InterEnbOutSucc_cause,
    UInt8                               *p_src,
    SInt32                               length_left,
    SInt32                              *p_length_read,
    oam_error_code_et                   *p_error_code
);

oam_return_et
oam_rrc_parse_meas_on_neigh_cell_ho_stats_list
(
    oam_rrc_meas_on_neigh_cell_ho_stats_list_t  *p_oam_rrc_meas_on_neigh_cell_ho_stats_list,
    UInt8 		                                *p_src,
    SInt32                                       length_left,
    SInt32                                      *p_length_read,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_rrc_parse_meas_on_neigh_cell_ho_stats
(
    oam_rrc_meas_on_neigh_cell_ho_stats_t  *p_oam_rrc_meas_on_neigh_cell_ho_stats,
    UInt8                                  *p_src,
    SInt32                                  length_left,
    SInt32                                 *p_length_read,
    oam_error_code_et                      *p_error_code
);

oam_return_et
oam_rrc_parse_ho_outatttarget_cause
(
    oam_rrc_ho_OutAttTarget_cause_t  *p_oam_rrc_ho_OutAttTarget_cause,
    UInt8                            *p_src,
    SInt32                            length_left,
    SInt32                           *p_length_read,
    oam_error_code_et                *p_error_code
);

oam_return_et
oam_rrc_parse_ho_outsucctarget_cause
(
    oam_rrc_ho_OutSuccTarget_cause_t *p_oam_rrc_ho_OutSuccTarget_cause,
    UInt8                            *p_src,
    SInt32                            length_left,
    SInt32                           *p_length_read,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_rrc_parse_intra_freq_ho_stats
(
    oam_rrc_intra_freq_ho_stats_t *p_oam_rrc_intra_freq_ho_stats,
    UInt8                         *p_src,
    SInt32                         length_left,
    SInt32                        *p_length_read,
    oam_error_code_et             *p_error_code
);

oam_return_et
oam_rrc_parse_inter_freq_ho_stats
(
    oam_rrc_inter_freq_ho_stats_t *p_rrc_oam_inter_freq_ho_stats,
    UInt8                         *p_src,
    SInt32                         length_left,
    SInt32                        *p_length_read,
    oam_error_code_et             *p_error_code
);

oam_return_et
oam_rrc_parse_drx_non_drx_ho_stats
(
    oam_rrc_drx_non_drx_ho_stats_t  *p_oam_rrc_drx_non_drx_ho_stats,
    UInt8                           *p_src,
    SInt32                           length_left,
    SInt32                          *p_length_read,
    oam_error_code_et               *p_error_code
);

oam_return_et
oam_rrc_parse_inter_rat_ho_stats
(
    oam_rrc_inter_rat_ho_stats_t  *p_oam_rrc_inter_rat_ho_stats,
    UInt8                         *p_src,
    SInt32                         length_left,
    SInt32                        *p_length_read,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_rrc_parse_ho_iratoutatt_cause
(
    oam_rrc_ho_IratOutAtt_cause_t  *p_oam_rrc_ho_IratOutAtt_cause,
    UInt8                          *p_src,
    SInt32                          length_left,
    SInt32                         *p_length_read,
    oam_error_code_et              *p_error_code
);

oam_return_et
oam_rrc_parse_ho_iratoutsucc_cause
(
    oam_rrc_ho_IratOutSucc_cause_t  *p_oam_rrc_ho_IratOutSucc_cause,
    UInt8                           *p_src,
    SInt32                           length_left,
    SInt32                          *p_length_read,
    oam_error_code_et               *p_error_code
);

oam_return_et
oam_rrc_parse_target_id
(
    oam_rrm_target_id_t  *p_oam_rrm_target_id,
    UInt8                *p_src,
    SInt32                length_left,
    SInt32               *p_length_read,
    oam_error_code_et    *p_error_code
);

oam_return_et
oam_rrc_parse_enb_id
(
    oam_rrm_enb_id_t   *p_oam_rrm_enb_id_t,
    UInt8              *p_src,
    SInt32              length_left,
    SInt32             *p_length_read,
    oam_error_code_et  *p_error_code
);

oam_return_et
oam_rrc_parse_eutra_globalenb_ecgi
(
    oam_eutra_globalEnb_ecgi_t  *p_oam_eutra_globalEnb_ecgi,
    UInt8                       *p_src,
    SInt32                       length_left,
    SInt32                      *p_length_read,
    oam_error_code_et           *p_error_code
);

oam_return_et
oam_rrc_parse_plmn_id
(
    oam_plmn_id_t     *p_oam_plmn_id,
    UInt8             *p_src,
    SInt32             length_left,
    SInt32            *p_length_read,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_rrc_parse_mnc
(
    oam_mnc_t          *p_oam_mnc,
    UInt8              *p_src,
    SInt32              length_left,
    SInt32             *p_length_read,
    oam_error_code_et  *p_error_code
);

oam_return_et
oam_rrc_parse_rrm_tai_info
(
    oam_rrm_tai_info_t  *p_oam_rrm_tai_info,
    UInt8               *p_src,
    SInt32               length_left,
    SInt32              *p_length_read,
    oam_error_code_et   *p_error_code
);

oam_return_et
oam_rrc_parse_rnc_id
(
    oam_rrm_rnc_id_t   *p_oam_rrm_rnc_id,
    UInt8              *p_src,
    SInt32              length_left,
    SInt32             *p_length_read,
    oam_error_code_et  *p_error_code
);

oam_return_et
oam_rrc_parse_lai_info
(
    oam_rrm_lai_info_t  *p_oam_rrm_lai_info,
    UInt8               *p_src,
    SInt32               length_left,
    SInt32              *p_length_read,
    oam_error_code_et   *p_error_code
);

oam_return_et
oam_rrc_parse_cgi_info
(
    oam_rrm_cgi_info_t *p_oam_rrm_cgi_info,
    UInt8              *p_src,
    SInt32              length_left,
    SInt32             *p_length_read,
    oam_error_code_et  *p_error_code
);

oam_return_et
oam_mac_get_kpi_stats_req
(
    oam_mac_get_KPI_stats_req_t  *p_mac_get_kpi_stats_req,
    UInt16                        src_module_id,
    UInt16                        transaction_id,
    oam_error_code_et            *p_error_code
);

void
oam_set_bit_in_array_lsb
(
    UInt8  *bit_array,
    UInt16  bit_num,
    UInt16  array_size
);

void
oam_pm_l2_file_close
(
/* + SPR 17439 */
 void 
/* - SPR 17439 */
);

oam_return_et
oam_send_rlc_reset_stats_req
(
    UInt16              src_module_id,
    UInt16              transaction_id,
    oam_error_code_et  *p_error_code
);

/*spr 19296 fix start*/
    /*code deleted*/
/*spr 19296 fix end*/
oam_return_et
oam_handle_rlc_reset_stats_cnf
(
    void  *p_api_buf
);

/*SPR 10023 FIX*/
/* SPR_12955_FIXES */	
oam_return_et
oam_pdcp_get_kpi_direct_thp_stats_cnf(UInt16 trans_id);
/* SPR_12955_FIXES */	

/*SPR 10023 FIX*/

oam_return_et
oam_rrc_parse_connection_related_stats
(
    rrc_connection_stats_t  *p_oam_rrc_connection_related_stats,
    UInt8                   *p_src,
    SInt32                   length_left,
    SInt32                  *p_length_read,
    oam_error_code_et       *p_error_code
);

oam_return_et
oam_rrc_parse_non_periodic_erab_related_stats
(
    oam_rrc_erab_related_stats_t  *p_oam_rrc_erab_related_stats,
    UInt8                         *p_src,
    SInt32                         length_left,
    SInt32                        *p_length_read,
    oam_error_code_et             *p_error_code
); 

oam_return_et
oam_rrc_parse_non_periodic_handover_stats
(
     rrc_oam_handover_related_stats_t *p_oam_rrc_handover_stats,
     UInt8                            *p_src,
     SInt32                            length_left,
     SInt32                           *p_length_read,
     oam_error_code_et                *p_error_code
);

UInt32 
cal_len_non_periodic_cell_stats_resp
(
/* + SPR 17439 */
 void 
/* - SPR 17439 */
);
/* SPR 17777 fix */
oam_return_et
oam_rrc_fill_non_periodic_get_cell_stats_resp
(   
    oam_rrc_get_cell_stats_resp_t  *oam_rrc_get_cell_stats_resp,
    UInt32                         *str_count_2
);

UInt32
cal_size_x_of_pdcp_ue_cell_resp
(
    UInt32  oam_num_Ue_Present
);

UInt32
cal_size_x_pdcp_ue_resp
(
/* + SPR 17439 */
 void 
/* - SPR 17439 */
);

oam_return_et
oam_rrc_parse_stat_erab_sim_num_stats
(
    rrc_uecc_oam_stat_erab_number_t  *p_oam_rrc_stat_erab_number,
    UInt8                            *p_src,
    SInt32                            length_left,
    SInt32                           *p_length_read,
    oam_error_code_et                *p_error_code
);

oam_return_et
oam_rrc_parse_rrc_connEstabTime_t
(
    rrc_connEstabTime_t  *p_oam_rrc_connEstabTime,
    UInt8                *p_src,
    SInt32                length_left,
    SInt32               *p_length_read,
    oam_error_code_et    *p_error_code
);

oam_return_et
oam_rrc_parse_erab_estabTime
(
    oam_rrc_erab_estabTime_t  *p_oam_rrc_oam_rrc_erab_estabTime,
    UInt8                     *p_src,
    SInt32                     length_left,
    SInt32                    *p_length_read,
    oam_error_code_et         *p_error_code
);

/* SPR 14019 FIX Start */
#ifdef PERF_STATS
/* SPR 14019 FIX End */
oam_return_et
oam_pdcp_handle_ue_cell_perf_stats_cnf
(
/* + SPR 17439 */
 void 
/* - SPR 17439 */
);
/* SPR 14019 FIX Start */
#endif
/* SPR 14019 FIX End */

UInt32
cal_size_x_of_ue_cell_resp
(
	UInt32  numOfUePresent
);

typedef struct
{
    /*CSR00074708 FIX*/
    UInt64 RRC_ConnSum;
    /*CSR00074708 FIX*/
    UInt32 RRC_ConnMax[MAX_NUM_CELLS];
    UInt32 RRC_Conn_Count;
    UInt32 RRC_ConnEstabTime_Count;
    UInt32 RRC_ConnEstabTimeSum[5];
    UInt32 RRC_ConnEstabTimeMax[5];
    UInt32 ERAB_EstabTimeSum_QCI[10];
    UInt32 ERAB_EstabTimeMax_QCI[10];
    UInt32 ERAB_EstabTime_Count[10];
    UInt32 ERAB_UsageNbr_Count[10];
    UInt32 ERAB_UsageNbrMax_QCI[10];
    UInt32 ERAB_UsageNbrSum_QCI[10];
}oam_rrc_params_t;

typedef struct
{
    UInt32 DL_Cell_Control_Plane_SDU_Bit_Rate_Count;
    UInt32 UL_Cell_Control_Plane_SDU_Bit_Rate_Count;
    UInt64 DL_Cell_Control_Plane_SDU_Bit_Rate_Sum;  
    UInt64 UL_Cell_Control_Plane_SDU_Bit_Rate_Sum; 
}oam_pdcp_params_t;

typedef struct
{
    oam_bool_et     periodic_reporting_ongoing;
    oam_bool_et     pm_xml_file_ready_to_upload;
    /**
    ** 0x00 - no SPV received from upload_interval_timer/
              upload_timer/file_write_timer/sampling_interval_timer
    ** 0x01 - pm_xml_upload_interval_timer
    ** 0x02 - pm_xml_upload_timer
    ** 0x04 - pm_xml_sampling_interval_timer
    **/
    UInt8            spv_bitmask;

    /* pm xml file writer pointer */
    xmlTextWriterPtr pm_xml_file_writer;

    /* pm xml file reporting enabled */
    UInt8            pm_xml_file_reporting;

    /* upload interval timer */
    UInt64           pm_xml_upload_interval_timer;

    /* upload timer */
    UInt64           pm_xml_upload_timer;
    
    /* file write time */
    UInt64           pm_xml_file_write_timer;
    
    /* Sampling interval */
    UInt16           pm_xml_sampling_interval_timer;

    char             periodic_upload_timer_str[OAM_MAX_TIME_LENGTH];
    /* SPR 14997 START */
    Char8            pm_xml_filename[60];
    //Char8            pm_xml_filename[50];
    /* SPR 14997 END */
    /* CSPL related timer ids */
    QTIMER           pm_xml_upload_interval_timer_id;
    QTIMER           pm_xml_upload_timer_id;
    QTIMER           pm_xml_file_write_timer_id;
    
    int              is_pm_xml_alarm_raised;
    int              pm_xml_upload_interval_timer_diff;
    oam_rrc_params_t rrc_params;
    oam_pdcp_params_t pdcp_params;
}oam_pm_xml_params_t;

typedef struct
{
    oam_bool_et rrm_periodic_reporting_ongoing;
    UInt64      rrm_periodic_duration;
    UInt64           pm_rrm_xml_file_write_timer;
    Char8            pm_rrm_xml_filename[50];
    QTIMER      pm_rrm_xml_file_write_timer_id;
}oam_rrm_xml_params_t;

typedef struct
{
    UInt64  ho_OutSuccTarget_Cause_radioNetwork;
    UInt64  ho_OutSuccTarget_Cause_transport;
    UInt64  ho_OutSuccTarget_Cause_nas;
    UInt64  ho_OutSuccTarget_Cause_protocol;
    UInt64  ho_OutSuccTarget_Cause_misc;

    UInt64  ho_OutSuccTarget_Cause_sum;

    UInt64  ho_OutAttTarget_Cause_radioNetwork;
    UInt64  ho_OutAttTarget_Cause_transport;
    UInt64  ho_OutAttTarget_Cause_nas;
    UInt64  ho_OutAttTarget_Cause_protocol;
    UInt64  ho_OutAttTarget_Cause_misc;

    UInt64  ho_OutAttTarget_Cause_sum;
}oam_eutran_causes_t;
oam_eutran_causes_t oam_eutran_causes;

/* SPR 17777 fix */
oam_return_et
oam_pm_rrc_read_linked_list_kpi_file
(
    oam_pm_rrc_kpi_info_Node_t *oam_pm_rrc_kpi_node
);
/* SPR 17777 fix */
oam_return_et
oam_pm_xml_file_open
(
    void
);

oam_return_et
oam_send_rrc_reset_config_request
(
    void  *p_api_buf,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_send_pdcp_reset_config_request
(
/* + SPR 17439 */
 void 
/* - SPR 17439 */
);
oam_return_et
oam_send_mac_reset_config_request
(
/* + SPR 17439 */
 void 
/* - SPR 17439 */
);

oam_return_et
oam_handle_rrc_stats_n_reset_req
(
    void  *p_api_buf
);

oam_return_et
oam_handle_rrc_reset_resp
(
    void  *p_api_buf,
    oam_error_code_et *p_error_code
);

		/* SPR_12955_FIXES */
oam_return_et oam_pm_xml_handle_spv_for_pm_params
(
	UInt16 trans_id
);
		/* SPR_12955_FIXES */
oam_return_et
oam_pm_timer_stop
(
    QTIMER  timer_id
);

oam_return_et
oam_send_config_gpv_resp
(
/* + SPR 17439 */
 void 
/* - SPR 17439 */
);

void 
oam_pm_write_xml_param_name
(
    xmlTextWriter  *writer, 
    char           **value_xml_param
);

oam_return_et 
oam_pdcp_handle_kpi_inds
(
    void  *p_api_buf
);

void 
oam_pm_xml_handle_file_upload_response
(
    UInt8  upload_status
);

oam_return_et
oam_mac_handle_kpi_inds
(
    void  *p_api_buf
);

oam_return_et
oam_send_ConfigNumberOfEntries_gpv_resp
(
 UInt16  trans_id,
 UInt16   dest_id 
);

/* SPR 17777 fix */
oam_return_et
oam_add_in_rrm_link_list
(
/* + SPR 17439 */
 oam_rrm_kpi_info_t *p_rrm_kpi_info
 /* - SPR 17439 */
);

void
oam_pm_rrm_read_data_from_link_list
(
/* + SPR 17439 */
 void 
/* - SPR 17439 */
);
oam_return_et
oam_pm_rrm_read_linked_list_kpi_file
(
    oam_rrm_kpi_ind_t *p_oam_pm_rrm_kpi_ind
);
#ifdef OAM_SON_ENABLE
oam_return_et
oam_handle_son_oam_mro_ho_counter_report
(
 son_oam_mro_ho_counter_report_t *p_mro_ho_counter_report
    /*SPR 17777 fix*/
);
#endif
oam_return_et
oam_handle_rrm_get_kpi_resp
(
 rrm_oam_get_kpi_resp_t *p_rrm_oam_get_kpi_resp,
    /*SPR 17777 fix*/
 UInt16 transaction_id,
 UInt16 dest_id,
 oam_error_code_et *p_error_code
 );

oam_return_et
oam_pm_rrm_send_config_req
(
   UInt8 index,
/* SPR_12955_FIXES */	
   UInt16 trans_id,
/* SPR_12955_FIXES */	
   oam_error_code_et *p_error_code
);

/* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED

oam_return_et oam_handle_m2ap_get_stat_resp(
        m2ap_oam_get_stat_resp_t *p_oam_m2ap_get_stat_resp,
        UInt16 transaction_id,
        UInt16 dest_id,
        oam_error_code_et *p_error_code
        );


#endif
/* EMBMS Changes End */

oam_return_et
oam_pm_rrm_kpi_stat_ind
(
 void* p_api_buf,
 oam_rrm_kpi_ind_t *p_KPI_stats_ind
);
oam_return_et
oam_rrm_kpi_ind_handle
(
    void *p_api_buf
);

oam_return_et
oam_s1ap_send_stats_ind
(
    UInt16     src_module_id,
    UInt16     transaction_id,
    oam_error_code_et *p_error_code
);

/* LWA changes start*/
/* LWA changes end*/

/* SPR 14019 FIX Start */
#ifdef PERF_STATS
/* SPR 14019 FIX End */
oam_return_et
oam_pm_pdcp_config_perf_stats_req
(
 oam_pdcp_config_perf_stats_req_t *p_oam_pdcp_config_perf_stats_req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 );

oam_return_et
oam_pm_handle_pdcp_config_perf_stats_cnf
(
 void *p_api_buf
 );

oam_return_et
oam_pm_pdcp_ue_perf_stat_ind
(
 void *p_api_buf,
 oam_pdcp_ue_perf_stats_ind_t *p_ue_perf_ind    
 );


oam_return_et oam_pdcp_handle_perf_stats_inds
(
 void *p_api_buf
 );

oam_return_et
oam_pm_pdcp_perf_read_linked_list_ue_stats_file
(
 oam_pdcp_ue_perf_stats_ind_t *p_oam_pdcp_ue_perf_stats,
 UInt16 *pdcp_perf_str_count 
 );



oam_return_et
oam_pm_pdcp_cell_perf_stat_ind
(
 void *p_api_buf,
 oam_pdcp_cell_perf_stats_ind_t *p_cell_perf_ind
 );
oam_return_et
oam_pm_pdcp_send_config_perf_stats_req
(
 oam_error_code_et *p_error_code
 );


/* SPR 14019 FIX Start */
#endif
/* SPR 14019 FIX End */
oam_return_et 
oam_set_bit_in_var
(
 UInt8   *p_bit_var,
 UInt16   bit_num
 );

void fill_plmn_id_from_mcc_mnc
(
    UInt8 out_plmn_id[],
    plmn_identity_t in_plmn_id
);
UInt32
oam_convert_byte_to_int(
    UInt8 source[]
);

/* CSR No 00055803 Start */
oam_return_et
oam_handle_kpi_config_success_fail_resp_handle
(
   void  *p_api_buf,
   UInt32 bit_mask   
);
/* CSR No 00055803 End */
#ifdef OAM_SON_ENABLE
oam_return_et
oam_handle_son_pci_conflict_confusion_ind
(
   oam_son_pci_conflict_confusion_ind_t *p_pci_conflict_confusion,
   oam_error_code_et *p_error_code	
);
#endif

/* SPR 11120 FIX start*/
/* + SPR 17439 */
void
oam_pm_stop_all_timer(void);
/* - SPR 17439 */
/* SPR 11120 FIX end*/
/*SPR 10023 start*/

/* SPR 19351 Code Removed */

/* SPR 14019 FIX Start */
#ifdef PERF_STATS
/* SPR 14019 FIX End */
oam_return_et
oam_handle_get_ue_pdcp_perf_stats_batch_data
(
 void *p_api_buf,
 oam_error_code_et *p_error_code,
 SInt8 *UE_count
 );
/* SPR 14019 FIX Start */
#endif
/* SPR 14019 FIX End */

/* SPR 14504 FIX START */
#define OAM_PM_MAX_IRAT_INC_MOB_LAI_CNT             4
/* SPR 14504 FIX END */
/* + SPR 10023 */
oam_return_et
oam_handle_get_pdcp_thp_batch_data
(
 void *p_api_buf,
 oam_error_code_et *p_error_code
 );

oam_return_et
oam_handle_pdcp_thp_batch_data
(
 void *p_api_buf,
 /* SPR 22296 Fix Start */
 oam_error_code_et *p_error_code,
 UInt8 cell_id
 /* SPR 22296 Fix End */
 );

void
add_to_pdcp_thp_batch_link_list
(
 oam_pdcp_KPI_THP_stats_ind_t *p_oam_pdcp_kpi_thp_stats_ind,
 /* SPR 22296 Fix Start */
 oam_error_code_et *p_error_code,
 UInt8 cell_id
 /* SPR 22296 Fix End */
 );

void
add_to_get_pdcp_thp_batch_link_list
(
 oam_pdcp_KPI_THP_stats_ind_t *p_oam_pdcp_kpi_thp_stats_ind,
 oam_error_code_et *p_error_code
 );
/* SPR 14019 FIX Start */
#ifdef PERF_STATS
/* SPR 14019 FIX End */
void
add_to_get_ue_pdcp_perf_stats_batch_link_list
(
 oam_pdcp_get_ue_perf_stats_cnf_t *p_oam_ue_pdcp_ue_perf_stats,
 oam_error_code_et *p_error_code
 );
/* SPR 14019 FIX Start */
#endif
/* SPR 14019 FIX End */


void
delete_pdcp_batch_data_from_link_list
(
 SInt32 api_id
);

oam_pdcp_thp_batch_node *
insert_batch_data_in_kpi_node
(
    oam_pdcp_thp_batch_node **p_KPI_THP_stats_ind,
    oam_error_code_et *p_error_code
);

void
delete_pdcp_batch_node_from_pdcp_kpi_node
(
    oam_pm_pdcp_kpi_info_node_t *pdcp_node
);

/* SPR 14019 FIX Start */
#ifdef PERF_STATS
/* SPR 14019 FIX End */
/* + SPR 17439 */
void
delete_pdcp_perf_stats_data_from_link_list(void);
/* - SPR 17439 */
/* SPR 14019 FIX Start */
#endif
/* SPR 14019 FIX End */
/*SPR 10023 end*/

/* SPR_12747_CHANGES */
void 
	oam_pm_write_ho_target_param
(
 oam_pm_rrc_kpi_info_Node_t *oam_pm_rrc_kpi_node,
 /* SPR 22296 Fix Start */
 oam_counter_t loop_counter,
 UInt8 rrc_cell_id
 /* SPR 22296 Fix End */
 );
/* SPR_12747_CHANGES */

#endif /* OAM_PM_H */
