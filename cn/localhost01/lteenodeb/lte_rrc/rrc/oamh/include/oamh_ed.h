/******************************************************************************
**
**  FILE NAME     :  oamh_ed.h
**
**  DESCRIPTION   :  This file contains the function prototypes for OAMH Module
**
**
**  DATE        AUTHOR      REF             REASON
**  --------    -------     ---             ----------
**  12-May-09   Shashank    DDv0.2          Initial
**  21-May-09   Shashank    DDv0.2          Review comments incorporated
**  27-May-09   Shashank    DDv0.2      Formal Review comments incorporated
**  14-Oct-09   OleksandrM  DDv1.41     Global redesign
**
**     Copyright (C) 2009 Aricent Inc . All Rights Reserved
******************************************************************************/
#ifndef _OAMH_ED_H_
#define _OAMH_ED_H_

/****************************************************************************
 * Header File Includes
 ****************************************************************************/
#include "rrc_common_utils.h"
#include "rrc_msg_mgmt.h"
#include "rrc_intrl_api.h"
#include "rrc_oamh_intf.h"
#include "rrc_ext_api.h"
#include "rrc_handlers.h"
#include "oamh_timers.h"

/****************************************************************************
 * Extern definitions
 ****************************************************************************/

/****************************************************************************
 * Macro Definitions
 ****************************************************************************/
#define OAMH_CSC_INIT_RECD       0x01   /* 0 0 0 0 0 0 0 1 */
#define OAMH_LLIM_INIT_RECD      0x02   /* 0 0 0 0 0 0 1 0 */
#define OAMH_UECCMD_INIT_RECD    0x04   /* 0 0 0 0 0 1 0 0 */

#define OAMH_CSC_PROV_RECD       0x01   /* 0 0 0 0 0 0 0 1 */
#define OAMH_LLIM_PROV_RECD      0x02   /* 0 0 0 0 0 0 1 0 */
#define OAMH_UECCMD_PROV_RECD    0x04   /* 0 0 0 0 0 1 0 0 */
#define OAMH_OAM_PROV_REQ_RECD   0x80   /* 1 0 0 0 0 0 0 0 */

#define OAMH_CSC_PROV_SUCCESS    0x08   /* 0 0 0 0 1 0 0 0 */
#define OAMH_LLIM_PROV_SUCCESS   0x10   /* 0 0 0 1 0 0 0 0 */
#define OAMH_UECCMD_PROV_SUCCESS 0x20   /* 0 0 1 0 0 0 0 0 */

#define OAMH_COMM_RECD         0x01   /* 0 0 0 0 0 0 0 1 */
#define OAMH_PROV_RECD         0x02   /* 0 0 0 0 0 0 1 0 */

#define OAMH_CSC_CLEANUP_RECD    0x01   /* 0 0 0 0 0 0 0 1 */
#define OAMH_LLIM_CLEANUP_RECD   0x02   /* 0 0 0 0 0 0 1 0 */
#define OAMH_UECCMD_CLEANUP_RECD 0x04   /* 0 0 0 0 0 1 0 0 */
#define OAMH_READY_CSC_FLAG      0x01 
/* lipa start */ 
#define OAMH_LGW_CONFIGURED 0x01 
/* lipa end */ 

/****************************************************************************
 * Enumeration Definitions
 ****************************************************************************/
typedef enum
{
    OAMH_EVENT_CSC_INIT_IND,      /* CSC initial indication message event */
    OAMH_EVENT_LLIM_INIT_IND,     /* LLIM initial indication message event */
    OAMH_EVENT_UECCMD_INIT_IND,   /* UECC MD initial indication message event */
    OAMH_EVENT_OAM_COMM_INFO_REQ, /* Communication info message event */
    OAMH_EVENT_OAM_PROV_REQ,      /* Provision request message event */
    OAMH_EVENT_CSC_PROV_RESP,     /* CSC provision reponse message event */
    OAMH_EVENT_LLIM_PROV_RESP,    /* LLIM provision reponse message event */
    OAMH_EVENT_UECCMD_PROV_RESP,  /* UECC MD provision reponse message event */
    OAMH_EVENT_OAM_CLEANUP_REQ,   /* Cleanup request message event */
    OAMH_EVENT_CSC_CLEANUP_RESP,  /* CSC cleanup request message event */
    OAMH_EVENT_LLIM_CLEANUP_RESP, /* LLIM cleanup request message event */
    OAMH_EVENT_UECCMD_CLEANUP_RESP, /* UECC MD cleanup request message event */
    /*lipa start*/
    OAMH_EVENT_OAM_ADD_LGW_REQ,     /* Add LGW request message event */
    OAMH_EVENT_OAM_DEL_LGW_REQ,    /*  Delete LGW request message event */
    /*lipa end*/
    /* SPR 20908 Start */
    OAMH_EVENT_CSC_COMMUNICATION_INFO_IND, /* Communication info from CSC */
    /* SPR 20908 End */
    OAMH_NUM_OF_EVENTS,
    OAMH_INVALID_EVENT
} oamh_event_et;


typedef struct _rrc_oamh_cell_ctx_t
{
    rrc_bool_t rrc_oam_get_cell_stats_uecc_status;
    rrc_bool_t rrc_oam_get_cell_stats_csc_status;

    U8    ready_condition;

} rrc_oamh_cell_ctx_t;
/****************************************************************************
 * Types Definitions
 ****************************************************************************/
#define RRC_OAMH_CONNECTION_ESTB_STATS_PRESENT        0x01
#define RRC_OAMH_CONNECTION_RE_ESTB_STATS_PRESENT     0x02
#define RRC_OAMH_CONNECTION_NUMBER_STATS_PRESENT      0x04
#define RRC_OAMH_CONNECTION_SETUP_TIME_STATS_PRESENT  0x08
#define RRC_OAMH_UE_CONTEXT_RELEASE_STATS_PRESENT     0x10
typedef struct _rrc_oamh_connection_stats_status_t
{
    U16                     bitmask;    /*^ BITMASK ^*/
    U16             sampling_period;
    rrc_bool_t      is_reset_done[MAX_NUM_CELLS];
    U16             time_remaining_till_reset[MAX_NUM_CELLS];
} rrc_oamh_connection_stats_status_t;

#define RRC_OAMH_ERAB_SETUP_STATS_PRESENT    0x01
#define RRC_OAMH_ERAB_RELEASE_STATS_PRESENT  0x02
#define RRC_OAMH_ERAB_MODIFY_STATS_PRESENT   0x04
#define RRC_OAMH_ERAB_NUMBER_STATS_PRESENT   0x08
typedef struct _rrc_oamh_erab_stats_status_t
{
    U16                     bitmask;    /*^ BITMASK ^*/
    U16             sampling_period;
    rrc_bool_t      is_reset_done[MAX_NUM_CELLS];
    U16             time_remaining_till_reset[MAX_NUM_CELLS];
} rrc_oamh_erab_stats_status_t;

#define RRC_OAMH_INTRA_ENB_HO_STATS_PRESENT                0x01
#define RRC_OAMH_INTER_ENB_HO_STATS_PRESENT                0x02
#define RRC_OAMH_MEAS_ON_NEIGH_CELL_HO_STATS_PRESENT       0x04
#define RRC_OAMH_INTRA_FREQ_HO_STATS_PRESENT               0x08
#define RRC_OAMH_INTER_FREQ_HO_STATS_PRESENT               0x10
#define RRC_OAMH_DRX_NON_DRX_HO_STATS_PRESENT              0x20
#define RRC_OAMH_INTER_RAT_HO_STATS_PRESENT                0x40
#define RRC_OAMH_HO_FAILURE_NUM_STATS_PRESENT              0x80
typedef struct _rrc_oamh_handover_stats_status_t
{
    U16                     bitmask;    /*^ BITMASK ^*/
    U16             sampling_period;
    rrc_bool_t      is_reset_done[MAX_NUM_CELLS];
    U16             time_remaining_till_reset[MAX_NUM_CELLS];
} rrc_oamh_handover_stats_status_t;

typedef struct _rrc_oamh_sampling_stats_t
{
    U16             sampling_period;
    rrc_bool_t      is_reset_done[MAX_NUM_CELLS];
    U16             time_remaining_till_reset[MAX_NUM_CELLS];
}rrc_oamh_sampling_stats_t;

typedef struct _rrc_oamh_stats_t
{
    struct {
        
        /* Set of bits which defines if counters are enabled or not */
        unsigned is_rrc_connection_counter_enable : 1;
        unsigned is_erab_counter_enable : 1;
        unsigned is_handover_counter_enable : 1;
        unsigned is_ue_assoc_s1_conn_counter_enable : 1;
        unsigned is_paging_counter_enable : 1;
        unsigned is_IratIncMobility_LAI_counter_enable : 1 ;
    } m;
    
    /* rrc connection stats */
    rrc_oamh_connection_stats_status_t  rrc_connection_stats_status;
    
    /* rrc erab stats */
    rrc_oamh_erab_stats_status_t        rrc_erab_stats_status;

    /* rrc handover stats */
    rrc_oamh_handover_stats_status_t    rrc_handover_stats_status;

    /* ue associted s1 connection stats */
    rrc_oamh_sampling_stats_t           rrc_ue_assoc_s1_conn_stats_status;

    /* rrc paging stats */
    rrc_oamh_sampling_stats_t           rrc_paging_stats_status;

    /* rrc incoming Irat mobility events per LA stats*/
    rrc_oamh_sampling_stats_t           rrc_IratIncMobility_LAI_stats_status;

}rrc_oamh_stats_t;


typedef struct _rrc_oamh_gb_context_t
{
    U8 init_ind;            /* Bit-0 for CSC, Bit-1 for LLIM, Bit-2 for UECC MD */
    U8 comm_prov_info_flag; /* Bit-0 for Comm, Bit-1 for Prov */
    U8 prov_status;         /* Bit-0 for CSC, Bit-1 for LLIM, Bit-2 for UECC MD,
                               Bit-8 for OAM */
    U8 cleanup_status;      /* Bit-0 for CSC, Bit-1 for LLIM, Bit-2 for UECC MD */
    U8 current_state;       /* This will give the current state. */
    U16 prov_req_trans_id;  /* Trans ID for Provision req message */
    U16 comm_req_trans_id;  /* Trans ID fro Communication Info req message */
    U16 cleanup_req_trans_id; /* Trans ID fro Cleanup req message */
    rrc_timer_duration_t oam_init_ind_duration; /*oam_init_ind_timer duration*/
    rrc_timer_t          oam_init_ind_timer;    /*timer for sending
                                                         RRC_OAM_INIT_IND msg*/
    rrc_timer_t          oam_license_timer;    /*timer for Licensing expiry*/                                                     
    comm_info_t          *p_comm_info_int;   /* Pointer on internal
                                                representation of comm. info */
    U8                   log_level;    /* value from rrc_oam_log_level_et */
    rrc_bool_et          log_enabled;  /* current state of logger(on/off) */
    U8                   max_csc_reconfig_count ; /* Maxium number retry count
                                                 for cell reconfiguartion */
    U8    csc_cell_index_status; /* Bitmask for presence of communication info of cell */
    rrc_oamh_cell_ctx_t        **p_p_csc_cell_ctx;
    U8                         no_of_uecc_instances;

    
    rrc_oamh_stats_t      rrc_oamh_stats_status;
    oamh_timer_data_t            timers_data;/* OAMH timers */
    U8                    gl_oamh_timer_exp_counter;
 /*Bug 9305 start*/
    U32                   rrc_oamh_health_monitor_val[RRC_MAX_THREADS];
 /*Bug 9305 start*/
    /*lipa start */
    U16 lgw_req_trans_id;  /* Trans ID for LGW req message */
    U8 lgw_configured_status; /* Bitmask indicates whether LGW is configured */
    /*lipa end */
    U8                    adv_logger_hm_chk;
} rrc_oamh_gb_context_t;


/****************************************************************************
 * Function Prototypes
 ****************************************************************************/
void rrc_oamh_connection_timer_expiry_handler(
    rrc_oamh_gb_context_t *p_oamh_gb_context
);

void rrc_oamh_erab_timer_expiry_handler(
    rrc_oamh_gb_context_t *p_oamh_gb_context
);

void rrc_oamh_handover_timer_expiry_handler(
    rrc_oamh_gb_context_t *p_oamh_gb_context
);

void rrc_oamh_ue_assoc_s1_conn_timer_expiry_handler(
    rrc_oamh_gb_context_t *p_oamh_gb_context
);

void rrc_oamh_paging_timer_expiry_handler(
    rrc_oamh_gb_context_t *p_oamh_gb_context
);

void rrc_oamh_common_la_overlap_irat_timer_expiry_handler(
    rrc_oamh_gb_context_t *p_oamh_gb_context
);

rrc_return_t oamh_timer_start(
    oamh_timer_id_et        oamh_timer_id,
    U32                     timer_duration,
    rrc_oamh_gb_context_t   *p_oamh_gb_context
);

void rrc_oam_build_rrc_conn_stats(
    rrc_oam_get_cell_stats_req_t        *message,
    rrc_oamh_gb_context_t               *p_oamh_gb_context,
    rrc_oam_connection_related_stats_t  *p_connection_related_stats
);

void rrc_oam_build_erab_stats(
    rrc_oam_get_cell_stats_req_t        *message,
    rrc_oamh_gb_context_t               *p_oamh_gb_context,
    rrc_oam_erab_related_stats_t        *p_erab_related_stats
);

void rrc_oam_build_handover_stats(
    rrc_oam_get_cell_stats_req_t        *message,
    rrc_oamh_gb_context_t               *p_oamh_gb_context,
    rrc_oam_handover_related_stats_t    *p_handover_related_stats
);    

void oamh_oam_process_config_stats_req(
    void                  *p_api,
    rrc_oamh_gb_context_t *p_oamh_gb_context
);

void rrc_oamh_stop_performance_related_timers(
    rrc_oamh_gb_context_t *p_oamh_gb_context
);


void rrc_oamh_update_intra_enb_ho_stats(
      rrc_oam_handover_stats_ind_t    *p_handover_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_update_inter_enb_ho_stats(
      rrc_oam_handover_stats_ind_t    *p_handover_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_update_inter_rat_ho_stats(
      rrc_oam_handover_stats_ind_t    *p_handover_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_build_conn_estb_stats(
      rrc_uecc_oam_stat_connection_establishment_t    *p_rrc_conn_estb_stats, 
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_build_conn_re_estb_stats(
      rrc_uecc_oam_stat_connection_reestablishment_t    *p_rrc_conn_re_estb_stats, 
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_build_conn_setup_time_stats(
      rrc_oam_connection_related_stats_t  *p_connection_related_stats,
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_build_ue_ctxt_rel_stats(
      rrc_uecc_oam_stat_ue_context_release_t    *p_ue_ctxt_rel_stats, 
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_build_erab_setup_stats(
      rrc_uecc_oam_stat_erab_setup_t *p_erab_setup_stats,
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_build_erab_rel_stats(
      rrc_uecc_oam_stat_erab_release_t    *p_erab_rel_stats,
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_build_erab_modify_stats(
      rrc_uecc_oam_stat_erab_modification_t    *p_erab_modify_stats,
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_build_intra_enb_ho_stats(
      rrc_oam_intra_enb_ho_stats_t    *p_intra_enb_ho_stats, 
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_build_inter_enb_ho_stats(
      rrc_oam_inter_enb_ho_stats_t    *p_inter_enb_ho_stats, 
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_build_meas_on_neigh_cell_ho_stats(
      rrc_oam_handover_related_stats_t    *p_handover_related_stats, 
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_build_inter_freq_ho_stats(
      rrc_oam_inter_freq_ho_stats_t    *p_inter_freq_ho_stats, 
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_build_drx_non_drx_ho_stats(
      rrc_oam_drx_non_drx_ho_stats_t    *p_drx_non_drx_ho_stats, 
      U8 cell_index,
      U8 uecc_index
);
/* BUG 12662 Fix Start */
void rrc_oamh_build_inter_rat_ho_stats(
      rrc_oam_handover_related_stats_t    *p_handover_related_stats, /* BUG 12662 Fix Stop */
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_update_conn_estb_stats(
      rrc_oam_rrc_connection_stats_ind_t    *p_conn_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_update_conn_re_estb_stats(
      rrc_oam_rrc_connection_stats_ind_t    *p_conn_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_update_conn_setup_time_stats(
      rrc_oam_rrc_connection_stats_ind_t    *p_conn_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_update_ue_ctxt_rel_stats(
      rrc_oam_rrc_connection_stats_ind_t    *p_conn_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_update_erab_setup_stats(
      rrc_oam_erab_stats_ind_t    *p_erab_stats_ind,
      U8 index,
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_update_erab_rel_stats(
      rrc_oam_erab_stats_ind_t    *p_erab_stats_ind,
      U8 index,
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_update_erab_modify_stats(
      rrc_oam_erab_stats_ind_t    *p_erab_stats_ind,
      U8 index,
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_update_meas_on_neigh_cell_ho_stats(
      rrc_oam_handover_stats_ind_t    *p_handover_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_update_intra_freq_ho_stats(
      rrc_oam_handover_stats_ind_t    *p_handover_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_update_inter_freq_ho_stats(
      rrc_oam_handover_stats_ind_t    *p_handover_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
);

void rrc_oamh_update_drx_non_drx_ho_stats(
      rrc_oam_handover_stats_ind_t    *p_handover_stats_ind, 
      U8 index,
      U8 cell_index,
      U8 uecc_index
);

void oamh_oam_process_proc_req
(
 /*SPR 17777 +-*/
    rrc_oamh_gb_context_t *p_oamh_gb_context
);

void rrc_oam_send_proc_res
(
    U16 src_module_id,
    U16 trans_id
);

void rrc_oamh_process_health_monitor_chk
(
    rrc_oamh_gb_context_t *p_oamh_gb_context
);

void start_oamh_health_monitor_timer
(
    rrc_oamh_gb_context_t   *p_oamh_gb_context,
    U32                     duration
);

#endif /* _OAMH_ED_H_ */

