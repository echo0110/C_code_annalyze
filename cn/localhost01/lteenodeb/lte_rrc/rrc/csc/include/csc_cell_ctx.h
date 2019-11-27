/******************************************************************************
*
*   FILE NAME:
*       csc_cell_ctx.h
*
*   DESCRIPTION:
*       This file contains RRC CSC cell contexts definitions.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 June 2009    VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _CSC_CELL_CTX_H_
#define _CSC_CELL_CTX_H_

#include "rrc_defines.h"
#include "rrc_rrm_intf.h"
#include "csc_cell_m_fsm_states.h"
#include "search_tree_wrap.h"
#include "rrc_s1ap_csc_intf.h"
/* SPR 20908 Start */
#include "rrc_oam_intf.h"
/* SPR 20908 End */
#define MAX_SCHDLING_COUNT  10

/* SPR 11229 Fix Start */
#define LAST_SENT_SIB8_SFN_UNINIT       SFN_UPPER_LIMIT
/* SPR 11229 Fix Stop */

/*CR 025 changes start */
#define RRC_CSC_UPDATE_PERFORMACE_COUNTER(COUNTER_NAME, INDEX, HIGHER_RANGE_CHECK)\
    if (INDEX > HIGHER_RANGE_CHECK) \
{ RRC_CSC_TRACE(RRC_WARNING, "%s : Wrong Range %d", __FUNCTION__, INDEX); } \
else \
{ COUNTER_NAME++; \
RRC_CSC_TRACE(RRC_DETAILED, \
        "%s: Counter: %s = %d",__FUNCTION__, #COUNTER_NAME, COUNTER_NAME); }
/*CR 025 changes stop */
 struct _csc_gl_ctx_t;
typedef enum 
{
  SI_MAPPING_NOT_CHANGED,
  SI_MAPPING_CHANGED,
  INVALID_SI_MAPPING_PWS,
}csc_si_mapping_et;

typedef enum _rrc_csc_cell_start_state_et
{
    RRC_CSC_AUTOMATIC_CELL_START,
    RRC_CSC_EXPLICIT_CELL_START
}rrc_csc_cell_start_state_et;

typedef struct _si_schdl_info_t
{
  U8    si_mapping_flag;
  U8    schdl_count;
  U16   si_bitmask[MAX_SCHDLING_COUNT];
  U16   curr_schdl_sibs_bitmask;
  new_scheduling_info_list_t     new_schdl_info;
} si_schdl_info_t;

typedef struct _sib_type_12_t
{
  U8                        num_of_segments;
  sib_type_12_Info_t        sib_type_12_Info;
}sib_type_12_t;

typedef struct _sib_type_12_list_t
{
  U8                        count;
  sib_type_12_t             sib_type_12[MAX_NUM_CMAS_WARNING];
}sib_type_12_list_t;

typedef struct _sib_type_11_t
{
    U8                   num_of_segments;
    sib_type_11_Info_t   sib_type_11_Info;
}sib_type_11_t;

typedef struct _pws_broadcast_config_t
{
  U16                           bitmask;   /*^ BITMASK ^*/
  sib_type_10_Info_t            sib_type_10_Info;
  sib_type_11_t                 sib11_info;
  sib_type_12_list_t            sib_type_12_list;
}pws_broadcast_config_t;

typedef struct _etws_info_t
{
    sib_type_10_Info_t           sib10_info;
    sib_type_11_t                sib11_info;
    U16                          sib10_si_periodicity;
    U16                          sib11_si_periodicity;
}etws_info_t;

typedef struct _cmas_info_t
{
    /* Tree anchor */
    search_tree_node_t   anchor;
    /* CMAS key: Serial No. and Message Id */
    U32                 cmas_key;
    /* CMAS counter*/
    U32                 cmas_counter;
    /* Number of segments */
    U8                  num_of_segments;
    /* Number of segments */
    U16                 num_of_repetitions;
    /* Flag for indefinite broadcast */
    rrc_bool_et         indefinite_broadcast;
    /* Time elapsed in broadcasting the warning */
    U32                 cmas_time_elapsed;
    /* SI Periodicity of SIB12 */
    U16                sib12_si_periodicity;
    /* SIB12 Info */
    sib_type_12_Info_t  sib12_info;
}cmas_info_t;

typedef search_tree_t cmas_info_list_t;

/*CDMA2000_CSFB_HO start*/
typedef enum
{
    RRM_TRIGGERED_SIB8_NO_MODIFY,
    RRM_TRIGGERED_SIB8_MODIFY,
    CSC_TRIGGERED_SIB8_MODIFY
}sib_mod_src_et;
/*CDMA2000_CSFB_HO stop*/


typedef struct _csc_cell_ctx_t
{
    /* cell index */
    rrc_cell_index_t            cell_index;
    
    /* Current state of Cell Management FSM */
    cell_m_fsm_state_et         current_state;

    /* SFN. SFN is 10 bits wide: 0, 1, 2, ... SFN_UPPER_LIMIT - 1 */
    rrc_sfn_t                   sfn;
    /* SFN timer */
    rrc_timer_t                 sfn_timer;

    /* timer for slow periodic MAC SFN sync */
    rrc_timer_t  mac_sfn_sync_timer;

    /* Keep current cell configuration */
    rrc_rrm_cell_setup_req_t    cfg_cell_param;
#ifdef LTE_EMBMS_SUPPORTED
    sib13_scheduling_info_t     sib13_scheduling_info;
#endif

    /* data for current operation, only one... */
    rrc_transaction_id_t        transaction_id;
    rrc_timer_t                 guard_timer;

    /* flag to manage indication of SI Modification */
    rrc_bool_t          si_info_modify;
   
    /* Paging records storage */
    void*                       p_paging_storage;


    /* This flag will be set when CSC received RRM cell delete req */
     U8                      rrc_rrm_del_cell_flag;

    /* This flag will be set when CSC received RRM cell stop req */
     U8                      rrc_rrm_stop_cell_flag;

    /* This flag will be set when S1AP DEREG_REQ and UECC_DEL_ALL_UE_REQ have been sent */
     U8                      rrc_rrm_s1ap_dereg_flag;

   /* Cell Reconfiguration (ONGOING)Parameters */
    rrc_rrm_cell_reconfig_req_t recfg_cell_param;   

    /* Retry count for Roll back */
    rrc_retry_count_t           retry_count;

    /* Final SI Schudling Info, updated after getting success from LLIM*/   
    si_schdl_info_t         si_schdl_info;              
    
    /* Temp SI Schudling Info */    
    si_schdl_info_t         *p_temp_si_schdl_info;              

    /* Modification period */
    U32                     mod_period;
    /* SFN GAP */
    U8                      sfn_gap;
    
    /* PAGING API timer due to SI Modification*/
    U32                     mod_period_boud_counter;
    /* Cell Reconfig API timer due to SI Mod*/
    U32                     cell_reconfig_counter;

    /* ETWS counter*/
    U32                     etws_counter;
    /* Time elapsed in broadcasting ETWS */
    U32                     etws_time_elapsed;
    /* ETWS paging counter*/ 
    U32                     etws_paging_counter;
    /* CMAS paging counter*/ 
    U32                     cmas_paging_counter;

    U8                      cmas_flag;

    U8                      si_mod_flag;
    /* Cell Reconfig Api */
    void                    *cell_reconfig_api;
    
    /****** TDD changes start ******/
    duplexing_mode_et duplexing_mode;
    /****** TDD changes start ******/

    /*RRC S1AP PWS req*/
    s1ap_pws_req_t *p_enb_pws_req;
    
    /*RRC S1AP KILL req*/
    s1ap_kill_req_t *p_enb_kill_req;

    /* Bitmask to indicate sibs related to PWS */
    U16                      pws_sibs_bitmask;
 
    /* Information related to ETWS Warning */
    etws_info_t              *etws_info;

    /* Information related to CMAS Warning */
    cmas_info_list_t         cmas_info_list;

    /* Number of broadcasts of CMAS Warning to be sent in kill resp*/
    U16                      number_of_broadcasts;

    /*Structure containing the SIBs related to PWS 
      that are successfully broadcasting */
    pws_broadcast_config_t   *pws_broadcast_config;

    /* List of warning that have been stopped due to timer expiry */
    rrc_warning_info_list_t  warning_info_list;

    /*Null Integrity Support of cell for Emergency services */
    rrc_bool_t              null_integrity_support;
  
    /*cr_398 IEs added*/
    /*thresh_serving_low_q in SIB3*/
    U8                      sib3_thresh_serving_low_q_flag; 
    /*cellSelectionInfo_v920 in SIB1*/
    U8                      sib1_cellSelectionInfo_v920_flag; 

    U8                      rcfg_status_flag;
    rrc_csc_oam_stat_paging_t   csc_statisctics;
    rrc_csc_cell_start_state_et explicit_start_required;
/* CDMA2000_CSFB_HO start*/
    U32                         cdma2000_rand;
    rrc_timer_t                 rand_regenerate_timer;  /* rand_regenerate timer */
    rrc_sf_t                    sf; /*sf*/
    rrc_timestamp_t             timestamp; /*timestamp*/
    rrc_sfn_t                   start_sib8_sfn;
    rrc_sf_t                    start_sib8_sf;
    U16                         sib8_periodicity;
    rrc_bool_et                 nw_in_sync;
    rrc_bool_et                 cell_setup_done;
    U64                         cdma_base_time;
    U32                         sib8_new_segments_generate_count;
    U8                          sib_modify_source; /*sib_mod_src_et*/
    sys_time_t                  sys_time;
    rrc_sfn_t                   last_sent_sib8_sfn;
    U8                          sfn_sf_timestamp_requested;/*rrc_bool_et*/
    U8                          cell_reconfig_done;
    /*following flag keeps track of mac_reconfig procedure triggered during cell setup
     * before cell_in_service state*/
    U8                          mac_reconfig_with_sib8_segments;/*rrc_bool_et*/

    /*sib8 not present in cell reconfig, but present in scheduling info list*/
    U8                          sib8_reconfig_sched_info_present;/*rrc_bool_et*/
/* CDMA2000_CSFB_HO stop */
/*CSR#53522 fix start*/
    U8                          sfn_sync_ongoing; /*rrc_bool_et*/
/*CSR#53522 fix stop*/
     /*cdma_fix bug 10186 start*/
    U8                          proc_buffer_taken; /*boolean flag to take sib8 instance generate processing buffer*/
    /*cdma_fix bug 10186 stop*/
    /* spr 10508 & 10509 start */
    U8                          cell_start_flag;    /*boolean flag to indicate cell start to lower layers in multi sector scenario*/
    /* spr 10508 & 10509 stop */
    /* SPR 11229 Fix Start */
    U8                          sib8_si_window_offset; /* signifies the SFN boundary taking SI window in account */ 
    /* SPR 11229 Fix Stop */
/* SPR 20908 Start */
    U8                          associated_layer2_inst; /* Instance Id of the Layer2 */
    
   rrc_lower_layer_comm_info_t l2_comm_info_params;    /* Communication info of Layer2 */
    void                          *p_api; /* p_api of cell_setup_req, recvd from RRM */
/* SPR 20908 End */
    /* SPR 21369 Start */
    U8                          l2_instance_reset_flag; /* Indicates the Layer2 presence during cell delelte */ 
    /* SPR 21369 End */
} csc_cell_ctx_t;

/******************************************************************************
*   FUNCTION NAME: csc_cell_ctx_create
*
*   DESCRIPTION:
*       Allocates and initializes CSC Cell context.
*
*   RETURNS:
*       pointer on CSC cell context which fields are initialized by default
*
******************************************************************************/
csc_cell_ctx_t* csc_cell_ctx_create(
    struct _csc_gl_ctx_t  *p_csc_gb_context,  /* CSC global context */
    rrc_cell_index_t cell_index);

/******************************************************************************
*   FUNCTION NAME: csc_cell_ctx_cleanup
*
*   DESCRIPTION:
*       Cleans up CSC Cell context.
*
*   RETURNS:
*       None
*
******************************************************************************/
void csc_cell_ctx_cleanup(
    struct _csc_gl_ctx_t  *p_csc_gb_context,  /* CSC global context */
    csc_cell_ctx_t *p_cell_ctx);


/******************************************************************************
*   FUNCTION NAME: csc_cell_ctx_out_of_service_cleanup
*
*   DESCRIPTION:
*       Cleans up CSC Cell context but paging is not deinitialized again.
*
*   RETURNS:
*       None
*
******************************************************************************/
void csc_cell_ctx_out_of_service_cleanup(csc_cell_ctx_t *p_cell_ctx,
    struct _csc_gl_ctx_t    *p_csc_gb_context);
/* SPR 20908 Start */
/******************************************************************************
*   FUNCTION NAME: ext_comm_info_lower_layer_update
*
*   DESCRIPTION:
*       Cleans up CSC Cell context but paging is not deinitialized again.
*
*   RETURNS:
*       None
*
******************************************************************************/
void ext_comm_info_lower_layer_update(rrc_oam_ext_comm_info_t *p_comm_info,
    rrc_l2_comm_info_t *p_l2comm_info );
/* SPR 20908 End */


rrc_return_t csc_cell_ctx_db_init(
    struct _csc_gl_ctx_t  *p_csc_gb_context  /* CSC global context */    
);

void csc_cell_ctx_db_free(
    struct _csc_gl_ctx_t  *p_csc_gb_context   /* CSC global context */
);

rrc_return_t csc_cell_ctx_allocate(
   struct _csc_gl_ctx_t  *p_csc_gb_context,  /* UECC global context */
    rrc_cell_index_t  cell_index
);

#endif /* _CSC_CELL_CTX_H_ */

