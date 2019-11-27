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
 *  File Description : The file oam_x2ap_il_composer.c contains the definitions 
 *                     of rrc-x2apOam interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/

#include "oam_x2ap_il_composer.h"
#include "rrc_x2apOam_intf.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
static oam_error_code_et oam_error = NO_ERROR;


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_length_t
rrc_il_get_x2_ip_addr_len
(
    x2_ip_addr_t *p_x2_ip_addr
);

static
oam_return_et
rrc_il_compose_x2_ip_addr
(
    UInt8  **pp_buffer,
    x2_ip_addr_t *p_x2_ip_addr
);

static
rrc_length_t
rrc_il_get_x2_ipv6_addr_len
(
    x2_ipv6_addr_t *p_x2_ipv6_addr
);

static
oam_return_et
rrc_il_compose_x2_ipv6_addr
(
    UInt8  **pp_buffer,
    x2_ipv6_addr_t *p_x2_ipv6_addr
);

static
rrc_length_t
rrc_il_get_x2_trans_layer_addrs_len
(
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs
);

static
oam_return_et
rrc_il_compose_x2_trans_layer_addrs
(
    UInt8  **pp_buffer,
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs
);

static
rrc_length_t
rrc_il_get_x2_extend_trans_layer_addrs_len
(
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs
);

static
oam_return_et
rrc_il_compose_x2_extend_trans_layer_addrs
(
    UInt8  **pp_buffer,
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs
);

static
rrc_length_t
rrc_il_get_x2_enb_comm_info_len
(
    x2_enb_comm_info_t *p_x2_enb_comm_info
);

static
oam_return_et
rrc_il_compose_x2_enb_comm_info
(
    UInt8  **pp_buffer,
    x2_enb_comm_info_t *p_x2_enb_comm_info
);

static
rrc_length_t
rrc_il_get_x2ap_sctp_conf_info_len
(
    x2ap_sctp_conf_info_t *p_x2ap_sctp_conf_info
);

static
oam_return_et
rrc_il_compose_x2ap_sctp_conf_info
(
    UInt8  **pp_buffer,
    x2ap_sctp_conf_info_t *p_x2ap_sctp_conf_info
);

static
rrc_length_t
rrc_il_get_x2ap_con_tim_info_len
(
    x2ap_con_tim_info_t *p_x2ap_con_tim_info
);

static
oam_return_et
rrc_il_compose_x2ap_con_tim_info
(
    UInt8  **pp_buffer,
    x2ap_con_tim_info_t *p_x2ap_con_tim_info
);

static
rrc_length_t
rrc_il_get_x2ap_conf_param_len
(
    x2ap_conf_param_t *p_x2ap_conf_param
);

static
oam_return_et
rrc_il_compose_x2ap_conf_param
(
    UInt8  **pp_buffer,
    x2ap_conf_param_t *p_x2ap_conf_param
);

static
rrc_length_t
rrc_il_get_x2ap_plmn_identity_len
(
    x2ap_plmn_identity_t *p_x2ap_plmn_identity
);

static
oam_return_et
rrc_il_compose_x2ap_plmn_identity
(
    UInt8  **pp_buffer,
    x2ap_plmn_identity_t *p_x2ap_plmn_identity
);

static
rrc_length_t
rrc_il_get_macro_enb_id_len
(
    macro_enb_id_t *p_macro_enb_id
);

static
oam_return_et
rrc_il_compose_macro_enb_id
(
    UInt8  **pp_buffer,
    macro_enb_id_t *p_macro_enb_id
);

static
rrc_length_t
rrc_il_get_home_enb_id_len
(
    home_enb_id_t *p_home_enb_id
);

static
oam_return_et
rrc_il_compose_home_enb_id
(
    UInt8  **pp_buffer,
    home_enb_id_t *p_home_enb_id
);

static
rrc_length_t
rrc_il_get_x2_enb_id_info_len
(
    x2_enb_id_info_t *p_x2_enb_id_info
);

static
oam_return_et
rrc_il_compose_x2_enb_id_info
(
    UInt8  **pp_buffer,
    x2_enb_id_info_t *p_x2_enb_id_info
);

static
rrc_length_t
rrc_il_get_x2_gb_enb_id_len
(
    x2_gb_enb_id_t *p_x2_gb_enb_id
);

static
oam_return_et
rrc_il_compose_x2_gb_enb_id
(
    UInt8  **pp_buffer,
    x2_gb_enb_id_t *p_x2_gb_enb_id
);

static
rrc_length_t
rrc_il_get_x2ap_tac_len
(
    x2ap_tac_t *p_x2ap_tac
);

static
oam_return_et
rrc_il_compose_x2ap_tac
(
    UInt8  **pp_buffer,
    x2ap_tac_t *p_x2ap_tac
);

static
rrc_length_t
rrc_il_get_x2ap_broadcast_plmn_len
(
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn
);

static
oam_return_et
rrc_il_compose_x2ap_broadcast_plmn
(
    UInt8  **pp_buffer,
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn
);

static
rrc_length_t
rrc_il_get_x2ap_earfcn_len
(
    x2ap_earfcn_t *p_x2ap_earfcn
);

static
oam_return_et
rrc_il_compose_x2ap_earfcn
(
    UInt8  **pp_buffer,
    x2ap_earfcn_t *p_x2ap_earfcn
);

static
rrc_length_t
rrc_il_get_x2ap_fdd_info_len
(
    x2ap_fdd_info_t *p_x2ap_fdd_info
);

static
oam_return_et
rrc_il_compose_x2ap_fdd_info
(
    UInt8  **pp_buffer,
    x2ap_fdd_info_t *p_x2ap_fdd_info
);

static
rrc_length_t
rrc_il_get_x2ap_special_subframe_info_len
(
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info
);

static
oam_return_et
rrc_il_compose_x2ap_special_subframe_info
(
    UInt8  **pp_buffer,
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info
);

static
rrc_length_t
rrc_il_get_x2ap_tdd_info_len
(
    x2ap_tdd_info_t *p_x2ap_tdd_info
);

static
oam_return_et
rrc_il_compose_x2ap_tdd_info
(
    UInt8  **pp_buffer,
    x2ap_tdd_info_t *p_x2ap_tdd_info
);

static
rrc_length_t
rrc_il_get_x2ap_choice_eutra_mode_len
(
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode
);

static
oam_return_et
rrc_il_compose_x2ap_choice_eutra_mode
(
    UInt8  **pp_buffer,
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode
);

static
rrc_length_t
rrc_il_get_x2ap_choice_eutra_mode_info_len
(
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info
);

static
oam_return_et
rrc_il_compose_x2ap_choice_eutra_mode_info
(
    UInt8  **pp_buffer,
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info
);

static
rrc_length_t
rrc_il_get_x2ap_prach_configuration_info_len
(
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info
);

static
oam_return_et
rrc_il_compose_x2ap_prach_configuration_info
(
    UInt8  **pp_buffer,
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info
);

static
rrc_length_t
rrc_il_get_x2ap_subframe_allocation_len
(
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation
);

static
oam_return_et
rrc_il_compose_x2ap_subframe_allocation
(
    UInt8  **pp_buffer,
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation
);

static
rrc_length_t
rrc_il_get_x2ap_mbsfn_info_len
(
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info
);

static
oam_return_et
rrc_il_compose_x2ap_mbsfn_info
(
    UInt8  **pp_buffer,
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info
);

static
rrc_length_t
rrc_il_get_x2ap_mbsfn_subframe_info_len
(
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info
);

static
oam_return_et
rrc_il_compose_x2ap_mbsfn_subframe_info
(
    UInt8  **pp_buffer,
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info
);

static
rrc_length_t
rrc_il_get_x2apOam_srvd_cell_info_len
(
    x2apOam_srvd_cell_info_t *p_x2apOam_srvd_cell_info
);

static
oam_return_et
rrc_il_compose_x2apOam_srvd_cell_info
(
    UInt8  **pp_buffer,
    x2apOam_srvd_cell_info_t *p_x2apOam_srvd_cell_info
);

static
rrc_length_t
rrc_il_get_x2ap_ecgi_len
(
    x2ap_ecgi_t *p_x2ap_ecgi
);

static
oam_return_et
rrc_il_compose_x2ap_ecgi
(
    UInt8  **pp_buffer,
    x2ap_ecgi_t *p_x2ap_ecgi
);

static
rrc_length_t
rrc_il_get_x2ap_neighbour_info_len
(
    x2ap_neighbour_info_t *p_x2ap_neighbour_info
);

static
oam_return_et
rrc_il_compose_x2ap_neighbour_info
(
    UInt8  **pp_buffer,
    x2ap_neighbour_info_t *p_x2ap_neighbour_info
);

static
rrc_length_t
rrc_il_get_x2ap_nbour_cell_arry_len
(
    x2ap_nbour_cell_arry_t *p_x2ap_nbour_cell_arry
);

static
oam_return_et
rrc_il_compose_x2ap_nbour_cell_arry
(
    UInt8  **pp_buffer,
    x2ap_nbour_cell_arry_t *p_x2ap_nbour_cell_arry
);

static
rrc_length_t
rrc_il_get_x2apOam_srvd_cell_arry_len
(
    x2apOam_srvd_cell_arry_t *p_x2apOam_srvd_cell_arry
);

static
oam_return_et
rrc_il_compose_x2apOam_srvd_cell_arry
(
    UInt8  **pp_buffer,
    x2apOam_srvd_cell_arry_t *p_x2apOam_srvd_cell_arry
);

static
rrc_length_t
rrc_il_get_x2apOam_srvd_cells_len
(
    x2apOam_srvd_cells_t *p_x2apOam_srvd_cells
);

static
oam_return_et
rrc_il_compose_x2apOam_srvd_cells
(
    UInt8  **pp_buffer,
    x2apOam_srvd_cells_t *p_x2apOam_srvd_cells
);

static
rrc_length_t
rrc_il_get_x2ap_gu_group_id_len
(
    x2ap_gu_group_id_t *p_x2ap_gu_group_id
);

static
oam_return_et
rrc_il_compose_x2ap_gu_group_id
(
    UInt8  **pp_buffer,
    x2ap_gu_group_id_t *p_x2ap_gu_group_id
);

static
rrc_length_t
rrc_il_get_x2ap_gu_grp_ids_len
(
    x2ap_gu_grp_ids_t *p_x2ap_gu_grp_ids
);

static
oam_return_et
rrc_il_compose_x2ap_gu_grp_ids
(
    UInt8  **pp_buffer,
    x2ap_gu_grp_ids_t *p_x2ap_gu_grp_ids
);

static
rrc_length_t
rrc_il_get_x2ap_info_len
(
    x2ap_info_t *p_x2ap_info
);

static
oam_return_et
rrc_il_compose_x2ap_info
(
    UInt8  **pp_buffer,
    x2ap_info_t *p_x2ap_info
);

static
rrc_length_t
rrc_il_get_son_comm_info_len
(
    son_comm_info_t *p_son_comm_info
);

static
oam_return_et
rrc_il_compose_son_comm_info
(
    UInt8  **pp_buffer,
    son_comm_info_t *p_son_comm_info
);

static
rrc_length_t
rrc_il_get_x2_gnb_id_len
(
    x2_gnb_id_t *p_x2_gnb_id
);

static
oam_return_et
rrc_il_compose_x2_gnb_id
(
    UInt8  **pp_buffer,
    x2_gnb_id_t *p_x2_gnb_id
);

static
rrc_length_t
rrc_il_get_x2_gb_gnb_id_len
(
    x2_gb_gnb_id_t *p_x2_gb_gnb_id
);

static
oam_return_et
rrc_il_compose_x2_gb_gnb_id
(
    UInt8  **pp_buffer,
    x2_gb_gnb_id_t *p_x2_gb_gnb_id
);

static
rrc_length_t
rrc_il_get_x2ap_oam_gb_enb_id_list_info_len
(
    x2ap_oam_gb_enb_id_list_info_t *p_x2ap_oam_gb_enb_id_list_info
);

static
oam_return_et
rrc_il_compose_x2ap_oam_gb_enb_id_list_info
(
    UInt8  **pp_buffer,
    x2ap_oam_gb_enb_id_list_info_t *p_x2ap_oam_gb_enb_id_list_info
);

static
rrc_length_t
rrc_il_get_x2ap_oam_gb_gnb_id_list_info_len
(
    x2ap_oam_gb_gnb_id_list_info_t *p_x2ap_oam_gb_gnb_id_list_info
);

static
oam_return_et
rrc_il_compose_x2ap_oam_gb_gnb_id_list_info
(
    UInt8  **pp_buffer,
    x2ap_oam_gb_gnb_id_list_info_t *p_x2ap_oam_gb_gnb_id_list_info
);

static
rrc_length_t
rrc_il_get_rrc_timeval_len
(
    rrc_timeval_t *p_rrc_timeval
);

static
oam_return_et
rrc_il_compose_rrc_timeval
(
    UInt8  **pp_buffer,
    rrc_timeval_t *p_rrc_timeval
);

static
rrc_length_t
rrc_il_get_x2ap_oam_gb_enb_id_status_params_len
(
    x2ap_oam_gb_enb_id_status_params_t *p_x2ap_oam_gb_enb_id_status_params
);

static
oam_return_et
rrc_il_compose_x2ap_oam_gb_enb_id_status_params
(
    UInt8  **pp_buffer,
    x2ap_oam_gb_enb_id_status_params_t *p_x2ap_oam_gb_enb_id_status_params
);

static
rrc_length_t
rrc_il_get_x2ap_oam_gb_enb_status_params_list_info_len
(
    x2ap_oam_gb_enb_status_params_list_info_t *p_x2ap_oam_gb_enb_status_params_list_info
);

static
oam_return_et
rrc_il_compose_x2ap_oam_gb_enb_status_params_list_info
(
    UInt8  **pp_buffer,
    x2ap_oam_gb_enb_status_params_list_info_t *p_x2ap_oam_gb_enb_status_params_list_info
);

static
rrc_length_t
rrc_il_get_x2ap_oam_failed_gb_enb_info_len
(
    x2ap_oam_failed_gb_enb_info_t *p_x2ap_oam_failed_gb_enb_info
);

static
oam_return_et
rrc_il_compose_x2ap_oam_failed_gb_enb_info
(
    UInt8  **pp_buffer,
    x2ap_oam_failed_gb_enb_info_t *p_x2ap_oam_failed_gb_enb_info
);

static
rrc_length_t
rrc_il_get_x2ap_oam_gb_enb_failed_list_len
(
    x2ap_oam_gb_enb_failed_list_t *p_x2ap_oam_gb_enb_failed_list
);

static
oam_return_et
rrc_il_compose_x2ap_oam_gb_enb_failed_list
(
    UInt8  **pp_buffer,
    x2ap_oam_gb_enb_failed_list_t *p_x2ap_oam_gb_enb_failed_list
);

static
rrc_length_t
rrc_il_get_x2ap_oam_gb_gnb_id_status_params_len
(
    x2ap_oam_gb_gnb_id_status_params_t *p_x2ap_oam_gb_gnb_id_status_params
);

static
oam_return_et
rrc_il_compose_x2ap_oam_gb_gnb_id_status_params
(
    UInt8  **pp_buffer,
    x2ap_oam_gb_gnb_id_status_params_t *p_x2ap_oam_gb_gnb_id_status_params
);

static
rrc_length_t
rrc_il_get_x2ap_oam_gb_gnb_status_params_list_info_len
(
    x2ap_oam_gb_gnb_status_params_list_info_t *p_x2ap_oam_gb_gnb_status_params_list_info
);

static
oam_return_et
rrc_il_compose_x2ap_oam_gb_gnb_status_params_list_info
(
    UInt8  **pp_buffer,
    x2ap_oam_gb_gnb_status_params_list_info_t *p_x2ap_oam_gb_gnb_status_params_list_info
);

static
rrc_length_t
rrc_il_get_x2ap_oam_failed_gb_gnb_info_len
(
    x2ap_oam_failed_gb_gnb_info_t *p_x2ap_oam_failed_gb_gnb_info
);

static
oam_return_et
rrc_il_compose_x2ap_oam_failed_gb_gnb_info
(
    UInt8  **pp_buffer,
    x2ap_oam_failed_gb_gnb_info_t *p_x2ap_oam_failed_gb_gnb_info
);

static
rrc_length_t
rrc_il_get_x2ap_oam_gb_gnb_failed_list_len
(
    x2ap_oam_gb_gnb_failed_list_t *p_x2ap_oam_gb_gnb_failed_list
);

static
oam_return_et
rrc_il_compose_x2ap_oam_gb_gnb_failed_list
(
    UInt8  **pp_buffer,
    x2ap_oam_gb_gnb_failed_list_t *p_x2ap_oam_gb_gnb_failed_list
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_length_t
rrc_il_get_x2ap_oam_provision_req_len
(
    x2ap_oam_provision_req_t *p_x2ap_oam_provision_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_provision_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_provision_req->bitmask);

    /* Optional element */
    if(p_x2ap_oam_provision_req->bitmask & RRC_X2AP_PROVISION_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_info_len(&p_x2ap_oam_provision_req->x2ap_info);
    }

    /* Optional element */
    if(p_x2ap_oam_provision_req->bitmask & SON_MODULE_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_son_comm_info_len(&p_x2ap_oam_provision_req->son_comm_info);
    }

    return length;
}

oam_return_et
rrc_il_compose_x2ap_oam_provision_req
(
    UInt8  **pp_buffer,
    x2ap_oam_provision_req_t *p_x2ap_oam_provision_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_provision_req != PNULL);

    /* This function composes x2ap_oam_provision_req */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_oam_provision_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_oam_provision_req->bitmask);

    /* Optional element */
    if(p_x2ap_oam_provision_req->bitmask & RRC_X2AP_PROVISION_INFO_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_info(pp_buffer, &p_x2ap_oam_provision_req->x2ap_info))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_oam_provision_req->bitmask & SON_MODULE_INFO_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_son_comm_info(pp_buffer, &p_x2ap_oam_provision_req->son_comm_info))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_x2ap_il_send_x2ap_oam_provision_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_OAM_PROVISION_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_x2ap_il_send_x2ap_oam_provision_req
(
    x2ap_oam_provision_req_t  *p_x2ap_oam_provision_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_x2apOam_msg;

    OAM_NULL_CHECK(p_x2ap_oam_provision_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_oam_provision_req_len(p_x2ap_oam_provision_req);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):X2AP_OAM_PROVISION_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_x2apOam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_x2apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_OAM_PROVISION_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apOam_msg = p_x2apOam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_x2apOam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, X2AP_OAM_PROVISION_REQ, msg_length,cell_index);

    /* Fill X2AP_OAM_PROVISION_REQ message */
    p_x2apOam_msg = p_x2apOam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_provision_req(&p_x2apOam_msg, p_x2ap_oam_provision_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_x2apOam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2_ip_addr_len
(
    x2_ip_addr_t *p_x2_ip_addr
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2_ip_addr != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2_ip_addr->ip_addr);

    return length;
}

static
oam_return_et
rrc_il_compose_x2_ip_addr
(
    UInt8  **pp_buffer,
    x2_ip_addr_t *p_x2_ip_addr
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2_ip_addr != PNULL);

    /* This function composes x2_ip_addr */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2_ip_addr->ip_addr); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_x2_ip_addr->ip_addr[loop], "ip_addr[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2_ipv6_addr_len
(
    x2_ipv6_addr_t *p_x2_ipv6_addr
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2_ipv6_addr != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2_ipv6_addr->ipv6_addr);

    return length;
}

static
oam_return_et
rrc_il_compose_x2_ipv6_addr
(
    UInt8  **pp_buffer,
    x2_ipv6_addr_t *p_x2_ipv6_addr
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2_ipv6_addr != PNULL);

    /* This function composes x2_ipv6_addr */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2_ipv6_addr->ipv6_addr); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_x2_ipv6_addr->ipv6_addr[loop], "ipv6_addr[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2_trans_layer_addrs_len
(
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2_trans_layer_addrs != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2_trans_layer_addrs->data_length);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_x2_trans_layer_addrs->data_length * sizeof(p_x2_trans_layer_addrs->data[0]));

    return length;
}

static
oam_return_et
rrc_il_compose_x2_trans_layer_addrs
(
    UInt8  **pp_buffer,
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2_trans_layer_addrs != PNULL);

    /* This function composes x2_trans_layer_addrs */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2_trans_layer_addrs->data_length > RRC_MAX_IP_ADDR_LEN_IPV6))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_trans_layer_addrs->data_length] should be less than"
            " or equal to RRC_MAX_IP_ADDR_LEN_IPV6. Incorrect value %u received.", p_x2_trans_layer_addrs->data_length);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2_trans_layer_addrs->data_length, "data_length");
    *pp_buffer += sizeof(p_x2_trans_layer_addrs->data_length);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_trans_layer_addrs->data_length; loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_x2_trans_layer_addrs->data[loop], "data[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2_extend_trans_layer_addrs_len
(
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2_extend_trans_layer_addrs != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2_extend_trans_layer_addrs->bitmask);

    /* Optional element */
    if(p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_IPSEC_ADDRS_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2_trans_layer_addrs_len(&p_x2_extend_trans_layer_addrs->ipsec_trans_layer_addrs);
    }

    /* Optional element */
    if(p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_NUM_GTP_ADDRS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2_extend_trans_layer_addrs->num_tlas);
    }

    /* Optional element */
    if(p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_GTP_ADDRS_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_extend_trans_layer_addrs->num_tlas < 1) || (p_x2_extend_trans_layer_addrs->num_tlas > MAX_NUM_ENB_X2AP_GTP_TLAS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_extend_trans_layer_addrs->num_tlas] should be in range "
            "1 to MAX_NUM_ENB_X2AP_GTP_TLAS. Incorrect value %d received.", p_x2_extend_trans_layer_addrs->num_tlas);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_extend_trans_layer_addrs->num_tlas; loop++)
        {
            length += rrc_il_get_x2_trans_layer_addrs_len(&p_x2_extend_trans_layer_addrs->gtp_trans_layer_addrs[loop]);
        }
    }
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2_extend_trans_layer_addrs
(
    UInt8  **pp_buffer,
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2_extend_trans_layer_addrs != PNULL);

    /* This function composes x2_extend_trans_layer_addrs */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2_extend_trans_layer_addrs->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2_extend_trans_layer_addrs->bitmask);

    /* Optional element */
    if(p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_IPSEC_ADDRS_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2_trans_layer_addrs(pp_buffer, &p_x2_extend_trans_layer_addrs->ipsec_trans_layer_addrs))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_NUM_GTP_ADDRS_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_extend_trans_layer_addrs->num_tlas < 1) || (p_x2_extend_trans_layer_addrs->num_tlas > MAX_NUM_ENB_X2AP_GTP_TLAS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_extend_trans_layer_addrs->num_tlas] should be in range "
            "1 to MAX_NUM_ENB_X2AP_GTP_TLAS. Incorrect value %u received.", p_x2_extend_trans_layer_addrs->num_tlas);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2_extend_trans_layer_addrs->num_tlas, "num_tlas");
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
            if (OAM_FAILURE == rrc_il_compose_x2_trans_layer_addrs(pp_buffer, &p_x2_extend_trans_layer_addrs->gtp_trans_layer_addrs[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2_enb_comm_info_len
(
    x2_enb_comm_info_t *p_x2_enb_comm_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2_enb_comm_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2_enb_comm_info->bitmask);

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2_enb_comm_info->num_ip_addr);
    }

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_ip_addr < 1) || (p_x2_enb_comm_info->num_ip_addr > MAX_NUM_IP_ADDR))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_enb_comm_info->num_ip_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %d received.", p_x2_enb_comm_info->num_ip_addr);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_enb_comm_info->num_ip_addr; loop++)
        {
            length += rrc_il_get_x2_ip_addr_len(&p_x2_enb_comm_info->ip_addr[loop]);
        }
    }
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2_enb_comm_info->port);

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2_enb_comm_info->num_ipv6_addr);
    }

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_ipv6_addr < 1) || (p_x2_enb_comm_info->num_ipv6_addr > MAX_NUM_IP_ADDR))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_enb_comm_info->num_ipv6_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %d received.", p_x2_enb_comm_info->num_ipv6_addr);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_enb_comm_info->num_ipv6_addr; loop++)
        {
            length += rrc_il_get_x2_ipv6_addr_len(&p_x2_enb_comm_info->ipv6_addr[loop]);
        }
    }
    }

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_EXT_NUM_TLAS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2_enb_comm_info->num_tlas);
    }

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_EXT_TLAS_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_tlas < 1) || (p_x2_enb_comm_info->num_tlas > MAX_NUM_ENB_X2AP_EXT_TLAS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_enb_comm_info->num_tlas] should be in range "
            "1 to MAX_NUM_ENB_X2AP_EXT_TLAS. Incorrect value %d received.", p_x2_enb_comm_info->num_tlas);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2_enb_comm_info->num_tlas; loop++)
        {
            length += rrc_il_get_x2_extend_trans_layer_addrs_len(&p_x2_enb_comm_info->extend_trans_layer_addrs[loop]);
        }
    }
    }

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_DEFAULT_SGNB_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2_enb_comm_info->default_sgnb);
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2_enb_comm_info
(
    UInt8  **pp_buffer,
    x2_enb_comm_info_t *p_x2_enb_comm_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2_enb_comm_info != PNULL);

    /* This function composes x2_enb_comm_info */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2_enb_comm_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2_enb_comm_info->bitmask);

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_ip_addr < 1) || (p_x2_enb_comm_info->num_ip_addr > MAX_NUM_IP_ADDR))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_enb_comm_info->num_ip_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %u received.", p_x2_enb_comm_info->num_ip_addr);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2_enb_comm_info->num_ip_addr, "num_ip_addr");
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
            if (OAM_FAILURE == rrc_il_compose_x2_ip_addr(pp_buffer, &p_x2_enb_comm_info->ip_addr[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2_enb_comm_info->port, "port");
    *pp_buffer += sizeof(p_x2_enb_comm_info->port);

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_ipv6_addr < 1) || (p_x2_enb_comm_info->num_ipv6_addr > MAX_NUM_IP_ADDR))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_enb_comm_info->num_ipv6_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %u received.", p_x2_enb_comm_info->num_ipv6_addr);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2_enb_comm_info->num_ipv6_addr, "num_ipv6_addr");
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
            if (OAM_FAILURE == rrc_il_compose_x2_ipv6_addr(pp_buffer, &p_x2_enb_comm_info->ipv6_addr[loop]))
            {
                return OAM_FAILURE;
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_enb_comm_info->num_tlas] should be in range "
            "1 to MAX_NUM_ENB_X2AP_EXT_TLAS. Incorrect value %u received.", p_x2_enb_comm_info->num_tlas);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2_enb_comm_info->num_tlas, "num_tlas");
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
            if (OAM_FAILURE == rrc_il_compose_x2_extend_trans_layer_addrs(pp_buffer, &p_x2_enb_comm_info->extend_trans_layer_addrs[loop]))
            {
                return OAM_FAILURE;
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
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2_enb_comm_info->default_sgnb] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2_enb_comm_info->default_sgnb);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2_enb_comm_info->default_sgnb, "default_sgnb");
    *pp_buffer += sizeof(p_x2_enb_comm_info->default_sgnb);
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_sctp_conf_info_len
(
    x2ap_sctp_conf_info_t *p_x2ap_sctp_conf_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_sctp_conf_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_sctp_conf_info->hbinterval);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_sctp_conf_info->pathmaxrxt);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_sctp_conf_info->init_num_ostreams);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_sctp_conf_info->init_max_instreams);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_sctp_conf_info->init_max_attempts);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_sctp_conf_info->init_max_init_timeo);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_sctp_conf_info->rto_initial);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_sctp_conf_info->rto_max);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_sctp_conf_info->rto_min);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_sctp_conf_info->bitmask);

    /* Optional element */
    if(p_x2ap_sctp_conf_info->bitmask & X2AP_SCTP_ASSOC_MAX_RETRANS_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_sctp_conf_info->assoc_max_retrans);
    }

    /* Optional element */
    if(p_x2ap_sctp_conf_info->bitmask & X2AP_SCTP_VAL_COOKIE_LIFE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_sctp_conf_info->valid_cookie_life);
    }

    /* Optional element */
    if(p_x2ap_sctp_conf_info->bitmask & X2AP_OAM_DSCP_VALUE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_sctp_conf_info->x2ap_dscp_value);
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_sctp_conf_info
(
    UInt8  **pp_buffer,
    x2ap_sctp_conf_info_t *p_x2ap_sctp_conf_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_sctp_conf_info != PNULL);

    /* This function composes x2ap_sctp_conf_info */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_sctp_conf_info->hbinterval, "hbinterval");
    *pp_buffer += sizeof(p_x2ap_sctp_conf_info->hbinterval);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_sctp_conf_info->pathmaxrxt, "pathmaxrxt");
    *pp_buffer += sizeof(p_x2ap_sctp_conf_info->pathmaxrxt);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_sctp_conf_info->init_num_ostreams, "init_num_ostreams");
    *pp_buffer += sizeof(p_x2ap_sctp_conf_info->init_num_ostreams);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_sctp_conf_info->init_max_instreams, "init_max_instreams");
    *pp_buffer += sizeof(p_x2ap_sctp_conf_info->init_max_instreams);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_sctp_conf_info->init_max_attempts, "init_max_attempts");
    *pp_buffer += sizeof(p_x2ap_sctp_conf_info->init_max_attempts);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_sctp_conf_info->init_max_init_timeo, "init_max_init_timeo");
    *pp_buffer += sizeof(p_x2ap_sctp_conf_info->init_max_init_timeo);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_sctp_conf_info->rto_initial, "rto_initial");
    *pp_buffer += sizeof(p_x2ap_sctp_conf_info->rto_initial);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_sctp_conf_info->rto_max, "rto_max");
    *pp_buffer += sizeof(p_x2ap_sctp_conf_info->rto_max);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_sctp_conf_info->rto_min, "rto_min");
    *pp_buffer += sizeof(p_x2ap_sctp_conf_info->rto_min);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_sctp_conf_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_sctp_conf_info->bitmask);

    /* Optional element */
    if(p_x2ap_sctp_conf_info->bitmask & X2AP_SCTP_ASSOC_MAX_RETRANS_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_sctp_conf_info->assoc_max_retrans, "assoc_max_retrans");
    *pp_buffer += sizeof(p_x2ap_sctp_conf_info->assoc_max_retrans);
    }

    /* Optional element */
    if(p_x2ap_sctp_conf_info->bitmask & X2AP_SCTP_VAL_COOKIE_LIFE_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_sctp_conf_info->valid_cookie_life, "valid_cookie_life");
    *pp_buffer += sizeof(p_x2ap_sctp_conf_info->valid_cookie_life);
    }

    /* Optional element */
    if(p_x2ap_sctp_conf_info->bitmask & X2AP_OAM_DSCP_VALUE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_sctp_conf_info->x2ap_dscp_value > 63))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_sctp_conf_info->x2ap_dscp_value] should be less than"
            " or equal to 63. Incorrect value %u received.", p_x2ap_sctp_conf_info->x2ap_dscp_value);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_sctp_conf_info->x2ap_dscp_value, "x2ap_dscp_value");
    *pp_buffer += sizeof(p_x2ap_sctp_conf_info->x2ap_dscp_value);
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_con_tim_info_len
(
    x2ap_con_tim_info_t *p_x2ap_con_tim_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_con_tim_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->sctp_connect_tim_val);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->x2ap_csi_guard_tim_val);

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_TIME_TO_WAIT_INDEX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->x2ap_time_to_wait_index);
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->x2ap_setup_retry_tim_val);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->sctp_connect_retry_cnt);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->x2ap_setup_retry_cnt);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->eNB_Config_Update_retry_cnt);

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_ENB_CFG_INFO_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->eNB_Config_Update_retry_timer_val);
    }

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_ENB_CFG_INFO_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->eNB_Config_Update_time_to_wait_retry_cnt);
    }

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_ENB_CFG_INFO_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->eNB_Config_Update_time_to_wait_offset);
    }

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_RESET_CFG_INFO_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->x2ap_enb_reset_retry_cnt);
    }

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_RESET_CFG_INFO_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->x2ap_enb_reset_guard_tim_val);
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->x2ap_rsu_enb_guard_tim_val);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->x2ap_rsu_rrm_guard_tim_val);

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_CELL_ACTIVATION_TIMER_INFO_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->x2ap_cell_activation_retry_cnt);
    }

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_CELL_ACTIVATION_TIMER_INFO_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->x2ap_cell_activation_guard_tim_val);
    }

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_MOBILITY_CHANGE_INFO_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->x2ap_mobility_setting_change_retry_cnt);
    }

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_MOBILITY_CHANGE_INFO_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->x2ap_mobility_setting_change_guard_tim_val);
    }

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_OAM_X2_HEALTH_MONITORING_TIMER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_con_tim_info->x2ap_health_mon_time_int);
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_con_tim_info
(
    UInt8  **pp_buffer,
    x2ap_con_tim_info_t *p_x2ap_con_tim_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_con_tim_info != PNULL);

    /* This function composes x2ap_con_tim_info */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_con_tim_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->bitmask);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_con_tim_info->sctp_connect_tim_val, "sctp_connect_tim_val");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->sctp_connect_tim_val);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_con_tim_info->x2ap_csi_guard_tim_val, "x2ap_csi_guard_tim_val");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->x2ap_csi_guard_tim_val);

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_TIME_TO_WAIT_INDEX_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_con_tim_info->x2ap_time_to_wait_index, "x2ap_time_to_wait_index");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->x2ap_time_to_wait_index);
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_con_tim_info->x2ap_setup_retry_tim_val, "x2ap_setup_retry_tim_val");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->x2ap_setup_retry_tim_val);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_con_tim_info->sctp_connect_retry_cnt, "sctp_connect_retry_cnt");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->sctp_connect_retry_cnt);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_con_tim_info->x2ap_setup_retry_cnt, "x2ap_setup_retry_cnt");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->x2ap_setup_retry_cnt);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_con_tim_info->eNB_Config_Update_retry_cnt, "eNB_Config_Update_retry_cnt");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->eNB_Config_Update_retry_cnt);

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_ENB_CFG_INFO_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_con_tim_info->eNB_Config_Update_retry_timer_val, "eNB_Config_Update_retry_timer_val");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->eNB_Config_Update_retry_timer_val);
    }

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_ENB_CFG_INFO_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_con_tim_info->eNB_Config_Update_time_to_wait_retry_cnt, "eNB_Config_Update_time_to_wait_retry_cnt");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->eNB_Config_Update_time_to_wait_retry_cnt);
    }

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_ENB_CFG_INFO_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_con_tim_info->eNB_Config_Update_time_to_wait_offset, "eNB_Config_Update_time_to_wait_offset");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->eNB_Config_Update_time_to_wait_offset);
    }

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_RESET_CFG_INFO_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_con_tim_info->x2ap_enb_reset_retry_cnt, "x2ap_enb_reset_retry_cnt");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->x2ap_enb_reset_retry_cnt);
    }

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_RESET_CFG_INFO_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_con_tim_info->x2ap_enb_reset_guard_tim_val, "x2ap_enb_reset_guard_tim_val");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->x2ap_enb_reset_guard_tim_val);
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_con_tim_info->x2ap_rsu_enb_guard_tim_val, "x2ap_rsu_enb_guard_tim_val");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->x2ap_rsu_enb_guard_tim_val);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_con_tim_info->x2ap_rsu_rrm_guard_tim_val, "x2ap_rsu_rrm_guard_tim_val");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->x2ap_rsu_rrm_guard_tim_val);

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_CELL_ACTIVATION_TIMER_INFO_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_con_tim_info->x2ap_cell_activation_retry_cnt, "x2ap_cell_activation_retry_cnt");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->x2ap_cell_activation_retry_cnt);
    }

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_CELL_ACTIVATION_TIMER_INFO_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_con_tim_info->x2ap_cell_activation_guard_tim_val, "x2ap_cell_activation_guard_tim_val");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->x2ap_cell_activation_guard_tim_val);
    }

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_MOBILITY_CHANGE_INFO_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_con_tim_info->x2ap_mobility_setting_change_retry_cnt, "x2ap_mobility_setting_change_retry_cnt");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->x2ap_mobility_setting_change_retry_cnt);
    }

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_MOBILITY_CHANGE_INFO_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_con_tim_info->x2ap_mobility_setting_change_guard_tim_val, "x2ap_mobility_setting_change_guard_tim_val");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->x2ap_mobility_setting_change_guard_tim_val);
    }

    /* Optional element */
    if(p_x2ap_con_tim_info->bitmask & X2AP_OAM_X2_HEALTH_MONITORING_TIMER_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_con_tim_info->x2ap_health_mon_time_int, "x2ap_health_mon_time_int");
    *pp_buffer += sizeof(p_x2ap_con_tim_info->x2ap_health_mon_time_int);
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_conf_param_len
(
    x2ap_conf_param_t *p_x2ap_conf_param
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_conf_param != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_sctp_conf_info_len(&p_x2ap_conf_param->sctp_conf_param);

    /* Get length of IE */
    length += rrc_il_get_x2ap_con_tim_info_len(&p_x2ap_conf_param->timer_conf_info);

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_conf_param
(
    UInt8  **pp_buffer,
    x2ap_conf_param_t *p_x2ap_conf_param
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_conf_param != PNULL);

    /* This function composes x2ap_conf_param */
    

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_sctp_conf_info(pp_buffer, &p_x2ap_conf_param->sctp_conf_param))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_con_tim_info(pp_buffer, &p_x2ap_conf_param->timer_conf_info))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_plmn_identity_len
(
    x2ap_plmn_identity_t *p_x2ap_plmn_identity
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_plmn_identity != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_plmn_identity->plmn_id);

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_plmn_identity
(
    UInt8  **pp_buffer,
    x2ap_plmn_identity_t *p_x2ap_plmn_identity
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_plmn_identity != PNULL);

    /* This function composes x2ap_plmn_identity */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_plmn_identity->plmn_id); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_plmn_identity->plmn_id[loop], "plmn_id[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_macro_enb_id_len
(
    macro_enb_id_t *p_macro_enb_id
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_macro_enb_id != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_macro_enb_id->eNB_id);

    return length;
}

static
oam_return_et
rrc_il_compose_macro_enb_id
(
    UInt8  **pp_buffer,
    macro_enb_id_t *p_macro_enb_id
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_macro_enb_id != PNULL);

    /* This function composes macro_enb_id */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_macro_enb_id->eNB_id); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_macro_enb_id->eNB_id[loop], "eNB_id[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_home_enb_id_len
(
    home_enb_id_t *p_home_enb_id
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_home_enb_id != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_home_enb_id->eNB_id);

    return length;
}

static
oam_return_et
rrc_il_compose_home_enb_id
(
    UInt8  **pp_buffer,
    home_enb_id_t *p_home_enb_id
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_home_enb_id != PNULL);

    /* This function composes home_enb_id */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_home_enb_id->eNB_id); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_home_enb_id->eNB_id[loop], "eNB_id[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2_enb_id_info_len
(
    x2_enb_id_info_t *p_x2_enb_id_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2_enb_id_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2_enb_id_info->presence_bitmask);

    /* Optional element */
    if(p_x2_enb_id_info->presence_bitmask & 1)
    {

    /* Get length of IE */
    length += rrc_il_get_macro_enb_id_len(&p_x2_enb_id_info->macro_enb_id);
    }

    /* Optional element */
    if(p_x2_enb_id_info->presence_bitmask & 2)
    {

    /* Get length of IE */
    length += rrc_il_get_home_enb_id_len(&p_x2_enb_id_info->home_enb_id);
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2_enb_id_info
(
    UInt8  **pp_buffer,
    x2_enb_id_info_t *p_x2_enb_id_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2_enb_id_info != PNULL);

    /* This function composes x2_enb_id_info */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2_enb_id_info->presence_bitmask, "presence_bitmask");
    *pp_buffer += sizeof(p_x2_enb_id_info->presence_bitmask);

    /* Optional element */
    if(p_x2_enb_id_info->presence_bitmask & 1)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_macro_enb_id(pp_buffer, &p_x2_enb_id_info->macro_enb_id))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2_enb_id_info->presence_bitmask & 2)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_home_enb_id(pp_buffer, &p_x2_enb_id_info->home_enb_id))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2_gb_enb_id_len
(
    x2_gb_enb_id_t *p_x2_gb_enb_id
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2_gb_enb_id != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_plmn_identity_len(&p_x2_gb_enb_id->plmn_identity);

    /* Get length of IE */
    length += rrc_il_get_x2_enb_id_info_len(&p_x2_gb_enb_id->enb_id);

    return length;
}

static
oam_return_et
rrc_il_compose_x2_gb_enb_id
(
    UInt8  **pp_buffer,
    x2_gb_enb_id_t *p_x2_gb_enb_id
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2_gb_enb_id != PNULL);

    /* This function composes x2_gb_enb_id */
    

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_plmn_identity(pp_buffer, &p_x2_gb_enb_id->plmn_identity))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2_enb_id_info(pp_buffer, &p_x2_gb_enb_id->enb_id))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_tac_len
(
    x2ap_tac_t *p_x2ap_tac
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_tac != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_tac->tac);

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_tac
(
    UInt8  **pp_buffer,
    x2ap_tac_t *p_x2ap_tac
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_tac != PNULL);

    /* This function composes x2ap_tac */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_tac->tac); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_tac->tac[loop], "tac[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_broadcast_plmn_len
(
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_broadcast_plmn != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_broadcast_plmn->num_bcast_plmn_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_broadcast_plmn->num_bcast_plmn_id < 1) || (p_x2ap_broadcast_plmn->num_bcast_plmn_id > MAX_NUM_BPLMN))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_broadcast_plmn->num_bcast_plmn_id] should be in range "
            "1 to MAX_NUM_BPLMN. Incorrect value %d received.", p_x2ap_broadcast_plmn->num_bcast_plmn_id);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_broadcast_plmn->num_bcast_plmn_id; loop++)
        {
            length += rrc_il_get_x2ap_plmn_identity_len(&p_x2ap_broadcast_plmn->plmn_identity[loop]);
        }
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_broadcast_plmn
(
    UInt8  **pp_buffer,
    x2ap_broadcast_plmn_t *p_x2ap_broadcast_plmn
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_broadcast_plmn != PNULL);

    /* This function composes x2ap_broadcast_plmn */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2ap_broadcast_plmn->num_bcast_plmn_id < 1) || (p_x2ap_broadcast_plmn->num_bcast_plmn_id > MAX_NUM_BPLMN))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_broadcast_plmn->num_bcast_plmn_id] should be in range "
            "1 to MAX_NUM_BPLMN. Incorrect value %u received.", p_x2ap_broadcast_plmn->num_bcast_plmn_id);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_broadcast_plmn->num_bcast_plmn_id, "num_bcast_plmn_id");
    *pp_buffer += sizeof(p_x2ap_broadcast_plmn->num_bcast_plmn_id);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_broadcast_plmn->num_bcast_plmn_id; loop++)
        {
            if (OAM_FAILURE == rrc_il_compose_x2ap_plmn_identity(pp_buffer, &p_x2ap_broadcast_plmn->plmn_identity[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_earfcn_len
(
    x2ap_earfcn_t *p_x2ap_earfcn
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_earfcn != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_earfcn->earfcn);

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_earfcn
(
    UInt8  **pp_buffer,
    x2ap_earfcn_t *p_x2ap_earfcn
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_earfcn != PNULL);

    /* This function composes x2ap_earfcn */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_earfcn->earfcn > MAX_EARFCN))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_earfcn->earfcn] should be less than"
            " or equal to MAX_EARFCN. Incorrect value %u received.", p_x2ap_earfcn->earfcn);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_earfcn->earfcn, "earfcn");
    *pp_buffer += sizeof(p_x2ap_earfcn->earfcn);

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_fdd_info_len
(
    x2ap_fdd_info_t *p_x2ap_fdd_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_fdd_info != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_earfcn_len(&p_x2ap_fdd_info->ul_earfcn);

    /* Get length of IE */
    length += rrc_il_get_x2ap_earfcn_len(&p_x2ap_fdd_info->dl_earfcn);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_fdd_info->ul_trnsmission_bdwidth);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_fdd_info->dl_trnsmission_bdwidth);

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_fdd_info
(
    UInt8  **pp_buffer,
    x2ap_fdd_info_t *p_x2ap_fdd_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_fdd_info != PNULL);

    /* This function composes x2ap_fdd_info */
    

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_earfcn(pp_buffer, &p_x2ap_fdd_info->ul_earfcn))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_earfcn(pp_buffer, &p_x2ap_fdd_info->dl_earfcn))
    {
        return OAM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_fdd_info->ul_trnsmission_bdwidth > 5))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_fdd_info->ul_trnsmission_bdwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_fdd_info->ul_trnsmission_bdwidth);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_fdd_info->ul_trnsmission_bdwidth, "ul_trnsmission_bdwidth");
    *pp_buffer += sizeof(p_x2ap_fdd_info->ul_trnsmission_bdwidth);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_fdd_info->dl_trnsmission_bdwidth > 5))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_fdd_info->dl_trnsmission_bdwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_fdd_info->dl_trnsmission_bdwidth);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_fdd_info->dl_trnsmission_bdwidth, "dl_trnsmission_bdwidth");
    *pp_buffer += sizeof(p_x2ap_fdd_info->dl_trnsmission_bdwidth);

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_special_subframe_info_len
(
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_special_subframe_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_special_subframe_info->special_subframe_patterns);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_special_subframe_info->dl_cp);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_special_subframe_info->ul_cp);

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_special_subframe_info
(
    UInt8  **pp_buffer,
    x2ap_special_subframe_info_t *p_x2ap_special_subframe_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_special_subframe_info != PNULL);

    /* This function composes x2ap_special_subframe_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->special_subframe_patterns > 8))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_special_subframe_info->special_subframe_patterns] should be less than"
            " or equal to 8. Incorrect value %u received.", p_x2ap_special_subframe_info->special_subframe_patterns);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_special_subframe_info->special_subframe_patterns, "special_subframe_patterns");
    *pp_buffer += sizeof(p_x2ap_special_subframe_info->special_subframe_patterns);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->dl_cp > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_special_subframe_info->dl_cp] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_special_subframe_info->dl_cp);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_special_subframe_info->dl_cp, "dl_cp");
    *pp_buffer += sizeof(p_x2ap_special_subframe_info->dl_cp);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_special_subframe_info->ul_cp > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_special_subframe_info->ul_cp] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_special_subframe_info->ul_cp);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_special_subframe_info->ul_cp, "ul_cp");
    *pp_buffer += sizeof(p_x2ap_special_subframe_info->ul_cp);

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_tdd_info_len
(
    x2ap_tdd_info_t *p_x2ap_tdd_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_tdd_info != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_earfcn_len(&p_x2ap_tdd_info->earfcn);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_tdd_info->trnsmission_bdwidth);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_tdd_info->subframe_assn);

    /* Get length of IE */
    length += rrc_il_get_x2ap_special_subframe_info_len(&p_x2ap_tdd_info->special_subframe_info);

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_tdd_info
(
    UInt8  **pp_buffer,
    x2ap_tdd_info_t *p_x2ap_tdd_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_tdd_info != PNULL);

    /* This function composes x2ap_tdd_info */
    

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_earfcn(pp_buffer, &p_x2ap_tdd_info->earfcn))
    {
        return OAM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_tdd_info->trnsmission_bdwidth > 5))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_tdd_info->trnsmission_bdwidth] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_tdd_info->trnsmission_bdwidth);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_tdd_info->trnsmission_bdwidth, "trnsmission_bdwidth");
    *pp_buffer += sizeof(p_x2ap_tdd_info->trnsmission_bdwidth);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_tdd_info->subframe_assn > 6))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_tdd_info->subframe_assn] should be less than"
            " or equal to 6. Incorrect value %u received.", p_x2ap_tdd_info->subframe_assn);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_tdd_info->subframe_assn, "subframe_assn");
    *pp_buffer += sizeof(p_x2ap_tdd_info->subframe_assn);

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_special_subframe_info(pp_buffer, &p_x2ap_tdd_info->special_subframe_info))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_choice_eutra_mode_len
(
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_choice_eutra_mode != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_choice_eutra_mode->bitmask);

    /* Optional element */
    if(p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_FDD)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_fdd_info_len(&p_x2ap_choice_eutra_mode->fdd_info);
    }

    /* Optional element */
    if(p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_TDD)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_tdd_info_len(&p_x2ap_choice_eutra_mode->tdd_info);
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_choice_eutra_mode
(
    UInt8  **pp_buffer,
    x2ap_choice_eutra_mode_t *p_x2ap_choice_eutra_mode
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_choice_eutra_mode != PNULL);

    /* This function composes x2ap_choice_eutra_mode */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_choice_eutra_mode->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_choice_eutra_mode->bitmask);

    /* Optional element */
    if(p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_FDD)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_fdd_info(pp_buffer, &p_x2ap_choice_eutra_mode->fdd_info))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_choice_eutra_mode->bitmask & X2AP_EUTRA_MODE_TDD)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_tdd_info(pp_buffer, &p_x2ap_choice_eutra_mode->tdd_info))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_choice_eutra_mode_info_len
(
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_choice_eutra_mode_info != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_choice_eutra_mode_len(&p_x2ap_choice_eutra_mode_info->choice_eutra_mode);

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_choice_eutra_mode_info
(
    UInt8  **pp_buffer,
    x2ap_choice_eutra_mode_info_t *p_x2ap_choice_eutra_mode_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_choice_eutra_mode_info != PNULL);

    /* This function composes x2ap_choice_eutra_mode_info */
    

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_choice_eutra_mode(pp_buffer, &p_x2ap_choice_eutra_mode_info->choice_eutra_mode))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_prach_configuration_info_len
(
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_prach_configuration_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_prach_configuration_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_prach_configuration_info->root_sequence_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_prach_configuration_info->zero_correlation_zone_config);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_prach_configuration_info->high_speed_flag);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_prach_configuration_info->prach_frequency_offset);

    /* Optional element */
    if(p_x2ap_prach_configuration_info->bitmask & X2AP_PRACH_CONFIG_INDEX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_prach_configuration_info->prach_config_index);
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_prach_configuration_info
(
    UInt8  **pp_buffer,
    x2ap_prach_configuration_info_t *p_x2ap_prach_configuration_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_prach_configuration_info != PNULL);

    /* This function composes x2ap_prach_configuration_info */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_prach_configuration_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->root_sequence_index > 837))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_prach_configuration_info->root_sequence_index] should be less than"
            " or equal to 837. Incorrect value %u received.", p_x2ap_prach_configuration_info->root_sequence_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_prach_configuration_info->root_sequence_index, "root_sequence_index");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->root_sequence_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->zero_correlation_zone_config > 15))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_prach_configuration_info->zero_correlation_zone_config] should be less than"
            " or equal to 15. Incorrect value %u received.", p_x2ap_prach_configuration_info->zero_correlation_zone_config);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_prach_configuration_info->zero_correlation_zone_config, "zero_correlation_zone_config");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->zero_correlation_zone_config);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->high_speed_flag > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_prach_configuration_info->high_speed_flag] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_prach_configuration_info->high_speed_flag);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_prach_configuration_info->high_speed_flag, "high_speed_flag");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->high_speed_flag);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->prach_frequency_offset > 94))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_prach_configuration_info->prach_frequency_offset] should be less than"
            " or equal to 94. Incorrect value %u received.", p_x2ap_prach_configuration_info->prach_frequency_offset);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_prach_configuration_info->prach_frequency_offset, "prach_frequency_offset");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->prach_frequency_offset);

    /* Optional element */
    if(p_x2ap_prach_configuration_info->bitmask & X2AP_PRACH_CONFIG_INDEX_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_prach_configuration_info->prach_config_index > 63))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_prach_configuration_info->prach_config_index] should be less than"
            " or equal to 63. Incorrect value %u received.", p_x2ap_prach_configuration_info->prach_config_index);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_prach_configuration_info->prach_config_index, "prach_config_index");
    *pp_buffer += sizeof(p_x2ap_prach_configuration_info->prach_config_index);
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_subframe_allocation_len
(
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_subframe_allocation != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_subframe_allocation->bitmask);

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
oam_return_et
rrc_il_compose_x2ap_subframe_allocation
(
    UInt8  **pp_buffer,
    x2ap_subframe_allocation_t *p_x2ap_subframe_allocation
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_subframe_allocation != PNULL);

    /* This function composes x2ap_subframe_allocation */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_subframe_allocation->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_subframe_allocation->bitmask);

    /* Optional element */
    if(p_x2ap_subframe_allocation->bitmask & X2AP_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_subframe_allocation->oneframe); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_subframe_allocation->oneframe[loop], "oneframe[]");
            *pp_buffer += sizeof(UInt8);
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
            OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_subframe_allocation->fourframe[loop], "fourframe[]");
            *pp_buffer += sizeof(UInt8);
        }
    }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_mbsfn_info_len
(
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_mbsfn_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_mbsfn_info->radioframe_alloc_period);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_mbsfn_info->radioframe_alloc_offset);

    /* Get length of IE */
    length += rrc_il_get_x2ap_subframe_allocation_len(&p_x2ap_mbsfn_info->subframe_alloc);

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_mbsfn_info
(
    UInt8  **pp_buffer,
    x2ap_mbsfn_info_t *p_x2ap_mbsfn_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_mbsfn_info != PNULL);

    /* This function composes x2ap_mbsfn_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_info->radioframe_alloc_period > 5))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_mbsfn_info->radioframe_alloc_period] should be less than"
            " or equal to 5. Incorrect value %u received.", p_x2ap_mbsfn_info->radioframe_alloc_period);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_mbsfn_info->radioframe_alloc_period, "radioframe_alloc_period");
    *pp_buffer += sizeof(p_x2ap_mbsfn_info->radioframe_alloc_period);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_info->radioframe_alloc_offset > 7))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_mbsfn_info->radioframe_alloc_offset] should be less than"
            " or equal to 7. Incorrect value %u received.", p_x2ap_mbsfn_info->radioframe_alloc_offset);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_mbsfn_info->radioframe_alloc_offset, "radioframe_alloc_offset");
    *pp_buffer += sizeof(p_x2ap_mbsfn_info->radioframe_alloc_offset);

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_subframe_allocation(pp_buffer, &p_x2ap_mbsfn_info->subframe_alloc))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_mbsfn_subframe_info_len
(
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_mbsfn_subframe_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_mbsfn_subframe_info->num_mbsfn);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_subframe_info->num_mbsfn > MAX_NUM_MBSFN))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_mbsfn_subframe_info->num_mbsfn] should be less than"
            " or equal to MAX_NUM_MBSFN. Incorrect value %d received.", p_x2ap_mbsfn_subframe_info->num_mbsfn);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_mbsfn_subframe_info->num_mbsfn; loop++)
        {
            length += rrc_il_get_x2ap_mbsfn_info_len(&p_x2ap_mbsfn_subframe_info->mbsfn_info[loop]);
        }
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_mbsfn_subframe_info
(
    UInt8  **pp_buffer,
    x2ap_mbsfn_subframe_info_t *p_x2ap_mbsfn_subframe_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_mbsfn_subframe_info != PNULL);

    /* This function composes x2ap_mbsfn_subframe_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_mbsfn_subframe_info->num_mbsfn > MAX_NUM_MBSFN))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_mbsfn_subframe_info->num_mbsfn] should be less than"
            " or equal to MAX_NUM_MBSFN. Incorrect value %u received.", p_x2ap_mbsfn_subframe_info->num_mbsfn);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_mbsfn_subframe_info->num_mbsfn, "num_mbsfn");
    *pp_buffer += sizeof(p_x2ap_mbsfn_subframe_info->num_mbsfn);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_mbsfn_subframe_info->num_mbsfn; loop++)
        {
            if (OAM_FAILURE == rrc_il_compose_x2ap_mbsfn_info(pp_buffer, &p_x2ap_mbsfn_subframe_info->mbsfn_info[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2apOam_srvd_cell_info_len
(
    x2apOam_srvd_cell_info_t *p_x2apOam_srvd_cell_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2apOam_srvd_cell_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2apOam_srvd_cell_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2apOam_srvd_cell_info->pci);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2apOam_srvd_cell_info->cell_id);

    /* Get length of IE */
    length += rrc_il_get_x2ap_tac_len(&p_x2apOam_srvd_cell_info->track_area_code);

    /* Get length of IE */
    length += rrc_il_get_x2ap_broadcast_plmn_len(&p_x2apOam_srvd_cell_info->broadcast_plmn_info);

    /* Get length of IE */
    length += rrc_il_get_x2ap_choice_eutra_mode_info_len(&p_x2apOam_srvd_cell_info->choice_eutra_mode);

    /* Optional element */
    if(p_x2apOam_srvd_cell_info->bitmask & NUM_ANTENNA_PORT_BITMASK)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2apOam_srvd_cell_info->num_antenna_port);
    }

    /* Optional element */
    if(p_x2apOam_srvd_cell_info->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_prach_configuration_info_len(&p_x2apOam_srvd_cell_info->prach_config);
    }

    /* Optional element */
    if(p_x2apOam_srvd_cell_info->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_mbsfn_subframe_info_len(&p_x2apOam_srvd_cell_info->mbsfn_subframe_info);
    }

    /* Optional element */
    if(p_x2apOam_srvd_cell_info->bitmask & X2AP_CSG_ID_INFO_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2apOam_srvd_cell_info->csg_identity);
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2apOam_srvd_cell_info
(
    UInt8  **pp_buffer,
    x2apOam_srvd_cell_info_t *p_x2apOam_srvd_cell_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2apOam_srvd_cell_info != PNULL);

    /* This function composes x2apOam_srvd_cell_info */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2apOam_srvd_cell_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2apOam_srvd_cell_info->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2apOam_srvd_cell_info->pci > 503))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2apOam_srvd_cell_info->pci] should be less than"
            " or equal to 503. Incorrect value %u received.", p_x2apOam_srvd_cell_info->pci);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2apOam_srvd_cell_info->pci, "pci");
    *pp_buffer += sizeof(p_x2apOam_srvd_cell_info->pci);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2apOam_srvd_cell_info->cell_id > MAX_CELL_INDEX))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2apOam_srvd_cell_info->cell_id] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_x2apOam_srvd_cell_info->cell_id);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2apOam_srvd_cell_info->cell_id, "cell_id");
    *pp_buffer += sizeof(p_x2apOam_srvd_cell_info->cell_id);

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_tac(pp_buffer, &p_x2apOam_srvd_cell_info->track_area_code))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_broadcast_plmn(pp_buffer, &p_x2apOam_srvd_cell_info->broadcast_plmn_info))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_choice_eutra_mode_info(pp_buffer, &p_x2apOam_srvd_cell_info->choice_eutra_mode))
    {
        return OAM_FAILURE;
    }

    /* Optional element */
    if(p_x2apOam_srvd_cell_info->bitmask & NUM_ANTENNA_PORT_BITMASK)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2apOam_srvd_cell_info->num_antenna_port > 4))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2apOam_srvd_cell_info->num_antenna_port] should be less than"
            " or equal to 4. Incorrect value %u received.", p_x2apOam_srvd_cell_info->num_antenna_port);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2apOam_srvd_cell_info->num_antenna_port, "num_antenna_port");
    *pp_buffer += sizeof(p_x2apOam_srvd_cell_info->num_antenna_port);
    }

    /* Optional element */
    if(p_x2apOam_srvd_cell_info->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_prach_configuration_info(pp_buffer, &p_x2apOam_srvd_cell_info->prach_config))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2apOam_srvd_cell_info->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_mbsfn_subframe_info(pp_buffer, &p_x2apOam_srvd_cell_info->mbsfn_subframe_info))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2apOam_srvd_cell_info->bitmask & X2AP_CSG_ID_INFO_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2apOam_srvd_cell_info->csg_identity); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_x2apOam_srvd_cell_info->csg_identity[loop], "csg_identity[]");
            *pp_buffer += sizeof(UInt8);
        }
    }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_ecgi_len
(
    x2ap_ecgi_t *p_x2ap_ecgi
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_ecgi != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_plmn_identity_len(&p_x2ap_ecgi->plmn_identity);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_ecgi->eutran_cell_id);

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_ecgi
(
    UInt8  **pp_buffer,
    x2ap_ecgi_t *p_x2ap_ecgi
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_ecgi != PNULL);

    /* This function composes x2ap_ecgi */
    

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_plmn_identity(pp_buffer, &p_x2ap_ecgi->plmn_identity))
    {
        return OAM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_ecgi->eutran_cell_id); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_ecgi->eutran_cell_id[loop], "eutran_cell_id[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_neighbour_info_len
(
    x2ap_neighbour_info_t *p_x2ap_neighbour_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_neighbour_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_neighbour_info->bitmask);

    /* Get length of IE */
    length += rrc_il_get_x2ap_ecgi_len(&p_x2ap_neighbour_info->ecgi);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_neighbour_info->pci);

    /* Get length of IE */
    length += rrc_il_get_x2ap_earfcn_len(&p_x2ap_neighbour_info->earfcn);

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_neighbour_info
(
    UInt8  **pp_buffer,
    x2ap_neighbour_info_t *p_x2ap_neighbour_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_neighbour_info != PNULL);

    /* This function composes x2ap_neighbour_info */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_neighbour_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_neighbour_info->bitmask);

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_ecgi(pp_buffer, &p_x2ap_neighbour_info->ecgi))
    {
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_neighbour_info->pci, "pci");
    *pp_buffer += sizeof(p_x2ap_neighbour_info->pci);

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_earfcn(pp_buffer, &p_x2ap_neighbour_info->earfcn))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_nbour_cell_arry_len
(
    x2ap_nbour_cell_arry_t *p_x2ap_nbour_cell_arry
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_nbour_cell_arry != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_nbour_cell_arry->num_neighbour_cell);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nbour_cell_arry->num_neighbour_cell > MAX_NEIGHBOURS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_nbour_cell_arry->num_neighbour_cell] should be less than"
            " or equal to MAX_NEIGHBOURS. Incorrect value %d received.", p_x2ap_nbour_cell_arry->num_neighbour_cell);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nbour_cell_arry->num_neighbour_cell; loop++)
        {
            length += rrc_il_get_x2ap_neighbour_info_len(&p_x2ap_nbour_cell_arry->neighbour_info_arr[loop]);
        }
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_nbour_cell_arry
(
    UInt8  **pp_buffer,
    x2ap_nbour_cell_arry_t *p_x2ap_nbour_cell_arry
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_nbour_cell_arry != PNULL);

    /* This function composes x2ap_nbour_cell_arry */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_nbour_cell_arry->num_neighbour_cell > MAX_NEIGHBOURS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_nbour_cell_arry->num_neighbour_cell] should be less than"
            " or equal to MAX_NEIGHBOURS. Incorrect value %u received.", p_x2ap_nbour_cell_arry->num_neighbour_cell);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_nbour_cell_arry->num_neighbour_cell, "num_neighbour_cell");
    *pp_buffer += sizeof(p_x2ap_nbour_cell_arry->num_neighbour_cell);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_nbour_cell_arry->num_neighbour_cell; loop++)
        {
            if (OAM_FAILURE == rrc_il_compose_x2ap_neighbour_info(pp_buffer, &p_x2ap_nbour_cell_arry->neighbour_info_arr[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2apOam_srvd_cell_arry_len
(
    x2apOam_srvd_cell_arry_t *p_x2apOam_srvd_cell_arry
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2apOam_srvd_cell_arry != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2apOam_srvd_cell_info_len(&p_x2apOam_srvd_cell_arry->served_cell_info);

    /* Get length of IE */
    length += rrc_il_get_x2ap_nbour_cell_arry_len(&p_x2apOam_srvd_cell_arry->nbour_cell_info);

    return length;
}

static
oam_return_et
rrc_il_compose_x2apOam_srvd_cell_arry
(
    UInt8  **pp_buffer,
    x2apOam_srvd_cell_arry_t *p_x2apOam_srvd_cell_arry
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2apOam_srvd_cell_arry != PNULL);

    /* This function composes x2apOam_srvd_cell_arry */
    

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2apOam_srvd_cell_info(pp_buffer, &p_x2apOam_srvd_cell_arry->served_cell_info))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_nbour_cell_arry(pp_buffer, &p_x2apOam_srvd_cell_arry->nbour_cell_info))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2apOam_srvd_cells_len
(
    x2apOam_srvd_cells_t *p_x2apOam_srvd_cells
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2apOam_srvd_cells != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2apOam_srvd_cells->num_srvd_cell);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2apOam_srvd_cells->num_srvd_cell > MAX_SERVED_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2apOam_srvd_cells->num_srvd_cell] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %d received.", p_x2apOam_srvd_cells->num_srvd_cell);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2apOam_srvd_cells->num_srvd_cell; loop++)
        {
            length += rrc_il_get_x2apOam_srvd_cell_arry_len(&p_x2apOam_srvd_cells->srvd_cell_arr[loop]);
        }
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2apOam_srvd_cells
(
    UInt8  **pp_buffer,
    x2apOam_srvd_cells_t *p_x2apOam_srvd_cells
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2apOam_srvd_cells != PNULL);

    /* This function composes x2apOam_srvd_cells */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2apOam_srvd_cells->num_srvd_cell > MAX_SERVED_CELLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2apOam_srvd_cells->num_srvd_cell] should be less than"
            " or equal to MAX_SERVED_CELLS. Incorrect value %u received.", p_x2apOam_srvd_cells->num_srvd_cell);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2apOam_srvd_cells->num_srvd_cell, "num_srvd_cell");
    *pp_buffer += sizeof(p_x2apOam_srvd_cells->num_srvd_cell);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2apOam_srvd_cells->num_srvd_cell; loop++)
        {
            if (OAM_FAILURE == rrc_il_compose_x2apOam_srvd_cell_arry(pp_buffer, &p_x2apOam_srvd_cells->srvd_cell_arr[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_gu_group_id_len
(
    x2ap_gu_group_id_t *p_x2ap_gu_group_id
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_gu_group_id != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_plmn_identity_len(&p_x2ap_gu_group_id->plmn_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_gu_group_id->mme_group_id);

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_gu_group_id
(
    UInt8  **pp_buffer,
    x2ap_gu_group_id_t *p_x2ap_gu_group_id
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_gu_group_id != PNULL);

    /* This function composes x2ap_gu_group_id */
    

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_plmn_identity(pp_buffer, &p_x2ap_gu_group_id->plmn_id))
    {
        return OAM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_gu_group_id->mme_group_id); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_gu_group_id->mme_group_id[loop], "mme_group_id[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_gu_grp_ids_len
(
    x2ap_gu_grp_ids_t *p_x2ap_gu_grp_ids
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_gu_grp_ids != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_gu_grp_ids->num_gu_group_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_gu_grp_ids->num_gu_group_id > MAX_ENB_POOLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_gu_grp_ids->num_gu_group_id] should be less than"
            " or equal to MAX_ENB_POOLS. Incorrect value %d received.", p_x2ap_gu_grp_ids->num_gu_group_id);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_gu_grp_ids->num_gu_group_id; loop++)
        {
            length += rrc_il_get_x2ap_gu_group_id_len(&p_x2ap_gu_grp_ids->group_id_arr[loop]);
        }
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_gu_grp_ids
(
    UInt8  **pp_buffer,
    x2ap_gu_grp_ids_t *p_x2ap_gu_grp_ids
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_gu_grp_ids != PNULL);

    /* This function composes x2ap_gu_grp_ids */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_gu_grp_ids->num_gu_group_id > MAX_ENB_POOLS))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_gu_grp_ids->num_gu_group_id] should be less than"
            " or equal to MAX_ENB_POOLS. Incorrect value %u received.", p_x2ap_gu_grp_ids->num_gu_group_id);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_gu_grp_ids->num_gu_group_id, "num_gu_group_id");
    *pp_buffer += sizeof(p_x2ap_gu_grp_ids->num_gu_group_id);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_gu_grp_ids->num_gu_group_id; loop++)
        {
            if (OAM_FAILURE == rrc_il_compose_x2ap_gu_group_id(pp_buffer, &p_x2ap_gu_grp_ids->group_id_arr[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_info_len
(
    x2ap_info_t *p_x2ap_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_info != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2_enb_comm_info_len(&p_x2ap_info->local_enb_comm_info);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_info->num_peer_enb);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_info->num_peer_enb > MAX_PEER_ENB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_info->num_peer_enb] should be less than"
            " or equal to MAX_PEER_ENB. Incorrect value %d received.", p_x2ap_info->num_peer_enb);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_info->num_peer_enb; loop++)
        {
            length += rrc_il_get_x2_enb_comm_info_len(&p_x2ap_info->peer_enb_comm_info[loop]);
        }
    }

    /* Get length of IE */
    length += rrc_il_get_x2ap_conf_param_len(&p_x2ap_info->conf_param);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_info->global_enb_id);

    /* Get length of IE */
    length += rrc_il_get_x2apOam_srvd_cells_len(&p_x2ap_info->srvd_cell);

    /* Get length of IE */
    length += rrc_il_get_x2ap_gu_grp_ids_len(&p_x2ap_info->x2ap_gu_grp_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_info->log_level);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_info->num_peer_gnb);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_info->num_peer_gnb > MAX_PEER_GNB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_info->num_peer_gnb] should be less than"
            " or equal to MAX_PEER_GNB. Incorrect value %d received.", p_x2ap_info->num_peer_gnb);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_info->num_peer_gnb; loop++)
        {
            length += rrc_il_get_x2_enb_comm_info_len(&p_x2ap_info->peer_gnb_comm_info[loop]);
        }
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_info
(
    UInt8  **pp_buffer,
    x2ap_info_t *p_x2ap_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_info != PNULL);

    /* This function composes x2ap_info */
    

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2_enb_comm_info(pp_buffer, &p_x2ap_info->local_enb_comm_info))
    {
        return OAM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_info->num_peer_enb > MAX_PEER_ENB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_info->num_peer_enb] should be less than"
            " or equal to MAX_PEER_ENB. Incorrect value %u received.", p_x2ap_info->num_peer_enb);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_info->num_peer_enb, "num_peer_enb");
    *pp_buffer += sizeof(p_x2ap_info->num_peer_enb);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_info->num_peer_enb; loop++)
        {
            if (OAM_FAILURE == rrc_il_compose_x2_enb_comm_info(pp_buffer, &p_x2ap_info->peer_enb_comm_info[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_conf_param(pp_buffer, &p_x2ap_info->conf_param))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_info->global_enb_id))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2apOam_srvd_cells(pp_buffer, &p_x2ap_info->srvd_cell))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_gu_grp_ids(pp_buffer, &p_x2ap_info->x2ap_gu_grp_id))
    {
        return OAM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_info->log_level > 6))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_info->log_level] should be less than"
            " or equal to 6. Incorrect value %u received.", p_x2ap_info->log_level);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_info->log_level, "log_level");
    *pp_buffer += sizeof(p_x2ap_info->log_level);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_info->num_peer_gnb > MAX_PEER_GNB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_info->num_peer_gnb] should be less than"
            " or equal to MAX_PEER_GNB. Incorrect value %u received.", p_x2ap_info->num_peer_gnb);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_info->num_peer_gnb, "num_peer_gnb");
    *pp_buffer += sizeof(p_x2ap_info->num_peer_gnb);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_info->num_peer_gnb; loop++)
        {
            if (OAM_FAILURE == rrc_il_compose_x2_enb_comm_info(pp_buffer, &p_x2ap_info->peer_gnb_comm_info[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_son_comm_info_len
(
    son_comm_info_t *p_son_comm_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_son_comm_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_son_comm_info->bitmask);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_son_comm_info->ip_addr);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_son_comm_info->port);

    return length;
}

static
oam_return_et
rrc_il_compose_son_comm_info
(
    UInt8  **pp_buffer,
    son_comm_info_t *p_son_comm_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_son_comm_info != PNULL);

    /* This function composes son_comm_info */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_son_comm_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_son_comm_info->bitmask);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_son_comm_info->ip_addr); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_son_comm_info->ip_addr[loop], "ip_addr[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_son_comm_info->port, "port");
    *pp_buffer += sizeof(p_son_comm_info->port);

    return OAM_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_oam_provision_resp_len
(
    x2ap_oam_provision_resp_t *p_x2ap_oam_provision_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_provision_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_provision_resp->response);

    return length;
}

oam_return_et
rrc_il_compose_x2ap_oam_provision_resp
(
    UInt8  **pp_buffer,
    x2ap_oam_provision_resp_t *p_x2ap_oam_provision_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_provision_resp != PNULL);

    /* This function composes x2ap_oam_provision_resp */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_provision_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_provision_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_oam_provision_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_provision_resp->response, "response");
    *pp_buffer += sizeof(p_x2ap_oam_provision_resp->response);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_x2ap_il_send_x2ap_oam_provision_resp
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_OAM_PROVISION_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_x2ap_il_send_x2ap_oam_provision_resp
(
    x2ap_oam_provision_resp_t  *p_x2ap_oam_provision_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_x2apOam_msg;

    OAM_NULL_CHECK(p_x2ap_oam_provision_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_oam_provision_resp_len(p_x2ap_oam_provision_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):X2AP_OAM_PROVISION_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_x2apOam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_x2apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_OAM_PROVISION_RESP, msg_api_length);

    /* Fill interface header */
    p_x2apOam_msg = p_x2apOam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_x2apOam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, X2AP_OAM_PROVISION_RESP, msg_length,cell_index);

    /* Fill X2AP_OAM_PROVISION_RESP message */
    p_x2apOam_msg = p_x2apOam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_provision_resp(&p_x2apOam_msg, p_x2ap_oam_provision_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_x2apOam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_oam_get_log_level_resp_len
(
    x2ap_oam_get_log_level_resp_t *p_x2ap_oam_get_log_level_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_get_log_level_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_get_log_level_resp->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_get_log_level_resp->log_on_off);

    /* Optional element */
    if(p_x2ap_oam_get_log_level_resp->bitmask & X2AP_OAM_LOG_LEVEL_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_get_log_level_resp->log_level);
    }

    return length;
}

oam_return_et
rrc_il_compose_x2ap_oam_get_log_level_resp
(
    UInt8  **pp_buffer,
    x2ap_oam_get_log_level_resp_t *p_x2ap_oam_get_log_level_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_get_log_level_resp != PNULL);

    /* This function composes x2ap_oam_get_log_level_resp */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_oam_get_log_level_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_oam_get_log_level_resp->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_get_log_level_resp->log_on_off > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_get_log_level_resp->log_on_off] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_oam_get_log_level_resp->log_on_off);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_get_log_level_resp->log_on_off, "log_on_off");
    *pp_buffer += sizeof(p_x2ap_oam_get_log_level_resp->log_on_off);

    /* Optional element */
    if(p_x2ap_oam_get_log_level_resp->bitmask & X2AP_OAM_LOG_LEVEL_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_get_log_level_resp->log_level > 6))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_get_log_level_resp->log_level] should be less than"
            " or equal to 6. Incorrect value %u received.", p_x2ap_oam_get_log_level_resp->log_level);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_get_log_level_resp->log_level, "log_level");
    *pp_buffer += sizeof(p_x2ap_oam_get_log_level_resp->log_level);
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_x2ap_il_send_x2ap_oam_get_log_level_resp
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_OAM_GET_LOG_LEVEL_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_x2ap_il_send_x2ap_oam_get_log_level_resp
(
    x2ap_oam_get_log_level_resp_t  *p_x2ap_oam_get_log_level_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_x2apOam_msg;

    OAM_NULL_CHECK(p_x2ap_oam_get_log_level_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_oam_get_log_level_resp_len(p_x2ap_oam_get_log_level_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):X2AP_OAM_GET_LOG_LEVEL_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_x2apOam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_x2apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_OAM_GET_LOG_LEVEL_RESP, msg_api_length);

    /* Fill interface header */
    p_x2apOam_msg = p_x2apOam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_x2apOam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, X2AP_OAM_GET_LOG_LEVEL_RESP, msg_length,cell_index);

    /* Fill X2AP_OAM_GET_LOG_LEVEL_RESP message */
    p_x2apOam_msg = p_x2apOam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_get_log_level_resp(&p_x2apOam_msg, p_x2ap_oam_get_log_level_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_x2apOam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_oam_set_log_level_req_len
(
    x2ap_oam_set_log_level_req_t *p_x2ap_oam_set_log_level_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_set_log_level_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_set_log_level_req->log_level);

    return length;
}

oam_return_et
rrc_il_compose_x2ap_oam_set_log_level_req
(
    UInt8  **pp_buffer,
    x2ap_oam_set_log_level_req_t *p_x2ap_oam_set_log_level_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_set_log_level_req != PNULL);

    /* This function composes x2ap_oam_set_log_level_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_set_log_level_req->log_level > 6))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_set_log_level_req->log_level] should be less than"
            " or equal to 6. Incorrect value %u received.", p_x2ap_oam_set_log_level_req->log_level);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_set_log_level_req->log_level, "log_level");
    *pp_buffer += sizeof(p_x2ap_oam_set_log_level_req->log_level);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_x2ap_il_send_x2ap_oam_set_log_level_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_OAM_SET_LOG_LEVEL_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_x2ap_il_send_x2ap_oam_set_log_level_req
(
    x2ap_oam_set_log_level_req_t  *p_x2ap_oam_set_log_level_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_x2apOam_msg;

    OAM_NULL_CHECK(p_x2ap_oam_set_log_level_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_oam_set_log_level_req_len(p_x2ap_oam_set_log_level_req);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):X2AP_OAM_SET_LOG_LEVEL_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_x2apOam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_x2apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_OAM_SET_LOG_LEVEL_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apOam_msg = p_x2apOam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_x2apOam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, X2AP_OAM_SET_LOG_LEVEL_REQ, msg_length,cell_index);

    /* Fill X2AP_OAM_SET_LOG_LEVEL_REQ message */
    p_x2apOam_msg = p_x2apOam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_set_log_level_req(&p_x2apOam_msg, p_x2ap_oam_set_log_level_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_x2apOam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_oam_set_log_level_resp_len
(
    x2ap_oam_set_log_level_resp_t *p_x2ap_oam_set_log_level_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_set_log_level_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_set_log_level_resp->response);

    return length;
}

oam_return_et
rrc_il_compose_x2ap_oam_set_log_level_resp
(
    UInt8  **pp_buffer,
    x2ap_oam_set_log_level_resp_t *p_x2ap_oam_set_log_level_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_set_log_level_resp != PNULL);

    /* This function composes x2ap_oam_set_log_level_resp */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_set_log_level_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_set_log_level_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_oam_set_log_level_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_set_log_level_resp->response, "response");
    *pp_buffer += sizeof(p_x2ap_oam_set_log_level_resp->response);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_x2ap_il_send_x2ap_oam_set_log_level_resp
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_OAM_SET_LOG_LEVEL_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_x2ap_il_send_x2ap_oam_set_log_level_resp
(
    x2ap_oam_set_log_level_resp_t  *p_x2ap_oam_set_log_level_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_x2apOam_msg;

    OAM_NULL_CHECK(p_x2ap_oam_set_log_level_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_oam_set_log_level_resp_len(p_x2ap_oam_set_log_level_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):X2AP_OAM_SET_LOG_LEVEL_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_x2apOam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_x2apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_OAM_SET_LOG_LEVEL_RESP, msg_api_length);

    /* Fill interface header */
    p_x2apOam_msg = p_x2apOam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_x2apOam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, X2AP_OAM_SET_LOG_LEVEL_RESP, msg_length,cell_index);

    /* Fill X2AP_OAM_SET_LOG_LEVEL_RESP message */
    p_x2apOam_msg = p_x2apOam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_set_log_level_resp(&p_x2apOam_msg, p_x2ap_oam_set_log_level_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_x2apOam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_oam_log_enable_req_len
(
    x2ap_oam_log_enable_req_t *p_x2ap_oam_log_enable_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_log_enable_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_log_enable_req->log_on_off);

    return length;
}

oam_return_et
rrc_il_compose_x2ap_oam_log_enable_req
(
    UInt8  **pp_buffer,
    x2ap_oam_log_enable_req_t *p_x2ap_oam_log_enable_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_log_enable_req != PNULL);

    /* This function composes x2ap_oam_log_enable_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_log_enable_req->log_on_off > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_log_enable_req->log_on_off] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_oam_log_enable_req->log_on_off);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_log_enable_req->log_on_off, "log_on_off");
    *pp_buffer += sizeof(p_x2ap_oam_log_enable_req->log_on_off);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_x2ap_il_send_x2ap_oam_log_enable_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_OAM_LOG_ENABLE_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_x2ap_il_send_x2ap_oam_log_enable_req
(
    x2ap_oam_log_enable_req_t  *p_x2ap_oam_log_enable_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_x2apOam_msg;

    OAM_NULL_CHECK(p_x2ap_oam_log_enable_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_oam_log_enable_req_len(p_x2ap_oam_log_enable_req);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):X2AP_OAM_LOG_ENABLE_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_x2apOam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_x2apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_OAM_LOG_ENABLE_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apOam_msg = p_x2apOam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_x2apOam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, X2AP_OAM_LOG_ENABLE_REQ, msg_length,cell_index);

    /* Fill X2AP_OAM_LOG_ENABLE_REQ message */
    p_x2apOam_msg = p_x2apOam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_log_enable_req(&p_x2apOam_msg, p_x2ap_oam_log_enable_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_x2apOam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_oam_log_enable_resp_len
(
    x2ap_oam_log_enable_resp_t *p_x2ap_oam_log_enable_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_log_enable_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_log_enable_resp->response);

    return length;
}

oam_return_et
rrc_il_compose_x2ap_oam_log_enable_resp
(
    UInt8  **pp_buffer,
    x2ap_oam_log_enable_resp_t *p_x2ap_oam_log_enable_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_log_enable_resp != PNULL);

    /* This function composes x2ap_oam_log_enable_resp */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_log_enable_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_log_enable_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_oam_log_enable_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_log_enable_resp->response, "response");
    *pp_buffer += sizeof(p_x2ap_oam_log_enable_resp->response);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_x2ap_il_send_x2ap_oam_log_enable_resp
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_OAM_LOG_ENABLE_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_x2ap_il_send_x2ap_oam_log_enable_resp
(
    x2ap_oam_log_enable_resp_t  *p_x2ap_oam_log_enable_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_x2apOam_msg;

    OAM_NULL_CHECK(p_x2ap_oam_log_enable_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_oam_log_enable_resp_len(p_x2ap_oam_log_enable_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):X2AP_OAM_LOG_ENABLE_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_x2apOam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_x2apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_OAM_LOG_ENABLE_RESP, msg_api_length);

    /* Fill interface header */
    p_x2apOam_msg = p_x2apOam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_x2apOam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, X2AP_OAM_LOG_ENABLE_RESP, msg_length,cell_index);

    /* Fill X2AP_OAM_LOG_ENABLE_RESP message */
    p_x2apOam_msg = p_x2apOam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_log_enable_resp(&p_x2apOam_msg, p_x2ap_oam_log_enable_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_x2apOam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_oam_x2_link_down_len
(
    x2ap_oam_x2_link_down_t *p_x2ap_oam_x2_link_down
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_x2_link_down != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_x2_link_down->bitmask);

    /* Optional element */
    if(p_x2ap_oam_x2_link_down->bitmask & X2AP_OAM_X2_LINK_DOWN_ENB_GB_ID_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_oam_x2_link_down->peer_enb_gb_id);
    }

    /* Optional element */
    if(p_x2ap_oam_x2_link_down->bitmask & X2AP_OAM_X2_LINK_DOWN_IPV4_ADDRESS_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_oam_x2_link_down->ip_addr);
    }

    /* Optional element */
    if(p_x2ap_oam_x2_link_down->bitmask & X2AP_OAM_X2_LINK_DOWN_IPV6_ADDRESS_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_oam_x2_link_down->ipv6_addr);
    }

    /* Optional element */
    if(p_x2ap_oam_x2_link_down->bitmask & X2AP_OAM_X2_LINK_DOWN_PORT_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_x2_link_down->port);
    }

    /* Optional element */
    if(p_x2ap_oam_x2_link_down->bitmask & X2AP_OAM_X2_LINK_DOWN_GNB_GB_ID_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2_gb_gnb_id_len(&p_x2ap_oam_x2_link_down->peer_gnb_gb_id);
    }

    return length;
}

oam_return_et
rrc_il_compose_x2ap_oam_x2_link_down
(
    UInt8  **pp_buffer,
    x2ap_oam_x2_link_down_t *p_x2ap_oam_x2_link_down
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_x2_link_down != PNULL);

    /* This function composes x2ap_oam_x2_link_down */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_x2_link_down->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_oam_x2_link_down->bitmask);

    /* Optional element */
    if(p_x2ap_oam_x2_link_down->bitmask & X2AP_OAM_X2_LINK_DOWN_ENB_GB_ID_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_oam_x2_link_down->peer_enb_gb_id))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_oam_x2_link_down->bitmask & X2AP_OAM_X2_LINK_DOWN_IPV4_ADDRESS_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_oam_x2_link_down->ip_addr); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_x2_link_down->ip_addr[loop], "ip_addr[]");
            *pp_buffer += sizeof(UInt8);
        }
    }
    }

    /* Optional element */
    if(p_x2ap_oam_x2_link_down->bitmask & X2AP_OAM_X2_LINK_DOWN_IPV6_ADDRESS_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_oam_x2_link_down->ipv6_addr); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_x2_link_down->ipv6_addr[loop], "ipv6_addr[]");
            *pp_buffer += sizeof(UInt8);
        }
    }
    }

    /* Optional element */
    if(p_x2ap_oam_x2_link_down->bitmask & X2AP_OAM_X2_LINK_DOWN_PORT_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_oam_x2_link_down->port, "port");
    *pp_buffer += sizeof(p_x2ap_oam_x2_link_down->port);
    }

    /* Optional element */
    if(p_x2ap_oam_x2_link_down->bitmask & X2AP_OAM_X2_LINK_DOWN_GNB_GB_ID_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2_gb_gnb_id(pp_buffer, &p_x2ap_oam_x2_link_down->peer_gnb_gb_id))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_x2ap_il_send_x2ap_oam_x2_link_down
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_OAM_LINK_DOWN_IND message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_x2ap_il_send_x2ap_oam_x2_link_down
(
    x2ap_oam_x2_link_down_t  *p_x2ap_oam_x2_link_down,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_x2apOam_msg;

    OAM_NULL_CHECK(p_x2ap_oam_x2_link_down != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_oam_x2_link_down_len(p_x2ap_oam_x2_link_down);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):X2AP_OAM_LINK_DOWN_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_x2apOam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_x2apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_OAM_LINK_DOWN_IND, msg_api_length);

    /* Fill interface header */
    p_x2apOam_msg = p_x2apOam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_x2apOam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, X2AP_OAM_LINK_DOWN_IND, msg_length,cell_index);

    /* Fill X2AP_OAM_LINK_DOWN_IND message */
    p_x2apOam_msg = p_x2apOam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_x2_link_down(&p_x2apOam_msg, p_x2ap_oam_x2_link_down))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_x2apOam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2_gnb_id_len
(
    x2_gnb_id_t *p_x2_gnb_id
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2_gnb_id != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2_gnb_id->x2_gnb_id);

    return length;
}

static
oam_return_et
rrc_il_compose_x2_gnb_id
(
    UInt8  **pp_buffer,
    x2_gnb_id_t *p_x2_gnb_id
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2_gnb_id != PNULL);

    /* This function composes x2_gnb_id */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2_gnb_id->x2_gnb_id); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_x2_gnb_id->x2_gnb_id[loop], "x2_gnb_id[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2_gb_gnb_id_len
(
    x2_gb_gnb_id_t *p_x2_gb_gnb_id
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2_gb_gnb_id != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2ap_plmn_identity_len(&p_x2_gb_gnb_id->plmn_identity);

    /* Get length of IE */
    length += rrc_il_get_x2_gnb_id_len(&p_x2_gb_gnb_id->x2_en_gnb_id);

    return length;
}

static
oam_return_et
rrc_il_compose_x2_gb_gnb_id
(
    UInt8  **pp_buffer,
    x2_gb_gnb_id_t *p_x2_gb_gnb_id
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2_gb_gnb_id != PNULL);

    /* This function composes x2_gb_gnb_id */
    

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_plmn_identity(pp_buffer, &p_x2_gb_gnb_id->plmn_identity))
    {
        return OAM_FAILURE;
    }

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2_gnb_id(pp_buffer, &p_x2_gb_gnb_id->x2_en_gnb_id))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_oam_x2_link_up_len
(
    x2ap_oam_x2_link_up_t *p_x2ap_oam_x2_link_up
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_x2_link_up != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_x2_link_up->bitmask);

    /* Optional element */
    if(p_x2ap_oam_x2_link_up->bitmask & X2AP_OAM_X2_LINK_UP_ENB_ID_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_oam_x2_link_up->peer_enb_gb_id);
    }

    /* Get length of IE */
    length += rrc_il_get_x2_enb_comm_info_len(&p_x2ap_oam_x2_link_up->enb_comm_info);

    /* Optional element */
    if(p_x2ap_oam_x2_link_up->bitmask & X2AP_OAM_X2_LINK_UP_IPV4_ADDRESS_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_oam_x2_link_up->ip_addr);
    }

    /* Optional element */
    if(p_x2ap_oam_x2_link_up->bitmask & X2AP_OAM_X2_LINK_UP_IPV6_ADDRESS_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_oam_x2_link_up->ipv6_addr);
    }

    /* Optional element */
    if(p_x2ap_oam_x2_link_up->bitmask & X2AP_OAM_X2_LINK_UP_PORT_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_x2_link_up->port);
    }

    /* Optional element */
    if(p_x2ap_oam_x2_link_up->bitmask & X2AP_OAM_X2_LINK_UP_GNB_ID_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2_gb_gnb_id_len(&p_x2ap_oam_x2_link_up->peer_gnb_gb_id);
    }

    return length;
}

oam_return_et
rrc_il_compose_x2ap_oam_x2_link_up
(
    UInt8  **pp_buffer,
    x2ap_oam_x2_link_up_t *p_x2ap_oam_x2_link_up
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_x2_link_up != PNULL);

    /* This function composes x2ap_oam_x2_link_up */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_x2_link_up->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_oam_x2_link_up->bitmask);

    /* Optional element */
    if(p_x2ap_oam_x2_link_up->bitmask & X2AP_OAM_X2_LINK_UP_ENB_ID_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_oam_x2_link_up->peer_enb_gb_id))
    {
        return OAM_FAILURE;
    }
    }

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2_enb_comm_info(pp_buffer, &p_x2ap_oam_x2_link_up->enb_comm_info))
    {
        return OAM_FAILURE;
    }

    /* Optional element */
    if(p_x2ap_oam_x2_link_up->bitmask & X2AP_OAM_X2_LINK_UP_IPV4_ADDRESS_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_oam_x2_link_up->ip_addr); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_x2_link_up->ip_addr[loop], "ip_addr[]");
            *pp_buffer += sizeof(UInt8);
        }
    }
    }

    /* Optional element */
    if(p_x2ap_oam_x2_link_up->bitmask & X2AP_OAM_X2_LINK_UP_IPV6_ADDRESS_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_oam_x2_link_up->ipv6_addr); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_x2_link_up->ipv6_addr[loop], "ipv6_addr[]");
            *pp_buffer += sizeof(UInt8);
        }
    }
    }

    /* Optional element */
    if(p_x2ap_oam_x2_link_up->bitmask & X2AP_OAM_X2_LINK_UP_PORT_PRESENT)
    {

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_oam_x2_link_up->port, "port");
    *pp_buffer += sizeof(p_x2ap_oam_x2_link_up->port);
    }

    /* Optional element */
    if(p_x2ap_oam_x2_link_up->bitmask & X2AP_OAM_X2_LINK_UP_GNB_ID_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2_gb_gnb_id(pp_buffer, &p_x2ap_oam_x2_link_up->peer_gnb_gb_id))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_x2ap_il_send_x2ap_oam_x2_link_up
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_OAM_LINK_UP_IND message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_x2ap_il_send_x2ap_oam_x2_link_up
(
    x2ap_oam_x2_link_up_t  *p_x2ap_oam_x2_link_up,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_x2apOam_msg;

    OAM_NULL_CHECK(p_x2ap_oam_x2_link_up != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_oam_x2_link_up_len(p_x2ap_oam_x2_link_up);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):X2AP_OAM_LINK_UP_IND", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_x2apOam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_x2apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_OAM_LINK_UP_IND, msg_api_length);

    /* Fill interface header */
    p_x2apOam_msg = p_x2apOam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_x2apOam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, X2AP_OAM_LINK_UP_IND, msg_length,cell_index);

    /* Fill X2AP_OAM_LINK_UP_IND message */
    p_x2apOam_msg = p_x2apOam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_x2_link_up(&p_x2apOam_msg, p_x2ap_oam_x2_link_up))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_x2apOam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_oam_x2_link_setup_req_len
(
    x2ap_oam_x2_link_setup_req_t *p_x2ap_oam_x2_link_setup_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_x2_link_setup_req != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_oam_x2_link_setup_req->peer_enb_gb_id);

    return length;
}

oam_return_et
rrc_il_compose_x2ap_oam_x2_link_setup_req
(
    UInt8  **pp_buffer,
    x2ap_oam_x2_link_setup_req_t *p_x2ap_oam_x2_link_setup_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_x2_link_setup_req != PNULL);

    /* This function composes x2ap_oam_x2_link_setup_req */
    

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_oam_x2_link_setup_req->peer_enb_gb_id))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_x2ap_il_send_x2ap_oam_x2_link_setup_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_OAM_LINK_SETUP_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_x2ap_il_send_x2ap_oam_x2_link_setup_req
(
    x2ap_oam_x2_link_setup_req_t  *p_x2ap_oam_x2_link_setup_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_x2apOam_msg;

    OAM_NULL_CHECK(p_x2ap_oam_x2_link_setup_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_oam_x2_link_setup_req_len(p_x2ap_oam_x2_link_setup_req);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):X2AP_OAM_LINK_SETUP_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_x2apOam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_x2apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_OAM_LINK_SETUP_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apOam_msg = p_x2apOam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_x2apOam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, X2AP_OAM_LINK_SETUP_REQ, msg_length,cell_index);

    /* Fill X2AP_OAM_LINK_SETUP_REQ message */
    p_x2apOam_msg = p_x2apOam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_x2_link_setup_req(&p_x2apOam_msg, p_x2ap_oam_x2_link_setup_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_x2apOam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_oam_x2_link_setup_resp_len
(
    x2ap_oam_x2_link_setup_resp_t *p_x2ap_oam_x2_link_setup_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_x2_link_setup_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_x2_link_setup_resp->response);

    return length;
}

oam_return_et
rrc_il_compose_x2ap_oam_x2_link_setup_resp
(
    UInt8  **pp_buffer,
    x2ap_oam_x2_link_setup_resp_t *p_x2ap_oam_x2_link_setup_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_x2_link_setup_resp != PNULL);

    /* This function composes x2ap_oam_x2_link_setup_resp */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_x2_link_setup_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_x2_link_setup_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_oam_x2_link_setup_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_x2_link_setup_resp->response, "response");
    *pp_buffer += sizeof(p_x2ap_oam_x2_link_setup_resp->response);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_x2ap_il_send_x2ap_oam_x2_link_setup_resp
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_OAM_LINK_SETUP_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_x2ap_il_send_x2ap_oam_x2_link_setup_resp
(
    x2ap_oam_x2_link_setup_resp_t  *p_x2ap_oam_x2_link_setup_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_x2apOam_msg;

    OAM_NULL_CHECK(p_x2ap_oam_x2_link_setup_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_oam_x2_link_setup_resp_len(p_x2ap_oam_x2_link_setup_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):X2AP_OAM_LINK_SETUP_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_x2apOam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_x2apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_OAM_LINK_SETUP_RESP, msg_api_length);

    /* Fill interface header */
    p_x2apOam_msg = p_x2apOam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_x2apOam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, X2AP_OAM_LINK_SETUP_RESP, msg_length,cell_index);

    /* Fill X2AP_OAM_LINK_SETUP_RESP message */
    p_x2apOam_msg = p_x2apOam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_x2_link_setup_resp(&p_x2apOam_msg, p_x2ap_oam_x2_link_setup_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_x2apOam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_oam_cleanup_resp_len
(
    x2ap_oam_cleanup_resp_t *p_x2ap_oam_cleanup_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_cleanup_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_cleanup_resp->response);

    return length;
}

oam_return_et
rrc_il_compose_x2ap_oam_cleanup_resp
(
    UInt8  **pp_buffer,
    x2ap_oam_cleanup_resp_t *p_x2ap_oam_cleanup_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_cleanup_resp != PNULL);

    /* This function composes x2ap_oam_cleanup_resp */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_cleanup_resp->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_cleanup_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2ap_oam_cleanup_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_cleanup_resp->response, "response");
    *pp_buffer += sizeof(p_x2ap_oam_cleanup_resp->response);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_x2ap_il_send_x2ap_oam_cleanup_resp
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_OAM_CLEANUP_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_x2ap_il_send_x2ap_oam_cleanup_resp
(
    x2ap_oam_cleanup_resp_t  *p_x2ap_oam_cleanup_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_x2apOam_msg;

    OAM_NULL_CHECK(p_x2ap_oam_cleanup_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_oam_cleanup_resp_len(p_x2ap_oam_cleanup_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):X2AP_OAM_CLEANUP_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_x2apOam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_x2apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_OAM_CLEANUP_RESP, msg_api_length);

    /* Fill interface header */
    p_x2apOam_msg = p_x2apOam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_x2apOam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, X2AP_OAM_CLEANUP_RESP, msg_length,cell_index);

    /* Fill X2AP_OAM_CLEANUP_RESP message */
    p_x2apOam_msg = p_x2apOam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_cleanup_resp(&p_x2apOam_msg, p_x2ap_oam_cleanup_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_x2apOam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_oam_get_sctp_status_req_len
(
    x2ap_oam_get_sctp_status_req_t *p_x2ap_oam_get_sctp_status_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_get_sctp_status_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_get_sctp_status_req->bitmask);

    /* Optional element */
    if(p_x2ap_oam_get_sctp_status_req->bitmask & X2AP_OAM_GLOBAL_ENB_STATUS_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_oam_gb_enb_id_list_info_len(&p_x2ap_oam_get_sctp_status_req->x2ap_oam_gb_enb_status_list);
    }

    /* Optional element */
    if(p_x2ap_oam_get_sctp_status_req->bitmask & X2AP_OAM_GLOBAL_GNB_STATUS_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_oam_gb_gnb_id_list_info_len(&p_x2ap_oam_get_sctp_status_req->x2ap_oam_gb_gnb_status_list);
    }

    return length;
}

oam_return_et
rrc_il_compose_x2ap_oam_get_sctp_status_req
(
    UInt8  **pp_buffer,
    x2ap_oam_get_sctp_status_req_t *p_x2ap_oam_get_sctp_status_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_get_sctp_status_req != PNULL);

    /* This function composes x2ap_oam_get_sctp_status_req */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_oam_get_sctp_status_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_oam_get_sctp_status_req->bitmask);

    /* Optional element */
    if(p_x2ap_oam_get_sctp_status_req->bitmask & X2AP_OAM_GLOBAL_ENB_STATUS_LIST_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_gb_enb_id_list_info(pp_buffer, &p_x2ap_oam_get_sctp_status_req->x2ap_oam_gb_enb_status_list))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_oam_get_sctp_status_req->bitmask & X2AP_OAM_GLOBAL_GNB_STATUS_LIST_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_gb_gnb_id_list_info(pp_buffer, &p_x2ap_oam_get_sctp_status_req->x2ap_oam_gb_gnb_status_list))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_x2ap_il_send_x2ap_oam_get_sctp_status_req
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_OAM_GET_SCTP_STATUS_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_x2ap_il_send_x2ap_oam_get_sctp_status_req
(
    x2ap_oam_get_sctp_status_req_t  *p_x2ap_oam_get_sctp_status_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_x2apOam_msg;

    OAM_NULL_CHECK(p_x2ap_oam_get_sctp_status_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_oam_get_sctp_status_req_len(p_x2ap_oam_get_sctp_status_req);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):X2AP_OAM_GET_SCTP_STATUS_REQ", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_x2apOam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_x2apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_OAM_GET_SCTP_STATUS_REQ, msg_api_length);

    /* Fill interface header */
    p_x2apOam_msg = p_x2apOam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_x2apOam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, X2AP_OAM_GET_SCTP_STATUS_REQ, msg_length,cell_index);

    /* Fill X2AP_OAM_GET_SCTP_STATUS_REQ message */
    p_x2apOam_msg = p_x2apOam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_get_sctp_status_req(&p_x2apOam_msg, p_x2ap_oam_get_sctp_status_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_x2apOam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_oam_gb_enb_id_list_info_len
(
    x2ap_oam_gb_enb_id_list_info_t *p_x2ap_oam_gb_enb_id_list_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_gb_enb_id_list_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_gb_enb_id_list_info->num_peer_enb);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_enb_id_list_info->num_peer_enb > MAX_PEER_ENB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_enb_id_list_info->num_peer_enb] should be less than"
            " or equal to MAX_PEER_ENB. Incorrect value %d received.", p_x2ap_oam_gb_enb_id_list_info->num_peer_enb);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_oam_gb_enb_id_list_info->num_peer_enb; loop++)
        {
            length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_oam_gb_enb_id_list_info->gb_enb_id[loop]);
        }
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_oam_gb_enb_id_list_info
(
    UInt8  **pp_buffer,
    x2ap_oam_gb_enb_id_list_info_t *p_x2ap_oam_gb_enb_id_list_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_gb_enb_id_list_info != PNULL);

    /* This function composes x2ap_oam_gb_enb_id_list_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_enb_id_list_info->num_peer_enb > MAX_PEER_ENB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_enb_id_list_info->num_peer_enb] should be less than"
            " or equal to MAX_PEER_ENB. Incorrect value %u received.", p_x2ap_oam_gb_enb_id_list_info->num_peer_enb);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_gb_enb_id_list_info->num_peer_enb, "num_peer_enb");
    *pp_buffer += sizeof(p_x2ap_oam_gb_enb_id_list_info->num_peer_enb);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_oam_gb_enb_id_list_info->num_peer_enb; loop++)
        {
            if (OAM_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_oam_gb_enb_id_list_info->gb_enb_id[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_oam_gb_gnb_id_list_info_len
(
    x2ap_oam_gb_gnb_id_list_info_t *p_x2ap_oam_gb_gnb_id_list_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_gb_gnb_id_list_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_gb_gnb_id_list_info->num_peer_gnb);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_gnb_id_list_info->num_peer_gnb > MAX_PEER_GNB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_gnb_id_list_info->num_peer_gnb] should be less than"
            " or equal to MAX_PEER_GNB. Incorrect value %d received.", p_x2ap_oam_gb_gnb_id_list_info->num_peer_gnb);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_oam_gb_gnb_id_list_info->num_peer_gnb; loop++)
        {
            length += rrc_il_get_x2_gb_gnb_id_len(&p_x2ap_oam_gb_gnb_id_list_info->gb_gnb_id[loop]);
        }
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_oam_gb_gnb_id_list_info
(
    UInt8  **pp_buffer,
    x2ap_oam_gb_gnb_id_list_info_t *p_x2ap_oam_gb_gnb_id_list_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_gb_gnb_id_list_info != PNULL);

    /* This function composes x2ap_oam_gb_gnb_id_list_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_gnb_id_list_info->num_peer_gnb > MAX_PEER_GNB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_gnb_id_list_info->num_peer_gnb] should be less than"
            " or equal to MAX_PEER_GNB. Incorrect value %u received.", p_x2ap_oam_gb_gnb_id_list_info->num_peer_gnb);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_gb_gnb_id_list_info->num_peer_gnb, "num_peer_gnb");
    *pp_buffer += sizeof(p_x2ap_oam_gb_gnb_id_list_info->num_peer_gnb);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_oam_gb_gnb_id_list_info->num_peer_gnb; loop++)
        {
            if (OAM_FAILURE == rrc_il_compose_x2_gb_gnb_id(pp_buffer, &p_x2ap_oam_gb_gnb_id_list_info->gb_gnb_id[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

rrc_length_t
rrc_il_get_x2ap_oam_get_sctp_status_resp_len
(
    x2ap_oam_get_sctp_status_resp_t *p_x2ap_oam_get_sctp_status_resp
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_get_sctp_status_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_get_sctp_status_resp->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_get_sctp_status_resp->response);

    /* Optional element */
    if(p_x2ap_oam_get_sctp_status_resp->bitmask & X2AP_OAM_GLOBAL_ENB_STATUS_PARAMS_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_oam_gb_enb_status_params_list_info_len(&p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list);
    }

    /* Optional element */
    if(p_x2ap_oam_get_sctp_status_resp->bitmask & X2AP_OAM_GLOBAL_ENB_FAILED_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_oam_gb_enb_failed_list_len(&p_x2ap_oam_get_sctp_status_resp->gb_enb_failed_list);
    }

    /* Optional element */
    if(p_x2ap_oam_get_sctp_status_resp->bitmask & X2AP_OAM_GLOBAL_GNB_STATUS_PARAMS_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_oam_gb_gnb_status_params_list_info_len(&p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list);
    }

    /* Optional element */
    if(p_x2ap_oam_get_sctp_status_resp->bitmask & X2AP_OAM_GLOBAL_GNB_FAILED_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_x2ap_oam_gb_gnb_failed_list_len(&p_x2ap_oam_get_sctp_status_resp->gb_gnb_failed_list);
    }

    return length;
}

oam_return_et
rrc_il_compose_x2ap_oam_get_sctp_status_resp
(
    UInt8  **pp_buffer,
    x2ap_oam_get_sctp_status_resp_t *p_x2ap_oam_get_sctp_status_resp
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_get_sctp_status_resp != PNULL);

    /* This function composes x2ap_oam_get_sctp_status_resp */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_x2ap_oam_get_sctp_status_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2ap_oam_get_sctp_status_resp->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_get_sctp_status_resp->response > 2))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_get_sctp_status_resp->response] should be less than"
            " or equal to 2. Incorrect value %u received.", p_x2ap_oam_get_sctp_status_resp->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_get_sctp_status_resp->response, "response");
    *pp_buffer += sizeof(p_x2ap_oam_get_sctp_status_resp->response);

    /* Optional element */
    if(p_x2ap_oam_get_sctp_status_resp->bitmask & X2AP_OAM_GLOBAL_ENB_STATUS_PARAMS_LIST_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_gb_enb_status_params_list_info(pp_buffer, &p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_oam_get_sctp_status_resp->bitmask & X2AP_OAM_GLOBAL_ENB_FAILED_LIST_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_gb_enb_failed_list(pp_buffer, &p_x2ap_oam_get_sctp_status_resp->gb_enb_failed_list))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_oam_get_sctp_status_resp->bitmask & X2AP_OAM_GLOBAL_GNB_STATUS_PARAMS_LIST_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_gb_gnb_status_params_list_info(pp_buffer, &p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list))
    {
        return OAM_FAILURE;
    }
    }

    /* Optional element */
    if(p_x2ap_oam_get_sctp_status_resp->bitmask & X2AP_OAM_GLOBAL_GNB_FAILED_LIST_PRESENT)
    {

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_gb_gnb_failed_list(pp_buffer, &p_x2ap_oam_get_sctp_status_resp->gb_gnb_failed_list))
    {
        return OAM_FAILURE;
    }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_x2ap_il_send_x2ap_oam_get_sctp_status_resp
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_OAM_GET_SCTP_STATUS_RESP message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_x2ap_il_send_x2ap_oam_get_sctp_status_resp
(
    x2ap_oam_get_sctp_status_resp_t  *p_x2ap_oam_get_sctp_status_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dst_module_id,  /* Destination module identifier */
    UInt16                 transaction_id, /* Interface transaction identifier */
    UInt8                  cell_index      /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_x2apOam_msg;

    OAM_NULL_CHECK(p_x2ap_oam_get_sctp_status_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_oam_get_sctp_status_resp_len(p_x2ap_oam_get_sctp_status_resp);
    
    if(msg_length == OAM_FAILURE)
        {
            return OAM_FAILURE;
    }
    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):X2AP_OAM_GET_SCTP_STATUS_RESP", src_module_id, dst_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed."
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_x2apOam_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_x2apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_OAM_GET_SCTP_STATUS_RESP, msg_api_length);

    /* Fill interface header */
    p_x2apOam_msg = p_x2apOam_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_x2apOam_msg, transaction_id, src_module_id,
        RRC_MODULE_ID, X2AP_OAM_GET_SCTP_STATUS_RESP, msg_length,cell_index);

    /* Fill X2AP_OAM_GET_SCTP_STATUS_RESP message */
    p_x2apOam_msg = p_x2apOam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == rrc_il_compose_x2ap_oam_get_sctp_status_resp(&p_x2apOam_msg, p_x2ap_oam_get_sctp_status_resp))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_x2apOam_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dst_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_timeval_len
(
    rrc_timeval_t *p_rrc_timeval
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_rrc_timeval != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_timeval->tv_sec);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_timeval->tv_usec);

    return length;
}

static
oam_return_et
rrc_il_compose_rrc_timeval
(
    UInt8  **pp_buffer,
    rrc_timeval_t *p_rrc_timeval
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_rrc_timeval != PNULL);

    /* This function composes rrc_timeval */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_timeval->tv_sec, "tv_sec");
    *pp_buffer += sizeof(p_rrc_timeval->tv_sec);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_rrc_timeval->tv_usec, "tv_usec");
    *pp_buffer += sizeof(p_rrc_timeval->tv_usec);

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_oam_gb_enb_id_status_params_len
(
    x2ap_oam_gb_enb_id_status_params_t *p_x2ap_oam_gb_enb_id_status_params
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_gb_enb_id_status_params != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_oam_gb_enb_id_status_params->global_enb_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_gb_enb_id_status_params->status);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_gb_enb_id_status_params->in_streams);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_gb_enb_id_status_params->out_streams);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_gb_enb_id_status_params->local_port);

    /* Get length of IE */
    length += rrc_il_get_rrc_timeval_len(&p_x2ap_oam_gb_enb_id_status_params->start_time);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_oam_gb_enb_id_status_params->primary_peer_addr);

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_oam_gb_enb_id_status_params
(
    UInt8  **pp_buffer,
    x2ap_oam_gb_enb_id_status_params_t *p_x2ap_oam_gb_enb_id_status_params
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_gb_enb_id_status_params != PNULL);

    /* This function composes x2ap_oam_gb_enb_id_status_params */
    

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_oam_gb_enb_id_status_params->global_enb_id))
    {
        return OAM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_enb_id_status_params->status > 9))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_enb_id_status_params->status] should be less than"
            " or equal to 9. Incorrect value %u received.", p_x2ap_oam_gb_enb_id_status_params->status);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_oam_gb_enb_id_status_params->status, "status");
    *pp_buffer += sizeof(p_x2ap_oam_gb_enb_id_status_params->status);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_oam_gb_enb_id_status_params->in_streams, "in_streams");
    *pp_buffer += sizeof(p_x2ap_oam_gb_enb_id_status_params->in_streams);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_oam_gb_enb_id_status_params->out_streams, "out_streams");
    *pp_buffer += sizeof(p_x2ap_oam_gb_enb_id_status_params->out_streams);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_oam_gb_enb_id_status_params->local_port, "local_port");
    *pp_buffer += sizeof(p_x2ap_oam_gb_enb_id_status_params->local_port);

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_rrc_timeval(pp_buffer, &p_x2ap_oam_gb_enb_id_status_params->start_time))
    {
        return OAM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_oam_gb_enb_id_status_params->primary_peer_addr); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_gb_enb_id_status_params->primary_peer_addr[loop], "primary_peer_addr[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_oam_gb_enb_status_params_list_info_len
(
    x2ap_oam_gb_enb_status_params_list_info_t *p_x2ap_oam_gb_enb_status_params_list_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_gb_enb_status_params_list_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_gb_enb_status_params_list_info->num_peer_enb);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_enb_status_params_list_info->num_peer_enb > MAX_PEER_ENB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_enb_status_params_list_info->num_peer_enb] should be less than"
            " or equal to MAX_PEER_ENB. Incorrect value %d received.", p_x2ap_oam_gb_enb_status_params_list_info->num_peer_enb);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_oam_gb_enb_status_params_list_info->num_peer_enb; loop++)
        {
            length += rrc_il_get_x2ap_oam_gb_enb_id_status_params_len(&p_x2ap_oam_gb_enb_status_params_list_info->staus_params[loop]);
        }
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_oam_gb_enb_status_params_list_info
(
    UInt8  **pp_buffer,
    x2ap_oam_gb_enb_status_params_list_info_t *p_x2ap_oam_gb_enb_status_params_list_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_gb_enb_status_params_list_info != PNULL);

    /* This function composes x2ap_oam_gb_enb_status_params_list_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_enb_status_params_list_info->num_peer_enb > MAX_PEER_ENB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_enb_status_params_list_info->num_peer_enb] should be less than"
            " or equal to MAX_PEER_ENB. Incorrect value %u received.", p_x2ap_oam_gb_enb_status_params_list_info->num_peer_enb);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_gb_enb_status_params_list_info->num_peer_enb, "num_peer_enb");
    *pp_buffer += sizeof(p_x2ap_oam_gb_enb_status_params_list_info->num_peer_enb);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_oam_gb_enb_status_params_list_info->num_peer_enb; loop++)
        {
            if (OAM_FAILURE == rrc_il_compose_x2ap_oam_gb_enb_id_status_params(pp_buffer, &p_x2ap_oam_gb_enb_status_params_list_info->staus_params[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_oam_failed_gb_enb_info_len
(
    x2ap_oam_failed_gb_enb_info_t *p_x2ap_oam_failed_gb_enb_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_failed_gb_enb_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_failed_gb_enb_info->cause);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_enb_id_len(&p_x2ap_oam_failed_gb_enb_info->global_enb_id);

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_oam_failed_gb_enb_info
(
    UInt8  **pp_buffer,
    x2ap_oam_failed_gb_enb_info_t *p_x2ap_oam_failed_gb_enb_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_failed_gb_enb_info != PNULL);

    /* This function composes x2ap_oam_failed_gb_enb_info */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_failed_gb_enb_info->cause, "cause");
    *pp_buffer += sizeof(p_x2ap_oam_failed_gb_enb_info->cause);

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2_gb_enb_id(pp_buffer, &p_x2ap_oam_failed_gb_enb_info->global_enb_id))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_oam_gb_enb_failed_list_len
(
    x2ap_oam_gb_enb_failed_list_t *p_x2ap_oam_gb_enb_failed_list
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_gb_enb_failed_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_gb_enb_failed_list->count);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_enb_failed_list->count > MAX_PEER_ENB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_enb_failed_list->count] should be less than"
            " or equal to MAX_PEER_ENB. Incorrect value %d received.", p_x2ap_oam_gb_enb_failed_list->count);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_oam_gb_enb_failed_list->count; loop++)
        {
            length += rrc_il_get_x2ap_oam_failed_gb_enb_info_len(&p_x2ap_oam_gb_enb_failed_list->failed_global_enb_info[loop]);
        }
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_oam_gb_enb_failed_list
(
    UInt8  **pp_buffer,
    x2ap_oam_gb_enb_failed_list_t *p_x2ap_oam_gb_enb_failed_list
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_gb_enb_failed_list != PNULL);

    /* This function composes x2ap_oam_gb_enb_failed_list */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_enb_failed_list->count > MAX_PEER_ENB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_enb_failed_list->count] should be less than"
            " or equal to MAX_PEER_ENB. Incorrect value %u received.", p_x2ap_oam_gb_enb_failed_list->count);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_gb_enb_failed_list->count, "count");
    *pp_buffer += sizeof(p_x2ap_oam_gb_enb_failed_list->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_oam_gb_enb_failed_list->count; loop++)
        {
            if (OAM_FAILURE == rrc_il_compose_x2ap_oam_failed_gb_enb_info(pp_buffer, &p_x2ap_oam_gb_enb_failed_list->failed_global_enb_info[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_oam_gb_gnb_id_status_params_len
(
    x2ap_oam_gb_gnb_id_status_params_t *p_x2ap_oam_gb_gnb_id_status_params
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_gb_gnb_id_status_params != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_gnb_id_len(&p_x2ap_oam_gb_gnb_id_status_params->global_gnb_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_gb_gnb_id_status_params->status);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_gb_gnb_id_status_params->in_streams);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_gb_gnb_id_status_params->out_streams);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_gb_gnb_id_status_params->local_port);

    /* Get length of IE */
    length += rrc_il_get_rrc_timeval_len(&p_x2ap_oam_gb_gnb_id_status_params->start_time);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_x2ap_oam_gb_gnb_id_status_params->primary_peer_addr);

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_oam_gb_gnb_id_status_params
(
    UInt8  **pp_buffer,
    x2ap_oam_gb_gnb_id_status_params_t *p_x2ap_oam_gb_gnb_id_status_params
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_gb_gnb_id_status_params != PNULL);

    /* This function composes x2ap_oam_gb_gnb_id_status_params */
    

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2_gb_gnb_id(pp_buffer, &p_x2ap_oam_gb_gnb_id_status_params->global_gnb_id))
    {
        return OAM_FAILURE;
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_gnb_id_status_params->status > 9))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_gnb_id_status_params->status] should be less than"
            " or equal to 9. Incorrect value %u received.", p_x2ap_oam_gb_gnb_id_status_params->status);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_oam_gb_gnb_id_status_params->status, "status");
    *pp_buffer += sizeof(p_x2ap_oam_gb_gnb_id_status_params->status);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_oam_gb_gnb_id_status_params->in_streams, "in_streams");
    *pp_buffer += sizeof(p_x2ap_oam_gb_gnb_id_status_params->in_streams);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_oam_gb_gnb_id_status_params->out_streams, "out_streams");
    *pp_buffer += sizeof(p_x2ap_oam_gb_gnb_id_status_params->out_streams);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_x2ap_oam_gb_gnb_id_status_params->local_port, "local_port");
    *pp_buffer += sizeof(p_x2ap_oam_gb_gnb_id_status_params->local_port);

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_rrc_timeval(pp_buffer, &p_x2ap_oam_gb_gnb_id_status_params->start_time))
    {
        return OAM_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_x2ap_oam_gb_gnb_id_status_params->primary_peer_addr); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_gb_gnb_id_status_params->primary_peer_addr[loop], "primary_peer_addr[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_oam_gb_gnb_status_params_list_info_len
(
    x2ap_oam_gb_gnb_status_params_list_info_t *p_x2ap_oam_gb_gnb_status_params_list_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_gb_gnb_status_params_list_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_gb_gnb_status_params_list_info->num_peer_gnb);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_gnb_status_params_list_info->num_peer_gnb > MAX_PEER_GNB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_gnb_status_params_list_info->num_peer_gnb] should be less than"
            " or equal to MAX_PEER_GNB. Incorrect value %d received.", p_x2ap_oam_gb_gnb_status_params_list_info->num_peer_gnb);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_oam_gb_gnb_status_params_list_info->num_peer_gnb; loop++)
        {
            length += rrc_il_get_x2ap_oam_gb_gnb_id_status_params_len(&p_x2ap_oam_gb_gnb_status_params_list_info->staus_params[loop]);
        }
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_oam_gb_gnb_status_params_list_info
(
    UInt8  **pp_buffer,
    x2ap_oam_gb_gnb_status_params_list_info_t *p_x2ap_oam_gb_gnb_status_params_list_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_gb_gnb_status_params_list_info != PNULL);

    /* This function composes x2ap_oam_gb_gnb_status_params_list_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_gnb_status_params_list_info->num_peer_gnb > MAX_PEER_GNB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_gnb_status_params_list_info->num_peer_gnb] should be less than"
            " or equal to MAX_PEER_GNB. Incorrect value %u received.", p_x2ap_oam_gb_gnb_status_params_list_info->num_peer_gnb);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_gb_gnb_status_params_list_info->num_peer_gnb, "num_peer_gnb");
    *pp_buffer += sizeof(p_x2ap_oam_gb_gnb_status_params_list_info->num_peer_gnb);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_oam_gb_gnb_status_params_list_info->num_peer_gnb; loop++)
        {
            if (OAM_FAILURE == rrc_il_compose_x2ap_oam_gb_gnb_id_status_params(pp_buffer, &p_x2ap_oam_gb_gnb_status_params_list_info->staus_params[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_oam_failed_gb_gnb_info_len
(
    x2ap_oam_failed_gb_gnb_info_t *p_x2ap_oam_failed_gb_gnb_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_failed_gb_gnb_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_failed_gb_gnb_info->cause);

    /* Get length of IE */
    length += rrc_il_get_x2_gb_gnb_id_len(&p_x2ap_oam_failed_gb_gnb_info->global_gnb_id);

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_oam_failed_gb_gnb_info
(
    UInt8  **pp_buffer,
    x2ap_oam_failed_gb_gnb_info_t *p_x2ap_oam_failed_gb_gnb_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_failed_gb_gnb_info != PNULL);

    /* This function composes x2ap_oam_failed_gb_gnb_info */
    

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_failed_gb_gnb_info->cause, "cause");
    *pp_buffer += sizeof(p_x2ap_oam_failed_gb_gnb_info->cause);

    /* Compose IE */
    if (OAM_FAILURE == rrc_il_compose_x2_gb_gnb_id(pp_buffer, &p_x2ap_oam_failed_gb_gnb_info->global_gnb_id))
    {
        return OAM_FAILURE;
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_il_get_x2ap_oam_gb_gnb_failed_list_len
(
    x2ap_oam_gb_gnb_failed_list_t *p_x2ap_oam_gb_gnb_failed_list
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_x2ap_oam_gb_gnb_failed_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2ap_oam_gb_gnb_failed_list->count);

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_gnb_failed_list->count > MAX_PEER_GNB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_gnb_failed_list->count] should be less than"
            " or equal to MAX_PEER_GNB. Incorrect value %d received.", p_x2ap_oam_gb_gnb_failed_list->count);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_oam_gb_gnb_failed_list->count; loop++)
        {
            length += rrc_il_get_x2ap_oam_failed_gb_gnb_info_len(&p_x2ap_oam_gb_gnb_failed_list->failed_global_gnb_info[loop]);
        }
    }

    return length;
}

static
oam_return_et
rrc_il_compose_x2ap_oam_gb_gnb_failed_list
(
    UInt8  **pp_buffer,
    x2ap_oam_gb_gnb_failed_list_t *p_x2ap_oam_gb_gnb_failed_list
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_x2ap_oam_gb_gnb_failed_list != PNULL);

    /* This function composes x2ap_oam_gb_gnb_failed_list */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2ap_oam_gb_gnb_failed_list->count > MAX_PEER_GNB))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_x2ap_oam_gb_gnb_failed_list->count] should be less than"
            " or equal to MAX_PEER_GNB. Incorrect value %u received.", p_x2ap_oam_gb_gnb_failed_list->count);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_x2ap_oam_gb_gnb_failed_list->count, "count");
    *pp_buffer += sizeof(p_x2ap_oam_gb_gnb_failed_list->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_x2ap_oam_gb_gnb_failed_list->count; loop++)
        {
            if (OAM_FAILURE == rrc_il_compose_x2ap_oam_failed_gb_gnb_info(pp_buffer, &p_x2ap_oam_gb_gnb_failed_list->failed_global_gnb_info[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

