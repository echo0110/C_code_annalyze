/*! \file son_anr_enb_ctxt_mgr.h
 *  \brief This file contains the prototypes of functions defined in son_anr_enb_ctxt_mgr.c
 *  \date  Sept, 2012
 *  \author Komal/Shekhar
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_ANR_ENB_CTXT_MGR_H_
#define _SON_ANR_ENB_CTXT_MGR_H_

/*! \headerfile son_anr_intf.h <>
 */
#include <son_anr_intf.h>

/*! \headerfile son_x2_ctxt_mgr.h <>
 */
#include <son_x2_ctxt_mgr.h>

/*! \headerfile son_anr_table.h <>
 */
#include <son_anr_table.h>

/*! \def SONANR_SET_ENB_PENDING_RES_ERROR_CODE(p_pending_resp_enb, error_code, api, tid)
 *  \brief This macro is used to set the error code in eNB Pending response
 */
#define SONANR_SET_ENB_PENDING_RES_ERROR_CODE(p_pending_resp_enb, error, api, tid) \
    do \
    { \
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, \
            SON_BRIEF, \
            "Error code set to %d for eNB [eNB Id: 0x%x] in Pending Response for API [API Id: %d] with TID [TID: %d]", \
            error, \
            son_convert_char_cell_id_to_int_cell_id(p_pending_resp_enb->nb_enb.enb_id.enb_id), \
            api, \
            tid); \
        p_pending_resp_enb->error_code = error; \
    } \
    while (0)

/*! \def SONANR_SET_ENB_PENDING_RES_EXPIRY_TIME(p_pending_resp_enb, expiry_interval,api,tid)
 *  \brief This macro is used to set the expiry time in eNB Pending response
 */
#define SONANR_SET_ENB_PENDING_RES_EXPIRY_TIME(p_pending_resp_enb, expiry_interval, api, tid) \
    do \
    { \
        SON_LOG( \
            anr_get_log_mode(), p_son_anr_facility_name, \
            SON_BRIEF, \
            "Expiry interval set to %d for eNB [eNB Id: 0x%x] in Pending Response for API [API Id: %d] with TID [TID: %d]", \
            expiry_interval, \
            son_convert_char_cell_id_to_int_cell_id(p_pending_resp_enb->nb_enb.enb_id.enb_id), \
            api, \
            tid); \
        p_pending_resp_enb->expiry_time = expiry_interval; \
        son_time(&p_pending_resp_enb->start_ts); \
    } \
    while (0)

/*! \def SONANR_INCR_X2_PENDING_RES_COUNT(p_pending_resp)
 *  \brief This macro is used to increment x2 expected pending response count
 */
#define SONANR_INCR_X2_PENDING_RES_COUNT(p_pending_resp) \
    do \
    { \
        if (p_pending_resp->expected_res_count < MAX_PENDING_RES_COUNT) \
        { \
            p_pending_resp->expected_res_count++; \
        } \
        else \
        { \
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, \
                SON_ERROR, \
                "Max value" \
                "of Expected Response Count in Pending Response reached\n"); \
        } \
    } \
    while (0)

/*! \def SONANR_DECR_X2_PENDING_RES_COUNT(p_pending_resp)
 *  \brief This macro is used to decrement x2 expected pending response count
 */
#define SONANR_DECR_X2_PENDING_RES_COUNT(p_pending_resp) \
    do \
    { \
        if (p_pending_resp->expected_res_count > MIN_PENDING_RES_COUNT) \
        { \
            p_pending_resp->expected_res_count--; \
        } \
        else \
        { \
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, \
                SON_ERROR, \
                "Max value" \
                "of Expected Response Count in Pending Response reached\n"); \
        } \
    } \
    while (0)

/*! \  anr_pending_nbr_enb_info_data_t
 *  \brief ANR eNB Specific Pending Response
 *  \param nb_enb       Neighbor eNB Information to which request is initiated
 *  \param error_code   Error_Code
 *  \param expiry_time  Timeout before which response is expected
 *  \param start_ts     Time stamp at which request was initiated
 *  \param p_enb_rec    Pointer the NRT hash record of the eNB
 */
typedef struct _anr_pending_nbr_enb_info_data_t
{
    son_neighbor_enb_info_t nb_enb;
    son_error_et error_code;
    son_u16 expiry_time;
    time_stamp_t start_ts;
    son_nbr_enb_hash_rec_t           *p_enb_rec;
}anr_pending_nbr_enb_info_data_t;

/*! \  anr_pending_nbr_enb_info_node_t
 *  \brief ANR eNB Specific Pending Response CSPL Tuple
 *  \param anchor    CSPL List Node
 *  \param data      eNB specific Pending Response data
 */
typedef struct _anr_pending_nbr_enb_info_node_t
{
    SON_LIST_NODE anchor;
    anr_pending_nbr_enb_info_data_t data;
}anr_pending_nbr_enb_info_node_t;

/*! \  anr_enb_pending_res_data_t
 *  \brief ANR API Specific Pending Response
 *  \param transaction_id       Transaction ID of initiating API
 *  \param api_id                               API Id of initiating API
 *  \param expected_res_count   Expected Response Count
 *  \param switch_on_resp_received   Switch ON response Received
 *  \param pending_nbr_enb_list List of eNB Specific Pending Response Data
 */
typedef struct _anr_enb_pending_res_data_t
{
    son_u16 transaction_id;
    son_procedure_code_et api_id;
    son_u8 expected_res_count;
    son_bool_et switch_on_resp_received;
    son_bool_et switch_on_sent;
    SON_LIST pending_nbr_enb_list;
}anr_enb_pending_res_data_t;

/*! \  anr_enb_pending_res_node_t
 *  \brief ANR API Specific Pending Response CSPL Tuple
 *  \param anchor    CSPL List Node
 *  \param data      API specific Pending Response data
 */
typedef struct _anr_enb_pending_res_node_t
{
    SON_LIST_NODE anchor;
    anr_enb_pending_res_data_t data;
}anr_enb_pending_res_node_t;

/*! \fn anr_enb_pending_res_data_t*  anr_add_x2_pending_resp_list(son_u16 tid,
 *                                                                                                                                son_procedure_code_et api)
 *  \brief   This function add API to X2 Pending Response List
 *  \param tid - Transaction ID
 *  \param api - API ID
 */
anr_enb_pending_res_data_t *
anr_add_x2_pending_resp_list
(
    son_u16 tid,
    son_procedure_code_et api
);

/*! \fn const son_void_t * anr_key_of_x2_list (const SON_LIST_NODE *p_node)
 *  \brief  This function determines the key of X2 Pending Res list
 *  \param p_node - anchor field of the X2 Pending Res list record
 */
const son_void_t *
anr_key_of_x2_list
(
    const SON_LIST_NODE *p_node
);

/*! \fn const son_void_t * anr_key_of_enb_list (const SON_LIST_NODE *p_node)
 *  \brief  This function determines the key of eNB Pending Res list
 *  \param p_node -  anchor field of the X2 Pending Res list record
 */
const son_void_t *
anr_key_of_enb_list
(
    const SON_LIST_NODE *p_node
);

/*! \fn son_s32 anr_compare_enb_id (const son_void_t *p_val_1, const son_void_t *p_val_2)
 *  \brief   This function to compares the eNB Id
 *  \param p_val_1 - Pointer to enB Id 1
 *  \param p_val_2 - Pointer to enB Id 2
 */
son_s32
anr_compare_enb_id
(
    const son_void_t *p_val_1,
    const son_void_t *p_val_2
);

/*! \fn anr_enb_pending_res_node_t * anr_find_x2_pending_resp_list (son_u16 tid)
 *  \brief   This function finds element in X2 Pending Response List
 *            with matching tid
 *  \param tid - Transaction Id
 */
anr_enb_pending_res_node_t *
anr_find_x2_pending_resp_list
(
    son_u16 tid
);

/*! \fn son_void_t anr_del_x2_pending_resp_list(son_u16 tid)
 *  \brief This function deletes element from X2 Pending Response List
 *          with the matching TID.
 *  \param tid - Transaction Id
 */
son_void_t
anr_del_x2_pending_resp_list
(
    son_u16 tid
);

/*! \fn anr_pending_nbr_enb_info_data_t * anr_find_enb_x2_pending_resp_list
 *    (son_global_enb_id_t *p_enb_id, anr_enb_pending_res_data_t **p_pending_resp)
 *  \brief   This function finds enB in X2 Pending Response List
 *  \param p_enb_id - Pointer to eNB Id to be searched
 *  \param p_pending_resp - Double Pointer to X2 pending response
 *                          element in which eNB is present
 */
anr_pending_nbr_enb_info_data_t *
anr_find_enb_x2_pending_resp_list
(
    son_global_enb_id_t *p_enb_id,
    anr_enb_pending_res_data_t * *p_pending_resp
);

/*! \fn anr_pending_nbr_enb_info_data_t* anr_add_enb_x2_pending_resp_elem (
 *      anr_enb_pending_res_data_t *p_pending_resp,
 *      son_neighbor_enb_info_t *p_enb_info,
 *      son_nbr_enb_hash_rec_t *p_enb_rec,
 *      son_error_et error_code)
 *  \brief   This function add enB in X2 Pending Response element
 *  \param p_pending_resp - Pointer to X2 pending Response element
 *  \param p_enb_info     - Pointer to eNB Info of eNB to be added
 *  \param p_enb_rec      - POinter to Hash Table rec of the eNB to be added
 *  \param error_code     - Error Code to be set
 */
anr_pending_nbr_enb_info_data_t *
anr_add_enb_x2_pending_resp_elem
(
    anr_enb_pending_res_data_t *p_pending_resp,
    son_neighbor_enb_info_t *p_enb_info,
    son_nbr_enb_hash_rec_t *p_enb_rec,
    son_error_et error_code
);

/*! \fn son_void_t	anr_del_enb_x2_pending_resp_elem (
 *      anr_enb_pending_res_data_t *p_pending_resp,
 *      son_global_enb_id_t *p_enb_id)
 *  \brief  This function deletes enB record in X2 Pending Response element
 *  \param p_pending_resp - Pointer to X2 pending Response element
 *  \param p_enb_id       - Pointer to eNB Id of eNB to be deleted
 */
son_void_t
anr_del_enb_x2_pending_resp_elem
(
    anr_enb_pending_res_data_t *p_pending_resp,
    son_global_enb_id_t *p_enb_id
);

/*! \fn anr_pending_nbr_enb_info_data_t *
 *      anr_find_enb_x2_pending_resp_elem (
 *      anr_enb_pending_res_data_t *p_pending_resp,
 *              son_global_enb_id_t *p_enb_id)
 *  \brief   This function finds enB in X2 Pending Response element
 *  \param p_pending_resp - Pointer to X2 pending Response element
 *  \param p_enb_id       - Pointer to eNB Id of eNB to be found
 */
anr_pending_nbr_enb_info_data_t *
anr_find_enb_x2_pending_resp_elem
(
    anr_enb_pending_res_data_t *p_pending_resp,
    son_global_enb_id_t *p_enb_id
);

/*! \fn nr_enb_pending_res_node_t * anr_get_first_x2_pending_resp_list()
 *  \brief This function gets first element X2 Pending Response List
 *  \param None
 */
anr_enb_pending_res_node_t *
anr_get_first_x2_pending_resp_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn nr_enb_pending_res_node_t * anr_get_next_x2_pending_resp_list(
 *          anr_enb_pending_res_node_t *p_node )
 *  \brief This function gets next element X2 Pending Response List
 *  \param p_node - Pointer to the current node
 */
anr_enb_pending_res_node_t *
anr_get_next_x2_pending_resp_list
(
    anr_enb_pending_res_node_t *p_node
);

/*! \fn anr_pending_nbr_enb_info_node_t *
 *  anr_get_first_enb_x2_pending_resp_elem(anr_enb_pending_res_data_t *p_pending_resp)
 *  \brief This function gets first element from eNB list
 *         in X2 Pending Response element
 *  \param p_pending_resp -  Pointer to the X2 Pending response element
 */
anr_pending_nbr_enb_info_node_t *
anr_get_first_enb_x2_pending_resp_elem
(
    anr_enb_pending_res_data_t *p_pending_resp
);

/*! \fn anr_pending_nbr_enb_info_node_t *
 *      anr_get_next_enb_x2_pending_resp_elem(anr_pending_nbr_enb_info_node_t *p_node)
 *  \brief This function gets first element from eNB list
 *         in X2 Pending Response element
 *  \param p_node - Pointer to the eNB node in eNB List in X2 Pending response element
 */
anr_pending_nbr_enb_info_node_t *
anr_get_next_enb_x2_pending_resp_elem
(
    anr_pending_nbr_enb_info_node_t *p_node
);

/*! \fn son_void_t anr_clear_x2_pending_resp_list()
 *  \brief This function clears the entire X2 Pending Response list
 *  \param None
 */
son_void_t
anr_clear_x2_pending_resp_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t anr_clear_enb_list_x2_pending_resp_elem(SON_LIST *p_list)
 *  \brief This function clears eNB List in X2 Pending Response element
 *  \param p_list - Pointer to list
 */
son_void_t
anr_clear_enb_list_x2_pending_resp_elem
(
    SON_LIST *p_list
);

/*! \fn son_void_t anr_construct_send_message_to_x2 (
 *      anr_enb_pending_res_data_t *p_pending_resp)
 *  \brief This function constructs and sends message to SON_X2 if required
 *  \param p_pending_resp - Pointer to the X2 Pending response element
 */
son_void_t
anr_construct_send_message_to_x2
(
    anr_enb_pending_res_data_t *p_pending_resp
);

/*! \fn son_void_t anr_construct_send_message_to_rrm (
 *              anr_pending_nbr_enb_info_data_t *p_pending_resp_enb,
 *                              anr_enb_pending_res_data_t *p_pending_resp)
 *  \brief This function constructs and sends TNL Discovery message to RRM module
 *                 for eNB element in Pending Response element
 *  \param p_pending_resp_enb - Pointer to the eNB in eNB List in X2 Pending Response element
 *  \param p_pending_resp - Pointer to the X2 Pending Response element
 */
son_void_t
anr_construct_send_message_to_rrm
(
    anr_pending_nbr_enb_info_data_t *p_pending_resp_enb,
    anr_enb_pending_res_data_t *p_pending_resp
);

/*! \fn son_void_t anr_construct_send_message_to_oam(anr_enb_pending_res_data_t *p_pending_resp)
 *  \brief This function constructs and sends response message to OAM module
 *                 for X2 Pending Response element
 *  \param p_pending_resp - Pointer to the X2 Pending Response element
 */
son_void_t
anr_construct_send_message_to_oam
(
    anr_enb_pending_res_data_t *p_pending_resp
);

/*! \fn son_void_t anr_neighbor_eNB_x2_setup(anr_intra_rat_hash_rec_t *p_nbr)
 *  \brief This function initiates X2 Setup for the newly added neighbor
 *  \param p_nbr - Intra Rat Neighbor Hash Record
 */
son_void_t
anr_neighbor_eNB_x2_setup
(
    anr_intra_rat_hash_rec_t *p_nbr
);

/*! \fn son_void_t anr_x2_pending_resp_timer_expiry ()
 *  \brief This function does X2 Pending response cleanup on timer expiry
 *  \param None
 */
son_void_t
anr_x2_pending_resp_timer_expiry
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t  anr_populate_x2_nrt_update_ind(son_x2_nrt_update_ind_t *p_nrt_upd_ind,
 *          son_intra_rat_global_cell_id_t      *p_cell_id,
 *          son_nrt_operation_performed_et  nr_op);
 *  \brief This function populates the  nrt update indication msg
 *  \param p_nrt_upd_ind - Pointer to nrt update ind msg.
 *  \param p_cell_id     - Pointer to intra rat global cell id struct.
 *  \param nr_op         - operation performed
 */
son_void_t
anr_populate_x2_nrt_update_ind
(
    son_x2_nrt_update_ind_t *p_nrt_upd_ind,
    son_intra_rat_global_cell_id_t      *p_cell_id,
    son_nrt_operation_performed_et nr_op
);

/*! \fn son_void_t anr_process_x2_link_up_ind_for_nrt_updation(x2ap_served_cell_info_arr_t  *p_srv_cell_info,
 *                                                             son_u16                       srv_cell_list,
 *                                                             son_global_enb_id_t          *p_enb_id,
 *                                                             son_nbr_enb_hash_rec_t       *p_enb_hash_rec,
 *                                                             son_bool_et                  is_x2_conn_triggred_by_oam)
 *  \brief This function processs  x2 link up  indication msg
 *  \param  p_srv_cell_info  - Pointer to the list of neighbors in X2 link up indication to be added in NRT
 *  \param srv_cell_list     - List size
 *  \param p_enb_id          - Pointer to global eNB id
 *  \param p_enb_hash_rec    - Pointer to enb hash record
 *  \param is_x2_conn_triggred_by_oam - booloean value to determine this message is triggered due to peer enb.
 */
son_void_t
anr_process_x2_link_up_ind_for_nrt_updation
(
    x2ap_served_cell_info_arr_t  *p_srv_cell_info,
    son_u16 srv_cell_list,
    son_global_enb_id_t          *p_enb_id,
    son_nbr_enb_hash_rec_t       *p_enb_hash_rec
/*SPR 17777 +-*/ 
);

/*! \fn son_bool_et anr_traverse_enb_config_update_ind_del_list(anr_cell_context_t  *p_cell_ctxt,
 *                                                             x2ap_served_cells_to_delete_list_t *p_del_list,
 *                                                             son_global_enb_id_t          *p_enb_id,
 *                                                             son_anr_updated_nrt_info_t   *p_oam_notif,
 *                                                             son_anr_rrm_updated_nrt_info_t *p_rrm_notif)
 *  \brief This function traverses all the neigbors in the eNB config update delete list to deletes NRs in the NRT
 *  \param  p_cell_ctxt      - Cell Context pointer
 *  \param p_del_list        - Neighbor List from L3 to be deleted in NRT
 *  \param p_enb_id          - Pointer to global eNB id
 *  \param p_oam_notif       - Pointer to oam notification msg
 *  \param p_rrm_notif       - Pointer to rrm notification msg
 */
son_bool_et
anr_traverse_enb_config_update_ind_del_list
(
    anr_cell_context_t                      *p_cell_ctxt,
    x2ap_served_cells_to_delete_list_t      *p_del_list,
    son_global_enb_id_t                     *p_enb_id,
    son_anr_updated_nrt_info_t              *p_oam_notif,
    son_anr_rrm_updated_nrt_info_t          *p_rrm_notif
);

/* ! \fn son_bool_et anr_traverse_enb_config_update_ind_modify_list(
 * anr_cell_context_t                      *p_cell_ctxt,
 * x2ap_served_cells_to_modify_list_t      *p_modify_list,
 * son_x2_status_et                        x2_status,
 * son_global_enb_id_t                     *p_enb_id,
 * son_anr_updated_nrt_info_t              *p_add_nr_list,
 * son_anr_updated_nrt_info_t              *p_upd_nr_list,
 * son_anr_updated_nrt_info_t              *p_del_nr_list,
 * son_anr_rrm_updated_nrt_info_t          *p_intra_rat_ncl);
 *  \brief  This function traverses all the neigbors in the eNB config update
 *           modify list to modify/delete NRs in the NRT
 *  \param  p_cell_ctxt - Cell Context pointer
 *  \param  p_modify_list - Neighbor List from L3 to be modifed in NRT
 *  \param  x2_status   - X2 status value
 *  \param  p_enb_id  -  Pointer to the eNB Id
 *  \param  p_oam_notif_for_add - Pointer to oam notification msg for add nr
 *  \param  p_oam_notif_for_upd - Pointer to oam notification msg for upd nr
 *  \param  p_oam_notif_for_del - Pointer to oam notification msg for del nr
 *  \param  p_rrm_notif         - Pointer to rrm notification msg
 */
son_bool_et
anr_traverse_enb_config_update_ind_modify_list
(
    anr_cell_context_t                      *p_cell_ctxt,
    x2ap_served_cells_to_modify_list_t      *p_modify_list,
    /*SPR 17777 +-*/
    /* SPR_19279_START */
    son_global_enb_id_t                     *p_old_enb_id,
    son_global_enb_id_t                     *p_new_enb_id,
    /* SPR_19279_END */
    son_anr_updated_nrt_info_t              *p_oam_notif_for_add,
    son_anr_updated_nrt_info_t              *p_oam_notif_for_upd,
    son_anr_updated_nrt_info_t              *p_oam_notif_for_del,
    son_anr_rrm_updated_nrt_info_t          *p_rrm_notif
);

/* ! \fn  son_bool_et anr_traverse_enb_config_update_add_list(
 * anr_cell_context_t                      *p_cell_ctxt,
 * x2ap_served_cells_to_add_list_t         *p_add_list,
 * son_x2_status_et                        x2_status,
 * son_global_enb_id_t                     *p_enb_id,
 * son_anr_updated_nrt_info_t              *p_add_nr_list,
 * son_anr_updated_nrt_info_t              *p_upd_nr_list,
 * son_anr_rrm_updated_nrt_info_t          *p_intra_rat_ncl
 * );
 *  \brief  This function traverses all the neigbors in the eNB config update
 *          add list to add them in NRT.
 *  \param  p_cell_ctxt - Cell Context pointer
 *  \param  p_add_list - Neighbor List from L3 to be added in NRT
 *  \param  x2_status   - X2 status value
 *  \param  p_enb_id  -  Pointer to the eNB Id
 *  \param  p_oam_notif_for_add - Pointer to oam notification msg for add
 *  \param  p_oam_notif_for_upd - Pointer to oam notification msg for upd
 *  \param  p_rrm_notif         - Pointer to rrm notification msg
 */
son_bool_et
anr_traverse_enb_config_update_add_list
(
    anr_cell_context_t                      *p_cell_ctxt,
    x2ap_served_cells_to_add_list_t         *p_add_list,
    /*SPR 17777 +-*/
    son_global_enb_id_t                     *p_enb_id,
    son_anr_updated_nrt_info_t              *p_add_nr_list,
    son_anr_updated_nrt_info_t              *p_upd_nr_list,
    son_anr_rrm_updated_nrt_info_t          *p_intra_rat_ncl
);

/*! \fn son_void_t anr_populate_send_x2_nrt_update_ind (
 *   son_x2_nrt_update_ind_t *p_nrt_upd_ind,
 *   son_intra_rat_global_cell_id_t      *p_cell_id,
 *   son_nrt_operation_performed_et  nr_op);
 *  \brief  This function populates and sends the  nrt update indication msg.
 *  \param  p_nrt_upd_ind  - Pointer to x2 nrt update ind msg
 *  \param  p_cell_id      - Pointer to intra rat global cell id
 *  \param  nr_op         - operation performed
 */
son_void_t
anr_populate_send_x2_nrt_update_ind
(
    son_x2_nrt_update_ind_t *p_nrt_upd_ind,
    son_intra_rat_global_cell_id_t      *p_cell_id,
    son_nrt_operation_performed_et nr_op
);

/*! \fn son_void_t anr_determine_enb_id(
 *                                  anr_intra_rat_hash_rec_t p_nbr,
 *                                  son_global_enb_id_t p_enb_id)
 *  \brief This function determines eNB Id from ECGI
 *  \param  p_nbr - Pointer to hash record
 *  \param  p_enb_id - Pointer to eNB id
 */
son_void_t
anr_determine_enb_id
(
    anr_intra_rat_hash_rec_t *p_nbr,
    son_global_enb_id_t *p_enb_id
);

/*! \fn son_void_t anr_process_inra_rat_nbr_nbrs(
 *      anr_intra_rat_hash_rec_t *p_nr,
 *      x2ap_nbour_cell_info_t *p_nbrs_nbr_list);
 *  \brief This function processes intra rat nbr
 *  \param  p_nr            - Pointer to intra rat hash table
 *  \param  p_nbrs_nbr_list - Pointer to x2 nbr list
 */
son_void_t
anr_process_inra_rat_nbr_nbrs
(
    anr_intra_rat_hash_rec_t *p_nr,
    /* Spr 17753 Changes Start */
    x2ap_nbour_cell_info_t *p_nbrs_nbr_list,
    anr_cell_context_t *p_cell_ctxt
    /* Spr 17753 Changes End */
);

/*! \fn son_x2_status_et
 * get_x2_sts_value_for_nbr_from_enb_rec (
 * son_nbr_enb_hash_rec_t *p_enb_hash_rec );
 *  \brief  This function determines the HO_status and Connection status
 *          for the neighbor X2_status filed.
 *          (x2_status in nbrs  = ho_status + connection_status)
 *  \param  p_enb_hash_rec    - Pointer to enb hash record
 */
son_x2_status_et
get_x2_sts_value_for_nbr_from_enb_rec
(
    son_nbr_enb_hash_rec_t *p_enb_hash_rec
);

/*! \fn son_void_t  anr_determine_macro_enb_id(
 *                  son_intra_rat_global_cell_id_t *p_cell_id,
 *                  son_global_enb_id_t *p_enb_id)
 *  \brief This function determines eNB Id from ECGI
 *  \param  p_cell_id - Pointer to cell id
 *  \param  p_enb_id - Pointer to eNB id
 */
son_void_t
anr_determine_macro_enb_id
(
    son_intra_rat_global_cell_id_t *p_cell_id,
    son_global_enb_id_t *p_enb_id
);

/*! \fn son_bool_et anr_insert_in_nr_nbr_list(SON_LIST *p_list,
 *                    x2ap_nbour_info_t * p_x2ap_nbr_info)
 * \brief  -This function inserts the NR nbrs in the nr nbr list
 * \param p_list  Pointer to nr nbr list
 * \param p_x2ap_nbr_info Pointer to the L3 nr nbr
 */
son_bool_et
anr_insert_in_nr_nbr_list
(
    SON_LIST *p_list,
    /* Spr 17753 Changes Start */
    x2ap_nbour_info_t *p_x2ap_nbr_info,
    anr_cell_context_t *p_cell_ctxt
    /* Spr 17753 Changes End */
);

/*! \fn anr_nr_nbr_list_node_t* anr_get_next_nr_nbr(anr_nr_nbr_list_node_t  *p_node)
 * \brief  -This function determine the next element in the nr neighbor List
 * \param p_node  Pointer to the current nr nbr list element
 */
anr_nr_nbr_list_node_t *
anr_get_next_nr_nbr
(
    anr_nr_nbr_list_node_t  *p_node
);

/*! \fn  anr_nr_nbr_list_node_t* anr_get_first_nr_nbr( SON_LIST *p_list)
 * \brief  -This function determine the first element in the nr neighbor List
 * \param p_list  Pointer to the current nr nbr list
 */
anr_nr_nbr_list_node_t *
anr_get_first_nr_nbr
(
    SON_LIST *p_list
);

/*! \fn son_void_t anr_clear_nr_nbrs_list(SON_LIST *p_list)
 * \brief  - This function clears the entire NR nbrs list for a configured cell
 * \param p_list  Pointer to the current nr nbr list
 */
son_void_t
anr_clear_nr_nbrs_list
(
    SON_LIST *p_list
);

son_bool_et
is_x2_connection_available
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/* SPR-10331 Fix Starts */
/*! \fn son_void_t anr_send_enb_config_update_ind_to_rrm(STACKAPIHDR *p_cspl_hdr, son_global_enb_id_t *p_peer_enb_id, son_void_t *p_l3_ind)
 * \brief  - This is the function which is used to forward the GU Group Id information recevied from L3 to RRM
 * \param p_cspl_hdr        Pointer to the CSPL Header Information
 * \param p_peer_enb_id     Pointer to peer eNB Id
 * \param p_l3_ind          Pointer to the incoming message from L3 X2AP
 */
son_void_t
anr_send_enb_config_update_ind_to_rrm
(
    STACKAPIHDR         *p_cspl_hdr,
    son_global_enb_id_t *p_peer_enb_id,
    son_void_t          *p_l3_ind
);
/* SPR-10331 Fix Ends */

/* SPR 15698 Start */
/*****************************************************************************
 * Function Name  : anr_cleanup_pending_cgi_request
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t - None
 * Description    : This function is used to implementa an Audit kind of funtionality
 *                  which is used to clean up the pending cgi nodes if no cgi measurement
 *                  received for some predefined time.
 ******************************************************************************/
/* + SPR 17439 */
son_void_t anr_cleanup_pending_cgi_request(void);
/* - SPR 17439 */
/* SPR 15698 End*/

/* Spr 16142 Fix Start */
/*! \fn son_void_t anr_construct_send_stop_tnl_discovery_ind_to_rrm(son_global_enb_id_t *p_enb_id)
 *  \brief This function constructs and sends tnl stop ind to RRM module
 *                    for nbr enb
 *  \param p_enb_id - Pointer to the enb id for wich tnl discovery needs to stop
 */
son_void_t
anr_construct_send_stop_tnl_discovery_ind_to_rrm
(
 son_global_enb_id_t *p_enb_id
 );
/* Spr 16142 Fix End */
/* Spr 17753 Changes Start */
/*! \fn son_void_t anr_insert_node_in_secode_degree_nbr_list
     (SON_LIST *p_second_degree_nbr_list,anr_second_degree_nr_list_node_t *p_list_node)
 *  \brief This function inserts a node in second degree nbr list
 */ 
son_void_t
anr_insert_node_in_secode_degree_nbr_list
(
 SON_LIST *p_second_degree_nbr_list,
 anr_second_degree_nr_list_node_t *p_list_node
 );

/*! \fn anr_second_degree_nr_list_node_t* anr_find_record_in_second_degree_nbr_list
     (SON_LIST *p_list, son_intra_rat_global_cell_id_t *p_cgi)
 *  \brief This function search a node in second degree nbr list
 */ 
anr_second_degree_nr_list_node_t *
anr_find_record_in_second_degree_nbr_list
(
 SON_LIST                            *p_list,
 son_intra_rat_global_cell_id_t      *p_cgi
 );

/*! \fn son_void_t* anr_key_of_second_degree_list(const SON_LIST_NODE *p_node)
 *  \brief This function generates the key 
 */ 
const son_void_t *
anr_key_of_second_degree_list
(
 const SON_LIST_NODE *p_node
 );

/*! \fn son_s32* anr_compare_cgi_value_second_degree_list_node(const son_void_t *p_val_1, const son_void_t *p_val_2)
 *  \brief This function compares the key field(cgi) of the nbr node in the list 
 */ 
son_s32
anr_compare_cgi_value_second_degree_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
);

/*! \fn son_return_et anr_insert_neigh_info_in_link_list
 *  \brief This function inserts a node in second degree nbr list 
 */ 
son_return_et
anr_insert_neigh_info_in_link_list
(
 x2ap_served_cell_info_arr_t  *p_srv_cell_info,
 anr_cell_context_t            *p_cell_ctxt,
 son_global_enb_id_t          *p_enb_id
 );

/*! \fn son_void_t anr_find_conflict_in_nrt_n_send_conflict_ind
 *  \brief This function finds the conflict in NRT and either send conflict ind
 */ 
son_void_t
anr_find_conflict_in_nrt_n_send_conflict_ind
(
 anr_cell_context_t *p_cell_ctxt
 );

/*! \fn son_void_t anr_handle_nr_add_timer_expiry
 *  \brief This function finds the conflict in NRT and either send conflict ind
 */ 
son_void_t
anr_handle_nr_add_timer_expiry
(
 anr_cell_context_t * p_cell_context
 );

/*! \fn son_bool_et anr_check_if_nr_add_timer_or_conflict_back_off_timer_expired
 *  \brief This function is used to check if NR add timer is expired 
 */ 
son_bool_et
anr_check_if_nr_add_timer_or_conflict_back_off_timer_expired
(
 son_timer_t timer_id,     /* Identity of the expired timer */
 anr_cell_context_t ** p_cell_context, /* Pointer to cell context */
 anr_timer_expiry_flag_et * p_expiry_flag /* Pointer to flag for timer type */
 );

/*! \fn son_void_t anr_handle_conflict_back_off_timer_expiry
 *  \brief This function is used handle conflict back off timer 
 */ 
son_void_t
anr_handle_conflict_back_off_timer_expiry
(
 anr_cell_context_t * p_cell_context
 );
/* Spr 17753 Changes End */

/*SPR 22419 Fix Start*/
son_u32
  anr_get_intf_scheme
  (
  son_u32     access_mode
  );
/*SPR 22419 Fix Stop*/

#endif /* ifndef _SON_ANR_ENB_CTXT_MGR_H_ */
