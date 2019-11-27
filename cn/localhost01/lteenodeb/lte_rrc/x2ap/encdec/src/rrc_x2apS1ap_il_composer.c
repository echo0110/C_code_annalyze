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
 *  File Description : The file rrc_x2apS1ap_il_composer.c contains the definitions 
 *                     of rrc-x2apS1ap interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/

#include "rrc_cp_common.h"
#include "rrc_x2apS1ap_il_composer.h"
#include "rrc_x2apS1ap_intf.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"
#include "rrc_msg_mgmt.h"

#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))


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
rrc_return_et
rrc_il_compose_x2_ip_addr
(
    U8  **pp_buffer,
    x2_ip_addr_t *p_x2_ip_addr
);

static
rrc_length_t
rrc_il_get_x2_ipv6_addr_len
(
    x2_ipv6_addr_t *p_x2_ipv6_addr
);

static
rrc_return_et
rrc_il_compose_x2_ipv6_addr
(
    U8  **pp_buffer,
    x2_ipv6_addr_t *p_x2_ipv6_addr
);

static
rrc_length_t
rrc_il_get_x2_trans_layer_addrs_len
(
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs
);

static
rrc_return_et
rrc_il_compose_x2_trans_layer_addrs
(
    U8  **pp_buffer,
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs
);

static
rrc_length_t
rrc_il_get_x2_extend_trans_layer_addrs_len
(
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs
);

static
rrc_return_et
rrc_il_compose_x2_extend_trans_layer_addrs
(
    U8  **pp_buffer,
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs
);

static
rrc_length_t
rrc_il_get_x2_enb_comm_info_len
(
    x2_enb_comm_info_t *p_x2_enb_comm_info
);

static
rrc_return_et
rrc_il_compose_x2_enb_comm_info
(
    U8  **pp_buffer,
    x2_enb_comm_info_t *p_x2_enb_comm_info
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

rrc_length_t
rrc_il_get_x2ap_s1ap_comm_info_len
(
    x2ap_s1ap_comm_info_t *p_x2ap_s1ap_comm_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2ap_s1ap_comm_info != PNULL);

    /* Get length of IE */
    length += rrc_il_get_x2_enb_comm_info_len(&p_x2ap_s1ap_comm_info->local_enb_info);

    return length;
}

rrc_return_et
rrc_il_compose_x2ap_s1ap_comm_info
(
    U8  **pp_buffer,
    x2ap_s1ap_comm_info_t *p_x2ap_s1ap_comm_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2ap_s1ap_comm_info != PNULL);

    /* This function composes x2ap_s1ap_comm_info */
    

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_enb_comm_info(pp_buffer, &p_x2ap_s1ap_comm_info->local_enb_info))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_x2apS1ap_il_send_x2ap_s1ap_comm_info
*
*   DESCRIPTION:
*       This function constructs and sends X2AP_S1AP_COMM_INFO message
*
*   RETURNS:
*       RRC_FAILURE     - Indicates failed message processing
*       RRC_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
rrc_return_et
rrc_x2apS1ap_il_send_x2ap_s1ap_comm_info
(
    x2ap_s1ap_comm_info_t  *p_x2ap_s1ap_comm_info,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
)
{
    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2apS1ap_msg;

    RRC_ASSERT(p_x2ap_s1ap_comm_info != PNULL);

    SET_CELL_INDEX(cell_index);

    /* Get API length */
    msg_length = rrc_il_get_x2ap_s1ap_comm_info_len(p_x2ap_s1ap_comm_info);
    
    if(msg_length == RRC_FAILURE)
        {
            return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_S1AP_COMM_INFO", src_module_id, dst_module_id);

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
    p_x2apS1ap_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2apS1ap_msg, RRC_VERSION_ID, src_module_id,
        dst_module_id, X2AP_S1AP_COMM_INFO, msg_api_length);

    /* Fill interface header */
    p_x2apS1ap_msg = p_x2apS1ap_msg + RRC_API_HEADER_SIZE;
    rrc_construct_interface_api_header(p_x2apS1ap_msg, transaction_id, src_module_id,
        RRC_X2APS1AP_MODULE_ID, X2AP_S1AP_COMM_INFO, msg_length,cell_index);

    /* Fill X2AP_S1AP_COMM_INFO message */
    p_x2apS1ap_msg = p_x2apS1ap_msg + RRC_INTERFACE_API_HEADER_SIZE;
    if (RRC_FAILURE == rrc_il_compose_x2ap_s1ap_comm_info(&p_x2apS1ap_msg, p_x2ap_s1ap_comm_info))
    {
        rrc_msg_mem_free(p_msg);

        return RRC_FAILURE;
    }

    if((p_x2apS1ap_msg - msg_api_length) != p_msg)
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
rrc_il_get_x2_ip_addr_len
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
rrc_il_compose_x2_ip_addr
(
    U8  **pp_buffer,
    x2_ip_addr_t *p_x2_ip_addr
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2_ip_addr != PNULL);

    /* This function composes x2_ip_addr */
    

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
rrc_il_get_x2_ipv6_addr_len
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
rrc_il_compose_x2_ipv6_addr
(
    U8  **pp_buffer,
    x2_ipv6_addr_t *p_x2_ipv6_addr
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2_ipv6_addr != PNULL);

    /* This function composes x2_ipv6_addr */
    

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
rrc_il_get_x2_trans_layer_addrs_len
(
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2_trans_layer_addrs != PNULL);
    /* Get length of parameter of basic type */
    length += (rrc_length_t)sizeof(p_x2_trans_layer_addrs->data_length);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_x2_trans_layer_addrs->data_length * sizeof(p_x2_trans_layer_addrs->data[0]));

    return length;
}

static
rrc_return_et
rrc_il_compose_x2_trans_layer_addrs
(
    U8  **pp_buffer,
    x2_trans_layer_addrs_t *p_x2_trans_layer_addrs
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2_trans_layer_addrs != PNULL);

    /* This function composes x2_trans_layer_addrs */
    

    /* Check for correct range [H - higher boundary] */
    if ((p_x2_trans_layer_addrs->data_length > RRC_MAX_IP_ADDR_LEN_IPV6))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_trans_layer_addrs->data_length] should be less than"
            " or equal to RRC_MAX_IP_ADDR_LEN_IPV6. Incorrect value %u received.", p_x2_trans_layer_addrs->data_length);
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
rrc_il_get_x2_extend_trans_layer_addrs_len
(
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2_extend_trans_layer_addrs != PNULL);
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
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_extend_trans_layer_addrs->num_tlas] should be in range "
            "1 to MAX_NUM_ENB_X2AP_GTP_TLAS. Incorrect value %d received.", p_x2_extend_trans_layer_addrs->num_tlas);
        return RRC_FAILURE;
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
rrc_return_et
rrc_il_compose_x2_extend_trans_layer_addrs
(
    U8  **pp_buffer,
    x2_extend_trans_layer_addrs_t *p_x2_extend_trans_layer_addrs
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2_extend_trans_layer_addrs != PNULL);

    /* This function composes x2_extend_trans_layer_addrs */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U16(*pp_buffer, &p_x2_extend_trans_layer_addrs->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2_extend_trans_layer_addrs->bitmask);

    /* Optional element */
    if(p_x2_extend_trans_layer_addrs->bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_IPSEC_ADDRS_PRESENT)
    {

    /* Compose IE */
    if (RRC_FAILURE == rrc_il_compose_x2_trans_layer_addrs(pp_buffer, &p_x2_extend_trans_layer_addrs->ipsec_trans_layer_addrs))
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
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_extend_trans_layer_addrs->num_tlas] should be in range "
            "1 to MAX_NUM_ENB_X2AP_GTP_TLAS. Incorrect value %u received.", p_x2_extend_trans_layer_addrs->num_tlas);
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
            if (RRC_FAILURE == rrc_il_compose_x2_trans_layer_addrs(pp_buffer, &p_x2_extend_trans_layer_addrs->gtp_trans_layer_addrs[loop]))
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
rrc_il_get_x2_enb_comm_info_len
(
    x2_enb_comm_info_t *p_x2_enb_comm_info
)
{
    rrc_length_t length = 0;

    RRC_ASSERT(p_x2_enb_comm_info != PNULL);
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
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->num_ip_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %d received.", p_x2_enb_comm_info->num_ip_addr);
        return RRC_FAILURE;
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
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->num_ipv6_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %d received.", p_x2_enb_comm_info->num_ipv6_addr);
        return RRC_FAILURE;
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
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->num_tlas] should be in range "
            "1 to MAX_NUM_ENB_X2AP_EXT_TLAS. Incorrect value %d received.", p_x2_enb_comm_info->num_tlas);
        return RRC_FAILURE;
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
rrc_return_et
rrc_il_compose_x2_enb_comm_info
(
    U8  **pp_buffer,
    x2_enb_comm_info_t *p_x2_enb_comm_info
)
{
    RRC_ASSERT(pp_buffer != PNULL);
    RRC_ASSERT(*pp_buffer != PNULL);
    RRC_ASSERT(p_x2_enb_comm_info != PNULL);

    /* This function composes x2_enb_comm_info */
    

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2_enb_comm_info->bitmask, "bitmask");
    *pp_buffer += sizeof(p_x2_enb_comm_info->bitmask);

    /* Optional element */
    if(p_x2_enb_comm_info->bitmask & X2_ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT)
    {

    /* Check for correct range [B - both higher and lower boundaries] */
    if ((p_x2_enb_comm_info->num_ip_addr < 1) || (p_x2_enb_comm_info->num_ip_addr > MAX_NUM_IP_ADDR))
    {
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->num_ip_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %u received.", p_x2_enb_comm_info->num_ip_addr);
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
            if (RRC_FAILURE == rrc_il_compose_x2_ip_addr(pp_buffer, &p_x2_enb_comm_info->ip_addr[loop]))
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
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->num_ipv6_addr] should be in range "
            "1 to MAX_NUM_IP_ADDR. Incorrect value %u received.", p_x2_enb_comm_info->num_ipv6_addr);
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
            if (RRC_FAILURE == rrc_il_compose_x2_ipv6_addr(pp_buffer, &p_x2_enb_comm_info->ipv6_addr[loop]))
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
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->num_tlas] should be in range "
            "1 to MAX_NUM_ENB_X2AP_EXT_TLAS. Incorrect value %u received.", p_x2_enb_comm_info->num_tlas);
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
            if (RRC_FAILURE == rrc_il_compose_x2_extend_trans_layer_addrs(pp_buffer, &p_x2_enb_comm_info->extend_trans_layer_addrs[loop]))
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
        RRC_TRACE(RRC_WARNING, "Parameter [p_x2_enb_comm_info->default_sgnb] should be less than"
            " or equal to 1. Incorrect value %u received.", p_x2_enb_comm_info->default_sgnb);
        return RRC_FAILURE;
    }

    /* Compose parameter of basic type */
    rrc_cp_pack_U8(*pp_buffer, &p_x2_enb_comm_info->default_sgnb, "default_sgnb");
    *pp_buffer += sizeof(p_x2_enb_comm_info->default_sgnb);
    }

    return RRC_SUCCESS;
}

