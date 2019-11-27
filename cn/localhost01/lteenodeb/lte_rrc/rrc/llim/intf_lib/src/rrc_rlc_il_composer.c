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
 *  File Description : The file rrc_rlc_il_composer.c contains the definitions 
 *                     of rrc-rlc interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/

#include "rrc_cp_common.h"
#include "rrc_rlc_il_composer.h"
#include "rrc_rlc_intf.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"
#include "rrc_msg_mgmt.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_length_t
rrc_il_get_rrc_rlc_qci_info_len
(
    rrc_rlc_qci_info_t *p_rrc_rlc_qci_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_qci_info_tlv_len
(
    rrc_rlc_qci_info_t *p_rrc_rlc_qci_info

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_qci_info
(
    U8  **pp_buffer,
    rrc_rlc_qci_info_t *p_rrc_rlc_qci_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_tx_bit_rate_info_len
(
    rrc_rlc_tx_bit_rate_info_t *p_rrc_rlc_tx_bit_rate_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_tx_bit_rate_info_tlv_len
(
    rrc_rlc_tx_bit_rate_info_t *p_rrc_rlc_tx_bit_rate_info

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_tx_bit_rate_info
(
    U8  **pp_buffer,
    rrc_rlc_tx_bit_rate_info_t *p_rrc_rlc_tx_bit_rate_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_queue_size_info_len
(
    rrc_rlc_queue_size_info_t *p_rrc_rlc_queue_size_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_queue_size_info_tlv_len
(
    rrc_rlc_queue_size_info_t *p_rrc_rlc_queue_size_info

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_queue_size_info
(
    U8  **pp_buffer,
    rrc_rlc_queue_size_info_t *p_rrc_rlc_queue_size_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_dc_bearer_type_info_len
(
    rrc_rlc_dc_bearer_type_info *p_rrc_rlc_dc_bearer_type_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_dc_bearer_type_info_tlv_len
(
    rrc_rlc_dc_bearer_type_info *p_rrc_rlc_dc_bearer_type_info

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_dc_bearer_type_info
(
    U8  **pp_buffer,
    rrc_rlc_dc_bearer_type_info *p_rrc_rlc_dc_bearer_type_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_create_tx_um_rlc_len
(
    rrc_rlc_create_tx_um_rlc_t *p_rrc_rlc_create_tx_um_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_create_tx_um_rlc_tlv_len
(
    rrc_rlc_create_tx_um_rlc_t *p_rrc_rlc_create_tx_um_rlc

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_create_tx_um_rlc
(
    U8  **pp_buffer,
    rrc_rlc_create_tx_um_rlc_t *p_rrc_rlc_create_tx_um_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_rx_bit_rate_info_len
(
    rrc_rlc_rx_bit_rate_info_t *p_rrc_rlc_rx_bit_rate_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_rx_bit_rate_info_tlv_len
(
    rrc_rlc_rx_bit_rate_info_t *p_rrc_rlc_rx_bit_rate_info

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_rx_bit_rate_info
(
    U8  **pp_buffer,
    rrc_rlc_rx_bit_rate_info_t *p_rrc_rlc_rx_bit_rate_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_create_rx_um_rlc_len
(
    rrc_rlc_create_rx_um_rlc_t *p_rrc_rlc_create_rx_um_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_create_rx_um_rlc_tlv_len
(
    rrc_rlc_create_rx_um_rlc_t *p_rrc_rlc_create_rx_um_rlc

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_create_rx_um_rlc
(
    U8  **pp_buffer,
    rrc_rlc_create_rx_um_rlc_t *p_rrc_rlc_create_rx_um_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_create_tx_rx_um_rlc_len
(
    rrc_rlc_create_tx_rx_um_rlc_t *p_rrc_rlc_create_tx_rx_um_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_create_tx_rx_um_rlc_tlv_len
(
    rrc_rlc_create_tx_rx_um_rlc_t *p_rrc_rlc_create_tx_rx_um_rlc

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_create_tx_rx_um_rlc
(
    U8  **pp_buffer,
    rrc_rlc_create_tx_rx_um_rlc_t *p_rrc_rlc_create_tx_rx_um_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_create_tx_rx_am_rlc_len
(
    rrc_rlc_create_tx_rx_am_rlc_t *p_rrc_rlc_create_tx_rx_am_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_create_tx_rx_am_rlc_tlv_len
(
    rrc_rlc_create_tx_rx_am_rlc_t *p_rrc_rlc_create_tx_rx_am_rlc

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_create_tx_rx_am_rlc
(
    U8  **pp_buffer,
    rrc_rlc_create_tx_rx_am_rlc_t *p_rrc_rlc_create_tx_rx_am_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_ue_bit_rate_info_len
(
    rrc_rlc_ue_bit_rate_info_t *p_rrc_rlc_ue_bit_rate_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_ue_bit_rate_info_tlv_len
(
    rrc_rlc_ue_bit_rate_info_t *p_rrc_rlc_ue_bit_rate_info

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_ue_bit_rate_info
(
    U8  **pp_buffer,
    rrc_rlc_ue_bit_rate_info_t *p_rrc_rlc_ue_bit_rate_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_dl_setup_info_len
(
    rrc_rlc_sps_dl_setup_info_t *p_rrc_rlc_sps_dl_setup_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_dl_setup_info_tlv_len
(
    rrc_rlc_sps_dl_setup_info_t *p_rrc_rlc_sps_dl_setup_info

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_sps_dl_setup_info
(
    U8  **pp_buffer,
    rrc_rlc_sps_dl_setup_info_t *p_rrc_rlc_sps_dl_setup_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_dl_info_len
(
    rrc_rlc_sps_dl_info_t *p_rrc_rlc_sps_dl_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_dl_info_tlv_len
(
    rrc_rlc_sps_dl_info_t *p_rrc_rlc_sps_dl_info

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_sps_dl_info
(
    U8  **pp_buffer,
    rrc_rlc_sps_dl_info_t *p_rrc_rlc_sps_dl_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_ul_setup_info_len
(
    rrc_rlc_sps_ul_setup_info_t *p_rrc_rlc_sps_ul_setup_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_ul_setup_info_tlv_len
(
    rrc_rlc_sps_ul_setup_info_t *p_rrc_rlc_sps_ul_setup_info

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_sps_ul_setup_info
(
    U8  **pp_buffer,
    rrc_rlc_sps_ul_setup_info_t *p_rrc_rlc_sps_ul_setup_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_ul_info_len
(
    rrc_rlc_sps_ul_info_t *p_rrc_rlc_sps_ul_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_ul_info_tlv_len
(
    rrc_rlc_sps_ul_info_t *p_rrc_rlc_sps_ul_info

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_sps_ul_info
(
    U8  **pp_buffer,
    rrc_rlc_sps_ul_info_t *p_rrc_rlc_sps_ul_info
);

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_config_len
(
    rrc_rlc_sps_config_t *p_rrc_rlc_sps_config
);

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_config_tlv_len
(
    rrc_rlc_sps_config_t *p_rrc_rlc_sps_config

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_sps_config
(
    U8  **pp_buffer,
    rrc_rlc_sps_config_t *p_rrc_rlc_sps_config
);

static
rrc_length_t
rrc_il_get_rrc_rlc_create_entity_error_len
(
    rrc_rlc_create_entity_error_t *p_rrc_rlc_create_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_rlc_create_entity_error_tlv_len
(
    rrc_rlc_create_entity_error_t *p_rrc_rlc_create_entity_error

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_create_entity_error
(
    U8  **pp_buffer,
    rrc_rlc_create_entity_error_t *p_rrc_rlc_create_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_rlc_delete_tx_um_rlc_len
(
    rrc_rlc_delete_tx_um_rlc_t *p_rrc_rlc_delete_tx_um_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_delete_tx_um_rlc_tlv_len
(
    rrc_rlc_delete_tx_um_rlc_t *p_rrc_rlc_delete_tx_um_rlc

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_delete_tx_um_rlc
(
    U8  **pp_buffer,
    rrc_rlc_delete_tx_um_rlc_t *p_rrc_rlc_delete_tx_um_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_delete_rx_um_rlc_len
(
    rrc_rlc_delete_rx_um_rlc_t *p_rrc_rlc_delete_rx_um_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_delete_rx_um_rlc_tlv_len
(
    rrc_rlc_delete_rx_um_rlc_t *p_rrc_rlc_delete_rx_um_rlc

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_delete_rx_um_rlc
(
    U8  **pp_buffer,
    rrc_rlc_delete_rx_um_rlc_t *p_rrc_rlc_delete_rx_um_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_delete_tx_rx_um_rlc_len
(
    rrc_rlc_delete_tx_rx_um_rlc_t *p_rrc_rlc_delete_tx_rx_um_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_delete_tx_rx_um_rlc_tlv_len
(
    rrc_rlc_delete_tx_rx_um_rlc_t *p_rrc_rlc_delete_tx_rx_um_rlc

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_delete_tx_rx_um_rlc
(
    U8  **pp_buffer,
    rrc_rlc_delete_tx_rx_um_rlc_t *p_rrc_rlc_delete_tx_rx_um_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_delete_tx_rx_am_rlc_len
(
    rrc_rlc_delete_tx_rx_am_rlc_t *p_rrc_rlc_delete_tx_rx_am_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_delete_tx_rx_am_rlc_tlv_len
(
    rrc_rlc_delete_tx_rx_am_rlc_t *p_rrc_rlc_delete_tx_rx_am_rlc

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_delete_tx_rx_am_rlc
(
    U8  **pp_buffer,
    rrc_rlc_delete_tx_rx_am_rlc_t *p_rrc_rlc_delete_tx_rx_am_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_tx_um_rlc_len
(
    rrc_rlc_reconfig_tx_um_rlc_t *p_rrc_rlc_reconfig_tx_um_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_tx_um_rlc_tlv_len
(
    rrc_rlc_reconfig_tx_um_rlc_t *p_rrc_rlc_reconfig_tx_um_rlc

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_reconfig_tx_um_rlc
(
    U8  **pp_buffer,
    rrc_rlc_reconfig_tx_um_rlc_t *p_rrc_rlc_reconfig_tx_um_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_rx_um_rlc_len
(
    rrc_rlc_reconfig_rx_um_rlc_t *p_rrc_rlc_reconfig_rx_um_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_rx_um_rlc_tlv_len
(
    rrc_rlc_reconfig_rx_um_rlc_t *p_rrc_rlc_reconfig_rx_um_rlc

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_reconfig_rx_um_rlc
(
    U8  **pp_buffer,
    rrc_rlc_reconfig_rx_um_rlc_t *p_rrc_rlc_reconfig_rx_um_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_tx_rx_um_rlc_len
(
    rrc_rlc_reconfig_tx_rx_um_rlc_t *p_rrc_rlc_reconfig_tx_rx_um_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_tx_rx_um_rlc_tlv_len
(
    rrc_rlc_reconfig_tx_rx_um_rlc_t *p_rrc_rlc_reconfig_tx_rx_um_rlc

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_reconfig_tx_rx_um_rlc
(
    U8  **pp_buffer,
    rrc_rlc_reconfig_tx_rx_um_rlc_t *p_rrc_rlc_reconfig_tx_rx_um_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_tx_rx_am_rlc_len
(
    rrc_rlc_reconfig_tx_rx_am_rlc_t *p_rrc_rlc_reconfig_tx_rx_am_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_tx_rx_am_rlc_tlv_len
(
    rrc_rlc_reconfig_tx_rx_am_rlc_t *p_rrc_rlc_reconfig_tx_rx_am_rlc

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_reconfig_tx_rx_am_rlc
(
    U8  **pp_buffer,
    rrc_rlc_reconfig_tx_rx_am_rlc_t *p_rrc_rlc_reconfig_tx_rx_am_rlc
);

static
rrc_length_t
rrc_il_get_rrc_rlc_delete_entity_error_len
(
    rrc_rlc_delete_entity_error_t *p_rrc_rlc_delete_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_rlc_delete_entity_error_tlv_len
(
    rrc_rlc_delete_entity_error_t *p_rrc_rlc_delete_entity_error

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_delete_entity_error
(
    U8  **pp_buffer,
    rrc_rlc_delete_entity_error_t *p_rrc_rlc_delete_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_entity_error_len
(
    rrc_rlc_reconfig_entity_error_t *p_rrc_rlc_reconfig_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_entity_error_tlv_len
(
    rrc_rlc_reconfig_entity_error_t *p_rrc_rlc_reconfig_entity_error

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_reconfig_entity_error
(
    U8  **pp_buffer,
    rrc_rlc_reconfig_entity_error_t *p_rrc_rlc_reconfig_entity_error
);

static
rrc_length_t
rrc_il_get_rrc_rlc_lc_state_len
(
    rrc_rlc_lc_state_t *p_rrc_rlc_lc_state
);

static
rrc_length_t
rrc_il_get_rrc_rlc_lc_state_tlv_len
(
    rrc_rlc_lc_state_t *p_rrc_rlc_lc_state

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_lc_state
(
    U8  **pp_buffer,
    rrc_rlc_lc_state_t *p_rrc_rlc_lc_state
);

static
rrc_length_t
rrc_il_get_rrc_rlc_entity_lcid_len
(
    rrc_rlc_entity_lcid_t *p_rrc_rlc_entity_lcid
);

static
rrc_length_t
rrc_il_get_rrc_rlc_entity_lcid_tlv_len
(
    rrc_rlc_entity_lcid_t *p_rrc_rlc_entity_lcid

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_entity_lcid
(
    U8  **pp_buffer,
    rrc_rlc_entity_lcid_t *p_rrc_rlc_entity_lcid
);

static
rrc_length_t
rrc_il_get_rrc_rlc_entity_lc_id_error_len
(
    rrc_rlc_entity_lc_id_error_t *p_rrc_rlc_entity_lc_id_error
);

static
rrc_length_t
rrc_il_get_rrc_rlc_entity_lc_id_error_tlv_len
(
    rrc_rlc_entity_lc_id_error_t *p_rrc_rlc_entity_lc_id_error

);

static
rrc_return_et
rrc_il_compose_rrc_rlc_entity_lc_id_error
(
    U8  **pp_buffer,
    rrc_rlc_entity_lc_id_error_t *p_rrc_rlc_entity_lc_id_error
);

static
rrc_length_t
rrc_il_get_rlc_communication_info_len
(
    rlc_communication_info_t *p_rlc_communication_info
);

static
rrc_return_et
rrc_il_compose_rlc_communication_info
(
    U8  **pp_buffer,
    rlc_communication_info_t *p_rlc_communication_info
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_length_t
rrc_il_get_rrc_rlc_create_ue_entity_req_len
(
    rrc_rlc_create_ue_entity_req_t *p_rrc_rlc_create_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_create_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_ue_entity_req->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_ue_entity_req->rnti);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_create_ue_entity_req->num_create_tx_um_rlc <= ARRSIZE(p_rrc_rlc_create_ue_entity_req->create_tx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_create_ue_entity_req->num_create_tx_um_rlc; loop++)
        {
            length += rrc_il_get_rrc_rlc_create_tx_um_rlc_tlv_len(&p_rrc_rlc_create_ue_entity_req->create_tx_um_rlc[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_create_ue_entity_req->num_create_rx_um_rlc <= ARRSIZE(p_rrc_rlc_create_ue_entity_req->create_rx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_create_ue_entity_req->num_create_rx_um_rlc; loop++)
        {
            length += rrc_il_get_rrc_rlc_create_rx_um_rlc_tlv_len(&p_rrc_rlc_create_ue_entity_req->create_rx_um_rlc[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_create_ue_entity_req->num_create_tx_rx_um_rlc <= ARRSIZE(p_rrc_rlc_create_ue_entity_req->create_tx_rx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_create_ue_entity_req->num_create_tx_rx_um_rlc; loop++)
        {
            length += rrc_il_get_rrc_rlc_create_tx_rx_um_rlc_tlv_len(&p_rrc_rlc_create_ue_entity_req->create_tx_rx_um_rlc[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_create_ue_entity_req->num_create_tx_rx_am_rlc <= ARRSIZE(p_rrc_rlc_create_ue_entity_req->create_tx_rx_am_rlc));
        for (loop = 0; loop < p_rrc_rlc_create_ue_entity_req->num_create_tx_rx_am_rlc; loop++)
        {
            length += rrc_il_get_rrc_rlc_create_tx_rx_am_rlc_tlv_len(&p_rrc_rlc_create_ue_entity_req->create_tx_rx_am_rlc[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_rlc_create_ue_entity_req->bitmask & RRC_RLC_UE_BIT_RATE_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_ue_bit_rate_info_tlv_len(&p_rrc_rlc_create_ue_entity_req->rlc_ue_bit_rate_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_create_ue_entity_req->bitmask & RRC_RLC_SPS_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_sps_config_tlv_len(&p_rrc_rlc_create_ue_entity_req->sps_config);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_rlc_create_ue_entity_req
(
    U8  **pp_buffer,
    rrc_rlc_create_ue_entity_req_t *p_rrc_rlc_create_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_create_ue_entity_req != PNULL);

    /* This function composes rrc_rlc_create_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_create_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_rlc_create_ue_entity_req->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_create_ue_entity_req->rnti, "rnti");
    *pp_buffer += sizeof(p_rrc_rlc_create_ue_entity_req->rnti);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_create_ue_entity_req->num_create_tx_um_rlc <= ARRSIZE(p_rrc_rlc_create_ue_entity_req->create_tx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_create_ue_entity_req->num_create_tx_um_rlc; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_create_tx_um_rlc(pp_buffer, &p_rrc_rlc_create_ue_entity_req->create_tx_um_rlc[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_create_ue_entity_req->num_create_rx_um_rlc <= ARRSIZE(p_rrc_rlc_create_ue_entity_req->create_rx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_create_ue_entity_req->num_create_rx_um_rlc; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_create_rx_um_rlc(pp_buffer, &p_rrc_rlc_create_ue_entity_req->create_rx_um_rlc[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_create_ue_entity_req->num_create_tx_rx_um_rlc <= ARRSIZE(p_rrc_rlc_create_ue_entity_req->create_tx_rx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_create_ue_entity_req->num_create_tx_rx_um_rlc; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_create_tx_rx_um_rlc(pp_buffer, &p_rrc_rlc_create_ue_entity_req->create_tx_rx_um_rlc[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_create_ue_entity_req->num_create_tx_rx_am_rlc <= ARRSIZE(p_rrc_rlc_create_ue_entity_req->create_tx_rx_am_rlc));
        for (loop = 0; loop < p_rrc_rlc_create_ue_entity_req->num_create_tx_rx_am_rlc; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_create_tx_rx_am_rlc(pp_buffer, &p_rrc_rlc_create_ue_entity_req->create_tx_rx_am_rlc[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_create_ue_entity_req->bitmask & RRC_RLC_UE_BIT_RATE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_ue_bit_rate_info(pp_buffer, &p_rrc_rlc_create_ue_entity_req->rlc_ue_bit_rate_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_create_ue_entity_req->bitmask & RRC_RLC_SPS_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_sps_config(pp_buffer, &p_rrc_rlc_create_ue_entity_req->sps_config))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rrc_rlc_create_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_RLC_CREATE_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rrc_rlc_create_ue_entity_req
(
    rrc_rlc_create_ue_entity_req_t  *p_rrc_rlc_create_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rrc_rlc_create_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_rlc_create_ue_entity_req_len(p_rrc_rlc_create_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_RLC_CREATE_UE_ENTITY_REQ", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_RLC_CREATE_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_RLC_CREATE_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_RLC_CREATE_UE_ENTITY_REQ message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_rlc_create_ue_entity_req(&p_rlc_msg, p_rrc_rlc_create_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
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
rrc_il_get_rrc_rlc_qci_info_len
(
    rrc_rlc_qci_info_t *p_rrc_rlc_qci_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_qci_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_qci_info->qci);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_qci_info_tlv_len
(
    rrc_rlc_qci_info_t *p_rrc_rlc_qci_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_qci_info != PNULL);

    length += rrc_il_get_rrc_rlc_qci_info_len(p_rrc_rlc_qci_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_qci_info
(
    U8  **pp_buffer,
    rrc_rlc_qci_info_t *p_rrc_rlc_qci_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_qci_info != PNULL);

    /* This function composes rrc_rlc_qci_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_QCI_INFO;
    tlv_length = rrc_il_get_rrc_rlc_qci_info_tlv_len(p_rrc_rlc_qci_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_rlc_qci_info->qci < 1) || (p_rrc_rlc_qci_info->qci > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_qci_info->qci] should be in range "
            "1 to 9. Incorrect value %u received.", p_rrc_rlc_qci_info->qci);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_qci_info->qci, "qci");
    *pp_buffer += sizeof(p_rrc_rlc_qci_info->qci);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_tx_bit_rate_info_len
(
    rrc_rlc_tx_bit_rate_info_t *p_rrc_rlc_tx_bit_rate_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_tx_bit_rate_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_tx_bit_rate_info->bit_rate_tx);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_tx_bit_rate_info_tlv_len
(
    rrc_rlc_tx_bit_rate_info_t *p_rrc_rlc_tx_bit_rate_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_tx_bit_rate_info != PNULL);

    length += rrc_il_get_rrc_rlc_tx_bit_rate_info_len(p_rrc_rlc_tx_bit_rate_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_tx_bit_rate_info
(
    U8  **pp_buffer,
    rrc_rlc_tx_bit_rate_info_t *p_rrc_rlc_tx_bit_rate_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_tx_bit_rate_info != PNULL);

    /* This function composes rrc_rlc_tx_bit_rate_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_TX_BIT_RATE_INFO;
    tlv_length = rrc_il_get_rrc_rlc_tx_bit_rate_info_tlv_len(p_rrc_rlc_tx_bit_rate_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_rlc_tx_bit_rate_info->bit_rate_tx, "bit_rate_tx");
    *pp_buffer += sizeof(p_rrc_rlc_tx_bit_rate_info->bit_rate_tx);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_queue_size_info_len
(
    rrc_rlc_queue_size_info_t *p_rrc_rlc_queue_size_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_queue_size_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_queue_size_info->rlc_queue_size);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_queue_size_info_tlv_len
(
    rrc_rlc_queue_size_info_t *p_rrc_rlc_queue_size_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_queue_size_info != PNULL);

    length += rrc_il_get_rrc_rlc_queue_size_info_len(p_rrc_rlc_queue_size_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_queue_size_info
(
    U8  **pp_buffer,
    rrc_rlc_queue_size_info_t *p_rrc_rlc_queue_size_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_queue_size_info != PNULL);

    /* This function composes rrc_rlc_queue_size_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_QUEUE_SIZE_INFO;
    tlv_length = rrc_il_get_rrc_rlc_queue_size_info_tlv_len(p_rrc_rlc_queue_size_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_queue_size_info->rlc_queue_size > 28311552))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_queue_size_info->rlc_queue_size] should be less than"
            " or equal to 28311552. Incorrect value %u received.", p_rrc_rlc_queue_size_info->rlc_queue_size);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_rlc_queue_size_info->rlc_queue_size, "rlc_queue_size");
    *pp_buffer += sizeof(p_rrc_rlc_queue_size_info->rlc_queue_size);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_dc_bearer_type_info_len
(
    rrc_rlc_dc_bearer_type_info *p_rrc_rlc_dc_bearer_type_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_dc_bearer_type_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_dc_bearer_type_info->dc_bearer_type);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_dc_bearer_type_info_tlv_len
(
    rrc_rlc_dc_bearer_type_info *p_rrc_rlc_dc_bearer_type_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_dc_bearer_type_info != PNULL);

    length += rrc_il_get_rrc_rlc_dc_bearer_type_info_len(p_rrc_rlc_dc_bearer_type_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_dc_bearer_type_info
(
    U8  **pp_buffer,
    rrc_rlc_dc_bearer_type_info *p_rrc_rlc_dc_bearer_type_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_dc_bearer_type_info != PNULL);

    /* This function composes rrc_rlc_dc_bearer_type_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_DC_BEARER_TYPE_INFO;
    tlv_length = rrc_il_get_rrc_rlc_dc_bearer_type_info_tlv_len(p_rrc_rlc_dc_bearer_type_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_dc_bearer_type_info->dc_bearer_type > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_dc_bearer_type_info->dc_bearer_type] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_rlc_dc_bearer_type_info->dc_bearer_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_dc_bearer_type_info->dc_bearer_type, "dc_bearer_type");
    *pp_buffer += sizeof(p_rrc_rlc_dc_bearer_type_info->dc_bearer_type);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_create_tx_um_rlc_len
(
    rrc_rlc_create_tx_um_rlc_t *p_rrc_rlc_create_tx_um_rlc
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_create_tx_um_rlc != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_tx_um_rlc->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_tx_um_rlc->sn_field_length);

    /* Get length of TLV */
    if (p_rrc_rlc_create_tx_um_rlc->optional_elems_present & RRC_RLC_QCI_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_qci_info_tlv_len(&p_rrc_rlc_create_tx_um_rlc->rrc_rlc_qci_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_create_tx_um_rlc->optional_elems_present & RRC_RLC_TX_BIT_RATE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_tx_bit_rate_info_tlv_len(&p_rrc_rlc_create_tx_um_rlc->rrc_rlc_tx_bit_rate_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_create_tx_um_rlc->optional_elems_present & RRC_RLC_QUEUE_SIZE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_queue_size_info_tlv_len(&p_rrc_rlc_create_tx_um_rlc->rrc_rlc_queue_size_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_create_tx_um_rlc->optional_elems_present & RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_TX_UM)
    {
        length += rrc_il_get_rrc_rlc_dc_bearer_type_info_tlv_len(&p_rrc_rlc_create_tx_um_rlc->dc_bearer_type_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_create_tx_um_rlc_tlv_len
(
    rrc_rlc_create_tx_um_rlc_t *p_rrc_rlc_create_tx_um_rlc

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_create_tx_um_rlc != PNULL);

    length += rrc_il_get_rrc_rlc_create_tx_um_rlc_len(p_rrc_rlc_create_tx_um_rlc);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_create_tx_um_rlc
(
    U8  **pp_buffer,
    rrc_rlc_create_tx_um_rlc_t *p_rrc_rlc_create_tx_um_rlc
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_create_tx_um_rlc != PNULL);

    /* This function composes rrc_rlc_create_tx_um_rlc */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_CREATE_TX_UM_RLC_ENTITY;
    tlv_length = rrc_il_get_rrc_rlc_create_tx_um_rlc_tlv_len(p_rrc_rlc_create_tx_um_rlc);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_create_tx_um_rlc->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_rlc_create_tx_um_rlc->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_create_tx_um_rlc->sn_field_length, "sn_field_length");
    *pp_buffer += sizeof(p_rrc_rlc_create_tx_um_rlc->sn_field_length);

    /* Compose TLV  */
    if (p_rrc_rlc_create_tx_um_rlc->optional_elems_present & RRC_RLC_QCI_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_qci_info(pp_buffer, &p_rrc_rlc_create_tx_um_rlc->rrc_rlc_qci_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_create_tx_um_rlc->optional_elems_present & RRC_RLC_TX_BIT_RATE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_tx_bit_rate_info(pp_buffer, &p_rrc_rlc_create_tx_um_rlc->rrc_rlc_tx_bit_rate_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_create_tx_um_rlc->optional_elems_present & RRC_RLC_QUEUE_SIZE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_queue_size_info(pp_buffer, &p_rrc_rlc_create_tx_um_rlc->rrc_rlc_queue_size_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_create_tx_um_rlc->optional_elems_present & RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_TX_UM)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_dc_bearer_type_info(pp_buffer, &p_rrc_rlc_create_tx_um_rlc->dc_bearer_type_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_rx_bit_rate_info_len
(
    rrc_rlc_rx_bit_rate_info_t *p_rrc_rlc_rx_bit_rate_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_rx_bit_rate_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_rx_bit_rate_info->bit_rate_rx);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_rx_bit_rate_info_tlv_len
(
    rrc_rlc_rx_bit_rate_info_t *p_rrc_rlc_rx_bit_rate_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_rx_bit_rate_info != PNULL);

    length += rrc_il_get_rrc_rlc_rx_bit_rate_info_len(p_rrc_rlc_rx_bit_rate_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_rx_bit_rate_info
(
    U8  **pp_buffer,
    rrc_rlc_rx_bit_rate_info_t *p_rrc_rlc_rx_bit_rate_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_rx_bit_rate_info != PNULL);

    /* This function composes rrc_rlc_rx_bit_rate_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_RX_BIT_RATE_INFO;
    tlv_length = rrc_il_get_rrc_rlc_rx_bit_rate_info_tlv_len(p_rrc_rlc_rx_bit_rate_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_rlc_rx_bit_rate_info->bit_rate_rx, "bit_rate_rx");
    *pp_buffer += sizeof(p_rrc_rlc_rx_bit_rate_info->bit_rate_rx);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_create_rx_um_rlc_len
(
    rrc_rlc_create_rx_um_rlc_t *p_rrc_rlc_create_rx_um_rlc
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_create_rx_um_rlc != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_rx_um_rlc->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_rx_um_rlc->sn_field_length);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_rx_um_rlc->t_reordering);

    /* Get length of TLV */
    if (p_rrc_rlc_create_rx_um_rlc->optional_elems_present & RRC_RLC_QCI_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_qci_info_tlv_len(&p_rrc_rlc_create_rx_um_rlc->rrc_rlc_qci_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_create_rx_um_rlc->optional_elems_present & RRC_RLC_RX_BIT_RATE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_rx_bit_rate_info_tlv_len(&p_rrc_rlc_create_rx_um_rlc->rrc_rlc_rx_bit_rate_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_create_rx_um_rlc->optional_elems_present & RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_RX_UM)
    {
        length += rrc_il_get_rrc_rlc_dc_bearer_type_info_tlv_len(&p_rrc_rlc_create_rx_um_rlc->dc_bearer_type_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_create_rx_um_rlc_tlv_len
(
    rrc_rlc_create_rx_um_rlc_t *p_rrc_rlc_create_rx_um_rlc

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_create_rx_um_rlc != PNULL);

    length += rrc_il_get_rrc_rlc_create_rx_um_rlc_len(p_rrc_rlc_create_rx_um_rlc);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_create_rx_um_rlc
(
    U8  **pp_buffer,
    rrc_rlc_create_rx_um_rlc_t *p_rrc_rlc_create_rx_um_rlc
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_create_rx_um_rlc != PNULL);

    /* This function composes rrc_rlc_create_rx_um_rlc */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_CREATE_RX_UM_RLC_ENTITY;
    tlv_length = rrc_il_get_rrc_rlc_create_rx_um_rlc_tlv_len(p_rrc_rlc_create_rx_um_rlc);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_create_rx_um_rlc->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_rlc_create_rx_um_rlc->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_create_rx_um_rlc->sn_field_length, "sn_field_length");
    *pp_buffer += sizeof(p_rrc_rlc_create_rx_um_rlc->sn_field_length);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_create_rx_um_rlc->t_reordering, "t_reordering");
    *pp_buffer += sizeof(p_rrc_rlc_create_rx_um_rlc->t_reordering);

    /* Compose TLV  */
    if (p_rrc_rlc_create_rx_um_rlc->optional_elems_present & RRC_RLC_QCI_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_qci_info(pp_buffer, &p_rrc_rlc_create_rx_um_rlc->rrc_rlc_qci_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_create_rx_um_rlc->optional_elems_present & RRC_RLC_RX_BIT_RATE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_rx_bit_rate_info(pp_buffer, &p_rrc_rlc_create_rx_um_rlc->rrc_rlc_rx_bit_rate_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_create_rx_um_rlc->optional_elems_present & RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_RX_UM)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_dc_bearer_type_info(pp_buffer, &p_rrc_rlc_create_rx_um_rlc->dc_bearer_type_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_create_tx_rx_um_rlc_len
(
    rrc_rlc_create_tx_rx_um_rlc_t *p_rrc_rlc_create_tx_rx_um_rlc
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_create_tx_rx_um_rlc != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_tx_rx_um_rlc->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_tx_rx_um_rlc->sn_field_length_tx);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_tx_rx_um_rlc->sn_field_length_rx);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_tx_rx_um_rlc->t_reordering);

    /* Get length of TLV */
    if (p_rrc_rlc_create_tx_rx_um_rlc->optional_elems_present & RRC_RLC_QCI_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_qci_info_tlv_len(&p_rrc_rlc_create_tx_rx_um_rlc->rrc_rlc_qci_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_create_tx_rx_um_rlc->optional_elems_present & RRC_RLC_TX_BIT_RATE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_tx_bit_rate_info_tlv_len(&p_rrc_rlc_create_tx_rx_um_rlc->rrc_rlc_tx_bit_rate_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_create_tx_rx_um_rlc->optional_elems_present & RRC_RLC_RX_BIT_RATE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_rx_bit_rate_info_tlv_len(&p_rrc_rlc_create_tx_rx_um_rlc->rrc_rlc_rx_bit_rate_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_create_tx_rx_um_rlc->optional_elems_present & RRC_RLC_QUEUE_SIZE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_queue_size_info_tlv_len(&p_rrc_rlc_create_tx_rx_um_rlc->rrc_rlc_queue_size_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_create_tx_rx_um_rlc->optional_elems_present & RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_TX_RX_UM)
    {
        length += rrc_il_get_rrc_rlc_dc_bearer_type_info_tlv_len(&p_rrc_rlc_create_tx_rx_um_rlc->dc_bearer_type_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_create_tx_rx_um_rlc_tlv_len
(
    rrc_rlc_create_tx_rx_um_rlc_t *p_rrc_rlc_create_tx_rx_um_rlc

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_create_tx_rx_um_rlc != PNULL);

    length += rrc_il_get_rrc_rlc_create_tx_rx_um_rlc_len(p_rrc_rlc_create_tx_rx_um_rlc);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_create_tx_rx_um_rlc
(
    U8  **pp_buffer,
    rrc_rlc_create_tx_rx_um_rlc_t *p_rrc_rlc_create_tx_rx_um_rlc
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_create_tx_rx_um_rlc != PNULL);

    /* This function composes rrc_rlc_create_tx_rx_um_rlc */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_CREATE_TX_RX_UM_RLC_ENTITY;
    tlv_length = rrc_il_get_rrc_rlc_create_tx_rx_um_rlc_tlv_len(p_rrc_rlc_create_tx_rx_um_rlc);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_create_tx_rx_um_rlc->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_rlc_create_tx_rx_um_rlc->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_create_tx_rx_um_rlc->sn_field_length_tx, "sn_field_length_tx");
    *pp_buffer += sizeof(p_rrc_rlc_create_tx_rx_um_rlc->sn_field_length_tx);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_create_tx_rx_um_rlc->sn_field_length_rx, "sn_field_length_rx");
    *pp_buffer += sizeof(p_rrc_rlc_create_tx_rx_um_rlc->sn_field_length_rx);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_create_tx_rx_um_rlc->t_reordering, "t_reordering");
    *pp_buffer += sizeof(p_rrc_rlc_create_tx_rx_um_rlc->t_reordering);

    /* Compose TLV  */
    if (p_rrc_rlc_create_tx_rx_um_rlc->optional_elems_present & RRC_RLC_QCI_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_qci_info(pp_buffer, &p_rrc_rlc_create_tx_rx_um_rlc->rrc_rlc_qci_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_create_tx_rx_um_rlc->optional_elems_present & RRC_RLC_TX_BIT_RATE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_tx_bit_rate_info(pp_buffer, &p_rrc_rlc_create_tx_rx_um_rlc->rrc_rlc_tx_bit_rate_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_create_tx_rx_um_rlc->optional_elems_present & RRC_RLC_RX_BIT_RATE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_rx_bit_rate_info(pp_buffer, &p_rrc_rlc_create_tx_rx_um_rlc->rrc_rlc_rx_bit_rate_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_create_tx_rx_um_rlc->optional_elems_present & RRC_RLC_QUEUE_SIZE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_queue_size_info(pp_buffer, &p_rrc_rlc_create_tx_rx_um_rlc->rrc_rlc_queue_size_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_create_tx_rx_um_rlc->optional_elems_present & RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_TX_RX_UM)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_dc_bearer_type_info(pp_buffer, &p_rrc_rlc_create_tx_rx_um_rlc->dc_bearer_type_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_create_tx_rx_am_rlc_len
(
    rrc_rlc_create_tx_rx_am_rlc_t *p_rrc_rlc_create_tx_rx_am_rlc
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_create_tx_rx_am_rlc != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_tx_rx_am_rlc->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_tx_rx_am_rlc->t_poll_retransmission);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_tx_rx_am_rlc->poll_pdu);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_tx_rx_am_rlc->poll_byte);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_tx_rx_am_rlc->max_re_tx_thrsh_ld);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_tx_rx_am_rlc->t_reordering);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_tx_rx_am_rlc->t_status_prohibit);

    /* Get length of TLV */
    if (p_rrc_rlc_create_tx_rx_am_rlc->optional_elems_present & RRC_RLC_QCI_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_qci_info_tlv_len(&p_rrc_rlc_create_tx_rx_am_rlc->rrc_rlc_qci_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_create_tx_rx_am_rlc->optional_elems_present & RRC_RLC_TX_BIT_RATE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_tx_bit_rate_info_tlv_len(&p_rrc_rlc_create_tx_rx_am_rlc->rrc_rlc_tx_bit_rate_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_create_tx_rx_am_rlc->optional_elems_present & RRC_RLC_RX_BIT_RATE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_rx_bit_rate_info_tlv_len(&p_rrc_rlc_create_tx_rx_am_rlc->rrc_rlc_rx_bit_rate_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_create_tx_rx_am_rlc->optional_elems_present & RRC_RLC_QUEUE_SIZE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_queue_size_info_tlv_len(&p_rrc_rlc_create_tx_rx_am_rlc->rrc_rlc_queue_size_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_create_tx_rx_am_rlc->optional_elems_present & RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_TX_RX_AM)
    {
        length += rrc_il_get_rrc_rlc_dc_bearer_type_info_tlv_len(&p_rrc_rlc_create_tx_rx_am_rlc->dc_bearer_type_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_create_tx_rx_am_rlc_tlv_len
(
    rrc_rlc_create_tx_rx_am_rlc_t *p_rrc_rlc_create_tx_rx_am_rlc

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_create_tx_rx_am_rlc != PNULL);

    length += rrc_il_get_rrc_rlc_create_tx_rx_am_rlc_len(p_rrc_rlc_create_tx_rx_am_rlc);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_create_tx_rx_am_rlc
(
    U8  **pp_buffer,
    rrc_rlc_create_tx_rx_am_rlc_t *p_rrc_rlc_create_tx_rx_am_rlc
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_create_tx_rx_am_rlc != PNULL);

    /* This function composes rrc_rlc_create_tx_rx_am_rlc */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_CREATE_TX_RX_AM_RLC_ENTITY;
    tlv_length = rrc_il_get_rrc_rlc_create_tx_rx_am_rlc_tlv_len(p_rrc_rlc_create_tx_rx_am_rlc);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_create_tx_rx_am_rlc->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_rlc_create_tx_rx_am_rlc->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_create_tx_rx_am_rlc->t_poll_retransmission, "t_poll_retransmission");
    *pp_buffer += sizeof(p_rrc_rlc_create_tx_rx_am_rlc->t_poll_retransmission);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_create_tx_rx_am_rlc->poll_pdu, "poll_pdu");
    *pp_buffer += sizeof(p_rrc_rlc_create_tx_rx_am_rlc->poll_pdu);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_create_tx_rx_am_rlc->poll_byte, "poll_byte");
    *pp_buffer += sizeof(p_rrc_rlc_create_tx_rx_am_rlc->poll_byte);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_create_tx_rx_am_rlc->max_re_tx_thrsh_ld, "max_re_tx_thrsh_ld");
    *pp_buffer += sizeof(p_rrc_rlc_create_tx_rx_am_rlc->max_re_tx_thrsh_ld);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_create_tx_rx_am_rlc->t_reordering, "t_reordering");
    *pp_buffer += sizeof(p_rrc_rlc_create_tx_rx_am_rlc->t_reordering);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_create_tx_rx_am_rlc->t_status_prohibit, "t_status_prohibit");
    *pp_buffer += sizeof(p_rrc_rlc_create_tx_rx_am_rlc->t_status_prohibit);

    /* Compose TLV  */
    if (p_rrc_rlc_create_tx_rx_am_rlc->optional_elems_present & RRC_RLC_QCI_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_qci_info(pp_buffer, &p_rrc_rlc_create_tx_rx_am_rlc->rrc_rlc_qci_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_create_tx_rx_am_rlc->optional_elems_present & RRC_RLC_TX_BIT_RATE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_tx_bit_rate_info(pp_buffer, &p_rrc_rlc_create_tx_rx_am_rlc->rrc_rlc_tx_bit_rate_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_create_tx_rx_am_rlc->optional_elems_present & RRC_RLC_RX_BIT_RATE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_rx_bit_rate_info(pp_buffer, &p_rrc_rlc_create_tx_rx_am_rlc->rrc_rlc_rx_bit_rate_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_create_tx_rx_am_rlc->optional_elems_present & RRC_RLC_QUEUE_SIZE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_queue_size_info(pp_buffer, &p_rrc_rlc_create_tx_rx_am_rlc->rrc_rlc_queue_size_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_create_tx_rx_am_rlc->optional_elems_present & RRC_RLC_DC_BEARER_TYPE_INFO_PRESENT_FOR_TX_RX_AM)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_dc_bearer_type_info(pp_buffer, &p_rrc_rlc_create_tx_rx_am_rlc->dc_bearer_type_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_ue_bit_rate_info_len
(
    rrc_rlc_ue_bit_rate_info_t *p_rrc_rlc_ue_bit_rate_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_ue_bit_rate_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_ue_bit_rate_info->bit_rate_tx);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_ue_bit_rate_info->bit_rate_rx);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_ue_bit_rate_info_tlv_len
(
    rrc_rlc_ue_bit_rate_info_t *p_rrc_rlc_ue_bit_rate_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_ue_bit_rate_info != PNULL);

    length += rrc_il_get_rrc_rlc_ue_bit_rate_info_len(p_rrc_rlc_ue_bit_rate_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_ue_bit_rate_info
(
    U8  **pp_buffer,
    rrc_rlc_ue_bit_rate_info_t *p_rrc_rlc_ue_bit_rate_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_ue_bit_rate_info != PNULL);

    /* This function composes rrc_rlc_ue_bit_rate_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_UE_BIT_RATE_INFO;
    tlv_length = rrc_il_get_rrc_rlc_ue_bit_rate_info_tlv_len(p_rrc_rlc_ue_bit_rate_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_rlc_ue_bit_rate_info->bit_rate_tx, "bit_rate_tx");
    *pp_buffer += sizeof(p_rrc_rlc_ue_bit_rate_info->bit_rate_tx);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_rlc_ue_bit_rate_info->bit_rate_rx, "bit_rate_rx");
    *pp_buffer += sizeof(p_rrc_rlc_ue_bit_rate_info->bit_rate_rx);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_dl_setup_info_len
(
    rrc_rlc_sps_dl_setup_info_t *p_rrc_rlc_sps_dl_setup_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_sps_dl_setup_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_sps_dl_setup_info->semi_persistent_sched_interval_dl);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_dl_setup_info_tlv_len
(
    rrc_rlc_sps_dl_setup_info_t *p_rrc_rlc_sps_dl_setup_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_sps_dl_setup_info != PNULL);

    length += rrc_il_get_rrc_rlc_sps_dl_setup_info_len(p_rrc_rlc_sps_dl_setup_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_sps_dl_setup_info
(
    U8  **pp_buffer,
    rrc_rlc_sps_dl_setup_info_t *p_rrc_rlc_sps_dl_setup_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_sps_dl_setup_info != PNULL);

    /* This function composes rrc_rlc_sps_dl_setup_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_SPS_DL_SETUP_INFO_TAG;
    tlv_length = rrc_il_get_rrc_rlc_sps_dl_setup_info_tlv_len(p_rrc_rlc_sps_dl_setup_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_sps_dl_setup_info->semi_persistent_sched_interval_dl > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_sps_dl_setup_info->semi_persistent_sched_interval_dl] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_rlc_sps_dl_setup_info->semi_persistent_sched_interval_dl);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_sps_dl_setup_info->semi_persistent_sched_interval_dl, "semi_persistent_sched_interval_dl");
    *pp_buffer += sizeof(p_rrc_rlc_sps_dl_setup_info->semi_persistent_sched_interval_dl);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_dl_info_len
(
    rrc_rlc_sps_dl_info_t *p_rrc_rlc_sps_dl_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_sps_dl_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_sps_dl_info->request_type);

    /* Get length of TLV */
    if (p_rrc_rlc_sps_dl_info->optional_elems_present & RRC_RLC_SPS_DL_SETUP_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_sps_dl_setup_info_tlv_len(&p_rrc_rlc_sps_dl_info->sps_dl_setup_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_dl_info_tlv_len
(
    rrc_rlc_sps_dl_info_t *p_rrc_rlc_sps_dl_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_sps_dl_info != PNULL);

    length += rrc_il_get_rrc_rlc_sps_dl_info_len(p_rrc_rlc_sps_dl_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_sps_dl_info
(
    U8  **pp_buffer,
    rrc_rlc_sps_dl_info_t *p_rrc_rlc_sps_dl_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_sps_dl_info != PNULL);

    /* This function composes rrc_rlc_sps_dl_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_SPS_DL_INFO_TAG;
    tlv_length = rrc_il_get_rrc_rlc_sps_dl_info_tlv_len(p_rrc_rlc_sps_dl_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_sps_dl_info->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_sps_dl_info->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_rlc_sps_dl_info->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_sps_dl_info->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_rlc_sps_dl_info->request_type);

    /* Compose TLV  */
    if (p_rrc_rlc_sps_dl_info->optional_elems_present & RRC_RLC_SPS_DL_SETUP_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_sps_dl_setup_info(pp_buffer, &p_rrc_rlc_sps_dl_info->sps_dl_setup_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_ul_setup_info_len
(
    rrc_rlc_sps_ul_setup_info_t *p_rrc_rlc_sps_ul_setup_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_sps_ul_setup_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_sps_ul_setup_info->semi_persistent_sched_interval_ul);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_ul_setup_info_tlv_len
(
    rrc_rlc_sps_ul_setup_info_t *p_rrc_rlc_sps_ul_setup_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_sps_ul_setup_info != PNULL);

    length += rrc_il_get_rrc_rlc_sps_ul_setup_info_len(p_rrc_rlc_sps_ul_setup_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_sps_ul_setup_info
(
    U8  **pp_buffer,
    rrc_rlc_sps_ul_setup_info_t *p_rrc_rlc_sps_ul_setup_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_sps_ul_setup_info != PNULL);

    /* This function composes rrc_rlc_sps_ul_setup_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_SPS_UL_SETUP_INFO_TAG;
    tlv_length = rrc_il_get_rrc_rlc_sps_ul_setup_info_tlv_len(p_rrc_rlc_sps_ul_setup_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_sps_ul_setup_info->semi_persistent_sched_interval_ul > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_sps_ul_setup_info->semi_persistent_sched_interval_ul] should be less than"
            " or equal to 9. Incorrect value %u received.", p_rrc_rlc_sps_ul_setup_info->semi_persistent_sched_interval_ul);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_sps_ul_setup_info->semi_persistent_sched_interval_ul, "semi_persistent_sched_interval_ul");
    *pp_buffer += sizeof(p_rrc_rlc_sps_ul_setup_info->semi_persistent_sched_interval_ul);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_ul_info_len
(
    rrc_rlc_sps_ul_info_t *p_rrc_rlc_sps_ul_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_sps_ul_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_sps_ul_info->request_type);

    /* Get length of TLV */
    if (p_rrc_rlc_sps_ul_info->optional_elems_present & RRC_RLC_SPS_UL_SETUP_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_sps_ul_setup_info_tlv_len(&p_rrc_rlc_sps_ul_info->sps_ul_setup_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_ul_info_tlv_len
(
    rrc_rlc_sps_ul_info_t *p_rrc_rlc_sps_ul_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_sps_ul_info != PNULL);

    length += rrc_il_get_rrc_rlc_sps_ul_info_len(p_rrc_rlc_sps_ul_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_sps_ul_info
(
    U8  **pp_buffer,
    rrc_rlc_sps_ul_info_t *p_rrc_rlc_sps_ul_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_sps_ul_info != PNULL);

    /* This function composes rrc_rlc_sps_ul_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_SPS_UL_INFO_TAG;
    tlv_length = rrc_il_get_rrc_rlc_sps_ul_info_tlv_len(p_rrc_rlc_sps_ul_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_sps_ul_info->request_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_sps_ul_info->request_type] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_rlc_sps_ul_info->request_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_sps_ul_info->request_type, "request_type");
    *pp_buffer += sizeof(p_rrc_rlc_sps_ul_info->request_type);

    /* Compose TLV  */
    if (p_rrc_rlc_sps_ul_info->optional_elems_present & RRC_RLC_SPS_UL_SETUP_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_sps_ul_setup_info(pp_buffer, &p_rrc_rlc_sps_ul_info->sps_ul_setup_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_config_len
(
    rrc_rlc_sps_config_t *p_rrc_rlc_sps_config
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_sps_config != PNULL);

    /* Get length of TLV */
    if (p_rrc_rlc_sps_config->optional_elems_present & RRC_RLC_SPS_DL_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_sps_dl_info_tlv_len(&p_rrc_rlc_sps_config->sps_dl_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_sps_config->optional_elems_present & RRC_RLC_SPS_UL_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_sps_ul_info_tlv_len(&p_rrc_rlc_sps_config->sps_ul_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_sps_config_tlv_len
(
    rrc_rlc_sps_config_t *p_rrc_rlc_sps_config

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_sps_config != PNULL);

    length += rrc_il_get_rrc_rlc_sps_config_len(p_rrc_rlc_sps_config);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_sps_config
(
    U8  **pp_buffer,
    rrc_rlc_sps_config_t *p_rrc_rlc_sps_config
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_sps_config != PNULL);

    /* This function composes rrc_rlc_sps_config */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_SPS_CONFIG_TAG;
    tlv_length = rrc_il_get_rrc_rlc_sps_config_tlv_len(p_rrc_rlc_sps_config);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose TLV  */
    if (p_rrc_rlc_sps_config->optional_elems_present & RRC_RLC_SPS_DL_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_sps_dl_info(pp_buffer, &p_rrc_rlc_sps_config->sps_dl_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_sps_config->optional_elems_present & RRC_RLC_SPS_UL_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_sps_ul_info(pp_buffer, &p_rrc_rlc_sps_config->sps_ul_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_rlc_create_ue_entity_cnf_len
(
    rrc_rlc_create_ue_entity_cnf_t *p_rrc_rlc_create_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_create_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_create_ue_entity_cnf->num_create_entity_error <= ARRSIZE(p_rrc_rlc_create_ue_entity_cnf->create_error_entities));
        for (loop = 0; loop < p_rrc_rlc_create_ue_entity_cnf->num_create_entity_error; loop++)
        {
            length += rrc_il_get_rrc_rlc_create_entity_error_tlv_len(&p_rrc_rlc_create_ue_entity_cnf->create_error_entities[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_rlc_create_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_rlc_create_ue_entity_cnf_t *p_rrc_rlc_create_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_create_ue_entity_cnf != PNULL);

    /* This function composes rrc_rlc_create_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_create_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_rlc_create_ue_entity_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_create_ue_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_create_ue_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_rlc_create_ue_entity_cnf->response_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_create_ue_entity_cnf->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_rlc_create_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_create_ue_entity_cnf->num_create_entity_error <= ARRSIZE(p_rrc_rlc_create_ue_entity_cnf->create_error_entities));
        for (loop = 0; loop < p_rrc_rlc_create_ue_entity_cnf->num_create_entity_error; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_create_entity_error(pp_buffer, &p_rrc_rlc_create_ue_entity_cnf->create_error_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rrc_rlc_create_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_RLC_CREATE_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rrc_rlc_create_ue_entity_cnf
(
    rrc_rlc_create_ue_entity_cnf_t  *p_rrc_rlc_create_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rrc_rlc_create_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_rlc_create_ue_entity_cnf_len(p_rrc_rlc_create_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_RLC_CREATE_UE_ENTITY_CNF", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_RLC_CREATE_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_RLC_CREATE_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_RLC_CREATE_UE_ENTITY_CNF message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_rlc_create_ue_entity_cnf(&p_rlc_msg, p_rrc_rlc_create_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
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
rrc_il_get_rrc_rlc_create_entity_error_len
(
    rrc_rlc_create_entity_error_t *p_rrc_rlc_create_entity_error
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_create_entity_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_entity_error->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_entity_error->response);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_create_entity_error_tlv_len
(
    rrc_rlc_create_entity_error_t *p_rrc_rlc_create_entity_error

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_create_entity_error != PNULL);

    length += rrc_il_get_rrc_rlc_create_entity_error_len(p_rrc_rlc_create_entity_error);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_create_entity_error
(
    U8  **pp_buffer,
    rrc_rlc_create_entity_error_t *p_rrc_rlc_create_entity_error
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_create_entity_error != PNULL);

    /* This function composes rrc_rlc_create_entity_error */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_CREATE_ENTITY_ERROR;
    tlv_length = rrc_il_get_rrc_rlc_create_entity_error_tlv_len(p_rrc_rlc_create_entity_error);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_create_entity_error->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_rlc_create_entity_error->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_create_entity_error->response, "response");
    *pp_buffer += sizeof(p_rrc_rlc_create_entity_error->response);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_rlc_reconfig_ue_entity_req_len
(
    rrc_rlc_reconfig_ue_entity_req_t *p_rrc_rlc_reconfig_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_reconfig_ue_entity_req->ue_index);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_um_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->create_tx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_um_rlc; loop++)
        {
            length += rrc_il_get_rrc_rlc_create_tx_um_rlc_tlv_len(&p_rrc_rlc_reconfig_ue_entity_req->create_tx_um_rlc[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_create_rx_um_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->create_rx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_create_rx_um_rlc; loop++)
        {
            length += rrc_il_get_rrc_rlc_create_rx_um_rlc_tlv_len(&p_rrc_rlc_reconfig_ue_entity_req->create_rx_um_rlc[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_rx_um_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->create_tx_rx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_rx_um_rlc; loop++)
        {
            length += rrc_il_get_rrc_rlc_create_tx_rx_um_rlc_tlv_len(&p_rrc_rlc_reconfig_ue_entity_req->create_tx_rx_um_rlc[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_rx_am_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->create_tx_rx_am_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_rx_am_rlc; loop++)
        {
            length += rrc_il_get_rrc_rlc_create_tx_rx_am_rlc_tlv_len(&p_rrc_rlc_reconfig_ue_entity_req->create_tx_rx_am_rlc[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_um_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->delete_tx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_um_rlc; loop++)
        {
            length += rrc_il_get_rrc_rlc_delete_tx_um_rlc_tlv_len(&p_rrc_rlc_reconfig_ue_entity_req->delete_tx_um_rlc[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_delete_rx_um_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->delete_rx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_delete_rx_um_rlc; loop++)
        {
            length += rrc_il_get_rrc_rlc_delete_rx_um_rlc_tlv_len(&p_rrc_rlc_reconfig_ue_entity_req->delete_rx_um_rlc[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_rx_um_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->delete_tx_rx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_rx_um_rlc; loop++)
        {
            length += rrc_il_get_rrc_rlc_delete_tx_rx_um_rlc_tlv_len(&p_rrc_rlc_reconfig_ue_entity_req->delete_tx_rx_um_rlc[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_rx_am_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->delete_tx_rx_am_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_rx_am_rlc; loop++)
        {
            length += rrc_il_get_rrc_rlc_delete_tx_rx_am_rlc_tlv_len(&p_rrc_rlc_reconfig_ue_entity_req->delete_tx_rx_am_rlc[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_um_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->reconfig_tx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_um_rlc; loop++)
        {
            length += rrc_il_get_rrc_rlc_reconfig_tx_um_rlc_tlv_len(&p_rrc_rlc_reconfig_ue_entity_req->reconfig_tx_um_rlc[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_rx_um_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->reconfig_rx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_rx_um_rlc; loop++)
        {
            length += rrc_il_get_rrc_rlc_reconfig_rx_um_rlc_tlv_len(&p_rrc_rlc_reconfig_ue_entity_req->reconfig_rx_um_rlc[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_rx_um_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->reconfig_tx_rx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_rx_um_rlc; loop++)
        {
            length += rrc_il_get_rrc_rlc_reconfig_tx_rx_um_rlc_tlv_len(&p_rrc_rlc_reconfig_ue_entity_req->reconfig_tx_rx_um_rlc[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_rx_am_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->reconfig_tx_rx_am_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_rx_am_rlc; loop++)
        {
            length += rrc_il_get_rrc_rlc_reconfig_tx_rx_am_rlc_tlv_len(&p_rrc_rlc_reconfig_ue_entity_req->reconfig_tx_rx_am_rlc[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_rlc_reconfig_ue_entity_req->bitmask & RRC_RLC_RECONFIG_UE_BIT_RATE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_ue_bit_rate_info_tlv_len(&p_rrc_rlc_reconfig_ue_entity_req->rlc_ue_bit_rate_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_reconfig_ue_entity_req->bitmask & RRC_RLC_RECONFIG_UE_SPS_CONFIG_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_sps_config_tlv_len(&p_rrc_rlc_reconfig_ue_entity_req->sps_config);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_rlc_reconfig_ue_entity_req
(
    U8  **pp_buffer,
    rrc_rlc_reconfig_ue_entity_req_t *p_rrc_rlc_reconfig_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req != PNULL);

    /* This function composes rrc_rlc_reconfig_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_reconfig_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_rlc_reconfig_ue_entity_req->ue_index);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_um_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->create_tx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_um_rlc; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_create_tx_um_rlc(pp_buffer, &p_rrc_rlc_reconfig_ue_entity_req->create_tx_um_rlc[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_create_rx_um_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->create_rx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_create_rx_um_rlc; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_create_rx_um_rlc(pp_buffer, &p_rrc_rlc_reconfig_ue_entity_req->create_rx_um_rlc[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_rx_um_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->create_tx_rx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_rx_um_rlc; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_create_tx_rx_um_rlc(pp_buffer, &p_rrc_rlc_reconfig_ue_entity_req->create_tx_rx_um_rlc[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_rx_am_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->create_tx_rx_am_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_create_tx_rx_am_rlc; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_create_tx_rx_am_rlc(pp_buffer, &p_rrc_rlc_reconfig_ue_entity_req->create_tx_rx_am_rlc[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_um_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->delete_tx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_um_rlc; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_delete_tx_um_rlc(pp_buffer, &p_rrc_rlc_reconfig_ue_entity_req->delete_tx_um_rlc[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_delete_rx_um_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->delete_rx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_delete_rx_um_rlc; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_delete_rx_um_rlc(pp_buffer, &p_rrc_rlc_reconfig_ue_entity_req->delete_rx_um_rlc[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_rx_um_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->delete_tx_rx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_rx_um_rlc; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_delete_tx_rx_um_rlc(pp_buffer, &p_rrc_rlc_reconfig_ue_entity_req->delete_tx_rx_um_rlc[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_rx_am_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->delete_tx_rx_am_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_delete_tx_rx_am_rlc; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_delete_tx_rx_am_rlc(pp_buffer, &p_rrc_rlc_reconfig_ue_entity_req->delete_tx_rx_am_rlc[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_um_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->reconfig_tx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_um_rlc; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_reconfig_tx_um_rlc(pp_buffer, &p_rrc_rlc_reconfig_ue_entity_req->reconfig_tx_um_rlc[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_rx_um_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->reconfig_rx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_rx_um_rlc; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_reconfig_rx_um_rlc(pp_buffer, &p_rrc_rlc_reconfig_ue_entity_req->reconfig_rx_um_rlc[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_rx_um_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->reconfig_tx_rx_um_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_rx_um_rlc; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_reconfig_tx_rx_um_rlc(pp_buffer, &p_rrc_rlc_reconfig_ue_entity_req->reconfig_tx_rx_um_rlc[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_rx_am_rlc <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_req->reconfig_tx_rx_am_rlc));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_req->num_reconfig_tx_rx_am_rlc; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_reconfig_tx_rx_am_rlc(pp_buffer, &p_rrc_rlc_reconfig_ue_entity_req->reconfig_tx_rx_am_rlc[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_reconfig_ue_entity_req->bitmask & RRC_RLC_RECONFIG_UE_BIT_RATE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_ue_bit_rate_info(pp_buffer, &p_rrc_rlc_reconfig_ue_entity_req->rlc_ue_bit_rate_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_reconfig_ue_entity_req->bitmask & RRC_RLC_RECONFIG_UE_SPS_CONFIG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_sps_config(pp_buffer, &p_rrc_rlc_reconfig_ue_entity_req->sps_config))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rrc_rlc_reconfig_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_RLC_RECONFIG_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rrc_rlc_reconfig_ue_entity_req
(
    rrc_rlc_reconfig_ue_entity_req_t  *p_rrc_rlc_reconfig_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_rlc_reconfig_ue_entity_req_len(p_rrc_rlc_reconfig_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_RLC_RECONFIG_UE_ENTITY_REQ", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_RLC_RECONFIG_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_RLC_RECONFIG_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_RLC_RECONFIG_UE_ENTITY_REQ message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_rlc_reconfig_ue_entity_req(&p_rlc_msg, p_rrc_rlc_reconfig_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
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
rrc_il_get_rrc_rlc_delete_tx_um_rlc_len
(
    rrc_rlc_delete_tx_um_rlc_t *p_rrc_rlc_delete_tx_um_rlc
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_delete_tx_um_rlc != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_delete_tx_um_rlc->lc_id);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_delete_tx_um_rlc_tlv_len
(
    rrc_rlc_delete_tx_um_rlc_t *p_rrc_rlc_delete_tx_um_rlc

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_delete_tx_um_rlc != PNULL);

    length += rrc_il_get_rrc_rlc_delete_tx_um_rlc_len(p_rrc_rlc_delete_tx_um_rlc);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_delete_tx_um_rlc
(
    U8  **pp_buffer,
    rrc_rlc_delete_tx_um_rlc_t *p_rrc_rlc_delete_tx_um_rlc
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_delete_tx_um_rlc != PNULL);

    /* This function composes rrc_rlc_delete_tx_um_rlc */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_DELETE_TX_UM_RLC_ENTITY;
    tlv_length = rrc_il_get_rrc_rlc_delete_tx_um_rlc_tlv_len(p_rrc_rlc_delete_tx_um_rlc);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_delete_tx_um_rlc->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_rlc_delete_tx_um_rlc->lc_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_delete_rx_um_rlc_len
(
    rrc_rlc_delete_rx_um_rlc_t *p_rrc_rlc_delete_rx_um_rlc
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_delete_rx_um_rlc != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_delete_rx_um_rlc->lc_id);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_delete_rx_um_rlc_tlv_len
(
    rrc_rlc_delete_rx_um_rlc_t *p_rrc_rlc_delete_rx_um_rlc

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_delete_rx_um_rlc != PNULL);

    length += rrc_il_get_rrc_rlc_delete_rx_um_rlc_len(p_rrc_rlc_delete_rx_um_rlc);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_delete_rx_um_rlc
(
    U8  **pp_buffer,
    rrc_rlc_delete_rx_um_rlc_t *p_rrc_rlc_delete_rx_um_rlc
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_delete_rx_um_rlc != PNULL);

    /* This function composes rrc_rlc_delete_rx_um_rlc */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_DELETE_RX_UM_RLC_ENTITY;
    tlv_length = rrc_il_get_rrc_rlc_delete_rx_um_rlc_tlv_len(p_rrc_rlc_delete_rx_um_rlc);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_delete_rx_um_rlc->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_rlc_delete_rx_um_rlc->lc_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_delete_tx_rx_um_rlc_len
(
    rrc_rlc_delete_tx_rx_um_rlc_t *p_rrc_rlc_delete_tx_rx_um_rlc
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_delete_tx_rx_um_rlc != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_delete_tx_rx_um_rlc->lc_id);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_delete_tx_rx_um_rlc_tlv_len
(
    rrc_rlc_delete_tx_rx_um_rlc_t *p_rrc_rlc_delete_tx_rx_um_rlc

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_delete_tx_rx_um_rlc != PNULL);

    length += rrc_il_get_rrc_rlc_delete_tx_rx_um_rlc_len(p_rrc_rlc_delete_tx_rx_um_rlc);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_delete_tx_rx_um_rlc
(
    U8  **pp_buffer,
    rrc_rlc_delete_tx_rx_um_rlc_t *p_rrc_rlc_delete_tx_rx_um_rlc
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_delete_tx_rx_um_rlc != PNULL);

    /* This function composes rrc_rlc_delete_tx_rx_um_rlc */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_DELETE_TX_RX_UM_RLC_ENTITY;
    tlv_length = rrc_il_get_rrc_rlc_delete_tx_rx_um_rlc_tlv_len(p_rrc_rlc_delete_tx_rx_um_rlc);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_delete_tx_rx_um_rlc->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_rlc_delete_tx_rx_um_rlc->lc_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_delete_tx_rx_am_rlc_len
(
    rrc_rlc_delete_tx_rx_am_rlc_t *p_rrc_rlc_delete_tx_rx_am_rlc
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_delete_tx_rx_am_rlc != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_delete_tx_rx_am_rlc->lc_id);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_delete_tx_rx_am_rlc_tlv_len
(
    rrc_rlc_delete_tx_rx_am_rlc_t *p_rrc_rlc_delete_tx_rx_am_rlc

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_delete_tx_rx_am_rlc != PNULL);

    length += rrc_il_get_rrc_rlc_delete_tx_rx_am_rlc_len(p_rrc_rlc_delete_tx_rx_am_rlc);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_delete_tx_rx_am_rlc
(
    U8  **pp_buffer,
    rrc_rlc_delete_tx_rx_am_rlc_t *p_rrc_rlc_delete_tx_rx_am_rlc
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_delete_tx_rx_am_rlc != PNULL);

    /* This function composes rrc_rlc_delete_tx_rx_am_rlc */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_DELETE_TX_RX_AM_RLC_ENTITY;
    tlv_length = rrc_il_get_rrc_rlc_delete_tx_rx_am_rlc_tlv_len(p_rrc_rlc_delete_tx_rx_am_rlc);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_delete_tx_rx_am_rlc->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_rlc_delete_tx_rx_am_rlc->lc_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_tx_um_rlc_len
(
    rrc_rlc_reconfig_tx_um_rlc_t *p_rrc_rlc_reconfig_tx_um_rlc
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_reconfig_tx_um_rlc != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_reconfig_tx_um_rlc->lc_id);

    /* Get length of TLV */
    if (p_rrc_rlc_reconfig_tx_um_rlc->optional_elems_present & RRC_RLC_QCI_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_qci_info_tlv_len(&p_rrc_rlc_reconfig_tx_um_rlc->rrc_rlc_qci_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_reconfig_tx_um_rlc->optional_elems_present & RRC_RLC_TX_BIT_RATE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_tx_bit_rate_info_tlv_len(&p_rrc_rlc_reconfig_tx_um_rlc->rrc_rlc_tx_bit_rate_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_reconfig_tx_um_rlc->optional_elems_present & RRC_RLC_QUEUE_SIZE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_queue_size_info_tlv_len(&p_rrc_rlc_reconfig_tx_um_rlc->rrc_rlc_queue_size_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_tx_um_rlc_tlv_len
(
    rrc_rlc_reconfig_tx_um_rlc_t *p_rrc_rlc_reconfig_tx_um_rlc

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_reconfig_tx_um_rlc != PNULL);

    length += rrc_il_get_rrc_rlc_reconfig_tx_um_rlc_len(p_rrc_rlc_reconfig_tx_um_rlc);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_reconfig_tx_um_rlc
(
    U8  **pp_buffer,
    rrc_rlc_reconfig_tx_um_rlc_t *p_rrc_rlc_reconfig_tx_um_rlc
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_reconfig_tx_um_rlc != PNULL);

    /* This function composes rrc_rlc_reconfig_tx_um_rlc */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_RECONFIG_TX_UM_RLC_ENTITY;
    tlv_length = rrc_il_get_rrc_rlc_reconfig_tx_um_rlc_tlv_len(p_rrc_rlc_reconfig_tx_um_rlc);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_reconfig_tx_um_rlc->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_rlc_reconfig_tx_um_rlc->lc_id);

    /* Compose TLV  */
    if (p_rrc_rlc_reconfig_tx_um_rlc->optional_elems_present & RRC_RLC_QCI_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_qci_info(pp_buffer, &p_rrc_rlc_reconfig_tx_um_rlc->rrc_rlc_qci_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_reconfig_tx_um_rlc->optional_elems_present & RRC_RLC_TX_BIT_RATE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_tx_bit_rate_info(pp_buffer, &p_rrc_rlc_reconfig_tx_um_rlc->rrc_rlc_tx_bit_rate_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_reconfig_tx_um_rlc->optional_elems_present & RRC_RLC_QUEUE_SIZE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_queue_size_info(pp_buffer, &p_rrc_rlc_reconfig_tx_um_rlc->rrc_rlc_queue_size_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_rx_um_rlc_len
(
    rrc_rlc_reconfig_rx_um_rlc_t *p_rrc_rlc_reconfig_rx_um_rlc
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_reconfig_rx_um_rlc != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_reconfig_rx_um_rlc->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_reconfig_rx_um_rlc->t_reordering);

    /* Get length of TLV */
    if (p_rrc_rlc_reconfig_rx_um_rlc->optional_elems_present & RRC_RLC_QCI_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_qci_info_tlv_len(&p_rrc_rlc_reconfig_rx_um_rlc->rrc_rlc_qci_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_reconfig_rx_um_rlc->optional_elems_present & RRC_RLC_RX_BIT_RATE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_rx_bit_rate_info_tlv_len(&p_rrc_rlc_reconfig_rx_um_rlc->rrc_rlc_rx_bit_rate_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_rx_um_rlc_tlv_len
(
    rrc_rlc_reconfig_rx_um_rlc_t *p_rrc_rlc_reconfig_rx_um_rlc

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_reconfig_rx_um_rlc != PNULL);

    length += rrc_il_get_rrc_rlc_reconfig_rx_um_rlc_len(p_rrc_rlc_reconfig_rx_um_rlc);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_reconfig_rx_um_rlc
(
    U8  **pp_buffer,
    rrc_rlc_reconfig_rx_um_rlc_t *p_rrc_rlc_reconfig_rx_um_rlc
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_reconfig_rx_um_rlc != PNULL);

    /* This function composes rrc_rlc_reconfig_rx_um_rlc */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_RECONFIG_RX_UM_RLC_ENTITY;
    tlv_length = rrc_il_get_rrc_rlc_reconfig_rx_um_rlc_tlv_len(p_rrc_rlc_reconfig_rx_um_rlc);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_reconfig_rx_um_rlc->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_rlc_reconfig_rx_um_rlc->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_reconfig_rx_um_rlc->t_reordering, "t_reordering");
    *pp_buffer += sizeof(p_rrc_rlc_reconfig_rx_um_rlc->t_reordering);

    /* Compose TLV  */
    if (p_rrc_rlc_reconfig_rx_um_rlc->optional_elems_present & RRC_RLC_QCI_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_qci_info(pp_buffer, &p_rrc_rlc_reconfig_rx_um_rlc->rrc_rlc_qci_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_reconfig_rx_um_rlc->optional_elems_present & RRC_RLC_RX_BIT_RATE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_rx_bit_rate_info(pp_buffer, &p_rrc_rlc_reconfig_rx_um_rlc->rrc_rlc_rx_bit_rate_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_tx_rx_um_rlc_len
(
    rrc_rlc_reconfig_tx_rx_um_rlc_t *p_rrc_rlc_reconfig_tx_rx_um_rlc
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_reconfig_tx_rx_um_rlc != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_reconfig_tx_rx_um_rlc->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_reconfig_tx_rx_um_rlc->t_reordering);

    /* Get length of TLV */
    if (p_rrc_rlc_reconfig_tx_rx_um_rlc->optional_elems_present & RRC_RLC_QCI_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_qci_info_tlv_len(&p_rrc_rlc_reconfig_tx_rx_um_rlc->rrc_rlc_qci_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_reconfig_tx_rx_um_rlc->optional_elems_present & RRC_RLC_TX_BIT_RATE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_tx_bit_rate_info_tlv_len(&p_rrc_rlc_reconfig_tx_rx_um_rlc->rrc_rlc_tx_bit_rate_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_reconfig_tx_rx_um_rlc->optional_elems_present & RRC_RLC_RX_BIT_RATE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_rx_bit_rate_info_tlv_len(&p_rrc_rlc_reconfig_tx_rx_um_rlc->rrc_rlc_rx_bit_rate_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_reconfig_tx_rx_um_rlc->optional_elems_present & RRC_RLC_QUEUE_SIZE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_queue_size_info_tlv_len(&p_rrc_rlc_reconfig_tx_rx_um_rlc->rrc_rlc_queue_size_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_tx_rx_um_rlc_tlv_len
(
    rrc_rlc_reconfig_tx_rx_um_rlc_t *p_rrc_rlc_reconfig_tx_rx_um_rlc

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_reconfig_tx_rx_um_rlc != PNULL);

    length += rrc_il_get_rrc_rlc_reconfig_tx_rx_um_rlc_len(p_rrc_rlc_reconfig_tx_rx_um_rlc);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_reconfig_tx_rx_um_rlc
(
    U8  **pp_buffer,
    rrc_rlc_reconfig_tx_rx_um_rlc_t *p_rrc_rlc_reconfig_tx_rx_um_rlc
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_reconfig_tx_rx_um_rlc != PNULL);

    /* This function composes rrc_rlc_reconfig_tx_rx_um_rlc */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_RECONFIG_TX_RX_UM_RLC_ENTITY;
    tlv_length = rrc_il_get_rrc_rlc_reconfig_tx_rx_um_rlc_tlv_len(p_rrc_rlc_reconfig_tx_rx_um_rlc);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_reconfig_tx_rx_um_rlc->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_rlc_reconfig_tx_rx_um_rlc->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_reconfig_tx_rx_um_rlc->t_reordering, "t_reordering");
    *pp_buffer += sizeof(p_rrc_rlc_reconfig_tx_rx_um_rlc->t_reordering);

    /* Compose TLV  */
    if (p_rrc_rlc_reconfig_tx_rx_um_rlc->optional_elems_present & RRC_RLC_QCI_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_qci_info(pp_buffer, &p_rrc_rlc_reconfig_tx_rx_um_rlc->rrc_rlc_qci_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_reconfig_tx_rx_um_rlc->optional_elems_present & RRC_RLC_TX_BIT_RATE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_tx_bit_rate_info(pp_buffer, &p_rrc_rlc_reconfig_tx_rx_um_rlc->rrc_rlc_tx_bit_rate_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_reconfig_tx_rx_um_rlc->optional_elems_present & RRC_RLC_RX_BIT_RATE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_rx_bit_rate_info(pp_buffer, &p_rrc_rlc_reconfig_tx_rx_um_rlc->rrc_rlc_rx_bit_rate_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_reconfig_tx_rx_um_rlc->optional_elems_present & RRC_RLC_QUEUE_SIZE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_queue_size_info(pp_buffer, &p_rrc_rlc_reconfig_tx_rx_um_rlc->rrc_rlc_queue_size_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_tx_rx_am_rlc_len
(
    rrc_rlc_reconfig_tx_rx_am_rlc_t *p_rrc_rlc_reconfig_tx_rx_am_rlc
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_reconfig_tx_rx_am_rlc != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_reconfig_tx_rx_am_rlc->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_reconfig_tx_rx_am_rlc->t_poll_retransmission);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_reconfig_tx_rx_am_rlc->poll_pdu);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_reconfig_tx_rx_am_rlc->poll_byte);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_reconfig_tx_rx_am_rlc->max_re_tx_thrsh_ld);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_reconfig_tx_rx_am_rlc->t_reordering);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_reconfig_tx_rx_am_rlc->t_status_prohibit);

    /* Get length of TLV */
    if (p_rrc_rlc_reconfig_tx_rx_am_rlc->optional_elems_present & RRC_RLC_QCI_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_qci_info_tlv_len(&p_rrc_rlc_reconfig_tx_rx_am_rlc->rrc_rlc_qci_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_reconfig_tx_rx_am_rlc->optional_elems_present & RRC_RLC_TX_BIT_RATE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_tx_bit_rate_info_tlv_len(&p_rrc_rlc_reconfig_tx_rx_am_rlc->rrc_rlc_tx_bit_rate_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_reconfig_tx_rx_am_rlc->optional_elems_present & RRC_RLC_RX_BIT_RATE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_rx_bit_rate_info_tlv_len(&p_rrc_rlc_reconfig_tx_rx_am_rlc->rrc_rlc_rx_bit_rate_info);
    }

    /* Get length of TLV */
    if (p_rrc_rlc_reconfig_tx_rx_am_rlc->optional_elems_present & RRC_RLC_QUEUE_SIZE_INFO_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_queue_size_info_tlv_len(&p_rrc_rlc_reconfig_tx_rx_am_rlc->rrc_rlc_queue_size_info);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_tx_rx_am_rlc_tlv_len
(
    rrc_rlc_reconfig_tx_rx_am_rlc_t *p_rrc_rlc_reconfig_tx_rx_am_rlc

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_reconfig_tx_rx_am_rlc != PNULL);

    length += rrc_il_get_rrc_rlc_reconfig_tx_rx_am_rlc_len(p_rrc_rlc_reconfig_tx_rx_am_rlc);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_reconfig_tx_rx_am_rlc
(
    U8  **pp_buffer,
    rrc_rlc_reconfig_tx_rx_am_rlc_t *p_rrc_rlc_reconfig_tx_rx_am_rlc
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_reconfig_tx_rx_am_rlc != PNULL);

    /* This function composes rrc_rlc_reconfig_tx_rx_am_rlc */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_RECONFIG_TX_RX_AM_RLC_ENTITY;
    tlv_length = rrc_il_get_rrc_rlc_reconfig_tx_rx_am_rlc_tlv_len(p_rrc_rlc_reconfig_tx_rx_am_rlc);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_reconfig_tx_rx_am_rlc->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_rlc_reconfig_tx_rx_am_rlc->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_reconfig_tx_rx_am_rlc->t_poll_retransmission, "t_poll_retransmission");
    *pp_buffer += sizeof(p_rrc_rlc_reconfig_tx_rx_am_rlc->t_poll_retransmission);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_reconfig_tx_rx_am_rlc->poll_pdu, "poll_pdu");
    *pp_buffer += sizeof(p_rrc_rlc_reconfig_tx_rx_am_rlc->poll_pdu);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_reconfig_tx_rx_am_rlc->poll_byte, "poll_byte");
    *pp_buffer += sizeof(p_rrc_rlc_reconfig_tx_rx_am_rlc->poll_byte);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_reconfig_tx_rx_am_rlc->max_re_tx_thrsh_ld, "max_re_tx_thrsh_ld");
    *pp_buffer += sizeof(p_rrc_rlc_reconfig_tx_rx_am_rlc->max_re_tx_thrsh_ld);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_reconfig_tx_rx_am_rlc->t_reordering, "t_reordering");
    *pp_buffer += sizeof(p_rrc_rlc_reconfig_tx_rx_am_rlc->t_reordering);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_reconfig_tx_rx_am_rlc->t_status_prohibit, "t_status_prohibit");
    *pp_buffer += sizeof(p_rrc_rlc_reconfig_tx_rx_am_rlc->t_status_prohibit);

    /* Compose TLV  */
    if (p_rrc_rlc_reconfig_tx_rx_am_rlc->optional_elems_present & RRC_RLC_QCI_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_qci_info(pp_buffer, &p_rrc_rlc_reconfig_tx_rx_am_rlc->rrc_rlc_qci_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_reconfig_tx_rx_am_rlc->optional_elems_present & RRC_RLC_TX_BIT_RATE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_tx_bit_rate_info(pp_buffer, &p_rrc_rlc_reconfig_tx_rx_am_rlc->rrc_rlc_tx_bit_rate_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_reconfig_tx_rx_am_rlc->optional_elems_present & RRC_RLC_RX_BIT_RATE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_rx_bit_rate_info(pp_buffer, &p_rrc_rlc_reconfig_tx_rx_am_rlc->rrc_rlc_rx_bit_rate_info))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_rlc_reconfig_tx_rx_am_rlc->optional_elems_present & RRC_RLC_QUEUE_SIZE_INFO_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_queue_size_info(pp_buffer, &p_rrc_rlc_reconfig_tx_rx_am_rlc->rrc_rlc_queue_size_info))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_rlc_reconfig_ue_entity_cnf_len
(
    rrc_rlc_reconfig_ue_entity_cnf_t *p_rrc_rlc_reconfig_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_reconfig_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_reconfig_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_cnf->num_create_entity_error <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_cnf->create_error_entities));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_cnf->num_create_entity_error; loop++)
        {
            length += rrc_il_get_rrc_rlc_create_entity_error_tlv_len(&p_rrc_rlc_reconfig_ue_entity_cnf->create_error_entities[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_cnf->num_delete_entity_error <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_cnf->delete_error_entities));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_cnf->num_delete_entity_error; loop++)
        {
            length += rrc_il_get_rrc_rlc_delete_entity_error_tlv_len(&p_rrc_rlc_reconfig_ue_entity_cnf->delete_error_entities[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_cnf->num_reconfig_entity_error <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_cnf->reconfig_error_entities));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_cnf->num_reconfig_entity_error; loop++)
        {
            length += rrc_il_get_rrc_rlc_reconfig_entity_error_tlv_len(&p_rrc_rlc_reconfig_ue_entity_cnf->reconfig_error_entities[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_rlc_reconfig_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_rlc_reconfig_ue_entity_cnf_t *p_rrc_rlc_reconfig_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_cnf != PNULL);

    /* This function composes rrc_rlc_reconfig_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_reconfig_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_rlc_reconfig_ue_entity_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_reconfig_ue_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_reconfig_ue_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_rlc_reconfig_ue_entity_cnf->response_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_reconfig_ue_entity_cnf->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_rlc_reconfig_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_cnf->num_create_entity_error <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_cnf->create_error_entities));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_cnf->num_create_entity_error; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_create_entity_error(pp_buffer, &p_rrc_rlc_reconfig_ue_entity_cnf->create_error_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_cnf->num_delete_entity_error <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_cnf->delete_error_entities));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_cnf->num_delete_entity_error; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_delete_entity_error(pp_buffer, &p_rrc_rlc_reconfig_ue_entity_cnf->delete_error_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_cnf->num_reconfig_entity_error <= ARRSIZE(p_rrc_rlc_reconfig_ue_entity_cnf->reconfig_error_entities));
        for (loop = 0; loop < p_rrc_rlc_reconfig_ue_entity_cnf->num_reconfig_entity_error; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_reconfig_entity_error(pp_buffer, &p_rrc_rlc_reconfig_ue_entity_cnf->reconfig_error_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rrc_rlc_reconfig_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_RLC_RECONFIG_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rrc_rlc_reconfig_ue_entity_cnf
(
    rrc_rlc_reconfig_ue_entity_cnf_t  *p_rrc_rlc_reconfig_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rrc_rlc_reconfig_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_rlc_reconfig_ue_entity_cnf_len(p_rrc_rlc_reconfig_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_RLC_RECONFIG_UE_ENTITY_CNF", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_RLC_RECONFIG_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_RLC_RECONFIG_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_RLC_RECONFIG_UE_ENTITY_CNF message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_rlc_reconfig_ue_entity_cnf(&p_rlc_msg, p_rrc_rlc_reconfig_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
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
rrc_il_get_rrc_rlc_delete_entity_error_len
(
    rrc_rlc_delete_entity_error_t *p_rrc_rlc_delete_entity_error
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_delete_entity_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_delete_entity_error->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_delete_entity_error->response);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_delete_entity_error_tlv_len
(
    rrc_rlc_delete_entity_error_t *p_rrc_rlc_delete_entity_error

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_delete_entity_error != PNULL);

    length += rrc_il_get_rrc_rlc_delete_entity_error_len(p_rrc_rlc_delete_entity_error);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_delete_entity_error
(
    U8  **pp_buffer,
    rrc_rlc_delete_entity_error_t *p_rrc_rlc_delete_entity_error
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_delete_entity_error != PNULL);

    /* This function composes rrc_rlc_delete_entity_error */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_DELETE_ENTITY_ERROR;
    tlv_length = rrc_il_get_rrc_rlc_delete_entity_error_tlv_len(p_rrc_rlc_delete_entity_error);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_delete_entity_error->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_rlc_delete_entity_error->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_delete_entity_error->response, "response");
    *pp_buffer += sizeof(p_rrc_rlc_delete_entity_error->response);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_entity_error_len
(
    rrc_rlc_reconfig_entity_error_t *p_rrc_rlc_reconfig_entity_error
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_reconfig_entity_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_reconfig_entity_error->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_reconfig_entity_error->response);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_reconfig_entity_error_tlv_len
(
    rrc_rlc_reconfig_entity_error_t *p_rrc_rlc_reconfig_entity_error

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_reconfig_entity_error != PNULL);

    length += rrc_il_get_rrc_rlc_reconfig_entity_error_len(p_rrc_rlc_reconfig_entity_error);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_reconfig_entity_error
(
    U8  **pp_buffer,
    rrc_rlc_reconfig_entity_error_t *p_rrc_rlc_reconfig_entity_error
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_reconfig_entity_error != PNULL);

    /* This function composes rrc_rlc_reconfig_entity_error */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_RECONFIG_ENTITY_ERROR;
    tlv_length = rrc_il_get_rrc_rlc_reconfig_entity_error_tlv_len(p_rrc_rlc_reconfig_entity_error);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_reconfig_entity_error->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_rlc_reconfig_entity_error->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_reconfig_entity_error->response, "response");
    *pp_buffer += sizeof(p_rrc_rlc_reconfig_entity_error->response);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_rlc_delete_ue_entity_req_len
(
    rrc_rlc_delete_ue_entity_req_t *p_rrc_rlc_delete_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_delete_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_delete_ue_entity_req->ue_index);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_rlc_delete_ue_entity_req
(
    U8  **pp_buffer,
    rrc_rlc_delete_ue_entity_req_t *p_rrc_rlc_delete_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_delete_ue_entity_req != PNULL);

    /* This function composes rrc_rlc_delete_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_delete_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_rlc_delete_ue_entity_req->ue_index);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rrc_rlc_delete_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_RLC_DELETE_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rrc_rlc_delete_ue_entity_req
(
    rrc_rlc_delete_ue_entity_req_t  *p_rrc_rlc_delete_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rrc_rlc_delete_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_rlc_delete_ue_entity_req_len(p_rrc_rlc_delete_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_RLC_DELETE_UE_ENTITY_REQ", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_RLC_DELETE_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_RLC_DELETE_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_RLC_DELETE_UE_ENTITY_REQ message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_rlc_delete_ue_entity_req(&p_rlc_msg, p_rrc_rlc_delete_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_rlc_delete_ue_entity_cnf_len
(
    rrc_rlc_delete_ue_entity_cnf_t *p_rrc_rlc_delete_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_delete_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_delete_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_delete_ue_entity_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_rlc_delete_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_rlc_delete_ue_entity_cnf_t *p_rrc_rlc_delete_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_delete_ue_entity_cnf != PNULL);

    /* This function composes rrc_rlc_delete_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_delete_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_rlc_delete_ue_entity_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_delete_ue_entity_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_delete_ue_entity_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_rlc_delete_ue_entity_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_delete_ue_entity_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_rlc_delete_ue_entity_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rrc_rlc_delete_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_RLC_DELETE_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rrc_rlc_delete_ue_entity_cnf
(
    rrc_rlc_delete_ue_entity_cnf_t  *p_rrc_rlc_delete_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rrc_rlc_delete_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_rlc_delete_ue_entity_cnf_len(p_rrc_rlc_delete_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_RLC_DELETE_UE_ENTITY_CNF", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_RLC_DELETE_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_RLC_DELETE_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_RLC_DELETE_UE_ENTITY_CNF message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_rlc_delete_ue_entity_cnf(&p_rlc_msg, p_rrc_rlc_delete_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_rlc_re_establish_ue_entity_req_len
(
    rrc_rlc_re_establish_ue_entity_req_t *p_rrc_rlc_re_establish_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_re_establish_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_re_establish_ue_entity_req->ue_index);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_re_establish_ue_entity_req->num_entity_lc_id <= ARRSIZE(p_rrc_rlc_re_establish_ue_entity_req->entity_lcids));
        for (loop = 0; loop < p_rrc_rlc_re_establish_ue_entity_req->num_entity_lc_id; loop++)
        {
            length += rrc_il_get_rrc_rlc_entity_lcid_tlv_len(&p_rrc_rlc_re_establish_ue_entity_req->entity_lcids[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_rlc_re_establish_ue_entity_req
(
    U8  **pp_buffer,
    rrc_rlc_re_establish_ue_entity_req_t *p_rrc_rlc_re_establish_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_re_establish_ue_entity_req != PNULL);

    /* This function composes rrc_rlc_re_establish_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_re_establish_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_rlc_re_establish_ue_entity_req->ue_index);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_re_establish_ue_entity_req->num_entity_lc_id <= ARRSIZE(p_rrc_rlc_re_establish_ue_entity_req->entity_lcids));
        for (loop = 0; loop < p_rrc_rlc_re_establish_ue_entity_req->num_entity_lc_id; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_entity_lcid(pp_buffer, &p_rrc_rlc_re_establish_ue_entity_req->entity_lcids[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rrc_rlc_re_establish_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_RLC_RE_ESTABLISH_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rrc_rlc_re_establish_ue_entity_req
(
    rrc_rlc_re_establish_ue_entity_req_t  *p_rrc_rlc_re_establish_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rrc_rlc_re_establish_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_rlc_re_establish_ue_entity_req_len(p_rrc_rlc_re_establish_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_RLC_RE_ESTABLISH_UE_ENTITY_REQ", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_RLC_RE_ESTABLISH_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_RLC_RE_ESTABLISH_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_RLC_RE_ESTABLISH_UE_ENTITY_REQ message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_rlc_re_establish_ue_entity_req(&p_rlc_msg, p_rrc_rlc_re_establish_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
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
rrc_il_get_rrc_rlc_lc_state_len
(
    rrc_rlc_lc_state_t *p_rrc_rlc_lc_state
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_lc_state != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_lc_state->state);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_lc_state_tlv_len
(
    rrc_rlc_lc_state_t *p_rrc_rlc_lc_state

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_lc_state != PNULL);

    length += rrc_il_get_rrc_rlc_lc_state_len(p_rrc_rlc_lc_state);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_lc_state
(
    U8  **pp_buffer,
    rrc_rlc_lc_state_t *p_rrc_rlc_lc_state
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_lc_state != PNULL);

    /* This function composes rrc_rlc_lc_state */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_LC_STATE;
    tlv_length = rrc_il_get_rrc_rlc_lc_state_tlv_len(p_rrc_rlc_lc_state);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_lc_state->state > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_lc_state->state] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_rlc_lc_state->state);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_lc_state->state, "state");
    *pp_buffer += sizeof(p_rrc_rlc_lc_state->state);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_entity_lcid_len
(
    rrc_rlc_entity_lcid_t *p_rrc_rlc_entity_lcid
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_entity_lcid != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_entity_lcid->lc_id);

    /* Get length of TLV */
    if (p_rrc_rlc_entity_lcid->bitmask & RRC_RLC_LC_STATE_PRESENT)
    {
        length += rrc_il_get_rrc_rlc_lc_state_tlv_len(&p_rrc_rlc_entity_lcid->lc_state);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_entity_lcid_tlv_len
(
    rrc_rlc_entity_lcid_t *p_rrc_rlc_entity_lcid

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_entity_lcid != PNULL);

    length += rrc_il_get_rrc_rlc_entity_lcid_len(p_rrc_rlc_entity_lcid);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_entity_lcid
(
    U8  **pp_buffer,
    rrc_rlc_entity_lcid_t *p_rrc_rlc_entity_lcid
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_entity_lcid != PNULL);

    /* This function composes rrc_rlc_entity_lcid */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_ENTITY_LCID;
    tlv_length = rrc_il_get_rrc_rlc_entity_lcid_tlv_len(p_rrc_rlc_entity_lcid);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_entity_lcid->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_rlc_entity_lcid->lc_id);

    /* Compose TLV  */
    if (p_rrc_rlc_entity_lcid->bitmask & RRC_RLC_LC_STATE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_rlc_lc_state(pp_buffer, &p_rrc_rlc_entity_lcid->lc_state))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_rlc_re_establish_ue_entity_cnf_len
(
    rrc_rlc_re_establish_ue_entity_cnf_t *p_rrc_rlc_re_establish_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_re_establish_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_re_establish_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_re_establish_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_re_establish_ue_entity_cnf->num_error_entity <= ARRSIZE(p_rrc_rlc_re_establish_ue_entity_cnf->error_entities));
        for (loop = 0; loop < p_rrc_rlc_re_establish_ue_entity_cnf->num_error_entity; loop++)
        {
            length += rrc_il_get_rrc_rlc_entity_lc_id_error_tlv_len(&p_rrc_rlc_re_establish_ue_entity_cnf->error_entities[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_rlc_re_establish_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_rlc_re_establish_ue_entity_cnf_t *p_rrc_rlc_re_establish_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_re_establish_ue_entity_cnf != PNULL);

    /* This function composes rrc_rlc_re_establish_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_re_establish_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_rlc_re_establish_ue_entity_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_re_establish_ue_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_re_establish_ue_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_rlc_re_establish_ue_entity_cnf->response_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_re_establish_ue_entity_cnf->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_rlc_re_establish_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_re_establish_ue_entity_cnf->num_error_entity <= ARRSIZE(p_rrc_rlc_re_establish_ue_entity_cnf->error_entities));
        for (loop = 0; loop < p_rrc_rlc_re_establish_ue_entity_cnf->num_error_entity; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_entity_lc_id_error(pp_buffer, &p_rrc_rlc_re_establish_ue_entity_cnf->error_entities[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rrc_rlc_re_establish_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_RLC_RE_ESTABLISH_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rrc_rlc_re_establish_ue_entity_cnf
(
    rrc_rlc_re_establish_ue_entity_cnf_t  *p_rrc_rlc_re_establish_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rrc_rlc_re_establish_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_rlc_re_establish_ue_entity_cnf_len(p_rrc_rlc_re_establish_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_RLC_RE_ESTABLISH_UE_ENTITY_CNF", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_RLC_RE_ESTABLISH_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_RLC_RE_ESTABLISH_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_RLC_RE_ESTABLISH_UE_ENTITY_CNF message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_rlc_re_establish_ue_entity_cnf(&p_rlc_msg, p_rrc_rlc_re_establish_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
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
rrc_il_get_rrc_rlc_entity_lc_id_error_len
(
    rrc_rlc_entity_lc_id_error_t *p_rrc_rlc_entity_lc_id_error
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_entity_lc_id_error != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_entity_lc_id_error->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_entity_lc_id_error->response);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_rlc_entity_lc_id_error_tlv_len
(
    rrc_rlc_entity_lc_id_error_t *p_rrc_rlc_entity_lc_id_error

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_entity_lc_id_error != PNULL);

    length += rrc_il_get_rrc_rlc_entity_lc_id_error_len(p_rrc_rlc_entity_lc_id_error);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_rlc_entity_lc_id_error
(
    U8  **pp_buffer,
    rrc_rlc_entity_lc_id_error_t *p_rrc_rlc_entity_lc_id_error
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_entity_lc_id_error != PNULL);

    /* This function composes rrc_rlc_entity_lc_id_error */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_RLC_ENTITY_LC_ID_ERROR;
    tlv_length = rrc_il_get_rrc_rlc_entity_lc_id_error_tlv_len(p_rrc_rlc_entity_lc_id_error);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_entity_lc_id_error->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_rlc_entity_lc_id_error->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_entity_lc_id_error->response, "response");
    *pp_buffer += sizeof(p_rrc_rlc_entity_lc_id_error->response);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rlc_ue_entity_error_ind_len
(
    rlc_ue_entity_error_ind_t *p_rlc_ue_entity_error_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rlc_ue_entity_error_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rlc_ue_entity_error_ind->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rlc_ue_entity_error_ind->lc_id);

    return length;
}

rrc_return_et
rrc_il_compose_rlc_ue_entity_error_ind
(
    U8  **pp_buffer,
    rlc_ue_entity_error_ind_t *p_rlc_ue_entity_error_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rlc_ue_entity_error_ind != PNULL);

    /* This function composes rlc_ue_entity_error_ind */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rlc_ue_entity_error_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rlc_ue_entity_error_ind->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rlc_ue_entity_error_ind->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rlc_ue_entity_error_ind->lc_id);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rlc_ue_entity_error_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_RLC_UE_ENTITY_ERROR_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rlc_ue_entity_error_ind
(
    rlc_ue_entity_error_ind_t  *p_rlc_ue_entity_error_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rlc_ue_entity_error_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rlc_ue_entity_error_ind_len(p_rlc_ue_entity_error_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_RLC_UE_ENTITY_ERROR_IND", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_RLC_UE_ENTITY_ERROR_IND, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_RLC_UE_ENTITY_ERROR_IND, msg_length,cell_index);

    /* Fill RRC_RLC_UE_ENTITY_ERROR_IND message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rlc_ue_entity_error_ind(&p_rlc_msg, p_rlc_ue_entity_error_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_rlc_change_crnti_req_len
(
    rrc_rlc_change_crnti_req_t *p_rrc_rlc_change_crnti_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_change_crnti_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_change_crnti_req->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_change_crnti_req->old_crnti);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_change_crnti_req->new_crnti);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_rlc_change_crnti_req
(
    U8  **pp_buffer,
    rrc_rlc_change_crnti_req_t *p_rrc_rlc_change_crnti_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_change_crnti_req != PNULL);

    /* This function composes rrc_rlc_change_crnti_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_change_crnti_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_rlc_change_crnti_req->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_change_crnti_req->old_crnti, "old_crnti");
    *pp_buffer += sizeof(p_rrc_rlc_change_crnti_req->old_crnti);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_change_crnti_req->new_crnti, "new_crnti");
    *pp_buffer += sizeof(p_rrc_rlc_change_crnti_req->new_crnti);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rrc_rlc_change_crnti_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_RLC_CHANGE_CRNTI_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rrc_rlc_change_crnti_req
(
    rrc_rlc_change_crnti_req_t  *p_rrc_rlc_change_crnti_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rrc_rlc_change_crnti_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_rlc_change_crnti_req_len(p_rrc_rlc_change_crnti_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_RLC_CHANGE_CRNTI_REQ", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_RLC_CHANGE_CRNTI_REQ, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_RLC_CHANGE_CRNTI_REQ, msg_length,cell_index);

    /* Fill RRC_RLC_CHANGE_CRNTI_REQ message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_rlc_change_crnti_req(&p_rlc_msg, p_rrc_rlc_change_crnti_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_rlc_change_crnti_cnf_len
(
    rrc_rlc_change_crnti_cnf_t *p_rrc_rlc_change_crnti_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_change_crnti_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_change_crnti_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_change_crnti_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_rlc_change_crnti_cnf
(
    U8  **pp_buffer,
    rrc_rlc_change_crnti_cnf_t *p_rrc_rlc_change_crnti_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_change_crnti_cnf != PNULL);

    /* This function composes rrc_rlc_change_crnti_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_change_crnti_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_rlc_change_crnti_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_change_crnti_cnf->response > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_change_crnti_cnf->response] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_rlc_change_crnti_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_change_crnti_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_rlc_change_crnti_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rrc_rlc_change_crnti_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_RLC_CHANGE_CRNTI_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rrc_rlc_change_crnti_cnf
(
    rrc_rlc_change_crnti_cnf_t  *p_rrc_rlc_change_crnti_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rrc_rlc_change_crnti_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_rlc_change_crnti_cnf_len(p_rrc_rlc_change_crnti_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_RLC_CHANGE_CRNTI_CNF", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_RLC_CHANGE_CRNTI_CNF, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_RLC_CHANGE_CRNTI_CNF, msg_length,cell_index);

    /* Fill RRC_RLC_CHANGE_CRNTI_CNF message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_rlc_change_crnti_cnf(&p_rlc_msg, p_rrc_rlc_change_crnti_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_rlc_re_establish_ue_entity_complete_ind_len
(
    rrc_rlc_re_establish_ue_entity_complete_ind_t *p_rrc_rlc_re_establish_ue_entity_complete_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_re_establish_ue_entity_complete_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_re_establish_ue_entity_complete_ind->ue_index);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_re_establish_ue_entity_complete_ind->num_entity_lc_id <= ARRSIZE(p_rrc_rlc_re_establish_ue_entity_complete_ind->entity_lcids));
        for (loop = 0; loop < p_rrc_rlc_re_establish_ue_entity_complete_ind->num_entity_lc_id; loop++)
        {
            length += rrc_il_get_rrc_rlc_entity_lcid_tlv_len(&p_rrc_rlc_re_establish_ue_entity_complete_ind->entity_lcids[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_rlc_re_establish_ue_entity_complete_ind
(
    U8  **pp_buffer,
    rrc_rlc_re_establish_ue_entity_complete_ind_t *p_rrc_rlc_re_establish_ue_entity_complete_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_re_establish_ue_entity_complete_ind != PNULL);

    /* This function composes rrc_rlc_re_establish_ue_entity_complete_ind */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_re_establish_ue_entity_complete_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_rlc_re_establish_ue_entity_complete_ind->ue_index);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_rlc_re_establish_ue_entity_complete_ind->num_entity_lc_id <= ARRSIZE(p_rrc_rlc_re_establish_ue_entity_complete_ind->entity_lcids));
        for (loop = 0; loop < p_rrc_rlc_re_establish_ue_entity_complete_ind->num_entity_lc_id; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_rlc_entity_lcid(pp_buffer, &p_rrc_rlc_re_establish_ue_entity_complete_ind->entity_lcids[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rrc_rlc_re_establish_ue_entity_complete_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rrc_rlc_re_establish_ue_entity_complete_ind
(
    rrc_rlc_re_establish_ue_entity_complete_ind_t  *p_rrc_rlc_re_establish_ue_entity_complete_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rrc_rlc_re_establish_ue_entity_complete_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_rlc_re_establish_ue_entity_complete_ind_len(p_rrc_rlc_re_establish_ue_entity_complete_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND, msg_length,cell_index);

    /* Fill RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_rlc_re_establish_ue_entity_complete_ind(&p_rlc_msg, p_rrc_rlc_re_establish_ue_entity_complete_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_rlc_re_establish_ue_entity_complete_cnf_len
(
    rrc_rlc_re_establish_ue_entity_complete_cnf_t *p_rrc_rlc_re_establish_ue_entity_complete_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_re_establish_ue_entity_complete_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_re_establish_ue_entity_complete_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_re_establish_ue_entity_complete_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_rlc_re_establish_ue_entity_complete_cnf
(
    U8  **pp_buffer,
    rrc_rlc_re_establish_ue_entity_complete_cnf_t *p_rrc_rlc_re_establish_ue_entity_complete_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_re_establish_ue_entity_complete_cnf != PNULL);

    /* This function composes rrc_rlc_re_establish_ue_entity_complete_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_re_establish_ue_entity_complete_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_rlc_re_establish_ue_entity_complete_cnf->ue_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_re_establish_ue_entity_complete_cnf->response > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_re_establish_ue_entity_complete_cnf->response] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_rlc_re_establish_ue_entity_complete_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_re_establish_ue_entity_complete_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_rlc_re_establish_ue_entity_complete_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rrc_rlc_re_establish_ue_entity_complete_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rrc_rlc_re_establish_ue_entity_complete_cnf
(
    rrc_rlc_re_establish_ue_entity_complete_cnf_t  *p_rrc_rlc_re_establish_ue_entity_complete_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rrc_rlc_re_establish_ue_entity_complete_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_rlc_re_establish_ue_entity_complete_cnf_len(p_rrc_rlc_re_establish_ue_entity_complete_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_CNF", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_CNF, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_CNF, msg_length,cell_index);

    /* Fill RRC_RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_CNF message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_rlc_re_establish_ue_entity_complete_cnf(&p_rlc_msg, p_rrc_rlc_re_establish_ue_entity_complete_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_rlc_create_mbms_area_entity_req_len
(
    rrc_rlc_create_mbms_area_entity_req_t *p_rrc_rlc_create_mbms_area_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_create_mbms_area_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_mbms_area_entity_req->num_of_area);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrc_rlc_create_mbms_area_entity_req->num_of_area * sizeof(p_rrc_rlc_create_mbms_area_entity_req->area_id[0]));

    return length;
}

rrc_return_et
rrc_il_compose_rrc_rlc_create_mbms_area_entity_req
(
    U8  **pp_buffer,
    rrc_rlc_create_mbms_area_entity_req_t *p_rrc_rlc_create_mbms_area_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_create_mbms_area_entity_req != PNULL);

    /* This function composes rrc_rlc_create_mbms_area_entity_req */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_rlc_create_mbms_area_entity_req->num_of_area < 1) || (p_rrc_rlc_create_mbms_area_entity_req->num_of_area > 16))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_create_mbms_area_entity_req->num_of_area] should be in range "
            "1 to 16. Incorrect value %u received.", p_rrc_rlc_create_mbms_area_entity_req->num_of_area);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_create_mbms_area_entity_req->num_of_area, "num_of_area");
    *pp_buffer += sizeof(p_rrc_rlc_create_mbms_area_entity_req->num_of_area);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_rlc_create_mbms_area_entity_req->num_of_area; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_create_mbms_area_entity_req->area_id[loop], "area_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rrc_rlc_create_mbms_area_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_M2AP_RLC_CREATE_MBMS_AREA_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rrc_rlc_create_mbms_area_entity_req
(
    rrc_rlc_create_mbms_area_entity_req_t  *p_rrc_rlc_create_mbms_area_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rrc_rlc_create_mbms_area_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_rlc_create_mbms_area_entity_req_len(p_rrc_rlc_create_mbms_area_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_M2AP_RLC_CREATE_MBMS_AREA_ENTITY_REQ", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_M2AP_RLC_CREATE_MBMS_AREA_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_M2AP_RLC_CREATE_MBMS_AREA_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_M2AP_RLC_CREATE_MBMS_AREA_ENTITY_REQ message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_rlc_create_mbms_area_entity_req(&p_rlc_msg, p_rrc_rlc_create_mbms_area_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_rlc_create_mbms_area_entity_cnf_len
(
    rrc_rlc_create_mbms_area_entity_cnf_t *p_rrc_rlc_create_mbms_area_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_create_mbms_area_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_create_mbms_area_entity_cnf->response_code);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_rlc_create_mbms_area_entity_cnf
(
    U8  **pp_buffer,
    rrc_rlc_create_mbms_area_entity_cnf_t *p_rrc_rlc_create_mbms_area_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_create_mbms_area_entity_cnf != PNULL);

    /* This function composes rrc_rlc_create_mbms_area_entity_cnf */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_create_mbms_area_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_create_mbms_area_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_rlc_create_mbms_area_entity_cnf->response_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_create_mbms_area_entity_cnf->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_rlc_create_mbms_area_entity_cnf->response_code);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rrc_rlc_create_mbms_area_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_M2AP_RLC_CREATE_MBMS_AREA_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rrc_rlc_create_mbms_area_entity_cnf
(
    rrc_rlc_create_mbms_area_entity_cnf_t  *p_rrc_rlc_create_mbms_area_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rrc_rlc_create_mbms_area_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_rlc_create_mbms_area_entity_cnf_len(p_rrc_rlc_create_mbms_area_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_M2AP_RLC_CREATE_MBMS_AREA_ENTITY_CNF", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_M2AP_RLC_CREATE_MBMS_AREA_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_M2AP_RLC_CREATE_MBMS_AREA_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_M2AP_RLC_CREATE_MBMS_AREA_ENTITY_CNF message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_rlc_create_mbms_area_entity_cnf(&p_rlc_msg, p_rrc_rlc_create_mbms_area_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_rlc_delete_mbms_area_entity_req_len
(
    rrc_rlc_delete_mbms_area_entity_req_t *p_rrc_rlc_delete_mbms_area_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_delete_mbms_area_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_delete_mbms_area_entity_req->num_of_area);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_rrc_rlc_delete_mbms_area_entity_req->num_of_area * sizeof(p_rrc_rlc_delete_mbms_area_entity_req->area_id[0]));

    return length;
}

rrc_return_et
rrc_il_compose_rrc_rlc_delete_mbms_area_entity_req
(
    U8  **pp_buffer,
    rrc_rlc_delete_mbms_area_entity_req_t *p_rrc_rlc_delete_mbms_area_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_delete_mbms_area_entity_req != PNULL);

    /* This function composes rrc_rlc_delete_mbms_area_entity_req */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_rlc_delete_mbms_area_entity_req->num_of_area < 1) || (p_rrc_rlc_delete_mbms_area_entity_req->num_of_area > 16))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_delete_mbms_area_entity_req->num_of_area] should be in range "
            "1 to 16. Incorrect value %u received.", p_rrc_rlc_delete_mbms_area_entity_req->num_of_area);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_delete_mbms_area_entity_req->num_of_area, "num_of_area");
    *pp_buffer += sizeof(p_rrc_rlc_delete_mbms_area_entity_req->num_of_area);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_rlc_delete_mbms_area_entity_req->num_of_area; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_delete_mbms_area_entity_req->area_id[loop], "area_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rrc_rlc_delete_mbms_area_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_M2AP_RLC_DELETE_MBMS_AREA_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rrc_rlc_delete_mbms_area_entity_req
(
    rrc_rlc_delete_mbms_area_entity_req_t  *p_rrc_rlc_delete_mbms_area_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rrc_rlc_delete_mbms_area_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_rlc_delete_mbms_area_entity_req_len(p_rrc_rlc_delete_mbms_area_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_M2AP_RLC_DELETE_MBMS_AREA_ENTITY_REQ", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_M2AP_RLC_DELETE_MBMS_AREA_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_M2AP_RLC_DELETE_MBMS_AREA_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_M2AP_RLC_DELETE_MBMS_AREA_ENTITY_REQ message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_rlc_delete_mbms_area_entity_req(&p_rlc_msg, p_rrc_rlc_delete_mbms_area_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_rlc_delete_mbms_area_entity_cnf_len
(
    rrc_rlc_delete_mbms_area_entity_cnf_t *p_rrc_rlc_delete_mbms_area_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_delete_mbms_area_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_delete_mbms_area_entity_cnf->response_code);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_rlc_delete_mbms_area_entity_cnf
(
    U8  **pp_buffer,
    rrc_rlc_delete_mbms_area_entity_cnf_t *p_rrc_rlc_delete_mbms_area_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_delete_mbms_area_entity_cnf != PNULL);

    /* This function composes rrc_rlc_delete_mbms_area_entity_cnf */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_delete_mbms_area_entity_cnf->response_code > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_delete_mbms_area_entity_cnf->response_code] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_rlc_delete_mbms_area_entity_cnf->response_code);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_delete_mbms_area_entity_cnf->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_rlc_delete_mbms_area_entity_cnf->response_code);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rrc_rlc_delete_mbms_area_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_M2AP_RLC_DELETE_MBMS_AREA_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rrc_rlc_delete_mbms_area_entity_cnf
(
    rrc_rlc_delete_mbms_area_entity_cnf_t  *p_rrc_rlc_delete_mbms_area_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rrc_rlc_delete_mbms_area_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_rlc_delete_mbms_area_entity_cnf_len(p_rrc_rlc_delete_mbms_area_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_M2AP_RLC_DELETE_MBMS_AREA_ENTITY_CNF", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_M2AP_RLC_DELETE_MBMS_AREA_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_M2AP_RLC_DELETE_MBMS_AREA_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_M2AP_RLC_DELETE_MBMS_AREA_ENTITY_CNF message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_rlc_delete_mbms_area_entity_cnf(&p_rlc_msg, p_rrc_rlc_delete_mbms_area_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_rlc_config_cell_req_len
(
    rrc_rlc_config_cell_req_t *p_rrc_rlc_config_cell_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_config_cell_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_config_cell_req->Cell_index);

    /* Get length of IE */
    length += rrc_il_get_rlc_communication_info_len(&p_rrc_rlc_config_cell_req->rlc_communication_info);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_rlc_config_cell_req
(
    U8  **pp_buffer,
    rrc_rlc_config_cell_req_t *p_rrc_rlc_config_cell_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_config_cell_req != PNULL);

    /* This function composes rrc_rlc_config_cell_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_config_cell_req->Cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_config_cell_req->Cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_rlc_config_cell_req->Cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_rlc_config_cell_req->Cell_index, "Cell_index");
    *pp_buffer += sizeof(p_rrc_rlc_config_cell_req->Cell_index);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rlc_communication_info(pp_buffer, &p_rrc_rlc_config_cell_req->rlc_communication_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rrc_rlc_config_cell_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_RLC_CELL_CONFIG_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rrc_rlc_config_cell_req
(
    rrc_rlc_config_cell_req_t  *p_rrc_rlc_config_cell_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rrc_rlc_config_cell_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_rlc_config_cell_req_len(p_rrc_rlc_config_cell_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_RLC_CELL_CONFIG_REQ", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_RLC_CELL_CONFIG_REQ, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_RLC_CELL_CONFIG_REQ, msg_length,cell_index);

    /* Fill RRC_RLC_CELL_CONFIG_REQ message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_rlc_config_cell_req(&p_rlc_msg, p_rrc_rlc_config_cell_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
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
rrc_il_get_rlc_communication_info_len
(
    rlc_communication_info_t *p_rlc_communication_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rlc_communication_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rlc_communication_info->pdcpInstance);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rlc_communication_info->pdcpRxPort);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_rlc_communication_info->pdcpIpAddr);

    return length;
}

static
rrc_return_et
rrc_il_compose_rlc_communication_info
(
    U8  **pp_buffer,
    rlc_communication_info_t *p_rlc_communication_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rlc_communication_info != PNULL);

    /* This function composes rlc_communication_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rlc_communication_info->pdcpInstance, "pdcpInstance");
    *pp_buffer += sizeof(p_rlc_communication_info->pdcpInstance);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rlc_communication_info->pdcpRxPort, "pdcpRxPort");
    *pp_buffer += sizeof(p_rlc_communication_info->pdcpRxPort);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_rlc_communication_info->pdcpIpAddr); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rlc_communication_info->pdcpIpAddr[loop], "pdcpIpAddr[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_rlc_config_cell_cnf_len
(
    rrc_rlc_config_cell_cnf_t *p_rrc_rlc_config_cell_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_rlc_config_cell_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_config_cell_cnf->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_rlc_config_cell_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_rlc_config_cell_cnf
(
    U8  **pp_buffer,
    rrc_rlc_config_cell_cnf_t *p_rrc_rlc_config_cell_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_rlc_config_cell_cnf != PNULL);

    /* This function composes rrc_rlc_config_cell_cnf */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_config_cell_cnf->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_config_cell_cnf->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_rlc_config_cell_cnf->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_config_cell_cnf->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_rlc_config_cell_cnf->cell_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_rlc_config_cell_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_rlc_config_cell_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_rlc_config_cell_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_rlc_config_cell_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_rlc_config_cell_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_rlc_il_send_rrc_rlc_config_cell_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_RLC_CELL_CONFIG_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_rlc_il_send_rrc_rlc_config_cell_cnf
(
    rrc_rlc_config_cell_cnf_t  *p_rrc_rlc_config_cell_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_rlc_msg;

    RRC_ASSERT(p_rrc_rlc_config_cell_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_rlc_config_cell_cnf_len(p_rrc_rlc_config_cell_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_RLC_CELL_CONFIG_CNF", src_module_id, dst_module_id);

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
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_rlc_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_RLC_CELL_CONFIG_CNF, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_rlc_msg, transaction_id, src_module_id,
        RRC_RLC_MODULE_ID, RRC_RLC_CELL_CONFIG_CNF, msg_length,cell_index);

    /* Fill RRC_RLC_CELL_CONFIG_CNF message */
    p_rlc_msg = p_rlc_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_rlc_config_cell_cnf(&p_rlc_msg, p_rrc_rlc_config_cell_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_rlc_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

