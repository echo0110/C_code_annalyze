/*******************************************************************************
* File Name   : oam_qcli_cmd_tree.h
*
* Description : This file contains prototype of the functions defined in 
*               command.c

* Revision History:
*
* Date         Author              Reference             Comments
* -----        ------              ---------             --------
* June,2012    Vivek Kumar Dixit   OAM Design Document   Initial Version
* Aug 2014      Aditi Singh         SPR 13116 Fix
* Copyright (c) 2012, Aricent Inc. All Rights Reserved
*******************************************************************************/
/*******************************************************************************
****************************** Includes****************************************
*******************************************************************************/
/*******************************************************************************
#ifndef __COMMAND_H_
#define __COMMAND_H_
#include <packet_codes.h>
*******************************************************************************/

    /*SPR 12727 FIX_2 START */
#define OAM_QCLI_ENB_CMD_MAX_PARAMS     16
        /*SPR 12727 FIX_2 END */

oam_return_et
oam_cell_mgmt_delete_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_cellmgmt_dlpc_params_pkt
(
    int C, 
	char *const *V
);
oam_return_et
oam_CellMgmt_sync_signals_pkt
(
    int C, 
	char *const *V
);
oam_return_et
oam_cellmgmt_mib_info_pkt
(
    int C, 
	char *const *V
);
oam_return_et
oam_cellmgmt_sib1_pkt
(
	int C, 
	char *const *V
);
oam_return_et
oam_cellmgmt_sib2_pkt
(
	int C, 
	char *const *V
);
oam_return_et
oam_cellmgmt_sib3_pkt
(
	int C, 
	char *const *V
);
oam_return_et
oam_cellmgmt_sib4_pkt
(
	int C, 
	char *const *V
);
oam_return_et
oam_cellmgmt_sib5_pkt
(
	int C, 
	char *const *V
);
oam_return_et
oam_cellmgmt_setup_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_cellmgmt_start_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_cellmgmt_stop_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_config_kpi_mac_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_config_kpi_pdcp_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_log_cat_l2_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_log_cat_l2_mac_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_log_cat_l2_rlc_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_log_cat_l2_pdcp_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_log_cat_l2_egtpu_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_stats_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_stats_l2_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_stats_l2_mac_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_stats_l2_pdcp_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_stats_l2_rlc_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_stats_l3_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_stats_l3_rrc_pkt
(
    int C,
    char *const *V
);
oam_return_et 
oam_stats_l3_s1ap_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_set_log_cat_l2_mac_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_l2_sys_fail_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_l1_inf_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_ul_harq_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_dl_harq_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_rach_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_rrc_inf_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_oam_inf_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_dl_pc_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_ul_pc_pkt
(
    int C,
    char *const *V
);
/* 55317 Start */
oam_return_et
oam_mac_dl_strategy_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_ul_strategy_pkt
(
 UInt16 C,
 char *const *V
 );

oam_return_et
oam_mac_pdsch_pkt
(
 UInt16 C,
 char *const *V
 );

oam_return_et
oam_mac_pdcch_pkt
(
 UInt16 C,
 char *const *V
 );
oam_return_et
oam_mac_pusch_pkt
(
 UInt16 C,
 char *const *V
 );
oam_return_et
oam_mac_pucch_pkt
(
 UInt16 C,
 char *const *V
 );
oam_return_et
oam_mac_bcch_pkt
(
 UInt16 C,
 char *const *V
 );
oam_return_et
oam_mac_pcch_pkt
(
 UInt16 C,
 char *const *V
 );
oam_return_et
oam_mac_ccch_pkt
(
 UInt16 C,
 char *const *V
 );
oam_return_et
oam_mac_cqi_pkt
(
 UInt16 C,
 char *const *V
 );
oam_return_et
oam_mac_srs_pkt
(
 UInt16 C,
 char *const *V
 );
oam_return_et
oam_mac_tb_ul_pkt
(
 UInt16 C,
 char *const *V
 );
oam_return_et
oam_mac_tb_dl_pkt
(
 UInt16 C,
 char *const *V
 );
oam_return_et
oam_mac_drx_pkt
(
 UInt16 C,
 char *const *V
 );
oam_return_et
oam_mac_meas_gap_pkt
(
 UInt16 C,
 char *const *V
 );
oam_return_et
oam_mac_meas_handler_pkt
(
 UInt16 C,
 char *const *V
 );
oam_return_et
oam_mac_ul_csi_pkt
(
 UInt16 C,
 char *const *V
 );
oam_return_et
oam_mac_mux_pkt
(
 UInt16 C,
 char *const *V
 );

oam_return_et
oam_mac_dl_sps_strategy_pkt
(
 UInt16 C,
 char *const *V
 );

oam_return_et
oam_mac_ul_sps_strategy_pkt
(
 UInt16 C,
 char *const *V
 );


oam_return_et
oam_mac_scheduler_pkt
(
 UInt16 C,
 char *const *V
 );
oam_return_et
oam_mac_atb_pkt
(
 UInt16 C,
 char *const *V
 );

oam_return_et
oam_mac_ta_pkt
(
 UInt16 C,
 char *const *V
 );

oam_return_et
oam_mac_mux_pkt
(
 UInt16 C,
 char *const *V
 );

oam_return_et
oam_rlc_log_cat_enable_all_pkt
(
 UInt16 C,
 char *const *V
 );

oam_return_et
oam_rlc_am_pkt
(
 UInt16 C,
 char *const *V
 );

oam_return_et
oam_rlc_um_pkt
(
 UInt16 C,
 char *const *V
 );

oam_return_et
oam_rlc_tm_pkt
(
 UInt16 C,
 char *const *V
 );

oam_return_et
oam_rlc_tx_pkt
(
 UInt16 C,
 char *const *V
 );

oam_return_et
oam_rlc_rx_pkt
(
 UInt16 C,
 char *const *V
 );
 

oam_return_et
oam_rlc_mac_pkt
(
 UInt16 C,
 char *const *V
 );

oam_return_et
oam_rlc_pdcp_pkt
(
 UInt16 C,
 char *const *V
 );

oam_return_et
oam_rlc_rrc_pkt
(
 UInt16 C,
 char *const *V
 );

oam_return_et
oam_rlc_oam_pkt
(
 UInt16 C,
 char *const *V
 );

oam_return_et
oam_rlc_timer_pkt
(
 UInt16 C,
 char *const *V
 );


oam_return_et
oam_disable_mac_dl_strategy_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_ul_strategy_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_pdsch_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_pdcch_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_pusch_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_pucch_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_pcch_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_bcch_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_ccch_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_cqi_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_srs_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_tb_ul_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_tb_dl_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_drx_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_meas_gap_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_meas_handler_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_ul_csi_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_mux_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_dl_sps_strategy_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_ul_sps_strategy_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_scheduler_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_atb_pkt
(
 UInt16 C,
 char* const *V
 );

oam_return_et
oam_disable_mac_ta_pkt
(
 UInt16 C,
 char* const *V
 );


oam_return_et
oam_set_log_cat_disable_rlc_all_pkt
(
 UInt16 C,
 char * const *V
 );

oam_return_et
oam_disable_rlc_am_pkt
(
 UInt16 C,
 char * const *V
 );

oam_return_et
oam_disable_rlc_um_pkt
(
 UInt16 C,
 char * const *V
 );

oam_return_et
oam_disable_rlc_tm_pkt
(
 UInt16 C,
 char * const *V
 );

oam_return_et
oam_disable_rlc_tx_pkt
(
 UInt16 C,
 char * const *V
 );

oam_return_et
oam_disable_rlc_rx_pkt
(
 UInt16 C,
 char * const *V
 );

oam_return_et
oam_disable_rlc_mac_pkt
(
 UInt16 C,
 char * const *V
 );

oam_return_et
oam_disable_rlc_pdcp_pkt
(
 UInt16 C,
 char * const *V
 );

oam_return_et
oam_disable_rlc_rrc_pkt
(
 UInt16 C,
 char * const *V
 );

oam_return_et
oam_disable_rlc_oam_pkt
(
 UInt16 C,
 char * const *V
 );

oam_return_et
oam_disable_rlc_timer_pkt
(
 UInt16 C,
 char * const *V
 );

/* Redundant Code Removed Here */
/* 55317 End */

/*CSR:00055317_FIX_START*/
oam_return_et
oam_disable_mac_rrm_inf_pkt
(
  UInt16 C,
   char* const *V
    );
oam_return_et
oam_mac_rrm_inf_pkt
(
  UInt16 C,
   char* const *V
    );
/*CSR:00055317_FIX_START*/


/* SPR 55317 START */

oam_return_et
oam_set_log_cat_disable_pdcp_rx_pkt
(
 UInt16 C,
 char * const*V
 );

oam_return_et
oam_pdcp_enable_log_cat_all_pkt
(
 UInt16 C,
 char * const*V
 );


oam_return_et
oam_pdcp_rrcoam_pkt
(
 UInt16 C,
 char * const*V
 );

oam_return_et
oam_pdcp_tx_pkt
(
 UInt16 C,
 char * const*V
 );

oam_return_et
oam_pdcp_rx_pkt
(
 UInt16 C,
 char * const*V
 );

oam_return_et
oam_pdcp_rrm_int_pkt
(
 UInt16 C,
 char * const*V
 );


oam_return_et
oam_set_log_cat_disable_pdcp_all_pkt
(
 UInt16 C,
 char * const*V
 );

oam_return_et
oam_set_log_cat_disable_pdcp_rrcoam_pkt
(
 UInt16 C,
 char * const*V
 );

oam_return_et
oam_set_log_cat_disable_pdcp_tx_pkt
(
 UInt16 C,
 char * const*V
 );

oam_return_et
oam_set_log_cat_disable_pdcp_rrm_int_pkt
(
 UInt16 C,
 char * const*V
 );




/* SPR 55317 END */
oam_return_et
oam_set_log_cat_l2_egtpu_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_fatallog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_errorlog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_warninglog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_infolog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_debuglog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_pdcp_invalidlog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_pdcp_fatallog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_pdcp_errorlog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_pdcp_warninglog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_pdcp_infolog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_pdcp_debuglog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rlc_disabled_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rlc_fatallog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rlc_errorlog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rlc_warninglog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rlc_infolog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rlc_debuglog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rrm_errorlog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rrm_warninglog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rrm_brieflog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rrm_detailedlog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_son_errorlog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_son_warninglog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_son_brieflog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_son_detailedlog_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_fatallog_l2all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_errorlog_l2all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_warninglog_l2all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_infolog_l2all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_debuglog_l2all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_pdcp_invalidlog_l2all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_pdcp_fatallog_l2all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_pdcp_errorlog_l2all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_pdcp_warninglog_l2all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_pdcp_infolog_l2all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_pdcp_debuglog_l2all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rlc_disabled_l2all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rlc_fatallog_l2all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rlc_errorlog_l2all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rlc_warninglog_l2all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rlc_infolog_l2all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rlc_debuglog_l2all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_fatallog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_errorlog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_warninglog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_infolog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_mac_debuglog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_pdcp_invalidlog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_pdcp_fatallog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_pdcp_errorlog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_pdcp_warninglog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_pdcp_infolog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_pdcp_debuglog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rlc_disabled_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rlc_fatallog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rlc_errorlog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rlc_warninglog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rlc_infolog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rlc_debuglog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_set_log_level_l3_rrc_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rrm_errorlog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rrm_warninglog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rrm_brieflog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_rrm_detailedlog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_son_errorlog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_son_warninglog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_son_brieflog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_son_detailedlog_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_anr_enable_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_anr_disable_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_es_enable_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_es_disable_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_switch_on_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_switch_off_pkt
(
    int C,
    char *const *V
);

oam_return_et
oam_show_log_cat_l2_mac_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_show_log_cat_l2_pdcp_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_show_log_cat_l2_rlc_pkt
(
    int C,
    char *const *V
);

oam_return_et
oam_show_log_level_l2_mac_pkt
(
    int C,
    char *const *V
);

oam_return_et
oam_show_log_level_l2_rlc_pkt
(
    int C,
    char *const *V
);

oam_return_et
oam_show_log_level_l2_pdcp_pkt
(
    int C,
    char *const *V
);

/*SPR 19748 FIXED START*/
oam_return_et
oam_show_log_level_l2_gtpu_pkt
(
    int C,
    char *const *V
);
/*SPR 19748 FIXED END*/

oam_return_et
oam_show_log_level_l2_all_pkt
(
    int C,
    char *const *V
);

/* SPR_13116_CHANGE_START */
/*SPR 13617 START*/
oam_return_et
oam_show_log_level_l3_pkt
(
 int C,
 char *const *V
 );

/*SPR 13617 END*/

oam_return_et
oam_show_log_level_l3_s1ap_pkt
(
  int C,
   char *const *V
    );

oam_return_et
oam_show_log_level_l3_x2ap_pkt
(
  int C,
   char *const *V
    );


oam_return_et
oam_show_log_level_oam_pkt
(
    int C,
        char *const *V
        );
/* SPR_13116_CHANGE_END */


oam_return_et
oam_show_stats_l2_mac_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_show_stats_l2_rlc_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_show_stats_l2_pdcp_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_show_stats_l2_egtpu_pkt
(
    int C,
    char *const *V
);

oam_return_et
oam_show_stats_l3_rrc_all_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_connection_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_paging_pkt
(
    int C,
    char *const *V
);
oam_return_et
oam_ue_assoc_pkt
(
    int C,
    char *const *V
);

