/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: rrc_llim_ue_ctx.h,v 1.12 2010/10/13 07:02:59 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : This file contains RRC LLIM global contexts definitions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rrc_llim_ue_ctx.h,v $
 * Revision 1.12.2.1  2010/12/02 06:08:55  gur21231
 * Reestablishment Code Review Req
 * Revision 1.12  2010/10/13 07:02:59  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.2.10.10  2010/10/01 06:42:07  gur04518
 * Removed interface guard timer from LLIM for source HO
 *
 * Revision 1.2.10.9  2010/09/21 04:14:56  gur04518
 * Added one more timer in UE context
 *
 *
 *
 ****************************************************************************/

#ifndef _RRC_LLIM_UE_CTX_H_
#define _RRC_LLIM_UE_CTX_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "rrc_defines.h"
#include "rrc_uecc_llim_intf.h"
#include "rrc_uemh_fsm_states.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef enum
{
    UEMH_FSM_DEL_REQ,
    UEMH_FSM_CREATE_FAIL,
    UEMH_FSM_CREATE_TE
} uemh_fsm_del_op_root_et;

/*SPR_14411_fix Start */
typedef enum
{
    /* Coverity Fix 88002 Start*/
    UEMH_FSM_DEL_INVALID = 0,
    /* Coverity Fix 88002 Stop*/
    UEMH_FSM_DEL_RLC = 1,
    UEMH_FSM_DEL_MAC = 2, 
    UEMH_FSM_DEL_PHY
}uemh_fsm_del_lower_layer_et;

typedef enum
{
     /*Coverity Fix 88089 Start */
    UEMH_FSM_INVALID = 0,
     /*Coverity Fix 88089 Stop */
    UEMH_FSM_CREATE_FAIL_OR_TE_DELETION_COMPLETE = 1,
    UEMH_FSM_DELETE_COMPLETE
}uemh_fsm_del_status_et;
/*SPR_14411_fix Stop */

/*CR 101 changes start*/
typedef struct _rrc_llim_delete_lc_status_t
{
    rrc_lc_id_t     lch_id;    
    U8              status;
} rrc_llim_delete_lc_status_t;
/*CR 101 changes stop*/

typedef struct _rrc_uemh_delete_ue_op_param_t
{
    rrc_timer_t             guard_timer;
    /* Trans ID for UECC_LLIM_DELETE_UE_ENTITY_REQ message */
    rrc_transaction_id_t    transaction_id;
    /* UECC_LLIM_DELETE_UE_ENTITY_REQ ue_index */
    rrc_ue_index_t          ue_index;
    /* UEMH FSM state */
    uemh_fsm_state_et       to_be_del_layers;
    /* cause of delete operation */
    uemh_fsm_del_op_root_et del_op_root;
    /* identify layers for CNF of delete operation */
    U16                     cur_mask_to_be_del_layers;
    /*CR 101 changes start*/
    /* For storing the lc status received from mac_del_ue_entity_cnf 
     * and this info is sent in uecc_llim_delete_ue_entity_resp to uecc */
    rrc_counter_t           mac_delete_lc_status_counter;
    rrc_llim_delete_lc_status_t mac_delete_lc_status[RRC_UECC_LLIM_MAX_DRB];    
    /*CR 101 changes stop*/

    /*SPR_14411_fix Start */
    /* identify lower layers for which delete request has been sent for delete operation */
    U16                     del_req_mask_to_lower_layers;

    /* This IE determines the completion of deletion scenario at lower layers */
    uemh_fsm_del_status_et  deletion_proc_complete_status;
    /*SPR_14411_fix Stop */

} rrc_uemh_delete_ue_op_param_t;

typedef struct _rrc_uemh_create_ue_op_param_t
{
    /* guard timer for UECC_LLIM_CREATE_UE_ENTITY_REQ msg */
    rrc_timer_t guard_timer;
    /* UECC_LLIM_CREATE_UE_ENTITY_REQ msg */
    rrc_uecc_llim_create_ue_entity_req_t uecc_create_ue;
    rrc_uecc_llim_create_ue_entity_resp_t create_ue_resp;
    /* This field maintains number of DRBs succesfully established at each
       layer*/
    U8 num_drb_to_create;
    U8 num_srb_to_create;
} rrc_uemh_create_ue_op_param_t;

 typedef struct _rrc_uemh_suspend_ue_op_param_t
{
    /* guard timer for UECC_LLIM_SUSPEND_UE_ENTITY_REQ msg */
    rrc_timer_t guard_timer;
    /* UECC_LLIM_SUSPEND_UE_ENTITY_REQ msg */
    rrc_uecc_llim_suspend_ue_entity_req_t uecc_suspend_ue;

    rrc_bool_et   pdcp_or_rlc_cnf_recvd;
} rrc_uemh_suspend_ue_op_param_t;

typedef struct _rrc_uemh_reestablish_ue_op_param_t
{
    /* guard timer for UECC_LLIM_REESTABLISH_UE_ENTITY_REQ msg */
    rrc_timer_t guard_timer;
    /* UECC_LLIM_REESTABLISH_UE_ENTITY_REQ msg */
    rrc_uecc_llim_re_establish_ue_entity_req_t uecc_reestablish_ue;
} rrc_uemh_reestablish_ue_op_param_t;


typedef enum _rrc_llim_drb_status_et
{
    RRC_LLIM_DRB_FAILED_AT_MAC,         
    RRC_LLIM_DRB_FAILED_AT_RLC,         
    RRC_LLIM_DRB_FAILED_AT_PDCP,        
    RRC_LLIM_DRB_FAILED_AT_S1U,        
    RRC_LLIM_DRB_FAILED_AT_PHY,         
    
    RRC_LLIM_DRB_PASSED_AT_MAC,         
    RRC_LLIM_DRB_PASSED_AT_RLC,        
    RRC_LLIM_DRB_PASSED_AT_PDCP,        
    RRC_LLIM_DRB_PASSED_AT_S1U,         
    RRC_LLIM_DRB_PASSED_AT_PHY,
    
    RRC_LLIM_DRB_WAITING_FOR_MAC,
    RRC_LLIM_DRB_WAITING_FOR_RLC,
    RRC_LLIM_DRB_WAITING_FOR_PDCP,
    RRC_LLIM_DRB_WAITING_FOR_S1U,
    RRC_LLIM_DRB_WAITING_FOR_PHY
}rrc_llim_drb_status_et;

typedef struct _rrc_llim_drb_status_t
{
#define RRC_DRB_STATUS_NULL   0
    rrc_lc_id_t lc_id;
    U16 drb_status;
    rrc_bool_et drb_modify_fail;
}rrc_llim_drb_status_t;

typedef enum _rrc_llim_reconfig_curr_proc_et
{
    RRC_LLIM_RECONFIG_ADD_LC_ON_GOING,
    RRC_LLIM_RECONFIG_ADD_LC_UECC_TRIGGERED_ROLLBACK_PENDING,
    RRC_LLIM_RECONFIG_ADD_LC_UECC_TRIGGERED_ROLLBACK_ON_GOING,
    RRC_LLIM_RECONFIG_MODIFY_LC_ON_GOING,
    RRC_LLIM_RECONFIG_MODIFY_LC_ROLLBACK_ON_GOING,
    RRC_LLIM_RECONFIG_MODIFY_LC_UECC_TRIGGERED_ROLLBACK_ON_GOING,
    RRC_LLIM_RECONFIG_MODIFY_LC_UECC_TRIGGERED_ROLLBACK_PENDING,
    RRC_LLIM_RECONFIG_HO_CREATE_UE_UECC_TRIGGERED_ROLLBACK_ON_GOING,
    /*ambr*/
    RRC_LLIM_RECONFIG_MODIFY_LC_ROLLBACK_ON_MAC_ON_GOING
    /*ambr*/
}rrc_llim_reconfig_curr_proc_et;

typedef struct _rrc_uemh_rcfg_ue_op_param_t
{
    /* guard timer */
    rrc_timer_t guard_timer;

    /* Flag to check weither ADD LC going on or MODIFY LC*/
    rrc_bool_t is_modify_lc_on_going;

    rrc_llim_reconfig_curr_proc_et current_procedure;

    /*Count for llim_drb_status*/
    U16 passed_drb_at_previous_lower_layer;
 
    /*Mask to keep track of the layer from which cnf is expected*/
    U16 cur_mask_lc_to_be_tracked_layers;
    
    /* Parameters for rollback LC on lower layers */
    rrc_llim_drb_status_t      llim_drb_status[MAX_ERAB_COUNT];

    /* UECC_LLIM_DELETE_LC_REQ msg */
    rrc_uecc_llim_delete_lc_req_t uecc_delete_lc;
    
    /* UECC_LLIM_DELETE_LC_RESP msg */
    rrc_uecc_llim_delete_lc_resp_t delete_lc_resp;
    /* UECC_LLIM_ADD_LC_REQ msg */
    rrc_uecc_llim_add_lc_req_t uecc_add_lc;

    /* Parameters for rollback adding LC on lower layers */
    U16 cur_mask_lc_to_be_del_layers;

    /* UECC_LLIM_ADD_LC_RESP msg */
    rrc_uecc_llim_add_lc_resp_t add_lc_resp;

    /*UECC_LLIM_ADD_LC_ROLLBACK_REQ msg*/
    rrc_uecc_llim_add_rollback_req_t uecc_add_lc_rollback;

    /* UECC_LLIM_ADD_LC_ROLLBACK_RESP msg */
    rrc_uecc_llim_add_rollback_resp_t add_lc_rollback_resp;
    
    /*UECC_LLIM_MODIFY_LC_REQ msg*/
    rrc_uecc_llim_modify_lc_req_t uecc_modify_lc;

    /*UECC_LLIM_MODIFY_LC_RESP msg*/
    rrc_uecc_llim_modify_lc_resp_t uecc_modify_lc_resp;

    /*UECC_LLIM_ADD_LC_ROLLBACK_REQ msg*/
    rrc_uecc_llim_modify_rollback_req_t uecc_modify_lc_rollback;

} rrc_uemh_rcfg_ue_op_param_t;

typedef struct _rrc_uemh_cfg_ue_security_op_param_t
{
    /* guard timer */
    rrc_timer_t guard_timer;

    /* security configuration parameter */
    rrc_uecc_llim_configure_security_req_t uecc_cfg_security;
} rrc_uemh_cfg_ue_security_op_param_t;

typedef struct _rrc_uemh_recfg_ue_security_op_param_t
{
    /* guard timer */
    rrc_timer_t guard_timer;

    /* security reconfiguration parameter */
    rrc_uecc_llim_reconfig_security_req_t  recfg_security_req;
} rrc_uemh_recfg_ue_security_op_param_t;
typedef struct _rrc_uemh_cfg_ue_rach_res_op_param_t
{
    /* guard timer */
    rrc_timer_t guard_timer;
 
    /* Rach res req from UECC */
    rrc_uecc_llim_ho_rach_resource_req_t uecc_llim_rach_res_req;
}rrc_uemh_cfg_ue_rach_res_op_param_t;
typedef struct _rrc_uemh_resume_ue_op_param_t
{
    /* guard timer */
    rrc_timer_t guard_timer;

    /* store to resume SRB or DRBs or BOTH in UL/DL or BOTH dir parameter */
    rrc_uecc_llim_resume_ue_entity_req_t  resume_ue_entity_req;
} rrc_uemh_resume_ue_op_param_t;

typedef struct _rrc_uemh_change_crnti_op_param_t
{
    /* guard timer */
    rrc_timer_t guard_timer;
    
    rrc_uecc_llim_change_crnti_req_t  change_crnti_req;

} rrc_uemh_change_crnti_op_param_t;

typedef struct _rrc_uemh_reset_ue_op_param_t 
{
    U8    transaction_id_presence; /* FALSE - 0, TRUE -1 */    
    rrc_transaction_id_t   transaction_id;
} rrc_uemh_reset_ue_op_param_t;

typedef struct _rrc_uemh_meas_config_op_param_t 
{
    U8    transaction_id_presence; /* FALSE - 0, TRUE -1 */    
    rrc_transaction_id_t   transaction_id;
} rrc_uemh_meas_config_op_param_t;


typedef struct _rrc_llim_ue_ctx_t
{
    /* Bug 839 Fix Start */
    rrc_bitmask_t             bitmask;
#define RRC_UECC_LLIM_UE_CTX_CRNTI_PRESENT 0x01
    /* Bug 839 Fix Stop */
    /*SPR_17121_START*/
#define RRC_UECC_LLIM_MAC_I_PRESENT 0x02
    /*SPR_17121_END*/


    /* cell index */
    rrc_cell_index_t    cell_index;

    /* UEMH FSM current state */
    uemh_fsm_state_et   current_state;

    /* UE index. Used only for logging purposes */
    rrc_ue_index_t      ue_index;

    /* Bug 839 Fix Start */
    /* CRNTI value received in HO_RACH_RESOURCE_RESP */
    rrc_rnti_t                crnti;
    /*^ O, RRC_UECC_LLIM_UE_CTX_CRNTI_PRESENT ^*/
    /* Bug 839 Fix Stop */

    /* data for current operation */
    struct 
    {
        rrc_uemh_create_ue_op_param_t       *p_create_ue;
        rrc_uemh_delete_ue_op_param_t       *p_del_ue;
        rrc_uemh_rcfg_ue_op_param_t         *p_rcfg_ue;
        rrc_uemh_cfg_ue_security_op_param_t *p_cfg_ue_security;
        rrc_uemh_cfg_ue_rach_res_op_param_t *p_rach_res_req;
        rrc_uemh_suspend_ue_op_param_t  *p_suspend_ue_req ;
        rrc_uemh_reestablish_ue_op_param_t  *p_reestablish_ue;
        rrc_uemh_recfg_ue_security_op_param_t *p_recfg_ue_security;
        rrc_uemh_resume_ue_op_param_t        *p_resume_ue;
        rrc_uemh_change_crnti_op_param_t     *p_change_crnti;
        rrc_uemh_meas_config_op_param_t      meas_config;
        rrc_uemh_reset_ue_op_param_t         *p_reset_ue;
    } curr_op_param;
    
    /*BUG 604 changes start*/
    U8                      ho_type;
    /*BUG 604 changes stop*/
    /*SPR_17121_START*/
    U32                     mac_i;
    /*^ O, RRC_UECC_LLIM_MAC_I_PRESENT ^*/
    /*SPR_17121_END*/
    /* SPR 21458 Start */
    U8                          l2_instance_reset_flag; /* Indicates the Layer2 presence during cell delete */ 
    /* SPR 21458 End */
    /* SPR 23458 Fix Start */
    U8         mocn_operator_id_already_configured;
    /* SPR 23458 Fix End */

} rrc_llim_ue_ctx_t;

typedef struct _rrc_llim_uecc_ctx_t
{

    struct _rrc_llim_ue_ctx_t * *p_p_ue_ctx;
} rrc_llim_uecc_ctx_t;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
 
#endif /* _RRC_LLIM_UE_CTX_H_ */
