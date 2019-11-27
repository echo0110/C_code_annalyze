/*! \file son_mlb_ctxt_mgr.h
 *  \brief This file contains the prototypes of functions defined in son_mlb_ctxt_mgr.c
 *  \date  Dec, 2012
 *  \author Sajida/Vinod
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_MLB_CTXT_MGR_H_
#define _SON_MLB_CTXT_MGR_H_

/*! \headerfile son_mlb_intf.h <>
 */
#include <son_mlb_intf.h>

/*! \headerfile rrm_son_intf.h <>
 */
#include <rrm_son_intf.h>

/*! \def MLB_MAX_FSM_STATE
 *  \brief This macro is used to store the MLB FSM states
 */

#define MLB_MAX_FSM_STATE 5

/*! \def NUM_OF_MLB_ITERATION
 *  \brief This macro is used to save the default number of MLB iteration used in store the MLB FSM states
 */
#define  NUM_OF_MLB_ITERATION 3

/*! \brief Default config parameters values----start
 */
#define DEFAULT_EUTRA_THRESHOLD_RSRP       20
#define DEFAULT_SMALL_STEP_CELL_OFFSET     10
#define DEFAULT_EUTRA_LOAD_THRESHOLD_IDLE  20
#define DEFAULT_EUTRA_LOAD_THRESHOLD_CONN  70
#define DEFAULT_MLB_START_THRESHOLD        30
#define DEFAULT_MLB_STOP_THRESHOLD         50
#define DEFAULT_MIN_CAPACITY_GAIN          5

#define MAX_EUTRA_THRESHOLD_RSRP           97
#define MAX_EUTRA_LOAD_THRESHOLD           100
#define MIN_NUM_OF_MLB_ITERATION           0
#define MAX_CAPACITY_GAIN                  10
/** eICIC changes start */
#define SON_CELL_DEFAULT_MIN_ABS_THRESHOLD  40
#define SON_CELL_DEFAULT_MAX_ABS_THRESHOLD  95
/** eICIC changes end */

/*! \brief Default config parameters values----end
 */
/*! \def SONMLB_FSM_SET_STATE(new_state)
 *  \brief This macro is used to update the state of SONMLB MODULE FSM
 */
#define SONMLB_FSM_SET_STATE(curr_state, new_state) \
    do \
    { \
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name, \
            SON_INFO, \
            "### MLB_FSM: Changed State %s -> %s ###", \
            SON_MLB_FSM_STATES_NAMES[curr_state], \
            SON_MLB_FSM_STATES_NAMES[new_state]); \
        curr_state = new_state; \
    } \
    while (0)

/*! \def SONMLB_FSM_SET_STATE(prev_state,curr_state, new_state)
 *  \brief This macro is used to update the state of SONMLB CELL FSM
 */
#define SONMLB_CELL_FSM_SET_STATE(p_cell_ctxt, prev_state, curr_state, new_state) \
    do \
    { \
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, \
            SON_INFO, \
            "### MLB_Cell_FSM: Cell 0x%x Changed State %s -> %s ###", \
            son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity), \
            SON_MLB_CELL_FSM_STATES_NAMES[curr_state], \
            SON_MLB_CELL_FSM_STATES_NAMES[new_state]); \
        prev_state = curr_state; \
        curr_state = new_state; \
    } \
    while (0)

/* SPR-12708 Fix Starts */
/*! \def MLB_DEFULT_PENDING_RESPONSE_TIMER
 *  \brief This macro stores default value of the time after which the pending
 * response list is cleaned automatically by MLB. The default value is 1 min = 60s
 */
#define MLB_DEFULT_PENDING_RESPONSE_TIMER           60
/* SPR-12708 Fix Ends */

/*! \a const son_u8 SON_mlb_FSM_STATES_NAMES[];
 * \brief MLB fsm State Names
 */
extern const son_s8 *SON_MLB_FSM_STATES_NAMES[];

/*! \a const son_u8 SON_MLB_CELL_FSM_STATES_NAMES[];
 * \brief MLB Cell FSm State Names
 */
extern const son_s8 *SON_MLB_CELL_FSM_STATES_NAMES[];

/*! \a const son_u8 SON_MLB_CELL_FSM_EVENT_NAMES[];
 *  \brief ES Cell Fsm Event Names
 */
extern const son_s8 *SON_MLB_CELL_FSM_EVENT_NAMES[];

#define MAX_NEIGH_CELL SON_MAX_NO_INTRA_RAT_NEIGHBOUR_CELLS
#define MAX_HO_SAMPLE 5

typedef STACKCPOOL mlb_hash_pool_t;

/*! \def MLB_NBR_Q_INIT(p_neigh_cell_info)
 *    *  \brief A macro specifies "ylInit( p_neigh_cell_info )" ,use to scan the queue.
 *        */

#define MLB_NBR_Q_INIT(p_neigh_cell_info) \
    ylInit( p_neigh_cell_info )

/*! \def MLB_NBR_Q_GET_COUNT(p_neigh_cell_info)
 *  *  \brief A macro specifies "ylCount( p_neigh_cell_info )" ,use to count the enteries in queue.
 *   */

#define MLB_NBR_Q_GET_COUNT(p_neigh_cell_info_head) \
    ylCount(p_neigh_cell_info_head)

/*! \def MLB_NBR_Q_ENQUEUE(p_neigh_cell_info, p_neigh_cell_record)
 *  *  \brief A macro specifies "ylPushTail( p_neigh_cell_info, &(p_neigh_cell_record)->sNode )" ,use to scan the enqueue requests.
 *   */

#define MLB_NBR_Q_ENQUEUE(p_neigh_cell_info, p_neigh_cell_record) \
    ylPushTail( p_neigh_cell_info, &(p_neigh_cell_record)->sNode )

/*! \def MLB_NBR_Q_DEQUEUE(p_neigh_cell_info, p_neigh_cell_record)
 *  \brief A macro that specifies "{\
 *  YLNODE *snode = ylPopHead( p_neigh_cell_info );\
 *  p_neigh_cell_record = YMEMBEROF( nmmctl_scan_record_t, sNode, snode );" ,use to scan the dequeue member.
 * */

#define MLB_NBR_Q_DEQUEUE(p_neigh_cell_info, p_neigh_cell_record){ \
        YLNODE *snode = ylPopHead( p_neigh_cell_info ); \
        p_neigh_cell_record = YMEMBEROF(son_mlb_neigh_cell_info_record_t, sNode, snode ); \
}

/*! \def MLB_NBR_Q_ENQUEUE(p_neigh_cell_info, p_neigh_cell_record)
 *  *  \brief A macro specifies "ylPushTail( p_neigh_cell_info, &(p_neigh_cell_record)->sNode )" ,use to scan the enqueue requests.
 *   */

#define MLB_NBR_Q_DELETE(p_neigh_cell_info, p_neigh_cell_record) \
    ylDelete( p_neigh_cell_info, (son_void_t *)p_neigh_cell_record)

typedef enum _mlb_fsm_state_e
{
    MLB_STATE_INIT = 0,
    MLB_STATE_ACTIVE,
    MLB_STATE_ENABLING,
    MLB_STATE_DISABLING,
    MLB_STATE_SHUTDOWN,
    MLB_STATE_UNDEFINED = 0xFF,
}mlb_fsm_state_et;

/*! \  mlb_global_t
 *  \brief MLB Global Context structure
 *  \param current_fsm_state            Global MLB State
 *  \param MLB_global_mode                  Global MLB Mode (Enabled or Disabled)
 *  \param log_enable                       log mode for MLB module
 *  \param enb_type                         eNB type(Home or Macro eNB)
 *  \param cell_ctxt_list                   MLB cell context List
 *  \param log_level                        MLB log level
 */
typedef struct _mlb_global_t
{
    mlb_fsm_state_et current_fsm_state;
    son_app_mode_et mlb_global_mode;
    son_oam_log_on_off_et log_enable;
    son_enb_type_et enb_type;
    SON_LIST cell_ctxt_list;
    son_mlb_config_param_t mlb_config_param;
    son_timer_t mlb_prending_res_timer_id;
    /* SPR-13251 Fix Starts */
    son_u8 log_level;
    /* SPR-13251 Fix Ends */
}mlb_global_t;

mlb_global_t g_mlb_context;

/*! \enum mlb_cell_fsm_state_et
 *  \brief An enum that specifies the SON MLB CELL FSM states
 */
typedef enum _mlb_cell_fsm_state_e
{
    MLB_CELL_STATE_DISABLING = 0,
    MLB_CELL_STATE_DISABLED,
    MLB_CELL_STATE_ENABLING,
    MLB_CELL_STATE_ENABLED,
    MLB_CELL_STATE_UNDEFINED = 0xFF
}mlb_cell_fsm_state_et;

/*! \  struct mlb_cell_context_t
 *  \brief cell's context linked list data definition
 *  \param cell_id                     Cell ID
 *  \param previous_cell_fsm_state     MLB FSM previous state of cell
 *  \param current_cell_fsm_state      MLB Cell FSM State
 *  \param current_cell_state          Cell State: Operational/Non operational
 */

typedef struct _son_mlb_neigh_cell_info
{
    son_intra_rat_global_cell_id_t neigh_cell_id;               /* Cell Id */
    son_u32 ho_count;
    son_u8 rsrp[MAX_HO_SAMPLE];

    rrm_son_offset_attr_t offset_attr;
    rrm_son_cell_load_info_t neigh_cell_load_info;
    /* SPR 22195 Fix Start */
    /* Code Removed */
    /* SPR 22195 Fix Stop */
    son_s8 new_cell_offset_idle;
    son_s8 new_cell_offset_connected;
    son_q_offset_range_et initial_cell_offset;  /*to save Qoffset*/
    son_q_offset_range_et initial_cm_cell_offset; /*to save OCN*/
    son_u8 nbr_csg_id_present;
    son_u8 mob_change_req_sent; /*flag to use for mob change req sent for a particular nbr*/
    son_u8 avg_rsrp;
    son_u8 csg_identity[SON_CSG_ID_OCTET_SIZE];
    son_u8 num_of_mlb_iteration; /*to store the number of load balancing iterations done so far for a
                                  * particular nbr cell.serving cell available capacity
                                  * is not increased substantially throughout this iteration*/
/** eICIC chages start */
    son_u32 eicic_scheme;
/** eICIC changes end */
/* bug_13866_start */
   son_u8 min_abs_usage_threshold_aggressor;
   son_u8 max_abs_usage_threshold_aggressor;
/* bug_13866_end */
}son_mlb_neigh_cell_info_t;

/*! \var typedef YLIST nmmctl_scan_queue_t
 *  \brief Represent the list for scan structure
 */

typedef YLIST son_mlb_neigh_cell_info_head_t;

/*! \  struct nmmctl_scan_record_t
 *  \brief              Contains the parameters require to scan record
 *  \param  sNode       Variable of type YNODE
 *  \param  scan_data   variable for nmmctl_start_scan_req_t
 */

typedef struct _son_mlb_neigh_cell_info_record_t
{
    YLNODE sNode;
    son_mlb_neigh_cell_info_t neigh_cell_info_data;
} son_mlb_neigh_cell_info_record_t;

/** eICIC changes start */
typedef enum
{
    SON_EICIC_DEACTIVATED,
    SON_ACT_AS_AGGRESSOR,
    SON_ACT_AS_VICTIM,
    SON_ACT_AS_AGGRESSOR_AND_VICTIM
}son_eicic_mode_type_et;
/** eICIC changes end */

typedef struct _mlb_cell_context_t
{
    son_intra_rat_global_cell_id_t cell_id;       /* Cell Id */
    mlb_cell_fsm_state_et previous_cell_fsm_state;
    mlb_cell_fsm_state_et current_cell_fsm_state; /* MLB cell FSM State */
    rrm_cell_operational_state_et current_cell_state;    /* Cell tate: Operational/Non operational */
    son_eutran_q_offset_config_t eutra_q_offset_config;
    rrm_son_cell_load_info_t serv_cell_load_info;
    son_mlb_neigh_cell_info_head_t neigh_cell_info_head;
    son_u8 serving_rsrp;
    /* SPR 22195 Fix Start */
    /* Code Removed */
    /* SPR 22195 Fix Stop */
    son_u32 eutra_threshold;
    son_u8 eutra_threshold_rsrp; /* THrsrp in equation (∑ RSRP(i) / n >= THrsrp)*/
    son_u32 eutra_threshold_idle;     /*Pth,idle*/
    son_u32 eutra_threshold_connected; /*Pth,conn*/
    son_u32 delta_offset_idle;
    son_u32 delta_offset_connected;
    son_u8 small_step_cell_offset; /* value of ‘A’ in equation (Qoffset,new = A * RSRP(C1) + Qinitial)*/
    son_u8 src_csg_id_present;
    son_u8 csg_identity[SON_CSG_ID_OCTET_SIZE];
    son_u8 count_for_neigh_cell;
    son_u8 strongest_nbr_rsrp;
    son_procedure_code_et dereg_invoking_api;
    son_u8 dl_cap_val[NUM_OF_MLB_ITERATION];
    son_bool_et mlb_start_flag;
    /** eICIC  changes start */
    son_eicic_mode_type_et  provision_type;   
    /** eICIC changes end */
    /* bug_13866_start */
    son_u8                  min_abs_usage_threshold_victim;
    son_u8                  max_abs_usage_threshold_victim;
    /* bug_13866_end */
   /*SPR_19279_START */
    son_u8 cell_index;
   /*SPR_19279_END */
}mlb_cell_context_t;

/*! \  struct mlb_cell_context_node_t
 *  \brief cell's context linked list
 *  \param sNode structure storing the next and
 *   previous pointer to the list nodes
 *  \param data Data stored in the list nodes
 */
typedef struct _mlb_cell_context_node_t
{
    SON_LIST_NODE sNode;
    mlb_cell_context_t data;
} mlb_cell_context_node_t;

/*! \  struct mlb_global_t
 *  \brief MLB Module's Global context
 *  \param current_fsm_state        MLB Module State
 *  \param es_global_mode           MLB Global Mode i.e. Enabled or Disabled
 *  \param cell_ctxt_list           MLB cell's context linked list
 *  \param log_enable               MLB Logging mode i.e. enabled or not
 */

/*! \fn son_void_t mlb_init_global_context()
 *  \brief function that Initialize MLB global Context
 */
son_void_t
mlb_init_global_context
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t mlb_init_cell_context(mlb_cell_context_t *p_ctxt)
 *  \brief function that Initialize MLB cell specific Context
 *  \param p_ctxt  Pointer to cell context
 */
son_void_t
mlb_init_cell_context
(
    mlb_cell_context_t *p_ctxt
);

/*! \fn son_void_t mlb_init_pending_res(son_u16 tid ,son_procedure_code_et api )
 *  \brief function that initialize pending_response structure
 *  \param tid          Transaction Id
 *  \param api          Procedure Code
 */
son_void_t
mlb_init_pending_res
(
    son_u16 tid,
    son_procedure_code_et api
);

/*! \fn son_u16 mlb_get_pending_res_transaction_id()
 *  \brief function that returns transaction id of pending response
 */
son_u16
mlb_get_pending_res_transaction_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_u8 mlb_get_pending_res_expected_count()
 *  \brief function that returns count of awaited responses from pending response
 */
son_u8
mlb_get_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t mlb_increment_pending_res_expected_count()
 *  \brief function that increments expected response count in pending response
 */
son_void_t
mlb_increment_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t mlb_decrement_pending_res_expected_count()
 *  \brief function that Decrements expected response count in pending response
 */
son_void_t
mlb_decrement_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_procedure_code_et mlb_get_pending_res_api_id()
 *  \brief function that returns API ID present in pending response
 */

son_procedure_code_et
mlb_get_pending_res_api_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn SON_LIST * mlb_get_pending_res_cell_list()
 *  \brief function that returns a pointer to cell_status_list present in
 *           pending response
 */
SON_LIST *
mlb_get_pending_res_cell_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_u32 mlb_get_pending_res_ts_diff_from_curr_ts()
 *  \brief function that returns difference between current timestamp and
 *           timestamp present in pending response
 */
son_u32
mlb_get_pending_res_ts_diff_from_curr_ts
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t mlb_set_pending_res_ts_with_current_timestamp()
 *  \brief function that set timestamp in pending response for current time
 */
son_void_t
mlb_set_pending_res_ts_with_current_timestamp
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t mlb_set_pending_res_error_rcvd( son_bool_et val )
 *  \brief function that set is_eror_received flag in pending response
 *  \param val        value to be updated is_eror_received flag of pending response
 */
son_void_t
mlb_set_pending_res_error_rcvd
(
    son_bool_et val
);

/*! \fn son_bool_et mlb_get_pending_res_error_rcvd()
 *  \brief function that get is_eror_received flag present in pending response
 */
son_bool_et
mlb_get_pending_res_error_rcvd
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn cell_status_list_data_t *mlb_find_cell_in_pending_res(
 *   son_intra_rat_global_cell_id_t * p_rcvd_cell_id)
 *  \brief function that find cell_status node in the cell_status_list
 *  \param p_rcvd_cell_id : Pointer to Cell ID
 */

cell_status_list_data_t *
mlb_find_cell_in_pending_res
(
    son_intra_rat_global_cell_id_t  *p_rcvd_cell_id
);

/*! \fn son_void_t mlb_clear_pending_res_list()
 *  \brief function that delete all cell_status node from the cell_status_list
 */
son_void_t
mlb_clear_pending_res_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_bool_et mlb_update_cell_in_pending_res( son_intra_rat_global_cell_id_t  *p_rcvd_cell_id,
 *        son_error_et  err_code)
 *  \brief function that update cell_status node in the cell_status_list
 *              for cell id, error code and result.
 *  \param p_rcvd_cell_id   Pointer to Cell ID
 *  \param err_code         error_code
 */
son_bool_et
mlb_update_cell_in_pending_res
(
    son_intra_rat_global_cell_id_t  *p_rcvd_cell_id,
    son_error_et err_code
);

/*! \fn son_void_t mlb_construct_cell_arr_from_pending_res_list (
 * son_u16 *p_cell_sts_arr_size, son_cell_status_t *p_cell_sts_arr,
 * mlb_cell_fsm_state_et cell_fsm_state)
 *  \brief Construct cell status array from pending response list
 *  \param p_cell_sts_arr_size  Pointer to cell status array size
 *  \param p_cell_sts_arr       pointer to cell status array
 *  \param cell_fsm_state       Cell FSM state to be set
 */
son_void_t
mlb_construct_cell_arr_from_pending_res_list
(
    son_u16                     *p_cell_sts_arr_size,
    son_cell_status_t           *p_cell_sts_arr,
    mlb_cell_fsm_state_et cell_fsm_state
);

/*! \fn const son_void_t * son_key_of_mlb(const SON_LIST_NODE *p_node)
 *  \brief function that returns cell node key
 *  \param p_node   Pointer to cell specific context node
 */
const son_void_t *
son_key_of_mlb
(
    const SON_LIST_NODE *p_node
);
/*SPR_19279_START */
/*! \fn const son_void_t * son_key_of_mlb(const SON_LIST_NODE *p_node)
 *  \brief function that returns cell node key
 *  \param p_node   Pointer to cell specific context node
 */
const son_void_t *
son_cell_index_key_of_mlb
(
    const SON_LIST_NODE *p_node
);
/*SPR_19279_END */

/*! \fn son_u32 mlb_get_context_cell_count()
 *  \brief function that returns the count of the cell ctxt node present in cell ctxt list
 */
son_u32
mlb_get_context_cell_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_oam_log_on_off_et mlb_get_log_mode()
 *  \brief function that returns log mode (on/off)
 */
son_oam_log_on_off_et
mlb_get_log_mode
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn SON_LIST * mlb_get_cell_ctxt_list()
 *  \brief function that returns cell context list
 */
SON_LIST *
mlb_get_cell_ctxt_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn mlb_cell_context_t * mlb_get_cell_context_from_global_context(
 *                              son_intra_rat_global_cell_id_t     *p_cell_id)
 *  \brief function that returns cell context for cell id passed as an argument
 *  \param p_cell_id    Pointer to Cell ID
 */
mlb_cell_context_t *
mlb_get_cell_context_from_global_context
(
    son_intra_rat_global_cell_id_t     *p_cell_id
);

/*! \fn mlb_fsm_state_et mlb_get_context_state()
 *  \brief function that returns ES module's Global state
 */
mlb_fsm_state_et
mlb_get_context_state
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t mlb_set_context_state ( mlb_fsm_state_et state)
 *  \brief function that set MLB module's Global state
 *  \param state    MLB module's Global state
 */
son_void_t
mlb_set_context_state
(
    mlb_fsm_state_et state
);

/*! \fn son_void_t mlb_set_log_mode(son_oam_log_on_off_et log_mode)
 *  \brief function that set MLB module's log mode
 *  \param log_mode   log mode for MLB module
 */
son_void_t
mlb_set_log_mode
(
    son_oam_log_on_off_et log_mode
);

/*SPR-11554-Start*/

/*! \fn mlb_add_new_cell_in_context * mlb_add_new_cell_in_context(
 * son_cell_t *p_cell,son_feature_state_et  *p_state,son_feature_state_change_cause_et *p_cause)
 *  \brief function that is used to add a new context in MLB global context
 *          if not already exists.
 *  \param p_cell - Pointer to the cell data
 *  \param p_cause   - Cause of failure if failure occurs in addition
 *  \param p_state   - MLB feature state for the cell
 */
mlb_cell_context_node_t *
mlb_add_new_cell_in_context
(
    son_cell_t                          *p_cell,
    son_feature_state_et                *p_state,
    /*SPR_19279_START*/
    son_feature_state_change_cause_et   *p_cause,
    son_u8                              cell_index
    /*SPR_19279_END*/
);

/*SPR-11554-End*/

/*! \fn son_void_t mlb_clear_context_list()
 *  \brief function that Clear MLB context list
 */
son_void_t
mlb_clear_context_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_bool_et mlb_delete_cell_from_context(
 *                                 son_intra_rat_global_cell_id_t *p_cell_id )
 *  \brief function that delete node from context list based on cell id passed
 *            as  argument
 *  \param  p_cell_id   Pointer to the cell Id
 */
son_bool_et
mlb_delete_cell_from_context
(
    son_intra_rat_global_cell_id_t     *p_cell_id
);

/*! \fn mlb_cell_context_node_t * mlb_get_first_cell_ctxt()
 *  \brief function that returns first context node from MLB context list
 */
mlb_cell_context_node_t *
mlb_get_first_cell_ctxt
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn mlb_cell_context_node_t * mlb_get_next_node(mlb_cell_context_node_t *p_node)
 *  \brief function that returns next context node in context list of the node passed
 *              as an argument
 *  \param p_node  Pointer to context node
 */
mlb_cell_context_node_t *
mlb_get_next_node
(
    mlb_cell_context_node_t *p_node
);

/*! \fn son_app_mode_et mlb_get_global_mode()
 *  \brief function that returns mlb global mode (enabled/disabled)
 */
son_app_mode_et
mlb_get_global_mode
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t mlb_set_global_mode (son_app_mode_et mode)
 *  \brief function that set mlb global mode (enabled/disabled)
 *  \param mode     - son app mode
 */

son_void_t
mlb_set_global_mode
(
    son_app_mode_et mode
);

son_bool_et
mlb_add_cell_in_pending_res
(
    son_cell_status_t   *p_sts_node
);

son_void_t
mlb_reset_pending_res
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

son_void_t
mlb_delete_cell_from_pending_res
(
    son_intra_rat_global_cell_id_t  *p_cell_id
);
son_u32
mlb_get_pending_res_cell_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

son_void_t
mlb_delete_succesful_cells_from_pending_res
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

son_void_t
mlb_pending_res_timer_handler
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);
mlb_cell_fsm_state_et
mlb_get_cell_mode
(
    son_intra_rat_global_cell_id_t     *p_cell_id
);

/*! \fn son_void_t mlb_send_feature_state_change_ind(
 *  son_intra_rat_global_cell_id_t *p_cell_id, son_feature_state_et state,
 *  son_feature_state_change_cause_et cause)
 *  \brief This function creates and sends the feature state change indication
 *  message to SONMgmtIfH.
 *  \param p_cell_id    Pointer to the cell Id
 *  \param state        MLB state for the cell
 *  \param cause        Cause for the present state of MLB for the cell
 */
son_void_t
mlb_send_feature_state_change_ind
(
    son_intra_rat_global_cell_id_t      *p_cell_id,
    son_feature_state_et state,
    son_feature_state_change_cause_et cause
);

/*! \fn son_void_t mlb_reset_pending_resp_list()
 *  \brief Resets MLB pending_response structure
 */
son_void_t
mlb_reset_pending_resp_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_bool_et mlb_is_any_cell_disabled()
 *  \brief Checks if at least one cell exists which is in
 *         MLB_CELL_STATE_DISABLED or MLB_CELL_STATE_DISABLING state.
 *         If such cell is found it indicates that global enable
 *         request can be processed as some cells needs to be enabled.
 */
son_bool_et
mlb_is_any_cell_disabled
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*!  \fn son_bool_et mlb_is_any_cell_enabled()
 *  \brief Checks if at least one cell exists which is in
 *          MLB_CELL_STATE_ENABLED or MLB_CELL_STATE_ENABLING state.
 *          If such cell is found it indicates that global disable
 *          request can be processed as some cells needs to be disabled.
 */
son_bool_et
mlb_is_any_cell_enabled
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t
 * mlb_populate_enable_disable_res_failure_data_from_pending_list
 * (son_u16 *p_cell_status_list_size,son_cell_status_t *p_cell_status_list
 *  \brief This function populates enable or disable response data
 *         struture with the cell Ids and the error codes of the cell
 *         for which (de)registration request could not be sent to
 *         RRM due to some SONMLB error or may be because ANR mode is
 *         not enabled for the cell or cell context is not found at
 *         ANR.
 *  \param p_cell_status_list_size   Number of cells to be send in response
 *  \param p_cell_status_list        Pointer to list containg cell Ids and error
 *                                   code indicating the reason for failure.
 */
son_void_t
mlb_populate_enable_disable_res_failure_data_from_pending_list
(
    son_u16            *p_cell_status_list_size,
    son_cell_status_t  *p_cell_status_list
);

/*! \fn son_void_t mlb_send_feature_state_change_ind(
 *  son_intra_rat_global_cell_id_t *p_cell_id, son_feature_state_et state,
 *  son_feature_state_change_cause_et cause)
 *  \brief This function creates and sends the feature state change indication
 *  message to SONMgmtIfH.
 *  \param p_cell_id    Pointer to the cell Id
 *  \param state        MLB state for the cell
 *  \param cause        Cause for the present state of MLB for the cell
 */
son_void_t
mlb_send_feature_state_change_ind
(
    son_intra_rat_global_cell_id_t      *p_cell_id,
    son_feature_state_et state,
    son_feature_state_change_cause_et cause
);

/*! \fn son_return_et son_mlb_fill_mlb_param_default_values(
 *  son_mlb_config_param_t *p_mlb_config_param)
 *  \brief This function fills MLB context with deafult values
 *  \param p_mlb_config_param    Pointer to MLB config parameters
 */
son_return_et
son_mlb_fill_mlb_param_default_values
(
    son_mlb_config_param_t *p_mlb_config_param
);

/* SPR-13251 Fix Starts */
/*! \fn son_u8 mlb_get_log_level()
 *  \brief This function gets the log level
 */
son_u8
mlb_get_log_level
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t mlb_set_log_level(son_u8 log_level)
 *  \brief This function sets the log level
 *  \param log_level Log Level to be set
 */
son_void_t
mlb_set_log_level
(
    son_u8 log_level
);
/* SPR-13251 Fix Ends */
/** eICIC changes start */
/*SPR 22419 Fix Start*/
/*Code Removed*/
/*SPR 22419 Fix Start*/
/* eICIC changes end */

#endif /*_SON_MLB_CTXT_MGR_H_*/
