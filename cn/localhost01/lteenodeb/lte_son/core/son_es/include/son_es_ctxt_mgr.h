/*! \file son_es_ctxt_mgr.h
 *  \brief This file contains the prototypes of functions defined in son_es_ctxt_mgr.c
 *  \date  Feb, 2012
 *  \author Atul/Reetesh
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_ES_CTXT_MGR_H_
#define _SON_ES_CTXT_MGR_H_

/*! \headerfile son_es_intf.h <>
 */
#include <son_es_intf.h>

/*! \headerfile son_x2_intf.h <>
 */
#include <son_x2_intf.h>

/*! \headerfile son_anr_intf.h <>
 */
#include <son_anr_intf.h>

/* SPR-12708 Fix Starts */
/*! \def DEFAULT_SON_ES_TIMER
 *  \brief This macro is used to store the timer value in seconds used by ES module to find the expiry of various pending response messages
 */
#define DEFAULT_SON_ES_TIMER 5
/* SPR-12708 Fix Ends */

#define DEFAULT_SON_ES_SWITCH_ON_WAIT 5

/*! \def SON_ES_RRM_LOAD_REPORT_PERIODIC_INTERVAL
 *  \brief This macro defines the interval after which periodic load report
 *         is received from RRM for the serving cells
 */
#define SON_ES_RRM_LOAD_REPORT_PERIODIC_INTERVAL    10

/*! \def SONES_FSM_SET_STATE(new_state)
 *  \brief This macro is used to update the state of SONES MODULE FSM
 */
#define SONES_FSM_SET_STATE(curr_state, new_state) \
    do \
    { \
        SON_LOG(g_es_context.log_enable, p_son_es_facility_name, \
            SON_BRIEF, \
            "ES FSM State changed from %s to %s", \
            SON_ES_FSM_STATES_NAMES[curr_state], \
            SON_ES_FSM_STATES_NAMES[new_state]); \
        curr_state = new_state; \
    } \
    while (0)

/*! \def SONES_CELL_FSM_SET_STATE(prev_state,curr_state, new_state)
 *  \brief This macro is used to update the state of SONES CELL FSM
 */
#define SONES_CELL_FSM_SET_STATE(p_cell_ctxt, prev_state, curr_state, new_state) \
    do \
    { \
        SON_LOG(es_get_log_mode(), p_son_es_facility_name, \
            SON_BRIEF, \
            "ES Cell FSM State changed for cell %x  from %s to %s", \
            son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity), \
            SON_ES_CELL_FSM_STATES_NAMES[curr_state], \
            SON_ES_CELL_FSM_STATES_NAMES[new_state]); \
        prev_state = curr_state; \
        curr_state = new_state; \
    } \
    while (0)

/*! \a const son_u8 SON_ES_FSM_STATES_NAMES[];
 * \brief ES Fsm State Names
 */
extern const son_s8 *SON_ES_FSM_STATES_NAMES[];


/*! \a const son_u8 SON_ES_CELL_FSM_STATES_NAMES[];
 * \brief ES Cell FSm State Names
 */
extern const son_s8 *SON_ES_CELL_FSM_STATES_NAMES[];

/*! \a const son_u8 SON_ES_CELL_FSM_EVENT_NAMES[];
 * \brief ES Cell Fsm Event Names
 */
extern const son_s8 *SON_ES_CELL_FSM_EVENT_NAMES[];

/*! \enum es_cell_fsm_state_et
 *  \brief An enum that specifies the SON ES CELL FSM states
 */
typedef enum _es_cell_fsm_state_e
{
    ES_CELL_STATE_DISABLING = 0,
    ES_CELL_STATE_DISABLED,
    ES_CELL_STATE_ENABLING,
    ES_CELL_STATE_ENABLED,
    ES_CELL_STATE_SWITCHING_OFF,
    ES_CELL_STATE_SWITCHING_ON,
    ES_CELL_STATE_SWITCHED_OFF,
    ES_CELL_STATE_UNDEFINED = 0xFF
}es_cell_fsm_state_et;

/*! \enum es_fsm_state_et
 *  \brief An enum that specifies the SON ES FSM states
 */
typedef enum _es_fsm_state_e
{
    ES_STATE_INIT = 0,
    ES_STATE_ACTIVE,
    ES_STATE_ENABLING,
    ES_STATE_DISABLING,
    ES_STATE_SHUTDOWN,
    ES_STATE_UNDEFINED = 0xFF,
}es_fsm_state_et;

/*! \enum es_cell_power_state_et
 *  \brief An enum that specifies the SON ES CELL Power states
 */
typedef enum _es_cell_power_state_et
{
    ES_CELL_POWER_STATE_NULL = 0,
    ES_CELL_POWER_STATE_SWITCHING_OFF,
    ES_CELL_POWER_STATE_SWITCHING_ON,
    ES_CELL_POWER_STATE_SWITCHED_OFF,
    ES_CELL_POWER_STATE_SWITCHED_ON,
    ES_CELL_POWER_STATE_UNDEFINED = 0xFF
}es_cell_power_state_et;

/* AUTONOMOUS SWITCH OFF START */
#define SON_ES_AUTONOMOUS_SWITCH_OFF_ENABLED            0x01
#define SON_ES_AUTONOMOUS_SWITCH_OFF_CHECK_LOAD_LEVEL   0x02
#define SON_ES_AUTONOMOUS_SWITCH_OFF_CHECK_UE_CNT       0x04
#define SON_ES_AUTONOMOUS_SWITCH_OFF_REQ_SENT_TO_RRM    0x08
/* Fix 875 Start */
#define SON_ES_SWITCH_OFF_LOW_LOAD_COND_HIT             0x01
#define SON_ES_SWITCH_OFF_LOW_UE_CNT_COND_HIT           0x02
/* Fix 875 End */
/* AUTONOMOUS SWITCH OFF END */

/*! \  struct es_cell_context_t
 *  \brief cell's context linked list data definition
 *  \param cell_id				   Cell ID
 *  \param previous_cell_fsm_state ES FSM previous state of cell
 *  \param current_cell_fsm_state  ES Cell FSM State
 *  \param current_cell_state	   Cell State: Operational/Non operational
 *  \param autonomous_switch_off_info Bit 1 - if set indicates Autonomous
 *                                            switch off is enabled
 *                                  Bit 2 - if set indicates Load Level is to be checked for
 *                                          triggering autonomous switch off
 *                                  Bit 3 - if set indicates Active UE Count is to be
 *                                          checked for triggering autonomous switch off
 *  \param load_level_threshold     Threshold for load level for autonomous switch off
 *  \param ue_report_interval       Interval for periodic UE report
 *  \param modify_ue_report_interval      Var to store value of UE report interval rcvd in modify req 
 *                                  temporarily before commit in ue_report_interval until res from RRM comes
 *  \param ue_count_threshold       Threshold for active UEs
 *  \param modify_ue_count_threshold_val    Var to store value active no. of UEs rcvd in modify req
 *                                  temporarily before commit in ue_report_interval until res from RRM comes
 *  \param es_timer_id              Timer for autonomous switch off
 *  \param switch_off_cond_hit_status Bit 1 - if set indicates low load condition has been hit
 *                                    Bit 2 - if set indicates less UE Count condition has been hit
 */
typedef struct _es_cell_context_t
{
    son_intra_rat_global_cell_id_t cell_id;               /* Cell Id */
    es_cell_fsm_state_et previous_cell_fsm_state;
    es_cell_fsm_state_et current_cell_fsm_state; /* ES Cell FSM State */
    rrm_cell_operational_state_et current_cell_state;     /* Cell State: Operational/Non operational */
    /* AUTONOMOUS SWITCH OFF START */
    son_u8 autonomous_switch_off_info; /* Autonomous switch off en/dis,
                                        *  Load level criteria, UE cnt criteria */
    son_u8 load_level_threshold;      /* Threshold value for autonomous switch off */
    /* Fix 875 Start */
    son_u8 ue_report_interval;        /* Interval for periodic UE report */
    son_u8 modify_ue_report_interval;       /* Var to store value of UE report interval rcvd in modify req temporarily*/
    son_u8 ue_count_threshold;        /* Threshold value for active UEs */
    son_u8 modify_ue_count_threshold_val; /* Var to store value active no. of UEs rcvd in modify req temporarily*/
    son_timer_t es_timer_id;          /* Timer for autonomous switch off */
    son_u8 switch_off_cond_hit_status;/* Indicates the hit status of the conditions for Auto Cell switch off*/
    /* Fix 875 End */
    /* AUTONOMOUS SWITCH OFF END */
    /*SPR11689 changes start */
    son_bool_et cell_activation_req_flag;
    son_return_et rrm_cell_activation_response;
    /*save the target and self enb ID for which cell 
     * activation req is received*/
    son_global_enb_id_t                 enbid_1;
    son_global_enb_id_t                 enbid_2;
    /*SPR11689 changes end */
    /*SPR_19279_START*/
    son_u8 cell_index;
    /*SPR_19279_END*/

}es_cell_context_t;

/*! \  struct es_cell_context_node_t
 *  \brief cell's context linked list
 *  \param sNode				structure storing the next and
 *                              previous pointer to the list nodes
 *  \param data					Data stored in the list nodes
 */
typedef struct _es_cell_context_node_t
{
    SON_LIST_NODE sNode;
    es_cell_context_t data;
} es_cell_context_node_t;

/*! \  struct es_global_t
 *  \brief ES Module's Global context
 *  \param current_fsm_state	ES Module State
 *  \param es_global_mode	    ES Global Mode i.e. Enabled or Disabled
 *  \param cell_ctxt_list	    cell's context linked list
 *  \param log_enable			ES Logging mode i.e. enabled or not
 *  \param enb_type             eNB type(Home or Macro eNB)
 *  \param g_autonomous_switch_off_info Bit 1 - if set indicates Autonomous
 *                                            switch off is globally enabled
 *                                  Bit 2 - if set indicates Load Level is to be checked for
 *                                          triggering autonomous switch off
 *                                  Bit 3 - if set indicates Active UE Count is to be
 *                                          checked for triggering autonomous switch off
 *                                  Bit 4 - If set indicates that Autonomous cell switch req
 *                                          is sent to RRM due to low load or less UE cnt cond
 *                                          and corresponding cell switch res
 *                                          is pending from RRM
 *  \param g_load_level_threshold     Threshold for load level for autonomous switch off
 *  \param autonomous_switch_off_timer_val  Timer duration for Auto Switch Off wait
 *  \param g_es_timer_id              Global es timer ID
 *  \param ue_report_interval         Interval for periodic UE report
 *  \param ue_count_threshold         Threshold value for no of active UEs
 *  \param log_level                  ES log level
 */
typedef struct _es_global_t
{
    es_fsm_state_et current_fsm_state; /* ES Module State */
    son_app_mode_et es_global_mode;   /* ES Global Mode  */
    SON_LIST cell_ctxt_list;   /* ES Cell Context List */
    son_oam_log_on_off_et log_enable;               /* ES Logging enabled or not */
    son_enb_type_et enb_type;
    /* AUTONOMOUS SWITCH OFF START */
    son_u8 g_autonomous_switch_off_info; /* Global Autonomous switch off en/dis,
                                          *  Load level criteria, UE cnt criteria */
    son_u8 g_load_level_threshold;      /* Global Threshold value for autonomous switch off */
    /* Fix 875 Start */
    son_u16 autonomous_switch_off_timer_val; /* Timer duration for Auto Switch Off wait */
    son_timer_t g_es_timer_id;                 /* Global es timer ID */
    /* Fix 875 End */
    /* AUTONOMOUS SWITCH OFF END */
    /* Fix 867 Start*/
    son_u8 ue_report_interval;  /* Interval for periodic UE report */
    son_u8 ue_count_threshold;  /* Threshold value for no of active UEs */
    /* Fix 867 End */

    /*SPR11689 changes start */
    /*to save whether cell activation from peer eNB is enabled by OAM or not*/
    son_bool_et peer_cell_activation_enabled;
    /*SPR11689 changes end */

    /* SPR-13251 Fix Starts */
    son_u8 log_level;
    /* SPR-13251 Fix Ends */
}es_global_t;

/*! \typedef son_cell_status_t
 *  \brief Structure for storing cell status
 *  \param cgi    - CGI of cell
 *  \param result     - Outcome(Success/Failure)
 *  \param error_code - indicates error type in case of Failure
 */
typedef struct _son_cell_switchon_status_t
{
    son_intra_rat_global_cell_id_t cgi;
    es_cell_power_state_et cell_power_state;
    son_bool_et oam_ind_required;
    x2_deactivation_flag_et deactivation_indication;
    son_return_et result;
    son_error_et error_code;
}son_cell_switchon_status_t;

/*! \typedef struct cell_switchon_status_list_data_t
 *  \brief cell switch on status linked list nodes data definition
 *  \param cell_switchon_status_node    YLNODE structure storing the next and
 *                                      previous pointer to the list nodes
 *  \param cell_switchon_status         Data stored in the list nodes
 */
typedef struct _cell_switchon_status_list_data_t
{
    SON_LIST_NODE cell_switchon_status_node;
    son_cell_switchon_status_t cell_switchon_status;
}cell_switchon_status_list_data_t;

/*! \typedef struct switch_off_pending_res_t
 *  \brief Structure to store information related to the cells with pending
 *          Switch off response messages.
 *  \param transaction_id       Transaction Identifier to map the req/res
 *                              messages
 *  \param expected_res_count   Expected number of pending response
 *  \param api_id               Procedure code (API Id)
 *  \param expiry_time          Expiry time for all expected responses
 *  \param cell_sts_list        List containing status of cells for which
 *                              register/derester response is pending
 */
typedef struct _switch_on_pending_res_t
{
    son_u16 transaction_id;
    son_u8 expected_res_count;
    son_procedure_code_et api_id;
    son_time_t expiry_time;
    SON_LIST cell_switchon_status_list;
}switch_on_pending_res_t;

/*! \fn son_void_t es_init_global_context()
 *  \brief function that Initialize ES global Context
 *  \param None
 */
son_void_t
es_init_global_context
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t es_init_cell_context(es_cell_context_t *p_ctxt)
 *  \brief function that Initialize ES cell specific Context
 *  \param p_ctxt  Pointer to cell context
 */
son_void_t
es_init_cell_context
(
    es_cell_context_t *p_ctxt
);

/*! \fn son_void_t es_reset_pending_res()
 *  \brief function that resets pending_response structure
 *  \param None
 */
son_void_t
es_reset_pending_res
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t es_init_pending_res(son_u16 tid ,son_procedure_code_et api )
 *  \brief function that initialize pending_response structure
 *  \param tid		Transaction Id
 *  \param api		Procedure Code
 */
son_void_t
es_init_pending_res
(
    son_u16 tid,
    son_procedure_code_et api
);

/*! \fn son_u16 es_get_pending_res_transaction_id()
 *  \brief function that returns transaction id of pending response
 *  \param None
 */
son_u16
es_get_pending_res_transaction_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_u8 es_get_pending_res_expected_count()
 *  \brief function that returns count of awaited responses from pending response
 *  \param None
 */
son_u8
es_get_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t es_increment_pending_res_expected_count()
 *  \brief function that increments expected response count in pending response
 *  \param None
 */
son_void_t
es_increment_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t es_decrement_pending_res_expected_count()
 *  \brief function that Decrements expected response count in pending response
 *  \param None
 */
son_void_t
es_decrement_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_procedure_code_et es_get_pending_res_api_id()
 *  \brief function that returns API ID present in pending response
 *  \param None
 */

son_procedure_code_et
es_get_pending_res_api_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn SON_LIST * es_get_pending_res_cell_list()
 *  \brief function that returns a pointer to cell_status_list present in
 *		   pending response
 *  \param None
 */
SON_LIST *
es_get_pending_res_cell_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_u32 es_get_pending_res_ts_diff_from_curr_ts()
 *  \brief function that returns difference between current timestamp and
 *		   timestamp present in pending response
 *  \param None
 */
son_u32
es_get_pending_res_ts_diff_from_curr_ts
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t es_set_pending_res_ts_with_current_timestamp()
 *  \brief function that set timestamp in pending response for current time
 *  \param None
 */
son_void_t
es_set_pending_res_ts_with_current_timestamp
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t es_set_pending_res_error_rcvd( son_bool_et val )
 *  \brief function that set is_eror_received flag in pending response
 *  \param val		value to be updated is_eror_received flag of pending response
 */
son_void_t
es_set_pending_res_error_rcvd
(
    son_bool_et val
);

/*! \fn son_bool_et es_get_pending_res_error_rcvd()
 *  \brief function that get is_eror_received flag present in pending response
 *  \param None
 */
son_bool_et
es_get_pending_res_error_rcvd
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn cell_status_list_data_t * es_find_cell_in_pending_res(
 *                  son_intra_rat_global_cell_id_t *p_rcvd_cell_id)
 *  \brief function that find cell_status node in the cell_status_list
 *  \param p_rcvd_cell_id : Pointer to Cell ID
 */

cell_status_list_data_t *
es_find_cell_in_pending_res
(
    son_intra_rat_global_cell_id_t  *p_rcvd_cell_id
);

/*! \fn son_void_t es_delete_succesful_cells_from_pending_res()
 *  \brief function that Delete those cell_status nodes from cell_status_list
 *          which are succefully registered
 *  \param None
 */
son_void_t
es_delete_succesful_cells_from_pending_res
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t es_add_cell_in_pending_res(son_cell_status_t   *p_sts_node)
 *  \brief function that add cell_status node in the cell_status_list
 *  \param p_sts_node    Pointer to Cell Status Node
 */

son_bool_et
es_add_cell_in_pending_res
(
    son_cell_status_t   *p_sts_node
);

/*! \fn son_void_t es_delete_cell_from_pending_res( son_intra_rat_global_cell_id_t  *p_cell_id )
 *  \brief function that delete cell_status node from the cell_status_list
 *  \param p_cell_id   Pointer to Cell Id
 */
son_void_t
es_delete_cell_from_pending_res
(
    son_intra_rat_global_cell_id_t  *p_cell_id
);

/*! \fn son_void_t es_clear_pending_res_list()
 *  \brief function that delete all cell_status node from the cell_status_list
 *  \param None
 */
son_void_t
es_clear_pending_res_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_bool_et es_update_cell_in_pending_res( son_intra_rat_global_cell_id_t  *p_rcvd_cell_id,
 *        son_error_et  err_code)
 *  \brief function that update cell_status node in the cell_status_list
 *              for cell id, error code and result.
 *  \param p_rcvd_cell_id   Pointer to Cell ID
 *  \param err_code		 error_code
 */
son_bool_et
es_update_cell_in_pending_res
(
    son_intra_rat_global_cell_id_t  *p_rcvd_cell_id,
    son_error_et err_code
);

/*! \fn son_void_t es_construct_cell_arr_from_pending_res_list(son_cell_status_t *p_cell_list)
 *  \brief function that Construct cell_status_arr from cell_status_list
 *  \param p_cell_list Pointer to cell list
 */
son_void_t
es_construct_cell_arr_from_pending_res_list
(
    son_cell_status_t *p_cell_list
);

/*! \fn const son_void_t * son_key_of_es(const SON_LIST_NODE *p_node)
 *  \brief function that returns cell node key
 *  \param p_node   Pointer to cell specific context node
 */
const son_void_t *
son_key_of_es
(
    const SON_LIST_NODE *p_node
);

/*! \fn son_u32 es_get_context_cell_count()
 *  \brief function that returns the count of the cell ctxt node present in cell ctxt list
 *  \param None
 */
son_u32
es_get_context_cell_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_oam_log_on_off_et es_get_log_mode()
 *  \brief function that returns log mode (on/off)
 *  \param None
 */
son_oam_log_on_off_et
es_get_log_mode
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn SON_LIST * es_get_cell_ctxt_list()
 *  \brief function that returns cell context list
 *  \param None
 */
SON_LIST *
es_get_cell_ctxt_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn es_cell_context_t * es_get_cell_context_from_global_context(
 *                                                              son_intra_rat_global_cell_id_t     *p_cell_id)
 *  \brief function that returns cell context for cell id passed as an argument
 *  \param p_cell_id	Pointer to Cell ID
 */
es_cell_context_t *
es_get_cell_context_from_global_context
(
    son_intra_rat_global_cell_id_t     *p_cell_id
);

/*! \fn es_fsm_state_et es_get_context_state()
 *  \brief function that returns ES module's Global state
 *  \param None
 */
es_fsm_state_et
es_get_context_state
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t es_set_context_state ( es_fsm_state_et state)
 *  \brief function that set ES module's Global state
 *  \param state    ES module's Global state
 */
son_void_t
es_set_context_state
(
    es_fsm_state_et state
);

/*! \fn son_void_t es_set_log_mode(son_oam_log_on_off_et log_mode)
 *  \brief function that set ES module's log mode
 *  \param log_mode   log mode for ES module
 */
son_void_t
es_set_log_mode
(
    son_oam_log_on_off_et log_mode
);

/*SPR-11554-Start-Parameters of this function changed*/

/*! \fn es_cell_context_node_t * es_add_new_cell_in_context(
 *              son_cell_t *p_cell,son_feature_state_et *p_state,son_feature_state_change_cause_et *p_cause);
 *  \brief function that is used to add a new context in ES global context
 *			if not already exists.
 *  \param p_cell - Pointer to the cell data
 *  \param p_cause   - Cause of failure if failure occurs in addition
 *  \param p_state   - ES feature state for the cell
 */
es_cell_context_node_t *
es_add_new_cell_in_context
(
    son_cell_t                          *p_cell,
    son_feature_state_et                *p_state,
    /*SPR_19279_START*/
    son_feature_state_change_cause_et   *p_cause,
    son_u8                              cell_index
    /*SPR_19279_END*/
);

/*SPR-11554-End*/

/*! \fn son_void_t es_clear_context_list()
 *  \brief function that Clear ES context list
 *  \param None
 */
son_void_t
es_clear_context_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_bool_et es_delete_cell_from_context(
 *                                 son_intra_rat_global_cell_id_t *p_cell_id )
 *  \brief function that delete node from context list based on cell id passed
 *			as  argument
 *  \param  p_cell_id   Pointer to the cell Id
 */
son_bool_et
es_delete_cell_from_context
(
    son_intra_rat_global_cell_id_t     *p_cell_id
);

/*! \fn es_cell_context_node_t * es_get_first_cell_ctxt()
 *  \brief function that returns first context node from ES context list
 *  \param None
 */
es_cell_context_node_t *
es_get_first_cell_ctxt
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn es_cell_context_node_t * es_get_next_node(es_cell_context_node_t *p_node)
 *  \brief function that returns next context node in context list of the node passed
 *              as an argument
 *  \param p_node  Pointer to context node
 */
es_cell_context_node_t *
es_get_next_node
(
    es_cell_context_node_t *p_node
);

/*! \fn son_app_mode_et es_get_global_mode()
 *  \brief function that returns es global mode (enabled/disabled)
 *  \param None
 */
son_app_mode_et
es_get_global_mode
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_u32 es_get_pending_res_cell_count()
 *  \brief function that returns the count of ctxt nodes present in pending response
 *  \param None
 */
son_u32
es_get_pending_res_cell_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t es_set_global_mode (son_app_mode_et mode)
 *  \brief function that set es global mode (enabled/disabled)
 *  \param mode     - son app mode
 */
son_void_t
es_set_global_mode
(
    son_app_mode_et mode
);

/*  ----------------ES 1.2 changes Begin------------*/

/*! \fn son_void_t es_reset_switch_on_pending_res()
 *  \brief function that resets switch_on_pending_res structure
 *  \param None
 */
son_void_t
es_reset_switch_on_pending_res
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t es_init_switch_on_pending_res(son_u16 tid ,son_procedure_code_et api )
 *  \brief function that initialize switch_on_pending_res structure
 *  \param tid      Transaction Id
 *  \param api      Procedure Code
 */
son_void_t
es_init_switch_on_pending_res
(
    son_u16 tid,
    son_procedure_code_et api
);

/*! \fn son_bool_t es_switchon_pending_res_has_entry()
 *  \brief function that returns true if entry found, otherwise false
 *  \param None
 */
son_bool_et
es_switchon_pending_res_has_entry
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_u16 es_get_switchon_pending_res_transaction_id()
 *  \brief function that returns transaction id of switch_on_pending_res
 *  \param None
 */
son_u16
es_get_switchon_pending_res_transaction_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_u8 es_get_switchon_pending_res_expected_count()
 *  \brief function that returns count of awaited responses from switch_on_pending_res
 *  \param None
 */
son_u8
es_get_switchon_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t es_increment_switchon_pending_res_expected_count()
 *  \brief function that increments expected response count in switch_on_pending_res
 *  \param None
 */
son_void_t
es_increment_switchon_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t es_decrement_switchon_pending_res_expected_count()
 *  \brief function that Decrements expected response count in switch_on_pending_res
 *  \param None
 */
son_void_t
es_decrement_switchon_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_procedure_code_et es_get_switchon_pending_res_api_id()
 *  \brief function that returns API ID present in switch_on_pending_res
 *  \param None
 */
son_procedure_code_et
es_get_switchon_pending_res_api_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_time_t es_get_switchon_expiry_time()
 *  \brief function that returns the expiry time of pending switch on response struct
 *  \param None
 */
son_time_t
es_get_switchon_expiry_time
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_time_t es_get_switchon_expiry_time()
 *  \brief function that sets timestamp in pending switch on response
 *  \param son_time_t  set_time
 */
son_void_t
es_set_switchon_expiry_time
(
    son_time_t set_time,
    son_u16 sec
);

/*! \fn SON_LIST * es_get_switchon_pending_res_cell_list()
 *  \brief function that returns a pointer to cell_switchon_status_list present in
 *         switch_on_pending_res
 *  \param None
 */
SON_LIST *
es_get_switchon_pending_res_cell_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn cell_switchon_status_list_data_t * es_find_cell_in_switchon_pending_res(
 *                  son_intra_rat_global_cell_id_t *p_rcvd_cell_id)
 *  \brief function that find cell_switchon_status_node in the cell_switchon_status_list
 *  \param p_rcvd_cell_id : Pointer to Cell ID
 */
cell_switchon_status_list_data_t *
es_find_cell_in_switchon_pending_res
(
    son_intra_rat_global_cell_id_t  *p_rcvd_cell_id
);

/*! \fn son_void_t es_add_cell_in_switchon_pending_res(son_cell_switchon_status_t   *p_sts_node)
 *  \brief function that add cell_switchon_status_node in the cell_switchon_status_list
 *  \param p_sts_node    Pointer to Cell Status Node
 */

son_bool_et
es_add_cell_in_switchon_pending_res
(
    son_cell_switchon_status_t   *p_sts_node
);

/*! \fn son_void_t es_delete_cell_from_switchon_pending_res( son_intra_rat_global_cell_id_t  *p_cell_id )
 *  \brief function that delete cell_switchon_status_node from the cell_switchon_status_list
 *  \param p_cell_id   Pointer to Cell Id
 */
son_void_t
es_delete_cell_from_switchon_pending_res
(
    son_intra_rat_global_cell_id_t  *p_cell_id
);

/*! \fn son_void_t es_clear_switchon_pending_res_list()
 *  \brief function that delete all cell_switchon_status_node from the cell_switchon_status_list
 *  \param None
 */
son_void_t
es_clear_switchon_pending_res_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_bool_et es_update_cell_in_switchon_pending_res( son_intra_rat_global_cell_id_t  *p_rcvd_cell_id,
 *          es_cell_power_state_et power_state, son_return_et  res,son_error_et  err_code)
 *  \brief function that update cell_switchon_status_node in the cell_switchon_status_list
 *              for cell id, error code and result.
 *  \param p_rcvd_cell_id   Pointer to Cell ID
 *  \param power_state      Cell Power State
 *  \param oam_ind          Indication to OAM required or not
 *  \param deactivation_flag    Whether cell is switched off or On.
 *  \param res              result
 *  \param err_code         error_code
 */
son_bool_et
es_update_cell_in_switchon_pending_res
(
    son_intra_rat_global_cell_id_t  *p_rcvd_cell_id,
    es_cell_power_state_et power_state,
    son_bool_et oam_ind,
    x2_deactivation_flag_et deactivation_flag,
    son_return_et res,
    son_error_et err_code
);

/* ES 1.2 changes completed */

/*! \fn son_void_t es_send_feature_state_change_ind(
 *  son_intra_rat_global_cell_id_t *p_cell_id, son_feature_state_et state,
 *  son_feature_state_change_cause_et cause)
 *  \brief This function creates and sends the feature state change indication
 *  message to SONMgmtIfH.
 *  \param p_cell_id    Pointer to the cell Id
 *  \param state        ES state for the cell
 *  \param cause        Cause for the present state of ES for the cell
 */
son_void_t
es_send_feature_state_change_ind
(
    son_intra_rat_global_cell_id_t      *p_cell_id,
    son_feature_state_et state,
    son_feature_state_change_cause_et cause
);

/*! \fn son_void_t es_print_global_context(
    const son_8* son_method_name)
 *  \brief This function prints the global context of ES
 *  \param son_method_name Name of the function calling this function
 */
son_void_t
es_print_global_context
(
    const son_8* son_method_name
);

/*! \fn son_void_t es_print_cell_context(
    const son_8* son_method_name)
 *  \brief This function prints the cell context of ES
 *  \param p_cell_ctxt Pointer to the cell context
 */
son_void_t
es_print_cell_context(
    es_cell_context_t   *p_cell_ctxt
);

/* Rel 3.0 Multi Sector code changes Start */
/*! \fn son_void_t es_send_sonanr_cell_switch_on_off_info_ind(son_intra_rat_global_cell_id_t *p_cell_id, es_cell_switch_on_off_status_et cell_on_off_status)
 *  \brief This function creates and sends the cell switch ON/OFF indication message to SON ANR.
 *  \param p_cell_id            Pointer to the cell Id
 *  \param cell_on_off_status   Cell switched ON/OFF status
 */
son_void_t
es_send_sonanr_cell_switch_on_off_info_ind
(
    son_intra_rat_global_cell_id_t  *p_cell_id,
    es_cell_switch_on_off_status_et cell_on_off_status
);

/*! \fn son_void_t es_pending_res_handler()
 *  \brief function cleans up Pending Response Structure and sends
 *         pending resp to OAM.
 */
son_void_t
es_pending_res_handler
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);
/* Rel 3.0 Multi Sector code changes End */

/* SPR-13251 Fix Starts */
/*! \fn son_u8 es_get_log_level()
 *  \brief This function gets the log level
 */
son_u8
es_get_log_level
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t es_set_log_level(son_u8 log_level)
 *  \brief This function sets the log level
 *  \param log_level Log Level to be set
 */
son_void_t
es_set_log_level
(
    son_u8 log_level
);
/* SPR-13251 Fix Ends */

#endif /* _SON_ES_CTXT_MGR_H_ */
