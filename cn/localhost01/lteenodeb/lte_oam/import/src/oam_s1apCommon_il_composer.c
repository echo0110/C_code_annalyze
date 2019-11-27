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
 *  File Description : The file oam_s1apCommon_il_composer.c contains the definitions 
 *                     of rrc-s1apCommon interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#include "oam_s1apCommon_il_composer.h"
#include "rrc_s1apCommon_intf.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))
static oam_error_code_et oam_error = NO_ERROR;


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
rrc_length_t
rrc_get_ip_addr_len
(
    ip_addr_t *p_ip_addr
);

static
oam_return_et
oam_compose_ip_addr
(
    UInt8  **pp_buffer,
    ip_addr_t *p_ip_addr
);

static
rrc_length_t
rrc_get_ipv6_addr_len
(
    ipv6_addr_t *p_ipv6_addr
);

static
oam_return_et
oam_compose_ipv6_addr
(
    UInt8  **pp_buffer,
    ipv6_addr_t *p_ipv6_addr
);

static
rrc_length_t
rrc_get_mme_comm_info_len
(
    mme_comm_info_t *p_mme_comm_info
);

static
oam_return_et
oam_compose_mme_comm_info
(
    UInt8  **pp_buffer,
    mme_comm_info_t *p_mme_comm_info
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_length_t
rrc_get_s1ap_add_mme_req_len
(
    s1ap_add_mme_req_t *p_s1ap_add_mme_req
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_s1ap_add_mme_req != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_s1ap_add_mme_req->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_s1ap_add_mme_req->num_mme);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_add_mme_req->num_mme < 1) || (p_s1ap_add_mme_req->num_mme > MAX_ADD_DYNAMIC_MME))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_s1ap_add_mme_req->num_mme] should be in range [1 <= %d <= MAX_ADD_DYNAMIC_MME] !", p_s1ap_add_mme_req->num_mme);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_add_mme_req->num_mme; loop++)
        {
            length += rrc_get_mme_comm_info_len(&p_s1ap_add_mme_req->mme_comm_info_arr[loop]);
        }
    }

    return length;
}

oam_return_et
oam_compose_s1ap_add_mme_req
(
    UInt8  **pp_buffer,
    s1ap_add_mme_req_t *p_s1ap_add_mme_req
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_s1ap_add_mme_req != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_s1ap_add_mme_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_add_mme_req->bitmask);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_add_mme_req->num_mme < 1) || (p_s1ap_add_mme_req->num_mme > MAX_ADD_DYNAMIC_MME))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_s1ap_add_mme_req->num_mme] should be in range [1 <= %d <= MAX_ADD_DYNAMIC_MME] !", p_s1ap_add_mme_req->num_mme);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_s1ap_add_mme_req->num_mme, "num_mme");
    *pp_buffer += sizeof(p_s1ap_add_mme_req->num_mme);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_add_mme_req->num_mme; loop++)
        {
            if (OAM_FAILURE == oam_compose_mme_comm_info(pp_buffer, &p_s1ap_add_mme_req->mme_comm_info_arr[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_s1apCommon_send_s1ap_add_mme_req
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_ADD_MME_REQ message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_s1apCommon_send_s1ap_add_mme_req
(
    s1ap_add_mme_req_t  *p_s1ap_add_mme_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dest_module_id,  /* Destination module identifier */
    UInt16                 transaction_id,  /* Interface transaction identifier */
    UInt8                  cell_index       /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_s1apCommon_msg;

    OAM_NULL_CHECK(p_s1ap_add_mme_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_s1ap_add_mme_req_len(p_s1ap_add_mme_req);
    if(msg_length == OAM_FAILURE)
    {
        return OAM_FAILURE;
    }

    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):S1AP_ADD_MME_REQ", src_module_id, dest_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_s1apCommon_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_s1apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, S1AP_ADD_MME_REQ, msg_api_length);

    /* Fill interface header */
    p_s1apCommon_msg = p_s1apCommon_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_s1apCommon_msg, transaction_id, src_module_id,
        dest_module_id, S1AP_ADD_MME_REQ, msg_length, cell_index);

    /* Fill S1AP_ADD_MME_REQ message */
    p_s1apCommon_msg = p_s1apCommon_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_compose_s1ap_add_mme_req(&p_s1apCommon_msg, p_s1ap_add_mme_req))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_s1apCommon_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dest_module_id);

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_get_ip_addr_len
(
    ip_addr_t *p_ip_addr
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_ip_addr != PNULL);

    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_ip_addr->ip_addr);

    return length;
}

static
oam_return_et
oam_compose_ip_addr
(
    UInt8  **pp_buffer,
    ip_addr_t *p_ip_addr
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_ip_addr != PNULL);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_ip_addr->ip_addr); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_ip_addr->ip_addr[loop], "ip_addr[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_get_ipv6_addr_len
(
    ipv6_addr_t *p_ipv6_addr
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_ipv6_addr != PNULL);

    /* Get length of OCTET_STRING FIXED of basic type elements */
    length += sizeof(p_ipv6_addr->ipv6_addr);

    return length;
}

static
oam_return_et
oam_compose_ipv6_addr
(
    UInt8  **pp_buffer,
    ipv6_addr_t *p_ipv6_addr
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_ipv6_addr != PNULL);

    /* Compose OCTET_STRING FIXED of basic type elements */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < ARRSIZE(p_ipv6_addr->ipv6_addr); loop++)
        {
            OAM_CP_PACK_UINT8(*pp_buffer, &p_ipv6_addr->ipv6_addr[loop], "ipv6_addr[]");
            *pp_buffer += sizeof(UInt8);
        }
    }

    return OAM_SUCCESS;
}

static
rrc_length_t
rrc_get_mme_comm_info_len
(
    mme_comm_info_t *p_mme_comm_info
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_mme_comm_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_mme_comm_info->num_ip_addr);

    /* Check for correct range [H - higher boundary] */
    if ((p_mme_comm_info->num_ip_addr > MAX_NUM_IP_ADDR))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_mme_comm_info->num_ip_addr] should be in range [%d <= MAX_NUM_IP_ADDR] !", p_mme_comm_info->num_ip_addr);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_mme_comm_info->num_ip_addr; loop++)
        {
            length += rrc_get_ip_addr_len(&p_mme_comm_info->ip_addr[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_mme_comm_info->port);
    /* Get length of parameter of basic type */
    length += sizeof(p_mme_comm_info->num_streams);
    /* Get length of parameter of basic type */
    length += sizeof(p_mme_comm_info->heart_beat_timer);
    /* Get length of parameter of basic type */
    length += sizeof(p_mme_comm_info->rel_of_mme);
    /* Get length of parameter of basic type */
    length += sizeof(p_mme_comm_info->bitmask);

    /* Optional element */
    if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {
    /* Get length of parameter of basic type */
    length += sizeof(p_mme_comm_info->num_ipv6_addr);
    }

    /* Optional element */
    if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_mme_comm_info->num_ipv6_addr < 1) || (p_mme_comm_info->num_ipv6_addr > MAX_NUM_IP_ADDR))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_mme_comm_info->num_ipv6_addr] should be in range [1 <= %d <= MAX_NUM_IP_ADDR] !", p_mme_comm_info->num_ipv6_addr);
        return OAM_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_mme_comm_info->num_ipv6_addr; loop++)
        {
            length += rrc_get_ipv6_addr_len(&p_mme_comm_info->ipv6_addr[loop]);
        }
    }
    }

    return length;
}

static
oam_return_et
oam_compose_mme_comm_info
(
    UInt8  **pp_buffer,
    mme_comm_info_t *p_mme_comm_info
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_mme_comm_info != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_mme_comm_info->num_ip_addr > MAX_NUM_IP_ADDR))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_mme_comm_info->num_ip_addr] should be in range [%d <= MAX_NUM_IP_ADDR] !", p_mme_comm_info->num_ip_addr);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_mme_comm_info->num_ip_addr, "num_ip_addr");
    *pp_buffer += sizeof(p_mme_comm_info->num_ip_addr);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_mme_comm_info->num_ip_addr; loop++)
        {
            if (OAM_FAILURE == oam_compose_ip_addr(pp_buffer, &p_mme_comm_info->ip_addr[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_mme_comm_info->port, "port");
    *pp_buffer += sizeof(p_mme_comm_info->port);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_mme_comm_info->num_streams < 2) || (p_mme_comm_info->num_streams > 10))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_mme_comm_info->num_streams] should be in range [2 <= %d <= 10] !", p_mme_comm_info->num_streams);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_mme_comm_info->num_streams, "num_streams");
    *pp_buffer += sizeof(p_mme_comm_info->num_streams);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT32(*pp_buffer, &p_mme_comm_info->heart_beat_timer, "heart_beat_timer");
    *pp_buffer += sizeof(p_mme_comm_info->heart_beat_timer);

    /* Check for correct range [H - higher boundary] */
    if ((p_mme_comm_info->rel_of_mme > 2))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_mme_comm_info->rel_of_mme] should be in range [%d <= 2] !", p_mme_comm_info->rel_of_mme);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_mme_comm_info->rel_of_mme, "rel_of_mme");
    *pp_buffer += sizeof(p_mme_comm_info->rel_of_mme);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_mme_comm_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_mme_comm_info->bitmask);

    /* Optional element */
    if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_mme_comm_info->num_ipv6_addr < 1) || (p_mme_comm_info->num_ipv6_addr > MAX_NUM_IP_ADDR))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_mme_comm_info->num_ipv6_addr] should be in range [1 <= %d <= MAX_NUM_IP_ADDR] !", p_mme_comm_info->num_ipv6_addr);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_mme_comm_info->num_ipv6_addr, "num_ipv6_addr");
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
            if (OAM_FAILURE == oam_compose_ipv6_addr(pp_buffer, &p_mme_comm_info->ipv6_addr[loop]))
            {
                return OAM_FAILURE;
            }
        }
    }
    }

    return OAM_SUCCESS;
}

rrc_length_t
rrc_get_s1ap_add_mme_res_len
(
    s1ap_add_mme_res_t *p_s1ap_add_mme_res
)
{
    rrc_length_t length = 0;

    OAM_NULL_CHECK(p_s1ap_add_mme_res != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_s1ap_add_mme_res->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_s1ap_add_mme_res->response);

    return length;
}

oam_return_et
oam_compose_s1ap_add_mme_res
(
    UInt8  **pp_buffer,
    s1ap_add_mme_res_t *p_s1ap_add_mme_res
)
{
    OAM_NULL_CHECK(pp_buffer != PNULL);
    OAM_NULL_CHECK(*pp_buffer != PNULL);
    OAM_NULL_CHECK(p_s1ap_add_mme_res != PNULL);

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT16(*pp_buffer, &p_s1ap_add_mme_res->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_add_mme_res->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_add_mme_res->response > 1))
    {
        OAM_LOG(OAM,OAM_WARNING, "Parameter [p_s1ap_add_mme_res->response] should be in range [%d <= 1] !", p_s1ap_add_mme_res->response);
        return OAM_FAILURE;
    }

    /* Compose parameter of basic type */
    OAM_CP_PACK_UINT8(*pp_buffer, &p_s1ap_add_mme_res->response, "response");
    *pp_buffer += sizeof(p_s1ap_add_mme_res->response);

    return OAM_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_s1apCommon_send_s1ap_add_mme_res
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_ADD_MME_RES message
*
*   RETURNS:
*       OAM_FAILURE     - Indicates failed message processing
*       OAM_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_return_et
oam_s1apCommon_send_s1ap_add_mme_res
(
    s1ap_add_mme_res_t  *p_s1ap_add_mme_res,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dest_module_id,  /* Destination module identifier */
    UInt16                 transaction_id,  /* Interface transaction identifier */
    UInt8                  cell_index       /* cell index */
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_s1apCommon_msg;

    OAM_NULL_CHECK(p_s1ap_add_mme_res != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_s1ap_add_mme_res_len(p_s1ap_add_mme_res);
    if(msg_length == OAM_FAILURE)
    {
        return OAM_FAILURE;
    }

    OAM_LOG(OAM,OAM_INFO, "Src(%u)->Dst(%u):S1AP_ADD_MME_RES", src_module_id, dest_module_id);

    msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = oam_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed"
                " Cannot send message");
        /* Not enough memory */
        return OAM_FAILURE;
    }

    oam_memset(p_msg, 0, msg_api_length);
    p_s1apCommon_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_s1apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, S1AP_ADD_MME_RES, msg_api_length);

    /* Fill interface header */
    p_s1apCommon_msg = p_s1apCommon_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_s1apCommon_msg, transaction_id, src_module_id,
        dest_module_id, S1AP_ADD_MME_RES, msg_length, cell_index);

    /* Fill S1AP_ADD_MME_RES message */
    p_s1apCommon_msg = p_s1apCommon_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    if (OAM_FAILURE == oam_compose_s1ap_add_mme_res(&p_s1apCommon_msg, p_s1ap_add_mme_res))
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    if((p_s1apCommon_msg - msg_api_length) != p_msg)
    {
        oam_mem_free(p_msg,&oam_error);

        return OAM_FAILURE;
    }

    /* Send message to */
    oam_rrm_send_message(p_msg, dest_module_id);

    return OAM_SUCCESS;
}

