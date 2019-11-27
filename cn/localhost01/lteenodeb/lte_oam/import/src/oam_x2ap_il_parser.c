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
 *  File Description : The file rrc_x2apOam_il_parser.c contains the definitions 
 *                     of rrc-x2apOam interface message parsing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#include "oam_x2ap_il_parser.h"
#include "rrc_x2apOam_intf.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
#define PUP_TL_LENGTH       (sizeof(UInt16) + sizeof(UInt16))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
oam_return_et
oam_rrc_il_parse_x2_ip_addr
(
    x2_ip_addr_t *p_x2_ip_addr,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2_ipv6_addr
(
    x2_ipv6_addr_t *p_x2_ipv6_addr,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2_trans_layer_addrs
(
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2_extend_trans_layer_addrs
(
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2_enb_comm_info
(
    x2_enb_comm_info_t *p_x2_enb_comm_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_sctp_conf_info
(
    x2ap_sctp_conf_info_t *p_x2ap_sctp_conf_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_con_tim_info
(
    x2ap_con_tim_info_t *p_x2ap_con_tim_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_conf_param
(
    x2ap_conf_param_t *p_x2ap_conf_param,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_plmn_identity
(
    x2ap_plmn_identity_t *p_x2ap_plmn_identity,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_macro_enb_id
(
    macro_enb_id_t *p_macro_enb_id,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_home_enb_id
(
    home_enb_id_t *p_home_enb_id,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2_enb_id_info
(
    x2_enb_id_info_t *p_x2_enb_id_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2_gb_enb_id
(
    x2_gb_enb_id_t *p_x2_gb_enb_id,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_tac
(
    x2ap_tac_t *p_x2ap_tac,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_broadcast_plmn
(
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_earfcn
(
    x2ap_earfcn_t *p_x2ap_earfcn,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_fdd_info
(
    x2ap_fdd_info_t *p_x2ap_fdd_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_special_subframe_info
(
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_tdd_info
(
    x2ap_tdd_info_t *p_x2ap_tdd_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_choice_eutra_mode
(
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_choice_eutra_mode_info
(
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_prach_configuration_info
(
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_subframe_allocation
(
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_mbsfn_info
(
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_mbsfn_subframe_info
(
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2apOam_srvd_cell_info
(
    x2apOam_srvd_cell_info_t *p_x2apOam_srvd_cell_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_ecgi
(
    x2ap_ecgi_t *p_x2ap_ecgi,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_neighbour_info
(
    x2ap_neighbour_info_t *p_x2ap_neighbour_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_nbour_cell_arry
(
    x2ap_nbour_cell_arry_t *p_x2ap_nbour_cell_arry,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2apOam_srvd_cell_arry
(
    x2apOam_srvd_cell_arry_t *p_x2apOam_srvd_cell_arry,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2apOam_srvd_cells
(
    x2apOam_srvd_cells_t *p_x2apOam_srvd_cells,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_gu_group_id
(
    x2ap_gu_group_id_t *p_x2ap_gu_group_id,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_gu_grp_ids
(
    x2ap_gu_grp_ids_t *p_x2ap_gu_grp_ids,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_info
(
    x2ap_info_t *p_x2ap_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_son_comm_info
(
    son_comm_info_t *p_son_comm_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2_gnb_id
(
    x2_gnb_id_t *p_x2_gnb_id,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2_gb_gnb_id
(
    x2_gb_gnb_id_t *p_x2_gb_gnb_id,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_oam_gb_enb_id_list_info
(
    x2ap_oam_gb_enb_id_list_info_t *p_x2ap_oam_gb_enb_id_list_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_oam_gb_gnb_id_list_info
(
    x2ap_oam_gb_gnb_id_list_info_t *p_x2ap_oam_gb_gnb_id_list_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_rrc_timeval
(
    rrc_timeval_t *p_rrc_timeval,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_oam_gb_enb_id_status_params
(
    x2ap_oam_gb_enb_id_status_params_t *p_x2ap_oam_gb_enb_id_status_params,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_oam_gb_enb_status_params_list_info
(
    x2ap_oam_gb_enb_status_params_list_info_t *p_x2ap_oam_gb_enb_status_params_list_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_oam_failed_gb_enb_info
(
    x2ap_oam_failed_gb_enb_info_t *p_x2ap_oam_failed_gb_enb_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_oam_gb_enb_failed_list
(
    x2ap_oam_gb_enb_failed_list_t *p_x2ap_oam_gb_enb_failed_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_oam_gb_gnb_id_status_params
(
    x2ap_oam_gb_gnb_id_status_params_t *p_x2ap_oam_gb_gnb_id_status_params,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_oam_gb_gnb_status_params_list_info
(
    x2ap_oam_gb_gnb_status_params_list_info_t *p_x2ap_oam_gb_gnb_status_params_list_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_oam_failed_gb_gnb_info
(
    x2ap_oam_failed_gb_gnb_info_t *p_x2ap_oam_failed_gb_gnb_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_return_et
oam_rrc_il_parse_x2ap_oam_gb_gnb_failed_list
(
    x2ap_oam_gb_gnb_failed_list_t *p_x2ap_oam_gb_gnb_failed_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

oam_return_et
oam_rrc_il_parse_x2ap_oam_provision_req
(
    x2ap_oam_provision_req_t *p_x2ap_oam_provision_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_oam_provision_req, 0, sizeof(x2ap_oam_provision_req_t));

    /* This function parses x2ap_oam_provision_req */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_oam_provision_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (p_x2ap_oam_provision_req->bitmask & RRC_X2AP_PROVISION_INFO_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_info(
        &p_x2ap_oam_provision_req->x2ap_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_oam_provision_req->bitmask & SON_MODULE_INFO_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_son_comm_info(
        &p_x2ap_oam_provision_req->son_comm_info,
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
oam_rrc_il_parse_x2_ip_addr
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
oam_rrc_il_parse_x2_ipv6_addr
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
oam_rrc_il_parse_x2_trans_layer_addrs
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_trans_layer_addrs->data_length] should be less than"
            " or equal to RRC_MAX_IP_ADDR_LEN_IPV6. Incorrect value %u received.", p_x2_trans_layer_addrs->data_length);
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
oam_rrc_il_parse_x2_extend_trans_layer_addrs
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

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2_extend_trans_layer_addrs->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_IPSEC_ADDRS_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_x2_trans_layer_addrs(
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_extend_trans_layer_addrs->num_tlas] should be in range "
            "1 to MAX_NUM_ENB_X2AP_GTP_TLAS. Incorrect value %u received.", p_x2_extend_trans_layer_addrs->num_tlas);
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
            if (OAM_FAILURE == oam_rrc_il_parse_x2_trans_layer_addrs(
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
oam_rrc_il_parse_x2_enb_comm_info
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_enb_comm_info->num_ip_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %u received.", p_x2_enb_comm_info->num_ip_addr);
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
            if (OAM_FAILURE == oam_rrc_il_parse_x2_ip_addr(
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

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2_enb_comm_info->port, p_src + *p_length_read, "port");
    *p_length_read += sizeof(UInt16);

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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_enb_comm_info->num_ipv6_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %u received.", p_x2_enb_comm_info->num_ipv6_addr);
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
            if (OAM_FAILURE == oam_rrc_il_parse_x2_ipv6_addr(
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_enb_comm_info->num_tlas] should be in range "
            "1 to MAX_NUM_ENB_X2AP_EXT_TLAS. Incorrect value %u received.", p_x2_enb_comm_info->num_tlas);
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
            if (OAM_FAILURE == oam_rrc_il_parse_x2_extend_trans_layer_addrs(
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_enb_comm_info->default_sgnb] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2_enb_comm_info->default_sgnb);
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
oam_rrc_il_parse_x2ap_sctp_conf_info
(
    x2ap_sctp_conf_info_t *p_x2ap_sctp_conf_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_x2ap_sctp_conf_info, 0, sizeof(x2ap_sctp_conf_info_t));

    /* This function parses x2ap_sctp_conf_info */

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_sctp_conf_info->hbinterval, p_src + *p_length_read, "hbinterval");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_sctp_conf_info->pathmaxrxt, p_src + *p_length_read, "pathmaxrxt");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_sctp_conf_info->init_num_ostreams, p_src + *p_length_read, "init_num_ostreams");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_sctp_conf_info->init_max_instreams, p_src + *p_length_read, "init_max_instreams");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_sctp_conf_info->init_max_attempts, p_src + *p_length_read, "init_max_attempts");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_sctp_conf_info->init_max_init_timeo, p_src + *p_length_read, "init_max_init_timeo");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_sctp_conf_info->rto_initial, p_src + *p_length_read, "rto_initial");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_sctp_conf_info->rto_max, p_src + *p_length_read, "rto_max");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_sctp_conf_info->rto_min, p_src + *p_length_read, "rto_min");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_sctp_conf_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (p_x2ap_sctp_conf_info->bitmask & X2AP_SCTP_ASSOC_MAX_RETRANS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_sctp_conf_info->assoc_max_retrans, p_src + *p_length_read, "assoc_max_retrans");
    *p_length_read += sizeof(UInt16);
    }

    if (p_x2ap_sctp_conf_info->bitmask & X2AP_SCTP_VAL_COOKIE_LIFE_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_sctp_conf_info->valid_cookie_life, p_src + *p_length_read, "valid_cookie_life");
    *p_length_read += sizeof(UInt32);
    }

    if (p_x2ap_sctp_conf_info->bitmask & X2AP_OAM_DSCP_VALUE_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_sctp_conf_info->x2ap_dscp_value, p_src + *p_length_read, "x2ap_dscp_value");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_sctp_conf_info->x2ap_dscp_value > 63))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_sctp_conf_info->x2ap_dscp_value] should be less than"
            " or equal to 63. Incorrect value %u received.", p_x2ap_sctp_conf_info->x2ap_dscp_value);
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
oam_rrc_il_parse_x2ap_con_tim_info
(
    x2ap_con_tim_info_t *p_x2ap_con_tim_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_x2ap_con_tim_info, 0, sizeof(x2ap_con_tim_info_t));

    /* This function parses x2ap_con_tim_info */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_con_tim_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_con_tim_info->sctp_connect_tim_val, p_src + *p_length_read, "sctp_connect_tim_val");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_con_tim_info->x2ap_csi_guard_tim_val, p_src + *p_length_read, "x2ap_csi_guard_tim_val");
    *p_length_read += sizeof(UInt32);

    if (p_x2ap_con_tim_info->bitmask & X2AP_TIME_TO_WAIT_INDEX_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_con_tim_info->x2ap_time_to_wait_index, p_src + *p_length_read, "x2ap_time_to_wait_index");
    *p_length_read += sizeof(UInt8);
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_con_tim_info->x2ap_setup_retry_tim_val, p_src + *p_length_read, "x2ap_setup_retry_tim_val");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_con_tim_info->sctp_connect_retry_cnt, p_src + *p_length_read, "sctp_connect_retry_cnt");
    *p_length_read += sizeof(UInt8);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_con_tim_info->x2ap_setup_retry_cnt, p_src + *p_length_read, "x2ap_setup_retry_cnt");
    *p_length_read += sizeof(UInt8);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_con_tim_info->eNB_Config_Update_retry_cnt, p_src + *p_length_read, "eNB_Config_Update_retry_cnt");
    *p_length_read += sizeof(UInt8);

    if (p_x2ap_con_tim_info->bitmask & X2AP_ENB_CFG_INFO_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_con_tim_info->eNB_Config_Update_retry_timer_val, p_src + *p_length_read, "eNB_Config_Update_retry_timer_val");
    *p_length_read += sizeof(UInt32);
    }

    if (p_x2ap_con_tim_info->bitmask & X2AP_ENB_CFG_INFO_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_con_tim_info->eNB_Config_Update_time_to_wait_retry_cnt, p_src + *p_length_read, "eNB_Config_Update_time_to_wait_retry_cnt");
    *p_length_read += sizeof(UInt8);
    }

    if (p_x2ap_con_tim_info->bitmask & X2AP_ENB_CFG_INFO_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_con_tim_info->eNB_Config_Update_time_to_wait_offset, p_src + *p_length_read, "eNB_Config_Update_time_to_wait_offset");
    *p_length_read += sizeof(UInt32);
    }

    if (p_x2ap_con_tim_info->bitmask & X2AP_RESET_CFG_INFO_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_con_tim_info->x2ap_enb_reset_retry_cnt, p_src + *p_length_read, "x2ap_enb_reset_retry_cnt");
    *p_length_read += sizeof(UInt8);
    }

    if (p_x2ap_con_tim_info->bitmask & X2AP_RESET_CFG_INFO_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_con_tim_info->x2ap_enb_reset_guard_tim_val, p_src + *p_length_read, "x2ap_enb_reset_guard_tim_val");
    *p_length_read += sizeof(UInt32);
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_con_tim_info->x2ap_rsu_enb_guard_tim_val, p_src + *p_length_read, "x2ap_rsu_enb_guard_tim_val");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_con_tim_info->x2ap_rsu_rrm_guard_tim_val, p_src + *p_length_read, "x2ap_rsu_rrm_guard_tim_val");
    *p_length_read += sizeof(UInt32);

    if (p_x2ap_con_tim_info->bitmask & X2AP_CELL_ACTIVATION_TIMER_INFO_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_con_tim_info->x2ap_cell_activation_retry_cnt, p_src + *p_length_read, "x2ap_cell_activation_retry_cnt");
    *p_length_read += sizeof(UInt32);
    }

    if (p_x2ap_con_tim_info->bitmask & X2AP_CELL_ACTIVATION_TIMER_INFO_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_con_tim_info->x2ap_cell_activation_guard_tim_val, p_src + *p_length_read, "x2ap_cell_activation_guard_tim_val");
    *p_length_read += sizeof(UInt32);
    }

    if (p_x2ap_con_tim_info->bitmask & X2AP_MOBILITY_CHANGE_INFO_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_con_tim_info->x2ap_mobility_setting_change_retry_cnt, p_src + *p_length_read, "x2ap_mobility_setting_change_retry_cnt");
    *p_length_read += sizeof(UInt8);
    }

    if (p_x2ap_con_tim_info->bitmask & X2AP_MOBILITY_CHANGE_INFO_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_con_tim_info->x2ap_mobility_setting_change_guard_tim_val, p_src + *p_length_read, "x2ap_mobility_setting_change_guard_tim_val");
    *p_length_read += sizeof(UInt32);
    }

    if (p_x2ap_con_tim_info->bitmask & X2AP_OAM_X2_HEALTH_MONITORING_TIMER_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_con_tim_info->x2ap_health_mon_time_int, p_src + *p_length_read, "x2ap_health_mon_time_int");
    *p_length_read += sizeof(UInt16);
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
oam_rrc_il_parse_x2ap_conf_param
(
    x2ap_conf_param_t *p_x2ap_conf_param,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_conf_param, 0, sizeof(x2ap_conf_param_t));

    /* This function parses x2ap_conf_param */

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_sctp_conf_info(
        &p_x2ap_conf_param->sctp_conf_param,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_con_tim_info(
        &p_x2ap_conf_param->timer_conf_info,
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
oam_rrc_il_parse_x2ap_plmn_identity
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
oam_rrc_il_parse_macro_enb_id
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
oam_rrc_il_parse_home_enb_id
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
oam_rrc_il_parse_x2_enb_id_info
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

    if (OAM_FAILURE == oam_rrc_il_parse_macro_enb_id(
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

    if (OAM_FAILURE == oam_rrc_il_parse_home_enb_id(
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
oam_rrc_il_parse_x2_gb_enb_id
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

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_plmn_identity(
        &p_x2_gb_enb_id->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_rrc_il_parse_x2_enb_id_info(
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
oam_rrc_il_parse_x2ap_tac
(
    x2ap_tac_t *p_x2ap_tac,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_x2ap_tac, 0, sizeof(x2ap_tac_t));

    /* This function parses x2ap_tac */

    if (*p_length_read + (SInt32)sizeof(p_x2ap_tac->tac) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_tac->tac); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2ap_tac->tac[loop], (void*)(p_src + *p_length_read), "tac[]");
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
oam_rrc_il_parse_x2ap_broadcast_plmn
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_broadcast_plmn->num_bcast_plmn_id] should be in range "
            "1 to MAX_NUM_BPLMN. Incorrect value %u received.", p_x2ap_broadcast_plmn->num_bcast_plmn_id);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_broadcast_plmn->num_bcast_plmn_id; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_x2ap_plmn_identity(
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
oam_rrc_il_parse_x2ap_earfcn
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

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_earfcn->earfcn, p_src + *p_length_read, "earfcn");
    *p_length_read += sizeof(UInt32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_earfcn->earfcn > MAX_EARFCN))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_earfcn->earfcn] should be less than"
            " or equal to MAX_EARFCN. Incorrect value %u received.", p_x2ap_earfcn->earfcn);
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
oam_rrc_il_parse_x2ap_fdd_info
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

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_earfcn(
        &p_x2ap_fdd_info->ul_earfcn,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_earfcn(
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_fdd_info->ul_trnsmission_bdwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_fdd_info->ul_trnsmission_bdwidth);
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_fdd_info->dl_trnsmission_bdwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_fdd_info->dl_trnsmission_bdwidth);
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
oam_rrc_il_parse_x2ap_special_subframe_info
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_special_subframe_info->special_subframe_patterns] should be less than"
            " or equal to 8. Incorrect value %u received.", p_x2ap_special_subframe_info->special_subframe_patterns);
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_special_subframe_info->dl_cp] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_special_subframe_info->dl_cp);
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_special_subframe_info->ul_cp] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_special_subframe_info->ul_cp);
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
oam_rrc_il_parse_x2ap_tdd_info
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

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_earfcn(
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_tdd_info->trnsmission_bdwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_tdd_info->trnsmission_bdwidth);
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_tdd_info->subframe_assn] should be less than"
            " or equal to 6. Incorrect value %u received.", p_x2ap_tdd_info->subframe_assn);
        return OAM_FAILURE;
    }

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_special_subframe_info(
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
oam_rrc_il_parse_x2ap_choice_eutra_mode
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

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_fdd_info(
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

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_tdd_info(
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
oam_rrc_il_parse_x2ap_choice_eutra_mode_info
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

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_choice_eutra_mode(
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
oam_rrc_il_parse_x2ap_prach_configuration_info
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

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_prach_configuration_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_prach_configuration_info->root_sequence_index, p_src + *p_length_read, "root_sequence_index");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->root_sequence_index > 837))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_prach_configuration_info->root_sequence_index] should be less than"
            " or equal to 837. Incorrect value %u received.", p_x2ap_prach_configuration_info->root_sequence_index);
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_prach_configuration_info->zero_correlation_zone_config] should be less than"
            " or equal to 15. Incorrect value %u received.", p_x2ap_prach_configuration_info->zero_correlation_zone_config);
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_prach_configuration_info->high_speed_flag] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_prach_configuration_info->high_speed_flag);
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_prach_configuration_info->prach_frequency_offset] should be less than"
            " or equal to 94. Incorrect value %u received.", p_x2ap_prach_configuration_info->prach_frequency_offset);
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_prach_configuration_info->prach_config_index] should be less than"
            " or equal to 63. Incorrect value %u received.", p_x2ap_prach_configuration_info->prach_config_index);
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
oam_rrc_il_parse_x2ap_subframe_allocation
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

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_subframe_allocation->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

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
oam_rrc_il_parse_x2ap_mbsfn_info
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_mbsfn_info->radioframe_alloc_period] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_mbsfn_info->radioframe_alloc_period);
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_mbsfn_info->radioframe_alloc_offset] should be less than"
            " or equal to 7. Incorrect value %u received.", p_x2ap_mbsfn_info->radioframe_alloc_offset);
        return OAM_FAILURE;
    }

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_subframe_allocation(
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
oam_rrc_il_parse_x2ap_mbsfn_subframe_info
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_mbsfn_subframe_info->num_mbsfn] should be less than"
            " or equal to MAX_NUM_MBSFN. Incorrect value %u received.", p_x2ap_mbsfn_subframe_info->num_mbsfn);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_mbsfn_subframe_info->num_mbsfn; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_x2ap_mbsfn_info(
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
oam_rrc_il_parse_x2apOam_srvd_cell_info
(
    x2apOam_srvd_cell_info_t *p_x2apOam_srvd_cell_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2apOam_srvd_cell_info, 0, sizeof(x2apOam_srvd_cell_info_t));

    /* This function parses x2apOam_srvd_cell_info */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2apOam_srvd_cell_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt8);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2apOam_srvd_cell_info->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(UInt32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2apOam_srvd_cell_info->pci > 503))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2apOam_srvd_cell_info->pci] should be less than"
            " or equal to 503. Incorrect value %u received.", p_x2apOam_srvd_cell_info->pci);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2apOam_srvd_cell_info->cell_id, p_src + *p_length_read, "cell_id");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2apOam_srvd_cell_info->cell_id > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2apOam_srvd_cell_info->cell_id] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_x2apOam_srvd_cell_info->cell_id);
        return OAM_FAILURE;
    }

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_tac(
        &p_x2apOam_srvd_cell_info->track_area_code,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_broadcast_plmn(
        &p_x2apOam_srvd_cell_info->broadcast_plmn_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_choice_eutra_mode_info(
        &p_x2apOam_srvd_cell_info->choice_eutra_mode,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2apOam_srvd_cell_info->bitmask & NUM_ANTENNA_PORT_BITMASK)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2apOam_srvd_cell_info->num_antenna_port, p_src + *p_length_read, "num_antenna_port");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2apOam_srvd_cell_info->num_antenna_port > 4))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2apOam_srvd_cell_info->num_antenna_port] should be less than"
            " or equal to 4. Incorrect value %u received.", p_x2apOam_srvd_cell_info->num_antenna_port);
        return OAM_FAILURE;
    }
    }

    if (p_x2apOam_srvd_cell_info->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_prach_configuration_info(
        &p_x2apOam_srvd_cell_info->prach_config,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2apOam_srvd_cell_info->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_mbsfn_subframe_info(
        &p_x2apOam_srvd_cell_info->mbsfn_subframe_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2apOam_srvd_cell_info->bitmask & X2AP_CSG_ID_INFO_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(p_x2apOam_srvd_cell_info->csg_identity) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2apOam_srvd_cell_info->csg_identity); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2apOam_srvd_cell_info->csg_identity[loop], (void*)(p_src + *p_length_read), "csg_identity[]");
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
oam_rrc_il_parse_x2ap_ecgi
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

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_plmn_identity(
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
oam_rrc_il_parse_x2ap_neighbour_info
(
    x2ap_neighbour_info_t *p_x2ap_neighbour_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_neighbour_info, 0, sizeof(x2ap_neighbour_info_t));

    /* This function parses x2ap_neighbour_info */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_neighbour_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt8);

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_ecgi(
        &p_x2ap_neighbour_info->ecgi,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_neighbour_info->pci, p_src + *p_length_read, "pci");
    *p_length_read += sizeof(UInt32);

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_earfcn(
        &p_x2ap_neighbour_info->earfcn,
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
oam_rrc_il_parse_x2ap_nbour_cell_arry
(
    x2ap_nbour_cell_arry_t *p_x2ap_nbour_cell_arry,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_nbour_cell_arry, 0, sizeof(x2ap_nbour_cell_arry_t));

    /* This function parses x2ap_nbour_cell_arry */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_nbour_cell_arry->num_neighbour_cell, p_src + *p_length_read, "num_neighbour_cell");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nbour_cell_arry->num_neighbour_cell > MAX_NEIGHBOURS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_nbour_cell_arry->num_neighbour_cell] should be less than"
            " or equal to MAX_NEIGHBOURS. Incorrect value %u received.", p_x2ap_nbour_cell_arry->num_neighbour_cell);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nbour_cell_arry->num_neighbour_cell; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_x2ap_neighbour_info(
                &p_x2ap_nbour_cell_arry->neighbour_info_arr[loop],
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
oam_rrc_il_parse_x2apOam_srvd_cell_arry
(
    x2apOam_srvd_cell_arry_t *p_x2apOam_srvd_cell_arry,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2apOam_srvd_cell_arry, 0, sizeof(x2apOam_srvd_cell_arry_t));

    /* This function parses x2apOam_srvd_cell_arry */

    if (OAM_FAILURE == oam_rrc_il_parse_x2apOam_srvd_cell_info(
        &p_x2apOam_srvd_cell_arry->served_cell_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_nbour_cell_arry(
        &p_x2apOam_srvd_cell_arry->nbour_cell_info,
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
oam_rrc_il_parse_x2apOam_srvd_cells
(
    x2apOam_srvd_cells_t *p_x2apOam_srvd_cells,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2apOam_srvd_cells, 0, sizeof(x2apOam_srvd_cells_t));

    /* This function parses x2apOam_srvd_cells */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2apOam_srvd_cells->num_srvd_cell, p_src + *p_length_read, "num_srvd_cell");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2apOam_srvd_cells->num_srvd_cell > MAX_SERVED_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2apOam_srvd_cells->num_srvd_cell] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2apOam_srvd_cells->num_srvd_cell);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2apOam_srvd_cells->num_srvd_cell; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_x2apOam_srvd_cell_arry(
                &p_x2apOam_srvd_cells->srvd_cell_arr[loop],
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
oam_rrc_il_parse_x2ap_gu_group_id
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

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_plmn_identity(
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
oam_rrc_il_parse_x2ap_gu_grp_ids
(
    x2ap_gu_grp_ids_t *p_x2ap_gu_grp_ids,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_gu_grp_ids, 0, sizeof(x2ap_gu_grp_ids_t));

    /* This function parses x2ap_gu_grp_ids */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_gu_grp_ids->num_gu_group_id, p_src + *p_length_read, "num_gu_group_id");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_gu_grp_ids->num_gu_group_id > MAX_ENB_POOLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_gu_grp_ids->num_gu_group_id] should be less than"
            " or equal to MAX_ENB_POOLS. Incorrect value %u received.", p_x2ap_gu_grp_ids->num_gu_group_id);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_gu_grp_ids->num_gu_group_id; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_x2ap_gu_group_id(
                &p_x2ap_gu_grp_ids->group_id_arr[loop],
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
oam_rrc_il_parse_x2ap_info
(
    x2ap_info_t *p_x2ap_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_info, 0, sizeof(x2ap_info_t));

    /* This function parses x2ap_info */

    if (OAM_FAILURE == oam_rrc_il_parse_x2_enb_comm_info(
        &p_x2ap_info->local_enb_comm_info,
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
    OAM_CP_UNPACK_UINT8(&p_x2ap_info->num_peer_enb, p_src + *p_length_read, "num_peer_enb");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_info->num_peer_enb > MAX_PEER_ENB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_info->num_peer_enb] should be less than"
            " or equal to MAX_PEER_ENB. Incorrect value %u received.", p_x2ap_info->num_peer_enb);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_info->num_peer_enb; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_x2_enb_comm_info(
                &p_x2ap_info->peer_enb_comm_info[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_conf_param(
        &p_x2ap_info->conf_param,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_info->global_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_rrc_il_parse_x2apOam_srvd_cells(
        &p_x2ap_info->srvd_cell,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_gu_grp_ids(
        &p_x2ap_info->x2ap_gu_grp_id,
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
    OAM_CP_UNPACK_UINT8(&p_x2ap_info->log_level, p_src + *p_length_read, "log_level");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_info->log_level > 6))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_info->log_level] should be less than"
            " or equal to 6. Incorrect value %u received.", p_x2ap_info->log_level);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_info->num_peer_gnb, p_src + *p_length_read, "num_peer_gnb");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_info->num_peer_gnb > MAX_PEER_GNB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_info->num_peer_gnb] should be less than"
            " or equal to MAX_PEER_GNB. Incorrect value %u received.", p_x2ap_info->num_peer_gnb);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_info->num_peer_gnb; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_x2_enb_comm_info(
                &p_x2ap_info->peer_gnb_comm_info[loop],
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
oam_rrc_il_parse_son_comm_info
(
    son_comm_info_t *p_son_comm_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_son_comm_info, 0, sizeof(son_comm_info_t));

    /* This function parses son_comm_info */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_son_comm_info->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(p_son_comm_info->ip_addr) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_comm_info->ip_addr); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_son_comm_info->ip_addr[loop], (void*)(p_src + *p_length_read), "ip_addr[]");
            *p_length_read += sizeof(UInt8);
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_son_comm_info->port, p_src + *p_length_read, "port");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

oam_return_et
oam_rrc_il_parse_x2ap_oam_provision_resp
(
    x2ap_oam_provision_resp_t *p_x2ap_oam_provision_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_x2ap_oam_provision_resp, 0, sizeof(x2ap_oam_provision_resp_t));

    /* This function parses x2ap_oam_provision_resp */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_provision_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_provision_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_provision_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_oam_provision_resp->response);
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
oam_rrc_il_parse_x2ap_oam_get_log_level_resp
(
    x2ap_oam_get_log_level_resp_t *p_x2ap_oam_get_log_level_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_x2ap_oam_get_log_level_resp, 0, sizeof(x2ap_oam_get_log_level_resp_t));

    /* This function parses x2ap_oam_get_log_level_resp */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_oam_get_log_level_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_get_log_level_resp->log_on_off, p_src + *p_length_read, "log_on_off");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_get_log_level_resp->log_on_off > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_get_log_level_resp->log_on_off] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_oam_get_log_level_resp->log_on_off);
        return OAM_FAILURE;
    }

    if (p_x2ap_oam_get_log_level_resp->bitmask & X2AP_OAM_LOG_LEVEL_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_get_log_level_resp->log_level, p_src + *p_length_read, "log_level");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_get_log_level_resp->log_level > 6))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_get_log_level_resp->log_level] should be less than"
            " or equal to 6. Incorrect value %u received.", p_x2ap_oam_get_log_level_resp->log_level);
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
oam_rrc_il_parse_x2ap_oam_set_log_level_req
(
    x2ap_oam_set_log_level_req_t *p_x2ap_oam_set_log_level_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_x2ap_oam_set_log_level_req, 0, sizeof(x2ap_oam_set_log_level_req_t));

    /* This function parses x2ap_oam_set_log_level_req */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_set_log_level_req->log_level, p_src + *p_length_read, "log_level");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_set_log_level_req->log_level > 6))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_set_log_level_req->log_level] should be less than"
            " or equal to 6. Incorrect value %u received.", p_x2ap_oam_set_log_level_req->log_level);
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
oam_rrc_il_parse_x2ap_oam_set_log_level_resp
(
    x2ap_oam_set_log_level_resp_t *p_x2ap_oam_set_log_level_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_x2ap_oam_set_log_level_resp, 0, sizeof(x2ap_oam_set_log_level_resp_t));

    /* This function parses x2ap_oam_set_log_level_resp */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_set_log_level_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_set_log_level_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_set_log_level_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_oam_set_log_level_resp->response);
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
oam_rrc_il_parse_x2ap_oam_log_enable_req
(
    x2ap_oam_log_enable_req_t *p_x2ap_oam_log_enable_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_x2ap_oam_log_enable_req, 0, sizeof(x2ap_oam_log_enable_req_t));

    /* This function parses x2ap_oam_log_enable_req */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_log_enable_req->log_on_off, p_src + *p_length_read, "log_on_off");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_log_enable_req->log_on_off > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_log_enable_req->log_on_off] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_oam_log_enable_req->log_on_off);
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
oam_rrc_il_parse_x2ap_oam_log_enable_resp
(
    x2ap_oam_log_enable_resp_t *p_x2ap_oam_log_enable_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_x2ap_oam_log_enable_resp, 0, sizeof(x2ap_oam_log_enable_resp_t));

    /* This function parses x2ap_oam_log_enable_resp */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_log_enable_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_log_enable_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_log_enable_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_oam_log_enable_resp->response);
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
oam_rrc_il_parse_x2ap_oam_x2_link_down
(
    x2ap_oam_x2_link_down_t *p_x2ap_oam_x2_link_down,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_oam_x2_link_down, 0, sizeof(x2ap_oam_x2_link_down_t));

    /* This function parses x2ap_oam_x2_link_down */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_x2_link_down->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt8);

    if (p_x2ap_oam_x2_link_down->bitmask & X2AP_OAM_X2_LINK_DOWN_ENB_GB_ID_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_oam_x2_link_down->peer_enb_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_oam_x2_link_down->bitmask & X2AP_OAM_X2_LINK_DOWN_IPV4_ADDRESS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(p_x2ap_oam_x2_link_down->ip_addr) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_oam_x2_link_down->ip_addr); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2ap_oam_x2_link_down->ip_addr[loop], (void*)(p_src + *p_length_read), "ip_addr[]");
            *p_length_read += sizeof(UInt8);
        }
    }
    }

    if (p_x2ap_oam_x2_link_down->bitmask & X2AP_OAM_X2_LINK_DOWN_IPV6_ADDRESS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(p_x2ap_oam_x2_link_down->ipv6_addr) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_oam_x2_link_down->ipv6_addr); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2ap_oam_x2_link_down->ipv6_addr[loop], (void*)(p_src + *p_length_read), "ipv6_addr[]");
            *p_length_read += sizeof(UInt8);
        }
    }
    }

    if (p_x2ap_oam_x2_link_down->bitmask & X2AP_OAM_X2_LINK_DOWN_PORT_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_oam_x2_link_down->port, p_src + *p_length_read, "port");
    *p_length_read += sizeof(UInt16);
    }

    if (p_x2ap_oam_x2_link_down->bitmask & X2AP_OAM_X2_LINK_DOWN_GNB_GB_ID_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_x2_gb_gnb_id(
        &p_x2ap_oam_x2_link_down->peer_gnb_gb_id,
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
oam_rrc_il_parse_x2_gnb_id
(
    x2_gnb_id_t *p_x2_gnb_id,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_x2_gnb_id, 0, sizeof(x2_gnb_id_t));

    /* This function parses x2_gnb_id */

    if (*p_length_read + (SInt32)sizeof(p_x2_gnb_id->x2_gnb_id) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2_gnb_id->x2_gnb_id); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2_gnb_id->x2_gnb_id[loop], (void*)(p_src + *p_length_read), "x2_gnb_id[]");
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
oam_rrc_il_parse_x2_gb_gnb_id
(
    x2_gb_gnb_id_t *p_x2_gb_gnb_id,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2_gb_gnb_id, 0, sizeof(x2_gb_gnb_id_t));

    /* This function parses x2_gb_gnb_id */

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_plmn_identity(
        &p_x2_gb_gnb_id->plmn_identity,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (OAM_FAILURE == oam_rrc_il_parse_x2_gnb_id(
        &p_x2_gb_gnb_id->x2_en_gnb_id,
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
oam_rrc_il_parse_x2ap_oam_x2_link_up
(
    x2ap_oam_x2_link_up_t *p_x2ap_oam_x2_link_up,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_oam_x2_link_up, 0, sizeof(x2ap_oam_x2_link_up_t));

    /* This function parses x2ap_oam_x2_link_up */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_x2_link_up->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt8);

    if (p_x2ap_oam_x2_link_up->bitmask & X2AP_OAM_X2_LINK_UP_ENB_ID_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_oam_x2_link_up->peer_enb_gb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (OAM_FAILURE == oam_rrc_il_parse_x2_enb_comm_info(
        &p_x2ap_oam_x2_link_up->enb_comm_info,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (p_x2ap_oam_x2_link_up->bitmask & X2AP_OAM_X2_LINK_UP_IPV4_ADDRESS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(p_x2ap_oam_x2_link_up->ip_addr) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_oam_x2_link_up->ip_addr); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2ap_oam_x2_link_up->ip_addr[loop], (void*)(p_src + *p_length_read), "ip_addr[]");
            *p_length_read += sizeof(UInt8);
        }
    }
    }

    if (p_x2ap_oam_x2_link_up->bitmask & X2AP_OAM_X2_LINK_UP_IPV6_ADDRESS_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(p_x2ap_oam_x2_link_up->ipv6_addr) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_oam_x2_link_up->ipv6_addr); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2ap_oam_x2_link_up->ipv6_addr[loop], (void*)(p_src + *p_length_read), "ipv6_addr[]");
            *p_length_read += sizeof(UInt8);
        }
    }
    }

    if (p_x2ap_oam_x2_link_up->bitmask & X2AP_OAM_X2_LINK_UP_PORT_PRESENT)
    {

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_oam_x2_link_up->port, p_src + *p_length_read, "port");
    *p_length_read += sizeof(UInt16);
    }

    if (p_x2ap_oam_x2_link_up->bitmask & X2AP_OAM_X2_LINK_UP_GNB_ID_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_x2_gb_gnb_id(
        &p_x2ap_oam_x2_link_up->peer_gnb_gb_id,
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
oam_rrc_il_parse_x2ap_oam_x2_link_setup_req
(
    x2ap_oam_x2_link_setup_req_t *p_x2ap_oam_x2_link_setup_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_oam_x2_link_setup_req, 0, sizeof(x2ap_oam_x2_link_setup_req_t));

    /* This function parses x2ap_oam_x2_link_setup_req */

    if (OAM_FAILURE == oam_rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_oam_x2_link_setup_req->peer_enb_gb_id,
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
oam_rrc_il_parse_x2ap_oam_x2_link_setup_resp
(
    x2ap_oam_x2_link_setup_resp_t *p_x2ap_oam_x2_link_setup_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_x2ap_oam_x2_link_setup_resp, 0, sizeof(x2ap_oam_x2_link_setup_resp_t));

    /* This function parses x2ap_oam_x2_link_setup_resp */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_x2_link_setup_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_x2_link_setup_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_x2_link_setup_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_oam_x2_link_setup_resp->response);
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
oam_rrc_il_parse_x2ap_oam_cleanup_resp
(
    x2ap_oam_cleanup_resp_t *p_x2ap_oam_cleanup_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_x2ap_oam_cleanup_resp, 0, sizeof(x2ap_oam_cleanup_resp_t));

    /* This function parses x2ap_oam_cleanup_resp */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_cleanup_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_cleanup_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_cleanup_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_oam_cleanup_resp->response);
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
oam_rrc_il_parse_x2ap_oam_get_sctp_status_req
(
    x2ap_oam_get_sctp_status_req_t *p_x2ap_oam_get_sctp_status_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_oam_get_sctp_status_req, 0, sizeof(x2ap_oam_get_sctp_status_req_t));

    /* This function parses x2ap_oam_get_sctp_status_req */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_oam_get_sctp_status_req->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (p_x2ap_oam_get_sctp_status_req->bitmask & X2AP_OAM_GLOBAL_ENB_STATUS_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_oam_gb_enb_id_list_info(
        &p_x2ap_oam_get_sctp_status_req->x2ap_oam_gb_enb_status_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_oam_get_sctp_status_req->bitmask & X2AP_OAM_GLOBAL_GNB_STATUS_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_oam_gb_gnb_id_list_info(
        &p_x2ap_oam_get_sctp_status_req->x2ap_oam_gb_gnb_status_list,
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
oam_rrc_il_parse_x2ap_oam_gb_enb_id_list_info
(
    x2ap_oam_gb_enb_id_list_info_t *p_x2ap_oam_gb_enb_id_list_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_oam_gb_enb_id_list_info, 0, sizeof(x2ap_oam_gb_enb_id_list_info_t));

    /* This function parses x2ap_oam_gb_enb_id_list_info */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_gb_enb_id_list_info->num_peer_enb, p_src + *p_length_read, "num_peer_enb");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_enb_id_list_info->num_peer_enb > MAX_PEER_ENB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_enb_id_list_info->num_peer_enb] should be less than"
            " or equal to MAX_PEER_ENB. Incorrect value %u received.", p_x2ap_oam_gb_enb_id_list_info->num_peer_enb);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_oam_gb_enb_id_list_info->num_peer_enb; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_x2_gb_enb_id(
                &p_x2ap_oam_gb_enb_id_list_info->gb_enb_id[loop],
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
oam_rrc_il_parse_x2ap_oam_gb_gnb_id_list_info
(
    x2ap_oam_gb_gnb_id_list_info_t *p_x2ap_oam_gb_gnb_id_list_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_oam_gb_gnb_id_list_info, 0, sizeof(x2ap_oam_gb_gnb_id_list_info_t));

    /* This function parses x2ap_oam_gb_gnb_id_list_info */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_gb_gnb_id_list_info->num_peer_gnb, p_src + *p_length_read, "num_peer_gnb");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_gnb_id_list_info->num_peer_gnb > MAX_PEER_GNB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_gnb_id_list_info->num_peer_gnb] should be less than"
            " or equal to MAX_PEER_GNB. Incorrect value %u received.", p_x2ap_oam_gb_gnb_id_list_info->num_peer_gnb);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_oam_gb_gnb_id_list_info->num_peer_gnb; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_x2_gb_gnb_id(
                &p_x2ap_oam_gb_gnb_id_list_info->gb_gnb_id[loop],
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
oam_rrc_il_parse_x2ap_oam_get_sctp_status_resp
(
    x2ap_oam_get_sctp_status_resp_t *p_x2ap_oam_get_sctp_status_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_oam_get_sctp_status_resp, 0, sizeof(x2ap_oam_get_sctp_status_resp_t));

    /* This function parses x2ap_oam_get_sctp_status_resp */

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT16(&p_x2ap_oam_get_sctp_status_resp->bitmask, p_src + *p_length_read, "bitmask");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_get_sctp_status_resp->response, p_src + *p_length_read, "response");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_get_sctp_status_resp->response > 2))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_get_sctp_status_resp->response] should be less than"
            " or equal to 2. Incorrect value %u received.", p_x2ap_oam_get_sctp_status_resp->response);
        return OAM_FAILURE;
    }

    if (p_x2ap_oam_get_sctp_status_resp->bitmask & X2AP_OAM_GLOBAL_ENB_STATUS_PARAMS_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_oam_gb_enb_status_params_list_info(
        &p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_oam_get_sctp_status_resp->bitmask & X2AP_OAM_GLOBAL_ENB_FAILED_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_oam_gb_enb_failed_list(
        &p_x2ap_oam_get_sctp_status_resp->gb_enb_failed_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_oam_get_sctp_status_resp->bitmask & X2AP_OAM_GLOBAL_GNB_STATUS_PARAMS_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_oam_gb_gnb_status_params_list_info(
        &p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;
    }

    if (p_x2ap_oam_get_sctp_status_resp->bitmask & X2AP_OAM_GLOBAL_GNB_FAILED_LIST_PRESENT)
    {

    if (OAM_FAILURE == oam_rrc_il_parse_x2ap_oam_gb_gnb_failed_list(
        &p_x2ap_oam_get_sctp_status_resp->gb_gnb_failed_list,
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
oam_rrc_il_parse_rrc_timeval
(
    rrc_timeval_t *p_rrc_timeval,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    *p_length_read = 0;

    oam_memset(p_rrc_timeval, 0, sizeof(rrc_timeval_t));

    /* This function parses rrc_timeval */

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_timeval->tv_sec, p_src + *p_length_read, "tv_sec");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_rrc_timeval->tv_usec, p_src + *p_length_read, "tv_usec");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}

static
oam_return_et
oam_rrc_il_parse_x2ap_oam_gb_enb_id_status_params
(
    x2ap_oam_gb_enb_id_status_params_t *p_x2ap_oam_gb_enb_id_status_params,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_oam_gb_enb_id_status_params, 0, sizeof(x2ap_oam_gb_enb_id_status_params_t));

    /* This function parses x2ap_oam_gb_enb_id_status_params */

    if (OAM_FAILURE == oam_rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_oam_gb_enb_id_status_params->global_enb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_oam_gb_enb_id_status_params->status, p_src + *p_length_read, "status");
    *p_length_read += sizeof(UInt32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_enb_id_status_params->status > 9))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_enb_id_status_params->status] should be less than"
            " or equal to 9. Incorrect value %u received.", p_x2ap_oam_gb_enb_id_status_params->status);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_oam_gb_enb_id_status_params->in_streams, p_src + *p_length_read, "in_streams");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_oam_gb_enb_id_status_params->out_streams, p_src + *p_length_read, "out_streams");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_oam_gb_enb_id_status_params->local_port, p_src + *p_length_read, "local_port");
    *p_length_read += sizeof(UInt32);

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_timeval(
        &p_x2ap_oam_gb_enb_id_status_params->start_time,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(p_x2ap_oam_gb_enb_id_status_params->primary_peer_addr) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_oam_gb_enb_id_status_params->primary_peer_addr); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2ap_oam_gb_enb_id_status_params->primary_peer_addr[loop], (void*)(p_src + *p_length_read), "primary_peer_addr[]");
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
oam_rrc_il_parse_x2ap_oam_gb_enb_status_params_list_info
(
    x2ap_oam_gb_enb_status_params_list_info_t *p_x2ap_oam_gb_enb_status_params_list_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_oam_gb_enb_status_params_list_info, 0, sizeof(x2ap_oam_gb_enb_status_params_list_info_t));

    /* This function parses x2ap_oam_gb_enb_status_params_list_info */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_gb_enb_status_params_list_info->num_peer_enb, p_src + *p_length_read, "num_peer_enb");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_enb_status_params_list_info->num_peer_enb > MAX_PEER_ENB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_enb_status_params_list_info->num_peer_enb] should be less than"
            " or equal to MAX_PEER_ENB. Incorrect value %u received.", p_x2ap_oam_gb_enb_status_params_list_info->num_peer_enb);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_oam_gb_enb_status_params_list_info->num_peer_enb; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_x2ap_oam_gb_enb_id_status_params(
                &p_x2ap_oam_gb_enb_status_params_list_info->staus_params[loop],
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
oam_rrc_il_parse_x2ap_oam_failed_gb_enb_info
(
    x2ap_oam_failed_gb_enb_info_t *p_x2ap_oam_failed_gb_enb_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_oam_failed_gb_enb_info, 0, sizeof(x2ap_oam_failed_gb_enb_info_t));

    /* This function parses x2ap_oam_failed_gb_enb_info */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_failed_gb_enb_info->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(UInt8);

    if (OAM_FAILURE == oam_rrc_il_parse_x2_gb_enb_id(
        &p_x2ap_oam_failed_gb_enb_info->global_enb_id,
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
oam_rrc_il_parse_x2ap_oam_gb_enb_failed_list
(
    x2ap_oam_gb_enb_failed_list_t *p_x2ap_oam_gb_enb_failed_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_oam_gb_enb_failed_list, 0, sizeof(x2ap_oam_gb_enb_failed_list_t));

    /* This function parses x2ap_oam_gb_enb_failed_list */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_gb_enb_failed_list->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_enb_failed_list->count > MAX_PEER_ENB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_enb_failed_list->count] should be less than"
            " or equal to MAX_PEER_ENB. Incorrect value %u received.", p_x2ap_oam_gb_enb_failed_list->count);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_oam_gb_enb_failed_list->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_x2ap_oam_failed_gb_enb_info(
                &p_x2ap_oam_gb_enb_failed_list->failed_global_enb_info[loop],
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
oam_rrc_il_parse_x2ap_oam_gb_gnb_id_status_params
(
    x2ap_oam_gb_gnb_id_status_params_t *p_x2ap_oam_gb_gnb_id_status_params,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_oam_gb_gnb_id_status_params, 0, sizeof(x2ap_oam_gb_gnb_id_status_params_t));

    /* This function parses x2ap_oam_gb_gnb_id_status_params */

    if (OAM_FAILURE == oam_rrc_il_parse_x2_gb_gnb_id(
        &p_x2ap_oam_gb_gnb_id_status_params->global_gnb_id,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_oam_gb_gnb_id_status_params->status, p_src + *p_length_read, "status");
    *p_length_read += sizeof(UInt32);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_gnb_id_status_params->status > 9))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_gnb_id_status_params->status] should be less than"
            " or equal to 9. Incorrect value %u received.", p_x2ap_oam_gb_gnb_id_status_params->status);
        return OAM_FAILURE;
    }

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_oam_gb_gnb_id_status_params->in_streams, p_src + *p_length_read, "in_streams");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_oam_gb_gnb_id_status_params->out_streams, p_src + *p_length_read, "out_streams");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT32(&p_x2ap_oam_gb_gnb_id_status_params->local_port, p_src + *p_length_read, "local_port");
    *p_length_read += sizeof(UInt32);

    if (OAM_FAILURE == oam_rrc_il_parse_rrc_timeval(
        &p_x2ap_oam_gb_gnb_id_status_params->start_time,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(p_x2ap_oam_gb_gnb_id_status_params->primary_peer_addr) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_oam_gb_gnb_id_status_params->primary_peer_addr); loop++)
        {
            OAM_CP_UNPACK_UINT8((void*)&p_x2ap_oam_gb_gnb_id_status_params->primary_peer_addr[loop], (void*)(p_src + *p_length_read), "primary_peer_addr[]");
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
oam_rrc_il_parse_x2ap_oam_gb_gnb_status_params_list_info
(
    x2ap_oam_gb_gnb_status_params_list_info_t *p_x2ap_oam_gb_gnb_status_params_list_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_oam_gb_gnb_status_params_list_info, 0, sizeof(x2ap_oam_gb_gnb_status_params_list_info_t));

    /* This function parses x2ap_oam_gb_gnb_status_params_list_info */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_gb_gnb_status_params_list_info->num_peer_gnb, p_src + *p_length_read, "num_peer_gnb");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_gnb_status_params_list_info->num_peer_gnb > MAX_PEER_GNB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_gnb_status_params_list_info->num_peer_gnb] should be less than"
            " or equal to MAX_PEER_GNB. Incorrect value %u received.", p_x2ap_oam_gb_gnb_status_params_list_info->num_peer_gnb);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_oam_gb_gnb_status_params_list_info->num_peer_gnb; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_x2ap_oam_gb_gnb_id_status_params(
                &p_x2ap_oam_gb_gnb_status_params_list_info->staus_params[loop],
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
oam_rrc_il_parse_x2ap_oam_failed_gb_gnb_info
(
    x2ap_oam_failed_gb_gnb_info_t *p_x2ap_oam_failed_gb_gnb_info,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_oam_failed_gb_gnb_info, 0, sizeof(x2ap_oam_failed_gb_gnb_info_t));

    /* This function parses x2ap_oam_failed_gb_gnb_info */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_failed_gb_gnb_info->cause, p_src + *p_length_read, "cause");
    *p_length_read += sizeof(UInt8);

    if (OAM_FAILURE == oam_rrc_il_parse_x2_gb_gnb_id(
        &p_x2ap_oam_failed_gb_gnb_info->global_gnb_id,
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
oam_rrc_il_parse_x2ap_oam_gb_gnb_failed_list
(
    x2ap_oam_gb_gnb_failed_list_t *p_x2ap_oam_gb_gnb_failed_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_memset(p_x2ap_oam_gb_gnb_failed_list, 0, sizeof(x2ap_oam_gb_gnb_failed_list_t));

    /* This function parses x2ap_oam_gb_gnb_failed_list */

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    OAM_CP_UNPACK_UINT8(&p_x2ap_oam_gb_gnb_failed_list->count, p_src + *p_length_read, "count");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_gnb_failed_list->count > MAX_PEER_GNB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_gnb_failed_list->count] should be less than"
            " or equal to MAX_PEER_GNB. Incorrect value %u received.", p_x2ap_oam_gb_gnb_failed_list->count);
        return OAM_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_oam_gb_gnb_failed_list->count; loop++)
        {
            if (OAM_FAILURE == oam_rrc_il_parse_x2ap_oam_failed_gb_gnb_info(
                &p_x2ap_oam_gb_gnb_failed_list->failed_global_gnb_info[loop],
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

