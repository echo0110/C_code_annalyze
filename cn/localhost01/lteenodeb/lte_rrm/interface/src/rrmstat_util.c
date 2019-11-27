/******************************************************************************
 *
 *   FILE NAME:
 *
 *       rrm_stat_cli_util.c
 *
 *   DESCRIPTION:
 *       This file reads the rrm stat information from the shared memory segment.
 *
 *   DATE            AUTHOR      REFERENCE       REASON
 *   21 June 2012    gur20966     ---------       Initial
 *
 *   Copyright (c) 2012, Aricent Inc. All Rights Reserved
 *
 ******************************************************************************/

/*
 * include user defined 
 * header files
 */
#include "rrm_stats.h"
#include "rrm_api_defines.h"
#include "rrm_utils.h"

// rrm stats 
rrmStats_t *gp_rrmStats;


rrm_void_t display_rrm_oamstats(rrmOamDebugStats_t*);
rrm_void_t display_rrm_rrcstats(rrmRrcDebugStats_t*);
rrm_void_t display_rrm_sonstats(rrmSonDebugStats_t*);


int main(int argc, char *argv[])
{
    if((gp_rrmStats = rrm_stat_shm_init(R_MODE)) == NULL)
    {
        RRM_PRINTF("Error in initializing data space to shared memory segment\n");
        RRM_EXIT(RRM_ONE);
    }
    display_rrm_oamstats(&(gp_rrmStats->rrmOamDebugStats));
    display_rrm_rrcstats(&(gp_rrmStats->rrmRrcDebugStats));
    display_rrm_sonstats(&(gp_rrmStats->rrmSonDebugStats));

    return RRM_FALSE;
}


rrm_void_t display_rrm_sonstats(rrmSonDebugStats_t *rrm_son_stats)
{
    RRM_PRINTF("RRM_SON DEBUG STATS_START\n\n");

    /* COMP_WARN_1_FEB:compilation warning removal 
       %d is changed to %ld in whole file
     */
    RRM_PRINTF("CELL_SWITCH_OFF_STATS_START\n");
    RRM_PRINTF("cell_switch_off_req : %ld\n",rrm_son_stats->cell_switch_off_req);
    RRM_PRINTF("cell_switch_off_resp_F : %ld\n",rrm_son_stats->cell_switch_off_resp_F);
    RRM_PRINTF("cell_switch_off_resp_S: %ld\n",rrm_son_stats->cell_switch_off_resp_S);
    RRM_PRINTF("cell_switch_off_resp_F_inv_param : %ld\n",rrm_son_stats->cell_switch_off_resp_F_inv_param);
    RRM_PRINTF("cell_switch_off_resp_F_cell_unonfig: %ld\n",rrm_son_stats->cell_switch_off_resp_F_cell_unonfig);
    RRM_PRINTF("cell_switch_off_resp_F_cell_parsing: %ld\n",rrm_son_stats->cell_switch_off_resp_F_cell_parsing);
    RRM_PRINTF("cell_switch_off_resp_F_already_switch_off :%ld\n",rrm_son_stats->cell_switch_off_resp_F_already_switch_off);
    RRM_PRINTF("CELL_SWITCH_OFF_STATS_END\n\n");

    RRM_PRINTF("CELL_SWITCH_ON_STATS_START\n");
    RRM_PRINTF("cell_switch_on_req : %ld\n",rrm_son_stats->cell_switch_on_req);
    RRM_PRINTF("cell_switch_on_resp_F : %ld\n",rrm_son_stats->cell_switch_on_resp_F);
    RRM_PRINTF("cell_switch_on_resp_S: %ld\n",rrm_son_stats->cell_switch_on_resp_S);
    RRM_PRINTF("cell_switch_on_resp_F_inv_param : %ld\n",rrm_son_stats->cell_switch_on_resp_F_inv_param);
    RRM_PRINTF("cell_switch_on_resp_F_cell_unonfig: %ld\n",rrm_son_stats->cell_switch_on_resp_F_cell_unonfig);
    RRM_PRINTF("cell_switch_on_resp_F_cell_parsing: %ld\n",rrm_son_stats->cell_switch_on_resp_F_cell_parsing);
    RRM_PRINTF("cell_switch_on_resp_F_sys_mem_alloc: %ld\n",rrm_son_stats->cell_switch_on_resp_F_sys_mem_alloc);
    RRM_PRINTF("CELL_SWITCH_ON_STATS_END\n\n");

    RRM_PRINTF("REGISTER_STATS_START\n");
    RRM_PRINTF("register_req : %ld\n",rrm_son_stats->register_req);
    RRM_PRINTF("register_resp_S: %ld\n",rrm_son_stats->register_resp_S);
    RRM_PRINTF("register_resp_F: %ld\n",rrm_son_stats->register_resp_F);
    RRM_PRINTF("register_resp_F_inv_param: %ld\n",rrm_son_stats->register_resp_F_inv_param);
    RRM_PRINTF("register_resp_F_cell_unonfig: %ld\n",rrm_son_stats->register_resp_F_cell_unonfig);
    RRM_PRINTF("registe_resp_F_anr_cell_registered: %ld\n",rrm_son_stats->registe_resp_F_anr_cell_registered);
    RRM_PRINTF("registe_resp_F_es_cell_registered: %ld\n",rrm_son_stats->registe_resp_F_es_cell_registered);
    RRM_PRINTF("REGISTER_STATS_END\n\n");

    RRM_PRINTF("DEREGISTER_STATS_START\n");
    RRM_PRINTF("deregister_req : %ld\n",rrm_son_stats->deregister_req);
    RRM_PRINTF("deregister_resp_S: %ld\n",rrm_son_stats->deregister_resp_S);
    RRM_PRINTF("deregister_resp_F: %ld\n",rrm_son_stats->deregister_resp_F);
    RRM_PRINTF("deregister_resp_F_inv_param: %ld\n",rrm_son_stats->deregister_resp_F_inv_param);
    RRM_PRINTF("deregister_resp_F_cell_unonfig: %ld\n",rrm_son_stats->deregister_resp_F_cell_unonfig);
    RRM_PRINTF("deregiste_resp_F_anr_cell_deregistered: %ld\n",rrm_son_stats->deregister_resp_F_anr_cell_deregistered);
    RRM_PRINTF("deregiste_resp_F_es_cell_deregistered: %ld\n",rrm_son_stats->deregister_resp_F_es_cell_deregistered);
    RRM_PRINTF("DEREGISTER_STATS_END\n\n");

    RRM_PRINTF("NMM_PREPARE_STATS_START\n");
    RRM_PRINTF("nmm_prepare_req : %ld\n",rrm_son_stats->nmm_prepare_req);
    RRM_PRINTF("nmm_prepare_resp_S: %ld\n",rrm_son_stats->nmm_prepare_resp_S);
    RRM_PRINTF("nmm_prepare_resp_F: %ld\n",rrm_son_stats->nmm_prepare_resp_F);
    RRM_PRINTF("NMM_PREPARE_STATS_END\n\n");

    RRM_PRINTF("NMM_COMPLETEP_STATS_START\n");
    RRM_PRINTF("nmm_complete_req : %ld\n",rrm_son_stats->nmm_complete_req);
    RRM_PRINTF("nmm_complete_resp_S: %ld\n",rrm_son_stats->nmm_complete_resp_S);
    RRM_PRINTF("nmm_complete_resp_F: %ld\n",rrm_son_stats->nmm_complete_resp_F);
    RRM_PRINTF("NMM_COMPLETE_STATS_END\n\n");

    RRM_PRINTF("MEAS_CONFIG_STATS_START\n");
    RRM_PRINTF("meas_config_req: %ld\n",rrm_son_stats->meas_config_req);
    RRM_PRINTF("meas_config_resp_S: %ld\n",rrm_son_stats->meas_config_resp_S);
    RRM_PRINTF("meas_config_resp_F: %ld\n",rrm_son_stats->meas_config_resp_F);
    RRM_PRINTF("meas_config_resp_F_inv_param: %ld\n",rrm_son_stats->meas_config_resp_F_inv_param);
    RRM_PRINTF("meas_config_resp_F_cell_unconfig: %ld\n",rrm_son_stats->meas_config_resp_F_cell_unconfig);
    RRM_PRINTF("meas_config_resp_F_trans_id_unmatched: %ld\n",rrm_son_stats->meas_config_resp_F_trans_id_unmatched);
    RRM_PRINTF("meas_config_resp_F_obj_non_exist : %ld\n",rrm_son_stats->meas_config_resp_F_obj_non_exist);
    RRM_PRINTF("meas_config_resp_F_ue_not_configured: %ld\n",rrm_son_stats->meas_config_resp_F_ue_not_configured);
    RRM_PRINTF("meas_config_resp_F_ue_internal: %ld\n",rrm_son_stats->meas_config_resp_F_ue_internal);
    RRM_PRINTF("meas_config_resp_F_obj_not_found: %ld\n",rrm_son_stats->meas_config_resp_F_obj_not_found);
    RRM_PRINTF("MEAS_CONFIG_STATS_END\n");

    RRM_PRINTF("RRM_SON DEBUG STATS_END\n\n");
}


rrm_void_t display_rrm_rrcstats(rrmRrcDebugStats_t *rrm_rrc_stats)
{
    RRM_PRINTF("RRM_RRC DEBUG STATS_START\n\n");

    RRM_PRINTF("CELL_SETUP_STATS_START\n");
    RRM_PRINTF("cell_setup_req : %ld\n",rrm_rrc_stats->cell_setup_req);
    RRM_PRINTF("cell_setup_resp_S : %ld\n",rrm_rrc_stats->cell_setup_resp_S);
    RRM_PRINTF("cell_setup_resp_F: %ld\n",rrm_rrc_stats->cell_setup_resp_F);
    RRM_PRINTF("cell_setup_resp_F_obj_non_exist : %ld\n",rrm_rrc_stats->cell_setup_resp_F_obj_non_exist);
    RRM_PRINTF("cell_setup_resp_F_parse_err: %ld\n",rrm_rrc_stats->cell_setup_resp_F_parse_err);
    RRM_PRINTF("cell_setup_resp_F_cell_deletion_expcted: %ld\n",rrm_rrc_stats->cell_setup_resp_F_cell_deletion_expcted);
    RRM_PRINTF("CELL_SETUP_STATS_END\n\n");

    RRM_PRINTF("CELL_START_STATS_START\n");
    RRM_PRINTF("cell_start_req : %ld\n",rrm_rrc_stats->cell_start_req);
    RRM_PRINTF("cell_start_resp_S : %ld\n",rrm_rrc_stats->cell_start_resp_S);
    RRM_PRINTF("cell_start_resp_F: %ld\n",rrm_rrc_stats->cell_start_resp_F);
    RRM_PRINTF("cell_start_resp_F_parse_err : %ld\n",rrm_rrc_stats->cell_start_resp_F_parse_err);
    RRM_PRINTF("cell_start_resp_F_obj_non_exist: %ld\n",rrm_rrc_stats->cell_start_resp_F_obj_non_exist);
    RRM_PRINTF("CELL_START_STATS_END\n\n");

    RRM_PRINTF("CELL_RECONFIG_STATS_START\n");
    RRM_PRINTF("cell_reconfig_req: %ld\n",rrm_rrc_stats->cell_reconfig_req);
    RRM_PRINTF("cell_reconfig_resp_S : %ld\n",rrm_rrc_stats->cell_reconfig_resp_S);
    RRM_PRINTF("cell_reconfig_resp_F: %ld\n",rrm_rrc_stats->cell_reconfig_resp_F);
    RRM_PRINTF("cell_reconfig_resp_F_parse_err: %ld\n",rrm_rrc_stats->cell_reconfig_resp_F_parse_err);
    RRM_PRINTF("cell_reconfig_resp_F_obj_non_exist: %ld\n",rrm_rrc_stats->cell_reconfig_resp_F_obj_non_exist);
    RRM_PRINTF("cell_reconfig_resp_F_cell_deletion_expcted: %ld\n",rrm_rrc_stats->cell_reconfig_resp_F_cell_deletion_expcted);
    RRM_PRINTF("cell_reconfig_resp_F_cell_config: %ld\n",rrm_rrc_stats->cell_reconfig_resp_F_cell_config);
    RRM_PRINTF("CELL_RECONFIG_STATS_END\n\n");

    RRM_PRINTF("CELL_STOP_STATS_START\n");
    RRM_PRINTF("cell_stop_req : %ld\n",rrm_rrc_stats->cell_stop_req);
    RRM_PRINTF("cell_stop_resp_S : %ld\n",rrm_rrc_stats->cell_stop_resp_S);
    RRM_PRINTF("cell_stop_resp_F: %ld\n",rrm_rrc_stats->cell_stop_resp_F);
    RRM_PRINTF("cell_stop_resp_F_parse_err : %ld\n",rrm_rrc_stats->cell_stop_resp_F_parse_err);
    RRM_PRINTF("cell_stop_resp_F_obj_non_exist: %ld\n",rrm_rrc_stats->cell_stop_resp_F_obj_non_exist);
    RRM_PRINTF("CELL_STOP_STATS_END\n\n");

    RRM_PRINTF("CELL_DELETE_STATS_START\n");
    RRM_PRINTF("cell_delete_req : %ld\n",rrm_rrc_stats->cell_delete_req);
    RRM_PRINTF("cell_delete_resp_F : %ld\n",rrm_rrc_stats->cell_delete_resp_F);
    RRM_PRINTF("cell_delete_resp_F_parse_err_cm: %ld\n",rrm_rrc_stats->cell_delete_resp_F_parse_err_cm);
    RRM_PRINTF("cell_delete_resp_F_obj_non_exist_cm: %ld\n",rrm_rrc_stats->cell_delete_resp_F_obj_non_exist_cm);
    RRM_PRINTF("cell_delete_resp_F_obj_non_exist_uem: %ld\n",rrm_rrc_stats->cell_delete_resp_F_obj_non_exist_uem);
    RRM_PRINTF("cell_delete_resp_F_cm: %ld\n",rrm_rrc_stats->cell_delete_resp_F_cm);
    RRM_PRINTF("cell_delete_resp_F_uem: %ld\n",rrm_rrc_stats->cell_delete_resp_F_uem);
    RRM_PRINTF("cell_delete_resp_S_cm: %ld\n",rrm_rrc_stats->cell_delete_resp_S_cm);
    RRM_PRINTF("cell_delete_resp_S_uem: %ld\n",rrm_rrc_stats->cell_delete_resp_S_uem);
    RRM_PRINTF("CELL_DELETE_STATS_END\n\n");

    RRM_PRINTF("UE_ADMISSION_STATS_START\n");
    RRM_PRINTF("ue_admission_req : %ld\n",rrm_rrc_stats->ue_admission_req);
    RRM_PRINTF("ue_admission_resp_S: %ld\n",rrm_rrc_stats->ue_admission_resp_S);
    RRM_PRINTF("ue_admission_resp_F: %ld\n",rrm_rrc_stats->ue_admission_resp_F);
    RRM_PRINTF("ue_admission_resp_F_parse_err: %ld\n",rrm_rrc_stats->ue_admission_resp_F_parse_err);
    RRM_PRINTF("ue_admission_resp_F_admission_not_alwd: %ld\n",rrm_rrc_stats->ue_admission_resp_F_admission_not_alwd);
    RRM_PRINTF("ue_admission_resp_F_new_context: %ld\n",rrm_rrc_stats->ue_admission_resp_F_new_context);
    RRM_PRINTF("ue_admission_resp_F_ue_exists: %ld\n",rrm_rrc_stats->ue_admission_resp_F_ue_exists);
    RRM_PRINTF("ue_admission_resp_F_rac_disabled: %ld\n",rrm_rrc_stats->ue_admission_resp_F_rac_disabled);
    RRM_PRINTF("ue_admission_resp_F_no_sr_resource: %ld\n",rrm_rrc_stats->ue_admission_resp_F_no_sr_resource);
    RRM_PRINTF("ue_admission_resp_F_no_cqi_resource: %ld\n",rrm_rrc_stats->ue_admission_resp_F_no_cqi_resource);
    RRM_PRINTF("UE_ADMISSION_STATS_END\n");

    RRM_PRINTF("MEAS_CONFIG_STATS_START\n");
    RRM_PRINTF("meas_config_req : %ld\n",rrm_rrc_stats->meas_config_req);
    RRM_PRINTF("meas_config_resp_S: %ld\n",rrm_rrc_stats->meas_config_resp_S);
    RRM_PRINTF("meas_config_resp_F: %ld\n",rrm_rrc_stats->meas_config_resp_F);
    RRM_PRINTF("MEAS_CONFIG_STATS_END\n"); 

    RRM_PRINTF("UE_ERB_SETUP_REQ_STATS_START\n");
    RRM_PRINTF("ue_erb_setup_req : %ld\n",rrm_rrc_stats->ue_erb_setup_req);
    RRM_PRINTF("ue_erb_setup_resp_S : %ld\n",rrm_rrc_stats->ue_erb_setup_resp_S);
    RRM_PRINTF("ue_erb_setup_resp_PS : %ld\n",rrm_rrc_stats->ue_erb_setup_resp_PS);
    RRM_PRINTF("ue_erb_setup_resp_F : %ld\n",rrm_rrc_stats->ue_erb_setup_resp_F);
    RRM_PRINTF("ue_erb_setup_resp_F_parse_err : %ld\n",rrm_rrc_stats->ue_erb_setup_resp_F_parse_err);
    RRM_PRINTF("ue_erb_setup_resp_F_congestion : %ld\n",rrm_rrc_stats->ue_erb_setup_resp_F_congestion);
    RRM_PRINTF("ue_erb_setup_resp_F_erb_exist : %ld\n",rrm_rrc_stats->ue_erb_setup_resp_F_erb_exist);
    RRM_PRINTF("UE_ERB_SETUP_REQ_STATS_END\n");

    RRM_PRINTF("RRM_RRC DEBUG STATS_END\n\n");
}
rrm_void_t display_rrm_oamstats(rrmOamDebugStats_t *rrm_oam_stats)
{

    RRM_PRINTF("RRM_OAM DEBUG STATS_START\n\n");

    RRM_PRINTF("INIT_CONFIG_STATS_START\n");
    RRM_PRINTF("init_config_req : %ld\n",rrm_oam_stats->init_config_req);
    RRM_PRINTF("init_config_resp_S : %ld\n",rrm_oam_stats->init_config_resp_S);
    RRM_PRINTF("init_config_resp_S_cm : %ld\n",rrm_oam_stats->init_config_resp_S_cm);
    RRM_PRINTF("init_config_resp_S_uem : %ld\n",rrm_oam_stats->init_config_resp_S_uem);
    RRM_PRINTF("init_config_resp_F : %ld\n",rrm_oam_stats->init_config_resp_F);
    RRM_PRINTF("init_config_resp_F_cell_parse : %ld\n",rrm_oam_stats->init_config_resp_F_cell_parse);
    RRM_PRINTF("init_config_resp_F_sys_mem_alloc : %ld\n",rrm_oam_stats->init_config_resp_F_sys_mem_alloc);
    RRM_PRINTF("INIT_CONFIG_STATS_END\n\n");

    RRM_PRINTF("CELL_CONFIG_STATS_START\n");
    RRM_PRINTF("cell_config_req : %ld\n",rrm_oam_stats->cell_config_req);
    RRM_PRINTF("cell_config_resp_S : %ld\n",rrm_oam_stats->cell_config_resp_S);
    RRM_PRINTF("cell_config_resp_F : %ld\n",rrm_oam_stats->cell_config_resp_F);
    RRM_PRINTF("cell_config_resp_F_cell_already_config : %ld\n",rrm_oam_stats->cell_config_resp_F_cell_already_config);
    RRM_PRINTF("cell_config_resp_F_sys_mem_alloc : %ld\n",rrm_oam_stats->cell_config_resp_F_sys_mem_alloc);
    RRM_PRINTF("cell_config_resp_F_cell_config_failure : %ld\n",rrm_oam_stats->cell_config_resp_F_cell_config_failure);
    RRM_PRINTF("cell_config_resp_F_obj_non_exist : %ld\n",rrm_oam_stats->cell_config_resp_F_obj_non_exist);
    RRM_PRINTF("cell_config_resp_F_cell_deletion_expcted: %ld\n",rrm_oam_stats->cell_config_resp_F_cell_deletion_expcted);
    RRM_PRINTF("cell_config_resp_F_indx_alloc_failed: %ld\n",rrm_oam_stats->cell_config_resp_F_indx_alloc_failed);
    RRM_PRINTF("CELL_CONFIG_STATS_END\n\n");

    RRM_PRINTF("CELL_RECONFIG_STATS_START\n");
    RRM_PRINTF("cell_reconfig_req : %ld\n",rrm_oam_stats->cell_reconfig_req);
    RRM_PRINTF("cell_reconfig_cell_delete_req : %ld\n",rrm_oam_stats->cell_reconfig_cell_delete_req);
    RRM_PRINTF("cell_reconfig_resp_S : %ld\n",rrm_oam_stats->cell_reconfig_resp_S);
    RRM_PRINTF("cell_reconfig_resp_F : %ld\n",rrm_oam_stats->cell_reconfig_resp_F);
    RRM_PRINTF("cell_reconfig_resp_F_inv_param : %ld\n",rrm_oam_stats->cell_reconfig_resp_F_inv_param);
    RRM_PRINTF("cell_reconfig_resp_F_cell_unconfig : %ld\n",rrm_oam_stats->cell_reconfig_resp_F_cell_unconfig);
    RRM_PRINTF("cell_reconfig_resp_F_sys_mem_alloc : %ld\n",rrm_oam_stats->cell_reconfig_resp_F_sys_mem_alloc);
    RRM_PRINTF("cell_reconfig_resp_F_cell_config : %ld\n",rrm_oam_stats->cell_reconfig_resp_F_cell_config);
    RRM_PRINTF("cell_reconfig_resp_F_cell_deletion_expcted : %ld\n",rrm_oam_stats->cell_reconfig_resp_F_cell_deletion_expcted);
    RRM_PRINTF("cell_reconfig_resp_F_obj_non_exist: %ld\n",rrm_oam_stats->cell_reconfig_resp_F_obj_non_exist);
    RRM_PRINTF("CELL_RECONFIG_STATS_END\n\n");

    RRM_PRINTF("CELL_DELETE_START\n");
    RRM_PRINTF("cell_delete_req : %ld\n",rrm_oam_stats->cell_delete_req);
    RRM_PRINTF("cell_delete_resp_S : %ld\n",rrm_oam_stats->cell_delete_resp_S);
    RRM_PRINTF("cell_delete_resp_F : %ld\n",rrm_oam_stats->cell_delete_resp_F);
    RRM_PRINTF("cell_delete_resp_F_inv_param : %ld\n",rrm_oam_stats->cell_delete_resp_F_inv_param);
    RRM_PRINTF("cell_delete_resp_F_cell_unonfig : %ld\n",rrm_oam_stats->cell_delete_resp_F_cell_unonfig);
    RRM_PRINTF("cell_delete_resp_F_sys_mem_alloc : %ld\n",rrm_oam_stats->cell_delete_resp_F_sys_mem_alloc);
    RRM_PRINTF("cell_delete_resp_F_obj_non_exist : %ld\n",rrm_oam_stats->cell_delete_resp_F_obj_non_exist);
    RRM_PRINTF("delete_all_ue_req : %ld\n",rrm_oam_stats->delete_all_ue_req);
    RRM_PRINTF("delete_all_ue_resp_F : %ld\n",rrm_oam_stats->delete_all_ue_resp_F);
    RRM_PRINTF("delete_all_ue_resp_S: %ld\n",rrm_oam_stats->delete_all_ue_resp_S);
    RRM_PRINTF("delete_all_ue_resp_F_ue_ctxt_not_found: %ld\n",rrm_oam_stats->delete_all_ue_resp_F_ue_ctxt_not_found);
    RRM_PRINTF("delete_all_ue_resp_F_inv_sr_res_idx: %ld\n",rrm_oam_stats->delete_all_ue_resp_F_inv_sr_res_idx);
    RRM_PRINTF("delete_all_ue_resp_F_inv_sr_res_idx_free: %ld\n",rrm_oam_stats->delete_all_ue_resp_F_inv_sr_res_idx_free);
    RRM_PRINTF("delete_all_ue_resp_F_inv_cqi_res_idx: %ld\n",rrm_oam_stats->delete_all_ue_resp_F_inv_cqi_res_idx);
    RRM_PRINTF("delete_all_ue_resp_F_inv_cqi_res_idx_free: %ld\n",rrm_oam_stats->delete_all_ue_resp_F_inv_cqi_res_idx_free);
    RRM_PRINTF("CELL_DELETE_STATS_END\n\n");

    RRM_PRINTF("CELL_SHUTDOWN_STATS_START\n");
    RRM_PRINTF("cell_shutdown_req: %ld\n",rrm_oam_stats->cell_shutdown_req);
    RRM_PRINTF("cell_shutdown_resp_S: %ld\n",rrm_oam_stats->cell_shutdown_resp_S);
    RRM_PRINTF("cell_shutdown_resp_F: %ld\n",rrm_oam_stats->cell_shutdown_resp_F);
    RRM_PRINTF("cell_shutdown_resp_F_inv_param: %ld\n",rrm_oam_stats->cell_shutdown_resp_F_inv_param);
    RRM_PRINTF("CELL_SHUTDOWN_STATS_END\n");


    RRM_PRINTF("CELL_START_STATS_START\n");
    RRM_PRINTF("cell_start_req: %ld\n",rrm_oam_stats->cell_start_req);
    RRM_PRINTF("cell_start_resp_S: %ld\n",rrm_oam_stats->cell_start_resp_S);
    RRM_PRINTF("cell_start_resp_F: %ld\n",rrm_oam_stats->cell_start_resp_F);
    RRM_PRINTF("cell_start_resp_F_inv_param: %ld\n",rrm_oam_stats->cell_start_resp_F_inv_param);
    RRM_PRINTF("cell_start_resp_F_cell_unonfig: %ld\n",rrm_oam_stats->cell_start_resp_F_cell_unonfig);
    RRM_PRINTF("cell_start_resp_F_sys_mem_alloc: %ld\n",rrm_oam_stats->cell_start_resp_F_sys_mem_alloc);
    RRM_PRINTF("cell_start_resp_F_obj_non_exist: %ld\n",rrm_oam_stats->cell_start_resp_F_obj_non_exist);
    RRM_PRINTF("CELL_START_STATS_END\n");

    RRM_PRINTF("CELL_STOP_STATS_START\n");
    RRM_PRINTF("cell_stop_req: %ld\n",rrm_oam_stats->cell_stop_req);
    RRM_PRINTF("cell_stop_resp_S: %ld\n",rrm_oam_stats->cell_stop_resp_S);
    RRM_PRINTF("cell_stop_resp_F: %ld\n",rrm_oam_stats->cell_stop_resp_F);
    RRM_PRINTF("cell_stop_resp_F_inv_param: %ld\n",rrm_oam_stats->cell_stop_resp_F_inv_param);
    RRM_PRINTF("cell_stop_resp_F_cell_unonfig: %ld\n",rrm_oam_stats->cell_stop_resp_F_cell_unonfig);
    RRM_PRINTF("CELL_STOP_STATS_END\n");

    RRM_PRINTF("RAC_ENBL_DISBL_STATS_START\n");
    RRM_PRINTF("rac_enbl_disbl_req: %ld\n",rrm_oam_stats->rac_enbl_disbl_req);
    RRM_PRINTF("rac_enbl_disbl_resp_S: %ld\n",rrm_oam_stats->rac_enbl_disbl_resp_S);
    RRM_PRINTF("rac_enbl_disbl_resp_F: %ld\n",rrm_oam_stats->rac_enbl_disbl_resp_F);
    RRM_PRINTF("rac_enbl_disbl_resp_F_inv_param: %ld\n",rrm_oam_stats->rac_enbl_disbl_resp_F_inv_param);
    RRM_PRINTF("rac_enbl_disbl_resp_F_cell_unconfig: %ld\n",rrm_oam_stats->rac_enbl_disbl_resp_F_cell_unconfig);
    RRM_PRINTF("RAC_ENBL_DISBL_STATS_END\n");

    RRM_PRINTF("RESUME_SRVC_STATS_START\n");
    RRM_PRINTF("resume_srvc_req: %ld\n",rrm_oam_stats->resume_srvc_req);
    RRM_PRINTF("resume_srvc_resp_S: %ld\n",rrm_oam_stats->resume_srvc_resp_S);
    RRM_PRINTF("resume_srvc_resp_F: %ld\n",rrm_oam_stats->resume_srvc_resp_F);
    RRM_PRINTF("RESUME_SRVC_STATS_END\n");

    RRM_PRINTF("C_FREQ_DL_TX_PWR_STATS_START\n");
    RRM_PRINTF("c_freq_and_dl_tx_pwr_req: %ld\n",rrm_oam_stats->c_freq_and_dl_tx_pwr_req);
    RRM_PRINTF("c_freq_and_dl_tx_pwr_resp_S: %ld\n",rrm_oam_stats->c_freq_and_dl_tx_pwr_resp_S);
    RRM_PRINTF("c_freq_and_dl_tx_pwr_resp_F: %ld\n",rrm_oam_stats->c_freq_and_dl_tx_pwr_resp_F);
    RRM_PRINTF("c_freq_and_dl_tx_pwr_resp_F_inv_param: %ld\n",rrm_oam_stats->c_freq_and_dl_tx_pwr_resp_F_inv_param);
    RRM_PRINTF("c_freq_and_dl_tx_pwr_resp_F_nw_scan: %ld\n",rrm_oam_stats->c_freq_and_dl_tx_pwr_resp_F_nw_scan);
    RRM_PRINTF("C_FREQ_DL_TX_PWR_STATS_END\n");

    RRM_PRINTF("RRM_OAM DEBUG STATS_END\n\n");
}
