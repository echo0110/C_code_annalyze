/*! \file son_anr_table.h
 *  \brief This file contains the function prototypes for Neighbor Relation Hash Table 
 *  \date  Jan, 2012
 *  \author --------
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved} 
 */

#ifndef _SON_ANR_TABLE_H_
#define _SON_ANR_TABLE_H_

/*! \headerfile son_hash.h <>
 */
#include <son_hash.h>

/*! \headerfile son_anr_intf.h <>
 */
#include <son_anr_intf.h>

/*! \headerfile son_anr_ctxt_mgr.h <>
 */
#include <son_anr_ctxt_mgr.h>

/*! \def ANR_MAX_INITIAL_NR_TBL_BKT
 *  \brief A macro that stores number of buckets of the hash table
 */
#define ANR_MAX_INITIAL_NR_TBL_BKT 50

/*! \def ANR_INIT_HASH_CTXT_POOL
 *  \brief A macro that stores initial size context and table pool
 */
#define ANR_INIT_HASH_CTXT_POOL   100

/*! \def ANR_MAX_HASH_CTXT_POOL
 *  \brief A macro that stores maximum size context and table pool
 */
#define ANR_MAX_HASH_CTXT_POOL    SON_MAX_NO_NEIGHBOUR_CELLS * SON_MAX_CELL_SUPPORTED_AT_ENB

/*! \def ANR_GET_TABLE
 *  \brief A macro that allocates and returns memory for a table in table pool
 */
#define ANR_GET_TABLE(pool, ptr_m)                              \
    do {                                                        \
        if (!(ptr_m = cvContextAlloc(pool))) {                      \
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,    \
                SON_ERROR,                                      \
                "Hash Table could not be created");         \
        return SON_FAILURE;                                     \
        }                                                       \
    } while (0)

/* Coverity CID-48275, 48276, 48279, 48280 and 48281 Fix Starts */
/*! \def ANR_FREE_TABLE
 *  \brief A macro that deallocates and returns memory for a table in table pool
 */
#define ANR_FREE_TABLE(ptr_m)                               \
    do {                                                    \
        cvContextFree(ptr_m);                               \
    } while (0)
/* Coverity CID-48275, 48276, 48279, 48280 and 48281 Fix Ends */

/*! \def ANR_GET_HASH_REC
 *  \brief A macro that allocates and returns memory for a table record in context pool
 */
#define ANR_GET_HASH_REC(pool, ptr_m)                           \
    do {                                                        \
        if (!(ptr_m = cvContextAlloc(pool))) {                      \
        /* SPR_19279_START/END Code Deleted  */                              \
        }                                                       \
    } while (0)

/*! \def ANR_FREE_HASH_REC
 *  \brief A macro that frees memory for a table record in context pool
 */
#define ANR_FREE_HASH_REC(pool, hash, ptr_m, anchor)                          \
    do {                                                        \
            hash_delete(hash, anchor);                            \
            cvContextFree(ptr_m);                               \
                                                                \
    } while (0)

/*! \def  enum anr_enbs_et
 *  \brief  Enum for determining  eNB.This is an internal enum used for code.
 */
typedef enum
{
    MACRO_ENB_TYPE_A,
    MACRO_ENB_TYPE_B,
    FEMTO_ENB,
    INVALID_ENB_TYPE
}anr_enbs_et;

/*! \typedef anr_nr_nbr_list_data_t
 *  \brief ANR Cell specific NR Nbr data record
 *  \param son_intra_rat_global_cell_id_t  nr_nbr_cgi;
 *  \param nr_nbr_cgi                   son_intra_rat_global_cell_id_t type variable having neighbour cgi
 *  \param pci                          PCI of the neighbors neighbors
 *  \param earfcn                       EARFCN of the neighbors neighbors
 */
typedef struct _anr_nr_nbr_data_t
{
    son_intra_rat_global_cell_id_t  nr_nbr_cgi;
    son_u16 pci;
    son_u32 earfcn;
}anr_nr_nbr_data_t;

/*! \typedef anr_nr_nbr_list_node_t
 *  \brief ANR Cell specific NR Nbr  list Node
 *  \param nr_nbr_anchor                CSPL's Node
 *  \param nr_nbr_data                  NR Nbr record
 */

typedef struct _anr_nr_nbr_list_node_t
{
    SON_LIST_NODE nr_nbr_anchor;
    anr_nr_nbr_data_t nr_nbr_data;
}anr_nr_nbr_list_node_t;

/*! \  struct son_mro_ho_counter_t
 *  \brief  Structure storing the counters mainted for Mobility Robustness Optimization (MRO)
 *  \param num_too_early_ho                                 Number of too early HOs
 *  \param num_too_late_ho                                  Number of too later HOs
 *  \param num_ho_to_wrong_cell_for_failure_cell            Number of HOs performed to wrong cell for failure cell
 *  \param num_ho_to_wrong_cell_for_reestablishment_cell    Number of HOs performed to wrong cell for re-establishment cell
 *  \param num_ho_to_wrong_cell_for_reestablishment_cell    Total number of HOs attempted
 */
typedef struct _son_mro_ho_counter_t
{
    son_u16                         num_too_early_ho;
    son_u16                         num_too_late_ho;
    son_u16                         num_ho_to_wrong_cell_for_failure_cell;
    son_u16                         num_ho_to_wrong_cell_for_reestablishment_cell;
    son_u16                         total_ho_attempted;
}son_mro_ho_counter_t;

typedef enum
{
    SON_TOO_EARLY,
    SON_TOO_LATE,
    /* PR fix 929 Start */
    SON_NO_OPTIMIZATION_DIR
    /* PR fix 929 End */
}mro_optimization_dir_et;

/*! \  struct son_mro_data_t
 *  \brief
 *  \param  is_cio_algo_running
 *  \param  retry_count
 *  \param  prev_cio
 *  \param  prev_ho_fail_rate
 *  \param  optimizaton_for
 *  \param expected_ho_improvement_for_single_step  Expected improvement in HO Failure rate after tuning CIO/TTT by Delta CIO/Delta TTT
 *  \param
 */
typedef struct _son_mro_data_t
{
    son_bool_et is_cio_algo_running;
    son_u8 retry_count;
    son_q_offset_range_et prev_cio;
    son_float prev_ho_fail_rate;
    mro_optimization_dir_et optimizaton_for;
    son_bool_et in_use_by_mlb;
    son_mro_ho_counter_t mro_ho_counter;
}son_mro_data_t;

/*! \  struct anr_intra_rat_hash_rec_t
 *  \brief  It is the structure storing INTRA RAT hash table record node data
 *  \param neighbor_info            Neighbor information
 *  \param num_intra_freq_neighbor  Size of neighbor's neighbor list 
 *  \param intra_freq_neighbor_list List storing neighbor's neighbor info
 *  \param no_nr_neighbors_ts 		No nr Timestamp
 *  \param last_activity_ts			 Timestamp of the last activity with neighbor
 *                                                                      (reported in UE measurements, or HO (S1/X2))
 *  \param ho_failed_for_pci_conflict	Number of HOs failed due to pci conflicts
 *  \param ho_failed_for_nr_removal     Number of HOs failed due to nr removal
 *  \param nr_nbrs_list                   Neighbour List
 *  \param mro_ho_counter           Structures of HO conunters maintained for Mobility Robustness Optimization (MRO)
 *  \param nbrs_list                List containing the neighbors of the cell available via X2 link
 */
typedef struct _anr_intra_rat_hash_rec_t
{
    son_intra_rat_neighbor_info_t   neighbor_info;
    son_u8                          num_intra_freq_neighbor;
    intra_freq_neighbor_info_t      intra_freq_neighbor_list[SON_MAX_INTRA_FREQ_NEIGHBORS_IN_SIB4];
    time_stamp_t                 	no_nr_neighbors_ts;
    time_stamp_t                 	last_activity_ts;
    son_u8							ho_failed_for_pci_conflict;
    son_u8							ho_failed_for_nr_removal;
    son_u8                          received_cgi_counter;
    son_mro_data_t mro_data;
    SON_LIST anr_nr_nbr_list;
    /* SPR 10103 FIX START  */
    son_global_enb_id_t enb_id;
    /* SPR 10103 FIX END */
}anr_intra_rat_hash_rec_t;

/*! \  struct anr_intra_rat_hash_nr_tuple_t
 *  \brief  It is the structure of NR record node 
 *  \param anchor	anchor field of the node
 *  \param data	data filed to contain neighbor information
 */
typedef struct _anr_intra_rat_hash_nr_tuple_t
{
    SON_HASH_NODE     			anchor;
    anr_intra_rat_hash_rec_t    data;
}anr_intra_rat_hash_nr_tuple_t;

/* Spr 17753 Changes Start */
/*! \  struct anr_second_degree_cell_info_t
 *  \brief  It is the structure of second degree cell info 
 *  \param first_degree_nbr_cgi   - first degree nbr cgi
 *  \param anr_nr_nbr_list        - second degree nbr list
 */
typedef struct _anr_second_degree_cell_info_t
{
    son_intra_rat_global_cell_id_t   first_degree_nbr_cgi;
    /* List to hold second degree nbr */
    SON_LIST anr_nr_nbr_list;
}anr_second_degree_cell_info_t;

/*! \typedef anr_second_degree_nr_list_node_t
 *  \brief ANR Second Degree NR Nbr list Node
 *  \param nr_nbr_anchor          CSPL's Node
 *  \param nr_nbr_data            NR Nbr record
 *  \param ts                     time stamp of
 *                                node entered in list 
 */
typedef struct _anr_second_degree_nr_list_node_t                
{   
    SON_LIST_NODE nr_nbr_anchor;
    anr_second_degree_cell_info_t nr_nbr_data; 
    time_stamp_t ts;
}anr_second_degree_nr_list_node_t;
/* Spr 17753 Changes End */
/*! \def ANR_UTRAN_NEIGHBOR
 *  \brief A macro used to identify UTRAN neighbor cell
 */
#define ANR_UTRAN_NEIGHBOR 1

/*! \def ANR_GERAN_NEIGHBOR
 *  \brief A macro used to identify GERAN neighbor cell
 */
#define ANR_GERAN_NEIGHBOR 2

 /*! \def ANR_CDMA_NEIGHBOR
  *  \brief A macro used to identify CDMA neighbor cell
  */
#define ANR_CDMA_NEIGHBOR  3

/*! \  struct anr_inter_rat_hash_rec_t
 *  \brief  It is the structure storing INTER RAT hash table record node data
 *  \param t                        Idetifier the RAT type
 *  \param u.utran_neighbor_info    Structure storing UTRAN neighbor data
 *  \param u.geran_neighbor_info    Structure storing GERAN neighbor data
 *  \param no_nr_neighbors_ts 		No nr Timestamp
 *  \param last_activity_ts			 Timestamp of the last activity with neighbor
 *                                                                      (reported in UE measurements, or HO (S1/X2))
 *  \param ho_failed_for_nr_removal     Number of HOs failed due to nr removal
 */
typedef struct _anr_inter_rat_hash_rec_t
{
    son_u8          t;
    union {
    son_utran_neighbor_info_t   utran_neighbor_info;   
    son_geran_neighbor_info_t   geran_neighbor_info;
    son_cdma_neighbor_info_t    cdma_neighbor_info;
    /* other RAT to be added in later release */
    }u;
    time_stamp_t                no_nr_neighbors_ts;
    time_stamp_t                last_activity_ts;
    son_u8						ho_failed_for_nr_removal;
}anr_inter_rat_hash_rec_t;

/*! \  struct anr_inter_rat_hash_nr_tuple_t
 *  \brief  It is the structure of NR record node 
 *  \param anchor		     anchor field of the node
 *  \param data				 data filed to contain neighbor information
 */
typedef struct _anr_inter_rat_hash_nr_tuple_t
{
    SON_HASH_NODE     			anchor;
    anr_inter_rat_hash_rec_t    data;
}anr_inter_rat_hash_nr_tuple_t;

/*! \fn son_s32 anr_intra_rat_cmp_nr_func(const son_void_t *val_1,const son_void_t *val_2);
 *  \brief This is a hash function used for comparing the key values of NR Hash table
 *  \param val_1 - Pointer to the structure of type anr_intra_rat_hash_rec_t
 *  \param val_2 - Pointer to the structure of type anr_intra_rat_hash_rec_t
 */
son_s32
anr_intra_rat_cmp_nr_func
(
    const son_void_t *val_1,
    const son_void_t *val_2
);

/*! \fn son_s32 anr_inter_rat_cmp_nr_func(const son_void_t *val_1,const son_void_t *val_2);
 *  \brief This is a hash function used for comparing the key values of inter RAT NR Hash table
 *  \param val_1 - Pointer to the structure of type anr_inter_rat_hash_rec_t
 *  \param val_2 - Pointer to the structure of type anr_inter_rat_hash_rec_t
 */
son_s32
anr_inter_rat_cmp_nr_func
(
    const son_void_t *val_1,
    const son_void_t *val_2
);

/*! \fn son_return_et anr_init_intra_rat_hash(SON_HASH *H);
 * \brief This function created hash table
 * \param H - Pointer to SON_HASH
 *
 */
son_return_et
anr_init_intra_rat_hash
(
SON_HASH *H
);

/*! \fn son_return_et anr_init_inter_rat_hash(SON_HASH *H);
 * \brief This function created hash table
 * \param H - Pointer to SON_HASH
 *
 */
son_return_et
anr_init_inter_rat_hash
(
SON_HASH *H
);

/*! \fn son_ul32 anr_intra_rat_hash_nr_func( const son_void_t *p_key);
 *  \brief This is a hash function for generating the hash value for the hash table.
 *  \param p_key - Pointer to the structure of type anr_intra_rat_hash_rec_t
 */
son_ul32
anr_intra_rat_hash_nr_func
(
    const son_void_t *p_key
);

/*! \fn son_ul32 anr_inter_rat_hash_nr_func( const son_void_t *p_key);
 *  \brief This is a hash function for generating the hash value for the inter 
 *      RAT hash table.
 *  \param p_key - Pointer to the structure of type anr_inter_rat_hash_rec_t
 */
son_ul32
anr_inter_rat_hash_nr_func
(
    const son_void_t *p_key
);

/*! \fn const son_void_t *anr_intra_rat_key_off_nr_func(const SON_HASH_NODE *p_node);
 *  \brief This is a function used to find the key of a node in hash table
 *  \param p_node -  Pointer to the anchor field of NR record node in NR table
 */
const son_void_t *
anr_intra_rat_key_off_nr_func
(
    const SON_HASH_NODE *p_node
);

/*! \fn const son_void_t *anr_inter_rat_key_off_nr_func(const SON_HASH_NODE *p_node);
 *  \brief This is a function used to find the key of a node in inter RAT hash table
 *  \param p_node -  Pointer to the anchor field of NR record node in NR table
 */
const son_void_t *
anr_inter_rat_key_off_nr_func
(
    const SON_HASH_NODE *p_node
);

/*! \fn anr_intra_rat_hash_rec_t* anr_intra_rat_insert_nr(anr_cell_context_t *p_cell_ctxt,
 *                  anr_intra_rat_hash_rec_t *p_nr,son_bool_et check_remove_list,
 *              son_error_et *p_error_code,son_anr_updated_nrt_info_t *p_oam_notification_add,
 *                   son_anr_updated_nrt_info_t *p_oam_notifiation_update,
 *                          son_anr_rrm_updated_nrt_info_t *p_rrm_notification,,son_bool_et is_conflict_confusion_trigger,
 *                          son_bool_et is_oam_triggered_insertion);
 *  \brief This function is used for inserting NR in the Neighbor Relation Table
 *  \param p_cell_ctxt - Pointer to cell context
 *  \param p_nr   -  Pointer to a structure containing the NR information
 *  \param check_remove_list - Boolean value for remove list
 *  \param p_error_code -  Pointer to a variable for storing error code
 *  \param p_oam_notification_add - Pointer to structure son_anr_updated_nrt_info_t for oam notification
 *  \param p_oam_notifiation_update - Pointer to structure son_anr_updated_nrt_info_t for oam notification 
 *  \param p_rrm_notification - Pointer to structure son_anr_rrm_updated_nrt_info_t for p_rrm_notification
 *  \param is_conflict_confusion_trigger - Boolean value for the conflict confusion handling
 *  \param is_oam_triggered_insertion       - Boolean value to determine if operation is OAM triggered
 */
anr_intra_rat_hash_rec_t *
anr_intra_rat_insert_nr
(
		anr_cell_context_t *p_cell_ctxt,
		anr_intra_rat_hash_rec_t *p_nr,
		son_bool_et check_remove_list,
		son_error_et *p_error_code,
		son_anr_updated_nrt_info_t *p_oam_notification_add,
        son_anr_updated_nrt_info_t *p_oam_notification_upd,
        son_anr_rrm_updated_nrt_info_t *p_rrm_notification,
        son_bool_et trigger_conflict_confusion,
    son_bool_et is_oam_triggered_add
);

/*start:gur32802*/

/*! \fn son_void_t anr_determine_cdma_changes_in_nrt_when_old_nr_not_send(anr_cell_context_t *p_cell_ctxt,
 *                                                              anr_inter_rat_hash_rec_t *p_old_hash,
 *                                                              anr_inter_rat_hash_rec_t *p_new_hash,
 *                                                              son_ho_status_et    old_ho_status,
 *                                                              son_ho_status_et    new_ho_status,
 *                                                              son_bool_et *p_is_old_nr_send,
 *                                                              son_bool_et *p_is_rrm_notif_reqd_for_new_nr,
 *                                                              son_error_et    *p_error_code)
 *  \brief This function is used to verify the new CDMA NBRs status to be notified to RRM when old NBR was not send to RRM
 *  \param p_cell_ctxt - Pointer to cell context
 *  \param p_old_hash  - Pointer to old NBR hash record
 *  \param p_new_hash  - Pointer to new NBR hash record
 *  \param old_ho_status -  HO status for old NBR
 *  \param new_ho_status -  HO status for new NBR
 *  \param p_is_old_nr_send - flag pointer to check if old NBR is sendable to RRM 
 *  \param p_is_rrm_notif_reqd_for_new_nr - Status flag to check if new NBR if to be notified to RRM 
 *  \param p_error_code - Pointer to error code
 */

son_void_t
anr_determine_cdma_changes_in_nrt_when_old_nr_not_send
(
 anr_cell_context_t        *p_cell_ctxt,
 anr_inter_rat_hash_rec_t  *p_old_hash,
 anr_inter_rat_hash_rec_t  *p_new_hash,
/*SPR 17777 +-*/ 
 son_bool_et               *p_is_old_nr_send,
 son_bool_et               *p_is_rrm_notif_reqd_for_new_nr,
 son_error_et              *p_error_code
 );

/*! \fn son_void_t anr_determine_cdma_changes_in_nrt_when_old_nr_send(anr_cell_context_t *p_cell_ctxt,
 *                                                              anr_inter_rat_hash_rec_t *p_old_hash,
 *                                                              anr_inter_rat_hash_rec_t *p_new_hash,
 *                                                              son_ho_status_et    old_ho_status,
 *                                                              son_ho_status_et    new_ho_status,
 *                                                              son_bool_et *p_is_old_nr_send,
 *                                                              son_bool_et *p_is_rrm_notif_reqd_for_new_nr,
 *                                                              son_error_et    *p_error_code)
 *  \brief This function is used to verify the new CDMA NBRs status to be notified to RRM when old NBR was send to RRM
 *  \param p_cell_ctxt - Pointer to cell context
 *  \param p_old_hash  - Pointer to old NBR hash record
 *  \param p_new_hash  - Pointer to new NBR hash record
 *  \param old_ho_status -  HO status for old NBR
 *  \param new_ho_status -  HO status for new NBR
 *  \param p_is_old_nr_send - flag pointer to check if old NBR is sendable to RRM 
 *  \param p_is_rrm_notif_reqd_for_new_nr - Status flag to check if new NBR if to be notified to RRM 
 *  \param p_error_code - Pointer to error code
 */
son_void_t
anr_determine_cdma_changes_in_nrt_when_old_nr_send
(
 anr_cell_context_t        *p_cell_ctxt,
 anr_inter_rat_hash_rec_t  *p_old_hash,
 anr_inter_rat_hash_rec_t  *p_new_hash,
 son_ho_status_et          old_ho_status,
 son_ho_status_et          new_ho_status,
 son_bool_et               *p_is_old_nr_send,
 son_bool_et               *p_is_rrm_notif_reqd_for_new_nr,
 son_error_et              *p_error_code
);

/*! \fn son_bool_et anr_validate_inter_rat_record_insertion(anr_cell_context_t *p_cell_ctxt,
 *                                                          anr_inter_rat_hash_rec_t *p_hash,
 *                                                          son_error_et    *p_error_code)
 *  \brief This function is used to verify if the inter rat NBR is valid for adding in NRT and sendable to rrm
 *  \param p_cell_ctxt - Pointer to cell context
 *  \param p_hash  - Pointer to old NBR hash record
 *  \param p_error_code - Pointer to error code
 */
son_bool_et
anr_validate_inter_rat_record_insertion
(
 anr_cell_context_t        *p_cell_ctxt,
 anr_inter_rat_hash_rec_t  *p_hash,
 son_error_et              *p_error_code
);

/*! \fn son_void_t anr_construct_rrm_inter_rat_ncl_for_cdma_xrtt_nbr(son_void_t *p_cdma_list,
 *                                                                  son_void_t *p_cdma_nbr_info,
 *                                                                  son_bool_et is_del_nr_performed)
 *  \brief This function constructs CDMA xRTT neighbor list from given CDMA xRTT neighbor on the basis of the handover status
 *  \param p_cdma_list         - Pointer to Inter-Rat CDMA XRTT nbr list
 *  \param p_cdma_nbr_info - Pointer to CDMA nbr info
 *  \param is_del_nr_performed - boolean value to determine if function is invoked by del nr opertion or not
 */
son_void_t
anr_construct_rrm_inter_rat_ncl_for_cdma_xrtt_nbr
(
  son_void_t            *p_cdma_list,
  son_void_t            *p_cdma_nbr_info,
  son_bool_et            is_del_nr_performed
);

/*! \fn son_void_t anr_construct_rrm_inter_rat_ncl_for_cdma_hrpd_nbr(son_void_t *p_cdma_list,
 *                                                                  son_void_t *p_cdma_nbr_info,
 *                                                                  son_bool_et is_del_nr_performed)
 *  \brief This function constructs CDMA neighbor list on the basis of the handover status of the given  CDMA HRPD neighbor
 *  \param p_cdma_list         - Pointer to Inter-Rat CDMA XRTT nbr list
 *  \param p_cdma_nbr_info     - Pointer to CDMA nbr info
 *  \param is_del_nr_performed - boolean value to determine if function is invoked by del nr opertion or not
 */
son_void_t
anr_construct_rrm_inter_rat_ncl_for_cdma_hrpd_nbr
(
 son_void_t            *p_cdma_list,
 son_void_t            *p_cdma_nbr_info,
 son_bool_et            is_del_nr_performed
 );

/*! \fn son_bool_et is_cdma_nbr_notifiable_to_rrm( son_cdma_neighbor_info_t *p_nbr_info)
 *  \brief This function is used to verify if the CDMA NBRs are to be notified to RRM
 *  \param p_nbr_info     - Pointer to CDMA nbr info
 */
son_bool_et
is_cdma_nbr_notifiable_to_rrm
(
    son_cdma_neighbor_info_t *p_nbr_info
);

/*end:gur32802*/

/*! \fn anr_inter_rat_hash_rec_t* anr_inter_rat_insert_nr(anr_cell_context_t *p_cell_ctxt,
 *                  anr_inter_rat_hash_rec_t *p_hash_rec,son_bool_et check_remove_list,son_error_et *p_error_code,
 *                  son_anr_updated_nrt_info_t *p_oam_notification,
 *                                      son_anr_rrm_updated_nrt_info_t *p_rrm_notification);
 *  \brief This function is used for inserting NR in the Neighbor Relation Table
 *  \param p_cell_ctxt - Pointer to cell context
 *  \param p_hash_rec   -  Pointer to a structure containing the NR information
 *  \param check_remove_list - Boolean structure for remove list
 *  \param p_error_code -  Pointer to a variable for storing error code
 *  \param p_oam_notification - Pointer to structure son_anr_updated_nrt_info_t for oam notification
 *  \param p_rrm_notification - Pointer to structure son_anr_rrm_updated_nrt_info_t for p_rrm_notification
 */
anr_inter_rat_hash_rec_t *
anr_inter_rat_insert_nr
(
		anr_cell_context_t *p_cell_ctxt,
		anr_inter_rat_hash_rec_t *p_nr,
		son_bool_et check_remove_list,
		son_error_et *p_error_code,
		son_anr_updated_nrt_info_t *p_oam_notification,
    son_anr_rrm_updated_nrt_info_t *p_rrm_notification
);

/*! \fn son_return_et anr_intra_rat_del_nr(
 * anr_cell_context_t *p_cell_ctxt,son_intra_rat_global_cell_id_t  *p_nr_cell_id,
 * son_bool_et check_remove_list,son_error_et *p_error_code,
 * son_void_t *p_hash_rec,son_anr_updated_nrt_info_t *p_oam_notification,
 * son_anr_rrm_updated_nrt_info_t *p_rrm_notification,son_bool_et is_conflict_confusion_trigger);
 *
 *  \brief This function is used for removing selected NR record for a cell configured
 *  \param p_cell_ctxt - Pointer to cell context
 *  \param p_nr_cell_id  - Pointer to the neighbor cell Id
 *  \param check_remove_list - 
 *  \param p_error_code -  Pointer to a variable for storing error code
 *  \param p_hash_rec - Pointer to hash record structure.  
 *  \param p_oam_notification - Pointer to structure son_anr_updated_nrt_info_t for oam notification
 *  \param p_rrm_notification - Pointer to structure son_anr_rrm_updated_nrt_info_t for p_rrm_notification
 *  \param is_conflict_confusion_trigger - Boolean value to trigger the conflict and confusion
 */
son_return_et
anr_intra_rat_del_nr
(
	anr_cell_context_t *p_cell_ctxt,
    son_intra_rat_global_cell_id_t  *p_nr_cell_id,
    son_bool_et check_remove_list,
	son_error_et *p_error_code,
    son_void_t                *p_hash_rec,
    son_anr_updated_nrt_info_t *p_oam_notification,
    son_anr_rrm_updated_nrt_info_t *p_rrm_notification,
    son_bool_et   trigger_conflict_confusion
);

/*! \fn son_return_et anr_inter_rat_del_nr(anr_cell_context_t *p_cell_ctxt,
 *      son_inter_rat_global_cell_id_t *p_nr_cell_id,son_bool_et check_remove_list,
 *       son_error_et *p_error_code, son_void_t *p_hash_rec,
 *    son_anr_updated_nrt_info_t *p_oam_notification,son_anr_rrm_updated_nrt_info_t *p_rrm_notification);
 *   \brief This function is used for removing selected NR record for a cell configured from inter RAT NRT
 *  \param p_cell_ctxt - Pointer to cell context
 *  \param p_nr_cell_id  - Pointer to the neighbor cell Id
 *  \param check_remove_list - Flag to check remove list
 *  \param p_error_code -  Pointer to a variable for storing error code
 *  \param p_hash_rec - Pointer to hash record structure.  
 *  \param p_oam_notification - Pointer to structure son_anr_updated_nrt_info_t for oam notification
 *  \param p_rrm_notification - Pointer to structure son_anr_rrm_updated_nrt_info_t for p_rrm_notification
 */
son_return_et
anr_inter_rat_del_nr
(
        anr_cell_context_t *p_cell_ctxt,
        son_inter_rat_global_cell_id_t  *p_nr_cell_id,
        son_bool_et check_remove_list,
        son_error_et *p_error_code,
        son_void_t                *p_hash_rec,
        son_anr_updated_nrt_info_t *p_oam_notification,
        son_anr_rrm_updated_nrt_info_t *p_rrm_notification
);

/*! \fn son_return_et anr_del_nr(anr_cell_context_t  *p_cell_ctxt,
 *      son_global_cell_id_t    *p_cell_id,son_bool_et check_remove_list, son_error_et *p_error_code,
 *      son_void_t* p_hash_rec, son_anr_updated_nrt_info_t *p_oam_notification,
 *       son_anr_rrm_updated_nrt_info_t *p_rrm_notification);
 *   \brief This function is used for removing selected NR record for a cell configured.
 *  \param p_cell_ctxt  -  Pointer to the context for a cell configured.
 *  \param p_cell_id    - Pointer to the neighbor cell Id
 *  \param check_remove_list - Flag to check remove list
 *  \param p_error_code -  Pointer to a variable for storing error code
 *  \param p_hash_rec - Pointer to hash record structure. 
 *  \param p_oam_notification - Pointer to structure son_anr_updated_nrt_info_t for oam notification
 *  \param p_rrm_notification - Pointer to structure son_anr_rrm_updated_nrt_info_t for p_rrm_notification
 */
son_return_et 
anr_del_nr
(
    anr_cell_context_t        *p_cell_ctxt,
    son_global_cell_id_t      *p_cell_id,
    son_bool_et check_remove_list,
    son_error_et              *p_error_code,
    son_void_t                *p_hash_rec,
    son_anr_updated_nrt_info_t *p_oam_notification,
    son_anr_rrm_updated_nrt_info_t *p_rrm_notification
);

/*! \fn anr_intra_rat_hash_rec_t* anr_intra_rat_modify_nr(anr_cell_context_t *p_cell_ctxt,
 *      anr_intra_rat_hash_rec_t *p_nr_mdfd,son_bool_et check_remove_list,son_error_et *p_error_code,
 *                  son_anr_updated_nrt_info_t *p_oam_notification_add,
 *                  son_anr_updated_nrt_info_t *p_oam_notification_upd,
 *              son_anr_rrm_updated_nrt_info_t *p_rrm_notification,son_bool_et is_conflict_confusion_trigger,
 *              son_bool_et is_oam_triggered_modification);
 *  \brief This function is used for modifying selected NR record for a cell configured
 *  \param p_cell_ctxt  -  Pointer to the context for a cell configured.
 *  \param p_nr_mdfd         -  Pointer to anr cell context
 *  \param check_remove_list - Flag to check remove list
 *  \param p_error_code -  Pointer to a variable for storing error code
 *  \param p_oam_notification_add - Pointer to structure son_anr_updated_nrt_info_t for oam notification
 *  \param p_oam_notification_upd - Pointer to structure son_anr_rrm_updated_nrt_info_t for p_rrm_notification
 *  \param p_rrm_notification - Pointer to structure son_anr_updated_nrt_info_t for oam notification 
 *  \param is_conflict_confusion_trigger - Flag to trigger the conflict confusion handler
 *  \param is_oam_triggered_modification - Boolean to determine if operation is OAM triggered
 */
anr_intra_rat_hash_rec_t *
anr_intra_rat_modify_nr
(
		anr_cell_context_t *p_cell_ctxt,
		anr_intra_rat_hash_rec_t *p_nr,
		son_bool_et check_remove_list,
		son_error_et *p_error_code,
		son_anr_updated_nrt_info_t *p_oam_notification_add,
        son_anr_updated_nrt_info_t *p_oam_notification_upd,
		son_anr_rrm_updated_nrt_info_t *p_rrm_notification,
        son_bool_et trigger_conflict_confusion,
    son_bool_et is_oam_triggered_modification
);

/*! \fn anr_inter_rat_hash_rec_t* anr_inter_rat_modify_nr(anr_cell_context_t *p_cell_ctxt,
 *      anr_inter_rat_hash_rec_t *p_nr_mdfd,son_bool_et check_remove_list,son_error_et *p_error_code,
 *      son_anr_updated_nrt_info_t *p_oam_notification,son_anr_rrm_updated_nrt_info_t *p_rrm_notification );
 *  \brief This function is used for modifying selected NR record for a cell configured
 *  \param p_cell_ctxt  -  Pointer to the context for a cell configured.
 *  \param p_nr_mdfd         -  Pointer to anr cell context
 *  \param check_remove_list - Flag to check remove list
 *  \param p_error_code -  Pointer to a variable for storing error code
 *  \param p_oam_notification - Pointer to structure son_anr_updated_nrt_info_t for oam notification
 *  \param p_rrm_notification - Pointer to structure son_anr_rrm_updated_nrt_info_t for p_rrm_notification
 */
anr_inter_rat_hash_rec_t *
anr_inter_rat_modify_nr
(
		anr_cell_context_t *p_cell_ctxt,
		anr_inter_rat_hash_rec_t *p_nr,
		son_bool_et check_remove_list,
		son_error_et *p_error_code,
		son_anr_updated_nrt_info_t *p_oam_notification,
    son_anr_rrm_updated_nrt_info_t *p_rrm_notification
);

/*! \fn  son_void_t * anr_find_nr( anr_cell_context_t      *p_cell_ctxt,
 * son_global_cell_id_t *p_tgt_cell_id, son_error_et *p_error_code,
 * son_rat_type_et *p_rat_type)
 *  \brief Function to return the NR record form inter RAT NR hash table
 *  \param p_cell_ctxt    Pointer to the context for a cell configured.
 *  \param p_tgt_cell_id  Pointer to the target cell_id
 *  \param p_error_code   Pointer to a variable for storing error code
 *  \param p_rat_type     Pointer to a variable for storing the rat type
 */
son_void_t *
anr_find_nr
(
        anr_cell_context_t      *p_cell_ctxt,
        son_global_cell_id_t    *p_tgt_cell_id,
        son_error_et            *p_error_code,
    son_rat_type_et         *p_rat_type
);

/*! \fn son_void_t anr_convert_inter_rat_nr_to_inter_rat_hash_rec (const son_inter_rat_neighbor_info_t *p_nr,
 *      anr_inter_rat_hash_rec_t *p_hash_rec);
 *  \brief This function populate inter rat hash record
 *  \param p_nr        Pointer to a structure containing the NR
 *  \param p_hash_rec  Pointer to the data field of the new NR record to be inserted.
 */

son_void_t
anr_convert_inter_rat_nr_to_inter_rat_hash_rec
(
const son_inter_rat_neighbor_info_t *p_nr,
anr_inter_rat_hash_rec_t *p_hash_rec
);

/*! \fn anr_intra_rat_hash_nr_tuple_t* anr_intra_rat_find_nr( SON_HASH *p_hash,
 *      son_intra_rat_global_cell_id_t  *p_nr_cell_id,son_error_et *p_error_code);
 *  \brief This function returns the NR record of a specified neighbor
 *  \param p_hash -   Pointer to the hash context for a cell configured
 *  \param p_nr_cell_id   - Pointer to the cell id of the neighbor 
 *  \param p_error_code -  Pointer to a variable for storing error code
 */
anr_intra_rat_hash_nr_tuple_t *
anr_intra_rat_find_nr
(
    SON_HASH *p_hash,
    son_intra_rat_global_cell_id_t  *p_nr_cell_id,
    son_error_et *p_error_code
);

/*! \fn anr_inter_rat_hash_nr_tuple_t* anr_inter_rat_find_nr( SON_HASH *p_hash,
 *      son_inter_rat_global_cell_id_t  *p_nr_cell_id, son_error_et *p_error_code,son_rat_type_et *p_rat_type);
 *  \brief This function returns the NR record of a specified neighbor from inter RAT NR hash table
 *  \param p_hash -   Pointer to the hash context for a cell configured
 *  \param p_nr_cell_id   - Pointer to the cell id of the neighbor 
 *  \param p_error_code -  Pointer to a variable for storing error code
 *  \param p_rat_type     Pointer to a variable for storing the rat type
 */
anr_inter_rat_hash_nr_tuple_t *
anr_inter_rat_find_nr
(
    SON_HASH *p_hash,
    son_inter_rat_global_cell_id_t  *p_nr_cell_id,
    son_error_et *p_error_code,
    son_rat_type_et *p_rat_type
);

/*! \fn anr_intra_rat_hash_nr_tuple_t *anr_get_next_nr_from_intra_rat_hash_table (SON_HASH *p_hash, 
 *                                                  SON_HASH_NODE *p_node)
 *  \brief Function to return the next NR record form hash table
 *  \param p_hash   Pointer to the hash context for a cell configured.
 *  \param p_node   Pointer to the current hash node
 */
anr_intra_rat_hash_nr_tuple_t *
anr_get_next_nr_from_intra_rat_hash_table
(
    SON_HASH        *p_hash,
    SON_HASH_NODE   *p_node
);

/*! \fn anr_inter_rat_hash_nr_tuple_t *anr_get_next_nr_from_inter_rat_hash_table (SON_HASH *p_hash, 
 *                                                 SON_HASH_NODE *p_node)
 *  \brief Function to return the next NR record form inter RAT NR hash table
 *  \param p_hash   Pointer to the hash context for a cell configured.
 *  \param p_node   Pointer to the current hash node
 */
anr_inter_rat_hash_nr_tuple_t *
anr_get_next_nr_from_inter_rat_hash_table
(
    SON_HASH        *p_hash,
    SON_HASH_NODE   *p_node
);

/*! \fn anr_intra_rat_hash_nr_tuple_t* anr_get_first_nr_from_intra_rat_hash_table (SON_HASH *p_hash )
 *  \brief Function to return the first NR record form hash table
 *  \param p_hash -   Pointer to the hash context for a cell configured.
 */
anr_intra_rat_hash_nr_tuple_t *
anr_get_first_nr_from_intra_rat_hash_table
(
SON_HASH *p_hash
);

/*! \fn anr_inter_rat_hash_nr_tuple_t* anr_get_first_nr_from_inter_rat_hash_table (SON_HASH *p_hash )
 *  \brief Function to return the first NR record form inter RAT NR hash table
 *  \param p_hash -   Pointer to the hash context for a cell configured.
 */
anr_inter_rat_hash_nr_tuple_t *
anr_get_first_nr_from_inter_rat_hash_table
(
SON_HASH *p_hash
);

/*! \fn son_void_t   anr_clear_nr_intra_rat_hash_table( SON_HASH *p_hash);
 *  \brief This function clears entire hash context along with the table for a configured cell
 *  \param p_hash -   Pointer to the hash context for a cell configured.
 */
son_void_t
anr_clear_nr_intra_rat_hash_table
(
    SON_HASH *p_hash
);

/*! \fn son_void_t   anr_clear_nr_inter_rat_hash_table( SON_HASH *p_hash);
 *  \brief This function clears entire inter RAT hash context along with the table for a configured cell
 *  \param p_hash -   Pointer to the hash context for a cell configured.
 */
son_void_t
anr_clear_nr_inter_rat_hash_table
(
    SON_HASH *p_hash
);

/*! \fn son_return_et anr_init_db(); 
 *  \brief This function initialize db for anr
 */
son_return_et
anr_init_db
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t anr_clear_db();
 *  \brief This function clears db for anr
 */  
son_void_t
anr_clear_db
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_u32 anr_get_cell_identity_from_inter_nr_rec(const anr_inter_rat_hash_rec_t  *p_hash_rec)
 *  \brief Function to get the cell Identity value from given 
 *      inter RAT hash table record
 *  \param p_hash_rec Pointer to the inter RAT hash table record
 */
son_u32
anr_get_cell_identity_from_inter_nr_rec
(
    const anr_inter_rat_hash_rec_t  *p_hash_rec
);

/*! \fn son_void_t anr_convert_inter_rat_hash_rec_to_inter_rat_nr(const anr_inter_rat_hash_rec_t  *p_inter_rat_hash_rec,
 * son_inter_rat_neighbor_info_t   *p_inter_rat_nr
 * )
 *  \brief Function to populate inter rat neighbor info structure from
 *         inter rat hash record
 *  \param p_inter_rat_hash_rec Pointer to the inter RAT hash table record
 *  \param  p_inter_rat_nr      Pointer to the inter RAT nr info
 */
son_void_t
anr_convert_inter_rat_hash_rec_to_inter_rat_nr
(
const anr_inter_rat_hash_rec_t  *p_inter_rat_hash_rec,
son_inter_rat_neighbor_info_t   *p_inter_rat_nr
);

/*! \fn son_void_t anr_extract_global_cell_id_from_inter_rat_hash_rec(const  anr_inter_rat_hash_rec_t *p_inter_rat_hash_rec,
 * son_global_cell_id_t *p_global_cell_id
 * )
 *  \brief Function to extrac cell id from intra rat hash record.
 *  \param p_inter_rat_hash_rec Pointer to the intra RAT hash table record
 *  \param p_global_cell_id     Pointer to the Global Cell Id 
 */
son_void_t
anr_extract_global_cell_id_from_inter_rat_hash_rec
(
const  anr_inter_rat_hash_rec_t *p_inter_rat_hash_rec,
son_global_cell_id_t *p_global_cell_id
);

/*! \fn son_void_t anr_convert_intra_rat_nr_to_intra_rat_hash_rec(const son_intra_rat_neighbor_info_t *p_nr,
 * anr_intra_rat_hash_rec_t *p_hash_rec
 * )
 *  \brief  Function to convert intra rat neighbor to intra rat hash record.
 *  \param  p_nr        Pointer to intra rat neighbor
 *  \param  p_hash_rec  Pointer to intra rat hash record
 */
son_void_t
anr_convert_intra_rat_nr_to_intra_rat_hash_rec
(
const son_intra_rat_neighbor_info_t *p_nr,
anr_intra_rat_hash_rec_t *p_hash_rec
);

/*! \fn son_void_t anr_convert_intra_rat_hash_rec_to_intra_rat_nr(const anr_intra_rat_hash_rec_t  *p_intra_rat_hash_rec,
 * son_intra_rat_neighbor_info_t   *p_intra_rat_nr
 * )
 *  \brief  Function to convert intra rat hash record to intra rat neighbor.
 *  \param  p_intra_rat_hash_rec    Pointer to intra rat hash record
 *  \param  p_intra_rat_nr          Pointer to intra rat neighbor
 */
son_void_t
anr_convert_intra_rat_hash_rec_to_intra_rat_nr
(
const anr_intra_rat_hash_rec_t  *p_intra_rat_hash_rec,
son_intra_rat_neighbor_info_t   *p_intra_rat_nr
);

/*! \fn son_void_t anr_automatic_neighbor_removal()
 *  \brief  Function to do automatic neighbor removal based on timer expiry
 */
son_void_t
anr_automatic_neighbor_removal
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t son_ho_status_et anr_get_intra_rat_ho_status(
 *          anr_intra_rat_hash_rec_t *p_nr)
 *  \brief This function returns HO Status of Intra-RAT neighbors
 *  \param p_nr     Pointer to intra rat NR
 */
son_ho_status_et
anr_get_intra_rat_ho_status
(
anr_intra_rat_hash_rec_t *p_nr
);

/*! \fn son_void_t anr_delete_nbr_cells_nrt( son_global_enb_id_t *p_enb_id)
*  \ brief This function deletes all the cells served by given eNB Id from the NRT and place them in "the removed list"
*  \param p_enb_id - pointer to  global eNB Id
*/
   
son_void_t
anr_delete_nbr_cells_nrt
(
    son_global_enb_id_t  *p_enb_id
);

/*! \fn son_void_t anr_update_x2_status_nbr_cells_nrt(son_global_enb_id_t*p_enb_id)
*  \ brief This function  updates the x2 status of all the cells of given enB id present in NRT
*  \param p_enb_id - pointer to  global eNB Id
*/
son_void_t
anr_update_x2_status_nbr_cells_nrt
(
    son_global_enb_id_t      *p_enb_id
);

/*! \fn son_void_t get_ecgi_from_enb_id_cell_id(son_intra_rat_global_cell_id_t *p_intra_rat_cell_id,
 *                                              son_u8 cell_id,
 *                                              son_global_enb_id_t *p_enb_id)
*  \ brief This constructs the intra rat global cell id from the given cell id  and global enb id
*  \param p_intra_rat_cell_id - Pointer to intra rat cell id
*  \param cell_id - Pointer to cell id
*  \param p_enb_id - Pointer to enb id
*/
son_void_t
get_ecgi_from_enb_id_cell_id
(
    son_intra_rat_global_cell_id_t *p_intra_rat_cell_id,
son_u8                         cell_id,
    son_global_enb_id_t            *p_enb_id
);

/*! \fn son_void_t  anr_convert_intra_rat_nr_to_global_cell_id_struct(
 *          const son_intra_rat_neighbor_info_t *p_nr,
 *          son_global_cell_id_t *p_cell_id);
 *  \brief This function is used to populate Global cell id
 *
 *  \param p_nr   -  Pointer to a structure containing the NR
 *  \param p_cell_id - Global cell id .
 */
son_void_t
anr_convert_intra_rat_nr_to_global_cell_id_struct
(
const son_intra_rat_neighbor_info_t *p_nr,
son_global_cell_id_t *p_cell_id
);

/*! \fn son_void_t  anr_convert_inter_rat_nr_to_global_cell_id_struct(
 *          const son_inter_rat_neighbor_info_t *p_nr,
 *          son_global_cell_id_t *p_cell_id);
 *  \brief This function is used to populate Global cell id
 *
 *  \param p_nr   -  Pointer to a structure containing the NR
 *  \param p_cell_id  - Global cell id .
 */

son_void_t
anr_convert_inter_rat_nr_to_global_cell_id_struct
(
const son_inter_rat_neighbor_info_t *p_nr,
son_global_cell_id_t *p_cell_id
);

/*! \fn son_ul32 hasher( son_u8* p_first, son_u8* p_last)
 *  \brief Function to generate hash code from char array
 *  \param  p_first - Pointer to first char
 *  \param  p_last  - Pointer to last char
 */
son_ul32 
hasher
(
    son_u8 *p_first,
    son_u8 *p_last
);

/*PCI_CONFLICT_CONFUSION_CHANGES_START*/

/*! \fn son_void_t anr_construct_sent_pci_conflict_confusion_ind(son_intra_rat_global_cell_id_t *p_cell_id,
 *                                                               anr_cell_context_t *p_cell_ctxt,
 *                                                               son_u16 pci,
 *                                                               son_u32 earfcn,
 *                                                               son_pci_cause_et pci_cause,
 *                                                               son_u32 count)
 *  \brief This function composes the pci conflict / confusion indication message
 *  \param  p_cell_id - Pointer to intra rat global cell id
 *  \param  p_cell_ctxt - Pointer to cell context
 *  \param  pci - PCI value
 *  \param  earfcn - EARFCN value
 *  \param  pci_cause - enum that specifies PCI Conflict or Confusion
 *  \param  count     - number of record in pci conflict confusion list
 */
son_void_t
anr_construct_sent_pci_conflict_confusion_ind
(
 son_intra_rat_global_cell_id_t *p_cell_id,
 anr_cell_context_t           *p_cell_ctxt,
 son_u16 pci,
 son_u32 earfcn,
 son_pci_cause_et pci_cause,
 son_u32 count
 );

/*! \fn anr_intra_rat_hash_rec_t * get_other_intra_rat_nr_with_given_pci_earfcn(SON_HASH *p_hash,
 *                                                                              anr_intra_rat_hash_rec_t *p_hash_rec)
 *  \brief This function retrieves other intra rat neighbours with know earfcn and pci as input
 *  \param  p_hash - Pointer to hash recod
 *  \param  p_hash_rec -   Pointer to hash recod
 */
anr_intra_rat_hash_rec_t *
get_other_intra_rat_nr_with_given_pci_earfcn
(
  SON_HASH *p_hash,
  anr_intra_rat_hash_rec_t *p_hash_rec
);

/*! \fn anr_intra_rat_hash_rec_t * get_other_intra_rat_nr_with_given_pci_earfcn(SON_HASH *p_hash,
 *                                                                              anr_intra_rat_hash_rec_t *p_hash_rec)
 *  \brief This function retrieves other intra rat neighbours with know earfcn and pci as input
 *  \param  p_hash - Pointer to hash recod
 *  \param  p_hash_rec -   Pointer to hash recod
 */
anr_intra_rat_hash_rec_t *
get_other_intra_rat_nr_with_given_pci_earfcn
(
  SON_HASH *p_hash,
  anr_intra_rat_hash_rec_t *p_hash_rec
);

/*! \fn son_void_t insert_pci_suspect_conflict_confusion_handler( anr_cell_context_t *p_cell_ctxt,
 *                                                                anr_intra_rat_hash_rec_t *p_inserted_hash_rec,
 *                                                                son_anr_updated_nrt_info_t *p_oam_notif_upd)
 *  \brief This function inserts / add the record in the suspect list
 *  \param  p_cell_ctxt - Pointer to cell context
 *  \param  p_inserted_hash_rec - Pointer to inserted hash record
 *  \param  p_oam_notif_upd - Pointer to oam notification structure
 */

son_void_t
insert_pci_suspect_conflict_confusion_handler
(
   anr_cell_context_t           *p_cell_ctxt,
   anr_intra_rat_hash_rec_t   *p_inserted_hash_rec,
   son_anr_updated_nrt_info_t  *p_oam_notif_upd
);

/*! \fn son_void_t delete_pci_suspect_conflict_confusion_handler( anr_cell_context_t *p_cell_ctxt,
 *                                                                anr_intra_rat_hash_rec_t *p_deleted_hash_rec)
 *  \brief This function inserts / add the record in the suspect list
 *  \param  p_cell_ctxt - Pointer to cell context
 *  \param  p_deleted_hash_rec - Pointer to hash record to be deleted
 */
son_void_t
delete_pci_suspect_conflict_confusion_handler
(
   anr_cell_context_t         *p_cell_ctxt,
   anr_intra_rat_hash_rec_t   *p_deleted_hash_rec
);

/*PCI_CONFLICT_CONFUSION_CHANGES_STOP*/

son_bool_et
is_intra_rat_nbr_notifiable_to_rrm
(
   son_intra_rat_neighbor_info_t *p_nbr_info
   );

/*! \fn son_void_t anr_print_eutran_nbr_mro_info(anr_intra_rat_hash_rec_t 
 * p_eutran_rec)
 *  \brief This funciton prints the MRO specific parameters for a particular 
 *  record of INTRA RAT NRT.
 *  \param p_eutran_rec     Pointer to INTRA RAT NRT hash record
 */
son_void_t
anr_print_eutran_nbr_mro_info
(
 anr_intra_rat_hash_rec_t *p_eutran_rec
);

/* SPR Fix Start */

/*! \fn son_ul32 anr_get_hash_of_intra_rat_cell_id(son_void_t *p_cell_id)
 *  \brief This is a function for generating the hash value for the given cell Id.
 *  \param p_cell_id  - Pointer to cell id
 */
son_ul32
anr_get_hash_of_intra_rat_cell_id
(
    son_void_t *p_cell_id
);

/* SPR Fix End */

/*! \fn son_u16 anr_get_no_of_xrtt_nbrs_per_frequency(anr_cdma_xrtt_freq_stats_node_t
 * p_cdma_xrtt_freq_stats_node)
 *  \brief  This funciton returns the total number of CDMA XRTT neighbors configured per ARFCN on RRM.
 *  \param p_cdma_xrtt_freq_stats_node - Pointer to CDMA XRTT neighbor frequency node
 */
son_u16
anr_get_no_of_xrtt_nbrs_per_frequency
(
anr_cdma_xrtt_freq_stats_node_t *p_cdma_xrtt_freq_stats_node
);

/*! \fn son_u16 anr_get_no_of_hrpd_nbrs_per_frequency(anr_cdma_hrpd_freq_stats_node_t
 * p_cdma_hrpd_freq_stats_node)
 *  \brief  This function returns the total number of CDMA HRPD neighbors configured per ARFCN on RRM.
 *  \param p_cdma_hrpd_freq_stats_node - Pointer to CDMA HRPD neighbor frequency node
 */
son_u16
anr_get_no_of_hrpd_nbrs_per_frequency
(
anr_cdma_hrpd_freq_stats_node_t *p_cdma_hrpd_freq_stats_node
);

/*! \fn son_bool_et anr_validate_eutran_neighbor_for_l3(anr_intra_rat_hash_rec_t *p_hash)
 *\brief This function validates the eutran neighbours for L3.
 * \param p_hash        Pointer to ANR intra rat hash record
 */
son_bool_et
anr_validate_eutran_neighbor_for_l3
(
    anr_intra_rat_hash_rec_t *p_hash
);

SON_LIST *
anr_determine_utran_freq_stats_list
(
 anr_cell_context_t *p_cell_ctxt,
 son_utran_physical_cell_id_t pci
 );
/*SPR 15799 Fix Start*/
/*! \fn son_void_t son_anr_set_x2_status_for_new_neighbor( 
 *           anr_cell_context_t *p_cell_ctxt,
 *            anr_intra_rat_hash_rec_t *p_nr);
 *  \brief This function determines the x2status value for a given
 *          detected and new neighbor.
 *  \param p_cell_ctxt          Pointer to ANR cell context
 *  \param p_nr                 Pointer to hash record
 *  \param p_enb_id             Pointer to Global eNB ID
 */
son_void_t  son_anr_set_x2_status_for_new_neighbor(
            anr_cell_context_t *p_cell_ctxt,
            anr_intra_rat_hash_rec_t *p_nr,
            son_global_enb_id_t      *p_enb_id);
/*SPR 15799 Fix Stop*/
/* Spr 17753 Changes Start */
/*! \fn son_bool_et anr_is_serving_conflicts_with_inserted_neighbour(
         anr_cell_context_t           *p_cell_ctxt,
         anr_intra_rat_hash_rec_t   *p_hash_rec,
         son_anr_pci_conflict_confusion_ind_t *p_msg);
 *  \brief This function determines conflicts of serving cell 
              with inserted neighbour 
 *  \param p_cell_ctxt          Pointer to ANR cell context
 *  \param p_hash_rec           Pointer to hash record
 *  \param p_msg                Pointer to msg
 */
son_bool_et             
anr_is_serving_conflicts_with_inserted_neighbour
(
 anr_cell_context_t           *p_cell_ctxt,
 anr_intra_rat_hash_rec_t   *p_hash_rec,
 son_anr_pci_conflict_confusion_ind_t *p_msg
 );

/*! \fn son_bool_et anr_clean_second_degree_conflict_data(
         anr_cell_context_t           *p_cell_ctxt);
 *  \brief This function cleans the second degree conflict data 
 *  \param p_cell_ctxt          Pointer to ANR cell context
 */
son_void_t
anr_clean_second_degree_conflict_data(anr_cell_context_t * p_cell_ctxt);
/* Spr 17753 Changes End */

/*SPR-8950-Start*/

/*! \fn son_bool_et is_utran_nbr_notifiable_to_rrm( son_utran_neighbor_info_t *p_nbr_info )
 *  \brief This function checks if new utran neighbor addition/modification should be notified to RRM
 *  \param p_nbr_info     - Pointer to UTRAN nbr info
 */
son_bool_et
is_utran_nbr_notifiable_to_rrm
(
 son_utran_neighbor_info_t *p_nbr_info
 );


/*SPR-8950-End*/

/* SPR 12542/SES-483 Fix Start */
/*! \fn son_bool_et is_geran_nbr_notifiable_to_rrm( son_geran_neighbor_info_t *p_nbr_info )
 *  \brief This function checks if new geran neighbor addition/modification should be notified to RRM
 *  \param p_nbr_info     - Pointer to GERAN nbr info
 */
son_bool_et
is_geran_nbr_notifiable_to_rrm
(
 son_geran_neighbor_info_t *p_nbr_info
 );
/* SPR 12542/SES-483 Fix End */

/* Rel 3.0 Multi Sector code changes Start */

/*! \fn son_void_t anr_update_serving_cell_as_nbr_autonomously(son_intra_rat_global_cell_id_t *p_srv_cell_id)
 *  \brief This is the function which updates the given serving cell in the NRT of all other serving cells.
 *  \param p_srv_cell_id    Pointer to the serving cell Id which is to be updated in NRT of all other serving cells
 */
son_void_t
anr_update_serving_cell_as_nbr_autonomously
(
    son_intra_rat_global_cell_id_t *p_srv_cell_id
);

/*! \fn son_void_t anr_add_serving_cell_as_nbr_autonomously(son_intra_rat_global_cell_id_t *p_srv_cell_id)
 *  \brief This is the function which adds the given serving cell in the NRT of all other serving cells.
 *  \param p_srv_cell_id Pointer to the serving cell Id which is to be added in NRT of all other serving cells.
 */
son_void_t
anr_add_serving_cell_as_nbr_autonomously
(
    son_intra_rat_global_cell_id_t *p_srv_cell_id
);

/*! \fn son_void_t anr_delete_serving_cell_as_nbr_autonomously(son_intra_rat_global_cell_id_t *p_srv_cell_id)
 *  \brief This is the function which finds the given serving cell in the NRT of all other serving cells and deletes it.
 *  \param p_srv_cell_id    Pointer to the serving cell Id which is to be deleted from NRT of all other serving cells
 */
son_void_t
anr_delete_serving_cell_as_nbr_autonomously
(
    son_intra_rat_global_cell_id_t *p_srv_cell_id
);

/*! \fn son_void_t anr_add_other_serving_cells_as_nbr_autonomously (anr_cell_context_t *p_srv_cell_ctxt)
 *  \brief This is the function which finds all the serving cells which are configured at ANR and them them as the neighbor in the NRT of given serving cell (The serving cell itself is not added in its own NRT).
 *  \param p_srv_cell_ctxt  Pointer to the serving cell context whose NRT is to be updated.
 */
son_void_t
anr_add_other_serving_cells_as_nbr_autonomously
(
    anr_cell_context_t *p_srv_cell_ctxt
);

/* Rel 3.0 Multi Sector code changes Stop */

#endif /* ifndef _SON_ANR_TABLE_H_ */
