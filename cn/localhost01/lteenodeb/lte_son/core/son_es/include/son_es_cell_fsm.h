/*! \file son_es_cell_fsm.h
 *  \brief This  File contains the struct, constants definitions
 *                   specific to SON ES at module level.
 *  \date  Feb, 2012
 *  \author Atul\ Reetesh
 */

/*!
 * \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_ES_CELL_FSM_H_
#define _SON_ES_CELL_FSM_H_

/*! \headerfile son_es_intf.h <>
 */
#include <son_es_intf.h>

/*! \headerfile son_x2_intf.h<>\
 */
#include <son_x2_intf.h>

/*! \headerfile son_es_ctxt_mgr.h <>
 */
#include <son_es_ctxt_mgr.h>

/*! \headerfile son_defines.h <>
 */
#include <son_defines.h>

/*! \def ES_CELL_NUM_OF_STATES
 *  \brief A macro that stores the number of states in SON ES FSM
 */
#define ES_CELL_NUM_OF_STATES    7

/*! \def ES_CELL_NUM_OF_EVENTS
 *  \brief A macro that stores the number of events in SON ES FSM
 */
/* AUTONOMOUS SWITCH OFF START */
#define ES_CELL_NUM_OF_EVENTS   17 
/* AUTONOMOUS SWITCH OFF END */

/*! \def ES_TIMEOUT_FOR_PENDING_RES
 *  \brief A macro that stores the seconds in which register/deregister
 *  response shall be received for all the cells.
 */
#define ES_TIMEOUT_FOR_PENDING_RES      60

/*! \def ES_CELL_SWITCH_OFF_TIME
 *  \brief A macro that stores the time interval in second after that
 *          switch off shall be done by RRM
 */
#define ES_CELL_SWITCH_OFF_TIME   20

/*! \def ES_MAX_LOAD
 *  \brief A macro that defines the max value(%) of load
 */
#define ES_MAX_LOAD     100

/*! \enum son_es_index_et
 *  \brief An enum that specifies the index value in FSM
 */
typedef enum _son_es_index_et
{
    IDX_SONES_BASE = 0,
    IDX_SONES_ENABLE_REQ = IDX_SONES_BASE,
    IDX_SONES_DISABLE_REQ,
    IDX_SONES_CELL_SWITCH_ON_REQ,
    IDX_SONES_CELL_SWITCH_OFF_REQ,
    IDX_SONES_MODIFY_UE_COUNT_INTERVAL_REQ,
    IDX_SONES_MODIFY_UE_COUNT_THRESHOLD_REQ,
    IDX_RRM_SON_REGISTER_RESP,
    IDX_RRM_SON_DEREGISTER_RESP,
    IDX_RRM_SON_SET_ATTRIBUTE_RESP,
    IDX_RRM_SON_CELL_SWITCH_ON_RESP,
    IDX_RRM_SON_CELL_SWITCH_OFF_RESP,
    IDX_RRM_SON_NUM_ACTIVE_UE_REPORT,
    IDX_RRM_UE_COUNT_THRESHOLD_REACHED_IND,
    IDX_RRM_S1_MSG_IND,
    IDX_RRM_SON_CELL_CHNAGE_IND,
    /* AUTONOMOUS SWITCH OFF START */
    IDX_RRM_SON_LOAD_REPORT,
    IDX_SONES_CELL_ACTIVATION_REQ,
    IDX_SONES_MAX = IDX_SONES_CELL_ACTIVATION_REQ,
    /* AUTONOMOUS SWITCH OFF END */
    IDX_NOT_AVAILABLE  = 0xFF
}son_es_index_et;

/*! \fn son_buf_retain_status_et es_cell_process_msg(son_es_index_et evnt_id,
 *                         es_cell_context_t *p_cell_ctxt, STACKAPIHDR *p_cspl_hdr,
 *                         son_u8 *p_es_msg)
 *  \brief This is the entry point function for ES Cell FSM
 *  \param evnt_id             Event identity
 *  \param p_cell_ctxt         Pointer to cell context
 *  \param p_cspl_hdr          Pointer to  the STACKAPIHDR structure
 *  \param p_es_msg            Pointer to incoming message
 */
son_buf_retain_status_et
es_cell_process_msg
(
    son_es_index_et evnt_id,
    es_cell_context_t      *p_cell_ctxt,
    STACKAPIHDR            *p_cspl_hdr,
    son_u8                 *p_es_msg
);

/*! \fn son_return_et es_construct_send_rrm_deregister_req(son_u16 transaction_id,
 *                                 son_intra_rat_global_cell_id_t *p_cell_id)
 *  \brief This is the function to create deregister request
 *  \param transaction_id   Transaction Id
 *  \param p_cell_id		Pointer to Cell Id
 */
son_return_et
es_construct_send_rrm_deregister_req
(
    son_u16 transaction_id,
    son_intra_rat_global_cell_id_t      *p_cell_id
);

/*! \fn son_void_t son_create_and_send_switch_on_ind_to_x2(
 *          son_procedure_code_et invoking_api, son_intra_rat_global_cell_id_t*             p_src_cgi)
 *  \brief Genericc function to construct and send
 *         son_x2_cell_switch_on_off_ind message to X2 Module.
 *  \param invoking_api      Invoking API ID
 *  \param p_src_cgi         Pointer to cell ID
 */
son_void_t
son_create_and_send_switch_on_ind_to_x2
(
    son_procedure_code_et invoking_api,
    son_intra_rat_global_cell_id_t *p_src_cgi
);

/*! \fn son_void_t son_create_and_send_switch_off_ind_to_x2(
 *          son_intra_rat_global_cell_id_t* p_src_cgi)
 *  \brief Genericc function to construct and send
 *         son_x2_cell_switch_on_off_ind message to X2 Module.
 *  \param p_src_cgi         Pointer to cell ID
 */
son_void_t
son_create_and_send_switch_off_ind_to_x2
(
    son_intra_rat_global_cell_id_t *p_src_cgi
);

/*! \fn son_void_t son_create_and_send_cell_switch_on_resp_to_oam(
 *          son_u16 transaction_id, son_intra_rat_global_cell_id_t *p_cell_id,
 *          son_return_et result,son_error_et error_code)
 *  \brief Generic function to construct and send cell switch on response
 *         to OAM.
 *  \param transaction_id   Transaction ID
 *  \param p_cell_id        Pointer to cell ID
 *  \param result           Succes/Failure as input
 *  \param error_code       Error Code
 */
son_void_t
son_create_and_send_cell_switch_on_resp_to_oam
(
    son_u16 transaction_id,
    son_intra_rat_global_cell_id_t      *p_cell_id,
    son_return_et result,
    son_error_et error_code
);

/*! \fn son_void_t son_create_and_send_cell_switch_off_resp_to_oam(
 *          son_u16 transaction_id,son_intra_rat_global_cell_id_t* p_cell_id,
 *          son_return_et result, son_error_et error_code)
 *  \brief Generic function to construct and send cell switch
 *         off response to OAM.
 *  \param transaction_id   Transaction ID
 *  \param p_cell_id        Pointer to cell ID
 *  \param result           Succes/Failure as input
 *  \param error_code       Error Code
 */
son_void_t
son_create_and_send_cell_switch_off_resp_to_oam
(
    son_u16 transaction_id,
    son_intra_rat_global_cell_id_t *p_cell_id,
    son_return_et result,
    son_error_et error_code
);

/*! \fn son_void_t son_create_and_send_x2_res_to_anr( son_u16 transaction_id,               son_return_et result, son_error_et error_code)
 *  \brief Generic function to construct and send cell switch
 *         on response for x2 setup request to X2 Module.
 *  \param transaction_id   Transaction ID
 *  \param result           Succes/Failure as input
 *  \param error_code       Error Code
 */
son_void_t
son_create_and_send_x2_res_to_anr
(
    son_u16 transaction_id,
    son_return_et result,
    son_error_et error_code
);

/*! \fn son_void_t son_create_and_send_cell_switch_on_off_ind_to_oam(
 *          son_return_et result,
 *          son_error_et error_code)
 *  \brief Generic function to construct and send cell switch
 *         on indication to OAM.
 *  \param None
 */
son_void_t
son_create_and_send_cell_switch_on_off_ind_to_oam
(
/* + SPR 17439 */
void
/* - SPR 17439 */
);

/* Fix 875 Start */
/*! \fn son_buf_retain_status_et es_fsm_cell_switch_off_req_handler(
 *          son_es_index_et     evnt_id,
 *          es_cell_context_t   *p_cell_ctxt,
 *          STACKAPIHDR         *p_cspl_hdr,
 *          son_u8              *p_msg)
 *  \brief Event Handler for SONES_CELL_SWITCH_OFF_REQ
 *  \param evnt_id Event ID for SONES_CELL_SWITCH_OFF_REQ
 *  \param p_cell_ctxt Pointer to the ES Cell Context
 *  \param p_cspl_hdr Pointer to the CSPL header
 *  \param p_msg Pointer to the message
 */
son_buf_retain_status_et
es_fsm_cell_switch_off_req_handler
(
    son_es_index_et     evnt_id,
    es_cell_context_t   *p_cell_ctxt,
    STACKAPIHDR         *p_cspl_hdr,
    son_u8              *p_msg
);
/* Fix 875 End */
/*SPR11689 changes start */
/* + SPR 17439 */
son_void_t
es_construct_and_send_cell_activation_response(void);
/* - SPR 17439 */
/*SPR11689 changes end */

#endif /* _SON_ES_CELL_FSM_H_ */
