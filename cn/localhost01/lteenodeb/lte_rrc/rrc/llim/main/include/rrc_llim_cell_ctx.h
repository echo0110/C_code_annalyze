/******************************************************************************
*
*   FILE NAME:
*       rrc_llim_cell_ctx.h
*
*   DESCRIPTION:
*       This file contains RRC LLIM global contexts definitions.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   01 Apr 2009     VasylN      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_LLIM_CELL_CTX_H_
#define _RRC_LLIM_CELL_CTX_H_

#include "rrc_defines.h"
#include "rrc_phy_intf.h"
#include "rrc_mac_intf.h"
#include "rrc_csc_llim_intf.h"
#include "rrc_cemh_fsm_states.h"

#define RRC_LLIM_CELL_NUM_MAX_MAC_TRIES         10

typedef enum
{
  NO_RECONFIG_REQ,
  RECONFIG_PHY_ONLY,
  RECONFIG_MAC_ONLY,
  RECONFIG_PHY_MAC,
  CSC_TIMER_EXPIRED,
  RECONFIG_MAC_RLC,
}cemh_reconfig_mode_et;

/* keep in sync with cemh_fsm_tbl! */
typedef enum
{
    CEMH_EVENT_CSC_CONFIG_CELL_REQ,
    CEMH_EVENT_PHY_CONFIG_CELL_CNF,
    CEMH_EVENT_MAC_CONFIG_CELL_CNF,
    CEMH_EVENT_MAC_SFN_CNF,
    CEMH_EVENT_CONFIG_CELL_TIMER_EXPIRED,
    CEMH_EVENT_CSC_DELETE_CELL_REQ,
    CEMH_EVENT_OAMH_CLEANUP_REQ,
    CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ,
    CEMH_EVENT_CSC_RECONFIG_CELL_REQ,   
    CEMH_EVENT_PHY_RECONFIG_CELL_CNF,
    CEMH_EVENT_MAC_RECONFIG_CELL_CNF,
    CEMH_EVENT_RECONFIG_CELL_TIMER_EXPIRED,
    CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ,
    CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ,
    CEMH_EVENT_PHY_CELL_START_REQ,
    CEMH_EVENT_PHY_CELL_START_CNF,
    CEMH_EVENT_MAC_CELL_START_CNF,
    CEMH_EVENT_PHY_CELL_STOP_REQ,
    CEMH_EVENT_PHY_CELL_STOP_CNF,
    CEMH_EVENT_MAC_CELL_STOP_CNF,
    CEMH_EVENT_PHY_DELETE_CELL_CNF,
    CEMH_EVENT_MAC_DELETE_CELL_CNF, 
    CEMH_EVENT_CELL_START_TIMER_EXPIRED,
    CEMH_EVENT_CELL_STOP_TIMER_EXPIRED,
    CEMH_EVENT_CELL_DELETE_TIMER_EXPIRED,
#ifdef LTE_EMBMS_SUPPORTED
    CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF,
    CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF,
#endif
    N_CEMH_EVENTS,
} cemh_fsm_event_et;

typedef enum _cemh_rrc_current_proc_et
{
    CEMH_CELL_SETUP_PHY_FAILED = 1,
    CEMH_CELL_SETUP_MAC_FAILED,
    CEMH_CELL_SETUP_MAC_SFN_FAILED,
    CEMH_CELL_DELETE_ONGOING

}cemh_rrc_current_proc_et;

typedef struct _rrc_llim_cell_ctx_t
{
    /* cell index */
    rrc_cell_index_t    cell_index;
    /* Current state of CEMH FSM  */
    cemh_fsm_state_et   current_state;

    cemh_fsm_event_et   msg_expected_against_event;

    /* Cell Reconfig mode ,PHY  ONLY,MAC ONLY, PHY and MAC both*/   
    cemh_reconfig_mode_et recfg_mode;
    /*Transaction ID*/
    rrc_transaction_id_t    transaction_id;
    /* Number of retries left for CELL Setup in MAC layer */
    rrc_counter_t           mac_retries_left;

    /* guard timer for CSC_LLIM_CONFIG_CELL_REQ */
    rrc_timer_t                       guard_timer;
    rrc_csc_llim_config_cell_req_t    *p_cfg_cell_param;   
    /* Reconfig Request*/
    rrc_csc_llim_reconfig_cell_req_t  *p_recfg_req_param;
    /* Modification period */

   cemh_rrc_current_proc_et            current_proc;

   /* Explicitly cell to be start */
   rrc_cell_start_state_et explicit_start_required;  

   /* MAC cell delete flag*/
   U8                         is_mac_cell_delete_reqd; 
   /* SPR 18354 Fix Start */
#ifdef LTE_EMBMS_SUPPORTED
   mbms_updated_area_info_list_t mbms_updated_area_info_list;
#endif
   /* SPR 18354 Fix End */
   /* SPR 23218/SES-496 Fix Start */
   rrc_transaction_id_t    mac_curr_trans_id;
   /* SPR 23218/SES-496 Fix End */

}rrc_llim_cell_ctx_t;


    

#endif /* _RRC_LLIM_CELL_CTX_H_ */

