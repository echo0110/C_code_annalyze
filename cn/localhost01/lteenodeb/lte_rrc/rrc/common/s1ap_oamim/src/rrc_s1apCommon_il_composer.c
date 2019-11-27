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
 *  File Description : The file rrc_s1apCommon_il_composer.c contains the definitions 
 *                     of rrc-s1apCommon interface message composing functions.
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
#include "rrc_s1apCommon_il_composer.h"
#include "rrc_s1apCommon_intf.h"
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
rrc_get_ip_addr_len
(
    ip_addr_t *p_ip_addr
);

static
rrc_return_et
rrc_compose_ip_addr
(
    U8  **pp_buffer,
    ip_addr_t *p_ip_addr
);

static
rrc_length_t
rrc_get_ipv6_addr_len
(
    ipv6_addr_t *p_ipv6_addr
);

static
rrc_return_et
rrc_compose_ipv6_addr
(
    U8  **pp_buffer,
    ipv6_addr_t *p_ipv6_addr
);

static
rrc_length_t
rrc_get_mme_comm_info_len
(
    mme_comm_info_t *p_mme_comm_info
);

static
rrc_return_et
rrc_compose_mme_comm_info
(
    U8  **pp_buffer,
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

    RRC_ASSERT(p_s1ap_add_mme_req != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_s1ap_add_mme_req->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_s1ap_add_mme_req->num_mme);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_add_mme_req->num_mme < 1) || (p_s1ap_add_mme_req->num_mme > MAX_ADD_DYNAMIC_MME))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_add_mme_req->num_mme] should be in range [1 <= %d <= MAX_ADD_DYNAMIC_MME] !", p_s1ap_add_mme_req->num_mme);
        return RRC_FAILURE;
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

rrc_return_et
rrc_compose_s1ap_add_mme_req
(
    U8  **pp_buffer,
    s1ap_add_mme_req_t *p_s1ap_add_mme_req
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_add_mme_req != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1ap_add_mme_req->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_add_mme_req->bitmask);

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_s1ap_add_mme_req->num_mme < 1) || (p_s1ap_add_mme_req->num_mme > MAX_ADD_DYNAMIC_MME))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_add_mme_req->num_mme] should be in range [1 <= %d <= MAX_ADD_DYNAMIC_MME] !", p_s1ap_add_mme_req->num_mme);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_add_mme_req->num_mme, "num_mme");
    *pp_buffer += sizeof(p_s1ap_add_mme_req->num_mme);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        rrc_counter_t loop;
        for (loop = 0; loop < p_s1ap_add_mme_req->num_mme; loop++)
        {
            if (RRC_FAILURE == rrc_compose_mme_comm_info(pp_buffer, &p_s1ap_add_mme_req->mme_comm_info_arr[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apCommon_send_s1ap_add_mme_req
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_ADD_MME_REQ message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apCommon_send_s1ap_add_mme_req
(
    s1ap_add_mme_req_t  *p_s1ap_add_mme_req,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apCommon_msg;

    RRC_ASSERT(p_s1ap_add_mme_req != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_s1ap_add_mme_req_len(p_s1ap_add_mme_req);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_ADD_MME_REQ", src_module_id, dest_module_id);

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
    p_s1apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, S1AP_ADD_MME_REQ, msg_api_length);

    /* Fill interface header */
    p_s1apCommon_msg = p_s1apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apCommon_msg, transaction_id, src_module_id,
        dest_module_id, S1AP_ADD_MME_REQ, msg_length, cell_index);

    /* Fill S1AP_ADD_MME_REQ message */
    p_s1apCommon_msg = p_s1apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_s1ap_add_mme_req(&p_s1apCommon_msg, p_s1ap_add_mme_req))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apCommon_msg - msg_api_length) != p_msg)
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
rrc_get_ip_addr_len
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
rrc_compose_ip_addr
(
    U8  **pp_buffer,
    ip_addr_t *p_ip_addr
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_ip_addr != PNULL);

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
rrc_get_ipv6_addr_len
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
rrc_compose_ipv6_addr
(
    U8  **pp_buffer,
    ipv6_addr_t *p_ipv6_addr
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_ipv6_addr != PNULL);

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
rrc_get_mme_comm_info_len
(
    mme_comm_info_t *p_mme_comm_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_mme_comm_info != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_mme_comm_info->num_ip_addr);

    /* Check for correct range [H - higher boundary] */
    if ((p_mme_comm_info->num_ip_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->num_ip_addr] should be in range [%d <= MAX_NUM_IP_ADDR] !", p_mme_comm_info->num_ip_addr);
        return RRC_FAILURE;
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
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->num_ipv6_addr] should be in range [1 <= %d <= MAX_NUM_IP_ADDR] !", p_mme_comm_info->num_ipv6_addr);
        return RRC_FAILURE;
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
rrc_return_et
rrc_compose_mme_comm_info
(
    U8  **pp_buffer,
    mme_comm_info_t *p_mme_comm_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_mme_comm_info != PNULL);

    /* Check for correct range [H - higher boundary] */
    if ((p_mme_comm_info->num_ip_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->num_ip_addr] should be in range [%d <= MAX_NUM_IP_ADDR] !", p_mme_comm_info->num_ip_addr);
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
            if (RRC_FAILURE == rrc_compose_ip_addr(pp_buffer, &p_mme_comm_info->ip_addr[loop]))
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
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->num_streams] should be in range [2 <= %d <= 10] !", p_mme_comm_info->num_streams);
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
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->rel_of_mme] should be in range [%d <= 2] !", p_mme_comm_info->rel_of_mme);
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
        RRC_TRACE(RRC_WARNING, "Parameter [p_mme_comm_info->num_ipv6_addr] should be in range [1 <= %d <= MAX_NUM_IP_ADDR] !", p_mme_comm_info->num_ipv6_addr);
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
            if (RRC_FAILURE == rrc_compose_ipv6_addr(pp_buffer, &p_mme_comm_info->ipv6_addr[loop]))
            {
                return RRC_FAILURE;
            }
        }
    }
    }

    return RRC_SUCCESS;
}

rrc_length_t
rrc_get_s1ap_add_mme_res_len
(
    s1ap_add_mme_res_t *p_s1ap_add_mme_res
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_s1ap_add_mme_res != PNULL);

    /* Get length of parameter of basic type */
    length += sizeof(p_s1ap_add_mme_res->bitmask);
    /* Get length of parameter of basic type */
    length += sizeof(p_s1ap_add_mme_res->response);

    return length;
}

rrc_return_et
rrc_compose_s1ap_add_mme_res
(
    U8  **pp_buffer,
    s1ap_add_mme_res_t *p_s1ap_add_mme_res
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_s1ap_add_mme_res != PNULL);

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_s1ap_add_mme_res->bitmask, "bitmask");
    *pp_buffer += sizeof(p_s1ap_add_mme_res->bitmask);

    /* Check for correct range [H - higher boundary] */
    if ((p_s1ap_add_mme_res->response > 1))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_s1ap_add_mme_res->response] should be in range [%d <= 1] !", p_s1ap_add_mme_res->response);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_s1ap_add_mme_res->response, "response");
    *pp_buffer += sizeof(p_s1ap_add_mme_res->response);

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_s1apCommon_send_s1ap_add_mme_res
*
*   DESCRIPTION:
*       This function constructs and sends S1AP_ADD_MME_RES message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_s1apCommon_send_s1ap_add_mme_res
(
    s1ap_add_mme_res_t  *p_s1ap_add_mme_res,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1apCommon_msg;

    RRC_ASSERT(p_s1ap_add_mme_res != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_get_s1ap_add_mme_res_len(p_s1ap_add_mme_res);
    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }

    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_ADD_MME_RES", src_module_id, dest_module_id);

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
    p_s1apCommon_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1apCommon_msg, RRC_VERSION_ID, src_module_id,
        dest_module_id, S1AP_ADD_MME_RES, msg_api_length);

    /* Fill interface header */
    p_s1apCommon_msg = p_s1apCommon_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_s1apCommon_msg, transaction_id, src_module_id,
        dest_module_id, S1AP_ADD_MME_RES, msg_length, cell_index);

    /* Fill S1AP_ADD_MME_RES message */
    p_s1apCommon_msg = p_s1apCommon_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_compose_s1ap_add_mme_res(&p_s1apCommon_msg, p_s1ap_add_mme_res))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_s1apCommon_msg - msg_api_length) != p_msg)
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    /* Send message to */
    rrc_send_message(p_msg, dest_module_id);

    return RRC_SUCCESS;
}

#ifdef LINUX_PC_TEST
rrc_return_et
	rrc_compose_test_sctp_message
(
	U8** pp_buffer,
	test_sctp_message_t* p_test_sctp_message,
	const void * pmsg,
	U32 msglen
)
{
	rrc_cp_pack_U32(*pp_buffer, &p_test_sctp_message->ulSocketID, "ulSocketID");
	*pp_buffer += sizeof(p_test_sctp_message->ulSocketID);

	rrc_cp_pack_U16(*pp_buffer, &p_test_sctp_message->usStreamID, "usStreamID");
	*pp_buffer += sizeof(p_test_sctp_message->usStreamID);
	
	memcpy(*pp_buffer, pmsg, msglen);

	return RRC_SUCCESS;
}

#endif


