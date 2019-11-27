/*! \file son_mro_cell_fsm.h
 *  \brief This file contains function prototype and enums related to MRO cell fsm.
 *  \date  Dec, 2012
 *  \author Ravi, Shilpi
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_MRO_CELL_FSM_H_
#define _SON_MRO_CELL_FSM_H_

/*! \headerfile son_utils.h <>
 */
#include <son_utils.h>

/*! \headerfile son_rrm_intf.h <>
 */
#include <son_rrm_intf.h>

/*! \headerfile son_mro_intf.h <>
 */
#include <son_mro_intf.h>

/*! \headerfile son_mro_ctxt_mgr.h <>
 */
#include <son_mro_ctxt_mgr.h>

/*! \def son_mro_cell_index_et
 *  \brief An enum that specifies the index value in MRO CELL FSM
 */
typedef enum son_mro_cell_index_et
{
    MRO_CELL_IDX_SONMRO_BASE = 0,
    MRO_CELL_IDX_SONMRO_EVENT_ENABLE_REQ = MRO_CELL_IDX_SONMRO_BASE,
    MRO_CELL_IDX_SONRMO_EVENT_DISABLE_REQ,
    MRO_CELL_IDX_SONMRO_EVENT_MODIFY_CONFIG_REQ,
    MRO_CELL_IDX_RRM_EVENT_REGISTER_RES,
    MRO_CELL_IDX_RRM_EVENT_DEREGISTER_RES,
    MRO_CELL_IDX_RRM_EVENT_HO_REPORT,
    MRO_CELL_IDX_RRM_EVENT_HO_ATTEMPTS,
    MRO_CELL_IDX_RRM_EVENT_CELL_STATE_CHANGE_IND,
    MRO_CELL_IDX_SONANR_CELL_ANR_MODE_IND,
    MRO_CELL_IDX_RRC_HO_REPORT_IND,
    MRO_CELL_IDX_MLB_NBR_CELL_STATUS_IND,
    MRO_CELL_NUM_OF_EVENTS,
    MRO_CELL_IDX_NOT_AVAILABLE  = 0xFF
}son_mro_cell_index_et;

/*! \fn son_return_et mro_construct_send_rrm_deregister_req(son_u16 transaction_id, son_intra_rat_global_cell_id_t *p_cell_id)
 *  \brief Constructs RRM_DEREGISTER_REQ
 *  \param transaction_id       Message transaction id
 *  \param p_cell_id            Pointer to cell id
 */
son_return_et
mro_construct_send_rrm_deregister_req
(
    son_u16 transaction_id,
    son_intra_rat_global_cell_id_t  *p_cell_id
);

/*! \fn son_void_t mro_cell_process_msg(STACKAPIHDR *p_cspl_hdr,
 * mro_cell_context_t *p_cell_ctxt, son_u8 *p_mro_msg)
 * \brief This is the entry point to the MRO Cell FSM. This function takes
 *         the API message received, CSPL header, cell context and calls
 *         the required event handler function.
 * \param p_cspl_hdr   Pointer to cspl header
 * \param p_cell_ctxt  Pointer to cell context
 * \param p_mro_msg    Pointer to MRO specific message
 */
son_void_t
mro_cell_process_msg
(
    STACKAPIHDR         *p_cspl_hdr,
    mro_cell_context_t  *p_cell_ctxt,
    son_u8              *p_mro_msg
);

#endif /* ifndef _SON_MRO_CELL_FSM_H_ */
