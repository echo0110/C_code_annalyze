/*! \file son_x2_enb_fsm.h
 *  \brief This file contains the enumerations and function prototypes of X2
 *  module eNB specific finite state machine.
 *  \date  Aug, 2012
 *  \author Shilpi
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_X2_ENB_FSM_H_
#define _SON_X2_ENB_FSM_H_

/*! \headerfile son_utils.h <>
 */
#include <son_utils.h>

/*! \headerfile son_x2_ctxt_mgr.h <>
 */
#include <son_x2_ctxt_mgr.h>

/*! \enum son_x2_enb_index_et
 *  \brief An enum that specifies the index value in SONX2 eNB FSM
 */
typedef enum son_x2_enb_index_e
{
    X2_ENB_IDX_BASE,

    X2_ENB_IDX_SONANR_ADD_ENB_REQ = X2_ENB_IDX_BASE,
    X2_ENB_IDX_RRC_ADD_ENB_RES,
    X2_ENB_IDX_RRC_LINK_UP_IND,
    X2_ENB_IDX_RRC_LINK_DOWN_IND,
    X2_ENB_IDX_SONANR_DELETE_ENB_REQ,
    X2_ENB_IDX_RRC_DELETE_ENB_RES,
    X2_ENB_IDX_MSG_ENB_CONFIG_UPDATE,
    X2_ENB_IDX_RRC_ENB_CONFIG_UPDATE_RES,
    X2_ENB_IDX_RRC_RESET_RES,
    X2_ENB_IDX_SONMLB_MOBILITY_CHANGE_REQ,
    X2_ENB_IDX_SONMLB_MOBILITY_CHANGE_ACK,
    X2_ENB_IDX_SONMLB_MOBILITY_CHANGE_FAILURE,
    X2_ENB_IDX_GU_GROUP_ID_CONFIG_REQ,
    /* SPR_19619 start */
    X2_ENB_IDX_PEER_CELL_ACTIVATION_REQ,
    /* SPR_19619 stop */
    X2_ENB_NUM_OF_EVENTS,
    X2_ENB_IDX_NOT_AVAILABLE  = 0xFF
}son_x2_enb_index_et;

/*! \fn son_x2_result_et x2_enb_process_msg(STACKAPIHDR *p_cspl_hdr, son_nbr_enb_hash_rec_t *p_enb_ctxt, son_u8 *p_x2_msg)
 *  \brief This function is used to invoke the appropriate event handlers based
 *  on the various messages received by the X2 module in different X2 ENB FSM
 *  states.
 *  \param p_cspl_hdr            Pointer to the CSPL header of message received by X2 module
 *  \param p_enb_ctxt           Pointer to the context of peer eNB to which message being processed is concerned
 *  \param p_x2_msg             Pointer to the message received
 */
son_x2_result_et
x2_enb_process_msg
(
    STACKAPIHDR             *p_cspl_hdr,
    son_nbr_enb_hash_rec_t  *p_enb_ctxt,
    son_u8                  *p_x2_msg
);

/* SPR Fix Start */

/*! \fn son_void_t x2_populate_temp_nbr_ecgi_enb_id_map(son_intra_rat_global_cell_id_t *p_src_cgi)
 *  \brief This function populates the nbr_ecgi_enb_id_map with all the neighbor cells ECGI and eNB IDs to send in eNB Config update request message to X2AP. This map is populated for the all the neighbors of all the serving cells which needs to be informed through eNB config update message and ensures that neighbors are not duplicate in the message.
 *  \param p_src_cgi    Pointer to the source cell ECGI
 */
son_void_t
x2_populate_temp_nbr_ecgi_enb_id_map
(
    son_intra_rat_global_cell_id_t      *p_src_cgi
);

/*! \fn son_void_t x2_populate_neigh_ecgi_enb_id_map(x2ap_neigh_ecgi_enb_id_map_list_t *p_neigh_ecgi_enb_id_map_list)
 *  \brief This function populates the neighbor cells ECGI and eNB IDs in the enb config update request message to be send to X2AP.
 *  \param p_neigh_ecgi_enb_id_map_list Pointer to the strucuting storing mapping of neighbor cells ECGI and eNB Ids
 */
son_void_t
x2_populate_neigh_ecgi_enb_id_map
(
    x2ap_neigh_ecgi_enb_id_map_list_t   *p_neigh_ecgi_enb_id_map_list
);

/* SPR Fix End */

#endif /* _SON_X2_ENB_FSM_H_ */
