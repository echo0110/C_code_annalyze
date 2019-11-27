/*! \file rrm_stats.h
 *  \brief This file contains the structures for RRM STATS.
 *  \date 
 *  \author 

 */
/*!

 * \copyright {Copyright (c) 2009, Aricent Inc. All Rights Reserved}

 */



#ifndef __RRM_STATS_H__
#define __RRM_STATS_H__

#include <rrm_utils.h>
/*! \struct rrmOamDebugStats_t
 *  \brief  This struct contains the RRM_OAM cell response and cell shutdown counters
 *  \param init_config_req initializes config request 
 *  \param init_config_resp_S                          initializes config resp
 *  \param init_config_resp_S_cm                       initializes config resp cm
 *  \param init_config_resp_S_uem                      initializes config resp uem
 *  \param init_config_resp_F                          initializes config resp
 *  \param init_config_resp_F_cell_parse               initializes config resp cell parsing
 *  \param init_config_resp_F_sys_mem_alloc            initializes config resp system memory allocation
 *  \param cell_config_req                             cell config request  
 *  \param cell_config_resp_S                          cell config response   
 *  \param cell_config_resp_F                          cell config response
 *  \param cell_config_resp_F_cell_already_config      cell config response already configured
 *  \param cell_config_resp_F_sys_mem_alloc            cell config response memory allocation
 *  \param cell_config_resp_F_cell_config_failure      cell config response cell config failure
 *  \param cell_config_resp_F_obj_non_exist            cell config response object non existent
 *  \param cell_config_resp_F_cell_deletion_expcted    cell config response deletion expected     
 *  \param cell_config_resp_F_indx_alloc_failed        cell config response index allocation failed
 *  \param cell_reconfig_req                          cell reconfig request
 *  \param cell_reconfig_cell_delete_req              cell reconfig response cell delete request
 *  \param cell_reconfig_resp_S                       cell reconfig response
 *  \param cell_reconfig_resp_F			     cell reconfig response 	
 *  \param cell_reconfig_resp_F_inv_param             cell reconfig response invalid params
 *  \param cell_reconfig_resp_F_cell_unconfig         cell reconfig response cell unconfig
 *  \param cell_reconfig_resp_F_sys_mem_alloc         cell reconfig response system memory allocation
 *  \param cell_reconfig_resp_F_cell_config           cell reconfig response cell config
 *  \param cell_reconfig_resp_F_cell_deletion_expcted cell reconfig response deletion expected   
 *  \param cell_delete_req                            delete cell request
 *  \param cell_delete_resp_S                         delete cell respone
 *  \param cell_delete_resp_F                         delete cell respone
 *  \param cell_delete_resp_F_inv_param               delete cell respone invalid params
 *  \param cell_delete_resp_F_cell_unonfig           delete cell respone cell unconfig 
 *  \param cell_delete_resp_F_sys_mem_alloc         delete cell respone system memory allocation
 *  \param cell_delete_resp_F_obj_non_exist         delete cell respone object non existent
 *  \param delete_all_ue_req                        deletes all ue request
 *  \param delete_all_ue_resp_F                     deletes all ue response
 *  \param delete_all_ue_resp_S                     deletes all ue response
 *  \param delete_all_ue_resp_F_ue_ctxt_not_found  delete all ue resp ue ctx not found
 *  \param delete_all_ue_resp_F_inv_sr_res_idx     delete all ue resp invalid sr index 
 *  \param delete_all_ue_resp_F_inv_sr_res_idx_free delete all ue resp invalid sr index free
 *  \param delete_all_ue_resp_F_inv_cqi_res_idx     delete all ue resp invalid cqi index
 *  \param delete_all_ue_resp_F_inv_cqi_res_idx_free delete all ue resp invalid cqi index  
 *  \param cell_shutdown_req                         cell shutdown request   
 *  \param cell_shutdown_resp_S                      cell shutdown response
 *  \param cell_shutdown_resp_F                      cell shutdown response 
 *  \param cell_shutdown_resp_F_inv_param            cell shutdown response invalid params
 *  \param rac_enbl_disbl_resp_F_cell_unconfig       rac_enbl_disbl_resp cell unconfig    
 *  \param resume_srvc_req                           resume sever request
 *  \param resume_srvc_resp_S                        resume sever response
 *  \param resume_srvc_resp_F                        resume sever response
 *  \param c_freq_and_dl_tx_pwr_req                  c freq and dl transmission power request   
 *  \param c_freq_and_dl_tx_pwr_resp_S               c freq and dl transmission power response
 *  \param c_freq_and_dl_tx_pwr_resp_F               c freq and dl transmission power response
 *  \param c_freq_and_dl_tx_pwr_resp_F_inv_param     c freq and dl transmission power response invalid params 
 *  \param c_freq_and_dl_tx_pwr_resp_F_nw_scan       c freq and dl transmission power response network scan  
 */ 
typedef struct _rrmOamDebugStats_t
{
    /* RRM_OAM INIT CONFIG REQ_RESP COUNTERS */
    S32 init_config_req;
    S32 init_config_resp_S;
    S32 init_config_resp_S_cm;
    S32 init_config_resp_S_uem;
    S32 init_config_resp_F;
    S32 init_config_resp_F_cell_parse;
    S32 init_config_resp_F_sys_mem_alloc;    

    /* RRM_OAM CELL CONFIG REQ_RESP_COUNTERS */
    S32 cell_config_req;
    S32 cell_config_resp_S;
    S32 cell_config_resp_F;
    S32 cell_config_resp_F_cell_already_config;
    S32 cell_config_resp_F_sys_mem_alloc;
    S32 cell_config_resp_F_cell_config_failure;
    S32 cell_config_resp_F_obj_non_exist;
    S32 cell_config_resp_F_cell_deletion_expcted;
    S32 cell_config_resp_F_indx_alloc_failed;

    /* RRM_OAM CELL RE-CONFIG REQ_RESP COUNTERS */
    S32 cell_reconfig_req;
    S32 cell_reconfig_cell_delete_req;
    S32 cell_reconfig_resp_S;
    S32 cell_reconfig_resp_F;
    S32 cell_reconfig_resp_F_inv_param;
    S32 cell_reconfig_resp_F_cell_unconfig;
    S32 cell_reconfig_resp_F_sys_mem_alloc;
    S32 cell_reconfig_resp_F_cell_config; 
    S32 cell_reconfig_resp_F_cell_deletion_expcted;
    S32 cell_reconfig_resp_F_obj_non_exist;

    /* RRM_OAM CELL DELETE REQ_RESP COUNTERS */
    S32 cell_delete_req;
    S32 cell_delete_resp_S;
    S32 cell_delete_resp_F;
    S32 cell_delete_resp_F_inv_param;
    S32 cell_delete_resp_F_cell_unonfig;
    S32 cell_delete_resp_F_sys_mem_alloc;
    S32 cell_delete_resp_F_obj_non_exist;
    S32 delete_all_ue_req;
    S32 delete_all_ue_resp_F;
    S32 delete_all_ue_resp_S;
    S32 delete_all_ue_resp_F_ue_ctxt_not_found;
    S32 delete_all_ue_resp_F_inv_sr_res_idx;
    S32 delete_all_ue_resp_F_inv_sr_res_idx_free;
    S32 delete_all_ue_resp_F_inv_cqi_res_idx;
    S32 delete_all_ue_resp_F_inv_cqi_res_idx_free;

    /* RRM_OAM CELL SHUTDOWN REQ_RESP COUNTERS */  
    S32 cell_shutdown_req;
    S32 cell_shutdown_resp_S;
    S32 cell_shutdown_resp_F;
    S32 cell_shutdown_resp_F_inv_param;
    //S32 cell_shutdown_res_F_cell_unonfig;
    //S32 cell_shutdown_res_F_sys_mem_alloc;

    /* RRM_OAM CELL START REQ_RESP COUNTERS */
    S32 cell_start_req;
    S32 cell_start_resp_S;
    S32 cell_start_resp_F;
    S32 cell_start_resp_F_inv_param;
    S32 cell_start_resp_F_cell_unonfig;
    S32 cell_start_resp_F_sys_mem_alloc;
    S32 cell_start_resp_F_obj_non_exist;

    /* RRM_OAM CELL STOP REQ_RESP COUNTERS */
    S32 cell_stop_req;
    S32 cell_stop_resp_S;
    S32 cell_stop_resp_F;
    S32 cell_stop_resp_F_inv_param;
    S32 cell_stop_resp_F_cell_unonfig;
    //S32 cell_stop_res_F_sys_mem_alloc;

    /* RRM_OAM RAC ENABLE/DISABLE REQ_RESP COUNTERS */
    S32 rac_enbl_disbl_req;
    S32 rac_enbl_disbl_resp_S;
    S32 rac_enbl_disbl_resp_F;
    S32 rac_enbl_disbl_resp_F_inv_param;
    S32 rac_enbl_disbl_resp_F_cell_unconfig;

    /* RRM_OAM RESUME SERVICE REQ_RESP COUNTERS*/
    S32 resume_srvc_req;
    S32 resume_srvc_resp_S;
    S32 resume_srvc_resp_F;

    /* RRM_OAM CARRIER FREQ AND DL TX POWER REQ_RESP COUNTERS */

    S32 c_freq_and_dl_tx_pwr_req;
    S32 c_freq_and_dl_tx_pwr_resp_S;
    S32 c_freq_and_dl_tx_pwr_resp_F;
    S32 c_freq_and_dl_tx_pwr_resp_F_inv_param;
    S32 c_freq_and_dl_tx_pwr_resp_F_nw_scan;

}rrmOamDebugStats_t;


/*! \struct rrmSonDebugStats_t
 *  \brief This struct contains RRM_SON cell switch off and on counters
 *  \param cell_switch_off_req               cell switch off request
 *  \param cell_switch_off_resp_F            cell switch off response
 *  \param cell_switch_off_resp_S            cell switch off response 
 *  \param cell_switch_off_resp_F_inv_param  cell switch off response ivalid params 
 *  \param cell_switch_off_resp_F_cell_unonfig cell switch off response 
 *  \param cell_switch_off_resp_F_cell_parsing cell switch off response cell parsing
 *  \param cell_switch_off_resp_F_already_switch_off cell switch off response already off
 *  \param cell_switch_on_req cell switch on request cell switch off response on request
 *  \param cell_switch_on_resp_F cell    switch on response
 *  \param cell_switch_on_resp_S cell    switch on response
 *  \param cell_switch_on_resp_F_inv_param   switch on response invalid param
 *  \param cell_switch_on_resp_F_cell_unonfig switch on response cell unconfig
 *  \param cell_switch_on_resp_F_cell_parsing switch on response cell parsing
 *  \param cell_switch_on_resp_F_sys_mem_alloc switch on respons system memory allocation
 *  \param register_req                        register request 
 *  \param register_resp_S                     register response  
 *  \param register_resp_F                     register response
 *  \param register_resp_F_inv_param           register response invalid params 
 *  \param register_resp_F_cell_unonfig        register response cell unconfig 
 *  \param registe_resp_F_anr_cell_registered  register response anr registered 
 *  \param registe_resp_F_es_cell_registered   register response es registered 
 *  \param deregister_req                          deregister request 
 *  \param deregister_resp_S                       deregister response
 *  \param deregister_resp_F                       deregister response
 *  \param deregister_resp_F_inv_param             response for invalid parameter
 *  \param deregister_resp_F_cell_unonfig          deregister cell response
 *  \param deregister_resp_F_anr_cell_deregistered deregister cell response anr cell deregistered
 *  \param deregister_resp_F_es_cell_deregistered  deregister cell response es cell deregistered
 *  \param nmm_prepare_req             prepare request
 *  \param nmm_prepare_resp_S          prepare response
 *  \param nmm_prepare_resp_F          prepare response
 *  \param nmm_complete_req            complete request
 *  \param nmm_complete_resp_S         complete response
 *  \param nmm_complete_resp_F         complete response
 *  \param set_attribute_req           set attributes   
 *  \param set_attribute_req_from_es   set attribute request from es
 *  \param set_attribute_req_from_anr  set attribute request from anr
 *  \param set_attribute_resp_F              set attribute response
 *  \param set_attribute_resp_S              set attribute response 
 *  \param set_attribute_resp_F_inv_param    set attribute response invalid params
 *  \param set_attribute_resp_F_cell_unonfig
 *  \param set_attribute_resp_F_son_pointer_alrdy_free
 *  \param set_attribute_resp_F_es_anr_pointer_alrdy_free
 *  \param meas_config_req          measure config response
 *  \param meas_config_resp_S      measure config response
 *  \param meas_config_resp_F       measure config response
 *  \param meas_config_resp_F_inv_param config response for invalid parameter
 *  \param meas_config_resp_F_cell_unconfig
 *  \param meas_config_resp_F_trans_id_unmatched
 *  \param meas_config_resp_F_obj_non_exist
 *  \param meas_config_resp_F_ue_not_configured
 *  \param meas_config_resp_F_ue_internal
 *  \param meas_config_resp_F_obj_not_found config response for object not found
 */
typedef struct _rrmSonDebugStats_t
{
    /*RRM_SON CELL SWITCH OFF REQ_RESP COUNTERS */
    S32 cell_switch_off_req;
    S32 cell_switch_off_resp_F;
    S32 cell_switch_off_resp_S;
    S32 cell_switch_off_resp_F_inv_param;
    S32 cell_switch_off_resp_F_cell_unonfig;
    S32 cell_switch_off_resp_F_cell_parsing;
    //S32 cell_switch_off_res_F_sys_mem_alloc;
    S32 cell_switch_off_resp_F_already_switch_off;

    /*RRM_SON CELL SWITCH ON REQ_RESP COUNTERS*/
    S32 cell_switch_on_req;
    S32 cell_switch_on_resp_F;
    S32 cell_switch_on_resp_S;
    S32 cell_switch_on_resp_F_inv_param;
    S32 cell_switch_on_resp_F_cell_unonfig;
    S32 cell_switch_on_resp_F_cell_parsing;
    S32 cell_switch_on_resp_F_sys_mem_alloc;

    /*RRM_SON REGISTER REQ_RESP COUNTERS*/ 
    S32 register_req;
    S32 register_resp_S;
    S32 register_resp_F;
    S32 register_resp_F_inv_param;
    S32 register_resp_F_cell_unonfig;
    S32 registe_resp_F_anr_cell_registered;
    S32 registe_resp_F_es_cell_registered;

    /*RRM_SON DEREGISTER REQ_RESP COUNTERS*/
    S32 deregister_req;
    S32 deregister_resp_S;
    S32 deregister_resp_F;
    S32 deregister_resp_F_inv_param;
    S32 deregister_resp_F_cell_unonfig;
    S32 deregister_resp_F_anr_cell_deregistered;
    S32 deregister_resp_F_es_cell_deregistered;


    /*RRM_SON NMM PREPARE REQ_RESP COUNTERS*/
    S32 nmm_prepare_req;
    S32 nmm_prepare_resp_S;
    S32 nmm_prepare_resp_F; 

    /*RRM_SON NMM COMPLETE REQ_RESP COUNTERS*/
    S32 nmm_complete_req;
    S32 nmm_complete_resp_S;
    S32 nmm_complete_resp_F;

    /*RRM_SON SET ATTRIBUTE REQ_RESP COUNTERS */
    S32 set_attribute_req;
    S32 set_attribute_req_from_es;
    S32 set_attribute_req_from_anr;
    S32 set_attribute_resp_F;
    S32 set_attribute_resp_S;
    S32 set_attribute_resp_F_inv_param;
    S32 set_attribute_resp_F_cell_unonfig;
    //S32 set_attribute_resp_F_cell_trans_id_unmatched;
    S32 set_attribute_resp_F_son_pointer_alrdy_free;
    S32 set_attribute_resp_F_es_anr_pointer_alrdy_free;

    /* MEAS CONFIG REQ_RESP COUNTERS */
    S32 meas_config_req;
    S32 meas_config_resp_S;
    S32 meas_config_resp_F;
    S32 meas_config_resp_F_inv_param;
    S32 meas_config_resp_F_cell_unconfig;
    S32 meas_config_resp_F_trans_id_unmatched;
    S32 meas_config_resp_F_obj_non_exist;
    S32 meas_config_resp_F_ue_not_configured;
    S32 meas_config_resp_F_ue_internal;
    S32 meas_config_resp_F_obj_not_found;

    /* RACH_OPTIMIZATION_CHANGES_START */
    S32 rach_info_req;
    S32 rach_info_resp_S;
    S32 rach_info_resp_F;
    /* RACH_OPTIMIZATION_CHANGES_END */
}rrmSonDebugStats_t;

/*! \struct rrmRrcDebugStats_t
 *   \brief This struct contains RRM_RRC cell setup,start and config counters
 *   \param cell_setup_resp_S  cell setup response
 *   \param cell_setup_resp_F  cell setup response
 *   \param cell_setup_resp_F_parse_err  cell setup response parsing error
 *   \param cell_setup_resp_F_obj_non_exist  cell setup response object non existent
 *   \param cell_setup_resp_F_cell_deletion_expcted  cell setup response deletion expected
 *   \param cell_start_req  cell start request
 *   \param cell_start_resp_S  cell start response
 *   \param cell_start_resp_F  cell start response
 *   \param cell_start_resp_F_parse_err  cell start response parsing error
 *   \param cell_start_resp_F_obj_non_exist cell start response object non existent
 *   \param cell_reconfig_req cell reconfig request 
 *   \param cell_reconfig_resp_S cell reconfig response
 *   \param cell_reconfig_resp_F cell reconfig response
 *   \param cell_reconfig_resp_F_parse_err cell reconfig response parsing error
 *   \param cell_reconfig_resp_F_obj_non_exist cell reconfig response object non existent
 *   \param cell_reconfig_resp_F_cell_deletion_expcted cell reconfig response cell deletion expected
 *   \param cell_reconfig_resp_F_cell_config  cell reconfig response
 *   \param cell_stop_req cell stop request
 *   \param cell_stop_resp_S cell stop request
 *   \param cell_stop_resp_F cell stop request
 *   \param cell_stop_resp_F_parse_err cell stop request parsing error
 *   \param cell_stop_resp_F_obj_non_exist cell stop request object non existent
 *   \param cell_delete_req    cell delete request
 *   \param cell_delete_resp_F  cell delete response
 *   \param cell_delete_resp_F_parse_err_cm  cell delete response parsing error
 *   \param cell_delete_resp_F_obj_non_exist_cm cell delete response object non existent cm
 *   \param cell_delete_resp_F_obj_non_exist_uem cell delete response object non existent uem
 *   \param cell_delete_resp_F_cm cell delete response cm
 *   \param cell_delete_resp_F_uem cell delete response uem
 *   \param cell_delete_resp_S_cm  cell delete response
 *   \param cell_delete_resp_S_uem cell delete response
 *   \param ue_admission_req     ue admission request
 *   \param ue_admission_resp_S  ue admission response
 *   \param ue_admission_resp_F  ue admission response
 *   \param ue_admission_resp_F_parse_err  ue admission response parsing error
 *   \param ue_admission_resp_F_admission_not_alwd  ue admission response not allowed
 *   \param ue_admission_resp_F_new_context ue admission response new context
 *   \param ue_admission_resp_F_ue_exists ue admission response ue exists
 *   \param ue_admission_resp_F_rac_disabled ue admission response rac disabled
 *   \param ue_admission_resp_F_no_sr_resource ue admission response no sr resource
 *   \param ue_admission_resp_F_no_cqi_resource ue admission response no cqi resource
 *   \param meas_config_req measure config request
 *   \param meas_config_resp_S  measure config response
 *   \param meas_config_resp_F  measure config response
 *   \param ue_erb_setup_req    ue erb setup request 
 *   \param ue_erb_setup_resp_S ue erb setup  response
 *   \param ue_erb_setup_resp_PS ue erb setup  response
 *   \param ue_erb_setup_resp_F  ue erb setup  response
 *   \param ue_erb_setup_resp_F_parse_err   ue erb setup response parsing error
 *   \param ue_erb_setup_resp_F_congestion  ue erb setup response congestion
 *   \param ue_erb_setup_resp_F_erb_exist   ue erb setup response erb exists
 *   \param ue_erb_setup_resp_S_gbr         ue erb setup response 
 *   \param ue_erb_setup_resp_F_gbr         ue erb setup response
 *   \param ue_erb_setup_resp_F_rbc         ue erb setup response
 */
typedef struct _rrmRrcDebugStats_t
{
    /* RRM_RRC CELL SETUP REQ_RESP COUNTERS*/
    S32 cell_setup_req;
    S32 cell_setup_resp_S;
    S32 cell_setup_resp_F;
    S32 cell_setup_resp_F_parse_err;
    S32 cell_setup_resp_F_obj_non_exist;
    S32 cell_setup_resp_F_cell_deletion_expcted;

    /* RRM_RRC CELL START REQ_RESP COUNTERS*/
    S32 cell_start_req;
    S32 cell_start_resp_S;
    S32 cell_start_resp_F;
    S32 cell_start_resp_F_parse_err;
    S32 cell_start_resp_F_obj_non_exist;

    /* RRM_RRC CELL RECONFIG REQ_RESP COUNTERS*/
    S32 cell_reconfig_req;
    S32 cell_reconfig_resp_S;
    S32 cell_reconfig_resp_F;
    S32 cell_reconfig_resp_F_parse_err;
    S32 cell_reconfig_resp_F_obj_non_exist;
    S32 cell_reconfig_resp_F_cell_deletion_expcted;
    S32 cell_reconfig_resp_F_cell_config;

    /* RRM_RRC CELL STOP REQ_RESP COUNTERS*/
    S32 cell_stop_req;
    S32 cell_stop_resp_S;
    S32 cell_stop_resp_F;
    S32 cell_stop_resp_F_parse_err;
    S32 cell_stop_resp_F_obj_non_exist;

    /* RRM_RRC CELL DELETE REQ_RESP COUNTERS*/
    S32 cell_delete_req;
    //S32 cell_delete_res_S;
    S32 cell_delete_resp_F;
    S32 cell_delete_resp_F_parse_err_cm;
    S32 cell_delete_resp_F_obj_non_exist_cm;
    //S32 cell_delete_res_F_parse_err_uem;
    S32 cell_delete_resp_F_obj_non_exist_uem;
    S32 cell_delete_resp_F_cm;
    S32 cell_delete_resp_F_uem;
    S32 cell_delete_resp_S_cm;
    S32 cell_delete_resp_S_uem;

    /* RRM_RRC UE ADMISSION REQ_RESP COUNTERS*/
    S32 ue_admission_req;    
    S32 ue_admission_resp_S;    
    S32 ue_admission_resp_F;
    S32 ue_admission_resp_F_parse_err;
    S32 ue_admission_resp_F_admission_not_alwd;
    S32 ue_admission_resp_F_new_context;
    S32 ue_admission_resp_F_ue_exists;
    S32 ue_admission_resp_F_rac_disabled;
    S32 ue_admission_resp_F_no_sr_resource;
    S32 ue_admission_resp_F_no_cqi_resource;

    /* RRM_RRC MEAS CONFIG REQ_RESP COUNTERS */
    S32 meas_config_req;
    S32 meas_config_resp_S;
    S32 meas_config_resp_F;

    /* RRM_RRC ERB SETUP REQ_RESP COUNTERS */
    S32 ue_erb_setup_req;
    S32 ue_erb_setup_resp_S;
    S32 ue_erb_setup_resp_PS;
    S32 ue_erb_setup_resp_F;
    S32 ue_erb_setup_resp_F_parse_err;
    S32 ue_erb_setup_resp_F_congestion;
    S32 ue_erb_setup_resp_F_erb_exist;
    S32 ue_erb_setup_resp_S_gbr;
    S32 ue_erb_setup_resp_F_gbr;
    S32 ue_erb_setup_resp_F_rbc;

}rrmRrcDebugStats_t;

/*! \  rrmStats_t
 *  \brief this struct contains the debug stats
 *  \param rrmOamDebugStats rrmOamdebug statistics
 *  \param rrmSonDebugStats rrmSondebug statistics
 *  \param rrmRrcDebugStats rrmrrcdebug statistics
 */
typedef struct rrmStats_t 
{
    rrmOamDebugStats_t  rrmOamDebugStats;
    rrmSonDebugStats_t  rrmSonDebugStats;
    //rrmL2DebugStats_t   rrmL2DebugStats;
    rrmRrcDebugStats_t  rrmRrcDebugStats;
    //rrmCellStats_t      rrmCellStats;

}rrmStats_t;
/*! \def RRM_STATS_SHARED_MEM_SIZE
 */
#define RRM_STATS_SHARED_MEM_SIZE sizeof(rrmStats_t)
/*
 * name of shared memory segment
 */
#define RRM_STATS_SHARED_MEM_KEY 1234

#define R_MODE 0
#define W_MODE 1

#define RRM_INCR_MSG_LOG_COUNTER(counter) counter++
#define RRM_DECR_MSG_LOG_COUNTER(counter) counter--

extern rrmStats_t *gp_rrmStats;

rrmStats_t* rrm_stat_shm_init
(
 unsigned short       access_mode
 );

#endif /*__RRM_STATS_H__*/
