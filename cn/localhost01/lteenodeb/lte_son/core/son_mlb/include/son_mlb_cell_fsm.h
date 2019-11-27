/*! \file son_mlb_cell_fsm.h
 *  \brief This file contains function prototype for entry point function for MLB Cell FSM
 *  \date  Dec, 2012
 *  \author Sajida
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_MLB_CELL_FSM_H_
#define _SON_MLB_CELL_FSM_H_

/*! \enum mlb_cell_fsm_state_et
 *  \brief An enum that specifies the SON MLB CELL FSM states
 */
#include <son_mlb_ctxt_mgr.h>
#include <son_mlb_intf.h>

#define MLB_CELL_NUM_OF_STATES  4
#define FIX_OFFSET_VALUE        0
#define HO_TRIGGER_RANGE        2
typedef enum _mlb_cell_fsm_event_e
{
    CELL_SONMLB_BASE = 0,
    CELL_SONMLB_ENABLE_REQ = CELL_SONMLB_BASE,
    CELL_SONMLB_DISABLE_REQ,
    CELL_SONMLB_MODIFY_REQ,
    CELL_RRM_REGISTER_RES,
    CELL_RRM_DEREGISTER_RES,
    CELL_RRM_SON_HO_REPORT,
    CELL_RRM_SON_LOAD_REPORT,
    CELL_SON_ANR_DISABLE_IND,
    CELL_SON_MLB_STATE_CHANGE_IND,
    CELL_MOBILITY_CHANGE_REQ,
    CELL_MOBILITY_CHANGE_FAILURE,
    CELL_MOBILITY_CHANGE_ACK,
/*Rel 3.0 Multi Sector code changes Start*/
    CELL_INTERNAL_MOBILITY_CHANGE_REQ,
    CELL_INTERNAL_MOBILITY_CHANGE_FAILURE,
    CELL_INTERNAL_MOBILITY_CHANGE_ACK,
/*Rel 3.0 Multi Sector code changes Stop*/
    CELL_SON_MLB_NBR_DELETE_IND,
    CELL_SON_MLB_OFFSET_CHANGE_REQ,
    CELL_SON_MLB_PEER_OFFSET_CHANGE_RESP,
/* eICIC feature code Starts */
    CELL_SON_EICIC_ENABLE_DISABLE_IND,
/* eICIC feature code Ends */
    SON_MLB_CELL_MAX_EVENT,
    CELL_SON_INVALID_EVENT = 0xff
}son_mlb_index_et;

typedef son_buf_retain_status_et (*mlb_cell_evt_handler_t)
(
    son_mlb_index_et evnt_id,
    mlb_cell_context_t *p_cell_ctxt,
    STACKAPIHDR *p_cspl_hdr,
    son_u8 *p_msg
);

son_buf_retain_status_et
mlb_fsm_state_change_handler
(
    son_mlb_index_et evnt_id,
    mlb_cell_context_t  *p_cell_ctxt,
    STACKAPIHDR         *p_cspl_hdr,
    son_u8              *p_msg
);

son_buf_retain_status_et
mlb_fsm_load_report_handler
(
    son_mlb_index_et evnt_id,
    mlb_cell_context_t   *p_cell_ctxt,
    STACKAPIHDR         *p_cspl_hdr,
    son_u8              *p_msg
);

son_buf_retain_status_et
mlb_fsm_ho_report_handler
(
    son_mlb_index_et evnt_id,
    mlb_cell_context_t   *p_cell_ctxt,
    STACKAPIHDR         *p_cspl_hdr,
    son_u8              *p_msg
);

son_buf_retain_status_et
mlb_fsm_modify_req_handler
(
    son_mlb_index_et evnt_id,
    mlb_cell_context_t   *p_cell_ctxt,
    STACKAPIHDR         *p_cspl_hdr,
    son_u8              *p_msg
);
son_buf_retain_status_et
mlb_fsm_disable_req_handler
(
    son_mlb_index_et evnt_id,
    mlb_cell_context_t   *p_cell_ctxt,
    STACKAPIHDR         *p_cspl_hdr,
    son_u8              *p_msg
);

son_buf_retain_status_et
mlb_fsm_nack_enable_req_handler
(
    son_mlb_index_et evnt_id,
    mlb_cell_context_t   *p_cell_ctxt,
    STACKAPIHDR         *p_cspl_hdr,
    son_u8                *p_msg
);
son_buf_retain_status_et
mlb_fsm_anr_disable_ind_handler
(
    son_mlb_index_et evnt_id,
    mlb_cell_context_t   *p_cell_ctxt,
    STACKAPIHDR         *p_cspl_hdr,
    son_u8                *p_msg
);
son_buf_retain_status_et
mlb_fsm_register_res_handler
(
    son_mlb_index_et evnt_id,
    mlb_cell_context_t   *p_cell_ctxt,
    STACKAPIHDR         *p_cspl_hdr,
    son_u8                *p_msg
);

son_buf_retain_status_et
mlb_fsm_deregister_res_handler
(
    son_mlb_index_et evnt_id,
    mlb_cell_context_t   *p_cell_ctxt,
    STACKAPIHDR         *p_cspl_hdr,
    son_u8                *p_msg
);
son_buf_retain_status_et
mlb_fsm_nack_disable_req_handler
(
    son_mlb_index_et evnt_id,
    mlb_cell_context_t  *p_cell_ctxt,
    STACKAPIHDR        *p_cspl_hdr,
    son_u8                *p_msg
);

son_bool_et
mlb_construct_send_rrm_register_req
(
    son_u16 transaction_id,
    son_intra_rat_global_cell_id_t  *p_cell_id
);

son_buf_retain_status_et
mlb_fsm_invalid_event_handler
(
    son_mlb_index_et evnt_id,
    mlb_cell_context_t     *p_cell_ctxt,
    STACKAPIHDR        *p_cspl_hdr,
    son_u8                *p_msg
);
son_buf_retain_status_et
mlb_fsm_enable_req_handler
(
    son_mlb_index_et evnt_id,
    mlb_cell_context_t  *p_cell_ctxt,
    STACKAPIHDR        *p_cspl_hdr,
    son_u8               *p_msg
);

son_buf_retain_status_et
mlb_cell_process_msg
(
    son_mlb_index_et evnt_id,
    mlb_cell_context_t   *p_cell_ctxt,
    STACKAPIHDR         *p_cspl_hdr,
    son_u8              *p_mlb_msg
);
son_return_et
mlb_construct_send_rrm_deregister_req
(
    son_u16 transaction_id,
    son_intra_rat_global_cell_id_t  *p_cell_id
);

son_void_t
convert_rrm_gl_cell_id_to_son_gl_cell_id
(
    son_global_cell_id_t        *p_son_gl_cell_id,
    rrm_oam_global_cell_id_t    *p_rrm_gl_cell_id
);

son_return_et
son_mlb_update_anr_table
(
    son_intra_rat_global_cell_id_t serv_cell_id,
    son_intra_rat_global_cell_id_t trgt_cell_id,
    son_q_offset_range_et cell_offset,
    son_u8 ocn_present,
    son_u8 nbr_csg_id_present
);

void
son_mlb_send_mobility_change_req
(
    mlb_cell_context_t *p_cell_ctxt,
    son_mlb_neigh_cell_info_t    *p_neigh_cell_info,
    son_s8 ho_trigger_change_nbr
);

son_void_t
son_mlb_send_mobility_change_fail
(
    mlb_cell_context_t *p_cell_ctxt,
    son_mlb_neigh_cell_info_t    *p_neigh_cell_info,
    /*SPR 20523 START*/
    /*code deleted*/
    /*SPR 20523 END*/
    son_mob_param_range_t *p_mob_range
);

son_void_t
son_mlb_send_mobility_change_ack
(
    mlb_cell_context_t *p_cell_ctxt,
    son_mlb_neigh_cell_info_t    *p_neigh_cell_info
);

son_buf_retain_status_et
mlb_fsm_nbr_update_info_handler
(
    son_mlb_index_et evnt_id,
    mlb_cell_context_t   *p_cell_ctxt,
    STACKAPIHDR         *p_cspl_hdr,
    son_u8                *p_msg
);

son_void_t
son_mlb_populate_source_enbId
(
    mlb_cell_context_t *p_cell_ctxt,
    son_global_enb_id_t                 *src_enbid,
    son_intra_rat_global_cell_id_t      *src_cgi1
);

son_void_t
son_mlb_populate_target_enbId
(
    son_mlb_neigh_cell_info_t    *p_neigh_cell_info,
    son_global_enb_id_t                 *trgt_enbid,
    son_intra_rat_global_cell_id_t      *src_cgi2
);

#endif /* ifndef _SON_MLB_CELL_FSM_H_ */
