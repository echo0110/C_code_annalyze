/******************************************************************************
 *
 *   FILE NAME:
 *       rrm_cell_rrc_intf.h
 *
 *   DESCRIPTION:
 *       Contains functions for Filling Structures of RRC API's
 *
 *
 *
 *   DATE            AUTHOR      REFERENCE       REASON
 *   28 Feb 2012     Gur29220    ---------       Initial
 *   28 MAY 2012     gur30784                 Emergency feature changes
 *
 *   Copyright (c) 2012, Aricent Inc. All Rights Reserved
 *
 ******************************************************************************/
/*! \file rrm_rrc_cell_intf.h
 *  \brief This file contains the Functions for  filling structures of RRC API's
 *  \date Feb 28 ,2012/May 28, 2012
 *  \author gur29220/gur30784
 */
/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */

#ifndef _RRM_RRC_CELL_INTF_H_
#define _RRM_RRC_CELL_INTF_H_ 
/*! \headerfile rrc_rrm_intf.h <>
 */
#include "rrc_rrm_intf.h"
/*! \headerfile rrm_cell_context.h <>
 */
#include "rrm_cell_context.h"
/* MLB Changes start */
/*! \enum rrm_rsu_reporting_periodicity_et
 *  \brief An enum that specifies rsu reporting periodicity
 */
typedef enum
{
    RRM_RSU_REPORTING_PERIODICITY_1000MS = 1,
    RRM_RSU_REPORTING_PERIODICITY_2000MS = 2,
    RRM_RSU_REPORTING_PERIODICITY_5000MS = 5,
    RRM_RSU_REPORTING_PERIODICITY_10000MS = 10
}rrm_rsu_reporting_periodicity_et;

/* MLB Changes end */
#define RRMCC_MAX_TAG_VALUE   32
/*! \fn rrm_return_et build_send_rrc_setup_req(rrm_cell_context_t  *cell_context,rrc_rrm_cell_setup_req_t *rrc_rrm_cell_setup_req)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param rrc_rrm_cell_setup_req Pointer to RRC RRM Cell Setup Request
 */
rrm_return_et
build_send_rrc_setup_req
(
 rrm_cell_context_t *cell_context,
 rrc_rrm_cell_setup_req_t *rrc_rrm_cell_setup_req
 );

/*! \fn rrm_return_et  map_mib_info(rrm_cell_context_t *cell_context, mib_info_t *p_mib_info)
 *  \brief This function Maps the cell setup params coming from MIF to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_mib_info Pointer to Mib Info
 */
rrm_return_et
map_mib_info
(
 rrm_cell_context_t *cell_context,
 mib_info_t         *p_mib_info
 );

/*! \fn rrm_return_et map_phich_config(rrm_cell_context_t *cell_context, phich_config_t *p_phich_config_info)
 *  \brief This function Maps the cell setup params coming from MIF to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_phich_config_info  Pointer to Phich Configuration
 */
rrm_return_et
map_phich_config
(
 rrm_cell_context_t *cell_context,
 phich_config_t     *p_phich_config_info
 );

/*! \fn rrm_return_et map_sib_type_1(rrm_cell_context_t *cell_context,  sib_type_1_Info_t  *p_sib_type_1_info)
 *  \brief This function Maps the cell setup params coming from MIF to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_sib_type_1_info  Pointer to Sib Type 1 Information
 */
rrm_return_et
map_sib_type_1
(
 rrm_cell_context_t *cell_context,
 sib_type_1_Info_t  *p_sib_type_1_info
 );

/* SPR_17367_Start */
rrm_bool_et rrm_cellm_is_trgt_enb_associated_with_source_plmn(
    rrm_oam_cell_plmn_info_t *p_plmn_id,
    rrm_connected_mme_node_t *p_rrm_connected_mme_node);
/* SPR_17367_End*/
/*! \fn rrm_return_et map_cell_access_related_info(rrm_cell_context_t *cell_context, cell_access_related_info_t *p_cell_access_info)
 *  \brief This function Maps the cell setup params coming from MIF to RRC
 *  \param cell_context Pointer to cell context
 *  \param p_cell_access_info Pointer to  Cell Access Related Info
 */
rrm_return_et
map_cell_access_related_info
(
 rrm_cell_context_t *cell_context,
 cell_access_related_info_t *p_cell_access_info
 );

/*! \fn rrm_return_et plmn_id_info_list(rrm_cell_context_t *cell_context, plmn_id_info_list_t *p_plmn_info_list)
 *  \brief This function Maps the cell setup params coming from MIF to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_plmn_info_list Pointer to Plmn Id Info List
 */
rrm_return_et
plmn_id_info_list
(
 rrm_cell_context_t *cell_context,
 plmn_id_info_list_t *p_plmn_info_list
 );

/*! \fn rrm_return_et scheduling_info_list(rrm_cell_context_t *cell_context, scheduling_info_list_t *p_sched_info)
 *  \brief This function Maps the cell setup params coming from MIF to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_sched_info Pointer to Scheduling Info List
 */
rrm_return_et
scheduling_info_list
(
 rrm_cell_context_t *cell_context,
 scheduling_info_list_t  *p_sched_info
 );

/*! \fn rrm_return_et map_cell_selection_info(rrm_cell_context_t *cell_context, cell_selection_Info_t *p_cell_selection_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_cell_selection_info Pointer to Cell Selection Info
 */
rrm_return_et
map_cell_selection_info
(
 rrm_cell_context_t *cell_context,
 cell_selection_Info_t *p_cell_selection_info
 );

/*! \fn rrm_return_et tdd_config(rrm_cell_context_t *cell_context,tdd_config_t *p_tdd_config)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to Cell context 
 *  \param p_tdd_config Pointer to TDD Configuration
 */
rrm_return_et
tdd_config
(void);
 /*SPR 17777 +-*/

/*! \fn rrm_return_et map_sib_type_2(rrm_cell_context_t *cell_context, sib_type_2_Info_t *p_sib_type_2_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_sib_type_2_info Pointer to Sib Type 2 Info
 */
rrm_return_et
map_sib_type_2
(
 rrm_cell_context_t *cell_context,
 sib_type_2_Info_t         *p_sib_type_2_info
 );

/*! \fn rrm_return_et access_barring_info(rrm_cell_context_t *cell_context, access_barring_info_t *p_access_barring_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_access_barring_info Pointer to Access Barring Info
 */

/*EICIC_TDD_CHANGES_START*/
U8
rrm_find_abs_frame_size(tdd_frame_structure_t *p_physical_layer_param_tdd_frame_structure);
/*EICIC_TDD_CHANGES_END*/

rrm_return_et
access_barring_info
(
 rrm_cell_context_t *cell_context,
 access_barring_info_t *p_access_barring_info
 );

/* SPR 20908 Start */
void rrm_fill_l2_comm_info_params
(
 rrc_lower_layer_comm_info_t        *p_cell_setup_l2_comm_info_params,
 rrm_oam_lower_layer_comm_info_t    *p_cell_cntxt_l2_comm_info_params 
 );
/* SPR 20908 End */


/*! \fn rrm_return_et ac_barring_for_mo_signalling(rrm_cell_context_t *cell_context, rrc_rrm_cell_setup_req_t  *rrc_rrm_cell_setup_req)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param rrc_rrm_cell_setup_req Pointer to Cell Setup Request to RRC
 */
rrm_return_et
ac_barring_for_mo_signalling
(
 rrm_cell_context_t *cell_context,
 rrc_rrm_cell_setup_req_t *rrc_rrm_cell_setup_req
 );


/*! \fn rrm_return_et ac_barring_for_mo_data(rrm_cell_context_t *cell_context, rrc_rrm_cell_setup_req_t  *rrc_rrm_cell_setup_req)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to cell context
 *  \param rrc_rrm_cell_setup_req Pointer to Cell Setup Request to RRC
 */
rrm_return_et
ac_barring_for_mo_data
(
 rrm_cell_context_t *cell_context,
 rrc_rrm_cell_setup_req_t *rrc_rrm_cell_setup_req
 );

/*! \fn rrm_return_et radio_resource_config_common_sib(rrm_cell_context_t *cell_context, radio_resource_config_common_sib_t *p_rrc_comon_sib_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to cell context
 *  \param p_rrc_comon_sib_info Pointer to Radio Resource Config Common sib
 */
rrm_return_et
radio_resource_config_common_sib
(
 rrm_cell_context_t *cell_context,
 radio_resource_config_common_sib_t *p_rrc_comon_sib_info
 );

/*! \fn rrm_return_et rach_config_common(rrm_cell_context_t *cell_context, rach_config_common_t *p_rach_config_common)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context          Pointer to Cell Context
 *  \param p_rach_config_common  Pointer to RACH Config
 */
rrm_return_et
rach_config_common
(
 rrm_cell_context_t *cell_context,
 rach_config_common_t *p_rach_config_common
 );

/*! \fn rrm_return_et preamble_info(rrm_cell_context_t *cell_context, preamble_info_t *p_preamble_info)
 *  \brief This funtion build and send setup request to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_preamble_info Pointer to Preamble Info
 */
rrm_return_et
preamble_info
(
 rrm_cell_context_t *cell_context,
 preamble_info_t    *p_preamble_info
 );

/*! \fn rrm_return_et preambles_group_a_config(rrm_cell_context_t *cell_context, preambles_group_a_config_t *p_preamble_group_a)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_preamble_group_a Pointer to Preambles Group A Config
 */
rrm_return_et
preambles_group_a_config
(
 rrm_cell_context_t *cell_context,
 preambles_group_a_config_t *p_preamble_group_a
 );

/*! \fn rrm_return_et power_ramping_params(rrm_cell_context_t *cell_context, power_ramping_params_t *p_power_ramping_params)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_power_ramping_params Power Ramping Params
 */
rrm_return_et
power_ramping_params
(
 rrm_cell_context_t *cell_context,
 power_ramping_params_t *p_power_ramping_params
 );

/*! \fn rrm_return_et ra_supervision_info(rrm_cell_context_t *cell_context, ra_supervision_info_t *p_ra_supervision_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_ra_supervision_info Pointer to RA Supervision information
 */
rrm_return_et
ra_supervision_info
(
 rrm_cell_context_t *cell_context,
 ra_supervision_info_t *p_ra_supervision_info
 );

/*! \fn rrm_return_et bcch_config(rrm_cell_context_t *cell_context, bcch_config_t *p_bcch_config_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_bcch_config_info Pointer to BCCH Config Information
 */
rrm_return_et
bcch_config
(
 rrm_cell_context_t *cell_context,
 bcch_config_t      *p_bcch_config_info
 );

/*! \fn rrm_return_et pcch_config(rrm_cell_context_t *cell_context, pcch_config_t *p_pcch_config_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_pcch_config_info Pointer to PCCH Config Information
 */
rrm_return_et
pcch_config
(
 rrm_cell_context_t *cell_context,
 pcch_config_t      *p_pcch_config_info
 );

/*! \fn rrm_return_et prach_config_sib(rrm_cell_context_t *cell_context,prach_config_sib_t *p_prach_config_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_prach_config_info Pointer to PRACH Config SIB info
 */
rrm_return_et
prach_config_sib
(
 rrm_cell_context_t *cell_context,
 prach_config_sib_t *p_prach_config_info 
 );

/*! \fn rrm_return_et pdsch_config_common(rrm_cell_context_t *cell_context,pdsch_config_common_t *p_pdsch_config_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_pdsch_config_info Pointer to PDSCH Config common
 */
rrm_return_et
pdsch_config_common
(
 rrm_cell_context_t *cell_context,
 pdsch_config_common_t *p_pdsch_config_info
 );

/*! \fn rrm_return_et pusch_config_common(rrm_cell_context_t *cell_context,pusch_config_common_t *p_pusch_config_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_pusch_config_info Pointer to PUSCH Config Common info
 */
rrm_return_et
pusch_config_common
(
 rrm_cell_context_t *cell_context,
 pusch_config_common_t *p_pusch_config_info
 );

/*! \fn rrm_return_et pusch_config_basic(rrm_cell_context_t *cell_context,pusch_config_basic_t *p_pusch_basic_config)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_pusch_basic_config Pointer to PUSCH Config Basic
 */
rrm_return_et
pusch_config_basic
(
 rrm_cell_context_t *cell_context,
 pusch_config_basic_t *p_pusch_basic_config
 );

/*! \fn rrm_return_et ul_ref_signals_pusch(rrm_cell_context_t *cell_context,ul_ref_signals_pusch_t *p_ul_ref_signal_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_ul_ref_signal_info Pointer to PUSCH UL Ref Signals
*/
rrm_return_et
ul_ref_signals_pusch
(
 rrm_cell_context_t *cell_context,
 ul_ref_signals_pusch_t *p_ul_ref_signal_info
 );

/*! \fn rrm_return_et pucch_config_common(rrm_cell_context_t *cell_context,pucch_config_common_t  *p_pucch_config_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Pointer to Cell Context
 *  \param p_pucch_config_info Pointer to PUCCH Config Common 
 */
rrm_return_et
pucch_config_common
(
 rrm_cell_context_t *cell_context,
 pucch_config_common_t  *p_pucch_config_info
 );

/*! \fn rrm_return_et sounding_rs_ul_config_common(rrm_cell_context_t *cell_context,sounding_rs_ul_config_common_t *p_sounding_rs_ul_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Cell Context Information
 *  \param p_sounding_rs_ul_info Sounding RS Uplink Information
 */
rrm_return_et
sounding_rs_ul_config_common
(
 rrm_cell_context_t *cell_context,
 sounding_rs_ul_config_common_t *p_sounding_rs_ul_info
 );

/*! \fn rrm_return_et uplink_power_control_common(rrm_cell_context_t *cell_context,uplink_power_control_common_t *p_ul_power_control_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Cell Context Information 
 *  \param p_ul_power_control_info  Uplink Power Control Information
 */
rrm_return_et
uplink_power_control_common
(
 rrm_cell_context_t *cell_context,
 uplink_power_control_common_t *p_ul_power_control_info
 );

/*! \fn rrm_return_et ue_timers_and_constants(rrm_cell_context_t *cell_context,ue_timers_and_constants_t *p_ue_timers)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Cell Context Information
 *  \param p_ue_timers UE timers Information
 */
rrm_return_et
ue_timers_and_constants
(
 rrm_cell_context_t *cell_context,
 ue_timers_and_constants_t *p_ue_timers
 );

/*! \fn rrm_return_et freq_info(rrm_cell_context_t *cell_context,freq_info_t *p_freq_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Cell Context Information
 *  \param p_freq_info Frequency Information
 */
rrm_return_et
freq_info
(
 rrm_cell_context_t *cell_context,
 freq_info_t        *p_freq_info
 );

/*! \fn rrm_return_et mbsfn_subframe_config_list(rrm_cell_context_t *cell_context, rrc_rrm_cell_setup_req_t  *rrc_rrm_cell_setup_req)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context rrm_cell_context_t
 *  \param rrc_rrm_cell_setup_req rrc_rrm_cell_setup_req_t
 */
rrm_return_et
mbsfn_subframe_config_list
(
 rrm_cell_context_t *cell_context,
 rrc_rrm_cell_setup_req_t *rrc_rrm_cell_setup_req
 );

/*! \fn rrm_return_et map_sib_type_3(rrm_cell_context_t *cell_context, rrc_rrm_cell_setup_req_t  *rrc_rrm_cell_setup_req)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context rrm_cell_context_t
 *  \param rrc_rrm_cell_setup_req rrc_rrm_cell_setup_req_t
 */
rrm_return_et
map_sib_type_3
(
 rrm_cell_context_t *cell_context,
 rrc_rrm_cell_setup_req_t *rrc_rrm_cell_setup_req
 );

/*! \fn rrm_return_et timecell_reselect_info_comm(rrm_cell_context_t *cell_context,rrc_rrm_cell_setup_req_t *rrc_rrm_cell_setup_req)
 * \brief This function build and send setup request to RRC
 * \param cell_context rrm_cell_context_t
 * \param rrc_rrm_cell_setup_req rrc_rrm_cell_setup_req_t
 */
rrm_return_et
timecell_reselect_info_comm
(
 rrm_cell_context_t *cell_context,
 rrc_rrm_cell_setup_req_t *rrc_rrm_cell_setup_req
 );

/*! \fn rrm_return_et cell_reselect_serv_freq_info(rrm_cell_context_t *cell_context, rrc_rrm_cell_setup_req_t  *rrc_rrm_cell_setup_req)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context rrm_cell_context_t
 *  \param rrc_rrm_cell_setup_req rrc_rrm_cell_setup_req_t
 */
rrm_return_et
cell_reselect_serv_freq_info
(
 rrm_cell_context_t *cell_context,
 rrc_rrm_cell_setup_req_t *rrc_rrm_cell_setup_req
 );

/*! \fn rrm_return_et intra_freq_cell_reselect_info(rrm_cell_context_t *cell_context, rrc_rrm_cell_setup_req_t  *rrc_rrm_cell_setup_req)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context rrm_cell_context_t
 *  \param rrc_rrm_cell_setup_req rrc_rrm_cell_setup_req_t
 */
rrm_return_et
intra_freq_cell_reselect_info
(
 rrm_cell_context_t *cell_context,
 rrc_rrm_cell_setup_req_t *rrc_rrm_cell_setup_req
 );

/*! \fn rrm_return_et map_sib_type_4(rrm_cell_context_t *cell_context, rrc_rrm_cell_setup_req_t  *rrc_rrm_cell_setup_req)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context rrm_cell_context_t
 *  \param rrc_rrm_cell_setup_req rrc_rrm_cell_setup_req_t
 */
rrm_return_et
map_sib_type_4
(
 rrm_cell_context_t *cell_context,
 rrc_rrm_cell_setup_req_t *rrc_rrm_cell_setup_req
 );

/*! \fn rrm_return_et intra_freq_neigh_black_cell_list(rrm_cell_context_t *cell_context, rrc_rrm_cell_setup_req_t  *rrc_rrm_cell_setup_req)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context rrm_cell_context_t
 *  \param rrc_rrm_cell_setup_req rrc_rrm_cell_setup_req_t
 */
rrm_return_et
intra_freq_neigh_black_cell_list
(
 rrm_cell_context_t *cell_context,
 rrc_rrm_cell_setup_req_t *rrc_rrm_cell_setup_req
 );

/*! \fn rrm_return_et csg_phy_cell_id_range(rrm_cell_context_t *cell_context, rrc_rrm_cell_setup_req_t  *rrc_rrm_cell_setup_req)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context rrm_cell_context_t
 *  \param rrc_rrm_cell_setup_req rrc_rrm_cell_setup_req_t
 */
rrm_return_et
csg_phy_cell_id_range
(
 rrm_cell_context_t *cell_context,
 rrc_rrm_cell_setup_req_t *rrc_rrm_cell_setup_req
 );

/*LTE_RRM_KLOCWORK_WARN_11_JULY_START*/

/*! \fn rrm_return_et map_sib_type_5_q_offset_cell(const rrm_cell_context_t *cell_context,sib_type_5_Info_t *p_sib_type_5_info,U8 inter_freq_list,U8 neigh_count)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Cell Context
 *  \param p_sib_type_5_info SIB type 5 information
 *  \param inter_freq_list Inter Frequency List
 *  \param neigh_count Neighbor Cell List Count
 */
 /* BUG_371 FIX START */
rrm_return_et
map_sib_type_5_q_offset_cell
(
const rrm_cell_context_t *cell_context,
sib_type_5_Info_t  *p_sib_type_5_info,
U8                  inter_freq_list,
U8 		    neigh_count
 );
 /* BUG_371 FIX END */

/*! \fn rrm_return_et map_sib_type_5_sf_high(const rrm_cell_context_t *cell_context,sib_type_5_Info_t *p_sib_type_5_info,U8 inter_freq_list)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Cell Context
 *  \param p_sib_type_5_info SIB type 5 information
 *  \param inter_freq_list Inter Frequency List
 */
rrm_return_et
map_sib_type_5_sf_high
(
const  rrm_cell_context_t *cell_context,
sib_type_5_Info_t  *p_sib_type_5_info,
U8                  inter_freq_list
 );

/*LTE_RRM_KLOCWORK_WARN_11_JULY_END*/

/*LTE_RRM_KLOCWORK_WARN_10_JULY_START*/
/*! \fn rrm_return_et map_sib_type_5_sf_medium(const rrm_cell_context_t *cell_context,sib_type_5_Info_t *p_sib_type_5_info,U8 inter_freq_list)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Cell Context
 *  \param p_sib_type_5_info SIB type 5 information
 *  \param inter_freq_list Inter Frequency List
 */
rrm_return_et
map_sib_type_5_sf_medium
(
const  rrm_cell_context_t *cell_context,
sib_type_5_Info_t  *p_sib_type_5_info,
U8		    inter_freq_list
 );

/*LTE_RRM_KLOCWORK_WARN_10_JULY_END*/

/*! \fn rrm_return_et map_sib_type_5(rrm_cell_context_t *cell_context,sib_type_5_Info_t *p_sib_type_5_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Cell Context
 *  \param p_sib_type_5_info SIB type 5 information
 */
rrm_return_et
map_sib_type_5
(
 rrm_cell_context_t *cell_context,
 sib_type_5_Info_t  *p_sib_type_5_info
 );

/*! \fn rrm_return_et map_sib_type_6(rrm_cell_context_t *cell_context,sib_type_6_Info_t *p_sib_type_6_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Cell Context
 *  \param p_sib_type_6_info SIB type 6 information
 */
rrm_return_et
map_sib_type_6
(
 rrm_cell_context_t *cell_context,
 sib_type_6_Info_t  *p_sib_type_6_info
 );

/*! \fn rrm_return_et map_sib_type_7(rrm_cell_context_t *cell_context,sib_type_7_Info_t *p_sib_type_7_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Cell Context
 *  \param p_sib_type_7_info SIB type 7 information
 */
rrm_return_et
map_sib_type_7
(
 rrm_cell_context_t *cell_context,
 sib_type_7_Info_t  *p_sib_type_7_info
 );

/*! \fn rrm_return_et map_sib_type_9(rrm_cell_context_t *cell_context,sib_type_9_Info_t *p_sib_type_9_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Cell Context
 *  \param p_sib_type_9_info SIB type 9 information
 */
rrm_return_et
map_sib_type_9
(
 rrm_cell_context_t *cell_context,
 sib_type_9_Info_t  *p_sib_type_9_info
 );

/*! \fn rrm_return_et map_cell_config_param(rrm_cell_context_t *cell_context,rrc_rrm_cell_config_t *p_cell_config_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Cell Context
 *  \param p_cell_config_info Cell Config Information
 */
rrm_return_et
map_cell_config_param
(
 rrm_cell_context_t *cell_context,
 rrc_rrm_cell_config_t *p_cell_config_info
 );

/*! \fn rrm_return_et cell_parameters(rrm_cell_context_t *cell_context,rrc_phy_cell_parameters_t *p_phy_cell_param)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Cell Context
 *  \param p_phy_cell_param Physical Cell parameter
 */
rrm_return_et
cell_parameters
(
 rrm_cell_context_t *cell_context,
 rrc_phy_cell_parameters_t *p_phy_cell_param
 );

/*! \fn rrm_return_et sync_signals(rrm_cell_context_t *cell_context,rrc_phy_sync_signals_t *p_phy_sync_signals_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Cell Context
 *  \param p_phy_sync_signals_info Physical Sync Signals Information
 */
rrm_return_et
sync_signals
(
 rrm_cell_context_t *cell_context,
 rrc_phy_sync_signals_t *p_phy_sync_signals_info
 );

/*SPR_15022_Start*/
/* Code deleted */
/*SPR_15022_End*/

/*! \fn rrm_return_et mac_config(rrm_cell_context_t *cell_context,rrc_mac_config_t *p_mac_config_info)
 *  \brief This function build and send setup request to RRC
 *  \param cell_context Cell Context
 *  \param p_mac_config_info MAC config information
 */
rrm_return_et
mac_config
(
 rrm_cell_context_t *cell_context,
 rrc_mac_config_t   *p_mac_config_info
 );

/*! \fn rrm_return_et build_send_rrc_update_req(rrm_cell_context_t  *p_cell_context)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 */
rrm_return_et
build_send_rrc_update_req
(
 rrm_cell_context_t *p_cell_context
 );

/*RECONFIG MAPPING STARTS*/

/*! \fn rrm_return_et build_send_rrc_reconfig_req(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req,rrm_cell_procedure_et cell_procedure)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 *  \param cell_procedure Cell Procedure
 */
rrm_return_et
build_send_rrc_reconfig_req
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req,
 rrm_cell_procedure_et cell_procedure
 );

/* Release 1.2 changes for cell reconfig interface for SON */
/*! \fn rrm_return_et build_send_rrc_reconfig_req_for_son(rrm_cell_context_t  *p_cell_context)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context 
 */
rrm_return_et
build_send_rrc_reconfig_req_for_son
(
 rrm_cell_context_t *p_cell_context
 );
/* Release 1.2 changes for cell reconfig interface for SON */

/*! \fn rrm_return_et map_reconf_downlink_power_control_common(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */

rrm_return_et
map_reconf_downlink_power_control_common
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
);

/*! \fn rrm_return_et map_reconf_broadcast_info(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_broadcast_info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req,
 /*DYNAMIC ICIC START*/
 rrm_cell_procedure_et cell_procedure
 /*DYNAMIC ICIC END*/
 );

/* Release 1.2 changes for cell reconfig interface for SON */
/*! \fn rrm_return_et map_reconf_sib3_intra_search_v920_for_son(rrm_cell_context_t  *p_cell_context, rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_sib3_intra_search_v920_for_son
(
 rrm_cell_context_t *p_cell_context, 
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_sib3_non_intra_search_v920_for_son(rrm_cell_context_t  *p_cell_context, rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_sib3_non_intra_search_v920_for_son
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_broadcast_info_for_son(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_broadcast_info_for_son
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );
/* Release 1.2 changes for cell reconfig interface for SON */

/*! \fn rrm_return_et map_reconf_mib_info(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_mib_info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_phich_config(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_phich_config
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_sib_type_1_Info(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_sib_type_1_Info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_cell_access_related_info(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_cell_access_related_info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_plmn_id_info_list(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_plmn_id_info_list
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_scheduling_info_list(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_scheduling_info_list
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_cell_selection_info(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_cell_selection_info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_tdd_config(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_tdd_config
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_sib_type_2_Info(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_sib_type_2_Info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_access_barring_info(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_access_barring_info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_void_t map_reconf_ac_barring_for_mo_signalling(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_void_t
map_reconf_ac_barring_for_mo_signalling
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_void_t map_reconf_ac_barring_for_mo_data(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context  Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_void_t
map_reconf_ac_barring_for_mo_data
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_radio_resource_config_common_sib(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_radio_resource_config_common_sib
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_rach_config_common(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_rach_config_common
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_preamble_info(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_preamble_info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*LTE_RRM_KLOCWORK_WARN_10_JULY_START*/
/*! \fn rrm_return_et map_reconf_preambles_group_b_msg_power_offset(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context 
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_preambles_group_b_msg_power_offset
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );
 /*LTE_RRM_KLOCWORK_WARN_10_JULY_END*/

/*! \fn rrm_return_et map_reconf_preambles_group_a_config(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_preambles_group_a_config
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_power_ramping_params(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell reconfig Request
 */
rrm_return_et
map_reconf_power_ramping_params
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*LTE_RRM_KLOCWORK_WARN_10_JULY_START*/
/*! \fn rrm_return_et map_reconf_mac_cont_resol_timer(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_mac_cont_resol_timer
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );


/*! \fn rrm_return_et map_reconf_ra_resp_win_size(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context  rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_ra_resp_win_size
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );
/*LTE_RRM_KLOCWORK_WARN_10_JULY_END*/

/*! \fn rrm_return_et map_reconf_ra_supervision_info(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_ra_supervision_info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_bcch_config(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Config
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_bcch_config
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_pcch_config(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_pcch_config
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_void_t map_reconf_prach_config_sib(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_void_t
map_reconf_prach_config_sib
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_void_t map_reconf_pdsch_config_common(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_void_t
map_reconf_pdsch_config_common
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_pusch_config_common(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 */
rrm_return_et
map_reconf_pusch_config_common
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_pusch_config_basic(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_pusch_config_basic
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_ul_ref_signals_pusch(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 */
rrm_return_et
map_reconf_ul_ref_signals_pusch
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_pucch_config_common(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function send map reconf pucch config common info
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 */
rrm_return_et
map_reconf_pucch_config_common
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*LTE_RRM_KLOCWORK_WARN_10_JULY_START*/
/*! \fn rrm_return_et map_reconf_sounding_ack_nack_srs_simul_trans(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send map reconf sounding ack nack srs simul trans
 *  \param p_cell_context 
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_sounding_ack_nack_srs_simul_trans
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );



/*! \fn rrm_return_et map_reconf_sounding_srs_max_up_pts(rrm_cell_context_t *p_cell_context,
                                                rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_sounding_srs_max_up_pts
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );


/*! \fn rrm_return_et map_reconf_sounding_srs_subframe_config(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t 
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_sounding_srs_subframe_config
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );
/*LTE_RRM_KLOCWORK_WARN_10_JULY_END*/


/*! \fn rrm_return_et map_reconf_sounding_rs_ul_config_common(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief  This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell reconfig request
 */
rrm_return_et
map_reconf_sounding_rs_ul_config_common
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

 /*LTE_RRM_KLOCWORK_WARN_10_JULY_START*/
/*! \fn rrm_return_et map_reconf_ul_pwrctrl_delta_f_pucch_format_two_b(rrm_cell_context_t *p_cell_context,
                                                      rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief  This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_ul_pwrctrl_delta_f_pucch_format_two_b
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );


/*! \fn rrm_return_et map_reconf_ul_pwrctrl_delta_f_pucch_format_two_a(rrm_cell_context_t *p_cell_context,
                                                      rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_ul_pwrctrl_delta_f_pucch_format_two_a
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );



/*! \fn rrm_return_et map_reconf_ul_pwrctrl_delta_f_pucch_format_two(rrm_cell_context_t *p_cell_context,
                                                      rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_ul_pwrctrl_delta_f_pucch_format_two
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );


/*! \fn map_reconf_ul_pwrctrl_delta_f_pucch_format_one_b(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_ul_pwrctrl_delta_f_pucch_format_one_b
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

 /*LTE_RRM_KLOCWORK_WARN_10_JULY_END*/



/*! \fn rrm_return_et map_reconf_ul_pwrctrl_delta_f_pucch_format_one(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_ul_pwrctrl_delta_f_pucch_format_one
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );


/*! \fn rrm_return_et map_reconf_uplink_power_control_common(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 */
rrm_return_et
map_reconf_uplink_power_control_common
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );


/*LTE_RRM_KLOCWORK_WARN_10_JULY_START*/

/*! \fn rrm_return_et map_reconf_ue_timers_and_constants_timer_n311( rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context  rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_ue_timers_and_constants_timer_n311
(
  rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
);


/*! \fn rrm_return_et map_reconf_ue_timers_and_constants_timer_n310( rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context  rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_ue_timers_and_constants_timer_n310
(
  rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
);


/*! \fn rrm_return_et map_reconf_ue_timers_and_constants_timer_311(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context  rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_ue_timers_and_constants_timer_311
(
  rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
);


/*! \fn rrm_return_et map_reconf_ue_timers_and_constants_timer_310( rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t 
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_ue_timers_and_constants_timer_310
(
  rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
);


/*! \fn rrm_return_et map_reconf_ue_timers_and_constants_timer_301( rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t  
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_ue_timers_and_constants_timer_301
(
  rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
);

/*LTE_RRM_KLOCWORK_WARN_10_JULY_END*/

/*! \fn rrm_return_et map_reconf_ue_timers_and_constants(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_ue_timers_and_constants
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_freq_info(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconf freq info to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_freq_info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_mbsfn_subframe_config_list(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_mbsfn_subframe_config_list
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_sib_type_3_Info(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send map reconf sib type 3 info to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_sib_type_3_Info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_sib_type_3_Info_for_son(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconf sib type 3 for son to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_sib_type_3_Info_for_son
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*LTE_RRM_KLOCWORK_WARN_10_JULY_START*/
/*! \fn rrm_return_et map_reconf_timecell_t_hyst_normal(rrm_cell_context_t *p_cell_context,
                                            ,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context  rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_timecell_t_hyst_normal(
 rrm_cell_context_t *p_cell_context,
rrc_rrm_cell_reconfig_req_t           *p_rrc_rrm_cell_reconfig_req
);

/*! \fn rrm_return_et map_reconf_timecell_t_hyst_normal_for_son(const rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_timecell_t_hyst_normal_for_son(
const rrm_cell_context_t *p_cell_context,
rrc_rrm_cell_reconfig_req_t           *p_rrc_rrm_cell_reconfig_req
);

/*! \fn rrm_return_et map_reconf_timecell_t_evaluation(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_timecell_t_evaluation(
 rrm_cell_context_t *p_cell_context,
rrc_rrm_cell_reconfig_req_t           *p_rrc_rrm_cell_reconfig_req
);

/*! \fn rrm_return_et map_reconf_timecell_t_evaluation_for_son(const rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_timecell_t_evaluation_for_son(
const rrm_cell_context_t  *p_cell_context,
rrc_rrm_cell_reconfig_req_t           *p_rrc_rrm_cell_reconfig_req
);


/*! \fn rrm_return_et map_reconf_timecell_q_hyst_sf_high(rrm_cell_context_t *p_cell_context,
                              ,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t 
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_timecell_q_hyst_sf_high(
 rrm_cell_context_t *p_cell_context,
rrc_rrm_cell_reconfig_req_t           *p_rrc_rrm_cell_reconfig_req
);

/*! \fn rrm_return_et map_reconf_timecell_q_hyst_sf_high_for_son(const rrm_cell_context_t *p_cell_context
                              ,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_timecell_q_hyst_sf_high_for_son(
const rrm_cell_context_t    *p_cell_context,
rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
);

/*! \fn rrm_return_et map_reconf_timecell_q_hyst_sf_medium(rrm_cell_context_t *p_cell_context,
                                      ,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_timecell_q_hyst_sf_medium(
rrm_cell_context_t *p_cell_context,
rrc_rrm_cell_reconfig_req_t           *p_rrc_rrm_cell_reconfig_req
);
/*LTE_RRM_KLOCWORK_WARN_10_JULY_END*/

/*! \fn rrm_return_et map_reconf_timecell_q_hyst_sf_medium_for_son(const rrm_cell_context_t *p_cell_context
           ,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_timecell_q_hyst_sf_medium_for_son(
const rrm_cell_context_t              *p_cell_context,
rrc_rrm_cell_reconfig_req_t           *p_rrc_rrm_cell_reconfig_req
);

/*! \fn rrm_return_et map_reconf_timecell_reselect_info_comm(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 */
rrm_return_et
map_reconf_timecell_reselect_info_comm
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_timecell_reselect_info_comm_for_son(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief this function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_timecell_reselect_info_comm_for_son
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_cell_reselect_serv_freq_info(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 */
rrm_return_et
map_reconf_cell_reselect_serv_freq_info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_cell_reselect_serv_freq_info_for_son(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_cell_reselect_serv_freq_info_for_son
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_intra_freq_cell_reselect_info(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 */
rrm_return_et
map_reconf_intra_freq_cell_reselect_info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_intra_freq_cell_reselect_info_for_son(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_intra_freq_cell_reselect_info_for_son
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_sib_type_4_Info(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 */
rrm_return_et
map_reconf_sib_type_4_Info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_intra_freq_neigh_black_cell_list(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context request
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig request
 */
rrm_return_et
map_reconf_intra_freq_neigh_black_cell_list
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_csg_phy_cell_id_range(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 */
rrm_return_et
map_reconf_csg_phy_cell_id_range
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );


/*LTE_RRM_KLOCWORK_WARN_10_JULY_START*/
/*! \fn rrm_return_et map_reconf_sib_type_5_inter_freq_Info(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req,U8 inter_freq_list,U8 *p_neigh_count,U8 *p_black_count)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 *  \param inter_freq_list inter frequency List  
 *  \param p_neigh_count Neighbor count
 *  \param p_black_count Blacklisted Cell Count
 */
rrm_return_et
map_reconf_sib_type_5_inter_freq_Info
(
rrm_cell_context_t *p_cell_context,
rrc_rrm_cell_reconfig_req_t             *p_rrc_rrm_cell_reconfig_req,
U8					inter_freq_list,
U8					*p_neigh_count,
U8					*p_black_count
);

/*LTE_RRM_KLOCWORK_WARN_10_JULY_END*/

/*! \fn rrm_return_et map_reconf_sib_type_5_Info(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 */
rrm_return_et
map_reconf_sib_type_5_Info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_sib_type_5_Info_for_son(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_sib_type_5_Info_for_son
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_sib_type_6_Info(idle_mode_mobility_irat_utra_params_t *p_idle_mode_mobility_irat_utra_params,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_idle_mode_mobility_irat_utra_params idle_mode_mobility_irat_utra_params
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_sib_type_6_Info
(
 /* TDD Support SON Start */
 idle_mode_mobility_irat_utra_params_t *p_idle_mode_mobility_irat_utra_params,
 /* TDD Support SON End */
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_sib_type_6_Info_for_son(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_sib_type_6_Info_for_son
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_sib_type_7_Info(rrm_cell_context_t *p_cell_context, rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function Maps the cell setup params coming from MIF to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */ 
rrm_return_et
map_reconf_sib_type_7_Info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_sib_type_7_Info_for_son(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_sib_type_7_Info_for_son
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_sib_type_9_Info(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief this function build and send reconfig request to RRC
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 */
rrm_return_et
map_reconf_sib_type_9_Info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_non_broadcast_info(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_non_broadcast_info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_cell_parameters(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief this function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_cell_parameters
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et map_reconf_sync_signals(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function build and send reconfig request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_reconfig_req rrc_rrm_cell_reconfig_req_t
 */
rrm_return_et
map_reconf_sync_signals
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et fill_rrc_rrm_cell_delete_req(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_del_req_t  *p_rrc_rrm_cell_del_req)
 *  \brief this function build and send delete request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_del_req rrc_rrm_cell_del_req_t
 */
rrm_return_et
fill_rrc_rrm_cell_delete_req
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_del_req_t  *p_rrc_rrm_cell_del_req
 );
/* SPR 21660 changes start */

/*! \fn rrm_return_et build_and_send_cell_start_req( U16  transaction_id, U8 cell_index)
 *  \brief this function is to build and send cell start request 
 *  \param U16  transaction_id
 *  \param U8 cell_index
 */
rrm_return_et
build_and_send_cell_start_req
(
 U16  transaction_id,
 U8   cell_index
 );

/*! \fn rrm_return_et build_send_start_req( U16  transaction_id, U8 cell_index, rrc_rrm_cell_start_req_t    *p_rrc_rrm_cell_start_req)
 *  \brief this function is to build and send stop request to RRC
 *  \param U16  transaction_id
 *  \param U8 cell_index
 *  \param p_rrc_rrm_cell_start_req rrc_rrm_cell_start_req_t
 */
rrm_return_et
build_send_start_req
(
 U16  transaction_id,
 U8   cell_index,
 rrc_rrm_cell_start_req_t    *p_rrc_rrm_cell_start_req
 );

/* SPR 21660 changes end */

/*SPR 21660 changes start_Shivani*/

/*! \fn rrm_return_et build_and_send_cell_stop_req( U16  transaction_id, U8 cell_index)
 *  \brief this function is to build and send cell stop request 
 *  \param U16  transaction_id
 *  \param U8 cell_index
 */
rrm_return_et
build_and_send_cell_stop_req
(
 U16  transaction_id,
 U8   cell_index
 );

/*! \fn rrm_return_et build_send_stop_req(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_stop_req_t *p_rrc_rrm_cell_stop_req)
 *  \brief This function build and send stop request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_stop_req rrc_rrm_cell_stop_req_t
 */
rrm_return_et
build_send_stop_req
(
 U16  transaction_id,
 U8   cell_index,  
 rrc_rrm_cell_stop_req_t *p_rrc_rrm_cell_stop_req
 );

/*SPR 21660 changes end_Shivani*/
/*! \fn rrm_return_et build_send_rrc_del_req(rrm_cell_context_t  *p_cell_context,rrc_rrm_cell_del_req_t  *p_rrc_rrm_cell_del_req)
 *  \brief This function is to build and send stop request to RRC
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_rrc_rrm_cell_del_req rrc_rrm_cell_del_req_t
 */
rrm_return_et
build_send_rrc_del_req
(
 rrm_cell_context_t  *p_cell_context
 );

/*! \fn U8 rrm_get_ac_barring_factor(rrm_access_class_barring_info_t *access_class_barring_info)
 *  \brief This function is to get ac barring factor
 *  \param access_class_barring_info rrm_access_class_barring_info_t
 */
U8
rrm_get_ac_barring_factor
(
 rrm_access_class_barring_info_t *access_class_barring_info
 );

/*! \fn U8 rrm_get_ac_barring_time(rrm_access_class_barring_info_t *access_class_barring_info)
 *  \brief This function is used to get ac barring time
 *  \param access_class_barring_info rrm_access_class_barring_info_t
 */
U8
rrm_get_ac_barring_time
(
 rrm_access_class_barring_info_t *access_class_barring_info
 );

/*! \fn U8 rrm_get_ac_barring_for_special_ac(rrm_access_class_barring_info_t *access_class_barring_info)
 *  \brief This function is used to get ac barring for special ac
 *  \param access_class_barring_info rrm_access_class_barring_info_t
 */
U8
rrm_get_ac_barring_for_special_ac
(
 rrm_access_class_barring_info_t *access_class_barring_info
 );

/* UPDATED NRT INFO CHANGES START */ 
/*! \fn  rrm_return_et build_send_rrc_reconfig_req_for_updated_nrt_info(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req,U8 bit_mask)
 *  \brief This function is used to build and send RRC reconfig request for Updated NRT information
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig
 *  \param bit_mask Bitmask Value
 */
rrm_return_et
build_send_rrc_reconfig_req_for_updated_nrt_info
(
 rrm_cell_context_t *p_cell_context, 
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req,
 /*ut_bug_15011_start*/
 U16 bit_mask
 /*ut_bug_15011_end*/
 );


/*! \fn rrm_return_et map_reconf_broadcast_info_for_nrt(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req,U8 bit_mask)
 *  \brief This function is used Maps the reconfig broadcast information for NRT
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 *  \param bit_mask Bitmask Value
 */
rrm_return_et
map_reconf_broadcast_info_for_nrt
(
 rrm_cell_context_t *p_cell_context, 
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req,
 /*ut_bug_15011_start*/
 U16 bit_mask
 /*ut_bug_15011_end*/
 );

/*! \fn void rrm_fill_plmn_identity_for_enb_transfer(rrm_oam_cell_plmn_info_t *p_rrm_plmn_id,plmn_identity_t *p_enb_plmn_id)
 *  \brief This function Maps the updated nrt info coming from MIF to RRC
 *  \param p_rrm_plmn_id Cell PLMN identifier
 *  \param p_enb_plmn_id EnodeB PLMN Identifier
 */
void
rrm_fill_plmn_identity_for_enb_transfer
(
   rrm_oam_cell_plmn_info_t *p_rrm_plmn_id,
   plmn_identity_t *p_enb_plmn_id
   );

/*! \fn rrm_return_et build_and_send_s1ap_rrm_pws_resp(s1ap_rrm_pws_response_t *p_s1ap_rrm_pws_response,U8 trans_id)
 *  \brief This function Maps updated nrt info param coming from MIF to RRC
 *  \param p_s1ap_rrm_pws_response s1ap RRM PWS response
 *  \param trans_id transaction Identifier
 */
rrm_return_et
build_and_send_s1ap_rrm_pws_resp(
s1ap_rrm_pws_response_t *p_s1ap_rrm_pws_response,
U8                      trans_id
);

/* warning removal:prototype declaration added, 10/04/2012 */
/*! \fn rrm_return_et build_and_send_update_pws_list_resp(rrc_rrm_updated_pws_si_list_resp_t *p_rrc_rrm_updated_pws_si_list_resp,U8 trans_id)
 *  \brief This function Maps updated nrt info param coming from MIF to RRC
 *  \param p_rrc_rrm_updated_pws_si_list_resp Updated pws si list
 *  \param trans_id transaction identifier
 */
rrm_return_et
build_and_send_update_pws_list_resp(
rrc_rrm_updated_pws_si_list_resp_t *p_rrc_rrm_updated_pws_si_list_resp,
U8                      trans_id
);

/* warning removal:prototype declaration added, 10/04/2012 */
/*! \fn rrm_return_et build_and_send_s1ap_rrm_kill_resp(s1ap_rrm_kill_response_t *p_s1ap_rrm_kill_response,U8 trans_id)
 *  \brief This function is used build and send s1ap kill response
 *  \param p_s1ap_rrm_kill_response Kill Response
 *  \param trans_id Transaction Identifier
 */
rrm_return_et
build_and_send_s1ap_rrm_kill_resp(
s1ap_rrm_kill_response_t *p_s1ap_rrm_kill_response,
U8                      trans_id
);

/* UPDATED NRT INFO CHANGES END */

/* COMP_WARN_1_FEB:compilation warning removal */
/*! \fn rrm_return_et map_cell_setup_tdd_config(rrm_cell_context_t *cell_context, sib_type_1_Info_t*  p_sib_type_1_info)
 *  \brief This function is used map Cell Setup TDD config 
 *  \param cell_context Cell Context
 *  \param p_sib_type_1_info SIB type 1 information
 */
rrm_return_et
map_cell_setup_tdd_config
(
 rrm_cell_context_t *cell_context,
 sib_type_1_Info_t*  p_sib_type_1_info
);
/*! \fn rrm_return_et update_reconf_preambles_group_a_config(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function is used is used update reconfig preambles Group A config
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 */
rrm_return_et
update_reconf_preambles_group_a_config
(
 rrm_cell_context_t *p_cell_context, 
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/* UPDATED NRT INFO CHANGES END */

/*! \fn rrm_void_t fill_mac_config_scheduling_info_list(rrm_si_window_length_et si_window_length,rrc_si_transmission_info_list_t *p_mac_config_si_info,const rrm_scheduling_info_list_t   *p_rrm_scheduling_info)
 *  \brief This function MAC configuration scheduling information information
 *  \param  si_window_length window length 
 *  \param  p_mac_config_si_info MAC config sib information
 *  \param  p_rrm_scheduling_info scheduling information
 */
rrm_void_t
fill_mac_config_scheduling_info_list(rrm_si_window_length_et si_window_length,
                                     rrc_si_transmission_info_list_t   *p_mac_config_si_info,
                                     const rrm_scheduling_info_list_t   *p_rrm_scheduling_info);
 
/* CDMA2000 changes start */

/*! \fn rrm_return_et map_sib_type_8(rrm_cell_context_t *p_cell_context,sib_type_8_Info_t  *p_sib_type_8_Info)
 *  \brief This function is used map sib type 8
 *  \param p_cell_context rrm_cell_context_t
 *  \param p_sib_type_8_Info sib_type_8_Info_t
 */
rrm_return_et
map_sib_type_8
(
 rrm_cell_context_t *p_cell_context,
 sib_type_8_Info_t  *p_sib_type_8_Info
 );

/*! \fn rrm_void_t map_params_cdma2000_hrpd_1xrtt(rrm_cell_context_t  *p_rrm_cell_context,sib_type_8_Info_t   *p_sib_type_8_Info)
 *  \brief This function is used map params CDMA2000 HRPD 1xrtt
 *  \param p_rrm_cell_context rrm_cell_context_t
 *  \param p_ncl_params     ncl_params_t
 *  \param p_sib_type_8_Info sib_type_8_Info_t
 */
/* COMP_WARN_1_FEB:compilation warning removal */
rrm_void_t
map_params_cdma2000_hrpd_1xrtt
(
 rrm_cell_context_t  *p_rrm_cell_context,
 /* Start 7514 NRT Fix - SIB 8 */
 ncl_params_t        *p_ncl_params,
 /* End 7514 NRT Fix - SIB 8 */
 sib_type_8_Info_t   *p_sib_type_8_Info
 );

/*! \fn rrm_void_t map_cdma2000_hrpd(rrm_cell_context_t  *p_rrm_cell_context, sib_type_8_Info_t   *p_sib_type_8_Info)
 *  \brief This function is used for map CDMA2000 HRPD
 *  \param p_rrm_cell_context Cell Context
 *  \param p_sib_type_8_Info SIB type 8 information
 */
/* COMP_WARN_1_FEB:compilation warning removal */
rrm_void_t
map_cdma2000_hrpd
(
 rrm_cell_context_t  *p_rrm_cell_context,
 sib_type_8_Info_t   *p_sib_type_8_Info
 );
/*! \fn rrm_void_t map_cdma2000_1xrtt(rrm_cell_context_t  *p_rrm_cell_context,sib_type_8_Info_t   *p_sib_type_8_Info)
 *  \brief This function is to map CDMA2000 1xrtt
 *  \param p_rrm_cell_context Cell Context
 *  \param p_sib_type_8_Info SIB type 8 information
 */
/* COMP_WARN_1_FEB:compilation warning removal */
rrm_void_t
map_cdma2000_1xrtt
(
 rrm_cell_context_t  *p_rrm_cell_context,
 sib_type_8_Info_t   *p_sib_type_8_Info
 );

/*! \fn rrm_void_t map_reselection_cdma2000_1xrtt_params(rrmcm_idle_mode_mobility_irat_cdma_params_t *p_irat_cdma_params, params_1_xrtt_t *p_params_1_xrtt)
 *  \brief This function is for reselection CDM2000 1xrtt parameters
 *  \param p_irat_cdma_params rrmcm_idle_mode_mobility_irat_cdma_params_t
 *  \param p_params_1_xrtt params_1_xrtt_t
 */
/* COMP_WARN_1_FEB:compilation warning removal */
rrm_void_t
map_reselection_cdma2000_1xrtt_params
(
 rrmcm_idle_mode_mobility_irat_cdma_params_t    *p_irat_cdma_params,
 params_1_xrtt_t                                *p_params_1_xrtt
 );

/*! \fn rrm_void_t map_bandclass_list(rrmcm_rmif_band_class_list_cdma2000_t  *p_rrmcm_band_class_list,band_class_list_cdma2000_t *P_rrc_band_class_list)
 *  \brief This function maps bandclass list
 *  \param p_rrmcm_band_class_list rrmcm_rmif_band_class_list_cdma2000_t
 *  \param P_rrc_band_class_list band_class_list_cdma2000_t
 */
/* COMP_WARN_1_FEB:compilation warning removal */
rrm_void_t
map_bandclass_list
(
 rrmcm_rmif_band_class_list_cdma2000_t  *p_rrmcm_band_class_list,
 band_class_list_cdma2000_t             *P_rrc_band_class_list
 );

/*! \fn rrm_void_t map_reselection_cdma2000_hrpd_params(params_hrpd_t *p_params_hrpd,rrmcm_idle_mode_mobility_irat_cdma_params_t *p_irat_cdma_params)
 *  \brief This function is used for MAP reselection CDMA2000 HRPD parameters
 *  \param p_params_hrpd HRPD params
 *  \param p_irat_cdma_params Inter RAT CDMA params
 */
/* COMP_WARN_1_FEB:compilation warning removal */
rrm_void_t
map_reselection_cdma2000_hrpd_params
(
 params_hrpd_t                                *p_params_hrpd,
 rrmcm_idle_mode_mobility_irat_cdma_params_t  *p_irat_cdma_params
 );

/*! \fn rrm_void_t map_hrpd_1xrtt_ncl_list(inter_rat_ncl_t *p_inter_rat_ncl,sib_type_8_Info_t   *p_sib_type_8_Info)
 *  \brief This function is used to MAP HRPD 1xrtt NCL list
 *  \param rrm_cell_context_t RRM Cell Context
 *  \param p_inter_rat_ncl Inter RAT NCL
 *  \param p_sib_type_8_Info SIB type 8 Info
 */
/* COMP_WARN_1_FEB:compilation warning removal */
rrm_void_t
map_hrpd_1xrtt_ncl_list
(
 rrm_cell_context_t  *p_rrm_cell_context,
 /* Start 7514 NRT Fix - SIB 8 */
 inter_rat_ncl_t     *p_inter_rat_ncl,
 /* End 7514 NRT Fix - SIB 8 */
 sib_type_8_Info_t   *p_sib_type_8_Info
 );

/*! \fn void map_cell_reselection_params_cdma2000(
     neigh_cell_list_cdma2000_t         *p_neigh_cell_list_cdma2000,
      neigh_cell_list_cdma2000_v920_t    *p_cell_resel_cdma2000_v920,
       inter_rat_ncl_t                    *p_inter_rat_ncl,
       U8                                  cdma2000_freq_cells,
        sib_type_8_Info_t                  *p_sib_type_8_Info)
 *  \brief This funtion is used to map cell reselection parameters for cdma2000
 *  \param p_neigh_cell_list_cdma2000 neigh cell list CDMA 2000
 *  \param p_cell_resel_cdma2000_v920 cell reselection CDMA 2000 v920 
 *  \param p_inter_rat_ncl inter RAT NCL params
 *  \param cdma2000_freq_cells CDMA2000 freq cells 
 *  \param p_sib_type_8_Info SIB type 8 Info
 */
void 
map_cell_reselection_params_cdma2000
(
 neigh_cell_list_cdma2000_t         *p_neigh_cell_list_cdma2000,
 neigh_cell_list_cdma2000_v920_t    *p_cell_resel_cdma2000_v920,
 inter_rat_ncl_t                    *p_inter_rat_ncl,
 U8                                  cdma2000_freq_cells,
 sib_type_8_Info_t                  *p_sib_type_8_Info
 );

/*! \fn rrm_void_t map_csfb_reg_1_xrtt_params(rrm_cell_context_t *p_rrm_cell_context,csfb_reg_param_1_xrtt_t  *p_csfb_reg_param_1_xrtt)
 *  \brief
 *  \param p_rrm_cell_context Cell Context
 *  \param p_csfb_reg_param_1_xrtt CSFB register param 1 xrtt
 */
/* COMP_WARN_1_FEB:compilation warning removal */
rrm_void_t
map_csfb_reg_1_xrtt_params
(
 /* Fix for reconfig 27/06/2013 */
 rrmcm_idle_mode_mobility_irat_cdma_params_t    *p_irat_cdma_params,
 csfb_reg_param_1_xrtt_t  *p_csfb_reg_param_1_xrtt
 );

/*! \fn rrm_void_t map_pre_reg_info_hrpd(rrmcm_idle_mode_mobility_irat_cdma_params_t  *p_irat_cdma_params,params_hrpd_t    *p_params_hrpd)
 *  \brief This function is used for mapping pre Reg information HRPD
 *  \param p_irat_cdma_params rrmcm_idle_mode_mobility_irat_cdma_params_t
 *  \param p_params_hrpd params_hrpd_t
 */
/* COMP_WARN_1_FEB:compilation warning removal */
rrm_void_t
map_pre_reg_info_hrpd
(
 rrmcm_idle_mode_mobility_irat_cdma_params_t  *p_irat_cdma_params,
 params_hrpd_t    *p_params_hrpd
 );

/*! \fn rrm_void_t map_ac_barring_config_1_xrtt_r9(rrmcm_rmif_ac_barring_config_1_xrtt_r9_t  *p_rrm_ac_barring_config_1_xrtt_r9,ac_barring_config_1_xrtt_r9_t    *p_rrc_ac_barring_config_1_xrtt_r9)
 *  \brief This function Access Barring config 1 xrtt R9
 *  \param p_rrm_ac_barring_config_1_xrtt_r9 access barring config 1 xrtt r9
 *  \param p_rrc_ac_barring_config_1_xrtt_r9 ac_barring_config_1_xrtt_r9_t
 */
/* COMP_WARN_1_FEB:compilation warning removal */
rrm_void_t
map_ac_barring_config_1_xrtt_r9
(
 rrmcm_rmif_ac_barring_config_1_xrtt_r9_t  *p_rrm_ac_barring_config_1_xrtt_r9,
 ac_barring_config_1_xrtt_r9_t    *p_rrc_ac_barring_config_1_xrtt_r9
 );

/* CDMA2000 changes end */

/* COMP_WARN_1_FEB:compilation warning removal */
/*! \fn rrm_return_et update_reconf_preambles_group_a_config(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function is used to update reconfig preambles group A Config 
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 */
rrm_return_et
update_reconf_preambles_group_a_config
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );
/*! \fn rrm_return_et update_reconf_ra_supervision_info(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function is used to update reconfig RA supervision information
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 */
rrm_return_et
update_reconf_ra_supervision_info
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );
/*! \fn rrm_return_et update_reconf_ra_resp_win_size ( const rrmcm_rmif_cell_reconfig_req_t  *p_reconfig_req_from_mif,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function is used to update reconfig RA response window size
 *  \param p_reconfig_req_from_mif Cell Reconfig request from MIF 
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 */
rrm_return_et
update_reconf_ra_resp_win_size
(
 const rrmcm_rmif_cell_reconfig_req_t  *p_reconfig_req_from_mif,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );

/*! \fn rrm_return_et update_reconf_preambles_group_a_config(rrm_cell_context_t *p_cell_context,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function is used update reconfig preambles group A info
 *  \param p_cell_context Cell Context
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 */
rrm_return_et
update_reconf_preambles_group_a_config
(
 rrm_cell_context_t *p_cell_context,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );
/*! \fn rrm_return_et update_reconf_mac_cont_resol_timer(const rrmcm_rmif_cell_reconfig_req_t *p_reconfig_req_from_mif,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function is used for update reconfig MAC cont resolution timer
 *  \param p_reconfig_req_from_mif Reconfig Request from MIF
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 */
rrm_return_et
update_reconf_mac_cont_resol_timer
(
 const rrmcm_rmif_cell_reconfig_req_t  *p_reconfig_req_from_mif,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
  );
/*! \fn rrm_return_et update_reconf_preambles_group_b_msg_power_offset(const rrmcm_rmif_cell_reconfig_req_t *p_reconfig_req_from_mif,rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function is used to update reconfig preambles group b msg power offset 
 *  \param p_reconfig_req_from_mif Reconfig Request from MIF
 *  \param p_rrc_rrm_cell_reconfig_req Cell Reconfig Request
 */
rrm_return_et
update_reconf_preambles_group_b_msg_power_offset
(
 const rrmcm_rmif_cell_reconfig_req_t  *p_reconfig_req_from_mif,
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );
/*! \fn rrm_void_t rrm_fill_x2_gb_cid(rrm_cell_global_id_eutra_t *p_x2_gb_cid,rrm_global_cell_id_t *p_rrm_gb_cid)
 *  \brief This function is used to fill x2 global cid
 *  \param p_x2_gb_cid Cell Global Eutran Cell ID 
 *  \param p_rrm_gb_cid Global Cell ID
 */
rrm_void_t rrm_fill_x2_gb_cid
(
 rrm_cell_global_id_eutra_t  *p_x2_gb_cid,
 rrm_global_cell_id_t        *p_rrm_gb_cid);
/*! \fn rrm_return_et update_reconf_sib_type_8_Info(ran_t *p_ran_info, rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req)
 *  \brief This function is used to update reconfig sib type 8 information
 *  \param p_ran_info RAN information
 *  \param p_rrc_rrm_cell_reconfig_req Cell reconfig request
 */
rrm_return_et
update_reconf_sib_type_8_Info
(
    rrm_cell_context_t *p_cell_context,
    /* Fix for reconfig 27/06/2013 */
    rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
);

/*! \fn rrm_return_et build_and_send_rrm_x2ap_rrm_rsu_start_req(rrm_meas_rpt_config_info_t *p_cell_info,U32 report_characteristics,U8 report_periodicity)
 *  \brief This function is used to build and send rrm x2ap rsu start request
 *  \param p_cell_info Cell information
 *  \param report_characteristics Report Characteristics
 *  \param report_periodicity Report Periodicity
 */
rrm_return_et build_and_send_rrm_x2ap_rrm_rsu_start_req
(
 rrm_meas_rpt_config_info_t *p_cell_info,
 U32 report_characteristics,
 U8 report_periodicity
);
rrm_void_t
map_cdma2000_cell_param
(
  rrmcm_rmif_cdma2000_cell_param_t  *p_cdma2000_cell_param_cell,
    cdma2000_cell_param_t             *p_cdma2000_cell_param_rrc
    );

/*RIM changes start*/
rrm_return_et
rrm_send_rim_ack_to_l3
(
 rrm_cell_context_t  *p_cell_context,
 rim_information_ack_t    *p_rrm_rim_info_ack
 );

rrm_return_et
rrm_send_rim_req
(
 rrm_cell_context_t  *p_cell_context,
 rim_information_req_t    *p_rrm_rim_info_req
 );
/*RIM changes end*/
/*! \fn rrm_void_t rrm_fill_x2_gb_enb_id(x2_gb_enb_id_t *p_x2_gb_enb_id,rrm_x2_gb_enb_id_t *p_rrm_gb_enb_id)
 *  \brief This function is used to fill x2 global enodeB ID
 *  \param p_x2_gb_enb_id Pointer of x2_gb_enb_id_t
 *  \param p_rrm_gb_enb_id Pointer of rrm_x2_gb_enb_id_t
 */
rrm_void_t rrm_fill_x2_gb_enb_id
(
 x2_gb_enb_id_t     *p_x2_gb_enb_id,
 rrm_x2_gb_enb_id_t *p_rrm_gb_enb_id
 );
/*! \fn rrm_void_t rrm_get_window_lenth_in_ms(rrm_si_window_length_et si_window_length,U8 *p_si_window_length_in_ms)
 *  \brief This function is used to get window length in MS
 *  \param si_window_length window length enum
 *  \param p_si_window_length_in_ms Pointer to window size
 */
rrm_void_t
rrm_get_window_lenth_in_ms
(
  rrm_si_window_length_et si_window_length , 
  U8 *p_si_window_length_in_ms
);

/*DYNAMIC SIB SCHEDULING START*/
rrm_void_t
rrm_get_rrm_rrc_si_window_length
(
  U8 si_window_length_in_ms ,
    rrm_si_window_length_et *p_si_window_length
);
/*DYNAMIC SIB SCHEDULING END*/
void
rrm_fill_plmn_identity_for_enb_load_req
(
    rrm_oam_cell_plmn_info_t *p_plmn_info,
     U8                      *p_enb_plmn_id
);

/*DYNAMIC ICIC CHANGES START*/

rrm_void_t build_and_send_x2ap_rrm_hii_req 
(
 rrm_cell_context_t                 *p_cell_context,
 rrm_oam_eutran_global_cell_id_t    *p_eutran_global_cell_id,
 U16                                num_cell,
 rrm_enb_type_et                    enodeb_type
);

/*DYNAMIC ICIC CHANGES END*/

/* BUG 680 FIX START*/
rrm_return_et rrm_map_scheduling_info_list(
    scheduling_info_list_t *p_rrc_scheduling_info_list,
    rrm_scheduling_info_list_t  *p_rrm_scheduling_info_list
    );
rrm_return_et rrm_map_sib_mapping_info(
    sib_mapping_info_t *p_rrc_sib_mapping_info,
    rrm_sib_mapping_info_t  *p_rrm_sib_mapping_info
    );
/* BUG 680 FIX END */
/* BUG_371 FIX START */ 
rrm_void_t
clear_sib_mapping_info_in_sib1
(
  scheduling_info_list_t *p_scheduling_info_list,
  rrm_sib_type_et         sib_type
);
/* BUG_371 FIX END */ 
rrm_return_et
map_reconf_sib_type_8_Info
(
 rrm_cell_context_t *p_cell_context,
/* Start 7514 NRT Fix - SIB 8 */
 ncl_params_t                   *p_ncl_params,
/* End 7514 NRT Fix - SIB 8 */
 rrc_rrm_cell_reconfig_req_t *p_rrc_rrm_cell_reconfig_req
 );
/* eICIC_PHASE_1_2_CHANGES_START */
rrm_void_t
rrm_fill_abs_pattern_info_fdd
(
    x2ap_rrm_load_information_req_t    *p_x2ap_rrm_load_info_req , 
    rrm_cell_context_t                 *p_cell_context 
);

rrm_void_t
rrm_fill_abs_pattern_info_tdd
(
    x2ap_rrm_load_information_req_t    *p_x2ap_rrm_load_info_req , 
    rrm_cell_context_t                 *p_cell_context 
);
/* eICIC_PHASE_1_2_CHANGES_END */




#endif /*_RRM_RRC_CELL_INTF_H__*/
