/*! \file son_types.h
 *  \brief This is the file containing structure definitions common to
 *         SONApps, SONES, SONANR modules.
 *  \date February 20, 2012
 *  \author Shilpi Jain
 */
/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_TYPES_H_
#define _SON_TYPES_H_

/*! \headerfile son_mif_intf.h <>
 */
#include <son_mif_intf.h>

/*! \headerfile son_oam_intf.h <>
 */
#include <son_oam_intf.h>

/*! \headerfile rrm_son_intf.h <>
 */
#include <rrm_son_intf.h>

/*! \headerfile son_nmm_apps_intf.h <>
 */
#include <son_nmm_apps_intf.h>

/*! \headerfile son_list.h <>
 */
#include <son_list.h>

/*! \headerfile son_hash.h <>
 */
#include <son_hash.h>

/*! \def ZERO_BYTE_OFFSET
 *  \brief This macro for zero byte offset
 */
#define ZERO_BYTE_OFFSET  0


/*! \def FIRST_BYTE_OFFSET 
 *  \brief This macro for one byte offset
 */
#define FIRST_BYTE_OFFSET  1

/*! \def SECOND_BYTE_OFFSET
 *  \brief This macro for two byte offset
 */
#define SECOND_BYTE_OFFSET 2

/*! \def THIRD_BYTE_OFFSET
 *  \brief This macro for four byte offset
 */
#define FOURTH_BYTE_OFFSET 4

/*! \def FIFTH_BYTE_OFFSET
 *  \brief This macro for five  byte offset
 */
#define FIFTH_BYTE_OFFSET  5

/*! \enum son_app_mode_et
 *  \brief An enum that specifies the ES/ANR FSM state
 */

/*! \def MIN_PENDING_RES_COUNT
 *  \brief Minimum number of pending responses
 */
#define MIN_PENDING_RES_COUNT   0

/*! \def MAX_PENDING_RES_COUNT
 *  \brief Maximum number of pending responses
 */
#define MAX_PENDING_RES_COUNT   255

/*! \def SON_UNDEFINED
 *  \brief Undefined Value
 */
#define SON_UNDEFINED 0xFFFF

/*! \def SON_MAX_TEMP_TXN_ID
 *  \brief Maximum number of transaction Ids that SON shall store during cell deletion process */
#define SON_MAX_TEMP_TXN_ID 20

/*! \enum son_app_mode_e
 *  \brief The enum indicates application(ES/ANR) mode
 */
typedef enum son_app_mode_e
{
    SON_ENABLED = 0,
    SON_DISABLED,
    SON_MODE_UNDEFINED = 0xFF
}son_app_mode_et;

/*! \enum mlb_nbr_cell_state_et
 *  \brief The enum indicates the MLB state for a neighbor cell to MRO such that MRO and MLB do not update the Cell Individual Offset simultaneously for the neighbor cell
 */
typedef enum _mlb_nbr_cell_state_e
{
    MLB_NBR_STATE_DISABLED = 0,
    MLB_NBR_STATE_ENABLED,
    MLB_NBR_STATE_UNDEFINED = 0xFF
}mlb_nbr_cell_state_et;

/*! \  struct cell_status_list_data_t
 *  \brief cell status linked list nodes data definition
 *  \param cell_status_node     YLNODE structure storing the next and
 *                              previous pointer to the list nodes
 *  \param cell_status          Data stored in the list nodes
 */
typedef struct _cell_status_list_data_t
{
    SON_LIST_NODE cell_status_node;
    son_cell_status_t cell_status;
}cell_status_list_data_t;

typedef time_t time_stamp_t;

/*! \  struct pending_res_t
 *  \brief Structure to store information related to the cells with pending
 *          response messages
 *  \param transaction_id       Transaction Identifier to map the req/res
 *                              messages
 *  \param expected_res_count   Expected number of pending response
 *  \param api_id               Procedure code (API Id)
 *  \param is_error_received    If error had been received in any response
 *  \param ts                   Current system time when entry is created
 *  \param cell_sts_list        List containing status of cells for which
 *                              register/derester response is pending
 */
typedef struct _pending_res_t
{
    son_u16 transaction_id;
    son_u8 expected_res_count;
    son_procedure_code_et api_id;
    son_bool_et is_error_received; /*coverity_CID_21637 fix start-end*/
    time_stamp_t ts;
    SON_LIST cell_sts_list;
}pending_res_t;

/*! \  struct son_rrm_register_req_t
 *  \brief Structure storing the Register Req message data
 *  \param transaction_id       Transaction Identifier to map the req/res
 *                              messages
 *  \param req                  Register Request message data
 */
typedef struct _son_rrm_register_req_t
{
    son_u16 transaction_id;
    rrm_son_register_req_t req;
}son_rrm_register_req_t;

/*! \  struct son_rrm_deregister_req_t
 *  \brief Structure storing the Deregister Req message data
 *  \param transaction_id       Transaction Identifier to map the req/res
 *                              messages
 *  \param req                  Deregister Request message data
 */
typedef struct _son_rrm_deregister_req_t
{
    son_u16 transaction_id;
    rrm_son_deregister_req_t req;
}son_rrm_deregister_req_t;

/*! \  struct son_rrm_set_attribute_value_req_t
 *  \brief Structure storing the Set Attribute Value Req message data
 *  \param transaction_id       Transaction Identifier to map the req/res
 *                              messages
 *  \param req                  Set Attribute Value Req message data
 */
typedef struct _son_rrm_set_attribute_value_req_t
{
    son_u16 transaction_id;
    rrm_son_set_attribute_value_req_t req;
}son_rrm_set_attribute_value_req_t;

/*! \  struct son_rrm_registration_res_t
 *  \brief Structure storing the Registration message data
 *  \param transaction_id       Transaction Identifier to map the req/res
 *                              messages
 *  \param res                  Register Response from RRM
 */
typedef struct _son_rrm_registration_res_t
{
    son_u16 transaction_id;
    rrm_son_registration_res_t res;
}son_rrm_registration_res_t;

/*! \  struct son_rrm_deregistration_res_t
 *  \brief Structure storing the Deregistration message data
 *  \param transaction_id       Transaction Identifier to map the req/res
 *                              messages
 *  \param res                  DeRegister Response from RRM
 */
typedef struct _son_rrm_deregistration_res_t
{
    son_u16 transaction_id;
    rrm_son_deregistration_res_t res;
}son_rrm_deregistration_res_t;

/*! \  struct son_rrm_set_attribute_res_t
 *  \brief Structure storing the Set-attribute Res
 *         message data
 *  \param transaction_id       Transaction Identifier to map the req/res
 *                              messages
 *  \param res                  Set-Attribute Response from RRM
 */
typedef struct _son_rrm_set_attribute_res_t
{
    son_u16 transaction_id;
    rrm_son_set_attr_res_t res;
}son_rrm_set_attribute_res_t;

/*! \  struct son_generic_response_t
 *  \brief   Generic Message Response Structure
 *  \param cell_id                          CGI of cell
 *  \param result                           Result
 *  \param error_code                       Error Code
 */

typedef struct _generic_response_with_cell_id_internal_t
{
    son_intra_rat_global_cell_id_t cell_id;
    son_return_et result;
    son_error_et error_code;
}generic_response_with_cell_id_internal_t;

/*! \  struct generic_response_t
 *  \brief   Generic Message Response Structure
 *  \param transaction_id               Transaction Identifier
 *  \param generic_res                  struct generic_res
 */

typedef struct _son_generic_response_with_cell_id_t
{
    son_u16 transaction_id;
    generic_response_with_cell_id_internal_t generic_res;
}son_generic_response_with_cell_id_t;

/*! \  struct generic_response_internal_t
 *  \brief Generic internal response structure storing result and error code
 *  \param result       Result (SON_SUCCESS, SON_FAILURE etc)
 *  \param error_code   Error cause
 */
typedef struct _generic_response_internal_t
{
    son_return_et result;
    son_error_et error_code;
}generic_response_internal_t;

/*! \  struct generic_response_t
 *  \brief  Generic Response Message Structure
 *  \param transaction_id                   Transaction Identifier
 *  \param generic_res                      struct generic_res
 */
typedef struct _son_generic_response_id_t
{
    son_u16 transaction_id;
    generic_response_internal_t generic_res;
}son_generic_response_t;

/*! \  son_mlb_config_param_t
 *  \brief Structure storing MLB cell specific configuration parameter
 *  \param max_ho_sample                Maximum Handover reports to trigger Idle mode MLB algo.n in equation (RSRP(i) / n >= THrsrp).
 *  \param q_offset_min                 Min value allowed for Qoffset.
 *  \param q_offset_max                 Max value allowed for Qoffset.
 *  \param eutra_threshold_rsrp         THrsrp in equation (RSRP(i) / n >= THrsrp)
 *  \param small_step_cell_offset       value of 'A' in equation (Qoffset,new = A * RSRP(C1) + Qinitial)
 *  \param eutra_load_threshold_idle    Pth,idle in idle mode MLB algo
 *  \param eutra_load_threshold_conn    Pth,conn in connected mode MLB algo
 *  \param eutra_offset_min_step        Qoffset,min,step_size
 *  \param eutra_ocn_min_step           OCNmin,step_size
 *  \param num_of_mlb_iteration         load balancing iterations till serving cell load does not change
 *  \param mlb_start_threshold          The threshold available capacity of serving cell to trigger MLB algo
 *  \param mlb_stop_threshold           The threshold available capacity of serving cell to stop MLB algo
 *  \param min_capacity_gain            Min increase in serving cell available capacity after offset change
 *  \param mlb_mode                     SON Mode type: Centralized or Distributed
 */
typedef struct _son_mlb_config_param_t
{
    son_u32 max_ho_sample;   /*n in equation (RSRP(i) / n >= THrsrp)*/
    son_q_offset_range_et q_offset_min;
    son_q_offset_range_et q_offset_max;
    son_u8 eutra_threshold_rsrp; /* THrsrp in equation (RSRP(i) / n >= THrsrp)*/
    son_u8 small_step_cell_offset; /* value of 'A' in equation (Qoffset,new = A * RSRP(C1) + Qinitial)*/
    son_u8 eutra_load_threshold_idle; /*Pth,idle*/
    son_u8 eutra_load_threshold_conn; /*Pth,conn*/
    son_q_offset_range_et eutra_offset_min_step; /*Qoffset,min,step_size */
    son_q_offset_range_et eutra_ocn_min_step; /* OCNmin,step_size */
    son_u8 self_load_report_interval;    /* Self load report interval */
    son_u8 nbr_load_report_interval;     /* X2 load report interval */
    son_u8 num_of_mlb_iteration; /*load balancing iterations till serving cell load does not change*/
    son_u8 mlb_start_threshold; /*the threshold available capacity of serving cell to trigger MLB algo*/
    son_u8 mlb_stop_threshold; /*the threshold available capacity of serving cell to stop MLB algo*/
    son_u8 min_capacity_gain; /*min increase in serving cell available capacity after offset change */
    son_mode_et mlb_mode;
}son_mlb_config_param_t;

/*! \  son_mro_file_config_param_t
 *  \brief Structure storing MRO cell specific configuration parameter read from file
 *  \param mro_mode                     Mode of SON MRO feature i.e. Centralized, Distributed.
 *  \param mro_param                    Parameter determines whether TTT or/and CIO is to be optimized based on certain criteria.
 *  \param mro_ho_failure_accumulation_duration Duration for which the HO failure counters are accumulated to analyze HO failures and then only if required HO parameter optimization is done.
 *  \param mro_ho_attempts_threshold    Minimum number of handover attempts to be made during the Handover Failure Accumulation duration to trigger HO optimization
 *  \param mro_ho_optimization_start_threshold  Minimum number of handover attempts to be made during the Handover Failure Accumulation duration to trigger HO optimization
 *  \param mro_ho_optimization_stop_threshold   HO optimization shall stop if total handover failure rate is less than or equal to this value. It is a Total HO Failure rate which includes TOO Early HO Failure, TOO Late HO Failure and To Wrong Cell Failure rate for Source cell and Target cell pair
 *  \param mro_ho_fail_rate_for_optimization    HO Failures are clubbed into two following pairs .
 * o  "Too early HO failure rate" + "HO to wrong cell for Failure cell" failure rate
 * o  "Too late HO failure rate" + "HO to wrong cell for re-establishment cell" failure rate.
 * Tuning of the Mobility Parameter (CIO/TTT) shall only be performed if failure rate of any of the above pair is higher or equals to this parameter (i.e. Handover Fail Rate for Optimization). Therefore, this is an additional condition which must be met for the tuning of the mobility parameters after the HO optimization is started
 *  \param ttt                          Default value of the TTT configured at SON RRM for the cell
 *  \param delta_cio                    Step value for increasing / decreasing CIO
 *  \param delta_ttt                    Step value for increasing / decreasing TTT
 *  \param mro_expected_ho_improvement_for_single_step  Expected improvement in HO Failure rate after tuning CIO/TTT by Delta CIO/Delta TTT
 *  \param min_ttt                      Minimum valid TTT value
 *  \param max_ttt                      Maximum valid TTT value
 *  \param min_cio                      Minimum valid CIO value
 *  \param max_cio                      Maximum valid CIO value
 */
typedef struct _son_mro_file_config_param_t
{
    son_mode_et mro_mode;
    son_mro_param_et mro_param;
    son_u8 mro_ho_failure_accumulation_duration;
    son_u16 mro_ho_attempts_threshold;
    son_u16 mro_ho_optimization_start_threshold;
    son_u16 mro_ho_optimization_stop_threshold;
    son_u16 mro_ho_fail_rate_for_optimization;
    son_time_to_trigger_range_et ttt;
    son_u8 delta_cio;
    son_u8 delta_ttt;
    son_u16 mro_expected_ho_improvement_for_single_step;
    son_time_to_trigger_range_et min_ttt;
    son_time_to_trigger_range_et max_ttt;
    son_q_offset_range_et min_cio;
    son_q_offset_range_et max_cio;
}son_mro_file_config_param_t;

/*! \sonapps_icic_config_params_t
 *  \brief Structure to store icic configuration parameters
 *  \param dl_tx_alpha downlink  transmission
 *  \param dl_tx_beta  downlink  transmission
 *  \param ul_tx_alpha uplink transmission
 *  \param ul_offset_k uplink offset
 */
typedef struct _sonapps_icic_config_params_t
{
    son_u8 dl_tx_alpha;
    son_u8 dl_tx_beta;
    son_u8 ul_tx_alpha;
    son_u8 ul_offset_k;
}sonapps_icic_config_params_t;

/*! \  son_logger_cfg_t
 *  \brief This struct is used for storing params from cfg
 *  \param wait_interval Interval for which logger will wait incase no log is available
 *  \param log_path Path on which log files will be created
 *  \param shm_size Shared memory size in MB
 *  \param log_file_sz Log file size in MB
 *  \param log_chunk_cnt Number of logs to be read from a single segment in each cycle
 *  \param log_cpu_bitmap The bitmap to specify the cores on which the logger thread will be executed. 
 */
typedef struct _son_logger_cfg_t
{
    son_u16     log_wait_interval;
    son_u8      log_path[SON_MAX_LOG_PATH_STR_LEN];
    son_u32     log_shm_size;
    son_u32     log_file_sz;
    son_u16     log_chunk_cnt;
    son_u32     log_cpu_bitmap;
    son_u8      log_file_cnt;
}son_logger_cfg_t;


/*! \son_rach_opt_read_only_config_param_t
 *  \brief This structure stores the RACH OPT read only configuration parameters.
 *  \param rachopt_strong_cell_count                        Parameter that defines the count of strongest neighbor cells be reported by the UEs in the UE reports being sent to RACHOPT by RRM.
 *  \param rachopt_ra_preambles_size_change_step            Parameter to increase or decrease the number of non-dedicated random access preambles (Contention based) in a single step.
 *  \param rachopt_ra_group_a_size_change_step              Parameter to increase or decrease the number of Contention based preambles for Group A in a single step. 
 *  \param rachopt_no_of_windows_to_skip_for_optimization   Parameter that defines the number of periods (i.e. number of RRM reports) to skip after cell is reconfigured with new values. This is to get the stable RACH counters for RACH-OPT algorithm.
 */
typedef struct _son_rach_opt_read_only_config_param_t
{
    son_u8  rachopt_strong_cell_count;
    son_u8  rachopt_no_of_windows_to_skip_for_optimization;
    son_u8  rachopt_grpAConfig_vs_grpAobsrvd_diff_threshold;
    son_u8  rachopt_cfra_unused_threshold;
}son_rach_opt_read_only_config_param_t;

/*! \son_rachopt_config_param_t
 *  \brief This structure stores the RACH-OPT configuration parameters.
 *  \param rach_cell_level_config_params    RACH OPT cell level config parameters which can be modified at run time.
 *  \param read_only_config_params          RACH OPT read only config parameters which can't be modified at run time.
 *  \param rachopt_mode                     RACH OPT mode (DISTRIBUTED/CENTRALIZED)
 */
typedef struct _son_rachopt_config_param_t
{
    son_rach_opt_oam_config_param_t         rach_cell_level_config_params;
    son_rach_opt_read_only_config_param_t   read_only_config_params;
    son_mode_et                             rachopt_mode;
}son_rachopt_config_param_t;

/* Rel 3.0 Multi Sector code changes Start */
/*! \son_anr_config_param_t
 *  \brief This structure stores the ANR configuration parameters.
 *  \param anr_auto_maintain_local_cell_as_nbr Parameter deciding whether ANR shall maintain the serving cell as neighbor cell in the NRT of all other serving cells configured at ANR or not. 
 *  \param anr_ps_ho_supported indicates whether PS HO is supported for this cell or not.
 *  \param anr_voip_capable indicates whether this cell is voip capable or not.
 */
typedef struct _son_anr_config_param_t
{
    son_bool_et anr_auto_maintain_local_cell_as_nbr;
    /*SRVCC-Start*/
    son_bool_et anr_ps_ho_supported;
    son_bool_et anr_voip_capable;
    /*SRVCC-End*/
    son_u8      anr_max_num_cgi_request;  /*Added for SPR-9759*/
}son_anr_config_param_t;

/* Rel 3.0 Multi Sector code changes Stop */
/*! \son_global_config_params_t
 *  \brief Global structure for configuration parameters to be filled
 * from xml file
 *  \param mro_config_params Variable for MRO configuration parameters
 *  \param mlb_config_params Variable for MLB configuration parameters
 *  \param icic_config_param Variable for ICIC configuration parameters
 *  \param son_logger_cfg Variable for logging configuration parameters
 *  \param anr_config_params    Variable for ANR configuration parameters
 */
typedef struct _son_global_config_params_t
{
    son_mro_file_config_param_t             mro_config_params;
    son_mlb_config_param_t                  mlb_config_params;
    sonapps_icic_config_params_t            icic_config_param;
    son_rachopt_config_param_t              rach_config_params;
    son_logger_cfg_t                        son_logger_cfg;
/* Rel 3.0 Multi Sector code changes Start */
    /*SRVCC-Start*/
    son_anr_config_param_t                  anr_config_params;
    /*SRVCC-End*/
/* Rel 3.0 Multi Sector code changes Stop */
}son_global_config_params_t;


/*! \  son_cell_state_change_indication_t from 
    rrm_cell_state_change_indication_t*/
typedef rrm_son_cell_state_change_ind_t son_cell_state_change_indication_t;

/*! \  struct son_cell_info_ind
 *  \brief cell info indication structure
 *  \param transaction_id   Transaction Identifier
 *  \param p_cell_list      Pointer to cell info list
 */
typedef son_oam_cell_info_ind_t son_cell_info_ind_t;

/*! \struct son_ho_failure_report_t
 *  \brief  Typedef son_ho_failure_report_t from rrm_son_ho_failure_report_t
 */
typedef rrm_son_ho_failure_report_t son_ho_failure_report_t;

/*! \struct mlb_nbr_cell_status_ind_t
 *  \brief Message data used to have MRO - MLB synchronization such that the Cell Individual Offset is modified by only one of the module at a time.
 *  \param serv_cell_id         Serving cell GCI
 *  \param nbr_cell_id          Neighbor cell CGI
 *  \param nbr_cell_state       MLB state for neighbor cell
 */
typedef struct _mlb_nbr_cell_status_ind_t
{
    son_intra_rat_global_cell_id_t serv_cell_id;
    son_intra_rat_global_cell_id_t nbr_cell_id;
    mlb_nbr_cell_state_et nbr_cell_state;
}mlb_nbr_cell_status_ind_t;

/* Rel 3.0 Cell delete support changes Start */
/*! \  struct son_cell_delete_resp_t
 *  \brief Structure for storing cell delete resp in MIF global context
 *  \param transaction_id       Transaction Identifier
 *  \param cell_delete_resp     Cell delete resp structure
 */
typedef struct _son_cell_delete_resp_t
{
    son_u16                     transaction_id;
    son_oam_cell_delete_resp_t  cell_delete_resp;
}son_cell_delete_resp_t;

/* Rel 3.0 Cell delete support changes Stop */

#endif /* _SON_TYPES_H_ */
