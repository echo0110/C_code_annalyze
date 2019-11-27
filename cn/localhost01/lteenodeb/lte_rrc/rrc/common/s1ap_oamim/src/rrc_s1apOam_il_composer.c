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
 *  File Description : The file rrc_s1apOam_il_composer.c contains the definitions 
 *                     of rrc-s1apOam interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifdef _MSC_VER
#include "pc_test.h"
#endif
#include "rrc_cp_common.h"
#include "rrc_s1apOam_il_composer.h"
#include "rrc_s1apOam_intf.h"
#include "rrc_ext_api.h"
#ifndef _MSC_VER
#include "rrc_common_utils.h"
#endif
#include "rrc_msg_mgmt.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_length_t
rrc_il_get_s1ap_plmn_identity_len
(
    s1ap_plmn_identity_t *p_s1ap_plmn_identity
);

static
rrc_return_et
rrc_il_compose_s1ap_plmn_identity
(
    U8  **pp_buffer,
    s1ap_plmn_identity_t *p_s1ap_plmn_identity
);

static
rrc_length_t
rrc_il_get_bc_plmn_list_len
(
    bc_plmn_list_t *p_bc_plmn_list
);

static
rrc_return_et
rrc_il_compose_bc_plmn_list
(
    U8  **pp_buffer,
    bc_plmn_list_t *p_bc_plmn_list
);

static
rrc_length_t
rrc_il_get_supp_ta_len
(
    supp_ta_t *p_supp_ta
);

static
rrc_return_et
rrc_il_compose_supp_ta
(
    U8  **pp_buffer,
    supp_ta_t *p_supp_ta
);

static
rrc_length_t
rrc_il_get_supp_ta_list_len
(
    supp_ta_list_t *p_supp_ta_list
);

static
rrc_return_et
rrc_il_compose_supp_ta_list
(
    U8  **pp_buffer,
    supp_ta_list_t *p_supp_ta_list
);

static
rrc_length_t
rrc_il_get_csg_id_info_len
(
    csg_id_info_t *p_csg_id_info
);

static
rrc_return_et
rrc_il_compose_csg_id_info
(
    U8  **pp_buffer,
    csg_id_info_t *p_csg_id_info
);

static
rrc_length_t
rrc_il_get_csg_id_list_len
(
    csg_id_list_t *p_csg_id_list
);

static
rrc_return_et
rrc_il_compose_csg_id_list
(
    U8  **pp_buffer,
    csg_id_list_t *p_csg_id_list
);

static
rrc_length_t
rrc_il_get_s1ap_oam_mme_id_list_info_len
(
    s1ap_oam_mme_id_list_info_t *p_s1ap_oam_mme_id_list_info
);

static
rrc_return_et
rrc_il_compose_s1ap_oam_mme_id_list_info
(
    U8  **pp_buffer,
    s1ap_oam_mme_id_list_info_t *p_s1ap_oam_mme_id_list_info
);

static
rrc_length_t
rrc_il_get_s1ap_oam_mme_conn_info_len
(
    s1ap_oam_mme_conn_info_t *p_s1ap_oam_mme_conn_info
);

static
rrc_return_et
rrc_il_compose_s1ap_oam_mme_conn_info
(
    U8  **pp_buffer,
    s1ap_oam_mme_conn_info_t *p_s1ap_oam_mme_conn_info
);

static
rrc_length_t
rrc_il_get_ip_addr_len
(
    ip_addr_t *p_ip_addr
);

static
rrc_return_et
rrc_il_compose_ip_addr
(
    U8  **pp_buffer,
    ip_addr_t *p_ip_addr
);

static
rrc_length_t
rrc_il_get_ipv6_addr_len
(
    ipv6_addr_t *p_ipv6_addr
);

static
rrc_return_et
rrc_il_compose_ipv6_addr
(
    U8  **pp_buffer,
    ipv6_addr_t *p_ipv6_addr
);

static
rrc_length_t
rrc_il_get_mme_comm_info_len
(
    mme_comm_info_t *p_mme_comm_info
);

static
rrc_return_et
rrc_il_compose_mme_comm_info
(
    U8  **pp_buffer,
    mme_comm_info_t *p_mme_comm_info
);

static
rrc_length_t
rrc_il_get_enb_comm_info_len
(
    enb_comm_info_t *p_enb_comm_info
);

static
rrc_return_et
rrc_il_compose_enb_comm_info
(
    U8  **pp_buffer,
    enb_comm_info_t *p_enb_comm_info
);

static
rrc_length_t
rrc_il_get_macro_enb_id_len
(
    macro_enb_id_t *p_macro_enb_id
);

static
rrc_return_et
rrc_il_compose_macro_enb_id
(
    U8  **pp_buffer,
    macro_enb_id_t *p_macro_enb_id
);

static
rrc_length_t
rrc_il_get_home_enb_id_len
(
    home_enb_id_t *p_home_enb_id
);

static
rrc_return_et
rrc_il_compose_home_enb_id
(
    U8  **pp_buffer,
    home_enb_id_t *p_home_enb_id
);

static
rrc_length_t
rrc_il_get_enb_id_info_len
(
    enb_id_info_t *p_enb_id_info
);

static
rrc_return_et
rrc_il_compose_enb_id_info
(
    U8  **pp_buffer,
    enb_id_info_t *p_enb_id_info
);

static
rrc_length_t
rrc_il_get_gb_enb_id_len
(
    gb_enb_id_t *p_gb_enb_id
);

static
rrc_return_et
rrc_il_compose_gb_enb_id
(
    U8  **pp_buffer,
    gb_enb_id_t *p_gb_enb_id
);

static
rrc_length_t
rrc_il_get_s1_setup_req_len
(
    s1_setup_req_t *p_s1_setup_req
);

static
rrc_return_et
rrc_il_compose_s1_setup_req
(
    U8  **pp_buffer,
    s1_setup_req_t *p_s1_setup_req
);

static
rrc_length_t
rrc_il_get_s1ap_sctp_conf_info_len
(
    s1ap_sctp_conf_info_t *p_s1ap_sctp_conf_info
);

static
rrc_return_et
rrc_il_compose_s1ap_sctp_conf_info
(
    U8  **pp_buffer,
    s1ap_sctp_conf_info_t *p_s1ap_sctp_conf_info
);

static
rrc_length_t
rrc_il_get_s1apInfo_len
(
    s1apInfo_t *p_s1apInfo
);

static
rrc_return_et
rrc_il_compose_s1apInfo
(
    U8  **pp_buffer,
    s1apInfo_t *p_s1apInfo
);

static
rrc_length_t
rrc_il_get_active_mme_ctx_len
(
    active_mme_ctx_t *p_active_mme_ctx
);

static
rrc_return_et
rrc_il_compose_active_mme_ctx
(
    U8  **pp_buffer,
    active_mme_ctx_t *p_active_mme_ctx
);

static
rrc_length_t
rrc_il_get_s1ap_served_group_id_len
(
    s1ap_served_group_id_t *p_s1ap_served_group_id
);

static
rrc_return_et
rrc_il_compose_s1ap_served_group_id
(
    U8  **pp_buffer,
    s1ap_served_group_id_t *p_s1ap_served_group_id
);

static
rrc_length_t
rrc_il_get_s1ap_served_mmec_len
(
    s1ap_served_mmec_t *p_s1ap_served_mmec
);

static
rrc_return_et
rrc_il_compose_s1ap_served_mmec
(
    U8  **pp_buffer,
    s1ap_served_mmec_t *p_s1ap_served_mmec
);

static
rrc_length_t
rrc_il_get_s1ap_served_gummei_info_len
(
    s1ap_served_gummei_info_t *p_s1ap_served_gummei_info
);

static
rrc_return_et
rrc_il_compose_s1ap_served_gummei_info
(
    U8  **pp_buffer,
    s1ap_served_gummei_info_t *p_s1ap_served_gummei_info
);

static
rrc_length_t
rrc_il_get_s1ap_mme_info_len
(
    s1ap_mme_info_t *p_s1ap_mme_info
);

static
rrc_return_et
rrc_il_compose_s1ap_mme_info
(
    U8  **pp_buffer,
    s1ap_mme_info_t *p_s1ap_mme_info
);

static
rrc_length_t
rrc_il_get_rrc_timeval_len
(
    rrc_timeval_t *p_rrc_timeval
);

static
rrc_return_et
rrc_il_compose_rrc_timeval
(
    U8  **pp_buffer,
    rrc_timeval_t *p_rrc_timeval
);

static
rrc_length_t
rrc_il_get_s1ap_oam_mme_id_status_params_len
(
    s1ap_oam_mme_id_status_params_t *p_s1ap_oam_mme_id_status_params
);

static
rrc_return_et
rrc_il_compose_s1ap_oam_mme_id_status_params
(
    U8  **pp_buffer,
    s1ap_oam_mme_id_status_params_t *p_s1ap_oam_mme_id_status_params
);

static
rrc_length_t
rrc_il_get_s1ap_oam_mme_status_params_list_info_len
(
    s1ap_oam_mme_status_params_list_info_t *p_s1ap_oam_mme_status_params_list_info
);

static
rrc_return_et
rrc_il_compose_s1ap_oam_mme_status_params_list_info
(
    U8  **pp_buffer,
    s1ap_oam_mme_status_params_list_info_t *p_s1ap_oam_mme_status_params_list_info
);

static
rrc_length_t
rrc_il_get_s1ap_oam_failed_mme_info_len
(
    s1ap_oam_failed_mme_info_t *p_s1ap_oam_failed_mme_info
);

static
rrc_return_et
rrc_il_compose_s1ap_oam_failed_mme_info
(
    U8  **pp_buffer,
    s1ap_oam_failed_mme_info_t *p_s1ap_oam_failed_mme_info
);

static
rrc_length_t
rrc_il_get_s1ap_oam_mme_failed_list_len
(
    s1ap_oam_mme_failed_list_t *p_s1ap_oam_mme_failed_list
);

static
rrc_return_et
rrc_il_compose_s1ap_oam_mme_failed_list
(
    U8  **pp_buffer,
    s1ap_oam_mme_failed_list_t *p_s1ap_oam_mme_failed_list
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_length_t
rrc_il_get_s1ap_oam_enb_config_update_len
(
    s1ap_oam_enb_config_update_t *p_s1ap_oam_enb_config_update
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_enb_config_update != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_enb_config_update->bitmask);

    /* Optional element */
    if(p_s1ap_oam_enb_config_update->bitmask & S1AP_OAM_ENB_CONFIG_UPDATE_ENB_NAME_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_oam_enb_config_update->enb_name);
    }

    /* Optional element */
    if(p_s1ap_oam_enb_config_update->bitmask & S1AP_OAM_ENB_CONFIG_UPDATE_TA_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_supp_ta_list_len(&p_s1ap_oam_enb_config_update->supp_ta_list);
    }

    /* Optional element */
    if(p_s1ap_oam_enb_config_update->bitmask & S1AP_OAM_ENB_CONFIG_UPDATE_CSG_ID_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_csg_id_list_len(&p_s1ap_oam_enb_config_update->csg_id_list);
    }

    /* Optional element */
    if(p_s1ap_oam_enb_config_update->bitmask & S1AP_OAM_ENB_CONFIG_UPDATE_PAGING_DRX_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_enb_config_update->default_paging_drx);
    }

    /* Optional element */
    if(p_s1ap_oam_enb_config_update->bitmask & S1AP_OAM_ENB_CONFIG_UPDATE_MME_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_s1ap_oam_mme_id_list_info_len(&p_s1ap_oam_enb_config_update->s1ap_oam_mme_id_list);
    }

    return length;
}

rrc_return_et
rrc_il_compose_s1ap_oam_enb_config_update
(
    U8  **pp_buffer,
    s1ap_oam_enb_config_update_t *p_s1ap_oam_enb_config_update
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_enb_config_update != PNULL);

    /* This function composes s1ap_oam_enb_config_update */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1ap_oam_enb_config_update->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_oam_enb_config_update->bitmask);

    /* Optional element */
    if(p_s1ap_oam_enb_config_update->bitmask & S1AP_OAM_ENB_CONFIG_UPDATE_ENB_NAME_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_oam_enb_config_update->enb_name); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_enb_config_update->enb_name[loop], "enb_name[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_s1ap_oam_enb_config_update->bitmask & S1AP_OAM_ENB_CONFIG_UPDATE_TA_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_supp_ta_list(pp_buffer, &p_s1ap_oam_enb_config_update->supp_ta_list))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_s1ap_oam_enb_config_update->bitmask & S1AP_OAM_ENB_CONFIG_UPDATE_CSG_ID_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_csg_id_list(pp_buffer, &p_s1ap_oam_enb_config_update->csg_id_list))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_s1ap_oam_enb_config_update->bitmask & S1AP_OAM_ENB_CONFIG_UPDATE_PAGING_DRX_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_enb_config_update->default_paging_drx > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_enb_config_update->default_paging_drx] should be less than"
            " or equal to 3. Incorrect value %u received.", p_s1ap_oam_enb_config_update->default_paging_drx);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_s1ap_oam_enb_config_update->default_paging_drx, "default_paging_drx");
    *pp_buffer += sizeof(p_s1ap_oam_enb_config_update->default_paging_drx);
    }

    /* Optional element */
    if(p_s1ap_oam_enb_config_update->bitmask & S1AP_OAM_ENB_CONFIG_UPDATE_MME_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_mme_id_list_info(pp_buffer, &p_s1ap_oam_enb_config_update->s1ap_oam_mme_id_list))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apOam_il_send_s1ap_oam_enb_config_update
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_OAM_ENB_CONFIG_UPDATE message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_enb_config_update
(
    s1ap_oam_enb_config_update_t  *p_s1ap_oam_enb_config_update,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_oam_enb_config_update != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_oam_enb_config_update_len(p_s1ap_oam_enb_config_update);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_ENB_CONFIG_UPDATE", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_OAM_ENB_CONFIG_UPDATE, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_OAM_ENB_CONFIG_UPDATE, msg_length,cell_index);

    /* Fill S1AP_OAM_ENB_CONFIG_UPDATE message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_enb_config_update(&p_s1apOam_msg, p_s1ap_oam_enb_config_update))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
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
rrc_il_get_s1ap_plmn_identity_len
(
    s1ap_plmn_identity_t *p_s1ap_plmn_identity
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_plmn_identity != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_plmn_identity->plmn_id);

    return length;
}

static
rrc_return_et
rrc_il_compose_s1ap_plmn_identity
(
    U8  **pp_buffer,
    s1ap_plmn_identity_t *p_s1ap_plmn_identity
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_plmn_identity != PNULL);

    /* This function composes s1ap_plmn_identity */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_plmn_identity->plmn_id); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_s1ap_plmn_identity->plmn_id[loop], "plmn_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_bc_plmn_list_len
(
    bc_plmn_list_t *p_bc_plmn_list
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_bc_plmn_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_bc_plmn_list->num_bplmn);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_bc_plmn_list->num_bplmn < 1) || (p_bc_plmn_list->num_bplmn > MAX_BC_PLMN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_bc_plmn_list->num_bplmn] should be in range "
            "1 to MAX_BC_PLMN. Incorrect value %d received.", p_bc_plmn_list->num_bplmn);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_bc_plmn_list->num_bplmn; loop++)
        {
            length += rrc_il_get_s1ap_plmn_identity_len(&p_bc_plmn_list->plmn_identity[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_bc_plmn_list
(
    U8  **pp_buffer,
    bc_plmn_list_t *p_bc_plmn_list
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_bc_plmn_list != PNULL);

    /* This function composes bc_plmn_list */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_bc_plmn_list->num_bplmn < 1) || (p_bc_plmn_list->num_bplmn > MAX_BC_PLMN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_bc_plmn_list->num_bplmn] should be in range "
            "1 to MAX_BC_PLMN. Incorrect value %u received.", p_bc_plmn_list->num_bplmn);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_bc_plmn_list->num_bplmn, "num_bplmn");
    *pp_buffer += sizeof(p_bc_plmn_list->num_bplmn);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_bc_plmn_list->num_bplmn; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_s1ap_plmn_identity(pp_buffer, &p_bc_plmn_list->plmn_identity[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_supp_ta_len
(
    supp_ta_t *p_supp_ta
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_supp_ta != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_supp_ta->bitmask);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_supp_ta->tac);

    /* Get length of IE */
    length += rrc_il_get_bc_plmn_list_len(&p_supp_ta->bc_plmn_list);

    return length;
}

static
rrc_return_et
rrc_il_compose_supp_ta
(
    U8  **pp_buffer,
    supp_ta_t *p_supp_ta
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_supp_ta != PNULL);

    /* This function composes supp_ta */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_supp_ta->bitmask, "bitmask");
    *pp_buffer += sizeof(p_supp_ta->bitmask);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_supp_ta->tac); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_supp_ta->tac[loop], "tac[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_bc_plmn_list(pp_buffer, &p_supp_ta->bc_plmn_list))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_supp_ta_list_len
(
    supp_ta_list_t *p_supp_ta_list
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_supp_ta_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_supp_ta_list->num_supported_tais);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_supp_ta_list->num_supported_tais < 1) || (p_supp_ta_list->num_supported_tais > MAX_NUM_TAC))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_supp_ta_list->num_supported_tais] should be in range "
            "1 to MAX_NUM_TAC. Incorrect value %d received.", p_supp_ta_list->num_supported_tais);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_supp_ta_list->num_supported_tais; loop++)
        {
            length += rrc_il_get_supp_ta_len(&p_supp_ta_list->supp_tais[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_supp_ta_list
(
    U8  **pp_buffer,
    supp_ta_list_t *p_supp_ta_list
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_supp_ta_list != PNULL);

    /* This function composes supp_ta_list */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_supp_ta_list->num_supported_tais < 1) || (p_supp_ta_list->num_supported_tais > MAX_NUM_TAC))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_supp_ta_list->num_supported_tais] should be in range "
            "1 to MAX_NUM_TAC. Incorrect value %u received.", p_supp_ta_list->num_supported_tais);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_supp_ta_list->num_supported_tais, "num_supported_tais");
    *pp_buffer += sizeof(p_supp_ta_list->num_supported_tais);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_supp_ta_list->num_supported_tais; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_supp_ta(pp_buffer, &p_supp_ta_list->supp_tais[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_csg_id_info_len
(
    csg_id_info_t *p_csg_id_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_csg_id_info != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_csg_id_info->csg_id);

    return length;
}

static
rrc_return_et
rrc_il_compose_csg_id_info
(
    U8  **pp_buffer,
    csg_id_info_t *p_csg_id_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_csg_id_info != PNULL);

    /* This function composes csg_id_info */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_csg_id_info->csg_id); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_csg_id_info->csg_id[loop], "csg_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_csg_id_list_len
(
    csg_id_list_t *p_csg_id_list
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_csg_id_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_csg_id_list->num_csg_ids);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_csg_id_list->num_csg_ids < 1) || (p_csg_id_list->num_csg_ids > MAX_CSG_ID_LIST))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_csg_id_list->num_csg_ids] should be in range "
            "1 to MAX_CSG_ID_LIST. Incorrect value %d received.", p_csg_id_list->num_csg_ids);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_csg_id_list->num_csg_ids; loop++)
        {
            length += rrc_il_get_csg_id_info_len(&p_csg_id_list->csg_ids[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_csg_id_list
(
    U8  **pp_buffer,
    csg_id_list_t *p_csg_id_list
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_csg_id_list != PNULL);

    /* This function composes csg_id_list */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_csg_id_list->num_csg_ids < 1) || (p_csg_id_list->num_csg_ids > MAX_CSG_ID_LIST))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_csg_id_list->num_csg_ids] should be in range "
            "1 to MAX_CSG_ID_LIST. Incorrect value %u received.", p_csg_id_list->num_csg_ids);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_csg_id_list->num_csg_ids, "num_csg_ids");
    *pp_buffer += sizeof(p_csg_id_list->num_csg_ids);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_csg_id_list->num_csg_ids; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_csg_id_info(pp_buffer, &p_csg_id_list->csg_ids[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_s1ap_oam_mme_id_list_info_len
(
    s1ap_oam_mme_id_list_info_t *p_s1ap_oam_mme_id_list_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_mme_id_list_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_mme_id_list_info->num_mme_id);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_s1ap_oam_mme_id_list_info->num_mme_id * sizeof(p_s1ap_oam_mme_id_list_info->mme_id[0]));

    return length;
}

static
rrc_return_et
rrc_il_compose_s1ap_oam_mme_id_list_info
(
    U8  **pp_buffer,
    s1ap_oam_mme_id_list_info_t *p_s1ap_oam_mme_id_list_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_mme_id_list_info != PNULL);

    /* This function composes s1ap_oam_mme_id_list_info */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_oam_mme_id_list_info->num_mme_id < 1) || (p_s1ap_oam_mme_id_list_info->num_mme_id > MAX_NUM_MME))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_mme_id_list_info->num_mme_id] should be in range "
            "1 to MAX_NUM_MME. Incorrect value %u received.", p_s1ap_oam_mme_id_list_info->num_mme_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_mme_id_list_info->num_mme_id, "num_mme_id");
    *pp_buffer += sizeof(p_s1ap_oam_mme_id_list_info->num_mme_id);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_oam_mme_id_list_info->num_mme_id; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_mme_id_list_info->mme_id[loop], "mme_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_s1ap_oam_mme_conn_status_response_len
(
    s1ap_oam_mme_conn_status_response_t *p_s1ap_oam_mme_conn_status_response
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_mme_conn_status_response != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_mme_conn_status_response->mme_count);

    /* Get length of OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_oam_mme_conn_status_response->oam_mme_conn_info_list); loop++)
        {
            length += rrc_il_get_s1ap_oam_mme_conn_info_len(&p_s1ap_oam_mme_conn_status_response->oam_mme_conn_info_list[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_s1ap_oam_mme_conn_status_response
(
    U8  **pp_buffer,
    s1ap_oam_mme_conn_status_response_t *p_s1ap_oam_mme_conn_status_response
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_mme_conn_status_response != PNULL);

    /* This function composes s1ap_oam_mme_conn_status_response */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_mme_conn_status_response->mme_count, "mme_count");
    *pp_buffer += sizeof(p_s1ap_oam_mme_conn_status_response->mme_count);

    /* Compose OCTET_STRING FIXED of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_oam_mme_conn_status_response->oam_mme_conn_info_list); loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_s1ap_oam_mme_conn_info(pp_buffer, &p_s1ap_oam_mme_conn_status_response->oam_mme_conn_info_list[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apOam_il_send_s1ap_oam_mme_conn_status_response
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_OAM_MME_CONN_INFO_RESPONSE message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_mme_conn_status_response
(
    s1ap_oam_mme_conn_status_response_t  *p_s1ap_oam_mme_conn_status_response,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_oam_mme_conn_status_response != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_oam_mme_conn_status_response_len(p_s1ap_oam_mme_conn_status_response);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_MME_CONN_INFO_RESPONSE", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_OAM_MME_CONN_INFO_RESPONSE, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_OAM_MME_CONN_INFO_RESPONSE, msg_length,cell_index);

    /* Fill S1AP_OAM_MME_CONN_INFO_RESPONSE message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_mme_conn_status_response(&p_s1apOam_msg, p_s1ap_oam_mme_conn_status_response))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
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
rrc_il_get_s1ap_oam_mme_conn_info_len
(
    s1ap_oam_mme_conn_info_t *p_s1ap_oam_mme_conn_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_mme_conn_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_mme_conn_info->mme_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_mme_conn_info->response);

    return length;
}

static
rrc_return_et
rrc_il_compose_s1ap_oam_mme_conn_info
(
    U8  **pp_buffer,
    s1ap_oam_mme_conn_info_t *p_s1ap_oam_mme_conn_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_mme_conn_info != PNULL);

    /* This function composes s1ap_oam_mme_conn_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_mme_conn_info->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_mme_conn_info->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_oam_mme_conn_info->mme_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_mme_conn_info->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_oam_mme_conn_info->mme_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_mme_conn_info->response > 4))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_mme_conn_info->response] should be less than"
            " or equal to 4. Incorrect value %u received.", p_s1ap_oam_mme_conn_info->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_mme_conn_info->response, "response");
    *pp_buffer += sizeof(p_s1ap_oam_mme_conn_info->response);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_s1ap_oam_enb_config_update_response_len
(
    s1ap_oam_enb_config_update_response_t *p_s1ap_oam_enb_config_update_response
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_enb_config_update_response != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_enb_config_update_response->mme_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_enb_config_update_response->response);

    return length;
}

rrc_return_et
rrc_il_compose_s1ap_oam_enb_config_update_response
(
    U8  **pp_buffer,
    s1ap_oam_enb_config_update_response_t *p_s1ap_oam_enb_config_update_response
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_enb_config_update_response != PNULL);

    /* This function composes s1ap_oam_enb_config_update_response */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_enb_config_update_response->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_enb_config_update_response->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_oam_enb_config_update_response->mme_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_enb_config_update_response->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_oam_enb_config_update_response->mme_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_enb_config_update_response->response > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_enb_config_update_response->response] should be less than"
            " or equal to 6. Incorrect value %u received.", p_s1ap_oam_enb_config_update_response->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_enb_config_update_response->response, "response");
    *pp_buffer += sizeof(p_s1ap_oam_enb_config_update_response->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apOam_il_send_s1ap_oam_enb_config_update_response
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_OAM_ENB_CONFIG_UPDATE_RESPONSE message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_enb_config_update_response
(
    s1ap_oam_enb_config_update_response_t  *p_s1ap_oam_enb_config_update_response,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_oam_enb_config_update_response != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_oam_enb_config_update_response_len(p_s1ap_oam_enb_config_update_response);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_ENB_CONFIG_UPDATE_RESPONSE", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_OAM_ENB_CONFIG_UPDATE_RESPONSE, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_OAM_ENB_CONFIG_UPDATE_RESPONSE, msg_length,cell_index);

    /* Fill S1AP_OAM_ENB_CONFIG_UPDATE_RESPONSE message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_enb_config_update_response(&p_s1apOam_msg, p_s1ap_oam_enb_config_update_response))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_s1ap_oam_close_mme_conn_len
(
    s1ap_oam_close_mme_conn_t *p_s1ap_oam_close_mme_conn
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_close_mme_conn != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_close_mme_conn->mme_count);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_oam_close_mme_conn->mme_id);

    return length;
}

rrc_return_et
rrc_il_compose_s1ap_oam_close_mme_conn
(
    U8  **pp_buffer,
    s1ap_oam_close_mme_conn_t *p_s1ap_oam_close_mme_conn
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_close_mme_conn != PNULL);

    /* This function composes s1ap_oam_close_mme_conn */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_oam_close_mme_conn->mme_count < 1) || (p_s1ap_oam_close_mme_conn->mme_count > MAX_NUM_MME))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_close_mme_conn->mme_count] should be in range "
            "1 to MAX_NUM_MME. Incorrect value %u received.", p_s1ap_oam_close_mme_conn->mme_count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_close_mme_conn->mme_count, "mme_count");
    *pp_buffer += sizeof(p_s1ap_oam_close_mme_conn->mme_count);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_oam_close_mme_conn->mme_id); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_close_mme_conn->mme_id[loop], "mme_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apOam_il_send_s1ap_oam_close_mme_conn
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_OAM_CLOSE_MME_CONN message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_close_mme_conn
(
    s1ap_oam_close_mme_conn_t  *p_s1ap_oam_close_mme_conn,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_oam_close_mme_conn != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_oam_close_mme_conn_len(p_s1ap_oam_close_mme_conn);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_CLOSE_MME_CONN", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_OAM_CLOSE_MME_CONN, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_OAM_CLOSE_MME_CONN, msg_length,cell_index);

    /* Fill S1AP_OAM_CLOSE_MME_CONN message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_close_mme_conn(&p_s1apOam_msg, p_s1ap_oam_close_mme_conn))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_s1ap_oam_reestab_mme_conn_len
(
    s1ap_oam_reestab_mme_conn_t *p_s1ap_oam_reestab_mme_conn
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_reestab_mme_conn != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_reestab_mme_conn->mme_count);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_oam_reestab_mme_conn->mme_id);

    return length;
}

rrc_return_et
rrc_il_compose_s1ap_oam_reestab_mme_conn
(
    U8  **pp_buffer,
    s1ap_oam_reestab_mme_conn_t *p_s1ap_oam_reestab_mme_conn
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_reestab_mme_conn != PNULL);

    /* This function composes s1ap_oam_reestab_mme_conn */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_oam_reestab_mme_conn->mme_count < 1) || (p_s1ap_oam_reestab_mme_conn->mme_count > MAX_NUM_MME))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_reestab_mme_conn->mme_count] should be in range "
            "1 to MAX_NUM_MME. Incorrect value %u received.", p_s1ap_oam_reestab_mme_conn->mme_count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_reestab_mme_conn->mme_count, "mme_count");
    *pp_buffer += sizeof(p_s1ap_oam_reestab_mme_conn->mme_count);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_oam_reestab_mme_conn->mme_id); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_reestab_mme_conn->mme_id[loop], "mme_id[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apOam_il_send_s1ap_oam_reestab_mme_conn
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_OAM_REESTAB_MME_CONN message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_reestab_mme_conn
(
    s1ap_oam_reestab_mme_conn_t  *p_s1ap_oam_reestab_mme_conn,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_oam_reestab_mme_conn != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_oam_reestab_mme_conn_len(p_s1ap_oam_reestab_mme_conn);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_REESTAB_MME_CONN", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_OAM_REESTAB_MME_CONN, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_OAM_REESTAB_MME_CONN, msg_length,cell_index);

    /* Fill S1AP_OAM_REESTAB_MME_CONN message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_reestab_mme_conn(&p_s1apOam_msg, p_s1ap_oam_reestab_mme_conn))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_s1ap_oam_provision_req_len
(
    s1ap_oam_provision_req_t *p_s1ap_oam_provision_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_provision_req != PNULL);

    /* Get length of IE */
    length += rrc_il_get_s1apInfo_len(&p_s1ap_oam_provision_req->s1apInfo);

    return length;
}

rrc_return_et
rrc_il_compose_s1ap_oam_provision_req
(
    U8  **pp_buffer,
    s1ap_oam_provision_req_t *p_s1ap_oam_provision_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_provision_req != PNULL);

    /* This function composes s1ap_oam_provision_req */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_s1apInfo(pp_buffer, &p_s1ap_oam_provision_req->s1apInfo))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apOam_il_send_s1ap_oam_provision_req
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_OAM_PROVISION_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_provision_req
(
    s1ap_oam_provision_req_t  *p_s1ap_oam_provision_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_oam_provision_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_oam_provision_req_len(p_s1ap_oam_provision_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_PROVISION_REQ", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_OAM_PROVISION_REQ, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_OAM_PROVISION_REQ, msg_length,cell_index);

    /* Fill S1AP_OAM_PROVISION_REQ message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_provision_req(&p_s1apOam_msg, p_s1ap_oam_provision_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
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
rrc_il_get_ip_addr_len
(
    ip_addr_t *p_ip_addr
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_ip_addr != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_ip_addr->ip_addr);

    return length;
}

static
rrc_return_et
rrc_il_compose_ip_addr
(
    U8  **pp_buffer,
    ip_addr_t *p_ip_addr
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_ip_addr != PNULL);

    /* This function composes ip_addr */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_ip_addr->ip_addr); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_ip_addr->ip_addr[loop], "ip_addr[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_ipv6_addr_len
(
    ipv6_addr_t *p_ipv6_addr
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_ipv6_addr != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_ipv6_addr->ipv6_addr);

    return length;
}

static
rrc_return_et
rrc_il_compose_ipv6_addr
(
    U8  **pp_buffer,
    ipv6_addr_t *p_ipv6_addr
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_ipv6_addr != PNULL);

    /* This function composes ipv6_addr */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_ipv6_addr->ipv6_addr); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_ipv6_addr->ipv6_addr[loop], "ipv6_addr[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_mme_comm_info_len
(
    mme_comm_info_t *p_mme_comm_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_mme_comm_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_mme_comm_info->num_ip_addr);

    /* Check for correct range [H - higher boundary] */
    if ((p_mme_comm_info->num_ip_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->num_ip_addr] should be less than"
            " or equal to MAX_NUM_IP_ADDR. Incorrect value %d received.", p_mme_comm_info->num_ip_addr);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_mme_comm_info->num_ip_addr; loop++)
        {
            length += rrc_il_get_ip_addr_len(&p_mme_comm_info->ip_addr[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_mme_comm_info->port);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_mme_comm_info->num_streams);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_mme_comm_info->heart_beat_timer);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_mme_comm_info->rel_of_mme);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_mme_comm_info->bitmask);

    /* Optional element */
    if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_mme_comm_info->num_ipv6_addr);
    }

    /* Optional element */
    if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_mme_comm_info->num_ipv6_addr < 1) || (p_mme_comm_info->num_ipv6_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->num_ipv6_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %d received.", p_mme_comm_info->num_ipv6_addr);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_mme_comm_info->num_ipv6_addr; loop++)
        {
            length += rrc_il_get_ipv6_addr_len(&p_mme_comm_info->ipv6_addr[loop]);
        }
    }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_mme_comm_info
(
    U8  **pp_buffer,
    mme_comm_info_t *p_mme_comm_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_mme_comm_info != PNULL);

    /* This function composes mme_comm_info */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_mme_comm_info->num_ip_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->num_ip_addr] should be less than"
            " or equal to MAX_NUM_IP_ADDR. Incorrect value %u received.", p_mme_comm_info->num_ip_addr);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_mme_comm_info->num_ip_addr, "num_ip_addr");
    *pp_buffer += sizeof(p_mme_comm_info->num_ip_addr);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_mme_comm_info->num_ip_addr; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_ip_addr(pp_buffer, &p_mme_comm_info->ip_addr[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_mme_comm_info->port, "port");
    *pp_buffer += sizeof(p_mme_comm_info->port);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_mme_comm_info->num_streams < 2) || (p_mme_comm_info->num_streams > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->num_streams] should be in range "
            "2 to 10. Incorrect value %u received.", p_mme_comm_info->num_streams);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_mme_comm_info->num_streams, "num_streams");
    *pp_buffer += sizeof(p_mme_comm_info->num_streams);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_mme_comm_info->heart_beat_timer, "heart_beat_timer");
    *pp_buffer += sizeof(p_mme_comm_info->heart_beat_timer);

    /* Check for correct range [H - higher boundary] */
    if ((p_mme_comm_info->rel_of_mme > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->rel_of_mme] should be less than"
            " or equal to 2. Incorrect value %u received.", p_mme_comm_info->rel_of_mme);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_mme_comm_info->rel_of_mme, "rel_of_mme");
    *pp_buffer += sizeof(p_mme_comm_info->rel_of_mme);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_mme_comm_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_mme_comm_info->bitmask);

    /* Optional element */
    if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_mme_comm_info->num_ipv6_addr < 1) || (p_mme_comm_info->num_ipv6_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->num_ipv6_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %u received.", p_mme_comm_info->num_ipv6_addr);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_mme_comm_info->num_ipv6_addr, "num_ipv6_addr");
    *pp_buffer += sizeof(p_mme_comm_info->num_ipv6_addr);
    }

    /* Optional element */
    if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_mme_comm_info->num_ipv6_addr; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_ipv6_addr(pp_buffer, &p_mme_comm_info->ipv6_addr[loop]))
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
rrc_il_get_enb_comm_info_len
(
    enb_comm_info_t *p_enb_comm_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_enb_comm_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_enb_comm_info->bitmask);

    /* Optional element */
    if(p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_enb_comm_info->num_ip_addr);
    }

    /* Optional element */
    if(p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV4_ADDR_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_enb_comm_info->num_ip_addr < 1) || (p_enb_comm_info->num_ip_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_enb_comm_info->num_ip_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %d received.", p_enb_comm_info->num_ip_addr);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_enb_comm_info->num_ip_addr; loop++)
        {
            length += rrc_il_get_ip_addr_len(&p_enb_comm_info->ip_addr[loop]);
        }
    }
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_enb_comm_info->port);

    /* Optional element */
    if(p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_enb_comm_info->num_ipv6_addr);
    }

    /* Optional element */
    if(p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV6_ADDR_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_enb_comm_info->num_ipv6_addr < 1) || (p_enb_comm_info->num_ipv6_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_enb_comm_info->num_ipv6_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %d received.", p_enb_comm_info->num_ipv6_addr);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_enb_comm_info->num_ipv6_addr; loop++)
        {
            length += rrc_il_get_ipv6_addr_len(&p_enb_comm_info->ipv6_addr[loop]);
        }
    }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_enb_comm_info
(
    U8  **pp_buffer,
    enb_comm_info_t *p_enb_comm_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_enb_comm_info != PNULL);

    /* This function composes enb_comm_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_enb_comm_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_enb_comm_info->bitmask);

    /* Optional element */
    if(p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_enb_comm_info->num_ip_addr < 1) || (p_enb_comm_info->num_ip_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_enb_comm_info->num_ip_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %u received.", p_enb_comm_info->num_ip_addr);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_enb_comm_info->num_ip_addr, "num_ip_addr");
    *pp_buffer += sizeof(p_enb_comm_info->num_ip_addr);
    }

    /* Optional element */
    if(p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV4_ADDR_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_enb_comm_info->num_ip_addr; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_ip_addr(pp_buffer, &p_enb_comm_info->ip_addr[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_enb_comm_info->port, "port");
    *pp_buffer += sizeof(p_enb_comm_info->port);

    /* Optional element */
    if(p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_enb_comm_info->num_ipv6_addr < 1) || (p_enb_comm_info->num_ipv6_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_enb_comm_info->num_ipv6_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %u received.", p_enb_comm_info->num_ipv6_addr);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_enb_comm_info->num_ipv6_addr, "num_ipv6_addr");
    *pp_buffer += sizeof(p_enb_comm_info->num_ipv6_addr);
    }

    /* Optional element */
    if(p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV6_ADDR_PRESENT)
    {

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_enb_comm_info->num_ipv6_addr; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_ipv6_addr(pp_buffer, &p_enb_comm_info->ipv6_addr[loop]))
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
rrc_il_get_macro_enb_id_len
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
rrc_il_compose_macro_enb_id
(
    U8  **pp_buffer,
    macro_enb_id_t *p_macro_enb_id
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_macro_enb_id != PNULL);

    /* This function composes macro_enb_id */
    

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
rrc_il_get_home_enb_id_len
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
rrc_il_compose_home_enb_id
(
    U8  **pp_buffer,
    home_enb_id_t *p_home_enb_id
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_home_enb_id != PNULL);

    /* This function composes home_enb_id */
    

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
rrc_il_get_enb_id_info_len
(
    enb_id_info_t *p_enb_id_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_enb_id_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_enb_id_info->presence_bitmask);

    /* Optional element */
    if(p_enb_id_info->presence_bitmask & 1)
    {

    /* Get length of IE */
    length += rrc_il_get_macro_enb_id_len(&p_enb_id_info->macro_enb_id);
    }

    /* Optional element */
    if(p_enb_id_info->presence_bitmask & 2)
    {

    /* Get length of IE */
    length += rrc_il_get_home_enb_id_len(&p_enb_id_info->home_enb_id);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_enb_id_info
(
    U8  **pp_buffer,
    enb_id_info_t *p_enb_id_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_enb_id_info != PNULL);

    /* This function composes enb_id_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_enb_id_info->presence_bitmask, "presence_bitmask");
    *pp_buffer += sizeof(p_enb_id_info->presence_bitmask);

    /* Optional element */
    if(p_enb_id_info->presence_bitmask & 1)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_macro_enb_id(pp_buffer, &p_enb_id_info->macro_enb_id))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_enb_id_info->presence_bitmask & 2)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_home_enb_id(pp_buffer, &p_enb_id_info->home_enb_id))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_gb_enb_id_len
(
    gb_enb_id_t *p_gb_enb_id
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_gb_enb_id != PNULL);

    /* Get length of IE */
    length += rrc_il_get_s1ap_plmn_identity_len(&p_gb_enb_id->plmn_identity);

    /* Get length of IE */
    length += rrc_il_get_enb_id_info_len(&p_gb_enb_id->enb_id);

    return length;
}

static
rrc_return_et
rrc_il_compose_gb_enb_id
(
    U8  **pp_buffer,
    gb_enb_id_t *p_gb_enb_id
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_gb_enb_id != PNULL);

    /* This function composes gb_enb_id */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_s1ap_plmn_identity(pp_buffer, &p_gb_enb_id->plmn_identity))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_enb_id_info(pp_buffer, &p_gb_enb_id->enb_id))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_s1_setup_req_len
(
    s1_setup_req_t *p_s1_setup_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1_setup_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1_setup_req->bitmask);

    /* Get length of IE */
    length += rrc_il_get_gb_enb_id_len(&p_s1_setup_req->gb_enb_id);

    /* Optional element */
    if(p_s1_setup_req->bitmask & 1)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1_setup_req->enb_name);
    }

    /* Get length of IE */
    length += rrc_il_get_supp_ta_list_len(&p_s1_setup_req->supp_ta_list);

    /* Optional element */
    if(p_s1_setup_req->bitmask & S1AP_OAM_CSG_ID_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_csg_id_list_len(&p_s1_setup_req->csg_id_list);
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1_setup_req->default_paging_drx);

    return length;
}

static
rrc_return_et
rrc_il_compose_s1_setup_req
(
    U8  **pp_buffer,
    s1_setup_req_t *p_s1_setup_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1_setup_req != PNULL);

    /* This function composes s1_setup_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1_setup_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1_setup_req->bitmask);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_gb_enb_id(pp_buffer, &p_s1_setup_req->gb_enb_id))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_s1_setup_req->bitmask & 1)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1_setup_req->enb_name); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_s1_setup_req->enb_name[loop], "enb_name[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_supp_ta_list(pp_buffer, &p_s1_setup_req->supp_ta_list))
    {
        return RRC_FAILURE;
    }

    /* Optional element */
    if(p_s1_setup_req->bitmask & S1AP_OAM_CSG_ID_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_csg_id_list(pp_buffer, &p_s1_setup_req->csg_id_list))
    {
        return RRC_FAILURE;
    }
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_s1_setup_req->default_paging_drx > 3))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1_setup_req->default_paging_drx] should be less than"
            " or equal to 3. Incorrect value %u received.", p_s1_setup_req->default_paging_drx);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_s1_setup_req->default_paging_drx, "default_paging_drx");
    *pp_buffer += sizeof(p_s1_setup_req->default_paging_drx);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_s1ap_sctp_conf_info_len
(
    s1ap_sctp_conf_info_t *p_s1ap_sctp_conf_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_sctp_conf_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_sctp_conf_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_sctp_conf_info->init_max_attempts);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_sctp_conf_info->rto_initial);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_sctp_conf_info->rto_max);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_sctp_conf_info->rto_min);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_sctp_conf_info->assoc_max_retrans);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_sctp_conf_info->valid_cookie_life);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_sctp_conf_info->path_max_retrans);

    /* Optional element */
    if(p_s1ap_sctp_conf_info->bitmask & S1AP_OAM_DSCP_VALUE_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_sctp_conf_info->s1ap_dscp_value);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_s1ap_sctp_conf_info
(
    U8  **pp_buffer,
    s1ap_sctp_conf_info_t *p_s1ap_sctp_conf_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_sctp_conf_info != PNULL);

    /* This function composes s1ap_sctp_conf_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1ap_sctp_conf_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_sctp_conf_info->bitmask);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_s1ap_sctp_conf_info->init_max_attempts, "init_max_attempts");
    *pp_buffer += sizeof(p_s1ap_sctp_conf_info->init_max_attempts);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_s1ap_sctp_conf_info->rto_initial, "rto_initial");
    *pp_buffer += sizeof(p_s1ap_sctp_conf_info->rto_initial);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_s1ap_sctp_conf_info->rto_max, "rto_max");
    *pp_buffer += sizeof(p_s1ap_sctp_conf_info->rto_max);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_s1ap_sctp_conf_info->rto_min, "rto_min");
    *pp_buffer += sizeof(p_s1ap_sctp_conf_info->rto_min);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1ap_sctp_conf_info->assoc_max_retrans, "assoc_max_retrans");
    *pp_buffer += sizeof(p_s1ap_sctp_conf_info->assoc_max_retrans);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_s1ap_sctp_conf_info->valid_cookie_life, "valid_cookie_life");
    *pp_buffer += sizeof(p_s1ap_sctp_conf_info->valid_cookie_life);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1ap_sctp_conf_info->path_max_retrans, "path_max_retrans");
    *pp_buffer += sizeof(p_s1ap_sctp_conf_info->path_max_retrans);

    /* Optional element */
    if(p_s1ap_sctp_conf_info->bitmask & S1AP_OAM_DSCP_VALUE_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_sctp_conf_info->s1ap_dscp_value > 63))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_sctp_conf_info->s1ap_dscp_value] should be less than"
            " or equal to 63. Incorrect value %u received.", p_s1ap_sctp_conf_info->s1ap_dscp_value);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_sctp_conf_info->s1ap_dscp_value, "s1ap_dscp_value");
    *pp_buffer += sizeof(p_s1ap_sctp_conf_info->s1ap_dscp_value);
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_s1apInfo_len
(
    s1apInfo_t *p_s1apInfo
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1apInfo != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1apInfo->num_mme);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1apInfo->num_mme < 1) || (p_s1apInfo->num_mme > MAX_NUM_MME))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1apInfo->num_mme] should be in range "
            "1 to MAX_NUM_MME. Incorrect value %d received.", p_s1apInfo->num_mme);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1apInfo->num_mme; loop++)
        {
            length += rrc_il_get_mme_comm_info_len(&p_s1apInfo->mme_comm_info[loop]);
        }
    }

    /* Get length of IE */
    length += rrc_il_get_enb_comm_info_len(&p_s1apInfo->enb_comm_info);

    /* Get length of IE */
    length += rrc_il_get_s1_setup_req_len(&p_s1apInfo->s1_setup_req_parameters);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1apInfo->s1_config_timer);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1apInfo->reset_ep_timer);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1apInfo->max_s1_config_retries);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1apInfo->max_reset_retries);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1apInfo->s1ap_pws_timer);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1apInfo->s1ap_kill_timer);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1apInfo->bitmask);

    /* Optional element */
    if(p_s1apInfo->bitmask & S1AP_OAM_MME_SELECTION_ALGO_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1apInfo->mme_selection_algo);
    }

    /* Optional element */
    if(p_s1apInfo->bitmask & S1AP_OAM_S1_CONN_RECOVERY_DURATION_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1apInfo->s1_conn_recovery_duration);
    }

    /* Optional element */
    if(p_s1apInfo->bitmask & S1AP_OAM_SCTP_CONF_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_s1ap_sctp_conf_info_len(&p_s1apInfo->s1ap_sctp_conf_param);
    }

    /* Optional element */
    if(p_s1apInfo->bitmask & S1AP_OAM_S1_HEALTH_MONITORING_TIMER_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1apInfo->s1ap_health_mon_time_int);
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1apInfo->log_level);

    return length;
}

static
rrc_return_et
rrc_il_compose_s1apInfo
(
    U8  **pp_buffer,
    s1apInfo_t *p_s1apInfo
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1apInfo != PNULL);

    /* This function composes s1apInfo */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1apInfo->num_mme < 1) || (p_s1apInfo->num_mme > MAX_NUM_MME))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1apInfo->num_mme] should be in range "
            "1 to MAX_NUM_MME. Incorrect value %u received.", p_s1apInfo->num_mme);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1apInfo->num_mme, "num_mme");
    *pp_buffer += sizeof(p_s1apInfo->num_mme);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1apInfo->num_mme; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_mme_comm_info(pp_buffer, &p_s1apInfo->mme_comm_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_enb_comm_info(pp_buffer, &p_s1apInfo->enb_comm_info))
    {
        return RRC_FAILURE;
    }

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_s1_setup_req(pp_buffer, &p_s1apInfo->s1_setup_req_parameters))
    {
        return RRC_FAILURE;
    }

    /* Check for correct range [L - lower boundary] */
    if ((p_s1apInfo->s1_config_timer < 50))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1apInfo->s1_config_timer] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_s1apInfo->s1_config_timer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_s1apInfo->s1_config_timer, "s1_config_timer");
    *pp_buffer += sizeof(p_s1apInfo->s1_config_timer);

    /* Check for correct range [L - lower boundary] */
    if ((p_s1apInfo->reset_ep_timer < 50))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1apInfo->reset_ep_timer] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_s1apInfo->reset_ep_timer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_s1apInfo->reset_ep_timer, "reset_ep_timer");
    *pp_buffer += sizeof(p_s1apInfo->reset_ep_timer);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1apInfo->max_s1_config_retries, "max_s1_config_retries");
    *pp_buffer += sizeof(p_s1apInfo->max_s1_config_retries);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1apInfo->max_reset_retries, "max_reset_retries");
    *pp_buffer += sizeof(p_s1apInfo->max_reset_retries);

    /* Check for correct range [L - lower boundary] */
    if ((p_s1apInfo->s1ap_pws_timer < 50))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1apInfo->s1ap_pws_timer] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_s1apInfo->s1ap_pws_timer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_s1apInfo->s1ap_pws_timer, "s1ap_pws_timer");
    *pp_buffer += sizeof(p_s1apInfo->s1ap_pws_timer);

    /* Check for correct range [L - lower boundary] */
    if ((p_s1apInfo->s1ap_kill_timer < 50))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1apInfo->s1ap_kill_timer] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_s1apInfo->s1ap_kill_timer);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_s1apInfo->s1ap_kill_timer, "s1ap_kill_timer");
    *pp_buffer += sizeof(p_s1apInfo->s1ap_kill_timer);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1apInfo->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1apInfo->bitmask);

    /* Optional element */
    if(p_s1apInfo->bitmask & S1AP_OAM_MME_SELECTION_ALGO_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_s1apInfo->mme_selection_algo > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1apInfo->mme_selection_algo] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1apInfo->mme_selection_algo);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1apInfo->mme_selection_algo, "mme_selection_algo");
    *pp_buffer += sizeof(p_s1apInfo->mme_selection_algo);
    }

    /* Optional element */
    if(p_s1apInfo->bitmask & S1AP_OAM_S1_CONN_RECOVERY_DURATION_PRESENT)
    {

    /* Check for correct range [L - lower boundary] */
    if ((p_s1apInfo->s1_conn_recovery_duration < 50))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1apInfo->s1_conn_recovery_duration] should be greater than"
            " or equal to 50. Incorrect value %u received.", p_s1apInfo->s1_conn_recovery_duration);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_s1apInfo->s1_conn_recovery_duration, "s1_conn_recovery_duration");
    *pp_buffer += sizeof(p_s1apInfo->s1_conn_recovery_duration);
    }

    /* Optional element */
    if(p_s1apInfo->bitmask & S1AP_OAM_SCTP_CONF_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_s1ap_sctp_conf_info(pp_buffer, &p_s1apInfo->s1ap_sctp_conf_param))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_s1apInfo->bitmask & S1AP_OAM_S1_HEALTH_MONITORING_TIMER_PRESENT)
    {

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1apInfo->s1ap_health_mon_time_int, "s1ap_health_mon_time_int");
    *pp_buffer += sizeof(p_s1apInfo->s1ap_health_mon_time_int);
    }

    /* Check for correct range [H - higher boundary] */
    if ((p_s1apInfo->log_level > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1apInfo->log_level] should be less than"
            " or equal to 6. Incorrect value %u received.", p_s1apInfo->log_level);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1apInfo->log_level, "log_level");
    *pp_buffer += sizeof(p_s1apInfo->log_level);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_s1ap_oam_provision_resp_len
(
    s1ap_oam_provision_resp_t *p_s1ap_oam_provision_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_provision_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_provision_resp->response);

    return length;
}

rrc_return_et
rrc_il_compose_s1ap_oam_provision_resp
(
    U8  **pp_buffer,
    s1ap_oam_provision_resp_t *p_s1ap_oam_provision_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_provision_resp != PNULL);

    /* This function composes s1ap_oam_provision_resp */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_provision_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_provision_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_oam_provision_resp->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_provision_resp->response, "response");
    *pp_buffer += sizeof(p_s1ap_oam_provision_resp->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apOam_il_send_s1ap_oam_provision_resp
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_OAM_PROVISION_RESP message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_provision_resp
(
    s1ap_oam_provision_resp_t  *p_s1ap_oam_provision_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_oam_provision_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_oam_provision_resp_len(p_s1ap_oam_provision_resp);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_PROVISION_RESP", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_OAM_PROVISION_RESP, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_OAM_PROVISION_RESP, msg_length,cell_index);

    /* Fill S1AP_OAM_PROVISION_RESP message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_provision_resp(&p_s1apOam_msg, p_s1ap_oam_provision_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_s1ap_oam_get_log_level_resp_len
(
    s1ap_oam_get_log_level_resp_t *p_s1ap_oam_get_log_level_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_get_log_level_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_get_log_level_resp->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_get_log_level_resp->log_on_off);

    /* Optional element */
    if(p_s1ap_oam_get_log_level_resp->bitmask & S1AP_OAM_LOG_LEVEL_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_get_log_level_resp->log_level);
    }

    return length;
}

rrc_return_et
rrc_il_compose_s1ap_oam_get_log_level_resp
(
    U8  **pp_buffer,
    s1ap_oam_get_log_level_resp_t *p_s1ap_oam_get_log_level_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_get_log_level_resp != PNULL);

    /* This function composes s1ap_oam_get_log_level_resp */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1ap_oam_get_log_level_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_oam_get_log_level_resp->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_get_log_level_resp->log_on_off > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_get_log_level_resp->log_on_off] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_oam_get_log_level_resp->log_on_off);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_get_log_level_resp->log_on_off, "log_on_off");
    *pp_buffer += sizeof(p_s1ap_oam_get_log_level_resp->log_on_off);

    /* Optional element */
    if(p_s1ap_oam_get_log_level_resp->bitmask & S1AP_OAM_LOG_LEVEL_PRESENT)
    {

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_get_log_level_resp->log_level > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_get_log_level_resp->log_level] should be less than"
            " or equal to 6. Incorrect value %u received.", p_s1ap_oam_get_log_level_resp->log_level);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_get_log_level_resp->log_level, "log_level");
    *pp_buffer += sizeof(p_s1ap_oam_get_log_level_resp->log_level);
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apOam_il_send_s1ap_oam_get_log_level_resp
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_OAM_GET_LOG_LEVEL_RESP message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_get_log_level_resp
(
    s1ap_oam_get_log_level_resp_t  *p_s1ap_oam_get_log_level_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_oam_get_log_level_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_oam_get_log_level_resp_len(p_s1ap_oam_get_log_level_resp);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_GET_LOG_LEVEL_RESP", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_OAM_GET_LOG_LEVEL_RESP, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_OAM_GET_LOG_LEVEL_RESP, msg_length,cell_index);

    /* Fill S1AP_OAM_GET_LOG_LEVEL_RESP message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_get_log_level_resp(&p_s1apOam_msg, p_s1ap_oam_get_log_level_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_s1ap_oam_set_log_level_req_len
(
    s1ap_oam_set_log_level_req_t *p_s1ap_oam_set_log_level_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_set_log_level_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_set_log_level_req->log_level);

    return length;
}

rrc_return_et
rrc_il_compose_s1ap_oam_set_log_level_req
(
    U8  **pp_buffer,
    s1ap_oam_set_log_level_req_t *p_s1ap_oam_set_log_level_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_set_log_level_req != PNULL);

    /* This function composes s1ap_oam_set_log_level_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_set_log_level_req->log_level > 6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_set_log_level_req->log_level] should be less than"
            " or equal to 6. Incorrect value %u received.", p_s1ap_oam_set_log_level_req->log_level);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_set_log_level_req->log_level, "log_level");
    *pp_buffer += sizeof(p_s1ap_oam_set_log_level_req->log_level);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apOam_il_send_s1ap_oam_set_log_level_req
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_OAM_SET_LOG_LEVEL_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_set_log_level_req
(
    s1ap_oam_set_log_level_req_t  *p_s1ap_oam_set_log_level_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_oam_set_log_level_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_oam_set_log_level_req_len(p_s1ap_oam_set_log_level_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_SET_LOG_LEVEL_REQ", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_OAM_SET_LOG_LEVEL_REQ, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_OAM_SET_LOG_LEVEL_REQ, msg_length,cell_index);

    /* Fill S1AP_OAM_SET_LOG_LEVEL_REQ message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_set_log_level_req(&p_s1apOam_msg, p_s1ap_oam_set_log_level_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_s1ap_oam_set_log_level_resp_len
(
    s1ap_oam_set_log_level_resp_t *p_s1ap_oam_set_log_level_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_set_log_level_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_set_log_level_resp->response);

    return length;
}

rrc_return_et
rrc_il_compose_s1ap_oam_set_log_level_resp
(
    U8  **pp_buffer,
    s1ap_oam_set_log_level_resp_t *p_s1ap_oam_set_log_level_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_set_log_level_resp != PNULL);

    /* This function composes s1ap_oam_set_log_level_resp */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_set_log_level_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_set_log_level_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_oam_set_log_level_resp->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_set_log_level_resp->response, "response");
    *pp_buffer += sizeof(p_s1ap_oam_set_log_level_resp->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apOam_il_send_s1ap_oam_set_log_level_resp
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_OAM_SET_LOG_LEVEL_RESP message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_set_log_level_resp
(
    s1ap_oam_set_log_level_resp_t  *p_s1ap_oam_set_log_level_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_oam_set_log_level_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_oam_set_log_level_resp_len(p_s1ap_oam_set_log_level_resp);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_SET_LOG_LEVEL_RESP", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_OAM_SET_LOG_LEVEL_RESP, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_OAM_SET_LOG_LEVEL_RESP, msg_length,cell_index);

    /* Fill S1AP_OAM_SET_LOG_LEVEL_RESP message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_set_log_level_resp(&p_s1apOam_msg, p_s1ap_oam_set_log_level_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_s1ap_oam_log_enable_req_len
(
    s1ap_oam_log_enable_req_t *p_s1ap_oam_log_enable_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_log_enable_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_log_enable_req->log_on_off);

    return length;
}

rrc_return_et
rrc_il_compose_s1ap_oam_log_enable_req
(
    U8  **pp_buffer,
    s1ap_oam_log_enable_req_t *p_s1ap_oam_log_enable_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_log_enable_req != PNULL);

    /* This function composes s1ap_oam_log_enable_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_log_enable_req->log_on_off > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_log_enable_req->log_on_off] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_oam_log_enable_req->log_on_off);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_log_enable_req->log_on_off, "log_on_off");
    *pp_buffer += sizeof(p_s1ap_oam_log_enable_req->log_on_off);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apOam_il_send_s1ap_oam_log_enable_req
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_OAM_LOG_ENABLE_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_log_enable_req
(
    s1ap_oam_log_enable_req_t  *p_s1ap_oam_log_enable_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_oam_log_enable_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_oam_log_enable_req_len(p_s1ap_oam_log_enable_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_LOG_ENABLE_REQ", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_OAM_LOG_ENABLE_REQ, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_OAM_LOG_ENABLE_REQ, msg_length,cell_index);

    /* Fill S1AP_OAM_LOG_ENABLE_REQ message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_log_enable_req(&p_s1apOam_msg, p_s1ap_oam_log_enable_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_s1ap_oam_log_enable_resp_len
(
    s1ap_oam_log_enable_resp_t *p_s1ap_oam_log_enable_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_log_enable_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_log_enable_resp->response);

    return length;
}

rrc_return_et
rrc_il_compose_s1ap_oam_log_enable_resp
(
    U8  **pp_buffer,
    s1ap_oam_log_enable_resp_t *p_s1ap_oam_log_enable_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_log_enable_resp != PNULL);

    /* This function composes s1ap_oam_log_enable_resp */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_log_enable_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_log_enable_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_oam_log_enable_resp->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_log_enable_resp->response, "response");
    *pp_buffer += sizeof(p_s1ap_oam_log_enable_resp->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apOam_il_send_s1ap_oam_log_enable_resp
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_OAM_LOG_ENABLE_RESP message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_log_enable_resp
(
    s1ap_oam_log_enable_resp_t  *p_s1ap_oam_log_enable_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_oam_log_enable_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_oam_log_enable_resp_len(p_s1ap_oam_log_enable_resp);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_LOG_ENABLE_RESP", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_OAM_LOG_ENABLE_RESP, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_OAM_LOG_ENABLE_RESP, msg_length,cell_index);

    /* Fill S1AP_OAM_LOG_ENABLE_RESP message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_log_enable_resp(&p_s1apOam_msg, p_s1ap_oam_log_enable_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_s1ap_oam_reset_resp_len
(
    s1ap_oam_reset_resp_t *p_s1ap_oam_reset_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_reset_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_reset_resp->response);

    return length;
}

rrc_return_et
rrc_il_compose_s1ap_oam_reset_resp
(
    U8  **pp_buffer,
    s1ap_oam_reset_resp_t *p_s1ap_oam_reset_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_reset_resp != PNULL);

    /* This function composes s1ap_oam_reset_resp */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_reset_resp->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_reset_resp->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_oam_reset_resp->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_reset_resp->response, "response");
    *pp_buffer += sizeof(p_s1ap_oam_reset_resp->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apOam_il_send_s1ap_oam_reset_resp
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_OAM_RESET_RESP message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_reset_resp
(
    s1ap_oam_reset_resp_t  *p_s1ap_oam_reset_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_oam_reset_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_oam_reset_resp_len(p_s1ap_oam_reset_resp);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_RESET_RESP", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_OAM_RESET_RESP, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_OAM_RESET_RESP, msg_length,cell_index);

    /* Fill S1AP_OAM_RESET_RESP message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_reset_resp(&p_s1apOam_msg, p_s1ap_oam_reset_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_s1ap_oam_s1ap_link_status_ind_len
(
    s1ap_oam_s1ap_link_status_ind_t *p_s1ap_oam_s1ap_link_status_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_s1ap_link_status_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_s1ap_link_status_ind->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_s1ap_link_status_ind->mme_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_s1ap_link_status_ind->status);

    /* Optional element */
    if(p_s1ap_oam_s1ap_link_status_ind->bitmask & S1AP_OAM_LINK_STATUS_IND_IPv4_ADDRESS_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_oam_s1ap_link_status_ind->ip_addr);
    }

    /* Optional element */
    if(p_s1ap_oam_s1ap_link_status_ind->bitmask & S1AP_OAM_LINK_STATUS_IND_IPv6_ADDRESS_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_oam_s1ap_link_status_ind->ipv6_addr);
    }

    /* Optional element */
    if(p_s1ap_oam_s1ap_link_status_ind->bitmask & S1AP_OAM_LINK_STATUS_IND_PORT_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_s1ap_link_status_ind->port);
    }

    return length;
}

rrc_return_et
rrc_il_compose_s1ap_oam_s1ap_link_status_ind
(
    U8  **pp_buffer,
    s1ap_oam_s1ap_link_status_ind_t *p_s1ap_oam_s1ap_link_status_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_s1ap_link_status_ind != PNULL);

    /* This function composes s1ap_oam_s1ap_link_status_ind */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1ap_oam_s1ap_link_status_ind->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_oam_s1ap_link_status_ind->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_s1ap_link_status_ind->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_s1ap_link_status_ind->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_s1ap_oam_s1ap_link_status_ind->mme_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_s1ap_link_status_ind->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_oam_s1ap_link_status_ind->mme_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_s1ap_link_status_ind->status > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_s1ap_link_status_ind->status] should be less than"
            " or equal to 1. Incorrect value %u received.", p_s1ap_oam_s1ap_link_status_ind->status);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_s1ap_link_status_ind->status, "status");
    *pp_buffer += sizeof(p_s1ap_oam_s1ap_link_status_ind->status);

    /* Optional element */
    if(p_s1ap_oam_s1ap_link_status_ind->bitmask & S1AP_OAM_LINK_STATUS_IND_IPv4_ADDRESS_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_oam_s1ap_link_status_ind->ip_addr); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_s1ap_link_status_ind->ip_addr[loop], "ip_addr[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_s1ap_oam_s1ap_link_status_ind->bitmask & S1AP_OAM_LINK_STATUS_IND_IPv6_ADDRESS_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_oam_s1ap_link_status_ind->ipv6_addr); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_s1ap_link_status_ind->ipv6_addr[loop], "ipv6_addr[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_s1ap_oam_s1ap_link_status_ind->bitmask & S1AP_OAM_LINK_STATUS_IND_PORT_PRESENT)
    {

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1ap_oam_s1ap_link_status_ind->port, "port");
    *pp_buffer += sizeof(p_s1ap_oam_s1ap_link_status_ind->port);
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apOam_il_send_s1ap_oam_s1ap_link_status_ind
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_OAM_S1AP_LINK_STATUS_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_s1ap_link_status_ind
(
    s1ap_oam_s1ap_link_status_ind_t  *p_s1ap_oam_s1ap_link_status_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_oam_s1ap_link_status_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_oam_s1ap_link_status_ind_len(p_s1ap_oam_s1ap_link_status_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_S1AP_LINK_STATUS_IND", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_OAM_S1AP_LINK_STATUS_IND, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_OAM_S1AP_LINK_STATUS_IND, msg_length,cell_index);

    /* Fill S1AP_OAM_S1AP_LINK_STATUS_IND message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_s1ap_link_status_ind(&p_s1apOam_msg, p_s1ap_oam_s1ap_link_status_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_s1ap_oam_status_resp_len
(
    s1ap_oam_status_resp_t *p_s1ap_oam_status_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_status_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_status_resp->active_mme_count);

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_oam_status_resp->active_mme_count; loop++)
        {
            length += rrc_il_get_active_mme_ctx_len(&p_s1ap_oam_status_resp->active_mme_list[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_s1ap_oam_status_resp
(
    U8  **pp_buffer,
    s1ap_oam_status_resp_t *p_s1ap_oam_status_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_status_resp != PNULL);

    /* This function composes s1ap_oam_status_resp */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_status_resp->active_mme_count, "active_mme_count");
    *pp_buffer += sizeof(p_s1ap_oam_status_resp->active_mme_count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_oam_status_resp->active_mme_count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_active_mme_ctx(pp_buffer, &p_s1ap_oam_status_resp->active_mme_list[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apOam_il_send_s1ap_oam_status_resp
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_OAM_STATUS_RESP message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_status_resp
(
    s1ap_oam_status_resp_t  *p_s1ap_oam_status_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_oam_status_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_oam_status_resp_len(p_s1ap_oam_status_resp);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_STATUS_RESP", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_OAM_STATUS_RESP, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_OAM_STATUS_RESP, msg_length,cell_index);

    /* Fill S1AP_OAM_STATUS_RESP message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_status_resp(&p_s1apOam_msg, p_s1ap_oam_status_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

#ifdef LINUX_PC_TEST
rrc_length_t
	rrc_il_get_s1ap_gb_status_resp_len

(
    s1ap_gb_status_resp_t *p_s1ap_gb_status_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_gb_status_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_gb_status_resp->mme_count);
	length += (rrc_length_t)sizeof(p_s1ap_gb_status_resp->cell_count);
	length += (rrc_length_t)sizeof(p_s1ap_gb_status_resp->s1ap_current_state);
	length += (rrc_length_t)sizeof(p_s1ap_gb_status_resp->current_enb_mgr_state);

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_gb_status_resp->mme_count; loop++)
        {
            length += (rrc_length_t)sizeof(pctest_mme_info_t);
        }
    }

    return length;
}

rrc_return_et
	rrc_il_compose_s1ap_gb_status_resp

(
    U8  **pp_buffer,
    s1ap_gb_status_resp_t *p_s1ap_gb_status_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_gb_status_resp != PNULL);    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_gb_status_resp->mme_count, "mme_count");
    *pp_buffer += sizeof(p_s1ap_gb_status_resp->mme_count);

    /* Compose OCTET_STRING VARIABLE of IEs */

    rrc_counter_t loop;
    for (loop = 0; loop < p_s1ap_gb_status_resp->mme_count; loop++)
    {
    	/* Compose parameter of basic type */
		rrc_cp_pack_U8(*pp_buffer, &p_s1ap_gb_status_resp->mme_info[loop].current_mme_state, "current_mme_state");
		*pp_buffer += sizeof(U8);
		
		/* Compose parameter of basic type */
		rrc_cp_pack_U8(*pp_buffer, &p_s1ap_gb_status_resp->mme_info[loop].mme_id, "mme_id");
		*pp_buffer += sizeof(U8);
		
		/* Compose parameter of basic type */
		rrc_cp_pack_U16(*pp_buffer, &p_s1ap_gb_status_resp->mme_info[loop].ue_count, "ue_count");
		*pp_buffer += sizeof(U16);

    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_gb_status_resp->cell_count, "cell_count");
    *pp_buffer += sizeof(U8);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_gb_status_resp->s1ap_current_state, "s1ap_current_state");
    *pp_buffer += sizeof(U8);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_gb_status_resp->current_enb_mgr_state, "s1ap_current_state");
    *pp_buffer += sizeof(U8);

    return RRC_SUCCESS;
}



rrc_return_et
rrc_s1apOam_il_send_s1ap_gb_status_resp
(
    s1ap_gb_status_resp_t  *p_s1ap_gb_status_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_gb_status_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_gb_status_resp_len(p_s1ap_gb_status_resp);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_STATUS_RESP", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_GB_STATUS_RESP, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_GB_STATUS_RESP, msg_length,cell_index);

    /* Fill S1AP_OAM_STATUS_RESP message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_gb_status_resp(&p_s1apOam_msg, p_s1ap_gb_status_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

#endif

static
rrc_length_t
rrc_il_get_active_mme_ctx_len
(
    active_mme_ctx_t *p_active_mme_ctx
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_active_mme_ctx != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_active_mme_ctx->mme_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_active_mme_ctx->count_of_ue);

    return length;
}

static
rrc_return_et
rrc_il_compose_active_mme_ctx
(
    U8  **pp_buffer,
    active_mme_ctx_t *p_active_mme_ctx
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_active_mme_ctx != PNULL);

    /* This function composes active_mme_ctx */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_active_mme_ctx->mme_id > 15))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_active_mme_ctx->mme_id] should be less than"
            " or equal to 15. Incorrect value %u received.", p_active_mme_ctx->mme_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_active_mme_ctx->mme_id, "mme_id");
    *pp_buffer += sizeof(p_active_mme_ctx->mme_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_active_mme_ctx->count_of_ue, "count_of_ue");
    *pp_buffer += sizeof(p_active_mme_ctx->count_of_ue);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_s1ap_oam_mme_info_len
(
    s1ap_oam_mme_info_t *p_s1ap_oam_mme_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_mme_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_mme_info->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_mme_info->mme_id);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_oam_mme_info->ip_addr);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_mme_info->port);

    /* Get length of IE */
    length += rrc_il_get_s1ap_mme_info_len(&p_s1ap_oam_mme_info->mme_info);

    return length;
}

rrc_return_et
rrc_il_compose_s1ap_oam_mme_info
(
    U8  **pp_buffer,
    s1ap_oam_mme_info_t *p_s1ap_oam_mme_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_mme_info != PNULL);

    /* This function composes s1ap_oam_mme_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1ap_oam_mme_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_oam_mme_info->bitmask);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_mme_info->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_oam_mme_info->mme_id);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_oam_mme_info->ip_addr); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_mme_info->ip_addr[loop], "ip_addr[]");
            *pp_buffer += sizeof(U8);
        }
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1ap_oam_mme_info->port, "port");
    *pp_buffer += sizeof(p_s1ap_oam_mme_info->port);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_s1ap_mme_info(pp_buffer, &p_s1ap_oam_mme_info->mme_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apOam_il_send_s1ap_oam_mme_info
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_OAM_MME_INFO message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_mme_info
(
    s1ap_oam_mme_info_t  *p_s1ap_oam_mme_info,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_oam_mme_info != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_oam_mme_info_len(p_s1ap_oam_mme_info);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_MME_INFO", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_OAM_MME_INFO, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_OAM_MME_INFO, msg_length,cell_index);

    /* Fill S1AP_OAM_MME_INFO message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_mme_info(&p_s1apOam_msg, p_s1ap_oam_mme_info))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
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
rrc_il_get_s1ap_served_group_id_len
(
    s1ap_served_group_id_t *p_s1ap_served_group_id
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_served_group_id != PNULL);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_served_group_id->mmegi);

    return length;
}

static
rrc_return_et
rrc_il_compose_s1ap_served_group_id
(
    U8  **pp_buffer,
    s1ap_served_group_id_t *p_s1ap_served_group_id
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_served_group_id != PNULL);

    /* This function composes s1ap_served_group_id */
    

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_served_group_id->mmegi); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_s1ap_served_group_id->mmegi[loop], "mmegi[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_s1ap_served_mmec_len
(
    s1ap_served_mmec_t *p_s1ap_served_mmec
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_served_mmec != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_served_mmec->mmec);

    return length;
}

static
rrc_return_et
rrc_il_compose_s1ap_served_mmec
(
    U8  **pp_buffer,
    s1ap_served_mmec_t *p_s1ap_served_mmec
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_served_mmec != PNULL);

    /* This function composes s1ap_served_mmec */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_served_mmec->mmec, "mmec");
    *pp_buffer += sizeof(p_s1ap_served_mmec->mmec);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_s1ap_served_gummei_info_len
(
    s1ap_served_gummei_info_t *p_s1ap_served_gummei_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_served_gummei_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_served_gummei_info->num_served_plmns);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_served_gummei_info->num_served_plmns < 1) || (p_s1ap_served_gummei_info->num_served_plmns > MAX_NUM_SERVED_PLMN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_served_gummei_info->num_served_plmns] should be in range "
            "1 to MAX_NUM_SERVED_PLMN. Incorrect value %d received.", p_s1ap_served_gummei_info->num_served_plmns);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_served_gummei_info->num_served_plmns; loop++)
        {
            length += rrc_il_get_s1ap_plmn_identity_len(&p_s1ap_served_gummei_info->plmn_identity[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_served_gummei_info->num_served_group_ids);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_served_gummei_info->num_served_group_ids < 1) || (p_s1ap_served_gummei_info->num_served_group_ids > MAX_NUM_SERVED_GROUP_ID))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_served_gummei_info->num_served_group_ids] should be in range "
            "1 to MAX_NUM_SERVED_GROUP_ID. Incorrect value %d received.", p_s1ap_served_gummei_info->num_served_group_ids);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_served_gummei_info->num_served_group_ids; loop++)
        {
            length += rrc_il_get_s1ap_served_group_id_len(&p_s1ap_served_gummei_info->served_group_id[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_served_gummei_info->num_served_mmecs);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_served_gummei_info->num_served_mmecs < 1) || (p_s1ap_served_gummei_info->num_served_mmecs > MAX_NUM_SERVED_MMEC))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_served_gummei_info->num_served_mmecs] should be in range "
            "1 to MAX_NUM_SERVED_MMEC. Incorrect value %d received.", p_s1ap_served_gummei_info->num_served_mmecs);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_served_gummei_info->num_served_mmecs; loop++)
        {
            length += rrc_il_get_s1ap_served_mmec_len(&p_s1ap_served_gummei_info->served_mmec[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_s1ap_served_gummei_info
(
    U8  **pp_buffer,
    s1ap_served_gummei_info_t *p_s1ap_served_gummei_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_served_gummei_info != PNULL);

    /* This function composes s1ap_served_gummei_info */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_served_gummei_info->num_served_plmns < 1) || (p_s1ap_served_gummei_info->num_served_plmns > MAX_NUM_SERVED_PLMN))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_served_gummei_info->num_served_plmns] should be in range "
            "1 to MAX_NUM_SERVED_PLMN. Incorrect value %u received.", p_s1ap_served_gummei_info->num_served_plmns);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_served_gummei_info->num_served_plmns, "num_served_plmns");
    *pp_buffer += sizeof(p_s1ap_served_gummei_info->num_served_plmns);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_served_gummei_info->num_served_plmns; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_s1ap_plmn_identity(pp_buffer, &p_s1ap_served_gummei_info->plmn_identity[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_served_gummei_info->num_served_group_ids < 1) || (p_s1ap_served_gummei_info->num_served_group_ids > MAX_NUM_SERVED_GROUP_ID))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_served_gummei_info->num_served_group_ids] should be in range "
            "1 to MAX_NUM_SERVED_GROUP_ID. Incorrect value %u received.", p_s1ap_served_gummei_info->num_served_group_ids);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1ap_served_gummei_info->num_served_group_ids, "num_served_group_ids");
    *pp_buffer += sizeof(p_s1ap_served_gummei_info->num_served_group_ids);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_served_gummei_info->num_served_group_ids; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_s1ap_served_group_id(pp_buffer, &p_s1ap_served_gummei_info->served_group_id[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_served_gummei_info->num_served_mmecs < 1) || (p_s1ap_served_gummei_info->num_served_mmecs > MAX_NUM_SERVED_MMEC))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_served_gummei_info->num_served_mmecs] should be in range "
            "1 to MAX_NUM_SERVED_MMEC. Incorrect value %u received.", p_s1ap_served_gummei_info->num_served_mmecs);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1ap_served_gummei_info->num_served_mmecs, "num_served_mmecs");
    *pp_buffer += sizeof(p_s1ap_served_gummei_info->num_served_mmecs);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_served_gummei_info->num_served_mmecs; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_s1ap_served_mmec(pp_buffer, &p_s1ap_served_gummei_info->served_mmec[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_s1ap_mme_info_len
(
    s1ap_mme_info_t *p_s1ap_mme_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_mme_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_mme_info->bitmask);

    /* Optional element */
    if(p_s1ap_mme_info->bitmask & S1AP_MME_NAME_PRESENT)
    {
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_mme_info->mme_name);
    }

    /* Optional element */
    if(p_s1ap_mme_info->bitmask & S1AP_SERVED_GUMMEI_INFO_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_s1ap_served_gummei_info_len(&p_s1ap_mme_info->served_gummei_info);
    }

    /* Optional element */
    if(p_s1ap_mme_info->bitmask & S1AP_RELATIVE_MME_CAPACITY_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_mme_info->relative_mme_capacity);
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_s1ap_mme_info
(
    U8  **pp_buffer,
    s1ap_mme_info_t *p_s1ap_mme_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_mme_info != PNULL);

    /* This function composes s1ap_mme_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1ap_mme_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_mme_info->bitmask);

    /* Optional element */
    if(p_s1ap_mme_info->bitmask & S1AP_MME_NAME_PRESENT)
    {

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_mme_info->mme_name); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_s1ap_mme_info->mme_name[loop], "mme_name[]");
            *pp_buffer += sizeof(U8);
        }
    }
    }

    /* Optional element */
    if(p_s1ap_mme_info->bitmask & S1AP_SERVED_GUMMEI_INFO_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_s1ap_served_gummei_info(pp_buffer, &p_s1ap_mme_info->served_gummei_info))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_s1ap_mme_info->bitmask & S1AP_RELATIVE_MME_CAPACITY_PRESENT)
    {

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_mme_info->relative_mme_capacity, "relative_mme_capacity");
    *pp_buffer += sizeof(p_s1ap_mme_info->relative_mme_capacity);
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_s1ap_oam_get_sctp_status_req_len
(
    s1ap_oam_get_sctp_status_req_t *p_s1ap_oam_get_sctp_status_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_get_sctp_status_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_get_sctp_status_req->bitmask);

    /* Optional element */
    if(p_s1ap_oam_get_sctp_status_req->bitmask & S1AP_OAM_MME_STATUS_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_s1ap_oam_mme_id_list_info_len(&p_s1ap_oam_get_sctp_status_req->s1ap_oam_mme_status_list);
    }

    return length;
}

rrc_return_et
rrc_il_compose_s1ap_oam_get_sctp_status_req
(
    U8  **pp_buffer,
    s1ap_oam_get_sctp_status_req_t *p_s1ap_oam_get_sctp_status_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_get_sctp_status_req != PNULL);

    /* This function composes s1ap_oam_get_sctp_status_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1ap_oam_get_sctp_status_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_oam_get_sctp_status_req->bitmask);

    /* Optional element */
    if(p_s1ap_oam_get_sctp_status_req->bitmask & S1AP_OAM_MME_STATUS_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_mme_id_list_info(pp_buffer, &p_s1ap_oam_get_sctp_status_req->s1ap_oam_mme_status_list))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apOam_il_send_s1ap_oam_get_sctp_status_req
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_OAM_GET_SCTP_STATUS_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_get_sctp_status_req
(
    s1ap_oam_get_sctp_status_req_t  *p_s1ap_oam_get_sctp_status_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_oam_get_sctp_status_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_oam_get_sctp_status_req_len(p_s1ap_oam_get_sctp_status_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_GET_SCTP_STATUS_REQ", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_OAM_GET_SCTP_STATUS_REQ, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_OAM_GET_SCTP_STATUS_REQ, msg_length,cell_index);

    /* Fill S1AP_OAM_GET_SCTP_STATUS_REQ message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_get_sctp_status_req(&p_s1apOam_msg, p_s1ap_oam_get_sctp_status_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_s1ap_oam_get_sctp_status_resp_len
(
    s1ap_oam_get_sctp_status_resp_t *p_s1ap_oam_get_sctp_status_resp
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_get_sctp_status_resp != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_get_sctp_status_resp->bitmask);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_get_sctp_status_resp->response);

    /* Optional element */
    if(p_s1ap_oam_get_sctp_status_resp->bitmask & S1AP_OAM_MME_STATUS_PARAMS_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_s1ap_oam_mme_status_params_list_info_len(&p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list);
    }

    /* Optional element */
    if(p_s1ap_oam_get_sctp_status_resp->bitmask & S1AP_OAM_MME_FAILED_LIST_PRESENT)
    {

    /* Get length of IE */
    length += rrc_il_get_s1ap_oam_mme_failed_list_len(&p_s1ap_oam_get_sctp_status_resp->mme_failed_list);
    }

    return length;
}

rrc_return_et
rrc_il_compose_s1ap_oam_get_sctp_status_resp
(
    U8  **pp_buffer,
    s1ap_oam_get_sctp_status_resp_t *p_s1ap_oam_get_sctp_status_resp
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_get_sctp_status_resp != PNULL);

    /* This function composes s1ap_oam_get_sctp_status_resp */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1ap_oam_get_sctp_status_resp->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_oam_get_sctp_status_resp->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_get_sctp_status_resp->response > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_get_sctp_status_resp->response] should be less than"
            " or equal to 2. Incorrect value %u received.", p_s1ap_oam_get_sctp_status_resp->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_get_sctp_status_resp->response, "response");
    *pp_buffer += sizeof(p_s1ap_oam_get_sctp_status_resp->response);

    /* Optional element */
    if(p_s1ap_oam_get_sctp_status_resp->bitmask & S1AP_OAM_MME_STATUS_PARAMS_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_mme_status_params_list_info(pp_buffer, &p_s1ap_oam_get_sctp_status_resp->s1ap_oam_mme_status_params_list))
    {
        return RRC_FAILURE;
    }
    }

    /* Optional element */
    if(p_s1ap_oam_get_sctp_status_resp->bitmask & S1AP_OAM_MME_FAILED_LIST_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_mme_failed_list(pp_buffer, &p_s1ap_oam_get_sctp_status_resp->mme_failed_list))
    {
        return RRC_FAILURE;
    }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apOam_il_send_s1ap_oam_get_sctp_status_resp
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_OAM_GET_SCTP_STATUS_RESP message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apOam_il_send_s1ap_oam_get_sctp_status_resp
(
    s1ap_oam_get_sctp_status_resp_t  *p_s1ap_oam_get_sctp_status_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apOam_msg;

    RRC_ASSERT(p_s1ap_oam_get_sctp_status_resp != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_s1ap_oam_get_sctp_status_resp_len(p_s1ap_oam_get_sctp_status_resp);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_OAM_GET_SCTP_STATUS_RESP", src_module_id, dst_module_id);

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
    p_s1apOam_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, S1AP_OAM_GET_SCTP_STATUS_RESP, msg_api_length);

    /* Fill interface header */
    p_s1apOam_msg = p_s1apOam_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apOam_msg, transaction_id, src_module_id,
        RRC_S1APOAM_MODULE_ID, S1AP_OAM_GET_SCTP_STATUS_RESP, msg_length,cell_index);

    /* Fill S1AP_OAM_GET_SCTP_STATUS_RESP message */
    p_s1apOam_msg = p_s1apOam_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_s1ap_oam_get_sctp_status_resp(&p_s1apOam_msg, p_s1ap_oam_get_sctp_status_resp))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apOam_msg - msg_api_length) != p_msg)
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
rrc_il_get_rrc_timeval_len
(
    rrc_timeval_t *p_rrc_timeval
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_timeval != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_timeval->tv_sec);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_timeval->tv_usec);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_timeval
(
    U8  **pp_buffer,
    rrc_timeval_t *p_rrc_timeval
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_timeval != PNULL);

    /* This function composes rrc_timeval */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_timeval->tv_sec, "tv_sec");
    *pp_buffer += sizeof(p_rrc_timeval->tv_sec);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_timeval->tv_usec, "tv_usec");
    *pp_buffer += sizeof(p_rrc_timeval->tv_usec);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_s1ap_oam_mme_id_status_params_len
(
    s1ap_oam_mme_id_status_params_t *p_s1ap_oam_mme_id_status_params
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_mme_id_status_params != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_mme_id_status_params->mme_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_mme_id_status_params->status);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_mme_id_status_params->in_streams);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_mme_id_status_params->out_streams);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_mme_id_status_params->local_port);

    /* Get length of IE */
    length += rrc_il_get_rrc_timeval_len(&p_s1ap_oam_mme_id_status_params->start_time);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1ap_oam_mme_id_status_params->primary_peer_addr);

    return length;
}

static
rrc_return_et
rrc_il_compose_s1ap_oam_mme_id_status_params
(
    U8  **pp_buffer,
    s1ap_oam_mme_id_status_params_t *p_s1ap_oam_mme_id_status_params
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_mme_id_status_params != PNULL);

    /* This function composes s1ap_oam_mme_id_status_params */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_mme_id_status_params->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_oam_mme_id_status_params->mme_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_oam_mme_id_status_params->status > 9))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_mme_id_status_params->status] should be less than"
            " or equal to 9. Incorrect value %u received.", p_s1ap_oam_mme_id_status_params->status);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_s1ap_oam_mme_id_status_params->status, "status");
    *pp_buffer += sizeof(p_s1ap_oam_mme_id_status_params->status);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_s1ap_oam_mme_id_status_params->in_streams, "in_streams");
    *pp_buffer += sizeof(p_s1ap_oam_mme_id_status_params->in_streams);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_s1ap_oam_mme_id_status_params->out_streams, "out_streams");
    *pp_buffer += sizeof(p_s1ap_oam_mme_id_status_params->out_streams);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_s1ap_oam_mme_id_status_params->local_port, "local_port");
    *pp_buffer += sizeof(p_s1ap_oam_mme_id_status_params->local_port);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_rrc_timeval(pp_buffer, &p_s1ap_oam_mme_id_status_params->start_time))
    {
        return RRC_FAILURE;
    }

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1ap_oam_mme_id_status_params->primary_peer_addr); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_mme_id_status_params->primary_peer_addr[loop], "primary_peer_addr[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_s1ap_oam_mme_status_params_list_info_len
(
    s1ap_oam_mme_status_params_list_info_t *p_s1ap_oam_mme_status_params_list_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_mme_status_params_list_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_mme_status_params_list_info->num_mme_id);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_oam_mme_status_params_list_info->num_mme_id < 1) || (p_s1ap_oam_mme_status_params_list_info->num_mme_id > MAX_NUM_MME))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_mme_status_params_list_info->num_mme_id] should be in range "
            "1 to MAX_NUM_MME. Incorrect value %d received.", p_s1ap_oam_mme_status_params_list_info->num_mme_id);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_oam_mme_status_params_list_info->num_mme_id; loop++)
        {
            length += rrc_il_get_s1ap_oam_mme_id_status_params_len(&p_s1ap_oam_mme_status_params_list_info->staus_params[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_s1ap_oam_mme_status_params_list_info
(
    U8  **pp_buffer,
    s1ap_oam_mme_status_params_list_info_t *p_s1ap_oam_mme_status_params_list_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_mme_status_params_list_info != PNULL);

    /* This function composes s1ap_oam_mme_status_params_list_info */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_oam_mme_status_params_list_info->num_mme_id < 1) || (p_s1ap_oam_mme_status_params_list_info->num_mme_id > MAX_NUM_MME))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_mme_status_params_list_info->num_mme_id] should be in range "
            "1 to MAX_NUM_MME. Incorrect value %u received.", p_s1ap_oam_mme_status_params_list_info->num_mme_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_mme_status_params_list_info->num_mme_id, "num_mme_id");
    *pp_buffer += sizeof(p_s1ap_oam_mme_status_params_list_info->num_mme_id);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_oam_mme_status_params_list_info->num_mme_id; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_s1ap_oam_mme_id_status_params(pp_buffer, &p_s1ap_oam_mme_status_params_list_info->staus_params[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_s1ap_oam_failed_mme_info_len
(
    s1ap_oam_failed_mme_info_t *p_s1ap_oam_failed_mme_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_failed_mme_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_failed_mme_info->mme_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_failed_mme_info->cause);

    return length;
}

static
rrc_return_et
rrc_il_compose_s1ap_oam_failed_mme_info
(
    U8  **pp_buffer,
    s1ap_oam_failed_mme_info_t *p_s1ap_oam_failed_mme_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_failed_mme_info != PNULL);

    /* This function composes s1ap_oam_failed_mme_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_failed_mme_info->mme_id, "mme_id");
    *pp_buffer += sizeof(p_s1ap_oam_failed_mme_info->mme_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_failed_mme_info->cause, "cause");
    *pp_buffer += sizeof(p_s1ap_oam_failed_mme_info->cause);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_s1ap_oam_mme_failed_list_len
(
    s1ap_oam_mme_failed_list_t *p_s1ap_oam_mme_failed_list
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_oam_mme_failed_list != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1ap_oam_mme_failed_list->count);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_oam_mme_failed_list->count < 1) || (p_s1ap_oam_mme_failed_list->count > MAX_NUM_MME))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_mme_failed_list->count] should be in range "
            "1 to MAX_NUM_MME. Incorrect value %d received.", p_s1ap_oam_mme_failed_list->count);
        return RRC_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_oam_mme_failed_list->count; loop++)
        {
            length += rrc_il_get_s1ap_oam_failed_mme_info_len(&p_s1ap_oam_mme_failed_list->failed_mme_info[loop]);
        }
    }

    return length;
}

static
rrc_return_et
rrc_il_compose_s1ap_oam_mme_failed_list
(
    U8  **pp_buffer,
    s1ap_oam_mme_failed_list_t *p_s1ap_oam_mme_failed_list
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_oam_mme_failed_list != PNULL);

    /* This function composes s1ap_oam_mme_failed_list */
    

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_oam_mme_failed_list->count < 1) || (p_s1ap_oam_mme_failed_list->count > MAX_NUM_MME))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_oam_mme_failed_list->count] should be in range "
            "1 to MAX_NUM_MME. Incorrect value %u received.", p_s1ap_oam_mme_failed_list->count);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_oam_mme_failed_list->count, "count");
    *pp_buffer += sizeof(p_s1ap_oam_mme_failed_list->count);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_oam_mme_failed_list->count; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_s1ap_oam_failed_mme_info(pp_buffer, &p_s1ap_oam_mme_failed_list->failed_mme_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

