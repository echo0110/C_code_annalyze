/*! \file son_mro_ctxt_mgr.h
 *  \brief This file contains the prototypes of functions defined in son_mro_ctxt_mgr.c
 *  \date  Dec, 2012
 *  \author Shilpi 
 */

 /*! 
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_MRO_CTXT_MGR_H_
#define _SON_MRO_CTXT_MGR_H_

/*! \headerfile son_mro_intf.h <>
 */
#include <son_mro_intf.h>

/*! \headerfile son_defines.h <>
 */
#include <son_defines.h>

/*! \headerfile son_types.h <>
 */
#include <son_types.h>

/*! \headerfile son_oam_intf.h <>
 */
#include <son_oam_intf.h>

/*! \headerfile son_anr_table.h <>
 */
#include <son_anr_table.h>

/**
 * A constant global array SON_MRO_FSM_STATES_NAMES
 * \brief MRO Fsm State Names
 */
extern const son_s8 *SON_MRO_FSM_STATES_NAMES[];

/** A constant global array SON_MRO_CELL_FSM_STATES_NAMES
 * \brief MRO Cell FSm State Names
 */
extern const son_s8 *SON_MRO_CELL_FSM_STATES_NAMES[];

/** A constant global array SON_MRO_CELL_FSM_EVENT_NAMES
 * \brief MRO Cell Fsm Event Names
 */
extern const son_s8 *SON_MRO_CELL_FSM_EVENT_NAMES[];

/*! \def SONMRO_FSM_SET_STATE(curr_state, new_state)
 *  \brief This macro is used to update the state of Global FSM of SONMRO
 */
#define SONMRO_FSM_SET_STATE(curr_state, new_state) \
    do \
    { \
        SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, \
            SON_INFO, \
            "### MRO FSM State changed %s->%s ###", \
            SON_MRO_FSM_STATES_NAMES[curr_state], \
            SON_MRO_FSM_STATES_NAMES[new_state]); \
        curr_state = new_state; \
    } \
    while (0)

/*! \def SONMRO_CELL_FSM_SET_STATE(p_cell_ctxt,prev_state,curr_state, new_state)
 *  \brief This macro is used to update the state of SONMRO CELL FSM
 */
#define SONMRO_CELL_FSM_SET_STATE(p_cell_ctxt, prev_state, curr_state, new_state) \
    do \
    { \
        SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, \
            SON_INFO, \
            "### MRO Cell FSM State changed for Cell 0x%x %s->%s ###", \
            son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity), \
            SON_MRO_CELL_FSM_STATES_NAMES[curr_state], \
            SON_MRO_CELL_FSM_STATES_NAMES[new_state]); \
        prev_state = curr_state; \
        curr_state = new_state; \
    } \
    while (0)

/*! \def MRO_SCALE_FACTOR
 * \brief This macro stores the scale factor which is used to convert the configuration parameters from float to interger and vice-versa.
 */
#define MRO_SCALE_FACTOR    100

/*! \def SON_MRO_DEFAULT_HO_FAILURE_ACCUMULATION_DURATION
 *  \brief This macro stores default value of Handover Failure Accumulation duration. Unit is minutes.
 */
#define SON_MRO_DEFAULT_HO_FAILURE_ACCUMULATION_DURATION    5

/*! \def SON_MRO_DEFAULT_DELTA_CIO
 *  \brief This macro stores default value of number of steps for increasing/
 * decreasing CIO. Default value is +1.
  */
#define SON_MRO_DEFAULT_DELTA_CIO                   1

/*! \def SON_MRO_DEFAULT_HO_ATTEMPTS_THRESHOLD
 *  \brief This macro stores default value of the Handover Attempts Threshold.
 */
#define SON_MRO_DEFAULT_HO_ATTEMPTS_THRESHOLD       10

/*! \def SON_MRO_DEFAULT_HO_OPTIMIZATION_START_THRESHOLD
 *  \brief This macro stores default value of HO Optimization Start Threshold.
 */
#define SON_MRO_DEFAULT_HO_OPTIMIZATION_START_THRESHOLD 0.1

/*! \def SON_MRO_DEFAULT_HO_OPTIMIZATION_STOP_THRESHOLD
 *  \brief This macro stores default value of HO Optimization Stop Threshold.
 */
#define SON_MRO_DEFAULT_HO_OPTIMIZATION_STOP_THRESHOLD  0.05

/*! \def SON_MRO_DEFAULT_HO_FAIL_RATE_FOR_OPTIMIZATION
 *  \brief This macro stores default value of Handover Fail Rate for Optimization.
 */
#define SON_MRO_DEFAULT_HO_FAIL_RATE_FOR_OPTIMIZATION   0.1

/*! \def SON_MRO_DEFAULT_EXPECTED_HO_IMPROVEMENT_FOR_SINGLE_STEP
 *  \brief This macro stores default value of expected HO improvement for single step.
 */
#define SON_MRO_DEFAULT_EXPECTED_HO_IMPROVEMENT_FOR_SINGLE_STEP 0.1

/*! \def SON_MRO_DEFAULT_DELTA_TTT
 *  \brief This macro stores default value of number of steps for increasing/
 * decreasing TTT. Default value is +1.
  */
#define SON_MRO_DEFAULT_DELTA_TTT                   1

/* SPR-12708 Fix Starts */
/*! \def MRO_DEFULT_PENDING_RESPONSE_TIMER
  *  \brief This macro stores default value of the time after which the pending response list is cleaned automatically by MRO. The default value is 1 min = 60s
  */
#define MRO_DEFULT_PENDING_RESPONSE_TIMER           60
/* SPR-12708 Fix Ends */

/*! \def MRO_MAX_RETRY_CNT
 *  \brief
 */
#define MRO_MAX_RETRY_CNT                           3

/*! \enum mro_cell_fsm_state_et
 *  \brief An enum that specifies the SON MRO CELL FSM states
 */
typedef enum 
{
    MRO_CELL_STATE_DISABLED,
    MRO_CELL_STATE_ENABLING,
    MRO_CELL_STATE_ENABLED,
    MRO_CELL_STATE_DISABLING,
    MRO_CELL_NUM_OF_STATES
}mro_cell_fsm_state_et;

/*! \enum mro_fsm_state_et
 *  \brief An enum that specifies the SON MRO FSM states
 */
typedef enum 
{
    MRO_STATE_INIT = 0,
    MRO_STATE_ACTIVE,
    MRO_STATE_ENABLING,
    MRO_STATE_DISABLING,
    MRO_STATE_SHUTDOWN,
    MRO_NUM_OF_STATES
}mro_fsm_state_et;

/*! \  son_mro_config_param_t
 *  \brief Structure storing MRO cell specific configuration parameter
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
typedef struct _son_mro_config_param_t
{
    son_mode_et mro_mode;
    son_mro_param_et mro_param;
    son_u8 mro_ho_failure_accumulation_duration;
    son_u16 mro_ho_attempts_threshold;
    son_float mro_ho_optimization_start_threshold;
    son_float mro_ho_optimization_stop_threshold;
    son_float mro_ho_fail_rate_for_optimization;
    son_time_to_trigger_range_et ttt;
    son_u8 delta_cio;
    son_u8 delta_ttt;
    son_float mro_expected_ho_improvement_for_single_step;
    son_time_to_trigger_range_et min_ttt;
    son_time_to_trigger_range_et max_ttt;
    son_q_offset_range_et min_cio;
    son_q_offset_range_et max_cio;
}son_mro_config_param_t;

/*! \  mro_cell_context_t
 *  \brief MRO cell Specific context
 *  \param cell_id                      ECGI of cell
 *  \param previous_cell_fsm_state      MRO's FSM previous state of cell
 *  \param current_cell_fsm_state       MRO's FSM current state of cell
 *  \param mro_config_param             MRO's configuration parameters
 *  \param dereg_invoking_api           API Id which triggered Deregistration off the cell
 *  \param ho_failure_accumulation_duration_timer_id    Timer Id of the HO failure acculation duration timer
 *  \param srvd_cell_algo_direction     Direction of optimization(too early or too late)
 *  \param is_ttt_algo_running          MRO algo running or not running status
 *  \param previous_ttt                 TTT value for the previous algo run
 *  \param retry_count                  Number of times TTT algo has given result less than mro_expected_ho_improvement_for_single_step
 *  \param previous_ho_fail_rate        HO Failure rate for the previous algo run
 *  \param is_ttt_update_reqd           To keep track if TTT update is required or not while the both CIO and TTT update option is selected for the algo
 */
typedef struct _mro_cell_context_t
{
    son_intra_rat_global_cell_id_t  cell_id;
    mro_cell_fsm_state_et           previous_cell_fsm_state; 
    mro_cell_fsm_state_et           current_cell_fsm_state; 
    son_mro_config_param_t          mro_config_param;
    son_procedure_code_et           dereg_invoking_api;
    son_timer_t ho_failure_accumulation_duration_timer_id;
    mro_optimization_dir_et srvd_cell_algo_direction;
    son_bool_et is_ttt_algo_running;
    son_time_to_trigger_range_et previous_ttt;
    son_u8 retry_count;
    son_float previous_ho_fail_rate;
    son_bool_et is_ttt_update_reqd;
   /*SPR_19279_START */
    son_u8 cell_index;
   /*SPR_19279_END */
}mro_cell_context_t;

/*! \  mro_global_context_t
 *  \brief MRO Global Context structure
 *  \param current_fsm_state            Global MRO FSM State
 *  \param log_mode                     log mode for MRO module
 *  \param mro_pending_res_timer_id     MRO pending register/deregister response Timer Identifier
 *  \param mro_config_param             MRO's configuration parameters
 *  \param mro_global_mode              Global MRO Mode (Enabled or Disabled)
 *  \param mro_cell_ctxt_list               MRO cell context List
 *  \param log_level                    MRO log level
 */
typedef struct _mro_global_context_t
{
    mro_fsm_state_et        current_fsm_state;  
    son_oam_log_on_off_et   log_mode;
    son_timer_t             mro_pending_res_timer_id;
    son_mro_config_param_t  mro_config_param;
    son_app_mode_et         mro_global_mode;
    SON_LIST                mro_cell_ctxt_list;
    /*SPR-11554-Start*/
    son_enb_type_et         enb_type;
    /*SPR-11554-End*/
    /* SPR-13251 Fix Starts */
    son_u8                  log_level;
    /* SPR-13251 Fix Ends */
}mro_global_context_t;

/*! \  mro_cell_context_node_t
 *  \brief MRO Cell Context List's Node
 *  \param sNode    CSPL's Node
 *  \param data     MRO cell Specific context
 */
typedef struct _mro_cell_context_node_t
{
    SON_LIST_NODE               sNode;
    mro_cell_context_t          data;
}mro_cell_context_node_t;

/*! \fn son_return_et mro_init_global_context ()
 *  \brief This function initializes the MRO global context
 */
son_return_et
mro_init_global_context
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t mro_set_log_mode(son_oam_log_on_off_et log_mode)
 *  \brief This function sets the log mode for MRO module
 *  \param log_mode      Log mode to be set
 */
son_void_t
mro_set_log_mode
(
    son_oam_log_on_off_et log_mode
);

/*! \fn son_app_mode_et mro_get_global_mode()
 *  \brief This function gets the MRO global mode (Enable/Disable)
 */
son_app_mode_et 
mro_get_global_mode
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_u16 mro_get_pending_res_transaction_id()
 *  \brief This function provides transaction id present in MRO pending ress
 ponse
  */
son_u16
mro_get_pending_res_transaction_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t mro_init_pending_resp_list(son_u16 tid, 
 *      son_procedure_code_et api)
 *  \brief Initialize MRO pending_response structure
 *  \param tid    Transaction Id
 *  \param api    Procedure Code
 */
son_void_t
mro_init_pending_resp_list
(
    son_u16                  tid,
    son_procedure_code_et    api
);

/*! \fn son_void_t mro_reset_pending_resp_list()
 *  \brief Resets MRO pending_response structure
 */
son_void_t
mro_reset_pending_resp_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn mro_cell_context_node_t *mro_add_new_cell_in_context(son_cell_t 
 * p_cell, son_feature_state_et *p_state, son_feature_state_change_cause_et *p_cause)
 *  \brief This function adds new cell in the MRO cell context
 *  \param p_cell   Pointer to the cell Information Structure
 *  \param p_state  MRO feature state for the cell
 *  \param p_cause  Cause of failure if failure occurs in addition 
 */
mro_cell_context_node_t *
mro_add_new_cell_in_context
(
    son_cell_t                          *p_cell,
    son_feature_state_et                *p_state,
    /*SPR_19279_START*/
    son_feature_state_change_cause_et   *p_cause,
    son_u8                              cell_index
    /*SPR_19279_END*/
);

/*! \fn const son_void_t *son_key_of_mro(const SON_LIST_NODE *p_node)
 *  \brief This function determines the key in MRO
 *  \param p_node   Anchor field of the record
 */
const son_void_t *
son_key_of_mro
(
    const SON_LIST_NODE *p_node
);
/*SPR_19279_START*/
/*! \fn const son_void_t *son_key_of_mro(const SON_LIST_NODE *p_node)
 *  \brief This function determines the key in MRO
 *  \param p_node   Anchor field of the record
 */
const son_void_t *
son_cell_index_key_of_mro
(
    const SON_LIST_NODE *p_node
);
/*SPR_19279_END*/


/*! \fn son_void_t mro_print_global_context(const son_8* son_method_name)
 *  \brief This function prints the MRO global context.
 *  \param son_method_name  Method mane passes as string
 */
son_void_t
mro_print_global_context
(
    const son_8 *son_method_name
);

/*! \fn mro_cell_context_node_t *mro_get_first_cell_ctxt()
 *  \brief This function gets the cell context of the first cell
 *         in the MRO cell context list
 */
mro_cell_context_node_t *
mro_get_first_cell_ctxt
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t mro_print_cell_context()
 *  \brief This function prints the MRO Cell contexts list
 */
son_void_t
mro_print_cell_context
(
/* + SPR 17439 */
const son_8 *son_method_name
/* - SPR 17439 */
);

/*! \fn mro_cell_context_node_t *mro_get_next_node (mro_cell_context_node_t 
 * p_node)
 *  \brief This function gets the  next MRO cell context in the list
 *  \param p_node   Pointer to the current cell context
 */
mro_cell_context_node_t *
mro_get_next_node
(
    mro_cell_context_node_t *p_node
);

/*! \fn son_return_et mro_add_cell_in_pending_res (
 * son_cell_status_t *p_sts_node)
 *  \brief Add cell_status node in the MRO pending response list
 *  \param p_sts_node   Pointer to Cell Status Node
 */
son_return_et
mro_add_cell_in_pending_res
(
    son_cell_status_t *p_sts_node
);

/*! \fn son_return_et mro_increment_pending_res_expected_count()
  * \brief Increment Expected Response Count present in pending response by 1
  */
son_return_et
mro_increment_pending_res_expected_count
(  
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_u8 mro_get_pending_res_expected_count()
 *  \brief Provides the expected response count present in MRO pending response            list.
 */
son_u8
mro_get_pending_res_expected_count
(  
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_bool_et mro_is_any_cell_disabled()
 *  \brief Checks if at least one cell exists which is in
 *         MRO_CELL_STATE_DISABLED or MRO_CELL_STATE_DISABLING state.
 *         If such cell is found it indicates that global enable
 *         request can be processed as some cells needs to be enabled.
 */
son_bool_et
mro_is_any_cell_disabled
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*!  \fn son_bool_et mro_is_any_cell_enabled()
  *  \brief Checks if at least one cell exists which is in
 *          MRO_CELL_STATE_ENABLED or MRO_CELL_STATE_ENABLING state.
 *          If such cell is found it indicates that global disable
 *          request can be processed as some cells needs to be disabled.
  */

son_bool_et
mro_is_any_cell_enabled
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t 
 * mro_populate_enable_disable_res_failure_data_from_pending_list
 * (son_u16 *p_cell_status_list_size,son_cell_status_t *p_cell_status_list
 *  \brief This function populates enable or disable response data
 *         struture with the cell Ids and the error codes of the cell
 *         for which (de)registration request could not be sent to
 *         RRM due to some SONMRO error or may be because ANR mode is
 *         not enabled for the cell or cell context is not found at
 *         ANR.
 *  \param p_cell_status_list_size   Number of cells to be send in response
 *  \param p_cell_status_list        Pointer to list containg cell Ids and error
 *                                   code indicating the reason for failure.
 */                                    
son_void_t
mro_populate_enable_disable_res_failure_data_from_pending_list
(
    son_u16            *p_cell_status_list_size,
    son_cell_status_t  *p_cell_status_list
);

/*! \fn mro_cell_context_t *mro_get_cell_context_from_global_context (
 * son_intra_rat_global_cell_id_t *p_cell_id)
 *  \brief This function gets the cell context of the specified cell 
 * from MRO's global context.
 *  \param p_cell_id    Pointer to the cell Id
 */
mro_cell_context_t *
mro_get_cell_context_from_global_context
(
    son_intra_rat_global_cell_id_t     *p_cell_id
);

/*! \fn son_void_t mro_clear_pending_res_list()
 *  \brief Delete all nodes from MRO pending response list
 */
son_void_t
mro_clear_pending_res_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t mro_clear_cell_contexts_list(son_u16 transaction_id)
 *  \brief Delete all the cell contexts from MRO cell context list
 *  \param transaction_id    Transaction Identifier
 */
son_void_t
mro_clear_cell_contexts_list
(
    son_u16 transaction_id
);

/*! \fn son_void_t mro_construct_cell_arr_from_pending_res_list (
 * son_u16 *p_cell_sts_arr_size, son_cell_status_t *p_cell_sts_arr,
 * mro_cell_fsm_state_et cell_fsm_state)
 *  \brief Construct cell status array from pending response list
 *  \param p_cell_sts_arr_size  Pointer to cell status array size
 *  \param p_cell_sts_arr       pointer to cell status array
 *  \param cell_fsm_state       Cell FSM state to be set
 */
son_void_t
mro_construct_cell_arr_from_pending_res_list
(
    son_u16                     *p_cell_sts_arr_size,
    son_cell_status_t           *p_cell_sts_arr,
    mro_cell_fsm_state_et       cell_fsm_state
);

/*! \fn son_void_t mro_print_pending_res(const son_8* son_method_name)
 *  \brief Prints MRO pending_response structure
 *  \param son_method_name  method name
 */
son_void_t
mro_print_pending_res
(
    const son_8 *son_method_name
);

/*! \fn son_u32 mro_get_pending_res_cell_count()
 *  \brief This function gets number of cells available in pending response
 */
son_u32
mro_get_pending_res_cell_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_procedure_code_et mro_get_pending_res_api_id()
 *  \brief Get API ID present in pending response
 */
son_procedure_code_et
mro_get_pending_res_api_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t mro_execute_optimization_algo(mro_cell_context_t *p_mro_cell_ctxt)
 *  \brief This function analyses the HO counters collected during
 *         the HO analysis period. If optimization is required
 *         determined using the configuation parameters,
 *         this functions calls the HO optimization function to perform
 *         the mobility robustness optimization.
 *  \param p_mro_cell_ctxt  Pointer to MRO cell context for which HO
 *                          analysis is to be done(i.e the cell for which
 *                          HO analyses period expired)
 */                           
son_void_t
mro_execute_optimization_algo
(
   mro_cell_context_t *p_mro_cell_ctxt
);

/*! \fn son_void_t mro_build_and_send_mro_ho_stats
 * (mro_cell_context_t *p_mro_cell_ctxt)
 * \brief Builds and send the HO counters report to mif. 
 *  \param p_mro_cell_ctxt  Pointer to MRO cell context 
 */

son_void_t
mro_build_and_send_mro_ho_stats
(
     mro_cell_context_t *p_mro_cell_ctxt
);
/*! \fn son_bool_et mro_delete_cell_from_context (
 * son_intra_rat_global_cell_id_t *p_cell_id)
 *  \brief This function deletes the specified cell context from the
 *         cell context list
 *  \param p_cell_id     Pointer to the cell Id
 */
son_bool_et
mro_delete_cell_from_context
(
    son_intra_rat_global_cell_id_t     *p_cell_id
);

/*! \fn son_u32 son_convert_min_into_millisecond(son_u8 time_in_min)
 *  \brief This function converts the time value from minute to millisecond.
 *  \param time_in_min Time value in minute
 */
son_u32
son_convert_min_into_millisecond
(
    son_u8  time_in_min
);

/*! \fn cell_status_list_data_t *mro_find_cell_in_pending_res (
 * son_intra_rat_global_cell_id_t *p_rcvd_cell_id)
 *  \brief Find node of the pendign response list with given cell Id
 *  \param p_rcvd_cell_id   Pointer to ECGI of the cell
 */
cell_status_list_data_t *
mro_find_cell_in_pending_res
(
    son_intra_rat_global_cell_id_t  *p_rcvd_cell_id
);

/*! \fn son_return_et mro_decrement_pending_res_expected_count()
 *  \brief Decrement Expected Response Count present in pending 
 *         response by 1.
 */
son_return_et
mro_decrement_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t mro_send_feature_state_change_ind(
 *  son_intra_rat_global_cell_id_t *p_cell_id, son_feature_state_et state,
 *  son_feature_state_change_cause_et cause)
 *  \brief This function creates and sends the feature state change indication 
 *  message to SONMgmtIfH.
 *  \param p_cell_id    Pointer to the cell Id
 *  \param state        MRO state for the cell
 *  \param cause        Cause for the present state of MRO for the cell
 */
son_void_t
mro_send_feature_state_change_ind
(
    son_intra_rat_global_cell_id_t      *p_cell_id,
    son_feature_state_et                state,
    son_feature_state_change_cause_et   cause
);

/*! \fn mro_cell_fsm_state_et mro_get_cell_mode(
 *  son_intra_rat_global_cell_id_t *p_cell_id)
 *  \brief This function find the cell FSM state for the given cell Id
 *  \param p_cell_id    Pointer to the cell Id
 */
mro_cell_fsm_state_et
mro_get_cell_mode
(
    son_intra_rat_global_cell_id_t     *p_cell_id
);

/*! \fn son_void_t mro_delete_cell_from_pending_res(son_intra_rat_global_cell_id_t *p_cell_id) 
 *  \brief This function deletes node with given cell Id from the pendig reponse list
 *  \param p_cell_id    Pointer to the cell Id
 */
son_void_t
mro_delete_cell_from_pending_res
(
    son_intra_rat_global_cell_id_t  *p_cell_id
);

/*! \fn son_void_t mro_reset_ho_counters(anr_intra_rat_hash_rec_t *p_intra_rat_hash_rec)
 *  \brief This function resets the MRO HO counters from NRT
 *  \param p_intra_rat_hash_rec     Pointer to NRT record
 */
son_void_t
mro_reset_ho_counters
(
    anr_intra_rat_hash_rec_t *p_intra_rat_hash_rec
);

/*! \fn son_void_t mro_reset_ttt_algo_params(mro_cell_context_t *p_mro_cell_ctxt)
 *  \brief This function resets the TTT specific algorithm parameters
 *  \param p_mro_cell_ctxt  Pointer to MRO cell context
 */
son_void_t
mro_reset_ttt_algo_params
(
    mro_cell_context_t *p_mro_cell_ctxt
);

/*! \fn son_void_t mro_reset_cio_algo_params(mro_cell_context_t *p_mro_cell_ctxt)
 *  \brief This function resets the CIO specific algorithm parameters
 *  \param p_mro_cell_ctxt  Pointer to MRO cell context
 */
son_void_t
mro_reset_cio_algo_params
(
    anr_intra_rat_hash_rec_t *p_intra_rat_hash_rec
);

/* SPR-13251 Fix Starts */
/*! \fn son_u8 mro_get_log_level()
 *  \brief This function gets the log level
 */
son_u8
mro_get_log_level
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t mro_set_log_level(son_u8 log_level)
 *  \brief This function sets the log level
 *  \param log_level Log Level to be set
 */
son_void_t
mro_set_log_level
(
    son_u8 log_level
);
/* SPR-13251 Fix Ends */

#endif /* ifndef _SON_MRO_CTXT_MGR_H_ */
