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
 *  File Description : The file rrc_x2apCommon_il_composer.c contains the definitions 
 *                     of rrc-x2apCommon interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#include "rrc_cp_common.h"
#include "rrc_x2apCommon_il_composer.h"
#include "rrc_x2apCommon_intf.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"
#include "rrc_msg_mgmt.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_length_t
rrc_get_x2ap_plmn_identity_len
(
    x2ap_plmn_identity_t *p_x2ap_plmn_identity
);

static
rrc_return_et
rrc_compose_x2ap_plmn_identity
(
    U8  **pp_buffer,
    x2ap_plmn_identity_t *p_x2ap_plmn_identity
);

static
rrc_length_t
rrc_get_macro_enb_id_len
(
    macro_enb_id_t *p_macro_enb_id
);

static
rrc_return_et
rrc_compose_macro_enb_id
(
    U8  **pp_buffer,
    macro_enb_id_t *p_macro_enb_id
);

static
rrc_length_t
rrc_get_home_enb_id_len
(
    home_enb_id_t *p_home_enb_id
);

static
rrc_return_et
rrc_compose_home_enb_id
(
    U8  **pp_buffer,
    home_enb_id_t *p_home_enb_id
);

static
rrc_length_t
rrc_get_x2_enb_id_info_len
(
    x2_enb_id_info_t *p_x2_enb_id_info
);

static
rrc_return_et
rrc_compose_x2_enb_id_info
(
    U8  **pp_buffer,
    x2_enb_id_info_t *p_x2_enb_id_info
);

static
rrc_length_t
rrc_get_x2_gb_enb_id_len
(
    x2_gb_enb_id_t *p_x2_gb_enb_id
);

static
rrc_return_et
rrc_compose_x2_gb_enb_id
(
    U8  **pp_buffer,
    x2_gb_enb_id_t *p_x2_gb_enb_id
);

static
rrc_length_t
rrc_get_x2ap_broadcast_plmn_len
(
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn
);

static
rrc_return_et
rrc_compose_x2ap_broadcast_plmn
(
    U8  **pp_buffer,
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn
);

static
rrc_length_t
rrc_get_x2ap_earfcn_len
(
    x2ap_earfcn_t *p_x2ap_earfcn
);

static
rrc_return_et
rrc_compose_x2ap_earfcn
(
    U8  **pp_buffer,
    x2ap_earfcn_t *p_x2ap_earfcn
);

static
rrc_length_t
rrc_get_x2ap_fdd_info_len
(
    x2ap_fdd_info_t *p_x2ap_fdd_info
);

static
rrc_return_et
rrc_compose_x2ap_fdd_info
(
    U8  **pp_buffer,
    x2ap_fdd_info_t *p_x2ap_fdd_info
);

static
rrc_length_t
rrc_get_x2ap_special_subframe_info_len
(
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info
);

static
rrc_return_et
rrc_compose_x2ap_special_subframe_info
(
    U8  **pp_buffer,
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info
);

static
rrc_length_t
rrc_get_x2ap_tdd_info_len
(
    x2ap_tdd_info_t *p_x2ap_tdd_info
);

static
rrc_return_et
rrc_compose_x2ap_tdd_info
(
    U8  **pp_buffer,
    x2ap_tdd_info_t *p_x2ap_tdd_info
);

static
rrc_length_t
rrc_get_x2ap_choice_eutra_mode_len
(
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode
);

static
rrc_return_et
rrc_compose_x2ap_choice_eutra_mode
(
    U8  **pp_buffer,
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode
);

static
rrc_length_t
rrc_get_x2ap_choice_eutra_mode_info_len
(
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info
);

static
rrc_return_et
rrc_compose_x2ap_choice_eutra_mode_info
(
    U8  **pp_buffer,
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info
);

static
rrc_length_t
rrc_get_x2ap_prach_configuration_info_len
(
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info
);

static
rrc_return_et
rrc_compose_x2ap_prach_configuration_info
(
    U8  **pp_buffer,
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info
);

static
rrc_length_t
rrc_get_x2ap_subframe_allocation_len
(
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation
);

static
rrc_return_et
rrc_compose_x2ap_subframe_allocation
(
    U8  **pp_buffer,
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation
);

static
rrc_length_t
rrc_get_x2ap_mbsfn_info_len
(
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info
);

static
rrc_return_et
rrc_compose_x2ap_mbsfn_info
(
    U8  **pp_buffer,
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info
);

static
rrc_length_t
rrc_get_x2ap_mbsfn_subframe_info_len
(
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info
);

static
rrc_return_et
rrc_compose_x2ap_mbsfn_subframe_info
(
    U8  **pp_buffer,
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info
);

static
rrc_length_t
rrc_get_x2ap_served_cell_info_len
(
    x2ap_served_cell_info_t *p_x2ap_served_cell_info
);

static
rrc_return_et
rrc_compose_x2ap_served_cell_info
(
    U8  **pp_buffer,
    x2ap_served_cell_info_t *p_x2ap_served_cell_info
);

static
rrc_length_t
rrc_get_x2ap_ecgi_len
(
    x2ap_ecgi_t *p_x2ap_ecgi
);

static
rrc_return_et
rrc_compose_x2ap_ecgi
(
    U8  **pp_buffer,
    x2ap_ecgi_t *p_x2ap_ecgi
);

static
rrc_length_t
rrc_get_x2ap_nbour_info_len
(
    x2ap_nbour_info_t *p_x2ap_nbour_info
);

static
rrc_return_et
rrc_compose_x2ap_nbour_info
(
    U8  **pp_buffer,
    x2ap_nbour_info_t *p_x2ap_nbour_info
);

static
rrc_length_t
rrc_get_x2ap_nbour_cell_info_len
(
    x2ap_nbour_cell_info_t *p_x2ap_nbour_cell_info
);

static
rrc_return_et
rrc_compose_x2ap_nbour_cell_info
(
    U8  **pp_buffer,
    x2ap_nbour_cell_info_t *p_x2ap_nbour_cell_info
);

static
rrc_length_t
rrc_get_x2ap_served_cell_info_arr_len
(
    x2ap_served_cell_info_arr_t *p_x2ap_served_cell_info_arr
);

static
rrc_return_et
rrc_compose_x2ap_served_cell_info_arr
(
    U8  **pp_buffer,
    x2ap_served_cell_info_arr_t *p_x2ap_served_cell_info_arr
);

static
rrc_length_t
rrc_get_x2ap_served_cells_to_add_list_len
(
    x2ap_served_cells_to_add_list_t *p_x2ap_served_cells_to_add_list
);

static
rrc_return_et
rrc_compose_x2ap_served_cells_to_add_list
(
    U8  **pp_buffer,
    x2ap_served_cells_to_add_list_t *p_x2ap_served_cells_to_add_list
);

static
rrc_length_t
rrc_get_x2ap_served_cells_to_modify_info_len
(
    x2ap_served_cells_to_modify_info_t *p_x2ap_served_cells_to_modify_info
);

static
rrc_return_et
rrc_compose_x2ap_served_cells_to_modify_info
(
    U8  **pp_buffer,
    x2ap_served_cells_to_modify_info_t *p_x2ap_served_cells_to_modify_info
);

static
rrc_length_t
rrc_get_x2ap_served_cells_to_modify_list_len
(
    x2ap_served_cells_to_modify_list_t *p_x2ap_served_cells_to_modify_list
);

static
rrc_return_et
rrc_compose_x2ap_served_cells_to_modify_list
(
    U8  **pp_buffer,
    x2ap_served_cells_to_modify_list_t *p_x2ap_served_cells_to_modify_list
);

static
rrc_length_t
rrc_get_x2ap_served_cells_to_delete_list_len
(
    x2ap_served_cells_to_delete_list_t *p_x2ap_served_cells_to_delete_list
);

static
rrc_return_et
rrc_compose_x2ap_served_cells_to_delete_list
(
    U8  **pp_buffer,
    x2ap_served_cells_to_delete_list_t *p_x2ap_served_cells_to_delete_list
);

static
rrc_length_t
rrc_get_x2ap_gu_group_id_len
(
    x2ap_gu_group_id_t *p_x2ap_gu_group_id
);

static
rrc_return_et
rrc_compose_x2ap_gu_group_id
(
    U8  **pp_buffer,
    x2ap_gu_group_id_t *p_x2ap_gu_group_id
);

static
rrc_length_t
rrc_get_x2ap_gu_group_id_list_len
(
    x2ap_gu_group_id_list_t *p_x2ap_gu_group_id_list
);

static
rrc_return_et
rrc_compose_x2ap_gu_group_id_list
(
    U8  **pp_buffer,
    x2ap_gu_group_id_list_t *p_x2ap_gu_group_id_list
);

static
rrc_length_t
rrc_get_x2ap_neigh_ecgi_enb_id_map_info_len
(
    x2ap_neigh_ecgi_enb_id_map_info_t *p_x2ap_neigh_ecgi_enb_id_map_info
);

static
rrc_return_et
rrc_compose_x2ap_neigh_ecgi_enb_id_map_info
(
    U8  **pp_buffer,
    x2ap_neigh_ecgi_enb_id_map_info_t *p_x2ap_neigh_ecgi_enb_id_map_info
);

static
rrc_length_t
rrc_get_x2ap_neigh_ecgi_enb_id_map_list_len
(
    x2ap_neigh_ecgi_enb_id_map_list_t *p_x2ap_neigh_ecgi_enb_id_map_list
);

static
rrc_return_et
rrc_compose_x2ap_neigh_ecgi_enb_id_map_list
(
    U8  **pp_buffer,
    x2ap_neigh_ecgi_enb_id_map_list_t *p_x2ap_neigh_ecgi_enb_id_map_list
);

static
rrc_length_t
rrc_get_x2ap_cause_len
(
    x2ap_cause_t *p_x2ap_cause
);

static
rrc_return_et
rrc_compose_x2ap_cause
(
    U8  **pp_buffer,
    x2ap_cause_t *p_x2ap_cause
);

static
rrc_length_t
rrc_get_x2_ip_addr_len
(
    x2_ip_addr_t *p_x2_ip_addr
);

static
rrc_return_et
rrc_compose_x2_ip_addr
(
    U8  **pp_buffer,
    x2_ip_addr_t *p_x2_ip_addr
);

static
rrc_length_t
rrc_get_x2_ipv6_addr_len
(
    x2_ipv6_addr_t *p_x2_ipv6_addr
);

static
rrc_return_et
rrc_compose_x2_ipv6_addr
(
    U8  **pp_buffer,
    x2_ipv6_addr_t *p_x2_ipv6_addr
);

static
rrc_length_t
rrc_get_x2_trans_layer_addrs_len
(
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs
);

static
rrc_return_et
rrc_compose_x2_trans_layer_addrs
(
    U8  **pp_buffer,
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs
);

static
rrc_length_t
rrc_get_x2_extend_trans_layer_addrs_len
(
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs
);

static
rrc_return_et
rrc_compose_x2_extend_trans_layer_addrs
(
    U8  **pp_buffer,
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs
);

static
rrc_length_t
rrc_get_x2_enb_comm_info_len
(
    x2_enb_comm_info_t *p_x2_enb_comm_info
);

static
rrc_return_et
rrc_compose_x2_enb_comm_info
(
    U8  **pp_buffer,
    x2_enb_comm_info_t *p_x2_enb_comm_info
);

static
rrc_length_t
rrc_get_x2ap_enb_comm_info_list_len
(
    x2ap_enb_comm_info_list_t *p_x2ap_enb_comm_info_list
);

static
rrc_return_et
rrc_compose_x2ap_enb_comm_info_list
(
    U8  **pp_buffer,
    x2ap_enb_comm_info_list_t *p_x2ap_enb_comm_info_list
);

static
rrc_length_t
rrc_get_x2ap_cell_activation_info_len
(
    x2ap_cell_activation_info_t *p_x2ap_cell_activation_info
);

static
rrc_return_et
rrc_compose_x2ap_cell_activation_info
(
    U8  **pp_buffer,
    x2ap_cell_activation_info_t *p_x2ap_cell_activation_info
);

static
rrc_length_t
rrc_get_x2ap_cell_activation_resp_len
(
    x2ap_cell_activation_resp_t *p_x2ap_cell_activation_resp
);

static
rrc_return_et
rrc_compose_x2ap_cell_activation_resp
(
    U8  **pp_buffer,
    x2ap_cell_activation_resp_t *p_x2ap_cell_activation_resp
);

static
rrc_length_t
rrc_get_x2ap_mob_param_len
(
    x2ap_mob_param_t *p_x2ap_mob_param
);

static
rrc_return_et
rrc_compose_x2ap_mob_param
(
    U8  **pp_buffer,
    x2ap_mob_param_t *p_x2ap_mob_param
);

static
rrc_length_t
rrc_get_x2ap_mobility_change_info_len
(
    x2ap_mobility_change_info_t *p_x2ap_mobility_change_info
);

static
rrc_return_et
rrc_compose_x2ap_mobility_change_info
(
    U8  **pp_buffer,
    x2ap_mobility_change_info_t *p_x2ap_mobility_change_info
);

static
rrc_length_t
rrc_get_x2ap_mob_range_len
(
    x2ap_mob_range_t *p_x2ap_mob_range
);

static
rrc_return_et
rrc_compose_x2ap_mob_range
(
    U8  **pp_buffer,
    x2ap_mob_range_t *p_x2ap_mob_range
);

static
rrc_length_t
rrc_get_x2ap_mob_ch_resp_len
(
    x2ap_mob_ch_resp_t *p_x2ap_mob_ch_resp
);

static
rrc_return_et
rrc_compose_x2ap_mob_ch_resp
(
    U8  **pp_buffer,
    x2ap_mob_ch_resp_t *p_x2ap_mob_ch_resp
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_length_t
rrc_get_x2ap_enb_config_update_req_len
(
    x2ap_enb_config_update_req_t *p_x2ap_enb_config_update_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_enb_config_update_req != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_enb_config_update_req->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_enb_config_update_req->num_peer_enb);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_enb_config_update_req->num_peer_enb > MAX_PEER_ENB))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_enb_config_update_req->num_peer_enb] should be in range [%d <= MAX_PEER_ENB] !", p_x2ap_enb_config_update_req->num_peer_enb);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_enb_config_update_req->num_peer_enb; loop++)
        {
            length += rrc_get_x2_gb_enb_id_len(&p_x2ap_enb_config_update_req->enb_info[loop]);
        }
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_req->bitmask & X2AP_SERVED_CELLS_TO_ADD_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_served_cells_to_add_list_len(&p_x2ap_enb_config_update_req->add_served_cell);
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_req->bitmask & X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_served_cells_to_modify_list_len(&p_x2ap_enb_config_update_req->mod_served_cell);
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_req->bitmask & X2AP_SERVED_CELLS_TO_DELETE_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_served_cells_to_delete_list_len(&p_x2ap_enb_config_update_req->del_served_cell);
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_req->bitmask & X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_gu_group_id_list_len(&p_x2ap_enb_config_update_req->add_gu_id);
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_req->bitmask & X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_gu_group_id_list_len(&p_x2ap_enb_config_update_req->delete_gu_id);
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_req->bitmask & X2AP_NEIGH_ECGI_ENB_ID_MAPPING_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_neigh_ecgi_enb_id_map_list_len(&p_x2ap_enb_config_update_req->neigh_ecgi_enb_id_map);
    }

    return length;
}

rrc_return_et
rrc_compose_x2ap_enb_config_update_req
(
    U8  **pp_buffer,
    x2ap_enb_config_update_req_t *p_x2ap_enb_config_update_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_enb_config_update_req != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_enb_config_update_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_enb_config_update_req->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_enb_config_update_req->num_peer_enb > MAX_PEER_ENB))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_enb_config_update_req->num_peer_enb] should be in range [%d <= MAX_PEER_ENB] !", p_x2ap_enb_config_update_req->num_peer_enb);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_enb_config_update_req->num_peer_enb, "num_peer_enb");
    *pp_buffer += sizeof(p_x2ap_enb_config_update_req->num_peer_enb);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_enb_config_update_req->num_peer_enb; loop++)
        {
            if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_enb_config_update_req->enb_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_req->bitmask & X2AP_SERVED_CELLS_TO_ADD_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_served_cells_to_add_list(pp_buffer, &p_x2ap_enb_config_update_req->add_served_cell))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_req->bitmask & X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_served_cells_to_modify_list(pp_buffer, &p_x2ap_enb_config_update_req->mod_served_cell))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_req->bitmask & X2AP_SERVED_CELLS_TO_DELETE_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_served_cells_to_delete_list(pp_buffer, &p_x2ap_enb_config_update_req->del_served_cell))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_req->bitmask & X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_gu_group_id_list(pp_buffer, &p_x2ap_enb_config_update_req->add_gu_id))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_req->bitmask & X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_gu_group_id_list(pp_buffer, &p_x2ap_enb_config_update_req->delete_gu_id))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_req->bitmask & X2AP_NEIGH_ECGI_ENB_ID_MAPPING_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_neigh_ecgi_enb_id_map_list(pp_buffer, &p_x2ap_enb_config_update_req->neigh_ecgi_enb_id_map))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_enb_config_update_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_ENB_CONFIG_UPDATE_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_enb_config_update_req
(
    x2ap_enb_config_update_req_t  *p_x2ap_enb_config_update_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_enb_config_update_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_enb_config_update_req_len(p_x2ap_enb_config_update_req);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_ENB_CONFIG_UPDATE_REQ", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_ENB_CONFIG_UPDATE_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_ENB_CONFIG_UPDATE_REQ, msg_length, cell_index);

    /* Fill X2AP_ENB_CONFIG_UPDATE_REQ message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_enb_config_update_req(&p_x2apCommon_msg, p_x2ap_enb_config_update_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_plmn_identity_len
(
    x2ap_plmn_identity_t *p_x2ap_plmn_identity
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_plmn_identity != PNULL);

    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_plmn_identity->plmn_id);

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_plmn_identity
(
    U8  **pp_buffer,
    x2ap_plmn_identity_t *p_x2ap_plmn_identity
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_plmn_identity != PNULL);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_plmn_identity->plmn_id); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_plmn_identity->plmn_id[loop], "plmn_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_macro_enb_id_len
(
    macro_enb_id_t *p_macro_enb_id
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_macro_enb_id != PNULL);

    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_macro_enb_id->eNB_id);

    return length;
}

static
rrc_return_et
rrc_compose_macro_enb_id
(
    U8  **pp_buffer,
    macro_enb_id_t *p_macro_enb_id
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_macro_enb_id != PNULL);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_macro_enb_id->eNB_id); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_macro_enb_id->eNB_id[loop], "eNB_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_home_enb_id_len
(
    home_enb_id_t *p_home_enb_id
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_home_enb_id != PNULL);

    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_home_enb_id->eNB_id);

    return length;
}

static
rrc_return_et
rrc_compose_home_enb_id
(
    U8  **pp_buffer,
    home_enb_id_t *p_home_enb_id
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_home_enb_id != PNULL);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_home_enb_id->eNB_id); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_home_enb_id->eNB_id[loop], "eNB_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2_enb_id_info_len
(
    x2_enb_id_info_t *p_x2_enb_id_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2_enb_id_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2_enb_id_info->presence_bitmask);

    /* Optional element */
    if(p_x2_enb_id_info->presence_bitmask & 1)
    {

    /* Get length of IE */
    length += rrc_get_macro_enb_id_len(&p_x2_enb_id_info->macro_enb_id);
    }

    /* Optional element */
    if(p_x2_enb_id_info->presence_bitmask & 2)
    {

    /* Get length of IE */
    length += rrc_get_home_enb_id_len(&p_x2_enb_id_info->home_enb_id);
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2_enb_id_info
(
    U8  **pp_buffer,
    x2_enb_id_info_t *p_x2_enb_id_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2_enb_id_info != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2_enb_id_info->presence_bitmask, "presence_bitmask");
    *pp_buffer += sizeof(p_x2_enb_id_info->presence_bitmask);

    /* Optional element */
    if(p_x2_enb_id_info->presence_bitmask & 1)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_macro_enb_id(pp_buffer, &p_x2_enb_id_info->macro_enb_id))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2_enb_id_info->presence_bitmask & 2)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_home_enb_id(pp_buffer, &p_x2_enb_id_info->home_enb_id))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2_gb_enb_id_len
(
    x2_gb_enb_id_t *p_x2_gb_enb_id
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2_gb_enb_id != PNULL);


    /* Get length of IE */
    length += rrc_get_x2ap_plmn_identity_len(&p_x2_gb_enb_id->plmn_identity);

    /* Get length of IE */
    length += rrc_get_x2_enb_id_info_len(&p_x2_gb_enb_id->enb_id);

    return length;
}

static
rrc_return_et
rrc_compose_x2_gb_enb_id
(
    U8  **pp_buffer,
    x2_gb_enb_id_t *p_x2_gb_enb_id
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2_gb_enb_id != PNULL);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_plmn_identity(pp_buffer, &p_x2_gb_enb_id->plmn_identity))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_enb_id_info(pp_buffer, &p_x2_gb_enb_id->enb_id))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_broadcast_plmn_len
(
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_broadcast_plmn != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_broadcast_plmn->num_bcast_plmn_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_broadcast_plmn->num_bcast_plmn_id < 1) || (p_x2ap_broadcast_plmn->num_bcast_plmn_id > MAX_NUM_BPLMN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_broadcast_plmn->num_bcast_plmn_id] should be in range [1 <= %d <= MAX_NUM_BPLMN] !", p_x2ap_broadcast_plmn->num_bcast_plmn_id);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_broadcast_plmn->num_bcast_plmn_id; loop++)
        {
            length += rrc_get_x2ap_plmn_identity_len(&p_x2ap_broadcast_plmn->plmn_identity[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_broadcast_plmn
(
    U8  **pp_buffer,
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_broadcast_plmn != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_broadcast_plmn->num_bcast_plmn_id < 1) || (p_x2ap_broadcast_plmn->num_bcast_plmn_id > MAX_NUM_BPLMN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_broadcast_plmn->num_bcast_plmn_id] should be in range [1 <= %d <= MAX_NUM_BPLMN] !", p_x2ap_broadcast_plmn->num_bcast_plmn_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_broadcast_plmn->num_bcast_plmn_id, "num_bcast_plmn_id");
    *pp_buffer += sizeof(p_x2ap_broadcast_plmn->num_bcast_plmn_id);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_broadcast_plmn->num_bcast_plmn_id; loop++)
        {
            if (RRC_FAILURE == rrc_compose_x2ap_plmn_identity(pp_buffer, &p_x2ap_broadcast_plmn->plmn_identity[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_earfcn_len
(
    x2ap_earfcn_t *p_x2ap_earfcn
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_earfcn != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_earfcn->earfcn);

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_earfcn
(
    U8  **pp_buffer,
    x2ap_earfcn_t *p_x2ap_earfcn
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_earfcn != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_earfcn->earfcn > MAX_EARFCN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_earfcn->earfcn] should be in range [%d <= MAX_EARFCN] !", p_x2ap_earfcn->earfcn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_earfcn->earfcn, "earfcn");
    *pp_buffer += sizeof(p_x2ap_earfcn->earfcn);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_fdd_info_len
(
    x2ap_fdd_info_t *p_x2ap_fdd_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_fdd_info != PNULL);


    /* Get length of IE */
    length += rrc_get_x2ap_earfcn_len(&p_x2ap_fdd_info->ul_earfcn);

    /* Get length of IE */
    length += rrc_get_x2ap_earfcn_len(&p_x2ap_fdd_info->dl_earfcn);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_fdd_info->ul_trnsmission_bdwidth);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_fdd_info->dl_trnsmission_bdwidth);

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_fdd_info
(
    U8  **pp_buffer,
    x2ap_fdd_info_t *p_x2ap_fdd_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_fdd_info != PNULL);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_earfcn(pp_buffer, &p_x2ap_fdd_info->ul_earfcn))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_earfcn(pp_buffer, &p_x2ap_fdd_info->dl_earfcn))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_fdd_info->ul_trnsmission_bdwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_fdd_info->ul_trnsmission_bdwidth] should be in range [%d <= 5] !", p_x2ap_fdd_info->ul_trnsmission_bdwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_fdd_info->ul_trnsmission_bdwidth, "ul_trnsmission_bdwidth");
    *pp_buffer += sizeof(p_x2ap_fdd_info->ul_trnsmission_bdwidth);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_fdd_info->dl_trnsmission_bdwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_fdd_info->dl_trnsmission_bdwidth] should be in range [%d <= 5] !", p_x2ap_fdd_info->dl_trnsmission_bdwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_fdd_info->dl_trnsmission_bdwidth, "dl_trnsmission_bdwidth");
    *pp_buffer += sizeof(p_x2ap_fdd_info->dl_trnsmission_bdwidth);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_special_subframe_info_len
(
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_special_subframe_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_special_subframe_info->special_subframe_patterns);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_special_subframe_info->dl_cp);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_special_subframe_info->ul_cp);

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_special_subframe_info
(
    U8  **pp_buffer,
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_special_subframe_info != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->special_subframe_patterns > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_special_subframe_info->special_subframe_patterns] should be in range [%d <= 8] !", p_x2ap_special_subframe_info->special_subframe_patterns);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_special_subframe_info->special_subframe_patterns, "special_subframe_patterns");
    *pp_buffer += sizeof(p_x2ap_special_subframe_info->special_subframe_patterns);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->dl_cp > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_special_subframe_info->dl_cp] should be in range [%d <= 1] !", p_x2ap_special_subframe_info->dl_cp);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_special_subframe_info->dl_cp, "dl_cp");
    *pp_buffer += sizeof(p_x2ap_special_subframe_info->dl_cp);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->ul_cp > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_special_subframe_info->ul_cp] should be in range [%d <= 1] !", p_x2ap_special_subframe_info->ul_cp);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_special_subframe_info->ul_cp, "ul_cp");
    *pp_buffer += sizeof(p_x2ap_special_subframe_info->ul_cp);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_tdd_info_len
(
    x2ap_tdd_info_t *p_x2ap_tdd_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_tdd_info != PNULL);


    /* Get length of IE */
    length += rrc_get_x2ap_earfcn_len(&p_x2ap_tdd_info->earfcn);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_tdd_info->trnsmission_bdwidth);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_tdd_info->subframe_assn);

    /* Get length of IE */
    length += rrc_get_x2ap_special_subframe_info_len(&p_x2ap_tdd_info->special_subframe_info);

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_tdd_info
(
    U8  **pp_buffer,
    x2ap_tdd_info_t *p_x2ap_tdd_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_tdd_info != PNULL);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_earfcn(pp_buffer, &p_x2ap_tdd_info->earfcn))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_tdd_info->trnsmission_bdwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_tdd_info->trnsmission_bdwidth] should be in range [%d <= 5] !", p_x2ap_tdd_info->trnsmission_bdwidth);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_tdd_info->trnsmission_bdwidth, "trnsmission_bdwidth");
    *pp_buffer += sizeof(p_x2ap_tdd_info->trnsmission_bdwidth);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_tdd_info->subframe_assn > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_tdd_info->subframe_assn] should be in range [%d <= 6] !", p_x2ap_tdd_info->subframe_assn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_tdd_info->subframe_assn, "subframe_assn");
    *pp_buffer += sizeof(p_x2ap_tdd_info->subframe_assn);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_special_subframe_info(pp_buffer, &p_x2ap_tdd_info->special_subframe_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_choice_eutra_mode_len
(
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_choice_eutra_mode != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_choice_eutra_mode->bitmask);

    /* Optional element */
    if(p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_FDD)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_fdd_info_len(&p_x2ap_choice_eutra_mode->fdd_info);
    }

    /* Optional element */
    if(p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_TDD)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_tdd_info_len(&p_x2ap_choice_eutra_mode->tdd_info);
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_choice_eutra_mode
(
    U8  **pp_buffer,
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_choice_eutra_mode != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_choice_eutra_mode->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_choice_eutra_mode->bitmask);

    /* Optional element */
    if(p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_FDD)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_fdd_info(pp_buffer, &p_x2ap_choice_eutra_mode->fdd_info))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_TDD)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_tdd_info(pp_buffer, &p_x2ap_choice_eutra_mode->tdd_info))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_choice_eutra_mode_info_len
(
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_choice_eutra_mode_info != PNULL);


    /* Get length of IE */
    length += rrc_get_x2ap_choice_eutra_mode_len(&p_x2ap_choice_eutra_mode_info->choice_eutra_mode);

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_choice_eutra_mode_info
(
    U8  **pp_buffer,
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_choice_eutra_mode_info != PNULL);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_choice_eutra_mode(pp_buffer, &p_x2ap_choice_eutra_mode_info->choice_eutra_mode))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_prach_configuration_info_len
(
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_prach_configuration_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_prach_configuration_info->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_prach_configuration_info->root_sequence_index);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_prach_configuration_info->zero_correlation_zone_config);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_prach_configuration_info->high_speed_flag);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_prach_configuration_info->prach_frequency_offset);

    /* Optional element */
    if(p_x2ap_prach_configuration_info->bitmask & X2AP_PRACH_CONFIG_INDEX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_prach_configuration_info->prach_config_index);
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_prach_configuration_info
(
    U8  **pp_buffer,
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_prach_configuration_info != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_prach_configuration_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->root_sequence_index > 837))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->root_sequence_index] should be in range [%d <= 837] !", p_x2ap_prach_configuration_info->root_sequence_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_prach_configuration_info->root_sequence_index, "root_sequence_index");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->root_sequence_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->zero_correlation_zone_config > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->zero_correlation_zone_config] should be in range [%d <= 15] !", p_x2ap_prach_configuration_info->zero_correlation_zone_config);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_prach_configuration_info->zero_correlation_zone_config, "zero_correlation_zone_config");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->zero_correlation_zone_config);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->high_speed_flag > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->high_speed_flag] should be in range [%d <= 1] !", p_x2ap_prach_configuration_info->high_speed_flag);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_prach_configuration_info->high_speed_flag, "high_speed_flag");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->high_speed_flag);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->prach_frequency_offset > 94))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->prach_frequency_offset] should be in range [%d <= 94] !", p_x2ap_prach_configuration_info->prach_frequency_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_prach_configuration_info->prach_frequency_offset, "prach_frequency_offset");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->prach_frequency_offset);

    /* Optional element */
    if(p_x2ap_prach_configuration_info->bitmask & X2AP_PRACH_CONFIG_INDEX_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->prach_config_index > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->prach_config_index] should be in range [%d <= 63] !", p_x2ap_prach_configuration_info->prach_config_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_prach_configuration_info->prach_config_index, "prach_config_index");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->prach_config_index);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_subframe_allocation_len
(
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_subframe_allocation != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_subframe_allocation->bitmask);

    /* Optional element */
    if(p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_subframe_allocation->oneframe);
    }

    /* Optional element */
    if(p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_subframe_allocation->fourframe);
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_subframe_allocation
(
    U8  **pp_buffer,
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_subframe_allocation != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_subframe_allocation->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_subframe_allocation->bitmask);

    /* Optional element */
    if(p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_subframe_allocation->oneframe); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_subframe_allocation->oneframe[loop], "oneframe[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_subframe_allocation->fourframe); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_subframe_allocation->fourframe[loop], "fourframe[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_mbsfn_info_len
(
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_mbsfn_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_mbsfn_info->radioframe_alloc_period);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_mbsfn_info->radioframe_alloc_offset);

    /* Get length of IE */
    length += rrc_get_x2ap_subframe_allocation_len(&p_x2ap_mbsfn_info->subframe_alloc);

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_mbsfn_info
(
    U8  **pp_buffer,
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_mbsfn_info != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_info->radioframe_alloc_period > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_info->radioframe_alloc_period] should be in range [%d <= 5] !", p_x2ap_mbsfn_info->radioframe_alloc_period);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_mbsfn_info->radioframe_alloc_period, "radioframe_alloc_period");
    *pp_buffer += sizeof(p_x2ap_mbsfn_info->radioframe_alloc_period);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_info->radioframe_alloc_offset > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_info->radioframe_alloc_offset] should be in range [%d <= 7] !", p_x2ap_mbsfn_info->radioframe_alloc_offset);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_mbsfn_info->radioframe_alloc_offset, "radioframe_alloc_offset");
    *pp_buffer += sizeof(p_x2ap_mbsfn_info->radioframe_alloc_offset);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_subframe_allocation(pp_buffer, &p_x2ap_mbsfn_info->subframe_alloc))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_mbsfn_subframe_info_len
(
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_mbsfn_subframe_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_mbsfn_subframe_info->num_mbsfn);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_subframe_info->num_mbsfn > MAX_NUM_MBSFN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_subframe_info->num_mbsfn] should be in range [%d <= MAX_NUM_MBSFN] !", p_x2ap_mbsfn_subframe_info->num_mbsfn);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_mbsfn_subframe_info->num_mbsfn; loop++)
        {
            length += rrc_get_x2ap_mbsfn_info_len(&p_x2ap_mbsfn_subframe_info->mbsfn_info[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_mbsfn_subframe_info
(
    U8  **pp_buffer,
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_mbsfn_subframe_info != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_subframe_info->num_mbsfn > MAX_NUM_MBSFN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_subframe_info->num_mbsfn] should be in range [%d <= MAX_NUM_MBSFN] !", p_x2ap_mbsfn_subframe_info->num_mbsfn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_mbsfn_subframe_info->num_mbsfn, "num_mbsfn");
    *pp_buffer += sizeof(p_x2ap_mbsfn_subframe_info->num_mbsfn);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_mbsfn_subframe_info->num_mbsfn; loop++)
        {
            if (RRC_FAILURE == rrc_compose_x2ap_mbsfn_info(pp_buffer, &p_x2ap_mbsfn_subframe_info->mbsfn_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_served_cell_info_len
(
    x2ap_served_cell_info_t *p_x2ap_served_cell_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_served_cell_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_served_cell_info->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_served_cell_info->pci);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_served_cell_info->cell_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_served_cell_info->tac);

    /* Get length of IE */
    length += rrc_get_x2ap_broadcast_plmn_len(&p_x2ap_served_cell_info->bcast_plmn_info);

    /* Get length of IE */
    length += rrc_get_x2ap_choice_eutra_mode_info_len(&p_x2ap_served_cell_info->choice_eutra_mode);

    /* Optional element */
    if(p_x2ap_served_cell_info->bitmask & X2AP_NUM_ANTENNA_INFO_PORT_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_served_cell_info->num_antenna_port);
    }

    /* Optional element */
    if(p_x2ap_served_cell_info->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_prach_configuration_info_len(&p_x2ap_served_cell_info->prach_config);
    }

    /* Optional element */
    if(p_x2ap_served_cell_info->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_mbsfn_subframe_info_len(&p_x2ap_served_cell_info->mbsfn_subframe_info);
    }

    /* Optional element */
    if(p_x2ap_served_cell_info->bitmask & X2AP_CSG_ID_INFO_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_served_cell_info->csg_identity);
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_served_cell_info
(
    U8  **pp_buffer,
    x2ap_served_cell_info_t *p_x2ap_served_cell_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_served_cell_info != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_served_cell_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_served_cell_info->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_served_cell_info->pci > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cell_info->pci] should be in range [%d <= 503] !", p_x2ap_served_cell_info->pci);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_served_cell_info->pci, "pci");
    *pp_buffer += sizeof(p_x2ap_served_cell_info->pci);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_served_cell_info->cell_id, "cell_id");
    *pp_buffer += sizeof(p_x2ap_served_cell_info->cell_id);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_served_cell_info->tac); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_served_cell_info->tac[loop], "tac[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_broadcast_plmn(pp_buffer, &p_x2ap_served_cell_info->bcast_plmn_info))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_choice_eutra_mode_info(pp_buffer, &p_x2ap_served_cell_info->choice_eutra_mode))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_x2ap_served_cell_info->bitmask & X2AP_NUM_ANTENNA_INFO_PORT_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_served_cell_info->num_antenna_port > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cell_info->num_antenna_port] should be in range [%d <= 2] !", p_x2ap_served_cell_info->num_antenna_port);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_served_cell_info->num_antenna_port, "num_antenna_port");
    *pp_buffer += sizeof(p_x2ap_served_cell_info->num_antenna_port);
    }

    /* Optional element */
    if(p_x2ap_served_cell_info->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_prach_configuration_info(pp_buffer, &p_x2ap_served_cell_info->prach_config))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_served_cell_info->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_mbsfn_subframe_info(pp_buffer, &p_x2ap_served_cell_info->mbsfn_subframe_info))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_served_cell_info->bitmask & X2AP_CSG_ID_INFO_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_served_cell_info->csg_identity); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_served_cell_info->csg_identity[loop], "csg_identity[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_ecgi_len
(
    x2ap_ecgi_t *p_x2ap_ecgi
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_ecgi != PNULL);


    /* Get length of IE */
    length += rrc_get_x2ap_plmn_identity_len(&p_x2ap_ecgi->plmn_identity);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_ecgi->eutran_cell_id);

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_ecgi
(
    U8  **pp_buffer,
    x2ap_ecgi_t *p_x2ap_ecgi
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_ecgi != PNULL);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_plmn_identity(pp_buffer, &p_x2ap_ecgi->plmn_identity))
    {
        return RRC_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_ecgi->eutran_cell_id); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_ecgi->eutran_cell_id[loop], "eutran_cell_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_nbour_info_len
(
    x2ap_nbour_info_t *p_x2ap_nbour_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_nbour_info != PNULL);


    /* Get length of IE */
    length += rrc_get_x2ap_ecgi_len(&p_x2ap_nbour_info->ecgi);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_nbour_info->pci);

    /* Get length of IE */
    length += rrc_get_x2ap_earfcn_len(&p_x2ap_nbour_info->earfcn);

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_nbour_info
(
    U8  **pp_buffer,
    x2ap_nbour_info_t *p_x2ap_nbour_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_nbour_info != PNULL);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_ecgi(pp_buffer, &p_x2ap_nbour_info->ecgi))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nbour_info->pci > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nbour_info->pci] should be in range [%d <= 503] !", p_x2ap_nbour_info->pci);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_nbour_info->pci, "pci");
    *pp_buffer += sizeof(p_x2ap_nbour_info->pci);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_earfcn(pp_buffer, &p_x2ap_nbour_info->earfcn))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_nbour_cell_info_len
(
    x2ap_nbour_cell_info_t *p_x2ap_nbour_cell_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_nbour_cell_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_nbour_cell_info->num_neighbour_cell);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nbour_cell_info->num_neighbour_cell > MAX_NEIGHBOURS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nbour_cell_info->num_neighbour_cell] should be in range [%d <= MAX_NEIGHBOURS] !", p_x2ap_nbour_cell_info->num_neighbour_cell);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nbour_cell_info->num_neighbour_cell; loop++)
        {
            length += rrc_get_x2ap_nbour_info_len(&p_x2ap_nbour_cell_info->neighbour_info_arr[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_nbour_cell_info
(
    U8  **pp_buffer,
    x2ap_nbour_cell_info_t *p_x2ap_nbour_cell_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_nbour_cell_info != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nbour_cell_info->num_neighbour_cell > MAX_NEIGHBOURS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nbour_cell_info->num_neighbour_cell] should be in range [%d <= MAX_NEIGHBOURS] !", p_x2ap_nbour_cell_info->num_neighbour_cell);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_nbour_cell_info->num_neighbour_cell, "num_neighbour_cell");
    *pp_buffer += sizeof(p_x2ap_nbour_cell_info->num_neighbour_cell);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nbour_cell_info->num_neighbour_cell; loop++)
        {
            if (RRC_FAILURE == rrc_compose_x2ap_nbour_info(pp_buffer, &p_x2ap_nbour_cell_info->neighbour_info_arr[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_served_cell_info_arr_len
(
    x2ap_served_cell_info_arr_t *p_x2ap_served_cell_info_arr
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_served_cell_info_arr != PNULL);


    /* Get length of IE */
    length += rrc_get_x2ap_served_cell_info_len(&p_x2ap_served_cell_info_arr->cell_info);

    /* Get length of IE */
    length += rrc_get_x2ap_nbour_cell_info_len(&p_x2ap_served_cell_info_arr->nbour_cell_info);

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_served_cell_info_arr
(
    U8  **pp_buffer,
    x2ap_served_cell_info_arr_t *p_x2ap_served_cell_info_arr
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_served_cell_info_arr != PNULL);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_served_cell_info(pp_buffer, &p_x2ap_served_cell_info_arr->cell_info))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_nbour_cell_info(pp_buffer, &p_x2ap_served_cell_info_arr->nbour_cell_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_served_cells_to_add_list_len
(
    x2ap_served_cells_to_add_list_t *p_x2ap_served_cells_to_add_list
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_served_cells_to_add_list != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_served_cells_to_add_list->num_served_cell);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_served_cells_to_add_list->num_served_cell < 1) || (p_x2ap_served_cells_to_add_list->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cells_to_add_list->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_served_cells_to_add_list->num_served_cell);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_served_cells_to_add_list->num_served_cell; loop++)
        {
            length += rrc_get_x2ap_served_cell_info_arr_len(&p_x2ap_served_cells_to_add_list->cell_list[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_served_cells_to_add_list
(
    U8  **pp_buffer,
    x2ap_served_cells_to_add_list_t *p_x2ap_served_cells_to_add_list
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_served_cells_to_add_list != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_served_cells_to_add_list->num_served_cell < 1) || (p_x2ap_served_cells_to_add_list->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cells_to_add_list->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_served_cells_to_add_list->num_served_cell);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_served_cells_to_add_list->num_served_cell, "num_served_cell");
    *pp_buffer += sizeof(p_x2ap_served_cells_to_add_list->num_served_cell);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_served_cells_to_add_list->num_served_cell; loop++)
        {
            if (RRC_FAILURE == rrc_compose_x2ap_served_cell_info_arr(pp_buffer, &p_x2ap_served_cells_to_add_list->cell_list[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_served_cells_to_modify_info_len
(
    x2ap_served_cells_to_modify_info_t *p_x2ap_served_cells_to_modify_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_served_cells_to_modify_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_served_cells_to_modify_info->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_served_cells_to_modify_info->old_cell_id);

    /* Get length of IE */
    length += rrc_get_x2ap_served_cell_info_arr_len(&p_x2ap_served_cells_to_modify_info->served_cell_info);

    /* Optional element */
    if(p_x2ap_served_cells_to_modify_info->bitmask & X2AP_DEACTIVATION_IND_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_served_cells_to_modify_info->deactivation_ind);
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_served_cells_to_modify_info
(
    U8  **pp_buffer,
    x2ap_served_cells_to_modify_info_t *p_x2ap_served_cells_to_modify_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_served_cells_to_modify_info != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_served_cells_to_modify_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_served_cells_to_modify_info->bitmask);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_served_cells_to_modify_info->old_cell_id, "old_cell_id");
    *pp_buffer += sizeof(p_x2ap_served_cells_to_modify_info->old_cell_id);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_served_cell_info_arr(pp_buffer, &p_x2ap_served_cells_to_modify_info->served_cell_info))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_x2ap_served_cells_to_modify_info->bitmask & X2AP_DEACTIVATION_IND_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_served_cells_to_modify_info->deactivation_ind > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cells_to_modify_info->deactivation_ind] should be in range [%d <= 1] !", p_x2ap_served_cells_to_modify_info->deactivation_ind);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_served_cells_to_modify_info->deactivation_ind, "deactivation_ind");
    *pp_buffer += sizeof(p_x2ap_served_cells_to_modify_info->deactivation_ind);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_served_cells_to_modify_list_len
(
    x2ap_served_cells_to_modify_list_t *p_x2ap_served_cells_to_modify_list
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_served_cells_to_modify_list != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_served_cells_to_modify_list->num_served_cell);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_served_cells_to_modify_list->num_served_cell < 1) || (p_x2ap_served_cells_to_modify_list->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cells_to_modify_list->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_served_cells_to_modify_list->num_served_cell);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_served_cells_to_modify_list->num_served_cell; loop++)
        {
            length += rrc_get_x2ap_served_cells_to_modify_info_len(&p_x2ap_served_cells_to_modify_list->cell_list[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_served_cells_to_modify_list
(
    U8  **pp_buffer,
    x2ap_served_cells_to_modify_list_t *p_x2ap_served_cells_to_modify_list
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_served_cells_to_modify_list != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_served_cells_to_modify_list->num_served_cell < 1) || (p_x2ap_served_cells_to_modify_list->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cells_to_modify_list->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_served_cells_to_modify_list->num_served_cell);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_served_cells_to_modify_list->num_served_cell, "num_served_cell");
    *pp_buffer += sizeof(p_x2ap_served_cells_to_modify_list->num_served_cell);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_served_cells_to_modify_list->num_served_cell; loop++)
        {
            if (RRC_FAILURE == rrc_compose_x2ap_served_cells_to_modify_info(pp_buffer, &p_x2ap_served_cells_to_modify_list->cell_list[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_served_cells_to_delete_list_len
(
    x2ap_served_cells_to_delete_list_t *p_x2ap_served_cells_to_delete_list
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_served_cells_to_delete_list != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_served_cells_to_delete_list->num_served_cell);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_x2ap_served_cells_to_delete_list->num_served_cell * sizeof(p_x2ap_served_cells_to_delete_list->cell_id_list[0]));

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_served_cells_to_delete_list
(
    U8  **pp_buffer,
    x2ap_served_cells_to_delete_list_t *p_x2ap_served_cells_to_delete_list
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_served_cells_to_delete_list != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_served_cells_to_delete_list->num_served_cell < 1) || (p_x2ap_served_cells_to_delete_list->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cells_to_delete_list->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_served_cells_to_delete_list->num_served_cell);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_served_cells_to_delete_list->num_served_cell, "num_served_cell");
    *pp_buffer += sizeof(p_x2ap_served_cells_to_delete_list->num_served_cell);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_served_cells_to_delete_list->num_served_cell; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_served_cells_to_delete_list->cell_id_list[loop], "cell_id_list[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_gu_group_id_len
(
    x2ap_gu_group_id_t *p_x2ap_gu_group_id
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_gu_group_id != PNULL);


    /* Get length of IE */
    length += rrc_get_x2ap_plmn_identity_len(&p_x2ap_gu_group_id->plmn_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_gu_group_id->mme_group_id);

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_gu_group_id
(
    U8  **pp_buffer,
    x2ap_gu_group_id_t *p_x2ap_gu_group_id
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_gu_group_id != PNULL);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_plmn_identity(pp_buffer, &p_x2ap_gu_group_id->plmn_id))
    {
        return RRC_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_gu_group_id->mme_group_id); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2ap_gu_group_id->mme_group_id[loop], "mme_group_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_gu_group_id_list_len
(
    x2ap_gu_group_id_list_t *p_x2ap_gu_group_id_list
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_gu_group_id_list != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_gu_group_id_list->num_gu_group_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_gu_group_id_list->num_gu_group_id < 1) || (p_x2ap_gu_group_id_list->num_gu_group_id > MAX_ENB_POOLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_gu_group_id_list->num_gu_group_id] should be in range [1 <= %d <= MAX_ENB_POOLS] !", p_x2ap_gu_group_id_list->num_gu_group_id);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_gu_group_id_list->num_gu_group_id; loop++)
        {
            length += rrc_get_x2ap_gu_group_id_len(&p_x2ap_gu_group_id_list->gu_id_list[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_gu_group_id_list
(
    U8  **pp_buffer,
    x2ap_gu_group_id_list_t *p_x2ap_gu_group_id_list
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_gu_group_id_list != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_gu_group_id_list->num_gu_group_id < 1) || (p_x2ap_gu_group_id_list->num_gu_group_id > MAX_ENB_POOLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_gu_group_id_list->num_gu_group_id] should be in range [1 <= %d <= MAX_ENB_POOLS] !", p_x2ap_gu_group_id_list->num_gu_group_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_gu_group_id_list->num_gu_group_id, "num_gu_group_id");
    *pp_buffer += sizeof(p_x2ap_gu_group_id_list->num_gu_group_id);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_gu_group_id_list->num_gu_group_id; loop++)
        {
            if (RRC_FAILURE == rrc_compose_x2ap_gu_group_id(pp_buffer, &p_x2ap_gu_group_id_list->gu_id_list[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_neigh_ecgi_enb_id_map_info_len
(
    x2ap_neigh_ecgi_enb_id_map_info_t *p_x2ap_neigh_ecgi_enb_id_map_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_neigh_ecgi_enb_id_map_info != PNULL);


    /* Get length of IE */
    length += rrc_get_x2ap_ecgi_len(&p_x2ap_neigh_ecgi_enb_id_map_info->ecgi);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_neigh_ecgi_enb_id_map_info->enb_id);

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_neigh_ecgi_enb_id_map_info
(
    U8  **pp_buffer,
    x2ap_neigh_ecgi_enb_id_map_info_t *p_x2ap_neigh_ecgi_enb_id_map_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_neigh_ecgi_enb_id_map_info != PNULL);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_ecgi(pp_buffer, &p_x2ap_neigh_ecgi_enb_id_map_info->ecgi))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_neigh_ecgi_enb_id_map_info->enb_id))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_neigh_ecgi_enb_id_map_list_len
(
    x2ap_neigh_ecgi_enb_id_map_list_t *p_x2ap_neigh_ecgi_enb_id_map_list
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_neigh_ecgi_enb_id_map_list != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count < 1) || (p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count > MAX_NEIGHBOURS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count] should be in range [1 <= %d <= MAX_NEIGHBOURS] !", p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count; loop++)
        {
            length += rrc_get_x2ap_neigh_ecgi_enb_id_map_info_len(&p_x2ap_neigh_ecgi_enb_id_map_list->neigh_ecgi_enb_id_info[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_neigh_ecgi_enb_id_map_list
(
    U8  **pp_buffer,
    x2ap_neigh_ecgi_enb_id_map_list_t *p_x2ap_neigh_ecgi_enb_id_map_list
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_neigh_ecgi_enb_id_map_list != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count < 1) || (p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count > MAX_NEIGHBOURS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count] should be in range [1 <= %d <= MAX_NEIGHBOURS] !", p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count, "neigh_info_count");
    *pp_buffer += sizeof(p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count; loop++)
        {
            if (RRC_FAILURE == rrc_compose_x2ap_neigh_ecgi_enb_id_map_info(pp_buffer, &p_x2ap_neigh_ecgi_enb_id_map_list->neigh_ecgi_enb_id_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_enb_config_update_resp_len
(
    x2ap_enb_config_update_resp_t *p_x2ap_enb_config_update_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_enb_config_update_resp != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_enb_config_update_resp->bitmask);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_enb_config_update_resp->enb1_gb_id);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_enb_config_update_resp->enb2_gb_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_enb_config_update_resp->response);

    /* Optional element */
    if(p_x2ap_enb_config_update_resp->bitmask & X2AP_CAUSE_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_cause_len(&p_x2ap_enb_config_update_resp->cause);
    }

    return length;
}

rrc_return_et
rrc_compose_x2ap_enb_config_update_resp
(
    U8  **pp_buffer,
    x2ap_enb_config_update_resp_t *p_x2ap_enb_config_update_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_enb_config_update_resp != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_enb_config_update_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_enb_config_update_resp->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_enb_config_update_resp->enb1_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_enb_config_update_resp->enb2_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_enb_config_update_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_enb_config_update_resp->response] should be in range [%d <= 1] !", p_x2ap_enb_config_update_resp->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_enb_config_update_resp->response, "response");
    *pp_buffer += sizeof(p_x2ap_enb_config_update_resp->response);

    /* Optional element */
    if(p_x2ap_enb_config_update_resp->bitmask & X2AP_CAUSE_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_cause(pp_buffer, &p_x2ap_enb_config_update_resp->cause))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_enb_config_update_resp
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_ENB_CONFIG_UPDATE_RES message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_enb_config_update_resp
(
    x2ap_enb_config_update_resp_t  *p_x2ap_enb_config_update_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_enb_config_update_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_enb_config_update_resp_len(p_x2ap_enb_config_update_resp);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_ENB_CONFIG_UPDATE_RES", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_ENB_CONFIG_UPDATE_RES, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_ENB_CONFIG_UPDATE_RES, msg_length, cell_index);

    /* Fill X2AP_ENB_CONFIG_UPDATE_RES message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_enb_config_update_resp(&p_x2apCommon_msg, p_x2ap_enb_config_update_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_cause_len
(
    x2ap_cause_t *p_x2ap_cause
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_cause != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_cause->type);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_cause->value);

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_cause
(
    U8  **pp_buffer,
    x2ap_cause_t *p_x2ap_cause
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_cause != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_cause->type, "type");
    *pp_buffer += sizeof(p_x2ap_cause->type);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_cause->value, "value");
    *pp_buffer += sizeof(p_x2ap_cause->value);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_enb_config_update_ind_len
(
    x2ap_enb_config_update_ind_t *p_x2ap_enb_config_update_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_enb_config_update_ind != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_enb_config_update_ind->bitmask);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_enb_config_update_ind->enb1_gb_id);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_enb_config_update_ind->enb2_gb_id);

    /* Optional element */
    if(p_x2ap_enb_config_update_ind->bitmask & X2AP_SERVED_CELLS_TO_ADD_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_served_cells_to_add_list_len(&p_x2ap_enb_config_update_ind->add_served_cell);
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_ind->bitmask & X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_served_cells_to_modify_list_len(&p_x2ap_enb_config_update_ind->mod_served_cell);
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_ind->bitmask & X2AP_SERVED_CELLS_TO_DELETE_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_served_cells_to_delete_list_len(&p_x2ap_enb_config_update_ind->del_served_cell);
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_ind->bitmask & X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_gu_group_id_list_len(&p_x2ap_enb_config_update_ind->add_gu_id);
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_ind->bitmask & X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_gu_group_id_list_len(&p_x2ap_enb_config_update_ind->delete_gu_id);
    }

    return length;
}

rrc_return_et
rrc_compose_x2ap_enb_config_update_ind
(
    U8  **pp_buffer,
    x2ap_enb_config_update_ind_t *p_x2ap_enb_config_update_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_enb_config_update_ind != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_enb_config_update_ind->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_enb_config_update_ind->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_enb_config_update_ind->enb1_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_enb_config_update_ind->enb2_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_ind->bitmask & X2AP_SERVED_CELLS_TO_ADD_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_served_cells_to_add_list(pp_buffer, &p_x2ap_enb_config_update_ind->add_served_cell))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_ind->bitmask & X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_served_cells_to_modify_list(pp_buffer, &p_x2ap_enb_config_update_ind->mod_served_cell))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_ind->bitmask & X2AP_SERVED_CELLS_TO_DELETE_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_served_cells_to_delete_list(pp_buffer, &p_x2ap_enb_config_update_ind->del_served_cell))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_ind->bitmask & X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_gu_group_id_list(pp_buffer, &p_x2ap_enb_config_update_ind->add_gu_id))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_enb_config_update_ind->bitmask & X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_gu_group_id_list(pp_buffer, &p_x2ap_enb_config_update_ind->delete_gu_id))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_enb_config_update_ind
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_ENB_CONFIG_UPDATE_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_enb_config_update_ind
(
    x2ap_enb_config_update_ind_t  *p_x2ap_enb_config_update_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_enb_config_update_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_enb_config_update_ind_len(p_x2ap_enb_config_update_ind);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_ENB_CONFIG_UPDATE_IND", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_ENB_CONFIG_UPDATE_IND, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_ENB_CONFIG_UPDATE_IND, msg_length, cell_index);

    /* Fill X2AP_ENB_CONFIG_UPDATE_IND message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_enb_config_update_ind(&p_x2apCommon_msg, p_x2ap_enb_config_update_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_reset_req_len
(
    x2ap_reset_req_t *p_x2ap_reset_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_reset_req != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_reset_req->bitmask);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_reset_req->gb_enb_id);

    /* Get length of IE */
    length += rrc_get_x2ap_cause_len(&p_x2ap_reset_req->cause);

    return length;
}

rrc_return_et
rrc_compose_x2ap_reset_req
(
    U8  **pp_buffer,
    x2ap_reset_req_t *p_x2ap_reset_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_reset_req != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_reset_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_reset_req->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_reset_req->gb_enb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_cause(pp_buffer, &p_x2ap_reset_req->cause))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_reset_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RESET_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_reset_req
(
    x2ap_reset_req_t  *p_x2ap_reset_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_reset_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_reset_req_len(p_x2ap_reset_req);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RESET_REQ", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_RESET_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_RESET_REQ, msg_length, cell_index);

    /* Fill X2AP_RESET_REQ message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_reset_req(&p_x2apCommon_msg, p_x2ap_reset_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_reset_resp_len
(
    x2ap_reset_resp_t *p_x2ap_reset_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_reset_resp != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_reset_resp->bitmask);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_reset_resp->enb1_gb_id);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_reset_resp->enb2_gb_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_reset_resp->response);

    return length;
}

rrc_return_et
rrc_compose_x2ap_reset_resp
(
    U8  **pp_buffer,
    x2ap_reset_resp_t *p_x2ap_reset_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_reset_resp != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_reset_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_reset_resp->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_reset_resp->enb1_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_reset_resp->enb2_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_reset_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_reset_resp->response] should be in range [%d <= 1] !", p_x2ap_reset_resp->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_reset_resp->response, "response");
    *pp_buffer += sizeof(p_x2ap_reset_resp->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_reset_resp
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RESET_RES message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_reset_resp
(
    x2ap_reset_resp_t  *p_x2ap_reset_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_reset_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_reset_resp_len(p_x2ap_reset_resp);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RESET_RES", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_RESET_RES, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_RESET_RES, msg_length, cell_index);

    /* Fill X2AP_RESET_RES message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_reset_resp(&p_x2apCommon_msg, p_x2ap_reset_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_reset_ind_len
(
    x2ap_reset_ind_t *p_x2ap_reset_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_reset_ind != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_reset_ind->bitmask);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_reset_ind->enb1_gb_id);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_reset_ind->enb2_gb_id);

    /* Get length of IE */
    length += rrc_get_x2ap_cause_len(&p_x2ap_reset_ind->cause);

    return length;
}

rrc_return_et
rrc_compose_x2ap_reset_ind
(
    U8  **pp_buffer,
    x2ap_reset_ind_t *p_x2ap_reset_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_reset_ind != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_reset_ind->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_reset_ind->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_reset_ind->enb1_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_reset_ind->enb2_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_cause(pp_buffer, &p_x2ap_reset_ind->cause))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_reset_ind
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_RESET_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_reset_ind
(
    x2ap_reset_ind_t  *p_x2ap_reset_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_reset_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_reset_ind_len(p_x2ap_reset_ind);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_RESET_IND", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_RESET_IND, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_RESET_IND, msg_length, cell_index);

    /* Fill X2AP_RESET_IND message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_reset_ind(&p_x2apCommon_msg, p_x2ap_reset_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_add_enb_req_len
(
    x2ap_add_enb_req_t *p_x2ap_add_enb_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_add_enb_req != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_add_enb_req->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_add_enb_req->num_peer_enb);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_add_enb_req->num_peer_enb < 1) || (p_x2ap_add_enb_req->num_peer_enb > MAX_PEER_ENB))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_add_enb_req->num_peer_enb] should be in range [1 <= %d <= MAX_PEER_ENB] !", p_x2ap_add_enb_req->num_peer_enb);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_add_enb_req->num_peer_enb; loop++)
        {
            length += rrc_get_x2_enb_comm_info_len(&p_x2ap_add_enb_req->enb_info[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_compose_x2ap_add_enb_req
(
    U8  **pp_buffer,
    x2ap_add_enb_req_t *p_x2ap_add_enb_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_add_enb_req != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_add_enb_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_add_enb_req->bitmask);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_add_enb_req->num_peer_enb < 1) || (p_x2ap_add_enb_req->num_peer_enb > MAX_PEER_ENB))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_add_enb_req->num_peer_enb] should be in range [1 <= %d <= MAX_PEER_ENB] !", p_x2ap_add_enb_req->num_peer_enb);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_add_enb_req->num_peer_enb, "num_peer_enb");
    *pp_buffer += sizeof(p_x2ap_add_enb_req->num_peer_enb);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_add_enb_req->num_peer_enb; loop++)
        {
            if (RRC_FAILURE == rrc_compose_x2_enb_comm_info(pp_buffer, &p_x2ap_add_enb_req->enb_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_add_enb_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_ADD_ENB_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_add_enb_req
(
    x2ap_add_enb_req_t  *p_x2ap_add_enb_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_add_enb_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_add_enb_req_len(p_x2ap_add_enb_req);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_ADD_ENB_REQ", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_ADD_ENB_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_ADD_ENB_REQ, msg_length, cell_index);

    /* Fill X2AP_ADD_ENB_REQ message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_add_enb_req(&p_x2apCommon_msg, p_x2ap_add_enb_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2_ip_addr_len
(
    x2_ip_addr_t *p_x2_ip_addr
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2_ip_addr != PNULL);

    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2_ip_addr->ip_addr);

    return length;
}

static
rrc_return_et
rrc_compose_x2_ip_addr
(
    U8  **pp_buffer,
    x2_ip_addr_t *p_x2_ip_addr
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2_ip_addr != PNULL);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2_ip_addr->ip_addr); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2_ip_addr->ip_addr[loop], "ip_addr[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2_ipv6_addr_len
(
    x2_ipv6_addr_t *p_x2_ipv6_addr
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2_ipv6_addr != PNULL);

    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2_ipv6_addr->ipv6_addr);

    return length;
}

static
rrc_return_et
rrc_compose_x2_ipv6_addr
(
    U8  **pp_buffer,
    x2_ipv6_addr_t *p_x2_ipv6_addr
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2_ipv6_addr != PNULL);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2_ipv6_addr->ipv6_addr); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2_ipv6_addr->ipv6_addr[loop], "ipv6_addr[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2_trans_layer_addrs_len
(
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2_trans_layer_addrs != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2_trans_layer_addrs->data_length);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_x2_trans_layer_addrs->data_length * sizeof(p_x2_trans_layer_addrs->data[0]));

    return length;
}

static
rrc_return_et
rrc_compose_x2_trans_layer_addrs
(
    U8  **pp_buffer,
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2_trans_layer_addrs != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2_trans_layer_addrs->data_length > RRC_MAX_IP_ADDR_LEN_IPV6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_trans_layer_addrs->data_length] should be in range [%d <= RRC_MAX_IP_ADDR_LEN_IPV6] !", p_x2_trans_layer_addrs->data_length);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2_trans_layer_addrs->data_length, "data_length");
    *pp_buffer += sizeof(p_x2_trans_layer_addrs->data_length);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_trans_layer_addrs->data_length; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_x2_trans_layer_addrs->data[loop], "data[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2_extend_trans_layer_addrs_len
(
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2_extend_trans_layer_addrs != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2_extend_trans_layer_addrs->bitmask);

    /* Optional element */
    if(p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_IPSEC_ADDRS_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2_trans_layer_addrs_len(&p_x2_extend_trans_layer_addrs->ipsec_trans_layer_addrs);
    }

    /* Optional element */
    if(p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_NUM_GTP_ADDRS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_x2_extend_trans_layer_addrs->num_tlas);
    }

    /* Optional element */
    if(p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_GTP_ADDRS_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_extend_trans_layer_addrs->num_tlas < 1) || (p_x2_extend_trans_layer_addrs->num_tlas > MAX_NUM_ENB_X2AP_GTP_TLAS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_extend_trans_layer_addrs->num_tlas] should be in range [1 <= %d <= MAX_NUM_ENB_X2AP_GTP_TLAS] !", p_x2_extend_trans_layer_addrs->num_tlas);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_extend_trans_layer_addrs->num_tlas; loop++)
        {
            length += rrc_get_x2_trans_layer_addrs_len(&p_x2_extend_trans_layer_addrs->gtp_trans_layer_addrs[loop]);
        }
    }
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2_extend_trans_layer_addrs
(
    U8  **pp_buffer,
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2_extend_trans_layer_addrs != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2_extend_trans_layer_addrs->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2_extend_trans_layer_addrs->bitmask);

    /* Optional element */
    if(p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_IPSEC_ADDRS_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_trans_layer_addrs(pp_buffer, &p_x2_extend_trans_layer_addrs->ipsec_trans_layer_addrs))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_NUM_GTP_ADDRS_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_extend_trans_layer_addrs->num_tlas < 1) || (p_x2_extend_trans_layer_addrs->num_tlas > MAX_NUM_ENB_X2AP_GTP_TLAS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_extend_trans_layer_addrs->num_tlas] should be in range [1 <= %d <= MAX_NUM_ENB_X2AP_GTP_TLAS] !", p_x2_extend_trans_layer_addrs->num_tlas);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2_extend_trans_layer_addrs->num_tlas, "num_tlas");
    *pp_buffer += sizeof(p_x2_extend_trans_layer_addrs->num_tlas);
    }

    /* Optional element */
    if(p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_GTP_ADDRS_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_extend_trans_layer_addrs->num_tlas; loop++)
        {
            if (RRC_FAILURE == rrc_compose_x2_trans_layer_addrs(pp_buffer, &p_x2_extend_trans_layer_addrs->gtp_trans_layer_addrs[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2_enb_comm_info_len
(
    x2_enb_comm_info_t *p_x2_enb_comm_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2_enb_comm_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2_enb_comm_info->bitmask);

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_x2_enb_comm_info->num_ip_addr);
    }

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_ip_addr < 1) || (p_x2_enb_comm_info->num_ip_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->num_ip_addr] should be in range [1 <= %d <= MAX_NUM_IP_ADDR] !", p_x2_enb_comm_info->num_ip_addr);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_enb_comm_info->num_ip_addr; loop++)
        {
            length += rrc_get_x2_ip_addr_len(&p_x2_enb_comm_info->ip_addr[loop]);
        }
    }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_x2_enb_comm_info->port);

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_x2_enb_comm_info->num_ipv6_addr);
    }

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_ipv6_addr < 1) || (p_x2_enb_comm_info->num_ipv6_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->num_ipv6_addr] should be in range [1 <= %d <= MAX_NUM_IP_ADDR] !", p_x2_enb_comm_info->num_ipv6_addr);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_enb_comm_info->num_ipv6_addr; loop++)
        {
            length += rrc_get_x2_ipv6_addr_len(&p_x2_enb_comm_info->ipv6_addr[loop]);
        }
    }
    }

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_EXT_NUM_TLAS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_x2_enb_comm_info->num_tlas);
    }

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_EXT_TLAS_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_tlas < 1) || (p_x2_enb_comm_info->num_tlas > MAX_NUM_ENB_X2AP_EXT_TLAS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->num_tlas] should be in range [1 <= %d <= MAX_NUM_ENB_X2AP_EXT_TLAS] !", p_x2_enb_comm_info->num_tlas);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_enb_comm_info->num_tlas; loop++)
        {
            length += rrc_get_x2_extend_trans_layer_addrs_len(&p_x2_enb_comm_info->extend_trans_layer_addrs[loop]);
        }
    }
    }

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_DEFAULT_SGNB_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_x2_enb_comm_info->default_sgnb);
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2_enb_comm_info
(
    U8  **pp_buffer,
    x2_enb_comm_info_t *p_x2_enb_comm_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2_enb_comm_info != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2_enb_comm_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2_enb_comm_info->bitmask);

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_ip_addr < 1) || (p_x2_enb_comm_info->num_ip_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->num_ip_addr] should be in range [1 <= %d <= MAX_NUM_IP_ADDR] !", p_x2_enb_comm_info->num_ip_addr);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2_enb_comm_info->num_ip_addr, "num_ip_addr");
    *pp_buffer += sizeof(p_x2_enb_comm_info->num_ip_addr);
    }

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_enb_comm_info->num_ip_addr; loop++)
        {
            if (RRC_FAILURE == rrc_compose_x2_ip_addr(pp_buffer, &p_x2_enb_comm_info->ip_addr[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2_enb_comm_info->port, "port");
    *pp_buffer += sizeof(p_x2_enb_comm_info->port);

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_ipv6_addr < 1) || (p_x2_enb_comm_info->num_ipv6_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->num_ipv6_addr] should be in range [1 <= %d <= MAX_NUM_IP_ADDR] !", p_x2_enb_comm_info->num_ipv6_addr);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2_enb_comm_info->num_ipv6_addr, "num_ipv6_addr");
    *pp_buffer += sizeof(p_x2_enb_comm_info->num_ipv6_addr);
    }

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_enb_comm_info->num_ipv6_addr; loop++)
        {
            if (RRC_FAILURE == rrc_compose_x2_ipv6_addr(pp_buffer, &p_x2_enb_comm_info->ipv6_addr[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    }

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_EXT_NUM_TLAS_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_tlas < 1) || (p_x2_enb_comm_info->num_tlas > MAX_NUM_ENB_X2AP_EXT_TLAS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->num_tlas] should be in range [1 <= %d <= MAX_NUM_ENB_X2AP_EXT_TLAS] !", p_x2_enb_comm_info->num_tlas);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2_enb_comm_info->num_tlas, "num_tlas");
    *pp_buffer += sizeof(p_x2_enb_comm_info->num_tlas);
    }

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_EXT_TLAS_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_enb_comm_info->num_tlas; loop++)
        {
            if (RRC_FAILURE == rrc_compose_x2_extend_trans_layer_addrs(pp_buffer, &p_x2_enb_comm_info->extend_trans_layer_addrs[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    }

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_DEFAULT_SGNB_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2_enb_comm_info->default_sgnb > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->default_sgnb] should be in range [%d <= 1] !", p_x2_enb_comm_info->default_sgnb);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2_enb_comm_info->default_sgnb, "default_sgnb");
    *pp_buffer += sizeof(p_x2_enb_comm_info->default_sgnb);
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_add_enb_res_len
(
    x2ap_add_enb_res_t *p_x2ap_add_enb_res
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_add_enb_res != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_add_enb_res->bitmask);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_add_enb_res->gb_enb_id);

    /* Optional element */
    if(p_x2ap_add_enb_res->bitmask & X2AP_SUCCESS_ENB_ADD_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_enb_comm_info_list_len(&p_x2ap_add_enb_res->success_enb_list);
    }

    /* Optional element */
    if(p_x2ap_add_enb_res->bitmask & X2AP_FAILURE_ENB_ADD_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_enb_comm_info_list_len(&p_x2ap_add_enb_res->failure_enb_list);
    }

    return length;
}

rrc_return_et
rrc_compose_x2ap_add_enb_res
(
    U8  **pp_buffer,
    x2ap_add_enb_res_t *p_x2ap_add_enb_res
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_add_enb_res != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_add_enb_res->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_add_enb_res->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_add_enb_res->gb_enb_id))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_x2ap_add_enb_res->bitmask & X2AP_SUCCESS_ENB_ADD_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_enb_comm_info_list(pp_buffer, &p_x2ap_add_enb_res->success_enb_list))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_add_enb_res->bitmask & X2AP_FAILURE_ENB_ADD_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_enb_comm_info_list(pp_buffer, &p_x2ap_add_enb_res->failure_enb_list))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_add_enb_res
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_ADD_ENB_RES message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_add_enb_res
(
    x2ap_add_enb_res_t  *p_x2ap_add_enb_res,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_add_enb_res != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_add_enb_res_len(p_x2ap_add_enb_res);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_ADD_ENB_RES", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_ADD_ENB_RES, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_ADD_ENB_RES, msg_length, cell_index);

    /* Fill X2AP_ADD_ENB_RES message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_add_enb_res(&p_x2apCommon_msg, p_x2ap_add_enb_res))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_enb_comm_info_list_len
(
    x2ap_enb_comm_info_list_t *p_x2ap_enb_comm_info_list
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_enb_comm_info_list != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_enb_comm_info_list->num_peer_enb);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_enb_comm_info_list->num_peer_enb < 1) || (p_x2ap_enb_comm_info_list->num_peer_enb > MAX_PEER_ENB))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_enb_comm_info_list->num_peer_enb] should be in range [1 <= %d <= MAX_PEER_ENB] !", p_x2ap_enb_comm_info_list->num_peer_enb);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_enb_comm_info_list->num_peer_enb; loop++)
        {
            length += rrc_get_x2_enb_comm_info_len(&p_x2ap_enb_comm_info_list->enb_comm_info[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_enb_comm_info_list
(
    U8  **pp_buffer,
    x2ap_enb_comm_info_list_t *p_x2ap_enb_comm_info_list
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_enb_comm_info_list != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_enb_comm_info_list->num_peer_enb < 1) || (p_x2ap_enb_comm_info_list->num_peer_enb > MAX_PEER_ENB))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_enb_comm_info_list->num_peer_enb] should be in range [1 <= %d <= MAX_PEER_ENB] !", p_x2ap_enb_comm_info_list->num_peer_enb);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_enb_comm_info_list->num_peer_enb, "num_peer_enb");
    *pp_buffer += sizeof(p_x2ap_enb_comm_info_list->num_peer_enb);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_enb_comm_info_list->num_peer_enb; loop++)
        {
            if (RRC_FAILURE == rrc_compose_x2_enb_comm_info(pp_buffer, &p_x2ap_enb_comm_info_list->enb_comm_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_delete_enb_req_len
(
    x2ap_delete_enb_req_t *p_x2ap_delete_enb_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_delete_enb_req != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_delete_enb_req->bitmask);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_delete_enb_req->gb_enb_id);

    return length;
}

rrc_return_et
rrc_compose_x2ap_delete_enb_req
(
    U8  **pp_buffer,
    x2ap_delete_enb_req_t *p_x2ap_delete_enb_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_delete_enb_req != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_delete_enb_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_delete_enb_req->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_delete_enb_req->gb_enb_id))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_delete_enb_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_DEL_ENB_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_delete_enb_req
(
    x2ap_delete_enb_req_t  *p_x2ap_delete_enb_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_delete_enb_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_delete_enb_req_len(p_x2ap_delete_enb_req);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_DEL_ENB_REQ", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_DEL_ENB_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_DEL_ENB_REQ, msg_length, cell_index);

    /* Fill X2AP_DEL_ENB_REQ message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_delete_enb_req(&p_x2apCommon_msg, p_x2ap_delete_enb_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_delete_enb_res_len
(
    x2ap_delete_enb_res_t *p_x2ap_delete_enb_res
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_delete_enb_res != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_delete_enb_res->bitmask);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_delete_enb_res->enb1_gb_id);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_delete_enb_res->enb2_gb_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_delete_enb_res->response);

    return length;
}

rrc_return_et
rrc_compose_x2ap_delete_enb_res
(
    U8  **pp_buffer,
    x2ap_delete_enb_res_t *p_x2ap_delete_enb_res
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_delete_enb_res != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_delete_enb_res->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_delete_enb_res->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_delete_enb_res->enb1_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_delete_enb_res->enb2_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_delete_enb_res->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_delete_enb_res->response] should be in range [%d <= 1] !", p_x2ap_delete_enb_res->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_delete_enb_res->response, "response");
    *pp_buffer += sizeof(p_x2ap_delete_enb_res->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_delete_enb_res
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_DEL_ENB_RES message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_delete_enb_res
(
    x2ap_delete_enb_res_t  *p_x2ap_delete_enb_res,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_delete_enb_res != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_delete_enb_res_len(p_x2ap_delete_enb_res);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_DEL_ENB_RES", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_DEL_ENB_RES, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_DEL_ENB_RES, msg_length, cell_index);

    /* Fill X2AP_DEL_ENB_RES message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_delete_enb_res(&p_x2apCommon_msg, p_x2ap_delete_enb_res))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_enb_link_up_ind_len
(
    x2ap_enb_link_up_ind_t *p_x2ap_enb_link_up_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_enb_link_up_ind != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_enb_link_up_ind->bitmask);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_enb_link_up_ind->enb1_gb_id);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_enb_link_up_ind->enb2_gb_id);

    /* Get length of IE */
    length += rrc_get_x2_enb_comm_info_len(&p_x2ap_enb_link_up_ind->enb_comm_info);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_enb_link_up_ind->num_served_cell);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_enb_link_up_ind->num_served_cell < 1) || (p_x2ap_enb_link_up_ind->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_enb_link_up_ind->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_enb_link_up_ind->num_served_cell);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_enb_link_up_ind->num_served_cell; loop++)
        {
            length += rrc_get_x2ap_served_cell_info_arr_len(&p_x2ap_enb_link_up_ind->served_cell_info[loop]);
        }
    }

    /* Optional element */
    if(p_x2ap_enb_link_up_ind->bitmask & X2AP_GU_GROUP_ID_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_gu_group_id_list_len(&p_x2ap_enb_link_up_ind->gu_group_id);
    }

    return length;
}

rrc_return_et
rrc_compose_x2ap_enb_link_up_ind
(
    U8  **pp_buffer,
    x2ap_enb_link_up_ind_t *p_x2ap_enb_link_up_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_enb_link_up_ind != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_enb_link_up_ind->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_enb_link_up_ind->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_enb_link_up_ind->enb1_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_enb_link_up_ind->enb2_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_enb_comm_info(pp_buffer, &p_x2ap_enb_link_up_ind->enb_comm_info))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_enb_link_up_ind->num_served_cell < 1) || (p_x2ap_enb_link_up_ind->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_enb_link_up_ind->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_enb_link_up_ind->num_served_cell);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_enb_link_up_ind->num_served_cell, "num_served_cell");
    *pp_buffer += sizeof(p_x2ap_enb_link_up_ind->num_served_cell);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_enb_link_up_ind->num_served_cell; loop++)
        {
            if (RRC_FAILURE == rrc_compose_x2ap_served_cell_info_arr(pp_buffer, &p_x2ap_enb_link_up_ind->served_cell_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Optional element */
    if(p_x2ap_enb_link_up_ind->bitmask & X2AP_GU_GROUP_ID_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_gu_group_id_list(pp_buffer, &p_x2ap_enb_link_up_ind->gu_group_id))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_enb_link_up_ind
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_ENB_LINK_UP_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_enb_link_up_ind
(
    x2ap_enb_link_up_ind_t  *p_x2ap_enb_link_up_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_enb_link_up_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_enb_link_up_ind_len(p_x2ap_enb_link_up_ind);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_ENB_LINK_UP_IND", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_ENB_LINK_UP_IND, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_ENB_LINK_UP_IND, msg_length, cell_index);

    /* Fill X2AP_ENB_LINK_UP_IND message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_enb_link_up_ind(&p_x2apCommon_msg, p_x2ap_enb_link_up_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_enb_link_down_ind_len
(
    x2ap_enb_link_down_ind_t *p_x2ap_enb_link_down_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_enb_link_down_ind != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_enb_link_down_ind->bitmask);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_enb_link_down_ind->enb1_gb_id);

    /* Optional element */
    if(p_x2ap_enb_link_down_ind->bitmask & X2AP_PEER_ENB_ID_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_enb_link_down_ind->enb2_gb_id);
    }

    /* Optional element */
    if(p_x2ap_enb_link_down_ind->bitmask & X2AP_PEER_ENB_COMM_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2_enb_comm_info_len(&p_x2ap_enb_link_down_ind->enb_comm_info);
    }

    return length;
}

rrc_return_et
rrc_compose_x2ap_enb_link_down_ind
(
    U8  **pp_buffer,
    x2ap_enb_link_down_ind_t *p_x2ap_enb_link_down_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_enb_link_down_ind != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_enb_link_down_ind->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_enb_link_down_ind->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_enb_link_down_ind->enb1_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_x2ap_enb_link_down_ind->bitmask & X2AP_PEER_ENB_ID_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_enb_link_down_ind->enb2_gb_id))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_enb_link_down_ind->bitmask & X2AP_PEER_ENB_COMM_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_enb_comm_info(pp_buffer, &p_x2ap_enb_link_down_ind->enb_comm_info))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_enb_link_down_ind
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_ENB_LINK_DOWN_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_enb_link_down_ind
(
    x2ap_enb_link_down_ind_t  *p_x2ap_enb_link_down_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_enb_link_down_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_enb_link_down_ind_len(p_x2ap_enb_link_down_ind);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_ENB_LINK_DOWN_IND", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_ENB_LINK_DOWN_IND, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_ENB_LINK_DOWN_IND, msg_length, cell_index);

    /* Fill X2AP_ENB_LINK_DOWN_IND message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_enb_link_down_ind(&p_x2apCommon_msg, p_x2ap_enb_link_down_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_enb_config_update_wait_ind_len
(
    x2ap_enb_config_update_wait_ind_t *p_x2ap_enb_config_update_wait_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_enb_config_update_wait_ind != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_enb_config_update_wait_ind->bitmask);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_enb_config_update_wait_ind->enb1_gb_id);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_enb_config_update_wait_ind->enb2_gb_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_enb_config_update_wait_ind->time_to_wait);

    return length;
}

rrc_return_et
rrc_compose_x2ap_enb_config_update_wait_ind
(
    U8  **pp_buffer,
    x2ap_enb_config_update_wait_ind_t *p_x2ap_enb_config_update_wait_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_enb_config_update_wait_ind != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_enb_config_update_wait_ind->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_enb_config_update_wait_ind->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_enb_config_update_wait_ind->enb1_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_enb_config_update_wait_ind->enb2_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_enb_config_update_wait_ind->time_to_wait > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_enb_config_update_wait_ind->time_to_wait] should be in range [%d <= 2] !", p_x2ap_enb_config_update_wait_ind->time_to_wait);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_enb_config_update_wait_ind->time_to_wait, "time_to_wait");
    *pp_buffer += sizeof(p_x2ap_enb_config_update_wait_ind->time_to_wait);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_enb_config_update_wait_ind
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_ENB_CONFIG_UPDATE_WAIT_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_enb_config_update_wait_ind
(
    x2ap_enb_config_update_wait_ind_t  *p_x2ap_enb_config_update_wait_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_enb_config_update_wait_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_enb_config_update_wait_ind_len(p_x2ap_enb_config_update_wait_ind);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_ENB_CONFIG_UPDATE_WAIT_IND", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_ENB_CONFIG_UPDATE_WAIT_IND, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_ENB_CONFIG_UPDATE_WAIT_IND, msg_length, cell_index);

    /* Fill X2AP_ENB_CONFIG_UPDATE_WAIT_IND message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_enb_config_update_wait_ind(&p_x2apCommon_msg, p_x2ap_enb_config_update_wait_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_ho_report_ind_len
(
    x2ap_ho_report_ind_t *p_x2ap_ho_report_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_ho_report_ind != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_ho_report_ind->bitmask);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_ho_report_ind->enb1_gb_id);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_ho_report_ind->enb2_gb_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_ho_report_ind->ho_report_type);

    /* Get length of IE */
    length += rrc_get_x2ap_cause_len(&p_x2ap_ho_report_ind->ho_cause);

    /* Get length of IE */
    length += rrc_get_x2ap_ecgi_len(&p_x2ap_ho_report_ind->src_ecgi);

    /* Get length of IE */
    length += rrc_get_x2ap_ecgi_len(&p_x2ap_ho_report_ind->fail_cell_ecgi);

    /* Optional element */
    if(p_x2ap_ho_report_ind->bitmask & X2AP_HO_REPORT_REEST_CELL_ECGI_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_ecgi_len(&p_x2ap_ho_report_ind->re_est_cell_ecgi);
    }

    return length;
}

rrc_return_et
rrc_compose_x2ap_ho_report_ind
(
    U8  **pp_buffer,
    x2ap_ho_report_ind_t *p_x2ap_ho_report_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_ho_report_ind != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_ho_report_ind->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_ho_report_ind->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_ho_report_ind->enb1_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_ho_report_ind->enb2_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_ho_report_ind->ho_report_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_ho_report_ind->ho_report_type] should be in range [%d <= 1] !", p_x2ap_ho_report_ind->ho_report_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_x2ap_ho_report_ind->ho_report_type, "ho_report_type");
    *pp_buffer += sizeof(p_x2ap_ho_report_ind->ho_report_type);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_cause(pp_buffer, &p_x2ap_ho_report_ind->ho_cause))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_ecgi(pp_buffer, &p_x2ap_ho_report_ind->src_ecgi))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_ecgi(pp_buffer, &p_x2ap_ho_report_ind->fail_cell_ecgi))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_x2ap_ho_report_ind->bitmask & X2AP_HO_REPORT_REEST_CELL_ECGI_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_ecgi(pp_buffer, &p_x2ap_ho_report_ind->re_est_cell_ecgi))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_ho_report_ind
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_HO_REPORT_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_ho_report_ind
(
    x2ap_ho_report_ind_t  *p_x2ap_ho_report_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_ho_report_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_ho_report_ind_len(p_x2ap_ho_report_ind);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_HO_REPORT_IND", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_HO_REPORT_IND, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_HO_REPORT_IND, msg_length, cell_index);

    /* Fill X2AP_HO_REPORT_IND message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_ho_report_ind(&p_x2apCommon_msg, p_x2ap_ho_report_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_peer_cell_activation_req_len
(
    x2ap_peer_cell_activation_req_t *p_x2ap_peer_cell_activation_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_peer_cell_activation_req != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_peer_cell_activation_req->bitmask);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_peer_cell_activation_req->gb_enb_id);

    /* Get length of IE */
    length += rrc_get_x2ap_cell_activation_info_len(&p_x2ap_peer_cell_activation_req->peer_cell_activation_info);

    return length;
}

rrc_return_et
rrc_compose_x2ap_peer_cell_activation_req
(
    U8  **pp_buffer,
    x2ap_peer_cell_activation_req_t *p_x2ap_peer_cell_activation_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_peer_cell_activation_req != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_peer_cell_activation_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_peer_cell_activation_req->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_peer_cell_activation_req->gb_enb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_cell_activation_info(pp_buffer, &p_x2ap_peer_cell_activation_req->peer_cell_activation_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_peer_cell_activation_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_PEER_CELL_ACTIVATION_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_peer_cell_activation_req
(
    x2ap_peer_cell_activation_req_t  *p_x2ap_peer_cell_activation_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_peer_cell_activation_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_peer_cell_activation_req_len(p_x2ap_peer_cell_activation_req);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_PEER_CELL_ACTIVATION_REQ", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_PEER_CELL_ACTIVATION_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_PEER_CELL_ACTIVATION_REQ, msg_length, cell_index);

    /* Fill X2AP_PEER_CELL_ACTIVATION_REQ message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_peer_cell_activation_req(&p_x2apCommon_msg, p_x2ap_peer_cell_activation_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_cell_activation_info_len
(
    x2ap_cell_activation_info_t *p_x2ap_cell_activation_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_cell_activation_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_cell_activation_info->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_cell_activation_info->num_served_cell);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_cell_activation_info->num_served_cell < 1) || (p_x2ap_cell_activation_info->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_cell_activation_info->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_cell_activation_info->num_served_cell);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_cell_activation_info->num_served_cell; loop++)
        {
            length += rrc_get_x2ap_ecgi_len(&p_x2ap_cell_activation_info->served_cell_list[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_cell_activation_info
(
    U8  **pp_buffer,
    x2ap_cell_activation_info_t *p_x2ap_cell_activation_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_cell_activation_info != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_cell_activation_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_cell_activation_info->bitmask);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_cell_activation_info->num_served_cell < 1) || (p_x2ap_cell_activation_info->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_cell_activation_info->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_cell_activation_info->num_served_cell);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_cell_activation_info->num_served_cell, "num_served_cell");
    *pp_buffer += sizeof(p_x2ap_cell_activation_info->num_served_cell);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_cell_activation_info->num_served_cell; loop++)
        {
            if (RRC_FAILURE == rrc_compose_x2ap_ecgi(pp_buffer, &p_x2ap_cell_activation_info->served_cell_list[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_peer_cell_activation_resp_len
(
    x2ap_peer_cell_activation_resp_t *p_x2ap_peer_cell_activation_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_peer_cell_activation_resp != PNULL);


    /* Get length of IE */
    length += rrc_get_x2ap_cell_activation_resp_len(&p_x2ap_peer_cell_activation_resp->peer_cell_activation_resp);

    return length;
}

rrc_return_et
rrc_compose_x2ap_peer_cell_activation_resp
(
    U8  **pp_buffer,
    x2ap_peer_cell_activation_resp_t *p_x2ap_peer_cell_activation_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_peer_cell_activation_resp != PNULL);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_cell_activation_resp(pp_buffer, &p_x2ap_peer_cell_activation_resp->peer_cell_activation_resp))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_peer_cell_activation_resp
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_PEER_CELL_ACTIVATION_RESP message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_peer_cell_activation_resp
(
    x2ap_peer_cell_activation_resp_t  *p_x2ap_peer_cell_activation_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_peer_cell_activation_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_peer_cell_activation_resp_len(p_x2ap_peer_cell_activation_resp);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_PEER_CELL_ACTIVATION_RESP", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_PEER_CELL_ACTIVATION_RESP, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_PEER_CELL_ACTIVATION_RESP, msg_length, cell_index);

    /* Fill X2AP_PEER_CELL_ACTIVATION_RESP message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_peer_cell_activation_resp(&p_x2apCommon_msg, p_x2ap_peer_cell_activation_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_cell_activation_resp_len
(
    x2ap_cell_activation_resp_t *p_x2ap_cell_activation_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_cell_activation_resp != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_cell_activation_resp->bitmask);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_cell_activation_resp->enb1_gb_id);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_cell_activation_resp->enb2_gb_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_cell_activation_resp->response);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_cell_activation_resp->num_served_cell);

    /* Optional element */
    if(p_x2ap_cell_activation_resp->bitmask & X2AP_CELL_ACTIVATION_SERVED_CELL_LIST_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_cell_activation_resp->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_cell_activation_resp->num_served_cell] should be in range [%d <= MAX_SERVED_CELLS] !", p_x2ap_cell_activation_resp->num_served_cell);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_cell_activation_resp->num_served_cell; loop++)
        {
            length += rrc_get_x2ap_ecgi_len(&p_x2ap_cell_activation_resp->served_cell_list[loop]);
        }
    }
    }

    /* Optional element */
    if(p_x2ap_cell_activation_resp->bitmask & X2AP_CELL_ACTIVATION_CAUSE_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_cause_len(&p_x2ap_cell_activation_resp->cause);
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_cell_activation_resp
(
    U8  **pp_buffer,
    x2ap_cell_activation_resp_t *p_x2ap_cell_activation_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_cell_activation_resp != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_cell_activation_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_cell_activation_resp->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_cell_activation_resp->enb1_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_cell_activation_resp->enb2_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_cell_activation_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_cell_activation_resp->response] should be in range [%d <= 1] !", p_x2ap_cell_activation_resp->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_cell_activation_resp->response, "response");
    *pp_buffer += sizeof(p_x2ap_cell_activation_resp->response);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_cell_activation_resp->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_cell_activation_resp->num_served_cell] should be in range [%d <= MAX_SERVED_CELLS] !", p_x2ap_cell_activation_resp->num_served_cell);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_cell_activation_resp->num_served_cell, "num_served_cell");
    *pp_buffer += sizeof(p_x2ap_cell_activation_resp->num_served_cell);

    /* Optional element */
    if(p_x2ap_cell_activation_resp->bitmask & X2AP_CELL_ACTIVATION_SERVED_CELL_LIST_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_cell_activation_resp->num_served_cell; loop++)
        {
            if (RRC_FAILURE == rrc_compose_x2ap_ecgi(pp_buffer, &p_x2ap_cell_activation_resp->served_cell_list[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    }

    /* Optional element */
    if(p_x2ap_cell_activation_resp->bitmask & X2AP_CELL_ACTIVATION_CAUSE_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_cause(pp_buffer, &p_x2ap_cell_activation_resp->cause))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_cell_activation_req_len
(
    x2ap_cell_activation_req_t *p_x2ap_cell_activation_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_cell_activation_req != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_cell_activation_req->bitmask);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_cell_activation_req->enb1_gb_id);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_cell_activation_req->enb2_gb_id);

    /* Get length of IE */
    length += rrc_get_x2ap_cell_activation_info_len(&p_x2ap_cell_activation_req->cell_activation_info);

    return length;
}

rrc_return_et
rrc_compose_x2ap_cell_activation_req
(
    U8  **pp_buffer,
    x2ap_cell_activation_req_t *p_x2ap_cell_activation_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_cell_activation_req != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_cell_activation_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_cell_activation_req->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_cell_activation_req->enb1_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_cell_activation_req->enb2_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_cell_activation_info(pp_buffer, &p_x2ap_cell_activation_req->cell_activation_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_cell_activation_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_CELL_ACTIVATION_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_cell_activation_req
(
    x2ap_cell_activation_req_t  *p_x2ap_cell_activation_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_cell_activation_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_cell_activation_req_len(p_x2ap_cell_activation_req);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_CELL_ACTIVATION_REQ", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_CELL_ACTIVATION_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_CELL_ACTIVATION_REQ, msg_length, cell_index);

    /* Fill X2AP_CELL_ACTIVATION_REQ message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_cell_activation_req(&p_x2apCommon_msg, p_x2ap_cell_activation_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_cell_activation_response_len
(
    x2ap_cell_activation_response_t *p_x2ap_cell_activation_response
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_cell_activation_response != PNULL);


    /* Get length of IE */
    length += rrc_get_x2ap_cell_activation_resp_len(&p_x2ap_cell_activation_response->cell_activation_resp);

    return length;
}

rrc_return_et
rrc_compose_x2ap_cell_activation_response
(
    U8  **pp_buffer,
    x2ap_cell_activation_response_t *p_x2ap_cell_activation_response
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_cell_activation_response != PNULL);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_cell_activation_resp(pp_buffer, &p_x2ap_cell_activation_response->cell_activation_resp))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_cell_activation_response
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_CELL_ACTIVATION_RESP message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_cell_activation_response
(
    x2ap_cell_activation_response_t  *p_x2ap_cell_activation_response,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_cell_activation_response != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_cell_activation_response_len(p_x2ap_cell_activation_response);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_CELL_ACTIVATION_RESP", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_CELL_ACTIVATION_RESP, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_CELL_ACTIVATION_RESP, msg_length, cell_index);

    /* Fill X2AP_CELL_ACTIVATION_RESP message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_cell_activation_response(&p_x2apCommon_msg, p_x2ap_cell_activation_response))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_mobility_change_req_len
(
    x2ap_mobility_change_req_t *p_x2ap_mobility_change_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_mobility_change_req != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_mobility_change_req->bitmask);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_mobility_change_req->gb_enb_id);

    /* Get length of IE */
    length += rrc_get_x2ap_mobility_change_info_len(&p_x2ap_mobility_change_req->mobility_change_info);

    return length;
}

rrc_return_et
rrc_compose_x2ap_mobility_change_req
(
    U8  **pp_buffer,
    x2ap_mobility_change_req_t *p_x2ap_mobility_change_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_mobility_change_req != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_mobility_change_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_mobility_change_req->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_mobility_change_req->gb_enb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_mobility_change_info(pp_buffer, &p_x2ap_mobility_change_req->mobility_change_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_mobility_change_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_MOBILITY_CHANGE_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_mobility_change_req
(
    x2ap_mobility_change_req_t  *p_x2ap_mobility_change_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_mobility_change_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_mobility_change_req_len(p_x2ap_mobility_change_req);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_MOBILITY_CHANGE_REQ", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_MOBILITY_CHANGE_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_MOBILITY_CHANGE_REQ, msg_length, cell_index);

    /* Fill X2AP_MOBILITY_CHANGE_REQ message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_mobility_change_req(&p_x2apCommon_msg, p_x2ap_mobility_change_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_mob_param_len
(
    x2ap_mob_param_t *p_x2ap_mob_param
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_mob_param != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_mob_param->handover_trigger_change);

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_mob_param
(
    U8  **pp_buffer,
    x2ap_mob_param_t *p_x2ap_mob_param
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_mob_param != PNULL);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_mob_param->handover_trigger_change < -20) || (p_x2ap_mob_param->handover_trigger_change > 20))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mob_param->handover_trigger_change] should be in range [-20 <= %d <= 20] !", p_x2ap_mob_param->handover_trigger_change);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_x2ap_mob_param->handover_trigger_change, "handover_trigger_change");
    *pp_buffer += sizeof(p_x2ap_mob_param->handover_trigger_change);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_mobility_change_info_len
(
    x2ap_mobility_change_info_t *p_x2ap_mobility_change_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_mobility_change_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_mobility_change_info->bitmask);

    /* Get length of IE */
    length += rrc_get_x2ap_ecgi_len(&p_x2ap_mobility_change_info->enb1_cell_id);

    /* Get length of IE */
    length += rrc_get_x2ap_ecgi_len(&p_x2ap_mobility_change_info->enb2_cell_id);

    /* Optional element */
    if(p_x2ap_mobility_change_info->bitmask & X2AP_ENB1_MOBILITY_PARAMETERS_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_mob_param_len(&p_x2ap_mobility_change_info->enb1_mob_param);
    }

    /* Get length of IE */
    length += rrc_get_x2ap_mob_param_len(&p_x2ap_mobility_change_info->enb2_mob_param);

    /* Get length of IE */
    length += rrc_get_x2ap_cause_len(&p_x2ap_mobility_change_info->cause);

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_mobility_change_info
(
    U8  **pp_buffer,
    x2ap_mobility_change_info_t *p_x2ap_mobility_change_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_mobility_change_info != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_mobility_change_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_mobility_change_info->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_ecgi(pp_buffer, &p_x2ap_mobility_change_info->enb1_cell_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_ecgi(pp_buffer, &p_x2ap_mobility_change_info->enb2_cell_id))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_x2ap_mobility_change_info->bitmask & X2AP_ENB1_MOBILITY_PARAMETERS_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_mob_param(pp_buffer, &p_x2ap_mobility_change_info->enb1_mob_param))
    {
        return RRC_FAILURE;
    }
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_mob_param(pp_buffer, &p_x2ap_mobility_change_info->enb2_mob_param))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_cause(pp_buffer, &p_x2ap_mobility_change_info->cause))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_mobility_change_resp_len
(
    x2ap_mobility_change_resp_t *p_x2ap_mobility_change_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_mobility_change_resp != PNULL);


    /* Get length of IE */
    length += rrc_get_x2ap_mob_ch_resp_len(&p_x2ap_mobility_change_resp->mobility_change_resp);

    return length;
}

rrc_return_et
rrc_compose_x2ap_mobility_change_resp
(
    U8  **pp_buffer,
    x2ap_mobility_change_resp_t *p_x2ap_mobility_change_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_mobility_change_resp != PNULL);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_mob_ch_resp(pp_buffer, &p_x2ap_mobility_change_resp->mobility_change_resp))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_mobility_change_resp
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_MOBILITY_CHANGE_RESP message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_mobility_change_resp
(
    x2ap_mobility_change_resp_t  *p_x2ap_mobility_change_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_mobility_change_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_mobility_change_resp_len(p_x2ap_mobility_change_resp);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_MOBILITY_CHANGE_RESP", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_MOBILITY_CHANGE_RESP, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_MOBILITY_CHANGE_RESP, msg_length, cell_index);

    /* Fill X2AP_MOBILITY_CHANGE_RESP message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_mobility_change_resp(&p_x2apCommon_msg, p_x2ap_mobility_change_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_mob_range_len
(
    x2ap_mob_range_t *p_x2ap_mob_range
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_mob_range != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_mob_range->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_mob_range->ho_trigger_change_lower_limit);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_mob_range->ho_trigger_change_upper_limit);

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_mob_range
(
    U8  **pp_buffer,
    x2ap_mob_range_t *p_x2ap_mob_range
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_mob_range != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_mob_range->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_mob_range->bitmask);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_mob_range->ho_trigger_change_lower_limit < -20) || (p_x2ap_mob_range->ho_trigger_change_lower_limit > 20))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mob_range->ho_trigger_change_lower_limit] should be in range [-20 <= %d <= 20] !", p_x2ap_mob_range->ho_trigger_change_lower_limit);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_x2ap_mob_range->ho_trigger_change_lower_limit, "ho_trigger_change_lower_limit");
    *pp_buffer += sizeof(p_x2ap_mob_range->ho_trigger_change_lower_limit);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_mob_range->ho_trigger_change_upper_limit < -20) || (p_x2ap_mob_range->ho_trigger_change_upper_limit > 20))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mob_range->ho_trigger_change_upper_limit] should be in range [-20 <= %d <= 20] !", p_x2ap_mob_range->ho_trigger_change_upper_limit);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_S8(*pp_buffer, &p_x2ap_mob_range->ho_trigger_change_upper_limit, "ho_trigger_change_upper_limit");
    *pp_buffer += sizeof(p_x2ap_mob_range->ho_trigger_change_upper_limit);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_get_x2ap_mob_ch_resp_len
(
    x2ap_mob_ch_resp_t *p_x2ap_mob_ch_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_mob_ch_resp != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_mob_ch_resp->bitmask);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_mob_ch_resp->enb1_gb_id);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_mob_ch_resp->enb2_gb_id);

    /* Get length of IE */
    length += rrc_get_x2ap_ecgi_len(&p_x2ap_mob_ch_resp->enb1_cell_id);

    /* Get length of IE */
    length += rrc_get_x2ap_ecgi_len(&p_x2ap_mob_ch_resp->enb2_cell_id);
    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_mob_ch_resp->response);

    /* Optional element */
    if(p_x2ap_mob_ch_resp->bitmask & X2AP_MOBILITY_SETTING_CAUSE_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_cause_len(&p_x2ap_mob_ch_resp->cause);
    }

    /* Optional element */
    if(p_x2ap_mob_ch_resp->bitmask & X2AP_MOBILITY_PARAM_MOD_RANGE_PRESENT)
    {

    /* Get length of IE */
    length += rrc_get_x2ap_mob_range_len(&p_x2ap_mob_ch_resp->enb2_mob_param_mod_range);
    }

    return length;
}

static
rrc_return_et
rrc_compose_x2ap_mob_ch_resp
(
    U8  **pp_buffer,
    x2ap_mob_ch_resp_t *p_x2ap_mob_ch_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_mob_ch_resp != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_mob_ch_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_mob_ch_resp->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_mob_ch_resp->enb1_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_mob_ch_resp->enb2_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_ecgi(pp_buffer, &p_x2ap_mob_ch_resp->enb1_cell_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_ecgi(pp_buffer, &p_x2ap_mob_ch_resp->enb2_cell_id))
    {
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2ap_mob_ch_resp->response, "response");
    *pp_buffer += sizeof(p_x2ap_mob_ch_resp->response);

    /* Optional element */
    if(p_x2ap_mob_ch_resp->bitmask & X2AP_MOBILITY_SETTING_CAUSE_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_cause(pp_buffer, &p_x2ap_mob_ch_resp->cause))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_mob_ch_resp->bitmask & X2AP_MOBILITY_PARAM_MOD_RANGE_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_mob_range(pp_buffer, &p_x2ap_mob_ch_resp->enb2_mob_param_mod_range))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_mobility_setting_change_req_len
(
    x2ap_mobility_setting_change_req_t *p_x2ap_mobility_setting_change_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_mobility_setting_change_req != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_x2ap_mobility_setting_change_req->bitmask);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_mobility_setting_change_req->enb1_gb_id);

    /* Get length of IE */
    length += rrc_get_x2_gb_enb_id_len(&p_x2ap_mobility_setting_change_req->enb2_gb_id);

    /* Get length of IE */
    length += rrc_get_x2ap_mobility_change_info_len(&p_x2ap_mobility_setting_change_req->mobility_change_info);

    return length;
}

rrc_return_et
rrc_compose_x2ap_mobility_setting_change_req
(
    U8  **pp_buffer,
    x2ap_mobility_setting_change_req_t *p_x2ap_mobility_setting_change_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_mobility_setting_change_req != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2ap_mobility_setting_change_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_mobility_setting_change_req->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_mobility_setting_change_req->enb1_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_mobility_setting_change_req->enb2_gb_id))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_mobility_change_info(pp_buffer, &p_x2ap_mobility_setting_change_req->mobility_change_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_mobility_setting_change_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_MOBILITY_SETTING_CHANGE_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_mobility_setting_change_req
(
    x2ap_mobility_setting_change_req_t  *p_x2ap_mobility_setting_change_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_mobility_setting_change_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_mobility_setting_change_req_len(p_x2ap_mobility_setting_change_req);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_MOBILITY_SETTING_CHANGE_REQ", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_MOBILITY_SETTING_CHANGE_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_MOBILITY_SETTING_CHANGE_REQ, msg_length, cell_index);

    /* Fill X2AP_MOBILITY_SETTING_CHANGE_REQ message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_mobility_setting_change_req(&p_x2apCommon_msg, p_x2ap_mobility_setting_change_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_x2ap_mobility_setting_change_resp_len
(
    x2ap_mobility_setting_change_resp_t *p_x2ap_mobility_setting_change_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_mobility_setting_change_resp != PNULL);


    /* Get length of IE */
    length += rrc_get_x2ap_mob_ch_resp_len(&p_x2ap_mobility_setting_change_resp->mobility_setting_change_resp);

    return length;
}

rrc_return_et
rrc_compose_x2ap_mobility_setting_change_resp
(
    U8  **pp_buffer,
    x2ap_mobility_setting_change_resp_t *p_x2ap_mobility_setting_change_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_mobility_setting_change_resp != PNULL);

    /* Compose IE */
    if (RRC_FAILURE == rrc_compose_x2ap_mob_ch_resp(pp_buffer, &p_x2ap_mobility_setting_change_resp->mobility_setting_change_resp))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apCommon_send_x2ap_mobility_setting_change_resp
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_MOBILITY_SETTING_CHANGE_RESP message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apCommon_send_x2ap_mobility_setting_change_resp
(
    x2ap_mobility_setting_change_resp_t  *p_x2ap_mobility_setting_change_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apCommon_msg;

    RRC_ASSERT(p_x2ap_mobility_setting_change_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_x2ap_mobility_setting_change_resp_len(p_x2ap_mobility_setting_change_resp);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_MOBILITY_SETTING_CHANGE_RESP", src_module_id, dest_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        RRC_TRACE(RRC_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, X2AP_MOBILITY_SETTING_CHANGE_RESP, msg_api_length);

    /* Fill interface header */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apCommon_msg, transaction_id, src_module_id,
        dest_module_id, X2AP_MOBILITY_SETTING_CHANGE_RESP, msg_length, cell_index);

    /* Fill X2AP_MOBILITY_SETTING_CHANGE_RESP message */
    p_x2apCommon_msg = p_x2apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_x2ap_mobility_setting_change_resp(&p_x2apCommon_msg, p_x2ap_mobility_setting_change_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

