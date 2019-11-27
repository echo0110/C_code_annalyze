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
 *  File Description : The file rrc_s1u_il_composer.c contains the definitions 
 *                     of rrc-s1u interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/

#include "rrc_cp_common.h"
#include "rrc_s1u_il_composer.h"
#include "rrc_s1u_intf.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"
#include "rrc_msg_mgmt.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_drb_id_len
(
    rrc_s1u_ie_drb_id_t *p_rrc_s1u_ie_drb_id
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_drb_id_tlv_len
(
    rrc_s1u_ie_drb_id_t *p_rrc_s1u_ie_drb_id

);

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_drb_id
(
    U8  **pp_buffer,
    rrc_s1u_ie_drb_id_t *p_rrc_s1u_ie_drb_id
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_buffer_ind_len
(
    rrc_s1u_ie_buffer_ind_t *p_rrc_s1u_ie_buffer_ind
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_buffer_ind_tlv_len
(
    rrc_s1u_ie_buffer_ind_t *p_rrc_s1u_ie_buffer_ind

);

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_buffer_ind
(
    U8  **pp_buffer,
    rrc_s1u_ie_buffer_ind_t *p_rrc_s1u_ie_buffer_ind
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_teid_self_len
(
    rrc_s1u_ie_teid_self_t *p_rrc_s1u_ie_teid_self
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_teid_self_tlv_len
(
    rrc_s1u_ie_teid_self_t *p_rrc_s1u_ie_teid_self

);

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_teid_self
(
    U8  **pp_buffer,
    rrc_s1u_ie_teid_self_t *p_rrc_s1u_ie_teid_self
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_qos_profile_len
(
    rrc_s1u_ie_qos_profile_t *p_rrc_s1u_ie_qos_profile
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_qos_profile_tlv_len
(
    rrc_s1u_ie_qos_profile_t *p_rrc_s1u_ie_qos_profile

);

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_qos_profile
(
    U8  **pp_buffer,
    rrc_s1u_ie_qos_profile_t *p_rrc_s1u_ie_qos_profile
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_seq_num_len
(
    rrc_s1u_ie_seq_num_t *p_rrc_s1u_ie_seq_num
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_seq_num_tlv_len
(
    rrc_s1u_ie_seq_num_t *p_rrc_s1u_ie_seq_num

);

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_seq_num
(
    U8  **pp_buffer,
    rrc_s1u_ie_seq_num_t *p_rrc_s1u_ie_seq_num
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_seq_disable_len
(
    rrc_s1u_ie_seq_disable_t *p_rrc_s1u_ie_seq_disable
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_seq_disable_tlv_len
(
    rrc_s1u_ie_seq_disable_t *p_rrc_s1u_ie_seq_disable

);

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_seq_disable
(
    U8  **pp_buffer,
    rrc_s1u_ie_seq_disable_t *p_rrc_s1u_ie_seq_disable
);

static
rrc_length_t
rrc_il_get_rrc_s1u_reordering_reqd_len
(
    rrc_s1u_reordering_reqd_t *p_rrc_s1u_reordering_reqd
);

static
rrc_length_t
rrc_il_get_rrc_s1u_reordering_reqd_tlv_len
(
    rrc_s1u_reordering_reqd_t *p_rrc_s1u_reordering_reqd

);

static
rrc_return_et
rrc_il_compose_rrc_s1u_reordering_reqd
(
    U8  **pp_buffer,
    rrc_s1u_reordering_reqd_t *p_rrc_s1u_reordering_reqd
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_pdcp_ext_len
(
    rrc_s1u_ie_pdcp_ext_t *p_rrc_s1u_ie_pdcp_ext
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_pdcp_ext_tlv_len
(
    rrc_s1u_ie_pdcp_ext_t *p_rrc_s1u_ie_pdcp_ext

);

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_pdcp_ext
(
    U8  **pp_buffer,
    rrc_s1u_ie_pdcp_ext_t *p_rrc_s1u_ie_pdcp_ext
);

static
rrc_length_t
rrc_il_get_rrc_s1u_sap_param_len
(
    rrc_s1u_sap_param_t *p_rrc_s1u_sap_param
);

static
rrc_length_t
rrc_il_get_rrc_s1u_sap_param_tlv_len
(
    rrc_s1u_sap_param_t *p_rrc_s1u_sap_param

);

static
rrc_return_et
rrc_il_compose_rrc_s1u_sap_param
(
    U8  **pp_buffer,
    rrc_s1u_sap_param_t *p_rrc_s1u_sap_param
);

static
rrc_length_t
rrc_il_get_rrc_dc_bearer_type_len
(
    rrc_dc_bearer_type_t *p_rrc_dc_bearer_type
);

static
rrc_length_t
rrc_il_get_rrc_dc_bearer_type_tlv_len
(
    rrc_dc_bearer_type_t *p_rrc_dc_bearer_type

);

static
rrc_return_et
rrc_il_compose_rrc_dc_bearer_type
(
    U8  **pp_buffer,
    rrc_dc_bearer_type_t *p_rrc_dc_bearer_type
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_teid_peer_len
(
    rrc_s1u_ie_teid_peer_t *p_rrc_s1u_ie_teid_peer
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_teid_peer_tlv_len
(
    rrc_s1u_ie_teid_peer_t *p_rrc_s1u_ie_teid_peer

);

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_teid_peer
(
    U8  **pp_buffer,
    rrc_s1u_ie_teid_peer_t *p_rrc_s1u_ie_teid_peer
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_gsn_addr_len
(
    rrc_s1u_ie_gsn_addr_t *p_rrc_s1u_ie_gsn_addr
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_gsn_addr_tlv_len
(
    rrc_s1u_ie_gsn_addr_t *p_rrc_s1u_ie_gsn_addr

);

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_gsn_addr
(
    U8  **pp_buffer,
    rrc_s1u_ie_gsn_addr_t *p_rrc_s1u_ie_gsn_addr
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_tunnel_info_len
(
    rrc_s1u_ie_tunnel_info_t *p_rrc_s1u_ie_tunnel_info
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_tunnel_info_tlv_len
(
    rrc_s1u_ie_tunnel_info_t *p_rrc_s1u_ie_tunnel_info

);

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_tunnel_info
(
    U8  **pp_buffer,
    rrc_s1u_ie_tunnel_info_t *p_rrc_s1u_ie_tunnel_info
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_relay_setup_sap_req_len
(
    rrc_s1u_ie_relay_setup_sap_req_t *p_rrc_s1u_ie_relay_setup_sap_req
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_relay_setup_sap_req_tlv_len
(
    rrc_s1u_ie_relay_setup_sap_req_t *p_rrc_s1u_ie_relay_setup_sap_req

);

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_relay_setup_sap_req
(
    U8  **pp_buffer,
    rrc_s1u_ie_relay_setup_sap_req_t *p_rrc_s1u_ie_relay_setup_sap_req
);

static
rrc_length_t
rrc_il_get_rrc_release_tunnel_info_len
(
    rrc_release_tunnel_info_t *p_rrc_release_tunnel_info
);

static
rrc_length_t
rrc_il_get_rrc_release_tunnel_info_tlv_len
(
    rrc_release_tunnel_info_t *p_rrc_release_tunnel_info

);

static
rrc_return_et
rrc_il_compose_rrc_release_tunnel_info
(
    U8  **pp_buffer,
    rrc_release_tunnel_info_t *p_rrc_release_tunnel_info
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_relay_rel_sap_req_len
(
    rrc_s1u_ie_relay_rel_sap_req_t *p_rrc_s1u_ie_relay_rel_sap_req
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_relay_rel_sap_req_tlv_len
(
    rrc_s1u_ie_relay_rel_sap_req_t *p_rrc_s1u_ie_relay_rel_sap_req

);

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_relay_rel_sap_req
(
    U8  **pp_buffer,
    rrc_s1u_ie_relay_rel_sap_req_t *p_rrc_s1u_ie_relay_rel_sap_req
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_tunnel_info_cnf_len
(
    rrc_s1u_ie_tunnel_info_cnf_t *p_rrc_s1u_ie_tunnel_info_cnf
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_tunnel_info_cnf_tlv_len
(
    rrc_s1u_ie_tunnel_info_cnf_t *p_rrc_s1u_ie_tunnel_info_cnf

);

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_tunnel_info_cnf
(
    U8  **pp_buffer,
    rrc_s1u_ie_tunnel_info_cnf_t *p_rrc_s1u_ie_tunnel_info_cnf
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_relay_setup_sap_cnf_len
(
    rrc_s1u_ie_relay_setup_sap_cnf_t *p_rrc_s1u_ie_relay_setup_sap_cnf
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_relay_setup_sap_cnf_tlv_len
(
    rrc_s1u_ie_relay_setup_sap_cnf_t *p_rrc_s1u_ie_relay_setup_sap_cnf

);

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_relay_setup_sap_cnf
(
    U8  **pp_buffer,
    rrc_s1u_ie_relay_setup_sap_cnf_t *p_rrc_s1u_ie_relay_setup_sap_cnf
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_relay_rel_sap_cnf_len
(
    rrc_s1u_ie_relay_rel_sap_cnf_t *p_rrc_s1u_ie_relay_rel_sap_cnf
);

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_relay_rel_sap_cnf_tlv_len
(
    rrc_s1u_ie_relay_rel_sap_cnf_t *p_rrc_s1u_ie_relay_rel_sap_cnf

);

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_relay_rel_sap_cnf
(
    U8  **pp_buffer,
    rrc_s1u_ie_relay_rel_sap_cnf_t *p_rrc_s1u_ie_relay_rel_sap_cnf
);

static
rrc_length_t
rrc_il_get_s1u_communication_info_len
(
    s1u_communication_info_t *p_s1u_communication_info
);

static
rrc_return_et
rrc_il_compose_s1u_communication_info
(
    U8  **pp_buffer,
    s1u_communication_info_t *p_s1u_communication_info
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_length_t
rrc_il_get_rrc_s1u_delete_ue_entity_req_len
(
    rrc_s1u_delete_ue_entity_req_t *p_rrc_s1u_delete_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_delete_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_delete_ue_entity_req->ue_index);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_s1u_delete_ue_entity_req
(
    U8  **pp_buffer,
    rrc_s1u_delete_ue_entity_req_t *p_rrc_s1u_delete_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_delete_ue_entity_req != PNULL);

    /* This function composes rrc_s1u_delete_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_s1u_delete_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_s1u_delete_ue_entity_req->ue_index);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1u_il_send_rrc_s1u_delete_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_S1U_DELETE_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1u_il_send_rrc_s1u_delete_ue_entity_req
(
    rrc_s1u_delete_ue_entity_req_t  *p_rrc_s1u_delete_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1u_msg;

    RRC_ASSERT(p_rrc_s1u_delete_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_s1u_delete_ue_entity_req_len(p_rrc_s1u_delete_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_S1U_DELETE_UE_ENTITY_REQ", src_module_id, dst_module_id);

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
    p_s1u_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1u_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_S1U_DELETE_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_s1u_msg = p_s1u_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1u_msg, transaction_id, src_module_id,
        RRC_S1U_MODULE_ID, RRC_S1U_DELETE_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_S1U_DELETE_UE_ENTITY_REQ message */
    p_s1u_msg = p_s1u_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_s1u_delete_ue_entity_req(&p_s1u_msg, p_rrc_s1u_delete_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1u_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_s1u_delete_ue_entity_cnf_len
(
    rrc_s1u_delete_ue_entity_cnf_t *p_rrc_s1u_delete_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_delete_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_delete_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_delete_ue_entity_cnf->response_code);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_s1u_delete_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_s1u_delete_ue_entity_cnf_t *p_rrc_s1u_delete_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_delete_ue_entity_cnf != PNULL);

    /* This function composes rrc_s1u_delete_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_s1u_delete_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_s1u_delete_ue_entity_cnf->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_delete_ue_entity_cnf->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_s1u_delete_ue_entity_cnf->response_code);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1u_il_send_rrc_s1u_delete_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_S1U_DELETE_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1u_il_send_rrc_s1u_delete_ue_entity_cnf
(
    rrc_s1u_delete_ue_entity_cnf_t  *p_rrc_s1u_delete_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1u_msg;

    RRC_ASSERT(p_rrc_s1u_delete_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_s1u_delete_ue_entity_cnf_len(p_rrc_s1u_delete_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_S1U_DELETE_UE_ENTITY_CNF", src_module_id, dst_module_id);

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
    p_s1u_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1u_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_S1U_DELETE_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_s1u_msg = p_s1u_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1u_msg, transaction_id, src_module_id,
        RRC_S1U_MODULE_ID, RRC_S1U_DELETE_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_S1U_DELETE_UE_ENTITY_CNF message */
    p_s1u_msg = p_s1u_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_s1u_delete_ue_entity_cnf(&p_s1u_msg, p_rrc_s1u_delete_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1u_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_s1u_reconfigure_ue_entity_req_len
(
    rrc_s1u_reconfigure_ue_entity_req_t *p_rrc_s1u_reconfigure_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_reconfigure_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_reconfigure_ue_entity_req->ue_index);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_reconfigure_ue_entity_req->num_setup_sap_req <= ARRSIZE(p_rrc_s1u_reconfigure_ue_entity_req->setup_sap_req));
        for (loop = 0; loop < p_rrc_s1u_reconfigure_ue_entity_req->num_setup_sap_req; loop++)
        {
            length += rrc_il_get_rrc_s1u_ie_relay_setup_sap_req_tlv_len(&p_rrc_s1u_reconfigure_ue_entity_req->setup_sap_req[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_reconfigure_ue_entity_req->num_rel_sap_req <= ARRSIZE(p_rrc_s1u_reconfigure_ue_entity_req->rel_sap_req));
        for (loop = 0; loop < p_rrc_s1u_reconfigure_ue_entity_req->num_rel_sap_req; loop++)
        {
            length += rrc_il_get_rrc_s1u_ie_relay_rel_sap_req_tlv_len(&p_rrc_s1u_reconfigure_ue_entity_req->rel_sap_req[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_s1u_reconfigure_ue_entity_req
(
    U8  **pp_buffer,
    rrc_s1u_reconfigure_ue_entity_req_t *p_rrc_s1u_reconfigure_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_reconfigure_ue_entity_req != PNULL);

    /* This function composes rrc_s1u_reconfigure_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_s1u_reconfigure_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_s1u_reconfigure_ue_entity_req->ue_index);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_reconfigure_ue_entity_req->num_setup_sap_req <= ARRSIZE(p_rrc_s1u_reconfigure_ue_entity_req->setup_sap_req));
        for (loop = 0; loop < p_rrc_s1u_reconfigure_ue_entity_req->num_setup_sap_req; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_relay_setup_sap_req(pp_buffer, &p_rrc_s1u_reconfigure_ue_entity_req->setup_sap_req[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_reconfigure_ue_entity_req->num_rel_sap_req <= ARRSIZE(p_rrc_s1u_reconfigure_ue_entity_req->rel_sap_req));
        for (loop = 0; loop < p_rrc_s1u_reconfigure_ue_entity_req->num_rel_sap_req; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_relay_rel_sap_req(pp_buffer, &p_rrc_s1u_reconfigure_ue_entity_req->rel_sap_req[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1u_il_send_rrc_s1u_reconfigure_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_S1U_RECONFIGURE_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1u_il_send_rrc_s1u_reconfigure_ue_entity_req
(
    rrc_s1u_reconfigure_ue_entity_req_t  *p_rrc_s1u_reconfigure_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1u_msg;

    RRC_ASSERT(p_rrc_s1u_reconfigure_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_s1u_reconfigure_ue_entity_req_len(p_rrc_s1u_reconfigure_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_S1U_RECONFIGURE_UE_ENTITY_REQ", src_module_id, dst_module_id);

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
    p_s1u_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1u_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_S1U_RECONFIGURE_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_s1u_msg = p_s1u_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1u_msg, transaction_id, src_module_id,
        RRC_S1U_MODULE_ID, RRC_S1U_RECONFIGURE_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_S1U_RECONFIGURE_UE_ENTITY_REQ message */
    p_s1u_msg = p_s1u_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_s1u_reconfigure_ue_entity_req(&p_s1u_msg, p_rrc_s1u_reconfigure_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1u_msg - msg_api_length) != p_msg)
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
rrc_il_get_rrc_s1u_ie_drb_id_len
(
    rrc_s1u_ie_drb_id_t *p_rrc_s1u_ie_drb_id
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_drb_id != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_drb_id->drb_id);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_drb_id_tlv_len
(
    rrc_s1u_ie_drb_id_t *p_rrc_s1u_ie_drb_id

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_drb_id != PNULL);

    length += rrc_il_get_rrc_s1u_ie_drb_id_len(p_rrc_s1u_ie_drb_id);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_drb_id
(
    U8  **pp_buffer,
    rrc_s1u_ie_drb_id_t *p_rrc_s1u_ie_drb_id
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_ie_drb_id != PNULL);

    /* This function composes rrc_s1u_ie_drb_id */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_DRBID_SELF_TAG;
    tlv_length = rrc_il_get_rrc_s1u_ie_drb_id_tlv_len(p_rrc_s1u_ie_drb_id);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_ie_drb_id->drb_id, "drb_id");
    *pp_buffer += sizeof(p_rrc_s1u_ie_drb_id->drb_id);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_buffer_ind_len
(
    rrc_s1u_ie_buffer_ind_t *p_rrc_s1u_ie_buffer_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_buffer_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_buffer_ind->buffer_ind);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_buffer_ind_tlv_len
(
    rrc_s1u_ie_buffer_ind_t *p_rrc_s1u_ie_buffer_ind

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_buffer_ind != PNULL);

    length += rrc_il_get_rrc_s1u_ie_buffer_ind_len(p_rrc_s1u_ie_buffer_ind);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_buffer_ind
(
    U8  **pp_buffer,
    rrc_s1u_ie_buffer_ind_t *p_rrc_s1u_ie_buffer_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_ie_buffer_ind != PNULL);

    /* This function composes rrc_s1u_ie_buffer_ind */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_BUFFER_IND_TAG;
    tlv_length = rrc_il_get_rrc_s1u_ie_buffer_ind_tlv_len(p_rrc_s1u_ie_buffer_ind);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_ie_buffer_ind->buffer_ind > 0))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_buffer_ind->buffer_ind] should be less than"
            " or equal to 0. Incorrect value %u received.", p_rrc_s1u_ie_buffer_ind->buffer_ind);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_ie_buffer_ind->buffer_ind, "buffer_ind");
    *pp_buffer += sizeof(p_rrc_s1u_ie_buffer_ind->buffer_ind);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_teid_self_len
(
    rrc_s1u_ie_teid_self_t *p_rrc_s1u_ie_teid_self
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_teid_self != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_teid_self->teid);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_teid_self_tlv_len
(
    rrc_s1u_ie_teid_self_t *p_rrc_s1u_ie_teid_self

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_teid_self != PNULL);

    length += rrc_il_get_rrc_s1u_ie_teid_self_len(p_rrc_s1u_ie_teid_self);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_teid_self
(
    U8  **pp_buffer,
    rrc_s1u_ie_teid_self_t *p_rrc_s1u_ie_teid_self
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_ie_teid_self != PNULL);

    /* This function composes rrc_s1u_ie_teid_self */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_TEID_SELF_TAG;
    tlv_length = rrc_il_get_rrc_s1u_ie_teid_self_tlv_len(p_rrc_s1u_ie_teid_self);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_s1u_ie_teid_self->teid < 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_teid_self->teid] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_s1u_ie_teid_self->teid);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_s1u_ie_teid_self->teid, "teid");
    *pp_buffer += sizeof(p_rrc_s1u_ie_teid_self->teid);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_qos_profile_len
(
    rrc_s1u_ie_qos_profile_t *p_rrc_s1u_ie_qos_profile
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_qos_profile != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_qos_profile->allocation_retention_priority);

    /* Add buffer size */
    length += p_rrc_s1u_ie_qos_profile->qos_profile_data_length;

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_qos_profile_tlv_len
(
    rrc_s1u_ie_qos_profile_t *p_rrc_s1u_ie_qos_profile

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_qos_profile != PNULL);

    length += rrc_il_get_rrc_s1u_ie_qos_profile_len(p_rrc_s1u_ie_qos_profile);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_qos_profile
(
    U8  **pp_buffer,
    rrc_s1u_ie_qos_profile_t *p_rrc_s1u_ie_qos_profile
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_ie_qos_profile != PNULL);

    /* This function composes rrc_s1u_ie_qos_profile */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_QOS_PROFILE_TAG;
    tlv_length = rrc_il_get_rrc_s1u_ie_qos_profile_tlv_len(p_rrc_s1u_ie_qos_profile);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_ie_qos_profile->allocation_retention_priority, "allocation_retention_priority");
    *pp_buffer += sizeof(p_rrc_s1u_ie_qos_profile->allocation_retention_priority);

    /* Compose OCTET_STRING LIMITED_TILL_THE_END */
    {
        if(ARRSIZE(p_rrc_s1u_ie_qos_profile->qos_profile_data) < p_rrc_s1u_ie_qos_profile->qos_profile_data_length)
        {
            RRC_TRACE(RRC_WARNING, "p_rrc_s1u_ie_qos_profile->qos_profile_data_length = %u is exceeding p_rrc_s1u_ie_qos_profile->qos_profile_data array boundary", p_rrc_s1u_ie_qos_profile->qos_profile_data_length);
            return RRC_FAILURE;
        }
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_s1u_ie_qos_profile->qos_profile_data_length; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_ie_qos_profile->qos_profile_data[loop], "qos_profile_data[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_seq_num_len
(
    rrc_s1u_ie_seq_num_t *p_rrc_s1u_ie_seq_num
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_seq_num != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_seq_num->ul_sequence_number);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_seq_num->dl_sequence_number);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_seq_num_tlv_len
(
    rrc_s1u_ie_seq_num_t *p_rrc_s1u_ie_seq_num

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_seq_num != PNULL);

    length += rrc_il_get_rrc_s1u_ie_seq_num_len(p_rrc_s1u_ie_seq_num);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_seq_num
(
    U8  **pp_buffer,
    rrc_s1u_ie_seq_num_t *p_rrc_s1u_ie_seq_num
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_ie_seq_num != PNULL);

    /* This function composes rrc_s1u_ie_seq_num */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_SEQ_NUM_TAG;
    tlv_length = rrc_il_get_rrc_s1u_ie_seq_num_tlv_len(p_rrc_s1u_ie_seq_num);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_s1u_ie_seq_num->ul_sequence_number, "ul_sequence_number");
    *pp_buffer += sizeof(p_rrc_s1u_ie_seq_num->ul_sequence_number);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_s1u_ie_seq_num->dl_sequence_number, "dl_sequence_number");
    *pp_buffer += sizeof(p_rrc_s1u_ie_seq_num->dl_sequence_number);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_seq_disable_len
(
    rrc_s1u_ie_seq_disable_t *p_rrc_s1u_ie_seq_disable
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_seq_disable != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_seq_disable->disable);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_seq_disable_tlv_len
(
    rrc_s1u_ie_seq_disable_t *p_rrc_s1u_ie_seq_disable

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_seq_disable != PNULL);

    length += rrc_il_get_rrc_s1u_ie_seq_disable_len(p_rrc_s1u_ie_seq_disable);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_seq_disable
(
    U8  **pp_buffer,
    rrc_s1u_ie_seq_disable_t *p_rrc_s1u_ie_seq_disable
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_ie_seq_disable != PNULL);

    /* This function composes rrc_s1u_ie_seq_disable */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_SEQ_DISABLE_TAG;
    tlv_length = rrc_il_get_rrc_s1u_ie_seq_disable_tlv_len(p_rrc_s1u_ie_seq_disable);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_ie_seq_disable->disable > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_seq_disable->disable] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_s1u_ie_seq_disable->disable);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_ie_seq_disable->disable, "disable");
    *pp_buffer += sizeof(p_rrc_s1u_ie_seq_disable->disable);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_reordering_reqd_len
(
    rrc_s1u_reordering_reqd_t *p_rrc_s1u_reordering_reqd
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_reordering_reqd != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_reordering_reqd->reordering_reqd);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_reordering_reqd_tlv_len
(
    rrc_s1u_reordering_reqd_t *p_rrc_s1u_reordering_reqd

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_reordering_reqd != PNULL);

    length += rrc_il_get_rrc_s1u_reordering_reqd_len(p_rrc_s1u_reordering_reqd);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_s1u_reordering_reqd
(
    U8  **pp_buffer,
    rrc_s1u_reordering_reqd_t *p_rrc_s1u_reordering_reqd
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_reordering_reqd != PNULL);

    /* This function composes rrc_s1u_reordering_reqd */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_REORDERING_REQD_TAG;
    tlv_length = rrc_il_get_rrc_s1u_reordering_reqd_tlv_len(p_rrc_s1u_reordering_reqd);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_reordering_reqd->reordering_reqd > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_reordering_reqd->reordering_reqd] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_s1u_reordering_reqd->reordering_reqd);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_reordering_reqd->reordering_reqd, "reordering_reqd");
    *pp_buffer += sizeof(p_rrc_s1u_reordering_reqd->reordering_reqd);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_pdcp_ext_len
(
    rrc_s1u_ie_pdcp_ext_t *p_rrc_s1u_ie_pdcp_ext
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_pdcp_ext != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_pdcp_ext->pdcp_ext_flag);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_pdcp_ext_tlv_len
(
    rrc_s1u_ie_pdcp_ext_t *p_rrc_s1u_ie_pdcp_ext

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_pdcp_ext != PNULL);

    length += rrc_il_get_rrc_s1u_ie_pdcp_ext_len(p_rrc_s1u_ie_pdcp_ext);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_pdcp_ext
(
    U8  **pp_buffer,
    rrc_s1u_ie_pdcp_ext_t *p_rrc_s1u_ie_pdcp_ext
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_ie_pdcp_ext != PNULL);

    /* This function composes rrc_s1u_ie_pdcp_ext */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_PDCP_EXT_TAG;
    tlv_length = rrc_il_get_rrc_s1u_ie_pdcp_ext_tlv_len(p_rrc_s1u_ie_pdcp_ext);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_ie_pdcp_ext->pdcp_ext_flag > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_pdcp_ext->pdcp_ext_flag] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_s1u_ie_pdcp_ext->pdcp_ext_flag);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_ie_pdcp_ext->pdcp_ext_flag, "pdcp_ext_flag");
    *pp_buffer += sizeof(p_rrc_s1u_ie_pdcp_ext->pdcp_ext_flag);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_sap_param_len
(
    rrc_s1u_sap_param_t *p_rrc_s1u_sap_param
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_sap_param != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_sap_param->sn_size);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_sap_param->rlc_mode);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_sap_param_tlv_len
(
    rrc_s1u_sap_param_t *p_rrc_s1u_sap_param

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_sap_param != PNULL);

    length += rrc_il_get_rrc_s1u_sap_param_len(p_rrc_s1u_sap_param);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_s1u_sap_param
(
    U8  **pp_buffer,
    rrc_s1u_sap_param_t *p_rrc_s1u_sap_param
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_sap_param != PNULL);

    /* This function composes rrc_s1u_sap_param */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_SAP_PARAM_TAG;
    tlv_length = rrc_il_get_rrc_s1u_sap_param_tlv_len(p_rrc_s1u_sap_param);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_s1u_sap_param->sn_size < 12) || (p_rrc_s1u_sap_param->sn_size > 18))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_sap_param->sn_size] should be in range "
            "12 to 18. Incorrect value %u received.", p_rrc_s1u_sap_param->sn_size);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_sap_param->sn_size, "sn_size");
    *pp_buffer += sizeof(p_rrc_s1u_sap_param->sn_size);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_sap_param->rlc_mode > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_sap_param->rlc_mode] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_s1u_sap_param->rlc_mode);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_sap_param->rlc_mode, "rlc_mode");
    *pp_buffer += sizeof(p_rrc_s1u_sap_param->rlc_mode);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_dc_bearer_type_len
(
    rrc_dc_bearer_type_t *p_rrc_dc_bearer_type
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_dc_bearer_type != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_dc_bearer_type->dc_bearer_type);

    /* Get length of TLV */
    if (p_rrc_dc_bearer_type->bitmask & RRC_S1U_IE_SAP_PARAM_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_sap_param_tlv_len(&p_rrc_dc_bearer_type->sap_param);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_dc_bearer_type_tlv_len
(
    rrc_dc_bearer_type_t *p_rrc_dc_bearer_type

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_dc_bearer_type != PNULL);

    length += rrc_il_get_rrc_dc_bearer_type_len(p_rrc_dc_bearer_type);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_dc_bearer_type
(
    U8  **pp_buffer,
    rrc_dc_bearer_type_t *p_rrc_dc_bearer_type
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_dc_bearer_type != PNULL);

    /* This function composes rrc_dc_bearer_type */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_DC_BEARER_FLAG;
    tlv_length = rrc_il_get_rrc_dc_bearer_type_tlv_len(p_rrc_dc_bearer_type);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_dc_bearer_type->dc_bearer_type > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_dc_bearer_type->dc_bearer_type] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_dc_bearer_type->dc_bearer_type);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_dc_bearer_type->dc_bearer_type, "dc_bearer_type");
    *pp_buffer += sizeof(p_rrc_dc_bearer_type->dc_bearer_type);

    /* Compose TLV  */
    if (p_rrc_dc_bearer_type->bitmask & RRC_S1U_IE_SAP_PARAM_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_sap_param(pp_buffer, &p_rrc_dc_bearer_type->sap_param))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_teid_peer_len
(
    rrc_s1u_ie_teid_peer_t *p_rrc_s1u_ie_teid_peer
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_teid_peer != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_teid_peer->teid);

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_teid_peer_tlv_len
(
    rrc_s1u_ie_teid_peer_t *p_rrc_s1u_ie_teid_peer

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_teid_peer != PNULL);

    length += rrc_il_get_rrc_s1u_ie_teid_peer_len(p_rrc_s1u_ie_teid_peer);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_teid_peer
(
    U8  **pp_buffer,
    rrc_s1u_ie_teid_peer_t *p_rrc_s1u_ie_teid_peer
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_ie_teid_peer != PNULL);

    /* This function composes rrc_s1u_ie_teid_peer */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_TEID_PEER_TAG;
    tlv_length = rrc_il_get_rrc_s1u_ie_teid_peer_tlv_len(p_rrc_s1u_ie_teid_peer);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_s1u_ie_teid_peer->teid, "teid");
    *pp_buffer += sizeof(p_rrc_s1u_ie_teid_peer->teid);

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_gsn_addr_len
(
    rrc_s1u_ie_gsn_addr_t *p_rrc_s1u_ie_gsn_addr
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_gsn_addr != PNULL);

    /* Add buffer size */
    length += p_rrc_s1u_ie_gsn_addr->data_length;

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_gsn_addr_tlv_len
(
    rrc_s1u_ie_gsn_addr_t *p_rrc_s1u_ie_gsn_addr

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_gsn_addr != PNULL);

    length += rrc_il_get_rrc_s1u_ie_gsn_addr_len(p_rrc_s1u_ie_gsn_addr);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_gsn_addr
(
    U8  **pp_buffer,
    rrc_s1u_ie_gsn_addr_t *p_rrc_s1u_ie_gsn_addr
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_ie_gsn_addr != PNULL);

    /* This function composes rrc_s1u_ie_gsn_addr */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_GSN_ADDR_TAG;
    tlv_length = rrc_il_get_rrc_s1u_ie_gsn_addr_tlv_len(p_rrc_s1u_ie_gsn_addr);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose OCTET_STRING LIMITED_TILL_THE_END */
    {
        if(ARRSIZE(p_rrc_s1u_ie_gsn_addr->data) < p_rrc_s1u_ie_gsn_addr->data_length)
        {
            RRC_TRACE(RRC_WARNING, "p_rrc_s1u_ie_gsn_addr->data_length = %u is exceeding p_rrc_s1u_ie_gsn_addr->data array boundary", p_rrc_s1u_ie_gsn_addr->data_length);
            return RRC_FAILURE;
        }
        rrc_counter_t loop;
        for (loop = 0; loop < p_rrc_s1u_ie_gsn_addr->data_length; loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_ie_gsn_addr->data[loop], "data[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_tunnel_info_len
(
    rrc_s1u_ie_tunnel_info_t *p_rrc_s1u_ie_tunnel_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_tunnel_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_tunnel_info->sap_flag);

    /* Get length of TLV */
    if (p_rrc_s1u_ie_tunnel_info->bitmask & RRC_S1U_IE_TUNNEL_INFO_TEID_PEER_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_ie_teid_peer_tlv_len(&p_rrc_s1u_ie_tunnel_info->teid_peer);
    }

    /* Get length of TLV */
    if (p_rrc_s1u_ie_tunnel_info->bitmask & RRC_S1U_IE_TUNNEL_INFO_TRANSPORT_ADDR_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_ie_gsn_addr_tlv_len(&p_rrc_s1u_ie_tunnel_info->transport_addr);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_tunnel_info_tlv_len
(
    rrc_s1u_ie_tunnel_info_t *p_rrc_s1u_ie_tunnel_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_tunnel_info != PNULL);

    length += rrc_il_get_rrc_s1u_ie_tunnel_info_len(p_rrc_s1u_ie_tunnel_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_tunnel_info
(
    U8  **pp_buffer,
    rrc_s1u_ie_tunnel_info_t *p_rrc_s1u_ie_tunnel_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_ie_tunnel_info != PNULL);

    /* This function composes rrc_s1u_ie_tunnel_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_TUNNEL_INFO_TAG;
    tlv_length = rrc_il_get_rrc_s1u_ie_tunnel_info_tlv_len(p_rrc_s1u_ie_tunnel_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_ie_tunnel_info->sap_flag, "sap_flag");
    *pp_buffer += sizeof(p_rrc_s1u_ie_tunnel_info->sap_flag);

    /* Compose TLV  */
    if (p_rrc_s1u_ie_tunnel_info->bitmask & RRC_S1U_IE_TUNNEL_INFO_TEID_PEER_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_teid_peer(pp_buffer, &p_rrc_s1u_ie_tunnel_info->teid_peer))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_s1u_ie_tunnel_info->bitmask & RRC_S1U_IE_TUNNEL_INFO_TRANSPORT_ADDR_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_gsn_addr(pp_buffer, &p_rrc_s1u_ie_tunnel_info->transport_addr))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_relay_setup_sap_req_len
(
    rrc_s1u_ie_relay_setup_sap_req_t *p_rrc_s1u_ie_relay_setup_sap_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_relay_setup_sap_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_relay_setup_sap_req->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_relay_setup_sap_req->qos_id);

    /* Get length of TLV */
    if (p_rrc_s1u_ie_relay_setup_sap_req->bitmask & RRC_S1U_IE_SETUP_DRBID_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_ie_drb_id_tlv_len(&p_rrc_s1u_ie_relay_setup_sap_req->drb_id);
    }

    /* Get length of TLV */
    if (p_rrc_s1u_ie_relay_setup_sap_req->bitmask & RRC_S1U_IE_SETUP_BUFFER_IND_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_ie_buffer_ind_tlv_len(&p_rrc_s1u_ie_relay_setup_sap_req->buffer_ind);
    }

    /* Get length of TLV */
    if (p_rrc_s1u_ie_relay_setup_sap_req->bitmask & RRC_S1U_IE_SETUP_TEID_SELF_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_ie_teid_self_tlv_len(&p_rrc_s1u_ie_relay_setup_sap_req->teid_self);
    }

    /* Get length of TLV */
    if (p_rrc_s1u_ie_relay_setup_sap_req->bitmask & RRC_S1U_IE_SETUP_QOS_PROFILE_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_ie_qos_profile_tlv_len(&p_rrc_s1u_ie_relay_setup_sap_req->qos_profile);
    }

    /* Get length of TLV */
    if (p_rrc_s1u_ie_relay_setup_sap_req->bitmask & RRC_S1U_IE_SETUP_SEQ_NUM_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_ie_seq_num_tlv_len(&p_rrc_s1u_ie_relay_setup_sap_req->seq_num);
    }

    /* Get length of TLV */
    if (p_rrc_s1u_ie_relay_setup_sap_req->bitmask & RRC_S1U_IE_SETUP_SEQ_FLAG_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_ie_seq_disable_tlv_len(&p_rrc_s1u_ie_relay_setup_sap_req->seq_flag);
    }

    /* Get length of TLV */
    if (p_rrc_s1u_ie_relay_setup_sap_req->bitmask & RRC_S1U_IE_SETUP_REORDERING_REQD_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_reordering_reqd_tlv_len(&p_rrc_s1u_ie_relay_setup_sap_req->reordering_reqd);
    }

    /* Get length of TLV */
    if (p_rrc_s1u_ie_relay_setup_sap_req->bitmask & RRC_S1U_IE_SETUP_PDCP_EXT_FLAG_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_ie_pdcp_ext_tlv_len(&p_rrc_s1u_ie_relay_setup_sap_req->pdcp_ext);
    }

    /* Get length of TLV */
    if (p_rrc_s1u_ie_relay_setup_sap_req->bitmask & RRC_S1U_IE_SETUP_DC_BEARER_TYPE_PRESENT)
    {
        length += rrc_il_get_rrc_dc_bearer_type_tlv_len(&p_rrc_s1u_ie_relay_setup_sap_req->dc_bearer_type);
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_ie_relay_setup_sap_req->num_tunnel_info <= ARRSIZE(p_rrc_s1u_ie_relay_setup_sap_req->tunnel_info));
        for (loop = 0; loop < p_rrc_s1u_ie_relay_setup_sap_req->num_tunnel_info; loop++)
        {
            length += rrc_il_get_rrc_s1u_ie_tunnel_info_tlv_len(&p_rrc_s1u_ie_relay_setup_sap_req->tunnel_info[loop]);
        }
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_relay_setup_sap_req_tlv_len
(
    rrc_s1u_ie_relay_setup_sap_req_t *p_rrc_s1u_ie_relay_setup_sap_req

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_relay_setup_sap_req != PNULL);

    length += rrc_il_get_rrc_s1u_ie_relay_setup_sap_req_len(p_rrc_s1u_ie_relay_setup_sap_req);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_relay_setup_sap_req
(
    U8  **pp_buffer,
    rrc_s1u_ie_relay_setup_sap_req_t *p_rrc_s1u_ie_relay_setup_sap_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_ie_relay_setup_sap_req != PNULL);

    /* This function composes rrc_s1u_ie_relay_setup_sap_req */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_RELAY_SETUP_SAP_REQ_TAG;
    tlv_length = rrc_il_get_rrc_s1u_ie_relay_setup_sap_req_tlv_len(p_rrc_s1u_ie_relay_setup_sap_req);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_s1u_ie_relay_setup_sap_req->lc_id < 3) || (p_rrc_s1u_ie_relay_setup_sap_req->lc_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_relay_setup_sap_req->lc_id] should be in range "
            "3 to 10. Incorrect value %u received.", p_rrc_s1u_ie_relay_setup_sap_req->lc_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_ie_relay_setup_sap_req->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_s1u_ie_relay_setup_sap_req->lc_id);

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_s1u_ie_relay_setup_sap_req->qos_id, "qos_id");
    *pp_buffer += sizeof(p_rrc_s1u_ie_relay_setup_sap_req->qos_id);

    /* Compose TLV  */
    if (p_rrc_s1u_ie_relay_setup_sap_req->bitmask & RRC_S1U_IE_SETUP_DRBID_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_drb_id(pp_buffer, &p_rrc_s1u_ie_relay_setup_sap_req->drb_id))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_s1u_ie_relay_setup_sap_req->bitmask & RRC_S1U_IE_SETUP_BUFFER_IND_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_buffer_ind(pp_buffer, &p_rrc_s1u_ie_relay_setup_sap_req->buffer_ind))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_s1u_ie_relay_setup_sap_req->bitmask & RRC_S1U_IE_SETUP_TEID_SELF_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_teid_self(pp_buffer, &p_rrc_s1u_ie_relay_setup_sap_req->teid_self))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_s1u_ie_relay_setup_sap_req->bitmask & RRC_S1U_IE_SETUP_QOS_PROFILE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_qos_profile(pp_buffer, &p_rrc_s1u_ie_relay_setup_sap_req->qos_profile))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_s1u_ie_relay_setup_sap_req->bitmask & RRC_S1U_IE_SETUP_SEQ_NUM_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_seq_num(pp_buffer, &p_rrc_s1u_ie_relay_setup_sap_req->seq_num))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_s1u_ie_relay_setup_sap_req->bitmask & RRC_S1U_IE_SETUP_SEQ_FLAG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_seq_disable(pp_buffer, &p_rrc_s1u_ie_relay_setup_sap_req->seq_flag))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_s1u_ie_relay_setup_sap_req->bitmask & RRC_S1U_IE_SETUP_REORDERING_REQD_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_reordering_reqd(pp_buffer, &p_rrc_s1u_ie_relay_setup_sap_req->reordering_reqd))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_s1u_ie_relay_setup_sap_req->bitmask & RRC_S1U_IE_SETUP_PDCP_EXT_FLAG_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_pdcp_ext(pp_buffer, &p_rrc_s1u_ie_relay_setup_sap_req->pdcp_ext))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_s1u_ie_relay_setup_sap_req->bitmask & RRC_S1U_IE_SETUP_DC_BEARER_TYPE_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_dc_bearer_type(pp_buffer, &p_rrc_s1u_ie_relay_setup_sap_req->dc_bearer_type))
        {
            return RRC_FAILURE;
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_ie_relay_setup_sap_req->num_tunnel_info <= ARRSIZE(p_rrc_s1u_ie_relay_setup_sap_req->tunnel_info));
        for (loop = 0; loop < p_rrc_s1u_ie_relay_setup_sap_req->num_tunnel_info; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_tunnel_info(pp_buffer, &p_rrc_s1u_ie_relay_setup_sap_req->tunnel_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_release_tunnel_info_len
(
    rrc_release_tunnel_info_t *p_rrc_release_tunnel_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_release_tunnel_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_release_tunnel_info->teid_self);

    /* Get length of TLV */
    if (p_rrc_release_tunnel_info->bitmask & RRC_S1U_IE_RELEASE_TUNNEL_INFO_TRANSPORT_ADDR_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_ie_gsn_addr_tlv_len(&p_rrc_release_tunnel_info->transport_addr);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_release_tunnel_info_tlv_len
(
    rrc_release_tunnel_info_t *p_rrc_release_tunnel_info

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_release_tunnel_info != PNULL);

    length += rrc_il_get_rrc_release_tunnel_info_len(p_rrc_release_tunnel_info);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_release_tunnel_info
(
    U8  **pp_buffer,
    rrc_release_tunnel_info_t *p_rrc_release_tunnel_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_release_tunnel_info != PNULL);

    /* This function composes rrc_release_tunnel_info */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_RELEASE_TUNNEL_INFO_TAG;
    tlv_length = rrc_il_get_rrc_release_tunnel_info_tlv_len(p_rrc_release_tunnel_info);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_release_tunnel_info->teid_self < 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_release_tunnel_info->teid_self] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_release_tunnel_info->teid_self);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_release_tunnel_info->teid_self, "teid_self");
    *pp_buffer += sizeof(p_rrc_release_tunnel_info->teid_self);

    /* Compose TLV  */
    if (p_rrc_release_tunnel_info->bitmask & RRC_S1U_IE_RELEASE_TUNNEL_INFO_TRANSPORT_ADDR_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_gsn_addr(pp_buffer, &p_rrc_release_tunnel_info->transport_addr))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_relay_rel_sap_req_len
(
    rrc_s1u_ie_relay_rel_sap_req_t *p_rrc_s1u_ie_relay_rel_sap_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_relay_rel_sap_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_relay_rel_sap_req->lc_id);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_ie_relay_rel_sap_req->num_release_tunnel_info <= ARRSIZE(p_rrc_s1u_ie_relay_rel_sap_req->release_tunnel_info));
        for (loop = 0; loop < p_rrc_s1u_ie_relay_rel_sap_req->num_release_tunnel_info; loop++)
        {
            length += rrc_il_get_rrc_release_tunnel_info_tlv_len(&p_rrc_s1u_ie_relay_rel_sap_req->release_tunnel_info[loop]);
        }
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_relay_rel_sap_req_tlv_len
(
    rrc_s1u_ie_relay_rel_sap_req_t *p_rrc_s1u_ie_relay_rel_sap_req

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_relay_rel_sap_req != PNULL);

    length += rrc_il_get_rrc_s1u_ie_relay_rel_sap_req_len(p_rrc_s1u_ie_relay_rel_sap_req);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_relay_rel_sap_req
(
    U8  **pp_buffer,
    rrc_s1u_ie_relay_rel_sap_req_t *p_rrc_s1u_ie_relay_rel_sap_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_ie_relay_rel_sap_req != PNULL);

    /* This function composes rrc_s1u_ie_relay_rel_sap_req */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_RELAY_REL_SAP_REQ_TAG;
    tlv_length = rrc_il_get_rrc_s1u_ie_relay_rel_sap_req_tlv_len(p_rrc_s1u_ie_relay_rel_sap_req);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_ie_relay_rel_sap_req->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_s1u_ie_relay_rel_sap_req->lc_id);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_ie_relay_rel_sap_req->num_release_tunnel_info <= ARRSIZE(p_rrc_s1u_ie_relay_rel_sap_req->release_tunnel_info));
        for (loop = 0; loop < p_rrc_s1u_ie_relay_rel_sap_req->num_release_tunnel_info; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_release_tunnel_info(pp_buffer, &p_rrc_s1u_ie_relay_rel_sap_req->release_tunnel_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_s1u_reconfigure_ue_entity_cnf_len
(
    rrc_s1u_reconfigure_ue_entity_cnf_t *p_rrc_s1u_reconfigure_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_reconfigure_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_reconfigure_ue_entity_cnf->ue_index);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_reconfigure_ue_entity_cnf->num_setup_sap_cnf <= ARRSIZE(p_rrc_s1u_reconfigure_ue_entity_cnf->setup_sap_cnf));
        for (loop = 0; loop < p_rrc_s1u_reconfigure_ue_entity_cnf->num_setup_sap_cnf; loop++)
        {
            length += rrc_il_get_rrc_s1u_ie_relay_setup_sap_cnf_tlv_len(&p_rrc_s1u_reconfigure_ue_entity_cnf->setup_sap_cnf[loop]);
        }
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_reconfigure_ue_entity_cnf->num_rel_sap_cnf <= ARRSIZE(p_rrc_s1u_reconfigure_ue_entity_cnf->rel_sap_cnf));
        for (loop = 0; loop < p_rrc_s1u_reconfigure_ue_entity_cnf->num_rel_sap_cnf; loop++)
        {
            length += rrc_il_get_rrc_s1u_ie_relay_rel_sap_cnf_tlv_len(&p_rrc_s1u_reconfigure_ue_entity_cnf->rel_sap_cnf[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_s1u_reconfigure_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_s1u_reconfigure_ue_entity_cnf_t *p_rrc_s1u_reconfigure_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_reconfigure_ue_entity_cnf != PNULL);

    /* This function composes rrc_s1u_reconfigure_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_s1u_reconfigure_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_s1u_reconfigure_ue_entity_cnf->ue_index);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_reconfigure_ue_entity_cnf->num_setup_sap_cnf <= ARRSIZE(p_rrc_s1u_reconfigure_ue_entity_cnf->setup_sap_cnf));
        for (loop = 0; loop < p_rrc_s1u_reconfigure_ue_entity_cnf->num_setup_sap_cnf; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_relay_setup_sap_cnf(pp_buffer, &p_rrc_s1u_reconfigure_ue_entity_cnf->setup_sap_cnf[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_reconfigure_ue_entity_cnf->num_rel_sap_cnf <= ARRSIZE(p_rrc_s1u_reconfigure_ue_entity_cnf->rel_sap_cnf));
        for (loop = 0; loop < p_rrc_s1u_reconfigure_ue_entity_cnf->num_rel_sap_cnf; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_relay_rel_sap_cnf(pp_buffer, &p_rrc_s1u_reconfigure_ue_entity_cnf->rel_sap_cnf[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1u_il_send_rrc_s1u_reconfigure_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_S1U_RECONFIGURE_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1u_il_send_rrc_s1u_reconfigure_ue_entity_cnf
(
    rrc_s1u_reconfigure_ue_entity_cnf_t  *p_rrc_s1u_reconfigure_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1u_msg;

    RRC_ASSERT(p_rrc_s1u_reconfigure_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_s1u_reconfigure_ue_entity_cnf_len(p_rrc_s1u_reconfigure_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_S1U_RECONFIGURE_UE_ENTITY_CNF", src_module_id, dst_module_id);

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
    p_s1u_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1u_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_S1U_RECONFIGURE_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_s1u_msg = p_s1u_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1u_msg, transaction_id, src_module_id,
        RRC_S1U_MODULE_ID, RRC_S1U_RECONFIGURE_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_S1U_RECONFIGURE_UE_ENTITY_CNF message */
    p_s1u_msg = p_s1u_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_s1u_reconfigure_ue_entity_cnf(&p_s1u_msg, p_rrc_s1u_reconfigure_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1u_msg - msg_api_length) != p_msg)
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
rrc_il_get_rrc_s1u_ie_tunnel_info_cnf_len
(
    rrc_s1u_ie_tunnel_info_cnf_t *p_rrc_s1u_ie_tunnel_info_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_tunnel_info_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_tunnel_info_cnf->cause);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_tunnel_info_cnf->rb_direction);

    /* Get length of TLV */
    if (p_rrc_s1u_ie_tunnel_info_cnf->bitmask & RRC_S1U_IE_TUNNEL_INFO_CNF_TEID_SELF_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_ie_teid_self_tlv_len(&p_rrc_s1u_ie_tunnel_info_cnf->teid_self);
    }

    /* Get length of TLV */
    if (p_rrc_s1u_ie_tunnel_info_cnf->bitmask & RRC_S1U_IE_TUNNEL_INFO_CNF_TEID_PEER_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_ie_teid_peer_tlv_len(&p_rrc_s1u_ie_tunnel_info_cnf->teid_peer);
    }

    /* Get length of TLV */
    if (p_rrc_s1u_ie_tunnel_info_cnf->bitmask & RRC_S1U_IE_TUNNEL_INFO_CNF_TRANSPORT_ADDR_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_ie_gsn_addr_tlv_len(&p_rrc_s1u_ie_tunnel_info_cnf->transport_addr);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_tunnel_info_cnf_tlv_len
(
    rrc_s1u_ie_tunnel_info_cnf_t *p_rrc_s1u_ie_tunnel_info_cnf

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_tunnel_info_cnf != PNULL);

    length += rrc_il_get_rrc_s1u_ie_tunnel_info_cnf_len(p_rrc_s1u_ie_tunnel_info_cnf);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_tunnel_info_cnf
(
    U8  **pp_buffer,
    rrc_s1u_ie_tunnel_info_cnf_t *p_rrc_s1u_ie_tunnel_info_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_ie_tunnel_info_cnf != PNULL);

    /* This function composes rrc_s1u_ie_tunnel_info_cnf */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_TUNNEL_INFO_CNF_TAG;
    tlv_length = rrc_il_get_rrc_s1u_ie_tunnel_info_cnf_tlv_len(p_rrc_s1u_ie_tunnel_info_cnf);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_ie_tunnel_info_cnf->cause > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_tunnel_info_cnf->cause] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rrc_s1u_ie_tunnel_info_cnf->cause);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_ie_tunnel_info_cnf->cause, "cause");
    *pp_buffer += sizeof(p_rrc_s1u_ie_tunnel_info_cnf->cause);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_ie_tunnel_info_cnf->rb_direction > 2))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_tunnel_info_cnf->rb_direction] should be less than"
            " or equal to 2. Incorrect value %u received.", p_rrc_s1u_ie_tunnel_info_cnf->rb_direction);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_s1u_ie_tunnel_info_cnf->rb_direction, "rb_direction");
    *pp_buffer += sizeof(p_rrc_s1u_ie_tunnel_info_cnf->rb_direction);

    /* Compose TLV  */
    if (p_rrc_s1u_ie_tunnel_info_cnf->bitmask & RRC_S1U_IE_TUNNEL_INFO_CNF_TEID_SELF_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_teid_self(pp_buffer, &p_rrc_s1u_ie_tunnel_info_cnf->teid_self))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_s1u_ie_tunnel_info_cnf->bitmask & RRC_S1U_IE_TUNNEL_INFO_CNF_TEID_PEER_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_teid_peer(pp_buffer, &p_rrc_s1u_ie_tunnel_info_cnf->teid_peer))
        {
            return RRC_FAILURE;
        }
    }

    /* Compose TLV  */
    if (p_rrc_s1u_ie_tunnel_info_cnf->bitmask & RRC_S1U_IE_TUNNEL_INFO_CNF_TRANSPORT_ADDR_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_gsn_addr(pp_buffer, &p_rrc_s1u_ie_tunnel_info_cnf->transport_addr))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_relay_setup_sap_cnf_len
(
    rrc_s1u_ie_relay_setup_sap_cnf_t *p_rrc_s1u_ie_relay_setup_sap_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_relay_setup_sap_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_relay_setup_sap_cnf->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_relay_setup_sap_cnf->cause);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_ie_relay_setup_sap_cnf->num_sap_cnf <= ARRSIZE(p_rrc_s1u_ie_relay_setup_sap_cnf->tunnel_info_cnf));
        for (loop = 0; loop < p_rrc_s1u_ie_relay_setup_sap_cnf->num_sap_cnf; loop++)
        {
            length += rrc_il_get_rrc_s1u_ie_tunnel_info_cnf_tlv_len(&p_rrc_s1u_ie_relay_setup_sap_cnf->tunnel_info_cnf[loop]);
        }
    }

    /* Get length of TLV */
    if (p_rrc_s1u_ie_relay_setup_sap_cnf->bitmask & RRC_S1U_IE_SETUP_SAP_CNF_SEQ_NUM_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_ie_seq_num_tlv_len(&p_rrc_s1u_ie_relay_setup_sap_cnf->seq_num);
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_relay_setup_sap_cnf_tlv_len
(
    rrc_s1u_ie_relay_setup_sap_cnf_t *p_rrc_s1u_ie_relay_setup_sap_cnf

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_relay_setup_sap_cnf != PNULL);

    length += rrc_il_get_rrc_s1u_ie_relay_setup_sap_cnf_len(p_rrc_s1u_ie_relay_setup_sap_cnf);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_relay_setup_sap_cnf
(
    U8  **pp_buffer,
    rrc_s1u_ie_relay_setup_sap_cnf_t *p_rrc_s1u_ie_relay_setup_sap_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_ie_relay_setup_sap_cnf != PNULL);

    /* This function composes rrc_s1u_ie_relay_setup_sap_cnf */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_RELAY_SETUP_SAP_CNF_TAG;
    tlv_length = rrc_il_get_rrc_s1u_ie_relay_setup_sap_cnf_tlv_len(p_rrc_s1u_ie_relay_setup_sap_cnf);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_s1u_ie_relay_setup_sap_cnf->lc_id < 3) || (p_rrc_s1u_ie_relay_setup_sap_cnf->lc_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_relay_setup_sap_cnf->lc_id] should be in range "
            "3 to 10. Incorrect value %u received.", p_rrc_s1u_ie_relay_setup_sap_cnf->lc_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_ie_relay_setup_sap_cnf->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_s1u_ie_relay_setup_sap_cnf->lc_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_ie_relay_setup_sap_cnf->cause > EGTPU_PEER_ADDRESS_MISSING))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_relay_setup_sap_cnf->cause] should be less than"
            " or equal to EGTPU_PEER_ADDRESS_MISSING. Incorrect value %u received.", p_rrc_s1u_ie_relay_setup_sap_cnf->cause);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_ie_relay_setup_sap_cnf->cause, "cause");
    *pp_buffer += sizeof(p_rrc_s1u_ie_relay_setup_sap_cnf->cause);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_ie_relay_setup_sap_cnf->num_sap_cnf <= ARRSIZE(p_rrc_s1u_ie_relay_setup_sap_cnf->tunnel_info_cnf));
        for (loop = 0; loop < p_rrc_s1u_ie_relay_setup_sap_cnf->num_sap_cnf; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_tunnel_info_cnf(pp_buffer, &p_rrc_s1u_ie_relay_setup_sap_cnf->tunnel_info_cnf[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    /* Compose TLV  */
    if (p_rrc_s1u_ie_relay_setup_sap_cnf->bitmask & RRC_S1U_IE_SETUP_SAP_CNF_SEQ_NUM_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_seq_num(pp_buffer, &p_rrc_s1u_ie_relay_setup_sap_cnf->seq_num))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_relay_rel_sap_cnf_len
(
    rrc_s1u_ie_relay_rel_sap_cnf_t *p_rrc_s1u_ie_relay_rel_sap_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_relay_rel_sap_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_relay_rel_sap_cnf->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_ie_relay_rel_sap_cnf->cause);

    /* Get length of TLV */
    if (p_rrc_s1u_ie_relay_rel_sap_cnf->bitmask & RRC_S1U_IE_REL_CNF_SEQ_NUM_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_ie_seq_num_tlv_len(&p_rrc_s1u_ie_relay_rel_sap_cnf->seq_num);
    }
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_ie_relay_rel_sap_cnf->num_release_tunnel_info <= ARRSIZE(p_rrc_s1u_ie_relay_rel_sap_cnf->release_tunnel_info));
        for (loop = 0; loop < p_rrc_s1u_ie_relay_rel_sap_cnf->num_release_tunnel_info; loop++)
        {
            length += rrc_il_get_rrc_release_tunnel_info_tlv_len(&p_rrc_s1u_ie_relay_rel_sap_cnf->release_tunnel_info[loop]);
        }
    }

    return length;
}

static
rrc_length_t
rrc_il_get_rrc_s1u_ie_relay_rel_sap_cnf_tlv_len
(
    rrc_s1u_ie_relay_rel_sap_cnf_t *p_rrc_s1u_ie_relay_rel_sap_cnf

)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_ie_relay_rel_sap_cnf != PNULL);

    length += rrc_il_get_rrc_s1u_ie_relay_rel_sap_cnf_len(p_rrc_s1u_ie_relay_rel_sap_cnf);
    length += (rrc_length_t)sizeof(rrc_tag_t) + (rrc_length_t)sizeof(rrc_length_t);

    return length;
}

static
rrc_return_et
rrc_il_compose_rrc_s1u_ie_relay_rel_sap_cnf
(
    U8  **pp_buffer,
    rrc_s1u_ie_relay_rel_sap_cnf_t *p_rrc_s1u_ie_relay_rel_sap_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_ie_relay_rel_sap_cnf != PNULL);

    /* This function composes rrc_s1u_ie_relay_rel_sap_cnf */
    


    rrc_tag_t tag = RRC_NULL;
    rrc_length_t tlv_length = RRC_NULL;

    /* Compose TL of TLV */
    tag = RRC_S1U_IE_RELAY_REL_SAP_CNF_TAG;
    tlv_length = rrc_il_get_rrc_s1u_ie_relay_rel_sap_cnf_tlv_len(p_rrc_s1u_ie_relay_rel_sap_cnf);

    rrc_cp_pack_U16(*pp_buffer, &tag, "<tag>");
    *pp_buffer += sizeof(rrc_tag_t);

    rrc_cp_pack_U16(*pp_buffer, &tlv_length, "<tlv_length>");
    *pp_buffer += sizeof(rrc_length_t);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_s1u_ie_relay_rel_sap_cnf->lc_id < 3) || (p_rrc_s1u_ie_relay_rel_sap_cnf->lc_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_relay_rel_sap_cnf->lc_id] should be in range "
            "3 to 10. Incorrect value %u received.", p_rrc_s1u_ie_relay_rel_sap_cnf->lc_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_ie_relay_rel_sap_cnf->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_s1u_ie_relay_rel_sap_cnf->lc_id);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_ie_relay_rel_sap_cnf->cause > 5))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_ie_relay_rel_sap_cnf->cause] should be less than"
            " or equal to 5. Incorrect value %u received.", p_rrc_s1u_ie_relay_rel_sap_cnf->cause);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_ie_relay_rel_sap_cnf->cause, "cause");
    *pp_buffer += sizeof(p_rrc_s1u_ie_relay_rel_sap_cnf->cause);

    /* Compose TLV  */
    if (p_rrc_s1u_ie_relay_rel_sap_cnf->bitmask & RRC_S1U_IE_REL_CNF_SEQ_NUM_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_seq_num(pp_buffer, &p_rrc_s1u_ie_relay_rel_sap_cnf->seq_num))
        {
            return RRC_FAILURE;
        }
    }
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_ie_relay_rel_sap_cnf->num_release_tunnel_info <= ARRSIZE(p_rrc_s1u_ie_relay_rel_sap_cnf->release_tunnel_info));
        for (loop = 0; loop < p_rrc_s1u_ie_relay_rel_sap_cnf->num_release_tunnel_info; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_release_tunnel_info(pp_buffer, &p_rrc_s1u_ie_relay_rel_sap_cnf->release_tunnel_info[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_s1u_create_ue_entity_req_len
(
    rrc_s1u_create_ue_entity_req_t *p_rrc_s1u_create_ue_entity_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_create_ue_entity_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_create_ue_entity_req->ue_index);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_create_ue_entity_req->num_setup_sap_req <= ARRSIZE(p_rrc_s1u_create_ue_entity_req->setup_sap_req));
        for (loop = 0; loop < p_rrc_s1u_create_ue_entity_req->num_setup_sap_req; loop++)
        {
            length += rrc_il_get_rrc_s1u_ie_relay_setup_sap_req_tlv_len(&p_rrc_s1u_create_ue_entity_req->setup_sap_req[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_s1u_create_ue_entity_req
(
    U8  **pp_buffer,
    rrc_s1u_create_ue_entity_req_t *p_rrc_s1u_create_ue_entity_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_create_ue_entity_req != PNULL);

    /* This function composes rrc_s1u_create_ue_entity_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_s1u_create_ue_entity_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_s1u_create_ue_entity_req->ue_index);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_create_ue_entity_req->num_setup_sap_req <= ARRSIZE(p_rrc_s1u_create_ue_entity_req->setup_sap_req));
        for (loop = 0; loop < p_rrc_s1u_create_ue_entity_req->num_setup_sap_req; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_relay_setup_sap_req(pp_buffer, &p_rrc_s1u_create_ue_entity_req->setup_sap_req[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1u_il_send_rrc_s1u_create_ue_entity_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_S1U_CREATE_UE_ENTITY_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1u_il_send_rrc_s1u_create_ue_entity_req
(
    rrc_s1u_create_ue_entity_req_t  *p_rrc_s1u_create_ue_entity_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1u_msg;

    RRC_ASSERT(p_rrc_s1u_create_ue_entity_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_s1u_create_ue_entity_req_len(p_rrc_s1u_create_ue_entity_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_S1U_CREATE_UE_ENTITY_REQ", src_module_id, dst_module_id);

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
    p_s1u_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1u_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_S1U_CREATE_UE_ENTITY_REQ, msg_api_length);

    /* Fill interface header */
    p_s1u_msg = p_s1u_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1u_msg, transaction_id, src_module_id,
        RRC_S1U_MODULE_ID, RRC_S1U_CREATE_UE_ENTITY_REQ, msg_length,cell_index);

    /* Fill RRC_S1U_CREATE_UE_ENTITY_REQ message */
    p_s1u_msg = p_s1u_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_s1u_create_ue_entity_req(&p_s1u_msg, p_rrc_s1u_create_ue_entity_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1u_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_s1u_create_ue_entity_cnf_len
(
    rrc_s1u_create_ue_entity_cnf_t *p_rrc_s1u_create_ue_entity_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_create_ue_entity_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_create_ue_entity_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_create_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Get length of TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_create_ue_entity_cnf->num_setup_sap_cnf <= ARRSIZE(p_rrc_s1u_create_ue_entity_cnf->setup_sap_cnf));
        for (loop = 0; loop < p_rrc_s1u_create_ue_entity_cnf->num_setup_sap_cnf; loop++)
        {
            length += rrc_il_get_rrc_s1u_ie_relay_setup_sap_cnf_tlv_len(&p_rrc_s1u_create_ue_entity_cnf->setup_sap_cnf[loop]);
        }
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_s1u_create_ue_entity_cnf
(
    U8  **pp_buffer,
    rrc_s1u_create_ue_entity_cnf_t *p_rrc_s1u_create_ue_entity_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_create_ue_entity_cnf != PNULL);

    /* This function composes rrc_s1u_create_ue_entity_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_s1u_create_ue_entity_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_s1u_create_ue_entity_cnf->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_create_ue_entity_cnf->response_code, "response_code");
    *pp_buffer += sizeof(p_rrc_s1u_create_ue_entity_cnf->response_code);
    {
        rrc_counter_t loop;
        /* Compose TLV SEQUENCE */
        RRC_ASSERT(p_rrc_s1u_create_ue_entity_cnf->num_setup_sap_cnf <= ARRSIZE(p_rrc_s1u_create_ue_entity_cnf->setup_sap_cnf));
        for (loop = 0; loop < p_rrc_s1u_create_ue_entity_cnf->num_setup_sap_cnf; loop++)
        {
            if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_relay_setup_sap_cnf(pp_buffer, &p_rrc_s1u_create_ue_entity_cnf->setup_sap_cnf[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1u_il_send_rrc_s1u_create_ue_entity_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_S1U_CREATE_UE_ENTITY_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1u_il_send_rrc_s1u_create_ue_entity_cnf
(
    rrc_s1u_create_ue_entity_cnf_t  *p_rrc_s1u_create_ue_entity_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1u_msg;

    RRC_ASSERT(p_rrc_s1u_create_ue_entity_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_s1u_create_ue_entity_cnf_len(p_rrc_s1u_create_ue_entity_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_S1U_CREATE_UE_ENTITY_CNF", src_module_id, dst_module_id);

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
    p_s1u_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1u_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_S1U_CREATE_UE_ENTITY_CNF, msg_api_length);

    /* Fill interface header */
    p_s1u_msg = p_s1u_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1u_msg, transaction_id, src_module_id,
        RRC_S1U_MODULE_ID, RRC_S1U_CREATE_UE_ENTITY_CNF, msg_length,cell_index);

    /* Fill RRC_S1U_CREATE_UE_ENTITY_CNF message */
    p_s1u_msg = p_s1u_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_s1u_create_ue_entity_cnf(&p_s1u_msg, p_rrc_s1u_create_ue_entity_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1u_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_s1u_error_ind_len
(
    rrc_s1u_error_ind_t *p_rrc_s1u_error_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_error_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_error_ind->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_error_ind->lc_id);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_s1u_error_ind
(
    U8  **pp_buffer,
    rrc_s1u_error_ind_t *p_rrc_s1u_error_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_error_ind != PNULL);

    /* This function composes rrc_s1u_error_ind */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_s1u_error_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_s1u_error_ind->ue_index);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_s1u_error_ind->lc_id < 3) || (p_rrc_s1u_error_ind->lc_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_error_ind->lc_id] should be in range "
            "3 to 10. Incorrect value %u received.", p_rrc_s1u_error_ind->lc_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_error_ind->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_s1u_error_ind->lc_id);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1u_il_send_rrc_s1u_error_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_S1U_ERROR_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1u_il_send_rrc_s1u_error_ind
(
    rrc_s1u_error_ind_t  *p_rrc_s1u_error_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1u_msg;

    RRC_ASSERT(p_rrc_s1u_error_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_s1u_error_ind_len(p_rrc_s1u_error_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_S1U_ERROR_IND", src_module_id, dst_module_id);

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
    p_s1u_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1u_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_S1U_ERROR_IND, msg_api_length);

    /* Fill interface header */
    p_s1u_msg = p_s1u_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1u_msg, transaction_id, src_module_id,
        RRC_S1U_MODULE_ID, RRC_S1U_ERROR_IND, msg_length,cell_index);

    /* Fill RRC_S1U_ERROR_IND message */
    p_s1u_msg = p_s1u_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_s1u_error_ind(&p_s1u_msg, p_rrc_s1u_error_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1u_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_s1u_path_failure_ind_len
(
    rrc_s1u_path_failure_ind_t *p_rrc_s1u_path_failure_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_path_failure_ind != PNULL);

    /* Get length of TLV */
    if (p_rrc_s1u_path_failure_ind->bitmask & RRC_TRANSPORT_ADDR_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_ie_gsn_addr_tlv_len(&p_rrc_s1u_path_failure_ind->transport_addr);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_s1u_path_failure_ind
(
    U8  **pp_buffer,
    rrc_s1u_path_failure_ind_t *p_rrc_s1u_path_failure_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_path_failure_ind != PNULL);

    /* This function composes rrc_s1u_path_failure_ind */
    

    /* Compose TLV  */
    if (p_rrc_s1u_path_failure_ind->bitmask & RRC_TRANSPORT_ADDR_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_gsn_addr(pp_buffer, &p_rrc_s1u_path_failure_ind->transport_addr))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1u_il_send_rrc_s1u_path_failure_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_S1U_PATH_FAILURE_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1u_il_send_rrc_s1u_path_failure_ind
(
    rrc_s1u_path_failure_ind_t  *p_rrc_s1u_path_failure_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1u_msg;

    RRC_ASSERT(p_rrc_s1u_path_failure_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_s1u_path_failure_ind_len(p_rrc_s1u_path_failure_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_S1U_PATH_FAILURE_IND", src_module_id, dst_module_id);

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
    p_s1u_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1u_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_S1U_PATH_FAILURE_IND, msg_api_length);

    /* Fill interface header */
    p_s1u_msg = p_s1u_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1u_msg, transaction_id, src_module_id,
        RRC_S1U_MODULE_ID, RRC_S1U_PATH_FAILURE_IND, msg_length,cell_index);

    /* Fill RRC_S1U_PATH_FAILURE_IND message */
    p_s1u_msg = p_s1u_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_s1u_path_failure_ind(&p_s1u_msg, p_rrc_s1u_path_failure_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1u_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_s1u_end_marker_ind_len
(
    rrc_s1u_end_marker_ind_t *p_rrc_s1u_end_marker_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_end_marker_ind != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_end_marker_ind->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_end_marker_ind->lc_id);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_end_marker_ind->teid_self);

    /* Get length of TLV */
    if (p_rrc_s1u_end_marker_ind->bitmask & RRC_S1U_END_MARKER_IND_TRANSPORT_ADDR_PRESENT)
    {
        length += rrc_il_get_rrc_s1u_ie_gsn_addr_tlv_len(&p_rrc_s1u_end_marker_ind->transport_addr);
    }

    return length;
}

rrc_return_et
rrc_il_compose_rrc_s1u_end_marker_ind
(
    U8  **pp_buffer,
    rrc_s1u_end_marker_ind_t *p_rrc_s1u_end_marker_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_end_marker_ind != PNULL);

    /* This function composes rrc_s1u_end_marker_ind */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_s1u_end_marker_ind->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_s1u_end_marker_ind->ue_index);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_rrc_s1u_end_marker_ind->lc_id < 3) || (p_rrc_s1u_end_marker_ind->lc_id > 10))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_end_marker_ind->lc_id] should be in range "
            "3 to 10. Incorrect value %u received.", p_rrc_s1u_end_marker_ind->lc_id);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_end_marker_ind->lc_id, "lc_id");
    *pp_buffer += sizeof(p_rrc_s1u_end_marker_ind->lc_id);

    /* Check for correct range [L - lower boundary] */
    if ((p_rrc_s1u_end_marker_ind->teid_self < 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_end_marker_ind->teid_self] should be greater than"
            " or equal to 1. Incorrect value %u received.", p_rrc_s1u_end_marker_ind->teid_self);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U32(*pp_buffer, &p_rrc_s1u_end_marker_ind->teid_self, "teid_self");
    *pp_buffer += sizeof(p_rrc_s1u_end_marker_ind->teid_self);

    /* Compose TLV  */
    if (p_rrc_s1u_end_marker_ind->bitmask & RRC_S1U_END_MARKER_IND_TRANSPORT_ADDR_PRESENT)
    {
        if (RRC_FAILURE == rrc_il_compose_rrc_s1u_ie_gsn_addr(pp_buffer, &p_rrc_s1u_end_marker_ind->transport_addr))
        {
            return RRC_FAILURE;
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1u_il_send_rrc_s1u_end_marker_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_S1U_END_MARKER_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1u_il_send_rrc_s1u_end_marker_ind
(
    rrc_s1u_end_marker_ind_t  *p_rrc_s1u_end_marker_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1u_msg;

    RRC_ASSERT(p_rrc_s1u_end_marker_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_s1u_end_marker_ind_len(p_rrc_s1u_end_marker_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_S1U_END_MARKER_IND", src_module_id, dst_module_id);

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
    p_s1u_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1u_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_S1U_END_MARKER_IND, msg_api_length);

    /* Fill interface header */
    p_s1u_msg = p_s1u_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1u_msg, transaction_id, src_module_id,
        RRC_S1U_MODULE_ID, RRC_S1U_END_MARKER_IND, msg_length,cell_index);

    /* Fill RRC_S1U_END_MARKER_IND message */
    p_s1u_msg = p_s1u_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_s1u_end_marker_ind(&p_s1u_msg, p_rrc_s1u_end_marker_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1u_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_s1u_path_success_ind_len
(
    rrc_s1u_path_success_ind_t *p_rrc_s1u_path_success_ind
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_path_success_ind != PNULL);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_s1u_path_success_ind
(
    U8  **pp_buffer,
    rrc_s1u_path_success_ind_t *p_rrc_s1u_path_success_ind
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_path_success_ind != PNULL);

    /* This function composes rrc_s1u_path_success_ind */
    

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1u_il_send_rrc_s1u_path_success_ind
*
*   DESCRIPTION:
*       This function constructs and sends RRC_S1U_PATH_SUCCESS_IND message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1u_il_send_rrc_s1u_path_success_ind
(
    rrc_s1u_path_success_ind_t  *p_rrc_s1u_path_success_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1u_msg;

    RRC_ASSERT(p_rrc_s1u_path_success_ind != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_s1u_path_success_ind_len(p_rrc_s1u_path_success_ind);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_S1U_PATH_SUCCESS_IND", src_module_id, dst_module_id);

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
    p_s1u_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1u_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_S1U_PATH_SUCCESS_IND, msg_api_length);

    /* Fill interface header */
    p_s1u_msg = p_s1u_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1u_msg, transaction_id, src_module_id,
        RRC_S1U_MODULE_ID, RRC_S1U_PATH_SUCCESS_IND, msg_length,cell_index);

    /* Fill RRC_S1U_PATH_SUCCESS_IND message */
    p_s1u_msg = p_s1u_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_s1u_path_success_ind(&p_s1u_msg, p_rrc_s1u_path_success_ind))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1u_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_s1u_intra_enb_data_fwd_req_len
(
    rrc_s1u_intra_enb_data_fwd_req_t *p_rrc_s1u_intra_enb_data_fwd_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_intra_enb_data_fwd_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_intra_enb_data_fwd_req->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_intra_enb_data_fwd_req->target_ue_index);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_s1u_intra_enb_data_fwd_req
(
    U8  **pp_buffer,
    rrc_s1u_intra_enb_data_fwd_req_t *p_rrc_s1u_intra_enb_data_fwd_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_intra_enb_data_fwd_req != PNULL);

    /* This function composes rrc_s1u_intra_enb_data_fwd_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_s1u_intra_enb_data_fwd_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_s1u_intra_enb_data_fwd_req->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_s1u_intra_enb_data_fwd_req->target_ue_index, "target_ue_index");
    *pp_buffer += sizeof(p_rrc_s1u_intra_enb_data_fwd_req->target_ue_index);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1u_il_send_rrc_s1u_intra_enb_data_fwd_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_S1U_INTRA_ENB_DATA_FWD_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1u_il_send_rrc_s1u_intra_enb_data_fwd_req
(
    rrc_s1u_intra_enb_data_fwd_req_t  *p_rrc_s1u_intra_enb_data_fwd_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1u_msg;

    RRC_ASSERT(p_rrc_s1u_intra_enb_data_fwd_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_s1u_intra_enb_data_fwd_req_len(p_rrc_s1u_intra_enb_data_fwd_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_S1U_INTRA_ENB_DATA_FWD_REQ", src_module_id, dst_module_id);

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
    p_s1u_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1u_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_S1U_INTRA_ENB_DATA_FWD_REQ, msg_api_length);

    /* Fill interface header */
    p_s1u_msg = p_s1u_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1u_msg, transaction_id, src_module_id,
        RRC_S1U_MODULE_ID, RRC_S1U_INTRA_ENB_DATA_FWD_REQ, msg_length,cell_index);

    /* Fill RRC_S1U_INTRA_ENB_DATA_FWD_REQ message */
    p_s1u_msg = p_s1u_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_s1u_intra_enb_data_fwd_req(&p_s1u_msg, p_rrc_s1u_intra_enb_data_fwd_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1u_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_s1u_intra_enb_data_fwd_cnf_len
(
    rrc_s1u_intra_enb_data_fwd_cnf_t *p_rrc_s1u_intra_enb_data_fwd_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_intra_enb_data_fwd_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_intra_enb_data_fwd_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_intra_enb_data_fwd_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_s1u_intra_enb_data_fwd_cnf
(
    U8  **pp_buffer,
    rrc_s1u_intra_enb_data_fwd_cnf_t *p_rrc_s1u_intra_enb_data_fwd_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_intra_enb_data_fwd_cnf != PNULL);

    /* This function composes rrc_s1u_intra_enb_data_fwd_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_s1u_intra_enb_data_fwd_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_s1u_intra_enb_data_fwd_cnf->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_intra_enb_data_fwd_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_s1u_intra_enb_data_fwd_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1u_il_send_rrc_s1u_intra_enb_data_fwd_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_S1U_INTRA_ENB_DATA_FWD_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1u_il_send_rrc_s1u_intra_enb_data_fwd_cnf
(
    rrc_s1u_intra_enb_data_fwd_cnf_t  *p_rrc_s1u_intra_enb_data_fwd_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1u_msg;

    RRC_ASSERT(p_rrc_s1u_intra_enb_data_fwd_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_s1u_intra_enb_data_fwd_cnf_len(p_rrc_s1u_intra_enb_data_fwd_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_S1U_INTRA_ENB_DATA_FWD_CNF", src_module_id, dst_module_id);

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
    p_s1u_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1u_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_S1U_INTRA_ENB_DATA_FWD_CNF, msg_api_length);

    /* Fill interface header */
    p_s1u_msg = p_s1u_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1u_msg, transaction_id, src_module_id,
        RRC_S1U_MODULE_ID, RRC_S1U_INTRA_ENB_DATA_FWD_CNF, msg_length,cell_index);

    /* Fill RRC_S1U_INTRA_ENB_DATA_FWD_CNF message */
    p_s1u_msg = p_s1u_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_s1u_intra_enb_data_fwd_cnf(&p_s1u_msg, p_rrc_s1u_intra_enb_data_fwd_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1u_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_s1u_local_path_switch_req_len
(
    rrc_s1u_local_path_switch_req_t *p_rrc_s1u_local_path_switch_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_local_path_switch_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_local_path_switch_req->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_local_path_switch_req->ue_index_for_path_switch);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_s1u_local_path_switch_req
(
    U8  **pp_buffer,
    rrc_s1u_local_path_switch_req_t *p_rrc_s1u_local_path_switch_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_local_path_switch_req != PNULL);

    /* This function composes rrc_s1u_local_path_switch_req */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_s1u_local_path_switch_req->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_s1u_local_path_switch_req->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_s1u_local_path_switch_req->ue_index_for_path_switch, "ue_index_for_path_switch");
    *pp_buffer += sizeof(p_rrc_s1u_local_path_switch_req->ue_index_for_path_switch);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1u_il_send_rrc_s1u_local_path_switch_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_S1U_LOCAL_PATH_SWITCH_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1u_il_send_rrc_s1u_local_path_switch_req
(
    rrc_s1u_local_path_switch_req_t  *p_rrc_s1u_local_path_switch_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1u_msg;

    RRC_ASSERT(p_rrc_s1u_local_path_switch_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_s1u_local_path_switch_req_len(p_rrc_s1u_local_path_switch_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_S1U_LOCAL_PATH_SWITCH_REQ", src_module_id, dst_module_id);

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
    p_s1u_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1u_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_S1U_LOCAL_PATH_SWITCH_REQ, msg_api_length);

    /* Fill interface header */
    p_s1u_msg = p_s1u_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1u_msg, transaction_id, src_module_id,
        RRC_S1U_MODULE_ID, RRC_S1U_LOCAL_PATH_SWITCH_REQ, msg_length,cell_index);

    /* Fill RRC_S1U_LOCAL_PATH_SWITCH_REQ message */
    p_s1u_msg = p_s1u_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_s1u_local_path_switch_req(&p_s1u_msg, p_rrc_s1u_local_path_switch_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1u_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_s1u_local_path_switch_cnf_len
(
    rrc_s1u_local_path_switch_cnf_t *p_rrc_s1u_local_path_switch_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_local_path_switch_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_local_path_switch_cnf->ue_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_local_path_switch_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_s1u_local_path_switch_cnf
(
    U8  **pp_buffer,
    rrc_s1u_local_path_switch_cnf_t *p_rrc_s1u_local_path_switch_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_local_path_switch_cnf != PNULL);

    /* This function composes rrc_s1u_local_path_switch_cnf */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_s1u_local_path_switch_cnf->ue_index, "ue_index");
    *pp_buffer += sizeof(p_rrc_s1u_local_path_switch_cnf->ue_index);

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_local_path_switch_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_s1u_local_path_switch_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1u_il_send_rrc_s1u_local_path_switch_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_S1U_LOCAL_PATH_SWITCH_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1u_il_send_rrc_s1u_local_path_switch_cnf
(
    rrc_s1u_local_path_switch_cnf_t  *p_rrc_s1u_local_path_switch_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1u_msg;

    RRC_ASSERT(p_rrc_s1u_local_path_switch_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_s1u_local_path_switch_cnf_len(p_rrc_s1u_local_path_switch_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_S1U_LOCAL_PATH_SWITCH_CNF", src_module_id, dst_module_id);

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
    p_s1u_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1u_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_S1U_LOCAL_PATH_SWITCH_CNF, msg_api_length);

    /* Fill interface header */
    p_s1u_msg = p_s1u_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1u_msg, transaction_id, src_module_id,
        RRC_S1U_MODULE_ID, RRC_S1U_LOCAL_PATH_SWITCH_CNF, msg_length,cell_index);

    /* Fill RRC_S1U_LOCAL_PATH_SWITCH_CNF message */
    p_s1u_msg = p_s1u_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_s1u_local_path_switch_cnf(&p_s1u_msg, p_rrc_s1u_local_path_switch_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1u_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_s1u_config_cell_req_len
(
    rrc_s1u_config_cell_req_t *p_rrc_s1u_config_cell_req
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_config_cell_req != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_config_cell_req->Cell_index);

    /* Get length of IE */
    length += rrc_il_get_s1u_communication_info_len(&p_rrc_s1u_config_cell_req->s1u_communication_info);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_s1u_config_cell_req
(
    U8  **pp_buffer,
    rrc_s1u_config_cell_req_t *p_rrc_s1u_config_cell_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_config_cell_req != PNULL);

    /* This function composes rrc_s1u_config_cell_req */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_config_cell_req->Cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_config_cell_req->Cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_s1u_config_cell_req->Cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_rrc_s1u_config_cell_req->Cell_index, "Cell_index");
    *pp_buffer += sizeof(p_rrc_s1u_config_cell_req->Cell_index);

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_s1u_communication_info(pp_buffer, &p_rrc_s1u_config_cell_req->s1u_communication_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1u_il_send_rrc_s1u_config_cell_req
*
*   DESCRIPTION:
*       This function constructs and sends RRC_S1U_CELL_CONFIG_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1u_il_send_rrc_s1u_config_cell_req
(
    rrc_s1u_config_cell_req_t  *p_rrc_s1u_config_cell_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1u_msg;

    RRC_ASSERT(p_rrc_s1u_config_cell_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_s1u_config_cell_req_len(p_rrc_s1u_config_cell_req);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_S1U_CELL_CONFIG_REQ", src_module_id, dst_module_id);

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
    p_s1u_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1u_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_S1U_CELL_CONFIG_REQ, msg_api_length);

    /* Fill interface header */
    p_s1u_msg = p_s1u_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1u_msg, transaction_id, src_module_id,
        RRC_S1U_MODULE_ID, RRC_S1U_CELL_CONFIG_REQ, msg_length,cell_index);

    /* Fill RRC_S1U_CELL_CONFIG_REQ message */
    p_s1u_msg = p_s1u_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_s1u_config_cell_req(&p_s1u_msg, p_rrc_s1u_config_cell_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1u_msg - msg_api_length) != p_msg)
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
rrc_il_get_s1u_communication_info_len
(
    s1u_communication_info_t *p_s1u_communication_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1u_communication_info != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1u_communication_info->pdcpInstance);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_s1u_communication_info->pdcpRxPort);
    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_s1u_communication_info->pdcpIpAddr);

    return length;
}

static
rrc_return_et
rrc_il_compose_s1u_communication_info
(
    U8  **pp_buffer,
    s1u_communication_info_t *p_s1u_communication_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1u_communication_info != PNULL);

    /* This function composes s1u_communication_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1u_communication_info->pdcpInstance, "pdcpInstance");
    *pp_buffer += sizeof(p_s1u_communication_info->pdcpInstance);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1u_communication_info->pdcpRxPort, "pdcpRxPort");
    *pp_buffer += sizeof(p_s1u_communication_info->pdcpRxPort);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_s1u_communication_info->pdcpIpAddr); loop++)
        {
            rrc_cp_pack_U8(*pp_buffer, &p_s1u_communication_info->pdcpIpAddr[loop], "pdcpIpAddr[]");
            *pp_buffer += sizeof(U8);
        }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_il_get_rrc_s1u_config_cell_cnf_len
(
    rrc_s1u_config_cell_cnf_t *p_rrc_s1u_config_cell_cnf
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_rrc_s1u_config_cell_cnf != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_config_cell_cnf->cell_index);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_rrc_s1u_config_cell_cnf->response);

    return length;
}

rrc_return_et
rrc_il_compose_rrc_s1u_config_cell_cnf
(
    U8  **pp_buffer,
    rrc_s1u_config_cell_cnf_t *p_rrc_s1u_config_cell_cnf
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_rrc_s1u_config_cell_cnf != PNULL);

    /* This function composes rrc_s1u_config_cell_cnf */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_config_cell_cnf->cell_index > MAX_CELL_INDEX))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_config_cell_cnf->cell_index] should be less than"
            " or equal to MAX_CELL_INDEX. Incorrect value %u received.", p_rrc_s1u_config_cell_cnf->cell_index);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_config_cell_cnf->cell_index, "cell_index");
    *pp_buffer += sizeof(p_rrc_s1u_config_cell_cnf->cell_index);

    /* Check for correct range [H - higher boundary] */
    if ((p_rrc_s1u_config_cell_cnf->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_rrc_s1u_config_cell_cnf->response] should be less than"
            " or equal to 1. Incorrect value %u received.", p_rrc_s1u_config_cell_cnf->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_rrc_s1u_config_cell_cnf->response, "response");
    *pp_buffer += sizeof(p_rrc_s1u_config_cell_cnf->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1u_il_send_rrc_s1u_config_cell_cnf
*
*   DESCRIPTION:
*       This function constructs and sends RRC_S1U_CELL_CONFIG_CNF message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1u_il_send_rrc_s1u_config_cell_cnf
(
    rrc_s1u_config_cell_cnf_t  *p_rrc_s1u_config_cell_cnf,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1u_msg;

    RRC_ASSERT(p_rrc_s1u_config_cell_cnf != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_rrc_s1u_config_cell_cnf_len(p_rrc_s1u_config_cell_cnf);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):RRC_S1U_CELL_CONFIG_CNF", src_module_id, dst_module_id);

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
    p_s1u_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1u_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, RRC_S1U_CELL_CONFIG_CNF, msg_api_length);

    /* Fill interface header */
    p_s1u_msg = p_s1u_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1u_msg, transaction_id, src_module_id,
        RRC_S1U_MODULE_ID, RRC_S1U_CELL_CONFIG_CNF, msg_length,cell_index);

    /* Fill RRC_S1U_CELL_CONFIG_CNF message */
    p_s1u_msg = p_s1u_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_rrc_s1u_config_cell_cnf(&p_s1u_msg, p_rrc_s1u_config_cell_cnf))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1u_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dst_module_id);

    return RRC_SUCCESS;
}

