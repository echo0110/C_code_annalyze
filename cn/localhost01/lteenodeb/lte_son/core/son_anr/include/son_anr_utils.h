/*! \file son_anr_utils.h
 *  \brief This file contains function prototype for ANR utility functions
 *  \date  Sep, 2012
 *  \author Komal
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_ANR_UTILS_H_
#define _SON_ANR_UTILS_H_

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

/*! \headerfile son_anr_cell_fsm.h <>
 */
#include <son_anr_cell_fsm.h>

/*! \fn son_void_t anr_construct_oam_updated_ncl(son_neighbor_info_t *p_nr_list_to,
 *               son_void_t *p_data ,son_bool_et   is_inter_rat)
 *  \brief This function is to populate update notification msg to OAM.
 *  \param p_nr_list_to   -  Pointer to NR list
 *  \param p_data         -  Pointer to the neighbor record.
 *  \param is_inter_rat   -  Flag to determine if input record is intra rat or inter rat
 */
son_void_t
anr_construct_oam_updated_ncl
(
    son_neighbor_info_t *p_nr_list_to,
    son_void_t          *p_data,
    son_bool_et is_inter_rat
);

/*! \fn son_void_t anr_validate_construct_rrm_intra_rat_ncl_on_nrt_update(
 *        son_rrm_intra_rat_neighbor_cell_list_t    *p_intra_rat_ncl,
 *        anr_intra_rat_hash_rec_t                  *p_old_nr,
 *        anr_intra_rat_hash_rec_t                  *p_new_nr);
 *  \brief This function processes the Intra-RAT Neighbors for the
 *         notification to the RRM when there is an update in the
 *         parameter values of the existing neigbor.
 *  \param p_intra_rat_ncl     Pointer to NR list
 *  \param p_old_nr            Pointer to old nr record.
 *  \param p_new_nr            Pointer to new nr record.
 */
son_void_t
anr_validate_construct_rrm_intra_rat_ncl_on_nrt_update
(
    son_rrm_intra_rat_neighbor_cell_list_t    *p_intra_rat_ncl,
    anr_intra_rat_hash_rec_t                  *p_old_nr,
    anr_intra_rat_hash_rec_t                  *p_new_nr
);

/*! \fn son_void_t anr_construct_rrm_intra_rat_ncl(
 *       son_rrm_intra_rat_neighbor_cell_list_t  *p_intra_rat_ncl,
 *       anr_intra_rat_hash_rec_t                *p_intra_rat_hash_rec,
 *       son_bool_et                             is_del_nr_performed )
 *  \brief This function constructs  EUTRAN neighbor list from the given
 *         EUTRAN neighbor on the basis of the handover status.
 *  \param p_intra_rat_ncl        - Pointer to intra rat neighbor cell list
 *  \param p_intra_rat_hash_rec   - Pointer to intra rat hash record
 *  \param is_del_nr_performed    - boolean value to determine if function is
 *                                  invoked by del nr opertion or not
 */
son_void_t
anr_construct_rrm_intra_rat_ncl
(
    son_rrm_intra_rat_neighbor_cell_list_t  *p_intra_rat_ncl,
    anr_intra_rat_hash_rec_t                *p_intra_rat_hash_rec,
    son_bool_et is_del_nr_performed
);

/*! \fn son_void_t anr_send_updated_ncl_to_rrm(
 *      son_anr_rrm_updated_nrt_info_t  *p_notification_msg,
 *      son_u32 cell_id)
 *  \brief Function to send notification message to RRM
 *  \param p_notification_msg       Notification msg
 *  \param cell_id                  Cell Identity
 */
son_void_t
anr_send_updated_ncl_to_rrm
(
    son_anr_rrm_updated_nrt_info_t  *p_notification_msg,
    son_u32 cell_id
);

/*! \fn construct_rrm_notificaton_for_geran_nbr(son_geran_neighbor_info_t *p_new_geran_nbr_info, son_geran_neighbor_info_t
 * p_old_geran_nbr_info, son_rrm_geran_neighbor_cell_list_t *p_geran_nbr_cell_list, son_anr_index_et evnt_id);
 *  \brief Function to constructs RRM notification msg for inter rat neighbors
 *  \param p_new_geran_nbr_info  - Pointer to new geran nr to be inserted/modified
 *  \param p_old_geran_nbr_info  - Pointer to old geran nr
 *  \param p_geran_nbr_cell_list - Pointer to geran neighbor cell list information
 *  \param evnt_id               - Event id
 */
son_void_t
construct_rrm_notificaton_for_geran_nbr
(
    son_geran_neighbor_info_t          *p_new_geran_nbr_info,
    son_geran_neighbor_info_t          *p_old_geran_nbr_info,
    son_rrm_geran_neighbor_cell_list_t *p_geran_nbr_cell_list,
    son_anr_index_et evnt_id
);

/*! \fn construct_rrm_notificaton_for_utran_nbr(son_utran_neighbor_info_t *p_new_utran_nbr_info, son_utran_neighbor_info_t
 * p_old_utran_nbr_info, son_rrm_utran_neighbor_cell_list_t *p_utran_nbr_cell_list, son_anr_index_et evnt_id);
 *  \brief Function to constructs RRM notification msg for inter rat neighbors
 *  \param p_new_utran_nbr_info  - Pointer to new utran nrt to me modified/inserted
 *  \param p_old_utran_nbr_info  - Pointer to old utran nr
 *  \param p_utran_nbr_cell_list - Pointer to utran cell list information
 *  \param evnt_id               - Event id
 */
son_void_t
construct_rrm_notificaton_for_utran_nbr
(
    son_utran_neighbor_info_t          *p_new_utran_nbr_info,
    son_utran_neighbor_info_t          *p_old_utran_nbr_info,
    son_rrm_utran_neighbor_cell_list_t *p_utran_nbr_cell_list,
    son_anr_index_et evnt_id
);

/*! \fn son_bool_et is_utran_nbrs_available_in_rrm_notification(
 *      son_rrm_utran_neighbor_cell_list_t *p_rrm_utran_list);
 *  \brief Function to check if UTRAN neighbors are available in the
 *         notification message to RRM.
 *  \param p_rrm_utran_list - Pointer to utran neighbor list available
 *                            in the notification message to RRM.
 */
son_bool_et
is_utran_nbrs_available_in_rrm_notification
(
    son_rrm_utran_neighbor_cell_list_t  *p_rrm_utran_list
);

/*! \fn son_bool_et  is_geran_nbrs_available_in_rrm_notification(
 *      son_rrm_geran_neighbor_cell_list_t  *p_rrm_geran_list);
 *  \brief Function to check if GERAN neighbors available in notification
 *          message to RRM.
 *  \param p_rrm_geran_list - Pointer to geran neighbour list
 *                            available in the notification message to RRM
 */
son_bool_et
is_geran_nbrs_available_in_rrm_notification
(
    son_rrm_geran_neighbor_cell_list_t  *p_rrm_geran_list
);

/*! \fn son_ho_status_et  anr_get_ho_status_of_utran_nr (son_void_t *p_inter_nr);
 *  \brief This function returns Handover Status of UTRAN neighbors
 *  \param p_inter_nr - Pointer to the utran neighbor record structure
 */
son_ho_status_et
anr_get_ho_status_of_utran_nr
(
    son_void_t *p_inter_nr
);

/*! \fn son_ho_status_et  anr_get_ho_status_of_geran_nr(son_void_t *p_inter_nr);
 *  \brief This function returns Handover status of the GERAN neighbors.
 *  \param p_inter_nr - Pointer to GERAN neighbor record structure
 */
son_ho_status_et
anr_get_ho_status_of_geran_nr
(
    son_void_t *p_inter_nr
);

/*! \fn son_void_t  anr_construct_rrm_inter_rat_ncl_for_utran_nbr(
 *              son_void_t *p_utran_list, son_void_t *p_utran_nbr_info,
 *              son_bool_et is_del_nr_performed);
 *  \brief Function to constructs nrt notification msg for inter rat utran nr
 *  \param p_utran_list         - Pointer to inter rat utran nbr list
 *  \param p_utran_nbr_info     - Pointer to utran neighbor to be inserted
 *  \param is_del_nr_performed  - boolean value to determine if function is
 *                                invoked by del nr opertion or not.
 */
son_void_t
anr_construct_rrm_inter_rat_ncl_for_utran_nbr
(
    son_void_t                        *p_utran_list,
    son_void_t                        *p_utran_nbr_info,
    son_bool_et is_del_nr_performed
);

/*! \fn anr_construct_rrm_inter_rat_ncl_for_geran_nbr (son_void_t*p_geran_list, son_void_t *p_geran_nbr_info,
 *   son_bool_et is_del_nr_performed);
 *  \brief Function to constructs nrt notification msg for inter rat geran nr
 *  \param p_geran_list         - Pointer to geran nr
 *  \param p_geran_nbr_info     - Pointer to geran nr info
 *  \param is_del_nr_performed  - FLag to check if deletion of nr
 */
son_void_t
anr_construct_rrm_inter_rat_ncl_for_geran_nbr
(
    son_void_t            *p_geran_list,
    son_void_t            *p_geran_nbr_info,
    son_bool_et is_del_nr_performed
);

/*! \fn anr_send_updated_ncl_to_oam(son_anr_updated_nrt_info_t *p_notification_msg);
 *  \brief Function to send notification message to OAM
 *  \param p_notification_msg - Ponter containing notification message
 */
son_void_t
anr_send_updated_ncl_to_oam
(
    son_anr_updated_nrt_info_t  *p_notification_msg
);

/*! \fn anr_send_updated_ncl_to_mlb(son_anr_updated_nrt_info_t *p_notification_msg);
 *  \brief Function to send notification message to MLB
 *  \param p_notification_msg - Ponter containing notification message
 */
son_void_t
anr_send_updated_ncl_to_mlb
(
    son_anr_updated_nrt_info_t  *p_notification_msg
);

son_void_t
construct_rrm_notificaton_for_cdma_xrtt_nbr
(
    son_cdma_neighbor_info_t          *p_new_cdma_nbr_info,
    son_cdma_neighbor_info_t          *p_old_cdma_nbr_info,
    son_rrm_cdma_neighbor_cell_list_xrtt_t *cdma_nbr_cell_list,
    son_anr_index_et evnt_id
);

/*! \fn son_bool_et is_cdma_xrtt_nbrs_available_in_rrm_notification(
 *         son_rrm_cdma_neighbor_cell_list_xrtt_t  *p_rrm_cdma_list);
 *  \brief Function to check if cdma XRTT nbr are available in notification
 *         message to RRM.
 *  \param p_rrm_cdma_list  - Pointer to  cdma XRTT neighbour list of
 *                            notification message to RRM.
 */
son_bool_et
is_cdma_xrtt_nbrs_available_in_rrm_notification
(
    son_rrm_cdma_neighbor_cell_list_xrtt_t  *p_rrm_cdma_list
);

/*! \fn son_bool_et is_cdma_hrpd_nbrs_available_in_rrm_notification(
 *         son_rrm_cdma_neighbor_cell_list_hrpd_t  *p_rrm_cdma_list);
 *  \brief Function to check if cdma HRPD nbr are available in notification
 *         message to RRM.
 *  \param p_rrm_cdma_list  - Pointer to  cdma HRPD neighbour list of
 *                            notification message to RRM.
 */
son_bool_et
is_cdma_hrpd_nbrs_available_in_rrm_notification
(
    son_rrm_cdma_neighbor_cell_list_hrpd_t  *p_rrm_cdma_list
);

/*! \fn son_ho_status_et anr_get_ho_status_of_cdma_nr(
 *         son_void_t *p_inter_nr);
 *  \brief This function returns HO Status of CDMA neighbors.
 *  \param p_inter_nr  - Pointer to CDMA neighbor record structure.
 */
son_ho_status_et
anr_get_ho_status_of_cdma_nr
(
    son_void_t *p_inter_nr
);

/*! \fn  son_void_t  anr_set_the_ul_earfcn_dl_earfcn_of_eutran_neighbor(
 *         anr_intra_rat_hash_rec_t *p_nr_mdfd);
 *  \brief This function determines sets EARFCNs on the basis of
            Duplex mode available in the PRACH configuration.
 *  \param p_nr_mdfd - Pointer to the Intra rat hash record
 */
son_void_t
anr_set_the_ul_earfcn_dl_earfcn_of_eutran_neighbor
(
 anr_intra_rat_hash_rec_t *p_nr_mdfd
);

/*SPR 19030 changes start*/
son_bool_et
son_anr_get_serving_enb_id
(
 son_intra_rat_global_cell_id_t      cell_id,
 son_global_enb_id_t                 *src_enbid
 );
/*SPR 19030 changes end*/

/* Spr 17753 Changes Start */
/*! \fn  son_u64  son_ecgi_to_u64(
 *         son_intra_rat_global_cell_id_t *p_ecgi);
 *  \brief This function convert ecgi to U64  
 *  \param p_ecgi- Pointer to the Intra rat ecgi
 */
son_u64 son_ecgi_to_u64
(   
 son_intra_rat_global_cell_id_t *p_ecgi
 );
/*! \fn  son_bool_et son_check_is_serving_cgi_greater_then_nbr_cgi(
 *         son_intra_rat_global_cell_id_t *p_seving_ecgi,
 *         son_intra_rat_global_cell_id_t *p_nbr_ecgi);
 *  \brief This function check if serving ecgi is greater then nbr or not  
 *  \param p_seving_ecgi- Pointer to the Intra rat ecgi
 *  \param p_nbr_ecgi - Pointer to the Intra rat ecgi
 */
son_bool_et 
son_check_is_serving_cgi_greater_then_nbr_cgi
(
 son_intra_rat_global_cell_id_t *p_seving_ecgi,
 son_intra_rat_global_cell_id_t *p_nbr_ecgi
 );
/*! \fn  son_timer_duration_t son_anr_get_random_timer_val(
 *                              son_timer_duration_t time_val,
 *                              son_u32 randomizer);
 *  \brief This function return the random value 
 *         for conflict backoff timer  
 *  \param time_val - timer value configured through OAM 
 *  \param randomizer - used to randomize the timer value
 */
son_timer_duration_t
son_anr_get_random_timer_val
(
 son_timer_duration_t time_val,
/* SPR 23260 + */
#ifndef __x86_64__
son_u32 randomizer
#else
son_u64 randomizer
#endif
/* SPR 23260 - */ 
);
/* Spr 17753 Changes End */
#endif /* ifndef _SON_ANR_UTILS_H_ */
