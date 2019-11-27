/*! \file son_anr_cell_fsm.h
 *  \brief This file contains function prototype for entry point function for ANR Cell FSM
 *  \date  Feb, 2012
 *  \author Komal/Shekhar
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_ANR_CELL_FSM_H_
#define _SON_ANR_CELL_FSM_H_

/*! \headerfile son_defines.h <>
 */
#include <son_defines.h>

/*! \headerfile son_rrm_intf.h <>
 */
#include <son_rrm_intf.h>

/*! \headerfile son_anr_intf.h <>
 */
#include <son_anr_intf.h>

/*! \headerfile son_anr_table.h <>
 */
#include <son_anr_table.h>

/*! \headerfile son_anr_ctxt_mgr.h <>
 */
#include <son_anr_ctxt_mgr.h>

/* SPR 20653 Fix Start */
/* Code Removed */
/* SPR 20653 Fix End */

/*! \def son_anr_index_et
 *  \brief An enum that specifies the index value in CELL FSM
 */
typedef enum son_anr_index_et
{
    IDX_SONANR_BASE = 0,
    IDX_SONANR_ENABLE_REQ = IDX_SONANR_BASE,
    IDX_SONANR_DISABLE_REQ,
    IDX_SONANR_NR_ADD_REQ,
    IDX_SONANR_NR_DELETE_REQ,
    IDX_SONANR_NR_UPDATE_REQ,
    IDX_SONANR_NR_RETRIEVE_REQ,
    IDX_RRM_REGISTER_RES,
    IDX_RRM_DEREGISTER_RES,
    IDX_RRM_CELL_STATE_CHANGE_IND,
    IDX_SON_ANR_RRM_MEAS_CONFIG_REQ,
    IDX_RRM_SON_MEAS_CONFIG_RESP,
    IDX_SONANR_MODIFY_UE_COUNT_REQ,
    IDX_SONANR_MODIFY_MEAS_REPORTING_INTERVAL_REQ,
    IDX_RRM_SONANR_SET_ATTRIBUTE_RESP,
    IDX_RRM_MEAS_RESULT_IND,
    IDX_SONANR_NR_DELETE_FROM_REMOVE_LIST_REQ,
    IDX_SONANR_MODIFY_REMOVAL_ATTRIBUTES_REQ,
    IDX_SONANR_MODIFY_PCI_CONFUSION_CFG_REQ,
    IDX_SONANR_ENABLE_DISABLE_LIMITED_MODE_REQ,
    IDX_RRM_HO_REPORT,
    IDX_RRM_HO_ATTEMPTS,
    IDX_RRM_HO_FAILURE_REPORT,
    IDX_X2_HO_FALIURE_REPORT,
    IDX_SON_ANR_RRM_MEAS_CONFIG_OBJ_REMOVE_REQ,
    IDX_RRM_SON_MEAS_CONFIG_OBJ_REMOVE_RESP,
    /* SPR 20653 Fix Start */
    IDX_SONANR_MODIFY_ATTRIBUTE_REQ,
    IDX_SONANR_MAX,
    /* SPR 20653 Fix End */

    IDX_NOT_AVAILABLE  = 0xFF
}son_anr_index_et;
#define SON_MODIFY_UE_COUNT_PRESENT 0x01
#define SON_MODIFY_REPORT_INTERVAL_PRESENT  0x02
#define SON_MODIFY_DL_THRESHOLD_PRESENT 0x04
#define SON_MODIFY_UL_THRESHOLD_PRESENT 0x08
typedef struct _son_modify_attribute_t
{
    son_u32                     bitmask;    /*^ BITMASK ^*/
    son_u16 reporting_interval; /*^ O, SON_MODIFY_REPORT_INTERVAL_PRESENT, B, 60, 1440 ^*/ 
    son_u8 ue_cnt;  /*^ O, SON_MODIFY_UE_COUNT_PRESENT, B, 1, 10 ^*/
    son_u8 ue_throughput_dl_anr_threshold;    
    /*^ O, SON_MODIFY_DL_THRESHOLD_PRESENT, B, 1, 255 ^*/
    son_u8 ue_throughput_ul_anr_threshold;     
    /*^ O, SON_MODIFY_UL_THRESHOLD_PRESENT, B, 1, 255 ^*/

}son_modify_attribute_t;
/* SPR 20653 Fix Start */
/*! \fn son_buf_retain_status_et anr_cell_process_msg (STACKAPIHDR *p_cspl_hdr, son_anr_index_et evnt_id,
 *     anr_cell_context_t *p_cell_ctxt, son_u8 *p_anr_msg)
 *  \brief This is the entry point function for ANR Cell FSM
 *  \param p_cspl_hdr          Pointer to CSPL Header
 *  \param evnt_id             event identity
 *  \param p_cell_ctxt         Pointer to cell context
 *  \param p_anr_msg           Pointer to incoming message
 */
son_buf_retain_status_et
anr_cell_process_msg
(
    STACKAPIHDR         *p_cspl_hdr,
    son_anr_index_et evnt_id,
    anr_cell_context_t  *p_cell_ctxt,
    son_u8                              *p_anr_msg
);

/*! \fn son_return_et anr_construct_send_rrm_deregister_req(son_u16 transaction_id,
 *                                                          son_intra_rat_global_cell_id_t  *p_cell_id)
 *  \brief Constructs and sends RRM_DEREGISTER_REQ
 *  \param transaction_id       transaction identity
 *  \param p_cell_id			Pointer to Cell_Id
 */
son_return_et
anr_construct_send_rrm_deregister_req
(
    son_u16 transaction_id,
    son_intra_rat_global_cell_id_t  *p_cell_id
);

/*! \fn son_void_et convert_rrm_gl_cell_id_to_son_gl_cell_id(son_global_cell_id_t   *p_son_gl_cell_id,
 *                                                           rrm_oam_global_cell_id_t   *p_rrm_gl_cell_id);
 *  \brief Convert RRM ECGI to SON ECGI
 *  \param p_son_gl_cell_id     global cell id for SON
 *  \param p_rrm_gl_cell_id     global cell id for RRM
 */

son_void_t
convert_rrm_gl_cell_id_to_son_gl_cell_id
(
    son_global_cell_id_t        *p_son_gl_cell_id,
    rrm_oam_global_cell_id_t    *p_rrm_gl_cell_id
);

/*! \fn son_error_et anr_map_rrm_error_codes(const son_8* son_func_name,
 *                                           son_return_et result,
 *                                           rrm_error_et rrm_error_code,
 *                                           rrm_son_message_resp_et msg_id);
 *  \brief This function maps and returns SON error code for the received RRM error code
 *  \param son_func_name        calling function name
 *  \param result               Result received by SON
 *  \param rrm_error_code       Error Code received from RRM
 *  \param msg_id               API Id of message received from RRM
 */
son_error_et
anr_map_rrm_error_codes
(
    const son_8 *son_func_name,
    son_return_et result,
    rrm_error_et rrm_error_code,
    rrm_son_message_resp_et msg_id
);

/*! \fn son_void_t determine_the_suspecion_deletion_of_nr( anr_intra_rat_hash_nr_tuple_t *p_eutran_nbr,
 *                                           anr_cell_context_t *p_cell_ctxt);
 *  \brief Determine the Suspecision or the deletion of the neighbor
 *  \param p_eutran_nbr         Pointer to hash record
 *  \param p_cell_ctxt          Pointer to ANR cell context
 */
son_void_t
determine_the_suspecion_deletion_of_nr
(
    anr_intra_rat_hash_nr_tuple_t *p_eutran_nbr,
    anr_cell_context_t   *p_cell_ctxt
);

/*SPR-8950-Start*/

/*! \fn son_void_t delete_nr_as_failed_ho_exceeded_threshold( anr_inter_rat_hash_nr_tuple_t *p_utran_nbr,
 *                                           anr_cell_context_t *p_cell_ctxt);
 *  \brief Deletes the neighbor if failed HO exceeds threshold
 *  \param p_utran_nbr         Pointer to hash record
 *  \param p_cell_ctxt          Pointer to ANR cell context
 */
son_void_t
delete_nr_as_failed_ho_exceeded_threshold
(
 anr_inter_rat_hash_nr_tuple_t *p_utran_nbr,
 anr_cell_context_t   *p_cell_ctxt
 );

/*SPR-8950-End*/
/* Spr 17753 Changes Start */
/*! \fn son_void_t anr_check_and_insert_second_degree_nbr_data
 *  \brief This function copy the second degree nrbr from the list 
 */
son_void_t  
anr_check_and_insert_second_degree_nbr_data
(       
 anr_intra_rat_hash_rec_t *p_intra_rat_hash_rec,
 anr_cell_context_t       *p_cell_ctxt
 );
/* Spr 17753 Changes End */

#ifdef LTE_EMBMS_SUPPORTED
son_buf_retain_status_et
anr_mbms_sf_info_ind_handler
(
 STACKAPIHDR *p_cspl_hdr,
 son_u8      *p_msg
);
#endif
#endif /* ifndef _SON_ANR_CELL_FSM_H_ */
