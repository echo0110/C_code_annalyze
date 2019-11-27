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
 *  File Description : The file rrc_x2apCommon_il_parser.c contains the definitions 
 *                     of rrc-x2apCommon interface message parsing functions.
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
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_return_et
rrc_parse_x2ap_plmn_identity
(
    x2ap_plmn_identity_t *p_x2ap_plmn_identity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_macro_enb_id
(
    macro_enb_id_t *p_macro_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_home_enb_id
(
    home_enb_id_t *p_home_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2_enb_id_info
(
    x2_enb_id_info_t *p_x2_enb_id_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2_gb_enb_id
(
    x2_gb_enb_id_t *p_x2_gb_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_broadcast_plmn
(
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_earfcn
(
    x2ap_earfcn_t *p_x2ap_earfcn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_fdd_info
(
    x2ap_fdd_info_t *p_x2ap_fdd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_special_subframe_info
(
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_tdd_info
(
    x2ap_tdd_info_t *p_x2ap_tdd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_choice_eutra_mode
(
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_choice_eutra_mode_info
(
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_prach_configuration_info
(
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_subframe_allocation
(
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_mbsfn_info
(
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_mbsfn_subframe_info
(
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_served_cell_info
(
    x2ap_served_cell_info_t *p_x2ap_served_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_ecgi
(
    x2ap_ecgi_t *p_x2ap_ecgi,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_nbour_info
(
    x2ap_nbour_info_t *p_x2ap_nbour_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_nbour_cell_info
(
    x2ap_nbour_cell_info_t *p_x2ap_nbour_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_served_cell_info_arr
(
    x2ap_served_cell_info_arr_t *p_x2ap_served_cell_info_arr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_served_cells_to_add_list
(
    x2ap_served_cells_to_add_list_t *p_x2ap_served_cells_to_add_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_served_cells_to_modify_info
(
    x2ap_served_cells_to_modify_info_t *p_x2ap_served_cells_to_modify_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_served_cells_to_modify_list
(
    x2ap_served_cells_to_modify_list_t *p_x2ap_served_cells_to_modify_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_served_cells_to_delete_list
(
    x2ap_served_cells_to_delete_list_t *p_x2ap_served_cells_to_delete_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_gu_group_id
(
    x2ap_gu_group_id_t *p_x2ap_gu_group_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_gu_group_id_list
(
    x2ap_gu_group_id_list_t *p_x2ap_gu_group_id_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_neigh_ecgi_enb_id_map_info
(
    x2ap_neigh_ecgi_enb_id_map_info_t *p_x2ap_neigh_ecgi_enb_id_map_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_neigh_ecgi_enb_id_map_list
(
    x2ap_neigh_ecgi_enb_id_map_list_t *p_x2ap_neigh_ecgi_enb_id_map_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_cause
(
    x2ap_cause_t *p_x2ap_cause,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2_ip_addr
(
    x2_ip_addr_t *p_x2_ip_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2_ipv6_addr
(
    x2_ipv6_addr_t *p_x2_ipv6_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2_trans_layer_addrs
(
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2_extend_trans_layer_addrs
(
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2_enb_comm_info
(
    x2_enb_comm_info_t *p_x2_enb_comm_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_enb_comm_info_list
(
    x2ap_enb_comm_info_list_t *p_x2ap_enb_comm_info_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_cell_activation_info
(
    x2ap_cell_activation_info_t *p_x2ap_cell_activation_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_cell_activation_resp
(
    x2ap_cell_activation_resp_t *p_x2ap_cell_activation_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_mob_param
(
    x2ap_mob_param_t *p_x2ap_mob_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_mobility_change_info
(
    x2ap_mobility_change_info_t *p_x2ap_mobility_change_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_mob_range
(
    x2ap_mob_range_t *p_x2ap_mob_range,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);

static
rrc_return_et
rrc_parse_x2ap_mob_ch_resp
(
    x2ap_mob_ch_resp_t *p_x2ap_mob_ch_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_return_et
rrc_parse_x2ap_enb_config_update_req
(
    x2ap_enb_config_update_req_t *p_x2ap_enb_config_update_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_enb_config_update_req, 0, sizeof(x2ap_enb_config_update_req_t));

    /* This function parses x2ap_enb_config_update_req */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_enb_config_update_req");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_enb_config_update_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_enb_config_update_req->num_peer_enb, p_src + *p_length_read, "num_peer_enb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_enb_config_update_req->num_peer_enb > MAX_PEER_ENB))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_enb_config_update_req->num_peer_enb] should be in range [%d <= MAX_PEER_ENB] !", p_x2ap_enb_config_update_req->num_peer_enb);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_enb_config_update_req->num_peer_enb; loop++)
        {
            if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
                &p_x2ap_enb_config_update_req->enb_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (p_x2ap_enb_config_update_req->bitmask & X2AP_SERVED_CELLS_TO_ADD_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_served_cells_to_add_list(
        &p_x2ap_enb_config_update_req->add_served_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_config_update_req->bitmask & X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_served_cells_to_modify_list(
        &p_x2ap_enb_config_update_req->mod_served_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_config_update_req->bitmask & X2AP_SERVED_CELLS_TO_DELETE_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_served_cells_to_delete_list(
        &p_x2ap_enb_config_update_req->del_served_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_config_update_req->bitmask & X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_gu_group_id_list(
        &p_x2ap_enb_config_update_req->add_gu_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_config_update_req->bitmask & X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_gu_group_id_list(
        &p_x2ap_enb_config_update_req->delete_gu_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_config_update_req->bitmask & X2AP_NEIGH_ECGI_ENB_ID_MAPPING_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_neigh_ecgi_enb_id_map_list(
        &p_x2ap_enb_config_update_req->neigh_ecgi_enb_id_map,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_plmn_identity
(
    x2ap_plmn_identity_t *p_x2ap_plmn_identity,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    memset_wrapper(p_x2ap_plmn_identity, 0, sizeof(x2ap_plmn_identity_t));

    /* This function parses x2ap_plmn_identity */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_plmn_identity");

    if (*p_length_read + (S32)sizeof(p_x2ap_plmn_identity->plmn_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_plmn_identity->plmn_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_plmn_identity->plmn_id[loop], (void*)(p_src + *p_length_read), "plmn_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_macro_enb_id
(
    macro_enb_id_t *p_macro_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    memset_wrapper(p_macro_enb_id, 0, sizeof(macro_enb_id_t));

    /* This function parses macro_enb_id */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing macro_enb_id");

    if (*p_length_read + (S32)sizeof(p_macro_enb_id->eNB_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_macro_enb_id->eNB_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_macro_enb_id->eNB_id[loop], (void*)(p_src + *p_length_read), "eNB_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_home_enb_id
(
    home_enb_id_t *p_home_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    memset_wrapper(p_home_enb_id, 0, sizeof(home_enb_id_t));

    /* This function parses home_enb_id */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing home_enb_id");

    if (*p_length_read + (S32)sizeof(p_home_enb_id->eNB_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_home_enb_id->eNB_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_home_enb_id->eNB_id[loop], (void*)(p_src + *p_length_read), "eNB_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2_enb_id_info
(
    x2_enb_id_info_t *p_x2_enb_id_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2_enb_id_info, 0, sizeof(x2_enb_id_info_t));

    /* This function parses x2_enb_id_info */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2_enb_id_info");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2_enb_id_info->presence_bitmask, p_src + *p_length_read, "presence_bitmask");
    *p_length_read += sizeof(U8);

    if (p_x2_enb_id_info->presence_bitmask & 1)
    {

    if (RRC_FAILURE == rrc_parse_macro_enb_id(
        &p_x2_enb_id_info->macro_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2_enb_id_info->presence_bitmask & 2)
    {

    if (RRC_FAILURE == rrc_parse_home_enb_id(
        &p_x2_enb_id_info->home_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2_gb_enb_id
(
    x2_gb_enb_id_t *p_x2_gb_enb_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2_gb_enb_id, 0, sizeof(x2_gb_enb_id_t));

    /* This function parses x2_gb_enb_id */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2_gb_enb_id");

    if (RRC_FAILURE == rrc_parse_x2ap_plmn_identity(
        &p_x2_gb_enb_id->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2_enb_id_info(
        &p_x2_gb_enb_id->enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_broadcast_plmn
(
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_broadcast_plmn, 0, sizeof(x2ap_broadcast_plmn_t));

    /* This function parses x2ap_broadcast_plmn */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_broadcast_plmn");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_broadcast_plmn->num_bcast_plmn_id, p_src + *p_length_read, "num_bcast_plmn_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_broadcast_plmn->num_bcast_plmn_id < 1) || (p_x2ap_broadcast_plmn->num_bcast_plmn_id > MAX_NUM_BPLMN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_broadcast_plmn->num_bcast_plmn_id] should be in range [1 <= %d <= MAX_NUM_BPLMN] !", p_x2ap_broadcast_plmn->num_bcast_plmn_id);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_broadcast_plmn->num_bcast_plmn_id; loop++)
        {
            if (RRC_FAILURE == rrc_parse_x2ap_plmn_identity(
                &p_x2ap_broadcast_plmn->plmn_identity[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_earfcn
(
    x2ap_earfcn_t *p_x2ap_earfcn,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    memset_wrapper(p_x2ap_earfcn, 0, sizeof(x2ap_earfcn_t));

    /* This function parses x2ap_earfcn */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_earfcn");

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_earfcn->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_earfcn->earfcn > MAX_EARFCN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_earfcn->earfcn] should be in range [%d <= MAX_EARFCN] !", p_x2ap_earfcn->earfcn);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_fdd_info
(
    x2ap_fdd_info_t *p_x2ap_fdd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_fdd_info, 0, sizeof(x2ap_fdd_info_t));

    /* This function parses x2ap_fdd_info */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_fdd_info");

    if (RRC_FAILURE == rrc_parse_x2ap_earfcn(
        &p_x2ap_fdd_info->ul_earfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2ap_earfcn(
        &p_x2ap_fdd_info->dl_earfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_fdd_info->ul_trnsmission_bdwidth, p_src + *p_length_read, "ul_trnsmission_bdwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_fdd_info->ul_trnsmission_bdwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_fdd_info->ul_trnsmission_bdwidth] should be in range [%d <= 5] !", p_x2ap_fdd_info->ul_trnsmission_bdwidth);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_fdd_info->dl_trnsmission_bdwidth, p_src + *p_length_read, "dl_trnsmission_bdwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_fdd_info->dl_trnsmission_bdwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_fdd_info->dl_trnsmission_bdwidth] should be in range [%d <= 5] !", p_x2ap_fdd_info->dl_trnsmission_bdwidth);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_special_subframe_info
(
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    memset_wrapper(p_x2ap_special_subframe_info, 0, sizeof(x2ap_special_subframe_info_t));

    /* This function parses x2ap_special_subframe_info */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_special_subframe_info");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_special_subframe_info->special_subframe_patterns, p_src + *p_length_read, "special_subframe_patterns");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->special_subframe_patterns > 8))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_special_subframe_info->special_subframe_patterns] should be in range [%d <= 8] !", p_x2ap_special_subframe_info->special_subframe_patterns);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_special_subframe_info->dl_cp, p_src + *p_length_read, "dl_cp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->dl_cp > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_special_subframe_info->dl_cp] should be in range [%d <= 1] !", p_x2ap_special_subframe_info->dl_cp);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_special_subframe_info->ul_cp, p_src + *p_length_read, "ul_cp");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->ul_cp > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_special_subframe_info->ul_cp] should be in range [%d <= 1] !", p_x2ap_special_subframe_info->ul_cp);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_tdd_info
(
    x2ap_tdd_info_t *p_x2ap_tdd_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_tdd_info, 0, sizeof(x2ap_tdd_info_t));

    /* This function parses x2ap_tdd_info */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_tdd_info");

    if (RRC_FAILURE == rrc_parse_x2ap_earfcn(
        &p_x2ap_tdd_info->earfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_tdd_info->trnsmission_bdwidth, p_src + *p_length_read, "trnsmission_bdwidth");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_tdd_info->trnsmission_bdwidth > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_tdd_info->trnsmission_bdwidth] should be in range [%d <= 5] !", p_x2ap_tdd_info->trnsmission_bdwidth);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_tdd_info->subframe_assn, p_src + *p_length_read, "subframe_assn");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_tdd_info->subframe_assn > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_tdd_info->subframe_assn] should be in range [%d <= 6] !", p_x2ap_tdd_info->subframe_assn);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_parse_x2ap_special_subframe_info(
        &p_x2ap_tdd_info->special_subframe_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_choice_eutra_mode
(
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_choice_eutra_mode, 0, sizeof(x2ap_choice_eutra_mode_t));

    /* This function parses x2ap_choice_eutra_mode */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_choice_eutra_mode");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_choice_eutra_mode->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_FDD)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_fdd_info(
        &p_x2ap_choice_eutra_mode->fdd_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_TDD)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_tdd_info(
        &p_x2ap_choice_eutra_mode->tdd_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_choice_eutra_mode_info
(
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_choice_eutra_mode_info, 0, sizeof(x2ap_choice_eutra_mode_info_t));

    /* This function parses x2ap_choice_eutra_mode_info */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_choice_eutra_mode_info");

    if (RRC_FAILURE == rrc_parse_x2ap_choice_eutra_mode(
        &p_x2ap_choice_eutra_mode_info->choice_eutra_mode,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_prach_configuration_info
(
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    memset_wrapper(p_x2ap_prach_configuration_info, 0, sizeof(x2ap_prach_configuration_info_t));

    /* This function parses x2ap_prach_configuration_info */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_prach_configuration_info");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_prach_configuration_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_prach_configuration_info->root_sequence_index, p_src + *p_length_read, "root_sequence_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->root_sequence_index > 837))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->root_sequence_index] should be in range [%d <= 837] !", p_x2ap_prach_configuration_info->root_sequence_index);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_prach_configuration_info->zero_correlation_zone_config, p_src + *p_length_read, "zero_correlation_zone_config");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->zero_correlation_zone_config > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->zero_correlation_zone_config] should be in range [%d <= 15] !", p_x2ap_prach_configuration_info->zero_correlation_zone_config);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_prach_configuration_info->high_speed_flag, p_src + *p_length_read, "high_speed_flag");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->high_speed_flag > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->high_speed_flag] should be in range [%d <= 1] !", p_x2ap_prach_configuration_info->high_speed_flag);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_prach_configuration_info->prach_frequency_offset, p_src + *p_length_read, "prach_frequency_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->prach_frequency_offset > 94))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->prach_frequency_offset] should be in range [%d <= 94] !", p_x2ap_prach_configuration_info->prach_frequency_offset);
        return RRC_FAILURE;
    }

    if (p_x2ap_prach_configuration_info->bitmask & X2AP_PRACH_CONFIG_INDEX_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_prach_configuration_info->prach_config_index, p_src + *p_length_read, "prach_config_index");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->prach_config_index > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_prach_configuration_info->prach_config_index] should be in range [%d <= 63] !", p_x2ap_prach_configuration_info->prach_config_index);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_subframe_allocation
(
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    memset_wrapper(p_x2ap_subframe_allocation, 0, sizeof(x2ap_subframe_allocation_t));

    /* This function parses x2ap_subframe_allocation */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_subframe_allocation");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_subframe_allocation->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_x2ap_subframe_allocation->oneframe) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_subframe_allocation->oneframe); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_subframe_allocation->oneframe[loop], (void*)(p_src + *p_length_read), "oneframe[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_x2ap_subframe_allocation->fourframe) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_subframe_allocation->fourframe); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_subframe_allocation->fourframe[loop], (void*)(p_src + *p_length_read), "fourframe[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_mbsfn_info
(
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_mbsfn_info, 0, sizeof(x2ap_mbsfn_info_t));

    /* This function parses x2ap_mbsfn_info */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_mbsfn_info");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_mbsfn_info->radioframe_alloc_period, p_src + *p_length_read, "radioframe_alloc_period");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_info->radioframe_alloc_period > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_info->radioframe_alloc_period] should be in range [%d <= 5] !", p_x2ap_mbsfn_info->radioframe_alloc_period);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_mbsfn_info->radioframe_alloc_offset, p_src + *p_length_read, "radioframe_alloc_offset");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_info->radioframe_alloc_offset > 7))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_info->radioframe_alloc_offset] should be in range [%d <= 7] !", p_x2ap_mbsfn_info->radioframe_alloc_offset);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_parse_x2ap_subframe_allocation(
        &p_x2ap_mbsfn_info->subframe_alloc,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_mbsfn_subframe_info
(
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_mbsfn_subframe_info, 0, sizeof(x2ap_mbsfn_subframe_info_t));

    /* This function parses x2ap_mbsfn_subframe_info */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_mbsfn_subframe_info");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_mbsfn_subframe_info->num_mbsfn, p_src + *p_length_read, "num_mbsfn");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_subframe_info->num_mbsfn > MAX_NUM_MBSFN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mbsfn_subframe_info->num_mbsfn] should be in range [%d <= MAX_NUM_MBSFN] !", p_x2ap_mbsfn_subframe_info->num_mbsfn);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_mbsfn_subframe_info->num_mbsfn; loop++)
        {
            if (RRC_FAILURE == rrc_parse_x2ap_mbsfn_info(
                &p_x2ap_mbsfn_subframe_info->mbsfn_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_served_cell_info
(
    x2ap_served_cell_info_t *p_x2ap_served_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_served_cell_info, 0, sizeof(x2ap_served_cell_info_t));

    /* This function parses x2ap_served_cell_info */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_served_cell_info");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_served_cell_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_served_cell_info->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_served_cell_info->pci > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cell_info->pci] should be in range [%d <= 503] !", p_x2ap_served_cell_info->pci);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_served_cell_info->cell_id, p_src + *p_length_read, "cell_id");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(p_x2ap_served_cell_info->tac) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_served_cell_info->tac); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_served_cell_info->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (RRC_FAILURE == rrc_parse_x2ap_broadcast_plmn(
        &p_x2ap_served_cell_info->bcast_plmn_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2ap_choice_eutra_mode_info(
        &p_x2ap_served_cell_info->choice_eutra_mode,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_served_cell_info->bitmask & X2AP_NUM_ANTENNA_INFO_PORT_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_served_cell_info->num_antenna_port, p_src + *p_length_read, "num_antenna_port");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_served_cell_info->num_antenna_port > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cell_info->num_antenna_port] should be in range [%d <= 2] !", p_x2ap_served_cell_info->num_antenna_port);
        return RRC_FAILURE;
    }
    }

    if (p_x2ap_served_cell_info->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_prach_configuration_info(
        &p_x2ap_served_cell_info->prach_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_served_cell_info->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_mbsfn_subframe_info(
        &p_x2ap_served_cell_info->mbsfn_subframe_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_served_cell_info->bitmask & X2AP_CSG_ID_INFO_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(p_x2ap_served_cell_info->csg_identity) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_served_cell_info->csg_identity); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_served_cell_info->csg_identity[loop], (void*)(p_src + *p_length_read), "csg_identity[]");
            *p_length_read += sizeof(U8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_ecgi
(
    x2ap_ecgi_t *p_x2ap_ecgi,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_ecgi, 0, sizeof(x2ap_ecgi_t));

    /* This function parses x2ap_ecgi */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_ecgi");

    if (RRC_FAILURE == rrc_parse_x2ap_plmn_identity(
        &p_x2ap_ecgi->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_x2ap_ecgi->eutran_cell_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_ecgi->eutran_cell_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_ecgi->eutran_cell_id[loop], (void*)(p_src + *p_length_read), "eutran_cell_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_nbour_info
(
    x2ap_nbour_info_t *p_x2ap_nbour_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_nbour_info, 0, sizeof(x2ap_nbour_info_t));

    /* This function parses x2ap_nbour_info */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_nbour_info");

    if (RRC_FAILURE == rrc_parse_x2ap_ecgi(
        &p_x2ap_nbour_info->ecgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_nbour_info->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nbour_info->pci > 503))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nbour_info->pci] should be in range [%d <= 503] !", p_x2ap_nbour_info->pci);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_parse_x2ap_earfcn(
        &p_x2ap_nbour_info->earfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_nbour_cell_info
(
    x2ap_nbour_cell_info_t *p_x2ap_nbour_cell_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_nbour_cell_info, 0, sizeof(x2ap_nbour_cell_info_t));

    /* This function parses x2ap_nbour_cell_info */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_nbour_cell_info");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_nbour_cell_info->num_neighbour_cell, p_src + *p_length_read, "num_neighbour_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nbour_cell_info->num_neighbour_cell > MAX_NEIGHBOURS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_nbour_cell_info->num_neighbour_cell] should be in range [%d <= MAX_NEIGHBOURS] !", p_x2ap_nbour_cell_info->num_neighbour_cell);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nbour_cell_info->num_neighbour_cell; loop++)
        {
            if (RRC_FAILURE == rrc_parse_x2ap_nbour_info(
                &p_x2ap_nbour_cell_info->neighbour_info_arr[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_served_cell_info_arr
(
    x2ap_served_cell_info_arr_t *p_x2ap_served_cell_info_arr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_served_cell_info_arr, 0, sizeof(x2ap_served_cell_info_arr_t));

    /* This function parses x2ap_served_cell_info_arr */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_served_cell_info_arr");

    if (RRC_FAILURE == rrc_parse_x2ap_served_cell_info(
        &p_x2ap_served_cell_info_arr->cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2ap_nbour_cell_info(
        &p_x2ap_served_cell_info_arr->nbour_cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_served_cells_to_add_list
(
    x2ap_served_cells_to_add_list_t *p_x2ap_served_cells_to_add_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_served_cells_to_add_list, 0, sizeof(x2ap_served_cells_to_add_list_t));

    /* This function parses x2ap_served_cells_to_add_list */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_served_cells_to_add_list");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_served_cells_to_add_list->num_served_cell, p_src + *p_length_read, "num_served_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_served_cells_to_add_list->num_served_cell < 1) || (p_x2ap_served_cells_to_add_list->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cells_to_add_list->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_served_cells_to_add_list->num_served_cell);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_served_cells_to_add_list->num_served_cell; loop++)
        {
            if (RRC_FAILURE == rrc_parse_x2ap_served_cell_info_arr(
                &p_x2ap_served_cells_to_add_list->cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_served_cells_to_modify_info
(
    x2ap_served_cells_to_modify_info_t *p_x2ap_served_cells_to_modify_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_served_cells_to_modify_info, 0, sizeof(x2ap_served_cells_to_modify_info_t));

    /* This function parses x2ap_served_cells_to_modify_info */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_served_cells_to_modify_info");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_served_cells_to_modify_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_served_cells_to_modify_info->old_cell_id, p_src + *p_length_read, "old_cell_id");
    *p_length_read += sizeof(U8);

    if (RRC_FAILURE == rrc_parse_x2ap_served_cell_info_arr(
        &p_x2ap_served_cells_to_modify_info->served_cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_served_cells_to_modify_info->bitmask & X2AP_DEACTIVATION_IND_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_served_cells_to_modify_info->deactivation_ind, p_src + *p_length_read, "deactivation_ind");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_served_cells_to_modify_info->deactivation_ind > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cells_to_modify_info->deactivation_ind] should be in range [%d <= 1] !", p_x2ap_served_cells_to_modify_info->deactivation_ind);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_served_cells_to_modify_list
(
    x2ap_served_cells_to_modify_list_t *p_x2ap_served_cells_to_modify_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_served_cells_to_modify_list, 0, sizeof(x2ap_served_cells_to_modify_list_t));

    /* This function parses x2ap_served_cells_to_modify_list */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_served_cells_to_modify_list");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_served_cells_to_modify_list->num_served_cell, p_src + *p_length_read, "num_served_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_served_cells_to_modify_list->num_served_cell < 1) || (p_x2ap_served_cells_to_modify_list->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cells_to_modify_list->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_served_cells_to_modify_list->num_served_cell);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_served_cells_to_modify_list->num_served_cell; loop++)
        {
            if (RRC_FAILURE == rrc_parse_x2ap_served_cells_to_modify_info(
                &p_x2ap_served_cells_to_modify_list->cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_served_cells_to_delete_list
(
    x2ap_served_cells_to_delete_list_t *p_x2ap_served_cells_to_delete_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    memset_wrapper(p_x2ap_served_cells_to_delete_list, 0, sizeof(x2ap_served_cells_to_delete_list_t));

    /* This function parses x2ap_served_cells_to_delete_list */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_served_cells_to_delete_list");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_served_cells_to_delete_list->num_served_cell, p_src + *p_length_read, "num_served_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_served_cells_to_delete_list->num_served_cell < 1) || (p_x2ap_served_cells_to_delete_list->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_served_cells_to_delete_list->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_served_cells_to_delete_list->num_served_cell);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_x2ap_served_cells_to_delete_list->num_served_cell * sizeof(p_x2ap_served_cells_to_delete_list->cell_id_list[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_served_cells_to_delete_list->num_served_cell; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_served_cells_to_delete_list->cell_id_list[loop], (void*)(p_src + *p_length_read), "cell_id_list[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_gu_group_id
(
    x2ap_gu_group_id_t *p_x2ap_gu_group_id,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_gu_group_id, 0, sizeof(x2ap_gu_group_id_t));

    /* This function parses x2ap_gu_group_id */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_gu_group_id");

    if (RRC_FAILURE == rrc_parse_x2ap_plmn_identity(
        &p_x2ap_gu_group_id->plmn_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(p_x2ap_gu_group_id->mme_group_id) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_gu_group_id->mme_group_id); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2ap_gu_group_id->mme_group_id[loop], (void*)(p_src + *p_length_read), "mme_group_id[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_gu_group_id_list
(
    x2ap_gu_group_id_list_t *p_x2ap_gu_group_id_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_gu_group_id_list, 0, sizeof(x2ap_gu_group_id_list_t));

    /* This function parses x2ap_gu_group_id_list */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_gu_group_id_list");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_gu_group_id_list->num_gu_group_id, p_src + *p_length_read, "num_gu_group_id");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_gu_group_id_list->num_gu_group_id < 1) || (p_x2ap_gu_group_id_list->num_gu_group_id > MAX_ENB_POOLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_gu_group_id_list->num_gu_group_id] should be in range [1 <= %d <= MAX_ENB_POOLS] !", p_x2ap_gu_group_id_list->num_gu_group_id);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_gu_group_id_list->num_gu_group_id; loop++)
        {
            if (RRC_FAILURE == rrc_parse_x2ap_gu_group_id(
                &p_x2ap_gu_group_id_list->gu_id_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_neigh_ecgi_enb_id_map_info
(
    x2ap_neigh_ecgi_enb_id_map_info_t *p_x2ap_neigh_ecgi_enb_id_map_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_neigh_ecgi_enb_id_map_info, 0, sizeof(x2ap_neigh_ecgi_enb_id_map_info_t));

    /* This function parses x2ap_neigh_ecgi_enb_id_map_info */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_neigh_ecgi_enb_id_map_info");

    if (RRC_FAILURE == rrc_parse_x2ap_ecgi(
        &p_x2ap_neigh_ecgi_enb_id_map_info->ecgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_neigh_ecgi_enb_id_map_info->enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_neigh_ecgi_enb_id_map_list
(
    x2ap_neigh_ecgi_enb_id_map_list_t *p_x2ap_neigh_ecgi_enb_id_map_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_neigh_ecgi_enb_id_map_list, 0, sizeof(x2ap_neigh_ecgi_enb_id_map_list_t));

    /* This function parses x2ap_neigh_ecgi_enb_id_map_list */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_neigh_ecgi_enb_id_map_list");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count, p_src + *p_length_read, "neigh_info_count");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count < 1) || (p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count > MAX_NEIGHBOURS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count] should be in range [1 <= %d <= MAX_NEIGHBOURS] !", p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count; loop++)
        {
            if (RRC_FAILURE == rrc_parse_x2ap_neigh_ecgi_enb_id_map_info(
                &p_x2ap_neigh_ecgi_enb_id_map_list->neigh_ecgi_enb_id_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_enb_config_update_resp
(
    x2ap_enb_config_update_resp_t *p_x2ap_enb_config_update_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_enb_config_update_resp, 0, sizeof(x2ap_enb_config_update_resp_t));

    /* This function parses x2ap_enb_config_update_resp */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_enb_config_update_resp");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_enb_config_update_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_enb_config_update_resp->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_enb_config_update_resp->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_enb_config_update_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_enb_config_update_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_enb_config_update_resp->response] should be in range [%d <= 1] !", p_x2ap_enb_config_update_resp->response);
        return RRC_FAILURE;
    }

    if (p_x2ap_enb_config_update_resp->bitmask & X2AP_CAUSE_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_cause(
        &p_x2ap_enb_config_update_resp->cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_cause
(
    x2ap_cause_t *p_x2ap_cause,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    memset_wrapper(p_x2ap_cause, 0, sizeof(x2ap_cause_t));

    /* This function parses x2ap_cause */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_cause");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_cause->type, p_src + *p_length_read, "type");
    *p_length_read += sizeof(U8);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_cause->value, p_src + *p_length_read, "value");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_enb_config_update_ind
(
    x2ap_enb_config_update_ind_t *p_x2ap_enb_config_update_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_enb_config_update_ind, 0, sizeof(x2ap_enb_config_update_ind_t));

    /* This function parses x2ap_enb_config_update_ind */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_enb_config_update_ind");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_enb_config_update_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_enb_config_update_ind->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_enb_config_update_ind->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_enb_config_update_ind->bitmask & X2AP_SERVED_CELLS_TO_ADD_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_served_cells_to_add_list(
        &p_x2ap_enb_config_update_ind->add_served_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_config_update_ind->bitmask & X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_served_cells_to_modify_list(
        &p_x2ap_enb_config_update_ind->mod_served_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_config_update_ind->bitmask & X2AP_SERVED_CELLS_TO_DELETE_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_served_cells_to_delete_list(
        &p_x2ap_enb_config_update_ind->del_served_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_config_update_ind->bitmask & X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_gu_group_id_list(
        &p_x2ap_enb_config_update_ind->add_gu_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_config_update_ind->bitmask & X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_gu_group_id_list(
        &p_x2ap_enb_config_update_ind->delete_gu_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_reset_req
(
    x2ap_reset_req_t *p_x2ap_reset_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_reset_req, 0, sizeof(x2ap_reset_req_t));

    /* This function parses x2ap_reset_req */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_reset_req");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_reset_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_reset_req->gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2ap_cause(
        &p_x2ap_reset_req->cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_reset_resp
(
    x2ap_reset_resp_t *p_x2ap_reset_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_reset_resp, 0, sizeof(x2ap_reset_resp_t));

    /* This function parses x2ap_reset_resp */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_reset_resp");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_reset_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_reset_resp->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_reset_resp->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_reset_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_reset_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_reset_resp->response] should be in range [%d <= 1] !", p_x2ap_reset_resp->response);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_reset_ind
(
    x2ap_reset_ind_t *p_x2ap_reset_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_reset_ind, 0, sizeof(x2ap_reset_ind_t));

    /* This function parses x2ap_reset_ind */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_reset_ind");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_reset_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_reset_ind->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_reset_ind->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2ap_cause(
        &p_x2ap_reset_ind->cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_add_enb_req
(
    x2ap_add_enb_req_t *p_x2ap_add_enb_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_add_enb_req, 0, sizeof(x2ap_add_enb_req_t));

    /* This function parses x2ap_add_enb_req */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_add_enb_req");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_add_enb_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_add_enb_req->num_peer_enb, p_src + *p_length_read, "num_peer_enb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_add_enb_req->num_peer_enb < 1) || (p_x2ap_add_enb_req->num_peer_enb > MAX_PEER_ENB))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_add_enb_req->num_peer_enb] should be in range [1 <= %d <= MAX_PEER_ENB] !", p_x2ap_add_enb_req->num_peer_enb);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_add_enb_req->num_peer_enb; loop++)
        {
            if (RRC_FAILURE == rrc_parse_x2_enb_comm_info(
                &p_x2ap_add_enb_req->enb_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2_ip_addr
(
    x2_ip_addr_t *p_x2_ip_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    memset_wrapper(p_x2_ip_addr, 0, sizeof(x2_ip_addr_t));

    /* This function parses x2_ip_addr */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2_ip_addr");

    if (*p_length_read + (S32)sizeof(p_x2_ip_addr->ip_addr) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2_ip_addr->ip_addr); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2_ip_addr->ip_addr[loop], (void*)(p_src + *p_length_read), "ip_addr[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2_ipv6_addr
(
    x2_ipv6_addr_t *p_x2_ipv6_addr,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    memset_wrapper(p_x2_ipv6_addr, 0, sizeof(x2_ipv6_addr_t));

    /* This function parses x2_ipv6_addr */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2_ipv6_addr");

    if (*p_length_read + (S32)sizeof(p_x2_ipv6_addr->ipv6_addr) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2_ipv6_addr->ipv6_addr); loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2_ipv6_addr->ipv6_addr[loop], (void*)(p_src + *p_length_read), "ipv6_addr[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2_trans_layer_addrs
(
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    memset_wrapper(p_x2_trans_layer_addrs, 0, sizeof(x2_trans_layer_addrs_t));

    /* This function parses x2_trans_layer_addrs */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2_trans_layer_addrs");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2_trans_layer_addrs->data_length, p_src + *p_length_read, "data_length");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2_trans_layer_addrs->data_length > RRC_MAX_IP_ADDR_LEN_IPV6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_trans_layer_addrs->data_length] should be in range [%d <= RRC_MAX_IP_ADDR_LEN_IPV6] !", p_x2_trans_layer_addrs->data_length);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)(p_x2_trans_layer_addrs->data_length * sizeof(p_x2_trans_layer_addrs->data[0])) > length_left)
    {
        RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_trans_layer_addrs->data_length; loop++)
        {
            rrc_cp_unpack_U8((void*)&p_x2_trans_layer_addrs->data[loop], (void*)(p_src + *p_length_read), "data[]");
            *p_length_read += sizeof(U8);
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2_extend_trans_layer_addrs
(
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2_extend_trans_layer_addrs, 0, sizeof(x2_extend_trans_layer_addrs_t));

    /* This function parses x2_extend_trans_layer_addrs */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2_extend_trans_layer_addrs");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2_extend_trans_layer_addrs->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_IPSEC_ADDRS_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2_trans_layer_addrs(
        &p_x2_extend_trans_layer_addrs->ipsec_trans_layer_addrs,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_NUM_GTP_ADDRS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2_extend_trans_layer_addrs->num_tlas, p_src + *p_length_read, "num_tlas");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_extend_trans_layer_addrs->num_tlas < 1) || (p_x2_extend_trans_layer_addrs->num_tlas > MAX_NUM_ENB_X2AP_GTP_TLAS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_extend_trans_layer_addrs->num_tlas] should be in range [1 <= %d <= MAX_NUM_ENB_X2AP_GTP_TLAS] !", p_x2_extend_trans_layer_addrs->num_tlas);
        return RRC_FAILURE;
    }
    }

    if (p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_GTP_ADDRS_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_extend_trans_layer_addrs->num_tlas; loop++)
        {
            if (RRC_FAILURE == rrc_parse_x2_trans_layer_addrs(
                &p_x2_extend_trans_layer_addrs->gtp_trans_layer_addrs[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2_enb_comm_info
(
    x2_enb_comm_info_t *p_x2_enb_comm_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2_enb_comm_info, 0, sizeof(x2_enb_comm_info_t));

    /* This function parses x2_enb_comm_info */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2_enb_comm_info");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2_enb_comm_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U8);

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2_enb_comm_info->num_ip_addr, p_src + *p_length_read, "num_ip_addr");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_ip_addr < 1) || (p_x2_enb_comm_info->num_ip_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->num_ip_addr] should be in range [1 <= %d <= MAX_NUM_IP_ADDR] !", p_x2_enb_comm_info->num_ip_addr);
        return RRC_FAILURE;
    }
    }

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_enb_comm_info->num_ip_addr; loop++)
        {
            if (RRC_FAILURE == rrc_parse_x2_ip_addr(
                &p_x2_enb_comm_info->ip_addr[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2_enb_comm_info->port, p_src + *p_length_read, "port");
    *p_length_read += sizeof(U16);

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2_enb_comm_info->num_ipv6_addr, p_src + *p_length_read, "num_ipv6_addr");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_ipv6_addr < 1) || (p_x2_enb_comm_info->num_ipv6_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->num_ipv6_addr] should be in range [1 <= %d <= MAX_NUM_IP_ADDR] !", p_x2_enb_comm_info->num_ipv6_addr);
        return RRC_FAILURE;
    }
    }

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_enb_comm_info->num_ipv6_addr; loop++)
        {
            if (RRC_FAILURE == rrc_parse_x2_ipv6_addr(
                &p_x2_enb_comm_info->ipv6_addr[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_EXT_NUM_TLAS_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2_enb_comm_info->num_tlas, p_src + *p_length_read, "num_tlas");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_tlas < 1) || (p_x2_enb_comm_info->num_tlas > MAX_NUM_ENB_X2AP_EXT_TLAS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->num_tlas] should be in range [1 <= %d <= MAX_NUM_ENB_X2AP_EXT_TLAS] !", p_x2_enb_comm_info->num_tlas);
        return RRC_FAILURE;
    }
    }

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_EXT_TLAS_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_enb_comm_info->num_tlas; loop++)
        {
            if (RRC_FAILURE == rrc_parse_x2_extend_trans_layer_addrs(
                &p_x2_enb_comm_info->extend_trans_layer_addrs[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_DEFAULT_SGNB_PRESENT)
    {

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2_enb_comm_info->default_sgnb, p_src + *p_length_read, "default_sgnb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2_enb_comm_info->default_sgnb > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->default_sgnb] should be in range [%d <= 1] !", p_x2_enb_comm_info->default_sgnb);
        return RRC_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_add_enb_res
(
    x2ap_add_enb_res_t *p_x2ap_add_enb_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_add_enb_res, 0, sizeof(x2ap_add_enb_res_t));

    /* This function parses x2ap_add_enb_res */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_add_enb_res");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_add_enb_res->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_add_enb_res->gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_add_enb_res->bitmask & X2AP_SUCCESS_ENB_ADD_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_enb_comm_info_list(
        &p_x2ap_add_enb_res->success_enb_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_add_enb_res->bitmask & X2AP_FAILURE_ENB_ADD_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_enb_comm_info_list(
        &p_x2ap_add_enb_res->failure_enb_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_enb_comm_info_list
(
    x2ap_enb_comm_info_list_t *p_x2ap_enb_comm_info_list,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_enb_comm_info_list, 0, sizeof(x2ap_enb_comm_info_list_t));

    /* This function parses x2ap_enb_comm_info_list */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_enb_comm_info_list");

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_enb_comm_info_list->num_peer_enb, p_src + *p_length_read, "num_peer_enb");
    *p_length_read += sizeof(U8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_enb_comm_info_list->num_peer_enb < 1) || (p_x2ap_enb_comm_info_list->num_peer_enb > MAX_PEER_ENB))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_enb_comm_info_list->num_peer_enb] should be in range [1 <= %d <= MAX_PEER_ENB] !", p_x2ap_enb_comm_info_list->num_peer_enb);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_enb_comm_info_list->num_peer_enb; loop++)
        {
            if (RRC_FAILURE == rrc_parse_x2_enb_comm_info(
                &p_x2ap_enb_comm_info_list->enb_comm_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_delete_enb_req
(
    x2ap_delete_enb_req_t *p_x2ap_delete_enb_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_delete_enb_req, 0, sizeof(x2ap_delete_enb_req_t));

    /* This function parses x2ap_delete_enb_req */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_delete_enb_req");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_delete_enb_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_delete_enb_req->gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_delete_enb_res
(
    x2ap_delete_enb_res_t *p_x2ap_delete_enb_res,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_delete_enb_res, 0, sizeof(x2ap_delete_enb_res_t));

    /* This function parses x2ap_delete_enb_res */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_delete_enb_res");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_delete_enb_res->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_delete_enb_res->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_delete_enb_res->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_delete_enb_res->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_delete_enb_res->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_delete_enb_res->response] should be in range [%d <= 1] !", p_x2ap_delete_enb_res->response);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_enb_link_up_ind
(
    x2ap_enb_link_up_ind_t *p_x2ap_enb_link_up_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_enb_link_up_ind, 0, sizeof(x2ap_enb_link_up_ind_t));

    /* This function parses x2ap_enb_link_up_ind */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_enb_link_up_ind");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_enb_link_up_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_enb_link_up_ind->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_enb_link_up_ind->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2_enb_comm_info(
        &p_x2ap_enb_link_up_ind->enb_comm_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_enb_link_up_ind->num_served_cell, p_src + *p_length_read, "num_served_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_enb_link_up_ind->num_served_cell < 1) || (p_x2ap_enb_link_up_ind->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_enb_link_up_ind->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_enb_link_up_ind->num_served_cell);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_enb_link_up_ind->num_served_cell; loop++)
        {
            if (RRC_FAILURE == rrc_parse_x2ap_served_cell_info_arr(
                &p_x2ap_enb_link_up_ind->served_cell_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (p_x2ap_enb_link_up_ind->bitmask & X2AP_GU_GROUP_ID_LIST_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_gu_group_id_list(
        &p_x2ap_enb_link_up_ind->gu_group_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_enb_link_down_ind
(
    x2ap_enb_link_down_ind_t *p_x2ap_enb_link_down_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_enb_link_down_ind, 0, sizeof(x2ap_enb_link_down_ind_t));

    /* This function parses x2ap_enb_link_down_ind */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_enb_link_down_ind");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_enb_link_down_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_enb_link_down_ind->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_enb_link_down_ind->bitmask & X2AP_PEER_ENB_ID_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_enb_link_down_ind->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_link_down_ind->bitmask & X2AP_PEER_ENB_COMM_INFO_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2_enb_comm_info(
        &p_x2ap_enb_link_down_ind->enb_comm_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_enb_config_update_wait_ind
(
    x2ap_enb_config_update_wait_ind_t *p_x2ap_enb_config_update_wait_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_enb_config_update_wait_ind, 0, sizeof(x2ap_enb_config_update_wait_ind_t));

    /* This function parses x2ap_enb_config_update_wait_ind */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_enb_config_update_wait_ind");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_enb_config_update_wait_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_enb_config_update_wait_ind->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_enb_config_update_wait_ind->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_enb_config_update_wait_ind->time_to_wait, p_src + *p_length_read, "time_to_wait");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_enb_config_update_wait_ind->time_to_wait > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_enb_config_update_wait_ind->time_to_wait] should be in range [%d <= 2] !", p_x2ap_enb_config_update_wait_ind->time_to_wait);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_ho_report_ind
(
    x2ap_ho_report_ind_t *p_x2ap_ho_report_ind,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_ho_report_ind, 0, sizeof(x2ap_ho_report_ind_t));

    /* This function parses x2ap_ho_report_ind */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_ho_report_ind");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_ho_report_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_ho_report_ind->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_ho_report_ind->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U32) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U32(&p_x2ap_ho_report_ind->ho_report_type, p_src + *p_length_read, "ho_report_type");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_ho_report_ind->ho_report_type > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_ho_report_ind->ho_report_type] should be in range [%d <= 1] !", p_x2ap_ho_report_ind->ho_report_type);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == rrc_parse_x2ap_cause(
        &p_x2ap_ho_report_ind->ho_cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2ap_ecgi(
        &p_x2ap_ho_report_ind->src_ecgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2ap_ecgi(
        &p_x2ap_ho_report_ind->fail_cell_ecgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_ho_report_ind->bitmask & X2AP_HO_REPORT_REEST_CELL_ECGI_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_ecgi(
        &p_x2ap_ho_report_ind->re_est_cell_ecgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_peer_cell_activation_req
(
    x2ap_peer_cell_activation_req_t *p_x2ap_peer_cell_activation_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_peer_cell_activation_req, 0, sizeof(x2ap_peer_cell_activation_req_t));

    /* This function parses x2ap_peer_cell_activation_req */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_peer_cell_activation_req");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_peer_cell_activation_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_peer_cell_activation_req->gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2ap_cell_activation_info(
        &p_x2ap_peer_cell_activation_req->peer_cell_activation_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_cell_activation_info
(
    x2ap_cell_activation_info_t *p_x2ap_cell_activation_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_cell_activation_info, 0, sizeof(x2ap_cell_activation_info_t));

    /* This function parses x2ap_cell_activation_info */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_cell_activation_info");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_cell_activation_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_cell_activation_info->num_served_cell, p_src + *p_length_read, "num_served_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_cell_activation_info->num_served_cell < 1) || (p_x2ap_cell_activation_info->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_cell_activation_info->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_cell_activation_info->num_served_cell);
        return RRC_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_cell_activation_info->num_served_cell; loop++)
        {
            if (RRC_FAILURE == rrc_parse_x2ap_ecgi(
                &p_x2ap_cell_activation_info->served_cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_peer_cell_activation_resp
(
    x2ap_peer_cell_activation_resp_t *p_x2ap_peer_cell_activation_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_peer_cell_activation_resp, 0, sizeof(x2ap_peer_cell_activation_resp_t));

    /* This function parses x2ap_peer_cell_activation_resp */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_peer_cell_activation_resp");

    if (RRC_FAILURE == rrc_parse_x2ap_cell_activation_resp(
        &p_x2ap_peer_cell_activation_resp->peer_cell_activation_resp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_cell_activation_resp
(
    x2ap_cell_activation_resp_t *p_x2ap_cell_activation_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_cell_activation_resp, 0, sizeof(x2ap_cell_activation_resp_t));

    /* This function parses x2ap_cell_activation_resp */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_cell_activation_resp");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_cell_activation_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_cell_activation_resp->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_cell_activation_resp->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_cell_activation_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_cell_activation_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_cell_activation_resp->response] should be in range [%d <= 1] !", p_x2ap_cell_activation_resp->response);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_cell_activation_resp->num_served_cell, p_src + *p_length_read, "num_served_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_cell_activation_resp->num_served_cell > MAX_SERVED_CELLS))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_cell_activation_resp->num_served_cell] should be in range [%d <= MAX_SERVED_CELLS] !", p_x2ap_cell_activation_resp->num_served_cell);
        return RRC_FAILURE;
    }

    if (p_x2ap_cell_activation_resp->bitmask & X2AP_CELL_ACTIVATION_SERVED_CELL_LIST_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_cell_activation_resp->num_served_cell; loop++)
        {
            if (RRC_FAILURE == rrc_parse_x2ap_ecgi(
                &p_x2ap_cell_activation_resp->served_cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return RRC_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_x2ap_cell_activation_resp->bitmask & X2AP_CELL_ACTIVATION_CAUSE_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_cause(
        &p_x2ap_cell_activation_resp->cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_cell_activation_req
(
    x2ap_cell_activation_req_t *p_x2ap_cell_activation_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_cell_activation_req, 0, sizeof(x2ap_cell_activation_req_t));

    /* This function parses x2ap_cell_activation_req */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_cell_activation_req");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_cell_activation_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_cell_activation_req->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_cell_activation_req->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2ap_cell_activation_info(
        &p_x2ap_cell_activation_req->cell_activation_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_cell_activation_response
(
    x2ap_cell_activation_response_t *p_x2ap_cell_activation_response,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_cell_activation_response, 0, sizeof(x2ap_cell_activation_response_t));

    /* This function parses x2ap_cell_activation_response */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_cell_activation_response");

    if (RRC_FAILURE == rrc_parse_x2ap_cell_activation_resp(
        &p_x2ap_cell_activation_response->cell_activation_resp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_mobility_change_req
(
    x2ap_mobility_change_req_t *p_x2ap_mobility_change_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_mobility_change_req, 0, sizeof(x2ap_mobility_change_req_t));

    /* This function parses x2ap_mobility_change_req */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_mobility_change_req");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_mobility_change_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_mobility_change_req->gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2ap_mobility_change_info(
        &p_x2ap_mobility_change_req->mobility_change_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_mob_param
(
    x2ap_mob_param_t *p_x2ap_mob_param,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    memset_wrapper(p_x2ap_mob_param, 0, sizeof(x2ap_mob_param_t));

    /* This function parses x2ap_mob_param */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_mob_param");

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_x2ap_mob_param->handover_trigger_change, p_src + *p_length_read, "handover_trigger_change");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_mob_param->handover_trigger_change < -20) || (p_x2ap_mob_param->handover_trigger_change > 20))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mob_param->handover_trigger_change] should be in range [-20 <= %d <= 20] !", p_x2ap_mob_param->handover_trigger_change);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_mobility_change_info
(
    x2ap_mobility_change_info_t *p_x2ap_mobility_change_info,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_mobility_change_info, 0, sizeof(x2ap_mobility_change_info_t));

    /* This function parses x2ap_mobility_change_info */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_mobility_change_info");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_mobility_change_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2ap_ecgi(
        &p_x2ap_mobility_change_info->enb1_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2ap_ecgi(
        &p_x2ap_mobility_change_info->enb2_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_mobility_change_info->bitmask & X2AP_ENB1_MOBILITY_PARAMETERS_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_mob_param(
        &p_x2ap_mobility_change_info->enb1_mob_param,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (RRC_FAILURE == rrc_parse_x2ap_mob_param(
        &p_x2ap_mobility_change_info->enb2_mob_param,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2ap_cause(
        &p_x2ap_mobility_change_info->cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_mobility_change_resp
(
    x2ap_mobility_change_resp_t *p_x2ap_mobility_change_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_mobility_change_resp, 0, sizeof(x2ap_mobility_change_resp_t));

    /* This function parses x2ap_mobility_change_resp */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_mobility_change_resp");

    if (RRC_FAILURE == rrc_parse_x2ap_mob_ch_resp(
        &p_x2ap_mobility_change_resp->mobility_change_resp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_mob_range
(
    x2ap_mob_range_t *p_x2ap_mob_range,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{

    *p_length_read = 0;

    memset_wrapper(p_x2ap_mob_range, 0, sizeof(x2ap_mob_range_t));

    /* This function parses x2ap_mob_range */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_mob_range");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_mob_range->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_x2ap_mob_range->ho_trigger_change_lower_limit, p_src + *p_length_read, "ho_trigger_change_lower_limit");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_mob_range->ho_trigger_change_lower_limit < -20) || (p_x2ap_mob_range->ho_trigger_change_lower_limit > 20))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mob_range->ho_trigger_change_lower_limit] should be in range [-20 <= %d <= 20] !", p_x2ap_mob_range->ho_trigger_change_lower_limit);
        return RRC_FAILURE;
    }

    if (*p_length_read + (S32)sizeof(S8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_S8(&p_x2ap_mob_range->ho_trigger_change_upper_limit, p_src + *p_length_read, "ho_trigger_change_upper_limit");
    *p_length_read += sizeof(S8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_mob_range->ho_trigger_change_upper_limit < -20) || (p_x2ap_mob_range->ho_trigger_change_upper_limit > 20))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2ap_mob_range->ho_trigger_change_upper_limit] should be in range [-20 <= %d <= 20] !", p_x2ap_mob_range->ho_trigger_change_upper_limit);
        return RRC_FAILURE;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

static
rrc_return_et
rrc_parse_x2ap_mob_ch_resp
(
    x2ap_mob_ch_resp_t *p_x2ap_mob_ch_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_mob_ch_resp, 0, sizeof(x2ap_mob_ch_resp_t));

    /* This function parses x2ap_mob_ch_resp */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_mob_ch_resp");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_mob_ch_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_mob_ch_resp->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_mob_ch_resp->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2ap_ecgi(
        &p_x2ap_mob_ch_resp->enb1_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2ap_ecgi(
        &p_x2ap_mob_ch_resp->enb2_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (S32)sizeof(U8) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U8(&p_x2ap_mob_ch_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(U8);

    if (p_x2ap_mob_ch_resp->bitmask & X2AP_MOBILITY_SETTING_CAUSE_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_cause(
        &p_x2ap_mob_ch_resp->cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_mob_ch_resp->bitmask & X2AP_MOBILITY_PARAM_MOD_RANGE_PRESENT)
    {

    if (RRC_FAILURE == rrc_parse_x2ap_mob_range(
        &p_x2ap_mob_ch_resp->enb2_mob_param_mod_range,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_mobility_setting_change_req
(
    x2ap_mobility_setting_change_req_t *p_x2ap_mobility_setting_change_req,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_mobility_setting_change_req, 0, sizeof(x2ap_mobility_setting_change_req_t));

    /* This function parses x2ap_mobility_setting_change_req */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_mobility_setting_change_req");

    if (*p_length_read + (S32)sizeof(U16) > length_left)
    {
       RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    rrc_cp_unpack_U16(&p_x2ap_mobility_setting_change_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_mobility_setting_change_req->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2_gb_enb_id(
        &p_x2ap_mobility_setting_change_req->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (RRC_FAILURE == rrc_parse_x2ap_mobility_change_info(
        &p_x2ap_mobility_setting_change_req->mobility_change_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

rrc_return_et
rrc_parse_x2ap_mobility_setting_change_resp
(
    x2ap_mobility_setting_change_resp_t *p_x2ap_mobility_setting_change_resp,
    U8  *p_src,
    S32 length_left,
    S32 *p_length_read
)
{
    S32 length_read = 0;


    *p_length_read = 0;

    memset_wrapper(p_x2ap_mobility_setting_change_resp, 0, sizeof(x2ap_mobility_setting_change_resp_t));

    /* This function parses x2ap_mobility_setting_change_resp */
    RRC_TRACE(RRC_COMP_PARSE, "Parsing x2ap_mobility_setting_change_resp");

    if (RRC_FAILURE == rrc_parse_x2ap_mob_ch_resp(
        &p_x2ap_mobility_setting_change_resp->mobility_setting_change_resp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return RRC_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    RRC_TRACE(RRC_WARNING, "Incoming message damaged!");
        return RRC_FAILURE;
    }

    return(RRC_SUCCESS);
}

