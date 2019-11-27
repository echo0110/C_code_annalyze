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
 *  File Description : The file oam_x2apCommon_il_parser.c contains the definitions 
 *                     of rrc-x2apCommon interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/

#include "oam_x2apCommon_il_parser.h"
#include "rrc_x2apCommon_intf.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(U16) + sizeof(U16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
oam_return_et
oam_parse_x2ap_plmn_identity
(
    x2ap_plmn_identity_t *p_x2ap_plmn_identity,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_macro_enb_id
(
    macro_enb_id_t *p_macro_enb_id,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_home_enb_id
(
    home_enb_id_t *p_home_enb_id,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2_enb_id_info
(
    x2_enb_id_info_t *p_x2_enb_id_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2_gb_enb_id
(
    x2_gb_enb_id_t *p_x2_gb_enb_id,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_broadcast_plmn
(
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_earfcn
(
    x2ap_earfcn_t *p_x2ap_earfcn,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_fdd_info
(
    x2ap_fdd_info_t *p_x2ap_fdd_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_special_subframe_info
(
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_tdd_info
(
    x2ap_tdd_info_t *p_x2ap_tdd_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_choice_eutra_mode
(
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_choice_eutra_mode_info
(
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_prach_configuration_info
(
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_subframe_allocation
(
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_mbsfn_info
(
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_mbsfn_subframe_info
(
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_served_cell_info
(
    x2ap_served_cell_info_t *p_x2ap_served_cell_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_ecgi
(
    x2ap_ecgi_t *p_x2ap_ecgi,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_nbour_info
(
    x2ap_nbour_info_t *p_x2ap_nbour_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_nbour_cell_info
(
    x2ap_nbour_cell_info_t *p_x2ap_nbour_cell_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_served_cell_info_arr
(
    x2ap_served_cell_info_arr_t *p_x2ap_served_cell_info_arr,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_served_cells_to_add_list
(
    x2ap_served_cells_to_add_list_t *p_x2ap_served_cells_to_add_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_served_cells_to_modify_info
(
    x2ap_served_cells_to_modify_info_t *p_x2ap_served_cells_to_modify_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_served_cells_to_modify_list
(
    x2ap_served_cells_to_modify_list_t *p_x2ap_served_cells_to_modify_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_served_cells_to_delete_list
(
    x2ap_served_cells_to_delete_list_t *p_x2ap_served_cells_to_delete_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_gu_group_id
(
    x2ap_gu_group_id_t *p_x2ap_gu_group_id,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_gu_group_id_list
(
    x2ap_gu_group_id_list_t *p_x2ap_gu_group_id_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_neigh_ecgi_enb_id_map_info
(
    x2ap_neigh_ecgi_enb_id_map_info_t *p_x2ap_neigh_ecgi_enb_id_map_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_neigh_ecgi_enb_id_map_list
(
    x2ap_neigh_ecgi_enb_id_map_list_t *p_x2ap_neigh_ecgi_enb_id_map_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_cause
(
    x2ap_cause_t *p_x2ap_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2_ip_addr
(
    x2_ip_addr_t *p_x2_ip_addr,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2_ipv6_addr
(
    x2_ipv6_addr_t *p_x2_ipv6_addr,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2_trans_layer_addrs
(
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2_extend_trans_layer_addrs
(
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2_enb_comm_info
(
    x2_enb_comm_info_t *p_x2_enb_comm_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_enb_comm_info_list
(
    x2ap_enb_comm_info_list_t *p_x2ap_enb_comm_info_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_cell_activation_info
(
    x2ap_cell_activation_info_t *p_x2ap_cell_activation_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_cell_activation_resp
(
    x2ap_cell_activation_resp_t *p_x2ap_cell_activation_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_mob_param
(
    x2ap_mob_param_t *p_x2ap_mob_param,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_mobility_change_info
(
    x2ap_mobility_change_info_t *p_x2ap_mobility_change_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_mob_range
(
    x2ap_mob_range_t *p_x2ap_mob_range,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_parse_x2ap_mob_ch_resp
(
    x2ap_mob_ch_resp_t *p_x2ap_mob_ch_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

oam_return_et
oam_parse_x2ap_enb_config_update_req
(
    x2ap_enb_config_update_req_t *p_x2ap_enb_config_update_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_enb_config_update_req, 0, sizeof(x2ap_enb_config_update_req_t));

    /* This function parses x2ap_enb_config_update_req */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_enb_config_update_req");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_enb_config_update_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_enb_config_update_req->num_peer_enb, p_src + *p_length_read, "num_peer_enb");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_enb_config_update_req->num_peer_enb > MAX_PEER_ENB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_enb_config_update_req->num_peer_enb] should be in range [%d <= MAX_PEER_ENB] !", p_x2ap_enb_config_update_req->num_peer_enb);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_enb_config_update_req->num_peer_enb; loop++)
        {
            if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
                &p_x2ap_enb_config_update_req->enb_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (p_x2ap_enb_config_update_req->bitmask & X2AP_SERVED_CELLS_TO_ADD_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_served_cells_to_add_list(
        &p_x2ap_enb_config_update_req->add_served_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_config_update_req->bitmask & X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_served_cells_to_modify_list(
        &p_x2ap_enb_config_update_req->mod_served_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_config_update_req->bitmask & X2AP_SERVED_CELLS_TO_DELETE_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_served_cells_to_delete_list(
        &p_x2ap_enb_config_update_req->del_served_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_config_update_req->bitmask & X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_gu_group_id_list(
        &p_x2ap_enb_config_update_req->add_gu_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_config_update_req->bitmask & X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_gu_group_id_list(
        &p_x2ap_enb_config_update_req->delete_gu_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_config_update_req->bitmask & X2AP_NEIGH_ECGI_ENB_ID_MAPPING_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_neigh_ecgi_enb_id_map_list(
        &p_x2ap_enb_config_update_req->neigh_ecgi_enb_id_map,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_plmn_identity
(
    x2ap_plmn_identity_t *p_x2ap_plmn_identity,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_x2ap_plmn_identity, 0, sizeof(x2ap_plmn_identity_t));

    /* This function parses x2ap_plmn_identity */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_plmn_identity");

    if (*p_length_read + (SInt32)sizeof(p_x2ap_plmn_identity->plmn_id) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_plmn_identity->plmn_id); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2ap_plmn_identity->plmn_id[loop], (void*)(p_src + *p_length_read), "plmn_id[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_macro_enb_id
(
    macro_enb_id_t *p_macro_enb_id,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_macro_enb_id, 0, sizeof(macro_enb_id_t));

    /* This function parses macro_enb_id */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing macro_enb_id");

    if (*p_length_read + (SInt32)sizeof(p_macro_enb_id->eNB_id) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_macro_enb_id->eNB_id); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_macro_enb_id->eNB_id[loop], (void*)(p_src + *p_length_read), "eNB_id[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_home_enb_id
(
    home_enb_id_t *p_home_enb_id,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_home_enb_id, 0, sizeof(home_enb_id_t));

    /* This function parses home_enb_id */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing home_enb_id");

    if (*p_length_read + (SInt32)sizeof(p_home_enb_id->eNB_id) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_home_enb_id->eNB_id); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_home_enb_id->eNB_id[loop], (void*)(p_src + *p_length_read), "eNB_id[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2_enb_id_info
(
    x2_enb_id_info_t *p_x2_enb_id_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2_enb_id_info, 0, sizeof(x2_enb_id_info_t));

    /* This function parses x2_enb_id_info */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2_enb_id_info");

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2_enb_id_info->presence_bitmask, p_src + *p_length_read, "presence_bitmask");
    *p_length_read += sizeof(UInt8);

    if (p_x2_enb_id_info->presence_bitmask & 1)
    {

    if (OAM_FAILURE == oam_parse_macro_enb_id(
        &p_x2_enb_id_info->macro_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2_enb_id_info->presence_bitmask & 2)
    {

    if (OAM_FAILURE == oam_parse_home_enb_id(
        &p_x2_enb_id_info->home_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2_gb_enb_id
(
    x2_gb_enb_id_t *p_x2_gb_enb_id,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2_gb_enb_id, 0, sizeof(x2_gb_enb_id_t));

    /* This function parses x2_gb_enb_id */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2_gb_enb_id");

    if (OAM_FAILURE == oam_parse_x2ap_plmn_identity(
        &p_x2_gb_enb_id->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2_enb_id_info(
        &p_x2_gb_enb_id->enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_broadcast_plmn
(
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_broadcast_plmn, 0, sizeof(x2ap_broadcast_plmn_t));

    /* This function parses x2ap_broadcast_plmn */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_broadcast_plmn");

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_broadcast_plmn->num_bcast_plmn_id, p_src + *p_length_read, "num_bcast_plmn_id");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_broadcast_plmn->num_bcast_plmn_id < 1) || (p_x2ap_broadcast_plmn->num_bcast_plmn_id > MAX_NUM_BPLMN))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_broadcast_plmn->num_bcast_plmn_id] should be in range [1 <= %d <= MAX_NUM_BPLMN] !", p_x2ap_broadcast_plmn->num_bcast_plmn_id);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_broadcast_plmn->num_bcast_plmn_id; loop++)
        {
            if (OAM_FAILURE == oam_parse_x2ap_plmn_identity(
                &p_x2ap_broadcast_plmn->plmn_identity[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_earfcn
(
    x2ap_earfcn_t *p_x2ap_earfcn,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_x2ap_earfcn, 0, sizeof(x2ap_earfcn_t));

    /* This function parses x2ap_earfcn */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_earfcn");

    if (*p_length_read + (SInt32)sizeof(U32) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_earfcn->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_earfcn->earfcn > MAX_EARFCN))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_earfcn->earfcn] should be in range [%d <= MAX_EARFCN] !", p_x2ap_earfcn->earfcn);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_fdd_info
(
    x2ap_fdd_info_t *p_x2ap_fdd_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_fdd_info, 0, sizeof(x2ap_fdd_info_t));

    /* This function parses x2ap_fdd_info */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_fdd_info");

    if (OAM_FAILURE == oam_parse_x2ap_earfcn(
        &p_x2ap_fdd_info->ul_earfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2ap_earfcn(
        &p_x2ap_fdd_info->dl_earfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_fdd_info->ul_trnsmission_bdwidth, p_src + *p_length_read, "ul_trnsmission_bdwidth");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_fdd_info->ul_trnsmission_bdwidth > 5))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_fdd_info->ul_trnsmission_bdwidth] should be in range [%d <= 5] !", p_x2ap_fdd_info->ul_trnsmission_bdwidth);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_fdd_info->dl_trnsmission_bdwidth, p_src + *p_length_read, "dl_trnsmission_bdwidth");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_fdd_info->dl_trnsmission_bdwidth > 5))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_fdd_info->dl_trnsmission_bdwidth] should be in range [%d <= 5] !", p_x2ap_fdd_info->dl_trnsmission_bdwidth);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_special_subframe_info
(
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_x2ap_special_subframe_info, 0, sizeof(x2ap_special_subframe_info_t));

    /* This function parses x2ap_special_subframe_info */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_special_subframe_info");

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_special_subframe_info->special_subframe_patterns, p_src + *p_length_read, "special_subframe_patterns");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->special_subframe_patterns > 8))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_special_subframe_info->special_subframe_patterns] should be in range [%d <= 8] !", p_x2ap_special_subframe_info->special_subframe_patterns);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_special_subframe_info->dl_cp, p_src + *p_length_read, "dl_cp");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->dl_cp > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_special_subframe_info->dl_cp] should be in range [%d <= 1] !", p_x2ap_special_subframe_info->dl_cp);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_special_subframe_info->ul_cp, p_src + *p_length_read, "ul_cp");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->ul_cp > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_special_subframe_info->ul_cp] should be in range [%d <= 1] !", p_x2ap_special_subframe_info->ul_cp);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_tdd_info
(
    x2ap_tdd_info_t *p_x2ap_tdd_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_tdd_info, 0, sizeof(x2ap_tdd_info_t));

    /* This function parses x2ap_tdd_info */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_tdd_info");

    if (OAM_FAILURE == oam_parse_x2ap_earfcn(
        &p_x2ap_tdd_info->earfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_tdd_info->trnsmission_bdwidth, p_src + *p_length_read, "trnsmission_bdwidth");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_tdd_info->trnsmission_bdwidth > 5))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_tdd_info->trnsmission_bdwidth] should be in range [%d <= 5] !", p_x2ap_tdd_info->trnsmission_bdwidth);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_tdd_info->subframe_assn, p_src + *p_length_read, "subframe_assn");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_tdd_info->subframe_assn > 6))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_tdd_info->subframe_assn] should be in range [%d <= 6] !", p_x2ap_tdd_info->subframe_assn);
        return OAM_FAILURE;
    }

    if (OAM_FAILURE == oam_parse_x2ap_special_subframe_info(
        &p_x2ap_tdd_info->special_subframe_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_choice_eutra_mode
(
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_choice_eutra_mode, 0, sizeof(x2ap_choice_eutra_mode_t));

    /* This function parses x2ap_choice_eutra_mode */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_choice_eutra_mode");

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_choice_eutra_mode->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt8);

    if (p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_FDD)
    {

    if (OAM_FAILURE == oam_parse_x2ap_fdd_info(
        &p_x2ap_choice_eutra_mode->fdd_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_TDD)
    {

    if (OAM_FAILURE == oam_parse_x2ap_tdd_info(
        &p_x2ap_choice_eutra_mode->tdd_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_choice_eutra_mode_info
(
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_choice_eutra_mode_info, 0, sizeof(x2ap_choice_eutra_mode_info_t));

    /* This function parses x2ap_choice_eutra_mode_info */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_choice_eutra_mode_info");

    if (OAM_FAILURE == oam_parse_x2ap_choice_eutra_mode(
        &p_x2ap_choice_eutra_mode_info->choice_eutra_mode,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_prach_configuration_info
(
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_x2ap_prach_configuration_info, 0, sizeof(x2ap_prach_configuration_info_t));

    /* This function parses x2ap_prach_configuration_info */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_prach_configuration_info");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_prach_configuration_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_prach_configuration_info->root_sequence_index, p_src + *p_length_read, "root_sequence_index");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->root_sequence_index > 837))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_prach_configuration_info->root_sequence_index] should be in range [%d <= 837] !", p_x2ap_prach_configuration_info->root_sequence_index);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_prach_configuration_info->zero_correlation_zone_config, p_src + *p_length_read, "zero_correlation_zone_config");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->zero_correlation_zone_config > 15))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_prach_configuration_info->zero_correlation_zone_config] should be in range [%d <= 15] !", p_x2ap_prach_configuration_info->zero_correlation_zone_config);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_prach_configuration_info->high_speed_flag, p_src + *p_length_read, "high_speed_flag");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->high_speed_flag > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_prach_configuration_info->high_speed_flag] should be in range [%d <= 1] !", p_x2ap_prach_configuration_info->high_speed_flag);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_prach_configuration_info->prach_frequency_offset, p_src + *p_length_read, "prach_frequency_offset");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->prach_frequency_offset > 94))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_prach_configuration_info->prach_frequency_offset] should be in range [%d <= 94] !", p_x2ap_prach_configuration_info->prach_frequency_offset);
        return OAM_FAILURE;
    }

    if (p_x2ap_prach_configuration_info->bitmask & X2AP_PRACH_CONFIG_INDEX_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_prach_configuration_info->prach_config_index, p_src + *p_length_read, "prach_config_index");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->prach_config_index > 63))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_prach_configuration_info->prach_config_index] should be in range [%d <= 63] !", p_x2ap_prach_configuration_info->prach_config_index);
        return OAM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_subframe_allocation
(
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_x2ap_subframe_allocation, 0, sizeof(x2ap_subframe_allocation_t));

    /* This function parses x2ap_subframe_allocation */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_subframe_allocation");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_subframe_allocation->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(p_x2ap_subframe_allocation->oneframe) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_subframe_allocation->oneframe); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2ap_subframe_allocation->oneframe[loop], (void*)(p_src + *p_length_read), "oneframe[]");
            *p_length_read += sizeof(UInt8);
        }
    }
    }

    if (p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(p_x2ap_subframe_allocation->fourframe) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_subframe_allocation->fourframe); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2ap_subframe_allocation->fourframe[loop], (void*)(p_src + *p_length_read), "fourframe[]");
            *p_length_read += sizeof(UInt8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_mbsfn_info
(
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_mbsfn_info, 0, sizeof(x2ap_mbsfn_info_t));

    /* This function parses x2ap_mbsfn_info */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_mbsfn_info");

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_mbsfn_info->radioframe_alloc_period, p_src + *p_length_read, "radioframe_alloc_period");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_info->radioframe_alloc_period > 5))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_mbsfn_info->radioframe_alloc_period] should be in range [%d <= 5] !", p_x2ap_mbsfn_info->radioframe_alloc_period);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_mbsfn_info->radioframe_alloc_offset, p_src + *p_length_read, "radioframe_alloc_offset");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_info->radioframe_alloc_offset > 7))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_mbsfn_info->radioframe_alloc_offset] should be in range [%d <= 7] !", p_x2ap_mbsfn_info->radioframe_alloc_offset);
        return OAM_FAILURE;
    }

    if (OAM_FAILURE == oam_parse_x2ap_subframe_allocation(
        &p_x2ap_mbsfn_info->subframe_alloc,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_mbsfn_subframe_info
(
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_mbsfn_subframe_info, 0, sizeof(x2ap_mbsfn_subframe_info_t));

    /* This function parses x2ap_mbsfn_subframe_info */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_mbsfn_subframe_info");

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_mbsfn_subframe_info->num_mbsfn, p_src + *p_length_read, "num_mbsfn");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_subframe_info->num_mbsfn > MAX_NUM_MBSFN))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_mbsfn_subframe_info->num_mbsfn] should be in range [%d <= MAX_NUM_MBSFN] !", p_x2ap_mbsfn_subframe_info->num_mbsfn);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_mbsfn_subframe_info->num_mbsfn; loop++)
        {
            if (OAM_FAILURE == oam_parse_x2ap_mbsfn_info(
                &p_x2ap_mbsfn_subframe_info->mbsfn_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_served_cell_info
(
    x2ap_served_cell_info_t *p_x2ap_served_cell_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_served_cell_info, 0, sizeof(x2ap_served_cell_info_t));

    /* This function parses x2ap_served_cell_info */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_served_cell_info");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_served_cell_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_served_cell_info->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_served_cell_info->pci > 503))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_served_cell_info->pci] should be in range [%d <= 503] !", p_x2ap_served_cell_info->pci);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_served_cell_info->cell_id, p_src + *p_length_read, "cell_id");
    *p_length_read += sizeof(UInt8);

    if (*p_length_read + (SInt32)sizeof(p_x2ap_served_cell_info->tac) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_served_cell_info->tac); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2ap_served_cell_info->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (OAM_FAILURE == oam_parse_x2ap_broadcast_plmn(
        &p_x2ap_served_cell_info->bcast_plmn_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2ap_choice_eutra_mode_info(
        &p_x2ap_served_cell_info->choice_eutra_mode,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_served_cell_info->bitmask & X2AP_NUM_ANTENNA_INFO_PORT_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_served_cell_info->num_antenna_port, p_src + *p_length_read, "num_antenna_port");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_served_cell_info->num_antenna_port > 2))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_served_cell_info->num_antenna_port] should be in range [%d <= 2] !", p_x2ap_served_cell_info->num_antenna_port);
        return OAM_FAILURE;
    }
    }

    if (p_x2ap_served_cell_info->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_prach_configuration_info(
        &p_x2ap_served_cell_info->prach_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_served_cell_info->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_mbsfn_subframe_info(
        &p_x2ap_served_cell_info->mbsfn_subframe_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_served_cell_info->bitmask & X2AP_CSG_ID_INFO_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(p_x2ap_served_cell_info->csg_identity) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_served_cell_info->csg_identity); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2ap_served_cell_info->csg_identity[loop], (void*)(p_src + *p_length_read), "csg_identity[]");
            *p_length_read += sizeof(UInt8);
        }
    }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_ecgi
(
    x2ap_ecgi_t *p_x2ap_ecgi,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_ecgi, 0, sizeof(x2ap_ecgi_t));

    /* This function parses x2ap_ecgi */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_ecgi");

    if (OAM_FAILURE == oam_parse_x2ap_plmn_identity(
        &p_x2ap_ecgi->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(p_x2ap_ecgi->eutran_cell_id) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_ecgi->eutran_cell_id); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2ap_ecgi->eutran_cell_id[loop], (void*)(p_src + *p_length_read), "eutran_cell_id[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_nbour_info
(
    x2ap_nbour_info_t *p_x2ap_nbour_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_nbour_info, 0, sizeof(x2ap_nbour_info_t));

    /* This function parses x2ap_nbour_info */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_nbour_info");

    if (OAM_FAILURE == oam_parse_x2ap_ecgi(
        &p_x2ap_nbour_info->ecgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(U32) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_nbour_info->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nbour_info->pci > 503))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_nbour_info->pci] should be in range [%d <= 503] !", p_x2ap_nbour_info->pci);
        return OAM_FAILURE;
    }

    if (OAM_FAILURE == oam_parse_x2ap_earfcn(
        &p_x2ap_nbour_info->earfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_nbour_cell_info
(
    x2ap_nbour_cell_info_t *p_x2ap_nbour_cell_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_nbour_cell_info, 0, sizeof(x2ap_nbour_cell_info_t));

    /* This function parses x2ap_nbour_cell_info */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_nbour_cell_info");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_nbour_cell_info->num_neighbour_cell, p_src + *p_length_read, "num_neighbour_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nbour_cell_info->num_neighbour_cell > MAX_NEIGHBOURS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_nbour_cell_info->num_neighbour_cell] should be in range [%d <= MAX_NEIGHBOURS] !", p_x2ap_nbour_cell_info->num_neighbour_cell);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nbour_cell_info->num_neighbour_cell; loop++)
        {
            if (OAM_FAILURE == oam_parse_x2ap_nbour_info(
                &p_x2ap_nbour_cell_info->neighbour_info_arr[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_served_cell_info_arr
(
    x2ap_served_cell_info_arr_t *p_x2ap_served_cell_info_arr,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_served_cell_info_arr, 0, sizeof(x2ap_served_cell_info_arr_t));

    /* This function parses x2ap_served_cell_info_arr */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_served_cell_info_arr");

    if (OAM_FAILURE == oam_parse_x2ap_served_cell_info(
        &p_x2ap_served_cell_info_arr->cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2ap_nbour_cell_info(
        &p_x2ap_served_cell_info_arr->nbour_cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_served_cells_to_add_list
(
    x2ap_served_cells_to_add_list_t *p_x2ap_served_cells_to_add_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_served_cells_to_add_list, 0, sizeof(x2ap_served_cells_to_add_list_t));

    /* This function parses x2ap_served_cells_to_add_list */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_served_cells_to_add_list");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_served_cells_to_add_list->num_served_cell, p_src + *p_length_read, "num_served_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_served_cells_to_add_list->num_served_cell < 1) || (p_x2ap_served_cells_to_add_list->num_served_cell > MAX_SERVED_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_served_cells_to_add_list->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_served_cells_to_add_list->num_served_cell);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_served_cells_to_add_list->num_served_cell; loop++)
        {
            if (OAM_FAILURE == oam_parse_x2ap_served_cell_info_arr(
                &p_x2ap_served_cells_to_add_list->cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_served_cells_to_modify_info
(
    x2ap_served_cells_to_modify_info_t *p_x2ap_served_cells_to_modify_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_served_cells_to_modify_info, 0, sizeof(x2ap_served_cells_to_modify_info_t));

    /* This function parses x2ap_served_cells_to_modify_info */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_served_cells_to_modify_info");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_served_cells_to_modify_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_served_cells_to_modify_info->old_cell_id, p_src + *p_length_read, "old_cell_id");
    *p_length_read += sizeof(UInt8);

    if (OAM_FAILURE == oam_parse_x2ap_served_cell_info_arr(
        &p_x2ap_served_cells_to_modify_info->served_cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_served_cells_to_modify_info->bitmask & X2AP_DEACTIVATION_IND_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_served_cells_to_modify_info->deactivation_ind, p_src + *p_length_read, "deactivation_ind");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_served_cells_to_modify_info->deactivation_ind > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_served_cells_to_modify_info->deactivation_ind] should be in range [%d <= 1] !", p_x2ap_served_cells_to_modify_info->deactivation_ind);
        return OAM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_served_cells_to_modify_list
(
    x2ap_served_cells_to_modify_list_t *p_x2ap_served_cells_to_modify_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_served_cells_to_modify_list, 0, sizeof(x2ap_served_cells_to_modify_list_t));

    /* This function parses x2ap_served_cells_to_modify_list */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_served_cells_to_modify_list");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_served_cells_to_modify_list->num_served_cell, p_src + *p_length_read, "num_served_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_served_cells_to_modify_list->num_served_cell < 1) || (p_x2ap_served_cells_to_modify_list->num_served_cell > MAX_SERVED_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_served_cells_to_modify_list->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_served_cells_to_modify_list->num_served_cell);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_served_cells_to_modify_list->num_served_cell; loop++)
        {
            if (OAM_FAILURE == oam_parse_x2ap_served_cells_to_modify_info(
                &p_x2ap_served_cells_to_modify_list->cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_served_cells_to_delete_list
(
    x2ap_served_cells_to_delete_list_t *p_x2ap_served_cells_to_delete_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_x2ap_served_cells_to_delete_list, 0, sizeof(x2ap_served_cells_to_delete_list_t));

    /* This function parses x2ap_served_cells_to_delete_list */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_served_cells_to_delete_list");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_served_cells_to_delete_list->num_served_cell, p_src + *p_length_read, "num_served_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_served_cells_to_delete_list->num_served_cell < 1) || (p_x2ap_served_cells_to_delete_list->num_served_cell > MAX_SERVED_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_served_cells_to_delete_list->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_served_cells_to_delete_list->num_served_cell);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)(p_x2ap_served_cells_to_delete_list->num_served_cell * sizeof(p_x2ap_served_cells_to_delete_list->cell_id_list[0])) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_served_cells_to_delete_list->num_served_cell; loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2ap_served_cells_to_delete_list->cell_id_list[loop], (void*)(p_src + *p_length_read), "cell_id_list[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_gu_group_id
(
    x2ap_gu_group_id_t *p_x2ap_gu_group_id,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_gu_group_id, 0, sizeof(x2ap_gu_group_id_t));

    /* This function parses x2ap_gu_group_id */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_gu_group_id");

    if (OAM_FAILURE == oam_parse_x2ap_plmn_identity(
        &p_x2ap_gu_group_id->plmn_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(p_x2ap_gu_group_id->mme_group_id) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_gu_group_id->mme_group_id); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2ap_gu_group_id->mme_group_id[loop], (void*)(p_src + *p_length_read), "mme_group_id[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_gu_group_id_list
(
    x2ap_gu_group_id_list_t *p_x2ap_gu_group_id_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_gu_group_id_list, 0, sizeof(x2ap_gu_group_id_list_t));

    /* This function parses x2ap_gu_group_id_list */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_gu_group_id_list");

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_gu_group_id_list->num_gu_group_id, p_src + *p_length_read, "num_gu_group_id");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_gu_group_id_list->num_gu_group_id < 1) || (p_x2ap_gu_group_id_list->num_gu_group_id > MAX_ENB_POOLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_gu_group_id_list->num_gu_group_id] should be in range [1 <= %d <= MAX_ENB_POOLS] !", p_x2ap_gu_group_id_list->num_gu_group_id);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_gu_group_id_list->num_gu_group_id; loop++)
        {
            if (OAM_FAILURE == oam_parse_x2ap_gu_group_id(
                &p_x2ap_gu_group_id_list->gu_id_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_neigh_ecgi_enb_id_map_info
(
    x2ap_neigh_ecgi_enb_id_map_info_t *p_x2ap_neigh_ecgi_enb_id_map_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_neigh_ecgi_enb_id_map_info, 0, sizeof(x2ap_neigh_ecgi_enb_id_map_info_t));

    /* This function parses x2ap_neigh_ecgi_enb_id_map_info */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_neigh_ecgi_enb_id_map_info");

    if (OAM_FAILURE == oam_parse_x2ap_ecgi(
        &p_x2ap_neigh_ecgi_enb_id_map_info->ecgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_neigh_ecgi_enb_id_map_info->enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_neigh_ecgi_enb_id_map_list
(
    x2ap_neigh_ecgi_enb_id_map_list_t *p_x2ap_neigh_ecgi_enb_id_map_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_neigh_ecgi_enb_id_map_list, 0, sizeof(x2ap_neigh_ecgi_enb_id_map_list_t));

    /* This function parses x2ap_neigh_ecgi_enb_id_map_list */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_neigh_ecgi_enb_id_map_list");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count, p_src + *p_length_read, "neigh_info_count");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count < 1) || (p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count > MAX_NEIGHBOURS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count] should be in range [1 <= %d <= MAX_NEIGHBOURS] !", p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_neigh_ecgi_enb_id_map_list->neigh_info_count; loop++)
        {
            if (OAM_FAILURE == oam_parse_x2ap_neigh_ecgi_enb_id_map_info(
                &p_x2ap_neigh_ecgi_enb_id_map_list->neigh_ecgi_enb_id_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_enb_config_update_resp
(
    x2ap_enb_config_update_resp_t *p_x2ap_enb_config_update_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_enb_config_update_resp, 0, sizeof(x2ap_enb_config_update_resp_t));

    /* This function parses x2ap_enb_config_update_resp */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_enb_config_update_resp");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_enb_config_update_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_enb_config_update_resp->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_enb_config_update_resp->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_enb_config_update_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_enb_config_update_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_enb_config_update_resp->response] should be in range [%d <= 1] !", p_x2ap_enb_config_update_resp->response);
        return OAM_FAILURE;
    }

    if (p_x2ap_enb_config_update_resp->bitmask & X2AP_CAUSE_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_cause(
        &p_x2ap_enb_config_update_resp->cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_cause
(
    x2ap_cause_t *p_x2ap_cause,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_x2ap_cause, 0, sizeof(x2ap_cause_t));

    /* This function parses x2ap_cause */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_cause");

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_cause->type, p_src + *p_length_read, "type");
    *p_length_read += sizeof(UInt8);

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_cause->value, p_src + *p_length_read, "value");
    *p_length_read += sizeof(U16);

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_enb_config_update_ind
(
    x2ap_enb_config_update_ind_t *p_x2ap_enb_config_update_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_enb_config_update_ind, 0, sizeof(x2ap_enb_config_update_ind_t));

    /* This function parses x2ap_enb_config_update_ind */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_enb_config_update_ind");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_enb_config_update_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_enb_config_update_ind->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_enb_config_update_ind->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_enb_config_update_ind->bitmask & X2AP_SERVED_CELLS_TO_ADD_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_served_cells_to_add_list(
        &p_x2ap_enb_config_update_ind->add_served_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_config_update_ind->bitmask & X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_served_cells_to_modify_list(
        &p_x2ap_enb_config_update_ind->mod_served_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_config_update_ind->bitmask & X2AP_SERVED_CELLS_TO_DELETE_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_served_cells_to_delete_list(
        &p_x2ap_enb_config_update_ind->del_served_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_config_update_ind->bitmask & X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_gu_group_id_list(
        &p_x2ap_enb_config_update_ind->add_gu_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_config_update_ind->bitmask & X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_gu_group_id_list(
        &p_x2ap_enb_config_update_ind->delete_gu_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_reset_req
(
    x2ap_reset_req_t *p_x2ap_reset_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_reset_req, 0, sizeof(x2ap_reset_req_t));

    /* This function parses x2ap_reset_req */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_reset_req");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_reset_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_reset_req->gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2ap_cause(
        &p_x2ap_reset_req->cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_reset_resp
(
    x2ap_reset_resp_t *p_x2ap_reset_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_reset_resp, 0, sizeof(x2ap_reset_resp_t));

    /* This function parses x2ap_reset_resp */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_reset_resp");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_reset_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_reset_resp->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_reset_resp->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_reset_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_reset_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_reset_resp->response] should be in range [%d <= 1] !", p_x2ap_reset_resp->response);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_reset_ind
(
    x2ap_reset_ind_t *p_x2ap_reset_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_reset_ind, 0, sizeof(x2ap_reset_ind_t));

    /* This function parses x2ap_reset_ind */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_reset_ind");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_reset_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_reset_ind->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_reset_ind->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2ap_cause(
        &p_x2ap_reset_ind->cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_add_enb_req
(
    x2ap_add_enb_req_t *p_x2ap_add_enb_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_add_enb_req, 0, sizeof(x2ap_add_enb_req_t));

    /* This function parses x2ap_add_enb_req */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_add_enb_req");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_add_enb_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_add_enb_req->num_peer_enb, p_src + *p_length_read, "num_peer_enb");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_add_enb_req->num_peer_enb < 1) || (p_x2ap_add_enb_req->num_peer_enb > MAX_PEER_ENB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_add_enb_req->num_peer_enb] should be in range [1 <= %d <= MAX_PEER_ENB] !", p_x2ap_add_enb_req->num_peer_enb);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_add_enb_req->num_peer_enb; loop++)
        {
            if (OAM_FAILURE == oam_parse_x2_enb_comm_info(
                &p_x2ap_add_enb_req->enb_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2_ip_addr
(
    x2_ip_addr_t *p_x2_ip_addr,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_x2_ip_addr, 0, sizeof(x2_ip_addr_t));

    /* This function parses x2_ip_addr */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2_ip_addr");

    if (*p_length_read + (SInt32)sizeof(p_x2_ip_addr->ip_addr) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2_ip_addr->ip_addr); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2_ip_addr->ip_addr[loop], (void*)(p_src + *p_length_read), "ip_addr[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2_ipv6_addr
(
    x2_ipv6_addr_t *p_x2_ipv6_addr,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_x2_ipv6_addr, 0, sizeof(x2_ipv6_addr_t));

    /* This function parses x2_ipv6_addr */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2_ipv6_addr");

    if (*p_length_read + (SInt32)sizeof(p_x2_ipv6_addr->ipv6_addr) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2_ipv6_addr->ipv6_addr); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2_ipv6_addr->ipv6_addr[loop], (void*)(p_src + *p_length_read), "ipv6_addr[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2_trans_layer_addrs
(
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_x2_trans_layer_addrs, 0, sizeof(x2_trans_layer_addrs_t));

    /* This function parses x2_trans_layer_addrs */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2_trans_layer_addrs");

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2_trans_layer_addrs->data_length, p_src + *p_length_read, "data_length");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2_trans_layer_addrs->data_length > RRC_MAX_IP_ADDR_LEN_IPV6))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_trans_layer_addrs->data_length] should be in range [%d <= RRC_MAX_IP_ADDR_LEN_IPV6] !", p_x2_trans_layer_addrs->data_length);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)(p_x2_trans_layer_addrs->data_length * sizeof(p_x2_trans_layer_addrs->data[0])) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_trans_layer_addrs->data_length; loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2_trans_layer_addrs->data[loop], (void*)(p_src + *p_length_read), "data[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2_extend_trans_layer_addrs
(
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2_extend_trans_layer_addrs, 0, sizeof(x2_extend_trans_layer_addrs_t));

    /* This function parses x2_extend_trans_layer_addrs */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2_extend_trans_layer_addrs");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2_extend_trans_layer_addrs->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_IPSEC_ADDRS_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2_trans_layer_addrs(
        &p_x2_extend_trans_layer_addrs->ipsec_trans_layer_addrs,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_NUM_GTP_ADDRS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2_extend_trans_layer_addrs->num_tlas, p_src + *p_length_read, "num_tlas");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_extend_trans_layer_addrs->num_tlas < 1) || (p_x2_extend_trans_layer_addrs->num_tlas > MAX_NUM_ENB_X2AP_GTP_TLAS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_extend_trans_layer_addrs->num_tlas] should be in range [1 <= %d <= MAX_NUM_ENB_X2AP_GTP_TLAS] !", p_x2_extend_trans_layer_addrs->num_tlas);
        return OAM_FAILURE;
    }
    }

    if (p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_GTP_ADDRS_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_extend_trans_layer_addrs->num_tlas; loop++)
        {
            if (OAM_FAILURE == oam_parse_x2_trans_layer_addrs(
                &p_x2_extend_trans_layer_addrs->gtp_trans_layer_addrs[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2_enb_comm_info
(
    x2_enb_comm_info_t *p_x2_enb_comm_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2_enb_comm_info, 0, sizeof(x2_enb_comm_info_t));

    /* This function parses x2_enb_comm_info */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2_enb_comm_info");

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2_enb_comm_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt8);

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2_enb_comm_info->num_ip_addr, p_src + *p_length_read, "num_ip_addr");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_ip_addr < 1) || (p_x2_enb_comm_info->num_ip_addr > MAX_NUM_IP_ADDR))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_enb_comm_info->num_ip_addr] should be in range [1 <= %d <= MAX_NUM_IP_ADDR] !", p_x2_enb_comm_info->num_ip_addr);
        return OAM_FAILURE;
    }
    }

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_enb_comm_info->num_ip_addr; loop++)
        {
            if (OAM_FAILURE == oam_parse_x2_ip_addr(
                &p_x2_enb_comm_info->ip_addr[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2_enb_comm_info->port, p_src + *p_length_read, "port");
    *p_length_read += sizeof(U16);

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2_enb_comm_info->num_ipv6_addr, p_src + *p_length_read, "num_ipv6_addr");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_ipv6_addr < 1) || (p_x2_enb_comm_info->num_ipv6_addr > MAX_NUM_IP_ADDR))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_enb_comm_info->num_ipv6_addr] should be in range [1 <= %d <= MAX_NUM_IP_ADDR] !", p_x2_enb_comm_info->num_ipv6_addr);
        return OAM_FAILURE;
    }
    }

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_enb_comm_info->num_ipv6_addr; loop++)
        {
            if (OAM_FAILURE == oam_parse_x2_ipv6_addr(
                &p_x2_enb_comm_info->ipv6_addr[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_EXT_NUM_TLAS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2_enb_comm_info->num_tlas, p_src + *p_length_read, "num_tlas");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_tlas < 1) || (p_x2_enb_comm_info->num_tlas > MAX_NUM_ENB_X2AP_EXT_TLAS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_enb_comm_info->num_tlas] should be in range [1 <= %d <= MAX_NUM_ENB_X2AP_EXT_TLAS] !", p_x2_enb_comm_info->num_tlas);
        return OAM_FAILURE;
    }
    }

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_EXT_TLAS_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_enb_comm_info->num_tlas; loop++)
        {
            if (OAM_FAILURE == oam_parse_x2_extend_trans_layer_addrs(
                &p_x2_enb_comm_info->extend_trans_layer_addrs[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_DEFAULT_SGNB_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2_enb_comm_info->default_sgnb, p_src + *p_length_read, "default_sgnb");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2_enb_comm_info->default_sgnb > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_enb_comm_info->default_sgnb] should be in range [%d <= 1] !", p_x2_enb_comm_info->default_sgnb);
        return OAM_FAILURE;
    }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_add_enb_res
(
    x2ap_add_enb_res_t *p_x2ap_add_enb_res,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_add_enb_res, 0, sizeof(x2ap_add_enb_res_t));

    /* This function parses x2ap_add_enb_res */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_add_enb_res");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_add_enb_res->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_add_enb_res->gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_add_enb_res->bitmask & X2AP_SUCCESS_ENB_ADD_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_enb_comm_info_list(
        &p_x2ap_add_enb_res->success_enb_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_add_enb_res->bitmask & X2AP_FAILURE_ENB_ADD_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_enb_comm_info_list(
        &p_x2ap_add_enb_res->failure_enb_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_enb_comm_info_list
(
    x2ap_enb_comm_info_list_t *p_x2ap_enb_comm_info_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_enb_comm_info_list, 0, sizeof(x2ap_enb_comm_info_list_t));

    /* This function parses x2ap_enb_comm_info_list */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_enb_comm_info_list");

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_enb_comm_info_list->num_peer_enb, p_src + *p_length_read, "num_peer_enb");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_enb_comm_info_list->num_peer_enb < 1) || (p_x2ap_enb_comm_info_list->num_peer_enb > MAX_PEER_ENB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_enb_comm_info_list->num_peer_enb] should be in range [1 <= %d <= MAX_PEER_ENB] !", p_x2ap_enb_comm_info_list->num_peer_enb);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_enb_comm_info_list->num_peer_enb; loop++)
        {
            if (OAM_FAILURE == oam_parse_x2_enb_comm_info(
                &p_x2ap_enb_comm_info_list->enb_comm_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_delete_enb_req
(
    x2ap_delete_enb_req_t *p_x2ap_delete_enb_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_delete_enb_req, 0, sizeof(x2ap_delete_enb_req_t));

    /* This function parses x2ap_delete_enb_req */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_delete_enb_req");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_delete_enb_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_delete_enb_req->gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_delete_enb_res
(
    x2ap_delete_enb_res_t *p_x2ap_delete_enb_res,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_delete_enb_res, 0, sizeof(x2ap_delete_enb_res_t));

    /* This function parses x2ap_delete_enb_res */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_delete_enb_res");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_delete_enb_res->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_delete_enb_res->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_delete_enb_res->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_delete_enb_res->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_delete_enb_res->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_delete_enb_res->response] should be in range [%d <= 1] !", p_x2ap_delete_enb_res->response);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_enb_link_up_ind
(
    x2ap_enb_link_up_ind_t *p_x2ap_enb_link_up_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_enb_link_up_ind, 0, sizeof(x2ap_enb_link_up_ind_t));

    /* This function parses x2ap_enb_link_up_ind */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_enb_link_up_ind");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_enb_link_up_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_enb_link_up_ind->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_enb_link_up_ind->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2_enb_comm_info(
        &p_x2ap_enb_link_up_ind->enb_comm_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_enb_link_up_ind->num_served_cell, p_src + *p_length_read, "num_served_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_enb_link_up_ind->num_served_cell < 1) || (p_x2ap_enb_link_up_ind->num_served_cell > MAX_SERVED_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_enb_link_up_ind->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_enb_link_up_ind->num_served_cell);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_enb_link_up_ind->num_served_cell; loop++)
        {
            if (OAM_FAILURE == oam_parse_x2ap_served_cell_info_arr(
                &p_x2ap_enb_link_up_ind->served_cell_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (p_x2ap_enb_link_up_ind->bitmask & X2AP_GU_GROUP_ID_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_gu_group_id_list(
        &p_x2ap_enb_link_up_ind->gu_group_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_enb_link_down_ind
(
    x2ap_enb_link_down_ind_t *p_x2ap_enb_link_down_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_enb_link_down_ind, 0, sizeof(x2ap_enb_link_down_ind_t));

    /* This function parses x2ap_enb_link_down_ind */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_enb_link_down_ind");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_enb_link_down_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_enb_link_down_ind->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_enb_link_down_ind->bitmask & X2AP_PEER_ENB_ID_INFO_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_enb_link_down_ind->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_enb_link_down_ind->bitmask & X2AP_PEER_ENB_COMM_INFO_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2_enb_comm_info(
        &p_x2ap_enb_link_down_ind->enb_comm_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_enb_config_update_wait_ind
(
    x2ap_enb_config_update_wait_ind_t *p_x2ap_enb_config_update_wait_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_enb_config_update_wait_ind, 0, sizeof(x2ap_enb_config_update_wait_ind_t));

    /* This function parses x2ap_enb_config_update_wait_ind */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_enb_config_update_wait_ind");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_enb_config_update_wait_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_enb_config_update_wait_ind->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_enb_config_update_wait_ind->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_enb_config_update_wait_ind->time_to_wait, p_src + *p_length_read, "time_to_wait");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_enb_config_update_wait_ind->time_to_wait > 2))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_enb_config_update_wait_ind->time_to_wait] should be in range [%d <= 2] !", p_x2ap_enb_config_update_wait_ind->time_to_wait);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_ho_report_ind
(
    x2ap_ho_report_ind_t *p_x2ap_ho_report_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_ho_report_ind, 0, sizeof(x2ap_ho_report_ind_t));

    /* This function parses x2ap_ho_report_ind */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_ho_report_ind");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_ho_report_ind->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_ho_report_ind->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_ho_report_ind->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(U32) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_ho_report_ind->ho_report_type, p_src + *p_length_read, "ho_report_type");
    *p_length_read += sizeof(U32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_ho_report_ind->ho_report_type > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_ho_report_ind->ho_report_type] should be in range [%d <= 1] !", p_x2ap_ho_report_ind->ho_report_type);
        return OAM_FAILURE;
    }

    if (OAM_FAILURE == oam_parse_x2ap_cause(
        &p_x2ap_ho_report_ind->ho_cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2ap_ecgi(
        &p_x2ap_ho_report_ind->src_ecgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2ap_ecgi(
        &p_x2ap_ho_report_ind->fail_cell_ecgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_ho_report_ind->bitmask & X2AP_HO_REPORT_REEST_CELL_ECGI_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_ecgi(
        &p_x2ap_ho_report_ind->re_est_cell_ecgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_peer_cell_activation_req
(
    x2ap_peer_cell_activation_req_t *p_x2ap_peer_cell_activation_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_peer_cell_activation_req, 0, sizeof(x2ap_peer_cell_activation_req_t));

    /* This function parses x2ap_peer_cell_activation_req */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_peer_cell_activation_req");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_peer_cell_activation_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_peer_cell_activation_req->gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2ap_cell_activation_info(
        &p_x2ap_peer_cell_activation_req->peer_cell_activation_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_cell_activation_info
(
    x2ap_cell_activation_info_t *p_x2ap_cell_activation_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_cell_activation_info, 0, sizeof(x2ap_cell_activation_info_t));

    /* This function parses x2ap_cell_activation_info */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_cell_activation_info");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_cell_activation_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_cell_activation_info->num_served_cell, p_src + *p_length_read, "num_served_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_cell_activation_info->num_served_cell < 1) || (p_x2ap_cell_activation_info->num_served_cell > MAX_SERVED_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_cell_activation_info->num_served_cell] should be in range [1 <= %d <= MAX_SERVED_CELLS] !", p_x2ap_cell_activation_info->num_served_cell);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_cell_activation_info->num_served_cell; loop++)
        {
            if (OAM_FAILURE == oam_parse_x2ap_ecgi(
                &p_x2ap_cell_activation_info->served_cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_peer_cell_activation_resp
(
    x2ap_peer_cell_activation_resp_t *p_x2ap_peer_cell_activation_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_peer_cell_activation_resp, 0, sizeof(x2ap_peer_cell_activation_resp_t));

    /* This function parses x2ap_peer_cell_activation_resp */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_peer_cell_activation_resp");

    if (OAM_FAILURE == oam_parse_x2ap_cell_activation_resp(
        &p_x2ap_peer_cell_activation_resp->peer_cell_activation_resp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_cell_activation_resp
(
    x2ap_cell_activation_resp_t *p_x2ap_cell_activation_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_cell_activation_resp, 0, sizeof(x2ap_cell_activation_resp_t));

    /* This function parses x2ap_cell_activation_resp */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_cell_activation_resp");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_cell_activation_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_cell_activation_resp->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_cell_activation_resp->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_cell_activation_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_cell_activation_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_cell_activation_resp->response] should be in range [%d <= 1] !", p_x2ap_cell_activation_resp->response);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_cell_activation_resp->num_served_cell, p_src + *p_length_read, "num_served_cell");
    *p_length_read += sizeof(U16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_cell_activation_resp->num_served_cell > MAX_SERVED_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_cell_activation_resp->num_served_cell] should be in range [%d <= MAX_SERVED_CELLS] !", p_x2ap_cell_activation_resp->num_served_cell);
        return OAM_FAILURE;
    }

    if (p_x2ap_cell_activation_resp->bitmask & X2AP_CELL_ACTIVATION_SERVED_CELL_LIST_PRESENT)
    {

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_cell_activation_resp->num_served_cell; loop++)
        {
            if (OAM_FAILURE == oam_parse_x2ap_ecgi(
                &p_x2ap_cell_activation_resp->served_cell_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }
    }

    if (p_x2ap_cell_activation_resp->bitmask & X2AP_CELL_ACTIVATION_CAUSE_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_cause(
        &p_x2ap_cell_activation_resp->cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_cell_activation_req
(
    x2ap_cell_activation_req_t *p_x2ap_cell_activation_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_cell_activation_req, 0, sizeof(x2ap_cell_activation_req_t));

    /* This function parses x2ap_cell_activation_req */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_cell_activation_req");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_cell_activation_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_cell_activation_req->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_cell_activation_req->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2ap_cell_activation_info(
        &p_x2ap_cell_activation_req->cell_activation_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_cell_activation_response
(
    x2ap_cell_activation_response_t *p_x2ap_cell_activation_response,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_cell_activation_response, 0, sizeof(x2ap_cell_activation_response_t));

    /* This function parses x2ap_cell_activation_response */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_cell_activation_response");

    if (OAM_FAILURE == oam_parse_x2ap_cell_activation_resp(
        &p_x2ap_cell_activation_response->cell_activation_resp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_mobility_change_req
(
    x2ap_mobility_change_req_t *p_x2ap_mobility_change_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_mobility_change_req, 0, sizeof(x2ap_mobility_change_req_t));

    /* This function parses x2ap_mobility_change_req */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_mobility_change_req");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_mobility_change_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_mobility_change_req->gb_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2ap_mobility_change_info(
        &p_x2ap_mobility_change_req->mobility_change_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_mob_param
(
    x2ap_mob_param_t *p_x2ap_mob_param,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_x2ap_mob_param, 0, sizeof(x2ap_mob_param_t));

    /* This function parses x2ap_mob_param */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_mob_param");

    if (*p_length_read + (SInt32)sizeof(SInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_SINT8(&p_x2ap_mob_param->handover_trigger_change, p_src + *p_length_read, "handover_trigger_change");
    *p_length_read += sizeof(SInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_mob_param->handover_trigger_change < -20) || (p_x2ap_mob_param->handover_trigger_change > 20))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_mob_param->handover_trigger_change] should be in range [-20 <= %d <= 20] !", p_x2ap_mob_param->handover_trigger_change);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_mobility_change_info
(
    x2ap_mobility_change_info_t *p_x2ap_mobility_change_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_mobility_change_info, 0, sizeof(x2ap_mobility_change_info_t));

    /* This function parses x2ap_mobility_change_info */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_mobility_change_info");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_mobility_change_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2ap_ecgi(
        &p_x2ap_mobility_change_info->enb1_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2ap_ecgi(
        &p_x2ap_mobility_change_info->enb2_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_mobility_change_info->bitmask & X2AP_ENB1_MOBILITY_PARAMETERS_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_mob_param(
        &p_x2ap_mobility_change_info->enb1_mob_param,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (OAM_FAILURE == oam_parse_x2ap_mob_param(
        &p_x2ap_mobility_change_info->enb2_mob_param,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2ap_cause(
        &p_x2ap_mobility_change_info->cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_mobility_change_resp
(
    x2ap_mobility_change_resp_t *p_x2ap_mobility_change_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_mobility_change_resp, 0, sizeof(x2ap_mobility_change_resp_t));

    /* This function parses x2ap_mobility_change_resp */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_mobility_change_resp");

    if (OAM_FAILURE == oam_parse_x2ap_mob_ch_resp(
        &p_x2ap_mobility_change_resp->mobility_change_resp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_mob_range
(
    x2ap_mob_range_t *p_x2ap_mob_range,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_memset(p_x2ap_mob_range, 0, sizeof(x2ap_mob_range_t));

    /* This function parses x2ap_mob_range */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_mob_range");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_mob_range->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (*p_length_read + (SInt32)sizeof(SInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_SINT8(&p_x2ap_mob_range->ho_trigger_change_lower_limit, p_src + *p_length_read, "ho_trigger_change_lower_limit");
    *p_length_read += sizeof(SInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_mob_range->ho_trigger_change_lower_limit < -20) || (p_x2ap_mob_range->ho_trigger_change_lower_limit > 20))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_mob_range->ho_trigger_change_lower_limit] should be in range [-20 <= %d <= 20] !", p_x2ap_mob_range->ho_trigger_change_lower_limit);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(SInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_SINT8(&p_x2ap_mob_range->ho_trigger_change_upper_limit, p_src + *p_length_read, "ho_trigger_change_upper_limit");
    *p_length_read += sizeof(SInt8);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_mob_range->ho_trigger_change_upper_limit < -20) || (p_x2ap_mob_range->ho_trigger_change_upper_limit > 20))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_mob_range->ho_trigger_change_upper_limit] should be in range [-20 <= %d <= 20] !", p_x2ap_mob_range->ho_trigger_change_upper_limit);
        return OAM_FAILURE;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_parse_x2ap_mob_ch_resp
(
    x2ap_mob_ch_resp_t *p_x2ap_mob_ch_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_mob_ch_resp, 0, sizeof(x2ap_mob_ch_resp_t));

    /* This function parses x2ap_mob_ch_resp */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_mob_ch_resp");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_mob_ch_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_mob_ch_resp->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_mob_ch_resp->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2ap_ecgi(
        &p_x2ap_mob_ch_resp->enb1_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2ap_ecgi(
        &p_x2ap_mob_ch_resp->enb2_cell_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_mob_ch_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    if (p_x2ap_mob_ch_resp->bitmask & X2AP_MOBILITY_SETTING_CAUSE_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_cause(
        &p_x2ap_mob_ch_resp->cause,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_mob_ch_resp->bitmask & X2AP_MOBILITY_PARAM_MOD_RANGE_PRESENT)
    {

    if (OAM_FAILURE == oam_parse_x2ap_mob_range(
        &p_x2ap_mob_ch_resp->enb2_mob_param_mod_range,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_mobility_setting_change_req
(
    x2ap_mobility_setting_change_req_t *p_x2ap_mobility_setting_change_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_mobility_setting_change_req, 0, sizeof(x2ap_mobility_setting_change_req_t));

    /* This function parses x2ap_mobility_setting_change_req */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_mobility_setting_change_req");

    if (*p_length_read + (SInt32)sizeof(U16) > length_left)
    {
       OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_mobility_setting_change_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(U16);

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_mobility_setting_change_req->enb1_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2_gb_enb_id(
        &p_x2ap_mobility_setting_change_req->enb2_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_parse_x2ap_mobility_change_info(
        &p_x2ap_mobility_setting_change_req->mobility_change_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_parse_x2ap_mobility_setting_change_resp
(
    x2ap_mobility_setting_change_resp_t *p_x2ap_mobility_setting_change_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;


    *p_length_read = 0;

    oam_memset(p_x2ap_mobility_setting_change_resp, 0, sizeof(x2ap_mobility_setting_change_resp_t));

    /* This function parses x2ap_mobility_setting_change_resp */
    OAM_LOG(OAM,OAM_COMP_PARSE, "Parsing x2ap_mobility_setting_change_resp");

    if (OAM_FAILURE == oam_parse_x2ap_mob_ch_resp(
        &p_x2ap_mobility_setting_change_resp->mobility_setting_change_resp,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
    OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

