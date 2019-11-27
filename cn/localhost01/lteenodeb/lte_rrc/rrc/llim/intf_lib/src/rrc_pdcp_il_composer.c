/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2014 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: $
 ****************************************************************************
 *
 *  File Description : The file rrc_pdcp_il_composer.c contains the definitions 
 *                     of rrc-pdcp interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/

#include "rrc_cp_common.h"
#include "rrc_pdcp_il_composer.h"
#include "rrc_pdcp_intf.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"
#include "rrc_msg_mgmt.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_length_t
rrc_il_get_rrc_pdcp_cr_srb_entity_len
(
    rrc_pdcp_cr_srb_entity_t *p_rrc_pdcp_cr_srb_entity
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_cr_srb_entity_tlv_len
(
    rrc_pdcp_cr_srb_entity_t *p_rrc_pdcp_cr_srb_entity

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_cr_srb_entity
(
    U8  **pp_buffer,
    rrc_pdcp_cr_srb_entity_t *p_rrc_pdcp_cr_srb_entity
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_rohc_len
(
    rrc_pdcp_config_rohc_t *p_rrc_pdcp_config_rohc
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_rohc_tlv_len
(
    rrc_pdcp_config_rohc_t *p_rrc_pdcp_config_rohc

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_rohc
(
    U8  **pp_buffer,
    rrc_pdcp_config_rohc_t *p_rrc_pdcp_config_rohc
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_disc_timer_len
(
    rrc_pdcp_config_disc_timer_t *p_rrc_pdcp_config_disc_timer
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_disc_timer_tlv_len
(
    rrc_pdcp_config_disc_timer_t *p_rrc_pdcp_config_disc_timer

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_disc_timer
(
    U8  **pp_buffer,
    rrc_pdcp_config_disc_timer_t *p_rrc_pdcp_config_disc_timer
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_sn_size_len
(
    rrc_pdcp_config_sn_size_t *p_rrc_pdcp_config_sn_size
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_sn_size_tlv_len
(
    rrc_pdcp_config_sn_size_t *p_rrc_pdcp_config_sn_size

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_sn_size
(
    U8  **pp_buffer,
    rrc_pdcp_config_sn_size_t *p_rrc_pdcp_config_sn_size
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_st_rep_required_len
(
    rrc_pdcp_config_st_rep_required_t *p_rrc_pdcp_config_st_rep_required
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_st_rep_required_tlv_len
(
    rrc_pdcp_config_st_rep_required_t *p_rrc_pdcp_config_st_rep_required

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_st_rep_required
(
    U8  **pp_buffer,
    rrc_pdcp_config_st_rep_required_t *p_rrc_pdcp_config_st_rep_required
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_ue_st_rep_required_len
(
    rrc_pdcp_config_ue_st_rep_required_t *p_rrc_pdcp_config_ue_st_rep_required
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_ue_st_rep_required_tlv_len
(
    rrc_pdcp_config_ue_st_rep_required_t *p_rrc_pdcp_config_ue_st_rep_required

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_ue_st_rep_required
(
    U8  **pp_buffer,
    rrc_pdcp_config_ue_st_rep_required_t *p_rrc_pdcp_config_ue_st_rep_required
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_cr_drb_entity_len
(
    rrc_pdcp_cr_drb_entity_t *p_rrc_pdcp_cr_drb_entity
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_cr_drb_entity_tlv_len
(
    rrc_pdcp_cr_drb_entity_t *p_rrc_pdcp_cr_drb_entity

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_cr_drb_entity
(
    U8  **pp_buffer,
    rrc_pdcp_cr_drb_entity_t *p_rrc_pdcp_cr_drb_entity
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_int_len
(
    rrc_pdcp_config_int_t *p_rrc_pdcp_config_int
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_int_tlv_len
(
    rrc_pdcp_config_int_t *p_rrc_pdcp_config_int

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_int
(
    U8  **pp_buffer,
    rrc_pdcp_config_int_t *p_rrc_pdcp_config_int
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_srb_ciph_len
(
    rrc_pdcp_config_srb_ciph_t *p_rrc_pdcp_config_srb_ciph
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_srb_ciph_tlv_len
(
    rrc_pdcp_config_srb_ciph_t *p_rrc_pdcp_config_srb_ciph

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_srb_ciph
(
    U8  **pp_buffer,
    rrc_pdcp_config_srb_ciph_t *p_rrc_pdcp_config_srb_ciph
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_drb_ciph_len
(
    rrc_pdcp_config_drb_ciph_t *p_rrc_pdcp_config_drb_ciph
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_drb_ciph_tlv_len
(
    rrc_pdcp_config_drb_ciph_t *p_rrc_pdcp_config_drb_ciph

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_drb_ciph
(
    U8  **pp_buffer,
    rrc_pdcp_config_drb_ciph_t *p_rrc_pdcp_config_drb_ciph
);

static
rrc_length_t
rrc_il_get_rrc_ue_inactive_time_len
(
    rrc_ue_inactive_time_t *p_rrc_ue_inactive_time
);

static
rrc_length_t
rrc_il_get_rrc_ue_inactive_time_tlv_len
(
    rrc_ue_inactive_time_t *p_rrc_ue_inactive_time

);

static
rrc_return_et
rrc_il_compose_rrc_ue_inactive_time
(
    U8  **pp_buffer,
    rrc_ue_inactive_time_t *p_rrc_ue_inactive_time
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_ho_triggered_len
(
    rrc_pdcp_ho_triggered_t *p_rrc_pdcp_ho_triggered
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_ho_triggered_tlv_len
(
    rrc_pdcp_ho_triggered_t *p_rrc_pdcp_ho_triggered

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_ho_triggered
(
    U8  **pp_buffer,
    rrc_pdcp_ho_triggered_t *p_rrc_pdcp_ho_triggered
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_dl_setup_info_len
(
    rrc_pdcp_sps_dl_setup_info_t *p_rrc_pdcp_sps_dl_setup_info
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_dl_setup_info_tlv_len
(
    rrc_pdcp_sps_dl_setup_info_t *p_rrc_pdcp_sps_dl_setup_info

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_sps_dl_setup_info
(
    U8  **pp_buffer,
    rrc_pdcp_sps_dl_setup_info_t *p_rrc_pdcp_sps_dl_setup_info
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_dl_info_len
(
    rrc_pdcp_sps_dl_info_t *p_rrc_pdcp_sps_dl_info
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_dl_info_tlv_len
(
    rrc_pdcp_sps_dl_info_t *p_rrc_pdcp_sps_dl_info

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_sps_dl_info
(
    U8  **pp_buffer,
    rrc_pdcp_sps_dl_info_t *p_rrc_pdcp_sps_dl_info
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_ul_setup_info_len
(
    rrc_pdcp_sps_ul_setup_info_t *p_rrc_pdcp_sps_ul_setup_info
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_ul_setup_info_tlv_len
(
    rrc_pdcp_sps_ul_setup_info_t *p_rrc_pdcp_sps_ul_setup_info

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_sps_ul_setup_info
(
    U8  **pp_buffer,
    rrc_pdcp_sps_ul_setup_info_t *p_rrc_pdcp_sps_ul_setup_info
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_ul_info_len
(
    rrc_pdcp_sps_ul_info_t *p_rrc_pdcp_sps_ul_info
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_ul_info_tlv_len
(
    rrc_pdcp_sps_ul_info_t *p_rrc_pdcp_sps_ul_info

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_sps_ul_info
(
    U8  **pp_buffer,
    rrc_pdcp_sps_ul_info_t *p_rrc_pdcp_sps_ul_info
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_config_len
(
    rrc_pdcp_sps_config_t *p_rrc_pdcp_sps_config
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_config_tlv_len
(
    rrc_pdcp_sps_config_t *p_rrc_pdcp_sps_config

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_sps_config
(
    U8  **pp_buffer,
    rrc_pdcp_sps_config_t *p_rrc_pdcp_sps_config
);

static
rrc_length_t
rrc_il_get_rrc_mac_i_msg_len
(
    rrc_mac_i_msg_t *p_rrc_mac_i_msg
);

static
rrc_length_t
rrc_il_get_rrc_mac_i_msg_tlv_len
(
    rrc_mac_i_msg_t *p_rrc_mac_i_msg

);

static
rrc_return_et
rrc_il_compose_rrc_mac_i_msg
(
    U8  **pp_buffer,
    rrc_mac_i_msg_t *p_rrc_mac_i_msg
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_ue_inactivity_timer_len
(
    rrc_pdcp_ue_inactivity_timer_t *p_rrc_pdcp_ue_inactivity_timer
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_ue_inactivity_timer_tlv_len
(
    rrc_pdcp_ue_inactivity_timer_t *p_rrc_pdcp_ue_inactivity_timer

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_ue_inactivity_timer
(
    U8  **pp_buffer,
    rrc_pdcp_ue_inactivity_timer_t *p_rrc_pdcp_ue_inactivity_timer
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_cr_srb_entity_error_len
(
    rrc_pdcp_cr_srb_entity_error_t *p_rrc_pdcp_cr_srb_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_cr_srb_entity_error_tlv_len
(
    rrc_pdcp_cr_srb_entity_error_t *p_rrc_pdcp_cr_srb_entity_error

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_cr_srb_entity_error
(
    U8  **pp_buffer,
    rrc_pdcp_cr_srb_entity_error_t *p_rrc_pdcp_cr_srb_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_cr_drb_entity_error_len
(
    rrc_pdcp_cr_drb_entity_error_t *p_rrc_pdcp_cr_drb_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_cr_drb_entity_error_tlv_len
(
    rrc_pdcp_cr_drb_entity_error_t *p_rrc_pdcp_cr_drb_entity_error

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_cr_drb_entity_error
(
    U8  **pp_buffer,
    rrc_pdcp_cr_drb_entity_error_t *p_rrc_pdcp_cr_drb_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_mac_i_len
(
    rrc_mac_i_t *p_rrc_mac_i
);

static
rrc_length_t
rrc_il_get_rrc_mac_i_tlv_len
(
    rrc_mac_i_t *p_rrc_mac_i

);

static
rrc_return_et
rrc_il_compose_rrc_mac_i
(
    U8  **pp_buffer,
    rrc_mac_i_t *p_rrc_mac_i
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_rcfg_srb_entity_len
(
    rrc_pdcp_rcfg_srb_entity_t *p_rrc_pdcp_rcfg_srb_entity
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_rcfg_srb_entity_tlv_len
(
    rrc_pdcp_rcfg_srb_entity_t *p_rrc_pdcp_rcfg_srb_entity

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_rcfg_srb_entity
(
    U8  **pp_buffer,
    rrc_pdcp_rcfg_srb_entity_t *p_rrc_pdcp_rcfg_srb_entity
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_lwa_config_lwa_bearer_type_len
(
    rrc_pdcp_config_lwa_config_lwa_bearer_type_t *p_rrc_pdcp_config_lwa_config_lwa_bearer_type
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_lwa_config_lwa_bearer_type_tlv_len
(
    rrc_pdcp_config_lwa_config_lwa_bearer_type_t *p_rrc_pdcp_config_lwa_config_lwa_bearer_type

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_lwa_config_lwa_bearer_type
(
    U8  **pp_buffer,
    rrc_pdcp_config_lwa_config_lwa_bearer_type_t *p_rrc_pdcp_config_lwa_config_lwa_bearer_type
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_lwa_len
(
    rrc_pdcp_config_lwa_t *p_rrc_pdcp_config_lwa
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_lwa_tlv_len
(
    rrc_pdcp_config_lwa_t *p_rrc_pdcp_config_lwa

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_lwa
(
    U8  **pp_buffer,
    rrc_pdcp_config_lwa_t *p_rrc_pdcp_config_lwa
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_rcfg_drb_entity_len
(
    rrc_pdcp_rcfg_drb_entity_t *p_rrc_pdcp_rcfg_drb_entity
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_rcfg_drb_entity_tlv_len
(
    rrc_pdcp_rcfg_drb_entity_t *p_rrc_pdcp_rcfg_drb_entity

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_rcfg_drb_entity
(
    U8  **pp_buffer,
    rrc_pdcp_rcfg_drb_entity_t *p_rrc_pdcp_rcfg_drb_entity
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_sec_offset_len
(
    rrc_pdcp_sec_offset_t *p_rrc_pdcp_sec_offset
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_sec_offset_tlv_len
(
    rrc_pdcp_sec_offset_t *p_rrc_pdcp_sec_offset

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_sec_offset
(
    U8  **pp_buffer,
    rrc_pdcp_sec_offset_t *p_rrc_pdcp_sec_offset
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_del_srb_entity_len
(
    rrc_pdcp_del_srb_entity_t *p_rrc_pdcp_del_srb_entity
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_del_srb_entity_tlv_len
(
    rrc_pdcp_del_srb_entity_t *p_rrc_pdcp_del_srb_entity

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_del_srb_entity
(
    U8  **pp_buffer,
    rrc_pdcp_del_srb_entity_t *p_rrc_pdcp_del_srb_entity
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_del_drb_entity_len
(
    rrc_pdcp_del_drb_entity_t *p_rrc_pdcp_del_drb_entity
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_del_drb_entity_tlv_len
(
    rrc_pdcp_del_drb_entity_t *p_rrc_pdcp_del_drb_entity

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_del_drb_entity
(
    U8  **pp_buffer,
    rrc_pdcp_del_drb_entity_t *p_rrc_pdcp_del_drb_entity
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_del_srb_entity_error_len
(
    rrc_pdcp_del_srb_entity_error_t *p_rrc_pdcp_del_srb_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_del_srb_entity_error_tlv_len
(
    rrc_pdcp_del_srb_entity_error_t *p_rrc_pdcp_del_srb_entity_error

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_del_srb_entity_error
(
    U8  **pp_buffer,
    rrc_pdcp_del_srb_entity_error_t *p_rrc_pdcp_del_srb_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_del_drb_entity_error_len
(
    rrc_pdcp_del_drb_entity_error_t *p_rrc_pdcp_del_drb_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_del_drb_entity_error_tlv_len
(
    rrc_pdcp_del_drb_entity_error_t *p_rrc_pdcp_del_drb_entity_error

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_del_drb_entity_error
(
    U8  **pp_buffer,
    rrc_pdcp_del_drb_entity_error_t *p_rrc_pdcp_del_drb_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_rcfg_srb_entity_error_len
(
    rrc_pdcp_rcfg_srb_entity_error_t *p_rrc_pdcp_rcfg_srb_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_rcfg_srb_entity_error_tlv_len
(
    rrc_pdcp_rcfg_srb_entity_error_t *p_rrc_pdcp_rcfg_srb_entity_error

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_rcfg_srb_entity_error
(
    U8  **pp_buffer,
    rrc_pdcp_rcfg_srb_entity_error_t *p_rrc_pdcp_rcfg_srb_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_rcfg_drb_entity_error_len
(
    rrc_pdcp_rcfg_drb_entity_error_t *p_rrc_pdcp_rcfg_drb_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_rcfg_drb_entity_error_tlv_len
(
    rrc_pdcp_rcfg_drb_entity_error_t *p_rrc_pdcp_rcfg_drb_entity_error

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_rcfg_drb_entity_error
(
    U8  **pp_buffer,
    rrc_pdcp_rcfg_drb_entity_error_t *p_rrc_pdcp_rcfg_drb_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_data_status_error_len
(
    rrc_pdcp_data_status_error_t *p_rrc_pdcp_data_status_error
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_data_status_error_tlv_len
(
    rrc_pdcp_data_status_error_t *p_rrc_pdcp_data_status_error

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_data_status_error
(
    U8  **pp_buffer,
    rrc_pdcp_data_status_error_t *p_rrc_pdcp_data_status_error
);

static
rrc_length_t
rrc_il_get_rrc_lossy_ho_required_len
(
    rrc_lossy_ho_required_t *p_rrc_lossy_ho_required
);

static
rrc_length_t
rrc_il_get_rrc_lossy_ho_required_tlv_len
(
    rrc_lossy_ho_required_t *p_rrc_lossy_ho_required

);

static
rrc_return_et
rrc_il_compose_rrc_lossy_ho_required
(
    U8  **pp_buffer,
    rrc_lossy_ho_required_t *p_rrc_lossy_ho_required
);

static
rrc_length_t
rrc_il_get_ul_rcv_sn_val_len
(
    ul_rcv_sn_val_t *p_ul_rcv_sn_val
);

static
rrc_length_t
rrc_il_get_ul_rcv_sn_val_tlv_len
(
    ul_rcv_sn_val_t *p_ul_rcv_sn_val

);

static
rrc_return_et
rrc_il_compose_ul_rcv_sn_val
(
    U8  **pp_buffer,
    ul_rcv_sn_val_t *p_ul_rcv_sn_val
);

static
rrc_length_t
rrc_il_get_rrc_lc_sn_hfn_status_len
(
    rrc_lc_sn_hfn_status_t *p_rrc_lc_sn_hfn_status
);

static
rrc_length_t
rrc_il_get_rrc_lc_sn_hfn_status_tlv_len
(
    rrc_lc_sn_hfn_status_t *p_rrc_lc_sn_hfn_status

);

static
rrc_return_et
rrc_il_compose_rrc_lc_sn_hfn_status
(
    U8  **pp_buffer,
    rrc_lc_sn_hfn_status_t *p_rrc_lc_sn_hfn_status
);

static
rrc_length_t
rrc_il_get_rrc_message_len
(
    rrc_message_t *p_rrc_message
);

static
rrc_length_t
rrc_il_get_rrc_message_tlv_len
(
    rrc_message_t *p_rrc_message

);

static
rrc_return_et
rrc_il_compose_rrc_message
(
    U8  **pp_buffer,
    rrc_message_t *p_rrc_message
);

static
rrc_length_t
rrc_il_get_rrc_msg_authentication_code_len
(
    rrc_msg_authentication_code_t *p_rrc_msg_authentication_code
);

static
rrc_length_t
rrc_il_get_rrc_msg_authentication_code_tlv_len
(
    rrc_msg_authentication_code_t *p_rrc_msg_authentication_code

);

static
rrc_return_et
rrc_il_compose_rrc_msg_authentication_code
(
    U8  **pp_buffer,
    rrc_msg_authentication_code_t *p_rrc_msg_authentication_code
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_rb_entity_len
(
    rrc_pdcp_rb_entity_t *p_rrc_pdcp_rb_entity
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_rb_entity_tlv_len
(
    rrc_pdcp_rb_entity_t *p_rrc_pdcp_rb_entity

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_rb_entity
(
    U8  **pp_buffer,
    rrc_pdcp_rb_entity_t *p_rrc_pdcp_rb_entity
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_rb_entity_error_len
(
    rrc_pdcp_rb_entity_error_t *p_rrc_pdcp_rb_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_rb_entity_error_tlv_len
(
    rrc_pdcp_rb_entity_error_t *p_rrc_pdcp_rb_entity_error

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_rb_entity_error
(
    U8  **pp_buffer,
    rrc_pdcp_rb_entity_error_t *p_rrc_pdcp_rb_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_drb_list_len
(
    rrc_pdcp_drb_list_t *p_rrc_pdcp_drb_list
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_drb_list_tlv_len
(
    rrc_pdcp_drb_list_t *p_rrc_pdcp_drb_list

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_drb_list
(
    U8  **pp_buffer,
    rrc_pdcp_drb_list_t *p_rrc_pdcp_drb_list
);

static
rrc_length_t
rrc_il_get_countMSB_uplink_len
(
    countMSB_uplink_t *p_countMSB_uplink
);

static
rrc_length_t
rrc_il_get_countMSB_uplink_tlv_len
(
    countMSB_uplink_t *p_countMSB_uplink

);

static
rrc_return_et
rrc_il_compose_countMSB_uplink
(
    U8  **pp_buffer,
    countMSB_uplink_t *p_countMSB_uplink
);

static
rrc_length_t
rrc_il_get_countMSB_downlink_len
(
    countMSB_downlink_t *p_countMSB_downlink
);

static
rrc_length_t
rrc_il_get_countMSB_downlink_tlv_len
(
    countMSB_downlink_t *p_countMSB_downlink

);

static
rrc_return_et
rrc_il_compose_countMSB_downlink
(
    U8  **pp_buffer,
    countMSB_downlink_t *p_countMSB_downlink
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_drb_countMSB_info_len
(
    rrc_pdcp_drb_countMSB_info_t *p_rrc_pdcp_drb_countMSB_info
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_drb_countMSB_info_tlv_len
(
    rrc_pdcp_drb_countMSB_info_t *p_rrc_pdcp_drb_countMSB_info

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_drb_countMSB_info
(
    U8  **pp_buffer,
    rrc_pdcp_drb_countMSB_info_t *p_rrc_pdcp_drb_countMSB_info
);

static
rrc_length_t
rrc_il_get_pdcp_communication_info_len
(
    pdcp_communication_info_t *p_pdcp_communication_info
);

static
rrc_return_et
rrc_il_compose_pdcp_communication_info
(
    U8  **pp_buffer,
    pdcp_communication_info_t *p_pdcp_communication_info
);

static
rrc_length_t
rrc_il_get_rrc_mac_i_msg_req_len
(
    rrc_mac_i_msg_req_t *p_rrc_mac_i_msg_req
);

static
rrc_length_t
rrc_il_get_rrc_mac_i_msg_req_tlv_len
(
    rrc_mac_i_msg_req_t *p_rrc_mac_i_msg_req

);

static
rrc_return_et
rrc_il_compose_rrc_mac_i_msg_req
(
    U8  **pp_buffer,
    rrc_mac_i_msg_req_t *p_rrc_mac_i_msg_req
);

static
rrc_length_t
rrc_il_get_rrc_ue_inactivity_time_val_len
(
    rrc_ue_inactivity_time_val_t *p_rrc_ue_inactivity_time_val
);

static
rrc_length_t
rrc_il_get_rrc_ue_inactivity_time_val_tlv_len
(
    rrc_ue_inactivity_time_val_t *p_rrc_ue_inactivity_time_val

);

static
rrc_return_et
rrc_il_compose_rrc_ue_inactivity_time_val
(
    U8  **pp_buffer,
    rrc_ue_inactivity_time_val_t *p_rrc_ue_inactivity_time_val
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_inactive_ue_info_len
(
    rrc_pdcp_inactive_ue_info_t *p_rrc_pdcp_inactive_ue_info
);

static
rrc_length_t
rrc_il_get_rrc_pdcp_inactive_ue_info_tlv_len
(
    rrc_pdcp_inactive_ue_info_t *p_rrc_pdcp_inactive_ue_info

);

static
rrc_return_et
rrc_il_compose_rrc_pdcp_inactive_ue_info
(
    U8  **pp_buffer,
    rrc_pdcp_inactive_ue_info_t *p_rrc_pdcp_inactive_ue_info
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_length_t
rrc_il_get_rrc_pdcp_cr_ue_entity_req_len
(
    rrc_pdcp_cr_ue_entity_req_t *p_rrc_pdcp_cr_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_cr_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_cr_ue_entity_req->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_cr_ue_entity_req->crnti);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_cr_ue_entity_req->num_cr_srb_entity <= ARRSIZE(p_rrc_pdcp_cr_ue_entity_req->cr_srb_entities));
        for (loop = 0; loop < p_rrc_pdcp_cr_ue_entity_req->num_cr_srb_entity; loop++)
        {
            length += rrc_il_get_rrc_pdcp_cr_srb_entity_tlv_len(&p_rrc_pdcp_cr_ue_entity_req->cr_srb_entities[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_cr_ue_entity_req->num_cr_drb_entity <= ARRSIZE(p_rrc_pdcp_cr_ue_entity_req->cr_drb_entities));
        for (loop = 0; loop < p_rrc_pdcp_cr_ue_entity_req->num_cr_drb_entity; loop++)
        {
            length += rrc_il_get_rrc_pdcp_cr_drb_entity_tlv_len(&p_rrc_pdcp_cr_ue_entity_req->cr_drb_entities[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_cr_ue_entity_req->optional_elems_present & RRC_PDCP_CR_SRB_CONFIG_INT_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_config_int_tlv_len(&p_rrc_pdcp_cr_ue_entity_req->config_integrity_protection);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_cr_ue_entity_req->optional_elems_present & RRC_PDCP_CR_SRB_CONFIG_CIPH_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_config_srb_ciph_tlv_len(&p_rrc_pdcp_cr_ue_entity_req->config_srb_ciphering);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_cr_ue_entity_req->optional_elems_present & RRC_PDCP_CR_DRB_CONFIG_CIPH_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_config_drb_ciph_tlv_len(&p_rrc_pdcp_cr_ue_entity_req->config_drb_ciphering);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_cr_ue_entity_req->optional_elems_present & RRC_PDCP_HO_TRIGGER_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_ho_triggered_tlv_len(&p_rrc_pdcp_cr_ue_entity_req->ho_triggered);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_cr_ue_entity_req->optional_elems_present & RRC_PDCP_SPS_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_sps_config_tlv_len(&p_rrc_pdcp_cr_ue_entity_req->sps_config);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_cr_ue_entity_req->optional_elems_present & RRC_PDCP_CONFIG_MACI_REQ_PRESENT)
    {
        length += rrc_il_get_rrc_mac_i_msg_tlv_len(&p_rrc_pdcp_cr_ue_entity_req->mac_i_req);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_cr_ue_entity_req->optional_elems_present & RRC_PDCP_UE_INACTIVITY_TIMER_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_ue_inactivity_timer_tlv_len(&p_rrc_pdcp_cr_ue_entity_req->ue_inactivity_timer);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_cr_ue_entity_req
(
    U8  **pp_buffer,
    rrc_pdcp_cr_ue_entity_req_t *p_rrc_pdcp_cr_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_cr_ue_entity_req != PNULL);

    /* This function composes rrc_pdcp_cr_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_cr_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_cr_ue_entity_req->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_cr_ue_entity_req->crnti, "crnti");
    *pp_buffer += sizeof(p_rrc_pdcp_cr_ue_entity_req->crnti);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_cr_ue_entity_req->num_cr_srb_entity <= ARRSIZE(p_rrc_pdcp_cr_ue_entity_req->cr_srb_entities));
        for (loop = 0; loop < p_rrc_pdcp_cr_ue_entity_req->num_cr_srb_entity; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_cr_srb_entity(pp_buffer, &p_rrc_pdcp_cr_ue_entity_req->cr_srb_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_cr_ue_entity_req->num_cr_drb_entity <= ARRSIZE(p_rrc_pdcp_cr_ue_entity_req->cr_drb_entities));
        for (loop = 0; loop < p_rrc_pdcp_cr_ue_entity_req->num_cr_drb_entity; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_cr_drb_entity(pp_buffer, &p_rrc_pdcp_cr_ue_entity_req->cr_drb_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_cr_ue_entity_req->optional_elems_present & RRC_PDCP_CR_SRB_CONFIG_INT_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_config_int(pp_buffer, &p_rrc_pdcp_cr_ue_entity_req->config_integrity_protection))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_cr_ue_entity_req->optional_elems_present & RRC_PDCP_CR_SRB_CONFIG_CIPH_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_config_srb_ciph(pp_buffer, &p_rrc_pdcp_cr_ue_entity_req->config_srb_ciphering))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_cr_ue_entity_req->optional_elems_present & RRC_PDCP_CR_DRB_CONFIG_CIPH_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_config_drb_ciph(pp_buffer, &p_rrc_pdcp_cr_ue_entity_req->config_drb_ciphering))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_cr_ue_entity_req->optional_elems_present & RRC_PDCP_HO_TRIGGER_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_ho_triggered(pp_buffer, &p_rrc_pdcp_cr_ue_entity_req->ho_triggered))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_cr_ue_entity_req->optional_elems_present & RRC_PDCP_SPS_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_sps_config(pp_buffer, &p_rrc_pdcp_cr_ue_entity_req->sps_config))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_cr_ue_entity_req->optional_elems_present & RRC_PDCP_CONFIG_MACI_REQ_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_i_msg(pp_buffer, &p_rrc_pdcp_cr_ue_entity_req->mac_i_req))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_cr_ue_entity_req->optional_elems_present & RRC_PDCP_UE_INACTIVITY_TIMER_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_ue_inactivity_timer(pp_buffer, &p_rrc_pdcp_cr_ue_entity_req->ue_inactivity_timer))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_cr_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_CREATE_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_cr_ue_entity_req
(
    rrc_pdcp_cr_ue_entity_req_t  *p_rrc_pdcp_cr_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_cr_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_cr_ue_entity_req_len(p_rrc_pdcp_cr_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_CREATE_UE_ENTITY_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_CREATE_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_CREATE_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_PDCP_CREATE_UE_ENTITY_REQ message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_cr_ue_entity_req(&p_pdcp_msg, p_rrc_pdcp_cr_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_cr_srb_entity_len
(
    rrc_pdcp_cr_srb_entity_t *p_rrc_pdcp_cr_srb_entity
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_cr_srb_entity != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_cr_srb_entity->srb_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_cr_srb_entity->rlc_mode);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_cr_srb_entity->rb_direction);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_cr_srb_entity_tlv_len
(
    rrc_pdcp_cr_srb_entity_t *p_rrc_pdcp_cr_srb_entity

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_cr_srb_entity != PNULL);

    length += rrc_il_get_rrc_pdcp_cr_srb_entity_len(p_rrc_pdcp_cr_srb_entity);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_cr_srb_entity
(
    U8  **pp_buffer,
    rrc_pdcp_cr_srb_entity_t *p_rrc_pdcp_cr_srb_entity
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_cr_srb_entity != PNULL);

    /* This function composes rrc_pdcp_cr_srb_entity */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_CREATE_SRB_ENTITY_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_cr_srb_entity_tlv_len(p_rrc_pdcp_cr_srb_entity);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_cr_srb_entity->srb_id, "srb_id");
    *pp_buffer += sizeof(p_rrc_pdcp_cr_srb_entity->srb_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_cr_srb_entity->rlc_mode, "rlc_mode");
    *pp_buffer += sizeof(p_rrc_pdcp_cr_srb_entity->rlc_mode);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_cr_srb_entity->rb_direction, "rb_direction");
    *pp_buffer += sizeof(p_rrc_pdcp_cr_srb_entity->rb_direction);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_rohc_len
(
    rrc_pdcp_config_rohc_t *p_rrc_pdcp_config_rohc
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_rohc != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_config_rohc->profile_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_config_rohc->max_cid);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_rohc_tlv_len
(
    rrc_pdcp_config_rohc_t *p_rrc_pdcp_config_rohc

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_rohc != PNULL);

    length += rrc_il_get_rrc_pdcp_config_rohc_len(p_rrc_pdcp_config_rohc);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_rohc
(
    U8  **pp_buffer,
    rrc_pdcp_config_rohc_t *p_rrc_pdcp_config_rohc
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_config_rohc != PNULL);

    /* This function composes rrc_pdcp_config_rohc */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_CONFIGURE_ROHC_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_config_rohc_tlv_len(p_rrc_pdcp_config_rohc);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_config_rohc->profile_id, "profile_id");
    *pp_buffer += sizeof(p_rrc_pdcp_config_rohc->profile_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_config_rohc->max_cid, "max_cid");
    *pp_buffer += sizeof(p_rrc_pdcp_config_rohc->max_cid);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_disc_timer_len
(
    rrc_pdcp_config_disc_timer_t *p_rrc_pdcp_config_disc_timer
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_disc_timer != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_config_disc_timer->disc_timer);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_disc_timer_tlv_len
(
    rrc_pdcp_config_disc_timer_t *p_rrc_pdcp_config_disc_timer

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_disc_timer != PNULL);

    length += rrc_il_get_rrc_pdcp_config_disc_timer_len(p_rrc_pdcp_config_disc_timer);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_disc_timer
(
    U8  **pp_buffer,
    rrc_pdcp_config_disc_timer_t *p_rrc_pdcp_config_disc_timer
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_config_disc_timer != PNULL);

    /* This function composes rrc_pdcp_config_disc_timer */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_CONFIGURE_DISCARD_TIMER_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_config_disc_timer_tlv_len(p_rrc_pdcp_config_disc_timer);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_config_disc_timer->disc_timer, "disc_timer");
    *pp_buffer += sizeof(p_rrc_pdcp_config_disc_timer->disc_timer);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_sn_size_len
(
    rrc_pdcp_config_sn_size_t *p_rrc_pdcp_config_sn_size
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_sn_size != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_config_sn_size->sn_size);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_sn_size_tlv_len
(
    rrc_pdcp_config_sn_size_t *p_rrc_pdcp_config_sn_size

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_sn_size != PNULL);

    length += rrc_il_get_rrc_pdcp_config_sn_size_len(p_rrc_pdcp_config_sn_size);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_sn_size
(
    U8  **pp_buffer,
    rrc_pdcp_config_sn_size_t *p_rrc_pdcp_config_sn_size
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_config_sn_size != PNULL);

    /* This function composes rrc_pdcp_config_sn_size */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_CONFIGURE_SN_SIZE_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_config_sn_size_tlv_len(p_rrc_pdcp_config_sn_size);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_config_sn_size->sn_size, "sn_size");
    *pp_buffer += sizeof(p_rrc_pdcp_config_sn_size->sn_size);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_st_rep_required_len
(
    rrc_pdcp_config_st_rep_required_t *p_rrc_pdcp_config_st_rep_required
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_st_rep_required != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_config_st_rep_required->st_rep_required);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_st_rep_required_tlv_len
(
    rrc_pdcp_config_st_rep_required_t *p_rrc_pdcp_config_st_rep_required

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_st_rep_required != PNULL);

    length += rrc_il_get_rrc_pdcp_config_st_rep_required_len(p_rrc_pdcp_config_st_rep_required);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_st_rep_required
(
    U8  **pp_buffer,
    rrc_pdcp_config_st_rep_required_t *p_rrc_pdcp_config_st_rep_required
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_config_st_rep_required != PNULL);

    /* This function composes rrc_pdcp_config_st_rep_required */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_CONFIGURE_ST_REPORT_REQUIRED_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_config_st_rep_required_tlv_len(p_rrc_pdcp_config_st_rep_required);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_config_st_rep_required->st_rep_required > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_config_st_rep_required->st_rep_required] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_pdcp_config_st_rep_required->st_rep_required);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_config_st_rep_required->st_rep_required, "st_rep_required");
    *pp_buffer += sizeof(p_rrc_pdcp_config_st_rep_required->st_rep_required);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_ue_st_rep_required_len
(
    rrc_pdcp_config_ue_st_rep_required_t *p_rrc_pdcp_config_ue_st_rep_required
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_ue_st_rep_required != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_config_ue_st_rep_required->ue_st_rep_required);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_ue_st_rep_required_tlv_len
(
    rrc_pdcp_config_ue_st_rep_required_t *p_rrc_pdcp_config_ue_st_rep_required

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_ue_st_rep_required != PNULL);

    length += rrc_il_get_rrc_pdcp_config_ue_st_rep_required_len(p_rrc_pdcp_config_ue_st_rep_required);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_ue_st_rep_required
(
    U8  **pp_buffer,
    rrc_pdcp_config_ue_st_rep_required_t *p_rrc_pdcp_config_ue_st_rep_required
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_config_ue_st_rep_required != PNULL);

    /* This function composes rrc_pdcp_config_ue_st_rep_required */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_CONFIGURE_UE_PSR_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_config_ue_st_rep_required_tlv_len(p_rrc_pdcp_config_ue_st_rep_required);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_config_ue_st_rep_required->ue_st_rep_required > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_config_ue_st_rep_required->ue_st_rep_required] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_pdcp_config_ue_st_rep_required->ue_st_rep_required);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_config_ue_st_rep_required->ue_st_rep_required, "ue_st_rep_required");
    *pp_buffer += sizeof(p_rrc_pdcp_config_ue_st_rep_required->ue_st_rep_required);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_cr_drb_entity_len
(
    rrc_pdcp_cr_drb_entity_t *p_rrc_pdcp_cr_drb_entity
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_cr_drb_entity != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_cr_drb_entity->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_cr_drb_entity->drb_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_cr_drb_entity->rlc_mode);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_cr_drb_entity->rb_direction);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_cr_drb_entity->qci);

    /* Get length of TLV */
    if (p_rrc_pdcp_cr_drb_entity->optional_elems_present & RRC_PDCP_CR_DRB_CONFIG_ROHC_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_config_rohc_tlv_len(&p_rrc_pdcp_cr_drb_entity->config_rohc);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_cr_drb_entity->optional_elems_present & RRC_PDCP_CR_DRB_CONFIG_DISC_TIMER_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_config_disc_timer_tlv_len(&p_rrc_pdcp_cr_drb_entity->config_disc_timer);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_cr_drb_entity->optional_elems_present & RRC_PDCP_CR_DRB_CONFIG_SN_SIZE_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_config_sn_size_tlv_len(&p_rrc_pdcp_cr_drb_entity->config_sn_size);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_cr_drb_entity->optional_elems_present & RRC_PDCP_CR_DRB_CONFIG_ST_REPORT_REQUIRED_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_config_st_rep_required_tlv_len(&p_rrc_pdcp_cr_drb_entity->st_rep_required);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_cr_drb_entity->optional_elems_present & RRC_PDCP_CR_DRB_CONFIG_UE_ST_REPORT_REQUIRED_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_config_ue_st_rep_required_tlv_len(&p_rrc_pdcp_cr_drb_entity->ue_st_rep_required);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_cr_drb_entity_tlv_len
(
    rrc_pdcp_cr_drb_entity_t *p_rrc_pdcp_cr_drb_entity

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_cr_drb_entity != PNULL);

    length += rrc_il_get_rrc_pdcp_cr_drb_entity_len(p_rrc_pdcp_cr_drb_entity);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_cr_drb_entity
(
    U8  **pp_buffer,
    rrc_pdcp_cr_drb_entity_t *p_rrc_pdcp_cr_drb_entity
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_cr_drb_entity != PNULL);

    /* This function composes rrc_pdcp_cr_drb_entity */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_CREATE_DRB_ENTITY_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_cr_drb_entity_tlv_len(p_rrc_pdcp_cr_drb_entity);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_cr_drb_entity->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_cr_drb_entity->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_cr_drb_entity->drb_id, "drb_id");
    *pp_buffer += sizeof(p_rrc_pdcp_cr_drb_entity->drb_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_cr_drb_entity->rlc_mode, "rlc_mode");
    *pp_buffer += sizeof(p_rrc_pdcp_cr_drb_entity->rlc_mode);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_cr_drb_entity->rb_direction, "rb_direction");
    *pp_buffer += sizeof(p_rrc_pdcp_cr_drb_entity->rb_direction);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_pdcp_cr_drb_entity->qci < 1) || (p_rrc_pdcp_cr_drb_entity->qci > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_cr_drb_entity->qci] should be in range "
            "1 to 9. Incorrect value %u received.", p_rrc_pdcp_cr_drb_entity->qci);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_cr_drb_entity->qci, "qci");
    *pp_buffer += sizeof(p_rrc_pdcp_cr_drb_entity->qci);

    /* Compose TLV  */
    if (p_rrc_pdcp_cr_drb_entity->optional_elems_present & RRC_PDCP_CR_DRB_CONFIG_ROHC_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_config_rohc(pp_buffer, &p_rrc_pdcp_cr_drb_entity->config_rohc))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_cr_drb_entity->optional_elems_present & RRC_PDCP_CR_DRB_CONFIG_DISC_TIMER_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_config_disc_timer(pp_buffer, &p_rrc_pdcp_cr_drb_entity->config_disc_timer))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_cr_drb_entity->optional_elems_present & RRC_PDCP_CR_DRB_CONFIG_SN_SIZE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_config_sn_size(pp_buffer, &p_rrc_pdcp_cr_drb_entity->config_sn_size))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_cr_drb_entity->optional_elems_present & RRC_PDCP_CR_DRB_CONFIG_ST_REPORT_REQUIRED_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_config_st_rep_required(pp_buffer, &p_rrc_pdcp_cr_drb_entity->st_rep_required))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_cr_drb_entity->optional_elems_present & RRC_PDCP_CR_DRB_CONFIG_UE_ST_REPORT_REQUIRED_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_config_ue_st_rep_required(pp_buffer, &p_rrc_pdcp_cr_drb_entity->ue_st_rep_required))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_int_len
(
    rrc_pdcp_config_int_t *p_rrc_pdcp_config_int
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_int != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_config_int->algorithm_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_pdcp_config_int->key);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_int_tlv_len
(
    rrc_pdcp_config_int_t *p_rrc_pdcp_config_int

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_int != PNULL);

    length += rrc_il_get_rrc_pdcp_config_int_len(p_rrc_pdcp_config_int);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_int
(
    U8  **pp_buffer,
    rrc_pdcp_config_int_t *p_rrc_pdcp_config_int
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_config_int != PNULL);

    /* This function composes rrc_pdcp_config_int */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_CONFIGURE_INTEGRITY_PROTECTION_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_config_int_tlv_len(p_rrc_pdcp_config_int);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_config_int->algorithm_id, "algorithm_id");
    *pp_buffer += sizeof(p_rrc_pdcp_config_int->algorithm_id);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_pdcp_config_int->key); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_config_int->key[loop], "key[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_srb_ciph_len
(
    rrc_pdcp_config_srb_ciph_t *p_rrc_pdcp_config_srb_ciph
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_srb_ciph != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_config_srb_ciph->algorithm_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_pdcp_config_srb_ciph->key);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_srb_ciph_tlv_len
(
    rrc_pdcp_config_srb_ciph_t *p_rrc_pdcp_config_srb_ciph

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_srb_ciph != PNULL);

    length += rrc_il_get_rrc_pdcp_config_srb_ciph_len(p_rrc_pdcp_config_srb_ciph);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_srb_ciph
(
    U8  **pp_buffer,
    rrc_pdcp_config_srb_ciph_t *p_rrc_pdcp_config_srb_ciph
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_config_srb_ciph != PNULL);

    /* This function composes rrc_pdcp_config_srb_ciph */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_CONFIGURE_SRB_CIPHERING_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_config_srb_ciph_tlv_len(p_rrc_pdcp_config_srb_ciph);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_config_srb_ciph->algorithm_id, "algorithm_id");
    *pp_buffer += sizeof(p_rrc_pdcp_config_srb_ciph->algorithm_id);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_pdcp_config_srb_ciph->key); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_config_srb_ciph->key[loop], "key[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_drb_ciph_len
(
    rrc_pdcp_config_drb_ciph_t *p_rrc_pdcp_config_drb_ciph
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_drb_ciph != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_config_drb_ciph->algorithm_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_pdcp_config_drb_ciph->key);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_drb_ciph_tlv_len
(
    rrc_pdcp_config_drb_ciph_t *p_rrc_pdcp_config_drb_ciph

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_drb_ciph != PNULL);

    length += rrc_il_get_rrc_pdcp_config_drb_ciph_len(p_rrc_pdcp_config_drb_ciph);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_drb_ciph
(
    U8  **pp_buffer,
    rrc_pdcp_config_drb_ciph_t *p_rrc_pdcp_config_drb_ciph
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_config_drb_ciph != PNULL);

    /* This function composes rrc_pdcp_config_drb_ciph */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_CONFIGURE_DRB_CIPHERING_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_config_drb_ciph_tlv_len(p_rrc_pdcp_config_drb_ciph);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_config_drb_ciph->algorithm_id, "algorithm_id");
    *pp_buffer += sizeof(p_rrc_pdcp_config_drb_ciph->algorithm_id);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_pdcp_config_drb_ciph->key); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_config_drb_ciph->key[loop], "key[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_ue_inactive_time_len
(
    rrc_ue_inactive_time_t *p_rrc_ue_inactive_time
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_ue_inactive_time != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_ue_inactive_time->ue_inactive_time_val);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_ue_inactive_time_tlv_len
(
    rrc_ue_inactive_time_t *p_rrc_ue_inactive_time

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_ue_inactive_time != PNULL);

    length += rrc_il_get_rrc_ue_inactive_time_len(p_rrc_ue_inactive_time);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_ue_inactive_time
(
    U8  **pp_buffer,
    rrc_ue_inactive_time_t *p_rrc_ue_inactive_time
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_ue_inactive_time != PNULL);

    /* This function composes rrc_ue_inactive_time */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_UE_INACTIVE_TIME_LAPSED_TAG;
    tlv_length = rrc_il_get_rrc_ue_inactive_time_tlv_len(p_rrc_ue_inactive_time);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_ue_inactive_time->ue_inactive_time_val > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_ue_inactive_time->ue_inactive_time_val] should be less than"
            " or equal to 63. Incorrect value %u received.", p_rrc_ue_inactive_time->ue_inactive_time_val);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_ue_inactive_time->ue_inactive_time_val, "ue_inactive_time_val");
    *pp_buffer += sizeof(p_rrc_ue_inactive_time->ue_inactive_time_val);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_ho_triggered_len
(
    rrc_pdcp_ho_triggered_t *p_rrc_pdcp_ho_triggered
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_ho_triggered != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_ho_triggered->ho_trigger);

    /* Get length of TLV */
    if (p_rrc_pdcp_ho_triggered->optional_elems_present & RRC_PDCP_UE_INACTIVE_TIME_LAPSED_PRESENT)
    {
        length += rrc_il_get_rrc_ue_inactive_time_tlv_len(&p_rrc_pdcp_ho_triggered->rrc_ue_inactive_time_lapsed);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_ho_triggered_tlv_len
(
    rrc_pdcp_ho_triggered_t *p_rrc_pdcp_ho_triggered

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_ho_triggered != PNULL);

    length += rrc_il_get_rrc_pdcp_ho_triggered_len(p_rrc_pdcp_ho_triggered);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_ho_triggered
(
    U8  **pp_buffer,
    rrc_pdcp_ho_triggered_t *p_rrc_pdcp_ho_triggered
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_ho_triggered != PNULL);

    /* This function composes rrc_pdcp_ho_triggered */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_HO_TRIGGER_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_ho_triggered_tlv_len(p_rrc_pdcp_ho_triggered);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_ho_triggered->ho_trigger > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_ho_triggered->ho_trigger] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_pdcp_ho_triggered->ho_trigger);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_ho_triggered->ho_trigger, "ho_trigger");
    *pp_buffer += sizeof(p_rrc_pdcp_ho_triggered->ho_trigger);

    /* Compose TLV  */
    if (p_rrc_pdcp_ho_triggered->optional_elems_present & RRC_PDCP_UE_INACTIVE_TIME_LAPSED_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_ue_inactive_time(pp_buffer, &p_rrc_pdcp_ho_triggered->rrc_ue_inactive_time_lapsed))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_dl_setup_info_len
(
    rrc_pdcp_sps_dl_setup_info_t *p_rrc_pdcp_sps_dl_setup_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_sps_dl_setup_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_sps_dl_setup_info->semi_persistent_sched_interval_dl);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_dl_setup_info_tlv_len
(
    rrc_pdcp_sps_dl_setup_info_t *p_rrc_pdcp_sps_dl_setup_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_sps_dl_setup_info != PNULL);

    length += rrc_il_get_rrc_pdcp_sps_dl_setup_info_len(p_rrc_pdcp_sps_dl_setup_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_sps_dl_setup_info
(
    U8  **pp_buffer,
    rrc_pdcp_sps_dl_setup_info_t *p_rrc_pdcp_sps_dl_setup_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_sps_dl_setup_info != PNULL);

    /* This function composes rrc_pdcp_sps_dl_setup_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_SPS_DL_SETUP_INFO_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_sps_dl_setup_info_tlv_len(p_rrc_pdcp_sps_dl_setup_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_sps_dl_setup_info->semi_persistent_sched_interval_dl > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_sps_dl_setup_info->semi_persistent_sched_interval_dl] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_pdcp_sps_dl_setup_info->semi_persistent_sched_interval_dl);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_sps_dl_setup_info->semi_persistent_sched_interval_dl, "semi_persistent_sched_interval_dl");
    *pp_buffer += sizeof(p_rrc_pdcp_sps_dl_setup_info->semi_persistent_sched_interval_dl);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_dl_info_len
(
    rrc_pdcp_sps_dl_info_t *p_rrc_pdcp_sps_dl_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_sps_dl_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_sps_dl_info->request_type);

    /* Get length of TLV */
    if (p_rrc_pdcp_sps_dl_info->optional_elems_present & RRC_PDCP_SPS_DL_SETUP_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_sps_dl_setup_info_tlv_len(&p_rrc_pdcp_sps_dl_info->sps_dl_setup_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_dl_info_tlv_len
(
    rrc_pdcp_sps_dl_info_t *p_rrc_pdcp_sps_dl_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_sps_dl_info != PNULL);

    length += rrc_il_get_rrc_pdcp_sps_dl_info_len(p_rrc_pdcp_sps_dl_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_sps_dl_info
(
    U8  **pp_buffer,
    rrc_pdcp_sps_dl_info_t *p_rrc_pdcp_sps_dl_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_sps_dl_info != PNULL);

    /* This function composes rrc_pdcp_sps_dl_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_SPS_DL_INFO_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_sps_dl_info_tlv_len(p_rrc_pdcp_sps_dl_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_sps_dl_info->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_sps_dl_info->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_pdcp_sps_dl_info->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_sps_dl_info->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_pdcp_sps_dl_info->request_type);

    /* Compose TLV  */
    if (p_rrc_pdcp_sps_dl_info->optional_elems_present & RRC_PDCP_SPS_DL_SETUP_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_sps_dl_setup_info(pp_buffer, &p_rrc_pdcp_sps_dl_info->sps_dl_setup_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_ul_setup_info_len
(
    rrc_pdcp_sps_ul_setup_info_t *p_rrc_pdcp_sps_ul_setup_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_sps_ul_setup_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_sps_ul_setup_info->semi_persistent_sched_interval_ul);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_ul_setup_info_tlv_len
(
    rrc_pdcp_sps_ul_setup_info_t *p_rrc_pdcp_sps_ul_setup_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_sps_ul_setup_info != PNULL);

    length += rrc_il_get_rrc_pdcp_sps_ul_setup_info_len(p_rrc_pdcp_sps_ul_setup_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_sps_ul_setup_info
(
    U8  **pp_buffer,
    rrc_pdcp_sps_ul_setup_info_t *p_rrc_pdcp_sps_ul_setup_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_sps_ul_setup_info != PNULL);

    /* This function composes rrc_pdcp_sps_ul_setup_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_SPS_UL_SETUP_INFO_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_sps_ul_setup_info_tlv_len(p_rrc_pdcp_sps_ul_setup_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_sps_ul_setup_info->semi_persistent_sched_interval_ul > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_sps_ul_setup_info->semi_persistent_sched_interval_ul] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_pdcp_sps_ul_setup_info->semi_persistent_sched_interval_ul);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_sps_ul_setup_info->semi_persistent_sched_interval_ul, "semi_persistent_sched_interval_ul");
    *pp_buffer += sizeof(p_rrc_pdcp_sps_ul_setup_info->semi_persistent_sched_interval_ul);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_ul_info_len
(
    rrc_pdcp_sps_ul_info_t *p_rrc_pdcp_sps_ul_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_sps_ul_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_sps_ul_info->request_type);

    /* Get length of TLV */
    if (p_rrc_pdcp_sps_ul_info->optional_elems_present & RRC_PDCP_SPS_UL_SETUP_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_sps_ul_setup_info_tlv_len(&p_rrc_pdcp_sps_ul_info->sps_ul_setup_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_ul_info_tlv_len
(
    rrc_pdcp_sps_ul_info_t *p_rrc_pdcp_sps_ul_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_sps_ul_info != PNULL);

    length += rrc_il_get_rrc_pdcp_sps_ul_info_len(p_rrc_pdcp_sps_ul_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_sps_ul_info
(
    U8  **pp_buffer,
    rrc_pdcp_sps_ul_info_t *p_rrc_pdcp_sps_ul_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_sps_ul_info != PNULL);

    /* This function composes rrc_pdcp_sps_ul_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_SPS_UL_INFO_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_sps_ul_info_tlv_len(p_rrc_pdcp_sps_ul_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_sps_ul_info->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_sps_ul_info->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_pdcp_sps_ul_info->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_sps_ul_info->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_pdcp_sps_ul_info->request_type);

    /* Compose TLV  */
    if (p_rrc_pdcp_sps_ul_info->optional_elems_present & RRC_PDCP_SPS_UL_SETUP_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_sps_ul_setup_info(pp_buffer, &p_rrc_pdcp_sps_ul_info->sps_ul_setup_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_config_len
(
    rrc_pdcp_sps_config_t *p_rrc_pdcp_sps_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_sps_config != PNULL);

    /* Get length of TLV */
    if (p_rrc_pdcp_sps_config->optional_elems_present & RRC_PDCP_SPS_DL_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_sps_dl_info_tlv_len(&p_rrc_pdcp_sps_config->sps_dl_info);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_sps_config->optional_elems_present & RRC_PDCP_SPS_UL_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_sps_ul_info_tlv_len(&p_rrc_pdcp_sps_config->sps_ul_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_sps_config_tlv_len
(
    rrc_pdcp_sps_config_t *p_rrc_pdcp_sps_config

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_sps_config != PNULL);

    length += rrc_il_get_rrc_pdcp_sps_config_len(p_rrc_pdcp_sps_config);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_sps_config
(
    U8  **pp_buffer,
    rrc_pdcp_sps_config_t *p_rrc_pdcp_sps_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_sps_config != PNULL);

    /* This function composes rrc_pdcp_sps_config */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_SPS_CONFIG_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_sps_config_tlv_len(p_rrc_pdcp_sps_config);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose TLV  */
    if (p_rrc_pdcp_sps_config->optional_elems_present & RRC_PDCP_SPS_DL_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_sps_dl_info(pp_buffer, &p_rrc_pdcp_sps_config->sps_dl_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_sps_config->optional_elems_present & RRC_PDCP_SPS_UL_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_sps_ul_info(pp_buffer, &p_rrc_pdcp_sps_config->sps_ul_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_i_msg_len
(
    rrc_mac_i_msg_t *p_rrc_mac_i_msg
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_i_msg != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_i_msg->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_i_msg->rb_direction);

    /* Add buffer size */
    length += p_rrc_mac_i_msg->encoded_var_short_mac_i_data_length;

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_i_msg_tlv_len
(
    rrc_mac_i_msg_t *p_rrc_mac_i_msg

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_i_msg != PNULL);

    length += rrc_il_get_rrc_mac_i_msg_len(p_rrc_mac_i_msg);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_i_msg
(
    U8  **pp_buffer,
    rrc_mac_i_msg_t *p_rrc_mac_i_msg
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_i_msg != PNULL);

    /* This function composes rrc_mac_i_msg */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_MACI_REQ_TAG;
    tlv_length = rrc_il_get_rrc_mac_i_msg_tlv_len(p_rrc_mac_i_msg);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_i_msg->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_mac_i_msg->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_i_msg->rb_direction, "rb_direction");
    *pp_buffer += sizeof(p_rrc_mac_i_msg->rb_direction);

    /* Compose OCTET_STRING LIMITED_TILL_THE_END */
    {
        if(ARRSIZE(p_rrc_mac_i_msg->encoded_var_short_mac_i) < p_rrc_mac_i_msg->encoded_var_short_mac_i_data_length)
        {
            RRC_TRACE(RRC_WARNING, "p_rrc_mac_i_msg->encoded_var_short_mac_i_data_length = %u is exceeding p_rrc_mac_i_msg->encoded_var_short_mac_i array boundary", p_rrc_mac_i_msg->encoded_var_short_mac_i_data_length);
            return RRC_FAILURE;
        }
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_mac_i_msg->encoded_var_short_mac_i_data_length; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_i_msg->encoded_var_short_mac_i[loop], "encoded_var_short_mac_i[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_ue_inactivity_timer_len
(
    rrc_pdcp_ue_inactivity_timer_t *p_rrc_pdcp_ue_inactivity_timer
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_ue_inactivity_timer != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_ue_inactivity_timer->ue_inactive_time_config);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_ue_inactivity_timer_tlv_len
(
    rrc_pdcp_ue_inactivity_timer_t *p_rrc_pdcp_ue_inactivity_timer

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_ue_inactivity_timer != PNULL);

    length += rrc_il_get_rrc_pdcp_ue_inactivity_timer_len(p_rrc_pdcp_ue_inactivity_timer);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_ue_inactivity_timer
(
    U8  **pp_buffer,
    rrc_pdcp_ue_inactivity_timer_t *p_rrc_pdcp_ue_inactivity_timer
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_ue_inactivity_timer != PNULL);

    /* This function composes rrc_pdcp_ue_inactivity_timer */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_UE_INACTIVITY_TIMER_CONFIG_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_ue_inactivity_timer_tlv_len(p_rrc_pdcp_ue_inactivity_timer);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_pdcp_ue_inactivity_timer->ue_inactive_time_config, "ue_inactive_time_config");
    *pp_buffer += sizeof(p_rrc_pdcp_ue_inactivity_timer->ue_inactive_time_config);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_cr_ue_entity_cnf_len
(
    rrc_pdcp_cr_ue_entity_cnf_t *p_rrc_pdcp_cr_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_cr_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_cr_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_cr_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_cr_ue_entity_cnf->num_cr_srb_entity_error <= ARRSIZE(p_rrc_pdcp_cr_ue_entity_cnf->cr_srb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_cr_ue_entity_cnf->num_cr_srb_entity_error; loop++)
        {
            length += rrc_il_get_rrc_pdcp_cr_srb_entity_error_tlv_len(&p_rrc_pdcp_cr_ue_entity_cnf->cr_srb_error_entities[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_cr_ue_entity_cnf->num_cr_drb_entity_error <= ARRSIZE(p_rrc_pdcp_cr_ue_entity_cnf->cr_drb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_cr_ue_entity_cnf->num_cr_drb_entity_error; loop++)
        {
            length += rrc_il_get_rrc_pdcp_cr_drb_entity_error_tlv_len(&p_rrc_pdcp_cr_ue_entity_cnf->cr_drb_error_entities[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_cr_ue_entity_cnf->optional_elems_present & RRC_PDCP_CONFIG_MACI_RESP_PRESENT)
    {
        length += rrc_il_get_rrc_mac_i_tlv_len(&p_rrc_pdcp_cr_ue_entity_cnf->mac_i);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_cr_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_pdcp_cr_ue_entity_cnf_t *p_rrc_pdcp_cr_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_cr_ue_entity_cnf != PNULL);

    /* This function composes rrc_pdcp_cr_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_cr_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_cr_ue_entity_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_cr_ue_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_cr_ue_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_pdcp_cr_ue_entity_cnf->response_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_cr_ue_entity_cnf->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_pdcp_cr_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_cr_ue_entity_cnf->num_cr_srb_entity_error <= ARRSIZE(p_rrc_pdcp_cr_ue_entity_cnf->cr_srb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_cr_ue_entity_cnf->num_cr_srb_entity_error; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_cr_srb_entity_error(pp_buffer, &p_rrc_pdcp_cr_ue_entity_cnf->cr_srb_error_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_cr_ue_entity_cnf->num_cr_drb_entity_error <= ARRSIZE(p_rrc_pdcp_cr_ue_entity_cnf->cr_drb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_cr_ue_entity_cnf->num_cr_drb_entity_error; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_cr_drb_entity_error(pp_buffer, &p_rrc_pdcp_cr_ue_entity_cnf->cr_drb_error_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_cr_ue_entity_cnf->optional_elems_present & RRC_PDCP_CONFIG_MACI_RESP_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_i(pp_buffer, &p_rrc_pdcp_cr_ue_entity_cnf->mac_i))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_cr_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_CREATE_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_cr_ue_entity_cnf
(
    rrc_pdcp_cr_ue_entity_cnf_t  *p_rrc_pdcp_cr_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_cr_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_cr_ue_entity_cnf_len(p_rrc_pdcp_cr_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_CREATE_UE_ENTITY_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_CREATE_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_CREATE_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_PDCP_CREATE_UE_ENTITY_CNF message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_cr_ue_entity_cnf(&p_pdcp_msg, p_rrc_pdcp_cr_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_cr_srb_entity_error_len
(
    rrc_pdcp_cr_srb_entity_error_t *p_rrc_pdcp_cr_srb_entity_error
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_cr_srb_entity_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_cr_srb_entity_error->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_cr_srb_entity_error->response);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_cr_srb_entity_error_tlv_len
(
    rrc_pdcp_cr_srb_entity_error_t *p_rrc_pdcp_cr_srb_entity_error

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_cr_srb_entity_error != PNULL);

    length += rrc_il_get_rrc_pdcp_cr_srb_entity_error_len(p_rrc_pdcp_cr_srb_entity_error);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_cr_srb_entity_error
(
    U8  **pp_buffer,
    rrc_pdcp_cr_srb_entity_error_t *p_rrc_pdcp_cr_srb_entity_error
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_cr_srb_entity_error != PNULL);

    /* This function composes rrc_pdcp_cr_srb_entity_error */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_CREATE_SRB_ENTITY_ERROR_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_cr_srb_entity_error_tlv_len(p_rrc_pdcp_cr_srb_entity_error);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_cr_srb_entity_error->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_cr_srb_entity_error->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_cr_srb_entity_error->response, "response");
    *pp_buffer += sizeof(p_rrc_pdcp_cr_srb_entity_error->response);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_cr_drb_entity_error_len
(
    rrc_pdcp_cr_drb_entity_error_t *p_rrc_pdcp_cr_drb_entity_error
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_cr_drb_entity_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_cr_drb_entity_error->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_cr_drb_entity_error->response);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_cr_drb_entity_error_tlv_len
(
    rrc_pdcp_cr_drb_entity_error_t *p_rrc_pdcp_cr_drb_entity_error

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_cr_drb_entity_error != PNULL);

    length += rrc_il_get_rrc_pdcp_cr_drb_entity_error_len(p_rrc_pdcp_cr_drb_entity_error);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_cr_drb_entity_error
(
    U8  **pp_buffer,
    rrc_pdcp_cr_drb_entity_error_t *p_rrc_pdcp_cr_drb_entity_error
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_cr_drb_entity_error != PNULL);

    /* This function composes rrc_pdcp_cr_drb_entity_error */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_CREATE_DRB_ENTITY_ERROR_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_cr_drb_entity_error_tlv_len(p_rrc_pdcp_cr_drb_entity_error);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_cr_drb_entity_error->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_cr_drb_entity_error->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_cr_drb_entity_error->response, "response");
    *pp_buffer += sizeof(p_rrc_pdcp_cr_drb_entity_error->response);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_i_len
(
    rrc_mac_i_t *p_rrc_mac_i
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_i != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_i->mac_i);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_i_tlv_len
(
    rrc_mac_i_t *p_rrc_mac_i

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_i != PNULL);

    length += rrc_il_get_rrc_mac_i_len(p_rrc_mac_i);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_i
(
    U8  **pp_buffer,
    rrc_mac_i_t *p_rrc_mac_i
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_i != PNULL);

    /* This function composes rrc_mac_i */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_MACI_RESP_TAG;
    tlv_length = rrc_il_get_rrc_mac_i_tlv_len(p_rrc_mac_i);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_mac_i->mac_i, "mac_i");
    *pp_buffer += sizeof(p_rrc_mac_i->mac_i);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_del_ue_entity_req_len
(
    rrc_pdcp_del_ue_entity_req_t *p_rrc_pdcp_del_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_del_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_del_ue_entity_req->ue_index);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_del_ue_entity_req
(
    U8  **pp_buffer,
    rrc_pdcp_del_ue_entity_req_t *p_rrc_pdcp_del_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_del_ue_entity_req != PNULL);

    /* This function composes rrc_pdcp_del_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_del_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_del_ue_entity_req->ue_index);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_del_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_DELETE_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_del_ue_entity_req
(
    rrc_pdcp_del_ue_entity_req_t  *p_rrc_pdcp_del_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_del_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_del_ue_entity_req_len(p_rrc_pdcp_del_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_DELETE_UE_ENTITY_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_DELETE_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_DELETE_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_PDCP_DELETE_UE_ENTITY_REQ message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_del_ue_entity_req(&p_pdcp_msg, p_rrc_pdcp_del_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_del_ue_entity_cnf_len
(
    rrc_pdcp_del_ue_entity_cnf_t *p_rrc_pdcp_del_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_del_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_del_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_del_ue_entity_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_del_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_pdcp_del_ue_entity_cnf_t *p_rrc_pdcp_del_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_del_ue_entity_cnf != PNULL);

    /* This function composes rrc_pdcp_del_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_del_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_del_ue_entity_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_del_ue_entity_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_del_ue_entity_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_pdcp_del_ue_entity_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_del_ue_entity_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_pdcp_del_ue_entity_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_del_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_DELETE_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_del_ue_entity_cnf
(
    rrc_pdcp_del_ue_entity_cnf_t  *p_rrc_pdcp_del_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_del_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_del_ue_entity_cnf_len(p_rrc_pdcp_del_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_DELETE_UE_ENTITY_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_DELETE_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_DELETE_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_PDCP_DELETE_UE_ENTITY_CNF message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_del_ue_entity_cnf(&p_pdcp_msg, p_rrc_pdcp_del_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_update_lwa_traffic_ind_len
(
    rrc_pdcp_update_lwa_traffic_ind_t *p_rrc_pdcp_update_lwa_traffic_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_update_lwa_traffic_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_update_lwa_traffic_ind->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_update_lwa_traffic_ind->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_update_lwa_traffic_ind->traffic_ind);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_update_lwa_traffic_ind
(
    U8  **pp_buffer,
    rrc_pdcp_update_lwa_traffic_ind_t *p_rrc_pdcp_update_lwa_traffic_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_update_lwa_traffic_ind != PNULL);

    /* This function composes rrc_pdcp_update_lwa_traffic_ind */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_update_lwa_traffic_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_update_lwa_traffic_ind->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_update_lwa_traffic_ind->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_update_lwa_traffic_ind->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_update_lwa_traffic_ind->traffic_ind, "traffic_ind");
    *pp_buffer += sizeof(p_rrc_pdcp_update_lwa_traffic_ind->traffic_ind);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_update_lwa_traffic_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_UPDATE_LWA_TRAFFIC_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_update_lwa_traffic_ind
(
    rrc_pdcp_update_lwa_traffic_ind_t  *p_rrc_pdcp_update_lwa_traffic_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_update_lwa_traffic_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_update_lwa_traffic_ind_len(p_rrc_pdcp_update_lwa_traffic_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_UPDATE_LWA_TRAFFIC_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_UPDATE_LWA_TRAFFIC_IND, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_UPDATE_LWA_TRAFFIC_IND, msg_length,cell_index);

    /* Fill RRC_PDCP_UPDATE_LWA_TRAFFIC_IND message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_update_lwa_traffic_ind(&p_pdcp_msg, p_rrc_pdcp_update_lwa_traffic_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_reconf_ue_entity_req_len
(
    rrc_pdcp_reconf_ue_entity_req_t *p_rrc_pdcp_reconf_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_reconf_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_reconf_ue_entity_req->ue_index);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconf_ue_entity_req->num_cr_srb_entity <= ARRSIZE(p_rrc_pdcp_reconf_ue_entity_req->cr_srb_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconf_ue_entity_req->num_cr_srb_entity; loop++)
        {
            length += rrc_il_get_rrc_pdcp_cr_srb_entity_tlv_len(&p_rrc_pdcp_reconf_ue_entity_req->cr_srb_entities[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconf_ue_entity_req->num_cr_drb_entity <= ARRSIZE(p_rrc_pdcp_reconf_ue_entity_req->cr_drb_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconf_ue_entity_req->num_cr_drb_entity; loop++)
        {
            length += rrc_il_get_rrc_pdcp_cr_drb_entity_tlv_len(&p_rrc_pdcp_reconf_ue_entity_req->cr_drb_entities[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconf_ue_entity_req->num_reconfig_srb_entity <= ARRSIZE(p_rrc_pdcp_reconf_ue_entity_req->rcfg_srb_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconf_ue_entity_req->num_reconfig_srb_entity; loop++)
        {
            length += rrc_il_get_rrc_pdcp_rcfg_srb_entity_tlv_len(&p_rrc_pdcp_reconf_ue_entity_req->rcfg_srb_entities[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconf_ue_entity_req->num_reconfig_drb_entity <= ARRSIZE(p_rrc_pdcp_reconf_ue_entity_req->rcfg_drb_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconf_ue_entity_req->num_reconfig_drb_entity; loop++)
        {
            length += rrc_il_get_rrc_pdcp_rcfg_drb_entity_tlv_len(&p_rrc_pdcp_reconf_ue_entity_req->rcfg_drb_entities[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present & RRC_PDCP_SECURITY_OFFSET_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_sec_offset_tlv_len(&p_rrc_pdcp_reconf_ue_entity_req->pdcp_security_offset);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present & RRC_PDCP_CR_SRB_CONFIG_INT_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_config_int_tlv_len(&p_rrc_pdcp_reconf_ue_entity_req->config_integrity_protection);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present & RRC_PDCP_CR_SRB_CONFIG_CIPH_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_config_srb_ciph_tlv_len(&p_rrc_pdcp_reconf_ue_entity_req->config_srb_ciphering);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present & RRC_PDCP_CR_DRB_CONFIG_CIPH_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_config_drb_ciph_tlv_len(&p_rrc_pdcp_reconf_ue_entity_req->config_drb_ciphering);
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconf_ue_entity_req->num_del_srb_entity <= ARRSIZE(p_rrc_pdcp_reconf_ue_entity_req->del_srb_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconf_ue_entity_req->num_del_srb_entity; loop++)
        {
            length += rrc_il_get_rrc_pdcp_del_srb_entity_tlv_len(&p_rrc_pdcp_reconf_ue_entity_req->del_srb_entities[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconf_ue_entity_req->num_del_drb_entity <= ARRSIZE(p_rrc_pdcp_reconf_ue_entity_req->del_drb_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconf_ue_entity_req->num_del_drb_entity; loop++)
        {
            length += rrc_il_get_rrc_pdcp_del_drb_entity_tlv_len(&p_rrc_pdcp_reconf_ue_entity_req->del_drb_entities[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present & RRC_PDCP_RECONFIG_UE_SPS_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_sps_config_tlv_len(&p_rrc_pdcp_reconf_ue_entity_req->sps_config);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present & RRC_PDCP_RECONFIG_MACI_REQ_PRESENT)
    {
        length += rrc_il_get_rrc_mac_i_msg_tlv_len(&p_rrc_pdcp_reconf_ue_entity_req->mac_i_req);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present & RRC_PDCP_RECONFIG_UE_INACTIVITY_TIMER_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_ue_inactivity_timer_tlv_len(&p_rrc_pdcp_reconf_ue_entity_req->ue_inactivity_timer);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_reconf_ue_entity_req
(
    U8  **pp_buffer,
    rrc_pdcp_reconf_ue_entity_req_t *p_rrc_pdcp_reconf_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_reconf_ue_entity_req != PNULL);

    /* This function composes rrc_pdcp_reconf_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_reconf_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_reconf_ue_entity_req->ue_index);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconf_ue_entity_req->num_cr_srb_entity <= ARRSIZE(p_rrc_pdcp_reconf_ue_entity_req->cr_srb_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconf_ue_entity_req->num_cr_srb_entity; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_cr_srb_entity(pp_buffer, &p_rrc_pdcp_reconf_ue_entity_req->cr_srb_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconf_ue_entity_req->num_cr_drb_entity <= ARRSIZE(p_rrc_pdcp_reconf_ue_entity_req->cr_drb_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconf_ue_entity_req->num_cr_drb_entity; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_cr_drb_entity(pp_buffer, &p_rrc_pdcp_reconf_ue_entity_req->cr_drb_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconf_ue_entity_req->num_reconfig_srb_entity <= ARRSIZE(p_rrc_pdcp_reconf_ue_entity_req->rcfg_srb_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconf_ue_entity_req->num_reconfig_srb_entity; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_rcfg_srb_entity(pp_buffer, &p_rrc_pdcp_reconf_ue_entity_req->rcfg_srb_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconf_ue_entity_req->num_reconfig_drb_entity <= ARRSIZE(p_rrc_pdcp_reconf_ue_entity_req->rcfg_drb_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconf_ue_entity_req->num_reconfig_drb_entity; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_rcfg_drb_entity(pp_buffer, &p_rrc_pdcp_reconf_ue_entity_req->rcfg_drb_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present & RRC_PDCP_SECURITY_OFFSET_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_sec_offset(pp_buffer, &p_rrc_pdcp_reconf_ue_entity_req->pdcp_security_offset))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present & RRC_PDCP_CR_SRB_CONFIG_INT_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_config_int(pp_buffer, &p_rrc_pdcp_reconf_ue_entity_req->config_integrity_protection))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present & RRC_PDCP_CR_SRB_CONFIG_CIPH_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_config_srb_ciph(pp_buffer, &p_rrc_pdcp_reconf_ue_entity_req->config_srb_ciphering))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present & RRC_PDCP_CR_DRB_CONFIG_CIPH_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_config_drb_ciph(pp_buffer, &p_rrc_pdcp_reconf_ue_entity_req->config_drb_ciphering))
        {
            return RRC_FAILURE;
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconf_ue_entity_req->num_del_srb_entity <= ARRSIZE(p_rrc_pdcp_reconf_ue_entity_req->del_srb_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconf_ue_entity_req->num_del_srb_entity; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_del_srb_entity(pp_buffer, &p_rrc_pdcp_reconf_ue_entity_req->del_srb_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconf_ue_entity_req->num_del_drb_entity <= ARRSIZE(p_rrc_pdcp_reconf_ue_entity_req->del_drb_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconf_ue_entity_req->num_del_drb_entity; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_del_drb_entity(pp_buffer, &p_rrc_pdcp_reconf_ue_entity_req->del_drb_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present & RRC_PDCP_RECONFIG_UE_SPS_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_sps_config(pp_buffer, &p_rrc_pdcp_reconf_ue_entity_req->sps_config))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present & RRC_PDCP_RECONFIG_MACI_REQ_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_i_msg(pp_buffer, &p_rrc_pdcp_reconf_ue_entity_req->mac_i_req))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_reconf_ue_entity_req->optional_elems_present & RRC_PDCP_RECONFIG_UE_INACTIVITY_TIMER_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_ue_inactivity_timer(pp_buffer, &p_rrc_pdcp_reconf_ue_entity_req->ue_inactivity_timer))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_reconf_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_RECONFIG_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_reconf_ue_entity_req
(
    rrc_pdcp_reconf_ue_entity_req_t  *p_rrc_pdcp_reconf_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_reconf_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_reconf_ue_entity_req_len(p_rrc_pdcp_reconf_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_RECONFIG_UE_ENTITY_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_RECONFIG_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_RECONFIG_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_PDCP_RECONFIG_UE_ENTITY_REQ message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_reconf_ue_entity_req(&p_pdcp_msg, p_rrc_pdcp_reconf_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_rcfg_srb_entity_len
(
    rrc_pdcp_rcfg_srb_entity_t *p_rrc_pdcp_rcfg_srb_entity
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_rcfg_srb_entity != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_rcfg_srb_entity->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_rcfg_srb_entity->rb_direction);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_rcfg_srb_entity_tlv_len
(
    rrc_pdcp_rcfg_srb_entity_t *p_rrc_pdcp_rcfg_srb_entity

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_rcfg_srb_entity != PNULL);

    length += rrc_il_get_rrc_pdcp_rcfg_srb_entity_len(p_rrc_pdcp_rcfg_srb_entity);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_rcfg_srb_entity
(
    U8  **pp_buffer,
    rrc_pdcp_rcfg_srb_entity_t *p_rrc_pdcp_rcfg_srb_entity
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_rcfg_srb_entity != PNULL);

    /* This function composes rrc_pdcp_rcfg_srb_entity */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_RECONFIG_SRB_ENTITY_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_rcfg_srb_entity_tlv_len(p_rrc_pdcp_rcfg_srb_entity);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_rcfg_srb_entity->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_rcfg_srb_entity->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_rcfg_srb_entity->rb_direction, "rb_direction");
    *pp_buffer += sizeof(p_rrc_pdcp_rcfg_srb_entity->rb_direction);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_lwa_config_lwa_bearer_type_len
(
    rrc_pdcp_config_lwa_config_lwa_bearer_type_t *p_rrc_pdcp_config_lwa_config_lwa_bearer_type
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_lwa_config_lwa_bearer_type != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_config_lwa_config_lwa_bearer_type->bearer_type);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_lwa_config_lwa_bearer_type_tlv_len
(
    rrc_pdcp_config_lwa_config_lwa_bearer_type_t *p_rrc_pdcp_config_lwa_config_lwa_bearer_type

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_lwa_config_lwa_bearer_type != PNULL);

    length += rrc_il_get_rrc_pdcp_config_lwa_config_lwa_bearer_type_len(p_rrc_pdcp_config_lwa_config_lwa_bearer_type);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_lwa_config_lwa_bearer_type
(
    U8  **pp_buffer,
    rrc_pdcp_config_lwa_config_lwa_bearer_type_t *p_rrc_pdcp_config_lwa_config_lwa_bearer_type
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_config_lwa_config_lwa_bearer_type != PNULL);

    /* This function composes rrc_pdcp_config_lwa_config_lwa_bearer_type */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_LWA_CONFIG_BEARER_TYPE_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_config_lwa_config_lwa_bearer_type_tlv_len(p_rrc_pdcp_config_lwa_config_lwa_bearer_type);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_config_lwa_config_lwa_bearer_type->bearer_type, "bearer_type");
    *pp_buffer += sizeof(p_rrc_pdcp_config_lwa_config_lwa_bearer_type->bearer_type);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_lwa_len
(
    rrc_pdcp_config_lwa_t *p_rrc_pdcp_config_lwa
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_lwa != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_config_lwa->lwa_bearer);

    /* Get length of TLV */
    if (p_rrc_pdcp_config_lwa->optional_elems_present & RRC_PDCP_DRB_CONFIG_LWA_BEARER_TYPE_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_config_lwa_config_lwa_bearer_type_tlv_len(&p_rrc_pdcp_config_lwa->lwa_bearer_type);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_config_lwa_tlv_len
(
    rrc_pdcp_config_lwa_t *p_rrc_pdcp_config_lwa

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_lwa != PNULL);

    length += rrc_il_get_rrc_pdcp_config_lwa_len(p_rrc_pdcp_config_lwa);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_config_lwa
(
    U8  **pp_buffer,
    rrc_pdcp_config_lwa_t *p_rrc_pdcp_config_lwa
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_config_lwa != PNULL);

    /* This function composes rrc_pdcp_config_lwa */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_CONFIGURE_LWA_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_config_lwa_tlv_len(p_rrc_pdcp_config_lwa);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_config_lwa->lwa_bearer, "lwa_bearer");
    *pp_buffer += sizeof(p_rrc_pdcp_config_lwa->lwa_bearer);

    /* Compose TLV  */
    if (p_rrc_pdcp_config_lwa->optional_elems_present & RRC_PDCP_DRB_CONFIG_LWA_BEARER_TYPE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_config_lwa_config_lwa_bearer_type(pp_buffer, &p_rrc_pdcp_config_lwa->lwa_bearer_type))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_rcfg_drb_entity_len
(
    rrc_pdcp_rcfg_drb_entity_t *p_rrc_pdcp_rcfg_drb_entity
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_rcfg_drb_entity != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_rcfg_drb_entity->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_rcfg_drb_entity->rb_direction);

    /* Get length of TLV */
    if (p_rrc_pdcp_rcfg_drb_entity->optional_elems_present & RRC_PDCP_RCFG_DRB_CONFIG_LWA_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_config_lwa_tlv_len(&p_rrc_pdcp_rcfg_drb_entity->lwa_config);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_rcfg_drb_entity->optional_elems_present & RRC_PDCP_RCFG_DRB_CONFIG_ROHC_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_config_rohc_tlv_len(&p_rrc_pdcp_rcfg_drb_entity->config_rohc);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_rcfg_drb_entity->optional_elems_present & RRC_PDCP_RCFG_DRB_CONFIG_ST_REPORT_REQUIRED_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_config_st_rep_required_tlv_len(&p_rrc_pdcp_rcfg_drb_entity->st_rep_required);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_rcfg_drb_entity->optional_elems_present & RRC_PDCP_RCFG_DRB_CONFIG_UE_ST_REPORT_REQUIRED_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_config_ue_st_rep_required_tlv_len(&p_rrc_pdcp_rcfg_drb_entity->ue_st_rep_required);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_rcfg_drb_entity_tlv_len
(
    rrc_pdcp_rcfg_drb_entity_t *p_rrc_pdcp_rcfg_drb_entity

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_rcfg_drb_entity != PNULL);

    length += rrc_il_get_rrc_pdcp_rcfg_drb_entity_len(p_rrc_pdcp_rcfg_drb_entity);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_rcfg_drb_entity
(
    U8  **pp_buffer,
    rrc_pdcp_rcfg_drb_entity_t *p_rrc_pdcp_rcfg_drb_entity
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_rcfg_drb_entity != PNULL);

    /* This function composes rrc_pdcp_rcfg_drb_entity */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_RECONFIG_DRB_ENTITY_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_rcfg_drb_entity_tlv_len(p_rrc_pdcp_rcfg_drb_entity);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_rcfg_drb_entity->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_rcfg_drb_entity->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_rcfg_drb_entity->rb_direction, "rb_direction");
    *pp_buffer += sizeof(p_rrc_pdcp_rcfg_drb_entity->rb_direction);

    /* Compose TLV  */
    if (p_rrc_pdcp_rcfg_drb_entity->optional_elems_present & RRC_PDCP_RCFG_DRB_CONFIG_LWA_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_config_lwa(pp_buffer, &p_rrc_pdcp_rcfg_drb_entity->lwa_config))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_rcfg_drb_entity->optional_elems_present & RRC_PDCP_RCFG_DRB_CONFIG_ROHC_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_config_rohc(pp_buffer, &p_rrc_pdcp_rcfg_drb_entity->config_rohc))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_rcfg_drb_entity->optional_elems_present & RRC_PDCP_RCFG_DRB_CONFIG_ST_REPORT_REQUIRED_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_config_st_rep_required(pp_buffer, &p_rrc_pdcp_rcfg_drb_entity->st_rep_required))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_rcfg_drb_entity->optional_elems_present & RRC_PDCP_RCFG_DRB_CONFIG_UE_ST_REPORT_REQUIRED_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_config_ue_st_rep_required(pp_buffer, &p_rrc_pdcp_rcfg_drb_entity->ue_st_rep_required))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_sec_offset_len
(
    rrc_pdcp_sec_offset_t *p_rrc_pdcp_sec_offset
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_sec_offset != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_sec_offset->configure_sec_offset);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_sec_offset_tlv_len
(
    rrc_pdcp_sec_offset_t *p_rrc_pdcp_sec_offset

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_sec_offset != PNULL);

    length += rrc_il_get_rrc_pdcp_sec_offset_len(p_rrc_pdcp_sec_offset);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_sec_offset
(
    U8  **pp_buffer,
    rrc_pdcp_sec_offset_t *p_rrc_pdcp_sec_offset
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_sec_offset != PNULL);

    /* This function composes rrc_pdcp_sec_offset */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_SECURITY_OFFSET_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_sec_offset_tlv_len(p_rrc_pdcp_sec_offset);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_pdcp_sec_offset->configure_sec_offset < 1) || (p_rrc_pdcp_sec_offset->configure_sec_offset > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_sec_offset->configure_sec_offset] should be in range "
            "1 to 1. Incorrect value %u received.", p_rrc_pdcp_sec_offset->configure_sec_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_sec_offset->configure_sec_offset, "configure_sec_offset");
    *pp_buffer += sizeof(p_rrc_pdcp_sec_offset->configure_sec_offset);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_del_srb_entity_len
(
    rrc_pdcp_del_srb_entity_t *p_rrc_pdcp_del_srb_entity
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_del_srb_entity != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_del_srb_entity->lc_id);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_del_srb_entity_tlv_len
(
    rrc_pdcp_del_srb_entity_t *p_rrc_pdcp_del_srb_entity

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_del_srb_entity != PNULL);

    length += rrc_il_get_rrc_pdcp_del_srb_entity_len(p_rrc_pdcp_del_srb_entity);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_del_srb_entity
(
    U8  **pp_buffer,
    rrc_pdcp_del_srb_entity_t *p_rrc_pdcp_del_srb_entity
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_del_srb_entity != PNULL);

    /* This function composes rrc_pdcp_del_srb_entity */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_DELETE_SRB_ENTITY_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_del_srb_entity_tlv_len(p_rrc_pdcp_del_srb_entity);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_del_srb_entity->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_del_srb_entity->lc_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_del_drb_entity_len
(
    rrc_pdcp_del_drb_entity_t *p_rrc_pdcp_del_drb_entity
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_del_drb_entity != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_del_drb_entity->lc_id);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_del_drb_entity_tlv_len
(
    rrc_pdcp_del_drb_entity_t *p_rrc_pdcp_del_drb_entity

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_del_drb_entity != PNULL);

    length += rrc_il_get_rrc_pdcp_del_drb_entity_len(p_rrc_pdcp_del_drb_entity);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_del_drb_entity
(
    U8  **pp_buffer,
    rrc_pdcp_del_drb_entity_t *p_rrc_pdcp_del_drb_entity
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_del_drb_entity != PNULL);

    /* This function composes rrc_pdcp_del_drb_entity */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_DELETE_DRB_ENTITY_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_del_drb_entity_tlv_len(p_rrc_pdcp_del_drb_entity);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_del_drb_entity->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_del_drb_entity->lc_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_reconfig_ue_entity_cnf_len
(
    rrc_pdcp_reconfig_ue_entity_cnf_t *p_rrc_pdcp_reconfig_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_reconfig_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_reconfig_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_reconfig_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconfig_ue_entity_cnf->num_cr_srb_entity_error <= ARRSIZE(p_rrc_pdcp_reconfig_ue_entity_cnf->cr_srb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconfig_ue_entity_cnf->num_cr_srb_entity_error; loop++)
        {
            length += rrc_il_get_rrc_pdcp_cr_srb_entity_error_tlv_len(&p_rrc_pdcp_reconfig_ue_entity_cnf->cr_srb_error_entities[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconfig_ue_entity_cnf->num_cr_drb_entity_error <= ARRSIZE(p_rrc_pdcp_reconfig_ue_entity_cnf->cr_drb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconfig_ue_entity_cnf->num_cr_drb_entity_error; loop++)
        {
            length += rrc_il_get_rrc_pdcp_cr_drb_entity_error_tlv_len(&p_rrc_pdcp_reconfig_ue_entity_cnf->cr_drb_error_entities[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconfig_ue_entity_cnf->num_del_srb_entity_error <= ARRSIZE(p_rrc_pdcp_reconfig_ue_entity_cnf->del_srb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconfig_ue_entity_cnf->num_del_srb_entity_error; loop++)
        {
            length += rrc_il_get_rrc_pdcp_del_srb_entity_error_tlv_len(&p_rrc_pdcp_reconfig_ue_entity_cnf->del_srb_error_entities[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconfig_ue_entity_cnf->num_del_drb_entity_error <= ARRSIZE(p_rrc_pdcp_reconfig_ue_entity_cnf->del_drb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconfig_ue_entity_cnf->num_del_drb_entity_error; loop++)
        {
            length += rrc_il_get_rrc_pdcp_del_drb_entity_error_tlv_len(&p_rrc_pdcp_reconfig_ue_entity_cnf->del_drb_error_entities[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconfig_ue_entity_cnf->num_rcfg_srb_entity_error <= ARRSIZE(p_rrc_pdcp_reconfig_ue_entity_cnf->rcfg_srb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconfig_ue_entity_cnf->num_rcfg_srb_entity_error; loop++)
        {
            length += rrc_il_get_rrc_pdcp_rcfg_srb_entity_error_tlv_len(&p_rrc_pdcp_reconfig_ue_entity_cnf->rcfg_srb_error_entities[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconfig_ue_entity_cnf->num_rcfg_drb_entity_error <= ARRSIZE(p_rrc_pdcp_reconfig_ue_entity_cnf->rcfg_drb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconfig_ue_entity_cnf->num_rcfg_drb_entity_error; loop++)
        {
            length += rrc_il_get_rrc_pdcp_rcfg_drb_entity_error_tlv_len(&p_rrc_pdcp_reconfig_ue_entity_cnf->rcfg_drb_error_entities[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_reconfig_ue_entity_cnf->optional_elems_present & RRC_PDCP_RECONFIG_MACI_RESP_PRESENT)
    {
        length += rrc_il_get_rrc_mac_i_tlv_len(&p_rrc_pdcp_reconfig_ue_entity_cnf->mac_i);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_reconfig_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_pdcp_reconfig_ue_entity_cnf_t *p_rrc_pdcp_reconfig_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_reconfig_ue_entity_cnf != PNULL);

    /* This function composes rrc_pdcp_reconfig_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_reconfig_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_reconfig_ue_entity_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_reconfig_ue_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_reconfig_ue_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_pdcp_reconfig_ue_entity_cnf->response_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_reconfig_ue_entity_cnf->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_pdcp_reconfig_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconfig_ue_entity_cnf->num_cr_srb_entity_error <= ARRSIZE(p_rrc_pdcp_reconfig_ue_entity_cnf->cr_srb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconfig_ue_entity_cnf->num_cr_srb_entity_error; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_cr_srb_entity_error(pp_buffer, &p_rrc_pdcp_reconfig_ue_entity_cnf->cr_srb_error_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconfig_ue_entity_cnf->num_cr_drb_entity_error <= ARRSIZE(p_rrc_pdcp_reconfig_ue_entity_cnf->cr_drb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconfig_ue_entity_cnf->num_cr_drb_entity_error; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_cr_drb_entity_error(pp_buffer, &p_rrc_pdcp_reconfig_ue_entity_cnf->cr_drb_error_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconfig_ue_entity_cnf->num_del_srb_entity_error <= ARRSIZE(p_rrc_pdcp_reconfig_ue_entity_cnf->del_srb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconfig_ue_entity_cnf->num_del_srb_entity_error; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_del_srb_entity_error(pp_buffer, &p_rrc_pdcp_reconfig_ue_entity_cnf->del_srb_error_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconfig_ue_entity_cnf->num_del_drb_entity_error <= ARRSIZE(p_rrc_pdcp_reconfig_ue_entity_cnf->del_drb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconfig_ue_entity_cnf->num_del_drb_entity_error; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_del_drb_entity_error(pp_buffer, &p_rrc_pdcp_reconfig_ue_entity_cnf->del_drb_error_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconfig_ue_entity_cnf->num_rcfg_srb_entity_error <= ARRSIZE(p_rrc_pdcp_reconfig_ue_entity_cnf->rcfg_srb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconfig_ue_entity_cnf->num_rcfg_srb_entity_error; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_rcfg_srb_entity_error(pp_buffer, &p_rrc_pdcp_reconfig_ue_entity_cnf->rcfg_srb_error_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_reconfig_ue_entity_cnf->num_rcfg_drb_entity_error <= ARRSIZE(p_rrc_pdcp_reconfig_ue_entity_cnf->rcfg_drb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_reconfig_ue_entity_cnf->num_rcfg_drb_entity_error; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_rcfg_drb_entity_error(pp_buffer, &p_rrc_pdcp_reconfig_ue_entity_cnf->rcfg_drb_error_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_reconfig_ue_entity_cnf->optional_elems_present & RRC_PDCP_RECONFIG_MACI_RESP_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_i(pp_buffer, &p_rrc_pdcp_reconfig_ue_entity_cnf->mac_i))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_reconfig_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_RECONFIG_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_reconfig_ue_entity_cnf
(
    rrc_pdcp_reconfig_ue_entity_cnf_t  *p_rrc_pdcp_reconfig_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_reconfig_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_reconfig_ue_entity_cnf_len(p_rrc_pdcp_reconfig_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_RECONFIG_UE_ENTITY_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_RECONFIG_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_RECONFIG_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_PDCP_RECONFIG_UE_ENTITY_CNF message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_reconfig_ue_entity_cnf(&p_pdcp_msg, p_rrc_pdcp_reconfig_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_del_srb_entity_error_len
(
    rrc_pdcp_del_srb_entity_error_t *p_rrc_pdcp_del_srb_entity_error
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_del_srb_entity_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_del_srb_entity_error->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_del_srb_entity_error->response);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_del_srb_entity_error_tlv_len
(
    rrc_pdcp_del_srb_entity_error_t *p_rrc_pdcp_del_srb_entity_error

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_del_srb_entity_error != PNULL);

    length += rrc_il_get_rrc_pdcp_del_srb_entity_error_len(p_rrc_pdcp_del_srb_entity_error);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_del_srb_entity_error
(
    U8  **pp_buffer,
    rrc_pdcp_del_srb_entity_error_t *p_rrc_pdcp_del_srb_entity_error
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_del_srb_entity_error != PNULL);

    /* This function composes rrc_pdcp_del_srb_entity_error */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_DELETE_SRB_ENTITY_ERROR_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_del_srb_entity_error_tlv_len(p_rrc_pdcp_del_srb_entity_error);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_del_srb_entity_error->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_del_srb_entity_error->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_del_srb_entity_error->response, "response");
    *pp_buffer += sizeof(p_rrc_pdcp_del_srb_entity_error->response);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_del_drb_entity_error_len
(
    rrc_pdcp_del_drb_entity_error_t *p_rrc_pdcp_del_drb_entity_error
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_del_drb_entity_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_del_drb_entity_error->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_del_drb_entity_error->response);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_del_drb_entity_error_tlv_len
(
    rrc_pdcp_del_drb_entity_error_t *p_rrc_pdcp_del_drb_entity_error

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_del_drb_entity_error != PNULL);

    length += rrc_il_get_rrc_pdcp_del_drb_entity_error_len(p_rrc_pdcp_del_drb_entity_error);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_del_drb_entity_error
(
    U8  **pp_buffer,
    rrc_pdcp_del_drb_entity_error_t *p_rrc_pdcp_del_drb_entity_error
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_del_drb_entity_error != PNULL);

    /* This function composes rrc_pdcp_del_drb_entity_error */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_DELETE_DRB_ENTITY_ERROR_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_del_drb_entity_error_tlv_len(p_rrc_pdcp_del_drb_entity_error);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_del_drb_entity_error->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_del_drb_entity_error->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_del_drb_entity_error->response, "response");
    *pp_buffer += sizeof(p_rrc_pdcp_del_drb_entity_error->response);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_rcfg_srb_entity_error_len
(
    rrc_pdcp_rcfg_srb_entity_error_t *p_rrc_pdcp_rcfg_srb_entity_error
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_rcfg_srb_entity_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_rcfg_srb_entity_error->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_rcfg_srb_entity_error->response);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_rcfg_srb_entity_error_tlv_len
(
    rrc_pdcp_rcfg_srb_entity_error_t *p_rrc_pdcp_rcfg_srb_entity_error

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_rcfg_srb_entity_error != PNULL);

    length += rrc_il_get_rrc_pdcp_rcfg_srb_entity_error_len(p_rrc_pdcp_rcfg_srb_entity_error);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_rcfg_srb_entity_error
(
    U8  **pp_buffer,
    rrc_pdcp_rcfg_srb_entity_error_t *p_rrc_pdcp_rcfg_srb_entity_error
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_rcfg_srb_entity_error != PNULL);

    /* This function composes rrc_pdcp_rcfg_srb_entity_error */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_RECONFIG_SRB_ENTITY_ERROR_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_rcfg_srb_entity_error_tlv_len(p_rrc_pdcp_rcfg_srb_entity_error);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_rcfg_srb_entity_error->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_rcfg_srb_entity_error->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_rcfg_srb_entity_error->response, "response");
    *pp_buffer += sizeof(p_rrc_pdcp_rcfg_srb_entity_error->response);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_rcfg_drb_entity_error_len
(
    rrc_pdcp_rcfg_drb_entity_error_t *p_rrc_pdcp_rcfg_drb_entity_error
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_rcfg_drb_entity_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_rcfg_drb_entity_error->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_rcfg_drb_entity_error->response);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_rcfg_drb_entity_error_tlv_len
(
    rrc_pdcp_rcfg_drb_entity_error_t *p_rrc_pdcp_rcfg_drb_entity_error

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_rcfg_drb_entity_error != PNULL);

    length += rrc_il_get_rrc_pdcp_rcfg_drb_entity_error_len(p_rrc_pdcp_rcfg_drb_entity_error);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_rcfg_drb_entity_error
(
    U8  **pp_buffer,
    rrc_pdcp_rcfg_drb_entity_error_t *p_rrc_pdcp_rcfg_drb_entity_error
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_rcfg_drb_entity_error != PNULL);

    /* This function composes rrc_pdcp_rcfg_drb_entity_error */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_RECONFIG_DRB_ENTITY_ERROR_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_rcfg_drb_entity_error_tlv_len(p_rrc_pdcp_rcfg_drb_entity_error);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_rcfg_drb_entity_error->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_rcfg_drb_entity_error->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_rcfg_drb_entity_error->response, "response");
    *pp_buffer += sizeof(p_rrc_pdcp_rcfg_drb_entity_error->response);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_srb_data_req_len
(
    rrc_pdcp_srb_data_req_t *p_rrc_pdcp_srb_data_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_srb_data_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_srb_data_req->transaction_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_srb_data_req->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_srb_data_req->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_srb_data_req->service_requested);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_srb_data_req
(
    U8  **pp_buffer,
    rrc_pdcp_srb_data_req_t *p_rrc_pdcp_srb_data_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_srb_data_req != PNULL);

    /* This function composes rrc_pdcp_srb_data_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_pdcp_srb_data_req->transaction_id, "transaction_id");
    *pp_buffer += sizeof(p_rrc_pdcp_srb_data_req->transaction_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_srb_data_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_srb_data_req->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_srb_data_req->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_srb_data_req->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_srb_data_req->service_requested, "service_requested");
    *pp_buffer += sizeof(p_rrc_pdcp_srb_data_req->service_requested);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_srb_data_ind_len
(
    rrc_pdcp_srb_data_ind_t *p_rrc_pdcp_srb_data_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_srb_data_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_srb_data_ind->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_srb_data_ind->lc_id);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_srb_data_ind
(
    U8  **pp_buffer,
    rrc_pdcp_srb_data_ind_t *p_rrc_pdcp_srb_data_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_srb_data_ind != PNULL);

    /* This function composes rrc_pdcp_srb_data_ind */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_srb_data_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_srb_data_ind->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_srb_data_ind->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_srb_data_ind->lc_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_srb_data_status_ind_len
(
    rrc_pdcp_srb_data_status_ind_t *p_rrc_pdcp_srb_data_status_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_srb_data_status_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_srb_data_status_ind->transaction_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_srb_data_status_ind->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_srb_data_status_ind->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_srb_data_status_ind->response_code);

    /* Get length of TLV */
    if (p_rrc_pdcp_srb_data_status_ind->optional_elems_present & RRC_PDCP_SRB_DATA_STATUS_ERROR_PRESENT)
    {
        length += rrc_il_get_rrc_pdcp_data_status_error_tlv_len(&p_rrc_pdcp_srb_data_status_ind->srb_data_status_error);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_srb_data_status_ind
(
    U8  **pp_buffer,
    rrc_pdcp_srb_data_status_ind_t *p_rrc_pdcp_srb_data_status_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_srb_data_status_ind != PNULL);

    /* This function composes rrc_pdcp_srb_data_status_ind */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_pdcp_srb_data_status_ind->transaction_id, "transaction_id");
    *pp_buffer += sizeof(p_rrc_pdcp_srb_data_status_ind->transaction_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_srb_data_status_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_srb_data_status_ind->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_srb_data_status_ind->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_srb_data_status_ind->lc_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_srb_data_status_ind->response_code > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_srb_data_status_ind->response_code] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_pdcp_srb_data_status_ind->response_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_srb_data_status_ind->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_pdcp_srb_data_status_ind->response_code);

    /* Compose TLV  */
    if (p_rrc_pdcp_srb_data_status_ind->optional_elems_present & RRC_PDCP_SRB_DATA_STATUS_ERROR_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_data_status_error(pp_buffer, &p_rrc_pdcp_srb_data_status_ind->srb_data_status_error))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_srb_data_status_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_SRB_DATA_STATUS_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_srb_data_status_ind
(
    rrc_pdcp_srb_data_status_ind_t  *p_rrc_pdcp_srb_data_status_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_srb_data_status_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_srb_data_status_ind_len(p_rrc_pdcp_srb_data_status_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_SRB_DATA_STATUS_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_SRB_DATA_STATUS_IND, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_SRB_DATA_STATUS_IND, msg_length,cell_index);

    /* Fill RRC_PDCP_SRB_DATA_STATUS_IND message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_srb_data_status_ind(&p_pdcp_msg, p_rrc_pdcp_srb_data_status_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_data_status_error_len
(
    rrc_pdcp_data_status_error_t *p_rrc_pdcp_data_status_error
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_data_status_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_data_status_error->error_code);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_data_status_error_tlv_len
(
    rrc_pdcp_data_status_error_t *p_rrc_pdcp_data_status_error

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_data_status_error != PNULL);

    length += rrc_il_get_rrc_pdcp_data_status_error_len(p_rrc_pdcp_data_status_error);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_data_status_error
(
    U8  **pp_buffer,
    rrc_pdcp_data_status_error_t *p_rrc_pdcp_data_status_error
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_data_status_error != PNULL);

    /* This function composes rrc_pdcp_data_status_error */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_SRB_DATA_STATUS_ERROR_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_data_status_error_tlv_len(p_rrc_pdcp_data_status_error);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_data_status_error->error_code, "error_code");
    *pp_buffer += sizeof(p_rrc_pdcp_data_status_error->error_code);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_suspend_ue_entity_req_len
(
    rrc_pdcp_suspend_ue_entity_req_t *p_rrc_pdcp_suspend_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_suspend_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_suspend_ue_entity_req->ue_index);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_suspend_ue_entity_req
(
    U8  **pp_buffer,
    rrc_pdcp_suspend_ue_entity_req_t *p_rrc_pdcp_suspend_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_suspend_ue_entity_req != PNULL);

    /* This function composes rrc_pdcp_suspend_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_suspend_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_suspend_ue_entity_req->ue_index);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_suspend_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_SUSPEND_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_suspend_ue_entity_req
(
    rrc_pdcp_suspend_ue_entity_req_t  *p_rrc_pdcp_suspend_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_suspend_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_suspend_ue_entity_req_len(p_rrc_pdcp_suspend_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_SUSPEND_UE_ENTITY_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_SUSPEND_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_SUSPEND_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_PDCP_SUSPEND_UE_ENTITY_REQ message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_suspend_ue_entity_req(&p_pdcp_msg, p_rrc_pdcp_suspend_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_suspend_ue_entity_cnf_len
(
    rrc_pdcp_suspend_ue_entity_cnf_t *p_rrc_pdcp_suspend_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_suspend_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_suspend_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_suspend_ue_entity_cnf->response_code);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_suspend_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_pdcp_suspend_ue_entity_cnf_t *p_rrc_pdcp_suspend_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_suspend_ue_entity_cnf != PNULL);

    /* This function composes rrc_pdcp_suspend_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_suspend_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_suspend_ue_entity_cnf->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_suspend_ue_entity_cnf->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_pdcp_suspend_ue_entity_cnf->response_code);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_suspend_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_SUSPEND_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_suspend_ue_entity_cnf
(
    rrc_pdcp_suspend_ue_entity_cnf_t  *p_rrc_pdcp_suspend_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_suspend_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_suspend_ue_entity_cnf_len(p_rrc_pdcp_suspend_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_SUSPEND_UE_ENTITY_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_SUSPEND_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_SUSPEND_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_PDCP_SUSPEND_UE_ENTITY_CNF message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_suspend_ue_entity_cnf(&p_pdcp_msg, p_rrc_pdcp_suspend_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_sn_hfn_status_req_len
(
    rrc_pdcp_sn_hfn_status_req_t *p_rrc_pdcp_sn_hfn_status_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_sn_hfn_status_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_sn_hfn_status_req->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_sn_hfn_status_req->sn_hfn_required);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_sn_hfn_status_req->num_lossy_drb_entity <= ARRSIZE(p_rrc_pdcp_sn_hfn_status_req->lossy_ho_required));
        for (loop = 0; loop < p_rrc_pdcp_sn_hfn_status_req->num_lossy_drb_entity; loop++)
        {
            length += rrc_il_get_rrc_lossy_ho_required_tlv_len(&p_rrc_pdcp_sn_hfn_status_req->lossy_ho_required[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_sn_hfn_status_req
(
    U8  **pp_buffer,
    rrc_pdcp_sn_hfn_status_req_t *p_rrc_pdcp_sn_hfn_status_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_sn_hfn_status_req != PNULL);

    /* This function composes rrc_pdcp_sn_hfn_status_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_sn_hfn_status_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_sn_hfn_status_req->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_sn_hfn_status_req->sn_hfn_required, "sn_hfn_required");
    *pp_buffer += sizeof(p_rrc_pdcp_sn_hfn_status_req->sn_hfn_required);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_sn_hfn_status_req->num_lossy_drb_entity <= ARRSIZE(p_rrc_pdcp_sn_hfn_status_req->lossy_ho_required));
        for (loop = 0; loop < p_rrc_pdcp_sn_hfn_status_req->num_lossy_drb_entity; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_lossy_ho_required(pp_buffer, &p_rrc_pdcp_sn_hfn_status_req->lossy_ho_required[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_sn_hfn_status_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_SN_HFN_STATUS_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_sn_hfn_status_req
(
    rrc_pdcp_sn_hfn_status_req_t  *p_rrc_pdcp_sn_hfn_status_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_sn_hfn_status_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_sn_hfn_status_req_len(p_rrc_pdcp_sn_hfn_status_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_SN_HFN_STATUS_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_SN_HFN_STATUS_REQ, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_SN_HFN_STATUS_REQ, msg_length,cell_index);

    /* Fill RRC_PDCP_SN_HFN_STATUS_REQ message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_sn_hfn_status_req(&p_pdcp_msg, p_rrc_pdcp_sn_hfn_status_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_lossy_ho_required_len
(
    rrc_lossy_ho_required_t *p_rrc_lossy_ho_required
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_lossy_ho_required != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_lossy_ho_required->lc_id);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_lossy_ho_required_tlv_len
(
    rrc_lossy_ho_required_t *p_rrc_lossy_ho_required

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_lossy_ho_required != PNULL);

    length += rrc_il_get_rrc_lossy_ho_required_len(p_rrc_lossy_ho_required);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_lossy_ho_required
(
    U8  **pp_buffer,
    rrc_lossy_ho_required_t *p_rrc_lossy_ho_required
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_lossy_ho_required != PNULL);

    /* This function composes rrc_lossy_ho_required */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_LOSSY_HO_REQUIRED_TAG;
    tlv_length = rrc_il_get_rrc_lossy_ho_required_tlv_len(p_rrc_lossy_ho_required);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_lossy_ho_required->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_lossy_ho_required->lc_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_sn_hfn_status_resp_len
(
    rrc_pdcp_sn_hfn_status_resp_t *p_rrc_pdcp_sn_hfn_status_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_sn_hfn_status_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_sn_hfn_status_resp->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_sn_hfn_status_resp->response_code);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_sn_hfn_status_resp->num_lc_sn_hfn_status <= ARRSIZE(p_rrc_pdcp_sn_hfn_status_resp->lc_sn_hfn_status));
        for (loop = 0; loop < p_rrc_pdcp_sn_hfn_status_resp->num_lc_sn_hfn_status; loop++)
        {
            length += rrc_il_get_rrc_lc_sn_hfn_status_tlv_len(&p_rrc_pdcp_sn_hfn_status_resp->lc_sn_hfn_status[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_sn_hfn_status_resp
(
    U8  **pp_buffer,
    rrc_pdcp_sn_hfn_status_resp_t *p_rrc_pdcp_sn_hfn_status_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_sn_hfn_status_resp != PNULL);

    /* This function composes rrc_pdcp_sn_hfn_status_resp */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_sn_hfn_status_resp->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_sn_hfn_status_resp->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_sn_hfn_status_resp->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_sn_hfn_status_resp->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_pdcp_sn_hfn_status_resp->response_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_sn_hfn_status_resp->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_pdcp_sn_hfn_status_resp->response_code);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_sn_hfn_status_resp->num_lc_sn_hfn_status <= ARRSIZE(p_rrc_pdcp_sn_hfn_status_resp->lc_sn_hfn_status));
        for (loop = 0; loop < p_rrc_pdcp_sn_hfn_status_resp->num_lc_sn_hfn_status; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_lc_sn_hfn_status(pp_buffer, &p_rrc_pdcp_sn_hfn_status_resp->lc_sn_hfn_status[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_sn_hfn_status_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_SN_HFN_STATUS_RESP message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_sn_hfn_status_resp
(
    rrc_pdcp_sn_hfn_status_resp_t  *p_rrc_pdcp_sn_hfn_status_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_sn_hfn_status_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_sn_hfn_status_resp_len(p_rrc_pdcp_sn_hfn_status_resp);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_SN_HFN_STATUS_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_SN_HFN_STATUS_RESP, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_SN_HFN_STATUS_RESP, msg_length,cell_index);

    /* Fill RRC_PDCP_SN_HFN_STATUS_RESP message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_sn_hfn_status_resp(&p_pdcp_msg, p_rrc_pdcp_sn_hfn_status_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_ul_rcv_sn_val_len
(
    ul_rcv_sn_val_t *p_ul_rcv_sn_val
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_ul_rcv_sn_val != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_ul_rcv_sn_val->ul_rcv_sn);

    return length;
}

static
rrc_length_t
rrc_il_get_ul_rcv_sn_val_tlv_len
(
    ul_rcv_sn_val_t *p_ul_rcv_sn_val

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_ul_rcv_sn_val != PNULL);

    length += rrc_il_get_ul_rcv_sn_val_len(p_ul_rcv_sn_val);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_ul_rcv_sn_val
(
    U8  **pp_buffer,
    ul_rcv_sn_val_t *p_ul_rcv_sn_val
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_ul_rcv_sn_val != PNULL);

    /* This function composes ul_rcv_sn_val */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_UL_RECV_SN_TAG;
    tlv_length = rrc_il_get_ul_rcv_sn_val_tlv_len(p_ul_rcv_sn_val);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_ul_rcv_sn_val->ul_rcv_sn); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_ul_rcv_sn_val->ul_rcv_sn[loop], "ul_rcv_sn[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_lc_sn_hfn_status_len
(
    rrc_lc_sn_hfn_status_t *p_rrc_lc_sn_hfn_status
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_lc_sn_hfn_status != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_lc_sn_hfn_status->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_lc_sn_hfn_status->dl_sn_cnt);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_lc_sn_hfn_status->dl_hfn_cnt);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_lc_sn_hfn_status->ul_sn_cnt);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_lc_sn_hfn_status->ul_hfn_cnt);

    /* Get length of TLV */
    if (p_rrc_lc_sn_hfn_status->optional_elems_present & RRC_PDCP_UL_RECV_SN_PRESENT)
    {
        length += rrc_il_get_ul_rcv_sn_val_tlv_len(&p_rrc_lc_sn_hfn_status->ul_rcv_sn_val);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_lc_sn_hfn_status_tlv_len
(
    rrc_lc_sn_hfn_status_t *p_rrc_lc_sn_hfn_status

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_lc_sn_hfn_status != PNULL);

    length += rrc_il_get_rrc_lc_sn_hfn_status_len(p_rrc_lc_sn_hfn_status);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_lc_sn_hfn_status
(
    U8  **pp_buffer,
    rrc_lc_sn_hfn_status_t *p_rrc_lc_sn_hfn_status
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_lc_sn_hfn_status != PNULL);

    /* This function composes rrc_lc_sn_hfn_status */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_LC_SN_HFN_STATUS_TAG;
    tlv_length = rrc_il_get_rrc_lc_sn_hfn_status_tlv_len(p_rrc_lc_sn_hfn_status);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_lc_sn_hfn_status->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_lc_sn_hfn_status->lc_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_lc_sn_hfn_status->dl_sn_cnt > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_lc_sn_hfn_status->dl_sn_cnt] should be less than"
            " or equal to 4095. Incorrect value %u received.", p_rrc_lc_sn_hfn_status->dl_sn_cnt);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_lc_sn_hfn_status->dl_sn_cnt, "dl_sn_cnt");
    *pp_buffer += sizeof(p_rrc_lc_sn_hfn_status->dl_sn_cnt);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_lc_sn_hfn_status->dl_hfn_cnt > 1048575))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_lc_sn_hfn_status->dl_hfn_cnt] should be less than"
            " or equal to 1048575. Incorrect value %u received.", p_rrc_lc_sn_hfn_status->dl_hfn_cnt);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_lc_sn_hfn_status->dl_hfn_cnt, "dl_hfn_cnt");
    *pp_buffer += sizeof(p_rrc_lc_sn_hfn_status->dl_hfn_cnt);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_lc_sn_hfn_status->ul_sn_cnt > 4095))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_lc_sn_hfn_status->ul_sn_cnt] should be less than"
            " or equal to 4095. Incorrect value %u received.", p_rrc_lc_sn_hfn_status->ul_sn_cnt);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_lc_sn_hfn_status->ul_sn_cnt, "ul_sn_cnt");
    *pp_buffer += sizeof(p_rrc_lc_sn_hfn_status->ul_sn_cnt);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_lc_sn_hfn_status->ul_hfn_cnt > 1048575))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_lc_sn_hfn_status->ul_hfn_cnt] should be less than"
            " or equal to 1048575. Incorrect value %u received.", p_rrc_lc_sn_hfn_status->ul_hfn_cnt);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_lc_sn_hfn_status->ul_hfn_cnt, "ul_hfn_cnt");
    *pp_buffer += sizeof(p_rrc_lc_sn_hfn_status->ul_hfn_cnt);

    /* Compose TLV  */
    if (p_rrc_lc_sn_hfn_status->optional_elems_present & RRC_PDCP_UL_RECV_SN_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_ul_rcv_sn_val(pp_buffer, &p_rrc_lc_sn_hfn_status->ul_rcv_sn_val))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_sn_hfn_status_ind_len
(
    rrc_pdcp_sn_hfn_status_ind_t *p_rrc_pdcp_sn_hfn_status_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_sn_hfn_status_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_sn_hfn_status_ind->ue_index);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_sn_hfn_status_ind->num_lc_sn_hfn_status <= ARRSIZE(p_rrc_pdcp_sn_hfn_status_ind->lc_sn_hfn_status));
        for (loop = 0; loop < p_rrc_pdcp_sn_hfn_status_ind->num_lc_sn_hfn_status; loop++)
        {
            length += rrc_il_get_rrc_lc_sn_hfn_status_tlv_len(&p_rrc_pdcp_sn_hfn_status_ind->lc_sn_hfn_status[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_sn_hfn_status_ind
(
    U8  **pp_buffer,
    rrc_pdcp_sn_hfn_status_ind_t *p_rrc_pdcp_sn_hfn_status_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_sn_hfn_status_ind != PNULL);

    /* This function composes rrc_pdcp_sn_hfn_status_ind */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_sn_hfn_status_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_sn_hfn_status_ind->ue_index);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_sn_hfn_status_ind->num_lc_sn_hfn_status <= ARRSIZE(p_rrc_pdcp_sn_hfn_status_ind->lc_sn_hfn_status));
        for (loop = 0; loop < p_rrc_pdcp_sn_hfn_status_ind->num_lc_sn_hfn_status; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_lc_sn_hfn_status(pp_buffer, &p_rrc_pdcp_sn_hfn_status_ind->lc_sn_hfn_status[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_sn_hfn_status_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_SN_HFN_STATUS_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_sn_hfn_status_ind
(
    rrc_pdcp_sn_hfn_status_ind_t  *p_rrc_pdcp_sn_hfn_status_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_sn_hfn_status_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_sn_hfn_status_ind_len(p_rrc_pdcp_sn_hfn_status_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_SN_HFN_STATUS_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_SN_HFN_STATUS_IND, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_SN_HFN_STATUS_IND, msg_length,cell_index);

    /* Fill RRC_PDCP_SN_HFN_STATUS_IND message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_sn_hfn_status_ind(&p_pdcp_msg, p_rrc_pdcp_sn_hfn_status_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_data_buffer_stop_ind_len
(
    rrc_pdcp_data_buffer_stop_ind_t *p_rrc_pdcp_data_buffer_stop_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_data_buffer_stop_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_data_buffer_stop_ind->ue_index);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_data_buffer_stop_ind
(
    U8  **pp_buffer,
    rrc_pdcp_data_buffer_stop_ind_t *p_rrc_pdcp_data_buffer_stop_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_data_buffer_stop_ind != PNULL);

    /* This function composes rrc_pdcp_data_buffer_stop_ind */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_data_buffer_stop_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_data_buffer_stop_ind->ue_index);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_data_buffer_stop_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_DATA_BUFFER_STOP_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_data_buffer_stop_ind
(
    rrc_pdcp_data_buffer_stop_ind_t  *p_rrc_pdcp_data_buffer_stop_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_data_buffer_stop_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_data_buffer_stop_ind_len(p_rrc_pdcp_data_buffer_stop_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_DATA_BUFFER_STOP_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_DATA_BUFFER_STOP_IND, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_DATA_BUFFER_STOP_IND, msg_length,cell_index);

    /* Fill RRC_PDCP_DATA_BUFFER_STOP_IND message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_data_buffer_stop_ind(&p_pdcp_msg, p_rrc_pdcp_data_buffer_stop_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_mac_i_req_len
(
    rrc_pdcp_mac_i_req_t *p_rrc_pdcp_mac_i_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_mac_i_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_mac_i_req->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_mac_i_req->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_mac_i_req->rb_direction);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_mac_i_req->num_message <= ARRSIZE(p_rrc_pdcp_mac_i_req->message));
        for (loop = 0; loop < p_rrc_pdcp_mac_i_req->num_message; loop++)
        {
            length += rrc_il_get_rrc_message_tlv_len(&p_rrc_pdcp_mac_i_req->message[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_mac_i_req
(
    U8  **pp_buffer,
    rrc_pdcp_mac_i_req_t *p_rrc_pdcp_mac_i_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_mac_i_req != PNULL);

    /* This function composes rrc_pdcp_mac_i_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_mac_i_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_mac_i_req->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_mac_i_req->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_mac_i_req->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_mac_i_req->rb_direction, "rb_direction");
    *pp_buffer += sizeof(p_rrc_pdcp_mac_i_req->rb_direction);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_mac_i_req->num_message <= ARRSIZE(p_rrc_pdcp_mac_i_req->message));
        for (loop = 0; loop < p_rrc_pdcp_mac_i_req->num_message; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_message(pp_buffer, &p_rrc_pdcp_mac_i_req->message[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_mac_i_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_MAC_I_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_mac_i_req
(
    rrc_pdcp_mac_i_req_t  *p_rrc_pdcp_mac_i_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_mac_i_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_mac_i_req_len(p_rrc_pdcp_mac_i_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_MAC_I_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_MAC_I_REQ, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_MAC_I_REQ, msg_length,cell_index);

    /* Fill RRC_PDCP_MAC_I_REQ message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_mac_i_req(&p_pdcp_msg, p_rrc_pdcp_mac_i_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_message_len
(
    rrc_message_t *p_rrc_message
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_message != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_message->cell_identity);

    /* Add buffer size */
    length += p_rrc_message->encoded_var_short_mac_i_data_length;

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_message_tlv_len
(
    rrc_message_t *p_rrc_message

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_message != PNULL);

    length += rrc_il_get_rrc_message_len(p_rrc_message);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_message
(
    U8  **pp_buffer,
    rrc_message_t *p_rrc_message
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_message != PNULL);

    /* This function composes rrc_message */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_MESSAGE_TAG;
    tlv_length = rrc_il_get_rrc_message_tlv_len(p_rrc_message);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_message->cell_identity); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_message->cell_identity[loop], "cell_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Compose OCTET_STRING LIMITED_TILL_THE_END */
    {
        if(ARRSIZE(p_rrc_message->encoded_var_short_mac_i) < p_rrc_message->encoded_var_short_mac_i_data_length)
        {
            RRC_TRACE(RRC_WARNING, "p_rrc_message->encoded_var_short_mac_i_data_length = %u is exceeding p_rrc_message->encoded_var_short_mac_i array boundary", p_rrc_message->encoded_var_short_mac_i_data_length);
            return RRC_FAILURE;
        }
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_message->encoded_var_short_mac_i_data_length; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_message->encoded_var_short_mac_i[loop], "encoded_var_short_mac_i[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_mac_i_resp_len
(
    rrc_pdcp_mac_i_resp_t *p_rrc_pdcp_mac_i_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_mac_i_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_mac_i_resp->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_mac_i_resp->response_code);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_mac_i_resp->num_var_short_mac_i <= ARRSIZE(p_rrc_pdcp_mac_i_resp->rrc_msg_authentication_code));
        for (loop = 0; loop < p_rrc_pdcp_mac_i_resp->num_var_short_mac_i; loop++)
        {
            length += rrc_il_get_rrc_msg_authentication_code_tlv_len(&p_rrc_pdcp_mac_i_resp->rrc_msg_authentication_code[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_mac_i_resp
(
    U8  **pp_buffer,
    rrc_pdcp_mac_i_resp_t *p_rrc_pdcp_mac_i_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_mac_i_resp != PNULL);

    /* This function composes rrc_pdcp_mac_i_resp */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_mac_i_resp->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_mac_i_resp->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_mac_i_resp->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_mac_i_resp->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_pdcp_mac_i_resp->response_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_mac_i_resp->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_pdcp_mac_i_resp->response_code);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_mac_i_resp->num_var_short_mac_i <= ARRSIZE(p_rrc_pdcp_mac_i_resp->rrc_msg_authentication_code));
        for (loop = 0; loop < p_rrc_pdcp_mac_i_resp->num_var_short_mac_i; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_msg_authentication_code(pp_buffer, &p_rrc_pdcp_mac_i_resp->rrc_msg_authentication_code[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_mac_i_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_MAC_I_RESP message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_mac_i_resp
(
    rrc_pdcp_mac_i_resp_t  *p_rrc_pdcp_mac_i_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_mac_i_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_mac_i_resp_len(p_rrc_pdcp_mac_i_resp);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_MAC_I_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_MAC_I_RESP, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_MAC_I_RESP, msg_length,cell_index);

    /* Fill RRC_PDCP_MAC_I_RESP message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_mac_i_resp(&p_pdcp_msg, p_rrc_pdcp_mac_i_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_msg_authentication_code_len
(
    rrc_msg_authentication_code_t *p_rrc_msg_authentication_code
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_msg_authentication_code != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rrc_msg_authentication_code->cell_identity);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_msg_authentication_code->response_code);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_msg_authentication_code->mac_i);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_msg_authentication_code_tlv_len
(
    rrc_msg_authentication_code_t *p_rrc_msg_authentication_code

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_msg_authentication_code != PNULL);

    length += rrc_il_get_rrc_msg_authentication_code_len(p_rrc_msg_authentication_code);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_msg_authentication_code
(
    U8  **pp_buffer,
    rrc_msg_authentication_code_t *p_rrc_msg_authentication_code
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_msg_authentication_code != PNULL);

    /* This function composes rrc_msg_authentication_code */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_MSG_AUTHENTICATION_CODE_TAG;
    tlv_length = rrc_il_get_rrc_msg_authentication_code_tlv_len(p_rrc_msg_authentication_code);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rrc_msg_authentication_code->cell_identity); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_msg_authentication_code->cell_identity[loop], "cell_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_msg_authentication_code->response_code > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_msg_authentication_code->response_code] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_msg_authentication_code->response_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_msg_authentication_code->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_msg_authentication_code->response_code);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_msg_authentication_code->mac_i, "mac_i");
    *pp_buffer += sizeof(p_rrc_msg_authentication_code->mac_i);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_re_establish_ue_entity_req_len
(
    rrc_pdcp_re_establish_ue_entity_req_t *p_rrc_pdcp_re_establish_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_re_establish_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_re_establish_ue_entity_req->ue_index);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_re_establish_ue_entity_req->num_re_est_rb_entity <= ARRSIZE(p_rrc_pdcp_re_establish_ue_entity_req->re_est_rb_entities));
        for (loop = 0; loop < p_rrc_pdcp_re_establish_ue_entity_req->num_re_est_rb_entity; loop++)
        {
            length += rrc_il_get_rrc_pdcp_rb_entity_tlv_len(&p_rrc_pdcp_re_establish_ue_entity_req->re_est_rb_entities[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_re_establish_ue_entity_req
(
    U8  **pp_buffer,
    rrc_pdcp_re_establish_ue_entity_req_t *p_rrc_pdcp_re_establish_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_re_establish_ue_entity_req != PNULL);

    /* This function composes rrc_pdcp_re_establish_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_re_establish_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_re_establish_ue_entity_req->ue_index);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_re_establish_ue_entity_req->num_re_est_rb_entity <= ARRSIZE(p_rrc_pdcp_re_establish_ue_entity_req->re_est_rb_entities));
        for (loop = 0; loop < p_rrc_pdcp_re_establish_ue_entity_req->num_re_est_rb_entity; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_rb_entity(pp_buffer, &p_rrc_pdcp_re_establish_ue_entity_req->re_est_rb_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_re_establish_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_RE_ESTABLISH_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_re_establish_ue_entity_req
(
    rrc_pdcp_re_establish_ue_entity_req_t  *p_rrc_pdcp_re_establish_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_re_establish_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_re_establish_ue_entity_req_len(p_rrc_pdcp_re_establish_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_RE_ESTABLISH_UE_ENTITY_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_RE_ESTABLISH_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_RE_ESTABLISH_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_PDCP_RE_ESTABLISH_UE_ENTITY_REQ message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_re_establish_ue_entity_req(&p_pdcp_msg, p_rrc_pdcp_re_establish_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_rb_entity_len
(
    rrc_pdcp_rb_entity_t *p_rrc_pdcp_rb_entity
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_rb_entity != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_rb_entity->lc_id);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_rb_entity_tlv_len
(
    rrc_pdcp_rb_entity_t *p_rrc_pdcp_rb_entity

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_rb_entity != PNULL);

    length += rrc_il_get_rrc_pdcp_rb_entity_len(p_rrc_pdcp_rb_entity);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_rb_entity
(
    U8  **pp_buffer,
    rrc_pdcp_rb_entity_t *p_rrc_pdcp_rb_entity
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_rb_entity != PNULL);

    /* This function composes rrc_pdcp_rb_entity */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_RB_ENTITY_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_rb_entity_tlv_len(p_rrc_pdcp_rb_entity);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_rb_entity->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_rb_entity->lc_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_re_establish_ue_entity_cnf_len
(
    rrc_pdcp_re_establish_ue_entity_cnf_t *p_rrc_pdcp_re_establish_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_re_establish_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_re_establish_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_re_establish_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_re_establish_ue_entity_cnf->num_re_est_rb_entity_error <= ARRSIZE(p_rrc_pdcp_re_establish_ue_entity_cnf->re_est_rb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_re_establish_ue_entity_cnf->num_re_est_rb_entity_error; loop++)
        {
            length += rrc_il_get_rrc_pdcp_rb_entity_error_tlv_len(&p_rrc_pdcp_re_establish_ue_entity_cnf->re_est_rb_error_entities[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_re_establish_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_pdcp_re_establish_ue_entity_cnf_t *p_rrc_pdcp_re_establish_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_re_establish_ue_entity_cnf != PNULL);

    /* This function composes rrc_pdcp_re_establish_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_re_establish_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_re_establish_ue_entity_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_re_establish_ue_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_re_establish_ue_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_pdcp_re_establish_ue_entity_cnf->response_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_re_establish_ue_entity_cnf->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_pdcp_re_establish_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_re_establish_ue_entity_cnf->num_re_est_rb_entity_error <= ARRSIZE(p_rrc_pdcp_re_establish_ue_entity_cnf->re_est_rb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_re_establish_ue_entity_cnf->num_re_est_rb_entity_error; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_rb_entity_error(pp_buffer, &p_rrc_pdcp_re_establish_ue_entity_cnf->re_est_rb_error_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_re_establish_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_RE_ESTABLISH_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_re_establish_ue_entity_cnf
(
    rrc_pdcp_re_establish_ue_entity_cnf_t  *p_rrc_pdcp_re_establish_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_re_establish_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_re_establish_ue_entity_cnf_len(p_rrc_pdcp_re_establish_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_RE_ESTABLISH_UE_ENTITY_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_RE_ESTABLISH_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_RE_ESTABLISH_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_PDCP_RE_ESTABLISH_UE_ENTITY_CNF message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_re_establish_ue_entity_cnf(&p_pdcp_msg, p_rrc_pdcp_re_establish_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_rb_entity_error_len
(
    rrc_pdcp_rb_entity_error_t *p_rrc_pdcp_rb_entity_error
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_rb_entity_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_rb_entity_error->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_rb_entity_error->response);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_rb_entity_error_tlv_len
(
    rrc_pdcp_rb_entity_error_t *p_rrc_pdcp_rb_entity_error

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_rb_entity_error != PNULL);

    length += rrc_il_get_rrc_pdcp_rb_entity_error_len(p_rrc_pdcp_rb_entity_error);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_rb_entity_error
(
    U8  **pp_buffer,
    rrc_pdcp_rb_entity_error_t *p_rrc_pdcp_rb_entity_error
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_rb_entity_error != PNULL);

    /* This function composes rrc_pdcp_rb_entity_error */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_RB_ENTITY_ERROR_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_rb_entity_error_tlv_len(p_rrc_pdcp_rb_entity_error);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_rb_entity_error->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_rb_entity_error->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_rb_entity_error->response, "response");
    *pp_buffer += sizeof(p_rrc_pdcp_rb_entity_error->response);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_resume_ue_entity_req_len
(
    rrc_pdcp_resume_ue_entity_req_t *p_rrc_pdcp_resume_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_resume_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_resume_ue_entity_req->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_resume_ue_entity_req->resume_direction);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_resume_ue_entity_req->num_resume_rb_entity <= ARRSIZE(p_rrc_pdcp_resume_ue_entity_req->resume_rb_entities));
        for (loop = 0; loop < p_rrc_pdcp_resume_ue_entity_req->num_resume_rb_entity; loop++)
        {
            length += rrc_il_get_rrc_pdcp_rb_entity_tlv_len(&p_rrc_pdcp_resume_ue_entity_req->resume_rb_entities[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_resume_ue_entity_req
(
    U8  **pp_buffer,
    rrc_pdcp_resume_ue_entity_req_t *p_rrc_pdcp_resume_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_resume_ue_entity_req != PNULL);

    /* This function composes rrc_pdcp_resume_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_resume_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_resume_ue_entity_req->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_resume_ue_entity_req->resume_direction, "resume_direction");
    *pp_buffer += sizeof(p_rrc_pdcp_resume_ue_entity_req->resume_direction);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_resume_ue_entity_req->num_resume_rb_entity <= ARRSIZE(p_rrc_pdcp_resume_ue_entity_req->resume_rb_entities));
        for (loop = 0; loop < p_rrc_pdcp_resume_ue_entity_req->num_resume_rb_entity; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_rb_entity(pp_buffer, &p_rrc_pdcp_resume_ue_entity_req->resume_rb_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_resume_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_RESUME_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_resume_ue_entity_req
(
    rrc_pdcp_resume_ue_entity_req_t  *p_rrc_pdcp_resume_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_resume_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_resume_ue_entity_req_len(p_rrc_pdcp_resume_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_RESUME_UE_ENTITY_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_RESUME_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_RESUME_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_PDCP_RESUME_UE_ENTITY_REQ message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_resume_ue_entity_req(&p_pdcp_msg, p_rrc_pdcp_resume_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_resume_ue_entity_cnf_len
(
    rrc_pdcp_resume_ue_entity_cnf_t *p_rrc_pdcp_resume_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_resume_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_resume_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_resume_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_resume_ue_entity_cnf->num_resume_rb_entity_error <= ARRSIZE(p_rrc_pdcp_resume_ue_entity_cnf->resume_rb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_resume_ue_entity_cnf->num_resume_rb_entity_error; loop++)
        {
            length += rrc_il_get_rrc_pdcp_rb_entity_error_tlv_len(&p_rrc_pdcp_resume_ue_entity_cnf->resume_rb_error_entities[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_resume_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_pdcp_resume_ue_entity_cnf_t *p_rrc_pdcp_resume_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_resume_ue_entity_cnf != PNULL);

    /* This function composes rrc_pdcp_resume_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_resume_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_resume_ue_entity_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_resume_ue_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_resume_ue_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_pdcp_resume_ue_entity_cnf->response_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_resume_ue_entity_cnf->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_pdcp_resume_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_resume_ue_entity_cnf->num_resume_rb_entity_error <= ARRSIZE(p_rrc_pdcp_resume_ue_entity_cnf->resume_rb_error_entities));
        for (loop = 0; loop < p_rrc_pdcp_resume_ue_entity_cnf->num_resume_rb_entity_error; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_rb_entity_error(pp_buffer, &p_rrc_pdcp_resume_ue_entity_cnf->resume_rb_error_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_resume_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_RESUME_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_resume_ue_entity_cnf
(
    rrc_pdcp_resume_ue_entity_cnf_t  *p_rrc_pdcp_resume_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_resume_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_resume_ue_entity_cnf_len(p_rrc_pdcp_resume_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_RESUME_UE_ENTITY_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_RESUME_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_RESUME_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_PDCP_RESUME_UE_ENTITY_CNF message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_resume_ue_entity_cnf(&p_pdcp_msg, p_rrc_pdcp_resume_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_change_crnti_req_len
(
    rrc_pdcp_change_crnti_req_t *p_rrc_pdcp_change_crnti_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_change_crnti_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_change_crnti_req->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_change_crnti_req->old_crnti);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_change_crnti_req->new_crnti);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_change_crnti_req
(
    U8  **pp_buffer,
    rrc_pdcp_change_crnti_req_t *p_rrc_pdcp_change_crnti_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_change_crnti_req != PNULL);

    /* This function composes rrc_pdcp_change_crnti_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_change_crnti_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_change_crnti_req->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_change_crnti_req->old_crnti, "old_crnti");
    *pp_buffer += sizeof(p_rrc_pdcp_change_crnti_req->old_crnti);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_change_crnti_req->new_crnti, "new_crnti");
    *pp_buffer += sizeof(p_rrc_pdcp_change_crnti_req->new_crnti);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_change_crnti_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_CHANGE_CRNTI_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_change_crnti_req
(
    rrc_pdcp_change_crnti_req_t  *p_rrc_pdcp_change_crnti_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_change_crnti_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_change_crnti_req_len(p_rrc_pdcp_change_crnti_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_CHANGE_CRNTI_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_CHANGE_CRNTI_REQ, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_CHANGE_CRNTI_REQ, msg_length,cell_index);

    /* Fill RRC_PDCP_CHANGE_CRNTI_REQ message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_change_crnti_req(&p_pdcp_msg, p_rrc_pdcp_change_crnti_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_change_crnti_cnf_len
(
    rrc_pdcp_change_crnti_cnf_t *p_rrc_pdcp_change_crnti_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_change_crnti_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_change_crnti_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_change_crnti_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_change_crnti_cnf
(
    U8  **pp_buffer,
    rrc_pdcp_change_crnti_cnf_t *p_rrc_pdcp_change_crnti_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_change_crnti_cnf != PNULL);

    /* This function composes rrc_pdcp_change_crnti_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_change_crnti_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_change_crnti_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_change_crnti_cnf->response > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_change_crnti_cnf->response] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_pdcp_change_crnti_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_change_crnti_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_pdcp_change_crnti_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_change_crnti_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_CHANGE_CRNTI_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_change_crnti_cnf
(
    rrc_pdcp_change_crnti_cnf_t  *p_rrc_pdcp_change_crnti_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_change_crnti_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_change_crnti_cnf_len(p_rrc_pdcp_change_crnti_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_CHANGE_CRNTI_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_CHANGE_CRNTI_CNF, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_CHANGE_CRNTI_CNF, msg_length,cell_index);

    /* Fill RRC_PDCP_CHANGE_CRNTI_CNF message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_change_crnti_cnf(&p_pdcp_msg, p_rrc_pdcp_change_crnti_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_count_wraparound_ind_len
(
    rrc_pdcp_count_wraparound_ind_t *p_rrc_pdcp_count_wraparound_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_count_wraparound_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_count_wraparound_ind->ue_index);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_count_wraparound_ind
(
    U8  **pp_buffer,
    rrc_pdcp_count_wraparound_ind_t *p_rrc_pdcp_count_wraparound_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_count_wraparound_ind != PNULL);

    /* This function composes rrc_pdcp_count_wraparound_ind */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_count_wraparound_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_count_wraparound_ind->ue_index);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_count_wraparound_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_COUNT_WRAPAROUND_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_count_wraparound_ind
(
    rrc_pdcp_count_wraparound_ind_t  *p_rrc_pdcp_count_wraparound_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_count_wraparound_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_count_wraparound_ind_len(p_rrc_pdcp_count_wraparound_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_COUNT_WRAPAROUND_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_COUNT_WRAPAROUND_IND, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_COUNT_WRAPAROUND_IND, msg_length,cell_index);

    /* Fill RRC_PDCP_COUNT_WRAPAROUND_IND message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_count_wraparound_ind(&p_pdcp_msg, p_rrc_pdcp_count_wraparound_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_notify_integrity_failure_len
(
    rrc_pdcp_notify_integrity_failure_t *p_rrc_pdcp_notify_integrity_failure
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_notify_integrity_failure != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_notify_integrity_failure->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_notify_integrity_failure->lc_id);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_notify_integrity_failure
(
    U8  **pp_buffer,
    rrc_pdcp_notify_integrity_failure_t *p_rrc_pdcp_notify_integrity_failure
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_notify_integrity_failure != PNULL);

    /* This function composes rrc_pdcp_notify_integrity_failure */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_notify_integrity_failure->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_notify_integrity_failure->ue_index);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_pdcp_notify_integrity_failure->lc_id < 1) || (p_rrc_pdcp_notify_integrity_failure->lc_id > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_notify_integrity_failure->lc_id] should be in range "
            "1 to 2. Incorrect value %u received.", p_rrc_pdcp_notify_integrity_failure->lc_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_notify_integrity_failure->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_notify_integrity_failure->lc_id);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_notify_integrity_failure
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_NOTIFY_INTEGRITY_FAILURE message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_notify_integrity_failure
(
    rrc_pdcp_notify_integrity_failure_t  *p_rrc_pdcp_notify_integrity_failure,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_notify_integrity_failure != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_notify_integrity_failure_len(p_rrc_pdcp_notify_integrity_failure);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_NOTIFY_INTEGRITY_FAILURE", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_NOTIFY_INTEGRITY_FAILURE, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_NOTIFY_INTEGRITY_FAILURE, msg_length,cell_index);

    /* Fill RRC_PDCP_NOTIFY_INTEGRITY_FAILURE message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_notify_integrity_failure(&p_pdcp_msg, p_rrc_pdcp_notify_integrity_failure))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_drb_count_msb_req_len
(
    rrc_pdcp_drb_count_msb_req_t *p_rrc_pdcp_drb_count_msb_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_drb_count_msb_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_drb_count_msb_req->ue_index);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_drb_count_msb_req->num_drb <= ARRSIZE(p_rrc_pdcp_drb_count_msb_req->drb_list));
        for (loop = 0; loop < p_rrc_pdcp_drb_count_msb_req->num_drb; loop++)
        {
            length += rrc_il_get_rrc_pdcp_drb_list_tlv_len(&p_rrc_pdcp_drb_count_msb_req->drb_list[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_drb_count_msb_req
(
    U8  **pp_buffer,
    rrc_pdcp_drb_count_msb_req_t *p_rrc_pdcp_drb_count_msb_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_drb_count_msb_req != PNULL);

    /* This function composes rrc_pdcp_drb_count_msb_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_drb_count_msb_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_drb_count_msb_req->ue_index);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_drb_count_msb_req->num_drb <= ARRSIZE(p_rrc_pdcp_drb_count_msb_req->drb_list));
        for (loop = 0; loop < p_rrc_pdcp_drb_count_msb_req->num_drb; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_drb_list(pp_buffer, &p_rrc_pdcp_drb_count_msb_req->drb_list[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_drb_count_msb_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_DRB_COUNT_MSB_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_drb_count_msb_req
(
    rrc_pdcp_drb_count_msb_req_t  *p_rrc_pdcp_drb_count_msb_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_drb_count_msb_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_drb_count_msb_req_len(p_rrc_pdcp_drb_count_msb_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_DRB_COUNT_MSB_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_DRB_COUNT_MSB_REQ, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_DRB_COUNT_MSB_REQ, msg_length,cell_index);

    /* Fill RRC_PDCP_DRB_COUNT_MSB_REQ message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_drb_count_msb_req(&p_pdcp_msg, p_rrc_pdcp_drb_count_msb_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_drb_list_len
(
    rrc_pdcp_drb_list_t *p_rrc_pdcp_drb_list
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_drb_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_drb_list->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_drb_list->rb_direction);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_drb_list_tlv_len
(
    rrc_pdcp_drb_list_t *p_rrc_pdcp_drb_list

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_drb_list != PNULL);

    length += rrc_il_get_rrc_pdcp_drb_list_len(p_rrc_pdcp_drb_list);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_drb_list
(
    U8  **pp_buffer,
    rrc_pdcp_drb_list_t *p_rrc_pdcp_drb_list
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_drb_list != PNULL);

    /* This function composes rrc_pdcp_drb_list */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_DRB_COUNTMSB_DRB_LIST_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_drb_list_tlv_len(p_rrc_pdcp_drb_list);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_drb_list->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_drb_list->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_drb_list->rb_direction, "rb_direction");
    *pp_buffer += sizeof(p_rrc_pdcp_drb_list->rb_direction);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_drb_count_msb_resp_len
(
    rrc_pdcp_drb_count_msb_resp_t *p_rrc_pdcp_drb_count_msb_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_drb_count_msb_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_drb_count_msb_resp->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_drb_count_msb_resp->response);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_drb_count_msb_resp->num_drb <= ARRSIZE(p_rrc_pdcp_drb_count_msb_resp->drb_countMSB_info));
        for (loop = 0; loop < p_rrc_pdcp_drb_count_msb_resp->num_drb; loop++)
        {
            length += rrc_il_get_rrc_pdcp_drb_countMSB_info_tlv_len(&p_rrc_pdcp_drb_count_msb_resp->drb_countMSB_info[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_drb_count_msb_resp
(
    U8  **pp_buffer,
    rrc_pdcp_drb_count_msb_resp_t *p_rrc_pdcp_drb_count_msb_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_drb_count_msb_resp != PNULL);

    /* This function composes rrc_pdcp_drb_count_msb_resp */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_drb_count_msb_resp->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_drb_count_msb_resp->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_drb_count_msb_resp->response > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_drb_count_msb_resp->response] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_pdcp_drb_count_msb_resp->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_drb_count_msb_resp->response, "response");
    *pp_buffer += sizeof(p_rrc_pdcp_drb_count_msb_resp->response);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_drb_count_msb_resp->num_drb <= ARRSIZE(p_rrc_pdcp_drb_count_msb_resp->drb_countMSB_info));
        for (loop = 0; loop < p_rrc_pdcp_drb_count_msb_resp->num_drb; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_drb_countMSB_info(pp_buffer, &p_rrc_pdcp_drb_count_msb_resp->drb_countMSB_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_drb_count_msb_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_DRB_COUNT_MSB_RESP message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_drb_count_msb_resp
(
    rrc_pdcp_drb_count_msb_resp_t  *p_rrc_pdcp_drb_count_msb_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_drb_count_msb_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_drb_count_msb_resp_len(p_rrc_pdcp_drb_count_msb_resp);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_DRB_COUNT_MSB_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_DRB_COUNT_MSB_RESP, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_DRB_COUNT_MSB_RESP, msg_length,cell_index);

    /* Fill RRC_PDCP_DRB_COUNT_MSB_RESP message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_drb_count_msb_resp(&p_pdcp_msg, p_rrc_pdcp_drb_count_msb_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_countMSB_uplink_len
(
    countMSB_uplink_t *p_countMSB_uplink
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_countMSB_uplink != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_countMSB_uplink->countMSB_Uplink);

    return length;
}

static
rrc_length_t
rrc_il_get_countMSB_uplink_tlv_len
(
    countMSB_uplink_t *p_countMSB_uplink

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_countMSB_uplink != PNULL);

    length += rrc_il_get_countMSB_uplink_len(p_countMSB_uplink);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_countMSB_uplink
(
    U8  **pp_buffer,
    countMSB_uplink_t *p_countMSB_uplink
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_countMSB_uplink != PNULL);

    /* This function composes countMSB_uplink */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_COUNTMSB_UPLINK_TAG;
    tlv_length = rrc_il_get_countMSB_uplink_tlv_len(p_countMSB_uplink);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_countMSB_uplink->countMSB_Uplink > 33554431))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_countMSB_uplink->countMSB_Uplink] should be less than"
            " or equal to 33554431. Incorrect value %u received.", p_countMSB_uplink->countMSB_Uplink);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_countMSB_uplink->countMSB_Uplink, "countMSB_Uplink");
    *pp_buffer += sizeof(p_countMSB_uplink->countMSB_Uplink);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_countMSB_downlink_len
(
    countMSB_downlink_t *p_countMSB_downlink
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_countMSB_downlink != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_countMSB_downlink->countMSB_Downlink);

    return length;
}

static
rrc_length_t
rrc_il_get_countMSB_downlink_tlv_len
(
    countMSB_downlink_t *p_countMSB_downlink

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_countMSB_downlink != PNULL);

    length += rrc_il_get_countMSB_downlink_len(p_countMSB_downlink);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_countMSB_downlink
(
    U8  **pp_buffer,
    countMSB_downlink_t *p_countMSB_downlink
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_countMSB_downlink != PNULL);

    /* This function composes countMSB_downlink */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_COUNTMSB_DOWNLINK_TAG;
    tlv_length = rrc_il_get_countMSB_downlink_tlv_len(p_countMSB_downlink);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_countMSB_downlink->countMSB_Downlink > 33554431))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_countMSB_downlink->countMSB_Downlink] should be less than"
            " or equal to 33554431. Incorrect value %u received.", p_countMSB_downlink->countMSB_Downlink);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_countMSB_downlink->countMSB_Downlink, "countMSB_Downlink");
    *pp_buffer += sizeof(p_countMSB_downlink->countMSB_Downlink);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_drb_countMSB_info_len
(
    rrc_pdcp_drb_countMSB_info_t *p_rrc_pdcp_drb_countMSB_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_drb_countMSB_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_drb_countMSB_info->lc_id);

    /* Get length of TLV */
    if (p_rrc_pdcp_drb_countMSB_info->optional_elems_present & RRC_PDCP_COUNTMSB_UPLINK_PRESENT)
    {
        length += rrc_il_get_countMSB_uplink_tlv_len(&p_rrc_pdcp_drb_countMSB_info->countMSB_Uplink);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_drb_countMSB_info->optional_elems_present & RRC_PDCP_COUNTMSB_DOWNLINK_PRESENT)
    {
        length += rrc_il_get_countMSB_downlink_tlv_len(&p_rrc_pdcp_drb_countMSB_info->countMSB_Downlink);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_drb_countMSB_info_tlv_len
(
    rrc_pdcp_drb_countMSB_info_t *p_rrc_pdcp_drb_countMSB_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_drb_countMSB_info != PNULL);

    length += rrc_il_get_rrc_pdcp_drb_countMSB_info_len(p_rrc_pdcp_drb_countMSB_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_drb_countMSB_info
(
    U8  **pp_buffer,
    rrc_pdcp_drb_countMSB_info_t *p_rrc_pdcp_drb_countMSB_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_drb_countMSB_info != PNULL);

    /* This function composes rrc_pdcp_drb_countMSB_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_DRB_COUNTMSB_INFO_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_drb_countMSB_info_tlv_len(p_rrc_pdcp_drb_countMSB_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_drb_countMSB_info->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_pdcp_drb_countMSB_info->lc_id);

    /* Compose TLV  */
    if (p_rrc_pdcp_drb_countMSB_info->optional_elems_present & RRC_PDCP_COUNTMSB_UPLINK_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_countMSB_uplink(pp_buffer, &p_rrc_pdcp_drb_countMSB_info->countMSB_Uplink))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_drb_countMSB_info->optional_elems_present & RRC_PDCP_COUNTMSB_DOWNLINK_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_countMSB_downlink(pp_buffer, &p_rrc_pdcp_drb_countMSB_info->countMSB_Downlink))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_config_cell_req_len
(
    rrc_pdcp_config_cell_req_t *p_rrc_pdcp_config_cell_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_cell_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_config_cell_req->Cell_index);

    /* Get length of IE */
    length += rrc_il_get_pdcp_communication_info_len(&p_rrc_pdcp_config_cell_req->pdcp_communication_info);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_config_cell_req
(
    U8  **pp_buffer,
    rrc_pdcp_config_cell_req_t *p_rrc_pdcp_config_cell_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_config_cell_req != PNULL);

    /* This function composes rrc_pdcp_config_cell_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_config_cell_req->Cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_config_cell_req->Cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_pdcp_config_cell_req->Cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_config_cell_req->Cell_index, "Cell_index");
    *pp_buffer += sizeof(p_rrc_pdcp_config_cell_req->Cell_index);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_pdcp_communication_info(pp_buffer, &p_rrc_pdcp_config_cell_req->pdcp_communication_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_config_cell_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_CELL_CONFIG_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_config_cell_req
(
    rrc_pdcp_config_cell_req_t  *p_rrc_pdcp_config_cell_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_config_cell_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_config_cell_req_len(p_rrc_pdcp_config_cell_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_CELL_CONFIG_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_CELL_CONFIG_REQ, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_CELL_CONFIG_REQ, msg_length,cell_index);

    /* Fill RRC_PDCP_CELL_CONFIG_REQ message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_config_cell_req(&p_pdcp_msg, p_rrc_pdcp_config_cell_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_pdcp_communication_info_len
(
    pdcp_communication_info_t *p_pdcp_communication_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_pdcp_communication_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_pdcp_communication_info->rlcInstance);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_pdcp_communication_info->rlcRxPort);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_pdcp_communication_info->rlcIpAddr);

    return length;
}

static
rrc_return_et
rrc_il_compose_pdcp_communication_info
(
    U8  **pp_buffer,
    pdcp_communication_info_t *p_pdcp_communication_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_pdcp_communication_info != PNULL);

    /* This function composes pdcp_communication_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_pdcp_communication_info->rlcInstance, "rlcInstance");
    *pp_buffer += sizeof(p_pdcp_communication_info->rlcInstance);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_pdcp_communication_info->rlcRxPort, "rlcRxPort");
    *pp_buffer += sizeof(p_pdcp_communication_info->rlcRxPort);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_pdcp_communication_info->rlcIpAddr); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_pdcp_communication_info->rlcIpAddr[loop], "rlcIpAddr[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_config_cell_cnf_len
(
    rrc_pdcp_config_cell_cnf_t *p_rrc_pdcp_config_cell_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_config_cell_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_config_cell_cnf->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_config_cell_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_config_cell_cnf
(
    U8  **pp_buffer,
    rrc_pdcp_config_cell_cnf_t *p_rrc_pdcp_config_cell_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_config_cell_cnf != PNULL);

    /* This function composes rrc_pdcp_config_cell_cnf */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_config_cell_cnf->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_config_cell_cnf->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_pdcp_config_cell_cnf->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_config_cell_cnf->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_pdcp_config_cell_cnf->cell_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_config_cell_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_config_cell_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_pdcp_config_cell_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_config_cell_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_pdcp_config_cell_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_config_cell_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_CELL_CONFIG_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_config_cell_cnf
(
    rrc_pdcp_config_cell_cnf_t  *p_rrc_pdcp_config_cell_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_config_cell_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_config_cell_cnf_len(p_rrc_pdcp_config_cell_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_CELL_CONFIG_CNF", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_CELL_CONFIG_CNF, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_CELL_CONFIG_CNF, msg_length,cell_index);

    /* Fill RRC_PDCP_CELL_CONFIG_CNF message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_config_cell_cnf(&p_pdcp_msg, p_rrc_pdcp_config_cell_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_ho_prep_info_req_len
(
    rrc_pdcp_ho_prep_info_req_t *p_rrc_pdcp_ho_prep_info_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_ho_prep_info_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_ho_prep_info_req->ue_index);

    /* Get length of TLV */
    if (p_rrc_pdcp_ho_prep_info_req->optional_elems_present & RRC_PDCP_MACI_MSG_REQ_PRESENT)
    {
        length += rrc_il_get_rrc_mac_i_msg_req_tlv_len(&p_rrc_pdcp_ho_prep_info_req->mac_i_msg_req);
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_ho_prep_info_req->optional_elems_present & RRC_PDCP_HO_PREP_UE_INACTIVE_TIME_REQ_PRESENT)
    {
        length += rrc_il_get_rrc_ue_inactivity_time_val_tlv_len(&p_rrc_pdcp_ho_prep_info_req->ue_inactive_time_val);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_ho_prep_info_req
(
    U8  **pp_buffer,
    rrc_pdcp_ho_prep_info_req_t *p_rrc_pdcp_ho_prep_info_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_ho_prep_info_req != PNULL);

    /* This function composes rrc_pdcp_ho_prep_info_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_ho_prep_info_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_ho_prep_info_req->ue_index);

    /* Compose TLV  */
    if (p_rrc_pdcp_ho_prep_info_req->optional_elems_present & RRC_PDCP_MACI_MSG_REQ_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_mac_i_msg_req(pp_buffer, &p_rrc_pdcp_ho_prep_info_req->mac_i_msg_req))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_ho_prep_info_req->optional_elems_present & RRC_PDCP_HO_PREP_UE_INACTIVE_TIME_REQ_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_ue_inactivity_time_val(pp_buffer, &p_rrc_pdcp_ho_prep_info_req->ue_inactive_time_val))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_ho_prep_info_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_HO_PREP_INFO_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_ho_prep_info_req
(
    rrc_pdcp_ho_prep_info_req_t  *p_rrc_pdcp_ho_prep_info_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_ho_prep_info_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_ho_prep_info_req_len(p_rrc_pdcp_ho_prep_info_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_HO_PREP_INFO_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_HO_PREP_INFO_REQ, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_HO_PREP_INFO_REQ, msg_length,cell_index);

    /* Fill RRC_PDCP_HO_PREP_INFO_REQ message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_ho_prep_info_req(&p_pdcp_msg, p_rrc_pdcp_ho_prep_info_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_mac_i_msg_req_len
(
    rrc_mac_i_msg_req_t *p_rrc_mac_i_msg_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_i_msg_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_i_msg_req->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_mac_i_msg_req->rb_direction);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_i_msg_req->num_message <= ARRSIZE(p_rrc_mac_i_msg_req->message));
        for (loop = 0; loop < p_rrc_mac_i_msg_req->num_message; loop++)
        {
            length += rrc_il_get_rrc_message_tlv_len(&p_rrc_mac_i_msg_req->message[loop]);
        }
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_mac_i_msg_req_tlv_len
(
    rrc_mac_i_msg_req_t *p_rrc_mac_i_msg_req

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_mac_i_msg_req != PNULL);

    length += rrc_il_get_rrc_mac_i_msg_req_len(p_rrc_mac_i_msg_req);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_mac_i_msg_req
(
    U8  **pp_buffer,
    rrc_mac_i_msg_req_t *p_rrc_mac_i_msg_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_mac_i_msg_req != PNULL);

    /* This function composes rrc_mac_i_msg_req */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_MACI_HO_REQ_TAG;
    tlv_length = rrc_il_get_rrc_mac_i_msg_req_tlv_len(p_rrc_mac_i_msg_req);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_mac_i_msg_req->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_mac_i_msg_req->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_mac_i_msg_req->rb_direction, "rb_direction");
    *pp_buffer += sizeof(p_rrc_mac_i_msg_req->rb_direction);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_mac_i_msg_req->num_message <= ARRSIZE(p_rrc_mac_i_msg_req->message));
        for (loop = 0; loop < p_rrc_mac_i_msg_req->num_message; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_message(pp_buffer, &p_rrc_mac_i_msg_req->message[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_ue_inactivity_time_val_len
(
    rrc_ue_inactivity_time_val_t *p_rrc_ue_inactivity_time_val
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_ue_inactivity_time_val != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_ue_inactivity_time_val->ue_inactive_time_val_flag);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_ue_inactivity_time_val_tlv_len
(
    rrc_ue_inactivity_time_val_t *p_rrc_ue_inactivity_time_val

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_ue_inactivity_time_val != PNULL);

    length += rrc_il_get_rrc_ue_inactivity_time_val_len(p_rrc_ue_inactivity_time_val);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_ue_inactivity_time_val
(
    U8  **pp_buffer,
    rrc_ue_inactivity_time_val_t *p_rrc_ue_inactivity_time_val
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_ue_inactivity_time_val != PNULL);

    /* This function composes rrc_ue_inactivity_time_val */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_UE_INACTIVE_TIME_REQ_TAG;
    tlv_length = rrc_il_get_rrc_ue_inactivity_time_val_tlv_len(p_rrc_ue_inactivity_time_val);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_ue_inactivity_time_val->ue_inactive_time_val_flag > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_ue_inactivity_time_val->ue_inactive_time_val_flag] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_ue_inactivity_time_val->ue_inactive_time_val_flag);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_ue_inactivity_time_val->ue_inactive_time_val_flag, "ue_inactive_time_val_flag");
    *pp_buffer += sizeof(p_rrc_ue_inactivity_time_val->ue_inactive_time_val_flag);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_ho_prep_info_resp_len
(
    rrc_pdcp_ho_prep_info_resp_t *p_rrc_pdcp_ho_prep_info_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_ho_prep_info_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_ho_prep_info_resp->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_ho_prep_info_resp->response_code);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_ho_prep_info_resp->num_var_short_mac_i <= ARRSIZE(p_rrc_pdcp_ho_prep_info_resp->rrc_msg_authentication_code));
        for (loop = 0; loop < p_rrc_pdcp_ho_prep_info_resp->num_var_short_mac_i; loop++)
        {
            length += rrc_il_get_rrc_msg_authentication_code_tlv_len(&p_rrc_pdcp_ho_prep_info_resp->rrc_msg_authentication_code[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_pdcp_ho_prep_info_resp->optional_elems_present & RRC_PDCP_HO_PREP_UE_INACTIVE_TIME_RESP_PRESENT)
    {
        length += rrc_il_get_rrc_ue_inactive_time_tlv_len(&p_rrc_pdcp_ho_prep_info_resp->ue_inactivity_time_resp);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_ho_prep_info_resp
(
    U8  **pp_buffer,
    rrc_pdcp_ho_prep_info_resp_t *p_rrc_pdcp_ho_prep_info_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_ho_prep_info_resp != PNULL);

    /* This function composes rrc_pdcp_ho_prep_info_resp */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_ho_prep_info_resp->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_ho_prep_info_resp->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_pdcp_ho_prep_info_resp->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_ho_prep_info_resp->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_pdcp_ho_prep_info_resp->response_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_ho_prep_info_resp->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_pdcp_ho_prep_info_resp->response_code);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_ho_prep_info_resp->num_var_short_mac_i <= ARRSIZE(p_rrc_pdcp_ho_prep_info_resp->rrc_msg_authentication_code));
        for (loop = 0; loop < p_rrc_pdcp_ho_prep_info_resp->num_var_short_mac_i; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_msg_authentication_code(pp_buffer, &p_rrc_pdcp_ho_prep_info_resp->rrc_msg_authentication_code[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_pdcp_ho_prep_info_resp->optional_elems_present & RRC_PDCP_HO_PREP_UE_INACTIVE_TIME_RESP_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_ue_inactive_time(pp_buffer, &p_rrc_pdcp_ho_prep_info_resp->ue_inactivity_time_resp))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_ho_prep_info_resp
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_HO_PREP_INFO_RESP message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_ho_prep_info_resp
(
    rrc_pdcp_ho_prep_info_resp_t  *p_rrc_pdcp_ho_prep_info_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_ho_prep_info_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_ho_prep_info_resp_len(p_rrc_pdcp_ho_prep_info_resp);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_HO_PREP_INFO_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_HO_PREP_INFO_RESP, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_HO_PREP_INFO_RESP, msg_length,cell_index);

    /* Fill RRC_PDCP_HO_PREP_INFO_RESP message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_ho_prep_info_resp(&p_pdcp_msg, p_rrc_pdcp_ho_prep_info_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_pdcp_inactive_ues_ind_len
(
    rrc_pdcp_inactive_ues_ind_t *p_rrc_pdcp_inactive_ues_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_inactive_ues_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue_counter <= ARRSIZE(p_rrc_pdcp_inactive_ues_ind->inactive_ue_info));
        for (loop = 0; loop < p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue_counter; loop++)
        {
            length += rrc_il_get_rrc_pdcp_inactive_ue_info_tlv_len(&p_rrc_pdcp_inactive_ues_ind->inactive_ue_info[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_pdcp_inactive_ues_ind
(
    U8  **pp_buffer,
    rrc_pdcp_inactive_ues_ind_t *p_rrc_pdcp_inactive_ues_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_inactive_ues_ind != PNULL);

    /* This function composes rrc_pdcp_inactive_ues_ind */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue < 1) || (p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue] should be in range "
            "1 to 10. Incorrect value %u received.", p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue, "num_of_inactive_ue");
    *pp_buffer += sizeof(p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue_counter <= ARRSIZE(p_rrc_pdcp_inactive_ues_ind->inactive_ue_info));
        for (loop = 0; loop < p_rrc_pdcp_inactive_ues_ind->num_of_inactive_ue_counter; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_inactive_ue_info(pp_buffer, &p_rrc_pdcp_inactive_ues_ind->inactive_ue_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_pdcp_il_send_rrc_pdcp_inactive_ues_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_PDCP_INACTIVE_UES_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_pdcp_il_send_rrc_pdcp_inactive_ues_ind
(
    rrc_pdcp_inactive_ues_ind_t  *p_rrc_pdcp_inactive_ues_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_pdcp_msg;

    RRC_ASSERT(p_rrc_pdcp_inactive_ues_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_pdcp_inactive_ues_ind_len(p_rrc_pdcp_inactive_ues_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_PDCP_INACTIVE_UES_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_pdcp_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_PDCP_INACTIVE_UES_IND, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_pdcp_msg, transaction_id, src_module_id,
        RRC_PDCP_MODULE_ID, RRC_PDCP_INACTIVE_UES_IND, msg_length,cell_index);

    /* Fill RRC_PDCP_INACTIVE_UES_IND message */
    p_pdcp_msg = p_pdcp_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_pdcp_inactive_ues_ind(&p_pdcp_msg, p_rrc_pdcp_inactive_ues_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_pdcp_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_inactive_ue_info_len
(
    rrc_pdcp_inactive_ue_info_t *p_rrc_pdcp_inactive_ue_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_inactive_ue_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_pdcp_inactive_ue_info->ue_index);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_pdcp_inactive_ue_info_tlv_len
(
    rrc_pdcp_inactive_ue_info_t *p_rrc_pdcp_inactive_ue_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_pdcp_inactive_ue_info != PNULL);

    length += rrc_il_get_rrc_pdcp_inactive_ue_info_len(p_rrc_pdcp_inactive_ue_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_pdcp_inactive_ue_info
(
    U8  **pp_buffer,
    rrc_pdcp_inactive_ue_info_t *p_rrc_pdcp_inactive_ue_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_pdcp_inactive_ue_info != PNULL);

    /* This function composes rrc_pdcp_inactive_ue_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_PDCP_INACTIVE_UE_INFO_TAG;
    tlv_length = rrc_il_get_rrc_pdcp_inactive_ue_info_tlv_len(p_rrc_pdcp_inactive_ue_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_pdcp_inactive_ue_info->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_pdcp_inactive_ue_info->ue_index);

    return RRC_SUCCESS;
}

