/*! \file son_anr_ctxt_mgr.h
 *  \brief This file contains the prototypes of functions defined in son_anr_ctxt_mgr.c
 *  \date  Feb, 2012
 *  \author Komal/Shekhar
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_ANR_CTXT_MGR_H_
#define _SON_ANR_CTXT_MGR_H_

/*! \headerfile son_anr_intf.h <>
 */
#include <son_anr_intf.h>

/*! \def SONANR_FSM_SET_STATE(curr_state, new_state)
 *  \brief This macro is used to update the state of Global FSM of SONANR
 */
#define SONANR_FSM_SET_STATE(curr_state, new_state) \
    do \
    { \
        SON_LOG(g_anr_context.log_enable, p_son_anr_facility_name, \
            SON_INFO, \
            "### ANR_FSM: State changed %s->%s ###", \
            SON_ANR_FSM_STATES_NAMES[curr_state], \
            SON_ANR_FSM_STATES_NAMES[new_state]); \
        curr_state = new_state; \
    } \
    while (0)

/*! \def SONANR_CELL_FSM_SET_STATE(p_cell_ctxt,prev_state,curr_state, new_state)
 *  \brief This macro is used to update the state of SONANR CELL FSM
 */
#define SONANR_CELL_FSM_SET_STATE(p_cell_ctxt, prev_state, curr_state, new_state) \
    do \
    { \
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, \
            SON_INFO, \
            "### ANR_CELL_FSM: State changed for cell [Cell Id: %d] %s->%s ###", \
            son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity), \
            SON_ANR_CELL_FSM_STATES_NAMES[curr_state], \
            SON_ANR_CELL_FSM_STATES_NAMES[new_state]); \
        prev_state = curr_state; \
        curr_state = new_state; \
    } \
    while (0)

/*! \def SON_ANR_DEFAULT_UE_COUNT
 *  \brief This macro defines the default for the number of UEs to be configured for
 *         ANR UE Measurements
 */
#define SON_ANR_DEFAULT_UE_COUNT            5

/*! \def SON_ANR_DEFAULT_REPORTING_INTERVAL
 *  \brief This macro defines the default interval at which RRM shall send
 *          the UE Measurements for ANR
 */
#define SON_ANR_DEFAULT_REPORTING_INTERVAL  60

/*! \def ANR_TIMEOUT_FOR_PENDING_RES
 *  \brief A macro that stores the seconds in which register/deregister
 *  response shall be received for all the cells.
 */

#define ANR_TIMEOUT_FOR_PENDING_RES 60

/* SPR-12708 Fix Starts */
/*! \def ANR_DEFAULT_TIMEOUT
 *  \brief This macro defines the default interval at which ANR
 *          handles timer expiry. The value is in seconds.
 */
#define ANR_DEFAULT_TIMEOUT     15
/* SPR-12708 Fix Ends */

/* SPR 15698 Start */
#define CGI_GAURD_TIMER_PERIODICITY_IN_MSECS 1000
#define CGI_GAURD_TIMEOUT_IN_MSECS 2000
/* SPR 15698 End */

/* SPR 15831 Start */
#define MSECS_IN_SEC 1000
/* SPR 15831 End */

/*! \def ANR_INTRA_RAT_NBRS_MAX_COUNT
 *  \brief This macro defines the maximum neighbor allowed
 */
#define ANR_INTRA_RAT_NBRS_MAX_COUNT       16

/*! \def MAX_NUM_OF_ALLOWED_EARFCNS_TO_RRM
 *  \brief This macro defines the maximum allowed EARFCNs
 */
#define MAX_NUM_OF_ALLOWED_EARFCNS_TO_RRM   9

/*! \def MAX_NUM_OF_ALLOWED_GERAN_FREQ
 *  \brief This macro defines the maximum allowed ARFCN
 */
#define MAX_NUM_OF_ALLOWED_GERAN_FREQ   1024

/*! \def MAX_NUM_OF_GERAN_BAND_INDICATOR
 *  \brief This macro defines the maximum no of geran band indicator
 */
#define MAX_NUM_OF_GERAN_BAND_INDICATOR     2

/* SPR 20653 Fix Start */
#define SON_DEFAULT_UE_THROUGHPUT_DL_ANR_THRESHOLD  50
#define SON_DEFAULT_UE_THROUGHPUT_UL_ANR_THRESHOLD  50
/* SPR 20653 Fix End */

/*! \a const son_u8 SON_ANR_FSM_STATES_NAMES[];
 * \brief ANR Fsm State Names
 */
extern const son_s8 *SON_ANR_FSM_STATES_NAMES[];

/*! \a const son_u8 SON_ANR_CELL_FSM_STATES_NAMES[];
 * \brief ANR Cell FSm State Names
 */
extern const son_s8 *SON_ANR_CELL_FSM_STATES_NAMES[];

/*! \a const son_u8 SON_ANR_CELL_FSM_EVENT_NAMES[];
 * \brief ANR Cell Fsm Event Names
 */
extern const son_s8 *SON_ANR_CELL_FSM_EVENT_NAMES[];

/*! \enum anr_cell_fsm_state_et
 *  \brief An enum that specifies the SON ANR CELL FSM states
 */
typedef enum
{
    ANR_CELL_STATE_ENABLING = 0,
    ANR_CELL_STATE_ENABLED,
    ANR_CELL_STATE_DISABLING,
    ANR_CELL_STATE_DISABLED,
    /* SPR 20653 Fix Start */
    ANR_CELL_STATE_MAX,
    /* SPR 20653 Fix End */
}anr_cell_fsm_state_et;

/*! \enum anr_fsm_state_et
 *  \brief An enum that specifies the SON ANR FSM states
 */
typedef enum
{
    ANR_STATE_INIT = 0,
    ANR_STATE_ACTIVE,
    ANR_STATE_ENABLING,
    ANR_STATE_DISABLING,
    ANR_STATE_SHUTDOWN,
    ANR_STATE_UNDEFINED,
}anr_fsm_state_et;


/*! \  anr_hash_pool_t from STACKCPOOL
 */
typedef STACKCPOOL anr_hash_pool_t;


/*! \ anr_rach_related_info_t
 *  \brief Rach related Information block
 *  \param old_prach_configuration              Old data
 *  \param is_cell_info_ind_valid_as_per_anr    Flag to determine the 
                                                serving data validity
 *  \param is_old_prach_configuration_available To identify if prach 
                                                configuraiton is 
                                                available or not.
 */

typedef struct _anr_rach_related_info_t
{
    son_prach_configuration_t  old_prach_configuration;
    son_bool_et                is_cell_info_ind_valid_as_per_anr;
    son_bool_et                is_old_prach_configuration_available;
}anr_rach_related_info_t;



/*! \  anr_cell_context_t
 *  \brief ANR cell Specific context
 *  \param cell_id                      ECGI of cell
 *  \param pci_value                    PCI of the serving cell
 *  \param earfcn                       EARFCN value
 *  \param previous_cell_fsm_state      ANR's FSM previous state of cell
 *  \param current_cell_fsm_state       ANR's FSM current state of cell
 *  \param current_cell_state			Cell State.
 *  \param intra_rat_neighbor_table	    Intra rat Neighbor relation table.
 *  \param inter_rat_neighbor_table	    Inter rat Neighbor relation table.
 *  \param nr_remove_list               Cell specific neighbor Remove List
 *  \param removal_criteria             Neighbor removal criteria info
 *  \param pci_confusion_cfg            Neighbor PCI confusion configuration
 *  \param limited_mode                 Limited mode Enabled/Disabled
 *  \param bs_allowed_stats_list        Broadcast neighbor statistics list per EARFCN
 *  \param prach_configuration          PRACH Configuration.
 *  \param rach_related_info            Old PRACH Configuration for RACH.
 */
typedef struct _anr_cell_context_t
{
    son_intra_rat_global_cell_id_t cell_id;
    son_u16 pci_value;
    son_u8 tac[SON_TAC_OCTET_SIZE];
    son_u8 num_plmn_id;
    son_cell_plmn_info_t plmn_id[SON_MAX_NUM_PLMN];
    son_choice_eutra_mode_t choice_eutra_mode;
    anr_cell_fsm_state_et previous_cell_fsm_state;
    anr_cell_fsm_state_et current_cell_fsm_state;
    rrm_cell_operational_state_et current_cell_state;
    SON_HASH intra_rat_neighbor_table;
    SON_HASH inter_rat_neighbor_table;
    SON_LIST nr_remove_list;
    SON_LIST txn_id_list;
    son_neighbor_removal_info_t removal_criteria;
    son_neighbor_pci_confusion_info_t pci_confusion_cfg;
    son_bool_et limited_mode;
    son_u8 csg_identity[SON_CSG_ID_OCTET_SIZE];
    SON_LIST bs_allowed_stats_list;
    SON_LIST cdma_xrtt_band_stats_list;
    SON_LIST cdma_hrpd_band_stats_list;
    SON_LIST pci_sus_conf_ctxt_list;
    SON_LIST utran_freq_fdd_stats_list;
    SON_LIST utran_freq_tdd_stats_list;
    SON_LIST geran_freq_stats_list;
    son_rrm_meas_config_obj_remove_req_t *p_meas_config_obj_remove;
    son_bool_et                is_prach_configuration_available;
    son_prach_configuration_t  prach_configuration;
    anr_rach_related_info_t    rach_related_info;
    son_bool_et                is_csg_id_available;
    SON_LIST extended_rnc_id_list;
    SON_LIST non_extended_rnc_id_list;
    SON_LIST eutran_pci_requested_cgi_list; /*SPR-9759-Fix*/
    SON_LIST utran_pci_requested_cgi_list; /*SPR-9759-Fix*/
    SON_LIST geran_pci_requested_cgi_list; /*SPR-9759-Fix*/
    SON_LIST cdma_pci_requested_cgi_list; /*SPR-9759-Fix*/
    SON_LIST conflict_confusion_pci_list; /*SPR-12596-Start-End*/
#ifdef LTE_EMBMS_SUPPORTED
    son_bool_et                is_srv_mbms_available;
    son_mbms_configuration_t   mbms_config;
#endif
    /*SPR_19279_START*/
    son_u8 cell_index;
    /*SPR_19279_END*/

    /* Spr 17753 Changes Start */
    son_bool_et is_conflict_with_second_degree_enable;
    son_timer_duration_t conflict_detection_backoff_timer_val;
    son_timer_t son_backoff_timer_id;
    son_timer_duration_t nr_add_wait_timer_val;
    son_timer_t son_nr_add_timer_id;
    SON_LIST second_degree_nbr_list;
    /* Spr 17753 Changes End */
}anr_cell_context_t;

/* Spr 17753 Changes Start */
/*! \ anr_timer_expiry_flag_et
 *  \brief timer expiry type NR_ADD/BACK_OFF 
 */
typedef enum
{
    NR_ADD_TIMER_EXPIRED = 0,
    BACK_OFF_TIMER_EXPIRED, 
    UNDEFINED  = 255
}anr_timer_expiry_flag_et;
/* Spr 17753 Changes End */
/*! \ son_pci_state_et
 *  \brief SON PCI State i.e. Confused/Suspected
 */
typedef enum
{
    SON_PCI_CONFUSED  = 0,
    SON_PCI_SUSPECTED
}son_pci_state_et;

/*! \ anr_pci_sus_conf_data_t
 *  \brief  ANR Cell specific Suspect-Confused PCI ctxt node
 *  \param  pci              - PCI
 *  \param  earfcn           - EARFCN
 *  \param  state            - Suspected/Confused
 *  \param  ask_cgi_counter  - Number of times CGI has been asked
 *  \param  ask_cgi_thx_val  - Threshhold value
 */
typedef struct _anr_pci_sus_conf_data_t
{
    son_u16 pci;
    son_u32 earfcn;
    son_pci_state_et state;
    son_u8 ask_cgi_counter;
    son_u8 ask_cgi_thx_val;
    son_u32 nbr_cell_count;
}anr_pci_sus_conf_data_t;

/*! \  anr_pci_sus_conf_node_t
 *  \brief ANR Cell specific transaction Id list Node
 *  \param anchor                           CSPL's Node
 *  \param pci_sus_conf_data                Suspected-confused PCI context.
 */
typedef struct _anr_pci_sus_conf_node_t
{
    SON_LIST_NODE anchor;
    anr_pci_sus_conf_data_t pci_sus_conf_data;
}anr_pci_sus_conf_node_t;

/*! \ anr_geran_freq_stats_data_t
 *  \brief Record for maintaining num of GERAN nbrs send to RRM for a given frequency.
 *  \param arfcn                      ARFCN
 *  \param band_ind                   band indicator
 *  \param num_of_geran_nbrs_to_rrm   number of cells send to RRM for a given frequency
 */
typedef struct _anr_geran_freq_stats_data_t
{
    son_u16 arfcn;
    son_geran_band_indicator_et band_ind;
    son_u8 num_of_geran_nbrs_to_rrm;
}anr_geran_freq_stats_data_t;

/*! \ anr_geran_freq_stats_node_t
 *  \brief Geran freq stats list node
 *  \param anchor                    CSPL's Node
 *  \param geran_stats               Geran data
 */
typedef struct _anr_geran_freq_stats_node_t
{
    SON_LIST_NODE anchor;
    anr_geran_freq_stats_data_t geran_stats;
}anr_geran_freq_stats_node_t;

/*! \ anr_utran_freq_stats_data_t
 *  \brief Record for maintaining num of UTRAN nbrs send to RRM for a given frequency.
 *  \param uarfcn                      UARFCN
 *  \param num_of_utran_nbrs_to_rrm    number of cells send to RRM for a given frequency
 */
typedef struct _anr_utran_freq_stats_data_t
{
    son_u16 uarfcn;
    son_u8 num_of_utran_nbrs_to_rrm;
}anr_utran_freq_stats_data_t;

/*! \ anr_utran_freq_stats_node_t
 *  \brief Utran freq stats list node
 *  \param anchor                    CSPL's Node
 *  \param utran_stats               Utran data
 */
typedef struct _anr_utran_freq_stats_node_t
{
    SON_LIST_NODE anchor;
    anr_utran_freq_stats_data_t utran_stats;
}anr_utran_freq_stats_node_t;

/*! \  anr_bs_allowed_data_t
 *  \brief ANR EARFCN specific broadcasted neighbor count Record
 *  \param earfcn                                 EARFCN
 *  \param bs_allowed_ho_allowed_nbrs_count       HO Allowed nbrs with allowed Broadcast Status
 *  \param bs_allowed_ho_blklisted_nbrs_count     HO Blacklisted nbrs with allowed Broadcast Status.
 *  \param bs_not_allowed_ho_allowed_nbrs_count   HO allowed nbrs with not allowed Broadcast Status
 *  \param bs_not_allowed_ho_blklisted_nbrs_count HO Blacklisted nbrs with not allowed Broadcast Status
 */
typedef struct _anr_bs_allowed_data_t
{
    son_u32 earfcn;
    son_u8 bs_allowed_ho_allowed_nbrs_count;
    son_u8 bs_allowed_ho_blklisted_nbrs_count;
    son_u8 bs_not_allowed_ho_allowed_nbrs_count;
    son_u8 bs_not_allowed_ho_blklisted_nbrs_count;
}anr_bs_allowed_data_t;

/*! \  anr_bs_allowed_node_t
 *  \brief ANR EARFCN specific broadcasted neighbor count Node
 *  \param brdcast_stats_list_node         CSPL's Node
 *  \param brdcast_nbr_per_earfcn_record   Broadcasted Nbr count record
 */
typedef struct _anr_bs_allowed_node_t
{
    SON_LIST_NODE bs_allowed_stats_list_node;
    anr_bs_allowed_data_t bs_allowed_per_earfcn_record;
}anr_bs_allowed_node_t;

/*! \  anr_txn_id_node_t
 *  \brief ANR Cell specific transaction Id list Node
 *  \param txn_list_node                CSPL's Node
 *  \param txn_id                       Transaction Identifier
 */
typedef struct _anr_txn_id_node_t
{
    SON_LIST_NODE txn_list_node;
    son_u16 txn_id;
}anr_txn_id_node_t;

/*start:gur32802  nbr table maintaining*/

/*! \  anr_cdma_xrtt_stats_data_t
 *  \brief ANR CDMA xrtt stats node structure containing band class and pointer to head of frequency list
 *  \param band_class                   CDMA2000 band class
 *  \param freq_list                    Pointer to head of frequency info link list
 */
typedef struct _anr_cdma_xrtt_stats_data_t
{
    son_u16 band_class;
    SON_LIST freq_list;
}anr_cdma_xrtt_stats_data_t;

/*! \  anr_cdma_xrtt_band_stats_node_t
 *  \brief ANR CDMA xrtt band stats node structure containing anchor to band node stucture and band class node
 *  \param anchor                      anchor to CDMA2000 band class node
 *  \param cdma_band_stats             CDMA2000 band class node structure
 */
typedef struct _anr_cdma_xrtt_stats_node_t
{
    SON_LIST_NODE anchor;
    anr_cdma_xrtt_stats_data_t cdma_band_stats;
}anr_cdma_xrtt_band_stats_node_t;

/*! \  anr_cdma_xrtt_freq_data_t
 *  \brief ANR CDMA xrtt freq stats data node structure containing count for nbr on there broadcast status and ho statusanr_cdma_xrtt_freq_data_t
 *  \param arfcn                       CDMA2000 frequency
 *  \param num_bs_true_ho_allwd        CDMA2000 NBR with broadcast status true and ho allowed
 *  \param num_bs_true_ho_blklstd      CDMA2000 NBR with broadcast status true and ho blacklisted
 *  \param num_bs_false_ho_allwd       CDMA2000 NBR with broadcast status false and ho allowed
 *  \param num_bs_false_ho_blklstd     CDMA2000 NBR with broadcast status false and ho blacklisted
 */
typedef struct _anr_cdma_xrtt_freq_data_t
{
    son_u16 arfcn;
    son_u16 num_bs_true_ho_allwd;
    son_u16 num_bs_true_ho_blklstd;
    son_u16 num_bs_false_ho_allwd;
    son_u16 num_bs_false_ho_blklstd;
}anr_cdma_xrtt_freq_data_t;

/*! \  anr_cdma_xrtt_freq_stats_node_t
 *  \brief ANR CDMA xrtt freq stats node structure containing anchor to freq node stucture and band class
 *  \param anchor                      anchor to CDMA2000 frequency list node
 *  \param arfcn_stats                 CDMA2000 freq class node structure
 */
typedef struct _anr_cdma_xrtt_freq_stats_node_t
{
    SON_LIST_NODE anchor;
    anr_cdma_xrtt_freq_data_t arfcn_stats;
}anr_cdma_xrtt_freq_stats_node_t;

/*! \  anr_cdma_hrpd_stats_data_t
 *  \brief ANR CDMA hrpd stats node structure containing band class information that is sendable to rrm
 *  \param band_class                  CDMA2000 band class
 *  \param freq_list                   Pointer to head of frequency list
 */
typedef struct _anr_cdma_hrpd_stats_data_t
{
    son_u16 band_class;
    SON_LIST freq_list;
}anr_cdma_hrpd_stats_data_t;

/*! \  anr_cdma_hrpd_freq_data_t
 *  \brief ANR CDMA hrpd band stats node structure containing anchor to band node stucture and band class node
 *  \param anchor                      anchor to CDMA2000 band class node
 *  \param cdma_band_stats             CDMA2000 band class node structure
 */

typedef struct _anr_cdma_hrpd_stats_node_t
{
    SON_LIST_NODE anchor;
    anr_cdma_hrpd_stats_data_t cdma_band_stats;
}anr_cdma_hrpd_band_stats_node_t;

/*! \  anr_cdma_hrpd_freq_data_t
 *  \brief ANR CDMA xrtt freq stats data node structure containing count for nbr on there broadcast status and ho statusanr_cdma_hrpd_freq_data_t
 *  \param arfcn                       CDMA2000 frequency
 *  \param num_bs_true_ho_allwd        CDMA2000 NBR with broadcast status true and ho allowed
 *  \param num_bs_true_ho_blklstd      CDMA2000 NBR with broadcast status true and ho blacklisted
 *  \param num_bs_false_ho_allwd       CDMA2000 NBR with broadcast status false and ho allowed
 *  \param num_bs_false_ho_blklstd     CDMA2000 NBR with broadcast status false and ho blacklisted
 */
typedef struct _anr_cdma_hrpd_freq_data_t
{
    son_u16 arfcn;
    son_u16 num_bs_true_ho_allwd;
    son_u16 num_bs_true_ho_blklstd;
    son_u16 num_bs_false_ho_allwd;
    son_u16 num_bs_false_ho_blklstd;
}anr_cdma_hrpd_freq_data_t;

/*! \  anr_cdma_hrpd_freq_stats_node_t
 *  \brief ANR CDMA hrpd freq stats node structure containing anchor to freq node stucture and band class
 *  \param anchor                      anchor to CDMA2000 frequency list node
 *  \param arfcn_stats                 CDMA2000 freq class node structure
 */
typedef struct _anr_cdma_hrpd_freq_stats_node_t
{
    SON_LIST_NODE anchor;
    anr_cdma_hrpd_freq_data_t arfcn_stats;
}anr_cdma_hrpd_freq_stats_node_t;

/*end:gur32802  */

/*SPR-9759-Start*/

/*! \typedef _ue_index_and_counter_data_t
 *  \param ue_index Index of the UE
 *  \param cgi_request_counter  CGI Requests counter
 */
typedef struct _ue_index_and_counter_data_t
{
    son_u16 ue_index;
    son_u8  cgi_request_counter;
}ue_index_and_counter_data_t;

/*! \typedef _ue_index_and_counter_list_node_t
 *  \param anchor   anchor to UE Index and CGI Req. counter list  
 *  \param ue_index_and_counter_values node structure of UE Index and CGI Req. counter list  
 */
typedef struct _ue_index_and_counter_list_node_t
{
    SON_LIST_NODE anchor;
    ue_index_and_counter_data_t ue_index_and_counter_values;
}ue_index_and_counter_list_node_t;

/*! \typedef _anr_eutran_cdma_pci_value_data_t
 *  \param pci PCI of the cell
 *  \param ue_index_and_counter_list UE Index and CGI Req. counter list
 */

/*SPR 18360 Fix Start*/
/*! \typedef _anr_eutran_key_t
 *  \param pci PCI of the cell
 *  \param earfcn EARFCN of the cell
 */
typedef struct _anr_eutran_key_t
{
    son_u16  pci;
    son_u32  earfcn;
}anr_eutran_key_t;

/*! \typedef _anr_cdma_key_t
 *  \param pci PCI of the cell
 *  \param earfcn EARFCN of the cell
 */
typedef struct _anr_cdma_key_t
{
    son_u16                             pci;
    son_cdma_band_class_et              band_class;
    son_u16                             arfcn;
}anr_cdma_key_t;

/*! \typedef _anr_cdma_key_t
 *  \param pci PCI of the cell
 *  \param earfcn EARFCN of the cell
 */
typedef union _Key_t
{
  anr_eutran_key_t    eutran_key;
  anr_cdma_key_t      cdma_key;
}Key_t;

#define EUTRAN_KEY  0x01
#define CDMA_KEY    0x02
/*! \typedef anr_eutran_cdma_key_t
 *  \param t  key_type
 *  \param key Key Union
 */
typedef struct  _anr_eutran_cdma_key_t
{
  son_u8  t; /*to identify the eutran or Cdma node*/ 
  Key_t   key;

}anr_eutran_cdma_key_t;
/*SPR 18360 Fix Stop*/

typedef struct _anr_eutran_cdma_pci_value_data_t
{
    /*SPR 18360 Fix Start*/
    /*Lines Deleted*/
    anr_eutran_cdma_key_t  key;
    /*SPR 18360 Fix Stop*/
    SON_LIST ue_index_and_counter_list;
}anr_eutran_cdma_pci_value_data_t;

/*SPR 18360 Fix Start*/
/*! \typedef _anr_utran_key_t
 *  \param pci PCI of the cell
 *  \param uarfcn UARFCN of the cell
 */
typedef struct _anr_utran_key_t
{
    rrm_son_utran_physical_cell_id_t  pci;
    son_u16  uarfcn;
}anr_utran_key_t;
/*SPR 18360 Fix Stop*/

/*! \typedef _anr_utran_pci_value_data_t
 *  \param pci PCI of the cell
 *  \param ue_index_and_counter_list UE Index and CGI Req. counter list
 */
typedef struct _anr_utran_pci_value_data_t
{
    /*SPR 18360 Fix Start*/
    /*Lines Deleted*/
    anr_utran_key_t  key;
    /*SPR 18360 Fix Stop*/
    SON_LIST ue_index_and_counter_list;
}anr_utran_pci_value_data_t;

/* Spr 18615 Changes Start */
/*! \typedef _anr_geran_key_t
 *  \param pci PCI of the cell
 *  \param arfcn ARFCN of the cell
 */
typedef struct _anr_geran_key_t
{
    rrm_son_geran_physical_cell_id_t  pci;
    son_u16  arfcn;
}anr_geran_key_t;
/* Spr 18615 Changes End */
/*! \typedef _anr_geran_pci_value_data_t
 *  \param pci PCI of the cell
 *  \param ue_index_and_counter_list UE Index and CGI Req. counter list
 */
typedef struct _anr_geran_pci_value_data_t
{
    /* Spr 18615 Changes Start */
    anr_geran_key_t key;
    /* Spr 18615 Changes End */
    SON_LIST ue_index_and_counter_list;
}anr_geran_pci_value_data_t;

/*! \typedef _anr_eutran_cdma_pci_value_node_t
 *  \brief ANR EUTRAN CDMA PCI value list Node
 *  \param anchor   anchor to EUTRAN or CDMA PCI value list
 *  \param eutran_cdma_pci_value  EUTRAN or CDMA PCI value list node structure
 */
typedef struct _anr_eutran_cdma_pci_value_node_t
{
    SON_LIST_NODE anchor;
    anr_eutran_cdma_pci_value_data_t eutran_cdma_pci_value;

    /* SPR 15698 Start */
    /// This field contains the value = Current Timestamp(secs) + CGI Gaurd Time(secs)
    time_stamp_t absolute_cgi_gaurd_time_in_secs;
    /* SPR 15698 End */
}anr_eutran_cdma_pci_value_node_t;

/*! \typedef _anr_utran_pci_value_node_t
 *  \brief ANR UTRAN PCI valuelist Node
 *  \param anchor   anchor to UTRAN PCI value list
 *  \param utran_pci_value  UTRAN PCI value list node structure
 */
typedef struct _anr_utran_pci_value_node_t
{
    SON_LIST_NODE anchor;
    anr_utran_pci_value_data_t utran_pci_value;

    /* SPR 15698 Start */
    /// This field contains the value = Current Timestamp(secs) + CGI Gaurd Time(secs)
    time_stamp_t absolute_cgi_gaurd_time_in_secs;
    /* SPR 15698 End */
}anr_utran_pci_value_node_t;

/*! \typedef _anr_geran_pci_value_node_t
 *  \brief ANR GERAN PCI value list Node
 *  \param anchor   anchor to the GERAN PCI value list
 *  \param geran_pci_value  GERAN PCI value list node structure
 */
typedef struct _anr_geran_pci_value_node_t
{
    SON_LIST_NODE anchor;
    anr_geran_pci_value_data_t geran_pci_value;

    /* SPR 15698 Start */
    /// This field contains the value = Current Timestamp(secs) + CGI Gaurd Time(secs)
    time_stamp_t absolute_cgi_gaurd_time_in_secs;
    /* SPR 15698 End */
}anr_geran_pci_value_node_t;

/*SPR-9759-End*/

/*SPR-9574-Start*/

/*! \typedef anr_intra_freq_list_data_t
 *  \brief ANR Intra freq list Node data
 *  \param earfcn                       Frequency
 *  \param intra_freq_open_pci_range    Open pci range
 *  \param intra_freq_closed_pci_range  Closed pci range 
 *  \param intra_freq_hybrid_pci_range  Hybrid pci range
 */
typedef struct _anr_intra_freq_list_data_t
{
    son_u32 earfcn;
    /*SPR-10426-Start*/
    son_bool_et                             open_pci_range_present;
    son_bool_et                             closed_pci_range_present;
    son_bool_et                             hybrid_pci_range_present;
    /*SPR-10426-End*/
    son_per_freq_pci_range_t                intra_freq_open_pci_range;
    son_per_freq_pci_range_t                intra_freq_closed_pci_range;
    son_per_freq_pci_range_t                intra_freq_hybrid_pci_range;
}anr_intra_freq_list_data_t;

/*! \typedef anr_inter_freq_list_data_t
 *  \brief ANR Inter freq list Node data
 *  \param earfcn                       Frequency
 *  \param inter_freq_open_pci_range    Open pci range
 *  \param inter_freq_closed_pci_range  Closed pci range
 *  \param inter_freq_hybrid_pci_range  Hybrid pci range
 */
typedef struct _anr_inter_freq_list_data_t
{
    son_u32 earfcn;
    /*SPR-10426-Start*/
    son_bool_et                             open_pci_range_present;
    son_bool_et                             closed_pci_range_present;
    son_bool_et                             hybrid_pci_range_present;
    /*SPR-10426-End*/
    son_per_freq_pci_range_t                inter_freq_open_pci_range;
    son_per_freq_pci_range_t                inter_freq_closed_pci_range;
    son_per_freq_pci_range_t                inter_freq_hybrid_pci_range;
}anr_inter_freq_list_data_t;

/*! \typedef anr_intra_freq_list_node_t
 *  \brief ANR Intra freq list Node
 *  \param anchor
 *  \param intra_freq_value Node data
 */
typedef struct _anr_intra_freq_list_node_t
{
    SON_LIST_NODE anchor;
    anr_intra_freq_list_data_t intra_freq_value;
}anr_intra_freq_list_node_t;

/*! \typedef anr_inter_freq_list_node_t
 *  \brief ANR Inter freq list Node
 *  \param                    anchor
 *  \param inter_freq_value   Node data
 */
typedef struct _anr_inter_freq_list_node_t
{
    SON_LIST_NODE anchor;
    anr_inter_freq_list_data_t inter_freq_value;
}anr_inter_freq_list_node_t;

/*SPR-9574-End*/

/*SPR-12596-Start*/

/*! \typedef conflict_confusion_pci_list_data_t
 *  \param  src_cgi
 *  \param  cgi_1
 *  \param  cgi_2
 *  \param  pci
 *  \param  cause
 */
typedef struct _conflict_confusion_pci_list_data_t
{
    son_u16                     pci;
    son_global_cell_id_t        src_cgi;
    son_global_cell_id_t        cgi_1;
    son_global_cell_id_t        cgi_2;
    son_bool_et                 is_in_conflict;
    son_bool_et                 is_confused;
    son_u16                     confusion_counter;
}conflict_confusion_pci_list_data_t;


/*! \typedef conflict_confusion_pci_list_node_t
 *  \param  anchor
 *  \param  Node data
 */
typedef struct _conflict_confusion_pci_list_node_t
{
    SON_LIST_NODE anchor;
    conflict_confusion_pci_list_data_t pci_data;
}conflict_confusion_pci_list_node_t;

/*SPR-12596-End*/

/*! \typedef anr_nr_remove_list_node_t
 *  \brief ANR Cell specific Remove list Node
 *  \param nr_remove_list_node          CSPL's Node
 *  \param cell_id                      Global Cell id
 */
typedef struct _anr_nr_remove_list_node_t
{
    SON_LIST_NODE nr_remove_list_node;
    son_global_cell_id_t cell_id;
}anr_nr_remove_list_node_t;

/*! \  anr_cell_context_node_t
 *  \brief ANR Context List's Node
 *  \param sNode	CSPL's Node
 *  \param data	ANR cell Specific context
 */
typedef struct _anr_cell_context_node_t
{
    SON_LIST_NODE sNode;
    anr_cell_context_t data;
} anr_cell_context_node_t;

/*! \  anr_global_t
 *  \brief ANR Global Context structure
 *  \param current_fsm_state		    Global ANR State
 *  \param anr_global_mode			    Global ANR Mode (Enabled or Disabled)
 *  \param pci_mode                     PCI Mode (Enabled or Disabled)
 *  \param pending_scan_res_count       Count of the pending nmm scan responses
 *  \param pending_scan_res_count       Error Code of the pending nmm scan responses
 *  \param x2_enabled                   X2 Enabled/Disabled
 *  \param log_enable				    log mode for ANR module
 *  \param anr_timer_id                 ANR Timer Identifier
 *  \param own_enb_id                   Own global eNB Identifier
 *  \param intra_rat_hash_rec_pool      ANR Intra RAT Hash Context Pool
 *  \param hash_node_pool               Hash Table Pool
 *  \param inter_rat_hash_rec_pool      ANR Inter RAT Hash Context Pool
 *  \param cell_ctxt_list			    ANR cell context List
 *  \param cell_ctxt_list			    ANR X2 Pending Response List
 *  \param log_level                    ANR log level
 */
typedef struct _anr_global_t
{
    anr_fsm_state_et current_fsm_state;
    son_app_mode_et anr_global_mode;
    son_pci_mode_et pci_mode;
    son_u8 pending_scan_res_count;
    son_error_et pending_scan_res_error_code;
    son_bool_et x2_enabled;
    son_oam_log_on_off_et log_enable;
    son_timer_t anr_timer_id;
    /* Rel 3.0 Multi Sector code changes Start */
    son_global_enb_id_t own_enb_id;
    /* Rel 3.0 Multi Sector code changes Stop */
    anr_hash_pool_t intra_rat_hash_rec_pool;
    anr_hash_pool_t hash_node_pool;
    anr_hash_pool_t inter_rat_hash_rec_pool;
    SON_LIST cell_ctxt_list;
    SON_LIST x2_pending_res_list;
    /*SPR-9574-Start*/
    SON_LIST intra_freq_list;
    SON_LIST inter_freq_list;
    /*SPR-9574-End*/

    /* SPR-13251 Fix Starts */
    son_u8 log_level;
    /* SPR-13251 Fix Ends */

    /* SPR 15698 Start */
    son_timer_t cgi_gaurd_timer_id;
    /* SPR 15698 End */

    /* eICIC feature code starts */
    son_u32 csg_scheme;
    son_u32 pico_scheme;
    son_u32 macro_scheme;
    /* eICIC feature code ends */
}anr_global_t;

/*! \fn son_return_et anr_init_global_context ()
 *  \brief This function initializes the  ANR global context
 */
/* + SPR 17439 */
son_return_et
anr_init_global_context(void);
/* - SPR 17439 */

/*! \fn son_void_t  anr_init_cell_context (anr_cell_context_t *p_ctxt)
 *  \brief   This function initializes the specified cell context
 *  \param p_ctxt  - Pointer to cell context.
 */
son_return_et
anr_init_cell_context
(
    anr_cell_context_t *p_ctxt
);

/*! \fn son_void_t anr_reset_pending_res ()
 *  \brief Resets pending response structure
 */
son_void_t
anr_reset_pending_res
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t anr_init_pending_res (son_u16 tid,son_procedure_code_et  api)
 *  \brief Initialize pending_response structure
 *  \param tid: Transaction Id
 *  \param api: Procedure Code
 */
son_void_t
anr_init_pending_res
(
    son_u16 tid,
    son_procedure_code_et api
);

/*! \fn son_u16 anr_get_pending_res_transaction_id ()
 *  \brief Get transaction id in  pending response
 */
son_u16
anr_get_pending_res_transaction_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_u8 anr_get_pending_res_expected_count ()
 *  \brief Get Expected Response Count present in pending response
 */
son_u8
anr_get_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t anr_increment_pending_res_expected_count ()
 *  \brief Increment Expected Response Count present in pending response structure
 */
son_void_t
anr_increment_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t anr_decrement_pending_res_expected_count ()
 *  \brief Decrement Expected Response Count present in pending response
 */
son_void_t
anr_decrement_pending_res_expected_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_procedure_code_et anr_get_pending_res_api_id ()
 *  \brief Get API ID present in pending response
 */
son_procedure_code_et
anr_get_pending_res_api_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn SON_LIST * anr_get_pending_res_cell_list ()
 *  \brief Returns pointer to cell_status_list present in pending response
 */
SON_LIST *
anr_get_pending_res_cell_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_u32 anr_get_pending_res_ts_diff_from_curr_ts ()
 *  \brief Returns difference between current timestamp and timestamp
 *                 present in pending response
 */
son_u32
anr_get_pending_res_ts_diff_from_curr_ts
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t anr_set_pending_res_ts_with_current_timestamp ()
 *  \brief Set timestamp present in pending response with current time
 */
son_void_t
anr_set_pending_res_ts_with_current_timestamp
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t anr_set_pending_res_error_rcvd (son_bool_et val)
 *  \brief Set is_eror_received flag present in pending response
 *  \param val    bool value
 */
son_void_t
anr_set_pending_res_error_rcvd
(
    son_bool_et val
);

/*! \fn son_bool_et anr_get_pending_res_error_rcvd ()
 *  \brief Get is_error_received flag present in pending response
 */
son_bool_et
anr_get_pending_res_error_rcvd
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn cell_status_list_data_t * anr_find_cell_in_pending_res (son_intra_rat_global_cell_id_t *p_rcvd_cell_id)
 *  \brief Find cell_status node in the cell_status_list
 *  \param p_rcvd_cell_id : Pointer to ECGI of the cell
 */
cell_status_list_data_t *
anr_find_cell_in_pending_res
(
    son_intra_rat_global_cell_id_t *p_rcvd_cell_id
);

/*! \fn son_void_t anr_delete_succesful_cells_from_pending_res ()
 *  \brief Delete cell_status nodes with result = SON_SUCCESS in the cell_status_list
 */
son_void_t
anr_delete_succesful_cells_from_pending_res
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_bool_et anr_add_cell_in_pending_res (son_cell_status_t    *p_sts_node)
 *  \brief Add cell_status node in the cell_status_list
 *  \param p_sts_node : Pointer to Cell Status Node
 */
son_bool_et
anr_add_cell_in_pending_res
(
    son_cell_status_t    *p_sts_node
);

/*! \fn son_void_t anr_delete_cell_from_pending_res (son_intra_rat_global_cell_id_t *p_cell_id)
 *  \brief Delete cell_status node from the cell_status_list
 *  \param p_cell_id : Pointer to Cell Id
 */
son_void_t
anr_delete_cell_from_pending_res
(
    son_intra_rat_global_cell_id_t *p_cell_id
);

/*! \fn son_void_t anr_clear_pending_res_list ()
 *  \brief Delete all cell_status node from the cell_status_list
 */
son_void_t
anr_clear_pending_res_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_bool_et anr_update_cell_in_pending_res (son_intra_rat_global_cell_id_t
 * p_rcvd_cell_id,son_error_et err_code)
 *  \brief Update cell_status node in the cell_status_list
 *  \param p_rcvd_cell_id   Pointer of ECGI of cell
 *  \param err_code         error code
 */
son_bool_et
anr_update_cell_in_pending_res
(
    son_intra_rat_global_cell_id_t  *p_rcvd_cell_id,
    son_error_et err_code
);

/*! \fn son_void_t anr_construct_cell_arr_from_pending_res_list(son_cell_status_t *p_cell_sts_arr)
 *  \brief Construct cell_status_arr from cell_status_list
 *  \param p_cell_sts_arr       Pointer to cell status
 */
son_void_t
anr_construct_cell_arr_from_pending_res_list
(
    son_cell_status_t *p_cell_sts_arr
);

/*! \fn son_void_t* son_key_of_anr (const SON_LIST_NODE *p_node)
 *  \brief This function determines the key in ANR Cell Context List
 *  \param p_node - anchor field of the Cell record
 */
const son_void_t *
son_key_of_anr
(
    const SON_LIST_NODE *p_node
);

/*! \fn const son_void_t * anr_key_of_txn_id_list (const SON_LIST_NODE *p_node)
 *  \brief This function determines the key in ANR transaction Id list
 *  \param p_node - anchor field of the transaction Id record
 */
const son_void_t *
anr_key_of_txn_id_list
(
    const SON_LIST_NODE *p_node
);

/*! \fn son_s32 anr_compare_txn_id(const son_void_t *p_val_1, const son_void_t *p_val_2)
 *  \brief This function compares the transaction Ids
 *  \param p_val_1      Pointer to the transaction Id as void pointer
 *  \param p_val_2      Pointer to the transaction Id as void pointer
 */
son_s32
anr_compare_txn_id
(
    const son_void_t *p_val_1,
    const son_void_t *p_val_2
);

/*! \fn const son_void_t * anr_key_of_nr_remove_list (const SON_LIST_NODE *p_node)
 *  \brief This function determines the key in NR Remove list
 *  \param p_node - anchor field of the nr remove list record
 */
const son_void_t *
anr_key_of_nr_remove_list
(
    const SON_LIST_NODE *p_node
);

/*! \fn son_s32 anr_compare_nr_remove_list_node(const son_void_t *p_val_1,
 *                                              const son_void_t *p_val_2)
 *  \brief This function compares the transaction Ids
 *  \param p_val_1  Pointer to the son_global_cell_id structure as void pointer
 *  \param p_val_2  Pointer to the son_global_cell_id structure as void pointer
 */
son_s32
anr_compare_nr_remove_list_node
(
    const son_void_t *p_val_1,
    const son_void_t *p_val_2
);

/*! \fn son_u32 anr_get_context_cell_count ()
 *  \brief This function gets number of cells configured
 */
son_u32
anr_get_context_cell_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_oam_log_on_off_et anr_get_log_mode ()
 *  \brief This function gets the  current log mode
 */
son_oam_log_on_off_et
anr_get_log_mode
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_bool_et son_get_x2_enabled ()
 *  \brief This function gets the current x2 enabled value
 */
son_bool_et
son_get_x2_enabled
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn SON_LIST * anr_get_cell_ctxt_list ()
 *  \brief This function gets the pointer to list of cells configured
 */
SON_LIST *
anr_get_cell_ctxt_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn anr_cell_context_t* anr_get_cell_context_from_global_context(son_intra_rat_global_cell_id_t *p_cell_id)
 *  \brief This function gets the cell conxtext of the specified cell
 *  \param p_cell_id - Pointer to the cell Id
 */
anr_cell_context_t *
anr_get_cell_context_from_global_context
(
    son_intra_rat_global_cell_id_t    *p_cell_id
);

/*SPR-9759-Start*/

/*! \fn anr_find_record_in_eutran_cdma_pci_value_list
 *  \brief This function finds the node for the given pci value
 *  \param p_list Pointer to the list of pci values of eutran or cdma cells
 *  \param pci pci value
 */
anr_eutran_cdma_pci_value_node_t *
anr_find_record_in_eutran_cdma_pci_value_list
(
 SON_LIST                     *p_list,
 /*SPR 18360 Fix Start*/
 /*Lines Deleted*/
 anr_eutran_cdma_key_t        *p_key
 /*SPR 18360 Fix Stop*/
 );

/*! \fn anr_key_of_eutran_cdma_pci_value_list
 *  \brief This function generates the key
 *  \param p_node Anchor to eutran or cdma pci value node
 */
const son_void_t *
anr_key_of_eutran_cdma_pci_value_list
(
 const SON_LIST_NODE *p_node
 );

/*! \fn anr_compare_eutran_cdma_pci_value_list_node
 *  \brief This function compares the key field(pci) of the pci value node in the list
 *  \param p_val_1 Pointer to the key field(pci)as void pointer
 *  \param p_val_2 Pointer to the key field(pci)as void pointer
 */
son_s32
anr_compare_eutran_cdma_pci_value_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 );

/*! \fn anr_find_record_in_ue_index_and_counter_list
 *  \brief This function finds the node for the given ue_index value
 *  \param p_list Pointer to the list of ue_index and counter values
 *  \param ue_index UE Index value 
 */
ue_index_and_counter_list_node_t *
anr_find_record_in_ue_index_and_counter_list
(
 SON_LIST                     *p_list,
 son_u16                       ue_index
 );

/*! \fn anr_key_of_ue_index_and_counter_list
 *  \brief This function generates the key
 *  \param p_node Anchor to ue_index and counter value node 
 */
const son_void_t *
anr_key_of_ue_index_and_counter_list
(
 const SON_LIST_NODE *p_node
 );

/*! \fn anr_compare_ue_index_and_counter_list_node
 *  \brief This function compares the key field(ue_index) of the ue_index and counter value node in the list
 *  \param p_val_1 Pointer to the key field(ue_index)as void pointer
 *  \param p_val_2 Pointer to the key field(ue_index)as void pointer
 */
son_s32
anr_compare_ue_index_and_counter_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 );

/*! \fn anr_delete_pci_from_eutran_cdma_pci_requested_cgi_list
 *  \brief This function deletes the pci node from eutran_pci_requested_cgi_list
 *         or cdma_pci_requested_cgi_list
 *  \param p_list Pointer to the list of pci values of eutran cells
 *  \param pci pci value
 */
son_void_t
anr_delete_pci_from_eutran_cdma_pci_requested_cgi_list
(
 SON_LIST                     *p_list,
 /*SPR 18360 Fix Start*/
 /*Lines Deleted*/
 anr_eutran_cdma_key_t        *p_key
 /*SPR 18360 Fix Stop*/
 );

/*! \fn anr_delete_ue_index_and_counter_list
 *  \brief This function delete the nodes of the ue_index_and_counter list
 *  \param p_list ue_index_and_counter list
 */
son_void_t
anr_delete_ue_index_and_counter_list
(
 SON_LIST  *p_list
  );

/*! \fn anr_find_record_in_utran_pci_value_list
 *  \brief This function finds the node for the given pci value
 *  \param p_list Pointer to the list of pci values of utran cells
 *  \param pci pci value
 */
anr_utran_pci_value_node_t *
anr_find_record_in_utran_pci_value_list
(
 SON_LIST                           *p_list,
 /*SPR 18360 Fix Start*/
 /*Lines Deleted*/
 anr_utran_key_t                     *p_key
 /*SPR 18360 Fix Stop*/
 );

/*! \fn anr_key_of_utran_pci_value_list
 *  \brief This function generates the key
 *  \param p_node Anchor to eutran or cdma pci value node
 */
const son_void_t *
anr_key_of_utran_pci_value_list
(
 const SON_LIST_NODE *p_node
 );

/*! \fn anr_compare_utran_pci_value_list_node
 *  \brief This function compares the key field(pci) of the pci value node in the list
 *  \param p_val_1 Pointer to the key field(pci)as void pointer
 *  \param p_val_2 Pointer to the key field(pci)as void pointer
 */
son_s32
anr_compare_utran_pci_value_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 );

/*! \fn anr_delete_pci_from_utran_pci_requested_cgi_list
 *  \brief This function deletes the pci node from utran_pci_requested_cgi_list
 *  \param p_list Pointer to the list of pci values of utran cells
 *  \param pci pci value
 */
son_void_t
anr_delete_pci_from_utran_pci_requested_cgi_list
(
 SON_LIST                           *p_list,
 rrm_son_utran_physical_cell_id_t    pci,
 /*SPR 18360 Fix Start*/
 son_u16                              uarfcn
 /*SPR 18360 Fix Stop*/
 );

/*! \fn anr_find_record_in_geran_pci_value_list
 *  \brief This function finds the node for the given pci value
 *  \param p_list Pointer to the list of pci values of geran cells
 *  \param pci pci value
 */
anr_geran_pci_value_node_t *
anr_find_record_in_geran_pci_value_list
(
 SON_LIST                           *p_list,
 /* Spr 18615 Changes Start */
 anr_geran_key_t                    *p_key
 /* Spr 18615 Changes End */
 );

/*! \fn anr_key_of_geran_pci_value_list
 *  \brief This function generates the key
 *  \param p_node Anchor to geran pci value node
 */
const son_void_t *
anr_key_of_geran_pci_value_list
(
 const SON_LIST_NODE *p_node
 );

/*! \fn anr_compare_geran_pci_value_list_node
 *  \brief This function compares the key field(pci) of the pci value node in the list
 *  \param p_val_1 Pointer to the key field(pci)as void pointer
 *  \param p_val_2 Pointer to the key field(pci)as void pointer
 */
son_s32
anr_compare_geran_pci_value_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 );

/*! \fn anr_delete_pci_from_geran_pci_requested_cgi_list
 *  \brief This function deletes the pci node from geran_pci_requested_cgi_list
 *  \param p_list Pointer to the list of pci values of geran cells
 *  \param pci pci value
 */
son_void_t
anr_delete_pci_from_geran_pci_requested_cgi_list
(
 SON_LIST                           *p_list,
 /* Spr 18615 Changes Start */
 anr_geran_key_t                    *p_key
 /* Spr 18615 Changes End */
 );

/*SPR-12596-Start*/

/*! \fn anr_insert_in_conflict_confusion_pci_list
 *  \brief This function is used to insert pci in conflict confusion pci list
 *  \param p_cell_ctxt Pointer to the cell context
 *  \param cgi_1 cgi of neighbor one
 *  \param cgi_2 cgi of neighbor two
 *  \param pci involved in conflict/confusion
 *  \param cause conflict/confusion
 */
son_void_t
anr_insert_in_conflict_confusion_pci_list
(
 anr_cell_context_t                   *p_cell_ctxt,
 son_intra_rat_global_cell_id_t        cgi_1,
 son_intra_rat_global_cell_id_t        cgi_2,
 son_u16                               pci,
 son_pci_cause_et                      cause
 );

/*! \fn anr_find_pci_in_conflict_confusion_pci_list
 *  \brief This function finds the node for the given pci value
 *  \param p_cell_ctxt Pointer to the cell context
 *  \param pci
 */
conflict_confusion_pci_list_node_t *
anr_find_pci_in_conflict_confusion_pci_list
(
 anr_cell_context_t            *p_cell_ctxt,
 son_u16                        pci
 );

/*! \fn anr_key_of_conflict_confusion_list
 *  \brief This function generates the key
 *  \param p_node 
 */
const son_void_t *
anr_key_of_conflict_confusion_list
(
 const SON_LIST_NODE *p_node
 );

/*! \fn anr_compare_pci_list_node
 *  \brief This function compares the key field(pci) of the pci node in the list
 *  \param p_val_1 Pointer to the key field(pci)as void pointer 
 *  \param p_val_2 Pointer to the key field(pci)as void pointer
 */
son_s32
anr_compare_pci_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
 );

/*SPR-12596-End*/


/*SPR-9759-End*/

/*SPR-9574-Start*/

/*SPR-11554-Start*/

/*Function for determining PCI range end value shifted to common area*/

/*SPR-11554-End*/

/*! \fn anr_find_freq_in_intra_freq_list
 *  \brief This function finds the node for the given earfcn value
 *  \param temp_earfcn Frequency
 */
anr_intra_freq_list_node_t *
anr_find_freq_in_intra_freq_list
(
 son_u32                       temp_earfcn
);

/*! \fn anr_key_of_intra_freq_list
 *  \brief This function generates the key
 *  \param p_node Anchor to Intra freq node
 */
const son_void_t *
anr_key_of_intra_freq_list
(
 const SON_LIST_NODE *p_node
);

/*! \fn anr_compare_intra_freq_list_node
 *  \brief This function compares the key field(frequency) of the frequency node in the list
 *  \param p_val_1 Pointer to the key field(frequency)as void pointer
 *  \param p_val_2 Pointer to the key field(frequency)as void pointer
 */
son_s32
anr_compare_intra_freq_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
);

/*! \fn anr_insert_in_inter_freq_list
 *  \brief This function is used to populate the inter frequency list in ANR global context
 *  \param p_in_config_list Pointer to the EUTRAN neighbor cells info list received from OAM
 *  \param list_size Size of EUTRAN neighbor cells info list received from OAM
 */
son_void_t
anr_insert_in_inter_freq_list
(
 const son_meas_config_eutran_t      *p_in_config_list,
 const son_u8                        list_size
);

/*! \fn anr_find_freq_in_inter_freq_list
 *  \brief This function finds the node for the given earfcn value
 *  \param temp_earfcn Frequency
 */
anr_inter_freq_list_node_t *
anr_find_freq_in_inter_freq_list
(
 son_u32                       temp_earfcn
);

/*! \fn anr_key_of_inter_freq_list
 *  \brief This function generates the key
 *  \param p_node Anchor to Inter freq node
 */
const son_void_t *
anr_key_of_inter_freq_list
(
 const SON_LIST_NODE *p_node
);

/*! \fn anr_compare_inter_freq_list_node
 *  \brief This function compares the key field(frequency) of the frequency node in the list
 *  \param p_val_1 Pointer to the key field(frequency)as void pointer
 *  \param p_val_2 Pointer to the key field(frequency)as void pointer
 */
son_s32
anr_compare_inter_freq_list_node
(
 const son_void_t *p_val_1,
 const son_void_t *p_val_2
);

/*SPR-9574-End*/

/*start:gur32802*/

/*! \fn  son_bool_et anr_delete_record_from_cdma_xrtt_stats_list(SON_LIST   *p_list,
 *                                                   son_cdma_band_class_et   band_class,
 *                                                   son_u16                  arfcn);
 *  \brief This function deletes the record from the CDMA freq/band statistics list
 *  \param p_list       Pointer to CDMA frequency list
 *  \param band_class   band class
 *  \param arfcn        ARFCN value
 */

son_bool_et
anr_delete_record_from_cdma_xrtt_stats_list
(
    SON_LIST                *p_list,
    son_cdma_band_class_et band_class,
    son_u16 arfcn
);

/*! \fn anr_cdma_xrtt_freq_stats_node_t* anr_find_record_in_cdma_xrtt_freq_stats_list(SON_LIST *p_list, son_u16 arfcn)
 *  \brief This function finds the frequency statistcs node for the given arfcn value
 *  \param p_list Pointer to frequency statistics List
 *  \param arfcn  ARFCN value
 */

anr_cdma_xrtt_freq_stats_node_t *
anr_find_record_in_cdma_xrtt_freq_stats_list
(
    SON_LIST                     *p_list,
    son_u16 arfcn
);

/*! \fn anr_cdma_xrtt_band_stats_node_t* anr_find_record_in_cdma_xrtt_band_stats_list(SON_LIST *p_list, son_cdma_band_class_et band)
 *  \brief This function finds the broadcast statistcs node for the given band class
 *  \param p_list Pointer to frequency statistics List
 *  \param band   CDMA2000 band class value
 */
anr_cdma_xrtt_band_stats_node_t *
anr_find_record_in_cdma_xrtt_band_stats_list
(
    SON_LIST                     *p_list,
    son_cdma_band_class_et band
);

/*! \fn anr_cdma_hrpd_freq_stats_node_t* anr_find_record_in_cdma_xrtt_band_stats_list(SON_LIST *p_list, son_u16 arfcn)
 *  \brief This function finds the frequency statistcs node for the given arfcn value
 *  \param p_list Pointer to frequency statistics List
 *  \param arfcn  CDMA2000 freq value
 */
anr_cdma_hrpd_freq_stats_node_t *
anr_find_record_in_cdma_hrpd_freq_stats_list
(
    SON_LIST                     *p_list,
    son_u16 arfcn
);

/*! \fn anr_cdma_hrpd_band_stats_node_t* anr_find_record_in_cdma_hrpd_band_stats_list(SON_LIST *p_list, son_cdma_band_class_et band)
 *  \brief This function finds the frequency statistcs node for the given arfcn value
 *  \param p_list Pointer to frequency statistics List
 *  \param band   CDMA2000 band class value
 */
anr_cdma_hrpd_band_stats_node_t *
anr_find_record_in_cdma_hrpd_band_stats_list
(
    SON_LIST                     *p_list,
    son_cdma_band_class_et band
);

/*! \fn anr_cdma_xrtt_band_stats_node_t* anr_insert_record_in_cdma_xrtt_band_stats_list(anr_cell_context_t *p_cell_ctxt,
 *                                                                                      son_cdma_neighbor_info_t *p_nbr_info)
 *  \brief This function inserts a CDMA statistics record in the cdma band stats list
 *  \param p_cell_ctxt Pointer to the Cell context
 *  \param p_nbr_info Pointer to CDMA neighbor info
 */
anr_cdma_xrtt_band_stats_node_t *
anr_insert_record_in_cdma_xrtt_band_stats_list
(
    anr_cell_context_t             *p_cell_ctxt,
    son_cdma_neighbor_info_t       *p_nbr_info
);

/*! \fn anr_cdma_hrpd_band_stats_node_t* anr_insert_record_in_cdma_hrpd_band_stats_list(anr_cell_context_t *p_cell_ctxt,
 *                                                                                      son_cdma_neighbor_info_t *p_nbr_info)
 *  \brief This function inserts a CDMA statistics record in the cdma band stats list
 *  \param p_cell_ctxt Pointer to the Cell context
 *  \param p_nbr_info Pointer to CDMA neighbor info
 */
anr_cdma_hrpd_band_stats_node_t *
anr_insert_record_in_cdma_hrpd_band_stats_list
(
    anr_cell_context_t         *p_cell_ctxt,
    son_cdma_neighbor_info_t   *p_nbr_info
);

/*! \fn son_bool_et increment_cdma_xrtt_nbr_count_for_rrm(son_cdma_neighbor_info_t   *p_nbr_info,
 *                                                        anr_cell_context_t         *p_cell_ctxt)
 *  \brief This function increments the CDMA XRTT neighbor count
 *  \param p_nbr_info  Pointer to CDMA neighbor information
 *  \param p_cell_ctxt Pointer to Serving Cell context
 */
son_bool_et
increment_cdma_xrtt_nbr_count_for_rrm
(
    son_cdma_neighbor_info_t   *p_nbr_info,
    anr_cell_context_t         *p_cell_ctxt
);

/*! \fn son_bool_et increment_cdma_hrpd_nbr_count_for_rrm(son_cdma_neighbor_info_t   *p_nbr_info,
 *                                                        anr_cell_context_t         *p_cell_ctxt)
 *  \brief This function increments the CDMA HRPD neighbor count
 *  \param p_nbr_info  Pointer to CDMA neighbor information
 *  \param p_cell_ctxt Pointer to Serving Cell context
 */
son_bool_et
increment_cdma_hrpd_nbr_count_for_rrm
(
    son_cdma_neighbor_info_t   *p_nbr_info,
    anr_cell_context_t         *p_cell_ctxt
);

/*! \fn son_bool_et anr_decrement_nbr_count_xrtt(SON_LIST *p_list,
 *                                               son_cdma_band_class_et band,
 *                                               son_u16 arfcn,
 *                                               son_bool_et    broadcast_status,
 *                                               son_ho_status_et ho_status);
 *  \brief This function decrements the CDMA XRTT neighbor count
 *  \param p_list              - Pointer to SON LIST HASH Table
 *  \param band                - CDMA2000 band class
 *  \param arfcn               - CDMA2000 frequency value
 *  \param broadcast_status    - CDMA broadcast status
 *  \param ho_status           - Handover status for CDMA2000 xrtt NBR
 */
son_bool_et
anr_decrement_nbr_count_xrtt
(
    SON_LIST                     *p_list,
    son_cdma_band_class_et band,
    son_u16 arfcn,
    son_bool_et broadcast_status,
    son_ho_status_et ho_status
);

/*! \fn son_bool_et decrement_cdma_xrtt_nbr_count_for_rrm( son_cdma_neighbor_info_t *p_nbr_info,
 *                                                          anr_cell_context_t *p_cell_ctxt)
 *  \brief This function decrements the CDMA XRTT neighbor count
 *  \param p_nbr_info       - CDMA neighbour information
 *  \param p_cell_ctxt - Pointer to serving cell context
 */
son_bool_et
decrement_cdma_xrtt_nbr_count_for_rrm
(
    son_cdma_neighbor_info_t   *p_nbr_info,
    anr_cell_context_t         *p_cell_ctxt
);

/*! \fn son_bool_et anr_decrement_nbr_count_hrpd(SON_LIST *p_list,
 *                                               son_cdma_band_class_et band,
 *                                               son_u16 arfcn,
 *                                               son_bool_et    broadcast_status,
 *                                               son_ho_status_et ho_status);
 *  \brief This function decrements the CDMA HRPD neighbor count
 *  \param p_list              - Pointer to SON LIST HASH Table
 *  \param band                - CDMA2000 band class
 *  \param arfcn               - CDMA2000 frequency value
 *  \param broadcast_status    - CDMA broadcast status
 *  \param ho_status           - Handover status for CDMA2000 xrtt NBR
 */
son_bool_et
anr_decrement_nbr_count_hrpd
(
    SON_LIST                     *p_list,
    son_cdma_band_class_et band,
    son_u16 arfcn,
    son_bool_et broadcast_status,
    son_ho_status_et ho_status
);

/*! \fn son_bool_et decrement_cdma_hrpd_nbr_count_for_rrm( son_cdma_neighbor_info_t *p_nbr_info,
 *                                                          anr_cell_context_t *p_cell_ctxt)
 *  \brief This function decrements the CDMA HRPD neighbor count
 *  \param p_nbr_info       - CDMA neighbour information
 *  \param p_cell_ctxt - Pointer to serving cell context
 */
son_bool_et
decrement_cdma_hrpd_nbr_count_for_rrm
(
    son_cdma_neighbor_info_t   *p_nbr_info,
    anr_cell_context_t         *p_cell_ctxt
);

/*! \fn son_bool_et anr_delete_record_from_cdma_hrpd_stats_list( SON_LIST *p_list,
 *                                                               son_cdma_band_class_et band_class,
 *                                                               son_u16 arfcn)
 *  \brief This function deletes the record from the CDMA freq/band statistics list
 *  \param p_list      - Pointer to CDMA frequency list
 *  \param band_class  - CDMA2000 Band Class value
 *  \param arfcn       - CDMA2000 frequency value
 */
son_bool_et
anr_delete_record_from_cdma_hrpd_stats_list
(
    SON_LIST                *p_list,
    son_cdma_band_class_et band_class,
    son_u16 arfcn
);

/*! \fn son_void_t anr_clear_cdma_xrtt_band_stats_list( SON_LIST *p_list)
 *  \brief This function clears the entire cdma xrtt band statistics list for a  configured cell
 *  \param p_list      - Pointer to CDMA frequency list
 */
son_void_t
anr_clear_cdma_xrtt_band_stats_list
(
    SON_LIST *p_list
);

/*! \fn son_void_t anr_clear_cdma_hrpd_band_stats_list( SON_LIST *p_list)
 *  \brief This function clears the entire cdma hrpd band statistics list for a  configured cell
 *  \param p_list      - Pointer to CDMA frequency list
 */
son_void_t
anr_clear_cdma_hrpd_band_stats_list
(
    SON_LIST *p_list
);

/*SPR-9759-Start*/

/*! \fn son_void_t anr_clear_eutran_cdma_pci_requested_cgi_list( SON_LIST *p_list)
 *  \brief This function clears the entire list of pci's for which cgi has been requested
 *  \param p_list      - Pointer to eutran_cdma_pci_requested_cgi_list
 */
son_void_t
anr_clear_eutran_cdma_pci_requested_cgi_list
(
 SON_LIST *p_list
 );

/*! \fn son_void_t anr_clear_utran_pci_requested_cgi_list( SON_LIST *p_list)
 *  \brief This function clears the entire list of pci's for which cgi has been requested
 *  \param p_list      - Pointer to utran_pci_requested_cgi_list
 */
son_void_t
anr_clear_utran_pci_requested_cgi_list
(
 SON_LIST *p_list
 );

/*! \fn son_void_t anr_clear_geran_pci_requested_cgi_list( SON_LIST *p_list)
 *  \brief This function clears the entire list of pci's for which cgi has been requested
 *  \param p_list      - Pointer to geran_pci_requested_cgi_list
 */
son_void_t
anr_clear_geran_pci_requested_cgi_list
(
 SON_LIST *p_list
 );

/*SPR-9759-End*/

/*SPR-12596-Start*/

/*! \fn anr_clear_conflict_confusion_pci_list( SON_LIST *p_list)
 *  \brief This function clears the entire list of pci's involved in conflict confusion
 *  \param p_list      - Pointer to the pci list
 */
son_void_t
anr_clear_conflict_confusion_pci_list
(
 SON_LIST *p_list
 );

/*SPR-12596-End*/

/*! \fn son_void_t anr_print_cdma_cell_id(son_cdma_global_cell_id_t *p_nbr)
 *  \brief Prints CDMA2000 NBR's ECGI and other details
 *  \param p_nbr       - Pointer to XRTT/HRPD neighbour cell identity
 */
son_void_t
anr_print_cdma_cell_id
(
    son_cdma_global_cell_id_t *p_nbr
);

/*! \fn anr_fsm_state_et anr_get_context_state ()
 *  \brief This function gets the ANR context state
 */
anr_fsm_state_et
anr_get_context_state
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t anr_set_context_state (anr_fsm_state_et state)
 *  \brief This function sets the context state
 *  \param state       context state
 */
son_void_t
anr_set_context_state
(
    anr_fsm_state_et state
);

/*! \fn son_void_t anr_set_log_mode ( son_oam_log_on_off_et  log_mode)
 *  \brief This function sets the log mode
 *  \param log_mode    log mode value
 */
son_void_t
anr_set_log_mode
(
    son_oam_log_on_off_et log_mode
);

/*! \fn son_void_t anr_set_x2_enabled ( son_bool_et  x2_enabled)
 *  \brief This function sets the x2 enable/disable
 *  \param x2_enabled    x2 enable/disable
 */
son_void_t
anr_set_x2_enabled
(
    son_bool_et x2_enabled
);

/*! \fn anr_cell_context_node_t * anr_add_new_cell_in_context(son_cell_t *p_cell)
 *  \brief This function adds new cell in the cell context
 *  \param p_cell       Pointer to cell information structure
 */
anr_cell_context_node_t *
anr_add_new_cell_in_context
(
    /*SPR_19279_START*/
    son_cell_t *p_cell,
    son_u8 cell_index
    /*SPR_19279_END*/
);

/*SPR-9574-Start*/

/*! \fn son_void_t anr_clear_intra_and_inter_freq_list
 *  \brief This function delete all the frequencies from Inter and Intra freq lists
 */
son_void_t
anr_clear_intra_and_inter_freq_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*SPR-9574-End*/

/*! \fn son_void_t anr_clear_context_list ()
 *  \brief This function deletes the cell contexts
 */
son_void_t
anr_clear_context_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_bool_et anr_delete_cell_from_context (son_intra_rat_global_cell_id_t *p_cell_id)
 *  \brief This function deletes the specified cell context from the cell context list
 *  \param p_cell_id       Pointer to the cell id
 */
son_bool_et
anr_delete_cell_from_context
(
    son_intra_rat_global_cell_id_t    *p_cell_id
);

/*! \fn anr_cell_context_node_t *anr_get_first_cell_ctxt ()
 *  \brief This function gets the cell context of the first cell in the context list
 */
anr_cell_context_node_t *
anr_get_first_cell_ctxt
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn anr_cell_context_node_t * anr_get_next_node(anr_cell_context_node_t *p_node)
 *  \brief This function gets the  next cell context in the list
 *  \param p_node   Pointer to the current cell context
 */
anr_cell_context_node_t *
anr_get_next_node
(
    anr_cell_context_node_t *p_node
);

/*! \fn son_app_mode_et anr_get_global_mode()
 *  \brief This function gets the anr mode
 */
son_app_mode_et
anr_get_global_mode
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_u32 anr_get_pending_res_cell_count()
 *  \brief This function gets number of cells available in pending response
 */
son_u32
anr_get_pending_res_cell_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t anr_set_global_mode( son_app_mode_et mode )
 *  \brief This function sets anr mode
 *  \param mode  Mode value
 */
son_void_t
anr_set_global_mode
(
    son_app_mode_et mode
);

/*! \fn anr_hash_pool_t* anr_get_hash_node_pool()
 * \brief This function returns hash table pool
 */
anr_hash_pool_t *
anr_get_hash_node_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn anr_hash_pool_t *anr_get_intra_rat_hash_rec_pool()
 * \brief This function returns hash table context pool
 */

anr_hash_pool_t *
anr_get_intra_rat_hash_rec_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn anr_hash_pool_t *anr_get_inter_rat_hash_rec_pool()
 * \brief This function returns inter RAT hash table context pool
 */
anr_hash_pool_t *
anr_get_inter_rat_hash_rec_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t anr_print_cell_context(anr_cell_context_t *p_ctxt)
 *  \brief This function prints the cell context
 *  \param p_ctxt Pointer to cell context
 */
son_void_t
anr_print_cell_context
(
    anr_cell_context_t *p_ctxt
);

/*! \fn      son_pci_mode_et anr_get_pci_module_mode()
 *  \brief   This function returns the  pci mode
 */
son_pci_mode_et
anr_get_pci_module_mode
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn   son_void_t anr_set_pci_module_mode(son_pci_mode_et pci_mode );
 *  \brief This function sets the  pci mode
 *  \param pci_mode  Mode value to be set
 */
son_void_t
anr_set_pci_module_mode
(
    son_pci_mode_et pci_mode
);

/*! \fn son_return_et anr_check_fsm_state(anr_cell_fsm_state_et cell_state)
 *  \brief This function checks the fsm state of all cells in cell context
 *  \param cell_state  Expected ANR cell state
 */
son_return_et
anr_check_fsm_state
(
    anr_cell_fsm_state_et cell_state
);

/*! \fn son_void_t anr_clear_txn_id_list(SON_LIST *p_list)
 *  \brief This function clears the entire transaction Id list for a configured
 * cell
 *  \param p_list   Pointer to the transaction Id list
 */
son_void_t
anr_clear_txn_id_list
(
    SON_LIST *p_list
);

/*! \fn son_u8 anr_get_pending_scan_res_count
 *  \brief This function returns the value of pending nmm scan response count
 */
son_u8
anr_get_pending_scan_res_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_u8 anr_increment_pending_scan_res_count
 *  \brief This function increments the value of pending nmm scan response count
 */
son_void_t
anr_increment_pending_scan_res_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_u8 anr_decrement_pending_scan_res_count
 *  \brief This function decrements the value of pending nmm scan response count
 */
son_void_t
anr_decrement_pending_scan_res_count
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_error_et anr_get_pending_scan_res_error_code
 *  \brief This function Gets Pending NMM Scan Response Error Code in anr_global_context
 */
son_error_et
anr_get_pending_scan_res_error_code
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t anr_set_pending_scan_res_error_code(son_error_et err)
 *  \brief This function Sets Pending NMM Scan Response Error Code in anr_global_context
 */
son_void_t
anr_set_pending_scan_res_error_code
(
    son_error_et err
);

/*! \fn son_void_t anr_pending_res_timer_handler
 *  \brief This function cleans up pending response structure and sends response to OAM
 */
son_void_t
anr_pending_res_timer_handler
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn SON_LIST* anr_get_x2_pending_res_list
 *  \brief This function return X2 Pending response LIST
 */
SON_LIST *
anr_get_x2_pending_res_list
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t anr_delete_from_txn_id_list(SON_LIST *p_list,son_u16 tid)
 *  \brief This function deletes a tid from txn_id_list
 *  \param p_list : pointer to list
 * \param tid : Transaction ID
 */
son_void_t
anr_delete_from_txn_id_list
(
    SON_LIST *p_list,
    son_u16 tid
);

/*! \fn anr_txn_id_node_t* anr_find_txn_id_list(SON_LIST *p_list,son_u16 tid)
 *  \brief This function finds a tid in txn_id_list
 *  \param p_list  : Cell specfic NR Remove List
 *  \param tid : Transaction ID
 */
anr_txn_id_node_t *
anr_find_txn_id_list
(
    SON_LIST *p_list,
    son_u16 tid
);

/*! \fn son_bool_et anr_insert_in_txn_id_list(SON_LIST *p_list,son_u16 tid)
 *  \brief This function adds a tid from txn_id_list
 *  \param tid : Transaction ID
 *  \param p_list pointer to list
 */
son_bool_et
anr_insert_in_txn_id_list
(
    SON_LIST *p_list,
    son_u16 tid
);

/*! \fn son_return_et anr_check_cell_state(rrm_cell_operational_state_et cell_state)
 *  \brief This function checks the operational state of all cells in cell context
 *  \param cell_state  Expected OPerational cell state
 */
son_return_et
anr_check_cell_state
(
    rrm_cell_operational_state_et cell_state
);

/*! \fn anr_nr_remove_list_node_t* anr_find_cell_in_remove_list(
 *                                  SON_LIST *p_list, son_global_cell_id_t *p_cell_id)
 *  \brief This function finds a cell in nr remove list
 *  \param p_list  : Cell specfic NR Remove List
 *  \param p_cell_id : SON Global cell id
 */
anr_nr_remove_list_node_t *
anr_find_cell_in_remove_list
(
    SON_LIST                *p_list,
    son_global_cell_id_t    *p_cell_id
);

/*! \fn son_bool_et anr_insert_cell_in_remove_list(
 *                                  SON_LIST *p_list, son_global_cell_id_t*)
 *  \brief This function inserts a cell in nr remove list
 *  \param p_list  : Cell specfic NR Remove List
 *  \param p_cell_id : SON Global cell id
 */
son_bool_et
anr_insert_cell_in_remove_list
(
    SON_LIST                *p_list,
    son_global_cell_id_t    *p_cell_id
);

/*! \fn son_bool_et anr_delete_cell_from_remove_list(
 *                                  SON_LIST *p_list,, son_global_cell_id_t*)
 *  \brief This function deletes a cell from nr remove list
 *  \param p_list  : Cell specfic NR Remove List
 *  \param p_cell_id : SON Global cell id
 */
son_bool_et
anr_delete_cell_from_remove_list
(
    SON_LIST                *p_list,
    son_global_cell_id_t    *p_cell_id
);

/*! \fn son_void_t anr_stop_timer()
 *  \brief This function stops ANR Timer
 */
son_void_t
anr_stop_timer
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_u16 anr_get_earfcn(anr_cell_context_t *p_cell_ctxt)
 *  \brief  This function returns EARFCN
 *  \param  p_cell_ctxt - Pointer to cell context
 */
son_u32
anr_get_earfcn
(
    anr_cell_context_t *p_cell_ctxt
);


/*! \fn son_u16 anr_get_ul_earfcn(anr_cell_context_t *p_cell_ctxt)
 *  \brief  This function returns EARFCN
 *  \param  p_cell_ctxt - Pointer to cell context
 */
son_u32
anr_get_ul_earfcn
(
    anr_cell_context_t *p_cell_ctxt
);

/*! \fn son_u8 anr_get_duplex_mode_of_serving_cell(
                    anr_cell_context_t *p_cell_ctxt)
 *  \brief  This function returns duplex mode of the serving cell
 *  \param  p_cell_ctxt - Pointer to cell context
 */
son_duplex_mode_et
anr_get_duplex_mode_of_serving_cell
(
    anr_cell_context_t *p_cell_ctxt
);

/*! \fn anr_hash_pool_t* anr_get_pci_conflict_hash_node_pool()
 * \brief This function returns hash table pool
 */
anr_hash_pool_t *
anr_get_pci_conflict_hash_node_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn anr_hash_pool_t* anr_get_pci_conflict_hash_rec_pool()
 * \brief This function returns hash table pool
 */
anr_hash_pool_t *
anr_get_pci_conflict_hash_rec_pool
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

SON_HASH *
anr_get_pci_conflict_detection_table
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

son_bool_et
is_cgi_matches_serving_cell
(
    son_intra_rat_global_cell_id_t *p_cell
);

/*! \fn anr_cell_fsm_state_et anr_get_anr_mode_for_cell(
 *  anr_cell_context_t *p_cell_ctxt)
 * \brief This function returns the ANR mode for the given cell.
 * \param p_cell_ctxt   Pointer to ANR cell context
 */
anr_cell_fsm_state_et
anr_get_anr_mode_for_cell
(
    anr_cell_context_t *p_cell_ctxt
);

/*! \fn const son_void_t * anr_key_of_bs_allowed_stats_list (
 *  const SON_LIST_NODE *p_node)
 * \brief This function determines the key field(earfcn) of the broadcast statis    tics node in the list.
 * \param p_node   anchor field of the broadcast statistics list node
 */
const son_void_t *
anr_key_of_bs_allowed_stats_list
(
    const SON_LIST_NODE *p_node
);

/*! \fn son_s32  anr_compare_bs_allowed_stats_list_node (
 *  const son_void_t *p_val_1,const son_void_t *p_val_2)
 * \brief This function compares the key field(earfcn) of the broadcast statis    tics node in the list.
 * \param p_val_1 Pointer to the key field(earfcn)  as void pointer
 * \param p_val_2 Pointer to the key field(earfcn)  as void pointer
 */
son_s32
anr_compare_bs_allowed_stats_list_node
(
    const son_void_t *p_val_1,
    const son_void_t *p_val_2
);

/*! \fn anr_bs_allowed_node_t* anr_find_record_in_bs_allowed_stats_list (
 *                                      SON_LIST *p_list,son_u32  earfcn)
 * \brief This function finds the broadcast statistcs node for the
 *        given earfcn value
 * \param p_list  Pointer to broadcast allowed neighbor statistics List
 * \param earfcn  EARFCN value
 */
anr_bs_allowed_node_t *
anr_find_record_in_bs_allowed_stats_list
(
    SON_LIST                *p_list,
    son_u32 earfcn
);

/*! \fn  anr_bs_allowed_node_t* anr_insert_record_in_bs_allowed_stats_list(
 *        SON_LIST   *p_list,son_u32  earfcn,son_bool_et broadcast_status,
 *        son_bool_et is_ho_allowd_nbr,son_bool_et is_ho_blklst_nbr)
 * \brief This function inserts a Broadcast statistics record in the
 *        List
 * \param p_list  :Pointer to broadcast allowed neighbor statistics List
 * \param earfcn  :EARFCN value
 * \param broadcast_status : Bool value. Is neighbor broadcastable or not
 * \param is_ho_allowd_nbr : Bool value. Is  HO allowed to be
 *                           incremented.
 * \param is_ho_blklst_nbr : Bool value. Is  HO blacklisted to be
 *                           incremented.
 */

anr_bs_allowed_node_t *
anr_insert_record_in_bs_allowed_stats_list
(
    SON_LIST                *p_list,
    son_u32 earfcn,
    son_bool_et broadcast_status,
    son_bool_et is_ho_allowd_nbr,
    son_bool_et is_ho_blklst_nbr
);

/*! \fn son_bool_et anr_delete_record_from_bs_allowed_stats_list(
 *         SON_LIST   *p_list,son_u32  earfcn)
 * \brief  This function deletes the record from the broadcast allowed
 *         neighbor count list
 * \param p_list  Pointer to broadcast allowed neighbor statistics List
 * \param earfcn  EARFCN value
 */
son_bool_et
anr_delete_record_from_bs_allowed_stats_list
(
    SON_LIST                *p_list,
    son_u32  earfcn
);

/*! \fn son_void_t anr_clear_bs_allowed_stats_list(SON_LIST *p_list)
 * \brief  This function clears the entire broadcast statistics list for a
 *         configured cell
 * \param p_list  Pointer to broadcast statistics List
 */
son_void_t
anr_clear_bs_allowed_stats_list
(
    SON_LIST *p_list
);

/*! \fn son_bool_et increment_bs_allowed_nbr_count(
 *      anr_cell_context_t *p_cell_ctxt,anr_bs_allowed_node_t *p_list_node,
 *       son_bool_et broadcast_status,son_bool_et is_ho_allowed,son_bool_et is_ho_blklsted)
 * \brief  This function increments the count of neighbor in  broadcast
 *         Statistics Record
 * \param  p_cell_ctxt - Pointer to serving cell context
 * \param  p_list_node - Pointer to the broadcast statistics node
 * \param  broadcast_status - Bool value.Is neighbor broadcastable or not
 * \param  is_ho_allowed - Bool value. Is HO Allowed count to be incremented
 * \param  is_ho_blklsted - Bool value.Is HO blacklisted count to be incremented
 */
son_bool_et
increment_bs_allowed_nbr_count
(
    anr_cell_context_t    *p_cell_ctxt,
    anr_bs_allowed_node_t *p_list_node,
    son_bool_et broadcast_status,
    son_bool_et is_ho_allowed,
    son_bool_et is_ho_blklsted
);

/*! \fn son_bool_et decrement_bs_allowed_nbr_count(
 *      anr_cell_context_t *p_cell_ctxt,anr_bs_allowed_node_t *p_list_node,
 *      son_bool_et broadcast_status,son_bool_et is_ho_allowed,son_bool_et is_ho_blklisted)
 * \brief  This function decrements the count of neighbor in  broadcast
 *         Statistics Record
 * \param  p_cell_ctxt - Pointer to serving cell context
 * \param  p_list_node - Pointer to the broadcast statistics node
 * \param  broadcast_status - Bool value.Is neighbor broadcastable or not
 * \param  is_ho_allowed - Bool value. Is HO Allowed count to be incremented
 * \param  is_ho_blklisted - Bool value. Is HO blacklisted count to be incremented
 */
son_bool_et
decrement_bs_allowed_nbr_count
(
    anr_cell_context_t    *p_cell_ctxt,
    anr_bs_allowed_node_t *p_list_node,
    son_bool_et broadcast_status,
    son_bool_et is_ho_allowed,
    son_bool_et is_ho_blklisted
);

/*PCI CONFUSION_SUSPECTED CHANGES:START*/
son_void_t
anr_print_pci_ctxt_from_sus_pci_ctxt_list
(
    anr_pci_sus_conf_node_t   *p_list_node
);

const son_void_t *
anr_key_of_sus_pci_list
(
    const SON_LIST_NODE *p_node
);

son_s32
anr_compare_sus_pci_ctxt_list_node
(
    const son_void_t *p_val_1,
    const son_void_t *p_val_2
);
anr_pci_sus_conf_node_t *
anr_insert_in_sus_pci_ctxt_list
(
    anr_cell_context_t *p_cell_ctxt,
    son_u16 pci,
    son_u32 earfcn,
    son_pci_state_et state,
    son_u32 nbr_cell_count
);

anr_pci_sus_conf_node_t *
anr_find_in_sus_pci_ctxt_list
(
    anr_cell_context_t *p_cell_ctxt,
    son_u16 pci,
    son_u32 earfcn
);

son_bool_et
anr_delete_pci_ctxt_from_sus_pci_ctxt_list
(
    anr_cell_context_t *p_cell_ctxt,
    son_u16 pci,
    son_u32 earfcn
);

son_bool_et
anr_reset_sus_pci_count
(
    anr_cell_context_t        *p_cell_ctxt,
    anr_pci_sus_conf_node_t   *p_sus_pci_ctxt_node
);

/*PCI CONFUSION_SUSPECTED CHANGES:STOP*/

/*UTRAN-GERAN STATS MGMT STARTS*/
const son_void_t *
anr_key_of_utran_freq_stats_list
(
    const SON_LIST_NODE *p_node
);

son_s32
anr_compare_utran_freq_stats_list_node
(
    const son_void_t *p_val_1,
    const son_void_t *p_val_2
);

anr_utran_freq_stats_node_t *
anr_find_record_in_utran_freq_stats_list
(
    SON_LIST                *p_list,
    son_u16 uarfcn
);

anr_utran_freq_stats_node_t *
anr_insert_record_in_utran_freq_stats_list
(
    SON_LIST                *p_list,
    son_u16 uarfcn
);

son_bool_et
anr_delete_record_from_utran_freq_stats_list
(
    SON_LIST                *p_list,
    son_u16 uarfcn
);

son_bool_et
increment_utran_nbr_count_for_rrm
(
    anr_cell_context_t         *p_cell_ctxt,
    anr_utran_freq_stats_node_t *p_list_node
);

son_bool_et
decrement_utran_nbr_count_for_rrm
(
    anr_cell_context_t    *p_cell_ctxt,
    anr_utran_freq_stats_node_t *p_list_node,
    SON_LIST *p_list
);

/*GERAN*/
const son_void_t *
anr_key_of_geran_freq_stats_list
(
    const SON_LIST_NODE *p_node
);

son_s32
anr_compare_geran_freq_stats_list_node
(
    const son_void_t *p_val_1,
    const son_void_t *p_val_2
);

anr_geran_freq_stats_node_t *
anr_find_record_in_geran_freq_stats_list
(
    SON_LIST                     *p_list,
    anr_geran_freq_stats_data_t  *p_data
);

anr_geran_freq_stats_node_t *
anr_insert_record_in_geran_freq_stats_list
(
    SON_LIST                *p_list,
    son_u16 arfcn,
    son_geran_band_indicator_et band_ind
);

son_bool_et
anr_delete_record_from_geran_freq_stats_list
(
    SON_LIST                *p_list,
    son_u16 arfcn,
    son_geran_band_indicator_et band_ind
);

son_bool_et
increment_geran_nbr_count_for_rrm
(
    anr_cell_context_t         *p_cell_ctxt,
    anr_geran_freq_stats_node_t *p_list_node
);

son_bool_et
decrement_geran_nbr_count_for_rrm
(
    anr_cell_context_t    *p_cell_ctxt,
    anr_geran_freq_stats_node_t *p_list_node
);

/*! \fn son_void_t anr_clear_utran_freq_stats_list(SON_LIST *p_list)
 * \brief  This function clears the Utran freq statistics list for a
 *         configured cell
 * \param p_list  Pointer to utran freq statistics List
 */
son_void_t
anr_clear_utran_freq_stats_list
(
    SON_LIST *p_list
);

/*! \fn son_void_t anr_clear_geran_freq_stats_list(SON_LIST *p_list)
 * \brief  This function clears the entire geran freq statistics list for a
 *         configured cell
 * \param p_list  Pointer to geran freq statistics List
 */
son_void_t
anr_clear_geran_freq_stats_list
(
    SON_LIST *p_list
);

/*! \fn son_void_t anr_clear_pci_confl_conf_list(SON_LIST *p_list)
 * \brief  This function clears the pci conflict confusion list for a
 *         configured cell
 * \param p_list  Pointer to geran freq statistics List
 */
son_void_t
anr_clear_pci_confl_conf_list
(
    SON_LIST *p_list
);

/*UTRAN-GERAN STATS MGMT STOPS*/

/*! \fn son_return_et anr_validate_cell_info_ind_rach_data(son_cell_t *p_cell_data)
 * \brief This function validates the RACH data received in SMIF_CELL_INFO_IND message received from SONMgmtIfH. 
 * \param p_cell_data Pointer to Serving Cell data received in SMIF_CELL_INFO_IND
 */
son_return_et
anr_validate_cell_info_ind_rach_data
(
    son_cell_t *p_cell_data
);

/*! \  anr_rnc_id_node_t
 *  \brief ANR RNC ID list node
 *  \param rnc_list_node        CSPL's Node
 *  \param rnc_id               RNC id (Extended/Non-Extended)
 */
typedef struct _anr_rnc_id_node_t
{
    SON_LIST_NODE   rnc_list_node;
    son_u8          rnc_id[SON_RNC_ID_SIZE];
}anr_rnc_id_node_t;

/*! \fn son_void_t anr_clear_rnc_id_list(SON_LIST *p_list)
 *  \brief This function clears the entire RNC Id list for a configured
 *         cell
 *  \param p_list   Pointer to the RNC Id list (Extended/Non-Extended)
 */
son_void_t
anr_clear_rnc_id_list
(
    SON_LIST    *p_list
);

/*! \fn son_bool_et anr_find_rnc_id_from_the_list(SON_LIST *p_list, son_u8 *p_rnc_id)
 *  \brief This function finds a RNC Id in RNC Id list
 *  \param p_list       : Pointer to RNC Id list (Extended/Non-Extended)
 *  \param p_rnc_id     : Pointer to RNC ID
 */
son_bool_et
anr_find_rnc_id_from_the_list
(
    SON_LIST    *p_list,
    son_u8      *p_rnc_id
);

/*! \fn son_bool_et anr_insert_in_rnc_id_list(SON_LIST *p_list, son_u8 *p_rnc_id)
 *  \brief This function adds a RNC Id in RNC Id list (Extended/Non-Extended)
 *  \param p_list   : Pointer to RNC Id list (Extended/Non-Extended)
 *  \param p_rnc_id : Pointer to RNC ID
 */
son_bool_et
anr_insert_in_rnc_id_list
(
    SON_LIST *p_list,
    son_u8   *p_rnc_id
);

/*! \fn const son_void_t * anr_key_of_rnc_id_list (const SON_LIST_NODE *p_node)
 *  \brief This function determines the key in ANR RNC Id list
 *  \param p_node - anchor field of the RNC Id record
 */
const son_void_t *
anr_key_of_rnc_id_list
(
    const SON_LIST_NODE *p_node
);

/*! \fn son_s32 anr_compare_rnc_id(const son_void_t *p_val_1, const son_void_t *p_val_2)
 *  \brief This function compares the RNC Ids
 *  \param p_val_1      Pointer to the RNC Id as void pointer
 *  \param p_val_2      Pointer to the RNC Id as void pointer
 */
son_s32
anr_compare_rnc_id
(
    const son_void_t *p_val_1,
    const son_void_t *p_val_2
);

/*! \fn const son_u8 anr_get_rnc_indicator_from_cell_id (son_u8  *p_cell_id)
 *  \brief This function returns the RNC Id indicator from UTRAN Cell Id
 *  \param p_cell_id - Pointer to UTRAN Cell Id
 */
son_u8
anr_get_rnc_indicator_from_cell_id
(
 son_u8  *p_cell_id
 );

/*! \fn const son_u8 anr_get_rnc_indicator_from_cell_id (son_u8  *p_cell_id)
 *  \brief This function gets the Non-Extended RNC Id from UTRAN Cell Id
 *  \param p_cell_id - Pointer to UTRAN Cell Id
 */
son_void_t
anr_get_non_extended_rnc_id_from_cell_id
(
    son_u8  *p_cell_id,
    son_u8  *p_rnc_id
 );

/*! \fn const son_u8 anr_get_extended_rnc_id_from_cell_id (son_u8  *p_cell_id)
 *  \brief This function gets the Extended RNC Id from UTRAN Cell Id
 *  \param p_cell_id - Pointer to UTRAN Cell Id
 */
son_void_t
anr_get_extended_rnc_id_from_cell_id
(
    son_u8  *p_cell_id,
    son_u8  *p_rnc_id
 );

/*! \ anr_geran_meas_obj_remove_data_t
 *  \brief Record for maintaining GERAN nbrs info for measurement object delete request.
 *  \param arfcn                      ARFCN
 *  \param band_ind                   band indicator
 */
typedef struct _anr_geran_meas_obj_remove_data_t
{
    son_u16 arfcn;
    son_geran_band_indicator_et band_ind;
}anr_geran_meas_obj_remove_data_t;

/* Rel 3.0 Multi Sector code changes Start */
/*! \fn son_global_enb_id_t anr_get_own_enb_id()
 *  \brief This function gets the own Global eNB Identifier
 */ 
son_global_enb_id_t
anr_get_own_enb_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*SPR-11554-Start*/

/*Function removed*/

/*SPR-11554-End*/

/*! \fn son_bool_et anr_get_auto_maintain_local_cell_as_nbr_status()
 *  \brief This function gets the value of anr_auto_maintain_local_cell_as_nbr configuration parameter
 */
son_bool_et
anr_get_auto_maintain_local_cell_as_nbr_status
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_bool_et anr_is_nbr_cell_on_serving_enb(son_global_enb_id_t *p_nbr_enb_id)
 *  \brief This function finds if a neighbor cell is on the serving eNB or is on peer eNB.
 *  \param p_nbr_enb_id Pointer to the eNB Id
 */ 
son_bool_et
anr_is_nbr_cell_on_serving_enb
(
    son_global_enb_id_t *p_nbr_enb_id
);
/* Rel 3.0 Multi Sector code changes Stop */

/* SPR-13251 Fix Starts */
/*! \fn son_u8 anr_get_log_level()
 *  \brief This function gets the log level
 */
son_u8
anr_get_log_level
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*! \fn son_void_t anr_set_log_level(son_u8 log_level)
 *  \brief This function sets the log level
 *  \param log_level Log Level to be set
 */
son_void_t
anr_set_log_level
(
    son_u8 log_level
);
/* SPR-13251 Fix Ends */
/** eICIC changes start */
son_void_t
anr_set_intf_csg_scheme
(
    son_u32 scheme
);

son_void_t
anr_set_intf_pico_scheme
(
    son_u32 scheme
);

son_void_t
anr_set_intf_macro_scheme
(
    son_u32 scheme
);
/** eICIC changes end */

#endif /* ifndef _SON_ANR_CTXT_MGR_H_ */
