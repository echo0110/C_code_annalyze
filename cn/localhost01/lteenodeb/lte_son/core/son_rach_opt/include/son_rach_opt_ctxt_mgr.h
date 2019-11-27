/*! \file son_rach_opt_ctxt_mgr.h
 *  \brief This file contains the prototypes of functions defined in son_rach_opt_ctxt_mgr.c
 *  \date  Aug, 2013
 *  \author Shilpi
 */
 /*!
 *  \copyright {Copyright (c) 2013, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_RACH_OPT_CTXT_MGR_H_
#define _SON_RACH_OPT_CTXT_MGR_H_

/*! \headerfile son_rach_opt_intf.h <>
 */
#include <son_rach_opt_intf.h>

/*! \headerfile son_defines.h <>
 */
#include <son_defines.h>

/*! \headerfile son_types.h <>
 */
#include <son_types.h>

/*! \headerfile son_oam_intf.h <>
 */
#include <son_oam_intf.h>

/**
 * A constant global array SON_RACH_OPT_FSM_STATES_NAMES
 * \brief RACH OPT FSM State Names
 */
extern const son_s8 *SON_RACH_OPT_FSM_STATES_NAMES[];

/** A constant global array SON_RACH_OPT_CELL_FSM_STATES_NAMES
 * \brief RACH OPT Cell FSm State Names
 */
extern const son_s8 *SON_RACH_OPT_CELL_FSM_STATES_NAMES[];

/** A constant global array SON_RACH_OPT_CELL_FSM_EVENT_NAMES
 * \brief RACH OPT Cell Fsm Event Names
 */
extern const son_s8 *SON_RACH_OPT_CELL_FSM_EVENT_NAMES[];

/*! \def SONRACH_OPT_FSM_SET_STATE(curr_state, new_state)
 *  \brief This macro is used to update the state of Global FSM of SON RACH OPT
 */
#define SONRACH_OPT_FSM_SET_STATE(curr_state, new_state) \
    do \
    { \
        SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,\
                SON_INFO,\
                "### RACH_FSM: Change State %s -> %s ###",\
                SON_RACH_OPT_FSM_STATES_NAMES[curr_state],\
                SON_RACH_OPT_FSM_STATES_NAMES[new_state]);\
        curr_state = new_state; \
    } \
    while (0)

/*! \def SONRACH_OPT_CELL_FSM_SET_STATE(p_cell_ctxt,prev_state,curr_state, new_state)
 *  \brief This macro is used to update the state of SONRACH_OPT CELL FSM
 */
#define SONRACH_OPT_CELL_FSM_SET_STATE(p_cell_ctxt, prev_state, curr_state, \
        new_state) \
do \
{ \
    SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, \
            SON_INFO, \
            "### RACH_Cell_FSM: Cell 0x%x Change State %s -> %s ###", \
            son_convert_char_cell_id_to_int_cell_id( \
            p_cell_ctxt->cell_id.cell_identity), \
            SON_RACH_OPT_CELL_FSM_STATES_NAMES[curr_state], \
            SON_RACH_OPT_CELL_FSM_STATES_NAMES[new_state]); \
    prev_state = curr_state; \
    curr_state = new_state; \
} \
while (0)

/* SPR-12708 Fix Starts */
/*! \def RACH_OPT_DEFULT_PENDING_RESPONSE_TIMER
 *  \brief This macro stores default value of the time after which the pending 
 response list is cleaned automatically by RACH OPT. The value is 1 min i.e 
 60 s
 */
#define RACH_OPT_DEFULT_PENDING_RESPONSE_TIMER                  60
/* SPR-12708 Fix Ends */

/*! \def RACH_OPT_DEFAULT_MODE
 *  \brief This macro stores default value of rachopt_mode parameter.
 */  
#define RACH_OPT_DEFAULT_MODE                               SON_MODE_DISTRIBUTED

/*! \def RACH_OPT_DEFAULT_REPORT_PERIOIDICITY 
 *  \brief This macro stores default value of rachopt_report_periodicity parameter.
 */
#define RACH_OPT_DEFAULT_REPORT_PERIOIDICITY                    60
#define RACH_OPT_MIN_REPORT_PERIOIDICITY                        1
#define RACH_OPT_MAX_REPORT_PERIOIDICITY                        600

/*! \def RACH_OPT_DEFAULT_MIN_UE_RACH_REPORT_COUNT
 *  \brief This macro stores default value of rachopt_min_ue_rach_report_count parameter.
 */
#define RACH_OPT_DEFAULT_MIN_UE_RACH_REPORT_COUNT               100
#define RACH_OPT_MIN_POSSIBLE_MIN_UE_RACH_REPORT_COUNT          10
#define RACH_OPT_MAX_POSSIBLE_MIN_UE_RACH_REPORT_COUNT          1000

/*! \def RACH_OPT_DEFAULT_UE_RACH_ATTEMPTS_THRESHOLD
 *  \brief This macro stores default value of rachopt_ue_rach_attempts_threshold parameter.
 */
#define RACH_OPT_DEFAULT_UE_RACH_ATTEMPTS_THRESHOLD             2
#define RACH_OPT_MIN_UE_RACH_ATTEMPTS_THRESHOLD                 2
#define RACH_OPT_MAX_UE_RACH_ATTEMPTS_THRESHOLD                 10

/*! \def RACH_OPT_DEFAULT_RACH_START_THRESHOLD
 *  \brief This macro stores default value of rachopt_start_threshold parameter.
 */
#define RACH_OPT_DEFAULT_RACH_START_THRESHOLD                   1000
#define RACH_OPT_MIN_RACH_START_THRESHOLD                       1
#define RACH_OPT_MAX_RACH_START_THRESHOLD                       1000

/*! \def RACH_OPT_DEFAULT_RACH_STOP_THRESHOLD
 *  \brief This macro stores default value of rachopt_stop_threshold parameter.
 */
#define RACH_OPT_DEFAULT_RACH_STOP_THRESHOLD                    500
#define RACH_OPT_MIN_RACH_STOP_THRESHOLD                        1
#define RACH_OPT_MAX_RACH_STOP_THRESHOLD                        1000

/*! \def RACH_OPT_DEFAULT_RACH_DEGRADE_STOP_OFFSET
 *  \brief This macro stores default value of rachopt_degrade_stop_offset parameter.
 */
#define RACH_OPT_DEFAULT_RACH_DEGRADE_STOP_OFFSET               300    
#define RACH_OPT_MIN_RACH_DEGRADE_STOP_OFFSET                   1   
#define RACH_OPT_MAX_RACH_DEGRADE_STOP_OFFSET                   1000    

/*! \def RACH_OPT_DEFAULT_MAX_ITERATION_TO_REACH_STOP_THRESHOLD 
 *  \brief This macro stores default value of rachopt_max_iterations_to_reach_stop_threshold parameter.
 */
#define RACH_OPT_DEFAULT_MAX_ITERATION_TO_REACH_STOP_THRESHOLD      5
#define RACH_OPT_MIN_POSSIBLE_MAX_ITERATION_TO_REACH_STOP_THRESHOLD 1
#define RACH_OPT_MAX_POSSIBLE_MAX_ITERATION_TO_REACH_STOP_THRESHOLD 10

/*! \def RACH_OPT_DEFAULT_CONTENTION_THRESHOLD
 *  \brief This macro stores default value of rachopt_contention_threshold parameter.
 */
#define RACH_OPT_DEFAULT_CONTENTION_THRESHOLD                   50
#define RACH_OPT_MIN_CONTENTION_THRESHOLD                       1
#define RACH_OPT_MAX_CONTENTION_THRESHOLD                       100

/*! \def RACH_OPT_DEFAULT_POWER_RAMPUP_THRESHOLD
 *  \brief This macro stores default value of rachopt_power_rampup_threshold parameter.
 */
#define RACH_OPT_DEFAULT_POWER_RAMPUP_THRESHOLD                 70
#define RACH_OPT_MIN_POWER_RAMPUP_THRESHOLD                     1
#define RACH_OPT_MAX_POWER_RAMPUP_THRESHOLD                     100

/*! \def RACH_OPT_DEFAULT_MIN_CFRA_PREAMBLES
 *  \brief This macro stores default value of rachopt_min_cfra_preambles parameter.
 */
#define RACH_OPT_DEFAULT_MIN_CFRA_PREAMBLES                     20
#define RACH_OPT_MIN_POSSIBLE_MIN_CFRA_PREAMBLES                4
#define RACH_OPT_MAX_POSSIBLE_MIN_CFRA_PREAMBLES                100

/*! \def RACH_OPT_DEFAULT_STRONG_CELL_COUNT
 *  \brief This macro stores default value of rachopt_strong_cell_count parameter.
 */
#define RACH_OPT_DEFAULT_STRONG_CELL_COUNT                      3
#define RACH_OPT_MAX_STRONG_CELL_COUNT                          3

/*! \def RACH_OPT_DEFAULT_NO_OF_WINDOWS_TO_SKIP_FOR_OPTIMIZATION
 *  \brief This macro stores default value of rachopt_no_of_windows_to_skip_for_optimization parameter.
 */
#define RACH_OPT_DEFAULT_NO_OF_WINDOWS_TO_SKIP_FOR_OPTIMIZATION 4
#define RACH_OPT_MAX_NO_OF_WINDOWS_TO_SKIP_FOR_OPTIMIZATION     20

/*! \def RACH_OPT_DEFAULT_GRPACONFIG_VS_GRPAOBSRVD_DIFF_THRESHOLD
 *  \brief This macro stores default value of rachopt_grpAConfig_vs_grpAobsrvd_diff_threshold parameter.
 */
#define RACH_OPT_DEFAULT_GRPACONFIG_VS_GRPAOBSRVD_DIFF_THRESHOLD    10
#define RACH_OPT_MIN_GRPACONFIG_VS_GRPAOBSRVD_DIFF_THRESHOLD        1
#define RACH_OPT_MAX_GRPACONFIG_VS_GRPAOBSRVD_DIFF_THRESHOLD        100

/*! \def RACH_OPT_DEFAULT_CFRA_UNUSED_THRESHOLD
 *  \brief This macro stores default value of rachopt_cfra_unused_threshold parameter.
 */
#define RACH_OPT_DEFAULT_CFRA_UNUSED_THRESHOLD                  10
#define RACH_OPT_MIN_CFRA_UNUSED_THRESHOLD                      1
#define RACH_OPT_MAX_CFRA_UNUSED_THRESHOLD                      100

/*! \def SON_UPDATE_PUSCH_HOPPING_OFFSET_PRESENT
 *  \brief This macro is used as a bitmask to store the presence of PUSCH hopping offset in cell info indication. This is used internally during handling of cell info indicaiton.
 */
#define SON_UPDATE_PUSCH_HOPPING_OFFSET_PRESENT                 0x800

#define RACH_OPT_DEFAULT_STEP_SIZE                               1

/*! \enum rach_opt_fsm_state_et
 *  \brief An enum that specifies the SON RACH OPT FSM states
 */
typedef enum
{
    RACH_OPT_STATE_INIT = 0,
    RACH_OPT_STATE_ACTIVE,
    RACH_OPT_STATE_ENABLING,
    RACH_OPT_STATE_DISABLING,
    RACH_OPT_STATE_SHUTDOWN,
    RACH_OPT_NUM_OF_STATES
}rach_opt_fsm_state_et;

/*! \enum rach_opt_cell_fsm_state_et
 *  \brief An enum that specifies the SON RACH OPT CELL FSM states
 */
typedef enum 
{
    RACH_OPT_CELL_STATE_DISABLED,
    RACH_OPT_CELL_STATE_ENABLING,
    RACH_OPT_CELL_STATE_ENABLED,
    RACH_OPT_CELL_STATE_DISABLING,
    RACH_OPT_CELL_NUM_OF_STATES
}rach_opt_cell_fsm_state_et;



/*! \ _rach_opt_algo_counters_context_t
 *  \brief Context containing counters for the RACH optimization algorithm
 *  \param samples_with_atmpt_more_than_thresh                 Number of RACH samples with RACH attempts more than configured threshold.
 *  \param samples_with_atmpt_more_than_one                    Number of RACH samples with RACH attempts more than 1.
 *  \param samples_with_contention_and_atmpt_more_than_thresh  Number of RACH samples with attempts more that configured threshold and contention.
 *  \param centre_samples_with_atmpt_more_than_thresh          Number of Centre RACH samples with attempts more than configured threshold.
 *  \param centre_samples_with_atmpt_more_than_thresh_and_no_contention Number of Centre RACH samples with no contention but attempts  more than configured threshold.
 *  \param centre_samples_count                                 Number of Centre RACH Samples.
 *  \param edge_samples_count                                   Number of Edge RACH Samples.
 *  \param percentage_of_samples_with_atmpt_more_than_thresh    Percentage of RACH samples with RACH attempts more than configured threshold.
                                                                (samples_with_atmpt_more_than_thresh/ total rach samples )*100.
 *  \param rach_failure_percentage                               RACH faiure percentage.(samples_with_atmpt_more_than_one/ total rach samples)*100
 *  \param contention_percentage                                (samples_with_contention_and_atmpt_more_than_thresh/
                                                                 samples_with_atmpt_more_than_thresh)* 100
 *  \param percentage_of_centre_samples_with_power_issue        (centre_samples_with_atmpt_more_than_thresh/
                                                                 centre_samples_count)*100
 *  \param total_unassigned_sum_cfra                                total unassigned SUM CFRA from L2 report via RRM
 *  \param percentage_of_centre_samples                          (centre_samples_count/total count)*100
 *  \param percentage_of_edge_samples                             (edge_samples_count/total count)*100
 */
typedef struct _rach_opt_algo_counters_context_t
{
    son_u16     samples_with_atmpt_more_than_thresh;
    son_u16     samples_with_atmpt_more_than_one;
    son_u16     samples_with_contention_and_atmpt_more_than_thresh;
    son_u16     centre_samples_with_atmpt_more_than_thresh;
    son_u16     centre_samples_with_atmpt_more_than_thresh_and_no_contention;
    son_u16     centre_samples_count;
    son_u16     edge_samples_count;
    son_float   percentage_of_samples_with_atmpt_more_than_thresh;
    son_float   rach_failure_percentage;
    son_float   contention_percentage;
    son_float   percentage_of_centre_samples_with_power_issue; 
    son_u64     total_unassigned_sum_cfra;
    /* SPR-20477 START */
    son_u64     num_of_group_a_preambles_observed;
    son_u64     num_of_group_b_preambles_observed;
    /* SPR-20477 END */
    son_float   percentage_of_centre_samples;
    son_float   percentage_of_edge_samples;
}rach_opt_algo_counters_context_t;


/*! \  rach_opt_oam_config_param_t
 *  \brief Strucutre storing various configurable parameters for RACH optimization. 
 *  \param rachopt_report_periodicity   For Distributed RACH OPT Mode, this parameter determines the interval of getting the UE Reports and L2 RACH stat reports from RRM and also defines the periodicity of running the RACH Optimization Algorithm. In case of Centralized Mode, this defines the periodicity of providing the RACH reports to Centralized SON entity.
 *  \param rachopt_min_ue_rach_report_count Minimum number of UE RACH sample requested from RRM during each RACH Report Period required for RACH optimization Algorithm. In case of Centralized mode, these samples will be sent to Centralized SON entity.
 *  \param rachopt_ue_rach_attempts_threshold   RACH Attempts Threshold above which a UE is considered to have RACH problem and is taken into consideration while analyzing RACH data for RACH parameter tuning
 *  \param rachopt_start_threshold  Threshold value of %UEs reporting RACH attempts greater than "rachopt_ue_rach_attempts_threshold" This condition triggers RACH optimization algorithm 
 *  \param rachopt_stop_threshold   Threshold value of % UEs reporting RACH attempts greater than "rachopt_ue_rach_attempts_threshold" Achieving this condition indicates objective of RACH algo is met and it should stop now.
 *  \param rachopt_degrade_stop_offset Percentage offset at which RACHOPT algo stops and notifies to OAM about failure if current % of UEs reporting more than "rachopt_ue_rach_attempts_threshold" RACH attempts increases by .rachopt_degrade_stop_offset.
 *  \param rachopt_max_iterations_to_reach_stop_threshold   Parameter defining mmaximum iterations RACH optimization algo will attempt to reach the stop threshold. If stop threshold is not achieved in these iterations, failure is notified to OAM.
 *  \param rachopt_contention_threshold Parameter defining % of UEs that report RACH attempts more than rachopt_ue_rach_attempts_threshold as well as well as contentionDetected flag as true. This indicates excessive contention in network for RACH procedure.
 *  \param rachopt_power_rampup_threshold Parameter defining % of Cell Centre RACH samples indicating RACH attempts more than "rachopt_ue_rach_attempts_threshold" but no contention. This implies problem lies in intial received target power setting.
 *  \param rachopt_min_cfra_preambles   Parameter defines the limit for CF preambles below which RACH-OPT algo shall not reduce the CF preambles.
 */
typedef struct _rach_opt_oam_config_param_t
{
    son_u16     rachopt_report_periodicity;
    son_u16     rachopt_min_ue_rach_report_count;
    son_u8      rachopt_ue_rach_attempts_threshold;
    son_float   rachopt_start_threshold;
    son_float   rachopt_stop_threshold;
    son_float   rachopt_degrade_stop_offset;
    son_u8      rachopt_max_iterations_to_reach_stop_threshold;
    son_u8      rachopt_contention_threshold;
    son_u8      rachopt_power_rampup_threshold;
    son_u8      rachopt_min_cfra_preambles;
}rach_opt_oam_config_param_t;

/*! \  rach_opt_cell_context_t
 *  \brief RACH OPT cell Specific context
 *  \param cell_id                      ECGI of cell
 *  \param previous_cell_fsm_state      RACH OPT's FSM previous state of cell
 *  \param current_cell_fsm_state       RACH OPT's FSM current state of cell
 *  \param oam_config_params            RACH OPT's configuration parameters which can be modified by OAM
 *  \param read_only_config_params      Read only RACH OPT's configuration parameters
 *  \param preamble_initial_rcvd_target_power   Indicates baseline for computatiion of transmit power for random access transmission parameters.
 *  \param number_of_ra_preambles   Number of non dedicated random access preambbles (Contention based)
 *  \param   size_of_ra_group_a     Number of non dedicated random access preamble group A 
 *  \param   pusch_hopping_offset   This is used to compute the set of physical resources blocks to be used for transmission on PUSCH if Uplink freq hopping mode is enabled
 *  \param rrm_request_txid         Transaction Id of RACH config request sent to RRM
 *  \param dereg_invoking_api           API Id which triggered Deregistration off the cell
 *  \param modify_res_bitmask           Bitmask to be used while sending modify config params response to OAM
 *  \param is_rach_algo_started         Is RACH algo has started
 *  \param rach_algo_run_count_after_started  RACH algo run count since it is started.
 *  \param previous_rach_failure_percentage   RACH failure percentage from the previous optimization attempt.
 *  \param rach_algo_counters                 RACH algorithm counters data
 *  \param is_algo_counters_updated_from_rrm_ue_report  Flag to determine if the RACH algorithm counters are filled from RRM_SON_RACH_UE_INFO_IND.
 *  \param is_algo_counters_updated_from_rrm_l2_report  Flag to determine if the RACH algorithm counters are filled from RRM_SON_L2_RACH_PERF_REPORT.
 *  \param previous_optimization_type 
 *  \param rach_opt_strongest_nbr_cell_list
 *  \param is_cell_info_ind_awaited
 *  \param no_of_windows_to_be_skipped_for_optimization
 *  \param last_update_req_data
 *  \param scaled_threshold_values   Scaled threshold values. 
 *  \param  reports_ignored_in_cell_info_ind_awaited_state
 */
typedef struct _rach_opt_cell_context_t
{
    son_intra_rat_global_cell_id_t          cell_id;
    rach_opt_cell_fsm_state_et              previous_cell_fsm_state; 
    rach_opt_cell_fsm_state_et              current_cell_fsm_state; 
    rach_opt_oam_config_param_t             oam_config_params;
    son_rach_opt_read_only_config_param_t   read_only_config_params;
    son_u32                                 preamble_initial_rcvd_target_power;
    son_u8                                  number_of_ra_preambles;
    son_u8                                  size_of_ra_group_a;
    son_u8                                  pusch_hopping_offset;
    /* SPR-19827 START */
    son_u32                                 rrm_request_txid;
    /* SPR-19827 END */
    son_procedure_code_et                   dereg_invoking_api;
    son_u32                                 modify_res_bitmask;
    son_bool_et                             is_rach_algo_started;
    son_u8                                  rach_algo_run_count_after_started;
    son_float                               previous_rach_failure_percentage;
    rach_opt_algo_counters_context_t        rach_algo_counters; 
    son_bool_et                             is_algo_counters_updated_from_rrm_ue_report;
    son_bool_et                             is_algo_counters_updated_from_rrm_l2_report;
    son_rach_opt_identified_optimization_type_et previous_optimization_type;
    SON_LIST                                rach_opt_strongest_nbr_cell_list;
    son_bool_et                             is_cell_info_ind_awaited;
    son_u8                                  no_of_windows_to_be_skipped_for_optimization;
    son_oam_rach_opt_update_req_param_t     last_update_req_data;
    son_u16                                 new_rachopt_report_periodicity;
    son_u16                                 new_rachopt_min_ue_rach_report_count;
    son_u16                                 reports_ignored_in_cell_info_ind_awaited_state;
    /*SPR_19279_START */
    son_u8 cell_index;
    /*SPR_19279_END */
}rach_opt_cell_context_t;

typedef struct _rach_opt_strongest_cell_info_t
{
    son_u16 pci;
    son_u16 count;
}rach_opt_strongest_cell_info_t;

/*! \  rach_opt_global_context_t
 *  \brief RACH OPT Global Context structure
 *  \param current_fsm_state                Global RACH OPT FSM State
 *  \param log_mode                         log mode for RACH OPT module
 *  \param rach_opt_pending_res_timer_id    Pending respose Timer Identifier
 *  \param rach_opt_global_mode             Global RACH OPT Module Mode (Enabled or Disabled)
 *  \param rachopt_mode                     Determines if the RACH-OPT shall be functioning in Centralized Mode or Distributed Mode. Default Mode: Distributed
 *  \param rach_opt_cell_ctxt_list          RACH OPT cell context List
 *  \param log_level                        RACH OPT log level
 */
typedef struct _rach_opt_global_context_t
{
    rach_opt_fsm_state_et                       current_fsm_state;
    son_oam_log_on_off_et                       log_mode;
    son_timer_t                                 rach_opt_pending_res_timer_id;
    son_app_mode_et                             rach_opt_global_mode;
    son_mode_et                                 rachopt_mode;
    SON_LIST                                    rach_opt_cell_ctxt_list;
    /*SPR-11554-Start*/
    son_enb_type_et                             enb_type;
    /*SPR-11554-End*/
    /* SPR-13251 Fix Starts */
    son_u8                                      log_level;
    /* SPR-13251 Fix Ends */
}rach_opt_global_context_t;

/*! \  rach_opt_cell_context_node_t
 *  \brief RACH OPT Cell Context List's Node
 *  \param sNode    CSPL's Node
 *  \param data     RACH OPT cell Specific context
 */
typedef struct _rach_opt_cell_context_node_t
{
    SON_LIST_NODE           sNode;
    rach_opt_cell_context_t data;
}rach_opt_cell_context_node_t;

/*! \  rach_opt_strongest_nbr_cell_node_t
 *  \brief RACH OPT strongest neighbor cells List's Node
 *  \param sNode    CSPL's Node
 *  \param data     RACH OPT strongest neighbor cell information
 */
typedef struct _rach_opt_strongest_nbr_cell_node_t
{
    SON_LIST_NODE                   sNode;
    rach_opt_strongest_cell_info_t  data;
}rach_opt_strongest_nbr_cell_node_t;

/*! \fn son_return_et rach_opt_init_global_context ()
 *  \brief This function initializes the RACH OPT global context
 */
son_return_et
rach_opt_init_global_context
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t rach_opt_set_log_mode(son_oam_log_on_off_et log_mode)
 *  \brief This function sets the log mode for RACH OPT module
 *  \param log_mode      Log mode to be set
 */
son_void_t
rach_opt_set_log_mode
(
    son_oam_log_on_off_et log_mode
);

/*! \fn son_void_t rach_opt_print_global_context(const son_s8* p_son_method_name)
 *  \brief This function prints the RACH OPT global context.
 *  \param p_son_method_name  Method name passes as string
 */
son_void_t
rach_opt_print_global_context
(
    const son_8* p_son_method_name
);

/*! \fn son_void_t rach_opt_send_feature_state_change_ind(
 *  son_intra_rat_global_cell_id_t *p_cell_id, son_feature_state_et state,
 *  son_feature_state_change_cause_et cause)
 *  \brief This function creates and sends the feature state change indication 
 *  message to SONMgmtIfH.
 *  \param p_cell_id    Pointer to the cell Id
 *  \param state        RACH OPT state for the cell
 *  \param cause        Cause for the present state of RACH OPT for the cell
 */
son_void_t
rach_opt_send_feature_state_change_ind
(
    son_intra_rat_global_cell_id_t      *p_cell_id,
    son_feature_state_et                state,
    son_feature_state_change_cause_et   cause
);

/*! \fn son_void_t rach_opt_reset_pending_resp_list()
 *  \brief Resets RACH OPT pending_response structure
 */
son_void_t
rach_opt_reset_pending_resp_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn rach_opt_cell_context_node_t *rach_opt_add_mod_cell_context(son_cell_t 
 * p_cell, son_feature_state_et *p_state, son_feature_state_change_cause_et *p_cause)
 *  \brief This function adds new cell in the RACH OPT cell context or
 *                  modify the existing data of already configured cell
 *  \param p_cell   Pointer to the cell Information Structure
 *  \param p_state  RACH OPT feature state for the cell
 *  \param p_cause  Cause of failure if failure occurs in addition 
 */
rach_opt_cell_context_node_t *
rach_opt_add_mod_cell_context
(
    son_cell_t                          *p_cell,
    son_feature_state_et                *p_state,
    /*SPR_19279_START*/
    son_feature_state_change_cause_et   *p_cause,
    son_u8                              cell_index
    /*SPR_19279_END*/
);

/*! \fn son_return_et rach_opt_add_cell_in_pending_res (
 * son_cell_status_t *p_sts_node)
 *  \brief Add cell_status node in the RACH OPT pending response list
 *  \param p_sts_node   Pointer to Cell Status Node
 */
son_return_et
rach_opt_add_cell_in_pending_res
(
    son_cell_status_t *p_sts_node
);

/*! \fn son_u16 rach_opt_get_pending_res_transaction_id()
 *  \brief This function provides transaction id present in RACH OPT pending response
 */
son_u16
rach_opt_get_pending_res_transaction_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t rach_opt_init_pending_resp_list(son_u16 tid, 
 *      son_procedure_code_et api)
 *  \brief Initialize RACH OPT pending_response structure
 *  \param tid    Transaction Id
 *  \param api    Procedure Code
 */
son_void_t
rach_opt_init_pending_resp_list
(
    son_u16                  tid,
    son_procedure_code_et    api
);

/*! \fn son_u8 rach_opt_get_pending_res_expected_count()
 *  \brief Provides the expected response count present in rach_opt pending response
 list.
 */
son_u8
rach_opt_get_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_bool_et rach_opt_is_any_cell_disabled()
 *  \brief Checks if at least one cell exists which is in
 *         RACH_OPT_CELL_STATE_DISABLED or RACH_OPT_CELL_STATE_DISABLING state.
 *         If such cell is found it indicates that global enable
 *         request can be processed as some cells needs to be enabled.
 */
son_bool_et
rach_opt_is_any_cell_disabled
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*!  \fn son_bool_et rach_opt_is_any_cell_enabled()
 *  \brief Checks if at least one cell exists which is in
 *          RACH_OPT_CELL_STATE_ENABLED or RACH_OPT_CELL_STATE_ENABLING state.
 *          If such cell is found it indicates that global disable
 *          request can be processed as some cells needs to be disabled.
 */

son_bool_et
rach_opt_is_any_cell_enabled
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t
 * rach_opt_populate_enable_disable_res_data_from_pending_list
 * (son_u16 *p_cell_status_list_size,son_cell_status_t *p_cell_status_list
 *  \brief This function populates enable or disable response data
 *         struture with the cell Ids and the error codes of the cell
 *         for which (de)registration request could not be sent to
 *         RRM due to some SONRACH OPT error or may be because ANR mode is
 *         not enabled for the cell or cell context is not found at
 *         ANR.
 *  \param p_cell_status_list_size   Number of cells to be send in response
 *  \param p_cell_status_list        Pointer to list containg cell Ids and error
 *                                   code indicating the reason for failure.
 */
son_void_t
rach_opt_populate_enable_disable_res_data_from_pending_list
(
    son_u16            *p_cell_status_list_size,
    son_cell_status_t  *p_cell_status_list
);

/*! \fn rach_opt_cell_context_t *rach_opt_get_cell_context_from_global_context (
 * son_intra_rat_global_cell_id_t *p_cell_id)
 *  \brief This function gets the cell context of the specified cell
 * from RACH OPT's global context.
 *  \param p_cell_id    Pointer to the cell Id
 */
rach_opt_cell_context_t *
rach_opt_get_cell_context_from_global_context
(
    son_intra_rat_global_cell_id_t     *p_cell_id
);

/*! \fn rach_opt_cell_context_node_t *rach_opt_get_first_cell_ctxt()
 *  \brief This function gets the cell context of the first cell
 *         in the RACH OPT cell context list
 */
rach_opt_cell_context_node_t *
rach_opt_get_first_cell_ctxt
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn rach_opt_cell_context_node_t *rach_opt_get_next_node (rach_opt_cell_context_node_t
 * p_node)
 *  \brief This function gets the  next RACH OPT cell context in the list
 *  \param p_node   Pointer to the current cell context
 */
rach_opt_cell_context_node_t *
rach_opt_get_next_node
(
    rach_opt_cell_context_node_t *p_node
);

/*! \fn son_void_t 
 * rach_opt_populate_enable_disable_res_data_from_pending_list
 * (son_u16 *p_cell_status_list_size,son_cell_status_t *p_cell_status_list
 *  \brief This function populates enable or disable response data
 *         struture with the cell Ids and the error codes of the cell
 *         for which (de)registration request could not be sent to
 *         RRM due to some SONRACH OPT error or may be because ANR mode is
 *         not enabled for the cell or cell context is not found at
 *         ANR.
 *  \param p_cell_status_list_size   Number of cells to be send in response
 *  \param p_cell_status_list        Pointer to list containg cell Ids and error
 *                                   code indicating the reason for failure.
 */                                    
son_void_t
rach_opt_populate_enable_disable_res_data_from_pending_list
(
    son_u16            *p_cell_status_list_size,
    son_cell_status_t  *p_cell_status_list
);

/*!  \fn son_bool_et rach_opt_is_any_cell_enabled()
 *  \brief Checks if at least one cell exists which is in
 *          RACH_OPT_CELL_STATE_ENABLED or RACH_OPT_CELL_STATE_ENABLING state.
 *          If such cell is found it indicates that global disable
 *          request can be processed as some cells needs to be disabled.
 */

son_bool_et
rach_opt_is_any_cell_enabled
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_bool_et rach_opt_is_any_cell_disabled()
 *  \brief Checks if at least one cell exists which is in
 *         RACH_OPT_CELL_STATE_DISABLED or RACH_OPT_CELL_STATE_DISABLING state.
 *         If such cell is found it indicates that global enable
 *         request can be processed as some cells needs to be enabled.
 */
son_bool_et
rach_opt_is_any_cell_disabled
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_u8 rach_opt_get_pending_res_expected_count()
 *  \brief Provides the expected response count present in RACH OPT pending response
 list.
 */
son_u8
rach_opt_get_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_return_et rach_opt_decrement_pending_res_expected_count()
 *  \brief Decrement Expected Response Count present in pending
 *         response by 1.
 */
son_return_et
rach_opt_decrement_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_return_et rach_opt_increment_pending_res_expected_count()
 * \brief Increment Expected Response Count present in pending response by 1
 */
son_return_et
rach_opt_increment_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_bool_et rach_opt_delete_cell_from_context (
 * son_intra_rat_global_cell_id_t *p_cell_id)
 *  \brief This function deletes the specified cell context from the
 *         cell context list
 *  \param p_cell_id     Pointer to the cell Id
 */
son_bool_et
rach_opt_delete_cell_from_context
(
    son_intra_rat_global_cell_id_t     *p_cell_id
);

/*! \fn son_void_t rach_opt_delete_cell_from_pending_res(son_intra_rat_global_cell_id_t *p_cell_id) 
 *  \brief This function deletes node with given cell Id from the pendig reponse list
 *  \param p_cell_id    Pointer to the cell Id
 */
son_void_t
rach_opt_delete_cell_from_pending_res
(
    son_intra_rat_global_cell_id_t  *p_cell_id
);

/*! \fn son_void_t rach_opt_construct_cell_arr_from_pending_res_list (
 * son_u16 *p_cell_sts_arr_size, son_cell_status_t *p_cell_sts_arr,
 * rach_opt_cell_fsm_state_et cell_fsm_state)
 *  \brief Construct cell status array from pending response list
 *  \param p_cell_sts_arr_size  Pointer to cell status array size
 *  \param p_cell_sts_arr       pointer to cell status array
 *  \param cell_fsm_state       Cell FSM state to be set
 */
son_void_t
rach_opt_construct_cell_arr_from_pending_res_list
(
    son_u16                     *p_cell_sts_arr_size,
    son_cell_status_t           *p_cell_sts_arr,
    rach_opt_cell_fsm_state_et       cell_fsm_state
);

/*! \fn cell_status_list_data_t *rach_opt_find_cell_in_pending_res (
 * son_intra_rat_global_cell_id_t *p_rcvd_cell_id)
 *  \brief Find node of the pendign response list with given cell Id
 *  \param p_rcvd_cell_id   Pointer to ECGI of the cell
 */
cell_status_list_data_t *
rach_opt_find_cell_in_pending_res
(
    son_intra_rat_global_cell_id_t  *p_rcvd_cell_id
);

/*! \fn son_void_t rach_opt_clear_pending_res_list()
 *  \brief Delete all nodes from RACH OPT pending response list
 */
son_void_t
rach_opt_clear_pending_res_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t rach_opt_print_pending_res(const son_8* son_method_name)
 *  \brief Prints RACH OPT pending_response structure
 *  \param son_method_name  method name
 */
son_void_t
rach_opt_print_pending_res
(
    const son_8 *son_method_name
);

/*! \fn const son_void_t *son_key_of_rach_opt(const SON_LIST_NODE *p_node)
 *  \brief This function determines the key in RACH OPT
 *  \param p_node   Anchor field of the record
 */
const son_void_t *
son_key_of_rach_opt
(
    const SON_LIST_NODE *p_node
);
/*SPR_19279_START*/
/*! \fn const son_void_t *son_key_of_rach_opt(const SON_LIST_NODE *p_node)
 *  \brief This function determines the key in RACH OPT
 *  \param p_node   Anchor field of the record
 */
const son_void_t *
son_cell_index_key_of_rach_opt
(
    const SON_LIST_NODE *p_node
);
/*SPR_19279_END*/

/*! \fn son_procedure_code_et rach_opt_get_pending_res_api_id()
 *  \brief Get API ID present in pending response
 */
son_procedure_code_et
rach_opt_get_pending_res_api_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t rach_opt_init_rach_counters(rach_opt_cell_context_t *p_cell_context);
 *  \brief This function initialize the rach algo counters available at the cell context.
 *  \param p_cell_context - Pointer to the cell context structure
 */
son_void_t
rach_opt_init_rach_counters
(
 rach_opt_cell_context_t         *p_cell_context
);

/*! \fn rach_opt_strongest_nbr_cell_node_t *rach_opt_get_first_strongest_nbr_cell(SON_LIST *p_list)
 *  \brief This function gets the first node of strongest neighbor cell list.
 *  \param p_list Pointer to the strongest neighbor cell list
 */
rach_opt_strongest_nbr_cell_node_t *
rach_opt_get_first_strongest_nbr_cell
(
    SON_LIST *p_list
);

/*! \fn rach_opt_strongest_nbr_cell_node_t *rach_opt_get_next_strongest_nbr_cell(rach_opt_strongest_nbr_cell_node_t *p_node)
 *  \brief This function gets the next node in the strongest neighbor cell list.
 *  \param p_node   Pointer to the current node of list
 */
rach_opt_strongest_nbr_cell_node_t *
rach_opt_get_next_strongest_nbr_cell
(
    rach_opt_strongest_nbr_cell_node_t *p_node
);

/*! \fn son_void_t rach_opt_clear_strongest_nbr_cells_list(rach_opt_cell_context_t *p_cell_context) 
 *  \brief Delete all the nodes from strongest neighbor cells list.
 *  \param p_cell_context   Pointer to the RACH OPT cell context
 */
son_void_t
rach_opt_clear_strongest_nbr_cells_list
(
    rach_opt_cell_context_t *p_cell_context    
);

/*! \fn son_void_t rach_opt_clear_cell_contexts_list (son_u16 transaction_id)
 *  \brief Delete all the cell contexts from RACH OPT cell context list
 *  \param transaction_id   Transaction Identifier
 */
son_void_t
rach_opt_clear_cell_contexts_list
(
 void
    /*SPR 17777 +-*/
);

/*! \fn son_u64 rach_opt_get_total_cfra_preamble (son_u8 prach_config_index, son_u16 l2_perf_report_duration, son_oam_ra_preamble_count_et number_of_ra_preambles)
 *  \brief  
 *  \param prach_config_index       PRACH configuration index
 *  \param l2_perf_report_duration  duration for which L2 RACH performance reports are accumulated
 *  \param number_of_ra_preambles   Number of non dedicated random access preambbles (Contention based)
 *  \param duplex_mode              Duplex mode of the serving cell (FDD/TDD) 
 */
son_u64
rach_opt_get_total_cfra_preamble
(
    son_u8              prach_config_index,
    son_u16             l2_perf_report_duration,
    son_u8              number_of_ra_preambles,
    son_duplex_mode_et  duplex_mode
);

/*! \fn son_void_t rach_opt_print_strongest_nbr_cell_list(SON_LIST *p_list)
 *  \brief This function prints the data of strongest neighbor cell list
 *  \param p_list   Pointer to the strongest neighbor cell list
 */
son_void_t
rach_opt_print_strongest_nbr_cell_list
(
    SON_LIST *p_list
);

/*! \fn rach_opt_cell_fsm_state_et rach_opt_get_cell_mode(son_intra_rat_global_cell_id_t *p_cell_id)
 *  \brief This function find the cell FSM state for the given cell Id
 *  \param p_cell_id    Pointer to the cell Id
 */
rach_opt_cell_fsm_state_et
rach_opt_get_cell_mode
(
    son_intra_rat_global_cell_id_t *p_cell_id
);

/*! \fn son_u32 rach_opt_get_pending_res_cell_count();
 *  \brief This function gets number of cells available in pending response list
 */ 
son_u32
rach_opt_get_pending_res_cell_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t rach_opt_print_cell_context(const son_8 *son_method_name)
 *  \brief This function prints the RACH OPT Cell contexts list
 *  \param son_method_name  Invoking Method name
 */
son_void_t
rach_opt_print_cell_context
(
    const son_8 *son_method_name
);

/* SPR-13251 Fix Starts */
/*! \fn son_u8 rach_opt_get_log_level()
 *  \brief This function gets the log level
 */
son_u8
rach_opt_get_log_level
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t rach_opt_set_log_level(son_u8 log_level)
 *  \brief This function sets the log level
 *  \param log_level Log Level to be set
 */
son_void_t
rach_opt_set_log_level
(
    son_u8 log_level
);
/* SPR-13251 Fix Ends */

#endif
