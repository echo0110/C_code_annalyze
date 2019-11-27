/******************************************************************************
*
*   FILE NAME:
*       rrc_llim_global_ctx.h
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


#ifndef _RRC_LLIM_GLOBAL_CTX_H_
#define _RRC_LLIM_GLOBAL_CTX_H_

#include "rrc_defines.h"

struct _rrc_llim_cell_ctx_t;
typedef enum _rrc_cell_start_state_et
{
    RRC_LLIM_AUTOMATIC_CELL_START,
    RRC_LLIM_EXPLICIT_CELL_START
}rrc_cell_start_state_et;

typedef struct _rrc_llim_gl_ctx_t
{
    /* oamh_init_ind_timer duration*/
    rrc_timer_duration_t    oamh_init_ind_duration;

    /* LLIM_OAMH_PROV_REQ: LLIM_CELL_SETUP_TIMER duration */
    rrc_timer_duration_t    cell_cfg_duration;

    /* LLIM_OAMH_PROV_REQ: LLIM_CELL_RECONFIG_TIMER duration */
    rrc_timer_duration_t    cell_recfg_duration;

    /* LLIM_OAMH_PROV_REQ: LLIM_CREATE_UE_ENTITY_TIMER duration */
    rrc_timer_duration_t    create_ue_duration;
    /* LLIM_OAMH_PROV_REQ: LLIM_DELETE_UE_ENTITY_TIMER duration */
    rrc_timer_duration_t    delete_ue_duration;
    /* LLIM_OAMH_PROV_REQ: LLIM_RECONFIG_UE_ENTITY_TIMER duration */
    rrc_timer_duration_t    reconfig_ue_duration;
    /* LLIM_OAMH_PROV_REQ: LLIM_RECONFIG_UE_ENTITY_TIMER duration of guard
     *  timer */
    rrc_timer_duration_t    del_rcfg_ue_duration;

    /* LLIM_OAMH_PROV_REQ: LLIM_CONFIG_UE_SECURITY_TIMER */
    rrc_timer_duration_t    config_ue_security_duration;

    /* LLIM_OAMH_PROV_REQ: LLIM_SUSPEND_UE_ENTITY_TIMER */
    rrc_timer_duration_t    suspend_ue_duration ;

    /* LLIM_OAMH_PROV_REQ: LLIM_REESTABLISH_UE_ENTITY_TIMER duration of guard
     * timer */
    rrc_timer_duration_t    reestablish_ue_duration ;
    /* LLIM_OAMH_PROV_REQ: LLIM_RECONFIG_SECURITY_UE_ENTITY_TIMER duration of
     * guard timer */
    rrc_timer_duration_t    reconfig_security_ue_duration ;
    /* LLIM_OAMH_PROV_REQ: LLIM_RESUME_UE_ENTITY_TIMER duration of
     * guard timer */
    rrc_timer_duration_t    resume_ue_duration ;
    /* LLIM_OAMH_PROV_REQ: LLIM_CHANGE_CRNTI_TIMER duration of
     * guard timer */
    rrc_timer_duration_t    change_crnti_duration ;

    struct _rrc_llim_cell_ctx_t **p_p_cell_ctx;

    struct _rrc_llim_uecc_ctx_t **p_p_uecc_ctx;

    /* LLIM_OAMH_INIT_IND msg timer */
    rrc_timer_t             oamh_init_ind_timer;

    /*  LLIM_CELL_START_TIMER duration */
    rrc_timer_duration_t    cell_start_duration;
    /*  LLIM_CELL_STOP_TIMER duration */
    rrc_timer_duration_t    cell_stop_duration;
    /*  LLIM_CELL_DELETE_TIMER duration */
    rrc_timer_duration_t    cell_delete_duration;
    
    /* LLIM_OAMH_PROV_REQ: max_num_supported_ue */
    U16                        max_num_supported_ue[MAX_NUM_CELLS];  

    U8                         no_of_uecc_instances;
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/
    rrc_timer_t             llim_health_monitor_timer;

    /*BUG 604 Fixes start*/
    U8                      ho_type;
    /*BUG 604 Fixes stop*/

} rrc_llim_gl_ctx_t;


void rrc_llim_init_gl_ctx
(
    rrc_llim_gl_ctx_t   *p_llim_gl_ctx
);

#endif /* _RRC_LLIM_GLOBAL_CTX_H_ */
